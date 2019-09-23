// QualityCheckSubReport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QualityCheckSubReport.h"
#include "afxdialogex.h"
#include "global.h"


// CQualityCheckSubReport �Ի���

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

// CQualityCheckSubReport ��Ϣ�������

void CQualityCheckSubReport::onApply()
{
	// TODO:  �ڴ����ר�ô����/����û���
	MStr info; m_context.GetWindowText(info[m_name]);
	IProjectMDB pdb; pdb.setJSSMTableInfo(info);
}

BOOL CQualityCheckSubReport::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	IProjectMDB pdb; MStr info = pdb.getJSSMTableInfo();
	m_context.SetWindowText(info[m_name]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}