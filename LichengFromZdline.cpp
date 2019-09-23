// LichengFromZdline.cpp: implementation of the CLichengFromZdline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "LichengFromZdline.h"
#include "LichengExcel.h"
#include "JoinTriangles.h"
#include "LichengExcel.h"
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//用户目录
extern TCHAR g_szUsePath[MAX_PATH];
//把实数转换为里程形式
extern void ConvertDoubleToKplus(double dRRR, TCHAR *szStr, int nWidth, TCHAR *szLCZMC, bool bMid = false);
//提取给定线经过实体的里程高程
extern void ThreeD_GetEntityHeightAlongLine(ads_name entPl,CJoinTriangles &sjw,AcGePoint3dArray &dw_lcNodes,AcGeDoubleArray &dw_lcDists);
//将图面高程点写入数据文件
extern int getgchfile(TCHAR *szFileName,int nSel,bool bDtm=true,bool bText=false);

/////////////////////////////////////////////////////////////////////////////
// CHeightDlg dialog


//CHeightDlg::CHeightDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CHeightDlg::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CHeightDlg)
//	m_height = 0.0;
//	//}}AFX_DATA_INIT
//	m_left = 0;
//	m_top = 0;
//}


//void CHeightDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CHeightDlg)
//	DDX_Text(pDX, IDC_HEIGHT, m_height);
//	//}}AFX_DATA_MAP
//}


//BEGIN_MESSAGE_MAP(CHeightDlg, CDialog)
//	//{{AFX_MSG_MAP(CHeightDlg)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeightDlg message handlers

//BOOL CHeightDlg::OnInitDialog() 
//{
//	CDialog::OnInitDialog();
//	
//	CRect lpRect;
//	int difX,difY;
//	GetWindowRect(lpRect);
//	difX=m_left-lpRect.left; difY=m_top-lpRect.top;
//	lpRect.top=m_top; lpRect.left=m_left; lpRect.bottom+=difY; lpRect.right+=difX;
//	MoveWindow(lpRect);
//	
//	return TRUE;  // return TRUE unless you set the focus to a control
//	              // EXCEPTION: OCX Property Pages should return FALSE
//}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLichengFromZdline::CLichengFromZdline()
{

}

CLichengFromZdline::~CLichengFromZdline()
{

}

bool CLichengFromZdline::SelectMainLine(ads_name ent)
{
	ads_point pt;
	if ( ads_entsel(_T("\n选择纵断面线"),ent,pt) != RTNORM ) return false;
	int nPolyType = GetPolyType(ent,true);
	if ( nPolyType < 0 ) { 
		ads_alert(_T("没有选中复合线")); return false; 
	}
	if ( nPolyType >= 10 ) {
		ads_alert(_T("复合线不可闭合")); return false;
	}
	return true;
}

void CLichengFromZdline::AddHdmLine(ads_point pt0,double dAngle,double dLeft,double dRight,int nColor)
{
	ads_point pt1, pt2, pt3, pt4, pt5, pt6;
	AcDbGroup *pGroup=NULL;
	AcDbObjectId idLine;
	AcDbObjectIdArray ids;
	AcGePoint2dArray Nodes;
	TCHAR szSTBM[50] = { 0 };

	//删除原有断面线
	pt1[X] = pt0[X] - 2.5; pt1[Y] = pt0[Y] + 2.5; Nodes.append(apoint(pt1));
	pt2[X] = pt0[X] - 2.5; pt2[Y] = pt0[Y] - 2.5; Nodes.append(apoint(pt2));
	pt3[X] = pt0[X] + 2.5; pt3[Y] = pt0[Y] - 2.5; Nodes.append(apoint(pt3));
	pt4[X] = pt0[X] + 2.5; pt4[Y] = pt0[Y] + 2.5; Nodes.append(apoint(pt4));
	ssFromNodes(ids, Nodes, 2);
	TCHAR szCompare[255] = { 0 };
	if (m_nSCorSJ == 1) _tcscpy(szCompare, _T("992001"));
	else _tcscpy(szCompare, _T("992000"));
	for (int j = 0; j < ids.length(); j++)
	{
		TCHAR szSTBM[50] = { 0 };
		AcDbObjectId id = ids[j];
		GetStdm(aname(id), szSTBM);
		CStringArray strs1;
		if (!_tcscmp(szSTBM, szCompare))
		{
			EntDel(aname(id));
			break;
		}
	}

	ads_polar(pt0,dAngle+PI/2.0,dLeft,pt5);
	ads_polar(pt0,dAngle+3*PI/2.0,dRight,pt6);
	if ( Acad::eOk != acdbOpenObject(pGroup, m_groupId, AcDb::kForWrite) ) return;
	if (m_nSCorSJ == 1)
		idLine = drawline(pt5, pt6, _T("992001"), pGroup, _T("dmx")); //设计横断面线编码
	else
		idLine = drawline(pt5, pt6, _T("992000"), pGroup, _T("dmx")); //实测横断面线编码
	if (nColor != -1)
		setcolor(aname(idLine), nColor);
	pGroup->close();
}

void CLichengFromZdline::AddHdmLine2(ads_point pt0, double dAngle, double dLeft, double dRight)
{
	ads_point pt1, pt2;
	AcDbGroup *pGroup = NULL;
	ads_polar(pt0, dAngle + PI, dLeft, pt1);
	ads_polar(pt0, dAngle + 2 * PI, dRight, pt2);
	if (Acad::eOk != acdbOpenObject(pGroup, m_groupId, AcDb::kForWrite)) return;
	if (m_nSCorSJ == 1)
		drawline(pt1, pt2, _T("992001"), pGroup, _T("dmx")); //设计横断面线编码
	else
		drawline(pt1, pt2, _T("992000"), pGroup, _T("dmx")); //实测横断面线编码
	pGroup->close();
}

double CLichengFromZdline::GetMiddleAngle(ads_point pt1,ads_point pt,ads_point pt2)
{
	double dAngle1 = ads_angle(pt,pt1);
	double dAngle2 = ads_angle(pt,pt2);
	double dInter = dAngle2 - dAngle1;
	if ( dInter < 0.0 ) dInter += PI*2;
	double dAngle = dAngle1 + dInter/2;
	return dAngle;
}

bool CLichengFromZdline::GetPointDeriv(AcDbCurve *pCurve,AcGePoint3d pt3d,double &dAngle)
{
	AcGeVector3d vector;
	AcGePoint3d pt3dE;
	if ( Acad::eOk != pCurve->getFirstDeriv(pt3d,vector) ) return false;
	pt3dE = pt3d + vector;
	dAngle = ads_angle(asDblArray(pt3d),asDblArray(pt3dE));
	return true;
}

