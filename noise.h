#ifndef NOISE_H
#define NOISE_H
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>

using namespace std;
using namespace cv;

void Add_salt_pepper_Noise(Mat& srcArr, float pa = 0.1, float pb = 0.05);
void Add_gaussian_Noise(Mat& srcArr, double mean =15, double sigma=15);
void add_uniform_noise(cv::Mat& img);

#endif // NOISE_H
