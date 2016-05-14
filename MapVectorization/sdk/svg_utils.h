#pragma once
#include <iostream>
#include <fstream>
#include "base_types.h"
using namespace std;
class SVGUtils
{
public:
	SVGUtils(void);
	~SVGUtils(void);

	//void WriteSVG (SDK_NAMESPACE::WVector wvector);
private:
	void writeLine (std::vector<CvPoint> points,std::ofstream& fout);
	void writeText (SDK_NAMESPACE::WText& text,std::ofstream& fout);
};

