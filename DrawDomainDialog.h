#pragma once

#include "resource.h"
#include "Global.h"
#include "afxwin.h"
// DrawDomainDialog �Ի���

class DrawDomainDialog
	: public CDialog
{
	DECLARE_DYNAMIC(DrawDomainDialog)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��19��
	 */
	DrawDomainDialog(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��19��
	 */
	virtual ~DrawDomainDialog();

	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_DRAWDOMAINDIALOG };

	afx_msg void OnBnClickedCheckSelectbh();

	afx_msg void OnBnClickedButtonBegindraw();

	afx_msg void OnCbnSelchangeComboDomainname();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��21��
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
