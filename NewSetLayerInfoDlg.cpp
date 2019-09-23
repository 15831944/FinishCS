// NewSetLayerInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NewSetLayerInfoDlg.h"
#include "afxdialogex.h"
#include <vector>
#include "XMJGMacro.h"
#include "Global.h"


// CNewSetLayerInfoDlg 对话框

IMPLEMENT_DYNAMIC(CNewSetLayerInfoDlg, CDialog)

CNewSetLayerInfoDlg::CNewSetLayerInfoDlg(bool isLayer, CWnd* pParent /*=NULL*/)
: CDialog(CNewSetLayerInfoDlg::IDD, pParent)
, m_id(AcDbObjectId::kNull), m_isLayer(isLayer)
{

}

CNewSetLayerInfoDlg::~CNewSetLayerInfoDlg()
{
}

void CNewSetLayerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LAYERINFOLISTS, m_LayerInfos);
	DDX_Control(pDX, IDC_BUTTON_ADDLAYER, m_addLayer);
	DDX_Control(pDX, IDC_BUTTON_COPYLAYER, m_copylayer);
	DDX_Control(pDX, IDC_BUTTON_DELLAYER, m_dellayer);
	DDX_Control(pDX, IDC_BUTTON_MOVEDOWN, m_movedown);
	DDX_Control(pDX, IDC_BUTTON_MOVEUP, m_moveup);
	DDX_Control(pDX, IDC_BUTTON_SPLITLAYER, m_splitLayer);
	DDX_Control(pDX, IDC_BUTTON_MAGERLAYER, m_margeLayer);
}

BEGIN_MESSAGE_MAP(CNewSetLayerInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, &CNewSetLayerInfoDlg::OnBnClickedButtonMoveup)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, &CNewSetLayerInfoDlg::OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_DELLAYER, &CNewSetLayerInfoDlg::OnBnClickedButtonDellayer)
	ON_BN_CLICKED(IDC_BUTTON_ADDLAYER, &CNewSetLayerInfoDlg::OnBnClickedButtonAddlayer)
	ON_BN_CLICKED(IDC_BUTTON_COPYLAYER, &CNewSetLayerInfoDlg::OnBnClickedButtonCopylayer)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SPLITLAYER, &CNewSetLayerInfoDlg::OnBnClickedButtonSplitlayer)
	ON_BN_CLICKED(IDC_BUTTON_MAGERLAYER, &CNewSetLayerInfoDlg::OnBnClickedButtonMagerlayer)
END_MESSAGE_MAP()

// CNewSetLayerInfoDlg 消息处理程序

void CNewSetLayerInfoDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	int count = m_LayerInfos.GetItemCount();
	int col = m_LayerInfos.GetColumnCount();
	IProjectMDB pdb; MStr filter;
	filter[_T("楼栋号")] = m_buildName;
	//pdb.delCXXTableInfo(filter);
	for(int idx = 0; idx < count; ++idx)
	{
		int offset = 1;
		MStr record; CString val;
		TCHAR isLoftLayer[2] = {0};
		_stprintf(isLoftLayer, _T("%s"), _T("0"));
		int type = m_LayerInfos.GetCheck(idx);
		record[_T("是否显示")].Format(_T("%d"), type);
		val = m_LayerInfos.GetItemText(idx, offset++); val.Trim();
		record[_T("楼栋号")] = val;
		val = m_LayerInfos.GetItemText(idx, offset++); val.Trim();
		record[_T("起始层名")] = val; int slyr = _ttoi(val);
		filter[_T("起始层名")] = val;
		val = m_LayerInfos.GetItemText(idx, offset++); val.Trim();
		record[_T("终止层名")] = val; int elyr = _ttoi(val);
		if(elyr == 0)
		{
			val = record[_T("起始层名")]; 
			if(val.Find(_T(';')) == -1) record[_T("层数")] = _T("1");
			else
			{
				CStringArray temps;
				AnalyzeData(temps, val, _T(";"));
				record[_T("层数")].Format(_T("%d"), temps.GetSize());
				_stprintf(isLoftLayer, _T("%s"), _T("1"));
			}
		}
		else if(elyr < slyr)
		{
			CString error; error.Format(_T("楼层设信息设置错误，请检查，错误发生在在第%d行"), idx);
			MessageBox(error, _T("错误信息"), 0); return;
		}
		else if (elyr > slyr)
			record[_T("层数")].Format(_T("%d"), elyr - slyr + 1);

		val = m_LayerInfos.GetItemText(idx, offset++); val.Trim();
		if(val.GetLength() == 0)
		{
			MessageBox(_T("请设置实测层高"), _T("错误信息")); return;
		}
		record[_T("实测层高")] = val;
		val = m_LayerInfos.GetItemText(idx, offset++); val.Trim();
		record[_T("设计层高")] = val;
		AddXdata(aname(m_id), _T("IsLoftLayer"), 0, isLoftLayer);
		double gdcz = _tstof(record[_T("实测层高")]) - _tstof(record[_T("设计层高")]);
		record[_T("高度差值")].Format(_T("%.3lf"), gdcz);
		val = m_LayerInfos.GetItemText(idx, offset++); val.Trim();
		record[_T("层备注")] = val;
		record[_T("楼层号")].Format(_T("%d"), idx + 1);
		//record[_T("是否跃层")].Format(_T("%s"), isLoftLayer);
		VMStr selectrow;
		if (pdb.getRecordCount(_T("CXX"), filter))
		{
			pdb.updataCXXTableInfo(filter, record);
			pdb.hasTableRow(_T("CXX"), filter, selectrow);
			pdb.deleteTableRecord(_T("CXX"), filter);
			pdb.addCXXTableInfo(selectrow.at(0));
		}
		else
		{
			pdb.addCXXTableInfo(record);
		}
		if(m_isLayer)
		{
			m_smls[idx].m_sLayer = record[_T("起始层名")];
			m_smls[idx].m_eLayer = record[_T("终止层名")];
		}
	}
	CDialog::OnOK();
}

void CNewSetLayerInfoDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

BOOL CNewSetLayerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	if(m_isLayer)
	{
		m_moveup.ShowWindow(SW_HIDE);
		m_movedown.ShowWindow(SW_HIDE);
		m_addLayer.ShowWindow(SW_HIDE);
		m_copylayer.ShowWindow(SW_HIDE);
		m_splitLayer.ShowWindow(SW_SHOW);
		m_margeLayer.ShowWindow(SW_SHOW);
	}
	else
	{
		m_moveup.ShowWindow(SW_SHOW);
		m_movedown.ShowWindow(SW_SHOW);
		m_addLayer.ShowWindow(SW_SHOW);
		m_copylayer.ShowWindow(SW_SHOW);
		m_splitLayer.ShowWindow(SW_HIDE);
		m_margeLayer.ShowWindow(SW_HIDE);
	}
	initListCtrl();
	setListCtrlData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CNewSetLayerInfoDlg::initListCtrl()
{
	int offset = 0;
	CRect rect; m_LayerInfos.GetClientRect(&rect);
	int width = (rect.right - rect.left - 240) / 5;
	m_LayerInfos.SetExtendedStyle(m_LayerInfos.GetExtendedStyle()|LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	if(m_isLayer)
		m_LayerInfos.InsertColumn(offset, _T("主楼层"));
	else
		m_LayerInfos.InsertColumn(offset, _T("是否显示"));
	m_LayerInfos.SetColumnWidth(offset++, 60);
	m_LayerInfos.InsertColumn(offset, _T("楼栋号"));
	m_LayerInfos.SetColumnWidth(offset++, width);
	m_LayerInfos.InsertColumn(offset, _T("起始层名"));
	m_LayerInfos.SetColumnWidth(offset++, width);
	m_LayerInfos.InsertColumn(offset, _T("终止层名"));
	m_LayerInfos.SetColumnWidth(offset++, width);
	m_LayerInfos.InsertColumn(offset, _T("实测层高"));
	m_LayerInfos.SetColumnWidth(offset++, width);
	m_LayerInfos.InsertColumn(offset, _T("设计层高"));
	m_LayerInfos.SetColumnWidth(offset++, width);
	m_LayerInfos.InsertColumn(offset, _T("层备注"));
	m_LayerInfos.SetColumnWidth(offset++, 240);
	m_LayerInfos.SetColumnReadOnly(0, TRUE);
}

void CNewSetLayerInfoDlg::setListCtrlData()
{
	IProjectMDB pdb; TCHAR info[255] = {0};
	VMStr records = pdb.getCXXTableInfo();
	map<int, SplitMargeLayer> ISMLs;
	if(m_isLayer)
	{
		IDataBaseOper oper; AcDbObjectIdArray tids;
		SelectFilter sf1(8, _T("分层平面")), sf2(RTDXF0, _T("*POLYLINE"));
		if(!SelectEntitys(tids, sf1, sf2, _T("X"))) return;
		if(!ReadXdata(aname(tids.first()), _T("楼栋号"), 0, info)) return;
		m_buildName = info; MStr filter; filter[_T("楼栋号")] = info;
		for(int indx = 0; indx < tids.length(); ++indx)
		{
			SplitMargeLayer sml; sml.m_bName = m_buildName;
			sml.m_id = tids[indx]; sml.m_lOper = kNoOper;
			ReadXdata(aname(sml.m_id), _T("楼层号"), 0, info);
			CString lch(info);
			int pos = lch.FindOneOf(_T(" ---> "));
			if(pos == -1)
				sml.m_sLayer = sml.m_eLayer = lch;
			else
			{
				sml.m_sLayer = lch.Left(pos);
				sml.m_eLayer = lch.Mid(pos + 7);
			}
			ReadXdata(aname(sml.m_id), _T("分层序号"), 0, info);
			m_smls[_ttoi(info)] = sml;
		}
	}
	else
	{
		if(!ReadXdata(aname(m_id), _T("楼栋号"), 0, info)) return;
		m_buildName = info;
	}
	int row = 0;
	for(int idx = records.size(); idx > 0; --idx)
	{
		int offset = 1;
		MStr record = records[idx - 1];
		CString val = record[_T("楼栋号")];
		if(val.CompareNoCase(m_buildName) != 0) continue;
		m_LayerInfos.InsertItem(row, _T(""));
		if(!m_isLayer)
		{
			if(record[_T("是否显示")].CompareNoCase(_T("1")) == 0)
				m_LayerInfos.SetCheck(row, TRUE);
			else m_LayerInfos.SetCheck(row, FALSE);
		}
		m_LayerInfos.SetItemText(row, offset++, m_buildName);
		m_LayerInfos.SetItemText(row, offset++, record[_T("起始层名")]);
		m_LayerInfos.SetItemText(row, offset++, record[_T("终止层名")]);
		m_LayerInfos.SetItemText(row, offset++, record[_T("实测层高")]);
		m_LayerInfos.SetItemText(row, offset++, record[_T("设计层高")]);
		m_LayerInfos.SetItemText(row, offset++, record[_T("层备注")]);
	}
}

void CNewSetLayerInfoDlg::OnBnClickedButtonMoveup()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_LayerInfos.GetSelectedItem();
	if(sel == 0) return;
	int total = m_LayerInfos.GetColumnCount();
	vector<CString> infos;
	for(int idx = 0; idx < total; ++idx)
	{
		CString info;
		if (idx == 0)
		{
			if (m_LayerInfos.GetCheck(sel))
				info = _T("1");
			else
				info = _T("0");;
		}
		else
			info = m_LayerInfos.GetItemText(sel, idx);
		infos.push_back(info);
	}
	m_LayerInfos.DeleteItem(sel);
	m_LayerInfos.InsertItem(sel - 1, _T(""));
	for (int idx = 0; idx < total; ++idx)
	{
		if (idx == 0)
		{
			if (infos[0].CompareNoCase(_T("1")) == 0)
				m_LayerInfos.SetCheck(sel-1, TRUE);
			else
				m_LayerInfos.SetCheck(sel-1, FALSE);
		}
		else
			m_LayerInfos.SetItemText(sel - 1, idx, infos[idx]);
	}
	m_LayerInfos.setSelectItem(sel - 1);
}

