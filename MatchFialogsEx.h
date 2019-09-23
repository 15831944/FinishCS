/*******************************************

 * @brief		高级字段匹配表
 *				
 * @author		徐东江
 * @date		[2014-09-25 17:31]

 **************************************************/             


#pragma once

// CMatchFialogsEx 对话框
#include "EditListCtrl.h"	
#include "XRecordOperate.h"
#include "resource.h"
#include "StringsOpe.h"
#include "EntitysInfo.h"

class CMatchFialogsEx : public CDialog
{
	DECLARE_DYNAMIC(CMatchFialogsEx)

public:
	CMatchFialogsEx(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMatchFialogsEx();

// 对话框数据
	enum { IDD = IDD_DIALOG_MATCHEX };

	CStringArray m_strArrKey;
	CStringArray m_strArrName;
	CStringArray m_strArrValue;
	CStringArray m_strArrType;

		/*
		0 直接读取配置文件值
		1 读取扩展属性 如果关键字里包含”|“ 如[SOUTH|2]，则读取south的第2条扩展记录
		11 读取扩展属性并提取编码
		12 读取扩展属性并提取值
		2  读取数据库字典
		3 读取lstitem.mdb里的值
		。。。
	*/
    short  m_type;			

	bool		m_bShowFrameScaleControl;
	int			m_dFrameScale;

	TCHAR			m_szDivWord[100];

	//获取列表控件上的数据
	bool				getListData();

	//开始获取图上数据
	bool				 getDatas();

private:
	CEditListCtrl m_ListMain;
	CXRecordOperate m_XRecOperate;
	TCHAR				m_szListCtrlConfigFile[1000];

	CString m_strPath;
	int m_nColCount;
	int m_nRowCount;
	CString m_strHeader;
	AcDbObjectId m_ObjId;

	vector<CString>		m_vLsiteData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	bool SetObjId(const AcDbObjectId ObjId);
	bool SetConfigFile(TCHAR *szFile);
	void SetListCtrlConfigFile(TCHAR *szFile = _T("字体匹配表列表控件.XML"));

	void	showControl();

};
