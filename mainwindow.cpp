#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/world.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QQueue>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "Image.h"
#include "processing.h"
#include "EdgeDetection.h"
#include "Histogram.h"
#include "Threshold.h"
#include "frequencyfilters.h"
#include "noise.h"
#include "noiseFilters.h"

using namespace std;
using namespace cv;

Image* img = new Image();
QString imgPath;

Image* img2 = new Image();
QString imgPath2;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->equalizeBtn->setDisabled(true);
    ui->normalizeBtn->setDisabled(true);
    ui->submitThreshold_2->setDisabled(true);
    ui->buttons_layout->setVisible(false);
    ui->freqBtnsLayout->setVisible(false);
    ui->verticalWidget->setVisible(false);
    ui->horizontalSlider->hide();
    ui->cSlider->hide();
    ui->blockSizeSlider->hide();
    ui->slider_value->hide();
    ui->slider_label->hide();
    ui->labelOriginalTab7->hide();
    ui->labelThreshold->hide();
    ui->localRadio->hide();
    ui->globalRadio->hide();
    ui->submitThreshold->hide();
    ui->cLabel->hide();
    ui->blockLabel->hide();
    ui->EdgesDirection->hide();
    ui->EdgesFilter->hide();
    ui->submitEdges->hide();
    ui->originalImgLbl_2->hide();
    ui->originalImgLbl_3->hide();
    ui->label->hide();
    ui->label2->hide();
    ui->sigmaSlider->hide();
    ui->sigmaValue->hide();
    ui->kernelSlider->hide();
    ui->KernelValue->hide();
    ui->lowThresholdSlider->hide();
    ui->lowThresholdValue->hide();
    ui->highThresholdValue->hide();
    ui->highThresholdSlider->hide();
    ui->lowThresholdLabel->hide();
    ui->highThresholdLabel->hide();
    ui->sigma_label->hide();
    ui->lowThresholdLabel->hide();
    ui->highThresholdLabel->hide();
    ui->kernelLabel->hide();
    ui->submitThreshold_2->hide();
    ui->distLabel->hide();
    ui->histLabel->hide();
    ui->comboBox->hide();
    ui->GaussianSlider->hide();
    ui->average_slider->hide();
    ui->average_slider_value->hide();
    this->origWidth = ui->originalImg->width();
    this->origHeight = ui->originalImg->height();
    ui->slider_magdy_val->hide();
    ui->mean_sliderGaussN->hide();
    ui->sigma_sliderGaussN->hide();
    ui->saltSlider->hide();
    ui->paperSlider->hide();
    ui->saltLabel->hide();
    ui->paperLabel->hide();
    ui->varianceLabel->hide();
    ui->meanLabel->hide();
    ui->saltLabel_2->hide();
    ui->paperLabel_2->hide();
    ui->meanLabel_2->hide();
    ui->varianceLabel_2->hide();
    ui->kernelSize->hide();
    ui->kernelSize_2->hide();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionupload_triggered()
{
    imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");

    // read image using opencv
    if(imgPath.isEmpty())
        return;

    // tab filtering
    ui->comboBox->show();
    ui->verticalWidget->setVisible(true);
    ui->buttons_layout->setVisible(true);
    ui->freqBtnsLayout->setVisible(true);
    ui->submitThreshold_2->show();
    ui->distLabel->show();
    ui->histLabel->show();
    Mat image = imread(imgPath.toStdString());
    cvtColor(image, image,COLOR_BGR2RGB);
    img->setImage(image);
    cvtColor(img->getOriginalImage(), img->getImage("filtering"), COLOR_BGR2GRAY);
    showImg(img->getImage("filtering"), ui->originalImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    showImg(img->getImage("filtering"), ui->filteredImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);

    // tab thresholding
    Mat grayImg;
    convertToGrayscale(image, grayImg);
    img->updateImage("threshold",grayImg);
    showImg(img->getImage("threshold"), ui->originalImgTab6, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    ui->labelOriginalTab7->show();
    ui->labelThreshold->show();
    ui->localRadio->show();
    ui->globalRadio->show();
    ui->submitThreshold->show();
    ui->thresholdedImg->clear();

    // tab process
    ui->equalizeBtn->setDisabled(false);
    ui->normalizeBtn->setDisabled(false);
    cvtColor(img->getOriginalImage(), img->getImage("process"), COLOR_BGR2GRAY);
    showImg(img->getImage("process"), ui->originalImg_tab4, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    showImg(img->getImage("process"), ui->processedImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    ui->originalImgLbl_tab4->setText("Original Image");
    ui->processedImgLbl->setText("Processed Image");
    Mat origHist = calc_histogram(img->getOriginalImage());
    Mat orihHistImg = plot_histogram(origHist, 255, 147, 111);
    showImg(orihHistImg, ui->orgininalHist, QImage::Format_RGB888, this->origWidth, this->origHeight);
    showImg(orihHistImg, ui->processedHist, QImage::Format_RGB888, this->origWidth, this->origHeight);

    // tab edge detection
    cvtColor(img->getOriginalImage(), img->getImage("edge-detection"), COLOR_BGR2GRAY);
    showImg(img->getImage("edge-detection"), ui->originalImgEdges, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    ui->EdgesFilter->show();
    ui->submitEdges->show();
    ui->EdgesDirection->show();
    ui->originalImgLbl_2->show();
    ui->originalImgLbl_3->show();
    ui->label->show();
    ui->label2->show();
    ui->filteredImgEdge->clear();

    // tab curves
    Mat histogram;
    Mat distCurve;
    histogram=calc_histogram(image);
    distCurve=DistributionCal(histogram);
    histogram=plot_histogram(histogram,255,147,111);
    showImg(histogram, ui->histImg, QImage::Format_RGB888, this->origWidth, this->origHeight);
    showImg(distCurve, ui->distImg, QImage::Format_RGB888, this->origWidth, this->origHeight);
    Mat r ,g ,b;
    std::tie(r, g,b) = splitChannels(image);
    showImg(r, ui->rHist, QImage::Format_RGB888, this->origWidth, this->origHeight);
    showImg(g, ui->gHist, QImage::Format_RGB888, this->origWidth, this->origHeight);
    showImg(b, ui->bHist, QImage::Format_RGB888, this->origWidth, this->origHeight);

    // tab frequency filters
    cvtColor(img->getOriginalImage(), img->getImage("hyprid"), COLOR_BGR2GRAY);
    Mat imaggrey = img->getImage("hyprid");
    showImg(imaggrey, ui->originalImg_freqfilters, QImage::QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    showImg(imaggrey, ui->originalImg_freqfilters2, QImage::QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


// show slider value while changing
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->slider_value->setText( QString::number(value));
}


void MainWindow::on_globalRadio_clicked()
{
    ui->horizontalSlider->show();
    ui->slider_value->show();
    ui->slider_label->show();
    ui->cSlider->hide();
    ui->blockSizeSlider->hide();
    ui->cLabel->hide();
    ui->blockLabel->hide();
    ui->cSliderValue->hide();
    ui->blockSliderValue->hide();
}


void MainWindow::on_normalizeBtn_clicked()
{
    ProcessImg::normalize(img->getImage("process"));
    showImg(img->getImage("process"), ui->processedImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    Mat processedHist = calc_histogram(img->getImage("process"));
    Mat processedHistImg = plot_histogram(processedHist, 255, 147, 111);
    showImg(processedHistImg, ui->processedHist, QImage::Format_RGB888, this->origWidth, this->origHeight);
}


void MainWindow::on_equalizeBtn_clicked()
{
    ProcessImg::histEqualize(img->getImage("process"));
    showImg(img->getImage("process"), ui->processedImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    Mat processedHist = calc_histogram(img->getImage("process"));
    Mat processedHistImg = plot_histogram(processedHist, 255, 147, 111);
    showImg(processedHistImg, ui->processedHist, QImage::Format_RGB888, this->origWidth, this->origHeight);
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(imgPath.isEmpty())
        return;
}


void MainWindow::on_submitThreshold_clicked()
{
    if (ui->globalRadio->isChecked()) {
        Mat originalImg=img->getImage("threshold");
        Mat globaThresholded;
        Threshold(originalImg, globaThresholded,ui->horizontalSlider->value() );
        showImg(globaThresholded, ui->thresholdedImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    } else {
        Mat originalImg=img->getImage("threshold");
        Mat localThresholded;
        localThreshold(originalImg, localThresholded,ui->blockSizeSlider->value(),ui->cSlider->value() );
        showImg(localThresholded, ui->thresholdedImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    }
}


void MainWindow::on_cSlider_valueChanged(int value)
{
    ui->cSliderValue->setText( QString::number(value));
}


void MainWindow::on_blockSizeSlider_valueChanged(int value)
{
    ui->blockSliderValue->setText( QString::number(value));
}


void MainWindow::on_localRadio_clicked()
{
    ui->cSlider->show();
    ui->blockSizeSlider->show();
    ui->cLabel->show();
    ui->blockLabel->show();
    ui->horizontalSlider->hide();
    ui->slider_label->hide();
    ui->slider_value->hide();
    ui->cSliderValue->show();
    ui->blockSliderValue->show();
}


void MainWindow::on_submitEdges_clicked()
{
    Mat originalImg=img->getImage("threshold");
    Mat result;
    int(*mask)[3];

    // convertToGrayscale(originalImg,grayScaled);
    if(ui->EdgesFilter->currentText().toStdString()=="Canny"){
        ui->EdgesDirection->hide();
        result=CannyEdgeDetection(originalImg,ui->sigmaSlider->value(),ui->lowThresholdSlider->value(),ui->highThresholdSlider->value(),ui->kernelSlider->value());
    }
    else{
        mask=getArray(ui->EdgesFilter->currentText().toStdString(),ui->EdgesDirection->currentText().toStdString());
        result=masking(originalImg,mask);
    }
    showImg(result, ui->filteredImgEdge, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_sigmaSlider_valueChanged(int value)
{
    ui->sigmaValue->setText( QString::number(value));
}


void MainWindow::on_kernelSlider_valueChanged(int value)
{
    ui->KernelValue->setText( QString::number(value));
}


void MainWindow::on_lowThresholdSlider_valueChanged(int value)
{
    ui->lowThresholdValue->setText( QString::number(value));
}


void MainWindow::on_EdgesFilter_currentIndexChanged(int index)
{
    if (index == 3)
        {
            ui->EdgesDirection->hide();
            ui->sigmaSlider->show();
            ui->sigmaValue->show();
            ui->kernelSlider->show();
            ui->KernelValue->show();
            ui->lowThresholdSlider->show();
            ui->lowThresholdValue->show();
            ui->highThresholdValue->show();
            ui->highThresholdSlider->show();
            ui->label2->hide();
            ui->sigma_label->show();
            ui->lowThresholdLabel->show();
            ui->highThresholdLabel->show();
            ui->kernelLabel->show();
        }
        else
        {
            ui->EdgesDirection->show();
            ui->sigmaSlider->hide();
            ui->sigmaValue->hide();
            ui->kernelSlider->hide();
            ui->KernelValue->hide();
            ui->lowThresholdSlider->hide();
            ui->lowThresholdValue->hide();
            ui->highThresholdValue->hide();
            ui->highThresholdSlider->hide();
            ui->sigma_label->hide();
            ui->lowThresholdLabel->hide();
            ui->highThresholdLabel->hide();
            ui->kernelLabel->hide();
        }
}


void MainWindow::on_highThresholdSlider_valueChanged(int value)
{
    ui->highThresholdValue->setText( QString::number(value));
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    Mat r ,g ,b;
    if(index==0){
        std::tie(r, g,b) = splitChannels(img->getOriginalImage());
}
    if(index==1){
        std::tie(r, g,b)=plot_rgb_distribution_function(img->getOriginalImage(),"DF");
    }
    if(index==2){
        std::tie(r, g,b)=plot_rgb_distribution_function(img->getOriginalImage(),"cumulative");
    }
    int width_img=ui->rHist->width();
    int height_img=ui->rHist->height();
    showImg(r, ui->rHist, QImage::Format_RGB888, width_img, height_img);
    showImg(g, ui->gHist, QImage::Format_RGB888, width_img, height_img);
    showImg(b, ui->bHist, QImage::Format_RGB888, width_img, height_img);
}


void MainWindow::on_lowpass_btn_clicked()
{
    cvtColor(img->getOriginalImage(), img->getImage("hyprid"), COLOR_BGR2GRAY);
    Mat img1 = img->getImage("hyprid");
    cv::resize(img1, img1, Size(512, 512), 0, 0);
    Mat complex_img = calcDFT(img1);
    Mat filter = createFilter(complex_img, 20, "lowpass");
    Mat output = applyFilter(complex_img, filter);
    ifft(output);

    // prepare to view
    double  minVal,  maxVal;
    minMaxLoc(output,  &minVal,  &maxVal);
    output.convertTo(output,  CV_8U,  255.0/(maxVal  -  minVal),  -minVal);
    showImg(output, ui->freq_filtered, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_highpass_btn_clicked()
{
    cvtColor(img->getOriginalImage(), img->getImage("hyprid"), COLOR_BGR2GRAY);
    Mat img1 = img->getImage("hyprid");
    cv::resize(img1, img1, Size(512, 512), 0, 0);
    Mat complex_img = calcDFT(img1);
    Mat filter = createFilter(complex_img, 20, "highpass");
    Mat output = applyFilter(complex_img, filter);
    ifft(output);

    // prepare to view
    double  minVal,  maxVal;
    minMaxLoc(output,  &minVal,  &maxVal);
    output.convertTo(output,  CV_8U,  255.0/(maxVal  -  minVal),  -minVal);
    showImg(output, ui->freq_filtered, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_submitThreshold_2_clicked()
{
    // first image
    cvtColor(img->getOriginalImage(), img->getImage("hyprid"), COLOR_BGR2GRAY);
    Mat imglow = img->getImage("hyprid");
    cv::resize(imglow, imglow, Size(512, 512), 0, 0);
    Mat complex_img1 = calcDFT(imglow);
    Mat filter1 = createFilter(complex_img1, 20, "lowpass");
    Mat output1 = applyFilter(complex_img1, filter1);

    // second image
    cvtColor(img2->getOriginalImage(), img2->getImage("hyprid"), COLOR_BGR2GRAY);
    Mat imghigh = img2->getImage("hyprid");
    cv::resize(imghigh, imghigh, Size(512, 512), 0, 0);
    Mat complex_img2 = calcDFT(imghigh);
    Mat filter2 = createFilter(complex_img2, 20, "highpass");
    Mat output2 = applyFilter(complex_img2, filter2);

    // create hybrid image
    Mat hybrid;
    add(output1, output2, hybrid);
    ifft(hybrid);
    ifft(output1);
    ifft(output2);

    // prepare to view
    double  minVal,  maxVal;
    minMaxLoc(output1,  &minVal,  &maxVal);
    output1.convertTo(output1,  CV_8U,  255.0/(maxVal  -  minVal),  -minVal);
    minMaxLoc(output2,  &minVal,  &maxVal);
    output2.convertTo(output2,  CV_8U,  255.0/(maxVal  -  minVal),  -minVal);
    minMaxLoc(hybrid,  &minVal,  &maxVal);
    hybrid.convertTo(hybrid,  CV_8U,  255.0/(maxVal  -  minVal),  -minVal);

    // view all images
    showImg(output1, ui->originalImg_freqfilters2, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    showImg(output2, ui->originalImg_freqfilters3, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
    showImg(hybrid, ui->resulthybrid, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_actionupload_2nd_img_triggered()
{
    ui->submitThreshold_2->setDisabled(false);
    Mat dest;
    imgPath2 = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
    if(imgPath2.isEmpty())
        return;
    Mat image = imread(imgPath2.toStdString());
    cvtColor(image, dest,COLOR_BGR2RGB);
    img2->setImage(dest);
    cvtColor(img2->getOriginalImage(), img2->getImage("hyprid"), COLOR_BGR2GRAY);
    showImg(img2->getImage("hyprid"), ui->originalImg_freqfilters3, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::showImg(Mat& img, QLabel* imgLbl, enum QImage::Format imgFormat, int width , int hieght)
{
    QImage image2((uchar*)img.data, img.cols, img.rows, imgFormat);
    QPixmap pix = QPixmap::fromImage(image2);
    int width_img = ui->originalImg->width();
    int height_img = ui->originalImg->height();
    imgLbl->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));
}


void MainWindow::on_saltNoBtn_2_clicked()
{
    ui->saltSlider->show();
    ui->paperSlider->show();
    ui->saltLabel->show();
    ui->paperLabel->show();
    ui->saltLabel_2->show();
    ui->paperLabel_2->show();
    ui->GaussianSlider->hide();
    ui->average_slider->hide();
    ui->average_slider_value->hide();
    ui->slider_magdy_val->hide();
    ui->mean_sliderGaussN->hide();
    ui->sigma_sliderGaussN->hide();
    ui->varianceLabel->hide();
    ui->meanLabel->hide();
    ui->meanLabel_2->hide();
    ui->varianceLabel_2->hide();
    ui->kernelSize->hide();
    ui->kernelSize_2->hide();

    Mat noisyImg = img->getImage("filtering");
    Add_salt_pepper_Noise(noisyImg,ui->saltSlider->value(),ui->paperSlider->value());
    showImg(noisyImg, ui->filteredImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_gaussianNoBtn_2_clicked()
{
    ui->mean_sliderGaussN->show();
    ui->sigma_sliderGaussN->show();
    ui->varianceLabel->show();
    ui->meanLabel->show();
    ui->meanLabel_2->show();
    ui->varianceLabel_2->show();
    ui->GaussianSlider->hide();
    ui->average_slider->hide();
    ui->average_slider_value->hide();
    ui->slider_magdy_val->hide();
    ui->saltSlider->hide();
    ui->paperSlider->hide();
    ui->saltLabel->hide();
    ui->paperLabel->hide();
    ui->saltLabel_2->hide();
    ui->paperLabel_2->hide();
    ui->kernelSize->hide();
    ui->kernelSize_2->hide();

    Mat noisyImg = img->getImage("filtering");
    Add_gaussian_Noise(noisyImg, ui->mean_sliderGaussN->value(), ui->sigma_sliderGaussN->value());
    showImg(noisyImg, ui->filteredImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_avgNoBtn_2_clicked()
{
    ui->GaussianSlider->hide();
    ui->average_slider->hide();
    ui->average_slider_value->hide();
    ui->slider_magdy_val->hide();
    ui->mean_sliderGaussN->hide();
    ui->sigma_sliderGaussN->hide();
    ui->saltSlider->hide();
    ui->paperSlider->hide();
    ui->saltLabel->hide();
    ui->paperLabel->hide();
    ui->varianceLabel->hide();
    ui->meanLabel->hide();
    ui->saltLabel_2->hide();
    ui->paperLabel_2->hide();
    ui->meanLabel_2->hide();
    ui->varianceLabel_2->hide();
    ui->kernelSize->hide();
    ui->kernelSize_2->hide();

    Mat noisyImg = img->getImage("filtering");
    add_uniform_noise(noisyImg);
    showImg(noisyImg, ui->filteredImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_altFiltBtn_2_clicked()
{
    ui->GaussianSlider->hide();
    ui->average_slider->hide();
    ui->average_slider_value->hide();
    ui->slider_magdy_val->hide();
    ui->mean_sliderGaussN->hide();
    ui->sigma_sliderGaussN->hide();
    ui->saltSlider->hide();
    ui->paperSlider->hide();
    ui->saltLabel->hide();
    ui->paperLabel->hide();
    ui->varianceLabel->hide();
    ui->meanLabel->hide();
    ui->saltLabel_2->hide();
    ui->paperLabel_2->hide();
    ui->meanLabel_2->hide();
    ui->varianceLabel_2->hide();
    ui->kernelSize->hide();
    ui->kernelSize_2->hide();

    Mat filteredImg = img->getImage("filtering");
    medianFilter(filteredImg);
    showImg(filteredImg, ui->filteredImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_gaussianFilBtn_2_clicked()
{
    ui->GaussianSlider->show();
    ui->slider_magdy_val->show();
    ui->kernelSize_2->show();
    ui->average_slider->hide();
    ui->average_slider_value->hide();
    ui->mean_sliderGaussN->hide();
    ui->sigma_sliderGaussN->hide();
    ui->saltSlider->hide();
    ui->paperSlider->hide();
    ui->saltLabel->hide();
    ui->paperLabel->hide();
    ui->varianceLabel->hide();
    ui->meanLabel->hide();
    ui->saltLabel_2->hide();
    ui->paperLabel_2->hide();
    ui->meanLabel_2->hide();
    ui->varianceLabel_2->hide();
    ui->kernelSize->hide();

    Mat filteredImg = img->getImage("filtering");
    gaussianFilter(filteredImg,ui->GaussianSlider->value(),ui->GaussianSlider->value());
    showImg(filteredImg, ui->filteredImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_avgFiltBtn_2_clicked()
{
    ui->average_slider->show();
    ui->average_slider_value->show();
    ui->kernelSize->show();
    ui->GaussianSlider->hide();
    ui->slider_magdy_val->hide();
    ui->mean_sliderGaussN->hide();
    ui->sigma_sliderGaussN->hide();
    ui->saltSlider->hide();
    ui->paperSlider->hide();
    ui->saltLabel->hide();
    ui->paperLabel->hide();
    ui->varianceLabel->hide();
    ui->meanLabel->hide();
    ui->saltLabel_2->hide();
    ui->paperLabel_2->hide();
    ui->meanLabel_2->hide();
    ui->varianceLabel_2->hide();
    ui->kernelSize_2->hide();

    Mat filteredImg = img->getImage("filtering");
    boxFilter(filteredImg,ui->average_slider->value(),ui->average_slider->value());
    showImg(filteredImg, ui->filteredImg, QImage::Format_Grayscale8, this->origWidth, this->origHeight);
}


void MainWindow::on_average_slider_valueChanged(int value)
{
    ui->average_slider_value->setText(QString::number(value)+""+"x"+""+QString::number(value));
}


void MainWindow::on_GaussianSlider_valueChanged(int value)
{
    ui->slider_magdy_val->setText(QString::number(value)+""+"x"+""+QString::number(value));
}


void MainWindow::on_mean_sliderGaussN_valueChanged(int value)
{
    ui->meanLabel->setText(QString::number(value));
}


void MainWindow::on_sigma_sliderGaussN_valueChanged(int value)
{
    ui->varianceLabel->setText(QString::number(value));
}


void MainWindow::on_paperSlider_valueChanged(int value)
{
    ui->paperLabel->setText(QString::number(value));
}


void MainWindow::on_saltSlider_valueChanged(int value)
{
    ui->saltLabel->setText(QString::number(value));
}

