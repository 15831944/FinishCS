#include "StdAfx.h"
#include "PipeSection.h"
#include "Global.h"
#include "cassGlobal.h"
#include "SaveBlocks.h"
#include "SBreakBlock.h"
#include "XMJGTool.h"

CPipeSection::CPipeSection()
{
	m_insert[0] = 0.0;
	m_insert[1] = 0.0;
	m_insert[2] = 0.0;
	str_corss_blk = _T("道路横断面图");
	idarr_save.setLogicalLength(0);
}

CPipeSection::~CPipeSection()
{

}

void CPipeSection::Start()
{
	ads_name ssname;
	resbuf *rc = ads_buildlist(RTDXF0, _T("*polyline"), 0);
	ads_ssget(_T("X"), NULL, NULL, rc, ssname);
	long nLen = 0;
	ads_sslength(ssname, &nLen);
	AcDbObjectIdArray idarr_scdlzxx, idarr_sjdlzxx;
	for(int i = 0; i < nLen; i++)
	{
		ads_name ent;
		ads_ssname(ssname, i, ent);
		if(IsScDlzxx(ent))
			idarr_scdlzxx.append(aname(ent));
		else if(IsSjDlzxx(ent))
		{
			idarr_sjdlzxx.append(aname(ent));
		}
	}

	ads_ssfree(ssname);
	if(idarr_sjdlzxx.length() < 1)
	{
		acutPrintf(_T("\n 图上没有设计道路中心线！"));
		return;
	}
	if(idarr_scdlzxx.length() < 1)
	{
		acutPrintf(_T("\n 图上没有实测道路中心线！"));
		return;
	}
	if(idarr_scdlzxx.length() != idarr_sjdlzxx.length())
	{
		acutPrintf(_T("\n 实测和设计的条数不相同！"));
		return;
	}

	m_vstrBlock.clear();
	ads_point ptInsert = {0}; TCHAR dlmc[25] = {0};
	map<CString, AcDbObjectIdArray, CmpCstr> dlDhx;
	acedSetStatusBarProgressMeter(_T("\n正在处理实测道路中心线:"), 0, idarr_scdlzxx.length() - 1);
	for(int i = 0; i < idarr_scdlzxx.length(); i++)
	{
		acedSetStatusBarProgressMeterPos(i);
		AcDbObjectId id = idarr_scdlzxx.at(i);
		vector<AcDbObjectId> hdmIds;
		GetGroupEnts(id, hdmIds);
		ReadXdata(aname(id), _T("道路中心线名称"), 0, dlmc);
		str_dlmc.Format(_T("%s"), dlmc);
		dlDhx[str_dlmc] = AcDbObjectIdArray();
		for(int idx = 0; idx < (int)hdmIds.size(); ++idx)
		{
			AcDbObjectId hdm = hdmIds.at(idx);
			if(!IsScDMX(hdm)) continue;
			dlDhx[str_dlmc].append(hdm);
			AcGePoint2dArray nodes_new;
			GetInterNodes(aname(hdm), nodes_new);
			if(nodes_new.length() < 1)
			{
				acutPrintf(_T("\n %s没有找到与道路边线交点"), str_dlmc);
				continue;
			}
			id_line = DrawPolyline2D(nodes_new, 0.1, false, _T("180303"), 0.0, 0, _T("tk"));
			if(id_line == AcDbObjectId::kNull) continue;
			if(false == ReadXdata(aname(hdm), _T("LCZNOTE"), 0, dlmc)) continue;
			PrepareLine(id_line, CString(dlmc));
			InsertTk();
		}
	}
	acedRestoreStatusBar();
	CSaveBlocks sblk;
	XMJGAssist assist;
	CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\道路\\横断面.dwg");
	sblk.setPath(strPath);
	ads_point ptMin, ptMax;
	GetEntsExtent(idarr_save, ptMin, ptMax, 5.0);
	ZoomWindow(ptMin, ptMax);
	AcGePoint2d ptMid;
	getEntsMiddlePoint(idarr_save, ptMid);
	sblk.setPoint(apoint(ptMid));
	sblk.saveBlocks2(idarr_save, false);
	//EntDel(aname(id_line));
	//screen_helper.RestoreScreen();
}

