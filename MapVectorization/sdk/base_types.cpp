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
    m_raster = imread(String(img_path), CV_LOAD_IMAGE_COLOR);   
    cvtColor(m_raster, m_raster, CV_RGB2RGBA, 4);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
void WRaster::IncreaseSharpness(double k)
{
    filter2D(m_raster, m_raster, m_raster.depth(), utils::WsharpKernel(k));
}
// ------------------------------------------------------------

// ------------------------------------------------------------
void WRaster::AddLayer()
{
    WLayer layer;
    layer.m_data = Mat(m_raster.size(), CV_8UC1);
    layer.m_color = Vec3b(-1, -1, -1);
    m_layers.push_back(layer);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
int WRaster::SetLayerMask(int layerNumber, std::vector<uchar> rgbScope)
{
    if (layerNumber >= m_layers.size())
        return 1;
    if (m_raster.size() != m_layers.at(layerNumber).m_data.size())
        return 2;

    uchar rLeft = rgbScope.at(0);
    uchar rRight = rgbScope.at(0);
    uchar gLeft = rgbScope.at(1);
    uchar gRight = rgbScope.at(1);
    uchar bLeft = rgbScope.at(2);
    uchar bRight = rgbScope.at(3);

    for (int y = 0; y < m_raster.rows; y++)
    {
        for (int x = 0; x < m_raster.rows; x++)
        {
            if (((m_raster.at<Vec4b>(y, x)[0] > rLeft) && (m_raster.at<Vec4b>(y, x)[0] < rRight)) &&
                ((m_raster.at<Vec4b>(y, x)[1] > gLeft) && (m_raster.at<Vec4b>(y, x)[1] < gRight)) &&
                ((m_raster.at<Vec4b>(y, x)[3] > bLeft) && (m_raster.at<Vec4b>(y, x)[3] < bRight)))
                m_layers.at(layerNumber).m_data.at<uchar>(y, x) = 1;
            else
                m_layers.at(layerNumber).m_data.at<uchar>(y, x) = 0;
        }
    }
    return 0;
}
// ------------------------------------------------------------

// ------------------------------------------------------------
int WRaster::SetLayerColor(int layerNumber, std::vector<uchar> rgbColor)
{
    if (layerNumber >= m_layers.size())
        return 1;
    m_layers.at(layerNumber).m_color = 
        Vec3b(rgbColor.at(0), rgbColor.at(1), rgbColor.at(2));
    return 0;
}
// ------------------------------------------------------------

  SDK_END_NAMESPACE
