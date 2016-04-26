#include "stdafx.h"
#include "svg_utils.h"


SVGUtils::SVGUtils(void)
{
}


SVGUtils::~SVGUtils(void)
{
}

void SVGUtils::WriteSVG (SDK_NAMESPACE::WVector wvector)
{
	std::ofstream fout;
	std::vector<SDK_NAMESPACE::WText> m_listTexts = wvector.GetTextList();
	std::vector<SDK_NAMESPACE::WPolyline> m_listPolylines = wvector.GetPolylineList();
	fout.open("path_to_file.svg");
	
	if (fout.fail())
	{
		return;
	}
	fout << "<?xml version=\"1.0\" standalone=\"no\"?>"<< endl;
	fout << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
	fout << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
	fout << "<svg version = \"1.1\"" << endl; 
	
	for (int i = 0; i<m_listTexts.size(); i++)
	{
		writeText(m_listTexts[i], fout);
	}

	for (int i = 0; i<m_listPolylines.size(); i++)
	{
		writeText(m_listPolylines[i].GetText(), fout);
		writeLine(m_listPolylines[i].getPoints(), fout);
	}

}

void SVGUtils::writeLine (std::vector<CvPoint> points, std::ofstream& fout)
{
	for (int i = 0; i<points.size()-1; i++)
	{
		fout<<"<line x1=\""<<points[i].x<<"\" y1=\""<<points[i].y<<"\"";
		fout<<"x2=\""<<points[i+1].x<<"\" y2=\""<<points[i+1].y<<"\"/>"<<endl;
	}
}

void SVGUtils::writeText(SDK_NAMESPACE::WText& text,std::ofstream& fout)
{
	double height = text.getPointRight().y - text.getPointLeft().y;
	double width = text.getPointRight().x - text.getPointLeft().x;
	fout<<"<text x=\""<<text.getPointLeft().x<<"\" y=\""<<text.getPointLeft().y<<"\" textLength=\""<<width<<
		"\" lengthAdjust=\"spacing\">"<<text.GetText()<<"</text>"<<endl;

}