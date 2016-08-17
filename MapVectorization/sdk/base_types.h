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

//������������ ��� ������� � ����������� ����������� 
//TODO ���������� ������
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


//����� ��������� ���� ��������� ��������
class WVectorObject
{
public:
	virtual size_t Length() const { return m_points.size(); };
    //���������� �� ���������� ������� �� �����
	virtual double DistanceTo(const cv::Point& mapPoint) const;

	SMapPoint GetPoint(int i) { return SMapPoint(m_points.at(i).x, m_points.at(i).y); };

	//TODO ���������� � private, �� ����� ��� ������������
	WPointsContainer  m_points;
};

//������ �������
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

	//�������� ����� �� �������������� ��������
	bool Contains(const cv::Point& object) const;
    //�������� ������� ���������� ������� �� �������������� ��������
	bool Contains(const WVectorObject& object) const;
	//virtual double DistanceTo(cv::Point mapPoint) const;
};

class WLayer; //TODO ������������ � ������� Wtext � WLine ������
class Wregion;
// ������ ������ �����
class WLine : public WVectorObject
{
public:

	WLine(const WPointsContainer& points)
	{
		m_points = points;
	};
	~WLine() {};

	//WLine& operator=(WLine& other);

    //������������ ���� �����
	void Concat(const WLine& line);
	//virtual double DistanceTo(cv::Point mapPoint) const;
	//��������� ����� ��������� �������� �������-������
    void SimplifyDP(double epsilon = 2.7);
    //�������� ������ � ���������� ����
    std::vector<Wregion> CutFromLayer(WLayer* layer);

private:

};
//������ �����
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
	//�������� �����
	void AddText(const std::string &text) { m_text = text; }
	std::string GetText() { return m_text; }
	//�������� ����
	void SetState(bool state) { m_state = state; }
	bool GetState() const { return m_state; }

	//����������� �������� � ������� �� cv::Mat � ������� �� ��������������� ���������
    //���� ��������� ��� ������ �������������� ���� ��� �����
    //������������ ��� ������������� ������
    void RotateToHorizon(WLayer* layer, cv::Mat& image);
    //�������� � ���������� ����
    std::vector<Wregion> CutFromLayer(WLayer* layer);

private:
	std::string m_text;     //������������ �����
	bool        m_state;    //���� ���������: 0 - ����� �����������, 1 - ����� ���������
};

class WMapObject : public WVectorObject
{
public:
	WMapObject() {};
	~WMapObject() {};

private:
};
//��������� ��� ��������� �������� 
typedef std::vector<WVectorObject> WObjectContainer;

//TODO ������ ���� �� �����
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
//���������, �������� �������� �����
//������������ ��� ���������� ��������� ����������� �� ����
struct w_range
{
	w_range();
    //���������� ����� � ��������
	void addColor(const w_color& color);
    //�������� ����� �� �������������� ���������
	inline bool contains(const cv::Vec3b& color);
	w_color getLow();
	w_color getHigh();

private:
    //������� � ������ ������� ���������
	cv::Vec3b low;
	cv::Vec3b high;
};

// ------------------------------------------------------------
//TODO �������� ����� �����������
typedef std::string             LayerUUID;
typedef std::string             GroupID;
typedef std::vector<LayerUUID>  LayerIDs;
// ------------------------------------------------------------
//������ ����. �������� ��������� ����� ���� � ��������� ��� ������������ ��������� ��������
class WLayer
{

public:
    //��������� ���� ����, �� ������ ������ ������������ ���� LT_TEXT � LT_LINES
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
	//��������, �������� �� ��� ���� ������������
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

    //��������� ����� �� ��������� ����.
    //������������ � �������� �������
	void DrawCircle(SMapPoint point, uint radius, uchar color);
    //������������� ���������� ���������� ��� ����
    //�������� ����������� ��� ������������ � ����������� �� ���� ����
	void InicializeVectorContainer();
    //������������� ������ �������� ��������� ��������� �� ���������� ��������
    SDKResult RecognizeText(std::vector<int> idxs, const float minConfidences);
    
    //��������� �������� ����� ����
    //������ ������� ����� ������������ ����� ���������� ���� uchar
    //������� �������� ������� �������� ���������� ������� �� �����, ��������� - �����������
	cv::Mat     m_data;

