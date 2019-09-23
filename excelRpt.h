#pragma once
#include "tinyexcel.h"
#include "PathDialog.h"
#include "EditListCtrl.h"
#include "tinyxml.h"
#include "structs.h"
#include "macros.h"
#include "SFilter.h"
#include "Selection_String.h"
#include "Global.h"
#include "CassInfo.h"
#include "XRecordOperate.h"
#include "ContructsOperate.h"

class CCheckResult
{
public:
	CCheckResult();
	~CCheckResult(){}

	CString m_strPathFeatureTable;
	CString m_strPathSummartTable;
	CString m_strPathBuildingAreaFrame;
	CString	m_strPathTotalFrame;

	vector<CSVInfo>	m_vCSVInfo_SummaryTable;
	vector<CSVInfo>	m_vCSVInfo_SummartBuildFrame;
	vector<CSVInfo>	m_vCSVInfo_TolalFrame;

	int							m_checkPrec;

	bool		checkBuildingName();


	bool		checkIndoorHeight();

private:
	bool		readSummaryTableInfo();

	bool		readBuildingAreaInfo();

	bool		readTotalFrameInfo();
};

struct FloorInfo_Excel
{
	vector<int>			vSequence;
	vector<CString>		vStrFloorName;
	vector<double> vFloorHeight;
	vector<double>		vDaughterWallHeight;
	vector<double>		vMachineHeight;

	vector<double>		vArea_Tol;
	vector<double>		vArea_TolTop;
	vector<double>		vArea_TolUnder;
	vector<int>			vFloorCount_Top;
	vector<int>			vFloorCount_Under;

};

enum enmIndoorReport
{
	IndoorReport_GroundAttachmentsTable,				//���ϸ�����ǼǱ�
	IndoorReport_OwnerInfoTable,							//������Ϣ�ǼǱ�
	IndoorReport_OwnerInfoSummaryTable,				//���ϸ�������ܱ�
	IndoorReport_GroundAttachmentsTable_standard2,		//���ϸ�����ǼǱ�_��׼2
	IndoorReport_OwnerInfoSummaryTable_standard2,		//���ϸ�������ܱ�_��׼2
};

class CExportSummaryTable
{
public:
	CExportSummaryTable(void);
	virtual ~CExportSummaryTable(void);

	CTinyExcel			m_excel;
	short					m_sReportType;

	CString				m_strSavePath;
	CString				m_strSourceExcel;
	CString				m_strSourceDwg;
	FloorInfo_Excel		m_FloorInfo;

	vector<CString>				m_vStrKey, m_vStrValue;

	//������ͼ�������
	bool		checkArea();

	//������ǼǱ�
	bool		exportGroundAttachmentsTable();
	bool		exportGroundAttachmentsTable_Single(AcDbObjectIdArray objArr);
	bool		getGroundAttachmentsTableInfo();

	//
	bool		getSummaryObjects(AcDbObjectIdArray objArr, AcDbObjectIdArray &objArrSummary);
	bool		getRecord(AcDbObjectIdArray objArrSummary, vector<CString> &vStrResult);

	//����������ܱ�
	bool		exportOwnerInfoTable();
	bool		exportOwnerInfoTable_Single(AcDbObjectIdArray objArr);
	bool		getOwnerInfoTableInfo();

	//��������ܱ�
	bool		exportOwnerInfoSummaryTable();
	bool		exportOwnerInfoSummaryTable_Single(AcDbObjectIdArray objArr);
	bool		getOwnerSummaryTemplateInfo();

	CString				m_strCSVPath;
	CString				m_strFeatureTableTempPath;
	vector<CSVInfo>	m_vCSVInfo;
	int						m_nMergedRow;
	bool		extractFeatureTable();
	bool		readCSVFile();
	bool		fillFeatureTable();
	bool		fillFeatureTable_Floor(CSVInfo csv);
	void		sortSingleCSVRecord(CSVInfo &csv);
	void		setFloorFormal(int nRowBegin, int nFloorRecordCount, int nHaveIndoorHeight);

	//���������������ܱ�
	bool		exportSummaryTable();
	bool		readSourceExcel();
	bool		readSourceXML();
	bool		readSourceDwg();
	bool		readSourceXRecord(AcDbDatabase *pExternalDb);
	bool		fillSummaryTable();

	//ѡ��ͼ�򣬹���ͼ�򣬶�ͼ��������򣬻�ȡ·��
	bool		getAndFilterFrame_Indoor(vector<AcDbObjectIdArray> &vObjArrFrame);

private:
		
	vector<CString>		m_vCode;
	vector<CString>		m_vApp;
	vector<CString>		m_vValue;
	vector<int>			m_vType;

	int							m_nStartRow;
	int							m_nWarning;


	bool		replaceLabels();

	bool		replaceLabels2();

};

