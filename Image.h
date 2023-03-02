#ifndef Image_H
#define Image_H

#include "opencv2/core/core.hpp"
#include <unordered_map>
#include <string>
#include <QPair>
using namespace std;
using namespace cv;
//#include "QTCV.h"

class Image
{
private:
    unordered_map<string, Mat> UpdatedImgs;
    Mat originalImg;
    void setOriginalMat();
public:
    Image();
    void setImage(Mat& img);
    Mat& getImage(string mode);
    Mat& getOriginalImage();
    void updateImage(string mode, Mat& img);
};

#endif // Image_H



