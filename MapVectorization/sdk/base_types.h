/// Whiteteam
/// \file base_types.h
/// \brief This file contains base types
/// \author Whiteteam authors

#ifndef _SDK_BASE_TYPES_H_
#define _SDK_BASE_TYPES_H_
#pragma once

#include <vector>

#include "app/sdk_const.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cxcore.h"


typedef std::vector<CvPoint>                WCVPointsContainer;
typedef unsigned char                      WColor;


SDK_BEGIN_NAMESPACE

//  Enumerator
class IEnumItem {
public:
  // return true - next item
  // return false - end of enum
  virtual bool Next(IEnumItem* item) const = 0;

  // reset the enumerator
  virtual void Reset() const = 0;

protected:
  virtual ~IEnumItem() {}
};
// ------------------------------------------------------------
struct WLayer
{
  enum class LAYER_TYPE
  {
    LT_TEXT_AND_LINES = 0,
    LT_AREAS,
    LT_OTHER
  };

  LAYER_TYPE  m_type;
  cv::Mat     m_data;
  cv::Vec3b   m_color;
  std::string m_name;

};
// ------------------------------------------------------------
struct w_color
{
    w_color(uchar r, uchar g, uchar b);
    w_color(cv::Vec4b color);
    cv::Vec3b& toVec3b();
    friend inline bool operator <= (const w_color &first, const cv::Vec4b &second);
    friend inline bool operator >= (const w_color &first, const cv::Vec4b &second);

private:
    uchar r;
    uchar g;
    uchar b;
};
// ------------------------------------------------------------
typedef std::vector<WLayer> LayersContainer;
// ------------------------------------------------------------
class WRaster //: public IEnumItem<cv::Mat>
{
public:
  WRaster(std::string img_path = "");

  virtual ~WRaster(){}

  void IncreaseSharpness(double k);

  void AddLayer();

  int SetLayerMask(int layerNumber, const w_color &colorLow, const w_color &colorHigh);

  int WRaster::SetLayerColor(int layerNumber);

  int SetLayerColor(int layerNumber, w_color& rgbColor);

  int SetLayerType(int layerNumber, WLayer::LAYER_TYPE type);

  int SetLayerName(int layerNumber, std::string name);

  std::vector<cv::Rect> detectLetters(int layerNumber);

public:

  //bool NextLayer(cv::Mat* layer) const { return Next(layer); }
  //void ResetLayerEnum() const { Reset(); }

private:

  //virtual bool Next(cv::Mat* layer) const;

  // reset the enumerator
  //virtual void Reset() const;

  void Initialize(std::string img_path);

public:
  
  cv::Mat           m_raster;
  LayersContainer   m_layers;

private:

//LayersContainer::const_iterator   m_layer_cit;
  std::wstring                      m_image_path;
};
// ------------------------------------------------------------


class WPolyline
{
public:
	WPolyline();
	virtual ~WPolyline();

	WPolyline& operator=(WPolyline other);
	void clearPoints();

	// line width
	void setWidth(double width);
	double getWidth() { return m_width; };
	void setColor(WColor color) { m_color = color; };

	// points
	void AddPoint(const CvPoint& point) { m_points.push_back(point); };
	bool AddPointAt(const CvPoint& point, size_t idx);
	CvPoint getPoint(size_t idx) { return m_points.size() > idx ? m_points[idx] : CvPoint::CvPoint(); };
	bool RemovePoint(size_t idx);
	size_t Lenght() { return m_points.size(); };
	WCVPointsContainer & getPoints() { return m_points; }
	void concat(WPolyline& line);
	void concatTornLine(WPolyline& line, bool firstOrder, bool secondOrder);


	WCVPointsContainer simplifyLine(WCVPointsContainer &vectorline, double EPSILON, int delta);

private:
	double            m_width;
	WCVPointsContainer  m_points;
	
	int m_scaler;
	WColor m_color;
};


SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
