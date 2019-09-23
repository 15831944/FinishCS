#pragma once
#include "StdAfx.h"

struct DrawThingsData 
{
	//入户调查，文字注记用
	CString  strCode;
	CString	strNote;
	CString	strNoteText;
	double	dTextHeight;
	double	dTextWidth;
	CString	strTextStyle;

	//绘制地物必填字段用
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
	short sType;		//1 条件点  2 放线点
	short sType2;		//1 注记两个坐标 2 注记一个坐标
	Parsing_Detecting()
	{
		dX1 = dX2 = dY1 = dY2 = 0.0;
		sType = 0;
		sType2 = 0;
	}
};

struct ElevationInfo
{
	CString	strFloorName;		//楼号
	CString	strName;			//标注名称
	CString	strPointName;		//点号
	double	dHeight;
};

//CSV输出要素表时用，要素表输出汇总表时用
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
	CString strName;		//楼层名
	double dParam;		//系数
	double dArea;			//原始面积
	double dArea2;		//计算面积
	CString strArea;		//取整后的面积

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
	CString strName;		//附属名
	double dParam;		//系数
	double dArea;			//原始面积
	double dArea2;		//计算面积
	CString strArea;		//取整后的面积
	CString strArea2;		//取整后乘以系数的面积

	//bool		bDivideArea;

	AttachArea()
	{
		dParam = 0.0;
		dArea = 0.0;
		dArea2 = 0.0;
	//	bDivideArea = false;

	}
};

//建筑分层图用
struct FloorStaticInfo
{
	CString	strBuildName;		//建筑名
	double   dBuildArea;		//原始建筑面积
	double	dBuildAreaAll;		//单层总建筑面积

	CString	strArea;				//单层取整后的总面积(没有附属地物)
	CString	strArea2;			//单层取整后跟附属地物取整的总面积
	CString	strAreaAll;			//单层取整后总面积乘以层数的总面积

	double	dParam;				//建筑面积系数
	short		sTopNum;
	short		sButtomNum;
	short		dFloorNum;

	//地上，地下建筑面积小计
	double dAreaUp_Public;		//地上公共
	double dAreaUp_Private;		//地上私人
	double dAreaUnder_Public;	//地下公共
	double dAreaUnder_Private;	//地下私人

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
	TCHAR name[64]; //字段名
	int type; //字段类型
	int length; //字段长度
	int decimal; //小数位数
	TCHAR text[256]; //字段文字说明
	TCHAR dict[64]; //数据字典表名, 90新标准,字典从MDB表中读.
	int check;	//CASS6长沙用到，具体用途不详,见Global.h GetBriefAttrInfoEx函数
	bool bMust;    //是否是必填通过$来确定
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
	TCHAR table[40]; //表名
	int type; //表类型
	TCHAR alias[20]; //长沙版
	TCHAR text[80]; //表说明
	CArray<ATTRFIELD,ATTRFIELD> AttrArr; //各个字段
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
	CString xh;		//序号
	CString mc;		//名称
	CString ms;		//描述
	CString wz;		//位置
	CString lj;		//路径
};