/// Whiteteam
/// \file skeletonization.cpp
/// \brief This file contains skeletonization
/// \author Whiteteam authors

#include "stdafx.h"
#include "skeletonization.h"
//#include "libs\dl_codes.h"
#include <set>

using namespace std;
using namespace cv;

SDK_BEGIN_NAMESPACE

//WSkeletonizer* WSkeletonizer::m_instance = nullptr;

WSkeletonizer::SkeletonTemplate::SkeletonTemplate(int row, int column, int center_x, int center_y)
  : m_center(std::make_pair(center_x, center_y))
{
  m_points.clear();
  m_points.reserve(row);
  /*for (int i = 0; i < row; ++i)
  {
    m_points.push_back(TemplateRow(column));
  }*/
}

WSkeletonizer::WSkeletonizer()
{
  InitializeTemplates();
  InitializeTemplatesExt();
}

/*WSkeletonizer* WSkeletonizer::Instance()
{
  if (m_instance == nullptr)
    m_instance = new WSkeletonizer();
  return m_instance;
}*/

void WSkeletonizer::InitializeTemplates()
{
  // #1 - (a)
  // 1 1 y
  // 1 c 0
  // 1 1 y
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Foreground); row1.push_back(Foreground); row1.push_back(Y); 
    row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Background);
    row3.push_back(Foreground); row3.push_back(Foreground); row3.push_back(Y);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #2 - (b)
  // 1 1 1
  // 1 c 1
  // y 0 y
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Foreground); row1.push_back(Foreground); row1.push_back(Foreground);
    row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Foreground);
    row3.push_back(Y);          row3.push_back(Background); row3.push_back(Y);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #3 - (c)
  // y 1 1 x
  // 0 c 1 1
  // y 1 1 x
  {
    SkeletonTemplate _template(3, 4, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Y);          row1.push_back(Foreground); row1.push_back(Foreground); row1.push_back(Any);
    row2.push_back(Background); row2.push_back(C);          row2.push_back(Foreground); row2.push_back(Foreground);
    row3.push_back(Y);          row3.push_back(Foreground); row3.push_back(Foreground); row3.push_back(Any);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #4 - (d)
  // y 0 y
  // 1 c 1
  // 1 1 1
  // x 1 x
  {
    SkeletonTemplate _template(4, 3, 1, 1);

    SRow row1, row2, row3, row4;
    row1.push_back(Y);          row1.push_back(Background); row1.push_back(Y);
    row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Foreground);
    row3.push_back(Foreground); row3.push_back(Foreground); row3.push_back(Foreground);
    row4.push_back(Any);        row4.push_back(Foreground); row4.push_back(Any);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);
    _template.m_points.push_back(row4);

    m_templates.push_back(_template);
  }

  // #5 - (e)
  // x 0 0
  // 1 c 0
  // x 1 x
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Any);        row1.push_back(Background); row1.push_back(Background);
    row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Background);
    row3.push_back(Any);        row3.push_back(Foreground); row3.push_back(Any);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #6 - (f)
  // x 1 1 
  // 0 c 1
  // 0 0 x

  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Any);        row1.push_back(Foreground); row1.push_back(Foreground);
    row2.push_back(Background); row2.push_back(C);          row2.push_back(Foreground);
    row3.push_back(Background); row3.push_back(Background); row3.push_back(Any);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #7 - (g)
  // 0 1 0
  // 0 c 1
  // 0 0 0
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Background); row1.push_back(Foreground); row1.push_back(Background);
    row2.push_back(Background); row2.push_back(C);          row2.push_back(Foreground);
    row3.push_back(Background); row3.push_back(Background); row3.push_back(Background);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #8 - (h)
  // x 1 x
  // 1 c 0
  // x 0 0
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Any);        row1.push_back(Foreground); row1.push_back(Any);
    row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Background);
    row3.push_back(Any);        row3.push_back(Background); row3.push_back(Background);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #9 - (i)
  // 0 0 x
  // 0 c 1
  // x 1 1
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Background); row1.push_back(Background); row1.push_back(Any);
    row2.push_back(Background); row2.push_back(Any);        row2.push_back(Foreground);
    row3.push_back(Any);        row3.push_back(Foreground); row3.push_back(Foreground);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #10 - (j)
  // 0 0 0
  // 0 c 1
  // 0 1 0
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Background); row1.push_back(Background); row1.push_back(Background);
    row2.push_back(Background); row2.push_back(C);          row2.push_back(Foreground);
    row3.push_back(Background); row3.push_back(Foreground); row3.push_back(Background);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #11 - (k)
  // 0 0 0
  // 0 c 0
  // 1 1 1
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Background); row1.push_back(Background); row1.push_back(Background);
    row2.push_back(Background); row2.push_back(C);          row2.push_back(Background);
    row3.push_back(Foreground); row3.push_back(Foreground); row3.push_back(Foreground);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #12 - (l)
  // 1 0 0
  // 1 c 0
  // 1 0 0
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Foreground); row1.push_back(Background); row1.push_back(Background);
    row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Background);
    row3.push_back(Foreground); row3.push_back(Background); row3.push_back(Background);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #13 - (m)
  // 1 1 1
  // 0 c 0
  // 0 0 0
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Foreground); row1.push_back(Foreground); row1.push_back(Foreground);
    row2.push_back(Background); row2.push_back(C);          row2.push_back(Background);
    row3.push_back(Background); row3.push_back(Background); row3.push_back(Background);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

  // #14 - (n)
  // 0 0 1
  // 0 c 1
  // 0 0 1
  {
    SkeletonTemplate _template(3, 3, 1, 1);

    SRow row1, row2, row3;
    row1.push_back(Background); row1.push_back(Background); row1.push_back(Foreground);
    row2.push_back(Background); row2.push_back(C);          row2.push_back(Foreground);
    row3.push_back(Background); row3.push_back(Background); row3.push_back(Foreground);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);

    m_templates.push_back(_template);
  }

}

