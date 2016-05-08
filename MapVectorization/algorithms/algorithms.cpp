// algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"

//---------------------------Не для проекта-----------------------
void saveImage(const std::string &filename, cv::Mat *img) {
	std::string output("C:/projects/MapVectorization/MapVectorization/sample/output/");
	output += filename;
	cv::imwrite(output, *img);
}

//---------------------------Для проекта -------------------------
std::vector<cv::Rect> detectLetters(cv::Mat &img, unsigned char elementId)
{
	std::vector<cv::Rect> boundRect;
	cv::Mat img_reversed, img_sobel, img_threshold, element, img_gray;

	cv::threshold(img, img_reversed, 0, 255, CV_THRESH_BINARY_INV);
	//cvtColor(img_reversed, img_reversed, CV_BGR2GRAY);
	saveImage("rever1.jpg", &img_reversed);

	//saveImage("rever1.jpg", &img_reversed);
	cv::Sobel(img_sobel, img_reversed, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	saveImage("obel1.jpg", &img_sobel);
	cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	element = getStructuringElement(cv::MORPH_RECT, cv::Size(20 * (int)pow(2, elementId), 3 * (int)pow(2, elementId)));
	cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);
	saveImage("morf.jpg", &img_threshold);

	std::vector< std::vector< cv::Point> > contours;
	cv::findContours(img_threshold, contours, 0, 1);

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		if ((contours.at(i).size() > 86 * pow(2, elementId)) && (contours.at(i).size() < 600 * pow(2, elementId)))
		{
			cv::approxPolyDP(cv::Mat(contours.at(i)), contours_poly.at(i), 3, true);
			cv::Rect appRect(boundingRect(cv::Mat(contours_poly.at(i))));

			if (appRect.width > appRect.height)
				boundRect.push_back(appRect);
		}
	}

	return boundRect;
}


cv::Mat makeElementOfPyramid(cv::Mat &img, int n)
{ //TODO: Размывать Гауссом
	cv::Mat result = img;

	for (int i = 1; i < n; i++)
		cv::pyrDown(result, result, cv::Size(result.cols / 2, result.rows / 2));

	for (int i = 1; i < n; i++)
		cv::pyrUp(result, result, cv::Size(result.cols * 2, result.rows * 2));

	return result;
}

//std::vector<cv::Rect> FindRegionsWithText(cv::Mat &img)
//{
//	const int number_of_pyramid_elements = 4;
//	cv::Mat tmp;
//	std::vector<cv::Rect> result;
//
//	for (int i = 1; i <= number_of_pyramid_elements; i++) {
//		tmp = makeElementOfPiramid(img, i);
//		result.push_back
//	}
//
//}
cv::Mat rotateImage(cv::Mat image) 
{
	 cv::Mat thr,dst;
	 threshold(image,thr,200,255,cv::THRESH_BINARY_INV);
	 imshow("thr",thr);

	  std::vector<cv::Point> points;
	  cv::Mat_<uchar>::iterator it = thr.begin<uchar>();
	  cv::Mat_<uchar>::iterator end = thr.end<uchar>();
	  for (; it != end; ++it)
		if (*it)
		  points.push_back(it.pos());

	  cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
	  cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, box.angle, 1);

	  //cv::Mat rotated(src.size(),src.type(),Scalar(255,255,255));
	  cv::Mat rotated;
	  cv::warpAffine(image, rotated, rot_mat, image.size(), cv::INTER_CUBIC);
	  imshow("rotated",rotated);
	  
	  return rotated;
}
int _tmain(int argc, _TCHAR* argv[])
{
	std::string imgPath("C:/projects/MapVectorization/MapVectorization/sample/map/black/1.png");

	cv::Mat img = cv::imread(imgPath);
	cv::threshold(img, img, 0, 255, CV_THRESH_BINARY);
	//saveImage("output1.jpg", &img);

	std::vector<cv::Rect> letterBBoxes = detectLetters(img, 0);

	cv::Mat img1 = makeElementOfPyramid(img, 2);
	std::vector<cv::Rect> letterBBoxes1 = detectLetters(img1, 1);

	cv::Mat img2 = makeElementOfPyramid(img, 3);
	std::vector<cv::Rect> letterBBoxes2 = detectLetters(img2, 2);

	for (int i = 0; i< letterBBoxes.size(); i++)
		cv::rectangle(img, letterBBoxes.at(i), cv::Scalar(0, 255, 0), 3, 8, 0);

	for (int i = 0; i< letterBBoxes1.size(); i++)
		cv::rectangle(img, letterBBoxes1.at(i), cv::Scalar(0, 0, 255), 3, 8, 0);

	for (int i = 0; i< letterBBoxes2.size(); i++)
		cv::rectangle(img, letterBBoxes2.at(i), cv::Scalar(255, 0, 0), 3, 8, 0);

	saveImage("output1.jpg", &img);
	return 0;
}
