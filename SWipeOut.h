#pragma once

#include <imgdef.h>
#include <imgent.h>


class SWipeOut
{
public:
	SWipeOut(void);
	virtual ~SWipeOut(void);

	AcDbObjectId		WipeOut(AcGePoint2dArray node);

	AcDbObjectId		WipeOut(AcDbObjectId objId, bool bDelete = true);
	
};

