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
	tinyxml2::XMLNode* raster_node = doc->NewElement("WRaster");	
	doc->InsertFirstChild(raster_node);
	tinyxml2::XMLElement* raster_filepath = doc->NewElement("filepath");
	raster_filepath->SetText(item->GetImgPath().c_str());
	raster_node->InsertEndChild(raster_filepath);

	tinyxml2::XMLElement* layer_container_node = doc->NewElement("layer_container");
	raster_node->InsertEndChild(layer_container_node);
	uint size = item->GetLayersCount();
	layer_container_node->SetAttribute("size", size); 
	for (uint i = 0; i < size; i++)
	{
		WLayerDataMapper::Write(item->GetLayerByContainerPosition(i), doc, layer_container_node);
	}
}

std::shared_ptr<WLayer> WLayerDataMapper::Read(size_t)
{
	std::shared_ptr<WLayer> a;
	return a;
}

void WLayerDataMapper::Write(WLayer* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* layer_node = doc->NewElement("layer");
	layer_node->SetAttribute("Name", item->getName().c_str());
	layer_node->SetAttribute("LayerUUID", item->getID().c_str());
	layer_node->SetAttribute("LAYER_TYPE", item->getType());
	node->InsertEndChild(layer_node);
}

SDK_END_NAMESPACE