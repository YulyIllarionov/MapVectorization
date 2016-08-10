#pragma once
#include "IDataMapper.h"
#include "../util/utils.h"

class SMapPointDataMapper :	public IDataMapper<SMapPoint>
{
public:
	SMapPointDataMapper(void);
	virtual ~SMapPointDataMapper(void);
};

