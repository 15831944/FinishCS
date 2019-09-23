#include "stdafx.h"
#include "ExportMdbInfo.h"

//�˲��ֵĹ���Ϊ������ĸ������������´򿪣�Ȼ��ϲ���һ�����У�
//��ΪҪ����com�ӿڣ��ٶ��������Ҽ�������������ѷ������ַ����ˡ�����
bool		CExportMdbInfo::mergeExcelColumns()
{
	acedSetStatusBarProgressMeter(_T("�ϲ�excel�����..."), 0, (int)m_vect_out_path.size());

	for(int t = 0; t < (int)m_vect_out_path.size(); t++)
	{
		acedSetStatusBarProgressMeterPos(t);

		CString strPath = m_vect_out_path.at(t);
		if (IsPathFileExists(strPath) == FALSE) continue;

		CTinyExcel excel;
		excel.SetTemplete(strPath);

		if (false == excel.StartExcel()) continue;
		//excel.Show();
		int nRow = 0, nCol = 0, nRow2 = 0, nCol2 = 0;
		if (false == excel.FindString(_T("����"), nRow, nCol)) continue;
		if (false == excel.FindString(_T("������Ա"), nRow2, nCol2)) continue;
		nRow2--;

		vector<int> vInt;
		excel.MergeSameStringNeightbourCell2(nRow + 1, nCol, nRow2, vInt, excel.m_range, false);



		for (size_t t = 0; t < vInt.size(); t++)
		{
			TCHAR szNum[20] = {0};
			_stprintf(szNum, _T("%d"), t + 1);
			excel.SetRangeContent(vInt.at(t), 1, szNum, excel.m_range);
			if (t != vInt.size() - 1)
				excel.MergeRange(vInt.at(t), 1, vInt.at(t + 1) - 1, 1, excel.m_range);
			else
				excel.MergeRange(vInt.at(t), 1, nRow2, 1, excel.m_range);

		}

		strPath.Replace(_T("���ϸ���������"), _T("����������"));
		excel.SaveAsWorkBook(excel.m_workbook, strPath);
		excel.CloseWorkBook(excel.m_workbook);
	}

	acedRestoreStatusBar();

	for (size_t t = 0; t < m_vect_out_path.size(); t++)
		DeleteFile(m_vect_out_path.at(t));


	return true;
}


bool  CExportMdbInfo::invokeFastReport()
{
	ads_printf(_T("\n\n ����excel�С�����\n"));

	vector<CString> vect_temp_mdb, vect_temp_template;//�ֱ�����ʱ���ݿ� ��ʱģ�� ���·��
	m_strParam = _T("excel");

	if (m_strSavePath == _T("")) return false;

	vector<CString> vect_data_list1;

	if (m_sReportType == IndoorReport_GroundAttachmentsTable_standard2)
	{
		CString strSQL = _T("SELECT ͼ���� from ���ϸ��������� group by ͼ����");
		if (!m_conn.ConnectAccess(m_szMdbPath))
		{
			ads_printf(_T("\n ��ģ�����ݿ�ʧ�ܣ� %s"), m_szMdbPath);
			CoUninitialize();
			return false;
		}

		CAdoRecordSet rSet;
		rSet.SetAdoConnection(&m_conn);
		if (FALSE == rSet.Open(strSQL, adCmdText)) return false;

		while (!rSet.IsEOF())
		{
			TCHAR szValue[200] = {0};
			rSet.GetCollect(_T("ͼ����"), szValue);

			vect_data_list1.push_back(szValue);

			rSet.MoveNext();
		}
		rSet.Close();

		CExportReportFactory TempFactory;
		for (int i = 0; i < (int)vect_data_list1.size(); i++)
		{
			CString strValue = vect_data_list1.at(i);
			CStringArray str_sql_array;
			strSQL.Format(_T("delete from ���ϸ��������� where ͼ���� <> '%s'"), strValue);
			str_sql_array.Add(strSQL);

			TCHAR    mdb_path[MAX_PATH] = {0};
			_tcscpy(mdb_path, m_szMdbPath);
			CString strPath = TempFactory.get_temp_mdb_path(m_conn, mdb_path, str_sql_array, i);
			if (strPath.GetLength() <= 0)
				continue;

			//������ʱģ��
			CString strTemplate = m_szFRXTempName;
			CString strTemp;
			strTemp.Format(_T("%d.frx"), i);
			strTemplate.Replace(_T(".frx"), strTemp);
			//����ģ��
			if (CopyFile(m_szFRXTempName, strTemplate, false) == FALSE)
			{
				AfxMessageBox(_T("\n����ģ��ʧ��"));
				continue;
			}
			vect_temp_mdb.push_back(strPath);//������ʱ���ݿ�
			vect_temp_template.push_back(strTemplate);
			TCHAR szOutPath[MAX_PATH] = {0};

			_stprintf(szOutPath, _T("%s_%s.xlsx"), m_strSavePath, strValue);
			m_vect_out_path.push_back(szOutPath);
		}
	}
	else if (m_sReportType == IndoorReport_OwnerInfoSummaryTable_standard2)
	{
		vect_temp_mdb.push_back(m_szMdbPath);
		vect_temp_template.push_back(m_szFRXTempName);
		m_vect_out_path.push_back(m_strSavePath);
	}

	if (export_report_by_temp_mdb_multiply(vect_temp_mdb, vect_temp_template, m_vect_out_path, m_szExe_path, 12, true) == false)
	{
		AfxMessageBox(_T("���ʧ��"));
	}

	return true;
}

