#ifndef FREQUENCYFILTERS_H
#define FREQUENCYFILTERS_H
#include <iostream>
#include <string>
#include <cmath>
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

Mat adjustSize(Mat& img);
void fourierShift(Mat& Img);
Mat calcDFT(Mat& img);
Mat createFilter(Mat& complex_img, float distance, string filterType);
void ifft(Mat& complex_img);
Mat applyFilter(Mat& complex_img, Mat& filter);

#endif // FREQUENCYFILTERS_H