//在曲线上等分距离
bool CurveMeasurePoint1(ads_name ent, double dStep, AcGePoint2dArray &Nodes, AcGeDoubleArray &AngArr,double dStart)
{
	AcDbCurve *pCurve;
	AcDbObjectId objId;
	double dParam, dLength;
	AcGePoint3d pt3d;

	acdbGetObjectId(objId, ent);
	if (acdbOpenObject(pCurve, objId, AcDb::kForRead) != Acad::eOk) return false;
	pCurve->getEndParam(dParam);
	pCurve->getDistAtParam(dParam, dLength);//获取曲线全长

	pCurve->getStartPoint(pt3d);
	GetPointAtCurve(pCurve, pt3d, -0.01, Nodes, AngArr);
	//如果差值大于5，先补一小段
	if (dStart>5)
	{
		pCurve->getPointAtDist(dStart, pt3d);
		GetPointAtCurve(pCurve, pt3d, dStart, Nodes, AngArr);
	}	
	for (double dDist = dStep + dStart; dDist < dLength; dDist = dDist + dStep)
	{
		pCurve->getPointAtDist(dDist, pt3d);
		GetPointAtCurve(pCurve, pt3d, dDist, Nodes, AngArr);
		if (dLength-dDist<dStep+5)
			break;
	}
	pCurve->getEndPoint(pt3d);

	GetPointAtCurve(pCurve, pt3d, -1.0, Nodes, AngArr);

	pCurve->close();
	return true;
}

void CLichengFromZdline::GetInfo(AcDbObjectId &idLine)
{
	AcGePoint2dArray pts,Nodes;
	ads_point pt1, pt2, pt3, pt4;
	AcDbObjectIdArray ids;
	TCHAR szText[255] = { 0 }, szCompare[255] = { 0 };
	ReadXdata(aname(idLine), _T("道路中心线名称"), STRING_TYPE, szText);
	m_strDLMC.Format(szText);
	ads_name sTemp;
	acdbGetAdsName(sTemp, idLine);
	GetPlList(sTemp, pts);
	
	pt1[X] = pts[0][X] - 2.5; pt1[Y] = pts[0][Y] + 2.5; Nodes.append(apoint(pt1));
	pt2[X] = pts[0][X] - 2.5; pt2[Y] = pts[0][Y] - 2.5; Nodes.append(apoint(pt2));
	pt3[X] = pts[0][X] + 2.5; pt3[Y] = pts[0][Y] - 2.5; Nodes.append(apoint(pt3));
	pt4[X] = pts[0][X] + 2.5; pt4[Y] = pts[0][Y] + 2.5; Nodes.append(apoint(pt4));
	ssFromNodes(ids, Nodes, 2);
	if (m_nSCorSJ == 1) _tcscpy(szCompare,_T("992001"));
	else _tcscpy(szCompare, _T("992000"));

	m_dQSLC = 0;//未绘制纵断面，初始化为0
	for (int j = 0; j < ids.length(); j++)
	{
		TCHAR szSTBM[50] = { 0 };
		AcDbObjectId id = ids[j];
		GetStdm(aname(id), szSTBM);
		CStringArray strs1;
		if (_tcscmp(szSTBM, szCompare)) continue;
		ReadXdata(aname(id), _T("LCZNOTE"), STRING_TYPE, szText);
		CString strText(szText);
		if (!strText.IsEmpty())
		{
			AnalyzeData(strs1, strText, _T("+"));
			m_strLCZMC = strs1.GetAt(0);
			m_dQSLC = _wtof(strs1[1]);
		}
	}
}

bool CLichengFromZdline::hdmcreate(AcDbObjectId &idLine, double &dStart)
{
	ads_name ent;
	ads_point pt,pt1,pt2;
	double dStep,dLength,dLeft,dRight;
	int i,nAddNodes = 2;
	AcGePoint2dArray plNodes;
	TCHAR szStdm[50] = { 0 };
	if ( false == SelectMainLine(ent) ) return false;

	acdbGetObjectId(idLine, ent);
	ReadXdata(aname(idLine), _T("SOUTH"), STRING_TYPE, szStdm);
	m_nSCorSJ = 0;
	if (!_tcscmp(szStdm, _T("180304"))) m_nSCorSJ = 1;//判断是实测还是设计道路
	GetInfo(idLine);

	int nPolyType = GetPolyType(ent);
	int nColor = getcolor(ent);
	CAcModuleResourceOverride myResources;
	CCreateHdmDlg dlg;
	dlg.m_nPolyType = nPolyType;
	dlg.m_start = m_dQSLC;
	if ( dlg.DoModal() != IDOK ) return false;
	nAddNodes = dlg.m_nMiddle;
	dStep = dlg.m_dStep;
	dLeft = dlg.m_dLeft;
	dRight = dlg.m_dRight;
	dStart = dlg.m_start;

	double dTemp = 0.0, dD = 0.0;
	int nTemp=0;
	while (dTemp < dStart)
	{
		++nTemp;
		dTemp = dStep*nTemp;
	}
	dD = dTemp - dStart;

	GetPlList(ent,plNodes);
	GetEntLength(ent,dLength);

	if ( false == GetEntGroup(ent,m_groupId) ) {
		if ( false == CreateGroup(ent,m_groupId) ) return false;
	}
	if ( 1 == nAddNodes || 3 == nAddNodes) 
	{
		AcDbCurve *pCurve;
		AcDbObjectId objId;
		acdbGetObjectId(objId,ent);
		if ( acdbOpenObject(pCurve,objId,AcDb::kForRead) != Acad::eOk ) return false;
		for (i=0; i<plNodes.length(); i++) 
		{
			AcGePoint3d pt3d,pt3dE,ps1,ps2;
			double dis1,dAng;
			
			pt[X]=plNodes.at(i)[X]; pt[Y]=plNodes.at(i)[Y]; pt[Z]=0.0;
			pt3d.x=pt[X]; pt3d.y=pt[Y]; pt3d.z=0.0; 
			pCurve->getClosestPointTo(pt3d,ps1);
			pCurve->getDistAtPoint(ps1,dis1);
			if ( i > 0 ) {
				pCurve->getPointAtDist(dis1-0.001,pt3d);
				pt1[X]=pt3d[X]; pt1[Y]=pt3d[Y]; pt1[Z]=0.0;
			}
			if ( i < plNodes.length()-1 ) {
				pCurve->getPointAtDist(dis1+0.001,pt3d);
				pt2[X]=pt3d[X]; pt2[Y]=pt3d[Y]; pt2[Z]=0.0;
			}
			
			if ( 0 == i ) 
				dAng = ads_angle(pt,pt2);
			else if ( plNodes.length()-1 == i ) 
				dAng = ads_angle(pt1,pt);
			else 
				dAng = ads_angle(pt1,pt);
			AddHdmLine(pt, dAng, dLeft, dRight, nColor);
			
			if ( plNodes.length()-1 == i ) break;
		}
		pCurve->close();
	}

	if (2 == nAddNodes || 3 == nAddNodes)
	{
		AcGePoint2dArray Nodes;
		AcGeDoubleArray AngArr;
		if ( false == CurveMeasurePoint1(ent,dStep,Nodes,AngArr,dD) ) return false;

		for (i=0; i<Nodes.length(); i++) 
		{
			AcGePoint2d pt2d = Nodes.at(i);
			pt[X]=pt2d[X]; pt[Y]=pt2d[Y]; pt[Z]=0.0;
			if ( 3 == nAddNodes ) {
				bool bNear=false;
				for (int j=0; j<plNodes.length(); j++) {
					if ( distOF2d(pt2d,plNodes.at(j)) <= 1.0 ) {
						bNear = true;
						break;
					}
				}
				if ( true == bNear ) continue;
			}
			AddHdmLine(pt, AngArr.at(i), dLeft, dRight, nColor);
		}
	}
	return true;
}

