#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QQueue>
#include "opencv2/world.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "Image.h"
#include"Threshold.h"
using namespace cv;

Image* img = new Image();
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->hide();
    ui->slider_value->hide();
    ui->slider_label->hide();
    ui->labelOriginalTab7->hide();
    ui->labelThreshold->hide();
    ui->localRadio->hide();
    ui->globalRadio->hide();
    ui->submitThreshold->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionupload_triggered()
{

    Mat dest;
    QString imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
    //read image using opencv

    Mat image = imread(imgPath.toStdString());
    cvtColor(image, dest,COLOR_BGR2RGB);
    img->setImage(dest);
    QImage image2((uchar*)dest.data, dest.cols, dest.rows,QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(image2);
    int width_img=ui->originalImg->width();
    int height_img=ui->originalImg->height();
    ui->originalImg->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    ui->filteredImg->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    ui->originalImgLbl->setText("Original Image");
    ui->filteredImgLbl->setText("Filtered Image");
//tab thresholding
    Mat grayImg;

    convertToGrayscale(image, grayImg);
    QImage imageGrayQt((uchar*)grayImg.data, grayImg.cols, grayImg.rows,QImage::Format_Grayscale8);
    QPixmap pixGray = QPixmap::fromImage(imageGrayQt);
    img->updateImage("threshold",grayImg);
    ui->originalImgTab6->setPixmap(pixGray.scaled(width_img,height_img,Qt::KeepAspectRatio));

    ui->labelOriginalTab7->show();
    ui->labelThreshold->show();
    ui->localRadio->show();
    ui->globalRadio->show();

    ui->submitThreshold->show();


}

//show slider value while changing
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->slider_value->setText( QString::number(value));

}


void MainWindow::on_globalRadio_clicked()
{
    ui->horizontalSlider->show();
    ui->slider_value->show();
    ui->slider_label->show();
}


void MainWindow::on_submitThreshold_clicked()
{
    if (ui->globalRadio->isChecked()) {

     Mat originalImg=img->getImage("threshold");
     Mat thresholded;

    Threshold(originalImg, thresholded,ui->horizontalSlider->value() );



     QImage image2((uchar*)thresholded.data, thresholded.cols, thresholded.rows,QImage::Format_Grayscale8);
     QPixmap pix = QPixmap::fromImage(image2);
     int width_img=ui->thresholdedImg->width();
     int height_img=ui->thresholdedImg->height();
     ui->thresholdedImg->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    } else {


    }
}

