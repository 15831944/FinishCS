#include "stdafx.h"

#ifndef XMJGPIPELINE_H
#include "XMJGPipeLine.h"
#endif /*XMJGPIPELINE_H*/

#ifndef XMJGEXPORTDLG
#include "XMJGDialog.h"
#endif /*XMJGEXPORTDLG*/

#include "smarttable_lua.h"

#include "XMJGTool.h"
#include "epsdef.h"
#include "EntitysOpe.h"
#include "SaveBlocks.h"
#include "SBreakBlock.h"
#include "SYPSampleJJG.h"

using namespace SmartTable;

bool g_pipelinedlg = false;
extern AttributeDef g_attribute;
CCommonDockControlBar g_pipelineproperty;
CPipeLinePropertyDlg g_pipelinepropertydlg;

CXMJGPipeLine::CXMJGPipeLine()
{

}

CXMJGPipeLine::~CXMJGPipeLine()
{

}

void CXMJGPipeLine::importPipeLine()
{
	XMJGAssist assist;	CString dir, szPathName;
	CString filter(_T("所有文件(*.*) | *.*; | Cor(*.cor) | *.cor | TXT(*.txt) | *.txt | DAT(*.dat) | *.dat | Excel(*.csv; *.xls; *.xlsx;) | *.csv; *xls; *xlsx ||"));
	if(false == assist.SelectFile(filter, dir, szPathName)) return;	VPInfo ptinfo; VStr error; szPathName = szPathName.MakeLower();
	if(szPathName.Find(_T(".cor")) != 0)
	{
		CorDef cordef;
		cordef.initFile(szPathName);
		cordef.drawData(); return;
	}
	assist.PipeExcel(szPathName, ptinfo, error, 0);
	for(int i = 0; i < (int)error.size(); ++i)
		acutPrintf(_T("\n%s该条数据不完整"), error[i]);
	IDataBaseOper prjmdb; TCHAR buf[260] = {0};
	CString str, strGdlx, strLayer, strGDDM, strKBM;
	for(int i = 1; i < (int)ptinfo.size(); ++i)
	{
		strGdlx = _T("");
		ads_point pt, ptText; CPointInfo temp = ptinfo[i];
		pt[X] = temp.m_pt3.x; pt[Y] = temp.m_pt3.y; pt[Z] = temp.m_pt3.z;
		_stprintf(buf, _T("%s"), temp.m_Name);
		for(int j = 0; j < temp.m_Name.GetLength(); ++j)
		{
			if(!isalpha(temp.m_Name.GetAt(j))) break;
			strGdlx += temp.m_Name[j];
		}
		prjmdb.readPipePointKBM(strGdlx, strKBM);
		prjmdb.readPipePointBM(strGdlx, strGDDM);
		AcDbObjectId id = addPipeLinePro(temp, strKBM);
		AddXdata(aname(id), _T("GDLX"), 0, strGdlx);
		strLayer.Format(_T("P_SC_%s"), strGdlx);
		setlayer(aname(id), strLayer);
		addapp(aname(id), strGDDM);
		ptText[X] = pt[X] + 0.2;
		ptText[Y] = pt[Y] + 0.15;
		ptText[Z] = pt[Z];
		AcDbObjectId idText = DrawText(ptText, temp.m_Name, _T("BL"), _T(""), 0.3);
		setlayer(aname(idText), _T("标注")); AcDbObjectId idGroup;
		bool bCreate = CreateGroup(aname(id), idGroup);
		bool bAdd = AddEntToGroup(idText, idGroup);
	}
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	acedCommand(RTSTR, _T("Regen"), RTNONE);
}
 
bool CXMJGPipeLine::ReplacePic(Word &word, VMStr &records, CString &strPic)
 {
 	char cValue[255] = {0};
 	int nSel = -1;
 	bool bSuc = false;
 
 	for(int i = 0; i < (int)records.size(); ++i)
 	{
 		MStr record = records.at(i);
 		CString strTemp(record[_T("位置")]);
 		if(!_tcscmp(strTemp, strPic))
 		{
 			nSel = i;
 			break;
 		}
 	}
 	if(nSel == -1)
 		return false;
 
 	MStr record = records.at(nSel);
 	CString path(record[_T("路径")]);
 	CString local(record[_T("位置")]);
 	TcharToChar(local, cValue);
	bSuc = word.setCurrentInsert(cValue, 0, true);
 	int width = 0, height = 0;
 	TcharToChar(path, cValue);
 	bSuc = getImagePngSize(cValue, width, height);
 	if(width < 3000 && height < 3000)
		bSuc = word.appendImage(cValue, 14.5, 10.88, SmartTable::Alignment_MC);
 	else if(width < 4000 && height < 4000)
		bSuc = word.appendImage(cValue, 16.18, 22.51, SmartTable::Alignment_MC);
 	else
		bSuc = word.appendImage(cValue, 34.94, 21.74, SmartTable::Alignment_MC);
 
 	return bSuc;
 }

void CXMJGPipeLine::remarkMultiPipePoint()
{
	ads_name ent; ads_point pt; TCHAR gxlx[20] = {0}; int order = 1;
	if(RTNORM != ads_entsel(_T("\n请选择要修改的管线："), ent, pt)) return;
	if(false == ReadXdata(ent, _T("GXLX"), 0, gxlx)) return;
	if(false == ads_getint(_T("\n请输入管线起始点号："), &order)) return;
	int count = GetXDataCount(ent, _T("GDZJ")); TCHAR text[200] = {0};
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(aname(ent), _T("GDZJ"), text, 0, idx);
		AcDbObjectId id = GetObjectByHandle(text);
		READMulXData(aname(ent), _T("DH"), text, 0, idx);
		int diff = idx + order - _ttoi(text + 2);
		_stprintf(text, _T("%s%d"), gxlx, idx + order);
		ADDMulXData(aname(ent), _T("DH"), text, 0, idx);
		SetEntText(aname(id), text);
		AddXdata(aname(id), _T("1"), 0, text);
		READMulXData(aname(ent), _T("LJDH"), text, 0, idx);
		if(_tcslen(text) == 0) continue;
		int index = _ttoi((text + 2));
		_stprintf(text, _T("%s%d"), gxlx, index + diff);
		ADDMulXData(aname(ent), _T("LJDH"), text, 0, idx);
	}
	flushPipeLineProperty();
	return;
	AcGePoint2dArray node; GetPlList(ent, node);
	clearExistRemark(ent);
	TCHAR layer[200] = {0}; GetEntLayer(ent, layer); int color = getcolor(ent);
	for(int idx = 0; idx < node.length(); ++idx)
	{
		AcGePoint2d tt = node.at(idx);
		_stprintf(text, _T("%s%d"), gxlx, idx + order);
		AcDbObjectId id = DrawText(AcGePoint3d(tt.x, tt.y, 0.0), text, _T("BL"), _T(""), 1.0);
		setlayer(aname(id), layer); setcolor(aname(id), color);
		CString handle = GetHandleByObject(id);
		ADDMulXData(aname(ent), _T("GDZJ"), handle, 0, idx);
		handle = GetHandleByObject(aname(ent));
		AddXdata(aname(id), _T("GLJB"), 0, handle);
		ADDMulXData(aname(ent), _T("DH"), text, 0, idx);
		AddXdata(aname(id), _T("1"), 0, text);
		READMulXData(aname(ent), _T("DMGC"), text, 3, idx);
		AddXdata(aname(ent), _T("2"), 0, text);
		READMulXData(aname(ent), _T("GXGC"), text, 3, idx);
		AddXdata(aname(ent), _T("3"), 0, text);
	}
}

void CXMJGPipeLine::remarkSinglePipePoint()
{

}

void CXMJGPipeLine::clearExistRemark(ads_name ent)
{
	int count = GetXDataCount(ent, _T("GDZJ"));
	TCHAR handle[200] = {0};
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(aname(ent), _T("GDZJ"), handle, 0, idx);
		AcDbObjectId id = GetObjectByHandle(handle);
		EntDel(aname(id));
	}
}

void CXMJGPipeLine::modifyExistRemark(ads_name ent, int order, bool dir)
{
	int count = GetXDataCount(ent, _T("GDZJ"));
	TCHAR handle[200] = {0};
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(aname(ent), _T("GDZJ"), handle, 0, idx);
		AcDbObjectId id = GetObjectByHandle(handle);

	}
}

AcDbObjectId CXMJGPipeLine::addPipeLinePro(const CPointInfo &temp, const CString &strKBM)
{
	CString str; ads_point pt;
	pt[X] = temp.m_pt3.x; pt[Y] = temp.m_pt3.y; pt[Z] = temp.m_pt3.z;
	AcDbObjectId id = DrawInsert(pt, strKBM);
	AddXdata(aname(id), _T("DH"), 200, temp.m_Name);
	str.Format(_T("%f"), temp.m_pt3.x);
	AddXdata(aname(id), _T("XZB"), 3, str);
	str.Format(_T("%f"), temp.m_pt3.y);
	AddXdata(aname(id), _T("YZB"), 3, str);
	str.Format(_T("%f"), temp.m_pt3.z);
	AddXdata(aname(id), _T("DMGC"), 3, str);
	str.Format(_T("%f"), temp.m_pipeheight);
	AddXdata(aname(id), _T("GXGC"), 3, str);
	str.Format(_T("%f"), temp.m_dvalue);
	AddXdata(aname(id), _T("DMCZ"), 3, str);
	return id;
}

