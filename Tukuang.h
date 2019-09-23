#pragma once
#include "SBreakBlock.h"
#include "Global.h"
#include "XRecordOperate.h"
#include "Layers.h"
#include "tinyxml.h"
#include "EditListCtrl.h"
#include "MatchFialogsEx.h"
#include "SLegendOpe.h"
#include "SFilter.h"
#include "Tukuang_Common.h"
#include "Dialogs.h"
#include "SDimensionOpe.h"

class CTukuang
{
public:
	CTukuang(void);
	virtual ~CTukuang(void);

	double							m_dhh;				//图形比例尺，左下角的
	double							m_dhhFrame;		//图框比例尺，用户出图时设置的，已除以1000

	bool								m_bMulty;
	bool								m_bLast;

	AcGePoint2d					m_pt2dLBTK;		//图框左下角插入点
	AcGePoint2d					m_pt2dRTTK;		//图框右上角插入点
	AcGePoint2d					m_pt2dWLBTK;	//外图框左下角
	AcGePoint2d					m_pt2dWRTTK;	//外图框右上角

	AcDbObjectId					m_objSummaryTable;
	AcDbObjectId					m_objInnerFrame;

	FloorStaticInfo				m_floorInfo;

	vector<CString>				m_vStrKey, m_vStrValue;

	//输出入户调查测量平面图
	bool  startIndoorFrame();

	//输出建筑分层图
	bool	startFloorPlane();

	//批量输出建筑分层图
	bool	startFloorPlane_Multy();

	//输出竣工总平面图
	bool	startTotal_Floor_Plan();

	bool  insertTable(AcGePoint3d ptRb);

	//插入右下角汇总表格，面积图专用
	bool sumAreaAndinsertTable(ads_point pt);

	bool insertTittle_FloorPlane(ads_point pt);

	bool insertLegen(ads_point pt1, ads_point pt2, ads_point ptInsert);

	bool insertLegend_Total_Floor_Plan(ads_point pt1, ads_point pt2, ads_point ptInsert);

	bool explodeTableAndDeleteLine(AcDbObjectId objBlock);
	bool explodeTableAndDeleteLine(AcDbObjectIdArray objArr);

	bool		showMatchFieldDlg();

	//按照图框比例尺缩放图框内文字
	bool		scaleTextsInFrame();

	bool		refreshFrameData();

	bool		refreshSingleFrame(AcDbObjectId objFrame);

	bool		refreshBatFrame(AcDbObjectId objFrame, AcDbObjectIdArray objArrTK);

	void		findBatAndLastFrame(AcDbObjectId objFrame, AcDbObjectIdArray &objArrBat, AcDbObjectId &objLast);

private:
	ads_point  m_ptminLegen, m_ptmaxLegen;
	ads_point	m_ptminTable, m_ptmaxTable;

	bool				m_bHaveLegend;
	CString			m_strJZMC;
	double			m_dTol;
	double			m_dTolTop;
	double			m_dTopUnder;
	int					m_nFloorCountTop;
	int					m_nFloorCountUnder;
	int					m_nFoorCountTop_Mul;
	int					m_nFloorCountUnder_Mul;
	CString			m_strBat_Id;

	bool				m_bDiveByFrame;
	int				m_sAutoLegend;

	AcDbObjectId		m_objIdDiv;

	bool		insertPrintPoint();

	bool		summaryTolArea();

	void		resetTolArea();

	bool		insertSummarytTable(ads_point pt);

	bool		drawInnerFrame();

	bool		combineElevationPoints();

	void		getFloorCount_Top_Under();

	bool		checkDivLine(AcDbObjectIdArray objArrLine);

	//范围线切割地形图，并设置地形图内，外层
	bool		trimLand();

	bool		saveBuildingInfo();

	bool		saveBuildingInfo(AcDbObjectId &objIdKey);

	//检查图上所有面积图注记，判断哪些是用户自己复制出来的，禁止用户复制软件生成的面积注记
	bool		checkSameGUID();

};

