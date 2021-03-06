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
    //��������� ��� ��������� RGB ������
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
		if (point.y >= 0)
		{
			if (image.at<uchar>(point) == color)
					neighbors.push_back(point);
			point.x++;

			if (point.x < image.cols)
			{
				if (image.at<uchar>(point) == color)
					neighbors.push_back(point);
			}
		}
		else 
			point.x++;

        point.y++;
		if (point.x < image.cols)
		{
			if (image.at<uchar>(point) == color)
				neighbors.push_back(point);
			point.y++;

			if (point.y < image.rows)
			{
				if (image.at<uchar>(point) == color)
					neighbors.push_back(point);
			}
		}
		else
			point.y++;

        point.x--;
		if (point.y < image.rows)
		{
			if (image.at<uchar>(point) == color)
				neighbors.push_back(point);
			point.x--;

			if (point.x >= 0)
			{
				if (image.at<uchar>(point) == color)
					neighbors.push_back(point);
			}
		}

		else
			point.x--;

		point.y--;
		if (point.x >= 0)
		{
			if (image.at<uchar>(point) == color)
				neighbors.push_back(point);
			point.y--;

			if (point.y >= 0)
			{
				if (image.at<uchar>(point) == color)
					neighbors.push_back(point);
			}
		}

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
        //������������ �����������
        SDK_NAMESPACE::WSkeletonizer::Instance().Skeletonize(img, skeleton);
        //cv::imwrite("skeleton.png", skeleton);
        for (int y = 0; y < skeleton.rows; y++)
        {
            for (int x = 0; x < skeleton.cols; x++)
            {
                cv::Point initial(x, y);
                //����� ��������� ����� �� �����������
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
                        //����� ����� � ��� ������� �� ��������� �����
                        for (int i = 0; i < firstNeighbors.size(); i++)
                        {
                            cv::Point current(firstNeighbors[i]);
                            std::vector<cv::Point> neighbors;
                            while (true)
                            {
                                lines[i].push_back(current);
                                neighbors = SDK_NAMESPACE::utils::getNeghboursClockwise(current, skeleton);
                                skeleton.at<uchar>(current) = 0;
                                //����������� ������ ���� ��������� �����������
                                if (neighbors.size() != 1)
                                    break;
                                current = neighbors[0];
                            }
                        }
                        //����������� ���� ��������� ������ ����� 
                        auto line = std::make_shared<WLine> (lines[0]);
                        for (int i = 1; i < lines.size(); i++)
                        {
                            line->Concat(lines[i]);
                        }
                        //��������� �����
                        line->SimplifyDP();
                        line->FindWidth(img);
                        linesContainer.push_back(line);
                    }
                }
            }
        }
        return linesContainer;
    }

	void  ErDraw(const cv::Mat &src, cv::Mat &dst, cv::text::ERStat& er)
	{
		if (er.parent != NULL) // deprecate the root region 
		{
			int newMaskVal = 255;
			int flags = 4 + (newMaskVal << 8) + cv::FLOODFILL_FIXED_RANGE + cv::FLOODFILL_MASK_ONLY;
			floodFill(src, dst, cv::Point(er.pixel%src.cols,er.pixel/src.cols),
				cv::Scalar(255), 0, cv::Scalar(er.level), cv::Scalar(0), flags);
		}
	}

	void  ErDraw(const cv::Mat &src, cv::Mat &dst, cv::Rect region)
	{
		src(region).copyTo(dst(region));
	}

	std::vector<cv::Rect> DetectOnlyLetters(const cv::Mat &img)
	{
		std::vector<cv::Rect> boundRect;
		cv::Mat img_reversed, img_sobel, img_threshold, element;
		img.copyTo(img_reversed);
	
		cv::Sobel(img_reversed, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
		element = getStructuringElement(cv::MORPH_RECT, cv::Size(43, 18));
		cv::morphologyEx(img_sobel, img_threshold, CV_MOP_CLOSE, element);;

		std::vector< std::vector< cv::Point> > contours;
		cv::findContours(img_threshold, contours, cv::CHAIN_APPROX_NONE, cv::RETR_LIST);

		std::vector<std::vector<cv::Point> > contours_poly(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			cv::approxPolyDP(cv::Mat(contours.at(i)), contours_poly.at(i), 3, true);
			cv::Rect appRect(boundingRect(cv::Mat(contours_poly.at(i))));
			appRect.x -= appRect.width/10;
			appRect.y -= appRect.height/10;
			appRect.width += appRect.width/5;
			appRect.height += appRect.height/5;
			FitSize(appRect, img.size());
			boundRect.push_back(appRect);		
		}

		return boundRect;
	}

	WObjectContainer FindTextOnMat(const cv::Mat& img)
	{
		WObjectContainer textContainer;

		// Create ERFilter objects with the 1st and 2nd stage default classifiers
		cv::Ptr<cv::text::ERFilter> er_filter1 = cv::text::createERFilterNM1(
            cv::text::loadClassifierNM1("trained_classifierNM1.xml"),8,0.00000015f,0.0023f,0.1f,true,0.1f);
		cv::Ptr<cv::text::ERFilter> er_filter2 = cv::text::createERFilterNM2(
            cv::text::loadClassifierNM2("trained_classifierNM2.xml"),0.1f);

		cv::Mat tmp;
		img.copyTo(tmp);
		tmp = 255 - tmp;
		std::vector<cv::text::ERStat> regions;

		// Apply the default cascade classifier to each independent channel (could be done in parallel)
		er_filter1->run(tmp, regions);
		er_filter2->run(tmp, regions);
		
		//std::vector<std::vector<cv::Point>> points;
		//detectRegions(tmp, er_filter1, er_filter2, points);	
	
		cv::Mat out_img_decomposition= cv::Mat::zeros(tmp.rows + 2, tmp.cols + 2, CV_8UC1);
		for (int i=0; i<(int)regions.size(); i++)
		{
			ErDraw(tmp, out_img_decomposition, regions[i]);
		}
			
		//cv::Mat out_img_decomposition_step_2 = Mat::zeros(image.rows+2, image.cols+2, CV_8UC1);
		//out_img_decomposition.copyTo(out_img_decomposition_step_2);
		
		// Detect character groups
		std::vector<cv::Rect> nm_boxes;
		std::vector<std::vector<cv::Vec2i>> nm_region_groups;
		std::vector<std::vector<cv::text::ERStat>> nm_regions;
		nm_regions.push_back(regions);
		std::vector<cv::Mat> nm_channels;
		nm_channels.push_back(tmp);

		if (regions.size() != 0)
		{
			cv::Mat tmp_rgb;
			cv::cvtColor(tmp, tmp_rgb, cv::COLOR_GRAY2RGB);
			cv::text::erGrouping(tmp_rgb, nm_channels, nm_regions, nm_region_groups, nm_boxes, cv::text::ERGROUPING_ORIENTATION_HORIZ);

			for (int i = 0; i < nm_boxes.size(); i++) 
			{
				cv::Mat tmp = cv::Mat::zeros(nm_boxes[i].height, nm_boxes[i].width, CV_8UC1);
				tmp.copyTo(out_img_decomposition(nm_boxes[i]));
			}
		}
		std::vector<cv::Rect> only_letters = DetectOnlyLetters(out_img_decomposition);

		nm_boxes.insert(nm_boxes.end(), only_letters.begin(), only_letters.end());

		for (int i = 0; i < nm_boxes.size(); i++)
		{
            std::vector<cv::Point> polygon;
			polygon.push_back(nm_boxes[i].tl());
            polygon.push_back(nm_boxes[i].tl() + cv::Point(nm_boxes[i].width, 0));
            polygon.push_back(nm_boxes[i].br());
            polygon.push_back(nm_boxes[i].tl() + cv::Point(0, nm_boxes[i].height));

			auto item = std::make_shared<WText>(polygon);
            item->SetState(false);
			textContainer.push_back(item);

		}

		return textContainer;
	}
    double SquareFilling(const cv::Mat &image, cv::Point center, int radius)
    {
        int tlX = std::max(center.x - radius, 0);
        int tlY = std::max(center.y - radius, 0);
        int brX = std::min(center.x + radius, image.cols - 1);
        int brY = std::min(center.y + radius, image.rows - 1);
        double square = 0;
        cv::Rect roi(cv::Point(tlX, tlY), cv::Point(brX, brY));
        for (int y = roi.y; y <= roi.y + roi.height; y++)
        {
            for (int x = roi.x; x <= roi.x + roi.width; x++)
            {
                //if (squaredDistanceBetween(center, cv::Point(x, y)) < ((radius + 1) * (radius + 1)))
                if (image.at<uchar>(y, x) != 0)
                    square++;
            }
        }
        return square / ((roi.height + 1) * (roi.width + 1));
    }
	void FitSize(cv::Rect& rect, cv::Size size)
	{
		rect.x = std::max(0, rect.x);
		rect.y = std::max(0, rect.y);
		rect.width = std::min(size.width - rect.x, rect.width);
		rect.height = std::min(size.height - rect.y, rect.height);
	}
}
SDK_END_NAMESPACE