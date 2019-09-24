#include "stdafx.h"
#include "XMJGReadData.h"
#include <apoint.hpp>
#include "Global.h"
#include <cassGlobal.h>
#include <SBreakBlock.h>
#include <SaveBlocks.h>
#include "Dialogs.h"
#include "smarttable_lua.h"
#include "XMJGDialog.h"
#include "geometric2d.h"
#include "Tukuang_Common.h"
#include <XRecordOperate.h>
#include <cassGlobal.h>
#include <dbplotsettings.h>
#include <AcPlPlotEngine.h>
#include <atlpath.h>
#include <axlock.h>
#include <AcDbLMgr.h>
#include <dbsymtb.h>
#include <dbplotsetval.h>
#include <AcPlPlotProgress.h>
#include <AcPlPlotInfoValidator.h>
#include <AcPlPlotFactory.h>
#include "DatabaseOperManager.h"
#include <dblead.h >
#include <dbmtext.h>
#include "XRecordOperate.h"
#include "setentitypropery.h"
#include "C3DOperator.h"
#include "NewSetLayerInfoDlg.h"
#include "QualityCheckReport.h"

typedef map<AcDbObjectId, AcDbObjectIdArray> MIID;
typedef MIID::iterator MIIDIter;

#define XMJGPB _T("Software\\SouthSurvey\\XMJG\\GPedestrianBridge")
#include "XMJGDialog.h"
#ifndef XMJGTOOL_H
#include "XMJGTool.h"
#endif /*XMJGTOOL_H*/

#include "FilePathOperater.h"
#include "DrawUpPlanning.h"
#define TEXTSIZE	1.679

extern FieldChangeTable g_fieldchagetable;

#define replacefield    XMJGReplceField::ReplaceField

IProjectInfo::IProjectInfo(const CString &title)
	: m_type(title)
{
	_manager = new CDatabaseOperManager; TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), path)) return;
	_manager->initialConnect(path);
	CString table(_T("XMJGPath")); MStr filter;
	filter[_T("�ؼ���")] = _T("����Ŀ¼");
	_manager->ReadDataBase(table, _T("����ֵ"), filter, m_dir);
	m_dir += m_type + _T("\\");
}

IProjectInfo::~IProjectInfo()
{

}

bool IProjectInfo::CreateProject(const MStr &record)
{
	m_record = const_cast<MStr &>(record);
	m_name = m_record[_T("��Ŀ����")]; m_code = m_record[_T("�����")];
	if(_taccess(m_dir, 0) == -1) _tmkdir(m_dir);
	CStringArray files, paths; CString path;
	path.Format(_T("%s%s_%s"), m_dir, m_code, m_name);
	CFilePathOperater::GetChildDirectory(m_dir, paths, files);
	for(int idx = 0; idx < files.GetSize(); ++idx)
	{
		CString file = files.GetAt(idx);
		int pos = file.Find(_T("_"));
		file = file.Mid(0, pos);
		if(file.Find(m_code) == -1) continue;
		CString info(_T("������Ѵ��ڣ���������д����ţ���Ҫ����ʹ�ø�����ţ�����ɾ������Ŀ¼�¸���Ŀ�ļ��С�"));
		int ret = MessageBox(GetForegroundWindow(), info, _T("��Ϣ��ʾ"), MB_OKCANCEL);
		if(ret == 1) CFilePathOperater::ClearFolder(path, false);
		else return false;
	}
	createProjectDir(); updataProjectInfo(); updataDWG();
	return true;
}

bool IProjectInfo::CreateProject(const CString &name, const MStr &record)
{
	IDataBaseOper oper;
	m_record = const_cast<MStr &>(record);
	CString outputfile = oper.readFileNameTable(name);
	m_name = m_record[_T("��Ŀ����")]; m_code = m_record[_T("�����")];
	if(_taccess(m_dir, 0) == -1) _tmkdir(m_dir);
	CStringArray files, paths; CString path;
	path.Format(_T("%s%s_%s"), m_dir, m_code, m_name);
	CFilePathOperater::GetChildDirectory(m_dir, paths, files);
	for(int idx = 0; idx < files.GetSize(); ++idx)
	{
		CString file = files.GetAt(idx);
		int pos = file.Find(_T("_"));
		file = file.Mid(0, pos);
		if(file.Find(m_code) == -1) continue;
		CString info(_T("������Ѵ��ڣ���������д����ţ���Ҫ����ʹ�ø�����ţ�����ɾ������Ŀ¼�¸���Ŀ�ļ��С�"));
		int ret = MessageBox(GetForegroundWindow(), info, _T("��Ϣ��ʾ"), MB_OKCANCEL);
		if(ret == 1) CFilePathOperater::ClearFolder(path, false);
		else return false;
	}
	createProjectDir(outputfile); updataProjectInfo(); updataDWG(outputfile);
	return true;
}

bool IProjectInfo::CreateProject(const CString &name, const CString &code)
{
	m_name = name; m_code = code;
	if(_taccess(m_dir, 0) == -1) _tmkdir(m_dir);
	CStringArray files, paths; CString path;
	path.Format(_T("%s%s_%s"), m_dir, code, name);
	CFilePathOperater::GetChildDirectory(m_dir, paths, files);
	for(int idx = 0; idx < files.GetSize(); ++idx)
	{
		CString file = files.GetAt(idx);
		int pos = file.Find(_T("_"));
		file = file.Mid(0, pos);
		if(file.Find(code) == -1) continue;
		CString info; info.Format(_T("�Ƿ񸲸��ļ���%s"), path);
		int ret = MessageBox(GetForegroundWindow(), info, _T("��Ϣ��ʾ"), MB_OKCANCEL);
		if(ret == 1) CFilePathOperater::ClearFolder(path, false);
		else return false;
	}
	createProjectDirEx(name); updataProjectInfo(); updataDWG();
	return true;
}

bool IProjectInfo::CreateProjectEx(const MStr &record, const CString &type)
{
	m_record = const_cast<MStr &>(record);
	m_name = m_record[_T("��Ŀ����")]; m_code = m_record[_T("�����")];
	if(_taccess(m_dir, 0) == -1) _tmkdir(m_dir);
	CStringArray files, paths; CString path;
	path.Format(_T("%s%s_%s\\%s"), m_dir, m_code, m_name, type);
	CFilePathOperater::GetChildDirectory(m_dir, paths, files);
	for(int idx = 0; idx < files.GetSize(); ++idx)
	{
		CString file = files.GetAt(idx);
		int pos = file.Find(_T("_"));
		file = file.Mid(0, pos);
		if(file.Find(m_code) == -1) continue;
		CString info(_T("������Ѵ��ڣ���������д����ţ���Ҫ����ʹ�ø�����ţ�����ɾ������Ŀ¼�¸���Ŀ�ļ��С�"));
		int ret = MessageBox(GetForegroundWindow(), info, _T("��Ϣ��ʾ"), MB_OKCANCEL);
		if(ret == 1) CFilePathOperater::ClearFolder(path, false);
		else return false;
	}
	createProjectDirEx(type); updataProjectInfo(type); CString dwgPath;
	if(type.GetLength() == 0) dwgPath.Format(_T("%s"), _T("GC\\��Ŀͼ��.dwg"));
	else dwgPath.Format(_T("%s\\%s"), type, _T("GC\\��Ŀͼ��.dwg"));
	updataDWG(dwgPath);
	return true;
}

bool IProjectInfo::CreateProject(const MStr &record, CString &strPath)
{
	m_record = const_cast<MStr &>(record);
	m_name = m_record[_T("��Ŀ����")]; m_code = m_record[_T("�����")];
	if(_taccess(m_dir, 0) == -1) _tmkdir(m_dir);
	CStringArray files, paths;
	strPath.Format(_T("%s%s_%s"), m_dir, m_code, m_name);
	CFilePathOperater::GetChildDirectory(m_dir, paths, files);
	for(int idx = 0; idx < files.GetSize(); ++idx)
	{
		CString file = files.GetAt(idx);
		int pos = file.Find(_T("_"));
		file = file.Mid(0, pos);
		if(file.Find(m_code) == -1) continue;
		CString info(_T("������Ѵ��ڣ���������д����ţ���Ҫ����ʹ�ø�����ţ�����ɾ������Ŀ¼�¸���Ŀ�ļ��С�"));
		int ret = MessageBox(GetForegroundWindow(), info, _T("��Ϣ��ʾ"), MB_OKCANCEL);
		if(ret == 1) CFilePathOperater::ClearFolder(strPath, false);
		else return false;
	}
	createProjectDir(); updataProjectInfo(); updataDWG();
	return true;
}

bool IProjectInfo::CreateProjectEx(const MStr &record, CString &strPath, const CString &type)
{
	m_record = const_cast<MStr &>(record);
	m_name = m_record[_T("��Ŀ����")]; m_code = m_record[_T("�����")];
	if(_taccess(m_dir, 0) == -1) _tmkdir(m_dir);
	CStringArray files, paths;
	strPath.Format(_T("%s%s_%s"), m_dir, m_code, m_name);
	CFilePathOperater::GetChildDirectory(m_dir, paths, files);
	for(int idx = 0; idx < files.GetSize(); ++idx)
	{
		CString file = files.GetAt(idx);
		int pos = file.Find(_T("_"));
		file = file.Mid(0, pos);
		if(file.Find(m_code) == -1) continue;
		CString info(_T("������Ѵ��ڣ���������д����ţ���Ҫ����ʹ�ø�����ţ�����ɾ������Ŀ¼�¸���Ŀ�ļ��С�"));
		int ret = MessageBox(GetForegroundWindow(), info, _T("��Ϣ��ʾ"), MB_OKCANCEL);
		if(ret == 1) CFilePathOperater::ClearFolder(strPath, false);
		else return false;
	}
	createProjectDirEx(type); updataProjectInfo(type);  CString dwgPath;
	if(type.GetLength() == 0) dwgPath.Format(_T("%s"), _T("GC\\��Ŀͼ��.dwg"));
	else dwgPath.Format(_T("%s\\%s"), type, _T("GC\\��Ŀͼ��.dwg"));
	updataDWG(dwgPath);
	return true;
}

void IProjectInfo::createProjectDir(CString name)
{
	CString path;
	path.Format(_T("%s%s_%s"), m_dir, m_code, m_name);
	_tmkdir(path); TCHAR pdb[255] = {0};
	_tmkdir(path + _T("\\CG"));
	_tmkdir(path + _T("\\CK"));
	_tmkdir(path + _T("\\GC"));
	ads_findfile(_T("project.mdb"), pdb);
	CopyFile(pdb, path + _T("\\GC\\��Ŀ��Ϣ.mdb"), TRUE);
	ads_findfile(_T("��Ŀͼ��.dwg"), pdb);
	CopyFile(pdb, path + _T("\\") + name, TRUE);
	if(m_record.size() == 0) return;
	CDatabaseOperManager oper;
	oper.initialConnect(path + _T("\\GC\\��Ŀ��Ϣ.mdb"));
	MSIter it = m_record.begin();
	for(; it != m_record.end(); ++it)
	{
		MStr record;
		record[_T("�ؼ���")] = it->first;
		record[_T("����ֵ")] = it->second;
		oper.InsertRecord(_T("JBXX"), record);
	}
	oper.UpdataRecord(_T("XMJGProjectInfo"), MStr(), m_record);
}

void IProjectInfo::createProjectDirEx(const CString &type, CString name)
{
	CString path;
	path.Format(_T("%s%s_%s\\%s"), m_dir, m_code, m_name, type);
	isExistFolder(path); TCHAR pdb[255] = {0};
	_tmkdir(path + _T("\\CG"));
	_tmkdir(path + _T("\\CK"));
	_tmkdir(path + _T("\\GC"));
	if(5100 != ads_findfile(_T("project.mdb"), pdb))
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ���Ŀmdb�ļ�"), _T("������Ϣ"), 0);
		return;
	}
	CopyFile(pdb, path + _T("\\GC\\��Ŀ��Ϣ.mdb"), TRUE);
	ads_findfile(_T("��Ŀͼ��.dwg"), pdb);
	CopyFile(pdb, path + _T("\\") + name, TRUE);
	if(m_record.size() == 0) return;
	CDatabaseOperManager oper;
	oper.initialConnect(path + _T("\\GC\\��Ŀ��Ϣ.mdb"));
	MSIter it = m_record.begin();
	for(; it != m_record.end(); ++it)
	{
		MStr record;
		record[_T("�ؼ���")] = it->first;
		record[_T("����ֵ")] = it->second;
		oper.InsertRecord(_T("JBXX"), record);
	}
	oper.UpdataRecord(_T("XMJGProjectInfo"), MStr(), m_record);
}

void IProjectInfo::updataProjectInfo(CString type)
{
	TCHAR mdbPath[255] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), mdbPath)) return;
	CDatabaseOperManager oper;
	if(false == oper.initialConnect(mdbPath)) return;
	CString path; MStr record, filter;
	if(type.GetLength())
		path.Format(_T("%s%s_%s\\%s"), m_dir, m_code, m_name, type);
	else
		path.Format(_T("%s%s_%s"), m_dir, m_code, m_name);
	filter[_T("�ؼ���")] = _T("��ǰĿ¼");
	record[_T("����ֵ")] = path;
	oper.UpdataRecord(_T("XMJGPath"), filter, record);
}

void IProjectInfo::updataDWG(CString name)
{
	closeDWG(_T("��Ŀͼ��")); CString path;
	path.Format(_T("%s%s_%s\\%s"), m_dir, m_code, m_name, name);
	SendCommand(_T("OpenDwgCmd "));
	SendCommand(path);
	SendCommand(_T(" "));
}

#pragma region ҵ��һ ���߷���

XMJGRedLineSettingOut::XMJGRedLineSettingOut()
{
	m_Scale = 500;
	read1(m_Scale);
	m_Scale /= 1000;
	IDataBaseOper db;
	db.readCommonTable(_T("Ұ����ע�Ǵ�С"), m_ywcd._size);
	db.readCommonTable(_T("Ұ����ע����ɫ"), m_ywcd._color);
	db.readCommonTable(_T("Ұ����ע��ͼ��"), m_ywcd._layer);
	db.readCommonTable(_T("���߽ڵ�ע�Ǵ�С"), m_hxjdzj._size);
	db.readCommonTable(_T("���߽ڵ�ע����ɫ"), m_hxjdzj._color);
	db.readCommonTable(_T("���߽ڵ�ע��ͼ��"), m_hxjdzj._layer);
	db.readCommonTable(_T("Ұ����ͼ����ɫ"), m_ywcdtk._color);
	db.readCommonTable(_T("Ұ����ͼ��ͼ��"), m_ywcdtk._layer);
}

XMJGRedLineSettingOut::~XMJGRedLineSettingOut()
{

}

void XMJGRedLineSettingOut::ImportData()
{
	CString pathName; XMJGAssist assist;
	CString filter(_T("�����ļ�(*.*) | *.*; | TXT(*.txt) | *.txt | DAT(*.dat) | *.dat | Excel(*.csv; *.xls; *.xlsx;) | CRD(*.crd) | *.csv; *xls; *xlsx ||"));
	if(!assist.SelectFile(filter, m_SaveDir, pathName)) return;
	VPInfo ptinfo; VStr error; double scale = 500;
	TCHAR reverse[20] = {0}; bool flag = false; read1(scale);
	if(RTCAN == ads_getstring(0, _T("\n �Ƿ�X��Y�Ի�(N/Y)��<N>"), reverse)) return;
	if(_tcscmp(reverse, _T("Y")) == 0 || _tcscmp(reverse, _T("y")) == 0) flag = true;
	if(pathName.Find(_T(".txt")) != -1 || pathName.Find(_T(".TXT")) != -1)
		purgeTxtDataFile(pathName, ptinfo, error, flag);
	else if(pathName.Find(_T(".dat")) != -1 || pathName.Find(_T(".DAT")) != -1)
		purgeDatDataFile(pathName, ptinfo, error, flag);
	else if(pathName.Find(_T(".crd")) != -1 || pathName.Find(_T(".CRD")) != -1)
		purgeTxtDataFile(pathName, ptinfo, error, flag);
	else
		purgeExcelDataFile(pathName, ptinfo, error, flag);
	drawPoint(ptinfo);
}

void XMJGRedLineSettingOut::setProperty()
{
	SelectFilter sf(RTDXF0, _T("*polyline")); AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T(""))) return;
	setlayer(aname(id), _T("����"));
	setcolor(aname(id), 1);
}

void XMJGRedLineSettingOut::CreateRLProject()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo1 dlg(true); dlg.DoModal();
}

void XMJGRedLineSettingOut::ReadDatFile()
{
	CString pathName; XMJGAssist assist;
	CString filter(_T("�����ļ�(*.txt; *.dat; *.csv; *.xls; *.xlsx;) | *.txt; *.dat; *.csv; *xls; *xlsx ||"));
	if(!assist.SelectFile(filter, m_SaveDir, pathName)) return;
	VPInfo ptinfo; VStr error; double scale = 500;
	TCHAR reverse[20] = {0}; bool flag = 0; read1(scale);
	if(RTCAN == ads_getstring(0, _T("\n �Ƿ�X��Y�Ի�(N/Y)��"), reverse)) return;
	if(pathName.Find(_T(".dat")) == -1 || pathName.Find(_T(".DAT")) == -1)
		assist.PurgeExcelFile(pathName, ptinfo, error, reverse == 0 ? false : true);
	else
		assist.PurgeDatFile(pathName, ptinfo, error, reverse == 0 ? false : true);
	assist.DrawPoint(ptinfo);	assist.PrintErrorInfo(error);
}

void XMJGRedLineSettingOut::ReadReveseDat()
{
	CString pathName; XMJGAssist assist;
	CString filter(_T("*.dat | *.dat"));
	if(!assist.SelectFile(filter, m_SaveDir, pathName)) return;
	VPInfo ptinfo; VStr error; double scale = 500;
	read1(scale); assist.PurgeDatFile(pathName, ptinfo, error, true);
	assist.DrawPoint(ptinfo);	assist.PrintErrorInfo(error);
}

void XMJGRedLineSettingOut::ReadExcelFile()
{
	CString pathName; XMJGAssist assist;
	CString filter(_T("Excel�ļ�(*.csv; *xls; *xlsx) | *.csv; *xls; *xlsx"));
	if(!assist.SelectFile(filter, m_SaveDir, pathName)) return;
	VPInfo ptinfo; VStr error; double scale = 500; read1(scale);
	int type = 0, reverse = 0;
	if(RTCAN == ads_getint(_T("[1]�����ݡ�[2]ȫվ�ǡ�[3]RTK��"), &type)) return;
	if(RTCAN == ads_getint(_T("�Ƿ񽻻�XYֵ[0]��������[1]������"), &reverse)) return;
	switch(type)
	{
		case 0:	assist.PurgeExcelFile(pathName, ptinfo, error, reverse == 0 ? false : true); break;
		case 1: assist.PurgeExcelQZY(pathName, ptinfo, error, reverse == 0 ? false : true); break;
		case 2: assist.PurgeExcelRTK(pathName, ptinfo, error, reverse == 0 ? false : true); break;
		default: acutPrintf(_T("\n ������������ʹ�������")); return;
	}
	assist.DrawPoint(ptinfo); assist.PrintErrorInfo(error);
}

void XMJGRedLineSettingOut::ManualDrawPoint()
{
	TCHAR name[255] = {0}; AcGePoint2d temp; AcGePoint3d pt;
	while(true)
	{
		if(false == getInputPoint(temp)) return;
		pt = apoint(temp);
		if(RTNORM != ads_getreal(_T("\n �����������ĸ߳�"), &pt.z)) return;
		if(RTNORM != ads_getstring(0, _T("\n �����������ĵ��"), name)) return;
		CPointInfo info(name, apoint(pt)); XMJGAssist assist;
		assist.DrawPoint(info);
	}
}

void XMJGRedLineSettingOut::DrawRedLine()
{
	AcDbObjectId id = drawLine();
	if(id == AcDbObjectId::kNull) return;
	setlayer(aname(id), _T("����"));
	setcolor(aname(id), 1);
	XMJGAssist assist; bool flag = false;
	TCHAR dh[255] = {0};
	if(RTCAN == ads_getstring(0, _T("\n ��������ǰ׺��"), dh)) return;
	if(GetPolyType(aname(id), true) >= 10) flag = true;
	assist.AutoCreateCode(id, flag, dh);
}

void XMJGRedLineSettingOut::AddRedLineNode()
{
	AcDbObjectId redId;
	acutPrintf(_T("\n ��ѡ����Ҫ��ӽڵ��ʵ��"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, _T("����"));
	if(!SelectEntity(redId, f1, f2, _T(""))) return;
	AcGePoint2d pt;
	if(false == getInputPoint(pt)) return;
	AcGePoint2dArray redArr; int index = -1;
	GetPlList(aname(redId), redArr);
	getNearestEdge(apoint(pt), redArr, index);
	redArr.insertAt(index + 1, apoint(pt));
	SetPolyCoord(aname(redId), redArr);
	XMJGAssist assist;
	assist.AutoCreateCode(redId);
}

void XMJGRedLineSettingOut::DelRedLineNode()
{
	acutPrintf(_T("\n ��ѡ����Ҫɾ���ڵ�ĺ���ʵ�壺"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, _T("����"));
	AcDbObjectIdArray idArr;
	if(!SelectEntitys(idArr, f1, f2, _T(""))) return;
	int index = -1; XMJGAssist assist;
	if(RTNORM != ads_getint(_T("\n ��������Ҫɾ���Ľں�:"), &index)) return;
	for(int idx = 0; idx < idArr.length(); ++idx)
	{
		AcDbObjectId id = idArr.at(idx);
		AcGePoint2dArray nodes;
		GetPlList(aname(id), nodes);
		AcGePoint2d pt = nodes.at(index - 1);
		nodes.remove(pt);
		SetPolyCoord(aname(id), nodes);
		assist.AutoCreateCode(id);
	}
}

void XMJGRedLineSettingOut::DelRedLineNodeEx()
{
	acutPrintf(_T("\n ��ѡ����Ҫɾ���ڵ�ĺ���ʵ�壺"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, _T("����"));
	AcDbObjectIdArray redArr;
	if(!SelectEntitys(redArr, f1, f2, _T(""))) return;
	acutPrintf(_T("\n ��ѡ����Ҫɾ���ڵ�ʵ�壺"));
	SelectFilter f3(RTDXF0, _T("TEXT")), f4(8, _T("ZJ"));
	AcDbObjectIdArray delArr; XMJGAssist assist;
	if(!SelectEntitys(delArr, f3, f4, _T(""))) return;
	for(int idx = 0; idx < redArr.length(); ++idx)
	{
		AcDbObjectId id = redArr.at(idx);
		AcGePoint2dArray nodes;
		GetPlList(aname(id), nodes);
		assist.RemoveNodeFromID(nodes, delArr, false);
		SetPolyCoord(aname(id), nodes);
	}
}

void XMJGRedLineSettingOut::AdjustNodesOrder()
{
	int order = 1, index = 1;
	if(RTCAN == acedGetInt(_T("\n �޸�Ϊ(1)˳ʱ��,(2)��ʱ��:<1> "), &order)) return;
	if(RTCAN == acedGetInt(_T("\n ��������ʼ���: <1>: "), &index)) return;
	order = order == 2 ? 0 : 1;
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, _T("����"));
	AcDbObjectIdArray idArr; XMJGAssist assist;
	if(!SelectEntitys(idArr, f1, f2, _T(""))) return;
	int len = idArr.length();
	acedSetStatusBarProgressMeter(_T("���ڵ����ڵ�˳��..."), 0, len);
	for(int idx = 0; idx < len; ++idx)
	{
		if(GetPolyType(aname(idArr.at(idx)), true) < 10) continue;
		acedSetStatusBarProgressMeterPos(idx);
		assist.AdjustNodeOrder(idArr.at(idx), order, index);
		assist.AutoCreateCode(idArr.at(idx), false);
	}
	acedRestoreStatusBar();
	acutPrintf(_T("\n �ڵ�˳��������"));
}

void XMJGRedLineSettingOut::AddAnnotation()
{
	acutPrintf(_T("\n ��ѡ����Ҫע�ǵĺ���ʵ�壺"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, _T("����"));
	AcDbObjectIdArray idArr;
	if(!SelectEntitys(idArr, f1, f2, _T(""))) return;
	int len = idArr.length(); CDrawArrow arrow;
	for(int idx = 0; idx < len; ++idx)
		arrow.DrawArrow(idArr.at(idx));
}

void XMJGRedLineSettingOut::ManualAnnotation()
{
	IDataBaseOper oper; TextProperty prop;
	CXRecordOperate record; double scale = 1.0; AcDbObjectIdArray idArr;
	if(false == record.ReadDoubleRecord(_T("GCCL"), _T("BLC"), scale))
		prop = oper.readTextPropery(_T("���߽ڵ�ע��"), true);
	else prop._size = (float)(scale * TEXTSIZE);
	singleNodeAnnotation(prop, scale);
}

void XMJGRedLineSettingOut::AutoAnnotation()
{
	acutPrintf(_T("\n ��ѡ����Ҫע�ǵ�ʵ�壺"));
	SelectFilter f1(RTDXF0, _T("*polyline"));
	IDataBaseOper oper; TextProperty prop;
	CXRecordOperate record; double scale = 1.0; AcDbObjectIdArray idArr;
	if(false == record.ReadDoubleRecord(_T("GCCL"), _T("BLC"), scale))
		prop = oper.readTextPropery(_T("���߽ڵ�ע��"), true);
	else prop._size = (float)(scale * TEXTSIZE);
	if(!SelectEntitys(idArr, f1, _T(""))) return;
	mulitNodeAnnotation(idArr, prop);
}

void XMJGRedLineSettingOut::autoAnnotation(AcDbObjectId &id, double length)
{
	XMJGAssist assist; CDrawArrow arrow; AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes); bool bDeasil = IsDeasil(nodes);
	int len = nodes.length(); ads_point spt, mpt;
	IDataBaseOper oper; TextProperty prop;
	CString ppath = oper.readPathTable("��ǰĿ¼");
	if(ppath.Find(_T("���ݿ���")) != -1)
		prop = oper.readTextPropery(_T("�����������߽ڵ�ע��"), true);
	else prop = oper.readTextPropery(_T("���߽ڵ�ע��"), true);
	for(int idx = 0; idx < len; ++idx)
	{
		spt[X] = nodes.at(idx).x; spt[Y] = nodes.at(idx).y;
		double angle = assist.GetOutCornerAndPosition(nodes, idx, bDeasil);
		ads_polar(spt, angle, length, mpt);
		AcDbObjectId ret = arrow.DrawLeader(spt, mpt, prop._size, prop._layer, prop._color);
		ADDMulXData(id, _T("��ע���"), GetHandleByObject(ret), STRING_TYPE, idx);
	}
}

bool XMJGRedLineSettingOut::getInputPoint(AcGePoint2d &pt)
{
	int type = 0;
	if(RTCAN == ads_getint(_T("\n �ֶ����[0]/��Ļʰȡ[1]<0> "), &type)) return false;
	if(type == 0)
		return getInputManual(pt);
	return getInputScreen(pt);
}

bool XMJGRedLineSettingOut::getInputManual(AcGePoint2d &pt1)
{
	TCHAR info[200] = {0};
	if(RTNORM != ads_getstring(0, _T("\n �����뼰�ڵ�����:"), info)) return false;
	CString coor(info);
	int pos = coor.Find(_T(','));
	pos = pos == -1 ? coor.Find(_T('��')) : pos;
	if(pos == -1)
	{
		MessageBox(GetForegroundWindow(), _T("����ڵ������ʽ���󣡣���"), _T("������ʾ"), MB_OK);
		return false;
	}
	pt1.x = _tstof(coor.Left(pos));
	pt1.y = _tstof(coor.Mid(pos + 1));
	return true;
}

bool XMJGRedLineSettingOut::getInputScreen(AcGePoint2d &pt)
{
	ads_point pt1; AcDbObjectIdArray ids;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ����ӵĽڵ�"), pt1)) return false;
	pt.x = pt1[X]; pt.y = pt1[Y]; return true;
}

void XMJGRedLineSettingOut::purgeTxtDataFile(const CString &dataFile, VPInfo &pinfo, VStr &errinfo, bool reverse)
{
	CStdioFile file; CString line; int type = 0;
	if(!file.Open(dataFile, CFile::modeRead)) return;
	while(file.ReadString(line))
	{
		VStr info, vline;
		AnalyzeData(vline, line, _T(" "));
		for(int idx = 0; idx < (int)vline.size(); ++idx)
		{
			CString temp = vline[idx];
			if(temp.CompareNoCase(_T("")) == 0) continue;
			if(temp.CompareNoCase(_T(" ")) == 0) continue;
			info.push_back(temp);
		}
		if(info.size() != 4)
		{
			errinfo.push_back(line);
			continue;
		}
		CPointInfo pt(info);
		if(true == reverse)
		{
			double temp = pt.m_pt3.x;
			pt.m_pt3.x = pt.m_pt3.y;
			pt.m_pt3.y = temp;
		}
		pinfo.push_back(pt);
	}
}

void XMJGRedLineSettingOut::purgeDatDataFile(const CString &dataFile, VPInfo &pinfo, VStr &errinfo, bool reverse)
{
	CStdioFile file; CString line; int type = 0;
	if(!file.Open(dataFile, CFile::modeRead)) return;
	while(file.ReadString(line))
	{
		VStr info, vline;
		AnalyzeData(vline, line, _T(" "));
		for(int idx = 0; idx < (int)vline.size(); ++idx)
		{
			CString temp = vline[idx];
			if(temp.CompareNoCase(_T("")) == 0) continue;
			if(temp.CompareNoCase(_T(" ")) == 0) continue;
			info.push_back(temp);
		}
		if(info.size() != 4)
		{
			errinfo.push_back(line);
			continue;
		}
		CPointInfo pt(info);
		if(true == reverse)
		{
			double temp = pt.m_pt3.x;
			pt.m_pt3.x = pt.m_pt3.y;
			pt.m_pt3.y = temp;
		}
		pinfo.push_back(pt);
	}
}

void XMJGRedLineSettingOut::purgeExcelDataFile(const CString &dataFile, VPInfo &pinfo, VStr &errinfo, bool reverse)
{
	CTinyExcel excel; vector<_Range> vRange;
	excel.SetTemplete(dataFile);
	excel.StartExcel();
	int i = 0, j = 0, type = 0;
	long nItemCount = 0;
	long nSubItemCount = 0;
	excel.GetWorksheet(excel.m_worksheets, excel.m_worksheet, 1);
	excel.m_range.AttachDispatch(excel.m_worksheet.GetUsedRange());
	excel.GetRowColCount(nItemCount, nSubItemCount);
	acedSetStatusBarProgressMeter(_T("���ڶ�ȡ���ݡ�����"), 1, nItemCount);
	for(int row = 1; row <= nItemCount; ++row)
	{
		acedSetStatusBarProgressMeterPos(row);
		CString line, cell; VStr info;
		cell = excel.GetRangeContent(row, 1, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 3, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 4, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 5, excel.m_range); line.Append(cell);
		AnalyzeData(info, line, _T(","));
		if(info.size() != 4)
		{
			errinfo.push_back(line);
			continue;
		}
		CPointInfo pt(info);
		if(reverse == true)
		{
			double temp = pt.m_pt3.x;
			pt.m_pt3.x = pt.m_pt3.y;
			pt.m_pt3.y = temp;
		}
		pinfo.push_back(pt);
	}
	acedRestoreStatusBar();
}

void XMJGRedLineSettingOut::drawPoint(const VPInfo &ptinfo)
{
	int len = (int)ptinfo.size();
	acedSetStatusBarProgressMeter(_T("���ڻ��Ƶ㡣����"), 0, len);
	for(int idx = 0; idx < len; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		CPointInfo info = ptinfo.at(idx);
		AcDbObjectId id = DrawInsert(apoint(info.m_pt3), _T("Point"));
		AddXdata(aname(id), _T("���"), STRING_TYPE, info.m_Name);
		TCHAR szContent[50]; _stprintf(szContent, _T("%f"), info.m_pt3.x);
		AddXdata(aname(id), _T("X����"), DOUBLE_TYPE, szContent);
		_stprintf(szContent, _T("%f"), info.m_pt3.y);
		AddXdata(aname(id), _T("Y����"), DOUBLE_TYPE, szContent);
		_stprintf(szContent, _T("%f"), info.m_pt3.z);
		AddXdata(aname(id), _T("�߳�"), DOUBLE_TYPE, szContent);
		setlayer(aname(id), m_ywcdtk._layer);
		setcolor(aname(id), m_ywcdtk._color);
		AcGePoint3d pt(info.m_pt3);
		pt.x += 0.25 * m_Scale;
		pt.y += 0.375 * m_Scale;
		AcDbObjectId codeid = DrawText(pt, info.m_Name, _T("ML"), _T(""), m_ywcd._size * m_Scale);
		setlayer(aname(codeid), m_ywcd._layer); setcolor(aname(codeid), m_ywcd._color);
	}
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	acedRestoreStatusBar();
}

void XMJGRedLineSettingOut::delAnnotation(AcDbObjectId &id)
{
	int count = GetXDataCount(aname(id), _T("��ע���"));
	TCHAR handle[20] = {0};
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(id, _T("��ע���"), handle, STRING_TYPE, idx);
		AcDbObjectId ent = GetObjectByHandle(handle);
		EntDel(aname(ent));
	}
}

void XMJGRedLineSettingOut::MoveAnnotation()
{
	acutPrintf(_T("\n ��ѡ����Ҫ�ƶ���ע�ǣ�"));
	SelectFilter f(8, _T("��ע"));
	AcDbObjectId id;
	int oldpick = GroupReset();
	if(!SelectEntity(id, f, _T(""))) return;
	GroupBack(oldpick);
	CDrawArrow arrow;
	AcDbObjectId arrowid = arrow.GetArrowObjectID(id);
	if(arrowid == AcDbObjectId::kNull) return;
	TCHAR coor[200] = {0}; AcGePoint2d spos;
	READMulXData(arrowid, _T("����"), coor, 0, 0);
	spos.y = _tstof(coor + 2);
	READMulXData(arrowid, _T("����"), coor, 0, 1);
	spos.x = _tstof(coor + 2);
	ReadXdata(aname(arrowid), _T("ע�ǳ���"), 3, coor);
	double length = _tstof(coor);
	ads_point to_pt; EntDel(aname(arrowid));
	if(RTCAN == ads_getpoint(NULL, _T("\n ��ָ���ƶ�����"), to_pt)) return;
	arrow.DrawArrow(spos, apoint(to_pt), length);
}

void XMJGRedLineSettingOut::ManualLineAnnotation()
{
	ads_point spt, ept, mpt; XMJGAssist assist;
	assist.SetAnnotationType();
	TCHAR annotationtype[200] = {0};
	assist.GetAnnotationType(annotationtype);
	IDataBaseOper oper;
	TextProperty prop = oper.readTextPropertyTable(_T("���������ע����"));
	TextProperty prop2 = oper.readTextPropertyTable(_T("���������ע����"));
	while(true)
	{
		if(RTNORM != ads_getpoint(NULL, _T("\n �������һ����"), spt)) return;
		if(RTNORM != ads_getpoint(spt, _T("\n ������ڶ�����"), ept)) return;
		mpt[X] = (spt[X] + ept[X]) / 2; mpt[Y] = (spt[Y] + ept[Y]) / 2;
		double dist1 = distOF2d(apoint(spt), apoint(ept));
		TCHAR _dist[255] = {0}; _stprintf(_dist, _T("%.2lf"), dist1);
		int old_osmode = PickstyleReset(0);
		ads_command(RTSTR, _T("dimaligned"), RTPOINT, spt, RTPOINT, ept, RTSTR, _T("T"), RTSTR, _dist, RTPOINT, mpt, RTNONE);
		PickstyleBack(old_osmode);
		acedCommand(RTSTR, _T("Regen"), RTNONE);
		ads_name ent;  ads_entlast(ent);
		SetTextStyle(ent, prop2._font);//����ע����������
		AcDbObjectId id; acdbGetObjectId(id, ent);
		AcDbEntity *pEnt = NULL;
		if(Acad::eOk != acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite)) continue;
		if(pEnt->isKindOf(AcDbDimension::desc()))
		{
			AcDbDimension *pDim = AcDbDimension::cast(pEnt);
			pDim->setDimtxt(prop2._size);//����ע�����ִ�С
			AcCmColor color; color.setColorIndex(prop._color);
			pDim->setDimclrd(color);
			pDim->setDimclrt(color);
		}
		pEnt->close();
		AddXdata(ent, _T("����"), 0, _dist);
		AddXdata(ent, _T("��ע����"), 0, annotationtype);
	}
}

void XMJGRedLineSettingOut::DrawScopeLine()
{

}

void XMJGRedLineSettingOut::RedlineSetAttatchedInfo()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo1 dlg(false);
	if(dlg.DoModal() != IDOK) return;
	int index = dlg.getEntityColor();
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, _T("����"));
	AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, f1, f2, _T("X"))) return;
	for(int idx = 0; idx < ids.length(); ++idx)
		setcolor(aname(ids.at(idx)), index);
}

void XMJGRedLineSettingOut::ReadRegisterValue(SBreakBlock *block)
{
	HKEY rKey; TCHAR value[1024] = {0};
	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG\\Project1"), 0, KEY_ALL_ACCESS, &rKey) == ERROR_SUCCESS)
	{
		CassQueryReg(rKey, _T("AcceptCode"), value); block->replaceTextEx(_T("#ACCEPTCODE#"), value);
		CassQueryReg(rKey, _T("EntrustCompany"), value); block->replaceTextEx(_T("#ENTRUSTCOMPANY#"), value);
		CassQueryReg(rKey, _T("BuildProject"), value); block->replaceTextEx(_T("#BUILDPROJECT#"), value);
		CassQueryReg(rKey, _T("MeansureContext"), value); block->replaceTextEx(_T("#MEASURECONTEXT#"), value);
		CassQueryReg(rKey, _T("TelePhone"), value);  block->replaceTextEx(_T("#TELEPHONE#"), value);
	}
	else
		return;
}

void XMJGRedLineSettingOut::GetReplaceValue(MStr &field)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	field[_T("#ACCEPTCODE#")] = record[_T("�����")];
	field[_T("#ENTRUSTCOMPANY#")] = record[_T("ί�е�λ")];
	field[_T("#BUILDPROJECT#")] = record[_T("��Ŀ����")];
	field[_T("#MEASURECONTEXT#")] = record[_T("��������")];
	field[_T("#TELEPHONE#")] = record[_T("��ϵ�绰")];
	field[_T("#CHECKCODE#")] = record[_T("���ֺ�")];
	return;
}

void XMJGRedLineSettingOut::RedLineExportDWG()
{
	CExportGCCL gccl;
	MStr field;  GetReplaceValue(field);
	gccl.beginExport(_T("���߷���"));
	gccl.replaceText(field);
	return;
	XMJGAssist assist; XMJGExport exportdwg;
	CString filepath = assist.getExportFilePath(true);
	CString filename = assist.getExportFileName(_T("���߷���"));
	filepath += _T("\\CG\\") + filename;
	if(filepath.GetLength() == 0) return;
	exportdwg.setSavePath(filepath);
	exportdwg.setPictureFrame(_T("ҵ��һͼ��"));
	exportdwg.isDrawRectangle(false);
	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
		openDwgCommand(filepath);

	// 	XMJGAssist assist; XMJGExport exportdwg;
	// 	CString filepath = assist.getExportFilePath(true);
	// 	CString filename = assist.getExportFileName(_T("���߷���"));
	// 	filepath += _T("\\CG\\") + filename;
	// 	if(filepath.GetLength() == 0) return;
	// 	exportdwg.setSavePath(filepath);
	// 	exportdwg.setPictureFrame(_T("ҵ��һͼ��"));
	// 	exportdwg.isDrawRectangle(false);
	// 	MStr field;  GetReplaceValue(field);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(filepath);	
}

void XMJGRedLineSettingOut::InputCoorPos()
{
	TCHAR coor[200] = {0};
	CString Coor; CStringArray info;
	while(true)
	{
		if(RTCAN == ads_getstring(0, _T("\n ����������:"), coor)) return;
		Coor.Format(_T("%s"), coor);
		AnalyzeData(info, coor, _T(","));
		if(info.GetSize() != 3) continue;
		if(RTCAN == ads_getstring(0, _T("\n ��������:"), coor)) return;
		double x = _tstof(info.GetAt(0));
		double y = _tstof(info.GetAt(1));
		double z = _tstof(info.GetAt(2));
		CPointInfo point(coor, x, y, z);
		XMJGAssist assist;
		assist.DrawPoint(point);
	}
}

void XMJGRedLineSettingOut::readLineAddNode()
{
	SelectFilter sf(8, _T("����"));
	AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T(""))) return;
	XMJGAssist assist; bool flag = false;
	TCHAR dh[255] = {0};
	if(RTCAN == ads_getstring(1, _T("\n ��������ǰ׺��"), dh)) return;
	if(GetPolyType(aname(id), true) >= 10) flag = true;
	assist.AutoCreateCode(id, flag, dh);
}

void XMJGRedLineSettingOut::redDistAnnotion()
{
	IDataBaseOper oper; ads_point spt, ept, mpt;
	TextProperty prop1 = oper.readTextPropertyTable(_T("���߾����ע����"));
	TextProperty prop2 = oper.readTextPropertyTable(_T("���߾����ע����"));
	while(true)
	{
		if(RTNORM != ads_getpoint(NULL, _T("\n �������һ����"), spt)) return;
		if(RTNORM != ads_getpoint(spt, _T("\n ������ڶ�����"), ept)) return;
		mpt[X] = (spt[X] + ept[X]) / 2; mpt[Y] = (spt[Y] + ept[Y]) / 2;
		AcDbAlignedDimension *dim = new AcDbAlignedDimension(apoint(spt), apoint(ept), apoint(mpt));
		dim->setDimtxt(prop2._size); AcDbObjectId id;
		AcCmColor color; color.setColorIndex(prop2._color);
		dim->setDimclrd(color); dim->setDimclrt(color);
		dim->setDimsah(true); dim->setDimtih(false);
		dim->setDimsd1(false); dim->setDimsd2(false);
		dim->setDimse1(true); dim->setDimse2(true);
		dim->setDimdec(2); dim->setDimtih(false);
		dim->setDimtad(0); dim->setLayer(prop1._layer);
		if(!AppendEntity(dim, id))
		{
			delete dim; return;
		}
		dim->close();	setlayer(aname(id), prop2._layer);
		SetTextStyle(id, prop2._font); return;
	}
}

#pragma endregion

#pragma region ҵ��� ���������

XMJGBuildSettingOut::XMJGBuildSettingOut()
{
	m_Scale = 500; read1(m_Scale); m_Scale /= 1000;
	IDataBaseOper oper;
	oper.readCommonTable(_T("����������ͼ��"), m_jzwlk._layer);
	oper.readCommonTable(_T("������������ɫ"), m_jzwlk._color);
	oper.readCommonTable(_T("����������ͼ��"), m_jzwmc._layer);
	oper.readCommonTable(_T("������������ɫ"), m_jzwmc._color);
	oper.readCommonTable(_T("���������ƴ�С"), m_jzwmc._size);
}

XMJGBuildSettingOut::~XMJGBuildSettingOut()
{

}

void XMJGBuildSettingOut::CreateBOSProject()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo2::destory();
	XMJGSetInfo2 *info = XMJGSetInfo2::Instance(true);
	if(TRUE == info->IsWindowVisible())
		return;
	info->ShowWindow(SW_SHOW);
}

bool openDesignDWGFile(const CString &path, AcDbDatabase *pExternalDb, AcDbObjectIdArray &ids, VStr &layers)
{
	if(path.IsEmpty()) return false;
	pExternalDb->readDwgFile(path);
	AcDbBlockTable* pBlockTable;
	pExternalDb->getBlockTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForRead);
	AcDbBlockTableRecordIterator* pItr;
	pBlockTableRecord->newIterator(pItr);
	int nTmp = 0, i = 0; TCHAR layer[20] = {0};
	for(pItr->start(); !pItr->done(); pItr->step())
	{
		AcDbEntity* pEnt;
		pItr->getEntity(pEnt, AcDb::kForRead);
		GetEntLayer(pEnt->objectId(), layer);
		if(layers.end() == find(layers.begin(), layers.end(), layer))
			layers.push_back(layer);
		ids.append(pEnt->objectId());
		pEnt->close();
	}
	delete pItr;
	pBlockTableRecord->close();
	pBlockTable->close();
	return true;
}

bool filterDesignEntity(const VStr &layers, AcDbObjectIdArray &ids, double scale)
{
	acedSetStatusBarProgressMeter(_T("���ڹ���ʵ��..."), 0, (int)ids.length());
	AcDbObjectIdArray tempids;
	tempids.append(ids);
	CString layer; TCHAR info[20] = {0};
	for(int idx = 0; idx < tempids.length(); ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = tempids.at(idx);
		GetEntLayer(id, info);
		bool flag = false;
		for(int indx = 0; indx < (int)layers.size(); ++indx)
		{
			layer = layers.at(indx);
			if(layer.CompareNoCase(info) == 0)
			{
				flag = true;
				break;
			}
		}
		if(flag == false) ids.remove(id);
	}
	ScaleEntitys(ids, AcGePoint3d(0, 0, 0), scale);
	acedRestoreStatusBar();
	return true;
}

void XMJGBuildSettingOut::InsertRedLineDwg()
{
	XMJGAssist assist; CString insertdwg;
	CString path = assist.getExportFilePath(_T("��������"));
	if(!assist.SelectFile(_T("*.dwg | *.dwg"), path, insertdwg))
		acutPrintf(_T("\n ����dwg�ļ�ʧ��"));
	AcDbDatabase* pExternalDb = new AcDbDatabase(false); AcDbObjectIdArray ids;
	VStr layers; if(false == openDesignDWGFile(insertdwg, pExternalDb, ids, layers)) return;
	CFilterLayerDlg dlg(layers);
	if(IDOK != dlg.DoModal()) return;
	if(false == filterDesignEntity(dlg.getLayers(), ids, dlg.getScale())) return;
	int len = ids.length();
	if(len > 0)
	{
		AcDbDatabase *pTempDb;
		pExternalDb->forceWblockDatabaseCopy();
		int es = pExternalDb->wblock(pTempDb, ids, AcGePoint3d::kOrigin);
		if(es != Acad::eOk)
		{
			CString str;
			str.Format(_T("%d"), es);
			AfxMessageBox(str);
			delete pExternalDb;
			return;
		}
		acdbHostApplicationServices()->workingDatabase()->insert(AcGeMatrix3d::kIdentity, pTempDb);
		delete pTempDb;
	}
	delete pExternalDb;
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	return;
	if(0 == InsertDwgFile(insertdwg))
		acutPrintf(_T("\n ����dwg�ļ�ʧ��"));
	else
		acutPrintf(_T("\n �ɹ�����dwg�ļ�"));
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
}

void XMJGBuildSettingOut::AddOutLineSymbol()
{
	AcDbObjectId id = drawLine();
	if(id == AcDbObjectId::kNull) return;
	CSetEntityPropery prop;
	prop.setBuild(id);
}

void XMJGBuildSettingOut::DrawBuildBoundaries()
{
	AcDbObjectId id = drawLine();
	if(id == AcDbObjectId::kNull) return;
	CSetEntityPropery prop;
	prop.setBuildBoundaries(id);
	// 
	// 	struct resbuf *cmd, *result;
	// 	cmd = ads_buildlist(RTSTR, _T("drawdjf"), RTSTR, _T(""), RTNONE);
	// 	ads_invoke(cmd, &result);
	// 	ads_relrb(cmd); ads_name ent;
	// 	ads_entlast(ent);
	// 	setlayer(ent, m_jzwlk._layer);
	// 	setlinetype(ent, _T("SOLID"));
	// 	setcolor(ent, m_jzwlk._color);
	// 	AddZDMJ2BuildEntity(ent);
}

void XMJGBuildSettingOut::ShowOutLineSymbol()
{
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, m_jzwlk._layer);
	AcDbObjectIdArray idArr;
	IDataBaseOper oper;
	if(!SelectEntitys(idArr, f1, f2, _T(""))) return;
	int len = idArr.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = idArr.at(idx);

		//����Ƿ���ע��
		CSetEntityPropery temp;
		AcGePoint3d mid = temp.getRectangleTopLeftPos(id);
		AcGePoint2dArray nodes;
		AcGePoint2d pt1, pt2, pt3, pt4;
		AcDbObjectIdArray ids;
		pt1[X] = mid[X] - 0.5; pt1[Y] = mid[Y] + 0.5; nodes.append(pt1);
		pt2[X] = mid[X] - 0.5; pt2[Y] = mid[Y] - 0.5; nodes.append(pt2);
		pt3[X] = mid[X] + 0.5; pt3[Y] = mid[Y] - 0.5; nodes.append(pt3);
		pt4[X] = mid[X] + 0.5; pt4[Y] = mid[Y] + 0.5; nodes.append(pt4);
		ssFromNodes(ids, nodes);
		if(ids.length() == 1) return;

		TextProperty jzwmc = oper.readTextPropery(_T("����������"), true);
		TCHAR name[255] = {0};
		ReadXdata(aname(id), _T("����������"), 0, name);
		bool bNoXdata = false;
		if(!_tcscmp(name, _T("")))
		{
			if(RTCAN == ads_getstring(0, _T("\n�����뽨��������:"), name)) return;
			AddXdata(aname(id), _T("����������"), 0, name);
			AddXdata(aname(id), _T("¥����"), 0, name);
			bNoXdata = true;
		}

		AcDbObjectId textid = DrawText(mid, name, _T("MC"), _T(""), jzwmc._size * m_Scale);
		setlayer(aname(textid), jzwmc._layer);
		setcolor(aname(textid), jzwmc._color);
		if(bNoXdata)
		{
			CString handle = GetHandleByObject(textid);
			AddXdata(aname(id), _T("JZWMCZJ"), 0, handle);
			CString guid = newGUID3();
			AddXdata(aname(id), _T("GUID"), 0, guid);
			IProjectMDB pdb; pdb.setBGGUID(guid);
		}
	}
}

void XMJGBuildSettingOut::HideOutLineSymbol()
{
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, m_jzwlk._layer);
	AcDbObjectIdArray idArr;
	if(!SelectEntitys(idArr, f1, f2, _T(""))) return;
	int len = idArr.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = idArr.at(idx); TCHAR handle[255] = {0};
		ReadXdata(aname(id), _T("BuildName"), 0, handle);
		AcDbObjectId textid = GetObjectByHandle(handle);
		EntDel(aname(textid));
	}
}

void XMJGBuildSettingOut::SetAnnotationAttribute()
{
	acutPrintf(_T("\n �����ñ�ע"));
	SelectFilter f(8, _T("��ע")); AcDbObjectId id;
	if(!SelectEntity(id, f, _T(""))) return;
	TCHAR bzbh[255] = {0}; TCHAR info[255] = {0}; int azimuth = 1;
	if(RTNORM != ads_getstring(0, _T("\n �������ע�ı��: "), bzbh)) return;
	_stprintf(info, _T("%s"), _T("\n ��ѡ���ע�ķ�λ(<1>����<2>�ϡ�<3>����<4>����<5>���ϡ�<6>������<7>���ϡ�<8>����)��"));
	if(RTNORM != ads_getint(info, &azimuth)) return;
	XMJGAssist assist; assist.SetAzimuthInfo(azimuth, info);
	AddXdata(aname(id), _T("��λ"), 0, info);
	AddXdata(aname(id), _T("���"), 0, bzbh);
}

void XMJGBuildSettingOut::ChangeAzimuthOrder()
{
	IDataBaseOper oper; TCHAR info[255] = {0}; ads_name ent; ads_point pt;
	TextProperty prop = oper.readTextPropertyTable(_T("���������ע����")); int code = 0;
	CXRecordOperate xRecord; xRecord.ReadIntegerRecord(_T("XMJG"), _T("JLBZBH"), code);
	while(1)
	{
		if(RTNORM != ads_entsel(_T("\n��ѡ����Ҫ���õı�ע��"), ent, pt)) break;
		GetEntLayer(ent, info);
		if(prop._layer.CompareNoCase(info) != 0) continue;;
		TCHAR bh[255] = {0}; int index = 1;
		if(false != ReadXdata(ent, _T("���"), 0, bh))
		{
			index = _ttoi(bh);
			_stprintf(bh, _T("\nʵ���ѱ��[%d]���������µı�ע���<%d>��"), index, code + 1);
		}
		else
			_stprintf(bh, _T("\n�������ע���<%d>��"), code + 1);
		int ret = ads_getint(bh, &index);
		if(ret == RTCAN) break;
		else if(ret == RTNONE) index = code + 1;
		_stprintf(bh, _T("%d"), index);
		AddXdata(ent, _T("���"), 0, bh);
		code = index > code ? index : code;
	}
	xRecord.AddIntegerRecord(_T("XMJG"), _T("JLBZBH"), code);
}

void XMJGBuildSettingOut::ChangeAzimuthOrderAll()
{
	acutPrintf(_T("\n ��ѡ������������"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, m_jzwlk._layer);
	AcDbObjectId id; if(!SelectEntity(id, f1, f2, _T(""))) return;
	TCHAR info[255] = {0}; int start = 1, index = 1;
	_stprintf(info, _T("%s"), _T("\n ��������Ҫ��ŵ���ʼλ�ã�"));
	if(RTNORM != ads_getint(info, &start)) return;
	_stprintf(info, _T("%s"), _T("\n �������µ���ʼ��ţ�"));
	if(RTNORM != ads_getint(info, &index)) return;
	AcDbObjectIdArray ids; AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes); ssFromNodes(ids, nodes);
	int len = ids.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId tid = ids.at(idx);
		TCHAR order[255] = {0};
		if(!ReadXdata(aname(tid), _T("���"), 0, order)) continue;
		int indx = _ttoi(order);
		if(indx < start) continue;
		_stprintf(order, _T("%d"), indx + index - start);
		AddXdata(aname(tid), _T("���"), 0, order);
	}
}

void XMJGBuildSettingOut::SetAttatchedInfo()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo2::destory();
	XMJGSetInfo2 *info = XMJGSetInfo2::Instance(false);
	if(TRUE == info->IsWindowVisible())	return;
	info->ShowWindow(SW_SHOW);
}

void XMJGBuildSettingOut::ExportToDWG()
{
	CExportGCCL gccl;
	gccl.beginExport(_T("��������1"));
	MStr field;  GetReplaceValue(field);
	gccl.replaceText(field);
	return;
	// 	XMJGAssist assist;
	// 	CString path = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("������������"));
	// 	path += _T("\\CG\\") + name;
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(path);
	// 	exportdwg.setPictureFrame(_T("ҵ�������1"));
	// 	exportdwg.isDrawRectangle(false);
	// 	MStr field;  GetReplaceValue(field);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(path);
}

void XMJGBuildSettingOut::ExportToDWG2()
{
	CExportGCCL gccl;
	gccl.beginExport(_T("��������2"));
	MStr field;  GetReplaceValue(field);
	gccl.replaceText(field);
	return;
	// 	XMJGAssist assist;
	// 	CString path = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("������������"));
	// 	path += _T("\\CG\\") + name;
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(path);
	// 	exportdwg.setPictureFrame(_T("ҵ�������2"));
	// 	exportdwg.isDrawRectangle(false);
	// 	MStr field;  GetReplaceValue(field);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(path);
}

void XMJGBuildSettingOut::ExportToDOC()
{
	XMJG2ExportDlg::destory();
	XMJG2ExportDlg *dlg = XMJG2ExportDlg::Instance(_T("��������"));
	if(TRUE != dlg->IsWindowVisible())
		dlg->ShowWindow(SW_SHOW);
}

void XMJGBuildSettingOut::GetReplaceValue(MStr &field)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	splitMulLineText(field, record[_T("��Ŀ����")], _T("#PROJECTNAME"));
	splitMulLineText(field, record[_T("���֤��")], _T("#LICENSECODE"));
	splitMulLineText(field, record[_T("���赥λ")], _T("#BUILDCOMPANY"));
	splitMulLineText(field, record[_T("��Ŀ�ص�")], _T("#PROJECTADDRESS"));
	field[_T("#BUILDAREA#")] = record[_T("�õ����")];
	field[_T("#ACCEPTCODE#")] = record[_T("�����")];
	field[(_T("#CHECKCODE#"))] = record[_T("���ֺ�")];
}

void XMJGBuildSettingOut::splitMulLineText(MStr &field, const CString &record, const CString &name)
{
	int idx = 0; field[name + _T("#")] = record;
	CString temp, sub(record), format(name + _T("%d#"));
	temp.Format(format, 1);
	field[temp] = record;
}

void XMJGBuildSettingOut::ReadRegisterValue(Word *word)
{
	IDataBaseOper db;
	MStr filter, field;
	filter[_T("ҵ������")] = _T("��������");
	RETFALSE(db.readDataBase(_T("XMJGProjectInfo"), filter, field));
	char val[1024] = {0};
	TcharToChar(field[_T("��Ŀ����")], val); word->replaceText("#ProjectName#", val);
	TcharToChar(field[_T("���赥λ")], val); word->replaceText("#BuildCompany#", val);
	TcharToChar(field[_T("��Ŀ�ص�")], val); word->replaceText("#ProjectAddress#", val);
	TcharToChar(field[_T("���֤��")], val); word->replaceText("#LicenseCode#", val);
	TcharToChar(field[_T("�õ����")], val); word->replaceText("#BuildArea#", val);
	TcharToChar(field[_T("�߽����")], val); word->replaceText("#EdgeDistance1#", val);
	TcharToChar(field[_T("��������")], val); word->replaceText("#BuildDistance1#", val);
	TcharToChar(field[_T("ʵ��߽�")], val); word->replaceText("#EdgeDistance2#", val);
	TcharToChar(field[_T("ʵ�����")], val); word->replaceText("#BuildDistance2#", val);
	int var = _ttoi(field[_T("������ǽ")]); if(var == 1) word->replaceText("#1", "��"); else word->replaceText("#1", "  ");
	var = _ttoi(field[_T("��������")]); if(var == 1) word->replaceText("#2", "��"); else word->replaceText("#2", "  ");
}

void XMJGBuildSettingOut::AddZDMJ2BuildEntity(ads_name ent)
{
	AcGePoint2dArray node; GetPlList(ent, node);
	AcDbObjectIdArray ids; TCHAR jzwlk[255] = {0};
	_stprintf(jzwlk, _T("%s"), m_jzwlk._layer);
	ssFromNodes(ids, node, 1, 1.0, _T("LWPOLYLINE"), jzwlk);
	if(ids.length() != 1)
	{
		acutPrintf(_T("\n������Χ���д��ڶ��������"));
		EntDel(ent);
		return;
	}
	double area = 0.0; CString info;
	GetEntArea(ent, area); info.Format(_T("%.3f"), area);
	AddXdata(aname(ids.first()), _T("ZDMJ"), DOUBLE_TYPE, info);
}

void XMJGBuildSettingOut::DrawBuildNearestBoundaries()
{
	IDataBaseOper oper; TextProperty text = oper.readTextPropery(_T("�ܱ߽�����"));
	AcDbObjectId lid = drawLine();
	if(lid == AcDbObjectId::kNull) return;
	setlayer(aname(lid), text._layer);
	setcolor(aname(lid), text._color);
	setlinetype(aname(lid), _T("SOLID"));
	TCHAR bName[255] = {0};
	if(RTCAN == ads_getstring(0, _T("\n �������ܱ߽���������:"), bName)) return;
	AddXdata(aname(lid), _T("����������"), 0, bName);
	AddXdata(aname(lid), _T("¥����"), 0, bName);
	AcGePoint3d mid; GetMiddlePoint(lid, mid);
	TextProperty bNameProp = oper.readTextPropery(_T("����������"), true);
	AcDbObjectId textid = DrawText(mid, bName, _T("MC"), _T(""), bNameProp._size * m_Scale);
	setlayer(aname(textid), bNameProp._layer); setcolor(aname(textid), bNameProp._color);

}

void XMJGBuildSettingOut::ModifyBuildName()
{
	SelectFilter sf(8, m_jzwlk._layer); AcDbObjectId id;
	if(SelectEntity(id, sf, _T("")) == false)
	{
		MessageBox(GetForegroundWindow(), _T("ѡ���ʵ�岻�Ƿ�������"), _T("������Ϣ"), MB_OK);
		return;
	}
	TCHAR bName[255] = {0}; TCHAR handle[255] = {0}; CString old_jzwmc;
	ReadXdata(aname(id), _T("¥����"), 0, bName); old_jzwmc = bName;
	if(RTCAN == ads_getstring(0, _T("\n�����뽨��������:"), bName)) return;
	ReadXdata(aname(id), _T("JZWMCZJ"), 0, handle);
	AcDbObjectId zjid = GetObjectByHandle(handle);
	SetEntText(aname(zjid), bName);
	AddXdata(aname(id), _T("����������"), 0, bName);
	AddXdata(aname(id), _T("¥����"), 0, bName);
	MStr filter, record; filter[_T("¥����")] = old_jzwmc;
	record[_T("¥����")] = bName;
	IProjectMDB pdb; pdb.updataCXXTableInfo(filter, record);
	// 	EntDel(aname(zjid)); AcGePoint3d mid; GetMiddlePoint(id, mid);
	// 	zjid = DrawText(mid, bName, _T("MC"), _T(""), m_jzwmc._size * m_Scale);
	// 	setlayer(aname(zjid), m_jzwmc._layer);
	// 	setcolor(aname(zjid), m_jzwmc._color);
	// 	AddXdata(aname(id), _T("JZWMCZJ"), 0, GetHandleByObject(zjid));
}

void XMJGBuildSettingOut::modifyBuildWidth()
{
	SelectFilter sf(8, m_jzwlk._layer);
	AcDbObjectId id;
	if(SelectEntity(id, sf, _T("")) == false)
	{
		MessageBox(GetForegroundWindow(), _T("ѡ���ʵ�岻�Ƿ�������"), _T("������Ϣ"), MB_OK);
		return;
	}
	double width = 100.0; TCHAR szdata[25] = {0};
	if(ads_getreal(_T("\n �����뷿�ݵĿ�ȣ�"), &width) == RTCAN) return;
	_stprintf(szdata, _T("%.3f"), width);
	AddXdata(aname(id), _T("���ݿ��"), 0, szdata);
}

#pragma endregion

#pragma region ҵ���� ��·����

XMJGRoadSettingOut::XMJGRoadSettingOut()
{
	m_Scale = 500;
	read1(m_Scale);
	m_Scale /= 1000;
	IDataBaseOper db;
	db.readCommonTable(_T("��·ע�Ǳ�Ŵ�С"), m_zjfont);
	db.readCommonTable(_T("��·ע�Ǳ��ͼ��"), m_zjlayer);
	db.readCommonTable(_T("��·ע�Ǳ����ɫ"), m_zjcolor);
	db.readCommonTable(_T("��·��β��Ŵ�С"), m_bhfont);
	db.readCommonTable(_T("��·��β���ͼ��"), m_bhlayer);
	db.readCommonTable(_T("��·��β�����ɫ"), m_bhcolor);
	db.readCommonTable(_T("��·����ͼ��"), m_dlbx._layer);
	db.readCommonTable(_T("��·������ɫ"), m_dlbx._color);
	db.readCommonTable(_T("��·������ͼ��"), m_dlzxx._layer);
	db.readCommonTable(_T("��·��������ɫ"), m_dlzxx._color);
	db.readCommonTable(_T("Ұ����ͼ��ͼ��"), m_ywcdtk._layer);
	db.readCommonTable(_T("Ұ����ͼ����ɫ"), m_ywcdtk._color);

}

XMJGRoadSettingOut::~XMJGRoadSettingOut()
{

}

void XMJGRoadSettingOut::CreateRSOProject()
{
	XMJGSetInfo3 info(true);
	info.DoModal();
}

void XMJGRoadSettingOut::DrawRoadEdgeLine()
{
	acutPrintf(_T("\n ���Ƶ�·���ߣ�"));
	AcDbObjectId id = drawLine();
	setlayer(aname(id), m_dlbx._layer);
	setcolor(aname(id), m_dlbx._color);
}

void XMJGRoadSettingOut::DrawRoadCentreLine()
{
	acutPrintf(_T("\n ���Ƶ�·�����ߣ�"));
	AcDbObjectId id = drawLine();
	setlayer(aname(id), m_dlzxx._layer);
	setcolor(aname(id), m_dlzxx._color);
	setlinetype(aname(id), _T("2"));
	AddXdata(aname(id), _T("����"), 0, _T("��·������"));
}

void XMJGRoadSettingOut::ExportCoodinateTable()
{
	SelectFilter sf(8, m_dlzxx._layer); AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T(""))) return;
	AcGePoint2dArray node;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcGePoint2dArray temp;
		GetPlList(aname(ids.at(idx)), temp);
		node.append(temp);
	}
	int show_count = 1; ads_point insert_pt; int row = 0;

	//��ȡ��·�������ϵĿ��Ƶ�
	vector<pair<AcDbObjectId, CString>> vKZD;
	BOOL bSuc = GetControlPoint(node, vKZD);
	if(!bSuc)
	{
		acutPrintf(_T("�õ�·���������޿��Ƶ�����!"));
		return;
	}
	int length = vKZD.size();

	TCHAR info[255] = {0}; _stprintf(info, _T("\n ������һ����ʾ������������ܹ���%d�����꣩��"), length);
	if(RTNORM != ads_getint(info, &show_count)) return;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ����������㣺"), insert_pt)) return;
	GetTableRowAndCol(length, show_count, row);
	VVStr content_list;	VStr titles;
	for(int i = 0; i < show_count; i++)
	{
		titles.push_back(_T("���"));
		titles.push_back(_T(" X "));
		titles.push_back(_T(" Y "));
	}
	content_list.push_back(titles);

	getRoadTableInfo(vKZD, content_list, row, show_count);
	AcDbObjectId t_id;
	DrawCadTable(asPnt3d(insert_pt), t_id, row, show_count * 3, 25 * m_Scale, 4 * m_Scale, false, content_list, 2 * m_Scale);
}

BOOL XMJGRoadSettingOut::GetControlPoint(const AcGePoint2dArray &nodes, vector<pair<AcDbObjectId, CString>> &vKZD)
{
	for(int i = 0; i < nodes.length(); ++i)
	{
		AcDbObjectIdArray ids;
		AcGePoint2dArray pts;
		AcGePoint2d pt1, pt2, pt3, pt4, ptS;
		ptS = nodes.at(i);
		pt1[X] = ptS[X] - 1; pt1[Y] = ptS[Y] + 1; pts.append(pt1);
		pt2[X] = ptS[X] - 1; pt2[Y] = ptS[Y] - 1; pts.append(pt2);
		pt3[X] = ptS[X] + 1; pt3[Y] = ptS[Y] - 1; pts.append(pt3);
		pt4[X] = ptS[X] + 1; pt4[Y] = ptS[Y] + 1; pts.append(pt4);
		ssFromNodes(ids, pts, 2, 0, 0, _T("��λ"));
		for(int j = 0; j < ids.length(); ++j)
		{
			AcDbObjectId idTemp = ids[j];
			TCHAR szDH[255] = {0};
			ReadXdata(aname(idTemp), _T("���"), STRING_TYPE, szDH);
			if(!_tcscmp(szDH, _T(""))) continue;
			pair<AcDbObjectId, CString> KZD(idTemp, szDH);
			vKZD.push_back(KZD); break;
		}
	}
	if(vKZD.size() == 0) return FALSE;
	return TRUE;
}

void XMJGRoadSettingOut::getRoadTableInfo(vector<pair<AcDbObjectId, CString>> &vKZD, VVStr &info, int row, int show_count)
{
	int ptNum = 0; int len = vKZD.size();
	CString x_str, row_str, y_str;
	for(int idx = 0; idx < row - 1; idx++)
	{
		VStr row_content;
		for(int indx = 0; indx < show_count; ++indx)
		{
			if(ptNum > len - 1)
			{
				row_str.Format(_T("")); row_content.push_back(row_str);
				x_str.Format(_T("")); row_content.push_back(x_str);
				y_str.Format(_T("")); row_content.push_back(y_str);
				continue;
			}
			pair<AcDbObjectId, CString> KZD = vKZD[ptNum];
			row_str.Format(_T("%s"), KZD.second); row_content.push_back(row_str);
			AcDbObjectId id = KZD.first;
			ads_point pos; GetEntPosition(aname(id), pos);
			x_str.Format(_T("%.3f"), pos[X]); row_content.push_back(x_str);
			y_str.Format(_T("%.3f"), pos[Y]); row_content.push_back(y_str);
			ptNum++;
		}
		info.push_back(row_content);
	}
}

void XMJGRoadSettingOut::getRoadTableInfo(const AcGePoint2dArray &node, VVStr &info, int row, int show_count)
{
	int ptNum = 0; int len = node.length();
	CString x_str, row_str, y_str;
	for(int idx = 0; idx < row - 1; idx++)
	{
		VStr row_content;
		for(int indx = 0; indx < show_count; ++indx)
		{
			if(ptNum > len - 1)
			{
				row_str.Format(_T("")); row_content.push_back(row_str);
				x_str.Format(_T("")); row_content.push_back(x_str);
				y_str.Format(_T("")); row_content.push_back(y_str);
				continue;
			}
			row_str.Format(_T("%d"), ptNum + 1); row_content.push_back(row_str);
			x_str.Format(_T("%.3f"), node.at(ptNum).y); row_content.push_back(x_str);
			y_str.Format(_T("%.3f"), node.at(ptNum).x); row_content.push_back(y_str);
			//addAnnotationCode(node.at(ptNum), ptNum + 1);			��ӽڵ���ע��
			ptNum++;
		}
		info.push_back(row_content);
	}
}

void XMJGRoadSettingOut::getRoadTableInfo(const AcDbObjectIdArray &ids, VVStr &info, int row, int show_count)
{
	int ptNum = 0; int len = ids.length(); double temp = 0.0;
	CString x_str, row_str, y_str; TCHAR szData[255] = {0};
	for(int idx = 0; idx < row - 1; idx++)
	{
		VStr row_content;
		for(int indx = 0; indx < show_count; ++indx)
		{
			if(ptNum > len - 1)
			{
				row_str.Format(_T("")); row_content.push_back(row_str);
				x_str.Format(_T("")); row_content.push_back(y_str);
				y_str.Format(_T("")); row_content.push_back(x_str);
				continue;
			}
			AcDbObjectId id = ids.at(ptNum);
			ReadXdata(aname(id), _T("���"), 0, szData); row_content.push_back(szData);
			ReadXdata(aname(id), _T("Y����"), 3, szData); temp = _tstof(szData);
			_stprintf(szData, _T("%.3lf"), temp); row_content.push_back(szData);
			ReadXdata(aname(id), _T("X����"), 3, szData); temp = _tstof(szData);
			_stprintf(szData, _T("%.3lf"), temp); row_content.push_back(szData);
			ptNum++;
		}
		info.push_back(row_content);
	}
}

void XMJGRoadSettingOut::addAnnotationCode(const AcGePoint2d &pt, int num)
{
	AcGePoint3d insert = apoint(pt);
	TCHAR info[4] = {0};
	_stprintf(info, _T("%d"), num);
	AcDbObjectId id = DrawText(insert, info, _T("MC"), _T(""), m_Scale * m_bhfont);
	setlayer(aname(id), m_bhlayer);
	setcolor(aname(id), m_bhcolor);
}

void XMJGRoadSettingOut::SetRoadText()
{
	acutPrintf(_T("\n ��ѡ����Ҫ��ע����յ������·�����ߣ�"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, m_dlzxx._layer);
	AcDbObjectId roadid;
	if(!SelectEntity(roadid, f1, f2, _T(""))) return;

	double annoLength;
	if(RTNORM != ads_getreal(_T("\n ������ע�ǵĳ��ȣ�"), &annoLength)) return;

	if(annoLength <= 0)
	{
		acutPrintf(_T("\n  ���볤�Ȳ�����"));
		return;
	}

	AcGePoint2dArray ptArr;
	GetPlList(aname(roadid), ptArr);
	ads_point spt, ept, sept, eept;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���·����㣺"), spt))
	{
		acutPrintf(_T("\n ��ѡ����ȷ�������"));
		return;
	}

	if(false == JudgePointInNodes(apoint(spt), ptArr)) return;

	CDrawArrow arrow;
	ads_polar(spt, PI * 0.25, annoLength, sept);

	arrow.DrawLeader(spt, sept, m_zjfont, m_zjlayer, m_zjcolor);

	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���·���յ㣺"), ept))
	{
		acutPrintf(_T("��ѡ����ȷ������"));
		return;
	}
	if(false == JudgePointInNodes(apoint(ept), ptArr)) return;

	ads_polar(ept, PI * 0.25, annoLength, eept);

	arrow.DrawLeader(ept, eept, m_zjfont, m_zjlayer, m_zjcolor);

	AddCoorInfoToEntity(roadid, apoint(spt), apoint(ept));
}

void XMJGRoadSettingOut::SetAttatchedInfo()
{
	XMJGSetInfo3 info(false);
	info.DoModal();
}

void XMJGRoadSettingOut::ExportDWGOutput()
{
	CExportGCCL gccl;
	gccl.beginExport(_T("�滮��·����ģ��"));
	MStr field;  GetReplaceValue(field);
	gccl.replaceText(field);
	return;

	// 	XMJGAssist assist;
	// 	CString path = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("��·��������"));
	// 	path += _T("\\CG\\") + name;
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(path);
	// 	exportdwg.setPictureFrame(_T("�滮��·����ģ��"));
	// 	MStr field;  GetReplaceValue(field);
	// 	exportdwg.isDrawRectangle(false);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(path);
}

void XMJGRoadSettingOut::ReadRegisterValue(Word *word)
{
	IProjectMDB pdb; char val[1024] = {0};	MStr field = pdb.getProjectInfo();
	TcharToChar(field[_T("�����")], val); word->replaceText("#ACCEPTCODE#", val);
	TcharToChar(field[_T("���赥λ")], val); word->replaceText("#BUILDCOMPANY#", val);
	TcharToChar(field[_T("��Ŀ����")], val); word->replaceText("#PROJECTNAME#", val);
	TcharToChar(field[_T("��Ŀ�ص�")], val); word->replaceText("#PROJECTADD#", val);
	TcharToChar(field[_T("���֤��")], val); word->replaceText("#LICENSECODE#", val);
	TcharToChar(field[_T("���ֺ�")], val); word->replaceText("#CHECKCODE#", val);
	TcharToChar(field[_T("�õ����")], val); word->replaceText("#BUILDAREA#", val);
	TcharToChar(field[_T("������")], val); word->replaceText("#JINGBANREN#", val);
	TcharToChar(field[_T("��ϵ�绰")], val); word->replaceText("#TELEPHONENUM#", val);
}

void XMJGRoadSettingOut::ReadRegisterValue(SBreakBlock *block)
{
	HKEY rKey; TCHAR value[1024] = {0};
	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG\\Project3"), 0, KEY_ALL_ACCESS, &rKey) == ERROR_SUCCESS)
	{
		CassQueryReg(rKey, _T("ProjectName"), value); block->replaceTextEx(_T("#PROJECTNAME#"), value);
		CassQueryReg(rKey, _T("BuildCompany"), value); block->replaceTextEx(_T("#BUILDCOMPANY#"), value);
		CassQueryReg(rKey, _T("ProjectAddress"), value); block->replaceTextEx(_T("#PROJECTADD#"), value);
		CassQueryReg(rKey, _T("JingBanRen"), value); block->replaceTextEx(_T("#JINGBANREN#"), value);
		CassQueryReg(rKey, _T("LicenseCode"), value); block->replaceTextEx(_T("#LICENESECODE#"), value);
		CassQueryReg(rKey, _T("AcceptCode"), value); block->replaceTextEx(_T("#ACCEPTCODE#"), value);
		CassQueryReg(rKey, _T("CheckCode"), value); block->replaceTextEx(_T("#CHECKCODE#"), value);
		CassQueryReg(rKey, _T("TelephoneNum"), value); block->replaceTextEx(_T("#TELEPHONENUM"), value);
	}
	else
		return;
}

void XMJGRoadSettingOut::GetReplaceValue(MStr &field)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	field[(_T("#ACCEPTCODE#"))] = record[_T("�����")];
	field[(_T("#BUILDCOMPANY#"))] = record[_T("���赥λ")];
	field[(_T("#BUILDCOMPANY1#"))] = record[_T("���赥λ")];
	field[(_T("#PROJECTNAME#"))] = record[_T("��Ŀ����")];
	field[(_T("#PROJECTNAME1#"))] = record[_T("��Ŀ����")];
	field[(_T("#PROJECTADD#"))] = record[_T("��Ŀ�ص�")];
	field[(_T("#LICENSECODE#"))] = record[_T("���֤��")];
	field[(_T("#CHECKCODE#"))] = record[_T("���ֺ�")];
	field[(_T("#BUILDAREA#"))] = record[_T("�õ����")];
	field[(_T("#JINGBANREN#"))] = record[_T("������")];
	field[(_T("#TELEPHONENUM#"))] = record[_T("��ϵ�绰")];
	field[(_T("#CHECKCODE#"))] = record[_T("���ֺ�")];
}

void XMJGRoadSettingOut::ExportWORDOutput()
{
	XMJGAssist assist;
	CString SavePath = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("��·��������"));
	SavePath += _T("\\CG\\") + name;
	char savepath[255] = {0}; TcharToChar(SavePath, savepath);
	SmartTable::Word word; TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("�����н��蹤�̣������ࣩ�����������뵥����.docx"), path))
	{
		acutPrintf(_T("\n �޷��ҵ�ģ���ļ� �����н��蹤�̣������ࣩ�����������뵥����.docx"));
		return;
	}
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath);
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	acutPrintf(_T("\n ��ѡ����Ҫ��������ĵ�·�����ߣ�һ������"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, m_dlzxx._layer);
	AcDbObjectId roadId; if(!SelectEntity(roadId, f1, f2, _T(""))) return;
	TCHAR szValue[200] = {0}; ReadRegisterValue(&word);
	if(!ReadXdata(aname(roadId), _T("����"), 0, szValue)) return;
	if(_tcscmp(szValue, _T("��·������")) != 0) return;
	TCHAR info[20] = {0}; char value[40] = {0};
	ReadXdata(aname(roadId), _T("�������X"), 0, info);
	TcharToChar(info, value);  word.replaceText("#STARTPOINTX#", value);
	ReadXdata(aname(roadId), _T("�������Y"), 0, info);
	TcharToChar(info, value);  word.replaceText("#STARTPOINTY#", value);
	ReadXdata(aname(roadId), _T("�յ�����X"), 0, info);
	TcharToChar(info, value);  word.replaceText("#ENDPOINTX#", value);
	ReadXdata(aname(roadId), _T("�յ�����Y"), 0, info);
	TcharToChar(info, value); word.replaceText("#ENDPOINTY#", value);
	word.save(savepath);
	acutPrintf(_T("\n ���������ɡ�·��Ϊ%s"), SavePath);
}

void XMJGRoadSettingOut::TextDataCheck()
{

}

void XMJGRoadSettingOut::SetRoadStartEndPoint()
{
	acutPrintf(_T("\n ��ѡ����Ҫ��������ĵ�·�����ߣ�һ������"));
	SelectFilter f1(RTDXF0, _T("*polyline")), f2(8, m_dlzxx._layer);
	AcDbObjectId roadId; if(!SelectEntity(roadId, f1, f2, _T(""))) return;
	TCHAR szValue[200] = {0};
	if(!ReadXdata(aname(roadId), _T("����"), 0, szValue)) return;
	if(_tcscmp(szValue, _T("��·������")) != 0)
	{
		acutPrintf(_T("\n ѡ���·������"));
		return;
	}
	ads_point spt, ept; TCHAR info[20] = {0};
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���·��㣺"), spt)) return;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���·�յ㣺"), ept)) return;
	_stprintf(info, _T("%.3f"), spt[X]);
	AddXdata(aname(roadId), _T("�������X"), 0, info);
	_stprintf(info, _T("%.3f"), spt[Y]);
	AddXdata(aname(roadId), _T("�������Y"), 0, info);
	_stprintf(info, _T("%.3f"), ept[X]);
	AddXdata(aname(roadId), _T("�յ�����X"), 0, info);
	_stprintf(info, _T("%.3f"), ept[Y]);
	AddXdata(aname(roadId), _T("�յ�����Y"), 0, info);
}

void XMJGRoadSettingOut::GetTableRowAndCol(int &len, int &per_count, int &row)
{
	//��ͷռһ�У���������һ
	row = (len + per_count - 1) / per_count + 1;
}

bool XMJGRoadSettingOut::JudgePointInNodes(const AcGePoint2d &pt, const AcGePoint2dArray &nodes)
{
	bool flag = false;
	for(int i = 0; i < nodes.length() - 1; i++)
	{
		if(!PointAtLine(nodes[i], nodes[i + 1], pt)) continue;
		flag = true; break;
	}

	if(flag == false)
	{
		acutPrintf(_T("\n ѡ��ĵ㲻�ڵ�·��������"));
		return false;
	}
	return true;
}

void XMJGRoadSettingOut::AddCoorInfoToEntity(AcDbObjectId &id, const AcGePoint2d &spt, const AcGePoint2d &ept)
{
	TCHAR info[20] = {0};
	_stprintf(info, _T("%.3f"), spt[Y]);
	AddXdata(aname(id), _T("�������X"), 0, info);
	_stprintf(info, _T("%.3f"), spt[X]);
	AddXdata(aname(id), _T("�������Y"), 0, info);
	_stprintf(info, _T("%.3f"), ept[Y]);
	AddXdata(aname(id), _T("�յ�����X"), 0, info);
	_stprintf(info, _T("%.3f"), ept[X]);
	AddXdata(aname(id), _T("�յ�����Y"), 0, info);
}

void XMJGRoadSettingOut::getAnnotationPoint(AcDbObjectIdArray &ids, AcGePoint2dArray &pts)
{
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcGePoint3d pt;
		AcDbObjectId id = ids.at(idx);
		getEntPosition(aname(id), pt);
		pts.append(apoint(pt));
	}
}

#pragma endregion

#pragma region ҵ���� ˮ׼�߳�

XMJGEvelingElevation::XMJGEvelingElevation()
{
	m_Scale = 500;	read1(m_Scale);	m_Scale /= 1000;
	//XMJGAssist assist; assist.ReadRegForContext(_T("ˮ׼�߳�"), m_SaveDir);
}

XMJGEvelingElevation::~XMJGEvelingElevation()
{

}

void XMJGEvelingElevation::createProject()
{
	XMJGSetInfo4 info(true);
	info.DoModal();
}

void XMJGEvelingElevation::SetAttatchedInfo()
{
	XMJGSetInfo4 info(false);
	info.DoModal();
}

void XMJGEvelingElevation::ExportResult2DWG1()
{
	MStr field;
	int count = GetControlPointCount(field);
	if(count < 1 || count > 3)
	{
		acutPrintf(_T("\n ���Ƶ�ĸ��������⣬������Ƶ�ĸ���������"));
		return;
	}
	TCHAR style[200] = {0};
	_stprintf(style, _T("ҵ����ʽ��1%d"), count);
	CExportGCCL gccl;
	gccl.beginExport(style);
	GetReplaceValue(field);
	gccl.replaceText(field);
	return;
	// 	XMJGAssist assist;
	// 	CString path = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("ˮ׼�߳�"));
	// 	path += _T("\\CG\\") + name;
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(path);
	// 	exportdwg.setPictureFrame(style);
	// 	exportdwg.isDrawRectangle(false);
	// 	GetReplaceValue(field);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(path);
}

void XMJGEvelingElevation::ExportResult2DWG2()
{
	MStr field;
	int count = GetControlPointCount(field);
	if(count < 1 || count > 3)
	{
		acutPrintf(_T("\n ���Ƶ�ĸ��������⣬������Ƶ㣡����"));
		return;
	}
	TCHAR style[200] = {0};
	_stprintf(style, _T("ҵ����ʽ��2%d"), count);
	CExportGCCL gccl;
	gccl.beginExport(style);
	GetReplaceValue(field);
	gccl.replaceText(field);
	return;
	// 	XMJGAssist assist;
	// 	CString path = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("ˮ׼�߳�"));
	// 	path += _T("\\CG\\") + name;
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(path);
	// 	exportdwg.setPictureFrame(style);
	// 	exportdwg.isDrawRectangle(false);
	// 	GetReplaceValue(field);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(path);
}

void XMJGEvelingElevation::GetReplaceValue(MStr &field)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	field[_T("#TELEPHONE#")] = record[_T("��ϵ�绰")];
	field[(_T("#COMPLAINT#"))] = record[_T("Ͷ�ߵ绰")];
	field[(_T("#ACCEPTCODE#"))] = record[_T("�����")];
	field[(_T("#PROJECTNAME#"))] = record[_T("��Ŀ����")];
	field[(_T("#ENTRUSTCOMPANY#"))] = record[_T("ί�е�λ")];
	field[(_T("#ELEVATIONSYSTEM#"))] = record[_T("�߳�ϵͳ")];
	field[(_T("#CHECKCODE#"))] = record[_T("���ֺ�")];
}

int XMJGEvelingElevation::GetControlPointCount(MStr &field)
{
	SelectFilter f1(8, _T("���Ƶ�"));
	AcDbObjectIdArray ids; int count = 0;
	SelectEntitys(ids, f1, _T("X"));
	TCHAR info[200] = {0}, name[20] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		ReadXdata(aname(id), _T("����"), 0, info);
		if(0 == _tcslen(info)) continue;
		_stprintf(name, _T("#CP%d#"), idx + 1);	field[name] = info;
		ReadXdata(aname(id), _T("�߳�"), 0, info);
		_stprintf(name, _T("#EV%d#"), idx + 1);	field[name] = info;
		ReadXdata(aname(id), _T("�ȼ�"), 0, info);
		_stprintf(name, _T("#ER%d#"), idx + 1);	field[name] = info;
		ReadXdata(aname(id), _T("����"), 0, info);
		_stprintf(name, _T("#FS%d#"), idx + 1);	field[name] = info;
		count++;
	}
	return count;
}

#pragma endregion

#pragma region ҵ���� �������߼���

XMJGCheckLine::XMJGCheckLine()
	: m_dist(4.0)
{
	m_Scale = 500; read1(m_Scale); m_Scale /= 1000;
	IDataBaseOper db;
	db.readCommonTable(_T("���߱�ע��С"), m_zxbz._size);
	db.readCommonTable(_T("���߱�ע��ɫ"), m_zxbz._color);
	db.readCommonTable(_T("���߱�עͼ��"), m_zxbz._layer);
	db.readCommonTable(_T("���߱�עԲ��С"), m_zxbzy._size);
	db.readCommonTable(_T("���߱�עԲ��ɫ"), m_zxbzy._color);
	db.readCommonTable(_T("���߱�עԲͼ��"), m_zxbzy._layer);
	db.readCommonTable(_T("���߱�עֱ�ߴ�С"), m_zxbzzx._size);
	db.readCommonTable(_T("���߱�עֱ����ɫ"), m_zxbzzx._color);
	db.readCommonTable(_T("���߱�עֱ��ͼ��"), m_zxbzzx._layer);
	db.readCommonTable(_T("���ݱ߳�ע�Ǵ�С"), m_fwbczj._size);
	db.readCommonTable(_T("���ݱ߳�ע����ɫ"), m_fwbczj._color);
	db.readCommonTable(_T("���ݱ߳�ע��ͼ��"), m_fwbczj._layer);
}

XMJGCheckLine::~XMJGCheckLine()
{

}

void XMJGCheckLine::createProject()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo5::destory();
	XMJGSetInfo5 *info = XMJGSetInfo5::Instance(true);
	if(info->IsWindowVisible() == FALSE)
		info->ShowWindow(SW_SHOW);
}

void XMJGCheckLine::DrawSurroundingBuild()
{
	struct resbuf *cmd, *result;
	cmd = ads_buildlist(RTSTR, _T("drawdjf"), RTSTR, _T(""), RTNONE);
	ads_invoke(cmd, &result); ads_relrb(cmd); ads_name ent;
	ads_entlast(ent); setlayer(ent, _T("�ܱ߽�����"));
}

void XMJGCheckLine::AnnPretreatment()
{
	SelectFilter f1(8, _T("����������")), f2(RTDXF0, _T("*polyline"));
	AcDbObjectIdArray ids;
	if(!SelectEntitys(ids, f1, f2, _T("")))
	{
		acutPrintf(_T("\n ѡ��ʵ��ʧ�ܣ�����"));
		return;
	}
	double dist = 3.2;
	if(RTCAN == ads_getreal(_T("\n ������ο�����ʵ��ľ��룺<3.2>"), &dist)) return;
	if(dist < 0) dist = 3.2;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		DrawLines(ids.at(idx));
		DrawReferenceEdge(ids.at(idx), dist);
	}
}

void XMJGCheckLine::AxisAutoAnnotion1()
{
	acutPrintf(_T("\n ��ѡ����Ҫ��ע�ķ��ݣ�"));
	SelectFilter f1(RTDXF0, _T("*polyline"));
	AcDbObjectId id; double dist = 0.0;
	if(!SelectEntity(id, f1, NULL)) return;
	if(RTNORM != ads_getreal(_T("\n �������ע�ľ��룺"), &dist)) return;
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	if(!IsDeasil(nodes))
		nodes.reverse();
}

void XMJGCheckLine::AxisAutoAnnotion()
{
	AcDbObjectId id; AcGePoint2d lspt(0, 0), lept(0, 0);
	if(false == GetAnnotionEntAndPoint(id, lspt, lept)) return;

	ads_point sPt, ptTemp;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ָ��ע����һ�ࣺ"), ptTemp)) return;

	ReadAnnotionDist();

	double angleStandard = 0.0;
	angleStandard = ads_angle(apoint(lspt), apoint(lept));
	ads_polar(apoint(lspt), angleStandard + PI / 2, m_dist, sPt);

	TCHAR content[200] = {0};
	if(RTNORM != ads_getstring(0, _T("\n ���������ߺ�ǰ׺"), content)) return;

	//��ȡ�ο������ϵ�ע�ǲ����
	AcGePoint2dArray nodes;
	AcGePoint2dArray nodesPairs;
	GetPlList(aname(id), nodes);
	double angle_reverse = IsDeasil(nodes) == true ? 0.5 : -0.5;
	for(int i = 0; i < nodes.length() - 1; i++)
	{
		double tempAngle = ads_angle(apoint(nodes[i]), apoint(nodes[i + 1]));
		if(abs(tempAngle - angleStandard) < 0.05)
		{
			if(nodesPairs.contains(nodes[i]))
			{
				nodesPairs.remove(nodes[i]);
				nodesPairs.append(nodes[i + 1]);
			}
			else
			{
				nodesPairs.append(nodes[i]);
				nodesPairs.append(nodes[i + 1]);
			}
		}
	}
	AcGePoint2dArray insertPts;
	int number = 1;
	//����ע��
	for(int i = 0; i < nodesPairs.length() - 1; i += 2)
	{
		AcGePoint2d projectedPoint = getProjectedPointOnLine(apoint(sPt), angleStandard, nodesPairs[i]);

		bool has_pt = false;
		for(int j = 0; j < insertPts.length(); j++)
		{
			if(abs(insertPts[j].x - projectedPoint.x) < EPS && abs(insertPts[j].y - projectedPoint.y) < EPS)
				has_pt = true;
		}
		insertPts.append(projectedPoint);

		if(has_pt)	continue;
		TCHAR szContent[200];
		_stprintf(szContent, _T("%s%d"), content, number++);
		drawSingleAxisAnno(projectedPoint, szContent, angleStandard + PI * angle_reverse, m_zxbz, m_zxbzy, m_zxbzzx);
	}
}

void XMJGCheckLine::AxisManualAnnotion()
{
	AcDbObjectId id; AcGePoint2d lspt(0, 0), lept(0, 0);
	if(false == GetAnnotionEntAndPoint(id, lspt, lept)) return;

	//ѡ��ע�ǵ�һ��
	ads_point sidePt;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ����Ҫע�ǵ�һ�ࣺ"), sidePt)) return;
	if((sidePt[X] - lspt.x) * (sidePt[X] - lept.x) < 0 ||
		(sidePt[Y] - lspt.y) * (sidePt[Y] - lept.y) < 0)
	{
		acutPrintf(_T("\n ѡ��ע�ǵ�λ�ô��󣡣���"));
		return;
	}
	ReadAnnotionDist();
	double angle = ads_angle(apoint(lspt), apoint(lept));

	double slen = distOF2d(lspt, apoint(sidePt));
	double elen = distOF2d(lept, apoint(sidePt));
	TCHAR content[200] = {0};
	if(RTNORM != ads_getstring(0, _T("\n ������ע�����ݣ�"), content)) return;

	ads_point insertpt;
	if(slen < elen)
	{
		ads_polar(apoint(lspt), angle + PI, m_dist, insertpt);
		drawSingleAxisAnno(apoint(insertpt), content, angle + PI, m_zxbz, m_zxbzy, m_zxbzzx);
	}
	else
	{
		ads_polar(apoint(lept), angle, m_dist, insertpt);
		drawSingleAxisAnno(apoint(insertpt), content, angle, m_zxbz, m_zxbzy, m_zxbzzx);
	}
}

void XMJGCheckLine::SetAttatchedInfo()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo5::destory();
	XMJGSetInfo5 *info = XMJGSetInfo5::Instance(false);
	if(info->IsWindowVisible() == FALSE)
		info->ShowWindow(SW_SHOW);
}

void XMJGCheckLine::AutoBuildingInsideAnnotion()
{
	acutPrintf(_T("\n ��ѡ��һ����Ҫ��ע��ʵ�壺"));
	SelectFilter f1(RTDXF0, _T("*line"));
	AcDbObjectIdArray buildingIdArr;
	if(!SelectEntitys(buildingIdArr, f1, _T(""))) return;
	int notelocal = 1; double mindist = 0.1;
	if(RTCAN == ads_getreal(_T("\n �����ñ�ע��ֵ��<0.1>"), &mindist)) return;
	if(RTCAN == ads_getint(_T("\n ������ע�ǵ�λ��<1>�ⲿ��<2>�ڲ���[1]"), &notelocal)) return;
	CXRecordOperate record; double scale = 1.0;
	if(TRUE == record.ReadDoubleRecord(_T("GCCL"), _T("BLC"), scale))
		m_fwbczj._size = (float)(scale * TEXTSIZE);
	else
	{
		IDataBaseOper oper;
		m_fwbczj = oper.readTextPropery(_T("�����������ݱ߳�ע��"), true);
	}
	for(int idx = 0; idx < buildingIdArr.length(); ++idx)
	{
		AcGePoint2dArray nodes; AcGeDoubleArray bluges;
		AcDbObjectId id = buildingIdArr.at(idx);
		GetPlList(aname(id), nodes);
		GetPlBulges(aname(id), bluges);
		if(IsDeasil(nodes) == false) nodes.reverse();
		double angle_reverse = notelocal == 1 ? -0.5 : 0.5;
		int len = nodes.length();
		bool isClose = 9 < GetPolyType(aname(id), true);
		for(int i = 0; i < len - (isClose == true ? 0 : 1); i++)
		{
			AcGePoint2d pts = nodes[i % len];
			AcGePoint2d pte = nodes[(i + 1) % len];
			double bluge = bluges[i];
			AcGePoint2d midpt(pte.x * 0.5 + pts.x * 0.5, pte.y * 0.5 + pts.y * 0.5);
			double dist = distOF2d(pts, pte);
			if(dist < mindist) continue; double angle = 0.0;
			CString dist_str;	ads_point insertpt;
			angle = ads_angle(apoint(pte), apoint(pts));
			if(fabs(bluge) > EPS)
			{
				double angle2 = atan(bluge) * 4;
				double radius = fabs(dist / 2.0 / sin(angle2 / 2));
				double val1 = radius - radius * cos(angle2 / 2) - m_fwbczj._size * m_Scale * 2 * angle_reverse;
				double angle1 = ads_angle(apoint(pts), apoint(pte)) + PI / 2;
				ads_polar(apoint(midpt), angle1, val1, insertpt);
				dist_str.Format(_T("R=%.2f"), radius);
			}
			else
			{
				dist_str.Format(_T("%.2f"), dist);
				ads_polar(apoint(midpt), angle + angle_reverse * PI, 0.4 * m_Scale, insertpt);
			}
			if (angle > PI / 2 + EPS && angle < 3 * PI / 2 + EPS) angle -= PI;
			AcDbObjectId textID = DrawText(insertpt, dist_str, _T("MC"), _T("20000"), m_fwbczj._size * m_Scale, angle, 1.0);
			setcolor(aname(textID), m_fwbczj._color);
			setlayer(aname(textID), m_fwbczj._layer);
		}
	}

}

void XMJGCheckLine::ExportResult2DOC()
{
	CAcModuleResourceOverride myResources;
	XMJG2ExportDlg::destory();
	XMJG2ExportDlg *dlg = XMJG2ExportDlg::Instance(_T("��������"));
	dlg->SetRegPath(false);
	if(dlg->IsWindowVisible() == FALSE)
		dlg->ShowWindow(SW_SHOW);
}

void XMJGCheckLine::ExportResult2DWG1()
{
	CExportGCCL gccl;
	gccl.beginExport(_T("ҵ������ʽ1"));
	MStr field;  GetReplaceValue(field);
	gccl.replaceText(field);
	return;
	// 	XMJGAssist assist;
	// 	CString path = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("�������������"));
	// 	path.Append(_T("\\CG\\")); path.Append(name);
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(path);
	// 	exportdwg.setPictureFrame(_T("ҵ������ʽ1"));
	// 	exportdwg.isDrawRectangle(false);
	// 	MStr field;  GetReplaceValue(field);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(path);
}

void XMJGCheckLine::ExportResult2DWG2()
{
	CExportGCCL gccl;
	gccl.beginExport(_T("ҵ������ʽ2"));
	MStr field;  GetReplaceValue(field);
	gccl.replaceText(field);
	return;
	// 	XMJGAssist assist;
	// 	CString path = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("�������������"));
	// 	path += _T("\\CG\\") + name;
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(path);
	// 	exportdwg.setPictureFrame(_T("ҵ������ʽ2"));
	// 	exportdwg.isDrawRectangle(false);
	// 	MStr field;  GetReplaceValue(field);
	// 	if(true == exportdwg.ExportDwg(field, XMJGReplaceField))
	// 		openDwgCommand(path);
}

void XMJGCheckLine::drawBuildOutLine()
{
	XMJGAssist assist; TCHAR bName[255] = {0};
	//AcDbObjectId id = assist.DrawLine(_T("����������"), _T(""), 4, _T("6"));
	AcDbObjectId id = drawLine();
	setlayer(aname(id), _T("����������"));
	setcolor(aname(id), 4);
	setlinetype(aname(id), _T("6"));
	if(RTCAN == ads_getstring(0, _T("\n �����뽨��������:"), bName)) return;
	AddXdata(aname(id), _T("����������"), 0, bName);
	AddXdata(aname(id), _T("¥����"), 0, bName);
	AcGePoint3d mid; GetMiddlePoint(id, mid);
	DrawText(mid, bName, _T("MC"), _T(""), 2.5 * m_Scale);
}

void XMJGCheckLine::splitMulLineText(MStr &field, const CString &record, const CString &name)
{
	int idx = 0; field[name + _T("#")] = record;
	CString temp, sub(record), format(name + _T("%d#"));
	for(idx = 0; idx < 3; ++idx)
	{
		int len = sub.GetLength();
		int pos = sub.FindOneOf(_T("\r\n"));
		temp.Format(format, idx + 1);
		if(pos == -1 && sub.GetLength() != 0)
		{
			field[temp] = sub;
			break;
		}
		field[temp] = sub.Mid(0, pos);
		sub = sub.Right(len - pos - (int)_tcslen(_T("\r\n")));
	}
	for(idx += 1; idx < 3; ++idx)
	{
		temp.Format(format, idx + 1);
		field[temp] = _T("    ");
	}
}

void XMJGCheckLine::ReadRegisterValue(SmartTable::Word *word)
{
	IDataBaseOper db; MStr field;
	RETFALSE(db.readProjectInfoTable(_T("��������"), field));
	char val[1024] = {0};
	TcharToChar(field[_T("��Ŀ����")], val); 	word->replaceText("#ProjectName#", val);
	TcharToChar(field[_T("���赥λ")], val); word->replaceText("#BuildCompany#", val);
	TcharToChar(field[_T("��Ŀ�ص�")], val); word->replaceText("#ProjectAddress#", val);
	TcharToChar(field[_T("���֤��")], val); word->replaceText("#LicenseCode#", val);
	TcharToChar(field[_T("�õ����")], val); word->replaceText("#BuildArea#", val);
	TcharToChar(field[_T("�滮���߼��")], val); word->replaceText("#EdgeDistance1#", val);
	TcharToChar(field[_T("�滮�������")], val); word->replaceText("#BuildDistance1#", val);
	TcharToChar(field[_T("ʵ����߼��")], val); word->replaceText("#EdgeDistance2#", val);
	TcharToChar(field[_T("ʵ�⽨�����")], val); word->replaceText("#BuildDistance2#", val);
	int var = _ttoi(field[_T("������ǽ")]); if(var == 1) word->replaceText("#1", "��"); else word->replaceText("#1", "  ");
	var = _ttoi(field[_T("��������")]); if(var == 1) word->replaceText("#2", "��"); else word->replaceText("#2", "  ");
}

void XMJGCheckLine::GetReplaceValue(MStr &field)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	splitMulLineText(field, record[_T("��Ŀ����")], _T("#PROJECTNAME"));
	splitMulLineText(field, record[_T("���֤��")], _T("#LICENSECODE"));
	splitMulLineText(field, record[_T("���赥λ")], _T("#BUILDCOMPANY"));
	splitMulLineText(field, record[_T("��Ŀ�ص�")], _T("#PROJECTADDRESS"));
	field[(_T("#BUILDAREA#"))] = record[_T("�õ����")];
	field[(_T("#ACCEPTCODE#"))] = record[_T("�����")];
	field[(_T("#CHECKCODE#"))] = record[_T("���ֺ�")];
}

void XMJGCheckLine::DrawLines(AcDbObjectId &id)
{
	AcGePoint2dArray node;
	GetPlList(aname(id), node);
	int len = node.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint2d ept = node.at(idx);
		AcGePoint2d spt = node.at((idx + 1) % len);
		AcDbObjectId lineid = drawline(apoint(ept), apoint(spt), _T(""), 0, _T("WXLAYER"));
		ads_name ent; acdbGetAdsName(ent, lineid);
		acedCommand(RTSTR, _T("draworder"), RTENAME, ent, RTSTR, _T(""), RTSTR, _T("B"), 0);
	}
}

void XMJGCheckLine::DrawReferenceEdge(AcDbObjectId &id, double offset)
{
	ads_point ptMin, ptMax, ptTmp;
	GetEntExtent(aname(id), ptMin, ptMax, offset);
	ptTmp[X] = ptMin[X]; ptTmp[Y] = ptMax[Y];
	AcGePoint2dArray node, nodes; GetPlList(aname(id), node);
	if(IsDeasil(node))
		Poly2dArrayOffset(node, offset, true, nodes);
	DrawPolyline2D(nodes, 0, true, _T(""), 0, 0, _T("REdge"));
}

void XMJGCheckLine::ReadAnnotionDist()
{
	HKEY rKey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG\\Project5"), 0, KEY_ALL_ACCESS, &rKey) == ERROR_SUCCESS)
		CassQueryReg(rKey, _T("ע�Ǿ���"), m_dist);
	TCHAR info[100] = {0};
	_stprintf(info, _T("\n ������ע�ǵľ��룺<%0.3f>"), m_dist);
	if(RTCAN == ads_getreal(info, &m_dist)) return;
	CassSetReg(rKey, _T("ע�Ǿ���"), m_dist);
}

bool XMJGCheckLine::GetAnnotionEntAndPoint(AcDbObjectId &id, AcGePoint2d &spt, AcGePoint2d &ept)
{
	if(false == SelectEntity(id, _T(""))) return false;
	ads_point pt;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ����Ҫע�ǵıߣ�"), pt)) return false;
	AcGePoint2dArray node; GetPlList(aname(id), node);
	if(!IsDeasil(node)) node.reverse(); SetPolyCoord(aname(id), node);
	double dist = 10000; int len = node.length();
	for(int idx = 0; idx < len; ++idx)
	{
		spt = node.at(idx);
		ept = node.at((idx + 1) % len);
		if(true == PointAtLine(spt, ept, apoint(pt))) return true;
	}
	acutPrintf(_T("\n ѡ���ע�Ǳ߲���ʵ���ϣ�������ѡ�񡣡���"));
	return false;
}

#pragma endregion

#pragma region ҵ���� �������ղ���

#pragma region ������������

XMJGPedestrianBridge::XMJGPedestrianBridge()
{
	m_Scale = 500; read1(m_Scale); m_Scale /= 1000; IDataBaseOper oper;
	XMJGAssist assist; assist.ReadRegForContext(_T("����"), m_SaveDir);
	m_qbjx = oper.readTextPropery(_T("���ſ����ű߽�"), true);
	m_tqtc = oper.readTextPropery(_T("���ſ������"));
	m_tqbg = oper.readTextPropery(_T("���ſ�����߱�ע"), true);
}

XMJGPedestrianBridge::~XMJGPedestrianBridge()
{

}

void XMJGPedestrianBridge::bridgeZPT()
{
	XMJGExportZPT zpt;
	zpt.setUndoFlag();
	zpt.setExportRange();
	zpt.setTuQian(_T("XMJGTuQian"));
	double zjzmj = 0.0; AcDbObjectId id;
	SelectFilter sf(8, m_qbjx._layer);
	if(false == SelectEntity(id, sf, _T("X"))) return;
	GetEntArea(aname(id), zjzmj);
	zpt.setJZMJArea(zjzmj);
	zpt.drawMesh();
	CString pathName = zpt.beginExport(_T("���ſ�����ƽͼ"));
	zpt.beginUndo();
	openDwgCommand(pathName);
}

void XMJGPedestrianBridge::bridgeHatch()
{
	SelectFilter f(8, m_qbjx._layer); AcDbObjectId buildid;
	if(false == SelectEntity(buildid, f, _T(""))) return;
	AcGePoint2dArray node; GetPlList(aname(buildid), node);
	AcDbObjectIdArray ids; ssFromNodes(ids, node);
	AcDbObjectIdArray unfill; ids.remove(buildid);
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		int type = 1;
		AcDbObjectId id = ids.at(idx);
		if(1 != isEntClosed(id)) continue;
		SplashEntity(aname(id), 6);
		if(RTCAN == ads_getint(_T("\n ʵ���Ƿ���Ҫ���[1]��[2]��:<1>"), &type)) return;
		if(type == 1) unfill.append(id);
	}
	AREAINFO info; info.IdFlood = buildid; info.idSafe.append(unfill);
	AcDbObjectId region = C3DOperator::CreateRingRegion(buildid, unfill);
	AcDbObjectId hatchid = C3DOperator::HatchObject(info, m_tqtc._color, _T("SOLID"));
	setcolor(aname(region), m_tqtc._color);
	SetLineWidth(aname(buildid), m_qbjx._size * m_Scale); ads_name ent; acdbGetAdsName(ent, buildid);
	acedCommand(RTSTR, _T("draworder"), RTENAME, ent, RTSTR, _T(""), RTSTR, _T("B"), 0);
}

void XMJGPedestrianBridge::DrawRedLine()
{
	XMJGAssist assist;
	acutPrintf(_T("\n ���ƺ��߷�Χ"));
	AcDbObjectId redid = drawLine();
	setcolor(aname(redid), 1);
	setlayer(aname(redid), _T("����"));
}

void XMJGPedestrianBridge::ExportDWGPB()
{
	// 	AcDbObjectIdArray IdArr;
	// 	if(false == SelectEntitys(IdArr, _T(""))) return;
	// 	CFileDialog dlg(FALSE, _T("��ѡ�񱣴�·��"));
	// 	dlg.m_ofn.lpstrInitialDir = m_SaveDir;
	// 	if(IDOK != dlg.DoModal())	return;
	// 	XMJGExport exportdwg;
	// 	exportdwg.setSavePath(dlg.GetPathName());
	// 	exportdwg.setObjects(IdArr); int exporttype = 0;
	// 	exportdwg.setPictureFrame(_T("XMJG�ֲ�ƽ��ͼ"));
	// 	ads_getint(_T("\n �����ʽ<0>dwg <1>jpg��"), &exporttype);
	// 	if(exporttype == 0)	exportdwg.ExportDwg();
	// 	else exportdwg.ExportPicture(true);
	// 	return;
	// 
	// 
	// 	AcDbObjectIdArray idArr;
	// 	SelectFilter filter(8, _T("�ֲ�ƽ��"));
	// 	if(false == SelectEntitys(idArr, filter, _T(""))) return;
	// 
	// 	XMJGPrintPicture picture;
	// 	picture.setObjectArray(idArr);
	// 	picture.setPrintSizeName();
	// 	picture.setPrintFormat(RasterFormat::Raster_Bmp);
	// 	picture.setPrintPath(_T("F:\\jpg\\config.jpg"));
	// 	if(false == picture.BeginPrintPicture()) return;
	// 	picture.rescale_image_jpeg();
	// 	return;
	// 	XMJGTool tool;
	// 	XMJGPath *path = new XMJGPath(_T("F:\\jpg\\test.docx"), _T("F:\\jpg\\config.csv"), _T("F:\\jpg"));
	// 	tool.ExportDocument(path, PedestrianBridge);
	// 	return;
	// 	XMJGTest();
	// 	return;

	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;
	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	TCHAR value[1024] = {0};
	assist.QueryRegisterValue(HKEY_CURRENT_USER, XMJGPB, _T("ProjectName"), value);
	//assist.DrawMesh(lb, rt, DiXingTu); 
	MVStr filter; VStr vals;
	vals.push_back(_T("��Ƭ")); vals.push_back(_T("���Ƶ�"));
	filter[_T("ͼ��")] = vals;
	assist.FilterEntitys(IdArr, filter);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	return;
}

void XMJGPedestrianBridge::birdgeHatch()
{
	SelectFilter f(8, m_qbjx._layer); AcDbObjectId tqid;
	if(false == SelectEntity(tqid, f, _T(""))) return; AcDbObjectId id;
	TCHAR layer[255] = {0}; _stprintf(layer, _T("%s"), m_tqtc._layer); AcDbObjectId tcid;
	tcid = DrawHatch_Auto(aname(tqid), _T("SOLID"), 1.0, 1.0, NULL, m_tqtc._color, layer);
	ads_name ent; acdbGetAdsName(ent, tcid); setcolor(ent, m_tqtc._color); setlayer(ent, layer);
	//	acedCommand(RTSTR, _T("_draworder"), RTENAME, aname(tqid), RTSTR, _T(""), RTSTR, _T("F"), RTNONE);
}

void XMJGPedestrianBridge::createProject()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo6 info(true); info.DoModal();
}

void XMJGPedestrianBridge::bridgeAnnoate()
{
	int typeindex = 1, idx = 1; IProjectMDB pdb;
	MStr values = pdb.getBridgeXX();
	if(values.size() == 0)
	{
		acutPrintf(_T("\n�����������Ÿ߶���Ϣ"));
		return;
	}
	CString type(_T("\n ��ѡ���ע�����ͣ�"));
	CString value(_T("\n ��ѡ��Ҫ��ע��ֵ��"));
	MSIter it = values.begin();
	for(; it != values.end(); ++it, idx++)
	{
		CString index; index.Format(_T("[%d]"), idx);
		type += index + it->first + _T("��");
	}
	type = type.Mid(0, type.GetLength() - 1) + _T(" <1>��");
A:	if(RTCAN == ads_getint(type, &typeindex)) return;
	it = values.begin(); VStr bzzs; idx = 0;
	if(typeindex > values.size()) return;
	for(idx = 1; idx < typeindex; ++idx) it++;
	AnalyzeData(bzzs, it->second, _T(";")); idx = 1;
	for(int i = 0; i < (int)bzzs.size(); ++i)
	{
		if(bzzs[i].IsEmpty() == true) continue;
		CString index; index.Format(_T("[%d]"), idx++);
		value += index + bzzs[i] + _T("��");
	}
	value += _T("[B]���� <1>��"); ads_point spt, ept, mpt;
	TCHAR _dist[255] = {0};
	while(true)
	{
		if(RTCAN == ads_getstring(0, value, _dist)) return;
		if(_tcscmp(_dist, _T("B")) == 0 || _tcscmp(_dist, _T("b")) == 0) goto A;
		typeindex = _ttoi(_dist); typeindex = typeindex == 0 ? 1 : typeindex;
		if(typeindex == 0 || typeindex > (int)bzzs.size()) return;
		if(it->first.FindOneOf(_T("���")) != -1)
		{
			if(RTNORM != ads_getpoint(NULL, _T("\n ��ʰȡ��ע��λ�ã�"), spt)) return;
			AcDbObjectId id = DrawInsert(spt, _T("���"), m_Scale, m_Scale);
			ads_polar(spt, PI / 2, 1.2 * m_Scale, ept); AcDbObjectId textid;
			_stprintf(_dist, _T("%s%sm"), it->first, bzzs[typeindex - 1]);
			textid = DrawText(ept, _dist, _T("BL"), _T(""), m_tqbg._size * m_Scale);
			SetTextStyle(textid, m_tqbg._font); setcolor(aname(textid), m_tqbg._color);
			setlayer(aname(textid), m_tqbg._layer); setlayer(aname(id), m_tqbg._layer);
			continue;
		}
		if(RTNORM != ads_getpoint(NULL, _T("\n �������һ����"), spt)) return;
		if(RTNORM != ads_getpoint(spt, _T("\n ������ڶ�����"), ept)) return;
		mpt[X] = (spt[X] + ept[X]) / 2; mpt[Y] = (spt[Y] + ept[Y]) / 2;
		_stprintf(_dist, _T("%s"), bzzs[typeindex - 1]);
		int old_osmode = PickstyleReset(0);
		ads_command(RTSTR, _T("dimaligned"), RTPOINT, spt, RTPOINT, ept, RTSTR, _T("T"), RTSTR, _dist, RTPOINT, mpt, RTNONE);
		PickstyleBack(old_osmode);	acedCommand(RTSTR, _T("Regen"), RTNONE);
		acutPrintf(_T("\n��ע�ɹ�"));
	}
}

void XMJGPedestrianBridge::setBirdgeLayer()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	SelectFilter sf0(RTDXF0, _T("*POLYLINE")), sf1(8, m_qbjx._layer);
	SelectFilter sf(8, m_qbjx._layer); AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T("X")))
	{
		acutPrintf(_T("\nͼ�����޷��ҵ�����ʵ��")); return;
	}
	int layer = 1;
	if(RTCAN == ads_getint(_T("\n ���������ŵĲ�����"), &layer)) return;
	ads_point ptMin, ptMax;	GetEntsExtent(ids, ptMin, ptMax, 2.0);
	AcGePoint2dArray node; node.append(apoint(ptMin));
	node.append(AcGePoint2d(ptMin[X], ptMax[Y])); node.append(apoint(ptMax));
	node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId lineid = DrawPolyline2D(node, 1.0, true, _T(""));
	ids.append(lineid);  AcDbObjectIdArray saveid; saveid.append(ids);
	AddXdata(aname(lineid), _T("TQBJK"), 0, _T("TQBJK"));
	setlayer(aname(lineid), _T("TQBJK"));
	double xmove = ptMax[X] - ptMin[X] + 10 * m_Scale;
	for(int idx = 1; idx < layer; ++idx)
	{
		AcDbObjectIdArray copyids;
		CopyEntitys(ids, copyids);
		saveid.append(copyids);
		MoveEntitys(copyids, xmove * idx, 0.0);
		ads_command(RTSTR, _T("ZOOM"), RTSTR, _T("E"), RTNONE);
	}
	CSaveBlocks blocks; XMJGAssist assist;
	CString path = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("�������ŷֲ�ͼ"));
	isExistFolder(path + name);
	blocks.setPath(path + name);
	blocks.saveBlocks2(saveid, false);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	openDwgCommand(path + name);
}

void XMJGPedestrianBridge::exportPBReport()
{
	SmartTable::Word word;
	openDocTemplate(word);
	replacePictureInfo(word);
	replaceProjectInfomation(word);
	replacePlanningTechnicalInformation(word);
	replaceBuildAreaAndHeightSummarySheet(word);
	replacePlanningConditionVerificationTable(word);
	saveDoc(word);
}

void XMJGPedestrianBridge::modifyPhoteSize()
{
	IDataBaseOper oper; CString layer; double font; float zpsize;
	oper.readCommonTable(_T("��Ƭ�����С"), zpsize);
	oper.readCommonTable(_T("��Ƭ����ͼ��"), layer);
	font = zpsize;
	SelectFilter sf(8, layer); AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T("X"))) return;
	int len = ids.length(); TCHAR info[200] = {0};
	_stprintf(info, _T("\n �����������ע�Ĵ�С:[%.2f] "), font);
	if(RTNORM != ads_getreal(info, &font)) return;
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		AcGePoint3d pos;
		getEntPosition(aname(id), pos);
		GetEntityType(aname(id), info);
		if(_tcscmp(info, _T("TEXT")) == 0 || _tcscmp(info, _T("text")) == 0)
			SetTextHeight(aname(id), m_Scale * font);
		else
			ScaleEntity(aname(id), pos, m_Scale * font);
	}
	zpsize = (float)font;
	oper.writeCommonTable(_T("��Ƭ�����С"), zpsize);
}

void XMJGPedestrianBridge::bridgeSetHeight()
{
	XMJGBridgeAnnotateDlg dlg;
	dlg.DoModal();
}

void XMJGPedestrianBridge::setPBClearWidth()
{
	IProjectMDB pdb; CString width = pdb.getClearWidth();
	VStr widths; AnalyzeData(widths, width, _T("��"));
	CString sWidth(width); int idx = 0, type = 0; CString temp;
	width = _T("\n ��ѡ����Ҫ��ע�ľ���");
	for(; idx < (int)widths.size(); ++idx)
	{
		temp.Format(_T("[%d]"), idx + 1);
		width += temp + widths[idx] + _T("��");
	}
	temp.Format(_T("[%d]����"), idx + 1);
	width += temp; double val = 0.0;
	if(RTNORM != ads_getint(width, &type)) return;
	if(type != (idx + 1)) val = _tstof(widths[type]);
	else
	{
		if(RTNORM != ads_getreal(_T("\n �����뾻��"), &val)) return;
		temp.Format(_T("%.3lf"), val); sWidth = temp;
	}
	VSIter it = find(widths.begin(), widths.end(), temp);
	if(it == widths.end()) pdb.setClearWidth(sWidth);
}

void XMJGPedestrianBridge::DrawControlPoint()
{
	XMJGControlPoint controlpoint;
	controlpoint.DrawControlPoint();
}

void XMJGPedestrianBridge::setPBClearHeight()
{
	ads_point pos; VStr heights; double val = 0.0;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���·��߱�ע��λ�ã�"), pos)) return;
	IProjectMDB pdb; CString height = pdb.getClearHeight(); CString sHeight(height);
	if(false == height.IsEmpty())
	{
		AnalyzeData(heights, height, _T("��"));
		int idx = 0, type = 0; CString temp;
		height = _T("\n ��ѡ����Ҫ��ע�ľ��գ�");
		for(; idx < (int)heights.size(); ++idx)
		{
			temp.Format(_T("[%d]"), idx + 1);
			height += temp + heights[idx] + _T("��");
		}
		temp.Format(_T("[%d]����"), idx + 1);
		height += temp;
		do
		{
			if(RTNORM != ads_getint(height, &type)) return;
		} while(type > (idx + 1));
		if(type == (idx + 1)) sHeight = heights[type];
	}
	else
	{
		if(RTNORM != ads_getreal(_T("\n �����뾻�գ�"), &val)) return;
		sHeight.Format(_T("%.2lf"), val);
	}
	VSIter it = find(heights.begin(), heights.end(), sHeight);
	if(it == heights.end()) pdb.setClearHeight(sHeight);
	height.Format(_T("��������%sm"), sHeight);
	AcDbObjectId id = DrawText(pos, height, _T("MC"), _T(""), 1.0);
	IDataBaseOper oper; TextProperty prop = oper.readTextPropertyTable(_T("�������߱�ע"));
	setlayer(aname(id), prop._layer); setcolor(aname(id), prop._color);
}

void XMJGPedestrianBridge::DrawPhotoPositon()
{
	XMJGTool tool;
	tool.DrawPhotoPositon();
}

void XMJGPedestrianBridge::SetAttatchedInfo()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo6 info(false); info.DoModal();
}

void XMJGPedestrianBridge::ExportApplyTable()
{

}

void XMJGPedestrianBridge::remarkLengthWidth()
{
	int type = 1; CString info;
	if(RTNORM != ads_getint(_T("\n ��ѡ���ע������[1]���ų��ȡ�[2]���ſ�ȣ�"), &type)) return;
	if(type != 1 && type != 2) return;
	ads_point spt, ept, mpt; if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���һ��Ҫ��ע�ĵ㣺"), spt)) return;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��ڶ���Ҫ��ע�ĵ㣺"), ept)) return;
	mpt[X] = (spt[X] + ept[X]) / 2; mpt[Y] = (spt[Y] + ept[Y]) / 2;
	double dist1 = distOF2d(apoint(spt), apoint(ept));
	TCHAR _dist[255] = {0}; _stprintf(_dist, _T("%.2lf"), dist1);
	int old_osmode = PickstyleReset(0);
	ads_command(RTSTR, _T("dimaligned"), RTPOINT, spt, RTPOINT, ept, RTSTR, _T("T"), RTSTR, _dist, RTPOINT, mpt, RTNONE);
	PickstyleBack(old_osmode); acedCommand(RTSTR, _T("Regen"), RTNONE);
	IDataBaseOper oper; TextProperty prop = oper.readTextPropertyTable(_T("���ų����ע"));
	ads_name lastent; ads_entlast(lastent);
	setlayer(lastent, prop._layer);
	setcolor(lastent, prop._color);
	AcDbObjectId id; acdbGetObjectId(id, lastent);
	AcDbEntity *pEnt = NULL;
	if(Acad::eOk != acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite)) return;
	if(pEnt->isKindOf(AcDbDimension::desc()))
	{
		AcDbDimension *pDim = AcDbDimension::cast(pEnt);
		pDim->setDimtxt(prop._size * m_Scale);				//����ע�����ִ�С
	}
	pEnt->close(); IProjectMDB pdb;
	if(type == 1)
	{
		AddXdata(lastent, _T("���ų���"), 0, _dist);
		pdb.setBridgeLength(_dist);
	}
	else if(type == 2)
	{
		AddXdata(lastent, _T("���ſ��"), 0, _dist);
		pdb.setBridgeWidth(_dist);
	}
}

void XMJGPedestrianBridge::statisticAreaInfo()
{
	CString name = GetCurrentDwgName();
	if(name.CompareNoCase(_T("��Ŀͼ��.dwg")) != 0)
	{
		MessageBox(GetForegroundWindow(), _T("���[��Ŀͼ��.dwg]�ļ�"), _T("������Ϣ"), MB_OK);
		return;
	}
	double qxxarea = getScopeOfOwnership();
	double hxarea = getBoundaryLineOfLand();
	double jzmd = 0.0;
	if(hxarea > EPS) jzmd = qxxarea / hxarea * 100;
	MStr record; record[_T("�����õ����")].Format(_T("%.3lf"), qxxarea);
	record[_T("�����ܶ�")].Format(_T("%.2lf"), jzmd);
	IProjectMDB pdb; pdb.setMJXXTableInfo(record);
}

void XMJGPedestrianBridge::setHandlePipeLine()
{
	CSetHandlePipeLineDlg dlg;
	dlg.DoModal();
}

void XMJGPedestrianBridge::saveDoc(Word &word)
{
	XMJGAssist assist; IDataBaseOper oper;
	CString savePath = assist.getExportFilePath();
	CString outname = oper.readFileNameTable(_T("�������ſ�������"));
	savePath += _T("\\") + outname; char savepath[255] = {0}; DeleteFile(savePath);
	TcharToChar(savePath, savepath); word.save(savepath);
	acutPrintf(_T("\n ���������ɡ�·��Ϊ%s"), savePath);
}

void XMJGPedestrianBridge::addPlotRectangle()
{
	ads_name ssSave; AcGePoint2dArray node;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	GetEntsExtent(ssSave, node, 2.0);
	AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T(""));
	setlayer(aname(id), _T("��ӡ��Χ"));
}

void XMJGPedestrianBridge::getReplaceField(MStr &field)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	field[_T("#BuildProject#")] = record[_T("��Ŀ����")];
	field[_T("#BuildCompany#")] = record[_T("���赥λ")];
	return;
}

void XMJGPedestrianBridge::exportVerticalView()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	TCHAR vertical[255] = {0}; XMJGAssist assist;
	if(RTCAN == ads_findfile(_T("������������ͼģ��.dwg"), vertical)) return;
	CString path = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("����������������ͼ"));
	path.Append(_T("\\")); path.Append(name); AcGePoint2d pos(0, 0);
	AcDbObjectId tk = DrawInsert(apoint(pos), _T("������������ͼģ��"));
	SBreakBlock block; block.SetBlock(tk); block.Explode();
	IProjectMDB pdb; MStr jbxx = pdb.getJBXXTableInfo();
	MStr bxx = pdb.getBridgeXX();
	block.replaceText(_T("#PROJECTNAME#"), jbxx[_T("��Ŀ����")], true);
	block.replaceText(_T("#BUILDCOMPANY#"), jbxx[_T("���赥λ")], true);
	block.replaceText(_T("#BRIDGEHEIGHT#"), bxx[_T("����")], true);
	block.replaceText(_T("#BUILDHEIGHT#"), bxx[_T("�������")], true);
	CSaveBlocks blocks; ads_point ptmin, ptmax;
	GetEntsExtent(block.m_objIdArr, ptmin, ptmax, 1.0);
	ZoomWindow(ptmin, ptmax); blocks.setPath(path); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptmin[X], ptmin[Y])); node.append(AcGePoint2d(ptmin[X], ptmax[Y]));
	node.append(AcGePoint2d(ptmax[X], ptmax[Y])); node.append(AcGePoint2d(ptmax[X], ptmin[Y]));
	AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(id), _T("��ӡ��Χ"));
	block.m_objIdArr.append(id); blocks.saveBlocks2(block.m_objIdArr, false);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	openDwgCommand(path);
}

void XMJGPedestrianBridge::remarkLoadLevation()
{
	IProjectMDB pdb; CString load = pdb.getLoadLevation();
	double val = 0.0; ads_point pos; CString sLoad;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���·��߱�ע��λ�ã�"), pos)) return;
	if(load.IsEmpty() == true)
	{
		if(RTNORM != ads_getreal(_T("\n �������ߣ�"), &val)) return;
		sLoad.Format(_T("%.3lf"), val);
	}
	else
	{
		VStr loads; AnalyzeData(loads, load, _T("��"));
		int idx = 0, type = 0; CString temp;
		load = _T("\n ��ѡ����Ҫ��ע�ĵ�·��ߣ�");
		for(; idx < (int)loads.size(); ++idx)
		{
			temp.Format(_T("[%d]"), idx + 1);
			load += temp + loads[idx] + _T("��");
		}
		temp.Format(_T("[%d]����"), idx + 1);
		load += temp; double val = 0.0;
		if(RTNORM != ads_getint(load, &type)) return;
		if(type != (idx + 1)) sLoad = loads[type - 1];
		else
		{
			if(RTNORM != ads_getreal(_T("\n �������ߣ�"), &val)) return;
			temp.Format(_T("%.2lf"), val); sLoad = temp;
		}
	}
	pdb.setLoadLevation(sLoad);
	IDataBaseOper oper; TextProperty prop = oper.readTextPropertyTable(_T("��·���ע��"));
	AcDbObjectId bid = DrawInsert(pos, _T("���1"), m_Scale, m_Scale);
	setlayer(aname(bid), prop._layer); setcolor(aname(bid), prop._color);
	pos[Y] += 1.5 * m_Scale; AcDbObjectId group;
	load.Format(_T("·����%sm"), sLoad);
	AcDbObjectId id = DrawText(pos, load, _T("BL"), _T(""), 1.0);
	setlayer(aname(id), prop._layer); setcolor(aname(id), prop._color);
	CreateGroup(aname(bid), group); AddEntToGroup(id, group);
}

void XMJGPedestrianBridge::remarkBridgeLevation()
{
	IProjectMDB pdb; CString bridge = pdb.getBridgeLevation();
	double val = 0.0; ads_point pos; CString sBridge;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��������ע�ǵ�λ�ã�"), pos)) return;
	if(bridge.IsEmpty() == true)
	{
		if(RTNORM != ads_getreal(_T("\n �������ߣ�"), &val)) return;
		sBridge.Format(_T("%.3lf"), val);
	}
	else
	{
		VStr bridges; AnalyzeData(bridges, bridge, _T("��"));
		int idx = 0, type = 0; CString temp;
		bridge = _T("\n ��ѡ����Ҫ��ע�������ߣ�");
		for(; idx < (int)bridges.size(); ++idx)
		{
			temp.Format(_T("[%d]"), idx + 1);
			bridge += temp + bridges[idx] + _T("��");
		}
		temp.Format(_T("[%d]����"), idx + 1);
		bridge += temp; double val = 0.0;
		if(RTNORM != ads_getint(bridge, &type)) return;
		if(type != (idx + 1)) sBridge = bridges[type - 1];
		else
		{
			if(RTNORM != ads_getreal(_T("\n �������ߣ�"), &val)) return;
			temp.Format(_T("%.2lf"), val); sBridge = temp;
		}
	}
	pdb.setLoadLevation(sBridge);
	IDataBaseOper oper; TextProperty prop = oper.readTextPropertyTable(_T("������ע��"));
	AcDbObjectId bid = DrawInsert(pos, _T("���2"), m_Scale, m_Scale);
	setlayer(aname(bid), prop._layer); setcolor(aname(bid), prop._color);
	pos[Y] += 1.5 * m_Scale; AcDbObjectId group;
	bridge.Format(_T("������%sm"), sBridge);
	AcDbObjectId id = DrawText(pos, bridge, _T("BL"), _T(""), 1.0);
	setlayer(aname(id), prop._layer); setcolor(aname(id), prop._color);
	CreateGroup(aname(bid), group); AddEntToGroup(id, group);
}

void XMJGPedestrianBridge::ExportDWGPBElevation()
{
	AcDbObjectIdArray IdArr;
	if(false == SelectEntitys(IdArr, _T(""))) return;
	CFileDialog dlg(FALSE, _T("��ѡ�񱣴�·��"));
	dlg.m_ofn.lpstrInitialDir = m_SaveDir;
	if(IDOK != dlg.DoModal())	return;
	XMJGExport exportdwg;
	exportdwg.setSavePath(dlg.GetPathName());
	exportdwg.setObjects(IdArr);
	exportdwg.setPictureFrame(_T("XMJG����ͼ"));
	exportdwg.ExportDwg(); return;



	/*
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcGePoint2d pos(0, 0);
	AcDbObjectId objFrame = DrawInsert(apoint(pos), _T("XMJG����ͼ"));
	AcGePoint3d entMid = GetMiddlePoint(apoint(lb), apoint(rt));

	SBreakBlock blockB;	blockB.SetBlock(objFrame); blockB.Explode();
	AcGePoint3d  ptLB, ptRT; AcDbObjectId lbid, rtid;
	blockB.findPointByHyperLink(_T("���½ǵ�"), ptLB, lbid);
	blockB.findPointByHyperLink(_T("���Ͻǵ�"), ptRT, rtid);
	EntDel(aname(lbid)); EntDel(aname(rtid));

	double dscale = assist.GetScale(apoint(ptLB), apoint(ptRT), apoint(lb), apoint(rt));
	ScaleEntitys(IdArr, entMid, dscale);	TCHAR bl[200] = {0};
	_stprintf(bl, _T("1:%d"), (int)(1000 / dscale));
	blockB.replaceTextEx(_T("#Scale#"), bl);
	AcGePoint3d tkMid = GetMiddlePoint(ptLB, ptRT);
	double dXMove = tkMid.x - entMid.x;
	double dYMove = tkMid.y - entMid.y;
	MoveEntitys(IdArr, dXMove, dYMove);
	assist.SaveEntitysToDWG();
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);

	*/

}

void XMJGPedestrianBridge::changePicturePropery()
{
	IDataBaseOper db;
	CString path = db.readPathTable(_T("��ǰĿ¼"));
	XMJGChangePictureProperty dlg(path);
	dlg.setPicturePropertyType(_T("����ͼƬ�����ʽ"));
	dlg.DoModal();
}

void XMJGPedestrianBridge::AnnotationBoundaries()
{
	SelectFilter sf1(8, _T("Ȩ���߽�")), sf2(RTDXF0, _T("*polyline"));
	AcDbObjectId id;
	if(!SelectEntity(id, sf1, sf2, NULL)) return;
}

void XMJGPedestrianBridge::remarkBridgeThickness()
{
	double thickness = 0.0; ads_point spt, ept, mpt;
	if(RTNORM != ads_getreal(_T("\n �����������ȣ�"), &thickness)) return;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��Ҫ��ע�����ȵĵ�һ�㣺"), spt)) return;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��Ҫ��ע�����ȵĵڶ��㣺"), ept)) return;
	int old_osmode = PickstyleReset(0); CString _dist; _dist.Format(_T("%.2lf"), thickness);
	mpt[X] = (spt[X] + ept[X]) / 2; mpt[Y] = (spt[Y] + ept[Y]) / 2;
	ads_command(RTSTR, _T("dimaligned"), RTPOINT, spt, RTPOINT, ept, RTSTR, _T("T"), RTSTR, _dist, RTPOINT, mpt, RTNONE);
	PickstyleBack(old_osmode); ads_name ent; ads_entlast(ent);
	acedCommand(RTSTR, _T("Regen"), RTNONE);
	IDataBaseOper oper; TextProperty prop = oper.readTextPropertyTable(_T("���ź��ע��"));
	setlayer(ent, prop._layer); setcolor(ent, prop._color);
	IProjectMDB pdb; pdb.setBridgeThickness(_dist);
}

void XMJGPedestrianBridge::remarkBridgeBalustrade()
{
	double thickness = 0.0; ads_point spt, ept, mpt;
	if(RTNORM != ads_getreal(_T("\n �������������ˣ�"), &thickness)) return;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��Ҫ��ע���˸߶ȵĵ�һ�㣺"), spt)) return;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��Ҫ��ע���˸߶ȵĵڶ��㣺"), ept)) return;
	int old_osmode = PickstyleReset(0); CString _dist; _dist.Format(_T("%.2lf"), thickness);
	mpt[X] = (spt[X] + ept[X]) / 2; mpt[Y] = (spt[Y] + ept[Y]) / 2;
	ads_command(RTSTR, _T("dimaligned"), RTPOINT, spt, RTPOINT, ept, RTSTR, _T("T"), RTSTR, _dist, RTPOINT, mpt, RTNONE);
	PickstyleBack(old_osmode); ads_name ent; ads_entlast(ent);
	acedCommand(RTSTR, _T("Regen"), RTNONE);
	IDataBaseOper oper; TextProperty prop = oper.readTextPropertyTable(_T("��������ע��"));
	setlayer(ent, prop._layer); setcolor(ent, prop._color);
	IProjectMDB pdb; pdb.setBridgeBalustrade(_dist);
}

double XMJGPedestrianBridge::getScopeOfOwnership()
{
	AcDbObjectIdArray ids; double area = 0.0;
	SelectFilter sf(8, _T("Ȩ���߽�"));
	if(false == SelectEntitys(ids, sf, _T("X"))) return area;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		double temp = 0.0;
		GetEntArea(aname(id), temp);
		area += temp;
	}
	return area;
}

void XMJGPedestrianBridge::setChannelBodyPosition()
{
	ads_point spt, ept;
	if(ads_getpoint(NULL, _T("\n ��ѡ��ͨ��������㣺"), spt) != RTNORM) return;
	if(ads_getpoint(NULL, _T("\n ��ѡ��ͨ�������յ㣺"), ept) != RTNORM) return;
	MStr record; record[_T("ͨ���������X")].Format(_T("%.3lf"), spt[X]);
	record[_T("ͨ���������Y")].Format(_T("%.3lf"), spt[Y]);
	record[_T("ͨ�������յ�X")].Format(_T("%.3lf"), ept[X]);
	record[_T("ͨ�������յ�Y")].Format(_T("%.3lf"), ept[Y]);
	IProjectMDB pdb; pdb.setChannelBodyPosition(record);
}

void XMJGPedestrianBridge::exportPedestrianBridge()
{
	IDataBaseOper oper;
	CString tempname = oper.readFileNameTable(_T("�������������ܱ�ģ��"));
	SmartTable::Word word; TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(tempname, path))
	{
		TCHAR info[255] = {0};
		_stprintf(info, _T("\n %s"), tempname);
		acutPrintf(info);
		return;
	}
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath);
	replaceWordField(word);
	XMJGAssist assist; MSBuild buildinfo;
	assist.readTableData(buildinfo);
	assist.exportTable2Doc(word, buildinfo);
	CString outname = oper.readFileNameTable(_T("�������������ܱ�"));
	CString savePath = oper.readPathTableEx(_T("��������"));
	savePath += _T("\\") + outname;
	char savepath[255] = {0}; TcharToChar(savePath, savepath);
	word.save(savepath);
	acutPrintf(_T("\n ���������ɡ�·��Ϊ%s"), savePath);
}

void XMJGPedestrianBridge::DrawOwnershipBoundaries()
{
	acutPrintf(_T("\n �������ű߽�"));
	AcDbObjectId lineID = drawLine();
	setlayer(aname(lineID), m_qbjx._layer);
	setcolor(aname(lineID), m_qbjx._color);
	SetPolyWidth(aname(lineID), m_Scale * m_qbjx._size);
}

double XMJGPedestrianBridge::getBoundaryLineOfLand()
{
	AcDbObjectIdArray ids; double area = 0.0;
	SelectFilter sf(8, _T("����"));
	if(false == SelectEntitys(ids, sf, _T("X"))) return area;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		double temp = 0.0;
		GetEntArea(aname(id), temp);
		area += temp;
	}
	return area;
}

bool XMJGPedestrianBridge::openDocTemplate(Word &word)
{
	XMJGAssist assist; TCHAR path[255] = {0};
	CString savePath = assist.getExportFilePath();
	if(RTNORM != ads_findfile(_T("�������ſ������ձ�����.docx"), path))
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ�ģ���ļ�[�������ſ������ձ�����.docx]"), _T("������Ϣ"), MB_OK);
		return false;
	}
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath);
	return ret == 1;
}

void XMJGPedestrianBridge::replaceWordField(Word &word)
{
	IProjectMDB pdb;
	VMStr records = pdb.getPictureTable();
	int len = (int)records.size();
	char info[255] = {0};
	SmartTable::WordTable table;
	int tableindex = word.findTable(0, 0, "#Format1#", table);
	int index = word.setPropertyTable(tableindex);
	for(int idx = 0; idx < len; ++idx)
	{
		PicInfo picinfo;
		MStr record = records.at(idx);
		CString path = record[_T("·��")];
		CString local = record[_T("λ��")];
		CString descr = record[_T("����")];
		TcharToChar(local, info);
		word.setCurrentInsert(info);
		TcharToChar(path, info);
		word.appendImage(info, 14.5, 10.88, SmartTable::Alignment_MC);
		TcharToChar(descr, info);
		word.appendParagraph(info, index);
	}
}

void XMJGPedestrianBridge::replacePictureInfo(Word &word)
{
	IProjectMDB pdb; VStr locals;
	VMStr records = pdb.getPictureTable();
	int len = (int)records.size();
	SmartTable::WordTable table; char info[255] = {0};
	int tableindex = word.findTable(1, 1, "#Format1#", table);
	int index = word.setPropertyTable(tableindex);
	for(int idx = 0; idx < len; ++idx)
	{
		MStr record = records.at(idx);
		CString path(record[_T("·��")]);
		CString local(record[_T("λ��")]);
		VSIter it = find(locals.begin(), locals.end(), local);
		if(it == locals.end()) locals.push_back(local);
		CString descr(record[_T("����")]);
		TcharToChar(local, info);
		word.setCurrentInsert(info);
		TcharToChar(path, info);
		int width = 0, height = 0;
		getImagePngSize(info, width, height);
		if(width < 3000 && height < 3000)
			word.appendImage(info, 14.5, 10.88, SmartTable::Alignment_MC);
		else if(width < 4000 && height < 4000)
			word.appendImage(info, 16.18, 22.51, SmartTable::Alignment_MC);
		else
			word.appendImage(info, 34.94, 22.74, SmartTable::Alignment_MC);
		TcharToChar(descr, info);
		word.appendParagraph(info, 0);
	}
	for(int idx = 0; idx < (int)locals.size(); ++idx)
	{
		TcharToChar(locals[idx], info);
		word.replaceText(info, "");
	}
	word.removeTable(tableindex);
}

void XMJGPedestrianBridge::ExportDWGPBStratificationPlan()
{
	AcDbObjectIdArray IdArr; XMJGAssist assist;
	acutPrintf(_T("\n��ѡ�����ű߽��"));
	if(false == SelectEntitys(IdArr, _T(""))) return;
	TCHAR info[144] = {0};	AcDbObjectIdArray tqbjk;
	for(int idx = 0; idx < IdArr.length(); ++idx)
	{
		AcDbObjectId id = IdArr.at(idx);
		if(false == ReadXdata(aname(id), _T("TQBJK"), 0, info)) continue;
		tqbjk.append(id);
	}
	if(tqbjk.length() == 0)
	{
		MessageBox(GetForegroundWindow(), _T("ͼ�����޷��ҵ����ű߽��"), _T("������Ϣ"), MB_OK);
		return;
	}
	CString name = assist.getExportFileName(_T("�������ŷֲ�ƽ��ͼ"));
	CString path = assist.getExportFilePath();
	CString dir; dir.Format(_T("%s%s"), path, name);
	isExistFolder(dir); VStr outPaths;
	for(int idx = 0; idx < tqbjk.length(); ++idx)
	{
		acDocManager->lockDocument(acDocManager->curDocument());
		XMJGExport exportdwg; AcDbObjectIdArray ids;
		AcDbObjectId id = tqbjk.at(idx);
		ids = getInnerObjects(id);
		MStr record; getReplaceField(record);
		for(int indx = 0; indx < ids.length(); ++indx)
		{
			TCHAR lyr[255] = {0};
			AcDbObjectId tid = ids[indx];
			GetEntLayer(tid, lyr); double area = 0.0;
			if(m_qbjx._layer.CompareNoCase(lyr)) continue;
			GetEntArea(aname(tid), area);
			record[_T("#BuildArea#")].Format(_T("%.2f"), area);
			break;
		}

		name.Format(_T("%s_%d.dwg"), dir, idx);
		exportdwg.setSavePath(name);
		exportdwg.setObjects(ids);
		exportdwg.setPictureFrame(_T("�������ŷֲ�ƽ��ͼ"));
		exportdwg.ExportDwg(record, XMJGReplaceField);
		outPaths.push_back(name);
		acDocManager->unlockDocument(acDocManager->curDocument());
	}
	AcApDocument *pDoc = NULL;
	for(int idx = 0; idx < (int)outPaths.size(); ++idx)
	{
		CString pathName = outPaths[idx];
		if(acDocManager->isApplicationContext())
			acDocManager->appContextOpenDocument(pathName);
	}
	acutPrintf(_T("\n�ɹ����[�������ŷֲ�ƽ��ͼ], ���·��Ϊ��%s\\CG\\��������"), path);
	return;
}

void XMJGPedestrianBridge::ControlPointAndRoadPicturePosition()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	TCHAR value[1024] = {0};
	assist.QueryRegisterValue(HKEY_CURRENT_USER, XMJGPB, _T("ProjectName"), value);
	assist.DrawMesh(lb, rt, ZhaoPianShiYiTu);
	MVStr filter; VStr vals;

	vals.push_back(_T("��ע")); //vals.push_back(_T("����"));
	filter[_T("ͼ��")] = vals;
	assist.FilterEntitys(IdArr, filter);
	addPlotRectangle();
	CString path = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("����������Ƭλ��ʾ��ͼ"));
	path += _T("\\") + name;
	bool bSuc = isExistFolder(path);
	assist.SaveEntitysToDWG(path);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	openDwgCommand(path);
}

void XMJGPedestrianBridge::replaceProjectInfomation(Word &word)
{
	IProjectMDB pdb; MStr jbxx = pdb.getJBXXTableInfo(); string info;
	splitBigString(jbxx[_T("ί�е�λ")], info); word.replaceText("#TQBuildCompany#", info.c_str());
	info.clear(); splitBigString(jbxx[_T("��������")], info); word.replaceText("#TQXS#", info.c_str());
	info.clear(); splitBigString(jbxx[_T("�����")], info); word.replaceText("#TQAcceptCode#", info.c_str());
	info.clear(); splitBigString(jbxx[_T("��Ŀ����")], info); word.replaceText("#TQProjectName#", info.c_str());
	info.clear(); splitBigString(jbxx[_T("��Ŀ�ص�")], info); word.replaceText("#TQProjectAddress#", info.c_str());
	info.clear(); splitBigString(jbxx[_T("������λ")], info); word.replaceText("#TQMeansureCompany#", info.c_str());
	time_t timep; time(&timep); char tmp[64]; strftime(tmp, sizeof(tmp), "20%y��%m��%d��", localtime(&timep));
	word.replaceText("#TQDate#", tmp);
}

void XMJGPedestrianBridge::ExportBuildAreaHeightAndBirdgeHeight()
{
	CTQZBINFO dlg;	IProjectMDB mdb;
	VMStr infos = mdb.readRXBridgeTableInfo();
	dlg.setDialogInfo(infos); infos.clear();
	if(IDOK != dlg.DoModal()) return;
	MVSIter it = dlg.mData.begin();
	for(; it != dlg.mData.end(); ++it)
	{
		CString pName = it->first;
		VStr heights = it->second; CString height;
		MStr record; record[_T("��Ŀ����")] = pName;
		record[_T("��Ҫָ��")] = dlg.m_strZYZB;
		record[_T("������")] = dlg.m_Pro_BridgeWidth[pName];
		record[_T("��ע")] = dlg.m_Pro_Remarks[pName];
		for(int idx = 0; idx < (int)heights.size(); ++idx)
		{
			if(heights[idx].GetLength() == 0) continue;
			height += heights[idx] + _T("|");
		}
		record[_T("��������")] = height; infos.push_back(record);
	}
	// 	bool bSuc = false; Excel excel; TCHAR path[255] = {0};
	// 	char filepath[1024] = {0}, cValue[255] = {0}, cPath[255] = {0};
	// 	if(RTNORM != ads_findfile(_T("��������������߶ȼ������߻��ܱ�.xlsx"), path)) return;
	// 	TcharToChar(path, filepath);
	// 	excel.setEncoding(SmartTable::Encoding_GBK);
	// 	int ret = excel.loadExcel(filepath);
	// 	 MStr mstr1;18720935541
	// 	CString strTemp = _T("");
	// 	mstr1 = mdb.getJBXXTableInfo();
	// 	strTemp = mstr1[_T("���赥λ")];
	// 	TcharToChar(strTemp, cValue);
	// 	excel.replaceText("#BuildCompany#", cValue);
	// 	strTemp = mstr1[_T("��Ŀ����")];
	// 	TcharToChar(strTemp, cValue);
	// 	excel.replaceText("#ProjectName#", cValue);
	// 	excel.replaceText("#ZYZB#", cValue);
	// 	ExcelSheet sheet;
	// 	int es = excel.getSheetByName("Sheet1", sheet);

	// 	VMStr vInfos;
	// 	map<CString, VStr>::iterator it1;
	// 	it1 = dlg.mData.end(); //���淴�����룬Ϊ�����������߷������
	// 	it1--;
	// 
	// 	for(int i = 0; i < (int)dlg.mData.size(); ++i)//-1��ȥ������������
	// 	{
	// 		MStr mInfos;
	// 		//mInfos[_T("��Ҫָ��")] = strZYZB;
	// 		CString strQLJG = _T("");
	// 		int nCol = it1->second.size();
	// 		bSuc = sheet.copyRow(11, 0, nCol);
	// 		TcharToChar(it1->first, cValue);
	// 		bSuc = sheet.setCellValueText(12, 1, cValue);
	// 		mInfos[_T("��Ŀ����")] = it1->first;
	// 		strTemp = dlg.m_Pro_BridgeWidth[it1->first];
	// 		TcharToChar(strTemp, cValue);
	// 		bSuc = sheet.setCellValueText(12, 5, cValue);
	// 		//mInfos[_T("������")] = dlg.m_strBridgeWidth;
	// 		strTemp = dlg.m_Pro_Remarks[it1->first];
	// 		TcharToChar(strTemp, cValue);
	// 		bSuc = sheet.setCellValueText(12, 7, cValue);
	// 		//mInfos[_T("��ע")] = dlg.m_strRemarks;
	// 		for(int j = 0; j < nCol; ++j)
	// 		{
	// 			TcharToChar(it1->second.at(j), cValue);
	// 			bSuc = sheet.setCellValueText(12 + j, 3, cValue);
	// 			sheet.merge(12 + j, 3, 12 + j, 4);
	// 			strQLJG += it1->second.at(j) + _T("|");
	// 		}
	// 		mInfos[_T("��������")] = strQLJG;
	// 		sheet.merge(12, 1, 12 + nCol - 1, 2);
	// 		sheet.merge(12, 5, 12 + nCol - 1, 6);
	// 		sheet.merge(12, 7, 12 + nCol - 1, 7);
	// 		it1--;	vInfos.push_back(mInfos);
	// 
	// 	}
	mdb.setRXBridgeTableInfo(infos);
	// 
	// 	sheet.removeRow(11, true);
	// 	XMJGAssist assist;
	// 	CString savePath = assist.getExportFilePath();
	// 	CString name = assist.getExportFileName(_T("��������������߶ȼ������߻��ܱ�"));
	// 	savePath += _T("\\") + name; TcharToChar(savePath, cPath);
	// 	bSuc = excel.save(cPath);
}

void XMJGPedestrianBridge::compressFile()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg(_T("��������"));
	dlg.DoModal();
}

void XMJGPedestrianBridge::replacePlanningTechnicalInformation(Word &word)
{
	IProjectMDB pdb; MStr jssm = pdb.getJSSMTableInfo(); string info;
	splitBigString(jssm[_T("���ɹ����ݵ��������")], info); word.replaceText("#TQLicenseDoc#", info.c_str());
	info.clear(); splitBigString(jssm[_T("�����������")], info); word.replaceText("#TQYYZL#", info.c_str());
	info.clear(); splitBigString(jssm[_T("ͼ���������")], info); word.replaceText("#TQBasePoint#", info.c_str());
	info.clear(); splitBigString(jssm[_T("���β���")], info); word.replaceText("#TQDXCL#", info.c_str());
	info.clear(); splitBigString(jssm[_T("���Ų���")], info); word.replaceText("#TQCL#", info.c_str());
	info.clear(); splitBigString(jssm[_T("��滮��ɲ�һ�����")], info); word.replaceText("#TQBYZQK#", info.c_str());
	info.clear(); splitBigString(jssm[_T("������Ҫ˵�������")], info); word.replaceText("#TQSM#", info.c_str());
}

void XMJGPedestrianBridge::replaceBuildAreaAndHeightSummarySheet(Word &word)
{
	IProjectMDB pdb; string info; CString value; double ch = 0.0; double max = 0.0;
	value = pdb.getClearHeight(); VStr chs; AnalyzeData(chs, value, _T("��"));
	for(int idx = 0; idx < (int)chs.size(); ++idx)
	{
		double temp = _tstof(chs[idx]);
		ch += max > temp ? max : temp;
	}
	value = pdb.getBridgeThickness(); ch += _tstof(value);
	value = pdb.getBridgeBalustrade(); ch += _tstof(value);
	value.Format(_T("%.3lf"), ch); splitBigString(value, info); word.replaceText("#TQGD#", info.c_str()); info.clear();
	value = pdb.getLoadLevation(); splitBigString(value, info); word.replaceText("#TQDPG#", info.c_str()); info.clear();
	int pos = value.Find(_T("��"));
	if(pos == -1) ch += _tstof(value);
	else ch += _tstof(value.Mid(0, pos));
	value.Format(_T("%.2lf"), ch);
	splitBigString(value, info); word.replaceText("#TQLGG#", info.c_str()); info.clear();
}

void XMJGPedestrianBridge::replacePlanningConditionVerificationTable(Word &word)
{
	IProjectMDB pdb; string info; CString value; MStr record = pdb.getMJXXTableInfo();
	value = pdb.getChannelBodyPosition(_T("ͨ���������X")); splitBigString(value, info); word.replaceText("#STARTPOINTX#", info.c_str()); info.clear();
	value = pdb.getChannelBodyPosition(_T("ͨ���������Y")); splitBigString(value, info); word.replaceText("#STARTPOINTY#", info.c_str()); info.clear();
	value = pdb.getChannelBodyPosition(_T("ͨ�������յ�X")); splitBigString(value, info); word.replaceText("#ENDPOINTX#", info.c_str()); info.clear();
	value = pdb.getChannelBodyPosition(_T("ͨ�������յ�Y")); splitBigString(value, info); word.replaceText("#ENDPOINTY#", info.c_str()); info.clear();
	splitBigString(record[_T("�����õ����")], info); word.replaceText("#TQJZMJ#", info.c_str()); double jzmj = _tstof(record[_T("�����õ����")]);
	double jzmd = _tstof(record[_T("�����ܶ�")]); double ydmj = jzmj / jzmd * 100; value.Format(_T("%.3lf"), ydmj); info.clear();
	splitBigString(value, info); word.replaceText("#TQYDMJ#", info.c_str()); info.clear();
	splitBigString(record[_T("�����ܶ�")], info); word.replaceText("#TQJZMD#", info.c_str());
	value = pdb.getClearWidth(); splitBigString(value, info); word.replaceText("#TQJK#", info.c_str()); info.clear();
	value = pdb.getClearHeight(); splitBigString(value, info); word.replaceText("#TQJG#", info.c_str()); info.clear();
	value = pdb.getBridgeWidth(); splitBigString(value, info); word.replaceText("#TQKD#", info.c_str()); info.clear();
	value = pdb.getBridgeLength(); splitBigString(value, info); word.replaceText("#TQCD#", info.c_str()); info.clear();
}

#pragma endregion

#pragma region ��·����

XMJGRoadAndPipeLine::XMJGRoadAndPipeLine()
{
	XMJGAssist assist; assist.ReadRegForContext(_T("��·����"), m_SaveDir);
}

XMJGRoadAndPipeLine::~XMJGRoadAndPipeLine()
{

}

void XMJGRoadAndPipeLine::DrawProjectScopeOfLine()
{

}

void XMJGRoadAndPipeLine::DrawControlPoint()
{
	XMJGControlPoint controlpoint;
	controlpoint.DrawControlPoint();
}

void XMJGRoadAndPipeLine::DrawPhotoPositon()
{
	XMJGTool tool;
	tool.DrawPhotoPositon();
}

void XMJGRoadAndPipeLine::ExportResults2Dwg()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	TCHAR value[1024] = {0};
	assist.QueryRegisterValue(HKEY_CURRENT_USER, XMJGPB, _T("ProjectName"), value);
	//assist.DrawMesh(lb, rt, value); 
	MVStr filter; VStr vals;
	vals.push_back(_T("ͼƬ")); vals.push_back(_T("���Ƶ�"));
	filter[_T("ͼ��")] = vals;
	assist.FilterEntitys(IdArr, filter);
	assist.SaveEntitysToDWG(m_SaveDir);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
}

void XMJGRoadAndPipeLine::ExportTotalFlatFigure()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	TCHAR value[1024] = {0};
	assist.QueryRegisterValue(HKEY_CURRENT_USER, XMJGPB, _T("ProjectName"), value);
	//assist.DrawMesh(lb, rt, value);
	MVStr filter; VStr vals;
	vals.push_back(_T("��ע")); vals.push_back(_T("����"));
	filter[_T("ͼ��")] = vals;
	assist.FilterEntitys(IdArr, filter);
	assist.SaveEntitysToDWG(m_SaveDir);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
}

void XMJGRoadAndPipeLine::ExportLongitudinalSectionFigure()
{

}

void XMJGRoadAndPipeLine::ExportRoadCrossSectionalComparisonChart()
{

}

void XMJGRoadAndPipeLine::RoadTechnologySummary()
{

}

void XMJGRoadAndPipeLine::ExportPipeLineTotalFlatFigure()
{

}

void XMJGRoadAndPipeLine::ExportPipeLineResultTable()
{

}

void XMJGRoadAndPipeLine::ExportPipeLineDesignAndMeasuredComparisonTable()
{

}

void XMJGRoadAndPipeLine::ExportPipeComparisonFigure()
{

}

void XMJGRoadAndPipeLine::ExportPipeLinecomprehensiveTotalFlatFigure()
{

}

void XMJGRoadAndPipeLine::ExportPipeLineCrossSectionalDwg()
{

}

void XMJGRoadAndPipeLine::PipelineTechnicalSummary()
{

}

void XMJGRoadAndPipeLine::MeasuredResultsTable()
{

}

void XMJGRoadAndPipeLine::TopographicBase()
{

}

#pragma endregion

#pragma region ��·

XMJGRoadNoPipeLine::XMJGRoadNoPipeLine()
{
	XMJGAssist assist;
	assist.ReadRegForContext(_T("��·"), m_SaveDir);
}

XMJGRoadNoPipeLine::~XMJGRoadNoPipeLine()
{

}

void XMJGRoadNoPipeLine::ExportControlPointAndRoadPicture()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	TCHAR value[1024] = {0};
	assist.QueryRegisterValue(HKEY_CURRENT_USER, XMJGPB, _T("ProjectName"), value);
	//assist.DrawMesh(lb, rt, value);
	MVStr filter; VStr vals;
	vals.push_back(_T("��ע")); vals.push_back(_T("����"));
	filter[_T("ͼ��")] = vals;
	assist.FilterEntitys(IdArr, filter);
	assist.SaveEntitysToDWG(m_SaveDir);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
}

void XMJGRoadNoPipeLine::ExportRoadComprehensiveCrossSectional()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	TCHAR value[1024] = {0};
	assist.QueryRegisterValue(HKEY_CURRENT_USER, XMJGPB, _T("ProjectName"), value);
	//assist.DrawMesh(lb, rt, value); 
	MVStr filter; VStr vals;
	vals.push_back(_T("��ע")); vals.push_back(_T("����"));
	filter[_T("ͼ��")] = vals;
	assist.FilterEntitys(IdArr, filter);
	assist.SaveEntitysToDWG(m_SaveDir);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
}

void XMJGRoadNoPipeLine::ExportResultsContrastTechnicalSummary()
{

}

void XMJGRoadNoPipeLine::ExportRoadCrossSectional()
{
	AcDbObjectIdArray IdArr;
	if(false == SelectEntitys(IdArr, _T(""))) return;
	CFileDialog dlg(FALSE, _T("��ѡ�񱣴�·��"));
	dlg.m_ofn.lpstrInitialDir = m_SaveDir;
	if(IDOK != dlg.DoModal())	return;
	XMJGExport exportdwg;
	exportdwg.setSavePath(dlg.GetPathName());
	exportdwg.setObjects(IdArr); int exporttype = 0;
	exportdwg.setPictureFrame(_T("XMJG�ֲ�ƽ��ͼ"));
	ads_getint(_T("\n �����ʽ<0>dwg <1>jpg��"), &exporttype);
	if(exporttype == 0)	exportdwg.ExportDwg();
	else exportdwg.ExportPicture(true);
}

void XMJGRoadNoPipeLine::ExportRoadProfile()
{
	TCHAR szStr[100], szFileName[MAX_PATH], *ppp;
	FILE *fp; AcGePoint2d pt2d; ads_point pt0;
	double dLicheng = -100.0, dLastLicheng = -100.0;
	int nSum = 0;
	ads_command(RTSTR, _T("CMDECHO"), RTSTR, _T("0"), RTNONE);
	if(GetCassFileName(_T("���������������ļ���"), _T(""), _T("hdm"), 0, szFileName) == false) return;
	if((fp = _tfopen(szFileName, _T("rt"))) == NULL)
	{
		ads_alert(_T("�޷����ļ�!")); return;
	}
	CAcModuleResourceOverride myResources;
	CDrawSection dmt;
	dmt.m_bAskJiange = TRUE;
	if(dmt.DoModal() != IDOK) return;
	pt0[X] = dmt.m_PlaceX; pt0[Y] = dmt.m_PlaceY; pt0[Z] = 0.0;

	while(1)
	{
		if(false == dmt.m_Nodes.isEmpty())
			dmt.m_Nodes.removeSubArray(0, dmt.m_Nodes.length());
		if(false == dmt.m_NextNodes.isEmpty())
			dmt.m_NextNodes.removeSubArray(0, dmt.m_NextNodes.length());
		bool bBegin = true;
		while(1)
		{
			if(NULL == _fgetts(szStr, 100, fp))
			{
				dLastLicheng = dLicheng;
				break;
			}
			if(!_tcsnicmp(szStr, _T("BEGIN"), 5))
			{
				dLastLicheng = dLicheng;
				dLicheng = -100.0;
				ppp = _tcschr(szStr, ',');
				if(NULL != ppp) dLicheng = _tstof(ppp + 1);
				if(dmt.m_Nodes.length() > 0) break;
				else continue;
			}
			if(!_tcsnicmp(szStr, _T("NEXT"), 4))
			{//����ߣ��ܴ�����next,2004.12.17
				if(false == bBegin)
				{
					pt2d.x = -1500000; pt2d.y = -1500000;
					dmt.m_NextNodes.append(pt2d);
				}
				else
					bBegin = false;
				continue;
			}
			pt2d[X] = _tstof(szStr);
			ppp = _tcschr(szStr, '='); //�ɵ�һ�ְ汾��ʽ
			if(NULL == ppp)
			{
				ppp = _tcschr(szStr, ',');
				if(NULL == ppp) continue;
			}
			ppp++; pt2d[Y] = _tstof(ppp);
			if(true == bBegin) dmt.m_Nodes.append(pt2d);
			else dmt.m_NextNodes.append(pt2d);
		}

		if(dmt.m_Nodes.length() >= 2)
		{
			dmt.m_PlaceX = pt0[X] + (nSum / dmt.m_jgnum) * dmt.m_jgx;
			dmt.m_PlaceY = pt0[Y] - (nSum % dmt.m_jgnum) * dmt.m_jgy;
			dmt.m_dLicheng = dLastLicheng;
			dmt.draw();
			nSum++;
		}
		else break;
	}
	fclose(fp);
	/*AcDbObjectIdArray IdArr;
	if(false == SelectEntitys(IdArr, NULL)) return;
	CFileDialog *dlg = new CFileDialog(FALSE, _T("��ѡ�񱣴�·��"));
	if(IDOK != dlg->DoModal())	return;
	XMJGExport exportdwg;	exportdwg.setObjects(IdArr);
	exportdwg.setSavePath(dlg->GetPathName());
	exportdwg.setObjects(IdArr); int exporttype = 0;
	exportdwg.setPictureFrame(_T("XMJG�ֲ�ƽ��ͼ"));
	ads_getint(_T("\n �����ʽ<0>dwg <1>jpg��"), &exporttype);
	if(exporttype == 0)	exportdwg.ExportDwg();
	else exportdwg.ExportPicture(true);*/
}

void XMJGRoadNoPipeLine::ExportRoadStratificationPlan()
{
	AcDbObjectIdArray IdArr;
	if(false == SelectEntitys(IdArr, _T(""))) return;
	CFileDialog dlg(FALSE, _T("��ѡ�񱣴�·��"));
	dlg.m_ofn.lpstrInitialDir = m_SaveDir;
	if(IDOK != dlg.DoModal())	return;
	XMJGExport exportdwg;
	exportdwg.setSavePath(dlg.GetPathName());
	exportdwg.setObjects(IdArr); int exporttype = 0;
	exportdwg.setPictureFrame(_T("XMJG�ֲ�ƽ��ͼ"));
	ads_getint(_T("\n �����ʽ<0>dwg <1>jpg��"), &exporttype);
	if(exporttype == 0)	exportdwg.ExportDwg();
	else exportdwg.ExportPicture(true);
}

#pragma endregion

#pragma region ����

XMJGHouse::XMJGHouse()
{
	int idx = -10; m_Scale = 500;
	read1(m_Scale); m_Scale /= 1000; m_isNotNormalFloor = 199;
	XMJGAssist assist; assist.ReadRegForContext(_T("���ݿ���"), m_SaveDir);
	IDataBaseOper oper;
	m_cw = oper.readTextPropery(_T("����������λ"));
	m_bg = oper.readTextPropery(_T("�����������"));								//������
	m_bgzj = oper.readTextPropery(_T("�����������ע��"), true);					//���+���
	m_hxjd = oper.readTextPropery(_T("�����������߽ڵ�ע��"), true);				//���߽ڵ�
	m_fcmjzs = oper.readTextPropery(_T("���������ֲ����ע��"), true);
	m_bgxxbz = oper.readTextPropery(_T("�������������Ϣ��ע"), true, true);		//ͼ�����½�
	m_dgzzbz = oper.readTextPropery(_T("�������������ע"), true);
	m_hxzbbz = oper.readTextPropery(_T("�������������ע"), true);
	m_dxfwx = oper.readTextPropery(_T("�����������·�Χ��"), true, true);
	oper.readCommonTable(_T("��������������ͼ��"), m_gnq._layer);
	oper.readCommonTable(_T("����������������ɫ"), m_gnq._color);
	oper.readCommonTable(_T("����������λע�Ǵ�С"), m_cwzj._size);
	oper.readCommonTable(_T("����������λע��ͼ��"), m_cwzj._layer);
	oper.readCommonTable(_T("����������λע����ɫ"), m_cwzj._color);
	oper.readCommonTable(_T("���������ֲ�ƽ��ͼͼ��"), m_fcpm._layer);
	oper.readCommonTable(_T("���������ֲ�ƽ��ͼ��ɫ"), m_fcpm._color);
	oper.readCommonTable(_T("������������������ͼ��"), m_jzwlk._layer);
	oper.readCommonTable(_T("��������������������ɫ"), m_jzwlk._color);
	oper.readCommonTable(_T("���������������ע�Ǵ�С"), m_jzbg._size);
	oper.readCommonTable(_T("���������������ע������"), m_jzbg._font);
	oper.readCommonTable(_T("���������������ע����ɫ"), m_jzbg._color);
	oper.readCommonTable(_T("���������������ע��ͼ��"), m_jzbg._layer);
	oper.readCommonTable(_T("�����������ݱ߳�ע�Ǵ�С"), m_fwbczj._size);
	oper.readCommonTable(_T("������������ͼ�����ƴ�С"), m_lmtcmc._size);
	oper.readCommonTable(_T("������������ͼ��������ɫ"), m_lmtcmc._color);
	oper.readCommonTable(_T("������������ͼ������ͼ��"), m_lmtcmc._layer);
	oper.readCommonTable(_T("�����������ݱ߳�ע����ɫ"), m_fwbczj._color);
	oper.readCommonTable(_T("�����������ݱ߳�ע��ͼ��"), m_fwbczj._layer);
	oper.readCommonTable(_T("���������ֲ�ƽ�����ע�Ǵ�С"), m_mjzj._size);
	oper.readCommonTable(_T("���������ֲ�ƽ�����ע����ɫ"), m_mjzj._color);
	oper.readCommonTable(_T("���������ֲ�ƽ�����ע��ͼ��"), m_mjzj._layer);
	oper.readCommonTable(_T("������������ͼ�߶�ע�Ǵ�С"), m_lmtgdzj._size);
	oper.readCommonTable(_T("������������ͼ�߶�ע����ɫ"), m_lmtgdzj._color);
	oper.readCommonTable(_T("������������ͼ�߶�ע��ͼ��"), m_lmtgdzj._layer);
	oper.readCommonTable(_T("�����������ڵ�����ע�Ǵ�С"), m_sndpbg._size);
	oper.readCommonTable(_T("�����������ڵ�����ע������"), m_sndpbg._font);
	oper.readCommonTable(_T("�������������ƺ���ע�Ǵ�С"), m_swdpbg._size);
	oper.readCommonTable(_T("�������������ƺ���ע������"), m_swdpbg._font);
	oper.readCommonTable(_T("�����������ڵ�����ע����ɫ"), m_sndpbg._color);
	oper.readCommonTable(_T("�����������ڵ�����ע��ͼ��"), m_sndpbg._layer);
	oper.readCommonTable(_T("�������������ƺ���ע����ɫ"), m_swdpbg._color);
	oper.readCommonTable(_T("�������������ƺ���ע��ͼ��"), m_swdpbg._layer);
}

XMJGHouse::~XMJGHouse()
{

}

void XMJGHouse::createProject()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo7 info(true); info.DoModal();
	TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("���ͳ��.ini"), path))
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ������ļ�[���ͳ��.ini]"), _T("�ļ���ʧ��ʾ"), MB_OK);
		return;
	}
	CStdioFile sf;
	CString filepath(path);
	if(FALSE == sf.Open(filepath, CFile::modeCreate | CFile::modeWrite))
		MessageBox(GetForegroundWindow(), _T("��������ļ�[���ͳ��.ini]ʧ��!!!"), _T("��Ϣ��ʾ"), MB_OK);
	sf.Close();
}

void XMJGHouse::setProjectContext()
{
	HKEY rkey; TCHAR dir[255] = {0};
	_stprintf(dir, _T("%s"), m_ProjectContext);
	if(false == GetDirectory(_T("�����ù���Ŀ¼"), dir)) return;
	int es = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), 0, KEY_ALL_ACCESS, &rkey);
	if(es == ERROR_SUCCESS)
	{
		CassSetReg(rkey, _T("����Ŀ¼"), dir);
	}
	else
	{
		RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), &rkey);
	}
}

void XMJGHouse::DrawCarAddress()
{
	XMJGAssist assist;
	AcDbObjectId id = assist.DrawLine(m_cw._layer, _T(""), m_cw._color);
	AddXdata(aname(id), _T("CWLX"), 0, _T("��׼��λ"));
	AddXdata(aname(id), _T("CWSM"), 1, _T("1"));
	AddXdata(aname(id), _T("SZLC"), 0, _T("����"));
}

void XMJGHouse::DimensionsForCar()
{
	XMJGDimensions dim;	ads_point pt;
	if(RTNORM != ads_getpoint(NULL, _T("\n ������Ҫ������λ��ʵ����ѡ��һ�㣺"), pt)) return;
	dim.setEntityColor(m_cw._color);
	dim.setEntityLayer(m_cw._layer);
	dim.setPoint(apoint(pt));
	for(int idx = 1; idx < 10; ++idx)
	{
		dim.setSearchDist(2.5 * idx);
		if(true != dim.beginDimensions()) continue;
		AcDbObjectId carid = dim.getEntityId();
		AddXdata(aname(carid), _T("CWLX"), 0, _T("��׼��λ"));
		AddXdata(aname(carid), _T("CWSM"), 1, _T("1"));
		AddXdata(aname(carid), _T("SZLC"), 0, _T("����"));
		AcGePoint3d pos; GetMiddlePoint(carid, pos);
		AcDbObjectId textid = DrawText(pos, _T("��׼��λ"), _T("MC"), _T(""), 1.5);
		setlayer(aname(textid), _T("��λ��ע")); setcolor(aname(textid), 4);
		acutPrintf(_T("\n ������λ�ɹ�������"));
		acutPrintf(_T("\n ����ʵ��ķ�Χ��%.1f"), 2.5 * idx);
		return;
	}
	acutPrintf(_T("\n ������λʧ�ܣ�����"));
}

void XMJGHouse::SetCarAnnotation()
{
	int SerialNum = 1; TCHAR IsChange[10] = {0}; _tcscpy(IsChange, _T("N"));
	if(RTCAN == ads_getstring(0, _T("\n �Ƿ��޸�Ĭ����ţ�<N>/<Y> "), IsChange));
	if(_tcscmp(IsChange, _T("Y")) == 0)
	{
		int ret = ads_getint(_T("\n �����복λ��ţ�"), &SerialNum);
		if(RTCAN == ret) return;
	}
	else
		SerialNum = GetCarSerialNum();
	AcDbObjectId id; SelectFilter f1(8, m_cw._layer);
	if(false == SelectEntity(id, f1, NULL)) return;
	AcGePoint3d mid; GetMiddlePoint(id, mid);
	TCHAR serial[10] = {0}; _stprintf(serial, _T("��λ %i"), SerialNum);
	AcDbObjectId textid = DrawText(mid, serial, _T("MC"), _T(""), m_cwzj._size * m_Scale);
	CXRecordOperate recordOperate;
	recordOperate.AddIntegerRecord(_T("XMJG"), _T("CWBZ"), SerialNum);
	setlayer(aname(textid), m_cwzj._layer);
	setcolor(aname(textid), m_cwzj._color);
	AddXdata(aname(textid), _T("Serial"), INT_TYPE, serial);
}

void XMJGHouse::DrawStratificationPlan()
{
	CString path = GetCurrentDwgName();
	CString name = GetCurrentDwgFullPath();
	int pos = name.ReverseFind(_T('\\'));
	pos = pos == -1 ? name.ReverseFind(_T('/')) : pos;
	CString savedir = name.Left(pos);
	if(0 == path.CompareNoCase(name))
	{
		acutPrintf(_T("\n ���Ƚ���ǰ�ļ����б���"));
		return;
	}
	SelectFilter sf1(8, m_jzwlk._layer), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectId id; TCHAR info[200] = {0}; AcDbObjectIdArray ids;
	SelectEntitys(ids, sf1, sf2, _T("X"));
	XMJGPlanDlg dlg;
	if(ids.length() > 0)
	{
		if(false == SelectEntity(id, sf1, sf2, _T(""))) return;
		if(false == ReadXdata(aname(id), _T("¥����"), 0, info))
		{
			AfxMessageBox(_T("��������¥����"));
			return;
		}
		dlg.setObjectID(id);
	}
	if(IDOK != dlg.DoModal()) return;
	VStr layerinfo = dlg.getLayerInfo();
	XMJGPlan plan;
	plan.setLayerInfo(layerinfo);
	if(id == AcDbObjectId::kNull)
	{
		TCHAR dName[200] = {0};
		if(RTNORM != ads_getstring(0, _T("\n�����뽨�������ƣ�"), dName)) return;
		plan.CreatePlanEx(savedir, dName);
	}
	else
	{
		plan.setObjectID(id);
		plan.CreatePlan(savedir);
	}
	IProjectMDB pdb; MStr record;
	record[_T("¥����")] = info;
	pdb.setDXXTalbeInfo(MStr(), record);
}

CString changeDWGView(CString dwgName)
{
	AcApDocManager *pDocMgr = acDocManagerPtr();
	AcApDocumentIterator *pDocItr;
	AcApDocument *pMainDoc = NULL;
	pDocItr = pDocMgr->newAcApDocumentIterator();
	CString cur(curDoc()->fileName());
	for(; !pDocItr->done(); pDocItr->step())
	{
		AcApDocument *pDoc = pDocItr->document();
		CString name(pDoc->fileName());
		if(name.Find(dwgName) == -1) continue;
		acDocManager->setCurDocument(pDoc);
		acDocManager->activateDocument(pDoc);
		break;
	}
	delete pDocItr;
	return cur;
}

CString margeCarCount(const CString &info, const CString &ldh, int count)
{
	if(count == 0) return info;
	int pos = info.Find(ldh);
	CString ret(info);
	if(pos == -1)
	{
		ret.Format(_T("%s%s_%d;"), info, ldh, count);
	}
	else
	{
		pos = info.Find(ldh) + ldh.GetLength() - 1;
		pos = info.Find(_T('_'), pos);
		int pos1 = info.Find(_T(';'), pos);
		int ccount = _ttoi(info.Mid(pos + 1, pos1 - pos));
		CString temp1; temp1.Format(_T("%s_%d"), ldh, ccount);
		CString temp2; temp2.Format(_T("%s_%d"), ldh, count);
		ret.Replace(temp1, temp2);
	}
	return ret;
}

void XMJGHouse::newCheWeiStatistic()
{
	CString name = GetCurrentDwgName();
	int ret = name.Find(_T("��Ŀͼ��.dwg"));
	if(ret == -1)
	{
		MessageBox(GetForegroundWindow(), _T("������Ŀͼ���Ͻ������ͳ��"), _T("������Ϣ"), 0);
		return;
	}
	AcDbObjectIdArray objects, IdArr;
	SelectFilter sf(8, m_cw._layer);
	SelectEntitys(IdArr, sf, _T("X"));
	MStr record; double jzmj = 0.0, jrmj = 0.0;
	int dscw = 0, dxcw = 0;	IProjectMDB pdb;
	for(int idx = 0; idx < IdArr.length(); ++idx)
	{
		AcDbObjectId id = IdArr.at(idx);
		getCarCount(id, record, jzmj, jrmj);
	}
	MStr record1 = pdb.getMJXXTableInfo();
	record[_T("ʵ����³�λ")] = margeCarCount(record1[_T("ʵ����³�λ")], _T("��ƽͼ"), _ttoi(record[_T("ʵ����³�λ")]));
	record[_T("ʵ����ϳ�λ")] = margeCarCount(record1[_T("ʵ����ϳ�λ")], _T("��ƽͼ"), _ttoi(record[_T("ʵ����ϳ�λ")]));
	pdb.setMJXXTableInfo(record);
}

void XMJGHouse::ExportBuildHeightAndAreaInfo()
{
	CString name = GetCurrentDwgName();
	int ret = name.Find(_T("�ֲ�ƽ��ͼ.dwg"));
	if(ret == -1)
	{
		MessageBox(GetForegroundWindow(), _T("���ڷֲ�ƽ��ͼ�Ͻ������ͳ��"), _T("������Ϣ"), 0);
		return;
	}
	AcDbObjectIdArray objects;
	m_HighlySchematicFlag = true;
	SelectFilter f1(8, m_fcpm._layer);
	if(false == SelectEntitys(objects, f1, _T("X"))) return;
	int len = objects.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = objects.at(idx);
		zoomWindow(id);
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		writeArea2ProjectMDB(id);
	}

	objects.setLogicalLength(0);
	objects.setPhysicalLength(0);

	SelectEntitys(objects, _T("X"));
	zoomWindow(objects);

}

void XMJGHouse::ExportBuildHeightAndAreaDoc()
{
	m_HighlySchematicFlag = true;
	SelectFilter f1(8, m_fcpm._layer);
	AcDbObjectIdArray objects, IdArr;
	if(false == SelectEntitys(objects, f1, _T("X"))) return;
	int len = objects.length();
	MSHAH infos; BuildHAH buildhah;
	for(int idx = 0; idx < len; ++idx)
	{
		LayerHAH hah;
		AcDbObjectId id = objects.at(idx);
		TCHAR value[255] = {0};
		getLayerArea(id, hah);
		int start = -1, end = -1;
		int count = getHouseLCH(hah.m_lch, start, end);
		hah.m_jrzmj *= count;
		hah.m_jzzmj *= count;
		ReadXdata(aname(id), _T("¥����"), 0, value);
		buildhah.m_build.Format(_T("%s"), value);
		addBuild2Layer(hah, buildhah);
	}
	SmartTable::Word word; TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("�������ͳ�Ʊ�.docx"), path))
	{
		acutPrintf(_T("\n �޷��ҵ�ģ���ļ� �������ͳ�Ʊ�.docx"));
		return;
	}
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrInitialDir = m_SaveDir;
	if(dlg.DoModal() != IDOK) return;
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath); char savepath[1024] = {0};
	TcharToChar(dlg.GetPathName() + _T(".docx"), savepath);
	exportDoc(word, buildhah);
	word.save(savepath);
	MSBuild build;
	build[buildhah.m_build] = buildhah;
	updataAreaIniFile(build);
	acutPrintf(_T("\n ���������ɡ�·��Ϊ%s"), path);
}

void XMJGHouse::SetStratificationPlanInfo()
{
	AcDbObjectId id; TCHAR LDH[200] = {0};
	SelectFilter filter(8, m_fcpm._layer);
	if(false == SelectEntity(id, filter, _T("")))
		return;
	if(false == ReadXdata(aname(id), _T("¥����"), 0, LDH))
	{
		AfxMessageBox(_T("��������¥����"));
		return;
	}
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetStratPlanInfo planinfo;
	planinfo.setObjectID(id);
	if(IDOK != planinfo.DoModal()) return;
}

void XMJGHouse::DimensionsForChangeArea()
{
	XMJGDimensions dim;	ads_point pt;
	if(RTNORM != ads_getpoint(NULL, _T("\n ������Ҫ������������ʵ����ѡ��һ�㣺"), pt)) return;
	dim.setPoint(apoint(pt));
	dim.setSearchDist(10);
	dim.setEntityColor(6);
	dim.beginDimensions();
}

void XMJGHouse::DrawBuildChangeArea()
{
	XMJGAssist assist;
	AcDbObjectId id = drawLine();
	CSetEntityPropery prop;
	prop.setBG(id);
}

void XMJGHouse::ExportChangeAreaDWG()
{
	CmdEcho cmdEcho;
	double angle = 0.0; int type = 1;
	if(RTCAN == ads_getint(_T("\n [1]:��ͼ����ˡ�[2]:�滮��ɡ�[3]:���ͼֽ��<1>"), &type)) return;
	if(type < 1 || type > 3) type = 1; VStr titles;
	titles.push_back(_T("������״����ͼ�����ͼֽ��һ�¶���ʾ��ͼ"));
	titles.push_back(_T("������״��滮��ɲ�һ�¶���ʾ��ͼ"));
	titles.push_back(_T("������״�����ͼͼֽ��һ�¶���ʾ��ͼ"));
	CString title = titles[type - 1];
	CXRecordOperate xrecord; TCHAR ldh[255] = {0};
	xrecord.ReadDoubleRecord(_T("XMJG"), _T("FCPM"), angle);
	if(false == selectExportObject()) return; XMJGAssist assist;
	CString pathName = assist.getExportFilePath();
	ReadXdata(aname(m_object.first()), _T("¥����"), 0, ldh);
	CString fileName = assist.getExportFileName(title);
	fileName.Append(ldh); fileName.Append(_T(".dwg"));
	pathName += _T("\\"); pathName += fileName;
	int count = createChangeAreaDWG(fileName, angle, _T("XMJG���ͼ"), title);
	if(count == 0) return;
	if(false == combineChangeAreaDWG(count, pathName)) return;
	openDwgCommand(pathName);
}

void XMJGHouse::ExportGuiHuaBuYiZhiDWG()
{
	double angle = 0.0;
	CXRecordOperate xrecord; TCHAR ldh[255] = {0};
	xrecord.ReadDoubleRecord(_T("XMJG"), _T("FCPM"), angle);
	if(false == selectExportObject()) return; XMJGAssist assist;
	CString pathName = assist.getExportFilePath();
	ReadXdata(aname(m_object.first()), _T("¥����"), 0, ldh);
	CString fileName = assist.getExportFileName(_T("����������滮ͼͼֽ��һ�¶���ʾ��ͼ"));
	fileName.Append(ldh); fileName.Append(_T(".dwg"));
	pathName += _T("\\"); pathName += fileName;
	int count = createChangeAreaDWG(fileName, angle, _T("XMJG�滮���ͼ"), _T("������״�����ͼͼֽ��һ�¶���ʾ��ͼ"));
	if(count == 0) return;
	if(false == combineChangeAreaDWG(count, pathName)) return;
	openDwgCommand(pathName);
}

void XMJGHouse::AddBuildChangeAnnotion()
{
	SelectFilter filter(8, m_bg._layer);
	CDrawArrow arrow; AcDbObjectId id;
	if(false == SelectEntity(id, filter, _T(""))) return;
	setChangeAreasInfo(id); return;
	// 	ads_point pos, corner; int order = 0;
	// 	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ����Ҫ��ע�Ľǵ㣺"), corner)) return;
	// 	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���ע��λ�ã�"), pos)) return;
	// 	if(RTNORM != ads_getint(_T("\n �������ע����Ϣ��"), &order)) return;
	// 	CString bgxx; bgxx.Format(_T("���%d"), order);
	// 	arrow.DrawLeader(corner, pos, m_bgzj._size * m_Scale, m_bgzj._layer, m_bgzj._color, bgxx);
	// 	AddXdata(aname(id), _T("������"), 0, bgxx);
}

void XMJGHouse::DrawFunctionalAreas()
{
	XMJGAssist assist;
	AcDbObjectId id = drawLine(m_gnq._layer, m_gnq._color);
	if(id == AcDbObjectId::kNull) return;

	FunctionNameTableEx fnt = setFunctionalAreaInfo(id);
	addFunctionProperty(id, fnt);
	addFunctionAreaNote(id);
	ads_name ent; acdbGetAdsName(ent, id);
	SetLineWidth(id, 0.06);
	if(fabs(_tstof(fnt.mjxs) - 0.5) < EPS)
	{
		setlinetype(aname(id), _T("X6"));
	}
	acedCommand(RTSTR, _T("draworder"), RTENAME, ent, RTSTR, _T(""), RTSTR, _T("F"), 0);
}

void XMJGHouse::DimensionsFunctionalAreas()
{
	XMJGDimensions dim;	ads_point pt; CString info;
	FunctionNameTableEx fntlast;
	while(true)
	{
		if(RTNORM != ads_getpoint(NULL, _T("\n ������Ҫ������ʵ����ѡ��һ�㣺"), pt)) return;
		dim.setEntityColor(m_gnq._color);
		dim.setEntityLayer(m_gnq._layer);
		dim.setPoint(apoint(pt));
		for(int idx = 1; idx < 100; ++idx)
		{
			dim.setSearchDist(2.5 * idx); double area = 0.0;
			if(true != dim.beginDimensions()) continue;
			if(fntlast.name.GetLength() != 0)
			{
				GetEntArea(aname(dim.getEntityId()), area);
				double scale = (float)_tstof(fntlast.mjxs);
				float mj = (float)(area * scale);
				info.Format(_T("%.3f"), mj);
				AddXdata(aname(dim.getEntityId()), _T("�������"), 3, info);
				scale = _tstof(fntlast.jrxs); mj = (float)(scale * area);
				info.Format(_T("%.3f"), mj);
				AddXdata(aname(dim.getEntityId()), _T("�������"), 0, info);
				AddXdata(aname(dim.getEntityId()), _T("����������"), 0, fntlast.name);
				AddXdata(aname(dim.getEntityId()), _T("�������߶�"), 0, fntlast.gaodu);
				AddXdata(aname(dim.getEntityId()), _T("���ϵ��"), 0, fntlast.mjxs);
				AddXdata(aname(dim.getEntityId()), _T("����ϵ��"), 0, fntlast.jrxs);
				addFunctionAreaNote(dim.getEntityId());
				break;
			}
			else
			{
				if(false == setFunctionalAreaInfo(dim.getEntityId(), fntlast))
					return;
				else
					break;
			}
			acutPrintf(_T("\n ����ʵ��ķ�Χ��%.1f"), idx * 2.5);
		}
	}
	acutPrintf(_T("\n ����ʧ��"));
}

void XMJGHouse::SetFunctionsAreasInfo()
{
	SelectFilter filter(8, m_gnq._layer);
	AcDbObjectId id;
	if(false == SelectEntity(id, filter, _T(""))) return;
	FunctionNameTableEx fnt = setFunctionalAreaInfo(id);
	if(!fnt.m_init) return;
	addFunctionProperty(id, fnt);
	addFunctionAreaNote(id);
}

void XMJGHouse::RemoveIslandFunctionArea(AcDbObjectIdArray &idArr)
{
	for(int i = 0; i < idArr.length(); ++i)
	{
		ads_name ent;
		acdbGetAdsName(ent, idArr[i]);
		TCHAR szData[255] = {0};
		ReadXdata(ent, _T("�۵�"), 0, szData);
		if(0 == _tcsicmp(szData, _T("true")))
		{
			idArr.removeAt(i);
			i--;
		}
	}
}

void XMJGHouse::StatisticFunctionArea()
{
	SelectFilter f1(8, m_fcpm._layer);
	AcDbObjectIdArray objects, IdArr;
	if(false == SelectEntitys(objects, f1, _T(""))) return;
	int len = objects.length(); ads_point pmin, pmax;
	TCHAR info[200] = {0};
	_stprintf(info, _T("%s,%s"), m_gnq._layer, m_cw._layer);
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = objects.at(idx);
		AcGePoint2dArray node; AcDbObjectIdArray ids, IdArr;
		GetPlList(aname(id), node);
		ssFromNodes(ids, node, 1, 1.0, _T("*POLYLINE"), info);
		RemoveIslandFunctionArea(ids);
		acutPrintf(_T("\n �������ĸ���Ϊ%d��"), ids.length());
		GetEntExtent(aname(id), pmin, pmax);
		ZoomWindow(pmin, pmax); TCHAR gd[25] = {0};
		ReadXdata(aname(id), _T("¥���"), 0, info);
		for(int idxx = 0; idxx < ids.length(); ++idxx)
		{
			ReadXdata(aname(ids[idxx]), _T("�������߶�"), 0, gd);
			if(_tcslen(gd) == 0)
				AddXdata(aname(ids[idxx]), _T("�������߶�"), 0, info);
		}
		readFunctionInfo(ids, IdArr);
	}
}

void XMJGHouse::addStoreyInfo(StoreyInfo &info, const CString &layer, MStr &record)
{
	info.setStartLayer(layer);
	info.setEndLayer(record[_T("��ֹ����")]);
	info.setHeight(_tstof(record[_T("ʵ����")]));
	info.setType(record[_T("����")]);
	info.setparam1(_tstof(record[_T("����1")]));
	info.setparam2(_tstof(record[_T("����2")]));
	info.setparam3(record[_T("����3")]);
	info.setlayerCount(_tstoi(record[_T("����")]));
}

void XMJGHouse::ExportHighlySchematic()
{
	double width = 50.0; double ft = 0.0;
	if(RTCAN == ads_getreal(_T("\n �����뷿�ݵĿ�ȣ�<50.0>"), &width)) return;
	if(RTCAN == ads_getreal(_T("\n �����븲���ĸ߶ȣ�"), &ft)) return;
	CAcModuleResourceOverride myResource;
	XMJGChangeLayerInfoDlg infodlg;
	if(infodlg.DoModal() != IDOK) return;
	m_HighlySchematicFlag = true;
	SelectFilter f1(8, m_fcpm._layer);
	AcDbObjectIdArray objects, IdArr;
	if(false == SelectEntitys(objects, f1, _T("X"))) return;
	int len = objects.length();
	TCHAR szVal[20] = {0};	MIStoreyInfo floorHeight;
	ReadXdata(aname(objects.first()), _T("¥����"), 0, szVal);
	MStr filter; filter[_T("¥����")] = szVal; VMStr records;
	{IProjectMDB pdb; records = pdb.getCXXTableInfoEx(filter); }
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx]; StoreyInfo sinfo;
		if(record[_T("�Ƿ���ʾ")].CompareNoCase(_T("0")) == 0) continue;
		if(record[_T("�Ƿ�Ծ��")].CompareNoCase(_T("1")) == 0)
		{
			CString sLayer = record[_T("��ʼ����")];
			CStringArray layers;
			AnalyzeData(layers, sLayer, _T(";"));
			for(int indx = 0; indx < layers.GetSize(); ++indx)
			{
				StoreyInfo sinfo; sinfo.setWidth(width);
				CString layer = layers.GetAt(indx);
				addStoreyInfo(sinfo, layer, record);
				CString layerBH; layerBH.Format(_T("%s"), layer);
				floorHeight[layerBH] = sinfo;
			}
		}
		else
		{
			sinfo.setWidth(width);
			addStoreyInfo(sinfo, record[_T("��ʼ����")], record);
			floorHeight[record[_T("¥���")]] = sinfo;
		}
	}
	if(false == DrawHightlySchematic(floorHeight, IdArr, ft, szVal)) return;
	MStr fieldvalue; TCHAR floor[255] = {0};
	ReadXdata(aname(objects.first()), _T("¥����"), 0, floor);
	m_FloorName.Format(_T("%s"), floor);
	CString value; value.Format(_T("%s"), m_FloorName);
	CString field; field.Format(_T("%s"), _T("#FLOOR#"));
	fieldvalue[field] = value;
	XMJGAssist assist;
	CString path = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("���������߶�ʾ��ͼ"));
	path.Append(_T("\\")); path.Append(name); path.Append(floor);
	path += _T(".dwg");  JGExportDWG exportdwg;
	exportdwg.setObjects(IdArr);
	getReplaceValue(fieldvalue);
	exportdwg.setPictureFrame(_T("XMJG�߶�ʾ��ͼ"));
	exportdwg.setCallBack(fieldvalue, XMJGReplaceField);
	exportdwg.setSavePath(path);
	exportdwg.isDrawRectangle(false);
	exportdwg.ExportDwg();
	EntsDel(IdArr);
	openDwgCommand(path);
}

void XMJGHouse::ExportEntitys2File()
{
	AcDbObjectIdArray IdArr;
	if(false == SelectEntitys(IdArr, _T(""))) return;
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrInitialDir = m_SaveDir;
	if(IDOK != dlg.DoModal())	return;
	XMJGExport exportdwg;	exportdwg.setObjects(IdArr);
	exportdwg.setSavePath(dlg.GetPathName());
	exportdwg.setObjects(IdArr); int exporttype = 0;
	ads_getint(_T("\n �����ʽ<0>dwg <1>jpg��"), &exporttype);
	if(exporttype == 0)	exportdwg.ExportDwg();
	else
	{
		exportdwg.ExportPicture(true); MStr record;
		record[_T("����")] = dlg.GetFileName();
		record[_T("λ��")] = _T("�ֲ�ƽ��ͼ");
	}
}

void XMJGHouse::ExportStratificationPlan()
{
	CXRecordOperate xreord; double angle = 0.0;
	xreord.ReadDoubleRecord(_T("XMJG"), _T("FCPM"), angle);
	if(false == selectExportObject()) return; XMJGAssist assist;
	CString pathName = assist.getExportFilePath(); TCHAR ldh[255] = {0};
	CString fileName = assist.getExportFileName(_T("���������ֲ�ƽ��ͼ"));
	ReadXdata(aname(m_object.first()), _T("¥����"), 0, ldh);
	fileName.Append(ldh); fileName.Append(_T(".dwg"));
	pathName += _T("\\"); pathName += fileName;
	if(!createStratificationPlan(fileName, angle)) return;
	combineStratificationPlan(pathName);
	openDwgCommand(pathName);
}

void XMJGHouse::SetExportHouseDocInfo()
{
	TCHAR path[255] = {0};
	_stprintf(path, _T("%s\\"), m_SaveDir);
	XMJGChangePictureProperty dlg(path);
	dlg.DoModal();
}

void XMJGHouse::ExportHouseDoc()
{
	CString name = GetCurrentDwgName();
	if(name.Find(_T("��Ŀͼ��.dwg")) == -1)
	{
		MessageBox(GetForegroundWindow(), _T("������Ŀͼ��������ܱ���"), _T("������Ϣ"), MB_OK);
		return;
	}
	{
		IProjectMDB pdb; MStr record; AcDbObjectIdArray hxids;
		SelectFilter sf1(8, _T("����")), sf2(RTDXF0, _T("*POLYLINE"));
		SelectEntitys(hxids, sf1, sf2, _T("X"));
		if(hxids.length() == 0)
			record[_T("ʵ�⽨���õ����")].Format(_T("/"));
		else if(hxids.length() == 1)
		{
			AcDbEntity *pEnt = NULL;
			acdbOpenAcDbEntity(pEnt, hxids.first(), AcDb::kForRead);
			AcDbPolyline *pLine = (AcDbPolyline *)pEnt;
			bool flag = Adesk::kTrue == pLine->isClosed(); pEnt->close();
			if(!flag)
				record[_T("ʵ�⽨���õ����")] = _T("�����ھֲ��õأ����޷��綨");
			else
			{
				double area = 0.0; GetEntArea(aname(hxids.first()), area);
				record[_T("ʵ�⽨���õ����")].Format(_T("%.2lf�O"), area);
			}
		}
		else
		{
			acutPrintf(_T("\����ͼ���Ƿ���ڶ�����Ŀ����"));
			return;
		}
		pdb.setMJXXTableInfo(record);
	}
	XMJGAssist assist;
	CString savePath = assist.getExportFilePath();
	SmartTable::Word word; TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("�������ձ�����.docx"), path))
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ�ģ���ļ�[�������ձ�����.docx]"), _T("������Ϣ"), MB_OK);
		return;
	}
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath);

	{
		TCHAR xmjgMDB[255] = {0}; VMStr records;
		ads_findfile(_T("xmjg.mdb"), xmjgMDB);
		CDatabaseOperManager dbManager;
		dbManager.initialConnect(xmjgMDB);
		dbManager.ReadDataBase_all(_T("JGFieldTable"), records);
		for(int indx = 0; indx < records.size(); ++indx)
		{
			MStr record = records[indx];
			CString title = record[_T("����")];
			CString field = record[_T("�ֶ�")];
			m_fieldTitle[field] = title;
			m_isReplace[title] = 0;
		}
	}

	replaceWordField(word);			//�滻��Ƭ
	replaceWordArea(word);			//�滮ʵʩ���
	replaceWordHeight(word);		//��������������߶ȼ���߻��ܱ�
	ExportDocInfo(word);
	DrawUpPlanning(word);
	DesignImplementation(word);
	replaceWordTitle(word);
	IDataBaseOper oper;
	CString outname = oper.readFileNameTable(_T("���������ܱ������������"));
	savePath += outname; char savepath[255] = {0}; DeleteFile(savePath);
	/*word.removeBlankParagraph();*/ //word.removeBlankPage();
	//word.setsectPrContinuous();

	word.removeBlankParagraphBetweenBookmark("blankbk");
	word.removeBlankSectPr();
	word.updataContent();
	TcharToChar(savePath, savepath); word.save(savepath);
	acutPrintf(_T("\n ���������ɡ�·��Ϊ%s"), savePath);
}

struct ImageSize
{
	double m_width;
	double m_height;
};

typedef map<CString, ImageSize> SImageSize;

bool XMJGHouse::replaceWordField(Word &word)
{
	IProjectMDB pdb;
	VMStr records = pdb.getPictureTable();
	int len = (int)records.size();
	bool byzsyt = false, yghtbyz = false;
	SmartTable::WordTable table; char info[255] = {0};
	int tableindex = word.findTable(1, 1, "#Format1#", table);
	int index = word.setPropertyTable(tableindex);
	IDataBaseOper oper; CString val; SImageSize sis;
	MStr filter; filter[_T("�ؼ���")] = _T("����ͼƬС");
	if(!oper.readDataBase(_T("Common"), _T("����ֵ"), filter, val))
	{
		MessageBox(GetForegroundWindow(), _T("�޷���ȡ[����ͼƬС]�ĳߴ�"), _T("������Ϣ"), 0);
		return false;
	}
	else
		sis[_T("����ͼƬС")].m_height = _tstof(val);

	if(!oper.readDataBase(_T("Common"), _T("����ֵ1"), filter, val))
	{
		MessageBox(GetForegroundWindow(), _T("�޷���ȡ[����ͼƬС]�ĳߴ�"), _T("������Ϣ"), 0);
		return false;
	}
	else
		sis[_T("����ͼƬС")].m_width = _tstof(val);

	filter[_T("�ؼ���")] = _T("����ͼƬ��");
	if(!oper.readDataBase(_T("Common"), _T("����ֵ"), filter, val))
	{
		MessageBox(GetForegroundWindow(), _T("�޷���ȡ[����ͼƬС]�ĳߴ�"), _T("������Ϣ"), 0);
		return false;
	}
	else
		sis[_T("����ͼƬ��")].m_height = _tstof(val);

	if(!oper.readDataBase(_T("Common"), _T("����ֵ1"), filter, val))
	{
		MessageBox(GetForegroundWindow(), _T("�޷���ȡ[����ͼƬС]�ĳߴ�"), _T("������Ϣ"), 0);
		return false;
	}
	else
		sis[_T("����ͼƬ��")].m_width = _tstof(val);

	filter[_T("�ؼ���")] = _T("����ͼƬ��");
	if(!oper.readDataBase(_T("Common"), _T("����ֵ"), filter, val))
	{
		MessageBox(GetForegroundWindow(), _T("�޷���ȡ[����ͼƬС]�ĳߴ�"), _T("������Ϣ"), 0);
		return false;
	}
	else
		sis[_T("����ͼƬ��")].m_height = _tstof(val);
	if(!oper.readDataBase(_T("Common"), _T("����ֵ1"), filter, val))
	{
		MessageBox(GetForegroundWindow(), _T("�޷���ȡ[����ͼƬС]�ĳߴ�"), _T("������Ϣ"), 0);
		return false;
	}
	else
		sis[_T("����ͼƬ��")].m_width = _tstof(val);

	MSInt buildNameXH;
	for(int indx = 0; indx < len; ++indx)
	{
		MStr record = records[indx];
		CString name = record[_T("����")];
		buildNameXH[name] = indx;
	}

	CString ppath = oper.readPathTable("��ǰĿ¼") + _T("\\CK\\");

	MSInt::reverse_iterator bnxhit = buildNameXH.rbegin();
	for(; bnxhit != buildNameXH.rend(); ++bnxhit)
	{
		CString tName = bnxhit->first;
		int index = bnxhit->second;
		MStr record = records.at(index);
		CString local(record[_T("λ��")]);
		CString descr(record[_T("����")]);
		CString name(record[_T("����")]);
		CString path(ppath + name);
		if(local.CompareNoCase(_T("#BYZSYTPicture1#")) == 0 && !byzsyt)
		{
			CString title = m_fieldTitle[_T("#BYZSYTPicture1#")];
			m_isReplace[title] = 1; byzsyt = true;
		}
		else if(local.CompareNoCase(_T("#YGHTBYZPicture1#")) == 0 && !yghtbyz)
		{
			CString title = m_fieldTitle[_T("YGHTBYZPicture1")];
			m_isReplace[title] = 1; yghtbyz = true;
		}
		TcharToChar(local, info);
		CString title = m_fieldTitle[info];
		m_isReplace[title] = 1;
		word.setCurrentInsert(info, 0, true);
		TcharToChar(path, info);
		int width = 0, height = 0;
		getImagePngSize(info, width, height);
		if(width == height && width == 0)
			word.appendImage(info, sis[_T("����ͼƬ��")].m_width, sis[_T("����ͼƬ��")].m_height, SmartTable::Alignment_MC);
		else if(width < 3000 && height < 3000)
			word.appendImage(info, sis[_T("����ͼƬС")].m_width, sis[_T("����ͼƬС")].m_height, SmartTable::Alignment_MC);
		else if(width < 4000 && height < 4000)
			word.appendImage(info, sis[_T("����ͼƬ��")].m_width, sis[_T("����ͼƬ��")].m_height, SmartTable::Alignment_MC);
		else
			word.appendImage(info, sis[_T("����ͼƬ��")].m_width, sis[_T("����ͼƬ��")].m_height, SmartTable::Alignment_MC);
			TcharToChar(descr, info); word.appendParagraph(info, tableindex);
	}

	MStr nameproperty;
	oper.readCommonTable(_T("ͼƬ�����ʽ"), nameproperty);
	MSIter it = nameproperty.begin();
	if(!byzsyt) word.replaceText("#BYZSYTPicture1#", "");
	if(!yghtbyz) word.replaceText("#BYZSYTPicture1#", "");
	for(; it != nameproperty.end(); ++it)
	{
		TcharToChar(it->first, info);
		word.removeParagraph(info);
	}
	word.removeTable(tableindex);
	word.removeBlankPage();
	return true;
}

void XMJGHouse::ExportPlanInfo2MDB()
{
	m_HighlySchematicFlag = true;
	SelectFilter f1(8, m_fcpm._layer);
	AcDbObjectIdArray objects, IdArr;
	if(false == SelectEntitys(objects, f1, _T("X"))) return;
	int len = objects.length(); MSHAH infos;
	for(int idx = 0; idx < len; ++idx)
	{
		XMJGHAH temp;
		AcDbObjectId id = objects.at(idx);
		getHouseAreaAndHeight(id, temp);
		getSameLevelArea(temp, infos);
		int start = -1, end = -1;
		int count = getHouseLCH(temp.m_lch, start, end);
		temp.m_jzzmj = temp.m_jzmj * count;
		temp.m_jrzmj = temp.m_jrmj * count;
	}
}

void XMJGHouse::BuildSetAttatchedInfo()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGSetInfo7 info(false);
	info.DoModal();
}

void XMJGHouse::FunctionDefine()
{
	CAcModuleResourceOverride myResouces;
	XMJGFunctionDefine def;
	def.DoModal();
}

void XMJGHouse::buildHatch()
{
	SelectFilter f(8, _T("����������")); AcDbObjectId buildid;//����ͼ��ɸѡ����
	if(false == SelectEntity(buildid, f, _T(""))) return;//ѡ��ͼ��Ϊ����������������ʵ��
	acutPrintf(_T("\n ��ѡ����Ҫ�۳���Ȩ����:"));
	ads_name ssName;//���ڴ洢��Ҫ�۳���Ȩ���߼���ads_name
	ads_ssget(_T(""), NULL, NULL, NULL, ssName);//ѡ����Ҫ�۳���Ȩ����
	AcDbObjectIdArray unfill;//���ڴ洢��Ҫ�۳���Ȩ���߼���ID��
	SSToIdArr(ssName, unfill);//��ѡ���Ȩ���߼���ads_nameת��Ϊʵ��ID����
	AREAINFO info; 
	info.IdFlood = buildid;//��������������ID��ֵ��info�е�ˮ����ID
	info.idSafe.append(unfill);//��ˮ�����еĵ���ID�����뵽info�еİ�ȫ��ID��
	AcDbObjectId region = C3DOperator::CreateRingRegion(buildid, unfill);//������״����
	AcDbObjectId hatchid = C3DOperator::HatchObject(info, 31, _T("SOLID"));//����ˮ�����͵��������������
	setcolor(aname(region), 31);//����״������ɫ����Ϊǳ��ɫ
	SetLineWidth(aname(buildid), 0.6 * m_Scale);//���ý����������߿��
	ads_name ent;//���潨�������ߵ�ads_name
	acdbGetAdsName(ent, buildid);//��ý����������ߵ�ads_name
	acedCommand(RTSTR, _T("draworder"), RTENAME, ent, RTSTR, _T(""), RTSTR, _T("B"), 0);//��������������������ʾ��ײ�
}

void XMJGHouse::calculateJZZDMJ()
{
	SelectFilter sfJZWLK(8, _T("����������")), sfPl(5020, _T("*POLYLINE"));//����ɸѡ��������ѡ������
	AcDbObjectIdArray JZWLKArr;//�洢���еĽ�����������
	if (false == SelectEntitys(JZWLKArr, sfJZWLK, sfPl, _T("X")))return;//ѡ��Χ�����н�����������
	int JZWLKLen = JZWLKArr.length();//���㷶Χ�ڵĽ�������������Ŀ
	if (JZWLKLen == 0)
	{
		acutPrintf(_T("\n��ǰDWG�����ڽ�����������"));
		return;
	}
	double JZZDMJ = 0.0;
	IProjectMDB pdb;
	for (int i = 0; i < JZWLKLen; i++)
	{
		AcDbObjectId id = JZWLKArr.at(i);
		AcGePoint2dArray nodes;
		GetPlList(aname(id), nodes);
		AcDbObjectIdArray hatchArr;//�洢�������������е��������
		ssFromNodes(hatchArr, nodes, 2, 0.1, _T("HATCH"));//ѡ�������������е������������
		int hatchCount = hatchArr.length();//���㽨�����������е�����������
		if (hatchCount == 0)continue;//�����������������򲻼���
		for (int j = 0; j < hatchCount; j++)//�����������������е��������
		{
			AcDbHatch *pHatch;
			if (Acad::eOk != acdbOpenObject(pHatch, hatchArr.at(i), AcDb::kForRead))return;//��ֻ����ʽ���������
			double hatchArea = 0.0;//�ݴ�����������
			if (Acad::eOk != pHatch->getArea(hatchArea))return;//�������������
			pHatch->close();//�ر��������
			JZZDMJ += hatchArea;//�ۼ�����������
		}
		TCHAR ldh[255] = { 0 };//�ݴ潨����¥����
		ReadXdata(aname(id), _T("¥����"), 0, ldh);//��ȡ�����������ߵ�XData�еĽ�����¥����
		MStr filter;//�����������������ļ�¼
		filter[_T("¥����")] = ldh;//ʹ��¥������Ϊ��������
		MStr setContent;//�ݴ���¼�¼����
		setContent[_T("����ռ�����")].Format(_T("%.3lf"), JZZDMJ);
		if (pdb.getRecordCount(_T("DXX"), filter) == 0)//�ж�¥����Ϣ�����Ƿ���ڸý������¼
		{//������һ�����ڸý������¼
			acutPrintf(_T("\n�����쳣�������󣬲����ڸý������¼��"));
		}
		else
		{
			pdb.setDXXTalbeInfo(filter, setContent);//���½���ռ�����
		}
	}
}

void XMJGHouse::drawRotateLine()
{
	ads_point spt, ept, pt1;
	if(RTNORM != ads_getpoint(NULL, _T("\n �������ת�ߵĵ�һ����:"), spt)) return;
	if(RTNORM != ads_getpoint(spt, _T("\n ��ѡ��ѡ���ȵ����һ����:"), ept)) return;
	AcDbObjectId id = drawline(spt, ept, _T("454")); setlayer(aname(id), _T("0"));
	double tangle = ads_angle(ept, spt);
	ads_polar(ept, tangle + PI / 6, 4, pt1);
	id = drawline(ept, pt1, _T("")); setlayer(aname(id), _T("0"));
	ads_polar(ept, tangle - PI / 6, 4, pt1);
	id = drawline(ept, pt1, _T("")); setlayer(aname(id), _T("0"));
	double angle1 = ads_angle(spt, ept);
	double angle2 = ads_angle(ept, spt);
	double angle = angle1 < angle2 ? angle1 : angle2;
	CXRecordOperate xrecord;
	xrecord.AddDoubleRecord(_T("XMJG"), _T("FCPM"), angle + PI * 1.5);
}

void XMJGHouse::changeLayerInfo()
{
	CAcModuleResourceOverride myResource;
	XMJGChangeLayerInfoDlg dlg;
	dlg.DoModal();
}

void XMJGHouse::drawAreaRange()
{
	SelectFilter sf(8, _T("����"));
	AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T("")))
	{
		MessageBox(GetForegroundWindow(), _T("��ѡ�����"), _T("������ʾ"), MB_OK);
		return;
	}
	if(GetPolyType(aname(id), true) < 10)
	{
		MessageBox(GetForegroundWindow(), _T("����δ�պ�"), _T("������ʾ"), MB_OK);
		return;
	}

	CSetEntityPropery prop;
	prop.setProjectLine(id);
	// 	AcGePoint2dArray node;
	// 	GetPlList(aname(id), node);
	// 	IDataBaseOper oper; CString layer; int color;
	// 	oper.readCommonTable(_T("�����Χ��ͼ��"), layer);
	// 	oper.readCommonTable(_T("�����Χ����ɫ"), color);
	// 	AcDbObjectId areaid = DrawPolyline2D(node, 1.0, true, _T(""));
	// 	setlayer(aname(areaid), layer);
	// 	setcolor(aname(areaid), color);
	// 	ADDMulXData(id, _T("south"), _T(""), 0, 0);
	// 	ADDMulXData(id, _T("south"), _T("100000"), 0, 1);
}

bool XMJGHouse::comprassFile()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg(_T("���ݿ���"));
	dlg.DoModal();
	return true;
}

void XMJGHouse::picturePosition()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	assist.DrawMesh(lb, rt, ZhaoPianShiYiTu);
	MVStr filter; VStr vals; filterObject(vals, _T("��Ƭλ��ʾ��ͼ����"));
	filter[_T("ͼ��")] = vals; assist.FilterEntitys(IdArr, filter);
	CString path = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("�����������Ƶ�ֲ��������������Ƭ����λ��ͼ"));
	path += _T("\\") + name;
	CSaveBlocks blocks;	CString pathName(path);
	if(pathName.Right(4) != ".dwg")	pathName.Append(_T(".dwg"));
	blocks.setPath(pathName); ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("��ӡ��Χ"));
	idArr.append(dyfw);	blocks.saveBlocks2(idArr, false);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	openDwgCommand(path);
}

void XMJGHouse::PicAndKZD()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	assist.DrawMesh(lb, rt, DLZhaoPianShiYiTu);
	MVStr filter; VStr vals;
	RoadfilterObj(vals);
	MIVStr filters;
	filters[FilterLayer] = vals;
	filter[_T("ͼ��")] = vals;
	assist.FilterEntitys(IdArr, filter);
	CString path = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("��·���Ƶ�ֲ��������������Ƭ����λ��ͼ"));
	path += _T("\\") + name;
	CSaveBlocks blocks;	CString pathName(path);
	if(pathName.Right(4) != ".dwg")	pathName.Append(_T(".dwg"));
	blocks.setPath(pathName); ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("��ӡ��Χ"));
	idArr.append(dyfw);	blocks.saveBlocks2(idArr, false);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	openDwgCommand(path);
}

void XMJGHouse::addCompass()
{
	SelectFilter f1(8, m_fcpm._layer);
	AcDbObjectIdArray objects;
	if(false == SelectEntitys(objects, f1, _T(""))) return;
	ads_point ptmin, ptmax, ptinsert;
	for(int idx = 0; idx < objects.length(); ++idx)
	{
		AcDbObjectId id = objects.at(idx);
		GetEntExtent(aname(id), ptmin, ptmax);
		AcGePoint2dArray rect;
		AcGePoint2d lb; lb.x = ptmax[X] - 3 * m_Scale; lb.y = ptmax[Y] - 3 * m_Scale;
		AcGePoint2d lt; lt.x = ptmax[X] - 3 * m_Scale; lt.y = ptmax[Y];
		AcGePoint2d rb; rb.x = ptmax[X]; rb.y = ptmax[Y] - 3 * m_Scale;
		AcGePoint2d rt; rt.x = ptmax[X]; rt.y = ptmax[Y];
		rect.append(lb); rect.append(lt);
		rect.append(rt); rect.append(rb);
		AcDbObjectId lid = DrawPolyline2D(rect, 1.0, true, _T(""));
		ptinsert[X] = ptmax[X] - 1.5 * m_Scale; ptinsert[Y] = ptmax[Y] - 1.5 * m_Scale;
		DrawInsert(ptinsert, _T("Compass"), _T(""), m_Scale);
	}
}

void XMJGHouse::lineConvertChangeArea()
{
	AcDbObjectId id;
	SelectFilter f1(RTDXF0, _T("*polyline"));
	if(false == SelectEntity(id, f1, _T(""))) return;
	setlayer(aname(id), m_bg._layer);
	setcolor(aname(id), m_bg._color);
	setChangeAreasInfo(id);
}

void XMJGHouse::AddFunctionAnnotion()
{
	SelectFilter f1(8, m_fcpm._layer);//��������ɸѡ�ֲ�ƽ����߽��ߵ�ͼ��ɸѡ����
	SelectFilter f2(RTDXF0, _T("*POLYLINE"));//��������ɸѡ�ֲ�ƽ����߽��ߵ�����ɸѡ����
	AcDbObjectIdArray objects, IdArr;
	if(false == SelectEntitys(objects, f1, f2, _T(""))) return;//ɸѡ���зֲ�ƽ�棬�����ǵ�id�Ÿ�ֵ��objects
	int len = objects.length();/*��÷ֲ�ƽ��ĸ���*/ ads_point pmin, pmax;
	TCHAR layer[200] = {0}; IProjectMDB pdb;//pdb�򿪵�ǰĿ¼��GC�ļ����е�mdb�ļ�������D:\����\0725_����·����������Ŀ\�滮\GC\��Ŀ��Ϣ.mdb
	TCHAR cwlayer[200] = {0};
	_stprintf(layer, _T("%s"), m_gnq._layer);//��������ͼ�㸳ֵ��layer
	_stprintf(cwlayer, _T("%s"), m_cw._layer);//����λͼ�㸳ֵ��cwlayer
	for(int idx = 0; idx < len; ++idx)//����ÿ���ֲ�ƽ��ͼ
	{
		AcDbObjectId id = objects.at(idx);//���һ���ֲ�ƽ��ͼ��id��
		AcGePoint2dArray node; AcDbObjectIdArray ids, IdArr;
		GetPlList(aname(id), node);//��÷ֲ�ƽ��ͼ�����нڵ�
		ssFromNodes(ids, node, 1, 1.0, 0, layer);//��÷ֲ�ƽ��ͼ�е����й�������id����
		GetEntExtent(aname(id), pmin, pmax);//��Χ�����½����ꡢ��Χ�����Ͻ�����
		ZoomWindow(pmin, pmax);//��󻯷ֲ�ƽ��ͼ
		double jzmj = 0.0, jrmj = 0.0;//�������jzmj���������jrmj
		getWaiBanQiangArea(id, jzmj, jrmj);//����ֲ�ƽ���е����ǽ�������jzmj�����ǽ�������jrmj
		TCHAR ldh[255] = {0}, lch[255] = {0};
		ReadXdata(aname(id), _T("¥����"), 0, ldh);//��ȡ�ֲ�ƽ���¥���ţ��ⶰ¥�����֣����������¼���԰��1#��1��¥����
		ReadXdata(aname(id), _T("¥���"), 0, lch);//��ȡ�ֲ�ƽ���¥��ţ�����¥��ţ�����"7;9"��ָ7��9��¥��"7 9"��ָ7��8��9�㣩
		CString LCH(lch); int pos = LCH.Find(_T(" "));//�ҵ�¥����еĿո�λ��
		if(pos != -1) _stprintf(lch, _T("%s"), LCH.Mid(0, pos));//¥����еĿո���������Ϊ��ʼ¥��ţ����������¥�㣬��Ϊ��һ���֣�����Ǽ��¥�㣬��Ϊ������7;9����ʽ
		MStr filter; filter[_T("¥����")] = ldh;//��filter�д�����¥���š����ԣ�����ֵΪldh
		filter[_T("��ʼ����")] = lch; CString temp;
		MStr info = pdb.getCXXTableInfo(filter);//����¥���ź���ʼ�����ҵ�CXX���еķֲ�ƽ����Ϣ�У�����ֵ��info
		ads_point loc;
		if(_ttoi(lch) > 0)
			temp.Format(_T("���%sm������ϵ��1.00"), info[_T("ʵ����")]);//���ϲ㣬����ϵ��Ϊ1.00
		else
			temp.Format(_T("���%sm������ϵ��0.00"), info[_T("ʵ����")]);//���²㣬����ϵ��Ϊ0.00
		addFunctionAnnotion(ids, temp, jzmj, loc);
		AcDbObjectIdArray cws;
		if(!ssFromNodes(cws, node, 1, 1.0, _T("*POLYLINE"), cwlayer)) continue;
		int cwCount = 0; map<CString, int> carmp;
		for(int indx = 0; indx < cws.length(); ++indx)
		{
			AcDbObjectId cwid = cws[indx]; TCHAR xdata[255] = {0};
			ReadXdata(aname(cwid), _T("CWSM"), 1, xdata);
			cwCount += _tstoi(xdata);
			TCHAR cwlx[255] = {0};
			ReadXdata(aname(cwid), _T("CWLX"), STRING_TYPE, cwlx);
			if(carmp.find(cwlx) != carmp.end())
				carmp[cwlx]++;
			else
				carmp.insert(make_pair(cwlx, 1));
		}
		//TCHAR text[255] = {0}; _stprintf(text, _T("�����ܼ�ͣ����λ%d��"), cwCount);
		CString strText;
		strText.Format(_T("�����ܼ�ͣ����λ%d��:"), cwCount);
		for(map<CString, int>::const_iterator it = carmp.begin(); it != carmp.end(); ++it)
		{
			CString t;
			if(it == carmp.begin())
				t.Format(_T("������%s%d����"), it->first, it->second);
			else
				t.Format(_T("%s%d����"), it->first, it->second);
			strText += t;
		}
		if(carmp.size() != 0)
			strText.Delete(strText.GetLength() - 1, 1);
		AcDbObjectId cwgsid = DrawText(loc, strText, _T("ML"), _T(""), m_fcmjzs._size * m_Scale, 0.0, 1.0);
		setlayer(aname(cwgsid), m_fcmjzs._layer);
	}
}

void XMJGHouse::splitLayer()
{
	// 	ads_name ent; ads_point pos; 
	// 	TCHAR layer[25] = {0};
	// 	TCHAR type[25] = {0};
	// 	int es = ads_entsel(_T("\n��ѡ����Ҫ��ֵ�¥�㣺<��ESC�˳�ѡ��>"), ent, pos);
	// 	while(es == RTNORM)
	// 	{
	// 		GetEntLayer(ent, layer);
	// 		GetEntityType(ent, type);
	// 		CString Type(type);
	// 		CString info = Type.MakeLower();
	// 		if(m_fcpm._layer.CompareNoCase(layer) == 0 && info.FindOneOf(_T("polyline") == 0) break;
	// 		es = ads_entsel(_T("\n��ѡ����Ҫ��ֵ�¥�㣺<��ESC�˳�ѡ��>"), ent, pos);
	// 	}

}

void XMJGHouse::tiQingMianJi()
{
	SelectFilter sf(8, m_gnq._layer);
	AcDbObjectIdArray ids; TCHAR info[20] = {0};
	if(!SelectEntitys(ids, sf, _T(""))) return;
	MSSId infos; TCHAR tj[20] = {0}, bh[20] = {0}, jc[25] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids[idx];
		ReadXdata(aname(id), _T("�ݼ�����"), 0, tj);
		if(infos.find(tj) == infos.end())
			infos[tj] = MSId();
		ReadXdata(aname(id), _T("����������"), 0, info);
		if(_tcscmp(info, _T("����滮")) != 0) continue;
		ReadXdata(aname(id), _T("���������"), 0, jc);
		ReadXdata(aname(id), _T("���������"), 0, bh);
		CString jcbh; jcbh.Format(_T("%s%s"), jc, bh);
		infos[tj][jcbh] = id;
	}
	ads_point pos; if(5100 != ads_getpoint(NULL, _T("\n��ѡ��Ҫע�ǵ�λ�ã�"), pos)) return;
	AcDbObjectId tid; double cztsd = 0.0;
	for(MSSIdIter sit = infos.begin(); sit != infos.end(); ++sit)
	{
		MSId msids = sit->second;
		for(MSIdIter it = msids.begin(); it != msids.end(); ++it)
		{
			AcDbObjectId id = it->second; double area = 0.0;
			GetEntArea(aname(id), area); cztsd += area;
		}
	}
	_stprintf(info, _T("TQMJ = %.3lf"), cztsd);
	tid = DrawText(pos, info, _T("ML"), _T(""), m_mjzj._size * m_Scale, 0.0, 1.0);
	setlayer(aname(tid), m_mjzj._layer); setcolor(aname(tid), m_mjzj._color);

}

AcGePoint2dArray getPointExtend(const AcGePoint2d &pos, const double &offset)
{
	AcGePoint2dArray node;
	AcGePoint2d lb(pos.x - offset, pos.y - offset);
	AcGePoint2d lt(pos.x - offset, pos.y + offset);
	AcGePoint2d rt(pos.x + offset, pos.y + offset);
	AcGePoint2d rb(pos.x + offset, pos.y - offset);
	node.append(lb); node.append(lt); node.append(rt); node.append(rb);
	return node;
}

AcDbObjectId drawAlignedDimension(const AcGePoint2d &spt, const AcGePoint2d &ept)
{
	IDataBaseOper oper; AcDbObjectId id;
	TextProperty prop1 = oper.readTextPropertyTable(_T("���������ע����"));
	TextProperty prop2 = oper.readTextPropertyTable(_T("���������ע����"));
	double angle = ads_angle(apoint(spt), apoint(ept)) + PI / 2;
	AcGePoint2d mpt;
	mpt.x = ((spt.x + ept.x) / 2);
	mpt.y = ((spt.y + ept.y) / 2);
	AcDbAlignedDimension *dim = new AcDbAlignedDimension(apoint(spt), apoint(ept), apoint(mpt));
	dim->setDimtxt(prop2._size);
	AcCmColor color; color.setColorIndex(prop2._color);
	dim->setDimclrd(color); dim->setDimclrt(color);
	dim->setDimclre(color);
	dim->setDimsah(true); dim->setDimtih(false);
	dim->setDimsd1(false); dim->setDimsd2(false);
	dim->setDimse1(false); dim->setDimse2(false);
	dim->setDimdec(2); dim->setDimtih(false);
	dim->setDimtad(1);
	if(!AppendEntity(dim, id))
	{
		delete dim; return AcDbObjectId::kNull;
	}
	dim->close();	setlayer(aname(id), prop2._layer);
	SetTextStyle(id, prop2._font, 1.0); return id;
}

CString getDirection(ads_point spt, ads_point ept)
{
	double rad = ads_angle(spt, ept);
	int angle = (int)((57.2957795130823208 * rad) * 100);
	int dir = angle / 2225 + 1;
	if(dir == 1 || dir == 16)return _T("��");
	if(dir == 2 || dir == 3)return _T("����");
	if(dir == 4 || dir == 5)return _T("��");
	if(dir == 6 || dir == 7)return _T("����");
	if(dir == 8 || dir == 9)return _T("��");
	if(dir == 10 || dir == 11)return _T("����");
	if(dir == 12 || dir == 13)return _T("��");
	if(dir == 14 || dir == 15) return _T("����");
	return "";
}

void XMJGHouse::distAnnotate()
{
	ads_point spt, ept; AcDbObjectIdArray ids1, ids2; int type = 2;
	TCHAR layer[255] = {0}, layer1[255] = {0};
	if(RTNORM != ads_getpoint(NULL, _T("\n��ѡ���ע����㣺"), spt)) return;
	AcGePoint2dArray node = getPointExtend(apoint(spt), 0.5);
	if(!ssFromNodes(ids1, node, type, 0.0, _T("*POLYLINE")/*, layer*/)) return;
	if(ids1.length() == 0) return;
	if(RTNORM != ads_getpoint(spt, _T("\n��ѡ���ע���յ㣺"), ept)) return;
	node = getPointExtend(apoint(ept), 0.5);
	if(!ssFromNodes(ids2, node, type, 0.0, _T("*POLYLINE")/*, layer*/)) return;
	if(ids2.length() == 0) return;
	AcDbObjectId id = drawAlignedDimension(apoint(spt), apoint(ept));
	CString dir = getDirection(spt, ept);
	double dist = distOF2d(apoint(spt), apoint(ept));
	TCHAR name1[255] = {0}, name2[255] = {0};
	for(int idx = 0; idx < ids1.length(); ++idx)
	{
		if(!ReadXdata(aname(ids1[idx]), _T("����������"), 0, name1))
		{
			GetEntLayer(ids1[idx], layer1);
			if(m_dxfwx._layer.CompareNoCase(layer1) == 0)
			{
				_stprintf(name1, _T("%s"), _T("�����ҷ�Χ��"));
				break;
			}
			else if(_tcscmp(layer1, _T("����")) == 0)
			{
				_stprintf(name1, _T("%s"), _T("����"));
				break;
			}
			else
				_stprintf(name1, _T("%s"), _T(""));
		}
	}
	for(int idx = 0; idx < ids2.length(); ++idx)
	{
		if(!ReadXdata(aname(ids2[idx]), _T("����������"), 0, name2))
		{
			GetEntLayer(ids2[idx], layer1);
			if(m_dxfwx._layer.CompareNoCase(layer1) == 0)
			{
				_stprintf(name2, _T("%s"), _T("�����ҷ�Χ��"));
				break;
			}
			else if(_tcscmp(layer1, _T("����")) == 0)
			{
				_stprintf(name2, _T("%s"), _T("����"));
				break;
			}
			else
				_stprintf(name2, _T("%s"), _T(""));
		}
	}
	if(_tcscmp(name1, _T("")) == 0 && _tcscmp(name2, _T("")) == 0)
		return;
	if(_tcscmp(name1, _T("����")) == 0 || _tcscmp(name2, _T("����")) == 0)
		AddXdata(aname(id), _T("��ע����"), 0, _T("�˺��߾����ע"));
	else if(m_dxfwx._layer.CompareNoCase(name1) || m_dxfwx._layer.CompareNoCase(name2))
		AddXdata(aname(id), _T("��ע����"), 0, _T("��������ע"));
	CString info; info.Format(_T("%s%s��%s%.2lf������"), name1, dir, name2, dist);
	AddXdata(aname(id), _T("���"), 0, info);
}

void XMJGHouse::checkGongNengQu()
{
	CBackScreen back;
	SelectFilter sf1(8, m_gnq._layer);
	SelectFilter sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids;
	if(!SelectEntitys(ids, sf1, sf2, _T("X"))) return;
	for(int idx = 0; idx < ids.length(); ++idx)
		DrawHatch(aname(ids[idx]), _T("ANSI31"), 0.3, 0, 0, 2, _T("0"));
	TCHAR info[255] = {0}; ads_getstring(0, _T(""), info);
}

void XMJGHouse::defineGongNengQu()
{
	ads_point pt; AcDbObjectId id; ads_initget(0, _T("B D I"));
	int ret = ads_getpoint(NULL, _T("\n���ڲ�ʰȡ��[�ڲ�(I)/�߽�(B)/����(D)]��"), pt);
	if(ret == RTNORM) id = createBoundary(pt);
	if(ret == RTKWORD)
	{
		TCHAR key[255] = {0};
		if(5100 != ads_getinput(key)) return;
		if(_tcscmp(key, _T("B")) == 0)
		{
			ads_name boundary;
			if(5100 != ads_entsel(_T("\n��ʰȡ�������߽磺"), boundary, pt)) return;
			if(Acad::eOk != acdbGetObjectId(id, boundary)) return;
		}
		else if(_tcscmp(key, _T("D")) == 0)
		{

		}
		else if(_tcscmp(key, _T("I")) == 0)
		{
			if(5100 != ads_getpoint(NULL, _T("\n���ڲ�ʰȡ�㣺"), pt)) return;
			id = createBoundary(pt);
		}
	}
	CSetEntityPropery prop;	prop.setGNQ(id); int type = 0; TCHAR info[20] = {0};
	ret = ads_getint(_T("\n���������ƣ�[0]��ͼ��ʰȡ��[1]�ֶ����룺<0>"), &type);
	if(type == 0)
	{
		ads_name ent;
		if(5100 != ads_entsel(_T("\n��ʰȡ���������ƣ�"), ent, pt)) return;
		GetEntText(ent, info);
	}
	else if(type == 1)
	{
		if(5100 != ads_getstring(0, _T("\n�����빦���������ƣ�"), info)) return;
	}
	AddXdata(aname(id), _T("���������"), 0, info);
	addFunctionAreaNote(id);
}

void XMJGHouse::checkBuildLayerAndMDB()
{
	SelectFilter sf1(8, m_fcpm._layer), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids; if(!SelectEntitys(ids, sf1, sf2, _T("X"))) return;
	CString buildName; VStr layerName; TCHAR szBuild[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids[idx]; memset(szBuild, 0, sizeof(szBuild));
		if(!ReadXdata(aname(id), _T("¥���"), 0, szBuild)) continue;
		layerName.push_back(szBuild); memset(szBuild, 0, sizeof(szBuild));
		if(!ReadXdata(aname(id), _T("¥����"), 0, szBuild)) continue;
	}
	if(_tcslen(szBuild) == 0) return;
	MStr filter; filter[_T("¥����")] = szBuild;
	IProjectMDB pdb; VMStr infos = pdb.getCXXTableInfoEx(filter);
	for(int idx = 0; idx < infos.size(); ++idx)
	{
		MStr info = infos[idx]; CString name = info[_T("¥���")];
		bool bFind = false;
		for(int indx = 0; indx < layerName.size(); ++indx)
		{
			if(name.CompareNoCase(layerName[indx]) == 0)
			{ bFind = true; break; }
		}
		if(bFind) continue;
		filter[_T("¥���")] = name; pdb.delCXXTableInfo(filter);
	}
}

void XMJGHouse::newSetLayerInfo()
{
	CNewSetLayerInfoDlg dlg;
	SelectFilter filter(8, m_jzwlk._layer);
	TCHAR info[200] = {0};
	if(false == SelectEntity(dlg.m_id, filter, _T("")))	return;
	dlg.DoModal();
}

void XMJGHouse::newQualityCheck()
{
	CQualityCheckReport dlg;
	dlg.DoModal();
}

void XMJGHouse::newCreateFCPMT()
{
	SelectFilter filter(8, m_jzwlk._layer);
	TCHAR info[200] = {0}; AcDbObjectId lid;
	if(false == SelectEntity(lid, filter, _T("")))	return;
	if(!ReadXdata(aname(lid), _T("¥����"), 0, info)) return;
	IProjectMDB pdb; VMStr records = pdb.getCXXTableInfo();
	VStr layerinfo;  CString slayer, elayer; XMJGPlan plan;
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx];
		if(record[_T("¥����")].CompareNoCase(info) != 0) continue;
		slayer = record[_T("��ʼ����")];
		elayer = record[_T("��ֹ����")];
		if(elayer.GetLength() != 0) slayer += _T(" ---> ") + elayer;
		layerinfo.push_back(slayer);
	}
	plan.setLayerInfo(layerinfo);
	plan.setObjectID(lid);
	plan.CreatePlan();
}

bool XMJGHouse::getIsOrNotJR()
{
	IDataBaseOper oper; IProjectMDB pdb;
	long standard = oper.readJRStartTime();
	MStr record = pdb.getJBXXTableInfo();
	long starttime = TimeConvertLong(record[_T("����ʱ��")]);
	return starttime > standard;
}

void XMJGHouse::copyFCPMT(const SplitMargeLayer &sml, bool isRight)
{
	ads_point ptmin, ptmax; AcDbObjectIdArray tids;
	GetEntExtent(aname(sml.m_id), ptmin, ptmax);
	CopyEntitys(sml.m_ids, tids);
	AcDbObjectId tid = CopyEntity(sml.m_id);
	tids.append(tid); TCHAR info[255] = {0};
	_stprintf(info, _T("%s ---> %s"), sml.m_sLayer, sml.m_eLayer);
	AddXdata(aname(tid), _T("¥���"), 0, info);
	double xMove = 50.0 + ptmax[0] - ptmin[0];
	MoveEntitys(tids, isRight ? xMove : -xMove, 0);
	ReadXdata(aname(tid), _T("�ֲ����"), 0, info);
	_stprintf(info, _T("%d"), _ttoi(info) + (isRight ? 1 : -1));
	AddXdata(aname(tid), _T("�ֲ����"), 0, info);
	if(sml.m_eLayer.GetLength() != 0)
		_stprintf(info, _T("%s��%s��ƽ��ͼ"), sml.m_sLayer, sml.m_eLayer);
	else
		_stprintf(info, _T("��%sƽ��ͼ"), sml.m_sLayer);
	GetEntExtent(aname(tid), ptmin, ptmax);
	AcGePoint3d mid = GetMiddlePoint(apoint(ptmin), apoint(ptmax));
	mid.y -= (ptmax[Y] - ptmin[Y]) / 2 + 15;
	AcDbObjectId ttid = DrawText(mid, info, _T("MC"), _T(""), m_Scale * 5);
	setlayer(aname(ttid), _T("FCPMMC"));
	CString handle = GetHandleByObject(ttid);
	AddXdata(aname(tid), _T("ƽ������"), 0, handle);
}

void XMJGHouse::moveFCPMT(const SplitMargeLayer &sml, bool isRight)
{
	FCPMTNameOper(sml, isRight);
	ads_point ptmin, ptmax; TCHAR info[255] = {0};
	GetEntExtent(aname(sml.m_id), ptmin, ptmax);
	double xMove = 50.0 + ptmax[0] - ptmin[0];
	xMove = isRight ? xMove : -xMove;
	MoveEntitys(sml.m_ids, xMove, 0);
	MoveEntity(aname(sml.m_id), xMove, 0);
	ReadXdata(aname(sml.m_id), _T("�ֲ����"), 0, info);
	_stprintf(info, _T("%d"), _ttoi(info) + (isRight ? 1 : -1));
	AddXdata(aname(sml.m_id), _T("�ֲ����"), 0, info);
}

void XMJGHouse::FCPMTNameOper(const SplitMargeLayer &sml, bool isRight)
{
	ads_point ptmin, ptmax;
	GetEntExtent(aname(sml.m_id), ptmin, ptmax);
	AcDbObjectIdArray tids; AcGePoint2dArray node;
	node.append(apoint(ptmin));
	node.append(AcGePoint2d(ptmax[0], ptmin[1]));
	node.append(AcGePoint2d(ptmax[0], ptmin[1] - 20));
	node.append(AcGePoint2d(ptmin[0], ptmin[1] - 20));
	ssFromNodes(tids, node, 1, 0.0, _T("*TEXT"), _T("FCPMMC"));
	if(sml.m_lOper == kRname)
	{
		CString name1, name2;
		if(sml.m_eLayer.GetLength() != 0)
		{
			name1.Format(_T("%s��%s��ƽ��ͼ"), sml.m_sLayer, sml.m_eLayer);
			name2.Format(_T("%s ---> %s"), sml.m_sLayer, sml.m_eLayer);
		}
		else
		{
			name1.Format(_T("��%sƽ��ͼ"), sml.m_sLayer);
			name2.Format(_T("%s ---> %s"), sml.m_sLayer);
		}
		SetEntText(aname(tids.first()), name1);
		AddXdata(aname(sml.m_id), _T("¥���"), 0, name2);
		return;
	}
	else if(sml.m_lOper == kDel)
	{
		EntsDel(tids); return;
	}
	double xMove = 50.0 + ptmax[0] - ptmin[0];
	xMove = isRight ? xMove : -xMove;
	MoveEntitys(tids, xMove, 0);
}

void XMJGHouse::newSplitMargeLayer()
{
	CNewSetLayerInfoDlg dlg(true); if(dlg.DoModal() != IDOK) return;
	VStr sortStyle; sortStyle.push_back(_T("�ֲ����"));
	SelectFilter sf1(8, m_fcpm._layer), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids; if(!SelectEntitys(ids, sf1, sf2, _T("X"))) return;
	sortObjectByXDataInt(ids, sortStyle); int offset = 0;
	SplitMargeLayers smls = dlg.getSplitMargeLayers();
	for(int idx = 0; idx < smls.size(); ++idx)
	{
		SplitMargeLayer sml = smls[idx];
		smls[idx].m_ids = getInnerObjects(sml.m_id);
	}

	for(int idx = 0; idx < smls.size(); ++idx)
	{
		SplitMargeLayer sml = smls[idx];
		if(sml.m_lOper == kCopy)
		{
			offset++;
			copyFCPMT(sml, true);
		}
		else if(sml.m_lOper == kDel)
		{
			FCPMTNameOper(sml, offset > 0);
			EntsDel(sml.m_ids);
			EntDel(aname(sml.m_id));
			offset--; continue;
		}
		else if(sml.m_lOper == kRname)
			FCPMTNameOper(sml, offset > 0);
		if(offset == 0) continue;
		if(sml.m_lOper == kNoOper)
			moveFCPMT(sml, offset > 0);
	}
}

void XMJGHouse::newDrawUpPlanning()
{
	CDrawUpPlanningDlg dlg;
	dlg.DoModal();
}

void XMJGHouse::newGHSTYSGSTBYZ()
{
	CmdEcho cmdEcho;
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	assist.DrawMesh(lb, rt, GHSTYSGSTBYZ);
	MVStr filter; VStr vals; filterObject(vals, _T("��ƽͼ��滮��ɲ�һ�¶���ʾ��ͼ����"));
	filter[_T("ͼ��")] = vals; assist.FilterEntitys(IdArr, filter);
	CString path = assist.getExportFilePath();
	CString name = assist.getExportFileName(_T("�滮��ͼ��ʩ����ͼ��һ��"));
	path += _T("\\") + name;
	CSaveBlocks blocks;	CString pathName(path);
	if(pathName.Right(4) != ".dwg")	pathName.Append(_T(".dwg"));
	blocks.setPath(pathName); ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("��ӡ��Χ"));
	idArr.append(dyfw);	blocks.saveBlocks2(idArr, false);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	openDwgCommand(path);
}

void XMJGHouse::setMainHeight()
{
	AGrid_ClearSheet(_T("����߶�"));
	SelectFilter f1(8, m_fcpm._layer); MIID miid;
	AcDbObjectIdArray IdArr; TCHAR info[200] = {0};
	if(false == SelectEntitys(IdArr, f1, _T(""))) return;
	for(int idx = 0; idx < IdArr.length(); ++idx)
	{
		AcDbObjectId id = IdArr.at(idx);
		AcDbObjectIdArray gnqids = getFunctionAreaIds(id);
		for(int indx = 0; indx < gnqids.length(); ++indx)
		{
			AcDbObjectId gnq = gnqids.at(indx);
			if(false == ReadXdata(aname(gnq), _T("����߶�"), 0, info)) continue;
			miid[id].append(gnq);
		}
	}
	MIIDIter it = miid.begin();
	for(; it != miid.end(); ++it)
	{
		if(it->second.length() == 1)
		{
			ReadXdata(aname(it->second.first()), _T("�������߶�"), 0, info);
			AddXdata(aname(it->first), _T("���"), 0, info);
			AddXdata(aname(it->first), _T("¥���"), 0, info);
		}
		else
		{
			AGrid_AppendMsgEnts(_T("����߶�"), _T("����������߶�"), _T("������Ϣ"), it->second);
		}
	}
	AGrid_ShowMe();
}

void XMJGHouse::setMezzanineInfo()
{
	int type = 0;
	if(RTCAN == ads_getint(_T("\n [0]���üв� [1]ȡ���в㣺0"), &type)) return;
	acutPrintf(_T("\n ��ѡ��Ҫ��Ӽв��ƽ��:"));
	SelectFilter sf(8, m_fcpm._layer); AcDbObjectIdArray ids;
	if(SelectEntitys(ids, sf, _T("")) == false) return;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(type != 0)
			RemoveXdata(aname(id), _T("�в�"));
		else
			AddXdata(aname(id), _T("�в�"), 0, _T("1"));
	}
}

void XMJGHouse::setCarProperty()
{
	SelectFilter sf(8, m_cw._layer);
	AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T(""))) return;
	setCarProperty(ids);
}

void XMJGHouse::settingProjectInfo()
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	CExportDocInfoDlg dlg;
	dlg.DoModal();
}

void XMJGHouse::addZPTElevation()
{
	IDataBaseOper oper; TCHAR guid[255] = {0};
	TextProperty prop = oper.readTextPropertyTable(_T("������������������"));
	SelectFilter sf(8, prop._layer);
	AcDbObjectId bid; if(false == SelectEntity(bid, sf, _T(""))) return;
	ReadXdata(aname(bid), _T("¥����"), 0, guid);
	double height = 0.0; int type = 1; ads_point pos;
	TCHAR block[255] = {0}, text[255] = {0}, bg[25] = {0};
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��Ҫ��ע��λ�ã�"), pos)) return;
	if(RTCAN == ads_getint(_T("\n �������ߵ����ͣ�[1]���⡢[2]���ڡ�[3]����"), &type)) return;
	if(type == 3)
	{
		IProjectMDB pdb;
		CString jzbg = pdb.getBGTable(guid, _T("�������"));
		_stprintf(bg, _T("%s"), jzbg);
	}
	else
	{
		if(RTNORM != ads_getreal(_T("\n �������ߣ�"), &height)) return;
		_stprintf(bg, _T("%.2lf"), height);
	}
	if(type == 3) _stprintf(block, _T("%s"), _T("���2"));
	else _stprintf(block, _T("%s"), _T("���1")); IProjectMDB pdb;
	AcDbObjectId id = DrawInsert(pos, block, m_Scale, m_Scale); ads_point ptres;
	ads_polar(pos, PI / 2, 1.2 * m_Scale, ptres); AcDbObjectId textid;
	if(type == 1)
	{
		_stprintf(text, _T("�����ƺ���%sm"), bg);
		AddXdata(aname(id), _T("BGLX"), 0, _T("�����ƺ���"));
		textid = DrawText(ptres, text, _T("BL"), _T(""), m_swdpbg._size * m_Scale);
		SetTextStyle(textid, m_swdpbg._font, 1.0); setcolor(aname(textid), m_swdpbg._color);
		setlayer(aname(textid), m_swdpbg._layer); pdb.setBGInfoTable(guid, _T("�����ƺ"), bg);
		setlayer(aname(id), m_swdpbg._layer);
	}
	else if(type == 2)
	{
		_stprintf(text, _T("���ڵ�����%sm"), bg);
		AddXdata(aname(id), _T("BGLX"), 0, _T("���ڵ�����"));
		textid = DrawText(ptres, text, _T("BL"), _T(""), m_sndpbg._size * m_Scale);
		SetTextStyle(textid, m_sndpbg._font, 1.0); setcolor(aname(textid), m_sndpbg._color);
		setlayer(aname(textid), m_sndpbg._layer); pdb.setBGInfoTable(guid, _T("���ڵ���"), bg);
		setlayer(aname(id), m_sndpbg._layer);
	}
	else if(type == 3)
	{
		_stprintf(text, _T("�������%sm"), bg);
		AddXdata(aname(id), _T("BGLX"), 0, _T("�������"));
		textid = DrawText(ptres, text, _T("BL"), _T(""), m_jzbg._size * m_Scale);
		SetTextStyle(textid, m_jzbg._font, 1.0); setcolor(aname(textid), m_jzbg._color);
		setlayer(aname(textid), m_jzbg._layer); pdb.setBGInfoTable(guid, _T("�������"), bg);
		setlayer(aname(id), m_jzbg._layer);
	}
	AcDbObjectId group;
	CreateGroup(aname(id), group);
	AddEntToGroup(textid, group);
}

void XMJGHouse::singleEdgeNote()
{
	// 	ads_name ent1, ent2;
	// 	ads_entlast(ent1);
	// 	ads_command(RTSTR, _T("_dimaligned"), RTSTR, PAUSE, 0);
	// 	ads_entlast(ent2);
	// 	if(ads_name_equal(ent1, ent2)) return;
	// 	AcDbEntity *pEnt = NULL;
	// 	if(Acad::eOk != acdbOpenAcDbEntity(pEnt, aname(ent2), AcDb::kForWrite)) return;
	// 	if(pEnt->isKindOf(AcDbAlignedDimension::desc()))
	// 	{
	// 		AcDbAlignedDimension *aligned = AcDbAlignedDimension::cast(pEnt);
	// 		AcCmColor clr; clr.setColorIndex(m_fwbczj._color);
	// 		aligned->setDimclrt(clr); aligned->setDimclrd(clr);
	// 		aligned->setLayer(m_fwbczj._layer);
	// 		aligned->setDimdec(2);
	// 		aligned->setDimblk(_T(""));
	// 		aligned->setDimsd1(true); aligned->setDimsd2(true);
	// 		aligned->setDimse1(true); aligned->setDimse2(true);
	// 	}
	// 	return;
	ads_point spt, ept, pos, sidept;
	if(RTNORM != ads_getpoint(NULL, _T("\n �߳�ע�ǵ����"), spt)) return;
	if(RTNORM != ads_getpoint(spt, _T("\n �߳�ע�ǵ��յ�"), ept)) return;
	pos[0] = (spt[0] + ept[0]) / 2; pos[1] = (spt[1] + ept[1]) / 2;
	if(RTNORM != ads_getpoint(NULL, _T("\n �߳�ע�Ƿ���һ��"), sidept)) return;
	AcDbAlignedDimension *aligned = new AcDbAlignedDimension(apoint(spt), apoint(ept), apoint(pos));
	AcGePoint2dArray nodes;
	nodes.append(asPnt2d(spt));
	nodes.append(asPnt2d(ept));
	// �жϵ��������㵽�յ�ָ��Ϊ�������ҡ��������ϣ�-1Ϊ�ң�1Ϊ��0Ϊ����
	int nSide = PointSideOfPolyline(asPnt2d(sidept), nodes);
	if (spt[X] > ept[X] || spt[Y] > ept[Y])nSide = 0 - nSide;
	aligned->setDimtxtdirection(false);
	switch (nSide)
	{
	case -1:aligned->setDimtad(1); break;
	case 0:aligned->setDimtad(0); break;
	case 1:aligned->setDimtad(4); break;
	default:;
	}
	aligned->setDimgap(0.4*m_Scale);
	aligned->setDimtxt(m_fwbczj._size); AcDbObjectId id;
	AcCmColor color; color.setColorIndex(m_fwbczj._color);
	aligned->setDimclrd(color); aligned->setDimclrt(color);
	aligned->setDimsah(true); aligned->setDimtih(false);
	aligned->setDimsd1(true); aligned->setDimsd2(true);
	aligned->setDimse1(true); aligned->setDimse2(true);
	aligned->setDimdec(2); aligned->setDimtih(false);
	aligned->setLayer(m_fwbczj._layer);
	if(!AppendEntity(aligned, id))
	{
		delete aligned;
		return;
	}
	//AcGePoint3d txtPt3d = aligned->textPosition();
	//int txtSide = PointSideOfPolyline(AcGePoint2d(txtPt3d.x, txtPt3d.y), nodes);
	aligned->close();	setlayer(aname(id), m_fwbczj._layer);
	SetTextStyle(id, m_fwbczj._font, 1.0); return;
	// 
	// 	//	AcGePoint2d midpt(pos[X], pos[Y] );
	// 	double dist = distOF2d(apoint(spt), apoint(ept));
	// 	CString dist_str;
	// 	dist_str.Format(_T("%.2f"), dist);
	// 	double angle = ads_angle(spt, ept);
	// 	// 	ads_point insertpt;
	// 	// 	ads_polar(apoint(midpt), angle + 0.5 * PI, 2 * m_Scale, insertpt);
	// 	if(angle > PI / 2 && angle < 3 * PI / 2)
	// 		angle -= PI;
	// 	AcDbObjectId textID = DrawText(pos, dist_str, _T("MC"), _T("20000"), m_fwbczj._size * m_Scale, angle, 1.0);
	// 	setcolor(aname(textID), m_fwbczj._color);
	// 	setlayer(aname(textID), m_fwbczj._layer);
}

void XMJGHouse::setLayerInfoDlg()
{
	SelectFilter sf(8, m_fcpm._layer); AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T(""))) return;
	CAcModuleResourceOverride myResources;	//�����ö������������Դ��ͻ����
	CSetLayerInfoDlg dlg; dlg.setObject(id);
	dlg.DoModal();
}

void XMJGHouse::exportXMFWX()
{
	CSaveBlocks blocks;	XMJGAssist assist; IDataBaseOper oper;
	CString savePath = assist.getExportFilePath();
	CString tempname = oper.readFileNameTable(_T("��Ŀ��Χ��"));
	blocks.setPath(savePath + _T("\\") + tempname);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	SelectFilter sf(8, _T("XMFW")); AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T(""))) return;
	TCHAR stdm[20] = {0}; GetStdm(aname(id), stdm);
	if(_tcscmp(_T("999950"), stdm) != 0) return;
	ads_name ssSave; AcDbObjectIdArray ids;
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	SSToIdArr(ssSave, ids); ids.remove(id);
	EntsDel(ids); ids.setPhysicalLength(0); ids.setLogicalLength(0);
	ids.append(id); blocks.saveBlocks2(ids, false);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	acutPrintf(_T("\n") + savePath + _T("\\") + tempname);
}

void XMJGHouse::setLayerInfo()
{
	CString path = GetCurrentDwgName();
	CString name = GetCurrentDwgFullPath();
	int pos = name.ReverseFind(_T('\\'));
	pos = pos == -1 ? name.ReverseFind(_T('/')) : pos;
	if(0 == path.CompareNoCase(name))
	{
		acutPrintf(_T("\n ���Ƚ���ǰ�ļ����б���"));
		return;
	}
	SelectFilter filter(8, m_jzwlk._layer);
	AcDbObjectId id; TCHAR info[200] = {0};
	if(false == SelectEntity(id, filter, _T("")))	return;
	if(false == ReadXdata(aname(id), _T("¥����"), 0, info))
	{
		AfxMessageBox(_T("��������¥����"));
		return;
	}
	CString savedir = name.Left(pos);
	XMJGPlanDlg dlg;
	dlg.setObjectID(id);
	if(IDOK != dlg.DoModal()) return;
}

void XMJGHouse::addChangeNote()
{
	SelectFilter sf(8, m_fcpm._layer); AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T(""))) return;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		MSId bgzj;
		AcDbObjectId id = ids.at(idx); ads_point pmin, pmax;
		TCHAR bgxx[24] = {0}; ads_point pos;
		AcGePoint2dArray node;	GetPlList(aname(id), node);
		GetEntExtent(aname(id), pmin, pmax);
		ZoomWindow(pmin, pmax);	AcDbObjectIdArray bgids;
		ssFromNodes(bgids, node); bgids.remove(id);
		for(int indx = 0; indx < bgids.length(); ++indx)
		{
			AcDbObjectId bgid = bgids.at(indx);
			if(ReadXdata(aname(bgid), _T("������"), 0, bgxx) == false) continue;
			bgzj[bgxx] = id;
		}
		if(bgzj.size() == 0) continue;
		if(RTCAN == ads_getpoint(NULL, _T("\n �������ע��λ�ã�"), pos)) return;
		MSIdIter it = bgzj.begin();
		for(; it != bgzj.end(); ++it)
		{
			TCHAR text[200] = {0}; _stprintf(text, _T("%s��"), it->first);
			AcDbObjectId textid = DrawText(pos, text, _T("ML"), _T(""), m_bgxxbz._size * m_Scale);
			SetTextStyle(textid, m_bgxxbz._font, 1.0);
			setlayer(aname(textid), m_bgxxbz._layer);
			setcolor(aname(textid), m_bgxxbz._color);
			pos[Y] -= (m_bgxxbz._size + 1.0) * m_Scale;
		}
	}

}

void XMJGHouse::getReplaceValue(MStr &field)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	field[_T("#GDSYTTITLE#")] = record[_T("��Ŀ����")];
	field[(_T("#CHECKCODE#"))] = record[_T("���ֺ�")];
	field[(_T("#JINGBANREN#"))] = record[_T("������")];
	field[(_T("#ACCEPTCODE#"))] = record[_T("�����")];
	field[(_T("#BUILDAREA#"))] = record[_T("�õ����")];
	field[(_T("#LicenseDoc#"))] = record[_T("���֤��")];
	field[_T("#BASEPOINT#")] = record[_T("ͼ���������")];
	field[(_T("#ProjectName#"))] = record[_T("��Ŀ����")];
	field[(_T("#TELEPHONENUM#"))] = record[_T("��ϵ�绰")];
	field[_T("#CONTROLPOINT#")] = record[_T("���Ƶ����")];
	field[(_T("#BuildCompany#"))] = record[_T("���赥λ")];
	field[_T("#MeansureCompany#")] = record[_T("������λ")];
	field[_T("#GHGGJZPT#")] = record[_T("�滮������������")];
	field[_T("#SCGGJZPT#")] = record[_T("ʵ�⹫����������")];
	field[(_T("#ProjectAddress#"))] = record[_T("��Ŀ�ص�")];
	field[_T("#DetailConvery#")] = record[_T("ϸ���������")];
}

void XMJGHouse::getReplaceValue(MStr &field, const CString &title)
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	field[_T("#GDSYTTITLE#")] = record[_T("��Ŀ����")];
	field[(_T("#CHECKCODE#"))] = record[_T("���ֺ�")];
	field[(_T("#JINGBANREN#"))] = record[_T("������")];
	field[(_T("#ACCEPTCODE#"))] = record[_T("�����")];
	field[(_T("#BUILDAREA#"))] = record[_T("�õ����")];
	field[(_T("#LicenseDoc#"))] = record[_T("���֤��")];
	field[_T("#BASEPOINT#")] = record[_T("ͼ���������")];
	field[(_T("#ProjectName#"))] = record[_T("��Ŀ����")];
	field[(_T("#TELEPHONENUM#"))] = record[_T("��ϵ�绰")];
	field[_T("#CONTROLPOINT#")] = record[_T("���Ƶ����")];
	field[(_T("#BuildCompany#"))] = record[_T("���赥λ")];
	field[_T("#MeansureCompany#")] = record[_T("������λ")];
	field[_T("#GHGGJZPT#")] = record[_T("�滮������������")];
	field[_T("#SCGGJZPT#")] = record[_T("ʵ�⹫����������")];
	field[(_T("#ProjectAddress#"))] = record[_T("��Ŀ�ص�")];
	field[_T("#DetailConvery#")] = record[_T("ϸ���������")];
	field[_T("#BGTTitle#")].Format(_T("%s%s"), record[_T("��Ŀ����")], title);
}

int XMJGHouse::GetCarSerialNum()
{
	int Serial = 0;
	CXRecordOperate recordOperate;
	if(recordOperate.ReadIntegerRecord(_T("XMJG"), _T("CWBZ"), Serial) == FALSE)
		Serial = 0;
	Serial++;
	return Serial;
}

bool XMJGHouse::selectExportObject()
{
	SelectFilter filter(8, m_fcpm._layer);
	if(false == SelectEntitys(m_object, filter, _T(""))) return false;
	return true;
}

void XMJGHouse::getExportObjects(AcDbObjectId &id, AcDbObjectIdArray &ids)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	ads_point ptmin, ptmax;
	GetEntExtent(aname(id), ptmin, ptmax, 1.0);
	ZoomWindow(ptmin, ptmax);
	ssFromNodes(ids, nodes);
	ids.remove(id);
	// 	TCHAR handle[10] = {0};
	// 	ReadXdata(aname(id), _T("ƽ������"), 0, handle);
	// 	AcDbObjectId title = GetObjectByHandle(handle);
	// 	if(title != AcDbObjectId::kNull)
	// 		ids.append(title);
}

void XMJGHouse::getFloorHeight(AcDbObjectId &id, MIStoreyInfo &floorHeight)
{
	TCHAR floor[20] = {0}; TCHAR height[20] = {0};
	TCHAR width[20] = {0}; TCHAR wall[20] = {0};
	ReadXdata(aname(id), _T("¥���"), 0, floor);
	ReadXdata(aname(id), _T("���"), 0, height);
	ReadXdata(aname(id), _T("���ݿ��"), 0, width);
	ReadXdata(aname(id), _T("Ů��ǽ"), 0, wall);
	int count = GetXDataCount(aname(id), _T("���"));
	for(int idx = 0; idx < count; ++idx)
	{
		TCHAR info[20] = {0}, layeyBh[255] = {0};
		StoreyInfo si;
		si.setObjectId(id);
		READMulXData(id, _T("���"), layeyBh, 0, idx);
		si.setFloorName(layeyBh);
		READMulXData(id, _T("���"), info, 0, idx);
		si.setHeight(_tstof(info));
		si.setWidth(_tstof(width));
		si.setParapetWall(_tstoi(wall) == 1);
		floorHeight[layeyBh] = si;
	}
}

void XMJGHouse::getFloorHeight(AcDbObjectId &id, MIStoreyInfo &floorHeight, const double &width)
{
	TCHAR floor[20] = {0}; TCHAR height[20] = {0};
	TCHAR wall[20] = {0}; CString linfo;
	ReadXdata(aname(id), _T("¥���"), 0, floor);
	ReadXdata(aname(id), _T("���"), 0, height);
	ReadXdata(aname(id), _T("Ů��ǽ"), 0, wall);
	int count = GetXDataCount(aname(id), _T("���"));
	for(int idx = 0; idx < count; ++idx)
	{
		TCHAR info[20] = {0}; StoreyInfo si;
		si.setObjectId(id);
		READMulXData(id, _T("���"), info, 0, idx);
		if(*info != _T('-') && false == stringIsDigit(info))
			linfo = g_fieldchagetable.getIndexByName(info);
		else linfo = info;
		si.setFloorName(linfo);
		READMulXData(id, _T("���"), info, 0, idx);
		si.setHeight(_tstof(info));	si.setWidth(width);
		si.setParapetWall(_tstoi(wall) == 1);
		floorHeight[linfo] = si;
	}
}

void XMJGHouse::getFloorHeight(AcDbObjectId &id, MDStoreyInfo &floorHeight, const double &width)
{
	TCHAR floor[20] = {0}; TCHAR height[20] = {0};
	TCHAR wall[20] = {0}; CString linfo;
	ReadXdata(aname(id), _T("¥���"), 0, floor);
	ReadXdata(aname(id), _T("���"), 0, height);
	ReadXdata(aname(id), _T("Ů��ǽ"), 0, wall);
	int count = GetXDataCount(aname(id), _T("���"));
	int step = (int)(1.0 / count);
	for(int idx = 0; idx < count; ++idx)
	{
		TCHAR info[20] = {0}; StoreyInfo si;
		si.setObjectId(id);
		READMulXData(id, _T("���"), info, 0, idx);
		if(*info != _T('-') && false == stringIsDigit(info))
			linfo = g_fieldchagetable.getIndexByName(info);
		else linfo = info;
		int layernum = _tstoi(linfo); si.setFloorName(linfo);
		READMulXData(id, _T("���"), info, 0, idx);
		si.setHeight(_tstof(info));	si.setWidth(width);
		if(_ttoi(wall) == 1)
			si.setParapetWall(true);
		else si.setParapetWall(false);
		floorHeight[layernum] = si;
	}
}

bool XMJGHouse::DrawHightlySchematic(const MIStoreyInfo &floofHeight, AcDbObjectIdArray & IdArr, double ft, const TCHAR *ldname)
{
	if(floofHeight.size() == 0) return false;
	CMIStoryInfoIter it = floofHeight.begin(); int index = 1;
	double width = it->second.getWidth(); double y = 0, swdp = y;
	AcGePoint3d spt, ept, mid;
	double height = 0.0;
	drawUnderground(height, it, IdArr);
	drawUnderFuTu(height, ft, width, IdArr);
	drawSubsurface(height, it, IdArr);
	drawDiPingXian(height, width, swdp, IdArr, ldname);
	TCHAR layercount[200] = {0};
	spt[X] = 0; spt[Y] = height;
	double maxHeight = 0.0, lastHeight = 0.0;
	mid.x = width / 2; mid.y = height;
	CString bgwz; int tjpos = -1; IProjectMDB pdb;
	bgwz = pdb.getBGTable(ldname, _T("���λ��"));
	for(; it != floofHeight.end(); ++it)
	{
		StoreyInfo info = it->second;
		CString floorname;
		double lh = info.getHeight();
		int sLayer = _ttoi(info.getStartLayer());
		int eLayer = _ttoi(info.getEndLayer());
		float temp = _tstof(info.getStartLayer());
		if(fabs(temp - sLayer - 0.5) < EPS) continue;		//�в㲻����
		if(eLayer == 0 && sLayer != 0) eLayer = sLayer;
		if(sLayer == 0 && eLayer == 0)
		{
			drawOneLayer(spt.y, info.getStartLayer(), info, IdArr); mid.y += lh / 2;
			if(info.getHeight() > EPS)
			{
				AcDbObjectId textid = DrawText(mid, info.getStartLayer(), _T("MC"), _T(""), m_lmtcmc._size * m_Scale, 0, 1.0);
				setlayer(aname(textid), m_lmtcmc._layer); setcolor(aname(textid), m_lmtcmc._color);
				IdArr.append(textid);	mid.y += lh / 2; spt.y += lh; lastHeight = 0.0;
			}
		}
		else
		{
			lastHeight = 0.0;
			for(int idx = sLayer; idx <= eLayer; ++idx)
			{
				floorname = g_fieldchagetable.getNameByIndex(idx);
				drawOneLayer(spt.y, floorname, info, IdArr); mid.y += lh / 2;
				AcDbObjectId textid = DrawText(mid, floorname, _T("MC"), _T(""), m_lmtcmc._size * m_Scale, 0.0, 1.0);
				setlayer(aname(textid), m_lmtcmc._layer); setcolor(aname(textid), m_lmtcmc._color);
				IdArr.append(textid); mid.y += lh / 2; spt.y += lh;
				// 				if(info.getType().CompareNoCase(_T("ƽ����")) == 0)
				// 				{
				// 					if(info.getparam1() < info.getparam2())
				// 						lastHeight = info.getparam2();
				// 					else if(info.getparam1() > info.getparam2())
				// 						lastHeight = info.getparam1();
				// 					else lastHeight = 0.0;
				// 				}
			}
		}
		if(bgwz.CompareNoCase(_T("�ݼ�")) == 0)
		{ maxHeight = info.getparam2(); tjpos = _ttoi(info.getparam3()); }
		else if(bgwz.CompareNoCase(_T("Ů��ǽ")) == 0)
			maxHeight = info.getparam1();
		else
			maxHeight = 0.0;
	}
	ads_point tspt; tspt[X] = width; tspt[Y] = swdp;
	//	if(fabs(lastHeight) > EPS) maxHeight += lastHeight;
	ads_point tept; tept[X] = width; tept[Y] = spt.y + maxHeight;									//��ע�ĵڶ���
	ads_point tmpt; tmpt[X] = width + 3; tmpt[Y] = (tept[Y] + tspt[Y]) / 2;							//��ע��λ��
	ads_command(RTSTR, _T("DIMLINEAR"), RTPOINT, tspt, RTPOINT, tept, RTSTR, _T("V"), RTPOINT, tmpt, RTNONE);
	ads_name ent; ads_entlast(ent); SetTextStyle(ent, m_jzbg._font, 1.0); setcolor(ent, m_jzbg._color);
	setlayer(ent, m_jzbg._layer); IdArr.append(aname(ent));

	double jzgd = tept[Y] - tspt[Y] + _tstof(pdb.getBGTable(ldname, _T("�����ƺ")));
	ads_point jzdppos; jzdppos[X] = width; jzdppos[Y] = spt.y + maxHeight;
	if(tjpos != -1) jzdppos[0] = width * (2 * tjpos + 1) / 6; ads_point ptres;
	AcDbObjectId id1 = DrawInsert(jzdppos, _T("���1"), m_Scale, m_Scale); IdArr.append(id1);
	ads_polar(jzdppos, PI / 2, 1.2 * m_Scale, ptres); TCHAR text[255] = {0};
	_stprintf(text, _T("�������%.2lfm"), jzgd);
	AcDbObjectId textid1 = DrawText(ptres, text, _T("BL"), _T(""), m_swdpbg._size * m_Scale);  IdArr.append(textid1);
	SetTextStyle(textid1, m_jzbg._font, 1.0); setcolor(aname(textid1), m_jzbg._color);
	setlayer(aname(textid1), m_jzbg._layer); _stprintf(text, _T("%.2lf"), jzgd);
	pdb.setBGInfoTable(ldname, _T("�������"), text);
	return true;
}

void XMJGHouse::drawMezzanineInfo(const StoreyInfo &info, double height, AcDbObjectIdArray &del)
{
	AcGePoint2d lb, lt, rt, rb;
	AcGePoint2dArray nodes; TCHAR szStr[20] = {0};
	if(ReadXdata(aname(info.getObjectId()), _T("�в�"), 0, szStr) == false) return;
	lb.x = 0; lb.y = height + info.getHeight() * 2 / 3;
	lt.x = 0; lt.y = height + info.getHeight();
	rt.x = info.getWidth() / 3; rt.y = height + info.getHeight();
	rb.x = info.getWidth() / 3; rb.y = height + info.getHeight() * 2 / 3;
	nodes.append(lb); nodes.append(lt);
	nodes.append(rt); nodes.append(rb);
	AcDbObjectId lineid = DrawPolyline2D(nodes, 1.0, true, _T(""));
	ads_point pos; pos[X] = (lb.x + rb.x) / 2;
	pos[Y] = (lb.y + rb.y) / 2;
	ReadXdata(aname(info.getObjectId()), _T("���"), 0, szStr);
	CString layername;
	layername.Format(_T("�в�%s"), g_fieldchagetable.getNameByIndex(szStr).Mid(2));
	AcDbObjectId textid = DrawText(pos, layername, _T("MC"), _T(""), 4.0 / 3);
	del.append(lineid);  del.append(textid);
}

void XMJGHouse::setHighlySchematicPath()
{
	AcDbObjectId id = m_object.first();
	CString path = GetCurrentDwgPrefix();
	TCHAR info[255] = {0};
	ReadXdata(aname(id), _T("¥����"), 0, info);
	m_FloorName.Format(_T("%s"), info);
	m_savePath.Format(_T("%s%s"), path, m_FloorName);
}

void XMJGHouse::drawOneLayer(double height, const CString &layer, const StoreyInfo &info, AcDbObjectIdArray &del)
{
	if(info.getParapetWall())
		drawParapetWall(height, info, del);
	else
		drawNoParapetWall(height, layer, info, del);
	drawHeightText(height, info, del);
	drawWuMianEntity(height, info, del);
	drawMezzanineInfo(info, height, del);
}

void XMJGHouse::drawUnderground(double &height, CMIStoryInfoIter &its, AcDbObjectIdArray &del)
{
	int slayer = 0;
	do
	{
		StoreyInfo info = its->second;
		CString layer = info.getStartLayer();
		slayer = _ttoi(layer); if(slayer > 0) return;
		for(int idx = 0; idx < info.getlayerCount(); ++idx)
		{
			drawOneLayer(height, layer, info, del);
			AcGePoint3d mid; mid.x = info.getWidth() / 2;
			mid.y = height + info.getHeight() / 2;
			layer = info.getStartLayer();
			CString name = g_fieldchagetable.getNameByIndex(layer);
			if(name.GetLength() == 0) name = layer;
			AcDbObjectId textid = DrawText(mid, name, _T("MC"), _T(""), m_lmtcmc._size * m_Scale, 0.0, 1.0);
			setlayer(aname(textid), m_lmtcmc._layer);
			setcolor(aname(textid), m_lmtcmc._color);
			height += info.getHeight(); del.append(textid);
		}
		its++;
	} while(slayer < -1);
}

void XMJGHouse::drawSubsurface(double &height, CMIStoryInfoIter &its, AcDbObjectIdArray &del)
{
	if(its->first.CompareNoCase(_T("0")) != 0) return;
	StoreyInfo info = its->second; AcGePoint2dArray node; its++;
	double sh = info.getHeight(); double width = info.getWidth();
	if(sh < EPS) return;
	node.append(AcGePoint2d(0, height + sh)); node.append(AcGePoint2d(0, height));
	node.append(AcGePoint2d(width, height)); node.append(AcGePoint2d(width, height + sh));
	AcGePoint3d pos; pos.x = width / 2; pos.y = height + sh / 2;
	AcDbObjectId tid = DrawText(pos, _T("����²�"), _T("MC"), _T(""), m_lmtcmc._size * m_Scale, 0.0, 1.0);
	AcDbObjectId lid = DrawPolyline2D(node, 1.0, true, _T("")); del.append(tid); del.append(lid);
	drawHeightText(height, info, del); height += sh;
}

void XMJGHouse::drawUnderFuTu(double &height, double ft, double width, AcDbObjectIdArray &del)
{
	AcGePoint2dArray node; if(height < EPS) return;
	node.append(AcGePoint2d(0, height + ft)); node.append(AcGePoint2d(0, height));
	node.append(AcGePoint2d(width, height)); node.append(AcGePoint2d(width, height + ft));
	AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T(""));
	AcDbObjectId tc = DrawHatch_Auto(aname(id), _T("411b"), 1, 0, NULL, 7, _T(""));
	del.append(id); del.append(tc); height += ft;
}

void XMJGHouse::drawParapetWall(double height, const StoreyInfo &info, AcDbObjectIdArray & del)
{
	AcGePoint2d lb, lt, rt, rb;
	AcGePoint2dArray nodes;
	lb.x = 0; lb.y = height;
	lt.x = 0; lt.y = height + info.getHeight();
	rt.x = info.getWidth(); rt.y = height + info.getHeight();
	rb.x = info.getWidth(); rb.y = height;
	nodes.append(lb); nodes.append(lt);
	nodes.append(rt); nodes.append(rb);
	AcDbObjectId lineid = DrawPolyline2D(nodes, 1.0, true, _T(""));
	del.append(lineid);
}

void XMJGHouse::drawNoParapetWall(double height, const CString &layer, const StoreyInfo &info, AcDbObjectIdArray & del)
{
	if(info.getHeight() < EPS) return;
	AcGePoint2d lb, lt, rt, rb;
	AcGePoint2dArray nodes; lb.x = 0; lb.y = height;
	lt.x = 0; lt.y = height + info.getHeight();
	rt.x = info.getWidth(); rt.y = height + info.getHeight();
	rb.x = info.getWidth(); rb.y = height;
	nodes.append(lb); nodes.append(lt);
	nodes.append(rt); nodes.append(rb);
	AcDbObjectId lineid = DrawPolyline2D(nodes, 1.0, true, _T(""));
	del.append(lineid); setlayer(aname(lineid), _T("GDSYT"));
	TCHAR cg[24] = {0}; _stprintf(cg, _T("%.3lf"), info.getHeight());
	AddXdata(aname(lineid), _T("CG"), 0, cg);
	AddXdata(aname(lineid), _T("CH"), 0, layer);
}

void XMJGHouse::drawWuMianEntity(double height, const StoreyInfo &info, AcDbObjectIdArray &del)
{
	height += info.getHeight(); double lower, upper;
	AcGePoint2dArray node; double width = info.getWidth();
	if(info.getType().CompareNoCase(_T("ƽ����")) == 0)
	{
		double pos1, pos2; double wid = width / 3;
		if(info.getparam3().CompareNoCase(_T("0")) == 0)
			pos1 = 0.0;
		else if(info.getparam3().CompareNoCase(_T("1")) == 0)
			pos1 = wid;
		else
			pos1 = wid * 2; pos2 = pos1 + wid; AcGePoint3d mid(pos1 + wid / 2, height + info.getparam2() / 2, 0);
		if(fabs(info.getparam2()) > EPS)
		{
			node.append(AcGePoint2d(pos1, height)); node.append(AcGePoint2d(pos1, height + info.getparam2()));
			node.append(AcGePoint2d(pos2, height + info.getparam2())); node.append(AcGePoint2d(pos2, height));
			AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T("")); del.append(id);
			AcDbObjectId textid1 = DrawText(mid, _T("�ݼ�"), _T("MC"), _T(""), m_lmtcmc._size * m_Scale);
			setlayer(aname(textid1), m_lmtcmc._layer); setcolor(aname(textid1), m_lmtcmc._color);
			setlayer(aname(id), m_lmtcmc._layer); del.append(textid1);
			mid[0] = pos2 - 1; mid[1] = height + info.getparam2() / 2; mid[2] = 0.0;
			TCHAR text[25] = {0}; _stprintf(text, _T("%.2lf"), info.getparam2());
			AcDbObjectId textid2 = DrawText(mid, text, _T("MC"), _T(""), m_lmtgdzj._size * m_Scale, PI / 2);
			setlayer(aname(textid2), m_lmtgdzj._layer); setcolor(aname(textid2), m_lmtgdzj._color); del.append(textid2);
		}
		drawNvErQiangEntity(height, info, del); lower = height + info.getparam1();
		upper = height + info.getparam2(); m_height = lower > upper ? lower : upper;
	}
	else if(info.getType().CompareNoCase(_T("б����")) == 0)
	{
		double lower = height + info.getparam1(), upper = height + info.getparam2(); double mid = width / 2;
		node.append(AcGePoint2d(0.0, lower)); node.append(AcGePoint2d(mid, upper)); node.append(AcGePoint2d(width, lower));
		node.append(AcGePoint2d(width, height)); node.append(AcGePoint2d(0.0, height));
		AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T("")); del.append(id);
		setlayer(aname(id), m_lmtcmc._layer);
		m_height = _tstof(info.getparam3()) >= 45 ? upper : lower;
	}
}

AcDbObjectIdArray XMJGHouse::drawLeader(const ads_point spt, const ads_point ept, const CString &info)
{
	AcDbObjectId tid;
	AcDbMText *text = new AcDbMText;
	text->setContents(info);
	text->setAttachment(AcDbMText::kBottomLeft);
	text->setLocation(AcGePoint3d(ept[X] + 0.5, ept[Y], 0));
	text->setTextHeight(m_Scale * m_jzbg._size);
	if(!AppendEntity(text, tid))
	{
		delete text;
		return AcDbObjectIdArray();
	} text->close();

	AcDbLeader *leader = new AcDbLeader;
	leader->appendVertex(apoint(spt));
	leader->appendVertex(apoint(ept));
	leader->setLayer(m_jzbg._layer);
	leader->setDimasz(m_jzbg._size);
	AcDbObjectId lid;
	if(!AppendEntity(leader, lid))
	{
		delete text;
		delete leader;
		return AcDbObjectIdArray();
	}
	leader->attachAnnotation(tid);
	leader->close();
	AcDbObjectIdArray ids;
	ids.append(tid); ids.append(lid);
	return ids;
}

void XMJGHouse::drawNvErQiangEntity(double height, const StoreyInfo &info, AcDbObjectIdArray &del)
{
	AcGePoint2dArray node;
	AcDbObjectId nvq1, nvq2;
	double offset = info.getWidth() - 0.2;
	ads_point spt, ept, mpt;
	CString loc = info.getparam3();
	CString name = (info.getparam1() < 0.8) ? _T("�ܿ�") : _T("Ů��ǽ");
	if(loc.CompareNoCase(_T("0")) == 0 || loc.CompareNoCase(_T("1")) == 0)
	{
		node.append(AcGePoint2d(offset, height)); node.append(AcGePoint2d(offset, height + info.getparam1()));
		node.append(AcGePoint2d(offset + 0.2, height + info.getparam1())); node.append(AcGePoint2d(offset + 0.2, height));
		AcDbObjectId lid = DrawPolyline2D(node, 1.0, true, _T("")); del.append(lid);
		setlayer(aname(lid), m_lmtcmc._layer);
		mpt[X] = ept[X] = spt[X] = offset; mpt[Y] = ept[Y] = spt[Y] = height;
		ept[Y] += info.getparam1(); mpt[Y] += info.getparam1() / 2; mpt[X] -= 0.5;
		ads_command(RTSTR, _T("DIMLINEAR"), RTPOINT, spt, RTPOINT, ept, RTSTR, _T("V"), RTPOINT, mpt, RTNONE);
		ads_name ent; ads_entlast(ent); SetTextStyle(ent, m_jzbg._font); setcolor(ent, m_jzbg._color);
		setlayer(ent, m_jzbg._layer); del.append(aname(ent));
		ads_point_set(ept, spt); ads_polar(spt, PI * 5 / 6, 10, ept);
		del.append(drawLeader(spt, ept, name));

	}
	if(loc.CompareNoCase(_T("2")) == 0 || loc.CompareNoCase(_T("1")) == 0)
	{
		node.append(AcGePoint2d(0.0, height)); node.append(AcGePoint2d(0.0, height + info.getparam1()));
		node.append(AcGePoint2d(0.2, height + info.getparam1())); node.append(AcGePoint2d(0.2, height));
		AcDbObjectId lid = DrawPolyline2D(node, 1.0, true, _T("")); del.append(lid);
		setlayer(aname(lid), m_lmtcmc._layer);
		mpt[X] = ept[X] = spt[X] = 0.0; mpt[Y] = ept[Y] = spt[Y] = height;
		ept[Y] += info.getparam1(); mpt[Y] += info.getparam1() / 2; mpt[X] -= 0.5;
		ads_command(RTSTR, _T("DIMLINEAR"), RTPOINT, spt, RTPOINT, ept, RTSTR, _T("V"), RTPOINT, mpt, RTNONE);
		ads_name ent; ads_entlast(ent); SetTextStyle(ent, m_jzbg._font); setcolor(ent, m_jzbg._color);
		setlayer(ent, m_jzbg._layer); del.append(aname(ent));
		ads_point_set(ept, spt); ads_polar(spt, PI * 5 / 6, 10, ept);
		del.append(drawLeader(spt, ept, name));
	}
}

void XMJGHouse::drawHeightText(double height, const StoreyInfo &info, AcDbObjectIdArray & del)
{
	if(info.getHeight() < EPS) return; AcDbObjectId lid = del.last();
	AcGePoint3d textpos; TCHAR text[20] = {0}; _stprintf(text, _T("%.2f"), info.getHeight());
	textpos.x = info.getWidth() - 2.0 * m_Scale; textpos.y = height + info.getHeight() / 2;
	AcDbObjectId textid = DrawText(textpos, text, _T("MC"), _T(""), m_lmtgdzj._size * m_Scale, PI / 2);
	setlayer(aname(textid), m_lmtgdzj._layer); setcolor(aname(textid), m_lmtgdzj._color);
	del.append(textid); CString handle = GetHandleByObject(textid);
	AddXdata(aname(lid), _T("CGZJ"), 0, handle);
}

FunctionNameTableEx XMJGHouse::setFunctionalAreaInfo(const AcDbObjectId &id)
{
	FunctionNameTableEx fnt; fnt.m_init = false;
	ads_point ptmin, ptmax; GetScreen(ptmin, ptmax);
	if(id == AcDbObjectId::kNull) return fnt;
	TCHAR height[255] = {0}, ldh[255] = {0}, lch[255] = {0};
	SelectFilter sf1(8, m_fcpm._layer), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids; SelectEntitys(ids, sf1, sf2, _T("X"));
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectIdArray tmps = getInnerObjects(ids[idx]);
		if(!tmps.contains(id)) continue;
		ReadXdata(aname(ids[idx]), _T("¥����"), 0, ldh);
		ReadXdata(aname(ids[idx]), _T("¥���"), 0, lch);
		CString info(lch); int pos = info.Find(_T(" "));
		if(pos != -1) info = info.Mid(0, pos + 1);
		IProjectMDB pdb; MStr filt;
		filt[_T("¥����")] = ldh; filt[_T("��ʼ����")] = info;
		MStr record = pdb.getCXXTableInfo(filt);
		_stprintf(height, _T("%s"), record[_T("ʵ����")]);
	}
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGFunctionsAreasInfo info;
	info.setEntInfo(id, height);
	ZoomWindow(ptmin, ptmax);
	if(IDOK != info.DoModal()) return fnt;
	fnt = info.getFNTInfo();
	return fnt;
}

void XMJGHouse::addFunctionAreaNote(const AcDbObjectId &id)
{
	IDataBaseOper oper; TCHAR szHandle[255] = {0};
	ReadXdata(aname(id), _T("ZJHANDLE"), 0, szHandle);
	AcDbObjectId toldid = GetObjectByHandle(szHandle); EntDel(aname(toldid));
	TextProperty prop = oper.readTextPropery(_T("������������������"), true, true);
	AcDbObjectId group; CreateGroup(aname(id), group);
	TCHAR name[200] = {0}; ads_point mid;
	AcGePoint2dArray node; GetPlList(aname(id), node);
	GetNodesMiddlePoint(node, mid); TCHAR jc[25] = {0};
	ReadXdata(aname(id), _T("���������"), 0, name);
	ReadXdata(aname(id), _T("���������"), 0, jc);
	if(_tcslen(jc) != 0)
		_stprintf(szHandle, _T("%s%s"), jc, name);
	else if(_tcscmp(name, _T("00")) == 0)
		_stprintf(szHandle, _T("%s"), jc);
	else
		_stprintf(szHandle, _T("%s%s"), jc, name);
	AcDbObjectId textid = DrawText(mid, szHandle, _T("MC"), _T(""), m_Scale * prop._size);
	CString handle = GetHandleByObject(textid);
	AddXdata(aname(id), _T("ZJHANDLE"), 0, handle);
	setcolor(aname(textid), prop._color); setlayer(aname(textid), prop._layer);
	AddEntToGroup(textid, group); SetTextStyle(textid, prop._font, 1.0);
	TCHAR szData[100] = {0};
	ReadXdata(aname(id), _T("�۵�"), 0, szData);
	if(0 == _tcsicmp(szData, _T("true")))
	{
		setcolor(aname(textid), 7);
	}
	else
	{
		setcolor(aname(textid), 4);
	}
}

void XMJGHouse::addFunctionProperty(const AcDbObjectId &id, const FunctionNameTableEx &fnt)
{
	AddXdata(aname(id), _T("����������"), 0, fnt.name);
	AddXdata(aname(id), _T("���������"), 0, fnt.cn);
	AddXdata(aname(id), _T("���������"), 0, fnt.jm);
	AddXdata(aname(id), _T("�������߶�"), 0, fnt.gaodu);
	AddXdata(aname(id), _T("���ϵ��"), 0, fnt.mjxs);
	AddXdata(aname(id), _T("����ϵ��"), 0, fnt.jrxs);
	AddXdata(aname(id), _T("�ݼ�����"), 0, fnt.tm);
	AddXdata(aname(id), _T("����߶�"), 0, fnt.m_isMainHeight ? _T("1") : _T("0"));
	if(fnt.m_island == 1)
	{
		AddXdata(aname(id), _T("�۵�"), 0, _T("true"));
	}
	else
	{
		AddXdata(aname(id), _T("�۵�"), 0, _T("false"));
	}
	IDataBaseOper oper;
	TextProperty gnq = oper.readTextPropery(_T("��������������"));
	setlayer(aname(id), gnq._layer); setcolor(aname(id), gnq._color);
}

bool XMJGHouse::setChangeAreasInfo(const AcDbObjectId &id)
{
	ads_point pt, pos; IDataBaseOper oper; CXRecordOperate xRecord;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���ע�㣺"), pt)) return false;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ���ע��λ�ã�"), pos)) return false;
	int order = 1; CDrawArrow arrow; AcDbObjectId group; int code = 0;
	xRecord.ReadIntegerRecord(_T("XMJG"), _T("BGXH"), code);
	TCHAR bhxh[255] = {0}; _stprintf(bhxh, _T("\n����������ţ�<%d>"), code + 1);
	int ret = ads_getint(bhxh, &order);
	if(ret == RTCAN) return false;
	else if(ret == RTNONE) order = code + 1;
	CString info; info.Format(_T("���%d"), order);
	AddXdata(aname(id), _T("������"), 0, info);
	TextProperty bgzj = oper.readTextPropery(_T("�����������ע������"), true, true);
	AcDbObjectId tid = DrawMText(pos, info, bgzj._font, bgzj._size * m_Scale, 0);
	setcolor(aname(tid), bgzj._color); setlayer(aname(tid), bgzj._layer);
	AcDbLeader *leader = new AcDbLeader;
	TextProperty pro = oper.readTextPropery(_T("�����������ע��"), true);
	leader->appendVertex(apoint(pt));
	leader->appendVertex(apoint(pos));
	leader->setDimasz(pro._size * m_Scale);
	leader->setLayer(pro._layer);
	leader->setColorIndex(pro._color);
	leader->setDimgap(0.09);
	leader->setDimtad(4);
	AcDbObjectId lid;
	if(!AppendEntity(leader, lid))
	{
		delete leader;
		return false;
	}
	leader->attachAnnotation(tid);
	leader->close();
	xRecord.AddIntegerRecord(_T("XMJG"), _T("BGXH"), order);
	return true;
}

void XMJGHouse::exportHouseZPT()
{
	XMJGExportZPT zpt; CBackScreen back;
	if(!zpt.setExportRange()) return;
	zpt.setTuQian(_T("XMJGTuQian"));
	IProjectMDB pdb;
	VMStr areas = pdb.getCXXTableInfo();
	double dsarea = 0.0, dxarea = 0.0; TCHAR pszArea[255] = {0};
	for(int idx = 0; idx < (int)areas.size(); ++idx)
	{
		MStr area = areas[idx];
		int lcount = _ttoi(area[_T("����")]);
		if(_ttoi(area[_T("��ֹ����")]) < 0 || _ttoi(area[_T("��ʼ����")]) < 0)
			dxarea += _tstof(area[_T("�������")]) * lcount;
		else
			dsarea += _tstof(area[_T("�������")]) * lcount;
	}
	MStr mjxx = pdb.getMJXXTableInfo();
	double jzydmj = _tstof(mjxx[_T("�����õ����")]);
	zpt.setJZMJArea(dsarea + dxarea, jzydmj, dsarea, dxarea);
	if(!zpt.drawMesh()) return;
	CString pathName = zpt.beginExport(_T("����������ƽͼ"));
	openDwgCommand(pathName);
}

void XMJGHouse::exportRoadZPT()
{
	XMJGExportZPT zpt;
	zpt.setUndoFlag();
	if(false == zpt.setExportRange()) return;
	zpt.setTuQian(_T("XMJGTuQian"));//��·ͼǩ
	IProjectMDB pdb;
	VMStr areas = pdb.getCXXTableInfo();
	double dsarea = 0.0, dxarea = 0.0; TCHAR pszArea[255] = {0};
	for(int idx = 0; idx < (int)areas.size(); ++idx)
	{
		MStr area = areas[idx];
		if(_ttoi(area[_T("���")]) < 0) dxarea += _tstof(area[_T("�������")]);
		else  dsarea += _tstof(area[_T("�������")]);
	}
	MStr mjxx = pdb.getMJXXTableInfo();
	double jzydmj = _tstof(mjxx[_T("�����õ����")]);
	zpt.setJZMJArea(dsarea + dxarea, jzydmj, dsarea, dxarea);
	zpt.drawMesh(_T("��·��ƽͼ����"));
	CString pathName = zpt.beginExport(_T("��·��ƽͼ"));
	zpt.beginUndo();
	openDwgCommand(pathName);
}


bool XMJGHouse::addFunctionAnnotion(const AcDbObjectIdArray &ids, CString &wbqinfo, double wbqmj, ads_point ptLoc)
{
	//ids���ֲ�ƽ�������й�����id����
	//wbqinfo������ֲ�ƽ���еĲ�ߺͼ���ϵ����Ϣ�����ϲ���"���:%sm������ϵ��1.00"�����²���"���:%sm������ϵ��0.00"
	//wbqmj���ֲ�ƽ���е����ǽ���
	//ptLoc��ads_point��
	CString info(_T("ע:\t")), strArea; double dzdarea = 0.0, sarea = 0.0;
	//info���ڼ�¼���������Ϣ
	TCHAR mj[2000] = {0}; TCHAR layer[200] = {0}; MStr Notetext;
	int len = ids.length();//len���ڼ�¼�ֲ�ƽ���й���������
	ads_point pos, ranglePoint; 
	_stprintf(layer, _T("%s,%s"), m_gnq._layer, m_cw._layer);//layer��¼������ͼ��ͳ�λͼ��
	if(RTCAN == ads_getpoint(NULL, _T("\n��ѡ��Ҫ��ע��λ�ã�"), pos)) return false;//pos��ʾע�ǵ�λ��
	//ranglePoint��ʾ���ֺ������߽��
	if (RTNORM != ads_getpoint(pos, _T("\n��ʰȡ���ֺ�����ʾ�Ҳ�߽�㣺"), ranglePoint))return false;
	double cwArea = 0.0;//��λ���
	MSSIds mssids = sortGNQByXData(ids);
	//����XData�Թ�������������
	/*
	������XDataʾ����
	���������ƣ�������ң���������ţ�01��
	��������ƣ����������߶ȣ�2.5�����ϵ����1��
	����ϵ��1���ݼ����ƣ�������߶ȣ�1��
	�۵���false��ZJHANDLE��337B
	*/
	bool wbqisAnn = false;
	for(MSSIdsIter sit = mssids.begin(); sit != mssids.end(); ++sit)//����¥�ݼ�
	{
		MSIds msids = sit->second; //�ݼ������й�����������Ϣ����
		CString tj = sit->first;//tj¥�ݼ�
		double area = 0.0;
		if(tj.GetLength() != 0)
		{
			info.Append(tj);//��λ����û�ж�Ӧ�ݼ�����
		}
		for(MSIdsIter it = msids.begin(); it != msids.end(); ++it)//�������ղ�ߺͼ���ϵ���ٷ���ķֲ�ƽ��
		{
			CString sm = it->first, dzdsm;//�ֲ�ƽ�水�ղ�ߺͼ���ϵ���ٷ��࣬sm�洢�ֲ�ƽ���ߺͼ���ϵ��
			AcDbObjectIdArray gnqs = it->second;//sm��Ӧ�����й�����ID����
			std::map<CString, double, CmpCstr> gnqmp;
			for(int idx = 0; idx < gnqs.length(); ++idx)//����������
			{
				AcDbObjectId id = gnqs.at(idx);//����ǰ������ָ��Ϊ���idxָ������
				AcDbObjectIdArray dzdid;//��ǰ�������еĵ��е�ID����
				AcGePoint2dArray nodes;//���浱ǰ���������нڵ�
				GetPlList(aname(id), nodes);//��õ�ǰ�����������нڵ�
				ssFromNodes(dzdid, nodes, 1, 0.5, 0, layer);//ѡ��ǰ�������а��������й������ͳ�λ
				dzdid.remove(id);//ȥ�����е�ID�����еĵ�ǰ������
				FunctionHAH hah;//���ڴ�ŵ�ǰ�������ı�š��������Ϣ
				getFunctionArea(id, hah);//��õ�ǰ�����������
				area = hah.m_jzmj;//area���ڴ�ŵ�ǰ�����������
				for(int indx = 0; indx < dzdid.length(); ++indx)//�������е�
				{
					FunctionHAH hah1;//���ڴ�ŵ�ǰ���ı�š��������Ϣ
					AcDbObjectId dzd = dzdid.at(indx);//ָ����ǰ��Ϊ���idxָ��ĵ��е��еĵ�
					getFunctionArea(dzd, hah1);//��õ�ǰ�������
					if(hah1.m_bh.CompareNoCase(_T("��λ")) == 0)
						cwArea += hah1.m_jzmj;//cwArea���ڴ������г�λ���
					//area -= hah1.m_jzmj;
				}
				if(fabs(area) < EPS) continue;//�����ǰ���������Ϊ�㣬����ȥ
				gnqmp.insert(std::make_pair(hah.m_bh, area));//��ǰ��������ź�������ֵ�Բ����gnqmp��
			}
			//�������湦���������map<CString, double, CmpCstr>
			for(std::map<CString, double, CmpCstr>::iterator itr = gnqmp.begin(); itr != gnqmp.end(); ++itr)
			{
				CString bh = itr->first;//���浱ǰ���������
				area = itr->second;//���浱ǰ���������
				_stprintf(mj, _T("%.3lf"), area);//��ʽ����ǰ���������������3λС����
				sarea += _tstof(mj);//����ʽ����ĵ�ǰ���������תΪ�����ȸ��������ӽ�sarea��
				dzdarea += _tstof(mj);//����ʽ����ĵ�ǰ���������תΪ�����ȸ��������ӽ�dzdarea��
				_stprintf(mj, _T("%s��"), bh, area);//��ʽ����ǰ���������������3λС����mj������"01 = 0.001"
				dzdsm += mj;
			}
			dzdsm = dzdsm.Mid(0, dzdsm.GetLength() - 1);//ȥ��β��Ӣ�Ķ���
			if(!wbqisAnn)
			{
				if(sm.CompareNoCase(wbqinfo) == 0 && fabs(wbqmj) > 0)
				{
					_stprintf(mj, _T("%.3lf"), wbqmj);
					dzdarea += _tstof(mj);
					sarea += _tstof(mj);
					wbqisAnn = true;
					_stprintf(mj, _T("WBQ = %.3lf��"), wbqmj);
					dzdsm += mj;
				}
			}
			if(sarea == 0.0) continue;
			strArea.Format(_T("����ϼ�%.3lfm{\\H0.7x;\\S2^;}��"), sarea);//sarea���ڴ洢ָ����ߺͼ���ϵ���µĹ����������ǽ���
			dzdsm += strArea + sm; sarea = 0.0;
			info += dzdsm;
			info += _T("\\P\t");
		}
	}
	if(!wbqisAnn && fabs(wbqmj) > 0)
	{
		_stprintf(mj, _T("WBQ = %.3lf��%s"), wbqmj, wbqinfo);
		dzdarea += wbqmj;//dzdarea���ڴ洢�ֲ�ƽ�������й����������ǽ�������
		info += mj;
		info += _T("\\P\t");
	}
	//{\\H0.7x;\\S2^;}��CAD�л���{\H0.7x;\S2^;}����ʾƽ���ϱ꣬���Խ�2��Ϊ3����ʾ�������Դ�����
	if(fabs(cwArea) > EPS)
		_stprintf(mj, _T("��λ����ϼ� = %.3lfm{\\H0.7x;\\S2^;}��������ϼ� = %.3lfm{\\H0.7x;\\S2^;}"), cwArea, dzdarea + cwArea);
	else _stprintf(mj, _T("������ϼ� = %.3lfm{\\H0.7x;\\S2^;}"), dzdarea);
	info += mj;
	AcDbObjectId textid = DrawMText(pos, info, _T(""), m_fcmjzs._size*m_Scale, 
		fabs(ranglePoint[X] - pos[X]), AcDbMText::kTopLeft);
	setcolor(aname(textid), m_fcmjzs._color);//����ʵ����ɫ
	setlayer(aname(textid), m_fcmjzs._layer);//����ʵ��ͼ��
	return true;
}

AcDbObjectIdArray XMJGHouse::getFunctionAreaIds(const AcDbObjectId &id)
{
	AcGePoint2dArray node;
	GetPlList(aname(id), node);
	AcDbObjectIdArray ids;
	TCHAR layer[200] = {0};
	_stprintf(layer, _T("%s"), m_gnq._layer);
	if(false == ssFromNodes(ids, node, 1, 1.0, 0, layer)) return ids;
	ids.remove(id);
	return ids;
}

void addDescribeInfo(const string &info, string &text)
{
	text.append("�O");
	if(info.length() == 0)
		return;
	text.append("(");
	text.append(info);
	text.append(")");
}

void XMJGHouse::DesignImplementation(SmartTable::Word &word)
{
	IProjectMDB pdb; MStr mjsm = pdb.getMJXXTableInfo();
	VMStr cxxs = pdb.getCXXTableInfo(); char text[4000]; string sm;
	string layerinfo; string buildinfo;	double tttt = 0.0;
	double jzzmj = 0.0, dsjzzmj = 0.0, dxjzzmj = 0.0, jrzmj = 0.0;


#pragma region ͳ�����

	for(int idx = 0; idx < (int)cxxs.size(); ++idx)
	{
		MStr cxx = cxxs[idx];
		int lct = _ttoi(cxx[_T("����")]);
		int sly = _ttoi(cxx[_T("��ʼ����")]);
		int ely = _ttoi(cxx[_T("��ֹ����")]);
		double jzmj = _tstof(cxx[_T("�������")]);
		jrzmj += _tstof(cxx[_T("�������")]) * lct;
		if(sly < 0 || ely < 0)
			dxjzzmj += jzmj * lct;
		else
			dsjzzmj += jzmj * lct;
	}
#pragma endregion ͳ�����

#pragma region ͳ�����
	// 	for(; it != info.end(); ++it)
	// 	{
	// 		jzzmj += it->second.m_jzzdmj;
	// 		jrzmj += it->second.m_jrmjxj;
	// 		MILIter lit = it->second.m_layers.begin();
	// 		for(; lit != it->second.m_layers.end(); ++lit)
	// 		{
	// 			CString lch = lit->second.m_lch;
	// 			if(lch.Mid(0, 1) == _T("-"))
	// 				dxjzzmj += lit->second.m_jzzmj;
	// 			else
	// 				dsjzzmj += lit->second.m_jzzmj;
	// 			if(lch.CompareNoCase(_T("0")) == 0 || lch.CompareNoCase(_T("999")) == 0)
	// 				TcharToChar(g_fieldchagetable.getNameByIndex(lch), text);
	// 			else TcharToChar(lch, text); layerinfo.append(text);
	// 			sprintf(text, "��%.3lfm \r\n", lit->second.m_lcg); layerinfo.append(text);
	// 		}
	// 		TcharToChar(it->first, text); buildinfo.append(text);
	// 		sprintf(text, "��%.3lfm \r\n", it->second.m_jzgd); buildinfo.append(text);
	// 	}
#pragma endregion ͳ�����

#pragma region �ܽ������
	buildinfo.clear(); splitBigString(mjsm[_T("ʵ���ܽ������˵��")], buildinfo);
	sprintf(text, "%.2lf", dxjzzmj + dsjzzmj); sm = text;
	addDescribeInfo(buildinfo, sm); double ghmj = 0.0;
	word.replaceText("#SCZJZMJ#", sm.c_str()); sm.clear();  buildinfo.clear();
	ghmj = _tstof(mjsm[_T("�滮�ܽ������")]);
	tttt = dxjzzmj + dsjzzmj - ghmj;
	if(tttt > 0) sprintf(text, "+%.2lf", tttt);
	else sprintf(text, "%.2lf", tttt);
	word.replaceText("#ZJZMJCZ#", text);
#pragma endregion �ܽ������

#pragma region �������

	IDataBaseOper oper;
	long standard = oper.readJRStartTime();
	MStr record = pdb.getJBXXTableInfo();
	long starttime = TimeConvertLong(record[_T("����ʱ��")]);
	bool isJR = starttime > standard;
	if(isJR)
	{
		splitBigString(mjsm[_T("ʵ��������˵��")], buildinfo);
		sprintf(text, "%.2lf", jrzmj); sm.append(text);
		addDescribeInfo(buildinfo, sm);
		word.replaceText("#SCZJRMJ#", sm.c_str()); sm.clear();  buildinfo.clear();
		ghmj = _tstof(mjsm[_T("�滮�������")]);
		tttt = jrzmj - ghmj;
		if(tttt < 0) sprintf(text, "%.2lf", tttt);
		else sprintf(text, "+%.2lf", tttt);
		word.replaceText("#JRMJCZ#", text);
	}
	else
	{
		sprintf(text, "%s", "/");
		word.replaceText("#SCZJRMJ#", text);
		word.replaceText("#JRMJCZ#", text);
	}
#pragma endregion �������

#pragma region �������
	splitBigString(mjsm[_T("ʵ��������˵��")], buildinfo);
	sprintf(text, "%.2lf", dsjzzmj); sm.append(text);
	addDescribeInfo(buildinfo, sm);
	word.replaceText("#SCDSZJZMJ#", sm.c_str()); sm.clear();  buildinfo.clear();
	ghmj = _tstof(mjsm[_T("�滮�������")]); sprintf(text, "%.3lf", dsjzzmj - ghmj);
	tttt = dsjzzmj - ghmj;
	if(tttt < 0) sprintf(text, "-%.2lf", tttt);
	else sprintf(text, "+%.2lf", tttt);
	word.replaceText("#DSJZMJCZ#", text);
#pragma endregion �������

#pragma region �������
	splitBigString(mjsm[_T("ʵ��������˵��")], buildinfo);
	sprintf(text, "%.2lf", dxjzzmj); sm.append(text);
	addDescribeInfo(buildinfo, sm);
	word.replaceText("#SCDXZJZMJ#", sm.c_str()); sm.clear();  buildinfo.clear();
	ghmj = _tstof(mjsm[_T("�滮�������")]);
	tttt = dxjzzmj - ghmj;
	if(tttt > 0) sprintf(text, "-%.2lf", tttt);
	else sprintf(text, "%.2lf", tttt);
	word.replaceText("#DXJZMJCZ#", text);
#pragma endregion �������
	TcharToChar(mjsm[_T("ʵ�⽨���õ����")], text); buildinfo = text;
	word.replaceText("#SCJZYDMJ#", buildinfo.c_str()); string sminfo;
	splitBigString(mjsm[_T("ʵ�⽨���õ����˵��")], sminfo);
	addDescribeInfo(sminfo, buildinfo);
	word.replaceText("#SCJZYDMJEX#", buildinfo.c_str());
	double ghydmj = _tstof(mjsm[_T("�����õ����")]);
	double scydmj = _tstof(mjsm[_T("ʵ�⽨���õ����")]);
	if(scydmj < 1.0)
		sprintf(text, "%s", "/");
	else if(ghydmj > scydmj)
		sprintf(text, "-%.2f", scydmj - ghydmj);
	else
		sprintf(text, "+%.2f", scydmj - ghydmj);
	word.replaceText("#YDMJCZ#", text);
}

void XMJGHouse::AreaReplaceInfo(SmartTable::Word &word)
{

}

void XMJGHouse::ExportDocInfo(SmartTable::Word &word)
{
	setJBXX(word);
	setJSSM(word);
	setMJXX(word);
	setBGXX(word);
	//�滮���߼��
}

void XMJGHouse::setJBXX(SmartTable::Word &word)
{
	IProjectMDB pdb; MStr jbxx = pdb.getJBXXTableInfo(); char info[255] = {0};
	CString strDate = jbxx[_T("����ʱ��")];
	TCHAR szDate[20] = {0};
	_tcscpy(szDate, strDate);
	if(strDate.GetLength() > 2)
	{
		int nIndex1 = strDate.Find(_T("/"), 0);
		int nIndex2 = strDate.Find(_T("/"), nIndex1 + 1);
		if(nIndex1 != nIndex2)
		{
			CString str;
			str.Format(_T("%s��%s��%s��"), strDate.Left(nIndex1), strDate.Mid(nIndex1 + 1, nIndex2 - nIndex1 - 1), strDate.Right(strDate.GetLength() - nIndex2 - 1));
			_tcscpy(szDate, str);
		}
	}

	TcharToChar(szDate, info); word.replaceText("#Date#", info);
	TcharToChar(jbxx[_T("�����")], info); word.replaceText("#AcceptCode#", info);
	TcharToChar(jbxx[_T("��Ŀ����")], info); word.replaceText("#ProjectName#", info);
	TcharToChar(jbxx[_T("��Ŀ����")], info); word.replaceText("#ProjectCode#", info);
	TcharToChar(jbxx[_T("���赥λ")], info); word.replaceText("#BuildCompany#", info);
	TcharToChar(jbxx[_T("��Ŀ�ص�")], info); word.replaceText("#ProjectAddress#", info);
	TcharToChar(jbxx[_T("������λ")], info); word.replaceText("#MeansureCompany#", info);
}

void XMJGHouse::setJSSM(SmartTable::Word &word)
{
	IProjectMDB pdb; MStr jssm = pdb.getJSSMTableInfo(); string newstring;
	splitBigString(jssm[_T("�������������Ĳ����������")], newstring);
	word.replaceText("#BCCLBGSDCLCSQK#", newstring.c_str()); newstring.clear();
	splitBigString(jssm[_T("��������")], newstring);
	word.replaceText("#MeasureMethod#", newstring.c_str()); newstring.clear();
	splitBigString(jssm[_T("�����������")], newstring);
	word.replaceText("#BuildAreaMeasure#", newstring.c_str()); newstring.clear();
	splitBigString(jssm[_T("���ɹ����ݵ��������")], newstring);
	word.replaceText("#LicenseDoc#", newstring.c_str()); newstring.clear();
	splitBigString(jssm[_T("������Ƶ�������")], newstring);
	word.replaceText("#ControlPoint#", newstring.c_str()); newstring.clear();
	splitBigString(jssm[_T("ͼ���������")], newstring);
	word.replaceText("#BasePoint#", newstring.c_str()); newstring.clear();
	splitBigString(jssm[_T("ϸ�������������")], newstring);
	word.replaceText("#DetailConvery#", newstring.c_str()); newstring.clear();
	CString title = m_fieldTitle[_T("#DetailConvery#")]; m_isReplace[title] = 1;
}

void XMJGHouse::setMJXX(SmartTable::Word &word)
{
	IProjectMDB pdb;  MStr mjxx = pdb.getMJXXTableInfo(); char info[4000] = {0}; string text;
	TcharToChar(mjxx[_T("�����õ����")], info); text = info; string oldstr;
	//splitBigString(mjxx[_T("�����õ����˵��")], oldstr);
	//addDescribeInfo(oldstr, text);
	word.replaceText("#GHJZYDMJ#", text.c_str()); oldstr.clear(); text.clear();
	CString title = m_fieldTitle[_T("#GHJZYDMJ#")]; m_isReplace[title] = 1;


	TcharToChar(mjxx[_T("�滮�ܽ������")], info); text = info;
	splitBigString(mjxx[_T("�滮�ܽ������˵��")], oldstr);
	addDescribeInfo(oldstr, text);
	word.replaceText("#GHZJZMJ#", text.c_str()); oldstr.clear(); text.clear();

	TcharToChar(mjxx[_T("�滮�������")], info); text = info;
	splitBigString(mjxx[_T("�滮�������˵��")], oldstr);
	addDescribeInfo(oldstr, text);
	word.replaceText("#GHDSJZMJ#", text.c_str()); oldstr.clear(); text.clear();

	TcharToChar(mjxx[_T("�滮�������")], info); text = info;
	splitBigString(mjxx[_T("�滮�������˵��")], oldstr);
	addDescribeInfo(oldstr, text);
	word.replaceText("#GHDXJZMJ#", text.c_str()); oldstr.clear(); text.clear();

	IDataBaseOper oper;
	long standard = oper.readJRStartTime();
	MStr record = pdb.getJBXXTableInfo();
	long starttime = TimeConvertLong(record[_T("����ʱ��")]);
	bool isJR = starttime > standard;
	if(isJR)
	{
		TcharToChar(mjxx[_T("�滮�������")], info); text = info;
		splitBigString(mjxx[_T("�滮�������˵��")], oldstr);
		addDescribeInfo(oldstr, text);
		word.replaceText("#GHJRMJ#", text.c_str()); oldstr.clear(); text.clear();
	}
	else
	{
		word.replaceText("#GHJRMJ#", "/");
	}

	splitBigString(mjxx[_T("���ϳ�λ"]), oldstr); word.replaceText("#GHDSCW#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("���³�λ"]), oldstr); word.replaceText("#GHDXCW#", oldstr.c_str()); oldstr.clear();

	TcharToChar(mjxx[_T("�滮��Ҫ�����λ��")], info); word.replaceText("#GHCRK#", info);
	TcharToChar(mjxx[_T("ʵ����Ҫ�����λ��")], info); word.replaceText("#SCCRK#", info);


}

void XMJGHouse::setBGXX(SmartTable::Word &word)
{
	IProjectMDB pdb;  MStr mjxx = pdb.getBGXXTableInfo(); string oldstr;
	splitBigString(mjxx[_T("�滮�˺��߼��"]), oldstr); word.replaceText("#GHTHXJJ#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("ʵ���˺��߼��"]), oldstr); word.replaceText("#SCTHXJJ#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("�滮�������"]), oldstr); word.replaceText("#GHJZJJ#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("ʵ�⽨�����"]), oldstr); word.replaceText("#SCJZJJ#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("�滮��������Ҫ��")], oldstr); word.replaceText("#GHJZLM#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("ʵ�⽨������Ҫ��")], oldstr); word.replaceText("#SCJZLM#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("�滮�����滮Ҫ��")], oldstr); word.replaceText("#GHQTGHYQ#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("ʵ�������滮Ҫ��")], oldstr); word.replaceText("#SCQTGHYQ#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("������Ҫ˵�������")], oldstr); word.replaceText("#QTXYSMQK#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("��滮��ɲ�һ�����")], oldstr); word.replaceText("#JGYSHBYZQK#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("�滮������������")], oldstr); word.replaceText("#GHGGJZPT#", oldstr.c_str()); oldstr.clear();
	splitBigString(mjxx[_T("ʵ�⹫����������")], oldstr); word.replaceText("#SCGGJZPT#", oldstr.c_str()); oldstr.clear();
	CString title = m_fieldTitle[_T("#SCGGJZPT#")]; m_isReplace[title] = 1;
	int val = _ttoi(mjxx[_T("��ʱ��ʩ�Ƿ���")]);
	if(val == 1)
	{
		word.replaceText("#1", "��");
		word.replaceText("#2", "  ");
	}
	else
	{
		word.replaceText("#1", "  ");
		word.replaceText("#2", "��");
	}
}

CString XMJGHouse::statisticLayerInfo(const VMStr &info)
{
	CString ret;
	return ret;
}

void XMJGHouse::writeArea2ProjectMDB(const AcDbObjectId &id)
{
	CString info, ldh; int lower = 0, upper = 0;
	TCHAR layer[255] = {0}; CString num; MStr filter;
	ReadXdata(aname(id), _T("¥����"), 0, layer);
	filter[_T("¥����")] = layer; IProjectMDB pdb;
	ldh = layer;
	ReadXdata(aname(id), _T("¥���"), 0, layer);
	info = layer; int pos = info.Find(_T(" ---> "));
	if(pos == -1)
		filter[_T("��ʼ����")] = info;
	else
	{
		filter[_T("��ʼ����")] = info.Left(pos);
		filter[_T("��ֹ����")] = info.Mid(pos + 6);
	}
	MStr record, record1, infos = pdb.getCXXTableInfo(filter);
	double jzmj = 0.0, jrmj = 0.0, zdmj = 0.0; int lcount = _ttoi(infos[_T("����")]);
	layerArea(id, jzmj, jrmj, zdmj); getCarCount(id, record1, jzmj, jrmj);
	infos = pdb.getMJXXTableInfo(); CString bl; bl.Format(_T("%s_%s"), ldh, layer);
	CString cw = margeCarCount(infos[_T("ʵ����ϳ�λ")], bl, _ttoi(record1[_T("ʵ����ϳ�λ")]));
	if(cw.GetLength() != 0) record1[_T("ʵ����ϳ�λ")] = cw;
	cw = margeCarCount(infos[_T("ʵ����³�λ")], bl, _ttoi(record1[_T("ʵ����³�λ")]));
	if(cw.GetLength() != 0) record1[_T("ʵ����³�λ")] = cw;
	pdb.setMJXXTableInfo(record1);
	for(int idx = lower; idx <= upper; ++idx)
	{
		record[_T("�������")].Format(_T("%.3lf"), jzmj);
		record[_T("�������")].Format(_T("%.3lf"), jrmj);
		record[_T("�����")].Format(_T("%.3lf"), zdmj);
		pdb.updataCXXTableInfo(filter, record);
	}
}

void XMJGHouse::drawDiPingXian(double &height, double width, double &swdpwz, AcDbObjectIdArray &del, const TCHAR *name)
{
	IProjectMDB pdb;
	double sndp = _tstof(pdb.getBGTable(name, _T("���ڵ���")));
	double swdp = _tstof(pdb.getBGTable(name, _T("�����ƺ")));
	double snwgc = sndp - swdp;			//������߲�
	swdpwz = height - snwgc;
	AcDbObjectId lineid; AcGePoint2dArray node;
	node.append(AcGePoint2d(-10.0, swdpwz)); node.append(AcGePoint2d(0.0, swdpwz));
	node.append(AcGePoint2d(0.0, 0.0)); node.append(AcGePoint2d(width, 0.0));
	node.append(AcGePoint2d(width, swdpwz)); node.append(AcGePoint2d(width + 10, swdpwz));
	AcDbObjectId dlx = DrawPolyline2D(node, 1.0, false, _T(""));  setlinetype(aname(dlx), _T("XM01"));
	TCHAR text1[200] = {0}, text2[200] = {0}; ads_point ptres1, ptres2; del.append(dlx);
	ads_point swdppos; swdppos[X] = width + 3; swdppos[Y] = swdpwz;
	AcDbObjectId id1 = DrawInsert(swdppos, _T("������"), m_Scale, m_Scale); del.append(id1);
	ads_polar(swdppos, PI / 2, 1.8 * m_Scale, ptres1);
	_stprintf(text1, _T("�����ƺ���%.2lfm"), swdp);
	AcDbObjectId textid1 = DrawText(ptres1, text1, _T("BL"), _T(""), m_swdpbg._size * m_Scale);  del.append(textid1);
	SetTextStyle(textid1, m_swdpbg._font, 1.0); setcolor(aname(textid1), m_swdpbg._color);
	setlayer(aname(textid1), m_swdpbg._layer);

	ads_point sndppos; sndppos[X] = width - 15; sndppos[Y] = height;
	AcDbObjectId id2 = DrawInsert(sndppos, _T("������"), m_Scale, m_Scale); del.append(id2);
	ads_polar(sndppos, PI / 2, 1.8 * m_Scale, ptres2);
	_stprintf(text2, _T("���ڵ�����%.2lfm"), sndp);
	AcDbObjectId textid2 = DrawText(ptres2, text2, _T("BL"), _T(""), m_sndpbg._size * m_Scale); del.append(textid2);
	SetTextStyle(textid2, m_sndpbg._font, 1.0); setcolor(aname(textid2), m_sndpbg._color);
	setlayer(aname(textid2), m_sndpbg._layer);
}

void XMJGHouse::filterObject(VStr &vals, const CString &name)
{
	IDataBaseOper oper; AcDbObjectIdArray tids;
	CString layer = oper.readCommonTable(name);
	AnalyzeData(vals, layer, _T(","));
}

void XMJGHouse::RoadfilterObj(VStr &vals)
{

	vals.push_back(_T("ʵ�����"));
	vals.push_back(_T("ʵ���ˮ"));
	vals.push_back(_T("ʵ��·��"));
	vals.push_back(_T("ʵ��ȼ��"));
	vals.push_back(_T("ʵ����ˮ"));
	vals.push_back(_T("ʵ�����ߵ���"));
	vals.push_back(_T("ʵ����ˮ"));
	vals.push_back(_T("���"));
	vals.push_back(_T("K��·����"));
	vals.push_back(_T("jsyd����ͼ��"));
	vals.push_back(_T("ʵ���"));
	vals.push_back(_T("��ע"));
	CStringArray retList;
	getLayerList(retList);
	for(int i = 0; i < (int)retList.GetSize(); ++i)
	{
		CString strTemp = _T("");
		strTemp = retList[i];
		if(!strTemp.Find(_T("P_SC_")) || !strTemp.Find(_T("L_SC_")))
			vals.push_back(strTemp);
	}
}

void XMJGHouse::PipefilterObj(VStr &vals, const CString &strPipeExport)
{
	vals.push_back(_T("���"));
	vals.push_back(_T("K��·����"));
	vals.push_back(_T("jsyd����ͼ��"));
	vals.push_back(_T("ʵ���"));
	vals.push_back(_T("��ע"));
	CStringArray retList;
	getLayerList(retList);
	for(int i = 0; i < (int)retList.GetSize(); ++i)
	{
		CString strTemp = _T("");
		strTemp = retList[i];
		if(strTemp.Find(strPipeExport) > 0)//��������strPipeExport�����ͼ��
			continue;
		if(!strTemp.Find(_T("P_SC_")) || !strTemp.Find(_T("L_SC_")))
			vals.push_back(strTemp);
	}
}

void XMJGHouse::setCarProperty(const AcDbObjectIdArray &ids)
{
	CChangeCarPro dlg;
	dlg.setObjects(ids);
	dlg.DoModal();
}

bool XMJGHouse::setFunctionalAreaInfo(AcDbObjectId &id, FunctionNameTableEx &fntex)
{
	if(id == AcDbObjectId::kNull) return false; TCHAR height[255] = {0};
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	SelectFilter sf1(8, m_fcpm._layer), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids; SelectEntitys(ids, sf1, sf2, _T("X"));
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectIdArray tmps = getInnerObjects(ids[idx]);
		if(!tmps.contains(id)) continue;
		ReadXdata(aname(id), _T("¥���"), 0, height);
	}
	XMJGFunctionsAreasInfo info;
	info.setEntInfo(id, height);
	if(IDOK != info.DoModal()) return false;
	AcDbObjectId group; CreateGroup(aname(id), group);
	TCHAR name[200] = {0}; ads_point mid;
	ReadXdata(aname(id), _T("����������"), 0, name);
	AcGePoint2dArray node; GetPlList(aname(id), node);
	GetNodesMiddlePoint(node, mid);
	mid[Y] -= 1 * m_Scale;
	AcDbObjectId textid = DrawText(mid, name, _T("MC"), _T(""), m_Scale * 2);
	mid[Y] += 2 * m_Scale; AddEntToGroup(textid, group);
	ReadXdata(aname(id), _T("���������"), 0, name);
	textid = DrawText(mid, name, _T("MC"), _T(""), m_Scale * 2);
	AddEntToGroup(textid, group);
	fntex = info.getFNTInfo();
	return true;
}

CString XMJGHouse::getSavePath(const TCHAR *path, const TCHAR *name)
{
	CString pathName(path);
	if(pathName.Right(1).CompareNoCase(_T("\\")) == 0 || pathName.Right(1).CompareNoCase(_T("/")) == 0)
		pathName.Format(_T("%s%s"), path, name);
	else
		pathName.Format(_T("%s\\%s"), path, name);
	return pathName;
}

void XMJGHouse::getHouseAreaAndHeight(AcDbObjectId &id, XMJGHAH &hah)
{
	AcGePoint2dArray node; GetPlList(aname(id), node);
	ads_name ssName; ssFromNodes(node, ssName, _T("CP"));
	long length = 0; ads_sslength(ssName, &length);
	TCHAR value[100] = {0};
	for(int idx = 0; idx < length; ++idx)
	{
		ads_name ent;
		ads_ssname(ssName, idx, ent);
		ReadXdata(ent, _T("�������"), 3, value);
		hah.m_jzmj += _tstof(value);
		ReadXdata(ent, _T("�������"), 3, value);
		hah.m_jrmj += _tstof(value);
	}
	ReadXdata(aname(id), _T("���"), 0, value);
	hah.m_cg = _tstof(value);
	ReadXdata(aname(id), _T("¥����"), 0, value);
	hah.m_ldh.Format(_T("%s"), value);
	ReadXdata(aname(id), _T("¥���"), 0, value);
	hah.m_lch.Format(_T("%s"), value);
	ads_ssfree(ssName);
}

void XMJGHouse::getPerLevelArea(AcDbObjectId &id, XMJGHAH &hah)
{
	AcGePoint2dArray node; GetPlList(aname(id), node);
	ads_name ssName; ssFromNodes(node, ssName, _T("CP"));
	long length = 0; ads_sslength(ssName, &length);
	TCHAR value[100] = {0};
	for(int idx = 0; idx < length; ++idx)
	{
		ads_name ent;
		ads_ssname(ssName, idx, ent);
		ReadXdata(ent, _T("�������"), 3, value);
		hah.m_jzmj += _tstof(value);
		ReadXdata(ent, _T("�������"), 3, value);
		hah.m_jrmj += _tstof(value);
	}
	ReadXdata(aname(id), _T("���"), 0, value);
	hah.m_cg = _tstof(value);
	ReadXdata(aname(id), _T("¥����"), 0, value);
	hah.m_ldh.Format(_T("%s"), value);
	ReadXdata(aname(id), _T("¥���"), 0, value);
	hah.m_lch.Format(_T("%s"), value);
}

void XMJGHouse::getSameLevelArea(XMJGHAH &level, MSHAH &hah)
{
	int start = -1, end = -1;
	getHouseLCH(level.m_lch, start, end);
	CString key; key.Format(_T("%s_"), level.m_ldh);
	CString layer;
	for(; start <= end; ++start)
	{
		layer.Format(_T("%d"), start);
		XMJGHAH temp;
		temp.setValue(level, start);
		key += temp.m_lch;
		hah[key] = temp;
	}
}

int XMJGHouse::getHouseLCH(const TCHAR *value, int &start, int &end)
{
	CString lch(value);
	int pos = lch.Find(_T(' '));
	if(pos == -1)
		start = end = _tstoi(lch);
	else
	{
		start = _tstoi(lch.Left(pos));
		pos = lch.ReverseFind(_T(' '));
		end = _tstoi(lch.Right(lch.GetLength() - pos));
	}
	return end - start + 1;
}

void XMJGHouse::getPlanInfo(AcDbObjectId &id, MStr &field, bool isJR)
{
	TCHAR value[255] = {0}; XMJGHAH hah;
	ReadXdata(aname(id), _T("¥����"), 0, value);
	field[_T("#BuildingNo#")] = value;
	ReadXdata(aname(id), _T("¥���"), 0, value);
	CString info = value; int count = 1;
	int sPos = info.Find(_T(" "));
	int ePos = info.ReverseFind(_T(' '));
	if(info.Mid(0, sPos).CompareNoCase(info.Mid(ePos + 1)) == 0)
		info = info.Mid(0, sPos);

	int start = 0, end = 0;
	if(info.Find(_T("--->")) != -1)
		info.Replace(_T("--->"), _T("-"));
	if(info.Find(_T(';')) != -1)
	{
		CStringArray temps;
		AnalyzeData(temps, info, _T(";"));
		count = temps.GetSize();
	}
	else
	{
		count = getHouseLCH(value, start, end);
	}
	if((start == end) && start < 0)
		field[_T("#LevelNum#")] = g_fieldchagetable.getNameByIndex(start);
	else if(start < 0 && end < 0)
		field[_T("#LevelNum#")] = g_fieldchagetable.getNameByIndex(start) +
		_T(" - ") + g_fieldchagetable.getNameByIndex(end);
	else if((start == end) && (start > 0) && (fabs(_tstof(value) - start - 0.5) < EPS))
		field[_T("#LevelNum#")].Format(_T("%d�в�"), start);
	else
		field[_T("#LevelNum#")] = info;
	LayerHAH funcarea; getLayerArea(id, funcarea);
	_stprintf(value, _T("%.3f"), funcarea.m_jzzmj);
	field[_T("#BuildArea#")] = value;
	_stprintf(value, _T("%.3f"), _tstof(value) * count);
	field[_T("#TotalBuildArea#")] = value;
	if(isJR)
	{
		_stprintf(value, _T("%.3f"), funcarea.m_jrzmj);
		field[_T("#CapacitArea#")] = value;
		_stprintf(value, _T("%.3f"), _tstof(value) * count);
		field[_T("#TotalCapacitArea#")] = value;
	}
	else
	{
		field[_T("#CapacitArea#")] = _T(" ");
		field[_T("#TotalCapacitArea#")] = _T(" ");
	}
}

void XMJGHouse::readArea2IniFile(MSBuild &info)
{
	CString filepath, line; TCHAR areastat[255] = {0};
	ads_findfile(_T("���ͳ��.ini"), areastat);
	filepath.Format(_T("%s"), areastat); bool flag = false;
	CStdioFile sf; BuildHAH build; LayerHAH layer;
	if(FALSE == sf.Open(filepath, CFile::modeReadWrite)) return;
	while(sf.ReadString(line))
	{
		//����	���	���	�������	�������	�������С��	�������С��//	����ռ�����
		int pos = line.Find(_T(":"));
		if(pos == -1) continue;
		CString name = line.Left(pos);
		line = line.Mid(pos + 2);
		pos = line.Find(_T(","));
		if(name.CompareNoCase(_T("Build")) == 0)
		{
			CString buildname = line.Left(pos);
			if(info.find(buildname) == info.end() && flag == true)
			{
				if(layer.m_lch.GetLength() != 0)
				{
					build.m_layers[layer.m_slayer] = layer;
					layer.m_function.clear();
				}
				info[build.m_build] = build;
			}
			flag = true;
			build.m_build = buildname;
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			build.m_jzgd = _tstof(line.Left(pos));
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			build.m_jzmjxj = _tstof(line.Left(pos));
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			build.m_jrmjxj = (float)_tstof(line.Left(pos));
			build.m_jzzdmj = (float)_tstof(line.Mid(pos + 1));
		}
		else if(name.CompareNoCase(_T("Layer")) == 0)
		{
			if(layer.m_lch.GetLength() != 0)
			{
				build.m_layers[layer.m_slayer] = layer;
				layer.m_function.clear();
			}
			layer.m_lch = line.Left(pos);
			getJGLayerNum(layer.m_lch, layer.m_slayer, layer.m_elayer);
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			layer.m_lcg = _tstof(line.Left(pos));
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			layer.m_jzzmj = _tstof(line.Left(pos));
			layer.m_jrzmj = (float)_tstof(line.Mid(pos + 1));
		}
		else if(name.CompareNoCase(_T("Function")) == 0)
		{
			FunctionHAH funct;
			funct.m_cg = (float)_tstof(line.Left(pos)); line = line.Mid(pos + 1);
			pos = line.Find(_T(",")); funct.m_jzmj = _tstof(line.Left(pos));
			funct.m_jrmj = (float)_tstof(line.Mid(pos + 1));
			layer.m_function[funct.m_cg] = funct;
		}
	}
	if(layer.m_lch.GetLength() != 0)
	{
		build.m_layers[layer.m_slayer] = layer;
		layer.m_function.clear();
	}
	if(info.find(build.m_build) == info.end() && flag == true)
		info[build.m_build] = build;
}

void XMJGHouse::writeArea2IniFile(const BuildHAH &build)
{

}

void XMJGHouse::writeArea2IniFile(const MSBuild &info)
{
	TCHAR configfile[255] = {0};
	if(RTNORM != ads_findfile(_T("���ͳ��.ini"), configfile))
	{
		AfxMessageBox(_T("���������ļ�[���ͳ��.ini]ʧ�ܣ�����"));
		return;
	}
	CStdioFile sf;
	CString line, buildinfo, layerinfo, funcinfo;
	CString filepath(configfile);
	if(FALSE == sf.Open(filepath, CFile::modeCreate | CFile::modeWrite)) return;
	MSBCIter it = info.begin();
	for(; it != info.end(); ++it)
	{
		CString bname = it->first;
		const BuildHAH hah = it->second;
		buildinfo.Format(_T("Build: %s,%.3lf,%.3f,%.3f,%.3f\n"), bname, hah.m_jzgd, hah.m_jzmjxj, hah.m_jrmjxj, hah.m_jzzdmj);
		sf.WriteString(buildinfo);
		MILCIter buildit = hah.m_layers.begin();
		for(; buildit != hah.m_layers.end(); ++buildit)
		{
			LayerHAH layer = buildit->second;
			layerinfo.Format(_T("Layer: %s,%.3lf,%.3lf,%.3lf\n"), layer.m_lch, layer.m_lcg, layer.m_jzzmj, layer.m_jrzmj);
			sf.WriteString(layerinfo);
			MFFIter funcit = layer.m_function.begin();
			for(; funcit != layer.m_function.end(); ++funcit)
			{
				FunctionHAH func = funcit->second;
				funcinfo.Format(_T("Function: %.2f,%.3f,%.3f\n"), func.m_cg, func.m_jzmj, func.m_jrmj);
				sf.WriteString(funcinfo);
			}
		}
	}
	sf.Flush();
	sf.Close();
}

void XMJGHouse::exportDoc(SmartTable::Word &word, const BuildHAH &hah)
{
	int count = hah.funccount;
	SmartTable::WordTable table;
	if(-1 == word.findTable(6, 2, "�����߶ȣ�#BUILDHEIGHT#", table))
	{
		acutPrintf(_T("\n �޷��ҵ���־λ!!!"));
		return;
	}
	int curidx = 8; char value[255] = {0};
	table.copyRow(curidx, 0, count - 1);
	table.merge(curidx, 1, curidx + count - 1, 1);
	table.merge(curidx, 6, curidx + count - 1, 6);
	table.merge(curidx, 7, curidx + count - 1, 7);
	table.merge(curidx, 8, curidx + count - 1, 8);
	TcharToChar(hah.m_build, value);
	table.setCellText(curidx, 1, value);
	sprintf(value, "%.3f", hah.m_jzmjxj);
	table.setCellText(curidx, 6, value);
	sprintf(value, "%.3f", hah.m_jrmjxj);
	table.setCellText(curidx, 7, value);
	sprintf(value, "%.3f", hah.m_jzzdmj);
	table.setCellText(curidx, 8, value);
	MILCIter layerit = hah.m_layers.begin();
	for(; layerit != hah.m_layers.end(); ++layerit)
	{
		CString layername = layerit->second.m_lch;
		LayerHAH layer = layerit->second;
		TcharToChar(layername, value);
		table.setCellText(curidx, 2, value);
		MFFIter it = layer.m_function.begin();
		int size = layer.funccount;
		int funcount = (int)layer.m_function.size();
		table.merge(curidx, 2, curidx + funcount - 1, 2);
		for(; it != layer.m_function.end(); ++it)
		{
			sprintf(value, "%.2f", it->first);
			table.setCellText(curidx, 3, value);
			sprintf(value, "%.3f", it->second.m_jzmj);
			table.setCellText(curidx, 4, value);
			sprintf(value, "%.3f", it->second.m_jrmj);
			table.setCellText(curidx, 5, value);
			curidx++;
		}
	}
}

void XMJGHouse::exportDoc(SmartTable::Word &word, const MSHAH &info)
{
	MSHAH::const_iterator it = info.begin();
	if(it == info.end()) return;
	SmartTable::WordTable table;
	if(-1 == word.findTable(6, 2, "�����߶ȣ�#BUILDHEIGHT#", table))
	{
		acutPrintf(_T("\n �޷��ҵ���־λ!!!"));
		return;
	}
	int count = (int)info.size(); int idx = 0;
	table.copyRow(8, 0, count - 1);
	table.merge(8, 1, 8 + count - 1, 1);
	table.merge(8, 6, 8 + count - 1, 6);
	table.merge(8, 7, 8 + count - 1, 7);
	char value[1000] = {0}; XMJGHAH temp = it->second;
	TcharToChar(temp.m_ldh, value); table.setCellText(8, 1, value);
	sprintf(value, "%.3f", temp.m_jzmjxj); table.setCellText(8, 6, value);
	sprintf(value, "%.3f", temp.m_jrmjxj); table.setCellText(8, 7, value);
	sprintf(value, "%.3f", temp.m_jzzdmj); table.setCellText(8, 8, value);


	for(; it != info.end(); ++it, ++idx)
	{
		XMJGHAH temp = it->second; char value[1000] = {0};
		TcharToChar(temp.m_lch, value); table.setCellText(8 + idx, 2, value);
		sprintf(value, "%.3f", temp.m_cg); table.setCellText(8 + idx, 3, value);
		sprintf(value, "%.3f", temp.m_jzmj); table.setCellText(8 + idx, 4, value);
		sprintf(value, "%.3f", temp.m_jrmj); table.setCellText(8 + idx, 5, value);
	}
}

void XMJGHouse::updataAreaIniFile(MSBuild &info)
{
	readArea2IniFile(info);
	writeArea2IniFile(info);
}

struct LayerInfo
{
	int lcount;
	CString ghcg;
	CString sccg;
	CString scbz;
	CString ghbz;
	CString slayer;
	CString elayer;
	double jzmj;
	double jrmj;

};

struct LayerCount
{
	int dxcs;
	int dscs;

	LayerCount()
		: dxcs(0), dscs(0)
	{

	}

};

typedef map<int, LayerInfo> LayerInfos;
typedef LayerInfos::iterator LIter;
typedef map<CString, LayerInfos> DLayerInfos;
typedef DLayerInfos::iterator DLIter;
typedef map<CString, LayerCount> BuildLayerCount;
typedef BuildLayerCount::iterator BLCountIter;

int getTotalCarCount(const CString &cwinfo)
{
	VStr cwinfos; int count = 0;
	AnalyzeData(cwinfos, cwinfo, _T(";"));
	for(int idx = 0; idx < (int)cwinfos.size(); ++idx)
	{
		CString info = cwinfos[idx];
		int pos = info.ReverseFind(_T('_'));
		count += _ttoi(info.Mid(pos + 1));
	}
	return count;
}

void XMJGHouse::replaceWordArea(SmartTable::Word &word)
{
	IProjectMDB pdb; MStr mjsm = pdb.getMJXXTableInfo();
	DLayerInfos dls; VMStr cxxs = pdb.getCXXTableInfo();
	int scdscw = 0, scdxcw = 0; bool isDsLayer = false; //�Ƿ��ǵ��ϲ�
	double jzzdmj = 0.0; int layercount = 0;
	double tttt = 0.0; BuildLayerCount lcinfo;
#pragma region ͳ����Ϣ

	for(int idx = 0; idx < (int)cxxs.size(); ++idx)
	{
		MStr cxx = cxxs.at(idx);
		CString dh = cxx[_T("¥����")];
		if(dls.find(dh) == dls.end())
			dls[dh] = LayerInfos();
		if(lcinfo.find(dh) == lcinfo.end())
			lcinfo[dh] = LayerCount();
		int layernum = _ttoi(cxx[_T("¥���")]);
		dls[dh][layernum].slayer = cxx[_T("��ʼ����")];
		dls[dh][layernum].elayer = cxx[_T("��ֹ����")];
		dls[dh][layernum].sccg = cxx[_T("ʵ����")];
		dls[dh][layernum].ghcg = cxx[_T("��Ʋ��")];
		dls[dh][layernum].ghbz = cxx[_T("��Ʊ�ע")];
		dls[dh][layernum].scbz = cxx[_T("ʵ�ⱸע")];
		dls[dh][layernum].lcount = _ttoi(cxx[_T("����")]);
		dls[dh][layernum].jzmj = _tstof(cxx[_T("�������")]);
		if(1 == _ttoi(cxx[_T("��ʼ����")]))
			jzzdmj += _tstof(cxx[_T("�����")]);
		dls[dh][layernum].jrmj = _tstof(cxx[_T("�������")]);
		CString cinfo = cxx[_T("��ʼ����")];
		if(fabs(_tstof(cinfo) - _ttoi(cinfo) - 0.5) < EPS) continue;
		//if(_tstof(cxx[_T("ʵ����")]) < EPS) continue;
		if(_ttoi(cxx[_T("�Ƿ���ʾ")]) == 0) continue;
		if(_ttoi(cinfo) > 0 && !isDsLayer) isDsLayer = true; //��Ҫ�Ǵ�������㣬תΪ���ֱ�Ϊ��
		if(isDsLayer) lcinfo[dh].dscs += _ttoi(cxx[_T("����")]);
		else  lcinfo[dh].dxcs += _ttoi(cxx[_T("����")]);
	}
	scdscw = getTotalCarCount(mjsm[_T("ʵ����ϳ�λ")]);
	scdxcw = getTotalCarCount(mjsm[_T("ʵ����³�λ")]);

#pragma endregion ͳ����Ϣ
	char temp[1024] = {0};
#pragma region ��λͳ��


	sprintf(temp, "%d", scdscw); word.replaceText("#SCDSCW#", temp);					//ʵ����ϳ�λ
	sprintf(temp, "%d", scdxcw); word.replaceText("#SCDXCW#", temp);					//ʵ����³�λ
	sprintf(temp, "%d", scdscw + scdxcw); word.replaceText("#SCCW#", temp);				//ʵ�⳵λ
	int ghdxcw = _ttoi(mjsm[_T("�滮���³�λ")]); int ghdscw = _ttoi(mjsm[_T("�滮���ϳ�λ")]);
	sprintf(temp, "%d", ghdscw); word.replaceText("#GHDSCW#", temp);		//�滮���ϳ�λ
	sprintf(temp, "%d", ghdxcw); word.replaceText("#GHDXCW#", temp);		//�滮���³�λ
	int cwcz = scdscw - ghdscw;
	if(cwcz > 0) sprintf(temp, "+%d", cwcz);
	else sprintf(temp, "%d", cwcz);
	word.replaceText("#DSCWCZ#", temp);
	cwcz = scdxcw - ghdxcw;
	if(cwcz > 0) sprintf(temp, "+%d", cwcz);
	else sprintf(temp, "%d", cwcz);
	word.replaceText("#DXCWCZ#", temp);

#pragma endregion ��λͳ��

	string sclayerinfo, ghlayerinfo, sclayer;				//����Ϣ
	string scdinfo, scdinfosm, ghdinfo, gdcz;				//�����߶���Ϣ
	char tlinfo[255] = {0};

	DLIter dlit = dls.begin();
	double scdg = 0.0, ghdg = 0.0, dxjzmj = 0.0;
	double zjrmj = 0.0, zjzmj = 0.0, dsjzmj = 0.0;
	SmartTable::WordTable table; int row = 15;
	int ret = word.findTable(row, 2, "#GHZYCG#", table);
	double sccg = 0, ghcg = 0; ghlayerinfo = "";
	for(; dlit != dls.end(); ++dlit)
	{
		LayerInfos ls = dlit->second;
		CString dh = dlit->first;
		table.copyRow(row, 0, ls.size());
		LIter lit = ls.begin(); int layercount = 0;
		for(; lit != ls.end(); ++lit, row++)
		{
			LayerInfo linfo = lit->second;
#pragma region ����Ϣ

			if(lit == ls.begin())
			{
				TcharToChar(dh, tlinfo);
				sclayerinfo.append(tlinfo); sclayerinfo.append("�� \n");
				ghlayerinfo.append(tlinfo); ghlayerinfo.append("�� \n");
			}
			if(fabs(_tstof(linfo.slayer) - _ttoi(linfo.slayer) - 0.5) < EPS)
			{
				sprintf(temp, "%d ��", _ttoi(linfo.slayer));
				sclayerinfo.append(temp);
				ghlayerinfo.append(temp);
			}
			else
			{
				//acutPrintf(linfo.slayer);
				TcharToChar(linfo.slayer, tlinfo);
				sclayerinfo.append(tlinfo);
				ghlayerinfo.append(tlinfo);
			}
			if(linfo.elayer.GetLength() != 0)
			{
				//acutPrintf(linfo.elayer);
				ghlayerinfo.append("��");
				sclayerinfo.append("��");
				TcharToChar(linfo.elayer, tlinfo);
				sclayerinfo.append(tlinfo);
				ghlayerinfo.append(tlinfo);
			}
			if(_ttoi(linfo.slayer) != 0)
			{
				ghlayerinfo.append("��");
				sclayerinfo.append("��");
			}

#pragma endregion ����Ϣ

#pragma region �滮���

			if(linfo.ghcg.GetLength() != 0)
			{
				//acutPrintf(_T("��%sm"), linfo.ghcg);
				sprintf(temp, "��%.2lfm", _tstof(linfo.ghcg));
				ghlayerinfo.append(temp);
				if(linfo.ghbz.GetLength() != 0)
				{
					ghlayerinfo.append("(");
					TcharToChar(linfo.ghbz, tlinfo);
					ghlayerinfo.append(tlinfo);
					ghlayerinfo.append(")");
				}
				table.setCellText(row, 2, ghlayerinfo.c_str());
			}
			else
				table.setCellText(row, 2, "�޹滮���");
			ghlayerinfo.clear();
#pragma endregion �滮���

#pragma region ʵ����

			if(linfo.sccg.GetLength() != 0)
			{
				//acutPrintf(_T("��%sm"), linfo.sccg);
				sprintf(temp, "��%.2lfm", _tstof(linfo.sccg));
				sclayerinfo.append(temp);
				if(linfo.scbz.GetLength() != 0)
				{
					sclayerinfo.append("(");
					TcharToChar(linfo.scbz, tlinfo);
					sclayerinfo.append(tlinfo);
					sclayerinfo.append(")");
				}
				sprintf(temp, "%s", sclayerinfo);
				table.setCellText(row, 3, temp);
			}
			else
				table.setCellText(row, 3, "��ʵ����");
			sclayer.append("\n"); sclayer += sclayerinfo;
			sclayerinfo.clear();

#pragma endregion ʵ����

#pragma region �߶Ȳ�ֵ

			if(linfo.ghcg.GetLength() != 0)
			{
				double cgcz = _tstof(linfo.sccg) - _tstof(linfo.ghcg);
				sprintf(temp, (cgcz > 0) ? "+%.2lfm" : "%.2lfm", cgcz);
				table.setCellText(row, 4, temp);
			}

#pragma endregion �߶Ȳ�ֵ

#pragma region �������

			zjzmj += linfo.jzmj * linfo.lcount;
			zjrmj += linfo.jrmj * linfo.lcount;
			if(_ttoi(linfo.slayer) < 0)
				dxjzmj += linfo.jzmj * linfo.lcount;
			else
			{
				int slayer = _tstof(linfo.slayer);
				if(fabs(_tstof(linfo.slayer) - slayer - 0.5) > EPS)
					layercount += linfo.lcount;
				dsjzmj += linfo.jzmj * linfo.lcount;
			}

			if(fabs(_tstof(linfo.slayer) - _ttoi(linfo.slayer) - 0.5) < EPS) continue;	//�в㲻����߶�
			scdg += _tstof(linfo.sccg) * linfo.lcount;
			ghdg += _tstof(linfo.ghcg) * linfo.lcount;

#pragma endregion �������

		}

#pragma region �����߶�

		TcharToChar(dlit->first, temp); scdinfo.append("\n"); ghdinfo.append("\n"); scdinfosm.append("\n");
		scdinfo.append(temp); ghdinfo.append(temp); scdinfosm.append(temp);
		CString strSWDP = pdb.getBGTable(dlit->first, _T("�����ƺ"));
		CString strJZBG = pdb.getBGTable(dlit->first, _T("�������"));
		scdg = _tstof(strJZBG) - _tstof(strSWDP);
		double gaodu = _tstof(strSWDP) + scdg;
		CString bgwz = pdb.getBGTable(dh, _T("���λ��"));
		if(bgwz.CompareNoCase(_T("����")) == 0)
			bgwz.Format(_T("%d"), layercount);
		CString strLCGD; strLCGD.Format(_T("%.2lfm���������ƺ���%sm������%s%.2lfmֹ��"), scdg, strSWDP, bgwz, gaodu);
		TcharToChar(strLCGD, temp);  scdinfosm.append(temp);
		sprintf(temp, ": %.2lfm", scdg); scdinfo.append(temp);
		MStr filter; filter[_T("¥����")] = dh;
		MStr record = pdb.getDXXTableInfo(filter);
		ghdg = _tstof(record[_T("�滮�߶�")]);
		sprintf(temp, "��%.2lfm", ghdg); ghdinfo.append(temp);
		sprintf(temp, "\n%.2lfm", scdg - ghdg); gdcz.append(temp);

#pragma endregion �����߶�

	}

	table.removeRow(row);

#pragma region ��������

	string ttt;
	BLCountIter msit = lcinfo.begin();
	for(; msit != lcinfo.end(); ++msit)
	{
		TcharToChar(msit->first, temp);
		LayerCount lc = msit->second; ttt.append(temp);
		sprintf(temp, "�� \n����%d�㣻\n����%d�� \n", lc.dxcs, lc.dscs);
		ttt.append(temp);
	}
	word.replaceText("#SCDSDXCS#", ttt.c_str());
	word.replaceText("#GHDSDXCS#", ttt.c_str());

#pragma endregion ��������

#pragma region �����߶�

	word.replaceText("#GHJZGD#", ghdinfo.c_str());							//�滮�����߶�

	word.replaceText("#SCJZGD#", scdinfo.c_str());							//�滮�����߶�
	word.replaceText("#SCJZGDSM#", scdinfosm.c_str());

	double jsydmj = _tstof(mjsm[_T("ʵ�⽨���õ����")]);
	sprintf(temp, "%.2lf", jzzdmj); ttt = temp;
	//addDescribeInfo("", ttt);
	word.replaceText("#JZZDMJ#", ttt.c_str());								//����ռ�����
	word.replaceText("#JZGDJCSJC#", gdcz.c_str());

	word.replaceText("#SCZYCG1#", sclayer.c_str());

#pragma endregion �����߶�

#pragma region �����ܶ�

	double jzmd = -1.0;

	if(fabs(jsydmj) < EPS)
	{
		word.replaceText("#SCJZMD#", "/");
	}
	else
	{
		jzmd = jzzdmj / jsydmj * 100;
		sprintf(temp, "%.2lf%", jzmd);
		word.replaceText("#SCJZMD#", temp);		//ʵ�⽨���ܶ�
	}
	double ghmd = _tstof(mjsm[_T("�����ܶ�")]);
	if(fabs(ghmd) < EPS)
	{
		word.replaceText("#GHJZMD#", "/");
	}
	else
	{
		sprintf(temp, "%.2lf%", ghmd);
		word.replaceText("#GHJZMD#", temp);
	}
	tttt = jzmd - ghmd;
	if(jzmd < 0) sprintf(temp, "%s", "");
	else if(tttt > 0) sprintf(temp, "+%.2lf", tttt);
	else sprintf(temp, "%.2lf", tttt);
	word.replaceText("#JZMDCZ#", temp);	//�����ܶȲ�ֵ

#pragma endregion �����ܶ�

	return;
}

BuildLHeightAreaInfo XMJGHouse::initAreaHeightInfo()
{
	VMStr cxxs; BuildLHeightAreaInfo blinfo;
	{
		IProjectMDB pdb;
		cxxs = pdb.getCXXTableInfo();
	}
	map<CString, MStr, CmpCstr> cxxinfo;
	for(int idx = 0; idx < (int)cxxs.size(); ++idx)
	{
		MStr cxx = cxxs[idx];
		CString ldh = cxx[_T("¥����")];
		CString lch = cxx[_T("¥���")];
		cxxinfo[ldh + _T("_") + lch] = cxx;
	}
	map<CString, MStr, CmpCstr>::iterator it = cxxinfo.begin();
	typedef map<CString, LHeightAreaInfos, CmpCstr> MSLHAI;
	typedef map<CString, MSLHAI> MSBLInfo; MSBLInfo msblinfo;
	for(; it != cxxinfo.end(); ++it)
	{
		LayerHeightAreaInfo info;
		MStr cxx = it->second; CString ldh = cxx[_T("¥����")];
		CString lname = cxx[_T("��ʼ����")];
		if(cxx[_T("��ֹ����")].GetLength())
			lname += _T(" ---> ") + cxx[_T("��ֹ����")];
		initFunctionAreaHeightInfo(ldh, lname, info._functions);
		info._jzjdmj = _tstof(cxx[_T("�����")]);
		info._jrarea = _tstof(cxx[_T("�������")]);
		info._jzarea = _tstof(cxx[_T("�������")]);
		info.slayer = cxx[_T("��ʼ����")];
		info.elayer = cxx[_T("��ֹ����")];
		info._layerinfo = cxx[_T("�㱸ע")];
		if(info.slayer.FindOneOf(_T(";")) != -1)
		{
			CStringArray strs;
			AnalyzeData(strs, info.slayer, _T(";"));
			info._lcount = strs.GetSize();
		}
		else
			info._lcount = _ttoi(cxx[_T("����")]);
		info._row = info._functions.size();
		if(msblinfo.find(ldh) == msblinfo.end())
			msblinfo[ldh] = MSLHAI();
		if(info.slayer.CompareNoCase(_T("����²�")) == 0)
		{
			info.lname = info.slayer;
			info.slayer.Format(_T("%d"), 0);
		}
		if(msblinfo[ldh].find(info.slayer) == msblinfo[ldh].end())
			msblinfo[ldh][info.slayer] = LHeightAreaInfos();
		msblinfo[ldh][info.slayer].push_back(info);
	}
	MSBLInfo::iterator msblInfoIt = msblinfo.begin();
	for(; msblInfoIt != msblinfo.end(); ++msblInfoIt)
	{
		MSLHAI mslhai = msblInfoIt->second;
		CString ldh = msblInfoIt->first;
		MSLHAI::iterator mslhaiIt = mslhai.begin();
		blinfo[ldh] = LHeightAreaInfos();
		for(; mslhaiIt != mslhai.end(); ++mslhaiIt)
		{
			LHeightAreaInfos infos = mslhaiIt->second;
			for(int idx = 0; idx < infos.size(); ++idx)
				blinfo[ldh].push_back(infos[idx]);
		}
	}
	return blinfo;
}

void XMJGHouse::initFunctionAreaHeightInfo(const CString &ldh, const CString &lname, HeightFunction &info)
{
	IProjectMDB pdb; MStr filter;
	filter[_T("����")] = ldh;
	filter[_T("���")] = lname;
	VMStr records = pdb.getGNQXXInfo(filter);
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		FunctionHeightArea area;
		area._height = _tstof(record[_T("�߶�")]);
		area._jrarea = _tstof(record[_T("�������")]);
		area._jzarea = _tstof(record[_T("�������")]);
		if(info.find(area._height) == info.end())
			info[area._height] = area;
		else
		{
			info[area._height]._jrarea += area._jrarea;
			info[area._height]._jzarea += area._jzarea;
		}
	}
}

void XMJGHouse::DrawUpPlanning(SmartTable::Word &word)
{
	IProjectMDB pdb; string info;
	MStr record = pdb.getJSSMTableInfo();
	splitBigString(record[_T("����滮��Ϣ")], info);
	if(info.length() != 0)
	{
		word.replaceText("#TQGHXX#", info.c_str());
		CString title = m_fieldTitle[_T("#TQGHXX#")];
		m_isReplace[title] = 1;
	}
	else
	{
		word.replaceText("#TQGHXX#", "");
	}
}

bool compareInt(const int &val1, const int &val2)
{
	return val1 > val2;
}

void XMJGHouse::replaceWordTitle(SmartTable::Word &word)
{
	TCHAR mdbPath[255] = {0};
	ads_findfile(_T("xmjg.mdb"), mdbPath);
	CDatabaseOperManager dbManager; MStr tokenBH;
	dbManager.initialConnect(mdbPath); VMStr records, tokens;
	dbManager.ReadDataBase_all(_T("JGReportTitle"), records);
	dbManager.ReadDataBase_all(_T("JGToken"), tokens);
	for(int idx = 0; idx < tokens.size(); ++idx)
	{
		MStr record = tokens[idx];
		CString title = record[_T("����")];
		CString xh = record[_T("���")];
		tokenBH[title] = xh;
	}
	VInt offsets;
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx];
		CString field = record[_T("�ֶ���")];
		CString token = record[_T("���ֵ")];
		CString ttoken = token;
		if(token.IsEmpty() || field.IsEmpty()) continue;
		char wField[255] = {0}, wToken[255] = {0}, tToken[255] = {0};
		ttoken.Replace(_T("Title"), _T("Paging"));
		TcharToChar(token, wToken);
		TcharToChar(ttoken, tToken);
		TcharToChar(field, wField);
		CString xh = tokenBH[token];
		CStringArray xhs; AnalyzeData(xhs, xh, _T(";"));
		if(m_isReplace.find(token) == m_isReplace.end()
			|| m_isReplace[token] == 0)
		{
			word.removeParagraph(wToken);
			for(int indx = 0; indx < xhs.GetSize(); ++indx)
			{
				xh = xhs.GetAt(indx);
				int val = _tstoi(xh);
				if(val) offsets.push_back(val);
			}
		}
		else
		{
			word.replaceText(wToken, wField);
			/*if(word.setCurrentInsert(tToken))
				word.newPage();*/
		}
		word.replaceText(tToken, "");
	}
	std::sort(offsets.begin(), offsets.end(), compareInt);
	for(int idx = 0; idx < offsets.size(); ++idx)
		word.removesectPr(offsets[idx]);
}

void XMJGHouse::replaceWordHeight(SmartTable::Word &word)
{
	BuildLHeightAreaInfo info = initAreaHeightInfo();
	BuildLHeightAreaInfo::iterator it = info.begin();
	int btotal = 0; SmartTable::WordTable table;
	int rowidx = 8; char szInfo[255] = {0}; bool isJR;
	if(-1 == word.findTable(rowidx, 1, "#layer#", table))
	{
		acutPrintf(_T("\n �޷��ҵ���־λ!!!"));
		return;
	}
	else
	{
		word.replaceText("#layer#", "");
		CString title = m_fieldTitle[_T("#layer#")];
		m_isReplace[title] = 1;
	}
	{IDataBaseOper oper; IProjectMDB pdb;
	long standard = oper.readJRStartTime();
	MStr record = pdb.getJBXXTableInfo();
	long starttime = TimeConvertLong(record[_T("����ʱ��")]);
	isJR = starttime > standard; }
	double zjzmj = 0.0, zjrmj = 0.0, zjzzdmj = 0.0;
	double jzmjxj = 0.0, jrmjxj = 0.0, jzzdmj = 0.0;
	for(; it != info.end(); ++it)
	{
		CString ldh = it->first; int sl = 0, dsl = 0;
		LHeightAreaInfos linfos = it->second; dsl = rowidx;
		int dxqsh = -999, dsqsh = -1;
		for(int idx = 0; idx < linfos.size(); ++idx)
		{
			LayerHeightAreaInfo linfo = linfos[idx];
			int lh = _ttoi(linfo.slayer), lrow = rowidx;
			if(lh < 0 && dxqsh == -999) dxqsh = rowidx;
			else if(lh > -1 && dsqsh == -1) dsqsh = rowidx;
			table.copyRow(rowidx, 0, (linfo._row != 0) ? linfo._row : 1);
			HeightFunction funs = linfo._functions;
			HeightFunction::iterator it = funs.begin();
			for(; it != funs.end(); ++it)
			{
				sprintf(szInfo, "%.2lf", it->first);
				table.setCellText(rowidx, 3, szInfo);
				sprintf(szInfo, "%.3lf", it->second._jzarea * linfo._lcount);
				jzmjxj += it->second._jzarea * linfo._lcount;
				zjzmj += it->second._jzarea * linfo._lcount;
				table.setCellText(rowidx, 4, szInfo);
				sprintf(szInfo, "%.3lf", it->second._jrarea * linfo._lcount);
				jrmjxj += it->second._jrarea * linfo._lcount;
				zjrmj += it->second._jrarea * linfo._lcount;
				if(isJR)
					table.setCellText(rowidx, 5, szInfo);
				else
					table.setCellText(rowidx, 5, "/");
				TcharToChar(linfo._layerinfo, szInfo);
				table.setCellText(rowidx, 9, szInfo);
				rowidx++;
				if(lh == 1) jzzdmj = linfo._jzjdmj;
			}
			if(lh == -1 && dxqsh != -999)
			{
				int dxzzh = (dsqsh == -1) ? rowidx - 1 : dsqsh - 1;
				table.merge(dxqsh, 6, dxzzh, 6);
				table.merge(dxqsh, 7, dxzzh, 7);
				sprintf(szInfo, "%.3lf", jzmjxj);
				table.setCellText(dxqsh, 6, szInfo);
				if(isJR)
				{
					sprintf(szInfo, "%.3lf", jrmjxj);
					table.setCellText(dxqsh, 7, szInfo);
				}
				else
					table.setCellText(dxqsh, 7, "/");
				sl = 0; jzmjxj = jrmjxj = 0.0;
			}
			if(funs.size() > 1)
				table.merge(lrow, 2, rowidx - 1, 2);
			else lrow = rowidx - 1;
			if(fabs(_tstof(linfo.slayer) - lh - 0.5) < EPS)
				sprintf(szInfo, "%d�в�", lh);
			else if(linfo._lcount == 1)
				TcharToChar(linfo.slayer, szInfo);
			else if(linfo.slayer.FindOneOf(_T(";")) != -1)
				TcharToChar(linfo.slayer, szInfo);
			else
				sprintf(szInfo, "%s-%s", linfo.slayer, linfo.elayer);
			table.setCellText(lrow, 2, szInfo);
		}
		if(dsqsh != -1)
		{
			table.merge(dsqsh, 6, rowidx - 1, 6);
			table.merge(dsqsh, 7, rowidx - 1, 7);
			sprintf(szInfo, "%.3lf", jzmjxj);
			table.setCellText(dsqsh, 6, szInfo);
			if(isJR)
			{
				sprintf(szInfo, "%.3lf", jrmjxj);
				table.setCellText(dsqsh, 7, szInfo);
			}
			else
				table.setCellText(dsqsh, 7, "/");
			table.merge(dsl, 8, rowidx - 1, 8); zjzzdmj += jzzdmj;
			sprintf(szInfo, "%.3lf", jzzdmj); table.setCellText(dsl, 8, szInfo);
			sl = 0; jzzdmj = jzmjxj = jrmjxj = 0.0;
		}
		dxqsh = (dxqsh == -999) ? dsqsh : dxqsh;
		table.merge(dxqsh, 1, rowidx - 1, 1);
		TcharToChar(ldh, szInfo);
		table.setCellText(dxqsh, 1, szInfo);
	}
	table.setCellText(rowidx, 1, "�ܼ�");
	sprintf(szInfo, "%.3lf", zjzmj);
	table.setCellText(rowidx, 4, szInfo);
	table.setCellText(rowidx, 6, szInfo);
	if(isJR)
	{
		sprintf(szInfo, "%.3lf", zjrmj);
		table.setCellText(rowidx, 5, szInfo);
		table.setCellText(rowidx, 7, szInfo);
	}
	else
	{
		table.setCellText(rowidx, 5, "/");
		table.setCellText(rowidx, 7, "/");
	}
	sprintf(szInfo, "%.3lf", zjzzdmj); table.setCellText(rowidx, 8, szInfo);
}

char* CStringToCharArray(const CString &str)
{
	char *ptr;
#ifdef _UNICODE
	LONG len;
	len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	ptr = new char[len + 1];
	memset(ptr, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, str, -1, ptr, len + 1, NULL, NULL);
#else
	ptr = new char[str.GetAllocLength() + 1];
#endif
	return ptr;
}

void XMJGHouse::setProjectInfo(SmartTable::Word &word)
{
	setJBXX(word);
	setJSSM(word);
	setMJXX(word);
	setBGXX(word);


	MStr field; char info[4000] = {0};
	getReplaceValue(field);	string newstring;
	splitBigString(field[_T("#BASEPOINT#")], newstring);
	word.replaceText("#BasePoint#", newstring.c_str());
	TcharToChar(field[_T("#ACCEPTCODE#")], info); word.replaceText("#AcceptCode#", info);
	TcharToChar(field[_T("#ProjectName#")], info); word.replaceText("#ProjectName#", info);
	splitBigString(field[_T("#CONTROLPOINT#")], newstring);
	word.replaceText("#CONTROLPOINT#", newstring.c_str()); newstring.clear();
	TcharToChar(field[_T("#CONTROLPOINT#")], info); word.replaceText("#ControlPoint#", info);
	TcharToChar(field[_T("#BuildCompany#")], info); word.replaceText("#BuildCompany#", info);
	splitBigString(field[_T("#DetailConvery#")], newstring);
	word.replaceText("#DetailConvery#", newstring.c_str()); newstring.clear();
	TcharToChar(field[_T("#ProjectAddress#")], info); word.replaceText("#ProjectAddress#", info);
	TcharToChar(field[_T("#MeansureCompany#")], info); word.replaceText("#MeansureCompany#", info);
	splitBigString(field[_T("#DetailConvery#")], newstring);
	word.replaceText("#DetailConvery#", newstring.c_str()); newstring.clear();
	splitBigString(field[_T("#GHGGJZPT#")], newstring);
	word.replaceText("#GHGGJZPT#", newstring.c_str()); newstring.clear();
	splitBigString(field[_T("#SCGGJZPT#")], newstring);
	word.replaceText("#SCGGJZPT#", newstring.c_str()); newstring.clear();
	splitBigString(field[_T("#LicenseDoc#")], newstring);
	word.replaceText("#LicenseDoc#", newstring.c_str()); newstring.clear();
	CString title = m_fieldTitle[_T("#LicenseDoc#")]; m_isReplace[title] = 1;
}

void XMJGHouse::setLicenseDoc(SmartTable::Word &word)
{
	CAcModuleResourceOverride myResources;//�����ö������������Դ��ͻ����
	XMJGLicenseDocDlg dlg;
	dlg.DoModal();
	MStr license;
	dlg.getLicenseDoc(license);
	MSIter it = license.begin();
	TCHAR ld[2000]; CString info, temp; int idx = 1;
	for(; it != license.end(); ++it)
	{
		temp.Format(_T("  ��%d)��%s��%s��\n"), idx++, it->first, it->second);
		info += temp;
	}
	_stprintf(ld, _T("%s"), info); char text[4000];
	TcharToChar(ld, text);
	word.replaceText("#LicenseDoc#", text);
}

void XMJGHouse::insertHousePicture(SmartTable::Word &word)
{
	SmartTable::WordTable table; int tableindex = -1;
	tableindex = word.findTable(1, 1, "#Format1#", table);
	if(tableindex == -1) return;
	word.setPropertyTable(tableindex);
	readHousePictureInfo();
	XMJGImageSize imagesize; double x, y;
	MIHPIter it = m_PicInfo.begin();
	TCHAR path[255] = {0}; char info[255] = {0};
	for(; it != m_PicInfo.end(); ++it)
	{
		HousePicture hp = it->second;
		TcharToChar(hp.m_pos, info);
		word.setCurrentInsert(info, 0, false);
		_stprintf(path, _T("%s\\%s"), m_SaveDir, hp.m_name);
		TcharToChar(path, info);
		imagesize.loadImage(info);
		imagesize.getExportSize(x, y);
		word.appendImage(info, x, y, Alignment_MC);
		TcharToChar(hp.m_desc, info);
		word.appendParagraph(info, 0);
	}
	word.removeTable(tableindex);
}

void XMJGHouse::readHousePictureInfo()
{
	TCHAR path[255] = {0}; CString line;
	_stprintf(path, _T("%s\\config.csv"), m_SaveDir);
	if(_taccess(path, 0) == -1) return;
	CStdioFile file(path, CFile::modeRead);
	while(file.ReadString(line))
	{
		HousePicture hp;
		line.Trim();
		int pos = line.Find(_T(","));
		hp.m_order = _ttoi(line.Left(pos));
		line = line.Right(line.GetLength() - pos - 1);
		pos = line.Find(_T(","));
		hp.m_name.Format(_T("%s"), line.Left(pos));
		line = line.Right(line.GetLength() - pos - 1);
		pos = line.Find(_T(","));
		hp.m_desc.Format(_T("%s"), line.Left(pos));
		line = line.Right(line.GetLength() - pos - 1);
		hp.m_pos.Format(_T("%s"), line);
		m_PicInfo[hp.m_order] = hp;
	}
}

bool checkGongNengQuIntersect(const AcDbObjectId &fid, const AcDbObjectId &iid)
{
	AcDbEntity *iEnt = NULL; bool flag = true;
	if(Acad::eOk != acdbOpenAcDbEntity(iEnt, fid, AcDb::kForRead)) return false;
	AcDbEntity *iEnt1 = NULL; AcGePoint3dArray node;
	if(Acad::eOk != acdbOpenAcDbEntity(iEnt1, iid, AcDb::kForRead))
	{
		iEnt->close(); return false;
	}
	if(Acad::eOk != iEnt1->intersectWith(iEnt, AcDb::kOnBothOperands, node))
	{
		iEnt->close(); iEnt1->close(); return false;
	}
	iEnt->close(); iEnt1->close(); AcGePoint2dArray node2;
	for(int idx = 0; idx < node.length(); ++idx)
		node2.append(AcGePoint2d(node[idx].x, node[idx].y));
	AcDbObjectId nid = DrawPolyline2D(node2, 0.0, true, _T(""));
	double area = 0.0; GetEntArea(aname(nid), area); EntDel(aname(nid));
	return area == 0.0;
}

void XMJGHouse::readFunctionInfo(const AcDbObjectIdArray &ids, AcDbObjectIdArray &idArr)
{
	int row = 1; ads_point insert,ranglePoint; int count = ids.length();
	if(count <= 10)
		row = count;
	else
	{
		row = 10;
		if(RTCAN == ads_getint(_T("������һ��ע���������<10>��"), &row)) return;
	}
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��Ҫ�����λ�ã�"), insert)) return;
	if (RTNORM != ads_getpoint(insert, _T("\n ��ʰȡ������ʾ�������߽�㣺"), ranglePoint))return;
	TCHAR info[255] = { 0 };
	typedef map<CString, CString, CmpCstr> CWMap;
	CWMap note; CString code;
	double area = 0.0, xs = 0.0; TCHAR layer[255] = { 0 };
	for (int idx = 0; idx < count; ++idx)
	{
		AcDbObjectId fid = ids.at(idx);
		ReadXdata(aname(fid), _T("�۵�"), 0, info);
		if (0 == _tcsicmp(info, _T("true"))) continue;
		GetEntArea(aname(fid), area);
		AcGePoint2dArray node;
		GetPlList(aname(fid), node);
		AcDbObjectIdArray gnqids;
		ssFromNodes(gnqids, node, 1, 0.0);
		gnqids.remove(fid);
		//���亯����ɾ��gnqids�е����˹�ϵ�еĵ�
		for (int indx = 0; indx < gnqids.length(); ++indx)
		{
			AcDbObjectId gnqid = gnqids.at(indx);
			GetEntLayer(gnqid, layer);
			if (m_gnq._layer.CompareNoCase(layer) != 0
				&& m_cw._layer.CompareNoCase(layer) != 0) continue;
			if (m_cw._layer.CompareNoCase(layer) == 0)
			{
				ReadXdata(aname(gnqid), _T("PR"), 0, info);
				if (_tcscmp(info, _T("1")) != 0) continue;
			}
			if (m_gnq._layer.CompareNoCase(layer) == 0)
			{
				ReadXdata(aname(gnqid), _T("�۵�"), 0, info);
				if (0 == _tcsicmp(info, _T("false")))continue;
			}
			double subarea = 0.0;
			GetEntArea(aname(gnqid), subarea);
			area -= subarea;
		}
		GetEntLayer(fid, layer);
		if (m_gnq._layer.CompareNoCase(layer) == 0)
		{
			ReadXdata(aname(fid), _T("���ϵ��"), STRING_TYPE, info);
			xs = _tstof(info); area *= xs;
			ReadXdata(aname(fid), _T("���������"), 0, info);
			code = info;
			ReadXdata(aname(fid), _T("���������"), 0, info);
			code += info;
		}
		else if (m_cw._layer.CompareNoCase(layer) == 0)
		{
			ReadXdata(aname(fid), _T("PR"), 0, info);
			if (_tcscmp(info, _T("2")) != 0) area = 0;
			else
			{
				ReadXdata(aname(fid), _T("CWBH"), STRING_TYPE, info);
				code.Format(_T("%s"), info);
			}
		}
		if (area < EPS) continue;
		CString temp(code); int idxx = 1;
		while (note.find(temp) != note.end())
			temp.Format(_T("%s_%d"), code, idxx++);
		code = temp;
		note[code].Format(_T("%.3lf"), area);
	}
	CString text;
	int index = 0;
	for (CWMap::iterator ait = note.begin(); ait != note.end(); ++ait)
	{
		if (index > row - 1 && index%row == 0)
		{
			text += _T("\\P");
		}
		CString temp;
		temp.Format(_T("%-4s= %-7s    "), ait->first, ait->second);
		text += temp;
		index++;
	}
	AcDbObjectId tid = DrawMText(insert, text, _T("����"), m_mjzj._size*m_Scale, 
		fabs(ranglePoint[X] - insert[X]), AcDbMText::kTopLeft);
	setlayer(aname(tid), m_mjzj._layer); setcolor(aname(tid), m_mjzj._color);
	AddXdata(aname(tid), _T("ZJLX"), 0, _T("GNQMJZJ"));		//���������ע��
}

void XMJGHouse::drawFunctionFrame(AcDbObjectId &ids, AcDbObjectIdArray &IdArr)
{
	ads_point lb, rt; IdArr.append(ids);
	GetEntsExtent(IdArr, lb, rt, 5);
	AcGePoint2dArray node;
	ads_point lt, rb;
	lt[X] = lb[X]; lt[Y] = rt[Y];
	rb[X] = rt[X]; rb[Y] = lb[Y];
	node.append(apoint(lb)); node.append(apoint(lt));
	node.append(apoint(rt)); node.append(apoint(rb));
	AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T(""));
	setlayer(aname(id), _T("ƽ��ͼ������"));
}

bool XMJGHouse::getLayerArea(const AcDbObjectId &id, LayerHAH &hah)
{
	AcGePoint2dArray node; GetPlList(aname(id), node);
	ads_name ssName; ssFromNodes(node, ssName, _T("CP"));
	AcDbObjectIdArray ids; SSToIdArr(ssName, ids);
	int len = ids.length(); TCHAR info[255] = {0};
	double jzmj = 0.0, jrmj = 0.0;
	getWaiBanQiangArea(id, jzmj, jrmj);
	for(int idx = 0; idx < len; ++idx)
	{
		FunctionHAH funarea;
		AcDbObjectId funid = ids.at(idx);
		GetEntLayer(funid, info);
		if(m_gnq._layer.CompareNoCase(info) != 0 && m_cw._layer.CompareNoCase(info) != 0) continue;
		getFunctionArea(funid, funarea);
		addFuncion2Layer(funarea, hah);
		SetLineWidth(funid, 0.06);
	}
	hah.m_jzzmj += jzmj; hah.m_jrzmj += jrmj;
	ReadXdata(aname(id), _T("¥���"), 0, info);
	hah.m_lch = info;
	getJGLayerNum(info, hah.m_slayer, hah.m_elayer);
	ReadXdata(aname(id), _T("¥���"), 0, info);
	hah.m_lcg = _tstof(info);
	return true;
}

void XMJGHouse::layerArea(const AcDbObjectId &id, double &jzmj, double &jrmj, double &zdmj)
{
	AcGePoint2dArray node; AcDbObjectIdArray ids;
	GetPlList(aname(id), node); TCHAR xs[255] = {0};
	ssFromNodes(ids, node); ids.remove(id); double mj;
	TCHAR ldh[255] = {0}, lch[255] = {0}; int val = 0;
	ReadXdata(aname(id), _T("¥����"), 0, ldh);
	ReadXdata(aname(id), _T("¥���"), 0, lch);
	IProjectMDB pdb; MStr f2; CString linfo(lch);
	f2[_T("¥����")] = ldh; int pos = linfo.Find(_T(" ---> "));
	f2[_T("��ʼ����")] = (pos == -1) ? lch : linfo.Left(pos);
	MStr lrecord = pdb.getCXXTableInfo(f2); MStr f1;
	f1[_T("����")] = ldh; f1[_T("���")] = lch;
	pdb.deleteTableRecord(_T("GNQXX"), f1);
	CString lcgd = lrecord[_T("ʵ����")];
	for(int i = 0; i < ids.length(); i++)
	{
		AcDbObjectId gnq = ids.at(i); double mj1 = 0, mj2 = 0, mj3 = 0;
		TCHAR  szngq[255] = {0}, gh[255] = {0};
		GetEntLayer(gnq, szngq); double cwArea = 0.0;
		GetPlList(aname(gnq), node);
		AcDbObjectIdArray gnqs;
		ssFromNodes(gnqs, node, 1, 0.0);
		if(m_gnq._layer.CompareNoCase(szngq) != 0) continue;
		if(!ReadXdata(aname(gnq), _T("����������"), 0, szngq)) continue;
		GetEntArea(aname(gnq), mj); double area = 0.0;
		for(int indx = 0; indx < gnqs.length(); ++indx)
		{
			TCHAR sublayer[255] = {0};
			GetEntLayer(gnqs[indx], sublayer);
			if(m_cw._layer.CompareNoCase(sublayer) == 0)
			{
				GetEntArea(aname(gnqs[indx]), area);
				cwArea += area; mj -= area; continue;
			}
			if(m_gnq._layer.CompareNoCase(sublayer) == 0)
			{
				GetEntArea(aname(gnqs[indx]), area);
				mj -= area;
			}
		}
		ReadXdata(aname(gnq), _T("���ϵ��"), 0, xs);
		mj1 = _tstof(xs) * mj; jzmj += mj1;
		ReadXdata(aname(gnq), _T("����ϵ��"), 0, xs);
		mj2 = _tstof(xs) * mj; jrmj += mj2;
		mj3 = mj; zdmj += mj3;
		if(!ReadXdata(aname(gnq), _T("�������߶�"), 0, gh))
			f1[_T("�߶�")] = lrecord[_T("ʵ����")];
		else
			f1[_T("�߶�")] = gh;
		MStr record; record[_T("����")] = ldh; record[_T("����")] = szngq;
		record[_T("���")] = lch;  record[_T("�߶�")] = gh; f1[_T("����")] = szngq;
		MStr trecrd = pdb.getGNQXXInfoEx(f1);

		record[_T("ͼ�����")].Format(_T("%.3lf"), mj3 + _tstof(trecrd[_T("ͼ�����")]));
		record[_T("�������")].Format(_T("%.3lf"), mj1 + _tstof(trecrd[_T("�������")]));
		record[_T("�������")].Format(_T("%.3lf"), mj2 + _tstof(trecrd[_T("�������")]));
		pdb.addGNQXXInfo(record, f1);

		if(cwArea > 0)
		{
			double jzarea = 0.0, jrarea = 0.0;
			f1[_T("����")] = _T("��λ");
			f1[_T("�߶�")] = lrecord[_T("ʵ����")];
			record[_T("�߶�")] = lrecord[_T("ʵ����")];
			record[_T("����")] = _T("��λ");
			record[_T("�������")].Format(_T("%.3lf"), cwArea);
			record[_T("�������")].Format(_T("%.3lf"), 0.00);
			pdb.addGNQXXInfo(record, f1);
		}
	}
	double jzarea = 0.0, jrarea = 0.0;
	getWaiBanQiangArea(id, jzarea, jrarea);
	if(jzarea > 0)
	{
		jrmj += jrarea; jzmj += jzarea;
		f1[_T("����")] = _T("���ǽ"); MStr record;
		record[_T("���")] = lch; record[_T("����")] = ldh;
		record[_T("�߶�")] = lcgd; record[_T("����")] = _T("���ǽ");
		record[_T("�������")].Format(_T("%.3lf"), jzarea);
		if(_ttoi(lrecord[_T("��ʼ����")]) < 0) jrarea = 0.0;
		record[_T("�������")].Format(_T("%.3lf"), jrarea);
		record[_T("ͼ�����")].Format(_T("%.3lf"), jzarea);
		pdb.addGNQXXInfo(record, f1);
	}
}

void XMJGHouse::removeSubFunction(const AcDbObjectId &id, double &jzArea, double &jrArea)
{
	AcGePoint2dArray node;
	GetPlList(aname(id), node);
	AcDbObjectIdArray ids; TCHAR layer[255] = {0};
	_stprintf(layer, _T("%s,%s"), m_gnq._layer, m_cw._layer);
	ssFromNodes(ids, node, 1, 0.5, _T("*POLYLINE"), layer);
	ids.remove(id);
	int len = ids.length();
	TCHAR jzxs[255] = {0};
	TCHAR jrxs[255] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		FunctionHAH hah;
		AcDbObjectId tid = ids[idx];
		//getFunctionArea(tid, hah);
		double area = 0.0;
		GetEntArea(aname(tid), area);
		jzArea -= area;
		jrArea -= area;
	}
}

bool XMJGHouse::getFunctionArea(AcDbObjectId &id, FunctionHAH &hah)//���㹦��������λ���
{//������ID��
	TCHAR layer[20] = { 0 };//�洢������ͼ��
	GetEntLayer(id, layer);//��ù�����ͼ��
	if(m_gnq._layer.CompareNoCase(layer) == 0)//�ж��Ƿ��ǹ�����
	{
		TCHAR value[255] = { 0 };
		double area = 0.0;
		double jzarea = 0.0;//�������
		double jrarea = 0.0;//�������
		GetEntArea(aname(id), area);//���㹦������λ�߽�����Χ���
		jzarea = jrarea = area;
		ReadXdata(aname(id), _T("����ϵ��"), 0, value);//��ȡ��������λ�ļ���ϵ��
		hah.m_jrxs = (float)_tstof(value);//������ϵ����ֵ��hah.m_jrxs
		//hah.m_jrmj = area * _tstof(value);
		ReadXdata(aname(id), _T("���ϵ��"), 0, value);//��ȡ��������λ�����ϵ��
		//hah.m_jzmj = area * _tstof(value);
		removeSubFunction(id, jzarea, jrarea);
		hah.m_jzmj = jzarea * _tstof(value);
		hah.m_jrmj = jrarea * hah.m_jrmj;
		ReadXdata(aname(id), _T("�������߶�"), 0, value);
		hah.m_cg = (float)_tstof(value);
		ReadXdata(aname(id), _T("���������"), 0, value);
		hah.m_bh.Format(_T("%s"), value);
		ReadXdata(aname(id), _T("���������"), 0, value);
		hah.m_bh += value;
	}
	else if(m_cw._layer.CompareNoCase(layer) == 0)//�ж��Ƿ��ǳ�λ
	{
		ReadXdata(aname(id), _T("PR"), 0, layer);
		if(_tcscmp(layer, _T("2")) == 0)
		{
			double area = 0.0;
			GetEntArea(aname(id), area);
			hah.m_jzmj = area;
			hah.m_bh = _T("��λ");
		}
		else
			return false;
	}
	return true;
}

void XMJGHouse::getWaiBanQiangArea(const AcDbObjectId &id, double &jzmj, double &jrmj)
{
	IDataBaseOper oper;	TextProperty wqx, nqx;
	wqx = oper.readTextPropertyTable(_T("������ǽ��"));
	nqx = oper.readTextPropertyTable(_T("������ǽ��"));
	//SelectFilter sf1(8, nqx._layer), sf2(8, wqx._layer), sf3(RTDXF0, _T("*POLYLINE"));//û��ʹ�ã�����ɾ��
	AcGePoint2dArray node; GetPlList(aname(id), node);//���id��ʶ�Ķ���ߵ����нڵ�
	AcDbObjectIdArray wids, nids; double warea = 0.0, narea = 0.0;
	TCHAR layer[255] = {0}; _stprintf(layer, _T("%s"), wqx._layer);
	if(!ssFromNodes(wids, node, 1, 1.0, _T("*POLYLINE"), layer)) return;//ѡ��id��ʶ�Ķ�����ڵ�������ǽ��
	_stprintf(layer, _T("%s"), nqx._layer);
	if(!ssFromNodes(nids, node, 1, 1.0, _T("*POLYLINE"), layer)) return;//ѡ��id��ʶ�Ķ�����ڵ�������ǽ��
	for(int idx = 0; idx < wids.length(); ++idx)//����������ǽ��Χ���֮��
	{
		double area = 0;
		GetEntArea(aname(wids[idx]), area);
		warea += area;
	}
	for(int idx = 0; idx < nids.length(); ++idx)//����������ǽ��Χ���֮��
	{
		double area = 0.0;
		GetEntArea(aname(nids[idx]), area);
		narea += area;
	}
	jzmj += (warea - narea); jrmj += (warea - narea);//���⣺Ϊʲô����������������ȴ��ֵһ��
}

bool XMJGHouse::addFuncion2Layer(const FunctionHAH &func, LayerHAH &layer)
{
	MFFIter it = layer.m_function.find(func.m_cg);
	if(it != layer.m_function.end() && layer.m_function.size() != 0)
	{
		layer.m_function[func.m_cg].m_jrmj += func.m_jrmj;
		layer.m_function[func.m_cg].m_jzmj += func.m_jzmj;
	}
	else
		layer.m_function[func.m_cg] = func;
	layer.m_jrzmj += func.m_jrmj;
	layer.m_jzzmj += func.m_jzmj;
	layer.funccount = (int)layer.m_function.size();
	return true;
}

bool XMJGHouse::addBuild2Layer(const LayerHAH &layer, BuildHAH &hah)
{
	hah.m_jzmjxj += layer.m_jzzmj;
	hah.m_jrmjxj += layer.m_jrzmj;
	CString lname(layer.m_lch);
	lname.Replace(_T(" ---> "), _T("-"));
	hah.m_layers[layer.m_slayer] = layer;
	hah.m_jzgd += layer.m_lcg;
	hah.funccount += layer.funccount;
	return true;
}

bool XMJGHouse::createStratificationPlan(const CString &pathName, double angle)
{
	CloseLayer _layer(_T("�ֲ�ƽ��ͼ�ɹ�����"));
	int len = m_object.length();
	MStr record; JGExportDWG exportdwg;
	getReplaceValue(record);
	CString system = getSystemPath();
	TCHAR info[255] = {0};
	MIId miid; IProjectMDB pdb;
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_object.at(idx);
		ReadXdata(aname(id), _T("¥����"), 0, info);
		MStr filter; filter[_T("¥����")] = info;
		if(!ReadXdata(aname(id), _T("¥���"), 0, info))
		{
			MessageBox(GetForegroundWindow(), _T("�������ò�����"), _T("ȱ��¼�����"), MB_OK);
			return false;
		}
		CString lname(info);
		int pos = lname.Find(_T(" ---> "));
		if(pos == -1) filter[_T("��ʼ����")] = lname;
		else filter[_T("��ʼ����")] = lname.Left(pos);
		MStr linfo = pdb.getCXXTableInfo(filter);
		miid[_ttoi(linfo[_T("¥���")])] = id;
	}
	m_object.setLogicalLength(0); m_object.setPhysicalLength(0);
	for(MIIdIter it = miid.begin(); it != miid.end(); ++it)
		m_object.append(it->second);
	bool isJR = getIsOrNotJR();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_object.at(idx);
		getPlanInfo(id, record, isJR);
		AcDbObjectIdArray objs;
		getExportObjects(id, objs);
		TCHAR pathname[255] = {0};
		_stprintf(pathname, _T("%s�ֲ�ƽ��%d.dwg"), system, idx + 1);
		exportdwg.setSavePath(pathname);
		exportdwg.setObjects(objs);
		exportdwg.setPictureFrame(_T("XMJG�ֲ�ƽ��ͼ"), angle);
		exportdwg.setObjectFilterFlag(FenCengPingMianFlag);
		exportdwg.setCallBack(record, XMJGReplaceField);
		exportdwg.isDrawRectangle(false);
		exportdwg.ExportDwg();
	}
	return true;
}

bool XMJGHouse::combineStratificationPlan(const CString &pathName)
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	int len = m_object.length(); ads_point spt;
	spt[X] = 0; spt[Y] = 0; spt[Z] = 0; TCHAR PN[255] = {0};
	AcDbObjectIdArray ids;	TCHAR pathname[255] = {0};
	ads_point pmin, pmax; double width;
	//	SBreakBlock blocks;
	for(int idx = 0; idx < len; ++idx)
	{
		//		SBreakBlock blocks; 
		_stprintf(pathname, _T("�ֲ�ƽ��%d"), idx + 1);
		AcDbObjectId id = DrawInsert(spt, pathname);
		if(id == AcDbObjectId::kNull) continue;
		GetEntExtent(aname(id), pmin, pmax);
		// 		blocks.SetBlock(id); blocks.Explode();
		// 		GetEntsExtent(blocks.m_objIdArr, pmin, pmax);
		if(idx != 0)
		{
			ads_point ptmin, ptmax;
			GetEntsExtent(ids, ptmin, ptmax);
			double offset = pmax[X] - ptmax[X];
			width = pmax[X] - pmin[X];
			double off = width + 10 - offset;
			MoveEntity(aname(id), off, 0);
			//			MoveEntitys(blocks.m_objIdArr, off, 0);
		}
		ads_name ent, head, last, temp; acdbGetAdsName(ent, id);
		ads_entlast(head);
		ads_command(RTSTR, _T("EXPLODE"), RTENAME, ent, RTNONE);
		ads_entlast(last); ads_entnext(head, temp);
		while(!ads_name_equal(temp, last))
		{
			if(temp[0] == 0) break;
			ids.append(aname(temp));
			ads_name_set(temp, head);
			ads_entnext(head, temp);
		}

		//		blocks.SetBlock(id); blocks.Explode();
		//		ids.append(blocks.m_objIdArr);
		//ids.append(id);
		//		ids.append(blocks.m_objIdArr);
	}
	CSaveBlocks saveblocks;
	saveblocks.setPath(pathName);
	saveblocks.saveBlocks2(ids, true);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	for(int idx = 0; idx < len; ++idx)
	{
		_stprintf(pathname, _T("�ֲ�ƽ��%d"), idx + 1);
		ads_findfile(pathname, PN); 	DeleteFile(PN);
	}
	return true;
}

int XMJGHouse::createChangeAreaDWG(const CString &pathname, double angle, const CString &frame, const CString &title)
{
	CloseLayer _layer(_T("��һ�¶���ͼ�ɹ�����"));
	int len = m_object.length(); int count = 0;
	MStr record; getReplaceValue(record, title);
	XMJGExport exportdwg; TCHAR layer[255] = {0};
	CString path = getSystemPath();
	TCHAR binfo[255] = {0}, linfo[255] = {0};
	TCHAR buildLayer[255] = {0};

	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_object.at(idx);
		AcDbObjectIdArray objs;
		getExportObjects(id, objs);
		bool isBG = false;
		for(int indx = 0; indx < objs.length(); ++indx)
		{
			AcDbObjectId tid = objs.at(indx);
			GetEntLayer(tid, layer);
			if(m_bg._layer.CompareNoCase(layer) != 0) continue;
			isBG = true; break;
		}
		if(isBG == false) continue; ads_point pos;
		ReadXdata(aname(id), _T("¥����"), 0, binfo);
		ReadXdata(aname(id), _T("¥���"), 0, linfo);
		CString layerName(linfo);
		int loc = layerName.Find(_T(" ---> "));
		if(loc < 0)
		{
			if(_ttoi(layerName) < 0)
				layerName = g_fieldchagetable.getNameByIndex(linfo);
			else
				layerName.Append(_T("��"));
		}
		else
		{ layerName.Append(_T("��")); }
		_stprintf(buildLayer, _T("%s    %s"), binfo, layerName);
		if(RTCAN == ads_getpoint(NULL, _T("\n��ʰȡ�����Լ����ע�ǵ�λ�ã�"), pos)) return 0;
		AcDbObjectId tid = DrawText(pos, buildLayer, _T("C"), _T(""), 0.7, 0.0, 1.0);
		SetTextStyle(tid, _T("����"), 1.0); setlayer(aname(tid), _T("���ע��"));
		objs.append(tid); TCHAR pathName[255] = {0};
		_stprintf(pathName, _T("%s���ͼ%d.dwg"), path, ++count);
		exportdwg.setSavePath(pathName);
		exportdwg.setObjects(objs);
		exportdwg.setScaleEntity(tid);
		exportdwg.setPictureFrame(frame, angle);
		exportdwg.setObjectFilterFlag(ChangeAreaFlag);
		exportdwg.setCallBack(record, XMJGReplaceField);
		exportdwg.ExportDwg(); EntDel(aname(tid));
	}
	return count;
}

bool XMJGHouse::combineChangeAreaDWG(int count, const CString &pathName)
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	int len = m_object.length(); ads_point spt, pmin, pmax;
	spt[X] = 0; spt[Y] = 0; spt[Z] = 0; TCHAR PN[255] = {0};
	AcDbObjectIdArray ids; TCHAR filePath[255] = {0};
	for(int idx = 0; idx < count; ++idx)
	{
		SBreakBlock blocks; double off = 0.0;
		TCHAR pathname[255] = {0}; double width = 0.0;
		_stprintf(pathname, _T("���ͼ%d"), idx + 1);
		AcDbObjectId id = DrawInsert(spt, pathname);
		if(id == AcDbObjectId::kNull) continue;
		blocks.SetBlock(id); blocks.Explode();
		GetEntsExtent(blocks.m_objIdArr, pmin, pmax);
		if(idx != 0)
		{
			ads_point ptmin, ptmax;
			GetEntsExtent(ids, ptmin, ptmax);
			double offset = pmax[X] - ptmax[X];
			width = pmax[X] - pmin[X];
			double off = width + 10 - offset;
			MoveEntitys(blocks.m_objIdArr, off, 0);
		}
		ids.append(blocks.m_objIdArr);
	}
	CSaveBlocks saveblocks;
	saveblocks.setPath(pathName);
	saveblocks.saveBlocks2(ids, true);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	return true;
}

void XMJGHouse::areaStatistic()
{
	SelectFilter sf(8, m_fcpm._layer); AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T(""))) return;
	AcDbObjectIdArray txids = getInnerObjects(id);
	TCHAR zjlx[255] = {0}; CString gnqarea; double area = 0.0;
	for(int idx = 0; idx < txids.length(); ++idx)
	{
		AcDbObjectId txid = txids.at(idx);
		if(false == ReadXdata(aname(txid), _T("ZJLX"), 0, zjlx)) continue;
		if(_tcscmp(zjlx, _T("GNQMJZJ")) != 0) continue;
		GetEntText(aname(txid), zjlx); gnqarea = zjlx;
		VStr gnqa; AnalyzeData(gnqa, gnqarea, _T("   "));
		for(int indx = 0; indx < (int)gnqa.size(); ++indx)
		{
			gnqarea = gnqa[indx];
			int pos = gnqarea.Find(_T("="));
			area += _tstof(gnqarea.Mid(pos + 2));
		}
	}
	ads_point pt;
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ��Ҫע�ǵ�λ�ã�"), pt)) return;
	gnqarea.Format(_T("[S] = %.3lf"), area);
	AcDbObjectId tid = DrawText(pt, gnqarea, _T("ML"), _T(""), m_mjzj._size * m_Scale, 0.0, 1.0);
	setlayer(aname(tid), m_mjzj._layer); setcolor(aname(tid), m_mjzj._color);
}

void XMJGHouse::innerWallLine()
{
	AcDbObjectId id; ads_point pt;
	if(false == GetBD(m_gnq._layer, id, pt, true)) return;
	CSetEntityPropery prop;
	prop.setInnerWall(id);
}

void XMJGHouse::statisticHalfWall()
{
	IDataBaseOper oper;	TextProperty wqx, nqx;
	wqx = oper.readTextPropertyTable(_T("������ǽ��"));
	nqx = oper.readTextPropertyTable(_T("������ǽ��"));
	SelectFilter sf1(8, nqx._layer), sf2(8, wqx._layer);
	AcDbObjectIdArray wqxids, nqxids;
	double nqarea = 0.0, wqarea = 0.0;
	acutPrintf(_T("\n ��ѡ������ǽ�ߣ�"));
	if(false == SelectEntitys(nqxids, sf1, _T(""))) return;
	acutPrintf(_T("\n ��ѡ������ǽ�ߣ�"));
	if(false == SelectEntitys(wqxids, sf2, _T(""))) return;
	for(int idx = 0; idx < nqxids.length(); ++idx)
	{
		double area = 0.0;
		GetEntArea(aname(nqxids[idx]), area);
		nqarea += area;
	}
	for(int idx = 0; idx < wqxids.length(); ++idx)
	{
		double area = 0.0;
		GetEntArea(aname(wqxids[idx]), area);
		wqarea += area;
	}
	nqarea = wqarea - nqarea;
	TCHAR area[255] = {0}; ads_point pos;
	_stprintf(area, _T("Q = %.3lf"), nqarea);
	if(RTNORM != ads_getpoint(NULL, _T("\n ��ѡ�����ĵ㣺"), pos)) return;
	AcDbObjectId tid = DrawText(pos, area, _T("ML"), _T(""), m_mjzj._size * m_Scale, 0.0, 1.0);
	setlayer(aname(tid), m_mjzj._layer); setcolor(aname(tid), m_mjzj._color);
}

void XMJGHouse::modifyJGBuildName()
{
	CSetEntityPropery prope;
	IDataBaseOper oper; TextProperty prop;
	prop = oper.readTextPropery(_T("������������������"), true); \
		AcDbObjectId id; SelectFilter sf(8, prop._layer);
	if(false == SelectEntity(id, sf, _T(""))) return;
	prope.setBuild(id); return;
	TCHAR bName[255] = {0};
	if(RTCAN == ads_getstring(0, _T("\n �����뽨�������ƣ�"), bName)) return;
	//Ѱ���������������ıߵ���ʼ��
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	ads_point ptmin, ptmax;
	GetEntExtent(aname(id), ptmin, ptmax, 0.0);
	AcGePoint2d ptLT;
	ptLT[X] = ptmin[X]; ptLT[Y] = ptmax[Y];
	int nMinDist = 0;
	double dMinDist = 0.0;
	for(int i = 0; i < nodes.length() - 1; ++i)
	{
		AcGePoint2d ptstr, ptend;
		ptstr.x = nodes[i].x; ptstr.y = nodes[i].y;
		ptend.x = nodes[(i + 1) % nodes.length()].x; ptend.y = nodes[(i + 1) % nodes.length()].y;
		double dist = SquareDistance(ptLT, ptstr, ptend);
		if(i == 0)
		{
			dMinDist = dist;
			continue;
		}
		if(distOF2d(ptLT, ptend) < EPS)
			dist = distOF2d(ptLT, ptstr);
		if(dist < dMinDist)
		{
			dMinDist = dist;
			nMinDist = i;
		}
	}
	AcGePoint3d mid;
	mid.x = nodes[nMinDist].x + 2;
	mid.y = nodes[nMinDist].y - 2;

	AcDbObjectId mcid = DrawText(mid, bName, _T("MC"), _T(""), m_fwbczj._size * m_Scale);
	CString handle = GetHandleByObject(mcid);
	setlayer(aname(mcid), m_fwbczj._layer);
	setcolor(aname(mcid), m_fwbczj._color);
}

void XMJGHouse::checkFunctionDefine()
{
	SelectFilter sf(8, m_fcpm._layer); AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T(""))) return;
	AcDbObjectIdArray ids = getInnerObjects(id);
	TCHAR layer[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId gnqid = ids.at(idx);
		GetEntLayer(gnqid, layer);
		if(m_gnq._layer.CompareNoCase(layer) != 0)
			continue;
	}
}

void XMJGHouse::drawLayerFirst()
{

}

void XMJGHouse::boundaryNote()
{
	ads_point pos; ads_name ent, last;
	int notelocal = 1; double mindist = 0.1;
	if(RTCAN == ads_getreal(_T("\n �����ñ�ע��ֵ��<0.1>"), &mindist)) return;
	if(RTCAN == ads_getint(_T("\n ������ע�ǵ�λ��<1>�ⲿ��<2>�ڲ���[1]"), &notelocal)) return;
	while(true)
	{
		if (RTNORM != ads_getpoint(NULL, _T("\n��ָ���ڲ�һ��"), pos))return;
		ads_entlast(last);
		ads_command(RTSTR, _T("-BOUNDARY"), RTSTR, _T("A"), RTSTR, _T("I"), RTSTR, _T("N"), RTSTR, _T(""), RTSTR, _T(""), RTPOINT, pos, RTSTR, _T(""), RTNONE);
		ads_entlast(ent); if(ads_name_equal(last, ent)) return;
		AcGePoint2dArray nodes;
		GetPlList(ent, nodes);
		if(IsDeasil(nodes) == false) nodes.reverse();
		double angle_reverse = notelocal == 1 ? -0.5 : 0.5;
		int len = nodes.length();
		bool isClose = 9 < GetPolyType(ent, true);
		for(int i = 0; i < len - (isClose ? 0 : 1); i++)
		{
			AcGePoint2d pts = nodes[i % len];
			AcGePoint2d pte = nodes[(i + 1) % len];
			AcGePoint2d midpt(pte.x * 0.5 + pts.x * 0.5, pte.y * 0.5 + pts.y * 0.5);
			double dist = distOF2d(pts, pte);
			if(dist < mindist) continue;
			CString dist_str;
			dist_str.Format(_T("%.2f"), dist);
			double angle = ads_angle(apoint(pte), apoint(pts));
			ads_point insertpt;
			ads_polar(apoint(midpt), angle + angle_reverse * PI, 0.4 * m_Scale, insertpt);
			if(angle > PI / 2 + EPS && angle < 3 * PI / 2 + EPS)
				angle -= PI;
			AcDbObjectId textID = DrawText(insertpt, dist_str, _T("MC"), _T("20000"), m_fwbczj._size * m_Scale, angle, 1.0);
			setcolor(aname(textID), m_fwbczj._color);
			setlayer(aname(textID), m_fwbczj._layer);
		}
		ads_entdel(ent);
	}
}

#pragma endregion

#pragma endregion

XMJGFinalAcceptance::XMJGFinalAcceptance()
{

}

XMJGFinalAcceptance::~XMJGFinalAcceptance()
{

}

void XMJGFinalAcceptance::FillBuildingArea()
{
}

void XMJGTest()
{
	SmartTable::Word word; TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("�����ĵ�.docx"), path)) return;
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath); char savepath[1024] = {0};
	CString edgetext, buildtext; char distance[2048] = {0};
	int prop = word.setPropertyTable(0);
	word.appendImage("C:\\Users\\Administrator\\Desktop\\jpg\\test3.jpg", 14.5, 10.88);
	word.appendParagraph("test3", 0);
	word.appendImage("C:\\Users\\Administrator\\Desktop\\jpg\\test2.jpg", 14.55, 19.4);
	word.appendParagraph("test2", 0);
	word.removeTable(0);
	word.save("C:\\Users\\Administrator\\Desktop\\jpg\\test.docx");
	SmartTable::WordTable table;
}

void XMJGHouse::CompareExport()
{
	XMJGExportZPT zpt;
	zpt.setUndoFlag();
	zpt.setExportRange();

	IProjectMDB pdb;
	VMStr areas = pdb.getCXXTableInfo();
	double dsarea = 0.0, dxarea = 0.0; TCHAR pszArea[255] = {0};
	for(int idx = 0; idx < (int)areas.size(); ++idx)
	{
		MStr area = areas[idx];
		if(_ttoi(area[_T("���")]) < 0) dxarea += _tstof(area[_T("�������")]);
		else  dsarea += _tstof(area[_T("�������")]);
	}
	MStr mjxx = pdb.getMJXXTableInfo();
	double jzydmj = _tstof(mjxx[_T("�����õ����")]);
	zpt.setJZMJArea(dsarea + dxarea, jzydmj, dsarea, dxarea);
	zpt.drawCompareMesh();
	CString pathName = zpt.beginExport(_T("�����������ϳ�λ��һ��ͼ"));
	zpt.beginUndo();
	openDwgCommand(pathName);
}
