#include "stdafx.h"
#include "excelRpt.h"

CCheckResult::CCheckResult()
{
	m_checkPrec = 2;
}

bool		CCheckResult::checkBuildingName()
{
	CExportSummaryTable exportExcel;
	exportExcel.m_strSourceExcel = m_strPathFeatureTable;
	exportExcel.readSourceExcel();

	readSummaryTableInfo();

	readBuildingAreaInfo();

	//要素表-汇总表
	for(vector<CSVInfo>::iterator it = exportExcel.m_vCSVInfo.begin(); it != exportExcel.m_vCSVInfo.end(); it ++)
	{
		bool bFind = false;
		for(vector<CSVInfo>::iterator itt = m_vCSVInfo_SummaryTable.begin(); itt != m_vCSVInfo_SummaryTable.end(); itt ++)
		{
			if(it->strFloorName == itt->strFloorName)
			{
				bFind = true;
				break;
			}
		}
		if(bFind == false)
			acutPrintf(_T("\n 要素表楼名与汇总表楼名不匹配！ %s"), it->strFloorName);
	}

	//汇总表-面积图
	for(vector<CSVInfo>::iterator it = m_vCSVInfo_SummaryTable.begin(); it != m_vCSVInfo_SummaryTable.end(); it ++)
	{
		bool bFind = false;
		for(vector<CSVInfo>::iterator itt = m_vCSVInfo_SummartBuildFrame.begin(); itt != m_vCSVInfo_SummartBuildFrame.end(); itt ++)
		{
			if(it->strFloorName == itt->strFloorName)
			{
				bFind = true;
				break;
			}
		}
		if(bFind == false)
			acutPrintf(_T("\n 汇总表楼名与面积图楼名不匹配！ %s"), it->strFloorName);
	}

	//要素表-面积图
	for(vector<CSVInfo>::iterator it = exportExcel.m_vCSVInfo.begin(); it != exportExcel.m_vCSVInfo.end(); it ++)
	{
		bool bFind = false;
		for(vector<CSVInfo>::iterator itt = m_vCSVInfo_SummartBuildFrame.begin(); itt != m_vCSVInfo_SummartBuildFrame.end(); itt ++)
		{
			if(it->strFloorName == itt->strFloorName)
			{
				bFind = true;
				break;
			}
		}
		if(bFind == false)
			acutPrintf(_T("\n 要素表楼名与面积图楼名不匹配！ %s"), it->strFloorName);
	}


	return true;
}

bool		CCheckResult::readSummaryTableInfo()
{
	CTinyExcel excel;
	excel.SetTemplete(m_strPathSummartTable);
	if(excel.StartExcel() == false) return false;

	int nRow = 5, nCol1 = 1, nCol2 = 2, nColHeightAverage = 5, nCol3 = 3, nColNoteName = 8;
	long nRows = 0, nCols = 0;
	CString strText;
	excel.GetRowColCount(nRows, nCols);

	m_vCSVInfo_SummaryTable.clear();

	while(nRow <= nRows)
	{
		strText = excel.GetRangeContent(nRow, nCol1, excel.m_range);
		if(strText == _T("合计")) break;

		strText = excel.GetRangeContent(nRow, nCol2, excel.m_range);

		CSVInfo csv;
		csv.strFloorName = strText;

		int nRowM = 0, nColM = 0;
		excel.GetMergedRowsAndCols(nRow, nCol2, nRowM, nColM);

		for(int j = nRow; j < nRow + nRowM; j ++)
		{
			strText = excel.GetRangeContent(j, nCol3, excel.m_range);
			ElevationInfo elev;
			elev.dHeight = _tstof(strText);
			csv.vIndoorHeight.push_back(elev);
		}

		if(csv.strFloorName != _T(""))
			m_vCSVInfo_SummaryTable.push_back(csv);


		nRow += nRowM;
	}

	excel.CloseWorkBook(excel.m_workbook);
	excel.Quit();

	return true;
}

bool		CCheckResult::readBuildingAreaInfo()
{

	// 源图形数据库  
	AcDbDatabase* pExternalDb = new AcDbDatabase(false);
	if(Acad::eOk != pExternalDb->readDwgFile(m_strPathBuildingAreaFrame)) return false;

	AcDbBlockTable* pBlockTable;
	pExternalDb->getBlockTable(pBlockTable,AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForRead);
	AcDbBlockTableRecordIterator* pItr;
	pBlockTableRecord->newIterator(pItr);

	m_vCSVInfo_SummartBuildFrame.clear();
	AcDbObjectIdArray list;  // 导出到临时图形数据库的实体数组 
	for (pItr->start(); !pItr->done(); pItr->step()) 
	{ 
		
		AcDbEntity* pEnt;
		pItr->getEntity(pEnt, AcDb::kForRead);

		TCHAR szType[500] = {0};
		ReadEntityXdata((AcDbObject*)pEnt, XDATA_ENTITY_TYPE, INT_TYPE, szType);

		if(_ttoi(szType) == 3)
		{
			AcDbTable *pTable = AcDbTable::cast(pEnt);
			if(pTable == NULL) continue;

			if(pTable->numRows() > 0 && pTable->numColumns() > 0)
			{
				CSVInfo csv;
				//因为表格是向上的，所以行索引从下面开始
				csv.strFloorName = pTable->textString(1, 0);
				csv.strFloorName.Replace(_T("建筑尺寸、面积图"), _T(""));
				m_vCSVInfo_SummartBuildFrame.push_back(csv);
			}
			
		}

	}
	delete pItr; 
	pBlockTableRecord->close();     
	pBlockTable->close();

	delete pExternalDb;


	return true;
}

bool		CCheckResult::readTotalFrameInfo()
{

	// 源图形数据库  
	AcDbDatabase* pExternalDb = new AcDbDatabase(false);
	if(Acad::eOk != pExternalDb->readDwgFile(m_strPathTotalFrame)) return false;

	AcDbBlockTable* pBlockTable;
	pExternalDb->getBlockTable(pBlockTable,AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForRead);
	AcDbBlockTableRecordIterator* pItr;
	pBlockTableRecord->newIterator(pItr);

	m_vCSVInfo_TolalFrame.clear();
	AcDbObjectIdArray list;  // 导出到临时图形数据库的实体数组 
	for (pItr->start(); !pItr->done(); pItr->step()) 
	{ 

		AcDbEntity* pEnt;
		pItr->getEntity(pEnt, AcDb::kForRead);

		TCHAR szType[500] = {0};
		ReadEntityXdata((AcDbObject*)pEnt, XDATA_ENTITY_TYPE, INT_TYPE, szType);

		if(_ttoi(szType) == 4)
		{
			szType[0] = _T('\0');
			ReadEntityXdata((AcDbObject*)pEnt, _T("JZMC"), STRING_TYPE, szType);

			CSVInfo csv;
			csv.strFloorName = szType;

			AcDbObjectId objId = pEnt->objectId();
			pEnt->close();

			vector<AcDbObjectId> vObj;
			//GetGroupEnts(objId, vObj);
			AcDbObjectId groupId;
			if ( true == GetEntGroupEx(objId,groupId) )
			{
				AcDbGroup *pGroup;
				if ( Acad::eOk == acdbOpenObject(pGroup, groupId, AcDb::kForRead) )
				{
					AcDbGroupIterator *pIter = pGroup->newIterator();
					for (; !pIter->done(); pIter->next()) 
					{
						AcDbObjectId objT = pIter->objectId();
						if ( objId != objT )
						{
							vObj.push_back( objT );
						}
					}
					delete pIter; 	
					pGroup->close(); 
				}

			}


			for(size_t t = 0; t < vObj.size(); t ++)
			{
				TCHAR szStdm[500] = {0};
				GetStdm(aname(vObj.at(t)), szStdm);

				//可能有多个室内坪
				if(0 == _tcscmp(szStdm, _T("450102")))
				{
					CString strText;
					szStdm[0] = _T('\0');
					GetEntText(aname(vObj.at(t)), szStdm);
					strText = szStdm;
					strText.Replace(_T("一层室内坪高程："), _T(""));
					strText.Replace(_T("米"), _T(""));

					ElevationInfo elev;
					elev.dHeight = _tstof(strText);

					csv.vIndoorHeight.push_back(elev);

				}
			}

			m_vCSVInfo_TolalFrame.push_back(csv);

		}

	}
	delete pItr; 
	pBlockTableRecord->close();     
	pBlockTable->close();

	delete pExternalDb;

	return true;
}

