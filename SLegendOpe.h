#pragma once
#include "SBreakBlock.h"
#include "Selection_String.h"
#include "SNodesOpe.h"
#include "tinyxml.h"
#include "EditListCtrl.h"
#include "Layers.h"
#include "SWipeOut.h"

class SLegendOpe
{
public:
	SLegendOpe(void);
	virtual ~SLegendOpe(void);

	set<CString>		m_sCodes;
	AcDbObjectIdArray	m_objArr;

	AcGePoint2d		m_ptBorderLB;
	AcGePoint2d		m_ptBorderRT;

	int nInterval_width, nInterval_Height;			//块与块之间的间隙
	int				m_scaleBlock;
	double dWidth_Block, dHeight_block;			//块长，高
	int nArrangeType;
	int nMaxCount;
	bool					m_bExplodeBlocks;				//是否炸开图例小块，默认不炸开

	double dFrameWidth_Gap, dFrameHeight_Gap;		//边框与块之间的间隙
	TCHAR szBorderLineType[200];							//图例边框线条样式
	double dWidth_Frame;									//图例边框线条宽度
	int			sBorderColor;									//边框颜色，索引色

	double dWidth_Tittle_Temp;					//图例头宽度
	double dWidth_Buttom_Temp;					//图例脚宽度
	TCHAR szLegendTittleName[200];
	TCHAR szLegendButtomName[200];

	int							m_nDrawFrameOut;		//是否绘制外边框

	short		sInsertPosition;
	short		sBlank;

	AcDbObjectIdArray			m_objArrLegend;		//保存图例的所有ID
	AcDbObjectIdArray			m_objArrBlocks;			

	void		setHH(double dHH = 1.0);

	//传入要生成图例的实体
	bool		setEntitys(AcDbObjectIdArray objArr);

	//插入图例
	bool		insertLegendTable(AcGePoint2d ptInsert);

	//读取图例配置文件
	bool		readLegendConfig(TCHAR *szFileName);
private:
	AcDbObjectIdArray			m_objArrMove;
	vector<CString>				m_vStrBlockNames;
	double							m_dhh;				//已除以1000

	//过滤出存在图例块模板的实体
	bool		filterUseLegendBlock();
};

