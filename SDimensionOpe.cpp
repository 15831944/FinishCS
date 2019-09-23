#include "stdafx.h"
#include "SDimensionOpe.h"

Acad::ErrorStatus SDimensionOpe::setTextHeiglt(AcDbObjectId objDime, double dHeight)
{
	AcDbDimension *pDim;
	Acad::ErrorStatus es = acdbOpenObject(pDim, objDime, AcDb::kForWrite);
	if(Acad::eOk != es)
	{
		acutPrintf(_T("\n 打开标注实体失败！"));
		return es;
	}

	es = pDim->setDimtxt(dHeight);
	pDim->close();

	return es;
}


Acad::ErrorStatus SDimensionOpe::scaleText(AcDbObjectId objDime, double dScale)
{
	
	AcDbDimension *pDim;
	Acad::ErrorStatus es = acdbOpenObject(pDim, objDime, AcDb::kForWrite);
	if(Acad::eOk != es)
	{
		acutPrintf(_T("\n 打开标注实体失败！"));
		return es;
	}

	double dHeight = pDim->dimtxt();
	dHeight *= dScale;
	es = pDim->setDimtxt(dHeight);
	pDim->close();

	return es;
}


SDimensionOpe::SDimensionOpe(void)
{
}


SDimensionOpe::~SDimensionOpe(void)
{
}
