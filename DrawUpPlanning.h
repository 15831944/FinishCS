#pragma once

#include "resource.h"
// CDrawUpPlanning 对话框

class CDrawUpPlanningDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawUpPlanningDlg)

public:
	CDrawUpPlanningDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDrawUpPlanningDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DRAWUPPLANNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_drawUpPlan;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
