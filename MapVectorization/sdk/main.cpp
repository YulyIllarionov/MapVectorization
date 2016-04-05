// sdk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// OpenCV include example
#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"
//

int _tmain(int argc, _TCHAR* argv[])
{

  // OpenCV usage example
  cv::Mat img = cv::imread("fon.bmp");
  cv::waitKey();

	return 0;
}

