/// Whiteteam
/// \file math_utils.cpp
/// \brief This file contains math utils
/// \author Whiteteam authors

#include "stdafx.h"
#include "math_utils.h"
#include "utils.h"
 
SDK_BEGIN_NAMESPACE

template<typename T> T MathUtils::Max(T val1, T val2)
{
  return val1 > val2 ? val1 : val2;
}

template<typename T> T MathUtils::Min(T val1, T val2)
{
  return val1 < val2 ? val1 : val2;
}
MathUtils::Wvector::Wvector()
{
    begin = cv::Point2f(0.0, 0.0);
    end = cv::Point2f(0.0, 0.0);
}
MathUtils::Wvector::Wvector(cv::Point2f p1, cv::Point2f p2)
{
    begin = p1;
    end = p2;
}
void MathUtils::Wvector::operator = (const Wvector& other)
{
    begin = other.begin;
    end = other.end;
}
double MathUtils::Wvector::Lenght()
{
    return std::sqrt(SDK_NAMESPACE::utils::squaredDistanceBetween(begin, end));
}
MathUtils::Wvector MathUtils::Wvector::ÑodirectionalVec(double lenght)
{
    Wvector outVec;
    double oldLenght = Lenght();
    double dx = lenght * (end.x - begin.x) / oldLenght;
    double dy = lenght * (end.y - begin.y) / oldLenght;
    outVec.begin = begin;
    outVec.end = begin + cv::Point2f(dx, dy);
    return outVec;
}
MathUtils::Wvector MathUtils::Wvector::OppositeVec()
{
    return Wvector(begin, begin * 2 - end);
}
MathUtils::Wvector MathUtils::Wvector::PerpendicularVec()
{
    cv::Point2f newEnd;
    newEnd.x = end.x;
    if (begin.y != end.y)
        newEnd.y = begin.y - (end.x - begin.x)*(end.x - begin.x) / (end.y - begin.y);
    else
        newEnd.y = end.y + Lenght();
    return Wvector(begin, newEnd);
}
MathUtils::Wvector MathUtils::Wvector::BisectorVec(Wvector vec)
{
    if (begin != vec.begin)
        return Wvector();
    Wvector codirVec = ÑodirectionalVec(vec.Lenght());
    return Wvector(begin, (codirVec.end + vec.end) / 2);
}

cv::Point2f MathUtils::Wvector::Middle()
{
    return (begin + end) / 2;
}

MathUtils::Wvector MathUtils::MinLengthVector(Wvector vec1, Wvector vec2)
{
    if (vec1.Lenght() < vec2.Lenght())
        return vec1;
    return vec2;
}

SDK_END_NAMESPACE
