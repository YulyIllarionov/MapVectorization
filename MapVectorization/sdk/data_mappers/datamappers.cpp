#include "stdafx.h"
#include "datamappers.h"


SDK_BEGIN_NAMESPACE

void WRasterDataMapper::Write(std::shared_ptr<WRaster> item, tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLNode* raster_node = doc->NewElement("raster");	
	doc->InsertEndChild(raster_node);
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

void WLayerDataMapper::Write(WLayer* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* layer_node = doc->NewElement("layer");
	layer_node->SetAttribute("Name", item->getName().c_str());
	layer_node->SetAttribute("LayerUUID", item->getID().c_str());
	layer_node->SetAttribute("LAYER_TYPE", item->getType());
	layer_node->SetAttribute("GroupID", item->getGroupId().c_str());
	WRangeDataMapper::Write(&item->getRange(), doc, layer_node);

	tinyxml2::XMLElement* layer_mask_node = doc->NewElement("layer_mask");
	layer_mask_node->SetAttribute("cols", item->m_data.cols);
	layer_mask_node->SetAttribute("rows", item->m_data.rows);
	WMaskDataMapper::Write(item->m_data, doc, layer_mask_node);
	layer_node->InsertEndChild(layer_mask_node);
	
	tinyxml2::XMLElement* vector_object_container_node = doc->NewElement("vector_object_container");
	layer_node->InsertEndChild(vector_object_container_node);
	uint size = item->getContainerSize();
	vector_object_container_node->SetAttribute("size", size); 
	for (uint i = 0; i < size; i++)
	{
		if(item->getType() == WLayer::LAYER_TYPE_ENUM::LT_LINES)
		{
			auto line_item = std::dynamic_pointer_cast<WLine>(item->GetObject(i));
			WLineDataMapper::Write(line_item, doc, vector_object_container_node);
		}

		if(item->getType() == WLayer::LAYER_TYPE_ENUM::LT_TEXT)
		{
			auto text_item = std::dynamic_pointer_cast<WText>(item->GetObject(i));
			WTextDataMapper::Write(text_item, doc, vector_object_container_node);
		}
	}		
	node->InsertEndChild(layer_node);
}
 
void WMaskDataMapper::Write(cv::Mat& item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	for (int x = 0; x < item.cols; x++)
    {
        for (int y = 0; y < item.rows; y++)
        {
			cv::Point tmp(x, y);
			if (item.at<uchar>(tmp) != 0)
				CVPointDataMapper::Write(&tmp, doc, node); 
		}
	}
}

void WRangeDataMapper::Write(w_range* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* range_node = doc->NewElement("range");
	range_node->SetAttribute("lowR", item->getLow().r);
	range_node->SetAttribute("lowG", item->getLow().g);
	range_node->SetAttribute("lowB", item->getLow().b);
	range_node->SetAttribute("highR", item->getHigh().r);
	range_node->SetAttribute("highG", item->getHigh().g);
	range_node->SetAttribute("highB", item->getHigh().b);
	node->InsertEndChild(range_node);
}

void CVPointDataMapper::Write(cv::Point* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* point_node = doc->NewElement("point");
	point_node->SetAttribute("x", item->x);
	point_node->SetAttribute("y", item->y);
	node->InsertEndChild(point_node);
}

void WLineDataMapper::Write(std::shared_ptr<WLine> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
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

void WTextDataMapper::Write(std::shared_ptr<WText> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
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
//------------------------------------------------------------

std::shared_ptr<WRaster> WRasterDataMapper::Read(tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLNode* raster_node = doc->FirstChild();
	if (raster_node == nullptr)
		return nullptr;

	tinyxml2::XMLElement* filepath_node = raster_node->FirstChildElement("filepath");
	if (filepath_node == nullptr)
		return nullptr;
	std::string filepath = filepath_node->GetText();

	auto result = std::make_shared<WRaster>(filepath);
	
	tinyxml2::XMLElement* layer_container_node = raster_node->FirstChildElement("layer_container");
	if (layer_container_node == nullptr)
		return result;

	tinyxml2::XMLElement* layer_node = layer_container_node->FirstChildElement("layer");
	while (layer_node != nullptr)
	{
		WLayer layer = WLayerDataMapper::Read(layer_node);
		result->AddLayer(layer);
		layer_node = layer_node->NextSiblingElement("layer");
	}

	return result;
}

WLayer WLayerDataMapper::Read(tinyxml2::XMLElement* node)
{
	WLayer result;

	const char* c_name = nullptr;
	c_name = node->Attribute("Name");
	if (c_name)
	{
		std::string name = c_name;
		result.setName(name);
	}

	const char* c_layeruuid = nullptr;
	c_layeruuid = node->Attribute("LayerUUID");
	if (c_layeruuid)
	{
		std::string layer_uuid = c_layeruuid;
		result.setID(layer_uuid);
	}

	const char* c_groupid = nullptr;
	c_layeruuid = node->Attribute("GroupID");
	if (c_groupid)
	{
		std::string layer_groupid = c_groupid;
		result.setGroupId(layer_groupid);
	}

	uint type;
	tinyxml2::XMLError m_error = node->QueryUnsignedAttribute("LAYER_TYPE", &type);
	result.setType(type);

	tinyxml2::XMLElement* range_node = node->FirstChildElement("range");
	w_range range = WRangeDataMapper::Read(range_node);	
	result.setRange(range);

	tinyxml2::XMLElement* layer_mask_node = node->FirstChildElement("layer_mask");
	cv::Mat mask = WMaskDataMapper::Read(layer_mask_node);
	result.m_data = mask;
	
	tinyxml2::XMLElement* vector_object_container_node = node->FirstChildElement("vector_object_container");
	if (type == WLayer::LAYER_TYPE_ENUM::LT_LINES)
	{
		tinyxml2::XMLElement*  vector_object_node = vector_object_container_node->FirstChildElement("line");
		while (vector_object_node != nullptr)
		{
			auto line = WLineDataMapper::Read(vector_object_node);
			result.AddVectorElement(line);
			vector_object_node = vector_object_node->NextSiblingElement("line");
		}
	}

	if (type == WLayer::LAYER_TYPE_ENUM::LT_TEXT)
	{
		tinyxml2::XMLElement*  vector_object_node = vector_object_container_node->FirstChildElement("text");
		while (vector_object_node != nullptr)
		{
			auto text = WTextDataMapper::Read(vector_object_node);
			result.AddVectorElement(text);
			vector_object_node = vector_object_node->NextSiblingElement("text");
		}
	}
	return result;
}

cv::Mat WMaskDataMapper::Read(tinyxml2::XMLElement* node)
{
	uint rows, cols;
	tinyxml2::XMLError m_error;
	m_error = node->QueryUnsignedAttribute("rows", &rows);
	m_error = node->QueryUnsignedAttribute("cols", &cols);

	cv::Mat result = cv::Mat::zeros(rows, cols, CV_8UC1);
	tinyxml2::XMLElement*  point_node = node->FirstChildElement("point");
	while (point_node != nullptr)
	{
		cv::Point tmp = CVPointDataMapper::Read(point_node);
		result.at<uchar>(tmp) = 255;
		point_node = point_node->NextSiblingElement("point");
	}

	return result;
}

w_range WRangeDataMapper::Read(tinyxml2::XMLElement* node)
{
	w_range result;
	uint r, g, b;
	tinyxml2::XMLError m_error;
	m_error = node->QueryUnsignedAttribute("lowR", &r);
	m_error = node->QueryUnsignedAttribute("lowG", &g);
	m_error = node->QueryUnsignedAttribute("lowB", &b);
	cv::Vec3b low = cv::Vec3b(b, g, r);
	m_error = node->QueryUnsignedAttribute("highR", &r);
	m_error = node->QueryUnsignedAttribute("highG", &g);
	m_error = node->QueryUnsignedAttribute("highB", &b);
	cv::Vec3b high = cv::Vec3b(b, g, r);
	result.setData(low, high);
	return result;
}

cv::Point CVPointDataMapper::Read(tinyxml2::XMLElement* node)
{
	uint x, y;
	tinyxml2::XMLError m_error;
	m_error = node->QueryUnsignedAttribute("x", &x);
	m_error = node->QueryUnsignedAttribute("y", &y);

	cv::Point result = cv::Point(x, y);
	return result;
}	

std::shared_ptr<WLine> WLineDataMapper::Read(tinyxml2::XMLElement* node)
{
	std::vector<cv::Point> points;
	tinyxml2::XMLElement* points_container_node = node->FirstChildElement("points_container");
	tinyxml2::XMLElement* point_node = points_container_node->FirstChildElement("point");
	while (point_node != nullptr)
	{
		auto point = CVPointDataMapper::Read(point_node);
		points.push_back(point);
		point_node = point_node->NextSiblingElement("point");
	}

	auto result = std::make_shared<WLine>(points);
	//tinyxml2::XMLElement* width_node = node->FirstChildElement("width");
	double width;	
	tinyxml2::XMLError m_error = node->QueryDoubleAttribute("width", &width);
	result->SetWidth(width);
	return result;
}

std::shared_ptr<WText> WTextDataMapper::Read(tinyxml2::XMLElement* node)
{
	std::vector<cv::Point> points;
	tinyxml2::XMLElement* points_container_node = node->FirstChildElement("points_container");
	tinyxml2::XMLElement* point_node = points_container_node->FirstChildElement("point");
	while (point_node != nullptr)
	{
		auto point = CVPointDataMapper::Read(point_node);
		points.push_back(point);
		point_node = point_node->NextSiblingElement("point");
	}

	auto result = std::make_shared<WText>(points);

	bool status;
	tinyxml2::XMLError m_error = node->QueryBoolAttribute("status", &status);
	result->SetState(status);

	float angle;
	m_error = node->QueryFloatAttribute("angle", &angle);
	result->setAngle(angle);

	const char* c_text = nullptr;
	c_text = node->Attribute("text");
	if (c_text)
	{
		std::string text = c_text;
		result->AddText(text);
	}

	return result;
}

// ---------------------------------------------------------------------------------------

void WRasterDataMapper::WriteSVG(std::shared_ptr<WRaster> item, tinyxml2::XMLDocument* doc)
{
	tinyxml2::XMLElement* raster_node = doc->NewElement("svg");	
	raster_node->SetAttribute("version", 1.1);
	raster_node->SetAttribute("width", item->m_raster.cols);
	raster_node->SetAttribute("height", item->m_raster.rows);
	raster_node->SetAttribute("baseProfile", "full");
	raster_node->SetAttribute("xmlns", "http://www.w3.org/2000/svg");
	raster_node->SetAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
	raster_node->SetAttribute("xmlns:ev", "http://www.w3.org/2001/xml-events");
	doc->InsertEndChild(raster_node);
	
	uint size = item->GetLayersCount();
	for (uint i = 0; i < size; i++)
	{
		WLayerDataMapper::WriteSVG(item->GetLayerByContainerPosition(i), doc, raster_node);
	}
}

void WLayerDataMapper::WriteSVG(WLayer* item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	uint size = item->getContainerSize();
	if (item->getType() == WLayer::LAYER_TYPE_ENUM::LT_LINES)
	{
		for (uint i = 0; i < size; i++)
		{
			auto line_item = std::dynamic_pointer_cast<WLine>(item->GetObject(i));
			WLineDataMapper::WriteSVG(line_item, doc, node);
			
		}
	}

	else if (item->getType() == WLayer::LAYER_TYPE_ENUM::LT_TEXT)
	{
		for (uint i = 0; i < size; i++)
		{
			auto text_item = std::dynamic_pointer_cast<WText>(item->GetObject(i));
			WTextDataMapper::WriteSVG(text_item, doc, node);
		}	
	}

	else
		return;
}

void WLineDataMapper::WriteSVG(std::shared_ptr<WLine> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* line_node = doc->NewElement("polyline");
	line_node->SetAttribute("stroke-width", item->GetWidth());
	line_node->SetAttribute("stroke", "rgb(0,0,0)");
	line_node->SetAttribute("fill", "rgb(255,255,255)");

	std::string points("");
	uint size = item->Length();
	for (uint i = 0; i < size; i++)
	{
		auto current = item->GetItem(i);
		std::string x = std::to_string(current->x);
		std::string y = std::to_string(current->y);
		std::string tmp = x + "," + y;
		points += tmp;
		points += " ";
	}
	line_node->SetAttribute("points", points.c_str());
	node->InsertEndChild(line_node);
}

void WTextDataMapper::WriteSVG(std::shared_ptr<WText> item, tinyxml2::XMLDocument* doc, tinyxml2::XMLNode* node)
{
	tinyxml2::XMLElement* text_node = doc->NewElement("text");
	text_node->SetAttribute("x", item->GetPoint(3).GetX());
	text_node->SetAttribute("y", item->GetPoint(3).GetY());
	
	std::string font_size = std::to_string(abs(item->GetPoint(0).GetY() - item->GetPoint(3).GetY()) * 
		std::cos(item->GetAngle() * CV_PI/180));
	font_size += "px";
	text_node->SetAttribute("font-size", font_size.c_str());
	if(item->GetText().empty())
		text_node->SetText("?");
	else
		text_node->SetText(item->GetText().c_str());
	
	node->InsertEndChild(text_node);
}

SDK_END_NAMESPACE