void WSkeletonizer::InitializeTemplatesExt()
{
  // #1 ext left down
  // 1 x 1 x
  // 1 1 x 1
  // 1 c 1 x
  // 0 1 1 1
  {
    SkeletonTemplate _template(4, 4, 2, 1);

    SRow row1, row2, row3, row4;
    row1.push_back(Foreground); row1.push_back(Any);        row1.push_back(Foreground); row1.push_back(Any);
    row2.push_back(Foreground); row2.push_back(Foreground); row2.push_back(Any);        row2.push_back(Foreground);
    row3.push_back(Foreground); row3.push_back(C);          row3.push_back(Foreground); row3.push_back(Any);
    row4.push_back(Background); row4.push_back(Foreground); row4.push_back(Foreground); row4.push_back(Foreground);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);
    _template.m_points.push_back(row4);

    m_templates_ext.push_back(_template);
  }

  // #2 ext left up
  // 0 1 1 1
  // 1 c 1 x
  // 1 1 x 1
  // 1 x 1 x
  {
    SkeletonTemplate _template(4, 4, 1, 1);

    SRow row1, row2, row3, row4;
    row1.push_back(Background); row1.push_back(Foreground); row1.push_back(Foreground); row1.push_back(Foreground);
    row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Foreground); row2.push_back(Any);
    row3.push_back(Foreground); row3.push_back(Foreground); row3.push_back(Any);        row3.push_back(Foreground);
    row4.push_back(Foreground); row4.push_back(Any);        row4.push_back(Foreground); row4.push_back(Any);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);
    _template.m_points.push_back(row4);

    m_templates_ext.push_back(_template);
  }


  // #3 ext right up
  // 1 1 1 0
  // x 1 c 1
  // 1 x 1 1
  // x 1 x 1
  {
    SkeletonTemplate _template(4, 4, 1, 2);

    SRow row1, row2, row3, row4;
    row1.push_back(Foreground); row1.push_back(Foreground); row1.push_back(Foreground); row1.push_back(Background);
    row2.push_back(Any);        row2.push_back(Foreground); row2.push_back(C);          row2.push_back(Foreground);
    row3.push_back(Foreground); row3.push_back(Any);        row3.push_back(Foreground); row3.push_back(Foreground);
    row4.push_back(Any);        row4.push_back(Foreground); row4.push_back(Any);        row4.push_back(Foreground);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);
    _template.m_points.push_back(row4);

    m_templates_ext.push_back(_template);
  }

  // #4 ext right down
  // x 1 x 1
  // 1 x 1 1
  // x 1 c 1
  // 1 1 1 0
  {
    SkeletonTemplate _template(4, 4, 2, 2);

    SRow row1, row2, row3, row4;
    row1.push_back(Any);        row1.push_back(Foreground); row1.push_back(Any);        row1.push_back(Foreground);
    row2.push_back(Foreground); row2.push_back(Any);        row2.push_back(Foreground); row2.push_back(Foreground);
    row3.push_back(Any);        row3.push_back(Foreground); row3.push_back(C);          row3.push_back(Foreground);
    row4.push_back(Foreground); row4.push_back(Foreground); row4.push_back(Foreground); row4.push_back(Background);

    _template.m_points.push_back(row1);
    _template.m_points.push_back(row2);
    _template.m_points.push_back(row3);
    _template.m_points.push_back(row4);

    m_templates_ext.push_back(_template);
  }

}

