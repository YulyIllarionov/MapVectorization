/// Whiteteam
/// \file img_preprocessing.cpp
/// \brief This file contains image preprocessing functions
/// \author Whiteteam authors

#include "stdafx.h"
#include "img_preprocessing.h"

SDK_BEGIN_NAMESPACE

// ----------------------------------------------------
// Inline �������:
//       1. ����������: WsharpKernel(float k) ������� ������������ ���� ������� ��� ���������� �������� 
//       2. ��������� �������:  k - ���������� ����
// ----------------------------------------------------
inline cv::Mat& WsharpKernel(double k)
{
    cv::Mat kernel(3, 3, CV_64F, -0.125);
    kernel *= k;
    kernel.at<double>(1, 1) = k + 1;
    return kernel;
}

// ----------------------------------------------------
//�������:
//       1. ����������: Wsharpen(cv::Mat& src, cv::Mat& dst, double k) ������� ��������� 
//          ���������� ����������� � ����� ������������ �������� WsharpKernel(...)
//       2. ��������� �������:    src - �������� �����������, dst - ����������� � ���������� ���������,
//          k - ���������� ���� ���������� ��������
// ----------------------------------------------------
void Wsharpen(cv::Mat& src, cv::Mat& dst, double k)
{
    cv::filter2D(src, dst, src.depth(), WsharpKernel(k));
}

SDK_END_NAMESPACE
