// sdk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// OpenCV include example
#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"
//

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    Mat img = imread("images/jp1263.jpg"); 

    namedWindow("Map", WINDOW_KEEPRATIO);
    imshow("Map", img);

    waitKey();

	return 0;
}

