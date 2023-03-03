#include "Image.h"
using namespace std;
using namespace cv;

Image::Image(){}

void Image::setImage(Mat& img)
{
    this->originalImg = img;
    setOriginalMat();
}

void Image::setOriginalMat(){
    const int modesNum = 5;
    string modes[modesNum] = {"filtring", "edge detection", "process", "threshold", "hyprid"};
    for(int i= 0; i<modesNum; i++){
        this->UpdatedImgs[modes[i]] = originalImg.clone();
    }

}

Mat& Image::getImage(string mode){
    return this->UpdatedImgs[mode];
}

Mat& Image::getOriginalImage(){
    return this->originalImg;
}


void Image::updateImage(string mode, Mat& img){
    this->UpdatedImgs[mode] = img;
}
