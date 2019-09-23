#include "StdAfx.h"
#include "wordRpt.h"


bool CExportSurveyTable::readSourceExcel()
{


	CTinyExcel excel;
	excel.SetTemplete(m_strPathExcel);
	if(excel.StartExcel() == false) return false;

#ifdef _DEBUG
	excel.Show();
#endif

	int nRow = 3;
	int nCol1 = 1, nCol1_1 = 2, nCol2 = 3, nColHeightAverage = 5, nCol3 = 7, nColNoteName = 8;
	long nRows = 0, nCols = 0;
	CString strText;
	excel.GetRowColCount(nRows, nCols);

	int nCount = 1;

	while(nRow <= nRows)
	{
		CString strTextFloor = excel.GetRangeContent(nRow, nCol1, excel.m_range);
		if(strTextFloor == _T("采用基准")) break;

		int nRowM = 0, nColM = 0;
		excel.GetMergedRowsAndCols(nRow, nCol1, nRowM, nColM);

		if(nColM == 2)		//正常提取
		{
			for(int i = nRow; i < nRowM + nRow;)
			{
				int nRowM2 = 0, nColM2 = 0;
				excel.GetMergedRowsAndCols(i, nColNoteName, nRowM2, nColM2);

				for(int j = i; j < i + nRowM2; j ++)
				{
					ElevationInfo elev;
					elev.strFloorName = strTextFloor;

					strText = excel.GetRangeContent(j, nCol2, excel.m_range);
					elev.strPointName = strText;

					strText = excel.GetRangeContent(i, nColNoteName, excel.m_range);
					elev.strName = strText;

					m_vElevaInfo.push_back(elev);

				}
				i += nRowM2;
			}
		}
		else if(nColM == 1)		//特殊情况
		{
			for(int i = nRow; i < nRowM + nRow;)
			{
				int nRowM2 = 0, nColM2 = 0;
				excel.GetMergedRowsAndCols(i, nCol1_1, nRowM2, nColM2);

				for(int j = i; j < i + nRowM2;)
				{
					int nRowM3 = 0, nColM3 = 0;
					excel.GetMergedRowsAndCols(j, nColNoteName, nRowM3, nColM3);

					for(int k = j; k < j + nRowM3; k ++)
					{
						strText = excel.GetRangeContent(i, nCol1_1, excel.m_range);
						ElevationInfo elev;
						elev.strFloorName = strTextFloor;
						elev.strFloorName += strText;

						strText = excel.GetRangeContent(k, nCol2, excel.m_range);
						elev.strPointName = strText;

						strText = excel.GetRangeContent(j, nColNoteName, excel.m_range);
						elev.strName = strText;

						m_vElevaInfo.push_back(elev);
					}

					j += nRowM3;
				}
				i += nRowM2;
			}
		}


		nRow += nRowM;
	}

	excel.CloseWorkBook(excel.m_workbook);
	excel.Quit();

	return true;
}

bool	CExportSurveyTable::extractOriginWord()
{
	CStringArray strArray;
	TCHAR szPath[500] = {0};
	_tcscpy(szPath, m_strPathWord);
	readTxtFile(strArray, szPath);

	CSmartTable word;
	for(int i = 0; i < strArray.GetSize() - 1; i ++)
	{
		CString str = strArray.GetAt(i);
		int nIndex = findRecord(str);
		if(nIndex >= 0) 
		{
			str.Format(_T("#标红#%s%s"), m_vElevaInfo.at(nIndex).strFloorName, m_vElevaInfo.at(nIndex).strName);
			strArray.InsertAt(i + 1, str);
			i ++;
		}
	}

	CString strText;
	for(int i = 0; i < strArray.GetSize(); i ++)
	{
		strText = strArray.GetAt(i);
		CSParaXml paraLeft(_T(""), ST_LEFT);
		if(strText.Find(_T("#标红#")) >= 0)
		{
			strText.Replace(_T("#标红#"), _T(""));
			paraLeft.AddText(strText, 12, "", false, false, false, _T("FF0000"));
		}
		else
		{
			paraLeft.AddText(strText, (int)10.5);
		}

		word.AddSObject( paraLeft );
		//acutPrintf("\n %s", strArray.GetAt(i));
	}

	ads_findfile(_T("原始数据.xml"), szPath);
	word.SaveToWord(m_strSavePath, szPath, _T("wordhere") );
	
	return false;

	CTinyWord tinyword;
	if (!tinyword.Open(m_strPathWord))
	{
		tinyword.QuitApp(FALSE);
		return false;
	}

	tinyword.ShowApp();

	msword::Selection selectT;
	msword::Range rangeF;
	tinyword.FindWord(_T("#KEYWORD#"), rangeF);
	tinyword.m_wdSel.AttachDispatch(rangeF.m_lpDispatch);
	tinyword.m_wdSel.Select();
	tinyword.m_wdSel.Copy();
	tinyword.FindWord(_T("#KEYWORD#"), _T(" "));

	msword::Paragraphs ps = tinyword.m_wdDoc.GetParagraphs();
	int nCount = ps.GetCount();
	acedSetStatusBarProgressMeter(_T("正在输出..."), 0, nCount);
	for(int i = 0; i < nCount; i ++)
	{
		acedSetStatusBarProgressMeterPos(i);
		msword::Paragraph ps1 = ps.Item(i + 1);

		msword::Range range;
		range.AttachDispatch(ps1.GetRange());
		CString str = range.GetText();
		//acutPrintf(_T("\n %s"), str);

		int nIndex = findRecord(str);
		if(nIndex == -1) continue;

		str.Format(_T("%s%s"), m_vElevaInfo.at(nIndex).strFloorName, m_vElevaInfo.at(nIndex).strName);

 		long lS = 0, lE = 0;
 		lS = range.GetStart();
 		lE = range.GetEnd();
 		range.SetRange(lE, lE);
 		range.InsertParagraph();
		//range.InsertParagraph();
		tinyword.m_wdSel.AttachDispatch(range.m_lpDispatch);
		tinyword.m_wdSel.Paste();
		lE = tinyword.m_wdSel.GetEnd();
		range.SetRange(lE, lE);
		range.InsertParagraph();
		tinyword.FindWord(_T("#KEYWORD#"), str);

		//
		nCount = ps.GetCount();
	
	}
	acedRestoreStatusBar();


	return true;
}

