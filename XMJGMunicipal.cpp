#include "stdafx.h"

#ifndef XMJGMUNICIPAL_H
#include "XMJGMunicipal.h"
#endif /*XMJGMUNICIPAL_H*/

#include "XMJGTool.h"
#include "Global.h"
#include "cassGlobal.h"
#include "AttributeDef.h"

extern AttributeDef g_attribute;

XMJGMunicipal::XMJGMunicipal()
	: m_type(0)
{

}

XMJGMunicipal::XMJGMunicipal(int type)
	: m_type(type)
{

}

XMJGMunicipal::~XMJGMunicipal()
{
	XMJGAssist assist; string strValue;
	CString strPath = assist.getExportFilePath();
	if(m_type == 7)
		strPath += _T("\\CG\\��·�����ܽ�.docx");
	else
		strPath += _T("\\CG\\�������ձ�����.docx");
	splitBigString(strPath, strValue);
	bool bSuc = m_word.save(strValue.c_str());
	if(bSuc)
	{
		strPath.Format(_T("\n����ɹ�: %s"), strPath);
		acutPrintf(strPath);
	}
	else
	{
		strPath.Format(_T("\n ��·�������ձ��������ʧ��"));
		acutPrintf(strPath);
	}
}

bool XMJGMunicipal::initWord(const CString &templet)
{
	TCHAR docPath[MAX_PATH] = {0};
	if(RTNORM != ads_findfile(templet, docPath))
	{
		AfxMessageBox(_T("δ�ҵ���·�������ձ�����ģ��"));
		return false;
	}
	m_word.setEncoding(Encoding_GBK); char szPath[MAX_PATH] = {0};
	TcharToChar(docPath, szPath);
	return m_word.loadWord(szPath) == 1;
}

