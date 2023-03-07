#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QQueue>
#include<QDebug>
#include "opencv2/world.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "Image.h"
#include "processing.h"
#include"EdgeDetection.h"
#include"Histogram.h"
using namespace std;


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
    ui->EdgesDirection->hide();
    ui->EdgesFilter->hide();
    ui->submitEdges->hide();

    ui->originalImgLbl_2->hide();
    ui->originalImgLbl_3->hide();
    ui->label->hide();
    ui->label2->hide();

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
    Mat origHist = calc_histogram(img->getOriginalImage());
    Mat orihHistImg = plot_histogram(origHist, 255, 147, 111);
    QImage processedhist((uchar*)orihHistImg.data, orihHistImg.cols, orihHistImg.rows,QImage::Format_RGB888);
    QPixmap Hpix4 = QPixmap::fromImage(processedhist);
    ui->orgininalHist->setPixmap(Hpix4.scaled(width_img,height_img,Qt::KeepAspectRatio));
    ui->processedHist->setPixmap(Hpix4.scaled(width_img,height_img,Qt::KeepAspectRatio));



    //    tab edge
    ui->EdgesFilter->show();
    ui->submitEdges->show();
    ui->EdgesDirection->show();
    ui->originalImgEdges->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    ui->originalImgLbl_2->show();
    ui->originalImgLbl_3->show();
    ui->label->show();
    ui->label2->show();
//    tab curves
    Mat histogram;
    Mat distCurve;
    histogram=calc_histogram(dest);
    distCurve=DistributionCal(histogram);
    histogram=plot_histogram(histogram,255,147,111);

    QImage hist((uchar*)histogram.data, histogram.cols, histogram.rows,QImage::Format_RGB888);
    QPixmap histPix = QPixmap::fromImage(hist);
    ui->histImg->setPixmap(histPix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    QImage distimg((uchar*)distCurve.data, distCurve.cols, distCurve.rows,QImage::Format_RGB888);
    QPixmap disttPix = QPixmap::fromImage(distimg);
    ui->distImg->setPixmap(disttPix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    Mat r ,g ,b;
    std::tie(r, g,b) = splitChannels(dest);

    QImage rImg((uchar*)r.data, r.cols, r.rows,QImage::Format_RGB888);
    QPixmap Rpix = QPixmap::fromImage(rImg);
    ui->rHist->setPixmap(Rpix.scaled(width_img,height_img,Qt::KeepAspectRatio));

    QImage gImg((uchar*)g.data, g.cols, g.rows,QImage::Format_RGB888);
    QPixmap Gpix = QPixmap::fromImage(gImg);
    ui->gHist->setPixmap(Gpix.scaled(width_img,height_img,Qt::KeepAspectRatio));
    QImage bImg((uchar*)b.data, b.cols, b.rows,QImage::Format_RGB888);
    QPixmap Bpix = QPixmap::fromImage(bImg);
    ui->bHist->setPixmap(Bpix.scaled(width_img,height_img,Qt::KeepAspectRatio));

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
    Mat origHist = calc_histogram(img->getImage("process"));
    Mat orihHistImg = plot_histogram(origHist, 255, 147, 111);
    QImage processedhist((uchar*)orihHistImg.data, orihHistImg.cols, orihHistImg.rows,QImage::Format_RGB888);
    QPixmap Hpix4 = QPixmap::fromImage(processedhist);
    ui->processedHist->setPixmap(Hpix4.scaled(width_img,height_img,Qt::KeepAspectRatio));


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
    Mat origHist = calc_histogram(img->getImage("process"));
    Mat orihHistImg = plot_histogram(origHist, 255, 147, 111);
    QImage processedhist((uchar*)orihHistImg.data, orihHistImg.cols, orihHistImg.rows,QImage::Format_RGB888);
    QPixmap Hpix4 = QPixmap::fromImage(processedhist);
    ui->processedHist->setPixmap(Hpix4.scaled(width_img,height_img,Qt::KeepAspectRatio));

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



void MainWindow::on_submitEdges_clicked()
{


 Mat originalImg=img->getImage("threshold");
 Mat result;
 int(*mask)[3];

//convertToGrayscale(originalImg,grayScaled);
 if(ui->EdgesFilter->currentText().toStdString()=="Canny"){
     ui->EdgesDirection->hide();
     result=CannyEdgeDetection(originalImg,7,1,100,3);
 }
 else{
     mask=getArray(ui->EdgesFilter->currentText().toStdString(),ui->EdgesDirection->currentText().toStdString());
 result=masking(originalImg,mask);
 }

 QImage image2((uchar*)result.data, result.cols, result.rows,QImage::Format_Grayscale8);

 QPixmap pix = QPixmap::fromImage(image2);
 int width_img=ui->filteredImgEdge->width();
 int height_img=ui->filteredImgEdge->height();
 ui->filteredImgEdge->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));



}


void MainWindow::on_filter_1_btn_clicked()
{

}

