#pragma once

#include "resource.h"
//#include "D:\VCPlatformTools\VC90\VC\atlmfc\include\afxwin.h"

// CSetAchievementInfo �Ի���

class CSetAchievementInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAchievementInfo)

public:
	CSetAchievementInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetAchievementInfo();

// �Ի�������
	enum { IDD = IDD_SETINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �����
	CEdit m_Number;
	// ί�е�λ
	CEdit m_Client;
	//������Ŀ
	CEdit m_BuildProject;
	//��Ŀ����
	CEdit m_ProjectName;
	//��������
	CEdit m_Measurement;
	//��ϵ�绰
	CEdit m_TelePhone;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
