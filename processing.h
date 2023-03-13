#ifndef PROCESSING_H
#define PROCESSING_H
#include "opencv2/core/core.hpp"
#include <iostream>
#include <vector>
#include <QPair>

using namespace std;
using namespace cv;

class ProcessImg
{
public:
    ProcessImg();
    static void normalize(Mat& img, int minVal = 0, int maxVal = 255);
    static void histEqualize(Mat& img);
    static QPair<vector<int>, int> calcHist(Mat& img, int maxVal = 255);
};

#endif // PROCESSING_H
