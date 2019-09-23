#include "resource.h"
#include "QualityCheckSubReport.h"
// CQualityCheckReport 对话框

class CQualityCheckReport 
	: public CDialog
{
	DECLARE_DYNAMIC(CQualityCheckReport)

public:
	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月16号
	 */
	CQualityCheckReport(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月16号
	 */
	virtual ~CQualityCheckReport();

// 对话框数据
	enum { IDD = IDD_DIALOG_QUALITYCHECKREPORT };

	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabQualitycheckreport(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	int m_CurSelTab;
	CQualityCheckSubReport* subDlag[4];
	virtual void OnOK();
};
