#pragma once
#include "resource.h"

// CQualityCheckSubReport �Ի���

class CQualityCheckSubReport : public CDialog
{
	DECLARE_DYNAMIC(CQualityCheckSubReport)

public:

	CQualityCheckSubReport(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CQualityCheckSubReport();

// �Ի�������
	enum { IDD = IDD_DIALOG_QUALITYCHECKSUBREPORT };

	void onApply();

	virtual BOOL OnInitDialog();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	CString m_name;
	CEdit m_context;

};