void CXMJGPipeLine::createPipeLineProject()
{
	CAcModuleResourceOverride myResources;//声明该对象用来解决资源冲突问题
	CPipeLineCreateProjectDlg dlg;
	dlg.DoModal();
}

void CXMJGPipeLine::modifyProjectInfo()
{
	CAcModuleResourceOverride myResources;//声明该对象用来解决资源冲突问题
	CPipeLineModifyProjectDlg dlg;
	dlg.DoModal();
}

void CXMJGPipeLine::drawPipeLine()
{
	if(g_pipelinedlg == false)
	{
		MessageBox(GetForegroundWindow(), _T("请先打开管线属性框"), _T("错误信息"), 0);
		return;
	}
	PipeLineTable type = g_pipelinepropertydlg.m_ldlg.getCurrentPipeLineType();
	if(type.strKey.IsEmpty() == true)
	{
		MessageBox(GetForegroundWindow(), _T("请选择绘制的管线类型"), _T("错误信息"), 0);
		return;
	}
	int color = _tstoi(type.strValue); ads_point spt, ept;
	CLockManager lock; AcDbObjectId id = AcDbObjectId::kNull;
	acedInitGet(NULL, _T("L P")); bool isSelPoint = false;
	int rc = acedGetPoint(NULL, _T("\n指定起点或[选择管线(L)/连接管点(P)]："), ept);
	AcGePoint2dArray node; bool first = true;
	while(rc == RTNORM || rc == RTKWORD)
	{
		bool isSelPoint = false;
		bool isUndo = false;
		TCHAR info[255] = {0};
		if(rc == RTNORM)
		{
			node.append(apoint(ept));
			if(node.length() == 1) first = false;
			else if(id == AcDbObjectId::kNull)
			{
				id = DrawPolyline2D(node, 0.0, false, type.strGXBM, 0.0, 0, type.strGXTC);
				setlinetype(aname(id), type.strGXXX); setcolor(aname(id), color);
			}
			else
			{
				EntDel(aname(id));
				id = DrawPolyline2D(node, 0.0, false, type.strGXBM, 0.0, 0, type.strGXTC);
				setlinetype(aname(id), type.strGXXX); setcolor(aname(id), color);
			}
		}
		else if(rc == RTKWORD)
		{
			ACHAR keyWord[20];
			if(RTNORM != acedGetInput(keyWord)) return;
			if(0 == _tcscmp(keyWord, _T("U")))
			{
				node.setLogicalLength(0);
				node.setPhysicalLength(0);
				GetPlList(aname(id), node); EntDel(aname(id));
				node.removeLast();
				ept[X] = node.last().x; ept[Y] = node.last().y;
				id = DrawPolyline2D(node, 0.0, false, type.strGXBM, 0.0, 0, type.strGXTC);
				setlinetype(aname(id), type.strGXXX); setcolor(aname(id), color);
			}
			else if(0 == _tcscmp(keyWord, _T("D")))
			{
				node.setLogicalLength(0);
				node.setPhysicalLength(0); isUndo = true;
				GetPlList(aname(id), node); node.reverse(); EntDel(aname(id));
				ept[X] = node.last().x; ept[Y] = node.last().y;
				id = DrawPolyline2D(node, 0.0, false, type.strGXBM, 0.0, 0, type.strGXTC);
				setlinetype(aname(id), type.strGXXX); setcolor(aname(id), color);
			}
			else if(0 == _tcscmp(keyWord, _T("L")))
			{
				ads_name ent; ads_point pos;
				if(!ads_entsel(_T("\n请选择管线："), ent, pos)) continue;
				acdbGetObjectId(id, ent); GetPlList(aname(id), node);
				ept[X] = node.last().x; ept[Y] = node.last().y;
			}
			else if(0 == _tcscmp(keyWord, _T("P")))
			{
				if(!acedGetPoint(NULL, _T("\n请选择需要连接的管点："), ept)) continue;
				AcGePoint2dArray tnode;	ads_name ssname, ent;
				tnode.append(AcGePoint2d(ept[X] - 1, ept[Y] - 1));
				tnode.append(AcGePoint2d(ept[X] - 1, ept[Y] + 1));
				tnode.append(AcGePoint2d(ept[X] + 1, ept[Y] + 1));
				tnode.append(AcGePoint2d(ept[X] + 1, ept[Y] - 1));
				ssFromNodes(tnode, ssname, _T("CP")); long len = 0;
				ads_sslength(ssname, &len);
				for(int idx = 0; idx < len; ++idx)
				{
					ads_ssname(ssname, idx, ent);
					if(ReadXdata(ent, _T("1"), 0, info)) break;
				}
				node.append(apoint(ept)); EntDel(aname(id)); isSelPoint = true;
				id = DrawPolyline2D(node, 0.0, false, type.strGXBM, 0.0, 0, type.strGXTC);
				g_pipelinepropertydlg.m_ldlg.addPoint(apoint(ept), id, isSelPoint, info);
				setlinetype(aname(id), type.strGXXX); setcolor(aname(id), color); break;
			}
		}
		acedInitGet(NULL, _T("L P")); ads_point_set(ept, spt);
		if(isUndo == false)
			g_pipelinepropertydlg.m_ldlg.addPoint(apoint(ept), id, isSelPoint, info);
		rc = acedGetPoint(spt, _T("\n指定下一点或[连接管点(P)]："), ept);
	}
	addapp(aname(id), type.strGXBM); AddXdata(aname(id), _T("GXLX"), 0, type.strKey);
	g_pipelinepropertydlg.m_ldlg.drawFinish(); return;
}

int CXMJGPipeLine::choosePipePoint(AcDbObjectId &pid, const CString &info, const CString &type)
{
	ads_name tmp;
	CString code;
	VObjectID vids;
	ads_point entpt;
	IDataBaseOper oper;
	TCHAR szTemp[50] = {0};
	TCHAR szType[25] = {0};
	if(RTCAN == ads_entsel(info, tmp, entpt)) return 1;
	GetGroupEnts(aname(tmp), vids);
	vids.push_back(aname(tmp));
	for(size_t i = 0; i < (int)vids.size(); ++i)
	{
		AcDbObjectId id = vids[i];
		if(false == ReadXdata(aname(id), _T("GDLX"), 0, szType)) continue;
		GetStdm(aname(id), szTemp);
		if(type.CompareNoCase(_T("*")) == 0)
			oper.readPipePointBM(szType, code);
		else
			oper.readPipePointBM(type, code);
		if(code.CompareNoCase(szTemp) != 0) continue;
		pid = id; return 0;
	}
	return 2;
}

bool CXMJGPipeLine::getPipePointData(const AcDbObjectId &pid, CString &type, CString &name, AcGePoint2d &pos)
{
	TCHAR szType[25] = {0}; ads_point tpos;
	if(false == ReadXdata(aname(pid), _T("GDLX"), 0, szType)) return false;
	type.Format(_T("%s"), szType);
	GetEntPosition(aname(pid), tpos);
	pos.x = tpos[0]; pos.y = tpos[1];
	if(false == ReadXdata(aname(pid), _T("DH"), 0, szType)) return false;;
	name.Format(_T("%s"), szType);
	return true;
}

bool CXMJGPipeLine::getPipeLineProperty(const CString &jm, CString &code, CString &layer, int &color)
{
	layer.Format(_T("L_SC_%s"), jm);
	IProjectMDB prjmdb;
	CString strColor;
	prjmdb.getPipelineColor(jm, strColor);
	IDataBaseOper configmdb;
	if(false == configmdb.readPipelineDM(jm, code)) return false;
	color = _tstoi(strColor);
	return true;
}

void CXMJGPipeLine::flushPipeLineProperty()
{
	if(g_pipelinedlg == false) return;
	if(g_pipelineproperty.GetSafeHwnd() == NULL)
		modifyPipeLineProperty();
	g_pipelinepropertydlg.m_ldlg.reflashPLProperty();
	g_pipelinepropertydlg.m_pdlg.flushPipeLineProperty();
}

void CXMJGPipeLine::modifyPipeLineProperty()
{
	CAcModuleResourceOverride myResources;
	g_pipelineproperty.SetChildDlg(&g_pipelinepropertydlg, g_pipelinepropertydlg.IDD);
	if(g_pipelineproperty.GetSafeHwnd() != NULL)
	{
		if(g_pipelineproperty.IsVisible() == false)
			acedGetAcadFrame()->ShowControlBar(&g_pipelineproperty, TRUE, FALSE);
	}
	else
	{
		g_pipelineproperty.Create(acedGetAcadFrame(), _T(""));
		g_pipelineproperty.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT | CBRS_ALIGN_ANY | CBRS_BORDER_RIGHT);
		g_pipelineproperty.RestoreControlBar();
	}
	g_pipelinedlg = true;
	g_pipelinepropertydlg.m_ldlg.OnClickedButtonPipelineReflashTable();
}

