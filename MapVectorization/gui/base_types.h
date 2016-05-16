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
template <typename T> class IEnumItem {
public:
  // return true - next item
  // return false - end of enum
  virtual bool Next(T* item) const = 0;

  // reset the enumerator
  virtual void Reset() const = 0;

protected:
  virtual ~IEnumItem() {}
};

typedef std::vector<cv::Mat> LayersContainer;
// ------------------------------------------------------------
class WRaster //: public IEnumItem<cv::Mat>
{
public:
  WRaster(std::string img_path);

  virtual ~WRaster(){}

public:

  //bool NextLayer(cv::Mat* layer) const { return Next(layer); }
  //void ResetLayerEnum() const { Reset(); }

private:

  //virtual bool Next(cv::Mat* layer) const;

  // reset the enumerator
  //virtual void Reset() const;

  void Initialize(std::string img_path);

public:

  cv::Mat         m_raster;

private:

//LayersContainer::const_iterator   m_layer_cit;
  LayersContainer                   m_layers;
  std::wstring                      m_image_path;
};
// ------------------------------------------------------------


SDK_END_NAMESPACE

#endif // _SDK_BASE_TYPES_H_
