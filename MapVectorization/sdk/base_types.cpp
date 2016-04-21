/// Whiteteam
/// \file base_types.cpp
/// \brief This file contains base types
/// \author Whiteteam authors

#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"


#include  "util/utils.h"
#include  "util/math_utils.h"

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
w_color::w_color(cv::Vec3b color)
{
    this->r = color[2];
    this->g = color[1];
    this->b = color[0];
}
// ------------------------------------------------------------
w_color::w_color(cv::Vec4b color)
{
  this->r = color[2];
  this->g = color[1];
  this->b = color[0];
}
// ------------------------------------------------------------
cv::Vec3b w_color::toVec3b() const
{
  return Vec3b(this->b, this->g, this->r);
}
// ------------------------------------------------------------
inline bool operator <= (const w_color &first, const cv::Vec3b &second)
{
  return bool((first.r <= second[2]) && (first.g <= second[1]) && (first.b <= second[0]));
}
// ------------------------------------------------------------
inline bool operator >= (const w_color &first, const cv::Vec3b &second)
{
  return bool((first.r >= second[2]) && (first.g >= second[1]) && (first.b >= second[0]));
}
// ------------------------------------------------------------
w_range::w_range()
{
    this->low = Vec3b(255, 255, 255);
    this->high = Vec3b(0, 0, 0);
}
// ------------------------------------------------------------
void w_range::addColor(const w_color& color)
{
    this->low[2] = std::min<uchar>(color.r, low[2]);
    this->low[1] = std::min<uchar>(color.g, low[1]);
    this->low[0] = std::min<uchar>(color.b,low[0]);
    this->high[2] = std::max<uchar>(color.r, high[2]);
    this->high[1] = std::max<uchar>(color.g, high[1]);
    this->high[0] = std::max<uchar>(color.b, high[0]);
}
// ------------------------------------------------------------
inline bool w_range::contains(const cv::Vec3b& color)
{
  return ((color >= this->low) && (color <= this->high));
}
// ------------------------------------------------------------
w_color w_range::getLow()
{
  return w_color(this->low);
}
// ------------------------------------------------------------
w_color w_range::getHigh()
{
  return w_color(this->high);
}
// ------------------------------------------------------------
WRaster::WRaster(const std::string& imgPath)
{
  if (imgPath != "")
    Initialize(imgPath);
}
// ------------------------------------------------------------
void WRaster::Initialize(const std::string& imgPath)
{
  // Read the file
  Mat raster = imread(String(imgPath), CV_LOAD_IMAGE_COLOR);   
  cvtColor(raster, m_raster, CV_RGB2RGBA, 4);
}
// ------------------------------------------------------------
SDKResult WRaster::IncreaseSharpness(double k) const
{
  filter2D(m_raster, m_raster, m_raster.depth(), utils::SharpKernel(k));
  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
WLayer* WRaster::AddLayer()
{
  WLayer layer;
  layer.m_data = Mat(m_raster.size(), CV_8UC1);
  layer.m_uuid = utils::genUUID();
  m_layers.push_back(layer);

  return &m_layers.back();
}
// ------------------------------------------------------------
SDKResult WRaster::RemoveLayer(const LayerUUID& layerId)
{
  LayersContainer::const_iterator cit = m_layers.begin();
  for ( ; cit != m_layers.end(); ++cit)
  {
    if (cit->m_uuid == layerId)
      break;
  }

  if (cit == m_layers.end())
    return kSDKResult_NotFound;

  m_layers.remove(*cit);

  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::AddColorToLayer(const LayerUUID& layerId, const w_color& color) const
{
  WLayer* layer = GetLayerById(layerId);
  if (layer == nullptr)
    return kSDKResult_NotFound;

  layer->m_color_range.addColor(color);

  for (int y = 0; y < m_raster.rows; y++)
  {
    for (int x = 0; x < m_raster.cols; x++)
    {
      const Vec3b currentColor = m_raster.at<Vec3b>(y, x);
      layer->m_data.at<uchar>(y, x) = layer->m_color_range.contains(currentColor) ? 1 : 0;
    }
  }

  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::AddLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type) const
{
  return SetLayerType(layerId, type, false);
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type) const
{
  return SetLayerType(layerId, type, true);
}
// ------------------------------------------------------------
SDKResult WRaster::RemoveLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type) const
{
  WLayer* layer = GetLayerById(layerId);
  if (layer == nullptr)
    return kSDKResult_NotFound;
 
  layer->m_type &= ~type;

  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type, bool overwrite) const
{
  WLayer* layer = GetLayerById(layerId);
  if (layer == nullptr)
    return kSDKResult_NotFound;
  
  if (overwrite)
    layer->m_type = type;
  else
    layer->m_type |= type;
  
  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerName(const LayerUUID& layerId, const std::string& name) const
{
  WLayer* layer = GetLayerById(layerId);
  if (layer == nullptr)
    return kSDKResult_NotFound;

  layer->m_name = name;
  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
WLayer* WRaster::GetLayerById(const LayerUUID& layerId) const
{
  for (LayersContainer::const_iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
  {
    if (cit->m_uuid == layerId)
      return const_cast<WLayer*>(&(*cit));    
  }
  return nullptr;
}
// ------------------------------------------------------------
WLayer* WRaster::GetLayerByName(const std::string& name) const
{
  for (LayersContainer::const_iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
  {
    if (cit->m_name == name)
      return const_cast<WLayer*>(&(*cit));  
  }
  return nullptr;
}
// ------------------------------------------------------------
SDKResult WRaster::GetLayersByType(WLayer::LAYER_TYPE type, LayerIDs& layer_ids) const
{
  layer_ids.clear();
  for (LayersContainer::const_iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
  {
    if ((cit->m_type & type) != 0)
      layer_ids.push_back(cit->m_uuid);      
  }
  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
// use only as an example. define other split funcs as a member of the WRaster class
SDKResult splitFuncExample(WLayer* layer, WLayer* l1, WLayer* l2)
{
  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SplitLayer(const LayerUUID& layerId, LayerIDs& splittedLayers)
{
  WLayer* layer = GetLayerById(layerId);
  if (layer == nullptr)
    return kSDKResult_NotFound;

  SDKResult result = kSDKResult_Error;

  __try{
    switch(layer->m_type)
    {
      case WLayer::LAYER_TYPE_ENUM::LT_NONE:
        break;

      case WLayer::LAYER_TYPE_ENUM::LT_LINES | WLayer::LAYER_TYPE_ENUM::LT_TEXT:
        {
        }
        break;

      case WLayer::LAYER_TYPE_ENUM::LT_LINES:
        {
        }
        break;

      case WLayer::LAYER_TYPE_ENUM::LT_TEXT:
        {
        }
        break;

      case WLayer::LAYER_TYPE_ENUM::LT_AREAS:
        {
        }
        break;

      case WLayer::LAYER_TYPE_ENUM::LT_OTHER:
        {
        }
        break;

      default:
        {   
          WLayer* l1 = AddLayer();
          WLayer* l2 = AddLayer();
          splitFuncExample(layer, l1, l2);
          l1 = nullptr;
          l2 = nullptr;
        }
        break;
    
    }
  }
  __finally
  {
    if (S_OK(result))
      RemoveLayer(layer->m_uuid);
    layer = nullptr;
  }
  
  return result;
}
// ------------------------------------------------------------
std::vector<cv::Rect> WRaster::DetectLetters(const LayerUUID& layerId) const
{
	std::vector<cv::Rect> boundRect;

  WLayer* layer = GetLayerById(layerId);
  if (layer == nullptr)
    return boundRect;

	cv::Mat img = layer->m_data;
  cv::Mat img_sobel, img_threshold, element;
  cv::Sobel(img, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
  cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
  
  element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3) );
  cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);
  
  std::vector< std::vector< cv::Point> > contours;
  cv::findContours(img_threshold, contours, 0, 1); 
  
  std::vector<std::vector<cv::Point> > contours_poly(contours.size());
  for( int i = 0; i < contours.size(); i++ )
  {
    if (contours[i].size()>100)
    { 
      cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
      cv::Rect appRect(boundingRect(cv::Mat(contours_poly[i])));
      if (appRect.width>appRect.height) 
        boundRect.push_back(appRect);
    }
  }
  
  return boundRect;
}
// ------------------------------------------------------------

  SDK_END_NAMESPACE