void CXMJGPipeLine::matchingPipePoint()
{
	ads_name ent, ent1, ent2; ads_point pt, pt1, pt2;
	TCHAR info1[255] = {0}; TCHAR info2[255] = {0};
	while(1)
	{
		if(RTNORM != ads_entsel(_T("\n 请选择管点："), ent1, pt1)) return;
		if(RTNORM != ads_entsel(_T("\n 请选择匹配管点："), ent2, pt2)) return;
		GetStdm(ent1, info1); GetStdm(ent2, info2);
		if(_tcslen(info1) != 0 && _tcslen(info2) == 0)
		{
			ads_name_set(ent1, ent);
			ReadXdata(ent, _T("GDLX"), 0, info1);
			if(_tcscmp(info1, _T("JD")) == 0)
				GetEntPosition(ent2, pt);
			else
				GetMiddlePoint(ent2, pt);
			AcDbObjectId group;
			GetEntGroup(ent, group);
			AddEntToGroup(ent2, group);
		}
		else if(_tcslen(info1) == 0 && _tcslen(info2) != 0)
		{
			ads_name_set(ent2, ent);
			ReadXdata(ent, _T("GDLX"), 0, info1);
			if(_tcscmp(info1, _T("JD")) == 0)
				GetEntPosition(ent1, pt);
			else
				GetMiddlePoint(ent1, pt);
			AcDbObjectId group;
			GetEntGroup(ent, group);
			AddEntToGroup(ent1, group);
		}
		else
		{
			acutPrintf(_T("\n 选择的管点错误，请重新选择"));
			continue;
		}
		_stprintf(info2, _T("%.3lf"), pt[X]);
		AddXdata(ent, _T("MXZB"), 3, info2);
		_stprintf(info2, _T("%.3lf"), pt[Y]);
		AddXdata(ent, _T("MYZB"), 3, info2);
		acutPrintf(_T("\n 匹配成功"));
	}
}

void CXMJGPipeLine::changePipeLineZJ()
{
	SelectFilter sf(8, _T("ZJ"));
	AcDbObjectIdArray ids; TCHAR info[255] = {0};
	if(false == SelectEntitys(ids, _T("X"))) return;
	TCHAR str[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		ReadXdata(aname(id), _T("ZJLX"), 0, info);
		if(_tcscmp(info, _T("XH")) == 0)
			_stprintf(info, _T("%s"), _T("ZZB"));
		else
			_stprintf(info, _T("%s"), _T("XH"));
		ReadXdata(aname(id), info, 0, str);
		SetEntText(aname(id), str);
		AddXdata(aname(id), _T("ZJLX"), 0, info);
	}
}

void CXMJGPipeLine::addPipeLineRemark()
{
	int type = 0;
	if(RTCAN == ads_getint(_T("\n 请选择注记的类型[1]点号、[2]地面高程、[3]管底高程:"), &type)) return;
	SelectFilter sf(RTDXF0, _T("TEXT"));
	AcDbObjectIdArray ids; TCHAR appName[25] = {0};
	if(false == SelectEntitys(ids, _T("X"))) return;
	TCHAR value[255] = {0}; _stprintf(appName, _T("%d"), type);
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(false == ReadXdata(aname(id), appName, 0, value)) continue;
		SetEntText(aname(id), value);
	}
}

void CXMJGPipeLine::exportPipeLineDwg()
{
	CPipeLineOutputDlg dlg;
	if(dlg.DoModal() != IDOK) return;
	VStr vmsLayers = dlg.m_sout.vmsLayers;
	if(vmsLayers.size() == 0) return;
	exportEntityToDWG(vmsLayers);
}

void CXMJGPipeLine::exportPipeLineDWG()
{
	VStr vmsLayers;
	XMJGAssist assist;
	CPipeLineSelectOutputDlg dlg;
	if(IDOK != dlg.DoModal()) return;
	if(dlg.vmsLayers.size() == 0) return; CString temp;
	for(int idx = 0; idx < (int)dlg.vmsLayers.size(); ++idx)
		temp += dlg.vmsLayers[idx] + _T(";");
	TCHAR layer[1024] = {0};
	_stprintf(layer, _T("%s"), temp.Mid(0, temp.GetLength() - 1));
	resbuf *list = NULL;
	resbuf *result = NULL;
	CString savePath = assist.getExportFilePath();
	TCHAR path[MAX_PATH] = {0}; _stprintf(path, _T("%s"), savePath);
	list = acutBuildList(RTSTR, _T("xmjgXieTuFen"), RTSTR, path, RTSTR, layer, RTNONE);
	if(list == NULL) return;
	int stat = acedInvoke(list, &result);
	vmsLayers = dlg.vmsLayers;
	exportEntityToDWG(vmsLayers);
}

void CXMJGPipeLine::remarkPipePoint()
{
	remarkMultiPipePoint();
}

typedef struct DIMFLOW
{
	int dir;
	CString ljd;
	CString tgcc;
	AcGePoint2d spt;
	AcGePoint2d ept;
}DimFlow;

void CXMJGPipeLine::dimPipelineFlow()
{
	ads_name ent; ads_point pt; AcGePoint2dArray node;
	typedef map<CString, DimFlow, CmpCstr> MSPoint; double dist = 0.0;
	if(RTNORM != ads_getreal(_T("\n请输入最小的标注距离："), &dist)) return;
	if(RTNORM != ads_entsel(_T("\n请选择需要标注流向的管线："), ent, pt)) return;
	MSPoint pts; GetPlList(ent, node); int count = GetXDataCount(ent, _T("DH"));
	TCHAR layer[200] = {0}; GetEntLayer(ent, layer); int color = getcolor(ent);
	if(node.length() != count) return;
	TCHAR dh[200] = {0}, ljd[200] = {0}, tgcc[20] = {0}, yl[20] = {0};
	for(int idx = 0; idx < count; ++idx)
	{
		if(false == READMulXData(aname(ent), _T("DH"), dh, 0, idx)) continue;
		READMulXData(aname(ent), _T("LJDH"), ljd, 0, idx);
		READMulXData(aname(ent), _T("TGCC"), tgcc, 0, idx);
		READMulXData(aname(ent), _T("DYYLZ"), yl, 0, idx);
		pts[dh].spt = node.at(idx); pts[dh].ljd = ljd;
		pts[dh].tgcc.Format(_T("%s"), tgcc);
	}
	MSPoint::iterator it = pts.begin();
	for(; it != pts.end(); ++it)
	{
		CString ljd = it->second.ljd;
		it->second.ept = pts[ljd].spt;
	}
	TCHAR dimlayer[200] = {0};
	_stprintf(dimlayer, _T("FLOW_%s"), layer);
	for(it = pts.begin(); it != pts.end(); ++it)
	{
		if(it->first.GetLength() == 0) continue;
		if(it->second.ljd.GetLength() == 0) continue;
		AcGePoint2d spt = it->second.spt;
		AcGePoint2d ept = it->second.ept;
		double angle = ads_angle(apoint(spt), apoint(ept));
		double tmp = distOF2d(spt, ept);
		if(tmp < dist) continue;
		ads_point mid, mid1; mid[Z] = 0.0;
		mid[X] = (spt.x + ept.x) / 2;
		mid[Y] = (spt.y + ept.y) / 2;
		ads_polar(mid, angle + 90, 2.0, mid1);
		DrawInsert(mid1, _T("pdim"), _T(""), 0.5, angle);			//管线流向注记
		ads_entlast(ent); setlayer(ent, dimlayer);
		setcolor(ent, color);
		ads_polar(mid, angle + 90, 0.75, mid1);
		AcDbObjectId tid = DrawText(mid1, it->second.tgcc, _T("C"), _T(""), 0.5, angle);
		setlayer(aname(tid), dimlayer); setcolor(aname(tid), color);
	}
}

void CXMJGPipeLine::addPrintTK()
{
	ads_point spt, ept; AcGePoint2dArray node;
	if(RTNORM != ads_getpoint(NULL, _T("\n请选择输出范围的左上角："), spt)) return;
	if(RTNORM != ads_getcorner(spt, _T("\n请选择输出范围的右上角："), ept)) return;
	node.append(apoint(spt)); node.append(AcGePoint2d(spt[X], ept[Y]));
	node.append(apoint(ept)); node.append(AcGePoint2d(ept[X], spt[Y]));
	AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T(""));
	setlayer(aname(id), _T("打印范围"));
}

void CXMJGPipeLine::exportSingleReport()
{
	CPipeLineSelectOutputDlg m_sout;
	//m_sout.GetZPTLayers
}