void XMJGMunicipal::replaceJBXX()
{
	IProjectMDB pdb; MStr msInfo = pdb.getJBXXTableInfo(); bool bSuc = false; string strValue; TCHAR szTime[255] = {0};
	splitBigString(msInfo[_T("�����")], strValue);		bSuc = m_word.replaceText("#AcceptCode#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("���赥λ")], strValue);	bSuc = m_word.replaceText("#BuildCompany#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("��Ŀ����")], strValue);	bSuc = m_word.replaceText("#ProjectName#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("��Ŀ�ص�")], strValue);	bSuc = m_word.replaceText("#ProjectAddress#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("������λ")], strValue);	bSuc = m_word.replaceText("#MeansureCompany#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("ʵʩ��λ")], strValue);	bSuc = m_word.replaceText("#ImpletedCompany#", strValue.c_str()); strValue.clear();
	stringsOpe::GetToday(szTime); splitBigString(szTime, strValue); bSuc = m_word.replaceText("#Date#", strValue.c_str()); strValue.clear();
	msInfo = pdb.getJSSMTableInfo();
	splitBigString(msInfo[_T("���ɹ����ݵ��������")], strValue); bSuc = m_word.replaceText("#LicenseDoc#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("�����������")], strValue); bSuc = m_word.replaceText("#YYZLQK#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("ͼ���������")], strValue); bSuc = m_word.replaceText("#BasePoint#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("ϸ�������������")], strValue); bSuc = m_word.replaceText("#DetailConvery#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("��滮��ɲ�һ�����")], strValue); bSuc = m_word.replaceText("#YGHXKBYZQK#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("������Ҫ˵�������")], strValue); bSuc = m_word.replaceText("#QTXYSMDQK#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("���Ų���")], strValue); bSuc = m_word.replaceText("#TQCL#", strValue.c_str()); strValue.clear();
	splitBigString(msInfo[_T("���¹��ߵ�̽��")], strValue); bSuc = m_word.replaceText("#DXGXDTC#", strValue.c_str()); strValue.clear();
}

void XMJGMunicipal::exportGHHSJBQK()
{
	IProjectMDB pdb; WordTable table;
	VStr names = pdb.getAllRoadName(); int oldRow = 2;
	if(names.size() == 0)
	{
		acutPrintf(_T("\n�������õ�·�滮ʵʩ�������"));
		return;
	}
	int ret = m_word.findTable(1, 1, "#GHSSJBQK#", table);
	if(ret == -1)
	{
		acutPrintf(_T("\n�޷��ҵ��滮ʵʩ���������"));
		return;
	}
	table.replaceText(1, 1, "#GHSSJBQK#", "");
	char szInfo[255] = {0}; int row = 2;
	int count = names.size();
	table.copyRow(row, 0, count * 3 - 1);
	for(int idx = 0; idx < (int)names.size(); ++idx)
	{
		int startRow = row; int oldRow = row;
		CString name = names[idx]; GHSSJBQKInfo info;
		pdb.getRoadInfo(name, info); CString scRoad, sjRoad;
		int num = info.roadWidth.m_scRoadWidth.size();
		for(int indx = 0; indx < num; ++indx)
		{
			scRoad += info.roadWidth.m_scRoadWidth[indx] + _T("\n");
			sjRoad += info.roadWidth.m_sjRoadWidth[indx] + _T("\n");
		}
		table.setCellText(row + idx + 0, 2, "��·���(m)");
		TcharToChar(sjRoad, szInfo);
		table.setCellText(row + idx + 0, 3, szInfo);
		TcharToChar(scRoad, szInfo);
		table.setCellText(row + idx + 0, 4, szInfo);
		scRoad = sjRoad = _T("");

		num = info.roadHeight.m_scRoadHeight.size();
		for(int indx = 0; indx < num; ++indx)
		{
			scRoad += info.roadHeight.m_scRoadHeight[indx] + _T("\n");
			sjRoad += info.roadHeight.m_sjRoadHeight[indx] + _T("\n");
		}
		table.setCellText(row + idx + 1, 2, "��·����(m)");
		TcharToChar(sjRoad, szInfo);
		table.setCellText(row + idx + 1, 3, szInfo);
		TcharToChar(scRoad, szInfo);
		table.setCellText(row + idx + 1, 4, szInfo);

		scRoad = sjRoad = _T("");
		num = info.roadLength.m_scRoadLength.size();
		for(int indx = 0; indx < num; ++indx)
		{
			scRoad += info.roadLength.m_scRoadLength[indx] + _T("\n");
			sjRoad += info.roadLength.m_sjRoadLength[indx] + _T("\n");
		}
		table.setCellText(row + idx + 2, 2, "��·����(m)");
		TcharToChar(sjRoad, szInfo);
		table.setCellText(row + idx + 2, 3, szInfo);
		TcharToChar(scRoad, szInfo);
		table.setCellText(row + idx + 2, 4, szInfo);

		table.merge(startRow, 1, startRow + 2, 1);
		TcharToChar(name, szInfo); row += 3;
		table.setCellText(startRow, 1, szInfo);
	}
}

void XMJGMunicipal::replaceWordField()
{
	IProjectMDB pdb;
	VMStr records = pdb.getPictureTable();
	int len = (int)records.size();
	bool byzsyt = false, yghtbyz = false;
	SmartTable::WordTable table; char info[255] = {0};
	int tableindex = m_word.findTable(1, 1, "#Format1#", table);
	int index = m_word.setPropertyTable(tableindex);
	for(int idx = len; 0 < idx; --idx)
	{
		MStr record = records.at(idx - 1);
		CString path(record[_T("·��")]);
		CString local(record[_T("λ��")]);
		CString descr(record[_T("����")]);
		if(local.CompareNoCase(_T("#BYZSYTPicture1#")) == 0 && !byzsyt)
			byzsyt = true;
		else if(local.CompareNoCase(_T("#YGHTBYZPicture1#")) == 0 && !yghtbyz)
			yghtbyz = true;
		TcharToChar(local, info);
		m_word.setCurrentInsert(info);
		TcharToChar(path, info);
		int width = 0, height = 0;
		getImagePngSize(info, width, height);
		if(width < 3000 && height < 3000)
			m_word.appendImage(info, 14.0, 10.0, SmartTable::Alignment_MC);
		else if(width < 4000 && height < 4000)
			m_word.appendImage(info, 14.00, 20.50, SmartTable::Alignment_MC);
		else
			m_word.appendImage(info, 34.00, 19.00, SmartTable::Alignment_MC);
		TcharToChar(descr, info);
		m_word.appendParagraph(info, 0);
	}
	IDataBaseOper oper; MStr nameproperty;
	oper.readCommonTable(_T("ͼƬ�����ʽ"), nameproperty);
	MSIter it = nameproperty.begin();
	//if(!byzsyt) m_word.replaceText("#BYZSYTPicture1#", "��");
	//if(!yghtbyz) m_word.replaceText("#BYZSYTPicture1#", "��");
	for(; it != nameproperty.end(); ++it)
	{
		TcharToChar(it->first, info);
		m_word.replaceText(info, "");
	}
	m_word.removeTable(tableindex);
	m_word.removeBlankPage();
}

void XMJGMunicipal::replaceGHSSJBQKPipe()
{
	WordTable table; CString info; int row = 2;
	int ret = m_word.findTable(1, 1, "#GHSSJBQK#", table);
	table.replaceText(1, 1, "#GHSSJBQK#", "");
	char szInfo[255] = {0}; int oldRow = 2;
	IProjectMDB pdb; VMStr records = pdb.getPipeTable();
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		CString name = record[_T("����")]; GHSSJBQKPipe pipe;
		pdb.getRoadInfo(name, pipe); int count = pipe.m_scPipeLine.size();
		if(count == 0) continue;
		table.copyRow(row, 0, count);
		for(int rowid = 0; rowid < (int)pipe.m_scPipeLine.size(); ++rowid)
		{
			info = pipe.m_sjPipeLine[rowid];
			TcharToChar(info, szInfo);
			table.setCellText(row, 3, szInfo);
			info = pipe.m_scPipeLine[rowid];
			TcharToChar(info, szInfo);
			table.setCellText(row++, 4, szInfo);
		}
		table.merge(oldRow, 2, row - 1, 2);
		TcharToChar(name, szInfo);
		table.setCellText(oldRow, 2, szInfo);
		oldRow = row;
	}
	table.merge(2, 1, row, 1);
	table.setCellText(2, 1, "����\n����\n����");
	table.removeRow(row);
}

void XMJGMunicipal::exportRoadJSZJ()
{
	XMJGAssist assist; MStr msInfo;
	CString strXMMC; IProjectMDB pdb;
	TCHAR szPath[255] = {0};
	bool bSuc = false;
	char cPath[255] = {0}, cXMMC[255] = {0}, cOutPutPath[255] = {0};
	ads_name ssName; long nLen = 0;
	AcDbObjectIdArray ids;
	msInfo = pdb.getProjectInfo();
	TcharToChar(strXMMC, cXMMC);
	bSuc = m_word.replaceText("#XMMC#", cXMMC);
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	struct resbuf *rc = acutBuildList(RTDXF0, _T("*POLYLINE"), 8, _T("SCDLX"), 0);
	acedSSGet(_T("X"), NULL, NULL, rc, ssName); ads_relrb(rc);
	acedSSLength(ssName, &nLen);
	SSToIdArr(ssName, ids, _T("$180302$"));
	PlaneComparisonTable(ids);//��·����ƽ����ձ�
	ZDMComparisonTable(ids);//��·�����ݶ϶��ձ�
	HDMComparisonTable(ids);//��·������϶��ձ�
	JSZJTable(ids);//��·���������ܽ�С��
}

void XMJGMunicipal::exportRoadGHHSSQB()
{
	// 	AcDbObjectIdArray ids;
	// 	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	// 	struct resbuf *rc = acutBuildList(RTDXF0, _T("*POLYLINE"), 8, _T("SCDLX"), 0);
	// 	acedSSGet(_T("X"), NULL, NULL, rc, ssName); ads_relrb(rc);
	// 	TCHAR info[20] = {0}; char value[40] = {0};
	// 	ReadXdata(aname(roadId), _T("�������X"), 0, info);
	// 	TcharToChar(info, value);  word.replaceText("#STARTPOINTX#", value);
	// 	ReadXdata(aname(roadId), _T("�������Y"), 0, info);
	// 	TcharToChar(info, value);  word.replaceText("#STARTPOINTY#", value);
	// 	ReadXdata(aname(roadId), _T("�յ�����X"), 0, info);
	// 	TcharToChar(info, value);  word.replaceText("#ENDPOINTX#", value);
	// 	ReadXdata(aname(roadId), _T("�յ�����Y"), 0, info);
	// 	TcharToChar(info, value); word.replaceText("#ENDPOINTY#", value);
}

void XMJGMunicipal::exportPipeLineTable()
{
	replaceJBXX(); replaceGHSSJBQKPipe();
	WordTable cgb, dzb; TCHAR info[255] = {0}; IDataBaseOper oper;
	if(-1 == m_word.findTable(1, 1, "#GXLX#���߳ɹ���", cgb)) return;
	if(-1 == m_word.findTable(1, 1, "#GXLX# �� �� �� �� �� ʵ �� �� �� �� �� ��", dzb)) return;
	SelectFilter sf(RTDXF0, _T("*POLYLINE")); AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T("X"))) return; MSIDS msid;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(false == ReadXdata(aname(id), _T("GXLX"), 0, info)) continue;
		MStr filter; filter[_T("�ؼ���")] = info;
		CString value = oper.readPipeLineTable(_T("����ֵ"), filter);
		msid[value].append(id);
	}
	m_word.setCurrentInsert("#DLGXJGCGDZJJSZJ#"); int chapter = 1;
	for(MSIDS::iterator it = msid.begin(); it != msid.end(); ++it)
	{
		exportPipeLineZPT(it->first, chapter++);
		exportPipeLineCGB(cgb, it->first, it->second, chapter++);
		exportPipeLineDZB(dzb, it->first, it->second, chapter++);
		exportPipeLineDZT(it->first, chapter++);
	}
}

