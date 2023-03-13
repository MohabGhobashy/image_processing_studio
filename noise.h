#ifndef NOISE_H
#define NOISE_H
#include <random>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>

using namespace std;
using namespace cv;

void Add_salt_pepper_Noise(Mat& img, float pSalt, float pPaper);
void Add_gaussian_Noise(Mat& img, double mean, double sigma);
void add_uniform_noise(Mat& img);

#endif // NOISE_H
