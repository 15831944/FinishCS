#include "stdafx.h"
#include "Global.h"
#include <cmath>
#include "ContructsOperate.h"
#include "XMJGMacro.h"
#include "PrintPDF.h"
#include "XMJGTool.h"
#include <acadi.h>
#include <dblead.h>
#include <atlpath.h>
#include "AutoLoop.h"
#include "pdf2png.h"
#include "AttributeDef.h"
#include "KernalTool.h"


int      g_nMaxInfo = 0;
ATTRINFO g_AttrInfo[MAX_ATTRSIZE];
Indexini g_index;
Workdef g_workdef;
AttributeDef g_attribute;


CListItemsMdb	g_lstitems;

FieldChangeTable g_fieldchagetable;

vector<DefaultAreaParam> g_vDefaultAreaParam;

void GetMiddlePoint(AcGePoint2dArray & nodes, ads_point& pt)
{
	ads_point ptMin, ptMax;
	GetPolyExtent(nodes, ptMin, ptMax, 0);
	pt[X] = (ptMin[X] + ptMax[X]) / 2;
	pt[Y] = (ptMin[Y] + ptMax[Y]) / 2;
}

void GetMiddlePoint(ads_name ent, ads_point &mid)
{
	AcGePoint2dArray nodes;
	GetPlList(ent, nodes);
	ads_point ptMin, ptMax;
	GetPolyExtent(nodes, ptMin, ptMax, 0);
	mid[X] = (ptMin[X] + ptMax[X]) / 2;
	mid[Y] = (ptMin[Y] + ptMax[Y]) / 2;
	mid[Z] = (ptMin[Z] + ptMax[Z]) / 2;
}

void GetMiddlePoint(const AcDbObjectId &id, AcGePoint2d &mid)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	ads_point ptMin, ptMax;
	GetPolyExtent(nodes, ptMin, ptMax, 0);
	mid.x = (ptMin[X] + ptMax[X]) / 2;
	mid.y = (ptMin[Y] + ptMax[Y]) / 2;
}

void GetMiddlePoint(const AcDbObjectId &id, AcGePoint3d &mid)//获得几何实体的最小外包矩形的中心
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);//获得几何实体的所有节点
	ads_point ptMin, ptMax;
	GetPolyExtent(nodes, ptMin, ptMax, 0);//获得几何实体最小外包矩形的左下角点和右上角点
	//最小外包矩形的左下角点和右上角点的中点就是中点
	mid.x = (ptMin[X] + ptMax[X]) / 2;
	mid.y = (ptMin[Y] + ptMax[Y]) / 2;
	mid.z = (ptMin[Z] + ptMax[Z]) / 2;
}

bool InsertDwgFile(CString csPath)
{
	if(csPath.IsEmpty()) return false;
	AcDbDatabase* pExternalDb = new AcDbDatabase(false);
	pExternalDb->readDwgFile(csPath);
	AcDbBlockTable* pBlockTable;
	pExternalDb->getBlockTable(pBlockTable, AcDb::kForRead);
	if(pBlockTable == NULL)
	{
		MessageBox(GetForegroundWindow(), _T("加载地图文件失败,请检查数据是否已经打开或者只读。。。"), _T("错误信息"), 0);
		return false;
	}
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForRead);
	AcDbBlockTableRecordIterator* pItr;
	pBlockTableRecord->newIterator(pItr);
	AcDbObjectIdArray list;  // 导出到临时图形数据库的实体数组 
	int nTmp = 0;
	int i = 0;
	for(pItr->start(); !pItr->done(); pItr->step())
	{
		AcDbObjectId tmpId;
		AcDbEntity* pEnt;
		pItr->getEntity(pEnt, AcDb::kForRead);
		list.append(pEnt->objectId());
		pEnt->close();
	}
	delete pItr;
	pBlockTableRecord->close();
	pBlockTable->close();
	int len = list.length();

	if(len > 0)
	{
		AcDbDatabase *pTempDb;
		pExternalDb->forceWblockDatabaseCopy();
		int es = pExternalDb->wblock(pTempDb, list, AcGePoint3d::kOrigin);
		if(es != Acad::eOk)
		{
			CString str;
			str.Format(_T("%d"), es);
			AfxMessageBox(str);
			delete pExternalDb;
			return false;
		}
		acdbHostApplicationServices()->workingDatabase()->insert(AcGeMatrix3d::kIdentity, pTempDb);
		delete pTempDb;
	}
	delete pExternalDb;
	return true;
}

bool getAllTablesName(CAdoConnection *conn, vector<CString> &v_table_name, short sType)
{
	//msysobjects
	if(conn == NULL) return false;
	if(conn->IsOpen() == FALSE) return false;

	CString strSQL;
	if(sType == 2)
		strSQL = _T("SELECT * FROM msysobjects WHERE type = 5 AND flags = 48");
	CAdoRecordSet p_record_set(conn);
	if(FALSE == p_record_set.Open(strSQL))
	{
		ads_printf(_T("\n 执行SQL语句失败！ %s"), strSQL);
		return false;
	}
	p_record_set.MoveFirst();
	while(!p_record_set.IsEOF())
	{
		TCHAR szTable[200] = {0};
		p_record_set.GetCollect(_T("Name"), szTable);

		if(0 == _tcslen(szTable)) continue;

		v_table_name.push_back(szTable);
		p_record_set.MoveNext();
	}
	p_record_set.Close();
	return true;
}

bool checkLoadARX(const TCHAR *szArxName)
{
	resbuf *rc = ads_arxloaded();
	resbuf *rlist = rc;
	bool bFind = false;
	while(rlist)
	{
		if(0 == _tcsicmp(szArxName, rlist->resval.rstring))
		{
			bFind = true;
			break;
		}
		//acutPrintf(_T("\n %s"), rlist->resval.rstring);
		rlist = rlist->rbnext;
	}

	return bFind;
}

bool HasXData(const AcDbObjectId& objId, const CString& csRegAppName)
{
	AcDbObject *pObj = NULL;
	if(Acad::eOk != acdbOpenObject(pObj, objId, AcDb::kForRead, true))
		return false;

	resbuf* pHead = pObj->xData();
	if(NULL == pHead)
		return false;

	bool bFind = false;
	resbuf* pNext = pHead;
	while(NULL != pNext)
	{
		if(AcDb::kDxfRegAppName == pNext->restype)
		{
			if(0 == _tcsicmp(pNext->resval.rstring, csRegAppName))
			{
				bFind = true;
				break;
			}
		}
		pNext = pNext->rbnext;
	}
	ads_relrb(pHead);
	pObj->close();

	return bFind;
}

bool AddEntsToGroup(const AcDbObjectId ObjId, AcDbObjectIdArray objArr)
{
	AcDbObjectId objGroupId;
	GetEntGroup(aname(ObjId), objGroupId);
	if(objGroupId == AcDbObjectId::kNull) return false;

	for(int i = 0; i < objArr.length(); i++)
		AddEntToGroup(objArr.at(i), objGroupId);

	return true;
}

bool setColorsEx(AcDbObjectIdArray objArr, Adesk::UInt16 nIndex)
{
	for(int i = 0; i < objArr.length(); i++)
	{
		TCHAR szType[200] = {0};
		GetEntityType(aname(objArr.at(i)), szType);
		if(0 == _tcscmp(szType, _T("INSERT")))
		{
			SBlockOpe blockO;
			blockO.ChangeBlockRefenceColor(objArr.at(i), nIndex);
			blockO.ChangeBlockAttributeColor(objArr.at(i), nIndex);
		}
		else
		{
			setcolor(aname(objArr.at(i)), nIndex);
		}

	}

	return true;
}

int getDecimalPrec(CString strDecimal)
{
	TCHAR sz1[100] = {0}, sz2[100] = {0};
	analyzeData(strDecimal, _T("."), sz1, sz2);
	return (int)_tcslen(sz2);
}

bool getEntsByGroupId(AcDbObjectId objGroup, AcDbObjectIdArray &objArr)
{
	AcDbGroup *pGroup;
	if(Acad::eOk != acdbOpenObject(pGroup, objGroup, AcDb::kForRead)) return false;
	AcDbGroupIterator *pIter = pGroup->newIterator();
	for(; !pIter->done(); pIter->next())
	{
		AcDbObjectId objId = pIter->objectId();
		objArr.append(objId);
	}
	delete pIter;
	pGroup->close();
	return true;
}

void SetPolyCoord_NoArc(ads_name ent, AcGePoint2dArray &Nodes)
{
	unsigned int nTotal = Nodes.length();
	if(nTotal <= 1) return;
	AcDbObjectId objId;
	AcGePoint2d pt2d;
	AcDbEntity *pEnt;
	double dWidth;
	AcGeVector3d vec0(0, 0, 1);

	if(false == GetPolyWidth(ent, dWidth)) return;
	acdbGetObjectId(objId, ent);
	if(Acad::eOk != acdbOpenObject(pEnt, objId, AcDb::kForWrite)) return;
	if(pEnt->isA() == AcDbPolyline::desc())
	{
		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
		pPoly->setNormal(vec0);

		Adesk::Boolean bIsClosed = pPoly->isClosed();
		unsigned nLen = pPoly->numVerts();
		unsigned i;
		pPoly->reset(false, nTotal);
		nLen = pPoly->numVerts();
		// 		for(i = 0; i < nLen; i ++) 
		// 			pPoly->removeVertexAt(i);
		// 		for (i=0; i<nLen; i++) {
		// 			if ( i >= nTotal )
		// 				pPoly->removeVertexAt(nTotal);
		// 			else {
		// 				pt2d = Nodes.at(i);
		// 				pPoly->setPointAt(i,pt2d);
		// 			}
		// 		}
		for(i = 0; i < nTotal; i++)
		{
			AcGePoint2d pt2d = Nodes.at(i);
			pPoly->addVertexAt(i, pt2d);
		}
		if(bIsClosed)
			pPoly->setClosed(true);
	}
	else if(pEnt->isA() == AcDb2dPolyline::desc())
	{

	}
	pEnt->close();
}

bool ssfromNodes_Arc(AcDbObjectIdArray &objArr, AcDbObjectId objPolyline, int nWay, double dOffset, TCHAR *szEntityType, TCHAR *szLayer)
{
	AcGePoint2dArray nodes;
	GetEntExtent(aname(objPolyline), nodes);
	if(nodes.length() < 3) return false;

	AcDbObjectIdArray objArrTemp;
	ssFromNodes(objArrTemp, nodes, nWay, dOffset, szEntityType, szLayer);
	objArrTemp.remove(objPolyline);		//把自己给排除掉

	nodes.setLogicalLength(0);
	AcGeDoubleArray doubleArr;
	GetPlList(aname(objPolyline), nodes);
	GetPlBulges(aname(objPolyline), doubleArr);

	if(nWay == 1)		//WP框选
	{
		for(int i = 0; i < objArrTemp.length(); i++)
		{
			AcGePoint2dArray nodesTemp;
			if(false == GetPlList(aname(objArrTemp.at(i)), nodesTemp)) continue;

			bool bAllIn = true;
			for(int j = 0; j < nodesTemp.length(); j++)
			{
				if(PointInPolygon(nodes, doubleArr, nodesTemp.at(j)) == 1)
				{
					bAllIn = false;
					break;
				}
			}

			//只有全部节点都在里面或在上面，才算
			if(bAllIn) objArr.append(objArrTemp.at(i));
		}
	}
	else if(nWay == 2)		//CP框选
	{
		for(int i = 0; i < objArrTemp.length(); i++)
		{
			AcGePoint2dArray nodesTemp;
			AcGeDoubleArray doubleArrTemp;
			if(false == GetPlList(aname(objArrTemp.at(i)), nodesTemp)) continue;
			GetPlBulges(aname(objArrTemp.at(i)), doubleArrTemp);

			AcGePoint2dArray nodesOut;
			AcGeDoubleArray doubleOut;
			polylineArcInter(nodes, doubleArr, nodesTemp, doubleArrTemp, nodesOut, doubleOut);

			//有相交的地方，当作框选
			if(nodesOut.length() > 0)
			{
				objArr.append(objArrTemp.at(i));
			}
			else		//没相交的地方，判断每个节点在不在里面
			{
				bool bAllIn = true;
				for(int j = 0; j < nodesTemp.length(); j++)
				{
					if(PointInPolygon(nodes, doubleArr, nodesTemp.at(j)) == 1)
					{
						bAllIn = false;
						break;
					}
				}

				//只有全部节点都在里面或在上面，才算
				if(bAllIn) objArr.append(objArrTemp.at(i));
			}

		}
	}

	return true;
}

int isPolyInPoly_Completely(AcDbObjectId obj1, AcDbObjectId obj2)
{
	AcGePoint2dArray nodes1, nodes2;
	AcGeDoubleArray doubleArr1, doubleArr2;
	if(false == GetPlList(aname(obj1), nodes1)) return -1;
	if(false == GetPlList(aname(obj2), nodes2)) return -1;
	GetPlBulges(aname(obj1), doubleArr1);
	GetPlBulges(aname(obj2), doubleArr2);

	if(CEntOper::ifPolyClosed(obj1))
	{
		nodes1.append(nodes1.first());
		doubleArr1.append(doubleArr1.first());
	}
	if(CEntOper::ifPolyClosed(obj2))
	{
		nodes2.append(nodes2.first());
		doubleArr2.append(doubleArr2.first());
	}
	AcGePoint2dArray nodesOut;
	AcGeDoubleArray doubleArrOut;
	//polylineIntersection(nodes1, nodes2, nodesOut, doubleArr);
	polylineArcInter(nodes1, doubleArr1, nodes2, doubleArr2, nodesOut, doubleArrOut);
	if(nodesOut.length() > 0) return 0;

	bool bIn = true;
	for(int i = 0; i < nodes1.length(); i++)
	{
		if(PointInPolygon(nodes2, nodes1.at(i)) == 0 || PointInPolygon(nodes2, nodes1.at(i)) == 1)
		{
			bIn = false;
			break;
		}
	}

	if(bIn) return 1;
	else return 0;
}

CString findDefaultValue(CString strCode)
{
	CString	strValue;
	for(size_t t = 0; t < g_vDefaultAreaParam.size(); t++)
	{
		if(strCode == g_vDefaultAreaParam.at(t).strCode)
		{
			strValue.Format(_T("%.2lf"), g_vDefaultAreaParam.at(t).dValue);
			break;
		}
	}

	return strValue;
}

bool getEntPositionEx(AcDbObjectId objId, AcGePoint3d &pt3d)
{
	ads_name ent;
	if(Acad::eOk != acdbGetAdsName(ent, objId)) return false;

	TCHAR szType[200] = {0};
	GetEntityType(ent, szType);
	if(0 == _tcsicmp(szType, _T("HATCH")))
	{
		AcDbHatch *pHatch;
		if(Acad::eOk != acdbOpenObject(pHatch, objId, AcDb::kForRead)) return false;

		AcDbExtents extents;
		pHatch->getGeomExtents(extents);
		SNodesOpe nodesO;
		pt3d = GetMiddlePoint(extents.minPoint(), extents.maxPoint());
		pHatch->close();
	}
	else if(0 == _tcsicmp(szType, _T("ELLIPSE")))
	{
		AcDbEllipse *pElli;
		if(Acad::eOk != acdbOpenObject(pElli, objId, AcDb::kForRead)) return false;
		//pt3d = pElli->center();
		double dParam = 0.0;
		double dLength = 0.0;
		pElli->getEndParam(dParam);
		pElli->getDistAtParam(dParam, dLength);
		dLength *= 0.5;
		Acad::ErrorStatus es = pElli->getPointAtDist(dLength, pt3d);
		pElli->close();

	}
	else
	{
		getEntPosition(ent, pt3d);
	}

	return true;
}

bool setColorsEx2(AcDbObjectIdArray objArr, int nType)
{
	for(int i = 0; i < objArr.length(); i++)
	{
		setColorsEx2(objArr.at(i), nType);
	}
	return true;
}

bool setColorsEx2(AcDbObjectId ObjId, int nType)
{
	AcDbEntity * pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, ObjId, AcDb::kForWrite);
	if(es != Acad::eOk) return false;

	AcCmColor color;
	if(nType == 1)
		color.setColorMethod(AcCmEntityColor::kByLayer);
	else
		color.setColorMethod(AcCmEntityColor::kByBlock);
	pEnt->setColor(color);
	pEnt->close();

	return true;
}

AcDbObjectId drawWipeOut(AcGePoint2dArray nodes)
{
	AcDbObjectId objBlank = DrawPolyline2D(nodes, 0.0, true, _T(""));
	if(objBlank == AcDbObjectId::kNull) return AcDbObjectId::kNull;

	ads_name LastNamePL;
	if(Acad::eOk != acdbGetAdsName(LastNamePL, objBlank)) return AcDbObjectId::kNull;
#if _MSC_VER > 1200
	acedCommand(RTSTR, _T("WIPEOUT"), RTSTR, _T("P"), RTENAME, LastNamePL, RTSTR, _T("Y"), 0);
#else
	acedCommand(RTSTR, _T("WIPEOUT"), RTSTR, _T("N"), RTENAME, LastNamePL, RTSTR, _T("Y"), 0);
#endif // _MSC_VER
	ads_name WipeEntName;
	acdbEntLast(WipeEntName);
	if(Acad::eOk != acdbGetObjectId(objBlank, WipeEntName)) return AcDbObjectId::kNull;

	return objBlank;
}

CString getBuildStruct_FCS(AcDbObjectId objBuild)
{
	TCHAR sz[100] = {0};
	GetStdm(aname(objBuild), sz);
	CString strCode = sz;
	if(strCode == _T("141141")) return _T("钢");
	else if(strCode == _T("141181")) return _T("砖混楼");
	else if(strCode == _T("141361")) return _T("砖混平");
	else if(strCode == _T("141461")) return _T("砖木");
	else if(strCode == _T("141210")) return _T("简易平");
	else if(strCode == _T("141520")) return _T("简易棚");
	else if(strCode == _T("141500")) return _T("简易棚");
	else if(strCode == _T("141171")) return _T("土坯");
	else if(strCode == _T("153610")) return _T("砖混");
	else if(strCode == _T("153620")) return _T("简易");
	else return _T("其他");
}

CString getBuildStruct_FCS_Config(AcDbObjectId objBuild)
{
	CString strConstruct;
	TiXmlDocument *tinyXml = new TiXmlDocument();
	if(openXMLFile(tinyXml, _T("入户文字注记格式配置文件.XML")) == false)
	{
		delete tinyXml;
		return strConstruct;
	}

	TiXmlElement* pRoot = tinyXml->FirstChildElement("NoteThings");
	if(NULL == pRoot)
		return strConstruct;

	TiXmlElement *pEntity = pRoot->FirstChildElement("Thing");
	if(pEntity == NULL) return strConstruct;

	TCHAR sz[100] = {0};
	GetStdm(aname(objBuild), sz);

	CEditListCtrl editlist;
	CString strText;
	while(pEntity)
	{
		strText = editlist.GetXMLAttribute_EditList(pEntity, ("code"));
		if(strText == sz)
		{
			strText = editlist.GetXMLAttribute_EditList(pEntity, ("struct"));
			strConstruct = strText;
			if(strText == _T("")) acutPrintf(_T("\n 编码: %s 在配置文件中找不到结构！"), sz);
			break;
		}
		pEntity = pEntity->NextSiblingElement();
	}

	delete tinyXml;

	return strConstruct;
}

CString format_double_2(double dValue, int nDecimal)
{
	// 	int nMax = 10;
	// 	for(int i = 0; i < nDecimal; i ++)
	// 		nMax *= nMax;

	dValue += 0.0000001;		//为了防止计算机小数后随机取舍，这里加一个补偿数

	dValue *= 100;
	int nValue = (int)dValue;
	dValue /= 100;

	CString str;
	str.Format(_T("%.2lf"), dValue);

	return str;
}

bool openXMLFile(TiXmlDocument *&tinyXml, TCHAR *szName)
{
	TCHAR szPath[1000] = {0};
	ads_findfile(szName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szName);
		return false;
	}
	if(tinyXml == NULL) tinyXml = new TiXmlDocument();

	char szTemp1[1000] = {0};
	bool es = false;
#ifdef _UNICODE
	TcharToChar(szPath, szTemp1);
	es = tinyXml->LoadFile(szTemp1, TIXML_DEFAULT_ENCODING);
#else
	//XML文件最后保存成UTF-8格式，然后内部编码采用UTF-8编码，不然会读取中文乱码
	_tcscpy(szTemp1, szPath);
	es = tinyXml->LoadFile(szTemp1, TIXML_ENCODING_UTF8);
#endif

	if(es == false)
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szName);
		return false;
	}

	return true;
}

bool readDefaultAreaParam()
{
	g_vDefaultAreaParam.clear();

	TiXmlDocument *tinyXml = new TiXmlDocument();
	if(openXMLFile(tinyXml, _T("默认面积系数配置文件.XML")) == false)
	{
		delete tinyXml;
		return false;
	}

	TiXmlElement* pRoot = tinyXml->FirstChildElement("DefaultValues");
	if(NULL == pRoot)
		return false;

	TiXmlElement *pEntity = pRoot->FirstChildElement("Entity");
	if(pEntity == NULL) return false;

	CEditListCtrl editlist;
	CString strText;
	while(pEntity)
	{
		DefaultAreaParam defaultArea;
		strText = editlist.GetXMLAttribute_EditList(pEntity, ("code"));
		defaultArea.strCode = strText;

		strText = editlist.GetXMLAttribute_EditList(pEntity, ("defaultValue"));
		defaultArea.dValue = _tstof(strText);

		g_vDefaultAreaParam.push_back(defaultArea);

		pEntity = pEntity->NextSiblingElement();
	}

	delete tinyXml;

	return true;
}

