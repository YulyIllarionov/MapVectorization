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
using namespace cv;
//
typedef std::vector<Point>                WPointsContainer;
typedef unsigned char                      WColor; 

struct SMapPoint
{
private:
	int x;
	int y;
public:
	int GetX() {
		return x;
	}
	int GetY() {
		return y;
	}
};

SDK_BEGIN_NAMESPACE


//Общий интерфейс всех объектов
class WVectorObject
{
public:
	virtual void clearPoints();
	virtual void setColor(WColor color);
	virtual void Add(const Point& point);
	virtual Point getPoint(size_t idx);
	virtual bool RemovePoint(size_t idx);
private:
};

//Объект полигон
class WPolygon : public WVectorObject
{
public:
	WPolygon();
    WPolygon(std::vector<SMapPoint> & mapPoints);
	~WPolygon();

	WPolygon& operator=(WPolygon& other);
	//Добавить точку
	void Add(const Point& point) { m_points.push_back(point); };
	//Добавить точку в по указанному индексу
	bool AddPointAt(const Point& point, size_t idx);
	//Взять точку по индексу
	Point GetPoint(size_t idx) { return m_points.size() > idx ? m_points[idx] : Point::Point_(); };
	//Удалить точку
	bool RemovePoint(size_t idx);
	size_t Lenght() { return m_points.size(); };
	WPointsContainer & GetPoints() { return m_points; }
    //Проверка точки на принадлежность
    inline bool Contains(const Point& point);

private:
	WPointsContainer  m_points;
	int m_scaler;
};

class WLine : public WVectorObject
{
public:
	WLine();

	WLine(double &width, WPointsContainer &points)
	{
		m_width = width;
		m_points = points;
	};
	~WLine();

	WLine& operator=(WLine& other);
	void clearPoints();

	// line width
	void setWidth(double width);
	double getWidth() { return m_width; };
	void setColor(WColor color) { m_color = color; };

	// points
	void Add(const Point& point) { m_points.push_back(point); };
	bool AddPointAt(const Point& point, size_t idx);
	Point getPoint(size_t idx) { return m_points.size() > idx ? m_points[idx] : Point::Point_(); };
	bool RemovePoint(size_t idx);
	size_t Lenght() { return m_points.size(); };
	WPointsContainer & getPoints() { return m_points; }
	void concat(WLine& line);
	void concatTornLine(WLine& line, bool firstOrder, bool secondOrder);

    //находится внутри полигона 
    bool BelongsTo(WPolygon polygon);

	//Упростить линию
	WPointsContainer simplifyLine(WPointsContainer &vectorline, double EPSILON, int delta);

private:
	double            m_width;
	WPointsContainer  m_points;
	int m_scaler;
	WColor m_color;
};


//Объект текст
class WText : public WVectorObject
{
public:
	WText();

	WText(WPolygon &polygon)
	{
		m_polygon = polygon;
	}

	WText(std::string &text, WPolygon &polygon, WLine &textline)
	{
		m_polygon = polygon;
		m_text = text;
		m_textline = textline;
		state = false;
	}
	~WText();

	WText& operator=(WText& other);
	//Добавить полигон
	void AddPolygon(WPolygon& polygon) {
		m_polygon = polygon;
	}
	WPolygon& getPolygon() {
		return m_polygon;
	}
	//Добавить текст
	void AddText(std::string &text) {
		m_text = text;
	}

	std::string GetText() {
		return m_text;
	}

	//Добавить линию текста
	void AddTextLine(WLine &textline)
	{
		m_textline = textline;
	}

	WLine& GetTextLine()
	{
		return m_textline;
	}

	//Изменить флаг
	void setStateTrue()
	{
		state = true;
	}
	void SetStateFalse()
	{
		state = false;
	}

	bool GetState()
	{
		return state;
	}
private:
	WPolygon m_polygon;//Границы текста на карте
	WLine m_textline; // Линия, обозначающая направление текста внутри полигона
	std::string m_text;//Запись
	bool state;//Флаг состояний: 0 - текст локализован, 1 - текст распознан
};

class WMapObject : public WVectorObject
{
public:
	WMapObject();
	~WMapObject();
	void Add(const Point& point) { m_points.push_back(point); };
	Point getPoint(size_t idx) { return m_points.size() > idx ? m_points[idx] : Point::Point_(); };
	bool RemovePoint(size_t idx);
private:
	WPointsContainer  m_points;

};

