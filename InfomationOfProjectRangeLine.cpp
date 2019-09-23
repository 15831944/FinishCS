// InfomationOfProjectRangeLine.cpp : 实现文件
//

#include "stdafx.h"
#include "Global.h"
#include "InfomationOfProjectRangeLine.h"
#include "afxdialogex.h"
#include "DatabaseOperManager.h"


// InfomationOfProjectRangeLine 对话框

IMPLEMENT_DYNAMIC(InfomationOfProjectRangeLine, CDialog)

InfomationOfProjectRangeLine::InfomationOfProjectRangeLine(CWnd* pParent /*=NULL*/)
: CDialog(InfomationOfProjectRangeLine::IDD, pParent), m_dbManager(new CDatabaseOperManager)
{
	TCHAR mdbPath[255] = {0};
	if(5100 != ads_findfile(_T("lstitems.mdb"), mdbPath))
	{
		MessageBox(_T("无法找到[lstitems]数据库文件"), _T("错误信息"), 0);
		return;
	}
	if(!m_dbManager->initialConnect(mdbPath))
	{
		MessageBox(_T("初始化数据库[lstitems]文件失败"), _T("错误信息"), 0);
		return;
	}
}

InfomationOfProjectRangeLine::~InfomationOfProjectRangeLine()
{
}

void InfomationOfProjectRangeLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SQDWXXFRMC, m_frmc);
	DDX_Control(pDX, IDC_SQDWXXFRZJH, m_frzjh);
	DDX_Control(pDX, IDC_SQDWXXFRZJZL, m_frzjzl);
	DDX_Control(pDX, IDC_SQDWXXSQRLX, m_sqrlx);
	DDX_Control(pDX, IDC_SQDWXXSQRMC, m_SQRMC);
	DDX_Control(pDX, IDC_SQDWXXSXH, m_SXH);
	DDX_Control(pDX, IDC_SQDWXXZJH, m_zjh);
	DDX_Control(pDX, IDC_SQDWXXZJZL, m_zjzl);
	DDX_Control(pDX, IDC_XMJBXXCHDW, m_chdw);
	DDX_Control(pDX, IDC_XMJBXXDSJZMJ, m_dsjzmj);
	DDX_Control(pDX, IDC_XMJBXXDXJZMJ, m_dxjzmj);
	DDX_Control(pDX, IDC_XMJBXXLXDH, m_lxdh);
	DDX_Control(pDX, IDC_XMJBXXLXR, m_lxr);
	DDX_Control(pDX, IDC_XMJBXXQXDM, m_qxdm);
	DDX_Control(pDX, IDC_XMJBXXXMBH, m_xmbh);
	DDX_Control(pDX, IDC_XMJBXXXMMC, m_xmmc);
	DDX_Control(pDX, IDC_XMJBXXYDDW, m_yddw);
	DDX_Control(pDX, IDC_XMJBXXYWFX, m_ywfx);
	DDX_Control(pDX, IDC_XMJBXXYWLX, m_ywlx);
	DDX_Control(pDX, IDC_XMJBXXZLKZ, m_zlkz);
	DDX_Control(pDX, IDC_XMJBXXZS, m_zs);
	DDX_Control(pDX, IDC_XMJBXXYDMJ, m_ydmj);
	DDX_Control(pDX, IDC_XMJBXXZJZMJ, m_zjzmj);
}

BEGIN_MESSAGE_MAP(InfomationOfProjectRangeLine, CDialog)
END_MESSAGE_MAP()

// InfomationOfProjectRangeLine 消息处理程序

void InfomationOfProjectRangeLine::setObject(const AcDbObjectId &id)
{
	m_obj = id;
}

