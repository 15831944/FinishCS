#ifndef _EXPORT_REPORT_FACTORY_
#define _EXPORT_REPORT_FACTORY_
#include "AdoConnection.h"

//�����ʱ���ݿ�
class IReport
{
public: 
	virtual void set_mdb_path(TCHAR *szMdbPath){};//���ݿ��·��
	virtual bool get_temp_mdb_path(TCHAR *szPath , int nIndex){return true;};//�����ʱ���ݿ� ������ʱ���ݿ��·��
	virtual bool get_temp_mdb_path(TCHAR *szPath,CAdoConnection & connect){return true;};
	virtual void set_sql_string(const CStringArray & strsql){return;};//���ö�����ѯ���
	virtual bool excute_sql(CString strPath , const CStringArray &strArray) {return true;};
	virtual bool excute_sql(CAdoConnection & connect , const CStringArray & strArray){return true;};
};
//�����ʱ���ݿ��·��
class Land_survey_report:public IReport
{
public:
	virtual void set_mdb_path(TCHAR *szMdbPath);
	virtual bool get_temp_mdb_path(TCHAR *szPath , int nIndex);//�����ʱ���ݿ�
	virtual bool get_temp_mdb_path(TCHAR *szPath,CAdoConnection & connect);
	virtual void set_sql_string(const CStringArray & strsql);
	virtual bool excute_sql(CAdoConnection & connect , const CStringArray & strArray);
	virtual bool excute_sql(CString strPath , const CStringArray &strArray);


private:
	TCHAR m_szMdbPath[MAX_PATH] ; //������Դ��·��
	CStringArray m_str_sql_array;//��ǰ�Ĳ�ѯ���
};
//�������ɹ���
class CExportReportFactory
{
public:
	CExportReportFactory(void);
	~CExportReportFactory(void);
	CString get_temp_mdb_path(TCHAR *szMdbPath , const CStringArray & strSqlArray , int nIndex = 1);//�����ʱ���·��
	CString get_temp_mdb_path(CAdoConnection &connect,TCHAR *szMdbPath , const CStringArray & strSqlArray , int nIndex);//�����ʱ���·��
};
#endif
