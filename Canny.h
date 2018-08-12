#pragma once
#ifndef CANNYFILTER_H
#define CANNYFILTER_H
#include<opencv2/opencv.hpp>
class cannyFilter
{
private:
	double **getGaussionArray(int size, double sigma);
public:
	void ConvertRGB2GRAY(const cv::Mat, cv::Mat &);
	void gaussionFilter(const cv::Mat, cv::Mat &);
	void sobelGradDirction(const cv::Mat, cv::Mat &, cv::Mat &, cv::Mat &, cv::Mat &);
	void nonMaxSup(const cv::Mat, const cv::Mat, cv::Mat &);
	void nonMaxSup1(const cv::Mat, const cv::Mat, const cv::Mat, cv::Mat &);
	void DoubleThreshold(const cv::Mat, cv::Mat &, double lowThreshold, double highThreshold);
	void DoubleThresholdLink(cv::Mat &, double lowThreshold, double highThreshold);

	void doubleThresholdTest(cv::Mat &non, const cv::Mat grad, int low, int high); //test 2/3
	cv::Mat threshold(cv::Mat, int, int);
};

#endif // CANNYFILTER_H

