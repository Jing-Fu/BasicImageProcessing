#pragma once
#ifndef IMAGEENHANCEMENT_H
#define	IMAGEENHANCEMENT_H
#include <opencv2/opencv.hpp>
#include "Canny.h"
class imageEnhance
{
private:
	cv::Mat src;
	int imgHeight;
	int imgWidth;

public:
	imageEnhance(int, int, cv::Mat&);
	cv::Mat meanFilter();
	cv::Mat medianFilter();
	cv::Mat gaussianFilter();
	cv::Mat sobelFilter();
	cv::Mat laplaceFilter();
	cv::Mat histNor();
	cv::Mat histEq();
	cv::Mat erosionFilter();
	cv::Mat dilationFilter();
	cv::Mat openingFilter();
	cv::Mat closingFilter();
	cv::Mat meanThreshold();
	cv::Mat otsuThreshold();
	cv::Mat CCL();
	cv::Mat cannyFilterImage();
};
#endif // 
