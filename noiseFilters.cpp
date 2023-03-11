#include "noiseFilters.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>
#include <QDebug>
#include <EdgeDetection.h>

using namespace std;
using namespace cv;

/*
 * helper function used for median filter to sort elements
 * params : array and its size
 * insertion sort algorithm implementation
*/
void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

/*
 * helper function used for padding the images with 2 rows and 2 columns of zeros
 * params : CV::Mat object image, kernal width and kernal height
 * first we clone the image then we create a uniform noise and finally we add this noise to the image
*/
Mat padding(Mat img, int k_width, int k_height)
{
    img.convertTo(img, CV_64FC1); // converting the image pixels to 64 bits
    int pad_rows, pad_cols;
    pad_rows = (k_height - 1) / 2; // = 1
    pad_cols = (k_width - 1) / 2;  // = 1
    Mat pad_image(Size(img.cols + 2 * pad_cols, img.rows + 2 * pad_rows), CV_64FC1, Scalar(0)); // resizing the image with the padding

    Mat rgbchannel[3];
    split(pad_image, rgbchannel);
    pad_image = rgbchannel[0];

    img.copyTo(pad_image(Rect(pad_cols, pad_rows, img.cols, img.rows)));
    // creating new padded image
    return pad_image;
}

// function to define kernels for gaussian convolution
Mat define_kernel_gaussian(int k_width, int k_height, int sigma)
{
    if(k_width%2==0){
        k_width=k_width+1;
        k_height=k_height+1;
    }
    const double pi = M_PI;
        int pad_rows = (k_height - 1) / 2;
        int pad_cols = (k_width - 1) / 2;

        // Initialize kernel matrix
        Mat kernel(k_height, k_width, CV_64FC1);

        // Calculate Gaussian function for each element of kernel matrix
        for (int i = -pad_rows; i <= pad_rows; i++)
            for (int j = -pad_cols; j <= pad_cols; j++)
                // Calculate Gaussian function value with corrected order of operations
                kernel.at<double>(i + pad_rows, j + pad_cols) = (1 / (2.0 * pi * sigma * sigma)) * exp(-((i * i + j * j) / (sigma * sigma)));



        // Normalize kernel to ensure that it sums to 1
        kernel = kernel / sum(kernel);

        // Return generated kernel
        return kernel;
}

// function to implement convolution of gaussian filter
void gaussianFilter(Mat& img, int k_w, int k_h, int sigma)
{
    if(k_w%2==0){
        k_w=k_w+1;
        k_h=k_h+1;
    }
    Mat pad_img, kernel;
    pad_img = padding(img, k_w, k_h);
    //copyMakeBorder(img,pad_img,1,1,1,1,BORDER_CONSTANT,Scalar(0));
    kernel = define_kernel_gaussian(k_w, k_h, sigma);

    Mat output = Mat::zeros(img.size(), CV_64FC1);

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            output.at<double>(i, j) = sum(kernel.mul(pad_img(Rect(j, i, k_w, k_h)))).val[0];

    output.convertTo(img, CV_8UC1);
    //GaussianBlur(img, img, cv::Size(5, 5), 0, 0);
}

// function to define kernels for convolution
Mat define_kernel_box(int k_width, int k_height)
{
    // box kernel
    Mat kernel(k_height, k_width, CV_64FC1, Scalar(1.0 / (k_width * k_height))); // creates 3x3 matrix multiplied in scaler value 1/9
    return kernel;
}

// function to implement convolution of average filter
void boxFilter(Mat& img, int k_w, int k_h)
{

    Mat pad_img, kernel;
    pad_img = padding(img, k_w, k_h);
    kernel = define_kernel_box(k_w, k_h);

    Mat output = Mat::zeros(img.size(), CV_64FC1);

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            output.at<double>(i, j) = sum(kernel.mul(pad_img(Rect(j, i, k_w, k_h)))).val[0]; // rect selects the corresponding neighbours of each pixel

    output.convertTo(img, CV_8UC1);
}

// function to implement convolution of median filter
void medianFilter(Mat& image)
{
    int window[9];
    for (int i = 1; i < image.rows - 1; i++)
    {
        for (int j = 1; j < image.cols - 1; j++)
        {
            window[0] = image.at<uchar>(i - 1, j - 1);
            window[1] = image.at<uchar>(i, j - 1);
            window[2] = image.at<uchar>(i + 1, j - 1);

            window[3] = image.at<uchar>(i - 1, j);
            window[4] = image.at<uchar>(i, j);
            window[5] = image.at<uchar>(i + 1, j);

            window[6] = image.at<uchar>(i - 1, j + 1);
            window[7] = image.at<uchar>(i, j + 1);
            window[8] = image.at<uchar>(i + 1, j + 1);
            insertionSort(window, 9);
            image.at<uchar>(i, j) = window[4];
        }
    }
}
