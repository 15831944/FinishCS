#pragma once

// CHeightPropertySettingDlg 对话框

#include "EditListCtrl.h"

class CHeightPropertySettingDlg
	: public CAcUiDialog
{
	DECLARE_DYNAMIC(CHeightPropertySettingDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	CHeightPropertySettingDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	virtual ~CHeightPropertySettingDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME

	enum { IDD = IDD_DIALOG_HEIGHTPROPERTYSETTING };

#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
#ifdef _WIN64
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
#else
	afx_msg LONG onAcadKeepFocus(UINT n, LONG l);

	/* @接口 列表单击事件
	 * @参数 NMHDR *
	 * @参数 LRESULT *
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	afx_msg void OnSingleClickListForHeight(NMHDR *, LRESULT *);

	/* @接口 列表单击事件
	 * @参数 NMHDR *
	 * @参数 LRESULT *
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	afx_msg void OnSingleClickListForBuildBG(NMHDR *, LRESULT *);

#endif

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	afx_msg void OnBnClickedOk();

private:

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	void initDialogListCtrl();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	void initListCtrlHeightValue();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	void saveListCtrlHeightValue();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	void initListCtrlBUildBGValue();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	void saveListCtrlBUildBGValue();

public:

	CString m_buildName;
	CEditListCtrl m_settingHeight;			// 高度控件
	CEditListCtrl m_buildBG;				// 建筑物标高控件

};
