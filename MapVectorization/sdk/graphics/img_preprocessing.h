/// Whiteteam
/// \file img_preprocessing.h
/// \brief This file contains utils
/// \author Whiteteam authors

#ifndef _IMG_PREPROCESSING_H_
#define _IMG_PREPROCESSING_H_
#pragma once

#include "..\app\sdk_const.h"
#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"

SDK_BEGIN_NAMESPACE

cv::Mat& WsharpKernel(double k);
void Wsharpen(cv::Mat& src, cv::Mat& dst, double k);

SDK_END_NAMESPACE

#endif // _IMG_PREPROCESSING_H_
