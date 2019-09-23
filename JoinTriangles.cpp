// JoinTriangles.cpp: implementation of the CJoinTriangles class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "JoinTriangles.h"
#include "dialogs.h"
//#include "Grid.h"
#include "cassGlobal.h"
//extern void SetColorByLayer(ads_name ent);
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#include "JoinTriangles.h"
//ˮ���ͼ��
TCHAR g_SsdLayer[30] = _T("GCD");
//////////////////////////////////////////////////////////////////

//���ֿ�߱�
extern double g_dTextFactor;
//ϵͳĿ¼
TCHAR g_szSystemPath[MAX_PATH];
//������ֵ
double TOLE = 0.001;
//�û�Ŀ¼
TCHAR g_szUsePath[MAX_PATH] = _T("..\\demo\\");
//�ȸ���ͼ��
#ifdef SUZHOU
TCHAR g_DgxLayer[30] = _T("TER");
#elif TJ_SURVEY
TCHAR g_DgxLayer[30] = _T("DM");
#else
TCHAR g_DgxLayer[30] = _T("DGX");
#endif
//������ͼ��
#ifdef SUZHOU
TCHAR g_DsxLayer[30] = _T("HYD");
#elif TJ_SURVEY
TCHAR g_DsxLayer[30] = _T("DM");
#else
TCHAR g_DsxLayer[30] = _T("DSX");
#endif
//�̵߳�ͼ��
TCHAR g_GcdLayer[30] = _T("GCD");

//�̵߳���������
TCHAR g_szGcdWordStyle[100] = _T("��������");

int g_nHighBits = 2;
//�߳�ע��С����λ��
int g_dHighBits = 2;
//����Ĭ�Ͽ���
double g_dKanHigh = 1.0;
//��������С��
double g_Dtm = 10.0;
//�̵߳��ָ�
double g_dGcdHeight = 2.0;
//������λ��
int g_nTufaDeci = 1;
//���������������߳�ע��λ��
int g_nFgwHighDeci = 3;
//������߸߳�ע��λ��
int g_nHdmHighDeci = 2;
//������߾���ע��λ��
int g_nHdmDistDeci = 2;
//���̼��㿼�Ƕ���
bool g_bConsiderDoukan = false;

bool g_bDrawPoly3D = true;

AcGePoint3dArray g_HaituPosArr; //��ͼע������λ��

AcGePoint3dArray g_SeaColrArr;

//����������
void DrawOneSjx(ads_point pt1, ads_point pt2, ads_point pt3, bool bChange = false)
{
	AcGePoint3dArray pt;
	AcGePoint3d point;
	pt.setGrowLength(3);
	point[X] = pt1[X]; point[Y] = pt1[Y]; point[Z] = pt1[Z]; pt.append(point);
	point[X] = pt2[X]; point[Y] = pt2[Y]; point[Z] = pt2[Z]; pt.append(point);
	point[X] = pt3[X]; point[Y] = pt3[Y]; point[Z] = pt3[Z]; pt.append(point);

	AcDb3dPolyline *pPline = new AcDb3dPolyline(AcDb::k3dSimplePoly, pt, Adesk::kTrue);
	if (true == bChange) pPline->setColorIndex(4);
	AcDbObjectId objId;
	AppendEntity(pPline, objId);
	pPline->close();

	//�ı�ͼ��
	if (true == bChange)
	{
		struct resbuf result;
		acedGetVar(_T("CLAYER"), &result);
		if (_tcsicmp(result.resval.rstring, _T("SJW")))
		{
			ads_name ent;
			acdbGetAdsName(ent, objId);
			setlayer(ent, _T("sjw"));
		}
	}
}

//�õ�������������������,���ض�����
int Sjw_GetDtm3pt(ads_name ent, ads_point pt1, ads_point pt2, ads_point pt3)
{
	int SumPt = 0;
	ads_name ent1, ent2;
	struct resbuf *rc, *rb;
	ads_point pt;

	rc = ads_entget(ent);
	rb = rc; while (rb->restype != 0) rb = rb->rbnext;
	if (_tcsicmp(rb->resval.rstring, _T("POLYLINE"))) { ads_relrb(rc); return SumPt; }
	rb = rc; while (rb->restype != 8) rb = rb->rbnext;
	if (_tcsicmp(rb->resval.rstring, _T("SJW"))) { ads_relrb(rc); return SumPt; }
	rb = rc; while (rb->restype != 70) rb = rb->rbnext;
	if (rb->resval.rint % 2 == 0) { ads_relrb(rc); return SumPt; }
	ads_relrb(rc);

	ads_entnext(ent, ent1);
	while (1)
	{
		rc = ads_entget(ent1);
		rb = rc; while (rb->restype != 0) rb = rb->rbnext;
		if (!_tcsicmp(rb->resval.rstring, _T("SEQEND"))) { ads_relrb(rc); break; }
		rb = rc; while (rb->restype != 10) rb = rb->rbnext;
		ads_point_set(rb->resval.rpoint, pt);
		ads_relrb(rc);
		if (0 == SumPt) ads_point_set(pt, pt1);
		else if (1 == SumPt) ads_point_set(pt, pt2);
		else if (2 == SumPt) ads_point_set(pt, pt3);
		SumPt++;
		if (SumPt > 3) break;
		ads_entnext(ent1, ent2);
		ads_name_set(ent2, ent1);
	}
	return SumPt;
}

void Sjw_SaveSjx(ads_name ssName, TCHAR *FileName)
{
	ads_name ent;
	long ll, length;
	FILE *fp;
	ads_point pt1, pt2, pt3;

	if ((fp = _tfopen(FileName, _T("wt"))) == NULL)
	{
		ads_alert(_T("�޷����ļ�")); return;
	}
	length = 0; ads_sslength(ssName, &length);
	for (ll = 0; ll < length; ll++)
	{
		ads_ssname(ssName, ll, ent);
		if (Sjw_GetDtm3pt(ent, pt1, pt2, pt3) < 3) continue;
		_ftprintf(fp, _T("%.3f\n%.3f\n%.3f\n"), pt1[X], pt1[Y], pt1[Z]);
		_ftprintf(fp, _T("%.3f\n%.3f\n%.3f\n"), pt2[X], pt2[Y], pt2[Z]);
		_ftprintf(fp, _T("%.3f\n%.3f\n%.3f\n"), pt3[X], pt3[Y], pt3[Z]);
	}
	_ftprintf(fp, _T("nil\n"));
	fclose(fp);
}

//��ʵ��ת��Ϊ�����ʽ
void ConvertDoubleToKplus(double dRRR, TCHAR *szStr, int nWidth, TCHAR *szLCZMC, bool bSorE)
{
	if(fabs(dRRR) < EPS)
	{
		_stprintf(szStr, _T("%s+000"), szLCZMC);
		return;
	}
	TCHAR ccc[50], szTemp[50];
	dRRR += 0.00001;
	int kilo = (int)(dRRR / 1000.0);
	double dTemp = dRRR - kilo*1000.0;
	if (dRRR < 10) _tcscpy(ccc, _T("00"));
	else if (dRRR < 100) _tcscpy(ccc, _T("0"));
	else _tcscpy(ccc, _T(""));
	if (bSorE)//�����յ����Ϊ����
		nWidth = 0;
	acdbRToS(dTemp, 2, nWidth, szTemp);
	if (!_tcsncmp(szTemp, _T("1000"), 4))
	{
		kilo++;
		acdbRToS(0.0, 2, nWidth, szTemp);
		_tcscpy(ccc, _T("00"));
	}
	//_stprintf(szStr, _T("K%d+%s%s"), kilo, ccc, szTemp);
	_stprintf(szStr, _T("%s+%s%s"), szLCZMC, ccc, szTemp);
}

//��ȡ�߳�ˮ����ATTRIBUTE���ָ߳�
bool GetAttributeHeight(ads_name ent, double &dHeight)
{
	AcDbObjectId objId;
	AcDbBlockReference *pBlock;
	TCHAR szHeight[100] = _T(""), szInteger[100] = _T(""), szDecimal[100] = _T("");

	if (Acad::eOk != acdbGetObjectId(objId, ent)) return false;
	if (Acad::eOk != acdbOpenObject(pBlock, objId, AcDb::kForRead)) return false;
	AcDbObjectIterator *pIteror = pBlock->attributeIterator();
	for (pIteror->start(); !pIteror->done(); pIteror->step())
	{
		AcDbObjectId attId = pIteror->objectId();
		AcDbAttribute *pAtt;
		if (Acad::eOk != acdbOpenObject(pAtt, attId, AcDb::kForRead)) continue;
		if (!_tcsicmp(pAtt->tag(), _T("integer"))) _tcscpy(szInteger, pAtt->textString());
		else if (!_tcsicmp(pAtt->tag(), _T("decimal"))) _tcscpy(szDecimal, pAtt->textString());
		else if (!_tcsicmp(pAtt->tag(), _T("height"))) _tcscpy(szHeight, pAtt->textString());
		pAtt->close();
	}
	delete pIteror;
	pBlock->close();

	if (_tcscmp(szInteger, _T("")) && _tcscmp(szDecimal, _T("")))
	{
		_stprintf(szHeight, _T("%s.%s"), szInteger, szDecimal);
	}
	if (_tcscmp(szHeight, _T("")))
	{
		dHeight = _tstof(szHeight);
		return true;
	}
	else
	{
		return false;
	}
}

int mstr_cmp(const void *pa, const void *pb)
{
	printf("%s\n%s\n", pa, pb);
	TCHAR *a = (TCHAR *)pa;
	TCHAR *b = (TCHAR *)pb;
	int flag = 0;
	while (*a == *b && *a != _T('\0'))
	{
		a++;
		b++;
	}
	flag = *a - *b;
	while (*a >= _T('0') && *a <= _T('9') && *b >= _T('0') && *b <= _T('9'))
	{
		a++;
		b++;
	}
	if (*a >= _T('0') && *a <= _T('9'))
		flag = 1;
	else if (*b >= _T('0') && *b <= _T('9'))
		flag = -1;
	return flag;
}

int Partition(vector<CString> &a, int low, int high)
{
	CString x = a[high];//��������������һ������Ϊ��Ԫ����������������л���
	int i = low - 1;//i�����һ��С����Ԫ�������±�
	for (int j = low; j < high; j++)//�����±���low��high-1����
	{

		if (mstr_cmp(a[j].GetBuffer(), x.GetBuffer()) < 0)//�����С����Ԫ�Ļ��ͽ�i��ǰŲ��һ��λ�ã����ҽ���j��i���ֱ�ָ�����
		{
			CString temp;
			i++;
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
		}
	}
	//���������ѭ��֮���±�Ϊ��low��i������i�������;�ΪС��x�����ˣ����ڽ���Ԫ��i+1λ������������н���
	a[high] = a[i + 1];
	a[i + 1] = x;
	return i + 1;
}

void QuickSort(vector<CString> &a, int low, int high)
{
	if (low < high)
	{
		int q = Partition(a, low, high);
		QuickSort(a, low, q - 1);
		QuickSort(a, q + 1, high);
	}
}

//��ͼ��̵߳�д�������ļ�
int getgchfile(TCHAR *szFileName, int nSel, bool bDtm = true, bool bText = false)
{//nSel��λ��=1:��ʾ 2:����ʾ,20-29:ֻ��ˮ���
	FILE *fp;
	long l, Length = 0;
	ads_name ssName, ent;
	struct resbuf *rc, *rb;
	TCHAR szStdm[50], szEnType[50];
	ads_point pt;
	int nType;
	bool bOnlySsd = false;//�Ƿ�ֻ����ˮ���
	int nTips = nSel % 10;
	if (1 == nSel / 10) bOnlySsd = true;

	//Code1(6);
	if (1 == nTips)
	{
		int val = ads_getint(_T("\n��ѡ��:(1)ѡȡ�̵߳�ķ�Χ (2)ֱ��ѡȡ�̵߳����Ƶ�<1>"), &nType);
		if (RTCAN == val) return 0;
		else if (val != RTNORM) nType = 1;
		if (1 != nType && 2 != nType) nType = 1;
	}
	else nType = 3;

	if (3 == nType)
	{//ͼ�ϸ߳�ȫѡ
		ads_ssget(_T("X"), NULL, NULL, NULL, ssName);
	}
	else if (2 == nType)
	{
		ads_printf(_T("\nѡ��̵߳����Ƶ�:"));
		ads_ssget(NULL, NULL, NULL, NULL, ssName);
	}
	else
	{
		ads_entsel(_T("\n��ѡȡ��ģ����߽�:"), ent, pt);
		if (GetPolyType(ent, true) < 10)
		{
			ads_alert(_T("��ѡ���ո�����")); return 0;
		}
		AcGePoint2dArray plNodes;
		GetPlList(ent, plNodes);
		ssFromNodes(plNodes, ssName, _T("CP"));
	}
	AcDbObjectIdArray idsTemp;
	SSToIdArr(ssName, idsTemp);
	RemoveSameId(idsTemp);
	IdArrToSS(idsTemp, ssName);

	AcGePoint3d pt3d, pt3dE;
	AcGePoint3dArray Nodes;
	AcGeIntArray DtmArr;
	ads_sslength(ssName, &Length);
	map<CString, AcGePoint3d> mapGCD_pt;
	map<CString, int> mapGCD_DM;
	for (l = 0; l < Length; l++)
	{
		ads_ssname(ssName, l, ent);
		GetEntityType(ent, szEnType);
		GetStdm(ent, szStdm);

		if (true == bOnlySsd)
		{//ֻ����ˮ���
			if (_tcscmp(szStdm, _T("186400")) || _tcsicmp(szEnType, _T("INSERT"))) continue;
		}
		else
		{
			if (!_tcsicmp(szEnType, _T("INSERT")) || /*!_tcsicmp(szEnType,_T("POINT")) ||*/ !_tcsicmp(szEnType, _T("SHAPE")))
			{
				if (false == CheckGcdCode(szStdm, true)) continue;
			}
			else continue;
		}

		rc = ads_entget(ent);
		rb = rc;
		while (rb->restype != 10) rb = rb->rbnext;
		ads_point_set(rb->resval.rpoint, asDblArray(pt3d));
		ads_relrb(rc);

		if (true == bText && !_tcsicmp(szEnType, _T("INSERT")))
		{//��ATTRIBUTE�ĸ߳�
			double dTemp;
			if (true == GetAttributeHeight(ent, dTemp)) pt3d[Z] = dTemp;
		}
		//if ( 0.0 == pt3d[Z] ) continue;

		short nDtm = 0;
		rc = xdataget(ent);
		if (NULL != rc)
		{
			rb = rc;
			while (rb)
			{
				if (1070 == rb->restype)
				{
					nDtm = rb->resval.rint;
					break;
				}
				rb = rb->rbnext;
			}
			ads_relrb(rc);
		}
		if (true == bDtm)
		{
			if (0 != nDtm) continue;
		}
		TCHAR szID[100] = { 0 };
		if (GetXDataCount(ent, _T("SOUTH")) >= 2)
		{
			READMulXData(aname(ent), _T("SOUTH"), szID, STRING_TYPE, 1);
		}


		if (_tcslen(szID) == 0)
		{
			bool bTooNear = false;
			if (true == bDtm)
			{
				for (int i = 0; i < Nodes.length(); i++)
				{
					pt3dE = Nodes.at(i);
					if (fabs(pt3d[X] - pt3dE[X]) < TOLE && fabs(pt3d[Y] - pt3dE[Y]) < TOLE)
					{
						bTooNear = true; break;
					}
				}
			}
			if (false == bTooNear)
			{
				Nodes.append(pt3d);
				DtmArr.append(nDtm);
			}
		}
		else
		{
			map<CString, AcGePoint3d>::iterator it = mapGCD_pt.find(CString(szID));
			if (it != mapGCD_pt.end())
			{
				acutPrintf(_T("���:%s��ͻ,����:%f,%f,%f;%f,%f,%f"), szID, it->second.x, it->second.y, it->second.z, pt3d[X], pt3d[Y], pt3d[Z]);
			}
			else
			{
				mapGCD_pt.insert(pair<CString, AcGePoint3d>(CString(szID), pt3d));
				mapGCD_DM.insert(pair<CString, int>(CString(szID), nDtm));
			}

		}

	}
	ads_ssfree(ssName);

	fp = _tfopen(szFileName, _T("wt"));
	map<CString, AcGePoint3d>::iterator it_PT = mapGCD_pt.begin();
	map<CString, int>::iterator it_DM = mapGCD_DM.begin();
	CString strNameLast;
	CString strX;
	CString strY;
	CString strZ;
	double dX = 0;
	double dY = 0;
	double dZ = 0;

	// 	char a[][128] = { "ab1", "ab10", "ab2" };
	// 	int i;
	// 	qsort(a, sizeof(a) / 128, 128, mstr_cmp);
	vector<CString> veStr;
	for (it_PT = mapGCD_pt.begin(); it_PT != mapGCD_pt.end(); it_PT++)
	{
		veStr.push_back(it_PT->first);
	}

	QuickSort(veStr, 0, veStr.size() - 1);

	for (int i = 0; i < (int)veStr.size(); i++)
	{
		it_PT = mapGCD_pt.find(veStr[i]);
		it_DM = mapGCD_DM.find(veStr[i]);
		if (it_PT == mapGCD_pt.end())
		{
			continue;
		}
		pt3d = it_PT->second;
		TCHAR szCode[30] = _T("");
		if (it_DM != mapGCD_DM.end())
		{
			if (0 != it_DM->second) _tcscpy(szCode, _T("*"));
		}
		strX.Format(_T("%.4f"), pt3d[X]);
		strY.Format(_T("%.4f"), pt3d[Y]);
		strZ.Format(_T("%.4f"), pt3d[Z]);
		if (_ttoi(strX.Right(1)) >= 5)
		{
			strX = strX.Left(strX.GetLength() - 1);
			dX = _tstof(strX);
			dX += 0.001;
		}
		else
		{
			strX = strX.Left(strX.GetLength() - 1);
			dX = _tstof(strX);
		}


		if (_tstoi(strY.Right(1)) >= 5)
		{
			strY = strY.Left(strY.GetLength() - 1);
			dY = _tstof(strY);
			dY += 0.001;
		}
		else
		{
			strY = strY.Left(strY.GetLength() - 1);
			dY = _tstof(strY);
		}

		if (_ttoi(strZ.Right(1)) >= 5)
		{
			strZ = strZ.Left(strZ.GetLength() - 1);
			dZ = _tstof(strZ);
			dZ += 0.001;
		}
		else
		{
			strZ = strZ.Left(strX.GetLength() - 1);
			dZ = _tstof(strZ);
		}
		_ftprintf(fp, _T("%s,%s,%.3f,%.3f,%.3f\n"), it_PT->first, szCode, dX, dY, dZ);
		strNameLast = it_PT->first;
	}
	// 	for (; it_PT != mapGCD_pt.end() && it_DM != mapGCD_DM.end(); it_PT++ , it_DM++)
	// 	{
	// 
	// 	}

	int nStart = mapGCD_pt.size() + 1;

	for (int i = 0; i < Nodes.length(); i++)
	{
		pt3d = Nodes.at(i);
		TCHAR szCode[30] = _T("");
		if (0 != DtmArr.at(i)) _tcscpy(szCode, _T("*"));
		_ftprintf(fp, _T("%d,%s,%.3f,%.3f,%.3f\n"), i + nStart, szCode, pt3d[X], pt3d[Y], pt3d[Z]);
	}
	fclose(fp);
	if (Nodes.isEmpty())
		return nStart;
	else
		return Nodes.length();

}

//�����������һ��߳�
bool ThreeD_HeightOnDxing(ads_name ent, AcGePoint3d ptIn, double &dHeight)
{
	AcDbCurve *pCurve;
	AcDbObjectId objId;
	AcGePoint3d pt3d;
	double dParam, dDist;
	AcGePoint3dArray kzdPtArr;
	AcGeDoubleArray kzdDistArr;
	int i;

	if (GetPolyType(ent) < 0) return false;
	if (threeD_GetGcdOnDxing(ent, kzdPtArr, kzdDistArr) < 1) return false; //��ȡ���ϸ߳̿��Ƶ�
	while (1)
	{//ɾ���Ƿ��߳�
		bool bErase = false;
		for (i = 0; i < kzdPtArr.length(); i++)
		{
			if (kzdPtArr[i].z < -1000.0)
			{
				kzdPtArr.removeAt(i);
				kzdDistArr.removeAt(i);
				bErase = true;
				break;
			}
		}
		if (false == bErase) break;
	}

	if (Acad::eOk != acdbGetObjectId(objId, ent)) return false;
	if (acdbOpenObject(pCurve, objId, AcDb::kForRead) != Acad::eOk) return false;
	pCurve->getClosestPointTo(ptIn, pt3d);
	pCurve->getParamAtPoint(pt3d, dParam);
	pCurve->getDistAtParam(dParam, dDist);
	pCurve->close();

	dHeight = threeD_NeichaHeight(dDist, kzdPtArr, kzdDistArr);
	return true;
}


//��ȡ�����߾���ʵ�����̸߳�
void ThreeD_GetEntityHeightAlongLine(ads_name entPl, CJoinTriangles &sjw, AcGePoint3dArray &dw_lcNodes, AcGeDoubleArray &dw_lcDists)
{
	AcGePoint2dArray Nodes;
	ads_name ssName, ent;
	long l, Length;
	AcDbCurve *pBound, *pCurve;
	AcDbObjectId objId;
	TCHAR szCode[50], szEntype[50], szDmCode[50];
	AcGePoint3d pt3d, ptNear;
	ads_point pt;
	int i;
	double dDist, dHeight;
	AcGePoint3d pnt(0, 0, 0); AcGeVector3d normal(0, 0, 1); AcGePlane projPlane(pnt, normal);

	GetStdm(entPl, szDmCode);
	if (false == GetNodesHaveArc(entPl, Nodes)) return;
	acdbGetObjectId(objId, entPl);
	if (acdbOpenObject(pBound, objId, AcDb::kForRead) != Acad::eOk) return;

	ssFromNodes(Nodes, ssName, _T("F"));
	ads_sslength(ssName, &Length);
	for (l = 0; l < Length; l++)
	{
// 		MSG message;
// 		while (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
// 		{
// 
// 			::TranslateMessage(&message);
// 			::DispatchMessage(&message);
// 		}

		ads_ssname(ssName, l, ent);
		GetEntityType(ent, szEntype);
		GetStdm(ent, szCode);
		if ((!_tcscmp(szDmCode, _T("992000")) || !_tcscmp(szDmCode, _T("992000"))) && !_tcsicmp(szEntype, _T("INSERT")))
		{
			if (!_tcscmp(szCode, _T("992001")) || !_tcscmp(szCode, _T("202101")))
			{//���������ϼӵĵ�͸̵߳�
				struct resbuf *rc, *rb;
				rc = ads_entget(ent);
				rb = rc; while (rb->restype != 10) rb = rb->rbnext;
				pt3d.x = rb->resval.rpoint[X]; pt3d.y = rb->resval.rpoint[Y]; pt3d.z = rb->resval.rpoint[Z];
				ads_relrb(rc);

				if (fabs(pt3d.z) > 0.0001)
				{//���и߳�ֵ
					pBound->getClosestPointTo(pt3d, ptNear);
					pBound->getDistAtPoint(ptNear, dDist);
					for (i = 0; i < dw_lcDists.length(); i++)
					{
						if (dDist < dw_lcDists[i]) break;
					}
					ptNear.z = pt3d.z; dw_lcNodes.insertAt(i, ptNear);
					dw_lcDists.insertAt(i, dDist);
					continue;
				}
			}
		}

		if (GetPolyType(ent) < 0) continue;
		if (ads_name_equal(ent, entPl)) continue; //����ʵ�岻����
		if (!_tcscmp(szCode, _T(""))) continue; //û���ԵĲ�����
		if (NULL != _tcschr(szCode, '-')) continue; //����ʵ�岻����
		if ('3' == szCode[0]) continue; //�ڵ�ͼ�߲�����
		if (!_tcsncmp(szCode, _T("17"), 2) || !_tcsncmp(szCode, _T("19"), 2)) continue; //������߲�����

		AcGePoint3dArray InterPts;
		acdbGetObjectId(objId, ent);
		if (acdbOpenObject(pCurve, objId, AcDb::kForRead) != Acad::eOk) continue;
		pBound->intersectWith(pCurve, AcDb::kOnBothOperands, projPlane, InterPts);
		pCurve->close();

		for (int i = 0; i < InterPts.length(); i++)
		{
			pt3d = InterPts.at(i);
			pBound->getClosestPointTo(pt3d, ptNear);
			pBound->getDistAtPoint(ptNear, dDist);

			bool bHave = false;
			if (true == CheckDzxCode(szCode))
			{//��ֵ��
				GetPolyHeight(ent, dHeight);
				bHave = true;
			}
			else if (true == CheckDoukanCode(szCode) || !_tcscmp(szCode, _T("201199")))
			{//������
				if (true == ThreeD_HeightOnDxing(ent, pt3d, dHeight)) bHave = true;
			}
			if (false == bHave)
			{
				pt[X] = pt3d.x; pt[Y] = pt3d.y; pt[Z] = pt3d.z;
				sjw.cz1(pt);
				dHeight = pt[Z];
			}

			bool bChongfu = false;
			int k;
			for (k = 0; k < dw_lcDists.length(); k++)
			{
				if (fabs(dDist - dw_lcDists[k]) < 0.01)
				{
					bChongfu = true; break;
				}
				if (dDist < dw_lcDists[k]) break;
			}
			if (true == bChongfu) continue;
			pt3d.z = dHeight; dw_lcNodes.insertAt(k, pt3d);
			dw_lcDists.insertAt(k, dDist);
		}
	}
	ads_ssfree(ssName);
	pBound->close();
}



