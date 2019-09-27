#include "stdafx.h"
#include "Global.h"
#include "XMJGMacro.h"
#ifndef DOMAINOPERATE_H
#include "domainoperate.h"
#endif /*DOMAINOPERATE_H*/

DomainOperate::DomainOperate()
{
	IDataBaseOper oper;
	oper.readCommonTable(_T("建筑竣工功能区图层"), m_gnq._layer);
	oper.readCommonTable(_T("建筑竣工功能区颜色"), m_gnq._color);
	// 	oper.readCommonTable(_T("建筑竣工车位注记大小"), m_cwzj._size);
	// 	oper.readCommonTable(_T("建筑竣工车位注记图层"), m_cwzj._layer);
	// 	oper.readCommonTable(_T("建筑竣工车位注记颜色"), m_cwzj._color);
	oper.readCommonTable(_T("建筑竣工分层平面图图层"), m_fcpm._layer);
	oper.readCommonTable(_T("建筑竣工分层平面图颜色"), m_fcpm._color);
}

DomainOperate::~DomainOperate()
{

}

void DomainOperate::setDomainProperty()
{
	// 	house.addFunctionProperty(lineid, last);
	// 	ads_name entT; acdbGetAdsName(entT, lineid);
	// 	// 扣岛
	// 	if(last.m_island == 1)
	// 	{
	// 		ADDMulXData(lineid, _T("扣岛"), _T("true"), 0, 0);
	// 		setcolor(entT, 7);
	// 	}
	// 	else
	// 	{
	// 		ADDMulXData(lineid, _T("扣岛"), _T("false"), 0, 0);
	// 		setcolor(entT, 4);
	// 	}
	// 	house.addFunctionAreaNote(lineid);
	// 	if(last.jm.IsEmpty())
	// 	{
	// 		if(last.code < 9)
	// 			last.cn.Format(_T("0%d"), ++last.code);
	// 		else
	// 			last.cn.Format(_T("%d"), ++last.code);
	// 	}
	// 	else
	// 	{
	// 		if(last.len == 2 && last.code < 9)
	// 			last.cn.Format(_T("0%d"), ++last.code);
	// 		else
	// 			last.cn.Format(_T("%d"), ++last.code);
	// 	}
	// 	if(fabs(_tstof(last.mjxs) - 0.5) < EPS)
	// 		setlinetype(aname(lineid), _T("x6"));
	// 	SetLineWidth(lineid, 0.06);	gnqs.append(lineid); acdbGetAdsName(entT, lineid);
	// 	acedCommand(RTSTR, _T("draworder"), RTENAME, entT, RTSTR, _T(""), RTSTR, _T("F"), 0);
}

void firstLetter(int nCode, CString& sLetter)
{
	if(nCode >= 1601 && nCode < 1637) sLetter = _T("A");
	if(nCode >= 1637 && nCode < 1833) sLetter = _T("B");
	if(nCode >= 1833 && nCode < 2078) sLetter = _T("C");
	if(nCode >= 2078 && nCode < 2274) sLetter = _T("D");
	if(nCode >= 2274 && nCode < 2302) sLetter = _T("E");
	if(nCode >= 2302 && nCode < 2433) sLetter = _T("F");
	if(nCode >= 2433 && nCode < 2594) sLetter = _T("G");
	if(nCode >= 2594 && nCode < 2787) sLetter = _T("H");
	if(nCode >= 2787 && nCode < 3106) sLetter = _T("J");
	if(nCode >= 3106 && nCode < 3212) sLetter = _T("K");
	if(nCode >= 3212 && nCode < 3472) sLetter = _T("L");
	if(nCode >= 3472 && nCode < 3635) sLetter = _T("M");
	if(nCode >= 3635 && nCode < 3722) sLetter = _T("N");
	if(nCode >= 3722 && nCode < 3730) sLetter = _T("O");
	if(nCode >= 3730 && nCode < 3858) sLetter = _T("P");
	if(nCode >= 3858 && nCode < 4027) sLetter = _T("Q");
	if(nCode >= 4027 && nCode < 4086) sLetter = _T("R");
	if(nCode >= 4086 && nCode < 4390) sLetter = _T("S");
	if(nCode >= 4390 && nCode < 4558) sLetter = _T("T");
	if(nCode >= 4558 && nCode < 4684) sLetter = _T("W");
	if(nCode >= 4684 && nCode < 4925) sLetter = _T("X");
	if(nCode >= 4925 && nCode < 5249) sLetter = _T("Y");
	if(nCode >= 5249 && nCode < 5590) sLetter = _T("Z");
}

