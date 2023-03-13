#include "processing.h"
#include "QDebug"
#define hist first
#define minValue second


ProcessImg::ProcessImg()
{

}


/*
 * function to implement median filter
 * params : CV::Mat object image
 * first it selects the neighbouring 8 pixels for each pixel in the picture, then sort them and inject the median in the current pix
*/
void ProcessImg::normalize(Mat& img, int minVal, int maxVal)
{
    int oldMin = 99999, oldMax = 0;
    for (int rowCounter = 0; rowCounter < img.rows; rowCounter++)
    for (int colCounter = 0; colCounter < img.cols; colCounter++)
    {
        const unsigned char& pixelVal = img.at<unsigned char>(rowCounter,colCounter);
        if(pixelVal>oldMax) oldMax = pixelVal;
        if(pixelVal<oldMin) oldMin = pixelVal;
    }

    if(minVal==oldMin && maxVal==oldMax) return ;

    for (int rowCounter = 0; rowCounter < img.rows; rowCounter++)
    for (int colCounter = 0; colCounter < img.cols; colCounter++)
    {
        unsigned char& pixelVal = img.at<unsigned char>(rowCounter,colCounter);
        pixelVal = ((pixelVal-oldMin) * (maxVal-minVal) / (oldMax-oldMin)) + minVal;
    }
}


/*
 * function to implement median filter
 * params : CV::Mat object image
 * first it selects the neighbouring 8 pixels for each pixel in the picture, then sort them and inject the median in the current pix
*/
void ProcessImg::histEqualize(Mat& img)
{
    int n_bins = 256;
    QPair<vector<int>, int> data = calcHist(img); // getting histogram array and the minima in pair
    vector<int> hist = data.hist;
    int minVal = data.minValue;
    float scale = (n_bins - 1.f) / (img.rows*img.cols - hist[minVal]);
    vector<int> lut(n_bins, 0);
    int i = minVal+1;
    int sum = 0;

    for (; i < hist.size(); ++i) {
        sum += hist[i];
        // the value is saturated in range [0, max_val]
        lut[i] = max(0, min(int(round(sum * scale)), 255));
    }

    for (int rowCounter = 0; rowCounter < img.rows; rowCounter++)
    for (int colCounter = 0; colCounter < img.cols; colCounter++)
    {
            unsigned char& pixelVal = img.at<unsigned char>(rowCounter,colCounter);
            pixelVal = lut[pixelVal];
    }
}


/*
 * function to implement median filter
 * params : CV::Mat object image
 * first it selects the neighbouring 8 pixels for each pixel in the picture, then sort them and inject the median in the current pix
*/
QPair<vector<int>, int> ProcessImg::calcHist(Mat& img, int maxVal)
{
    int n_bins = maxVal + 1;
    int minVal = maxVal;
    // Compute histogram
    vector<int> hist(n_bins, 0);
    for (int rowCounter = 0; rowCounter < img.rows; rowCounter++)
    for (int colCounter = 0; colCounter < img.cols; colCounter++)
    {
        const unsigned char& pixelVal = img.at<unsigned char>(rowCounter,colCounter);
        // getting the lowest pixel value index as it will be the first non zero value so we can know the CDF min after
        // finishing scanning the image
        if (pixelVal < minVal) minVal = pixelVal;
        hist[pixelVal] += 1;
    }

    QPair<vector<int>, int> data;
    data.hist = hist;
    data.minValue = minVal;
    return data;
}
