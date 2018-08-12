#include "imageEnhancement.h"
using namespace cv;
using namespace std;
#define MAX_COMPONENTS 100
int getConnectedComponentLabeling(Mat &img, unsigned char *link);
int Mymin(int *v, unsigned char *link);
imageEnhance::imageEnhance(int height, int width, Mat &src)
{
	this->src = src;
	imgHeight = height;
	imgWidth = width;
}
Mat imageEnhance::cannyFilterImage()
{
	Mat grayImg, gauImg, tmpx, tmpy, tmpxy, theta, nonsup, thres;
	cannyFilter *tmp = new cannyFilter();
	//tmp->ConvertRGB2GRAY(src, grayImg);
	tmp->gaussionFilter(src, gauImg);
	tmp->sobelGradDirction(gauImg, tmpx, tmpy, tmpxy, theta);
	tmp->nonMaxSup(tmpxy, theta, nonsup);
	//tmp->nonMaxSup1(tmpxy,tmpx,tmpy,nonsup);

	//tmp->doubleThresholdTest(nonsup, tmpxy, 40,80);
	//tmp->DoubleThreshold(nonsup,thres,90,160);
	//tmp->DoubleThresholdLink(thres, 40, 80);
	thres = tmp->threshold(nonsup, 40, 80);
	return thres;
}
Mat imageEnhance::meanFilter()
{
	Mat dst = Mat::zeros(Size(imgWidth, imgHeight), src.type());
	Mat src2;
	copyMakeBorder(src, src2, 1, 1, 1, 1, BORDER_REPLICATE);
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			dst.at<uchar>(i, j) = (src2.at<uchar>(i, j) + src2.at<uchar>(i, j + 1) + src2.at<uchar>(i, j + 2) +
				src2.at<uchar>(i + 1, j) + src2.at<uchar>(i + 1, j + 1) + src2.at<uchar>(i + 1, j + 2) +
				src2.at<uchar>(i + 2, j) + src2.at<uchar>(i + 2, j + 1) + src2.at<uchar>(i + 2, j + 2)) / 9;
		}
	}
	return dst;
}
Mat imageEnhance::medianFilter()
{
	Mat dst = Mat::zeros(Size(imgWidth, imgHeight), src.type());
	Mat src2;
	copyMakeBorder(src, src2, 1, 1, 1, 1, BORDER_REPLICATE);
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{

			int num = 0;
			unsigned char window[9];
			for (int x = i - 1; x < i + 2; ++x)
			{
				for (int y = j - 1; y < j + 2; ++y)
				{
					window[num++] = src2.at<uchar>(x + 1, y + 1);
				}
			}

			for (int m = 0; m < 5; ++m)
			{
				int min = m;
				for (int n = m + 1; n < 9; ++n)
				{
					if (window[n] < window[min])
					{
						min = n;
					}
				}
				unsigned char tmp = window[m];
				window[m] = window[min];
				window[min] = tmp;
			}

			dst.at<uchar>(i, j) = window[4];
		}
	}
	return dst;
}