void CLichengFromZdline::hdmadd()
{
	ads_name ent;
	AcGePoint3d pt3d,ptTemp;
	double dLength,dLeft,dRight,dAngle;
	int sel;
	AcDbObjectId objId;

	if ( false == SelectMainLine(ent) ) return;
	ads_initget(6,NULL);
	if ( RTNORM != ads_getreal(_T("\n输入横断面左边长度:(米) "),&dLeft) ) return;
	ads_initget(6,NULL);
	if ( RTNORM != ads_getreal(_T("\n输入横断面右边长度:(米) "),&dRight) ) return;
	GetEntLength(ent,dLength);
	int val = ads_getint(_T("\n选择获取中桩位置方式: (1)鼠标定点 (2)输入里程 <1>"),&sel);
	if ( RTCAN == val ) return;
	else if ( RTNORM != val ) sel = 1;
	if ( false == GetEntGroup(ent,m_groupId) ) {
		if ( false == CreateGroup(ent,m_groupId) ) return;
	}

	acdbGetObjectId(objId,ent);
	AcDbCurve *pCurve;
	if ( acdbOpenObject(pCurve,objId,AcDb::kForRead) != Acad::eOk ) return;
	while (1) 
	{
		if ( 2 == sel ) {
			double dLicheng;
			if ( RTNORM != ads_getreal(_T("\n输入加桩点里程: "),&dLicheng) ) break;
			if ( dLicheng >= dLength ) {
				ads_alert(_T("里程值超过复合线长度")); continue;
			}
			pCurve->getPointAtDist(dLicheng,pt3d);
		}
		else {
			if ( RTNORM != ads_getpoint(NULL,_T("\n指定加桩位置:"),asDblArray(ptTemp)) ) break;
			if ( Acad::eOk != pCurve->getClosestPointTo(ptTemp,pt3d) ) {
				ads_alert(_T("无法找到纵断面线上对应点")); continue;
			}
		}
		AcGeVector3d vector;
		pCurve->getFirstDeriv(pt3d,vector);

		ptTemp = pt3d + vector;
		dAngle = ads_angle(asDblArray(pt3d),asDblArray(ptTemp)) - PI/2;
		AddHdmLine2(asDblArray(pt3d),dAngle,dLeft,dRight);
	}
	pCurve->close();
}

void CLichengFromZdline::changelength()
{
	ads_name ssName,ent,entMain;
	long l,Length=0;
	double dLeft,dRight,dLength,dDist;
	TCHAR szCode[50];
	AcDbObjectId objId;
	AcDbEntity *pEnt;
	AcGePoint3d pnt(0,0,0); AcGeVector3d normal(0,0,1); AcGePlane projPlane(pnt,normal);

	if ( false == SelectMainLine(entMain) ) return;
	if ( false == GetEntGroup(entMain,m_groupId) ) {
		ads_alert(_T("该线上没有横断面线")); return;
	}

	ads_printf(_T("\n选择横断面线:"));
	int oldGroup = GroupReset();
	int val = ads_ssget(NULL,NULL,NULL,NULL,ssName);
	GroupBack(oldGroup);
	if ( RTNORM != val ) return;
	ads_initget(6,NULL);
	if ( RTNORM != ads_getreal(_T("\n输入横断面左边长度:(米) "),&dLeft) ) {
		ads_ssfree(ssName); return;
	}
	ads_initget(6,NULL);
	if ( RTNORM != ads_getreal(_T("\n输入横断面右边长度:(米) "),&dRight) ) {
		ads_ssfree(ssName); return;
	}

	acdbGetObjectId(objId,entMain);
	if ( Acad::eOk != acdbOpenAcDbEntity(pEnt,objId,AcDb::kForRead) ) return;
	ads_sslength(ssName,&Length);
	TCHAR szCompare[255] = { 0 };
	if (m_nSCorSJ == 1) _tcscpy(szCompare, _T("992001"));
	else _tcscpy(szCompare, _T("992000"));
	for (l=0; l<Length; l++)
	{
		ads_ssname(ssName,l,ent);
		GetStdm(ent,szCode);
		if (_tcscmp(szCode, szCompare)) continue;
		acdbGetObjectId(objId,ent);
		GetEntLength(ent,dLength);
		AcDbLine *pLine;
		AcGePoint3d pt3d,ptL,ptR;
		if ( Acad::eOk != acdbOpenObject(pLine, objId, AcDb::kForWrite) ) continue;
		ptL=pLine->startPoint(); ptL.z=0.0; pLine->setStartPoint(ptL);
		ptR=pLine->endPoint(); ptR.z=0.0; pLine->setEndPoint(ptR);
		double dAngle = ads_angle(asDblArray(ptL),asDblArray(ptR));

		//判断是否在组内
		AcDbGroup *pGroup;
		if ( Acad::eOk != acdbOpenObject(pGroup, m_groupId, AcDb::kForRead) ) {
			pLine->close(); continue;
		}
		bool bHave = pGroup->has(pLine);
		pGroup->close();
		if ( false == bHave ) {
			pLine->close(); continue;
		}

		AcGePoint3dArray tempPts;
		pLine->intersectWith(pEnt,AcDb::kOnBothOperands,projPlane,tempPts);
		if ( tempPts.length() != 1 ) {
			pLine->close(); continue;
		}
		pLine->getClosestPointTo(tempPts.at(0),pt3d);
		pt3d[Z] = 0.0;
		
		pLine->getDistAtPoint(pt3d,dDist);
		ads_polar(asDblArray(pt3d),dAngle+PI,dLeft,asDblArray(ptL));
		pLine->setStartPoint(ptL);
		ads_polar(asDblArray(pt3d),dAngle,dRight,asDblArray(ptR));
		pLine->setEndPoint(ptR);
		pLine->close();
	}
	pEnt->close();
	ads_ssfree(ssName);
}

void CLichengFromZdline::hdmtrim()
{
	ads_name entMain,entTrim,ent;
	ads_point pt;
	TCHAR szCode[50];
	AcDbGroup *pGroup;

	if ( false == SelectMainLine(entMain) ) return;
	if ( false == GetEntGroup(entMain,m_groupId) ) {
		ads_alert(_T("该线上没有横断面线")); return;
	}
	if ( ads_entsel(_T("\n选择切割边界线"),entTrim,pt) != RTNORM ) return;
	if ( false == IsCurve(entTrim) ) {
		ads_alert(_T("请选择线状实体")); return;
	}

	if ( Acad::eOk != acdbOpenObject(pGroup, m_groupId, AcDb::kForRead) ) return;
	AcDbGroupIterator *pIter = pGroup->newIterator();
	TCHAR szCompare[255] = { 0 };
	if (m_nSCorSJ == 1) _tcscpy(szCompare, _T("992001"));
	else _tcscpy(szCompare, _T("992000"));
	for (; !pIter->done(); pIter->next()) 
	{
		AcDbObjectId objId = pIter->objectId();
		acdbGetAdsName(ent,objId); 
		GetStdm(ent,szCode);
		if ( _tcscmp(szCode,szCompare) ) continue;
		TrimOneHdm(entMain,entTrim,objId);
	}
	delete pIter; 
	pGroup->close();
}

