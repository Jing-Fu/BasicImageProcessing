#include "Canny.h"

#include<iostream>
using namespace cv;
using namespace std;
void connectLow(Mat &non, const Mat grad, int y, int x, int low);
void cannyFilter::ConvertRGB2GRAY(const cv::Mat image, cv::Mat &grayImg)
{
	if (!image.data || image.channels() != 3)
	{
		return;
	}
	grayImg = Mat::zeros(image.size(), CV_8UC1);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			grayImg.at<uchar>(i, j) = (uchar)((image.at<Vec3b>(i, j)[0] * 0.114) + (image.at<Vec3b>(i, j)[1] * 0.587) + (image.at<Vec3b>(i, j)[2] * 0.299));
		}
	}
}
double ** cannyFilter::getGaussionArray(int size, double sigma)
{
	int i, j;
	double sum = 0.0;
	const double PI = 4.0*atan(1.0);
	int center = size / 2; //以第一個點的座標為原點，求出中心點的座標  

	double **arr = new double*[size];//建立size*size大小的二维陣列  
	for (i = 0; i < size; ++i)
		arr[i] = new double[size];

	for (i = 0; i < size; ++i)
		for (j = 0; j < size; ++j) {
			arr[i][j] = (1 / (2 * PI*sigma*sigma))*exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (2 * sigma*sigma));
			sum += arr[i][j];
		}
	for (i = 0; i < size; ++i)
	{
		for (j = 0; j < size; ++j)
		{
			arr[i][j] /= sum;
			//cout << arr[i][j] << "  ";
		}
		//cout << endl << endl;
	}
	return arr;
}
void cannyFilter::gaussionFilter(const Mat _src, Mat &_dst)
{
	if (!_src.data) return;
	double **arr;
	Mat tmp, tmp1;

	cv::copyMakeBorder(_src, tmp, 1, 1, 1, 1, BORDER_REPLICATE);
	tmp1 = Mat::zeros(tmp.rows - 2, tmp.cols - 2, CV_8UC1);
	for (int i = 1; i < _src.rows - 1; ++i)
		for (int j = 1; j < _src.cols - 1; ++j)
		{
			//邊緣不處理
			if ((i - 1) > 0 && (i + 1) < _src.rows && (j - 1) > 0 && (j + 1) < _src.cols)
			{
				arr = getGaussionArray(5, 1);//自定義權重
				tmp1.at<uchar>(i - 1, j - 1) = 0;

				for (int x = 0; x < 5; x++)
				{
					for (int y = 0; y < 5; y++)
					{
						tmp1.at<uchar>(i - 1, j - 1) += (arr[x][y] * (double)tmp.at<uchar>(i + 2 - x, j + 2 - y));
						//int a = tmp1.at<uchar>(i - 1, j - 1);
					}
				}
			}
		}
	tmp1.copyTo(_dst);
}
void cannyFilter::sobelGradDirction(const Mat _src, Mat & dx, Mat & dy, Mat &dxy, Mat & theta)
{
	Mat img_d, img_pad;
	_src.convertTo(img_d, CV_8UC1);
	// Padding of image
	cv::copyMakeBorder(img_d, img_pad, 1, 1, 1, 1, BORDER_REPLICATE);
	const double PI = 4.0*atan(0.8);
	dx = Mat::zeros(img_pad.rows - 2, img_pad.cols - 2, CV_64FC1);
	dy = Mat::zeros(img_pad.rows - 2, img_pad.cols - 2, CV_64FC1);
	dxy = Mat::zeros(img_pad.rows - 2, img_pad.cols - 2, CV_8UC1);
	theta = Mat::zeros(img_pad.rows - 2, img_pad.cols - 2, CV_64FC1);
	int gx, gy, gxy;
	for (int i = 1; i < _src.rows - 1; i++)
	{
		for (int j = 1; j < _src.cols - 1; j++)
		{
			/*dx.at<uchar>(i - 1, j - 1) = ((img_pad.at<uchar>(i - 1, j - 1) - img_pad.at<uchar>(i - 1, j + 1) +
			2 * img_pad.at<uchar>(i, j - 1) - 2 * img_pad.at<uchar>(i, j + 1) +
			img_pad.at<uchar>(i + 1, j - 1) - img_pad.at<uchar>(i + 1, j + 1)));

			dy.at<uchar>(i - 1, j - 1) = ((img_pad.at<uchar>(i - 1, j - 1) + 2 * img_pad.at<uchar>(i - 1, j) +
			img_pad.at<uchar>(i - 1, j + 1) - img_pad.at<uchar>(i + 1, j - 1)
			- 2 * img_pad.at<uchar>(i + 1, j) - img_pad.at<uchar>(i + 1, j + 1)));
			dxy.at<uchar>(i - 1, j - 1) = (double)sqrt((dx.at<uchar>(i - 1, j - 1)* dx.at<uchar>(i - 1, j - 1)) + (dy.at<uchar>(i - 1, j - 1)* dy.at<uchar>(i - 1, j - 1)));*/

			gx = ((img_pad.at<uchar>(i - 1, j - 1) - img_pad.at<uchar>(i - 1, j + 1) +
				2 * img_pad.at<uchar>(i, j - 1) - 2 * img_pad.at<uchar>(i, j + 1) +
				img_pad.at<uchar>(i + 1, j - 1) - img_pad.at<uchar>(i + 1, j + 1)));

			gy = ((img_pad.at<uchar>(i - 1, j - 1) + 2 * img_pad.at<uchar>(i - 1, j) +
				img_pad.at<uchar>(i - 1, j + 1) - img_pad.at<uchar>(i + 1, j - 1)
				- 2 * img_pad.at<uchar>(i + 1, j) - img_pad.at<uchar>(i + 1, j + 1)));
			dx.at<double>(i - 1, j - 1) = (gx);
			dy.at<double>(i - 1, j - 1) = (gy);
			gxy = (sqrt((gx* gx) + (gy*gy)));
			if (gxy > 255) //Unsigned Char Fix
				gxy = 255;
			dxy.at<uchar>(i - 1, j - 1) = gxy;


			//theta.at<double>(i - 1, j - 1) = ((double)(atan2(gy, gx) / PI) * 180);
			if (gx == 0)
				theta.at<double>(i - 1, j - 1) = 90;
			else
				theta.at<double>(i - 1, j - 1) = ((double)(atan2(gy, gx)));

		}
	}



}
void cannyFilter::nonMaxSup1(const Mat _src, const Mat dx, const Mat dy, Mat &non)
{
	int gx, gy;
	int g1, g2, g3, g4;
	double weight;
	double dTemp, dTemp1, dTemp2;
	non = Mat(_src.rows - 2, _src.cols - 2, CV_8UC1);
	for (int i = 1; i < _src.rows - 1; i++)
	{
		for (int j = 1; j < _src.cols - 1; j++)
		{
			if (_src.at<uchar>(i, j) == 0)
			{
				non.at<uchar>(i - 1, j - 1) = 0;
			}
			else
			{
				dTemp = _src.at<uchar>(i, j);
				gx = dx.at<uchar>(i - 1, j - 1);
				gy = dy.at<uchar>(i - 1, j - 1);
				if (abs(gy) > abs(gx))
				{
					weight = fabs(gx) / fabs(gy);
					g2 = _src.at<uchar>(i - 1, j);
					g4 = _src.at<uchar>(i + 1, j);

					if (gx*gy > 0)
					{
						g1 = _src.at<uchar>(i - 1, j - 1);
						g3 = _src.at<uchar>(i + 1, j + 1);
					}
					else
					{
						g1 = _src.at<uchar>(i - 1, j + 1);
						g3 = _src.at<uchar>(i + 1, j - 1);
					}
				}
				else
				{
					weight = fabs(gy) / fabs(gx);
					g2 = _src.at<uchar>(i, j + 1);
					g4 = _src.at<uchar>(i, j - 1);
					if (gx*gy > 0)
					{
						g1 = _src.at<uchar>(i + 1, j + 1);
						g3 = _src.at<uchar>(i - 1, j - 1);
					}
					else
					{
						g1 = _src.at<uchar>(i - 1, j + 1);
						g3 = _src.at<uchar>(i + 1, j - 1);
					}
				}
				dTemp1 = weight*g1 + (1 - weight)*g2;
				dTemp2 = weight*g3 + (1 - weight)*g4;
				if (dTemp >= dTemp1 && dTemp >= dTemp2)
				{
					non.at<uchar>(i - 1, j - 1) = 128;
				}
				else
				{
					non.at<uchar>(i - 1, j - 1) = 0;
				}
			}
		}
	}
}
void cannyFilter::nonMaxSup(const Mat _src, const Mat the, Mat &non)
{


	non = Mat(_src.rows - 2, _src.cols - 2, CV_8UC1);
	for (int i = 1; i< _src.rows - 1; i++)
	{
		for (int j = 1; j<_src.cols - 1; j++)
		{
			double Tangent = the.at<double>(i, j);

			non.at<uchar>(i - 1, j - 1) = _src.at<uchar>(i, j);
			//Horizontal Edge
			if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
			{
				if ((_src.at<uchar>(i, j) < _src.at<uchar>(i, j + 1)) || (_src.at<uchar>(i, j) < _src.at<uchar>(i, j - 1)))
					non.at<uchar>(i - 1, j - 1) = 0;

			}
			//Vertical Edge
			else if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
			{
				if ((_src.at<uchar>(i, j) < _src.at<uchar>(i + 1, j)) || (_src.at<uchar>(i, j) < _src.at<uchar>(i - 1, j)))
					non.at<uchar>(i - 1, j - 1) = 0;

			}

			//-45 Degree Edge
			else if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
			{
				if ((_src.at<uchar>(i, j) < _src.at<uchar>(i + 1, j + 1)) || (_src.at<uchar>(i, j) < _src.at<uchar>(i - 1, j - 1)))
					non.at<uchar>(i - 1, j - 1) = 0;

			}

			//45 Degree Edge
			else if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
			{
				if ((_src.at<uchar>(i, j) < _src.at<uchar>(i - 1, j + 1)) || (_src.at<uchar>(i, j) < _src.at<uchar>(i + 1, j - 1)))
					non.at<uchar>(i - 1, j - 1) = 0;


			}

		}
	}
}