void CreateLog(CString csPath)
{
	CStdioFile stdF;
	if(stdF.Open(csPath, CFile::modeCreate))
	{
		stdF.Close();
	}
}

void AddLog(CString csPath, CString csStr)
{
	CStdioFile stdF;
	TCHAR szTime[_MAX_PATH] = _T("");
	time_t curTime;
	time(&curTime);
	tm *t = localtime(&curTime);
	_tcsftime(szTime, _MAX_PATH, _T("%Y-%m-%d %H:%M:%S"), t);
	csStr = csStr + _T("  ");
	csStr += szTime;
	csStr += _T("\n");
	if(stdF.Open(csPath, CFile::modeWrite))
	{
		stdF.SeekToEnd();
		stdF.WriteString(csStr);
		stdF.Close();
	}
}

int AddXdataShort_bat(AcDbObjectIdArray objArr, const TCHAR *AppName, short nValue)
{
	for(int i = 0; i < objArr.length(); i++) AddXdataShort(aname(objArr.at(i)), AppName, nValue);
	return 0;
}

void AddXdataDouble_bat(AcDbObjectIdArray objArr, const TCHAR *AppName, double dValue)
{
	for(int i = 0; i < objArr.length(); i++) AddXdataDouble(aname(objArr.at(i)), AppName, dValue);
}

bool ssFromLayers(AcDbObjectIdArray &objArr, const TCHAR *szLayer, bool bRemove)
{
	if(bRemove) objArr.setLogicalLength(0);

	ads_name ssname;
	ssFromLayers(ssname, szLayer);
	SSToIdArr(ssname, objArr);

	return true;
}

bool checkDlgEditControlText(HWND hwnd, int nMaxCount, TCHAR *szSpecialWords)
{
	bool bFind = false;
	HWND childwnd = ::GetWindow(hwnd, GW_CHILD);
	while(childwnd)
	{
		childwnd = ::GetWindow(childwnd, GW_HWNDNEXT);

		TCHAR sz[200] = {0};
		GetClassName(childwnd, sz, 200);

		if(0 == _tcscmp(sz, _T("Edit")))
		{
			TCHAR *szText = new TCHAR[nMaxCount * 2];
			GetWindowText(childwnd, szText, nMaxCount * 2);
			if((int)_tcslen(szText) > nMaxCount)
			{
				bFind = true;
			}
			delete[] szText;
		}
		if(bFind) break;
	}

	return bFind;
}

bool IsPolyValid(AcGePoint2dArray &Nodes, CArray<AcGePoint2dArray, AcGePoint2dArray> &NodesArr)
{
	int i = 0;
	CAutoLoop autoLoop;
	ads_point ptMin, ptMax, ptMid;
	GetPolyExtent(Nodes, ptMin, ptMax, 0);
	ptMid[X] = (ptMin[X] + ptMax[X]) / 2;
	ptMid[Y] = (ptMin[Y] + ptMax[Y]) / 2;
	ptMid[Z] = 0;
	double dArea = fabs(GetNodesArea(Nodes));
	AcGePoint2dArray ptMidArr;
	AcGeDoubleArray dAreaArr;
	for(i = 0; i < NodesArr.GetSize(); i++)
	{
		AcGePoint2dArray tmpPt2dArr = NodesArr.GetAt(i);
		ads_point tmpptMin, tmpptMax, tmpptMid;
		GetPolyExtent(tmpPt2dArr, tmpptMin, tmpptMax, 0);
		tmpptMid[X] = (tmpptMin[X] + tmpptMax[X]) / 2;
		tmpptMid[Y] = (tmpptMin[Y] + tmpptMax[Y]) / 2;
		tmpptMid[Z] = 0;
		double dtmpArea = fabs(GetNodesArea(tmpPt2dArr));
		ptMidArr.append(asPnt2d(tmpptMid));
		dAreaArr.append(dtmpArea);
	}
	for(i = 0; i < NodesArr.GetSize(); i++)
	{
		if(fabs(dArea - dAreaArr[i]) < EPS && ptMidArr[i].distanceTo(AcGePoint2d(ptMid[X], ptMid[Y])) < EPS)
		{
			return false;
		}
	}
	return true;
}

bool filterObjectIdArr_ByGISTable(AcDbObjectIdArray objArr, AcDbObjectIdArray &objArrResult, const TCHAR *szGisTableName, short sRemoveResult)
{
	if(sRemoveResult == 1) objArrResult.setLogicalLength(0);

	for(int i = 0; i < objArr.length(); i++)
	{
		TCHAR szStdm[100] = {0}, szTable[500] = {0};
		GetStdm(aname(objArr.at(i)), szStdm);
		g_index.get_table(szStdm, szTable);

		if(0 == _tcsicmp(szTable, szGisTableName))
			objArrResult.append(objArr.at(i));
	}

	return true;
}

void	cstringArrToVector(const CStringArray &strArr, vector<CString> &vStr)
{
	vStr.clear();
	for(int i = 0; i < strArr.GetSize(); i++) vStr.push_back(strArr.GetAt(i));
}

void vectorToAcDbObjectIdArray(const vector<AcDbObjectId> vObjId, AcDbObjectIdArray &objArr, bool bClearResult)
{
	if(bClearResult) objArr.setLogicalLength(0);
	for(size_t t = 0; t < vObjId.size(); t++)
		objArr.append(vObjId.at(t));
}

void vectorToStringArray(const vector<CString> &vStr, CStringArray &strArr)
{
	strArr.RemoveAll();
	for(size_t t = 0; t < vStr.size(); t++)
		strArr.Add(vStr.at(t));
}

bool findAttriTable(const TCHAR *szTableName, CArray<ATTRFIELD, ATTRFIELD> *&AttrArr)
{
	if(0 == _tcscmp(szTableName, _T(""))) return false;
	for(int i = 0; i < g_nMaxInfo; i++)
	{
		if(0 == _tcscmp(g_AttrInfo[i].table, szTableName))
		{
			AttrArr = &(g_AttrInfo[i].AttrArr);
			if(AttrArr == NULL)
				return false;
			else
				return true;
		}
	}
	return false;
}

bool index_name_code(const TCHAR *szName, TCHAR *szCode)
{
	for(size_t t = 0; t < g_index.m_items.size(); t++)
	{
		if(0 == _tcscmp(g_index.m_items.at(t).name, szName))
		{
			_tcscpy(szCode, g_index.m_items.at(t).code);
			return true;
		}
	}
	return false;
}

bool index_name_code(const TCHAR *szTable, const TCHAR *szName, TCHAR *szCode)
{
	for(size_t t = 0; t < g_index.m_items.size(); t++)
	{
		if(0 == _tcscmp(g_index.m_items.at(t).table, szTable) &&
			0 == _tcscmp(g_index.m_items.at(t).name, szName))
		{
			_tcscpy(szCode, g_index.m_items.at(t).code);
			return true;
		}
	}
	return false;
}

void orderObjects(AcDbObjectIdArray &objArr)
{

	ads_point ptMin = {0}, ptMax = {0};
	GetEntsExtent(objArr, ptMin, ptMax, 5);
	AcDbObjectId objId;
	ads_point pt;
	AcGePoint2d pt2d;
	AcGePoint2dArray midArr;
	AcDbObjectIdArray tempIdArr;
	double dCur, dHeight = 5.0;
	int i = 0, j = 0;

	for(i = 0; i < objArr.length(); i++)
	{
		AcGePoint2dArray Nodes;
		GetPlList(aname(objArr.at(i)), Nodes);
		GetNodesMiddlePoint(Nodes, pt);
		pt2d.x = pt[X]; pt2d.y = pt[Y]; midArr.append(pt2d);
		tempIdArr.append(objArr.at(i));
	}
	//按横条状排列
	dCur = ptMax[Y];

	AcDbObjectIdArray objArrOrdered;
	while(dCur > ptMin[Y])
	{
		AcGeDoubleArray distArr;
		AcDbObjectIdArray daiIdArr;
		for(i = 0; i < midArr.length(); i++)
		{
			pt2d = midArr.at(i);
			objId = tempIdArr.at(i);
			if(pt2d.y >= dCur || pt2d.y < (dCur - dHeight)) continue;
			for(j = 0; j < distArr.length(); j++)
			{
				if(pt2d.x <= distArr[j]) break;
			}
			distArr.insertAt(j, pt2d.x);
			daiIdArr.insertAt(j, objId);
		}

		for(i = 0; i < daiIdArr.length(); i++)
		{
			objId = daiIdArr.at(i);
			objArrOrdered.append(objId);
		}
		dCur -= dHeight;
	}

	objArr.setLogicalLength(0);
	objArr.append(objArrOrdered);
}

bool filterSameString(vector<CString> &vStr)
{

	return true;
}

void SetXMLAttribute(TiXmlElement * pEle, CString pName, CString pValue)
{
#ifdef UNICODE
	char csName[255] = {0};
	char csValue[255] = {0};
	UnicodeToUTF8(pName, csName);
	UnicodeToUTF8(pValue, csValue);
	pEle->SetAttribute(csName, csValue);
#else
	pEle->SetAttribute(GBToUTF8(pName), GBToUTF8(pValue));
#endif

}

bool initAttribute()
{
	FILE *fp;
	TCHAR szStr[2000], *ppp, *qqq, szPath[MAX_PATH];

	//Attribute.def中读取配置文件
	acedFindFile(_T("attribute.def"), szPath);
	fp = _tfopen(szPath, _T("rt"));
	if(NULL != fp)
	{
		int i = -1;
		while(_fgetts(szStr, 255, fp))
		{
			CString csStr = CString(szStr);
			csStr.Replace(_T(" "), _T(""));
			csStr.Replace(_T("	"), _T(""));
			csStr.Replace(_T("　"), _T(""));
			_stprintf(szStr, _T("%s"), csStr);

			//if ( (!_tcsnicmp(szStr,_T("END"),3)) && (_tcsnicmp(szStr,_T("END,"),4)) ) break;

			szStr[_tcslen(szStr) - 1] = '\0';
			if('*' == szStr[0])
			{//新标准
				i++; if(i >= (MAX_ATTRSIZE - 4)) break;
				if(GetNumberOfComma(szStr) < 2) break;

				qqq = szStr + 1;
				GetStrgBeforeComma(qqq, g_AttrInfo[i].table);

				ppp = _tcschr(szStr, ','); qqq = ppp + 1;
				g_AttrInfo[i].type = _ttoi(qqq);
				ppp = _tcschr(qqq, ','); qqq = ppp + 1;
				GetStrgBeforeComma(qqq, g_AttrInfo[i].text);
			}
			else if(i >= 0)
			{
				ATTRFIELD Field;
				if(GetNumberOfComma(szStr) < 4) continue;

				GetStrgBeforeComma(szStr, Field.name);
				ppp = _tcschr(szStr, ','); qqq = ppp + 1;
				Field.type = _ttoi(qqq);
				ppp = _tcschr(qqq, ','); qqq = ppp + 1;
				Field.length = _ttoi(qqq);
				ppp = _tcschr(qqq, ','); qqq = ppp + 1;
				Field.decimal = _ttoi(qqq);
				ppp = _tcschr(qqq, ','); qqq = ppp + 1;

				//考虑数据字典
				ppp = _tcschr(qqq, ',');
				if(NULL == ppp)
				{
					_tcscpy(Field.text, qqq);
					_tcscpy(Field.dict, _T(""));
				}
				else
				{
					*ppp = '\0';
					_tcscpy(Field.text, qqq);
					_tcscpy(Field.dict, ppp + 1);
				}

				if('$' == Field.text[0])
				{
					TCHAR szTemp[50];
					ppp = Field.text + 1;
					_tcscpy(szTemp, ppp);
					_tcscpy(Field.text, szTemp);
					Field.bMust = true;
				}

				g_AttrInfo[i].AttrArr.Add(Field);
			}
		}
		fclose(fp);
		g_nMaxInfo = i + 1;
	}

	return true;
}

bool InitParaMeter()
{
	initAttribute();

	g_index.init();
	g_workdef.init();
	g_lstitems.readLstitem();
	g_attribute.initAttributeDef();
	readDefaultAreaParam();

	return true;
}

extern void requeue_WNqueue(AcGePoint2dArray tempNodes, AcGePoint2dArray& Nodes, AcGeDoubleArray &tempBulges, AcGeDoubleArray &Bulges);

bool BeginWestNorth(const AcDbObjectId &id)
{
	AcGePoint2dArray nodes_out;
	AcGePoint2dArray nodes;
	ads_point ptMid, ptTemp;
	if(GetPlList(aname(id), nodes) == false)	return false;
	GetEntCentroidPt(aname(id), ptMid);
	double LT_point_angle = 0.0;
	int index_first = 0;;
	for(int i = 0; i < nodes.length(); i++)
	{
		ptTemp[X] = nodes.at(i).x;
		ptTemp[Y] = nodes.at(i).y;
		double dAngle = 0.0;
		dAngle = ads_angle(ptMid, ptTemp);
		double temp_abs = 0.0;
		if(dAngle < 0)
			temp_abs = fabs(5 * PI / 4 + dAngle);
		else
			temp_abs = fabs(dAngle - 3 * PI / 4);

		if(i == 0)
		{
			LT_point_angle = temp_abs;
		}
		else
		{
			if(temp_abs < LT_point_angle)
			{
				LT_point_angle = temp_abs;
				index_first = i;               //获得最接近北偏西45度方向的点序号
			}
		}
	}
	// 	if (index_first!=0)
	// 	{
	// 		for (int j = index_first; j<nodes.length(); j++)   //点重排列
	// 			nodes_out.append(nodes.at(j));
	// 		for (int j = 0; j<index_first; j++)
	// 			nodes_out.append(nodes.at(j));
	// 	}
	// 	else
	nodes_out = nodes;
	SetPolyCoord(aname(id), nodes_out);
	return true;
}

void getNearestEdge(const AcGePoint2d &pt, const AcGePoint2dArray &nodes, int &index)
{
	int len = nodes.length();
	AcGePoint2dArray node;
	node.append(nodes);
	RemoveSameNodes(node);
	double dist = 99999999;
	for(int idx = 0; idx < len; ++idx)
	{
		int sidx = idx;
		int eidx = (idx + 1) % len;
		double dis = getPoint2LineMidPointDist(pt, node.at(sidx), node.at(eidx));
		index = abs(dis) < dist ? idx : index;
		dist = abs(dis) < dist ? abs(dis) : dist;
	}
}

double getPoint2LineMidPointDist(const AcGePoint2d &pt, const AcGePoint2d &spt, const AcGePoint2d &ept)
{
	AcGePoint2d mid;
	mid.x = (spt.x + ept.x) / 2;
	mid.y = (spt.y + ept.y) / 2;
	return distOF2d(pt, mid);
}

double getPoint2LineDist(const AcGePoint2d &pt, const AcGePoint2d &spt, const AcGePoint2d &ept)
{
	double y1 = (ept.y - spt.y) * pt.x; double y2 = (spt.y - ept.y) * spt.x;
	double x1 = (spt.x - ept.x) * pt.y; double x2 = (spt.x - ept.x) * spt.y;
	double x3 = pow((spt.x - ept.x), 2); double y3 = pow((ept.y - spt.y), 2);
	double dist = abs(y1 + y2 + x1 + x2) / sqrt(y3 + x3);
	return dist;
}

double getIntersectionAngle(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pty)
{
	double angle1 = ads_angle(apoint(pto), apoint(ptx));
	double angle2 = ads_angle(apoint(pto), apoint(pty));
	return angle2 - angle1;
}

void getIntersectionAngle(const AcDbObjectId &id, vector<double> &angles)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	int len = nodes.length();
	angles.clear();
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint2d ptx = nodes.at((len - idx - 1) % len);
		AcGePoint2d pto = nodes.at((len - idx + 0) % len);
		AcGePoint2d pty = nodes.at((len - idx + 1) % len);
		angles.push_back(getIntersectionAngle(ptx, pto, pty));
	}
}

void getAngleBisection(const AcDbObjectId &id, vector<double> &angle)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	getAngleBisection(nodes, angle);
}

void getAngleBisection(const AcGePoint2dArray &nodes, vector<double> &angle)
{
	int len = nodes.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint2d ptx = nodes.at((len - idx - 1) % len);
		AcGePoint2d pto = nodes.at((len - idx + 0) % len);
		AcGePoint2d pty = nodes.at((len - idx + 1) % len);
		angle.push_back(getAngleBisection(ptx, pto, pty));
	}
}

double getAngleBisection(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pty)
{
	double angle1 = ads_angle(apoint(pto), apoint(ptx));
	double angle2 = ads_angle(apoint(pto), apoint(pty));
	angle1 = angle1 > PI ? (angle1 - 2 * PI) : angle1;
	angle2 = angle2 > PI ? (angle2 - 2 * PI) : angle2;
	return (angle1 + angle2) / 2;
}

void getOutAngleBisection(const AcDbObjectId &id, vector<double> &angle)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	int len = nodes.length();
	bool bDeasil = IsDeasil(nodes);

	getOutAngleBisection(nodes, angle);
}

void getOutAngleBisection(const AcGePoint2dArray &nodes, vector<double> &angle)
{
	int len = nodes.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint2d ptx = nodes.at((len - idx - 1) % len);
		AcGePoint2d pto = nodes.at((len - idx + 0) % len);
		AcGePoint2d pty = nodes.at((len - idx + 1) % len);
		angle.push_back(getOutAngleBisection(ptx, pto, pty));
	}
}

double getOutAngleBisection(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pty)
{
	double ox = distOF2d(pto, ptx);			//点y所对应的边
	double oy = distOF2d(pto, pty);			//点x所对应的边
	double xy = distOF2d(ptx, pty);			//点o所对应的边
	double O = (ox * ox + oy * oy - xy * xy) / 2 / ox / oy;
	double start_angle = ads_angle(apoint(pto), apoint(ptx));
	double angle = acos(O) / 2 + start_angle;
	return angle;
}

bool SelectEntity(AcDbObjectId &id, const TCHAR *way)
{
	ads_name ssName; AcDbObjectIdArray IdArr;
	if(RTNORM != acedSSGet(way, NULL, NULL, NULL, ssName))	return false;
	SSToIdArr(ssName, IdArr);
	if(IdArr.length() != 1)
	{
		acutPrintf(_T("\n 请选择单个实体"));
		return false;
	}
	id = IdArr.first();
	return true;
}

bool SelectEntity(AcDbObjectId &id, const SelectFilter &filter, const TCHAR *way)
{
	resbuf *rc = acutBuildList(filter.type, filter.value, RTNONE);
	ads_name ssName; AcDbObjectIdArray ids;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	SSToIdArr(ssName, ids);
	if(ids.length() != 1)
	{
		acutPrintf(_T("\n 请选择单个实体"));
		return false;
	}
	id = ids.first(); return true;
}

bool SelectEntity(AcDbObjectId &id, const SelectFilter &f1, const SelectFilter &f2, const TCHAR *way)
{
	resbuf *rc = acutBuildList(f1.type, f1.value, f2.type, f2.value, RTNONE);
	ads_name ssName; AcDbObjectIdArray ids;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	SSToIdArr(ssName, ids);
	if(ids.length() != 1)
	{
		acutPrintf(_T("\n 请选择单个实体"));
		return false;
	}
	id = ids.first(); return true;
}

bool SelectEntity(AcDbObjectId &id, const SelectFilter &f1, const SelectFilter &f2, const SelectFilter &f3, const TCHAR *way)
{
	resbuf *rc = acutBuildList(f1.type, f1.value, f2.type, f2.value, f3.type, f3.value, RTNONE);
	ads_name ssName; AcDbObjectIdArray ids;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	SSToIdArr(ssName, ids);
	if(ids.length() != 1)
	{
		acutPrintf(_T("\n 请选择单个实体"));
		return false;
	}
	id = ids.first(); return true;
}

bool SelectEntitys(AcDbObjectIdArray &ids, const TCHAR *way)
{
	ads_name ssName;
	if(RTNORM != acedSSGet(way, NULL, NULL, NULL, ssName)) return false;
	SSToIdArr(ssName, ids);
	return ids.length() != 0;
}

bool SelectEntitys(AcDbObjectIdArray &ids, const SelectFilter &filter, const TCHAR *way)
{
	resbuf *rc = acutBuildList(filter.type, filter.value, RTNONE);
	ads_name ssName;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	SSToIdArr(ssName, ids);
	return ids.length() != 0;
}

bool SelectEntitys(AcDbObjectIdArray &ids, const SelectFilter &f1, const SelectFilter &f2, const TCHAR *way)
{
	resbuf *rc = acutBuildList(f1.type, f1.value, f2.type, f2.value, RTNONE);
	ads_name ssName;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	SSToIdArr(ssName, ids);
	return ids.length() != 0;
}