void XMJGMunicipal::exportRoadPipeReport()
{
	exportRoadJGYSBGS();
	exportPipeLineTable();
}

void XMJGMunicipal::exportRoadPipeBridgeReport()
{
	exportRoadJGYSBGS();
	exportPipeLineTable();
	exporBridgeJGYSBGS();
}

void XMJGMunicipal::save()
{
	XMJGAssist assist; string strValue;
	CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\�������ձ�����.docx");
	splitBigString(strPath, strValue);
	bool bSuc = m_word.save(strValue.c_str());
	if(bSuc)
	{
		strPath.Format(_T("\n����ɹ�: %s"), strPath);
		acutPrintf(strPath);
	}
	else
	{
		strPath.Format(_T("\n ��·�������ձ��������ʧ��"));
		acutPrintf(strPath);
	}
}

void XMJGMunicipal::JSZJTable(const AcDbObjectIdArray &ids)
{
	bool bSuc = false;
	WordTable table1;
	int nFind = m_word.findTable(2, 2, "#START5#", table1);
	for(int i = 0; i < ids.length(); i++)
	{
		WordTable table2;
		AcDbObjectId idTemp = ids.at(i);
		TCHAR szStdm[255] = {0};
		GetStdm(aname(idTemp), szStdm);
		if(_tcscmp(szStdm, _T("180302"))) continue;
		if(ids.length() - i > 1)
		{
			bSuc = m_word.setCurrentInsert(table1);
			bSuc = m_word.appendParagraph("");
			bSuc = m_word.setCurrentInsert(table1, 1);
			m_word.copyTable(table1, table2);
		}
		TCHAR szDLMC[255] = {0};
		char cTemp[255] = {0};
		ReadXdata(aname(idTemp), _T("��·����������"), STRING_TYPE, szDLMC);
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(2, 1, cTemp);
		m_word.replaceText("#START5#", "");

		TCHAR info[20] = {0}; char value[40] = {0};
		ReadXdata(aname(idTemp), _T("��·���������"), 0, info);
		if(_tcslen(info) == 0) _stprintf(info, _T("%s"), _T(",,"));
		CStringArray scoor; AnalyzeData(scoor, info, _T(","));
		TcharToChar(scoor.GetAt(0), value);  m_word.replaceText("#STARTPOINTX#", value);
		TcharToChar(scoor.GetAt(1), value);  m_word.replaceText("#STARTPOINTY#", value);
		ReadXdata(aname(idTemp), _T("��·�������յ�"), 0, info);
		if(_tcslen(info) == 0) _stprintf(info, _T("%s"), _T(",,"));
		CStringArray ecoor; AnalyzeData(ecoor, info, _T(","));
		TcharToChar(ecoor.GetAt(0), value);  m_word.replaceText("#ENDPOINTX#", value);
		TcharToChar(ecoor.GetAt(1), value);  m_word.replaceText("#ENDPOINTY#", value);
		table1 = table2;
	}
}

void getPercent(vector<int> &nInput, vector<double> &dOutput)
{
	if(nInput.size() == 0)
		return;

	int nSum = 0;
	for(int i = 0; i < (int)nInput.size(); ++i)
		nSum += nInput.at(i);

	vector<double> dTemps;
	double dTemp = 0.0;
	for(int i = 0; i < (int)nInput.size(); ++i)
	{
		dTemp = (double)nInput.at(i) / nSum;
		char buf[10];
		sprintf(buf, "%.4lf", dTemp);
		dTemp = atof(buf);
		dTemps.push_back(dTemp);
	}

	for(int i = 0; i < (int)dTemps.size(); ++i)
		dTemp += dTemps.at(i);

	if(dTemp == 1.00)
	{
		dOutput = dTemps;
		return;
	}

	double dValue = 1.00 - dTemp;
	for(int i = 0; i < (int)dTemps.size(); ++i)
	{
		dTemp = dTemps.at(i) + dValue*dTemps.at(i);
		char buf[10];
		sprintf(buf, "%.4lf", dTemp);
		dTemp = atof(buf);
		dOutput.push_back(dTemp);
	}
}

bool XMJGMunicipal::ReplacePic(VMStr &records, CString &strPic, int index)
{
	char cValue[255] = {0}; bool bSuc = false;
	for(int i = 0; i < (int)records.size(); ++i)
	{
		MStr record = records.at(i);
		CString strTemp(record[_T("λ��")]);
		if(_tcscmp(strTemp, strPic)) continue;
		CString path(record[_T("·��")]);
		CString local(record[_T("λ��")]);
		CString descr(record[_T("����")]);
		TcharToChar(local, cValue);
		bSuc = m_word.setCurrentInsert(cValue, 0, true);
		int width = 0, height = 0;
		TcharToChar(path, cValue);
		bSuc = getImagePngSize(cValue, width, height);
		if(width < 3000 && height < 3000)
			bSuc = m_word.appendImage(cValue, 13.0, 10.0, SmartTable::Alignment_MC);
		else if(width < 4000 && height < 4000)
			bSuc = m_word.appendImage(cValue, 16.18, 22.51, SmartTable::Alignment_MC);
		else
			bSuc = m_word.appendImage(cValue, 34.94, 20.0, SmartTable::Alignment_MC);
		TcharToChar(descr, cValue);
		m_word.appendParagraph(cValue, 0);
	}
	return bSuc;
}

