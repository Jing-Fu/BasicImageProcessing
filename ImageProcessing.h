#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ImageProcessing.h"
#include "imageEnhancement.h"
#include <opencv2/opencv.hpp>
class ImageProcessing : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessing(QWidget *parent = Q_NULLPTR);

private:
	Ui::ImageProcessingClass ui;
	cv::Mat src;
	imageEnhance *object;
	void showLabel(cv::Mat&);
private slots:
void loadFile();
void meanFilter();
void medianFilter();
void gaussianFilter();
void sobelFilter();
void laplaceFilter();
void histNor();
void histEq();
void erosionFilter();
void dilationFilter();
void openingFilter();
void closingFilter();
void meanThreshold();
void otsuThreshod();
void CCL();
void cannyFilterImage();

};

//規劃mask進行點運算函式