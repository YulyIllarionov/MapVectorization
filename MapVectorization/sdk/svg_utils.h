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

	void WriteLineToSVG (SDK_NAMESPACE::WLineVector<SDK_NAMESPACE::WLine> wvector, string filename);
	void WriteTextToSVG (SDK_NAMESPACE::WTextVector<SDK_NAMESPACE::WText> wvector, string filename);
private:
	void WriteLine (std::vector<Point> points,std::ofstream& fout);
	void WriteText (SDK_NAMESPACE::WText& text,std::ofstream& fout);
	void WritePolygon(std::vector<Point> points, std::ofstream& fout);
};