void CXMJGPipeLine::modifyPipeLineZJXH()
{
	ads_name ent; ads_point pt; int xh = 0;
	if(RTNORM != ads_entsel(_T(""), ent, pt)) return;
	if(RTNORM != ads_getint(_T("\n请输入管点序号:"), &xh)) return;
	TCHAR type[255] = {0}, value[255] = {0}; MStr oldnew;
	if(false == ReadXdata(ent, _T("GXLX"), 0, type)) return;
	int count = GetXDataCount(ent, _T("DH")); CString oldname;
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(aname(ent), _T("DH"), value, 0, idx);
		oldname = value;
		_stprintf(value, _T("%s%d"), type, xh + idx);
		oldnew[oldname] = value;
		ADDMulXData(aname(ent), _T("DH"), value, 0, idx);
	}
	count = GetXDataCount(ent, _T("LJDH"));
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(aname(ent), _T("LJDH"), value, 0, idx);
		_stprintf(type, _T("%s"), oldnew[value]);
		ADDMulXData(aname(ent), _T("LJDH"), type, 0, idx);
	}
	count = GetXDataCount(ent, _T("GDZJ"));
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(aname(ent), _T("GDZJ"), value, 0, idx);
		AcDbObjectId id = GetObjectByHandle(value);
		if(id == AcDbObjectId::kNull) continue;
		ReadXdata(aname(id), _T("1"), 0, value);
		SetEntText(aname(id), oldnew[value]);
	}
	flushPipeLineProperty();
}

void CXMJGPipeLine::PipefilterObj(VStr &vals, const CString &strPipeExport)
{
	vals.push_back(_T("里程"));
	vals.push_back(_T("K道路中线"));
	vals.push_back(_T("jsyd红线图层"));
	vals.push_back(_T("实里程"));
	vals.push_back(_T("标注"));
	CStringArray retList;
	getLayerList(retList);
	IDataBaseOper oper;
	VStr layers = oper.readAllPipeLineLayer();
	VSIter it = find(layers.begin(), layers.end(), strPipeExport);
	layers.erase(it);
	for(int i = 0; i < (int)layers.size(); ++i)
		vals.push_back(layers[i]);
}

void CXMJGPipeLine::SavePipeBlock(AcDbObjectIdArray &idArr)
{
	ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	SSToIdArr(ssSave, idArr);	ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("打印范围"));
	idArr.append(dyfw);
}

bool CXMJGPipeLine::exportEntityToDWG(const VStr &vmsLayers)
{
	if(vmsLayers.size() == 0) return false;
	XMJGAssist assist;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return false;
	CString strMC, strJM, fileName; MStr filter;
	IDataBaseOper oper;
	acedSetStatusBarProgressMeter(_T("正在输出管线两图..."), 0, vmsLayers.size() - 1);
	for(int i = 0; i < (int)vmsLayers.size(); ++i)
	{
		acedSetStatusBarProgressMeterPos(i);
		filter[_T("附加值")] = vmsLayers[i];
		CString layer = oper.readPipeLineTable(_T("管线图层"), filter);
		MStr ml; ml[_T("名称")] = vmsLayers[i];
		ml[_T("图层")] = layer;
		CString jm = oper.readPipeLineTable(_T("关键字"), filter);
		ml[_T("简码")] = jm;
		exportPipeLineCGB(ml);
		exportPipeLineDZB(ml);
	}
	acedRestoreStatusBar();
	return true;
}

void CXMJGPipeLine::filterLayerEntity(const AcGePoint2dArray &node, MStr &mcly, int nType)
{
	//过滤其他图层
	AcDbObjectId id = DrawPolyline2D(node, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	AcGePoint2d lb, rt;
	getNodesExtent(node, lb, rt);
	XMJGAssist assist;
	AcDbObjectIdArray ids = assist.DrawMesh(lb, rt, mcly[_T("名称")], nType);
	MVStr filter; VStr vals;
	CString szLy = mcly[_T("图层")];
	PipefilterObj(vals, szLy);
	MIVStr filters;
	filters[FilterLayer] = vals;
	filter[_T("图层")] = vals;
	assist.FilterEntitys(ids, filter);
	if(nType == 1)
	{
		ads_point pos = {rt[X], lb[Y], 0.0};
		addPipeLineDZTuQian(pos, mcly[_T("名称")]);
		return;
	}
	TCHAR sjlayer[25] = {0};
	_stprintf(sjlayer, _T("SJ_%s"), mcly[_T("图层")]);
	TCHAR layer[25] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		GetEntLayer(id, layer);
		if(_tcscmp(layer, sjlayer) == 0) EntDel(aname(id));
	}
}

