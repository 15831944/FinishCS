#include "stdafx.h"
#include "Tukuang.h"

void		CTukuang::findBatAndLastFrame(AcDbObjectId objFrame, AcDbObjectIdArray &objArrBat, AcDbObjectId &objLast)
{
	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 1, _T("*LINE*"), _T("图廓"));

	TCHAR szBatId[200] = {0};
	ReadXdata(aname(objFrame), _T("FRAME_BAT_ID"), STRING_TYPE, szBatId);

	SFilter filter;
	filter.filterObjectIdArray(objArr, _T("FRAME_BAT_ID"), szBatId);
	objArrBat.append(objArr);

	AcDbObjectIdArray objArrTemp = objArr;
	filter.filterObjectIdArray(objArrTemp, _T("FRAME_BAT_LAST"), _T("1"), INT_TYPE);

	if(objArrTemp.length()) objLast = objArrTemp.last();

}


bool		CTukuang::refreshBatFrame(AcDbObjectId objFrame, AcDbObjectIdArray objArrTK)
{
	vector<AcDbObjectId> vObjId;
	GetGroupEnts(objFrame, vObjId);
	AcDbObjectIdArray objArr;
	vectorToAcDbObjectIdArray(vObjId, objArr);

	SFilter filter;
	filter.filterObjectIdArray(objArr, XDATA_ENTITY_TYPE, _T("1"), INT_TYPE);
	EntsDel(objArr);


	ads_point ptmin, ptmax;
	GetEntExtent(aname(objFrame), ptmin, ptmax);
	SNodesOpe nodesO;
	nodesO.LB_RT_NodedArround(apoint(ptmin), apoint(ptmax));

	m_pt2dLBTK.x = ptmin[X]; m_pt2dLBTK.y = ptmin[Y];
	m_pt2dRTTK.x = ptmax[X]; m_pt2dRTTK.y = ptmax[Y];

	//先更新图例
	m_objInnerFrame = objFrame;
	insertLegen(ptmin, ptmax, ptmin);

	//再更新最后一个图的汇总表
	AcDbObjectIdArray objArrBat;
	AcDbObjectId objLast;
	findBatAndLastFrame(objFrame, objArrBat, objLast);

	AcDbObjectIdArray objArrAll;
	objArrAll.append(objArrBat);
	objArr.append(objLast);
	resetTolArea();

	for(int i = 0; i < objArrAll.length(); i ++)
	{
		GetEntExtent(aname(objArrAll.at(i)), ptmin, ptmax);
		nodesO.LB_RT_NodedArround(apoint(ptmin), apoint(ptmax));

		m_pt2dLBTK.x = ptmin[X]; m_pt2dLBTK.y = ptmin[Y];
		m_pt2dRTTK.x = ptmax[X]; m_pt2dRTTK.y = ptmax[Y];

		summaryTolArea();
	}


	vObjId.clear();
	objArr.setLogicalLength(0);
	GetGroupEnts(objLast, vObjId);
	vectorToAcDbObjectIdArray(vObjId, objArr);
	filter.filterObjectIdArray(objArr, XDATA_ENTITY_TYPE, _T("2"), INT_TYPE);
	EntsDel(objArr);

	GetEntExtent(aname(objLast), ptmin, ptmax);
	nodesO.LB_RT_NodedArround(apoint(ptmin), apoint(ptmax));

	m_objInnerFrame = objLast;
	insertSummarytTable(apoint(nodesO.m_pointsArrond.ptRB));

	

	return true;
}

bool		CTukuang::refreshSingleFrame(AcDbObjectId objFrame)
{
	vector<AcDbObjectId> vObjId;
	GetGroupEnts(objFrame, vObjId);
	AcDbObjectIdArray objArr;
	vectorToAcDbObjectIdArray(vObjId, objArr);
	
	SFilter filter;
	filter.filterObjectIdArray(objArr, XDATA_ENTITY_TYPE, _T("2"), INT_TYPE);
	EntsDel(objArr);

	vObjId.clear();
	objArr.setLogicalLength(0);
	GetGroupEnts(objFrame, vObjId);
	vectorToAcDbObjectIdArray(vObjId, objArr);
	filter.filterObjectIdArray(objArr, XDATA_ENTITY_TYPE, _T("1"), INT_TYPE);
	EntsDel(objArr);

	//1 左下角图例 2 右下角汇总表

	ads_point ptmin, ptmax;
	GetEntExtent(aname(objFrame), ptmin, ptmax);
	SNodesOpe nodesO;
	nodesO.LB_RT_NodedArround(apoint(ptmin), apoint(ptmax));

	m_pt2dLBTK.x = ptmin[X]; m_pt2dLBTK.y = ptmin[Y];
	m_pt2dRTTK.x = ptmax[X]; m_pt2dRTTK.y = ptmax[Y];

	m_objInnerFrame = objFrame;
	resetTolArea();
	sumAreaAndinsertTable(apoint(nodesO.m_pointsArrond.ptRB));

	insertLegen(ptmin, ptmax, ptmin);

	return true;
}

bool		CTukuang::refreshFrameData()
{
	AcDbObjectIdArray objArrTK;
	CSelect selectT;
	selectT.SelectObjects2(objArrTK, 2, _T("*LINE*"), _T("图廓"));

	for (int i = 0; i < objArrTK.length(); i ++)
	{
		AcDbObjectId objId = objArrTK.at(i);
		short sType = 0;
		ReadXdataShort(aname(objId), XDATA_FRAME_TYPE, sType);

		if(sType == 1)
			refreshSingleFrame(objId);
		else if(sType == 2)
			refreshBatFrame(objId, objArrTK);
	}

	acutPrintf(_T("\n 更新完成！"));

	return true;
}


