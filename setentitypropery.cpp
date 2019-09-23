#include "stdafx.h"
#include "setentitypropery.h"
#include "XMJGReadData.h"
#include "Global.h"

CSetEntityPropery::CSetEntityPropery()
	: m_scale(500), m_gnqbh(0)
{
	read1(m_scale); m_scale /= 1000.0;
}

CSetEntityPropery::~CSetEntityPropery()
{
}

void CSetEntityPropery::setProjectLine(const AcDbObjectId &id)
{
	addapp(aname(id), _T("999950"));
	TextProperty text = oper.readTextPropery(_T("项目范围线"));
	setcolor(aname(id), text._color); setlayer(aname(id), text._layer);
	TextProperty build = oper.readTextPropery(_T("建筑物轮廓"));
	TextProperty dxfw = oper.readTextPropery(_T("建筑竣工地下范围线"));
	AcGePoint2dArray node; GetPlList(aname(id), node);
	AcDbObjectIdArray ids; ssFromNodes(ids, node); ids.remove(id);
	double area = 0.0, temp = 0.0, dxfwx = 0.0; int zs = 0;
	TCHAR layer[255] = {0}, name[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId tid = ids.at(idx);
		GetEntityType(aname(tid), name);
		if(_tcscmp(name, _T("TEXT")) == 0 || _tcscmp(name, _T("text")) == 0) continue;
		GetEntLayer(tid, layer);
		if(build._layer.CompareNoCase(layer) == 0)
		{
			GetEntArea(aname(tid), temp);
			area += temp; zs += 1;
		}
		else if(dxfw._layer.CompareNoCase(layer) == 0)
		{
			GetEntArea(aname(tid), temp);
			dxfwx += temp;
		}
	}
	_stprintf(name, _T("%d"), zs); 	AddXdata(aname(id), _T("ZS"), 1, name);
	_stprintf(name, _T("%.2lf"), area);  AddXdata(aname(id), _T("DSJZMJ"), 3, name);
	_stprintf(name, _T("%.2lf"), dxfwx); AddXdata(aname(id), _T("DXJZMJ"), 3, name);
	_stprintf(name, _T("%.2lf"), area + dxfwx); AddXdata(aname(id), _T("ZJZMJ"), 3, name);
	//	GetEntArea(aname(id), area);
	IProjectMDB pdb; MStr records = pdb.getMJXXTableInfo();
	_stprintf(name, _T("%.2lf"), _tstof(records[_T("实测建筑用地面积")]));
	//_stprintf(name, _T("%.2lf"), area);
	AddXdata(aname(id), _T("YDMJ"), 3, name);
	setlayer(aname(id), text._layer); setcolor(aname(id), text._color);
	/*IProjectMDB pdb;*/ MStr info = pdb.getJBXXTableInfo();
	AddXdata(aname(id), _T("XMBH"), 0, info[_T("受理号")]);
	AddXdata(aname(id), _T("XMMC"), 0, info[_T("项目名称")]);
	AddXdata(aname(id), _T("ZL"), 0, info[_T("项目地点")]);
	AddXdata(aname(id), _T("YDDW"), 0, info[_T("建设单位")]);
	AddXdata(aname(id), _T("CHDW"), 0, info[_T("测量单位")]);
	GetEntArea(aname(id), area); MStr record;
	record[_T("实测建筑用地面积")].Format(_T("%.2lf"), area);
	pdb.setMJXXTableInfo(record);

}

void CSetEntityPropery::setDXFWX(const AcDbObjectId &id)
{
	TextProperty dxfwx = oper.readTextPropery(_T("建筑竣工地下范围线"), true, true);
	setlayer(aname(id), dxfwx._layer); setcolor(aname(id), dxfwx._color);
	SetLineWidth(id, dxfwx._size);
	setlinetype(aname(id), dxfwx._font, 0.5); //线性
}

void CSetEntityPropery::setGNQ(const AcDbObjectId &id)
{
	TextProperty gnq = oper.readTextPropery(_T("建筑竣工功能区"));
	setlayer(aname(id), gnq._layer); setcolor(aname(id), gnq._color);
}

void CSetEntityPropery::setBG(const AcDbObjectId &id)
{
	TextProperty bg = oper.readTextPropery(_T("建筑竣工变更"), true);
	setlayer(aname(id), bg._layer); setcolor(aname(id), bg._color);
	SetLineWidth(id, bg._size);
	XMJGHouse house; house.setChangeAreasInfo(id);
}