void CXMJGPipeLine::filterLayerEntity(XMJGAssist &assist, const AcGePoint2dArray &node, MStr &strMCJM, int nType)
{
	//过滤其他图层
	AcDbObjectId id = DrawPolyline2D(node, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	AcGePoint2d lb, rt;
	CString strMC = strMCJM[_T("名称")];
	CString strJM = strMCJM[_T("简码")];
	getNodesExtent(node, rt, rt);
	assist.DrawMesh(lb, rt, strMC, nType);
	MVStr filter; VStr vals;
	PipefilterObj(vals, strJM);
	if(nType == 0)
	{
		vals.push_back(_T("L_SJ_") + strJM);
		vals.push_back(_T("P_SJ_") + strJM);
	}
	MIVStr filters;
	filters[FilterLayer] = vals;
	filter[_T("图层")] = vals;
	AcDbObjectIdArray IdArr;
	assist.GetExportEntitys(IdArr, node);
	assist.FilterEntitys(IdArr, filter);
}

void CXMJGPipeLine::saveEntityToDwg(const CString &strMC, const CString &fileName)
{
	XMJGAssist assist; CSaveBlocks blocks;
	CString path = assist.getExportFilePath();
	path += _T("\\CG\\") + strMC + _T("\\");
	CString pathName(path);
	if(pathName.Right(9) != fileName) pathName.Append(fileName);
	blocks.setPath(pathName); AcDbObjectIdArray idArr;
	SavePipeBlock(idArr);
	blocks.saveBlocks2(idArr, false);
}

void CXMJGPipeLine::saveEntityToDwg(XMJGAssist &assist, const CString &strMC, const CString &fileName)
{
	CString path = assist.getExportFilePath();
	path += _T("\\CG\\") + strMC + _T("\\");
	CSaveBlocks blocks;
	CString pathName(path);
	if(pathName.Right(9) != fileName) pathName.Append(fileName);
	blocks.setPath(pathName);
	AcDbObjectIdArray idArr;
	SavePipeBlock(idArr);
	blocks.saveBlocks2(idArr, false);
}

void CXMJGPipeLine::savePipeLineDZT(MStr &mcs, const AcGePoint2dArray &node)
{
	XMJGExportZPT zpt;	zpt.setUndoFlag();
	filterLayerEntity(node, mcs, 1);
	saveEntityToDwg(mcs[_T("名称")], mcs[_T("名称")] + _T("管线对照图.dwg"));
	zpt.beginUndo();
}

void CXMJGPipeLine::savePipeLineZPT(const MStr &mcly, const AcGePoint2dArray &node)
{
	MStr mcs = const_cast<MStr &>(mcly);
	XMJGExportZPT zpt; zpt.setUndoFlag();
	filterLayerEntity(node, mcs, 0);
	CString name = mcs[_T("名称")];
	saveEntityToDwg(name, name + _T("管线总平图.dwg"));
	zpt.beginUndo();
}

MSStr CXMJGPipeLine::readPipeLineProperty(const AcDbObjectId &id, const ATTRFIELD_ARRAY &table)
{
	TCHAR value[255] = {0}; int row = 8; MSStr pipls;
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
	return pipls;
}


void fillToFormTableCGB(ExcelSheet &table, const MStr &minfo, int row, const ATTRFIELD_ARRAY &atable, bool &flag, CString ljd)
{
	table.copyRow(row); char info[255] = {0};
	MStr infos = static_cast<MStr>(minfo);
	for(int idx = 1; idx < (int)atable.size(); ++idx)
	{
		ATTRFIELD_DG dg = atable[idx];
		if(_tcscmp(dg.text, _T("连接点号")) == 0 && ljd.IsEmpty() == false)
			TcharToChar(ljd, info);
		else if(_tcscmp(dg.text, _T("压力或电压")) == 0)
		{
			if(ljd.GetLength() == 0)
			{
				TcharToChar(infos[dg.text], info);
				flag = _ttoi(infos[_T("压力或电压")]) == 1;
			}
			else
			{
				CString value;
				value.Format(_T("%d"), flag ? 0 : 1);
				TcharToChar(value, info);
			}
		}
		else
			TcharToChar(infos[dg.text], info);
		table.setCellValueText(row, idx, info);
	}
}


void CXMJGPipeLine::exportPipeLineCGB(const MStr &mcjm)
{
	MStr mcs = const_cast<MStr &>(mcjm);
	CString jm = mcs[_T("简码")];
	AcDbObjectIdArray lids = getPipeLineEntitys(mcs[_T("图层")]);
	MVStr gdxhs; TCHAR info[255] = {0}; CString bm;
	IDataBaseOper oper; oper.readPipelineDM(jm, bm);
	int len = lids.length();
	map<CString, AcDbObjectId, CmpCstr> mpobj; _stprintf(info, _T("正在预处理%s管线数据。。。"), mcs[_T("名称")]);
	acedSetStatusBarProgressMeter(info, 0, len - 1); int idx = 0;
	for(idx = 0; idx < len; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = lids.at(idx);
		GetStdm(aname(id), info);
		if(bm.CompareNoCase(info) != 0) continue;
		if(!ReadXdata(aname(id), _T("GXXH"), 0, info)) continue;
		mpobj[info] = id;
	}
	acedRestoreStatusBar();

	ATTRFIELD_ARRAY atable;
	g_attribute.get_gistable_columns(_T("PIPL"), atable);
	TCHAR path[255] = {0}; char value[255] = {0};
	if(RTNORM != ads_findfile(_T("管线成果表.xlsx"), path)) return;
	Excel excel; excel.setEncoding(Encoding_GBK);
	char szPath[255] = {0}; TcharToChar(path, szPath);
	int es = excel.loadExcel(szPath); ExcelSheet sheet;
	es = excel.getSheetByName("Sheet1", sheet); int row = 8;
	acedSetStatusBarProgressMeter(info, 0, len - 1);  idx = -1;
	map<CString, AcDbObjectId, CmpCstr>::iterator it; int indx = 0;
	for(it = mpobj.begin(); it != mpobj.end(); ++it)
	{
		acedSetStatusBarProgressMeterPos(idx); 	AcDbObjectId id = it->second;
		MSStr infos = readPipeLineProperty(id, atable);
		for(MSSIter it = infos.begin(); it != infos.end(); ++it)
		{
			MStr info = it->second; bool yl = false;
			fillToFormTableCGB(sheet, info, row++, atable, yl, _T(""));
			CString ljdh = info[_T("连接点号")]; if(ljdh.GetLength() == 0) continue;
			fillToFormTableCGB(sheet, infos[ljdh], row++, atable, yl, info[_T("点号")]);
		}
		continue;
	}
	acedRestoreStatusBar();
	XMJGAssist assist; CString savePath = assist.getExportFilePath();
	IProjectMDB pdb; MStr record = pdb.getProjectInfo(); CString name = mcs[_T("名称")];
	char val[255] = {0}; TcharToChar(name, val); excel.replaceText("#PipeLineType#", val);
	TcharToChar(record[_T("项目名称"]), val); excel.replaceText("#ProjectName#", val);
	TcharToChar(record[_T("建设单位"]), val); excel.replaceText("#BuildCompany#", val);
	savePath += _T("\\CG\\") + name + _T("\\") + name + _T("管线成果表.xlsx");
	TcharToChar(savePath, value);
	excel.save(value);
}

void fillToFormTableDZB(ExcelSheet &table, const MStr &minfo, int row)
{
	MStr infos = static_cast<MStr>(minfo);
	table.copyRow(row, 0, 3); char info[255] = {0}, dh[255] = {0}, sjdh[255] = {0};
	table.merge(row, 1, row + 2, 1);
	table.merge(row, 2, row + 2, 2);
	table.merge(row, 3, row + 2, 3);
	table.setCellValueText(row + 0, 4, "设计");
	table.setCellValueText(row + 1, 4, "实测");
	table.setCellValueText(row + 2, 4, "误差");
	TcharToChar(infos[_T("点号")], dh);
	TcharToChar(infos[_T("设计点号")], sjdh);
	if(strlen(sjdh) != 0 || strcmp(sjdh, dh) != 0)
		sprintf(info, "%s\n%s", sjdh, dh);
	else
		sprintf(info, "%s", dh);
	table.setCellValueText(row, 1, info);
	TcharToChar(infos[_T("特征点")], info); table.setCellValueText(row, 2, info);
	TcharToChar(infos[_T("附属物")], info); table.setCellValueText(row, 3, info);
	TcharToChar(infos[_T("设计X坐标")], info); table.setCellValueText(row, 5, info);
	TcharToChar(infos[_T("X坐标")], info); table.setCellValueText(row + 1, 5, info);
	double xval = _tstof(infos[_T("X坐标")]) - _tstof(infos[_T("设计X坐标")]);
	sprintf(info, "%.3lf", xval); table.setCellValueText(row + 2, 5, info);
	TcharToChar(infos[_T("设计Y坐标")], info); table.setCellValueText(row, 6, info);
	TcharToChar(infos[_T("Y坐标")], info); table.setCellValueText(row + 1, 6, info);
	double yval = _tstof(infos[_T("Y坐标")]) - _tstof(infos[_T("设计Y坐标")]);
	sprintf(info, "%.3lf", yval); table.setCellValueText(row + 2, 6, info);
	double val = sqrt(pow(xval, 2) + pow(yval, 2));
	sprintf(info, "%.3lf", val); table.setCellValueText(row + 2, 7, info);
	TcharToChar(infos[_T("设计地面高程")], info); table.setCellValueText(row, 8, info);
	TcharToChar(infos[_T("地面高程")], info); table.setCellValueText(row + 1, 8, info);
	val = _tstof(infos[_T("地面高程")]) - _tstof(infos[_T("设计地面高程")]);
	sprintf(info, "%.3lf", val); table.setCellValueText(row + 2, 8, info);
	TcharToChar(infos[_T("设计井深")], info); table.setCellValueText(row, 9, info);
	val = _tstof(infos[_T("地面高程")]) - _tstof(infos[_T("井深")]);
	sprintf(info, "%.3lf", val); table.setCellValueText(row + 1, 9, info);
	val = _tstof(infos[_T("设计地面高程")]) - _tstof(infos[_T("设计井深")]);
	sprintf(info, "%.3lf", val); table.setCellValueText(row + 2, 9, info);
	TcharToChar(infos[_T("设计管径")], info); table.setCellValueText(row, 10, info);
	TcharToChar(infos[_T("管径")], info); table.setCellValueText(row + 1, 10, info);
	TcharToChar(infos[_T("设计材质")], info); table.setCellValueText(row, 11, info);
	TcharToChar(infos[_T("材质")], info); table.setCellValueText(row + 1, 11, info);
	TcharToChar(infos[_T("设计埋设方式")], info); table.setCellValueText(row, 12, info);
	TcharToChar(infos[_T("埋设方式")], info); table.setCellValueText(row + 1, 12, info);
}

void CXMJGPipeLine::exportPipeLineDZB(const MStr &mcjm)
{
	//获取所有管点
	MStr mcs = const_cast<MStr &>(mcjm);
	CString jm = mcs[_T("简码")];
	AcDbObjectIdArray pids_sc = getPipeLineEntitys(mcs[_T("图层")]);

	TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("管线对照表.xlsx"), path)) return;
	Excel excel; excel.setEncoding(Encoding_GBK);
	char szPath[255] = {0}; TcharToChar(path, szPath);
	int isl = excel.loadExcel(szPath); ExcelSheet sheet;
	excel.setPropertySheet(1); ATTRFIELD_ARRAY atable;
	g_attribute.get_gistable_columns(_T("PIPP"), atable);
	int sheet_type = excel.getProperty(8, 1);
	bool isg = excel.getSheetByID(1, sheet); int firstRow = 8;
	map<CString, TCT, CmpCstr> cmap;//存储管点信息
	map<CString, TCT, CmpCstr>::iterator it;
	acedSetStatusBarProgressMeter(_T("正在预处理实测管点数据..."), 0, pids_sc.length() - 1);
	for(int i = 0; i < pids_sc.length(); ++i)
	{
		acedSetStatusBarProgressMeterPos(i);
		MSStr infos = readPipeLineProperty(pids_sc.at(i), atable);
		int row = 8;
		for(MSSIter it = infos.begin(); it != infos.end(); ++it, row += 3)
		{
			MStr vals = it->second;
			fillToFormTableDZB(sheet, vals, row);
		}

		// 		TCT curPoint; TCHAR value[255] = {0};
		// 		ReadXdata(aname(pids_sc.at(i)), _T("DH"), 0, value); curPoint.dh.Format(_T("%s"), value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("TZD"), 0, value); curPoint.tzd.Format(_T("%s"), value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("FSW"), 0, value); curPoint.fsw.Format(_T("%s"), value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("XZB"), 3, value); curPoint.x = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MXZB"), 3, value); curPoint.mx = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("YZB"), 3, value); curPoint.y = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MYZB"), 3, value); curPoint.my = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("DMGC"), 3, value); curPoint.dmgc = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MDMGC"), 3, value); curPoint.mdmgc = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("GXGC"), 3, value); curPoint.gxgc = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MGXGC"), 3, value); curPoint.mgxgc = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("GJ"), 0, value); curPoint.gj = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MGJ"), 0, value); curPoint.mgj = _tstof(value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("CZ"), 0, value); curPoint.cz.Format(_T("%s"), value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MCZ"), 0, value); curPoint.mcz.Format(_T("%s"), value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MSFS"), 0, value); curPoint.msfs.Format(_T("%s"), value);
		// 		ReadXdata(aname(pids_sc.at(i)), _T("MMSFS"), 0, value); curPoint.mmsfs.Format(_T("%s"), value);
		// 		cmap[curPoint.dh] = curPoint;
		// 	}
		// 	acedRestoreStatusBar();
		// 	int count = cmap.size(); sheet.copyRow(8, 0, count * 3 - 1);
		// 	it = cmap.begin(); char cvalue[255] = {0}; int row = 8; int idx = 0;
		// 	acedSetStatusBarProgressMeter(_T("正在输出管点数据..."), 0, count - 1);
		// 	for(it = cmap.begin(); it != cmap.end(); ++it, row += 3)
		// 	{
		// 		acedSetStatusBarProgressMeterPos(idx++); TCT tct = it->second;
		// 		sheet.merge(row, 1, row + 2, 1); TcharToChar(tct.dh, cvalue); sheet.setCellValueText(row, 1, cvalue);
		// 		sheet.merge(row, 2, row + 2, 2); TcharToChar(tct.tzd, cvalue); sheet.setCellValueText(row, 2, cvalue);
		// 		sheet.merge(row, 3, row + 2, 3); TcharToChar(tct.fsw, cvalue); sheet.setCellValueText(row, 3, cvalue);
		// 		sheet.setCellValueText(row, 4, "设计"); sheet.setCellValueText(row + 1, 4, "实测");
		// 		sheet.setCellValueText(row + 2, 4, "误差");
		// 		sheet.setCellValueDouble(row + 0, 5, tct.mx); sheet.setCellValueDouble(row + 1, 5, tct.x);
		// 		double x = tct.x - tct.mx; 	sheet.setCellValueDouble(row + 2, 5, x);
		// 		sheet.setCellValueDouble(row + 0, 6, tct.my); sheet.setCellValueDouble(row + 1, 6, tct.y);
		// 		double y = tct.y - tct.my; sheet.setCellValueDouble(row + 2, 6, y);
		// 		sheet.setCellValueDouble(row + 2, 7, sqrt(pow(x, 2) + pow(y, 2)));
		// 		sheet.setCellValueDouble(row, 8, tct.mdmgc); sheet.setCellValueDouble(row + 1, 8, tct.dmgc);
		// 		sheet.setCellValueDouble(row + 2, 8, tct.dmgc - tct.mdmgc);
		// 		sheet.setCellValueDouble(row, 9, tct.mgxgc); sheet.setCellValueDouble(row + 1, 9, tct.gxgc);
		// 		sheet.setCellValueDouble(row + 2, 9, tct.gxgc - tct.mgxgc);
		// 		sheet.setCellValueDouble(row, 10, tct.mgj); sheet.setCellValueDouble(row + 1, 10, tct.gj);
		// 		TcharToChar(tct.mcz, cvalue); sheet.setCellValueText(row, 11, cvalue);
		// 		TcharToChar(tct.cz, cvalue); sheet.setCellValueText(row + 1, 11, cvalue);
		// 		TcharToChar(tct.mmsfs, cvalue); sheet.setCellValueText(row, 12, cvalue);
		// 		TcharToChar(tct.msfs, cvalue); sheet.setCellValueText(row + 1, 12, cvalue);
		// 	}
		acedRestoreStatusBar();
		// 	it = cmap.begin(); int curRow = firstRow;
		// 		
		// 	for(int i = 0;it != cmap.end(); ++it, ++i)
		// 	{
		// 		acedSetStatusBarProgressMeterPos(i);
		// 		char cvalue[255]; TcharToChar(it->first, cvalue);
		// 		char curValue[255];
		// 		sheet.merge(curRow, 1, curRow + 2, 1);
		// 		sheet.setCellValueText(curRow, 1, cvalue);
		// 		TcharToChar(it->second.pfeat, cvalue);
		// 		sheet.merge(curRow, 2, curRow + 2, 2);
		// 		sheet.setCellValueText(curRow, 2, cvalue);
		// 		TcharToChar(it->second.pattach, cvalue);
		// 		sheet.setCellValueText(curRow, 3, cvalue);
		// 		sheet.merge(curRow, 3, curRow + 2, 3);
		// 		sheet.setCellValueText(curRow, 4, "设计");
		// 		sheet.setCellValueText(curRow + 1, 4, "实测");
		// 		sheet.setCellValueText(curRow + 2, 4, "误差");
		// 		if(it->second.type == 1)
		// 		{
		// 			sheet.setCellValueDouble(curRow, 5, it->second.x);
		// 			sheet.setCellValueDouble(curRow, 6, it->second.y);
		// 			sheet.setCellValueDouble(curRow, 8, it->second.gh);
		// 			sheet.setCellValueDouble(curRow, 9, it->second.bh);
		// 			sheet.setCellValueDouble(curRow, 10, it->second.r);
		// 			TcharToChar(it->second.pmat, cvalue);
		// 			sheet.setCellValueText(curRow, 11, cvalue);
		// 			TcharToChar(it->second.pbury, cvalue);
		// 			sheet.setCellValueText(curRow, 12, cvalue);
		// 			if (it->second->isc == 1)
		// 			{
		// 				sheet.setCellValueDouble(curRow + 1, 5, it->second.cp->x);
		// 				sheet.setCellValueDouble(curRow + 1, 6, it->second.cp->y);
		// 				sprintf(cvalue, "%s%d%s%d", 'E', curRow + 1, "-E", curRow);
		// 				sheet.setCellFunction(curRow + 2, 5, cvalue, curValue);
		// 				sprintf(cvalue, "%s%d%s%d", 'F', curRow + 1, "-F", curRow);
		// 				sheet.setCellFunction(curRow + 2, 6, cvalue, curValue);
		// 				sprintf(cvalue, "%s%d%s%d%s%d%s", "SQRT(E", curRow + 2, "*E", curRow + 2, "+F", curRow + 2, "*F", curRow + 2, ")");
		// 				sheet.setCellFunction(curRow + 2, 7, cvalue, curValue);
		// 				sheet.setCellValueDouble(curRow + 1, 8, it->second->cp->gh);
		// 				sheet.setCellValueDouble(curRow + 1, 9, it->second->cp->bh);
		// 				sprintf(cvalue, "%s%d%s%d", 'H', curRow, "-H", curRow + 1);
		// 				sheet.setCellFunction(curRow + 2, 8, cvalue, curValue);
		// 				sprintf(cvalue, "%s%d%s%d", 'I', curRow, "-I", curRow + 1);
		// 				sheet.setCellFunction(curRow + 2, 9, cvalue, curValue);
		// 
		// 				sheet.setCellValueDouble(curRow + 1, 10, it->second->cp->r);
		// 				TcharToChar(it->second->cp->pmat, cvalue);
		// 				sheet.setCellValueText(curRow + 1, 11, cvalue);
		// 				TcharToChar(it->second->cp->pbury, cvalue);
		// 				sheet.setCellValueText(curRow + 1, 12, cvalue);
		// 
		// 			}
		// 		}
		// 		else
		// 		{
		// 			sheet.setCellValueDouble(curRow + 1, 5, it->second->x);
		// 			sheet.setCellValueDouble(curRow + 1, 6, it->second->y);
		// 			sheet.setCellValueDouble(curRow + 1, 8, it->second->gh);
		// 			sheet.setCellValueDouble(curRow + 1, 9, it->second->bh);
		// 			sheet.setCellValueDouble(curRow + 1, 10, it->second->r);
		// 			TcharToChar(it->second->pmat, cvalue);
		// 			sheet.setCellValueText(curRow + 1, 11, cvalue);
		// 			TcharToChar(it->second->pbury, cvalue);
		// 			sheet.setCellValueText(curRow + 1, 12, cvalue);
		// 		}
		// 		curRow += 3;
		// 	}
		// 	acedRestoreStatusBar();
		// 	sheet.merge(curRow, 1, curRow, 4);
		// 	sheet.setCellValueText(curRow, 1, "(注：管底高程对照数据均以主干管线数据为准）");
		// 	for (int i = 8; i < curRow; ++i)//写入格式
		// 	{
		// 		for (int j = 0; j < 13; ++j)
		// 		{
		// 			sheet.setCellProperty(i, j, sheet_type);
		// 		}
	}
	XMJGAssist assist; CString mcsstr = mcs[_T("名称")];
	CString outPath = assist.getExportFilePath();
	IProjectMDB pdb; MStr record = pdb.getProjectInfo(); CString name = mcs[_T("名称")];
	char val[255] = {0}; TcharToChar(name, val); excel.replaceText("#PipeLineType#", val);
	TcharToChar(record[_T("项目名称"]), val); excel.replaceText("#ProjectName#", val);
	outPath += _T("\\CG\\") + mcs[_T("名称")] + _T("\\") + mcs[_T("名称")] + _T("管线对照表.xlsx");
	char svalue[255] = {0}; TcharToChar(outPath, svalue);
	bool isssss = excel.save(svalue); acutPrintf(_T("\n") + outPath);
}

