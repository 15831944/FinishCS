// SetAchievementInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetAchievementInfo.h"
#include "afxdialogex.h"

#define REGPATH_XMJG _T("Software\\SouthSurvey\\XMJG")

// CSetAchievementInfo �Ի���

IMPLEMENT_DYNAMIC(CSetAchievementInfo, CDialogEx)

CSetAchievementInfo::CSetAchievementInfo(CWnd* pParent /*=NULL*/)
: CDialogEx(CSetAchievementInfo::IDD, pParent)
{

}

CSetAchievementInfo::~CSetAchievementInfo()
{
}

void CSetAchievementInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NUMBER, m_Number);
	DDX_Control(pDX, IDC_CLIENT, m_Client);
	DDX_Control(pDX, IDC_BUILDPROJECT, m_BuildProject);
	DDX_Control(pDX, IDC_PROJECTNAME, m_ProjectName);
	DDX_Control(pDX, IDC_MEASUREMENTCONTENT, m_Measurement);
	DDX_Control(pDX, IDC_TELEPHONE, m_TelePhone);
}


BEGIN_MESSAGE_MAP(CSetAchievementInfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetAchievementInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetAchievementInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetAchievementInfo ��Ϣ�������


void CSetAchievementInfo::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HKEY rKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, REGPATH_XMJG, 0, KEY_READ, &rKey);
	//if (!RegOpenKeyEx(HKEY_CURRENT_USER, REGPATH_XMJG, 0, KEY_READ, &rKey)) return;
	TCHAR *number = new TCHAR[200];
	TCHAR *client = new TCHAR[200];
	TCHAR *bproj = new TCHAR[200];
	TCHAR *phone = new TCHAR[200];
	TCHAR *measu = new TCHAR[200];
	m_Number.GetWindowText(number, 200);
	m_Client.GetWindowText(client, 200);
	m_BuildProject.GetWindowText(bproj, 200);
	m_ProjectName.GetWindowText(bproj, 200);
	m_Measurement.GetWindowText(measu, 200);
	CassSetReg(rKey, _T("number"), number);
	CassSetReg(rKey, _T("client"), client);
	CassSetReg(rKey, _T("bproject"), bproj);
	CassSetReg(rKey, _T("measu"), measu);
	CassSetReg(rKey, _T("phone"), phone);
	CDialogEx::OnOK();
}


void CSetAchievementInfo::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL CSetAchievementInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HKEY rKey;
	//if (!RegOpenKeyEx(HKEY_CURRENT_USER, REGPATH_XMJG, 0, KEY_READ, &rKey)) return FALSE;
	RegOpenKeyEx(HKEY_CURRENT_USER, REGPATH_XMJG, 0, KEY_READ, &rKey);
	TCHAR *number = new TCHAR[200];
	TCHAR *client = new TCHAR[200];
	TCHAR *bproj = new TCHAR[200];
	TCHAR *phone = new TCHAR[200];
	TCHAR *measu = new TCHAR[200];
	CassQueryReg(rKey, _T("number"), number);
	CassQueryReg(rKey, _T("client"), client);
	CassQueryReg(rKey, _T("bproject"), bproj);
	CassQueryReg(rKey, _T("measu"), measu);
	CassQueryReg(rKey, _T("phone"), phone);
	m_Number.SetWindowText(number);
	m_Client.SetWindowText(client);
	m_BuildProject.SetWindowText(bproj);
	m_ProjectName.SetWindowText(bproj);
	m_Measurement.SetWindowText(measu);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
