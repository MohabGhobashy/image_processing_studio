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

 void localThreshold( cv::Mat& input, cv::Mat& output, int blockSize, double C) {
     if(blockSize%2==0){
         blockSize=blockSize+1;
     }

     vector<int> pixelValues(blockSize * blockSize);
     output.create(input.rows, input.cols, CV_8UC1);

     for (int y = 0; y < input.rows; y++)
         {
             for (int x = 0; x < input.cols; x++)
             {
                 // Get the neighborhood around the pixel
                 int index = 0;
                 for (int j = -blockSize / 2; j <= blockSize / 2; j++)
                 {
                     for (int i = -blockSize / 2; i <= blockSize / 2; i++)
                     {
                         // Check if the pixel is within the image boundaries
                         int px = x + i;
                         int py = y + j;
                         if (px >= 0 && px < input.cols && py >= 0 && py < input.rows)
                         {
                             pixelValues[index++] = input.at<uchar>(py, px);
                         }
                     }
                 }

                 // Compute the local threshold using the mean and constant C
                 int threshold = (int)round(cv::mean(pixelValues)[0] - C);

                 // Apply the threshold to the pixel
                 if (input.at<uchar>(y, x) >= threshold)
                 {
                     output.at<uchar>(y, x) = 255;
                 }
                 else
                 {
                     output.at<uchar>(y, x) = 0;
                 }
             }
         }
}
