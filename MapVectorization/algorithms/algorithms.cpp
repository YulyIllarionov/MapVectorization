// algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <cmath>
using namespace cv;
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
	//saveImage("rever1.jpg", &img_reversed);

	//saveImage("rever1.jpg", &img_reversed);
	cv::Sobel(img_reversed, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	//saveImage("obel1.jpg", &img_sobel);
	//cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	element = getStructuringElement(cv::MORPH_RECT, cv::Size(55 * (int)pow(2, elementId), 3 * (int)pow(2, elementId)));
	cv::morphologyEx(img_sobel, img_threshold, CV_MOP_CLOSE, element);
	cv::cvtColor(img_threshold, img_threshold, CV_RGB2GRAY);
	saveImage("morf.jpg", &img_threshold);

	std::vector< std::vector< cv::Point> > contours;
	cv::findContours(img_threshold, contours, cv::CHAIN_APPROX_NONE, cv::RETR_LIST);

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		//if ((contours.at(i).size() > 86 * pow(2, elementId)) && (contours.at(i).size() < 600 * pow(2, elementId)))
		{
			cv::approxPolyDP(cv::Mat(contours.at(i)), contours_poly.at(i), 3, true);
			cv::Rect appRect(boundingRect(cv::Mat(contours_poly.at(i))));

			//if (appRect.width > appRect.height)
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
cv::Mat rotateImage(std::vector<cv::Point> vec_points,cv::Mat img, float angle) 
{
	int x1 = vec_points[0].x;
	int y1 = vec_points[0].y;
	int width = vec_points[1].x-vec_points[0].x;
	int height = vec_points[2].y - vec_points[0].y;

	//cv::Mat image = cv::Mat(img, cv::Rect(x1,y1, width, height)); 
	cv::Mat image = img;
	cv::Mat thr,dst;
	threshold(image,thr,200,255,cv::THRESH_BINARY_INV);
	imshow("thr",thr);

	std::vector<cv::Point> points;
	cv::Mat_<uchar>::iterator it = thr.begin<uchar>();
	cv::Mat_<uchar>::iterator end = thr.end<uchar>();
	for (; it != end; ++it)
	{
		if (*it)
		{
			points.push_back(it.pos());
		}
	}
	cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
	cv::Size box_size = box.size;
	if (box.angle < -45.)
	{
		std::swap(box_size.width, box_size.height);
	}
	cv::Mat rot_mat = cv::getRotationMatrix2D(box.center,angle, 1);
	  
	std::cout<<box.angle;
	cv::Mat rotated;
	cv::warpAffine(image, rotated, rot_mat, image.size(), cv::INTER_CUBIC);
	  
	cv::Mat cropped;
	cv::getRectSubPix(rotated, box_size, box.center, cropped);

	imshow("rotated",cropped);
	cv::waitKey(27);
	  
	return cropped;
}

float countAngle(std::vector<Vec4i> lines) 
{
	float angle = 0;
	for (size_t i = 0; i < lines.size(); i++)
        {
        	Vec4i l = lines[i];
			Point p1, p2;
			p1=Point(l[0], l[1]);
			p2=Point(l[2], l[3]);
			angle += atan2(p1.y - p2.y, p1.x - p2.x);
        }
	angle = angle/lines.size();
	angle = (angle*180)/CV_PI;
	if (angle < -45.)
	{
		angle += 180.;
	} 
	else if (angle > 45.) 
	{
		angle-=180;
	}
	return angle; 
}
int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<cv::Point> data;
	data.push_back(cv::Point(0,0));
	data.push_back(cv::Point(200,0));
	data.push_back(cv::Point(0,179));
	data.push_back(cv::Point(200,179));

	cv::Mat src=cv::imread("E:/Downloads/text22.png",0);
	rotateImage(data,src, -65);

    namedWindow("1", CV_WINDOW_KEEPRATIO);

    Mat text = imread("E:/Downloads/text22.png", CV_LOAD_IMAGE_COLOR);
    Mat textGray;
    Mat current;
    cvtColor(text, textGray, CV_BGR2GRAY);
    //GaussianBlur(gray, gray, Size(9, 9), 2, 2);
    //blur(gray, gray, Size(3, 3));
    int threshold = 40;
    int minLineLength = 40;
    int maxLineGap = 10;

    cvCreateTrackbar("threshold: ", "1", &threshold, 500);
    cvCreateTrackbar("minLineLength: ", "1", &minLineLength, 200);
    cvCreateTrackbar("maxLineGap: ", "1", &maxLineGap, 100);

    int key;

    while (true)
    {
        current = text.clone();
        std::vector<Vec4i> lines;
        //Ищем линии
        HoughLinesP(textGray, lines, 1, CV_PI / 180, threshold, minLineLength, maxLineGap);
		float angle=0;
        for (size_t i = 0; i < lines.size(); i++)
        {
        	Vec4i l = lines[i];
			Point p1, p2;
			p1=Point(l[0], l[1]);
			p2=Point(l[2], l[3]);
        	line(current, p1, p2, Scalar(0, 0, 255), 1, CV_AA);
        }
		angle = countAngle(lines);
		std::cout<<angle<<" ";
        imshow("1", current);

        key = waitKey(100);
        if (key == 27)
            break;
    }

    return 0;
	//std::string imgPath("C:/projects/MapVectorization/MapVectorization/sample/map/black/cu71Black.png");

//	cv::Mat img = cv::imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
	//cv::threshold(img, img, 0, 255, CV_THRESH_BINARY);
	

	//**
	//Поиск текста
	//**
	//cvCvtColor(&img, &img, CV_RGB2BGRA);
	////saveImage("output1.jpg", &img);

	//std::vector<cv::Rect> letterBBoxes = detectLetters(img, 0);

	////cv::Mat img1 = makeElementOfPyramid(img, 2);
	////std::vector<cv::Rect> letterBBoxes1 = detectLetters(img1, 1);

	////cv::Mat img2 = makeElementOfPyramid(img, 3);
	////std::vector<cv::Rect> letterBBoxes2 = detectLetters(img2, 2);

	//for (int i = 0; i< letterBBoxes.size(); i++)
	//	cv::rectangle(img, letterBBoxes.at(i), cv::Scalar(0, 255, 0), 3, 8, 0);

	////for (int i = 0; i< letterBBoxes1.size(); i++)
	////	cv::rectangle(img, letterBBoxes1.at(i), cv::Scalar(0, 0, 255), 3, 8, 0);

	////for (int i = 0; i< letterBBoxes2.size(); i++)
	////	cv::rectangle(img, letterBBoxes2.at(i), cv::Scalar(255, 0, 0), 3, 8, 0);
	//
	//saveImage("output1.jpg", &img);



	//**
	//Поиск линий
	//**
	//RegionClassifierImpl classifier(img);
	//output_data objects = classifier.GetOutputData();
	////saveImage("output1.jpg", &img);
    //
	//cv::Mat img1(img.size(), CV_8UC3);
	//img1.setTo(cv::Scalar(0, 0, 0));
    //
	//uchar step = 0;
	////cv::Vec3b color;
	////color[0] = 90 + 10 * (++step); color[1] = 180 + 10 * step; color[2] = 10 * step; // "Рандомный" цвет
	////img1.at <cv::Vec3b>(37, 466) = color;
	//
	//cv::Vec3b color(255, 255, 255);
	//	
	//for (auto i : objects.regions)
	//{	
	//	for (auto j : i)
	//	{
	//		img1.at <cv::Vec3b>(j) = color;
	//	}
	//}
    //
	//cv::Vec3b color_tr(255, 0, 0); // Рисуем мусор
	//for (auto i : objects.trash)
	//{
	//	for (auto j : i)
	//	{
	//		img1.at <cv::Vec3b>(j) = color_tr;
	//	}
	//}
	// 
	//cv::Vec3b color_ln(0, 255, 0); // Линии
	//for (auto i : objects.lines)
	//{
	//	for (auto j : i)
	//	{
	//		img1.at <cv::Vec3b>(j) = color_ln;
	//	}
	//}
    //
	//saveImage("count3.jpg", &img1);
	//return 0;
}