// void SetColorByLayer(AcDbObjectId id)
// {
// 	AcDbEntity *pEnt;
// 	if (Acad::eOk == acdbOpenObject(pEnt, id, AcDb::kForWrite))
// 	{
// 		AcCmColor color;
// 		color.setColorMethod(AcCmEntityColor::kByLayer);
// 		pEnt->setColor(color);
// 		pEnt->close();
// 	}
// }


// void SetColorByLayer(ads_name ent)
// {
// 	AcDbObjectId id;
// 	acdbGetObjectId(id, ent);
// 	SetColorByLayer(id);
// }

//������ֵ
extern double TOLE;


//����Ĭ�Ͽ���
extern double g_dKanHigh;
//��������С��
extern double g_Dtm;
//�̵߳�ͼ��
extern TCHAR g_DgxLayer[30];
//ˮ���ͼ��
extern TCHAR g_DsxLayer[30];
//���̼��㿼�Ƕ���
extern bool g_bConsiderDoukan;
//�߳�ע��С����λ��
extern int g_dHighBits;
//�û�Ŀ¼
extern TCHAR g_szUsePath[MAX_PATH];

////����������
//extern void DrawOneSjx(ads_point pt1,ads_point pt2,ads_point pt3,bool bChange=false);
////�õ�������������������,���ض�����
//extern int Sjw_GetDtm3pt(ads_name ent,ads_point pt1,ads_point pt2,ads_point pt3);
////����ͼ��������
//extern void Sjw_SaveSjx(ads_name ssName,TCHAR *FileName);
////��ͼ��̵߳�д�������ļ�
//extern int getgchfile(TCHAR *szFileName,int nSel,bool bDtm=true,bool bText=false);

//��˼�����
extern SENSE4_CONTEXT s4_context;
//����
extern unsigned short g_nDogValue;
extern bool Reduce_DogTimes();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJoinTriangles::CJoinTriangles()
: m_bSjwPick(false)
{
	point = NULL;
	llb1 = NULL;
	llb2 = NULL;
	llb3 = NULL;
	Doukan = NULL;
	m_Kanshu = 0;
	m_LenXyz = -1;
	m_LenSjx = 0;
	m_bKan = false;
	m_MinAngle = 10.0;

	m_bDixing = FALSE;
	m_DrawType = 1;
	_tcscpy(m_datFile,_T(""));
	m_nColor = 0;
	m_bSjwPick = false; //������,2004.8.6

//#ifdef CSKCSJY
	m_nDxingPointNum = 0;
//#endif

	m_bOnlyDat = false; //Ĭ������ͼ���Ҷ����ȵ�����
}

CJoinTriangles::~CJoinTriangles()
{
	ReleaseMemory();
}

void CJoinTriangles::ReleaseMemory()
{
	if ( NULL != point ) { delete[] point; point = NULL; }
	if ( NULL != llb1 ) { delete[] llb1; llb1 = NULL; }
	if ( NULL != llb2 ) { delete[] llb2; llb2 = NULL; }
	if ( NULL != llb3 ) { delete[] llb3; llb3 = NULL; }
	if ( NULL != Doukan ) { delete[] Doukan; Doukan = NULL; }
	
	dxArray.removeSubArray(0,dxArray.length()-1);
	for (int i=0; i<m_DxingArr.GetSize(); i++)
	{
		m_DxingArr[i].removeSubArray(0,m_DxingArr[i].length()-1);
	}
	m_DxingArr.RemoveAll();
}

bool CJoinTriangles::ReadDatFile()
{
	FILE *fp;
	int nSum,nSjx;
	ads_point pt1,pt2;
	TCHAR szMsg[255],szStr[255],szName[50],szCode[50];
	bool CheckSamePt=true;

	fp = _tfopen(m_datFile,_T("rt"));
	if ( NULL == fp ) {
		ads_alert(_T("�޷����ļ�")); return false;
	}
	nSum = 0; 
	while ( _fgetts(szStr,255,fp) ) nSum++;
	rewind(fp);
	if ( nSum > 20000 ) {
		_tcscpy(szMsg,_T("\n�����ɢ�����,�������ظ�����Ϸ�ʱ��,�Ƿ���?(Y/N) <N>")); 
		if ( ads_getstring(0,szMsg,szStr) != RTNORM ) _tcscpy(szStr,_T("N"));
		if ( _tcsnicmp(szStr,_T("Y"),1) ) CheckSamePt = false;
	}
	nSum *= 2; //�̵߳���,�ٶ�ÿ���̵߳㶼�ǵ����ߵĽ��
//#ifdef CSKCSJY
	nSum += m_nDxingPointNum;
//#endif

	//DogResult = rand()*100000;
	point = new ads_point[nSum];
	if (NULL == point) {
		fclose(fp); ads_alert(_T("�ڴ泬��!\n")); return false;
	}
	nSjx = nSum * 2;//�����������
	llb1 = new int[nSjx];
	if (NULL == llb1) {
		fclose(fp); ads_alert(_T("�ڴ泬��!")); return false;
	}
	llb2 = new int[nSjx];
	if (NULL == llb2) {
		fclose(fp); ads_alert(_T("�ڴ泬��!")); return false;
	}
	llb3 = new int[nSjx];
	if (NULL == llb2) {
		fclose(fp); ads_alert(_T("�ڴ泬��!")); return false;
	}

	//���������ļ�
	m_LenXyz = 0;
	while ( _fgetts(szStr,255,fp) ) 
	{
		int val = ReadDataLine(szStr,szName,szCode,point[m_LenXyz]);
		if (0 == val) continue;
		else if (val != 4) {
			fclose(fp); 
			_stprintf(szMsg,_T("�������ļ���ʽ����!������:%s"),szName); 
			ads_alert(szMsg);
			return false;
		}
#ifdef GD_HANGDAO //�㶫��������Ϣ����,ֻ����һλС��
		double dSim,dTemp; int nTemp;
		dTemp = point[m_LenXyz][X]; dSim = 1.0; if ( dTemp < 0 ) { dTemp *= -1.0; dSim = -1.0; }
		nTemp = (int)( dTemp * 10 + 0.5 ); point[m_LenXyz][X] = dSim * (nTemp / 10.0);
		dTemp = point[m_LenXyz][Y]; dSim = 1.0; if ( dTemp < 0 ) { dTemp *= -1.0; dSim = -1.0; }
		nTemp = (int)( dTemp * 10 + 0.5 ); point[m_LenXyz][Y] = dSim * (nTemp / 10.0);
		dTemp = point[m_LenXyz][Z]; dSim = 1.0; if ( dTemp < 0 ) { dTemp *= -1.0; dSim = -1.0; }
		nTemp = (int)( dTemp * 10 + 0.5 ); point[m_LenXyz][Z] = dSim * (nTemp / 10.0);
#else
		if (point[m_LenXyz][Z]==0.0) continue;
#endif

		if ( _tcschr(szCode,'*') ) continue; //������"*"��ʾ���μӹ���
		if (0 == m_LenXyz) {
			ads_point_set(point[m_LenXyz],pt1);
			ads_point_set(point[m_LenXyz],pt2);
		}
		else {
			if (point[m_LenXyz][X] < pt1[X]) pt1[X] = point[m_LenXyz][X];
			if (point[m_LenXyz][Y] < pt1[Y]) pt1[Y] = point[m_LenXyz][Y];
			if (point[m_LenXyz][X] > pt2[X]) pt2[X] = point[m_LenXyz][X];
			if (point[m_LenXyz][Y] > pt2[Y]) pt2[Y] = point[m_LenXyz][Y];
		}

		bool TooNear=false;
		if ( true == CheckSamePt ) {
			for (int i=0; i<m_LenXyz; i++) {
				if ( fabs(point[m_LenXyz][X]-point[i][X])<TOLE && fabs(point[m_LenXyz][Y]-point[i][Y])<TOLE ) { 
					TooNear=true; break; 
				}
			}
		}
		if (false == TooNear) m_LenXyz++;
	}
 	fclose(fp);
	if ( 1 == m_Type ) {
		pt1[X]-=20.0; pt1[Y]-=20.0; pt2[X]+=20.0; pt2[Y]+=20.0;
		ads_command(RTSTR,_T("zoom"),RTSTR,_T("W"),RTPOINT,pt1,RTPOINT,pt2,RTNONE);
	}
	//SecondConvert = *pResult; //����2003.4.1
	//DogData=&LinkConvert; DogBytes=4; DogConvert();
	return true;
}

bool CJoinTriangles::ReadNodes(AcGePoint3dArray &Nodes)
{
	ads_point pt1,pt2;
	TCHAR szMsg[255],szStr[255];
	bool CheckSamePt=true;

	int nSum = Nodes.length();
	if ( nSum > 20000 ) {
		_tcscpy(szMsg,_T("\n�����ɢ�����,�������ظ�����Ϸ�ʱ��,�Ƿ���?(Y/N) <N>")); 
		if ( ads_getstring(0,szMsg,szStr) != RTNORM ) _tcscpy(szStr,_T("N"));
		if ( _tcsnicmp(szStr,_T("Y"),1) ) CheckSamePt = false;
	}
	nSum *= 2; //�̵߳���,�ٶ�ÿ���̵߳㶼�ǵ����ߵĽ��
//#ifdef CSKCSJY
	nSum += m_nDxingPointNum;
//#endif

	//DogResult = rand()*100000;
	point = new ads_point[nSum];
	if (NULL == point) { ads_alert(_T("�ڴ泬��!\n")); return false; }
	int nSjx = nSum * 2;//�����������
	llb1 = new int[nSjx];
	if (NULL == llb1) { ads_alert(_T("�ڴ泬��!\n")); return false; }
	llb2 = new int[nSjx];
	if (NULL == llb2) { ads_alert(_T("�ڴ泬��!\n")); return false; }
	llb3 = new int[nSjx];
	if (NULL == llb2) { ads_alert(_T("�ڴ泬��!\n")); return false; }

	//������������
	m_LenXyz = 0;
	for (int i=0; i<Nodes.length(); i++)
	{
		ads_point_set(asDblArray(Nodes.at(i)),point[m_LenXyz]);
		//if (point[m_LenXyz][Z]==0.0) continue;
		if (0 == m_LenXyz) {
			ads_point_set(point[m_LenXyz],pt1);
			ads_point_set(point[m_LenXyz],pt2);
		}
		else {
			if (point[m_LenXyz][X] < pt1[X]) pt1[X] = point[m_LenXyz][X];
			if (point[m_LenXyz][Y] < pt1[Y]) pt1[Y] = point[m_LenXyz][Y];
			if (point[m_LenXyz][X] > pt2[X]) pt2[X] = point[m_LenXyz][X];
			if (point[m_LenXyz][Y] > pt2[Y]) pt2[Y] = point[m_LenXyz][Y];
		}

		bool TooNear=false;
		if ( true == CheckSamePt ) {
			for (int i=0; i<m_LenXyz; i++) {
				if ( fabs(point[m_LenXyz][X]-point[i][X])<TOLE && fabs(point[m_LenXyz][Y]-point[i][Y])<TOLE ) { 
					TooNear=true; break; 
				}
			}
		}
		if (false == TooNear) m_LenXyz++;
	}
	if ( 1 == m_Type ) {
		pt1[X]-=20.0; pt1[Y]-=20.0; pt2[X]+=20.0; pt2[Y]+=20.0;
		ads_command(RTSTR,_T("zoom"),RTSTR,_T("W"),RTPOINT,pt1,RTPOINT,pt2,RTNONE);
	}
	//SecondConvert = *pResult; //����2003.4.1
	//DogData=&LinkConvert; DogBytes=4; DogConvert();
	return true;
}

//������point��Ѱ��ĳ������
int CJoinTriangles::GetNumFromGcd(double x,double y,int nUpper)
{
	for (int i=0; i<=nUpper; i++) 
	{
		if ( fabs(point[i][X]-x) < TOLE && fabs(point[i][Y]-y) < TOLE ) return i;
	}
	return -1;
}

//ѡ�������
void CJoinTriangles::Dgx_Dixing()
{
	long length=0,l;
	ads_name ent,ssName;

	ads_printf(_T("��ѡ�������:(������Ӧ���Ѳ�㣬�粻ѡ��ֱ�ӻس�)\n"));
	if ( RTNORM != ads_ssget(NULL,NULL,NULL,NULL,ssName) ) return;
	ads_sslength(ssName,&length);
	for (l = 0; l < length; l++)
	{
		ads_ssname(ssName,l,ent);
		int nPolyType = GetPolyType(ent,true);
		if ( nPolyType < 0 ) continue;

		AcGePoint2d pt2d;
		AcGePoint2dArray Nodes;
		GetPlList(ent,Nodes);
		if (nPolyType >= 10) Nodes.append(Nodes.first());
		for (int i = 0; i < (Nodes.length()-1); i++ ) {
			pt2d = Nodes.at(i); 
			int Number1 = GetNumFromGcd(pt2d[X],pt2d[Y],m_LenXyz);
			pt2d = Nodes.at(i+1); 
			int Number2 = GetNumFromGcd(pt2d[X],pt2d[Y],m_LenXyz);
			if ( Number1>=0 && Number2>=0 ) {
				dxArray.append(Number1); 
				dxArray.append(Number2); 
			}
		}
	}
	ads_ssfree(ssName);
}

bool CJoinTriangles::Get1040(ads_name ent, double &d1040)
{
	struct resbuf *rc,*rb;
	rc = xdataget(ent); 
	if ( NULL == rc ) return false;
	rb = rc; 
	bool bFind=false;
	while (rb) {
		if (1040 == rb->restype) { 
			d1040 = rb->resval.rreal; 
			bFind = true;
			break; 
		} 
		rb = rb->rbnext;
	}
	ads_relrb(rc);
	return bFind;
}

//�������̵߳�Ҳ�ڲ���߳���
void CJoinTriangles::threeD_InsertHeightOnDxing(AcGePoint3dArray &pt3dArr,AcGeDoubleArray &distArr)
{
	//���ҳ����Ƶ�
	AcGeDoubleArray kzd_DistArr;
	AcGePoint3dArray kzd_PtArr;
	int i;
	AcGePoint3d pt3d;

	for (i=0; i<pt3dArr.length(); i++)
	{
		pt3d = pt3dArr.at(i);
		if ( pt3d.z > -1000.0 ) {
			kzd_PtArr.append(pt3d);
			kzd_DistArr.append(distArr[i]);
		}
	}

	for (i=0; i<distArr.length(); i++)
	{
		AcGePoint3d pt3d = pt3dArr.at(i);
		if ( pt3d.z > -1000.0 ) continue;
		double dDist = distArr.at(i);
		pt3d.z = threeD_NeichaHeight(dDist,kzd_PtArr,kzd_DistArr);
		pt3dArr.setAt(i,pt3d);
	}
}

void CJoinTriangles::threeD_AddDxing(AcGePoint3dArray &pt3dArr,int BeforeKan)
{
	AcGeIntArray UnderKan;
	int nLen = pt3dArr.length();
	int i;
	for (i=0; i<nLen; i++) 
	{
		AcGePoint3d pt3d = pt3dArr.at(i);
		int Number = GetNumFromGcd(pt3d[X],pt3d[Y],BeforeKan);
		if (Number >= 0) {
			UnderKan.append(Number);
		}
		else {
			m_LenXyz++; 
			point[m_LenXyz][X]=pt3d.x; point[m_LenXyz][Y]=pt3d.y; point[m_LenXyz][Z]=pt3d.z;
			UnderKan.append(m_LenXyz);
		}
	}
	for (i=0; i<UnderKan.length()-1; i++) 
	{
		int Num1 = UnderKan.at(i); 
		int Num2 = UnderKan.at(i+1); 
		if ( Num1>=0 && Num2>=0 ) { 
			dxArray.append(Num1); dxArray.append(Num2); 
		}
	}
}

//�������ڲ�����߻�
void CJoinTriangles::threeD_InsertOnQuxian(AcGePoint3dArray &Nodes,AcGeDoubleArray &Bulges,AcGePoint3dArray &pt3dArr,AcGeDoubleArray &distArr)
{
	double dTotalDist=0.0;
	double dJiange=9.0*PI/180.0;
	int nLen = Nodes.length();

	for (int i=0; i<nLen; i++)
	{
		AcGePoint3d ptB = Nodes.at(i);
		pt3dArr.append(ptB);
		distArr.append(dTotalDist);
		if ( nLen-1 == i ) continue; //���һ��

		double dBulge = Bulges.at(i);
		AcGePoint3d ptE = Nodes.at(i+1);
		if ( fabs(dBulge) > 0.001 ) 
		{ //���ڻ���,��Ҫ�����
			double ang,dInter,dAngle,dAngB,dAngE,dRadius;
			ads_point pt,Center,pointB,pointE;
			AcGePoint3d pt3d;

			pointB[X]=ptB.x; pointB[Y]=ptB.y; pointB[Z]=0.0;
			pointE[X]=ptE.x; pointE[Y]=ptE.y; pointE[Z]=0.0;
			double Angle = ads_angle(pointB,pointE);
			pt[X] = (ptB[X]+ptE[X])/2; pt[Y] = (ptB[Y]+ptE[Y])/2; pt[Z] = 0.0;
			double Chord = distOF3d(ptB,ptE) / 2.0;
			double Ht = Chord * fabs(dBulge);
			dRadius = (Chord*Chord+Ht*Ht)/(2*Ht);
			if ( dBulge < 0.0 ) ads_polar(pt,Angle+PI/2,(Ht-dRadius),Center);
			else ads_polar(pt,Angle-PI/2,(Ht-dRadius),Center);

			dAngB = ads_angle(Center,pointB); 
			dAngE = ads_angle(Center,pointE);
			if (dBulge > 0.0) dAngle=dAngE-dAngB; else dAngle=dAngB-dAngE;
			if ( dAngle < 0.0 ) dAngle += PI*2;
			double dLength = dRadius * dAngle; //���γ���
			dTotalDist += dLength;

			int nNum = (int)(dAngle/dJiange) + 1;
			dInter = dAngle / nNum;
			for (int j=1; j<nNum; j++) 
			{
				if (dBulge > 0.0) { ang = dAngB + dInter*j; if (ang > PI*2) ang -= PI*2; }
				else { ang = dAngB - dInter*j; if (ang < 0.0) ang += PI*2; }
				ads_polar(Center,ang,dRadius,pt);
				pt3d.x=pt[X]; pt3d.y=pt[Y]; pt3d.z = -2999.0;
				pt3dArr.append(pt3d);
				distArr.append(dTotalDist+dRadius*dInter*j);
			}
		}
		else 
		{
			dTotalDist += distOF3d(ptB,ptE);
		}
	}
}

bool CJoinTriangles::threeD_OneGcdHouse(ads_name entHouse,AcGePoint3dArray &pt3dArr)
{
	ads_name ssName,ent;
	long l,Length=0;
	AcGePoint2dArray Nodes;
	TCHAR szEntity[50],szCode[50];
	ads_point pt;
	struct resbuf *rc,*rb;
	double dHeight;
	int nNumber=0;

	if ( false == GetNodesHaveArc(entHouse,Nodes,15.0) ) return false;
	if ( false == ssFromNodes(Nodes,ssName,_T("cp")) ) return false;
	ads_sslength(ssName,&Length);
	for (l=0; l<Length; l++)
	{
		ads_ssname(ssName,l,ent);
		GetEntityType(ent,szEntity);
		if ( _tcsicmp(szEntity,_T("INSERT")) ) continue;
		GetStdm(ent,szCode);
		if ( _tcsicmp(szCode,_T("202101")) ) continue;
		rc = ads_entget(ent);
		rb=rc; while (rb->restype != 10) rb=rb->rbnext;
		ads_point_set(rb->resval.rpoint,pt);
		ads_relrb(rc);

		if ( 1 == ifEntInsidePoly(entHouse,pt) ) {
			nNumber++;
			dHeight = pt[Z];
			if ( nNumber >= 2 ) break;
		}
	}
	ads_ssfree(ssName);

	if ( 1 == nNumber )
	{//ֻ��һ���̵߳������
		AcGePoint3d pt3d;
		for (int i=0; i<Nodes.length(); i++) {
			pt3d.x=Nodes[i].x; pt3d.y=Nodes[i].y; pt3d.z=dHeight;
			pt3dArr.append(pt3d);
		}
		if ( distOF3d(pt3dArr.first(),pt3dArr.last()) > 1.0 ) pt3dArr.append(pt3dArr.first());
		return true;
	}
	return false;
}

//Ѱ�ҵ�����
void CJoinTriangles::threeD_SeekDxing()
{
	struct resbuf *rc;
	ads_name ssName,ent;
	AcDbObjectId objId;
	long l,Length;
	TCHAR szCode[50];

	rc = ads_buildlist(-4,_T("<or"),RTDXF0,_T("POLYLINE"),RTDXF0,_T("LWPOLYLINE"),-4,_T("or>"),0);
	ads_ssget(_T("X"),NULL,NULL,rc,ssName); 
	ads_relrb(rc);
	Length=0; ads_sslength(ssName,&Length);
	for (l=0; l<Length; l++) 
	{
		ads_ssname(ssName,l,ent);
		GetStdm(ent,szCode);
		if ( true == CheckDoukanCode(szCode) || !_tcscmp(szCode,_T("201199")) ) 
		{
			//��ȡ���ϸ߳̿��Ƶ�
			AcGePoint3dArray pt3dArr,Nodes;
			AcGeDoubleArray distArr,kzdDistArr;
			if ( threeD_GetGcdOnDxing(ent,Nodes,kzdDistArr) > 0 ) 
			{//����������һ�����Ƶ�
				//���߲��
				AcGeDoubleArray Bulges;
				GetPlBulges(ent,Bulges,true);
				
				threeD_InsertOnQuxian(Nodes,Bulges,pt3dArr,distArr);
				
				//���ϸ̲߳�ֵ
				threeD_InsertHeightOnDxing(pt3dArr,distArr);
				
				//�ѵ����ߴ�����
				m_DxingArr.Add(pt3dArr);
				m_nDxingPointNum += pt3dArr.length();
				
				//������ά�����DTM�̵߳�
				//threeD_AddDxing(pt3dArr,BeforeKan);
			}
			else if ( true == CheckDoukanCode(szCode) )
			{//�ϵĻ��ڿ��ߵĶ���
				acdbGetObjectId(objId,ent);
				m_OldKanArr.append(objId);
			}
		}
#ifdef CSKCSJY
		else if ( !_tcsnicmp(szCode,_T("141"),3) && GetPolyType(ent,true) >= 10 )
		{
			AcGePoint3dArray pt3dArr;
			if ( true == threeD_OneGcdHouse(ent,pt3dArr) ) {
				//�ѵ����ߴ�����
				m_DxingArr.Add(pt3dArr);
				m_nDxingPointNum += pt3dArr.length();
			}
		}
#endif
	}
	ads_ssfree(ssName);
}

