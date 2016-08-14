/// Whiteteam
/// \file base_types.cpp
/// \brief This file contains base types
/// \author Whiteteam authors

#include "stdafx.h"
#include <fstream>  
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/text.hpp"

#include "util/utils.h"
#include "util/math_utils.h"
#include "graphics/skeletonization.h"
#include <stack>
#include <numeric>

#include <math.h>
#include <algorithm>

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

    WObjectContainer lines = SDK_NAMESPACE::utils::FindTextOnMat(m_data);
    std::vector<int> idxs(lines.size());
    for (size_t i = 0; i < idxs.size(); i++)
    {
        idxs[i] = m_objects.end() - m_objects.begin() + i;
    }
    m_objects.insert(m_objects.end(), lines.begin(), lines.end());
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        WText* currentText = dynamic_cast<WText*>(&m_objects[i]);
        this->RecognizeText(idxs, 0.0);
    }
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
    // Read the file
    Mat raster = imread(String(imgPath), CV_LOAD_IMAGE_COLOR);
    cvtColor(raster, m_raster, CV_BGR2BGRA, 4);
}
// ------------------------------------------------------------
SDKResult WRaster::IncreaseSharpness(double k) const
{
    //filter2D(m_raster, m_raster, m_raster.depth(), utils::SharpKernel(k));

    Mat kernel = utils::SharpKernel(k);
    Mat img;
    cvtColor(m_raster, img, CV_BGRA2BGR);
    Mat imgOut(img);
    Mat imgGray;
    Mat imgEdges;

    cvtColor(m_raster, imgGray, COLOR_RGB2GRAY);
    Canny(imgGray, imgEdges, 90, 200);

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
    layer.m_uuid = utils::genUUID();
    layer.m_group_id = groupId.empty() ? utils::genUUID() : groupId;
    m_layers.push_back(layer);

    return &m_layers.back();
}
// ------------------------------------------------------------
SDKResult WRaster::RemoveLayer(const LayerUUID& layerId)
{
    LayersContainer::const_iterator cit = m_layers.begin();
    for (; cit != m_layers.end(); ++cit)
    {
        if (cit->m_uuid == layerId)
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

    layer->m_color_range.addColor(color);

    for (int y = 0; y < m_raster.rows; y++)
    {
        for (int x = 0; x < m_raster.cols; x++)
        {
            const Vec4b currentColorOld = m_raster.at<Vec4b>(y, x);
            const Vec3b currentColor(currentColorOld[0], currentColorOld[1], currentColorOld[2]);
            layer->m_data.at<uchar>(y, x) = layer->m_color_range.contains(currentColor) ? 1 : 0;
        }
    }

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::AddLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type)
{
    return SetLayerType(layerId, type, false);
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type)
{
    return SetLayerType(layerId, type, true);
}
// ------------------------------------------------------------
SDKResult WRaster::RemoveLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    layer->m_type &= ~type;

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerType(const LayerUUID& layerId, WLayer::LAYER_TYPE type, bool overwrite)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    if (overwrite)
        layer->m_type = type;
    else
        layer->m_type |= type;

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SetLayerName(const LayerUUID& layerId, const std::string& name)
{
    WLayer* layer = GetLayerById(layerId);
    if (layer == nullptr)
        return kSDKResult_NotFound;

    layer->m_name = name;
    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
WLayer* WRaster::GetLayerById(const LayerUUID& layerId)
{
    for (LayersContainer::iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
    {
        if (cit->m_uuid == layerId)
            return &(*cit);
    }
    return nullptr;
}
// ------------------------------------------------------------
WLayer* WRaster::GetLayerByName(const std::string& name)
{
    for (LayersContainer::iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
    {
        if (cit->m_name == name)
            return &(*cit);
    }
    return nullptr;
}
// ------------------------------------------------------------
SDKResult WRaster::GetLayersByType(WLayer::LAYER_TYPE type, LayerIDs& layer_ids) const
{
    layer_ids.clear();
    for (LayersContainer::const_iterator cit = m_layers.begin(); cit != m_layers.end(); ++cit)
    {
        if ((cit->m_type & type) != 0)
            layer_ids.push_back(cit->m_uuid);
    }
    return kSDKResult_Succeeded;
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

    switch (layer->m_type)
    {
    case WLayer::LAYER_TYPE_ENUM::LT_NONE:
        break;
    case WLayer::LAYER_TYPE_ENUM::LT_LINES | WLayer::LAYER_TYPE_ENUM::LT_TEXT /*| WLayer::LAYER_TYPE_ENUM::LT_OTHER*/:
    {
        WLayer* linesLayer = this->AddLayer(layer->getGroupId());
        this->SetLayerType(linesLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_LINES);
        this->SetLayerName(linesLayer->getID(), std::string("Lines from ") + layer->getName());
        WLayer* othersLayer = this->AddLayer(layer->getGroupId());
        this->SetLayerType(othersLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_TEXT);
        //this->SetLayerType(othersLayer->getID(), WLayer::LAYER_TYPE_ENUM::LT_OTHER);
        this->SetLayerName(othersLayer->getID(), std::string("Text from ") + layer->getName());
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
            RemoveLayer(layer->m_uuid);
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
        if (cit->m_group_id == groupId)
            relatedLayers.push_back(cit->m_uuid);
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

    for (size_t i = 0; i < layer->m_objects.size(); i++)
    {
        double currentDistance = layer->m_objects[i].DistanceTo(pointCV);
        if (currentDistance < distance)
        {
            distance = currentDistance;
            index = i;
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

    int idx = 0;
    WObjectContainer::const_iterator cit = layer->m_objects.begin();
    for (; cit != layer->m_objects.end(); ++cit, ++idx)
    {
        if (mapPoints.Contains(*cit))
            ids.push_back(idx);
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
                linesImg.at<uchar>(objectPoints[i] - roi.tl()) = 255;
            }
            vectorObjects = SDK_NAMESPACE::utils::FindLinesOnMat(linesImg);
            for (size_t j = 0; j < vectorObjects.size(); j++)
                for (size_t k = 0; k < vectorObjects[i].m_points.size(); k++)
                    vectorObjects[i].m_points[k] += roi.tl();

            break;
        }
        case WLayer::LT_TEXT:
        {
            Point2f rectPoints[4];
            cv::RotatedRect lineRect = cv::minAreaRect(objectPoints);
            lineRect.points(rectPoints);
            std::vector<Point> polygonPoints(4);
            for (size_t i = 0; i < 4; i++)
                polygonPoints[i] = rectPoints[i];
            for (size_t i = 0; i < polygonPoints.size(); i++)
            {
                polygonPoints[i].x = std::max(polygonPoints[i].x, 0);
                polygonPoints[i].x = std::min(polygonPoints[i].x, layer->m_data.cols);
                polygonPoints[i].y = std::max(polygonPoints[i].x, 0);
                polygonPoints[i].y = std::min(polygonPoints[i].x, layer->m_data.rows);
            }
            vectorObjects.push_back(WText(polygonPoints));

            break;
        }
        default:
            break;
        }
        layer->m_objects.insert(layer->m_objects.end(), vectorObjects.begin(), vectorObjects.end());
    }
    //Распознавание текста при необходимости
    if (layer->getType() == WLayer::LT_TEXT)
    {
        std::vector<int> textIdxs(rasterObjects.size());
        std::iota(textIdxs.begin(), textIdxs.end(), layer->m_objects.size() - 1 - rasterObjects.size());
        layer->RecognizeText(textIdxs, 0.0);
    }
    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
std::vector<std::vector<Wregion>> WRaster::CutObjectsFromLayer(const LayerUUID& layerId, std::vector<int> idxs)
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
            if (idxs[i] < layer->m_objects.size())
            {
                //cut from raster
                WLine* text = dynamic_cast<WLine*>(&layer->m_objects[idxs[i]]);
                regions.push_back(text->CutFromLayer(layer));
                //delete from vector
                layer->m_objects.erase(layer->m_objects.begin() + idxs[i]);
            }
        }
    }
    case WLayer::LAYER_TYPE_ENUM::LT_TEXT:
    {
        for (size_t i = 0; i < idxs.size(); i++)
        {
            if (idxs[i] < layer->m_objects.size())
            {
                //cut from raster
                WText* text = dynamic_cast<WText*>(&layer->m_objects[idxs[i]]);
                regions.push_back(text->CutFromLayer(layer));
                //delete from vector
                layer->m_objects.erase(layer->m_objects.begin() + idxs[i]);
            }
        }
    }

    }
    return regions;
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
    switch (*result)
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

    Point2f rectPoints[4];
    cv::RotatedRect lineRect = cv::minAreaRect(m_points);
    lineRect.points(rectPoints);
    std::vector<Point> polygonPoints(4);
    for (size_t i = 0; i < 4; i++)
        polygonPoints[i] = rectPoints[i];

    for (size_t i = 0; i < polygonPoints.size(); i++)
    {
        polygonPoints[i].x = std::max(polygonPoints[i].x, 0);
        polygonPoints[i].x = std::min(polygonPoints[i].x, layer->m_data.cols);
        polygonPoints[i].y = std::max(polygonPoints[i].x, 0);
        polygonPoints[i].y = std::min(polygonPoints[i].x, layer->m_data.rows);
    }
    WPolygon linePolygon(polygonPoints);

    Rect roi = boundingRect(linePolygon.m_points);
    std::vector<Wregion> letters;

    for (size_t i = 0; i < m_points.size(); i++)
    {
        if (linePolygon.Contains(m_points[i]))
        {
            if (layer->m_data.at<uchar>(m_points[i]) != 0)
            {
                Wregion currentRegion(m_points[i], layer->m_data);
                if (!currentRegion.IsEmpty())
                    letters.push_back(currentRegion);
            }
        }
    }
    return letters;
}
// ------------------------------------------------------------
cv::Mat WText::RotateToHorizon(WLayer* layer)
{
    if (layer->getType() != WLayer::LAYER_TYPE_ENUM::LT_TEXT)
        return cv::Mat();

    //Копирование полигона на отдельное изображение 
    Rect roi = boundingRect(m_points);
    Mat img2Recognition(roi.size(), CV_8UC1, Scalar(0));
    for (int y = roi.y; y < roi.y + roi.height; y++)
    {
        for (int x = roi.x; x < roi.x + roi.width; x++)
        {
            Point current(x, y);
            if (this->Contains(current))
            {
                img2Recognition.at<uchar>(y - roi.y, x - roi.x) = layer->m_data.at<uchar>(current);
            }
        }
    }

    //Нахождение угла поворота
    std::vector<cv::Vec4i> textLines;
    HoughLinesP(img2Recognition, textLines, 1, CV_PI / 180, 80, img2Recognition.cols / 4, img2Recognition.cols / 10);
    std::vector<double> lineAngles(textLines.size());
    for (size_t i = 0; i < textLines.size(); i++)
    {
        lineAngles[i] = std::atan2(textLines[i][3] - textLines[i][1], textLines[i][2] - textLines[i][0]);
    }
    std::sort(lineAngles.begin(), lineAngles.end());
    //double angle = 0.0;
    //if (std::abs(lineAngles.back() - lineAngles.front()) < (std::M_PI_2))
    double    angle = lineAngles[lineAngles.size() / 2] * 180 / 3.141592; //TODO заменить на пи

    //Поворот
    cv::Point2f center(img2Recognition.cols / 2.0, img2Recognition.rows / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::Rect bbox = cv::RotatedRect(center, img2Recognition.size(), angle).boundingRect();
    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
    cv::warpAffine(img2Recognition, img2Recognition, rot, bbox.size());

    return img2Recognition;
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
bool Wregion::IsLine()
{
    float radius;
    minEnclosingCircle(points, Point2f(), radius);
    double ratio = (double)this->Square() / radius / radius;
    //std::fstream fs;
    //fs.open("./testRatio.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    //fs << ratio << std::endl;
    //fs.close();
    return (ratio < 0.6);
}
// ------------------------------------------------------------
void Wregion::drawOn(Mat& img, uchar color)
{
    for (int i = 0; i < points.size(); i++)
    {
        //if ((points[i].x<img.cols)&& (points[i].y<img.rows))
        img.at<uchar>(points.at(i)) = color;
    }
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

    Mat temp = layer->m_data.clone();
    for (int y = 1; y < temp.rows - 1; y++)
    {
        for (int x = 1; x < temp.cols - 1; x++)
        {
            if (temp.at<uchar>(y, x)>0)
            {
                Wregion region(Point(x, y), temp);
                if (region.Square() > 4)
                {
                    if (region.IsLine())
                        region.drawOn(linesLayer->m_data, 255);
                    else
                        region.drawOn(othersLayer->m_data, 255);
                }
            }
        }
    }

    return kSDKResult_Succeeded;
}
// ------------------------------------------------------------
SDKResult WRaster::SplitText(const LayerUUID& layerId, const LayerUUID& textLayerID, const LayerUUID& othersLayerID)
{
    SDKResult result = kSDKResult_Succeeded;

    return result;
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
            WText* text = dynamic_cast<WText*>(&m_objects[idxs[i]]);
            cv::Mat rotatedTextImg = text->RotateToHorizon(this);

            std::string output;
            std::vector<cv::Rect>   boxes;
            std::vector<std::string> words;
            std::vector<float>  confidences;
            ocr->run(rotatedTextImg, output, &boxes, &words, &confidences, cv::text::OCR_LEVEL_WORD);

            //output.erase(remove(output.begin(), output.end(), '\n'), output.end());

            text->AddText(output);
            text->SetState(true);

        }
    }
}

SDK_END_NAMESPACE
