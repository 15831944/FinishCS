#pragma once

#include "resource.h"

// InfomationOfProjectRangeLine 对话框

class CDatabaseOperManager;

class InfomationOfProjectRangeLine : public CDialog
{
	DECLARE_DYNAMIC(InfomationOfProjectRangeLine)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年8月29号
	 */
	InfomationOfProjectRangeLine(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年8月29号
	 */
	virtual ~InfomationOfProjectRangeLine();

// 对话框数据
	enum { IDD = IDD_INFOMATIONOFPROJECTRANGELINE };

public:

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年8月21号
	 */
	void setObject(const AcDbObjectId &);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年8月21号
	 */
	void initComboBoxValue(const CString &, CComboBox &, const TCHAR *, bool showCode = true);

	/* @接口
	 * @参数 
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年8月21号
	 */
	void getComboBoxCode(const CString &, const CString &, bool showCode = true);

private:

	AcDbObjectId m_obj; 
	CDatabaseOperManager *m_dbManager;

public:
	// 法人名称
	CEdit m_frmc;
	// 法人证件号
	CEdit m_frzjh;
	// 法人证件种类
	CComboBox m_frzjzl;
	// 申请人类型
	CComboBox m_sqrlx;
	// 申请人名称
	CEdit m_SQRMC;
	// 顺序号
	CEdit m_SXH;
	// 证件号
	CEdit m_zjh;
	// 证件种类
	CComboBox m_zjzl;
	// 测绘单位
	CEdit m_chdw;
	// 地上建筑面积
	CEdit m_dsjzmj;
	// 地下建筑面积
	CEdit m_dxjzmj;
	// 联系电话
	CEdit m_lxdh;
	// 联系人
	CEdit m_lxr;
	// 区县代码
	CComboBox m_qxdm;
	// 项目编号
	CEdit m_xmbh;
	// 项目名称
	CEdit m_xmmc;
	// 用地单位
	CEdit m_yddw;
	// 业务分项
	CComboBox m_ywfx;
	// 业务类型
	CComboBox m_ywlx;
	// 坐落扩展
	CEdit m_zlkz;
	// 幢数
	CEdit m_zs;
	// 用地面积
	CEdit m_ydmj;
	// 总建筑面积
	CEdit m_zjzmj;
};