bool  CTukuang::startIndoorFrame()
{
	double hh = 0.0;
	read1(hh); hh /= 1000.0;

	ads_point pt1, pt2, pt4;
	if(RTCAN == ads_getpoint(NULL, _T("\n 请指定左下角"), pt1)) return false;
	if(RTCAN == ads_getcorner(pt1, _T("\n 请指定右上角"), pt2)) return false;

	SNodesOpe nodesO;
	nodesO.modifyLB_RTPoint(pt1, pt2);

	CAcModuleResourceOverride myResources;

	CMatchFialogsEx matchDialog;
	matchDialog.SetConfigFile(_T("字段匹配表_入户调查平面图.ini"));
	if(matchDialog.DoModal() != IDOK) return false;

	AcGePoint2dArray nodes;
	nodesO.LB_RT_Nodes(apoint(pt1), apoint(pt2), nodes);
	AcDbObjectId objTemp = DrawPolyline2D(nodes, 0.0, true, _T(""), 0, 0, _T("TK"));

	pt4[X] = pt1[X];pt4[Y] = pt2[Y];pt4[Z] = 0.0;
	//AcDbObjectId objTittle = DrawInsert(pt4, _T("入户调查图_表头"), dXScale);
	AcDbObjectId objTittle = DrawInsert(pt4, _T("入户调查图_表头"));

	SBreakBlock blockB;
	blockB.SetBlock(objTittle);
	blockB.Explode();
	setlayers(blockB.m_objIdArr, _T("TK"));
	
	vector<CString> vStrKeys, vStrValues;
	AcDbObjectId objTable;
	if(blockB.findTable(objTable))
	{
		cstringArrToVector(matchDialog.m_strArrKey, vStrKeys);
		cstringArrToVector(matchDialog.m_strArrValue, vStrValues);
		ScaleEntity(aname(objTable), apoint(pt4), hh);
		AcDbTable *pTable = NULL;
		if(Acad::eOk == acdbOpenObject(pTable, objTable, AcDb::kForWrite))
		{
			//这里判断表头是否朝上，如果不是，则必为朝上，并重新设置位置
			pTable->setWidth(pt2[X] - pt1[X]);
			if(AcDb::kTtoB == pTable->flowDirection())
			{
				pTable->setFlowDirection(AcDb::kBtoT);
				pTable->setPosition(apoint(pt4));
			}
			pTable->close();
		}
		blockB.replaceTextInTable(objTable, vStrKeys, vStrValues);
	}

	ads_point ptName;
	AcDbExtents extents;
	GetEntExtent(aname(objTable), extents);
	ptName[X] = extents.minPoint().x; ptName[X] += (extents.maxPoint().x - extents.minPoint().x) / 2;
	ptName[Y] = extents.maxPoint().y;
	AcDbObjectId objTittleName = DrawInsert(ptName, _T("入户调查图_表名"), hh, hh);

	AcDbObjectId objButtom = DrawInsert(pt1, _T("入户调查图_表底"));

	CString strText;
	blockB.m_objIdArr.setLogicalLength(0);
	blockB.SetBlock(objButtom);
	blockB.Explode();
	setlayers(blockB.m_objIdArr, _T("TK"));
	if(blockB.findTable(objTable))
	{
		ScaleEntity(aname(objTable), apoint(pt1), hh);
		AcDbTable *pTable = NULL;
		if(Acad::eOk == acdbOpenObject(pTable, objTable, AcDb::kForWrite))
		{
			//这里判断表头是否朝上，如果不是，则必为朝上，并重新设置位置
			pTable->setWidth(pt2[X] - pt1[X]);
			if(AcDb::kBtoT == pTable->flowDirection())
			{
				pTable->setFlowDirection(AcDb::kTtoB);
				pTable->setPosition(apoint(pt4));
			}
			pTable->close();
		}

		cstringArrToVector(matchDialog.m_strArrKey, vStrKeys);
		cstringArrToVector(matchDialog.m_strArrValue, vStrValues);
		vStrKeys.push_back(_T("#SCALE#"));
		strText.Format(_T("1:%.lf"), hh * 1000);
		vStrValues.push_back(strText);
		blockB.replaceTextInTable(objTable, vStrKeys, vStrValues);
	}
	blockB.m_objIdArr.setLogicalLength(0);
	blockB.SetBlock(objTittleName);
	blockB.Explode();

	for(int i = 0; i < matchDialog.m_strArrKey.GetSize(); i ++)
	{
		strText = matchDialog.m_strArrKey.GetAt(i);
		if(strText == _T("#BH#"))
			AddXdata(aname(objTemp), _T("BH"), STRING_TYPE, matchDialog.m_strArrValue.GetAt(i));
		else if(strText == _T("#HZMC#"))
			AddXdata(aname(objTemp), _T("HZMC"), STRING_TYPE, matchDialog.m_strArrValue.GetAt(i));
		else if(strText == _T("#SFZH#"))
			AddXdata(aname(objTemp), _T("SFZH"), STRING_TYPE, matchDialog.m_strArrValue.GetAt(i));
		else if(strText == _T("#XMZL#"))
			AddXdata(aname(objTemp), _T("XMZL"), STRING_TYPE, matchDialog.m_strArrValue.GetAt(i));

		TCHAR szText[200] = {0};
		_tcscpy(szText, matchDialog.m_strArrKey.GetAt(i));
		blockB.replaceTextEx(szText, matchDialog.m_strArrValue.GetAt(i));
	}

	DrawInsert(pt2, _T("入户调查图_指北针"));

	return true;
}

void	CTukuang::getFloorCount_Top_Under()
{
	SNodesOpe nodesO;
	AcGePoint2dArray nodes;
	nodesO.LB_RT_Nodes(m_pt2dLBTK, m_pt2dRTTK, nodes);

	AcDbObjectIdArray objArr;
	ssFromNodes(objArr, nodes, 2, 0.0, _T("*LINE*"));

	vector<int> vTop, vUnder;
	for(int i = 0; i < objArr.length(); i ++)
	{
		short sCBJ = 0;
		short sTop = 0, sUnder = 0;
		ReadXdataShort(aname(objArr.at(i)), _T("CBJ"), sCBJ);
		ReadXdataShort(aname(objArr.at(i)), _T("QSLC"), sUnder);
		ReadXdataShort(aname(objArr.at(i)), _T("ZZLC"), sTop);
		if(sTop == 0 || sUnder == 0) continue;

		if(sCBJ == 0)
		{
			//只统计地上层数
			if(sTop < 0 || sUnder < 0) continue;

			vTop.push_back(sTop);
			vTop.push_back(sUnder);
		}
		else
		{
			vUnder.push_back(sTop);
			vUnder.push_back(sUnder);
		}

	}

	if(vTop.size() >1)
	{
		std::sort(vTop.begin(), vTop.end());
		vector<int>::iterator it = vTop.begin();
		vector<int>::iterator it2 = vTop.end();
		it2 --;
		m_nFloorCountTop = (*it2) - (*it);
		m_nFloorCountTop ++;
		m_nFoorCountTop_Mul += m_nFloorCountTop;
	}
	else
	{
		m_nFloorCountTop = 0;
	}
	if(vUnder.size() >1)
	{
		std::sort(vUnder.begin(), vUnder.end());
		vector<int>::iterator it = vUnder.begin();
		vector<int>::iterator it2 = vUnder.end();
		it2 --;
		m_nFloorCountUnder = (*it2)  - (*it);
		m_nFloorCountUnder ++;
		m_nFloorCountUnder_Mul += m_nFloorCountUnder;
	}
	else
	{
		m_nFloorCountUnder = 0;
	}
	

}

void		CTukuang::resetTolArea()
{
	m_floorInfo.dAreaUnder_Private = 0.0; 
	m_floorInfo.dAreaUnder_Public = 0.0;
	m_floorInfo.dAreaUp_Private = 0.0;
	m_floorInfo.dAreaUp_Public = 0.0;
}


bool	CTukuang::summaryTolArea()
{
	SNodesOpe nodesO;
	AcGePoint2dArray nodes;
	nodesO.LB_RT_Nodes(m_pt2dLBTK, m_pt2dRTTK, nodes);

	AcDbObjectIdArray objArr;
	ssFromNodes(objArr, nodes, 2, 0.0, _T("*TEXT*"));

	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR sz[100] = {0};
		READMulXData(objArr.at(i), _T("FloorSummary"), sz, DOUBLE_TYPE, 0);
		m_floorInfo.dAreaUnder_Private += _tstof(sz); 
		_tcscpy(sz, _T(""));
		READMulXData(objArr.at(i), _T("FloorSummary"), sz, DOUBLE_TYPE, 1);
		m_floorInfo.dAreaUnder_Public += _tstof(sz);
		_tcscpy(sz, _T(""));
		READMulXData(objArr.at(i), _T("FloorSummary"), sz, DOUBLE_TYPE, 2);
		m_floorInfo.dAreaUp_Private += _tstof(sz);
		_tcscpy(sz, _T(""));
		READMulXData(objArr.at(i), _T("FloorSummary"), sz, DOUBLE_TYPE, 3);
		m_floorInfo.dAreaUp_Public += _tstof(sz);
	}


	return true;
}