bool		CCheckResult::checkIndoorHeight()
{
	CExportSummaryTable exportExcel;
	exportExcel.m_strSourceExcel = m_strPathFeatureTable;
	exportExcel.readSourceExcel();

	readSummaryTableInfo();

	readTotalFrameInfo();


	//要素表-汇总表
	for(vector<CSVInfo>::iterator it = exportExcel.m_vCSVInfo.begin(); it != exportExcel.m_vCSVInfo.end(); it ++)
	{
		bool bFind = false;
		for(vector<CSVInfo>::iterator itt = m_vCSVInfo_SummaryTable.begin(); itt != m_vCSVInfo_SummaryTable.end(); itt ++)
		{
			if(it->strFloorName == itt->strFloorName)
			{
				for(vector<ElevationInfo>::iterator itE1 = it->vIndoorHeight.begin(); itE1 != it->vIndoorHeight.end(); itE1 ++)
				{
					bool bFindE = false;
					for(vector<ElevationInfo>::iterator itE2 = itt->vIndoorHeight.begin(); itE2 != itt->vIndoorHeight.end(); itE2 ++)
					{
						CString	str1 = format_double(itE1->dHeight, m_checkPrec);
						CString	str2 = format_double(itE2->dHeight, m_checkPrec);
						if(str1 == str2)
						{
							bFindE = true;
							break;
						}
					}

					if(bFindE == false)
						acutPrintf(_T("\n 要素表楼 %s 与汇总表楼 %s 的室内坪不匹配！"), it->strFloorName, itt->strFloorName);
				}
				
				bFind = true;
				break;
			}
		}
 		if(bFind == false)
 			acutPrintf(_T("\n 要素表楼 %s 在汇总表中找不到匹配楼(找不到室内坪注记)！"), it->strFloorName);
	}

	//汇总表-总平图
	for(vector<CSVInfo>::iterator it = m_vCSVInfo_SummaryTable.begin(); it != m_vCSVInfo_SummaryTable.end(); it ++)
	{
		bool bFind = false;
		for(vector<CSVInfo>::iterator itt = m_vCSVInfo_TolalFrame.begin(); itt != m_vCSVInfo_TolalFrame.end(); itt ++)
		{
			if(it->strFloorName == itt->strFloorName)
			{
				for(vector<ElevationInfo>::iterator itE1 = it->vIndoorHeight.begin(); itE1 != it->vIndoorHeight.end(); itE1 ++)
				{
					bool bFindE = false;
					for(vector<ElevationInfo>::iterator itE2 = itt->vIndoorHeight.begin(); itE2 != itt->vIndoorHeight.end(); itE2 ++)
					{
						CString	str1 = format_double(itE1->dHeight, m_checkPrec);
						CString	str2 = format_double(itE2->dHeight, m_checkPrec);
						if(str1 == str2)
						{
							bFindE = true;
							break;
						}
					}

					if(bFindE == false)
						acutPrintf(_T("\n 汇总表楼 %s 与总平图楼 %s 的室内坪不匹配！"), it->strFloorName, itt->strFloorName);
				}

				bFind = true;
				break;
			}
		}
 		if(bFind == false)
			acutPrintf(_T("\n 汇总表楼 %s 在总平图中找不到匹配楼(找不到室内坪注记)！"), it->strFloorName);

	}

	//要素表-总平图
	for(vector<CSVInfo>::iterator it = exportExcel.m_vCSVInfo.begin(); it != exportExcel.m_vCSVInfo.end(); it ++)
	{
		bool bFind = false;
		for(vector<CSVInfo>::iterator itt = m_vCSVInfo_TolalFrame.begin(); itt != m_vCSVInfo_TolalFrame.end(); itt ++)
		{
			if(it->strFloorName == itt->strFloorName)
			{
				for(vector<ElevationInfo>::iterator itE1 = it->vIndoorHeight.begin(); itE1 != it->vIndoorHeight.end(); itE1 ++)
				{
					bool bFindE = false;
					for(vector<ElevationInfo>::iterator itE2 = itt->vIndoorHeight.begin(); itE2 != itt->vIndoorHeight.end(); itE2 ++)
					{
						CString	str1 = format_double(itE1->dHeight, m_checkPrec);
						CString	str2 = format_double(itE2->dHeight, m_checkPrec);
						if(str1 == str2)
						{
							bFindE = true;
							break;
						}
					}

					if(bFindE == false)
						acutPrintf(_T("\n 要素表楼 %s 与总平图楼 %s 的室内坪不匹配！"), it->strFloorName, itt->strFloorName);
				}

				bFind = true;
				break;
			}
		}
 		if(bFind == false)
			acutPrintf(_T("\n 要素表楼 %s 在总平图中找不到匹配楼(找不到室内坪注记)！"), it->strFloorName);

	}

	return true;
}

bool	CExportSummaryTable::checkArea()
{

	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 2, _T("*LINE*"), _T("*"), _T(""));
	if(objArr.length() == 0) return false;

	TCHAR szPath[1000] = {0};
	TCHAR szName[] = _T("check_area_template.xlsx");
	ads_findfile(szName, szPath);
	if(0 == _tcscmp(szPath, _T(""))) 
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szName);
		return false;
	}

	TCHAR szFilters[]=
		_T("xlsx (*.xlsx)|*.xlsx|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE,_T("xlsx"),_T("*.xlsx"),OFN_FILEMUSTEXIST
		|OFN_HIDEREADONLY,szFilters,NULL);
	if(fileDlg.DoModal() != IDOK) return false;
	m_strSavePath = fileDlg.GetPathName();

	CopyFile(szPath, m_strSavePath, false);
	
	vector< vector<CString> > vvTable;
	CString strText;
	for(int i = 0; i < objArr.length(); i ++)
	{
		ads_name ent;
		if(Acad::eOk != acdbGetAdsName(ent, objArr.at(i))) continue;

		AcGePoint2dArray nodes;
		GetPlList(ent, nodes);
		if(nodes.length() <= 2) continue;

		TCHAR szStdm[100] = {0};
		GetStdm(ent, szStdm);
		if(0 == _tcsicmp(szStdm, _T("TK"))) continue;

		CContructsOperate constructsOpe;
		constructsOpe.getFitPoint(objArr.at(i));
		strText.Format(_T("%d"), i + 1);
		AcDbObjectId objText = DrawText(asDblArray(constructsOpe.m_ptNoteInsertPoint), strText, _T("M"), _T(""), 1.0);
		setlayer(aname(objText), TEMP_LAYER);
		
		vector<CString> vRecord;
		vRecord.push_back(strText);
		
		TCHAR szText[500] = {0};
		ReadXdata(ent, _T("LH"), INT_TYPE, szText);
		vRecord.push_back(szText);

		_tcscpy(szText, _T(""));
		//ReadXdata(ent, _T("JZMC"), STRING_TYPE, szText);
		g_index.get_name(szStdm, szText);
		vRecord.push_back(szText);

		_tcscpy(szText, _T(""));
		ReadXdata(ent, _T("CBJ"), INT_TYPE, szText);
		vRecord.push_back(szText);

		_tcscpy(szText, _T(""));;
		ReadXdata(ent, _T("QSLC"), INT_TYPE, szText);
		vRecord.push_back(szText);

		_tcscpy(szText, _T(""));
		ReadXdata(ent, _T("ZZLC"), INT_TYPE, szText);
		vRecord.push_back(szText);

		_tcscpy(szText, _T(""));
		ReadXdata(ent, _T("SFKC"), INT_TYPE, szText);
		vRecord.push_back(szText);

		double dArea = 0.0;
		GetEntArea(ent, dArea);
		strText.Format(_T("%.5lf"), dArea);
		vRecord.push_back(strText);

		_tcscpy(szText, _T(""));
		ReadXdata(ent, _T("MJXS"), DOUBLE_TYPE, szText);
		vRecord.push_back(szText);

		vvTable.push_back(vRecord);

	}

	if(vvTable.size() == 0) return false;

	m_excel.SetTemplete(m_strSavePath);
	if(m_excel.StartExcel() == false) return false;
#ifdef _DEBUG
	m_excel.Show();
#endif

	int nRowStart = 2;
	int nColStart = 1;
	//m_excel.InsertRow(nRowStart, vvTable.size());

	for(vector< vector<CString> >::const_iterator it = vvTable.begin(); it != vvTable.end(); it ++)
	{
		CStringArray strArr;
		vectorToStringArray(*it, strArr);
		m_excel.AddStringArr(nRowStart++, nColStart, strArr);

	}
	
 	m_excel.SaveWorkBook(m_excel.m_workbook);
 	m_excel.CloseWorkBook(m_excel.m_workbook);
 	m_excel.Quit();
 
 	if( IDYES == AfxMessageBox(_T("生成完毕！是否打开？") , MB_YESNO) )
 	{
 		ShellExecute(adsw_hwndAcad,_T("open"), m_strSavePath , _T(""),NULL,SW_SHOWNORMAL);
 	}

	return true;
}

bool	CExportSummaryTable::getAndFilterFrame_Indoor(vector<AcDbObjectIdArray> &vObjArrFrame)
{
	acutPrintf(_T("\n 请选择图框:"));
	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 2, _T("*LINE*"), _T("TK"), _T(""));
	if(objArr.length() == 0) return false;

	if(m_sReportType == IndoorReport_GroundAttachmentsTable || m_sReportType == IndoorReport_OwnerInfoTable
		|| m_sReportType == IndoorReport_GroundAttachmentsTable_standard2)
	{
		m_strSavePath = getPathName_AutoChange();
		if(m_strSavePath == _T("")) return false;
	}
	else
	{
		TCHAR szFilters[]=
			_T("xlsx (*.xlsx)|*.xlsx|All Files (*.*)|*.*||");
		CFileDialog fileDlg(FALSE,_T("xlsx"),_T("*.xlsx"),OFN_FILEMUSTEXIST
			|OFN_HIDEREADONLY,szFilters,NULL);
		if(fileDlg.DoModal() != IDOK) return false;
		m_strSavePath = fileDlg.GetPathName();

		//m_strSavePath = _T("D:\\12.xlsx");
	}


	SFilter filter;
	if(m_sReportType == IndoorReport_GroundAttachmentsTable 
		|| m_sReportType == IndoorReport_OwnerInfoTable
		|| m_sReportType == IndoorReport_OwnerInfoSummaryTable
		||  m_sReportType == IndoorReport_GroundAttachmentsTable_standard2
		|| m_sReportType == IndoorReport_OwnerInfoSummaryTable_standard2)
	{
		filter.filterObjectIdArray(objArr, _T("BH"), _T(""), STRING_TYPE, 0, 2);
		filter.ClassifyEntByXdata(objArr, vObjArrFrame, _T("BH"));
	}
// 	else
// 	{
// 		filter.filterObjectIdArray(objArr, _T("SFZH"), _T(""), STRING_TYPE, 0, 2);
// 		filter.ClassifyEntByXdata(objArr, vObjArrFrame, _T("SFZH"));
// 	}


	return true;
}

bool	CExportSummaryTable::getOwnerSummaryTemplateInfo()
{
	TCHAR szPath[1000] = {0};
	TCHAR szTempName[] = _T("village_hourses_area_summary_table_template.xlsx");
	ads_findfile(szTempName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n 找不到模板 ：%s"), szTempName);
		return false;
	}

	m_excel.SetTemplete(szPath);
	if(m_excel.StartExcel() == false) return false;

	long nRowCount = 0, nColCount = 0;
	int nColCode = 4, nColApp = 5, nColValue = 6, nColType = 7, nColFill = 8;
	int nRowStart = 3, nColStart = 4;
	m_excel.GetRowColCount(nRowCount, nColCount);
	CString strText;
	for(int i = nColStart; i <= nColCount; i ++)
	{
		strText = m_excel.GetRangeContent(nRowStart, i, m_excel.m_range);
		TrimString_Space(strText);
		if(strText == _T("")) break;

		m_vCode.push_back(strText);
		strText = m_excel.GetRangeContent(nRowStart + 1, i, m_excel.m_range);
		TrimString_Space(strText);
		m_vApp.push_back(strText);
		strText = m_excel.GetRangeContent(nRowStart + 2, i, m_excel.m_range);
		TrimString_Space(strText);
		m_vValue.push_back(strText);
		strText = m_excel.GetRangeContent(nRowStart + 3, i, m_excel.m_range);
		TrimString_Space(strText);
		m_vType.push_back(_ttoi(strText));

	}

	m_excel.Quit();

	return true;
}

