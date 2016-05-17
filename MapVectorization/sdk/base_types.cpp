/// Whiteteam
/// \file base_types.cpp
/// \brief This file contains base types
/// \author Whiteteam authors

#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"


#include "util/utils.h"
#include "util/math_utils.h"
#include "graphics/skeletonization.h"
#include <stack>

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
SDKResult WLayer::InicializeLinesContainer()
{
    if(m_type != LT_LINES)
        return kSDKResult_Error;
    WObjectContainer lines = SDK_NAMESPACE::utils::FindLinesOnMat(m_data);
    m_objects.insert(m_objects.end(), lines.begin(), lines.end());
}
// ------------------------------------------------------------
void WLayer::InicializeVectorContainer()
{
    switch (this->m_type)
    {
    case WLayer::LAYER_TYPE_ENUM::LT_NONE:
        break;
        
    case WLayer::LAYER_TYPE_ENUM::LT_LINES:
    {
        InicializeLinesContainer();
    }
    break;

    case WLayer::LAYER_TYPE_ENUM::LT_TEXT:
    {
    }
    break;
    
    case WLayer::LAYER_TYPE_ENUM::LT_OTHER:
    {
    }
    break;

    default:
        break;
    }
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
  layer.m_data = Mat(m_raster.size(), CV_8UC1, Scalar(0));
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
    case WLayer::LAYER_TYPE_ENUM::LT_LINES | WLayer::LAYER_TYPE_ENUM::LT_TEXT | WLayer::LAYER_TYPE_ENUM::LT_OTHER:
    {
        WLayer* linesLayer = this->AddLayer(layer->getGroupId());
        this->SetLayerType(linesLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_LINES);
        this->SetLayerName(linesLayer->getID(), std::string("Lines from ") + layer->getName());
        WLayer* othersLayer = this->AddLayer(layer->getGroupId());
        this->SetLayerType(othersLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_TEXT);
        this->SetLayerType(othersLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_OTHER);
        this->SetLayerName(othersLayer->getID(), std::string("Text from ")+layer->getName());
        this->SplitLines(layerId, linesLayer->getID(), othersLayer->getID());
        linesLayer->InicializeVectorContainer();
        splittedLayers.clear();
        splittedLayers.push_back(linesLayer->getID());
        splittedLayers.push_back(othersLayer->getID());
        break;
    }
    case WLayer::LAYER_TYPE_ENUM::LT_LINES | WLayer::LAYER_TYPE_ENUM::LT_TEXT:
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
std::vector<int> WRaster::DefineObjectsNearPoint(const LayerUUID& layerId, SMapPoint point)
{
    Point pointCV(point.GetX(), point.GetY());
    std::vector<int> ids;
    WLayer* layer = GetLayerById(layerId);
    if (!layer->IsSingleType())
        return ids;

    double distance = DBL_MAX;
    int index;

    for (size_t i = 0; i < layer->m_objects.size(); i++)
    {
        double currentDistance = layer->m_objects[i].DistanceTo(pointCV);
        if (currentDistance < distance)
        {
            distance = currentDistance;
            index = i;
        }
    }
    ids.push_back(index);
    return ids;
}
// ------------------------------------------------------------
// define objects inside polygon
std::vector<int> WRaster::DefineObjectsInsidePolygon(const LayerUUID& layerId, const WPolygon& mapPoints)
{
    std::vector<int> ids;
    WLayer* layer = GetLayerById(layerId);
    if (!layer->IsSingleType() || mapPoints.Length() == 0)
        return ids;
    
    int idx = 0;
    WObjectContainer::const_iterator cit = layer->m_objects.begin();
    for (; cit != layer->m_objects.end(); ++cit, ++idx)
    {
        if (mapPoints.Contains(*cit))
          ids.push_back(idx);
    }
    
	  return ids;
}
// ------------------------------------------------------------
// copy object from one layer to another
void WRaster::CopyObjectsToAnotherLayer(const LayerUUID& departureLayerId, const LayerUUID& arrivalLayerId, WPolygon mapPoints)
{
    // copy raster
    Rect roi = boundingRect(mapPoints.m_points); // GetPoints());
    WLayer* departureLayer = GetLayerById(arrivalLayerId);
    WLayer* arrivalLayer = GetLayerById(departureLayerId);
    for (int y = roi.y; y < roi.y + roi.height; y++)
    {
        for (int x = roi.x; x < roi.x + roi.width; x++)
        {
            //Point current(x, y);
            //if (mapPoints.Contains(current))
            //    arrivalLayer->m_data.at<uchar>(current) =
            //        departureLayer->m_data.at<uchar>(current);
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
// ------------------------------------------------------------
void WRaster::DeleteOblectsFromLayer(const LayerUUID& layerId, WPolygon mapPoints)
{
    // delete raster
    WLayer* layer = GetLayerById(layerId);
    if (mapPoints.Length() > 1)
    {
        Rect roi = boundingRect(mapPoints.m_points); // GetPoints());
        for (int y = roi.y; y < roi.y + roi.height; y++)
        {
            for (int x = roi.x; x < roi.x + roi.width; x++)
            {
                Point current(x, y);
                //if (mapPoints.Contains(current))
                //    layer->m_data.at<uchar>(current) = 0;
            }
        }
    }

    // delete vector objects    
    if (!layer->IsSingleType())
      return;

    std::vector<int> ids = DefineObjectsInsidePolygon(layerId, mapPoints);
    WLayer::LAYER_TYPE layerType = layer->getType();

    switch (layerType)
    {
      // from Text layer
      case WLayer::LT_TEXT:
      {
          //for (int i = 0; i < ids.size(); i++)
          //    layer->m_objects_text.RemoveById(i);
      }
      break;
      // from Lines layer
      case WLayer::LT_LINES:
      {
          //for (int i = 0; i < ids.size(); i++)
          //    layer->m_objects_line.RemoveById(i);
      }
      break;
      // from Other layer
      case WLayer::LT_OTHER:
      {
        
      }
      break;
    }
}
// ------------------------------------------------------------
WPolygon::WPolygon(std::vector<SMapPoint> & mapPoints)
{
    for (int i = 0; i < mapPoints.size(); i++) {
        m_points.push_back(Point::Point_(mapPoints[i].GetX(), mapPoints[i].GetY()));
    }
};
// ------------------------------------------------------------
bool WPolygon::Contains(const WVectorObject& object) const
{
  bool result = false;
  WPointsContainer::const_iterator cit = object.m_points.begin();
  for (; cit != object.m_points.end(); cit++)
  {
    if (pointPolygonTest(m_points, *cit, false) >= 0)
      result = true;
    else
      return false;
  }
  return result;
}
// ------------------------------------------------------------
double WVectorObject::DistanceTo(cv::Point mapPoint) const
{
    int squaredDistance = INT_MAX;
    for (size_t i = 0; i < m_points.size(); i++)
    {
        int currentDistance = SDK_NAMESPACE::utils::squaredDistanceBetween(mapPoint, m_points[i]);
        if (currentDistance < squaredDistance)
            squaredDistance = currentDistance;
    }
    return std::sqrt((double)squaredDistance);
}
// ------------------------------------------------------------
void WLine::Concat(const WLine& line)
{
    std::vector<int> distances;
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.front(), line.m_points.front()));
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.front(), line.m_points.back()));
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.back(), line.m_points.front()));
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.back(), line.m_points.back()));
    std::vector<int>::iterator result = std::min_element(distances.begin(), distances.end());
    if (result == distances.end())
        return;
    switch (*result)
    {
    case 0:
        m_points.insert(m_points.begin(), line.m_points.rbegin(), line.m_points.rend());
        break;
    case 1:
        m_points.insert(m_points.begin(), line.m_points.begin(), line.m_points.end());
        break;
    case 2:
        m_points.insert(m_points.end(), line.m_points.rbegin(), line.m_points.rend());
        break;
    case 3:
        m_points.insert(m_points.end(), line.m_points.begin(), line.m_points.end());
        break;
    default:
        break;
    }
}
// ------------------------------------------------------------
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
// ------------------------------------------------------------
Wregion::Wregion(cv::Point point, cv::Mat& img)
{
    if (img.at<uchar>(point) == 0)
        return;

    std::stack <cv::Point> stack;

    stack.emplace(point);

    while (!stack.empty())
    {
        cv::Point currentPoint = stack.top();
        stack.pop(); // Удалить точку из стека
        points.push_back(currentPoint); //Добавить точку к результату

        for (int k = currentPoint.x - 1; k <= currentPoint.x + 1; k++)
        {
            if (k == -1) // Следим за границами изображения
                continue;
            if (k == img.cols)
                break;

            for (int l = currentPoint.y - 1; l <= currentPoint.y + 1; l++)
            {
                if ((l == -1) || ((l == point.y) && (k == point.x)))
                    continue;
                if (l == img.rows)
                    break;

                if (img.at<uchar>(l, k) != 0)
                    stack.emplace(k, l);
            }
        }
        img.at<uchar>(currentPoint) = 0; // Закрасить на изображении
    }
}
// ------------------------------------------------------------
cv::Rect Wregion::boundingRectangle()
{
    return boundingRect(points);
}
// ------------------------------------------------------------
int Wregion::Square()
{
    return (int)points.size();
}
// ------------------------------------------------------------
bool Wregion::IsLine()
{
    float radius;
    minEnclosingCircle(points, Point2f(), radius);
    double ratio = (double)this->Square() / radius / radius;
    return (ratio < 0.07);
}
// ------------------------------------------------------------
void Wregion::drawOn(Mat& img, uchar color)
{
    for (int i = 0; i < points.size(); i++)
    {
        //if ((points[i].x<img.cols)&& (points[i].y<img.rows))
        img.at<uchar>(points[i]) = color;
    }
}
// ------------------------------------------------------------
SDKResult WRaster::SplitLines(const LayerUUID& layerId, const LayerUUID& linesLayerID, const LayerUUID& othersLayerID)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NullPointer;

    WLayer* linesLayer = GetLayerById(linesLayerID);
    //linesLayer->m_data = Mat(layer->m_data.size(), layer->m_data.type(), 0);

    WLayer* othersLayer = GetLayerById(othersLayerID);
    //othersLayer->m_data = Mat(layer->m_data.size(), layer->m_data.type(), 0);

    Mat temp = layer->m_data.clone();
    for (int y = 1; y < temp.rows - 1; y++)
    {
        for (int x = 1; x < temp.cols - 1; x++)
        {
            if (temp.at<uchar>(y, x)>0)
            {
                Wregion region(Point(x, y), temp);
                if (region.Square() > 4)
                {
                    if (region.IsLine())
                        region.drawOn(linesLayer->m_data, 1);
                    else
                        region.drawOn(othersLayer->m_data, 1);
                }
            }
        }
    }

}
// ------------------------------------------------------------
  SDK_END_NAMESPACE
