#include "stdafx.h"
#include "ListItemsMdb.h"

bool CListItemsMdb::readLstitem(const TCHAR *szName)
{

	//每种CASS编码对应实体的显示
	TCHAR szPath[1024] = {0},szValue[1024] = {0};
	szPath[0]=0;
	acedFindFile(szName, szPath);
	CoInitialize(NULL);
	CAdoConnection adoConnection;

	if (!adoConnection.ConnectAccess(szPath))
	{
		AfxMessageBox(_T("连接数据库失败!"));
		return false;
	}

	CAdoRecordSet rset;
	_bstr_t Value;
	CString strSQL,strTablename;CString strError;
	int nCount = 0;
	try
	{
		if (adoConnection.GetConnection()->State != adStateOpen) return false;
		rset = adoConnection.OpenSchema (adSchemaColumns);
		while (!rset.IsEOF())
		{
			CString strValue;
			//rset.GetValueString(strValue, "TABLE_NAME");
			rset.GetCollect(_T("TABLE_NAME"), strValue);
			if (strValue != strTablename)
			{
				strTablename = strValue;
				if ( _tcsnicmp(strValue,_T("msys"),4) == 0)
				{
					rset.MoveNext();
					continue;
				}
				strSQL.Format(_T("select * from %s"),strTablename);
				CAdoRecordSet rsetIn;
				rsetIn.SetAdoConnection(&adoConnection);

				nCount++;
				int nResult = rsetIn.Open(strSQL, adCmdText);
				if ( nResult == -1)
				{
					//strError = adoConnection.GetLastError();
					strError = adoConnection.GetLastErrorText();
					AfxMessageBox(strError);
					return false;
				}
				else if (nResult == 0)
				{
					//没有记录的表
					rset.MoveNext();
					//acutPrintf(_T("\n空表 %d %s"),nCount,strTablename);
					continue;
				}

				int nColCount=rsetIn.GetFields()->Count;

				int i=0;CStringArray saField;bool bHaveCassCode = false;
				CString strFieldName;

				FieldDict2 dict;
				_tcscpy(dict.szDictName,strTablename);

				//读取数据
				CString strCode, strValue, strData;
				while (!rsetIn.IsEOF())
				{
					strValue.Empty();
					//rsetIn.GetValueString(strValue,_T("CassCode"));
					rsetIn.GetCollect(_T("code"), strCode);
					strCode.TrimLeft();
					strCode.TrimRight();
					rsetIn.GetCollect(_T("value"), strValue);
					strValue.TrimLeft();
					strValue.TrimRight();

					if ( _tcslen(strCode) > 0 && _tcslen(strValue) > 0 )
					{
						//dict.strData += (CString(strCode) + _T(" ")) + CString(strValue) + _T(",");
						strData.Format(_T("%s %s"), strCode, strValue);
						dict.vRecord.push_back(strData);
					}

					rsetIn.MoveNext();
				}

				if (dict.vRecord.size()) 
				{
					
					m_vFieldDicts.push_back(dict);
				}
			}

			rset.MoveNext();
		}
	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("在提取表名，字段时出错"));
		return false;
	}
	CoUninitialize();

	sort(m_vFieldDicts.begin(),m_vFieldDicts.end(),FieldDictLess());

	return true;
}


CListItemsMdb::CListItemsMdb(void)
{
}


CListItemsMdb::~CListItemsMdb(void)
{
}
