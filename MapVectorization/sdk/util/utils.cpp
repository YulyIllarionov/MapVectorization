/// Whiteteam
/// \file utils.cpp
/// \brief This file contains utils
/// \author Whiteteam authors

#include "stdafx.h"

extern "C"
{
#ifdef WIN32
#include <Rpc.h>
#else
#include <uuid/uuid.h>
#endif
}

#include <iostream>

#include "utils.h"

SDK_BEGIN_NAMESPACE

namespace utils {
    // ----------------------------------------------------
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
    // ----------------------------------------------------
    // Inline Функция:
    //       1. именование: WsharpKernel(float k) функция возвращающая ядро свертки для увеличения резкости 
    //       2. аргументы функции:  k - коэффицент ядра
    // ----------------------------------------------------
    cv::Mat SharpKernel(double k)
    {
        cv::Mat kernel(3, 3, CV_64F, -0.125);
        kernel *= k;
        kernel.at<double>(1, 1) = k + 1;
        return kernel;
    }
    // ----------------------------------------------------
    void SetTransparent(cv::Mat& img, cv::Mat& mask, bool needTrue, uchar alphaTrue, bool needFalse, uchar alphaFalse)
    {
        for (int y = 0; y < img.rows; y++)
            for (int x = 0; x < img.cols; x++)
                if (mask.at<uchar>(y, x) > 0)
                {
                    if (needTrue)
                        img.at<cv::Vec4b>(y, x)[3] = alphaTrue;
                    if (needFalse)
                        img.at<cv::Vec4b>(y, x)[3] = alphaFalse;
                }
                    
    } 
    // ----------------------------------------------------
    std::string genUUID()
    {
    #ifdef WIN32
      UUID uuid;
      UuidCreate ( &uuid );

      unsigned char * str;
      UuidToStringA ( &uuid, &str );

      std::string s( ( char* ) str );

      RpcStringFreeA ( &str );
    #else
      uuid_t uuid;
      uuid_generate_random ( uuid );
      char s[37];
      uuid_unparse ( uuid, s );
    #endif
      return s;
    }
    // ----------------------------------------------------
    inline bool operator < (cv::Vec3b& first, cv::Vec3b& second)
    {
        if (first[0] < second[0])
            if (first[1] < second[1])
                if (first[1] < second[1])
                    return true;
        return false;
    }
    // ----------------------------------------------------
    inline bool operator > (cv::Vec3b& first, cv::Vec3b& second)
    {
        return (second < first);
    }
    // ----------------------------------------------------
    inline bool operator <= (cv::Vec3b& first, cv::Vec3b& second)
    {
        return !(second < first);
    }
    // ----------------------------------------------------
    inline bool operator >= (cv::Vec3b& first, cv::Vec3b& second)
    {
        return !(first < second);
    }
    // ----------------------------------------------------

}
SDK_END_NAMESPACE