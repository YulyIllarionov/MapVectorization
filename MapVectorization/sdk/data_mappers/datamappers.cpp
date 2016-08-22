#include "stdafx.h"
#include "datamappers.h"


SDK_BEGIN_NAMESPACE

//std::shared_ptr<WRaster> WRasterDataMapper::Read(size_t id)
//{
//	std::shared_ptr<WRaster> a;
//	return a;
//}

void WRasterDataMapper::Write(std::shared_ptr<WRaster> item, tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLNode* raster_node = doc->NewElement("raster");	
	doc->InsertFirstChild(raster_node);
	tinyxml2::XMLElement* raster_filepath = doc->NewElement("filepath");
	raster_filepath->SetText(item->GetImgPath().c_str());
	raster_node->InsertEndChild(raster_filepath);

	tinyxml2::XMLElement* layer_container_node = doc->NewElement("layer_container");
	uint size = item->GetLayersCount();
	layer_container_node->SetAttribute("size", size); 
	for (uint i = 0; i < size; i++)
	{
		WLayerDataMapper::Write(item->GetLayerByContainerPosition(i), doc, layer_container_node);
	}
	raster_node->InsertEndChild(layer_container_node);
}

//std::shared_ptr<WLayer> WLayerDataMapper::Read(size_t)
//{
//	std::shared_ptr<WLayer> a;
//	return a;
//}

void WLayerDataMapper::Write(WLayer* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* layer_node = doc->NewElement("layer");
	layer_node->SetAttribute("Name", item->getName().c_str());
	layer_node->SetAttribute("LayerUUID", item->getID().c_str());
	layer_node->SetAttribute("LAYER_TYPE", item->getType());
	WRangeDataMapper::Write(&item->getRange(), doc, layer_node);

	tinyxml2::XMLElement* vector_object_container_node = doc->NewElement("vector_object_container");
	layer_node->InsertEndChild(vector_object_container_node);
	uint size = item->getContainerSize();
	vector_object_container_node->SetAttribute("size", size); 
	for (uint i = 0; i < size; i++)
	{
		if(item->getType() == WLayer::LAYER_TYPE_ENUM::LT_LINES)
		{
			WLine* line_item = dynamic_cast<WLine*>(item->GetObject(i));
			WLineDataMapper::Write(line_item, doc, vector_object_container_node);
		}

		if(item->getType() == WLayer::LAYER_TYPE_ENUM::LT_TEXT)
		{
			WText* text_item = dynamic_cast<WText*>(item->GetObject(i));
			WTextDataMapper::Write(text_item, doc, vector_object_container_node);
		}
	}		
	node->InsertEndChild(layer_node);
}

void WRangeDataMapper::Write(w_range* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* range_node = doc->NewElement("range");
	range_node->SetAttribute("low[r]", item->getLow().r);
	range_node->SetAttribute("low[g]", item->getLow().g);
	range_node->SetAttribute("low[b]", item->getLow().b);
	range_node->SetAttribute("high[r]", item->getHigh().r);
	range_node->SetAttribute("high[g]", item->getHigh().g);
	range_node->SetAttribute("high[b]", item->getHigh().b);
	node->InsertEndChild(range_node);
}

void CVPointDataMapper::Write(cv::Point* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* point_node = doc->NewElement("point");
	point_node->SetAttribute("x", item->x);
	point_node->SetAttribute("y", item->y);
	node->InsertEndChild(point_node);
}

void WLineDataMapper::Write(WLine* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* line_node = doc->NewElement("line");
	line_node->SetAttribute("width", item->GetWidth());
	node->InsertEndChild(line_node);

	tinyxml2::XMLElement* ponts_container_node = doc->NewElement("points_container");
	uint size = item->Length();
	ponts_container_node->SetAttribute("size", size); 
	for (uint i = 0; i < size; i++)
	{
		CVPointDataMapper::Write(item->GetItem(i), doc, ponts_container_node);
	}
	line_node->InsertEndChild(ponts_container_node);
}

void WTextDataMapper::Write(WText* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* text_node = doc->NewElement("text");
	text_node->SetAttribute("text", item->GetText().c_str());
	text_node->SetAttribute("status", item->GetState());
	text_node->SetAttribute("angle", item->GetAngle());	
	node->InsertEndChild(text_node);

	tinyxml2::XMLElement* ponts_container_node = doc->NewElement("points_container");
	uint size = item->Length();
	ponts_container_node->SetAttribute("size", size); 
	for (uint i = 0; i < size; i++)
	{
		CVPointDataMapper::Write(item->GetItem(i), doc, ponts_container_node);
	}
	text_node->InsertEndChild(ponts_container_node);
}

SDK_END_NAMESPACE