// algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
//
//#include "opencv2/opencv.hpp" 
//#include "opencv/cv.h"
//#include "opencv/highgui.h"

#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;
using namespace cv::text;

int _tmain(int argc, _TCHAR* argv[])
{
	std::string imgPath("C:/projects/MapVectorization/MapVectorization/sample/map/alphabet.jpg");

	cv::Mat image = cv::imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);

	string vocabulary = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // must have the same order as the clasifier output classes

    Ptr<OCRHMMDecoder::ClassifierCallback> ocr = loadOCRHMMClassifierCNN("OCRBeamSearch_CNN_model_data.xml.gz");

    double t_r = (double)getTickCount();
    vector<int> out_classes;
    vector<double> out_confidences;

    ocr->eval(image, out_classes, out_confidences);

    cout << "OCR output = \"" << vocabulary[out_classes[0]] << "\" with confidence "
         << out_confidences[0] << ". Evaluated in "
         << ((double)getTickCount() - t_r)*1000/getTickFrequency() << " ms." << endl << endl;

    return 0;
}

////---------------------------Не для проекта-----------------------
//void saveImage(const std::string &filename, cv::Mat *img) {
//	std::string output("C:/projects/MapVectorization/MapVectorization/sample/output/");
//	output += filename;
//	cv::imwrite(output, *img);
//}
//
////---------------------------Для проекта -------------------------
//std::vector<cv::Rect> detectLetters(cv::Mat &img, unsigned char elementId)
//{
//	std::vector<cv::Rect> boundRect;
//	cv::Mat img_reversed, img_sobel, img_threshold, element, img_gray;
//
//	cv::threshold(img, img_reversed, 0, 255, CV_THRESH_BINARY_INV);
//	//cvtColor(img_reversed, img_reversed, CV_BGR2GRAY);
//	//saveImage("rever1.jpg", &img_reversed);
//
//	//saveImage("rever1.jpg", &img_reversed);
//	cv::Sobel(img_reversed, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
//	//saveImage("obel1.jpg", &img_sobel);
//	//cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
//	element = getStructuringElement(cv::MORPH_RECT, cv::Size(55 * (int)pow(2, elementId), 3 * (int)pow(2, elementId)));
//	cv::morphologyEx(img_sobel, img_threshold, CV_MOP_CLOSE, element);
//	cv::cvtColor(img_threshold, img_threshold, CV_RGB2GRAY);
//	saveImage("morf.jpg", &img_threshold);
//
//	std::vector< std::vector< cv::Point> > contours;
//	cv::findContours(img_threshold, contours, cv::CHAIN_APPROX_NONE, cv::RETR_LIST);
//
//	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
//
//	for (int i = 0; i < contours.size(); i++)
//	{
//		//if ((contours.at(i).size() > 86 * pow(2, elementId)) && (contours.at(i).size() < 600 * pow(2, elementId)))
//		{
//			cv::approxPolyDP(cv::Mat(contours.at(i)), contours_poly.at(i), 3, true);
//			cv::Rect appRect(boundingRect(cv::Mat(contours_poly.at(i))));
//
//			//if (appRect.width > appRect.height)
//				boundRect.push_back(appRect);
//		}
//	}
//
//	return boundRect;
//}
//
//
//cv::Mat makeElementOfPyramid(cv::Mat &img, int n)
//{ //TODO: Размывать Гауссом
//	cv::Mat result = img;
//
//	for (int i = 1; i < n; i++)
//		cv::pyrDown(result, result, cv::Size(result.cols / 2, result.rows / 2));
//
//	for (int i = 1; i < n; i++)
//		cv::pyrUp(result, result, cv::Size(result.cols * 2, result.rows * 2));
//
//	return result;
//}
//
////std::vector<cv::Rect> FindRegionsWithText(cv::Mat &img)
////{
////	const int number_of_pyramid_elements = 4;
////	cv::Mat tmp;
////	std::vector<cv::Rect> result;
////
////	for (int i = 1; i <= number_of_pyramid_elements; i++) {
////		tmp = makeElementOfPiramid(img, i);
////		result.push_back
////	}
////
////}
//cv::Mat rotateImage(cv::Mat image) 
//{
//	 cv::Mat thr,dst;
//	 threshold(image,thr,200,255,cv::THRESH_BINARY_INV);
//	 imshow("thr",thr);
//
//	  std::vector<cv::Point> points;
//	  cv::Mat_<uchar>::iterator it = thr.begin<uchar>();
//	  cv::Mat_<uchar>::iterator end = thr.end<uchar>();
//	  for (; it != end; ++it)
//		if (*it)
//		  points.push_back(it.pos());
//
//	  cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
//	  cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, box.angle, 1);
//
//	  //cv::Mat rotated(src.size(),src.type(),Scalar(255,255,255));
//	  cv::Mat rotated;
//	  cv::warpAffine(image, rotated, rot_mat, image.size(), cv::INTER_CUBIC);
//	  imshow("rotated",rotated);
//	  
//	  return rotated;
//}

//int _tmain(int argc, _TCHAR* argv[])
//{
//	std::string imgPath("C:/projects/MapVectorization/MapVectorization/sample/map/black/cu71Black.png");
//
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
//}