void XMJGMunicipal::ZDMComparisonTable(const AcDbObjectIdArray &ids)
{
	bool bSuc = false;
	WordTable table1, table2;
	int nFind = m_word.findTable(3, 2, "#START2#", table1);
	nFind = m_word.findTable(2, 3, "#START3#", table2);
	TCHAR szStdm[255] = {0};
	AcDbObjectIdArray idgets;
	AcGePoint2dArray Nodes;

	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	for(int i = 0; i < ids.length(); i++)
	{
		WordTable table3, table4;
		if(ids.length() - i > 1)
		{
			bSuc = m_word.setCurrentInsert(table1);
			bSuc = m_word.appendParagraph("");
			bSuc = m_word.setCurrentInsert(table1, 1);
			m_word.copyTable(table1, table3);

			bSuc = m_word.setCurrentInsert(table2);
			bSuc = m_word.appendParagraph("");
			bSuc = m_word.setCurrentInsert(table2, 1);
			m_word.copyTable(table2, table4);
		}

		TCHAR szDLMC[255] = {0};
		char cTemp[255] = {0};
		AcDbObjectId idTemp = ids.at(i);
		ReadXdata(aname(idTemp), _T("��·����������"), STRING_TYPE, szDLMC);
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(3, 1, cTemp);
		table2.setCellText(2, 1, cTemp);

		int n5 = 0, n10 = 0, n15 = 0, n20 = 0, n20P = 0;
		char cValue[255] = {0};
		int nCout = GetXDataCount(aname(idTemp), _T("LCZGC"));
		for(int j = 0; j < nCout; ++j)
		{
			TCHAR szLCZInfo[255] = {0};
			READMulXData(idTemp, _T("LCZGC"), szLCZInfo, STRING_TYPE, j);
			if(!_tcscmp(szLCZInfo, _T("")))
				_tcscpy(szLCZInfo, _T("____"));
			vector<CString> vstr1;
			CString strDLQD(szLCZInfo);
			AnalyzeData(vstr1, strDLQD, _T("_"));
			if(nCout - j > 1)
			{
				bSuc = table1.copyRow(3, 3 + j);
			}

			TcharToChar(vstr1.at(0), cValue);
			table1.setCellText(3 + j, 2, cValue);
			TcharToChar(vstr1.at(2), cValue);
			table1.setCellText(3 + j, 3, cValue);
			TcharToChar(vstr1.at(3), cValue);
			table1.setCellText(3 + j, 4, cValue);
			TcharToChar(vstr1.at(4), cValue);
			table1.setCellText(3 + j, 5, cValue);
			double dValue = _wtof(vstr1.at(4));
			dValue = fabs(dValue);
			if(dValue <= 5.0)
				n5++;
			else if(dValue > 5 && dValue <= 10.0)
				n10++;
			else if(dValue > 10 && dValue <= 15.0)
				n15++;
			else if(dValue > 15 && dValue <= 20.0)
				n20++;
			else
				n20P++;
		}
		table1.merge(3, 1, 2 + nCout, 1);
		int nSum = n5 + n10 + n15 + n20 + n20P;
		vector<double> dOutput;
		vector<int> nInput;
		nInput.push_back(n5); nInput.push_back(n10); nInput.push_back(n15); nInput.push_back(n20); nInput.push_back(n20P);
		getPercent(nInput, dOutput);
		if(dOutput.size() == 0)
		{
			for(int z = 0; z < 5; ++z)
				dOutput.push_back(0.0);
		}

		_itoa(n5, cValue, 10);
		table2.setCellText(2, 3, cValue);
		sprintf(cValue, "%.2f%", dOutput.at(0) * 100);
		table2.setCellText(2, 4, cValue);

		_itoa(n10, cValue, 10);
		table2.setCellText(3, 3, cValue);
		sprintf(cValue, "%.2f%", dOutput.at(1) * 100);
		table2.setCellText(3, 4, cValue);

		_itoa(n15, cValue, 10);
		table2.setCellText(4, 3, cValue);
		sprintf(cValue, "%.2f%", dOutput.at(2) * 100);
		table2.setCellText(4, 4, cValue);

		_itoa(n20, cValue, 10);
		table2.setCellText(5, 3, cValue);
		sprintf(cValue, "%.2f%", dOutput.at(3) * 100);
		table2.setCellText(5, 4, cValue);

		_itoa(n20P, cValue, 10);
		table2.setCellText(6, 3, cValue);
		sprintf(cValue, "%.2f%", dOutput.at(4) * 100);
		table2.setCellText(6, 4, cValue);

		table1 = table3;
		table2 = table4;
	}
}

void XMJGMunicipal::HDMComparisonTable(const AcDbObjectIdArray &ids)
{
	bool bSuc = false;
	WordTable table1;
	int nFind = m_word.findTable(2, 2, "#START4#", table1);
	TCHAR stdm[255] = {0}; int count = -1;
	for(int i = 0; i < ids.length(); i++)
	{
		AcDbObjectId idTemp = ids.at(i);
		GetStdm(aname(idTemp), stdm);
		if(_tcscmp(stdm, _T("180302")) != 0) continue;
		WordTable table2; count++;
		if(ids.length() - i > 1)
		{
			bSuc = m_word.setCurrentInsert(table1);
			bSuc = m_word.appendParagraph("");
			bSuc = m_word.setCurrentInsert(table1, 1);
			m_word.copyTable(table1, table2);
		}

		TCHAR szDLMC[255] = {0};
		char cTemp[255] = {0};
		ReadXdata(aname(idTemp), _T("��·����������"), STRING_TYPE, szDLMC);
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(2, 1, cTemp);
		m_word.replaceText("#START4#", "");

		double dTemp = vSCLength.at(count);
		sprintf(cTemp, "%lf", dTemp);
		table1.setCellText(2, 3, cTemp);

		dTemp = vSJLength.at(count);
		sprintf(cTemp, "%lf", dTemp);
		table1.setCellText(2, 4, cTemp);

		dTemp = vDLength.at(count);
		sprintf(cTemp, "%lf", dTemp);
		table1.setCellText(2, 5, cTemp);

		table1 = table2;
	}
}