Mat imageEnhance::gaussianFilter()
{
	int templates[25] = { 1, 4, 7, 4, 1,
		4, 16, 26, 16, 4,
		7, 26, 41, 26, 7,
		4, 16, 26, 16, 4,
		1, 4, 7, 4, 1 };
	Mat src2;
	copyMakeBorder(src, src2, 2, 2, 2, 2, BORDER_REPLICATE);
	Mat dst = Mat::zeros(Size(imgWidth, imgHeight), CV_8U);
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			int sum = 0;
			int index = 0;

			for (int x = i - 2; x < i + 3; x++)
			{
				for (int y = j - 2; y < j + 3; y++)
				{
					sum += src2.at<uchar>(x + 2, y + 2)*templates[index++];
				}
			}

			sum /= 273;
			if (sum > 255)
				sum = 255;
			dst.at<uchar>(i, j) = sum;
		}
	}
	return dst;
}
Mat imageEnhance::sobelFilter()
{
	Mat dst = Mat::zeros(Size(imgWidth, imgHeight), CV_8U);
	int templates_x[9] = { 1,0,-1,2,0,-2,1,0,-1 };
	int templates_y[9] = { 1,2,1,0,0,0,-1,-2,-1 };
	for (int i = 1; i < imgHeight - 1; i++)
	{
		for (int j = 1; j < imgWidth - 1; j++)
		{
			int Gx = 0;
			int Gy = 0;
			int Mag = 0;
			/*	int index1 = 0;
			int index2 = 0;
			for (int x = i - 1; x < i + 2; x++)
			{
			for (int y = j - 1; y < j + 2; y++)
			{
			Gx += testImg.at<uchar>(x, y)*templates_x[index1++];
			Gy += testImg.at<uchar>(x, y)*templates_y[index2++];
			}
			}
			Gx =abs(Gx);
			Gy =abs(Gy);*/
			Gx = ((src.at<uchar>(i - 1, j - 1) - src.at<uchar>(i - 1, j + 1) +
				2 * src.at<uchar>(i, j - 1) - 2 * src.at<uchar>(i, j + 1) +
				src.at<uchar>(i + 1, j - 1) - src.at<uchar>(i + 1, j + 1)));
			Gy = ((src.at<uchar>(i - 1, j - 1) + 2 * src.at<uchar>(i - 1, j) + src.at<uchar>(i - 1, j + 1) -

				src.at<uchar>(i + 1, j - 1) - 2 * src.at<uchar>(i + 1, j) - src.at<uchar>(i + 1, j + 1)));
			Mag = abs(Gx) + abs(Gy);
			//dst.at<uchar>(i, j) = Mag;

			dst.at<uchar>(i, j) = Mag > 255 ? 255 : Mag;
			dst.at<uchar>(i, j) = Mag < 0 ? 0 : Mag;
		
			/*if (Mag > 255)
				dst.at<uchar>(i, j) = 255;
			else if(Mag<0)
				dst.at<uchar>(i, j) = 0;
			else 
				dst.at<uchar>(i, j) = Mag;*/
		}
	}
	return dst;
}
Mat imageEnhance::laplaceFilter()
{
	Mat dst = Mat::zeros(src.size(), CV_8U);
	int templates[9] = { 0,-1,0,-1,4,-1,0,-1,0 };
	for (int i = 1; i < imgHeight - 1; i++)
	{
		for (int j = 1; j < imgWidth - 1; j++)
		{
			int G = 0;
			int index = 0;
			for (int x = i - 1; x < i + 2; x++)
			{
				for (int y = j - 1; y < j + 2; y++)
				{
					G += src.at<uchar>(x, y)*templates[index++];

				}
			}
			dst.at<uchar>(i, j) = G > 255 ? 255 : G;
			dst.at<uchar>(i, j) = G < 0 ? 0 : G;
		}
	}
	//int templates[25] = { 0, 0, -1, 0, 0,
	//	0, -1, -2, -1, 0,
	//	-1, -2, 16 , -2, -1,
	//	0, -1, -2, -1, 0,
	//	0, 0, -1, 0, 0 };

	//for (int i = 2; i < imgHeight - 2; i++)
	//{
	//	for (int j = 2; j < imgWidth - 2; j++)
	//	{
	//		int G = 0;
	//		int index = 0;

	//		for (int x = i - 2; x < i + 3; x++)
	//		{
	//			for (int y = j - 2; y < j + 3; y++)
	//			{
	//				G += src.at<uchar>(x, y)*templates[index++];

	//			}
	//		}

	//		dst.at<uchar>(i, j) = G > 255 ? 255 : G;
	//		dst.at<uchar>(i, j) = G < 0 ? 0 : G;

	//	}
	//}
	return dst;
}
Mat imageEnhance::histNor()
{
	Mat dst = Mat::zeros(src.size(), src.type());
	int hist[256];
	int histMin = 255;
	int histMax = 0;
	int histSize = 256;
	for (int i = 0; i < histSize; i++)
	{
		hist[i] = 0;
	}

	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			hist[src.at<uchar>(i, j)]++;
			if (src.at<uchar>(i, j) < histMin)
				histMin = src.at<uchar>(i, j);
			if (src.at<uchar>(i, j) > histMax)
				histMax = src.at<uchar>(i, j);
		}
	}

	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			dst.at<uchar>(i, j) = ((float)(src.at<uchar>(i, j) - histMin) / (histMax - histMin)) * 255;
			cout << dst.at<uchar>(i, j);
		}
	}

	return dst;
}
Mat imageEnhance::histEq()
{
	Mat dst = Mat::zeros(src.size(), src.type());
	int hist[256];
	int histCDF[256];
	int histEq[256];
	int histSize = 256;

	for (int i = 0; i < histSize; i++)
	{
		hist[i] = 0;
		histCDF[i] = 0;
		histEq[i] = 0;
	}
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			hist[src.at<uchar>(i, j)]++;
		}
	}
	bool firstVal = false;
	for (int i = 0; i < histSize; i++)
	{
		if (hist[i] != 0)
		{
			if (firstVal)
			{
				histCDF[i] = hist[i] + histCDF[i - 1];
			}
			else
			{
				histCDF[i] = hist[i];
			}
			firstVal = true;
		}
		else if (firstVal)
		{
			histCDF[i] = histCDF[i - 1];
		}
		else
		{
			histCDF[i] = 0;
		}
	}
	unsigned int CDFmin = (~0);
	int color = 0;
	for (int i = 0; i < 256; i++)
	{
		if (histCDF[i] < CDFmin&&histCDF[i] != 0)
		{
			CDFmin = histCDF[i];
		}
	}
	for (int i = 0; i < imgHeight; i++)
	{

		for (int j = 0; j < imgWidth; j++)
		{
			color = (histCDF[src.at<uchar>(i, j)] - CDFmin) * 255 / (imgHeight*imgWidth - CDFmin);

			dst.at<uchar>(i, j) = color;
		}
	}
	return dst;
}

