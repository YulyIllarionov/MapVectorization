#pragma once
#include <tinyxml2.h>
#include <memory>
#include "../base_types.h"

SDK_BEGIN_NAMESPACE

class WRasterDataMapper
{
public:
	//static std::shared_ptr<WRaster> Read(size_t);
	static void Write(WRaster* item, tinyxml2::XMLDocument* doc);
};

class WLayerDataMapper
{
public:
	//static std::shared_ptr<WLayer> Read(size_t);
	static void Write(WLayer* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class WRangeDataMapper
{
public:
	//static std::shared_ptr<w_range> Read(size_t);
	static void Write(w_range* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class CVPointDataMapper
{
public:
	//static std::shared_ptr<WLine> Read(size_t);
	static void Write(cv::Point* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class WLineDataMapper
{
public:
	//static std::shared_ptr<WLine> Read(size_t);
	static void Write(WLine* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};

class WTextDataMapper
{
public:
	//static std::shared_ptr<WLine> Read(size_t);
	static void Write(WText* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node);
};


SDK_END_NAMESPACE