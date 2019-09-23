#include "stdafx.h"
#include "ExportPipeInfo.h"

bool		CExportPipeInfo::replacePipePointType()
{
	CString strSQL = _T("SELECT �ܵ����ͱ���, ���������ͱ��� FROM ���ߵ�ɹ���");
	_RecordsetPtr res;
	CAdoRecordSet rectSet(&m_conn);
	if(FALSE == rectSet.Open(strSQL))
	{
		ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);
		return false;
	}

	set<CString> set1, set2;
	CString strValue;
	rectSet.MoveFirst();
	while(!rectSet.IsEOF())
	{
		rectSet.GetCollect(_T("�ܵ����ͱ���"), strValue);
		if(strValue != _T(""))
			set1.insert(strValue);			
// 		else
// 			ads_printf(_T("\n һ���յĹܵ����ͱ��룡"));

		rectSet.GetCollect(_T("���������ͱ���"), strValue);
		if(strValue != _T(""))
			set2.insert(strValue);			
// 		else
// 			ads_printf(_T("\n һ���յĸ��������ͱ��룡"));
		
		rectSet.MoveNext();

	}
	rectSet.Close();	

	set<CString>::iterator it;
	vector<CString> vStr1, vStr2;
	for(it = set1.begin(); it != set1.end(); it ++)
	{
		bool bFind = false;
		for(int i = 0; i < gPipeBlock.GetSize(); i ++)
		{
			PIPEBLOCK pBlock = gPipeBlock.GetAt(i);
			if(0 == _tcscmp(*it, pBlock.block))
			{
				vStr1.push_back(pBlock.name);
				bFind = true;
				break;
			}

		}
		if(bFind == false)
		{
			vStr2.push_back(_T(""));
			ads_printf(_T("\n  �ܵ����ͱ��� %s �Ҳ�����Ӧ�Ĺܵ��������ƣ� casspipe.mdb���symbol��"), *it);
		}
	}

	for(it = set2.begin(); it != set2.end(); it ++)
	{
		bool bFind = false;
		for(int i = 0; i < gPipeBlock.GetSize(); i ++)
		{
			PIPEBLOCK pBlock = gPipeBlock.GetAt(i);
			if(0 == _tcscmp(*it, pBlock.block))
			{
				vStr2.push_back(pBlock.name);
				bFind = true;
				break;
			}

		}
		if(bFind == false)
		{
			vStr2.push_back(_T(""));
			ads_printf(_T("\n  ���������ͱ��� %s �Ҳ�����Ӧ�Ĺܵ��������ƣ� casspipe.mdb���symbol��"), *it);
		}
	}

	if(set1.size() != vStr1.size() || set2.size() != vStr2.size()) return false;

	//�����ԭ�е�����
	strSQL = _T("UPDATE ���ߵ�ɹ��� SET �ܵ����� = '', ������ = ''");
	res = m_conn.Execute(strSQL);
	if(res == NULL)
		ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);

	it = set1.begin();
	vector<CString>::iterator itNew = vStr1.begin();
	for(; it != set1.end(); it ++, itNew++)
	{
		if((*itNew) == _T("")) continue;

		strSQL.Format(_T("UPDATE ���ߵ�ɹ��� SET �ܵ����� = '%s' WHERE �ܵ����ͱ��� = '%s'"), *itNew, *it);
		res = m_conn.Execute(strSQL);
		if(res == NULL)
			ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);
	}

	it = set2.begin();
	itNew = vStr2.begin();
	for(; it != set2.end(); it ++, itNew++)
	{
		if((*itNew) == _T("")) continue;

		strSQL.Format(_T("UPDATE ���ߵ�ɹ��� SET ������ = '%s' WHERE ���������ͱ��� = '%s'"), *itNew, *it);
		res = m_conn.Execute(strSQL);
		if(res == NULL)
			ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);
	}

	return true;
}


