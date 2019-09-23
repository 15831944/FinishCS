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

	int nInterval_width, nInterval_Height;			//�����֮��ļ�϶
	int				m_scaleBlock;
	double dWidth_Block, dHeight_block;			//�鳤����
	int nArrangeType;
	int nMaxCount;
	bool					m_bExplodeBlocks;				//�Ƿ�ը��ͼ��С�飬Ĭ�ϲ�ը��

	double dFrameWidth_Gap, dFrameHeight_Gap;		//�߿����֮��ļ�϶
	TCHAR szBorderLineType[200];							//ͼ���߿�������ʽ
	double dWidth_Frame;									//ͼ���߿��������
	int			sBorderColor;									//�߿���ɫ������ɫ

	double dWidth_Tittle_Temp;					//ͼ��ͷ���
	double dWidth_Buttom_Temp;					//ͼ���ſ��
	TCHAR szLegendTittleName[200];
	TCHAR szLegendButtomName[200];

	int							m_nDrawFrameOut;		//�Ƿ������߿�

	short		sInsertPosition;
	short		sBlank;

	AcDbObjectIdArray			m_objArrLegend;		//����ͼ��������ID
	AcDbObjectIdArray			m_objArrBlocks;			

	void		setHH(double dHH = 1.0);

	//����Ҫ����ͼ����ʵ��
	bool		setEntitys(AcDbObjectIdArray objArr);

	//����ͼ��
	bool		insertLegendTable(AcGePoint2d ptInsert);

	//��ȡͼ�������ļ�
	bool		readLegendConfig(TCHAR *szFileName);
private:
	AcDbObjectIdArray			m_objArrMove;
	vector<CString>				m_vStrBlockNames;
	double							m_dhh;				//�ѳ���1000

	//���˳�����ͼ����ģ���ʵ��
	bool		filterUseLegendBlock();
};

