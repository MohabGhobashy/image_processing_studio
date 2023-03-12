#include "noise.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>
#include <random>

using namespace std;
using namespace cv;


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
 *
*/
void Add_gaussian_Noise(Mat& img, double mean, double sigma)
{
    Mat noise(img.size(),img.type());
    randn(noise, mean, sigma);
    img += noise;
}


/*
 * function to add salt and paper noise to an image
 * params : CV::Mat object type image
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
 *
*/
void Add_salt_pepper_Noise(Mat& img, float pSalt, float pPaper)
{
    RNG rng;
    // Calculating the percentage of pixels user want to convert to salt or paper
    int amount1 = img.rows * img.cols * (pSalt/100);
    int amount2 = img.rows * img.cols * (pPaper/100);

    // Choosing random index -in the boundary of the rows and columns of course- and then put to 0 or 255
    for (int counter = 0; counter < amount1; ++counter)
        img.at<uchar>(rng.uniform(0, img.rows), rng.uniform(0, img.cols)) = 255;

    for (int counter = 0; counter < amount2; ++counter)
        img.at<uchar>(rng.uniform(0, img.rows), rng.uniform(0, img.cols)) = 0;
}


/*
 * function to average noise to an image
 * params : CV::Mat object type image
 * first we clone the image then we create a uniform noise and finally we add this noise to the image
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

    // Adding noise to the image
    out_img = norm_img + noise;

    // Convert double to the 8-bit unsigned
    normalize(out_img, out_img, 0.0, 1.0, cv::NORM_MINMAX, CV_64FC1);
    out_img *= 255;
    out_img.convertTo(out_img, CV_8UC1);
    img = out_img;
}