bool WSkeletonizer::InitializeSkeletonByRaster(const Mat& raster, WSkeleton& skeleton)
{
  //int debug_foreground_count = 0;
  skeleton.resize(raster.rows);
  //std::map<uchar, int> color_table;
  for (int i = 0; i < raster.rows; i++)
  {
    skeleton[i].resize(raster.cols);
    for (int j = 0; j < raster.cols; j++)
    {
      //uchar color = raster.at<uchar>(i, j);
      //if (color_table.find(color) != color_table.end())
      //  color_table[color]++;
      //else
      //  color_table[color] = 1;

      skeleton[i][j] = raster.at<uchar>(i, j) == WCOLOR_WHITE ? Background : Foreground;
      //if (raster.at(j, i) != WCOLOR_WHITE)
      //  debug_foreground_count++;
    }
  }
  return true;
}

int WSkeletonizer::MatchPatterns(const WSkeleton& skeleton, int coord_x, int coord_y)
{
  for (size_t i = 0; i < m_templates.size(); i++)
  {
    if (MatchPattern(m_templates[i], skeleton, coord_x, coord_y))
      return i;
  }
  return -1;
}

bool WSkeletonizer::MatchPattern(const SkeletonTemplate& _template, const WSkeleton& skeleton,
  int coord_x, int coord_y)
{

  if (_template.m_points.size() == 0 || skeleton.size() == 0)
    return false;

  int center_x = _template.m_center.first;
  int center_y = _template.m_center.second;

  bool isY1Init  = false;
  bool isContainY = false;
  bool isY1Background = false;
  bool isY2Background = false;

  for (size_t i = 0; i < _template.m_points.size(); i++)
  {
    for (size_t j = 0; j < _template.m_points[i].size(); j++)
    {
      // is pattern could be checked
      if (coord_x - center_x + i < 0
        || coord_x - center_x + i >= skeleton.size()
        || coord_y - center_y + j < 0
        || coord_y - center_y + j >= skeleton[i].size())
      {
        return false;
      }

      if ((Y & _template.m_points[i][j]) != 0)
      {
        isContainY = true;
        if (!isY1Init)
        {
          isY1Init = true;
          isY1Background = (skeleton[coord_x - center_x + i][coord_y - center_y + j] & Background) != 0;
        }
        else
        {
          isY2Background = (skeleton[coord_x - center_x + i][coord_y - center_y + j] & Background) != 0;
        }
      } else if ((skeleton[coord_x - center_x + i][coord_y - center_y + j]
        & _template.m_points[i][j]) == 0)
      {
        return false;
      }
    }
  }

  if (isContainY)
    return isY1Background || isY2Background;

  return true;
}