//Класс коллекции объектов
template<class T>
class WVector
{
public:
	//Конструктор
	WVector(void) {};
	~WVector(void) {};

	//Взять объекты
	virtual std::vector<T> GetObjectList() = 0;

	//Установить новый список объектов
	virtual void SetObjectList(std::vector<T>& listObjects) = 0;

	//Взять объет по идентификатору
	virtual T& GetObjectByID(int id) = 0;

	//Добавить объект в коллекцию
	virtual void Add(T &object) = 0;

	//Удалить объект из коллекции
	virtual void Remove(T &object) = 0;

	//Взять количество элементов в коллекции
	virtual int GetLength() = 0;

	//Клонировать коллекцию
	virtual WVector* Clone(WVector* newCollection) = 0;
};

//Класс коллекции объектов
template<class WText>
class WTextVector : public WVector<WText>
{
public:
	//Конструктор
	WTextVector(void) {};
	~WTextVector(void) {};

	WTextVector(WTextVector* textVector)
	{
		std::vector<WText> m_newlisttextObjects;
		for (int i = 0; i < textVector->GetLength(); i++)
		{
			if (textVector->GetObjectByID(i).GetState())
				m_newlisttextObjects.push_back(textVector->GetObjectByID(i));
		}
		this.m_listTextObjects = m_newlisttextObjects;
	}

	//Взять объекты
	std::vector<WText> GetTextObjectList() {
		return m_listTextObjects;
	}

	//Установить новый список объектов
	void SetObjectList(std::vector<WText>& listObjects) {
		m_listTextObjects = listObjects;
	}

	//Взять объет по идентификатору
	WText& GetObjectByID(int id) {
		return m_listTextObjects[id];
	}

	//Добавить объект в коллекцию
	void Add(WText &object) {
		return m_listTextObjects.push_back(object);
	}

	//Удалить объект из коллекции
	void Remove(WText &object) {
		m_listTextObjects.erase(std::remove(m_listTextObjects.begin(), m_listTextObjects.end(), object), m_listTextObjects.end());
	}

	//Взять длину коллекции
	int GetLength()
	{
		return m_listTextObjects.size();
	}

	//Клонировать коллекцию
	WTextVector& Clone(WTextVector* newCollection)
	{
		newCollection = new WTextVector(*this);
		return newCollection;
	}

private:
	std::vector<WText> m_listTextObjects;//Коллекция объектов
};

//Класс коллекции объектов
template<class WLine>
class WLineVector : public WVector<WLine>
{
public:
	//Конструктор
	WLineVector(void) {};
	~WLineVector(void) {};

	//Взять объекты
	std::vector<WLine> GetLineObjectList() {
		return m_listLineObjects;
	}

	//Установить новый список объектов
	void SetObjectList(std::vector<WLine>& listObjects) {
		m_listLineObjects = listObjects;
	}

	//Взять объет по идентификатору
	WLine& GetObjectByID(int id) {
		return m_listLineObjects[id];
	}

	//Добавить объект в коллекцию
	void Add(WLine &object) {
		return m_listLineObjects.push_back(object);
	}

	//Удалить объект из коллекции
	void Remove(WLine &object) {
		m_listLineObjects.erase(std::remove(m_listLineObjects.begin(), m_listLineObjects.end(), object), m_listLineObjects.end());
	}

	//Удалить объект по идентификатору
	void RemoveById(int id) {
		m_listLineObjects.erase(m_listLineObjects.begin() + id);
	}

private:
	std::vector<WLine> m_listLineObjects;//Коллекция объектов
};



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

  // define objects inside polygon
  std::vector<int> DefineObjectsInsidePolygon(WVector<WVectorObject> &vectorObjects, std::vector<SMapPoint> & mapPoints);
	
  // copy object from one layer to another
  void WRaster::CopyObjectsToAnotherLayer(const LayerUUID& departureLayerId, const LayerUUID& arrivalLayerId, WPolygon mapPoints);
  // удаление объектов со слоя 
  void WRaster::DeleteOblectsFromLayer(const LayerUUID& layerId, WPolygon mapPoints);
 
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

SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
