/// Whiteteam
/// \file base_types.h
/// \brief This file contains base types
/// \author Whiteteam authors

#ifndef _SDK_BASE_TYPES_H_
#define _SDK_BASE_TYPES_H_
#pragma once

#include <vector>
#include <string>

#include "app/sdk_const.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cxcore.h"

//
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

//Общий интерфейс обращения к объектам
class WVectorObject
{
public:
	virtual void clearPoints() const = 0;
	virtual void setColor(WColor color) const = 0;
	virtual void AddPoint(const CvPoint& point) const = 0;
	virtual CvPoint getPoint(size_t idx) const = 0;
	virtual bool RemovePoint(size_t idx) const = 0;
private:
};


class WText : public WVectorObject
{
public:
	WText(CvPoint &point1, CvPoint &point2, std::string &text)
	{
		m_point_left_down = point1;
		m_point_right_up = point2;
		m_text = text;
	}
	~WText();
	void AddPoints(const CvPoint& point1, const CvPoint& point2) {
		m_point_left_down = point1;
		m_point_right_up = point2;
	}
	CvPoint getPointLeft() {
		return m_point_left_down;
	}

	CvPoint getPointRight() {
		return m_point_right_up;
	}

	void AddText(std::string &text) {
		m_text = text;
	}

	std::string GetText() {
		return m_text;
	}
	//Конкатенация текста
	void Concat(WText &text, bool leftOrRight);
private:
	CvPoint m_point_left_down;//Левая нижняя точка текста
	CvPoint m_point_right_up;//Правая верхняя точка текста
	std::string m_text;//Запись

};

class WPolyline : public WVectorObject
{
public:
	WPolyline();
	~WPolyline();

	WPolyline& operator=(WPolyline& other);
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


	void AddText(WText& text) {
		m_linetext = text;
	}
	WText& GetText() {
		return m_linetext;
	}
	//Упростить линию
	WCVPointsContainer simplifyLine(WCVPointsContainer &vectorline, double EPSILON, int delta);

private:
	double            m_width;
	WCVPointsContainer  m_points;
	WText&				m_linetext;
	
	int m_scaler;
	WColor m_color;
};



//Класс для хранения всех объектов
class WVector
{
public:
	WVector(void) {};
	~WVector(void) {};

	std::vector<WText> GetTextList() {
		return m_listTexts;
	}

	void SetTextList(std::vector<WText> & textlist) {
		m_listTexts = textlist;
	}

	WText& GetTextById(int id) {
		return m_listTexts[id];
	}

	void AddText(WText &text) {
		return m_listTexts.push_back(text);
	}

	void RemoveText(WText &text) {
		m_listTexts.erase(std::remove(m_listTexts.begin(), m_listTexts.end(), text),m_listTexts.end());
	}

	std::vector<WPolyline> GetPolylineList() {
		return m_listPolylines;
	}

	void SetPolylineList(std::vector<WPolyline> listpolylines) {
		m_listPolylines = listpolylines;
	}

	WPolyline& GetPolylineById(int id) {
		return m_listPolylines[id];
	}

	void AddPolyline(WPolyline &polyline) {
		m_listPolylines.push_back(polyline);
	}

	void RemovePolyline(WPolyline *polyline) {
		m_listPolylines.erase(std::remove(m_listPolylines.begin(), m_listPolylines.end(), polyline),m_listPolylines.end());
	}

private:
	std::vector<WText> m_listTexts;//Коллекция текстов
	std::vector<WPolyline> m_listPolylines;//Коллекция линий
};

SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
