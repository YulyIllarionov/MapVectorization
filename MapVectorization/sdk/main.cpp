// sdk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"


int _tmain(int argc, _TCHAR* argv[])
{
  cv::Mat img = cv::imread("fon.bmp"); //Считываем картинку в матрицу img
  cv::waitKey();

	return 0;
}

