#ifndef THRESHOLD_H
#define THRESHOLD_H
#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <string>
#include <QPair>

void convertToGrayscale(cv::Mat& input_image, cv::Mat& output_image);
void Threshold(cv::Mat& input_image, cv::Mat& output_image,int threshold );
#endif // THRESHOLD_H
