#include "stdafx.h"
#include "datamappers.h"


SDK_BEGIN_NAMESPACE

static std::shared_ptr<WRaster> Read(size_t id)
{
}

static void Write(WRaster& item, tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLElement* result = doc->NewElement("WRaster");
	tinyxml2::XMLAttribute* filename;
	filename->SetAttribute(item.GetImgPath().c_str());
}

SDK_END_NAMESPACE