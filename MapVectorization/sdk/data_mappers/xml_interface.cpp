#include "stdafx.h"
#include "xml_interface.h"


SDK_BEGIN_NAMESPACE

int SaveProject(std::string &filename, WRaster* item)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument;
	doc->NewDeclaration();
	WRasterDataMapper::Write(item, doc);
	int ret = doc->SaveFile(filename.c_str());
	//delete doc;
	return ret;
}

SDK_END_NAMESPACE