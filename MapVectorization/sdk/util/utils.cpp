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

#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/text.hpp"

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
    bool isEdgePoint(cv::Point point, const cv::Mat& image)
    {
        if ((point.x == 0) || (point.x == (image.cols - 1)) ||
            (point.y == 0) || (point.y == (image.rows - 1)))
            return true;
        return false;
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
                    else if (firstNeighbors.size() != 0)
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

	WObjectContainer FindTextOnMat(const cv::Mat& img)
	{
		WObjectContainer textContainer;

		// Create ERFilter objects with the 1st and 2nd stage default classifiers
		cv::Ptr<cv::text::ERFilter> er_filter1 = cv::text::createERFilterNM1(cv::text::loadClassifierNM1("trained_classifierNM1.xml"),8,0.000015f,0.23f,0.1f,true,0.1f);
		cv::Ptr<cv::text::ERFilter> er_filter2 = cv::text::createERFilterNM2(cv::text::loadClassifierNM2("trained_classifierNM2.xml"),0.1f);

		std::vector<cv::text::ERStat> regions;
		// Apply the default cascade classifier to each independent channel (could be done in parallel)
		er_filter1->run(img, regions);
		er_filter2->run(img, regions);
		
		std::vector<std::vector<cv::Point>> points;
		detectRegions(img, er_filter1, er_filter2, points);	
	
		cv::Mat out_img_decomposition= cv::Mat::zeros(img.rows + 2, img.cols + 2, CV_8UC1);
		std::vector<cv::Vec2i> tmp_group;

		// Detect character groups
		std::vector<cv::Rect> nm_boxes;
		cv::text::erGrouping(img, img, points, nm_boxes, cv::text::ERGROUPING_ORIENTATION_HORIZ);

		for (int i = 0; i < nm_boxes.size(); i++)
		{
			//TODO
			//std::vector<cv::Point> polygon {nm_boxes[i].tl(), nm_boxes[i].br(), 
			//	nm_boxes[i].tl() + cv::Point(nm_boxes[i].width,0), nm_boxes[i].tl() + cv::Point(0, nm_boxes[i].height)};
			//polygon.push_back(nm_boxes[i].tl());
			//polygon.push_back(nm_boxes[i].br());

			
			//WText item(polygon);
			//textContainer.push_back(item);
		}

		return textContainer;
	}
}
SDK_END_NAMESPACE