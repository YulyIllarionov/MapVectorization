// algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;



int main()
{
    Mat image;
    image = imread("../sample/map/a2056.jpg", CV_LOAD_IMAGE_COLOR);

    namedWindow("map", WINDOW_KEEPRATIO);// Create a window for display.
    imshow("map", image);

    waitKey(0);

    return 0;
}

