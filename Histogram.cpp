#include "Histogram.h"
#include <iostream>
using namespace cv ;

using namespace std;

Mat calc_histogram(Mat image) {
    Mat hist;
    hist = Mat::zeros(256, 1, CV_32F);
    image.convertTo(image, CV_32F);
    double value = 0;
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            value = image.at<float>(i, j);
            hist.at<float>(value) = hist.at<float>(value) + 1;
        }
    }
    return hist;
}
Mat plot_histogram(Mat histogram ,int r,int g,int b) {
    Mat histogram_image(400, 512, CV_8UC3, Scalar(0, 0, 0));
    Mat normalized_histogram;
    normalize(histogram, normalized_histogram, 0, 400, NORM_MINMAX, -1, Mat());

    for (int i = 0; i < 256; i++)
    {
            rectangle(histogram_image, Point(2 * i, histogram_image.rows - normalized_histogram.at<float>(i)),
            Point(2 * (i + 1), histogram_image.rows), Scalar(r, g, b));
    }
    return histogram_image;

};
std::tuple<cv::Mat, cv::Mat,cv::Mat> splitChannels(cv::Mat image){
    std::vector<Mat> channels;
    Mat red,green,blue;
    split(image, channels);
    red=channels[2];
    green=channels[1];
    blue=channels[0];
    red=calc_histogram(red);
    green=calc_histogram(green);
    blue=calc_histogram(blue);
    red=plot_histogram(red,255);
    green=plot_histogram(green,0,255);
    blue=plot_histogram(blue,0,0,255);
    return std::make_tuple(red,green,blue);


}
cv::Mat DistributionCal(cv::Mat histogram) {

        int num_bins = histogram.rows;
        Mat curve_image(400, 512, CV_8UC3, Scalar(0, 0, 0));
        Mat normalized_histogram;
        normalize(histogram, normalized_histogram, 0, 400, NORM_MINMAX, -1, Mat());

        vector<Point> curve_points(num_bins);
        for (int i = 0; i < num_bins; i++) {
            curve_points[i] = Point(2 * i, curve_image.rows - normalized_histogram.at<float>(i));
        }
        const Point *pts = (const Point*)Mat(curve_points).data;
        int npts = Mat(curve_points).rows;

        polylines(curve_image, &pts, &npts, 1, false, Scalar(255, 0, 0), 2);

        return curve_image;

};