void XMJGMunicipal::PlaneComparisonTable(const AcDbObjectIdArray &ids)
{
	bool bSuc = false;
	WordTable table1, table3;
	if(-1 == m_word.findTable(5, 2, "#START1#", table1)) return;
	for(int i = 0; i < ids.length(); i++)
	{
		WordTable table2;
		if(ids.length() - i > 1)
		{
			bSuc = m_word.setCurrentInsert(table1);
			m_word.copyTable(table1, table2);
		}

		TCHAR szStdm[50] = {0}, szDLMC[255] = {0}, szDLQD[255] = {0}, szDLZD[255] = {0},
			szSJS[255] = {0}, szSJE[255] = {0};
		char cTemp[255] = {0};
		AcDbObjectId idTemp = ids.at(i);
		GetStdm(aname(idTemp), szStdm);
		if(_tcscmp(szStdm, _T("180302"))) continue;
		ReadXdata(aname(idTemp), _T("��·����������"), STRING_TYPE, szDLMC);
		ReadXdata(aname(idTemp), _T("��·���������"), STRING_TYPE, szDLQD);
		ReadXdata(aname(idTemp), _T("��·�������յ�"), STRING_TYPE, szDLZD);
		int nCout = GetXDataCount(aname(idTemp), _T("LCZGC"));
		READMulXData(idTemp, _T("LCZGC"), szSJS, STRING_TYPE, 0);
		READMulXData(idTemp, _T("LCZGC"), szSJE, STRING_TYPE, nCout - 1);

		//��ʽ���
		if(!_tcscmp(szDLQD, _T("")))
			_tcscpy(szDLQD, _T(",,"));
		if(!_tcscmp(szDLZD, _T("")))
			_tcscpy(szDLZD, _T(",,"));
		if(!_tcscmp(szSJS, _T("")))
			_tcscpy(szSJS, _T("____"));
		if(!_tcscmp(szSJE, _T("")))
			_tcscpy(szSJE, _T("____"));

		vector<CString> vstr1, vstr2, vstr3, vstr4, vstr5, vstr6;
		CString strDLQD(szDLQD), strDLZD(szDLZD), strSJQD(szSJS), strSJZD(szSJE);
		AnalyzeData(vstr1, strDLQD, _T(","));
		AnalyzeData(vstr2, strDLZD, _T(","));
		AnalyzeData(vstr3, strSJQD, _T("_"));
		AnalyzeData(vstr5, vstr3.at(1), _T(","));//��ȡ����������
		AnalyzeData(vstr4, strSJZD, _T("_"));
		AnalyzeData(vstr6, vstr4.at(1), _T(","));//��ȡ����յ�����
		if(vstr5.size() == 0)
		{
			vstr5.push_back(_T("")); vstr5.push_back(_T(""));
		}
		if(vstr6.size() == 0)
		{
			vstr6.push_back(_T("")); vstr6.push_back(_T(""));
		}
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(1, 1, cTemp);
		TcharToChar(vstr1.at(0), cTemp);
		table1.setCellText(5, 2, cTemp);
		TcharToChar(vstr5.at(0), cTemp);//��ȡ������X����
		table1.setCellText(5, 3, cTemp);
		TcharToChar(vstr1.at(1), cTemp);
		table1.setCellText(6, 2, cTemp);
		TcharToChar(vstr5.at(1), cTemp);//��ȡ������Y����
		table1.setCellText(6, 3, cTemp);
		TcharToChar(vstr1.at(2), cTemp);
		table1.setCellText(7, 2, cTemp);
		TcharToChar(vstr3.at(2), cTemp);
		table1.setCellText(7, 3, cTemp);//��ȡ������߳�
		TcharToChar(vstr2.at(0), cTemp);
		table1.setCellText(8, 2, cTemp);
		TcharToChar(vstr6.at(0), cTemp);//��ȡ����յ�X����
		table1.setCellText(8, 3, cTemp);
		TcharToChar(vstr2.at(1), cTemp);
		table1.setCellText(9, 2, cTemp);
		TcharToChar(vstr6.at(1), cTemp);//��ȡ����յ�Y����
		table1.setCellText(9, 3, cTemp);
		TcharToChar(vstr2.at(2), cTemp);
		table1.setCellText(10, 2, cTemp);
		TcharToChar(vstr4.at(2), cTemp);//��ȡ����յ�߳�
		table1.setCellText(10, 3, cTemp);
		double dSCRoadLen = Dist2Points(vstr1, vstr2);
		vSCLength.push_back(dSCRoadLen);
		sprintf(cTemp, "%f", dSCRoadLen);
		table1.setCellText(11, 2, cTemp);
		double dSJRoadLen = Dist2Points(vstr5.at(0), vstr5.at(1), vstr3.at(2), vstr6.at(0), vstr6.at(1), vstr4.at(2));
		vSJLength.push_back(dSJRoadLen);
		sprintf(cTemp, "%f", dSJRoadLen);
		double dVlength = dSCRoadLen - dSJRoadLen;
		vDLength.push_back(dVlength);
		table1.setCellText(11, 3, cTemp);
		table1 = table2;
	}
}

void XMJGMunicipal::exportPipeLineZPT(const CString &type, int chapter)
{
	CString title; title.Format(_T("7.%d��%s������ƽͼ"), chapter, type);
	addTitle(title, _T("���߱���"), 0);
}

void XMJGMunicipal::exportPipeLineDZT(const CString &type, int chapter)
{
	CString title; title.Format(_T("7.%d��%s���߶���ͼ"), chapter, type);
	addTitle(title, _T("���߱���"), 0);
}

void XMJGMunicipal::addTitle(const CString &title, const CString &context, int row)
{
	char info[255] = {0}; TcharToChar(context, info);
	WordTable tt; int cell = m_word.findTable(row, 0, info, tt);
	int pro = m_word.setPropertyTable(cell);
	TcharToChar(title, info); bool flag = m_word.appendParagraph(info, pro);
}