AcDbObjectIdArray CXMJGPipeLine::getPipeLineEntitys(const CString &layer)
{
	SelectFilter sf(8, layer); AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T("X"))) return ids;
	return ids;
}

AcDbObjectIdArray CXMJGPipeLine::getPipePointEntitys(const CString &jm)
{
	//SelectFilter sf(8, layer);
	return AcDbObjectIdArray();
}

void CXMJGPipeLine::exportRoadJSZJ()
{
	XMJGAssist assist;
	MStr msInfo;
	CString strXMMC;
	IProjectMDB pdb;
	Word word;
	TCHAR szPath[255] = {0};
	bool bSuc = false;
	char cPath[255] = {0}, cXMMC[255] = {0}, cOutPutPath[255] = {0};
	ads_name ssName;
	long nLen = 0;
	AcDbObjectIdArray ids;

	msInfo = pdb.getProjectInfo();
	strXMMC = msInfo[_T("项目名称")];
	if(RTNORM != ads_findfile(_T("道路技术总结.docx"), szPath)) return;
	TcharToChar(szPath, cPath);
	word.setEncoding(SmartTable::Encoding_GBK);
	if(1 != word.loadWord(cPath)) return;
	TcharToChar(strXMMC, cXMMC);
	bSuc = word.replaceText("#XMMC#", cXMMC);

	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	struct resbuf *rc = acutBuildList(RTDXF0, _T("*POLYLINE"), 8, _T("SCDLX"), 0);
	acedSSGet(_T("X"), NULL, NULL, rc, ssName); ads_relrb(rc);
	acedSSLength(ssName, &nLen);
	SSToIdArr(ssName, ids);
	PlaneComparisonTable(ids, word);//道路竣工平面对照表
	ZDMComparisonTable(ids, word);//道路竣工纵断对照表
	HDMComparisonTable(ids, word);//道路竣工横断对照表
	JSZJTable(ids, word);//道路竣工技术总结小表

	CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\道路\\道路技术总结.docx");
	TcharToChar(strPath, cOutPutPath);
	bSuc = word.save(cOutPutPath);
	strXMMC.Format(_T("\n %s"), strPath);
	acutPrintf(strXMMC);
}