Mat imageEnhance::erosionFilter()
{

	Mat dst = Mat::zeros(src.size(), src.type());
	src.copyTo(dst);
	int mtx[9] = { 255,255,255,255,255,255,255,255,255 };
	threshold(src, src, 120, 255, THRESH_BINARY);
	for (int i = 1; i < imgHeight - 1; i++)
	{
		for (int j = 1; j < imgWidth - 1; j++)
		{

			if (src.at<uchar>(i - 1, j - 1) == mtx[0] &&
				src.at<uchar>(i - 1, j) == mtx[1] &&
				src.at<uchar>(i - 1, j + 1) == mtx[2] &&
				src.at<uchar>(i, j - 1) == mtx[3] &&
				src.at<uchar>(i, j) == mtx[4] &&
				src.at<uchar>(i, j + 1) == mtx[5] &&
				src.at<uchar>(i + 1, j - 1) == mtx[6] &&
				src.at<uchar>(i + 1, j) == mtx[7] &&
				src.at<uchar>(i + 1, j + 1) == mtx[8])

			{
				dst.at<uchar>(i, j) = 255;
			}
			else
			{
				dst.at<uchar>(i, j) = 0;
			}


		}
	}

	return dst;
}
Mat imageEnhance::dilationFilter()
{
	Mat dst = Mat::zeros(src.size(), src.type());

	int mtx[9] = { 255,255,255,255,255,255,255,255,255 };
	threshold(src, src, 120, 255, THRESH_BINARY);
	for (int i = 1; i < imgHeight - 1; i++)
	{
		for (int j = 1; j < imgWidth - 1; j++)
		{


			/*if (mtx[0] == 255)
			{
				dst.at<uchar>(i - 1, j - 1) = 255;
			}
			if (mtx[1] = 255)
			{
				dst.at<uchar>(i, j - 1) = 255;
			}
			if (mtx[2] = 255)
			{
				dst.at<uchar>(i + 1, j + 1) = 255;
			}
			if (mtx[3] = 255)
			{
				dst.at<uchar>(i - 1, j) = 255;
			}
			if (mtx[4] = 255)
			{
				dst.at<uchar>(i, j) = 255;
			}
			if (mtx[5] = 255)
			{
				dst.at<uchar>(i + 1, j) = 255;
			}
			if (mtx[6] = 255)
			{
				dst.at<uchar>(i - 1, j + 1) = 255;
			}
			if (mtx[1] = 255)
			{
				dst.at<uchar>(i, j + 1) = 255;
			}
			if (mtx[1] = 255)
			{
				dst.at<uchar>(i + 1, j + 1) = 255;
			}*/
			if (src.at<uchar>(i - 1, j - 1) == mtx[0] ||
				src.at<uchar>(i - 1, j) == mtx[1] ||
				src.at<uchar>(i - 1, j + 1) == mtx[2] ||
				src.at<uchar>(i, j - 1) == mtx[3] ||
				src.at<uchar>(i, j) == mtx[4] ||
				src.at<uchar>(i, j + 1) == mtx[5] ||
				src.at<uchar>(i + 1, j - 1) == mtx[6] ||
				src.at<uchar>(i + 1, j) == mtx[7] ||
				src.at<uchar>(i + 1, j + 1) == mtx[8])
			{
				dst.at<uchar>(i, j) = 255;
			}

			else
				dst.at<uchar>(i, j) = 0;
		}
	}
	return dst;
}