int CExportSurveyTable::findRecord(CString strName)
{
	CStringArray strArr;
	AnalyzeData(strArr, strName, _T(","));
	if(strArr.GetSize() < 4 ) return -1;

	bool bFind = false;
	for(int t = 0; t < (int)m_vElevaInfo.size(); t ++)
	{
		if(strArr.GetAt(0) == m_vElevaInfo.at(t).strPointName) 
		{
			return t;
		}
	}

	return -1;
}


bool CExportSurveyTable::extractToOrignDoc()
{
	CFileDialog openFD(TRUE,_T("*.xlsx"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("要素表(*.xlsx)|*.xlsx|excel文件(*.xlsx)|*.xlsx||"));
	if (openFD.DoModal() != IDOK) return false;
	m_strPathExcel = openFD.GetPathName();

// 	CFileDialog openFD2(TRUE,_T("*.doc"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("原始数据(*.doc)|*.doc|word文件(*.doc)|*.doc||"));
// 	if (openFD2.DoModal() != IDOK) return false;
// 	m_strPathWord = openFD2.GetPathName();

	CFileDialog openFD2(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("原始数据 (*.dat;*.txt;)|*.dat;*.txt;||"));
	if (openFD2.DoModal() != IDOK) return false;
	m_strPathWord = openFD2.GetPathName();

	TCHAR szFilters[]=
		_T("doc (*.doc)|*.doc|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE,_T("doc"),_T("*.doc"),OFN_FILEMUSTEXIST
		|OFN_HIDEREADONLY,szFilters,NULL);
	if(fileDlg.DoModal() != IDOK) return false;
	m_strSavePath = fileDlg.GetPathName();

	if(false == readSourceExcel()) return false;

	if(false == extractOriginWord()) return false;


	return true;
}


bool CExportSurveyTable::exportSurveyResultReport()
{
	checkProjInfo();

	CMatchFialogsEx matchDialog;
	_tcscpy(matchDialog.m_szDivWord, _T("|"));	//报告书字段匹配文件比较特殊，是用“|”字符来分割的
	matchDialog.SetConfigFile(_T("字段匹配表_测量成果报告书.ini"));
	if(matchDialog.DoModal() != IDOK) return false;

		TCHAR szFilters[]=
		_T("docx (*.docx)|*.docx|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE,_T("docx"),_T("*.docx"),OFN_FILEMUSTEXIST
		|OFN_HIDEREADONLY,szFilters,NULL);
	if(fileDlg.DoModal() != IDOK) return false;

	CString strPath = fileDlg.GetPathName();

	TCHAR szTempPath[MAX_PATH];
	//TCHAR szTempName[255]=_T("建设工程规划竣工测量成果_模板.docx");
	TCHAR szTempName[255]=_T("constructs_survey_result_template.docx");
	if (ads_findfile(szTempName,szTempPath) != RTNORM)
	{
		TCHAR szTemp[255]={0};
		_stprintf(szTemp,_T("\n 没有找到模板:%s"),szTempName);
		acutPrintf(szTemp);
		return false;
	}

	CopyFile(szTempPath,strPath,false);

	CTinyWord tinyword;
	if (!tinyword.Open(strPath))
	{
		tinyword.QuitApp(FALSE);
		return false;
	}

	CString strText1, strText2;
	for(int i = 0; i < matchDialog.m_strArrKey.GetSize(); i ++)
	{
		strText1 = matchDialog.m_strArrKey.GetAt(i);
		strText2 = matchDialog.m_strArrValue.GetAt(i);
		tinyword.FindWord(strText1, strText2);
	}

	CXRecordOperate xrecordO;
	for(int i = 0; i < g_nMaxInfo; i ++)
	{
		if(0 == _tcscmp(g_AttrInfo[i].table, _T("XMXX")))
		{
			for(int j = 0; j < g_AttrInfo[i].AttrArr.GetSize(); j ++)
			{

				xrecordO.ReadStringRecord(DICT_PROJECTINFO, g_AttrInfo[i].AttrArr.GetAt(j).name, strText2);
				strText1.Format(_T("#%s#"), g_AttrInfo[i].AttrArr.GetAt(j).name);
				tinyword.FindWord(strText1, strText2);

			}
			break;
		}
	}

	tinyword.Save();
	tinyword.QuitApp(FALSE);

	if( IDYES == AfxMessageBox(_T("生成完毕！是否打开所在目录？") , MB_YESNO) )
	{
		ShellExecute(adsw_hwndAcad,_T("open"),strPath , _T(""),NULL,SW_SHOWNORMAL);
	}

	return true;
}
