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

using namespace cv;

Image* img = new Image();
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


}

