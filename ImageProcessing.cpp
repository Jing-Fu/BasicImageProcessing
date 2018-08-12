#include "ImageProcessing.h"
#include "QFileDialog.h"
#include "qDebug.h"
#include "imageEnhancement.h"
using namespace std;
using namespace cv;
QImage cvMat2QImage(const cv::Mat& mat);
ImageProcessing::ImageProcessing(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.label->setFixedSize(360,256);
	ui.label_2->setFixedSize(360, 256);
	connect(ui.pushButton1,SIGNAL(clicked()),this,SLOT(loadFile()));
	connect(ui.MeanFilter, SIGNAL(clicked()), this, SLOT(meanFilter()));
	connect(ui.MedianFIlter, SIGNAL(clicked()), this, SLOT(medianFilter()));
	connect(ui.GaussianFiter, SIGNAL(clicked()), this, SLOT(gaussianFilter()));
	connect(ui.SobelFilter, SIGNAL(clicked()), this, SLOT(sobelFilter()));
	connect(ui.LaplaceFilter, SIGNAL(clicked()), this, SLOT(laplaceFilter()));
	connect(ui.HistNor, SIGNAL(clicked()), this, SLOT(histNor()));
	connect(ui.HistEq, SIGNAL(clicked()), this, SLOT(histEq()));
	connect(ui.Erosion, SIGNAL(clicked()), this, SLOT(erosionFilter()));
	connect(ui.Dilation, SIGNAL(clicked()), this, SLOT(dilationFilter()));
	connect(ui.Opening, SIGNAL(clicked()), this, SLOT(openingFilter()));
	connect(ui.Closing, SIGNAL(clicked()), this, SLOT(closingFilter()));
	connect(ui.MeanThreshold, SIGNAL(clicked()), this, SLOT(meanThreshold()));
	connect(ui.OtsuThreshold, SIGNAL(clicked()), this, SLOT(otsuThreshod()));
	connect(ui.OtsuThreshold, SIGNAL(clicked()), this, SLOT(otsuThreshod()));
	connect(ui.CCL, SIGNAL(clicked()), this, SLOT(CCL()));
	connect(ui.CannyFilter, SIGNAL(clicked()), this, SLOT(cannyFilterImage()));
}

void ImageProcessing::loadFile()
{
	QString qstr;
	qstr = QFileDialog::getOpenFileName(this,
		tr("OpenImage"), ".",
		tr("ImageFiles(*.png *.jpg *.jpeg* .bmp)"));
	String str = qstr.toStdString();
	Mat img = imread(str);
	cv::cvtColor(img, img, CV_BGR2GRAY);
	img.copyTo(src);
	object = new imageEnhance(src.rows, src.cols, src);
	QImage image = cvMat2QImage(img);
	ui.label->setPixmap(QPixmap::fromImage(image));
	ui.label->setScaledContents(true);
	ui.label->show();
}
void ImageProcessing::meanFilter()
{
	Mat data=object->meanFilter();
	showLabel(data);
}
void ImageProcessing::medianFilter()
{
	Mat data = object->medianFilter();
	showLabel(data);
}
void ImageProcessing::gaussianFilter()
{
	Mat data = object->gaussianFilter();
	showLabel(data);
}

void ImageProcessing::sobelFilter() 
{
	Mat data = object->sobelFilter();
	showLabel(data);
}
void ImageProcessing::laplaceFilter()
{
	Mat data = object->laplaceFilter();
	showLabel(data);
}
void ImageProcessing::histNor()
{
	Mat data = object->histNor();
	showLabel(data);
}
void  ImageProcessing::histEq()
{
	Mat data = object->histEq();
	showLabel(data);
}
void ImageProcessing::erosionFilter()
{
	Mat data = object->erosionFilter();
	showLabel(data);
}
void ImageProcessing::dilationFilter()
{
	Mat data = object->dilationFilter();
	showLabel(data);
}
void ImageProcessing::openingFilter()
{
	Mat data = object->openingFilter();
		showLabel(data);
}
void ImageProcessing::closingFilter()
{
	Mat data = object->closingFilter();
	showLabel(data);
}
void ImageProcessing::meanThreshold()
{
	Mat data = object->meanThreshold();
	showLabel(data);
}
void ImageProcessing::otsuThreshod()
{

	Mat data = object->otsuThreshold();
	showLabel(data);
}
void ImageProcessing::CCL()
{
	Mat data = object->CCL();
	showLabel(data);
}
void ImageProcessing::cannyFilterImage()
{
	Mat data = object->cannyFilterImage();
	showLabel(data);
}
void ImageProcessing::showLabel(Mat &data)
{
	QImage image = cvMat2QImage(data);
	ui.label_2->setPixmap(QPixmap::fromImage(image));
	ui.label_2->setScaledContents(true);
	ui.label_2->show();
}
QImage cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat  
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}