void CPipeSection::StartEx()
{
	CBackScreen backScreen;
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	AcDbObjectIdArray idsDel; if(!SelectEntitys(idsDel, _T("X"))) return;
	SelectFilter sf1(RTDXF0, _T("*line")), sf2(8, _T("dmx"));
	AcDbObjectId hdm; if(!SelectEntity(hdm, sf1, sf2, _T(""))) return;
	AcGePoint2dArray nodes_new; GetInterNodes(aname(hdm), nodes_new);
	if(nodes_new.length() < 1)
	{
		acutPrintf(_T("\n %s没有找到与道路边线交点"), str_dlmc);
		return;
	}
	id_line = DrawPolyline2D(nodes_new, 0.1, false, _T("180303"), 0.0, 0, _T("tk"));
	if(id_line == AcDbObjectId::kNull) return; TCHAR dlmc[255] = {0};
	if(false == ReadXdata(aname(hdm), _T("LCZNOTE"), 0, dlmc)) return;
	EntsDel(idsDel);
	AcGePoint2dArray nodes;	GetPlList(aname(id_line), nodes);
	double dAngle = ads_angle(apoint(nodes.first()), apoint(nodes.last()));
	RotateEntity(aname(id_line), -dAngle, AcGePoint3d(nodes.first()[X], nodes.first()[Y], 0.0));

	ads_point ptMin, ptMax;
	GetEntExtent(aname(id_line), ptMin, ptMax);

	if(DrawInsert(m_insert, str_corss_blk, _T(""), 1.0, 0.0, 0, _T("TK")) == false)
	{
		acutPrintf(_T("\n%s图框模板加载失败！"), str_corss_blk);
		return;
	}
	ads_name ent; ads_entlast(ent);
	double dist = distOF2d(nodes.first(), nodes.last());
	double scale = dist / 297.0; SBreakBlock blk;
	blk.SetBlock(aname(ent)); blk.Explode();
	AcGePoint3d pt_sc;
	IProjectMDB mdb;
	MStr mstr = mdb.getProjectInfo();
	blk.replaceText(_T("#XMMC#"), mstr[_T("项目名称")], true);
	blk.replaceText(_T("#DLMC#"), dlmc, true);
	if(blk.findPointByHyperLink(_T("SCPT"), pt_sc) == false)
	{
		acutPrintf(_T("\n 没有找到实测图插入点！"));
		return;
	}
	double xmove = pt_sc.x - ptMin[X];
	double ymove = pt_sc.y - ptMin[Y];
	MoveEntity(aname(id_line), xmove, ymove);
	nodes_new.setLogicalLength(0);
	nodes_new.setPhysicalLength(0);
	GetPlList(aname(id_line), nodes_new);
	AddNote(id_line); EntDel(aname(id_line));
	for(int idx = 0; idx < nodes_new.length() - 1; ++idx)
	{
		AcDbObjectId id = drawline(apoint(nodes_new.at(idx)), apoint(nodes_new.at(idx + 1)), _T(""));
		AddXdata(aname(id), _T("HDM"), 0, _T("HDM"));
	}
	CSaveBlocks sblk;
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	AcDbObjectIdArray saveIds;
	SelectEntitys(saveIds, _T("X"));
	GetEntsExtent(saveIds, ptMin, ptMax, 5.0);
	AcGePoint2dArray node;
	node.append(apoint(ptMin)); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(apoint(ptMax)); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyx = DrawPolyline2D(node, 1.0, true, _T(""));
	setlayer(aname(dyx), _T("打印范围")); saveIds.append(dyx);
	XMJGAssist assist;
	CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\道路\\横断面.dwg");
	sblk.setPath(strPath);
	sblk.saveBlocks2(saveIds);
	openDwgCommand(strPath);
}

