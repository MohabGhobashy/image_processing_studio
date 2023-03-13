#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include<iostream>
#include <cstdio>

using namespace std;
using namespace cv;

Mat plot_histogram(Mat histogram,int r=0,int g=0,int b=0);
Mat calc_histogram(Mat image);
Mat DistributionCal(Mat hist);
tuple<Mat, Mat,Mat> splitChannels(Mat image );
tuple<Mat, Mat,Mat> plot_rgb_distribution_function(Mat image,string mode) ;

#endif // HISTOGRAM_H
