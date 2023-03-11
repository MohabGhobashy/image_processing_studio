#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<Threshold.h>
#include<Image.h>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showImg(Mat& img, QLabel* imgLbl, enum QImage::Format imgormat, int width, int hieght);
    int origWidth, origHeight;

private slots:

    void on_actionupload_triggered();


    void on_normalizeBtn_clicked();

    void on_equalizeBtn_clicked();

    void on_tabWidget_tabBarClicked(int index);
    void on_horizontalSlider_valueChanged(int value);

    void on_globalRadio_clicked();

    void on_submitThreshold_clicked();

    void on_cSlider_actionTriggered(int action);

    void on_cSlider_valueChanged(int value);

    void on_blockSizeSlider_valueChanged(int value);

    void on_localRadio_clicked();

    void on_submitEdges_clicked();

    void on_sigmaSlider_valueChanged(int value);

    void on_kernelSlider_valueChanged(int value);

    void on_highThreshold_valueChanged(int value);

    void on_lowThresholdSlider_valueChanged(int value);

    void on_EdgesFilter_currentIndexChanged(int index);

    void on_highThresholdSlider_valueChanged(int value);

    void on_comboBox_currentIndexChanged(int index);

    void on_filter_3_btn_clicked();

    void on_lowpass_btn_clicked();

    void on_highpass_btn_clicked();

    void on_submitThreshold_2_clicked();

    void on_actionupload_2nd_img_triggered();

    void on_saltNoBtn_2_clicked();

    void on_gaussianNoBtn_2_clicked();

    void on_avgNoBtn_2_clicked();

    void on_altFiltBtn_2_clicked();

    void on_gaussianFilBtn_2_clicked();

    void on_avgFiltBtn_2_clicked();

    void on_average_slider_valueChanged(int value);

    void on_GaussianSlider_valueChanged(int value);

    void on_mean_sliderGaussN_actionTriggered(int action);

    void on_mean_sliderGaussN_valueChanged(int value);

    void on_sigma_sliderGaussN_valueChanged(int value);

    void on_paperSlider_valueChanged(int value);

    void on_saltSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
