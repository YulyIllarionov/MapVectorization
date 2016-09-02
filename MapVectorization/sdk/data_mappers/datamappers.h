#pragma once
#include <tinyxml2.h>
#include <memory>
#include "../base_types.h"

SDK_BEGIN_NAMESPACE

class WRasterDataMapper
{
public:
	static std::shared_ptr<WRaster> Read(tinyxml2::XMLDocument* doc);
	static void Write(std::shared_ptr<WRaster> item, tinyxml2::XMLDocument* doc);
	static void WriteSVG(std::shared_ptr<WRaster> item, tinyxml2::XMLDocument* doc);
};

class WLayerDataMapper
{
public:
	static WLayer Read(tinyxml2::XMLElement* node);
	static void Write(WLayer* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
	static void WriteSVG(WLayer* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class WRangeDataMapper
{
public:
	static w_range Read(tinyxml2::XMLElement* node);
	static void Write(w_range* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class CVPointDataMapper
{
public:
	static cv::Point Read(tinyxml2::XMLElement* node);
	static void Write(cv::Point* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class WLineDataMapper
{
public:
	static std::shared_ptr<WLine> Read(tinyxml2::XMLElement* node);
	static void Write(std::shared_ptr<WLine> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
	static void WriteSVG(std::shared_ptr<WLine> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class WTextDataMapper
{
public:
	static std::shared_ptr<WText> Read(tinyxml2::XMLElement* node);
	static void Write(std::shared_ptr<WText> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
	static void WriteSVG(std::shared_ptr<WText> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class WMaskDataMapper
{
public:
	static cv::Mat Read(tinyxml2::XMLElement* node);
	static void Write(cv::Mat& item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

SDK_END_NAMESPACE