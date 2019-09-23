#pragma once

#include "resource.h"

// InfomationOfProjectRangeLine �Ի���

class CDatabaseOperManager;

class InfomationOfProjectRangeLine : public CDialog
{
	DECLARE_DYNAMIC(InfomationOfProjectRangeLine)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��8��29��
	 */
	InfomationOfProjectRangeLine(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��8��29��
	 */
	virtual ~InfomationOfProjectRangeLine();

// �Ի�������
	enum { IDD = IDD_INFOMATIONOFPROJECTRANGELINE };

public:

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��8��21��
	 */
	void setObject(const AcDbObjectId &);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��8��21��
	 */
	void initComboBoxValue(const CString &, CComboBox &, const TCHAR *, bool showCode = true);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��8��21��
	 */
	void getComboBoxCode(const CString &, const CString &, bool showCode = true);

private:

	AcDbObjectId m_obj; 
	CDatabaseOperManager *m_dbManager;

public:
	// ��������
	CEdit m_frmc;
	// ����֤����
	CEdit m_frzjh;
	// ����֤������
	CComboBox m_frzjzl;
	// ����������
	CComboBox m_sqrlx;
	// ����������
	CEdit m_SQRMC;
	// ˳���
	CEdit m_SXH;
	// ֤����
	CEdit m_zjh;
	// ֤������
	CComboBox m_zjzl;
	// ��浥λ
	CEdit m_chdw;
	// ���Ͻ������
	CEdit m_dsjzmj;
	// ���½������
	CEdit m_dxjzmj;
	// ��ϵ�绰
	CEdit m_lxdh;
	// ��ϵ��
	CEdit m_lxr;
	// ���ش���
	CComboBox m_qxdm;
	// ��Ŀ���
	CEdit m_xmbh;
	// ��Ŀ����
	CEdit m_xmmc;
	// �õص�λ
	CEdit m_yddw;
	// ҵ�����
	CComboBox m_ywfx;
	// ҵ������
	CComboBox m_ywlx;
	// ������չ
	CEdit m_zlkz;
	// ����
	CEdit m_zs;
	// �õ����
	CEdit m_ydmj;
	// �ܽ������
	CEdit m_zjzmj;
};
