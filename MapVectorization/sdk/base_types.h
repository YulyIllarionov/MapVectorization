/// Whiteteam
/// \file base_types.h
/// \brief This file contains base types
/// \author Whiteteam authors

#ifndef _SDK_BASE_TYPES_H_
#define _SDK_BASE_TYPES_H_
#pragma once

#include <vector>

#include "app/sdk_const.h"
#include "app/sdk_result.h"

#include "opencv/cxcore.h"
#include <list>

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
  inline bool contains(const cv::Vec3b& color);
  w_color getLow();
  w_color getHigh();

private:
  cv::Vec3b low;
  cv::Vec3b high;
};

// ------------------------------------------------------------
typedef std::string             LayerUUID;
typedef std::vector<LayerUUID>  LayerIDs;
// ------------------------------------------------------------
struct WLayer
{
  typedef uint LAYER_TYPE;
  enum/* class*/ LAYER_TYPE_ENUM //: uint
  {
    LT_NONE = 0x0,
    LT_TEXT = 0x1,
    LT_LINES = 0x2,
    LT_AREAS = 0x4,
    LT_OTHER = 0x8,
    // = 0x10  
  };
  friend class WRaster;

public:

  friend bool operator==(const WLayer& lhs, const WLayer& rhs)
  {
    return lhs.m_uuid == rhs.m_uuid;
  }

  friend bool operator!=(const WLayer& lhs, const WLayer& rhs)
  {
    return !(lhs == rhs);
  }

  LayerUUID   getID()    const { return m_uuid; }
  LAYER_TYPE  getType()  const { return m_type; }
  w_range     getRange() const { return m_color_range; }
  std::string getName()  const { return m_name; }

  cv::Mat     m_data;

private:
  LayerUUID   m_uuid;
  LAYER_TYPE  m_type;
  w_range     m_color_range;
  std::string m_name;
};
// ------------------------------------------------------------
typedef std::list<WLayer>       LayersContainer;
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
class WRaster //: public IEnumItem<cv::Mat>
{
public:
  WRaster(const std::string& imgPath = "");

  virtual ~WRaster(){}

  SDKResult IncreaseSharpness(double k) const;
  std::vector<cv::Rect> DetectLetters(const LayerUUID& layerId) const;

  //{ layer
  // create and add new layer
  WLayer*   AddLayer();      
  // create and add new layer
  SDKResult RemoveLayer     (const LayerUUID& layerId);
  // add layer color
  SDKResult AddColorToLayer (const LayerUUID& layerId, const w_color& color) const;
  // add, set, remove layer type
  SDKResult AddLayerType    (const LayerUUID& layerId, WLayer::LAYER_TYPE type) const;
  SDKResult SetLayerType    (const LayerUUID& layerId, WLayer::LAYER_TYPE type) const;
  SDKResult RemoveLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type) const;
  // set layer name
  SDKResult SetLayerName    (const LayerUUID& layerId, const std::string& name) const;
  // return layer's ref
  WLayer*   GetLayerById    (const LayerUUID& layerId) const;
  WLayer*   GetLayerByName  (const std::string& name) const;
  // get layers idxs by type
  SDKResult GetLayersByType (WLayer::LAYER_TYPE type, LayerIDs& layerIds) const;
  // split layer by function
  SDKResult SplitLayer      (const LayerUUID& layerId, LayerIDs& splittedLayers);
  //}

public:


private:
  SDKResult SetLayerType (const LayerUUID& layerId, WLayer::LAYER_TYPE type, bool overwrite) const;
  void Initialize(const std::string& imgPath);

  // depricate copy and move operations
  WRaster(const WRaster& other)
    /*: m_raster{other.m_raster},
      m_layers{other.m_layers},
      m_image_path{other.m_image_path}*/
  {
  }

  WRaster(WRaster&& other)
    /*: m_raster{std::move(other.m_raster)},
      m_layers{std::move(other.m_layers)},
      m_image_path{std::move(other.m_image_path)}*/
  {
  }

  //WRaster& operator=(WRaster other)
  //{
  //  using std::swap;
  //  swap(*this, other);
  //  return *this;
  //}

  cv::Mat           m_raster;
  LayersContainer   m_layers;

private:

  std::wstring                      m_image_path;
};
// ------------------------------------------------------------


SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