void CPipeSection::GetInterNodes(ads_name ent, AcGePoint2dArray &nodes_new)
{
	AcGePoint2dArray nodes;
	GetPlList(ent, nodes);

	double dAngle = ads_angle(apoint(nodes[0]), apoint(nodes[1]));

	ads_point ptMid; ptMid[Z] = 0.0;
	ptMid[X] = (nodes.first().x + nodes.last().x) / 2;
	ptMid[Y] = (nodes.first().y + nodes.last().y) / 2;
	ads_point ptS, ptE;
	ads_polar(ptMid, dAngle, 50.0, ptS);
	ads_polar(ptMid, dAngle + PI, 50.0, ptE);
	AcDbObjectId id_corss_line;
	id_corss_line = drawline(ptS, ptE, _T(""));
	AcGePoint2dArray node_corss; ads_name ssname;
	GetPlList(aname(id_corss_line), node_corss);
	ssFromNodes(node_corss, ssname, _T("F"), 0.5);
	long nLen = 0;
	ads_sslength(ssname, &nLen);

	AcDbCurve *pBound;

	//建立边界对象
	if(acdbOpenObject(pBound, id_corss_line, AcDb::kForRead) != Acad::eOk)
	{
		return;
	}

	AcGePoint3d pnt(0, 0, 0); AcGeVector3d normal(0, 0, 1); AcGePlane projPlane(pnt, normal);
	AcGePoint3dArray nodes3d;

	for(int i = 0; i < nLen; i++)
	{
		ads_name ent_dl;
		TCHAR sz_stdm[50] = {0};
		ads_ssname(ssname, i, ent_dl);
		GetStdm(ent_dl, sz_stdm);
		if(_tcscmp(sz_stdm, SCDLZXX) != 0 && _tcscmp(sz_stdm, SCDLBX) != 0 && _tcscmp(sz_stdm, DLFJX) != 0)
			continue;
		if(false == IsCurve(ent_dl)) continue;
		AcDbCurve *pCurve;
		AcGePoint3dArray tempPts, onlinePts, InterPts;
		AcGePoint3d ptMid, ptStart, ptEnd;

		AcDbObjectId id_dl;
		if(Acad::eOk != acdbGetObjectId(id_dl, ent_dl)) continue;

		bool bWPolyline = false;
		AcDb2dPolyline *p2dPoly;
		if(Acad::eOk == acdbOpenObject(p2dPoly, id_dl, AcDb::kForRead))
		{
			bWPolyline = true;
			p2dPoly->close();
		}

		//转换重量线
		ConvertToLWPoly(ent_dl);
		//转换样条曲线
		if(false == ConvertSplinePoly(ent_dl)) continue;
		//转换三维线
		ConvertThreeAxis(ent_dl);

		//求交点
		if(Acad::eOk != acdbGetObjectId(id_dl, ent_dl)) continue;

		if(Acad::eOk != acdbOpenObject(pCurve, id_dl, AcDb::kForWrite)) continue;
		Acad::ErrorStatus es = pCurve->intersectWith(pBound, AcDb::kOnBothOperands, projPlane, tempPts);
		CheckStartPoint(pCurve, tempPts); //交点与起始点重合的情况
		if(tempPts.length() <= 0)
		{
			if(true == DiluteEntity(ent_dl))
			{ //转圆和弧，稀释复合线,2005.1.18
				pCurve->close();
				acdbGetObjectId(id_dl, ent_dl);
				if(Acad::eOk != acdbOpenObject(pCurve, id_dl, AcDb::kForWrite)) continue;
				pBound->intersectWith(pCurve, AcDb::kOnBothOperands, projPlane, tempPts);
				if(tempPts.length() <= 0)
				{
					pCurve->erase();
					pCurve->close();
					continue;
				}
			}
			else
			{
				/////////////////////////////////////////20091230 马会超
				//原因 二次拟合的多段线求交点时,如果点正好在线上,可能会求不到,但CP选却能选中
				AcGePoint3d pt3d;
				pCurve->getStartPoint(pt3d);
				if(ifInsidePoly(pBound, pt3d) == 1)
				{
					pCurve->erase();
				}
				//////////////////////////////////////////20091230 马会超, 
				pCurve->close();
				continue;
			}
		}

		//检查交点是否在线上,20081209
		double mindist = 9999.9; AcGePoint3d minpt, ptTmp;
		for(int k = 0; k < tempPts.length(); k++)
		{
			AcGePoint3d pt3d = tempPts.at(k);
			double temp = distOF2d(apoint(ptS), apoint(pt3d));
			if(temp > mindist) continue;
			mindist = temp; minpt = pt3d;
		}

		if(Acad::eOk == pCurve->getClosestPointTo(minpt, ptTmp))
		{
			if(distOF3d(minpt, ptTmp) < 0.000001)
				onlinePts.append(minpt); //只有交点在曲线上才认为是合法的
		}
		RequeueInterPoints(pCurve, onlinePts, InterPts);
		nodes3d.append(InterPts);

		acutPrintf(_T("\n%d"), InterPts.length());
		pCurve->close();
	}
	pBound->close();
	ads_ssfree(ssname);
	EntDel(aname(id_corss_line));
	nodes3d.append(apoint(ptMid));
	AcGePoint3dArray nodes3d_sort;
	map<double, AcGePoint2d> map_dist;
	for(int i = 0; i < nodes3d.length(); i++)
	{
		double dist = distOF2d(AcGePoint2d(nodes3d.at(i).x, nodes3d.at(i).y), apoint(ptS));
		map_dist[dist] = AcGePoint2d(nodes3d.at(i).x, nodes3d.at(i).y);
	}

	nodes_new.setLogicalLength(0);
	map<double, AcGePoint2d>::iterator iter = map_dist.begin();
	for(; iter != map_dist.end(); iter++)
	{
		nodes_new.append(iter->second);
	}
}


