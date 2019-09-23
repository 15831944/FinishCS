#pragma once
#include "entitysInfo.h"
#include "SBlockOpe.h"
#include "StringsOpe.h"
#include "Global.h"
#include "Selection_String.h"

class CEntitysCheck
{
public:
	CEntitysCheck(void);
	virtual ~CEntitysCheck(void);

	bool		checkElevationPointAttribute();
};

