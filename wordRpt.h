#pragma once
#include "MatchFialogsEx.h"
#include "tinyword.h"
#include "macros.h"
#include "Global.h"
#include "structs.h"
#include "TinyExcel.h"
#include "SmartTable.h"

class CExportSurveyTable
{
public:
	CExportSurveyTable(){}
	~CExportSurveyTable(){}

	CString					m_strPathExcel;
	CString					m_strPathWord;
	CString					m_strSavePath;

	bool exportSurveyResultReport();

	//提取原始坐标数据到doc文档上
	bool extractToOrignDoc();

private:

	vector<ElevationInfo>		m_vElevaInfo;

	int		findRecord(CString strName);

	bool		readSourceExcel();

	bool		extractOriginWord();



};