#include "stdafx.h"
#include <stack>
#include "RegionClassifierImpl.h"


RegionClassifierImpl::RegionClassifierImpl()
{
}

RegionClassifierImpl::RegionClassifierImpl(cv::Mat& input)
{
	SetInputImage(input);
}

RegionClassifierImpl::~RegionClassifierImpl()
{
}

void RegionClassifierImpl::SetInputImage(cv::Mat& input)
{
	m_inputPtr = &input;
}

output_data RegionClassifierImpl::GetOutputData()
{
	getRegions();
	return m_result;
}

void RegionClassifierImpl::getRegions()
{
	fireMethod();
}

void RegionClassifierImpl::fireMethod()
{
	//cv::MatIterator_<uchar> it, end, begin = m_inputPtr->begin <uchar>(); // Моднейшие итераторы, например
	//for (it = begin, end = m_inputPtr->end <uchar>(); it != end; ++it)
	for (int i = 0; i < m_inputPtr->rows; i++)
	{
		for (int j = 0; j < m_inputPtr->cols; j++)
		{
			if (m_inputPtr->at <uchar>(i, j) == 0)
				continue;

			std::vector <cv::Point> currentRegion;
			std::stack <cv::Point> stack;

			stack.emplace(j, i);

			while (!stack.empty())
			{
				cv::Point currentPoint = stack.top();
				stack.pop(); // Удалить точку из стека
				currentRegion.push_back(currentPoint); //Добавить точку к результату

				for (int k = currentPoint.x - 1; k <= currentPoint.x + 1; k++)
				{
					if (k == -1) // Следим за границами изображения
						continue;
					if (k == m_inputPtr->cols)
						break;
					
					for (int l = currentPoint.y - 1; l <= currentPoint.y + 1; l++)
					{
						if ((l == -1) || ((l == i) && (k == j)))
							continue;
						if (l == m_inputPtr->rows)
							break;
						
						if (m_inputPtr->at <uchar>(l, k) != 0)
							stack.emplace(k, l);
					}
				}
				m_inputPtr->at <uchar>(currentPoint) = 0; // Закрасить на изображении
			}
			
			regionClassifier(currentRegion);
		}
	}
}

void RegionClassifierImpl::regionClassifier(std::vector <cv::Point> &region)
{
	if (region.size() < 70)
	{
		m_result.trash.push_back(region);
		return;
	}

	cv::Point2f center;
	float radius;
	std::vector <cv::Point> contours_poly(region.size());
	
	cv::approxPolyDP(cv::Mat(region), contours_poly, 3, true);
	cv::minEnclosingCircle(cv::Mat(contours_poly), center, radius);

	std::cout << radius / region.size() << std::endl;
	if (radius / region.size() > m_radDivSquare)
		m_result.lines.push_back(region);
	else
		m_result.regions.push_back(region);
}