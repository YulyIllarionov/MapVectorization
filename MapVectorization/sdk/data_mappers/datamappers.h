#pragma once
#include <tinyxml2.h>
#include <memory>
#include "../base_types.h"

SDK_BEGIN_NAMESPACE

class WRasterDataMapper
{
public:
	static std::shared_ptr<WRaster> Read(size_t);
	static void Write(WRaster* item, tinyxml2::XMLDocument* doc);
};

SDK_END_NAMESPACE