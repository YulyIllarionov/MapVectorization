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
#include "opencv2/text.hpp"
#include "../base_types.h"

SDK_BEGIN_NAMESPACE
   
namespace utils
{  
  std::string WSTR2STR(const std::wstring& wide);
  // Функция возвращающая ядро свертки для увеличения резкости 
  cv::Mat SharpKernel(double k);
  //Функция изменяющая прозрачность изображения по маске
  //Используется в графическом интерфейсе
  void SetTransparent(cv::Mat& img, cv::Mat& mask, uchar alphaTrue, uchar alphaFalse=255, bool needTrue=true, bool needFalse=true);
  std::string genUUID();
  
  inline bool operator < (cv::Vec3b& first, cv::Vec3b& second);
  inline bool operator > (cv::Vec3b& first, cv::Vec3b& second);
  inline bool operator <= (cv::Vec3b& first, cv::Vec3b& second);
  inline bool operator >= (cv::Vec3b& first, cv::Vec3b& second);
  //Возвращает вектор из "ненулевых" соседей пикселя
  std::vector<cv::Point> getNeghboursClockwise(cv::Point point, const cv::Mat& image);
  //Проверяет пиксель на принадлежность границе изображения
  bool isEdgePoint(cv::Point point, const cv::Mat& image);
  //Возвращает квадрат расстояния между точками
  int squaredDistanceBetween(const cv::Point& a, const cv::Point& b);
  //Векторизация - нахождение линий на растровом слое
  WObjectContainer FindLinesOnMat(const cv::Mat& img);
  //Векторизация - локализация текста на растровом слое
  WObjectContainer FindTextOnMat(const cv::Mat& img);
  //TODO 
  std::vector<cv::Rect> DetectOnlyLetters(const cv::Mat &img);
  //TODO
  void  ErDraw(const cv::Mat &src, cv::Mat &dst, cv::text::ERStat& er);
  //TODO
  void  ErDraw(const cv::Mat &src, cv::Mat &dst, cv::Rect region);
  //
  double SquareFilling(const cv::Mat &image, cv::Point center, int radius);
  //
  void FitSize(cv::Rect& rect, cv::Size size);
}

SDK_END_NAMESPACE

#endif // _SDK_UTILS_H_
