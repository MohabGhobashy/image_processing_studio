#include "frequencyfilters.h"

using namespace std;
using namespace cv;


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
// create output image of optimal size - prepare the image for dft
Mat adjustSize(Mat& img)
{
    Mat padded;
    int rows = getOptimalDFTSize(img.rows);
    int cols = getOptimalDFTSize(img.cols);
    copyMakeBorder(img, padded, 0, rows - img.rows, 0, cols - img.cols, BORDER_CONSTANT, Scalar::all(0));
    return padded;
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
void fourierShift(Mat& Img)
{
    Img = Img(Rect(0, 0, Img.cols & -2, Img.rows & -2));
    int cx = Img.cols / 2;
    int cy = Img.rows / 2;
    // Split image into quadrants
    Mat q0(Img, Rect(0, 0, cx, cy));
    Mat q1(Img, Rect(cx, 0, cx, cy));
    Mat q2(Img, Rect(0, cy, cx, cy));
    Mat q3(Img, Rect(cx, cy, cx, cy));
    // Swap quadrants
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    // Swap other quadrants
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
// Convert the image to complex numbers by applying dft
Mat calcDFT(Mat& img) {
    // Mat padded = adjustSize(img);
    // copy the source image, on the border add zero values
    Mat planes[] = { Mat_<float>(img), Mat::zeros(img.size(), CV_32F) };
    Mat complex_img;
    merge(planes, 2, complex_img);
    // create a complex matrix (Fourier transform)
    dft(complex_img, complex_img);
    fourierShift(complex_img);

    return complex_img;
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
Mat createFilter(Mat& complex_img, float distance, string filterType)
{
    Mat filter(complex_img.size(), CV_32F, Scalar(1));
    Point center = Point(complex_img.rows / 2, complex_img.cols / 2);
    float radius;
    for (int i = 0; i < complex_img.rows; i++)
    for (int j = 0; j < complex_img.cols; j++)
    {
        radius = sqrt(pow((i - center.x), 2.0) + pow((j - center.y), 2.0));

        if (filterType == "lowpass"){
            if (radius > distance)
                filter.at<float>(i, j) = 0;
        }
        else if (filterType == "highpass") {
            if (radius < distance)
                filter.at<float>(i, j) = 0;
        }
    }

    return filter;
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
void ifft(Mat& complex_img)
{
    fourierShift(complex_img);
    dft(complex_img, complex_img, DFT_INVERSE | DFT_REAL_OUTPUT);
    normalize(complex_img, complex_img, 0, 1, NORM_MINMAX);
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
Mat applyFilter(Mat& complex_img, Mat& filter)
{
    Mat output_img;
    Mat planes_filter[] = { Mat_<float>(filter.clone()), Mat_<float>(filter.clone()) };

    Mat planes_dft[] = { Mat_<float>(complex_img), Mat::zeros(complex_img.size(), CV_32F) };
    split(complex_img, planes_dft);

    Mat planes_out[] = { Mat::zeros(complex_img.size(), CV_32F), Mat::zeros(complex_img.size(), CV_32F) };
    planes_out[0] = planes_filter[0].mul(planes_dft[0]);
    planes_out[1] = planes_filter[1].mul(planes_dft[1]);

    merge(planes_out, 2, output_img);

    return output_img;
}
