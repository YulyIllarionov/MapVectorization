#pragma once
#include "datamappers.h"

SDK_BEGIN_NAMESPACE

int SaveProject(std::string& filename, std::shared_ptr<WRaster> item);
std::shared_ptr<WRaster> LoadProject(std::string &filename);
int SaveSVG(std::string& filename, std::shared_ptr<WRaster> item);

SDK_END_NAMESPACE