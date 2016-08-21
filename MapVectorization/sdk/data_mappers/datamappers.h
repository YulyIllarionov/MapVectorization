#pragma once
#include <tinyxml2.h>
#include <memory>
#include "../base_types.h"

SDK_BEGIN_NAMESPACE

class WRasterDataMapper
{
public:
	static std::shared_ptr<WRaster> Read(size_t);
	static void Write(WRaster&, tinyxml2::XMLDocument*);
};

SDK_END_NAMESPACE