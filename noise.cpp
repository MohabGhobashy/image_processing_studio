#include "noise.h"
#include "opencv2/core.hpp"
#include <highgui.h>
#include <math.h>
#include <float.h>

double uniform()
{
    return (rand()/(float)0x7fff)-0.5;
}

//IplImage* GenerateNoise(IplImage* img, float amount=255)
//{
//    CvSize imgSize = cvGetSize(img);
//    IplImage* imgTemp = cvCloneImage(img); // This will hold the noisy image
//}

noise::noise()
{



}
