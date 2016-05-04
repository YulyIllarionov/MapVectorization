#include "stdafx.h"
#include "text_recognition.h"


TextRecognition::TextRecognition(void)
{
}


TextRecognition::~TextRecognition(void)
{
}

void TextOCR (SDK_NAMESPACE::WVector wvector, cv::Mat img) 
{
	std::vector<SDK_NAMESPACE::WText> m_listTexts = wvector.GetTextList();
	
	for (int i = 0; i<m_listTexts.size(); i++)
	{
		int width = m_listTexts[i].getPointRight().x - m_listTexts[i].getPointLeft().x;
		int height = m_listTexts[i].getPointRight().y - m_listTexts[i].getPointLeft().y;
		ocr (img, cvRect(m_listTexts[i].getPointRight().y, m_listTexts[i].getPointLeft().x, width, height));
	}
}

std::string ocr(cv::Mat full_img, CvRect rect)
{

	cv::Mat subImage(full_img, rect); 
	std::string outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	api->SetImage(image);
	// Get OCR result
	outText = api->GetUTF8Text();
	api->End();
}