bool SelectEntitysOr(AcDbObjectIdArray &ids, const SelectFilter &f1, const SelectFilter &f2, const TCHAR *way)
{
	resbuf *rc = acutBuildList(-4, _T("<or"), f1.type, f1.value, f2.type, f2.value, -4, _T("or>"), 0);
	ads_name ssName;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	//AcDbObjectIdArray tids;
	if (ids.isEmpty() != true)ids.removeAll();
	SSToIdArr(ssName, ids);
	//if(tids.length() != 0)
	//	CopyEntitys(tids, ids);
	return !ids.isEmpty();
}

bool SelectEntitysOr(AcDbObjectIdArray &ids, const SelectFilter &f1, const SelectFilter &f2, const SelectFilter &f3, const TCHAR *way)
{
	resbuf *rc = acutBuildList(-4, _T("<or"), f1.type, f1.value, f2.type, f2.value, f3.type, f3.value, -4, _T("or>"), RTNONE);
	ads_name ssName;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	SSToIdArr(ssName, ids);
	return ids.length() != 0;
}

bool SelectEntitys(AcDbObjectIdArray &ids, const SelectFilter &f1, const SelectFilter &f2, const SelectFilter &f3, const TCHAR *way)
{
	resbuf *rc = acutBuildList(RTDXF0, f1.type, f1.value, f2.type, f2.value, f3.type, f3.value, 0);
	ads_name ssName;
	if(RTNORM != acedSSGet(way, NULL, NULL, rc, ssName))
	{
		ads_relrb(rc);
		return false;
	}
	ads_relrb(rc);
	SSToIdArr(ssName, ids);
	return ids.length() != 0;
}

CString GetHandleByObject(const AcDbObjectId &id, bool)
{
	TCHAR handle[20] = {0};
	AcDbHandle hand = id.handle();
	hand.getIntoAsciiBuffer(handle);
	CString ret(handle);
	return ret;
}

AcDbObjectId GetObjectByHandle(const TCHAR *handle)
{
	AcDbObjectId id = AcDbObjectId::kNull;
	ads_name ent;
	int nRes = acdbHandEnt(handle, ent);
	if(nRes != RTNORM) return AcDbObjectId::kNull;
	if(Acad::eOk != acdbGetObjectId(id, ent))
	{
		id = AcDbObjectId::kNull;
	}
	return id;
}

bool GetTableEntFromID(const AcDbObjectId &id, AcDbTable *&pTable)
{
	AcDbEntity *p_ent = NULL;
	if(Acad::eOk != acdbOpenObject(p_ent, id, AcDb::kForWrite))
	{
		return false;
	}
	pTable = AcDbTable::cast(p_ent);
	if(pTable == NULL)
	{
		return false;
	}
	return true;
}

void SetCadTableAlignCenter(AcDbTable* p_table)
{
	int rows = p_table->numRows();
	int cols = p_table->numColumns();
	int i = 0, j = 0;
	for(i = 0; i < rows; ++i)
	{
		for(j = 0; j < cols; ++j)
		{
			p_table->setAlignment(i, j, AcDb::kMiddleCenter);
		}
	}
}

AcDbObjectId DrawHatch_Auto(ads_name ent, TCHAR * szHatch, double dScale, double dAngle, AcDbObjectIdArray *pislandIds, int nColor, TCHAR * szLayer)
{
	AcDbObjectId objNew = DrawHatch(ent, szHatch, dScale, dAngle, pislandIds, nColor, szLayer);
	if(objNew == AcDbObjectId::kNull)
	{
		//防止因为填充密度过大，或者实体太小导致的填充失败,对比例不断缩小
		double dScaleIn = dScale;
		dScaleIn /= 2.0;
		objNew = DrawHatch(ent, szHatch, dScaleIn, dAngle, pislandIds, nColor, szLayer);
		int num = 0;
		while(!objNew)
		{
			dScaleIn /= 2;
			objNew = DrawHatch(ent, szHatch, dScaleIn, dAngle, pislandIds, nColor, szLayer);
			num++;
			if(num >= 10)
				break;
		}
		if(objNew == AcDbObjectId::kNull)
		{
			double dScaleOut = dScale;
			dScaleOut /= 2.0;
			objNew = DrawHatch(ent, szHatch, dScaleOut, dAngle, pislandIds, nColor, szLayer);
			int num = 0;
			while(!objNew)
			{
				dScaleOut *= 2;
				objNew = DrawHatch(ent, szHatch, dScaleOut, dAngle, pislandIds, nColor, szLayer);
				num++;
				if(num >= 10)
					break;
			}
		}

		return objNew;

	}
	else
		return objNew;

}

AcDbObjectId DrawHatch(ads_name ent, TCHAR * szHatch, double dScale, double dAngle, AcDbObjectIdArray *pislandIds, int nColor, TCHAR * szLayer)
{
	TCHAR szLayer2[255] = {0};
	GetEntLayer(ent, szLayer2);
	Acad::ErrorStatus es;

	AcDbHatch* pHatch = new AcDbHatch();

	// Set hatch plane
	//
	AcGeVector3d normal(0.0, 0.0, 1.0);
	es = pHatch->setNormal(normal);
	es = pHatch->setElevation(0.0);

	// Set associative hatch
	//
	es = pHatch->setAssociative(Adesk::kTrue);

	// Set hatch pattern
	//
	//2009.1.7  ZJ 修改填充比例不正确的错误
	es = pHatch->setPatternScale(dScale);
	es = pHatch->setPatternAngle(dAngle);
	es = pHatch->setPattern(AcDbHatch::kPreDefined, szHatch);
	//	pHatch->evaluateHatch(true);
	es = pHatch->setColorIndex(nColor);
	// Set hatch style to kNormal
	//
	es = pHatch->setHatchStyle(AcDbHatch::kNormal);

	// Append an external loop (rectangle) to hatch boundary

	AcDbObjectIdArray idArr;
	idArr.append(aname(ent));
	es = pHatch->appendLoop(AcDbHatch::kExternal, idArr);

	// 添加内部岛实体
	//2009.4.15 ZJ 修改添加岛的算法,
	if(pislandIds != NULL)
	{
		es = pHatch->appendLoop(AcDbHatch::kDefault, *pislandIds);
	}

	// Elaborate solid fill
	//
	es = pHatch->evaluateHatch();
	if(NULL != szLayer)
	{
		if(0 != _tcslen(szLayer))
		{
			AcDbLayerTable *pLayerTbl;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTbl, AcDb::kForWrite);
			if(!pLayerTbl->has(szLayer))
			{
				AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
				pLayerTblRcd->setName(szLayer);
				pLayerTbl->add(pLayerTblRcd);
				pLayerTblRcd->close();
			}
			pLayerTbl->close();
			pHatch->setLayer(szLayer);
		}

	}
	else
	{
		pHatch->setLayer(szLayer2);
	}
	// Post hatch entity to database
	//
	AcDbObjectId newId;
	AppendEntity(pHatch, newId);
	pHatch->close();

	//关联
	AcDbEntity *pEnt;


	ads_name entT;
	acdbGetAdsName(entT, newId);

	acedSSSetFirst(NULL, NULL);

	acedCommand(RTSTR, _T("draworder"), RTENAME, entT, RTSTR, _T(""), RTSTR, _T("B"), 0);

	if(NULL != pislandIds)
	{
		pislandIds->append(aname(ent));
		for(int i = 0; i < pislandIds->length(); i++)
		{
			es = acdbOpenAcDbEntity(pEnt, pislandIds->at(i), AcDb::kForWrite);
			if(es == Acad::eOk)
			{
				pEnt->addPersistentReactor(newId);
				pEnt->close();
			}
		}

		pislandIds->removeLast();
	}
	return newId;

}

bool PointAtLine(AcGePoint2d pi, AcGePoint2d pj, AcGePoint2d pk)
{
	if(min(pi.x, pj.x) <= pk.x&&pk.x <= max(pi.x, pj.x))
	{
		if(min(pi.y, pj.y) <= pk.y&&pk.y <= max(pi.y, pj.y))
		{
			return true;
		}
	}
	return false;
}

double direction(AcGePoint2d pi, AcGePoint2d pj, AcGePoint2d pk)
{
	return (pi.x - pk.x)*(pi.y - pj.y) - (pi.y - pk.y)*(pi.x - pj.x);
}

bool judgeIntersection(AcGePoint2d p1, AcGePoint2d p2, AcGePoint2d p3, AcGePoint2d p4)
{
	double d1 = direction(p3, p4, p1);
	double d2 = direction(p3, p4, p2);
	double d3 = direction(p1, p2, p3);
	double d4 = direction(p1, p2, p4);
	if(d1*d2 < 0 && d3*d4 < 0)
		return true;
	if(d1 == 0 && PointAtLine(p3, p4, p1))
		return true;
	if(d2 == 0 && PointAtLine(p3, p4, p2))
		return true;
	if(d3 == 0 && PointAtLine(p1, p2, p3))
		return true;
	if(d4 == 0 && PointAtLine(p1, p2, p4))
		return true;
	return false;
}

AcGePoint2dArray getOneStraightSideOfEnt(AcDbObjectId &outId)
{
	acutPrintf(_T("请选择一条边"));
	ads_point pt1, pt2;
	if(RTCAN == ads_getpoint(NULL, _T("\n 请指定范围线左下角"), pt1)) return AcGePoint2dArray();
	if(RTCAN == ads_getcorner(pt1, _T("\n 请指定范围线右上角"), pt2)) return AcGePoint2dArray();

	SNodesOpe nodesO;
	nodesO.modifyLB_RTPoint(pt1, pt2);
	AcGePoint2dArray nodes;
	nodesO.LB_RT_Nodes(apoint(pt1), apoint(pt2), nodes);

	if(nodes.length() != 4)
		return AcGePoint2dArray();

	ads_name ssNames;	ssFromNodes(nodes, ssNames, _T("CP"));
	AcDbObjectIdArray ids;
	SSToIdArr(ssNames, ids);

	if(ids.length() > 1)
	{
		acutPrintf(_T("只能选择一条边！"));
		return AcGePoint2dArray();
	}
	if(ids.length() == 0)
	{
		acutPrintf(_T("没有边被选中！"));
		return AcGePoint2dArray();
	}

	outId = ids[0];

	AcGePoint2dArray PtArray, interArray;
	AcGeDoubleArray interParam;

	GetPlList(aname(ids[0]), PtArray);

	AcArray<int> lineIndexes;
	for(int i = 0; i < PtArray.logicalLength() - 1; i++)
	{
		AcGePoint2d spt, ept;
		spt = PtArray[i];
		ept = PtArray[i + 1];
		interArray.removeAll();
		bool isIntersected = judgeIntersection(AcGePoint2d(nodes[0][0], nodes[0][1]), AcGePoint2d(nodes[2][0], nodes[2][1]), spt, ept);
		if(!isIntersected)
		{
			isIntersected = judgeIntersection(AcGePoint2d(nodes[1][0], nodes[1][1]), AcGePoint2d(nodes[3][0], nodes[3][1]), spt, ept);
		}
		if(isIntersected)
		{
			lineIndexes.append(i);
		}
	}

	if(lineIndexes.logicalLength() > 1)
	{
		acutPrintf(_T("只能选择一条边！"));
		return AcGePoint2dArray();
	}
	if(lineIndexes.logicalLength() < 1)
	{
		acutPrintf(_T("没有边被选中！"));
		return AcGePoint2dArray();
	}
	AcGePoint2dArray outarray;
	AcGePoint2d pts = PtArray[lineIndexes[0]];
	AcGePoint2d pte = PtArray[lineIndexes[0] + 1];
	outarray.append(pts);
	outarray.append(pte);
	return outarray;
}

AcDbObjectId drawSingleAxisAnno(AcGePoint2d spt, CString annoText, double annoAngle)
{
	double scale = 500; read1(scale);
	double font = scale / 1000;
	double annoLength = 5 * font;
	ads_point endAnno, textPt;
	ads_polar(apoint(spt), annoAngle, annoLength, endAnno);
	ads_polar(endAnno, annoAngle, 2 * font, textPt);
	AcDbObjectId lineID = drawline(apoint(spt), endAnno, _T(""));
	setlayer(aname(lineID), _T("标注")); AcDbObjectId group;
	setlinetype(aname(lineID), _T("6"));
	CreateGroup(aname(lineID), group);
	AcDbObjectId textID = DrawText(textPt, annoText, _T("MC"), _T("20000"), font);
	AddEntToGroup(textID, group); setlayer(aname(textID), _T("标注"));
	AcDbObjectId circleID = DrawCircle(apoint(textPt), 2 * font, _T(""));
	AddEntToGroup(circleID, group); setlayer(aname(textID), _T("标注"));
	setcolor(aname(lineID), 4);	setcolor(aname(textID), 4);
	setcolor(aname(circleID), 4);
	return group;
}

AcDbObjectId drawSingleAxisAnno(AcGePoint2d spt, CString annoText, double annoAngle, const TextProperty &zxbz, const TextProperty &zxbzy, const TextProperty &zxbzzx)
{
	double scale = 500; read1(scale);
	double font = scale / 1000;
	double annoLength = zxbzzx._size * font;
	ads_point endAnno, textPt;
	ads_polar(apoint(spt), annoAngle, annoLength, endAnno);
	ads_polar(endAnno, annoAngle, zxbzy._size * font, textPt);
	AcDbObjectId lineID = drawline(apoint(spt), endAnno, _T(""));
	setlayer(aname(lineID), zxbzzx._layer);  setcolor(aname(lineID), zxbzzx._color);
	setlinetype(aname(lineID), _T("6")); AcDbObjectId group;
	CreateGroup(aname(lineID), group);
	AcDbObjectId textID = DrawText(textPt, annoText, _T("MC"), _T("20000"), font * zxbz._size);
	setlayer(aname(textID), zxbz._layer); setcolor(aname(textID), zxbz._color);
	AddEntToGroup(textID, group);
	AcDbObjectId circleID = DrawCircle(apoint(textPt), zxbzy._size * font, _T(""));
	setlayer(aname(circleID), zxbzy._layer); setcolor(aname(circleID), zxbzy._color);
	AddEntToGroup(circleID, group); return group;
}

AcGePoint2d getProjectedPointOnLine(AcGePoint2d linept, double lineAngle, AcGePoint2d inPt)
{
	double anglePts = ads_angle(apoint(linept), apoint(inPt));
	double lengthPts = distOF2d(linept, inPt);
	double projectedLength = lengthPts * cos(lineAngle - anglePts);
	ads_point outpt;
	ads_polar(apoint(linept), lineAngle, projectedLength, outpt);

	return apoint(outpt);

}

//修改块参照的颜色
void ChangeBlockRefenceColor(AcDbObjectId ObjId, int nRed, int nGreen, int nBlue)
{
	AcDbEntity * pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, ObjId, AcDb::kForWrite);
	if(es != Acad::eOk)
		return;
	if(pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference * pBlockRef;
		AcDbObjectId id;
		pBlockRef = AcDbBlockReference::cast(pEnt);
		//遍历块参照里面的实体
		AcDbBlockTableRecordIterator *iter;
		AcDbBlockTableRecord *pRecord;
		id = pBlockRef->blockTableRecord();
		pEnt->close();
		es = acdbOpenObject(pRecord, id, AcDb::kForRead);
		pRecord->newIterator(iter);
		pRecord->close();
		for(iter->start(); !iter->done(); iter->step())
		{
			AcDbEntity * pent;
			iter->getEntity(pent, AcDb::kForWrite);
			//判断是不是块参照
			if(pent->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef;
				pRef = AcDbBlockReference::cast(pent);
				AcDbBlockTableRecordIterator * iter1;
				AcDbBlockTableRecord * precord;
				AcDbObjectId id1;
				id1 = pRef->blockTableRecord();
				pent->close();
				es = acdbOpenObject(precord, id1, AcDb::kForRead);
				precord->newIterator(iter1);
				precord->close();
				for(iter1->start(); !iter1->done(); iter1->step())
				{
					AcDbEntity *pEntity;
					iter1->getEntity(pEntity, AcDb::kForWrite);
					AcCmColor color;
					color.setColorMethod(AcCmEntityColor::kByBlock);
					color.setRGB(nRed, nGreen, nBlue);
					pEntity->setColor(color);
					pEntity->close();
				}
				delete iter1;
			}
			else
			{
				AcCmColor color;
				color.setRGB(nRed, nGreen, nBlue);
				pent->setColor(color);
				pent->close();
			}
		}
		delete iter;
	}
	else
	{
		AcCmColor color;
		color.setColorMethod(AcCmEntityColor::kByBlock);
		color.setRGB(nRed, nGreen, nBlue);
		pEnt->setColor(color);
		pEnt->close();
	}
}

/*修改块参照的颜色(coloIndex)*/
void ChangeBlockRefenceColor(AcDbObjectId ObjId, int colorIndex)
{
	AcDbEntity * pEnt = NULL;
	Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, ObjId, AcDb::kForWrite);
	if(es != Acad::eOk)
		return;
	if(pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference * pBlockRef;
		AcDbObjectId id;
		pBlockRef = AcDbBlockReference::cast(pEnt);
		//遍历块参照里面的实体
		AcDbBlockTableRecordIterator *iter;
		AcDbBlockTableRecord *pRecord;
		id = pBlockRef->blockTableRecord();
		pEnt->close();
		es = acdbOpenObject(pRecord, id, AcDb::kForRead);
		pRecord->newIterator(iter);
		pRecord->close();
		for(iter->start(); !iter->done(); iter->step())
		{
			AcDbEntity * pent;
			iter->getEntity(pent, AcDb::kForWrite);
			//判断是不是块参照
			if(pent->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef;
				pRef = AcDbBlockReference::cast(pent);
				AcDbBlockTableRecordIterator * iter1;
				AcDbBlockTableRecord * precord;
				AcDbObjectId id1;
				id1 = pRef->blockTableRecord();
				pent->close();
				es = acdbOpenObject(precord, id1, AcDb::kForRead);
				precord->newIterator(iter1);
				precord->close();
				for(iter1->start(); !iter1->done(); iter1->step())
				{
					AcDbEntity *pEntity;
					iter1->getEntity(pEntity, AcDb::kForWrite);
					pEntity->setColorIndex(colorIndex);
					pEntity->close();
				}
				delete iter1;
			}
			else
			{
				pent->setColorIndex(colorIndex);
				pent->close();
			}
		}
		delete iter;
	}
	else
	{
		pEnt->setColorIndex(colorIndex);
		pEnt->close();
	}
}

void DrawCadTable(const AcGePoint3d InsertPt, AcDbObjectId &TblId, const int nRows, const int nCols, const double dWidth, const double dHeight, bool bAutoScale, vector<vector<CString>> _content, double textHight)
{
#if _MSC_VER >= 1300
	AcDbTable *pTable = new AcDbTable();
	Acad::ErrorStatus es;
	AcDbDictionary *pDict = NULL;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	es = pDb->getTableStyleDictionary(pDict, AcDb::kForRead);
	AcDbObjectId styleId;
	es = pDict->getAt(_T("Standard"), styleId);
	es = pDict->close();
	pTable->setTableStyle(styleId);
	pTable->setNumRows(nRows);
	pTable->setNumColumns(nCols);
	pTable->suppressTitleRow(true);
	pTable->suppressHeaderRow(true);
	SetCadTableAlignCenter(pTable);	// 上面那句不起作用，所以用这个
	pTable->unmergeCells(0, 0, 0, nCols - 1);
	pTable->generateLayout();
	pTable->setRowHeight(dHeight);
	pTable->setColumnWidth(dWidth);
	pTable->setPosition(InsertPt);
	pTable->setTextHeight(textHight);

	if(_content.size() == nRows)
	{
		for(int row_count = 0; row_count < nRows; row_count++)
		{
			vector<CString> row_content = _content.at(row_count);
			if(row_content.size() == nCols)
			{
				for(int column_count = 0; column_count < nCols; column_count++)
				{
					pTable->setTextString(row_count, column_count, row_content.at(column_count));
				}
			}
		}
	}

	int i = 0, j = 0;
	for(i = 0; i < nRows; ++i)
	{
		for(j = 0; j < nCols; ++j)
		{
			pTable->setAutoScale(i, j, bAutoScale);
		}
	}

	AcDbBlockTable *pBlockTable;
	pDb->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();
	pBlockTableRecord->appendAcDbEntity(pTable);
	TblId = pTable->objectId();
	pTable->close();
	pBlockTableRecord->close();
#else
#pragma message(_T("警告：VC6.0下函数 DrawTable 无效"))
#endif

}

void DrawCadTable(const AcGePoint3d InsertPt, AcDbObjectId &TblId, const int nRows, const int nCols, const double dWidth, const double dHeight, bool bAutoScale)
{
#if _MSC_VER >= 1300
	AcDbTable *pTable = new AcDbTable();
	Acad::ErrorStatus es;
	AcDbDictionary *pDict = NULL;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	es = pDb->getTableStyleDictionary(pDict, AcDb::kForRead);
	AcDbObjectId styleId;
	es = pDict->getAt(_T("Standard"), styleId);
	es = pDict->close();
	pTable->setTableStyle(styleId);
	pTable->setNumRows(nRows);
	pTable->setNumColumns(nCols);
	pTable->suppressTitleRow(true);
	pTable->suppressHeaderRow(true);
	//pTable->setAlignment(AcDb::CellAlignment::kMiddleCenter, AcDb::kAllRows);
	SetCadTableAlignCenter(pTable);	// 上面那句不起作用，所以用这个
	pTable->unmergeCells(0, 0, 0, nCols - 1);
	pTable->generateLayout();
	pTable->setRowHeight(dHeight);
	pTable->setColumnWidth(dWidth);
	pTable->setPosition(InsertPt);

	AcDbBlockTable *pBlockTable;
	pDb->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt
		(
		ACDB_MODEL_SPACE,
		pBlockTableRecord,
		AcDb::kForWrite
		);
	pBlockTable->close();
	pBlockTableRecord->appendAcDbEntity(pTable);
	TblId = pTable->objectId();
	pTable->close();
	pBlockTableRecord->close();
#else
#pragma message(_T("警告：VC6.0下函数 DrawTable 无效"))
#endif

}