bool		CTukuang::insertSummarytTable(ads_point pt)
{

	TCHAR szTempName[200] = {0};
	// 	if(m_bMulty == false)
	// 	{
	// 		if(floorInfo.dAreaUp_Public >0 || floorInfo.dAreaUnder_Public > 0)
	// 			_tcscpy(szTempName, _T("分层图_图表5"));			//地上，下，公，私
	// 		else if(floorInfo.dAreaUnder_Private > 0 && floorInfo.dAreaUp_Private > 0)
	// 			_tcscpy(szTempName, _T("分层图_图表4"));			//地上，下
	// 		else if(floorInfo.dAreaUnder_Private > 0 && floorInfo.dAreaUp_Private <= 0)
	// 			_tcscpy(szTempName, _T("分层图_图表3"));			//地下
	// 		else
	// 			_tcscpy(szTempName, _T("分层图_图表2"));			//地上
	// 	}
	// 	else
	// 	{
	if(m_bLast == true)
	{
		if(m_floorInfo.dAreaUp_Public >0 || m_floorInfo.dAreaUnder_Public > 0)
			_tcscpy(szTempName, _T("分层图_图表5"));			//地上，下，公，私
		else if(m_floorInfo.dAreaUnder_Private > 0 && m_floorInfo.dAreaUp_Private > 0)
			_tcscpy(szTempName, _T("分层图_图表4"));			//地上，下
		else if(m_floorInfo.dAreaUnder_Private > 0 && m_floorInfo.dAreaUp_Private <= 0)
			_tcscpy(szTempName, _T("分层图_图表3"));			//地下
		else
			_tcscpy(szTempName, _T("分层图_图表2"));			//地上
	}
	else
		_tcscpy(szTempName, _T("分层图_图表_批量"));

	//	}


	m_objSummaryTable = DrawInsert(pt, szTempName, m_dhhFrame, m_dhhFrame);

	GetEntExtent(aname(m_objSummaryTable), m_ptminTable, m_ptmaxTable);

	m_dTol = m_floorInfo.dAreaUp_Public + m_floorInfo.dAreaUnder_Public + m_floorInfo.dAreaUnder_Private + m_floorInfo.dAreaUp_Private;
	m_dTolTop = m_floorInfo.dAreaUp_Public + m_floorInfo.dAreaUp_Private;
	m_dTopUnder = m_floorInfo.dAreaUnder_Public + m_floorInfo.dAreaUnder_Private;

	SBreakBlock blockB;
	blockB.SetBlock(m_objSummaryTable);
	blockB.Explode();

	CString	strText, strText2;
	if(m_bLast == true)
	{
		strText.Format(_T("%.2lf"), m_dTol);
		blockB.replaceText(_T("#TOTALAREA#"), strText);
		strText.Format(_T("%.2lf"), m_floorInfo.dAreaUp_Private);
		blockB.replaceText(_T("#AREA_UP_LIVE#"), strText);
		strText.Format(_T("%.2lf"), m_floorInfo.dAreaUp_Public);
		blockB.replaceText(_T("#AREA_UP_PUBLIC#"), strText);
		strText.Format(_T("%.2lf"), m_floorInfo.dAreaUnder_Private);
		blockB.replaceText(_T("#AREA_UNDER_LIVE#"), strText);
		strText.Format(_T("%.2lf"), m_floorInfo.dAreaUnder_Public);
		blockB.replaceText(_T("#AREA_UNDER_PUBLIC#"), strText);
		strText.Format(_T("%.2lf"), m_dTolTop);
		blockB.replaceText(_T("#AREA_UP#"), strText);
		strText.Format(_T("%.2lf"), m_dTopUnder);
		blockB.replaceText(_T("#AREA_UNDER#"), strText);
	}


	CXRecordOperate xrecordO;
	for(int i = 0; i < g_nMaxInfo; i ++)
	{
		if(0 == _tcscmp(g_AttrInfo[i].table, _T("XMXX")))
		{
			for(int j = 0; j < g_AttrInfo[i].AttrArr.GetSize(); j ++)
			{
				strText.Format(_T("#%s#"), g_AttrInfo[i].AttrArr.GetAt(j).name);

				xrecordO.ReadStringRecord(DICT_PROJECTINFO, g_AttrInfo[i].AttrArr.GetAt(j).name, strText2);
				blockB.replaceText(strText, strText2);

			}
			break;
		}
	}

	//blockB.replaceScale();
	strText.Format(_T("1:%.lf"), m_dhhFrame * 1000.0);
	blockB.replaceText(_T("#SCALE#"), strText, true);

	AcDbObjectId objIdKey;
	for(int i = 0; i < blockB.m_objIdArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(blockB.m_objIdArr.at(i)), szStdm);
		if(0 == _tcscmp(szStdm, CASSCODE_480410))
		{
			ReadXdata(aname(blockB.m_objIdArr.at(i)), XDATA_TXTZ, STRING_TYPE, szStdm);
			if(0 == _tcscmp(szStdm, _T("总建筑面积")))
			{
				objIdKey = blockB.m_objIdArr.at(i);
				break;
			}
		}
	}

	if(objIdKey != AcDbObjectId::kNull)
	{
		if(m_bLast)
			saveBuildingInfo(objIdKey);
	}
	else
	{
		acutPrintf(_T("\n 找不到模板关键字！写入汇总面积失败: 总建筑面积"));
	}

	ads_name ssname;
	IdArrToSS(blockB.m_objIdArr, ssname);
	AddXdata_bat(ssname, XDATA_ENTITY_TYPE, INT_TYPE, _T("2"));
	ads_ssfree(ssname);
	AddEntsToGroup(m_objInnerFrame, blockB.m_objIdArr);

	return true;
}



bool CTukuang::sumAreaAndinsertTable(ads_point pt)
{
	summaryTolArea();

	insertSummarytTable(pt);


	return true;
}

bool	CTukuang::combineElevationPoints()
{
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();

	AcGePoint2dArray nodes;
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(m_pt2dLBTK, m_pt2dRTTK, nodes);

	AcDbObjectIdArray objArr;
	ssFromNodes(objArr, nodes, 2, 0.0, NULL, _T("底图_N"));
	ssFromNodes(objArr, nodes, 2, 0.0, NULL, _T("底图_W"));

	ads_name ssElevPoint;
	ads_ssadd(NULL, NULL, ssElevPoint);
	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(objArr.at(i)), szStdm);

		if(0 == _tcscmp(szStdm, _T("202101")) || 0 == _tcscmp(szStdm, _T("202111")))
		{
			ads_name ent;
			if(Acad::eOk == acdbGetAdsName(ent, objArr.at(i)))
				ads_ssadd(ent, ssElevPoint, ssElevPoint);
		}
	}
	long len = 0;
	ads_sslength(ssElevPoint, &len);
	if(len > 0)
		ads_command(RTSTR, _T("RESUMEGCD"), RTPICKS, ssElevPoint, RTSTR, _T(""), RTREAL, 100.0, /*RTSTR, _T(""),*/ RTNONE);
	ads_ssfree(ssElevPoint);

//	AfxMessageBox("111");

	objArr.setLogicalLength(0);
	ssFromNodes(objArr, nodes, 2, 0.0, NULL, _T("底图_N"));
	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(objArr.at(i)), szStdm);
		if(CheckGcdCode(szStdm,false))
		{
			SBlockOpe blockO;
			blockO.ChangeBlockAttributeColor(objArr.at(i), 251);
		}
	}

	objArr.setLogicalLength(0);
	ssFromNodes(objArr, nodes, 2, 0.0, NULL, _T("底图_W"));
	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(objArr.at(i)), szStdm);
		if(CheckGcdCode(szStdm,false))
		{
			SBlockOpe blockO;
			blockO.ChangeBlockAttributeColor(objArr.at(i), 254);
		}
	}

	return true;
}


