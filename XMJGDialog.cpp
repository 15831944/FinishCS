// XMJGExportDlg.cpp : 实现文件
//

#include "stdafx.h"

#ifndef XMJGEXPORTDLG
#include "XMJGDialog.h"
#endif /*XMJGEXPORTDLG*/

#include "afxdialogex.h"
#include "cassdog.h"
#include "Global.h"
#include <tchar.h>
#include "cassGlobal.h"
#include "DatabaseOperManager.h"
#include "FilePathOperater.h"
#include "afxmsg_.h"
#include "carinfo.h"
#include "adscodes.h"
#include "XMJGMacro.h"
#include "XMJGReadData.h"
#include "XRecordOperate.h"
#include "ZipFunction.h"
#include "SaveBlocks.h"
#include "AttributeDef.h"
#include "gdal_priv.h"
#include "SYPSampleJJG.h"

extern AttributeDef g_attribute;
extern FieldChangeTable g_fieldchagetable;
extern bool g_pipelinedlg;

#pragma region 业务一信息设置对话框

IMPLEMENT_DYNAMIC(XMJGSetInfo1, CDialog)

XMJGSetInfo1::XMJGSetInfo1(bool isCreate, CWnd* pParent /*=NULL*/)
: CDialog(XMJGSetInfo1::IDD, pParent), m_isCreate(isCreate)
{

}

XMJGSetInfo1::~XMJGSetInfo1()
{

}

void XMJGSetInfo1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCEPTCODE1, m_AcceptCode);
	DDX_Control(pDX, IDC_ENTRUSTCOMPANY1, m_EntrustCompany);
	DDX_Control(pDX, IDC_BUILDPROJECT1, m_BuildProject);
	DDX_Control(pDX, IDC_MEANSURECONTEXT1, m_MeansureContext);
	DDX_Control(pDX, IDC_EDIT_XMJG1PROJECTCONTECT, m_projectcontext);
}

BEGIN_MESSAGE_MAP(XMJGSetInfo1, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetInfo1::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL XMJGSetInfo1::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_AcceptCode.SetReadOnly(!m_isCreate);
	IDataBaseOper db; MStr filter; VStr record;
	filter[_T("关键字")] = _T("放样类型");
	db.readDataBase(_T("Common"), _T("属性值"), filter, record);
	int len = (int)record.size();
	for(int idx = 0; idx < len; ++idx)
		m_MeansureContext.InsertString(idx, record[idx]);
	m_MeansureContext.SetCurSel(0);
	if(m_isCreate == false)
	{
		IProjectMDB pdb;
		MStr info = pdb.getProjectInfo();
		if(info[_T("测量内容")].GetLength() != 0)
			m_MeansureContext.SetWindowText(info[_T("测量内容")]);
		m_AcceptCode.SetWindowText(info[_T("受理号")]);
		m_BuildProject.SetWindowText(info[_T("项目名称")]);
		m_EntrustCompany.SetWindowText(info[_T("委托单位")]);
	}
	m_projectcontext.SetWindowText(db.readPathTable(_T("工程目录")) + _T("红线放样\\"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGSetInfo1::OnBnClickedOk()
{
	CString value; MStr record; int nNum = 0;
	m_AcceptCode.GetWindowText(record[_T("受理号")]);
	m_EntrustCompany.GetWindowText(record[_T("委托单位")]);
	m_BuildProject.GetWindowText(record[_T("项目名称")]);
	m_MeansureContext.GetWindowText(record[_T("测量内容")]);
	if(m_isCreate == true)
	{
		IProjectInfo pi(_T("红线放样"));
		if(false == pi.CreateProject(_T("红线放样"), record)) return;
	}
	else
	{
		IProjectMDB pdb;
		pdb.setJBXXTableInfo(record);
	}
	IDataBaseOper db; CString str; MStr filter;
	filter[_T("关键字")] = _T("放样类型");
	filter[_T("属性值")] = record[_T("测量内容")];
	db.readDataBase(_T("Common"), _T("属性值1"), filter, str);
	m_entityColor = _ttoi(str);
	CDialog::OnOK();
}

int XMJGSetInfo1::getEntityColor()
{
	return m_entityColor;
}

#pragma endregion

#pragma region 业务二信息设置对话框

XMJGSetInfo2 *XMJGSetInfo2::_setInfo = NULL;

IMPLEMENT_DYNAMIC(XMJGSetInfo2, CAcUiDialog)

XMJGSetInfo2::XMJGSetInfo2(bool isCreate, CWnd* pParent /*=NULL*/, HINSTANCE hInstance)
: CAcUiDialog(XMJGSetInfo2::IDD, pParent, hInstance), m_isCreate(isCreate)
{

}

XMJGSetInfo2::~XMJGSetInfo2()
{

}

XMJGSetInfo2 *XMJGSetInfo2::Instance(bool isCreate)
{
	if(NULL == _setInfo)
	{
		_setInfo = new XMJGSetInfo2(isCreate);
		_setInfo->Create(XMJGSetInfo2::IDD, NULL);
	}
	return _setInfo;
}

void XMJGSetInfo2::destory()
{
	if(_setInfo != NULL)
		delete _setInfo;
	_setInfo = NULL;
}

void XMJGSetInfo2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUILDAREA2, m_BuildArea);
	DDX_Control(pDX, IDC_ACCEPTCODE2, m_AcceptCode);
	DDX_Control(pDX, IDC_PROJECTNAME2, m_ProjectName);
	DDX_Control(pDX, IDC_LICENSECODE2, m_LicenseCode);
	DDX_Control(pDX, IDC_BUILDCOMPANY2, m_BuildCompany);
	DDX_Control(pDX, IDC_PROJECTADD2, m_ProjectAddress);
	DDX_Control(pDX, IDC_EDIT_PROJECTDIR2, m_projectdir);
}

BEGIN_MESSAGE_MAP(XMJGSetInfo2, CAdUiDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetInfo2::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL XMJGSetInfo2::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_AcceptCode.SetReadOnly(!m_isCreate);
	if(m_isCreate == false)
	{
		IProjectMDB pdb; MStr record = pdb.getProjectInfo();
		m_AcceptCode.SetWindowText(record[_T("受理号")]);
		m_BuildArea.SetWindowText(record[_T("用地面积")]);
		m_ProjectName.SetWindowText(record[_T("项目名称")]);
		m_LicenseCode.SetWindowText(record[_T("许可证号")]);
		m_BuildCompany.SetWindowText(record[_T("建设单位")]);
		m_ProjectAddress.SetWindowText(record[_T("项目地点")]);
	}
	IDataBaseOper db;
	m_projectdir.SetWindowText(db.readPathTable(_T("工程目录")) + _T("建筑放样\\"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

#ifdef _WIN64
LRESULT XMJGSetInfo2::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}
#else
LONG XMJGSetInfo2::onAcadKeepFocus(UINT n, LONG l)
{
	return TRUE;
}
#endif

void XMJGSetInfo2::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString value; MStr context;
	m_AcceptCode.GetWindowText(context[_T("受理号")]);
	m_BuildArea.GetWindowText(context[_T("用地面积")]);
	m_ProjectName.GetWindowText(context[_T("项目名称")]);
	m_LicenseCode.GetWindowText(context[_T("许可证号")]);
	m_BuildCompany.GetWindowText(context[_T("建设单位")]);
	m_ProjectAddress.GetWindowText(context[_T("项目地点")]);
	if(m_isCreate == true)
	{
		IProjectInfo pi(_T("建筑放样"));
		if(false == pi.CreateProject(_T("建筑放样检查表"), context)) return;
	}
	else
	{
		IProjectMDB pdb;
		pdb.setJBXXTableInfo(context);
	}
	CDialog::OnOK();
}

void XMJGSetInfo2::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	__super::OnClose();
}

#pragma endregion

#pragma region 业务三信息设置对话框

IMPLEMENT_DYNAMIC(XMJGSetInfo3, CDialog)

XMJGSetInfo3::XMJGSetInfo3(bool isCreate, CWnd* pParent /*=NULL*/)
: CDialog(XMJGSetInfo3::IDD, pParent), m_isCreate(isCreate)
{

}

XMJGSetInfo3::~XMJGSetInfo3()
{
}

void XMJGSetInfo3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKCODE3, m_CheckCode);
	DDX_Control(pDX, IDC_BUILDAREA3, m_BuildArea);
	DDX_Control(pDX, IDC_ACCEPTCODE3, m_AcceptCode);
	DDX_Control(pDX, IDC_PROJECTNAME3, m_ProjectName);
	DDX_Control(pDX, IDC_LICENESECODE3, m_LicenseCode);
	DDX_Control(pDX, IDC_PROJECTADDRESS3, m_ProjectAdd);
	DDX_Control(pDX, IDC_BUILDPROJECT3, m_BuildCompany);
	DDX_Control(pDX, IDC_EDIT_PROJECTDIR3, m_projectdir);
}

BEGIN_MESSAGE_MAP(XMJGSetInfo3, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetInfo3::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL XMJGSetInfo3::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_AcceptCode.SetReadOnly(!m_isCreate);
	if(m_isCreate == false)
	{
		IProjectMDB pdb; MStr context = pdb.getProjectInfo();
		m_CheckCode.SetWindowText(context[_T("验字号")]);
		m_AcceptCode.SetWindowText(context[_T("受理号")]);
		m_BuildArea.SetWindowText(context[_T("用地面积")]);
		m_ProjectAdd.SetWindowText(context[_T("项目地点")]);
		m_ProjectName.SetWindowText(context[_T("项目名称")]);
		m_LicenseCode.SetWindowText(context[_T("许可证号")]);
		m_BuildCompany.SetWindowText(context[_T("建设单位")]);
	}
	IDataBaseOper db;
	m_projectdir.SetWindowText(db.readPathTable(_T("工程目录")) + _T("道路放样\\"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGSetInfo3::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	MStr context;
	m_CheckCode.GetWindowText(context[_T("验字号")]);
	m_AcceptCode.GetWindowText(context[_T("受理号")]);
	m_BuildArea.GetWindowText(context[_T("用地面积")]);
	m_ProjectAdd.GetWindowText(context[_T("项目地点")]);
	m_ProjectName.GetWindowText(context[_T("项目名称")]);
	m_LicenseCode.GetWindowText(context[_T("许可证号")]);
	m_BuildCompany.GetWindowText(context[_T("建设单位")]);
	if(m_isCreate == true)
	{
		IProjectInfo pi(_T("道路放样"));
		if(false == pi.CreateProject(_T("道路放样检查表"), context)) return;
	}
	else
	{
		IProjectMDB pdb;
		pdb.setJBXXTableInfo(context);
	}
	CDialog::OnOK();
}

#pragma endregion

#pragma region 业务四信息设置对话框

IMPLEMENT_DYNAMIC(XMJGSetInfo4, CDialog)

XMJGSetInfo4::XMJGSetInfo4(bool isCreate, CWnd* pParent /*=NULL*/)
: CDialog(XMJGSetInfo4::IDD, pParent), m_isCreate(isCreate)
{

}

XMJGSetInfo4::~XMJGSetInfo4()
{

}

void XMJGSetInfo4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCEPTCODE4, m_AcceptCode);
	DDX_Control(pDX, IDC_BUILDPROJECT4, m_ProjectName);
	DDX_Control(pDX, IDC_ENTRUSTCOMPANY4, m_EntrustCompany);
	DDX_Control(pDX, IDC_EDIT_PROJECTDIR4, m_projectdir);
}

BEGIN_MESSAGE_MAP(XMJGSetInfo4, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetInfo4::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL XMJGSetInfo4::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_AcceptCode.SetReadOnly(!m_isCreate);
	if(m_isCreate == false)
	{
		IProjectMDB pdb; MStr context = pdb.getProjectInfo();
		m_AcceptCode.SetWindowText(context[_T("受理号")]);
		m_ProjectName.SetWindowText(context[_T("项目名称")]);
		m_EntrustCompany.SetWindowText(context[_T("委托单位")]);
	}
	IDataBaseOper db;
	m_projectdir.SetWindowText(db.readPathTable(_T("工程目录")) + _T("水准高程\\"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGSetInfo4::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	MStr context;
	m_AcceptCode.GetWindowText(context[_T("受理号")]);
	m_ProjectName.GetWindowText(context[_T("项目名称")]);
	m_EntrustCompany.GetWindowText(context[_T("委托单位")]);
	if(m_isCreate == true)
	{
		IProjectInfo pi(_T("水准高程"));
		if(false == pi.CreateProject(_T("水准高程"), context)) return;
	}
	else
	{
		IProjectMDB pdb;
		pdb.setJBXXTableInfo(context);
	}
	CDialog::OnOK();
}

void XMJGSetInfo4::getWindowControl(MStr &context)
{
	CString value;
	m_AcceptCode.GetWindowText(value); context[_T("受理号")] = value;
	m_ProjectName.GetWindowText(value); context[_T("项目名称")] = value;
	m_EntrustCompany.GetWindowText(value); context[_T("委托单位")] = value;
}

void XMJGSetInfo4::setWindowControl(MStr &context)
{
	m_AcceptCode.SetWindowText(context[_T("受理号")]);
	m_ProjectName.SetWindowText(context[_T("项目名称")]);
	m_EntrustCompany.SetWindowText(context[_T("委托单位")]);
}

#pragma endregion

#pragma region 业务五信息设置对话框

XMJGSetInfo5 *XMJGSetInfo5::_setinfo = NULL;

IMPLEMENT_DYNAMIC(XMJGSetInfo5, CAcUiDialog)

XMJGSetInfo5::XMJGSetInfo5(bool isCreate, CWnd* pParent /*=NULL*/)
: CAcUiDialog(XMJGSetInfo5::IDD, pParent), m_isCreate(isCreate)
{

}

XMJGSetInfo5::~XMJGSetInfo5()
{

}

void XMJGSetInfo5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCEPTCODE5, m_AcceptCode);
	DDX_Control(pDX, IDC_BUILDCOMPANY5, m_BuildCompany);
	DDX_Control(pDX, IDC_PROJECTNAME5, m_ProjectName);
	DDX_Control(pDX, IDC_PROJECTADD5, m_ProjectAdd);
	DDX_Control(pDX, IDC_LICENSECODE5, m_LicenseCode);
	DDX_Control(pDX, IDC_CHECKCODE5, m_CheckCode);
	DDX_Control(pDX, IDC_INFOMATION5, m_Infomation);
	DDX_Control(pDX, IDC_BUILDAREA5, m_BuildArea);
	DDX_Control(pDX, IDC_EDIT2_PROJECTDIR5, m_projectdir);
}

BEGIN_MESSAGE_MAP(XMJGSetInfo5, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetInfo5::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()

BOOL XMJGSetInfo5::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_AcceptCode.SetReadOnly(!m_isCreate);
	if(m_isCreate == false)
	{
		IProjectMDB pdb; MStr context = pdb.getProjectInfo();
		m_CheckCode.SetWindowText(context[_T("验字号")]);
		m_AcceptCode.SetWindowText(context[_T("受理号")]);
		m_BuildArea.SetWindowText(context[_T("用地面积")]);
		m_ProjectAdd.SetWindowText(context[_T("项目地点")]);
		m_Infomation.SetWindowText(context[_T("备注信息")]);
		m_ProjectName.SetWindowText(context[_T("项目名称")]);
		m_LicenseCode.SetWindowText(context[_T("许可证号")]);
		m_BuildCompany.SetWindowText(context[_T("建设单位")]);
	}
	IDataBaseOper db;
	m_projectdir.SetWindowText(db.readPathTable(_T("工程目录")) + _T("正负零线\\"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGSetInfo5::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	MStr context;
	m_CheckCode.GetWindowText(context[_T("验字号")]);
	m_AcceptCode.GetWindowText(context[_T("受理号")]);
	m_BuildArea.GetWindowText(context[_T("用地面积")]);
	m_Infomation.GetWindowText(context[_T("备注信息")]);
	m_ProjectAdd.GetWindowText(context[_T("项目地点")]);
	m_ProjectName.GetWindowText(context[_T("项目名称")]);
	m_LicenseCode.GetWindowText(context[_T("许可证号")]);
	m_BuildCompany.GetWindowText(context[_T("建设单位")]);
	if(m_isCreate == true)
	{
		IProjectInfo pi(_T("正负零线"));
		if(false == pi.CreateProject(_T("正负零线意见表"), context)) return;
	}
	else
	{
		IProjectMDB pdb;
		pdb.setJBXXTableInfo(context);
	}
	CDialog::OnOK();
}

void XMJGSetInfo5::destory()
{
	if(_setinfo != NULL)
		delete _setinfo;
	_setinfo = NULL;
}

XMJGSetInfo5 *XMJGSetInfo5::Instance(bool isCreate)
{
	if(_setinfo == NULL)
	{
		_setinfo = new XMJGSetInfo5(isCreate);
		_setinfo->Create(XMJGSetInfo5::IDD);
	}
	return _setinfo;
}

#ifdef _WIN64
LRESULT XMJGSetInfo5::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}
#else
LONG XMJGSetInfo5::onAcadKeepFocus(UINT n, LONG l)
{
	return TRUE;
}
#endif

void XMJGSetInfo5::getWindowControl(MStr &context)
{
	CString value;
	m_CheckCode.GetWindowText(value); context[_T("验字号")] = value;
	m_AcceptCode.GetWindowText(value); context[_T("受理号")] = value;
	m_BuildArea.GetWindowText(value); context[_T("用地面积")] = value;
	m_ProjectAdd.GetWindowText(value); context[_T("项目地点")] = value;
	m_Infomation.GetWindowText(value); context[_T("备注信息")] = value;
	m_ProjectName.GetWindowText(value); context[_T("项目名称")] = value;
	m_LicenseCode.GetWindowText(value); context[_T("许可证号")] = value;
	m_BuildCompany.GetWindowText(value); context[_T("建设单位")] = value;
}

void XMJGSetInfo5::setWindowControl(MStr &context)
{
}

#pragma endregion

#pragma region 业务六信息设置对话框

IMPLEMENT_DYNAMIC(XMJGSetInfo6, CAcUiDialog)

XMJGSetInfo6::XMJGSetInfo6(bool isCreate, CWnd* pParent /*=NULL*/)
: CAcUiDialog(XMJGSetInfo6::IDD, pParent), m_isCreate(isCreate)
{

}

XMJGSetInfo6::~XMJGSetInfo6()
{

}

void XMJGSetInfo6::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKCODEGPB, m_CheckCode);
	DDX_Control(pDX, IDC_ACCEPTCODEGPB, m_AcceptCode);
	DDX_Control(pDX, IDC_PROJECTADDGPB, m_ProjectAdd);
	DDX_Control(pDX, IDC_EDIT_BRIDGETYPE, m_BridgeType);
	DDX_Control(pDX, IDC_PROJECTNAMEGPB, m_ProjectName);
	DDX_Control(pDX, IDC_LICENSECODEGPB, m_LicenseCode);
	DDX_Control(pDX, IDC_EDIT_PROJECTDIR6, m_projectdir);
	DDX_Control(pDX, IDC_INFOMATIONGPB, m_MeasureCompany);
	DDX_Control(pDX, IDC_EDIT_ENTRUSTMENTUNIT, m_Entrustment);
	DDX_Control(pDX, IDC_EDIT_PIPELINEBUILDPROJECT, m_pipelinebuildproject);
	DDX_Control(pDX, IDC_EDIT_PIPELINEBUILDCOMPANY, m_pipelinebuildcompany);
	DDX_Control(pDX, IDC_EDIT_PIPELINEBUILDAREA, m_pipelinebuildarea);
}

BEGIN_MESSAGE_MAP(XMJGSetInfo6, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetInfo6::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL XMJGSetInfo6::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_AcceptCode.SetReadOnly(!m_isCreate);
	IDataBaseOper oper; VStr cldws;
	CString cldw = oper.readJGCommon(_T("测量单位"));
	AnalyzeData(cldws, cldw, _T(";"));
	for(int idx = 0; idx < (int)cldws.size(); ++idx)
		m_MeasureCompany.InsertString(idx, cldws[idx]);
	if(m_isCreate == false)
	{
		IProjectMDB pdb; MStr context = pdb.getProjectInfo();
		m_CheckCode.SetWindowText(context[_T("验字号")]);
		m_AcceptCode.SetWindowText(context[_T("受理号")]);
		m_ProjectAdd.SetWindowText(context[_T("项目地点")]);
		m_BridgeType.SetWindowText(context[_T("天桥类型")]);
		m_Entrustment.SetWindowText(context[_T("委托单位")]);
		m_ProjectName.SetWindowText(context[_T("项目名称")]);
		m_LicenseCode.SetWindowText(context[_T("许可证号")]);
		m_MeasureCompany.SetWindowText(context[_T("测量单位")]);
	}
	else
		m_MeasureCompany.SetWindowText(_T("厦门市测绘与基础地理信息中心"));
	m_projectdir.SetWindowText(oper.readPathTable(_T("工程目录")) + _T("市政天桥\\"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGSetInfo6::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	MStr context;
	m_CheckCode.GetWindowText(context[_T("验字号")]);
	m_AcceptCode.GetWindowText(context[_T("受理号")]);
	m_ProjectAdd.GetWindowText(context[_T("项目地点")]);
	m_BridgeType.GetWindowText(context[_T("天桥类型")]);
	m_Entrustment.GetWindowText(context[_T("委托单位")]);
	m_ProjectName.GetWindowText(context[_T("项目名称")]);
	m_LicenseCode.GetWindowText(context[_T("许可证号")]);
	m_MeasureCompany.GetWindowText(context[_T("测量单位")]);
	m_pipelinebuildproject.GetWindowText(context[_T("建设项目")]);
	m_pipelinebuildcompany.GetWindowText(context[_T("建设单位")]);
	m_pipelinebuildarea.GetWindowText(context[_T("建筑面积")]);
	if(m_isCreate == true)
	{
		IProjectInfo pi(_T("人行天桥"));
		if(false == pi.CreateProject(context)) return;
	}
	else
	{
		IProjectMDB pdb;
		pdb.setJBXXTableInfo(context);
	}
	CAcUiDialog::OnOK();
}

void XMJGSetInfo6::getWindowControl(MStr &context)
{
	CString value;
	m_CheckCode.GetWindowText(value); context[_T("验字号")] = value;
	m_AcceptCode.GetWindowText(value); context[_T("受理号")] = value;
	m_ProjectAdd.GetWindowText(value); context[_T("项目地点")] = value;
	m_BridgeType.GetWindowText(value); context[_T("天桥类型")] = value;
	m_Entrustment.GetWindowText(value); context[_T("委托单位")] = value;
	m_ProjectName.GetWindowText(value); context[_T("项目名称")] = value;
	m_LicenseCode.GetWindowText(value); context[_T("许可证号")] = value;
	m_MeasureCompany.GetWindowText(value); context[_T("测量单位")] = value;
}

void XMJGSetInfo6::setWindowControl(MStr &context)
{
	m_CheckCode.SetWindowText(context[_T("验字号")]);
	m_AcceptCode.SetWindowText(context[_T("受理号")]);
	m_BridgeType.SetWindowText(context[_T("天桥类型")]);
	m_ProjectAdd.SetWindowText(context[_T("项目地点")]);
	m_Entrustment.SetWindowText(context[_T("委托单位")]);
	m_ProjectName.SetWindowText(context[_T("项目名称")]);
	m_LicenseCode.SetWindowText(context[_T("许可证号")]);
	m_MeasureCompany.SetWindowText(context[_T("测量单位")]);
}

#pragma endregion

#pragma region 业务七信息设置对话框

IMPLEMENT_DYNAMIC(XMJGSetInfo7, CDialog)

XMJGSetInfo7::XMJGSetInfo7(bool isCreate, CWnd* pParent /*=NULL*/)
: CDialog(XMJGSetInfo7::IDD, pParent), m_isCreate(isCreate)
{

}

XMJGSetInfo7::~XMJGSetInfo7()
{

}

void XMJGSetInfo7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_projectdir);
	DDX_Control(pDX, IDC_ACCEPTCODE7, m_AcceptCode);
	DDX_Control(pDX, IDC_PROJECTNAME7, m_ProjectName);
	DDX_Control(pDX, IDC_BUILDCOMPANY7, m_BuildCompany);
	DDX_Control(pDX, IDC_DATETIMEAPPLY, m_datatimeapply);
	DDX_Control(pDX, IDC_SHISHICOMPANY7, m_ShiShiCompany);
	DDX_Control(pDX, IDC_PROJECTADDRESS7, m_ProjectAddress);
	DDX_Control(pDX, IDC_MEANSURECOMPANY7, m_MeansureCompany);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EXPORT, m_exportTime);
	DDX_Control(pDX, IDC_HOUSEProjectCode, m_hprojectcode);
}

BEGIN_MESSAGE_MAP(XMJGSetInfo7, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetInfo7::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()

#ifdef _WIN64
LRESULT XMJGSetInfo7::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}
#else
LONG XMJGSetInfo7::onAcadKeepFocus(UINT n, LONG l)
{
	return TRUE;
}
#endif

BOOL XMJGSetInfo7::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_AcceptCode.SetReadOnly(!m_isCreate);
	IDataBaseOper oper; VStr cldws;
	CString cldw = oper.readJGCommon(_T("测量单位"));
	AnalyzeData(cldws, cldw, _T(";"));
	for(int idx = 0; idx < (int)cldws.size(); ++idx)
		m_MeansureCompany.InsertString(idx, cldws[idx]);
	if(m_isCreate == false)
	{
		IProjectMDB pdb; MStr context = pdb.getProjectInfo();
		m_AcceptCode.SetWindowText(context[_T("受理号")]);
		m_ProjectName.SetWindowText(context[_T("项目名称")]);
		m_BuildCompany.SetWindowText(context[_T("建设单位")]);
		m_hprojectcode.SetWindowText(context[_T("项目代码")]);
		m_ProjectAddress.SetWindowText(context[_T("项目地点")]);
		m_MeansureCompany.SetWindowText(context[_T("测量单位")]);
		m_ShiShiCompany.SetWindowText(context[_T("组织单位")]);
		m_datatimeapply.SetWindowText(context[_T("审批时间")]);
	}
	else
		m_ShiShiCompany.SetWindowText(_T("厦门市测绘与基础地理信息中心"));
	IDataBaseOper db;
	m_projectdir.SetWindowText(db.readPathTable(_T("工程目录")) + _T("房屋竣工\\"));
	return TRUE;
}

void XMJGSetInfo7::OnBnClickedOk()
{
	MStr context; CString info;
	m_AcceptCode.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("项目编号不能为空")); return;
	}
	context[_T("受理号")] = info;
	m_BuildCompany.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("建设单位不能为空")); return;
	}
	context[_T("建设单位")] = info;
	m_ProjectName.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("项目名称不能为空")); return;
	}
	context[_T("项目名称")] = info;
	m_hprojectcode.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("项目代码不能为空"));  return;
	}
	context[_T("项目代码")] = info;
	m_ProjectAddress.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("项目地点不能为空")); return;
	}
	context[_T("项目地点")] = info;
	m_MeansureCompany.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("测量单位不能为空")); return;
	}
	context[_T("测量单位")] = info;
	m_ShiShiCompany.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("实施单位不能为空")); return;
	}
	context[_T("组织单位")] = info;
	m_datatimeapply.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("审批时间不能为空")); return;
	}
	context[_T("审批时间")] = info;
	m_exportTime.GetWindowText(info);
	if(info.GetLength() == 0)
	{
		AfxMessageBox(_T("审批时间不能为空")); return;
	}
	context[_T("报告时间")] = info;
	if(m_isCreate == true)
	{
		IProjectInfo pi(_T("房屋竣工"));
		if(false == pi.CreateProjectEx(context, CString(_T("规划")))) return;
	}
	else
	{
		IProjectMDB pdb;
		pdb.setJBXXTableInfo(context);
	}
	info.Format(_T("项目编号：%s，项目名称：%s"), context[_T("受理号")], context[_T("项目名称")]);
	acedGrText(-1, info, 0);
	CDialog::OnOK();
}

#pragma endregion

#pragma region 业务一成果输出对话框

#pragma endregion

#pragma region 业务二成果输出对话框

XMJG2ExportDlg *XMJG2ExportDlg::_setinfo = NULL;

IMPLEMENT_DYNAMIC(XMJG2ExportDlg, CAcUiDialog)

XMJG2ExportDlg::XMJG2ExportDlg(CWnd* pParent /*=NULL*/)
: CAcUiDialog(XMJG2ExportDlg::IDD, pParent)
{
	RegPath.Format(_T("Software\\SouthSurvey\\XMJG\\Project2"));
}

XMJG2ExportDlg::XMJG2ExportDlg(const CString &name)
	: CAcUiDialog(XMJG2ExportDlg::IDD, NULL)
	, m_Name(name)
{

}

XMJG2ExportDlg::~XMJG2ExportDlg()
{

}

void XMJG2ExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XMJG2_PROJECTNAME, m_ProjectName);
	DDX_Control(pDX, IDC_XMJG2_BUILDCOMPANY, m_BuildCompany);
	DDX_Control(pDX, IDC_XMJG2_PROJECTAREA, m_ProjectAdd);
	DDX_Control(pDX, IDC_XMJG2_PROJECTCODE, m_ProjectCode);
	DDX_Control(pDX, IDC_XMJG2_EDGEDISTANCE, m_EdgeDistance);
	DDX_Control(pDX, IDC_XMJG2_BUILDDISTANCE, m_BuildDistance);
	DDX_Control(pDX, IDC_XMJG2_SCEDGEDISTANCE, m_SCEdgeDistance);
	DDX_Control(pDX, IDC_XMJG2_SCBUILDDISTANCE, m_SCBuildDistance);
	DDX_Control(pDX, IDC_BUILDWALL2, m_BuildWall);
	DDX_Control(pDX, IDC_BUILDAIXS2, m_BuildAixs);
}

BEGIN_MESSAGE_MAP(XMJG2ExportDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &XMJG2ExportDlg::OnBnClickedOk)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_DISTANCE, &XMJG2ExportDlg::OnBnClickedDistance)
	ON_BN_CLICKED(IDC_BETWEEN, &XMJG2ExportDlg::OnBnClickedBetween)
END_MESSAGE_MAP()

#ifdef _WIN64
LRESULT XMJG2ExportDlg::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}
#else
LONG XMJG2ExportDlg::onAcadKeepFocus(UINT n, LONG l)
{
	return TRUE;
}
#endif

void XMJG2ExportDlg::getWindowControl(MStr &context)
{
	CString value;
	m_ProjectName.GetWindowText(value); context[_T("项目名称")] = value;
	m_BuildCompany.GetWindowText(value); context[_T("建设单位")] = value;
	m_ProjectAdd.GetWindowText(value); context[_T("项目地点")] = value;
	m_ProjectCode.GetWindowText(value); context[_T("许可证号")] = value;
	m_EdgeDistance.GetWindowText(value); context[_T("规划退红线间距")] = value;
	m_BuildDistance.GetWindowText(value); context[_T("规划建筑间距")] = value;
	m_SCEdgeDistance.GetWindowText(value); context[_T("实测退红线间距")] = value;
	m_SCBuildDistance.GetWindowText(value); context[_T("实测建筑间距")] = value;
	int check = m_BuildWall.GetCheck(); value.Format(_T("%d"), check); context[_T("建筑外墙")] = value;
	check = m_BuildAixs.GetCheck(); value.Format(_T("%d"), check); context[_T("建筑轴线")] = value;
}

void XMJG2ExportDlg::setWindowControl(MStr &context)
{
	XMJGAssist assist; CString binfo, rinfo;
	assist.GetBuildDistanceText(binfo);
	assist.GetEdgeDistanceText(rinfo);
	if(rinfo.GetLength() != 0)
		context[_T("实测退红线间距")] = rinfo;
	if(binfo.GetLength() != 0)
		context[_T("实测建筑间距")] = binfo;
	m_ProjectName.SetWindowText(context[_T("项目名称")]);
	m_BuildCompany.SetWindowText(context[_T("建设单位")]);
	m_ProjectAdd.SetWindowText(context[_T("项目地点")]);
	m_ProjectCode.SetWindowText(context[_T("许可证号")]);
	m_EdgeDistance.SetWindowText(context[_T("规划退红线间距")]);
	m_BuildDistance.SetWindowText(context[_T("规划建筑间距")]);
	m_SCEdgeDistance.SetWindowText(context[_T("实测退红线间距")]);
	m_SCBuildDistance.SetWindowText(context[_T("实测建筑间距")]);
	int check = _ttoi(context[_T("建筑外墙")]); m_BuildWall.SetCheck(check);
	check = _ttoi(context[_T("建筑轴线")]); m_BuildAixs.SetCheck(check);
}

XMJG2ExportDlg *XMJG2ExportDlg::Instance(const CString &name)
{
	if(_setinfo == NULL)
	{
		_setinfo = new XMJG2ExportDlg(name);
		_setinfo->Create(XMJG2ExportDlg::IDD);
	}
	return _setinfo;
}

void XMJG2ExportDlg::destory()
{
	if(_setinfo == NULL) return;
	delete _setinfo;
	_setinfo = NULL;
}

void XMJG2ExportDlg::SetRegPath(bool flag)
{
	RegPath.Format(_T("Software\\SouthSurvey\\XMJG\\Project%d"), flag == true ? 2 : 5);
}

BOOL XMJG2ExportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_docname[_T("正负零线")] = _T("正负零线附表");
	m_docname[_T("建筑放样")] = _T("建筑放样附表");
	// TODO:  在此添加额外的初始化
	IProjectMDB pdb;
	MStr jbxx = pdb.getJBXXTableInfo();
	MStr bgxx = pdb.getBGXXTableInfo();
	jbxx[_T("规划退红线间距")] = bgxx[_T("规划退红线间距")];
	jbxx[_T("规划建筑间距")] = bgxx[_T("规划建筑间距")];
	jbxx[_T("实测退红线间距")] = bgxx[_T("实测退红线间距")];
	jbxx[_T("实测建筑间距")] = bgxx[_T("实测建筑间距")];
	setWindowControl(jbxx);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

#include "smarttable_lua.h"

void XMJG2ExportDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	MStr filter, value;
	int nflag = 0;
	filter[_T("业务名称")] = m_Name;
	getWindowControl(value);
	IProjectMDB pdb;
	value[_T("用地面积")] = pdb.getProjectInfo(_T("用地面积"));
	if(!_tcscmp(m_Name, _T("正负零线")))
		nflag = 1;
	exportWord(value, nflag);
	pdb.setJBXXTableInfo(value);
	pdb.setBGXXTableInfo(value);
	CDialog::OnOK();
}

void XMJG2ExportDlg::exportWord(MStr &value, int nflag)
{
	XMJGAssist assist;
	CString path1 = assist.getExportFilePath(true);
	CString name = assist.getExportFileName(m_docname[m_Name]);
	path1 += _T("\\") + name;
	char savepath[1024] = {0};
	TcharToChar(path1, savepath);
	SmartTable::Word word; TCHAR path[255] = {0};
	CString strPath = _T("");
	if(nflag == 0)
		strPath = _T("厦门市建设工程规划放样验线申请表附表.docx");
	else
		strPath = _T("厦门市建设工程0-0验线（建筑类)申请表附表.docx");
	if(RTNORM != ads_findfile(strPath, path))
	{
		acutPrintf(_T("\n 无法找到模板文件 %s"), strPath);
		return;
	}
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath);
	readDataBase(&word, value);
	word.save(savepath);
	acutPrintf(_T("\n 报告输出完成。路径为%s"), path1);
}

void XMJG2ExportDlg::readDataBase(SmartTable::Word *word, MStr &field)
{
	char val[1024] = {0}; CString value;
	value = field[_T("项目名称")]; value.Replace(_T("\r\n"), _T(" "));
	TcharToChar(value, val); word->replaceText("#ProjectName#", val);
	value = field[_T("建设单位")]; value.Replace(_T("\r\n"), _T(" "));
	TcharToChar(value, val); word->replaceText("#BuildCompany#", val);
	value = field[_T("项目地点")]; value.Replace(_T("\r\n"), _T(" "));
	TcharToChar(value, val); word->replaceText("#ProjectAddress#", val);
	value = field[_T("许可证号")]; value.Replace(_T("\r\n"), _T(" "));
	TcharToChar(value, val); word->replaceText("#LicenseCode#", val);
	TcharToChar(field[_T("用地面积")], val); word->replaceText("#BuildArea#", val);
	TcharToChar(field[_T("规划退红线间距")], val); word->replaceText("#EdgeDistance1#", val);
	TcharToChar(field[_T("规划建筑间距")], val); word->replaceText("#BuildDistance1#", val);
	TcharToChar(field[_T("实测退红线间距")], val); word->replaceText("#EdgeDistance2#", val);
	TcharToChar(field[_T("实测建筑间距")], val); word->replaceText("#BuildDistance2#", val);
	int var = _ttoi(field[_T("建筑外墙")]); if(var == 1) word->replaceText("#1", "√"); else word->replaceText("#1", "  ");
	var = _ttoi(field[_T("建筑轴线")]); if(var == 1) word->replaceText("#2", "√"); else word->replaceText("#2", "  ");
}

#pragma endregion

#pragma region 业务三成果输出对话框

#pragma endregion

#pragma region 业务四成果输出对话框

#pragma region 绘制高程点

IMPLEMENT_DYNAMIC(XMJGControlPointDlg, CDialog)

XMJGControlPointDlg::XMJGControlPointDlg(CWnd* pParent /*=NULL*/)
: CDialog(XMJGControlPointDlg::IDD, pParent)
{

}

XMJGControlPointDlg::~XMJGControlPointDlg()
{

}

void XMJGControlPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTROLPOINTTYPE, m_ControlPoinType);
	DDX_Control(pDX, IDC_CONTROLPOINTRANK, m_ControlPointRank);
}

BEGIN_MESSAGE_MAP(XMJGControlPointDlg, CDialog)

	ON_BN_CLICKED(IDOK, &XMJGControlPointDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL XMJGControlPointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	MStr filter; VStr value;
	filter[_T("关键字")] = _T("控制点类型");
	IDataBaseOper db;
	db.readDataBase(COMMONTABLE, _T("属性值"), filter, value);
	for(int idx = 0; idx < (int)value.size(); ++idx)
		m_ControlPoinType.InsertString(idx, value.at(idx));
	filter[_T("关键字")] = _T("控制点等级"); value.clear();
	db.readDataBase(COMMONTABLE, _T("属性值"), filter, value);
	for(int idx = 0; idx < (int)value.size(); ++idx)
		m_ControlPointRank.InsertString(idx, value.at(idx));
	m_ControlPoinType.SetCurSel(0);
	m_ControlPointRank.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void XMJGControlPointDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ControlPointRank.GetWindowText(m_Rank);
	m_ControlPoinType.GetWindowText(m_Type);
	CDialog::OnOK();
}

CString XMJGControlPointDlg::getControlPointRank() const
{
	return m_Rank;
}

CString XMJGControlPointDlg::getControlPointType() const
{
	return m_Type;
}

#pragma endregion 绘制高程点

#pragma region 设置水准高程信息

#pragma endregion 设置水准高程信息


#pragma endregion

#pragma region 业务五成果输出对话框

#pragma endregion

IMPLEMENT_DYNAMIC(XMJGPlanDlg, CDialog)

XMJGPlanDlg::XMJGPlanDlg(CWnd* pParent /*=NULL*/)
: CDialog(XMJGPlanDlg::IDD, pParent)
, m_ObjectID(AcDbObjectId::kNull)
{

}

XMJGPlanDlg::~XMJGPlanDlg()
{
}

void XMJGPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FLOORNUM1, m_FloorNum1);
	DDX_Control(pDX, IDC_FLOORNUM2, m_FloorNum2);
	DDX_Control(pDX, IDC_LIST_LAYERINFO, m_layerinfo);
}

BEGIN_MESSAGE_MAP(XMJGPlanDlg, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGPlanDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PLANADD, &XMJGPlanDlg::OnBnClickedPlanAdd)
	ON_BN_CLICKED(IDC_PLANDEL, &XMJGPlanDlg::OnBnClickedPlanDel)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, &XMJGPlanDlg::OnBnClickedButtonMoveup)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, &XMJGPlanDlg::OnBnClickedButtonMovedown)
END_MESSAGE_MAP()

void XMJGPlanDlg::setObjectID(const AcDbObjectId &Id)
{
	m_ObjectID = Id;
}

void XMJGPlanDlg::getFloorNum(CString &num)
{
	CString num1, num2;
	m_FloorNum1.GetWindowText(num1);
	m_FloorNum2.GetWindowText(num2);
	if(num1.GetLength() != 0 && num2.GetLength() != 0)
	{
		if(_tstoi(num1) > _tstoi(num2)) swap(num1, num2);
		num.Format(_T("%s ---> %s"), num1, num2); return;
	}
	else if(num1.GetLength() == 0 && num2.GetLength() == 0)
		return;
	num1 = num1.GetLength() == 0 ? num2 : num1;
	if(stringIsDigit(num1) == true)
		num.Format(_T("%s ---> %s"), num1, num1);
	else
		num = num1;
}

void XMJGPlanDlg::addFloorNum(const CString &info)
{
	int count = m_layerinfo.GetItemCount();
	CString num;
	num.Format(_T("%d"), count);
	m_layerinfo.InsertItem(count, num);
	m_layerinfo.SetItemText(count, 1, info);
	m_layerinfo.setSelectItem(count);
}

CString XMJGPlanDlg::getFloorNumByList(int idx) const
{
	CString order = m_layerinfo.GetItemText(idx, 0);
	CString info = m_layerinfo.GetItemText(idx, 1);
	order += _T("_") + info; return order;
	int pos = info.Find(_T("--->"));
	if(pos == -1 && info.Left(1).CompareNoCase(_T("-")) != 0 && stringIsDigit(info) == false)
		return g_fieldchagetable.getIndexByName(info);
	if(pos != -1 && _tstoi(info.Left(pos)) == _tstoi(info.Right(pos)))
		info = info.Left(pos);
	return info;
}

void XMJGPlanDlg::OnBnClickedPlanAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	CString floor;
	getFloorNum(floor);
	addFloorNum(floor);
	m_FloorNum1.SetWindowText(_T(""));
	m_FloorNum2.SetWindowText(_T(""));
}

void XMJGPlanDlg::OnBnClickedPlanDel()
{
	int idx = m_layerinfo.GetSelectedItem();
	m_layerinfo.DeleteItem(idx);
	int count = m_layerinfo.GetItemCount();
	for(; idx < count; ++idx)
	{
		CString order; order.Format(_T("%d"), idx);
		m_layerinfo.SetItemText(idx, 0, order);
	}
}

void XMJGPlanDlg::OnBnClickedOk()
{
	int len = m_layerinfo.GetItemCount(); CString info;
	RemoveXdata(aname(m_ObjectID), _T("相同楼层编号"));
	for(int idx = 0; idx < len; ++idx)
	{
		info = getFloorNumByList(idx);
		m_LayerInfo.push_back(info);
		ADDMulXData(m_ObjectID, _T("相同楼层编号"), info, 0, idx);
	}
	CDialog::OnOK();
}

VStr XMJGPlanDlg::getLayerInfo()
{
	return m_LayerInfo;
}

BOOL XMJGPlanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_layerinfo.Init();	CRect rect;
	m_layerinfo.GetClientRect(&rect);
	int width = rect.right - rect.left;
	m_layerinfo.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 60);
	m_layerinfo.InsertColumn(1, _T("名称"), LVCFMT_CENTER, width - 60);
	int len = GetXDataCount(aname(m_ObjectID), _T("相同楼层编号"));
	for(int idx = 0; idx < len; ++idx)
	{
		TCHAR info[255] = {0}, num[255] = {0};
		READMulXData(m_ObjectID, _T("相同楼层编号"), num, 0, idx);
		CString layername(num);
		int pos = layername.Find(_T("_"));
		m_layerinfo.InsertItem(idx, layername.Mid(0, pos));
		m_layerinfo.SetItemText(idx, 1, layername.Mid(pos + 1)); continue;
		if(stringIsDigit(num) == true)
		{
			addFloorNum(g_fieldchagetable.getNameByIndex(num));
			continue;
		}
		if(_tcslen(num) < 6)
			_stprintf(info, _T("%s ---> %s"), num, num);
		else
			_stprintf(info, _T("%s"), num);
		addFloorNum(info);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGPlanDlg::OnBnClickedButtonMoveup()
{
	// TODO:  在此添加控件通知处理程序代码
	int row = m_layerinfo.GetSelectedItem();
	if(row == 0) return;
	CString uptext = m_layerinfo.GetItemText(row - 1, 1);
	CString cutext = m_layerinfo.GetItemText(row, 1);
	m_layerinfo.SetItemText(row, 1, uptext);
	m_layerinfo.SetItemText(row - 1, 1, cutext);
	m_layerinfo.setSelectItem(row - 1);
}

void XMJGPlanDlg::OnBnClickedButtonMovedown()
{
	// TODO:  在此添加控件通知处理程序代码
	int row = m_layerinfo.GetSelectedItem();
	int count = m_layerinfo.GetItemCount();
	if(row == count - 1) return;
	CString dwtext = m_layerinfo.GetItemText(row + 1, 1);
	CString cutext = m_layerinfo.GetItemText(row, 1);
	m_layerinfo.SetItemText(row, 1, dwtext);
	m_layerinfo.SetItemText(row + 1, 1, cutext);
	m_layerinfo.setSelectItem(row + 1);
}

// XMJGSetStratPlanInfo 对话框

IMPLEMENT_DYNAMIC(XMJGSetStratPlanInfo, CDialog)

XMJGSetStratPlanInfo::XMJGSetStratPlanInfo(CWnd* pParent /*=NULL*/)
: CDialog(XMJGSetStratPlanInfo::IDD, pParent)
{

}

XMJGSetStratPlanInfo::~XMJGSetStratPlanInfo()
{
}

void XMJGSetStratPlanInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLANCODE, m_PlanCode);
	DDX_Control(pDX, IDC_FLOORCODE, m_FloorCode);
	DDX_Control(pDX, IDC_FLOORHEIGHT, m_FloorHeight);
	DDX_Control(pDX, IDC_PARAPETWALL, m_ParapetWall);
}

BEGIN_MESSAGE_MAP(XMJGSetStratPlanInfo, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGSetStratPlanInfo::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL XMJGSetStratPlanInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_ObjectID == AcDbObjectId::kNull)
	{
		AfxMessageBox(_T("请选择要设置的楼层"));
		return FALSE;
	}
	setControlInfo(&m_PlanCode, _T("楼栋号"));
	setControlInfo(&m_FloorCode, _T("楼层号"));
	setControlInfo(&m_FloorHeight, _T("楼层高"));
	TCHAR *info = new TCHAR[20];
	ReadXdata(aname(m_ObjectID), _T("女儿墙"), 0, info);
	int val = _tstoi(info);	m_ParapetWall.SetCheck(val);
	delete info; info = NULL;
	return TRUE;
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGSetStratPlanInfo::setObjectID(const AcDbObjectId &id)
{
	m_ObjectID = id;
}

void XMJGSetStratPlanInfo::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	this->getControlInfo(&m_FloorHeight, _T("楼层高"));
	CString layerheight;
	m_FloorHeight.GetWindowText(layerheight);
	setLayerInfo(layerheight);
	int value = m_ParapetWall.GetCheck(); TCHAR val[20] = {0};
	_stprintf(val, _T("%d"), value);
	AddXdata(aname(m_ObjectID), _T("女儿墙"), 0, val);
	CDialog::OnOK();
}

void XMJGSetStratPlanInfo::setLayerInfo(const CString &info)
{
	TCHAR layer[255] = {0}; int lower = -1, upper = -1;
	ReadXdata(aname(m_ObjectID), _T("楼层号"), 0, layer);
	getLayerNum(layer, lower, upper); CString num;
	if(lower == 0 && upper == 0)
	{
		AddXdata(aname(m_ObjectID), _T("层号"), 0, layer);
		AddXdata(aname(m_ObjectID), _T("层高"), 0, info);
		return;
	}
	for(int idx = lower; idx <= upper; ++idx)
	{
		num.Format(_T("%d"), idx);
		ADDMulXData(m_ObjectID, _T("层号"), num, 0, idx - lower);
		ADDMulXData(m_ObjectID, _T("层高"), info, 0, idx - lower);
	}
}

void XMJGSetStratPlanInfo::getControlInfo(CWnd *control, const TCHAR *AppName)
{
	TCHAR *info = new TCHAR[255];
	control->GetWindowText(info, 255);
	AddXdata(aname(m_ObjectID), AppName, 0, info);
	if(info != NULL) delete info; info = NULL;
}

void XMJGSetStratPlanInfo::setControlInfo(CWnd *control, const TCHAR *AppName)
{
	TCHAR *info = new TCHAR[255];
	ReadXdata(aname(m_ObjectID), AppName, 0, info);
	control->SetWindowText(info);
	if(info != NULL) delete info;
	info = NULL;
}

// XMJGFunctionsAreasInfo 对话框

IMPLEMENT_DYNAMIC(XMJGFunctionsAreasInfo, CDialog)

XMJGFunctionsAreasInfo::XMJGFunctionsAreasInfo(CWnd* pParent /*=NULL*/)
: CDialog(XMJGFunctionsAreasInfo::IDD, pParent), m_len(0)
{
	acutPrintf(_T("\n 设置功能信息"));
	m_island = 0;
}

XMJGFunctionsAreasInfo::~XMJGFunctionsAreasInfo()
{
}

void XMJGFunctionsAreasInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOXMJG_FUNCTIONNAME, m_functionnames);
	DDX_Control(pDX, IDC_FUNCTIONSCODE, m_FunctionsCode);
	DDX_Control(pDX, IDC_FUNCTIONSNAMES, m_functionnames);
	DDX_Control(pDX, IDC_FUNCTIONSHEIGHT, m_FunctionsHeight);
	DDX_Control(pDX, IDC_AREACOEFFICIENT, m_AreaCoefficient);
	DDX_Control(pDX, IDC_FUNCTIONSFACTOR, m_FunctionsFactor);
	DDX_Control(pDX, IDC_EDIT2_TINAME, m_tiname);
	DDX_Check(pDX, IDC_CHECK_ISLAND, m_island);
}

BEGIN_MESSAGE_MAP(XMJGFunctionsAreasInfo, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGFunctionsAreasInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &XMJGFunctionsAreasInfo::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_XMJGADD, &XMJGFunctionsAreasInfo::OnClickedXmjgadd)
	ON_BN_CLICKED(IDC_BUTTON_XMJGDEL, &XMJGFunctionsAreasInfo::OnClickedXmjgdel)
	ON_CBN_SELCHANGE(IDC_COMBOXMJG_FUNCTIONNAME, &XMJGFunctionsAreasInfo::OnFunctionChangeName)
END_MESSAGE_MAP()

BOOL XMJGFunctionsAreasInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_fnt.m_init = true;
	// TODO:  在此添加额外的初始化
	IDataBaseOper oper;
	oper.readFunctionTable(m_fnts);
	m_tiname.AddString(_T("A梯"));
	m_tiname.AddString(_T("B梯"));
	m_tiname.AddString(_T("C梯"));
	m_tiname.AddString(_T("D梯"));
	m_tiname.AddString(_T("E梯"));
	m_tiname.AddString(_T("F梯"));
	int size = (int)m_fnts.size();
	for(int idx = 0; idx < size; ++idx)
		m_functionnames.AddString(m_fnts.at(idx).name);
	TCHAR info[100] = {0};
	if(m_height.GetLength() != 0)
		m_FunctionsHeight.SetWindowText(m_height);
	if(false == ReadXdata(aname(m_ObjectID), _T("功能区名称"), 0, info)) return TRUE;
	m_functionnames.SelectString(0, info); int pos = 0;
	for(; pos < (int)m_fnts.size(); ++pos)
	{
		if(m_fnts.at(pos).name.CompareNoCase(info) == 0)
			break;
	}
	if(pos == m_fnts.size()) return TRUE;
	ReadXdata(aname(m_ObjectID), _T("梯间名称"), 0, info);
	m_tiname.SelectString(0, info); CString jmbh;
	ReadXdata(aname(m_ObjectID), _T("功能区简称"), 0, info); jmbh += info;
	ReadXdata(aname(m_ObjectID), _T("功能区编号"), 0, info); jmbh += info;
	m_FunctionsCode.SetWindowTextW(jmbh);
	ReadXdata(aname(m_ObjectID), _T("功能区高度"), 0, info);
	m_FunctionsHeight.SetWindowTextW(info);
	ReadXdata(aname(m_ObjectID), _T("面积系数"), 0, info);
	if(0 == _tcslen(info)) _stprintf(info, _T("%.1f"), 1.0);
	m_AreaCoefficient.SetWindowTextW(info);
	ReadXdata(aname(m_ObjectID), _T("计容系数"), 0, info);
	if(0 == _tcslen(info)) _stprintf(info, _T("%.1f"), 1.0);
	m_FunctionsFactor.SetWindowText(info);
	ReadXdata(aname(m_ObjectID), _T("扣岛"), 0, info);
	if (0 == _tcsicmp(info, _T("true")))
	{
		m_island = 1;
	}
	UpdateData(FALSE);
	return TRUE;
}

void XMJGFunctionsAreasInfo::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_tiname.GetWindowText(m_fnt.tm);
	m_functionnames.GetWindowText(m_fnt.name); double scale = 1.0;
	if(m_fnt.name.GetLength() == 0)
	{
		MessageBox(_T("功能区名称为空！！！"), _T("错误信息"));
		return;
	}
	CString code;
	m_FunctionsCode.GetWindowText(code);
	if(code.GetLength() == 0)
	{
		MessageBox(_T("功能区编号为空！！！"), _T("错误信息"));
		return;
	}
	// 由于负号比较特设，所以特殊处理一下
	code.Replace(_T("-"), _T("@"));
	getStrAndNumByString(code, m_fnt.jm, m_fnt.code);
	code.Replace(_T("@"), _T("-"));
	m_fnt.jm.Replace(_T("@"), _T("-"));
	m_fnt.len = code.GetLength() - m_fnt.jm.GetLength();
	m_FunctionsHeight.GetWindowText(m_fnt.gaodu);
	m_fnt.m_isMainHeight = (m_height.CompareNoCase(m_fnt.gaodu) == 0);
	m_AreaCoefficient.GetWindowText(m_fnt.mjxs);
	if(m_fnt.mjxs.GetLength() == 0)
	{
		MessageBox(_T("面积系数为空！！！"), _T("错误信息")); return;
	}

	m_FunctionsFactor.GetWindowText(m_fnt.jrxs);
	if(m_fnt.jrxs.GetLength() == 0)
	{
		MessageBox(_T("计容系数为空！！！"), _T("错误信息")); return;
	}
	m_fnt.m_init = true;
	m_fnt.m_island = m_island;
	IDataBaseOper oper;
	oper.saveFunctionTable(m_fnts);
	CDialog::OnOK();
}

FunctionNameTableEx XMJGFunctionsAreasInfo::getFNTInfo()
{
	if(m_fnt.jm.IsEmpty())
	{
		if(m_fnt.code < 10)
			m_fnt.cn.Format(_T("0%d"), m_fnt.code);
		else
			m_fnt.cn.Format(_T("%d"), m_fnt.code);
	}
	else
	{
		if (m_fnt.code == 0)
		{
			m_fnt.cn = _T("");
		}
		else
		{
			if(m_fnt.len == 1)
				m_fnt.cn.Format(_T("%d"), m_fnt.code);
			else
				m_fnt.cn.Format(_T("0%d"), m_fnt.code);
		}
	}

	return m_fnt;
}

void XMJGFunctionsAreasInfo::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void XMJGFunctionsAreasInfo::OnFunctionChangeName()
{
	FunctionNameTable curfnt;
	if(false == getWindowText(curfnt))
	{
		m_functionnames.SetWindowText(curfnt.name);
		return;
	}
	if(curfnt.name.GetLength() != 0)
	{
		for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
		{
			if(curfnt.name.CompareNoCase(m_fnts.at(idx).name) != 0) continue;
			m_fnts.at(idx).jrxs = curfnt.jrxs;
			m_fnts.at(idx).mjxs = curfnt.mjxs;
		}
	}
	CString name;
	int nIndex = m_functionnames.GetCurSel(); //当前选中的项
	m_functionnames.SetCurSel(nIndex);
	m_functionnames.GetWindowText(name);
	for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
	{
		FunctionNameTable fnt = m_fnts.at(idx);
		if(fnt.name.CompareNoCase(name) != 0) continue;
		m_AreaCoefficient.SetWindowText(fnt.mjxs);
		m_FunctionsFactor.SetWindowTextW(fnt.jrxs);
	}
}

void XMJGFunctionsAreasInfo::setEntInfo(const AcDbObjectId &id, const CString &info)
{
	m_ObjectID = id; m_height = info;
}

bool XMJGFunctionsAreasInfo::getWindowText(FunctionNameTable &fnt)
{
	CString name, mjxs, jrxs;
	m_functionnames.GetWindowText(name);
	m_AreaCoefficient.GetWindowText(mjxs);
	m_FunctionsFactor.GetWindowText(jrxs);
	if(name.GetLength() == 0) return true;
	if(mjxs.GetLength() == 0 || jrxs.GetLength() == 0)
	{
		MessageBox(_T("输出参数有问题，请进行检查"), _T("错误信息"));
		return false;
	}
	fnt.name = name; fnt.mjxs = mjxs; fnt.jrxs = jrxs;
	return true;
}

void XMJGFunctionsAreasInfo::OnClickedXmjgadd()
{
	// TODO:  在此添加控件通知处理程序代码
	FunctionNameTable fnt;
	if(false == getWindowText(fnt)) return;
	for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
	{
		FunctionNameTable tmp = m_fnts.at(idx);
		if(tmp.name.CompareNoCase(fnt.name) != 0) continue;
		CString name; name.Format(_T("功能区%s名称重复，请检查之后再进行添加！！！"), fnt.name);
		MessageBox(name, _T("错误信息"));	return;
	}
	m_functionnames.AddString(fnt.name);
	m_fnts.push_back(fnt);
}

void XMJGFunctionsAreasInfo::OnClickedXmjgdel()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_functionnames.GetCurSel();
	if(sel == -1) return;
	if(sel >= (int)m_fnts.size()) return;
	m_functionnames.DeleteString(sel);
	FNTIter it = m_fnts.begin();
	it = it + sel;
	m_fnts.erase(it);
}

// XMJGRedLineQuality 对话框

IMPLEMENT_DYNAMIC(XMJGRedLineQuality, CDialog)

XMJGRedLineQuality::XMJGRedLineQuality(CWnd* pParent /*=NULL*/)
: CDialog(XMJGRedLineQuality::IDD, pParent)
{

}

XMJGRedLineQuality::~XMJGRedLineQuality()
{
}

void XMJGRedLineQuality::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REDLINECOORANN, m_RedLineCoorAnn);
	DDX_Control(pDX, IDC_REDLINESEGMENTANN, m_RedLineSegmentAnn);
	DDX_Control(pDX, IDC_REDLINELENGHTANN, m_RedLineLenghtAnn);
	DDX_Control(pDX, IDC_NOCROSSREDLINENODEANN, m_NoRedLineCrossNodeAnn);
}

BEGIN_MESSAGE_MAP(XMJGRedLineQuality, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGRedLineQuality::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &XMJGRedLineQuality::OnBnClickedCancel)
END_MESSAGE_MAP()

// XMJGRedLineQuality 消息处理程序

void XMJGRedLineQuality::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_CheckCoorAnn = m_RedLineCoorAnn.GetCheck() == 1;
	m_CheckLenghtAnn = m_RedLineLenghtAnn.GetCheck() == 1;
	m_CheckSegmentAnn = m_RedLineSegmentAnn.GetCheck() == 1;
	m_CheckCrossNodeAnn = m_NoRedLineCrossNodeAnn.GetCheck() == 1;
	CDialog::OnOK();
}

void XMJGRedLineQuality::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

BOOL XMJGRedLineQuality::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_RedLineCoorAnn.SetCheck(1);
	m_RedLineLenghtAnn.SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

bool XMJGRedLineQuality::getCheckCoorAnn()
{
	return m_CheckCoorAnn;
}

bool XMJGRedLineQuality::getCheckLenghtAnn()
{
	return m_CheckLenghtAnn;
}

bool XMJGRedLineQuality::getCheckSegmentAnn()
{
	return m_CheckSegmentAnn;
}

bool XMJGRedLineQuality::getCheckRedLineNodeAnn()
{
	return m_CheckCrossNodeAnn;
}

//获取里程字符串
void GetLichengText(double dLicheng, TCHAR *szStr)
{
	int kilo = (int)(dLicheng / 1000.0);
	double Decimal = dLicheng - kilo * 1000.0;
	_stprintf(szStr, _T("K%d+%.2f"), kilo, Decimal);
}

CDrawSection::CDrawSection(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(CDrawSection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawSection)
	m_biaochi = 0.0;
	m_ifbch = FALSE;
	m_iffg = TRUE;
	m_jgnum = 5;
	m_jgx = 300.0;
	m_jgy = 200.0;
	m_scalex = 500;
	m_scaley = 100;
	m_fgx = 10;
	m_fgy = 10;
	m_Licheng = 0;
	m_height = 1;
	m_dBeginLC = 0.0;
	m_PlaceX = 0.0;
	m_PlaceY = 0.0;
	m_nHpmt = 0;
	m_dWidth = 40.0;
	m_dLchHigh = 3.0;
	m_dLchMin = 3.0;
	m_nLchWei = 1;
	m_dLicheng = -100.0;
	m_bAskHpmt = FALSE;
	m_bAskBegin = FALSE;
	m_bAskJiange = FALSE;
	m_oldPick = PickstyleReset(0);
	ads_command(RTSTR, _T("linetype"), RTSTR, _T("s"), RTSTR, _T("continuous"), RTSTR, _T(""), RTNONE);
}

CDrawSection::~CDrawSection()
{
	PickstyleBack(m_oldPick);
	ads_command(RTSTR, _T("layer"), RTSTR, _T("s"), RTSTR, _T("0"), RTSTR, _T(""), RTNONE);
}

void CDrawSection::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ZONG_BIAOCHI, m_biaochi);
	DDX_Check(pDX, IDC_ZONG_IFBCH, m_ifbch);
	DDX_Check(pDX, IDC_ZONG_IFFG, m_iffg);
	DDX_Text(pDX, IDC_ZONG_JGNUM, m_jgnum);
	DDV_MinMaxInt(pDX, m_jgnum, 1, 10000);
	DDX_Text(pDX, IDC_ZONG_JGX, m_jgx);
	DDV_MinMaxDouble(pDX, m_jgx, 0., 100000.);
	DDX_Text(pDX, IDC_ZONG_JGY, m_jgy);
	DDV_MinMaxDouble(pDX, m_jgy, 0., 100000.);
	DDX_Text(pDX, IDC_ZONG_SCALEX, m_scalex);
	DDV_MinMaxInt(pDX, m_scalex, 1, 100000);
	DDX_Text(pDX, IDC_ZONG_SCALEY, m_scaley);
	DDV_MinMaxInt(pDX, m_scaley, 1, 100000);
	DDX_Text(pDX, IDC_ZONG_FGX, m_fgx);
	DDX_Text(pDX, IDC_ZONG_FGY, m_fgy);
	DDX_Radio(pDX, IDC_ZONG_LICHENG1, m_Licheng);
	DDX_Radio(pDX, IDC_ZONG_HEIGHT1, m_height);
	DDX_Text(pDX, IDC_ZONG_BEGINLC, m_dBeginLC);
	DDX_Text(pDX, IDC_ZONG_PLACEX, m_PlaceX);
	DDX_Text(pDX, IDC_ZONG_PLACEY, m_PlaceY);
	DDX_Radio(pDX, IDC_ZONG_HPMT1, m_nHpmt);
	DDX_Text(pDX, IDC_ZONG_WIDTH, m_dWidth);
	DDV_MinMaxDouble(pDX, m_dWidth, 1., 10000.);
	DDX_Text(pDX, IDC_ZONG_LCHHIGH, m_dLchHigh);
	DDV_MinMaxDouble(pDX, m_dLchHigh, 0.1, 100.);
	DDX_Text(pDX, IDC_ZONG_LCHMIN, m_dLchMin);
	DDV_MinMaxDouble(pDX, m_dLchMin, 0., 100.);
	DDX_Radio(pDX, IDC_ZONG_LCHWEI1, m_nLchWei);
}

BEGIN_MESSAGE_MAP(CDrawSection, CAcUiDialog)
	ON_BN_CLICKED(IDC_ZONG_IFBCH, OnZongIfbch)
	ON_BN_CLICKED(IDC_ZONG_IFFG, OnZongIffg)
	ON_BN_CLICKED(IDC_ZONG_PICK, OnZongPick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawSection message handlers

BOOL CDrawSection::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	GetDlgItem(IDC_ZONG_BCHTEXT)->EnableWindow(m_ifbch);
	GetDlgItem(IDC_ZONG_BIAOCHI)->EnableWindow(m_ifbch);

	GetDlgItem(IDC_ZONG_FGTEXTX)->EnableWindow(m_iffg == FALSE);
	GetDlgItem(IDC_ZONG_FGTEXTY)->EnableWindow(m_iffg == FALSE);
	GetDlgItem(IDC_ZONG_FGX)->EnableWindow(m_iffg == FALSE);
	GetDlgItem(IDC_ZONG_FGY)->EnableWindow(m_iffg == FALSE);

	GetDlgItem(IDC_ZONG_JGTEXT1)->EnableWindow(m_bAskJiange);
	GetDlgItem(IDC_ZONG_JGTEXT2)->EnableWindow(m_bAskJiange);
	GetDlgItem(IDC_ZONG_JGTEXT3)->EnableWindow(m_bAskJiange);
	GetDlgItem(IDC_ZONG_JGTEXT4)->EnableWindow(m_bAskJiange);
	GetDlgItem(IDC_ZONG_JGNUM)->EnableWindow(m_bAskJiange);
	GetDlgItem(IDC_ZONG_JGX)->EnableWindow(m_bAskJiange);
	GetDlgItem(IDC_ZONG_JGY)->EnableWindow(m_bAskJiange);

	GetDlgItem(IDC_ZONG_PMTTEXT)->EnableWindow(m_bAskHpmt);
	GetDlgItem(IDC_ZONG_HPMT1)->EnableWindow(m_bAskHpmt);
	GetDlgItem(IDC_ZONG_HPMT2)->EnableWindow(m_bAskHpmt);

	GetDlgItem(IDC_ZONG_LCTEXT1)->EnableWindow(m_bAskBegin);
	GetDlgItem(IDC_ZONG_BEGINLC)->EnableWindow(m_bAskBegin);
	GetDlgItem(IDC_ZONG_LCTEXT2)->EnableWindow(m_bAskBegin);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawSection::OnZongIfbch()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ZONG_BCHTEXT)->EnableWindow(m_ifbch);
	GetDlgItem(IDC_ZONG_BIAOCHI)->EnableWindow(m_ifbch);
}

void CDrawSection::OnZongIffg()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_ZONG_FGTEXTX)->EnableWindow(m_iffg == FALSE);
	GetDlgItem(IDC_ZONG_FGTEXTY)->EnableWindow(m_iffg == FALSE);
	GetDlgItem(IDC_ZONG_FGX)->EnableWindow(m_iffg == FALSE);
	GetDlgItem(IDC_ZONG_FGY)->EnableWindow(m_iffg == FALSE);
}

void CDrawSection::OnZongPick()
{
	UpdateData(TRUE);
	BeginEditorCommand();
	ads_point pt;
	if(ads_getpoint(NULL, _T("\n请指定断面图左下点:"), pt) == RTNORM)
	{
		CompleteEditorCommand();
		m_PlaceX = pt[X]; m_PlaceY = pt[Y];
		UpdateData(FALSE);
	}
	else CompleteEditorCommand();
}

void CDrawSection::OnOK()
{
	UpdateData(TRUE);
	if(m_ifbch == FALSE) m_biaochi = -1.0;
	if(m_iffg == TRUE)
	{
		m_fgx = -1; m_fgy = -1;
	}
	UpdateData(FALSE);

	CAcUiDialog::OnOK();
}

//绘单根断面线
void CDrawSection::DrawDuanmianLine(AcGePoint2dArray& Nodes, ads_point pt0, double dScaleX, double dScaleY,
	double zhMin, int bgMin, AcDbGroup *pGroup, bool bNext)
{
	int nLen = Nodes.length();
	if(nLen < 2) return;
	AcGePoint2dArray dmxArr;
	for(int j = 0; j < nLen; j++)
	{
		AcGePoint2d pt2d = Nodes.at(j);
		pt2d[X] = pt0[X] + 5.0 + (pt2d[X] - zhMin)*dScaleX;
		pt2d[Y] = pt0[Y] + (pt2d[Y] - bgMin)*dScaleY;
		dmxArr.append(pt2d);
	}
	DrawPolyline2D(dmxArr, 0.0, false, _T(""));

	ads_name ent;
	AcDbObjectId objId;
	ads_entlast(ent);
	acdbGetObjectId(objId, ent);
	pGroup->append(objId);
	if(true == bNext) setcolor(ent, 7); //设计线
}

void CDrawSection::draw()
{
	TCHAR szStr[255];
	double rr, dLastX;
	ads_point pt, pt0, pt1, pt2;
	AcGePoint2d pt2d;
	ads_name ent;
	AcDbObjectId groupId, objId;
	AcDbGroup *pGroup;
	double dScaleX, dScaleY, zhMin, zhMax, XM, YM;
	int bgMin, bgMax;

	Code1(7);
	//计算断面图比例范围
	if(m_Nodes.length() < 2) return;
	GetPolyExtent(m_Nodes, pt1, pt2, 0.0);

	// #ifndef NOSENSE
	// 	BYTE szIn[100],szOut[255],*pb;
	// 	DWORD returnlen;
	// 	XM=m_scalex; YM=m_scaley;
	// 	pb=szIn+0; memcpy(pb,&pt1[X],sizeof(double));
	// 	pb=szIn+8; memcpy(pb,&pt1[Y],sizeof(double));
	// 	pb=szIn+16; memcpy(pb,&pt2[X],sizeof(double));
	// 	pb=szIn+24; memcpy(pb,&pt2[Y],sizeof(double));
	// 	pb=szIn+32; memcpy(pb,&XM,sizeof(double));
	// 	pb=szIn+40; memcpy(pb,&YM,sizeof(double));
	// 	if ( S4_SUCCESS == S4Execute(&s4_context,"1033",szIn,48,szOut,255,&returnlen) ) 
	// 	{ //Dmt
	// 		if ( 56 == returnlen ) {
	// 			long lTemp;
	// 			pb=szOut+0; memcpy(&dScaleX,pb,sizeof(double));
	// 			pb=szOut+8; memcpy(&dScaleY,pb,sizeof(double));
	// 			pb=szOut+16; memcpy(&zhMin,pb,sizeof(double));
	// 			pb=szOut+24; memcpy(&zhMax,pb,sizeof(double));
	// 			pb=szOut+32; memcpy(&XM,pb,sizeof(double));
	// 			pb=szOut+40; memcpy(&YM,pb,sizeof(double));
	// 			pb=szOut+48; memcpy(&lTemp,pb,sizeof(long)); S4_ConvertSignLong(&lTemp); bgMin=lTemp;
	// 			pb=szOut+52; memcpy(&lTemp,pb,sizeof(long)); S4_ConvertSignLong(&lTemp); bgMax=lTemp;
	// 		}
	// 	}
	// #else
	dScaleX = 1000.0 / m_scalex;
	dScaleY = 1000.0 / m_scaley;
	bgMin = (int)floor(pt1[Y]) - 3;
	bgMax = (int)floor(pt2[Y]) + 3;
	zhMin = pt1[X];
	zhMax = pt2[X];
	XM = (zhMax - zhMin)*dScaleX + 10;
	YM = (bgMax - bgMin)*dScaleY;
	//#endif

	//断面主线
	m_biaochi *= dScaleX;
	pt0[X] = m_PlaceX; pt0[Y] = m_PlaceY; pt0[Z] = 0.0; //左下角位置

	ads_command(RTSTR, _T("layer"), RTSTR, _T("M"), RTSTR, _T("zdm1"), RTSTR, _T("C"), RTSTR, _T("2"), RTSTR, _T(""), RTSTR, _T(""), 0);
	ads_point_set(pt0, pt1); pt1[X] += 5; ads_point_set(pt0, pt2); pt2[X] += XM;
	drawline(pt1, pt2, _T(""));
	ads_entlast(ent);
	struct resbuf *rc;
	double d1 = m_scalex, d2 = m_scaley, d3 = zhMin + m_dBeginLC, d4 = bgMin;
	rc = ads_buildlist(-3, 1001, _T("south"), 1000, _T("991000"), 1040, d1, 1040, d2, 1040, d3, 1040, d4, 0);
	addappRC(ent, rc);
	ads_relrb(rc);
	if(false == CreateGroup(ent, groupId)) return;
	if(Acad::eOk != acdbOpenObject(pGroup, groupId, AcDb::kForWrite)) return;

	//画图框
	ads_point_set(pt0, pt1); pt1[X] -= 40.0; ads_point_set(pt0, pt); pt[X] += 5;
	drawline(pt1, pt, _T(""), pGroup);
	pt1[Y] -= 15.0; pt2[Y] -= 15.0; drawline(pt1, pt2, _T(""), pGroup);
	pt1[Y] -= 20.0; pt2[Y] -= 20.0; drawline(pt1, pt2, _T(""), pGroup);
	pt2[X] = pt1[X]; pt2[Y] = pt0[Y]; drawline(pt1, pt2, _T(""), pGroup);
	pt1[X] += 40; pt2[X] += 40; drawline(pt1, pt2, _T(""), pGroup);
	pt[X] = pt0[X] - 20.0; pt[Y] = pt0[Y] - 7.5;
	DrawText(pt, _T("地 面 高 程"), _T("M"), _T(""), 6.0);
	ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
	pt[Y] = pt0[Y] - 25;
	if(0 == m_Licheng) DrawText(pt, _T("里    程"), _T("M"), _T(""), 6.0);
	else DrawText(pt, _T("距    离"), _T("M"), _T(""), 6.0);
	ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

	//横断面里程,2004.04.20
	if(m_dLicheng >= 0.0)
	{
		ads_polar(pt0, -PI / 2, 39.0, pt);
		ads_polar(pt, 0.0, XM / 2.0, pt);
		GetLichengText(m_dLicheng, szStr);
		DrawText(pt, szStr, _T("M"), _T(""), 4.0);
		ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
	}

	//画标尺粗线
	int i;
	for(i = bgMin; i < bgMax; i = i + 2)
	{
		AcGePoint2dArray ptArr;
		pt2d[X] = pt0[X]; pt2d[Y] = pt0[Y] + (i - bgMin + 1)*dScaleY; ptArr.append(pt2d);
		pt2d[Y] += dScaleY; ptArr.append(pt2d);
		DrawPolyline2D(ptArr, 0.0, false, _T(""), 1.0);
		ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

		pt2d[X] = pt0[X] + XM; pt2d[Y] = pt0[Y] + (i - bgMin + 1)*dScaleY; ptArr.setAt(0, pt2d);
		pt2d[Y] += dScaleY; ptArr.setAt(1, pt2d);
		DrawPolyline2D(ptArr, 0.0, false, _T(""), 1.0);
		ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

		if(m_biaochi > 0.0)
		{//中间标尺
			for(rr = m_biaochi; rr < XM - m_biaochi; rr = rr + m_biaochi)
			{
				pt2d[X] = pt0[X] + rr; pt2d[Y] = pt0[Y] + (i - bgMin + 1)*dScaleY; ptArr.setAt(0, pt2d);
				pt2d[Y] += dScaleY; ptArr.setAt(1, pt2d);
				DrawPolyline2D(ptArr, 0.0, false, _T(""), 1.0);
				ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
			}
		}
	}

	//画标尺细线
	pt1[X] = pt2[X] = pt0[X] - 0.5; pt1[Y] = pt0[Y]; pt2[Y] = pt1[Y] + (bgMax - bgMin)*dScaleY;
	drawline(pt1, pt2, _T(""), pGroup);
	pt1[X] += 1.0; pt2[X] += 1.0; drawline(pt1, pt2, _T(""), pGroup);
	pt1[X] = pt2[X] = pt0[X] + XM - 0.5; drawline(pt1, pt2, _T(""), pGroup);
	pt1[X] += 1.0; pt2[X] += 1.0; drawline(pt1, pt2, _T(""), pGroup);
	if(m_biaochi > 0.0)
	{//中间标尺
		for(rr = m_biaochi; rr < XM - m_biaochi; rr = rr + m_biaochi)
		{
			pt1[X] = pt2[X] = pt0[X] + rr - 0.5; drawline(pt1, pt2, _T(""), pGroup);
			pt1[X] += 1.0; pt2[X] += 1.0; drawline(pt1, pt2, _T(""), pGroup);
		}
	}

	//注记标尺分划
	for(i = bgMin; i < bgMax; i++)
	{
		if((i - floor(i / 5.0)*5.0) < 0.001)
		{//逢5逢0才注记
			pt1[X] = pt0[X] - 10; pt1[Y] = pt0[Y] + (i - bgMin)*dScaleY;
			pt2[X] = pt0[X] + XM + 10.; pt2[Y] = pt1[Y];
			if(m_fgx <= 0 || m_fgy <= 0)
			{
				drawline(pt1, pt2, _T(""), pGroup);
				ads_entlast(ent); setlinetype(ent, _T("x4")); //标尺横线
			}
			_stprintf(szStr, _T("%d"), i);
			pt1[X] += 9; DrawText(pt1, szStr, _T("r"), _T(""), 3.0);
			ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
			pt2[X] -= 9; DrawText(pt2, szStr, _T(""), _T(""), 3.0);
			ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
			if(m_biaochi > 0.0)
			{//中间标尺
				for(rr = m_biaochi; rr < XM - m_biaochi; rr = rr + m_biaochi)
				{
					pt1[X] = pt0[X] + rr - 1; DrawText(pt1, szStr, _T("r"), _T(""), 3.0);
					ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
				}
			}
		}
	}

	//方格网水平线
	if(m_fgx > 0 && m_fgy > 0)
	{
		rr = pt0[Y] + m_fgy;
		pt1[X] = pt0[X] + 5.0; pt2[X] = pt0[X] + XM - 5.0;
		while(rr <= pt0[Y] + YM)
		{
			pt1[Y] = pt2[Y] = rr;
			drawline(pt1, pt2, _T(""), pGroup);
			ads_entlast(ent); setlinetype(ent, _T("x4"));
			rr += m_fgy;
		}
	}

	//绘地面线
	ads_command(RTSTR, _T("layer"), RTSTR, _T("M"), RTSTR, _T("DMX"), RTSTR, _T("C"), RTSTR, _T("1"), RTSTR, _T(""), RTSTR, _T(""), 0);
	DrawDuanmianLine(m_Nodes, pt0, dScaleX, dScaleY, zhMin, bgMin, pGroup, false);
	i = 0;
	while(i < m_NextNodes.length() - 1)
	{//绘制设计线,可能有多条
		bool bHaveDrawn = false;
		AcGePoint2dArray dmxArr;
		while(i < m_NextNodes.length())
		{
			pt2d = m_NextNodes.at(i);
			i++;
			if(pt2d.x < -1000000 && pt2d.x < -1000000)
			{
				DrawDuanmianLine(dmxArr, pt0, dScaleX, dScaleY, zhMin, bgMin, pGroup, true);
				bHaveDrawn = true;
				break;
			}
			else
			{
				dmxArr.append(pt2d);
			}
		}
		if(false == bHaveDrawn)
			DrawDuanmianLine(dmxArr, pt0, dScaleX, dScaleY, zhMin, bgMin, pGroup, true);
	}

	//里程及地面高程
	ads_command(RTSTR, _T("layer"), RTSTR, _T("M"), RTSTR, _T("ZDMT5"), RTSTR, _T("C"), RTSTR, _T("3"), RTSTR, _T(""), RTSTR, _T(""), 0);
	for(i = 0; i < m_Nodes.length(); i++)
	{
		pt2d = m_Nodes.at(i);
		if(0 == i || fabs(pt2d[X] - dLastX) >= m_dLchMin / dScaleX)
		{ //防止重叠在一起
			pt[X] = pt0[X] + 5.0 + (pt2d[X] - zhMin)*dScaleX - 0.5; pt[Y] = pt0[Y] - 25.0;
			if(0 == m_Licheng)
			{ //按里程方式注记
				TCHAR szTemp[50];
				int kilo, fuhao = 1;
				double dOld = pt2d[X] + m_dBeginLC;
				if(dOld < 0.0)
				{
					dOld *= -1.0; fuhao = -1;
				}
				kilo = (int)(dOld / 1000.0);
				double dNew = dOld - kilo*1000.0;

				ads_rtos(dNew, 2, m_nLchWei, szTemp);
				if(dNew < 10.0)
					_stprintf(szStr, _T("%d+00%s"), kilo, szTemp);
				else if(dNew < 100.0)
					_stprintf(szStr, _T("%d+0%s"), kilo, szTemp);
				else
					_stprintf(szStr, _T("%d+%s"), kilo, szTemp);
				if(fuhao < 0)
				{ //里程为负
					_stprintf(szTemp, _T("-%s"), szStr);
					_tcscpy(szStr, szTemp);
				}
			}
			else
			{
				ads_rtos(pt2d[X], 2, m_nLchWei, szStr);
			}
			DrawText(pt, szStr, _T("bc"), _T(""), m_dLchHigh, PI / 2);
			ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

			pt[Y] = pt0[Y] - 7.5;
			ads_rtos(pt2d[Y], 2, m_height + 1, szStr); //高程注记位数根据设置
			DrawText(pt, szStr, _T("bc"), _T(""), m_dLchHigh, PI / 2);
			ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
			pt[Y] = pt0[Y]; pt[X] += 0.5; ads_point_set(pt, pt1); pt1[Y] -= 35.0;
			drawline(pt1, pt, _T(""), pGroup);
			dLastX = pt2d[X];
		}
	}

	//方格竖线
	if(m_fgx <= 0 || m_fgy <= 0)
	{
		for(i = 0; i < m_Nodes.length(); i++)
		{//各点的竖线
			pt2d = m_Nodes.at(i);
			pt1[X] = pt0[X] + 5.0 + (pt2d[X] - zhMin)*dScaleX; pt1[Y] = pt0[Y];
			pt2[X] = pt1[X]; pt2[Y] = pt0[Y] + (pt2d[Y] - bgMin)*dScaleY;
			drawline(pt1, pt2, _T(""), pGroup);
			ads_entlast(ent); setlinetype(ent, _T("x4"));
		}
	}
	else
	{
		rr = pt0[X] + 5.0;
		pt1[Y] = pt0[Y]; pt2[Y] = pt0[Y] + YM;
		while(rr <= pt0[X] + XM - 5.0)
		{
			pt1[X] = pt2[X] = rr;
			drawline(pt1, pt2, _T(""));
			ads_entlast(ent); setlinetype(ent, _T("x4"));
			rr += m_fgx;
		}
	}

	_stprintf(szStr, _T("比例尺:  横向 1:%d  纵向 1:%d"), m_scalex, m_scaley);
	pt[X] = pt0[X] + XM - 10.0; pt[Y] = pt0[Y] + YM + 5.0;
	DrawText(pt, szStr, _T("r"), _T(""), 5.0);
	ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

	if(1 == m_nHpmt) DrawPmt(pGroup, XM); //绘平面图
	pGroup->close();
}

void CDrawSection::DrawPmt(AcDbGroup *pGroup, double XM)
{
	ads_point pt, pt0, pt1, pt2, pt3, pt4, ptB, ptLast;
	AcGePoint2d pt2d;
	AcGePoint2dArray NodeArr;
	ads_name ent, ssName;
	AcDbObjectId objId;
	long l, Length;
	double lastangle;
	TCHAR szBlock[50] = _T("hpmt$.dwg"), szTemp[MAX_PATH];

	int oldGroup = GroupReset();
	ptB[X] = m_PlaceX; ptB[Y] = m_PlaceY; ptB[Z] = 0.0; //左下角位置
	ads_command(RTSTR, _T("layer"), RTSTR, _T("M"), RTSTR, _T("zdm1"), RTSTR, _T("OFF"), RTSTR, _T("ASSIST"), RTSTR, _T(""), RTNONE);
	GetPlList(m_Ent, NodeArr);
	FullCurView(NodeArr, 20.0);
	ads_polar(ptB, PI*1.5, 35.0, pt1);
	ads_polar(pt1, PI*1.5, 15.0 + m_dWidth, pt2);
	drawline(pt1, pt2, _T(""), pGroup);
	pt1[X] -= 40; pt2[X] -= 40;
	drawline(pt1, pt2, _T(""), pGroup);
	pt1[X] = ptB[X] - 40; pt1[Y] = ptB[Y] - 50; pt2[X] = ptB[X] + XM; pt2[Y] = ptB[Y] - 50;
	drawline(pt1, pt2, _T(""), pGroup);
	pt1[Y] -= m_dWidth; pt2[Y] -= m_dWidth;
	drawline(pt1, pt2, _T(""), pGroup);
	pt1[X] = ptB[X] - 20; pt1[Y] = ptB[Y] - 42.5;
	DrawText(pt1, _T("转 折 角"), _T("M"), _T(""), 6.0);
	ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);
	pt1[Y] -= 7.5 + m_dWidth / 2.0;
	DrawText(pt1, _T("平 面 图"), _T("M"), _T(""), 6.0);
	ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

	ads_polar(ptB, PI*1.5, 50 + m_dWidth / 2.0, pt); ads_polar(pt, 0, 5, pt);
	double dTotalDist = m_dBeginLC;
	for(int i = 0; i < NodeArr.length() - 1; i++)
	{
		pt0[X] = NodeArr.at(i)[X]; pt0[Y] = NodeArr.at(i)[Y]; pt0[Z] = 0.0;
		pt4[X] = NodeArr.at(i + 1)[X]; pt4[Y] = NodeArr.at(i + 1)[Y]; pt4[Z] = 0.0;
		double angle = ads_angle(pt0, pt4);
		double juli = distOFpt(pt0, pt4);
		if(juli < 0.5)
		{//线段太短
			pt[X] += juli*1000.0 / m_scalex; dTotalDist += juli;
			continue;
		}
		ads_polar(pt0, angle + PI / 2, 0.5*m_dWidth*m_scalex / 1000, pt1);
		ads_polar(pt0, angle - PI / 2, 0.5*m_dWidth*m_scalex / 1000, pt2);
		ads_polar(pt4, angle - PI / 2, 0.5*m_dWidth*m_scalex / 1000, pt3);
		ads_polar(pt4, angle + PI / 2, 0.5*m_dWidth*m_scalex / 1000, pt4);

		AcGePoint2dArray Nodes;
		pt2d[X] = pt1[X]; pt2d[Y] = pt1[Y]; Nodes.append(pt2d);
		pt2d[X] = pt2[X]; pt2d[Y] = pt2[Y]; Nodes.append(pt2d);
		pt2d[X] = pt3[X]; pt2d[Y] = pt3[Y]; Nodes.append(pt2d);
		pt2d[X] = pt4[X]; pt2d[Y] = pt4[Y]; Nodes.append(pt2d);
		ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
		DrawPolyline2D(Nodes, 0.0, true, _T(""));
		ads_entlast(ent);
		TrimPoly(ent, true, false, false);
		ads_entdel(ent);
		if(ads_findfile(szBlock, szTemp) == RTNORM)
			ads_command(RTSTR, _T("wblock"), RTSTR, szBlock, RTSTR, _T("Y"), RTSTR, _T(""), RTPOINT, pt0, RTSTR, _T("cp"), RTPOINT, pt1, RTPOINT, pt2, RTPOINT, pt3, RTPOINT, pt4, RTSTR, _T(""), RTSTR, _T(""), RTNONE);
		else
			ads_command(RTSTR, _T("wblock"), RTSTR, szBlock, RTSTR, _T(""), RTPOINT, pt0, RTSTR, _T("cp"), RTPOINT, pt1, RTPOINT, pt2, RTPOINT, pt3, RTPOINT, pt4, RTSTR, _T(""), RTSTR, _T(""), RTNONE);
		ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
		ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);

		if(i > 0)
		{//转角
			int du, fen;
			double miao;
			AcGePoint2dArray ptArr;

			ads_point_set(pt, pt1); pt1[X] += 6.0; pt1[Y] += 10.0 + 0.5*m_dWidth;
			ads_point_set(pt1, pt2); pt2[X] += 16.0;
			drawline(pt1, pt2, _T(""), pGroup);
			int val = (int)(dTotalDist / 1000.0);
			_stprintf(szTemp, _T("%d+%.2f"), val, dTotalDist - val*1000.0);
			pt3[X] = (pt1[X] + pt2[X]) / 2.0; pt3[Y] = pt1[Y] + 2.0;
			DrawText(pt3, szTemp, _T("M"), _T(""), 2.0);
			ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

			double inangle = lastangle + PI - angle;
			if(inangle > PI*2.0) inangle -= PI*2.0;
			else if(inangle < 0.0) inangle += PI*2.0;
			inangle *= 180.0 / PI;
			undegree(inangle, du, fen, miao);
			_stprintf(szTemp, _T("%d°%d′%.f″"), du, fen, miao);
			pt3[Y] -= 4.0;
			DrawText(pt3, szTemp, _T("M"), _T(""), 2.0);
			ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

			pt2d[X] = pt[X] - 3.0; pt2d[Y] = ptB[Y] - 45.0; ptArr.append(pt2d);
			pt2d[X] = pt[X]; pt2d[Y] = ptB[Y] - 40.0; ptArr.append(pt2d);
			pt2d[X] = pt[X] + 3.0; pt2d[Y] = ptB[Y] - 45.0; ptArr.append(pt2d);
			DrawPolyline2D(ptArr, 0.0, false, _T(""));
			ads_entlast(ent); acdbGetObjectId(objId, ent); pGroup->append(objId);

			pt1[Y] = pt2[Y] = ptB[Y] - 45;
			pt1[X] = ptLast[X]; if(i > 1) pt1[X] += 3.0;
			pt2[X] = pt[X] - 3.0;
			drawline(pt1, pt2, _T(""), pGroup);
		}
		DrawInsert(pt, _T("hpmt$"), _T(""), 1000.0 / m_scalex, -1.0*angle);
		ads_entlast(ent);
		ads_command(RTSTR, _T("EXPLODE"), RTENAME, ent, RTNONE);

		ads_ssget(_T("P"), NULL, NULL, NULL, ssName);
		Length = 0; ads_sslength(ssName, &Length);
		for(l = 0; l < Length; l++)
		{//旋转图块与文字
			struct resbuf *rc, *rb;
			TCHAR entype[50], szCode[50];
			ads_ssname(ssName, l, ent);
			GetStdm(ent, szCode);
			if(!_tcscmp(szCode, _T("171111")) || !_tcscmp(szCode, _T("171211")) || !_tcscmp(szCode, _T("172011"))
				|| !_tcscmp(szCode, _T("171101-2")) || !_tcscmp(szCode, _T("171201-2")) || !_tcscmp(szCode, _T("172001-2")))
			{//电力线箭头
				continue;
			}

			rc = ads_entget(ent);
			rb = rc; while(rb->restype != 0) rb = rb->rbnext; _tcscpy(entype, rb->resval.rstring);
			if(_tcsicmp(entype, _T("INSERT")) && _tcsicmp(entype, _T("TEXT")))
			{
				ads_relrb(rc); continue;
			}
			rb = rc; while(rb->restype != 50) rb = rb->rbnext;
			if(rb->resval.rreal != 0.0)
			{
				rb->resval.rreal = 0.0;
				ads_entmod(rc);
			}
			ads_relrb(rc);
		}
		ads_ssfree(ssName);
		ads_command(RTSTR, _T("PURGE"), RTSTR, _T("B"), RTSTR, _T("hpmt$"), RTSTR, _T("N"), RTNONE);
		_tremove(szBlock);

		ads_point_set(pt, ptLast);
		pt[X] += juli*1000.0 / m_scalex;
		dTotalDist += juli;
		lastangle = angle;
	}
	pt1[Y] = pt2[Y] = ptB[Y] - 45.0; pt1[X] = ptLast[X] + 3.0; pt2[X] = pt[X];
	drawline(pt1, pt2, _T(""), pGroup);
	ads_command(RTSTR, _T("layer"), RTSTR, _T("ON"), RTSTR, _T("ASSIST"), RTSTR, _T(""), RTNONE);
	GroupBack(oldGroup);
}

// CSetProjectCodeDlg 对话框

IMPLEMENT_DYNAMIC(XMJGProjectCodeDlg, CDialog)

XMJGProjectCodeDlg::XMJGProjectCodeDlg(CWnd* pParent /*=NULL*/)
: CDialog(XMJGProjectCodeDlg::IDD, pParent)
{

}

XMJGProjectCodeDlg::XMJGProjectCodeDlg(const TCHAR *title, bool flag)
	: CDialog(XMJGProjectCodeDlg::IDD, NULL)
	, _manager(0), m_isconfig(flag)
{
	try
	{
		m_name.Format(_T("%s"), title);
		m_title.Format(_T("%s受理编号设置"), title);
		_manager = new CDatabaseOperManager; TCHAR path[255] = {0};
		if(RTNORM != ads_findfile(_T("xmjg.mdb"), path)) return;
		_manager->initialConnect(path);
	}
	catch(...)
	{
		printErrorInfo(__FILE__, __FUNCTION__, __LINE__);
	}
}

XMJGProjectCodeDlg::~XMJGProjectCodeDlg()
{
}

void XMJGProjectCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETACCEPTCODE, m_SetAcceptCode);
	DDX_Control(pDX, IDC_PROJECTDIR, m_ProjectDir);
}

BEGIN_MESSAGE_MAP(XMJGProjectCodeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGProjectCodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()

CString XMJGProjectCodeDlg::GetAcceptCode()
{
	return m_code;
}

CString XMJGProjectCodeDlg::GetProjectDir()
{
	return m_dir;
}

BOOL XMJGProjectCodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	try
	{
		this->SetWindowText(m_title);
		CString table(_T("XMJGPath")); MStr filter;
		filter[_T("关键字")] = _T("工程目录");
		_manager->ReadDataBase(table, _T("属性值"), filter, m_dir);
		m_dir.Append(m_name); m_dir.Append(_T("\\"));
		m_ProjectDir.SetWindowText(m_dir);
		m_SetAcceptCode.SetWindowText(m_code);
	}
	catch(...)
	{
		printErrorInfo(__FILE__, __FUNCTION__, __LINE__);
	}
	return TRUE;
}

void XMJGProjectCodeDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_SetAcceptCode.GetWindowText(m_code);
	if(m_code.GetLength() == 0)
	{
		MessageBox(_T("请输入受理号"), _T("错误信息"));
		return;
	}
	m_ProjectDir.GetWindowText(m_dir);
	if(false == createProjectDir()) return;
	CString path; path.Format(_T("%s%s"), m_dir, m_code);
	MStr filter, record;
	filter[_T("关键字")] = _T("当前目录");
	record[_T("属性值")] = path;
	_manager->UpdataRecord(_T("XMJGPath"), filter, record);
	CDatabaseOperManager *pdb = new CDatabaseOperManager;
	pdb->initialConnect(path + _T("\\GC\\项目信息.mdb"));
	record.clear(); record[_T("业务名称")] = m_name;
	record[_T("受理号")] = m_code; filter.clear();
	pdb->UpdataRecord(_T("XMJGProjectInfo"), filter, record);
	delete pdb;
	CDialog::OnOK();
}

bool XMJGProjectCodeDlg::createProjectDir()
{
	if(_taccess(m_dir, 0) == -1) _tmkdir(m_dir);
	CStringArray files, paths; CString path(m_dir + m_code);
	CFilePathOperater::GetChildDirectory(m_dir, paths, files);
	for(int idx = 0; idx < files.GetSize(); ++idx)
	{
		if(files.GetAt(idx).Find(m_code) == -1) continue;
		CString info; info.Format(_T("是否覆盖文件：%s"), path);
		int ret = MessageBox(info, _T("信息提示"), MB_OKCANCEL);
		if(ret == 1) CFilePathOperater::ClearFolder(path, false);
		else return false;
	}
	_tmkdir(path);
	TCHAR pdb[255] = {0};
	_tmkdir(path + _T("\\CG"));
	_tmkdir(path + _T("\\CK"));
	_tmkdir(path + _T("\\GC"));
	ads_findfile(_T("project.mdb"), pdb);
	CopyFile(pdb, path + _T("\\GC\\项目信息.mdb"), TRUE);
	ads_findfile(_T("项目图形.dwg"), pdb);
	CopyFile(pdb, path + _T("\\GC\\项目图形.dwg"), TRUE);
	return true;
}

XMJGReadReg::XMJGReadReg()
{
	int es = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), 0, KEY_ALL_ACCESS, &m_rkey);
	m_rkey = es == ERROR_SUCCESS ? m_rkey : 0;
}

CString XMJGReadReg::ReadReg(const TCHAR *name)
{
	if(m_rkey == NULL) return _T("");
	TCHAR value[1000] = {0};
	cass::CassQueryReg(m_rkey, name, value);
	CString val; val.Format(_T("%s"), value);
	return val;
}

void XMJGReadReg::WriteReg(const TCHAR *name, const TCHAR *value)
{
	if(m_rkey == NULL) return;
	cass::CassSetReg(m_rkey, name, value);
}

XMJGReadReg::~XMJGReadReg()
{
	if(m_rkey == NULL) return;
	RegCloseKey(m_rkey);
}

// XMJGLicenseDocDlg 对话框

IMPLEMENT_DYNAMIC(XMJGLicenseDocDlg, CDialog)

XMJGLicenseDocDlg::XMJGLicenseDocDlg(CWnd* pParent /*=NULL*/)
: CDialog(XMJGLicenseDocDlg::IDD, pParent)
{

}

XMJGLicenseDocDlg::~XMJGLicenseDocDlg()
{
}

void XMJGLicenseDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LICENSEDOC, m_LicenseDoc);
}

bool XMJGLicenseDocDlg::ReadMDB()
{
	TCHAR mdbpath[256] = {0};
	ads_findfile(_T("lstitems.mdb"), mdbpath);
	if(RTNORM != ads_findfile(_T("lstitems.mdb"), mdbpath))
	{
		acutPrintf(_T("\n 无法找到lstitems.mdb文件！！！"));
		return false;
	}
	CDatabaseOperManager manager;
	manager.initialConnect(mdbpath);
	return manager.GetValueByField(_T("TBL_XKZWS"), _T("LicenseDoc"), m_data);
}

bool XMJGLicenseDocDlg::InitListCtrl()
{
	int count = (int)m_data.size();
	CString value; int idx = 0;	CRect rect;
	m_LicenseDoc.GetClientRect(&rect);
	int width = (rect.right - rect.left - 25) / 5;
	m_LicenseDoc.InsertColumn(0, _T(""), LVCFMT_CENTER, 25);
	m_LicenseDoc.InsertColumn(1, _T("许可文书"), LVCFMT_LEFT, width * 3);
	m_LicenseDoc.InsertColumn(2, _T("编号"), LVCFMT_LEFT, width * 2);
	for(int idx = 0; idx < count; ++idx)
	{
		m_LicenseDoc.InsertItem(idx, _T(""));
		m_LicenseDoc.SetCheck(idx, FALSE);
		m_LicenseDoc.SetItemText(idx, 1, m_data.at(idx));
	}
	return true;
}

BOOL XMJGLicenseDocDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_LicenseDoc.Init();
	DWORD lvs = m_LicenseDoc.GetExtendedStyle();
	lvs |= LVS_EX_CHECKBOXES | LVS_EX_INFOTIP | LVS_EX_CHECKBOXES;
	m_LicenseDoc.SetExtendedStyle(lvs);
	if(false == ReadMDB()) return FALSE;
	InitListCtrl();
	return TRUE;
}

BEGIN_MESSAGE_MAP(XMJGLicenseDocDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LICENSEDOC, OnLvnClickListItem)
END_MESSAGE_MAP()

void XMJGLicenseDocDlg::getLicenseDoc(MStr &data)
{
	MSIter it = m_Ret.begin();
	for(; it != m_Ret.end(); ++it)
		data[it->first] = it->second;
}

void XMJGLicenseDocDlg::OnOK()
{
	CString license;
	int row = m_LicenseDoc.GetItemCount();
	for(int idx = 0; idx < row; ++idx)
	{
		CString name = m_LicenseDoc.GetItemText(idx, 0);
		CString lice = m_LicenseDoc.GetItemText(idx, 1);
		if(name.GetLength() == 0 || lice.GetLength() == 0)
		{
			license.Format(_T("许可文书：%s的编号为空！！！"), name);
			MessageBox(license, _T("错误提示"), MB_OK);
			return;
		}
		m_Ret[name] = lice;
	}
	CDialog::OnOK();
}

void XMJGLicenseDocDlg::OnLvnClickListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	int oldstate = INDEXTOSTATEIMAGEMASK(1);
	int newstate = INDEXTOSTATEIMAGEMASK(2);
	LPNMITEMACTIVATE   pNMLV = (LPNMITEMACTIVATE)pNMHDR;
	if((pNMLV->uOldState & oldstate) /* old state : unchecked */
		&& (pNMLV->uNewState & newstate)) /* new state : checked */
	{
		int index = pNMLV->iItem;
		m_Data[index] = m_data.at(index);
	}
	else if((pNMLV->uOldState & newstate) /* old state : checked */
		&& (pNMLV->uNewState & oldstate))/* new state : unchecked */
	{
		int index = pNMLV->iItem;
		m_Data.erase(index);
	}
}

// XMJGChangePictureProperty 对话框

IMPLEMENT_DYNAMIC(XMJGChangePictureProperty, CDialog)

XMJGChangePictureProperty::XMJGChangePictureProperty(CWnd* pParent /*=NULL*/)
: CDialog(XMJGChangePictureProperty::IDD, pParent)
{

}

XMJGChangePictureProperty::XMJGChangePictureProperty(const TCHAR *path)
	: CDialog(XMJGChangePictureProperty::IDD), m_type(_T(""))
{
	m_dir.Format(_T("%s"), path);
}

XMJGChangePictureProperty::~XMJGChangePictureProperty()
{
}

void XMJGChangePictureProperty::setPicturePropertyType(const CString &type)
{
	//m_type = type;
}

void XMJGChangePictureProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XMJGPICTUREBROWING, m_PictureBrowingInfo);
	DDX_Control(pDX, IDC_XMJGMOVEUP, m_moveup);
	DDX_Control(pDX, IDC_XMJGMOVEDOWN, m_movedown);
	DDX_Control(pDX, IDC_XMJGMOVETOP, m_movetop);
	DDX_Control(pDX, IDC_XMJGMOVEBUTTOM, m_movebuttom);
}

BEGIN_MESSAGE_MAP(XMJGChangePictureProperty, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGChangePictureProperty::OnBnClickedOk)
	ON_MESSAGE(WM_SET_ITEMS, &XMJGChangePictureProperty::PopulateComboList)
	ON_BN_CLICKED(IDC_XMJGMOVEUP, &XMJGChangePictureProperty::OnBnClickedXMJGMMoveUp)
	ON_BN_CLICKED(IDC_XMJGMOVETOP, &XMJGChangePictureProperty::OnBnClickedXMJGMoveTop)
	ON_BN_CLICKED(IDC_XMJGMOVEDOWN, &XMJGChangePictureProperty::OnBnClickedMJGMoveDown)
	ON_BN_CLICKED(IDC_XMJGMOVEBUTTOM, &XMJGChangePictureProperty::OnBnClickedMJGMoveButtom)
	ON_BN_CLICKED(IDC_XMJGADDPIXTURE, &XMJGChangePictureProperty::OnBnClickedXmjgAddPixture)
	ON_BN_CLICKED(IDC_XMJGDELPICTURE, &XMJGChangePictureProperty::OnBnClickedXMJGDelPicture)
	ON_NOTIFY(NM_CLICK, IDC_XMJGPICTUREBROWING, &XMJGChangePictureProperty::OnClickedBrowIngPicture)
END_MESSAGE_MAP()

BOOL XMJGChangePictureProperty::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	InitListCtrl();
	readConfigFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGChangePictureProperty::InitListCtrl()
{
	IDataBaseOper oper; bool flag = false;
	if(true == m_type.IsEmpty())
		flag = oper.readCommonTable(_T("图片输出样式"), m_nameproperty);
	else
		flag = oper.readCommonTable(m_type, m_nameproperty);
	if(false == flag) return;
	m_PictureBrowingInfo.Init(TRUE, 24); CRect rect;
	m_PictureBrowingInfo.SetExtendedStyle(m_PictureBrowingInfo.GetExtendedStyle()
		| LVS_OWNERDRAWFIXED | LVS_EX_DOUBLEBUFFER);
	m_PictureBrowingInfo.GetClientRect(&rect);
	int width = (rect.right - rect.left - 60) / 3;
	m_PictureBrowingInfo.InsertColumn(0, _T("序    号"), LVCFMT_CENTER, 60);
	m_PictureBrowingInfo.InsertColumn(1, _T("名    称"), LVCFMT_CENTER, width);
	m_PictureBrowingInfo.InsertColumn(2, _T("描    述"), LVCFMT_CENTER, width);
	m_PictureBrowingInfo.InsertColumn(3, _T("位    置"), LVCFMT_LEFT, width);
	m_PictureBrowingInfo.SetColumnReadOnly(0, TRUE);
	m_PictureBrowingInfo.SetColumnReadOnly(1, TRUE);
	MSIter it = m_nameproperty.begin();
	for(; it != m_nameproperty.end(); ++it)
	{
		m_propertyname[it->second] = it->first;
		m_combox += _T(";") + it->second;
	}
}

bool XMJGChangePictureProperty::selectPictures()
{
	IProjectMDB pdb;
	MStr jbxx = pdb.getJBXXTableInfo();
	CString name_code = jbxx[_T("受理号")].MakeLower() + _T("_") + jbxx[_T("项目名称")].MakeLower() + _T("\\规划\\ck\\");
	CString filter = _T("图片文件|*.jpg;*.bmp;*.png;*.wmf|所有文件|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL,
		OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY,
		filter);
	IDataBaseOper oper;
	CString ppath = oper.readPathTable("当前目录"); ppath += _T("\\CK\\");
	fileDlg.m_ofn.lpstrInitialDir = ppath.GetBuffer(); ppath.ReleaseBuffer();
	const int MIN_FILE_NUMBER = 10;																//至少允许选择10个文件
	fileDlg.m_ofn.lpstrFile = new TCHAR[_MAX_PATH * MIN_FILE_NUMBER];							//重新定义缓冲区大小          
	memset(fileDlg.m_ofn.lpstrFile, 0, _MAX_PATH * MIN_FILE_NUMBER);							//初始化定义的缓冲区
	fileDlg.m_ofn.nMaxFile = _MAX_PATH * MIN_FILE_NUMBER;
	map<CString, int> files;
	if(IDOK == fileDlg.DoModal())
	{
		CString info; int index = 0;
		POSITION pos = fileDlg.GetStartPosition();
		while(NULL != pos)
		{
			CString fileName = fileDlg.GetNextPathName(pos).MakeLower();
			files[fileName] = index++;
		}
	}
	map<CString, int>::iterator it = files.begin();
	for(; it != files.end(); ++it)
	{
		CString fileName = it->first;
		CStringArray temps; MStr record;
		record[_T("路径")] = fileName;
		AnalyzeData(temps, fileName, name_code);
		if(temps.GetSize() == 1)
		{
			MessageBox(_T("选择的不是项目路径中CK文件夹中的图片"), _T("错误信息"), MB_OK);
			continue;
		}
		int row = m_PictureBrowingInfo.GetItemCount();
		CString info; info.Format(_T("%d"), row);
		m_PictureBrowingInfo.InsertItem(row, info);
		CString name = temps.GetAt(1);
		m_PictureBrowingInfo.SetItemText(row, 1, name);
		CString tName = name.MakeLower();
		int pos = tName.FindOneOf(_T("."));
		CString suffix = tName.Mid(pos + 1);
		if(suffix.CompareNoCase(_T("jpg"))  == 0)
		{
			tName = tName.Mid(0, pos);
			m_PictureBrowingInfo.SetItemText(row, 2, tName);
		}
		pos = tName.FindOneOf(_T("_"));
		m_PictureBrowingInfo.SetComboColumns(3, m_combox);
		record[_T("序号")].Format(_T("%d"), row);
		record[_T("名称")] = name; m_records.push_back(record);
	}
	delete[] fileDlg.m_ofn.lpstrFile;     //最后别忘了释放内存
	return true;
}

bool XMJGChangePictureProperty::selectPicture(int row)
{
	CString filter = _T("图片文件|*.jpg;*.bmp;*.png|所有文件|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, 6, filter);
	if(IDOK == fileDlg.DoModal())
	{
		CString fileName = fileDlg.GetPathName();
		//m_records[row][_T("路径")] = fileName;
		int pos = fileName.ReverseFind(_T('\\'));
		CString name = fileName.Mid(pos + 1);
		name = name.Left(name.GetLength() - 4);
		m_records[row][_T("路径")] = name;
		m_records[row][_T("名称")] = name;
		m_PictureBrowingInfo.SetItemText(row, 1, name);
	}
	return true;
}

void XMJGChangePictureProperty::swapPicturePosition(int front, int last)
{
	CString desc1 = m_PictureBrowingInfo.GetItemText(front, 2);
	CString desc2 = m_PictureBrowingInfo.GetItemText(last, 2);
	CString loca1 = m_PictureBrowingInfo.GetItemText(front, 3);
	CString loca2 = m_PictureBrowingInfo.GetItemText(last, 3);
	m_records[front][_T("描述")] = desc1;
	m_records[last][_T("描述")] = desc2;
	m_records[front][_T("位置")] = m_propertyname[loca1];
	m_records[last][_T("位置")] = m_propertyname[loca2];
	swap(m_records.at(front), m_records.at(last));
	setListShow(front);	setListShow(last);
	m_PictureBrowingInfo.setSelectItem(last);
}

void XMJGChangePictureProperty::readConfigFile()
{
	IProjectMDB pdb;
	m_records = pdb.getPictureTable();
	int len = (int)m_records.size();
	CString wz;
	for(int idx = 0; idx < len; ++idx)
	{
		MStr record = m_records.at(idx);
		m_PictureBrowingInfo.InsertItem(LVIF_TEXT | LVIF_STATE, idx, record[_T("序号")],
			LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);
		m_PictureBrowingInfo.SetItemText(idx, 1, record[_T("名称")]);
		m_PictureBrowingInfo.SetItemText(idx, 2, record[_T("描述")]);
		m_PictureBrowingInfo.SetComboColumns(3, m_combox);
		wz = record[_T("位置")];
		m_PictureBrowingInfo.SetItemText(idx, 3, m_nameproperty[wz]);
	}
}

void XMJGChangePictureProperty::setListShow(int index)
{
	MStr record = m_records[index];
	m_PictureBrowingInfo.SetItemText(index, 0, record[_T("序号")]);
	m_PictureBrowingInfo.SetItemText(index, 1, record[_T("名称")]);
	m_PictureBrowingInfo.SetItemText(index, 2, record[_T("描述")]);
	CString wz = record[_T("位置")];
	m_PictureBrowingInfo.SetItemText(index, 3, m_nameproperty[wz]);
	UpdateData(FALSE);
}

void XMJGChangePictureProperty::readPictureInfo()
{

}

void XMJGChangePictureProperty::writePictureInfo()
{

}

void XMJGChangePictureProperty::OnBnClickedXMJGMMoveUp()
{
	// TODO:  在此添加控件通知处理程序代码
	int idx = m_PictureBrowingInfo.GetSelectedItem();
	if(idx == 0) return; swapPicturePosition(idx, idx - 1);
}

void XMJGChangePictureProperty::OnBnClickedXMJGMoveTop()
{
	// TODO:  在此添加控件通知处理程序代码
	int idx = m_PictureBrowingInfo.GetSelectedItem();
	if(idx == 0) return; swapPicturePosition(idx, 0);
}

void XMJGChangePictureProperty::OnBnClickedMJGMoveDown()
{
	// TODO:  在此添加控件通知处理程序代码
	int idx = m_PictureBrowingInfo.GetSelectedItem();
	int count = m_PictureBrowingInfo.GetItemCount() - 1;
	if(idx == count) return; swapPicturePosition(idx, idx + 1);
}

void XMJGChangePictureProperty::OnBnClickedMJGMoveButtom()
{
	// TODO:  在此添加控件通知处理程序代码
	int idx = m_PictureBrowingInfo.GetSelectedItem();
	int count = m_PictureBrowingInfo.GetItemCount() - 1;
	if(idx == count) return; swapPicturePosition(idx, count);
}

void XMJGChangePictureProperty::OnClickedBrowIngPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int rowNo = pNMListView->iItem;					//行号
	int colNo = pNMListView->iSubItem;				//列号
	if(colNo == 1)
		selectPicture(rowNo);
}

LRESULT XMJGChangePictureProperty::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());
	CRect obWindowRect;
	pInPlaceCombo->GetWindowRect(&obWindowRect);
	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft());
	m_PictureBrowingInfo.GetWindowRect(&obWindowRect);
	int iColIndex = (int)wParam;
	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll();
	if(obWindowRect.PtInRect(obInPlaceComboTopLeft))
	{
		if(iColIndex == 1)
		{
			pComboList->AddTail(_T("1."));//低
			pComboList->AddTail(_T("2."));//低
		}
		if(iColIndex == 2)
		{
			pComboList->AddTail(_T("1."));//低
			pComboList->AddTail(_T("2."));//低
			pComboList->AddTail(_T("3."));//低
			pComboList->AddTail(_T("4."));//低
		}
		if(iColIndex == 3)
		{
			pComboList->AddTail(_T("Num1."));//低
			pComboList->AddTail(_T("Num2."));//低
			pComboList->AddTail(_T("Num3."));//低
			pComboList->AddTail(_T("Num4."));//低
		}
	}
	return true;
}

void XMJGChangePictureProperty::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_PictureBrowingInfo.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		m_records[idx][_T("序号")] = m_PictureBrowingInfo.GetItemText(idx, 0);
		m_records[idx][_T("名称")] = m_PictureBrowingInfo.GetItemText(idx, 1);
		m_records[idx][_T("描述")] = m_PictureBrowingInfo.GetItemText(idx, 2);
		m_records[idx][_T("位置")] = m_propertyname[m_PictureBrowingInfo.GetItemText(idx, 3)];
	}
	IProjectMDB pdb;
	pdb.updataPictureTable(m_records);
	CDialog::OnOK();
}

void XMJGChangePictureProperty::OnBnClickedXmjgAddPixture()
{
	// TODO:  在此添加控件通知处理程序代码
	selectPictures();
}

void XMJGChangePictureProperty::OnBnClickedXMJGDelPicture()
{
	// TODO:  在此添加控件通知处理程序代码
	int nItem = m_PictureBrowingInfo.GetItemCount();
	for(; nItem >= 0; nItem--)
	{
		if(m_PictureBrowingInfo.GetItemState(nItem, LVNI_ALL | LVNI_SELECTED) != LVIS_SELECTED) continue;
		m_PictureBrowingInfo.DeleteItem(nItem);
		VMSIter it = m_records.begin();
		m_records.erase(it + nItem);
	}
}

void XMJG2ExportDlg::OnBnClickedDistance()
{
	// TODO:  在此添加控件通知处理程序代码
	CString text;
	m_EdgeDistance.GetWindowText(text);
	m_SCEdgeDistance.SetWindowText(text);
}

void XMJG2ExportDlg::OnBnClickedBetween()
{
	// TODO:  在此添加控件通知处理程序代码
	CString text;
	m_BuildDistance.GetWindowText(text);
	m_SCBuildDistance.SetWindowText(text);
}

// XMJGFunctionDefine 对话框

IMPLEMENT_DYNAMIC(XMJGFunctionDefine, CDialog)

XMJGFunctionDefine::XMJGFunctionDefine(CWnd* pParent /*=NULL*/)
: CDialog(XMJGFunctionDefine::IDD, pParent)
{

}

XMJGFunctionDefine::~XMJGFunctionDefine()
{
}

void XMJGFunctionDefine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITXMJG_JRXS, m_jrxs);
	DDX_Control(pDX, IDC_EDITXMJG_MJXS, m_mjxs);
	DDX_Control(pDX, IDC_COMBOXMJG_FUNCTIONNAME, m_functionnames);
}

BEGIN_MESSAGE_MAP(XMJGFunctionDefine, CDialog)
	ON_BN_CLICKED(IDC_BUTTONXMJG_ADD, &XMJGFunctionDefine::OnClickedXMJGAdd)
	ON_BN_CLICKED(IDC_BUTTONXMJG_DEL, &XMJGFunctionDefine::OnClickedXMJGDel)
	ON_CBN_SELCHANGE(IDC_COMBOXMJG_FUNCTIONNAME, &XMJGFunctionDefine::OnFunctionChangeName)
END_MESSAGE_MAP()

// XMJGFunctionDefine 消息处理程序

BOOL XMJGFunctionDefine::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	IDataBaseOper oper;
	oper.readFunctionTable(m_fnts);
	int size = (int)m_fnts.size();
	for(int idx = 0; idx < size; ++idx)
		m_functionnames.AddString(m_fnts.at(idx).name);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGFunctionDefine::OnFunctionChangeName()
{
	int nIndex = m_functionnames.GetCurSel(); //当前选中的项
	m_functionnames.SetCurSel(nIndex);
	CString name, _name;
	m_functionnames.GetWindowText(name);
	for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
	{
		FunctionNameTable fnt = m_fnts.at(idx);
		if(fnt.name.CompareNoCase(name) != 0) continue;
		m_mjxs.SetWindowText(fnt.mjxs);
		m_jrxs.SetWindowTextW(fnt.jrxs);
	}
}

void XMJGFunctionDefine::OnClickedXMJGAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	CString name, mjxs, jrxs;
	m_functionnames.GetWindowText(name);
	m_mjxs.GetWindowText(mjxs);
	m_jrxs.GetWindowText(jrxs);
	if(name.GetLength() == 0 || mjxs.GetLength() == 0 || jrxs.GetLength() == 0)
	{
		MessageBox(_T("输出参数有问题，请进行检查"), _T("错误信息"));
		return;
	}
	m_functionnames.AddString(name);
	FunctionNameTable fnt;
	fnt.name = name; fnt.mjxs = mjxs; fnt.jrxs = jrxs;
	m_fnts.push_back(fnt);
}

void XMJGFunctionDefine::OnClickedXMJGDel()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_functionnames.GetCurSel();
	if(sel >= (int)m_fnts.size()) return;
	m_functionnames.DeleteString(sel);
	FNTIter it = m_fnts.begin();
	it = it + sel;
	m_fnts.erase(it);
}

// XMJGChangeLayerInfoDlg 对话框

IMPLEMENT_DYNAMIC(XMJGChangeLayerInfoDlg, CDialog)

XMJGChangeLayerInfoDlg::XMJGChangeLayerInfoDlg(CWnd* pParent /*=NULL*/)
: CDialog(XMJGChangeLayerInfoDlg::IDD, pParent)
{

}

XMJGChangeLayerInfoDlg::~XMJGChangeLayerInfoDlg()
{
}

void XMJGChangeLayerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_XMJGLAYERINFO, m_layerinfo);
	DDX_Control(pDX, IDC_COMBO_XMJGBUILDNAME, m_buildname);
}

BEGIN_MESSAGE_MAP(XMJGChangeLayerInfoDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_XMJGBUILDNAME, &XMJGChangeLayerInfoDlg::OnChangeBuildName)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &XMJGChangeLayerInfoDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()

// XMJGChangeLayerInfoDlg 消息处理程序

BOOL XMJGChangeLayerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_layerinfo.Init();
	m_layerinfo.SetExtendedStyle(m_layerinfo.GetExtendedStyle()
		| LVS_OWNERDRAWFIXED | LVS_EX_DOUBLEBUFFER);
	CRect rect;
	m_layerinfo.GetClientRect(&rect);
	int width = (rect.right - rect.left - 20) / 3;
	m_layerinfo.InsertColumn(0, _T("层    号"), LVCFMT_CENTER, width);
	m_layerinfo.InsertColumn(1, _T("设计高度"), LVCFMT_CENTER, width);
	m_layerinfo.InsertColumn(1, _T("实测高度"), LVCFMT_CENTER, width);
	m_layerinfo.SetColumnReadOnly(0, TRUE);
	initDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGChangeLayerInfoDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	CString buildname;
	m_buildname.GetWindowText(buildname);
	saveLayerInfo(buildname);
	CDialog::OnOK();
}

bool XMJGChangeLayerInfoDlg::initDialog()
{
	SelectFilter sf1(8, _T("分层平面")), sf2(RTDXF0, _T("*polyline"));
	AcDbObjectIdArray ids;
	TCHAR info[255] = {0}, height[20] = {0}, num[20] = {0};
	SelectEntitys(ids, sf1, sf2, _T("X"));
	IProjectMDB pdb; VMStr records = pdb.getCXXTableInfo();
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		CString ldh = record[_T("楼栋号")];
		BuildLayerObjectsIter buildit = m_buildInfo.find(ldh);
		if(buildit == m_buildInfo.end())
			m_buildInfo[ldh] = LayerObjects();
		int layernum = _ttoi(record[_T("楼层号")]);
		m_buildInfo[ldh][layernum].m_slayer = record[_T("起始层名")];
		m_buildInfo[ldh][layernum].m_elayer = record[_T("终止层名")];
		m_buildInfo[ldh][layernum].m_scheight = record[_T("实测层高")];
		m_buildInfo[ldh][layernum].m_ghheight = record[_T("设计层高")];
	}
	initComboBox();
	initListCtrl();
	return true;
}

bool XMJGChangeLayerInfoDlg::initComboBox()
{
	BuildLayerObjectsIter it = m_buildInfo.begin();
	for(; it != m_buildInfo.end(); ++it)
		m_buildname.AddString(it->first);
	m_buildname.SetCurSel(0);
	return true;
}

void XMJGChangeLayerInfoDlg::OnBnClickedButtonDelete()
{
	// TODO:  在此添加控件通知处理程序代码
	int item = m_layerinfo.GetSelectedItem();
	m_layerinfo.DeleteItem(item);
}

bool XMJGChangeLayerInfoDlg::initListCtrl()
{
	CString buildname;	VMStr layerinfo;
	m_buildname.GetWindowText(buildname);
	return setLayerInfo(buildname);
}

bool XMJGChangeLayerInfoDlg::setLayerInfo(const CString &buildname)
{
	m_layerinfo.DeleteAllItems(); CString layername;
	BuildLayerObjectsIter it = m_buildInfo.find(buildname);
	if(it == m_buildInfo.end()) return false;
	LayerObjects layerinfo = it->second; TCHAR lch[200] = {0};
	for(LayerObjectsIter lit = layerinfo.begin(); lit != layerinfo.end(); ++lit)
	{
		LayerObject linfo = lit->second;
		int count = m_layerinfo.GetItemCount();
		CString name = linfo.m_slayer;
		if(linfo.m_elayer.GetLength() != 0)
			name += _T("到") + linfo.m_elayer;
		m_layerinfo.InsertItem(LVIF_TEXT | LVIF_STATE, count, name,
			LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);
		m_layerinfo.SetItemText(count, 1, linfo.m_scheight);
		m_layerinfo.SetItemText(count, 2, linfo.m_ghheight);
	}
	return true;
}

bool XMJGChangeLayerInfoDlg::saveLayerInfo(const CString &buildname)
{
	int count = m_layerinfo.GetItemCount();
	IProjectMDB pdb;
	MStr filter; filter[_T("楼栋号")] = buildname;
	for(int idx = 0; idx < count; ++idx)
	{
		filter[_T("楼层号")].Format(_T("%d"), idx + 1);
		CString height = m_layerinfo.GetItemText(idx, 1);
		double gh = _tstof(m_buildInfo[buildname][idx + 1].m_ghheight);
		double scg = _tstof(height); MStr record;
		record[_T("实测层高")] = height;
		height.Format(_T("%.2lf"), scg - gh);
		record[_T("高度差值")] = height;
		pdb.updataCXXTableInfo(filter, record);
	}
	return true;
}

void XMJGChangeLayerInfoDlg::OnChangeBuildName()
{
	CString name; m_buildname.GetWindowText(name);
	saveLayerInfo(name);
	int nIndex = m_buildname.GetCurSel(); //当前选中的项
	m_buildname.SetCurSel(nIndex);
	m_buildname.GetWindowText(name);
	setLayerInfo(name);
}

// XMJGSubCompressDlg 对话框

IMPLEMENT_DYNAMIC(XMJGSubCompressDlg, CDialog)

XMJGSubCompressDlg::XMJGSubCompressDlg(const TCHAR *name)
: CDialog(XMJGSubCompressDlg::IDD, NULL)
{
	m_iscomplete = true;
	m_name.Format(_T("%s"), name);
}

XMJGSubCompressDlg::XMJGSubCompressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(XMJGSubCompressDlg::IDD, pParent)
{
	m_iscomplete = true;
}

XMJGSubCompressDlg::~XMJGSubCompressDlg()
{
}

void XMJGSubCompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCOMPRESS, m_compressfiles);
}

BEGIN_MESSAGE_MAP(XMJGSubCompressDlg, CDialog)
END_MESSAGE_MAP()

BOOL XMJGSubCompressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initListCtrl();
	setListCtrlData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void XMJGSubCompressDlg::initListCtrl()
{
	m_compressfiles.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_compressfiles.GetClientRect(&rect);
	m_compressfiles.InsertColumn(0, _T("是否压缩"), LVCFMT_CENTER, 80);
	m_compressfiles.InsertColumn(1, _T("文件名称"), LVCFMT_CENTER, rect.Width() - 80);
	m_compressfiles.SetColumnReadOnly(1, TRUE);
	m_compressfiles.SetItemColor(1, RGB(255, 0, 0));
	m_compressfiles.SetCellColor(2, 1, RGB(255, 0, 0));
	UpdateData(FALSE);
}

void XMJGSubCompressDlg::setListCtrlData()
{
	IDataBaseOper oper; MStr record;
	CStringArray filesName;
	CString fileFilter = oper.readCommonTable(_T("过滤文件类型"));
	CFilePathOperater::GetChildFile(m_typePath, m_filesPath, filesName, false);
	int filesCount = filesName.GetSize(); int idx = 0;
	while (idx < filesCount)
	{
		CString fileName = filesName.GetAt(idx);
		int pos = fileName.ReverseFind(_T('.'));
		if (pos == -1)
		{
			m_filesPath.RemoveAt(idx); filesName.RemoveAt(idx);
			filesCount--; continue;
		}
		CString suffix = fileName.Mid(pos + 1);
		if (fileFilter.Find(suffix) != -1)
		{
			m_filesPath.RemoveAt(idx); filesName.RemoveAt(idx);
			filesCount--; continue;
		}
		m_compressfiles.InsertItem(idx, _T(""));
		m_compressfiles.SetItemText(idx, 1, fileName);
		m_compressfiles.SetCheck(idx);
		idx++;
	}
}

void XMJGSubCompressDlg::judgeFileOccupy()
{
	CFile file; CString fileName;
	m_deficiencyfiles.clear();
	int count = m_compressfiles.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL flag = m_compressfiles.GetCheck(idx);
		if(flag == FALSE) continue;
		CString listName = m_compressfiles.GetItemText(idx, 1);
		for(int indx = 0; indx < m_filesPath.GetSize(); ++indx)
		{
			CString path = m_filesPath.GetAt(indx);
			if(path.Find(listName) == -1) continue;
			if(_taccess(path, 0) == -1) continue;
			file.Open(path, CFile::modeReadWrite);
			if(file.m_hFile != CFile::hFileNull)
				file.Close();
			else
				m_deficiencyfiles.push_back(path);
		}
	}
}

CString XMJGSubCompressDlg::getCompressFiles()
{
	CString files;
	int count = m_compressfiles.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL flag = m_compressfiles.GetCheck(idx);
		if(flag == FALSE) continue;
		files += _T(" ") + m_filesPath.GetAt(idx);
	}
	return files;
}

void XMJGSubCompressDlg::setWindowName(const CString &name, const CString &tempPath, const CString &zippath, const CString &codename, const CString &type)
{
	m_name.Format(_T("%s"), name);
	m_tempPath.Format(_T("%s"), tempPath);
	m_zippath.Format(_T("%s"), zippath);
	m_codename.Format(_T("%s"), codename);
	m_type.Format(_T("%s"), type);
	m_projectpath = m_zippath;
	m_typePath = m_projectpath + _T("\\") + m_type;
}

void XMJGSubCompressDlg::OnOK()
{
	// TODO:  在此添加专用代码
	VStr files; m_deficiencyfiles.clear();
	CString m_tempTypePath = m_tempPath + _T("\\") + m_type;
	isExistFolder(m_tempTypePath);
	int filesCount = m_filesPath.GetCount();
	for(int idx = 0; idx < filesCount; ++idx)
	{
		BOOL flag = m_compressfiles.GetCheck(idx);
		if (flag == FALSE) continue;
		CString filePath = m_filesPath.GetAt(idx);
		int pos = filePath.ReverseFind(_T('\\'));
		if (pos == -1)pos = filePath.ReverseFind(_T('/'));
		CString fileName = filePath.Mid(pos + 1);
		flag = CopyFile(filePath, m_tempPath + _T("\\") + m_type + _T("\\") + fileName, FALSE);
		if (flag == FALSE)continue;
		files.push_back(filePath);
	}
	IDataBaseOper oper; VStr values;
	oper.readCompressTable(m_name + _T("_") + m_type, values);
	for(int idx = 0; idx < (int)values.size(); ++idx)
	{
		CString value = values.at(idx);  bool iscomplete = false;
		for(int indx = 0; indx < (int)files.size(); ++indx)
		{
			CString file = files.at(indx);
			int pos = file.Find(value);
			if(pos != -1)
			{
				iscomplete = true;
				break;
			}
		}
		if(iscomplete == false)
			m_deficiencyfiles.push_back(m_type + _T("\\") + value);
	}
	CDialog::OnOK();
}

void XMJGSubCompressDlg::addCompressFile(const CString &file)
{
	m_filesPath.Add(file);
	int count = m_compressfiles.GetItemCount();
	m_compressfiles.InsertItem(count, _T(""));
	int pos = file.ReverseFind(_T('\\'));
	m_compressfiles.SetItemText(count, 1, file.Mid(pos + 1));
	m_compressfiles.SetCheck(count);
}

bool XMJGSubCompressDlg::checkFileIsComplete()
{
	return m_iscomplete;
}

void XMJGSubCompressDlg::deleteFiles()
{
	int count = m_compressfiles.GetItemCount();
	for(int idx = count - 1; idx >= 0; --idx)
	{
		bool flag = m_compressfiles.GetCheck(idx) == 1;
		if(flag == false) continue;
		CString name = m_compressfiles.GetItemText(idx, 1);
		for(int indx = 0; indx < m_filesPath.GetSize(); ++indx)
		{
			CString file = m_filesPath.GetAt(indx);
			if(file.Find(name) == -1) continue;
			m_filesPath.RemoveAt(indx);
		}
		m_compressfiles.DeleteItem(idx);
	}
}

// CRZChangeCarPro 对话框

IMPLEMENT_DYNAMIC(CChangeCarPro, CDialog)

CChangeCarPro::CChangeCarPro(CWnd* pParent /*=NULL*/)
: CDialog(CChangeCarPro::IDD, pParent)
{

}

CChangeCarPro::~CChangeCarPro()
{
}

void CChangeCarPro::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPROCARNUM, m_procarnum);
	DDX_Control(pDX, IDC_RADIO_XMJG_DIXIA, m_dixia);
	DDX_Control(pDX, IDC_COMBOPROCARTYPE, m_procartype);
	DDX_Control(pDX, IDC_RADIO_XMJG_DISHANG, m_dishang);
}

BOOL CChangeCarPro::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_procarnum.EnableWindow(FALSE);
	if(/*m_objects.length() > 1 ||*/ m_objects.length() == 0) return TRUE;
	readDataBase();
	TCHAR info[255] = {0};
	ReadXdata(aname(m_objects.first()), _T("CWLX"), 0, info);
	m_procartype.SetCurSel(m_nameindex[info]); setComboBoxState(info);
	ReadXdata(aname(m_objects.first()), _T("SZLC"), 0, info);
	if(_tcscmp(_T("地上"), info) == 0)
		m_dishang.SetCheck(1);
	else m_dixia.SetCheck(1);
	if(m_objects.length() > 1)
		_stprintf(info, _T("%d"), 1);
	else if(false == ReadXdata(aname(m_objects.first()), _T("CWSM"), 1, info))
		_stprintf(info, _T("%d"), 1);
	m_procarnum.SetWindowText(info);
	return TRUE;
}

bool CChangeCarPro::readDataBase()
{
	IDataBaseOper oper; int idx = 0;
	oper.readCarTable(m_cartypes);
	MSIter it = m_cartypes.begin();
	for(; it != m_cartypes.end(); ++it)
	{
		CString name = it->first;
		m_procartype.InsertString(idx, name);
		m_nameindex[name] = idx++;
	}
	return true;
}

void CChangeCarPro::setComboBoxState(const CString &info)
{
	if(m_cartypes[info] == "1")
		m_procarnum.EnableWindow(FALSE);
	else
		m_procarnum.EnableWindow(TRUE);
}

void CChangeCarPro::setCarNumCtrl(const CString &info)
{

}

void CChangeCarPro::setBarrierCarCtrl(const CString &info)
{
}

void CChangeCarPro::OnCbnSelchangecartype()
{
	CString gettype;
	m_procartype.GetWindowText(gettype);
	setComboBoxState(gettype);
}

BEGIN_MESSAGE_MAP(CChangeCarPro, CDialog)
	ON_BN_CLICKED(IDOK, &CChangeCarPro::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBOPROCARTYPE, &CChangeCarPro::OnCbnSelchangecartype)
END_MESSAGE_MAP()

void CChangeCarPro::setObjects(const AcDbObjectId &id)
{
	m_objects.append(id);
}

void CChangeCarPro::setObjects(const AcDbObjectIdArray &ids)
{
	m_objects.append(ids);
}

void CChangeCarPro::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString type, count;
	m_procartype.GetWindowText(type);
	m_procarnum.GetWindowText(count);
	if(count.GetLength() == 0)
		count.Format(_T("%d"), 1);
	bool flag1 = m_dixia.GetCheck() == 1;
	bool flag2 = m_dishang.GetCheck() == 1;
	CCarInfo info; int len = m_objects.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_objects.at(idx);
		if(flag1 == true) AddXdata(aname(id), _T("SZLC"), 0, _T("地下"));
		if(flag2 == true) AddXdata(aname(id), _T("SZLC"), 0, _T("地上"));
		AddXdata(aname(id), _T("CWLX"), 0, type);
		AddXdata(aname(id), _T("CWSM"), 1, count);
		info.setProperty(id);
	}
	CDialog::OnOK();
}

// CProjectInfoSettingDlg 对话框

IMPLEMENT_DYNAMIC(CProjectBaseInfoSettingDlg, CDialog)

CProjectBaseInfoSettingDlg::CProjectBaseInfoSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CProjectBaseInfoSettingDlg::IDD, pParent)
{
}

CProjectBaseInfoSettingDlg::~CProjectBaseInfoSettingDlg()
{
}

void CProjectBaseInfoSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROJECTDM, m_ProjectDM);
	DDX_Control(pDX, IDC_EDIT_EXPORTTIME, m_exportTime);
	DDX_Control(pDX, IDC_EDITDETAILSURVEY, m_detailsurvey);
	DDX_Control(pDX, IDC_EDITPISPROJECTNAME, m_projectname);
	DDX_Control(pDX, IDC_EDITPISLICENSEDOCS, m_licensedocs);
	DDX_Control(pDX, IDC_DATETIMEPIBAPPLY, m_datatimeapply);
	DDX_Control(pDX, IDC_EDITPISBUILDCOMPANY, m_buildcompany);
	DDX_Control(pDX, IDC_EDITCONDUCECOMPANY, m_conductcompany);
	DDX_Control(pDX, IDC_EDITPISCONTROLPOINT, m_controlpoints);
	DDX_Control(pDX, IDC_EDITPISACCEPTANCECODE, m_acceptancecode);
	DDX_Control(pDX, IDC_EDITPISPROJECTADDRESS, m_projectaddress);
	DDX_Control(pDX, IDC_EDITCONTROLBASEPOINT, m_controlbasepoint);
	DDX_Control(pDX, IDC_EDITPISMEANSURECOMPANY, m_meansurecompany);
	DDX_Control(pDX, IDC_EDITMEASUREREPORTMEASURE, m_measureReport);
	DDX_Control(pDX, IDC_EDITBUILDAREAMEASURE, m_buildAreaMeasure);
	DDX_Control(pDX, IDC_EDITMEASUREMETHOD, m_measureMethod);
}

BOOL CProjectBaseInfoSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	CWnd *wnd = GetFocus();
	if(wnd != GetDlgItem(IDC_EDITPISLICENSEDOCS) ||
		wnd != GetDlgItem(IDC_EDITCONTROLBASEPOINT) ||
		wnd != GetDlgItem(IDC_EDITDETAILSURVEY) ||
		wnd != GetDlgItem(IDC_EDITPISCONTROLPOINT) ||
		wnd != GetDlgItem(IDC_EDITPISMEANSURECOMPANY) ||
		wnd != GetDlgItem(IDC_EDITPISBUILDCOMPANY) ||
		wnd != GetDlgItem(IDC_EDITPISPROJECTNAME) ||
		wnd != GetDlgItem(IDC_DATETIMEPIBAPPLY) ||
		wnd != GetDlgItem(IDC_EDITCONDUCECOMPANY) ||
		wnd != GetDlgItem(IDC_EDITPISACCEPTANCECODE) ||
		wnd != GetDlgItem(IDC_EDITPISPROJECTADDRESS))
		return CDialog::PreTranslateMessage(pMsg);
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CProjectBaseInfoSettingDlg, CDialog)
END_MESSAGE_MAP()

// CProjectInfoSettingDlg 消息处理程序

void CProjectBaseInfoSettingDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

CTime getTimeByString(const CString &time)
{
	int pos1 = time.FindOneOf(_T("/"));
	pos1 = pos1 == -1 ? time.FindOneOf(_T("-")) : pos1;
	int year = _ttoi(time.Mid(0, pos1));
	int pos2 = time.ReverseFind(_T('/'));
	pos2 = pos2 == -1 ? time.ReverseFind(_T('-')) : pos2;
	int day = _ttoi(time.Mid(pos2 + 1));
	int mouth = _ttoi(time.Mid(pos1 + 1, (pos2 - pos1)));
	return CTime(year, mouth, day, 0, 0, 0);
}

BOOL CProjectBaseInfoSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	IProjectMDB pdb; MStr jbxx, jssm;
	jbxx = pdb.getJBXXTableInfo();
	jssm = pdb.getJSSMTableInfo();
	if(!jbxx[_T("报告时间")].IsEmpty())
		m_exportTime.SetTime(&getTimeByString(jbxx[_T("报告时间")]));
	if(!jbxx[_T("审批时间")].IsEmpty())
		m_datatimeapply.SetTime(&getTimeByString(jbxx[_T("审批时间")]));
	m_ProjectDM.SetWindowText(jbxx[_T("项目代码")]);
	m_projectname.SetWindowText(jbxx[_T("项目名称")]);
	m_acceptancecode.SetWindowText(jbxx[_T("受理号")]);
	m_buildcompany.SetWindowText(jbxx[_T("建设单位")]);
	m_projectaddress.SetWindowText(jbxx[_T("项目地点")]);
	m_conductcompany.SetWindowText(jbxx[_T("组织单位")]);
	m_meansurecompany.SetWindowText(jbxx[_T("测量单位")]);
	m_detailsurvey.SetWindowText(jssm[_T("细部测量基本情况")]);
	m_controlbasepoint.SetWindowText(jssm[_T("图根控制情况")]);
	m_controlpoints.SetWindowText(jssm[_T("起算控制点基本情况")]);
	m_licensedocs.SetWindowText(jssm[_T("测绘成果依据的许可文书")]);
	m_measureReport.SetWindowText(jssm[_T("本册测量报告书的测量次数情况")]);
	m_buildAreaMeasure.SetWindowText(jssm[_T("建筑面积测量")]);
	m_measureMethod.SetWindowText(jssm[_T("测量方法")]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CProjectBaseInfoSettingDlg::OnOK()
{
	// TODO:  在此添加控件通知处理程序代码
	MStr jbxx, jssm; IProjectMDB pdb;
	m_ProjectDM.GetWindowText(jbxx[_T("项目代码")]);
	m_exportTime.GetWindowText(jbxx[_T("报告时间")]);
	m_projectname.GetWindowText(jbxx[_T("项目名称")]);
	m_acceptancecode.GetWindowText(jbxx[_T("受理号")]);
	m_buildcompany.GetWindowText(jbxx[_T("建设单位")]);
	m_datatimeapply.GetWindowText(jbxx[_T("审批时间")]);
	m_projectaddress.GetWindowText(jbxx[_T("项目地点")]);
	m_conductcompany.GetWindowText(jbxx[_T("组织单位")]);
	m_meansurecompany.GetWindowText(jbxx[_T("测量单位")]);
	m_detailsurvey.GetWindowText(jssm[_T("细部测量基本情况")]);
	m_controlbasepoint.GetWindowText(jssm[_T("图根控制情况")]);
	m_controlpoints.GetWindowText(jssm[_T("起算控制点基本情况")]);
	m_licensedocs.GetWindowText(jssm[_T("测绘成果依据的许可文书")]);
	m_measureReport.GetWindowText(jssm[_T("本册测量报告书的测量次数情况")]);
	m_buildAreaMeasure.GetWindowText(jssm[_T("建筑面积测量")]);
	m_measureMethod.GetWindowText(jssm[_T("测量方法")]);
	pdb.setJBXXTableInfo(jbxx); pdb.setJSSMTableInfo(jssm);
	CDialog::OnOK();
}

IMPLEMENT_DYNAMIC(CDeficiencyFilesDlg, CDialog)

CDeficiencyFilesDlg::CDeficiencyFilesDlg(const CString &title, CWnd* pParent /*=NULL*/)
: CDialog(CDeficiencyFilesDlg::IDD, pParent), m_title(title)
{

}

CDeficiencyFilesDlg::~CDeficiencyFilesDlg()
{

}

void CDeficiencyFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEFICIENCYFILES, m_deficiencylist);
}

BEGIN_MESSAGE_MAP(CDeficiencyFilesDlg, CDialog)
END_MESSAGE_MAP()

// CDeficiencyFilesDlg 消息处理程序

bool CDeficiencyFilesDlg::isDeficiencyFile()
{
	return m_files.size() != 0;
}

void CDeficiencyFilesDlg::addDeficiencyFile(const VStr &files)
{
	for(int idx = 0; idx < (int)files.size(); ++idx)
		m_files.push_back(files.at(idx));
}

BOOL CDeficiencyFilesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_deficiencylist.Init();
	CRect rect; CString fileName;
	m_deficiencylist.GetClientRect(&rect);
	// TODO:  在此添加额外的初始化
	m_deficiencylist.InsertColumn(0, m_title, LVCFMT_CENTER, rect.Width());
	for(int idx = 0; idx < (int)m_files.size(); ++idx)
	{
		CString file = m_files.at(idx);
		m_deficiencylist.InsertItem(idx, file);
		// 		int pos = file.ReverseFind(_T('\\'));
		// 		if(pos == -1) pos = file.ReverseFind(_T('/'));
		// 		fileName = file.Mid(pos); file = file.Mid(0, pos - 1);
		// 		pos = file.ReverseFind(_T('\\'));
		// 		if(pos == -1) pos = file.ReverseFind(_T('/'));
		// 		file = file.Mid(pos) + fileName;
		// 		m_deficiencylist.InsertItem(idx, file);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDeficiencyFilesDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	CDialog::OnOK();
}

// CBaseProjectInfoDlg 对话框

IMPLEMENT_DYNAMIC(CProjectExportDocSettingDlg, CDialog)

CProjectExportDocSettingDlg::CProjectExportDocSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CProjectExportDocSettingDlg::IDD, pParent)
{

}

CProjectExportDocSettingDlg::~CProjectExportDocSettingDlg()
{

}

void CProjectExportDocSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_XMJGGHJZJJ, m_ghjzjj);
	DDX_Control(pDX, IDC_EDIT_XMJSCGJZJJ, m_scgjzjj);
	DDX_Control(pDX, IDC_EDIT_XMJGGHJZLMYQ, m_jzlmyq);
	DDX_Control(pDX, IDC_EDIT_XMJGGHTHXJJ, m_ghthxjj);
	DDX_Control(pDX, IDC_EDIT_XMJGSCTHXJJ, m_scthxjj);
	DDX_Control(pDX, IDC_EDIT_XMJSCJZLMYQ, m_scjzlmyq);
	DDX_Control(pDX, IDC_EDIT_XMJSCQTGHYQ, m_scqtghyq);
	DDX_Control(pDX, IDC_EDIT_XMJGGHQTGHYQ, m_ghqtghyq);
	DDX_Control(pDX, IDC_EDIT_XMJGGHGGJZPT, m_ghggjzpt);
	DDX_Control(pDX, IDC_EDIT_XMJGSCGGJZPT, m_scggjzpt);
	DDX_Control(pDX, IDC_EDIT_XMJG_QTXYSMDQK, m_qtxysmdqk);
	DDX_Control(pDX, IDC_RADIO_XMJG_LSSSSFCC1, m_lssssfcc1);
	DDX_Control(pDX, IDC_RADIO_XMJG_LSSSSFCC2, m_lssssfcc2);
	DDX_Control(pDX, IDC_EDIT_XMJG_YGHXKBYZQK, m_yghxkbyzqk);
}

BOOL CProjectExportDocSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	CWnd *wnd = GetFocus();
	if(wnd != GetDlgItem(IDC_EDIT_XMJGGHJZJJ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJSCGJZJJ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJGGHJZLMYQ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJGGHTHXJJ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJGSCTHXJJ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJSCJZLMYQ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJSCQTGHYQ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJGGHQTGHYQ) ||
		wnd != GetDlgItem(IDC_EDIT_XMJGGHGGJZPT) ||
		wnd != GetDlgItem(IDC_EDIT_XMJGSCGGJZPT) ||
		wnd != GetDlgItem(IDC_EDIT_XMJG_QTXYSMDQK) ||
		wnd != GetDlgItem(IDC_EDIT_XMJG_YGHXKBYZQK))
		return CDialog::PreTranslateMessage(pMsg);

	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CProjectExportDocSettingDlg, CDialog)
END_MESSAGE_MAP()

// CBaseProjectInfoDlg 消息处理程序

BOOL CProjectExportDocSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	IProjectMDB pdb; MStr bgxx = pdb.getBGXXTableInfo();
	XMJGAssist assist; CString binfo, rinfo;
	assist.GetBuildDistanceInfo(binfo);
	assist.GetEdgeDistanceInfo(rinfo);
	if(binfo.GetLength() != 0) bgxx[_T("实测建筑间距")] = binfo;
	if(rinfo.GetLength() != 0) bgxx[_T("实测退红线间距")] = rinfo;
	m_ghjzjj.SetWindowText(bgxx[_T("规划建筑间距")]);
	m_scgjzjj.SetWindowText(bgxx[_T("实测建筑间距")]);
	m_ghthxjj.SetWindowText(bgxx[_T("规划退红线间距")]);
	m_scthxjj.SetWindowText(bgxx[_T("实测退红线间距")]);
	m_jzlmyq.SetWindowText(bgxx[_T("规划建筑立面要求")]);
	m_ghqtghyq.SetWindowText(bgxx[_T("规划其他规划要求")]);
	m_ghggjzpt.SetWindowText(bgxx[_T("规划公共建筑配套")]);
	m_scjzlmyq.SetWindowText(bgxx[_T("实测建筑立面要求")]);
	m_scqtghyq.SetWindowText(bgxx[_T("实测其他规划要求")]);
	m_scggjzpt.SetWindowText(bgxx[_T("实测公共建筑配套")]);
	m_qtxysmdqk.SetWindowText(bgxx[_T("其他需要说明的情况")]);
	if(_ttoi(bgxx[_T("临时设施是否拆除")]) == 1) m_lssssfcc1.SetCheck(1);
	if(_ttoi(bgxx[_T("临时设施是否拆除")]) == 2) m_lssssfcc2.SetCheck(1);
	m_yghxkbyzqk.SetWindowText(bgxx[_T("与规划许可不一致情况")]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CProjectExportDocSettingDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	IProjectMDB pdb; MStr bgxx;
	m_ghjzjj.GetWindowText(bgxx[_T("规划建筑间距")]);
	m_scgjzjj.GetWindowText(bgxx[_T("实测建筑间距")]);
	m_ghthxjj.GetWindowText(bgxx[_T("规划退红线间距")]);
	m_scthxjj.GetWindowText(bgxx[_T("实测退红线间距")]);
	m_jzlmyq.GetWindowText(bgxx[_T("规划建筑立面要求")]);
	m_ghqtghyq.GetWindowText(bgxx[_T("规划其他规划要求")]);
	m_ghggjzpt.GetWindowText(bgxx[_T("规划公共建筑配套")]);
	m_scjzlmyq.GetWindowText(bgxx[_T("实测建筑立面要求")]);
	m_scqtghyq.GetWindowText(bgxx[_T("实测其他规划要求")]);
	m_scggjzpt.GetWindowText(bgxx[_T("实测公共建筑配套")]);
	if(m_lssssfcc1.GetCheck() == 1) bgxx[_T("临时设施是否拆除")] = _T("1");
	if(m_lssssfcc2.GetCheck() == 1) bgxx[_T("临时设施是否拆除")] = _T("2");
	m_qtxysmdqk.GetWindowText(bgxx[_T("其他需要说明的情况")]);
	m_yghxkbyzqk.GetWindowText(bgxx[_T("与规划许可不一致情况")]);
	pdb.setBGXXTableInfo(bgxx);
	CDialog::OnOK();
}

void CProjectExportDocSettingDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

// CProjectAreaSettingDlg 对话框

IMPLEMENT_DYNAMIC(CProjectAreaSettingDlg, CDialog)

CProjectAreaSettingDlg::CProjectAreaSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CProjectAreaSettingDlg::IDD, pParent)
{

}

CProjectAreaSettingDlg::~CProjectAreaSettingDlg()
{

}

void CProjectAreaSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ASDSM, m_sm);
	DDX_Control(pDX, IDC_EDIT_ASDDSCW, m_dscw);
	DDX_Control(pDX, IDC_EDIT_ASDDXCW, m_dxcw);
	DDX_Control(pDX, IDC_EDIT_ASDJZMD, m_jzmd);
	DDX_Control(pDX, IDC_EDIT_ASDJZYDMJ, m_jzydmj);
	DDX_Control(pDX, IDC_EDIT_ASDGHDSMJ, m_ghdsmj);
	DDX_Control(pDX, IDC_EDIT_ASDGHDXMJ, m_ghdxmj);
	DDX_Control(pDX, IDC_EDIT_ADSGHJRMJ, m_ghjrmj);
	DDX_Control(pDX, IDC_EDIT_ASDGHZJZMJ, m_ghzjzmj);
	DDX_Control(pDX, IDC_EDIT_ASDSCDSMJSM, m_scdsmjsm);
	DDX_Control(pDX, IDC_EDIT_ASDSCDXMJSM, m_scdxmjsm);
	DDX_Control(pDX, IDC_EDIT_ASDGHDSMJSM, m_ghdsmjsm);
	DDX_Control(pDX, IDC_EDIT_ASDGHDXMJSM, m_ghdxmjsm);
	DDX_Control(pDX, IDC_EDIT_ADSGHJRMJSM, m_ghjrmjsm);
	DDX_Control(pDX, IDC_EDIT_ADSSCJRMJSM, m_scjrmjsm);
	DDX_Control(pDX, IDC_EDIT_ASDSCZYCRKWZ, m_sczycrkwz);
	DDX_Control(pDX, IDC_EDIT_ASDGHZJZMJSM, m_ghzjzmjsm);
	DDX_Control(pDX, IDC_EDIT_ASDGHZYCRKWZ, m_ghzycrkwz);
	DDX_Control(pDX, IDC_EDIT_ASDSCZJZMJSM, m_sczjzmjsm);
	DDX_Control(pDX, IDC_EDIT_ASDSCJZYDMJSM, m_scjzydsm);
}

BOOL CProjectAreaSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	CWnd *wnd = GetFocus();
	if(wnd != GetDlgItem(IDC_EDIT_ASDGHZJZMJSM) ||
		wnd != GetDlgItem(IDC_EDIT_ASDSCZJZMJSM) ||
		wnd != GetDlgItem(IDC_EDIT_ASDGHDSMJSM) ||
		wnd != GetDlgItem(IDC_EDIT_ASDSCDSMJSM) ||
		wnd != GetDlgItem(IDC_EDIT_ASDGHDXMJSM) ||
		wnd != GetDlgItem(IDC_EDIT_ASDSCDXMJSM) ||
		wnd != GetDlgItem(IDC_EDIT_ADSGHJRMJSM) ||
		wnd != GetDlgItem(IDC_EDIT_ADSSCJRMJSM))
		return CDialog::PreTranslateMessage(pMsg);
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CProjectAreaSettingDlg, CDialog)
END_MESSAGE_MAP()

// CExportDocInfoDlg 消息处理程序

BOOL CProjectAreaSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	IProjectMDB pdb; MStr mjxx = pdb.getMJXXTableInfo();
	m_sm.SetWindowText(mjxx[_T("说明")]);
	m_dscw.SetWindowText(mjxx[_T("规划地上车位")]);
	m_dxcw.SetWindowText(mjxx[_T("规划地下车位")]);
	m_jzmd.SetWindowText(mjxx[_T("建筑密度")]);
	m_jzydmj.SetWindowText(mjxx[_T("建筑用地面积")]);
	m_ghdsmj.SetWindowText(mjxx[_T("规划地上面积")]);
	m_ghdxmj.SetWindowText(mjxx[_T("规划地下面积")]);
	m_ghjrmj.SetWindowText(mjxx[_T("规划计容面积")]);
	m_ghzjzmj.SetWindowText(mjxx[_T("规划总建筑面积")]);
	m_scdsmjsm.SetWindowText(mjxx[_T("实测地上面积说明")]);
	m_scdxmjsm.SetWindowText(mjxx[_T("实测地下面积说明")]);
	m_scjrmjsm.SetWindowText(mjxx[_T("实测计容面积说明")]);
	m_ghdsmjsm.SetWindowText(mjxx[_T("规划地上面积说明")]);
	m_ghdxmjsm.SetWindowText(mjxx[_T("规划地下面积说明")]);
	m_ghjrmjsm.SetWindowText(mjxx[_T("规划计容面积说明")]);
	m_ghzjzmjsm.SetWindowText(mjxx[_T("规划总建筑面积说明")]);
	m_ghzycrkwz.SetWindowText(mjxx[_T("规划主要出入口位置")]);
	m_sczjzmjsm.SetWindowText(mjxx[_T("实测总建筑面积说明")]);
	m_sczycrkwz.SetWindowText(mjxx[_T("实测主要出入口位置")]);
	//m_ghjzydsm.SetWindowText(mjxx[_T("规划建筑用地面积说明")]);
	m_scjzydsm.SetWindowText(mjxx[_T("实测建筑用地面积说明")]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CProjectAreaSettingDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	MStr pinfo;
	m_sm.GetWindowText(pinfo[_T("说明")]);
	m_dscw.GetWindowText(pinfo[_T("规划地上车位")]);
	m_dxcw.GetWindowText(pinfo[_T("规划地下车位")]);
	m_jzmd.GetWindowText(pinfo[_T("建筑密度")]);
	m_jzydmj.GetWindowText(pinfo[_T("建筑用地面积")]);
	m_ghdsmj.GetWindowText(pinfo[_T("规划地上面积")]);
	m_ghdxmj.GetWindowText(pinfo[_T("规划地下面积")]);
	m_ghjrmj.GetWindowText(pinfo[_T("规划计容面积")]);
	m_ghzjzmj.GetWindowText(pinfo[_T("规划总建筑面积")]);
	m_scdsmjsm.GetWindowText(pinfo[_T("实测地上面积说明")]);
	m_scdxmjsm.GetWindowText(pinfo[_T("实测地下面积说明")]);
	m_scjrmjsm.GetWindowText(pinfo[_T("实测计容面积说明")]);
	m_ghdsmjsm.GetWindowText(pinfo[_T("规划地上面积说明")]);
	m_ghdxmjsm.GetWindowText(pinfo[_T("规划地下面积说明")]);
	m_ghjrmjsm.GetWindowText(pinfo[_T("规划计容面积说明")]);
	m_ghzjzmjsm.GetWindowText(pinfo[_T("规划总建筑面积说明")]);
	m_ghzycrkwz.GetWindowText(pinfo[_T("规划主要出入口位置")]);
	m_sczjzmjsm.GetWindowText(pinfo[_T("实测总建筑面积说明")]);
	m_sczycrkwz.GetWindowText(pinfo[_T("实测主要出入口位置")]);
	//m_ghjzydsm.GetWindowText(pinfo[_T("规划建筑用地面积说明")]);
	m_scjzydsm.GetWindowText(pinfo[_T("实测建筑用地面积说明")]);
	IProjectMDB pdb; pdb.setMJXXTableInfo(pinfo);
	CDialog::OnOK();
}

void CProjectAreaSettingDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

// CExportDocInfoDlg 对话框

IMPLEMENT_DYNAMIC(CExportDocInfoDlg, CDialog)

CExportDocInfoDlg::CExportDocInfoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CExportDocInfoDlg::IDD, pParent)
{

}

CExportDocInfoDlg::~CExportDocInfoDlg()
{

}

void CExportDocInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_EXPORTDOC, m_tab);
}

BEGIN_MESSAGE_MAP(CExportDocInfoDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EXPORTDOC, &CExportDocInfoDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()

// CExportDocInfoDlg 消息处理程序


BOOL CExportDocInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_tab.InsertItem(0, _T("竣工测量基本信息"));
	m_tab.InsertItem(1, _T("竣工规划信息设置"));
	m_tab.InsertItem(2, _T("竣工输出报告信息"));
	CAcModuleResourceOverride myResources0;//声明该对象用来解决资源冲突问题
	m_pasdlg.Create(IDD_DIALOG_XMJG_PROJECTAREASETTINGDLG, &m_tab);
	CAcModuleResourceOverride myResources1;//声明该对象用来解决资源冲突问题
	m_pbisdlg.Create(IDD_DIALOG_XMJG_PROJECTBASEINFOSETTINGDLG, &m_tab);
	CAcModuleResourceOverride myResources2;//声明该对象用来解决资源冲突问题
	m_pedsdlg.Create(IDD_DIALOG_XMJG_PROJECTEXPORTDOCSETTINGDLG, &m_tab);
	CRect rect; m_tab.GetClientRect(&rect);
	rect.top += 20; rect.bottom += 20; rect.right += 40;
	m_pasdlg.MoveWindow(&rect);
	m_pbisdlg.MoveWindow(&rect);
	m_pedsdlg.MoveWindow(&rect);
	m_pbisdlg.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	return TRUE;
	// return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CExportDocInfoDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_pbisdlg.ShowWindow(TRUE);
			m_pasdlg.ShowWindow(FALSE);
			m_pedsdlg.ShowWindow(FALSE);
			break;
		case 1:
			m_pbisdlg.ShowWindow(FALSE);
			m_pasdlg.ShowWindow(TRUE);
			m_pedsdlg.ShowWindow(FALSE);
			break;
		case 2:
			m_pbisdlg.ShowWindow(FALSE);
			m_pasdlg.ShowWindow(FALSE);
			m_pedsdlg.ShowWindow(TRUE);
			break;
		default:;
	}
	*pResult = 0;
}

void CExportDocInfoDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	m_pbisdlg.OnOK();
	m_pasdlg.OnOK();
	m_pedsdlg.OnOK();
	CDialog::OnOK();
}

void CExportDocInfoDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类
	m_pasdlg.OnCancel();
	m_pasdlg.OnCancel();
	m_pedsdlg.OnCancel();
	CDialog::OnCancel();
}

// CCompressDlg 对话框

IMPLEMENT_DYNAMIC(CCompressDlg, CDialog)

CCompressDlg::CCompressDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCompressDlg::IDD, pParent)
{
	IDataBaseOper oper;
	m_path = oper.readPathTable(_T("当前目录"));
	int pos = m_path.ReverseFind(_T('\\'));
	m_codename = m_path.Mid(pos + 1);
	m_zippath = m_path;
}

CCompressDlg::CCompressDlg(const CString &name)
	: CDialog(CCompressDlg::IDD, NULL), m_name(name)
{
	IDataBaseOper oper;
	m_path = oper.readPathTable(_T("当前目录"));
	m_zippath = m_path;
	int pos = m_path.ReverseFind(_T('\\'));
	m_tempPath = m_path.Mid(0, pos) + _T("\\temp");
	m_codename = m_path.Mid(pos + 1);
	//_trmdir(m_tempPath);//删除空目录
	//CFilePathOperater::SetHidden(m_tempPath + _T("\\temp"), true);
}

CCompressDlg::~CCompressDlg()
{
}

void CCompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_XMJGCOMPRESSCODE, m_compresscode);
	DDX_Control(pDX, IDC_EDIT_XMJGCOMPRESSNAME, m_compressname);
	DDX_Control(pDX, IDC_TAB_XMJGCOMPRESSTAB, m_tab);
}

BEGIN_MESSAGE_MAP(CCompressDlg, CDialog)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_XMJGCOMPRESSTAB, &CCompressDlg::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESSALLSELECT, &CCompressDlg::OnBnClickedButtonCompressallselect)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESSREVERSE, &CCompressDlg::OnBnClickedButtonCompressreverse)
	ON_BN_CLICKED(IDC_BUTTON_COMPRESSADDFILE, &CCompressDlg::OnBnClickedButtonCompressaddfile)
END_MESSAGE_MAP()

BOOL CCompressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int pos = m_codename.ReverseFind(_T('_'));
	m_compresscode.SetWindowText(m_codename.Mid(0, pos));
	m_compressname.SetWindowText(m_codename.Mid(pos + 1));
	m_tab.InsertItem(0, _T("参考数据"));
	m_tab.InsertItem(1, _T("过程数据"));
	m_tab.InsertItem(2, _T("成果数据"));
	CAcModuleResourceOverride myResource1;
	m_subck.setWindowName(m_name, m_tempPath, m_zippath, m_codename, _T("CK")); m_subck.Create(IDD_DIALOG_SUBCOMPRESSDLG, &m_tab);
	CAcModuleResourceOverride myResource2;
	m_subgc.setWindowName(m_name, m_tempPath, m_zippath, m_codename, _T("GC")); m_subgc.Create(IDD_DIALOG_SUBCOMPRESSDLG, &m_tab);
	CAcModuleResourceOverride myResource3;
	m_subcg.setWindowName(m_name, m_tempPath, m_zippath, m_codename, _T("CG")); m_subcg.Create(IDD_DIALOG_SUBCOMPRESSDLG, &m_tab);
	CRect rect; m_tab.GetClientRect(&rect);
	rect.top += 20; 
	//rect.bottom += 20; rect.right += 40;
	m_subck.MoveWindow(&rect);
	m_subgc.MoveWindow(&rect);
	m_subcg.MoveWindow(&rect);
	m_subck.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CCompressDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	m_subcg.OnOK(); m_subgc.OnOK(); m_subck.OnOK();
	CDeficiencyFilesDlg dlg1(_T("缺少下列文件")); 
	dlg1.addDeficiencyFile(m_subgc.m_deficiencyfiles); dlg1.addDeficiencyFile(m_subck.m_deficiencyfiles); dlg1.addDeficiencyFile(m_subcg.m_deficiencyfiles);
	if(dlg1.isDeficiencyFile() == true) { dlg1.DoModal(); return; }
	//SYSTEMTIME st = {0}; GetLocalTime(&st);
	//CString time; time.Format(_T("%d%02d%02d"), st.wYear, st.wMonth, st.wDay);
	CString name = m_codename + _T(".zip");
	ZRESULT zr = ZipUtils::CompressDirToZip(m_tempPath, name, m_zippath);
	MessageBox(zr == 0 ? _T("数据打包完成") : _T("打包失败！！！"), _T("信息提示"));
	CDialog::OnOK();
}

void deleteFile(const CString &path)
{
	HANDLE file;
	WIN32_FIND_DATA fileData;
	file = FindFirstFile(path, &fileData);
	FindNextFile(file, &fileData);
	while(FindNextFile(file, &fileData))
	{
		// 如果是一个文件目录
		if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			deleteFile(path + _T("\\") + fileData.cFileName);
		else
			DeleteFile(path + _T("\\") + fileData.cFileName);
	}
}

void CCompressDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类	
	deleteFile(m_tempPath + _T("\\temp"));
	CDialog::OnCancel();
}

void CCompressDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值	
	deleteFile(m_tempPath + _T("\\temp"));
	CDialog::OnClose();
}

void CCompressDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel;
	CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_subck.ShowWindow(TRUE);
			m_subgc.ShowWindow(FALSE);
			m_subcg.ShowWindow(FALSE);
			break;
		case 1:
			m_subck.ShowWindow(FALSE);
			m_subgc.ShowWindow(TRUE);
			m_subcg.ShowWindow(FALSE);
			break;
		case 2:
			m_subck.ShowWindow(FALSE);
			m_subgc.ShowWindow(FALSE);
			m_subcg.ShowWindow(TRUE);
			break;
		default:;
	}
	*pResult = 0;
}

void CCompressDlg::OnBnClickedButtonCompressallselect()
{
	// TODO:  在此添加控件通知处理程序代码
	int cursel = m_tab.GetCurSel();
	if(cursel == 0)
		selectAllFile(m_subck);
	else if(cursel == 1)
		selectAllFile(m_subgc);
	else if(cursel == 2)
		selectAllFile(m_subcg);
}

void CCompressDlg::OnBnClickedButtonCompressreverse()
{
	// TODO:  在此添加控件通知处理程序代码
	int cursel = m_tab.GetCurSel();
	if(cursel == 0)
		selectReverseFile(m_subck);
	else if(cursel == 1)
		selectReverseFile(m_subgc);
	else if(cursel == 2)
		selectReverseFile(m_subcg);
}

void CCompressDlg::OnBnClickedButtonCompressaddfile()
{
	// TODO:  在此添加控件通知处理程序代码
	int cursel = m_tab.GetCurSel();
	if(cursel == 0)
		selectFiles(m_subck, _T("CK"));
	else if(cursel == 1)
		selectFiles(m_subgc, _T("GC"));
	else if(cursel == 2)
		selectFiles(m_subcg, _T("CG"));
}

void CCompressDlg::selectFiles(XMJGSubCompressDlg &cursel, const CString &type)
{
	CString fileExtensions = "所有文件(*.*) |*.*||";
	CFileDialog fileDlg(TRUE, NULL, NULL,
		OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY,
		fileExtensions);
	const int MIN_FILE_NUMBER = 1;                                                           //至少允许选择10个文件
	fileDlg.m_ofn.lpstrFile = new TCHAR[_MAX_PATH * MIN_FILE_NUMBER]; //重新定义缓冲区大小          
	memset(fileDlg.m_ofn.lpstrFile, 0, _MAX_PATH * MIN_FILE_NUMBER);  //初始化定义的缓冲区
	fileDlg.m_ofn.nMaxFile = _MAX_PATH * MIN_FILE_NUMBER;
	if(IDOK == fileDlg.DoModal())
	{
		CString fileName, name, typepath(m_path + _T("\\"));
		POSITION pos = fileDlg.GetStartPosition();
		while(NULL != pos)
		{
			fileName = fileDlg.GetNextPathName(pos);
			CString path(m_zippath + _T("\\") + type);
			int pos = fileName.ReverseFind(_T('\\'));
			pos = pos == -1 ? fileName.ReverseFind(_T('/')) : pos;
			name = fileName.Mid(pos);
			CopyFile(fileName, typepath + type + name, FALSE);
			cursel.addCompressFile(fileName);
		}
	}
	delete[] fileDlg.m_ofn.lpstrFile;     //最后别忘了释放内存
	return;
}

void CCompressDlg::selectAllFile(XMJGSubCompressDlg &cursel)
{
	int count = cursel.m_compressfiles.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		bool flag = cursel.m_compressfiles.GetCheck(idx) == 1;
		if(flag == false)
			cursel.m_compressfiles.SetCheck(idx, 1);
	}
}

void CCompressDlg::selectReverseFile(XMJGSubCompressDlg &cursel)
{
	int count = cursel.m_compressfiles.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		bool flag = cursel.m_compressfiles.GetCheck(idx) == 1;
		if(flag == false)
			cursel.m_compressfiles.SetCheck(idx, 1);
		else
			cursel.m_compressfiles.SetCheck(idx, 0);
	}
}

// CSetLayerInfoDlg 对话框

IMPLEMENT_DYNAMIC(CSetLayerInfoDlg, CDialog)

CSetLayerInfoDlg::CSetLayerInfoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSetLayerInfoDlg::IDD, pParent)
{

}

CSetLayerInfoDlg::~CSetLayerInfoDlg()
{

}

void CSetLayerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SLI_JZMJ, m_jzmj);
	DDX_Control(pDX, IDC_EDIT_SLI_JRMJ, m_jrmj);
	DDX_Control(pDX, IDC_EDIT_SLI_ANGLE, m_angle);
	DDX_Control(pDX, IDC_EDIT_SLI_TJHEIGHT, m_tjheight);
	DDX_Control(pDX, IDC_EDIT_SLI_WDHEIGHT, m_wdheight);
	DDX_Control(pDX, IDC_COMBO_TJLOCATION, m_tjlocation);
	DDX_Control(pDX, IDC_EDIT_SLI_BUILDNAME, m_buildname);
	DDX_Control(pDX, IDC_EDIT_SLI_LAYERNAME, m_layername);
	DDX_Control(pDX, IDC_EDIT_SLI_SCCONTEXT, m_sccontext);
	DDX_Control(pDX, IDC_EDIT_SLI_SJCONTEXT, m_sjcontext);
	DDX_Control(pDX, IDC_EDIT_SLI_LAYERBEIZHU, m_layerbz);
	DDX_Control(pDX, IDC_CHECK_SLI_XIEWUMIAN, m_xiewumian);
	DDX_Control(pDX, IDC_CHECK_SLI_PINGWUMIAN, m_pingwumian);
	DDX_Control(pDX, IDC_EDIT_SLI_CWALLHEIGHT, m_cwallheight);
	DDX_Control(pDX, IDC_EDIT_SLI_NVWALLHEIGHT, m_nvwallheight);
	DDX_Control(pDX, IDC_EDIT_SLI_SJLAYERHEIGHT, m_sjlayerheight);
	DDX_Control(pDX, IDC_EDIT_SLI_SCLAYERHEIGHT, m_sclayerheight);
}

BEGIN_MESSAGE_MAP(CSetLayerInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_SLI_PINGWUMIAN, &CSetLayerInfoDlg::OnBnClickedCheckSliPingwumian)
	ON_BN_CLICKED(IDC_CHECK_SLI_XIEWUMIAN, &CSetLayerInfoDlg::OnBnClickedCheckSliXiewumian)
END_MESSAGE_MAP()

// CSetLayerInfoDlg 消息处理程序

BOOL CSetLayerInfoDlg::OnInitDialog()
{
	// TODO:  在此添加额外的初始化
	CDialog::OnInitDialog(); TCHAR info[255] = {0}; m_filter.clear();	
	ReadXdata(aname(m_obj), _T("楼栋号"), 0, info); m_ldname = info;
	m_buildname.SetWindowText(info); m_filter[_T("楼栋号")] = m_ldname;
	ReadXdata(aname(m_obj), _T("楼层号"), 0, info); m_lcname = info;
	if(m_lcname.Find(_T(" ---> ")) != -1)
	{
		int pos = m_lcname.Find(_T(" ---> "));
		m_filter["起始层名"] = m_lcname.Left(pos);
		m_filter["终止层名"] = m_lcname.Mid(pos + _tcslen(_T(" ---> ")));
	}
	else
		m_filter["起始层名"] = info;
	IProjectMDB pdb; MStr linfo = pdb.getCXXTableInfo(m_filter);
	if(linfo[_T("是否跃层")].CompareNoCase(_T("1")) == 0)
	{
		m_pingwumian.EnableWindow(FALSE);
		m_xiewumian.EnableWindow(FALSE);
	}
	m_layername.SetWindowText(m_lcname);
	m_filter[_T("楼层号")] = linfo[_T("楼层号")];
	m_layerbz.SetWindowText(linfo[_T("层备注")]);
	m_sclayerheight.SetWindowText(linfo[_T("实测层高")]);
	m_sjlayerheight.SetWindowText(linfo[_T("设计层高")]);
	m_sjcontext.SetWindowText(linfo[_T("设计备注")]);
	m_sccontext.SetWindowText(linfo[_T("实测备注")]);
	m_tjlocation.InsertString(0, _T("左侧"));
	m_tjlocation.InsertString(1, _T("中间"));
	m_tjlocation.InsertString(2, _T("右侧"));
	setPWMParament(linfo); setXWMParament(linfo);
	CString type = pdb.getBGTable(m_ldname, _T("标高位置"));
	statisticArea(); CButton *btn = NULL;
	if(type.CompareNoCase(_T("女儿墙")) == 0 || type.CompareNoCase(_T("檐口")) == 0)
	{ btn = (CButton *)GetDlgItem(IDC_RADIO_HEIGHTNVQ); btn->SetCheck(1); }
	else if(type.CompareNoCase(_T("梯间")) == 0)
	{ btn = (CButton *)GetDlgItem(IDC_RADIO_HEIGHTTJ); btn->SetCheck(1); }
	else if(type.CompareNoCase(_T("顶层")) == 0)
	{ btn = (CButton *)GetDlgItem(IDC_RADIO_HEIGHTDC); btn->SetCheck(1); }
	return TRUE;
	// 异常:  OCX 属性页应返回 FALSE
}

void CSetLayerInfoDlg::setObject(const AcDbObjectId &id)
{
	m_obj = id;
}

void CSetLayerInfoDlg::OnBnClickedCheckSliPingwumian()
{
	// TODO:  在此添加控件通知处理程序代码
	m_xiewumian.SetCheck(0);
	m_angle.EnableWindow(FALSE);
	m_tjheight.EnableWindow(TRUE);
	m_wdheight.EnableWindow(FALSE);
	m_tjlocation.EnableWindow(TRUE);
	m_cwallheight.EnableWindow(FALSE);
	m_nvwallheight.EnableWindow(TRUE);

}

void CSetLayerInfoDlg::OnBnClickedCheckSliXiewumian()
{
	// TODO:  在此添加控件通知处理程序代码
	m_pingwumian.SetCheck(0);
	m_angle.EnableWindow(TRUE);
	m_wdheight.EnableWindow(TRUE);
	m_tjheight.EnableWindow(FALSE);
	m_tjlocation.EnableWindow(FALSE);
	m_cwallheight.EnableWindow(TRUE);
	m_nvwallheight.EnableWindow(FALSE);
}

void CSetLayerInfoDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	CString info;
	m_sjlayerheight.GetWindowText(info);
	MStr record; IProjectMDB pdb; addHeightInfo(info);
	record = pdb.getCXXTableInfo(m_filter);
	if(false == getPWMParament(record)) getXWMParament(record);
	getCommonInfo(record); 
	//pdb.deleteTableRecord(_T("CXX"), m_filter);
	double jzmj = 0.0, jrmj = 0.0;
	double sccg = _tstof(record[_T("实测层高")]);
	double sjcg = _tstof(record[_T("设计层高")]);
	getLayerArea(m_obj, jzmj, jrmj, sccg); getCarCount(m_obj, record, jzmj, jrmj);
	record[_T("高度差值")].Format(_T("%.3lf"), sccg - sjcg);
	int lcount = _ttoi(record[_T("层数")]);
	record[_T("建筑面积")].Format(_T("%.3lf"), jzmj * lcount);
	record[_T("计容面积")].Format(_T("%.3lf"), jrmj * lcount);
	//pdb.addCXXTableInfo(record); 
	pdb.updataCXXTableInfo(m_filter, record);
	CString type;
	CButton *btn = (CButton *)GetDlgItem(IDC_RADIO_HEIGHTNVQ);
	if(btn->GetCheck())
	{
		if(record[_T("屋面")].CompareNoCase(_T("平屋面")) == 0)
		{
			double height = _tstof(record[_T("参数1")]);
			if(height < 0.8) type = _T("檐口");
			else type = _T("女儿墙");
		}
	}
	btn = (CButton *)GetDlgItem(IDC_RADIO_HEIGHTTJ);
	if(btn->GetCheck()) type = _T("梯间");
	btn = (CButton *)GetDlgItem(IDC_RADIO_HEIGHTDC);
	if(btn->GetCheck()) type = _T("顶层");
	pdb.setBGInfoTable(m_ldname, _T("标高位置"), type);
	CDialog::OnOK();
}

void CSetLayerInfoDlg::getCommonInfo(MStr &record)
{
	CString info; m_sjlayerheight.GetWindowText(info);
	AddXdata(aname(m_obj), _T("设计层高"), 0, info);
	record[_T("设计层高")] = info;
	m_sclayerheight.GetWindowText(info);
	AddXdata(aname(m_obj), _T("实测层高"), 0, info);
	record[_T("实测层高")] = info;
	m_sccontext.GetWindowText(info);
	AddXdata(aname(m_obj), _T("实测备注"), 0, info);
	record[_T("实测备注")] = info;
	m_sjcontext.GetWindowText(info);
	AddXdata(aname(m_obj), _T("设计备注"), 0, info);
	record[_T("设计备注")] = info;
	m_layerbz.GetWindowText(info);
	AddXdata(aname(m_obj), _T("层备注"), 0, info);
	record[_T("层备注")] = info;
}

void CSetLayerInfoDlg::statisticArea()
{
	IDataBaseOper oper; TCHAR layer[255] = {0};
	TextProperty prop = oper.readTextPropertyTable(_T("建筑功能区"));
	AcDbObjectIdArray ids = getInnerObjects(m_obj);
	TCHAR info[25] = {0};
	double area = 0.0, jrmj = 0.0, jzmj = 0.0;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		GetEntLayer(id, layer);
		if(prop._layer.CompareNoCase(layer) != 0) continue;
		GetEntArea(aname(id), area);
		AcDbObjectIdArray gnqs = getInnerObjects(id);
		for(int indx = 0; indx < gnqs.length(); ++indx)
		{
			GetEntLayer(gnqs[indx], info);
			if(prop._layer.CompareNoCase(info) != 0) continue;
			double subArea = 0.0; GetEntArea(aname(gnqs[indx]), subArea);
			area -= subArea;
		}
		ReadXdata(aname(id), _T("面积系数"), 0, info);
		jzmj += area * _tstof(info);
		ReadXdata(aname(id), _T("计容系数"), 0, info);
		jrmj += area * _tstof(info);
	}
	_stprintf(info, _T("%.3lf"), jzmj);
	m_jzmj.SetWindowText(info);
	_stprintf(info, _T("%.3lf"), jrmj);
	m_jrmj.SetWindowText(info);
}

bool CSetLayerInfoDlg::getPWMParament(MStr &record)
{
	if(0 == m_pingwumian.GetCheck())
	{
		RemoveXdata(aname(m_obj), _T("平屋面"));
		RemoveXdata(aname(m_obj), _T("PARAM1"));
		RemoveXdata(aname(m_obj), _T("PARAM2"));
		RemoveXdata(aname(m_obj), _T("PARAM3"));
		record[_T("屋面")] = _T("");
		record[_T("参数1")] = _T("");
		record[_T("参数2")] = _T("");
		record[_T("参数3")] = _T("");
		return false;
	}
	AddXdata(aname(m_obj), _T("平屋面"), 0, _T("1"));
	record[_T("屋面")] = _T("平屋面");  CString info;
	m_nvwallheight.GetWindowText(info);
	record[_T("参数1")] = info;
	AddXdata(aname(m_obj), _T("PARAM1"), 0, info);
	m_tjheight.GetWindowTextW(info);
	record[_T("参数2")] = info;
	AddXdata(aname(m_obj), _T("PARAM2"), 0, info);
	int sel = m_tjlocation.GetCurSel(); info.Format(_T("%d"), sel);
	AddXdata(aname(m_obj), _T("PARAM3"), 0, info);
	record[_T("参数3")] = info;
	return true;
}

void CSetLayerInfoDlg::getXWMParament(MStr &record)
{
	if(0 == m_xiewumian.GetCheck())
	{
		RemoveXdata(aname(m_obj), _T("斜屋面"));
		RemoveXdata(aname(m_obj), _T("PARAM1"));
		RemoveXdata(aname(m_obj), _T("PARAM2"));
		RemoveXdata(aname(m_obj), _T("PARAM3"));
		record[_T("屋面")] = _T("");
		record[_T("参数1")] = _T("");
		record[_T("参数2")] = _T("");
		record[_T("参数3")] = _T("");
	}
	else
	{
		AddXdata(aname(m_obj), _T("斜屋面"), 0, _T("1")); CString info;
		record[_T("屋面")] = _T("斜屋面");
		m_cwallheight.GetWindowText(info);
		record[_T("参数1")] = info;
		AddXdata(aname(m_obj), _T("PARAM1"), 0, info);
		m_wdheight.GetWindowText(info);
		record[_T("参数2")] = info;
		AddXdata(aname(m_obj), _T("PARAM2"), 0, info);
		m_angle.GetWindowText(info);
		record[_T("参数3")] = info;
		AddXdata(aname(m_obj), _T("PARAM3"), 0, info);
	}
}

void CSetLayerInfoDlg::setPWMParament(int type)
{
	m_tjlocation.InsertString(0, _T("左侧"));
	m_tjlocation.InsertString(1, _T("中间"));
	m_tjlocation.InsertString(2, _T("右侧"));
	m_pingwumian.SetCheck(type); m_nvwallheight.EnableWindow(type);
	m_tjheight.EnableWindow(type); m_tjlocation.EnableWindow(type);
	if(type == 0) return; TCHAR info[255] = {0};
	ReadXdata(aname(m_obj), _T("PARAM1"), 0, info); m_nvwallheight.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM2"), 0, info); m_tjheight.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM3"), 0, info); m_tjlocation.SetCurSel(_ttoi(info));
}

void CSetLayerInfoDlg::setPWMParament(MStr &record)
{
	int type = 0;
	if(record[_T("屋面")].CompareNoCase(_T("平屋面")) == 0) type = 1;
	m_pingwumian.SetCheck(type); m_nvwallheight.EnableWindow(type);
	m_tjheight.EnableWindow(type); m_tjlocation.EnableWindow(type);
	if(type == 0) return; TCHAR info[255] = {0};
	ReadXdata(aname(m_obj), _T("PARAM1"), 0, info); m_nvwallheight.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM2"), 0, info); m_tjheight.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM3"), 0, info); m_tjlocation.SetCurSel(_ttoi(info));
}

void CSetLayerInfoDlg::setXWMParament(int type)
{
	m_angle.EnableWindow(type);	m_wdheight.EnableWindow(type);
	m_cwallheight.EnableWindow(type); m_xiewumian.SetCheck(type);
	if(type == 0) return; TCHAR info[255] = {0};
	ReadXdata(aname(m_obj), _T("PARAM3"), 0, info); m_angle.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM2"), 0, info); m_wdheight.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM1"), 0, info); m_cwallheight.SetWindowText(info);
}

void CSetLayerInfoDlg::setXWMParament(MStr &record)
{
	int type = 0;
	if(record[_T("屋面")].CompareNoCase(_T("斜屋面")) == 0) type = 1;
	m_angle.EnableWindow(type);	m_wdheight.EnableWindow(type);
	m_cwallheight.EnableWindow(type); m_xiewumian.SetCheck(type);
	if(type == 0) return; TCHAR info[255] = {0};
	ReadXdata(aname(m_obj), _T("PARAM3"), 0, info); m_angle.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM2"), 0, info); m_wdheight.SetWindowText(info);
	ReadXdata(aname(m_obj), _T("PARAM1"), 0, info); m_cwallheight.SetWindowText(info);
}

void CSetLayerInfoDlg::addHeightInfo(const CString &height)
{
	AcDbObjectIdArray ids = getInnerObjects(m_obj);
	TCHAR info[25] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(!ReadXdata(aname(id), _T("功能区高度"), 0, info))
			AddXdata(aname(id), _T("功能区高度"), 0, height);
	}
}

// CSetLayerInfoDlg 对话框

IMPLEMENT_DYNAMIC(CProjectInfoPrompt, CAcUiDialog)

CProjectInfoPrompt::CProjectInfoPrompt(CWnd* pParent /*=NULL*/)
: CAcUiDialog(CSetLayerInfoDlg::IDD, pParent)
{

}

CProjectInfoPrompt::~CProjectInfoPrompt()
{

}

void CProjectInfoPrompt::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROJECTCODEPROMPT, m_promptcode);
	DDX_Control(pDX, IDC_STATIC_PROJECTNAMEPROMPT, m_promptname);
}

BEGIN_MESSAGE_MAP(CProjectInfoPrompt, CAcUiDialog)
END_MESSAGE_MAP()

// CSetLayerInfoDlg 消息处理程序

extern CProjectInfoDockbar g_projectpromptdock;

BOOL CProjectInfoPrompt::OnInitDialog()
{
	// TODO:  在此添加额外的初始化
	CAcUiDialog::OnInitDialog();
	CRect prect; g_projectpromptdock.GetClientRect(&prect);
	int width = abs(prect.right - prect.left) / 2;
	m_promptcode.MoveWindow(2, 0, width, 40);
	m_promptname.MoveWindow(width + 2, 0, width, 40);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// CProjectInfoDockbar 对话框

IMPLEMENT_DYNAMIC(CProjectInfoDockbar, CAdUiDockControlBar)

BEGIN_MESSAGE_MAP(CProjectInfoDockbar, CAdUiDockControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static CLSID clsCMyDockBar = {0x4ebb7914, 0x7356, 0x43de, {0xb4, 0x50, 0x62, 0xbe, 0xae, 0x21, 0x06, 0xde}};

//-----------------------------------------------------------------------------
CProjectInfoDockbar::CProjectInfoDockbar()
	: CAdUiDockControlBar()
{

}

void CProjectInfoDockbar::setDlg(int IndexIDD)
{
	m_IDD = IndexIDD;
}

BOOL CProjectInfoDockbar::Create(CWnd*pParent, LPCTSTR lpszTitle)
{
	CString strWndClass;
	strWndClass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW));
	CRect rect(0, 0, 250, 200);
	if(!CAdUiDockControlBar::Create(strWndClass, lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParent, 0))
		return (FALSE);
	EnableDocking(CBRS_ALIGN_BOTTOM);
	SetToolID(&clsCMyDockBar);
	return (TRUE);
}

int CProjectInfoDockbar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CAdUiDockControlBar::OnCreate(lpCreateStruct) == -1)
		return (-1);
	m_prompt.Create(m_IDD, this);
	m_prompt.ShowWindow(SW_SHOW);
	return (0);
}

void CProjectInfoDockbar::SizeChanged(CRect *lpRect, BOOL bFloating, int flags)
{
	CAdUiDockControlBar::SizeChanged(lpRect, bFloating, flags);
}

BOOL CProjectInfoDockbar::DestroyWindow()
{
	m_prompt.DestroyWindow();
	return CAdUiDockControlBar::DestroyWindow();
}

BOOL CProjectInfoDockbar::AddCustomMenuItems(LPARAM hMenu)
{
	HMENU hPopMenu;	hPopMenu = (HMENU)(hMenu);
	CMenu* pop_menu = CMenu::FromHandle(hPopMenu);
	pop_menu->RemoveMenu(ID_ADUI_HIDEBAR, MF_BYCOMMAND); //去掉"允许固定"
	pop_menu->RemoveMenu(ID_ADUI_ALLOWDOCK, MF_BYCOMMAND);//去掉"隐藏"
	return true;
}

// XMJGBridgeAnnotateDlg 对话框

IMPLEMENT_DYNAMIC(XMJGBridgeAnnotateDlg, CDialog)

XMJGBridgeAnnotateDlg::XMJGBridgeAnnotateDlg(CWnd* pParent /*=NULL*/)
: CDialog(XMJGBridgeAnnotateDlg::IDD, pParent)
{

}

XMJGBridgeAnnotateDlg::~XMJGBridgeAnnotateDlg()
{

}

void XMJGBridgeAnnotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboBoxType);
	DDX_Control(pDX, IDC_LIST3, m_listCtrlValue);
}

BEGIN_MESSAGE_MAP(XMJGBridgeAnnotateDlg, CDialog)
	ON_BN_CLICKED(IDOK, &XMJGBridgeAnnotateDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &XMJGBridgeAnnotateDlg::OnAddNewRow)
	ON_BN_CLICKED(IDC_IDDelete, &XMJGBridgeAnnotateDlg::OnBnClickedDelete)
	ON_CBN_SELCHANGE(IDC_COMBO1, &XMJGBridgeAnnotateDlg::OnChangeSelectItem)
	ON_BN_CLICKED(IDC_BUTTONADD, &XMJGBridgeAnnotateDlg::OnBnClickedButtonAdd)
END_MESSAGE_MAP()

// XMJGBridgeAnnotateDlg 消息处理程序

void XMJGBridgeAnnotateDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	IProjectMDB pdb; 	CString type;
	m_comboBoxType.GetWindowText(type);
	m_value[type] = getListCtrlValues();
	pdb.setBridgeXX(m_value);
	CDialog::OnOK();
}

void XMJGBridgeAnnotateDlg::OnBnClickedDelete()
{
	// TODO:  在此添加控件通知处理程序代码
	int nItem = m_listCtrlValue.GetItemCount();
	CString name; m_comboBoxType.GetWindowText(name);
	for(; nItem >= 0; nItem--)
	{
		if(m_listCtrlValue.GetItemState(nItem, LVNI_ALL | LVNI_SELECTED) != LVIS_SELECTED) continue;
		m_listCtrlValue.DeleteItem(nItem);
	}
	nItem = m_listCtrlValue.GetItemCount();
	if(m_listCtrlValue.GetItemText(nItem, 0).IsEmpty() == false)
		m_listCtrlValue.InsertItem(nItem + 1, _T(""));
	this->UpdateData();
}

BOOL XMJGBridgeAnnotateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	IDataBaseOper oper;
	CString type = oper.readCommonTable(_T("人形天桥标注"));
	AnalyzeData(m_types, type, _T(";"));
	for(int idx = 0; idx < (int)m_types.size(); ++idx)
		m_comboBoxType.InsertString(idx, m_types[idx]);
	m_listCtrlValue.Init(true, 24); CRect rect; m_listCtrlValue.GetClientRect(&rect);
	m_listCtrlValue.InsertColumn(0, _T("值"));
	m_listCtrlValue.SetColumnWidth(0, rect.right - rect.left);
	m_listCtrlValue.InsertItem(1, _T(""));
	IProjectMDB pdb;
	m_value = pdb.getBridgeXX();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void XMJGBridgeAnnotateDlg::OnBnClickedButtonAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	CString name; m_comboBoxType.GetWindowText(name);
	VSIter it = find(m_types.begin(), m_types.end(), name);
	if(it != m_types.end()) return;
	m_comboBoxType.InsertString(m_types.size(), name);
	m_types.push_back(name);
}

void XMJGBridgeAnnotateDlg::OnAddNewRow(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int rowNo = pNMListView->iItem;
	int colNo = pNMListView->iSubItem;
	int count = m_listCtrlValue.GetItemCount();
	CString value = m_listCtrlValue.GetItemText(rowNo, 0);
	if(value.IsEmpty() == false && (rowNo + 1) == count)
		m_listCtrlValue.InsertItem(count, _T(""));
}

void XMJGBridgeAnnotateDlg::OnChangeSelectItem()
{
	int nIndex = m_comboBoxType.GetCurSel();
	CString oldType;
	m_comboBoxType.GetWindowText(oldType);
	if(oldType.IsEmpty() == false)
		m_value[oldType] = getListCtrlValues();
	CString type = m_types[nIndex];
	setListCtrlValues(m_value[type]);
}

CString XMJGBridgeAnnotateDlg::getListCtrlValues()
{
	CString value, ret;
	int count = m_listCtrlValue.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		value = m_listCtrlValue.GetItemText(idx, 0);
		if(value.IsEmpty() == true) continue;
		ret += value + _T(";");
	}
	return ret;
}

void XMJGBridgeAnnotateDlg::setListCtrlValues(const CString &value)
{
	m_listCtrlValue.DeleteAllItems();
	VStr values; AnalyzeData(values, value, _T(";"));
	for(int idx = 0; idx < (int)values.size(); ++idx)
		m_listCtrlValue.InsertItem(idx, values[idx]);
	if(values.size() == 0)
		m_listCtrlValue.InsertItem(values.size(), _T(""));
	else
		if(values[values.size() - 1].IsEmpty() == false)
			m_listCtrlValue.InsertItem(values.size(), _T(""));
}

// COpenProjectDlg 对话框

IMPLEMENT_DYNAMIC(COpenProjectDlg, CDialog)

COpenProjectDlg::COpenProjectDlg(CWnd* pParent /*=NULL*/)
: CDialog(COpenProjectDlg::IDD, pParent)
{

}

COpenProjectDlg::~COpenProjectDlg()
{

}

void COpenProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECTDWG, m_projectdwg);
	DDX_Control(pDX, IDC_EDIT_PROJECTPATH, m_projectpath);
}

BEGIN_MESSAGE_MAP(COpenProjectDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ASELECT, &COpenProjectDlg::OnAllSelect)
	ON_BN_CLICKED(IDC_BUTTON_RSELECT, &COpenProjectDlg::OnReverseSelect)
	ON_BN_CLICKED(IDC_BUTTON_SELECTPROJECT, &COpenProjectDlg::OnSelectProject)
END_MESSAGE_MAP()

// COpenProjectDlg 消息处理程序

BOOL COpenProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initListCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

CString COpenProjectDlg::getProjectPath()
{
	return m_projectPath;
}

void COpenProjectDlg::setProjectDir(const CString &dir)
{
	CFilePathOperater::GetChildFile(dir, m_pathNames, m_fileNames);
	for(int idx = m_fileNames.GetSize() - 1; idx >= 0; --idx)
	{
		CString filename = m_fileNames.GetAt(idx);
		if(filename.Find(_T(".dwg")) != -1) continue;
		m_pathNames.RemoveAt(idx);
		m_fileNames.RemoveAt(idx);
	}
	setListValue(dir);
}

void COpenProjectDlg::initListCtrl()
{
	m_projectdwg.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_projectdwg.GetClientRect(&rect);
	int width = (rect.right - rect.left - 60) / 2;
	m_projectdwg.InsertColumn(0, _T("是否打开"), LVCFMT_CENTER, 60);
	m_projectdwg.InsertColumn(1, _T("文件名称"), LVCFMT_LEFT, width);
	m_projectdwg.InsertColumn(2, _T("文件名称"), LVCFMT_LEFT, width);
	m_projectdwg.SetColumnReadOnly(1, TRUE);
	m_projectdwg.SetColumnReadOnly(2, TRUE);
}

void COpenProjectDlg::setListValue(const CString &dir)
{
	int len = dir.GetLength(); CString pathName;
	for(int idx = 0; idx < m_pathNames.GetSize(); ++idx)
	{
		m_projectdwg.InsertItem(idx, _T(""));
		m_projectdwg.SetItemText(idx, 1, m_fileNames.GetAt(idx));
		pathName = m_pathNames.GetAt(idx);
		m_projectdwg.SetItemText(idx, 2, pathName.Mid(len + 1));
		m_projectdwg.SetCheck(idx);
	}
}

void COpenProjectDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	int count = m_projectdwg.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL flag = m_projectdwg.GetCheck(idx);
		if(flag == FALSE) continue;
		m_openFiles.push_back(m_pathNames.GetAt(idx));
	}
	m_projectpath.GetWindowText(m_projectPath);
	CDialog::OnOK();
}

VStr COpenProjectDlg::getOpenFiles()
{
	return m_openFiles;
}

CString cdName(const CString &name)
{
	CString Name;
	int pos = name.ReverseFind(_T('\\'));
	Name = name.Mid(pos + 1);
	return Name;
}

CString cdPath(const CString &path, int times)
{
	CString temp(path); int pos = -1;
	for(int idx = 0; idx < times; ++idx)
	{
		pos = temp.ReverseFind(_T('\\'));
		temp = temp.Mid(0, pos);
	}
	return temp;
}

void COpenProjectDlg::OnSelectProject()
{
	// TODO:  在此添加控件通知处理程序代码
	IDataBaseOper oper;
	CString dir = oper.readPathTable(_T("工程目录"));
	CFileDialog dlg(TRUE, 0, 0, 6, _T("*.mdb | *.mdb"));
	dlg.m_ofn.lpstrInitialDir = dir;
	dlg.m_ofn.lpstrTitle = _T("请选择项目图形");
	if(dlg.DoModal() != IDOK) return;
	dir = dlg.GetPathName();
	dir = cdPath(dir, 2);
	m_projectpath.SetWindowText(dir);
	setProjectDir(dir);
}

void COpenProjectDlg::OnAllSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_projectdwg.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
		m_projectdwg.SetCheck(idx);
}

void COpenProjectDlg::OnReverseSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_projectdwg.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL flag = m_projectdwg.GetCheck(idx);
		m_projectdwg.SetCheck(idx, !flag);
	}
}

// CPersonBridgeTechnicalDlg 对话框

IMPLEMENT_DYNAMIC(CPersonBridgeTechnicalDlg, CDialog)

CPersonBridgeTechnicalDlg::CPersonBridgeTechnicalDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPersonBridgeTechnicalDlg::IDD, pParent)
{

}

CPersonBridgeTechnicalDlg::~CPersonBridgeTechnicalDlg()
{

}

void CPersonBridgeTechnicalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_XMJG_PBQTXYSMDQK, m_pbqtxysmdqk);
	DDX_Control(pDX, IDC_EDIT_XMJG_PBYGHXKBYZQK, m_pbyghxkbyzqk);
	DDX_Control(pDX, IDC_EDITPBPISLICENSEDOCS, m_pbpislicensedocs);
	DDX_Control(pDX, IDC_EDITPBCONTROLBASEPOINT, m_pbcontrolbasepoint);
	DDX_Control(pDX, IDC_EDITPBTOPOGRAPHICSURVEY, m_pbtopographicsurvey);
	DDX_Control(pDX, IDC_EDITPBEXISTINGINFOMATION, m_pbexistinginfomation);
	DDX_Control(pDX, IDC_EDIT_PBOVERPASSMEASUREMENT, m_pboverpassmeasurment);
}

BEGIN_MESSAGE_MAP(CPersonBridgeTechnicalDlg, CDialog)
END_MESSAGE_MAP()

// CPersonBridgeTechnicalDlg 消息处理程序

BOOL CPersonBridgeTechnicalDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	CWnd *wnd = GetFocus();
	if(wnd != GetDlgItem(IDC_EDIT_XMJG_PBQTXYSMDQK) ||
		wnd != GetDlgItem(IDC_EDITPBPISLICENSEDOCS) ||
		wnd != GetDlgItem(IDC_EDIT_XMJG_PBYGHXKBYZQK) ||
		wnd != GetDlgItem(IDC_EDITPBCONTROLBASEPOINT) ||
		wnd != GetDlgItem(IDC_EDITPBTOPOGRAPHICSURVEY) ||
		wnd != GetDlgItem(IDC_EDITPBEXISTINGINFOMATION) ||
		wnd != GetDlgItem(IDC_EDIT_PBOVERPASSMEASUREMENT))
		return CDialog::PreTranslateMessage(pMsg);
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPersonBridgeTechnicalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	IProjectMDB pdb; MStr jssm = pdb.getJSSMTableInfo();
	m_pbtopographicsurvey.SetWindowText(jssm[_T("地形测量")]);
	m_pboverpassmeasurment.SetWindowText(jssm[_T("天桥测量")]);
	m_pbqtxysmdqk.SetWindowText(jssm[_T("其他需要说明的情况")]);
	m_pbexistinginfomation.SetWindowText(jssm[_T("已有资料情况")]);
	m_pbyghxkbyzqk.SetWindowText(jssm[_T("与规划许可不一致情况")]);
	m_pbcontrolbasepoint.SetWindowText(jssm[_T("起算控制点基本情况")]);
	m_pbpislicensedocs.SetWindowText(jssm[_T("测绘成果依据的许可文书")]);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CPersonBridgeTechnicalDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	IProjectMDB pdb; MStr jssm;
	m_pbtopographicsurvey.GetWindowText(jssm[_T("地形测量")]);
	m_pboverpassmeasurment.GetWindowText(jssm[_T("天桥测量")]);
	m_pbqtxysmdqk.GetWindowText(jssm[_T("其他需要说明的情况")]);
	m_pbexistinginfomation.GetWindowText(jssm[_T("已有资料情况")]);
	m_pbyghxkbyzqk.GetWindowText(jssm[_T("与规划许可不一致情况")]);
	m_pbcontrolbasepoint.GetWindowText(jssm[_T("起算控制点基本情况")]);
	m_pbpislicensedocs.GetWindowText(jssm[_T("测绘成果依据的许可文书")]);
	pdb.setJSSMTableInfo(jssm);
	CDialog::OnOK();
}

// CSetHandlePipeLineDlg 对话框

IMPLEMENT_DYNAMIC(CSetHandlePipeLineDlg, CDialog)

CSetHandlePipeLineDlg::CSetHandlePipeLineDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSetHandlePipeLineDlg::IDD, pParent)
{

}

CSetHandlePipeLineDlg::~CSetHandlePipeLineDlg()
{

}

void CSetHandlePipeLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PIPELINE, m_listpipeline);
}

BEGIN_MESSAGE_MAP(CSetHandlePipeLineDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &CSetHandlePipeLineDlg::OnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_ADDPIPELINE, &CSetHandlePipeLineDlg::OnClickedButtonAddPipeline)
	ON_BN_CLICKED(IDC_BUTTON_REVERSESELECT, &CSetHandlePipeLineDlg::OnClickedButtonReverseSelect)
END_MESSAGE_MAP()

// CSetHandlePipeLineDlg 消息处理程序

void CSetHandlePipeLineDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialog::OnOK();
}

void CSetHandlePipeLineDlg::setListData()
{
	CString line = _T("饮用水管段,非饮用水管段,雨、污合流管段,雨水管段,污水管段,中国电信管段,中国联通管段,中国移动管段,中国网通管段,监控信号管段,军用管段,电力通信管段,宽带管段,电视管段,广播管段,废水管段,石油管段,不明管线管段,交通信号管段,路灯管段,交通信号管段,保密管段,其他通信管段,煤气管段,液化气管段,天然气管段,蒸汽管段,温泉管段,氢气管段,氧气管段,氮气管段,饮用水管段,非饮用水管段,雨、污合流管段,雨水管段,污水管段,中国电信管段,中国联通管段,中国移动管段,中国网通管段,监控信号管段,军用管段,电力通信管段,宽带管段,电视管段,广播管段,废水管段,石油管段,不明管线管段,供电管段,路灯管段,交通信号管段,保密管段,其他通信管段,煤气管段,液化气管段,天然气管段,蒸汽管段,温泉管段,氢气管段,氧气管段,氮气管段");
	VStr lines; AnalyzeData(lines, line, _T(","));
	for(int idx = 0; idx < (int)lines.size(); ++idx)
	{
		m_listpipeline.InsertItem(idx, _T(""));
		m_listpipeline.SetItemText(idx, 1, lines[idx]);
		m_listpipeline.SetCheck(idx);
		m_listpipeline.SetCellColor(idx, 0, RGB(255, 0, 0));
		m_listpipeline.SetCellColor(idx, 1, RGB(0, 255, 0));
		m_listpipeline.SetCellColor(idx, 2, RGB(0, 0, 255));
	}
}

BOOL CSetHandlePipeLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	initializeList();
	setListData();
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CSetHandlePipeLineDlg::initializeList()
{
	m_listpipeline.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_listpipeline.GetClientRect(&rect);
	int width = (rect.right - rect.left - 80) / 3;
	m_listpipeline.InsertColumn(0, _T("是否处理"), LVCFMT_CENTER, 65);
	m_listpipeline.InsertColumn(1, _T("管线名称"), LVCFMT_CENTER, width);
	m_listpipeline.InsertColumn(2, _T("管线颜色"), LVCFMT_CENTER, width);
	m_listpipeline.InsertColumn(3, _T("管线图层"), LVCFMT_CENTER, width);
	m_listpipeline.SetColumnReadOnly(0, TRUE);
	m_listpipeline.SetColumnReadOnly(1, TRUE);
	m_listpipeline.EnableCustomBackkColor(TRUE);
	UpdateData(FALSE);
}

void CSetHandlePipeLineDlg::OnClickedButtonSelectAll()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_listpipeline.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
		m_listpipeline.SetCheck(idx, TRUE);
}

void CSetHandlePipeLineDlg::OnClickedButtonAddPipeline()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_listpipeline.GetItemCount();
	m_listpipeline.InsertItem(count, _T(""));
	m_listpipeline.SetItemText(count, 1, _T("你是谁"));
	m_listpipeline.SetCheck(count);
	m_listpipeline.SetCellColor(count, 0, RGB(255, 0, 0));
	m_listpipeline.SetCellColor(count, 1, RGB(0, 255, 0));
	m_listpipeline.SetCellColor(count, 2, RGB(0, 0, 255));
}

void CSetHandlePipeLineDlg::OnClickedButtonReverseSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_listpipeline.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL flag = m_listpipeline.GetCheck(idx);
		m_listpipeline.SetCheck(idx, !flag);
	}
}

// CPipeConfig 对话框
IMPLEMENT_DYNAMIC(CPipeConfig, CDialog)

CPipeConfig::CPipeConfig(CWnd* pParent /*=NULL*/)
: CDialog(CPipeConfig::IDD, pParent)
{

}

CPipeConfig::~CPipeConfig()
{

}

void CPipeConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTNAME, m_projectname);
	DDX_Control(pDX, IDC_EDIT_PIPELINEBUILDCOMPANY, m_buildcompany);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTCODE, m_projectcode);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTADDRESS, m_projectaddress);
	DDX_Control(pDX, IDC_EDIT_PIPELINEMEANSURECOMPANY, m_meansurecompany);
	DDX_Control(pDX, IDC_EDIT_PIPELINEIMPLEMENTCOMPANY, m_implementcompany);
	DDX_Control(pDX, IDC_EDIT_PIPELINECOORSYSTEM, m_coorsystem);
	DDX_Control(pDX, IDC_EDIT_PIPELINEEVEALATIONSYSTEM, m_evelationsystem);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTDIR, m_projectdir);
}

BEGIN_MESSAGE_MAP(CPipeConfig, CDialog)
	//	ON_WM_CLOSE()
	// 	ON_BN_CLICKED(IDOK3, &CPipeConfig::OnOK)
	// 	ON_BN_CLICKED(ID_CHOOSEALL, &CPipeConfig::OnClickedButtonSelectAll)
	// 	ON_BN_CLICKED(ID_CHOOSENONE, &CPipeConfig::OnClickedButtonSelectNone)
	// 	ON_BN_CLICKED(ID_OPCHOOSE, &CPipeConfig::OnClickedButtonReverseSelect)
END_MESSAGE_MAP()

// CPipeConfig 消息处理程序
BOOL CPipeConfig::OnInitDialog()
{
	CDialog::OnInitDialog();
	initProjectInfo();
	return TRUE;
}

//void CPipeConfig::OnClickedButtonSelectAll()
//{
//
//}
//
//void CPipeConfig::OnClickedButtonSelectNone()
//{
//
//}
//
//void CPipeConfig::OnClickedButtonReverseSelect()
//{
//
//}
// 
// void CPipeConfig::OnOK()
// {
// 	CDialog::OnOK();
// 	CString strPath = _T("");
// 	setProjectInfo(strPath);
// 	createFolder(strPath);
// }

void CPipeConfig::LoadMdb()
{


}

// void CPipeConfig::OnClose()
// {
// 	CDialog::OnCancel();
// }

//从RGB得到cad颜色索引值
int GetColorIndexFromRBG(COLORREF color)
{
	long acirgb, r, g, b;
	long mindst = 2147483647L;
	long dst = 0;
	int minndx = 0;
	long red = GetRValue(color);
	long green = GetGValue(color);
	long blue = GetBValue(color);
	for(int i = 1; i < 255; i++)
	{
		acirgb = acdbGetRGB(i);
		r = GetRValue(acirgb);
		g = GetGValue(acirgb);
		b = GetBValue(acirgb);

		dst = abs(r - red) + abs(g - green) + abs(b - blue);
		if(dst < mindst)
		{
			minndx = i;
			mindst = dst;
		}
	}
	return minndx;
}
// 
// void CPipeConfig::DBClickColor(NMHDR* pNMHDR, LRESULT* pResult)
// {
// 	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
// 	int colNo = pNMListView->iSubItem;
// 	int rowNo = pNMListView->iItem;
// 	if(colNo == 3)
// 	{
// 		COLORREF color = RGB(255, 0, 0);      // 颜色对话框的初始颜色为红色  
// 		CColorDialog colorDlg(color);         // 构造颜色对话框，传入初始颜色值   
// 		if(IDOK == colorDlg.DoModal())
// 		{
// 			color = colorDlg.GetColor();
// 			int nColor = GetColorIndexFromRBG(color);
// 			strColor.Format(_T("%d"), nColor);
// 			m_pipilist.SetItemText(rowNo, 2, strColor);
// 		}
// 	}
// }

void CPipeConfig::createFolder(CString &strPath)
{
	// 	CString name, code;
	// 	m_projectcode.GetWindowText(name);
	// 	m_projectname.GetWindowText(code);
	// 	CString dir; dir.Format(_T("%s\\%s_%s"), m_strdir, name, code);
	// 	if(_taccess(dir, 0) == -1) _tmkdir(dir);
	// 	//int nCount = m_pipilist.GetItemCount();
	// 	CString str1, str2, str3; VMStr vtemp;
	// 	dir.Append(_T("\\CG\\"));
	// 	_tmkdir(dir + _T("道路"));
	// 	for(size_t i = 0; i < nCount; ++i)
	// 	{
	// 		if(FALSE == m_pipilist.GetCheck(i)) continue;
	// 		str1 = m_pipilist.GetItemText(i, 1);
	// 		str2 = m_pipilist.GetItemText(i, 2);
	// 		str3 = m_pipilist.GetItemText(i, 3);
	// 		_tmkdir(dir + str1);
	// 		MStr mtemp;
	// 		mtemp[_T("名称")] = str1;
	// 		mtemp[_T("简码")] = str2;
	// 		mtemp[_T("颜色")] = str3;
	// 		vtemp.push_back(mtemp);
	// 	}
	// 
	// 	IProjectMDB pdb;
	// 	//这里有个奇怪的bug,pdb直接构造的mdb为上一个项目,故手动重新设置当前mdb
	// 	pdb.ChangeConnection(strPath + _T("\\GC\\项目信息.mdb"));
	// 	pdb.setPipeLineTable(_T("PIPELINE"), vtemp);
}

void CPipeConfig::setProjectInfo(CString &strPath)
{
	MStr context;
	m_projectcode.GetWindowText(context[_T("受理号")]);
	m_coorsystem.GetWindowText(context[_T("坐标系统")]);
	m_projectname.GetWindowText(context[_T("项目名称")]);
	m_buildcompany.GetWindowText(context[_T("建设单位")]);
	m_projectaddress.GetWindowText(context[_T("项目地点")]);
	m_meansurecompany.GetWindowText(context[_T("测量单位")]);
	m_evelationsystem.GetWindowText(context[_T("高程系统")]);
	m_implementcompany.GetWindowText(context[_T("实施单位")]);
	IProjectInfo pi(_T("管线竣工"));
	pi.CreateProject(context, strPath);
}

void CPipeConfig::initProjectInfo()
{
	IDataBaseOper db;
	m_strdir = db.readPathTable(_T("工程目录")) + _T("管线竣工");
	m_projectdir.SetWindowText(m_strdir);
}

void CPipeConfig::initPipeLineList()
{
	// 	m_pipilist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	// 	CRect rect; m_pipilist.GetClientRect(&rect);
	// 	int width = (rect.right - rect.left - 25) / 3;
	// 	m_pipilist.InsertColumn(0, _T(""), LVCFMT_CENTER, 25);
	// 	m_pipilist.InsertColumn(1, _T("管线名称"), LVCFMT_CENTER, width - 10);
	// 	m_pipilist.InsertColumn(2, _T("简码"), LVCFMT_CENTER, width - 10);
	// 	m_pipilist.InsertColumn(3, _T("管线颜色"), LVCFMT_CENTER, width - 10);
	// 	m_pipilist.SetColumnReadOnly(1, TRUE);
	// 	m_pipilist.SetColumnReadOnly(2, TRUE);
	// 	LoadMdb(); UpdateData(FALSE);
}

void CPipeConfig::Apply()
{
	CString strPath = _T("");
	setProjectInfo(strPath);
	createFolder(strPath);
}

// CPipeZPTExport 对话框

IMPLEMENT_DYNAMIC(CPipeZPTExport, CDialog)

CPipeZPTExport::CPipeZPTExport(CWnd* pParent /*=NULL*/)
: CDialog(CPipeZPTExport::IDD, pParent)
{

}

CPipeZPTExport::~CPipeZPTExport()
{
}

void CPipeZPTExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_pipelist);
}


BEGIN_MESSAGE_MAP(CPipeZPTExport, CDialog)
	ON_BN_CLICKED(IDOK, &CPipeZPTExport::OnOK)
END_MESSAGE_MAP()

void CPipeZPTExport::OnOK()
{
	CDialog::OnOK();
	GetZPTLayers();
}

BOOL CPipeZPTExport::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pipelist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_pipelist.GetClientRect(&rect);
	int width = (rect.right - rect.left - 25) / 3;
	m_pipelist.InsertColumn(0, _T(""), LVCFMT_CENTER, 25);
	m_pipelist.InsertColumn(1, _T("管线名称"), LVCFMT_CENTER, width * 2);
	m_pipelist.InsertColumn(2, _T("简码"), LVCFMT_CENTER, width);
	m_pipelist.SetColumnReadOnly(1, TRUE);

	IProjectMDB oper;
	VMStr vmRecords;
	vmRecords = oper.getPipeTable();
	int size = vmRecords.size();
	for(int idx = 0; idx < size; ++idx)
	{
		MStr msTemp = vmRecords[idx];
		m_pipelist.InsertItem(idx, _T(""));
		m_pipelist.SetItemText(idx, 1, msTemp[_T("名称")]);
		m_pipelist.SetItemText(idx, 2, msTemp[_T("简码")]);
	}
	return true;
}

void CPipeZPTExport::GetZPTLayers()
{
	vmsLayers.clear();
	int nCount = m_pipelist.GetItemCount();
	for(int i = 0; i < nCount; ++i)
	{
		CString str1 = _T(""), str2 = _T("");
		MStr msTemp;
		if(FALSE == m_pipelist.GetCheck(i)) continue;
		str1 = m_pipelist.GetItemText(i, 1);
		str2 = m_pipelist.GetItemText(i, 2);
		msTemp[_T("名称")] = str1;
		msTemp[_T("简码")] = str2;
		vmsLayers.push_back(msTemp);
	}
}

// CPipeLineJBXXDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLineJBXXDlg, CDialog)

CPipeLineJBXXDlg::CPipeLineJBXXDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineJBXXDlg::IDD, pParent)
{

}

CPipeLineJBXXDlg::~CPipeLineJBXXDlg()
{
}

void CPipeLineJBXXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PLCOORSYSTEM, m_coorsystem);
	DDX_Control(pDX, IDC_EDIT_PLPROJECTCODE, m_projectcode);
	DDX_Control(pDX, IDC_EDIT_PLPROJECTNAME, m_projectname);
	DDX_Control(pDX, IDC_EDIT_PLBUILDCOMPANY, m_buildcompany);
	DDX_Control(pDX, IDC_EDIT_PLPROJECTADDRESS, m_projectaddress);
	DDX_Control(pDX, IDC_EDIT_PLMEASURECOMPANY, m_measurecompany);
	DDX_Control(pDX, IDC_EDIT_PLIMPLEMENTCOMPANY, m_implementcompany);
	DDX_Control(pDX, IDC_EDIT_PLEVEATIONALSYSTEM, m_evelationalsystem);
	DDX_Control(pDX, IDC_EDIT_PIPELINECCCGYJXKWS, m_cccgyjxkws);
	DDX_Control(pDX, IDC_EDIT_PIPELINEYYZLQK, m_yyzlqk);
	DDX_Control(pDX, IDC_EDIT_PIPELINETGKZQK, m_tgkzqk);
	DDX_Control(pDX, IDC_EDIT_PIPELINEXBCLJBQK, m_xbcljbqk);
	DDX_Control(pDX, IDC_EDIT_PIPELINEDXGXDTC, m_dxgxdtc_tqcl);
	DDX_Control(pDX, IDC_EDIT_PIPELINEYGHXKBYZQK, m_yghxkbyzqk);
	DDX_Control(pDX, IDC_EDIT_PIPELINEQTXYSMDQK, m_qtxysmdqk);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dataPick);
	DDX_Control(pDX, IDC_RADIO_TQCL, m_tqcl);
	DDX_Control(pDX, IDC_RADIO_DXGXDTC, m_dxgxdtc);
}

BEGIN_MESSAGE_MAP(CPipeLineJBXXDlg, CDialog)
END_MESSAGE_MAP()

// CPipeLineJBXXDlg 消息处理程序

void CPipeLineJBXXDlg::Apply()
{
	saveProjectInfo();
	saveProjectJSSM();
	CDialog::OnOK();
}

BOOL CPipeLineJBXXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initProjectInfo();
	initProjectJSSM();
	m_dxgxdtc.SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

BOOL CPipeLineJBXXDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPipeLineJBXXDlg::initProjectInfo()
{
	IProjectMDB pdb; IDataBaseOper db; CStringArray values;
	CString value = db.readJGCommon(_T("测量单位"));
	AnalyzeData(values, value, _T(";"));
	MStr context = pdb.getProjectInfo(); int index = 0;
	for(int idx = 0; idx < (int)values.GetSize(); ++idx)
	{
		CString value = values.GetAt(idx);
		m_measurecompany.InsertString(idx, value);
		if(value.CompareNoCase(context[_T("测量单位")]) != 0) continue;
		index = idx;
	}
	values.RemoveAll();
	value = context[_T("项目时间")];
	if(-1 != value.Find(_T('/')))
		AnalyzeData(values, value, _T("/"));
	else if(-1 != value.Find(_T('-')))
		AnalyzeData(values, value, _T("-"));
	if(values.GetSize() == 3)
	{
		int year = _ttoi(values.GetAt(0));
		int mouth = _ttoi(values.GetAt(1));
		int day = _ttoi(values.GetAt(2));
		CTime time(year, mouth, day, 0, 0, 0);
		m_dataPick.SetTime(&time);
	}
	m_projectcode.SetWindowText(context[_T("受理号")]);
	m_projectname.SetWindowText(context[_T("项目名称")]);
	m_buildcompany.SetWindowText(context[_T("建设单位")]);
	m_projectaddress.SetWindowText(context[_T("项目地点")]);
	m_implementcompany.SetWindowText(context[_T("实施单位")]);
	if(0 == context[_T("高程系统")].GetLength())
		context[_T("高程系统")] = _T("1985国家高程基准");
	if(0 == context[_T("坐标系统")].GetLength())
		context[_T("坐标系统")] = _T("92厦门坐标系");
	m_coorsystem.SetWindowText(context[_T("坐标系统")]);
	m_evelationalsystem.SetWindowText(context[_T("高程系统")]);
	m_measurecompany.SetCurSel(index);
	m_projectcode.EnableWindow(FALSE);
	m_projectname.EnableWindow(FALSE);
}

void CPipeLineJBXXDlg::initProjectJSSM()
{
	IProjectMDB pdb; MStr context = pdb.getJSSMTableInfo();
	m_cccgyjxkws.SetWindowText(context[_T("测绘成果依据的许可文书")]);
	m_yyzlqk.SetWindowText(context[_T("已有资料情况")]);
	m_tgkzqk.SetWindowText(context[_T("图根控制情况")]);
	m_xbcljbqk.SetWindowText(context[_T("细部测量基本情况")]);
	if(context[_T("地下管线的图层")].GetLength())
	{
		m_dxgxdtc.SetCheck(1); m_tqcl.SetCheck(0);
		m_dxgxdtc_tqcl.SetWindowText(context[_T("地下管线的探测")]);
	}
	else if(context[_T("天桥测量")].GetLength())
	{
		m_tqcl.SetCheck(1); m_dxgxdtc.SetCheck(0);
		m_dxgxdtc_tqcl.SetWindowText(context[_T("天桥测量")]);
	}
	m_yghxkbyzqk.SetWindowText(context[_T("与规划许可不一致情况")]);
	m_qtxysmdqk.SetWindowText(context[_T("其他需要说明的情况")]);
}

void CPipeLineJBXXDlg::saveProjectInfo()
{
	MStr context;
	m_dataPick.GetWindowText(context[_T("项目时间")]);
	m_projectcode.GetWindowText(context[_T("受理号")]);
	m_coorsystem.GetWindowText(context[_T("坐标系统")]);
	m_projectname.GetWindowText(context[_T("项目名称")]);
	m_buildcompany.GetWindowText(context[_T("建设单位")]);
	m_projectaddress.GetWindowText(context[_T("项目地点")]);
	m_measurecompany.GetWindowText(context[_T("测量单位")]);
	m_implementcompany.GetWindowText(context[_T("实施单位")]);
	m_evelationalsystem.GetWindowText(context[_T("高程系统")]);
	IProjectMDB pdb; pdb.setJBXXTableInfo(context);
}

void CPipeLineJBXXDlg::saveProjectJSSM()
{
	MStr context;
	m_cccgyjxkws.GetWindowText(context[_T("测绘成果依据的许可文书")]);
	m_yyzlqk.GetWindowText(context[_T("已有资料情况")]);
	m_tgkzqk.GetWindowText(context[_T("图根控制情况")]);
	m_xbcljbqk.GetWindowText(context[_T("细部测量基本情况")]);
	if(m_dxgxdtc.GetCheck())
		m_dxgxdtc_tqcl.GetWindowText(context[_T("地下管线的探测")]);
	else
		m_dxgxdtc_tqcl.GetWindowText(context[_T("天桥测量")]);
	m_yghxkbyzqk.GetWindowText(context[_T("与规划许可不一致情况")]);
	m_qtxysmdqk.GetWindowText(context[_T("其他需要说明的情况")]);
	IProjectMDB pdb; pdb.setJSSMTableInfo(context);
}

// CPipeLineSelectDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLineSelectDlg, CDialog)

CPipeLineSelectDlg::CPipeLineSelectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineSelectDlg::IDD, pParent)
{

}

CPipeLineSelectDlg::~CPipeLineSelectDlg()
{
}

void CPipeLineSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PIPELINESELECTLIDG, m_list);
}

BEGIN_MESSAGE_MAP(CPipeLineSelectDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PIPELINESELECTLIDG, &CPipeLineSelectDlg::DBClickColor)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINESELECTALL, &CPipeLineSelectDlg::OnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINENOTSELECT, &CPipeLineSelectDlg::OnClickedButtonSelectNone)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINEREVERSESELECT, &CPipeLineSelectDlg::OnClickedButtonReverseSelect)
END_MESSAGE_MAP()

// CPipeLineSelectDlg 消息处理程序

void CPipeLineSelectDlg::Apply()
{
	VMStr vtemp;
	CString strPath, str1, str2, str3;
	IProjectMDB pdb;
	IDataBaseOper opr;
	strPath = opr.readPathTable("当前目录");
	CString dir(strPath);
	int nCount = m_list.GetItemCount();
	dir.Append(_T("\\CG\\"));
	_tmkdir(dir + _T("道路"));
	for(int i = 0; i < nCount; ++i)
	{
		if(FALSE == m_list.GetCheck(i)) continue;
		str1 = m_list.GetItemText(i, 1);
		str2 = m_list.GetItemText(i, 2);
		str3 = m_list.GetItemText(i, 3);
		_tmkdir(dir + str1);
		MStr mtemp;
		mtemp[_T("名称")] = str1;
		mtemp[_T("简码")] = str2;
		mtemp[_T("颜色")] = str3;
		vtemp.push_back(mtemp);
	}

	//这里有个奇怪的bug,pdb直接构造的mdb为上一个项目,故手动重新设置当前mdb
	pdb.ChangeConnection(strPath + _T("\\GC\\项目信息.mdb"));
	pdb.setPipeLineTable(_T("PIPELINE"), vtemp);
}

void CPipeLineSelectDlg::LoadMdb()
{
	IDataBaseOper oper;
	IProjectMDB pdb;
	VMStr pipes = pdb.getPipeTable();
	VStr names;
	for(int idx = 0; idx < (int)pipes.size(); ++idx)
	{
		MStr pipe = pipes.at(idx);
		names.push_back(pipe[_T("名称")]);
	}
	oper.readPipelineTable(m_fnts);
	int size = (int)m_fnts.size();
	for(int idx = 0; idx < size; ++idx)
	{
		m_list.InsertItem(idx, _T(""));
		CString name = m_fnts.at(idx).strExtra;
		m_list.SetItemText(idx, 1, m_fnts.at(idx).strExtra);
		m_list.SetItemText(idx, 2, m_fnts.at(idx).strKey);
		m_list.SetItemText(idx, 3, m_fnts.at(idx).strValue);
		if(names.end() != find(names.begin(), names.end(), name))
			m_list.SetCheck(idx);
	}
}

BOOL CPipeLineSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_list.GetClientRect(&rect);
	int width = (rect.right - rect.left - 25) / 3;
	m_list.InsertColumn(0, _T(""), LVCFMT_CENTER, 25);
	m_list.InsertColumn(1, _T("管线名称"), LVCFMT_CENTER, width - 10);
	m_list.InsertColumn(2, _T("简码"), LVCFMT_CENTER, width - 10);
	m_list.InsertColumn(3, _T("管线颜色"), LVCFMT_CENTER, width - 10);
	m_list.SetColumnReadOnly(1, TRUE);
	m_list.SetColumnReadOnly(2, TRUE);
	LoadMdb(); UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

BOOL CPipeLineSelectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPipeLineSelectDlg::OnClickedButtonSelectAll()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
		m_list.SetCheck(idx, TRUE);
}

void CPipeLineSelectDlg::OnClickedButtonSelectNone()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
		m_list.SetCheck(idx, FALSE);
}

void CPipeLineSelectDlg::OnClickedButtonReverseSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL flag = m_list.GetCheck(idx);
		m_list.SetCheck(idx, !flag);
	}
}

void CPipeLineSelectDlg::DBClickColor(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int colNo = pNMListView->iSubItem;
	int rowNo = pNMListView->iItem;
	if(colNo == 3)
	{
		COLORREF color = RGB(255, 0, 0);      // 颜色对话框的初始颜色为红色  
		CColorDialog colorDlg(color);         // 构造颜色对话框，传入初始颜色值   
		if(IDOK == colorDlg.DoModal())
		{
			color = colorDlg.GetColor();
			int nColor = GetColorIndexFromRBG(color);
			strColor.Format(_T("%d"), nColor);
			m_list.SetItemText(rowNo, 2, strColor);
		}
	}
}

// CPipeLineRoadInfo 对话框

IMPLEMENT_DYNAMIC(CPipeLineRoadInfo, CDialog)

CPipeLineRoadInfo::CPipeLineRoadInfo(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineRoadInfo::IDD, pParent)
, m_oldRoadName(_T("")), m_oldPipeName(_T(""))
{

}

CPipeLineRoadInfo::~CPipeLineRoadInfo()
{
}

void CPipeLineRoadInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROADNAME, m_roadName);
	DDX_Control(pDX, IDC_EDIT_ROADPINGMIAN, m_roadkuandu);
	DDX_Control(pDX, IDC_LIST_SZPIPELINEINFO, m_szpipeinfo);
	DDX_Control(pDX, IDC_COMBO_SZPIPELINETYPE, m_szpipetype);
	DDX_Control(pDX, IDC_EDIT_ROADZONGDUANMIAN, m_roadduanmian);
	DDX_Control(pDX, IDC_EDIT_ROADHENGDUANMIAN, m_roadchangdu);
}

BEGIN_MESSAGE_MAP(CPipeLineRoadInfo, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ROADNAME, &CPipeLineRoadInfo::OnSelectedShowRoadName)
	ON_CBN_SELCHANGE(IDC_COMBO_SZPIPELINETYPE, &CPipeLineRoadInfo::OnSelectedShowPipeName)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINETYPEADD, &CPipeLineRoadInfo::OnBnClickedButtonPipelineTypeAdd)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINETYPEDEL, &CPipeLineRoadInfo::OnBnClickedButtonPipelineTypeDel)
	ON_BN_CLICKED(IDC_BUTTON_ROADPINGMIANADD, &CPipeLineRoadInfo::OnBnClickedButtonRoadPingMianAdd)
	ON_BN_CLICKED(IDC_BUTTON_ROADPINGMIANDEL, &CPipeLineRoadInfo::OnBnClickedButtonRoadPingMianDel)
	ON_BN_CLICKED(IDC_BUTTON_ROADZONGDUANMIANADD, &CPipeLineRoadInfo::OnBnClickedButtonRoadZongDuanMianAdd)
	ON_BN_CLICKED(IDC_BUTTON_ROADZONGDUANMIANDEL, &CPipeLineRoadInfo::OnBnClickedButtonRoadZongDuanMianDel)
	ON_BN_CLICKED(IDC_BUTTON_ROADHENGDUANMIANADD, &CPipeLineRoadInfo::OnBnClickedButtonRoadHengDuanMianAdd)
	ON_BN_CLICKED(IDC_BUTTON_ROADHENGDUANMIANDEL, &CPipeLineRoadInfo::OnBnClickedButtonRoadHengDuanMianDel)
END_MESSAGE_MAP()

// CPipeLineRoadInfo 消息处理程序

void CPipeLineRoadInfo::Apply()
{
	CString name; IProjectMDB pdb;
	m_roadName.GetWindowText(name);
	saveListCtrlValueRoadInfo(name);
	m_szpipetype.GetWindowText(name);
	saveListCtrlValuePipeLine(name);
	MSRoadInfoIter it1 = m_msRoadInfo.begin();
	for(; it1 != m_msRoadInfo.end(); ++it1)
	{
		GHSSJBQKInfo roadInfo = it1->second;
		pdb.setRoadInfo(it1->first, roadInfo);
	}
	MSGHSSJBQKPipeIter it2 = m_msPipeLine.begin();
	for(; it2 != m_msPipeLine.end(); ++it2)
	{
		GHSSJBQKPipe pipeInfo = it2->second;
		pdb.setRoadInfo(it2->first, it2->second);
	}
}

BOOL CPipeLineRoadInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initListCtrl();
	initComboxCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CPipeLineRoadInfo::initGHSSJBQK()
{

}

void CPipeLineRoadInfo::initListCtrl()
{
	m_roadkuandu.Init(); CRect rect;
	m_roadkuandu.GetClientRect(&rect);
	int width = (rect.right - rect.left) / 2;
	m_roadkuandu.InsertColumn(0, _T("道路设计"), LVCFMT_CENTER, width);
	m_roadkuandu.InsertColumn(1, _T("道路实测"), LVCFMT_CENTER, width);

	m_roadchangdu.Init();
	m_roadchangdu.GetClientRect(&rect);
	width = (rect.right - rect.left) / 2;
	m_roadchangdu.InsertColumn(0, _T("道路设计"), LVCFMT_CENTER, width);
	m_roadchangdu.InsertColumn(1, _T("道路实测"), LVCFMT_CENTER, width);

	m_roadduanmian.Init();
	m_roadduanmian.GetClientRect(&rect);
	width = (rect.right - rect.left) / 2;
	m_roadduanmian.InsertColumn(0, _T("道路设计"), LVCFMT_CENTER, width);
	m_roadduanmian.InsertColumn(1, _T("道路实测"), LVCFMT_CENTER, width);

	m_szpipeinfo.Init();
	m_szpipeinfo.GetClientRect(&rect);
	width = (rect.right - rect.left) / 2;
	m_szpipeinfo.InsertColumn(0, _T("管线设计"), LVCFMT_CENTER, width);
	m_szpipeinfo.InsertColumn(1, _T("管线实测"), LVCFMT_CENTER, width);
}

void CPipeLineRoadInfo::initComboxCtrl()
{
	SelectFilter sf1(8, _T("SCDLX")), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids; if(false == SelectEntitys(ids, sf1, sf2, _T("X"))) return;
	TCHAR info[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(false == ReadXdata(aname(id), _T("道路中心线名称"), 0, info)) continue;
		m_msRoadInfo[CString(info)] = GHSSJBQKInfo();
		m_roadName.AddString(info);
	}
	MSRoadInfoIter it1 = m_msRoadInfo.begin();
	IProjectMDB pdb;
	for(; it1 != m_msRoadInfo.end(); ++it1)
		pdb.getRoadInfo(it1->first, it1->second);
	m_roadName.SetCurSel(0); CString name;
	m_roadName.GetWindowText(name);
	setRoadListCtrlValue(name);
	VMStr records = pdb.getPipeTable();
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		name.Format(_T("%s"), record[_T("名称")]);
		m_msPipeLine[name] = GHSSJBQKPipe();
		m_szpipetype.AddString(name);
	}
	MSGHSSJBQKPipeIter it2 = m_msPipeLine.begin();
	for(; it2 != m_msPipeLine.end(); ++it2)
		pdb.getRoadInfo(it2->first, it2->second);
	m_szpipetype.SetCurSel(0);
	m_szpipetype.GetWindowText(name);
	setPipeListCtrlValue(name);
}

void CPipeLineRoadInfo::OnSelectedShowPipeName()
{
	CString name;
	saveListCtrlValuePipeLine(m_oldPipeName);
	m_szpipetype.GetWindowText(name);
	setPipeListCtrlValue(name);
}

void CPipeLineRoadInfo::OnSelectedShowRoadName()
{
	CString name;
	saveListCtrlValueRoadInfo(m_oldRoadName);
	m_roadName.GetWindowText(name);
	setRoadListCtrlValue(name);
}

BOOL CPipeLineRoadInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPipeLineRoadInfo::OnBnClickedButtonPipelineTypeAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_szpipeinfo.GetItemCount();
	m_szpipeinfo.InsertItem(count, _T(""));
	m_szpipeinfo.EnsureVisible(count, TRUE);
}

void CPipeLineRoadInfo::OnBnClickedButtonPipelineTypeDel()
{
	// TODO:  在此添加控件通知处理程序代码
	int item = m_szpipeinfo.GetSelectedItem();
	m_szpipeinfo.DeleteItem(item);
}

void CPipeLineRoadInfo::OnBnClickedButtonRoadPingMianAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_roadkuandu.GetItemCount();
	m_roadkuandu.InsertItem(count, _T(""));
	m_roadkuandu.EnsureVisible(count, TRUE);
}

void CPipeLineRoadInfo::OnBnClickedButtonRoadPingMianDel()
{
	// TODO:  在此添加控件通知处理程序代码
	int item = m_roadkuandu.GetSelectedItem();
	m_roadkuandu.DeleteItem(item);
}

void CPipeLineRoadInfo::setRoadListCtrlValue(const CString &name)
{
	if(name.GetLength() == 0) return;
	m_oldRoadName = name;
	GHSSJBQKInfo::RoadWidth roadWidth = m_msRoadInfo[name].roadWidth;
	m_roadkuandu.DeleteAllItems();
	int count = max(roadWidth.m_sjRoadWidth.size(),
		roadWidth.m_scRoadWidth.size());
	for(int idx = 0; idx < count; ++idx)
		m_roadkuandu.InsertItem(idx, _T(""));
	for(int idx = 0; idx < (int)roadWidth.m_sjRoadWidth.size(); ++idx)
		m_roadkuandu.SetItemText(idx, 0, roadWidth.m_sjRoadWidth[idx]);
	for(int idx = 0; idx < (int)roadWidth.m_scRoadWidth.size(); ++idx)
		m_roadkuandu.SetItemText(idx, 1, roadWidth.m_scRoadWidth[idx]);

	GHSSJBQKInfo::RoadLength roadLength = m_msRoadInfo[name].roadLength;
	m_roadchangdu.DeleteAllItems();
	count = max(roadLength.m_scRoadLength.size(),
		roadLength.m_sjRoadLength.size());
	for(int idx = 0; idx < count; ++idx)
		m_roadchangdu.InsertItem(idx, _T(""));
	for(int idx = 0; idx < (int)roadLength.m_sjRoadLength.size(); ++idx)
		m_roadchangdu.SetItemText(idx, 0, roadLength.m_sjRoadLength[idx]);
	for(int idx = 0; idx < (int)roadLength.m_scRoadLength.size(); ++idx)
		m_roadchangdu.SetItemText(idx, 1, roadLength.m_scRoadLength[idx]);

	GHSSJBQKInfo::RoadHeight roadHeight = m_msRoadInfo[name].roadHeight;
	m_roadduanmian.DeleteAllItems();
	count = max(roadHeight.m_scRoadHeight.size(),
		roadHeight.m_sjRoadHeight.size());
	for(int idx = 0; idx < count; ++idx)
		m_roadduanmian.InsertItem(idx, _T(""));
	for(int idx = 0; idx < (int)roadHeight.m_scRoadHeight.size(); ++idx)
		m_roadduanmian.SetItemText(idx, 0, roadHeight.m_sjRoadHeight[idx]);
	for(int idx = 0; idx < (int)roadHeight.m_sjRoadHeight.size(); ++idx)
		m_roadduanmian.SetItemText(idx, 1, roadHeight.m_scRoadHeight[idx]);
}

void CPipeLineRoadInfo::setPipeListCtrlValue(const CString &name)
{
	if(name.GetLength() == 0) return;
	GHSSJBQKPipe pipe = m_msPipeLine[name];
	m_oldPipeName = name;
	m_szpipeinfo.DeleteAllItems();
	int count = max(pipe.m_scPipeLine.size(), pipe.m_sjPipeLine.size());
	for(int idx = 0; idx < count; ++idx)
		m_szpipeinfo.InsertItem(idx, _T(""));
	for(int idx = 0; idx < (int)pipe.m_sjPipeLine.size(); ++idx)
		m_szpipeinfo.SetItemText(idx, 0, pipe.m_sjPipeLine[idx]);
	for(int idx = 0; idx < (int)pipe.m_scPipeLine.size(); ++idx)
		m_szpipeinfo.SetItemText(idx, 1, pipe.m_scPipeLine[idx]);
}

void CPipeLineRoadInfo::OnBnClickedButtonRoadZongDuanMianAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_roadchangdu.GetItemCount();
	m_roadchangdu.InsertItem(count, _T(""));
	m_roadchangdu.EnsureVisible(count, TRUE);
}

void CPipeLineRoadInfo::OnBnClickedButtonRoadZongDuanMianDel()
{
	// TODO:  在此添加控件通知处理程序代码
	int item = m_roadchangdu.GetSelectedItem();
	m_roadchangdu.DeleteItem(item);
}

void CPipeLineRoadInfo::OnBnClickedButtonRoadHengDuanMianAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_roadduanmian.GetItemCount();
	m_roadduanmian.InsertItem(count, _T(""));
	m_roadduanmian.EnsureVisible(count, TRUE);
}

void CPipeLineRoadInfo::OnBnClickedButtonRoadHengDuanMianDel()
{
	// TODO:  在此添加控件通知处理程序代码
	int item = m_roadduanmian.GetSelectedItem();
	m_roadduanmian.DeleteItem(item);
}

void CPipeLineRoadInfo::saveListCtrlValueRoadInfo(const CString &name)
{
	if(name.GetLength() == 0) return;
	int count = m_roadkuandu.GetItemCount();
	m_msRoadInfo.erase(name);
	for(int idx = 0; idx < count; ++idx)
	{
		CString info = m_roadkuandu.GetItemText(idx, 0);
		m_msRoadInfo[name].roadWidth.m_sjRoadWidth.push_back(info);
		info = m_roadkuandu.GetItemText(idx, 1);
		m_msRoadInfo[name].roadWidth.m_scRoadWidth.push_back(info);
	}
	count = m_roadchangdu.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		CString info = m_roadchangdu.GetItemText(idx, 0);
		m_msRoadInfo[name].roadLength.m_sjRoadLength.push_back(info);
		info = m_roadchangdu.GetItemText(idx, 1);
		m_msRoadInfo[name].roadLength.m_scRoadLength.push_back(info);
	}
	count = m_roadduanmian.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		CString info = m_roadduanmian.GetItemText(idx, 0);
		m_msRoadInfo[name].roadHeight.m_sjRoadHeight.push_back(info);
		info = m_roadduanmian.GetItemText(idx, 1);
		m_msRoadInfo[name].roadHeight.m_scRoadHeight.push_back(info);
	}
}

void CPipeLineRoadInfo::saveListCtrlValuePipeLine(const CString &name)
{
	if(name.GetLength() == 0) return;
	int count = m_szpipeinfo.GetItemCount();
	m_msPipeLine.erase(name);
	for(int idx = 0; idx < count; ++idx)
	{
		CString info = m_szpipeinfo.GetItemText(idx, 0);
		m_msPipeLine[name].m_sjPipeLine.push_back(info);
		info = m_szpipeinfo.GetItemText(idx, 1);
		m_msPipeLine[name].m_scPipeLine.push_back(info);
	}
}

// CPipeLineSelectOutputDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLineSelectOutputDlg, CDialog)

CPipeLineSelectOutputDlg::CPipeLineSelectOutputDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineSelectOutputDlg::IDD, pParent)
{

}

CPipeLineSelectOutputDlg::~CPipeLineSelectOutputDlg()
{
}

void CPipeLineSelectOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_pipelist);
}

BEGIN_MESSAGE_MAP(CPipeLineSelectOutputDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPipeLineSelectOutputDlg::OnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CPipeLineSelectOutputDlg::OnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CPipeLineSelectOutputDlg::OnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CPipeLineSelectOutputDlg::OnClickedButtonReverseSelect)
END_MESSAGE_MAP()

// CPipeLineSelectOutputDlg 消息处理程序

BOOL CPipeLineSelectOutputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pipelist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_pipelist.GetClientRect(&rect);
	int width = (rect.right - rect.left - 30);
	m_pipelist.InsertColumn(0, _T(""), LVCFMT_CENTER, 25);
	m_pipelist.InsertColumn(1, _T("管线名称"), LVCFMT_CENTER, width);
	m_pipelist.SetColumnReadOnly(1, TRUE);
	IProjectMDB oper;
	vmRecords = oper.getPipeTable();
	int size = vmRecords.size();
	for(int idx = 0; idx < size; ++idx)
	{
		MStr msTemp = vmRecords[idx];
		m_pipelist.InsertItem(idx, _T(""));
		m_pipelist.SetItemText(idx, 1, msTemp[_T("名称")]);
	}
	return true;
}

void CPipeLineSelectOutputDlg::OnClickedButtonExport()
{
	GetZPTLayers();
	CDialog::OnOK();
}

void CPipeLineSelectOutputDlg::OnClickedButtonCancel()
{
	CDialog::OnCancel();
}

void CPipeLineSelectOutputDlg::OnClickedButtonSelectAll()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_pipelist.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
		m_pipelist.SetCheck(idx, TRUE);
}

void CPipeLineSelectOutputDlg::OnClickedButtonReverseSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_pipelist.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL flag = m_pipelist.GetCheck(idx);
		m_pipelist.SetCheck(idx, !flag);
	}
}

//保存打印范围的实体
void CPipeLineSelectOutputDlg::SavePipeBlock(AcDbObjectIdArray &idArr)
{
	ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	SSToIdArr(ssSave, idArr);
	ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("打印范围"));
	idArr.append(dyfw);
}

void CPipeLineSelectOutputDlg::GetZPTLayers()
{
	vmsLayers.clear();
	int nCount = m_pipelist.GetItemCount();
	for(int i = 0; i < nCount; ++i)
	{
		CString str1 = _T("");
		if(FALSE == m_pipelist.GetCheck(i)) continue;
		str1 = m_pipelist.GetItemText(i, 1);
		vmsLayers.push_back(str1);
	}
}

void CPipeLineSelectOutputDlg::PipefilterObj(VStr &vals, const CString &strPipeExport)
{
	vals.push_back(_T("里程"));
	vals.push_back(_T("K道路中线"));
	vals.push_back(_T("jsyd红线图层"));
	vals.push_back(_T("实里程"));
	vals.push_back(_T("标注"));
	CStringArray retList;
	getLayerList(retList);
	for(int i = 0; i < retList.GetSize(); ++i)
	{
		CString strTemp = _T("");
		strTemp = retList[i];
		if(strTemp.Find(strPipeExport) > 0)//保留含有strPipeExport简码的图层
			continue;
		if(!strTemp.Find(_T("P_SC_")) || !strTemp.Find(_T("L_SC_")) || !strTemp.Find(_T("L_SJ_")) || !strTemp.Find(_T("L_SJ_")))
			vals.push_back(strTemp);
	}
}

void CPipeLineSelectOutputDlg::Apply()
{
	GetZPTLayers();
	return CDialog::OnOK();
	// 
	// 	//指定打印范围并获得范围内的实体
	// 	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	// 	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	// 	if(false == assist.GetExportRange(lb, rt)) return;
	// 	assist.GetExportRange(lb, rt, nodes);
	// 	assist.GetExportEntitys(IdArr, nodes);
	// 	CString strMC, strJM;
	// 	for(size_t i = 0; i < vmsLayers.size(); ++i)
	// 	{
	// 		XMJGExportZPT zpt;
	// 		zpt.setUndoFlag();
	// 		//acDocManager->lockDocument(curDoc()); // 锁定文档
	// 		strMC = vmsLayers.at(i)[_T("名称")];
	// 		strJM = vmsLayers.at(i)[_T("简码")];
	// 
	// 		//过滤其他图层
	// 		AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	// 		TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	// 		assist.DrawMesh(lb, rt, strMC, 1);
	// 		MVStr filter; VStr vals;
	// 		PipefilterObj(vals, strJM);
	// 		MIVStr filters;
	// 		filters[FilterLayer] = vals;
	// 		filter[_T("图层")] = vals;
	// 		assist.FilterEntitys(IdArr, filter);
	// 
	// 		//保存
	// 		CString path = assist.getExportFilePath();
	// 		path += _T("\\CG\\") + strMC + _T("\\");
	// 		CSaveBlocks blocks;	CString pathName(path);
	// 		if(pathName.Right(9) != "管线对照图.dwg")	pathName.Append(_T("管线对照图.dwg"));
	// 		blocks.setPath(pathName);
	// 		AcDbObjectIdArray idArr;
	// 		SavePipeBlock(idArr);
	// 		blocks.saveBlocks2(idArr, false);
	// 		//acDocManager->unlockDocument(curDoc());
	// 		zpt.beginUndo();
	// 	}
	// 	acutPrintf(_T("管线对照图输出完成!"));
}

BOOL CPipeLineSelectOutputDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

// CPipeLineCreateDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLineModifyProjectDlg, CDialog)

CPipeLineModifyProjectDlg::CPipeLineModifyProjectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineModifyProjectDlg::IDD, pParent)
{

}

CPipeLineModifyProjectDlg::~CPipeLineModifyProjectDlg()
{

}

void CPipeLineModifyProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PIPEPANEL, m_tab);
}

BEGIN_MESSAGE_MAP(CPipeLineModifyProjectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPipeLineModifyProjectDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PIPEPANEL, &CPipeLineModifyProjectDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()

// CPipeLineCreateDlg 消息处理程序

BOOL CPipeLineModifyProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initTabContrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CPipeLineModifyProjectDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_jbxx.ShowWindow(TRUE);
			m_road.ShowWindow(FALSE);
			m_plp.ShowWindow(FALSE);
			break;
		case 1:
			m_jbxx.ShowWindow(FALSE);
			m_road.ShowWindow(TRUE);
			m_plp.ShowWindow(FALSE);
			break;
		case 2:
			m_jbxx.ShowWindow(FALSE);
			m_road.ShowWindow(FALSE);
			m_plp.ShowWindow(TRUE);
			break;
		default:;
	}
	*pResult = 0;
}

void CPipeLineModifyProjectDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_jbxx.Apply();
	m_road.Apply();
	m_plp.Apply();
	CDialog::OnOK();
}

void CPipeLineModifyProjectDlg::initTabContrl()
{
	m_tab.InsertItem(0, _T("基本信息"));
	m_tab.InsertItem(1, _T("规划实施基本情况"));
	m_tab.InsertItem(2, _T("管线信息"));
	CAcModuleResourceOverride myResources0;//声明该对象用来解决资源冲突问题
	m_jbxx.Create(IDD_DIALOG_PIPELINEJBXX, &m_tab);
	CAcModuleResourceOverride myResources1;//声明该对象用来解决资源冲突问题
	m_road.Create(IDD_DIALOG_PIPELINEROADINFO, &m_tab);
	CAcModuleResourceOverride myResources2;//声明该对象用来解决资源冲突问题
	m_plp.Create(IDD_DIALOG_PIPELINESELECT, &m_tab);
	CRect rect; m_tab.GetClientRect(&rect);
	rect.top += 20; rect.bottom += 20; rect.right += 40;
	m_jbxx.MoveWindow(&rect);
	m_road.MoveWindow(&rect);
	m_plp.MoveWindow(&rect);
	m_jbxx.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
}

// CPipeLineOutputPartDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLineOutputPartDlg, CDialog)

CPipeLineOutputPartDlg::CPipeLineOutputPartDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineOutputPartDlg::IDD, pParent)
{

}

CPipeLineOutputPartDlg::~CPipeLineOutputPartDlg()
{

}

void CPipeLineOutputPartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PLOUTPUTPART, m_list);
}

BEGIN_MESSAGE_MAP(CPipeLineOutputPartDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPipeLineOutputPartDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPipeLineOutputPartDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CPipeLineOutputPartDlg 消息处理程序

void CPipeLineOutputPartDlg::Apply()
{
	int len = m_list.GetItemCount();
	for(int idx = 0; idx < len; ++idx)
	{
		if(m_list.GetCheck(idx) == FALSE) continue;
		m_export.push_back(m_list.GetItemText(idx, 1));
	}
}

VStr CPipeLineOutputPartDlg::getExportPart()
{
	return m_export;
}

BOOL CPipeLineOutputPartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	IDataBaseOper oper;
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_list.GetClientRect(&rect);
	MStr filter; filter[_T("关键字")] = _T("成果报告");
	VStr values;
	oper.readDataBase(_T("JGCommon"), _T("属性值"), filter, values);
	//MStr format = oper.readOutputFormatTable(_T("道路管线竣工"));
	int width = rect.right - rect.left;
	m_list.InsertColumn(0, _T("是否输出"), LVCFMT_CENTER, 60);
	m_list.InsertColumn(1, _T("单项报告"), LVCFMT_CENTER, width - 60);
	for(int idx = 0; idx < (int)values.size(); ++idx)
	{
		m_list.InsertItem(idx, _T(""));
		m_list.SetItemText(idx, 1, values[idx]);
	}
	m_list.SetColumnReadOnly(1, TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CPipeLineOutputPartDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		if(TRUE != m_list.GetCheck(idx)) continue;
		m_export.push_back(m_list.GetItemText(idx, 1));
	}
	CDialog::OnOK();
}

void CPipeLineOutputPartDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

// CPipeLineOutputDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLineOutputDlg, CDialog)

CPipeLineOutputDlg::CPipeLineOutputDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineOutputDlg::IDD, pParent)
{

}

CPipeLineOutputDlg::~CPipeLineOutputDlg()
{
}

void CPipeLineOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PIPELINEPANEL, m_tab);
}

BEGIN_MESSAGE_MAP(CPipeLineOutputDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPipeLineOutputDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PIPELINEPANEL, &CPipeLineOutputDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()

// CPipeLineOutputDlg 消息处理程序

BOOL CPipeLineOutputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_tab.InsertItem(0, _T("道路信息"));
	m_tab.InsertItem(1, _T("管线输出"));
	m_road.Create(IDD_DIALOG_PIPELINEROADINFO, &m_tab);
	m_sout.Create(IDD_DIALOG_PIPELINESELECTOUTPUT, &m_tab);
	CRect rect; m_tab.GetClientRect(&rect);
	rect.top += 20; rect.bottom += 20; rect.right += 40;
	m_road.MoveWindow(&rect);
	m_sout.MoveWindow(&rect);
	m_road.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CPipeLineOutputDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_road.ShowWindow(TRUE);
			m_sout.ShowWindow(FALSE);
			break;
		case 1:
			m_road.ShowWindow(FALSE);
			m_sout.ShowWindow(TRUE);
			break;
		default:;
	}
	*pResult = 0;
}

void CPipeLineOutputDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnOK();
	m_road.Apply();
	m_sout.Apply();
	//CDialog::OnOK();
}

// CPipeLineCreateProjectDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLineCreateProjectDlg, CDialog)

CPipeLineCreateProjectDlg::CPipeLineCreateProjectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLineCreateProjectDlg::IDD, pParent)
{

}

CPipeLineCreateProjectDlg::~CPipeLineCreateProjectDlg()
{

}

void CPipeLineCreateProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PIPELINECREATETAB, m_tab);
}

BEGIN_MESSAGE_MAP(CPipeLineCreateProjectDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PIPELINECREATETAB, &CPipeLineCreateProjectDlg::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDOK, &CPipeLineCreateProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPipeLineCreateProjectDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CPipeLineCreateProjectDlg 消息处理程序

BOOL CPipeLineCreateProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_tab.InsertItem(0, _T("基本信息"));
	m_tab.InsertItem(1, _T("作业管线"));
	m_pconfig.Create(IDD_DIALOG_PipiConfig, &m_tab);
	m_pselect.Create(IDD_DIALOG_PIPELINESELECT, &m_tab);
	CRect rect; m_tab.GetClientRect(&rect);
	rect.top += 20; rect.bottom += 20; rect.right += 40;
	m_pconfig.MoveWindow(&rect);
	m_pselect.MoveWindow(&rect);
	m_pconfig.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CPipeLineCreateProjectDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_pconfig.ShowWindow(TRUE);
			m_pselect.ShowWindow(FALSE);
			break;
		case 1:
			m_pconfig.ShowWindow(FALSE);
			m_pselect.ShowWindow(TRUE);
			break;
		default:;
	}
	*pResult = 0;
}

void CPipeLineCreateProjectDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_pconfig.Apply();
	m_pselect.Apply();
	CDialog::OnOK();
}

void CPipeLineCreateProjectDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

// CSubPipeLinePropertyDlg 对话框

IMPLEMENT_DYNAMIC(CSubPipeLinePropertyDlg, CDialog)

CSubPipeLinePropertyDlg::CSubPipeLinePropertyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSubPipeLinePropertyDlg::IDD, pParent)
{

}

CSubPipeLinePropertyDlg::~CSubPipeLinePropertyDlg()
{

}

void CSubPipeLinePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PIPELINEPROPERTY, m_list);
	DDX_Control(pDX, IDC_COMBO_PIPELINE, m_combox);
	DDX_Control(pDX, IDC_COMBO_PIPELINEGXLX, m_gxlx);
	DDX_Control(pDX, IDC_RADIO_LOCATION_PL, m_checkpl);
	DDX_Control(pDX, IDC_RADIO_LOCATIONPP, m_checkpp);
}

BEGIN_MESSAGE_MAP(CSubPipeLinePropertyDlg, CDialog)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_PIPELINE, &CSubPipeLinePropertyDlg::OnSelectShowPipeLine)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PIPELINEPROPERTY, &CSubPipeLinePropertyDlg::OnClickedSelectRow)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINEREFLASHTABLE, &CSubPipeLinePropertyDlg::OnClickedButtonPipelineReflashTable)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINECORRELATIONENTITY, &CSubPipeLinePropertyDlg::OnClickedButtonPipelineCorrelationEntity)
	ON_CBN_SELCHANGE(IDC_COMBO_PIPELINEGXLX, &CSubPipeLinePropertyDlg::OnCbnSelchangeComboPipelinegxlx)
END_MESSAGE_MAP()

// CSubPipeLinePropertyDlg 消息处理程序

BOOL CSubPipeLinePropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initListCtrl();
	initComboxCtrl();
	initListCtrlValue();
	initComboxCtrlGXLX();
	CButton *wnd = (CButton *)GetDlgItem(IDC_RADIO_LOCATION_PL);
	//wnd->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CSubPipeLinePropertyDlg::initListCtrl()
{
	m_list.Init(TRUE, 16); ATTRFIELD_ARRAY table;
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_OWNERDRAWFIXED | LVS_EX_DOUBLEBUFFER);
	g_attribute.get_gistable_columns(_T("PIPL"), table); VInt reads;
	for(int idx = 0; idx < (int)table.size(); ++idx)
	{
		m_list.InsertColumn(idx, table[idx].text, LVCFMT_CENTER, 80);
		if(true == table[idx].bMust) reads.push_back(idx);
	}
	for(int idx = 0; idx < (int)reads.size(); ++idx)
		m_list.SetColumnReadOnly(reads[idx], TRUE);
}

void CSubPipeLinePropertyDlg::initComboxCtrl()
{
	IDataBaseOper oper;
	CStringArray layers;
	getLayerList(layers);
	m_combox.InsertString(0, _T("所有管线"));
	for(int idx = 0; idx < layers.GetSize(); ++idx)
	{
		CString layer = layers.GetAt(idx);
		if(layer.Find(_T("L_SC")) == -1) continue;
		int pos = layer.ReverseFind(_T('_'));
		layer = layer.Mid(pos + 1);
		MStr filter; filter[_T("关键字")] = layer;
		CString name = oper.readPipeLineTable(_T("附加值"), filter);
		m_combox.AddString(name);
	}
	int pos = m_combox.FindString(0, _T("所有管线"));
	m_combox.SetCurSel(pos);
}

void CSubPipeLinePropertyDlg::initListCtrlValue()
{
	OnClickedButtonPipelineReflashTable();
}

void CSubPipeLinePropertyDlg::reflashPLProperty()
{
	TCHAR info[255] = {0};
	m_list.DeleteAllItems(); VStr field, alias; VInt fieldtype; int row = -1;
	g_attribute.get_gistable_columns(_T("PIPL"), field, fieldtype, alias);
	IDataBaseOper oper;
	CString strTZD = oper.readCommonTable(_T("管线点特征点"));
	CString strFSW = oper.readCommonTable(_T("管线点附属物"));
	CString strCZ = oper.readCommonTable(_T("管线点材质"));
	CString strMSFS = oper.readCommonTable(_T("管线点埋设方式"));
	CString strLX(_T("0;1"));
	for(map<CString, AcDbObjectId, CmpCstr>::iterator it = m_sid.begin(); it != m_sid.end(); ++it)
	{
		AcDbObjectId id = it->second;
		int fieldcount = GetXDataCount(aname(id), field[1]);
		for(int fc = 0; fc < fieldcount; ++fc)
		{
			m_list.InsertItem(++row, it->first);
			for(int indx = 1; indx < (int)field.size(); ++indx)
			{
				if(false == READMulXData(id, field[indx], info, fieldtype[indx] % 10, fc)) continue;
				if(fieldtype[indx] % 10 == 3 && abs(_tstof(info)) < EPS) continue;
				CString value(info); int pos;
				if(alias[indx].CompareNoCase(_T("特征点")) == 0)
					m_list.SetComboColumns(indx, strTZD);
				else if(alias[indx].CompareNoCase(_T("附属物")) == 0)
					m_list.SetComboColumns(indx, strFSW);
				else if(alias[indx].CompareNoCase(_T("材质")) == 0)
					m_list.SetComboColumns(indx, strCZ);
				else if(alias[indx].CompareNoCase(_T("埋设方式")) == 0)
					m_list.SetComboColumns(indx, strMSFS);
				if(-1 != (pos = value.Find(_T("."))))
					m_list.SetItemText(row, indx, value.Mid(0, pos + 4));
				else
					m_list.SetItemText(row, indx, info);
			}
		}
	}
}

void CSubPipeLinePropertyDlg::selectPoint(const AcDbObjectIdArray &ids)
{
	if(m_checkpl.GetCheck() == 0) return;
	if(m_checkpp.GetCheck() == 0) return;
	TCHAR info[255] = {0}; int count = 0;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		count += (true == ReadXdata(aname(id), _T("1"), 0, info)) ? 1 : 0;
	}
	if(count > 1)
	{
		MessageBox(_T("请选择单个管点"), _T("错误提示")); return;
	}
	else if(count == 0) return;
	count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		CString gdxh = m_list.GetItemText(idx, 1);
		if(gdxh.CompareNoCase(info) != 0) continue;
		m_list.EnsureVisible(idx, TRUE);
		m_list.setSelectItem(idx);
	}
}

void CSubPipeLinePropertyDlg::initComboxCtrlGXLX()
{
	IProjectMDB pdb;
	VMStr pipes = pdb.getPipeTable();
	IDataBaseOper oper;
	oper.readPipelineTable(m_plts);
	m_gxlx.InsertString(0, _T("选择类型"));
	for(int idx = 0; idx < (int)pipes.size(); ++idx)
	{
		MStr pipe = pipes[idx];
		m_gxlx.AddString(pipe[_T("名称")]);
	}
	int pos = m_gxlx.FindString(0, _T("选择类型"));
	m_gxlx.SetCurSel(pos);
}

void CSubPipeLinePropertyDlg::drawFinish()
{
	int count = m_list.GetItemCount();
	CString xh = m_list.GetItemText(count - 1, 0);
	VStr field, alias; VInt fieldtype; MSInt record; CLockManager lock;
	g_attribute.get_gistable_columns(_T("PIPL"), field, fieldtype, alias);
	for(int idx = 0; idx < count; ++idx)
	{
		CString xh = m_list.GetItemText(idx, 0);
		AcDbObjectId id = m_sid[xh];
		if(record.end() == record.find(xh)) record[xh] = 0;
		for(int indx = 0; indx < (int)field.size(); ++indx)
			ADDMulXData(id, field[indx], m_list.GetItemText(idx, indx), fieldtype[indx] % 10, record[xh]);
		record[xh]++;
	}
	drawLineRemark();
}

void CSubPipeLinePropertyDlg::OnClickedSelectRow(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int rowNo = pNMListView->iItem;
	int colNo = pNMListView->iSubItem;
	::SetFocus(acedGetAcadDwgView()->GetSafeHwnd());
	ATTRFIELD_ARRAY table;
	g_attribute.get_gistable_columns(_T("PIPL"), table);
	if(true == table[colNo].bMust)
	{
		ads_point ptMin, ptMax;
		if(m_checkpl.GetCheck() == 1)
		{
			ads_name ssname; ads_ssadd(NULL, NULL, ssname);
			CString xh = m_list.GetItemText(rowNo, 0);
			AcDbObjectId id = m_sid[xh];
			ads_ssadd(aname(id), ssname, ssname);
			GetEntsExtent(ssname, ptMin, ptMax, 2.0);
			ads_sssetfirst(ssname, NULL);
			ads_ssfree(ssname);
		}
		else if(m_checkpp.GetCheck() == 1)
		{
			double x = 0.0, y = 0.0;
			for(int idx = 0; idx < (int)table.size(); ++idx)
			{
				CString name(table[idx].text);
				if(name.CompareNoCase(_T("X坐标")) == 0)
					x = _tstof(m_list.GetItemText(rowNo, idx));
				else if(name.CompareNoCase(_T("Y坐标")) == 0)
					y = _tstof(m_list.GetItemText(rowNo, idx));
			}
			ptMin[X] = x - 2.0; ptMin[Y] = y - 2.0;
			ptMax[X] = x + 2.0; ptMax[Y] = y + 2.0;
		}
		else return;
		ZoomWindow(ptMin, ptMax);
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
	}
}

void CSubPipeLinePropertyDlg::addPoint(const AcGePoint2d &pt, const AcDbObjectId &id, bool isNew, CString olddh)
{
	int indx = 0; CString index; TCHAR szxh[24] = {0};
	int count = m_list.GetItemCount();
	CString temp = m_list.GetItemText(count - 1, 0);
	m_sid[temp] = id;
	if(isNew == true)
	{
		m_list.SetItemText(count - 1, 2, olddh);

		return;
	}
	int gdxh = 0; CXRecordOperate opera; MStr filter;
	CString gdlx; m_gxlx.GetWindowText(gdlx); filter[_T("附加值")] = gdlx;
	IDataBaseOper oper; gdlx = oper.readPipeLineTable(_T("关键字"), filter);
	opera.ReadIntegerRecord(_T("XMJG"), gdlx + _T("_XH"), gdxh);
	ATTRFIELD_ARRAY table; g_attribute.get_gistable_columns(_T("PIPL"), table);
	if(id != AcDbObjectId::kNull)
	{
		if(true == ReadXdata(aname(id), table[0].name, 0, szxh))
			index.Format(_T("%s"), szxh);
		else
			index = m_list.GetItemText(count - 1, 0);
	}
	else
	{
		int xh = _ttoi(m_list.GetItemText(count - 1, 0));
		index.Format(_T("%d"), xh + 1);
	}
	m_list.InsertItem(count, index);
	for(int idx = 0; idx < (int)table.size(); ++idx)
	{
		if(_tcscmp(table[idx].text, _T("点号")) == 0)
		{
			CString dh; dh.Format(_T("%s%d"), gdlx, ++gdxh);
			m_list.SetItemText(count, idx, dh);
			if(id != AcDbObjectId::kNull)
				m_list.SetItemText(count - 1, idx + 1, dh);
		}
		if(_tcscmp(table[idx].text, _T("X坐标")) == 0)
		{
			CString xval; xval.Format(_T("%.3lf"), pt.x);
			m_list.SetItemText(count, idx, xval);
		}
		else if(_tcscmp(table[idx].text, _T("Y坐标")) == 0)
		{
			CString yval; yval.Format(_T("%.3lf"), pt.y);
			m_list.SetItemText(count, idx, yval);
		}
	}
	opera.AddIntegerRecord(_T("XMJG"), gdlx + _T("_XH"), gdxh);
}

PipeLineTable CSubPipeLinePropertyDlg::getCurrentPipeLineType()
{
	OnClickedButtonPipelineReflashTable();
	CString type; m_gxlx.GetWindowText(type);
	for(int idx = 0; idx < (int)m_plts.size(); ++idx)
		if(type.CompareNoCase(m_plts[idx].strExtra) == 0) return m_plts[idx];
	return PipeLineTable();
}

BOOL CSubPipeLinePropertyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSubPipeLinePropertyDlg::OnCbnSelchangeComboPipelinegxlx()
{
	// TODO:  在此添加控件通知处理程序代码
	::SetFocus(acedGetAcadDwgView()->GetSafeHwnd());
}

void CSubPipeLinePropertyDlg::OnSelectShowPipeLine()
{
	CString name; m_list.DeleteAllItems();
	m_combox.GetWindowText(name);
	if(name.CompareNoCase(_T("所有管线")) == 0)
	{
		m_combox.Clear();
		OnClickedButtonPipelineReflashTable(); return;
	}
	SelectFilter sf(8, name);
	AcDbObjectIdArray ids;
	TCHAR gxxl[25] = {0}; m_sid.clear();
	if(false == SelectEntitys(ids, sf, _T("X"))) return;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(false == ReadXdata(aname(id), _T("GXXH"), 0, gxxl)) continue;
		m_sid[gxxl] = id;
	}

	zoomWindow(ids);
	reflashPLProperty();
}

void CSubPipeLinePropertyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;
	m_list.GetWindowRect(&rect);
	rect.left = 0;	rect.right = cx;
	rect.top = 30; rect.bottom = cy;
	m_list.MoveWindow(rect);
	// TODO:  在此处添加消息处理程序代码
}

void CSubPipeLinePropertyDlg::drawLineRemark()
{
	TCHAR info[255] = {0};
	TCHAR value[255] = {0}; TCHAR layer[25] = {0};
	map<CString, AcDbObjectId, CmpCstr>::iterator it = m_sid.begin();
	for(; it != m_sid.end(); ++it)
	{
		AcDbObjectId id = it->second; GetEntLayer(id, layer);
		int color = getcolor(aname(id));
		int count = GetXDataCount(aname(id), _T("GDZJ"));
		if(-1 == count)
		{
			AcGePoint2dArray node; GetPlList(aname(id), node);
			for(int index = 0; index < node.length(); ++index)
			{
				if(false == READMulXData(id, _T("DH"), value, 0, index)) continue;
				AcGePoint3d pos(node.at(index).x, node.at(index).y, 0.0);
				AcDbObjectId tid = DrawText(pos, value, _T("BL"), _T(""), 1.0);
				AddXdata(aname(tid), _T("1"), 0, value);
				setlayer(aname(tid), layer); setcolor(aname(tid), color);
				READMulXData(id, _T("DMGC"), value, 3, index);
				AddXdata(aname(tid), _T("2"), 0, value);
				READMulXData(id, _T("GXGC"), value, 3, index);
				AddXdata(aname(tid), _T("3"), 0, value);
				CString handle = GetHandleByObject(tid);
				ADDMulXData(id, _T("GDZJ"), handle, 0, index);
				_stprintf(value, _T("%d"), index);
				AddXdata(aname(tid), _T("4"), 0, value);
			}
		}
		else
		{
			for(int idx = 0; idx < count; ++idx)
			{
				READMulXData(id, _T("GDZJ"), value, 0, idx);
				AcDbObjectId tid = GetObjectByHandle(value);
				READMulXData(id, _T("DH"), value, 0, idx);
				SetEntText(aname(tid), value);
			}
		}
	}
}

void CSubPipeLinePropertyDlg::OnClickedButtonPipelineReflashTable()
{
	// TODO:  在此添加控件通知处理程序代码
	int pos = m_combox.FindString(0, _T("所有管线"));
	m_combox.SetCurSel(pos);
	m_list.DeleteAllItems(); m_sid.clear(); CLockManager lock;
	SelectFilter sf(RTDXF0, _T("*POLYLINE")); TCHAR info[255] = {0};
	AcDbObjectIdArray ids; if(false == SelectEntitys(ids, sf, _T("X"))) return;
	zoomWindow(ids); AcDbObjectIdArray lids;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids[idx];
		if(false == ReadXdata(aname(id), _T("GXXH"), 0, info)) continue;
		m_sid[info] = id; GetStdm(aname(id), info);
		for(int index = 0; index < (int)m_plts.size(); ++index)
		{
			if(m_plts[index].strGXBM.CompareNoCase(info) != 0) continue;
			CString layer = m_plts[index].strGXTC;
			if(-1 == m_combox.FindString(0, layer))
				m_combox.AddString(m_plts[index].strGXTC); break;
		}
		lids.append(id);
	}
	reflashPLProperty();
}

void CSubPipeLinePropertyDlg::OnClickedButtonPipelineCorrelationEntity()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount(); CString index;
	VStr field, alias; VInt fieldtype; MSInt record; CLockManager lock;
	g_attribute.get_gistable_columns(_T("PIPL"), field, fieldtype, alias);
	for(int idx = 0; idx < count; ++idx)
	{
		index = m_list.GetItemText(idx, 0);
		if(record.end() == record.find(index)) record[index] = 0;
		AcDbObjectId id = m_sid[index];
		for(int indx = 1; indx < (int)field.size(); ++indx)
			ADDMulXData(id, field[indx], m_list.GetItemText(idx, indx), fieldtype[indx] % 10, record[index]);
		record[index]++;
	}
}

// CSubPipePointPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CSubPipePointPropertyDlg, CDialog)

CSubPipePointPropertyDlg::CSubPipePointPropertyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSubPipePointPropertyDlg::IDD, pParent)
{

}

CSubPipePointPropertyDlg::~CSubPipePointPropertyDlg()
{
}

void CSubPipePointPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PIPEPOINTPROPERTY, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_CHECK_MATCHPIPEPOINT, m_match);
	DDX_Control(pDX, IDC_CHECK_MATCHPIPL, m_matchpl);
}

BEGIN_MESSAGE_MAP(CSubPipePointPropertyDlg, CDialog)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSubPipePointPropertyDlg::OnSelectShowPipePoint)
	ON_BN_CLICKED(IDC_CHECK_MATCHPIPL, &CSubPipePointPropertyDlg::OnBnClickedCheckMatchPipeLine)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PIPEPOINTPROPERTY, &CSubPipePointPropertyDlg::OnClickedOnSelectRow)
	ON_BN_CLICKED(IDC_CHECK_MATCHPIPEPOINT, &CSubPipePointPropertyDlg::OnBnClickedCheckMatchPipePoint)
	ON_BN_CLICKED(IDC_BUTTON_PIPEPOINTREASSOCIATE, &CSubPipePointPropertyDlg::OnBnClickedButtonPipepointreassociate)
	ON_BN_CLICKED(IDC_BUTTON_PIPEPOINTFLUSHAPROPERTY, &CSubPipePointPropertyDlg::OnBnClickedButtonPipepointflushaproperty)
END_MESSAGE_MAP()

// CSubPipePointPropertyDlg 消息处理程序

BOOL CSubPipePointPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initListCtrl();
	initComboxCtrl();
	initListCtrlValue();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CSubPipePointPropertyDlg::selectPoint(const AcDbObjectIdArray &ids)
{
	if(m_match.GetCheck() == 0) return;
	if(m_matchpl.GetCheck() == 0) return;
	TCHAR info[255] = {0}; int count = 0;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		count += (true == ReadXdata(aname(id), _T("1"), 0, info)) ? 1 : 0;
	}
	if(count > 1)
	{
		MessageBox(_T("请选择单个管点"), _T("错误提示")); return;
	}
	else if(count == 0) return;
	count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		CString gdxh = m_list.GetItemText(idx, 1);
		if(gdxh.CompareNoCase(info) != 0) continue;
		m_list.EnsureVisible(idx, TRUE);
		m_list.setSelectItem(idx);
	}
}

void CSubPipePointPropertyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;
	m_list.GetWindowRect(&rect);
	rect.left = 0;	rect.right = cx;
	rect.top = 30; rect.bottom = cy;
	m_list.MoveWindow(rect);
	// TODO:  在此处添加消息处理程序代码
}

void CSubPipePointPropertyDlg::initListCtrl()
{
	m_list.Init(TRUE, 20); CString val; ATTRFIELD_ARRAY table;
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_OWNERDRAWFIXED | LVS_EX_DOUBLEBUFFER);
	g_attribute.get_gistable_columns(_T("PIPP"), table); VInt reads;
	for(int idx = 0; idx < (int)table.size(); ++idx)
	{
		m_list.InsertColumn(idx, table[idx].text, LVCFMT_CENTER, 80);
		if(true == table[idx].bMust) reads.push_back(idx);
	}
	for(int idx = 0; idx < (int)reads.size(); ++idx)
		m_list.SetColumnReadOnly(reads[idx], TRUE);
	// 	for(int idx = 0; idx < alias.size(); ++idx)
	// 	{
	// 		m_list.InsertColumn(idx, alias[idx], LVCFMT_CENTER, 80);
	// 		if(fieldtype[idx] > 9)
	// 			m_list.SetColumnReadOnly(idx, TRUE);
	// 	}
	/*m_list.InsertColumn(idx++, _T("点号"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("连接点"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("特征点"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("附属物"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("X坐标"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("Y坐标"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("地面高程"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("管线高程"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("管径"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("套管尺寸"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("材质"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("压力"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("电缆根数"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("管孔数"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("埋设方式"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("埋设日期"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("道路名称"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(idx++, _T("备注"), LVCFMT_CENTER, 80);
	*/
	// 	m_list.SetColumnReadOnly(0, TRUE);
	// 	m_list.SetColumnReadOnly(3, TRUE);
	// 	m_list.SetColumnReadOnly(4, TRUE);
	// 	m_list.SetColumnReadOnly(5, TRUE);
	// 	m_list.SetColumnReadOnly(6, TRUE);
}

void CSubPipePointPropertyDlg::initComboxCtrl()
{
	IDataBaseOper oper;
	oper.readPipelineTable(m_plts);
	CStringArray layers;
	getLayerList(layers);
	m_combox.AddString(_T("所有管点"));
	for(int idx = 0; idx < layers.GetSize(); ++idx)
	{
		CString layer = layers.GetAt(idx);
		if(layer.Find(_T("P_SC")) == -1) continue;
		int pos = layer.ReverseFind(_T('_'));
		layer = layer.Mid(pos + 1);
		MStr filter; filter[_T("关键字")] = layer;
		CString name = oper.readPipeLineTable(_T("附加值"), filter);
		m_combox.AddString(name);
	}
	int pos = m_combox.FindString(0, _T("所有管点"));
	m_combox.SelectString(0, _T("所有管点")); return;
}

bool SplitPipeNum(CString str, CString &strPre, CString &strNum, CString &strPost)
{
	int nlen = _tcslen(str);
	bool bPre = true;
	for(int i = 0; i < nlen; ++i)
	{
		int nT = str.GetAt(i);
		if(bPre && (nT >= 'a'&&nT <= 'z' || nT >= 'A'&&nT <= 'Z'))
			strPre += str.GetAt(i);
		else if(nT >= '0'&&nT <= '9')
		{
			strNum += str.GetAt(i);
			bPre = false;
		}
		else if(nT >= 'a'&&nT <= 'z' || nT >= 'A'&&nT <= 'Z')
			strPost += str.GetAt(i);
	}
	return true;
}

bool bSort(pip pip1, pip pip2)
{
	CString strPre1 = _T(""), strPost1 = _T(""), strNum1 = _T(""), strNum2 = _T(""), strPre2 = _T(""), strPost2 = _T("");
	bool bSuc = true;
	bSuc = SplitPipeNum(pip1.strNum, strPre1, strNum1, strPost1);
	bSuc = SplitPipeNum(pip2.strNum, strPre2, strNum2, strPost2);
	int n1 = _tstoi(strNum1), n2 = _tstoi(strNum2);
	if(_tcscmp(strPre1, strPre2))
		return strPre1 < strPre2;
	else if(n1 != n2)
	{
		if(n1 < n2)
			return true;
		else
			return false;
	}
	else if(_tcscmp(strPost1, strPost2))
		return strPost1 < strPost2;
	else
		return true;
}

void CSubPipePointPropertyDlg::initListCtrlValue()
{
	m_list.DeleteAllItems(); m_sid.clear(); CLockManager lock;
	SelectFilter sf(RTDXF0, _T("*POLYLINE"));
	AcDbObjectIdArray ids; if(false == SelectEntitys(ids, sf, _T("X"))) return;
	zoomWindow(ids); IDataBaseOper oper;  ATTRFIELD_ARRAY table;
	CString strCZ = oper.readCommonTable(_T("管线点材质"));
	CString strMSFS = oper.readCommonTable(_T("管线点埋设方式"));
	int length = ids.length(); TCHAR info[255] = {0};
	g_attribute.get_gistable_columns(_T("PIPP"), table);
	for(int idx = 0; idx < length; ++idx)
	{
		AcDbObjectId id = ids[idx];
		if(false == ReadXdata(aname(id), _T("GXXH"), 0, info)) continue;
		m_sid[info] = id;
	}
	map<CString, AcDbObjectId, CmpCstr>::iterator it = m_sid.begin();
	int row = -1; CString value;
	for(; it != m_sid.end(); ++it)
	{
		CString gxxh(it->first);
		AcDbObjectId id = it->second;
		int count = GetXDataCount(aname(id), table[1].name);
		for(int indx = 0; indx < count; ++indx)
		{
			m_list.InsertItem(++row, gxxh);
			for(int idx = 1; idx < (int)table.size(); ++idx)
			{
				if(false == READMulXData(id, table[idx].name, info, table[idx].type % 10, indx)) continue;
				if(table[idx].type % 10 == 3 && abs(_tstof(info)) < EPS) continue;
				CString value, value1(info);; int pos = -1;
				value.Format(_T("%s"), table[idx].text);
				if(value.CompareNoCase(_T("设计材质")) == 0)
					m_list.SetComboColumns(idx, strCZ);
				else if(value.CompareNoCase(_T("设计埋设方式")) == 0)
					m_list.SetComboColumns(idx, strMSFS);
				if(-1 != (pos = value1.Find(_T("."))))
					m_list.SetItemText(row, idx, value1.Mid(0, pos + 4));
				else
					m_list.SetItemText(row, idx, info);
			}
		}
	}
	return;
}

void CSubPipePointPropertyDlg::middleEntity(const AcDbObjectId &id)
{
	if(!id.isValid()) return;
	ads_point ptmin, ptmax;
	GetEntExtent(aname(id), ptmin, ptmax, 2.0);
	ZoomWindow(ptmin, ptmax);
	ads_name ssname;
	ads_ssadd(NULL, NULL, ssname);
	ads_ssadd(aname(id), ssname, ssname);
	ads_sssetfirst(ssname, NULL);
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	ads_ssfree(ssname);
}

void CSubPipePointPropertyDlg::OnClickedOnSelectRow(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int rowNo = pNMListView->iItem;
	int colNo = pNMListView->iSubItem;
	ATTRFIELD_ARRAY table; g_attribute.get_gistable_columns(_T("PIPP"), table);
	if(false == table[colNo].bMust) return;
	CString gxxh = m_list.GetItemText(rowNo, 0);
	middleEntity(m_sid[gxxh]);
	::SetFocus(acedGetAcadDwgView()->GetSafeHwnd());
	if(m_match.GetCheck() == 1)
		matchingPipePoint(rowNo);
	if(m_matchpl.GetCheck() == 1)
		matchingPipeLine(rowNo);
}

void CSubPipePointPropertyDlg::matchingPipePoint(int row)
{
	double x = 0.0, y = 0.0; ads_point ptMin, ptMax;
	ATTRFIELD_ARRAY table; g_attribute.get_gistable_columns(_T("PIPP"), table);
	for(int idx = 0; idx < (int)table.size(); ++idx)
	{
		CString name(table[idx].text);
		if(name.CompareNoCase(_T("X坐标")) == 0)
			x = _tstof(m_list.GetItemText(row, idx));
		else if(name.CompareNoCase(_T("Y坐标")) == 0)
			y = _tstof(m_list.GetItemText(row, idx));
	}
	ptMin[X] = x - 2.0; ptMin[Y] = y - 2.0;
	ptMax[X] = x + 2.0; ptMax[Y] = y + 2.0;
	ZoomWindow(ptMin, ptMax);
	TCHAR info1[255] = {0}; TCHAR info2[255] = {0};
	SYPSampleJJG jjg; AcGePoint3d pos; AcDbObjectId id;
	SYPSampleJJG::DrawType dt; CLockManager lock;
	if(SYPSampleJJG::SelNull == (dt = jjg.selectPoint(pos, id))) return;
	CString gxxh = m_list.GetItemText(row, 0);
	AcDbObjectId lid = m_sid[gxxh];
	if(dt == SYPSampleJJG::SelEntity)
	{
		AcDbObjectId group;
		CreateGroup(aname(lid), group);
		AddEntToGroup(id, group);
	}
	int isMatch = 0;
	for(int idx = 0; idx < (int)table.size(); ++idx)
	{
		CString name(table[idx].text);
		if(name.CompareNoCase(_T("设计X坐标")) == 0)
		{
			name.Format(_T("%.3lf"), pos.x);
			m_list.SetItemText(row, idx, name);
			isMatch++;
		}
		else if(name.CompareNoCase(_T("设计Y坐标")) == 0)
		{
			name.Format(_T("%.3lf"), pos.y);
			m_list.SetItemText(row, idx, name);
			isMatch++;
		}
	}
	acutPrintf(_T("\n 匹配特征点%s。\n"), isMatch == 2 ? _T("成功") : _T("失败"));
}

void CSubPipePointPropertyDlg::matchingPipeLine(int row)
{
	ads_point pt; ads_name ent;  CLockManager lock; bool isMatch = false;
	if(RTNORM == ads_entsel(_T("\n 请选择设计相应的管线:"), ent, pt))
	{
		CString gxxh = m_list.GetItemText(row, 0);
		AcDbObjectId lid = m_sid[gxxh];
		AcDbObjectId group;
		TCHAR layer[25] = {0};
		TCHAR info[25] = {0};
		CreateGroup(aname(lid), group);
		AddEntToGroup(ent, group);
		GetEntLayer(lid, layer);
		_stprintf(info, _T("SJ_%s"), layer);
		setlayer(ent, info); isMatch = true;
	}
	acutPrintf(_T("\n 匹配设计管线成功。\n"), isMatch ? _T("成功") : _T("失败"));
}

void CSubPipePointPropertyDlg::OnSelectShowPipePoint()
{
	acDocManager->lockDocument(curDoc()); // 锁定文档
	CString type; m_combox.GetWindowText(type);
	int row = 0; IDataBaseOper oper;
	if(type.CompareNoCase(_T("所有管点")) == 0)
	{
		map<CString, AcDbObjectIdArray>::iterator it = m_lids.begin();
		for(; it != m_lids.end(); ++it)
			updatePipeLineProperty(it->first, row);
	}
	else
	{
		MStr filter; filter[_T("附加值")] = type;
		CString name = oper.readPipeLineTable(_T("关键字"), filter);
		updatePipeLineProperty(name, row);
	}
	acDocManager->unlockDocument(curDoc());
}

void CSubPipePointPropertyDlg::updatePipeLineProperty(const CString &layer, int &row)
{
	if(row == 0)
		m_list.DeleteAllItems();
	AcDbObjectIdArray ids = m_lids[layer];
	int len = ids.length(); TCHAR info[255] = {0};
	acedSetStatusBarProgressMeter(_T("正在更新管点表属性..."), 0, len - 1);
	VStr field, alias; VInt fieldtype; VInt read;
	g_attribute.get_gistable_columns(_T("PIPP"), field, fieldtype, alias);
	for(int idx = 0; idx < len; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = ids.at(idx); int count = 1;
		for(int col = 0; col < (int)field.size(); ++col)
		{
			ReadXdata(aname(id), field[col], fieldtype[col] % 10, info);
			if(col == 0)
				m_list.InsertItem(idx, info);
			else
				m_list.SetItemText(idx, col, info);
		}
	}
	acedRestoreStatusBar();
	row += len;
}

BOOL CSubPipePointPropertyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSubPipePointPropertyDlg::flushPipeLineProperty()
{
	OnBnClickedButtonPipepointflushaproperty();
}

void CSubPipePointPropertyDlg::OnBnClickedButtonPipepointflushaproperty()
{
	// TODO:  在此添加控件通知处理程序代码
	m_list.DeleteAllItems();
	m_combox.ResetContent();
	initComboxCtrl();
	initListCtrlValue();
}

void CSubPipePointPropertyDlg::OnBnClickedButtonPipepointreassociate()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount(); CString index;
	VStr field, alias; VInt fieldtype; MSInt record; CLockManager lock;
	g_attribute.get_gistable_columns(_T("PIPP"), field, fieldtype, alias);
	for(int idx = 0; idx < count; ++idx)
	{
		index = m_list.GetItemText(idx, 0);
		if(record.end() == record.find(index)) record[index] = 0;
		AcDbObjectId id = m_sid[index];
		for(int indx = 1; indx < (int)field.size(); ++indx)
			ADDMulXData(id, field[indx], m_list.GetItemText(idx, indx), fieldtype[indx] % 10, record[index]);
		record[index]++;
	}
	return;
}

void CSubPipePointPropertyDlg::OnBnClickedCheckMatchPipePoint()
{
	// TODO:  在此添加控件通知处理程序代码
	m_matchpl.SetCheck(0);
}

void CSubPipePointPropertyDlg::OnBnClickedCheckMatchPipeLine()
{
	// TODO:  在此添加控件通知处理程序代码
	m_match.SetCheck(0);
}

// CPipeLinePropertyDlg 对话框

IMPLEMENT_DYNAMIC(CPipeLinePropertyDlg, CDialog)

CPipeLinePropertyDlg::CPipeLinePropertyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPipeLinePropertyDlg::IDD, pParent)
{

}

CPipeLinePropertyDlg::~CPipeLinePropertyDlg()
{

}

void CPipeLinePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PIPELINEPROPERTY, m_tab);
}

void CPipeLinePropertyDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_ldlg.ShowWindow(TRUE);
			m_pdlg.ShowWindow(FALSE);
			break;
		case 1:
			m_ldlg.ShowWindow(FALSE);
			m_pdlg.ShowWindow(TRUE);
			break;
		default:;
	}
	*pResult = 0;
}

BOOL CPipeLinePropertyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc按键事件  
				return true;
			case VK_RETURN: //Enter按键事件  
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CPipeLinePropertyDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PIPELINEPROPERTY, &CPipeLinePropertyDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()

// CPipeLinePropertyDlg 消息处理程序

void CPipeLinePropertyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;
	m_tab.GetWindowRect(&rect);
	rect.left = 0;	rect.right = cx;
	rect.top = 0; rect.bottom = cy;
	m_tab.MoveWindow(rect);
	rect.top = 20;
	m_pdlg.MoveWindow(rect);
	m_ldlg.MoveWindow(rect);
	// TODO:  在此处添加消息处理程序代码
}


BOOL CPipeLinePropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_tab.InsertItem(0, _T("管线属性"));
	m_tab.InsertItem(1, _T("管点属性"));
	m_ldlg.Create(IDD_DIALOG_SUBPIPELINEPROPERTYDLG, &m_tab);
	m_pdlg.Create(IDD_DIALOG_SUBPIPEPOINTPROPERTYDLG, &m_tab);
	m_tab.GetClientRect(&m_rect);
	m_ldlg.MoveWindow(&m_rect);
	m_pdlg.MoveWindow(&m_rect);
	m_ldlg.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

IMPLEMENT_DYNAMIC(CCommonDockControlBar, CAdUiDockControlBar)
BEGIN_MESSAGE_MAP(CCommonDockControlBar, CAdUiDockControlBar)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static CLSID clsCCommonDockBar = {0x4ebb7912, 0x7356, 0x43de, {0xb1, 0x50, 0x62, 0xbe, 0xae, 0x21, 0x06, 0xde}};

CCommonDockControlBar::CCommonDockControlBar()
	: CAdUiDockControlBar()
{

}

CCommonDockControlBar::~CCommonDockControlBar()
{

}

BOOL CCommonDockControlBar::Create(CWnd*pParent, TCHAR *lpszTitle)
{
	CString strWndClass;
	strWndClass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW));
	CRect rect(0, 0, 250, 200);
	if(!CAdUiDockControlBar::Create(strWndClass, lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParent, 0))
		return (FALSE);
	SetToolID(&clsCCommonDockBar);
	return (TRUE);
}

int CCommonDockControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CAdUiDockControlBar::OnCreate(lpCreateStruct) == -1)
		return (-1);
	m_p_dlg->Create(m_IDD, this);
	m_p_dlg->ShowWindow(SW_SHOW);
	return (0);
}

void CCommonDockControlBar::OnHideBar()
{
	g_pipelinedlg = false;
	CAdUiDockControlBar::OnHideBar();
}

bool CCommonDockControlBar::OnClosing()
{
	g_pipelinedlg = false;
	return CAdUiDockControlBar::OnClosing();
}

void CCommonDockControlBar::SizeChanged(CRect *lpRect, BOOL bFloating, int flags)
{
	m_p_dlg->MoveWindow(lpRect);
	CAdUiDockControlBar::SizeChanged(lpRect, bFloating, flags);
}

BOOL CCommonDockControlBar::PreTranslateMessage(MSG* pMsg)
{
	// 	if(pMsg->message == WM_MOUSELEAVE)
	// 		::SetFocus(acedGetAcadDwgView()->GetSafeHwnd());
	return CAdUiDockControlBar::PreTranslateMessage(pMsg);
}

BOOL CCommonDockControlBar::DestroyWindow()
{
	m_p_dlg->DestroyWindow();
	return CAdUiDockControlBar::DestroyWindow();
}

void CCommonDockControlBar::SetChildDlg(CDialog* p_dlg, int IDD)
{
	m_p_dlg = p_dlg;
	m_IDD = IDD;
}

// CModifyRoadPro 对话框

IMPLEMENT_DYNAMIC(CModifyRoadPro, CAdUiDialog)

CModifyRoadPro::CModifyRoadPro(CWnd* pParent /*=NULL*/)
: CAdUiDialog(CModifyRoadPro::IDD, pParent)
{

}

CModifyRoadPro::~CModifyRoadPro()
{

}

void CModifyRoadPro::DoDataExchange(CDataExchange* pDX)
{
	CAdUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_road);
	DDX_Control(pDX, IDC_LIST_1, m_list);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_add);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_del);
	DDX_Control(pDX, IDC_BUTTON_PICKPOINT, m_pickpoint);
}

BOOL CModifyRoadPro::GetRoadIds()
{
	ads_name ssName;
	AcDbObjectIdArray idsTemp;
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	struct resbuf *rc = acutBuildList(RTDXF0, _T("*POLYLINE"), 8, _T("SCDLX"), 0);
	acedSSGet(_T("X"), NULL, NULL, rc, ssName); ads_relrb(rc);
	SSToIdArr(ssName, idsTemp);
	m_ids.removeAll();
	for(int i = 0; i < idsTemp.length(); ++i)
	{
		AcDbObjectId idTemp = idsTemp.at(i);
		TCHAR szStdm[255] = {0};
		GetStdm(aname(idTemp), szStdm);
		if(_tcscmp(szStdm, _T("180302"))) continue;
		m_ids.append(idTemp);
	}
	if(m_ids.isEmpty()) return FALSE;
	return TRUE;
}

void CModifyRoadPro::ComboxAndList()
{
	for(int i = 0; i < m_ids.length(); ++i)
	{
		AcDbObjectId idTemp = m_ids.at(i);
		TCHAR szRoad[255] = {0};
		if(false == ReadXdata(aname(idTemp), _T("道路中心线名称"), STRING_TYPE, szRoad)) continue;
		m_sid[szRoad] = idTemp;
		m_road.AddString(szRoad);
	}
	m_road.SetCurSel(0);
	m_list.Init();	CRect rect;
	m_list.GetClientRect(&rect);
	m_road.GetWindowText(m_oldName);
	int width = rect.right - rect.left - 20;
	m_list.InsertColumn(0, _T("桩号名称"), LVCFMT_CENTER, width / 5);
	m_list.InsertColumn(1, _T("设计点坐标"), LVCFMT_CENTER, width / 5 + 20);
	m_list.InsertColumn(2, _T("设计高程"), LVCFMT_CENTER, width / 5);
	m_list.InsertColumn(3, _T("竣工高程"), LVCFMT_CENTER, width / 5);
	m_list.InsertColumn(4, _T("高差"), LVCFMT_CENTER, width / 5);
	m_list.SetColumnReadOnly(1, TRUE);
	m_list.SetColumnReadOnly(3, TRUE);
	m_list.SetColumnReadOnly(4, TRUE);
}

BOOL CModifyRoadPro::OnInitDialog()
{
	CAdUiDialog::OnInitDialog();
	if(!GetRoadIds())
	{
		acutPrintf(_T("图面上无道路中心线!\n"));
		return TRUE;
	}
	ComboxAndList();
	m_list.DeleteAllItems();
	m_road.GetWindowText(m_oldName);
	AcDbObjectId id = m_sid[m_oldName];
	if(0 >= GetXDataCount(aname(id), _T("LCZGC")))
		getRoadInfo(id);
	else
		readRoadInfo(id);
	return TRUE;
}

void CModifyRoadPro::saveRoadInfo(const AcDbObjectId &id)
{
	int row = m_list.GetItemCount();
	CString zh, sjpos, sjgc, scgc, gc, info;
	RemoveXdata(aname(id), _T("LCZGC"));
	bool bSuc = false;
	for(int idx = 0; idx < row; ++idx)
	{
		zh = m_list.GetItemText(idx, 0);
		sjpos = m_list.GetItemText(idx, 1);
		sjgc = m_list.GetItemText(idx, 2);
		scgc = m_list.GetItemText(idx, 3);
		gc = m_list.GetItemText(idx, 4);
		info.Format(_T("%s_%s_%s_%s_%s"), zh, sjpos, sjgc, scgc, gc);
		bSuc = ADDMulXData(id, _T("LCZGC"), info, 0, idx);
	}
}

void CModifyRoadPro::getRoadInfo(const AcDbObjectId &id)
{
	AcGePoint2dArray pts;
	GetPlList(aname(id), pts);
	int nConut = 0; VStr lcz;
	for(int i = 0; i < pts.length() - 1; ++i)
	{
		AcGePoint2d ptS(pts[i]), ptE(pts[i + 1]), pt1, pt2, pt3, pt4;
		AcGePoint2dArray nodes;
		pt1[X] = ptS[X] - 2; pt1[Y] = ptS[Y] - 2; nodes.append(pt1);
		pt2[X] = ptE[X] + 2; pt2[Y] = ptE[Y] - 2; nodes.append(pt2);
		pt3[X] = ptE[X] + 2; pt3[Y] = ptE[Y] + 2; nodes.append(pt3);
		pt4[X] = ptS[X] - 2; pt4[Y] = ptS[Y] + 2; nodes.append(pt4);
		AcDbObjectIdArray ids;
		ssFromNodes(ids, nodes, 3, 5.0, 0, _T("dmx"));
		for(int j = 0; j < ids.length(); ++j)
		{
			AcDbObjectId idTemp = ids.at(j);
			TCHAR szStdm[255] = {0}, szLCZ[255] = {0}, szGC[255] = {0};
			CString strGC = _T("");
			GetStdm(aname(idTemp), szStdm);
			if(_tcscmp(szStdm, _T("992000"))) continue;
			ReadXdata(aname(idTemp), _T("LCZNOTE"), STRING_TYPE, szLCZ);
			if(lcz.end() != find(lcz.begin(), lcz.end(), szLCZ)) continue;
			ReadXdata(aname(idTemp), _T("LCZGC"), DOUBLE_TYPE, szGC);
			double dTemp = _tstof(szGC); strGC.Format(_T("%.4f"), dTemp);
			m_list.InsertItem(nConut, szLCZ); lcz.push_back(szLCZ);
			m_list.SetItemText(nConut++, 3, strGC);
		}
	}
}

void CModifyRoadPro::setListItemValue(const VStr &infos, int row)
{
	if(5 != infos.size()) return;
	m_list.InsertItem(row, infos[0]);
	m_list.SetItemText(row, 1, infos[1]);
	m_list.SetItemText(row, 2, infos[2]);
	m_list.SetItemText(row, 3, infos[3]);
	m_list.SetItemText(row, 4, infos[4]);
}

void CModifyRoadPro::readRoadInfo(const AcDbObjectId &id)
{
	int nCount = GetXDataCount(aname(id), _T("LCZGC"));
	TCHAR info[200] = {0}; VStr lcz; int count = 0;
	for(int idx = 0; idx < nCount; ++idx)
	{
		VStr infos;
		READMulXData(id, _T("LCZGC"), info, 0, idx);
		AnalyzeData(infos, info, _T("_"));
		if(lcz.end() != find(lcz.begin(), lcz.end(), infos[0])) continue;
		setListItemValue(infos, count++); lcz.push_back(infos[0]);
	}
}

void CModifyRoadPro::OnSelRoad()
{
	saveRoadInfo(m_sid[m_oldName]);
	m_list.DeleteAllItems();
	m_road.GetWindowText(m_oldName);
	AcDbObjectId id = m_sid[m_oldName];
	if(0 >= GetXDataCount(aname(id), _T("LCZGC")))
		getRoadInfo(id);
	else
		readRoadInfo(id);
}

void CModifyRoadPro::OnLvnItemchangingList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	int nSel = m_list.GetSelectedItem();
	CString str1 = m_list.GetItemText(nSel, 2);
	if(str1.IsEmpty())
	{
		m_list.SetItemText(nSel, 4, _T(""));
		return;
	}
	CString str2 = m_list.GetItemText(nSel, 3);
	double d1, d2, dD;
	d1 = _tstof(str1);
	d2 = _tstof(str2);
	dD = d1 - d2;
	CString str3;
	str3.Format(_T("%.4f"), dD);
	m_list.SetItemText(nSel, 4, str3);
}

void CModifyRoadPro::OnBnClickedAdd()
{
	int nCount = m_list.GetItemCount();
	m_list.InsertItem(nCount, _T(""));
	m_list.EnsureVisible(nCount, TRUE);
	m_list.setSelectItem(nCount);
}

void CModifyRoadPro::OnBnClickedDel()
{
	m_list.SetRedraw(FALSE);
	int nSel = m_list.GetSelectedItem();
	m_list.DeleteItem(nSel);
	m_list.SetRedraw(TRUE);
	m_list.Invalidate();
	m_list.UpdateWindow();
	int nCount = m_list.GetItemCount();
	m_list.setSelectItem(nCount - 1);
}

void CModifyRoadPro::OnBnClickedOK()
{
	CDialog::OnOK();
	saveRoadInfo(m_sid[m_oldName]);
}

void CModifyRoadPro::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CModifyRoadPro::OnBnClickedButtonPickPoint()
{
	// TODO:  在此添加控件通知处理程序代码
	int nSel = m_list.GetSelectedItem();
	if(nSel == -1)
	{
		AfxMessageBox(_T("请选中需要添加设计点坐标的表格行后再拾取"));
		return;
	}
	BeginEditorCommand();
	ads_point pos;
	ads_getpoint(NULL, _T("\n请拾取设计点坐标"), pos);
	CString strPos;
	strPos.Format(_T("%f,%f"), pos[X], pos[Y]);
	m_list.SetItemText(nSel, 1, strPos);
	acutPrintf(_T("\n成功拾取设计坐标"));
	CompleteEditorCommand();
}

BEGIN_MESSAGE_MAP(CModifyRoadPro, CAdUiDialog)
	ON_BN_CLICKED(IDOK, &CModifyRoadPro::OnBnClickedOK)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CModifyRoadPro::OnSelRoad)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CModifyRoadPro::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CModifyRoadPro::OnBnClickedDel)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_1, &CModifyRoadPro::OnLvnItemchangingList1)
	ON_BN_CLICKED(IDC_BUTTON_PICKPOINT, &CModifyRoadPro::OnBnClickedButtonPickPoint)
END_MESSAGE_MAP()

// CFilterLayerDlg 对话框

IMPLEMENT_DYNAMIC(CFilterLayerDlg, CDialog)

CFilterLayerDlg::CFilterLayerDlg(const VStr &layers, CWnd* pParent /*=NULL*/)
: CDialog(CFilterLayerDlg::IDD, pParent)
{
	for(int idx = 0; idx < (int)layers.size(); ++idx)
		m_layers.push_back(layers[idx]);

}

CFilterLayerDlg::~CFilterLayerDlg()
{

}

void CFilterLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILTERLAYER, m_list);
	DDX_Control(pDX, IDC_EDIT_INSERTDWGSCALE, m_scale);
}

BEGIN_MESSAGE_MAP(CFilterLayerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFilterLayerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTALL, &CFilterLayerDlg::OnBnClickedButtonImportall)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINEREVERSE, &CFilterLayerDlg::OnBnClickedButtonPipelinReverse)
	ON_BN_CLICKED(IDC_BUTTON_PIPELINENOTSELECT, &CFilterLayerDlg::OnBnClickedButtonPipelineNotSelect)
END_MESSAGE_MAP()

// CFilterLayerDlg 消息处理程序


BOOL CFilterLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	CRect rect; m_list.GetClientRect(&rect);
	m_list.InsertColumn(0, _T("是否导入"), LVCFMT_CENTER, 80);
	m_list.InsertColumn(1, _T("图层名称"), LVCFMT_CENTER, rect.Width() - 80);
	for(int idx = 0; idx < (int)m_layers.size(); ++idx)
	{
		m_list.InsertItem(idx, _T(""));
		m_list.SetItemText(idx, 1, m_layers[idx]);
	}
	m_layers.clear();
	m_list.SetColumnReadOnly(1, TRUE);
	m_scale.SetWindowText(_T("1.00"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CFilterLayerDlg::setLayers(const VStr &layers)
{
	for(int idx = 0; idx < (int)layers.size(); ++idx)
		m_layers.push_back(layers[idx]);
}

VStr CFilterLayerDlg::getLayers()
{
	return m_layers;
}

double CFilterLayerDlg::getScale()
{
	return m_bl;
}

void CFilterLayerDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	CString info;
	for(int idx = 0; idx < count; ++idx)
	{
		if(m_list.GetCheck(idx) == FALSE) continue;
		info = m_list.GetItemText(idx, 1);
		m_layers.push_back(info);
	}
	m_scale.GetWindowText(info);
	m_bl = _tstof(info);
	CDialog::OnOK();
}

void CFilterLayerDlg::OnBnClickedButtonImportall()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
		m_list.SetCheck(idx, TRUE);
}

void CFilterLayerDlg::OnBnClickedButtonPipelinReverse()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		BOOL FLAG = m_list.GetCheck(idx);
		m_list.SetCheck(idx, !FLAG);
	}
}

void CFilterLayerDlg::OnBnClickedButtonPipelineNotSelect()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
		m_list.SetCheck(idx, FALSE);
}

// CSZConfig 对话框
IMPLEMENT_DYNAMIC(CSZConfig, CDialog)

CSZConfig::CSZConfig(CWnd* pParent /*=NULL*/)
: CDialog(CSZConfig::IDD, pParent)
{

}

CSZConfig::~CSZConfig()
{

}

void CSZConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SZXM, m_dataPick);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTDIR, m_projectdir);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTCODE, m_projectcode);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTNAME, m_projectname);
	DDX_Control(pDX, IDC_EDIT_PIPELINEBUILDCOMPANY, m_buildcompany);
	DDX_Control(pDX, IDC_EDIT_PIPELINEPROJECTADDRESS, m_projectaddress);
	DDX_Control(pDX, IDC_EDIT_PIPELINEMEANSURECOMPANY, m_meansurecompany);
	DDX_Control(pDX, IDC_EDIT_PIPELINEIMPLEMENTCOMPANY, m_implementcompany);
}

BEGIN_MESSAGE_MAP(CSZConfig, CDialog)
END_MESSAGE_MAP()

// CPipeConfig 消息处理程序
BOOL CSZConfig::OnInitDialog()
{
	CAcModuleResourceOverride myResources;
	CDialog::OnInitDialog();
	initProjectInfo();
	return TRUE;
}

void CSZConfig::setProjectInfo(CString &strPath)
{
	MStr context;
	m_dataPick.GetWindowText(context[_T("项目时间")]);
	m_projectcode.GetWindowText(context[_T("受理号")]);
	m_projectname.GetWindowText(context[_T("项目名称")]);
	m_buildcompany.GetWindowText(context[_T("建设单位")]);
	m_projectaddress.GetWindowText(context[_T("项目地点")]);
	m_meansurecompany.GetWindowText(context[_T("测量单位")]);
	m_implementcompany.GetWindowText(context[_T("实施单位")]);
	IProjectInfo pi(_T("市政竣工"));
	pi.CreateProjectEx(context, strPath, _T("规划"));
}

void CSZConfig::initProjectInfo()
{
	IDataBaseOper db; CStringArray values;
	CString value = db.readJGCommon(_T("测量单位"));
	AnalyzeData(values, value, _T(";"));
	for(int idx = 0; idx < (int)values.GetSize(); ++idx)
		m_meansurecompany.InsertString(idx, values.GetAt(idx));
	m_strdir = db.readPathTable(_T("工程目录")) + _T("市政竣工");
	m_implementcompany.SetWindowText(_T("厦门市测绘与基础地理信息中心"));
	m_projectdir.SetWindowText(m_strdir);
}

void CSZConfig::Apply()
{
	CString strPath = _T("");
	setProjectInfo(strPath);
}

// CSZProjectDlg 对话框

IMPLEMENT_DYNAMIC(CSZProjectDlg, CDialog)

CSZProjectDlg::CSZProjectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSZProjectDlg::IDD, pParent)
{

}

CSZProjectDlg::~CSZProjectDlg()
{

}

void CSZProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PIPELINECREATETAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CSZProjectDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PIPELINECREATETAB1, &CSZProjectDlg::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDOK, &CSZProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSZProjectDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CPipeLineCreateProjectDlg 消息处理程序


BOOL CSZProjectDlg::OnInitDialog()
{
	CAcModuleResourceOverride myResources;
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_tab.InsertItem(0, _T("基本信息"));
	m_tab.InsertItem(1, _T("作业管线"));
	m_pconfig.Create(IDD_DIALOG_SHIZHENG, &m_tab);
	m_pselect.Create(IDD_DIALOG_PIPELINESELECT, &m_tab);
	CRect rect; m_tab.GetClientRect(&rect);
	rect.top += 20; rect.bottom += 20; rect.right += 40;
	m_pconfig.MoveWindow(&rect);
	m_pselect.MoveWindow(&rect);
	m_pconfig.ShowWindow(TRUE);
	m_tab.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CSZProjectDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
		case 0:
			m_pconfig.ShowWindow(TRUE);
			m_pselect.ShowWindow(FALSE);
			break;
		case 1:
			m_pconfig.ShowWindow(FALSE);
			m_pselect.ShowWindow(TRUE);
			break;
		default:;
	}
	*pResult = 0;
}

void CSZProjectDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	m_pconfig.Apply();
	m_pselect.Apply();
	CDialog::OnOK();
}

void CSZProjectDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

// TQZBINFO 对话框

IMPLEMENT_DYNAMIC(CTQZBINFO, CDialog)

CTQZBINFO::CTQZBINFO(CWnd* pParent /*=NULL*/)
: CDialog(CTQZBINFO::IDD, pParent)
{

}

CTQZBINFO::~CTQZBINFO()
{
}

void CTQZBINFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_add);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_del);
	DDX_Control(pDX, IDC_BUTTON_ADD2, m_add2);
	DDX_Control(pDX, IDC_BUTTON_DEL2, m_del2);
}

BOOL CTQZBINFO::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list.Init(); CRect rect;
	m_list.GetClientRect(&rect);
	int width = rect.right - rect.left; CString pName;
	m_list.InsertColumn(0, _T("项目名称"), LVCFMT_CENTER, width);
	SetDlgItemText(IDC_EDIT1, m_strZYZB); int index = 0;
	for(MSIter it = m_Pro_Remarks.begin(); it != m_Pro_Remarks.end(); ++it)
	{
		pName = it->first;
		m_list.InsertItem(index++, pName);
	}
	m_list.EnsureVisible(0, TRUE);
	m_list.setSelectItem(0);
	m_list2.Init(); CRect rect2;
	m_list2.GetClientRect(&rect2);
	int width2 = rect2.right - rect2.left;
	m_list2.InsertColumn(0, _T("桥梁净高"), LVCFMT_CENTER, width2);
	pName = m_list.GetItemText(0, 0);
	VStr heights = mData[pName];
	for(int idx = 0; idx < (int)heights.size(); ++idx)
	{
		m_list2.InsertItem(idx, heights[idx]);
	}
	SetDlgItemText(IDC_EDIT2, m_Pro_BridgeWidth[pName]);
	SetDlgItemText(IDC_EDIT3, m_Pro_Remarks[pName]);
	return TRUE;
}

void CTQZBINFO::OnBnClickedAdd()
{
	int nCount = m_list.GetItemCount();
	m_list.InsertItem(nCount, _T(""));
	m_list.EnsureVisible(nCount, TRUE);
	m_list.setSelectItem(nCount);
	m_list2.DeleteAllItems();

	SetDlgItemText(IDC_EDIT2, _T(""));
	SetDlgItemText(IDC_EDIT3, _T(""));
}

void CTQZBINFO::OnBnClickedDel()
{
	m_list.SetRedraw(FALSE);
	int nSel = m_list.GetSelectedItem();
	CString str1 = m_list.GetItemText(nSel, 0);

	m_list.DeleteItem(nSel);
	//int nCount = m_list.GetItemCount();
	//m_list.setSelectItem(nCount - 1);
	m_list.SetRedraw(TRUE);
	m_list.Invalidate();
	m_list.UpdateWindow();

	m_list2.DeleteAllItems();
	m_list2.SetRedraw(TRUE);
	m_list2.Invalidate();
	m_list2.UpdateWindow();

	mData.erase(str1);
	m_Pro_BridgeWidth.erase(str1);
	m_Pro_Remarks.erase(str1);
	SetDlgItemText(IDC_EDIT2, _T(""));
	SetDlgItemText(IDC_EDIT3, _T(""));
}

void CTQZBINFO::OnBnClickedAdd2()
{
	int nSel = m_list.GetSelectedItem();
	if(nSel == -1)
	{
		AfxMessageBox(_T("请选中项目名称后再添加桥梁净高"));
		return;
	}

	int nCount = m_list2.GetItemCount();
	m_list2.InsertItem(nCount, _T(""));
	m_list2.EnsureVisible(nCount, TRUE);
	m_list2.setSelectItem(nCount);
}

void CTQZBINFO::OnBnClickedDel2()
{
	m_list2.SetRedraw(FALSE);
	int nSel = m_list2.GetSelectedItem();
	m_list2.DeleteItem(nSel);
	m_list2.SetRedraw(TRUE);
	m_list2.Invalidate();
	m_list2.UpdateWindow();
	int nCount = m_list2.GetItemCount();
	m_list2.setSelectItem(nCount - 1);
}

VStr CTQZBINFO::GetList2Data()
{
	VStr vStr;
	int nSum = m_list2.GetItemCount();
	for(int i = 0; i < nSum; ++i)
	{
		CString str1 = m_list2.GetItemText(i, 0);
		vStr.push_back(str1);
	}
	return vStr;
}

void CTQZBINFO::OnClickList1Main(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	int nSel = m_list.GetSelectedItem();
	CString str1 = m_list.GetItemText(nSel, 0);
	VStr strs = mData[str1];
	m_strFormer = str1;

	m_list2.DeleteAllItems();
	for(int i = 0; i < (int)strs.size(); ++i)
		m_list2.InsertItem(i, strs.at(i));

	SetDlgItemText(IDC_EDIT2, m_Pro_BridgeWidth[str1]);
	SetDlgItemText(IDC_EDIT3, m_Pro_Remarks[str1]);
}

void CTQZBINFO::OnLvnEndlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	int nSel = m_list.GetSelectedItem();
	CString str1 = m_list.GetItemText(nSel, 0);
	map<CString, VStr>::iterator it;
	it = mData.find(str1);
	if(!str1.IsEmpty() && it != mData.end() && str1 != m_strFormer)
	{
		m_list.SetItemText(nSel, 0, m_strFormer);
		AfxMessageBox(_T("项目名称重复,请检查后重新输入"));
		return;
	}

	if(str1 != m_strFormer)
	{
		VStr vStr = GetList2Data();
		mData.insert(pair<CString, VStr>(str1, vStr));
		mData.erase(m_strFormer);
	}
	CString info;
	GetDlgItemText(IDC_EDIT2, info);
	m_Pro_BridgeWidth[str1] = info;
	GetDlgItemText(IDC_EDIT3, info);
	m_Pro_Remarks[str1] = info;
}

void CTQZBINFO::OnLvnEndlabeleditList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	*pResult = 0;

	VStr vStr = GetList2Data();
	int n1 = m_list.GetSelectedItem();
	CString str1 = m_list.GetItemText(n1, 0);
	mData[str1].clear();
	for(int idx = 0; idx < (int)vStr.size(); ++idx)
		mData[str1].push_back(vStr[idx]);
}

void CTQZBINFO::OnEnChangeEdit2()
{
	int nSel = m_list.GetSelectedItem();
	if(nSel == -1)
	{
		SetDlgItemText(IDC_EDIT2, _T(""));
		AfxMessageBox(_T("请选中项目名称后再添加桥梁宽"));
		return;
	}
	CString info;
	GetDlgItemText(IDC_EDIT2, info);
	CString str1 = m_list.GetItemText(nSel, 0);
	m_Pro_BridgeWidth[str1] = info;
}

void CTQZBINFO::OnEnChangeEdit3()
{
	int nSel = m_list.GetSelectedItem();
	if(nSel == -1)
	{
		SetDlgItemText(IDC_EDIT3, _T(""));
		AfxMessageBox(_T("请选中项目名称后再添加备注"));
		return;
	}
	CString info;
	GetDlgItemText(IDC_EDIT3, info);
	CString str1 = m_list.GetItemText(nSel, 0);
	m_Pro_Remarks[str1] = info;
}

void CTQZBINFO::OnBnClickedOK()
{
	GetDlgItemText(IDC_EDIT1, m_strZYZB);
	CDialog::OnOK();
}

void CTQZBINFO::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CTQZBINFO::setDialogInfo(const VMStr &records)
{
	int len = (int)records.size();
	for(int idx = 0; idx < len; ++idx)
	{
		MStr record = records[idx]; m_strZYZB = record[_T("主要指标")];
		m_Pro_Remarks[record[_T("项目名称")]] = record[_T("备注")];
		m_Pro_BridgeWidth[record[_T("项目名称")]] = record[_T("桥梁宽")];
		AnalyzeData(mData[record[_T("项目名称")]], record[_T("桥梁净高")], _T("|"));
	}
}

BEGIN_MESSAGE_MAP(CTQZBINFO, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CTQZBINFO::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CTQZBINFO::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BUTTON_ADD2, &CTQZBINFO::OnBnClickedAdd2)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, &CTQZBINFO::OnBnClickedDel2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTQZBINFO::OnClickList1Main)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, &CTQZBINFO::OnLvnEndlabeleditList1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST2, &CTQZBINFO::OnLvnEndlabeleditList2)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CTQZBINFO::OnEnChangeEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CTQZBINFO::OnEnChangeEdit3)
	ON_BN_CLICKED(IDOK, &CTQZBINFO::OnBnClickedOK)
END_MESSAGE_MAP()

// CShiZhengPipeLine 对话框

IMPLEMENT_DYNAMIC(CShiZhengPipeLine, CDialog)

CShiZhengPipeLine::CShiZhengPipeLine(CWnd* pParent /*=NULL*/)
: CDialog(CShiZhengPipeLine::IDD, pParent)
{

}

CShiZhengPipeLine::~CShiZhengPipeLine()
{

}

void CShiZhengPipeLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SZPIPELINETYPE, m_pipetype);
	DDX_Control(pDX, IDC_LIST_SZPIPELINEINFO, m_pipeinfo);
}

BEGIN_MESSAGE_MAP(CShiZhengPipeLine, CDialog)
END_MESSAGE_MAP()

// CShiZhengPipeLine 消息处理程序

BOOL CShiZhengPipeLine::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	initComboxCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CShiZhengPipeLine::initComboxCtrl()
{
	IProjectMDB pdb;
	VMStr records = pdb.getPipeTable();
	for(int idx = 0; idx < (int)records.size(); ++idx)
	{
		MStr record = records[idx];
		m_pipetype.AddString(record[_T("名称")]);
	}
}

// CRoadHDMTLDlg 对话框

IMPLEMENT_DYNAMIC(CRoadHDMTLDlg, CDialog)

CRoadHDMTLDlg::CRoadHDMTLDlg(CWnd* pParent /*=NULL*/)
: CAdUiDialog(CRoadHDMTLDlg::IDD, pParent)
{

}

CRoadHDMTLDlg::~CRoadHDMTLDlg()
{
}

void CRoadHDMTLDlg::DoDataExchange(CDataExchange* pDX)
{
	CAdUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SELECTENT, m_selEnt);
	DDX_Control(pDX, IDC_LIST_ROADDZTLIST, m_list);
}


BEGIN_MESSAGE_MAP(CRoadHDMTLDlg, CAdUiDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECTENT, &CRoadHDMTLDlg::OnBnClickedButtonSelectEnt)
	ON_BN_CLICKED(IDC_BUTTON_DELETEITEM, &CRoadHDMTLDlg::OnBnClickedButtonDeleteItem)
	ON_BN_CLICKED(IDOK, &CRoadHDMTLDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CRoadHDMTLDlg 消息处理程序

void CRoadHDMTLDlg::OnBnClickedButtonSelectEnt()
{
	// TODO:  在此添加控件通知处理程序代码
	BeginEditorCommand();
	ads_point pos; ads_name ent;
	if(RTNORM != ads_entsel(_T(""), ent, pos))
	{
		CompleteEditorCommand();
		return;
	}
	TCHAR code[255] = {0};
	ReadXdata(ent, _T("HDM"), 0, code);
	if(_tcslen(code) == 0)
	{
		CompleteEditorCommand();
		return;
	}
	AcDbObjectId id;
	acdbGetObjectId(id, ent);
	if(m_ids.find(id) != -1)
	{
		CompleteEditorCommand();
		return;
	}
	m_ids.append(id);
	setcolor(aname(id), 1);
	int count = m_list.GetItemCount();
	TCHAR xh[25] = {0};
	_stprintf(xh, _T("%d"), count + 1);
	m_list.InsertItem(count, xh);
	m_list.SetComboColumns(1, m_name);
	CompleteEditorCommand();
}

BOOL CRoadHDMTLDlg::OnInitDialog()
{
	CAdUiDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_list.Init(TRUE, 16); 	CRect rect;
	m_list.GetClientRect(rect);
	int width = rect.right - rect.left;
	IDataBaseOper oper; m_name += _T(" ");
	oper.readCommonTable(_T("道路竣工横断面图例"), m_tuli);
	for(MSIter it = m_tuli.begin(); it != m_tuli.end(); ++it)
		m_name += _T(";") + it->first;
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_OWNERDRAWFIXED | LVS_EX_DOUBLEBUFFER);
	m_list.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 60);
	m_list.InsertColumn(1, _T("图例类型"), LVCFMT_CENTER, width - 60);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE

}

void CRoadHDMTLDlg::OnBnClickedButtonDeleteItem()
{
	// TODO:  在此添加控件通知处理程序代码
	int nSel = m_list.GetSelectedItem();
	m_list.DeleteItem(nSel);
	m_ids.removeAt(nSel);
	int count = m_list.GetItemCount();
	TCHAR xh[25] = {0};
	for(int idx = nSel; idx < count; ++idx)
	{
		_stprintf(xh, _T("%d"), idx + 1);
		m_list.SetItemText(idx, 0, xh);
	}
}

MIdStr CRoadHDMTLDlg::getIdBlock()
{
	return m_idBlock;
}

void CRoadHDMTLDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	int count = m_list.GetItemCount();
	for(int idx = 0; idx < count; ++idx)
	{
		AcDbObjectId id = m_ids.at(idx);
		CString value = m_list.GetItemText(idx, 1);
		m_idBlock[id] = m_tuli[value];
	}
	CAdUiDialog::OnOK();
}

// CLayerManagerDlg 对话框

IMPLEMENT_DYNAMIC(CLayerManagerDlg, CDialog)

CLayerManagerDlg::CLayerManagerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLayerManagerDlg::IDD, pParent)
{

}

CLayerManagerDlg::~CLayerManagerDlg()
{

}

void CLayerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLayerManagerDlg, CDialog)
END_MESSAGE_MAP()

// CLayerManagerDlg 消息处理程序

IMPLEMENT_DYNAMIC(CModifyProjectInfo, CDialog)

CModifyProjectInfo::CModifyProjectInfo(CWnd* pParent /*=NULL*/)
: CDialog(CModifyProjectInfo::IDD, pParent)
, m_selRow(-1)
{

}

CModifyProjectInfo::~CModifyProjectInfo()
{

}

void CModifyProjectInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECTNAMES, m_names);
	DDX_Control(pDX, IDC_EDIT_MPROJECTCODE, m_pcode);
	DDX_Control(pDX, IDC_EDIT_MPROJECTNAME, m_pname);
	DDX_Control(pDX, IDC_EDIT_DPROJECTNAME, m_dname);
	DDX_Control(pDX, IDC_EDIT_DPROJECTCODE, m_dcode);
}

BEGIN_MESSAGE_MAP(CModifyProjectInfo, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CModifyProjectInfo::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CModifyProjectInfo::OnBnClickedRepost)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROJECTNAMES, &CModifyProjectInfo::OnLvnClickListItem)
END_MESSAGE_MAP()

// CModifyProjectInfo 消息处理程序

BOOL CModifyProjectInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_names.Init(TRUE, 24); CRect rect;
	m_names.SetExtendedStyle(LVS_OWNERDRAWFIXED | LVS_EX_DOUBLEBUFFER);
	m_names.GetClientRect(&rect);
	int width = (rect.right - rect.left);
	m_names.InsertColumn(0, _T("项目文件夹"), LVCFMT_CENTER, width);
	m_names.SetColumnReadOnly(0, TRUE);
	for(int idx = 0; idx < (int)m_infos.size(); ++idx)
	{
		ProjectInfo pInfo = m_infos[idx];
		CString info = pInfo.m_src.m_code;
		info += _T("_");
		info += pInfo.m_src.m_name;
		m_names.InsertItem(idx, info);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CModifyProjectInfo::OnBnClickedApply()
{
	// TODO:  在此添加控件通知处理程序代码
}

void CModifyProjectInfo::OnBnClickedRepost()
{
	// TODO:  在此添加控件通知处理程序代码
}

void CModifyProjectInfo::saveChangeInfo(int row)
{
	if(row == -1) return;
	CString name, code;
	m_dname.GetWindowText(name);
	m_dcode.GetWindowText(code);
	ProjectInfo pinfo = m_infos[row];
	if(pinfo.m_des.m_name.CompareNoCase(name) != 0
		|| pinfo.m_des.m_code.CompareNoCase(code) != 0)
	{
		m_infos[row].isModify = true;
		m_infos[row].m_des.m_name = name;
		m_infos[row].m_des.m_code = code;
	}
}

ProjectInfos CModifyProjectInfo::getProjectInfos()
{
	return m_infos;
}

void CModifyProjectInfo::setProjectInfo(const ProjectInfos &infos)
{
	for(int idx = 0; idx < (int)infos.size(); ++idx)
		m_infos.push_back(infos[idx]);
}

void CModifyProjectInfo::OnLvnClickListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_selRow != -1)
		saveChangeInfo(m_selRow);
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int rowNo = pNMListView->iItem;
	ProjectInfo info = m_infos[rowNo];
	m_pname.SetWindowText(info.m_src.m_name);
	m_pcode.SetWindowText(info.m_src.m_code);
	m_dname.SetWindowText(info.m_des.m_name);
	m_dcode.SetWindowText(info.m_des.m_code);
	m_selRow = rowNo;
}

void CModifyProjectInfo::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	saveChangeInfo(m_selRow);
	CDialog::OnOK();
}
