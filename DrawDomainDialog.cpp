// DrawDomainDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawDomainDialog.h"
#include "afxdialogex.h"
#include "Global.h"
#include "domainoperate.h"

FunctionNameTableEx g_fntEx;

// DrawDomainDialog 对话框

IMPLEMENT_DYNAMIC(DrawDomainDialog, CDialog)

DrawDomainDialog::DrawDomainDialog(CWnd* pParent /*=NULL*/)
: CDialog()
{
	g_fntEx.m_init = false;
}

DrawDomainDialog::~DrawDomainDialog()
{

}

void DrawDomainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IMPORTBH, m_imPortBH);
	DDX_Control(pDX, IDC_COMBO_JRFACTOR, m_jrFactor);
	DDX_Control(pDX, IDC_COMBO_DOMAINNAME, m_domainName);
	DDX_Control(pDX, IDC_COMBO_AREAFACTOR, m_areaFactor);
	DDX_Control(pDX, IDC_CHECK_AREAKOUCHU, m_areaKouChu);
	DDX_Control(pDX, IDC_COMBO_SELECTSTYLE, m_selectStyle);
	DDX_Control(pDX, IDC_EDIT_DOMAINHEIGHT, m_domainHeight);
}

BEGIN_MESSAGE_MAP(DrawDomainDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BEGINDRAW, &DrawDomainDialog::OnBnClickedButtonBegindraw)
	ON_BN_CLICKED(IDC_CHECK_SELECTBH, &DrawDomainDialog::OnBnClickedCheckSelectbh)
	ON_CBN_SELCHANGE(IDC_COMBO_DOMAINNAME, &DrawDomainDialog::OnCbnSelchangeComboDomainname)
END_MESSAGE_MAP()

// DrawDomainDialog 消息处理程序

BOOL DrawDomainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	IDataBaseOper dbOper;
	m_domainName.AddString(_T(" "));
	dbOper.readFunctionTable(m_fnts);
	for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
	{
		FunctionNameTable fnt = m_fnts[idx];
		m_domainName.AddString(fnt.name);
	}
	m_selectStyle.AddString(_T("点选"));
	m_selectStyle.AddString(_T("边界"));
	m_selectStyle.AddString(_T("手动绘制"));

	m_selectStyle.SetCurSel(0);
	this->MoveWindow(0, 0, 400, 30);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void DrawDomainDialog::OnBnClickedCheckSelectbh()
{
	// TODO:  在此添加控件通知处理程序代码
}

void DrawDomainDialog::setControlStatus(BOOL flag)
{
	m_imPortBH.EnableWindow(flag);
	m_jrFactor.EnableWindow(flag);
	m_areaKouChu.EnableWindow(flag);
	m_domainName.EnableWindow(flag);
	m_areaFactor.EnableWindow(flag);
	m_selectStyle.EnableWindow(flag);
	m_domainHeight.EnableWindow(flag);
}

void DrawDomainDialog::OnCbnSelchangeComboDomainname()
{
	// TODO:  在此添加控件通知处理程序代码
	CString domainName;	int curSel = m_domainName.GetCurSel();
	m_domainName.GetLBText(curSel, domainName);	//获取CComBox下拉框当前选中的值
	for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
	{
		FunctionNameTable fnt = m_fnts[idx];
		if(domainName.CompareNoCase(fnt.name)) continue;
		m_jrFactor.SetWindowText(fnt.jrxs);
		m_areaFactor.SetWindowText(fnt.mjxs);
	}
}