void CLichengFromZdline::TrimOneHdm(ads_name entMain,ads_name entTrim,AcDbObjectId objId)
{
	AcDbLine *pLine;
	AcDbEntity *pEntTrim,*pEntMain;
	AcDbObjectId entId;
	AcGePoint3d ptMid,pt,pt1,pt2;
	AcGePoint3dArray tempPts,PointArr;
	double dMid,dDist,dDist1,dDist2;
	AcGePoint3d pnt(0,0,0); AcGeVector3d normal(0,0,1); AcGePlane projPlane(pnt,normal);

	if ( Acad::eOk != acdbOpenObject(pLine, objId, AcDb::kForWrite) ) return;
	acdbGetObjectId(entId,entMain);
	if ( Acad::eOk != acdbOpenAcDbEntity(pEntMain,entId,AcDb::kForRead) ) {
		pLine->close(); return;
	}
	pLine->intersectWith(pEntMain,AcDb::kOnBothOperands,projPlane,tempPts);
	pEntMain->close();
	if ( tempPts.length() != 1 ) {
		pLine->close(); return;
	}
	pLine->getClosestPointTo(tempPts.at(0),ptMid);
	pLine->getDistAtPoint(ptMid,dMid);

	acdbGetObjectId(entId,entTrim);
	if ( Acad::eOk != acdbOpenAcDbEntity(pEntTrim,entId,AcDb::kForRead) ) {
		pLine->close(); return;
	}
	pLine->intersectWith(pEntTrim,AcDb::kOnBothOperands,projPlane,PointArr);
	pEntTrim->close();
	int nLen = PointArr.length();
	if ( nLen > 2 ) {//针对多出无用交点的情况
		AcGePoint3dArray InterArr;
		RequeueInterPoints(pLine,PointArr,InterArr);
		if ( 1 == InterArr.length() || 2 == InterArr.length() ) {
			nLen = InterArr.length();
			if ( PointArr.length() > 0 ) PointArr.removeFirst();
			for (int i=0; i<nLen; i++) PointArr.append(InterArr[i]);
		}
	}

	if ( 1 == nLen ) {
		pLine->getClosestPointTo(PointArr.at(0),pt);
		pLine->getDistAtPoint(pt,dDist);
		pt[Z] = 0.0;
		if ( dDist < dMid ) pLine->setStartPoint(pt);
		else if ( dDist > dMid ) pLine->setEndPoint(pt);
	}
	else if ( 2 == nLen ) {
		pLine->getClosestPointTo(PointArr.at(0),pt1);
		pLine->getClosestPointTo(PointArr.at(1),pt2);
		pLine->getDistAtPoint(pt1,dDist1);
		pLine->getDistAtPoint(pt2,dDist2);
		pt1[Z] = 0.0; pt2[Z] = 0.0;
		if ( dDist1 > dDist2 ) {
			dDist=dDist1; dDist1=dDist2; dDist2=dDist;
			pt=pt1; pt1=pt2; pt2=pt;
		}
		if ( dMid>=dDist1 && dMid<=dDist2 ) {
			pLine->setStartPoint(pt1);
			pLine->setEndPoint(pt2);
		}
		else if ( dMid < dDist1 ) 
			pLine->setEndPoint(pt1);
		else if ( dMid > dDist2 ) 
			pLine->setStartPoint(pt2);
	}
	pLine->close();
}

//void CLichengFromZdline::hdmdesign()
//{
//	ads_name ent,entBound;
//	ads_point pt,ptP,ptScreen,ptMid,ptOrigin;
//	AcGePoint3d pt1,pt2,pt3d;
//	struct resbuf result,*rc;
//	double dHeight,dScale,dDesign1,dDesign2,dDist1,dDist2;
//	TCHAR szCode[50];
//	AcDbEntity *pEnt;
//	AcDbObjectId objId;
//	AcGePoint3d pnt(0,0,0); AcGeVector3d normal(0,0,1); AcGePlane projPlane(pnt,normal);
//
//	if ( ads_entsel(_T("\n选择切割边界线"),entBound,pt) != RTNORM ) return;
//	if ( false == IsCurve(entBound) ) {
//		ads_alert(_T("请选择线状实体")); return;
//	}
//
//	HWND hWndACAD = adsw_acadDocWnd();
//	CWnd *pWnd = CWnd::FromHandle(hWndACAD);
//	CRect rectC,rectS;
//	pWnd->GetWindowRect(rectC); 
//	rectS = rectC;
//	pWnd->ScreenToClient(rectC);
//
//	acdbGetObjectId(objId,entBound);
//	if ( Acad::eOk != acdbOpenAcDbEntity(pEnt,objId,AcDb::kForRead) ) return;
//	int oldGroup = GroupReset();
//	while (1)
//	{
//		int val = ads_entsel(_T("\n选择横断面线"),ent,pt);
//		if ( RTNORM != val ) break;
//		GetStdm(ent,szCode);
//		if ( _tcscmp(szCode,_T("992000")) ) {
//			ads_alert(_T("没有选中横断面线")); continue;
//		}
//
//		AcDbLine *pLine;
//		acdbGetObjectId(objId,ent);
//		if ( Acad::eOk != acdbOpenObject(pLine, objId, AcDb::kForRead) ) continue;
//		AcGePoint3dArray tempPts;
//		pLine->intersectWith(pEnt,AcDb::kOnBothOperands,projPlane,tempPts);
//		if ( 2 != tempPts.length() ) {
//			pLine->close(); ads_alert(_T("与边界没有两个交点")); continue;
//		}
//
//		pLine->getClosestPointTo(tempPts.at(0),pt1);
//		pLine->getDistAtPoint(pt1,dDist1);
//		pLine->getClosestPointTo(tempPts.at(1),pt2);
//		pLine->getDistAtPoint(pt2,dDist2);
//		pLine->close();
//		if ( dDist1 == dDist2 ) continue;
//		if ( dDist1 > dDist2 ) {
//			double dTemp = dDist1; dDist1 = dDist2; dDist2 = dTemp;
//			pt3d = pt1; pt1 = pt2; pt2 = pt3d;
//		}
//
//		//确保横断面线在显示区内
//		AcGePoint2d pt2d;
//		AcGePoint2dArray pt2dArr;
//		pt2d.x=pt1.x; pt2d.y=pt1.y; pt2dArr.append(pt2d);
//		pt2d.x=pt2.x; pt2d.y=pt2.y; pt2dArr.append(pt2d);
//		FullCurView(pt2dArr,10.0);
//		ads_getvar(_T("SCREENSIZE"),&result); ads_point_set(result.resval.rpoint,ptScreen);
//  		ads_getvar(_T("VIEWCTR"),&result); ads_point_set(result.resval.rpoint,ptMid);
//		ads_getvar(_T("VIEWSIZE"),&result); dHeight = result.resval.rreal;
//		dScale = ptScreen[Y]/dHeight;
//		ptOrigin[X] = ptMid[X]-ptScreen[X]*dHeight/(ptScreen[Y]*2); 
//		ptOrigin[Y] = ptMid[Y]+dHeight/2;
//
//		int i;
//		for (i=0; i<2; i++) {
//			CAcModuleResourceOverride myResources;
//			CHeightDlg dlg;
//			if ( 0 == i ) ads_point_set(asDblArray(pt1),ptP);
//			else ads_point_set(asDblArray(pt2),ptP);
//			pt[X] = (ptP[X]-ptOrigin[X])*dScale+rectS.left-rectC.left; 
//			pt[Y] = (ptOrigin[Y]-ptP[Y])*dScale+rectS.top-rectC.top;
//			dlg.m_left = (int)pt[X]; dlg.m_top=(int)pt[Y];
//			if ( dlg.DoModal() != IDOK ) break;
//			if ( 0 == i ) dDesign1 = dlg.m_height;
//			else dDesign2 = dlg.m_height;
//		}
//		if ( 2 == i) {
//			rc = ads_buildlist(-3,1001,_T("south"),1000,_T("992000"),1040,dDist1,1040,dDesign1,1040,dDist2,1040,dDesign2,0);
//			addappRC(ent,rc);
//			ads_relrb(rc);
//		}
//	}
//	pEnt->close();
//	GroupBack(oldGroup);
//}

