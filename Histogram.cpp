#include "Histogram.h"
#include <iostream>

using namespace cv;
using namespace std;


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
Mat calc_histogram(Mat image)
{
    Mat hist;
    hist = Mat::zeros(256, 1, CV_32F);
    image.convertTo(image, CV_32F);
    double value = 0;
    for (int i = 0; i < image.rows; i++)
    for (int j = 0; j < image.cols; j++)
    {
        value = image.at<float>(i, j);
        hist.at<float>(value) = hist.at<float>(value) + 1;
    }
    return hist;
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
Mat plot_histogram(Mat histogram ,int r,int g,int b)
{
    Mat histogram_image(400, 512, CV_8UC3, Scalar(0, 0, 0));
    Mat normalized_histogram;
    normalize(histogram, normalized_histogram, 0, 400, NORM_MINMAX, -1, Mat());

    for (int i = 0; i < 256; i++)
    {
        rectangle(histogram_image, Point(2 * i, histogram_image.rows - normalized_histogram.at<float>(i)),
        Point(2 * (i + 1), histogram_image.rows), Scalar(r, g, b));
    }

    return histogram_image;
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
tuple<Mat, Mat,Mat> splitChannels(Mat image)
{
    vector<Mat> channels;
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
    return make_tuple(red,green,blue);
}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
Mat DistributionCal(Mat histogram)
{
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

}


/*
 * function to add gaussian noise to an image
 * params : CV::Mat object type image, double mean, double variance
 * first we clone the image then we create a gaussian noise and finally we add this noise to the image
*/
tuple<Mat, Mat,Mat> plot_rgb_distribution_function(Mat image,string mode)
{
    vector<Mat> bgr_planes;
    split(image, bgr_planes);

    const int num_bins = 256;
    const int hist_height = 400;
    const int hist_width = 512;
    const int bin_width = cvRound(static_cast<double>(hist_width) / num_bins);

    // Create histograms for each color channel
    Mat b_hist, g_hist, r_hist;
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &num_bins, 0);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &num_bins, 0);
    calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &num_bins, 0);

    // Create separate images for each histogram
    Mat b_DF(hist_height, hist_width, CV_8UC3, Scalar(0, 0, 0));
    Mat g_DF(hist_height, hist_width, CV_8UC3, Scalar(0, 0, 0));
    Mat r_DF(hist_height, hist_width, CV_8UC3, Scalar(0, 0, 0));

    if(mode=="cumulative")
    {
        Mat cumulative_r,cumulative_b,cumulative_g;
        r_hist.copyTo(cumulative_r);
        g_hist.copyTo(cumulative_b);
        b_hist.copyTo(cumulative_g);


        for(int j=1;j<num_bins;j++)
        {
            cumulative_r.at<float >(j)+=cumulative_r.at<float>(j-1);
            cumulative_b.at<float >(j)+=cumulative_b.at<float>(j-1);
            cumulative_g.at<float >(j)+=cumulative_g.at<float>(j-1);
        }

        normalize(cumulative_b, b_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
        normalize(cumulative_g, g_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
        normalize(cumulative_r, r_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
    }
    else {
        // Normalize the histograms
        normalize(b_hist, b_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
        normalize(g_hist, g_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
        normalize(r_hist, r_hist, 0, hist_height, NORM_MINMAX, -1, Mat());
    }


    // Plot the histograms
    for (int i = 1; i < num_bins; i++)
    {
        line(b_DF, Point(bin_width * (i - 1), hist_height - cvRound(b_hist.at<float>(i - 1))),
            Point(bin_width * i, hist_height - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);
        line(g_DF, Point(bin_width * (i - 1), hist_height - cvRound(g_hist.at<float>(i - 1))),
            Point(bin_width * i, hist_height - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);
        line(r_DF, Point(bin_width * (i - 1), hist_height - cvRound(r_hist.at<float>(i - 1))),
            Point(bin_width * i, hist_height - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
    }

    return make_tuple(r_DF,g_DF,b_DF);
}