BOOL InfomationOfProjectRangeLine::OnInitDialog()
{
	CDialog::OnInitDialog(); TCHAR xdata[255] = {0};
// 	IProjectMDB pdb; TCHAR dbPath[255] = {0};
// 	MStr record = pdb.getProjectInfo();
// 	CString xmmc = record[_T("项目名称")];
// 	if(5100 != ads_findfile(_T("xmjg.mdb"), dbPath)) return;
// 	IDataBaseOper dbOper;

	ReadXdata(aname(m_obj), _T("FRMC"), 0, xdata); m_frmc.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("frzjh"), 0, xdata); m_frzjh.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("FRZJZL"), 0, xdata); initComboBoxValue(_T("TBL_ZJLX"), m_frzjzl, xdata);
	ReadXdata(aname(m_obj), _T("SQRLX"), 0, xdata); initComboBoxValue(_T("TBL_SQRLX"), m_sqrlx, xdata);
	ReadXdata(aname(m_obj), _T("SQRMC"), 0, xdata); m_SQRMC.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("SXH"), 1, xdata); m_SXH.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("zjh"), 0, xdata); m_zjh.SetWindowText(xdata);
	
	ReadXdata(aname(m_obj), _T("zjzl"), 0, xdata); initComboBoxValue(_T("TBL_ZJLX"), m_zjzl, xdata);
	ReadXdata(aname(m_obj), _T("chdw"), 0, xdata); m_chdw.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("DSJZMJ"), 3, xdata); _stprintf(xdata, _T("%.2lf"), _tstof(xdata)); m_dsjzmj.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("DXJZMJ"), 3, xdata); _stprintf(xdata, _T("%.2lf"), _tstof(xdata));  m_dxjzmj.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("lxdh"), 0, xdata); m_lxdh.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("lxr"), 0, xdata); m_lxr.SetWindowText(xdata);

	ReadXdata(aname(m_obj), _T("QXDM"), 0, xdata); initComboBoxValue(_T("TBL_QXDM"), m_qxdm, xdata);

	ReadXdata(aname(m_obj), _T("xmbh"), 0, xdata);
	CString XMBH(xdata); int pos = XMBH.GetLength() - 9;
	m_xmbh.SetWindowText(XMBH.Mid(pos));

	ReadXdata(aname(m_obj), _T("xmmc"), 0, xdata); m_xmmc.SetWindowText(xdata);

	ReadXdata(aname(m_obj), _T("yddw"), 0, xdata); m_yddw.SetWindowText(xdata);

	ReadXdata(aname(m_obj), _T("ywfx"), 0, xdata); initComboBoxValue(_T("TBL_YWFX"), m_ywfx, xdata, false);

	ReadXdata(aname(m_obj), _T("ywlx"), 0, xdata); initComboBoxValue(_T("TBL_YWLX"), m_ywlx, xdata, false); //m_ywlx.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("ZL"), 0, xdata); m_zlkz.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("zs"), 1, xdata); m_zs.SetWindowText(xdata);

	AcDbObjectId hxid; SelectFilter sf1(8, _T("红线"));
	SelectFilter sf2(5020, _T("*POLYLINE"));
	_stprintf(xdata, _T("%.2lf"), 0.00f);
	if(SelectEntity(hxid, sf1, sf2, _T("X")))
	{
		AcDbEntity *pEnt = NULL;
		if(Acad::eOk == acdbOpenAcDbEntity(pEnt, hxid, AcDb::kForRead))
		{
			double area = 0.0;
			AcDbPolyline *line = (AcDbPolyline *)pEnt;
			if(Adesk::kTrue == line->isClosed())
				line->getArea(area); pEnt->close();
			_stprintf(xdata, _T("%.2lf"), area);
		}
	}
	m_ydmj.SetWindowText(xdata);
	ReadXdata(aname(m_obj), _T("ZJZMJ"), 3, xdata); _stprintf(xdata, _T("%.2lf"), _tstof(xdata));  m_zjzmj.SetWindowText(xdata);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void InfomationOfProjectRangeLine::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	CString xdata, errInfo;
	m_frmc.GetWindowText(xdata);          AddXdata(aname(m_obj), _T("FRMC"), 0, xdata);
	m_frzjh.GetWindowText(xdata);         AddXdata(aname(m_obj), _T("frzjh"), 0, xdata);

	m_frzjzl.GetWindowText(xdata);   getComboBoxCode(_T("FRZJZL"), xdata);

	m_sqrlx.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("申请人类型、");
	else getComboBoxCode(_T("SQRLX"), xdata);

	m_SQRMC.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("申请人名称、");
	else AddXdata(aname(m_obj), _T("SQRMC"), 0, xdata);

	m_SXH.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("顺序号、");
	else AddXdata(aname(m_obj), _T("SXH"), 1, xdata);

	m_zjh.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("证件号、");
	else AddXdata(aname(m_obj), _T("zjh"), 0, xdata);

	m_zjzl.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("证件种类、");
	else getComboBoxCode(_T("ZJZL"), xdata);

	m_chdw.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("测绘单位、");
	else AddXdata(aname(m_obj), _T("chdw"), 0, xdata);

	m_dsjzmj.GetWindowText(xdata);
	AddXdata(aname(m_obj), _T("DSJZMJ"), 3, xdata);
	m_dxjzmj.GetWindowText(xdata);
	AddXdata(aname(m_obj), _T("DXJZMJ"), 3, xdata);

	m_lxdh.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("联系电话、");
	else AddXdata(aname(m_obj), _T("lxdh"), 0, xdata);

	m_lxr.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("联系人、");
	else AddXdata(aname(m_obj), _T("lxr"), 0, xdata);

	m_qxdm.GetWindowText(xdata);
	if(xdata.IsEmpty())
	{
		MessageBox(_T("区划代码为空"), _T("错误信息"), 0);
		return;
	}

	getComboBoxCode(_T("QXDM"), xdata);
	m_xmbh.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("项目编号、");
	else AddXdata(aname(m_obj), _T("xmbh"), 0, xdata);

	m_xmmc.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("项目名称、");
	else AddXdata(aname(m_obj), _T("xmmc"), 0, xdata);

	m_yddw.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("用地单位、");
	else AddXdata(aname(m_obj), _T("yddw"), 0, xdata);

	m_ywfx.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("业务分项、");
	else getComboBoxCode(_T("YWFX"), xdata);

	m_ywlx.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("业务类型、");
	else getComboBoxCode(_T("ywlx"), xdata);

	m_zlkz.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("坐落扩展、");
	else AddXdata(aname(m_obj), _T("ZL"), 0, xdata);

	m_zs.GetWindowText(xdata);
	AddXdata(aname(m_obj), _T("zs"), 1, xdata);

	m_ydmj.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("用地面积、");
	else AddXdata(aname(m_obj), _T("YDMJ"), 3, xdata);

	m_zjzmj.GetWindowText(xdata);
	if(xdata.IsEmpty()) errInfo += _T("总建筑面积");
	else AddXdata(aname(m_obj), _T("ZJZMJ"), 3, xdata);
	if(!errInfo.IsEmpty())
	{
		errInfo += _T("等字段为空");
		MessageBox(errInfo, _T("错误信息"), 0);
		return;
	}
	CDialog::OnOK();
}

