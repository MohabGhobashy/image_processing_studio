#ifndef NOISEFILTERS_H
#define NOISEFILTERS_H
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>

using namespace std;
using namespace cv;

Mat padding(Mat img, int k_width, int k_height);
void insertionSort(int arr[], int n);
Mat define_kernel_box(int k_width, int k_height);
Mat define_kernel_gaussian(int k_width, int k_height);
void boxFilter(Mat scr, Mat& dst, int k_w, int k_h);
void gaussianFilter(Mat scr, Mat& dst, int k_w, int k_h, int sigma);
void medianFilter(Mat& image);


#endif // NOISEFILTERS_H
