#include "stdafx.h"
#include "svg_utils.h"

SVGUtils::SVGUtils(void)
{
}


SVGUtils::~SVGUtils(void)
{
}

void SVGUtils::WriteLineToSVG(SDK_NAMESPACE::WLineVector<SDK_NAMESPACE::WLine> wvector, string filename)
{
	std::ofstream fout;
	std::vector<SDK_NAMESPACE::WLine> m_listLines = wvector.GetObjectList();
	fout.open(filename);
	
	if (fout.fail())
	{
		return;
	}
	fout << "<?xml version=\"1.0\" standalone=\"no\"?>"<< endl;
	fout << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
	fout << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
	fout << "<svg version = \"1.1\"" << endl; 
	
	for (int i = 0; i<m_listLines.size(); i++)
	{
		WriteLine(m_listLines[i].getPoints(), fout);
	}
	fout.close();

}

void SVGUtils::WriteTextToSVG(SDK_NAMESPACE::WTextVector<SDK_NAMESPACE::WText> wvector, string filename)
{
	std::ofstream fout;
	std::vector<SDK_NAMESPACE::WText> m_listLines = wvector.GetObjectList();
	fout.open(filename);

	if (fout.fail())
	{
		return;
	}
	fout << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
	fout << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
	fout << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
	fout << "<svg version = \"1.1\"" << endl;

	for (int i = 0; i<m_listLines.size(); i++)
	{
		WriteText(m_listLines[i], fout);
	}
	fout.close();

}

void SVGUtils::WriteLine (std::vector<Point> points, std::ofstream& fout)
{
	fout << "<line>";
	for (int i = 0; i<points.size(); i++)
	{
		fout << "<x=\"" << points[i].x << "\" y=\"" << points[i].y << "\"/>" << endl;
	}
	fout << "</line>";
}

void SVGUtils::WriteText(SDK_NAMESPACE::WText& text,std::ofstream& fout)
{
	fout << "<text>";
	fout << "\" lengthAdjust=\"spacing\">" << text.GetText() << endl;
	WritePolygon(text.getPolygon().getPoints(), fout);
	fout << "</text>";

}

void SVGUtils::WritePolygon(std::vector<Point> points, std::ofstream& fout)
{
	fout << "<polygon>";
	for (int i = 0; i<points.size(); i++)
	{
		fout << "<x=\"" << points[i].x << "\" y=\"" << points[i].y << "\"/>" << endl;
	}
	fout << "</polygon>";
}