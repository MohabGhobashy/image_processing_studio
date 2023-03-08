#include "noiseFilters.h"
#include <iostream>
#include <opencv2/core.hpp>
#include<opencv2\highgui.hpp>

using namespace std;
using namespace cv;

/*
 * function to convert normal image to it's corresponding fourier transform
 * params : CV::Mat object type image
 * return : CV::Mat object type image
 * first we adjust the size of it to be apropriate to fourier
 * by zero padding
 * then construct the complex matrix to apply fourier on it
 * and calculate the magnitude from the resulting matrix
 * with small modification on the magnitude matrix like
 * the logarithmic sclae and rearrangement of it's quarters.
*/

/**
  * A method for convenience that returns shifted fourier transform to original
  *
  * @param planes: The dft output as separated planes planes[0] -> real, planes[1] -> imaginary.
  *
 */



// padding images to apply kernals on them
Mat padding(Mat img, int k_width, int k_height)
{
    img.convertTo(img, CV_64FC1); // converting the image pixels to 64 bits
    int pad_rows, pad_cols;
    pad_rows = (k_height - 1) / 2; // = 1
    pad_cols = (k_width - 1) / 2;  // = 1
    Mat pad_image(Size(img.cols + 2 * pad_cols, img.rows + 2 * pad_rows), CV_64FC1, Scalar(0)); // resizing the image with the padding
    img.copyTo(pad_image(Rect(pad_cols, pad_rows, img.cols, img.rows))); // creating new padded image
    return pad_image;
}

// insertion sort algorithm used in median filter
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

// function to define kernels for convolution
Mat define_kernel_box(int k_width, int k_height)
{
    // box kernel
    Mat kernel(k_height, k_width, CV_64FC1, Scalar(1.0 / (k_width * k_height))); // creates 3x3 matrix multiplied in scaler value 1/9
    return kernel;
}

// function to define kernels for gaussian convolution
Mat define_kernel_gaussian(int k_width, int k_height, int sigma)
{

    const double pi = 3.1415;
    int pad_rows = (k_height - 1) / 2; // = 1
    int pad_cols = (k_width - 1) / 2;  // = 1
    Mat kernel(k_height, k_width, CV_64FC1); // creates 3x3 matrix
    for (int i = -pad_rows; i <= pad_rows; i++)
        for (int j = -pad_cols; j <= pad_cols; j++)
            kernel.at<double>(i + pad_rows, j + pad_cols) = (1/(2.0*pi*sigma*sigma))*exp(-(i * i + j * j) / sigma*sigma);

    kernel = kernel / sum(kernel);
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

// function to implement convolution of gaussian filter
void gaussianFilter(Mat& img, int k_w, int k_h, int sigma)
{
    Mat pad_img, kernel;
    pad_img = padding(img, k_w, k_h);
    kernel = define_kernel_gaussian(k_w, k_h, sigma);

    Mat output = Mat::zeros(img.size(), CV_64FC1);

    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            output.at<double>(i, j) = sum(kernel.mul(pad_img(Rect(j, i, k_w, k_h)))).val[0];

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