bool	CExportSummaryTable::getOwnerInfoTableInfo()
{
	TCHAR szPath[1000] = {0};
	TCHAR szTempName[] = _T("hourse_area_summary_table_template.xlsx");
	ads_findfile(szTempName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n 找不到模板 ：%s"), szTempName);
		return false;
	}

	CTinyExcel excel;
	excel.SetTemplete(szPath);
	if(excel.StartExcel() == false) return false;

	long nRowCount = 0, nColCount = 0;
	int nRowCode = 3;
	int nColStart = 4;
	excel.GetRowColCount(nRowCount, nColCount);
	CString strText;
	for(int i = nColStart; i <= nColCount; i ++)
	{
		strText = excel.GetRangeContent(nRowCode, i, excel.m_range);
		TrimString_Space(strText);
		if(strText == _T("")) break;

		m_vCode.push_back(strText);
	}

	excel.Quit();

	return true;
}


bool	CExportSummaryTable::getGroundAttachmentsTableInfo()
{
	TCHAR szPath[1000] = {0};
	TCHAR szTempName[] = _T("attachments_upground_survey_template.xlsx");
	ads_findfile(szTempName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n 找不到模板 ：%s "), szTempName);
		return false;
	}

	CTinyExcel excel;
	excel.SetTemplete(szPath);
	if(excel.StartExcel() == false) return false;

	long nRowCount = 0, nColCount = 0;
	int nColCode = 4, nColApp = 5, nColValue = 6, nColType = 7, nColFill = 8;
	int nRowStart = 3;
	excel.GetRowColCount(nRowCount, nColCount);
	CString strText;
	for(int i = 0; i <= nRowCount; i ++)
	{
		strText = excel.GetRangeContent(i + nRowStart, nColCode, excel.m_range);
		TrimString_Space(strText);
		if(strText == _T("")) break;

		m_vCode.push_back(strText);
		strText = excel.GetRangeContent(i + nRowStart, nColApp, excel.m_range);
		TrimString_Space(strText);
		m_vApp.push_back(strText);
		strText = excel.GetRangeContent(i + nRowStart, nColValue, excel.m_range);
		TrimString_Space(strText);
		m_vValue.push_back(strText);
		strText = excel.GetRangeContent(i + nRowStart, nColType, excel.m_range);
		TrimString_Space(strText);
		m_vType.push_back(_ttoi(strText));

	}

	excel.CloseWorkBook(excel.m_workbook);
	excel.Quit();

	return true;
}

bool	CExportSummaryTable::getSummaryObjects(AcDbObjectIdArray objArr, AcDbObjectIdArray &objArrSummary)
{
	for(int i = 0; i < objArr.length(); i ++)
	{
		AcGePoint2dArray nodes;
		if(false == GetPlList(aname(objArr.at(i)), nodes)) continue;
		AcDbObjectIdArray objArrTemp;
		ssFromNodes(objArrTemp, nodes, 1);

		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("FW"), 0);
		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("FSWM"), 0);
		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("FSWL"), 0);
		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("SMZJ"), 0);
		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("SJZJ"), 0);
		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("QYZJ"), 0);
		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("NEQZJ"), 0);
		filterObjectIdArr_ByGISTable(objArrTemp, objArrSummary, _T("DMZJ"), 0);

	}
	if(objArrSummary.length() == 0) return false;

	return true;
}

bool	CExportSummaryTable::getRecord(AcDbObjectIdArray objArrSummary, vector<CString> &vStrResult)
{
	CString strText;
	AGrid_ClearSheet(_T("输出excel"));
	int nWarning = 0;
	for(size_t t = 0; t < m_vCode.size(); t ++)
	{
		strText = m_vCode.at(t);
		int nType = m_vType.at(t);
		CString strApp = m_vApp.at(t);
		CString strValue = m_vValue.at(t);

		double dValue = 0.0;
		double dLengthWell = 0.0, dHeightWell = 0.0;

		for(int i = 0; i < objArrSummary.length(); i ++)
		{
			AcDbObjectId objId = objArrSummary.at(i);
			TCHAR szStdm[100] = {0};
			GetStdm(aname(objId), szStdm);
			//如果模板上编码关键字有“|”，则判断有没有在其中，如果没有，则下一下
			if(strText.Find(_T("|")) >= 0)
			{
				CStringArray strArr;
				AnalyzeData(strArr, strText, _T("|"));
				if(_FindString(strArr, szStdm) < 0) continue;
			}
			else		//如果模板上没有“|”，则直接判断
			{
				if(0 != _tcscmp(szStdm, strText)) continue;
			}



			if(strApp != _T(""))
			{
				int nXType = STRING_TYPE;
				TCHAR szApp[500] = {0};
				_tcscpy(szApp, strApp);
				GetXDataType(objId, szApp, 0, nXType);
				TCHAR szValue[500] = {0};
				ReadXdata(aname(objId), strApp, nXType, szValue);
				if(0 == _tcscmp(szValue, _T("")))
				{
					nWarning++;
					TCHAR szMsg[200] = {0};
					_stprintf(szMsg, _T("附属地物扩展 %s 属性为空"), strApp);
					AGrid_AppendMsgEnt(_T("输出excel"),szMsg,_T(""), objId);
				}
				if(strValue != szValue)continue;
			}

			if(nType == 1)
			{
				double dArea2 = 0.0;
				//GetEntArea(aname(objId), dArea2);
				TCHAR szValue[500] = {0};
				ReadXdata(aname(objId), _T("ZJMJ"), STRING_TYPE, szValue);
				dArea2 = _tstof(szValue);

// 				//如果是FW层的还要乘以面积系数
// 				TCHAR szTable[200] = {0};
// 				g_index.get_table(szStdm, szTable);
// 				if(0 == _tcscmp(szTable, _T("FW")))
// 				{
// 					double dParam = 1.0;
// 					ReadXdataDouble(aname(objId), _T("MJXS"), dParam);
// 					dArea2 *= dParam;
// 				}

				dValue += dArea2;
			}
			else if(nType == 2)
			{
				double dArea2 = 0.0, dDeep = 0.0;
				// 				GetEntArea(aname(objId), dArea2);
				// 				ReadXdataDouble(aname(objId), _T("SD"), dDeep);
				// 				dArea2 *= dDeep;
				TCHAR szValue[500] = {0};
				ReadXdata(aname(objId), _T("ZJTJ"), STRING_TYPE, szValue);
				dArea2 = _tstof(szValue);
				dValue += dArea2;
			}
			else if(nType == 3)
			{
				double dLength = 0.0;
				//GetEntLength(aname(objId), dLength);
				TCHAR szValue[500] = {0};
				ReadXdata(aname(objId), _T("ZJCD"), STRING_TYPE, szValue);
				if(0 == _tcscmp(szValue, _T("")))
				{
					nWarning++;
					AGrid_AppendMsgEnt(_T("输出excel"), _T("附属物线没有注记长度！"),_T(""), objId);
				}
				dLength = _tstof(szValue);
				dValue += dLength;
			}
			else if(nType == 4)
			{
				short nCount = 0;
				ReadXdataShort(aname(objId), _T("SM"), nCount);
				dValue += nCount;
			}
			else if(nType == 5)
			{
				double dLength = 0.0, dHeight = 0.0;
				ReadXdataDouble(aname(objId), _T("CD"), dLength);
				ReadXdataDouble(aname(objId), _T("GD"), dHeight);
				dLengthWell += dLength;
				dHeightWell += dHeight;
			}

		}

		strText = _T("");
		if(nType == 1 || nType == 2 || nType == 3)
		{
			if(dValue > 0.0)strText.Format(_T("%.2lf"), dValue);
			else strText = _T("");
		}
		else if(nType == 4)
		{
			if(dValue > 0.0)	strText.Format(_T("%.0lf"), dValue);
			else strText = _T("");
		}
		else
		{
			if(dLengthWell > 0.0 ) strText.Format(_T("%.2lf"), dLengthWell);
			else strText = _T(""); 
		}

		vStrResult.push_back(strText);
	}

	if(nWarning) AGrid_ShowMe();

	return true;
}

