#include "resource.h"
#include "QualityCheckSubReport.h"
// CQualityCheckReport �Ի���

class CQualityCheckReport 
	: public CDialog
{
	DECLARE_DYNAMIC(CQualityCheckReport)

public:
	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��16��
	 */
	CQualityCheckReport(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��16��
	 */
	virtual ~CQualityCheckReport();

// �Ի�������
	enum { IDD = IDD_DIALOG_QUALITYCHECKREPORT };

	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabQualitycheckreport(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	int m_CurSelTab;
	CQualityCheckSubReport* subDlag[4];
	virtual void OnOK();
};
