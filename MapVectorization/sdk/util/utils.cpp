/// Whiteteam
/// \file utils.cpp
/// \brief This file contains utils
/// \author Whiteteam authors

#include "stdafx.h"

#include <iostream>

#include "utils.h"

SDK_BEGIN_NAMESPACE

using namespace utils;

std::string WSTR2STR(const std::wstring& wide)
{
    //const wchar_t *orig = const_cast<wchar_t>(wide.c_str());
    
    // Convert the wchar_t string to a char* string. Record 
    //.the length of the original string and add 1 to it to
    //.account for the terminating null character.
    size_t origsize = wide.size() + 1;
    size_t convertedChars = 0;
    // Allocate two bytes in the multibyte output string for every wide
    // character in the input string (including a wide character
    // null). Because a multibyte character can be one or two bytes,
    // you should allot two bytes for each character. Having extra
    // space for the new string is not an error, but having
    // insufficient space is a potential security problem.
    const size_t newsize = origsize * 2;
    // The new string will contain a converted copy of the original
    // string plus the type of string appended to it.
    char *nstring = new char[newsize];
    // Put a copy of the converted string into nstring
    wcstombs_s(&convertedChars, nstring, newsize, wide.c_str(), _TRUNCATE);

    return std::string(nstring);
} 


SDK_END_NAMESPACE