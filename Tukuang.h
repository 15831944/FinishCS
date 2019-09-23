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

	double							m_dhh;				//ͼ�α����ߣ����½ǵ�
	double							m_dhhFrame;		//ͼ������ߣ��û���ͼʱ���õģ��ѳ���1000

	bool								m_bMulty;
	bool								m_bLast;

	AcGePoint2d					m_pt2dLBTK;		//ͼ�����½ǲ����
	AcGePoint2d					m_pt2dRTTK;		//ͼ�����Ͻǲ����
	AcGePoint2d					m_pt2dWLBTK;	//��ͼ�����½�
	AcGePoint2d					m_pt2dWRTTK;	//��ͼ�����Ͻ�

	AcDbObjectId					m_objSummaryTable;
	AcDbObjectId					m_objInnerFrame;

	FloorStaticInfo				m_floorInfo;

	vector<CString>				m_vStrKey, m_vStrValue;

	//����뻧�������ƽ��ͼ
	bool  startIndoorFrame();

	//��������ֲ�ͼ
	bool	startFloorPlane();

	//������������ֲ�ͼ
	bool	startFloorPlane_Multy();

	//���������ƽ��ͼ
	bool	startTotal_Floor_Plan();

	bool  insertTable(AcGePoint3d ptRb);

	//�������½ǻ��ܱ�����ͼר��
	bool sumAreaAndinsertTable(ads_point pt);

	bool insertTittle_FloorPlane(ads_point pt);

	bool insertLegen(ads_point pt1, ads_point pt2, ads_point ptInsert);

	bool insertLegend_Total_Floor_Plan(ads_point pt1, ads_point pt2, ads_point ptInsert);

	bool explodeTableAndDeleteLine(AcDbObjectId objBlock);
	bool explodeTableAndDeleteLine(AcDbObjectIdArray objArr);

	bool		showMatchFieldDlg();

	//����ͼ�����������ͼ��������
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

	//��Χ���и����ͼ�������õ���ͼ�ڣ����
	bool		trimLand();

	bool		saveBuildingInfo();

	bool		saveBuildingInfo(AcDbObjectId &objIdKey);

	//���ͼ���������ͼע�ǣ��ж���Щ���û��Լ����Ƴ����ģ���ֹ�û�����������ɵ����ע��
	bool		checkSameGUID();

};