//防止求得的交点大于一个的时候，取最靠近横断面中间的那个
bool CLichengFromZdline::GetPointNearMiddle(AcGePoint3dArray &tempPts,AcGePoint3d pt1,AcGePoint3d pt2,AcGePoint3d &ptInt)
{
	AcGePoint3d ptMid;
	double dMinDist=0.0;
	ptMid.x = (pt1.x+pt2.x)/2; ptMid.y = (pt1.y+pt2.y)/2; ptMid.z = 0.0;
	if ( tempPts.length() < 1 ) return false;
	for (int k=0; k<tempPts.length(); k++)
	{
		double dTemp = distOF3d(ptMid,tempPts[k]);
		if ( 0 == k || dTemp < dMinDist ) 
		{
			ptInt = tempPts[k];
			dMinDist = dTemp;
		}
	}
	return true;
}

void CLichengFromZdline::StandLineDmx(ads_name entMain,AcDbObjectIdArray &DmxArr,AcGeDoubleArray &LichengArr,AcGePoint3dArray &InterPtArr,AcGeDoubleArray &AngleArr)
{
	AcDbGroup *pGroup;
	ads_name ent;
	AcDbObjectId objId;
	AcDbCurve *pCurve;
	AcGePoint3d pt3d,pt1,pt2;
	double dDist,dAngle;
	TCHAR szCode[50];
	AcGePoint3d pnt(0,0,0); AcGeVector3d normal(0,0,1); AcGePlane projPlane(pnt,normal);

	acdbGetObjectId(objId,entMain);
	if ( Acad::eOk != acdbOpenObject(pCurve, objId, AcDb::kForRead) ) return;
	if ( Acad::eOk != acdbOpenObject(pGroup, m_groupId, AcDb::kForRead) ) {
		pCurve->close(); return;
	}
	AcDbGroupIterator *pIter = pGroup->newIterator();
	TCHAR szCompare[255] = { 0 };
	if (m_nSCorSJ == 1) _tcscpy(szCompare, _T("992001"));
	else _tcscpy(szCompare, _T("992000"));
	for (; !pIter->done(); pIter->next()) 
	{
		objId = pIter->objectId();
		acdbGetAdsName(ent,objId); 
		GetStdm(ent,szCode);
		if (_tcscmp(szCode, szCompare)) continue;
		AcDbLine *pLine;
		if ( Acad::eOk != acdbOpenObject(pLine, objId, AcDb::kForRead) ) continue;
		AcGePoint3dArray tempPts;
		pCurve->intersectWith(pLine,AcDb::kOnBothOperands,projPlane,tempPts);
		pt1 = pLine->startPoint();
		pt2 = pLine->endPoint();
		pLine->close();

		//求离中心点最近的交点
		AcGePoint3d ptInt;
		if ( false == GetPointNearMiddle(tempPts,pt1,pt2,ptInt) ) continue;

		//将交点信息写入阵列
		pCurve->getClosestPointTo(ptInt,pt3d);
		pCurve->getDistAtPoint(pt3d,dDist);
		int i;
		for (i=0; i<LichengArr.length(); i++) {
			if ( dDist < LichengArr.at(i) ) break;
		}
		dAngle = ads_angle(asDblArray(pt1),asDblArray(pt2));
		AngleArr.insertAt(i,dAngle);
		LichengArr.insertAt(i,dDist);
		DmxArr.insertAt(i,objId);
		InterPtArr.insertAt(i,pt3d);
	}
	delete pIter; 
	pGroup->close();
	pCurve->close();
}

void CLichengFromZdline::GetHeadTailPoint(ads_name ent,ads_name entMain,AcGePoint2dArray &jiePtArr,AcGeDoubleArray &jieDistArr,ads_point ptt1,ads_point pttM,ads_point ptt2)
{
	AcDbLine *pLine;
	AcDbEntity *pEnt;
	AcDbObjectId objId;
	AcGePoint2d pt2d;
	AcGePoint3d ptM,ptL,ptR,pt3d;
	double dLength,dDist;
	AcGePoint3d pnt(0,0,0); AcGeVector3d normal(0,0,1); AcGePlane projPlane(pnt,normal);

	acdbGetObjectId(objId,ent);
	if ( Acad::eOk != acdbOpenObject(pLine, objId, AcDb::kForRead) ) return;
	ptL = pLine->startPoint();
	ptR = pLine->endPoint();
	dLength = distOF3d(ptL,ptR);

	acdbGetObjectId(objId,entMain);
	if ( Acad::eOk != acdbOpenAcDbEntity(pEnt,objId,AcDb::kForRead) ) {
		pLine->close(); return;
	}

	AcGePoint3dArray tempPts;
	pLine->intersectWith(pEnt,AcDb::kOnBothOperands,projPlane,tempPts);
	pEnt->close();
	
	//求离中心点最近的交点
	AcGePoint3d ptInt;
	if ( false == GetPointNearMiddle(tempPts,ptL,ptR,ptInt) ) {
		pLine->close(); return;
	}

	pLine->getClosestPointTo(ptInt,ptM);
	pLine->getDistAtPoint(ptM,dDist);
	pLine->close();

	pt2d.x=ptL.x; pt2d.y=ptL.y; jiePtArr.append(pt2d); jieDistArr.append(0.0);
	pt2d.x=ptM.x; pt2d.y=ptM.y; jiePtArr.append(pt2d); jieDistArr.append(dDist);
	pt2d.x=ptR.x; pt2d.y=ptR.y; jiePtArr.append(pt2d); jieDistArr.append(dLength);
	ptt1[X]=ptL.x; ptt1[Y]=ptL.y; ptt1[Z]=0.0;
	ptt2[X]=ptR.x; ptt2[Y]=ptR.y; ptt2[Z]=0.0;
	pttM[X]=ptM.x; pttM[Y]=ptM.y; pttM[Z]=0.0;
}

