/*******************************************

 * @brief		�߼��ֶ�ƥ���
 *				
 * @author		�춫��
 * @date		[2014-09-25 17:31]

 **************************************************/             


#pragma once

// CMatchFialogsEx �Ի���
#include "EditListCtrl.h"	
#include "XRecordOperate.h"
#include "resource.h"
#include "StringsOpe.h"
#include "EntitysInfo.h"

class CMatchFialogsEx : public CDialog
{
	DECLARE_DYNAMIC(CMatchFialogsEx)

public:
	CMatchFialogsEx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMatchFialogsEx();

// �Ի�������
	enum { IDD = IDD_DIALOG_MATCHEX };

	CStringArray m_strArrKey;
	CStringArray m_strArrName;
	CStringArray m_strArrValue;
	CStringArray m_strArrType;

		/*
		0 ֱ�Ӷ�ȡ�����ļ�ֵ
		1 ��ȡ��չ���� ����ؼ����������|�� ��[SOUTH|2]�����ȡsouth�ĵ�2����չ��¼
		11 ��ȡ��չ���Բ���ȡ����
		12 ��ȡ��չ���Բ���ȡֵ
		2  ��ȡ���ݿ��ֵ�
		3 ��ȡlstitem.mdb���ֵ
		������
	*/
    short  m_type;			

	bool		m_bShowFrameScaleControl;
	int			m_dFrameScale;

	TCHAR			m_szDivWord[100];

	//��ȡ�б�ؼ��ϵ�����
	bool				getListData();

	//��ʼ��ȡͼ������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	bool SetObjId(const AcDbObjectId ObjId);
	bool SetConfigFile(TCHAR *szFile);
	void SetListCtrlConfigFile(TCHAR *szFile = _T("����ƥ����б�ؼ�.XML"));

	void	showControl();

};
