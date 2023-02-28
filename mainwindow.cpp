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
using namespace cv;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
String currentFileName;
Mat dest;

//take path from uploader

QString imgPath = QFileDialog::getOpenFileName(this, "Open an Image", "..", "Images (*.png *.xpm *.jpg *.bmb)");
//read image using opencv

Mat image = imread(imgPath.toStdString());
cvtColor(image, dest,COLOR_BGR2RGB);

//convert image to greyscale
Mat grey;
cvtColor(image, grey,COLOR_BGR2GRAY);

//convert opencv_photo to Qimage to show it in qt
QImage image2((uchar*)dest.data, dest.cols, dest.rows,QImage::Format_RGB888);
QPixmap pix = QPixmap::fromImage(image2);
int width_img=ui->label_img->width();
int height_img=ui->label_img->height();
ui->label_img->setPixmap(pix.scaled(width_img,height_img,Qt::KeepAspectRatio));


QImage image3((uchar*)grey.data, grey.cols, grey.rows,QImage::Format_Grayscale8);
QPixmap pix3 = QPixmap::fromImage(image3);
int width_img3=ui->grey_image->width();
int height_img3=ui->grey_image->height();
ui->grey_image->setPixmap(pix3.scaled(width_img3,height_img3,Qt::KeepAspectRatio));



//Mat threshold;
//cv::threshold(grey,threshold,244, 255, THRESH_BINARY);
}


void MainWindow::on_pushButton_2_clicked()
{

}

