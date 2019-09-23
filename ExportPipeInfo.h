#pragma once
#include "enumcl.h"
#include "structs.h"
#include "Selection_String.h"
#include "DataProcess.h"
#include ".\\libs\\cplpub.h"
#include "DialogCheck.h"

/*******************************************

 * @brief		������ߵ�ɹ�����࣬����fastreport�����Ƽ���
 * @author		�춫��
 * @date		[2016-03-18 09:06]

 **************************************************/             
class CExportPipeInfo
{
public:
	CExportPipeInfo(void);
	virtual ~CExportPipeInfo(void);

	bool		getData(AcDbObjectId objPP, AcDbObjectIdArray objArrPl);

	bool		exportPipeResultTable();

	bool		getFieldsAndTypes();

	bool	exportInfoToMdb();

	bool  invokeFastReport();

	bool		replacePipePointType();

	CAdoConnection		m_conn;
	CAdoRecordSet		m_rs;

	CString			m_strSavePath;	
	TCHAR			m_szMdbPath[1000];
	vector<CString>		m_vFieldsName;
	vector<short>		m_vFieldsType;
	vector<short>		m_vFieldPipeType;
	vector<CString>		m_vFieldText;

private:
		PIPETYPE m_pipeCur;

		CString m_strParam;//�������

		//���ö��߳� ���ɱ���
		bool export_report_by_temp_mdb_multiply(const vector<CString> & mdbPathArray , const vector<CString> & templatePathArray,
			const vector<CString> &outPathArray , TCHAR *szExePath , int nCount , bool you_see_is_you_get = true);
};