void CJoinTriangles::Dgx_Kangao(int BeforeKan)
{
	int i;
	for (i=0; i<m_DxingArr.GetSize(); i++)
	{
		AcGePoint3dArray pt3dArr = m_DxingArr.GetAt(i);
		threeD_AddDxing(pt3dArr,BeforeKan);
	}
	m_DxingArr.RemoveAll();

	//�϶���
	for (i=0; i<m_OldKanArr.length(); i++)
	{
		ads_name ent;
		acdbGetAdsName(ent,m_OldKanArr[i]);
		old_OneKanGao(ent,BeforeKan);
	}
}

void CJoinTriangles::old_OneKanGao(ads_name ent,int BeforeKan)
{
	double dWidth=0.5;  //Width:�½����뿲���߾���
	struct resbuf *rc,*rb;
	ads_point pt,pt1,pt2;
	AcGePoint3d pt3d;
	AcGePoint3dArray Nodes;
	AcGeDoubleArray KanArray;
	AcGeIntArray UpKan,UnderKan;
	int Number;
	double d1040,dKangao,DefaultKan = g_dKanHigh;
	int nPolyType = GetPolyType(ent,true);
	if ( true == Get1040(ent,d1040) ) DefaultKan = d1040;

	//��ȡ�����꼰����
	if ( nPolyType%10 > 0 ) 
	{//������
		ads_name ent1,ent2;
		int int70;
		ads_entnext(ent,ent1); 
		while (1) 
		{
			rc = ads_entget(ent1); 
			rb=rc; while (rb->restype!=0) rb=rb->rbnext;
			if ( !_tcsicmp(rb->resval.rstring,_T("SEQEND")) ) { ads_relrb(rc); break; }
			rb=rc; while (rb->restype!=10) rb=rb->rbnext; 
			ads_point_set(rb->resval.rpoint,pt); pt[Z]=0.0;
			rb=rc; while (rb->restype!=70) rb=rb->rbnext; 
			int70=rb->resval.rint%16; 
			ads_relrb(rc);
				
			if ( (int70%2 == 0) && (int70 < 8) ) {
				if ( true == Get1040(ent1,d1040) ) dKangao = d1040;
				else dKangao = DefaultKan;
				pt3d[X]=pt[X]; pt3d[Y]=pt[Y]; pt3d[Z]=pt[Z]; 
				Nodes.append(pt3d);
				KanArray.append(dKangao);
			}
			ads_name_set(ent1,ent2); 
			ads_entnext(ent2,ent1); 
		}
	}
	else 
	{
		rc = ads_entget(ent);
		rb = rc;
		while (rb) {
			if (10 == rb->restype) {
				pt3d[X]=rb->resval.rpoint[X]; pt3d[Y]=rb->resval.rpoint[Y]; pt3d[Z]=0.0; 
				Nodes.append(pt3d);
				KanArray.append(DefaultKan);
			}
			rb = rb->rbnext;
		}
		ads_relrb(rc);
	}

	//��ȡ���߳�
	int i,nSum = Nodes.length();
	for (i=0; i<nSum; i++) 
	{
		pt3d = Nodes.at(i);
		Number = GetNumFromGcd(pt3d[X],pt3d[Y],BeforeKan);
		UpKan.append(Number);
		if (Number >= 0) { 
			pt3d[Z]=point[Number][Z]; 
			Nodes.setAt(i,pt3d); 
		}
	}

	//��ȡ����
	for (i=0; i<nSum; i++) 
	{
		if ( UpKan.at(i) < 0 ) { 
			UnderKan.append(-1); continue; 
		}
		pt3d=Nodes.at(i); pt[X]=pt3d[X]; pt[Y]=pt3d[Y]; pt[Z]=pt3d[Z];
		if (0 == i) pt3d = Nodes.at(nSum-1); 
		else pt3d = Nodes.at(i-1); 
		pt1[X]=pt3d[X]; pt1[Y]=pt3d[Y]; pt1[Z]=pt3d[Z];
		if (nSum-1 == i) pt3d = Nodes.at(0); 
		else pt3d = Nodes.at(i+1); 
		pt2[X]=pt3d[X]; pt2[Y]=pt3d[Y]; pt2[Z]=pt3d[Z];

		double Angle,Ang,Ang1,Ang2;
		if ( 0==i && nPolyType<10 ) {
			Angle = ads_angle(pt,pt2)+PI/2;
		}
		else if ( nSum-1==i && nPolyType<10 ) {
			Angle = ads_angle(pt,pt1)-PI/2;
		}
		else {
			Ang1 = ads_angle(pt,pt1); 
			Ang2 = ads_angle(pt,pt2); 
			Ang = Ang1 - Ang2;
			if (Ang < 0.0) Ang += PI*2; 
			else if (Ang > PI*2) Ang -= PI*2;
			Angle = Ang2 + Ang/2.0;
		}

		ads_polar(pt,Angle,dWidth,pt); 
		pt[Z] -= KanArray.at(i);
		Number = GetNumFromGcd(pt[X],pt[Y],m_LenXyz);
		if (Number >= 0) UnderKan.append(Number);
		else {
			m_LenXyz++; ads_point_set(pt,point[m_LenXyz]); UnderKan.append(m_LenXyz);
		}
	}
	if (nPolyType >= 10) { 
		UpKan.append(UpKan.at(0)); 
		UnderKan.append(UnderKan.at(0)); 
	}

	//������������
	for (i=0; i<UpKan.length()-1; i++) 
	{
		for (int j=0; j<2; j++) 
		{
			int Num1,Num2;
			if (0 == j) { 
				Num1=UpKan.at(i); Num2=UpKan.at(i+1); 
			}
			else if (1 == j) { 
				Num1=UnderKan.at(i); Num2=UnderKan.at(i+1); 
			}
			if ( Num1>=0 && Num2>=0 ) { 
				dxArray.append(Num1); dxArray.append(Num2); 
			}
		}
	}
}

/*
void CJoinTriangles::Dgx_Kangao(int BeforeKan)
{
	struct resbuf *rc,*rb;
	ads_name ssName,ssName1,ent;
	long ll,length;
	ads_point pt,pt1,pt2;
	TCHAR szStr[50];
	double dWidth=0.5;  //Width:�½����뿲���߾���

	rc = ads_buildlist(-4,_T("<or"),RTDXF0,_T("POLYLINE"),RTDXF0,_T("LWPOLYLINE"),-4,_T("or>"),0);
	ads_ssget(_T("X"),NULL,NULL,rc,ssName1); 
	ads_relrb(rc);
	length=0; ads_sslength(ssName1,&length);
	ads_ssadd(NULL,NULL,ssName);
	for (ll=0; ll<length; ll++) {
		ads_ssname(ssName1,ll,ent);
		GetStdm(ent,szStr);
		if ( true == CheckDoukanCode(szStr) ) ads_ssadd(ent,ssName,ssName);
	}
	ads_ssfree(ssName1);
	
	length=0; ads_sslength(ssName,&length);
	for (ll = 0; ll < length; ll++)
	{
		ads_ssname(ssName,ll,ent); 
		int nPolyType = GetPolyType(ent,true);
		double d1040,dKangao,DefaultKan = g_dKanHigh;
		if ( true == Get1040(ent,d1040) ) DefaultKan = d1040;

		AcGePoint3d pt3d;
		AcGePoint3dArray Nodes;
		AcGeDoubleArray KanArray;
		AcGeIntArray UpKan,UnderKan;
		int Number;

		//��ȡ�����꼰����
		if ( nPolyType%10 > 0 ) 
		{
			ads_name ent1,ent2;
			int int70;
			ads_entnext(ent,ent1); 
			while (1) 
			{
				rc = ads_entget(ent1); 
				rb=rc; while (rb->restype!=0) rb=rb->rbnext;
				if ( !_tcsicmp(rb->resval.rstring,_T("SEQEND")) ) {ads_relrb(rc);break;}
				rb=rc; while (rb->restype!=10) rb=rb->rbnext; 
				ads_point_set(rb->resval.rpoint,pt); pt[Z]=0.0;
				rb=rc; while (rb->restype!=70) rb=rb->rbnext; 
				int70=rb->resval.rint%16; 
				ads_relrb(rc);
				
				if ( (int70%2 == 0) && (int70 < 8) ) {
					if ( true == Get1040(ent1,d1040) ) dKangao = d1040;
					else dKangao = DefaultKan;
					pt3d[X]=pt[X]; pt3d[Y]=pt[Y]; pt3d[Z]=pt[Z]; 
					Nodes.append(pt3d);
					KanArray.append(dKangao);
				}
				ads_name_set(ent1,ent2); 
				ads_entnext(ent2,ent1); 
			}
		}
		else {
			rc = ads_entget(ent);
			rb = rc;
			while (rb) {
				if (10 == rb->restype) {
					pt3d[X]=rb->resval.rpoint[X]; pt3d[Y]=rb->resval.rpoint[Y]; pt3d[Z]=0.0; 
					Nodes.append(pt3d);
					KanArray.append(DefaultKan);
				}
				rb = rb->rbnext;
			}
			ads_relrb(rc);
		}

		//��ȡ���߳�
		int i,nSum = Nodes.length();
		for (i=0; i<nSum; i++) 
		{
			pt3d = Nodes.at(i);
			Number = GetNumFromGcd(pt3d[X],pt3d[Y],BeforeKan);
			UpKan.append(Number);
			if (Number >= 0) { 
				pt3d[Z]=point[Number][Z]; 
				Nodes.setAt(i,pt3d); 
			}
		}

		//��ȡ����
		for (i=0; i<nSum; i++) 
		{
			if ( UpKan.at(i) < 0 ) { 
				UnderKan.append(-1); continue; 
			}
			pt3d=Nodes.at(i); pt[X]=pt3d[X]; pt[Y]=pt3d[Y]; pt[Z]=pt3d[Z];
			if (0 == i) pt3d = Nodes.at(nSum-1); 
			else pt3d = Nodes.at(i-1); 
			pt1[X]=pt3d[X]; pt1[Y]=pt3d[Y]; pt1[Z]=pt3d[Z];
			if (nSum-1 == i) pt3d = Nodes.at(0); 
			else pt3d = Nodes.at(i+1); 
			pt2[X]=pt3d[X]; pt2[Y]=pt3d[Y]; pt2[Z]=pt3d[Z];

			double Angle,Ang,Ang1,Ang2;
			if ( 0==i && nPolyType<10 ) 
				Angle = ads_angle(pt,pt2)+PI/2;
			else if ( nSum-1==i && nPolyType<10 ) 
				Angle = ads_angle(pt,pt1)-PI/2;
			else {
				Ang1 = ads_angle(pt,pt1); 
				Ang2 = ads_angle(pt,pt2); 
				Ang = Ang1 - Ang2;
				if (Ang < 0.0) Ang += PI*2; 
				else if (Ang > PI*2) Ang -= PI*2;
				Angle = Ang2 + Ang/2.0;
			}

			ads_polar(pt,Angle,dWidth,pt); 
			pt[Z] -= KanArray.at(i);
			Number = GetNumFromGcd(pt[X],pt[Y],m_LenXyz);
			if (Number >= 0) UnderKan.append(Number);
			else {
				m_LenXyz++; ads_point_set(pt,point[m_LenXyz]); UnderKan.append(m_LenXyz);
			}
		}
		if (nPolyType >= 10) { 
			UpKan.append(UpKan.at(0)); 
			UnderKan.append(UnderKan.at(0)); 
		}

		//������������
		for (i=0; i<UpKan.length()-1; i++) 
		{
			for (int j=0; j<2; j++) 
			{
				int Num1,Num2;
				if (0 == j) { 
					Num1=UpKan.at(i); Num2=UpKan.at(i+1); 
				}
				else if (1 == j) { 
					Num1=UnderKan.at(i); Num2=UnderKan.at(i+1); 
				}
				if ( Num1>=0 && Num2>=0 ) { 
					dxArray.append(Num1); dxArray.append(Num2); 
				}
			}
		}
	}
	ads_ssfree(ssName);
}
*/

//������������д���ļ�
void CJoinTriangles::WriteSjwFile(FILE *out,int llb1,int llb2,int llb3)
{
	_ftprintf(out,_T("%.3f\n%.3f\n%.3f\n"),point[llb1][X],point[llb1][Y],point[llb1][Z]);
	_ftprintf(out,_T("%.3f\n%.3f\n%.3f\n"),point[llb2][X],point[llb2][Y],point[llb2][Z]);
	_ftprintf(out,_T("%.3f\n%.3f\n%.3f\n"),point[llb3][X],point[llb3][Y],point[llb3][Z]);
}

void CJoinTriangles::Drawsjx(ads_point pt1,ads_point pt2,ads_point pt3)
{
	if ( 3 == m_DrawType ) return;
	if ( 2 == m_DrawType ) {
		ads_name ent;
		if ( true == m_bSjwPick ) {
			DrawOneSjx(pt1,pt2,pt3);
			ads_entlast(ent); 
		}
		else {
#if _MSC_VER > 1200
			ads_command(RTSTR,_T("3dpoly"),RT3DPOINT,pt1,RT3DPOINT,pt2,RT3DPOINT,pt3,RTSTR,_T(""),0);
			ads_entlast(ent); 
			ads_command(RTSTR,_T("pedit"),RTENAME,ent,RTSTR,_T("C"),RTSTR,_T(""),0);
#else
			ads_command(RTSTR,_T("3dpoly"),RT3DPOINT,pt1,RT3DPOINT,pt2,RT3DPOINT,pt3,RTSTR,_T("C"),0);
			ads_entlast(ent); 
#endif // _MSC_VER
		}
		if ( m_nColor > 0 ) setcolor(ent,m_nColor);
		ads_ssadd(ent,m_ssJiaName,m_ssJiaName);
	}
	else {
		DrawOneSjx(pt1,pt2,pt3);
	}
}

bool CJoinTriangles::KanInter(int Number1,int Number2,ads_point *pointEx)
{//����number1��number2�������Ƿ��DouKan�еĵ������ཻ
	if ( m_Kanshu<=0 || NULL==Doukan ) return false;
	int num1,num2;
	ads_point *pp;
	if ( NULL == pointEx ) pp = point; 
	else pp = pointEx;
	int i;
	for (i=0; i<m_Kanshu-1; i=i+2) {
		num1 = Doukan[i]; num2 = Doukan[i+1]; 
		if ( ( (Number1==num1) && (Number2==num2) ) || ( (Number1==num2) && (Number2==num1) ) ) //������ĳ�������غ�
			return false;
	}

	ads_point from1,to1,from2,to2,result;
	double minX1,maxX1,minY1,maxY1,minX2,maxX2,minY2,maxY2;
	from1[X]=pp[Number1][X]; from1[Y]=pp[Number1][Y]; from1[Z]=0.0;
	to1[X]=pp[Number2][X]; to1[Y]=pp[Number2][Y]; to1[Z]=0.0;
	if (from1[X] > to1[X]) { minX1=to1[X]; maxX1=from1[X]; } 
	else { minX1=from1[X]; maxX1=to1[X]; }
	if (from1[Y] > to1[Y]) { minY1=to1[Y]; maxY1=from1[Y]; } 
	else { minY1=from1[Y]; maxY1=to1[Y]; }

	for (i=0; i<m_Kanshu-1; i=i+2) {
		num1 = Doukan[i]; num2 = Doukan[i+1]; 
		if ( (Number1==num1) || (Number1==num2) || (Number2==num1) || (Number2==num2) ) continue;
		from2[X]=pp[num1][X]; from2[Y]=pp[num1][Y]; from2[Z]=0.0;
		to2[X]=pp[num2][X]; to2[Y]=pp[num2][Y]; to2[Z]=0.0;
		if (from2[X] > to2[X]) { minX2=to2[X]; maxX2=from2[X]; } 
		else { minX2=from2[X]; maxX2=to2[X]; }
		if (from2[Y] > to2[Y]) { minY2=to2[Y]; maxY2=from2[Y]; } 
		else { minY2=from2[Y]; maxY2=to2[Y]; }
		if ( !( minX1>=maxX2 || maxX1<=minX2 || minY1>=maxY2 || maxY1<=minY2 ) ) {
			if (ads_inters(from1,to1,from2,to2,1,result)==RTNORM) return true;
		}
	}
	return false;
}

bool CJoinTriangles::SjxInter(int Cur,int n1,int n2,int n3,ads_point *pp)
{//�緵��1��ʾ���������н���
	//�鿴�Ƿ������������غϱ�
	int SideNum1=0,SideNum2=0;
	//һ�������������������ͨ��
	int i;
	for (i=0; i<=Cur-1; i++) 
	{ 
		if ( ((n1==llb1[i])||(n1==llb2[i])||(n1==llb3[i])) && ((n3==llb1[i])||(n3==llb2[i])||(n3==llb3[i])) ) SideNum1++;
		if ( ((n2==llb1[i])||(n2==llb2[i])||(n2==llb3[i])) && ((n3==llb1[i])||(n3==llb2[i])||(n3==llb3[i])) ) SideNum2++;
		if ( SideNum1>=2 || SideNum2>=2 )  return true;
	}
	if ( m_MinAngle<0.0001 && m_LenXyz>15000 ) return false; //������С��Ӧ�ò�����ֽ���

	double Angle,Angle1,Angle2,Angle3,Angle4;
	Angle1=ads_angle(pp[n1],pp[n3]); Angle2=ads_angle(pp[n2],pp[n3]);
	Angle3=ads_angle(pp[n3],pp[n1]); Angle4=ads_angle(pp[n3],pp[n2]);
	
	//һ�������������������ͨ��
	for (i=0; i<=Cur-1; i++) 
	{ 
		int pa1,pa2,pb1,pb2,pc1,pc2;
		bool HaveA=false,HaveB=false,HaveC=false;
		double Ang,Ang1,Ang2;

		if ( llb1[i] == n1 ) { pa1=llb2[i]; pa2=llb3[i]; HaveA=true; }
		else if ( llb2[i] == n1 ) { pa1=llb1[i]; pa2=llb3[i]; HaveA=true; }
		else if ( llb3[i] == n1 ) { pa1=llb1[i]; pa2=llb2[i]; HaveA=true; }

		if ( llb1[i] == n2 ) { pb1=llb2[i]; pb2=llb3[i]; HaveB=true; }
		else if ( llb2[i] == n2 ) { pb1=llb1[i]; pb2=llb3[i]; HaveB=true; }
		else if ( llb3[i] == n2 ) { pb1=llb1[i]; pb2=llb2[i]; HaveB=true; }

		if (HaveA==true && HaveB==true) continue;
		//HaveC��ʾ����ҵ�n3�غ�
		if ( llb1[i] == n3 ) { pc1=llb2[i]; pc2=llb3[i]; HaveC=true; }
		else if ( llb2[i] == n3 ) { pc1=llb1[i]; pc2=llb3[i]; HaveC=true; }
		else if ( llb3[i] == n3 ) { pc1=llb1[i]; pc2=llb2[i]; HaveC=true; }

		for (int j=0; j<4; j++) {
			if (j==0) {
				if ( HaveA==false || pa1==n3 || pa2==n3 ) continue;
				Angle = Angle1; Ang1 = ads_angle(pp[n1],pp[pa1]); Ang2 = ads_angle(pp[n1],pp[pa2]);
			}
			else if (j==1) {
				if ( HaveB==false || pb1==n3 || pb2==n3 ) continue;
				Angle = Angle2; Ang1 = ads_angle(pp[n2],pp[pb1]); Ang2 = ads_angle(pp[n2],pp[pb2]);
			}
			else if (j==2) {
				if ( HaveC==false || HaveA==true ) continue;
				Angle = Angle3; Ang1 = ads_angle(pp[n3],pp[pc1]); Ang2 = ads_angle(pp[n3],pp[pc2]);
			}
			else if (j==3) {
				if ( HaveC==false || HaveB==true ) continue;
				Angle = Angle4; Ang1 = ads_angle(pp[n3],pp[pc1]); Ang2 = ads_angle(pp[n3],pp[pc2]);
			}
			if (Ang1 > Ang2) { Ang=Ang1; Ang1=Ang2; Ang2=Ang; }
			if ( (Ang2-Ang1) > PI ) { 
				if ( Angle<=Ang1 || Angle>=Ang2 ) return true;
			}
			else {
				if ( Angle<=Ang2 && Angle>=Ang1 ) return true;
			}
		}
	}
	return false;
}

//�����ε����������������ķ���
short CJoinTriangles::WhichSide(ads_point pt1,ads_point pt2,ads_point pt3)
{
	short Flag=0;
	double aa,bb,cc;

	if (pt2[X] == pt1[X]) {
		if (pt3[X] < pt1[X]) Flag=-1;
		else if (pt3[X] > pt1[X]) Flag=1;
	}
	else if (pt2[Y] == pt1[Y]) {
			if (pt3[Y] < pt1[Y]) Flag=-1;
			else if (pt3[Y] > pt1[Y]) Flag=1;
	}
	else if ( pt1[X] > 55555555 || pt1[Y] > 55555555 || pt2[X] > 55555555 || pt2[Y] > 55555555 ) {
		aa = ads_angle(pt3,pt1);
		bb = ads_angle(pt3,pt2);
		cc = aa-bb;
		if ( cc < 0.0 ) cc += PI*2;
		if ( cc < PI ) Flag = 1;
		else if ( cc > PI ) Flag = -1;
	}
	else {
		aa = (pt2[Y]-pt1[Y]) / (pt2[X]-pt1[X]);
		bb = (pt1[Y]*pt2[X]-pt2[Y]*pt1[X]) / (pt2[X]-pt1[X]);
		cc = pt3[Y]-aa*pt3[X]-bb;
	 	if (cc<0) Flag=-1; else if (cc>0) Flag=1;
	}
	return Flag;
}