bool	CExportSummaryTable::exportGroundAttachmentsTable_Single(AcDbObjectIdArray objArr)
{
	if(objArr.length() == 0) return false;
	
	TCHAR szPath[1000] = {0};
	TCHAR szTempName[] = _T("attachments_upground_survey_template.xlsx");
	ads_findfile(szTempName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n 找不到模板 ：%s "), szTempName);
		return false;
	}

	AcDbObjectIdArray objArrSummary;
	if(false == getSummaryObjects(objArr, objArrSummary)) return false;

	if(m_vCode.size() == 0) return false;

	long nRowCount = 0, nColCount = 0;
	int nRowStart = 3;
	int nColCode = 4, nColApp = 5, nColValue = 6, nColType = 7, nColFill = 8;
	vector<CString> vStrResult;
	
	getRecord(objArrSummary, vStrResult);

	TCHAR szBH[500] = {0};
	ReadXdata(aname(objArr.first()), _T("BH"), STRING_TYPE, szBH);
	CString strSavePath = m_strSavePath;
	strSavePath += _T("地上附着物调查登记表_");
	strSavePath += szBH;
	strSavePath += _T(".xlsx");

	CopyFile(szPath, strSavePath, false);
	
	m_excel.SetTemplete(strSavePath);

	if(m_excel.StartExcel() == false) return false;
//#ifdef _DEBUG
//	m_excel.Show();
//#endif
	
	CStringArray strArr;
	vectorToStringArray(vStrResult, strArr);
	m_excel.AddStringArr(nRowStart, nColFill, strArr, false);

	TCHAR szHZMC[500] = {0};
	vector<long> vR, vC;
	ReadXdata(aname(objArr.first()), _T("HZMC"), STRING_TYPE, szHZMC);
	m_excel.FindString(_T("#HZMC#"), vR, vC, szHZMC);
	m_excel.FindString(_T("#BH#"), vR, vC, szBH);

	//把模板上的标识行删除
	m_excel.DelCol(4);
	m_excel.DelCol(4);
	m_excel.DelCol(4);
	m_excel.DelCol(4);
	
	int nRowFirst = 0;
	int nRowEnd = 0, nStartCol = 0;
	vector<int> vInt;
	m_excel.FindString(_T("#ROW_START#"), nRowStart, nStartCol);
	m_excel.FindString(_T("#ROW_END#"), nRowEnd, nStartCol);
	nRowStart ++;
	nRowEnd --;
	if( IDYES == AfxMessageBox(_T("是否删除空白记录？") , MB_YESNO) )
	{
		for(int i = nRowEnd; i >= nRowStart; i--)
		{
			CString	str = m_excel.GetRangeContent(i, 4, m_excel.m_range);
			str.Trim();
			if(str	== _T(""))
				m_excel.DelRow(i);
		}
		
		m_excel.FindString(_T("#ROW_START#"), nRowStart, nStartCol);
		m_excel.FindString(_T("#ROW_END#"), nRowEnd, nStartCol);
		nRowStart ++;
		nRowEnd --;

		m_excel.MergeSameStringNeightbourCell2(nRowStart, 2, nRowEnd, vInt, m_excel.m_range, false);

		int nNum = 1;
		for(int i = nRowStart; i <=nRowEnd;)
		{
			TCHAR szNum[100] = {0};
			_stprintf(szNum, _T("%d"), nNum);
			m_excel.SetRangeContent(i, 1, szNum, m_excel.m_range);

			int nMRow = 0, nMCol = 0;
			m_excel.GetMergedRowsAndCols(nRowStart, 1, nMRow, nMCol);
			i += nMRow;
			nNum++;
		}
	}
	else
	{
		m_excel.MergeSameStringNeightbourCell2(nRowStart, 2, nRowEnd, vInt, m_excel.m_range, false);
	}

	vector<long> vRow, vCol;
	m_excel.FindString(_T("#ROW_START#"), vRow, vCol, _T(" "));
	m_excel.FindString(_T("#ROW_END#"), vRow, vCol, _T(" "));




	m_excel.SaveWorkBook(m_excel.m_workbook);
	m_excel.CloseWorkBook(m_excel.m_workbook);
	m_excel.Quit();

	return true;
}


bool	CExportSummaryTable::exportGroundAttachmentsTable()
{
	
	vector<AcDbObjectIdArray> vObjArrFrame;
	if(false == getAndFilterFrame_Indoor(vObjArrFrame)) return false;

	if(false == getGroundAttachmentsTableInfo()) return false;

	for(size_t t = 0; t < vObjArrFrame.size(); t ++)
	{
		exportGroundAttachmentsTable_Single(vObjArrFrame.at(t));
	}

	if( IDYES == AfxMessageBox(_T("生成完毕！是否打开？") , MB_YESNO) )
	{
		ShellExecute(adsw_hwndAcad,_T("open"), m_strSavePath , _T(""),NULL,SW_SHOWNORMAL);
	}

	return true;
}

bool	CExportSummaryTable::exportOwnerInfoTable_Single(AcDbObjectIdArray objArr)
{
	if(objArr.length() == 0) return false;

	TCHAR szPath[1000] = {0};
	TCHAR szTempName[] = _T("hourse_area_summary_table_template.xlsx");
	ads_findfile(szTempName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n 找不到模板 ：%s"), szTempName);
		return false;
	}

	//先统计房屋层的建筑
	AcDbObjectIdArray objArrSummary, objArrSummaryOthers;
	vector<CString> vCode;
	CStringArray strArr;
	CString strText = m_vCode.at(0);
	if(strText.Find(_T("|")) >= 0)
	{
		AnalyzeData(strArr, strText, _T("|"));
	}
	else
	{
		strArr.Add(strText);
	}
	for(int i = 0; i < objArr.length(); i ++)
	{
		AcGePoint2dArray nodes;
		if(false == GetPlList(aname(objArr.at(i)), nodes)) continue;
		AcDbObjectIdArray objArrTemp, objArrTemp2;
		ssFromNodes(objArrTemp, nodes, 1);
			
		for(int j = 0; j < objArrTemp.length(); j ++)
		{
			TCHAR szCode[100] = {0};
			GetStdm(aname(objArrTemp.at(j)), szCode);

			if(_FindString(strArr, szCode) >= 0)
				objArrSummary.append(objArrTemp.at(j));			//房屋类
			else
				objArrSummaryOthers.append(objArrTemp.at(j));//其它类（附属）
		}
	}

	if(objArrSummary.length() == 0) return false;
	SFilter filter;
	filter.SortIdByXData(objArrSummary, _T("FWBH"), INT_TYPE);

	TCHAR szBH[500] = {0};
	ReadXdata(aname(objArr.first()), _T("BH"), STRING_TYPE, szBH);
	CString strSavePath = m_strSavePath;
	strSavePath += _T("房屋面积一览表_");
	strSavePath += szBH;
	strSavePath += _T(".xlsx");

	CopyFile(szPath, strSavePath, false);

	CTinyExcel excel;
	excel.SetTemplete(strSavePath);
	if(excel.StartExcel() == false) return false;
#ifdef _DEBUG
	excel.Show();
#endif

	int nRowStart = 4;
	excel.InsertRow(nRowStart, objArrSummary.length());

	CStringArray strArr1, strArr2, strArr3, strArr4, strArr5, strArr6, strArr7;
	
	int nColFWBH = 1, nColFloorCount = 2, nColStruct = 3, nColAreaHourse = 4, nColBalcony = 5, 
		nColXiaZi = 6, nColStep = 7, nColStair = 8, nColEaves = 9, nColBasement = 10;

	for(int i = 0; i < objArrSummary.length(); i ++)
	{
		AcDbObjectId objId = objArrSummary.at(i);
		short sFWBH = 0;
		ReadXdataShort(aname(objId), _T("FWBH"), sFWBH);
		strText.Format(_T("(%d)"), sFWBH);
		excel.SetRangeContent(nRowStart + i, nColFWBH, strText, excel.m_range);

		int nFloor = GetBuildFloor(aname(objId));
		strText.Format(_T("%d"), nFloor);
		excel.SetRangeContent(nRowStart + i, nColFloorCount, strText, excel.m_range);

		//strText = getBuildStruct_FCS(objId);
		strText = getBuildStruct_FCS_Config(objId);
		excel.SetRangeContent(nRowStart + i, nColStruct, strText, excel.m_range);

		double dArea = 0.0, dMJXS = 1.0;
// 		GetEntArea(aname(objId), dArea);
// 		ReadXdataDouble(aname(objId), _T("MJXS"), dMJXS);
// 		dArea *= dMJXS;
// 		dArea *= nFloor;
// 		strText.Format(_T("%.5lf"), dArea);
		TCHAR szValue[200] = {0};
		ReadXdata(aname(objId), _T("ZJMJ"), STRING_TYPE, szValue);
		strText = szValue;
		excel.SetRangeContent(nRowStart + i, nColAreaHourse, strText, excel.m_range);


	}

	//开始追加除房屋外的附属地物面积
	vector<double> vAreas;
	vAreas.resize(m_vCode.size() - 1);
	for(size_t t = 0; t < vAreas.size(); t ++) vAreas.at(t) = 0.0;

	for(int i = 0; i < objArrSummaryOthers.length(); i ++)
	{
		AcDbObjectId objId = objArrSummaryOthers.at(i);
		TCHAR szCode[100] = {0};
		GetStdm(aname(objId), szCode);

		bool bFind = false;
		int nIndex = 0;
		for(int t = 1; t < (int)m_vCode.size(); t++)
		{
			strArr.RemoveAll();
			if(m_vCode.at(t).Find(_T("|")))
			{
				AnalyzeData(strArr, m_vCode.at(t), _T("|"));
				if(_FindString(strArr, szCode) >= 0)
				{
					bFind = true;
					nIndex = t - 1;
					break;
				}
			}
			else
			{
				if(0 == _tcscmp(szCode, m_vCode.at(t)))
				{
					bFind = true;
					nIndex = t - 1;
					break;
				}
			}
		}

		if(bFind == true)
		{
			TCHAR szValue[200] = {0};
			ReadXdata(aname(objId), _T("ZJMJ"), STRING_TYPE, szValue);
			vAreas.at(nIndex) += _tstof(szValue);
		}

	}

	if(vAreas.at(0))
	{
		strText = format_double_2(vAreas.at(0), 2);
		excel.SetRangeContent(nRowStart, nColBalcony, strText, excel.m_range);
	}
	if(vAreas.at(1))
	{
		strText = format_double_2(vAreas.at(1), 2);
		excel.SetRangeContent(nRowStart, nColXiaZi, strText, excel.m_range);
	}
	if(vAreas.at(2))
	{
		strText = format_double_2(vAreas.at(2), 2);
		excel.SetRangeContent(nRowStart, nColStep, strText, excel.m_range);
	}
	if(vAreas.at(3))
	{
		strText = format_double_2(vAreas.at(3), 2);
		excel.SetRangeContent(nRowStart, nColStair, strText, excel.m_range);
	}
	if(vAreas.at(4))
	{
		strText = format_double_2(vAreas.at(4), 2);
		excel.SetRangeContent(nRowStart, nColEaves, strText, excel.m_range);
	}
	if(vAreas.at(5))
	{
		strText = format_double_2(vAreas.at(5), 2);
		excel.SetRangeContent(nRowStart, nColBasement, strText, excel.m_range);
	}

	//把配置信息行删除
	excel.DelRow(nRowStart - 1);

	TCHAR szXMZL[500] = {0}, szHZMC[500] = {0};
	vector<long> vR, vC;
	ReadXdata(aname(objArr.first()), _T("XMZL"), STRING_TYPE, szXMZL);
	ReadXdata(aname(objArr.first()), _T("HZMC"), STRING_TYPE, szHZMC);
	excel.FindString(_T("#XMZL#"), vR, vC, szXMZL);
	excel.FindString(_T("#HZMC#"), vR, vC, szHZMC);
	excel.FindString(_T("#BH#"), vR, vC, szBH);

	

// 	CStringArray strArr;
// 	vectorToStringArray(vStrResult, strArr);
// 	excel.AddStringArr(nRowStart, nColFill, strArr, false);

	excel.SaveWorkBook(excel.m_workbook);
	excel.CloseWorkBook(excel.m_workbook);
	excel.Quit();

	return true;
}