bool	CTukuang::trimLand()
{
	
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();

	STrimPoly trimP;
	trimP.TrimPoly_Simple(aname(m_objIdDiv));

	AcGePoint2dArray nodes;
	GetPlList(aname(m_objIdDiv), nodes);

	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();

	//根据节点位置判断哪些实体放在DXT_N层，哪些实体放在DXT_W层
	AcDbObjectIdArray objArrIn, objArrOut;
	ads_name ssname;
	ssFromLayersB(ssname,  _T("DXT"));
	long len = 0;
	ads_sslength(ssname, &len);
	for(long l = 0; l < len; l ++)
	{
		ads_name ent;
		ads_ssname(ssname, l, ent);
		TCHAR szType[200] = {0};
		GetEntityType(ent, szType);
		CString strType = szType;
		ads_point ptPos;
		if(strType == _T("LINE"))
		{
			//GetEntPosition(ent, ptPos);
			AcGePoint2dArray nodesTemp;
			GetPlList(ent, nodesTemp);
			GetNodesMiddlePoint(nodesTemp, ptPos);
		}
		else
		{
			if(strType.Find(_T("LINE")) >= 0)	//如果实体是线形(除直线外)，则判断线的中间点位置
			{
				AcGePoint2d ptMid;
				GetPointOnPolyLine(0.5, ptMid, aname(ent));
				ptPos[X] = ptMid.x; ptPos[Y] = ptMid.y; ptPos[Z] = 0.0;
			}
			else
			{
				//GetEntPosition(ent, ptPos);
				AcGePoint3d pt3dPos;
				getEntPositionEx(aname(ent), pt3dPos);
				ptPos[X] = pt3dPos.x; ptPos[Y] = pt3dPos.y; ptPos[Z] = 0.0;
			}
		}

		int nPos = PointInPolygon(nodes, apoint(ptPos));
		if(nPos == 1)
			objArrOut.append(aname(ent));
		else
			objArrIn.append(aname(ent));
	}
	ads_ssfree(ssname);

	setlayers(objArrIn, _T("底图_N"));
	//setColorsEx(objArrIn, 251);
	for(int i = 0; i < objArrIn.length(); i ++)
	{
		TCHAR szType[100] = {0};
		GetEntityType(aname(objArrIn.at(i)), szType);
		if(0 == _tcsicmp(szType, _T("INSERT")))
		{
// 			if(isElevationPoint(objArrIn.at(i)) == true) 
// 			{
// 				SBlockOpe blockO;
// 				blockO.ChangeBlockAttributeColor(objArrIn.at(i), 251);
// 			}
			SBlockOpe blockO;
			blockO.ChangeBlockRefenceColor(objArrIn.at(i), 251);
			setColorsEx2(objArrIn.at(i), 1);
		}
		else
		{
			setColorsEx2(objArrIn.at(i), 1);
		}
	}
	
	setlayers(objArrOut, _T("底图_W"));
	for(int i = 0; i < objArrOut.length(); i ++)
	{
		TCHAR szType[100] = {0};
		GetEntityType(aname(objArrOut.at(i)), szType);
		if(0 == _tcsicmp(szType, _T("INSERT")))
		{
// 			if(isElevationPoint(objArrOut.at(i)) == true) 
// 			{
// 				SBlockOpe blockO;
// 				blockO.ChangeBlockAttributeColor(objArrOut.at(i), 254);
// 			}
			SBlockOpe blockO;
			blockO.ChangeBlockRefenceColor(objArrOut.at(i), 254);
			setColorsEx2(objArrOut.at(i), 1);
		}
		else
		{
			setColorsEx2(objArrOut.at(i), 1);
		}
	}
	
	//setColorsEx(objArrOut, 254);


	return true;
}


bool	CTukuang::checkDivLine(AcDbObjectIdArray objArrLine)
{
	//如果是直接用图框切割的话则不用判断
	if(m_bDiveByFrame)
	{
		m_objIdDiv = objArrLine.at(0);
		return true;
	}

	bool bFind = false;
	AcGePoint2dArray nodesFrame;
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(m_pt2dLBTK, m_pt2dRTTK, nodesFrame);
	nodesFrame.append(nodesFrame.first());
	//发现有一条线不是闭合的，则退出
	for(int i = 0; i < objArrLine.length(); i ++)
	{
		AcGePoint2dArray nodes, nodesRes;
		AcGeDoubleArray doubleArr;
		GetPlList(aname(objArrLine.at(i)), nodes);
		if(nodes.length() >= 2)
		{
			if(CEntOper::ifPolyClosed(objArrLine.at(i)))
				nodes.append(nodes.first());
		}
		polylineIntersection(nodesFrame, nodes, nodesRes, doubleArr);
		if(nodesRes.length() > 0)
		{
			bFind = true;
			break;
		}
		
	}
	if(bFind == false)
	{
		m_objIdDiv = objArrLine.at(0);
		return true;
	}

	AfxMessageBox(_T("发现多个切割面，请选择一个合适的面来出图！"));

	vector<AcGePoint2dArray> vNodes;
	vNodes.push_back(nodesFrame);
	for(int i = 0; i < objArrLine.length(); i ++)
	{
		AcGePoint2dArray nodes;
		GetPlList(aname(objArrLine.at(i)), nodes);
		if(nodes.length() >= 2)
		{
			if(CEntOper::ifPolyClosed(objArrLine.at(i)))
				nodes.append(nodes.first());
		}
		vNodes.push_back(nodes);
	}

	CArray<AcGePoint2dArray, AcGePoint2dArray> nodesArr;
	CArray<AcGeDoubleArray, AcGeDoubleArray> doubleArr;
	nodesO.getBorder(vNodes, nodesArr, doubleArr);
	if(nodesArr.GetSize() == 0)
	{
		acutPrintf(_T("\n 构面失败！"));
		return false;
	}

	CEntitysListDlg listDlg;
	listDlg.m_sType = 2;
	for(int i = 0; i < nodesArr.GetSize(); i ++)
	{
		AcDbObjectId objId = DrawPolyline2D(nodesArr.GetAt(i), 0.0, true, _T(""), 0.0, 0, _T("TEMP"));
		CLayers layer;
		layer.drawOrder(objId);
		listDlg.m_objIdArr.append(objId);
	}
	if(listDlg.DoModal() != IDOK) return false;

	if(listDlg.m_objIdArrSelected.length() == 0) return false;

	m_objIdDiv = listDlg.m_objIdArrSelected.first();

	return true;
}

