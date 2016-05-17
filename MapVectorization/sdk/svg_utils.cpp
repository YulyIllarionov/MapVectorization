#include "stdafx.h"
#include "svg_utils.h"

SVGUtils::SVGUtils(void)
{
}


SVGUtils::~SVGUtils(void)
{
}

void SVGUtils::WriteLineToSVG(SDK_NAMESPACE::WObjectContainer wvector, string filename)
{
	std::ofstream fout;
	fout.open(filename);
	
	if (fout.fail())
	{
		return;
	}
	fout << "<?xml version=\"1.0\" standalone=\"no\"?>"<< endl;
	fout << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
	fout << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
	fout << "<svg version = \"1.1\"" << endl; 
	
	for (int i = 0; i<wvector.size(); i++)
	{
		WriteLine(reinterpret_cast<SDK_NAMESPACE::WLine>(wvector[i]), fout);
	}
	fout.close();

}

void SVGUtils::WriteTextToSVG(SDK_NAMESPACE::WObjectContainer& wvector, string filename)
{
	std::ofstream fout;
	fout.open(filename);

	if (fout.fail())
	{
		return;
	}
	fout << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl;
	fout << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
	fout << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
	fout << "<svg version = \"1.1\"" << endl;

	for (int i = 0; i<wvector.size(); i++)
	{
		WriteText((SDK_NAMESPACE::WText)wvector[i], fout);
	}
	fout.close();

}

void SVGUtils::WriteLine (SDK_NAMESPACE::WLine& line, std::ofstream& fout)
{
	fout << "<line>";
	for (int i = 0; i<line.Length(); i++)
	{
		fout << "<x=\"" << line.GetPoint(i).GetX() << "\" y=\"" << line.GetPoint(i).GetY() << "\"/>" << endl;
	}
	fout << "</line>";
}

void SVGUtils::WriteText(SDK_NAMESPACE::WText& text,std::ofstream& fout)
{
	fout << "<text>";
	fout << "\" lengthAdjust=\"spacing\">" << text.GetText() << endl;
	WritePolygon(text.GetPolygon(), fout);
	fout << "</text>";

}

void SVGUtils::WritePolygon(SDK_NAMESPACE::WPolygon& polygon, std::ofstream& fout)
{
	fout << "<polygon>";
	for (int i = 0; i<polygon.Length(); i++)
	{
		fout << "<x=\"" << polygon.GetPoint(i).GetX() << "\" y=\"" << polygon.GetPoint(i).GetY() << "\"/>" << endl;
	}
	fout << "</polygon>";
}