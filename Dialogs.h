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

// CProjectInfoDlg 对话框

class CProjectInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CProjectInfoDlg)

public:
	CProjectInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjectInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECTINFO };

	CEditListCtrl m_list;
	vector<CString>		m_vKeys;
	CString project_type;//XMJG_RoadAlignment

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();

private:
	vector<CString>			m_vFieldName;
	vector<CString>			m_vValue;

	//读取上一个工程保存的工程信息配置
	bool			readPreProjectInfo();//项目信息_临时保存_XMJG_RoadAlignment.XML/_XMJG_RedLine.XML

	//确定对话框时，把用户填好的项目信息保存起来
	bool			setPreProjectInfo();

	CString		findPreProjInfo(CString strFieldName);

};

// CFloorInfoDlg 对话框

class CFloorInfoDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CFloorInfoDlg)

public:
	CFloorInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFloorInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLOORINFO };
	CEditListCtrl				m_list;
	CEditListCtrl				m_listText;

	AcDbObjectIdArray		m_objArr;

	//特殊建筑，一半算地上，一半算地下
	AcDbObjectIdArray		m_objArrSpecail;

	double						m_dTextSize;
	FloorStaticInfo			m_FloorStatic;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);

	//框选建筑物
	afx_msg void OnBnClickedButtonSelect();
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedButtonCaculateArea();

	//开始汇总面积，生成汇总文字
	afx_msg void OnBnClickedButtonStatic();

	//开始注记
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

	//检查数据，保证实体的属性都正常
	bool	checkEntity(int nPos, AcDbObjectId objId);

	bool	staticOrignData();

	bool	staticPrecData();

	bool	staticTextData();

	bool  staticDivideAreaText();

	void	getBuildName_Floor();

	bool  checkBuildInDivideBuild();

	bool  filterDivideEntitys();
};

// CIndoorSurveyNote 对话框

class CIndoorSurveyNoteDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CIndoorSurveyNoteDlg)

public:
	CIndoorSurveyNoteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIndoorSurveyNoteDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INDOORSURVEY_NOTE };
	CEditListCtrl				m_list1;
	CEditListCtrl				m_list2;

	int								m_nCount;
	double						m_dLength;
	//double						m_dHeight;

	int								m_nNoteType;
	double						m_dhh;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickListNoteType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
};


// CIndoorSurveyNote_Standard2_Dlg 对话框

class CIndoorSurveyNote_Standard2_Dlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CIndoorSurveyNote_Standard2_Dlg)

public:
	CIndoorSurveyNote_Standard2_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIndoorSurveyNote_Standard2_Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INDOORSURVEY_NOTE_STANDARD2 };
	CEditListCtrl				m_list1;
	CEditListCtrl				m_list2;
	int								m_nNoteType;
	TCHAR						m_szCurrentTable[100];

	CString						m_nCount;
	CString						m_dLength;
	CString						m_dHeight;
	CString						m_dDeep;
	CString						m_dSpan;		//跨
	CString						m_dWelldiameter;//管
	CString						m_dTreediameter;//胸

protected:
	BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnNMClickListNoteType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMClickListNoteType2(NMHDR *pNMHDR, LRESULT *pResult);
};


// CTestDlg 对话框

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


// CExtentBuildingDlg 对话框

class CExtentBuildingDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CExtentBuildingDlg)

public:
	CExtentBuildingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExtentBuildingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXTENT_BUILDING };
	CEditListCtrl		m_list;
	double				m_dDist;
	AcDbObjectId		m_objExtentMain;		//扩面后的临时实体
	AcDbObjectIdArray		m_objArrClipAttach;		//扩面后相交的附属物

	AcDbObjectIdArray			m_objArrExtentClip;	//扩面后与附属物相交生成的临时实体
	vector<vector<AcDbObjectId> >		m_vObjArrRecord;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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

// CDrawThingsDlg 对话框

class CDrawThingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawThingsDlg)

public:
	CDrawThingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDrawThingsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DRAWTHINGS };
	CEditListCtrl			m_list;
	CString					m_strTableName;
	CString					m_strCode;
	vector<ATTRFIELD> m_vFields;
	vector<DrawThingsData>		m_sDrawThings;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool		checkAndModifyUnderGroundNumber();


};


// CEntitysListDlg 对话框

class CEntitysListDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CEntitysListDlg)

public:
	CEntitysListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntitysListDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENTITYS_LIST };
	AcDbObjectIdArray			m_objIdArrSelected;
	AcDbObjectIdArray			m_objIdArr;
	CEditListCtrl					m_list;
	TCHAR							m_szConfigFileName[500];
	short								m_sType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSplash();
	afx_msg void OnBnClickedOk();
};


// CSelectTypeWizard 对话框

class CSelectTypeWizard : public CPropertyPage
{
	DECLARE_DYNAMIC(CSelectTypeWizard)

public:
	CSelectTypeWizard();
	virtual ~CSelectTypeWizard();

	// 对话框数据
	enum { IDD = IDD_PROPPAGE_WIZARD_SELECT_TYPE };
	CEditListCtrl		m_list;
	vector<int>		m_vSelectedType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};


// CSelectPathWizard 对话框

class CSelectPathWizard : public CPropertyPage
{
	DECLARE_DYNAMIC(CSelectPathWizard)

public:
	CSelectPathWizard();
	virtual ~CSelectPathWizard();

	// 对话框数据
	enum { IDD = IDD_PROPPAGE_WIZARD_SELECT_PATH };
	CString		m_strPathFeatureTabe;
	CString		m_strPathBuildAreaFrame;
	CString		m_strPathSummaryTable;
	CString		m_strPathTotalFrame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse1();
	afx_msg void OnBnClickedButtonBrowse2();
	afx_msg void OnBnClickedButtonBrowse3();
	afx_msg void OnBnClickedButtonBrowse4();
};

// CExportResultWizard 对话框

class CExportResultWizard : public CPropertyPage
{
	DECLARE_DYNAMIC(CExportResultWizard)

public:
	CExportResultWizard();
	virtual ~CExportResultWizard();

	// 对话框数据
	enum { IDD = IDD_PROPPAGE_WIZARD_EXPORT_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


// CResultsCheckWizardDlg 对话框

class CResultsCheckWizardDlg : public CAdUiBaseDialog
{
	DECLARE_DYNAMIC(CResultsCheckWizardDlg)

public:
	CResultsCheckWizardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResultsCheckWizardDlg();

// 对话框数据
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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