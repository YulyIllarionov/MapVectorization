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
static const SDKResult kSDKResult_Succeeded     = 0x00000000;
//
static const SDKResult kSDKResult_Failed        = 0x00000001;
//
static const SDKResult kSDKResult_Error         = 0x00000002;
//
static const SDKResult kSDKResult_NotFound      = 0x00000004;
//
static const SDKResult kSDKResult_InternalError = 0x00000008;
//
static const SDKResult kSDKResult_NullPointer   = 0x00000010;

inline bool S_Suc(const SDKResult& result) { return result == kSDKResult_Succeeded; }
inline bool S_Err(const SDKResult& result) { return (result & 0xFFFFFFFF) != 0; }
inline bool S_Ok(const SDKResult& result) { return !S_Err(result); }




SDK_END_NAMESPACE

#endif // SDK_RESULT_H_