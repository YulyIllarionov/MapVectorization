/// Whiteteam
/// \file utils.h
/// \brief This file contains utils
/// \author Whiteteam authors

#ifndef _SDK_UTILS_H_
#define _SDK_UTILS_H_
#pragma once
 
#include <string>

#include "..\app\sdk_const.h"

SDK_BEGIN_NAMESPACE

namespace utils
{
  
  std::string WSTR2STR(const std::wstring& wide);

}

SDK_END_NAMESPACE

#endif // _SDK_UTILS_H_