//�ö��߳����������
//�����ֱ��� ���ݿ�·�� ģ��·�� ���λ�� exe·�� ���ɵ��̵߳�����
//����������߳�������12��
// you_see_is_you_get�Ƿ�����������
bool CExportMdbInfo::export_report_by_temp_mdb_multiply(const vector<CString> & mdbPathArray, const vector<CString> & templatePathArray, const vector<CString> &outPathArray, TCHAR *szExePath, int nCount, bool you_see_is_you_get)
{
	nCount = 10;
	CString str_out_type = _T("true");
	if (you_see_is_you_get == false)
		str_out_type = _T("false");

	for (long i = 0; i < (long)mdbPathArray.size(); i += nCount)
	{

		long nStart = i, nEnd = i + nCount -1;
		if (nEnd >= (int)mdbPathArray.size())
			nEnd = (int)mdbPathArray.size() - 1;
		HANDLE threadHandle[10];
		HANDLE processHandle[10];
		long nIndex = 0;
		for (long j = nStart; j <= nEnd; j++)
		{
			CString strPath1 = mdbPathArray.at(j), strPath2 = templatePathArray.at(j), strPath3 = outPathArray.at(j);
			strPath1.Replace(_T(' '), _T('?'));
			strPath2.Replace(_T(' '), _T('?'));
			strPath3.Replace(_T(' '), _T('?'));
			TCHAR szParam[2000] = {0};
			_stprintf(szParam, _T(" %s %s %s %s %s"), strPath1, strPath2, strPath3, m_strParam, str_out_type);
			PROCESS_INFORMATION ProcessInfo;
			STARTUPINFO StartupInfo;//This is an [in] parameter
			ZeroMemory(&StartupInfo, sizeof(StartupInfo));
			StartupInfo.cb = sizeof StartupInfo;  //Only compulsory field

			//ads_printf(_T("\n %s            %s\n"), szExePath, szParam);

			if (CreateProcess(szExePath, szParam, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
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
		if (nIndex > 0)
		{
			//��Ҫע�������������ĵ�һ�����������ǵڶ��������ĳ��ȣ�������ǵڶ��������ĳ��ȣ��᲻�ȵ��ڶ������������е��߳�
			//ִ�н����ͷ���
			WaitForMultipleObjects(nIndex, processHandle, TRUE, INFINITE);//�ȵ����е��߳̽��� 
		}
	}

	return true;
}

bool	CExportMdbInfo::exportOwnerInfoSummaryTable_Standard2()
{
	ads_findfile(_T("ExportReport.exe"), m_szExe_path);
	if (0 == _tcscmp(m_szExe_path, _T("")))
	{
		ads_printf(_T("\n �Ҳ����ļ� ��ExportReport.exe"));
		return false;
	}

	int nRes = ads_findfile(_T("�뻧����.accdb"), m_szMdbPath);
	if (0 == _tcslen(m_szMdbPath) || nRes != RTNORM)
	{
		ads_printf(_T("\n ���Բ��� �뻧����.mdb"));
		ads_findfile(_T("�뻧����.mdb"), m_szMdbPath);
	}

	ads_findfile(_T("���ϸ����������ܱ�.frx"), m_szFRXTempName);
	if (0 == _tcscmp(m_szFRXTempName, _T("")))
	{
		ads_printf(_T("\n �Ҳ����ļ� �����ϸ����������ܱ�.frx"));
		return false;
	}

	m_sReportType = IndoorReport_OwnerInfoSummaryTable_standard2;

	if (false == exportInfoToMdb()) return false;

	if (false == invokeFastReport()) return false;

	ads_printf(_T("\n\n �����ɣ�\n"));

	if (IDYES == AfxMessageBox(_T("������ϣ��Ƿ����ڴ򿪣�"), MB_YESNO))
	{
		ShellExecute(adsw_hwndAcad, _T("open"), m_strSavePath, _T(""), NULL, SW_SHOWNORMAL);
	}

	return true;
}


bool	CExportMdbInfo::exportGroundAttachmentsTable_Standard2()
{
	ads_findfile(_T("ExportReport.exe"), m_szExe_path);
	if (0 == _tcscmp(m_szExe_path, _T("")))
	{
		ads_printf(_T("\n �Ҳ����ļ� ��ExportReport.exe"));
		return false;
	}

	int nRes = ads_findfile(_T("�뻧����.accdb"), m_szMdbPath);
	if (0 == _tcslen(m_szMdbPath) || nRes != RTNORM)
	{
		ads_printf(_T("\n ���Բ��� �뻧����.mdb"));
		ads_findfile(_T("�뻧����.mdb"), m_szMdbPath);
	}

	if (0 == _tcscmp(m_szMdbPath, _T("")))
	{
		ads_printf(_T("\n �Ҳ���ģ���ļ����뻧����.mdb"));
		return false;
	}
	ads_findfile(_T("���ϸ��������ǼǱ�.frx"), m_szFRXTempName);
	if (0 == _tcscmp(m_szFRXTempName, _T("")))
	{
		ads_printf(_T("\n �Ҳ����ļ� �����ϸ��������ǼǱ�.frx"));
		return false;
	}

	m_sReportType = IndoorReport_GroundAttachmentsTable_standard2;

	if (false == exportInfoToMdb()) return false;

	if (false == invokeFastReport()) return false;

	//mergeExcelColumns();

	ads_printf(_T("\n\n �����ɣ�\n"));

	if (IDYES == AfxMessageBox(_T("������ϣ��Ƿ����ڴ򿪣�"), MB_YESNO))
	{
		ShellExecute(adsw_hwndAcad, _T("open"), m_strSaveFlolder, _T(""), NULL, SW_SHOWNORMAL);
	}

	return true;
}

bool		CExportMdbInfo::readSortAndClassInfo()
{
	m_vSortAndClass.clear();

	CAdoRecordSet rsetIn;
	rsetIn.SetAdoConnection(&m_conn);

	int nResult = rsetIn.Open(_T("SELECT * FROM ����_����_������ձ�"), adCmdText);
	if (nResult == -1)
		return false;

	//��ȡ����
	CString  strValue;
	while (!rsetIn.IsEOF())
	{
		SSortAndClass sortClass;

		rsetIn.GetCollect(_T("����"), sortClass.nSort);
		rsetIn.GetCollect(_T("����"), strValue);
		strValue.Trim();
		_tcscpy(sortClass.szClass, strValue);
		rsetIn.GetCollect(_T("����"), strValue);
		strValue.Trim();
		_tcscpy(sortClass.szCodes, strValue);

		m_vSortAndClass.push_back(sortClass);

		rsetIn.MoveNext();
	}

	rsetIn.Close();
	return true;
}

bool		CExportMdbInfo::readUnitsFromTable()
{
	m_vUnitAndCodes.clear();

	CAdoRecordSet rsetIn;
	rsetIn.SetAdoConnection(&m_conn);

	int nResult = rsetIn.Open(_T("SELECT * FROM ��λ_������ձ�"), adCmdText);
	if (nResult == -1)
		return false;

	//��ȡ����
	CString  strValue;
	while (!rsetIn.IsEOF())
	{
		SUnitAndCode unitCodes;

		rsetIn.GetCollect(_T("��λ"), strValue);
		strValue.Trim();
		_tcscpy(unitCodes.szUnit, strValue);

		rsetIn.GetCollect(_T("����"), unitCodes.strCodes);

		m_vUnitAndCodes.push_back(unitCodes);

		rsetIn.MoveNext();
	}

	rsetIn.Close();
	return true;
}

bool		CExportMdbInfo::getSortAndClass(const TCHAR *szCode, int &nSort, TCHAR *szClass)
{
	for (vector<SSortAndClass>::iterator it = m_vSortAndClass.begin(); it != m_vSortAndClass.end(); it++)
	{
		CString strText = it->szCodes;
		if (strText.Find(szCode) >= 0)
		{
			nSort = it->nSort;
			_tcscpy(szClass, it->szClass);
			return true;
		}

	}
	return false;
}

bool		CExportMdbInfo::getUnits(const TCHAR *szCode, TCHAR *szUnit)
{
	for (vector<SUnitAndCode>::iterator it = m_vUnitAndCodes.begin(); it != m_vUnitAndCodes.end(); it++)
	{
		if (it->strCodes.Find(szCode) >= 0)
		{
			_tcscpy(szUnit, it->szUnit);
			return true;
		}

	}
	return false;
}

bool	CExportMdbInfo::exportInfoToMdb_GroundAttachmentsTableSummary()
{
	vector<AcDbObjectIdArray> vObjArrFrame;
	CExportSummaryTable exportSummary;
	exportSummary.m_sReportType = m_sReportType;
	if (false == exportSummary.getAndFilterFrame_Indoor(vObjArrFrame)) return false;
	m_strSavePath.Format(_T("%s���ϸ�����������"), exportSummary.m_strSavePath);

	if (!m_conn.ConnectAccess(m_szMdbPath))
	{
		ads_printf(_T("\n ��ģ�����ݿ�ʧ�ܣ� %s"), m_szMdbPath);
		CoUninitialize();
		return false;
	}

	//���������
	m_conn.Execute(_T("DELETE * FROM ���ϸ���������"));

	for (size_t t = 0; t < vObjArrFrame.size(); t++)
	{
		AcDbObjectIdArray objArr = vObjArrFrame.at(t);
		for (int i = 0; i < objArr.length(); i++)
		{
			AcGePoint2dArray nodes;
			if (false == GetPlList(aname(objArr.at(i)), nodes)) continue;
			AcDbObjectIdArray objArrTemp;
			ssFromNodes(objArrTemp, nodes, 1);
			objArrTemp.remove(objArr.at(i));

			TCHAR szFrameNum[200] = {0};
			ReadXdata(aname(objArr.at(i)), _T("BH"), STRING_TYPE, szFrameNum);

			for (int j = 0; j < objArrTemp.length(); j++)
			{
				_RecordsetPtr res;
				CString strSQL = _T("INSERT INTO ���ϸ��������� VALUES(");
				ads_name ent;
				if (Acad::eOk != acdbGetAdsName(ent, objArrTemp.at(j))) continue;

				int nSort = 0;
				TCHAR szValue[200] = {0};
				TCHAR szCode[200] = {0};
				TCHAR szTemp[500] = {0};

				GetStdm(ent, szCode);
				if (0 == _tcscmp(szCode, _T(""))) continue;

				if (false == getSortAndClass(szCode, nSort, szValue)) continue;

				_stprintf(szTemp, _T("%d, '%s',"), nSort, szValue);
				strSQL += szTemp;

				if (0 == _tcscmp(szCode, _T("144311")))
				{
					short sType = 0;
					ReadXdataShort(ent, _T("WQCZ"), sType);
					for (vector<FieldDict2>::iterator it = g_lstitems.m_vFieldDicts.begin(); it != g_lstitems.m_vFieldDicts.end(); it++)
					{
						//�ҵ��ˣ���������ѡ��
						if (0 == _tcsicmp(it->szDictName, _T("TBL_WQCZ")))
						{
							for (size_t t = 0; t < it->vRecord.size(); t++)
							{
								int nCode = TrimString2(it->vRecord.at(t));
								if (nCode == sType)
								{
									_tcscpy(szValue, TrimString3(it->vRecord.at(t)));
									break;
								}
							}
							break;

						}
					}
				}
				else
				{
					_tcscpy(szValue, _T(""));
					ReadXdata(ent, _T("NAME"), STRING_TYPE, szValue);
					if (0 == _tcscmp(szValue, _T("")))
						g_index.get_name(szCode, szValue);
				}

				TCHAR szUnit[100] = {0};
				getUnits(szCode, szUnit);

				_stprintf(szTemp, _T("'%s%s',"), szValue, szUnit);
				strSQL += szTemp;

				double dValue = 0.0;
				ReadXdataDouble(ent, _T("GD"), dValue);
				_stprintf(szTemp, _T("%lf,"), dValue);
				strSQL += szTemp;

				dValue = 0.0;
				ReadXdataDouble(ent, _T("DEEP"), dValue);
				_stprintf(szTemp, _T("%lf,"), dValue);
				strSQL += szTemp;

				dValue = 0.0;
				ReadXdataDouble(ent, _T("KJ"), dValue);
				_stprintf(szTemp, _T("%lf,"), dValue);
				strSQL += szTemp;

				dValue = 0.0;
				ReadXdataDouble(ent, _T("GJ"), dValue);
				_stprintf(szTemp, _T("%lf,"), dValue);
				strSQL += szTemp;

				dValue = 0.0;
				ReadXdataDouble(ent, _T("XJ"), dValue);
				_stprintf(szTemp, _T("%lf,"), dValue);
				strSQL += szTemp;

				short sCount = 0;
				ReadXdataShort(ent, _T("SM"), sCount);
				_stprintf(szTemp, _T("%d,"), sCount);
				strSQL += szTemp;

				_tcscpy(szValue, _T(""));
				ReadXdata(ent, _T("ZJCD"), STRING_TYPE, szValue);
				_stprintf(szTemp, _T("%lf,"), _tstof(szValue));
				strSQL += szTemp;

				_tcscpy(szValue, _T(""));
				ReadXdata(ent, _T("ZJMJ"), STRING_TYPE, szValue);
				_stprintf(szTemp, _T("%lf,"), _tstof(szValue));
				strSQL += szTemp;

				_tcscpy(szValue, _T(""));
				ReadXdata(ent, _T("ZJTJ"), STRING_TYPE, szValue);
				_stprintf(szTemp, _T("%lf,"), _tstof(szValue));
				strSQL += szTemp;

				_stprintf(szTemp, _T("'%s')"), szFrameNum);
				strSQL += szTemp;


				res = m_conn.Execute(strSQL);
				if (res == NULL)
				{
					ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);
					continue;
				}
			}

		}
	}

	m_conn.Close();
	return true;
}

bool	CExportMdbInfo::excuteStoreProcess()
{
	TCHAR szFile[1000] = {0};
	ads_findfile(_T("�������ݿ�SQL���.ini"), szFile);
	if (0 == _tcslen(szFile))
	{
		ads_printf(_T("\n �Ҳ����ļ�! �������ݿ�SQL���.ini"));
		return false;
	}
	CStringArray strArr;
	readTxtFile(strArr, szFile);
	if (strArr.GetSize() == 0) return false;

	CString strSQL;
	_RecordsetPtr res;
	for (int i = 0; i < strArr.GetSize(); i++)
	{
		strSQL = strArr.GetAt(i);
		strSQL.Trim();
		if (strSQL == _T("")) continue;

		res = m_conn.Execute(strSQL);
		if (res == NULL)
			ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);

	}

	return true;
}

bool		CExportMdbInfo::checkFilesExits(vector<AcDbObjectIdArray> vObjArrFrame)
{
	vector<CString> vStrFile, vStrFileFind;
	GetPathFileName(m_strSaveFlolder, _T("xlsx"), vStrFile, 1);

	for (size_t t = 0; t < vObjArrFrame.size(); t++)
	{
		AcDbObjectIdArray objArr = vObjArrFrame.at(t);
		for (int i = 0; i < objArr.length(); i++)
		{
			ads_name entFrame;
			if (Acad::eOk != acdbGetAdsName(entFrame, objArr.at(i))) continue;

			TCHAR szFrameNum[200] = {0};
			ReadXdata(entFrame, _T("BH"), STRING_TYPE, szFrameNum);

			CString strFile;
			strFile.Format(_T("����������_%s.xlsx"), szFrameNum);
			bool bFindFile = false;
			for (size_t t = 0; t < vStrFile.size(); t++)
			{
				if (strFile == vStrFile.at(t))
				{
					bFindFile = true;
					break;
				}
			}

			if (bFindFile)
				vStrFileFind.push_back(strFile);

		}
	}

	if (vStrFileFind.size())
	{
		CString str = _T("����Ŀ���ļ����´���ͬ���ļ����Ƿ񸲸ǣ�  \n\n");

		for (size_t t = 0; t < vStrFileFind.size(); t++)
		{
			str += _T("\n ");
			str += vStrFileFind.at(t);
			str += _T(";");
		}

		if (IDYES == AfxMessageBox(str, MB_YESNO))
		{
			for (size_t t = 0; t < vStrFileFind.size(); t++)
			{
				str = m_strSaveFlolder;
				str += vStrFileFind.at(t);

				FileDelete(str);
			}
		}
		else
		{
			str = getPathName_AutoChange();
			if (str == _T("")) return false;

			m_strSavePath.Format(_T("%s���ϸ���������"), str);
			m_strSaveFlolder = str;

		}
	}
	return true;
}


bool	CExportMdbInfo::exportInfoToMdb()
{
	vector<AcDbObjectIdArray> vObjArrFrame;
	CExportSummaryTable exportSummary;
	exportSummary.m_sReportType = m_sReportType;
	if (false == exportSummary.getAndFilterFrame_Indoor(vObjArrFrame)) return false;
	if (m_sReportType ==IndoorReport_GroundAttachmentsTable_standard2)
	{
		m_strSavePath.Format(_T("%s���ϸ���������"), exportSummary.m_strSavePath);
		m_strSaveFlolder = exportSummary.m_strSavePath;
	}
	else
		m_strSavePath = exportSummary.m_strSavePath;

	if (!m_conn.ConnectAccess(m_szMdbPath))
	{
		ads_printf(_T("\n ��ģ�����ݿ�ʧ�ܣ� %s"), m_szMdbPath);
		CoUninitialize();
		return false;
	}

	if (m_sReportType ==IndoorReport_GroundAttachmentsTable_standard2)
	{
		if (false == checkFilesExits(vObjArrFrame)) return false;
	}

	readSortAndClassInfo();

	if (m_sReportType == IndoorReport_GroundAttachmentsTable_standard2)
	{
		readUnitsFromTable();
	}

	CString strSQL;
	_RecordsetPtr res;
	//���������
	m_conn.Execute(_T("DELETE * FROM ���ϸ���������"));

	m_vRecords.clear();

	for (size_t t = 0; t < vObjArrFrame.size(); t++)
	{
		AcDbObjectIdArray objArr = vObjArrFrame.at(t);
		for (int i = 0; i < objArr.length(); i++)
		{
			AcGePoint2dArray nodes;
			AcDbObjectId objFrame = objArr.at(i);
			ads_name entFrame;
			if (Acad::eOk != acdbGetAdsName(entFrame, objFrame)) continue;

			if (false == GetPlList(entFrame, nodes)) continue;
			AcDbObjectIdArray objArrTemp;
			ssFromNodes(objArrTemp, nodes, 1);
			objArrTemp.remove(objFrame);

			TCHAR szFrameNum[200] = {0};
			ReadXdata(entFrame, _T("BH"), STRING_TYPE, szFrameNum);

			TCHAR szOwnerName[200] = {0};
			ReadXdata(entFrame, _T("HZMC"), STRING_TYPE, szOwnerName);

			TCHAR szID[200] = {0};
			ReadXdata(entFrame, _T("SFZH"), STRING_TYPE, szID);

			TCHAR szIDIndex[200] = {0};
			_stprintf(szIDIndex, _T("%d"), t + 1);

			for (int j = 0; j < objArrTemp.length(); j++)
			{

				ads_name ent;
				if (Acad::eOk != acdbGetAdsName(ent, objArrTemp.at(j))) continue;

				int nSort = 0;
				TCHAR szValue[200] = {0};
				TCHAR szCode[200] = {0};
				TCHAR szTemp[500] = {0};

				GetStdm(ent, szCode);
				if (0 == _tcscmp(szCode, _T(""))) continue;

				SStaticData sData;
				_tcscpy(sData.szFrameNum, szFrameNum);
				_tcscpy(sData.szID, szID);
				_tcscpy(sData.szIDIndex, szIDIndex);
				_tcscpy(sData.szOwnerName, szOwnerName);

				if (false == getSortAndClass(szCode, sData.sNum, sData.szClass1)) continue;

				if (0 == _tcscmp(szCode, _T("144311")))
				{
					TCHAR szType[20] = {0};
					ReadXdata(ent, _T("WQCZ"), STRING_TYPE, szType);
					for (vector<FieldDict2>::iterator it = g_lstitems.m_vFieldDicts.begin(); it != g_lstitems.m_vFieldDicts.end(); it++)
					{
						//�ҵ��ˣ���������ѡ��
						if (0 == _tcsicmp(it->szDictName, _T("TBL_WQCZ")))
						{
							for (size_t t = 0; t < it->vRecord.size(); t++)
							{
								int nCode = TrimString2(it->vRecord.at(t));
								if (nCode == _ttol(szType))
								{
									_tcscpy(szValue, TrimString3(it->vRecord.at(t)));
									break;
								}
							}
							break;

						}
					}
				}
				else
				{
					_tcscpy(szValue, _T(""));
					ReadXdata(ent, _T("NAME"), STRING_TYPE, szValue);
					if (0 == _tcscmp(szValue, _T("")))
						g_index.get_name(szCode, szValue);
				}

				TCHAR szUnit[100] = {0};
				getUnits(szCode, szUnit);

				_stprintf(sData.szClass2, _T("%s%s"), szValue, szUnit);

				ReadXdataDouble(ent, _T("GD"), sData.dHeight);

				ReadXdataDouble(ent, _T("DEEP"), sData.dDeep);

				ReadXdataDouble(ent, _T("KJ"), sData.dSpan);

				ReadXdataDouble(ent, _T("GJ"), sData.dWellDiameter);

				ReadXdataDouble(ent, _T("XJ"), sData.dTreediameter);

				short sCount = 0;
				ReadXdataShort(ent, _T("SM"), sCount);
				sData.nCount = sCount;

				//��Ϊǰ�涼���Ը�ʽ��2λ����ַ�д����չ���Եģ������������ַ�����ȡ
				if (0 == _tcscmp(szCode, _T("144311")) || 0 == _tcscmp(szCode, _T("144410")))
				{
					_tcscpy(szValue, _T(""));
					ReadXdata(ent, _T("ZJCD"), STRING_TYPE, szValue);
					sData.dLength = _tstof(szValue);
				}
				else
				{
					ReadXdataDouble(ent, _T("ZJCD"), sData.dLength);
				}

				ReadXdata(ent, _T("ZJMJ"), STRING_TYPE, szValue);
				sData.dArea = _tstof(szValue);

				ReadXdata(ent, _T("ZJTJ"), STRING_TYPE, szValue);
				sData.dVolume = _tstof(szValue);

				if (m_sReportType == IndoorReport_GroundAttachmentsTable_standard2)
				{
					addToCurrentData(sData);
				}
				else
					m_vRecords.push_back(sData);

			}

		}
	}

	for (vector<SStaticData>::iterator it = m_vRecords.begin(); it != m_vRecords.end(); it++)
	{
		strSQL = _T("INSERT INTO ���ϸ��������� VALUES(");

		TCHAR szTemp[200] = {0};
		_stprintf(szTemp, _T("%d, '%s',"), it->sNum, it->szClass1);
		strSQL += szTemp;

		_stprintf(szTemp, _T("'%s',"), it->szClass2);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dHeight);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dDeep);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dSpan);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dWellDiameter);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dTreediameter);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%d,"), it->nCount);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dLength);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dArea);
		strSQL += szTemp;

		_stprintf(szTemp, _T("%lf,"), it->dVolume);
		strSQL += szTemp;

		_stprintf(szTemp, _T("'%s',"), it->szFrameNum);
		strSQL += szTemp;

		_stprintf(szTemp, _T("'%s',"), it->szOwnerName);
		strSQL += szTemp;

		_stprintf(szTemp, _T("'%s',"), it->szID);
		strSQL += szTemp;

		_stprintf(szTemp, _T("0.0,"));
		strSQL += szTemp;

		_stprintf(szTemp, _T("'%s')"), it->szIDIndex);
		strSQL += szTemp;

		res = m_conn.Execute(strSQL);
		if (res == NULL)
		{
			ads_printf(_T("\n ִ��SQL���ʧ�ܣ� %s"), strSQL);
			continue;
		}
	}

	if (m_sReportType == IndoorReport_OwnerInfoSummaryTable_standard2)
		excuteStoreProcess();

	m_conn.Close();
	return true;
}

