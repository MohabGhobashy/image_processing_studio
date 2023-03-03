#include "Threshold.h"
#include <iostream>
using namespace cv ;

using namespace std;

void convertToGrayscale(Mat& input_image, Mat& output_image)
{
    if(input_image.empty())
    {
        cout << "Input image is empty" ;
        return;
    }

    output_image.create(input_image.rows, input_image.cols, CV_8UC1);

    for(int i = 0; i < input_image.rows; i++)
    {
        for(int j = 0; j < input_image.cols; j++)
        {
            Vec3b pixel = input_image.at<Vec3b>(i, j);

            int gray_value = 0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0];

            output_image.at<uchar>(i, j) = gray_value;
        }
    }
}
void Threshold(Mat& input_image, Mat& output_image,int threshold ) {
    output_image.create(input_image.rows, input_image.cols, CV_8UC1);
    for (int i = 0; i < input_image.rows; i++) {
        for (int j = 0; j < input_image.cols; j++) {
            if (input_image.at<uchar>(i, j) > threshold) {
                output_image.at<uchar>(i, j) = 0;
            } else {
                output_image.at<uchar>(i, j) = 255;
            }
        }
    }
}
