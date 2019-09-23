// QualityCheckSubReport.cpp : 实现文件
//

#include "stdafx.h"
#include "QualityCheckSubReport.h"
#include "afxdialogex.h"
#include "global.h"


// CQualityCheckSubReport 对话框

IMPLEMENT_DYNAMIC(CQualityCheckSubReport, CDialog)

CQualityCheckSubReport::CQualityCheckSubReport(CWnd* pParent /*=NULL*/)
	: CDialog(CQualityCheckSubReport::IDD, pParent)
{

}

CQualityCheckSubReport::~CQualityCheckSubReport()
{
}

void CQualityCheckSubReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_QUALITYCHECKCONTEXT, m_context);
}

BEGIN_MESSAGE_MAP(CQualityCheckSubReport, CDialog)
END_MESSAGE_MAP()

// CQualityCheckSubReport 消息处理程序

void CQualityCheckSubReport::onApply()
{
	// TODO:  在此添加专用代码和/或调用基类
	MStr info; m_context.GetWindowText(info[m_name]);
	IProjectMDB pdb; pdb.setJSSMTableInfo(info);
}

BOOL CQualityCheckSubReport::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	IProjectMDB pdb; MStr info = pdb.getJSSMTableInfo();
	m_context.SetWindowText(info[m_name]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}