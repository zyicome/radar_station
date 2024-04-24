#pragma once

#include <opencv2/opencv.hpp>
#include "NvInfer.h"
#include "types.h"
#include <map>


void cuda_preprocess_init_car(int max_image_size);
void cuda_preprocess_init_armor(int max_image_size);
void cuda_preprocess_destroy_car();
void cuda_preprocess_destroy_armor();
void cuda_preprocess_car(uint8_t *src, int src_width, int src_height, float *dst, int dst_width, int dst_height, cudaStream_t stream);
void cuda_preprocess_armor(uint8_t *src, int src_width, int src_height, float *dst, int dst_width, int dst_height, cudaStream_t stream);

void cuda_batch_preprocess(std::vector<cv::Mat> &img_batch, float *dst, int dst_width, int dst_height, cudaStream_t stream);