void XMJGMunicipal::fillToFormTableDZB(WordTable &table, const MStr &minfo, int row)
{
	MStr infos = static_cast<MStr>(minfo);
	table.copyRow(row, 0, 3); char info[255] = {0}, dh[255] = {0}, sjdh[255] = {0};
	table.merge(row, 1, row + 2, 1);
	table.merge(row, 2, row + 2, 2);
	table.merge(row, 3, row + 2, 3);
	table.setCellText(row + 0, 4, "���");
	table.setCellText(row + 1, 4, "ʵ��");
	table.setCellText(row + 2, 4, "���");
	TcharToChar(infos[_T("���")], dh);
	TcharToChar(infos[_T("��Ƶ��")], sjdh);
	if(strlen(sjdh) != 0 || strcmp(sjdh, dh) != 0)
		sprintf(info, "%s\n%s", sjdh, dh);
	else
		sprintf(info, "%s", dh);
	table.setCellText(row, 1, info);
	TcharToChar(infos[_T("������")], info); table.setCellText(row, 2, info);
	TcharToChar(infos[_T("������")], info); table.setCellText(row, 3, info);
	TcharToChar(infos[_T("���X����")], info); table.setCellText(row, 5, info);
	TcharToChar(infos[_T("X����")], info); table.setCellText(row + 1, 5, info);
	double xval = _tstof(infos[_T("X����")]) - _tstof(infos[_T("���X����")]);
	sprintf(info, "%.3lf", xval); table.setCellText(row + 2, 5, info);
	TcharToChar(infos[_T("���Y����")], info); table.setCellText(row, 6, info);
	TcharToChar(infos[_T("Y����")], info); table.setCellText(row + 1, 6, info);
	double yval = _tstof(infos[_T("Y����")]) - _tstof(infos[_T("���Y����")]);
	sprintf(info, "%.3lf", yval); table.setCellText(row + 2, 6, info);
	double val = sqrt(pow(xval, 2) + pow(yval, 2));
	sprintf(info, "%.3lf", val); table.setCellText(row + 2, 7, info);
	TcharToChar(infos[_T("��Ƶ���߳�")], info); table.setCellText(row, 8, info);
	TcharToChar(infos[_T("����߳�")], info); table.setCellText(row + 1, 8, info);
	val = _tstof(infos[_T("����߳�")]) - _tstof(infos[_T("��Ƶ���߳�")]);
	sprintf(info, "%.3lf", val); table.setCellText(row + 2, 8, info);
	TcharToChar(infos[_T("��ƾ���")], info); table.setCellText(row, 9, info);
	val = _tstof(infos[_T("����߳�")]) - _tstof(infos[_T("����")]);
	sprintf(info, "%.3lf", val); table.setCellText(row + 1, 9, info);
	val = _tstof(infos[_T("��Ƶ���߳�")]) - _tstof(infos[_T("��ƾ���")]);
	sprintf(info, "%.3lf", val); table.setCellText(row + 2, 9, info);
	TcharToChar(infos[_T("��ƹܾ�")], info); table.setCellText(row, 10, info);
	TcharToChar(infos[_T("�ܾ�")], info); table.setCellText(row + 1, 10, info);
	TcharToChar(infos[_T("��Ʋ���")], info); table.setCellText(row, 11, info);
	TcharToChar(infos[_T("����")], info); table.setCellText(row + 1, 11, info);
	TcharToChar(infos[_T("������跽ʽ")], info); table.setCellText(row, 12, info);
	TcharToChar(infos[_T("���跽ʽ")], info); table.setCellText(row + 1, 12, info);
}

double XMJGMunicipal::Dist2Points(const vector<CString> &pt1, const vector<CString> &pt2)
{
	AcGePoint3d ptS, ptE;
	ptS[X] = _wtof(pt1.at(0));
	ptS[Y] = _wtof(pt1.at(1));
	ptS[Z] = _wtof(pt1.at(2));
	ptE[X] = _wtof(pt2.at(0));
	ptE[Y] = _wtof(pt2.at(1));
	ptE[Z] = _wtof(pt2.at(2));
	double dd = distOF3d(ptS, ptE);
	return dd;
}

MSStr XMJGMunicipal::readPipeLineProperty(const AcDbObjectIdArray &ids, const ATTRFIELD_ARRAY &table)
{
	TCHAR value[255] = {0}; int row = 8; MSStr pipls;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		int count = GetXDataCount(aname(id), _T("DH"));
		for(int idx = 0; idx < count; ++idx)
		{
			MStr pipl;  CString dh;
			for(int tt = 0; tt < (int)table.size(); ++tt)
			{
				READMulXData(id, table[tt].name, value, table[tt].type, idx);
				if(table[tt].type == 3)
				{
					double d = _tstof(value);
					_stprintf(value, _T("%.3lf"), d);
				}
				if(_tcscmp(table[tt].name, _T("DH")) == 0) dh.Format(_T("%s"), value);
				pipl[table[tt].text] = CString(value);
			}
			pipls[dh] = pipl;
		}
	}
	return pipls;
}

void XMJGMunicipal::fillToFormTableCGB(WordTable &table, const MStr &minfo, int row, const ATTRFIELD_ARRAY &atable, CString ljd, int loc)
{
	table.copyRow(row); char info[255] = {0};
	MStr infos = static_cast<MStr>(minfo);
	for(int idx = 1; idx < (int)atable.size(); ++idx)
	{
		ATTRFIELD_DG dg = atable[idx];
		if(_tcscmp(dg.text, _T("���ӵ��")) == 0 && ljd.IsEmpty() == false)
			TcharToChar(ljd, info);
		else
			TcharToChar(infos[dg.text], info);
		table.setCellText(row, idx, info);
	}
}

void XMJGMunicipal::exportPipeLineCGB(WordTable &cgb, const CString &type, const AcDbObjectIdArray &ids, int chapter)
{
	CString title; title.Format(_T("7.%d��%s���߳ɹ���"), chapter, type);
	addTitle(title, _T("���߱���"), 0); ATTRFIELD_ARRAY atable;
	WordTable table; m_word.copyTable(cgb, table);
	g_attribute.get_gistable_columns(_T("PIPL"), atable);
	MSStr plp = readPipeLineProperty(ids, atable);
	char info[255] = {0}; TcharToChar(type, info);
	table.replaceText(1, 1, "#GXLX#", info);
	table.replaceText(4, 4, "#GXLX#", info);
	int row = 8;
	for(MSSIter it = plp.begin(); it != plp.end(); ++it)
	{
		MStr vals = it->second;
		fillToFormTableCGB(table, vals, row++, atable);
		CString ljd = vals[_T("���ӵ��")];
		if(ljd.IsEmpty()) continue;
		fillToFormTableCGB(table, plp[ljd], row++, atable, vals[_T("���")]);
	}
	table.removeRow(row);
}

