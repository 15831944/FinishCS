#pragma once
class SDimensionOpe
{
public:
	SDimensionOpe(void);
	virtual ~SDimensionOpe(void);

	Acad::ErrorStatus scaleText(AcDbObjectId objDime, double dScale);

	Acad::ErrorStatus setTextHeiglt(AcDbObjectId objDime, double dHeight);

};

