// CNewSetLayerInfoDlg 对话框
#include "resource.h"
#include "EditListCtrl.h"

enum LayerOper
{
	kDel,
	kCopy,
	kNoOper,
	kRname
};

struct SplitMargeLayer
{
	CString m_bName;
	CString m_sLayer;
	CString m_eLayer;
	AcDbObjectId m_id;
	LayerOper m_lOper;
	AcDbObjectIdArray m_ids;
	
};

typedef vector<SplitMargeLayer> SplitMargeLayers;

class CNewSetLayerInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewSetLayerInfoDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月15号
	 */
	CNewSetLayerInfoDlg(bool isLayer = false, CWnd* pParent = NULL);   // 标准构造函数
	
	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月15号
	 */
	virtual ~CNewSetLayerInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NEW_LAYERINFO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	SplitMargeLayers getSplitMargeLayers();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnBnClickedButtonAddlayer();
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonDellayer();
	afx_msg void OnBnClickedButtonCopylayer();
	afx_msg void OnBnClickedButtonSplitlayer();
	afx_msg void OnBnClickedButtonMagerlayer();

private:

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月15号
	 */
	void initListCtrl();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月15号
	 */
	void setListCtrlData();

public:

	bool m_isLayer;
	CButton m_moveup;
	AcDbObjectId m_id;
	CButton m_dellayer;
	CButton m_movedown;
	CButton m_addLayer;
	CButton m_copylayer;
	CString m_buildName;
	CButton m_splitLayer;
	CButton m_margeLayer;
	vector<CString> m_combox;
	CEditListCtrl m_LayerInfos;
	map<int, SplitMargeLayer> m_smls;

};
