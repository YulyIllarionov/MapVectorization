/// Whiteteam
/// \file skeletonization.h
/// \brief This file contains skeletonization
/// \author Whiteteam authors
/// \usage 
///  cv::Mat skeleton;
///  SDK_NAMESPACE::WSkeletonizer::Instance().Skeletonize(image, skeleton);

#ifndef _VECTORIZATION_APP_SKELETONIZATION_H_
#define _VECTORIZATION_APP_SKELETONIZATION_H_
#pragma once

#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "..\app\sdk_const.h"
#include "..\base_types.h"
#include <vector>

SDK_BEGIN_NAMESPACE

#define WCOLOR_WHITE 255


class WSkeletonizer
{

  enum PointTypeEnum
  {
    Undefined = 0x0,
    Background = 0x1,
    Foreground = 0x2,
    Any = Background | Foreground,  // Background or foreground
    C = 0x2,                        // Central pixel
    Y = 0x4,                        // At least one of them should be Background
    Marked = 0x8,                   // Marked
  };

  enum ConcavePixel
  {
    UP = 0x0,
    DOWN = 0x1,
    RIGHT = 0x2,
    LEFT = 0x4
  };

  typedef unsigned char    SPoint;
  typedef std::vector<SPoint>   SRow;
  typedef std::vector<SRow>     WSkeleton;

  class SkeletonTemplate
  {
  public:

    SkeletonTemplate(int row, int column, int center_x, int center_y);
    WSkeleton m_points;
    std::pair<int, int> m_center;
 
  private:

    SkeletonTemplate() {};
  };

public:

  static WSkeletonizer& Instance() {
    static WSkeletonizer instance;
    return instance;
  }

  bool Skeletonize(const cv::Mat& raster, cv::Mat& skeleton);

private:

  WSkeletonizer();
  WSkeletonizer(const WSkeletonizer&);
  WSkeletonizer& operator=(WSkeletonizer&);

  // Initialize main templates
  void InitializeTemplates();

  // Initialize extended templates
  void InitializeTemplatesExt();

  static bool InitializeSkeletonByRaster(const cv::Mat& raster, WSkeleton& skeleton);

  // pattern matching 
  // [out] index of matched pattern
  int MatchPatterns(const WSkeleton& skeleton, int coord_x, int coord_y);

  // try matching certain pattern
  static bool MatchPattern(const SkeletonTemplate& _template, const WSkeleton& skeleton, 
    int coord_x, int coord_y);

  // is concave corner pixel
  static bool IsConcaveCornelPixel(const WSkeleton& skeleton, int coord_x, int coord_y);
  // return concave corner pixels
  static int ConcaveCornelPixels(const WSkeleton& skeleton, 
    int coord_x, int coord_y, int pattern_num);


  std::vector<SkeletonTemplate> m_templates;
  std::vector<SkeletonTemplate> m_templates_ext;

};


SDK_END_NAMESPACE

#endif // _VECTORIZATION_APP_SKELETONIZATION_H_
