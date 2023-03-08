#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include<iostream>
#include <cstdio>

cv::Mat toGreyScale(cv::Mat image);
int(*(getArray)(std::string mode, std::string direction))[3];
cv::Mat masking(cv::Mat image, int mask[3][3]);
cv::Mat CannyEdgeDetection(cv::Mat image,int segma,int lowThreshold,int highThreshold,int KernalSize);
void non_max_suppression(cv::Mat& magnitude,cv::Mat& direction, cv::Mat& result);
#endif // EDGEDETECTION_H
