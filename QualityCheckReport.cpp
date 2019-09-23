// QualityCheckReport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QualityCheckReport.h"
#include "afxdialogex.h"


// CQualityCheckReport �Ի���

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


// CQualityCheckReport ��Ϣ�������


BOOL CQualityCheckReport::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rc; m_tab.GetClientRect(rc); m_CurSelTab = 0;
	rc.top += 20; rc.bottom -= 0; rc.left += 0; rc.right -= 0;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_tab.InsertItem(0, _T("�Բ������¼"));
	m_tab.InsertItem(1, _T("���������¼"));
	m_tab.InsertItem(2, _T("һ���ʼ����"));
	m_tab.InsertItem(3, _T("�����ʼ����"));
	m_tab.InsertItem(4, _T("���������¼"));
	//���������Ի���
	subDlag[0]->m_name.Format(_T("%s"), _T("�Բ������¼")); subDlag[0]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[1]->m_name.Format(_T("%s"), _T("���������¼")); subDlag[1]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[2]->m_name.Format(_T("%s"), _T("һ���ʼ����")); subDlag[2]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[3]->m_name.Format(_T("%s"), _T("�����ʼ����")); subDlag[3]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[4]->m_name.Format(_T("%s"), _T("���������¼")); subDlag[4]->Create(IDD_DIALOG_QUALITYCHECKSUBREPORT, &m_tab);
	subDlag[0]->MoveWindow(&rc); subDlag[0]->ShowWindow(SW_SHOW);
	subDlag[1]->MoveWindow(&rc); subDlag[1]->ShowWindow(SW_HIDE);
	subDlag[2]->MoveWindow(&rc); subDlag[2]->ShowWindow(SW_HIDE);
	subDlag[3]->MoveWindow(&rc); subDlag[3]->ShowWindow(SW_HIDE);
	subDlag[4]->MoveWindow(&rc); subDlag[4]->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CQualityCheckReport::OnTcnSelchangeTabQualitycheckreport(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	subDlag[m_CurSelTab]->ShowWindow(SW_HIDE);        //�õ��µ�ҳ������    
	m_CurSelTab = m_tab.GetCurSel();        //���µ�ҳ����ʾ����        
	subDlag[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;
}


void CQualityCheckReport::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	for(int idx = 0; idx < 5; ++idx)
		subDlag[idx]->onApply();
	CDialog::OnOK();
}
