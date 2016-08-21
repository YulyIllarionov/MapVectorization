#include "stdafx.h"
#include "datamappers.h"


SDK_BEGIN_NAMESPACE

std::shared_ptr<WRaster> WRasterDataMapper::Read(size_t id)
{
	std::shared_ptr<WRaster> a;
	return a;
}

void WRasterDataMapper::Write(WRaster* item, tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLElement* result = doc->NewElement("WRaster");
	result->SetAttribute("filepath", item->GetImgPath().c_str());
}

SDK_END_NAMESPACE