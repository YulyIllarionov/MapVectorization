// algorithms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::text;

//Calculate edit distance netween two words
size_t edit_distance(const string& A, const string& B);
size_t min(size_t x, size_t y, size_t z);
bool   isRepetitive(const string& s);
bool   sort_by_lenght(const string &a, const string &b);
//Draw ER's in an image via floodFill
void  er_draw(Mat &src, Mat &dst, ERStat& er);
void  er_draw(Mat &src, Mat &dst, Rect region);
std::vector<cv::Rect> detectLetters(cv::Mat &img);	

//Perform text detection and recognition and evaluate results using edit distance
int main(int argc, char* argv[])
{
    namedWindow("1", CV_WINDOW_KEEPRATIO);

    Mat text = imread("C:\\MapVectorization\\MapVectorization\\algorithms\\black\\text1.png", CV_LOAD_IMAGE_COLOR);
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
        vector<Vec4i> lines;
        //Ищем линии
        HoughLinesP(textGray, lines, 1, CV_PI / 180, threshold, minLineLength, maxLineGap);

        for (size_t i = 0; i < lines.size(); i++)
        {
            Vec4i l = lines[i];
            line(current, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
        }

        imshow("1", current);

        key = waitKey(100);
        if (key == 27)
            break;
    }
    
    return 0;
}

size_t min(size_t x, size_t y, size_t z)
{
    return x < y ? min(x,z) : min(y,z);
}

size_t edit_distance(const string& A, const string& B)
{
    size_t NA = A.size();
    size_t NB = B.size();

    vector< vector<size_t> > M(NA + 1, vector<size_t>(NB + 1));

    for (size_t a = 0; a <= NA; ++a)
        M[a][0] = a;

    for (size_t b = 0; b <= NB; ++b)
        M[0][b] = b;

    for (size_t a = 1; a <= NA; ++a)
        for (size_t b = 1; b <= NB; ++b)
        {
            size_t x = M[a-1][b] + 1;
            size_t y = M[a][b-1] + 1;
            size_t z = M[a-1][b-1] + (A[a-1] == B[b-1] ? 0 : 1);
            M[a][b] = min(x,y,z);
        }

    return M[A.size()][B.size()];
}

bool isRepetitive(const string& s)
{
    int count = 0;
    for (int i=0; i<(int)s.size(); i++)
    {
        if ((s[i] == 'i') ||
                (s[i] == 'l') ||
                (s[i] == 'I'))
            count++;
    }
    if (count > ((int)s.size()+1)/2)
    {
        return true;
    }
    return false;
}


void  er_draw(Mat &src, Mat &dst, ERStat& er)
{

    if (er.parent != NULL) // deprecate the root region 
    {
        int newMaskVal = 255;
        int flags = 4 + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
        floodFill(src,dst,Point(er.pixel%src.cols,er.pixel/src.cols),Scalar(255),0,Scalar(er.level),Scalar(0),flags);
    }

}

void  er_draw(Mat &src, Mat &dst, Rect region)
{
	src(region).copyTo(dst(region));
}

bool   sort_by_lenght(const string &a, const string &b){return (a.size()>b.size());}

//---------------------------Не для проекта-----------------------
void saveImage(const std::string &filename, cv::Mat *img) {
	std::string output("C:/projects/MapVectorization/MapVectorization/sample/output/");
	output += filename;
	cv::imwrite(output, *img);
}
////---------------------------Не для проекта-----------------------
//void saveImage(const std::string &filename, cv::Mat *img) {
//	std::string output("C:/projects/MapVectorization/MapVectorization/sample/output/");
//	output += filename;
//	cv::imwrite(output, *img);
//}
//
////---------------------------Для проекта -------------------------
std::vector<cv::Rect> detectLetters(cv::Mat &img)
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
		boundRect.push_back(appRect);		
	}

	return boundRect;
}
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

