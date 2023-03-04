#include "EdgeDetection.h"
#include <iostream>
using namespace std;


cv::Mat toGreyScale(cv::Mat image) {
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            cv::Vec3b rgbPixel = image.at<cv::Vec3b>(i, j); // we get pixel at each postion
            unsigned char grayScale = (rgbPixel[2] + rgbPixel[1] + rgbPixel[0]) / 3;
            image.at<cv::Vec3b>(i, j) = { grayScale ,grayScale ,grayScale };
        }
    }
    return image;
}

int(*(getArray)(std::string mode, std::string direction))[3]{
    if (mode == "Sobel") {
        if (direction == "horizontal") {
            static int mask[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };
            return mask;
        }
        else {
            static int mask[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
            return mask;
        }
    }
if (mode == "Roberts") {
    if (direction == "horizontal") {
        static int mask[3][3] = { {0,0,0},{0,-1,0},{0,0,1} };
        return mask;
    }
    else {
        static int mask[3][3] = { {0,0,0},{0,1,0},{0,0,-1} };
        return mask;
    }
}
  if (mode == "Sobel") {
        if (direction == "horizontal") {
            static int mask[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };
            return mask;
        }
        else {
            static int mask[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
            return mask;
        }
    }
else if(mode =="Prewitt")  {
        if (direction == "horizontal") {
            static int mask[3][3] = { {-1,-1,-1},{0,0,0},{1,1,1} };
            return mask;
        }
        else {
            static int mask[3][3] = { {-1,0,1},{-1,0,1},{-1,0,1} };
            return mask;
        }
    }
}

cv::Mat masking(cv::Mat image,int mask[3][3])
{
        cv::Mat temImage = image.clone();
        for (int i = 1; i < image.rows - 1; i++)
        {
            for (int j = 1; j < image.cols - 1; j++)
            {

                    int pixel1 = image.at<uchar>(i - 1, j - 1) * mask[0][0];
                    int pixel2 = image.at<uchar>(i, j - 1) * mask[0][1];
                    int pixel3 = image.at<uchar>(i + 1, j - 1) * mask[0][2];

                    int pixel4 = image.at<uchar>(i - 1, j) * mask[1][0];
                    int pixel5 = image.at<uchar>(i, j) * mask[1][1];
                    int pixel6 = image.at<uchar>(i + 1, j) * mask[1][2];

                    int pixel7 = image.at<uchar>(i - 1, j + 1) * mask[2][0];
                    int pixel8 = image.at<uchar>(i, j + 1) * mask[2][1];
                    int pixel9 = image.at<uchar>(i + 1, j + 1) * mask[2][2];

                    int sum = pixel1 + pixel2 + pixel3 + pixel4 + pixel5 + pixel6 + pixel7 + pixel8 + pixel9;
                    if (sum < 0)
                        sum = 0;

                    if (sum > 255)
                        sum = 255;

                    temImage.at<uchar>(i, j) = sum;

            }
        }
        return temImage;
}





using namespace std;
