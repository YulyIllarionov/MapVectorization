/// Whiteteam
/// \file base_types.cpp
/// \brief This file contains base types
/// \author Whiteteam authors

#include "stdafx.h"
#include <fstream>  
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/text.hpp"

#include "util/utils.h"
#include "util/math_utils.h"
#include "graphics/skeletonization.h"
#include <stack>
#include <numeric>

#include <math.h>
#include <algorithm>
#include <locale>
#include <codecvt>
#include "base_types.h"



SDK_BEGIN_NAMESPACE

using namespace cv;

// ------------------------------------------------------------
w_color::w_color(uchar r, uchar g, uchar b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}
// ------------------------------------------------------------
w_color::w_color(const cv::Vec3b& color)
{
    this->r = color[2];
    this->g = color[1];
    this->b = color[0];
}
// ------------------------------------------------------------
w_color::w_color(const cv::Vec4b& color)
{
    this->r = color[2];
    this->g = color[1];
    this->b = color[0];
}
// ------------------------------------------------------------
cv::Vec3b w_color::toVec3b() const
{
    return Vec3b(this->b, this->g, this->r);
}
// ------------------------------------------------------------
inline bool operator <= (const w_color &first, const cv::Vec3b &second)
{
    return bool((first.r <= second[2]) && (first.g <= second[1]) && (first.b <= second[0]));
}
// ------------------------------------------------------------
inline bool operator >= (const w_color &first, const cv::Vec3b &second)
{
    return bool((first.r >= second[2]) && (first.g >= second[1]) && (first.b >= second[0]));
}
// ------------------------------------------------------------
w_range::w_range()
{
    this->low = Vec3b(255, 255, 255);
    this->high = Vec3b(0, 0, 0);
}
// ------------------------------------------------------------
//Каждая из компонент входного цвета является кандидатом на новое значение границы
void w_range::addColor(const w_color& color)
{
    this->low[2] = std::min<uchar>(color.r, low[2]);
    this->low[1] = std::min<uchar>(color.g, low[1]);
    this->low[0] = std::min<uchar>(color.b, low[0]);
    this->high[2] = std::max<uchar>(color.r, high[2]);
    this->high[1] = std::max<uchar>(color.g, high[1]);
    this->high[0] = std::max<uchar>(color.b, high[0]);
}
// ------------------------------------------------------------
inline bool w_range::contains(const cv::Vec3b& color)
{
    return ((color >= this->low) && (color <= this->high));
}
// ------------------------------------------------------------
w_color w_range::getLow()
{
    return w_color(this->low);
}
// ------------------------------------------------------------
w_color w_range::getHigh()
{
    return w_color(this->high);
}
// ------------------------------------------------------------
void w_range::setData(cv::Vec3b m_low, cv::Vec3b m_high)
{
	low = m_low;
	high = m_high;
}
// ------------------------------------------------------------
void WLayer::DrawCircle(SMapPoint point, uint radius, uchar color)
{
    circle(m_data, point.ToPoint(), radius, color, -1);
}
// ------------------------------------------------------------
SDKResult WLayer::InicializeLinesContainer()
{
    if (m_type != LT_LINES)
        return kSDKResult_Error;

    WObjectContainer lines = SDK_NAMESPACE::utils::FindLinesOnMat(m_data);
    m_objects.insert(m_objects.end(), lines.begin(), lines.end());

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WLayer::InicializeTextContainer()
{
    if (m_type != LT_TEXT)
        return kSDKResult_Error;
    //Локализация текста
    WObjectContainer text = SDK_NAMESPACE::utils::FindTextOnMat(m_data);
    std::vector<int> idxs(text.size());
    for (size_t i = 0; i < idxs.size(); i++)
    {
        idxs[i] = m_objects.size() + i;
    }
    m_objects.insert(m_objects.end(), text.begin(), text.end());
    //Распознавание текста
    this->RecognizeText(idxs, 0.0);
 
    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
void WLayer::InicializeVectorContainer()
{
    switch (this->m_type)
    {
    case WLayer::LAYER_TYPE_ENUM::LT_NONE:
        break;

    case WLayer::LAYER_TYPE_ENUM::LT_LINES:
    {
        this->InicializeLinesContainer();
    }
    break;

    case WLayer::LAYER_TYPE_ENUM::LT_TEXT:
    {
        this->InicializeTextContainer();
    }
    break;

    case WLayer::LAYER_TYPE_ENUM::LT_OTHER:
    {
    }
    break;

    default:
        break;
    }
}
// ------------------------------------------------------------
WRaster::WRaster(const std::string& imgPath)
{
    if (!imgPath.empty())
        Initialize(imgPath);
}
// ------------------------------------------------------------
void WRaster::Initialize(const std::string& imgPath)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	m_image_path = converter.from_bytes(imgPath);
    // Read the file
    Mat raster = imread(String(imgPath), CV_LOAD_IMAGE_COLOR);
    cvtColor(raster, m_raster, CV_BGR2BGRA, 4);
}
// ------------------------------------------------------------
SDKResult WRaster::IncreaseSharpness(double k) const
{
    //filter2D(m_raster, m_raster, m_raster.depth(), utils::SharpKernel(k));
    //Ядро свертки для увеличения резкости
    Mat kernel = utils::SharpKernel(k);
    Mat img;
    cvtColor(m_raster, img, CV_BGRA2BGR);
    Mat imgOut(img);
    Mat imgGray;
    Mat imgEdges;

    //Детектор границ Кэнни
    cvtColor(m_raster, imgGray, COLOR_RGB2GRAY);
    Canny(imgGray, imgEdges, 90, 200);
    //Увеличени резкости: свертка изображения с ядром
    for (int y = 1; y < img.rows - 1; y++)
    {
        for (int x = 1; x < img.cols - 1; x++)
        {
            if (imgEdges.at<uchar>(y, x) > 0)
            {
                Rect region_of_interest = Rect(x - 1, y - 1, 3, 3);
                Mat image_roi = img(region_of_interest);
                Vec3d sum(0.0, 0.0, 0.0);
                Vec3d currentColor;
                for (int y1 = 0; y1 < image_roi.rows; y1++)
                {
                    for (int x1 = 0; x1 < image_roi.cols; x1++)
                    {
                        currentColor = image_roi.at<Vec3b>(y1, x1);
                        sum += kernel.at<double>(y1, x1)*currentColor;
                    }
                }
                imgOut.at<Vec3b>(y, x) = sum;
            }
        }
    }
    cvtColor(imgOut, m_raster, CV_BGR2BGRA);
    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
void WRaster::SegmentationBilateral()
{
    Mat img;
    Mat imgOut;
    cvtColor(m_raster, img, CV_BGRA2BGR);
    bilateralFilter(img, imgOut, 9, 50, 0);
    cvtColor(imgOut, m_raster, CV_BGR2BGRA);
}
// ------------------------------------------------------------
void WRaster::SegmentationMeanshift()
{
    Mat img;
    Mat imgOut;
    cvtColor(m_raster, img, CV_BGRA2BGR);
    pyrMeanShiftFiltering(img, imgOut, 5, 40, 0);
    cvtColor(imgOut, m_raster, CV_BGR2BGRA);
}
// ------------------------------------------------------------
WLayer* WRaster::AddLayer(const GroupID& groupId)
{
    WLayer layer;
    layer.m_data = Mat(m_raster.size(), CV_8UC1, Scalar(0));
    layer.setID(utils::genUUID());
    layer.getGroupId() = groupId.empty() ? utils::genUUID() : groupId;
    m_layers.push_back(layer);

    return &m_layers.back();
}
// ------------------------------------------------------------
void WRaster::AddLayer(WLayer layer)
{
	m_layers.push_back(layer);
}
// ------------------------------------------------------------
SDKResult WRaster::RemoveLayer(const LayerUUID& layerId)
{
    LayersContainer::const_iterator cit = m_layers.begin();
    for (; cit != m_layers.end(); ++cit)
    {
        if (cit->getID() == layerId)
            break;
    }

    if (cit == m_layers.end())
        return kSDKResult_NotFound;

    m_layers.remove(*cit);

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::AddColorToLayer(const LayerUUID& layerId, const w_color& color)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    layer->addColorToRange(color);

    for (int y = 0; y < m_raster.rows; y++)
    {
        for (int x = 0; x < m_raster.cols; x++)
        {
            const Vec4b currentColorOld = m_raster.at<Vec4b>(y, x);
            const Vec3b currentColor(currentColorOld[0], currentColorOld[1], currentColorOld[2]);
            layer->m_data.at<uchar>(y, x) = layer->colorContains(currentColor) ? 255 : 0;
        }
    }

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::AddLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    layer->addType(type);

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    layer->setType(type);

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::RemoveLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    layer->removeType(type);

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerName(const LayerUUID& layerId, const std::string& name)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    layer->setName(name);
    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
WLayer* WRaster::GetLayerById(const LayerUUID& layerId)
{
    for (LayersContainer::iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
    {
        if (cit->getID() == layerId)
            return &(*cit);
    }
    return nullptr;
}
// ------------------------------------------------------------
WLayer* WRaster::GetLayerByName(const std::string& name)
{
    for (LayersContainer::iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
    {
        if (cit->getName() == name)
            return &(*cit);
    }
    return nullptr;
}
//-------------------------------------------------------------
WLayer* WRaster::GetLayerByContainerPosition(uint pos)
{
	if (pos < m_layers.size())
	{
		auto it = m_layers.begin();
		std::advance(it, pos);
		return &*it;
	}
	return nullptr;
}
// ------------------------------------------------------------
SDKResult WRaster::GetLayersByType(WLayer::LAYER_TYPE type, LayerIDs& layer_ids) const
{
    layer_ids.clear();
    for (LayersContainer::const_iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
    {
        if ((cit->getType() & type) != 0)
            layer_ids.push_back(cit->getID());
    }
    return kSDKResult_Succeeded;
}
//------------------------------------------------------------
std::string WRaster::GetImgPath()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(m_image_path);
}
// ------------------------------------------------------------
// use only as an example. define other split funcs as a member of the WRaster class
SDKResult splitFuncExample(WLayer* layer, WLayer* l1, WLayer* l2)
{
    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SplitLayer(const LayerUUID& layerId, LayerIDs& splittedLayers)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    SDKResult result = kSDKResult_Error;

    switch (layer->getType())
    {
    case WLayer::LAYER_TYPE_ENUM::LT_NONE:
        break;
    case WLayer::LAYER_TYPE_ENUM::LT_LINES | WLayer::LAYER_TYPE_ENUM::LT_TEXT /*| WLayer::LAYER_TYPE_ENUM::LT_OTHER*/:
    {
        WLayer* linesLayer = this->AddLayer(layer->getGroupId());
        linesLayer->setType(WLayer::LAYER_TYPE_ENUM::LT_LINES);
        linesLayer->setName(std::string("Lines from ") + layer->getName());
        //this->SetLayerType(linesLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_LINES);
        //this->SetLayerName(linesLayer->getID(), std::string("Lines from ") + layer->getName());
        WLayer* othersLayer = this->AddLayer(layer->getGroupId());
        othersLayer->setType(WLayer::LAYER_TYPE_ENUM::LT_TEXT);
        othersLayer->setName(std::string("Text from ") + layer->getName());
        //this->SetLayerType(othersLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_TEXT);
        //this->SetLayerName(othersLayer->getID(), std::string("Text from ") + layer->getName());
        this->SplitLines(layerId, linesLayer->getID(), othersLayer->getID());
        splittedLayers.clear();
        splittedLayers.push_back(linesLayer->getID());
        splittedLayers.push_back(othersLayer->getID());

        linesLayer = nullptr;
        othersLayer = nullptr;
        break;
    }
    //case WLayer::LAYER_TYPE_ENUM::LT_LINES | WLayer::LAYER_TYPE_ENUM::LT_TEXT:
    //{
    //    break;
    //}
    //case WLayer::LAYER_TYPE_ENUM::LT_TEXT | WLayer::LAYER_TYPE_ENUM::LT_OTHER:
    //{
    //    WLayer* textLayer = this->AddLayer(layer->getGroupId());
    //    this->SetLayerType(textLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_TEXT);
    //    this->SetLayerName(textLayer->getID(), std::string("Text from ") + layer->getName());
    //    
    //    WLayer* othersLayer = this->AddLayer(layer->getGroupId());
    //    this->SetLayerType(othersLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_OTHER);
    //    this->SetLayerName(othersLayer->getID(), std::string("Other from ") + layer->getName());
    //    
    //    this->SplitText(layerId, textLayer->getID(), othersLayer->getID());
    //    
    //    splittedLayers.clear();
    //    splittedLayers.push_back(textLayer->getID());
    //    splittedLayers.push_back(othersLayer->getID());
    //    break;
    //}
    default:
    {
        WLayer* l1 = AddLayer();
        WLayer* l2 = AddLayer();
        result = splitFuncExample(layer, l1, l2);
        l1 = nullptr;
        l2 = nullptr;
        if (S_Ok(result))
            RemoveLayer(layer->getID());
        layer = nullptr;
    }
    break;
    }

    return result;
}
// ------------------------------------------------------------
SDKResult WRaster::GetLayersByGroupId(const GroupID& groupId, LayerIDs& relatedLayers) const
{
    relatedLayers.clear();
    for (LayersContainer::const_iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
    {
        if (cit->getGroupId() == groupId)
            relatedLayers.push_back(cit->getGroupId());
    }

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
std::vector<cv::Rect> WRaster::DetectLetters(const LayerUUID& layerId)
{
    std::vector<cv::Rect> boundRect;

    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return boundRect;

    cv::Mat img = layer->m_data;
    cv::Mat img_sobel, img_threshold, element;
    cv::Sobel(img, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

    element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3));
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);

    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1);

    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        if (contours.at(i).size()>100)
        {
            cv::approxPolyDP(cv::Mat(contours.at(i)), contours_poly.at(i), 3, true);
            cv::Rect appRect(boundingRect(cv::Mat(contours_poly.at(i))));
            if (appRect.width > appRect.height)
                boundRect.push_back(appRect);
        }
    }

    return boundRect;
}
// ------------------------------------------------------------
std::vector<int> WRaster::DefineObjectsNearPoint(const LayerUUID& layerId, SMapPoint point)
{
    Point pointCV(point.GetX(), point.GetY());
    std::vector<int> ids;
    WLayer* layer = GetLayerById(layerId);
    if (!layer->IsSingleType())
        return ids;

    double distance = DBL_MAX;
    int index;

    const size_t containerSize = layer->VectorContainerElementsNumber();
    for (size_t i = 0; i < containerSize; i++)
    {
        auto currentObject = layer->GetVectorElement(i);
        if (currentObject != NULL)
        {
            double currentDistance = currentObject->DistanceTo(pointCV);
            if (currentDistance < distance)
            {
                distance = currentDistance;
                index = i;
            }
        }
    }
    ids.push_back(index);
    return ids;
}
// ------------------------------------------------------------
// define objects inside polygon
std::vector<int> WRaster::DefineObjectsInsidePolygon(const LayerUUID& layerId, const WPolygon& mapPoints)
{
    std::vector<int> ids;
    WLayer* layer = GetLayerById(layerId);
    if (!layer->IsSingleType() || mapPoints.Length() == 0)
        return ids;

    const size_t containerSize = layer->VectorContainerElementsNumber();
    for (size_t i = 0; i <= containerSize; i++)
    {
        auto currentObject = layer->GetVectorElement(i);
        if (currentObject != NULL)
            if (mapPoints.Contains(*currentObject))
                ids.push_back(i);
    }

    return ids;
}
// ------------------------------------------------------------
SDKResult WRaster::PasteObjectsToLayer(const LayerUUID& layerId, std::vector<std::vector<Wregion>> rasterObjects)
{
    WLayer* layer = GetLayerById(layerId);
    if (!layer->IsSingleType())
        return kSDKResult_Error;
    for (size_t i = 0; i < rasterObjects.size(); i++)
    {
        //Объединение всех связных областей текущего объекта в один вектор
        std::vector<Point> objectPoints;
        for (size_t j = 0; j < rasterObjects[i].size(); j++)
        {
            std::vector<Point> currentPoints = rasterObjects[i][j].GetPoints();
            objectPoints.insert(objectPoints.end(), currentPoints.begin(), currentPoints.end());
        }
        //Вставка растровых объектов на слой
        for (size_t j = 0; j < objectPoints.size(); j++)
        {
            layer->m_data.at<uchar>(objectPoints[j]) = 255;
        }
        //Получение векторных объектов для имеющихся растровых
        WObjectContainer vectorObjects;
        switch (layer->getType())
        {
        case WLayer::LT_LINES:
        {
            Rect roi = boundingRect(objectPoints);
            Mat linesImg(roi.size(), CV_8UC1, Scalar(0));
            for (size_t j = 0; j < objectPoints.size(); j++)
            {
                linesImg.at<uchar>(objectPoints[j] - roi.tl()) = 255;
            }
            //imwrite("textToLines.png", linesImg);
            vectorObjects = SDK_NAMESPACE::utils::FindLinesOnMat(linesImg);
            for (size_t j = 0; j < vectorObjects.size(); j++)
                for (size_t k = 0; k < vectorObjects[j]->m_points.size(); k++)
                    vectorObjects[j]->m_points[k] += roi.tl();

            break;
        }
        case WLayer::LT_TEXT:
        {
            cv::RotatedRect lineRect = cv::minAreaRect(objectPoints);
            vectorObjects.push_back(std::make_shared<WText> (WPolygon(lineRect, layer->m_data.size()).m_points));//TODO 

            break;
        }
        default:
            break;
        }
        //Костыль
        for (size_t i = 0; i < vectorObjects.size(); i++)
            layer->AddVectorElement(vectorObjects[i]);
    }
    //Распознавание текста 
    if (layer->getType() == WLayer::LT_TEXT)
    {
        std::vector<int> textIdxs(rasterObjects.size());
        std::iota(textIdxs.begin(), textIdxs.end(), layer->VectorContainerElementsNumber() - 1 - rasterObjects.size());
        layer->RecognizeText(textIdxs, 0.0);
    }
    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
std::vector<std::vector<Wregion>> WRaster::CutObjectsFromLayer(const LayerUUID& layerId, std::vector<size_t> idxs)
{
    WLayer* layer = GetLayerById(layerId);
    if (!layer->IsSingleType())
        return std::vector<std::vector<Wregion>>();

    std::vector<std::vector<Wregion>> regions;
    switch (layer->getType())
    {
    case WLayer::LAYER_TYPE_ENUM::LT_LINES:
    {
        for (size_t i = 0; i < idxs.size(); i++)
        {
            //Вырезание с растрового слоя 
            auto line = std::dynamic_pointer_cast<WLine>(layer->GetVectorElement(idxs[i]));
            if (line != NULL)
                regions.push_back(line->CutFromLayer(layer));
        }
        break;
    }
    case WLayer::LAYER_TYPE_ENUM::LT_TEXT:
    {
        for (size_t i = 0; i < idxs.size(); i++)
        {
            //Вырезание с растрового слоя 
            auto text = std::dynamic_pointer_cast<WText>(layer->GetVectorElement(idxs[i]));
            if (text != NULL)
                regions.push_back(text->CutFromLayer(layer));
        }
        break;
    }
    }
    //Удаление из векторной коллекции 
    layer->RemoveVectorElements(idxs);

    return regions;
}
// ------------------------------------------------------------
WPolygon::WPolygon(cv::Rect rect)
{
    m_points.push_back(rect.tl());
    m_points.push_back(rect.tl() + cv::Point(rect.width, 0));
    m_points.push_back(rect.br());
    m_points.push_back(rect.tl() + cv::Point(0, rect.height));
}
// ------------------------------------------------------------
WPolygon::WPolygon(cv::RotatedRect rect, cv::Size borders)
{
    Point2f rectPoints[4];
    rect.points(rectPoints);
    m_points.resize(4);
    for (size_t i = 0; i < 4; i++)
    {
        m_points[i].x = std::max((int)rectPoints[i].x, 0);
        m_points[i].x = std::min((int)rectPoints[i].x, borders.width);
        m_points[i].y = std::max((int)rectPoints[i].y, 0);
        m_points[i].y = std::min((int)rectPoints[i].y, borders.height);
    }
}
// ------------------------------------------------------------
WPolygon::WPolygon(std::vector<SMapPoint> & mapPoints)
{
    for (int i = 0; i < mapPoints.size(); i++) {
        m_points.push_back(cv::Point(mapPoints[i].GetX(), mapPoints[i].GetY()));
    }
};
// ------------------------------------------------------------
WPolygon::WPolygon(const std::vector<cv::Point> & mapPoints)
{
    m_points = mapPoints;
};
// ------------------------------------------------------------
bool WPolygon::Contains(const WVectorObject& object) const
{
    bool result = false;
    WPointsContainer::const_iterator cit = object.m_points.begin();
    for (; cit != object.m_points.end(); cit++)
    {
        if (pointPolygonTest(m_points, *cit, false) >= 0)
            result = true;
        else
            return false;
    }
    return result;
}
// ------------------------------------------------------------
bool WPolygon::Contains(const cv::Point& point) const
{
    return pointPolygonTest(m_points, point, false) >= 0;
}
// ------------------------------------------------------------
cv::Point* WVectorObject::GetItem(uint i) 
{
	if (i < m_points.size())
		return &m_points[i]; 
	return nullptr;
};
// ------------------------------------------------------------
double WVectorObject::DistanceTo(const cv::Point& mapPoint) const
{
    int squaredDistance = INT_MAX;
    for (size_t i = 0; i < m_points.size(); i++)
    {
        int currentDistance = SDK_NAMESPACE::utils::squaredDistanceBetween(mapPoint, m_points[i]);
        if (currentDistance < squaredDistance)
            squaredDistance = currentDistance;
    }
    return std::sqrt((double)squaredDistance);
}
// ------------------------------------------------------------
//Конкатенация двух линий. Производится по двум ближайщим концам линий
void WLine::Concat(const WLine& line)
{
    std::vector<int> distances;
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.front(), line.m_points.front()));
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.front(), line.m_points.back()));
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.back(), line.m_points.front()));
    distances.push_back(SDK_NAMESPACE::utils::squaredDistanceBetween(m_points.back(), line.m_points.back()));
    std::vector<int>::iterator result = std::min_element(distances.begin(), distances.end());
    if (result == distances.end())
        return;
    switch (result - distances.begin())
    {
    case 0:
        m_points.insert(m_points.begin(), line.m_points.rbegin(), line.m_points.rend());
        break;
    case 1:
        m_points.insert(m_points.begin(), line.m_points.begin(), line.m_points.end());
        break;
    case 2:
        m_points.insert(m_points.end(), line.m_points.rbegin(), line.m_points.rend());
        break;
    case 3:
        m_points.insert(m_points.end(), line.m_points.begin(), line.m_points.end());
        break;
    default:
        break;
    }
}
// ------------------------------------------------------------
void WLine::SimplifyDP(double epsilon)
{
    cv::approxPolyDP(m_points, m_points, epsilon, false);
}
// ------------------------------------------------------------
std::vector<Wregion> WLine::CutFromLayer(WLayer* layer)
{
    if (layer->getType() != WLayer::LAYER_TYPE_ENUM::LT_LINES)
        return std::vector<Wregion>();

    if (this->Length() <= 1)
        return std::vector<Wregion>();

    Wregion line;
    MathUtils::Wvector vec1;
    MathUtils::Wvector vec2;
    MathUtils::Wvector bisectVec;
    cv::Point2f point1;
    cv::Point2f point2;
    std::vector<Point> borderPoints;

    cv::circle(layer->m_data, m_points[0], m_width * 0.7, 0, 2);
    cv::circle(layer->m_data, m_points.back(), m_width * 0.7, 0, 2);

    //cv::namedWindow("lineCut", CV_WINDOW_KEEPRATIO);
    //cv::imshow("lineCut", layer->m_data);
    //cv::waitKey();

    if (m_points.size() == 2)
        m_points.insert(m_points.begin() + 1, 
            MathUtils::Wvector(m_points.front(), m_points.back()).Middle());
    Wregion(m_points.front(), layer->m_data);
    Wregion(m_points.back(), layer->m_data);
    for (size_t i = 1; i < m_points.size() - 1; i++)
    {
        line.Concat(Wregion(m_points[i], layer->m_data));
        //cv::imshow("lineCut", layer->m_data);
        //cv::waitKey();
    }
    //cv::destroyAllWindows();
    
    return std::vector<Wregion>(1, line);
}
// ------------------------------------------------------------
void WLine::FindWidth(const cv::Mat& image)
{
    if (m_points.empty())
        return;

    std::vector<int> widths(m_points.size());
    for (size_t i = 0; i < widths.size(); i++)
    {
        int halfWidth = 1;
        while (SDK_NAMESPACE::utils::SquareFilling(image, m_points[i], halfWidth) > 0.75)
            halfWidth++;
        widths[i] = halfWidth * 2 - 1;
    }
    std::sort(widths.begin(), widths.end());
    m_width = (double)widths[widths.size() / 2];
}
// ------------------------------------------------------------
double WLine::GetWidth()
{
	return m_width;
}
// ------------------------------------------------------------
double WText::RotateToHorizon(WLayer* layer, cv::Mat& img2Recognition)
{
    if (layer->getType() != WLayer::LAYER_TYPE_ENUM::LT_TEXT)
        return 361.0;

    //Копирование полигона на отдельное изображение 
    Rect roi = cv::boundingRect(m_points);
    roi.width = std::min((layer->m_data.cols - roi.x), roi.width);
    roi.height = std::min((layer->m_data.rows - roi.y), roi.height);
	
	if ((roi.x < 0) || (roi.y < 0) || (roi.x >= layer->m_data.cols) || (roi.y >= layer->m_data.rows))
		return 361.0;

    img2Recognition = cv::Mat(roi.size(), CV_8UC1, Scalar(0));
    for (int y = 0; y < img2Recognition.rows; y++)
    {
        for (int x = 0; x < img2Recognition.cols; x++)
        {
            img2Recognition.at<uchar>(y, x) = layer->m_data.at<uchar>(cv::Point(x,y) + roi.tl());
        }
    }
    //Нахождение угла поворота
    std::vector<cv::Vec4i> textLines;
    cv::HoughLinesP(img2Recognition, textLines, 1, CV_PI / 180, 40, img2Recognition.cols / 4, img2Recognition.cols / 8);
    std::vector<double> lineAngles(textLines.size());
    for (size_t i = 0; i < textLines.size(); i++)
    {
        lineAngles[i] = std::atan2(textLines[i][3] - textLines[i][1], textLines[i][2] - textLines[i][0]);
    }
    std::sort(lineAngles.begin(), lineAngles.end());
    double angle = 0.0;
    if (!lineAngles.empty())//(std::abs(lineAngles.back() - lineAngles.front()) < (CV_PI/2))
        angle = lineAngles[lineAngles.size() / 2] * 180 / CV_PI; //TODO заменить на пи

    //Скелетизация изображения
    //SDK_NAMESPACE::WSkeletonizer::Instance().Skeletonize(img2Recognition.clone(), img2Recognition); 

    //Поворот
    cv::Point2f center(img2Recognition.cols / 2.0, img2Recognition.rows / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::Rect bbox = cv::RotatedRect(center, img2Recognition.size(), angle).boundingRect();
    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
    cv::warpAffine(img2Recognition, img2Recognition, rot, bbox.size());
    //namedWindow("before", CV_WINDOW_KEEPRATIO);
    //namedWindow("after", CV_WINDOW_KEEPRATIO);
    //imshow("before", image);
    //imshow("after", img2Recognition);
    //waitKey();
    return angle;
}
// ------------------------------------------------------------
std::vector<Wregion> WText::CutFromLayer(WLayer* layer)
{
    if (layer->getType() != WLayer::LAYER_TYPE_ENUM::LT_TEXT)
        return std::vector<Wregion>();

    Rect roi = boundingRect(m_points);
    std::vector<Wregion> letters;
    for (int y = roi.y; y < roi.y + roi.height; y++)
    {
        for (int x = roi.x; x < roi.x + roi.width; x++)
        {
            Point current(x, y);
            if (this->Contains(current))
            {
                if (layer->m_data.at<uchar>(current) != 0)
                {
                    Wregion currentRegion(current, layer->m_data);
                    if (!currentRegion.IsEmpty())
                        letters.push_back(currentRegion);
                }
            }
        }
    }
    return letters;
}
// ------------------------------------------------------------
Wregion::Wregion(const cv::Point& point, cv::Mat& img)
{
    if (img.at<uchar>(point) == 0)
        return; // <- Исправить

    cv::Point currentPoint;
    std::stack <cv::Point> stack;

    stack.push(point);

    while (!stack.empty())
    {
        currentPoint = stack.top();
        stack.pop(); // Удалить точку из стека
        //Пропуск граничных точек изображения
        if (SDK_NAMESPACE::utils::isEdgePoint(currentPoint, img))
            continue;
        points.push_back(currentPoint); //Добавить точку к результату

        img.at<uchar>(currentPoint) = 0; // Закрасить на изображении

        for (int k = currentPoint.x - 1; k <= currentPoint.x + 1; k++)
        {
            for (int l = currentPoint.y - 1; l <= currentPoint.y + 1; l++)
            {
                //if ((l == point.y) && (k == point.x))
                  //  continue;

                if (img.at<uchar>(l, k) != 0)
                {
                    stack.push(Point(k, l));
                    img.at<uchar>(l, k) = 0; // Закрасить на изображении
                }
            }
        }
    }
}
// ------------------------------------------------------------
Wregion::Wregion(const cv::Point& point, cv::Mat& img, WPolygon edges)
{
    if (img.at<uchar>(point) == 0)
        return;
    if (!edges.Contains(point))
        return;


    cv::Point currentPoint;
    std::stack <cv::Point> stack;

    stack.push(point);

    while (!stack.empty())
    {
        currentPoint = stack.top();
        stack.pop(); // Удалить точку из стека
        //Пропуск граничных точек изображения
        if (SDK_NAMESPACE::utils::isEdgePoint(currentPoint, img))
            continue;
        //Пропуск граничных точек полигона
        if (!edges.Contains(currentPoint))
            continue;
        points.push_back(currentPoint); //Добавить точку к результату

        img.at<uchar>(currentPoint) = 0; // Закрасить на изображении

        for (int k = currentPoint.x - 1; k <= currentPoint.x + 1; k++)
        {
            for (int l = currentPoint.y - 1; l <= currentPoint.y + 1; l++)
            {
                //if ((l == point.y) && (k == point.x))
                //  continue;

                if (img.at<uchar>(l, k) != 0)
                {
                    stack.push(Point(k, l));
                    img.at<uchar>(l, k) = 0; // Закрасить на изображении
                }
            }
        }
    }
}
// ------------------------------------------------------------
cv::Rect Wregion::boundingRectangle()
{
    return boundingRect(points);
}
// ------------------------------------------------------------
int Wregion::Square()
{
    return (int)points.size();
}
// ------------------------------------------------------------
//Проверка является ли связная область линией 
bool Wregion::IsLine()
{
    float radius;
    minEnclosingCircle(points, Point2f(), radius);
    //Отношение площади связной области к площади описанного круга
    double ratio = (double)this->Square() / radius / radius;
    //Превышение отношением порога 
    return (ratio < 0.6);
}
// ------------------------------------------------------------
//Рисование региона на слое
void Wregion::drawOn(Mat& img, uchar color)
{
    for (int i = 0; i < points.size(); i++)
    {
        //if ((points[i].x<img.cols)&& (points[i].y<img.rows))
        img.at<uchar>(points.at(i)) = color;
    }
}
// ------------------------------------------------------------
void Wregion::Concat(const Wregion& other)
{
    points.insert(points.end(), other.points.begin(), other.points.end());
}
// ------------------------------------------------------------
SDKResult WRaster::SplitLines(const LayerUUID& layerId, const LayerUUID& linesLayerID, const LayerUUID& othersLayerID)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NullPointer;

    WLayer* linesLayer = GetLayerById(linesLayerID);
    //linesLayer->m_data = Mat(layer->m_data.size(), layer->m_data.type(), 0);

    WLayer* othersLayer = GetLayerById(othersLayerID);
    //othersLayer->m_data = Mat(layer->m_data.size(), layer->m_data.type(), 0);

    linesLayer->m_data = layer->m_data.clone();
    othersLayer->m_data = cv::Mat(layer->m_data.size(), CV_8UC1, Scalar(0));

    //Скопировано из utils.cpp TODO 
    WObjectContainer text = SDK_NAMESPACE::utils::FindTextOnMat(layer->m_data);

    for (size_t i = 0; i < text.size(); i++)
    {
        Rect roi = boundingRect(text[i]->m_points);
        roi.width = std::min(layer->m_data.cols - roi.x, roi.width);
        roi.height = std::min(layer->m_data.rows - roi.y, roi.height);
        for (int y = roi.y; y < (roi.y + roi.height); y++)
        {
            for (int x = roi.x; x < (roi.x + roi.width); x++)
            {
                othersLayer->m_data.at<uchar>(y, x) = linesLayer->m_data.at<uchar>(y, x);
                linesLayer->m_data.at<uchar>(y, x) = 0;
            }
        }
    }


    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
// Распознает текст на входных изображениях. В будущем будет осуществляться фильтрация результатов 
// по вероятности правильного распознавания (параметр minConfidences)
SDKResult WLayer::RecognizeText(std::vector<int> idxs, const float minConfidences)
{
    if (m_type != WLayer::LAYER_TYPE_ENUM::LT_TEXT)
        return kSDKResult_Error;

    cv::Ptr<cv::text::OCRTesseract> ocr = cv::text::OCRTesseract::create();
    std::vector<std::vector<std::string>> result;

    for (int i = 0; i < idxs.size(); i++)
    {
        if (idxs[i] < m_objects.size())
        {
			auto text = std::dynamic_pointer_cast<WText>(m_objects[idxs[i]]);
            cv::Mat rotatedTextImg;
			float angle = text->RotateToHorizon(this, rotatedTextImg);

			if (angle > 360)
				continue;
            text->setAngle(angle);

            std::string output;
            std::vector<cv::Rect>   boxes;
            std::vector<std::string> words;
            std::vector<float>  confidences;
            rotatedTextImg = 255 - rotatedTextImg;
            cv::cvtColor(rotatedTextImg, rotatedTextImg, CV_GRAY2BGR);
            ocr->run(rotatedTextImg, output, &boxes, &words, &confidences, cv::text::OCR_LEVEL_WORD);
            //output.erase(remove(output.begin(), output.end(), '\n'), output.end());
            //std::string textString;
            //for (size_t j = 0; j < words.size(); j++)
            //    textString += (words[j] + " ");
            //textString.pop_back();
			if (!output.empty()) 
			{
            text->AddText(output);
            text->SetState(true);
			}
        }
    }
}
// ------------------------------------------------------------
void WLayer::AddVectorElement(std::shared_ptr<WVectorObject> element)
{
    m_objects.push_back(element);
}
// ------------------------------------------------------------
std::shared_ptr<WVectorObject> WLayer::GetObject(uint i)
{
	if(i < m_objects.size())
		return m_objects[i];
	return nullptr;
}
// ------------------------------------------------------------
std::shared_ptr<WVectorObject> WLayer::GetVectorElement(size_t idx)
{
    if (idx < m_objects.size())
        return m_objects[idx];
    return NULL;
}
// ------------------------------------------------------------
void WLayer::RemoveVectorElement(size_t idx)
{
    if (idx < m_objects.size())
    {
        m_objects.erase(m_objects.begin() + idx);
    }
}
// ------------------------------------------------------------
void WLayer::RemoveVectorElements(std::vector<size_t> idxs)
{
    std::sort(idxs.rbegin(), idxs.rend());
    for (size_t i = 0; i < idxs.size(); i++)
        RemoveVectorElement(idxs[i]);
}
// ------------------------------------------------------------

SDK_END_NAMESPACE
