// XMJGReConfigurationDlg.cpp : 实现文件
//

#include "stdafx.h"
#ifndef XMJGRECONFIGURATIONDLG_H
#include "XMJGReConfigurationDlg.h"
#endif /*XMJGRECONFIGURATIONDLG_H*/

// XMJGReConfigurationDlg 对话框

bool g_isPointSel;
bool g_isBoundarySel;

IMPLEMENT_DYNAMIC(XMJGReConfigurationDlg, CDialog)

XMJGReConfigurationDlg::XMJGReConfigurationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(XMJGReConfigurationDlg::IDD, pParent)
{

}

XMJGReConfigurationDlg::~XMJGReConfigurationDlg()
{

}

void XMJGReConfigurationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(XMJGReConfigurationDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &XMJGReConfigurationDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &XMJGReConfigurationDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &XMJGReConfigurationDlg::OnBnClickedRadio3)
END_MESSAGE_MAP()

// XMJGReConfigurationDlg 消息处理程序


// XMJGReConfigurationDockbar 消息处理程序

// XMJGReConfigurationDockbar 对话框

IMPLEMENT_DYNAMIC(XMJGReConfigurationDockbar, CAdUiDockControlBar)

BEGIN_MESSAGE_MAP(XMJGReConfigurationDockbar, CAdUiDockControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static CLSID clsCMyDockBar = {0x4ebb7914, 0x7356, 0x43de, {0xb4, 0x50, 0x62, 0xbe, 0xae, 0x21, 0x06, 0xde}};

XMJGReConfigurationDockbar::XMJGReConfigurationDockbar()
	: CAdUiDockControlBar(), m_dlg(NULL)
{

}

void XMJGReConfigurationDockbar::setChildDlg(CDialog *dlg, int IndexIDD)
{
	m_dlg = dlg; m_IDD = IndexIDD;
}

BOOL XMJGReConfigurationDockbar::Create(CWnd*pParent, LPCTSTR lpszTitle)
{
	CString strWndClass;
	strWndClass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW));
	CRect rect(0, 0, 250, 200);
	if(!CAdUiDockControlBar::Create( strWndClass, lpszTitle,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParent, 0))
		return (FALSE);
	SetToolID(&clsCMyDockBar); return (TRUE);
}

int XMJGReConfigurationDockbar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CAdUiDockControlBar::OnCreate(lpCreateStruct) == -1)
		return (-1);
	m_dlg->Create(m_IDD, this);
	m_dlg->ShowWindow(SW_SHOW);
	return (0);
}

void XMJGReConfigurationDockbar::SizeChanged(CRect *lpRect, BOOL bFloating, int flags)
{
	m_dlg->MoveWindow(lpRect->left, lpRect->top + 2,
		abs(lpRect->right - lpRect->left), abs(lpRect->bottom - lpRect->top) - 2);
	CAdUiDockControlBar::SizeChanged(lpRect, bFloating, flags);
}

BOOL XMJGReConfigurationDockbar::DestroyWindow()
{
	m_dlg->DestroyWindow();
	return CAdUiDockControlBar::DestroyWindow();
}

BOOL XMJGReConfigurationDockbar::AddCustomMenuItems(LPARAM hMenu)
{
	HMENU hPopMenu;	hPopMenu = (HMENU)(hMenu);
	CMenu* pop_menu = CMenu::FromHandle(hPopMenu);
	pop_menu->RemoveMenu(ID_ADUI_HIDEBAR, MF_BYCOMMAND); //去掉"允许固定"
	pop_menu->RemoveMenu(ID_ADUI_ALLOWDOCK, MF_BYCOMMAND);//去掉"隐藏"
	return true;
}

void XMJGReConfigurationDlg::OnBnClickedRadio1()
{
	// TODO:  在此添加控件通知处理程序代码
	CButton *btn = (CButton *)GetDlgItem(IDC_RADIO1);
	g_isPointSel = (bool)btn->GetCheck();
	if(g_isPointSel)
		g_isBoundarySel = false;
}

void XMJGReConfigurationDlg::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
	CButton *btn = (CButton *)GetDlgItem(IDC_RADIO2);
	g_isBoundarySel = (bool)btn->GetCheck();
	if(g_isBoundarySel)
		g_isPointSel = false;
}

void XMJGReConfigurationDlg::OnBnClickedRadio3()
{
	// TODO:  在此添加控件通知处理程序代码
	g_isPointSel = false;
	g_isBoundarySel = false;
}

IMPLEMENT_DYNAMIC(XMJGDrawDomainDockControl, CAdUiDockControlBar)

BEGIN_MESSAGE_MAP(XMJGDrawDomainDockControl, CAdUiDockControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static CLSID clsCMyDockBar1 = {0x4ebb7914, 0x7356, 0x43de, {0xb4, 0x50, 0x62, 0xbe, 0xae, 0x21, 0x06, 0xde}};

XMJGDrawDomainDockControl::XMJGDrawDomainDockControl()
	: CAdUiDockControlBar(), m_dlg(NULL)
{

}

void XMJGDrawDomainDockControl::setChildDlg(CDialog *dlg, int IndexIDD)
{
	m_dlg = dlg; m_IDD = IndexIDD;
}

BOOL XMJGDrawDomainDockControl::Create(CWnd*pParent, LPCTSTR lpszTitle)
{
	CString strWndClass;
	strWndClass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW));
	CRect rect(0, 0, 250, 200);
	if(!CAdUiDockControlBar::Create(strWndClass, lpszTitle,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParent, 0))
		return (FALSE);
	SetToolID(&clsCMyDockBar); return (TRUE);
}

int XMJGDrawDomainDockControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CAdUiDockControlBar::OnCreate(lpCreateStruct) == -1)
		return (-1);
	m_dlg->Create(m_IDD, this);
	m_dlg->ShowWindow(SW_SHOW);
	return (0);
}

void XMJGDrawDomainDockControl::SizeChanged(CRect *lpRect, BOOL bFloating, int flags)
{
	m_dlg->MoveWindow(lpRect->left, lpRect->top + 2,
		abs(lpRect->right - lpRect->left), abs(lpRect->bottom - lpRect->top) - 2);
	CAdUiDockControlBar::SizeChanged(lpRect, bFloating, flags);
}

BOOL XMJGDrawDomainDockControl::DestroyWindow()
{
	m_dlg->DestroyWindow();
	return CAdUiDockControlBar::DestroyWindow();
}

BOOL XMJGDrawDomainDockControl::AddCustomMenuItems(LPARAM hMenu)
{
	HMENU hPopMenu;	hPopMenu = (HMENU)(hMenu);
	CMenu* pop_menu = CMenu::FromHandle(hPopMenu);
	pop_menu->RemoveMenu(ID_ADUI_HIDEBAR, MF_BYCOMMAND); //去掉"允许固定"
	pop_menu->RemoveMenu(ID_ADUI_ALLOWDOCK, MF_BYCOMMAND);//去掉"隐藏"
	return true;
}