void CPipeSection::InsertTk()
{
	ads_point ptInsert = {0};
	if(DrawInsert(ptInsert, str_corss_blk, _T(""), 1.0, 0.0, 0, _T("tk")) == false)
	{
		acutPrintf(_T("\n%s图框模板加载失败！"), str_corss_blk);
		return;
	}
	ads_name ent;
	ads_entlast(ent);
	AcDbObjectId m_idTk;
	acdbGetObjectId(m_idTk, ent);
	SBreakBlock blk;
	blk.SetBlock(m_idTk);
	blk.Explode();
	AcGePoint3d pt_sc;
	if(blk.findPointByHyperLink(_T("SCPT"), pt_sc) == false)
	{
		acutPrintf(_T("\n 没有找到实测图插入点！"));
		return;
	}
	blk.reblock(m_idTk);
	Replace(m_idTk);
	for(int i = 0; i < (int)m_vstrBlock.size(); i++)
	{
		CString csInFile = m_vstrBlock.at(i);
		csInFile = csInFile.Left(csInFile.GetLength() - 4);
		TCHAR szIns[_MAX_PATH] = _T("");
		_tcscpy(szIns, csInFile.Mid(csInFile.ReverseFind(_T('\\')) + 1));
		if(DrawInsert(apoint(pt_sc), szIns, _T(""), 1.0, 0.0, 0, _T("tk")) == false)
		{
			acutPrintf(_T("\n%s加载失败！"), m_vstrBlock.at(i));
			_tremove(m_vstrBlock.at(i));
			continue;
		}
		ads_entlast(ent);
		ads_command(RTSTR, _T("EXPLODE"), RTENAME, ent, RTNONE);
		ads_name ssName;
		ads_ssget(_T("P"), NULL, NULL, NULL, ssName);
		ads_command(RTSTR, _T("PURGE"), RTSTR, _T("B"), RTSTR, szIns, RTSTR, _T("N"), RTNONE);
		_tremove(m_vstrBlock.at(i));
		AcDbObjectIdArray idarrtp;
		SSToIdArr(ssName, idarrtp);
		idarr_save.append(idarrtp);
		AddNote(idarrtp.at(0));
	}

}

void CPipeSection::PrepareLine(const AcDbObjectId &id, const CString &name)
{
	// 	CBackScreen backScreen;
	// 	AcGePoint2dArray nodes;
	// 	GetPlList(aname(id), nodes);
	// 	double dAngle = ads_angle(apoint(nodes.first()), apoint(nodes.last()));
	// 	RotateEntity(aname(id), -dAngle, AcGePoint3d(nodes.first()[X], nodes.first()[Y], 0.0));
	// 	AcGePoint2dArray node;
	// 	ads_point ptMin, ptMax;
	// 	GetEntExtent(aname(id), ptMin, ptMax);
	// 
	// 	CSaveBlocks saveB;
	// 	CSt
	// 	saveB.setPoint(apoint(ptMin));
	// 	saveB.setPath(strPath);
	// 	AcDbObjectIdArray idarr;
	// 	idarr.append(id);
	// 	saveB.saveBlocks2(idarr);
	// 	m_vstrBlock.push_back(strPath);
	// 
	// 	if(DrawInsert(m_insert, str_corss_blk, _T(""), 1.0, 0.0, 0, _T("TK")) == false)
	// 	{
	// 		acutPrintf(_T("\n%s图框模板加载失败！"), str_corss_blk);
	// 		return;
	// 	}
	// 	ads_name ent; ads_entlast(ent);
	// 	double dist = distOF2d(nodes.first(), nodes.last());
	// 	double scale = dist / 297.0; SBreakBlock blk;
	// 	blk.SetBlock(aname(ent)); blk.Explode();
	// 	ScaleEntitys(blk.m_objIdArr, AcGePoint3d(0, 0, 0), scale);
}

void CPipeSection::AddNote(AcDbObjectId id)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	ads_name ent;
	for(int i = 0; i < nodes.length() - 1; i++)
	{
		AcGePoint2d pt1 = nodes.at(i);
		AcGePoint2d pt2 = nodes.at(i + 1);
		AlignedNote(apoint(pt1), apoint(pt2), 0.0, 5.0);
		ads_entlast(ent);
		idarr_save.append(aname(ent));
	}
	AlignedNote(apoint(nodes.first()), apoint(nodes.last()), 0.0, -10.0);
	ads_entlast(ent);
	idarr_save.append(aname(ent));
}

void CPipeSection::Replace(AcDbObjectId id_tk)
{
	SBreakBlock sblk;
	sblk.SetBlock(id_tk);
	sblk.Explode();

	IProjectMDB mdb;
	MStr mstr = mdb.getProjectInfo();

	sblk.replaceText(_T("#XMMC#"), mstr[_T("项目名称")]);
	sblk.replaceText(_T("#DLMC#"), str_dlmc);
	sblk.reblock(id_tk);
	idarr_save.append(id_tk);
}