    //��������� ��� ��������� �������� ��������� �� ���� 
	WObjectContainer  m_objects;

private:
    //ID ����
	LayerUUID   m_uuid;
    //��� ����
	LAYER_TYPE  m_type;
    //�������� ������ ������������� ���� 
	w_range     m_color_range;
    //��� ����
	std::string m_name;
    //��������� ID. ��� ���������� ������ ���� �� ��������� ���������� ���� �������� ���� ��������� ID
	GroupID     m_group_id;

    //��������� ������������ ��� ���� ����� 
	SDKResult InicializeLinesContainer();
    //��������� ������������ ��� ���� ������
    SDKResult InicializeTextContainer();
public:
};
// ------------------------------------------------------------
//��������� ��� �����
typedef std::list<WLayer>       LayersContainer;
// ------------------------------------------------------------
// ��������� �������� ���������� rgb �����, ������������� ��� ������� � ����������� �����������
struct w_color
{
	w_color(uchar r, uchar g, uchar b);
	w_color(const cv::Vec3b& color);
	w_color(const cv::Vec4b& color);
    //������� ����� � ������ ������������ � opencv
	cv::Vec3b toVec3b() const;
	friend inline bool operator <= (const w_color &first, const cv::Vec3b &second);
	friend inline bool operator >= (const w_color &first, const cv::Vec3b &second);
    //���������� ����� � ��������
	friend void w_range::addColor(const w_color& color);

	uchar r;
	uchar g;
	uchar b;
};
// ------------------------------------------------------------
//������, �������� ��������� ����������� � ��� ���� 
class WRaster
{
public:
    //�������� ���������� �����������
	WRaster(const std::string& imgPath = "");

	virtual ~WRaster(){}
    //���������� �������� 
	SDKResult IncreaseSharpness(double k) const;
    //����������� ����������� �� ������
	void SegmentationBilateral();
	void SegmentationMeanshift();
    //TODO ������ ���� �� ������������
	std::vector<cv::Rect> DetectLetters(const LayerUUID& layerId);

	//TODO ������ ��� ��� �� ������������
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

	//����������� ������� ������ �������� ������� ���� � ���������� �� ������� � ���������� ����
    //������������ � ����������� ����������
	std::vector<int> DefineObjectsInsidePolygon(const LayerUUID& layerId, const WPolygon& mapPoints);
    //����������� ��������� � ��������� ����� ������ ���� � ���������� ��� ������ � ���������� ����
    //������������ � ����������� ����������
	std::vector<int> DefineObjectsNearPoint(const LayerUUID& layerId, SMapPoint point);

	//��������� �������� �� ����
	std::vector<std::vector<Wregion>> CutObjectsFromLayer(const LayerUUID& layerId, std::vector<int> idxs);
    //������� �������� �� ���� 
    SDKResult PasteObjectsToLayer(const LayerUUID& layerId, std::vector<std::vector<Wregion>> rasterObjects);

private:
	SDKResult SetLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type, bool overwrite);
	void Initialize(const std::string& imgPath);
    //��������� ���� �� ���� ����� � ������
	SDKResult SplitLines(const LayerUUID& layerId, const LayerUUID& linesLayerID, const LayerUUID& othersLayerID);

	//��������� ����������� � �����������
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

    //��������� �� �����
	LayersContainer   m_layers;
public:
    //�������������� ��������� �����������
	cv::Mat           m_raster;

private:
    //���� � ����������� �����������
	std::wstring                      m_image_path;
};
// ------------------------------------------------------------
//������ �������������� ����� ������� ������� �� ����
class Wregion
{
public:
    //����� ������� ������� �� �����
	Wregion(const cv::Point& point, cv::Mat& img);
    //����� ������� ������� �� ����� � ��������� ������ ��������� ���������
    Wregion(const cv::Point& point, cv::Mat& img, WPolygon edges);
    //���������� ��������� �������������, ������������ ���������
	cv::Rect boundingRectangle();
    //������� ������� �������
	int Square();
    //��������, �������� �� ������� ������� ������ 
    //������������ ��� ���������� �� ���� ����� � ������
	bool IsLine();
    //��������� ������� �� �����������
	void drawOn(cv::Mat& img, uchar color);
    //�������� �� �������� �� ������� ������
    bool IsEmpty() { return (points.size() == 0); }
    std::vector<cv::Point> GetPoints() { return points; }
private:
    //������ �� ����� �������
	std::vector<cv::Point> points;
};

SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
