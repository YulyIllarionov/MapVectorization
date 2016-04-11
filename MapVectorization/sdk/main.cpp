﻿// sdk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// OpenCV include example
#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "base_types.h"
#include "util/utils.h"
//

using namespace cv;
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
    SDK_NAMESPACE::WRaster raster("images/part2.jpg");
    SDK_NAMESPACE::utils::SetTransparent(raster.m_raster, Mat(), 127);

    Mat img = imread("images/part2.jpg"); 
    Mat imgSharp1;

    blur(img, imgSharp1, cv::Size(3, 3));
    addWeighted(img, 1.9, imgSharp1, -0.9, 0, imgSharp1);
    
    namedWindow("Map", WINDOW_KEEPRATIO);
    namedWindow("MapSharp1", WINDOW_KEEPRATIO);

    imshow("Map", raster.m_raster);
    imshow("MapSharp1", imgSharp1);

    waitKey();

	return 0;
}

