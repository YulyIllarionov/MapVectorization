/// Whiteteam
/// \file base_types.cpp
/// \brief This file contains base types
/// \author Whiteteam authors

#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"

#include  "util/utils.h"

#include "base_types.h"

SDK_BEGIN_NAMESPACE

using namespace cv;

// ------------------------------------------------------------
WRaster::WRaster(std::string img_path)
{
  Initialize(img_path);
}
// ------------------------------------------------------------
void WRaster::Initialize(std::string img_path)
{
  // Read the file
  m_raster = imread(cv::String(img_path), CV_LOAD_IMAGE_COLOR);   
}
// ------------------------------------------------------------

// ------------------------------------------------------------
void WRaster::IncreaseSharpness(double k)
{
    cv::filter2D(m_raster, m_raster, m_raster.depth(), utils::WsharpKernel(k));
}
// ------------------------------------------------------------

  SDK_END_NAMESPACE