void CXMJGPipeLine::exportRoadJSZJ(Word &word)
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
	bSuc = word.replaceText("#XMMC#", cXMMC);
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	struct resbuf *rc = acutBuildList(RTDXF0, _T("*POLYLINE"), 8, _T("SCDLX"), 0);
	acedSSGet(_T("X"), NULL, NULL, rc, ssName); ads_relrb(rc);
	acedSSLength(ssName, &nLen);
	SSToIdArr(ssName, ids);
	PlaneComparisonTable(ids, word);//道路竣工平面对照表
	ZDMComparisonTable(ids, word);//道路竣工纵断对照表
	HDMComparisonTable(ids, word);//道路竣工横断对照表
	JSZJTable(ids, word);//道路竣工技术总结小表

	/*CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\道路\\道路技术总结.docx");
	TcharToChar(strPath, cOutPutPath);
	bSuc = word.save(cOutPutPath);*/
	bSuc = word.save();
	/*strXMMC.Format(_T("\n %s"), strPath);
	acutPrintf(strXMMC);*/
}

//道路竣工平面对照表
void CXMJGPipeLine::PlaneComparisonTable(const AcDbObjectIdArray &ids, Word &word)
{
	bool bSuc = false;
	WordTable table1;
	if(-1 == word.findTable(5, 2, "#START1#", table1)) return;
	for(int i = 0; i < ids.length(); i++)
	{
		WordTable table2;
		if(ids.length() - i > 1)
		{
			bSuc = word.setCurrentInsert(table1);
			word.copyTable(table1, table2);
		}

		TCHAR szStdm[50] = {0}, szDLMC[255] = {0}, szDLQD[255] = {0}, szDLZD[255] = {0};
		char cTemp[255] = {0};
		AcDbObjectId idTemp = ids.at(i);
		GetStdm(aname(idTemp), szStdm);
		if(_tcscmp(szStdm, _T("180302"))) continue;
		ReadXdata(aname(idTemp), _T("道路中心线名称"), STRING_TYPE, szDLMC);
		ReadXdata(aname(idTemp), _T("道路中心线起点"), STRING_TYPE, szDLQD);
		ReadXdata(aname(idTemp), _T("道路中心线终点"), STRING_TYPE, szDLZD);
		vector<CString> vstr1, vstr2;
		CString strDLQD(szDLQD), strDLZD(szDLZD);
		AnalyzeData(vstr1, strDLQD, _T(","));
		AnalyzeData(vstr2, strDLZD, _T(","));
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(1, 1, cTemp);
		TcharToChar(vstr1.at(0), cTemp);
		table1.setCellText(5, 2, cTemp);
		TcharToChar(vstr1.at(1), cTemp);
		table1.setCellText(6, 2, cTemp);
		TcharToChar(vstr1.at(2), cTemp);
		table1.setCellText(7, 2, cTemp);
		TcharToChar(vstr2.at(0), cTemp);
		table1.setCellText(8, 2, cTemp);
		TcharToChar(vstr2.at(1), cTemp);
		table1.setCellText(9, 2, cTemp);
		TcharToChar(vstr2.at(2), cTemp);
		table1.setCellText(10, 2, cTemp);
		double dRoadLen = Dist2Points(vstr1, vstr2);
		sprintf(cTemp, "%f", dRoadLen);
		table1.setCellText(11, 2, cTemp);
		table1 = table2;
	}
}

//道路竣工纵断对照表
void CXMJGPipeLine::ZDMComparisonTable(const AcDbObjectIdArray &ids, Word &word)
{
	bool bSuc = false;
	WordTable table1, table2;
	int nFind = word.findTable(3, 2, "#START2#", table1);
	nFind = word.findTable(2, 3, "#START3#", table2);
	ads_point pt1, pt2;
	ads_name ssName, ssText;
	long nLen = 0;
	TCHAR szStdm[255] = {0};
	AcDbObjectIdArray idgets;
	AcGePoint2dArray Nodes;

	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	for(int i = 0; i < ids.length(); i++)
	{
		WordTable table3, table4;
		if(ids.length() - i > 1)
		{
			bSuc = word.setCurrentInsert(table1);
			bSuc = word.appendParagraph("");
			bSuc = word.setCurrentInsert(table1, 1);
			word.copyTable(table1, table3);

			bSuc = word.setCurrentInsert(table2);
			bSuc = word.appendParagraph("");
			bSuc = word.setCurrentInsert(table2, 1);
			word.copyTable(table2, table4);
		}

		TCHAR szDLMC[255] = {0};
		char cTemp[255] = {0};
		AcDbObjectId idTemp = ids.at(i);
		ReadXdata(aname(idTemp), _T("道路中心线名称"), STRING_TYPE, szDLMC);
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(3, 1, cTemp);
		table2.setCellText(2, 1, cTemp);
		word.replaceText("#START3#", "");

		GetPlList(aname(idTemp), Nodes);
		pt1[X] = Nodes.at(0)[X]; pt1[Y] = Nodes.at(0)[Y];
		pt2[X] = Nodes.at(1)[X]; pt2[Y] = Nodes.at(1)[Y];
		SNodesOpe nodesO;
		nodesO.modifyLB_RTPoint(pt1, pt2);
		pt1[X] = pt1[X] - 2; pt1[Y] = pt1[Y] - 2;
		pt2[X] = pt2[X] + 2; pt2[Y] = pt2[Y] + 2;
		struct resbuf *rc = acutBuildList(RTDXF0, _T("TEXT"), 8, _T("LCZH"), 0);
		acedSSGet(_T("W"), pt1, pt2, rc, ssName);
		ads_relrb(rc);
		acedSSLength(ssName, &nLen);
		for(int j = 0; j < nLen; ++j)
		{
			TCHAR szLCZBH[255] = {0};
			char cLCZBH[255] = {0};
			ads_ssname(ssName, nLen - j - 1, ssText);//ssget后的ssname桩号是逆序,所以nLen-j-1使桩号正序
			GetStdm(ssText, szStdm);
			if(_tcscmp(szStdm, _T("992100"))) continue;
			GetEntText(ssText, szLCZBH);
			TcharToChar(szLCZBH, cLCZBH);
			if(nLen - j > 1)
			{
				bSuc = table1.copyRow(3, 3 + j);
			}
			table1.setCellText(3 + j, 2, cLCZBH);
		}
		table1 = table3;
		table2 = table4;
	}
}

