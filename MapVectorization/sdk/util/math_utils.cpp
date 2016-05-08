/// Whiteteam
/// \file math_utils.cpp
/// \brief This file contains math utils
/// \author Whiteteam authors

#include "stdafx.h"
#include "math_utils.h"
 
SDK_BEGIN_NAMESPACE

template<typename T> T MathUtils::Max(T val1, T val2)
{
  return val1 > val2 ? val1 : val2;
}

template<typename T> T MathUtils::Min(T val1, T val2)
{
  return val1 < val2 ? val1 : val2;
}

SDK_END_NAMESPACE
