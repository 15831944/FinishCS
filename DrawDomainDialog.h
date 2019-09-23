#pragma once

#include "resource.h"
#include "Global.h"
#include "afxwin.h"
// DrawDomainDialog 对话框

class DrawDomainDialog
	: public CDialog
{
	DECLARE_DYNAMIC(DrawDomainDialog)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月19号
	 */
	DrawDomainDialog(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月19号
	 */
	virtual ~DrawDomainDialog();

	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_DRAWDOMAINDIALOG };

	afx_msg void OnBnClickedCheckSelectbh();

	afx_msg void OnBnClickedButtonBegindraw();

	afx_msg void OnCbnSelchangeComboDomainname();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月21号
	 */
	void setControlStatus(BOOL);

public:

	CEdit		m_imPortBH;
	CButton		m_areaKouChu;
	CComboBox	m_jrFactor;
	CEdit		m_domainHeight;
	CComboBox	m_domainName;
	CComboBox	m_areaFactor;
	CComboBox	m_selectStyle;

private:

	CString m_name;
	CString m_mjxs;
	CString m_jrxs;
	int m_drawStyle;
	FunctionNameTables m_fnts;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
