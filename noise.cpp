#include "noise.h"
#include <iostream>
#include <opencv2/core.hpp>
#include<opencv2\highgui.hpp>

using namespace std;
using namespace cv;

void Add_salt_pepper_Noise(Mat& srcArr, float pa, float pb)
{
    RNG rng; // rand number generate
    int amount1 = srcArr.rows * srcArr.cols * pa;
    int amount2 = srcArr.rows * srcArr.cols * pb;
    for (int counter = 0; counter < amount1; ++counter)
    {
        srcArr.at<uchar>(rng.uniform(0, srcArr.rows), rng.uniform(0, srcArr.cols)) = 0;
    }
    for (int counter = 0; counter < amount2; ++counter)
    {
        srcArr.at<uchar>(rng.uniform(0, srcArr.rows), rng.uniform(0, srcArr.cols)) = 255;
    }
}

void Add_gaussian_Noise(Mat& srcArr, double mean, double sigma)
{
    Mat NoiseArr = srcArr.clone();
    RNG rng;
    rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);
    add(srcArr, NoiseArr, srcArr);
}

Mat add_uniform_noise(cv::Mat& img)
{
    // Convert image to the double
    Mat out_img;
    Mat norm_img;
    img.convertTo(norm_img, CV_64FC3);
    norm_img = norm_img / 255;

    // Generate uniform noise
    Mat noise(norm_img.size(), CV_64FC3);
    randu(noise, 0, 1);

    out_img = norm_img + noise;

    // Convert double to the 8-bit unsigned
    normalize(out_img, out_img, 0.0, 1.0, cv::NORM_MINMAX, CV_64FC3);
    out_img *= 255;
    out_img.convertTo(out_img, CV_8UC3);

    return out_img;
}
