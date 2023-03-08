#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include<iostream>
#include <cstdio>
cv::Mat plot_histogram(cv::Mat histogram,int r=0,int g=0,int b=0);
cv::Mat calc_histogram(cv::Mat image);
cv::Mat DistributionCal(cv::Mat hist);
std::tuple<cv::Mat, cv::Mat,cv::Mat> splitChannels(cv::Mat image );
std::tuple<cv::Mat, cv::Mat,cv::Mat> plot_rgb_distribution_function(cv::Mat image,std::string mode) ;
using namespace std;
using namespace cv;
#endif // HISTOGRAM_H
