#pragma once

#include "resource.h"
// CDrawUpPlanning �Ի���

class CDrawUpPlanningDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawUpPlanningDlg)

public:
	CDrawUpPlanningDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDrawUpPlanningDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DRAWUPPLANNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_drawUpPlan;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
