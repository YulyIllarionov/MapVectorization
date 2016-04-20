/// Whiteteam
/// \file math_utils.h
/// \brief This file contains math utils
/// \author Whiteteam authors


#ifndef _SDK_MATH_UTILS_H_
#define _SDK_MATH_UTILS_H_
#pragma once

#include "..\app\sdk_const.h"

SDK_BEGIN_NAMESPACE

class MathUtils
{
public:
  // return maximum of two values
  template<typename T> static T Max(T val1, T val2);
  // return minimum of two values
  template<typename T> static T Min(T val1, T val2);

};

SDK_END_NAMESPACE


#endif // _SDK_MATH_UTILS_H_