bool	CTukuang::startTotal_Floor_Plan()
{
	checkProjInfo();

	read1(m_dhh); m_dhh /= 1000.0;

	ads_point pt1, pt2, pt3, pt4;//左下，右上，左上，右下
	if(RTCAN == ads_getpoint(NULL, _T("\n 请指定左下角"), pt1)) return false;
	if(RTCAN == ads_getcorner(pt1, _T("\n 请指定右上角"), pt2)) return false;

	SNodesOpe nodesO;
	nodesO.modifyLB_RTPoint(pt1, pt2);

	pt3[X] = pt1[X]; pt3[Y] = pt2[Y]; pt3[Z] = 0.0;
	pt4[X] = pt2[X]; pt4[Y] = pt1[Y]; pt4[Z] = 0.0;
	m_pt2dLBTK = apoint(pt1);
	m_pt2dRTTK = apoint(pt2);
	CTukuang_Common tukuanC;
	tukuanC.m_dhh = m_dhh;
	tukuanC.coordFit(m_pt2dLBTK, 2);
	tukuanC.coordFit(m_pt2dRTTK, 2);
	pt1[X] = m_pt2dLBTK.x; pt1[Y] = m_pt2dLBTK.y;
	pt2[X] = m_pt2dRTTK.x; pt2[Y] = m_pt2dRTTK.y;

	AcGePoint2dArray nodes;
	nodesO.LB_RT_Nodes(apoint(pt1), apoint(pt2), nodes);
	AcDbObjectIdArray objArr;
	ssFromNodes(objArr, nodes, 2, 0.0, _T("*LINE*"), _T("规划控制线"));
	
	CEntitysListDlg listDlg;
	if(objArr.length() == 0)
	{
		AfxMessageBox(_T("没有规范线，改用图框切割底图！"));
		AcDbObjectId objTempFrame = DrawPolyline2D(nodes, 0.0, true, _T(""));
		listDlg.m_objIdArrSelected.append(objTempFrame);
		m_bDiveByFrame = true;
	}
	else if(objArr.length() == 1)
	{
		listDlg.m_objIdArrSelected.append(objArr);
	}
	else
	{
		AfxMessageBox(_T("图上存在多条规划红线，请选择其中一条作为出图规划线！"));

		listDlg.m_sType = 1;
		listDlg.m_objIdArr.append(objArr);
		if(IDOK != listDlg.DoModal()) return false;
		if(listDlg.m_objIdArrSelected.length() == 0)
		{
			AfxMessageBox(_T("请选择一条切割范围线！"));
			return false;
		}
	}

	if(checkDivLine(listDlg.m_objIdArrSelected) == false) return false;

	if(ads_getint(_T("\n 请选择图例生成方式：<1> 自动生成 <2> 固定图例 <1>"), &m_sAutoLegend) == RTCAN) return false;
	if(m_sAutoLegend != 1 && m_sAutoLegend != 2) m_sAutoLegend = 1;

	acedSetStatusBarProgressMeter(_T("图框切割中..."),0, 10);
	acedSetStatusBarProgressMeterPos(0);

	AcDbObjectId objTemp = DrawPolyline2D(nodes, 0.0, true, _T(""));
	TrimPoly(aname(objTemp), true, false, true);

	acedSetStatusBarProgressMeter(_T("插入模板中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(1);

	double dScaleX = (pt2[X] - pt1[X]) / 1000;
	double dScaleY = (pt2[Y] - pt1[Y]) / 1000;
	AcDbObjectId objFrame = DrawInsert(pt1, _T("总平面竣工图_模板"), dScaleX, dScaleY);

	SBreakBlock blockB;
	blockB.SetBlock(objFrame);
	blockB.Explode();

	AcGePoint3d ptMid = GetMiddlePoint(apoint(pt3), apoint(pt2));
	AcDbObjectId objTittle = DrawInsert(apoint(ptMid), _T("总平面竣工图_标题_模板"), m_dhh, m_dhh);
	blockB.SetBlock(objTittle);
	blockB.Explode();

	acedSetStatusBarProgressMeter(_T("填写项目信息中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(2);

	AcGePoint3d  ptLB, ptRT, ptRB, ptWLB, ptWRT;
	blockB.findPointByHyperLink(_T("图框左下角"), ptLB);
	blockB.findPointByHyperLink(_T("图框右上角"), ptRT);
	blockB.findPointByHyperLink(_T("图框右下角"), ptRB);
	m_pt2dLBTK = apoint(ptLB);
	m_pt2dRTTK = apoint(ptRT);

	CXRecordOperate xrecordO;
	CArray<ATTRFIELD,ATTRFIELD> *AttrArr = NULL;
	CString strText;
	if(findAttriTable(_T("XMXX"), AttrArr))
	{
		for(int j = 0; j < AttrArr->GetSize(); j ++)
		{
			TCHAR szText[100] = {0};
			_stprintf(szText, _T("#%s#"), AttrArr->GetAt(j).name);
			xrecordO.ReadStringRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, strText);
			blockB.replaceTextEx(szText, strText);
		}
	}

	acedSetStatusBarProgressMeter(_T("绘制坐标注记中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(3);

	tukuanC.drawCrossCoord(m_pt2dLBTK, m_pt2dRTTK);

	acedSetStatusBarProgressMeter(_T("设置内外地形图中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(4);

	trimLand();

	acedSetStatusBarProgressMeter(_T("合并高程点注记中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(5);

	combineElevationPoints();

	tukuanC.drawCross_Auto(m_pt2dLBTK, m_pt2dRTTK);

	acedSetStatusBarProgressMeter(_T("插入图例中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(5);

	insertLegend_Total_Floor_Plan(pt1, pt2, apoint(ptLB));

	acedSetStatusBarProgressMeter(_T("插入表格中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(7);

 	insertTable(ptRB);

 	tukuanC.drawFrameOut(m_pt2dLBTK, m_pt2dRTTK);
	
	acedSetStatusBarProgressMeter(_T("设置实体叠盖顺序中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(8);

	ads_name ssname;
	ssFromLayers(ssname, _T("TK"));
	objArr.setLogicalLength(0);
	SSToIdArr(ssname, objArr);
	setcolors(objArr, 7);

	acedSetStatusBarProgressMeter(_T("删除无用实体中..."), 0, 10);
	acedSetStatusBarProgressMeterPos(9);

	ssFromLayers(ssname, _T("TEMP"));
	EntsDel(ssname);
	ads_ssfree(ssname);

	acedRestoreStatusBar();

	ads_printf(_T("\n\n 输出完成！"));

	return true;
}

bool CTukuang::explodeTableAndDeleteLine(AcDbObjectIdArray objArr)
{
	for(int i = 0; i < objArr.length(); i ++)
	{
		AcDbObjectId objId = objArr.at(i);
		TCHAR szType[200] = {0};
		GetEntityType(aname(objId), szType);
		if(0 != _tcsicmp(szType, _T("ACAD_TABLE"))) continue;

		SBreakBlock breakB;
		breakB.SetBlock(objId);
		breakB.Explode();
		for(int j = breakB.m_objIdArr.length() - 1; j >= 0 ; j --)
		{
			objId = breakB.m_objIdArr.at(j);
			GetEntityType(aname(objId), szType);
			if(0 == _tcsicmp(szType, _T("LINE")))
			{
				if(getcolor(aname(objId)) == 8)
					EntDel(aname(objId));
			}
		}
	}
	return true;
}


bool CTukuang::insertLegend_Total_Floor_Plan(ads_point pt1, ads_point pt2, ads_point ptInsert)
{
	if(m_sAutoLegend == 1)
	{
		AcGePoint2dArray nodes;
		SNodesOpe nodesO;
		nodesO.LB_RT_Nodes(apoint(pt1), apoint(pt2), nodes);
		AcDbObjectIdArray objArr;
		ssFromNodes(objArr, nodes, 2);
		SLegendOpe lengOpe;
		lengOpe.m_bExplodeBlocks = 1;
		if(lengOpe.readLegendConfig(_T("竣工总平面图图例_配置.XML")))
		{
			lengOpe.setHH(1.0);
			lengOpe.setEntitys(objArr);
			lengOpe.insertLegendTable(apoint(ptInsert));
			for(int j = 0; j < lengOpe.m_objArrBlocks.length(); j ++)
			{
				AcDbBlockReference *pBlock;
				if(Acad::eOk == acdbOpenObject(pBlock, lengOpe.m_objArrBlocks.at(j), AcDb::kForWrite))
				{
					pBlock->explodeToOwnerSpace();
					pBlock->erase();
					pBlock->close();
				}
			}
			//explodeTableAndDeleteLine(lengOpe.m_objArrLegend);
		}
	}
	else
	{
		AcDbObjectId objLegend = DrawInsert(ptInsert, _T("总平面竣工图_固定图例"));
		if(objLegend == AcDbObjectId::kNull)
		{
			acutPrintf(_T("\n 插入模板失败：总平面竣工图_固定图例.dwg"));
			return false;
		}
		//ScaleEntity(aname(objLegend), apoint(ptInsert), m_dhh);
	}


	return true;
}

bool	CTukuang::scaleTextsInFrame()
{
	AcGePoint2dArray nodes;
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(m_pt2dLBTK, m_pt2dRTTK, nodes);
	AcDbObjectIdArray objArr;
	ssFromNodes(objArr, nodes, 2, 0.0);

	//缩放注记
	AcDbObjectIdArray objArrDimen;
	SFilter filter;
	filter.filterObjectIdArray_ByType(objArr, objArrDimen, _T("DIMENSION"));
	for(int i = 0; i < objArrDimen.length(); i ++)
	{
		SDimensionOpe dimenOpe;
		dimenOpe.setTextHeiglt(objArrDimen.at(i), 2.0 * m_dhhFrame);
	}
	
	
	filter.filterObjectIdArray(objArr, _T("SOUTH"), _T("480410"));
	set<AcDbObjectId> sGroupId;
	for(int i = 0; i < objArr.length(); i ++)
	{
		AcDbObjectId objGroup;
		GetEntGroupEx(objArr.at(i), objGroup);
		sGroupId.insert(objGroup);
	}

	for(set<AcDbObjectId>::iterator it = sGroupId.begin(); it != sGroupId.end(); it ++)
	{
		AcDbObjectId objId = *it;
		AcDbObjectIdArray objGroups, objArrText;
		getEntsByGroupId(objId, objGroups);

		filter.filterObjectIdArray_ByType(objGroups, objArrText, _T("TEXT"));

		if(objArrText.length() == 0) continue;

		ads_point ptmin, ptmax;
		GetEntsExtent(objArrText, ptmin, ptmax);
		nodesO.LB_RT_NodedArround(apoint(ptmin), apoint(ptmax));

		double dHeight = GetTextHeight(aname(objArrText.first()));
		double dScale = (2.5 * m_dhhFrame) / dHeight;
		ScaleEntitys(objArrText, apoint(nodesO.m_pointsArrond.ptTop), dScale);
	}

	return true;
}


bool CTukuang::insertLegen(ads_point pt1, ads_point pt2, ads_point ptInsert)
{
	AcGePoint2dArray nodes;
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(apoint(pt1), apoint(pt2), nodes);
	AcDbObjectIdArray objArr;
	ssFromNodes(objArr, nodes, 2);
	SLegendOpe lengOpe;
	lengOpe.setHH(m_dhhFrame);
	if(lengOpe.readLegendConfig(_T("建筑分层图图例_配置.XML")))
	{
		lengOpe.setEntitys(objArr);
		if(false == lengOpe.insertLegendTable(apoint(ptInsert)))
		{
			m_bHaveLegend = false;
			return false;
		}
 		setlayers(lengOpe.m_objArrLegend, _T("图廓"));

		for(int j = 0; j < lengOpe.m_objArrBlocks.length(); j ++)
		{
			AcDbBlockReference *pBlock;
			if(Acad::eOk == acdbOpenObject(pBlock, lengOpe.m_objArrBlocks.at(j), AcDb::kForWrite))
			{
				pBlock->explodeToOwnerSpace();
				pBlock->erase();
				pBlock->close();
			}
		}
 			//ScaleEntitys(lengOpe.m_objArrLegend, apoint(ptInsert), m_dhhFrame);
	}

	
	ads_name ssname;
	IdArrToSS(lengOpe.m_objArrLegend, ssname);
	AddXdata_bat(ssname, XDATA_ENTITY_TYPE, INT_TYPE, _T("1"));
	ads_ssfree(ssname);
	AddEntsToGroup(m_objInnerFrame, lengOpe.m_objArrLegend);

	m_ptminLegen[X] = lengOpe.m_ptBorderLB.x;m_ptminLegen[Y] = lengOpe.m_ptBorderLB.y;  
	m_ptmaxLegen[X] = lengOpe.m_ptBorderRT.x;m_ptmaxLegen[Y] = lengOpe.m_ptBorderRT.y;

	return true;
}

bool	CTukuang::startFloorPlane_Multy()
{
	if(checkSameGUID() == false) return false;

	checkProjInfo();

	double hh = 0.0;
	read1(hh); hh /= 1000.0;

	m_strBat_Id = newGUID2();

	//startFloorPlane();

	int nCount = 0;
	while(1)
	{

		ads_point pt1, pt2;
		if(RTCAN == ads_getpoint(NULL, _T("\n 请指定左下角"), pt1)) return false;
		if(RTCAN == ads_getcorner(pt1, _T("\n 请指定右上角"), pt2)) return false;

		SNodesOpe nodesO;
		nodesO.modifyLB_RTPoint(pt1, pt2);

		m_pt2dLBTK.x = pt1[X];m_pt2dLBTK.y = pt1[Y];
		m_pt2dRTTK.x = pt2[X];m_pt2dRTTK.y = pt2[Y];

		m_bMulty = true;
		int nRes = IDNO;
		if(nCount > 0)
		{
			nRes = AfxMessageBox(_T("是不是最后一幅了？"), MB_YESNOCANCEL);
			if(nRes == IDYES) m_bLast = true;
		}
		else
		{
			m_bLast = false;
		}
		
		startFloorPlane();

		nCount ++;

		if(nRes == IDNO) continue;
		else if(nRes ==IDYES) break;
		else return false;

	}

	return true;
}

bool CTukuang::insertTittle_FloorPlane(ads_point pt)
{
	double dXScale = (m_pt2dRTTK.x - m_pt2dLBTK.x) / 195.32;			//表头模板长度
	AcDbObjectId objTittle = DrawInsert(pt, _T("分层图_表头"), dXScale);
	
	SBreakBlock breakB;
	AcDbObjectId objTable;
	breakB.SetBlock(objTittle);
	breakB.Explode();
	if(false == breakB.findTable(objTable)) return true;

	AcDbTable *pTable;
	if(Acad::eOk != acdbOpenObject(pTable, objTable, AcDb::kForWrite)) return true;
	for(Adesk::UInt32 i = 0; i < pTable->numRows(); i ++)
	{
		double dHeight = pTable->textHeight(i, 0);
		dHeight *= m_dhhFrame;
		pTable->setTextHeight(i, 0, dHeight);
	}
	pTable->setHeight(15.5 * m_dhhFrame);
	pTable->close();

	ads_point ptmin3, ptmax3;
	GetEntExtent(aname(objTable), ptmin3, ptmax3);
	m_pt2dWRTTK.x = ptmax3[X]; m_pt2dWRTTK.y = ptmax3[Y];

	breakB.replaceTextInTable(objTable, m_vStrKey, m_vStrValue);

	AddXdataShort(aname(objTable), XDATA_ENTITY_TYPE, 3);

	return true;
}

bool CTukuang::showMatchFieldDlg()
{

	CMatchFialogsEx matchDialog;
	matchDialog.m_bShowFrameScaleControl = true;
	matchDialog.SetConfigFile(_T("字段匹配表_建筑分层平面图.ini"));
	if(matchDialog.DoModal() != IDOK) return false;
	if(matchDialog.m_dFrameScale == 0) m_dhhFrame = 0.5;
	else if(matchDialog.m_dFrameScale == 1) m_dhhFrame = 1.0;
	else m_dhhFrame = 2.0;

	cstringArrToVector(matchDialog.m_strArrKey, m_vStrKey);
	cstringArrToVector(matchDialog.m_strArrValue, m_vStrValue);

	for(size_t i = 0; i < m_vStrKey.size(); i ++)
	{
		if(m_vStrKey.at(i).Find(_T("JZMC")) >= 0)
		{
			m_strJZMC = m_vStrValue.at(i);
			break;
		}
	}

	return true;
}

bool		CTukuang::drawInnerFrame()
{
	AcGePoint2dArray nodes;
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(m_pt2dLBTK, m_pt2dRTTK, nodes);
	m_objInnerFrame = DrawPolyline2D(nodes, 0.0, true, _T(""), 0.0, 0, _T("图廓"));
	setcolor(aname(m_objInnerFrame), 7);

	if(m_bMulty)
	{
		AddXdataShort(aname(m_objInnerFrame), _T("FRAME_TYPE"), 2);
		AddXdata(aname(m_objInnerFrame), _T("FRAME_BAT_ID"), STRING_TYPE, m_strBat_Id);
	}
	else
		AddXdataShort(aname(m_objInnerFrame), _T("FRAME_TYPE"), 1);

	if(m_bLast)
		AddXdataShort(aname(m_objInnerFrame), _T("FRAME_BAT_LAST"), 1);

	AcDbObjectId objGroup;
	CreateGroup(aname(m_objInnerFrame), objGroup);

	return true;
}

bool		CTukuang::checkSameGUID()
{
	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 1, _T("*TEXT*"));

	bool bFind = false;
	AGrid_ClearSheet(_T("统计楼层信息"));
	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR sz[100] = {0};
		ReadXdata(aname(objArr.at(i)), _T("GUID"), STRING_TYPE, sz);
		if(0 == _tcscmp(sz, _T(""))) continue;

		for(int j = i + 1; j < objArr.length(); j ++)
		{
			TCHAR sz2[100] = {0};
			ReadXdata(aname(objArr.at(j)), _T("GUID"), STRING_TYPE, sz2);
			if(0 == _tcscmp(sz2, _T(""))) continue;

			if(0 == _tcscmp(sz, sz2))
			{
				AGrid_AppendMsgEnt(_T("统计楼层信息"),_T("重复面积注记！"),_T("警告"), objArr.at(i));
				AGrid_AppendMsgEnt(_T("统计楼层信息"),_T("重复面积注记！"),_T("警告"), objArr.at(j));

				bFind = true;
			}
		}
	}

	if(bFind)
	{
		AGrid_ShowMe();
		if( IDYES == AfxMessageBox(_T("发现有重复面积注记（是否复制出来？），继续操作可能会有面积错误，是否继续？"), MB_YESNO) )
			return true;
		else
			return false;
	}
	return true;
}


bool	CTukuang::startFloorPlane()
{
	ads_point pt1, pt2, pt3, pt4;	//左下，右上，右下，左上

	if(checkSameGUID() == false) return false;

	//批量输出在外面指定插入点
	if(m_bMulty == false)
	{
		checkProjInfo();

		double hh = 0.0;
		read1(hh); hh /= 1000.0;

		if(RTCAN == ads_getpoint(NULL, _T("\n 请指定左下角"), pt1)) return false;
		if(RTCAN == ads_getcorner(pt1, _T("\n 请指定右上角"), pt2)) return false;

		SNodesOpe nodesO;
		nodesO.modifyLB_RTPoint(pt1, pt2);

		m_pt2dLBTK.x = pt1[X];m_pt2dLBTK.y = pt1[Y];
		m_pt2dRTTK.x = pt2[X];m_pt2dRTTK.y = pt2[Y];
	}

	if(false == showMatchFieldDlg()) return false;

	getFloorCount_Top_Under();

	//一定要先绘制内图框
	drawInnerFrame();

	pt3[X] = m_pt2dRTTK.x;pt3[Y] = m_pt2dLBTK.y;pt3[Z] = 0.0;
	pt4[X] = m_pt2dLBTK.x;pt4[Y] = m_pt2dRTTK.y;pt4[Z] = 0.0;
	sumAreaAndinsertTable(pt3);

	insertLegen(apoint(m_pt2dLBTK), apoint(m_pt2dRTTK), apoint(m_pt2dLBTK));

	insertTittle_FloorPlane(pt4);

	CString strText;
	if(m_bHaveLegend)
	{
		//如果有生成图例，则取图例跟右下角表格中取Y值最小的那个
		m_pt2dWLBTK.x = m_pt2dLBTK.x;m_pt2dWLBTK.y = m_ptminLegen[Y] < m_ptminTable[Y] ? m_ptminLegen[Y] : m_ptminTable[Y];
	}
	else
	{
		//如果没有生成图例，则直接取右下角表格作为Y值
		m_pt2dWLBTK.x = m_pt2dLBTK.x;m_pt2dWLBTK.y =  m_ptminTable[Y];
	}

	AcGePoint2dArray nodes, nodes2;
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(m_pt2dWLBTK, m_pt2dRTTK, nodes);
	AcDbObjectId objTemp = DrawPolyline2D(nodes, 0.0, true, _T(""), 0.0, 0, _T("图廓"));
	setcolor(aname(objTemp), 7);

	insertPrintPoint();

	scaleTextsInFrame();

// 	GetEntExtent(aname(objTemp), ptmin, ptmax);
// 	ads_polar(ptmin, PI + PI / 4, sqrt(10*10 + 10 * 10), pt1);
// 	pt2[X] = pt1[X];pt2[Y] = ptmax[Y] + (ptmax3[Y] - ptmin3[Y]);
// 	pt3[X] = ptmax[X] + 10;pt3[Y] = ptmax[Y] + (ptmax3[Y] - ptmin3[Y]);
// 	pt4[X] = pt3[X];pt4[Y] = pt1[Y];
// 	nodes.setLogicalLength(0);
// 	nodes.append(apoint(pt1));
// 	nodes.append(apoint(pt2));
// 	nodes.append(apoint(pt3));
// 	nodes.append(apoint(pt4));
// 	objTemp = DrawPolyline2D(nodes, 0.0, true, _T(""), 0.0, 0, _T("TK"));
// 	setcolor(aname(objTemp), 7);
// 	setlinetype(aname(objTemp), _T("X5"));

	

	//saveBuildingInfo();

	return true;
}

bool	CTukuang::insertPrintPoint()
{
	ads_point pt1, pt2;
	ads_polar(apoint(m_pt2dWLBTK), PI, 7.5 * m_dhhFrame, pt1);
	ads_polar(pt1, PI + PI / 2 , 7.5 * m_dhhFrame, pt2);
	AcDbObjectId objId = DrawInsert(pt2, _T("建筑分层图_打印定位点_左下角"));
	if(objId == AcDbObjectId::kNull) acutPrintf(_T("\n 插入块失败！ 建筑分层图_打印定位点_左下角.dwg"));

	ads_polar(apoint(m_pt2dWRTTK), 0, 7.5 * m_dhhFrame, pt1);
	//ads_polar(pt1, PI / 2 , 15.5 * m_dhhFrame, pt2);

	objId = DrawInsert(pt1, _T("建筑分层图_打印定位点_右上角"));
	if(objId == AcDbObjectId::kNull) acutPrintf(_T("\n 插入块失败！ 建筑分层图_打印定位点_右上角.dwg"));

	return true;
}

bool	CTukuang::saveBuildingInfo(AcDbObjectId &objIdKey)
{
	CString strText;
	AddXdata(aname(objIdKey), _T("BUILDING_NAME"), STRING_TYPE, m_strJZMC);
	strText.Format(_T("%lf"), m_dTol);
	AddXdata(aname(objIdKey), _T("AREA_TOL"), STRING_TYPE, strText);
	strText.Format(_T("%lf"), m_dTolTop);
	AddXdata(aname(objIdKey), _T("AREA_TOL_TOP"), STRING_TYPE, strText);
	strText.Format(_T("%lf"), m_dTopUnder);
	AddXdata(aname(objIdKey), _T("AREA_TOL_UNDER"), STRING_TYPE, strText);
	//strText.Format(_T("%d"), m_nFloorCountTop);
	strText.Format(_T("%d"), m_nFoorCountTop_Mul);
	AddXdata(aname(objIdKey), _T("FLOOR_COUNT_TOP"), STRING_TYPE, strText);
	//strText.Format(_T("%d"), m_nFloorCountUnder);
	strText.Format(_T("%d"), m_nFloorCountUnder_Mul);
	AddXdata(aname(objIdKey), _T("FLOOR_COUNT_UNDER"), STRING_TYPE, strText);

	return true;
}


bool CTukuang::saveBuildingInfo()
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

	//写入信息前判断当前dwg的项目名称，许可证编号跟xml里的一不一样，如果不一样，则把xml里以前的数据删除
	//如果一样则不删除，下面会更新每一条记录
	TiXmlElement *pProjectInfo = pRoot->FirstChildElement("ProjectInfo");
	TiXmlElement *pBuild;
	CEditListCtrl edtiList;
	CXRecordOperate xrecordO;
	if(pProjectInfo != NULL)
	{
		
		CString str1, str2, str3, str4;
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("GCMC"), str1);
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("XKZBH"), str2);
		str3 = edtiList.GetXMLAttribute_EditList(pProjectInfo, ("GCMC"));
		str4 = edtiList.GetXMLAttribute_EditList(pProjectInfo, ("XKZBH"));
		if(str1 != str3 || str2 != str4)
		{
			SetXMLAttribute(pProjectInfo, _T("GCMC"), str1);
			SetXMLAttribute(pProjectInfo, _T("XKZBH"), str2);

			pBuild = pRoot->FirstChildElement("Building");
			while(pBuild)
			{
				pRoot->RemoveChild(pBuild);
				pBuild = pBuild->NextSiblingElement();
			}
			
		}

	}
	else		//找不到工程信息节点，创建一个
	{
#ifdef UNICODE
		char pointerNewItem[255] = {0};
		UnicodeToUTF8(_T("ProjectInfo"),pointerNewItem);
		TiXmlElement pItemNew(pointerNewItem);
#else
		TiXmlElement pItemNew(GBToUTF8(_T("ProjectInfo")));
#endif
		CString str1, str2;
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("GCMC"), str1);
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("XKZBH"), str2);
		SetXMLAttribute(&pItemNew, _T("GCMC"), str1);
		SetXMLAttribute(&pItemNew, _T("XKZBH"), str2);
		pRoot->InsertEndChild(pItemNew);
	}

	//确保了当前dwg跟xml里保存的数据属于同一工程的前提下，开始更新或插入数据
	bool bFind = false;
	pBuild = pRoot->FirstChildElement("Building");
	CString strText;
	while(pBuild)
	{
		strText = edtiList.GetXMLAttribute_EditList(pBuild, ("name"));
		if(strText == m_strJZMC)
		{
			strText.Format(_T("%lf"), m_dTol);
			SetXMLAttribute(pBuild, _T("area_tol"), strText);
			strText.Format(_T("%lf"), m_dTolTop);
			SetXMLAttribute(pBuild, _T("area_top"), strText);
			strText.Format(_T("%lf"), m_dTopUnder);
			SetXMLAttribute(pBuild, _T("area_under"), strText);
			strText.Format(_T("%d"), m_nFloorCountTop);
			SetXMLAttribute(pBuild, _T("floorcount_top"), strText);
			strText.Format(_T("%d"), m_nFloorCountUnder);
			SetXMLAttribute(pBuild, _T("floorcount_under"), strText);
			bFind = true;
			break;
		}

		pBuild = pBuild->NextSiblingElement();
	}

	if(bFind == false)
	{
#ifdef UNICODE
		char pointerNewItem[255] = {0};
		UnicodeToUTF8(_T("Building"),pointerNewItem);
		TiXmlElement pItemNew(pointerNewItem);
#else
		TiXmlElement pItemNew(GBToUTF8(_T("Building")));
#endif

		SetXMLAttribute(&pItemNew, _T("name"), m_strJZMC);
		strText.Format(_T("%lf"), m_dTol);
		SetXMLAttribute(&pItemNew, _T("area_tol"), strText);
		strText.Format(_T("%lf"), m_dTolTop);
		SetXMLAttribute(&pItemNew, _T("area_top"), strText);
		strText.Format(_T("%lf"), m_dTopUnder);
		SetXMLAttribute(&pItemNew, _T("area_under"), strText);
		strText.Format(_T("%d"), m_nFloorCountTop);
		SetXMLAttribute(&pItemNew, _T("floorcount_top"), strText);
		strText.Format(_T("%d"), m_nFloorCountUnder);
		SetXMLAttribute(&pItemNew, _T("floorcount_under"), strText);

		pRoot->InsertEndChild(pItemNew);
	}

	tinyXml.SaveFile();

	return true;
}




CTukuang::CTukuang(void)
{
	m_dTol = m_dTolTop = m_dTopUnder = 0.0;
	m_nFloorCountTop = m_nFloorCountUnder = 0;
	m_dhh = 0.0;
	m_bMulty = false;
	m_bLast = true;
	m_bDiveByFrame = false;
	m_dhh = 0.5;
	m_dhhFrame = 0.5;
	m_bHaveLegend = true;
	m_nFoorCountTop_Mul = m_nFloorCountUnder_Mul = 0;
	m_sAutoLegend = 1;
}


CTukuang::~CTukuang(void)
{
}



bool  CTukuang::insertTable(AcGePoint3d ptRb)
{
	AcDbObjectId objTable = DrawInsert(apoint(ptRb), _T("总平面竣工图_表格"));

	SBreakBlock blockB;
	blockB.m_objIdArr.setLogicalLength(0);
	blockB.SetBlock(objTable);
	blockB.Explode();
	if(blockB.m_objIdArr.length() == 0) return false;
	blockB.findTable(objTable);

	CString strText;
	CXRecordOperate xrecordO;
	vector<CString> vStrKeys, vStrValues;
	strText.Format(_T("1:%.lf"), m_dhh * 1000);
	vStrKeys.push_back(_T("#SCALE#"));
	vStrValues.push_back(strText);

	for(int i = 0; i < g_nMaxInfo; i ++)
	{
		if(0 == _tcscmp(g_AttrInfo[i].table, _T("XMXX")))
		{
			for(int j = 0; j < g_AttrInfo[i].AttrArr.GetSize(); j ++)
			{
				xrecordO.ReadStringRecord(DICT_PROJECTINFO, g_AttrInfo[i].AttrArr.GetAt(j).name, strText);

				TCHAR szText[100] = {0};
				_stprintf(szText, _T("#%s#"), g_AttrInfo[i].AttrArr.GetAt(j).name);
				vStrKeys.push_back(szText);
				vStrValues.push_back(strText);

			}
			break;
		}
	}

	blockB.replaceTextInTable(objTable, vStrKeys, vStrValues);

// 	CLayers layer;
// 	layer.drawOrder(objTable);

	//因为上面进行了文本替换，表格可能因为内容太长换行而导致表格向下挤，超出图框，所以
	//这里重新把表格移到右下角，并绘制消隐
	ads_point ptmin, ptmax, ptRBTable;
	GetEntExtent(aname(objTable), ptmin, ptmax);
	ptRBTable[X] = ptmax[X]; ptRBTable[Y] = ptmin[Y];
	MoveEntity(aname(objTable), ptRb.x - ptRBTable[X], ptRb.y - ptRBTable[Y]);
	GetEntExtent(aname(objTable), ptmin, ptmax);

	AcGePoint2dArray nodes;
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(apoint(ptmin), apoint(ptmax), nodes);
 	AcDbObjectId objWipe = drawWipeOut(nodes);
// 	if(objWipe != AcDbObjectId::kNull)
// 	{
// 		CLayers layers;
// 		layers.drawOrder(objWipe, objTable, false);
// 	}
	CLayers layers;
	layers.drawOrder2(objTable);
	
	
	return true;
}
