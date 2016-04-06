/// Whiteteam
/// \file img_preprocessing.cpp
/// \brief This file contains image preprocessing functions
/// \author Whiteteam authors

#include "stdafx.h"
#include "img_preprocessing.h"

SDK_BEGIN_NAMESPACE

// ----------------------------------------------------
// Inline Функция:
//       1. именование: WsharpKernel(float k) функция возвращающая ядро свертки для увеличения резкости 
//       2. аргументы функции:  k - коэффицент ядра
// ----------------------------------------------------
inline cv::Mat& WsharpKernel(double k)
{
    cv::Mat kernel(3, 3, CV_64F, -0.125);
    kernel *= k;
    kernel.at<double>(1, 1) = k + 1;
    return kernel;
}

// ----------------------------------------------------
//Функция:
//       1. именование: Wsharpen(cv::Mat& src, cv::Mat& dst, double k) функция вычисляет 
//          корреляцию изображения с ядром возвращаемым функцией WsharpKernel(...)
//       2. аргументы функции:    src - исходное изображение, dst - изображение с повышенной резкостью,
//          k - коэффицент ядра увеличения резкости
// ----------------------------------------------------
void Wsharpen(cv::Mat& src, cv::Mat& dst, double k)
{
    cv::filter2D(src, dst, src.depth(), WsharpKernel(k));
}

SDK_END_NAMESPACE
