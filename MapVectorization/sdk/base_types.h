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

//Используется для общения с графическим интерфейсом 
//TODO желательно убрать
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
	SMapPoint(cv::Point point)
	{
		this->x = point.x;
		this->y = point.y;
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


//Общий интерфейс всех векторных объектов
class WVectorObject
{
public:
	virtual size_t Length() const { return m_points.size(); };
    //расстояние от векторного объекта до точки
	virtual double DistanceTo(const cv::Point& mapPoint) const;

	SMapPoint GetPoint(int i) { return SMapPoint(m_points.at(i).x, m_points.at(i).y); };

	//TODO переделать в private, но много где используется
	WPointsContainer  m_points;
};

//Объект полигон
class WPolygon : public WVectorObject
{
public:
	WPolygon();
    WPolygon(cv::Rect rect);
    WPolygon(cv::RotatedRect rect, cv::Size borders);
	WPolygon(std::vector<SMapPoint> & mapPoints);
	WPolygon(const std::vector<cv::Point> & mapPoints);
	~WPolygon() {};

	WPolygon& operator=(WPolygon& other);

	//Проверка точки на принадлежность полигону
	bool Contains(const cv::Point& object) const;
    //Проверка другого векторного объекта на принадлежность полигону
	bool Contains(const WVectorObject& object) const;
	//virtual double DistanceTo(cv::Point mapPoint) const;
};

class WLayer; //TODO используется в классах Wtext и WLine убрать
class Wregion;
// Объект кривая линия
class WLine : public WVectorObject
{
public:

	WLine(const WPointsContainer& points)
	{
		m_points = points;
	};
	~WLine() {};

	//WLine& operator=(WLine& other);

    //Конкатенация двух линий
	void Concat(const WLine& line);
	//virtual double DistanceTo(cv::Point mapPoint) const;
	//Упростить линию изпользуя алгоритм Дугласа-Пекера
    void SimplifyDP(double epsilon = 2.7);
    //Вырезать объект с растрового слоя
    std::vector<Wregion> CutFromLayer(WLayer* layer);

private:

};
//Объект текст
class WText : public WPolygon
{
public:

	WText(const std::vector<cv::Point> & mapPoints) : 
		WPolygon(mapPoints)
	{
	}

	~WText() {};

	bool operator==(const WText& other) {
		return other.m_text == m_text;
	}
	//Добавить текст
	void AddText(const std::string &text) { m_text = text; }
	std::string GetText() { return m_text; }
	//Изменить флаг
	void SetState(bool state) { m_state = state; }
	bool GetState() const { return m_state; }

	//Копирование полигона с текстом на cv::Mat и поворот до горизонтального положения
    //Угол находится при помощи преобразования Хафа для линий
    //Используется при распознавании текста
    void RotateToHorizon(WLayer* layer, cv::Mat& image);
    //Вырезать с растрового слоя
    std::vector<Wregion> CutFromLayer(WLayer* layer);

private:
	std::string m_text;     //Содержащийся текст
	bool        m_state;    //Флаг состояний: 0 - текст локализован, 1 - текст распознан
};

class WMapObject : public WVectorObject
{
public:
	WMapObject() {};
	~WMapObject() {};

private:
};
//Контейнер для векторных объектов 
typedef std::vector<WVectorObject> WObjectContainer;

//TODO Убрать если не нужен
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
//Структура, хранящая диапазон слоев
//Используется при разделении исходного изображения на слои
struct w_range
{
	w_range();
    //Добавление цвета в диапазон
	void addColor(const w_color& color);
    //Проверка цвета на принадлежность диапазону
	inline bool contains(const cv::Vec3b& color);
	w_color getLow();
	w_color getHigh();

private:
    //Верхняя и нижняя границы диапазона
	cv::Vec3b low;
	cv::Vec3b high;
};

// ------------------------------------------------------------
//TODO оставить умный комментарий
typedef std::string             LayerUUID;
typedef std::string             GroupID;
typedef std::vector<LayerUUID>  LayerIDs;
// ------------------------------------------------------------
//Объект слой. Содержит растровую маску слоя и контейнер для распознанных векторных объектов
class WLayer
{

public:
    //Возможные типы слоя, на данный момент используются типы LT_TEXT и LT_LINES
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
	//Проверка, является ли тип слоя единственным
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

    //Рисование круга на растровом слое.
    //Используется в качестве ластика
	void DrawCircle(SMapPoint point, uint radius, uchar color);
    //Инициализация векторного контейнера для слоя
    //Вызывает необходимый вид векторизации в зависимости от типа слоя
	void InicializeVectorContainer();
    //Распознавание текста объектов векторной коллекции по задаваемым индексам
    SDKResult RecognizeText(std::vector<int> idxs, const float minConfidences);
    
    //Растровая бинарная маска слоя
    //Каждый пиксель маски представляет собой переменную типа uchar
    //Нулевое значение пикселя означает отсутствие пикселя на маске, ненулевое - присутствие
	cv::Mat     m_data;

    //Контейнер для векторных объектов найденных на слое 
	WObjectContainer  m_objects;

private:
    //ID слоя
	LayerUUID   m_uuid;
    //Тип слоя
	LAYER_TYPE  m_type;
    //Диапазон цветов принадлежащих слою 
	w_range     m_color_range;
    //Имя слоя
	std::string m_name;
    //Групповое ID. При разделении одного слоя на несколько полученные слои получают одно групповое ID
	GroupID     m_group_id;

    //Выполняет векторизацию для слоя линий 
	SDKResult InicializeLinesContainer();
    //Выполняет векторизацию для слоя текста
    SDKResult InicializeTextContainer();
public:
};
// ------------------------------------------------------------
//Контейнер для слоев
typedef std::list<WLayer>       LayersContainer;
// ------------------------------------------------------------
// Структура хрянящая компоненты rgb цвета, изспользуется для общения с графическим интерфейсом
struct w_color
{
	w_color(uchar r, uchar g, uchar b);
	w_color(const cv::Vec3b& color);
	w_color(const cv::Vec4b& color);
    //Перевод цвета в формат используемый в opencv
	cv::Vec3b toVec3b() const;
	friend inline bool operator <= (const w_color &first, const cv::Vec3b &second);
	friend inline bool operator >= (const w_color &first, const cv::Vec3b &second);
    //Добавление цвета в диапазон
	friend void w_range::addColor(const w_color& color);

	uchar r;
	uchar g;
	uchar b;
};
// ------------------------------------------------------------
//Объект, хранящий растровое изображение и его слои 
class WRaster
{
public:
    //Загрузка растрового изображения
	WRaster(const std::string& imgPath = "");

	virtual ~WRaster(){}
    //Увеличение резкости 
	SDKResult IncreaseSharpness(double k) const;
    //Сегментация изображения по цветам
	void SegmentationBilateral();
	void SegmentationMeanshift();
    //TODO убрать если не используется
	std::vector<cv::Rect> DetectLetters(const LayerUUID& layerId);

	//TODO убрать все что не используется
	WLayer*   AddLayer(const GroupID& groupId = "");      
	// create and add new layer
	SDKResult RemoveLayer     (const LayerUUID& layerId);
	// add layer color
	SDKResult AddColorToLayer (const LayerUUID& layerId, const w_color& color);
	// add, set, remove layer type
	SDKResult AddLayerType    (const LayerUUID& layerId, WLayer::LAYER_TYPE type);
	SDKResult SetLayerType    (const LayerUUID& layerId, WLayer::LAYER_TYPE type);
	SDKResult RemoveLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type);
	// set layer name
	SDKResult SetLayerName    (const LayerUUID& layerId, const std::string& name);
	// return layer's ref
	WLayer*   GetLayerById    (const LayerUUID& layerId);
	WLayer*   GetLayerByName  (const std::string& name);
	// get layers idxs by type
	SDKResult GetLayersByType (WLayer::LAYER_TYPE type, LayerIDs& layerIds) const;
	// split layer by function
	SDKResult SplitLayer      (const LayerUUID& layerId, LayerIDs& splittedLayers);
	// get layers count
	size_t    GetLayersCount  () const { return m_layers.size(); }
	// get related layers
	SDKResult GetLayersByGroupId(const GroupID& groupId, LayerIDs& relatedLayers) const;

	//Опеределяет лежащие внутри полигона объекты слоя и возвращает их индексы в контейнере слоя
    //Используется в графическом интерфейсе
	std::vector<int> DefineObjectsInsidePolygon(const LayerUUID& layerId, const WPolygon& mapPoints);
    //Опеределяет ближайший к указанной точке объект слоя и возвращает его индекс в контейнере слоя
    //Используется в графическом интерфейсе
	std::vector<int> DefineObjectsNearPoint(const LayerUUID& layerId, SMapPoint point);

	//Вырезание объектов со слоя
	std::vector<std::vector<Wregion>> CutObjectsFromLayer(const LayerUUID& layerId, std::vector<int> idxs);
    //Вставка объектов на слой 
    SDKResult PasteObjectsToLayer(const LayerUUID& layerId, std::vector<std::vector<Wregion>> rasterObjects);

private:
	SDKResult SetLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type, bool overwrite);
	void Initialize(const std::string& imgPath);
    //Разделяет слой на слои линий и текста
	SDKResult SplitLines(const LayerUUID& layerId, const LayerUUID& linesLayerID, const LayerUUID& othersLayerID);

	//Запрещает копирование и перемещение
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

    //Контейнер из слоев
	LayersContainer   m_layers;
public:
    //Обрабатываемое растровое изображение
	cv::Mat           m_raster;

private:
    //Путь к растрововму изображение
	std::wstring                      m_image_path;
};
// ------------------------------------------------------------
//Объект представляющий собой связную область на слое
class Wregion
{
public:
    //Поиск связной области по точке
	Wregion(const cv::Point& point, cv::Mat& img);
    //Поиск связной области по точке с границами поиска заданными полигоном
    Wregion(const cv::Point& point, cv::Mat& img, WPolygon edges);
    //Возвращает описанный прямоугольник, параллельный горизонту
	cv::Rect boundingRectangle();
    //Площадь связной области
	int Square();
    //Проверка, является ли связная область линией 
    //Используется при разделении на слои линий и текста
	bool IsLine();
    //Рисование области на изображении
	void drawOn(cv::Mat& img, uchar color);
    //Проверка не является ли область пустой
    bool IsEmpty() { return (points.size() == 0); }
    std::vector<cv::Point> GetPoints() { return points; }
private:
    //Вектор из точек области
	std::vector<cv::Point> points;
};

SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