//道路竣工横断对照表
void CXMJGPipeLine::HDMComparisonTable(const AcDbObjectIdArray &ids, Word &word)
{
	bool bSuc = false;
	WordTable table1;
	int nFind = word.findTable(2, 2, "#START4#", table1);

	for(int i = 0; i < ids.length(); i++)
	{
		WordTable table2;
		if(ids.length() - i > 1)
		{
			bSuc = word.setCurrentInsert(table1);
			bSuc = word.appendParagraph("");
			bSuc = word.setCurrentInsert(table1, 1);
			word.copyTable(table1, table2);
		}

		TCHAR szDLMC[255] = {0};
		char cTemp[255] = {0};
		AcDbObjectId idTemp = ids.at(i);
		ReadXdata(aname(idTemp), _T("道路中心线名称"), STRING_TYPE, szDLMC);
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(2, 1, cTemp);
		word.replaceText("#START4#", "");
		table1 = table2;
	}
}

//道路竣工技术总结小表
void CXMJGPipeLine::JSZJTable(const AcDbObjectIdArray &ids, Word &word)
{
	bool bSuc = false;
	WordTable table1;
	int nFind = word.findTable(2, 2, "#START5#", table1);
	for(int i = 0; i < ids.length(); i++)
	{
		WordTable table2;
		AcDbObjectId idTemp = ids.at(i);
		TCHAR szStdm[255] = {0};
		GetStdm(aname(idTemp), szStdm);
		if(_tcscmp(szStdm, _T("180302"))) continue;
		if(ids.length() - i > 1)
		{
			bSuc = word.setCurrentInsert(table1);
			bSuc = word.appendParagraph("");
			bSuc = word.setCurrentInsert(table1, 1);
			word.copyTable(table1, table2);
		}
		TCHAR szDLMC[255] = {0};
		char cTemp[255] = {0};
		ReadXdata(aname(idTemp), _T("道路中心线名称"), STRING_TYPE, szDLMC);
		TcharToChar(szDLMC, cTemp);
		table1.setCellText(2, 1, cTemp);
		word.replaceText("#START5#", "");
		table1 = table2;
	}
}

double CXMJGPipeLine::Dist2Points(const vector<CString> &pt1, const vector<CString> &pt2)
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

void CXMJGPipeLine::exportRoadGXJSZJ(Word &word)
{
	MStr msInfo;
	CString strXKWS;
	IProjectMDB pdb;
	XMJGAssist assist;
	bool bSuc = false;
	char cXKWS[255] = {0}, cOutPutPath[255] = {0};

	msInfo = pdb.getJSSMTableInfo();
	strXKWS = msInfo[_T("测绘成果依据的许可文书")];
	TcharToChar(strXKWS, cXKWS);
	bSuc = word.replaceText("#LicenseDoc#", cXKWS);

	CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\测绘成果依据的许可文书.docx");
	TcharToChar(strPath, cOutPutPath);
	bSuc = word.save(cOutPutPath);
	if(bSuc)
	{
		strPath.Format(_T("\n %s"), strPath);
		acutPrintf(strPath);
	}
	else
	{
		strPath.Format(_T("\n %s输出失败"), strXKWS);
		acutPrintf(strPath);
	}
	//bSuc = word.save();
}

//bool ReplacePic(Word &word, VMStr &records, CString &strPic)
//{
//	char cValue[255] = {0};
//	int nSel = -1;
//	bool bSuc = false;
//
//	for(int i = 0; i < records.size(); ++i)
//	{
//		MStr record = records.at(i);
//		CString strTemp(record[_T("位置")]);
//		if(!_tcscmp(strTemp, strPic))
//		{
//			nSel = i;
//			break;
//		}
//	}
//	if(nSel == -1)
//		return false;
//
//	MStr record = records.at(nSel);
//	CString path(record[_T("路径")]);
//	CString local(record[_T("位置")]);
//	TcharToChar(local, cValue);
//	bSuc = word.setCurrentInsert(cValue, 0, true);
//	int width = 0, height = 0;
//	TcharToChar(path, cValue);
//	bSuc = getImagePngSize(cValue, width, height);
//	if(width < 3000 && height < 3000)
//		bSuc = word.appendImage(cValue, 14.5, 10.88, SmartTable::Alignment_MC);
//	else if(width < 4000 && height < 4000)
//		bSuc = word.appendImage(cValue, 16.18, 22.51, SmartTable::Alignment_MC);
//	else
//		bSuc = word.appendImage(cValue, 34.94, 21.74, SmartTable::Alignment_MC);
//
//	return bSuc;
//}

void CXMJGPipeLine::exportRoadJGYSBGS()
{
	TCHAR szInPath[255] = {0};
	char cValue[255] = {0};
	int nRet = 0;
	bool bSuc = false;
	MStr msInfo;
	CString strTemp = _T("");
	IProjectMDB pdb;
	TCHAR szTime[255] = {0};

	if(RTNORM != ads_findfile(_T("道路竣工验收报告书.docx"), szInPath))
	{
		AfxMessageBox(_T("未找到道路竣工验收报告书模版"));
		return;
	}
	Word word; word.setEncoding(Encoding_GBK);
	TcharToChar(szInPath, cValue);
	nRet = word.loadWord(cValue);

	msInfo = pdb.getJBXXTableInfo();
	strTemp = msInfo[_T("受理号")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#AcceptCode#", cValue);
	strTemp = msInfo[_T("建设单位")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#BuildCompany#", cValue);
	strTemp = msInfo[_T("项目名称")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#ProjectName#", cValue);
	strTemp = msInfo[_T("项目地点")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#ProjectAddress#", cValue);
	strTemp = msInfo[_T("测量单位")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#MeansureCompany#", cValue);
	strTemp = msInfo[_T("实施单位")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#ImpletedCompany#", cValue);
	stringsOpe::GetToday(szTime);
	TcharToChar(szTime, cValue);
	bSuc = word.replaceText("#Date#", cValue);

	msInfo = pdb.getJSSMTableInfo();
	strTemp = msInfo[_T("测绘成果依据的许可文书")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#LicenseDoc#", cValue);
	strTemp = msInfo[_T("已有资料情况")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#YYZLQK#", cValue);
	strTemp = msInfo[_T("图根控制情况")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#BasePoint#", cValue);
	strTemp = msInfo[_T("细部测量基本情况")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#DetailConvery#", cValue);
	strTemp = msInfo[_T("规划实施的基本情况")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#GHHSJBQK#", cValue);
	strTemp = msInfo[_T("与规划许可不一致情况")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#YGHXKBYZQK#", cValue);
	strTemp = msInfo[_T("其他需要说明的情况")];
	TcharToChar(strTemp, cValue);
	bSuc = word.replaceText("#QTXYSMQK#", cValue);

	VMStr records = pdb.getPictureTable();
	CString strPic = _T("#RoadTakePhoto1#");
	bSuc = ReplacePic(word, records, strPic);
	if(!bSuc)
	{
		acutPrintf(_T("未找到控制点分布及道路外观照片拍摄位置分布图"));
	}

	strPic = _T("#RoadOutPicture1#");
	bSuc = ReplacePic(word, records, strPic);
	if(!bSuc)
	{
		acutPrintf(_T("\n未找到道路外观照片"));
	}

	strPic = _T("#RoadZPT#");
	bSuc = ReplacePic(word, records, strPic);
	if(!bSuc)
	{
		acutPrintf(_T("\n未找到道路总平图"));
	}

	strPic = _T("#RoadZDMDZT#");
	bSuc = ReplacePic(word, records, strPic);
	if(!bSuc)
	{
		acutPrintf(_T("\n未找到道路竣工纵断面对照图"));
	}

	strPic = _T("#RoadHDMDZT#");
	bSuc = ReplacePic(word, records, strPic);
	if(!bSuc)
	{
		acutPrintf(_T("\n未找到道路竣工横断面对照图"));
	}

	XMJGAssist assist;
	CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\道路竣工验收报告书.docx");
	TcharToChar(strPath, cValue);
	bSuc = word.save(cValue);
	if(bSuc)
	{
		strPath.Format(_T("\n输出成功: %s"), strPath);
		acutPrintf(strPath);
	}
	else
	{
		strPath.Format(_T("\n 道路竣工验收报告书输出失败"));
		acutPrintf(strPath);

	}
}

void CXMJGPipeLine::addPipeLineDZTuQian(ads_point pos, const CString &name)
{
	AcDbObjectId id = DrawInsert(pos, _T("PipeLineDZ"));
	SBreakBlock block; block.SetBlock(id); block.Explode();
	TCHAR info[255] = {0}; IDataBaseOper oper; CString val;
	MStr filter; filter[_T("附加值")] = name;
	for(int idx = 0; idx < block.m_objIdArr.length(); ++idx)
	{
		AcDbObjectId id = block.m_objIdArr.at(idx);
		if(true == ReadXdata(aname(id), _T("SCGX"), 0, info))
		{
			val = oper.readPipeLineTable(_T("属性值"), filter);
			setcolor(aname(id), _ttoi(val));
		}
		else if(true == ReadXdata(aname(id), _T("SCLX"), 0, info))
		{
			val.Format(_T("实测%s"), name);
			SetEntText(aname(id), val);
		}
		else if(true == ReadXdata(aname(id), _T("SJLX"), 0, info))
		{
			val.Format(_T("设计%s"), name);
			SetEntText(aname(id), val);
		}
	}
}