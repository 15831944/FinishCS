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
	DomainOperate domainOperate; bool flag = true;
	acDocManager->lockDocument(curDoc());
	acedGetAcadDockCmdLine()->SetFocus();
	while(flag)
	{
		CString info; AcDbObjectId gnqid;
		m_selectStyle.GetWindowText(info);

		if(info.IsEmpty())
		{
			MessageBox(_T("请选择功能区绘制方式"), _T("错误信息"), 0);
			goto DrawDomainDialogFlag;
		}
		CString gd, bh, name;
		m_domainName.GetWindowText(name);
		m_jrFactor.GetWindowText(m_jrxs);
		m_domainHeight.GetWindowText(gd);
		m_areaFactor.GetWindowText(m_mjxs);
		m_imPortBH.GetWindowText(bh);
		m_drawStyle = m_selectStyle.GetCurSel();
		bool isLand = (1 == m_areaKouChu.GetCheck());
		switch(m_drawStyle)
		{
			case 0: flag = domainOperate.createDomainByBoundary(gnqid); break;
			case 1: flag = domainOperate.createDomainByPointSel(gnqid); break;
			case 2: flag = domainOperate.createDomainByDraw(gnqid); break;
		}
		if(!flag) goto DrawDomainDialogFlag;
		if(name.IsEmpty())
		{
			while(true)
			{
				ads_point pt; ads_name ent; TCHAR txt[255] = {0};
				int ret = ads_entsel(_T("\n 请选择功能区名称:"), ent, pt);
				if(ret == RTCAN) goto DrawDomainDialogFlag; 
				if(!GetEntityType(ent, txt))
				{
					acutPrintf(_T("\n 选择的不是文本, 请重新选择"));
					continue;
				}
				GetEntText(ent, txt); name.Format(_T("%s"), txt);
				break;
			}
		}
		if(bh.IsEmpty())
		{
			while(true)
			{
				ads_point pt; ads_name ent; TCHAR txt[255] = {0};
				int ret = ads_entsel(_T("\n 请选择注记编号:"), ent, pt);
				if(ret == RTCAN) goto DrawDomainDialogFlag;
				else if(ret == -5001) break;
				GetEntText(ent, txt);
				bh.Format(_T("%s"), txt);
				break;
			}
		}
		else
			m_imPortBH.GetWindowText(bh);
		domainOperate.addDomainProperty(gnqid, name, m_mjxs, m_jrxs, gd, bh, isLand);
		acutPrintf(_T("\n 重构功能区成功"));
	}

DrawDomainDialogFlag:
	acDocManager->unlockDocument(curDoc());
	setControlStatus(TRUE);
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