void CLichengFromZdline::InsertPointAtDmx(ads_name ent,ads_name entMain,double dStep,AcGePoint3dArray &Nodes,AcGeDoubleArray &DistArr)
{
	AcDbLine *pLine;
	AcDbEntity *pEnt;
	AcDbObjectId objId;
	AcGePoint3d ptM,ptL,ptR,pt3d;
	double dLength,dDist;
	AcGePoint3d pnt(0,0,0); AcGeVector3d normal(0,0,1); AcGePlane projPlane(pnt,normal);

	GetEntLength(ent,dLength);
	acdbGetObjectId(objId,ent);
	if ( Acad::eOk != acdbOpenObject(pLine, objId, AcDb::kForRead) ) return;
	ptL = pLine->startPoint();
	ptR = pLine->endPoint();

	acdbGetObjectId(objId,entMain);
	if ( Acad::eOk != acdbOpenAcDbEntity(pEnt,objId,AcDb::kForRead) ) {
		pLine->close(); return;
	}

	AcGePoint3dArray tempPts;
	pLine->intersectWith(pEnt,AcDb::kOnBothOperands,projPlane,tempPts);
	pEnt->close();
	
	//求离中心点最近的交点
	AcGePoint3d ptInt;
	if ( false == GetPointNearMiddle(tempPts,ptL,ptR,ptInt) ) {
		pLine->close(); return;
	}

	pLine->getClosestPointTo(ptInt,ptM);
	pLine->getDistAtPoint(ptM,dDist);
	//左边线
	double dis;
	for (dis=dDist-dStep; dis>0.001; dis=dis-dStep) {
		pLine->getPointAtDist(dis,pt3d);
		Nodes.insertAt(0,pt3d);
		DistArr.insertAt(0,dis-dDist);
	}
	//左点
	Nodes.insertAt(0,ptL);
	DistArr.insertAt(0,-1.0*dDist);
	//中点
	Nodes.append(ptM);
	DistArr.append(0.0);
	//右边线
	for (dis=dDist+dStep; dis<dLength-0.001; dis=dis+dStep) {
		pLine->getPointAtDist(dis,pt3d);
		Nodes.append(pt3d);
		DistArr.append(dis-dDist);
	}
	//右点
	Nodes.append(ptR);
	DistArr.append(dLength-dDist);

	pLine->close();
}

//获取横断面线设计高
bool CLichengFromZdline::GetDesignHeight(ads_name ent,double &dDist1,double &dHeight1,double &dDist2,double &dHeight2)
{
	struct resbuf *rc,*rb;
	rc = xdataget(ent);
	if ( NULL == rc ) return false;
	rb = rc;
	int num=0;
	while (rb) {
		if ( 1040 == rb->restype ) {
			num++;
			if ( 1 == num ) dDist1 = rb->resval.rreal;
			else if ( 2 == num ) dHeight1 = rb->resval.rreal;
			else if ( 3 == num ) dDist2 = rb->resval.rreal;
			else if ( 4 == num ) dHeight2 = rb->resval.rreal;
		}
		rb = rb->rbnext;
	}
	ads_relrb(rc);
	if ( num >= 4 ) return true;
	else return false;
}

