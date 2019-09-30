// heightpropertysettingdlg.cpp: 实现文件
//

#include "stdafx.h"
#include "global.h"
#include "resource.h"
#include "afxdialogex.h"
#include "heightpropertysettingdlg.h"

// CHeightPropertySettingDlg 对话框

IMPLEMENT_DYNAMIC(CHeightPropertySettingDlg, CAcUiDialog)

CHeightPropertySettingDlg::CHeightPropertySettingDlg(CWnd* pParent /*=nullptr*/)
	: CAcUiDialog(IDD_DIALOG_HEIGHTPROPERTYSETTING, pParent)
{

}

CHeightPropertySettingDlg::~CHeightPropertySettingDlg()
{

}

void CHeightPropertySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SETTINGHEIGHT, m_settingHeight);
	DDX_Control(pDX, IDC_LIST_SETTINGBUILDBGINFO, m_buildBG);
}

#ifdef _WIN64
LRESULT CHeightPropertySettingDlg::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}
#else
LONG CHeightPropertySettingDlg::onAcadKeepFocus(UINT n, LONG l)
{
	return TRUE;
}
#endif

BEGIN_MESSAGE_MAP(CHeightPropertySettingDlg, CAcUiDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SETTINGHEIGHT, OnSingleClickListForHeight)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SETTINGBUILDBGINFO, OnSingleClickListForBuildBG)
	ON_BN_CLICKED(IDOK, &CHeightPropertySettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CHeightPropertySettingDlg 消息处理程序

BOOL CHeightPropertySettingDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	initDialogListCtrl();
	CString dwgName = GetCurrentDwgName();
	int pos1 = dwgName.FindOneOf(_T("_"));
	dwgName = dwgName.Mid(pos1 + 1);
	pos1 = dwgName.FindOneOf(_T("."));
	m_buildName = dwgName.Mid(0, pos1);
	initListCtrlHeightValue();
	initListCtrlBUildBGValue();
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CHeightPropertySettingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	saveListCtrlHeightValue();
	saveListCtrlBUildBGValue();
	CAcUiDialog::OnOK();
}

void CHeightPropertySettingDlg::initDialogListCtrl()
{
	m_settingHeight.Init();
	RECT rect; m_settingHeight.GetClientRect(&rect); int row = 0;
	int width = (rect.right - rect.left - 260) / 4;
	m_settingHeight.InsertColumn(row++, _T("层名"), LVCFMT_CENTER, 60);
	m_settingHeight.InsertColumn(row++, _T("规划层高"), LVCFMT_CENTER, 100);
	m_settingHeight.InsertColumn(row++, _T("实测层高"), LVCFMT_CENTER, 100);
	m_settingHeight.InsertColumn(row++, _T("规划局部层高下限"), LVCFMT_CENTER, width);
	m_settingHeight.InsertColumn(row++, _T("规划局部层高上限"), LVCFMT_CENTER, width);
	m_settingHeight.InsertColumn(row++, _T("实测局部层高下限"), LVCFMT_CENTER, width);
	m_settingHeight.InsertColumn(row++, _T("实测局部层高上限"), LVCFMT_CENTER, width);

	m_buildBG.GetClientRect(&rect); row = 0;
	width = (rect.right - rect.left) / 5; m_buildBG.Init();
	m_buildBG.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_buildBG.InsertColumn(row++, _T("层名"), LVCFMT_CENTER, width);
	m_buildBG.InsertColumn(row++, _T("室内地面标高"), LVCFMT_CENTER, width);
	m_buildBG.InsertColumn(row++, _T("室外地坪标高"), LVCFMT_CENTER, width);
	m_buildBG.InsertColumn(row++, _T("建筑高度"), LVCFMT_CENTER, width);
	m_buildBG.InsertColumn(row++, _T("标高位置"), LVCFMT_CENTER, width);
}

void CHeightPropertySettingDlg::initListCtrlHeightValue()
{
	MStr filter; filter[_T("楼栋号")] = m_buildName;
	IProjectMDB pdb; VMStr records = pdb.getCXXTableInfoEx(filter);
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx]; CString lName;
		if(record[_T("终止层名")].IsEmpty()) lName.Format(_T("%s"), record[_T("起始层名")]);
		else
			lName.Format(_T("%s ----> %s"), record[_T("起始层名")], record[_T("终止层名")]);
		m_settingHeight.InsertItem(idx, lName); int col = 1;
		m_settingHeight.SetItemText(idx, col++, record[_T("设计层高")]);
		m_settingHeight.SetItemText(idx, col++, record[_T("实测层高")]);
		m_settingHeight.SetItemText(idx, col++, record[_T("规划局部层高下限")]);
		m_settingHeight.SetItemText(idx, col++, record[_T("规划局部层高上限")]);
		m_settingHeight.SetItemText(idx, col++, record[_T("实测局部层高下限")]);
		m_settingHeight.SetItemText(idx, col++, record[_T("实测局部层高上限")]);
	}
}