/**
*  @date        2013/11/15 11:56
*  @brief       画cad表格
*  @param[in]   InsertPt  插入点
*  @param[out]  TblId     表格ObjectId
*  @param[in]   nRows     表格行数
*  @param[in]   nCols     表格列数
*  @param[in]   dWidth    表格单元格宽度
*  @param[in]   dHeight   表格单元格高度
*  @param[in]   bAutoScale是否自动缩放单元格
*  @param[in]   szTxtStyle表格字体
*  @param[in]   dTxtHeight字体高度
*/
void DrawTable(const AcGePoint2d InsertPt, AcDbObjectId &TblId, const int nRows, const int nCols,
	const double dWidth, const double dHeight, const bool bAutoScale,
	const TCHAR *szTxtStyle, const double dTxtHeight)
{
#if _MSC_VER >= 1300
	AcDbTable *pTable = new AcDbTable();
	Acad::ErrorStatus es;
	AcDbDictionary *pDict = NULL;
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	es = pDb->getTableStyleDictionary(pDict, AcDb::kForRead);
	AcDbObjectId styleId;
	es = pDict->getAt(_T("Standard"), styleId);
	es = pDict->close();
	pTable->setTableStyle(styleId);
	pTable->setNumRows(nRows);
	pTable->setNumColumns(nCols);
	AcDbObjectId TxtStyleId;
	if(GetTextStyleId(szTxtStyle, TxtStyleId))
	{
		pTable->setTextStyle(TxtStyleId);
	}
	pTable->setTextHeight(dTxtHeight);

	pTable->suppressTitleRow(true);
	pTable->suppressHeaderRow(true);
	pTable->unmergeCells(0, 0, 0, nCols - 1);
	pTable->generateLayout();
	pTable->setRowHeight(dHeight);
	pTable->setColumnWidth(dWidth);
	pTable->setPosition(apoint(InsertPt));
	int i = 0, j = 0;
	for(i = 0; i < nRows; ++i)
	{
		for(j = 0; j < nCols; ++j)
		{
			pTable->setAutoScale(i, j, bAutoScale);
		}
	}
	AcDbBlockTable *pBlockTable;
	pDb->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt
		(
		ACDB_MODEL_SPACE,
		pBlockTableRecord,
		AcDb::kForWrite
		);
	pBlockTable->close();
	pBlockTableRecord->appendAcDbEntity(pTable);
	TblId = pTable->objectId();
	pTable->close();
	pBlockTableRecord->close();
#else
#pragma message( "警告：VC6.0下函数 DrawTable 无效" )
#endif
}

CString GetHandleByObject(const AcDbObjectId &id)
{
	return CString(GetHandleByObject(id, true));
}

AcDbObjectId GetObjectByHandle(const CString &handle)
{
	int len = handle.GetLength();
	TCHAR *hand = new TCHAR[len];
	_stprintf(hand, _T("%s"), handle);
	AcDbObjectId id = GetObjectByHandle(hand);
	delete hand; hand = NULL;
	return id;
}

AcGePoint2d GetArrowEndPoint(const AcDbObjectId &group)
{
	AcDbObjectIdArray ids;
	getEntsByGroupId(group, ids);
	int len = ids.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = ids.at(idx);
	}
	return AcGePoint2d(0, 0);
}

void printErrorFunction(const char *fun)
{
	MessageBoxA(GetForegroundWindow(), fun, "错误函数", MB_OK);
}

void printErrorInfo(const char *file, const char *fun, int len)
{
	char info[1000] = {0};
	sprintf(info, "出错文件：%s \n 出错函数为：%s；该地方在第%d行", file, fun, len);
	MessageBoxA(GetForegroundWindow(), info, "错误信息", MB_OK);
}

void printErrorInfo(const char *file, const char *fun, int len, const char *error)
{
	char info[1000] = {0};
	sprintf(info, "出错文件：%s \n 出错函数为：%s；该地方在第%d行\n。错误信息为：%s !", file, fun, len, error);
	MessageBoxA(GetForegroundWindow(), info, "错误信息", MB_OK);
}

bool OpenDWG(CString strPath)
{
	if(strPath.GetLength() <= 0) return false;
	AcApDocManager *pDocMgr = acDocManagerPtr();
	AcApDocumentIterator *pDocItr;
	AcApDocument *pMainDoc = NULL;
	pDocItr = pDocMgr->newAcApDocumentIterator();
	for(; !pDocItr->done(); pDocItr->step())
	{
		AcApDocument * pDoc = pDocItr->document();
		TCHAR szName[255] = {0};
		_stprintf(szName, _T("%s"), pDoc->fileName());
		if(_tcsicmp(szName, strPath) == 0)
		{
			pMainDoc = pDoc;
			break;
		}
	}
	delete pDocItr;
	if(pMainDoc != NULL)
	{
		acDocManager->setCurDocument(pMainDoc);
		acDocManager->activateDocument(pMainDoc);
	}
	else
	{
		acDocManager->appContextOpenDocument(strPath.GetBuffer());
		strPath.ReleaseBuffer();
	}
	return true;
}

bool saveCurDwg(const TCHAR *pathname)
{
	int es = acDocManager->mdiActiveDocument()->database()->save();
	if(es == Acad::eOk)
		return true;
	return false;
}

void openDwgCommand(const TCHAR *dwgpath)
{
	TCHAR curPath[255] = {0};
	_stprintf(curPath, _T("%s"), GetCurrentDwgFullPath());
	saveCurDwg(curPath);
	SendCommand(_T("OpenDwgCmd "));
	SendCommand(dwgpath);
	SendCommand(_T(" "));
}

void sendCommand(const TCHAR *szCommand)
{
	COPYDATASTRUCT msg;
	msg.dwData = (DWORD)1;
	msg.cbData = (DWORD)(_tcslen(szCommand) + 1)*sizeof(TCHAR);
	msg.lpData = (TCHAR*)szCommand;
	acedGetAcadFrame()->SendMessage(WM_COPYDATA, (WPARAM)adsw_acadMainWnd(), (LPARAM)&msg);
	acedGetAcadDockCmdLine()->SetFocus();
}

IDataBaseOper::IDataBaseOper()
{
	try
	{
		_db = new CDatabaseOperManager;
		TCHAR mdbpath[256] = {0};
		if(RTNORM != ads_findfile(_T("xmjg.mdb"), mdbpath))
		{
			AfxMessageBox(_T("数据库文件xmjg.mdb无法找到"));
			return;
		}
		if(true == _db->initialConnect(mdbpath)) return;
		acutPrintf(_T("\n 数据库初始化失败！！！"));
		printErrorInfo(__FILE__, __FUNCTION__, __LINE__);
		return;
	}
	catch(...)
	{
		printErrorInfo(__FILE__, __FUNCTION__, __LINE__);
	}
}

IDataBaseOper::~IDataBaseOper()
{
	if(_db == NULL) return;
	_db->close();
	delete _db;
	_db = NULL;
}

long IDataBaseOper::readJRStartTime()
{
	CString value = readCommonTable(_T("建筑竣工计容起算时间"));
	return TimeConvertLong(value);
}

