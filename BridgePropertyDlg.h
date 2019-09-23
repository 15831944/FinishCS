#pragma once
#include "E:\VCPlatformTools\VC90\VC\atlmfc\include\afxwin.h"


// BridgePropertyDlg �Ի���

class BridgePropertyDlg
	: public CAcUiDialog
{
	DECLARE_DYNAMIC(BridgePropertyDlg)

public:
	BridgePropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~BridgePropertyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BRIDGEPROPERTRYDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonStartpt();
	afx_msg void OnBnClickedButtonEndpt();

private:

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��7��3��
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
