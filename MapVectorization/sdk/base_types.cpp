/// Whiteteam
/// \file base_types.cpp
/// \brief This file contains base types
/// \author Whiteteam authors

#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"


#include  "util/utils.h"
#include  "util/math_utils.h"

#include <math.h>
#include <algorithm>

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
void WLayer::DrawCircle(SMapPoint point, uint radius, uchar color)
{
    circle(m_data, point.ToPoint(), radius, color, -1);
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
  cvtColor(raster, m_raster, CV_BGR2BGRA, 4);
}
// ------------------------------------------------------------
SDKResult WRaster::IncreaseSharpness(double k) const
{
  //filter2D(m_raster, m_raster, m_raster.depth(), utils::SharpKernel(k));

  Mat kernel = utils::SharpKernel(k);
  Mat img;
  cvtColor(m_raster, img, CV_BGRA2BGR);
  Mat imgOut(img);
  Mat imgGray;
  Mat imgEdges;

  cvtColor(m_raster, imgGray, COLOR_RGB2GRAY);
  Canny(imgGray, imgEdges, 90, 200);
  
  for (int y = 1; y < img.rows - 1; y++)
  {
    for (int x = 1; x < img.cols - 1; x++)
    {
      if (imgEdges.at<uchar>(y, x) > 0)
      {
        Rect region_of_interest = Rect(x - 1, y - 1, 3, 3);
        Mat image_roi = img(region_of_interest);
        Vec3d sum(0.0, 0.0, 0.0);
        Vec3d currentColor;
        for (int y1 = 0; y1 < image_roi.rows; y1++)
        {
          for (int x1 = 0; x1 < image_roi.cols; x1++)
          {
            currentColor = image_roi.at<Vec3b>(y1, x1);
            sum += kernel.at<double>(y1, x1)*currentColor;
          }
        }
        imgOut.at<Vec3b>(y, x) = sum;
      }
    }
  }
  cvtColor(imgOut, m_raster, CV_BGR2BGRA);
  return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
void WRaster::SegmentationBilateral()
{
    Mat img;
    Mat imgOut;
    cvtColor(m_raster, img, CV_BGRA2BGR);
    bilateralFilter(img, imgOut, 9, 50, 0);
    cvtColor(imgOut, m_raster, CV_BGR2BGRA);
}
// ------------------------------------------------------------
void WRaster::SegmentationMeanshift()
{
    Mat img;
    Mat imgOut;
    cvtColor(m_raster, img, CV_BGRA2BGR);
    pyrMeanShiftFiltering(img, imgOut, 5, 40, 0);
    cvtColor(imgOut, m_raster, CV_BGR2BGRA);
}
// ------------------------------------------------------------
WLayer* WRaster::AddLayer(const GroupID& groupId)
{
  WLayer layer;
  layer.m_data = Mat(m_raster.size(), CV_8UC1);
  layer.m_uuid = utils::genUUID();
  layer.m_group_id = groupId.empty() ? utils::genUUID() : groupId;
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
      const Vec4b currentColorOld = m_raster.at<Vec4b>(y, x);
      const Vec3b currentColor(currentColorOld[0], currentColorOld[1], currentColorOld[2]);
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
        result = splitFuncExample(layer, l1, l2);
        l1 = nullptr;
        l2 = nullptr;
        if (S_Ok(result))
          RemoveLayer(layer->m_uuid);
        layer = nullptr;
      }
      break;
  }
  
  return result;
}
// ------------------------------------------------------------
SDKResult WRaster::GetLayersByGroupId(const GroupID& groupId, LayerIDs& relatedLayers) const
{
  relatedLayers.clear();
  for (LayersContainer::const_iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
  {
    if (cit->m_group_id == groupId)
      relatedLayers.push_back(cit->m_uuid);      
  }
  
  return kSDKResult_Succeeded;
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

// define objects inside polygon
std::vector<int> WRaster::DefineObjectsInsidePolygon(
  const WObjectContainer<WVectorObject>& vectorObjects, 
  std::vector<SMapPoint>& mapPoints)
{
	std::vector<int> indexes;//Коллекция индексов внутри коллекции

	return indexes;
}

// copy object from one layer to another
void WRaster::CopyObjectsToAnotherLayer(const LayerUUID& departureLayerId, const LayerUUID& arrivalLayerId, WPolygon mapPoints)
{
    // copy raster
    Rect roi = boundingRect(mapPoints.GetPoints());
    WLayer* departureLayer = GetLayerById(arrivalLayerId);
    WLayer* arrivalLayer = GetLayerById(departureLayerId);
    for (int y = roi.y; y < roi.y + roi.height; y++)
    {
        for (int x = roi.x; x < roi.x + roi.width; x++)
        {
            Point current(x, y);
            if (mapPoints.Contains(current))
                arrivalLayer->m_data.at<uchar>(current) =
                    departureLayer->m_data.at<uchar>(current);
        }
    }

    // copy vector objects
    if (!departureLayer->IsSingleType() || !arrivalLayer->IsSingleType())
      return;

    WLayer::LAYER_TYPE departureType = departureLayer->getType();
    WLayer::LAYER_TYPE arrivalType   = arrivalLayer->getType();

    switch (departureType)
    {
      // from Text layer
      case WLayer::LT_TEXT:
      {
        switch (arrivalType)
        {
        case WLayer::LT_LINES:
          {
            // from Text layer to Lines layer
            
            // e.g.
            // get texts
            std::vector<WText> texts = departureLayer->m_objects_text.GetObjectList();
            // iterate and find
            std::vector<WText>::const_iterator cit = texts.begin();
            for (; cit != texts.end(); cit++)
            {
              // when found
              break;
            }
            departureLayer->m_objects_text.Remove(*cit);
            // create new line object
            WLine line;
            arrivalLayer->m_objects_line.Add(line);
            // profit
          }
          break;
        case WLayer::LT_AREAS:
          {
            // from Text layer to Areas layer
          }
          break;
        case WLayer::LT_OTHER:
          {
            // from Text layer to Other layer
          }
          break;
        }
      }
      break;
      // from Lines layer
      case WLayer::LT_LINES:
      {
        switch (arrivalType)
        {
        case WLayer::LT_TEXT:
          {
            // from Lines layer to Text layer
          }
          break;
        case WLayer::LT_AREAS:
          {
            // from Lines layer to Areas layer
          }
          break;
        case WLayer::LT_OTHER:
          {
            // from Lines layer to Other layer
          }
          break;
        }
      }
      break;
      // from Other layer
      case WLayer::LT_OTHER:
      {
        switch (arrivalType)
        {
        case WLayer::LT_LINES:
          {
            // from Other layer to Lines layer
          }
          break;
        case WLayer::LT_AREAS:
          {
            // from Other layer to Areas layer
          }
          break;
        case WLayer::LT_TEXT:
          {
            // from Other layer to Text layer
          }
          break;
        }
      }
      break;
    }
}

void WRaster::DeleteOblectsFromLayer(const LayerUUID& layerId, WPolygon mapPoints)
{
    // delete raster
    Rect roi = boundingRect(mapPoints.GetPoints());
    WLayer* layer = GetLayerById(layerId);
    for (int y = roi.y; y < roi.y + roi.height; y++)
    {
        for (int x = roi.x; x < roi.x + roi.width; x++)
        {
            Point current(x, y);
            if (mapPoints.Contains(current))
                layer->m_data.at<uchar>(current) = 0;
        }
    }

    
    // delete vector objects
    
    if (!layer->IsSingleType())
      return;

    WLayer::LAYER_TYPE layerType = layer->getType();

    switch (layerType)
    {
      // from Text layer
      case WLayer::LT_TEXT:
      {
        
      }
      break;
      // from Lines layer
      case WLayer::LT_LINES:
      {
        
      }
      break;
      // from Other layer
      case WLayer::LT_OTHER:
      {
        
      }
      break;
    }
}

//Реализация функций для работы с объектами
//Добавить точку
bool WVectorObject::AddPointAt(/*const*/ Point& point, size_t idx)
{
	if (idx < m_points.size())
	{
		std::vector<Point>::iterator it;
		it = m_points.begin();
		m_points.insert(it + idx, point);
		return true;
	}
	return false;
}

bool WVectorObject::RemovePoint(size_t idx)
{
	if (idx < m_points.size())
	{
		std::vector<Point>::iterator it;
		it = m_points.begin();
		m_points.erase(it + idx);
		return true;
	}
	return false;
}

WPolygon::WPolygon(std::vector<SMapPoint> & mapPoints)
{
    for (int i = 0; i < mapPoints.size(); i++) {
        m_points.push_back(Point::Point_(mapPoints[i].GetX(), mapPoints[i].GetY()));
    }
};

bool WPolygon::Contains(/*const*/ Point& point) const
{
    return (pointPolygonTest(m_points, point, false)>=0);
}

void WLine::Concat(const WLine& line)
{
  //std::reverse(line.m_points.begin(), line.m_points.end());
	//m_points.insert(m_points.end(), line.m_points.begin(), line.m_points.end());
  m_points.insert(m_points.end(), line.m_points.rbegin(), line.m_points.rend());
}

void WLine::ConcatTornLine(const WLine& line, bool firstOrder, bool secondOrder)
{
	if (firstOrder && secondOrder)
	{
		m_points.insert(m_points.end(), line.m_points.begin(), line.m_points.end());
	}
	else if (firstOrder && !secondOrder)
	{
		//std::reverse(line.m_points.begin(), line.m_points.end());
		//m_points.insert(m_points.end(), line.m_points.begin(), line.m_points.end());
    m_points.insert(m_points.end(), line.m_points.rbegin(), line.m_points.rend());

	}
	else if (!firstOrder && secondOrder)
	{
		//std::reverse(line.m_points.begin(), line.m_points.end());
		//m_points.insert(m_points.begin(), line.m_points.begin(), line.m_points.end());
    m_points.insert(m_points.end(), line.m_points.rbegin(), line.m_points.rend());
	}
	else if (!firstOrder && !secondOrder)
	{
		m_points.insert(m_points.begin(), line.m_points.begin(), line.m_points.end());
	}
}

bool WLine::BelongsTo(const WPolygon& polygon)
{
  if (m_points.size() == 0)
    return false;
  for (int i = 0; i < m_points.size(); i++)
  {
    if (!polygon.Contains(m_points[i]))
      return false;
  }
  return true;
}

WPointsContainer WLine::SimplifyLine(const WPointsContainer& linevector, double EPSILON, int delta)
{
	int i = 0;
	double k; // curvature
	WPointsContainer outpoints;
	while ((i + 2 * delta) <= linevector.size())
	{
		k = (((linevector[i + 2 * delta].y - linevector[i + delta].y) 
      / (linevector[i + 2 * delta].x - linevector[i + delta].x)) 
      / ((linevector[i + delta].y - linevector[i].y) 
      / (linevector[i + delta].x - linevector[i].x))) 
      / pow((1 + pow((linevector[i + delta].y - linevector[i].y) 
      / (linevector[i + delta].x - linevector[i].x), 2)), 3 / 2);
		if (k < EPSILON)
		{
			std::vector<Point>::const_iterator cit = 
        std::find(outpoints.begin(), outpoints.end(), linevector[i + 2 * delta]);
			if((linevector[i + 2 * delta].x != (*cit).x) && (linevector[i + 2 * delta].y != (*cit).y))
				outpoints.push_back(linevector[i + 2 * delta]);
			
      cit = std::find(outpoints.begin(), outpoints.end(), linevector[i + delta]);
			if((linevector[i + delta].x != (*cit).x) && (linevector[i + delta].y != (*cit).y))
				outpoints.push_back(linevector[i + delta]);
			
      cit = std::find(outpoints.begin(), outpoints.end(), linevector[i + 2 * delta]);
			if((linevector[i].x != (*cit).x) && (linevector[i].y != (*cit).y))
				outpoints.push_back(linevector[i]);
		}
		else
		{
			std::vector<Point>::const_iterator cit = linevector.begin() + i;
			while (cit != (linevector.begin() + i + 2 * delta))
			{
				std::vector<Point>::const_iterator cit2 = 
          std::find(outpoints.begin(), outpoints.end(), linevector[i + 2 * delta]);
				if (((*cit).x != (*cit2).x) && ((*cit).y != (*cit2).y))
					outpoints.push_back((*cit));
			}
		}
		i += delta;
	}
	return outpoints;
}
  SDK_END_NAMESPACE