CString IDataBaseOper::readJGCommon(const CString &field)
{
	MStr filter;
	filter[_T("关键字")] = field;
	VMStr records; CString value;
	if(false == _db->ReadDataBase(JGCOMMONTABLE, records, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		ErrorInfo();
		return value;
	}
	return records.at(0)[_T("属性值")];
}

bool IDataBaseOper::readJGCommon(const CString &field, long &value)
{
	CString prop = readJGCommon(field);
	if(prop.GetLength() == 0) return false;
	value = _ttol(prop); return true;
}

bool IDataBaseOper::readJGCommon(const CString &field, float &value)
{
	CString prop = readJGCommon(field);
	if(prop.GetLength() == 0) return false;
	value = (float)_tstof(prop); return true;
}

bool IDataBaseOper::readDataBase(const MStr &filter, MStr &value)
{
	VMStr record;
	if(false == _db->ReadDataBase(_T("XMJGProjectInfo"), record, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		return false;
	}
	if(record.size() != 0)
	{
		value = record[0];
		return true;
	}
	AfxMessageBox(_T("读取数据库失败！！！"));
	ErrorInfo();
	return false;
}

bool IDataBaseOper::updataDataBase(const MStr &fv, const MStr &record)
{
	if(true == _db->UpdataRecord(_T("XMJGProjectInfo"), fv, record))
		return true;
	AfxMessageBox(_T("更新数据库失败！！！"));
	ErrorInfo();
	return false;
}

bool IDataBaseOper::readDataBase(const CString &table, const MStr &filter, MStr &value)
{
	VMStr record;
	if(false == _db->ReadDataBase(table, record, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		ErrorInfo();
		return false;
	}
	if(record.size() != 0)
	{
		value = record[0];
		return true;
	}
	AfxMessageBox(_T("读取数据库失败！！！"));
	ErrorInfo();
	return false;
}

bool IDataBaseOper::readDataBase(const CString &table, const MStr &filter, VMStr &values)
{
	if(false == _db->ReadDataBase(table, values, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		ErrorInfo();
		return false;
	}
	return true;
}

bool IDataBaseOper::readDataBase(const CString &table, const CString &field, CString &value)
{
	VStr values;
	if(false == _db->GetValueByField(table, field, values)) return false;
	if(values.size() == 0) return false;
	value = value[0];
	return true;
}

bool IDataBaseOper::readDataBase(const CString &table, const CString &field, const MStr &filter, VStr &value)
{
	if(false == _db->GetValueByField(table, field, value, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		ErrorInfo();
		return false;
	};
	return true;
}

bool IDataBaseOper::readDataBase(const CString &table, const CString &field, const MStr &filter, CString &value)
{
	MStr record;
	if(false == readDataBase(table, filter, record)) return false;
	if(record.size() == 0) return false;
	value = record[field];
	return true;
}

CString IDataBaseOper::readCommonTable(const CString &field)
{
	MStr filter;
	filter[_T("关键字")] = field;
	VMStr records; CString value;
	if(false == _db->ReadDataBase(COMMONTABLE, records, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		ErrorInfo();
		return value;
	}
	return records.at(0)[_T("属性值")];
}

bool IDataBaseOper::readCommonTable(const CString &key, MStr &value)
{
	MStr filter;
	filter[_T("关键字")] = key;
	VMStr records;
	if(false == _db->ReadDataBase(COMMONTABLE, records, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		ErrorInfo();
		return false;
	}
	int len = (int)records.size(); CString k, v;
	for(int idx = 0; idx < len; ++idx)
	{
		MStr record = records.at(idx);
		k = record[_T("属性值")];
		v = record[_T("属性值1")];
		value[k] = v;
	}
	return true;
}

bool IDataBaseOper::readCommonTable(const CString &key, CString &value)
{
	MStr filter;
	filter[_T("关键字")] = key;
	VMStr records;
	if(false == _db->ReadDataBase(COMMONTABLE, records, filter))
	{
		AfxMessageBox(_T("读取数据库失败！！！"));
		ErrorInfo();
		return false;
	}
	value = records.at(0)[_T("属性值")];
	return true;
}

bool IDataBaseOper::readCommonTable(const CString &key, int &value)
{
	CString val = readCommonTable(key);
	if(val.GetLength() == 0) return false;
	value = _ttoi(val);
	return true;
}

bool IDataBaseOper::readCommonTable(const CString &key, float &value)
{
	CString val = readCommonTable(key);
	if(val.GetLength() == 0) return false;
	value = (float)_tstof(val);
	return true;
}

bool IDataBaseOper::addPictureTableInfo(const CString &savename)
{
	TCHAR info[255] = {0};
	int pos = savename.ReverseFind(_T('\\'));
	pos == -1 ? savename.ReverseFind(_T('/')) : pos;
	CString name = savename.Right(savename.GetLength() - pos - 1);
	pos = name.Find(_T("."));
	CString desc;
	if(pos == -1)
		desc = name + _T(".jpg");
	else
		desc = name;
	MStr record, filter;
	int count = _db->GetTableRecordCount(PICTURETABLE);
	_stprintf(info, _T("%d"), count);
	record[_T("序号")] = info;
	record[_T("名称")] = name;
	record[_T("描述")] = desc;
	record[_T("路径")] = savename + _T(".jpg");
	record[_T("位置")] = _T("");
	return _db->AddOneRecord(PICTURETABLE, record, filter);
}

int IDataBaseOper::getFieldCountByTable(const CString &table)
{
	return _db->GetTableRecordCount(table);
}

bool IDataBaseOper::readProjectInfoTable(const CString &key, MStr &record)
{
	MStr filter;
	filter[_T("业务名称")] = key;
	return readDataBase(PROJECTINFOTABLE, filter, record);
}

bool IDataBaseOper::readCommonTable(const CString &key, const CString &field, CString &value)
{
	return readDataBase(COMMONTABLE, field, value);
}

bool IDataBaseOper::writeCommonTable(const CString &key, int &val)
{
	if(false == _db->DeleteDataBase(COMMONTABLE, _T("关键字"), key))
	{
		MessageBox(GetForegroundWindow(), _T("更新表失败！！！"), _T("错误提示"), MB_OK);
		return false;
	}
	CString value; value.Format(_T("%d"), val);
	MStr record; record[_T("关键字")] = key; record[_T("属性值")] = value;
	return _db->InsertRecord(COMMONTABLE, record);
}

bool IDataBaseOper::writeCommonTable(const CString &key, float &val)
{
	if(false == _db->DeleteDataBase(COMMONTABLE, _T("关键字"), key))
	{
		MessageBox(GetForegroundWindow(), _T("更新表失败！！！"), _T("错误提示"), MB_OK);
		return false;
	}
	CString value; value.Format(_T("%.3f"), val);
	MStr record; record[_T("关键字")] = key; record[_T("属性值")] = value;
	return _db->InsertRecord(COMMONTABLE, record);
}

bool IDataBaseOper::writeCommonTable(const CString &key, const CString &val)
{
	if(false == _db->DeleteDataBase(COMMONTABLE, _T("关键字"), key))
	{
		MessageBox(GetForegroundWindow(), _T("更新表失败！！！"), _T("错误提示"), MB_OK);
		return false;
	}
	MStr record; record[_T("关键字")] = key; record[_T("属性值")] = val;
	return _db->InsertRecord(COMMONTABLE, record);
}

CString IDataBaseOper::readPathTable(const CString &field)
{
	MStr filter, record; CString value;
	filter[_T("关键字")] = field;
	if(false == readDataBase(XMJGPATHTABLE, filter, record)) return value;
	if(record.size() == 0) return value;
	value = record[_T("属性值")];
	return value;
}

CString IDataBaseOper::readPathTableEx(const CString &name)
{
	if(0 == name.CompareNoCase(_T("当前目录")))
		return readPathTable(name);
	CString path = readPathTable(_T("工程目录"));
	CString temp = readPathTable(name);
	CString dir(path);
	dir += name + _T("\\") + temp;
	return dir;
}

CString IDataBaseOper::readFileNameTable(const CString &temp)
{
	MStr filter; filter[_T("项目名称")] = temp; CString value;
	RETURNFLAG(readDataBase(FILENAMETABLE, _T("文件名称"), filter, value), value);
	return value;
}

void IDataBaseOper::readFunctionTable(FunctionNameTables &fnts)
{
	VMStr records;
	_db->ReadDataBase_all(_T("XMJGFunction"), records);
	int size = (int)records.size();
	for(int idx = 0; idx < size; ++idx)
	{
		MStr record = records.at(idx);
		MSIter it = record.begin();
		FunctionNameTable fnt;
		fnt.name = it->second; it++;
		fnt.mjxs = it->second; it++;
		fnt.jrxs = it->second;
		fnts.push_back(fnt);
	}
}

void IDataBaseOper::saveFunctionTable(const FunctionNameTables &fnts)
{
	_db->clearTable(_T("XMJGFunction"));
	int len = (int)fnts.size();
	MStr record;
	for(int idx = 0; idx < len; ++idx)
	{
		FunctionNameTable fnt = fnts.at(idx);
		record[_T("功能区名称")] = fnt.name;
		record[_T("面积系数")] = fnt.mjxs;
		record[_T("计容系数")] = fnt.jrxs;
		_db->InsertRecord(_T("XMJGFunction"), record);
	}
}

void IDataBaseOper::readHeightTable(MVMStr &record)
{
	VMStr records;
	if(_db->ReadDataBase_all(HEIGHTTABLE, records) == false) return;
	int len = (int)records.size();
	for(int idx = 0; idx < len; ++idx)
	{
		MStr values = records.at(idx);
		CString buildname = values[_T("关键字")];
		CString layername = values[_T("属性值")];
		CString heightval = values[_T("附加值")];
		MStr value; value[layername] = heightval;
		if(record.find(buildname) == record.end())
			record[buildname] = VMStr();
		record[buildname].push_back(value);
	}
}

void IDataBaseOper::writeHeightTable(const MVMStr &)
{

}

bool IDataBaseOper::readFieldChangeTable()
{
	VMStr records; g_fieldchagetable.clear();
	if(_db->ReadDataBase(_T("XMJGFieldChangeTable"), records) == false) return false;
	int size = (int)records.size();
	for(int idx = 0; idx < size; ++idx)
	{
		MStr record = records.at(idx);
		g_fieldchagetable.addField(record[_T("关键字")], record[_T("属性值")]);
	}
	return true;
}

bool IDataBaseOper::readCarTable(MStr &record)
{
	VMStr records;
	if(false == _db->ReadDataBase(_T("XMJGCar"), records)) return false;
	int size = (int)records.size();
	for(int idx = 0; idx < size; ++idx)
	{
		MStr temp = records.at(idx);
		record[temp[_T("属性值")]] = temp[_T("附加值")];
	}
	return true;
}

bool IDataBaseOper::readCompressTable(const CString &name, VStr &values)
{
	MStr filter; filter[_T("关键字")] = name;
	if(false == _db->GetValueByField(_T("XMJGCompress"), _T("属性值"), values, filter)) return false;
	return true;
}

TextProperty IDataBaseOper::readTextPropertyTable(const CString &name)
{
	TextProperty prop; MStr record;
	if(false == _db->ReadDataBase(TEXTPROPERTY, name, record))
	{
		CString text; text.Format(_T("在读取%s注记属性时失败！！！"), name);
		AfxMessageBox(text);
		return prop;
	}
	prop._layer = record[_T("图层")];
	prop._font = record[_T("字体")];
	prop._color = _ttoi(record[_T("颜色")]);
	prop._size = (float)_tstof(record[_T("大小")]);
	return prop;
}

TextProperty IDataBaseOper::readTextPropery(const CString &name, bool size, bool font)
{
	TextProperty text;
	if(font == true)
		readCommonTable(name + _T("字体"), text._font);//读取Common表中的“建筑竣工车位注记字体”
	if(size == true)
		readCommonTable(name + _T("大小"), text._size);//读取Common表中的“建筑竣工车位注记大小”
	readCommonTable(name + _T("图层"), text._layer);//读取Common表中的“建筑竣工车位图层”或者“建筑竣工车位注记图层”
	readCommonTable(name + _T("颜色"), text._color);//读取Common表中的“建筑竣工车位颜色”或者“建筑竣工车位注记颜色”
	return text;
}

void IDataBaseOper::readPipelineTable(PipeLineTables &fnts)
{
	VMStr records;
	_db->ReadDataBase_all(_T("PIPELINE"), records);
	int size = (int)records.size();
	for(int idx = 0; idx < size; ++idx)
	{
		MStr record = records.at(idx);
		PipeLineTable fnt;
		fnt.strKey = record[_T("关键字")];
		fnt.strValue = record[_T("属性值")];
		fnt.strExtra = record[_T("附加值")];
		fnt.strGDBM = record[_T("管点编码")];
		fnt.strGXBM = record[_T("管线编码")];
		fnt.strGXTC = record[_T("管线图层")];
		fnt.strGXXX = record[_T("管线线型")];
		fnts.push_back(fnt);
	}
}

void IDataBaseOper::readPipePointBM(const CString &strGDLX, CString &strGDBM)
{
	MStr filter; filter[_T("关键字")] = strGDLX; strGDBM = _T("");
	_db->ReadDataBase(_T("PIPELINE"), _T("管点编码"), filter, strGDBM);
}

bool IDataBaseOper::readPipelineBM(const MStr &filter, CString &strGXDM)
{
	return _db->ReadDataBase(_T("PIPELINE"), _T("管线编码"), filter, strGXDM);
}

bool IDataBaseOper::readPipelineDM(const CString &strGXLX, CString &strGXDM)
{
	MStr filter; strGXDM = _T(""); filter[_T("关键字")] = strGXLX;
	return _db->ReadDataBase(_T("PIPELINE"), _T("管线编码"), filter, strGXDM);
}

void IDataBaseOper::readPipePointKBM(const CString &strGDLX, CString &strKBM)
{
	MStr filter; strKBM = _T(""); filter[_T("关键字")] = strGDLX;
	_db->ReadDataBase(_T("PIPELINE"), _T("块编码"), filter, strKBM);
}

CString IDataBaseOper::readPipeLineTable(const CString &name, const MStr &filter)
{
	CString value;
	_db->ReadDataBase(_T("PIPELINE"), name, filter, value);
	return value;
}

MStr IDataBaseOper::readOutputFormatTable(const CString &name)
{
	VMStr records; MStr filter, ret;
	filter[_T("关键字")] = name;
	_db->ReadDataBase(_T("XMJGExportFormat"), records, filter);
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr temp = records[idx];
		ret[temp[_T("属性值")]] = temp[_T("附加值")];
	}
	return ret;
}

VStr IDataBaseOper::readAllPipeLineLayer()
{
	VMStr records; VStr layers;
	_db->ReadDataBase_all(_T("PIPELINE"), records);
	int size = (int)records.size();
	for(int idx = 0; idx < size; ++idx)
	{
		MStr record = records.at(idx);
		layers.push_back(record[_T("管线图层")]);
	}
	return layers;
}

void IDataBaseOper::savePipelineTable(const PipeLineTables &fnts)
{
	_db->clearTable(_T("PIPELINE"));
	int len = (int)fnts.size();
	MStr record;
	for(int idx = 0; idx < len; ++idx)
	{
		PipeLineTable fnt = fnts.at(idx);
		record[_T("关键字")] = fnt.strKey;
		record[_T("属性值")] = fnt.strValue;
		record[_T("附加值")] = fnt.strExtra;
		_db->InsertRecord(_T("PIPELINE"), record);
	}
}

bool getLayerNum(const CString &info, int &snum, int &ennum)
{
	int pos = info.Find(_T(';'));
	if(pos != -1)
	{
		snum = ennum = 0;
		return true;
	}
	pos = info.Find(_T(' '));
	if(pos == -1)
	{
		ennum = snum = _ttoi(info);
		return true;
	}
	snum = _ttoi(info.Left(pos));
	pos = info.ReverseFind(_T(' '));
	ennum = _ttoi(info.Mid(pos + 1));
	return true;
}

void FieldChangeTable::clear()
{
	m_indexname.clear();
	m_nameindex.clear();
}

bool FieldChangeTable::findName(const CString &name)
{
	return m_nameindex.find(name) != m_nameindex.end();
}

bool FieldChangeTable::findIndex(const CString &index)
{
	return m_indexname.find(index) != m_indexname.end();
}

CString FieldChangeTable::getIndexByName(const CString &name)
{
	if(findName(name) == false) return _T("");
	return m_nameindex[name];
}

CString FieldChangeTable::getNameByIndex(int index)
{
	CString indx; indx.Format(_T("%d"), index);
	return getNameByIndex(indx);
}

CString FieldChangeTable::getNameByIndex(const CString &index)
{
	CString indx(index); indx.Trim();
	if(m_indexname.find(indx) == m_indexname.end())
		return _T("");
	return m_indexname[indx];
}

void FieldChangeTable::addField(const CString &index, const CString &name)
{
	m_indexname[index] = name;
	m_nameindex[name] = index;
}

IProjectMDB::IProjectMDB()
	: db(NULL)
{
	initMDB();
}

IProjectMDB::~IProjectMDB()
{
	if(db != NULL)
		delete db;
	db = NULL;
}

MStr IProjectMDB::getBridgeXX()
{
	VMStr records; MStr ret;
	if(false == db->ReadDataBase(BRIDGE, records)) return MStr();
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		MSIter it = record.begin();
		CString name = it->second;
		CString value = (++it)->second;
		ret[name] = value;
	}
	return ret;
}

MStr IProjectMDB::getProjectInfo()
{
	VMStr records; MStr ret;
	db->ReadDataBase_all(_T("JBXX"), records);
	if(records.size() == 0) return MStr();
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records.at(idx);
		MSIter it = record.begin();
		CString name = it->second;
		ret[name] = (++it)->second;
	}
	return ret;
}

CString IProjectMDB::getClearWidth()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("天桥净宽");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

VMStr IProjectMDB::getCXXTableInfo()
{
	VMStr records;
	db->ReadDataBase_all(_T("CXX"), records);
	return records;
}

MStr IProjectMDB::getJSSMTableInfo()
{
	return getTableInfo(_T("JSSM"));
}

MStr IProjectMDB::getMJXXTableInfo()
{
	return getTableInfo(_T("MJXX"));
}

VMStr IProjectMDB::getPictureTable()
{
	VMStr records;
	db->ReadDataBase(PICTURETABLE, records);
	return records;
}

MStr IProjectMDB::getBGXXTableInfo()
{
	return getTableInfo(_T("BGXX"));
}

MStr IProjectMDB::getJBXXTableInfo()
{
	return getTableInfo(_T("JBXX"));
}

VMStr IProjectMDB::getDXXTableInfo()
{
	VMStr records;
	db->ReadDataBase_all(_T("DXX"), records);
	return records;
}

CString IProjectMDB::getClearHeight()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("天桥净高");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

CString IProjectMDB::getBridgeWidth()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("天桥宽度");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

bool IProjectMDB::clearPictureTable()
{
	return db->clearTable(PICTURETABLE);
}

CString IProjectMDB::getProjectName()
{
	CString value;
	db->ReadDataBase(_T("JBXX"), _T("项目名称"), value);
	return value;
}

CString IProjectMDB::getBridgeLength()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("天桥长度");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

CString IProjectMDB::getLoadLevation()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("道路标高");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

CString IProjectMDB::getBridgeLevation()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("桥面标高");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

CString IProjectMDB::getBridgeThickness()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("天桥厚度");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

CString IProjectMDB::getBridgeBalustrade()
{
	CString value; MStr filter;
	filter[_T("关键字")] = _T("天桥栏杆");
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

bool IProjectMDB::initMDB(const CString &path)
{
	if(db != NULL)
	{
		delete db;
		db = NULL;
	}
	if(db == NULL)
		db = new DataBase;
	if(path.GetLength() == 0)
	{
		IDataBaseOper oper;
		CString ppath = oper.readPathTable("当前目录");
		return db->initialConnect(ppath + _T("\\GC\\项目信息.mdb"));
	}
	return db->initialConnect(path);
}

bool IProjectMDB::setBridgeXX(const MStr &info)
{
	for(MSCIter it = info.begin(); it != info.end(); ++it)
	{
		MStr filter, record;
		filter[_T("关键字")] = it->first;
		record[_T("关键字")] = it->first;
		record[_T("属性值")] = it->second;
		db->AddOneRecord(BRIDGE, record, filter);
	}
	return true;
}

VMStr IProjectMDB::getGNQXXInfo(const MStr &filter)
{
	VMStr records;
	db->ReadDataBase(_T("GNQXX"), records, filter);
	return records;
}

MStr IProjectMDB::getGNQXXInfoEx(const MStr &filter)
{
	VMStr records = getGNQXXInfo(filter);
	return (records.size() == 0) ? MStr() : records[0];
}

CString IProjectMDB::getBGTable(const CString &name, const CString &info)
{
	CString value; MStr filter;
	filter[_T("GUID")] = name;
	db->ReadDataBase(_T("BG"), info, filter, value);
	return value;
}

void IProjectMDB::setMJXXTableInfo(const MStr &mjxx)
{
	setProjectInfo(_T("MJXX"), mjxx);
}

void IProjectMDB::setBGXXTableInfo(const MStr &mjxx)
{
	setProjectInfo(_T("BGXX"), mjxx);
}

MStr IProjectMDB::getTableInfo(const CString &table)
{
	VMStr records; MStr ret;
	db->ReadDataBase_all(table, records);
	if(records.size() == 0) return MStr();
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records.at(idx);
		MSIter it = record.begin();
		CString name = it->second;
		ret[name] = (++it)->second;
	}
	return ret;
}

bool IProjectMDB::setClearWidth(const CString &value)
{
	CString width = getClearWidth();
	int len = width.GetLength();
	if(len == 0) width.Append(value);
	else if(width.ReverseFind(_T('、')) == len - 1)
		width.Append(value);
	else if(value.Find(_T('、')) == 0) width.Append(value);
	else width.Append(_T("、")); width.Append(value);
	MStr filter, record;
	filter[_T("关键字")] = _T("天桥净宽");
	record[_T("属性值")] = width;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

void IProjectMDB::delBGTableInfo(const CString &name)
{

}

void IProjectMDB::delCXXTableInfo(const MStr &filter)
{
	db->DeleteReocrd(_T("CXX"), filter);
}

void IProjectMDB::addCXXTableInfo(const MStr &record)
{
	db->InsertRecord(_T("CXX"), record);
}

bool IProjectMDB::hasTableRow(const CString& table, const MStr &field_value, VMStr &selectRow)
{
	return db->hasRow(table, field_value, selectRow);
}

MStr IProjectMDB::getCXXTableInfo(const MStr &filter)
{
	VMStr records;
	db->ReadDataBase(_T("CXX"), records, filter);
	if(records.size() == 0) return MStr();
	return records[0];
}

MStr IProjectMDB::getDXXTableInfo(const MStr &filter)
{
	VMStr records;
	db->ReadDataBase(_T("DXX"), records, filter);
	if(records.size() == 0) return MStr();
	return records[0];
}

bool IProjectMDB::setBridgeWidth(const CString &value)
{
	MStr filter, record; filter[_T("关键字")] = _T("天桥宽度");
	record[_T("属性值")] = value;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

bool IProjectMDB::setClearHeight(const CString &value)
{
	if(true == value.IsEmpty()) return false;
	CString height = getClearHeight();
	if(height.Find(value) != -1) return true;
	else
	{
		int len = height.GetLength();
		if(len != 0 && height.ReverseFind(_T('、')) == len - 1)
			height.Append(_T("、"));
	}
	height.Append(value);
	MStr filter, record;
	filter[_T("关键字")] = _T("天桥净高");
	record[_T("属性值")] = height;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

void IProjectMDB::setJBXXTableInfo(const MStr &record)
{
	setProjectInfo(_T("JBXX"), record);
}

void IProjectMDB::setJSSMTableInfo(const MStr &record)
{
	setProjectInfo(_T("JSSM"), record);
}

bool IProjectMDB::setLoadLevation(const CString &value)
{
	if(true == value.IsEmpty()) return false;
	CString load = getLoadLevation();
	if(load.Find(value) != -1) return true;
	else
	{
		int len = load.GetLength();
		if(len != 0 && load.ReverseFind(_T('、')) == len - 1)
			load.Append(_T("、"));
	}
	load.Append(value);
	MStr filter, record;
	filter[_T("关键字")] = _T("道路标高");
	record[_T("属性值")] = load;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

bool IProjectMDB::setBridgeLength(const CString &value)
{
	MStr filter, record;
	filter[_T("关键字")] = _T("天桥长度");
	record[_T("属性值")] = value;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

CString IProjectMDB::getProjectInfo(const CString &info)
{
	CString value; MStr filter;
	filter[_T("关键字")] = info;
	db->ReadDataBase(_T("JBXX"), _T("属性值"), filter, value);
	return value;
}

void IProjectMDB::clearCXXTable(const CString &layernum)
{
	MStr filter; filter[_T("层号")] = layernum;
	deleteTableRecord(_T("CXX"), filter);
}

VMStr IProjectMDB::getCXXTableInfoEx(const MStr &filter)
{
	VMStr records;
	db->ReadDataBase(_T("CXX"), records, filter);
	if(records.size() == 0) return records;
	return records;
}

bool IProjectMDB::setBridgeLevation(const CString &value)
{
	MStr filter, record;
	filter[_T("关键字")] = _T("桥面标高");
	record[_T("属性值")] = value;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

void IProjectMDB::setChannelBodyPosition(const MStr &info)
{
	MSCIter it = info.begin();
	for(; it != info.end(); ++it)
	{
		MStr filter, record;
		filter[_T("关键字")] = it->first;
		record[_T("属性值")] = it->second;
		bool flag = db->UpdataRecord(_T("RXTQ"), filter, record);
		if(flag == true) continue;
		CString error; error.Format(_T("\n %s无法更新"), it->first);
		acutPrintf(error);
	}
}

bool IProjectMDB::updataPictureTable(const VMStr &records)
{
	clearPictureTable();
	int size = (int)records.size();
	for(int idx = 0; idx < size; ++idx)
		db->InsertRecord(PICTURETABLE, records.at(idx));
	return true;
}

bool IProjectMDB::setBridgeThickness(const CString &value)
{
	MStr filter, record;
	filter[_T("关键字")] = _T("天桥厚度");
	record[_T("属性值")] = value;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

bool IProjectMDB::setBridgeBalustrade(const CString &value)
{
	MStr filter, record;
	filter[_T("关键字")] = _T("天桥栏杆");
	record[_T("属性值")] = value;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

VMStr IProjectMDB::getCXXTableInfo(const CString &layernum)
{
	VMStr records; MStr filter;
	filter[_T("层号")] = layernum;
	db->ReadDataBase(_T("CXX"), records, filter);
	if(records.size() == 0) return VMStr();
	return records;
}

CString IProjectMDB::getChannelBodyPosition(const CString &type)
{
	CString value; MStr filter;
	filter[_T("关键字")] = type;
	db->ReadDataBase(_T("RXTQ"), _T("属性值"), filter, value);
	return value;
}

void IProjectMDB::addGNQXXInfo(const MStr &record, const MStr &filter)
{
	db->AddOneRecord(_T("GNQXX"), record, filter);
}

void IProjectMDB::setDXXTalbeInfo(const MStr &filter, const MStr &record)
{
	if(filter.size() == 0)
		db->InsertRecord(_T("DXX"), record);
	else
		db->UpdataRecord(_T("DXX"), filter, record);
}

void IProjectMDB::setCXXTalbeInfo(const MStr &filter, const MStr &record)
{
	db->UpdataRecord(_T("CXX"), filter, record);
}

void IProjectMDB::setProjectInfo(const CString &table, const MStr &record)
{
	MSCIter it = record.begin();
	MStr value, filter;
	for(; it != record.end(); ++it)
	{
		filter[_T("关键字")] = it->first;
		value[_T("属性值")] = it->second;
		value[_T("关键字")] = it->first;
		int nCount = db->TableContainFieldValue(table, filter);
		if(nCount == 0)
			db->InsertRecord(table, value);
		else
			db->UpdataRecord(table, filter, value);
	}
}

void IProjectMDB::setProjectInfo(const CString &name, const CString &info)
{
	MStr record, filter;
	filter[_T("关键字")] = name;
	record[_T("属性值")] = info;
	db->UpdataRecord(_T("JBXX"), filter, record);
}

void IProjectMDB::setBGGUID(const CString &guid)
{
	MStr record; record[_T("GUID")] = guid;
	db->InsertRecord(_T("BG"), record);
}

void IProjectMDB::setBGInfoTable(const CString &guid, const CString &name, const CString &info)
{
	MStr record, filter;
	filter[_T("GUID")] = guid;
	record[name] = info;
	db->UpdataRecord(_T("BG"), filter, record);
}

bool IProjectMDB::updataBridgeXX(const CString &name, const CString &value)
{
	return true;
}

void IProjectMDB::updataCXXTableInfo(const MStr &filter, const MStr &record)
{
	db->UpdataRecord(_T("CXX"), filter, record);
}

bool IProjectMDB::deleteTableRecord(const CString &table, const MStr &filter)
{
	return db->DeleteReocrd(table, filter);
}

bool IProjectMDB::setChannelBodyPosition(const CString &name, const CString &value)
{
	MStr record, filter;
	filter[_T("关键字")] = name;
	record[_T("属性值")] = value;
	return db->UpdataRecord(_T("RXTQ"), filter, record);
}

void IProjectMDB::setPipeLineTable(const CString &table, const VMStr &records)
{
	db->DeleteDataBase_All(_T("PIPELINE"));
	db->InsertRecord(table, records);
}

bool IProjectMDB::getPipelineColor(const CString &strGxlx, int &nR, int &nG, int &nB)
{
	MStr filter;
	int nFirst, nComma1, nComma2, nlen, nEnd;
	CString value, strTemp;
	filter[_T("名称")] = strGxlx;
	if(false == db->ReadDataBase(_T("PIPELINE"), _T("颜色"), filter, value)) return false;
	nFirst = value.Find(_T("("));
	nComma1 = value.Find(_T(","));
	strTemp = value.Mid(nFirst + 1, nComma1 - 1);
	nR = _tstoi(strTemp);
	nComma2 = value.Find(_T(","), nComma1 + 1);
	nlen = nComma2 - nComma1 - 1;
	strTemp = value.Mid(nComma1 + 1, nlen);
	nG = _tstoi(strTemp);
	nEnd = value.Find(_T(")"), nComma1);
	nlen = nEnd - nComma2 - 1;
	strTemp = value.Mid(nComma2 + 1, nlen);
	return true;
}

bool IProjectMDB::getPipelineColor(const CString &strGxlx, CString &strColor)
{
	MStr filter;
	filter[_T("简码")] = strGxlx;
	if(false == db->ReadDataBase(_T("PIPELINE"), _T("颜色"), filter, strColor)) return false;
	return true;
}

BOOL IProjectMDB::ChangeConnection(const CString &strPath)
{
	bool bChange = db->initialConnect(strPath);
	return bChange;
}

VMStr IProjectMDB::getPipeTable()
{
	VMStr records;
	db->ReadDataBase_all(_T("PIPELINE"), records);
	return records;
}

bool IProjectMDB::setRoadInfo(const CString &name, const GHSSJBQKInfo &roadInfo)
{
	MStr record; record[_T("名称")] = name;
	db->DeleteReocrd(_T("XMJGRoad"), record);
	int count = (int)roadInfo.roadWidth.m_scRoadWidth.size();
	record[_T("类型")] = _T("道路宽度");
	for(int idx = 0; idx < count; ++idx)
	{
		record[_T("设计")] = roadInfo.roadWidth.m_sjRoadWidth[idx];
		record[_T("实测")] = roadInfo.roadWidth.m_scRoadWidth[idx];
		db->InsertRecord(_T("XMJGRoad"), record);
	}
	count = (int)roadInfo.roadLength.m_scRoadLength.size();
	record[_T("类型")] = _T("道路长度");
	for(int idx = 0; idx < count; ++idx)
	{
		record[_T("设计")] = roadInfo.roadLength.m_sjRoadLength[idx];
		record[_T("实测")] = roadInfo.roadLength.m_scRoadLength[idx];
		db->InsertRecord(_T("XMJGRoad"), record);
	}
	count = (int)roadInfo.roadHeight.m_scRoadHeight.size();
	record[_T("类型")] = _T("道路断面");
	for(int idx = 0; idx < count; ++idx)
	{
		record[_T("设计")] = roadInfo.roadHeight.m_sjRoadHeight[idx];
		record[_T("实测")] = roadInfo.roadHeight.m_scRoadHeight[idx];
		db->InsertRecord(_T("XMJGRoad"), record);
	}
	return true;
}

bool IProjectMDB::setRoadInfo(const CString &name, const GHSSJBQKPipe &pipeInfo)
{
	MStr record; record[_T("名称")] = name;
	db->DeleteReocrd(_T("XMJGRoad"), record);
	record[_T("类型")] = _T("管线信息");
	int count = (int)pipeInfo.m_scPipeLine.size();
	for(int idx = 0; idx < count; ++idx)
	{
		record[_T("设计")] = pipeInfo.m_sjPipeLine[idx];
		record[_T("实测")] = pipeInfo.m_scPipeLine[idx];
		db->InsertRecord(_T("XMJGRoad"), record);
	}
	return true;
}

void IProjectMDB::getRoadInfo(const CString &name, GHSSJBQKInfo &info)
{
	VMStr records;
	db->ReadDataBase_all(_T("XMJGRoad"), records);
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		CString Name = record[_T("名称")];
		if(Name.CompareNoCase(name) != 0) continue;
		if(record[_T("类型")].CompareNoCase(_T("道路宽度")) == 0)
		{
			info.roadWidth.m_scRoadWidth.push_back(record[_T("实测")]);
			info.roadWidth.m_sjRoadWidth.push_back(record[_T("设计")]);
		}
		if(record[_T("类型")].CompareNoCase(_T("道路断面")) == 0)
		{
			info.roadHeight.m_scRoadHeight.push_back(record[_T("实测")]);
			info.roadHeight.m_sjRoadHeight.push_back(record[_T("设计")]);
		}
		if(record[_T("类型")].CompareNoCase(_T("道路长度")) == 0)
		{
			info.roadLength.m_scRoadLength.push_back(record[_T("实测")]);
			info.roadLength.m_sjRoadLength.push_back(record[_T("设计")]);
		}
	}
}

void IProjectMDB::getRoadInfo(const CString &name, GHSSJBQKPipe &info)
{
	VMStr records;
	db->ReadDataBase_all(_T("XMJGRoad"), records);
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		CString Name = record[_T("名称")];
		if(Name.CompareNoCase(name) != 0) continue;
		if(record[_T("类型")].CompareNoCase(_T("管线信息")) == 0)
		{
			info.m_scPipeLine.push_back(record[_T("实测")]);
			info.m_sjPipeLine.push_back(record[_T("设计")]);
		}
	}
}

void IProjectMDB::setRXBridgeTableInfo(const VMStr &records)
{
	db->DeleteDataBase_All(_T("RXBridgeInfo"));
	db->InsertRecord(_T("RXBridgeInfo"), records);
}

VMStr IProjectMDB::readRXBridgeTableInfo()
{
	VMStr records;
	db->ReadDataBase_all(_T("RXBridgeInfo"), records);
	return records;
}

VStr IProjectMDB::getAllRoadName()
{
	VMStr records; VStr names;
	db->ReadDataBase_all(_T("XMJGRoad"), records);
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx];
		CString name = record[_T("名称")];
		if(names.end() == std::find(names.begin(), names.end(), name))
			names.push_back(name);
	}
	return names;
}

int IProjectMDB::getRecordCount(const CString &table, const MStr &filter)
{
	return db->TableContainFieldValue(table, filter);
}

bool closeDWG(const CString &name)
{
	AcApDocManager *pDocMgr = acDocManagerPtr();
	AcApDocumentIterator *pDocItr;
	AcApDocument *pMainDoc = NULL; CString szName;
	pDocItr = pDocMgr->newAcApDocumentIterator();
	for(; !pDocItr->done(); pDocItr->step())
	{
		AcApDocument * pDoc = pDocItr->document();
		acDocManager->setCurDocument(pDoc);
		szName.Format(_T("%s"), pDoc->fileName());
		if(szName.Find(name) == -1) continue;
		pDoc->database()->saveAs(szName);
		pDocMgr->closeDocument(pDoc);
		return true;
	}
	return false;
}

void splitBigString(const CString &szFieldValue, string &bigchar)
{
	CString strTemp; char tempchar[255] = {0};
	strTemp.Format(_T("%s"), szFieldValue);
	if(strTemp.GetLength() > 100)
	{
		int nSplit = strTemp.GetLength() / 100;
		int nSY = strTemp.GetLength() % 100;
		if(strTemp.GetLength() % 100 != 0)
			nSplit++;
		for(int nIndex = 0; nIndex < nSplit; nIndex++)
		{
			int nLenCh = 100;
			CString strTemp1;
			if(nIndex == 0)
			{
				strTemp1 = strTemp.Left(nLenCh);
				strTemp1 += _T("#TIHUAN#");
				if(-1 == strTemp1.Find(_T("#TIHUAN#")))
				{
					nLenCh++;
					strTemp1 = strTemp.Left(nLenCh);
					strTemp1 += _T("#TIHUAN#");
				}
				TcharToChar(strTemp1, tempchar);
				bigchar.append(tempchar);
				strTemp = strTemp.Right(strTemp.GetLength() - nLenCh);
				continue;
			}
			if(nIndex != (nSplit - 1))
			{
				strTemp1 = strTemp.Left(nLenCh);
				strTemp1 += _T("#TIHUAN#");
				if(-1 == strTemp1.Find(_T("#TIHUAN#")))
				{
					nLenCh++;
					strTemp1 = strTemp.Left(nLenCh);
					strTemp1 += _T("#TIHUAN#");
				}
				TcharToChar(strTemp1, tempchar);
				bigchar.append(tempchar);
				if(nLenCh >= strTemp.GetLength())
					strTemp = _T("");
				else
					strTemp = strTemp.Right(strTemp.GetLength() - nLenCh);
			}
			else
			{
				strTemp1 = strTemp.Left(nSY);
				TcharToChar(strTemp1, tempchar);
				bigchar.append(tempchar);
			}
		}
	}
	else
	{
		TcharToChar(szFieldValue, tempchar);
		bigchar.append(tempchar);
	}
	string oldstr("#TIHUAN#"), newstr("");
	for(string::size_type pos = 0; pos != string::npos; pos += oldstr.length())
	{
		if((pos = bigchar.find(oldstr, pos)) != string::npos)
			bigchar.replace(pos, oldstr.length(), newstr);
		else break;
	}
}

typedef map<CString, AcDbObjectId, CmpCstr> MSIdEx;
typedef MSIdEx::iterator MSIdExIter;

void sortObjectByXDataInt(AcDbObjectIdArray &ids, const VStr &filter, bool del)
{
	AcDbObjectIdArray temps(ids); ids.removeAll();
	MSIdEx mid; TCHAR value[255] = {0}; AcDbObjectIdArray tmps;
	for(int idx = 0; idx < temps.length(); ++idx)
	{
		AcDbObjectId id = temps.at(idx); CString temp;
		for(int indx = 0; indx < (int)filter.size(); ++indx)
		{
			if(ReadXdata(aname(id), filter[indx], 0, value))
				temp += value;
		}
		mid[temp] = id;
		if(del == false && temp.GetLength() == 0) tmps.append(id);
	}
	MSIdExIter it = mid.begin();
	for(; it != mid.end(); ++it)
		ids.append(it->second);
	ids.append(tmps);
}

MSSIds sortGNQByXData(const AcDbObjectIdArray &ids)
{
	MSSIds mssids; TCHAR value[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		CString temp, tj; AcDbObjectId id = ids[idx];
		ReadXdata(aname(id), _T("梯间名称"), 0, value);
		if(_tcslen(value) != 0) tj = value; CString jrxs;
		if(!ReadXdata(aname(id), _T("功能区高度"), 0, value)) continue;
		temp.Format(_T("层高%.2lfm"), _tstof(value));
		if(!ReadXdata(aname(id), _T("计容系数"), 0, value)) continue;
		if(_tstof(value) > EPS) jrxs.Format(_T("，计容系数%.2lf"), _tstof(value));
		temp += jrxs; if(mssids.find(tj) == mssids.end()) mssids[tj] = MSIds();
		if(mssids[tj].find(temp) == mssids[tj].end()) mssids[tj][temp] = AcDbObjectIdArray();
		mssids[tj][temp].append(id);
	}
	VStr filter;
	filter.push_back(_T("功能区简称"));
	filter.push_back(_T("功能区编号"));
	for(MSSIdsIter sit = mssids.begin(); sit != mssids.end(); ++sit)
	{
		MSIds msids = sit->second;
		for(MSIdsIter it = msids.begin(); it != msids.end(); ++it)
		{
			AcDbObjectIdArray tids = it->second;
			sortObjectByXDataInt(tids, filter);
			it->second = tids;
		}
	}
	return mssids;
}

bool stringIsDigit(const CString &str)
{
	CString num(str);
	if(str.Left(1).CompareNoCase(_T("-")) == 0)
		num = str.Mid(1);
	int len = num.GetLength();
	for(int idx = 0; idx < len; ++idx)
	{
		TCHAR ch = num.GetAt(idx);
		if(_T('0') <= ch && _T('9') >= ch) continue;
		return false;
	}
	return true;
}

void convertDwg2Pdf(const CString &pathName)
{
	SelectFilter sf(8, _T("打印范围"));	CPrintPDF printPDF;
	AcDbObjectIdArray extendids; if(false == SelectEntitys(extendids, sf, _T("X"))) return;
	CString savePath; int len = extendids.length();
	for(int idx = 0; idx < len; ++idx)
	{
		ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
		AcGePoint2dArray nodes; ads_point ptMin, ptMax;
		AcDbObjectId extendid = extendids.at(idx);
		GetEntExtent(aname(extendid), ptMin, ptMax, 0);
		double height = ptMax[Y] - ptMin[Y];
		double width = ptMax[X] - ptMin[X];
		GetPlList(aname(extendid), nodes);
		AcDbObjectIdArray ids;
		ZoomWindow(ptMin, ptMax);
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		ssFromNodes(ids, nodes, 1, 0.0);
		ids.remove(extendid);
		EntDel(aname(extendid));
		if(height > width)
		{
			for(int indx = 0; indx < ids.length(); ++indx)
				RotateEntity(aname(ids[indx]), PI / 2, apoint(ptMin));
		}
		MoveEntitys(ids, 0, 1000);
		zoomWindow(ids);
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		printPDF.m_objArrSave.setLogicalLength(0);
		printPDF.m_objArrSave.setPhysicalLength(0);
		printPDF.m_objArrSave.append(ids);
		if(len != 1)
			savePath.Format(_T("%s%d.pdf"), pathName, idx);
		else savePath.Format(_T("%s.pdf"), pathName);
		printPDF.m_strTempSavePath = savePath;
		if(false == printPDF.setPrintSize()) return;
		printPDF.Plot();
#ifdef _WIN64

		TCHAR szPath[MAX_PATH] = {0};
		if(ads_findfile(_T("Pdf2Png.exe"), szPath) != RTNORM)
		{
			acutPrintf(_T("\n找不到自定义报表导出程序\'生成自定义模版.exe\'"));
			return;
		}
		TCHAR commandline[4000] = {0};
		_stprintf(commandline, _T("%s 2 %s"), szPath, savePath);
		STARTUPINFO si; PROCESS_INFORMATION pi;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		if(FALSE == CreateProcess(NULL, commandline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			MessageBox(GetForegroundWindow(), _T("打包失败！！！"), _T("错误信息"), MB_OK);
			return;
		}
		else
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			WaitForSingleObject(pi.hThread, INFINITE);
		}
#elif defined WIN32
		CString output(savePath); output.Replace(_T(".pdf"), _T(".png"));
		PDF2PNG(savePath, output); DeleteFile(savePath);
#endif
		ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
		ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	}
	FileDelete(savePath);
}

void convertDwg2WMF(const CString &pathName)
{
	SelectFilter sf(8, _T("打印范围"));	CPrintPDF printPDF;
	AcDbObjectIdArray extendids; if(false == SelectEntitys(extendids, sf, _T("X"))) return;
	CString savePath; int len = extendids.length();
	for(int idx = 0; idx < len; ++idx)
	{
		CBackScreen backScreen;
		AcGePoint2dArray nodes; ads_point ptMin, ptMax;
		AcDbObjectId extendid = extendids.at(idx);
		GetEntExtent(aname(extendid), ptMin, ptMax, 0);
		double height = ptMax[Y] - ptMin[Y];
		double width = ptMax[X] - ptMin[X];
		GetPlList(aname(extendid), nodes);
		AcDbObjectIdArray ids;
		ZoomWindow(ptMin, ptMax);
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		ssFromNodes(ids, nodes, 1, 0.0);
		ids.remove(extendid);
		EntDel(aname(extendid));
		if(height > width)
		{
			for(int indx = 0; indx < ids.length(); ++indx)
				RotateEntity(aname(ids[indx]), PI / 2, apoint(ptMin));
		}
		MoveEntitys(ids, 0, 1000);
		zoomWindow(ids);
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		if(len != 1)
			savePath.Format(_T("%s%d.wmf"), pathName, idx);
		else savePath.Format(_T("%s.wmf"), pathName);
		CKernalTool::WMFOutEx(savePath, ids);
	}
}

double getLayerJZMJArea(const AcDbObjectId &id)
{
	AcGePoint2dArray node; AcDbObjectIdArray ids;
	GetPlList(aname(id), node); TCHAR layer[255] = {0};
	IDataBaseOper oper; CString gnqlayer;
	oper.readCommonTable(_T("功能区图层"), gnqlayer);
	double area, total = 0.0; ids.remove(id);
	for(int i = 0; i < ids.length(); i++)
	{
		AcDbObjectId gnq = ids.at(i);
		GetEntLayer(gnq, layer);
		if(gnqlayer.CompareNoCase(layer) != 0) continue;
		GetEntArea(aname(gnq), area);
		total += area;
	}
	return total;
}

double getLayerJRMJArea(const AcDbObjectId &id)
{
	AcGePoint2dArray node; AcDbObjectIdArray ids;
	GetPlList(aname(id), node); TCHAR info[255] = {0};
	double total = 0.0; ids.remove(id);
	for(int i = 0; i < ids.length(); i++)
	{
		AcDbObjectId gnq = ids.at(i);
		ReadXdata(aname(gnq), _T("计容面积"), 3, info);
		total += _tstof(info);
	}
	return total;
}

void getLayerArea(const AcDbObjectId &id, double &jzmj, double &jrmj, double height)
{
	AcGePoint2dArray node; AcDbObjectIdArray ids;
	GetPlList(aname(id), node); TCHAR area[255] = {0};
	ssFromNodes(ids, node); ids.remove(id);
	for(int i = 0; i < ids.length(); i++)
	{
		AcDbObjectId gnq = ids.at(i); double mj = 0.0;
		GetEntArea(aname(gnq), mj);
		ReadXdata(aname(gnq), _T("面积系数"), 0, area);
		jzmj += _tstof(area) * mj;
		ReadXdata(aname(gnq), _T("计容系数"), 0, area);
		jrmj += _tstof(area) * mj;
		ReadXdata(aname(gnq), _T("主体高度"), 0, area);
		if(_tcscmp(area, _T("1")) == 0)
		{
			_stprintf(area, _T("%.3lf"), height);
			AddXdata(aname(gnq), _T("功能区高度"), 0, area);
		}
	}
}

void getCarCount(const AcDbObjectId &id, MStr &record, double &jzmj, double &jrmj)
{
	AcGePoint2dArray node; AcDbObjectIdArray ids;
	GetPlList(aname(id), node); TCHAR sm[255] = {0};
	ssFromNodes(ids, node, 1, 1.0, _T("*POLYLINE"), _T("CW"));// ids.remove(id);
	int dscw = _ttoi(record[_T("实测地上车位")]);
	int dxcw = _ttoi(record[_T("实测地下车位")]);
	TCHAR lc[255] = {0};
	double temparea = 0.0;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(false == ReadXdata(aname(id), _T("CWLX"), 0, lc)) continue;
		ReadXdata(aname(id), _T("SZLC"), 0, lc);
		ReadXdata(aname(id), _T("CWSM"), 1, sm);
		if(_tcscmp(lc, _T("地上")) == 0) dscw += _ttoi(sm);
		else dxcw += _ttoi(sm);
		GetEntArea(aname(id), temparea);
		jzmj += temparea; //jrmj += temparea;
	}
	if(dscw != 0) record[_T("实测地上车位")].Format(_T("%d"), dscw);
	if(dxcw != 0) record[_T("实测地下车位")].Format(_T("%d"), dxcw);
}

short isEntClosed(const AcDbObjectId &objId)
{
	ads_name ent;
	if(Acad::eOk != acdbGetAdsName(ent, objId)) return -2;

	TCHAR szType[200] = {0};
	GetEntityType(ent, szType);
	CString	str = szType;
	if(str.Find(_T("LINE")) <= 0) return -1;

	AcDbCurve *pCur;
	if(acdbOpenObject(pCur, objId, AcDb::kForRead) != Acad::eOk) return -2;
	Adesk::Boolean bClose = pCur->isClosed();
	pCur->close();

	if(bClose) return 1;
	else return 0;
}

bool SetLineWidth(const AcDbObjectId &id, const double width)
{
	AcDbPolyline* pPL;
	if(acdbOpenObject(pPL, id, AcDb::kForWrite) != Acad::eOk) return false;
	pPL->setConstantWidth(width); pPL->close();
	// 	int nNumber = pPL->numVerts();
	// 
	// 	int i = 0;
	// 	for(; i < nNumber; i++)
	// 	{
	// 		int nRes = pPL->setWidthsAt(i, width, width);
	// 		nRes = 0;
	// 	}
	// 	pPL->close();
	return true;
}

AcDbObjectIdArray getInnerObjects(const AcDbObjectId &id)
{
	AcGePoint2dArray node; GetPlList(aname(id), node);
	AcDbObjectIdArray ids;	ssFromNodes(ids, node);
	ids.remove(id); return ids;
}

bool getNextPointByLine(const AcDbObjectId &id, AcGePoint2dArray &node)
{
	AcGePoint2dArray pts;
	GetPlList(aname(id), pts);
	if(node.length() != 0)
	{
		if(pts.contains(node.first()) == true) return true;
		if(pts.contains(node.last()) == false) return false;
	}
	for(int idx = 0; idx < pts.length(); ++idx)
	{
		AcGePoint2d pt1 = pts.at(idx);
		if(node.contains(pt1) == true) continue;
		node.append(pt1);
	}
	return true;
}

AcDbObjectId drawLine()
{
	ads_name last, ent;
	struct resbuf *cmd, *result;
	ads_entlast(last);
	cmd = ads_buildlist(RTSTR, _T("drawdjf"), RTSTR, _T(""), RTNONE);
	ads_invoke(cmd, &result); ads_relrb(cmd);
	if(result == NULL) return AcDbObjectId::kNull;
	ads_entlast(ent);	ads_relrb(result);
	if(ads_name_equal(last, ent))
	{
		return AcDbObjectId::kNull;
	}
	return aname(ent);
}

AcDbObjectId drawLine(const CString &_layer, int _color)//创建多段线并且设置多段线的图层和颜色
{
	struct resbuf *cmd, *result; ads_name ent;
	cmd = ads_buildlist(RTSTR, _T("drawdjf"), RTSTR, _T(""), RTNONE);//创建画多段线的指令
	ads_invoke(cmd, &result);//画多段线
	ads_relrb(cmd);//释放画多段线指令
	if(result == NULL) return AcDbObjectId::kNull;//如果没有返回值，则创建失败，返回空ID号
	if(RTENAME == result->restype)//如果返回的是实体，则创建成功
	{
		ads_name_set(result->resval.rlname, ent);//将返回值赋给ent
		ads_relrb(result);//释放实体缓冲集
	}
	else//如果返回的不是实体，则创建失败
	{
		ads_relrb(result);//释放实体缓冲集
		return AcDbObjectId::kNull;//返回空ID号
	}
	setlayer(ent, _layer);//设置创建的多段线图层
	setcolor(ent, _color);//设置创建的多段线颜色
	return aname(ent);//返回多段线ID号，aname获得ads_name对应的ID号
}

long TimeConvertLong(const CString &value)
{
	long starttime = 0; if(value.GetLength() == 0) return starttime;
	int spos = value.Find(_T("/")); int epos = value.ReverseFind(_T('/'));
	if(spos == -1 || epos == -1)
	{
		spos = value.Find(_T("-"));
		epos = value.ReverseFind(_T('-'));
	}
	if(spos == -1 || epos == -1)
		return starttime;
	starttime = _ttol(value.Mid(0, spos));
	starttime = starttime * 100 + _ttol(value.Mid(spos + 1, epos));
	starttime = starttime * 100 + _ttol(value.Mid(epos + 1));
	return starttime;
}

void openAllDwg(const VStr &dwgs)
{
	for(int idx = 0; idx < (int)dwgs.size(); ++idx)
		OpenDWG(dwgs[idx]);
}

void closeAllDwg(VStr &dwgs)
{
	AcApDocManager *pDocMgr = acDocManagerPtr();
	AcApDocumentIterator *pDocItr;
	AcApDocument *pMainDoc = NULL; CString szName;
	pDocItr = pDocMgr->newAcApDocumentIterator();
	for(; !pDocItr->done(); pDocItr->step())
	{
		AcApDocument * pDoc = pDocItr->document();
		acDocManager->setCurDocument(pDoc);
		szName.Format(_T("%s"), pDoc->fileName());
		if(szName.Find(_T("Drawing")) != -1) continue;
		pDoc->database()->saveAs(szName);
		pDocMgr->closeDocument(pDoc);
		dwgs.push_back(szName);
	}
}

AcDbObjectId nodeAnnotation(ads_point pt, ads_point pos, const TextProperty &prop)
{
	CDrawArrow arrow; AcDbObjectId group;
	group = arrow.DrawLeader(pt, pos, (float)prop._size, prop._layer, prop._color);
	CString handle = GetHandleByObject(group);
	AddXdata(aname(group), _T("坐标标注"), 0, handle);
	return group;
}

void singleNodeAnnotation(const TextProperty &prop, double scale)
{
	ads_point pt, pos;
	while(true)
	{
		ads_name head, temp, last; ads_entlast(head);
		if(RTNORM != ads_getpoint(NULL, _T("\n 请选择需要注记的点"), pt)) return;
		if(RTNORM != ads_getpoint(NULL, _T("\n 请输入需要标注的位置："), pos)) return;
		nodeAnnotation(pt, pos, prop); ads_entlast(last); AcDbObjectIdArray ids;
		while(1)
		{
			ads_entnext(head, temp);
			if(temp[0] == 0) break;
			ids.append(aname(temp));
			ads_name_set(temp, head);
		}
		modifyEntsArrowSize(ids, scale);
	}
}

void singleNodeAnnotation(const TextProperty &prop, const TextProperty &prop1)
{
	ads_point pt, pos;
	while(true)
	{
		ads_name head, temp, last; ads_entlast(head);
		if(RTNORM != ads_getpoint(NULL, _T("\n 请选择需要注记的点"), pt)) return;
		if(RTNORM != ads_getpoint(NULL, _T("\n 请输入需要标注的位置："), pos)) return;
		nodeAnnotation(pt, pos, prop); ads_entlast(last); AcDbObjectIdArray ids;
		while(1)
		{
			ads_entnext(head, temp);
			if(temp[0] == 0) break;
			ids.append(aname(temp));
			ads_name_set(temp, head);
		}
		modifyEntsArrowSize(ids, prop1);
	}
}

AcDbObjectIdArray AddRaidusNote(const AcDbObjectId &id, double dLeaderLength)
{
	AcDbObjectIdArray idArrRaidusNote;
	AcDbEntity *pEnt;
	if(Acad::eOk != acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead))
		return idArrRaidusNote;
	if(pEnt == NULL)
	{
		pEnt->close();
		return idArrRaidusNote;
	}
	if(pEnt->isA() == AcDbPolyline::desc())
	{
		AcDbPolyline *pPline;
		pPline = AcDbPolyline::cast(pEnt);
		unsigned int i = 0, number = pPline->numVerts();
		for(; i < number; i++)
		{
			AcGeCircArc2d arc;
			if(Acad::eOk != pPline->getArcSegAt(i, arc)) continue;
			AcGeCircArc2d *pArc = new AcGeCircArc2d(arc);
			double dRadius = pArc->radius();
			if(dRadius < EPSS) continue;
			AcGePoint2d ptCenter = pArc->center();
			AcGePoint2d ptStart = pArc->startPoint();
			AcGePoint2d ptEnd = pArc->endPoint();
			double distParam = sqrt(pow((ptStart.x + ptEnd.x) / 2 - ptCenter.x, 2) + pow((ptStart.y + ptEnd.y) / 2 - ptCenter.y, 2)) / dRadius;//中垂线长度
			if(distParam < EPSS) continue;
			AcGePoint2d ptArcMid1, ptArcMid2;
			ptArcMid1.x = ((ptStart.x + ptEnd.x) / 2 - ptCenter.x) / distParam + ptCenter.x;
			ptArcMid1.y = ((ptStart.y + ptEnd.y) / 2 - ptCenter.y) / distParam + ptCenter.y;
			ptArcMid2.x = (ptCenter.x - (ptStart.x + ptEnd.x) / 2) / distParam + ptCenter.x;
			ptArcMid2.y = (ptCenter.y - (ptStart.y + ptEnd.y) / 2) / distParam + ptCenter.y;
			AcGePoint2d ptMid; ptMid.x = (ptStart.x + ptEnd.x) / 2; ptMid.y = (ptStart.y + ptEnd.y) / 2;
			if(pArc->isOn(ptArcMid1) == false && pArc->isOn(ptArcMid2) == false) continue;
			double angle = ads_angle(apoint(ptCenter), apoint(ptMid));
			ads_point ptArcMid; ads_polar(apoint(ptCenter), angle, dRadius, ptArcMid);
			//半径标注类型实体
			AcDbRadialDimension *adRD = new AcDbRadialDimension;
			adRD->setCenter(AcGePoint3d(ptCenter.x, ptCenter.y, 0));
			adRD->setTextPosition(AcGePoint3d(ptMid.x, ptMid.y, 0));
			adRD->setChordPoint(AcGePoint3d(ptArcMid[X], ptArcMid[Y], 0));
			if(fabs(dLeaderLength) < EPS)
				adRD->setLeaderLength(dRadius / 4);
			else
				adRD->setLeaderLength(dRadius);
			if(pArc->isOn(ptArcMid2))
			{
				AcDbMText *mt = NULL;
				Acad::ErrorStatus es = adRD->fieldToMText(mt);
				mt->setHeight(0.5);
				es = adRD->fieldFromMText(mt);
			}
			delete pArc;
			pArc = NULL;
			AcDbObjectId idRaidusNote;
			AppendEntity(adRD, idRaidusNote);
			idArrRaidusNote.append(idRaidusNote);
			adRD->close();

		}
	}
	pEnt->close();
	acedCommand(RTSTR, _T("Regen"), RTNORM);
	return idArrRaidusNote;
}

AcDbObjectIdArray AddRaidusNote(const AcDbObjectId &id, double dLeaderLength, const TextProperty &prop)
{
	AcDbObjectIdArray idArrRaidusNote;
	AcDbEntity *pEnt;
	if(Acad::eOk != acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead))
		return idArrRaidusNote;
	if(pEnt == NULL)
	{
		pEnt->close();
		return idArrRaidusNote;
	}
	if(pEnt->isA() == AcDbPolyline::desc())
	{
		AcDbPolyline *pPline; double scale = 500.0;
		read1(scale); scale /= 1000.0;
		pPline = AcDbPolyline::cast(pEnt);
		unsigned int i = 0, number = pPline->numVerts();
		for(; i < number; i++)
		{
			AcGeCircArc2d arc;
			if(Acad::eOk != pPline->getArcSegAt(i, arc)) continue;
			AcGeCircArc2d *pArc = new AcGeCircArc2d(arc);
			double dRadius = pArc->radius();
			if(dRadius < EPSS) continue;
			AcGePoint2d ptCenter = pArc->center();
			AcGePoint2d ptStart = pArc->startPoint();
			AcGePoint2d ptEnd = pArc->endPoint();
			double distParam = sqrt(pow((ptStart.x + ptEnd.x) / 2 - ptCenter.x, 2) + pow((ptStart.y + ptEnd.y) / 2 - ptCenter.y, 2)) / dRadius;//中垂线长度
			if(distParam < EPSS) continue;
			AcGePoint2d ptArcMid1, ptArcMid2;
			ptArcMid1.x = ((ptStart.x + ptEnd.x) / 2 - ptCenter.x) / distParam + ptCenter.x;
			ptArcMid1.y = ((ptStart.y + ptEnd.y) / 2 - ptCenter.y) / distParam + ptCenter.y;
			ptArcMid2.x = (ptCenter.x - (ptStart.x + ptEnd.x) / 2) / distParam + ptCenter.x;
			ptArcMid2.y = (ptCenter.y - (ptStart.y + ptEnd.y) / 2) / distParam + ptCenter.y;
			AcGePoint2d ptMid; ptMid.x = (ptStart.x + ptEnd.x) / 2; ptMid.y = (ptStart.y + ptEnd.y) / 2;
			if(pArc->isOn(ptArcMid1) == false && pArc->isOn(ptArcMid2) == false) continue;
			double angle = ads_angle(apoint(ptCenter), apoint(ptMid));
			ads_point ptArcMid; ads_polar(apoint(ptCenter), angle, dRadius, ptArcMid);
			//半径标注类型实体
			AcDbRadialDimension *adRD = new AcDbRadialDimension;
			adRD->setCenter(AcGePoint3d(ptCenter.x, ptCenter.y, 0));
			adRD->setTextPosition(AcGePoint3d(ptMid.x, ptMid.y, 0));
			adRD->setChordPoint(AcGePoint3d(ptArcMid[X], ptArcMid[Y], 0));
			if(fabs(dLeaderLength) < EPS)
				adRD->setLeaderLength(dRadius / 4);
			else
				adRD->setLeaderLength(dRadius);
			if(pArc->isOn(ptArcMid2))
			{
				AcDbMText *mt = NULL;
				Acad::ErrorStatus es = adRD->fieldToMText(mt);
				mt->setHeight(prop._size * scale);
				es = adRD->fieldFromMText(mt);
			}
			adRD->setDimscale(1.0);
			adRD->setDimtxt(prop._size * scale);
			adRD->setDimdec(2);
			adRD->setColorIndex(prop._color);
			adRD->setLayer(prop._layer);
			delete pArc; pArc = NULL;
			AcDbObjectId idRaidusNote;
			AppendEntity(adRD, idRaidusNote);
			idArrRaidusNote.append(idRaidusNote);
			adRD->close();
		}
	}
	pEnt->close();
	acedCommand(RTSTR, _T("Regen"), RTNORM);
	return idArrRaidusNote;
}

void mulitNodeAnnotation(const AcDbObjectIdArray &ids, const TextProperty &prop)
{
	XMJGAssist assist; double length = 1.5; ads_point spt, ept;
	if(RTCAN == ads_getreal(_T("\n 请输入注记线的长度："), &length)) return;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);	AcGePoint2dArray nodes;
		GetPlList(aname(id), nodes); bool bDeasil = IsDeasil(nodes);
		for(int indx = 0; indx < nodes.length(); ++indx)
		{
			spt[X] = nodes.at(indx).x; spt[Y] = nodes.at(indx).y;
			double angle = assist.GetOutCornerAndPosition(nodes, indx, bDeasil);
			ads_polar(spt, angle, length, ept);
			AcDbObjectId ret = nodeAnnotation(spt, ept, prop);
			ADDMulXData(id, _T("标注句柄"), GetHandleByObject(ret), STRING_TYPE, indx);
		}
		AddRaidusNote(id, length, prop);
	}
}

bool GetPrinterPath(CString &printer_path)
{
	CWinApp *pApp = acedGetAcadWinApp();
	HRESULT hRes;
	LPDISPATCH pDisp = NULL;

	if(!pApp)
		return false;

	pDisp = pApp->GetIDispatch(TRUE);
	if(!pDisp)
		return false;

	CComPtr<IAcadApplication>  pComApp;
	hRes = pDisp->QueryInterface(
		IID_IAcadApplication,
		(void**)&pComApp
		);
	if(FAILED(hRes))
		return false;

	CComPtr<IAcadPreferences>  pComPref;
	hRes = pComApp->get_Preferences(&pComPref);
	if(FAILED(hRes))
		return false;

	CComPtr<IAcadPreferencesFiles>  pComOutputPref;
	hRes = pComPref->get_Files(&pComOutputPref);
	if(FAILED(hRes))
		return false;

	BSTR bstDefaultFilePath;
	hRes = ((IAcadPreferencesFiles*)(pComOutputPref.p))->get_PrinterConfigPath(&bstDefaultFilePath);
	if(FAILED(hRes))
		return false;
	printer_path = CString(bstDefaultFilePath);
	return true;
}

void UpdatePrinterConfig()
{
	CString str_dest_path;
	TCHAR sz_soure_path[MAX_PATH] = {0};
	if(!GetPrinterPath(str_dest_path))
	{
		return;
	}
	CStringArray config_file_arr;
	config_file_arr.Add(_T("DWG To PDF.pc3"));
	int i = 0;
	int count = (int)config_file_arr.GetSize();
	for(i = 0; i < count; ++i)
	{
		CString str_conifg_file = config_file_arr[i];
		if(RTNORM != acedFindFile(str_conifg_file, sz_soure_path))
		{
			return;
		}

		CPath path;
		path.Combine(str_dest_path, str_conifg_file);
		CopyFile(sz_soure_path, path.m_strPath, false);
	}

	CStringArray pmp_config_file_arr;
	pmp_config_file_arr.Add(_T("DWG To PDF.pmp"));
	count = (int)pmp_config_file_arr.GetSize();
	for(i = 0; i < count; ++i)
	{
		CString str_conifg_file = pmp_config_file_arr[i];
		if(RTNORM != acedFindFile(str_conifg_file, sz_soure_path))
		{
			return;
		}

		CPath path;
		path.Combine(str_dest_path, _T("PMP Files"));
		path.Combine(path.m_strPath, str_conifg_file);
		CopyFile(sz_soure_path, path.m_strPath, false);
	}
}

int getSystemVersion()
{
	return 0;
}

void modifyEntArrowSize(const AcDbObjectId &id, double height)
{
	AcDbEntity * pEnt = NULL; Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
	if(es != Acad::eOk) return;
	if(pEnt->isKindOf(AcDbLeader::desc()))
	{
		AcDbLeader *pBlockRef = AcDbLeader::cast(pEnt);
		pBlockRef->setDimasz(2.0);
		pBlockRef->setDimscale(height);
	}
	else if(pEnt->isKindOf(AcDbDimension::desc()))
	{
		AcDbDimension *pDim = AcDbDimension::cast(pEnt);
		pDim->setDimasz(2.0);
		pDim->setDimscale(height);
	}
	pEnt->setLinetypeScale(height);
	pEnt->close();
}

void modifyEntCircleSize(const AcDbObjectId &id, double size)
{
	AcDbEntity *pEnt = NULL; Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
	if(es != Acad::eOk) return;
	if(pEnt->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *circle = AcDbCircle::cast(pEnt);
		circle->setRadius(size / 2);
	}
	pEnt->setLinetypeScale(size);
	pEnt->close();
}

void modifyEntTextHeight(const AcDbObjectId &id, double height)
{
	AcDbEntity * pEnt = NULL; Acad::ErrorStatus es;
	es = acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
	if(es != Acad::eOk) return;
	if(pEnt->isKindOf(AcDbMText::desc()))
	{
		AcDbMText *pMText = AcDbMText::cast(pEnt);
		pMText->setTextHeight(height);
	}
	else if(pEnt->isKindOf(AcDbText::desc()))
	{
		AcDbText *pMText = AcDbText::cast(pEnt);
		pMText->setHeight(height);
	}
	pEnt->setLinetypeScale(height / 1.679);
	pEnt->close();
}

void modifyEntsArrowSize(const AcDbObjectIdArray & ids, double height)
{
	acedSetStatusBarProgressMeter(_T("正在修改箭头的的大小..."), 0, ids.length());
	acDocManager->lockDocument(acDocManager->curDocument());
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		modifyEntArrowSize(ids.at(idx), height);
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	acedRestoreStatusBar();

}

void modifyEntsArrowSize(const AcDbObjectIdArray & ids, const TextProperty &prop)
{
	acedSetStatusBarProgressMeter(_T("正在修改箭头的的大小..."), 0, ids.length());
	acDocManager->lockDocument(acDocManager->curDocument());
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = ids.at(idx);
		AcDbEntity * pEnt = NULL; Acad::ErrorStatus es;
		es = acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
		if(es != Acad::eOk) return;
		if(pEnt->isKindOf(AcDbLeader::desc()))
		{
			AcDbLeader *pBlockRef = AcDbLeader::cast(pEnt);
			pBlockRef->setDimasz(prop._size);
			pBlockRef->setDimscale(1.0);
		}
		else if(pEnt->isKindOf(AcDbDimension::desc()))
		{
			AcDbDimension *pDim = AcDbDimension::cast(pEnt);
			pDim->setDimasz(prop._size);
			pDim->setDimscale(1.0);
		}
		pEnt->setLinetypeScale(1.0);
		pEnt->close();
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	acedRestoreStatusBar();
}

void modifyEntsTextHeight(const AcDbObjectIdArray & ids, double height)
{
	acedSetStatusBarProgressMeter(_T("修改字体大小..."), 0, ids.length());
	acDocManager->lockDocument(acDocManager->curDocument());
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		modifyEntTextHeight(ids.at(idx), height);
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	acedRestoreStatusBar();
}

void getJGLayerNum(const CString &layername, int &slayer, int &elayer)
{
	CString lname;
	if(layername.Mid(0, 1) != _T('-') && layername.Find(_T("--->")) == -1 && stringIsDigit(layername) == false)
	{
		lname.Format(_T("%s"), g_fieldchagetable.getIndexByName(layername));
		slayer = elayer = _ttoi(lname);
	}
	else
		getLayerNum(layername, slayer, elayer);
}

void getStrAndNumByString(const CString &text, CString &str, int &num)
{
	int len = text.GetLength();
	CString substr, tmpstr;
	bool lx = true; num = 0; bool bStr = false;
	int laststr = -1, lastnum = -1, idx = 0;
	for(idx = len - 1; idx >= 0; --idx)
	{
		substr = text.Mid(idx, len - idx);
		if(stringIsDigit(substr) == true) continue;
		substr = text.Mid(idx + 1, len - idx - 1);
		num = _ttoi(substr); str = text.Mid(0, idx + 1);
		bStr = true; break;
	}
	if(bStr == false)
		num = _ttoi(text);
	num = num == 0 ? 1 : num;
}

double square2dDist_forBorders(AcGePoint2d pt1, AcGePoint2d pt2)
{
	return (pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y);
}

int CompareNode(AcGePoint2d pt1, AcGePoint2d pt2)
{
	if(fabs(pt1.x - pt2.x) < EPS && fabs(pt1.y - pt2.y) < EPS)
		return 0;
	if(fabs(pt1.x - pt2.x) < EPS)
	{
		if(pt1.y < pt2.y)
			return -1;
		else
			return 1;
	}
	else
	{
		if(pt1.x < pt2.x)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
}

bool FindNode(AcGePoint2dArray & nodes, AcGePoint2d pt, int &nIdx)
{
	int low = 0, high = nodes.length() - 1;
	int mid;

	while(low <= high)
	{
		mid = (low + high) / 2;
		int es = CompareNode(nodes[mid], pt);

		if(0 == es)
		{
			nIdx = mid;
			return true;
		}
		else if(-1 == es)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}

	nIdx = low;
	return false;
}

void GetBoders(AcDbObjectIdArray & idArr, CArray< AcGePoint2dArray, AcGePoint2dArray > & m_arrLoops,
	CArray< AcGeDoubleArray, AcGeDoubleArray > & m_arrBulges, bool bMax/* = false*/)
{
	AcGePoint2dArray nodesMin, nodesMax, nodesR;
	CArray< AcGePoint2dArray, AcGePoint2dArray > arrNodes;
	CArray< AcGeDoubleArray, AcGeDoubleArray > arrBulges;
	int i = 0;
	for(i = 0; i < idArr.length(); i++)
	{
		int j = 0;
		AcGePoint2dArray nodes;
		AcGeDoubleArray bulges;
		AcDbObjectId idAdd;
		ads_name entIdx;
		double dLength = 0;
		acdbGetAdsName(entIdx, idArr[i]);

		GetEntLength(entIdx, dLength);
		if(dLength < 1e-7)
			continue;
		GetPlList(entIdx, nodes, true);
		GetPlBulges(entIdx, bulges, true);
		if(bulges.length() == 0)
		{
			for(int nIndex = 0; nIndex < nodes.length(); nIndex++)
			{
				bulges.append(0.0);
			}
		}
		if(distOF2d(nodes.first(), nodes.last()) > 0.001)
		{
			nodes.append(nodes.last());
		}
		if(GetPolyType(entIdx, true) >= 10
			&& square2dDist_forBorders(nodes.first(), nodes.last()) > 1e-12)
		{
			nodes.append(nodes.first());
			bulges.append(0);
		}
		//过滤细微点
		for(j = 0; j < nodes.length(); j++)
		{
			int nIdx = 0;
			if(FindNode(nodesR, nodes[j], nIdx))
			{
				nodes.setAt(j, nodesR[nIdx]);
			}
			else
			{
				nodesR.insertAt(nIdx, nodes[j]);
			}
		}
		arrNodes.Add(nodes);
		arrBulges.Add(bulges);
	}

	CAutoDiv autoD;

	set<Line2P> arrLineIn;
	for(i = 0; i < arrNodes.GetSize(); i++)
	{
		for(int j = 0; j < arrNodes[i].length() - 1; j++)
		{
			Line2P line2P(arrNodes[i][j], arrNodes[i][j + 1], arrBulges[i][j]);
			// 			line2P.m_ptS = ;
			// 			line2P.m_ptE = ;
			// 			line2P.m_dBulge = ;
			line2P.m_bCompareWithDirect = false;
			arrLineIn.insert(line2P);
		}
	}

	set<Line2P>::iterator theIterator;
	for(theIterator = arrLineIn.begin(); theIterator != arrLineIn.end(); theIterator++)
	{
		AcGePoint2dArray nodesA;
		AcGeDoubleArray bulgeA;
		AcGePoint2dArray nodesB;
		AcGeDoubleArray bulgeB;
		nodesA.append(theIterator->m_ptS);
		nodesA.append(theIterator->m_ptE);
		bulgeA.append(theIterator->m_dBulge);
		bulgeA.append(0);

		autoD.AddNodeArr(nodesA, bulgeA, nodesB, bulgeB);
		//DrawPolyline2D(nodesD,10,false,_T(""),0.0,0,NULL);
	}

	//autoD.AddNodeArr( arrNodes , arrBulges , true );

	autoD.RunDiv(false);
	autoD.ShowMePolyline();

	CAutoLoop autoL;

	for(i = 0; i < autoD.m_arrNodesOut.GetSize(); i++)
	{
		autoL.AddEdge(autoD.m_arrNodesOut[i], autoD.m_arrBulgeOut[i]);
	}

	if(bMax)
		autoL.GetBulgesDoubleLoops(AUTO_BORDER);
	else
		autoL.GetBulgesDoubleLoops();

	m_arrLoops.Append(autoL.m_arrLoops);
	m_arrBulges.Append(autoL.m_arrLoopsBulge);

}

bool GetBD(const CString &szLayer, AcDbObjectId &objid, ads_point ptInsert, bool bMax)
{
	long i = 0;
	objid = NULL;
	AcDbObjectIdArray objs;
	CArray<AcGePoint2dArray, AcGePoint2dArray> arrNodes, arrNodesOut;
	CArray<AcGeDoubleArray, AcGeDoubleArray> arrBulges, arrBulgesOut;
	acutPrintf(_T("\n选择参与构面的实体:"));
	SelectFilter sf(8, szLayer);
	if(false == SelectEntitys(objs, sf, _T(""))) return false;
	if(objs.length() <= 0) return false;
	AcDbObjectIdArray ids;
	for(i = 0; i < objs.length(); i++)
	{
		AcDbObjectId obj = objs.at(i);
		AcGePoint2dArray arrNodesT;
		AcGeDoubleArray arrBulgesT;
		int nType = GetPolyType(aname(obj), true);
		GetPlBulges(aname(obj), arrBulgesT, true);
		if(GetPlList(aname(obj), arrNodesT, true) == false)
		{
			AcGePoint3dArray node3d;
			GetPlList3d(obj, node3d);
			arrNodesT.setLogicalLength(0);
			arrBulgesT.setLogicalLength(0);
			for(int i = 0; i < node3d.length(); i++)
			{
				arrNodesT.append(AcGePoint2d(node3d.at(i).x, node3d.at(i).y));
				arrBulgesT.append(0.0);
			}
		}
		ids.append(obj);
		if(arrBulgesT.length() == 0)
		{
			for(int i = 0; i < arrNodesT.length(); i++)
			{
				arrBulgesT.append(0.0);
			}
		}
		if(arrBulgesT.length() != arrNodesT.length())
			continue;
		if(nType >= 10 && (distOF2d(arrNodesT.first(), arrNodesT.last())>EPS))
		{
			arrNodesT.append(arrNodesT.first());
			arrBulgesT.append(0);
		}
		arrNodes.Add(arrNodesT);
		arrBulges.Add(arrBulgesT);
	}
	if(arrNodes.GetSize() > 0)
		GetBoders(ids, arrNodesOut, arrBulgesOut, bMax);
	else if(arrNodes.GetSize() == 1)
	{
		if(arrNodes[0].length() >= 3)
		{
			arrNodesOut.Add(arrNodes.GetAt(0));
		}
	}
	acutPrintf(_T("完成构面%d个"), arrNodesOut.GetSize());
	if(0 == arrNodesOut.GetSize()) return false;
	ads_point ptIst = {0, 0, 0};
	TCHAR szStr[25] = {0};
	int count = 0; ads_name ent;
	while(RTNORM == ads_getpoint(NULL, _T("\n选择内部一点"), ptIst))
	{
		AcGePoint2d ptIn;
		ptIn.x = ptIst[X];
		ptIn.y = ptIst[Y];
		for(int k = 0; k < arrNodesOut.GetSize(); k++)
		{
			AcGePoint2dArray nodes = arrNodesOut.GetAt(k);
			AcGeDoubleArray bugles = arrBulgesOut.GetAt(k);
			if(nodes.length() <= 2)
				continue;
			int isIn = -100;

			isIn = PointInPolygon(nodes, bugles, ptIn);
			if(isIn != -1)continue;
			bool bCorrect = true;
			DrawPolyline2D(nodes, 0, true, _T(""), 0, 0, szLayer);//绘制面;
			ads_entlast(ent);
			if(k < arrBulgesOut.GetSize())
			{
				SetPolylineBulge(ent, arrBulgesOut.GetAt(k));
			}
			AcDbObjectIdArray tids;
			actrTransactionManager->startTransaction();
			DrawHatch(ent, _T("411b"), 1.0, 0.0, &tids, 4, _T("0"));
			int es = ads_getstring(0, _T("\n是否正确?(Y/N) <Y>"), szStr);
			if(RTCAN == es || !_tcsnicmp(szStr, _T("N"), 1)) bCorrect = false;
			actrTransactionManager->abortTransaction();
			if(false == bCorrect)
			{
				ads_entdel(ent);
				continue;
			}
			if(Acad::eOk != acdbGetObjectId(objid, ent))
				continue;
			count++;
			ptInsert[X] = ptIst[X];
			ptInsert[Y] = ptIst[Y];
			break;
		}
		if(count > 0)
			return true;
	}
	return false;
}

int get_extension(const char *img)
{
	string fname(img);
	int pos = (int)fname.find_last_of('.');
	if(pos == -1) return 0;
	fname = fname.substr(pos + 1);
	if(fname.compare("gif") == 0) return 1;
	else if(fname.compare("jpg") == 0) return 2;
	else if(fname.compare("png") == 0) return 3;
	else if(fname.compare("bmp") == 0) return 4;
	else return 0;
	char c = fname.at(fname.length() - 1);
	char c2 = fname.at(fname.length() - 3);
	if((c == 'f') && (c2 == 'g'))
		return 1;
	else if((c == 'g') && (c2 == 'j'))
		return 2;
	else if((c == 'g') && (c2 == 'p'))
		return 3;
	else if((c == 'p') && (c2 == 'b'))
		return 4;
	return 0;
}

bool getImagePngSize(const char *img, int &widht, int &height)
{
	ifstream ffin(img, std::ios::binary);
	if(!ffin)
	{
		AfxMessageBox(_T("无法打开图片"));
		return false;
	}
	int result = get_extension(img);
	char s1[2] = {0}, s2[2] = {0};
	switch(result)
	{
		case 1:
			ffin.seekg(6);
			ffin.read(s1, 2);
			ffin.read(s2, 2);
			break;
		case 2:
			ffin.seekg(164);
			ffin.read(s1, 2);
			ffin.read(s2, 2);
			break;
		case 3:
			ffin.seekg(17);
			ffin.read(s1, 2);
			ffin.seekg(2, std::ios::cur);
			ffin.read(s2, 2);
			break;
		case 4:
			ffin.seekg(18);
			ffin.read(s1, 2);
			ffin.seekg(2, std::ios::cur);
			ffin.read(s2, 2);
			break;
		default:
			ffin.close(); return false;
	}
	widht = (unsigned int)(s1[1]) << 8 | (unsigned int)(s1[0]);
	height = (unsigned int)(s2[1]) << 8 | (unsigned int)(s2[0]);
	ffin.close(); return true;
}

void AlignedNote(ads_point pt1, ads_point pt2, double dist, double dOffset)
{
	AcDbObjectId objId;
	ads_point pt;
	TCHAR szDist[50] = {0};
	if(fabs(pt1[Y] - pt2[Y]) < 1e-6)
	{
		pt[X] = (pt1[X] + pt2[X]) / 2;
		pt[Y] = (pt1[Y] + pt2[Y]) / 2;
		pt[Y] += dOffset;
	}
	else
	{
		pt[X] = max(pt1[X], pt2[X]);
		pt[Y] = (pt1[Y] + pt2[Y]) / 2;
	}

	if(fabs(dist - 0) < 1.0e-6)
		dist = distOF2d(asPnt2d(pt1), asPnt2d(pt2));
	_stprintf(szDist, _T("%.2f"), dist);

	// 	AcDbDatabase *pcurdb=acdbHostApplicationServices()->workingDatabase();
	// 	AcDbDimStyleTable *pnewdimtable;
	// 	pcurdb->getSymbolTable(pnewdimtable,AcDb::kForWrite);
	// 	AcDbDimStyleTableRecord *pnewdimrecord=new AcDbDimStyleTableRecord();
	// 	pnewdimrecord->setDimasz(20);//设置箭头大小
	// 	pnewdimrecord->setDimblk1(_T("建筑标记"));
	// 		pnewdimrecord->setDimblk2(_T("建筑标记"));
	// 		pnewdimrecord->setDimblk(_T("建筑标记"));
	// 		pnewdimrecord->setDimldrblk(_T("建筑标记"));
	// 	pnewdimrecord->close();
	// 	delete pcurdb;

	AcDbAlignedDimension *pAligDim = new AcDbAlignedDimension(asPnt3d(pt1),
		asPnt3d(pt2), asPnt3d(pt), szDist);
	AppendEntity(pAligDim, objId);
	pAligDim->close();
}

bool IsScDMX(ads_name ent)
{
	TCHAR szStdm[20] = {0};
	GetStdm(ent, szStdm);
	if(_tcsicmp(szStdm, SCDMX) == 0)
		return true;
	return false;
}

bool IsScDMX(const AcDbObjectId &id)
{
	ads_name ent;
	acdbGetAdsName(ent, id);
	return IsScDMX(ent);
}

bool IsSjDMX(ads_name ent)
{
	TCHAR szStdm[20] = {0};
	GetStdm(ent, szStdm);
	if(_tcsicmp(szStdm, SJDMX) == 0)
		return true;
	return false;
}

bool IsSjDMX(const AcDbObjectId &id)
{
	ads_name ent;
	acdbGetAdsName(ent, id);
	return IsSjDMX(ent);
}

bool IsScDlzxx(ads_name ent)
{
	TCHAR szStdm[20] = {0};
	GetStdm(ent, szStdm);
	if(_tcsicmp(szStdm, SCDLZXX) == 0)
		return true;
	return false;
}

bool IsScDlzxx(AcDbObjectId id)
{
	ads_name ent;
	acdbGetAdsName(ent, id);
	return IsScDlzxx(ent);
}

bool IsSjDlzxx(ads_name ent)
{
	TCHAR szStdm[20] = {0};
	GetStdm(ent, szStdm);
	if(_tcsicmp(szStdm, SJDLZXX) == 0)
		return true;
	return false;
}
bool IsSjDlzxx(AcDbObjectId id)
{
	ads_name ent;
	acdbGetAdsName(ent, id);
	return IsSjDlzxx(ent);
}
bool IsScDlbx(ads_name ent)
{
	TCHAR szStdm[20] = {0};
	GetStdm(ent, szStdm);
	if(_tcsicmp(szStdm, SCDLBX) == 0)
		return true;
	return false;
}
bool IsScDlbx(AcDbObjectId id)
{
	ads_name ent;
	acdbGetAdsName(ent, id);
	return IsScDlbx(ent);
}
bool IsSjDlbx(ads_name ent)
{
	TCHAR szStdm[20] = {0};
	GetStdm(ent, szStdm);
	if(_tcsicmp(szStdm, SJDLBX) == 0)
		return true;
	return false;
}
bool IsSjDlbx(AcDbObjectId id)
{
	ads_name ent;
	acdbGetAdsName(ent, id);
	return IsSjDlbx(ent);
}

bool changeTextByXData(AcDbObjectIdArray &ids, const TCHAR *AppName, const TCHAR *xData, const TCHAR *val)
{
	acedSetStatusBarProgressMeter(_T("正在修改实体的文本。。。"), 0, ids.length());
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		changeTextByXData(ids.at(idx), AppName, xData, val);
	}
	return true;
}

bool changeTextByXData(AcDbObjectId &id, const TCHAR *AppName, const TCHAR *xData, const TCHAR *val)
{
	ads_name ent;
	if(Acad::eOk != acdbGetAdsName(ent, id)) return false;

	TCHAR info[255] = {0};
	if(false == ReadXdata(ent, AppName, 0, info)) return false;
	if(_tcscmp(info, xData) != 0) return false;

	//获得原来文字的对齐方式
	CString strAlign = GetTextAlign(id);
	SetEntText(ent, val);//设置实体的文本
	TCHAR szAlign[200] = {0};
	_stprintf(szAlign, _T("%s"), strAlign);
	SetTextAlign(id, szAlign);//重新设置原来的对齐方式
	return true;
}

void modifyCGFrame()
{
	SelectFilter sf(8, _T("TK")); AcDbObjectIdArray ids;
	SelectEntitys(ids, sf, _T("X")); ads_point spt, ept;
	double dscale = 1.0; CXRecordOperate oper; int dtype = 1;
	if(ids.length() == 0)
	{
		MessageBox(GetForegroundWindow(), _T("此命令仅仅用于修改图框大小，不能用于直接套图\n框，如果想套图框，请执行相应模块下的成果输出，谢谢。"), _T("错误提示"), 0);
		return;

	}
	else
	{
		oper.ReadDoubleRecord(_T("GCCL"), _T("BLC"), dscale);
		oper.ReadIntegerRecord(_T("GCCL"), _T("TKLX"), dtype);
		GetEntsExtent(ids, spt, ept);
	}
	int type = 0; double scale;
	double width = ept[X] - spt[X]; width = (width < 0 ? -width : width) / dscale;
	double height = ept[Y] - spt[Y]; height = (height < 0 ? -height : height) / dscale;
	if(RTCAN == ads_getint(_T("\n 请选择出图的大小[1]A4、[2]A3、[3]A2、[4]A1、[5]A0：<1>"), &type)) return;
	if(type > 5) type = 0; scale = pow(1.4141414141414144, dtype - type); dscale *= scale;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	AcDbObjectIdArray oids, tkids; SelectFilter(8, _T("TK"));
	SelectEntitys(tkids, sf, _T("X")); SelectEntitys(oids, _T("X"));
	for(int idx = 0; idx < tkids.length(); ++idx)
		oids.remove(tkids.at(idx));
	TCHAR SCALE[255] = {0};
	int f = round(dscale * 10); f *= 100;
	_stprintf(SCALE, _T("比例尺1:%d"), f);
	dscale = f * 1.0 / 1000;
	modifyEntsTextHeight(oids, dscale * 1.679);
	modifyEntsArrowSize(oids, dscale);
	changeTextByXData(tkids, _T("SCALE"), _T("SCALE"), SCALE);
	oper.AddDoubleRecord(_T("GCCL"), _T("BLC"), dscale);
	oper.AddIntegerRecord(_T("GCCL"), _T("TKLX"), type);
}

CBackScreen::CBackScreen()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
}

CBackScreen::~CBackScreen()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
}

void deleteOtherAllEntitys(const AcDbObjectIdArray &ids)
{
	ads_command(RTSTR, _T("ZOOM"), RTSTR, _T("E"), RTNONE);
	ads_name ssName; ads_ssget(_T("X"), NULL, NULL, NULL, ssName);
	AcDbObjectIdArray delArr; SSToIdArr(ssName, delArr);
	for(int index = 0; index < ids.length(); ++index)
		delArr.remove(ids.at(index));
	EntsDel(delArr);
}

void addRoadHDMTuLi(const AcDbObjectId &id, const CString &block)
{
	AcGePoint2dArray node; AcGePoint3d pos;
	GetPlList(aname(id), node);
	GetMiddlePoint(id, pos); pos.y += 5;
	double len = distOF2d(node.first(), node.last());
	if(false == DrawInsert(apoint(pos), block, _T(""), 1.0)) return;
	ads_name ent; ads_entlast(ent);
	ads_point ptMin, ptMax;
	GetEntExtent(ent, ptMin, ptMax, 0.5);
	double width = ptMax[X] - ptMin[X];
	double scale = len * 1.0 / width;
	ScaleEntity(ent, pos, scale);
}

AcDbObjectId GetObjectIdByHandle(TCHAR *szHandle)
{
	AcDbObjectId id = AcDbObjectId::kNull;
	ads_name ent;
	int nRes = acdbHandEnt(szHandle, ent);
	if(nRes != RTNORM)
		return AcDbObjectId::kNull;
	if(Acad::eOk != acdbGetObjectId(id, ent))
	{
		return AcDbObjectId::kNull;
	}
	return id;
}

bool isLayerExistinDb(const CString &layer)
{
	if(layer.GetLength() == 0) return false;
	AcDbLayerTable *pLayerTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTbl, AcDb::kForWrite);
	bool flag = pLayerTbl->has(layer);
	pLayerTbl->close(); return flag;
}