void InfomationOfProjectRangeLine::getComboBoxCode(const CString &table, const CString &value, bool showCode)
{
	int pos = value.Find(_T(' '));
	CString code = (pos == -1) ? value : value.Mid(0, pos);
	AddXdata(aname(m_obj), table, 0, code);
}

void InfomationOfProjectRangeLine::initComboBoxValue(const CString &table, CComboBox &box, const TCHAR *xdata, bool showCode)
{
	VMStr records;
	if(!m_dbManager->ReadDataBase_all(table, records))
	{
		MessageBox(_T("无法读取数据库中的[TBL_ZJLX]表"), _T("错误信息"), 0);
		return;
	}
	CString code, value, val, temp;
	for(int idx = 0; idx < records.size(); ++idx)
	{
		MStr record = records[idx];
		value = record["VALUE"];
		if(showCode)
		{
			code = record["CODE"];
			temp = code; temp += _T(" ");
			temp += value;
			if(code.CompareNoCase(xdata) == 0)
				val = temp;
		}
		else
		{
			temp = value;
			if(temp.CompareNoCase(xdata) == 0)
				val = temp;
		}
		box.AddString(temp);
	}
	if(val.IsEmpty())
		box.SetCurSel(0);
	else
		box.SetWindowText(val);
	return;
}