void DrawDomainDialog::OnBnClickedButtonBegindraw()
{
	// TODO:  在此添加控件通知处理程序代码
	setControlStatus(FALSE);
	DomainOperate domainOperate; 
	acDocManager->lockDocument(curDoc());//锁定当前DWG文档
	acedGetAcadDockCmdLine()->SetFocus();
	m_areaFactor.GetWindowText(m_mjxs);//获取面积系数输入框中的值
	m_mjxs.Trim();//去除首尾所有空格
	if (m_mjxs.GetLength() == 0 || m_mjxs.GetLength() != m_mjxs.SpanIncluding(_T("0123456789.")).GetLength())//判断面积系数输入值是否符合规范
	{
		CString cstrError = _T("面积系数输入不符合规范！");
		if (m_mjxs.IsEmpty() == true)cstrError = _T("请输入面积系数");
		MessageBox(cstrError, _T("错误信息"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	m_jrFactor.GetWindowText(m_jrxs);//获取计容系数输入框中的值
	m_jrxs.Trim();//去除首尾所有空格
	if (m_jrxs.GetLength() != m_jrxs.SpanIncluding(_T("0123456789.")).GetLength())//判断计容系数输入值是否符合规范
	{
		CString cstrError = _T("计容系数输入不符合规范！");
		if (m_jrxs.IsEmpty() == true)cstrError = _T("请输入计容系数");
		MessageBox(cstrError, _T("错误信息"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	CString selsty;//拾取实体的方式
	m_selectStyle.GetWindowText(selsty);//获取拾取方式输入框中的值
	selsty.Trim();//去除首尾所有空格
	if (selsty.GetLength() == 0 || selsty.GetLength() != selsty.SpanIncluding(_T("边界点选手动绘制")).GetLength())
	{
		CString cstrError = _T("没有该拾取方式，请重新选择！");
		if (selsty.IsEmpty() == true)cstrError = _T("请选择功能区绘制方式");
		MessageBox(cstrError, _T("错误信息"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	CString gd;
	m_domainHeight.GetWindowText(gd);//获取功能区高度输入框中的值
	gd.Trim();//去除首尾所有空格
	if (gd.GetLength() == 0 || gd.GetLength() != gd.SpanIncluding(_T("0123456789.")).GetLength())
	{
		CString cstrError = _T("功能区高度输入不规范！");
		if (gd.IsEmpty() == true)cstrError = _T("请输入功能区高度");
		MessageBox(cstrError, _T("错误信息"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	CString name;//功能区名称
	m_domainName.GetWindowText(name);
	name.Trim();//功能区名称首尾不允许有空格
	CString bh;//功能区编号
	m_imPortBH.GetWindowText(bh);
	bh.Trim();
	if (bh.GetLength() != bh.SpanIncluding(_T("0123456789")).GetLength())
	{
		acutPrintf(_T("\n功能区编号必须为纯数字编号"));
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	BOOL isFirstDraw = true;
	while(TRUE)
	{
		bool flag = true;//true：功能区拾取成功；false：取消拾取功能区
		m_drawStyle = m_selectStyle.GetCurSel();
		AcDbObjectId gnqid;//选择的多段线ID号
		switch(m_drawStyle)
		{
			case 0: flag = domainOperate.createDomainByBoundary(gnqid); break;
			case 1: flag = domainOperate.createDomainByPointSel(gnqid); break;
			case 2: flag = domainOperate.createDomainByDraw(gnqid); break;
		}
		if (!flag)//取消拾取功能区，直接返回
		{
			acDocManager->unlockDocument(curDoc());
			setControlStatus(TRUE);
			return;
		}
		if(name.IsEmpty())
		{
			do
			{
				ads_name txtent;
				ads_point txtpt;
				int ret = ads_entsel(_T("\n请拾取功能区名称："), txtent, txtpt);
				TCHAR enttype[255] = { 0 };
				if (RTNORM == ret)//实体拾取成功，判断实体类型是否符合要求
				{
					if (!GetEntityType(txtent, enttype))
					{
						acutPrintf(_T("\n无法判断是否为文本，请重新拾取"));
						continue;
					}
					if (_tcscmp(enttype, _T("TEXT")) != 0 && _tcscmp(enttype, _T("MTEXT")) != 0)
					{
						acutPrintf(_T("\n请拾取文本"));
						continue;
					}
					else
					{
						TCHAR txt[255] = { 0 };
						GetEntText(txtent, txt);
						name = txt;
						break;
					}
				}
				else
				{
					acutPrintf(_T("\n已取消拾取功能区名称"));
					break;
				}
			} while (TRUE);
		}
		if(bh.IsEmpty())
		{
			do
			{
				ads_name txtent;
				ads_point txtpt;
				int ret = ads_entsel(_T("\n请拾取功能区编号："), txtent, txtpt);
				TCHAR enttype[255] = { 0 };
				if (RTNORM == ret)//实体拾取成功，判断实体类型是否符合要求
				{
					if (!GetEntityType(txtent, enttype))
					{
						acutPrintf(_T("\n无法判断是否为文本，请重新拾取"));
						continue;
					}
					if (_tcscmp(enttype, _T("TEXT")) != 0 && _tcscmp(enttype, _T("MTEXT")))
					{
						acutPrintf(_T("\n请拾取文本"));
						continue;
					}
					else
					{
						TCHAR txt[255] = { 0 };
						GetEntText(txtent, txt);
						bh = txt;
						break;
					}
				}
				else//取消拾取注记编号文本，直接返回
				{
					acutPrintf(_T("\n已取消拾取功能区编号"));
					break;
				}
			} while (TRUE);
		}
		if (false == bh.IsEmpty())
		{
			while (TRUE)
			{
				if (isFirstDraw == FALSE)
				{
					int ibh = _tstoi(bh);
					ibh++;
					int cstrOriLen = bh.GetLength();
					bh.Format(_T("%d"), ibh);
					int cstrNewLen = bh.GetLength();
					for (int i = 0; i < cstrOriLen - cstrNewLen; i++)
					{
						bh.Insert(0, _T("0"));
					}
				}
				else//第一个编号直接测试是否存在，不自加一
				{
					isFirstDraw = FALSE;
				}
				DomainOperate domainoperate;
				AcDbObjectId fcpmid = domainoperate.getBuildLayerRangeEntity(gnqid);
				AcDbObjectIdArray gnqids;
				if (fcpmid == AcDbObjectId::kNull)
				{
					SelectFilter sfe(8, _T("功能区")), sfl(RTDXF0, _T("*POLYLINE"));
					SelectEntitys(gnqids, sfe, sfl, _T("X"));
				}
				else
				{
					AcGePoint2dArray fcpmnodes;
					GetPlList(aname(fcpmid), fcpmnodes);
					CString gnqlayer;
					IDataBaseOper oper;
					oper.readCommonTable(_T("建筑竣工功能区图层"), gnqlayer);
					TCHAR cstrlayer[255] = { 0 };
					_stprintf(cstrlayer, _T("%s"), gnqlayer);
					ssFromNodes(gnqids, fcpmnodes, 1, 0.5, _T("*POLYLINE"), cstrlayer);
				}
				BOOL existsamename = FALSE;
				for (int idx = 0; idx < gnqids.length(); idx++)
				{
					AcDbObjectId Xgnqid = gnqids.at(idx);
					TCHAR Xname[255] = { 0 };
					ReadXdata(aname(Xgnqid), _T("功能区名称"), 0, Xname);
					TCHAR Xbh[255] = { 0 };
					ReadXdata(aname(Xgnqid), _T("功能区编号"), 0, Xbh);
					if (_tcscmp(Xname, name) == 0 && _tcscmp(Xbh, bh) == 0)
					{
						existsamename = TRUE;
						break;
					}
				}
				if (existsamename == FALSE)break;
			}
		}
		bool isLand = (1 == m_areaKouChu.GetCheck());
		domainOperate.addDomainProperty(gnqid, name, m_mjxs, m_jrxs, gd, bh, isLand);
		acutPrintf(_T("\n重构功能区成功"));
	}
}

BOOL DrawDomainDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			case VK_SPACE:
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