void cannyFilter::DoubleThreshold(Mat ori, Mat &imageIput, double lowThreshold, double highThreshold)
{
	imageIput = Mat(ori.rows, ori.cols, ori.type());
	//imageIput = Mat(ori.rows, ori.cols, CV_8UC1);
	for (int i = 0; i<ori.rows; i++)
	{
		for (int j = 0; j<ori.cols; j++)
		{
			if (ori.at<uchar>(i, j)>highThreshold)
			{
				imageIput.at<uchar>(i, j) = 255;
			}
			if (ori.at<uchar>(i, j)<lowThreshold)
			{
				imageIput.at<uchar>(i, j) = 0;
			}
		}
	}
}
void cannyFilter::DoubleThresholdLink(Mat &imageInput, double lowThreshold, double highThreshold)
{
	for (int i = 1; i<imageInput.rows - 1; i++)
	{
		for (int j = 1; j<imageInput.cols - 1; j++)
		{
			if (imageInput.at<uchar>(i, j)>lowThreshold&&imageInput.at<uchar>(i, j)<255)
			{
				if (imageInput.at<uchar>(i - 1, j - 1) == 255 || imageInput.at<uchar>(i - 1, j) == 255 || imageInput.at<uchar>(i - 1, j + 1) == 255 ||
					imageInput.at<uchar>(i, j - 1) == 255 || imageInput.at<uchar>(i, j) == 255 || imageInput.at<uchar>(i, j + 1) == 255 ||
					imageInput.at<uchar>(i + 1, j - 1) == 255 || imageInput.at<uchar>(i + 1, j) == 255 || imageInput.at<uchar>(i + 1, j + 1) == 255)
				{
					imageInput.at<uchar>(i, j) = 255;
					DoubleThresholdLink(imageInput, lowThreshold, highThreshold);  
				}
				else
				{
					imageInput.at<uchar>(i, j) = 0;
				}
			}
		}
	}
}
Mat cannyFilter::threshold(Mat imgin, int low, int high)
{
	if (low > 255)
		low = 255;
	if (high > 255)
		high = 255;
	//imgin.convertTo(imgin, CV_8UC1, 255);
	Mat EdgeMat = Mat(imgin.rows, imgin.cols, CV_8UC1);

	for (int i = 0; i<imgin.rows; i++)
	{
		for (int j = 0; j<imgin.cols; j++)
		{
			EdgeMat.at<uchar>(i, j) = abs(imgin.at<uchar>(i, j));
			if (EdgeMat.at<uchar>(i, j) > high)
				EdgeMat.at<uchar>(i, j) = 255;
			else if (EdgeMat.at<uchar>(i, j) < low)
				EdgeMat.at<uchar>(i, j) = 0;
			else
			{
				bool anyHigh = false;
				bool anyBetween = false;
				for (int x = i - 1; x < i + 2; x++)
				{
					for (int y = j - 1; y<j + 2; y++)
					{
						if (x <= 0 || y <= 0 || EdgeMat.rows || y > EdgeMat.cols) //Out of bounds
							continue;
						else
						{
							if (EdgeMat.at<uchar>(x, y) > high)
							{
								EdgeMat.at<uchar>(i, j) = 255;
								anyHigh = true;
								break;
							}
							else if (EdgeMat.at<uchar>(x, y) <= high && EdgeMat.at<uchar>(x, y) >= low)
								anyBetween = true;
						}
					}
					if (anyHigh)
						break;
				}
				if (!anyHigh && anyBetween)
					for (int x = i - 1; x < i + 2; x++)
					{
						for (int y = j - 1; y<j + 2; y++)
						{
							if (x < 0 || y < 0 || x > EdgeMat.rows || y > EdgeMat.cols) //Out of bounds
								continue;
							else
							{
								if (EdgeMat.at<uchar>(x, y) > high)
								{
									EdgeMat.at<uchar>(i, j) = 255;
									anyHigh = true;
									break;
								}
							}
						}
						if (anyHigh)
							break;
					}
				if (!anyHigh)
					EdgeMat.at<uchar>(i, j) = 0;
			}
		}
	}

	return EdgeMat;
}


