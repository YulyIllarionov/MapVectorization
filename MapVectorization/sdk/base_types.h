/// Whiteteam
/// \file base_types.h
/// \brief This file contains base types
/// \author Whiteteam authors

#ifndef _SDK_BASE_TYPES_H_
#define _SDK_BASE_TYPES_H_
#pragma once

#include <vector>


#include "app/sdk_const.h"
#include "app/sdk_result.h"

#include "opencv/cxcore.h"
#include <list>

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
struct w_color;
// ------------------------------------------------------------
struct w_range
{
  w_range();
  void addColor(const w_color& color);
  inline bool contains(const cv::Vec3b& color);
  w_color getLow();
  w_color getHigh();

private:
  cv::Vec3b low;
  cv::Vec3b high;
};

// ------------------------------------------------------------
typedef std::string             LayerUUID;
typedef std::string             GroupID;
typedef std::vector<LayerUUID>  LayerIDs;
// ------------------------------------------------------------
struct WLayer
{
  typedef uint LAYER_TYPE;
  enum/* class*/ LAYER_TYPE_ENUM : uint
  {
    LT_NONE    = 0x0000,
    LT_TEXT    = 0x0001,
    LT_LINES   = 0x0002,
    LT_AREAS   = 0x0004,
    LT_SYMBOLS = 0x0008,
    LT_OTHER   = 0x0010, 
    LT_ALL     = 0xFFFF, 
  };
  friend class WRaster;

public:

  friend bool operator==(const WLayer& lhs, const WLayer& rhs)
  {
    return lhs.m_uuid == rhs.m_uuid;
  }

  friend bool operator!=(const WLayer& lhs, const WLayer& rhs)
  {
    return !(lhs == rhs);
  }

  LayerUUID   getID()        const { return m_uuid; }
  LAYER_TYPE  getType()      const { return m_type; }
  w_range     getRange()     const { return m_color_range; }
  std::string getName()      const { return m_name; }
  GroupID     getGroupId()   const { return m_group_id; }

  cv::Mat     m_data;

private:
  LayerUUID   m_uuid;
  LAYER_TYPE  m_type;
  w_range     m_color_range;
  std::string m_name;
  GroupID     m_group_id;
};
// ------------------------------------------------------------
typedef std::list<WLayer>       LayersContainer;
// ------------------------------------------------------------
struct w_color
{
  //friend w_range;
  w_color(uchar r, uchar g, uchar b);
  w_color(cv::Vec3b color);
  w_color(cv::Vec4b color);

  cv::Vec3b toVec3b() const;
  friend inline bool operator <= (const w_color &first, const cv::Vec3b &second);
  friend inline bool operator >= (const w_color &first, const cv::Vec3b &second);
  friend void w_range::addColor(const w_color& color);

  uchar r;
  uchar g;
  uchar b;
};
// ------------------------------------------------------------
class WRaster //: public IEnumItem<cv::Mat>
{
public:
  WRaster(const std::string& imgPath = "");

  virtual ~WRaster(){}

  SDKResult IncreaseSharpness(double k) const;
  void SegmentationBilateral();
  void SegmentationMeanshift();
  std::vector<cv::Rect> DetectLetters(const LayerUUID& layerId) const;

  //{ layer
  // create and add new layer
  WLayer*   AddLayer(const GroupID& groupId = "");      
  // create and add new layer
  SDKResult RemoveLayer     (const LayerUUID& layerId);
  // add layer color
  SDKResult AddColorToLayer (const LayerUUID& layerId, const w_color& color) const;
  // add, set, remove layer type
  SDKResult AddLayerType    (const LayerUUID& layerId, WLayer::LAYER_TYPE type) const;
  SDKResult SetLayerType    (const LayerUUID& layerId, WLayer::LAYER_TYPE type) const;
  SDKResult RemoveLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type) const;
  // set layer name
  SDKResult SetLayerName    (const LayerUUID& layerId, const std::string& name) const;
  // return layer's ref
  WLayer*   GetLayerById    (const LayerUUID& layerId) const;
  WLayer*   GetLayerByName  (const std::string& name) const;
  // get layers idxs by type
  SDKResult GetLayersByType (WLayer::LAYER_TYPE type, LayerIDs& layerIds) const;
  // split layer by function
  SDKResult SplitLayer      (const LayerUUID& layerId, LayerIDs& splittedLayers);
  // get layers count
  size_t    GetLayersCount  () { return m_layers.size(); }
  // get related layers
SDKResult WRaster::GetLayersByGroupId(const GroupID& groupId, LayerIDs& relatedLayers) const;
  //}
 
public:


private:
  SDKResult SetLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type, bool overwrite) const;
  void Initialize(const std::string& imgPath);

  // depricate copy and move operations
  WRaster(const WRaster& other)
    /*: m_raster{other.m_raster},
      m_layers{other.m_layers},
      m_image_path{other.m_image_path}*/
  {
  }

  WRaster(WRaster&& other)
    /*: m_raster{std::move(other.m_raster)},
      m_layers{std::move(other.m_layers)},
      m_image_path{std::move(other.m_image_path)}*/
  {
  }

  //WRaster& operator=(WRaster other)
  //{
  //  using std::swap;
  //  swap(*this, other);
  //  return *this;
  //}

  
  LayersContainer   m_layers;
public:
  cv::Mat           m_raster;

private:

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
