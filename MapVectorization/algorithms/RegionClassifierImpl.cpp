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

			cv::Point currentPoint = cv::Point(i, j);
			stack.push(currentPoint);

			while (!stack.empty())
			{
				currentPoint = stack.top();
				stack.pop(); // Удалить точку из стека
				currentRegion.push_back(currentPoint); //Добавить точку к результату
				m_inputPtr->at <uchar>(currentPoint.x, currentPoint.y) = 0; // Закрасить на изображении

				for (int k = currentPoint.x - 1; k < currentPoint.x + 1; k++)
				{
					if (k == -1) // Следим за границами изображения
						continue;
					if (k == m_inputPtr->cols)
						break;
					
					for (int l = currentPoint.y - 1; l < currentPoint.y + 1; l++)
					{
						if (l == -1)
							continue;
						if (l == m_inputPtr->cols)
							break;
						
						if (m_inputPtr->at <uchar>(k, l))
							stack.push(cv::Point(k, l));
					}
				}
			}
			
			m_result.regions.push_back(currentRegion);
		}
	}
}