bool createLayer(const CString &layer)
{
	if(isLayerExistinDb(layer)) return true;
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTbl, AcDb::kForWrite);
	AcDbLayerTableRecord *pLayerTblRcd = new AcDbLayerTableRecord;
	if(Acad::eOk != pLayerTblRcd->setName(layer))
	{
		pLayerTbl->close();
		delete pLayerTblRcd;
		return false;
	}
	if(Acad::eOk != pLayerTbl->add(pLayerTblRcd))
	{
		pLayerTbl->close();
		delete pLayerTblRcd;
		return false;
	}
	pLayerTblRcd->close();
	pLayerTbl->close();
	return true;
}

CloseLayer::CloseLayer(const CString &name)
{
	IDataBaseOper oper; CStringArray layers;
	CString info = oper.readCommonTable(name);
	AnalyzeData(m_layers, info, _T(";"));
	AcDbLayerTableRecord *pLayerTblRcd; AcDbLayerTable * pLayerTable;
	if(acdbHostApplicationServices() == NULL || acdbHostApplicationServices()->workingDatabase() == NULL) return;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if(es != Acad::eOk) { acutPrintf(acadErrorStatusTextChs(es)); return; }
	AcDbLayerTableIterator *it = NULL;
	for(int idx = 0; idx < m_layers.GetSize(); ++idx)
	{
		CString layer = m_layers.GetAt(idx);
		es = pLayerTable->newIterator(it);
		if(es != Acad::eOk) { acutPrintf(acadErrorStatusTextChs(es)); return; }
		for(it->start(); !it->done(); it->step())
		{
			it->getRecord(pLayerTblRcd, AcDb::kForWrite);
			TCHAR *szLyr = NULL; pLayerTblRcd->getName(szLyr);
			if(layer.CompareNoCase(szLyr) == 0)
			{
				pLayerTblRcd->setIsOff(true);
				if(es != Acad::eOk) { acutPrintf(acadErrorStatusTextChs(es)); return; }
				pLayerTblRcd->close(); break;
			}
			pLayerTblRcd->close();
		}
		delete it; it = NULL;
	}
	pLayerTable->close();
}

