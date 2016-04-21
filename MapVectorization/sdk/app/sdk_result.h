/// Whiteteam
/// \file app_const.h
/// \brief This file contains sdk result
/// \author Whiteteam authors

#ifndef SDK_RESULT_H_
#define SDK_RESULT_H_
#pragma once
 
#include "sdk_const.h"


SDK_BEGIN_NAMESPACE


// for return result
typedef __int32 SDKResult;

//
static const SDKResult kSDKResult_Succeeded     = 0;
//
static const SDKResult kSDKResult_Failed        = 1;
//
static const SDKResult kSDKResult_Error         = 2;
//
static const SDKResult kSDKResult_NotFound      = 3;
//
static const SDKResult kSDKResult_InternalError = 4;
//
static const SDKResult kSDKResult_NullPointer   = 5;

inline bool S_Suc(const SDKResult& result) { return result == kSDKResult_Succeeded; }
inline bool S_ERR(const SDKResult& result) { return (result & 0x11111111) != 0; }
inline bool S_OK(const SDKResult& result) { return !S_ERR(result); }




SDK_END_NAMESPACE

#endif // SDK_RESULT_H_