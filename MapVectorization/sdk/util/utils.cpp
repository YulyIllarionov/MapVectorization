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
#include "../graphics/skeletonization.h"

SDK_BEGIN_NAMESPACE

namespace utils {
    // ----------------------------------------------------
    std::string WSTR2STR(const std::wstring& wide)
    {
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

        delete[] nstring;

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
    void SetTransparent(cv::Mat& img, cv::Mat& mask, uchar alphaTrue, uchar alphaFalse, bool needTrue, bool needFalse)
    {
        for (int y = 0; y < img.rows; y++)
            for (int x = 0; x < img.cols; x++)
                if (mask.at<uchar>(y, x) > 0)
                {
                    if (needTrue)
                        img.at<cv::Vec4b>(y, x)[3] = alphaTrue;
                    
                }
                else
                {
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
    std::vector<cv::Point> getNeghboursClockwise(cv::Point point, const cv::Mat& image)
    {
        std::vector<cv::Point> neighbors;
        uchar color = image.at<uchar>(point);
        point.y--;
        if (image.at<uchar>(point) == color)
                neighbors.push_back(point);
        point.x++;
        if (image.at<uchar>(point) == color)
            neighbors.push_back(point);
        point.y++;
        if (image.at<uchar>(point) == color)
            neighbors.push_back(point);
        point.y++;
        if (image.at<uchar>(point) == color)
            neighbors.push_back(point);
        point.x--;
        if (image.at<uchar>(point) == color)
            neighbors.push_back(point);
        point.x--;
        if (image.at<uchar>(point) == color)
            neighbors.push_back(point);
        point.y--;
        if (image.at<uchar>(point) == color)
            neighbors.push_back(point);
        point.y--;
        if (image.at<uchar>(point) == color)
            neighbors.push_back(point);

        return neighbors;
    }
    // ----------------------------------------------------
    int squaredDistanceBetween(const cv::Point& a, const cv::Point& b)
    {
        return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
    }
    // ----------------------------------------------------
    WObjectContainer FindLinesOnMat(const cv::Mat & img)
    {
        WObjectContainer linesContainer;
        cv::Mat skeleton;
        SDK_NAMESPACE::WSkeletonizer::Instance().Skeletonize(img, skeleton);
        cv::imwrite("skeleton.png", skeleton);
        for (int y = 1; y < skeleton.rows - 1; y++)
        {
            for (int x = 1; x < skeleton.cols - 1; x++)
            {
                cv::Point initial(x, y);
                if (skeleton.at<uchar>(initial) > 0)
                {
                    std::vector<cv::Point> firstNeighbors = SDK_NAMESPACE::utils::getNeghboursClockwise(initial, skeleton);
                    if (firstNeighbors.size() > 2)
                        continue;
                    else
                    {
                        std::vector<WPointsContainer> lines(firstNeighbors.size());
                        lines[0].push_back(initial);
                        skeleton.at<uchar>(initial) = 0;
                        for (int i = 0; i < firstNeighbors.size(); i++)
                        {
                            cv::Point current(firstNeighbors[i]);
                            std::vector<cv::Point> neighbors;
                            while (true)
                            {
                                lines[i].push_back(current);
                                neighbors = SDK_NAMESPACE::utils::getNeghboursClockwise(current, skeleton);
                                skeleton.at<uchar>(current) = 0;
                                if (neighbors.size() != 1)
                                    break;
                                current = neighbors[0];
                            }
                        }
                        WLine line(lines[0]);
                        for (int i = 1; i < lines.size(); i++)
                        {
                            line.Concat(lines[i]);
                        }
                        linesContainer.push_back(line);
                    }
                }
            }
        }
        return linesContainer;
    }

}
SDK_END_NAMESPACE