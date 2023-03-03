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
#include "processing.h"


#include"Threshold.h"
using namespace cv;

Image* img = new Image();
QString imgPath;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->equalizeBtn->setDisabled(true);
    ui->normalizeBtn->setDisabled(true);

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
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionupload_triggered()
{

    Mat dest;
    imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
    //read image using opencv
    if(imgPath.isEmpty())
        return;

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

    //tab4
    ui->equalizeBtn->setDisabled(false);
    ui->normalizeBtn->setDisabled(false);
    cvtColor(img->getOriginalImage(), img->getImage("process"), COLOR_BGR2GRAY);
    QImage processedImg((uchar*)img->getImage("process").data, img->getImage("process").cols, img->getImage("process").rows,QImage::Format_Grayscale8);
    QPixmap Ppix4 = QPixmap::fromImage(processedImg);
    ui->originalImg_tab4->setPixmap(Ppix4.scaled(width_img,height_img,Qt::KeepAspectRatio));
    ui->processedImg->setPixmap(Ppix4.scaled(width_img,height_img,Qt::KeepAspectRatio));
    ui->originalImgLbl_tab4->setText("Original Image");
    ui->processedImgLbl->setText("Processed Image");

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
    ui->cSlider->hide();
    ui->blockSizeSlider->hide();
    ui->cLabel->hide();
    ui->blockLabel->hide();
    ui->cSliderValue->hide();
    ui->blockSliderValue->hide();

    



}



void MainWindow::on_normalizeBtn_clicked()
{
//    Mat N_img = img->getImage("process");
    ProcessImg::normalize(img->getImage("process"));
    QImage normalizedImg((uchar*)img->getImage("process").data, img->getImage("process").cols, img->getImage("process").rows,QImage::Format_Grayscale8);
    QPixmap N_pix4 = QPixmap::fromImage(normalizedImg);
    int width_img=ui->originalImg_tab4->width();
    int height_img=ui->originalImg_tab4->height();
    ui->processedImg->setPixmap(N_pix4.scaled(width_img,height_img,Qt::KeepAspectRatio));


}


void MainWindow::on_equalizeBtn_clicked()
{
//    Mat E_img = img->getImage("process");
    ProcessImg::histEqualize(img->getImage("process"));
    QImage equalizedImg((uchar*)img->getImage("process").data, img->getImage("process").cols, img->getImage("process").rows,QImage::Format_Grayscale8);
    QPixmap N_pix4 = QPixmap::fromImage(equalizedImg);
    int width_img=ui->originalImg_tab4->width();
    int height_img=ui->originalImg_tab4->height();
    ui->processedImg->setPixmap(N_pix4.scaled(width_img,height_img,Qt::KeepAspectRatio));
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(imgPath.isEmpty())
        return;
    if(index==3){
//        cvtColor(img->getOriginalImage(), img->getImage("process"), COLOR_BGR2GRAY);
//        QImage processedImg((uchar*)img->getImage("process").data, img->getImage("process").cols, img->getImage("process").rows,QImage::Format_Grayscale8);
//        QPixmap Ppix4 = QPixmap::fromImage(processedImg);
//        int width_img2=ui->originalImg->width();
//        int height_img2=ui->originalImg->height();
//        ui->originalImg_tab4->setPixmap(Ppix4.scaled(width_img2,height_img2,Qt::KeepAspectRatio));
//        ui->processedImg->setPixmap(Ppix4.scaled(width_img2,height_img2,Qt::KeepAspectRatio));
//        ui->originalImgLbl_tab4->setText("Original Image");
//        ui->processedImgLbl->setText("Processed Image");

    }
}


void MainWindow::on_submitThreshold_clicked()
{
    if (ui->globalRadio->isChecked()) {

     Mat originalImg=img->getImage("threshold");
     Mat globaThresholded;

    Threshold(originalImg, globaThresholded,ui->horizontalSlider->value() );



     QImage image2((uchar*)globaThresholded.data, globaThresholded.cols, globaThresholded.rows,QImage::Format_Grayscale8);
     QPixmap pix = QPixmap::fromImage(image2);
     int width_img=ui->thresholdedImg->width();
     int height_img=ui->thresholdedImg->height();
     ui->thresholdedImg->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    } else {

        Mat originalImg=img->getImage("threshold");
        Mat localThresholded;
        localThreshold(originalImg, localThresholded,ui->blockSizeSlider->value(),ui->cSlider->value() );
        QImage image2((uchar*)localThresholded.data, localThresholded.cols, localThresholded.rows,QImage::Format_Grayscale8);
        QPixmap pix = QPixmap::fromImage(image2);
        int width_img=ui->thresholdedImg->width();
        int height_img=ui->thresholdedImg->height();
        ui->thresholdedImg->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));

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


