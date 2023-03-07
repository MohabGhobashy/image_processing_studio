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
void non_max_suppression(cv::Mat& magnitude,cv::Mat& direction,cv::Mat& result) {
    // Create a copy of the magnitude matrix
    result = magnitude.clone();

    // Suppress non-maximum points
    for (int y = 1; y < magnitude.rows - 1; y++) {
        for (int x = 1; x < magnitude.cols - 1; x++) {
            // Calculate the angle of the gradient at this pixel
            float angle = direction.at<float>(y, x) * 180.0 / CV_PI;

            // Wrap the angle around 180 degrees
            if (angle < 0) {
                angle += 180;
            }

            // Find the two neighboring pixels along the gradient direction
            int x1, y1, x2, y2;
            if (angle < 22.5 || angle >= 157.5) {
                x1 = x2 = x;
                y1 = y - 1;
                y2 = y + 1;
            }
            else if (angle < 67.5) {
                x1 = x - 1;
                y1 = y - 1;
                x2 = x + 1;
                y2 = y + 1;
            }
            else if (angle < 112.5) {
                x1 = x - 1;
                y1 = y;
                x2 = x + 1;
                y2 = y;
            }
            else {
                x1 = x - 1;
                y1 = y + 1;
                x2 = x + 1;
                y2 = y - 1;
            }

            // Suppress the point if its magnitude is smaller than either of its neighbors
            float mag = magnitude.at<float>(y, x);
            float mag1 = magnitude.at<float>(y1, x1);
            float mag2 = magnitude.at<float>(y2, x2);
            if (mag < mag1 || mag < mag2) {
                result.at<float>(y, x) = 0;
            }
        }
    }
}

cv::Mat CannyEdgeDetection(cv::Mat image, int segma, int lowThreshold, int highThreshold, int KernalSize) {
    cv::Mat Blured, magnitude, direction, result;
    int(*maskH)[3];
    int(*maskV)[3];
    //Gussian Bluring
    GaussianBlur(image, Blured, cv::Size(KernalSize, KernalSize), segma, segma);
    maskH = getArray("Sobel", "horizontal");
    maskV = getArray("Sobel", "vertical");
    //Sobel Edge detection in both vertical and horizontal directions
    cv::Mat gradientx = masking(Blured, maskH);
    cv::Mat gradienty = masking(Blured, maskV);
    gradientx.convertTo(gradientx, CV_32F);
    gradienty.convertTo(gradienty, CV_32F);
    cartToPolar(gradientx, gradienty, magnitude, direction, true);
    non_max_suppression(magnitude, direction, result);
    inRange(result, cv::Scalar(lowThreshold), cv::Scalar(highThreshold), result);
    return result;
};

using namespace std;