bool CExportSummaryTable::exportOwnerInfoTable()
{
	vector<AcDbObjectIdArray> vObjArrFrame;
	if(false == getAndFilterFrame_Indoor(vObjArrFrame)) return false;

	if(false == getOwnerInfoTableInfo()) return false;

	for(size_t t = 0; t < vObjArrFrame.size(); t ++)
	{
		exportOwnerInfoTable_Single(vObjArrFrame.at(t));
	}

	if( IDYES == AfxMessageBox(_T("生成完毕！是否打开？") , MB_YESNO) )
	{
		ShellExecute(adsw_hwndAcad,_T("open"), m_strSavePath , _T(""),NULL,SW_SHOWNORMAL);
	}

	return true;
}

bool	CExportSummaryTable::exportOwnerInfoSummaryTable_Single(AcDbObjectIdArray objArr)
{
	if(objArr.length() == 0) return false;

	AcDbObjectIdArray objArrSummary;
	if(false == getSummaryObjects(objArr, objArrSummary)) return false;

	if(m_vCode.size() == 0) return false;

	int nColNum = 1, nColName = 2, nColID = 3, nColStart = 4;
	TCHAR szNum[50] = {0}, szName[200] = {0}, szID[200] = {0};
	CString strText;
	ReadXdata(aname(objArr.first()	), _T("BH"), STRING_TYPE, szNum);
	strText.Format(_T("%s"), szNum);
	m_excel.SetRangeContent(m_nStartRow, nColNum, strText, m_excel.m_range);
	ReadXdata(aname(objArr.first()	), _T("HZMC"), STRING_TYPE, szName);
	m_excel.SetRangeContent(m_nStartRow, nColName, szName, m_excel.m_range);
	ReadXdata(aname(objArr.first()	), _T("SFZH"), STRING_TYPE, szID);
	m_excel.SetRangeContent(m_nStartRow, nColID, szID, m_excel.m_range);

	long nRowCount = 0, nColCount = 0;
	int nColFill = 4;
	
	vector<CString> vStrResult;
	getRecord(objArrSummary, vStrResult);	

	CStringArray strArr;
	vectorToStringArray(vStrResult, strArr);
	m_excel.AddStringArr(m_nStartRow++, nColFill, strArr);

	return true;
}


bool CExportSummaryTable::exportOwnerInfoSummaryTable()
{
	vector<AcDbObjectIdArray> vObjArrFrame;
	if(false == getAndFilterFrame_Indoor(vObjArrFrame)) return false;

	if(false == getOwnerSummaryTemplateInfo()) return false;

	TCHAR szPath[1000] = {0};
	TCHAR szTempName[] = _T("village_hourses_area_summary_table_template.xlsx");
	ads_findfile(szTempName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n 找不到模板 ：%s"), szTempName);
		return false;
	}

	CopyFile(szPath, m_strSavePath, false);
	m_excel.SetTemplete(m_strSavePath);
	if(m_excel.StartExcel() == false) return false;
#ifdef _DEBUG
	m_excel.Show();
#endif

	AGrid_ClearSheet(_T("房屋面积汇总"));
	m_nWarning = 0;
	m_nStartRow = 7;
	for(size_t t = 0; t < vObjArrFrame.size(); t ++)
	{
		exportOwnerInfoSummaryTable_Single(vObjArrFrame.at(t));
		
	}
	if(m_nWarning) AGrid_ShowMe();

	//把模板上的标识行删除
	m_excel.DelRow(3);
	m_excel.DelRow(3);
	m_excel.DelRow(3);
	m_excel.DelRow(3);

	
	m_excel.SaveWorkBook(m_excel.m_workbook);
	m_excel.CloseWorkBook(m_excel.m_workbook);
	m_excel.Quit();

	if( IDYES == AfxMessageBox(_T("生成完毕！是否打开？") , MB_YESNO) )
	{
		ShellExecute(adsw_hwndAcad,_T("open"), m_strSavePath , _T(""),NULL,SW_SHOWNORMAL);
	}

	return true;
}



bool	CExportSummaryTable::readCSVFile()
{
	CTinyExcel excel;
	excel.SetTemplete(m_strCSVPath);
	if(excel.StartExcel() == false) return false;

	long nRows = 0, nCols = 0;
	int nCol0 = 1, nCol1 = 2, nCol2 = 3, nCol3 = 6;		//楼名，标注，点号，Z
	excel.GetRowColCount(nRows, nCols);

	vector<ElevationInfo> vElevInfo;
	set<CString>			vFloorNames;
	CString strText, strText2;
	for(int i = 2; i < nRows; i ++)
	{
		strText = excel.GetRangeContent(i, nCol0, excel.m_range);
		TrimString_Space(strText);
		if(strText == _T("")) continue;

		vFloorNames.insert(strText);

		ElevationInfo elevInfo;
		elevInfo.strFloorName = strText;
		elevInfo.strName = excel.GetRangeContent(i, nCol1, excel.m_range);
		elevInfo.strPointName = excel.GetRangeContent(i, nCol2, excel.m_range);
		elevInfo.dHeight = _tstof(excel.GetRangeContent(i, nCol3, excel.m_range));
		vElevInfo.push_back(elevInfo);
	}

	excel.CloseWorkBook(excel.m_workbook);
	excel.Quit();

	for(set<CString>::iterator itS = vFloorNames.begin(); itS != vFloorNames.end(); itS ++)
	{
		strText = *itS;
		CSVInfo csv;
		for(vector<ElevationInfo>::iterator it = vElevInfo.begin(); it != vElevInfo.end(); it ++)
		{
			if(strText == it->strFloorName)
			{
				
				csv.strFloorName = strText;
				strText2 = it->strName;
				ElevationInfo elevInfo = *it;
				if(strText2.Find(_T("室内坪")) >= 0)
				{
					csv.vIndoorHeight.push_back(elevInfo);
				}
				else if(strText2.Find(_T("楼高")) >= 0)
				{
					csv.vDaughterHeight.push_back(elevInfo);
				}
				else if(strText2.Find(_T("机房间高")) >= 0)
				{
					csv.vMachineHeight.push_back(elevInfo);
				}
				else
				{
					csv.vOtherHeight.push_back(elevInfo);
				}

			}
		}

		sortSingleCSVRecord(csv);

		m_vCSVInfo.push_back(csv);

		
	}

	
	return true;
}

void CExportSummaryTable::sortSingleCSVRecord(CSVInfo &csv)
{
	set<CString> sStr;
	for(size_t t = 0; t < csv.vIndoorHeight.size(); t ++)
	{
		sStr.insert(csv.vIndoorHeight.at(t).strName);
	}
	vector<ElevationInfo> vElev;
	for(set<CString>::iterator itS = sStr.begin(); itS != sStr.end()	; itS ++)
	{
		for(vector<ElevationInfo>::iterator it = csv.vIndoorHeight.begin(); it != csv.vIndoorHeight.end(); it ++)
		{
			if(*itS == it->strName)
			{
				vElev.push_back(*it);
			}
		}
	}
	csv.vIndoorHeight.swap(vElev);
	vElev.clear();

	sStr.clear();
	for(size_t t = 0; t < csv.vDaughterHeight.size(); t ++)
	{
		sStr.insert(csv.vDaughterHeight.at(t).strName);
	}
	for(set<CString>::iterator itS = sStr.begin(); itS != sStr.end()	; itS ++)
	{
		for(vector<ElevationInfo>::iterator it = csv.vDaughterHeight.begin(); it != csv.vDaughterHeight.end(); it ++)
		{
			if(*itS == it->strName)
			{
				vElev.push_back(*it);
			}
		}
	}
	csv.vDaughterHeight.swap(vElev);
	vElev.clear();

	sStr.clear();
	for(size_t t = 0; t < csv.vMachineHeight.size(); t ++)
	{
		sStr.insert(csv.vMachineHeight.at(t).strName);
	}
	for(set<CString>::iterator itS = sStr.begin(); itS != sStr.end()	; itS ++)
	{
		for(vector<ElevationInfo>::iterator it = csv.vMachineHeight.begin(); it != csv.vMachineHeight.end(); it ++)
		{
			if(*itS == it->strName)
			{
				vElev.push_back(*it);
			}
		}
	}
	csv.vMachineHeight.swap(vElev);
	vElev.clear();

	sStr.clear();
	for(size_t t = 0; t < csv.vOtherHeight.size(); t ++)
	{
		sStr.insert(csv.vOtherHeight.at(t).strName);
	}
	for(set<CString>::iterator itS = sStr.begin(); itS != sStr.end()	; itS ++)
	{
		for(vector<ElevationInfo>::iterator it = csv.vOtherHeight.begin(); it != csv.vOtherHeight.end(); it ++)
		{
			if(*itS == it->strName)
			{
				vElev.push_back(*it);
			}
		}
	}
	csv.vOtherHeight.swap(vElev);
	vElev.clear();

}


