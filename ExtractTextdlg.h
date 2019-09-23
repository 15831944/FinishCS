#pragma once
#include "Selection_String.h"
#include "resource.h"

// CExtractTextdlg 对话框

class CExtractTextdlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CExtractTextdlg)

public:
	CExtractTextdlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExtractTextdlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXTRACTDLG };
	CString			m_strText;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);

};
