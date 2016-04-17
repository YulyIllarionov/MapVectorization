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
w_color::w_color(uchar r, uchar g, uchar b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}
// ------------------------------------------------------------

// ------------------------------------------------------------
w_color::w_color(cv::Vec4b color)
{
    this->r = color[2];
    this->g = color[1];
    this->b = color[0];
}
// ------------------------------------------------------------

// ------------------------------------------------------------
inline bool operator <= (const w_color &first, const cv::Vec4b &second)
{
    return bool((first.r <= second[2]) && (first.g <= second[1]) && (first.b <= second[0]));
}
// ------------------------------------------------------------

// ------------------------------------------------------------
inline bool operator >= (const w_color &first, const cv::Vec4b &second)
{
    return bool((first.r >= second[2]) && (first.g >= second[1]) && (first.b >= second[0]));
}
// ------------------------------------------------------------

// ------------------------------------------------------------
WRaster::WRaster(std::string img_path)
{
  Initialize(img_path);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
void WRaster::Initialize(std::string img_path)
{
    // Read the file
    Mat raster = imread(String(img_path), CV_LOAD_IMAGE_COLOR);   
    cvtColor(raster, m_raster, CV_RGB2RGBA, 4);
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
int WRaster::SetLayerMask(int layerNumber, const w_color& colorLow, const w_color &colorHigh)
{
    if (layerNumber >= m_layers.size())
        return 1;
    if (m_raster.size() != (m_layers[layerNumber]).m_data.size())
        return 2;

    for (int y = 0; y < m_raster.rows; y++)
    {
        for (int x = 0; x < m_raster.cols; x++)
        {
            const Vec4b currentColor = m_raster.at<Vec4b>(y, x);
            m_layers.at(layerNumber).m_data.at<uchar>(y, x) =
                ((colorLow <= currentColor) && (colorHigh >= currentColor)) ? 1 : 0;
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

// ------------------------------------------------------------
int WRaster::SetLayerType(int layerNumber, WLayer::LAYER_TYPE type)
{
    if (layerNumber >= m_layers.size())
        return 1;
    m_layers.at(layerNumber).m_type = type;
    return 0;
}
// ------------------------------------------------------------

// ------------------------------------------------------------
int WRaster::SetLayerName(int layerNumber, std::string name)
{
    if (layerNumber >= m_layers.size())
        return 1;
    (m_layers[layerNumber]).m_name = name;
    return 0;
}
// ------------------------------------------------------------

// ------------------------------------------------------------
std::vector<cv::Rect> WRaster::detectLetters(int layerNumber)
{
	std::vector<cv::Rect> boundRect;
	cv::Mat img = m_layers.at(layerNumber).m_data;
    cv::Mat img_sobel, img_threshold, element;
    cv::Sobel(img, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3) );
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1); 
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
        if (contours[i].size()>100)
        { 
            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
            cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
            if (appRect.width>appRect.height) 
                boundRect.push_back(appRect);
        }
    return boundRect;
}
// ------------------------------------------------------------

  SDK_END_NAMESPACE