void CSetEntityPropery::setBridge(const AcDbObjectId &id)
{
	TextProperty bridge = oper.readTextPropery(_T("天桥竣工桥边界"), true);
	setlayer(aname(id), bridge._layer); setcolor(aname(id), bridge._color);
	if(bridge._size != -1.0) SetLineWidth(aname(id), bridge._size * m_scale);
}

void CSetEntityPropery::setRedLine(const AcDbObjectId &id)
{
	setlayer(aname(id), _T("红线")); setcolor(aname(id), 1);
}

void CSetEntityPropery::setJGBuild(const AcDbObjectId &id)
{
	double ghjzgd = 0.0; IProjectMDB pdb;
	int dxxCount = 0, bgCount = 0; TCHAR bName[255] = {0};
	if(RTCAN == ads_getstring(0, _T("\n 请输入建筑物名称:"), bName)) return;
	{
		MStr filter; filter[_T("楼栋号")].Format(_T("%s"), bName);
		dxxCount = pdb.getRecordCount(_T("DXX"), filter);
		filter.clear(); filter[_T("GUID")].Format(_T("%s"), bName);
		bgCount = pdb.getRecordCount(_T("BG"), filter);
	}
	CString bNameStr(bName); TextProperty build;
	bool flag = (bNameStr.FindOneOf(_T("地下")) != -1);
	build = oper.readTextPropery(_T("建筑竣工建筑物轮廓"));
	if(flag)
	{
		TextProperty dxfwx = oper.readTextPropery(_T("建筑竣工地下范围线"), true, true);
		setcolor(aname(id), dxfwx._color); SetLineWidth(id, dxfwx._size);
		setlinetype(aname(id), dxfwx._font, 0.5); //线性
	}
	else
		setcolor(aname(id), build._color);

	TextProperty jzwmc = oper.readTextPropery(_T("建筑竣工建筑物名称"), true);
	setlayer(aname(id), build._layer);
	AddXdata(aname(id), _T("建筑物名称"), 0, bName);
	AddXdata(aname(id), _T("楼栋号"), 0, bName);
	if (dxxCount == 0 && _tcscmp(bName, _T("地下室")) != 0)
	{
		if(RTNORM != ads_getreal(_T("\n 请输入规划楼栋高度"), &ghjzgd)) return;
		MStr record; record[_T("楼栋号")] = bName;
		record[_T("规划高度")].Format(_T("%.2lf"), ghjzgd);
		pdb.setDXXTalbeInfo(MStr(), record);

	}
	if(bgCount == 0)
	{
		pdb.setBGGUID(bName);
	}
	if (_tcscmp(bName, _T("地下室")) != 0)
	{
		AcGePoint3d mid = getRectangleTopLeftPos(id);
		AcDbObjectId mcid = DrawText(mid, bName, _T("MC"), _T(""), jzwmc._size * m_scale);
		CString handle = GetHandleByObject(mcid);
		setlayer(aname(mcid), jzwmc._layer);
		setcolor(aname(mcid), jzwmc._color);
		AddXdata(aname(id), _T("JZWMCZJ"), 0, handle);
	}

	SelectFilter sf1(8, _T("XMFW")), sf2(5020, _T("*POLYLINE"));
	AcDbObjectId xmfw; TCHAR zs[255] = {0};
	if(!SelectEntity(xmfw, sf1, sf2, _T("X"))) return;
	ReadXdata(aname(xmfw), _T("ZS"), 1, zs);
	_stprintf(zs, _T("%d"), _tstoi(zs) + 1);
	AddXdata(aname(xmfw), _T("ZS"), 1, zs);
}

AcGePoint3d CSetEntityPropery::getRectangleTopLeftPos(const AcDbObjectId &id)
{
	//寻找离外包矩形最近的边的起始点
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	if(!IsDeasil(nodes))
		nodes.reverse();
	ads_point ptmin, ptmax;
	GetEntExtent(aname(id), ptmin, ptmax, 0.0);
	AcGePoint2d ptLT;
	ptLT[X] = ptmin[X]; ptLT[Y] = ptmin[Y];
	int nMinDist = 0; int len = nodes.length();
	double dMinDist = 999;
	for(int i = 0; i < len; ++i)
	{
		double dist = distOF2d(ptLT, nodes[i]);
		if(dist < dMinDist)
		{
			dMinDist = dist;
			nMinDist = i;
		}
	}

	AcGePoint2d pt1 = nodes[(len + nMinDist - 1) % len];
	AcGePoint2d pt2 = nodes[(len + nMinDist) % len];
	AcGePoint2d pt3 = nodes[(len + nMinDist + 1) % len];
	double angle1 = ads_angle(apoint(pt2), apoint(pt1));
	double angle2 = ads_angle(apoint(pt2), apoint(pt3));
	angle1 = angle1 > PI ? (angle1 - 2 * PI) : angle1;
	angle2 = angle2 > PI ? (angle2 - 2 * PI) : angle2;

	double angle = (angle1 + angle2) / 2; ads_point ret;
	ads_polar(apoint(pt2), angle, 4, ret);
	AcGePoint3d mid; mid.x = ret[0];
	mid.y = ret[1]; mid.z = ret[2];
	return mid;
}

