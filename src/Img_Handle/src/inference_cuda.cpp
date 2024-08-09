#include "inference_cuda.h"
Logger gLogger;
const int kOutputSize = kMaxNumOutputBbox * sizeof(Detection) / sizeof(float) + 1;
void Inference_cuda::Inference_cuda_reset(const std::string &wtsName ,const std::string &engineName ,const std::string &classes_txt_file,int k_num_class, float gd = 0.33,float gw = 0.25)
{
    this->engine_name = engineName;
    this->wts_name = wtsName;
    this->classes_path = classes_txt_file;
    loadClassesFromFile();
    cudaSetDevice(kGpuId);//设置cuda
    int max_channels = 1024;
    this->k_num_class = k_num_class;
    // std::cout<<"kNumClass:"<<this->k_num_class<<std::endl;
    sub_type = "";
    // if (!wts_name.empty()) {
    //     std::cout<<wts_name<<std::endl;
    //     std::cout<<engine_name<<std::endl;
    //     serialize_engine(wts_name, engine_name, is_p, sub_type, gd, gw, max_channels);
    // }
    // Deserialize the engine from file
    deserialize_engine(this->engine_name, &this->runtime, &this->engine, &this->context);
    CUDA_CHECK(cudaStreamCreate(&this->stream));
    if(this->k_num_class == 1)
    {
        cuda_preprocess_init_car(kMaxInputImageSize);
    }
    else
    {
        cuda_preprocess_init_armor(kMaxInputImageSize);
    }
    // cuda_preprocess_init(kMaxInputImageSize);
    auto out_dims = engine->getBindingDimensions(1);
    model_bboxes = out_dims.d[0];
    prepare_buffer(engine, &device_buffers[0], &device_buffers[1], &output_buffer_host, &decode_ptr_host,
                   &decode_ptr_device, cuda_post_process);
    // std::cout<<"Inference_cuda init end!"<<std::endl;

}
Inference_cuda::Inference_cuda()
{
}
Inference_cuda::~Inference_cuda()
{

    // Release stream and buffers
    cudaStreamDestroy(stream);
    CUDA_CHECK(cudaFree(device_buffers[0]));
    CUDA_CHECK(cudaFree(device_buffers[1]));
    CUDA_CHECK(cudaFree(decode_ptr_device));
    delete[] decode_ptr_host;
    delete[] output_buffer_host;
    // cuda_preprocess_destroy();
    if(k_num_class == 1)
    {
        cuda_preprocess_destroy_car();
    }
    else
    {
        cuda_preprocess_destroy_armor();
    }
    // Destroy the engine
    delete context;
    delete engine;
    delete runtime;

}


void Inference_cuda::serialize_engine(std::string& wts_name, std::string& engine_name, int& is_p, std::string& sub_type, float& gd,
                      float& gw, int& max_channels) {
    IBuilder* builder = createInferBuilder(gLogger);
    IBuilderConfig* config = builder->createBuilderConfig();
    IHostMemory* serialized_engine = nullptr;

    if (is_p == 6) {
        serialized_engine = buildEngineYolov8DetP6(builder, config, DataType::kFLOAT, wts_name, gd, gw, max_channels);
    } else if (is_p == 2) {
        serialized_engine = buildEngineYolov8DetP2(builder, config, DataType::kFLOAT, wts_name, gd, gw, max_channels);
    } else {
        serialized_engine = buildEngineYolov8Det(builder, config, DataType::kFLOAT, wts_name, gd, gw, max_channels);
    }

    assert(serialized_engine);
    std::ofstream p(engine_name, std::ios::binary);
    if (!p) {
        std::cout << "could not open plan output file" << std::endl;
        assert(false);
    }
    p.write(reinterpret_cast<const char*>(serialized_engine->data()), serialized_engine->size());

    delete serialized_engine;
    delete config;
    delete builder;
}

void Inference_cuda::deserialize_engine(std::string& engine_name, IRuntime** runtime, ICudaEngine** engine,
                        IExecutionContext** context) {
    std::ifstream file(engine_name, std::ios::binary);
    if (!file.good()) {
        std::cerr << "read " << engine_name << " error!" << std::endl;
        assert(false);
    }
    size_t size = 0;
    file.seekg(0, file.end);
    size = file.tellg();
    file.seekg(0, file.beg);
    char* serialized_engine = new char[size];
    assert(serialized_engine);
    file.read(serialized_engine, size);
    file.close();

    *runtime = createInferRuntime(gLogger);
    assert(*runtime);
    *engine = (*runtime)->deserializeCudaEngine(serialized_engine, size);
    assert(*engine);
    *context = (*engine)->createExecutionContext();
    assert(*context);
    delete[] serialized_engine;
}

