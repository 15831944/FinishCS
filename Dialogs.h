#pragma once
#include "Resource.h"
#include "EditListCtrl.h"
#include "Global.h"
#include "XRecordOperate.h"
#include "macros.h"
#include "Selection_String.h"
#include "SBreakBlock.h"
#include "SFilter.h"
#include "AutoRebuild.h"
#include "aduiBaseDialog.h"
#include "EntOper.h"
#include "excelRpt.h"
#include "SReadConfig.h"

// CProjectInfoDlg �Ի���

class CProjectInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CProjectInfoDlg)

public:
	CProjectInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROJECTINFO };

	CEditListCtrl m_list;
	vector<CString>		m_vKeys;
	CString project_type;//XMJG_RoadAlignment

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();

private:
	vector<CString>			m_vFieldName;
	vector<CString>			m_vValue;

	//��ȡ��һ�����̱���Ĺ�����Ϣ����
	bool			readPreProjectInfo();//��Ŀ��Ϣ_��ʱ����_XMJG_RoadAlignment.XML/_XMJG_RedLine.XML

	//ȷ���Ի���ʱ�����û���õ���Ŀ��Ϣ��������
	bool			setPreProjectInfo();

	CString		findPreProjInfo(CString strFieldName);

};

// CFloorInfoDlg �Ի���

class CFloorInfoDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CFloorInfoDlg)

public:
	CFloorInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFloorInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLOORINFO };
	CEditListCtrl				m_list;
	CEditListCtrl				m_listText;

	AcDbObjectIdArray		m_objArr;

	//���⽨����һ������ϣ�һ�������
	AcDbObjectIdArray		m_objArrSpecail;

	double						m_dTextSize;
	FloorStaticInfo			m_FloorStatic;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);

	//��ѡ������
	afx_msg void OnBnClickedButtonSelect();
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedButtonCaculateArea();

	//��ʼ������������ɻ�������
	afx_msg void OnBnClickedButtonStatic();

	//��ʼע��
	afx_msg void OnBnClickedButtonNote();
	afx_msg void OnBnClickedButtonPosition();

private:
	bool readRuleFile();
	vector<CString>		m_vStaticInfo;

	vector<CString>			m_vRuleName;
	vector<double>			m_vParam;

	int				m_nErrorCount;

	void						clearData();
	bool	markSummaryArea(AcDbObjectId objId);

	//������ݣ���֤ʵ������Զ�����
	bool	checkEntity(int nPos, AcDbObjectId objId);

	bool	staticOrignData();

	bool	staticPrecData();

	bool	staticTextData();

	bool  staticDivideAreaText();

	void	getBuildName_Floor();

	bool  checkBuildInDivideBuild();

	bool  filterDivideEntitys();
};

// CIndoorSurveyNote �Ի���

class CIndoorSurveyNoteDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CIndoorSurveyNoteDlg)

public:
	CIndoorSurveyNoteDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIndoorSurveyNoteDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INDOORSURVEY_NOTE };
	CEditListCtrl				m_list1;
	CEditListCtrl				m_list2;

	int								m_nCount;
	double						m_dLength;
	//double						m_dHeight;

	int								m_nNoteType;
	double						m_dhh;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickListNoteType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
};


// CIndoorSurveyNote_Standard2_Dlg �Ի���

class CIndoorSurveyNote_Standard2_Dlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CIndoorSurveyNote_Standard2_Dlg)

public:
	CIndoorSurveyNote_Standard2_Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIndoorSurveyNote_Standard2_Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INDOORSURVEY_NOTE_STANDARD2 };
	CEditListCtrl				m_list1;
	CEditListCtrl				m_list2;
	int								m_nNoteType;
	TCHAR						m_szCurrentTable[100];

	CString						m_nCount;
	CString						m_dLength;
	CString						m_dHeight;
	CString						m_dDeep;
	CString						m_dSpan;		//��
	CString						m_dWelldiameter;//��
	CString						m_dTreediameter;//��

protected:
	BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnNMClickListNoteType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMClickListNoteType2(NMHDR *pNMHDR, LRESULT *pResult);
};


// CTestDlg �Ի���

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


// CExtentBuildingDlg �Ի���

class CExtentBuildingDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CExtentBuildingDlg)

public:
	CExtentBuildingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExtentBuildingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_EXTENT_BUILDING };
	CEditListCtrl		m_list;
	double				m_dDist;
	AcDbObjectId		m_objExtentMain;		//��������ʱʵ��
	AcDbObjectIdArray		m_objArrClipAttach;		//������ཻ�ĸ�����

	AcDbObjectIdArray			m_objArrExtentClip;	//������븽�����ཻ���ɵ���ʱʵ��
	vector<vector<AcDbObjectId> >		m_vObjArrRecord;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
	afx_msg void OnBnClickedButtonSelectBuilding();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCombineToMain();
	afx_msg void OnBnClickedButtonCombineToAttachment();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedChkselectall();
	afx_msg void OnBnClickedChkinvertsel();

private:
	CButton m_btnSelectAll;
	CButton m_btnInvertSel;

	AcDbObjectIdArray		m_objArrOldBuilding;

	bool			changeArcPolyLine(AcDbObjectIdArray &objArr);

	bool			changeArcPolyLine(AcDbObjectId &objId);

	bool			checkArcPolyLine(AcDbObjectIdArray objArr);
public:
	afx_msg void OnBnClickedButtonClearResult();
	afx_msg void OnBnClickedButtonDeleteOldBuilding();
};

extern CExtentBuildingDlg *g_extentBuildingDlg;

