#ifndef _EXPORT_REPORT_FACTORY_
#define _EXPORT_REPORT_FACTORY_
#include "AdoConnection.h"

//获得临时数据库
class IReport
{
public: 
	virtual void set_mdb_path(TCHAR *szMdbPath){};//数据库的路径
	virtual bool get_temp_mdb_path(TCHAR *szPath , int nIndex){return true;};//获得临时数据库 返回临时数据库的路径
	virtual bool get_temp_mdb_path(TCHAR *szPath,CAdoConnection & connect){return true;};
	virtual void set_sql_string(const CStringArray & strsql){return;};//设置多条查询语句
	virtual bool excute_sql(CString strPath , const CStringArray &strArray) {return true;};
	virtual bool excute_sql(CAdoConnection & connect , const CStringArray & strArray){return true;};
};
//获得临时数据库的路径
class Land_survey_report:public IReport
{
public:
	virtual void set_mdb_path(TCHAR *szMdbPath);
	virtual bool get_temp_mdb_path(TCHAR *szPath , int nIndex);//获得临时数据库
	virtual bool get_temp_mdb_path(TCHAR *szPath,CAdoConnection & connect);
	virtual void set_sql_string(const CStringArray & strsql);
	virtual bool excute_sql(CAdoConnection & connect , const CStringArray & strArray);
	virtual bool excute_sql(CString strPath , const CStringArray &strArray);


private:
	TCHAR m_szMdbPath[MAX_PATH] ; //总数据源的路径
	CStringArray m_str_sql_array;//当前的查询语句
};
//报表生成工厂
class CExportReportFactory
{
public:
	CExportReportFactory(void);
	~CExportReportFactory(void);
	CString get_temp_mdb_path(TCHAR *szMdbPath , const CStringArray & strSqlArray , int nIndex = 1);//获得临时库的路径
	CString get_temp_mdb_path(CAdoConnection &connect,TCHAR *szMdbPath , const CStringArray & strSqlArray , int nIndex);//获得临时库的路径
};
#endif
