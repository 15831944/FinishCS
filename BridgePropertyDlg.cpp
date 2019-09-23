// BridgePropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "BridgePropertyDlg.h"
#include "afxdialogex.h"
#include "Global.h"

// BridgePropertyDlg 对话框

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

// BridgePropertyDlg 消息处理程序

BOOL BridgePropertyDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_endPt.AutoLoad();
	m_startPt.AutoLoad();
	IDataBaseOper oper; CStringArray values;
	CString value = oper.readJGCommon(_T("天桥型式"));
	AnalyzeData(values, value, _T(";"));
	for(int idx = 0; idx < values.GetSize(); ++idx)
		m_tqgcxs.AddString(values[idx]);
	setDialogValue();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void BridgePropertyDlg::setDialogValue()
{
	IProjectMDB pdb;
	MStr record = pdb.getJBXXTableInfo();
	CString info = record[_T("天桥型式")];
	m_tqgcxs.SetWindowText(info);
	m_jgcltbh.SetWindowText(record[_T("竣工测量图编号")]);
	m_startPtX.SetWindowText(record[_T("天桥主体通道起点X")]);
	m_startPtY.SetWindowText(record[_T("天桥主体通道起点Y")]);
	m_endPtX.SetWindowText(record[_T("天桥主体通道终点X")]);
	m_endPtY.SetWindowText(record[_T("天桥主体通道终点Y")]);
	m_jjtrydbjjlqk.SetWindowText(record[_T("建筑退让用地边界距离情况")]);
}

LRESULT BridgePropertyDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return 1;
}

void BridgePropertyDlg::OnBnClickedButtonStartpt()
{
	// TODO:  在此添加控件通知处理程序代码
	BeginEditorCommand();
	ads_point startpt;
	if(5100 == ads_getpoint(NULL, _T("\n请拾取天桥通道位置起点:"), startpt))
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
	// TODO:  在此添加控件通知处理程序代码
	BeginEditorCommand();
	ads_point startpt;
	if(5100 == ads_getpoint(NULL, _T("\n请拾取天桥通道位置起点:"), startpt))
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
	// TODO:  在此添加控件通知处理程序代码
	CString info;
	m_tqgcxs.GetWindowText(info);
	MStr record; record[_T("天桥型式")] = info;
	m_jgcltbh.GetWindowText(info);
	record[_T("竣工测量图编号")] = info;
	m_startPtX.GetWindowText(info);
	record[_T("天桥主体通道起点X")] = info;
	m_startPtY.GetWindowText(info);
	record[_T("天桥主体通道起点Y")] = info;
	m_endPtX.GetWindowText(info);
	record[_T("天桥主体通道终点X")] = info;
	m_endPtY.GetWindowText(info);
	record[_T("天桥主体通道终点Y")] = info;
	m_jjtrydbjjlqk.GetWindowText(info);
	record[_T("建筑退让用地边界距离情况")] = info;
	IProjectMDB pdb; pdb.setJBXXTableInfo(record);
	CAcUiDialog::OnOK();
}