// CDrawThingsDlg �Ի���

class CDrawThingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawThingsDlg)

public:
	CDrawThingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDrawThingsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DRAWTHINGS };
	CEditListCtrl			m_list;
	CString					m_strTableName;
	CString					m_strCode;
	vector<ATTRFIELD> m_vFields;
	vector<DrawThingsData>		m_sDrawThings;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool		checkAndModifyUnderGroundNumber();


};


// CEntitysListDlg �Ի���

class CEntitysListDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CEntitysListDlg)

public:
	CEntitysListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntitysListDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ENTITYS_LIST };
	AcDbObjectIdArray			m_objIdArrSelected;
	AcDbObjectIdArray			m_objIdArr;
	CEditListCtrl					m_list;
	TCHAR							m_szConfigFileName[500];
	short								m_sType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSplash();
	afx_msg void OnBnClickedOk();
};


// CSelectTypeWizard �Ի���

class CSelectTypeWizard : public CPropertyPage
{
	DECLARE_DYNAMIC(CSelectTypeWizard)

public:
	CSelectTypeWizard();
	virtual ~CSelectTypeWizard();

	// �Ի�������
	enum { IDD = IDD_PROPPAGE_WIZARD_SELECT_TYPE };
	CEditListCtrl		m_list;
	vector<int>		m_vSelectedType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};


// CSelectPathWizard �Ի���

class CSelectPathWizard : public CPropertyPage
{
	DECLARE_DYNAMIC(CSelectPathWizard)

public:
	CSelectPathWizard();
	virtual ~CSelectPathWizard();

	// �Ի�������
	enum { IDD = IDD_PROPPAGE_WIZARD_SELECT_PATH };
	CString		m_strPathFeatureTabe;
	CString		m_strPathBuildAreaFrame;
	CString		m_strPathSummaryTable;
	CString		m_strPathTotalFrame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse1();
	afx_msg void OnBnClickedButtonBrowse2();
	afx_msg void OnBnClickedButtonBrowse3();
	afx_msg void OnBnClickedButtonBrowse4();
};

// CExportResultWizard �Ի���

class CExportResultWizard : public CPropertyPage
{
	DECLARE_DYNAMIC(CExportResultWizard)

public:
	CExportResultWizard();
	virtual ~CExportResultWizard();

	// �Ի�������
	enum { IDD = IDD_PROPPAGE_WIZARD_EXPORT_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


// CResultsCheckWizardDlg �Ի���

class CResultsCheckWizardDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CResultsCheckWizardDlg)

public:
	CResultsCheckWizardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResultsCheckWizardDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESULTS_CHECK_WIZARD };

	CButton				m_GroupControl;
	CPropertySheet	*m_pProperSheet;
 	CSelectTypeWizard			*m_SelectTypePage;		
 	CSelectPathWizard			*m_SelectPathPage;		
 	CExportResultWizard			*m_ExportResultPage;		

private:
	short					m_dPagePos;
	short					m_dPageCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPrevious();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedOk();
};

// CCreateHdmDlg dialog

class CCreateHdmDlg : public CDialog
{
	// Construction
public:
	int m_nPolyType;
	CCreateHdmDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCreateHdmDlg)
	enum { IDD = IDD_DIALOG_HDMCREATE };
	double	m_dLeft;
	double	m_dRight;
	double	m_dStep;
	int		m_nMiddle;
	double m_start;
	CAcUiPickButton m_yfh;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateHdmDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateHdmDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

// CWriteLchFileDlg dialog

class CWriteLchFileDlg : public CDialog
{
	// Construction
public:
	CWriteLchFileDlg(CWnd* pParent = NULL);   // standard constructor
	void ShowDatFilenameControl();
	void ShowIntervalControl();

	// Dialog Data
	//{{AFX_DATA(CWriteLchFileDlg)
	enum { IDD = IDD_DIALOG_WRITELCHFILE };
	CString	m_strDatFile;
	CString	m_strGcdFile;
	double	m_dInterval;
	CString	m_strLchFile;
	CString	m_strLCZMC;
	BOOL	m_bExcel;
	int		m_nType;
	BOOL	m_bChazhi;
	BOOL	m_bJiaodian;
	TCHAR   g_szUsePath[255];
	CString m_strDLMC;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteLchFileDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWriteLchFileDlg)
	virtual void OnOK();
	afx_msg void OnLchfileGcdbrowse();
	afx_msg void OnLchfileLchbrowse();
	afx_msg void OnLchfileDatbrowse();
	afx_msg void OnLchfileType1();
	afx_msg void OnLchfileType2();
	afx_msg void OnLchfileChazhi();
	virtual BOOL OnInitDialog();
	afx_msg void OnLchfileType3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// CDrawDzxDlg dialog

class CDrawDzxDlg : public CDialog
{
	// Construction
public:
	int m_nDzxType;
	CDrawDzxDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDrawDzxDlg)
	enum { IDD = IDD_DIALOG_DRAWDZX };
	double	m_dDeltz;
	double	m_dMaxH;
	double	m_dMinH;
	int		m_nNihe;
	BOOL	m_bSingle;
	double	m_dSingleH;
	double	m_dSmgc;
	double	m_dStep;
	double	m_dTolerance;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawDzxDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDrawDzxDlg)
	afx_msg void OnDzxSingle();
	virtual BOOL OnInitDialog();
	afx_msg void OnDzxNihe1();
	afx_msg void OnDzxNihe2();
	afx_msg void OnDzxNihe3();
	afx_msg void OnDzxNihe4();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ShowControl1();
	void ShowControl2();
};

#pragma once