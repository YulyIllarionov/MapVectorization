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
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "app/sdk_result.h"
#include "opencv/cxcore.h"
#include <list>

//using namespace cv;
//
typedef std::vector<cv::Point>            WPointsContainer;
typedef unsigned char                     WColor; 

struct SMapPoint
{
private:
	int x;
	int y;
public:
    SMapPoint(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
	int GetX() {
		return x;
	}
	int GetY() {
		return y;
	}

    cv::Point ToPoint() {
        return cv::Point(x, y);
    }
};

SDK_BEGIN_NAMESPACE


//Общий интерфейс всех объектов
class WVectorObject
{
public:
	//virtual void Add(/*const*/ cv::Point& point) { m_points.push_back(point); };
  ////Добавить точку в по указанному индексу
	//virtual bool AddPointAt(/*const*/ cv::Point& point, size_t idx);
	////Удалить точку
	//virtual bool RemovePoint(size_t idx);
	////Взять точку по индексу
	//virtual cv::Point GetPoint(size_t idx) { return m_points.size() > idx ? m_points[idx] : cv::Point::Point_(); };
  // // get points
  //const WPointsContainer& GetPoints() const { return m_points; };
  // get length
	virtual size_t Length() const { return m_points.size(); };
  virtual double DistanceTo(cv::Point mapPoint) const;

//protected:
  WPointsContainer  m_points;
};

//Объект полигон
class WPolygon : public WVectorObject
{
public:
	WPolygon();
  WPolygon(std::vector<SMapPoint> & mapPoints);
  ~WPolygon() {};

	//WPolygon& operator=(WPolygon& other);
	
  //Проверка точки на принадлежность
  bool Contains(const WVectorObject& object) const;
  //virtual double DistanceTo(cv::Point mapPoint) const;

private:
	
	int m_scaler;
};

class WLine : public WVectorObject
{
public:
  WLine() : m_width(0) {};

	WLine(const WPointsContainer& points)
	{
		m_points = points;
	};
  ~WLine() {};

	//WLine& operator=(WLine& other);
	//void clearPoints();

	// line width
	void   SetWidth(double width);
	double GetWidth() { return m_width; };
	void   SetColor(WColor color) { m_color = color; };

	void Concat(const WLine& line);
  //virtual double DistanceTo(cv::Point mapPoint) const;
	//Упростить линию
	WPointsContainer SimplifyLine(const WPointsContainer& vectorline, double EPSILON, int delta);

private:
	double    m_width;
	int       m_scaler;
	WColor    m_color;
};


//Объект текст
class WText : public WPolygon
{
public:
	WText();

	WText(const WPolygon &polygon)
    : m_polygon(polygon)
	{
	}

	WText(const std::string &text, const WPolygon &polygon, const WLine &textline)
    : m_polygon(polygon), m_text(text), m_textline(textline), m_state(false)
	{
	}
  ~WText() {};

	//WText& operator=(WText& other);
  bool operator==(const WText& other) {
    return other.m_text == m_text;
	}
	//Добавить полигон
	void AddPolygon(WPolygon& polygon) { m_polygon = polygon; }
	WPolygon& GetPolygon() { return m_polygon; }
	//Добавить текст
	void AddText(const std::string &text) { m_text = text; }
	std::string GetText() { return m_text; }
	//Добавить линию текста
	void   AddTextLine(WLine &textline) { m_textline = textline; }
	WLine& GetTextLine() { return m_textline; }
	//Изменить флаг
	void SetState(bool state) { m_state = state; }
	bool GetState() const { return m_state; }

  //virtual double DistanceTo(cv::Point mapPoint) const;

private:
	WPolygon    m_polygon;  //Границы текста на карте
	WLine       m_textline; // Линия, обозначающая направление текста внутри полигона
	std::string m_text;     //Запись
	bool        m_state;    //Флаг состояний: 0 - текст локализован, 1 - текст распознан
};

class WMapObject : public WVectorObject
{
public:
  WMapObject() {};
  ~WMapObject() {};
  
  //virtual double DistanceTo(cv::Point mapPoint) const;
private:
};

typedef std::vector<WVectorObject> WObjectContainer;

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
class WLayer
{

public:
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

private:
  // check if type is single
  static bool IsSingleType(LAYER_TYPE type)
  {
    int typesCount = 0;
    if ((type & LT_TEXT) != 0) 
      ++typesCount;
    if ((type & LT_LINES) != 0) 
      ++typesCount;
    if ((type & LT_AREAS) != 0) 
      ++typesCount;
    if ((type & LT_SYMBOLS) != 0) 
      ++typesCount;
    if ((type & LT_OTHER) != 0) 
      ++typesCount;
    if ((type & LT_TEXT) != 0) 
      ++typesCount;

    return typesCount == 1;
  }

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
  bool        IsSingleType() const { return IsSingleType(m_type); }
  w_range     getRange()     const { return m_color_range; }
  std::string getName()      const { return m_name; }
  GroupID     getGroupId()   const { return m_group_id; }

  void DrawCircle(SMapPoint point, uint radius, uchar color);
  void InicializeVectorContainer();

  cv::Mat     m_data;
    
  WObjectContainer  m_objects;

private:
  LayerUUID   m_uuid;
  LAYER_TYPE  m_type;
  w_range     m_color_range;
  std::string m_name;
  GroupID     m_group_id;

  SDKResult InicializeLinesContainer();
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
  size_t    GetLayersCount  () const { return m_layers.size(); }
  // get related layers
  SDKResult GetLayersByGroupId(const GroupID& groupId, LayerIDs& relatedLayers) const;

  // define objects inside polygon
  std::vector<int> DefineObjectsForPolygon(const LayerUUID& layerId, WPolygon mapPoints);
	
  // copy object from one layer to another
  void CopyObjectsToAnotherLayer(const LayerUUID& departureLayerId, const LayerUUID& arrivalLayerId, WPolygon mapPoints);
  // удаление объектов со слоя 
  void DeleteOblectsFromLayer(const LayerUUID& layerId, WPolygon mapPoints);
 
private:
  SDKResult SetLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type, bool overwrite) const;
  void Initialize(const std::string& imgPath);
  SDKResult SplitLines(const LayerUUID& layerId, const LayerUUID& linesLayerID, const LayerUUID& othersLayerID);
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
class Wregion
{
public:
    Wregion(cv::Point point, cv::Mat& img);
    cv::Rect boundingRectangle();
    int Square();
    bool IsLine();
    void drawOn(cv::Mat& img, uchar color);
private:
    std::vector<cv::Point> points;
};

SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