CloseLayer::~CloseLayer()
{
	AcDbLayerTableRecord *pLayerTblRcd; AcDbLayerTable * pLayerTable;
	if(acdbHostApplicationServices() == NULL || acdbHostApplicationServices()->workingDatabase() == NULL) return;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	AcDbLayerTableIterator *it = NULL;
	for(int idx = 0; idx < m_layers.GetSize(); ++idx)
	{
		CString layer = m_layers.GetAt(idx);
		pLayerTable->newIterator(it);
		for(it->start(); !it->done(); it->step())
		{
			it->getRecord(pLayerTblRcd, AcDb::kForWrite);
			TCHAR *szLyr = NULL; pLayerTblRcd->getName(szLyr);
			if(layer.CompareNoCase(szLyr) == 0)
			{
				pLayerTblRcd->setIsOff(false);
				pLayerTblRcd->close(); break;
			}
			pLayerTblRcd->close();
		}
		delete it; it = NULL;
	}
	pLayerTable->close();
}

AcDbObjectId createBoundary(ads_point pt)
{
	ads_name last, cur; ads_entlast(last); AcDbObjectId id = AcDbObjectId::kNull;
	int ret = ads_command(RTSTR, _T("-BOUNDARY"), RTSTR, _T("A"), RTSTR, _T("O"), RTSTR, _T("P"), RTSTR, _T(""), RTPOINT, pt, RTSTR, _T(""), RTNONE);
	ads_entlast(cur); if(ads_name_equal(last, cur)) return id; acdbGetObjectId(id, cur); return id;
}