CLoadVerFace::CLoadVerFace()
{
	str_ver_blk = _T("道路纵断面图");
	idarr_save.setLogicalLength(0);
	dDist_one_tk = 300.0; nNum = 0;
}
CLoadVerFace::~CLoadVerFace()
{

}


void CLoadVerFace::Start()
{
	CBackScreen backscreen;
	CBackUpScreenHelper screen_helper;
	screen_helper.BackScreen();
	ads_name ssname;
	resbuf *rc = ads_buildlist(RTDXF0, _T("*polyline"), 0);
	ads_ssget(_T("X"), NULL, NULL, rc, ssname);
	long nLen = 0;
	ads_sslength(ssname, &nLen);
	AcDbObjectIdArray idarr_scdlzxx, idarr_sjdlzxx;
	for(int i = 0; i < nLen; i++)
	{
		ads_name ent;
		ads_ssname(ssname, i, ent);
		if(IsScDlzxx(ent))
			idarr_scdlzxx.append(aname(ent));
		else if(IsSjDlzxx(ent))
			idarr_sjdlzxx.append(aname(ent));
	}
	ads_ssfree(ssname);
	if(idarr_sjdlzxx.length() < 1)
	{
		acutPrintf(_T("\n 图上没有设计道路中心线！"));
		return;
	}
	if(idarr_scdlzxx.length() < 1)
	{
		acutPrintf(_T("\n 图上没有实测道路中心线！"));
		return;
	}
	if(idarr_scdlzxx.length() != idarr_sjdlzxx.length())
	{
		acutPrintf(_T("\n 实测和设计的条数不相同！"));
		return;
	}

	m_vstrBlock.clear();

	ads_point ptInsert = {0};
	acedSetStatusBarProgressMeter(_T("\n正在处理实测道路中心线"), 0, idarr_scdlzxx.length() - 1);
	for(int i = 0; i < idarr_scdlzxx.length(); i++)
	{
		acedSetStatusBarProgressMeterPos(i);
		AcDbObjectId id = idarr_scdlzxx.at(i);
		ads_name ent;
		acdbGetAdsName(ent, id);

		TCHAR sz_dlmc[200] = {0};
		ReadXdata(ent, _T("道路中心线名称"), 0, sz_dlmc);
		str_dlmc = sz_dlmc;

		if(false == GetLczxx(ent, map_dist_gc)) return;


		for(int m = 0; m < idarr_sjdlzxx.length(); m++)
		{
			ads_name ent_sj;
			acdbGetAdsName(ent_sj, idarr_sjdlzxx.at(m));
			TCHAR sz_sjdlmc[200] = {0};
			ReadXdata(ent, _T("道路中心线名称"), 0, sz_sjdlmc);
			if(_tcsicmp(sz_sjdlmc, sz_dlmc) != 0) continue;
			GetLczxx(ent_sj, map_dist_gc_sj, 1);
		}

		CalTkNum();

		if(i != 0) ptInsert[Y] -= 400.0;

		ads_point ptT;
		ads_point_set(ptInsert, ptT);
		for(int m = 0; m < nNum; m++)
		{
			if(m != 0) ptT[X] += 600.0;
			InsertTk(ptT);
			AcGePoint2dArray nodes_sc, nodes_sj;
			DrawLinePt(map_dist_gc, m, nodes_sc, map_sc_earse);
			DrawLinePt(map_dist_gc_sj, m, nodes_sj, map_sj_earse);
			AcGePoint2dArray nodes;
			CombineNodes(nodes_sc, nodes_sj, nodes);
			DrawTable(nodes);
			ads_name ent_last;
			DrawPolyline2D(nodes_sc, 0.0, false, SCDLZXX, 0.0, 0, _T("SCDLX"));
			ads_entlast(ent_last);
			setcolor(ent_last, 1);
			idarr_save.append(aname(ent_last));
			DrawPolyline2D(nodes_sj, 0.0, false, SJDLZXX, 0.0, 0, _T("SJDLX"));
			ads_entlast(ent_last);
			setcolor(ent_last, 3);
			idarr_save.append(aname(ent_last));
			InsertData(nodes);
		}

	}
	acedRestoreStatusBar();
	ads_relrb(rc);

	CSaveBlocks sblk;
	XMJGAssist assist;
	CString strPath = assist.getExportFilePath();
	strPath += _T("\\CG\\道路\\纵断面.dwg");
	sblk.setPath(strPath);
	ads_point ptMin, ptMax;
	GetEntsExtent(idarr_save, ptMin, ptMax, 5.0);
	AcGePoint2dArray node;
	node.append(apoint(ptMin)); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(apoint(ptMax)); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyx = DrawPolyline2D(node, 1.0, true, _T(""));
	setlayer(aname(dyx), _T("打印范围")); idarr_save.append(dyx);
	ZoomWindow(ptMin, ptMax);
	AcGePoint2d ptMid;
	getEntsMiddlePoint(idarr_save, ptMid);
	sblk.setPoint(apoint(ptMid));
	sblk.saveBlocks2(idarr_save, false);
	screen_helper.RestoreScreen();
	openDwgCommand(strPath);
}

