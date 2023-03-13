#ifndef THRESHOLD_H
#define THRESHOLD_H
#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <iostream>
#include <string>
#include <QPair>

void convertToGrayscale(cv::Mat& input_image, cv::Mat& output_image);
void Threshold(cv::Mat& input_image, cv::Mat& output_image,int threshold );
void localThreshold( cv::Mat& input, cv::Mat& output, int blockSize, double C);

#endif // THRESHOLD_H