LayerOperator::LayerOperator()
{

}

LayerOperator::~LayerOperator()
{

}

void LayerOperator::open(const CString &lyr, bool other)
{
	VStr lyrs; lyrs.push_back(lyr);
	opens(lyrs, other);
}

void LayerOperator::opens(VStr &lyrs, bool other)
{
	AcDbLayerTable * pLayerTable; AcDbLayerTableRecord *pLayerTblRcd;
	if(acdbHostApplicationServices() == NULL || acdbHostApplicationServices()->workingDatabase() == NULL)
		return;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	AcDbLayerTableIterator *it = NULL; pLayerTable->newIterator(it);
	for(it->start(); !it->done(); it->step())
	{
		it->getRecord(pLayerTblRcd, AcDb::kForWrite);
		TCHAR *szLayer = NULL;
		pLayerTblRcd->getName(szLayer);
		if(lyrs.size() == 0)
			pLayerTblRcd->setIsOff(false);
		else
		{
			VSIter lyrIter = find(lyrs.begin(), lyrs.end(), CString(szLayer));
			if(other)
			{
				if(lyrIter == lyrs.end())
					pLayerTblRcd->setIsOff(false);
				else
					pLayerTblRcd->setIsOff(true);
			}
			else
				if(lyrIter != lyrs.end())
					pLayerTblRcd->setIsOff(false);
				else
					pLayerTblRcd->setIsOff(true);
		}
		pLayerTblRcd->close();
	}
	delete it; it = NULL;
	pLayerTable->close();
}

void LayerOperator::close(const CString &lyr, bool other)
{
	VStr lyrs; lyrs.push_back(lyr);
	closes(lyrs, other);
}

void LayerOperator::closes(VStr &lyrs, bool other)
{
	AcDbLayerTable * pLayerTable; AcDbLayerTableRecord *pLayerTblRcd;
	if(acdbHostApplicationServices() == NULL || acdbHostApplicationServices()->workingDatabase() == NULL)
		return;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	AcDbLayerTableIterator *it = NULL; pLayerTable->newIterator(it);
	for(it->start(); !it->done(); it->step())
	{
		it->getRecord(pLayerTblRcd, AcDb::kForWrite);
		TCHAR *szLayer = NULL;
		pLayerTblRcd->getName(szLayer);
		if(lyrs.size() == 0)
			pLayerTblRcd->setIsOff(true);
		else
		{
			VSIter lyrIter = find(lyrs.begin(), lyrs.end(), CString(szLayer));
			if(other)
			{
				if(lyrIter == lyrs.end())
					pLayerTblRcd->setIsOff(true);
				else
					pLayerTblRcd->setIsOff(false);
			}
			else
			{
				if(lyrIter != lyrs.end())
					pLayerTblRcd->setIsOff(true);
				else
					pLayerTblRcd->setIsOff(false);
			}
		}
		pLayerTblRcd->close();
	}
	delete it; it = NULL;
	pLayerTable->close();
}

bool LayerOperator::frozen(const CString &lyr, bool isclose)
{
	return false;
}

bool LayerOperator::frozens(const VStr &lyrs, bool isclose)
{
	return false;
}

CmdEcho::CmdEcho()
{
	ads_command(RTSTR, _T("CMDECHO"), RTSTR, _T("0"), RTNONE);
}

CmdEcho::~CmdEcho()
{
	ads_command(RTSTR, _T("CMDECHO"), RTSTR, _T("1"), RTNONE);
}