void CLichengFromZdline::writeFile(AcDbObjectId &idLine, double &dStart)
{
	ads_name entMain,ent;
	TCHAR datFile[MAX_PATH],hdmFile[MAX_PATH],coordFile[MAX_PATH];
	double dStep,dBegin,dHeight1,dHeight2,dDist1,dDist2;
	//AcDbObjectIdArray DmxArr;
	DmxArr.removeAll();
	AcGeDoubleArray LichengArr,HeightArr,AngleArr;
	AcGePoint3dArray InterPtArr;
	FILE *fp,*fpDat;
	int nDatNumber=0;

	//if ( false == SelectMainLine(entMain) ) return;
	acdbGetAdsName(entMain, idLine);
	if ( false == GetEntGroup(entMain,m_groupId) ) {
		ads_alert(_T("该线上没有横断面线")); return;
	}

	CAcModuleResourceOverride myResources;
	CWriteLchFileDlg dlg1;
	dBegin = dStart;
	dlg1.m_strLCZMC = m_strLCZMC;
	dlg1.m_strDLMC = m_strDLMC;
	if ( dlg1.DoModal() != IDOK ) return;
	BOOL bChazhi = dlg1.m_bChazhi;
	BOOL bJiaodian = dlg1.m_bJiaodian;
	if ( TRUE == bChazhi ) dStep = dlg1.m_dInterval;
	else dStep = 0.5; //加密插值距离
	AddXdata(aname(idLine), _T("道路中心线名称"), STRING_TYPE, dlg1.m_strDLMC);

	IDataBaseOper oper;
	CString dir = oper.readPathTable(_T("当前目录")) + _T("\\CK\\");
	dlg1.m_strLchFile = dir + _T("LCHFile.hdm");
	dlg1.m_strDatFile = dir + _T("DATFile.dat");
	_stprintf(datFile,_T("%s"),dlg1.m_strGcdFile);
	_stprintf(hdmFile,_T("%s"),dlg1.m_strLchFile);
	_stprintf(coordFile,_T("%s"),dlg1.m_strDatFile);

	if ( 1 == dlg1.m_nType ) {
		_tcscpy(datFile,_T("sjx$.dat"));
		if ( getgchfile(datFile,2) < 3 ) return;
	}
	
	CJoinTriangles sjw;
	_tcscpy(sjw.m_datFile,datFile);
	if ( 2 == dlg1.m_nType ) sjw.ReadSjwFile(datFile); //读入三角网文件
	else sjw.sanjiao(); //根据坐标文件重建三角网
	if ( sjw.m_LenSjx <= 0 ) {
		ads_alert(_T("无法读入坐标数据")); return;
	}

	//排列各断面线
	StandLineDmx(entMain,DmxArr,LichengArr,InterPtArr,AngleArr);

	fp = _tfopen(hdmFile,_T("wt"));
	if ( NULL == fp ) { 
		ads_alert(_T("无法打开文件")); return; 
	}
	fpDat = _tfopen(coordFile,_T("wt"));

	for (int i=0; i<DmxArr.length(); i++) 
	{
		AcGePoint3d pt3d;
		ads_point pt;
		acdbGetAdsName(ent,DmxArr.at(i));

		//求经过的地物交点
		AcGePoint3dArray dw_lcNodes;
		AcGeDoubleArray dw_lcDists;
		if ( TRUE == bJiaodian ) {
			ThreeD_GetEntityHeightAlongLine(ent,sjw,dw_lcNodes,dw_lcDists);
		}

		//获取首尾点及中桩点
		AcGePoint2dArray jiePtArr;
		AcGeDoubleArray jieDistArr;
		ads_point ptL,ptM,ptR;
		GetHeadTailPoint(ent,entMain,jiePtArr,jieDistArr,ptL,ptM,ptR);
		double dLength = distOFpt(ptL,ptR);
		double dLeft = distOFpt(ptL,ptM);
		double dAngle = ads_angle(ptL,ptR);

		//插入首尾点及中桩点
		int k;
		for (int z=0; z<jiePtArr.length(); z++)
		{
			AcGePoint2d pt2d = jiePtArr.at(z);
			double dDist = jieDistArr.at(z);
			bool bEqual=false;
			for (k=0; k<dw_lcDists.length(); k++) {
				if ( fabs(dDist-dw_lcDists[k]) < 0.1 ) { bEqual = true; break; }
				else if ( dDist < dw_lcDists[k] ) break;
			}
			if ( false == bEqual ) {
				pt[X] = pt2d.x; pt[Y] = pt2d.y; pt[Z] = 0.0;
				sjw.cz1(pt);
				pt3d.x=pt[X]; pt3d.y=pt[Y]; pt3d.z=pt[Z];
				dw_lcNodes.insertAt(k,pt3d);
				dw_lcDists.insertAt(k,dDist);
			}
		}

		if ( TRUE == bChazhi )
		{//内插
			k = 0;
			while ( k < dw_lcDists.length()-1 )
			{
				if ( fabs(dw_lcDists[k+1]-dw_lcDists[k]) > dStep*1.3 )
				{
					double dDist = dw_lcDists[k] + dStep;
					ads_polar(ptL,dAngle,dDist,pt);
					sjw.cz1(pt);
					pt3d.x = pt[X]; pt3d.y = pt[Y]; pt3d.z = pt[Z];
					dw_lcNodes.insertAt(k+1,pt3d);
					dw_lcDists.insertAt(k+1,dDist);
				}
				k++;
			}
		}
		else
		{//取极值
			k = 0;
			while ( k < dw_lcDists.length()-1 )
			{
				if ( fabs(dw_lcDists[k+1]-dw_lcDists[k]) > dStep*9 )
				{
					AcGePoint3dArray Nodes;
					AcGeDoubleArray distArr;
					double dDist;
					for (dDist=dw_lcDists[k]+dStep; dDist<dw_lcDists[k+1]; dDist=dDist+dStep)
					{
						ads_polar(ptL,dAngle,dDist,pt);
						sjw.cz1(pt);
						pt3d.x = pt[X]; pt3d.y = pt[Y]; pt3d.z = pt[Z]; Nodes.append(pt3d);
						distArr.append(dDist);
					}
					for (int v=4; v<=Nodes.length()-5; v++)
					{
						pt3d = Nodes.at(v);
						dDist = distArr.at(v);
						double dS = 0.0001;
						if ( Nodes[v-4].z<pt3d.z-dS && Nodes[v-3].z<pt3d.z-dS && Nodes[v-2].z<=pt3d.z && Nodes[v-1].z<pt3d.z 
							&& Nodes[v+4].z<pt3d.z-dS && Nodes[v+3].z<pt3d.z-dS && Nodes[v+2].z<=pt3d.z && Nodes[v+1].z<pt3d.z )
						{
							dw_lcNodes.insertAt(k+1,pt3d);
							dw_lcDists.insertAt(k+1,dDist);
							break;
						}
						if ( Nodes[v-4].z>pt3d.z+dS && Nodes[v-3].z>pt3d.z+dS && Nodes[v-2].z>=pt3d.z && Nodes[v-1].z>pt3d.z 
							&& Nodes[v+4].z>pt3d.z+dS && Nodes[v+3].z>pt3d.z+dS && Nodes[v+2].z>=pt3d.z && Nodes[v+1].z>pt3d.z )
						{
							dw_lcNodes.insertAt(k+1,pt3d);
							dw_lcDists.insertAt(k+1,dDist);
							break;
						}
					}
				}
				k++;
			}
		}

		//写入文件
		_ftprintf(fp,_T("BEGIN,%.3f:%d\n"),LichengArr.at(i)+dBegin,i+1);
		for (k=0; k<dw_lcNodes.length(); k++) 
		{
			TCHAR szStr1[50],szStr2[50];
			pt3d = dw_lcNodes.at(k);
			ads_rtos(dw_lcDists.at(k)-dLeft,2,3,szStr1); if ( !_tcscmp(szStr1,_T("-0.000")) ) _tcscpy(szStr1,_T("0.000")); //防止出现-0.000的情况
#ifdef XIANGTAN
			TCHAR *ppp;
			ppp = _tcschr(szStr1,'.');
			if ( NULL != ppp ) { ppp += 3; *ppp='\0'; } //湘潭直接截断为小数后两位
#endif
			ads_rtos(pt3d.z,2,3,szStr2); if ( !_tcscmp(szStr2,_T("-0.000")) ) _tcscpy(szStr2,_T("0.000")); //防止出现-0.000的情况
			_ftprintf(fp,_T("%s,%s\n"),szStr1,szStr2); 
			nDatNumber++;
			if ( NULL != fpDat ) {
				_ftprintf(fpDat,_T("%d,%d,%.3f,%.3f,%.3f\n"),nDatNumber,i+1,pt3d.x,pt3d.y,pt3d.z);
			}
		}

		//AcGePoint3dArray Nodes;
		//AcGeDoubleArray DistArr;
		//InsertPointAtDmx(ent,entMain,dStep,Nodes,DistArr);
		//for (int j=0; j<Nodes.length(); j++) {
		//	pt3d = Nodes.at(j);
		//	sjw.cz1(asDblArray(pt3d));
		//	_ftprintf(fp,_T("%.3f,%.3f\n"),DistArr.at(j),pt3d[Z]);
		//	nDatNumber++;
		//	if ( NULL != fpDat ) {
		//		_ftprintf(fpDat,_T("%d,%d,%.3f,%.3f,%.3f\n"),nDatNumber,i+1,pt3d.x,pt3d.y,pt3d.z);
		//	}
		//}
		
		
		//设计线
		if ( true == GetDesignHeight(ent,dDist1,dHeight1,dDist2,dHeight2) ) {
			_ftprintf(fp,_T("NEXT\n"));
			_ftprintf(fp,_T("%.3f,%.3f\n"),dDist1-dLeft,dHeight1);
			_ftprintf(fp,_T("%.3f,%.3f\n"),dDist2-dLeft,dHeight2);
		}
		
		//求中点高程,2005.04.23
		pt3d = InterPtArr.at(i);
		sjw.cz1(asDblArray(pt3d));
		InterPtArr.setAt(i,pt3d);
	}
	fclose(fp);
	if ( NULL != fpDat ) fclose(fpDat);
	WriteMiddlePointNumber(InterPtArr, dBegin, LichengArr, AngleArr, dlg1.m_strLCZMC, idLine);
	//WriteHeighttoDMX(idLine, InterPtArr);//将纵断面信息写入断面线扩展属性
	if ( TRUE == dlg1.m_bExcel )
	{//输出EXCEL表
		VerFace excel;
		excel.xlsOut(hdmFile,_T("横断面成果表"));
	}
}

