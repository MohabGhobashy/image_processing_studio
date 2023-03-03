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
    int width_img1=ui->originalImg->width();
    int height_img1=ui->originalImg->height();
    ui->originalImg->setPixmap(pix.scaled(width_img1,height_img1,Qt::KeepAspectRatio));
    ui->filteredImg->setPixmap(pix.scaled(width_img1,height_img1,Qt::KeepAspectRatio));
    ui->originalImgLbl->setText("Original Image");
    ui->filteredImgLbl->setText("Filtered Image");

    // tab 4
//    cvtColor(img->getOriginalImage(), img->getImage("process"), COLOR_BGR2GRAY);
//    QImage processedImg((uchar*)img->getImage("process").data, img->getImage("process").cols, img->getImage("process").rows,QImage::Format_Grayscale8);
//    QPixmap Ppix4 = QPixmap::fromImage(processedImg);
//    int width_img2=ui->originalImg_tab4->width();
//    int height_img2=ui->originalImg_tab4->height();
//    ui->originalImg_tab4->setPixmap(Ppix4.scaled(width_img2,height_img2,Qt::KeepAspectRatio));
//    ui->processedImg->setPixmap(Ppix4.scaled(width_img2,height_img2,Qt::KeepAspectRatio));
//    ui->originalImgLbl_tab4->setText("Original Image");
//    ui->processedImgLbl->setText("Processed Image");
//    ui->equalizeBtn->setDisabled(false);
//    ui->normalizeBtn->setDisabled(false);

    ui->equalizeBtn->setDisabled(false);
    ui->normalizeBtn->setDisabled(false);

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
        cvtColor(img->getOriginalImage(), img->getImage("process"), COLOR_BGR2GRAY);
        QImage processedImg((uchar*)img->getImage("process").data, img->getImage("process").cols, img->getImage("process").rows,QImage::Format_Grayscale8);
        QPixmap Ppix4 = QPixmap::fromImage(processedImg);
        int width_img2=ui->originalImg->width();
        int height_img2=ui->originalImg->height();
        ui->originalImg_tab4->setPixmap(Ppix4.scaled(width_img2,height_img2,Qt::KeepAspectRatio));
        ui->processedImg->setPixmap(Ppix4.scaled(width_img2,height_img2,Qt::KeepAspectRatio));
        ui->originalImgLbl_tab4->setText("Original Image");
        ui->processedImgLbl->setText("Processed Image");

    }
}

