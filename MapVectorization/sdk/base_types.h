/// Whiteteam
/// \file base_types.h
/// \brief This file contains base types
/// \author Whiteteam authors

#ifndef _SDK_BASE_TYPES_H_
#define _SDK_BASE_TYPES_H_
#pragma once

#include <vector>

#include "app/sdk_const.h"

#include "opencv/cxcore.h"

SDK_BEGIN_NAMESPACE


//  Enumerator
class IEnumItem {
public:
  // return true - next item
  // return false - end of enum
  virtual bool Next(IEnumItem* item) const = 0;

  // reset the enumerator
  virtual void Reset() const = 0;

protected:
  virtual ~IEnumItem() {}
};
// ------------------------------------------------------------
struct w_color;
// ------------------------------------------------------------
struct w_range
{
    w_range();
    void addColor(const w_color& color);
    inline bool ñontains(const cv::Vec3b& color);
    w_color getLow();
    w_color getHigh();

private:
    cv::Vec3b low;
    cv::Vec3b high;
};
// ------------------------------------------------------------
struct w_color
{
    //friend w_range;
    w_color(uchar r, uchar g, uchar b);
    w_color(cv::Vec3b color);
    w_color(cv::Vec4b color);
    cv::Vec3b toVec3b() const;
    friend inline bool operator <= (const w_color &first, const cv::Vec3b &second);
    friend inline bool operator >= (const w_color &first, const cv::Vec3b &second);
    friend void w_range::addColor(const w_color& color);

    uchar r;
    uchar g;
    uchar b;
};
// ------------------------------------------------------------
struct WLayer
{
    enum class LAYER_TYPE
    {
        LT_TEXT_AND_LINES = 0,
        LT_AREAS,
        LT_OTHER
    };

    LAYER_TYPE  m_type;
    cv::Mat     m_data;
    w_range     m_color_range;
    std::string m_name;

};
// ------------------------------------------------------------
typedef std::vector<WLayer> LayersContainer;
// ------------------------------------------------------------
class WRaster //: public IEnumItem<cv::Mat>
{
public:
  WRaster(std::string img_path = "");

  virtual ~WRaster(){}

  void IncreaseSharpness(double k);

  void AddLayer();

  int AddColorToLayer(int layerNumber, const w_color& color);
  
  int SetLayerType(int layerNumber, WLayer::LAYER_TYPE type);

  int SetLayerName(int layerNumber, std::string name);

  std::vector<cv::Rect> detectLetters(int layerNumber);

public:

  //bool NextLayer(cv::Mat* layer) const { return Next(layer); }
  //void ResetLayerEnum() const { Reset(); }

private:

  //virtual bool Next(cv::Mat* layer) const;

  // reset the enumerator
  //virtual void Reset() const;

  void Initialize(std::string img_path);

public:
  
  cv::Mat           m_raster;
  LayersContainer   m_layers;

private:

//LayersContainer::const_iterator   m_layer_cit;
  std::wstring                      m_image_path;
};
// ------------------------------------------------------------


SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
