#ifndef XMJGRECONFIGURATIONDLG_H
#define XMJGRECONFIGURATIONDLG_H

// XMJGReConfigurationDlg �Ի���

#include "resource.h"
#include "aduiDock.h"

class XMJGReConfigurationDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGReConfigurationDlg)

public:
	
	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	XMJGReConfigurationDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	virtual ~XMJGReConfigurationDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECONFIGURATIONDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��08��15��
	*/
	XMJGReConfigurationDockbar();

public:

	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);

	virtual BOOL DestroyWindow();

	void setChildDlg(CDialog *, int IndexIDD);

protected:

	int m_IDD;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL AddCustomMenuItems(LPARAM hMenu);//ȥ��ͣ�������� Ĭ�ϵ��Ҽ��˵�

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

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��08��15��
	*/
	XMJGDrawDomainDockControl();

public:

	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);

	virtual BOOL DestroyWindow();

	void setChildDlg(CDialog *, int IndexIDD);

protected:

	int m_IDD;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL AddCustomMenuItems(LPARAM hMenu);//ȥ��ͣ�������� Ĭ�ϵ��Ҽ��˵�

	DECLARE_MESSAGE_MAP()

	virtual void SizeChanged(CRect *lpRect, BOOL bFloating, int flags);

public:

	CDialog *m_dlg;

};

#endif /*XMJGRECONFIGURATIONDLG_H*/