void cannyFilter::doubleThresholdTest(Mat &non, const Mat grad, int low, int high)
{
	//if (low<0 || high<0 || low >= high)
	//	getThresholds(src, grad, imgSize, &high, &low);

	for (int i = 1; i<grad.rows - 1; i++)
	{
		for (int j = 1; j<grad.cols - 1; j++)
		{
			if ((non.at<uchar>(i - 1, j - 1)) == 128 && grad.at<uchar>(i, j) >= high)
			{
				non.at<uchar>(i - 1, j - 1) = 255;
				connectLow(non, grad, i, j, low);
			}
		}
	}

	for (int i = 1; i<grad.rows - 1; i++)
	{
		for (int j = 1; j<grad.cols - 1; j++)
		{
			if (non.at<uchar>(i - 1, j - 1) != 255)
				non.at<uchar>(i - 1, j - 1) = 0;
		}
	}
}
void connectLow(Mat &non, const Mat grad, int y, int x, int low)
{
	int xNum[8] = { 1,1,0,-1,-1,-1,0,1 };
	int yNum[8] = { 0,1,1,1,0,-1,-1,-1 };
	for (int i = 0; i < 8; i++)
	{
		int xx = x + xNum[i];
		int yy = y + yNum[i];
		if (xx >= 0 && xx < grad.cols - 1 && yy >= 0 && yy < grad.rows - 1)
		{
			if ((non.at<uchar>(y - 1, x - 1) == 128 && (grad.at<uchar>(y, x) >= low)))
			{
				non.at<uchar>(y - 1, x - 1) = 255;
				connectLow(non, grad, yy, xx, low);
			}
		}
	}
}