int CJoinTriangles::seeksjx(int n1,int n2,int n3,int j,ads_point *pointEx)
{
	short FlagSjx=-1,Flag=0;
	double AllTT=-1.0,MagTT=-1.0;
	int i,Sides=0,AllLLB=-1,MagLLB=-1;
	bool Margin = true;//�Ƿ�Ϊ��Ե������
	ads_point *pp;

	//Code1(25);
	if ( NULL == pointEx ) pp = point; else pp = pointEx;
	FlagSjx = WhichSide(pp[n1],pp[n2],pp[n3]);
	
	//Ŀ��߲���ͨ����������������
	for (i=0; i<=j-1; i++) { 
	   	if ( ((n1==llb1[i])||(n1==llb2[i])||(n1==llb3[i])) && ((n2==llb1[i])||(n2==llb2[i])||(n2==llb3[i])) ) Sides++;
	}
	if ( Sides >= 2 )  return 0;

	//����������Ѱ�����������
	double aa,bb,cc,disA,disB,Cosin,Obtuse,Acute;
	cc=(pp[n2][X]-pp[n1][X])*(pp[n2][X]-pp[n1][X])+(pp[n2][Y]-pp[n1][Y])*(pp[n2][Y]-pp[n1][Y]);
	Acute = cos(m_MinAngle/180.0*PI); 
	Obtuse = cos((180-2*m_MinAngle)/180.0*PI);

 	for (i=0; i<=m_LenXyz; i++) 
	{
		if ( (i==n1) || (i==n2) || (i==n3) ) continue;
		Flag = WhichSide(pp[n1],pp[n2],pp[i]);
		if (FlagSjx == Flag) continue; //i��n3��һ��
		 	
		aa=(pp[i][X]-pp[n1][X])*(pp[i][X]-pp[n1][X])+(pp[i][Y]-pp[n1][Y])*(pp[i][Y]-pp[n1][Y]);
		bb=(pp[i][X]-pp[n2][X])*(pp[i][X]-pp[n2][X])+(pp[i][Y]-pp[n2][Y])*(pp[i][Y]-pp[n2][Y]);
		if ( aa<0.000001 || bb<0.000001 ) continue;
		disA=sqrt(aa); disB=sqrt(bb);
	 	Cosin = 1 - (aa+bb-cc) / (2*disA*disB);
		if (Margin == true ) {
			if ( ( (1-Cosin) < Acute ) && ( (1-Cosin) > Obtuse ) ) Margin = false;
		}

		if ( ( (1-Cosin) < 1.0 ) && ( (1-Cosin) > -1.0 ) ) {
			if ( Cosin <= MagTT ) continue;
			else if ( Cosin<=AllTT && ( (1-Cosin)>=Acute ) ) continue; //�Ƕ�С����ֵʱ

			if ( m_Kanshu > 0 ) {
				if ( KanInter(i,n1,pointEx)==true ) continue; 
				else if ( KanInter(i,n2,pointEx)==true ) continue;
			}
			if ( Cosin > AllTT ) { AllTT=Cosin; AllLLB=i; }
			if ( ( (1-Cosin) < Acute ) && ( (1-Cosin) > Obtuse ) && ( Cosin > MagTT ) ) { MagTT=Cosin; MagLLB=i; }
		}
    }

	if ( m_MinAngle<0.0 || m_MinAngle>90.0 ) return 0; //�Ҳ�����
	else if ( Margin == true ) return 0;
	else if ( MagLLB>=0 && SjxInter(j,n1,n2,MagLLB,pp)==false ) llb3[j] = MagLLB;
	else if ( AllLLB>=0 && AllLLB!=MagLLB ) {
		if ( SjxInter(j,n1,n2,AllLLB,pp)==false ) llb3[j] = AllLLB; else return 0;
	}
	else return 0;
	Drawsjx(pp[llb1[j]],pp[llb2[j]],pp[llb3[j]]);
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�ظ���Ϊ�˼���,2004.04.12

/*
int CJoinTriangles::seeksjx1(int n1,int j,int n2,int n3,ads_point *pointEx)
{//Ϊ�˼��ܵ�Ŀ�ĸ��ƺ���seeksjx,2004.04.12
	short FlagSjx=1,Flag=0;
	double AllTT=-1.0,MagTT=-1.0;
	int i,Sides=0,AllLLB=-1,MagLLB=-1;
	bool Margin = true;//�Ƿ�Ϊ��Ե������
	ads_point *pp;

	Code1(25);
	if ( NULL == pointEx ) pp = point; else pp = pointEx;
	//Ŀ��߲���ͨ����������������
	for (i=0; i<=j-1; i++) { 
	   	if ( ((n1==llb1[i])||(n1==llb2[i])||(n1==llb3[i])) && ((n2==llb1[i])||(n2==llb2[i])||(n2==llb3[i])) ) Sides++;
	}
	if ( Sides >= 2 )  return 0;

	BYTE szIn[100],szOut[255],*pb;
	DWORD returnlen;
	pb=szIn+0; memcpy(pb,&pp[n1][X],sizeof(double));
	pb=szIn+8; memcpy(pb,&pp[n1][Y],sizeof(double));
	pb=szIn+16; memcpy(pb,&pp[n2][X],sizeof(double));
	pb=szIn+24; memcpy(pb,&pp[n2][Y],sizeof(double));
	pb=szIn+32; memcpy(pb,&pp[n3][X],sizeof(double));
	pb=szIn+40; memcpy(pb,&pp[n3][Y],sizeof(double));
	if ( S4_SUCCESS == S4Execute(&s4_context,"1011",szIn,48,szOut,255,&returnlen) ) 
	{ //WhichSide
		memcpy(&FlagSjx,szOut,sizeof(short));
		S4_ConvertSignShort(&FlagSjx);
	}
 	
	//����������Ѱ�����������
	double aa,bb,cc,disA,disB,Cosin,Obtuse,Acute;
	cc=(pp[n2][X]-pp[n1][X])*(pp[n2][X]-pp[n1][X])+(pp[n2][Y]-pp[n1][Y])*(pp[n2][Y]-pp[n1][Y]);
	Acute = cos(m_MinAngle/180.0*PI); 
	Obtuse = cos((180-2*m_MinAngle)/180.0*PI);

 	for (i=0; i<=m_LenXyz; i++) 
	{
		if ( (i==n1) || (i==n2) || (i==n3) ) continue;
		Flag = WhichSide(pp[n1],pp[n2],pp[i]);
		if (FlagSjx == Flag) continue; //i��n3��һ��
		 	
		aa=(pp[i][X]-pp[n1][X])*(pp[i][X]-pp[n1][X])+(pp[i][Y]-pp[n1][Y])*(pp[i][Y]-pp[n1][Y]);
		bb=(pp[i][X]-pp[n2][X])*(pp[i][X]-pp[n2][X])+(pp[i][Y]-pp[n2][Y])*(pp[i][Y]-pp[n2][Y]);
		if ( aa<0.000001 || bb<0.000001 ) continue;
		disA=sqrt(aa); disB=sqrt(bb);
	 	Cosin = 1 - (aa+bb-cc) / (2*disA*disB);
		if (Margin == true ) {
			if ( ( (1-Cosin) < Acute ) && ( (1-Cosin) > Obtuse ) ) Margin = false;
		}

		if ( ( (1-Cosin) < 1.0 ) && ( (1-Cosin) > -1.0 ) ) {
			if ( Cosin <= MagTT ) continue;
			else if ( Cosin<=AllTT && ( (1-Cosin)>=Acute ) ) continue; //�Ƕ�С����ֵʱ

			if ( m_Kanshu > 0 ) {
				if ( KanInter(i,n1,pointEx)==true ) continue; 
				else if ( KanInter(i,n2,pointEx)==true ) continue;
			}
			if ( Cosin > AllTT ) { AllTT=Cosin; AllLLB=i; }
			if ( ( (1-Cosin) < Acute ) && ( (1-Cosin) > Obtuse ) && ( Cosin > MagTT ) ) { MagTT=Cosin; MagLLB=i; }
		}
    }

	if ( m_MinAngle<0.0 || m_MinAngle>90.0 ) return 0; //�Ҳ�����
	else if ( Margin == true ) return 0;
	else if ( MagLLB>=0 && SjxInter1(n1,n2,MagLLB,j,pp)==false ) llb3[j] = MagLLB;
	else if ( AllLLB>=0 && AllLLB!=MagLLB ) {
		if ( SjxInter1(n1,n2,AllLLB,j,pp)==false ) llb3[j] = AllLLB; else return 0;
	}
	else return 0;
	Drawsjx(pp[llb1[j]],pp[llb2[j]],pp[llb3[j]]);
	return 1;
}
*/

bool CJoinTriangles::SjxInter1(int n1,int n2,int n3,int Cur,ads_point *pp)
{//�緵��1��ʾ���������н���,����,2004.04.12
	//�鿴�Ƿ������������غϱ�
	int SideNum1=0,SideNum2=0;
	//һ�������������������ͨ��
	int i;
	for (i=0; i<=Cur-1; i++) 
	{ 
		if ( ((n1==llb1[i])||(n1==llb2[i])||(n1==llb3[i])) && ((n3==llb1[i])||(n3==llb2[i])||(n3==llb3[i])) ) SideNum1++;
		if ( ((n2==llb1[i])||(n2==llb2[i])||(n2==llb3[i])) && ((n3==llb1[i])||(n3==llb2[i])||(n3==llb3[i])) ) SideNum2++;
		if ( SideNum1>=2 || SideNum2>=2 )  return true;
	}
	if ( m_MinAngle<0.0001 && m_LenXyz>15000 ) return false; //������С��Ӧ�ò�����ֽ���

	double Angle,Angle1,Angle2,Angle3,Angle4;
	Angle1=ads_angle(pp[n1],pp[n3]); Angle2=ads_angle(pp[n2],pp[n3]);
	Angle3=ads_angle(pp[n3],pp[n1]); Angle4=ads_angle(pp[n3],pp[n2]);
	
	//һ�������������������ͨ��
	int nSum=0;
	for (i=0; i<=Cur-1; i++) 
	{ 
		int pa1,pa2,pb1,pb2,pc1,pc2;
		bool HaveA=false,HaveB=false,HaveC=false;
		double Ang,Ang1,Ang2;

		if ( llb1[i] == n1 ) { pa1=llb2[i]; pa2=llb3[i]; HaveA=true; }
		else if ( llb2[i] == n1 ) { pa1=llb1[i]; pa2=llb3[i]; HaveA=true; }
		else if ( llb3[i] == n1 ) { pa1=llb1[i]; pa2=llb2[i]; HaveA=true; }

		if ( llb1[i] == n2 ) { pb1=llb2[i]; pb2=llb3[i]; HaveB=true; }
		else if ( llb2[i] == n2 ) { pb1=llb1[i]; pb2=llb3[i]; HaveB=true; }
		else if ( llb3[i] == n2 ) { pb1=llb1[i]; pb2=llb2[i]; HaveB=true; }

		if (HaveA==true && HaveB==true) continue;
		//HaveC��ʾ����ҵ�n3�غ�
		if ( llb1[i] == n3 ) { pc1=llb2[i]; pc2=llb3[i]; HaveC=true; }
		else if ( llb2[i] == n3 ) { pc1=llb1[i]; pc2=llb3[i]; HaveC=true; }
		else if ( llb3[i] == n3 ) { pc1=llb1[i]; pc2=llb2[i]; HaveC=true; }

		for (int j=0; j<4; j++) {
			if (j==0) {
				if ( HaveA==false || pa1==n3 || pa2==n3 ) continue;
				Angle = Angle1; Ang1 = ads_angle(pp[n1],pp[pa1]); Ang2 = ads_angle(pp[n1],pp[pa2]);
			}
			else if (j==1) {
				if ( HaveB==false || pb1==n3 || pb2==n3 ) continue;
				Angle = Angle2; Ang1 = ads_angle(pp[n2],pp[pb1]); Ang2 = ads_angle(pp[n2],pp[pb2]);
			}
			else if (j==2) {
				if ( HaveC==false || HaveA==true ) continue;
				Angle = Angle3; Ang1 = ads_angle(pp[n3],pp[pc1]); Ang2 = ads_angle(pp[n3],pp[pc2]);
			}
			else if (j==3) {
				if ( HaveC==false || HaveB==true ) continue;
				Angle = Angle4; Ang1 = ads_angle(pp[n3],pp[pc1]); Ang2 = ads_angle(pp[n3],pp[pc2]);
			}
			if (Ang1 > Ang2) { Ang=Ang1; Ang1=Ang2; Ang2=Ang; }
			if ( (Ang2-Ang1) > PI ) { 
				if ( Angle<=Ang1 || Angle>=Ang2 ) {nSum++; break;}//return true; //dog
			}
			else {
				if ( Angle<=Ang2 && Angle>=Ang1 ) {nSum++; break;}//return true; //dog
			}
		}
	}
	if ( 1 == nSum ) return true; //dog,�����������,���ֻཻͬһ��������,
								  //�ڲ����������,���ཻһ�����һ��������
	return false;
}

void CJoinTriangles::TheThirdSanjiao(int n1,int n2,int n3,bool IfNum,int &j,FILE *out)
{
	short FlagSjx=1,Flag=0;
	double AllTT=-1.0,MagTT=-1.0;
	int i,Sides=0,AllLLB=-1,MagLLB=-1;
	bool Margin = true;//�Ƿ�Ϊ��Ե������
	ads_point *pp;
	double aa,bb,cc,disA,disB,Cosin,Obtuse,Acute,dTemp;

	//Ŀ��߲���ͨ����������������
	for (i=0; i<=j; i++) { 
	   	if ( ((n1==llb1[i])||(n1==llb2[i])||(n1==llb3[i])) && ((n2==llb1[i])||(n2==llb2[i])||(n2==llb3[i])) ) Sides++;
	}
	if ( Sides >= 2 )  return;

// #ifndef NOSENSE
// 	BYTE szIn[100],szOut[255],*pb;
// 	DWORD returnlen,nTemp;
// 	//j++; 
// 	dTemp = PI;
// 	pb=szIn+0; memcpy(pb,&dTemp,sizeof(double));
// 	nTemp = (DWORD)j; S4_ConvertLong(&nTemp);
// 	pb=szIn+8; memcpy(pb,&nTemp,sizeof(DWORD));
// 
// 	if ( S4_SUCCESS == S4Execute(&s4_context,"1087",szIn,12,szOut,255,&returnlen) ) 
// 	{ 
// 		pb=szOut; memcpy(&dTemp,pb,sizeof(double));
// 		pb=szOut+8; memcpy(&nTemp,pb,sizeof(DWORD));
// 		S4_ConvertLong(&nTemp);
// 		j = (int)nTemp;
// 	}
// #else
	j++;
	dTemp = PI / 180.0;
//#endif

	llb1[j]=n1; llb2[j]=n2; llb3[j]=-1;
	pp = point; 

	//����������Ѱ�����������
	cc=(pp[n2][X]-pp[n1][X])*(pp[n2][X]-pp[n1][X])+(pp[n2][Y]-pp[n1][Y])*(pp[n2][Y]-pp[n1][Y]);
	Acute = cos(m_MinAngle/180.0*PI); 
	Obtuse = cos((180-2*m_MinAngle)/180.0*PI);

// #ifndef NOSENSE
// 	pb=szIn+0; memcpy(pb,&dTemp,sizeof(double));
// 	pb=szIn+8; memcpy(pb,&m_MinAngle,sizeof(double));
// 	pb=szIn+16; memcpy(pb,&pp[n1][X],sizeof(double));
// 	pb=szIn+24; memcpy(pb,&pp[n1][Y],sizeof(double));
// 	pb=szIn+32; memcpy(pb,&pp[n2][X],sizeof(double));
// 	pb=szIn+40; memcpy(pb,&pp[n2][Y],sizeof(double));
// 	pb=szIn+48; memcpy(pb,&pp[n3][X],sizeof(double));
// 	pb=szIn+56; memcpy(pb,&pp[n3][Y],sizeof(double));
// 	if ( S4_SUCCESS == S4Execute(&s4_context,"1088",szIn,64,szOut,255,&returnlen) ) 
// 	{ //WhichSide
// 		pb=szOut+0; memcpy(&cc,pb,sizeof(double));
// 		pb=szOut+8; memcpy(&Acute,pb,sizeof(double));
// 		pb=szOut+16; memcpy(&Obtuse,pb,sizeof(double));
// 		pb=szOut+24; memcpy(&FlagSjx,pb,sizeof(short));
// 		S4_ConvertSignShort(&FlagSjx);
// 	}
// #else
	if ( fabs(pp[n2][X]-pp[n1][X]) < 0.00001 ) {
		if ( pp[n3][X] < pp[n1][X] ) FlagSjx=-1;
		else if ( pp[n3][X] > pp[n1][X] ) FlagSjx=1;
	}
	else if ( fabs(pp[n2][Y]-pp[n1][Y]) < 0.00001 ) {
		if ( pp[n3][Y] < pp[n1][Y] ) FlagSjx=-1;
		else if ( pp[n3][Y] > pp[n1][Y] ) FlagSjx=1;
	}
	else {
		double aa = (pp[n2][Y]-pp[n1][Y]) / (pp[n2][X]-pp[n1][X]);
		double bb = (pp[n1][Y]*pp[n2][X]-pp[n2][Y]*pp[n1][X]) / (pp[n2][X]-pp[n1][X]);
		double dTemp = pp[n3][Y]-aa*pp[n3][X]-bb;
		if (dTemp<0) FlagSjx=-1; else if (dTemp>0) FlagSjx=1;
	}

	cc=(pp[n2][X]-pp[n1][X])*(pp[n2][X]-pp[n1][X])+(pp[n2][Y]-pp[n1][Y])*(pp[n2][Y]-pp[n1][Y]);
	Acute = cos(m_MinAngle/180.0*PI); 
	Obtuse = cos((180-2*m_MinAngle)/180.0*PI);
//#endif
 	
 	for (i=0; i<=m_LenXyz; i++) 
	{
		if ( (i==n1) || (i==n2) || (i==n3) ) continue;
		Flag = WhichSide(pp[n1],pp[n2],pp[i]);
		if (FlagSjx == Flag) continue; //i��n3��һ��
		 	
		aa=(pp[i][X]-pp[n1][X])*(pp[i][X]-pp[n1][X])+(pp[i][Y]-pp[n1][Y])*(pp[i][Y]-pp[n1][Y]);
		bb=(pp[i][X]-pp[n2][X])*(pp[i][X]-pp[n2][X])+(pp[i][Y]-pp[n2][Y])*(pp[i][Y]-pp[n2][Y]);
		if ( aa<0.000001 || bb<0.000001 ) continue;
		disA=sqrt(aa); disB=sqrt(bb);
	 	Cosin = 1 - (aa+bb-cc) / (2*disA*disB);
		if (Margin == true ) {
			if ( ( (1-Cosin) < Acute ) && ( (1-Cosin) > Obtuse ) ) Margin = false;
		}

		if ( ( (1-Cosin) < 1.0 ) && ( (1-Cosin) > -1.0 ) ) {
			if ( Cosin <= MagTT ) continue;
			else if ( Cosin<=AllTT && ( (1-Cosin)>=Acute ) ) continue; //�Ƕ�С����ֵʱ

			if ( m_Kanshu > 0 ) {
				if ( KanInter(i,n1)==true ) continue; 
				else if ( KanInter(i,n2)==true ) continue;
			}
			if ( Cosin > AllTT ) { AllTT=Cosin; AllLLB=i; }
			if ( ( (1-Cosin) < Acute ) && ( (1-Cosin) > Obtuse ) && ( Cosin > MagTT ) ) { MagTT=Cosin; MagLLB=i; }
		}
    }

	if ( m_MinAngle<0.0 || m_MinAngle>90.0 ) { j--; return; } //�Ҳ�����
	else if ( Margin == true ) { j--; return; }
	else if ( MagLLB>=0 && SjxInter1(n1,n2,MagLLB,j,pp)==false ) llb3[j] = MagLLB;
	else if ( AllLLB>=0 && AllLLB!=MagLLB ) {
		if ( SjxInter1(n1,n2,AllLLB,j,pp)==false ) llb3[j] = AllLLB; 
		else { j--; return; }
	}
	else { j--; return; }
	Drawsjx(pp[llb1[j]],pp[llb2[j]],pp[llb3[j]]);
	
	if ( true == IfNum ) WriteSjwFile(out,llb1[j],llb2[j],llb3[j]);
	//if (1 == m_Type && 1 != m_DrawType) ads_printf(_T("��%d��������.\n"),j+1);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void CJoinTriangles::Join_Sjw(int BeginSjx,int ThiEdge,int TwoEdge)
{//(0<=���<=m_LenXyz)
	//Code1(26);
	if ( 1 == m_Type ) ads_printf(_T("\n������������,���Ժ�!\n"));
	TCHAR numFile[MAX_PATH],*ppp;
	FILE *out;
	bool IfNum = false;
	if ( 1 == m_Type && _tcscmp(m_datFile,_T("")) ) {
	    _tcscpy(numFile,m_datFile); 
		ppp=_tcsrchr(numFile,'.'); *ppp='\0'; 
		_tcscat(numFile,_T(".sjw"));
		out = _tfopen(numFile,_T("wt")); 
		if (out != NULL) IfNum = true;
	}

	if ( 1 == m_Type ) {
		if ( (g_Dtm >= 0.0) && (g_Dtm <= 30.0) ) m_MinAngle = g_Dtm;
	}
	//MinAngle += (DogResult-LinkConvert)%120;

	double dis,dist;
	int i,j,n1,n2,n3,knk;
	short nPlus; //����

	//û���Ѵ���������
	if (0 == BeginSjx) 
	{ 
		j = 0;
		llb1[j] = llb2[j] = llb3[j] = 0; 
		bool nFirst = true;
		for (i=1; i<=m_LenXyz; i++) 
		{
			dis = distOFpt(point[i],point[0]);
			if ( ( true==nFirst || dis<dist ) && (dis > TOLE) ) {
				if (m_Kanshu <= 0) {
					dist=dis; llb2[j]=i; nFirst=false;
				}
				else if ( false == KanInter(0,i) ) { 
					dist=dis; llb2[j]=i; nFirst=false;
				}
			}
		}
	
		nFirst = true;
		for ( i=0; i<=m_LenXyz; i++ ) 
		{
			if ( (llb2[j] == i) || (llb1[j] == i) ) continue;
			double dAngle = (ads_angle(point[i],point[llb1[j]]) - ads_angle(point[i],point[llb2[j]])) * 180.0/PI;
			if ( dAngle < 0.0 ) dAngle += 360.0; 
			if ( dAngle > 180.0 ) dAngle = 360.0 - dAngle;
			if ( dAngle<0.01 || dAngle>179.9 ) continue; //��ֹ�Ƕ�̫С

			double bb = distOFpt(point[i],point[llb1[j]]);
			if (bb < TOLE) continue;
			double aa = distOFpt(point[i],point[llb2[j]]);
			if (aa < TOLE) continue;
			dis = aa+bb;
			if ( true == nFirst || dis < dist ) {
				if (m_Kanshu <= 0) {
					dist=dis; llb3[j]=i; nFirst=false;
				}
				else if ( KanInter(i,llb1[j])==false && KanInter(i,llb2[j])==false ) { 
					dist=dis; llb3[j]=i; nFirst=false;
				}
			}
		}

		if ( true == IfNum ) 
			WriteSjwFile(out,llb1[j],llb2[j],llb3[j]);
 		Drawsjx(point[llb1[j]],point[llb2[j]],point[llb3[j]]);
		knk = 0; //knk�����ڴ�������������
	}
	else { //�޲���
		j = BeginSjx-1; 
		knk = ThiEdge; //knk�����ڴ�������������
	}
	
// #ifndef NOSENSE
// 	Sense_ConstShort(14,nPlus); //����
// #else
	nPlus = 1;
//#endif

	int PROGRESS = 998;
	if ( m_LenXyz >= PROGRESS )
	{
		if ( 1 == m_Type ) 
			acedSetStatusBarProgressMeter(_T("������������..."),0,m_LenXyz*2);
		else if ( 3 == m_Type ) 
			acedSetStatusBarProgressMeter(_T("���ڽ��е��η���..."),0,m_LenXyz*2);
	}
	do {
	 	n1=llb1[knk]; n2=llb3[knk]; n3=llb2[knk]; 
		j++; llb1[j]=n1; llb2[j]=n2; llb3[j]=-1;
		if ( seeksjx(n1,n2,n3,j) > 0 ) {
			if ( true == IfNum ) WriteSjwFile(out,llb1[j],llb2[j],llb3[j]);
			//if (1 == m_Type && 1 != m_DrawType) ads_printf(_T("��%d��������.\n"),j+1);
		}//extend2;
		else j--;

		n1=llb2[knk]; n2=llb3[knk]; n3=llb1[knk];
		j++; llb1[j]=n1; llb2[j]=n2; llb3[j]=-1;
		if ( seeksjx(n1,n2,n3,j) > 0 ) {
			if ( true == IfNum ) WriteSjwFile(out,llb1[j],llb2[j],llb3[j]);
			//if (1 == m_Type && 1 != m_DrawType) ads_printf(_T("��%d��������.\n"),j+1);
		}//extend2;
		else j--;
	 	
		//�Ե�һ�������������߶�Ҫ����
		if ( (0 == knk) || (knk>=ThiEdge && knk<TwoEdge) ) 
		{
			TheThirdSanjiao(llb1[knk],llb2[knk],llb3[knk],IfNum,j,out);
		}

		//knk++;
		knk += nPlus; //����
		//knk += labs(SecondConvert - LinkConvertEx) % 10; //����2003.4.1

		if (m_LenXyz >= PROGRESS) 
		{ //��ֹ������
			MSG message;
			while ( ::PeekMessage(&message,NULL,0,0,PM_REMOVE) ) {
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
			if ( 1 == m_Type ||  3 == m_Type ) {
				if ( acedSetStatusBarProgressMeterPos(j+1) < 0 ) 
				{
					if ( 1 == m_Type ) 
						acedSetStatusBarProgressMeter(_T("������������..."),0,m_LenXyz*2);
					else if ( 3 == m_Type ) 
						acedSetStatusBarProgressMeter(_T("���ڽ��е��η���..."),0,m_LenXyz*2);
					acedSetStatusBarProgressMeterPos(j+1);
				}
			}
		}
	} while(knk <= j);//һֱ����ͼ�����������ζ���һ��
	if ( (1==m_Type || 3==m_Type) && m_LenXyz >= PROGRESS ) acedRestoreStatusBar();

	if (true == IfNum) { 
		_ftprintf(out,_T("nil\n")); 
		fclose(out); 
	}
	m_LenSjx = j+1; //���ɵ������θ���
 	if ( 1 == m_Type ) ads_printf(_T("\n�����������!,�� %d ��������\n"),m_LenSjx);
}

void CJoinTriangles::EraseOutside()
{
	ads_name ssName,ent;
	AcGePoint2d pt2d;
	AcGePoint2dArray Nodes;
	long Length;

	for (int i=0; i<=m_LenXyz; i++) {
		pt2d[X]=point[i][X];
		pt2d[Y]=point[i][Y];
		Nodes.append(pt2d);
	}
	ssFromNodes(Nodes,ssName,_T("WP"));

	Length=0; ads_sslength(m_ssJiaName,&Length);
	for (long l=0; l<Length; l++) {
		ads_ssname(m_ssJiaName,l,ent);
		if ( ads_ssmemb(ent,ssName ) != RTNORM ) ads_entdel(ent);
	}
	ads_ssfree(ssName); 
}

void CJoinTriangles::start()
{
	//Code1(46);
//#ifdef CSKCSJY
	if ( true == m_bKan ) threeD_SeekDxing();
//#endif
	if ( false == ReadDatFile() ) return;
	
// #ifndef NOSENSE
// 	unsigned long nOut;
// 	if ( true == Sense_Long_Long("\\0001","1037",(unsigned long)m_LenXyz,nOut) ) m_LenXyz = (int)nOut; //��ͬm_LenXyz--
// #else
	m_LenXyz--; //m_LenXyz��������
//#endif

	//���㿲���ܵ���
	int BeforeKan = m_LenXyz; 
	
	//ѡ�������
	if ( 1 == m_Type && TRUE == m_bDixing ) Dgx_Dixing();

	//���Ƕ��������
	if (true == m_bKan) Dgx_Kangao(BeforeKan);

	if (m_LenXyz < 2) { 
		ads_alert(_T("����̫��")); return; 
	}
	if ( 2 == m_Type ) m_DrawType = 2;

	//��������������
	if ( false == ReadDxArray() ) return;
	
	int nOldPick = PickstyleReset(0);
	ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,_T("sjw"),RTSTR,_T("C"),RTSTR,"RED",RTSTR,_T(""),RTSTR,_T(""),0);
	if (2 == m_Type) {
		m_MinAngle = 0.0; 
		m_nColor = 4;
		ads_ssadd(NULL,NULL,m_ssJiaName);
		Join_Sjw(0,0,0);
		EraseOutside();
		ads_ssfree(m_ssJiaName);
	}
	else {
		Join_Sjw(0,0,0);
	}
	PickstyleBack(nOldPick);
    ads_command(RTSTR,_T("layer"),RTSTR,_T("s"),RTSTR,_T("0"),RTSTR,_T(""),RTNONE);

	if ( 1 == m_Type ) {//��ͨ������
		struct resbuf rb;
		rb.rbnext = NULL; rb.restype = RTSTR; rb.resval.rstring = m_datFile;
		ads_setvar(_T("users5"),&rb);
		remove("dtmsjw.sjw");
	}
}

void CJoinTriangles::SetMinAngle(double dAngle)
{
	m_MinAngle = dAngle;
}

void CJoinTriangles::sanjiao(double dMinAngle)
{
	m_Type = 3;
	m_DrawType = 3;
	
	if ( false == m_bOnlyDat )
	{
		if ( true == g_bConsiderDoukan ) {
			threeD_SeekDxing();
		}
	}

	if ( false == ReadDatFile() ) return;
	m_LenXyz--; //m_LenXyz��������
	
	/////////////////////////////////////////////////////////////////////////////
	//�Ѷ������ǽ���,20100813
	if ( false == m_bOnlyDat )
	{
		if ( true == g_bConsiderDoukan ) {
			int BeforeKan = m_LenXyz; //���㿲���ܵ���
			Dgx_Kangao(BeforeKan); //���Ƕ��������
			if ( false == ReadDxArray() ) return; //��������������
		}
	}
	/////////////////////////////////////////////////////////////////////////////

	//��m_MinAngle̫��,�ᵼ�ºܶ�㲻������,20070416
	//m_MinAngle = 0.5;
	m_MinAngle = dMinAngle; //��С�ǿɴӺ�������ȡ��

	if (m_LenXyz >= 2) Join_Sjw(0,0,0);
	else ads_printf(_T("\n���ݵ�������\n"));
}

void CJoinTriangles::sanjiao_()
{
	m_Type = 3;
	m_DrawType = 3;

	if (false == m_bOnlyDat)
	{
		if (true == g_bConsiderDoukan) {
			threeD_SeekDxing();
		}
	}

	if (false == ReadDatFile()) return;
	m_LenXyz--; //m_LenXyz��������

	/////////////////////////////////////////////////////////////////////////////
	//�Ѷ������ǽ���,20100813
	if (false == m_bOnlyDat)
	{
		if (true == g_bConsiderDoukan) {
			int BeforeKan = m_LenXyz; //���㿲���ܵ���
			Dgx_Kangao(BeforeKan); //���Ƕ��������
			if (false == ReadDxArray()) return; //��������������
		}
	}
	/////////////////////////////////////////////////////////////////////////////

	//��m_MinAngle̫��,�ᵼ�ºܶ�㲻������,20070416
	//m_MinAngle = 0.5;
	//m_MinAngle = dMinAngle; //��С�ǿɴӺ�������ȡ��
	if ((g_Dtm >= 0.0) && (g_Dtm <= 30.0)) m_MinAngle = g_Dtm;
	else m_MinAngle = 0.5;
	if (m_LenXyz >= 2) Join_Sjw(0, 0, 0);
	else ads_printf(_T("\n���ݵ�������\n"));
}

bool CJoinTriangles::sanjiaoNodes(AcGePoint3dArray &Nodes)
{
	m_Type = 3;
	m_DrawType = 3;
//#ifdef CSKCSJY
	threeD_SeekDxing();
//#endif
	if ( false == ReadNodes(Nodes) ) return false;
	m_LenXyz--; //m_LenXyz��������
	
	if (m_LenXyz >= 2) 
	{
		m_MinAngle = 0.5;

		/////////////////////////////////////////////////////////////////////////////
		//�Ѷ������ǽ���,20100813
		int BeforeKan = m_LenXyz; //���㿲���ܵ���
		Dgx_Kangao(BeforeKan); //���Ƕ��������
		/////////////////////////////////////////////////////////////////////////////
		
		if ( false == ReadDxArray() ) return false; //��������
		Join_Sjw(0,0,0);
		return true;
	}
	else return false;
}

bool CJoinTriangles::sanjiaoPick(AcGePoint3dArray &Nodes,ads_name ssName)
{
	m_Type = 2;
	m_DrawType = 2;
	m_bSjwPick = true;
//#ifdef CSKCSJY
	threeD_SeekDxing();
//#endif
	if ( false == ReadNodes(Nodes) ) return false;
	m_LenXyz--; //m_LenXyz��������
	if (m_LenXyz >= 2) 
	{
		m_MinAngle = 0.5;

		/////////////////////////////////////////////////////////////////////////////
		//�Ѷ������ǽ���,20100813
		int BeforeKan = m_LenXyz; //���㿲���ܵ���
		Dgx_Kangao(BeforeKan); //���Ƕ��������
		if ( false == ReadDxArray() ) return false; //��������������
		/////////////////////////////////////////////////////////////////////////////

		ads_ssadd(NULL,NULL,m_ssJiaName);
		Join_Sjw(0,0,0);
		ads_name_set(m_ssJiaName,ssName);
		ads_command(RTSTR,_T("REDRAW"),0);
		return true;
	}
	else return false;
}

bool CJoinTriangles::ReadDxArray()
{
	m_Kanshu = 0;
	if (dxArray.length() > 1) {
		m_Kanshu = dxArray.length();
		if ( m_Kanshu%2 == 1 ) m_Kanshu--;
		Doukan = new int[m_Kanshu+10];
		if (NULL == Doukan) { 
			ads_alert(_T("�ڴ泬��!")); return false; 
		}
		for (int i=0; i<m_Kanshu; i++) Doukan[i] = dxArray.at(i);
	}
	return true;
}

bool CJoinTriangles::cz2(ads_point ptt)
{//ֻ����ظ���
	for (int i=0; i<=m_LenXyz; i++) {
		if ( distOFpt(point[i],ptt) < 0.001 ) {
			ptt[Z]=point[i][Z]; return true;
		}
	}
	return false;
}

bool CJoinTriangles::cz1(ads_point ptt)
{
	struct NEARNEAR {
		double dis;
		double high;
	};
	struct NEARNEAR near1,near2,near3;
	ads_point pt1,pt2,pt3;
	double dis1,dis2,dis3,edge12,edge13,edge23;
	double ang1,ang2,ang3,result=0.0,res1,res2;
	bool bFind=true;

	int i;
	for (i=0; i<=m_LenXyz; i++) {//���ȼ���Ƿ����غϵ�
		if ( distOFpt(point[i],ptt) < TOLE ) {
			ptt[Z]=point[i][Z]; return true;
		}
	}
	if ( 0 == m_LenXyz ) {//ֻ��һ����
		ptt[Z] = point[0][Z]; return false;
	}
	else if (1 == m_LenXyz) {//ֻ�ж�����
		dis1 = distOFpt(point[0],ptt);
		dis2 = distOFpt(point[1],ptt);
		ptt[Z] = point[0][Z] - dis1*(point[0][Z]-point[1][Z])/(dis1+dis2); 
		return false;
	}

	for (i=0; i<m_LenSjx; i++)
	{
		ads_point_set(point[llb1[i]],pt1);
		ads_point_set(point[llb2[i]],pt2);
		ads_point_set(point[llb3[i]],pt3);
		//�����ж��Ƿ��������η�Χ��
		if ( ptt[X] < pt1[X]-TOLE && ptt[X] < pt2[X]-TOLE && ptt[X] < pt3[X]-TOLE ) continue;
		if ( ptt[X] > pt1[X]+TOLE && ptt[X] > pt2[X]+TOLE && ptt[X] > pt3[X]+TOLE ) continue;
		if ( ptt[Y] < pt1[Y]-TOLE && ptt[Y] < pt2[Y]-TOLE && ptt[Y] < pt3[Y]-TOLE ) continue;
		if ( ptt[Y] > pt1[Y]+TOLE && ptt[Y] > pt2[Y]+TOLE && ptt[Y] > pt3[Y]+TOLE ) continue;

		dis1 = distOFpt(pt1,ptt);
		if ( dis1 < TOLE ) { 
			result=pt1[Z]; break; 
		}
		dis2 = distOFpt(pt2,ptt);
		if (dis2 < TOLE) {
			result=pt2[Z]; break;
		}
		dis3 = distOFpt(pt3,ptt);
		if (dis3 < TOLE) {
			result=pt3[Z]; break;
		}

		edge12 = distOFpt(pt1,pt2);
		edge13 = distOFpt(pt1,pt3);
		edge23 = distOFpt(pt2,pt3);
		
		double dCos1 = (dis1*dis1+dis2*dis2-edge12*edge12) / (2*dis1*dis2);
		double dCos2 = (dis1*dis1+dis3*dis3-edge13*edge13) / (2*dis1*dis3);
		double dCos3 = (dis3*dis3+dis2*dis2-edge23*edge23) / (2*dis3*dis2);
		if ( dCos1 < -0.99999 ) ang1 = PI; else ang1 = acos(dCos1);
		if ( dCos2 < -0.99999 ) ang2 = PI; else ang2 = acos(dCos2);
		if ( dCos3 < -0.99999 ) ang3 = PI; else ang3 = acos(dCos3);

		if ((ang1 < 0.01)||(ang2 < 0.01)||(ang3 < 0.01)) continue;//��ֹ����ͬ��
		if ((PI-ang1) < 0.001) 
		{
			result = pt1[Z]-(pt1[Z]-pt2[Z])*dis1/edge12; break;
		}
		if ((PI-ang2) < 0.001) 
		{
			result = pt1[Z]-(pt1[Z]-pt3[Z])*dis1/edge13;break;
		}
		if ((PI-ang3) < 0.001) 
		{
			result = pt2[Z]-(pt2[Z]-pt3[Z])*dis2/edge23;
			break;
		}
		
		if ( fabs(ang1+ang2+ang3-2*PI) < 0.01 )
		{//������������
			double a0,a1,a2,t1,t2;
			if ( (fabs(pt1[X]-pt2[X])<0.001) || (fabs(pt1[X]-pt3[X])<0.001) ) 
			{
				if ( (fabs(pt2[X]-pt1[X])>0.001) && (fabs(pt2[X]-pt3[X])>0.001) ) 
				{
					a0=pt1[X]; a1=pt1[Y]; a2=pt1[Z];
					pt1[X]=pt2[X]; pt1[Y]=pt2[Y]; pt1[Z]=pt2[Z];
					pt2[X]=a0; pt2[Y]=a1; pt2[Z]=a2;
				}
				else 
				{
					a0=pt1[X]; a1=pt1[Y]; a2=pt1[Z];
					pt1[X]=pt3[X]; pt1[Y]=pt3[Y]; pt1[Z]=pt3[Z];
					pt3[X]=a0; pt3[Y]=a1; pt3[Z]=a2;
				}
			}//��ֹ��������з�ĸΪ0
			t1 = (pt1[Z]-pt2[Z])/(pt1[X]-pt2[X])-(pt1[Z]-pt3[Z])/(pt1[X]-pt3[X]);
			t2 = (pt1[Y]-pt2[Y])/(pt1[X]-pt2[X])-(pt1[Y]-pt3[Y])/(pt1[X]-pt3[X]);
			a2 = t1/t2;
			a1 = ((pt1[Z]-pt2[Z])-(pt1[Y]-pt2[Y])*a2)/(pt1[X]-pt2[X]);
			a0 = pt1[Z]-a1*pt1[X]-a2*pt1[Y];
			result = a0+a1*ptt[X]+a2*ptt[Y];
			break;
		}
	}

	//�������������������
	if ( i >= m_LenSjx ) 
	{
		bFind = false;
		dis1 = distOFpt(point[0],ptt);
		dis2 = distOFpt(point[1],ptt);
		dis3 = distOFpt(point[2],ptt);
		if (dis1>dis2) {
			if (dis2>dis3) {
				near1.dis=dis1;near1.high=point[0][Z];
				near2.dis=dis2;near2.high=point[1][Z];
				near3.dis=dis3;near3.high=point[2][Z];
			}
			else if (dis1<dis3) {
				near1.dis=dis3;near1.high=point[2][Z];
				near2.dis=dis1;near2.high=point[0][Z];
				near3.dis=dis2;near3.high=point[1][Z];
			}
			else {
				near1.dis=dis1;near1.high=point[0][Z];
				near2.dis=dis3;near2.high=point[2][Z];
				near3.dis=dis2;near3.high=point[1][Z];
			}
		}
		else {
			if (dis1>dis3) {
				near1.dis=dis2;near1.high=point[1][Z];
				near2.dis=dis1;near2.high=point[0][Z];
				near3.dis=dis3;near3.high=point[2][Z];
			}
			else if (dis2<dis3) {
				near1.dis=dis3;near1.high=point[2][Z];
				near2.dis=dis2;near2.high=point[1][Z];
				near3.dis=dis1;near3.high=point[0][Z];
			}
			else {
				near1.dis=dis2;near1.high=point[1][Z];
				near2.dis=dis3;near2.high=point[2][Z];
				near3.dis=dis1;near3.high=point[0][Z];
			}
		}
		for (int j=3; j<=m_LenXyz; j++)
		{
			dis1 = distOFpt(point[j],ptt);
			if (dis1<near3.dis) {
				near1.dis=near2.dis;near1.high=near2.high;
				near2.dis=near3.dis;near2.high=near3.high;
				near3.dis=dis1;near3.high=point[j][Z];
			}
			else if (dis1<near2.dis) {
				near1.dis=near2.dis;near1.high=near2.high;
				near2.dis=dis1;near2.high=point[j][Z];
			}
			else if (dis1<near1.dis) {
				near1.dis=dis1;near1.high=point[j][Z];
			}
		}
		if (near3.dis < TOLE) 
			result=near3.high;
		else {
			res1 = near1.high/(near1.dis*near1.dis)+near2.high/(near2.dis*near2.dis)+near3.high/(near3.dis*near3.dis);
			res2 = 1./(near1.dis*near1.dis)+1./(near2.dis*near2.dis)+1./(near3.dis*near3.dis);
			result = res1/res2;
		}
	}
	ptt[Z] = result;
	return bFind;
}

int CJoinTriangles::Sjx_CheckArray(AcGePoint3dArray& ptArray,ads_point pt)
{
	AcGePoint3d pt3d;
	int Total = ptArray.length();
	int i;
	for (i=0; i<Total; i++) 
	{
		pt3d = ptArray.at(i);
		double dist = sqrt( (pt[X]-pt3d[X])*(pt[X]-pt3d[X]) + (pt[Y]-pt3d[Y])*(pt[Y]-pt3d[Y]) );
		if (dist < TOLE) break;
	}
	if (i >= Total) 
	{
		pt3d[X]=pt[X]; pt3d[Y]=pt[Y]; pt3d[Z]=pt[Z];
		ptArray.append(pt3d);
	}
	return i;
}

bool CJoinTriangles::SjwSeekEdge(int Vertex1,int Vertex2,AcGePoint3dArray& SjwArray,int Num)
{
	bool Seeked=false;
	for (int i=0; i<SjwArray.length(); i++) 
	{
		if (i==Num) continue;
		AcGePoint3d pt3d = SjwArray.at(i);
		if ( (Vertex1==(int)pt3d[X] && Vertex2==(int)pt3d[Y]) || (Vertex1==(int)pt3d[Y] && Vertex2==(int)pt3d[X]) ) 
		{ 
			Seeked=true; break; 
		}
		if ( (Vertex1==(int)pt3d[X] && Vertex2==(int)pt3d[Z]) || (Vertex1==(int)pt3d[Z] && Vertex2==(int)pt3d[X]) ) 
		{ 
			Seeked=true; break; 
		}
		if ( (Vertex1==(int)pt3d[Y] && Vertex2==(int)pt3d[Z]) || (Vertex1==(int)pt3d[Z] && Vertex2==(int)pt3d[Y]) ) 
		{ 
			Seeked=true; break; 
		}
	}
	return Seeked;
}

short CJoinTriangles::Sjx_InsideSjx(AcGePoint3d pt, ads_point pt1, ads_point pt2, ads_point pt3)
{//1:����,0:�߽�,-1:����
	double dist12,dist13,dist23,dist1,dist2,dist3,ang12,ang13,ang23;

	dist1=sqrt((pt[X]-pt1[X])*(pt[X]-pt1[X])+(pt[Y]-pt1[Y])*(pt[Y]-pt1[Y]));
	dist2=sqrt((pt[X]-pt2[X])*(pt[X]-pt2[X])+(pt[Y]-pt2[Y])*(pt[Y]-pt2[Y]));
	dist3=sqrt((pt[X]-pt3[X])*(pt[X]-pt3[X])+(pt[Y]-pt3[Y])*(pt[Y]-pt3[Y]));
	if ( (dist1<=TOLE) || (dist2<=TOLE) || (dist3<=TOLE) ) return 0;

	dist12=sqrt((pt1[X]-pt2[X])*(pt1[X]-pt2[X])+(pt1[Y]-pt2[Y])*(pt1[Y]-pt2[Y]));
	dist13=sqrt((pt1[X]-pt3[X])*(pt1[X]-pt3[X])+(pt1[Y]-pt3[Y])*(pt1[Y]-pt3[Y]));
	dist23=sqrt((pt2[X]-pt3[X])*(pt2[X]-pt3[X])+(pt2[Y]-pt3[Y])*(pt2[Y]-pt3[Y]));
	ang12=acos((dist1*dist1+dist2*dist2-dist12*dist12)/(2*dist1*dist2));
	ang13=acos((dist1*dist1+dist3*dist3-dist13*dist13)/(2*dist1*dist3));
	ang23=acos((dist2*dist2+dist3*dist3-dist23*dist23)/(2*dist2*dist3));

	if ( (ang12>=(180-2*m_MinAngle)*PI/180) || (ang13>=(180-2*m_MinAngle)*PI/180) || (ang23>=(180-2*m_MinAngle)*PI/180) ) return 0;
	if ( (ang12+ang13+ang23) >= (PI*2) ) return 1;
	else return -1;
}

void CJoinTriangles::appendsjx()
{
	struct resbuf *rc,*rb;
	ads_name ssname,ssSjw,ent;
	long ll,length;
	ads_point pt;
	int *Inter;
	int i,j,LenPoint,ThiEdge=0,TwoEdge=0;
	
	//Code1(27);
	Reduce_DogTimes();////////����
	AcGePoint3dArray GcdArray; GcdArray.setGrowLength(50);
	ads_printf(_T("\nѡ��Ҫ����ĸ̵߳㡢���Ƶ㼰������:"));
	if (ads_ssget(NULL,NULL,NULL,NULL,ssname) != RTNORM) return;
	length=0; ads_sslength(ssname,&length);
	ads_ssadd(NULL,NULL,ssSjw);

	for (ll=0; ll<length; ll++) 
	{
		TCHAR Pltype[30],Layer[30],stdm[30];
		ads_ssname(ssname,ll,ent);
	    rc = ads_entget(ent);
		rb=rc; while (rb->restype!=0) rb=rb->rbnext; 
		_tcscpy(Pltype,rb->resval.rstring);
		rb=rc; while (rb->restype!=8) rb=rb->rbnext; 
		_tcscpy(Layer,rb->resval.rstring);
		if ( (!_tcsicmp(Pltype,_T("POLYLINE"))) && (!_tcsicmp(Layer,_T("sjw"))) ) { 
			ads_relrb(rc); ads_ssadd(ent,ssSjw,ssSjw); continue; 
		}
		if ( _tcsicmp(Pltype,_T("INSERT")) && _tcsicmp(Pltype,_T("POINT")) ) { 
			ads_relrb(rc); continue; 
		}
		rb=rc; while (rb->restype!=10) rb=rb->rbnext; 
		ads_point_set(rb->resval.rpoint,pt);
		ads_relrb(rc);
		GetStdm(ent,stdm);
		if ( false == CheckGcdCode(stdm,true) ) continue;
		if ( pt[Z] != 0.0 ) Sjx_CheckArray(GcdArray,pt);
	}
	ads_ssfree(ssname);

	AcGePoint3d pt3d,pt3dE;
	AcGePoint3dArray ptArray,SjwArray;
	ptArray.setGrowLength(100); 
	SjwArray.setGrowLength(100);
	m_MinAngle=10.0; //�������С�Ƕ�
	if ( (g_Dtm >= 0.0) && (g_Dtm <= 30.0) ) m_MinAngle = g_Dtm;

	//��ͼ��������
	length=0; ads_sslength(ssSjw,&length);
	for (ll=0; ll<length; ll++) 
	{
		ads_point ptSjx[3];
		ads_ssname(ssSjw,ll,ent);
		if (Sjw_GetDtm3pt(ent,ptSjx[0],ptSjx[1],ptSjx[2]) < 3) continue;
		pt3d[X]=Sjx_CheckArray(ptArray,ptSjx[0]);
		pt3d[Y]=Sjx_CheckArray(ptArray,ptSjx[1]);
		pt3d[Z]=Sjx_CheckArray(ptArray,ptSjx[2]);

		//���̵߳��Ƿ�����������
		bool Seeked = false;
		for (i=0; i<GcdArray.length(); i++) 
		{
			AcGePoint3d Gcd = GcdArray.at(i);
			if (Gcd[Z] < -10000) continue; //�ø̵߳��Ѵ����
			short InsideSjx = Sjx_InsideSjx(Gcd, ptSjx[0], ptSjx[1], ptSjx[2]);
			if ( (InsideSjx > 0) && (Seeked == false) ) 
			{ //���ж����ֻ����һ��
				ads_entdel(ent);
				Drawsjx(asDblArray(Gcd),ptSjx[0],ptSjx[1]); 
				Drawsjx(asDblArray(Gcd),ptSjx[0],ptSjx[2]); 
				Drawsjx(asDblArray(Gcd),ptSjx[1],ptSjx[2]); 
				int Middle = Sjx_CheckArray(ptArray,asDblArray(Gcd));
				pt3dE=pt3d; pt3dE[X]=Middle; SjwArray.append(pt3dE);
				pt3dE=pt3d; pt3dE[Y]=Middle; SjwArray.append(pt3dE);
				pt3dE=pt3d; pt3dE[Z]=Middle; SjwArray.append(pt3dE);
				Seeked = true;
			}
			if (InsideSjx >= 0) { 
				Gcd[Z] = -15000; 
				GcdArray.setAt(i,Gcd); 
			}
		}
		if (false == Seeked) SjwArray.append(pt3d);
	}
	ads_ssfree(ssSjw);

	//�������������ڵĸ̵߳�
	for (i=0; i<GcdArray.length(); i++) {
		pt3d = GcdArray.at(i);
		if (pt3d[Z] < -10000) continue;
		Sjx_CheckArray(ptArray,asDblArray(pt3d));
	}

	//���̵߳����point
	LenPoint = ptArray.length();
	point = new ads_point[LenPoint+10];
	if (NULL == point) { 
		ads_alert(_T("�޷������ڴ�")); return; 
	}
	for (i=0; i<LenPoint; i++) {
		pt3d = ptArray.at(i);
		point[i][X]=pt3d[X]; point[i][Y]=pt3d[Y]; point[i][Z]=pt3d[Z];
	}

	//ͳ�������ι���
	Inter = new int[SjwArray.length()+10];
	if (NULL == Inter) { 
		ads_alert(_T("�޷������ڴ�")); return; 
	}
	for (i=0; i<SjwArray.length(); i++) 
	{
		pt3d = SjwArray.at(i);
		bool Seek12 = SjwSeekEdge((int)pt3d[X],(int)pt3d[Y],SjwArray,i);
		bool Seek13 = SjwSeekEdge((int)pt3d[X],(int)pt3d[Z],SjwArray,i);
		bool Seek23 = SjwSeekEdge((int)pt3d[Y],(int)pt3d[Z],SjwArray,i);
		if ( Seek12==true && Seek13==true && Seek23==true ) Inter[i]=3;
		else if ( Seek12==false && Seek13==false && Seek23==false ) Inter[i]=0;
		else {
			Inter[i] = 2;
			if (false == Seek12) {
				if (true == Seek13) { 
					pt3dE[X]=pt3d[X]; pt3dE[Y]=pt3d[Z]; pt3dE[Z]=pt3d[Y]; 
				}
				else if (true == Seek23) { 
					pt3dE[X]=pt3d[Z]; pt3dE[Y]=pt3d[Y]; pt3dE[Z]=pt3d[X]; 
				}
				SjwArray.setAt(i,pt3dE);
			}
		}
	}

	//�������ζ���llb
	llb1 = new int[LenPoint*4];
	if (NULL == llb1) { 
		delete[] Inter; ads_alert(_T("�޷������ڴ�")); return; 
	}
	llb2 = new int[LenPoint*4];
	if (NULL == llb2) { 
		delete[] Inter; ads_alert(_T("�޷������ڴ�")); return; 
	}
	llb3 = new int[LenPoint*4];
	if (NULL == llb3) { 
		delete[] Inter; ads_alert(_T("�޷������ڴ�")); return; 
	}
	
	int Num = 0;
	for (j=1; j<=3; j++) 
	{
		int InterEdge;
		if (1 == j) InterEdge=3; else if (2 == j) InterEdge=0; else InterEdge=2;
		if (2 == j) ThiEdge=Num; else if (3 == j) TwoEdge=Num;
		for (i=0; i<SjwArray.length(); i++) 
		{
			if (Inter[i] == InterEdge) 
			{
				pt3d = SjwArray.at(i);
				llb1[Num] = (int)pt3d[X]; llb2[Num] = (int)pt3d[Y]; llb3[Num] = (int)pt3d[Z];
				Num++;
			}
		}
	}
	delete[] Inter;

	//��������
	m_Kanshu = 0; 
	m_LenXyz = LenPoint-1;
	int nOldPick = PickstyleReset(0);
	ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,_T("sjw"),RTSTR,_T("C"),RTSTR,"RED",RTSTR,_T(""),RTSTR,_T(""),0);
	Join_Sjw(Num,ThiEdge,TwoEdge);
	PickstyleBack(nOldPick);
    ads_command(RTSTR,_T("layer"),RTSTR,_T("s"),RTSTR,_T("0"),RTSTR,_T(""),RTNONE);
}

void CJoinTriangles::WriteToArray(AcGePoint3dArray &SanJiao)
{
	AcGePoint3d pt3d;
	for (int i=0; i<m_LenSjx; i++)
	{
		ads_point_set(point[llb1[i]],asDblArray(pt3d));
		SanJiao.append(pt3d);
		ads_point_set(point[llb2[i]],asDblArray(pt3d));
		SanJiao.append(pt3d);
		ads_point_set(point[llb3[i]],asDblArray(pt3d));
		SanJiao.append(pt3d);
	}
}

void CJoinTriangles::GetExtremHeight(double &dMinH,double &dMaxH)
{
	for (int i=0; i<=m_LenXyz; i++)
	{
		if ( 0 == i ) {
			dMinH = dMaxH = point[i][Z];
		}
		else {
			if ( point[i][Z] < dMinH ) dMinH = point[i][Z];
			if ( point[i][Z] > dMaxH ) dMaxH = point[i][Z];
		}
	}
}

void CJoinTriangles::GetPointAt(int i, ads_point pt)
{
	ads_point_set(point[i],pt);
}

bool CJoinTriangles::ReadSjwFile(TCHAR *datFile)
{
	TCHAR szStr[100];
	AcGePoint3dArray Nodes;
	AcGePoint3d pt3d;
	AcGeIntArray lb1Arr,lb2Arr,lb3Arr;

	FILE *fp = _tfopen(datFile,_T("rt"));
	if ( NULL == fp ) { 
		ads_alert(_T("�޷��������ļ�")); return false;
	}
	while ( _fgetts(szStr,255,fp) ) 
	{
		ads_point pt,pp,pp1,pp2,pp3;
		if ( !_tcsnicmp(szStr,_T("nil"),3) ) break;
		pp1[X] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp1[Y] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp1[Z] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp2[X] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp2[Y] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp2[Z] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp3[X] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp3[Y] = _tstof(szStr);
		if ( _fgetts(szStr,255,fp) == NULL ) break; pp3[Z] = _tstof(szStr);

		int lb,lb1,lb2,lb3;
		for (int ij=1; ij<=3; ij++) 
		{
			if ( 1 == ij ) ads_point_set(pp1,pt); 
			else if ( 2 == ij ) ads_point_set(pp2,pt);
			else if ( 3 == ij ) ads_point_set(pp3,pt);

			int nNumber = Nodes.length();
			int j;
			for (j=0; j<nNumber; j++) {
				ads_point_set(asDblArray(Nodes.at(j)),pp);
				double dDist = distOFpt(pt,pp);
				if (dDist < TOLE) break;
			}
			if ( j >= nNumber ) {
				ads_point_set(pt,asDblArray(pt3d));
				Nodes.append(pt3d);
				lb = nNumber;
			}
			else lb = j;

			if ( 1 == ij ) lb1 = lb;
			else if ( 2 == ij ) lb2 = lb;
			else if ( 3 == ij ) lb3 = lb;
		}
		if ( lb1!=lb2 && lb1!=lb3 && lb2!=lb3 ) {
			lb1Arr.append(lb1); lb2Arr.append(lb2); lb3Arr.append(lb3);
		}
	}
	fclose(fp);
	if ( lb1Arr.length() <= 0 ) { 
		ads_alert(_T("�����ļ�����")); return false;
	}

	m_LenXyz = Nodes.length()-1;
	point = new ads_point[m_LenXyz+5];
	int i;
	for (i=0; i<=m_LenXyz; i++) {
		pt3d = Nodes.at(i);
		ads_point_set(asDblArray(pt3d),point[i]);
	}

	m_LenSjx = lb1Arr.length();
	llb1 = new int[m_LenSjx+5];
	for (i=0; i<m_LenSjx; i++) llb1[i] = lb1Arr.at(i);
	llb2 = new int[m_LenSjx+5];
	for (i=0; i<m_LenSjx; i++) llb2[i] = lb2Arr.at(i);
	llb3 = new int[m_LenSjx+5];
	for (i=0; i<m_LenSjx; i++) llb3[i] = lb3Arr.at(i);

	return true;
}

//////////////////////////////////////////////////////////////////////
// CDrawDzx Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawDzx::CDrawDzx()
{
	double dM;
	read1(dM);
	hh = dM / 1000.0;
	m_StepSize = 2.0;
	m_ToleRance = 0.0;

	m_bJiqu = false;
	m_dSmgc = 0.0;
#ifdef CASSXBL
	m_dWidth = 0.12*hh;
#else
	m_dWidth = 0.15*hh;
#endif
	m_Nihe = 1;
	m_Type = 1;
	m_bNhqm = false;
	
	oldPick = PickstyleReset(0);
}

CDrawDzx::~CDrawDzx()
{
	PickstyleBack(oldPick);
}

int CDrawDzx::get_lw(double dKuan)
{
	int data[24],i;
	double dWidth;

	dWidth = dKuan*100;
	data[0]=0;data[1]=5;data[2]=9;data[3]=13;data[4]=15;data[5]=18;data[6]=20;data[7]=25;
	data[8]=30;data[9]=35;data[10]=40;data[11]=50;data[12]=53;data[13]=60;data[14]=70;data[15]=80;
	data[16]=90;data[17]=100;data[18]=106;data[19]=120;data[20]=140;data[21]=158;data[22]=200;data[23]=211;
	if ( dWidth <= data[0]) return data[0];
	else if (dWidth >= data[23]) return data[23];
	for (i=0;i<23;i++)
	{
		if (dWidth == data[i+1]) return data[i+1];
		if (dWidth > data[i+1]) continue;
		if ( (data[i+1]-dWidth) >= (dWidth-data[i]) ) return data[i];
		else return data[i+1];
	}
	return data[4];
}

double CDrawDzx::zlyth_dist(double x1,double x2,double y1,double y2)
{
	double dDist = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
	return dDist;
}

void CDrawDzx::pre_zlyth(int nTotal,double *xd0,double *yd0,int &nResult,double *x,double *y)
{
	double dAver=0.0,dDist;
	nResult = 0;
	
	int i;
	for (i=1; i<nTotal; i++)
	{
		dDist = zlyth_dist(xd0[i],xd0[i+1],yd0[i],yd0[i+1]);
		dAver += dDist;
	}
	dAver /= nTotal-1; //ƽ������
	dAver /= 20.0; //��С����

	int nCur = 1;
	while ( nCur <= nTotal )
	{
		int nNum=0;
		i = nCur;
		while ( i < nTotal ) {
			dDist = zlyth_dist(xd0[i],xd0[i+1],yd0[i],yd0[i+1]);
			i++;
			if ( dDist < dAver ) nNum++;
			else break;
		}
		int nTemp = nCur + nNum/2;
		if ( 1 == nCur ) nTemp = 1; //��֤��һ��
		else if ( nTotal==i && nNum>0 ) nTemp = nTotal; //��֤��ĩ��
		nResult++; x[nResult] = xd0[nTemp]; y[nResult] = yd0[nTemp]; 
		
		if ( nTotal == i ) 
		{
			if ( 0 == nNum ) {
				nResult++; x[nResult] = xd0[nTotal]; y[nResult] = yd0[nTotal]; 
			}
			break;
		}
		nCur = i;
	}
}

//xd0,yd0��1��nTotal(1<=n<=nTotal)
void CDrawDzx::zlyth(int nTotal,double *xd0,double *yd0,int bikai,double dHeight)
{
	int k,n,n1,n2,i1,i,fcount;
 	double *xp,*yp,*hp,*s,*a,*b,*c,*p,*q,*dx,*dy;
 	double delx1,dely1,dels1,delx2,dely2,dels2,sx1,sy1,sxn,syn,delsn,delxn,delyn,dx1,dy1;
 	double dx2,dy2,sigma,dels,d1,d2,ss,zs1,zs2,zs3,xx,yy;

	//������
	double *x,*y;
	x = new double[nTotal+5];
	y = new double[nTotal+5];
	pre_zlyth(nTotal,xd0,yd0,n,x,y);

 	int lengthxyz = n+5;
	xp=new double[lengthxyz];
 	yp=new double[lengthxyz];
 	hp=new double[lengthxyz];
 	s=new double[lengthxyz];
 	a=new double[lengthxyz];
 	b=new double[lengthxyz];
 	c=new double[lengthxyz];
 	p=new double[lengthxyz];
 	q=new double[lengthxyz];
 	dx=new double[lengthxyz];
 	dy=new double[lengthxyz];

 	n1=n-1; n2=n-2;
	delx1 = x[2]-x[1];
	dely1 = y[2]-y[1];
 	dels1=sqrt(delx1*delx1+dely1*dely1);
 	sx1=delx1/dels1;
 	sy1=dely1/dels1;
	if ( (x[1]==x[n]) && (y[1]==y[n]) ) {
		sxn=sx1; syn=sy1;
	}
	else {
		delxn=x[n]-x[n1];
		delyn=y[n]-y[n1];
		delsn=sqrt(delxn*delxn+delyn*delyn);
	  	sxn=delxn/delsn;
	  	syn=delyn/delsn;
	}
 	dx1=delx1/dels1;
 	dy1=dely1/dels1;
 	xp[1]=dx1-sx1;dx[1]=xp[1];
 	yp[1]=dy1-sy1;dy[1]=yp[1];
 	hp[1]=dels1;
 	s[1]=0.0;
 	s[2]=dels1;
 	for(i=2;i<=n1;i++) {
		delx2=x[i+1]-x[i];
		dely2=y[i+1]-y[i];
		dels2=sqrt(delx2*delx2+dely2*dely2);
		dx2=delx2/dels2;
		dy2=dely2/dels2;
		xp[i]=dx2-dx1;dx[i]=xp[i];
		yp[i]=dy2-dy1;dy[i]=yp[i];
		hp[i]=dels2;
		dx1=dx2;
		dy1=dy2;
		i1=i+1;
		s[i1]=s[i]+dels2;
  	}

 	xp[n]=0.0;dx[n]=0.0;
 	yp[n]=0.0;dy[n]=0.0;
 	sigma=4.5*(n-1)/s[n]; //�޸Ĳ���,20060308
 	dels=sigma*hp[1];
 	d1=sigma*(((exp(dels)+exp(-dels))/2)/((exp(dels)-exp(-dels))/2))-1/hp[1];

 	b[1]=d1;
 	c[1]=1/hp[1]-sigma/((exp(dels)-exp(-dels))/2);
 	a[1]=0.0;
 	for(i=2;i<=n1;i++)
	{
		dels=sigma*hp[i];
		dely1=y[2]-y[1];
		c[i]=1/hp[i]-sigma/((exp(dels)-exp(-dels))/2);
		a[i]=c[i];
		d2=sigma*(((exp(dels)+exp(-dels))/2)/((exp(dels)-exp(-dels))/2))-1/hp[i];
		b[i]=d1+d2;
		d1=d2;
  	}
 	a[n]=1/delsn-sigma/((exp(sigma*delsn)-exp(-sigma*delsn))/2);
 	b[n]=sigma*((exp(sigma*delsn)+exp(-sigma*delsn))/2)/((exp(sigma*delsn)-exp(-sigma*delsn))/2)-1/delsn;
 	c[n]=0.0;

 	xp[1]=xp[1]/b[1];
 	yp[1]=yp[1]/b[1];
 	q[1]=c[1]/b[1];
 	for(k=2;k<=n;k++)
  	{
  		xp[k]=(dx[k]-a[k]*xp[k-1])/(b[k]-a[k]*q[k-1]);
  		yp[k]=(dy[k]-a[k]*yp[k-1])/(b[k]-a[k]*q[k-1]);
  		q[k]=c[k]/(b[k]-a[k]*q[k-1]);
  	}

 	for (i=n1;i>=1;i--) xp[i]-=q[i]*xp[i+1];
 	for (i=n1;i>=1;i--) yp[i]-=q[i]*yp[i+1];
	
	AcGePoint2d point; 
	AcGePoint2dArray ptS; ptS.setGrowLength(10);
 	for (i=bikai; i<=n1; i++) 
	{
		fcount=(int)(hp[i]/m_StepSize+1);
		//if (fcount<2) fcount=4;
		for (k=0; k<=fcount; k++) 
		{
			if ( 0==k && i>bikai ) continue;//�����ظ�,20060308
			if ( k*hp[i]/fcount >= hp[i] )  ss=s[i]+hp[i];
	  		else ss=s[i]+k*hp[i]/fcount;
	  		zs1=sigma*(s[i+1]-ss);
	  		zs2=sigma*(ss-s[i]);
	  		zs3=sigma*hp[i];
	  		xx=(xp[i]*((exp(zs1)-exp(-zs1))/2)+xp[i+1]*((exp(zs2)-exp(-zs2))/2))/((exp(zs3)-exp(-zs3))/2)+(x[i]-xp[i])*(s[i+1]-ss)/hp[i]+(x[i+1]-xp[i+1])*(ss-s[i])/hp[i];
	  		yy=(yp[i]*((exp(zs1)-exp(-zs1))/2)+yp[i+1]*((exp(zs2)-exp(-zs2))/2))/((exp(zs3)-exp(-zs3))/2)+(y[i]-yp[i])*(s[i+1]-ss)/hp[i]+(y[i+1]-yp[i+1])*(ss-s[i])/hp[i];
			point[X]=xx; point[Y]=yy; ptS.append(point);
		}
 	}
	DrawPolyline2D(ptS,dHeight,false,m_szStdm,m_dWidth);

	delete[] a; delete[] b; delete[] c; delete[] dx; delete[] dy; delete[] p;
	delete[] xp; delete[] yp; delete[] hp; delete[] s; delete[] q;
	delete[] x; delete[] y;
}

/*
//���������㹹���ߵĽǶ�
double zl_angle(double x1,double y1,double x0,double y0)
{
	ads_point pt1,pt0;
	pt1[X]=x1; pt1[Y]=y1; pt1[Z]=0.0;
	pt0[X]=x0; pt0[Y]=y0; pt0[Z]=0.0;
	double dAngle = ads_angle(pt1,pt0);
	return dAngle;
}

double zl_sinh(double ds)
{
	double dR = ( ( exp(ds) - exp(-ds) ) / 2 );
	return dR;
}

//n:���ߵ�ԭʼ���ݵ���
//x,y�����ߵ���ά����
void CDrawDzx::zlyth(int n,double *x,double *y,int bikai,double dHeight)
{
	double *xx,*yy;
	double *xp,*yp,*s,*a,*b,*c,*hp;
	double dtx1,dtx2,ds1,dty1,dty2,six1,siy1,sixn,siyn;
	double dx1,dy1,ds2,dx2,dy2,sma,ds;
	double d1,d2,d3,q,ss,e1,e2,e3,x1,y1,alf;
	int i,i1,k,k1,n1,j; 

	if ( n <= 2 ) return;
	xx = (double *)malloc((n+2)*sizeof(double));
	yy = (double *)malloc((n+2)*sizeof(double));
	xp = (double *)malloc((n+2)*sizeof(double));
	yp = (double *)malloc((n+2)*sizeof(double));
	s = (double *)malloc((n+2)*sizeof(double));
	a = (double *)malloc((n+2)*sizeof(double));
	b = (double *)malloc((n+2)*sizeof(double));
	c = (double *)malloc((n+2)*sizeof(double));
	hp = (double *)malloc((n+2)*sizeof(double));

	memcpy(xx,x,(n+2)*8);
	memcpy(yy,y,(n+2)*8);
	n--;
	n1 = n-1;

	alf = zl_angle(xx[1],yy[1],xx[0],yy[0]);
	dtx1 = xx[1]-xx[0];
	dty1 = yy[1]-yy[0];
	ds1 = sqrt( dtx1 * dtx1 + dty1 * dty1 );
	six1 = cos(alf);
	siy1 = sin(alf);
	if ((xx[0]==xx[n])&&(yy[0]==yy[n])) {
		sixn=six1;
		siyn=siy1;
	}
	else {
		alf=zl_angle(xx[n],yy[n],xx[n1],yy[n1]);
		sixn=cos(alf);
		siyn=sin(alf);
	}

	dx1=six1;
	dy1=siy1;
	xp[0]=dx1-six1;
	yp[0]=dy1-siy1;
	hp[0]=ds1;
	s[0]=0.0;
	s[1]=ds1;
	d3=ds1;

	for (i=1;i<=n1;i++) 
	{
		dtx2=xx[i+1]-xx[i];
		dty2=yy[i+1]-yy[i];
		ds2 = sqrt( dtx2 * dtx2 + dty2 * dty2 );

		alf=zl_angle(xx[i+1],yy[i+1],xx[i],yy[i]);
		dx2=cos(alf);
		dy2=sin(alf);
		xp[i]=dx2-dx1;
		yp[i]=dy2-dy1;
		hp[i]=ds2;
		dx1=dx2;
		dy1=dy2;
		i1=i+1;
		s[i1]=s[i]+ds2;
		d3=d3+ds2;
	} 

	xp[n]=sixn-dx1;
	yp[n]=siyn-dy1;
	sma=1.5*n/s[n];
	ds=sma*hp[0];
	d1 = sma * ( ((exp(ds)+exp(-ds))/2) / zl_sinh(ds) ) - 1/hp[0];
	b[0]=d1;
	c[0]=1/hp[0]-sma/zl_sinh(ds);
	a[0]=c[0];
	
	for (i=1;i<=n1;i++) 
	{
		ds=sma*hp[i];
		c[i]=1/hp[i]-sma/zl_sinh(ds);
		a[i]=c[i-1];
		d2=sma*(((exp(ds)+exp(-ds))/2)/zl_sinh(ds))-1/hp[i];
		b[i]=d1+d2;
		d1=d2;
	}

	b[n]=d1;
	xp[0]=xp[0]/b[0];
	yp[0]=yp[0]/b[0];
	q=b[0];
	
	for (k=0;k<=n1;k++) 
	{
		b[k]=c[k]/q;
		q=b[k+1]-a[k]*b[k];
		k1=k+1;
		xp[k1]=(xp[k1]-a[k]*xp[k])/q;
		yp[k1]=(yp[k1]-a[k]*yp[k])/q;
	}
	for (i=0;i<=n1;i++) 
	{
		k=n-i-1;
		xp[k]=xp[k]-b[k]*xp[k+1];
		yp[k]=yp[k]-b[k]*yp[k+1];
	}

	AcGePoint2d pt2d; 
	AcGePoint2dArray Nodes; Nodes.setGrowLength(10);
	pt2d.x=xx[0]; pt2d.y=yy[0]; Nodes.append(pt2d);
	for (i=0; i<=n1; i++) 
	{
		k = (int)(hp[i]/m_StepSize);
		for (j=1;j<k;j++) 
		{
			ss=s[i]+j*m_StepSize;
			e1=sma*(s[i+1]-ss);
			e2=sma*(ss-s[i]);
			e3=sma*hp[i];
			x1=(xp[i]*zl_sinh(e1)+xp[i+1]*zl_sinh(e2))/zl_sinh(e3)+(xx[i]-xp[i])
			*(s[i+1]-ss)/hp[i]+(xx[i+1]-xp[i+1])*(ss-s[i])/hp[i];
			y1=(yp[i]*zl_sinh(e1)+yp[i+1]*zl_sinh(e2))/zl_sinh(e3)+(yy[i]-yp[i])
			*(s[i+1]-ss)/hp[i]+(yy[i+1]-yp[i+1])*(ss-s[i])/hp[i];
			pt2d.x=x1; pt2d.y=y1; Nodes.append(pt2d);
		} 
		pt2d.x=xx[i+1]; pt2d.y=yy[i+1]; Nodes.append(pt2d);
	} 
	DrawPolyline2D(Nodes,dHeight,false,m_szStdm,m_dWidth);
	
	free(xp);free(yp);free(hp);
	free(s);free(a);free(b);free(c);
	free(xx);free(yy);
}
*/

void CDrawDzx::DrawDzx(double dHeight)
{
	int *lb,mm,flag,i,j,ld1,Oldld1;
	double *xd0,*yd0,*xb[2],*yb[2],x1,y1;
	AcGePoint2d pt2d; 
	AcGePoint2dArray pt2dArray;
	AcGePoint3d pt1,pt2,pt3;
	ads_name ent;
	double dMin = 0.0001;

	int LWeight = get_lw(0.3*hh);
	int LenSjx = lb1.length();
	xb[0] = new double[LenSjx+10];
	xb[1] = new double[LenSjx+10];
	yb[0] = new double[LenSjx+10];
	yb[1] = new double[LenSjx+10];
	xd0 = new double[LenSjx+10]; 
	yd0 = new double[LenSjx+10]; 
	lb = new int[LenSjx+10]; 
	if ( NULL==xb[0] || NULL==xb[1] || NULL==yb[0] || NULL==yb[1] || NULL==xd0 || NULL==yd0 || NULL==lb )
	{
		if ( NULL != xb[0] ) delete[] xb[0];
		if ( NULL != xb[1] ) delete[] xb[1];
		if ( NULL != yb[0] ) delete[] yb[0];
		if ( NULL != yb[1] ) delete[] yb[1];
		if ( NULL != xd0 ) delete[] xd0;
		if ( NULL != yd0 ) delete[] yd0;
		if ( NULL != lb ) delete[] lb;
		ads_alert(_T("�޷������ڴ�"));
		return;
	}

	//��ʼ��ֵ
	for (i=0; i<LenSjx; i++) 
	{
		pt1 = Points.at(lb1.at(i)); 
		pt2 = Points.at(lb2.at(i)); 
		pt3 = Points.at(lb3.at(i));
		if ( fabs(dHeight-pt1[Z])<dMin && fabs(dHeight-pt2[Z])<dMin && fabs(dHeight-pt3[Z])<dMin ) 
		{//��������������߳�һ��,����߼���pt2dArray
			double dist12,dist23,dist13;
			dist12 = sqrt( (pt1[X]-pt2[X])*(pt1[X]-pt2[X]) + (pt1[Y]-pt2[Y])*(pt1[Y]-pt2[Y]) );
			dist23 = sqrt( (pt2[X]-pt3[X])*(pt2[X]-pt3[X]) + (pt2[Y]-pt3[Y])*(pt2[Y]-pt3[Y]) );
			dist13 = sqrt( (pt1[X]-pt3[X])*(pt1[X]-pt3[X]) + (pt1[Y]-pt3[Y])*(pt1[Y]-pt3[Y]) );
			if ( (dist12>=dist23) && (dist12>=dist13) ) {
				pt2d[X]=pt1[X]; pt2d[Y]=pt1[Y]; pt2dArray.append(pt2d);
				pt2d[X]=pt2[X]; pt2d[Y]=pt2[Y]; pt2dArray.append(pt2d);
			}
			else if (dist23>=dist13) {
				pt2d[X]=pt2[X]; pt2d[Y]=pt2[Y]; pt2dArray.append(pt2d);
				pt2d[X]=pt3[X]; pt2d[Y]=pt3[Y]; pt2dArray.append(pt2d);
			}
			else {
				pt2d[X]=pt1[X]; pt2d[Y]=pt1[Y]; pt2dArray.append(pt2d);
				pt2d[X]=pt3[X]; pt2d[Y]=pt3[Y]; pt2dArray.append(pt2d);
			}
		}
	}
	for (i=0; i<LenSjx; i++)	//m,lb[i]Ϊĳ�ȸ����������εĽ�����,xb[m-1][i],yb[m-1][i]�ǽ�������
	{							//lb1[i],lb2[i],lb3[i]Ϊ��i������������������
		pt1 = Points.at(lb1.at(i)); 
		pt2 = Points.at(lb2.at(i)); 
		pt3 = Points.at(lb3.at(i));
		mm=0; flag=0;
		bool bEqual1=false,bEqual2=false,bEqual3=false; //20060922

		if ( fabs(pt1[Z]-pt2[Z])<dMin && fabs(pt2[Z]-pt3[Z])<dMin ) { //��������������߳�һ��
			lb[i] = 0; continue;
		}
		if ( fabs(dHeight-pt1[Z]) < dMin ) { //�ȸ��߸̵߳��ڵ�һ������߳�
			bEqual1 = true;
			mm++; flag++; xb[mm-1][i]=pt1[X]; yb[mm-1][i]=pt1[Y];
		}
		if ( fabs(dHeight-pt2[Z]) < dMin ) { //�ȸ��߸̵߳��ڵڶ�������߳�
			bEqual2 = true;
			mm++; flag++; xb[mm-1][i]=pt2[X]; yb[mm-1][i]=pt2[Y];
		}
		if ( fabs(dHeight-pt3[Z]) < dMin ) { //�ȸ��߸̵߳��ڵ���������߳�
			bEqual3 = true;
			mm++; flag++; xb[mm-1][i]=pt3[X]; yb[mm-1][i]=pt3[Y];
		}

		if ( false == bEqual1 && false == bEqual2 ) {
			if ((dHeight-pt1[Z])*(dHeight-pt2[Z])<0) { //�ȸ��ߴ�����12
				mm++;
				xb[mm-1][i] = pt1[X]+(pt2[X]-pt1[X])*(dHeight-pt1[Z])/(pt2[Z]-pt1[Z]);
				yb[mm-1][i] = pt1[Y]+(pt2[Y]-pt1[Y])*(dHeight-pt1[Z])/(pt2[Z]-pt1[Z]);
	  		}
		}
		if ( false == bEqual2 && false == bEqual3 ) {
			if ((dHeight-pt2[Z])*(dHeight-pt3[Z])<0) { //�ȸ��ߴ�����23
				mm++;
				xb[mm-1][i] = pt2[X]+(pt3[X]-pt2[X])*(dHeight-pt2[Z])/(pt3[Z]-pt2[Z]);
				yb[mm-1][i] = pt2[Y]+(pt3[Y]-pt2[Y])*(dHeight-pt2[Z])/(pt3[Z]-pt2[Z]);
	  		}
		}
		if ( false == bEqual3 && false == bEqual1 ) {
			if ((dHeight-pt3[Z])*(dHeight-pt1[Z])<0) { //�ȸ��ߴ�����13
				mm++;
				xb[mm-1][i] = pt3[X]+(pt1[X]-pt3[X])*(dHeight-pt3[Z])/(pt1[Z]-pt3[Z]);
				yb[mm-1][i] = pt3[Y]+(pt1[Y]-pt3[Y])*(dHeight-pt3[Z])/(pt1[Z]-pt3[Z]);
	  		}
		}

		if (mm != 2) mm=0; //��ֻ��һ�����㽫��������
		if ( (2 == mm) && (2 == flag) ) 
		{ //�������㶼�������ζ���
			int SeekEd=0;
			for (j=0; j<pt2dArray.length(); j=j+2) {
				if ( fabs(xb[0][i]-pt2dArray.at(j)[X])<dMin && fabs(yb[0][i]-pt2dArray.at(j)[Y])<dMin && fabs(xb[1][i]-pt2dArray.at(j+1)[X])<dMin && fabs(yb[1][i]-pt2dArray.at(j+1)[Y])<dMin )
				{ SeekEd=1; break; }
				if ( fabs(xb[1][i]-pt2dArray.at(j)[X])<dMin && fabs(yb[1][i]-pt2dArray.at(j)[Y])<dMin && fabs(xb[0][i]-pt2dArray.at(j+1)[X])<dMin && fabs(yb[0][i]-pt2dArray.at(j+1)[Y])<dMin )
				{ SeekEd=1; break; }
			}
			if (0 == SeekEd) {
				pt2d[X]=xb[0][i]; pt2d[Y]=yb[0][i]; pt2dArray.append(pt2d);
				pt2d[X]=xb[1][i]; pt2d[Y]=yb[1][i]; pt2dArray.append(pt2d);
			}
			else mm=0;
		}
		lb[i] = mm;
	} //��ֵ���
		
	//����ͷ��β
	for (i=0; i<LenSjx; i++) //��lb[i]=1,xb[0][i]����ͷ
	{
		if ( lb[i] != 2 ) continue;
		//��ʼ�����һ������
		mm = 0;
		for (j=0; j<LenSjx; j++) {
			if ( 0 == lb[j] ) continue;
			if ( fabs(xb[0][i]-xb[0][j])<dMin && fabs(yb[0][i]-yb[0][j])<dMin ) mm++; 
			if ( fabs(xb[0][i]-xb[1][j])<dMin && fabs(yb[0][i]-yb[1][j])<dMin ) mm++; 
	 	}
		if ( 1 == mm ) { //ȷ������ͷ
			lb[i]=1; continue; 
		} 

		//��ʼ����ڶ�������
		mm = 0;
		for (j=0; j<LenSjx; j++) {
			if ( 0 == lb[j] ) continue;
			if ( fabs(xb[1][i]-xb[0][j])<dMin && fabs(yb[1][i]-yb[0][j])<dMin ) mm++;
			if ( fabs(xb[1][i]-xb[1][j])<dMin && fabs(yb[1][i]-yb[1][j])<dMin ) mm++;
	 	}
		if ( 1 == mm ) { //ȷ������ͷ
			x1=xb[0][i]; y1=yb[0][i]; xb[0][i]=xb[1][i]; yb[0][i]=yb[1][i]; xb[1][i]=x1; yb[1][i]=y1;
			lb[i]=1; //ȷ������ͷ,�����������������
		}
  	}//����ͷ��β���
	
	//������
	for (i=0; i<LenSjx; i++) 
	{
	  	if (1 == lb[i]) 
		{
			xd0[0]=xb[0][i]; yd0[0]=yb[0][i];
			xd0[1]=xb[1][i]; yd0[1]=yb[1][i];
		 	flag = 0;
	 		for (j=0; j<LenSjx; j++) {
				if (0 == lb[j]) continue; //add at 2001.11.27
				if ( fabs(xb[0][j]-xd0[0])<dMin && fabs(yb[0][j]-yd0[0])<dMin ) flag++;
				if ( fabs(xb[1][j]-xd0[0])<dMin && fabs(yb[1][j]-yd0[0])<dMin ) flag++; //add at 2001.11.27
			}
			if (flag >= 2) { //modify at 2001.11.27
				xd0[0]=xb[1][i]; yd0[0]=yb[1][i];
				xd0[1]=xb[0][i]; yd0[1]=yb[0][i];
			}
		 	
			lb[i] = 0; //��־�ѱ�����
	 		ld1 = 1;
			while (1) 
			{
				Oldld1 = ld1;
				for (j=0; j<LenSjx; j++) {
					if ( 0 == lb[j] ) continue;
					if ( fabs(xd0[ld1]-xb[0][j])<dMin && fabs(yd0[ld1]-yb[0][j])<dMin ) {
						ld1++;
						xd0[ld1]=xb[1][j]; yd0[ld1]=yb[1][j];
						lb[j]=0; break;
					}
					if ( fabs(xd0[ld1]-xb[1][j])<dMin && fabs(yd0[ld1]-yb[1][j])<dMin ) {
						ld1++;
						xd0[ld1]=xb[0][j]; yd0[ld1]=yb[0][j];
						lb[j]=0; break;
					}
			  	}
				if (Oldld1 == ld1) break; //û���ҵ���һ�������Σ���������
	   		}
   		
			//��ʼ��ͼ
			if ( 1 == m_Nihe || 3 == m_Nihe )
			{
				int nSpline=0;
				if ( 3 == m_Nihe ) nSpline = 2;
				AcGePoint2d pt2d;
				AcGePoint2dArray Nodes; Nodes.setGrowLength(10);
				for (j=0; j<=ld1; j++) {
					pt2d[X]=xd0[j]; pt2d[Y]=yd0[j];
					Nodes.append(pt2d);
				}
				DrawPolyline2D(Nodes,dHeight,false,m_szStdm,m_dWidth,nSpline);
			}
			else if (4 == m_Nihe) {
				AcGePoint3d point; 
				AcGePoint3dArray ptS; ptS.setGrowLength(10);
				for (j=0; j<=ld1; j++) {
					point[X]=xd0[j]; point[Y]=yd0[j]; point[Z]=dHeight;
					ptS.append(point);
				}
				AcDbSpline *pPline = new AcDbSpline(ptS,4,m_ToleRance);
				if (true == m_bJiqu)
					pPline->setLineWeight((AcDb::LineWeight)LWeight);
				AcDbObjectId objId;
				AppendEntity(pPline,objId);
				pPline->close();
			}
	   		else if (2 == m_Nihe) {
				for (j=ld1; j>=0; j--) { 
					xd0[j+1]=xd0[j]; yd0[j+1]=yd0[j]; 
				}
				zlyth(ld1+1,xd0,yd0,1,dHeight);
			}

			ads_entlast(ent);
#ifdef JKS_CZDJ
			CString Str;
			if ( true == m_bJiqu ) { Str.Format(_T("710102")); AddXdata(ent,_T("DGXLX"),0,Str); }
			else { Str.Format(_T("710101")); AddXdata(ent,_T("DGXLX"),0,Str); }
#endif

			if ( 4 == m_Nihe ) addapp(ent,m_szStdm); //2004.12.13
#if ( !defined(SUZHOU) && !defined(TJ_SURVEY) )
			if (true == m_bJiqu) {
#ifndef JINGSAI_2013
				Adesk::UInt16 nColor=3;
#ifdef GD_HANGDAO
				nColor = 1;
#endif
				setcolor(ent,nColor);
#else
				SetColorByLayer(ent);
#endif
			}
#endif
			//if ( (2 == m_Type) && (dHeight > m_dSmgc) ) setlinetype(ent,_T("continuous"));

#ifdef SUZHOU
#ifndef CASSXBL
			TCHAR szSimbol[30];
			if (1 == m_Type) {
				if (true == m_bJiqu) _tcscpy(szSimbol,_T("812002"));
				else _tcscpy(szSimbol,_T("811002"));
			}
			else {
				if (true == m_bJiqu) _tcscpy(szSimbol,_T("675022"));
				else _tcscpy(szSimbol,_T("675012"));
			}
			ChangeLineTypeName(ent,szSimbol);
#endif
#endif
		}
	}

 	///////������
	for (i=0; i<LenSjx; i++)
	{
	  	if (2 == lb[i]) 
		{
			xd0[0]=xb[0][i]; yd0[0]=yb[0][i];
			xd0[1]=xb[1][i]; yd0[1]=yb[1][i];
		  	lb[i] = 0;
		  	ld1 = 1;
		 	while (1) {
				Oldld1=ld1;
				for (j=0; j<LenSjx; j++) {
					if (0 == lb[j]) continue;
				  	if ( fabs(xd0[ld1]-xb[0][j])<dMin && fabs(yd0[ld1]-yb[0][j])<dMin ) {
						ld1++;
						xd0[ld1]=xb[1][j]; yd0[ld1]=yb[1][j];
						lb[j]=0; break;
					}
				  	if ( fabs(xd0[ld1]-xb[1][j])<dMin && fabs(yd0[ld1]-yb[1][j])<dMin ) {
						ld1++;
						xd0[ld1]=xb[0][j]; yd0[ld1]=yb[0][j];
						lb[j]=0; break;
					}
				}
				if (Oldld1==ld1) break;
			}
			xd0[ld1]=xd0[0]; yd0[ld1]=yd0[0];
			if (2 == m_Nihe) { 
				ld1++; xd0[ld1]=xd0[1]; yd0[ld1]=yd0[1]; 
			}

			if ( 1 == m_Nihe || 3 == m_Nihe )
			{
				int nSpline=0;
				if ( 3 == m_Nihe ) nSpline = 2;
				AcGePoint2d pt2d;
				AcGePoint2dArray Nodes; Nodes.setGrowLength(10);
				for (j=0; j<ld1; j++) { //���һ�����һ���غ�,��Ҫ
					pt2d[X]=xd0[j]; pt2d[Y]=yd0[j];
					Nodes.append(pt2d);
				}
				bool bClose=true;
				if (ld1 <= 2) bClose = false;
				DrawPolyline2D(Nodes,dHeight,bClose,m_szStdm,m_dWidth,nSpline);
			}
			else if (4 == m_Nihe) 
			{
				ads_point point;
				for (j=0; j<ld1; j++) { //���һ�����һ���غ�,��Ҫ
					point[X]=xd0[j]; point[Y]=yd0[j]; point[Z]=dHeight;
					if (0 == j) 
						ads_command(RTSTR,_T("SPLINE"),RT3DPOINT,point,RTNONE);
					else 
						ads_command(RT3DPOINT,point,RTNONE);
				}
				if (ld1 <= 1) ads_command(RTSTR,_T(""),RTNONE);
				else if (ld1>2) ads_command(RTSTR,_T("C"),RTSTR,_T(""),RTNONE);
				else ads_command(RTSTR,_T(""),RTSTR,_T(""),RTSTR,_T(""),RTNONE);

				if (ld1 > 1) {
					ads_entlast(ent);
					addapp(ent,m_szStdm);
					if (true == m_bJiqu) {
						AcDbObjectId eld;
						acdbGetObjectId(eld,ent);
						AcDbSpline *pPline;
						acdbOpenObject(pPline,eld,AcDb::kForWrite);
						pPline->setLineWeight((AcDb::LineWeight)LWeight);
						pPline->close();
					}
				}
			}
	   		else if (2 == m_Nihe) {
				for (j=ld1;j>=0;j--) { 
					xd0[j+1]=xd0[j]; yd0[j+1]=yd0[j]; 
				}
				zlyth(ld1+1,xd0,yd0,2,dHeight);
			}	

			ads_entlast(ent);
#ifdef JKS_CZDJ
			CString Str;
			if ( true == m_bJiqu ) { Str.Format(_T("710102")); AddXdata(ent,_T("DGXLX"),0,Str); }
			else { Str.Format(_T("710101")); AddXdata(ent,_T("DGXLX"),0,Str); }
#endif

#if ( !defined(SUZHOU) && !defined(TJ_SURVEY) )
			if (true == m_bJiqu) {
#ifndef JINGSAI_2013
				Adesk::UInt16 nColor=3;
#ifdef GD_HANGDAO
				nColor = 1;
#endif
				setcolor(ent,nColor);
#else
				SetColorByLayer(ent);
#endif
			}
#endif
			//if ( (2 == m_Type) && (dHeight > m_dSmgc) ) setlinetype(ent,_T("continuous"));
#ifdef SUZHOU
#ifndef CASSXBL
			TCHAR szSimbol[30];
			if (1 == m_Type) {
				if (true == m_bJiqu) _tcscpy(szSimbol,_T("812002"));
				else _tcscpy(szSimbol,_T("811002"));
			}
			else {
				if (true == m_bJiqu) _tcscpy(szSimbol,_T("675022"));
				else _tcscpy(szSimbol,_T("675012"));
			}
			ChangeLineTypeName(ent,szSimbol);
#endif
#endif
		}
	}

	delete[] lb;delete[] xd0; delete[] yd0; 
	delete[] xb[0]; delete[] xb[1]; delete[] yb[0]; delete[] yb[1];
}

void CDrawDzx::GetDzxStdmWidth(double dHeight)
{
#ifdef CASSXBL
	if (true == m_bJiqu) {
		if (1 == m_Type) {
			if ( dHeight > -0.00001 ) { _tcscpy(m_szStdm,_T("x710102")); m_dWidth = 0.25*hh; }
			else { _tcscpy(m_szStdm,_T("x270702")); m_dWidth = 0.2*hh; }
		}
		else { 
			_tcscpy(m_szStdm,_T("x270900")); m_dWidth = 0.12*hh; //������
			//_tcscpy(m_szStdm,_T("x270702")); m_dWidth = 0.25*hh; 
		}
	}
	else {
		if (1 == m_Type) {
			if ( dHeight > -0.00001 ) _tcscpy(m_szStdm,_T("x710101")); 
			else _tcscpy(m_szStdm,_T("x270701")); 
		}
		else {
			_tcscpy(m_szStdm,_T("x270900")); //������
			//_tcscpy(m_szStdm,_T("x270701")); //ˮ�µȸ���
		}
		m_dWidth = 0.12*hh;
	}
#else
	if (true == m_bJiqu) {
		if (1 == m_Type) _tcscpy(m_szStdm,_T("201102")); 
		else _tcscpy(m_szStdm,_T("186302"));
		m_dWidth = 0.3*hh;
	}
	else {
		if (1 == m_Type) _tcscpy(m_szStdm,_T("201101")); 
		else _tcscpy(m_szStdm,_T("186301"));
		m_dWidth = 0.15*hh;
	}
#ifdef GD_HANGDAO
	if (2 == m_Type) {
		if ( fabs(dHeight) < 0.0001 ) _tcscpy(m_szStdm,_T("186303"));
	}
#endif

#endif
}

void CDrawDzx::draw()
{
	FILE *fp;
	TCHAR szStr[255],NumFile[255],*ppp;
	int LenSjx,LenPoint=0,MinCount,MaxCount;
	AcGePoint3d pt3d;
	double maxZ,minZ,deltz,dSingle;
    bool DtmSjw = false;
	bool bFind = false;
	bool bSingle = false; //���Ƶ��ȸ���
	struct resbuf res;
	
	//Code1(23);

	//�ȿ���users5
	ads_getvar(_T("users5"),&res); 
	_tcscpy(NumFile,res.resval.rstring);
	if ( _tcscmp(NumFile,_T("")) ) {
		_tcscpy(NumFile,res.resval.rstring); 
		ppp = _tcsrchr(NumFile,'.'); 
		if (ppp != NULL) { 
			*ppp='\0'; _tcscat(NumFile,_T(".SJW")); 
			fp = _tfopen(NumFile,_T("rt"));
			if ( NULL != fp ) bFind = true;
		}
	}

	//�ڶ�����dtmsjw.sjw
	if ( false == bFind ) {
		fp = _tfopen(_T("dtmsjw.sjw"),_T("rt")); 
		if (fp != NULL) {
			DtmSjw = true; bFind = true; 
		}
	}

	//��������ͼ���
	if ( false == bFind ) {
		ads_name ssName;
		ads_ssget(_T("X"),NULL,NULL,NULL,ssName);
		Sjw_SaveSjx(ssName,_T("dtmsjw.sjw"));
		ads_ssfree(ssName);
		fp = _tfopen(_T("dtmsjw.sjw"),_T("rt")); 
		if (fp != NULL) {
			DtmSjw = true; bFind = true; 
		}
	}

	if ( false == bFind ) {
		ads_prompt(_T("����������������!!!\n")); return;
	}
	
	while ( _fgetts(szStr,255,fp) ) LenPoint++; 
	LenPoint /= 3;
	if ( 0 == LenPoint ) {
		fclose(fp);
		if ( true == DtmSjw ) remove("dtmsjw.sjw");
		ads_prompt(_T("����������������!!!\n")); return;
	}

	rewind(fp);
	Points.setLogicalLength(LenPoint);
	int i;
	for ( i=0; i<LenPoint; i++) {
		if (_fgetts(szStr,255,fp)==NULL) break; pt3d[X]=_tstof(szStr);
		if (_fgetts(szStr,255,fp)==NULL) break; pt3d[Y]=_tstof(szStr);
		if (_fgetts(szStr,255,fp)==NULL) break; pt3d[Z]=_tstof(szStr);
		Points.setAt(i,pt3d);
	}
	fclose(fp);
    if ( true == DtmSjw ) remove("dtmsjw.sjw");

	LenSjx = Points.length()/3;
	lb1.setLogicalLength(LenSjx); 
	lb2.setLogicalLength(LenSjx); 
	lb3.setLogicalLength(LenSjx);
    for (i=0; i<LenSjx; i++) {
		lb1.setAt(i,i*3); lb2.setAt(i,i*3+1); lb3.setAt(i,i*3+2);
	}
	
	//�������С�߳�
	minZ = maxZ = Points.at(0)[Z];
	for (i=1; i<Points.length(); i++) {
		if (Points.at(i)[Z]<minZ) minZ=Points.at(i)[Z];
		if (Points.at(i)[Z]>maxZ) maxZ=Points.at(i)[Z];
	}
	
	//�ɶԻ����ȡ���ò���
	CAcModuleResourceOverride myResources;
	CDrawDzxDlg dlg;
	dlg.m_dMinH = minZ; dlg.m_dMaxH = maxZ;
	dlg.m_nDzxType = m_Type;
	if ( dlg.DoModal() != IDOK ) return;
	deltz = dlg.m_dDeltz; //�ȸ߾�
	if ( TRUE == dlg.m_bSingle ) bSingle = true; //�Ƿ�浥��
	dSingle = dlg.m_dSingleH; //�����ȸ��߸߳�
	m_dSmgc = dlg.m_dSmgc; //ˮ��߳�

#ifdef GD_HANGDAO //�㶫������
	m_dSmgc = 99999.0; //����������
#endif

	m_Nihe = dlg.m_nNihe+1; //��Ϸ�ʽ
	m_StepSize = dlg.m_dStep; //������������
	m_ToleRance = dlg.m_dTolerance; //SPLINE֮���������ݲ�

	if ( 1 == m_Type ) {//�ȸ���
#ifdef JINGSAI_2013
		ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,g_DgxLayer,RTSTR,_T(""),0);
#else
		ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,g_DgxLayer,RTSTR,_T("C"),RTSTR,_T("YELLOW"),RTSTR,_T(""),RTSTR,_T(""),0);
#endif
		ads_command(RTSTR,_T("LINETYPE"),RTSTR,_T("s"),RTSTR,_T("continuous"),RTSTR,_T(""),0);
	}
	else {//������
#ifdef JINGSAI_2013
		ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,g_DsxLayer,RTSTR,_T(""),0);
