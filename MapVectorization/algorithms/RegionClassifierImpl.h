#pragma once
#include "IOperation.h"

//typedef cv::OutputArrayOfArrays 

typedef struct
{
	std::vector <std::vector <cv::Point>> regions;
} output_data;

class RegionClassifierImpl : public IOperation <output_data>
{
public:
	RegionClassifierImpl();
	RegionClassifierImpl(cv::Mat& input);
	virtual ~RegionClassifierImpl();

	virtual void SetInputImage(cv::Mat& input);
	virtual output_data GetOutputData();

private:
	cv::Mat *m_inputPtr;
	output_data m_result;

	virtual void getRegions();
	void fireMethod();// Тащемта, дичайший метод для нахождения связанных областей
};

