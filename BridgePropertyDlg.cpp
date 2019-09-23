// BridgePropertyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "BridgePropertyDlg.h"
#include "afxdialogex.h"
#include "Global.h"

// BridgePropertyDlg �Ի���

IMPLEMENT_DYNAMIC(BridgePropertyDlg, CAcUiDialog)

BridgePropertyDlg::BridgePropertyDlg(CWnd* pParent /*=NULL*/)
: CAcUiDialog(BridgePropertyDlg::IDD, pParent)
{

}

BridgePropertyDlg::~BridgePropertyDlg()
{
}

void BridgePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ENDPT, m_endPt);
	DDX_Control(pDX, IDC_BUTTON_STARTPT, m_startPt);
	DDX_Control(pDX, IDC_EDIT_STARYPTX, m_startPtX);
	DDX_Control(pDX, IDC_EDIT_STARTPTY, m_startPtY);
	DDX_Control(pDX, IDC_EDIT_ENDXPT, m_endPtX);
	DDX_Control(pDX, IDC_EDIT_ENTYPT, m_endPtY);
	DDX_Control(pDX, IDC_COMBO_TQGCXS, m_tqgcxs);
	DDX_Control(pDX, IDC_EDIT_JGCLTBH, m_jgcltbh);
	DDX_Control(pDX, IDC_EDIT_JZTRYDBJJLQK, m_jjtrydbjjlqk);
}

BEGIN_MESSAGE_MAP(BridgePropertyDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_STARTPT, &BridgePropertyDlg::OnBnClickedButtonStartpt)
	ON_BN_CLICKED(IDC_BUTTON_ENDPT, &BridgePropertyDlg::OnBnClickedButtonEndpt)
	ON_BN_CLICKED(IDOK, &BridgePropertyDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// BridgePropertyDlg ��Ϣ�������

BOOL BridgePropertyDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_endPt.AutoLoad();
	m_startPt.AutoLoad();
	IDataBaseOper oper; CStringArray values;
	CString value = oper.readJGCommon(_T("������ʽ"));
	AnalyzeData(values, value, _T(";"));
	for(int idx = 0; idx < values.GetSize(); ++idx)
		m_tqgcxs.AddString(values[idx]);
	setDialogValue();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void BridgePropertyDlg::setDialogValue()
{
	IProjectMDB pdb;
	MStr record = pdb.getJBXXTableInfo();
	CString info = record[_T("������ʽ")];
	m_tqgcxs.SetWindowText(info);
	m_jgcltbh.SetWindowText(record[_T("��������ͼ���")]);
	m_startPtX.SetWindowText(record[_T("��������ͨ�����X")]);
	m_startPtY.SetWindowText(record[_T("��������ͨ�����Y")]);
	m_endPtX.SetWindowText(record[_T("��������ͨ���յ�X")]);
	m_endPtY.SetWindowText(record[_T("��������ͨ���յ�Y")]);
	m_jjtrydbjjlqk.SetWindowText(record[_T("���������õر߽�������")]);
}

LRESULT BridgePropertyDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return 1;
}

void BridgePropertyDlg::OnBnClickedButtonStartpt()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BeginEditorCommand();
	ads_point startpt;
	if(5100 == ads_getpoint(NULL, _T("\n��ʰȡ����ͨ��λ�����:"), startpt))
	{
		CString info; info.Format(_T("%.3lf"), startpt[0]);
		m_startPtX.SetWindowText(info);
		info.Format(_T("%.3lf"), startpt[1]);
		m_startPtY.SetWindowText(info);
	}
	CompleteEditorCommand();
}

void BridgePropertyDlg::OnBnClickedButtonEndpt()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BeginEditorCommand();
	ads_point startpt;
	if(5100 == ads_getpoint(NULL, _T("\n��ʰȡ����ͨ��λ�����:"), startpt))
	{
		CString info; info.Format(_T("%.3lf"), startpt[0]);
		m_endPtX.SetWindowText(info);
		info.Format(_T("%.3lf"), startpt[1]);
		m_endPtY.SetWindowText(info);
	}
	CompleteEditorCommand();
}

void BridgePropertyDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString info;
	m_tqgcxs.GetWindowText(info);
	MStr record; record[_T("������ʽ")] = info;
	m_jgcltbh.GetWindowText(info);
	record[_T("��������ͼ���")] = info;
	m_startPtX.GetWindowText(info);
	record[_T("��������ͨ�����X")] = info;
	m_startPtY.GetWindowText(info);
	record[_T("��������ͨ�����Y")] = info;
	m_endPtX.GetWindowText(info);
	record[_T("��������ͨ���յ�X")] = info;
	m_endPtY.GetWindowText(info);
	record[_T("��������ͨ���յ�Y")] = info;
	m_jjtrydbjjlqk.GetWindowText(info);
	record[_T("���������õر߽�������")] = info;
	IProjectMDB pdb; pdb.setJBXXTableInfo(record);
	CAcUiDialog::OnOK();
}