bool	CExportSummaryTable::fillFeatureTable_Floor(CSVInfo csv)
{
	int nRowIndoor = 0, nColIndoor = 0;
	int nColPointName = 3, nColHeight = 4, nColNoteName = 8;
	int nRowTemp = 0;
	if(false == m_excel.FindString(_T("#INDOOR_PART#"), nRowIndoor, nColIndoor)) return false;
	if ((int)csv.vIndoorHeight.size() > 2)
		m_excel.InsertRow(nRowIndoor, (int)csv.vIndoorHeight.size() - 2);

	nRowTemp = nRowIndoor;
	for(size_t t = 0; t < csv.vIndoorHeight.size(); t ++)
	{
		m_excel.SetRangeContent(nRowTemp, nColPointName, csv.vIndoorHeight.at(t).strPointName, m_excel.m_range);
		TCHAR szText[100] = {0};
		_stprintf(szText, _T("%lf"), csv.vIndoorHeight.at(t).dHeight);
		m_excel.SetRangeContent(nRowTemp, nColHeight, szText, m_excel.m_range);
		nRowTemp ++;
	}

	set<CString> vStrTemp;
	for(size_t t = 0; t < csv.vDaughterHeight.size(); t ++)
		vStrTemp.insert(csv.vDaughterHeight.at(t).strName);

	if(vStrTemp.size() > 1)
	{
// 		msexcel::Range rangeCopy;
 		if(false == m_excel.FindString(_T("#DAUGHTER_PART#"), nRowIndoor, nColIndoor)) return false;
// 		m_excel.copyRows(nRowIndoor, nRowIndoor + 1, rangeCopy);

		for(size_t t = 1; t < vStrTemp.size(); t ++)
		{
			//m_excel.InsertRow(nRowIndoor, rangeCopy, false);
			m_excel.InsertRow(nRowIndoor + 1, 1);
			m_excel.InsertRow(nRowIndoor + 1, 1);
			m_excel.CopyCells(nRowIndoor, nColHeight, nRowIndoor + 1, nColNoteName, nRowIndoor + 2, nColHeight,
				m_excel.m_worksheet, m_excel.m_worksheet);
		}
		

		for(set<CString>::iterator itS = vStrTemp.begin(); itS != vStrTemp.end(); itS ++)
		{
			vector<ElevationInfo> vElev;
			for(vector<ElevationInfo>::iterator it = csv.vDaughterHeight.begin()	; it != csv.vDaughterHeight.end(); it ++)
			{
				if(*itS == it->strName)
				{
					vElev.push_back(*it);
				}
			}

			if(false == m_excel.FindString(_T("#DAUGHTER_PART#"), nRowIndoor, nColIndoor)) return false;
			if ((int)vElev.size() > 2)
				m_excel.InsertRow(nRowIndoor, (int)vElev.size() - 2);

			nRowTemp = nRowIndoor;
			for(size_t t = 0; t < vElev.size(); t ++)
			{
				m_excel.SetRangeContent(nRowTemp, nColPointName, vElev.at(t).strPointName, m_excel.m_range);
				TCHAR szText[100] = {0};
				_stprintf(szText, _T("%lf"), vElev.at(t).dHeight);
				m_excel.SetRangeContent(nRowTemp, nColHeight, szText, m_excel.m_range);
				nRowTemp ++;
			}

		}
		
	}
	else
	{
		if(false == m_excel.FindString(_T("#DAUGHTER_PART#"), nRowIndoor, nColIndoor)) return false;
		if(csv.vDaughterHeight.size() > 2)
			m_excel.InsertRow(nRowIndoor, (int)csv.vDaughterHeight.size() - 2);

		nRowTemp = nRowIndoor;
		for(size_t t = 0; t < csv.vDaughterHeight.size(); t ++)
		{
			m_excel.SetRangeContent(nRowTemp, nColPointName, csv.vDaughterHeight.at(t).strPointName, m_excel.m_range);
			TCHAR szText[100] = {0};
			_stprintf(szText, _T("%lf"), csv.vDaughterHeight.at(t).dHeight);
			m_excel.SetRangeContent(nRowTemp, nColHeight, szText, m_excel.m_range);
			nRowTemp ++;
		}
	}
	
	vector<long> vRows, vCols;
	m_excel.FindString(_T("#INDOOR_PART#"), vRows, vCols, _T(" "));
	m_excel.FindString(_T("#DAUGHTER_PART#"), vRows, vCols, _T(" "));
	m_excel.FindString(_T("#MACHINE_PART#"), vRows, vCols, _T(" "));
	m_excel.FindString(_T("#OTHERS#"), vRows, vCols, _T(" "));

	int nRowFS = 0, nColFS = 0, nRowFE = 0, nColFE = 0;
	m_excel.FindString(_T("#FLOORNAME_S#"), nRowFS, nColFS);
	m_excel.FindString(_T("#FLOORNAME_E#"), nRowFE, nColFE);
	m_excel.FindString(_T("#FLOORNAME_E#"), vRows, vCols, _T(" "));
	m_excel.MergeRange(nRowFS, nColFS, nRowFE, nColFE, m_excel.m_range);

	int nMergeCos = 0;
	m_excel.GetMergedRowsAndCols(nRowFS, nColFS, m_nMergedRow, nMergeCos);

	return true;
}

void CExportSummaryTable::setFloorFormal(int nRowBegin, int nFloorRecordCount, int nHaveIndoorHeight)
{
	int nColFloorName = 1, nColPointName = 3, nColHeight = 4, nColHeightAverage = 5, nColFloorHeight = 7, nColNoteName = 8;
	int nRowTemp = nRowBegin + 1;
	int nRowIndoorHeight = 0;
	vector<int> vMergedRowIndex;
	m_excel.MergeSameStringNeightbourCell(nRowBegin, nColNoteName, nRowBegin + nFloorRecordCount - 1, vMergedRowIndex, m_excel.m_range);
	
	for(size_t t = 0; t < vMergedRowIndex.size(); t ++)
	{
		int nRowMerge = vMergedRowIndex.at(t);
		int nRowM = 0, nColM = 0;
		m_excel.GetMergedRowsAndCols(nRowMerge, nColNoteName, nRowM, nColM);

		CString strFormaula;
		strFormaula.Format(_T("=AVERAGE(D%d:D%d)"), nRowMerge, nRowMerge + nRowM - 1);
		m_excel.SetRangeContent(nRowMerge, nColHeightAverage, strFormaula, m_excel.m_range);
		m_excel.MergeRange(nRowMerge, nColHeightAverage, nRowMerge + nRowM - 1, nColHeightAverage, m_excel.m_range);

		if(nHaveIndoorHeight)
		{
			if(t == 0) 
			{
				nRowIndoorHeight = nRowMerge;
				continue;
			}

			strFormaula.Format(_T("=E%d - E%d"), nRowMerge, nRowIndoorHeight);
			m_excel.SetRangeContent(nRowMerge, nColFloorHeight, strFormaula, m_excel.m_range);
			m_excel.MergeRange(nRowMerge, nColFloorHeight, nRowMerge + nRowM - 1, nColFloorHeight, m_excel.m_range);

		}
	}

	m_excel.MergeRange(nRowBegin, nColFloorName, nRowBegin + nFloorRecordCount - 1, nColFloorName, m_excel.m_range);

}


bool	CExportSummaryTable::fillFeatureTable()
{
	CopyFile(m_strFeatureTableTempPath, m_strSavePath, false);
	m_excel.SetTemplete(m_strSavePath);
	if(m_excel.StartExcel() == false) return false;
#ifdef _DEBUG
	m_excel.Show();
#endif

	int nRowCopy1 = 1, nRowCopy2 = 12;
	int nRowStart = 3;
	int nRowTemp = 0;
	int nRowsCopyCount = 12;
	int nColFloorName = 1, nColPointName = 3, nColHeight = 4, nColNoteName = 8;

	int nCount = 0;
	for(vector<CSVInfo>::iterator it = m_vCSVInfo.begin(); it != m_vCSVInfo.end(); it ++)
	{
		nCount += (int)it->vIndoorHeight.size();
		nCount += (int)it->vDaughterHeight.size();
		nCount += (int)it->vMachineHeight.size();
		nCount += (int)it->vOtherHeight.size();
	}
	m_excel.InsertRow(nRowStart, nCount - 3);	//这里少插3行，因为模板已经有3行了

	nRowTemp = nRowStart;
	for(vector<CSVInfo>::iterator it = m_vCSVInfo.begin(); it != m_vCSVInfo.end(); it ++)
	{
		int nRowBegin = nRowTemp;
		m_excel.SetRangeContent(nRowTemp, nColFloorName, it->strFloorName, m_excel.m_range);
		for(vector<ElevationInfo>::iterator itt = it->vIndoorHeight.begin(); itt != it->vIndoorHeight.end(); itt ++)
		{
			m_excel.SetRangeContent(nRowTemp, nColPointName, itt->strPointName, m_excel.m_range);
			TCHAR szText[100] = {0};
			_stprintf(szText, _T("%lf"), itt->dHeight);
			m_excel.SetRangeContent(nRowTemp, nColHeight, szText, m_excel.m_range);
			m_excel.SetRangeContent(nRowTemp, nColNoteName, itt->strName, m_excel.m_range);

			nRowTemp ++;
		}
		for(vector<ElevationInfo>::iterator itt = it->vDaughterHeight.begin(); itt != it->vDaughterHeight.end(); itt ++)
		{
			m_excel.SetRangeContent(nRowTemp, nColPointName, itt->strPointName, m_excel.m_range);
			TCHAR szText[100] = {0};
			_stprintf(szText, _T("%lf"), itt->dHeight);
			m_excel.SetRangeContent(nRowTemp, nColHeight, szText, m_excel.m_range);
			m_excel.SetRangeContent(nRowTemp, nColNoteName, itt->strName, m_excel.m_range);

			nRowTemp ++;
		}
		for(vector<ElevationInfo>::iterator itt = it->vMachineHeight.begin(); itt != it->vMachineHeight.end(); itt ++)
		{
			m_excel.SetRangeContent(nRowTemp, nColPointName, itt->strPointName, m_excel.m_range);
			TCHAR szText[100] = {0};
			_stprintf(szText, _T("%lf"), itt->dHeight);
			m_excel.SetRangeContent(nRowTemp, nColHeight, szText, m_excel.m_range);
			m_excel.SetRangeContent(nRowTemp, nColNoteName, itt->strName, m_excel.m_range);

			nRowTemp ++;
		}
		for(vector<ElevationInfo>::iterator itt = it->vOtherHeight.begin(); itt != it->vOtherHeight.end(); itt ++)
		{
			m_excel.SetRangeContent(nRowTemp, nColPointName, itt->strPointName, m_excel.m_range);
			TCHAR szText[100] = {0};
			_stprintf(szText, _T("%lf"), itt->dHeight);
			m_excel.SetRangeContent(nRowTemp, nColHeight, szText, m_excel.m_range);
			m_excel.SetRangeContent(nRowTemp, nColNoteName, itt->strName, m_excel.m_range);

			nRowTemp ++;
		}

		setFloorFormal(nRowBegin, nRowTemp - nRowBegin, (int)it->vIndoorHeight.size());

	}


// 	m_excel.SaveWorkBook(m_excel.m_workbook);
// 	m_excel.Quit();

	return true;
}