void Inference_cuda::prepare_buffer(ICudaEngine* engine, float** input_buffer_device, float** output_buffer_device,
                    float** output_buffer_host, float** decode_ptr_host, float** decode_ptr_device,
                    std::string cuda_post_process) {
    assert(engine->getNbBindings() == 2);
    // In order to bind the buffers, we need to know the names of the input and output tensors.
    // Note that indices are guaranteed to be less than IEngine::getNbBindings()
    const int inputIndex = engine->getBindingIndex(kInputTensorName);
    const int outputIndex = engine->getBindingIndex(kOutputTensorName);
    assert(inputIndex == 0);
    assert(outputIndex == 1);
    // Create GPU buffers on device
    CUDA_CHECK(cudaMalloc((void**)input_buffer_device, kBatchSize * 3 * kInputH * kInputW * sizeof(float)));
    CUDA_CHECK(cudaMalloc((void**)output_buffer_device, kBatchSize * kOutputSize * sizeof(float)));
    if (cuda_post_process == "c") {
        *output_buffer_host = new float[kBatchSize * kOutputSize];
    } else if (cuda_post_process == "g") {
        if (kBatchSize > 1) {
            std::cerr << "Do not yet support GPU post processing for multiple batches" << std::endl;
            exit(0);
        }
        // Allocate memory for decode_ptr_host and copy to device
        *decode_ptr_host = new float[1 + kMaxNumOutputBbox * bbox_element];
        CUDA_CHECK(cudaMalloc((void**)decode_ptr_device, sizeof(float) * (1 + kMaxNumOutputBbox * bbox_element)));
    }
}
void Inference_cuda::infer(IExecutionContext& context, cudaStream_t& stream, void **buffers, float* output, int batchsize,
           float* decode_ptr_host, float* decode_ptr_device, int model_bboxes, std::string cuda_post_process) {
    // infer on the batch asynchronously, and DMA output back to host
    auto start = std::chrono::system_clock::now();
    context.enqueue(batchsize, buffers, stream, nullptr);
    if (cuda_post_process == "c") {
        CUDA_CHECK(cudaMemcpyAsync(output, buffers[1], batchsize * kOutputSize * sizeof(float), cudaMemcpyDeviceToHost,
                                   stream));
        auto end = std::chrono::system_clock::now();
        // std::cout << "inference time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        //           << "ms" << std::endl;
    } else if (cuda_post_process == "g") {
        CUDA_CHECK(
                cudaMemsetAsync(decode_ptr_device, 0, sizeof(float) * (1 + kMaxNumOutputBbox * bbox_element), stream));
        cuda_decode((float*)buffers[1], model_bboxes, kConfThresh, decode_ptr_device, kMaxNumOutputBbox, stream);
        cuda_nms(decode_ptr_device, kNmsThresh, kMaxNumOutputBbox, stream);  //cuda nms
        CUDA_CHECK(cudaMemcpyAsync(decode_ptr_host, decode_ptr_device,
                                   sizeof(float) * (1 + kMaxNumOutputBbox * bbox_element), cudaMemcpyDeviceToHost,
                                   stream));
        auto end = std::chrono::system_clock::now();
        std::cout << "inference and gpu postprocess time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    }

    CUDA_CHECK(cudaStreamSynchronize(stream));
}

void Inference_cuda::loadClassesFromFile()
{
    std::ifstream inputFile(classes_path);
    if (inputFile.is_open())
    {
        std::string classLine;
        while (std::getline(inputFile, classLine))
            classes.push_back(classLine);
        inputFile.close();
    }
}


std::vector<Detection_output> Inference_cuda::runInferenceCuda(const cv::Mat &input)
{
    
    cv::Mat img;
    input.copyTo(img);
    if (this->k_num_class == 1)
    {
        cuda_preprocess_car(img.ptr(), img.cols, img.rows, device_buffers[0], 640, 640, stream);
    }else{
        cuda_preprocess_armor(img.ptr(), img.cols, img.rows, device_buffers[0], 640, 640, stream);
    }
    infer(*context, stream, (void**)device_buffers, output_buffer_host, BATCH_SIZE, decode_ptr_host,decode_ptr_device, model_bboxes, cuda_post_process);
    std::vector<Detection> res_car;
    // batch_process(res_batch, decode_ptr_host, img_batch.size(), bbox_element, img_batch);
    int count = static_cast<int>(*decode_ptr_host);
    process_decode_ptr_host(res_car,decode_ptr_host,bbox_element,img,count); 
    std::vector<Detection_output> detections{};
    for (unsigned long i = 0; i < res_car.size(); ++i)
    {
        int idx = res_car[i].class_id;

        Detection_output result;
        result.class_id = idx;
        result.confidence = res_car[i].conf;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(100, 255);
        result.color = cv::Scalar(dis(gen),
                                  dis(gen),
                                  dis(gen));

        result.className = classes[result.class_id];
        cv::Rect r = get_rect(img, res_car[i].bbox);
        result.box = r;
        detections.push_back(result);
    }

    return detections;
}
