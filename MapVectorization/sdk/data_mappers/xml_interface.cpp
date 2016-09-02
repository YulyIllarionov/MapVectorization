#include "stdafx.h"
#include "xml_interface.h"


SDK_BEGIN_NAMESPACE

int SaveProject(std::string &filename, std::shared_ptr<WRaster> item)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument;
	tinyxml2::XMLDeclaration* declaration = doc->NewDeclaration();
	//doc->InsertFirstChild(declaration);
	WRasterDataMapper::Write(item, doc);
	int result = doc->SaveFile(filename.c_str());
	delete doc;
	return result;
}

std::shared_ptr<WRaster> LoadProject(std::string &filename)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument;
	tinyxml2::XMLError load_result = doc->LoadFile(filename.c_str());
	auto result = WRasterDataMapper::Read(doc);
	delete doc;
	return result;
}


int SaveSVG(std::string& filename, std::shared_ptr<WRaster> item)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument;
	tinyxml2::XMLDeclaration* declaration = doc->NewDeclaration();
	//doc->InsertFirstChild(declaration);
	WRasterDataMapper::WriteSVG(item, doc);
	int result = doc->SaveFile(filename.c_str());
	delete doc;
	return result;
}

SDK_END_NAMESPACE