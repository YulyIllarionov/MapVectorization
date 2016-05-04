#pragma once
#include <string>
#include "base_types.h"
class TextRecognition
{
public:
	void TextOCR (SDK_NAMESPACE::WVector wvector, cv::Mat img);
	TextRecognition(void);
	~TextRecognition(void);
private:
	std::string ocr (cv::Mat img, CvRect rect);
};

