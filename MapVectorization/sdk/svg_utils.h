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

	void WriteLineToSVG (SDK_NAMESPACE::WObjectContainer& wvector, string filename);
	void WriteTextToSVG (SDK_NAMESPACE::WObjectContainer& wvector, string filename);
private:
	void WriteLine (SDK_NAMESPACE::WLine& line,std::ofstream& fout);
	void WriteText (SDK_NAMESPACE::WText& text,std::ofstream& fout);
	void WritePolygon(SDK_NAMESPACE::WPolygon& polygon, std::ofstream& fout);
};