bool CLoadVerFace::GetLczxx(ads_name ent, map<CString, double, CmpCstr> &map_res, int nType)
{
	AcGePoint2dArray nodes;
	GetPlList(ent, nodes);
	ads_name ssname_lcz; //里程桩;
	ads_ssadd(NULL, NULL, ssname_lcz);
	resbuf *rc = ads_buildlist(RTDXF0, _T("*line"), 0);
	ads_ssget(_T("C"), apoint(nodes.first()), apoint(nodes.last()), rc, ssname_lcz);
	ads_relrb(rc);
	AcDbObjectIdArray idarr_lcz;
	if(nType == 0)
		SSToIdArrEx(ssname_lcz, idarr_lcz, _T("$992000$"));
	else
		SSToIdArrEx(ssname_lcz, idarr_lcz, _T("$992001$"));
	for(int j = 0; j < idarr_lcz.length(); j++)
	{
		AcDbObjectId id_lcz = idarr_lcz.at(j);
		double d_dist = 0.0, d_gc = 0.0;
		if(!ReadXdataDouble(aname(id_lcz), _T("LCZGC"), d_gc)) continue;
		if(!ReadXdataDouble(aname(id_lcz), _T("LCZDIST"), d_dist)) continue;
		CString strT;
		strT.Format(_T("%.2f"), d_dist);
		if(j == 0)
		{
			TCHAR sz_lcz_Pre[50] = {0};
			ReadXdata(aname(id_lcz), _T("LCZNOTE"), 0, sz_lcz_Pre);
			CString str = sz_lcz_Pre;
			int nIndex = str.Find(_T("+"));
			if(nIndex != -1)
				str_lcz_pre = str.Left(nIndex + 1);
		}
		map_res[strT] = d_gc;
	}
	return map_res.size() != 0;
}

void CLoadVerFace::CalTkNum()
{
	map<CString, double, CmpCstr>::iterator iter1, iter2;
	if(map_dist_gc.size() == 0) return;
	if(map_dist_gc_sj.size() == 0) return;
	iter1 = map_dist_gc.begin();
	iter2 = map_dist_gc_sj.begin();
	double d_qs = _tstof(iter1->first);

	map<CString, double, CmpCstr>::reverse_iterator iter3 = map_dist_gc.rbegin();
	map<CString, double, CmpCstr>::reverse_iterator iter4 = map_dist_gc_sj.rbegin();
	double d_zz = _wtof(iter3->first);
	double d_dist = d_zz - d_qs;

	double d_qs_sj = _wtof(iter2->first);
	double d_zz_sj = _wtof(iter4->first);
	double d_dist_sj = d_zz_sj - d_qs_sj;
	if(d_dist < d_dist_sj)
		d_dist = d_dist_sj;
	nNum = (int)(d_dist / 301) + 1;
}

//特定针对这个表的合并 ;
void CLoadVerFace::CombineNodes(AcGePoint2dArray nodes1, AcGePoint2dArray nodes2, AcGePoint2dArray &nodesSrc)
{
	nodesSrc.setLogicalLength(0);
	AcGePoint2dArray nodes;
	for(int i = 0; i < nodes1.length(); i++)
	{
		AcGePoint2d pt = nodes1.at(i);
		pt[Y] = ptTable[Y];
		nodes.append(pt);
	}

	for(int i = 0; i < nodes2.length(); i++)
	{
		AcGePoint2d pt = nodes2.at(i);
		pt[Y] = ptTable[Y];
		nodes.append(pt);
	}

	RemoveSameNodes(nodes);
	AcGePoint2d ptT = apoint(ptTable);
	map<CString, AcGePoint2d, CmpCstr> map_nodes;
	for(int i = 0; i < nodes.length(); i++)
	{
		AcGePoint2d pt = nodes.at(i);
		double dist = distOF2d(pt, ptT);
		CString strT;
		strT.Format(_T("%.2f"), dist);
		map_nodes[strT] = pt;
	}

	map<CString, AcGePoint2d, CmpCstr>::iterator iter;
	for(iter = map_nodes.begin(); iter != map_nodes.end(); iter++)
	{
		nodesSrc.append(iter->second);
	}
}