bool WSkeletonizer::IsConcaveCornelPixel(const WSkeleton& skeleton, int coord_x, int coord_y)
{
  // only 1 of P(8) pixel is foreground.
  // p5 p6 p7
  // p4 p  p0
  // p3 p2 p1

  if (coord_x - 1 < 0 || coord_x + 1 >= (int)skeleton[0].size()
    || coord_y - 1 < 0 || coord_y + 1 >= (int)skeleton.size())
  {
    return false;
  }

  int background_pixels = 0;

  if ((skeleton[coord_x + 1][coord_y] & Background) != 0)
    background_pixels++;

  if ((skeleton[coord_x + 1][coord_y - 1] & Background) != 0)
    background_pixels++;

  if ((skeleton[coord_x][coord_y - 1] & Background) != 0)
    background_pixels++;

  if ((skeleton[coord_x - 1][coord_y - 1] & Background) != 0)
    background_pixels++;

  if ((skeleton[coord_x - 1][coord_y] & Background) != 0)
    background_pixels++;

  if ((skeleton[coord_x - 1][coord_y + 1] & Background) != 0)
    background_pixels++;

  if ((skeleton[coord_x][coord_y + 1] & Background) != 0)
    background_pixels++;

  if ((skeleton[coord_x + 1][coord_y + 1] & Background) != 0)
    background_pixels++;

  return background_pixels == 1;
}

