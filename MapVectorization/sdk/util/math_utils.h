/// Whiteteam
/// \file math_utils.h
/// \brief This file contains math utils
/// \author Whiteteam authors


#ifndef _SDK_MATH_UTILS_H_
#define _SDK_MATH_UTILS_H_
#pragma once

#include "..\app\sdk_const.h"
#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"

SDK_BEGIN_NAMESPACE

class MathUtils
{
public:
  // return maximum of two values
  template<typename T> static T Max(T val1, T val2);
  // return minimum of two values
  template<typename T> static T Min(T val1, T val2);

  class Wvector
  {
  public:
      Wvector();
      Wvector(cv::Point2f p1, cv::Point2f p2);
      void operator = (const Wvector& other);
      cv::Point2f Begin() { return begin; }
      cv::Point2f End() { return end; };
      double Lenght();
      Wvector ÑodirectionalVec(double lenght);
      Wvector OppositeVec();
      Wvector PerpendicularVec();
      Wvector BisectorVec(Wvector vec);
      cv::Point2f Middle();

  //private:
      cv::Point2f begin;
      cv::Point2f end;
  };
  Wvector MinLengthVector(Wvector vec1, Wvector vec2);
 
};

SDK_END_NAMESPACE


#endif // _SDK_MATH_UTILS_H_