void CLoadVerFace::DrawLinePt(map<CString, double, CmpCstr> &map_dist, int nCurNum, AcGePoint2dArray &nodes, map<CString, double, CmpCstr> &map_earse)
{
	int nTotal = nCurNum + 1;
	nodes.setLogicalLength(0);
	map<CString, double, CmpCstr>::iterator iter;
	ads_point pt = {0};
	pt[X] = ptTable[X]; pt[Y] = ptTable[Y];
	int nCount_earse = 0;
	for(iter = map_dist.begin(); iter != map_dist.end(); iter++, nCount_earse++)
	{
		ads_point ptT;
		ptT[X] = pt[X] + (_wtof(iter->first) - dDist_one_tk * nCurNum) / 20.0 * 17.0;
		ptT[Y] = pt[Y] + (iter->second + 10) / 2.0 * 9.0;

		nodes.append(apoint(ptT));
		if(_wtof(iter->first) >= dDist_one_tk * nTotal)
		{
			break;
		}
	}

	int nCount = nCount_earse;
	if(nCurNum == nNum - 1)
		nCount_earse--;
	for(iter = map_dist.begin(); nCount_earse > 0; iter++, nCount_earse--)
	{
		map_earse[iter->first] = iter->second;
	}

	for(; nCount > 0; nCount--)
	{
		map_dist.erase(map_dist.begin());
	}
}

void CLoadVerFace::DrawTable(AcGePoint2dArray nodes)
{
	if(nodes.length() == 0) return;
	ads_point ptT;
	ptT[X] = ptTable[X]; ptT[Y] = ptTable[Y]; ptT[Z] = ptTable[Z];
	ads_point ptE;
	ptE[X] = nodes.last()[X];
	ptE[Y] = nodes.last()[Y];
	ptE[Y] = ptT[Y];
	double d_height = 9.0;

	for(int m = 0; m < 6; m++)
	{
		AcDbObjectId id;
		id = drawline(ptT, ptE, _T(""), 0, _T("tk"));
		idarr_save.append(id);
		if(m == 4)
			d_height = 16.2;
		//ads_polar(ptT, -PI / 2, d_height, ptT);
		//ads_polar(ptE, -PI / 2, d_height, ptE);
		ptE[Y] -= d_height;
		ptT[Y] -= d_height;
	}

	ads_point ptS;
	ptT[X] = ptTable[X]; ptT[Y] = ptTable[Y]; ptT[Z] = ptTable[Z];
	ads_point_set(ptT, ptS);
	for(int i = 0; i < nodes.length(); i++)
	{
		ptS[X] = nodes.at(i)[X];
		if(i == 0)
		{
			if(fabs(ptS[X] - ptTable[X]) > 1e-6)
			{
				ads_point_set(ptT, ptE);
				ptE[Y] -= 52.2;
				AcDbObjectId id;
				id = drawline(ptT, ptE, _T(""), 0, _T("tk"));
				idarr_save.append(id);
			}
		}
		ads_point_set(ptS, ptE);
		ptE[Y] -= 52.2;
		AcDbObjectId id;
		id = drawline(ptS, ptE, _T(""), 0, _T("tk"));
		ptS[Y] = ptT[Y];
		idarr_save.append(id);
	}
}

void CLoadVerFace::InsertTk(ads_point pt)
{

	if(DrawInsert(pt, str_ver_blk, _T(""), 1.0, 0.0, 0, _T("tk")) == false)
	{
		acutPrintf(_T("\n%s图框模板加载失败！"), str_ver_blk);
		return;
	}
	ads_name ent;
	ads_entlast(ent);
	AcDbObjectId m_idTk;
	acdbGetObjectId(m_idTk, ent);

	SBreakBlock blk;
	blk.SetBlock(m_idTk);
	blk.Explode();
	if(blk.findPointByHyperLink(_T("QSD"), ptTable) == false)
	{
		acutPrintf(_T("\n 没有找到实测图插入点！"));
		return;
	}

	blk.reblock(m_idTk);

	Replace(m_idTk);

}

