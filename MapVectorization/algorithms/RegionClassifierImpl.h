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
	float m_radDivSquare = 0.0093;// �������� ��������� ������� ����������� ���������� � �������, ��������, ��������
			//(��� ������������� ��������� �� ���� ����� / �� �����)

private:
	cv::Mat *m_inputPtr;
	output_data m_result;

	virtual void getRegions();
	void fireMethod();// �������, ��������� ����� ��� ���������� ��������� ��������
	void regionClassifier(std::vector <cv::Point>& region);
};