bool DomainOperate::createDomainByDraw(AcDbObjectId &lineid)
{
	lineid = drawPolyline(m_gnq._layer, m_gnq._color);
	return lineid != AcDbObjectId::kNull;
	// 	if(lineid == AcDbObjectId::kNull) return false;
	// 	ads_point ptmin, ptmax; GetScreen(ptmin, ptmax);
	// 	TCHAR height[255] = {0}, ldh[255] = {0}, lch[255] = {0};
	// 	AcDbObjectId lcid = getBuildLayerRangeEntity(lineid);
	// 	ReadXdata(aname(lcid), _T("楼栋号"), 0, ldh);
	// 	ReadXdata(aname(lcid), _T("楼层号"), 0, lch);
	//	return true;
}

void getFirstLetter(const CString &sName, CString& sFirstLetter)
{
	//高低字节
	TCHAR cHigh = 0, cLow = 0;
	//内码
	int  nCode = 0;
	//每个字的拼音首字母
	CString sLetter;

	sFirstLetter.Empty();
	int nLen = sName.GetLength();

	char* buffer = new char[nLen * sizeof(TCHAR) + 1];
	ZeroMemory(buffer, nLen * sizeof(TCHAR) + 1);
	WideCharToMultiByte(CP_ACP, NULL, sName, nLen, buffer, nLen * sizeof(TCHAR) + 1, NULL, NULL);

	for(UINT i = 0; i < nLen * sizeof(TCHAR); i++)
	{
		if((UCHAR)buffer[i] < 0x80)
			continue;
		cHigh = (UCHAR)buffer[i];
		cLow = (UCHAR)buffer[i + 1];
		if(cHigh < 0xa1 || cLow < 0xa1)
			continue;
		else
			nCode = (cHigh - 0xa0) * 100 + cLow - 0xa0;

		firstLetter(nCode, sLetter);
		sFirstLetter += sLetter;
		i++;
	}

	delete[]buffer;
}

void DomainOperate::addDomainAnnotate(const AcDbObjectId &gnqid)
{
	TCHAR info[255] = {0}, jm[255] = {0}, bh[255] = {0};
	AcGePoint3d pt; TCHAR jb[255] = {0};
	ReadXdata(aname(gnqid), _T("ZJHANDLE"), 0, jb);
	ReadXdata(aname(gnqid), _T("功能区简码"), 0, jm);
	ReadXdata(aname(gnqid), _T("功能区编号"), 0, bh);
	_stprintf(info, _T("%s%s"), jm, bh);
	if(_tcslen(jb))
	{
		AcDbObjectId tid = GetObjectByHandle(jb);
		SetEntText(aname(tid), info); return;
	}
	GetMiddlePoint(gnqid, pt); IDataBaseOper dbOper;
	TextProperty ann = dbOper.readTextPropertyTable(_T("功能区名称"));
	AcDbObjectId tid = DrawText(pt, info, _T("MC"), _T(""), ann._size);
	setcolor(aname(tid), ann._color); setlayer(aname(tid), ann._layer);
	SetTextStyle(tid, ann._font, 0.8);
	CString handle = GetHandleByObject(tid);
	AddXdata(aname(gnqid), _T("ZJHANDLE"), 0, handle);
}

bool DomainOperate::createDomainByPointSel(AcDbObjectId &lineid)
{
	int selType = 0; AcDbObjectIdArray gnqs; ads_point pt;
	SelectFilter sf(8, _T("功能区")), sf1(RTDXF0, _T("*POLYLINE"));
//	SelectEntitys(gnqs, sf, sf1, _T("X")); bool isInner = false;
//	if(RTNORM != ads_getpoint(NULL, _T("\n请在需要重构的实体中点选（实体必须是闭合）:"), pt)) return false;
	return GetBD(_T("*"), lineid, pt);
	/*
	AcDbObjectId blid = getBuildLayerEntity(apoint(pt));
	SelectFilter sf2(RTDXF0, _T("*LINE")); AcDbObjectIdArray ids;
	if(!SelectEntitys(ids, sf2, _T("X"))) return false;
	CArray<AcGePoint2dArray, AcGePoint2dArray> arrLoops;
	CArray<AcGeDoubleArray, AcGeDoubleArray> arrBulges;

	for(int i = 0; i < ids.length(); i++)
	{
		AcDbObjectId tid = ids[i];
		AcGePoint2dArray node;
		AcGeDoubleArray bulge;
		GetPlList(aname(tid), node);
		arrLoops.Append(node);
		arrBulges.Append(bulge);
	}
	bool existFlag = false; int count = 0; AcDbObjectIdArray idArr;
	GetBoders(idArr, arrLoops, arrBulges);
	for(int idx = 0; idx < idArr.length(); ++idx)
	{

	}

// 
// 	for(int index = 0; index < gnqs.length(); ++index)
// 	{
// 		AcGePoint2dArray node; GetPlList(aname(gnqs[index]), node);
// 		isInner = PointInPolygon_New(node, apoint(pt)) < 1;
// 		if(isInner) break;
// 	}
// 	if(!isInner) lineid = createBoundary(pt);
// 	if(isInner)
// 	{
// 		int yesorno = 0;
// 		if(RTCAN == ads_getint(_T("\n该实体已经是功能区，是否要继续重构<0>是、<1>否：<0>"), &yesorno)) return false;
// 		isInner = (yesorno == 1);
// 	}

		return true;
		*/
}

