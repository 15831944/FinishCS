// CNewSetLayerInfoDlg �Ի���
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

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��15��
	 */
	CNewSetLayerInfoDlg(bool isLayer = false, CWnd* pParent = NULL);   // ��׼���캯��
	
	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��15��
	 */
	virtual ~CNewSetLayerInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NEW_LAYERINFO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��15��
	 */
	void initListCtrl();

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��15��
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