bool	CExportSummaryTable::extractFeatureTable()
{
	CFileDialog openFD(TRUE,_T("*.csv"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("原始坐标文件(*.csv)|*.csv|原始坐标文件(*.csv)|*.csv||"));
	if (openFD.DoModal() != IDOK) return false;
	m_strCSVPath = openFD.GetPathName();

	TCHAR szPath[1000] = {0};
	//TCHAR szName[] = _T("要素计算表_模板.xlsx");
	TCHAR szName[] = _T("feature_calculation_template.xlsx");
	ads_findfile(szName, szPath);
	if(0 == _tcscmp(szPath, _T(""))) 
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szName);
		return false;
	}
	m_strFeatureTableTempPath = szPath;

	m_strSavePath = getPathName_AutoChange();
	if(m_strSavePath == _T("")) return false;
	m_strSavePath += _T("feature_table.xlsx");

	readCSVFile();

	fillFeatureTable();

	m_excel.SaveWorkBook(m_excel.m_workbook);
	m_excel.CloseWorkBook(m_excel.m_workbook);
	m_excel.Quit();

	if( IDYES == AfxMessageBox(_T("生成完毕！是否打开？") , MB_YESNO) )
	{
		ShellExecute(adsw_hwndAcad,_T("open"), m_strSavePath , _T(""),NULL,SW_SHOWNORMAL);
	}

	return true;
}

bool	CExportSummaryTable::readSourceDwg()
{

	// 源图形数据库  
	AcDbDatabase* pExternalDb = new AcDbDatabase(false);
	if(Acad::eOk != pExternalDb->readDwgFile(m_strSourceDwg)) return false;

	AcDbBlockTable* pBlockTable;
	pExternalDb->getBlockTable(pBlockTable,AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForRead);
	AcDbBlockTableRecordIterator* pItr;
	pBlockTableRecord->newIterator(pItr);

	vector<CSVInfo>	vCSVInfo;

	AcDbObjectIdArray list;  // 导出到临时图形数据库的实体数组 
	for (pItr->start(); !pItr->done(); pItr->step()) 
	{ 
		AcDbEntity* pEnt;
		pItr->getEntity(pEnt, AcDb::kForRead);
		//AcDbObjectId objId = pEnt->objectId();
		//pEnt->close();
		TCHAR szStdm[100] = {0};
		ReadEntityXdata((AcDbObject*)pEnt, _T("SOUTH"), STRING_TYPE, szStdm);
		if(0 == _tcscmp(szStdm, CASSCODE_480410))
		{
			ReadEntityXdata((AcDbObject*)pEnt, XDATA_TXTZ, STRING_TYPE, szStdm);
			
			if(0 == _tcscmp(szStdm, _T("总建筑面积")))
			{
				CSVInfo csv;
				TCHAR szText[500] = {0};
				ReadEntityXdata((AcDbObject*)pEnt, _T("BUILDING_NAME"), STRING_TYPE, szText);
				csv.strFloorName = szText;
				ReadEntityXdata((AcDbObject*)pEnt, _T("AREA_TOL"), STRING_TYPE, szText);
				csv.dArea_Tol = _tstof(szText);
				ReadEntityXdata((AcDbObject*)pEnt, _T("AREA_TOL_TOP"), STRING_TYPE, szText);
				csv.dArea_TolTop = _tstof(szText);
				ReadEntityXdata((AcDbObject*)pEnt, _T("AREA_TOL_UNDER"), STRING_TYPE, szText);
				csv.dArea_TolUnder = _tstof(szText);
				ReadEntityXdata((AcDbObject*)pEnt, _T("FLOOR_COUNT_TOP"), STRING_TYPE, szText);
				csv.dFloorCount_Top = _ttoi(szText);
				ReadEntityXdata((AcDbObject*)pEnt, _T("FLOOR_COUNT_UNDER"), STRING_TYPE, szText);
				csv.dFloorCount_Under = _ttoi(szText);
				vCSVInfo.push_back(csv);
			}
		}
		
		
	}
	delete pItr; 
	pBlockTableRecord->close();     
	pBlockTable->close();

	readSourceXRecord(pExternalDb);

	delete pExternalDb;

	for(vector<CSVInfo>::iterator it = m_vCSVInfo.begin(); it != m_vCSVInfo.end(); it ++)
	{
		bool bFind = false;
		for(vector<CSVInfo>::iterator itTemp = vCSVInfo.begin(); itTemp != vCSVInfo.end(); itTemp ++)
		{
			if(itTemp->strFloorName == it->strFloorName)
			{
				it->dArea_Tol = itTemp->dArea_Tol;
				it->dArea_TolTop = itTemp->dArea_TolTop;
				it->dArea_TolUnder = itTemp->dArea_TolUnder;
				it->dFloorCount_Top = itTemp->dFloorCount_Top;
				it->dFloorCount_Under =itTemp->dFloorCount_Under;
				bFind = true;
				break;
			}
		}

		if(bFind == false)
		{
			it->dArea_Tol = INVALID_VALUE;
			it->dArea_TolTop = INVALID_VALUE;
			it->dArea_TolUnder = INVALID_VALUE;
			it->dFloorCount_Top = INVALID_VALUE;
			it->dFloorCount_Under = INVALID_VALUE;

			acutPrintf(_T("\n 要素表记录 %s 找不到分层图数据，没有统计建筑分层？"), it->strFloorName);
		}

		

	}


	return true;
}

bool	CExportSummaryTable::readSourceXRecord(AcDbDatabase *pExternalDb)
{
	CXRecordOperate xrecordO;
	CString strText;
	CArray<ATTRFIELD,ATTRFIELD> *AttrArr = NULL;
	if(findAttriTable(_T("XMXX"), AttrArr))
	{
		for(int j = 0; j < AttrArr->GetSize(); j ++)
		{
			m_vStrKey.push_back(AttrArr->GetAt(j).name);

			//xrecordO.ReadStringRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, strText);

			struct resbuf *pRb = NULL;
			strText = _T("");
			if (!xrecordO.ReadXRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, &pRb, 0, pExternalDb))
			{
				acutRelRb(pRb);
				continue;
			}
			strText.Format(_T("%s"), pRb->resval.rstring);
			acutRelRb(pRb);
			m_vStrValue.push_back(strText);
		}
	}

	return true;	
}


bool	CExportSummaryTable::readSourceXML()
{
	TCHAR szPath[1000] = {0};
	TCHAR szName[] = _T("建筑信息表.XML");
	ads_findfile(szName, szPath);
	if(0 == _tcscmp(szPath, _T(""))) 
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szName);
		return false;
	}

	TiXmlDocument tinyXml;
	char szTemp1[1000] = {0};
	bool es = false;
#ifdef _UNICODE
	TcharToChar(szPath, szTemp1);
	es = tinyXml.LoadFile(szTemp1, TIXML_DEFAULT_ENCODING );
#else
	//XML文件最后保存成UTF-8格式，然后内部编码采用UTF-8编码，不然会读取中文乱码
	_tcscpy(szTemp1,szPath);
	es = tinyXml.LoadFile(szTemp1, TIXML_ENCODING_UTF8 );
#endif
	
	if (es == false)
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szName);
		return false;
	}

	TiXmlElement* pRoot = tinyXml.FirstChildElement( "Building_Info" );
	if ( NULL == pRoot )
		return false;

	
	TiXmlElement *pBuild = pRoot->FirstChildElement("Building");

	CString strText;
	CEditListCtrl edtiList;
	for(vector<CSVInfo>::iterator it = m_vCSVInfo.begin(); it != m_vCSVInfo.end(); it ++)
	{
		bool bFind = false;
		while(pBuild)
		{
			strText = edtiList.GetXMLAttribute_EditList(pBuild, ("name"));

			if(strText == it->strFloorName)
			{
				strText = edtiList.GetXMLAttribute_EditList(pBuild, ("area_tol"));
				it->dArea_Tol = _tstof(strText);
				strText = edtiList.GetXMLAttribute_EditList(pBuild, ("area_top"));
				it->dArea_TolTop = _tstof(strText);
				strText = edtiList.GetXMLAttribute_EditList(pBuild, ("area_under"));
				it->dArea_TolUnder = _tstof(strText);
				strText = edtiList.GetXMLAttribute_EditList(pBuild, ("floorcount_top"));
				it->dFloorCount_Top = _ttoi(strText);
				strText = edtiList.GetXMLAttribute_EditList(pBuild, ("floorcount_under"));
				it->dFloorCount_Under = _ttoi(strText);
				bFind = true;
				break;
			}

			pBuild = pBuild->NextSiblingElement();
		}

		if(bFind == false)
		{
			it->dArea_Tol = INVALID_VALUE;
			it->dArea_TolTop = INVALID_VALUE;
			it->dArea_TolUnder = INVALID_VALUE;
			it->dFloorCount_Top = INVALID_VALUE;
			it->dFloorCount_Under = INVALID_VALUE;

			acutPrintf(_T("\n 要素表记录 %s 找不到分层图数据，没有统计建筑分层？"), it->strFloorName);
		}

		pBuild = pRoot->FirstChildElement("Building");

	}
	return true;

}


void vectorToStringArray(const vector<double> &vStr, CStringArray &strArr)
{
	strArr.RemoveAll();

// 	TCHAR sz[50] = {0};
// 	_stprintf(sz, _T("%d"), nPrec);
	CString strText, strText1;
// 	strText = _T("%.");
// 	strText += sz;
// 	strText += _T("lf");
	for(size_t t = 0; t < vStr.size(); t ++)
	{
		strText1.Format(_T("%lf"), vStr.at(t));
		strArr.Add(strText1);
	}
}


void vectorToStringArray(const vector<int> &vStr, CStringArray &strArr)
{
	strArr.RemoveAll();

	CString strText;
	for(size_t t = 0; t < vStr.size(); t ++)
	{
		strText.Format(_T("%d"), vStr.at(t));
		strArr.Add(strText);
	}
}