//选择需要重构的多段线，该多段线已经是功能区，需要用户手动选择输入是否继续重构功能区的选项数字
bool DomainOperate::createDomainByBoundary(AcDbObjectId &lineid)
{
	AcDbObjectIdArray gnqids;//当前DWG中所有功能区
	SelectFilter sf(8, _T("功能区")), sf1(RTDXF0, _T("*POLYLINE"));//创建功能区选择条件（图层、实体类型）
	SelectEntitys(gnqids, sf, sf1, _T("X")); //选择当前DWG中所有功能区
	ads_name plent;//用于储存选择的多段线
	ads_point pt;//选择实体的拾取点
	if(RTNORM != ads_entsel(_T("\n请选择重构的边界"), plent, pt)) return false;//拾取实体
	acdbGetObjectId(lineid, plent);//将实体ID转化为ads_name
	if (gnqids.contains(lineid) == true)
	{
		int yesorno = 0;//如果该实体已经是功能区，判断是否还需要重构
		do
		{

			int es = ads_getint(_T("\n该实体已经是功能区，是否要继续重构<0>是、<1>否：<0>"), &yesorno);
			switch (es)
			{
			case RTNONE:yesorno = 0; break;
			case RTNORM:  if (yesorno != 0 && yesorno != 1)acutPrintf(_T("\n输入选项数字无效，请重新输入！")); break;
			case RTCAN:  return false;
			default: yesorno = 2;
			}
		} while (yesorno != 0 && yesorno != 1);
	}
	return true;
}

AcDbObjectId DomainOperate::getBuildLayerEntity(const AcGePoint2d &pt)
{
	SelectFilter sf1(8, m_fcpm._layer), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids; SelectEntitys(ids, sf1, sf2, _T("X"));
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids[idx];
		AcGePoint2dArray node; GetPlList(aname(id), node);
		if(-1 == PointInPolygon_New(node, apoint(pt))) return id;
	}
	return AcDbObjectId::kNull;
}

AcDbObjectId DomainOperate::getBuildLayerRangeEntity(const AcDbObjectId &gnqid)
{
	AcGePoint3d pt; GetMiddlePoint(gnqid, pt);
	return getBuildLayerEntity(apoint(pt));
}

void DomainOperate::addDomainProperty(const AcDbObjectId &id, const CString &name, const CString &mjxs, const CString &jrxs, const CString &gd, const CString &bh, bool isLand)
{
	if(fabs(_tstof(mjxs) - 0.5) < EPS)
		setlinetype(aname(id), _T("X6"));
	SetLineWidth(id, 0.06); IDataBaseOper oper;
	addXdataProperty(id, name, mjxs, jrxs, gd, bh, isLand);
	setlayer(aname(id), m_gnq._layer);
	setcolor(aname(id), m_gnq._color);
	addDomainAnnotate(id);
}

void DomainOperate::addXdataProperty(const AcDbObjectId &id, const CString &name, const CString &mjxs, const CString &jrxs, const CString &gd, const CString &bh, bool isLand)
{
	double area = 0.0; TCHAR info[25] = {0};
	GetEntArea(aname(id), area); CString jm;
	AddXdata(aname(id), _T("功能区名称"), 0, name);
	AddXdata(aname(id), _T("功能区高度"), 0, gd);
	AddXdata(aname(id), _T("面积系数"), 0, mjxs);
	AddXdata(aname(id), _T("计容系数"), 0, jrxs);
	if(isLand) AddXdata(aname(id), _T("扣岛"), 0, _T("true"));
	else AddXdata(aname(id), _T("扣岛"), 0, _T("false"));
	getFirstLetter(name, jm);
	AddXdata(aname(id), _T("功能区简码"), 0, jm);
	_stprintf(info, _T("%.3lf"), _tstof(mjxs) * area);
	AddXdata(aname(id), _T("建筑面积"), 3, info);
	_stprintf(info, _T("%.3lf"), _tstof(jrxs) * area);
	AddXdata(aname(id), _T("计容面积"), 3, info);
	AddXdata(aname(id), _T("功能区编号"), 0, bh);
}