void XMJGMunicipal::exportPipeLineDZB(WordTable &dzb, const CString &type, const AcDbObjectIdArray &ids, int chapter)
{
	CString title; title.Format(_T("7.%d��%s���߶��ձ�"), chapter, type);
	addTitle(title, _T("���߱���"), 0);	ATTRFIELD_ARRAY atable;
	WordTable table; m_word.copyTable(dzb, table);
	g_attribute.get_gistable_columns(_T("PIPP"), atable);
	MSStr plp = readPipeLineProperty(ids, atable);
	char info[255] = {0}; TcharToChar(type, info);
	table.replaceText(1, 1, "#GXLX#", info);
	table.replaceText(4, 4, "#GXLX#", info);
	int row = 8;
	for(MSSIter it = plp.begin(); it != plp.end(); ++it, row += 3)
	{
		MStr vals = it->second;
		fillToFormTableDZB(table, vals, row);
	}
	table.removeRow(row);
}

double XMJGMunicipal::Dist2Points(const CString &strSX, const CString &strSY, const CString &strSZ, const CString &strEX, const CString &strEY, const CString &strEZ)
{
	AcGePoint3d ptS, ptE;
	ptS[X] = _tstof(strSX);
	ptS[Y] = _tstof(strSY);
	ptS[Z] = _tstof(strSZ);
	ptE[X] = _tstof(strEX);
	ptE[Y] = _tstof(strEY);
	ptE[Z] = _tstof(strEZ);
	double dd = distOF3d(ptS, ptE);
	return dd;
}

void XMJGMunicipal::exportRoadJGYSBGS()
{
	TCHAR szInPath[255] = {0};
	string strValue;
	int nRet = 0;
	bool bSuc = false;
	MStr msInfo;
	CString strTemp = _T("");
	IProjectMDB pdb;
	TCHAR szTime[255] = {0};

	VMStr records = pdb.getPictureTable();
	SmartTable::WordTable table; VStr locals;
	int tableindex = m_word.findTable(1, 1, "#Format1#", table);
	int index = m_word.setPropertyTable(tableindex);
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx];
		CString local = record[_T("λ��")];
		if(std::find(locals.begin(), locals.end(), local) == locals.end())
			locals.push_back(local);
	}
	for(int idx = 0; idx < locals.size(); ++idx)
		ReplacePic(records, locals[idx], index);
	m_word.removeTable(tableindex);
	exportGHHSJBQK();
	// 
	// 
	// 
	// 	VMStr records = pdb.getPictureTable();
	// 	if(records.size() != 0)
	// 	{
	// 		CString strPic = _T("#RoadTakePhoto1#");
	// 		bSuc = ReplacePic(records, strPic);
	// 		if(!bSuc)
	// 			acutPrintf(_T("δ�ҵ����Ƶ�ֲ�����·�����Ƭ����λ�÷ֲ�ͼ"));
	// 
	// 		strPic = _T("#RoadOutPicture1#");
	// 		bSuc = ReplacePic(records, strPic);
	// 		if(!bSuc)
	// 			acutPrintf(_T("\nδ�ҵ���·�����Ƭ"));
	// 
	// 		strPic = _T("#RoadZPT#");
	// 		bSuc = ReplacePic(records, strPic);
	// 		if(!bSuc)
	// 			acutPrintf(_T("\nδ�ҵ���·��ƽͼ"));
	// 
	// 		strPic = _T("#RoadZDMDZT#");
	// 		bSuc = ReplacePic(records, strPic);
	// 		if(!bSuc)
	// 			acutPrintf(_T("\nδ�ҵ���·�����ݶ������ͼ"));
	// 
	// 		strPic = _T("#RoadHDMDZT#");
	// 		bSuc = ReplacePic(records, strPic);
	// 		if(!bSuc)
	// 			acutPrintf(_T("\nδ�ҵ���·������������ͼ"));
	// 	}
	// 	else
	// 		acutPrintf(_T("\n��ȡ��·ͼƬ��Ϣʧ��"));
	exportRoadGHHSSQB();
	exportRoadJSZJ();
	// 
	// 	XMJGAssist assist;
	// 	CString strPath = assist.getExportFilePath();
	// 	strPath += _T("\\CG\\��·\\��·�������ձ�����.docx");
	// 	splitBigString(strPath, strValue);
	// 	bSuc = m_word.save(strValue.c_str());
	// 	if(bSuc)
	// 	{
	// 		strPath.Format(_T("\n����ɹ�: %s"), strPath);
	// 		acutPrintf(strPath);
	// 	}
	// 	else
	// 	{
	// 		strPath.Format(_T("\n ��·�������ձ��������ʧ��"));
	// 		acutPrintf(strPath);
	// 	}
}