void CNewSetLayerInfoDlg::OnBnClickedButtonAddlayer()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_LayerInfos.GetItemCount();
	m_LayerInfos.InsertItem(count, _T(""));
	m_LayerInfos.SetCheck(count, 1);
	m_LayerInfos.SetItemText(count, 1, m_buildName);
}

void CNewSetLayerInfoDlg::OnBnClickedButtonMovedown()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_LayerInfos.GetSelectedItem();
	int count = m_LayerInfos.GetItemCount();
	if(sel == (count - 1)) return;
	int total = m_LayerInfos.GetColumnCount();
	vector<CString> infos;
	for(int idx = 0; idx < total; ++idx)
	{
		CString info;
		if (idx == 0)
		{
			if (m_LayerInfos.GetCheck(sel))
				info = _T("1");
			else
				info = _T("0");;
		}
		else
			info = m_LayerInfos.GetItemText(sel, idx);
		infos.push_back(info);
	}
	m_LayerInfos.DeleteItem(sel);
	m_LayerInfos.InsertItem(sel + 1, _T(""));
	for (int idx = 0; idx < total; ++idx)
	{
		if (idx == 0)
		{
			if (infos[0].CompareNoCase(_T("1")) == 0)
				m_LayerInfos.SetCheck(sel + 1, TRUE);
			else
				m_LayerInfos.SetCheck(sel + 1, FALSE);
		}
		else
		m_LayerInfos.SetItemText(sel + 1, idx, infos[idx]);
	}
	m_LayerInfos.setSelectItem(sel + 1);
}