void CLoadVerFace::InsertData(AcGePoint2dArray nodes)
{
	//求差值列表;
	map<CString, double, CmpCstr>::iterator iter, iter1;
	map<CString, double, CmpCstr> map_combine;
	for(iter = map_sc_earse.begin(); iter != map_sc_earse.end(); iter++)
	{
		map_combine[iter->first] = iter->second;
	}

	for(iter = map_sj_earse.begin(); iter != map_sj_earse.end(); iter++)
	{
		map_combine[iter->first] = iter->second;
	}

	vector<vector<CString> > vvstr_data;
	for(iter = map_combine.begin(); iter != map_combine.end(); iter++)
	{
		vector<CString> vstr;
		CString m_dist_sc = iter->first;
		vstr.push_back(str_lcz_pre + m_dist_sc);
		iter1 = map_sc_earse.find(m_dist_sc);
		if(iter1 != map_sc_earse.end())
		{
			CString strT;
			strT.Format(_T("%.2f"), iter1->second);
			vstr.push_back(strT);
		}
		else
		{
			vstr.push_back(_T(""));
		}

		iter1 = map_sj_earse.find(m_dist_sc);
		if(iter1 != map_sj_earse.end())
		{
			CString strT;
			strT.Format(_T("%.2f"), iter1->second);
			vstr.push_back(strT);
		}
		else
		{
			vstr.push_back(_T(""));
		}

		vvstr_data.push_back(vstr);
	}

	map_sc_earse.clear();
	map_sj_earse.clear();
	// 
	// 	if(vvstr_data.size() != nodes.length())
	// 	{
	// 		acutPrintf(_T("\n %d, %d,  2222"), vvstr_data.size(), nodes.length());
	// 		return;
	// 	}

	for(int i = 0; i < (int)vvstr_data.size(); i++)
	{
		vector<CString> vstr = vvstr_data.at(i);
		CString str_lcz = vstr.at(0);
		CString str_sc_gc = vstr.at(1);
		CString str_sj_gc = vstr.at(2);
		CString str_scG, str_sjG;
		if(!str_sc_gc.IsEmpty() && !str_sj_gc.IsEmpty())
		{
			double d_sc_gc = _wtof(str_sc_gc);
			double d_sj_gc = _wtof(str_sj_gc);
			if(d_sc_gc > d_sj_gc)
			{
				str_scG.Format(_T("%.2f"), d_sc_gc - d_sj_gc);
			}
			else
			{
				str_sjG.Format(_T("%.2f"), d_sj_gc - d_sc_gc);
			}
		}

		AcGePoint2d pt = nodes.at(i);
		AcGePoint2d ptT = pt;
		double dHeight = 1.8;
		double dFactor = 0.7;
		TCHAR sz_Font[50] = _T("STANDARD");
		ptT[X] -= dHeight;
		ptT[Y] -= 4.5;
		AcDbObjectId id;
		if(!str_scG.IsEmpty())
		{
			id = DrawText(AcGePoint3d(ptT[X], ptT[Y], 0), str_scG.GetBuffer(0), _T("MC"), _T(""), dHeight, PI / 2, dFactor);
			SetTextStyle(id, sz_Font);
			idarr_save.append(id);
		}

		ptT[Y] -= 9.0;
		if(!str_sjG.IsEmpty())
		{
			id = DrawText(AcGePoint3d(ptT[X], ptT[Y], 0), str_sjG.GetBuffer(0), _T("MC"), _T(""), dHeight, PI / 2, dFactor);
			SetTextStyle(id, sz_Font);
			idarr_save.append(id);
		}

		ptT[Y] -= 9.0;

		if(!str_sj_gc.IsEmpty())
		{
			id = DrawText(AcGePoint3d(ptT[X], ptT[Y], 0), str_sj_gc.GetBuffer(0), _T("MC"), _T(""), dHeight, PI / 2, dFactor);
			SetTextStyle(id, sz_Font);
			idarr_save.append(id);
		}

		ptT[Y] -= 9.0;
		if(!str_sc_gc.IsEmpty())
		{
			id = DrawText(AcGePoint3d(ptT[X], ptT[Y], 0), str_sc_gc.GetBuffer(0), _T("MC"), _T(""), dHeight, PI / 2, dFactor);
			SetTextStyle(id, sz_Font);
			idarr_save.append(id);
		}

		ptT[Y] -= 12.6;
		if(!str_lcz.IsEmpty())
		{
			id = DrawText(AcGePoint3d(ptT[X], ptT[Y], 0), str_lcz.GetBuffer(0), _T("MC"), _T(""), 2.0, PI / 2, 0.8);
			SetTextStyle(id, sz_Font);
			idarr_save.append(id);
		}

	}
}

void CLoadVerFace::Replace(AcDbObjectId id_tk)
{
	SBreakBlock sblk;
	sblk.SetBlock(id_tk);
	sblk.Explode();

	IProjectMDB mdb;
	MStr mstr = mdb.getProjectInfo();

	sblk.replaceText(_T("#XMMC#"), mstr[_T("项目名称")]);
	sblk.replaceText(_T("#DLMC#"), str_dlmc);

	sblk.reblock(id_tk);
	idarr_save.append(id_tk);
}