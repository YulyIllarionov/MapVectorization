/// Whiteteam
/// \file base_types.cpp
/// \brief This file contains base types
/// \author Whiteteam authors

#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"

#include  "util/utils.h"

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

// ------------------------------------------------------------
w_color::w_color(cv::Vec4b color)
{
    this->r = color[2];
    this->g = color[1];
    this->b = color[0];
}
// ------------------------------------------------------------

// ------------------------------------------------------------
cv::Vec3b& w_color::toVec3b()
{
    return Vec3b(this->b, this->g, this->r);
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
  if (img_path != "")
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
int WRaster::SetLayerColor(int layerNumber, w_color& rgbColor)
{
    if (layerNumber >= m_layers.size())
        return 1;
    m_layers.at(layerNumber).m_color = rgbColor.toVec3b();
    return 0;
}
// ------------------------------------------------------------

// ------------------------------------------------------------
int WRaster::SetLayerColor(int layerNumber)
{
    if (layerNumber >= m_layers.size())
        return 1;
    Vec3b  averageColor(0,0,0);
    int number = 0;
    for (int y = 0; y < m_raster.rows; y++)
    {
        for (int x = 0; x < m_raster.cols; x++)
        {
            if (m_layers[layerNumber].m_data.at<uchar>(y, x)>0)
            {
                averageColor += m_raster.at<Vec3b>(y, x);
                number++;
            }
        }
    }
    m_layers[layerNumber].m_color = averageColor / number;
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


//
bool WPolyline::AddPointAt(const CvPoint& point, size_t idx)
{
	if (idx < m_points.size())
	{
		std::vector<CvPoint>::iterator it;
		it = m_points.begin();
		m_points.insert(it + idx, point);
		return true;
	}
	return false;
}

bool WPolyline::RemovePoint(size_t idx)
{
	if (idx < m_points.size())
	{
		std::vector<CvPoint>::iterator it;
		it = m_points.begin();
		m_points.erase(it + idx);
		return true;
	}
	return false;
}

void WPolyline::concat(WPolyline& line)
{
	std::vector<CvPoint> tmp = line.m_points;
	std::reverse(tmp.begin(), tmp.end());
	m_points.insert(m_points.end(), tmp.begin(), tmp.end());
}

void WPolyline::concatTornLine(WPolyline& line, bool firstOrder, bool secondOrder)
{
	std::vector<CvPoint> tmp = line.m_points;
	if (firstOrder && secondOrder)
	{
		m_points.insert(m_points.end(), tmp.begin(), tmp.end());
	}
	else if (firstOrder && !secondOrder)
	{
		std::reverse(tmp.begin(), tmp.end());
		m_points.insert(m_points.end(), tmp.begin(), tmp.end());
	}
	else if (!firstOrder && secondOrder)
	{
		std::reverse(tmp.begin(), tmp.end());
		m_points.insert(m_points.begin(), tmp.begin(), tmp.end());
	}
	else if (!firstOrder && !secondOrder)
	{
		m_points.insert(m_points.begin(), tmp.begin(), tmp.end());
	}
}

WCVPointsContainer WPolyline::simplifyLine(WCVPointsContainer &linevector, double EPSILON, int delta)
{
	int i = 0;
	double k; // curvature
	WCVPointsContainer m_outpoints;
	while ( (i + 2 * delta) <= linevector.size())
	{
		k = (((linevector[i + 2 * delta].y - linevector[i + delta].y) / (linevector[i + 2 * delta].x - linevector[i + delta].x)) / ((linevector[i + delta].y - linevector[i].y) / (linevector[i + delta].x - linevector[i].x))) / pow((1 + pow((linevector[i + delta].y - linevector[i].y) / (linevector[i + delta].x - linevector[i].x), 2)), 3 / 2);
		if (k < EPSILON)
		{
			std::vector<CvPoint>::iterator it = std::find(m_outpoints.begin(), m_outpoints.end(), linevector[i + 2 * delta]);
			if( (linevector[i + 2 * delta].x != (*it).x) && (linevector[i + 2 * delta].y != (*it).y) )
				m_outpoints.push_back(linevector[i + 2 * delta]);
			it = std::find(m_outpoints.begin(), m_outpoints.end(), linevector[i + delta]);
			if( (linevector[i + delta].x != (*it).x) && (linevector[i + delta].y != (*it).y) )
				m_outpoints.push_back(linevector[i + delta]);
			it = std::find(m_outpoints.begin(), m_outpoints.end(), linevector[i + 2 * delta]);
			if( (linevector[i].x != (*it).x) && (linevector[i].y != (*it).y) )
				m_outpoints.push_back(linevector[i]);
		}
		else
		{
			std::vector<CvPoint>::iterator it = linevector.begin() + i;
			while (it != (linevector.begin() + i + 2 * delta))
			{
				std::vector<CvPoint>::iterator it2 = std::find(m_outpoints.begin(), m_outpoints.end(), linevector[i + 2 * delta]);
				if (( (*it).x != (*it2).x) && ((*it).y != (*it2).y))
					m_outpoints.push_back((*it));
			}
		}
		i += delta;
	}
	return m_outpoints;
}

  SDK_END_NAMESPACE
