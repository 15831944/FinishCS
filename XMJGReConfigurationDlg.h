#ifndef XMJGRECONFIGURATIONDLG_H
#define XMJGRECONFIGURATIONDLG_H

// XMJGReConfigurationDlg 对话框

#include "resource.h"
#include "aduiDock.h"

class XMJGReConfigurationDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGReConfigurationDlg)

public:
	
	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	XMJGReConfigurationDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	virtual ~XMJGReConfigurationDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECONFIGURATIONDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
};

class XMJGReConfigurationDockbar
	: public CAdUiDockControlBar
{

	DECLARE_DYNAMIC(XMJGReConfigurationDockbar)

public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年08月15日
	*/
	XMJGReConfigurationDockbar();

public:

	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);

	virtual BOOL DestroyWindow();

	void setChildDlg(CDialog *, int IndexIDD);

protected:

	int m_IDD;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL AddCustomMenuItems(LPARAM hMenu);//去掉停靠工具栏 默认的右键菜单

	DECLARE_MESSAGE_MAP()

	virtual void SizeChanged(CRect *lpRect, BOOL bFloating, int flags);

public:

	CDialog *m_dlg;

};

class XMJGDrawDomainDockControl
	: public CAdUiDockControlBar
{

	DECLARE_DYNAMIC(XMJGDrawDomainDockControl)

public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年08月15日
	*/
	XMJGDrawDomainDockControl();

public:

	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);

	virtual BOOL DestroyWindow();

	void setChildDlg(CDialog *, int IndexIDD);

protected:

	int m_IDD;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL AddCustomMenuItems(LPARAM hMenu);//去掉停靠工具栏 默认的右键菜单

	DECLARE_MESSAGE_MAP()

	virtual void SizeChanged(CRect *lpRect, BOOL bFloating, int flags);

public:

	CDialog *m_dlg;

};

#endif /*XMJGRECONFIGURATIONDLG_H*/