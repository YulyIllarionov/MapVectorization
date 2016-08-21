#include "stdafx.h"
#include "xml_interface.h"


SDK_BEGIN_NAMESPACE

void SaveProject(std::string &filename, WRaster& item)
{
	tinyxml2::XMLDocument doc;
	doc.NewDeclaration();
	WRasterDataMapper::Write(item, &doc);
	doc.SaveFile(filename.c_str());
}

SDK_END_NAMESPACE