Mat imageEnhance::openingFilter()
{
	Mat dst = erosionFilter();
	this->src = dst;
	Mat dst1 = dilationFilter();

	return dst1;
}

Mat imageEnhance::closingFilter()
{
	Mat dst = dilationFilter();
	this->src = dst;
	Mat dst1 = erosionFilter();

	return dst1;
}

Mat imageEnhance::meanThreshold()
{
	Mat dst = Mat::zeros(src.size(), src.type());
	/*Mat src2;
	copyMakeBorder(src, src2, 1, 1, 1, 1, BORDER_REPLICATE);*/
	int pixelTotal = 0;
	int pixelMean = 0;
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			pixelTotal += src.at<uchar>(i, j);
		}
	}
	pixelMean = pixelTotal / (imgHeight*imgWidth);

	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			if (src.at<uchar>(i, j) > pixelMean)
				dst.at<uchar>(i, j) = 255;
			else
				dst.at<uchar>(i, j) = 0;
		}
	}
	return dst;

}

Mat imageEnhance::otsuThreshold()
{
	Mat dst = Mat::zeros(src.size(), src.type());
	int hisCount[256] = { 0 };
	float hisPro[256] = { 0 };
	int i, j, pixelSum = imgHeight*imgWidth, threshold = 0;
	for (i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			hisCount[src.at<uchar>(i, j)]++;
		}
	}
	for (i = 0; i < 256; i++)
	{
		hisPro[i] = (float)hisCount[i];
	}

	float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;
	//w0,w1背景和前景的總個數
	//u0tmp,u1tmp背景和前景的灰階總和
	//u0,u1 背景和前景平均灰階
	//deltaTmp,deltaMax 當前分差 最大分差
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i)   //背景部分  
			{
				w0 += hisPro[j];
				u0tmp += j * hisPro[j];
			}
			else   //前景部分  
			{
				w1 += hisPro[j];
				u1tmp += j * hisPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2));
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			dst.at<uchar>(i, j) = (src.at<uchar>(i, j) > threshold) ? 255 : 0;
		}
	}
	return dst;
	//Mat dst = Mat::zeros(src.size(), src.type());
	//int hisMtx[256];
	//int CDFMtx[256];
	//for (int i = 0; i < 256; i++)
	//{
	//	hisMtx[i] = 0;
	//	CDFMtx[i] = 0;
	//}
	//
	//for (int i = 0; i < imgHeight; i++)
	//{
	//	for (int j = 0; j < imgWidth; j++)
	//	{
	//		hisMtx[src.at<uchar>(i, j)]++;
	//	}
	//}
	//for (int i = 0; i < imgHeight; i++)
	//{
	//	for (int j = 0; j < imgWidth; j++)
	//	{
	//		hisMtx[src.at<uchar>(i, j)]/(src.rows*src.cols);
	//	}
	//}
	//bool firstVal = false;
	//for (int i = 0; i < 256; i++)
	//{
	//	if (hisMtx[i] != 0)
	//	{
	//		if (firstVal)
	//		{
	//			CDFMtx[i] = hisMtx[i] + CDFMtx[i - 1];
	//		}
	//		else
	//		{
	//			CDFMtx[i] = hisMtx[i];
	//		}
	//		firstVal = true;
	//	}
	//	else if (firstVal)
	//	{
	//		CDFMtx[i] = CDFMtx[i - 1];
	//	}
	//	else
	//		CDFMtx[i] = 0;
	//}
	//int count;
	//unsigned int q1, q2, u1, u2;
	//unsigned int csum, sum;
	//int betweenVariance;
	//int OtsuTH = 0;
	//int max;
	//int totalP;
	//totalP = imgHeight*imgWidth;

	//for (count = 0, sum = 0; count < 256; count++)
	//{
	//	sum += count*hisMtx[count];
	//}
	//for (max = 0, csum = 0, count = 0; count < 256; count++)
	//{
	//
	//	if (CDFMtx[count] == 0) { continue; }
	//	q1 = CDFMtx[count];
	//	q2 = totalP - q1;
	//	if (q2 == 0) { break; }
	//	csum += count*hisMtx[count];
	//	u1 = csum / q1;
	//	u2 = (sum - csum) / q2;
	//	betweenVariance = q1*q2*(u1 - u2)*(u1 - u2);
	//	if (betweenVariance > max)
	//	{
	//		max = betweenVariance;
	//		OtsuTH = count;
	//	}
	//}


}