bool		CExportMdbInfo::addToCurrentData(SStaticData sData)
{
	bool bFind = false;
	for (vector<SStaticData>::iterator it = m_vRecords.begin(); it != m_vRecords.end(); it++)
	{
		//ֻ׷��ͬһͼ���ϵĵ���
		if (0 != _tcscmp(sData.szFrameNum, it->szFrameNum)) continue;

		if (0 == _tcscmp(sData.szClass1, _T("Ů��ǽ"))
			|| 0 == _tcscmp(sData.szClass1, _T("Χǽ")))
		{
			if (0 == _tcscmp(it->szClass1, sData.szClass1) && 0 == _tcscmp(sData.szClass2, it->szClass2))
			{
				if (fabs(sData.dHeight - it->dHeight) <= EPS)
				{
					it->dLength += sData.dLength;
					bFind = true;
					break;
				}
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("դ��ǽ")))
		{
			if (0 != _tcscmp(it->szClass1, _T("դ��ǽ"))) continue;
			if (fabs(sData.dHeight - it->dHeight) <= EPS)
			{
				it->dLength += sData.dLength;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("������")))
		{
			if (0 != _tcscmp(it->szClass1, _T("������"))) continue;
			if (0 == _tcscmp(sData.szClass2, it->szClass2))
			{
				if (0 == _tcscmp(sData.szClass2, _T("���ӣ������ף�"))
					|| 0 == _tcscmp(sData.szClass2, _T("����أ������ף�"))
					|| 0 == _tcscmp(sData.szClass2, _T("�ؽѣ������ף�")))
					it->dVolume += sData.dVolume;
				else
					it->dArea += sData.dArea;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("����")))
		{
			if (0 != _tcscmp(it->szClass1, _T("����"))) continue;
			if (0 != _tcscmp(it->szClass2, sData.szClass2)) continue;
			if (fabs(it->dSpan - sData.dSpan) <= EPS)
			{
				it->dLength += sData.dLength;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("̨��")))
		{
			if (0 != _tcscmp(it->szClass1, _T("̨��"))) continue;
			if (fabs(sData.dHeight - it->dHeight) <= EPS)
			{
				it->dLength += sData.dLength;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("ʯ�ߣ��ӹ̶�����")))
		{
			if (0 != _tcscmp(it->szClass1, _T("ʯ�ߣ��ӹ̶�����"))) continue;
			if (fabs(sData.dHeight - it->dHeight) <= EPS)
			{
				it->dLength += sData.dLength;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("ˮ��"))
			|| 0 == _tcscmp(sData.szClass1, _T("ˮ��")))
		{
			if (0 == _tcscmp(it->szClass2, sData.szClass2))
			{
				it->dVolume += sData.dVolume;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("ˮ��")))
		{
			if (0 != _tcscmp(it->szClass1, _T("ˮ��"))) continue;
			if (0 != _tcscmp(sData.szClass2, it->szClass2)) continue;
			if (fabs(it->dDeep - sData.dDeep) <= EPS && fabs(it->dWellDiameter - sData.dWellDiameter) <= EPS)
			{
				it->nCount += sData.nCount;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("����"))
			|| 0 == _tcscmp(sData.szClass1, _T("������"))
			|| 0 == _tcscmp(sData.szClass1, _T("С��"))
			|| 0 == _tcscmp(sData.szClass1, _T("����"))
			|| 0 == _tcscmp(sData.szClass1, _T("����"))
			|| 0 == _tcscmp(sData.szClass1, _T("��ˮ��ֳ"))
			|| 0 == _tcscmp(sData.szClass1, _T("����"))
			|| 0 == _tcscmp(sData.szClass1, _T("����")))
		{
			if (0 == _tcscmp(it->szClass1, sData.szClass1) && 0 == _tcscmp(sData.szClass2, it->szClass2))
			{
				it->dArea += sData.dArea;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("שҤ"))
			|| 0 == _tcscmp(sData.szClass1, _T("����"))
			|| 0 == _tcscmp(sData.szClass1, _T("��ľ"))
			|| 0 == _tcscmp(sData.szClass1, _T("������ͨѶ��·"))
			|| 0 == _tcscmp(sData.szClass1, _T("Ǩ����ʩ")))
		{
			if (0 == _tcscmp(it->szClass1, sData.szClass1) && 0 == _tcscmp(sData.szClass2, it->szClass2))
			{
				it->nCount += sData.nCount;
				bFind = true;
				break;
			}
		}
		else if (0 == _tcscmp(sData.szClass1, _T("��ľ"))
			||  0 == _tcscmp(sData.szClass1, _T("����")))
		{
			if (abs(it->dTreediameter - sData.dTreediameter) < EPS && 0 == _tcscmp(it->szClass1, sData.szClass1) && 0 == _tcscmp(sData.szClass2, it->szClass2))
			{
				it->nCount += sData.nCount;
				bFind = true;
				break;
			}
		}
	}

	if (bFind == false)
		m_vRecords.push_back(sData);

	return true;
}



CExportMdbInfo::CExportMdbInfo(void)
{
	m_bReadAccdbTemp = false;
}


CExportMdbInfo::~CExportMdbInfo(void)
{
}