int WSkeletonizer::ConcaveCornelPixels(const WSkeleton& skeleton, int coord_x, int coord_y, 
  int pattern_num)
{
  int pixels = 0x0;

  // only 1 of P(8) pixel is foreground.
  // p7 p0 p1
  // p6 p  p2 p8
  // p5 p4 p3
  //    p9

  switch (pattern_num)
  {
    /*Pattern(a) : 
    If pixel p1 = 1 then p0 is a concave corner pixel
    If pixel p3 = 1 then p4 is a concave corner pixel
    */
    case 0:
      if (((skeleton[coord_x + 1][coord_y + 1]) & Foreground) != 0)
        pixels |= UP;
      if (((skeleton[coord_x + 1][coord_y - 1]) & Foreground) != 0)
        pixels |= DOWN;
      break;

    /*  Pattern (b): 
    If pixel p3 = 1 then p2 is a concave corner pixel
    If pixel p5 = 1 then p6 is a concave corner pixel*/
    case 1:
      if (((skeleton[coord_x + 1][coord_y - 1]) & Foreground) != 0)
        pixels |= RIGHT;
      if (((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0)
        pixels |= LEFT;
      break;
    
    /*  Pattern (c): 
    If pixel p5 = 1 then p4 is a concave corner pixel
    If pixel p7 = 1 then p2 is a concave corner pixel*/
    case 2:
      if (((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0)
        pixels |= DOWN;
      if (((skeleton[coord_x - 1][coord_y + 1]) & Foreground) != 0)
        pixels |= RIGHT;
      break;
    
    /*  Pattern (d): 
    If pixel p1 = 1 then p2 is a concave corner pixel
    If pixel p7 = 1 then p6 is a concave corner pixel*/
    case 3:
      if (((skeleton[coord_x + 1][coord_y + 1]) & Foreground) != 0)
        pixels |= RIGHT;
      if (((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0)
        pixels |= LEFT;
      break;
    
    /*  Pattern (e): 
    If pixel p7 = 1 and p5 = 1 then p6 is a concave corner pixel
    If pixel p5 = 1 and p3 = 1 then p4 is a concave corner pixel*/
    case 4:
      if (((skeleton[coord_x - 1][coord_y + 1]) & Foreground) != 0
        && ((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0)
        pixels |= LEFT;
      if (((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0
        && ((skeleton[coord_x + 1][coord_y - 1]) & Foreground) != 0)
        pixels |= DOWN;
      break;
    
    /*  Pattern (f): 
    If pixel p7 = 1 then p0 is a concave corner pixel
    If pixel p5 = 1 then p2 is a concave corner pixel*/
    case 5:
      if (((skeleton[coord_x - 1][coord_y + 1]) & Foreground) != 0)
        pixels |= UP;
      if (((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0)
        pixels |= RIGHT;
      break;
    
    /*  Pattern (h): 
    If pixel p7 = 1 and p1 = 1 then p0 is a concave corner pixel
    If pixel p7 = 1 and p5 = 1 then p6 is a concave corner pixel*/
    case 7:
      if (((skeleton[coord_x - 1][coord_y + 1]) & Foreground) != 0
        && ((skeleton[coord_x + 1][coord_y + 1]) & Foreground) != 0)
        pixels |= UP;
      if (((skeleton[coord_x - 1][coord_y + 1]) & Foreground) != 0
        && ((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0)
        pixels |= LEFT;
      break;
    
    /*  Pattern (i): 
    If pixel p1 = 1 then p2 is a concave corner pixel
    If pixel p5 = 1 then p4 is a concave corner pixel*/
    case 8:
      if (((skeleton[coord_x + 1][coord_y + 1]) & Foreground) != 0)
        pixels |= RIGHT;
      if (((skeleton[coord_x - 1][coord_y - 1]) & Foreground) != 0)
        pixels |= DOWN;
      break;
    /**/
    default:
      break;
  }

  return pixels;
}


bool WSkeletonizer::Skeletonize(const Mat& raster, Mat& _skeleton)
{
  // Initialize skeleton Mat
  _skeleton = Mat(raster.rows, raster.cols, raster.type());

  WSkeleton next_skeleton;
  if (!InitializeSkeletonByRaster(raster, next_skeleton))
    return false;

  WSkeleton curr_skeleton;
  bool pixel_was_deleted = false;
  do {
    pixel_was_deleted = false;
    curr_skeleton = next_skeleton; // ???
    for (size_t i = 0; i < curr_skeleton.size(); i++)
    {
      for (size_t j = 0; j < curr_skeleton[i].size(); j++)
      {
        if (curr_skeleton[i][j] == Foreground )
        {
          int pattern_num = MatchPatterns(curr_skeleton, i, j);
          if (pattern_num == -1)
            continue;
        
          next_skeleton[i][j] = Background;
          pixel_was_deleted = true;

#if 0
          int concave_pixels = ConcaveCornelPixels(curr_skeleton, i, j, pattern_num);

          // every of 4 bound pixels
          // left
          if ((concave_pixels & LEFT) != 0)
          {
            // check if marked
            if ((curr_skeleton[i - 1][j] & Marked) == 0)
            {
              curr_skeleton[i - 1][j] |= Marked; // if not then mark
            }
            else if (true || MatchPattern(m_templates_ext[0], curr_skeleton, i - 1, j))
            {
              next_skeleton[i - 1][j] = Background;
              //curr_skeleton[i - 1][j] &= ~Marked;
            }
          }
          // up
          if ((concave_pixels & UP) != 0)
          {
            // check if marked
            if ((curr_skeleton[i][j + 1] & Marked) == 0)
            {
              curr_skeleton[i][j + 1] |= Marked; // if not then mark
            }
            else if (true || MatchPattern(m_templates_ext[0], curr_skeleton, i, j + 1))
            {
              next_skeleton[i][j + 1] = Background;
              //curr_skeleton[i][j + 1] &= ~Marked;
            }
          }
          // right
          if ((concave_pixels & RIGHT) != 0)
          {
            // check if marked
            if ((curr_skeleton[i + 1][j] & Marked) == 0)
            {
              curr_skeleton[i + 1][j] |= Marked; // if not then mark
            }
            else if (true || MatchPattern(m_templates_ext[0], curr_skeleton, i + 1, j))
            {
              next_skeleton[i + 1][j] = Background;
              //curr_skeleton[i + 1][j] &= ~Marked;
            }
          }
          // down
          if ((concave_pixels & DOWN) != 0)
          {
            // check if marked
            if ((curr_skeleton[i][j - 1] & Marked) == 0)
            {
              curr_skeleton[i][j - 1] |= Marked; // if not then mark
            }
            else if (true || MatchPattern(m_templates_ext[0], curr_skeleton, i, j - 1))
            {
              next_skeleton[i][j - 1] = Background;
              //curr_skeleton[i][j - 1] &= ~Marked;
            }
          }
#endif

        }
      }      
    }
  } while (pixel_was_deleted);

  for (size_t i = 0; i < next_skeleton.size(); i++)
  {
    for (size_t j = 0; j < next_skeleton[i].size(); j++)
    {
      if ((next_skeleton[i][j] & Foreground) != 0)
        _skeleton.at<uchar>(i, j) = raster.at<uchar>(i, j);
      else
        _skeleton.at<uchar>(i, j) = WCOLOR_WHITE;
    }
  }
  return false;
}


SDK_END_NAMESPACE


