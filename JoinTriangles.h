// JoinTriangles.h: interface for the CJoinTriangles class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOINTRIANGLES_H__AE8C0608_4BAE_48E9_8215_F7954C6C3F77__INCLUDED_)
#define AFX_JOINTRIANGLES_H__AE8C0608_4BAE_48E9_8215_F7954C6C3F77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJoinTriangles  
{
public:
	BOOL m_bDixing; //����������ʱ�Ƿ��ֹ�ѡ�������
	bool m_bKan; //�Ƿ��ǿ���
	int m_DrawType;
	double m_MinAngle; //��������С��
	bool m_bOnlyDat; //����������ʱ�Ƿ�ֻ������������꣬����ͼ������������

	void GetPointAt(int i,ads_point pt);
	void GetExtremHeight(double &dMinH,double &dMaxH);
	void WriteToArray(AcGePoint3dArray &SanJiao);
	bool ReadSjwFile(TCHAR *datFile); //����SJW�ļ�
	bool cz1(ads_point ptt);
	bool cz2(ads_point ptt);
	void appendsjx();
	void start();
	void sanjiao(double dMinAngle=0.5); //�̲߳�ֵ��
	void sanjiao_();
	bool sanjiaoNodes(AcGePoint3dArray &Nodes); //�����������AcGePoint3dArray
	bool sanjiaoPick(AcGePoint3dArray &Nodes,ads_name ssName); //����������
	void SetMinAngle(double dAngle);
	void ReleaseMemory(); //����ڴ�

	TCHAR m_datFile[MAX_PATH];
	int m_Type; //1,��ͨ������ 2,ɾ����ʱ 3,��߳�
	int m_LenSjx; //���ɵ������θ���,0<=sjx<m_LenSjx
	int m_LenXyz; //��������������,0<=xyz<=m_LenXyz
	AcGeIntArray dxArray; //����������
	short m_nColor; //��������ɫ
	bool m_bSjwPick; //����������������

	CJoinTriangles();
	virtual ~CJoinTriangles();

private:
	void EraseOutside();
	bool Get1040(ads_name ent,double &d1040);
	void Dgx_Dixing();
	bool ReadDatFile();
	bool ReadNodes(AcGePoint3dArray &Nodes);
	int GetNumFromGcd(double x,double y,int nUpper);
	void Dgx_Kangao(int BeforeKan);
	void Join_Sjw(int BeginSjx,int ThiEdge,int TwoEdge);
	void WriteSjwFile(FILE *out,int llb1,int llb2,int llb3);
	void Drawsjx(ads_point pt1,ads_point pt2,ads_point pt3);
	bool KanInter(int Number1,int Number2,ads_point *pointEx=NULL);
	short WhichSide(ads_point pt1,ads_point pt2,ads_point pt3);
	int Sjx_CheckArray(AcGePoint3dArray& ptArray,ads_point pt);
	bool SjwSeekEdge(int Vertex1,int Vertex2,AcGePoint3dArray& SjwArray,int Num);
	short Sjx_InsideSjx(AcGePoint3d pt, ads_point pt1, ads_point pt2, ads_point pt3);
	bool ReadDxArray(); //��dxarray����Doukan
	bool SjxInter(int Cur,int n1,int n2,int n3,ads_point *pp);
	int seeksjx(int n1,int n2,int n3,int j,ads_point *pointEx=NULL);
	
/////////////////////////////////////////////////////////////////////////////////////////////
//�ظ���Ϊ�˼���,2004.04.12
	void TheThirdSanjiao(int n1,int n2,int n3,bool IfNum,int &j,FILE *fp);
	//int seeksjx1(int n1,int j,int n2,int n3,ads_point *pointEx=NULL); //dog
	bool SjxInter1(int n1,int n2,int n3,int Cur,ads_point *pp); //dog
/////////////////////////////////////////////////////////////////////////////////////////////
//#ifdef CSKCSJY
	CArray <AcGePoint3dArray,AcGePoint3dArray> m_DxingArr;
	int m_nDxingPointNum; 
	AcDbObjectIdArray m_OldKanArr;
	void threeD_SeekDxing();
	void threeD_AddDxing(AcGePoint3dArray &pt3dArr,int BeforeKan);
	void threeD_InsertHeightOnDxing(AcGePoint3dArray &pt3dArr,AcGeDoubleArray &distArr);
	void threeD_InsertOnQuxian(AcGePoint3dArray &Nodes,AcGeDoubleArray &Bulges,AcGePoint3dArray &pt3dArr,AcGeDoubleArray &distArr);
	bool threeD_OneGcdHouse(ads_name entHouse,AcGePoint3dArray &pt3dArr);
	void old_OneKanGao(ads_name ent,int BeforeKan);
//#endif
	
	ads_point *point;
	int *llb1;
	int *llb2;
	int *llb3;
	int *Doukan;
	int m_Kanshu;
	ads_name m_ssJiaName;
};


class CDrawDzx  
{
public:
	void ReadFromArray(AcGePoint3dArray &SanJiao);
	void draw();
	void DrawDzx(double dHeight);
	void DrawDzx_ByNhqm();
	int m_Type;
	TCHAR m_szStdm[50];
	bool m_bNhqm;

	CDrawDzx();
	virtual ~CDrawDzx();

private:
	int get_lw(double dKuan);
	double zlyth_dist(double x1,double x2,double y1,double y2);
	void pre_zlyth(int nTotal,double *xd0,double *yd0,int &nResult,double *x,double *y);
	void zlyth(int nTotal,double *xd0,double *yd0,int bikai,double dHeight);
	void GetDzxStdmWidth(double dHeight);
	void gdhd_GetDsxIsland(AcDbObjectIdArray &objArr);

	double hh;
	int oldPick;
	double m_StepSize;
	double m_ToleRance;
	bool m_bJiqu;
	AcGePoint3dArray Points;
	AcGeIntArray lb1;
	AcGeIntArray lb2;
	AcGeIntArray lb3;
	int m_Nihe;
	double m_dSmgc;
	double m_dWidth;
};

#endif // !defined(AFX_JOINTRIANGLES_H__AE8C0608_4BAE_48E9_8215_F7954C6C3F77__INCLUDED_)
