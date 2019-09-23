#pragma once
#include "StdAfx.h"

struct DrawThingsData 
{
	//�뻧���飬����ע����
	CString  strCode;
	CString	strNote;
	CString	strNoteText;
	double	dTextHeight;
	double	dTextWidth;
	CString	strTextStyle;

	//���Ƶ�������ֶ���
	CString	strName;		
	CString	strText;
	CString	strMJXS;
	CString	strType;
	CString  strValue;

	DrawThingsData()
	{
		dTextHeight = 1.0;
		dTextWidth = 1.0;
	}
};

struct SMustFillField 
{
	CString strCode;

	vector<DrawThingsData> vFields;
};

struct DefaultAreaParam 
{
	CString strCode;
	double dValue;
	DefaultAreaParam()
	{
		dValue = 1.0;
	}
};

struct Parsing_Detecting
{
	CString		strName;
	double dX1, dX2, dY1, dY2;
	short sType;		//1 ������  2 ���ߵ�
	short sType2;		//1 ע���������� 2 ע��һ������
	Parsing_Detecting()
	{
		dX1 = dX2 = dY1 = dY2 = 0.0;
		sType = 0;
		sType2 = 0;
	}
};

struct ElevationInfo
{
	CString	strFloorName;		//¥��
	CString	strName;			//��ע����
	CString	strPointName;		//���
	double	dHeight;
};

//CSV���Ҫ�ر�ʱ�ã�Ҫ�ر�������ܱ�ʱ��
struct CSVInfo 
{
	CString		strFloorName;
	vector<ElevationInfo>  vIndoorHeight;
	vector<ElevationInfo>	 vDaughterHeight;
	vector<ElevationInfo>	 vMachineHeight;
	vector<ElevationInfo>	 vOtherHeight;

	double		dArea_Tol;
	double		dArea_TolTop;
	double		dArea_TolUnder;
	int			dFloorCount_Top;
	int			dFloorCount_Under;
	CSVInfo()
	{
		dArea_TolUnder = 0.0;
		dArea_Tol = 0.0;
		dArea_TolUnder = 0.0;
		dFloorCount_Top = 0;
		dFloorCount_Under = 0;
	}

};

struct MainBuild
{
	CString strName;		//¥����
	double dParam;		//ϵ��
	double dArea;			//ԭʼ���
	double dArea2;		//�������
	CString strArea;		//ȡ��������

	//bool		bDivideArea;

	MainBuild()
	{
		dParam = 0.0;
		dArea = 0.0;
		dArea2 = 0.0;
		//bDivideArea = false;
	}
};

struct AttachArea
{
	CString strName;		//������
	double dParam;		//ϵ��
	double dArea;			//ԭʼ���
	double dArea2;		//�������
	CString strArea;		//ȡ��������
	CString strArea2;		//ȡ�������ϵ�������

	//bool		bDivideArea;

	AttachArea()
	{
		dParam = 0.0;
		dArea = 0.0;
		dArea2 = 0.0;
	//	bDivideArea = false;

	}
};

//�����ֲ�ͼ��
struct FloorStaticInfo
{
	CString	strBuildName;		//������
	double   dBuildArea;		//ԭʼ�������
	double	dBuildAreaAll;		//�����ܽ������

	CString	strArea;				//����ȡ����������(û�и�������)
	CString	strArea2;			//����ȡ�������������ȡ���������
	CString	strAreaAll;			//����ȡ������������Բ����������

	double	dParam;				//�������ϵ��
	short		sTopNum;
	short		sButtomNum;
	short		dFloorNum;

	//���ϣ����½������С��
	double dAreaUp_Public;		//���Ϲ���
	double dAreaUp_Private;		//����˽��
	double dAreaUnder_Public;	//���¹���
	double dAreaUnder_Private;	//����˽��

	vector<MainBuild>	 vMainBuild;
	vector<AttachArea> vAttach;

	FloorStaticInfo()
	{
		dBuildArea = 0.0;
		dBuildAreaAll = 0.0;
		dParam = 0.0;
		dFloorNum = 1;
		sTopNum = 0;
		sButtomNum = 0;

		dAreaUnder_Private = 0.0;
		dAreaUnder_Public = 0.0;
		dAreaUp_Private = 0.0;
		dAreaUp_Public = 0.0;
	}
};

struct ATTRFIELD
{
	TCHAR name[64]; //�ֶ���
	int type; //�ֶ�����
	int length; //�ֶγ���
	int decimal; //С��λ��
	TCHAR text[256]; //�ֶ�����˵��
	TCHAR dict[64]; //�����ֵ����, 90�±�׼,�ֵ��MDB���ж�.
	int check;	//CASS6��ɳ�õ���������;����,��Global.h GetBriefAttrInfoEx����
	bool bMust;    //�Ƿ��Ǳ���ͨ��$��ȷ��
	ATTRFIELD()
	{
		_tcscpy(name , _T(""));
		int type = -1;
		length = 0;
		decimal = 0;
		_tcscpy(text , _T(""));
		_tcscpy(dict , _T(""));
		check = -1;
		bMust = false;
	}
} ;

struct ATTRINFO{
	TCHAR table[40]; //����
	int type; //������
	TCHAR alias[20]; //��ɳ��
	TCHAR text[80]; //��˵��
	CArray<ATTRFIELD,ATTRFIELD> AttrArr; //�����ֶ�
	ATTRINFO()
	{
		type = 0;
		RtlZeroMemory(table, sizeof(table));
		RtlZeroMemory(alias, sizeof(alias));
		RtlZeroMemory(text, sizeof(text));
	}

};

struct PICTUREINFO
{
	CString xh;		//���
	CString mc;		//����
	CString ms;		//����
	CString wz;		//λ��
	CString lj;		//·��
};