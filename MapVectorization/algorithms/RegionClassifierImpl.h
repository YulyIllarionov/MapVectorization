#pragma once
#include "IOperation.h"

//typedef cv::OutputArrayOfArrays 

typedef struct
{
	std::vector <std::vector <cv::Point>> lines;
	std::vector <std::vector <cv::Point>> regions;
	std::vector <std::vector <cv::Point>> trash;
} output_data;

class RegionClassifierImpl : public IOperation <output_data>
{
public:
	RegionClassifierImpl();
	RegionClassifierImpl(cv::Mat& input);
	virtual ~RegionClassifierImpl();

	virtual void SetInputImage(cv::Mat& input);
	virtual output_data GetOutputData();

protected:
	float m_radDivSquare = 0.0093;// Дичайшее отношение радиуса описываемой окружности к площади, например, пикселей
			//(для классификации элементов по типу линия / не линия)

private:
	cv::Mat *m_inputPtr;
	output_data m_result;

	virtual void getRegions();
	void fireMethod();// Тащемта, моднейший метод для нахождения связанных областей
	void regionClassifier(std::vector <cv::Point>& region);
};

