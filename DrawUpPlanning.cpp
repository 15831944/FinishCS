// DrawUpPlanning.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawUpPlanning.h"
#include "afxdialogex.h"
#include "Global.h"

// CDrawUpPlanning 对话框

IMPLEMENT_DYNAMIC(CDrawUpPlanningDlg, CDialog)

CDrawUpPlanningDlg::CDrawUpPlanningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawUpPlanningDlg::IDD, pParent)
{

}

CDrawUpPlanningDlg::~CDrawUpPlanningDlg()
{
}

void CDrawUpPlanningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DRAWUPPLANNINGINPUT, m_drawUpPlan);
}

BEGIN_MESSAGE_MAP(CDrawUpPlanningDlg, CDialog)
END_MESSAGE_MAP()

// CDrawUpPlanning 消息处理程序

BOOL CDrawUpPlanningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	IProjectMDB pdb;
	MStr record = pdb.getJSSMTableInfo();
	if(record[_T("提请规划信息")].GetLength() != 0)
		m_drawUpPlan.SetWindowText(record[_T("提请规划信息")]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDrawUpPlanningDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	CString info; IProjectMDB pdb;
	m_drawUpPlan.GetWindowText(info);
	MStr record; record[_T("提请规划信息")] = info;
	pdb.setJSSMTableInfo(record);
	CDialog::OnOK();
}
