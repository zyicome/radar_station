#ifndef INFERENCE_CUDA
#define INFERENCE_CUDA

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include "cuda_utils.h"
#include "logging.h"
#include "model.h"
#include "config.h"
#include "postprocess.h"
#include "preprocess.h"
#include "utils.h"

using namespace nvinfer1;
#define BATCH_SIZE 1

struct Detection_output
{
    int class_id{0};
    std::string className{};
    float confidence{0.0};
    cv::Scalar color{};
    cv::Rect box{};
};

class Inference_cuda
{
public:
    Inference_cuda(const std::string &wtsName,const std::string &engineName,const std::string &classes_txt_file,int k_num_class, float gd,float gw);
    Inference_cuda();
    ~Inference_cuda();
    std::vector<Detection_output> runInferenceCuda(const cv::Mat &input);
    std::vector<std::string> classes;
private:
    int is_p=0;
    std::string sub_type;//yolov8模型相关
    int k_num_class;
    std::string wts_name;
    std::string engine_name;
    std::string classes_path; 
    std::string cuda_post_process = "g";
    int model_bboxes;
    
    IRuntime* runtime = nullptr;
    ICudaEngine* engine = nullptr;
    IExecutionContext* context = nullptr;
    cudaStream_t stream;
    // Prepare cpu and gpu buffers
    
    float* device_buffers[2];
    float* output_buffer_host = nullptr;
    float* decode_ptr_host = nullptr;
    float* decode_ptr_device = nullptr;
    void serialize_engine(std::string& wts_name, std::string& engine_name, int& is_p, std::string& sub_type, float& gd,
                      float& gw, int& max_channels);
    void deserialize_engine(std::string& engine_name, IRuntime** runtime, ICudaEngine** engine,
                        IExecutionContext** context);
    void prepare_buffer(ICudaEngine* engine, float** input_buffer_device, float** output_buffer_device,
                    float** output_buffer_host, float** decode_ptr_host, float** decode_ptr_device,
                    std::string cuda_post_process);
    void infer(IExecutionContext& context, cudaStream_t& stream, void **buffers, float* output, int batchsize,
           float* decode_ptr_host, float* decode_ptr_device, int model_bboxes, std::string cuda_post_process);
    void loadClassesFromFile();
};

#endif INFERENCE_CUDA