void XMJGMunicipal::exporBridgeJGYSBGS()
{
	TCHAR szInPath[255] = {0};
	char cValue[5000] = {0};
	int nRet = 0;
	bool bSuc = false;
	MStr msInfo;
	CString strTemp = _T("");
	IProjectMDB pdb;
	TCHAR szTime[255] = {0};

	// 	if(RTNORM != ads_findfile(_T("�������ſ������ձ�����.docx"), szInPath))
	// 	{
	// 		AfxMessageBox(_T("δ�ҵ��������ſ������ձ�����ģ��"));
	// 		return;
	// 	}
	// 	m_word.setEncoding(Encoding_GBK);
	// 	TcharToChar(szInPath, cValue);
	// 	nRet = m_word.loadWord(cValue);
	// 
	// 	msInfo = pdb.getJBXXTableInfo(); string strInfo;
	// 	splitBigString(msInfo[_T("�����")], strInfo); bSuc = m_word.replaceText("#TQAcceptCode#", strInfo.c_str()); strInfo.clear();
	// 	splitBigString(msInfo[_T("���赥λ")], strInfo); bSuc = m_word.replaceText("#TQBuildCompany#", strInfo.c_str()); strInfo.clear();
	// 	splitBigString(msInfo[_T("��Ŀ����")], strInfo); bSuc = m_word.replaceText("#TQProjectName#", strInfo.c_str()); strInfo.clear();
	// 	splitBigString(msInfo[_T("��Ŀ�ص�")], strInfo); bSuc = m_word.replaceText("#TQProjectAddress#", strInfo.c_str()); strInfo.clear();
	// 	splitBigString(msInfo[_T("������λ")], strInfo); bSuc = m_word.replaceText("#TQMeansureCompany#", strInfo.c_str()); strInfo.clear();
	// 	splitBigString(msInfo[_T("ʵʩ��λ")], strInfo); bSuc = m_word.replaceText("#TQImpletedCompany#", strInfo.c_str()); strInfo.clear();
	// 	stringsOpe::GetToday(szTime); splitBigString(szTime, strInfo); bSuc = m_word.replaceText("#TQDate#", strInfo.c_str()); strInfo.clear();

	// 	msInfo = pdb.getJSSMTableInfo();
	// 	strTemp = msInfo[_T("���ɹ����ݵ��������")];
	// 	TcharToChar(strTemp, cValue);
	// 	bSuc = m_word.replaceText("#TQLicenseDoc#", cValue);
	// 	strTemp = msInfo[_T("�����������")];
	// 	TcharToChar(strTemp, cValue);
	// 	bSuc = m_word.replaceText("#TQYYZL#", cValue);
	// 	strTemp = msInfo[_T("ͼ���������")];
	// 	TcharToChar(strTemp, cValue);
	// 	bSuc = m_word.replaceText("#TQBasePoint#", cValue);
	// 	if(msInfo.find(_T("���β���")) != msInfo.end())
	// 	{
	// 		splitBigString(msInfo[_T("���β���")], strInfo);
	// 		sprintf(cValue, "%s", strInfo.c_str()); strInfo.clear();
	// 	}
	// 	else sprintf(cValue, "%s", "");
	// 	bSuc = m_word.replaceText("#TQDXCL#", cValue);
	// 	if(msInfo.find(_T("���Ų���")) != msInfo.end())
	// 	{
	// 		splitBigString(msInfo[_T("���Ų���")], strInfo);
	// 		sprintf(cValue, "%s", strInfo.c_str()); strInfo.clear();
	// 	}
	// 	else sprintf(cValue, "%s", "");
	// 	bSuc = m_word.replaceText("#TQCL#", cValue);
	// 	strTemp = msInfo[_T("��滮��ɲ�һ�����")];
	// 	TcharToChar(strTemp, cValue);
	// 	bSuc = m_word.replaceText("#TQBYZQK#", cValue);
	// 	strTemp = msInfo[_T("������Ҫ˵�������")];
	// 	TcharToChar(strTemp, cValue);
	// 	bSuc = m_word.replaceText("#TQSM#", cValue);

	MStr tqxx = pdb.getJBXXTableInfo();
	TcharToChar(tqxx[_T("������ʽ")], cValue);
	m_word.replaceText("#TQXS#", cValue);
	TcharToChar(tqxx[_T("��������ͼ���")], cValue);
	m_word.replaceText("#JGCLTBH#", cValue);
	TcharToChar(tqxx[_T("���������õر߽�������")], cValue);
	m_word.replaceText("#TQTHXJL#", cValue);
	TcharToChar(tqxx[_T("��������ͨ�����X")], cValue);
	m_word.replaceText("#STARTPOINTX#", cValue);
	TcharToChar(tqxx[_T("��������ͨ�����Y")], cValue);
	m_word.replaceText("#STARTPOINTY#", cValue);
	TcharToChar(tqxx[_T("��������ͨ���յ�X")], cValue);
	m_word.replaceText("#ENDPOINTX#", cValue);
	TcharToChar(tqxx[_T("��������ͨ���յ�Y")], cValue);
	m_word.replaceText("#ENDPOINTY#", cValue);

	tqxx.clear();
	tqxx = pdb.getBridgeXX();
	strTemp = tqxx[_T("����")];
	strTemp = strTemp.Mid(0, strTemp.GetLength() - 1);
	TcharToChar(strTemp, cValue);
	m_word.replaceText("#TQJG#", cValue);

	strTemp = tqxx[_T("����")];
	strTemp = strTemp.Mid(0, strTemp.GetLength() - 1);
	TcharToChar(strTemp, cValue);
	m_word.replaceText("#TQJK#", cValue);

	tqxx.clear();
	tqxx = pdb.getMJXXTableInfo();
	TcharToChar(tqxx[_T("�õغ������")], cValue);
	m_word.replaceText("#TQYDMJ#", cValue);
	TcharToChar(tqxx[_T("���Ž������")], cValue);
	m_word.replaceText("#TQJZMJ#", cValue);
	CString midu = tqxx[_T("���Ž����ܶ�")];
	midu.Format(_T("%.2lf"), _tstof(midu) * 100);
	TcharToChar(midu, cValue);
	m_word.replaceText("#TQJZMD#", cValue);

	VMStr vInfos = pdb.readRXBridgeTableInfo();
	WordTable table1;
	nRet = m_word.findTable(5, 2, "#ZYZB#", table1);
	if(vInfos.size() != 0)
	{
		MStr mTemp = vInfos.at(0);
		CString strZYZB = mTemp[_T("��Ҫָ��")];
		TcharToChar(strZYZB, cValue);
		table1.setCellText(5, 2, cValue);
	}
	for(int i = 0; i < (int)vInfos.size(); ++i)
	{
		MStr mInfos = vInfos.at(i);
		CString strJGs = mInfos[_T("��������")];
		CStringArray  strs;
		AnalyzeData(strs, strJGs, _T("|"));
		int nJGs = strs.GetSize() - 1;
		if(vInfos.size() - i> 0)
			bSuc = table1.copyRow(7, 0, nJGs);
		for(int j = 0; j < nJGs; ++j)
		{
			TcharToChar(strs.GetAt(j), cValue);
			table1.setCellText(8 + j, 2, cValue);
		}

		TcharToChar(mInfos[_T("��Ŀ����")], cValue);
		table1.setCellText(8, 1, cValue);
		TcharToChar(mInfos[_T("������")], cValue);
		table1.setCellText(8, 3, cValue);
		TcharToChar(mInfos[_T("��ע")], cValue);
		table1.setCellText(8, 4, cValue);

		table1.merge(8, 1, 8 + nJGs - 1, 1);
		table1.merge(8, 3, 8 + nJGs - 1, 3);
		table1.merge(8, 4, 8 + nJGs - 1, 4);
	}
	table1.removeRow(7);

	VMStr records = pdb.getPictureTable();
	SmartTable::WordTable table; VStr locals;
	int tableindex = m_word.findTable(1, 1, "#Format1#", table);
	int index = m_word.setPropertyTable(tableindex);
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx];
		CString local = record[_T("λ��")];
		if(std::find(locals.begin(), locals.end(), local) == locals.end())
			locals.push_back(local);
	}
	for(int idx = 0; idx < locals.size(); ++idx)
		ReplacePic(records, locals[idx], index);
	m_word.removeTable(tableindex);
}