void CNewSetLayerInfoDlg::OnBnClickedButtonDellayer()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_LayerInfos.GetSelectedItem();
	m_LayerInfos.DeleteItem(sel);
}

void CNewSetLayerInfoDlg::OnBnClickedButtonCopylayer()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_LayerInfos.GetSelectedItem();
	m_LayerInfos.InsertItem(count + 1, _T(""));
	int total = m_LayerInfos.GetColumnCount();
	BOOL flag = m_LayerInfos.GetCheck(count);
	m_LayerInfos.SetCheck(count + 1, flag);
	for(int idx = 1; idx < total; ++idx)
	{
		CString info = m_LayerInfos.GetItemText(count, idx);
		m_LayerInfos.SetItemText(count + 1, idx, info);
	}
	m_LayerInfos.setSelectItem(count + 1);
}

void CNewSetLayerInfoDlg::OnBnClickedButtonSplitlayer()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_LayerInfos.GetSelectedItem();
	m_LayerInfos.InsertItem(count + 1, m_buildName);
	int total = m_LayerInfos.GetColumnCount();
	for(int idx = 0; idx < total; ++idx)
	{
		CString info = m_LayerInfos.GetItemText(count, idx);
		m_LayerInfos.SetItemText(count + 1, idx, info);
	}
	for(int idx = m_smls.size(); idx > 0; --idx)
	{
		if(idx == count)
			m_smls[idx].m_lOper = kRname;
		if(idx < count + 1) continue;
		m_smls[idx] = m_smls[idx - 1];
		if(idx == count + 1)
			m_smls[idx].m_lOper = kCopy;
	}
	m_LayerInfos.setSelectItem(count + 1);
}

void CNewSetLayerInfoDlg::OnBnClickedButtonMagerlayer()
{
	// TODO:  在此添加控件通知处理程序代码
	int total = m_LayerInfos.GetColumnCount();
	BOOL isCheck = FALSE; int selRow = 0, _row;
	for(; selRow < total; ++selRow)
		if((isCheck = m_LayerInfos.GetCheck(selRow))) break;
	vector<int> sels; m_LayerInfos.GetSelectedItem(sels);
	if(!isCheck || sels.size() != 2)
	{ MessageBox(_T("请选择主要楼层或没有选择合并的楼层"), _T("错误提示"), 0); return; }
	_row = (selRow == sels[0]) ? sels[1] : sels[0]; CString slyr;
	if(abs(_row - selRow) != 1)
	{ MessageBox(_T("选择待合并的楼层错误"), _T("错误提示"), 0); return; }
	for(int idx = 0; idx < sels.size(); ++idx)
	{
		CString lName = m_LayerInfos.GetItemText(sels[idx], 2);
		if(lName.FindOneOf(_T(";")) != -1)
		{
			MessageBox(_T("跃层不支持合并"), _T("错误信息"), 0);
			return;
		}
	}
	{
		CString lName = m_LayerInfos.GetItemText(selRow, 2);
		if(lName.FindOneOf(_T(";")) != -1)
		{
			MessageBox(_T("跃层不支持合并"), _T("错误信息"), 0);
			return;
		}
	}
	if(_row > selRow)
	{
		slyr = m_LayerInfos.GetItemText(_row, 2);
		CString elyr = m_LayerInfos.GetItemText(_row, 3);
		if(elyr.GetLength() == 0)
		{
			m_LayerInfos.SetItemText(selRow, 3, slyr);
			m_smls[selRow].m_sLayer = slyr;
		}
		else
		{
			m_LayerInfos.SetItemText(selRow, 3, elyr);
			m_smls[selRow].m_sLayer = elyr;
		}
	}
	else
	{
		slyr = m_LayerInfos.GetItemText(_row, 2);
		m_LayerInfos.SetItemText(selRow, 2, slyr);
		m_smls[selRow].m_sLayer = slyr;
	}
	m_smls[selRow].m_lOper = kRname;
	m_smls[_row].m_lOper = kDel;
	m_LayerInfos.DeleteItem(_row);
}

SplitMargeLayers CNewSetLayerInfoDlg::getSplitMargeLayers()
{
	map<int, SplitMargeLayer>::iterator it = m_smls.begin();
	SplitMargeLayers smls;
	for(; it != m_smls.end(); ++it)
		smls.push_back(it->second);
	return smls;
}