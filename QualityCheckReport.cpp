// QualityCheckReport.cpp : 实现文件
//

#include "stdafx.h"
#include "QualityCheckReport.h"
#include "afxdialogex.h"


// CQualityCheckReport 对话框

IMPLEMENT_DYNAMIC(CQualityCheckReport, CDialog)

CQualityCheckReport::CQualityCheckReport(CWnd* pParent /*=NULL*/)
	: CDialog(CQualityCheckReport::IDD, pParent)
{
	for(int idx = 0; idx < 5; ++idx)
		subDlag[idx] = new CQualityCheckSubReport;
}

CQualityCheckReport::~CQualityCheckReport()
{
}

void CQualityCheckReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_QUALITYCHECKREPORT, m_tab);
}


BEGIN_MESSAGE_MAP(CQualityCheckReport, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_QUALITYCHECKREPORT, &CQualityCheckReport::OnTcnSelchangeTabQualitycheckreport)
END_MESSAGE_MAP()


// CQualityCheckReport 消息处理程序


BOOL CQualityCheckReport::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rc; m_tab.GetClientRect(rc); m_CurSelTab = 0;
	rc.top += 20; rc.bottom -= 0; rc.left += 0; rc.right -= 0;
	// TODO:  在此添加额外的初始化
	m_tab.InsertItem(0, _T("自查情况记录"));
	m_tab.InsertItem(1, _T("互查情况记录"));
	m_tab.InsertItem(2, _T("一级质检情况"));
	m_tab.InsertItem(3, _T("二级质检情况"));
	m_tab.InsertItem(4, _T("初审情况记录"));
	//创建两个对话框
	subDlag[0]->m_name.Format(_T("%s"), _T("自查情况记录")); subDlag[0]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[1]->m_name.Format(_T("%s"), _T("互查情况记录")); subDlag[1]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[2]->m_name.Format(_T("%s"), _T("一级质检情况")); subDlag[2]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[3]->m_name.Format(_T("%s"), _T("二级质检情况")); subDlag[3]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[4]->m_name.Format(_T("%s"), _T("初审情况记录")); subDlag[4]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[0]->MoveWindow(&rc); subDlag[0]->ShowWindow(SW_SHOW);
	subDlag[1]->MoveWindow(&rc); subDlag[1]->ShowWindow(SW_HIDE);
	subDlag[2]->MoveWindow(&rc); subDlag[2]->ShowWindow(SW_HIDE);
	subDlag[3]->MoveWindow(&rc); subDlag[3]->ShowWindow(SW_HIDE);
	subDlag[4]->MoveWindow(&rc); subDlag[4]->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CQualityCheckReport::OnTcnSelchangeTabQualitycheckreport(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	subDlag[m_CurSelTab]->ShowWindow(SW_HIDE);        //得到新的页面索引    
	m_CurSelTab = m_tab.GetCurSel();        //把新的页面显示出来        
	subDlag[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;
}


void CQualityCheckReport::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	for(int idx = 0; idx < 5; ++idx)
		subDlag[idx]->onApply();
	CDialog::OnOK();
}