void CSetEntityPropery::setBuild(const AcDbObjectId &id)
{
	TextProperty build = oper.readTextPropery(_T("建筑物轮廓"));
	TextProperty jzwmc = oper.readTextPropery(_T("建筑物名称"), true);
	setlayer(aname(id), build._layer); setcolor(aname(id), build._color);
	TCHAR bName[255] = {0}; ReadXdata(aname(id), _T("JZWMCZJ"), 0, bName);
	AcDbObjectId bObj = GetObjectByHandle(bName); EntDel(aname(bObj));
	if(RTCAN == ads_getstring(0, _T("\n 请输入建筑物名称:"), bName)) return;
	AddXdata(aname(id), _T("建筑物名称"), 0, bName);
	AddXdata(aname(id), _T("楼栋号"), 0, bName);
	MStr filter; 
	IProjectMDB pdb;
	filter[_T("楼栋号")].Format(_T("%s"), bName);
	int dxxCount = 0; 
	dxxCount = pdb.getRecordCount(_T("DXX"), filter); filter.clear();
	if (dxxCount == 0)
	{
		double ghjzgd;
		if (RTNORM != ads_getreal(_T("\n 请输入规划楼栋高度"), &ghjzgd))return;
		filter[_T("楼栋号")] = bName;
		filter[_T("规划高度")].Format(_T("%.21f"), ghjzgd);
		pdb.setDXXTalbeInfo(MStr(), filter); filter.clear();
	}
	filter[_T("GUID")].Format(_T("%s"), bName);
	int bgCount = 0;
	bgCount = pdb.getRecordCount(_T("BG"), filter); filter.clear();
	if (bgCount == 0)
	{
		pdb.setBGGUID(bName);
	}
	AcGePoint3d mid = getRectangleTopLeftPos(id);
	AcDbObjectId mcid = DrawText(mid, bName, _T("MC"), _T(""), jzwmc._size * m_scale);
	CString handle = GetHandleByObject(mcid);
	setlayer(aname(mcid), jzwmc._layer);
	setcolor(aname(mcid), jzwmc._color);
	AddXdata(aname(id), _T("JZWMCZJ"), 0, handle);
	//IProjectMDB pdb; pdb.setBGGUID(bName);

	SelectFilter sf1(8, _T("XMFW")), sf2(5020, _T("*POLYLINE"));
	AcDbObjectId xmfw; TCHAR zs[255] = {0};
	if(!SelectEntity(xmfw, sf1, sf2, _T("X"))) return;
	ReadXdata(aname(xmfw), _T("ZS"), 1, zs);
	_stprintf(zs, _T("%d"), _ttoi(zs) + 1);
	AddXdata(aname(xmfw), _T("ZS"), 1, zs);
}

void CSetEntityPropery::setBuildBoundaries(const AcDbObjectId &id)
{
	TextProperty build = oper.readTextPropery(_T("周边建筑物"));
	setlayer(aname(id), build._layer); setcolor(aname(id), build._color);
}

void CSetEntityPropery::setExteriorWall(const AcDbObjectId &id)
{
	TextProperty ew = oper.readTextPropertyTable(_T("建筑外墙线"));
	setlayer(aname(id), ew._layer); setcolor(aname(id), ew._color);
	SetLineWidth(aname(id), ew._size);
	AddXdata(aname(id), _T("XLX"), 0, _T("WQX"));
}

void CSetEntityPropery::setInnerWall(const AcDbObjectId &id)
{
	TextProperty ew = oper.readTextPropertyTable(_T("建筑内墙线"));
	setlayer(aname(id), ew._layer); setcolor(aname(id), ew._color);
	SetLineWidth(aname(id), ew._size);
	AddXdata(aname(id), _T("XLX"), 0, _T("NQX"));
}