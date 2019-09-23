#pragma once
#include "resource.h"

// CQualityCheckSubReport 对话框

class CQualityCheckSubReport : public CDialog
{
	DECLARE_DYNAMIC(CQualityCheckSubReport)

public:

	CQualityCheckSubReport(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CQualityCheckSubReport();

// 对话框数据
	enum { IDD = IDD_DIALOG_QUALITYCHECKSUBREPORT };

	void onApply();

	virtual BOOL OnInitDialog();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CString m_name;
	CEdit m_context;

};
