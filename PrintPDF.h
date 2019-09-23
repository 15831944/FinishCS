#pragma once
#include "axlock.h"
#include "dbplotsettings.h"
#include "dbplotsetval.h"
#include "AcPlPlotEngine.h"
#include "AcPlPlotFactory.h"
#include "AcPlPlotProgress.h"
#include "AcPlPlotInfo.h"
#include "AcPlPlotInfoValidator.h"
#include "AcDbLMgr.h"
class CPrintPDF
{
public:
	CPrintPDF();
	~CPrintPDF();
	CString m_strPath;
	CString m_str_divice;		// 打印机名称
	CString m_strTempSavePath;
	AcDbObjectIdArray m_objArrSave;
	
	void Plot();

	bool setPrintSize();

	bool GetPlotSizeName(AcDbPlotSettingsValidator* pPSV, AcDbPlotSettings* pPlotSettings, CString& real_size_name);

private:
	CString m_str_size_name;	// 宗地图纸大小名称
};

