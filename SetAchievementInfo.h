#pragma once

#include "resource.h"
//#include "D:\VCPlatformTools\VC90\VC\atlmfc\include\afxwin.h"

// CSetAchievementInfo 对话框

class CSetAchievementInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAchievementInfo)

public:
	CSetAchievementInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAchievementInfo();

// 对话框数据
	enum { IDD = IDD_SETINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 受理号
	CEdit m_Number;
	// 委托单位
	CEdit m_Client;
	//建设项目
	CEdit m_BuildProject;
	//项目名称
	CEdit m_ProjectName;
	//测量内容
	CEdit m_Measurement;
	//联系电话
	CEdit m_TelePhone;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
