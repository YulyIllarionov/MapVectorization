#include "stdafx.h"
#include "text_recognition.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/core/core_c.h"
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
		m_listTexts[i].AddText = ocr (img, cvRect(m_listTexts[i].getPointRight().y, m_listTexts[i].getPointLeft().x, width, height));
	}
}

std::string ocr(cv::Mat full_img, CvRect rect)
{

	cv::Mat subImage(full_img, rect); 
	std::string outText;
	//Pix *image = pixRead("E:/Downloads/a2056.jpg");
	IplImage* Image = cvLoadImage( "test" );
	 
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	api->Init(NULL, "eng");
	api->SetImage( (uchar*)Image->imageData, Image->width, Image->height, Image->nChannels, Image->widthStep );
	Boxa* boxes = api->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	char* ocrResult;
	for (int i = 0; i < boxes->n; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		char* ocrResult = api->GetUTF8Text();
		int conf = api->MeanTextConf();	
	}

	return ocrResult;
}