#else
		ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,g_DsxLayer,RTSTR,_T("C"),RTSTR,_T("YELLOW"),RTSTR,_T(""),RTSTR,_T(""),0);
#endif
		ads_command(RTSTR,_T("LINETYPE"),RTSTR,_T("s"),RTSTR,_T("continuous"),RTSTR,_T(""),0);
	}

/////////////////////////////////////////////////////////////////////////	
	MinCount = (int)(Points.at(0)[Z]); MaxCount = MinCount + 10;
// #ifndef NOSENSE
// 	float fTemp;
// 	BYTE szIn[100],szOut[255],*pb;
// 	DWORD returnlen;
// 	fTemp=(float)minZ; S4_ConvertFloat(&fTemp); pb=szIn+0; memcpy(pb,&fTemp,sizeof(float));
// 	fTemp=(float)maxZ; S4_ConvertFloat(&fTemp); pb=szIn+4; memcpy(pb,&fTemp,sizeof(float));
// 	fTemp=(float)deltz; S4_ConvertFloat(&fTemp); pb=szIn+8; memcpy(pb,&fTemp,sizeof(float));
// 	if ( S4_SUCCESS == S4Execute(&s4_context,"1016",szIn,12,szOut,255,&returnlen) ) 
// 	{ //Dzx
// 		long lTemp;
// 		memcpy(&lTemp,szOut,sizeof(long));
// 		S4_ConvertSignLong(&lTemp);
// 		MinCount = lTemp;
// 		pb=szOut+4; memcpy(&lTemp,pb,sizeof(long));
// 		S4_ConvertSignLong(&lTemp);
// 		MaxCount = lTemp;
// 	}
// #else
	if (minZ < 0) MinCount = (int)(minZ/deltz); 
	else MinCount = (int)(minZ/deltz)+1;
	if (maxZ < 0) MaxCount = (int)(maxZ/deltz)-1; 
	else MaxCount = (int)(maxZ/deltz);
