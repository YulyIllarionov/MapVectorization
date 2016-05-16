#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
#include "opencv/highgui.h"

template <class T>
class IOperation
{
public:
	virtual void SetInputImage(cv::Mat& input) = 0;
	virtual T GetOutputData() = 0;
};