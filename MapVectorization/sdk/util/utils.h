/// Whiteteam
/// \file utils.h
/// \brief This file contains utils
/// \author Whiteteam authors

#ifndef _SDK_UTILS_H_
#define _SDK_UTILS_H_
#pragma once
 
#include <string>

#include "..\app\sdk_const.h"

#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"

SDK_BEGIN_NAMESPACE

namespace utils
{  
  std::string WSTR2STR(const std::wstring& wide);
  inline cv::Mat WsharpKernel(double k);
  void SetTransparent(cv::Mat& img, cv::Mat& mask, uchar aTrue, uchar aFalse = 255);
}

SDK_END_NAMESPACE

#endif // _SDK_UTILS_H_