bool		CExportPipeInfo::getData(AcDbObjectId objPP, AcDbObjectIdArray objArrPl)
{
	_RecordsetPtr res;
	CString strField1, strField2;
	
	for(int i = 0; i < objArrPl.length(); i ++)
	{
		CString strSQL = _T("INSERT INTO ���ߵ�ɹ��� VALUES(");
		AcDbObjectId objPL = objArrPl.at(i);

		for(size_t t = 0; t < m_vFieldText.size(); t ++)
		{
			CString strField = m_vFieldsName.at(t);
			CString strText = m_vFieldText.at(t);
			short		sType = m_vFieldsType.at(t);
			short		sPipeType = m_vFieldPipeType.at(t);

			TCHAR szValue[500] = {0};
			if(strField == _T("���ߴ���(�ڲ��ֶ�)")) 
			{
				_tcscpy(szValue, m_pipeCur.brief);
			}
			else if(strField == _T("����С��(�ڲ��ֶ�)"))
			{
				_tcscpy(szValue, m_pipeCur.brief);
			}
			else
			{
				if(sPipeType == kPipePoint)
				{
					ReadXdata(aname(objPP), strField, sType, szValue);

					if(strField == g_myKeyAppname.szPpMapNumber || strField == g_myKeyAppname.szPpInerNumber)
					{
						if(i > 0) _tcscpy(szValue, _T(""));
					}
				}
				else
				{
					ReadXdata(aname(objPL), strField, sType, szValue);

					if(strField == g_myKeyAppname.szPlEndNum)
					{
						TCHAR szExpNo[200] = {0};
						ReadXdata(aname(objPP), g_myKeyAppname.szPpMapNumber, sType, szExpNo);
						if(0 == _tcscmp(szExpNo, szValue))
						{
							ReadXdata(aname(objPL), g_myKeyAppname.szPlStartNum, sType, szValue);
						}
					}
				}

			}

			TCHAR szTemp[500] = {0};
			if(sType == INT_TYPE)
			{
				if(0 != _tcscmp(szValue, _T("")))
				{
					if(_ttoi(szValue) == 0)
						_tcscpy(szTemp, _T("0"));
					else
						_stprintf(szTemp, _T("%s"), szValue);
				}
				else
					_tcscpy(szTemp, _T("NULL"));

			}
			else if(sType == LONG_TYPE)
			{
				if(0 != _tcscmp(szValue, _T("")))
				{
					if(_ttol(szValue) == 0)
						_tcscpy(szTemp, _T("0"));
					else
						_stprintf(szTemp, _T("%s"), szValue);
				}
				else
					_tcscpy(szTemp, _T("NULL"));
			}
			else if(sType == DOUBLE_TYPE)
			{
				if(0 != _tcscmp(szValue, _T("")))
				{
					if(_tstof(szValue) == 0)
						_tcscpy(szTemp, _T("0.0"));
					else
						_stprintf(szTemp, _T("%s"), szValue);
				}
				else
					_tcscpy(szTemp, _T("NULL"));
			}
			else
				_stprintf(szTemp, _T("'%s'"), szValue);

			if(t != m_vFieldText.size() - 1)
				_tcscat(szTemp, _T(","));
		
				
			strSQL += szTemp;

		}

		strSQL += _T(")");
		res = m_conn.Execute(strSQL);
		if(res == NULL)
		{
			ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);
			continue;
		}
	}


	return true;
}


bool	CExportPipeInfo::exportInfoToMdb()
{
	//���������
	m_conn.Execute(_T("DELETE * FROM ���ߵ�ɹ���"));

	acedSetStatusBarProgressMeter( _T("��ȡ���������С�����"), 0, gPipeType.GetSize());

	for(int i = 0; i < gPipeType.GetSize(); i ++)
	{
		acedSetStatusBarProgressMeterPos(i);

		m_pipeCur = gPipeType.GetAt(i);

		TCHAR szLyr[200] = {0};
		_stprintf(szLyr, _T("GX_%sP"), m_pipeCur.brief);

		AcDbObjectIdArray objArrPoint, objArrAttach, objArr;
		CSelect selectT;
		selectT.SelectObjects2(objArrPoint, 1, _T("*INSERT*"), szLyr);

		_stprintf(szLyr, _T("GX_%sD"), m_pipeCur.brief);
		selectT.SelectObjects2(objArrAttach, 1, _T("*INSERT*"), szLyr);

		objArr.append(objArrPoint);
		objArr.append(objArrAttach);

		if(objArr.length() == 0) continue;

		for(int j = 0; j < objArr.length(); j ++)
		{
			AcDbObjectId objPP = objArr.at(j);
			if(isPipePoint(objPP) == false) continue;

			AcDbObjectIdArray      aLine;//��ȡ���ӹ���Id����
			if(!GetConnPipeLinesByPipePoint(objPP,aLine)) continue;

			getData(objPP, aLine);

		}
	}

	acedRestoreStatusBar();

	return true;
}


