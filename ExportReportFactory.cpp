#include "stdafx.h"
#include "ExportReportFactory.h"


CExportReportFactory::CExportReportFactory(void)
{
}


CExportReportFactory::~CExportReportFactory(void)
{
	
}
CString CExportReportFactory::get_temp_mdb_path(TCHAR *szMdbPath , const CStringArray & strSqlArray , int nIndex)
{
	IReport * pReport = NULL;
	pReport = new Land_survey_report;
	if(pReport != NULL)
	{
		pReport->set_mdb_path(szMdbPath);
		pReport->set_sql_string(strSqlArray);
		TCHAR szTempPath[MAX_PATH] = {0};
		pReport->get_temp_mdb_path(szTempPath , nIndex);
		delete pReport;
		pReport = NULL;
		return szTempPath;
	}
	return _T("");
}
CString CExportReportFactory::get_temp_mdb_path(CAdoConnection & connect , TCHAR *szMdbPath , const CStringArray & strSqlArray , int nIndex)
{
	IReport * pReport = NULL;
	pReport = new Land_survey_report;
	if(pReport != NULL)
	{
		pReport->set_mdb_path(szMdbPath);
		pReport->set_sql_string(strSqlArray);
		TCHAR szTempPath[MAX_PATH] = {0};
		pReport->get_temp_mdb_path(szTempPath , nIndex);
		delete pReport;
		pReport = NULL;
		return szTempPath;
	}
	return _T("");
}
void Land_survey_report::set_mdb_path(TCHAR *szMdbPath)
{
	_tcscpy(m_szMdbPath , szMdbPath);
}
bool Land_survey_report::excute_sql(CString strPath , const CStringArray &strArray)
{
	CAdoConnection connect;
	if(FALSE == connect.ConnectAccess(strPath))
	{
		AfxMessageBox(_T("\n连接数据库失败"));
		return false;
	}
	for (int i = 0 ; i < strArray.GetSize() ; i++)
	{
		CString strSQL = strArray.GetAt( i );
		if(strSQL.GetLength() <= 0)
			continue;
		connect.Execute(strSQL);//执行
	}
	connect.Close();
	return true;
}
bool Land_survey_report::excute_sql(CAdoConnection & connect , const CStringArray & strArray)
{
	if(FALSE == connect.IsOpen())
		return false;
	for (int i = 0 ; i < strArray.GetSize() ; i++)
	{
		CString strSQL = strArray.GetAt( i );
		if(strSQL.GetLength() <= 0)
			continue;
		connect.Execute(strSQL);//执行
	}
	connect.Close();
	return true;
}



//设置查询
void Land_survey_report::set_sql_string(const CStringArray & strsql)
{
	m_str_sql_array.RemoveAll();
	for(int  i = 0 ; i < strsql.GetSize() ; i++)
	{
		CString str = strsql.GetAt( i );
		if(str.GetLength() > 0)
			m_str_sql_array.Add(str);
	}
}
//获得临时数据库
bool Land_survey_report::get_temp_mdb_path(TCHAR *szPath , int nIndex)
{
	if(_tcslen(m_szMdbPath) <= 0)
		return false;
	//拷贝数据库
	TCHAR szNewPath[MAX_PATH] = {0};
	CString str = m_szMdbPath;
	CString strTemp;
	if(str.Right(5).CompareNoCase(_T("accdb"))== 0)
	{
		strTemp.Format(_T("tmp%d.accdb"),nIndex);
		str.Replace(_T(".accdb") , strTemp);
	}
	else
	{
		strTemp.Format(_T("tmp%d.mdb"),nIndex);
		str.Replace(_T(".mdb") , strTemp);
	}

// #ifdef DEBUG
// 	AfxMessageBox(_T("\n当前的路径 %s") , str);
// #endif
	if(CopyFile(m_szMdbPath , str , false) == FALSE)
		return false;
	//执行sql
	if(false == excute_sql(str , m_str_sql_array))
		return false;
	_stprintf(szPath , _T("%s") , str);
	return true;
}
//获得临时数据库
bool Land_survey_report::get_temp_mdb_path(TCHAR *szPath , CAdoConnection & connect)
{
	if(_tcslen(m_szMdbPath) <= 0)
		return false;
	//拷贝数据库
	TCHAR szNewPath[MAX_PATH] = {0};
	CString str = m_szMdbPath;
	str.Replace(_T(".mdb") , _T("tmp.mdb"));
// #ifdef DEBUG
// 	acutPrintf(_T("\n当前的路径 %s") , str);
// #endif
	if(CopyFile(m_szMdbPath , str , false) == FALSE)
		return false;
	//执行sql
	if(false == excute_sql(connect , m_str_sql_array))
		return false;
	_stprintf(szPath , _T("%s") , str);
	return true;
}