//cv::Mat rotateImage(std::vector<cv::Point> vec_points,cv::Mat img, float angle) 
//{
//	int x1 = vec_points[0].x;
//	int y1 = vec_points[0].y;
//	int width = vec_points[1].x-vec_points[0].x;
//	int height = vec_points[2].y - vec_points[0].y;
//
//	//cv::Mat image = cv::Mat(img, cv::Rect(x1,y1, width, height)); 
//	cv::Mat image = img;
//	cv::Mat thr,dst;
//	threshold(image,thr,200,255,cv::THRESH_BINARY_INV);
//	imshow("thr",thr);
//
//	std::vector<cv::Point> points;
//	cv::Mat_<uchar>::iterator it = thr.begin<uchar>();
//	cv::Mat_<uchar>::iterator end = thr.end<uchar>();
//	for (; it != end; ++it)
//	{
//		if (*it)
//		{
//			points.push_back(it.pos());
//		}
//	}
//	cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
//	cv::Size box_size = box.size;
//	if (box.angle < -45.)
//	{
//		std::swap(box_size.width, box_size.height);
//	}
//	cv::Mat rot_mat = cv::getRotationMatrix2D(box.center,angle, 1);
//	  
//	cv::Mat rotated;
//	cv::warpAffine(image, rotated, rot_mat, image.size(), cv::INTER_CUBIC);
//	  
//	cv::Mat cropped;
//	cv::getRectSubPix(rotated, box_size, box.center, cropped);
//
//	imshow("rotated",cropped);
//	cv::waitKey(0);
//	  
//	return cropped;
//}
//
//float countAngle(String image_name) 
//{
//	Mat text = imread(image_name, CV_LOAD_IMAGE_COLOR);
//	Mat textGray;
//    Mat current;
//    cvtColor(text, textGray, CV_BGR2GRAY);
//    //GaussianBlur(gray, gray, Size(9, 9), 2, 2);
//    //blur(gray, gray, Size(3, 3));
//    int threshold = 40;
//    int minLineLength = 40;
//    int maxLineGap = 10;
//
//    cvCreateTrackbar("threshold: ", "1", &threshold, 500);
//    cvCreateTrackbar("minLineLength: ", "1", &minLineLength, 200);
//    cvCreateTrackbar("maxLineGap: ", "1", &maxLineGap, 100);
//
//	current = text.clone();
//    std::vector<Vec4i> lines;
//    //Ищем линии
//    HoughLinesP(textGray, lines, 1, CV_PI / 180, 40, 0, 50);
//	float angle=0;
//    
//	for (size_t i = 0; i < lines.size(); i++)
//    {
//    	Vec4i l = lines[i];
//		Point p1, p2;
//		p1=Point(l[0], l[1]);
//		p2=Point(l[2], l[3]);
//		angle += atan2(p1.y - p2.y, p1.x - p2.x);
//    }
//	angle = angle/lines.size();
//	angle = (angle*180.)/CV_PI;
//	if (angle < 0.)
//	{
//		angle += 180.;
//	} 
//	else if (angle > 0.) 
//	{
//		angle-=180;
//	}
//	std::cout<<angle<<std::endl;
//	return angle; 
//}
//int _tmain(int argc, _TCHAR* argv[])
//{
//	std::vector<cv::Point> data;
//	data.push_back(cv::Point(0,0));
//	data.push_back(cv::Point(200,0));
//	data.push_back(cv::Point(0,179));
//	data.push_back(cv::Point(200,179));
//	String img_name = "E:/Downloads/text22.png";
//	cv::Mat src=cv::imread(img_name,0);
//	rotateImage (data,src, countAngle(img_name));
//	
//    return 0;
//	//std::string imgPath("C:/projects/MapVectorization/MapVectorization/sample/map/black/cu71Black.png");
//
////	cv::Mat img = cv::imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
//	//cv::threshold(img, img, 0, 255, CV_THRESH_BINARY);
//	
//
//	//**
//	//Поиск текста
//	//**
//	//cvCvtColor(&img, &img, CV_RGB2BGRA);
//	////saveImage("output1.jpg", &img);
//
//	//std::vector<cv::Rect> letterBBoxes = detectLetters(img, 0);
//
//	////cv::Mat img1 = makeElementOfPyramid(img, 2);
//	////std::vector<cv::Rect> letterBBoxes1 = detectLetters(img1, 1);
//
//	////cv::Mat img2 = makeElementOfPyramid(img, 3);
//	////std::vector<cv::Rect> letterBBoxes2 = detectLetters(img2, 2);
//
//	//for (int i = 0; i< letterBBoxes.size(); i++)
//	//	cv::rectangle(img, letterBBoxes.at(i), cv::Scalar(0, 255, 0), 3, 8, 0);
//
//	////for (int i = 0; i< letterBBoxes1.size(); i++)
//	////	cv::rectangle(img, letterBBoxes1.at(i), cv::Scalar(0, 0, 255), 3, 8, 0);
//
//	////for (int i = 0; i< letterBBoxes2.size(); i++)
//	////	cv::rectangle(img, letterBBoxes2.at(i), cv::Scalar(255, 0, 0), 3, 8, 0);
//	//
//	//saveImage("output1.jpg", &img);
//
//
//
//	//**
//	//Поиск линий
//	//**
//	//RegionClassifierImpl classifier(img);
//	//output_data objects = classifier.GetOutputData();
//	////saveImage("output1.jpg", &img);
//    //
//	//cv::Mat img1(img.size(), CV_8UC3);
//	//img1.setTo(cv::Scalar(0, 0, 0));
//    //
//	//uchar step = 0;
//	////cv::Vec3b color;
//	////color[0] = 90 + 10 * (++step); color[1] = 180 + 10 * step; color[2] = 10 * step; // "Рандомный" цвет
//	////img1.at <cv::Vec3b>(37, 466) = color;
//	//
//	//cv::Vec3b color(255, 255, 255);
//	//	
//	//for (auto i : objects.regions)
//	//{	
//	//	for (auto j : i)
//	//	{
//	//		img1.at <cv::Vec3b>(j) = color;
//	//	}
//	//}
//    //
//	//cv::Vec3b color_tr(255, 0, 0); // Рисуем мусор
//	//for (auto i : objects.trash)
//	//{
//	//	for (auto j : i)
//	//	{
//	//		img1.at <cv::Vec3b>(j) = color_tr;
//	//	}
//	//}
//	// 
//	//cv::Vec3b color_ln(0, 255, 0); // Линии
//	//for (auto i : objects.lines)
//	//{
//	//	for (auto j : i)
//	//	{
//	//		img1.at <cv::Vec3b>(j) = color_ln;
//	//	}
//	//}
//    //
//	//saveImage("count3.jpg", &img1);
//	//return 0;
////}
//