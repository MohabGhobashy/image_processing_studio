#ifndef NOISE_H
#define NOISE_H
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>

using namespace std;
using namespace cv;

void Add_salt_pepper_Noise(Mat& srcArr, float pa, float pb);
void Add_gaussian_Noise(Mat& srcArr, double mean, double sigma);
Mat add_uniform_noise(cv::Mat& img);

#endif // NOISE_H