bool  CExportPipeInfo::invokeFastReport()
{
	ads_printf(_T("\n\n ����excel�С�����\n"));
	vector<CString> vect_data_list1;

	vector<CString> vect_temp_mdb , vect_temp_template , vect_out_path;//�ֱ�����ʱ���ݿ� ��ʱģ�� ���·��
	m_strParam = _T("excel");

	TCHAR szMdbPath[1000] = {0};
	ads_findfile(_T("Pipe.mdb"), szMdbPath);
	vect_temp_mdb.push_back(szMdbPath);
	if(0 == _tcscmp(szMdbPath, _T("")))
	{
		ads_printf(_T("\n �Ҳ����ļ� ��Pipe.mdb"));
		return false;
	}

	TCHAR szReportTempPath[1000] = {0};
	TCHAR szTempName[] =  _T("���ߵ�ɹ���.frx");

	ads_findfile(szTempName, szReportTempPath);
	vect_temp_template.push_back(szReportTempPath);
	if(0 == _tcscmp(szReportTempPath, _T("")))
	{
		ads_printf(_T("\n �Ҳ����ļ� ��%s"), szTempName);
		return false;
	}

	if(m_strSavePath == _T("")) return false;

	vect_out_path.push_back(m_strSavePath);

	TCHAR szExe_path[1000] = {0};
	ads_findfile(_T("ExportReport.exe"), szExe_path);
	if(0 == _tcscmp(szExe_path, _T("")))
	{
		ads_printf(_T("\n �Ҳ����ļ� ��ExportReport.exe"));
		return false;
	}

	if(export_report_by_temp_mdb_multiply(vect_temp_mdb , vect_temp_template , vect_out_path , szExe_path , 12 , true) == false)
	{
		AfxMessageBox(_T("���ʧ��"));
	}

	return true;
}

bool		CExportPipeInfo::getFieldsAndTypes()
{
	if (!m_conn.ConnectAccess(m_szMdbPath))
	{
		CoUninitialize();
		return false;
	}

	CAccessProcess accessP;
	accessP.SetConnection(&m_conn);
	accessP.GetAllFieldsName(_T("���ߵ�ɹ���"), m_vFieldText);
	if(m_vFieldText.size() == 0) return false;

	//��صİ취��ֱ���ҵ��ֶε�����Ϊֹ
	for(size_t t = 0; t < m_vFieldText.size(); t ++)
	{
		bool bFind = false;
		CString str = m_vFieldText.at(t);
		m_vFieldsName.push_back(str);

		//�����ֶ����⴦��
		if(str == _T("���ߴ���(�ڲ��ֶ�)") || str == _T("����С��(�ڲ��ֶ�)"))
		{
			m_vFieldsType.push_back(EnumCollection::eString);
			m_vFieldPipeType.push_back(kPipePoint);
			continue;
		}

		//����һЩ�̶����ֶ����ͣ�ֱ��д��
		if(str == g_myKeyAppname.szPpAuxiliary || str ==  g_myKeyAppname.szPpCharacter)
		{
			m_vFieldsType.push_back(EnumCollection::eString);
			m_vFieldPipeType.push_back(kPipePoint);
			continue;
		}

		//������casspipe.mdb�л�ȡ
		for(int i = 0; i < gPipeType.GetSize(); i ++)
		{
			PIPETYPE pipeT = gPipeType.GetAt(i);

			CStringArray propLst; 
			CUIntArray dataTypLst; 
			CUIntArray ctrlTypLst; 
			GetPipePropertiesList(pipeT.code, propLst, dataTypLst, ctrlTypLst, kPipePoint);

			for(int j = 0; j < propLst.GetSize(); j ++)
			{
				CString ss = propLst.GetAt(j);
				if(0 == _tcscmp(str, propLst.GetAt(j)))
				{
					m_vFieldsType.push_back(dataTypLst.GetAt(j));
					m_vFieldPipeType.push_back(kPipePoint);
					bFind = true;
					break;
				}
			}

			if(bFind) break;

			GetPipePropertiesList(pipeT.code, propLst, dataTypLst, ctrlTypLst, kPipeLine);
			for(int j = 0; j < propLst.GetSize(); j ++)
			{
				CString ss = propLst.GetAt(j);

				if(0 == _tcscmp(str, propLst.GetAt(j)))
				{
					m_vFieldsType.push_back(dataTypLst.GetAt(j));
					m_vFieldPipeType.push_back(kPipeLine);
					bFind = true;
					break;
				}
			}

			if(bFind) break;

		}

		if(bFind == false)
		{
			ads_printf(_T("\n �����ֶ�����ʧ�ܣ� Pipe.mdb: %s"), str);
		}
	}

	if(m_vFieldText.size() != m_vFieldsName.size()
		|| m_vFieldsName.size() != m_vFieldPipeType.size()
		|| m_vFieldPipeType.size() != m_vFieldsType.size())
		return false;

	//�ѹ����ڲ��ֶ����͸ĳ�CASS�ֶ�����
	for(size_t t = 0; t < m_vFieldsType.size(); t ++)
	{
		if(m_vFieldsType.at(t) == EnumCollection::eInt)
			m_vFieldsType.at(t) = LONG_TYPE;
		else if(m_vFieldsType.at(t) == EnumCollection::eShortInt)
			m_vFieldsType.at(t) = INT_TYPE;
		else if(m_vFieldsType.at(t) == EnumCollection::eDouble || m_vFieldsType.at(t) == EnumCollection::eFloat)
			m_vFieldsType.at(t) = DOUBLE_TYPE;
		else
			m_vFieldsType.at(t) = STRING_TYPE;
	}

	return true;
}

