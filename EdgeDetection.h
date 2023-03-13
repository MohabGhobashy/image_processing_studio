#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include<iostream>
#include <cstdio>

using namespace std;
using namespace cv;

Mat toGreyScale(Mat image);
int(*(getArray)(std::string mode, std::string direction))[3];
Mat masking(Mat image, int mask[3][3]);
Mat CannyEdgeDetection(Mat image,int segma,int lowThreshold,int highThreshold,int KernalSize);
void non_max_suppression(Mat& magnitude,Mat& direction, Mat& result);

#endif // EDGEDETECTION_H