void CHeightPropertySettingDlg::saveListCtrlHeightValue()
{
	int row = m_settingHeight.GetItemCount();
	IProjectMDB pdb;
	MStr filter, record; filter[_T("楼栋号")] = m_buildName;
	for(int idx = 0; idx < row; ++idx)
	{
		filter.clear(); record.clear();
		CString lName = m_settingHeight.GetItemText(idx, 0);
		int pos = lName.FindOneOf(_T(" "));
		if(pos > 0)
		{
			filter[_T("起始层名")] = lName.Mid(0, pos);
			lName = lName.Mid(pos + 1);
			int pos = lName.FindOneOf(_T(" "));
			filter[_T("终止层名")] = lName.Mid(pos + 1);
		}
		else
			filter[_T("起始层名")] = lName;
		CString info = m_settingHeight.GetItemText(idx, 1); filter[_T("设计层高")] = info;
		info = m_settingHeight.GetItemText(idx, 2); if(!info.IsEmpty())	record[_T("实测层高")] = info;
		info = m_settingHeight.GetItemText(idx, 3); if(!info.IsEmpty()) record[_T("规划局部层高下限")] = info;
		info = m_settingHeight.GetItemText(idx, 4); if(!info.IsEmpty()) record[_T("规划局部层高上限")] = info;
		info = m_settingHeight.GetItemText(idx, 5); if(!info.IsEmpty()) record[_T("实测局部层高下限")] = info;
		info = m_settingHeight.GetItemText(idx, 6); if(!info.IsEmpty()) record[_T("实测局部层高上限")] = info;
		pdb.updataCXXTableInfo(filter, record);
	}
}

void CHeightPropertySettingDlg::initListCtrlBUildBGValue()
{
	MStr filter; filter[_T("楼栋号")] = m_buildName;
	IProjectMDB pdb; VMStr records = pdb.getCXXTableInfoEx(filter);
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx]; CString lName;
		//if(record[_T("楼栋号")].CompareNoCase(m_buildName)) continue;
		if(record[_T("终止层名")].IsEmpty()) lName.Format(_T("%s"), record[_T("起始层名")]);
		else
			lName.Format(_T("%s ----> %s"), record[_T("起始层名")], record[_T("终止层名")]);
		m_buildBG.InsertItem(idx, lName); int col = 1;
		m_buildBG.SetItemText(idx, col++, record[_T("室内地面")]);
		m_buildBG.SetItemText(idx, col++, record[_T("室外地坪")]);
		m_buildBG.SetItemText(idx, col++, record[_T("建筑高度")]);
		m_buildBG.SetItemText(idx, col++, record[_T("标高位置")]);
	}
}

void CHeightPropertySettingDlg::saveListCtrlBUildBGValue()
{
	int row = m_buildBG.GetItemCount();
	IProjectMDB pdb; MStr filter, record;
	filter[_T("楼栋号")] = m_buildName;
	for(int idx = 0; idx < row; ++idx)
	{
		filter.clear(); record.clear();
		CString lName = m_buildBG.GetItemText(idx, 0);
		int pos = lName.FindOneOf(_T(" "));
		if(pos > 0)
		{
			filter[_T("起始层名")] = lName.Mid(0, pos);
			lName = lName.Mid(pos + 1);
			int pos = lName.FindOneOf(_T(" "));
			filter[_T("终止层名")] = lName.Mid(pos + 1);
		}
		else
			filter[_T("起始层名")] = lName;
		CString info = m_buildBG.GetItemText(idx, 1); if (!info.IsEmpty()) record[_T("室内地面")] = info;
		info = m_buildBG.GetItemText(idx, 2); if(!info.IsEmpty()) record[_T("室外地坪")] = info;
		info = m_buildBG.GetItemText(idx, 3); if(!info.IsEmpty()) record[_T("建筑高度")] = info;
		info = m_buildBG.GetItemText(idx, 4); if(!info.IsEmpty()) record[_T("标高位置")] = info;
		pdb.updataCXXTableInfo(filter, record);
	}
}

void CHeightPropertySettingDlg::OnSingleClickListForHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int rowNo = pNMListView->iItem;
	int colNo = pNMListView->iSubItem;
	if(colNo >= 0 && colNo < 7 && rowNo >= 0)
	{
		BeginEditorCommand(); ads_name ent; ads_point pt;
		TCHAR txt[255] = {0};
		if(RTNORM == ads_entsel(_T("\n 请选择文字注记："), ent, pt))
			GetEntText(ent, txt);
		m_settingHeight.SetItemText(rowNo, colNo, txt);
		CompleteEditorCommand();
	}
	*pResult = 0;
}

void CHeightPropertySettingDlg::OnSingleClickListForBuildBG(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int rowNo = pNMListView->iItem;
	int colNo = pNMListView->iSubItem;
	if(colNo >= 0 && colNo < 4 && rowNo >= 0)	//判断单击位置是否在有数据的列表项上面
	{
		BeginEditorCommand(); ads_name ent; ads_point pt;
		TCHAR txt[255] = {0};
		if(RTNORM == ads_entsel(_T("\n 请选择文字注记："), ent, pt))
			GetEntText(ent, txt);
		m_buildBG.SetItemText(rowNo, colNo, txt);
		CompleteEditorCommand();
	}
	*pResult = 0;
}