bool CExportSummaryTable::fillSummaryTable()
{
	m_excel.SetTemplete(m_strSavePath);
	if(m_excel.StartExcel() == false) return false;

#ifdef _DEBUG
	m_excel.Show();
#endif
	
	int nRow = 5, nCol0 = 1, nCol1 = 2, nCol2 = 3, nCol3 = 9, nCol4 = 10, nCol5 = 11, nCol6 = 12, nCol7 = 13;
	int nCol8 = 4, nCol9 = 5;

	m_excel.InsertRow(nRow, (int)m_vCSVInfo.size());

	int nCount = 1;
	for(vector<CSVInfo>::iterator it = m_vCSVInfo.begin(); it != m_vCSVInfo.end(); it ++)
	{
		CStringArray strArr;
		CString strText;

		TCHAR sz[500] = {0};
		_stprintf(sz, _T("%d"), nCount++);
		strArr.Add(sz);

		strArr.Add(it->strFloorName);

		bool bHaveIndoorHeight = true;
		if(it->vIndoorHeight.size() == 0) bHaveIndoorHeight = false;
		if(bHaveIndoorHeight)
		{
			_stprintf(sz, _T("%lf"), it->vIndoorHeight.at(0).dHeight);
			strArr.Add(sz);
		}
		else
			strArr.Add(_T("/"));

		if(it->dFloorCount_Top != INVALID_VALUE)
		{
			_stprintf(sz, _T("%d"), it->dFloorCount_Top);
			strArr.Add(sz);
		}
		else
			strArr.Add(_T("/"));

		if(it->dFloorCount_Under != INVALID_VALUE)
		{
			_stprintf(sz, _T("%d"), it->dFloorCount_Under);
			strArr.Add(sz);
		}
		else
			strArr.Add(_T("/"));

		strArr.Add(_T(""));
		strArr.Add(_T(""));

		if(it->vDaughterHeight.size() == 0)
		{
			strArr.Add(_T("/"));
		}
		else if(it->vDaughterHeight.size() == 1)
		{
			_stprintf(sz, _T("%.2lf"), it->vDaughterHeight.at(0).dHeight);
			strArr.Add(sz);
		}
		else
		{
			strText = _T("");
			for(size_t t = 0; t < it->vDaughterHeight.size(); t ++)
			{
				strText += it->vDaughterHeight.at(t).strName;
				strText += _T(": ");
				_stprintf(sz, _T("%.2lf"), it->vDaughterHeight.at(t).dHeight);
				strText += sz;
				strText += _T("\n");
			}
			strArr.Add(strText);
		}

		if(it->vMachineHeight.size() == 0)
		{
			strArr.Add(_T("/"));
		}
		else if(it->vMachineHeight.size() == 1)
		{
			_stprintf(sz, _T("%.2lf"), it->vMachineHeight.at(0).dHeight);
			strArr.Add(sz);
		}
		else
		{
			strText = _T("");
			for(size_t t = 0; t < it->vMachineHeight.size(); t ++)
			{
				strText += it->vMachineHeight.at(t).strName;
				strText += _T(": ");
				_stprintf(sz, _T("%.2lf"), it->vMachineHeight.at(t).dHeight);
				strText += sz;
				strText += _T("\n");
				
			}
			strArr.Add(strText);
		}

		if(it->vOtherHeight.size() == 0)
		{
			strArr.Add(_T("/"));
		}
		else if(it->vOtherHeight.size() == 1)
		{
			_stprintf(sz, _T("%.2lf"), it->vOtherHeight.at(0).dHeight);
			strArr.Add(sz);
		}
		else
		{
			strText = _T("");
			for(size_t t = 0; t < it->vOtherHeight.size(); t ++)
			{
				strText += it->vOtherHeight.at(t).strName;
				strText += _T(": ");
				_stprintf(sz, _T("%.2lf"), it->vOtherHeight.at(t).dHeight);
				strText += sz;
				strText += _T("\n");
				
			}
			strArr.Add(strText);
		}

		if(fabs(it->dArea_TolTop - INVALID_VALUE) > 0.00001)
		{
			_stprintf(sz, _T("%.5lf"), it->dArea_TolTop);
			strArr.Add(sz);
		}
		else
			strArr.Add(_T(" "));
		if(fabs(it->dArea_TolUnder - INVALID_VALUE) > 0.00001)
		{
			_stprintf(sz, _T("%.5lf"), it->dArea_TolUnder);
			strArr.Add(sz);
		}
		else
			strArr.Add(_T(" "));
		if(fabs(it->dArea_Tol - INVALID_VALUE) > 0.00001)
		{
			_stprintf(sz, _T("%.5lf"), it->dArea_Tol);
			strArr.Add(sz);
		}
		else
			strArr.Add(_T(" "));

		m_excel.AddStringArr(nRow, nCol0, strArr);

		nRow ++;
	}
	
	return true;
}


bool CExportSummaryTable::readSourceExcel()
{
	CTinyExcel excel;
	excel.SetTemplete(m_strSourceExcel);
	if(excel.StartExcel() == false) return false;

	int nRow = 3, nCol1 = 1, nColHeightAverage = 5, nCol3 = 7, nColNoteName = 8;
	long nRows = 0, nCols = 0;
	CString strText;
	excel.GetRowColCount(nRows, nCols);

	int nCount = 1;

	while(nRow <= nRows)
	{
		strText = excel.GetRangeContent(nRow, nCol1, excel.m_range);
		if(strText == _T("采用基准")) break;

		int nRowM = 0, nColM = 0;
		excel.GetMergedRowsAndCols(nRow, nCol1, nRowM, nColM);

		CSVInfo csv;
		csv.strFloorName = strText;
		for(int i = nRow; i < nRowM + nRow;)
		{
			ElevationInfo elev;
			strText = excel.GetRangeContent(i, nColNoteName, excel.m_range);

			elev.strName = strText;
			
			//提取规则：室内坪只提取高程那一列
			//楼高，机房高只提取楼高那一列，
			//其它则判断楼高那一列有没有数据，没有就不提取(因为即使是其它室内坪或层高也不会填到楼高那一列上)
			if(strText.Find(_T("一层室内坪")) >= 0)
			{
				elev.dHeight = _tstof(excel.GetRangeContent(i, nColHeightAverage, excel.m_range));
				csv.vIndoorHeight.push_back(elev);
			}
			else if(strText.Find(_T("楼高")) >= 0)
			{
				elev.dHeight = _tstof(excel.GetRangeContent(i, nCol3, excel.m_range));
				csv.vDaughterHeight.push_back(elev);
			}
			else if(strText.Find(_T("机房间高")) >= 0)
			{
				elev.dHeight = _tstof(excel.GetRangeContent(i, nCol3, excel.m_range));
				csv.vMachineHeight.push_back(elev);
			}
			else
			{
				strText = excel.GetRangeContent(i, nCol3, excel.m_range);
				TrimString_Space(strText);
				if(strText != _T(""))
				{
					elev.dHeight = _tstof(strText);
					csv.vOtherHeight.push_back(elev);
				}

			}

			int nRowM2 = 0, nColM2 = 0;
			excel.GetMergedRowsAndCols(i, nColNoteName, nRowM2, nColM2);
			i += nRowM2;
		}
		
		m_vCSVInfo.push_back(csv);

		nRow += nRowM;
	}

	excel.CloseWorkBook(excel.m_workbook);
	excel.Quit();

	return true;
}


bool CExportSummaryTable::exportSummaryTable()
{
	
	CFileDialog openFD(TRUE, NULL, NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("要素计算表 (*.xls;*.xlsx;)|*.xls;*.xlsx;||"));
	if(openFD.DoModal() != IDOK) return false;
	m_strSourceExcel = openFD.GetPathName();

	CFileDialog openFD2(TRUE, NULL, NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("建筑面积图 (*.dwg;*.dwg;)|*.dwg;*.dwg;||"));
	if(openFD2.DoModal() != IDOK) return false;
	m_strSourceDwg = openFD2.GetPathName();
	
	TCHAR szFilters[]=
		_T("xlsx (*.xlsx)|*.xlsx|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE,_T("xlsx"),_T("*.xlsx"),OFN_FILEMUSTEXIST
		|OFN_HIDEREADONLY,szFilters,NULL);
	if(fileDlg.DoModal() != IDOK) return false;
	m_strSavePath = fileDlg.GetPathName();

	TCHAR szTempPath[MAX_PATH];
	TCHAR szTempName[255]=_T("survey_result_table_template.xlsx");
	if (ads_findfile(szTempName,szTempPath) != RTNORM)
	{
		TCHAR szTemp[255]={0};
		_stprintf(szTemp,_T("\n 没有找到模板:%s"),szTempName);
		acutPrintf(szTemp);
		return false;
	}

	if(false == readSourceExcel()) return false;

	//readSourceXML();

	if(false == readSourceDwg()) 
	{
		ads_printf(_T("\n 读取dwg失败！ %s"), m_strSourceDwg);
		return false;
	}

	CopyFile(szTempPath,m_strSavePath,false);

	fillSummaryTable();

	//replaceLabels();

	replaceLabels2();

	m_excel.SaveWorkBook(m_excel.m_workbook);
	m_excel.CloseWorkBook(m_excel.m_workbook);
	m_excel.Quit();

	if( IDYES == AfxMessageBox(_T("生成完毕！是否打开？") , MB_YESNO) )
	{
		ShellExecute(adsw_hwndAcad,_T("open"), m_strSavePath , _T(""),NULL,SW_SHOWNORMAL);
	}

	return true;
}

bool	CExportSummaryTable::replaceLabels2()
{
	vector<long> vR, vC;
	for(size_t t = 0; t < m_vStrKey.size(); t ++)
	{
		TCHAR szText[200] = {0}, sz2[500] = {0};
		_stprintf(szText, _T("#%s#"), m_vStrKey.at(t));
		_tcscpy(sz2, m_vStrValue.at(t));
		m_excel.FindString(szText, vR, vC, sz2);
	}

	m_excel.ReplaceText_Common();

	return true;
}


bool	CExportSummaryTable::replaceLabels()
{

	CXRecordOperate xrecordO;
	CArray<ATTRFIELD,ATTRFIELD> *AttrArr = NULL;
	CString strText;
	vector<long> vR, vC;
	if(findAttriTable(_T("XMXX"), AttrArr))
	{
		for(int j = 0; j < AttrArr->GetSize(); j ++)
		{
			TCHAR szText[100] = {0};
			_stprintf(szText, _T("#%s#"), AttrArr->GetAt(j).name);
			xrecordO.ReadStringRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, strText);
			TCHAR szReplace[500] = {0};
			_tcscpy(szReplace, strText);
			m_excel.FindString(szText, vR, vC, szReplace);
		}
	}

	m_excel.ReplaceText_Common();



	return true;
}



CExportSummaryTable::CExportSummaryTable(void)
{
	m_nMergedRow = 0;
	m_nStartRow = 0;
	m_nWarning = 0;
}


CExportSummaryTable::~CExportSummaryTable(void)
{
}