//#endif
/////////////////////////////////////////////////////////////////////////	

	if ( true == m_bNhqm ) {//������淽������
		//DrawDzx_ByNhqm(deltz,bSingle,dSingle);
	}
	else if ( true == bSingle ) {//2004.09.06
		m_bJiqu = false;
		GetDzxStdmWidth(dSingle);
		DrawDzx(dSingle);
	}
	else {
		for (i=MinCount; i<=MaxCount; i++) 
		{
			double dHeight = i * deltz; 
			if ( (i%5) == 0 ) m_bJiqu = true; 
			else m_bJiqu = false;
			GetDzxStdmWidth(dHeight);
			if (1 == m_Type) ads_printf(_T("�ȸ��߸߳�Ϊ %.1f M.\n"),dHeight); 
			else ads_printf(_T("�����߸߳�Ϊ %.1f M.\n"),dHeight); 
		
			DrawDzx(dHeight);
 		}
	}
	ads_command(RTSTR,_T("layer"),RTSTR,_T("s"),RTSTR,_T("0"),RTSTR,_T(""),RTNONE);
	if (2 == m_Type) 
		ads_command(RTSTR,_T("LINETYPE"),RTSTR,_T("s"),RTSTR,_T("continuous"),RTSTR,_T(""),0);
	ads_printf(_T("�������!\n"));
}

