// DrawUpPlanning.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrawUpPlanning.h"
#include "afxdialogex.h"
#include "Global.h"

// CDrawUpPlanning �Ի���

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

// CDrawUpPlanning ��Ϣ�������

BOOL CDrawUpPlanningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	IProjectMDB pdb;
	MStr record = pdb.getJSSMTableInfo();
	if(record[_T("����滮��Ϣ")].GetLength() != 0)
		m_drawUpPlan.SetWindowText(record[_T("����滮��Ϣ")]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CDrawUpPlanningDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString info; IProjectMDB pdb;
	m_drawUpPlan.GetWindowText(info);
	MStr record; record[_T("����滮��Ϣ")] = info;
	pdb.setJSSMTableInfo(record);
	CDialog::OnOK();
}
