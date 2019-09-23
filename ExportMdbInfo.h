#pragma once
#include "AdoConnection.h"
#include "excelRpt.h"
#include "ExportReportFactory.h"

struct SSortAndClass
{
	int  nSort;
	TCHAR szClass[200];

	TCHAR szCodes[200];

	SSortAndClass()
	{
		nSort = 0;
		_tcscpy(szClass, _T(""));
		_tcscpy(szCodes, _T(""));
	}
};

struct SUnitAndCode
{
	TCHAR szUnit[200];
	CString strCodes;

	SUnitAndCode()
	{
		_tcscpy(szUnit, _T(""));
	}
};

struct SStaticData
{
	int		  sNum;
	TCHAR szFrameNum[200];
	TCHAR szOwnerName[200];
	TCHAR szID[200];
	TCHAR szIDIndex[200];
	TCHAR szClass1[200];
	TCHAR szClass2[200];
	double dHeight;
	double dDeep;
	double dSpan;
	double dWellDiameter;
	double dTreediameter;
	int nCount;
	double dLength;
	double dArea;
	double dVolume;

	SStaticData()
	{
		sNum = 0;
		_tcscpy(szFrameNum, _T(""));
		_tcscpy(szOwnerName, _T(""));
		_tcscpy(szID, _T(""));
		_tcscpy(szIDIndex, _T(""));
		_tcscpy(szClass1, _T(""));
		_tcscpy(szClass2, _T(""));
		dHeight = 0.0;
		dDeep = 0.0;
		dSpan = 0.0;
		dWellDiameter = 0.0;
		dTreediameter = 0.0;
		nCount = 0;
		dLength = 0.0;
		dArea = 0.0;
		dVolume = 0.0;
	}
};

class CExportMdbInfo
{
public:
	CExportMdbInfo(void);
	virtual ~CExportMdbInfo(void);

	short					m_sReportType;

	CString					m_strSavePath;
	CString					m_strSaveFlolder;
	TCHAR					m_szExe_path[1000];
	TCHAR					m_szMdbPath[1000];
	CAdoConnection		m_conn;

	vector<SStaticData>		m_vRecords;

	vector<CString>		m_vStrSavePath;
	vector<CString>		m_vect_out_path;
	bool	exportGroundAttachmentsTable_Standard2();
	bool	exportOwnerInfoSummaryTable_Standard2();

	bool	exportInfoToMdb();

	bool	excuteStoreProcess();

	bool	exportInfoToMdb_GroundAttachmentsTableSummary();


	bool  invokeFastReport();

private:
	bool					m_bReadAccdbTemp;
	CString				m_strParam;//输出参数
	TCHAR				m_szFRXTempName[500];

	vector<SSortAndClass>	m_vSortAndClass;
	vector<SUnitAndCode>	 m_vUnitAndCodes;

	bool		mergeExcelColumns();

	bool		addToCurrentData(SStaticData sData);


	//调用多线程 生成报表
	bool export_report_by_temp_mdb_multiply(const vector<CString> & mdbPathArray , const vector<CString> & templatePathArray,
			const vector<CString> &outPathArray , TCHAR *szExePath , int nCount , bool you_see_is_you_get = true);

	//这部分的代码可以写在mdb的查询表格中，利用查询更新来实现，不用代码来实现
	bool		getSortAndClass(const TCHAR *szCode, int &nSort, TCHAR *szClass);

	bool		getUnits(const TCHAR *szCode, TCHAR *szUnit);


	bool		readSortAndClassInfo();
	bool		readUnitsFromTable();

	bool		checkFilesExits(vector<AcDbObjectIdArray> vObjArrFrame);

};