void CDrawDzx::gdhd_GetDsxIsland(AcDbObjectIdArray &objArr)
{
	ads_name ssName,ent;
	AcDbObjectId objId;
	long l,Length=0;
	TCHAR szCode[50];
	acedSSGet( _T("X") , NULL , NULL , NULL , ssName );
	ads_sslength(ssName,&Length);
	for (l=0; l<Length; l++)
	{
		ads_ssname(ssName,l,ent);
		if ( GetPolyType(ent,true) < 10 ) continue;
		GetStdm(ent,szCode);
		if ( !_tcscmp(szCode,_T("186309")) ) {
			if ( Acad::eOk == acdbGetObjectId(objId,ent) ) {
				objArr.append(objId);
			}
		}
	}
	ads_ssfree(ssName);
}

////�����������
//void CDrawDzx::DrawDzx_ByNhqm()
//{
//	vector<CVertex*> vVertex;
//	vector<CTriangle*> vTriangle;
//	//vector<vector<CVertex*> > vvEleVer;
//	double *pValue;
//	int i,j,k,nNumber=0;
//
///*
//	//��������3���������ά����洢������pValue��,pValue����ǰ�����ڴ�,n��pValue�Ĵ�С
//	nNumber = Points.length();
//	pValue = new double[nNumber*3];
//	for (i=0; i<nNumber; i++) {
//		pValue[3*i] = Points[i].x;
//		pValue[3*i+1] = Points[i].y;
//		pValue[3*i+2] = Points[i].z;
//	}
//	if ( false == TransformData(vVertex, vTriangle, pValue, nNumber*3, g_dHighBits) ) {
//		ads_printf(_T("\n���ݶ������!\n")); delete[] pValue; return;
//	}
//	delete[] pValue;
//*/
//
//	TCHAR szDatFile[MAX_PATH],szStr[255],szName[50],szCode[50];
//	FILE *fp;
//	ads_point pt;
//	double dDeltz,dSingle;
//	bool bSingle = false; //���Ƶ��ȸ���
//	double dGridWidth=10.0;
//	bool bShowGrid=false;
//
//	//if ( GetCassFileName(_T("����̵߳������ļ���"),g_szUsePath,_T("dat"),0,szDatFile) == false ) return;
//
//	//�ɶԻ����ȡ���ò���
//	CAcModuleResourceOverride myResources;
//	//CDrawDzxDlg dlg;
//	//dlg.m_dMinH = minZ; dlg.m_dMaxH = maxZ;
//	//dlg.m_nDzxType = m_Type;
//	CDrawDzxNhqmDlg dlg;
//
//	if ( dlg.DoModal() != IDOK ) return;
//	dDeltz = dlg.m_dDeltz; //�ȸ߾�
//	if ( TRUE == dlg.m_bSingle ) bSingle = true; //�Ƿ�浥��
//	dSingle = dlg.m_dSingleH; //�����ȸ��߸߳�
//	m_Nihe = dlg.m_nNihe+1; //��Ϸ�ʽ
//	m_StepSize = dlg.m_dStep; //������������
//	m_ToleRance = dlg.m_dTolerance; //SPLINE֮���������ݲ�
//	dGridWidth = dlg.m_dGridWidth; //�������
//	if ( 0 == dlg.m_nShowGrid ) bShowGrid = true; //�Ƿ���ʾ����
//
//	//m_dSmgc = dlg.m_dSmgc; //ˮ��߳�
//	m_dSmgc = 99999.0; //����������
//
//	if ( 0 == dlg.m_nDatType ) {//�����ļ�
//		_stprintf(szDatFile,_T("%s"),dlg.m_sFileName);
//	}
//	else {//ͼ��ˮ���
//		_tcscpy(szDatFile,_T("sjx$.dat"));
//		if ( getgchfile(szDatFile,12,true,true) < 3 ) { //ֻ����ˮ���
//			ads_alert(_T("ͼ��ˮ������")); return;
//		}
//	}
//	fp = _tfopen(szDatFile,_T("rt"));
//	if ( NULL == fp ) {
//		ads_alert(_T("�޷����ļ�")); return;
//	}
//	while ( _fgetts(szStr,255,fp) ) {
//		if ( 4 == ReadDataLine(szStr,szName,szCode,pt) ) {
//			//if ( 0 == nNumber ) { minZ = maxZ = pt[Z]; }
//			//else {
//			//	if ( pt[Z] < minZ ) minZ = pt[Z];
//			//	if ( pt[Z] > maxZ ) maxZ = pt[Z];
//			//}
//			nNumber++;
//		}
//	}
//	fclose(fp);
//
//	if ( dDeltz < 0.01 ) {
//		ads_alert(_T("���̫С")); return; //��಻Ӧ̫С
//	}
//
//	if ( 1 == m_Type ) {//�ȸ���
//		ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,g_DgxLayer,RTSTR,_T("C"),RTSTR,_T("YELLOW"),RTSTR,_T(""),RTSTR,_T(""),0);
//		ads_command(RTSTR,_T("LINETYPE"),RTSTR,_T("s"),RTSTR,_T("continuous"),RTSTR,_T(""),0);
//	}
//	else {//������
//		ads_command(RTSTR,_T("layer"),RTSTR,_T("m"),RTSTR,g_DsxLayer,RTSTR,_T("C"),RTSTR,_T("YELLOW"),RTSTR,_T(""),RTSTR,_T(""),0);
//		ads_command(RTSTR,_T("LINETYPE"),RTSTR,_T("s"),RTSTR,_T("continuous"),RTSTR,_T(""),0);
//	}
//
//	//�������깹��������
//	pValue = new double[nNumber*3+30];
//	i = 0;
//	fp = _tfopen(szDatFile,_T("rt"));
//	while ( _fgetts(szStr,255,fp) ) 
//	{
//		if (4 != ReadDataLine(szStr,szName,szCode,pt)) continue;
//		pValue[3*i] = pt[X]; pValue[3*i+1] = pt[Y]; pValue[3*i+2] = pt[Z];
//		i++;
//	}
//	fclose(fp);
//
//	if ( false == TransformVertex(vVertex, pValue, nNumber*3) ) {
//		ads_printf(_T("\n���ݶ������!\n")); delete[] pValue; return;
//	}
//	delete[] pValue;
//
//	//BuildTriangleNetwork_Insert(vVertex, vTriangle);
//	//DeleteOutlineTriangle(vTriangle); //ɾ����Χ����������
//
//	//nv����ɢ�����
//	int nv = vVertex.size();
//	if (nv < 3)
//	{
//		ads_printf(_T("\n����������!\n")); return;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	//2014-3-14
//	vector<CVertex *> vPV;
//	GetPolygon(vVertex, vPV);
//	BuildPolygonTin(vPV, vTriangle);
//	if (vTriangle.size() > 0)
//	{
//		for (int i = 0; i < (int)vVertex.size(); ++i)
//		{
//			InsertPoint2Tin(vTriangle, vVertex.at(i));
//		}
//		//ResetFlatTriangle(vTriangle);
//	}
//	DeleteOutlineTriangle(vTriangle);//ɾ����Χ����������
//	//////////////////////////////////////////////////////////////////////////
//
//	//�û�����������
//	//double dGridWidth=10.0;
//	//if ( RTNORM != ads_getreal(_T("\n������������ <10��>:"),&dGridWidth) ) dGridWidth = 10.0;
//	//if ( dGridWidth < 0.0001 ) dGridWidth = 10.0;
//	//bool bShowGrid=false;
//	//_tcscpy(szStr,_T("n"));
//	//ads_getstring(0,_T("\n��ʾ����<N>?"),szStr);
//	//if ( !_tcsicmp(szStr,_T("y")) ) bShowGrid = true;
//
//	//������ʼ����
//	//clock_t ct1 = clock();
//	CBlockGrid grids;
//	//grids.SetDXDY(dGridWidth, dGridWidth);
//	//grids.SetGridSize(vTriangle, nv);
//
//	//���õ�
//	AcDbObjectIdArray objArr;
//	gdhd_GetDsxIsland(objArr);
//	vector<bool> vIsInside;
//	vector< vector<CVertex*> > vvPolys;
//	for (i=0; i<objArr.length(); i++)
//	{
//		ads_name ent;
//		AcGePoint2dArray Nodes;
//		acdbGetAdsName(ent,objArr[i]);
//		if ( GetPlList(ent,Nodes) == false ) continue;
//		Nodes.append(Nodes.first());
//		vector<CVertex*> vPoly;
//		for (j=0; j<Nodes.length(); j++) {
//			AcGePoint2d pt2d = Nodes.at(j);
//			CVertex* pV = new CVertex(0, pt2d.x, pt2d.y, 0.0);
//			vPoly.push_back(pV);
//		}
//		vvPolys.push_back(vPoly);
//		vIsInside.push_back(false);
//	}
//	
//	//if ( false == grids.BuildGrid(vVertex,dGridWidth,dGridWidth) ) {
//	//	ads_printf(_T("\n�޷�����ģ��!\n")); return;
//	//}
//	//Լ������
//	//grids.SetValidNode(vvPolys, vIsInside);
//
//
//	//������ʼ����
//	grids.SetBlockGridSize(vVertex, dGridWidth,dGridWidth);
//
//	//Լ������
//	grids.SetValidNode(vTriangle);
//	for (i = 0; i < (int)vvPolys.size(); ++i)
//	{
//		grids.SetValidNode(vvPolys.at(i), false);
//	}
//
//	//�ͷ��ڴ�
//	for (i=0; i<(int)vvPolys.size(); i++) {
//		DeleteVertex(vvPolys.at(i));
//	}
//	vvPolys.clear();
//
//	//�����������
//	if ( false == grids.BuildBlockGrid() ) {
//		ads_printf(_T("\n�޷������������!\n"));
//		DeleteTriangle(vTriangle);
//		DeleteVertex(vVertex);
//		return;
//	}
//
//	//����ƽ�����ε�������ĸ���,20130809
//	vector< vector<CVertex *> > vvPV;
//	GetFlatPolygon(vTriangle, vvPV);
//	for (i = vvPV.size() - 1; i >= 0; --i)
//	{
//		grids.SetPlaneNode(vvPV.at(i), vvPV.at(i).at(0)->m_z);
//	}
//
//	//׷�ٵȸ���
//	vector<CLine *> vLine;
//	double minH, maxH;
//	GetMinMaxZ(vVertex, minH, maxH);
//	grids.TraceContour(vLine, dDeltz, minH, maxH);
//	//grids.GridNetTraceContour(vvEleVer, dDeltz, minH, maxH);
//	//DeleteWrongContour(vvEleVer, grids.m_rtVertex);
//
//	//�޸��ȸ���,20130809
//	//double da = 60.0;
//	//double ds = grids.GetGridAt(0)->GetDX()/2;
//	//double ds = grids.GetGridDX(); //20130814
//
//	double dx = grids.GetBlockGridDX();
//	double dy = grids.GetBlockGridDY();
//	//double ds = dx / 4.0;
//	CRTree rtVertex, rtTriangle;
//	BuildRTree_Vertex(rtVertex, vVertex);
//	BuildRTree_Triangle(rtTriangle, vTriangle);
//	//RepairContourWithElevationPoint(vvEleVer, rtVertex, dx + dy);//����������
//	////ConnetContour(vvEleVer, rtTriangle);//�޸��Ͽ��ĵȸ���
//	//ConnetContour(vvEleVer, ds);//�޸��Ͽ��ĵȸ���,20130814
//	//RepairContourInPlaneTriangles(vvEleVer, vvPV, rtTriangle);//�޸�ƽ����������ĵȸ���
//	//RepairContourNode(vvEleVer, rtVertex, ds, da);//�޸����ڽڵ���ܻ�нǹ�С�����
//	//DeleteWrongContour(vvEleVer, grids.m_rtVertex);//ɾ��û�а����̵߳�ıպϵȸ���
//
//	RepairContourNode(vLine, &rtVertex, dx / 4.0, 60.0);//�޸����ڽڵ���ܻ�нǹ�С�����
//	RepairBreakContour(vLine, (dx + dy) / 2.0);//�޸��Ͽ��ĵȸ���
//	RepairContourInFlatTriangles(vLine, vvPV, &rtTriangle);//�޸�ƽ����������ĵȸ���
//	DeleteWrongContour(vLine, &rtVertex);//ɾ��û�а����̵߳�ıպϵȸ���
//	
//	//��ʾ����
//	if ( true == bShowGrid )
//	{
//		CGrid grid;
//		grids.BlockGrid2Grid(grid);
//		ads_name entTmp;
//		ads_point pt1,pt2;
//		double x1,x2,y1,y2,h1,h2;
//		int ii,jj;
//		int nRow = grid.GetRow();
//		int nCol = grid.GetColumn();
//		//�����߶�
//		for (ii=0; ii<nRow; ii++)//��ѭ��
//		{
//			y1 = y2 = grid.GetGridY(ii);
//			for (jj=1; jj<nCol; jj++)//��ѭ��
//			{
//				x1 = grid.GetGridX(jj-1);
//				h1 = grid.GetGridH(ii, jj-1);
//				x2 = grid.GetGridX(jj);
//				h2 = grid.GetGridH(ii, jj);
//				if (h1 >= INVALID_H_VALUE || h2 >= INVALID_H_VALUE ) continue;
//				pt1[X]=x1; pt1[Y]=y1; pt1[Z]=0.0;
//				pt2[X]=x2; pt2[Y]=y2; pt2[Z]=0.0;
//				drawline(pt1,pt2,_T(""),NULL,_T("GW")); //ads_entlast(entTmp); setcolor(entTmp,249);
//			}
//		}
//
//		//�����߶�
//		for (jj=0; jj<nCol; jj++)//��ѭ��
//		{
//			x1 = x2 = grid.GetGridX(jj);
//			for (ii=1; ii<nRow; ii++)//��ѭ��
//			{
//				y1 = grid.GetGridY(ii-1);
//				h1 = grid.GetGridH(ii-1, jj);
//				y2 = grid.GetGridY(ii);
//				h2 = grid.GetGridH(ii, jj);
//				if (h1 >= INVALID_H_VALUE || h2 >= INVALID_H_VALUE ) continue;
//				pt1[X]=x1; pt1[Y]=y1; pt1[Z]=0.0;
//				pt2[X]=x2; pt2[Y]=y2; pt2[Z]=0.0;
//				drawline(pt1,pt2,_T(""),NULL,_T("GW")); //ads_entlast(entTmp); setcolor(entTmp,249);
//			}
//		}
//	}
//
//	//���Ƶ�ֵ��
//	int ne = vLine.size();
//	int LWeight = get_lw(0.3*hh);
//	for (i=0; i<ne; i++)
//	{
//		double dHeight = vLine.at(i)->m_vNode.at(0)->m_z;
//		if ( true == bSingle ) {//ֻ���Ƶ���
//			if ( fabs(dHeight-dSingle) > 0.001 ) continue;
//			m_bJiqu = false;
//		}
//		int nZheng = (int)((fabs(dHeight)+0.1*dDeltz) / dDeltz);
//		if ( (nZheng % 5) == 0 ) m_bJiqu = true; 
//		else m_bJiqu = false;
//		GetDzxStdmWidth(dHeight); //��ȡm_szStdm,m_dWidth
//
//		AcGePoint2dArray Nodes;
//		AcGePoint2d pt2d;
//		for (k=0; k < (int)vLine.at(i)->m_vNode.size(); k++) {
//			pt2d.x = vLine.at(i)->m_vNode.at(k)->m_x;
//			pt2d.y = vLine.at(i)->m_vNode.at(k)->m_y;
//			Nodes.append(pt2d);
//		}
//		int nSum = Nodes.length();
//		if ( nSum < 5 ) continue;
//		ads_name ent;
//		if ( 1 == m_Nihe || 3 == m_Nihe ) {
//			bool bClose=false;
//			if ( distOF2d(Nodes.first(),Nodes.last()) < 0.0001 ) {
//				Nodes.removeLast();
//				bClose = true;
//			}
//			int nSpline=0;
//			if ( 3 == m_Nihe ) nSpline = 2;
//			DrawPolyline2D(Nodes,dHeight,bClose,m_szStdm,m_dWidth,nSpline);
//			ads_entlast(ent);
//		}
//		else if (4 == m_Nihe) {//SPLINE
//			AcGePoint3d pt3d; 
//			AcGePoint3dArray ptS; ptS.setGrowLength(10);
//			for (k=0; k<nSum; k++) {
//				pt3d[X]=Nodes[k].x; pt3d[Y]=Nodes[k].y; pt3d[Z]=dHeight;
//				ptS.append(pt3d);
//			}
//			AcDbSpline *pPline = new AcDbSpline(ptS,4,m_ToleRance);
//			if (true == m_bJiqu) pPline->setLineWeight((AcDb::LineWeight)LWeight);
//			AcDbObjectId objId;
//			AppendEntity(pPline,objId);
//			pPline->close();
//			ads_entlast(ent);
//			addapp(ent,m_szStdm);
//		}
//		else if (2 == m_Nihe) {//��������
//			double *xd0,*yd0;
//			xd0 = new double[nSum+5];
//			yd0 = new double[nSum+5];
//			xd0[0] = Nodes[0].x; yd0[0] = Nodes[0].y;
//			for (k=1; k<=nSum; k++) {
//				xd0[k] = Nodes[k-1].x; yd0[k] = Nodes[k-1].y;
//			}
//			int nBikai = 1;
//			if ( distOF2d(Nodes.first(),Nodes.last()) < 0.0001 ) {
//				nBikai = 2;
//				nSum++; xd0[nSum]=Nodes[1].x; yd0[nSum]=Nodes[1].y; //���պϣ���ĩβ�����2��
//			}
//			zlyth(nSum,xd0,yd0,nBikai,dHeight);
//			delete []xd0; delete []yd0;
//			ads_entlast(ent);
//		}
//		 
//#ifdef GD_HANGDAO
//		if ( 2 == m_Type ) {
//			if (true == m_bJiqu) setcolor(ent,1);
//			if ( fabs(dHeight) > 0.0001 ) setlinetype(ent,_T("x11"));
//			continue;
//		}
//#endif
//		if (true == m_bJiqu) setcolor(ent,3);
//	}
//
///*
//	clock_t ct5 = clock();
//	float t4 = (float)(ct5 - ct4)/CLOCKS_PER_SEC;
//	float t5 = (float)(ct5 - ct1)/CLOCKS_PER_SEC;
//	cout<<setiosflags(ios::fixed)<<setprecision(1);
//	cout<<"��ɢ���� "<<nv<<endl;
//	cout<<"����������ʱ "<<t1<<" ��"<<endl;
//	cout<<"Լ��������ʱ "<<t2<<" ��"<<endl;
//	cout<<"����������Ϻ�ʱ "<<t3<<" ��"<<endl;
//	cout<<"׷�ٵȸ��ߺ�ʱ "<<t4<<" ��"<<endl;
//	cout<<"�ܺ�ʱ "<<t5<<" ��"<<endl;
//	cout<<"�ȸ������� "<<vvEleVer.size()<<endl;
//	cout<<endl;
//*/
//
//	//�ͷ��ڴ�
//	DeleteTriangle(vTriangle);
//	DeleteVertex(vVertex);
//	DeleteLine(vLine);
//
//	ads_command(RTSTR,_T("layer"),RTSTR,_T("s"),RTSTR,_T("0"),RTSTR,_T(""),RTNONE);
//	if (2 == m_Type) 
//		ads_command(RTSTR,_T("LINETYPE"),RTSTR,_T("s"),RTSTR,_T("continuous"),RTSTR,_T(""),0);
//	ads_printf(_T("�������!\n"));
//}

void CDrawDzx::ReadFromArray(AcGePoint3dArray &SanJiao)
{
	for (int i=0; i<SanJiao.length(); i++) 
	{
		AcGePoint3d pt3d = SanJiao.at(i); 
		Points.append(pt3d);
		if ( 0 == i%3 ) lb1.append(i);
		else if ( 1 == i%3 ) lb2.append(i);
		else if ( 2 == i%3 ) lb3.append(i);
	}
}

