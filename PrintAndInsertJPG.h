#include "StdAfx.h"
#include "tinyword.h"
#pragma once

class AcDbPlotSettings;
class AcDbPlotSettingsValidator;

int rescale_image_jpeg(const char* , const char* ,
	const unsigned char &,	const unsigned char &,
	const unsigned char &,	const long &,	const long &,
	void *callback = NULL);

class CPrintAndInsertJPG
{
public:
	CPrintAndInsertJPG(void);
	~CPrintAndInsertJPG(void);
	void Plot(AcDbObjectIdArray m_objArrSave);

private:

	bool GetPlotSizeName(AcDbPlotSettingsValidator *pPSV, AcDbPlotSettings *pPlotSettings, CString &real_size_name);
	bool insertZDCT(CTinyWord &m_tinyword);

public:

	CString m_str_divice;
	CString m_str_size_name;

};

