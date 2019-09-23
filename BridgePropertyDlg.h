#pragma once
#include "E:\VCPlatformTools\VC90\VC\atlmfc\include\afxwin.h"


// BridgePropertyDlg 对话框

class BridgePropertyDlg
	: public CAcUiDialog
{
	DECLARE_DYNAMIC(BridgePropertyDlg)

public:
	BridgePropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BridgePropertyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BRIDGEPROPERTRYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonStartpt();
	afx_msg void OnBnClickedButtonEndpt();

private:

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年7月3号
	 */
	void setDialogValue();

private:

	CAcUiPickButton m_endPt;
	CAcUiPickButton m_startPt;

public:
	CEdit m_startPtX;
	CEdit m_startPtY;
	CEdit m_endPtX;
	CEdit m_endPtY;
	CComboBox m_tqgcxs;
	CEdit m_jgcltbh;
	CEdit m_jjtrydbjjlqk;
};