bool		CExportPipeInfo::exportPipeResultTable()
{
	resbuf rc;
	if(ads_getfiled(_T("���������Ϣ�ɹ���"), NULL, _T("xlsx"), 1,&rc) != RTNORM) return false;
	m_strSavePath= rc.resval.rstring;

	ads_findfile(_T("Pipe.mdb"), m_szMdbPath);
	if(0 == _tcscmp(m_szMdbPath, _T("")))
	{
		ads_printf(_T("\n �Ҳ���ģ���ļ���Pipe.mdb"));
		return false;
	}

	CAcModuleResourceOverride     myResources;

	CUserDataDlg userDlg;
	if(userDlg.DoModal() != IDOK)
	{
		m_conn.Close();
		return false;
	}


	if(false == getFieldsAndTypes())
	{
		m_conn.Close();
		return false;
	}

	if(false == exportInfoToMdb()) return false;

	replacePipePointType();

	m_conn.Close();

	if(false == invokeFastReport()) return false;

	ads_printf(_T("\n\n �����ɣ�\n"));

	if( IDYES == AfxMessageBox(_T("������ϣ��Ƿ����ڴ򿪣�") , MB_YESNO) )
	{
		ShellExecute(adsw_hwndAcad,_T("open"), m_strSavePath, _T(""),NULL,SW_SHOWNORMAL);
	}

	return true;
}


//�ö��߳����������
//�����ֱ��� ���ݿ�·�� ģ��·�� ���λ�� exe·�� ���ɵ��̵߳�����
//����������߳�������12��
// you_see_is_you_get�Ƿ�����������
bool CExportPipeInfo::export_report_by_temp_mdb_multiply(const vector<CString> & mdbPathArray , const vector<CString> & templatePathArray, const vector<CString> &outPathArray , TCHAR *szExePath , int nCount,bool you_see_is_you_get)
{
	nCount = 10;
	CString str_out_type = _T("true");
	if(you_see_is_you_get == false)
		str_out_type = _T("false");	

	for (long i = 0 ; i < (long)mdbPathArray.size() ; i += nCount)
	{

		long nStart = i , nEnd = i + nCount -1;
		if(nEnd >= (long)mdbPathArray.size())
			nEnd = mdbPathArray.size() - 1;
		HANDLE threadHandle[10];
		HANDLE processHandle[10];
		long nIndex = 0;
		for (long j = nStart  ; j <= nEnd ; j++)
		{
			CString strPath1 = mdbPathArray.at( j ),strPath2 = templatePathArray.at( j ), strPath3 = outPathArray.at( j );
			strPath1.Replace(_T(' ') , _T('?'));
			strPath2.Replace(_T(' ') , _T('?'));
			strPath3.Replace(_T(' ') , _T('?'));
			TCHAR szParam[2000] = {0};
			_stprintf(szParam, _T(" %s %s %s %s %s"),strPath1,strPath2,strPath3, m_strParam,str_out_type);
			PROCESS_INFORMATION ProcessInfo; 
			STARTUPINFO StartupInfo;//This is an [in] parameter
			ZeroMemory(&StartupInfo,sizeof(StartupInfo));
			StartupInfo.cb = sizeof StartupInfo ;  //Only compulsory field
			if(CreateProcess(szExePath, szParam, NULL,NULL,FALSE,0,NULL, NULL,&StartupInfo,&ProcessInfo))
			{
				processHandle[nIndex] = ProcessInfo.hProcess;
				threadHandle[nIndex] = ProcessInfo.hThread;
				nIndex++;
			} 
			else
			{
				continue;
			}
		}
		if(nIndex > 0)
		{
			//��Ҫע�������������ĵ�һ�����������ǵڶ��������ĳ��ȣ�������ǵڶ��������ĳ��ȣ��᲻�ȵ��ڶ������������е��߳�
			//ִ�н����ͷ���
			WaitForMultipleObjects(nIndex , processHandle , TRUE , INFINITE);//�ȵ����е��߳̽��� 
		}
	}

	return true;
}

CExportPipeInfo::CExportPipeInfo(void)
{
	_tcscpy(m_szMdbPath, _T(""));
}


CExportPipeInfo::~CExportPipeInfo(void)
{
}