Mat imageEnhance::CCL()
{
	srand(time(NULL));
	Mat dst;
	Mat dst0=Mat::zeros(src.size(), CV_8UC3);
	threshold(src, dst, 128, 255, THRESH_BINARY);

	unsigned char link[MAX_COMPONENTS];
	for (int i = 1; i < MAX_COMPONENTS; i++)
		link[i - 1] = i;
	getConnectedComponentLabeling(dst, link);

	int tmpPix;
	for (int i = 1; i < dst.rows - 1; i++)
	{
		for (int j = 1; j < dst.cols - 1; j++)
		{
			if (dst.at<uchar>(i, j) != 0)
			{
				tmpPix = link[dst.at<uchar>(i, j) - 1];
				dst.at<uchar>(i, j) = tmpPix;
			}
		}
	}
	
	int buffer[255];
	int cnt = 0;
	int k;
	for (int i = 1; i < dst.rows - 1; i++)
	{
		for (int j = 1; j < dst.cols - 1; j++)
		{
			if (dst.at<uchar>(i, j) != 0)
			{
				if (cnt == 0)
				{
					buffer[cnt++] = dst.at<uchar>(i, j);
				}
				else
				{
					for (k = 0; k < cnt; k++)
					{
						if (buffer[k] == dst.at<uchar>(i, j))
							break;
					}
					cout << k;
					if (k == cnt)
						buffer[cnt++] = dst.at<uchar>(i, j);

				}
			}
		}

	}


	for (k = 0; k < cnt; k++)
	{
		uchar gray = 255 * rand();
		Vec3b color; //用 color 变量描述一种 RGB 颜色
		color[0] = (rand() % 255); //0通道的B 分量
		color[1] = (rand() % 255); //1通道的G 分量
		color[2] = (rand() % 255); //2通道的R 分量
		for (int i = 0; i < dst.rows - 1; i++)
		{
			for (int j = 0; j < dst.cols - 1; j++)
			{
				if (dst.at<uchar>(i, j) == buffer[k])
				{

					dst.at<uchar>(i, j) = gray;
					dst0.at<Vec3b>(i, j)[0] = color[0];
					dst0.at<Vec3b>(i, j)[1] = color[1];
					dst0.at<Vec3b>(i, j)[2] = color[2];
				}
			}
		}

	}

	return dst0;
}
int Mymin(int *v, unsigned char *link)
{
	int i;
	int index = 88;
	unsigned short min = { 500 };
	for (int i = 0; i < 4; i++)
	{
		if (v[i] < min && v[i] != 0)
		{
			min = v[i];
			index = i;
		}
	}
	if (index == 88)
	{
		return 0;
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			if (v[i] != 0)
			{
				link[v[i] - 1] = link[v[index] - 1];

			}
		}
		return v[index];
	}
}
int getConnectedComponentLabeling(Mat &img, unsigned char *link)
{
	int i, j;
	int currlabel = 1;
	int label;
	int a[4];
	for (i = 1; i < img.rows - 1; i++)
	{
		for (j = 1; j < img.cols - 1; j++)
		{
			if (img.at<uchar>(i, j) != 0)
			{
				a[0] = img.at<uchar>(i - 1, j - 1);
				a[1] = img.at<uchar>(i - 1, j);
				a[2] = img.at<uchar>(i - 1, j + 1);
				a[3] = img.at<uchar>(i, j - 1);
				label = Mymin(a, link);
				if (label != 0 && label != 255)
				{
					img.at<uchar>(i, j) = label;
				}
				else
				{
					img.at<uchar>(i, j) = currlabel;
					currlabel++;
				}
			}
		}
	}
	return (currlabel - 1);
}