void CLichengFromZdline::WriteHeighttoDMX(AcDbObjectId &idLine,AcGePoint3dArray InterPtArr)
{
	ads_point pt1, pt2, pt3, pt4;
	for (int i = 0; i < InterPtArr.length(); ++i)
	{
		AcDbObjectIdArray ids;
		AcGePoint2dArray Nodes;
		TCHAR szText[255] = { 0 }, szSHeight[50] = { 0 }, szEHeight[50] = { 0 };
		AcDbObjectId idText, idDMX;
		ReadXdata(aname(idLine), _T("道路中心线起点"), STRING_TYPE, szSHeight);
		ReadXdata(aname(idLine), _T("道路中心线终点"), STRING_TYPE, szEHeight);
		pt1[X] = InterPtArr[i][X] - 2.5; pt1[Y] = InterPtArr[i][Y] + 2.5; Nodes.append(apoint(pt1));
		pt2[X] = InterPtArr[i][X] - 2.5; pt2[Y] = InterPtArr[i][Y] - 2.5; Nodes.append(apoint(pt2));
		pt3[X] = InterPtArr[i][X] + 2.5; pt3[Y] = InterPtArr[i][Y] - 2.5; Nodes.append(apoint(pt3));
		pt4[X] = InterPtArr[i][X] + 2.5; pt4[Y] = InterPtArr[i][Y] + 2.5; Nodes.append(apoint(pt4));
		ssFromNodes(ids, Nodes, 2);
		TCHAR szCompare[255] = { 0 };
		if (m_nSCorSJ == 1) _tcscpy(szCompare, _T("992001"));
		else _tcscpy(szCompare, _T("992000"));

		for (int j = 0; j < ids.length(); j++)
		{
			TCHAR szSTBM[50] = { 0 };
			AcDbObjectId id = ids[j];
			GetStdm(aname(id), szSTBM);
			CStringArray strs1;
			if (!_tcscmp(szSTBM, _T("992100"))) 
			{
				GetEntText(aname(id), szText);
			}
			if (_tcscmp(szSTBM, szCompare)) continue;
			CString strHeight, strDist(szText);
			if (strDist.IsEmpty())
			{
				acutPrintf(_T("\n未找到里程桩注记！"));
				return;
			}
			if (i==0)//起始纵断面的高程来自道路中线用户输入值;
			{
				CString strSHeight(szSHeight);
				if (strSHeight.IsEmpty()) 
					acutPrintf(_T("\n该道路中心线起点的高程为空"));
				else
				{
					AnalyzeData(strs1, strSHeight, _T(","));
					AddXdata(aname(id), _T("LCZGC"), DOUBLE_TYPE, strs1.GetAt(2));
				}	
			}
			else if (i == InterPtArr.length() - 1)//终点纵断面的高程来自道路中线用户输入值
			{
				CString strEHeight(szEHeight);
				if (strEHeight.IsEmpty())
					acutPrintf(_T("\n该道路中心线终点的高程为空"));
				else
				{
					AnalyzeData(strs1, strEHeight, _T(","));
					AddXdata(aname(id), _T("LCZGC"), DOUBLE_TYPE, strs1.GetAt(2));
				}
			}
			else//中间纵断面的高程来自内插计算
			{
				strHeight.Format(_T("%f"), InterPtArr[i][Z]);
				AddXdata(aname(id), _T("LCZGC"), DOUBLE_TYPE, strHeight);
			}
			CStringArray strs;
			AnalyzeData(strs, strDist, _T("+"));
			AddXdata(aname(id), _T("LCZDIST"), DOUBLE_TYPE, strs.GetAt(1));
			AddXdata(aname(id), _T("LCZNOTE"), STRING_TYPE, szText);
			break;
		}
	}
}

void CLichengFromZdline::WriteMiddlePointNumber(AcGePoint3dArray InterPtArr, double dBegin, AcGeDoubleArray LichengArr, AcGeDoubleArray AngleArr, const CString &strLCZMC, AcDbObjectId &idLine)
{
	AcDbGroup *pGroup;
	ads_name ent;
	AcDbObjectId objId;
	ads_point pt, ptL, ptR;
	TCHAR szCode[50], szEnType[50], szStrL[50], szStrR[50], szLCZMC[50];
	double dH, dScale;
	read1(dScale); dH = dScale / 1000.0;
	if (Acad::eOk != acdbOpenObject(pGroup, m_groupId, AcDb::kForWrite)) return;

	TCHAR szCompare[255] = { 0 };
	if (m_nSCorSJ == 1) _tcscpy(szCompare, _T("992001"));
	else _tcscpy(szCompare, _T("992000"));
	AcGePoint2dArray Nodes;
	TCHAR szText[255] = { 0 }, szSHeight[50] = { 0 }, szEHeight[50] = { 0 };
	AcDbObjectId idText, idDMX;
	ReadXdata(aname(idLine), _T("道路中心线起点"), STRING_TYPE, szSHeight);
	ReadXdata(aname(idLine), _T("道路中心线终点"), STRING_TYPE, szEHeight);

	//删除已有注记
	AcDbGroupIterator *pIter = pGroup->newIterator();
	for (; !pIter->done(); pIter->next())
	{
		acdbGetAdsName(ent, pIter->objectId());
		GetStdm(ent, szCode);
		GetEntityType(ent, szEnType);
		if (!_tcscmp(szCode, _T("992100")) && !_tcsicmp(szEnType, _T("TEXT"))) ads_entdel(ent);
	}
	delete pIter;

	//if (RTCAN == ads_getstring(NULL, _T("\n请输入里程桩名称:"), szLCZMC)) return;
	_stprintf(szLCZMC, _T("%s"), strLCZMC);
	//写文字
	for (int i = 0; i<InterPtArr.length(); i++)
	{
		pt[X] = InterPtArr[i].x; pt[Y] = InterPtArr[i].y; pt[Z] = 0.0;
		double dAngle = AngleArr.at(i);
		if(dAngle > PI) dAngle -= PI;
		ads_polar(pt, dAngle + PI, 0.5 * dH, ptL);
		ads_polar(pt, dAngle, 0.5 * dH, ptR);
		bool bMid = false;
		if (i != 0 && i != InterPtArr.length() - 1)
			bMid = true;
		ConvertDoubleToKplus(LichengArr[i] + dBegin, szStrL, 2, szLCZMC, bMid);
		_stprintf(szStrR, _T("%.2f"), InterPtArr[i].z);

		DrawText(pt, szStrL, _T("BC"), _T("992100"), 2.0 * dH, dAngle);
		ads_entlast(ent);
		acdbGetObjectId(objId, ent);
		setcolor(ent, 3);
		setlayer(ent, _T("LCZH"));
		pGroup->append(objId);

		TCHAR szSTBM[50] = { 0 };
		AcDbObjectId id = DmxArr[i];;
		CStringArray strs1;

		CString strHeight, strDist(szStrL);
		if (strDist.IsEmpty())
		{
			acutPrintf(_T("\n未找到里程桩注记！"));
			return;
		}
		if (i == 0)//起始纵断面的高程来自道路中线用户输入值;
		{
			CString strSHeight(szSHeight);
			if (strSHeight.IsEmpty())
				acutPrintf(_T("\n该道路中心线起点的高程为空"));
			else
			{
				AnalyzeData(strs1, strSHeight, _T(","));
				AddXdata(aname(id), _T("LCZGC"), DOUBLE_TYPE, strs1.GetAt(2));
			}
		}
		else if (i == InterPtArr.length() - 1)//终点纵断面的高程来自道路中线用户输入值
		{
			CString strEHeight(szEHeight);
			if (strEHeight.IsEmpty())
				acutPrintf(_T("\n该道路中心线终点的高程为空"));
			else
			{
				AnalyzeData(strs1, strEHeight, _T(","));
				AddXdata(aname(id), _T("LCZGC"), DOUBLE_TYPE, strs1.GetAt(2));
			}
		}
		else//中间纵断面的高程来自内插计算
		{
			strHeight.Format(_T("%f"), InterPtArr[i][Z]);
			AddXdata(aname(id), _T("LCZGC"), DOUBLE_TYPE, strHeight);
		}
		CStringArray strs;
		AnalyzeData(strs, strDist, _T("+"));
		AddXdata(aname(id), _T("LCZDIST"), DOUBLE_TYPE, strs.GetAt(1));
		AddXdata(aname(id), _T("LCZNOTE"), STRING_TYPE, szStrL);
		//break;
		//}

	}

	pGroup->close();
}
