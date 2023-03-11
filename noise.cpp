#include "noise.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>
#include <random>

using namespace std;
using namespace cv;

/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double standard diviation
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
 *
*/
void Add_gaussian_Noise(Mat& img, double mean, double sigma)
{
//    Mat clonedImg = img.clone();
//    RNG rng;
//    rng.fill(clonedImg, RNG::NORMAL, mean, sigma);
//    add(img, clonedImg, img);

    Mat noise(img.size(),img.type());
    randn(noise, mean, sigma); //mean and variance
    img += noise;
}

/*
 * function to add salt and paper noise to an image
 * params : CV::Mat object type image
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
 *
*/
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

/*
 * function to average noise to an image
 * params : CV::Mat object type image0
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
 *
*/
void add_uniform_noise(Mat& img)
{
    // Convert image to the double
    Mat out_img;
    Mat norm_img;
    img.convertTo(img, CV_64FC1);
    norm_img = img / 255;

    // Generate uniform noise
    Mat noise(norm_img.size(), CV_64FC1);
    randu(noise, 0, 1);

    out_img = norm_img + noise;

    // Convert double to the 8-bit unsigned
    normalize(out_img, out_img, 0.0, 1.0, cv::NORM_MINMAX, CV_64FC1);
    out_img *= 255;
    out_img.convertTo(out_img, CV_8UC1);
    img=out_img;


}
