#ifndef XMJGEXPORTDLG
#define XMJGEXPORTDLG

#include "Resource.h"
#include <list>
#include <EditListCtrl.h>
#include <map>
#include <afxcmn.h>
#include "aduiDialog.h"
#include "Global.h"
using namespace std;
using std::list;

// XMJGExportDlg 对话框

#ifndef _RIGHTS_HOLDER_MANAGER_
#include "DatabaseOperManager.h"
#endif /*_RIGHTS_HOLDER_MANAGER_*/

namespace SmartTable
{
	class Word;
};

class IWindowControl
{
public:

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月22日
	*/
	virtual ~IWindowControl()
	{
	}

	/* @接口 获取窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void getWindowControl(MStr &) = 0;

	/* @接口 设置窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void setWindowControl(MStr &) = 0;

};

#pragma region 业务一信息设置对话框

class XMJGSetInfo1
	: public CDialog

{
	DECLARE_DYNAMIC(XMJGSetInfo1)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	XMJGSetInfo1(bool, CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月17日
	*/
	virtual ~XMJGSetInfo1();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGSETINFO1
	};

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月23日
	 */
	int getEntityColor();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	afx_msg void OnBnClickedOk();

public:

	CEdit m_AcceptCode;
	CEdit m_BuildProject;
	CEdit m_EntrustCompany;
	CEdit m_projectcontext;
	CComboBox m_MeansureContext;

private:

	bool m_isCreate;
	int m_entityColor;
	vector<CString> m_MeansureType;

};

#pragma endregion

#pragma region 业务二信息设置对话框

class XMJGSetInfo2
	: public CAcUiDialog
{
	DECLARE_DYNAMIC(XMJGSetInfo2)

private:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月16日
	 */
	XMJGSetInfo2(bool, CWnd* pParent = NULL, HINSTANCE hInstance = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月16日
	*/
	virtual ~XMJGSetInfo2();

public:

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGSETINFO2
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月16日
	 */
	static XMJGSetInfo2 *Instance(bool isCreate);

	static void destory();

#ifdef _WIN64
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
#else
	afx_msg LONG onAcadKeepFocus(UINT n, LONG l);
#endif

public:

	CEdit m_BuildArea;
	CEdit m_AcceptCode;
	CEdit m_ProjectName;
	CEdit m_LicenseCode;
	CEdit m_BuildCompany;
	CEdit m_ProjectAddress;

private:

	bool m_isCreate;
	static XMJGSetInfo2 *_setInfo;

public:
	afx_msg void OnClose();
	CEdit m_projectdir;
};

#pragma endregion

#pragma region 业务三信息设置对话框

class XMJGSetInfo3
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGSetInfo3)

public:
	XMJGSetInfo3(bool, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGSetInfo3();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGSETINFO3
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEdit m_CheckCode;
	CEdit m_BuildArea;
	CEdit m_projectdir;
	CEdit m_AcceptCode;
	CEdit m_ProjectAdd;
	CEdit m_ProjectName;
	CEdit m_LicenseCode;
	CEdit m_BuildCompany;

public:

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

private:

	bool m_isCreate;

};

#pragma endregion

#pragma region 业务四信息设置对话框

class XMJGSetInfo4
	: public CDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJGSetInfo4)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	XMJGSetInfo4(bool, CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月17日
	*/
	virtual ~XMJGSetInfo4();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGSETINFO4
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEdit m_AcceptCode;
	CEdit m_projectdir;
	CEdit m_ProjectName;
	CEdit m_EntrustCompany;

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	afx_msg void OnBnClickedOk();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual BOOL OnInitDialog();

	/* @接口 获取窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void getWindowControl(MStr &);

	/* @接口 设置窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void setWindowControl(MStr &);

private:

	bool m_isCreate;

public:
};

#pragma endregion

#pragma region 业务五信息设置对话框

class XMJGSetInfo5
	: public CAcUiDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJGSetInfo5)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	XMJGSetInfo5(bool, CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月17日
	*/
	~XMJGSetInfo5();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGSETINFO5
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
#ifdef _WIN64
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
#else
	afx_msg LONG onAcadKeepFocus(UINT n, LONG l);
#endif

public:

	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();

	static XMJGSetInfo5 *Instance(bool);

	static void destory();

	/* @接口 获取窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void getWindowControl(MStr &);

	/* @接口 设置窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void setWindowControl(MStr &);

public:

	CEdit m_AcceptCode;
	CEdit m_BuildCompany;
	CEdit m_ProjectName;
	CEdit m_ProjectAdd;
	CEdit m_LicenseCode;
	CEdit m_CheckCode;
	CEdit m_Infomation;
	CEdit m_BuildArea;

private:

	bool m_isCreate;
	static XMJGSetInfo5 *_setinfo;

public:
	CEdit m_projectdir;
};

#pragma endregion

#pragma region 业务六人行市政天桥

class XMJGSetInfo6
	: public CAcUiDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJGSetInfo6)

public:

	XMJGSetInfo6(bool, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGSetInfo6();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGSETINFO6
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	afx_msg void OnBnClickedOk();

	/* @接口 获取窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void getWindowControl(MStr &);

	/* @接口 设置窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void setWindowControl(MStr &);

public:

	CEdit m_CheckCode;
	CEdit m_AcceptCode;
	CEdit m_ProjectAdd;
	CEdit m_projectdir;
	CEdit m_BridgeType;
	CEdit m_Entrustment;
	CEdit m_LicenseCode;
	CEdit m_ProjectName;
	CComboBox m_MeasureCompany;

private:

	bool m_isCreate;

public:
	afx_msg void OnEnChangeEditPipelinebuildproject();
	CEdit m_pipelinebuildproject;
	CEdit m_pipelinebuildcompany;
	CEdit m_pipelinebuildarea;
};

#pragma endregion

#pragma region 业务七信息设置对话框

class XMJGSetInfo7
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGSetInfo7)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月18日
	 */
	XMJGSetInfo7(bool, CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月18日
	*/
	virtual ~XMJGSetInfo7();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGSETINFO7
	};

protected:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
#ifdef _WIN64
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
#else
	afx_msg LONG onAcadKeepFocus(UINT n, LONG l);
#endif

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEdit m_AcceptCode;
	CEdit m_projectdir;
	CEdit m_ProjectName;
	CEdit m_hprojectcode;
	CEdit m_BuildCompany;
	CEdit m_ShiShiCompany;
	CEdit m_ProjectAddress;
	CDateTimeCtrl m_exportTime;
	CComboBox m_MeansureCompany;
	CDateTimeCtrl m_datatimeapply;

public:

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

private:

	bool m_isCreate;

};

#pragma endregion

#pragma region 业务一成果输出对话框

#pragma endregion

#pragma region 业务二成果输出对话框

class XMJG2ExportDlg
	: public CAcUiDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJG2ExportDlg)

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月21日
	 */
	XMJG2ExportDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月23日
	 */
	XMJG2ExportDlg(const CString &);

public:

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月21日
	*/
	virtual ~XMJG2ExportDlg();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJEXPORTINFO2
	};

protected:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
#ifdef _WIN64
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
#else
	afx_msg LONG onAcadKeepFocus(UINT n, LONG l);
#endif

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	void setSavePath(const CString &);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

	/* @接口 设置读取注册表的位置
	* @参数 bool 默认为true（表示读取业务二的注册表信息）
	如果为false（表示读取业务五的注册表信息）
	* @作者 杨发荷
	* @时间 2017年06月20日
	*/
	void SetRegPath(bool flag = true);

	/* @接口 获取窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void getWindowControl(MStr &);

	/* @接口 设置窗口控件的内容
	 * @参数 窗口控件的值集合
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	virtual void setWindowControl(MStr &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月21日
	 */
	static XMJG2ExportDlg *Instance(const CString &);

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月23日
	 */
	static void destory();

	afx_msg void OnBnClickedDistance();

	afx_msg void OnBnClickedBetween();

	/* @接口 0:厦门市建设工程规划放样验线申请表附表.docx	1:厦门市建设工程0-0验线（建筑类)申请表附表.docx
	 * @作者 杨发荷
	 * @时间 2017年08月28日
	 */
	void exportWord(MStr &, int nflag = 0);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月28日
	 */
	void readDataBase(SmartTable::Word *, MStr &);

public:

	CEdit m_ProjectName;
	CEdit m_BuildCompany;
	CEdit m_ProjectAdd;
	CEdit m_ProjectCode;
	CEdit m_EdgeDistance;
	CEdit m_BuildDistance;
	CEdit m_SCEdgeDistance;
	CEdit m_SCBuildDistance;
	CButton m_BuildWall;
	CButton m_BuildAixs;

private:

	CString m_Area;
	CString m_Name;
	MStr m_docname;
	CString RegPath;
	CString m_SaveDir;
	static XMJG2ExportDlg *_setinfo;

public:
};

#pragma endregion

#pragma region 业务三成果输出对话框

#pragma endregion

#pragma region 业务四成果输出对话框

#pragma endregion

#pragma region 业务五成果输出对话框

#pragma endregion

class XMJGControlPointDlg
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGControlPointDlg)

public:
	XMJGControlPointDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGControlPointDlg();

	// 对话框数据
	enum
	{
		IDD = IDD_CONTROLPOINTATTRIBUTE
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

	CString getControlPointRank() const;

	CString getControlPointType() const;

public:

	CComboBox m_ControlPoinType;
	CComboBox m_ControlPointRank;

private:

	CString m_Rank;
	CString m_Type;

};

// XMJGPlanDlg 设置分层对话框

class XMJGPlanDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGPlanDlg)

public:

	XMJGPlanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGPlanDlg();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGPLAN
	};

public:

	VStr getLayerInfo();

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedPlanAdd();

	afx_msg void OnBnClickedPlanDel();

	void setObjectID(const AcDbObjectId &);

	afx_msg void OnBnClickedButtonMoveup();

	afx_msg void OnBnClickedButtonMovedown();

private:

	void getFloorNum(CString &);

	void addFloorNum(const CString &);

	CString getFloorNumByList(int) const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	VStr m_LayerInfo;
	AcDbObjectId m_ObjectID;

public:
	CEdit m_FloorNum1;
	CEdit m_FloorNum2;
	CEditListCtrl m_layerinfo;
};

// XMJGSetStratPlanInfo 对话框

class XMJGSetStratPlanInfo
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGSetStratPlanInfo)

public:
	XMJGSetStratPlanInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGSetStratPlanInfo();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOG_SETSTRATIFINCATIONPLANINFO
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEdit m_PlanCode;
	CEdit m_FloorCode;
	CEdit m_FloorHeight;
	CButton m_IsMainFloorHeight;

public:

	void setObjectID(const AcDbObjectId &);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

private:

	void setLayerInfo(const CString &);

	void getControlInfo(CWnd *, const TCHAR *);

	void setControlInfo(CWnd *, const TCHAR *);

private:

	AcDbObjectId m_ObjectID;

public:
	CButton m_ParapetWall;
	CButton m_MainFloorHeight;
};

// XMJGFunctionsAreasInfo 对话框

class XMJGFunctionsAreasInfo
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGFunctionsAreasInfo)

public:

	XMJGFunctionsAreasInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGFunctionsAreasInfo();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOG_FUNCTIONALAREAS
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CComboBox m_tiname;
	CEdit m_FunctionsCode;				//功能区的编号
	CEdit m_FunctionsHeight;			//功能区的高度
	CEdit m_AreaCoefficient;			//面积系数
	CEdit m_FunctionsFactor;			//计容系数

public:

	/* @接口 功能区信息设置对话框初始化
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	virtual BOOL OnInitDialog();

	/* @接口 点击ok事件
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	afx_msg void OnBnClickedOk();

	/* @接口 点击取消按钮
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	afx_msg void OnBnClickedCancel();

	/* @接口 新增功能区类型
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	afx_msg void OnClickedXmjgadd();

	/* @接口 删除已有的功能区类型
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	afx_msg void OnClickedXmjgdel();

	FunctionNameTableEx getFNTInfo();

	afx_msg void OnFunctionChangeName();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月18号
	 */
	void setEntInfo(const AcDbObjectId &, const CString &);

private:

	bool getWindowText(FunctionNameTable &);

public:

	int m_len; 
	int m_island;
	CString m_height;
	AcDbObjectId m_ObjectID;
	FunctionNameTableEx m_fnt;
	CComboBox m_functionnames;
	FunctionNameTables m_fnts;
};

// XMJGRedLineQuality 对话框

class XMJGRedLineQuality : public CDialog
{
	DECLARE_DYNAMIC(XMJGRedLineQuality)

public:
	XMJGRedLineQuality(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGRedLineQuality();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJGREDLINEQUALITY
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedCancel();

	virtual BOOL OnInitDialog();

public:

	CButton m_RedLineCoorAnn;
	CButton m_RedLineLenghtAnn;
	CButton m_RedLineSegmentAnn;
	CButton m_NoRedLineCrossNodeAnn;

private:

	bool m_CheckCoorAnn;
	bool m_CheckLenghtAnn;
	bool m_CheckSegmentAnn;
	bool m_CheckCrossNodeAnn;

public:

	bool getCheckCoorAnn();

	bool getCheckLenghtAnn();

	bool getCheckSegmentAnn();

	bool getCheckRedLineNodeAnn();

};

// CDrawSection dialog

class CDrawSection : public CAcUiDialog
{

public:

	BOOL m_bAskJiange; //是否提示断面图间隔
	BOOL m_bAskHpmt;  //是否提示绘平面图
	BOOL m_bAskBegin; //是否提示起始里程
	AcGePoint2dArray m_Nodes; //断面各点坐标
	AcGePoint2dArray m_NextNodes; //断面设计线
	int m_oldPick;
	ads_name m_Ent; //断面线
	double m_dLicheng; //横断面里程

	void draw();
	CDrawSection(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDrawSection();

	enum
	{
		IDD = IDD_DIALOG_ZONGDUAN
	};

	double	m_biaochi;
	BOOL	m_ifbch;
	BOOL	m_iffg;
	int		m_jgnum;
	double	m_jgx;
	double	m_jgy;
	int		m_scalex;
	int		m_scaley;
	int		m_fgx;
	int		m_fgy;
	int		m_Licheng;
	int		m_height;
	double	m_dBeginLC;
	double	m_PlaceX;
	double	m_PlaceY;
	int		m_nHpmt;
	double	m_dWidth;
	double	m_dLchHigh;
	double	m_dLchMin;
	int		m_nLchWei;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnZongIfbch();
	afx_msg void OnZongIffg();
	virtual void OnOK();
	afx_msg void OnZongPick();

	DECLARE_MESSAGE_MAP()
private:

	void DrawPmt(AcDbGroup *pGroup, double XM);
	void DrawDuanmianLine(AcGePoint2dArray& Nodes, ads_point pt0, double dScaleX, double dScaleY,
		double zhMin, int bgMin, AcDbGroup *pGroup, bool bNext);

};

class XMJGReadReg
{
public:
	XMJGReadReg();
	CString ReadReg(const TCHAR *);
	void WriteReg(const TCHAR *, const TCHAR *);
	~XMJGReadReg();
private:
	HKEY m_rkey;
};

// CSetProjectCodeDlg 对话框

class XMJGProjectCodeDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGProjectCodeDlg)

public:

	XMJGProjectCodeDlg(CWnd* pParent = NULL);   // 标准构造函数

	XMJGProjectCodeDlg(const TCHAR *, bool);

	virtual ~XMJGProjectCodeDlg();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJG_SETPROJECTCODE
	};

public:

	CString GetAcceptCode();

	CString GetProjectDir();

	void SetWindowTitle(const TCHAR *);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	bool createProjectDir();

public:

	CEdit m_SetAcceptCode;
	CEdit m_ProjectDir;

private:

	CString m_dir;
	CString m_code;
	CString m_name;
	bool m_isconfig;
	CString m_title;
	XMJGReadReg m_reg;
	CString m_regPath;
	CDatabaseOperManager *_manager;

};

// XMJGLicenseDocDlg 对话框

class XMJGLicenseDocDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGLicenseDocDlg)

	typedef vector<CString> VStr;
	typedef map<int, CString> MIStr;
	typedef map<CString, CString> MStr;

public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	XMJGLicenseDocDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	virtual ~XMJGLicenseDocDlg();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJG_LINCESEDOCDLG
	};

public:

	void getLicenseDoc(MStr &);

private:

	/* @接口 读取MDB
	 * @参数 VMStr 读取的数据
	 * @参数 bool 读取MDB成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年06月28日
	 */
	bool ReadMDB();

	/* @接口 初始化窗体控件
	 * @参数 VMStr 初始化控件数据
	 * @参数 bool 初始化控件成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年06月28日
	 */
	bool InitListCtrl();

public:

	/* @接口 初始化对话框
	 * @作者 杨发荷
	 * @时间 2017年06月28日
	 */
	virtual BOOL OnInitDialog();

	virtual void OnOK();

	void OnLvnClickListItem(NMHDR *pNMHDR, LRESULT *pResult);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEditListCtrl m_LicenseDoc;

private:

	MStr m_Ret;
	VStr m_data;
	MIStr m_Data;

};

// XMJGChangePictureProperty 对话框

#define WM_SET_ITEMS	WM_USER + 0x7FFC

class XMJGChangePictureProperty
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGChangePictureProperty)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	XMJGChangePictureProperty(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	XMJGChangePictureProperty(const TCHAR *);

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	virtual ~XMJGChangePictureProperty();

	// 对话框数据
	enum
	{
		IDD = IDD_XMJG_CHANGEPICTUREPROPERTY
	};

	/* @接口 设置图片属性的类型
	 * @参数 CString 类型
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月1号
	 */
	void setPicturePropertyType(const CString &);

protected:

	/* @接口 将图片上移
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	afx_msg void OnBnClickedXMJGMMoveUp();

	/* @接口 将图片置顶
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	afx_msg void OnBnClickedXMJGMoveTop();

	/* @接口 将图片下移
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	afx_msg void OnBnClickedMJGMoveDown();

	/* @接口 将图片置底
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	afx_msg void OnBnClickedMJGMoveButtom();

	/* @function 浏览图片事件
	* @author 杨发荷
	* @time 2016年10月23号
	*/
	afx_msg void OnClickedBrowIngPicture(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedXmjgAddPixture();

	afx_msg void OnBnClickedXMJGDelPicture();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	/* @接口 窗口初始化
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	virtual BOOL OnInitDialog();

	/* @接口 初始化ListCtrl控件
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	void InitListCtrl();

	DECLARE_MESSAGE_MAP()

private:

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月29日
	 */
	void swapPicturePosition(int, int);

	/* @接口 读取配置文件
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	void readConfigFile();

	/* @接口 读取图片信息
	 * @作者 杨发荷
	 * @时间 2017年09月18日
	 */
	void readPictureInfo();

	void writeConfigFile();

	/* @接口 写图片信息
	 * @作者 杨发荷
	 * @时间 2017年09月18日
	 */
	void writePictureInfo();

	bool selectPictures();

	bool selectPicture(int);

	void getConfigProperty();

	void setListShow(int);

	void changePictureOrder(int, MStr &);

public:

	CEditListCtrl m_PictureBrowingInfo;

private:


	CString m_dir;
	CString m_type;
	CString m_combox;
	MStr m_nameproperty;
	MStr m_propertyname;
	VMStr m_records;

public:
	CButton m_moveup;
	CButton m_movedown;
	CButton m_movetop;
	CButton m_movebuttom;
};

class XMJGFunctionDefine : public CDialog
{
	DECLARE_DYNAMIC(XMJGFunctionDefine)

public:
	XMJGFunctionDefine(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGFunctionDefine();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOG_XMJGFUNCTIONDEFINE
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedXMJGAdd();
	afx_msg void OnClickedXMJGDel();
	afx_msg void OnFunctionChangeName();
public:
	CEdit m_jrxs;
	CEdit m_mjxs;
	CComboBox m_functionnames;
	FunctionNameTables m_fnts;

};

struct LayerObject
{
	CString m_elayer;
	CString m_slayer;
	AcDbObjectId m_id;
	CString m_scheight;
	CString m_ghheight;
};

typedef map<int, LayerObject> LayerObjects;
typedef LayerObjects::iterator LayerObjectsIter;
typedef map<CString, LayerObjects > BuildLayerObjects;
typedef BuildLayerObjects::iterator BuildLayerObjectsIter;

class XMJGChangeLayerInfoDlg
	: public CDialog

{
	DECLARE_DYNAMIC(XMJGChangeLayerInfoDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年09月26日
	 */
	XMJGChangeLayerInfoDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @时间 2017年09月26日
	 */
	virtual ~XMJGChangeLayerInfoDlg();

	// 对话框数据
	enum
	{
		IDD = IDD_DIALOG_XMJG_CHANGELAYERINFO
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	virtual void OnOK();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	afx_msg void OnChangeBuildName();

private:

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool initDialog();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool initComboBox();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool initListCtrl();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	bool setLayerInfo(const CString &);

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	bool saveLayerInfo(const CString &);

public:

	CEditListCtrl m_layerinfo;

	CComboBox m_buildname;

private:

	/*	MVMStr m_records;*/
	BuildLayerObjects m_buildInfo;

public:
	afx_msg void OnBnClickedButtonDelete();
};

// XMJGSubCompressDlg 对话框

class XMJGSubCompressDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGSubCompressDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年09月26日
	 */
	XMJGSubCompressDlg(const TCHAR *);   // 标准构造函数

	/* @接口 默认构造接口
	 * @作者 杨发荷
	 * @时间 2017年10月30日
	 */
	XMJGSubCompressDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构接口
	 * @作者 杨发荷
	 * @时间 2017年10月30日
	 */
	virtual ~XMJGSubCompressDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SUBCOMPRESSDLG };

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	virtual void OnOK();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月30日
	 */
	void setWindowName(const CString &, const CString &, const CString &, const CString &);

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年10月9日
	 */
	CString getCompressFiles();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月31日
	 */
	void addCompressFile(const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月31日
	 */
	bool checkFileIsComplete();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月2日
	 */
	void deleteFiles();

	void judgeFileOccupy();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年10月9日
	 */
	void initListCtrl();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月30日
	 */
	void setListCtrlData();

public:

	VStr m_deficiencyfiles;
	CEditListCtrl m_compressfiles;

private:

	CString m_name;
	CString m_type;
	bool m_iscomplete;
	CString m_zippath;
	CString m_tempPath;
	CString m_codename;
	CString m_typePath;
	CStringArray m_files;
	CStringArray m_temps;
	CString m_projectname;
	CString m_projectcode;
	CString m_projectpath;
	CStringArray m_zipfile;

};

// CChangeCarPro 对话框

class CChangeCarPro : public CDialog
{
	DECLARE_DYNAMIC(CChangeCarPro)

public:

	CChangeCarPro(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CChangeCarPro();

	virtual BOOL OnInitDialog();

	void setObjects(const AcDbObjectId &);

	void setObjects(const AcDbObjectIdArray &);

private:

	bool readDataBase();

	void setComboBoxState(const CString &);

	void setCarNumCtrl(const CString &);

	void setBarrierCarCtrl(const CString &);

	afx_msg void OnCbnSelchangecartype();

	afx_msg void OnBnClickedOk();

	// 对话框数据
	enum { IDD = IDD_DIALOG_CHANGECARPRO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	AcDbObjectIdArray m_objects;

public:

	VStr m_barrier;
	MStr m_cartypes;
	CButton m_dixia;
	CButton m_dishang;
	MSInt m_nameindex;
	MSInt m_indexloca;
	CEdit m_procarnum;
	CString m_cartype;
	CComboBox m_procartype;

};

// CProjectInfoSettingDlg 对话框

class CProjectBaseInfoSettingDlg
	: public CDialog
{

	DECLARE_DYNAMIC(CProjectBaseInfoSettingDlg)

public:

	/* @接口 默认构造接口
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	CProjectBaseInfoSettingDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构接口
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	virtual ~CProjectBaseInfoSettingDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_XMJG_PROJECTBASEINFOSETTINGDLG };

	virtual void OnOK();

protected:

	DECLARE_MESSAGE_MAP()

	virtual void OnCancel();

	/* @接口 初始化对话框
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	CEdit m_ProjectDM;
	CEdit m_projectname;
	CEdit m_licensedocs;
	CEdit m_detailsurvey;
	CEdit m_buildcompany;
	CEdit m_controlpoints;
	CEdit m_acceptancecode;
	CEdit m_projectaddress;
	CEdit m_conductcompany;
	CEdit m_meansurecompany;
	CEdit m_controlbasepoint;
	CDateTimeCtrl m_exportTime;
	CDateTimeCtrl m_datatimeapply;
	CEdit m_measureReport;
	CEdit m_buildAreaMeasure;
	CEdit m_measureMethod;
};

class CDeficiencyFilesDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CDeficiencyFilesDlg)

public:

	CDeficiencyFilesDlg(const CString &, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CDeficiencyFilesDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_DEFICIENCYFILESDLG };

protected:

	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	bool isDeficiencyFile();

	void addDeficiencyFile(const VStr &);

public:

	CEditListCtrl m_deficiencylist;

private:

	VStr m_files;
	CString m_title;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};

// CBaseProjectInfoDlg 对话框

class CProjectExportDocSettingDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CProjectExportDocSettingDlg)

public:

	virtual void OnOK();

	virtual void OnCancel();

	virtual BOOL OnInitDialog();

	CProjectExportDocSettingDlg(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CProjectExportDocSettingDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_XMJG_PROJECTEXPORTDOCSETTINGDLG };

protected:

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	CEdit m_jzlmyq;
	CEdit m_ghjzjj;
	CEdit m_ghthxjj;
	CEdit m_scthxjj;
	CEdit m_scgjzjj;
	CEdit m_ghqtghyq;
	CEdit m_scjzlmyq;
	CEdit m_scqtghyq;
	CEdit m_qtxysmdqk;
	CEdit m_yghxkbyzqk;
	CButton m_lssssfcc1;
	CButton m_lssssfcc2;

	CEdit m_ghggjzpt;
	CEdit m_scggjzpt;
};

// CProjectAreaSettingDlg 对话框

class CProjectAreaSettingDlg
	: public CDialog
{

	DECLARE_DYNAMIC(CProjectAreaSettingDlg)

public:

	virtual void OnOK();

	virtual void OnCancel();

	virtual BOOL OnInitDialog();

	CProjectAreaSettingDlg(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CProjectAreaSettingDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_XMJG_PROJECTAREASETTINGDLG };

protected:

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	CEdit m_sm;
	CEdit m_jzmd;
	CEdit m_dscw;
	CEdit m_dxcw;
	CEdit m_jzydmj;
	CEdit m_ghdsmj;
	CEdit m_ghdxmj;
	CEdit m_ghjrmj;
	CEdit m_ghzjzmj;
	CEdit m_scjzydsm;
	CEdit m_ghdsmjsm;
	CEdit m_ghdxmjsm;
	CEdit m_scdsmjsm;
	CEdit m_scdxmjsm;
	CEdit m_ghjrmjsm;
	CEdit m_scjrmjsm;
	CEdit m_ghzjzmjsm;
	CEdit m_ghzycrkwz;
	CEdit m_sczjzmjsm;
	CEdit m_sczycrkwz;

};

// CExportDocInfoDlg 对话框

class CExportDocInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportDocInfoDlg)

public:
	CExportDocInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportDocInfoDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_XMJG_EXPORTINFODLG };

protected:

	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	void OnTcnSelchangeTab(NMHDR *, LRESULT *);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	CTabCtrl m_tab;

private:

	CProjectAreaSettingDlg m_pasdlg;
	CProjectBaseInfoSettingDlg m_pbisdlg;
	CProjectExportDocSettingDlg m_pedsdlg;

	virtual void OnCancel();
};

class CCompressDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CCompressDlg)

public:

	CCompressDlg(CWnd* pParent = NULL);   // 标准构造函数

	CCompressDlg(const CString &);

	virtual ~CCompressDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_XMJGCOMPRESSDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
private:

	CString m_type;
	CString m_path;
	CString m_name;
	CString m_zippath;
	CString m_tempPath;
	CString m_codename;

public:
	CEdit m_compresscode;
	CEdit m_compressname;
	CTabCtrl m_tab;
	XMJGSubCompressDlg m_subck;
	XMJGSubCompressDlg m_subgc;
	XMJGSubCompressDlg m_subcg;
	afx_msg void OnBnClickedButtonCompressallselect();
	afx_msg void OnBnClickedButtonCompressreverse();
	afx_msg void OnBnClickedButtonCompressaddfile();
	void selectAllFile(XMJGSubCompressDlg &cursel);
	void selectReverseFile(XMJGSubCompressDlg &cursel);
	void selectFiles(XMJGSubCompressDlg &cursel, const CString &type);
};

// CSetLayerInfoDlg 对话框

class CSetLayerInfoDlg : public CDialog
{

	DECLARE_DYNAMIC(CSetLayerInfoDlg)

public:

	CSetLayerInfoDlg(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CSetLayerInfoDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_XMJG_SETLAYERINFODLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	virtual void OnOK();

	virtual BOOL OnInitDialog();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月17号
	 */
	void setObject(const AcDbObjectId &);

	afx_msg void OnBnClickedCheckSliXiewumian();

	afx_msg void OnBnClickedCheckSliPingwumian();

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月31号
	 */
	void statisticArea();

	/* @接口 获取平屋面参数
	 * @作者 杨发荷
	 * @时间 2017年11月17号
	 */
	bool getPWMParament(MStr &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月21号
	 */
	void getCommonInfo(MStr &);

	/* @接口 获取斜屋面参数
	 * @作者 杨发荷
	 * @时间 2017年11月17号
	 */
	void getXWMParament(MStr &);

	/* @接口 设置平屋面参数
	 * @参数 int 是否设置（非零即一）
	 * @作者 杨发荷
	 * @时间 2017年11月17号
	 */
	void setPWMParament(int);

	/* @接口 设置平屋面参数
	* @参数 int 是否设置（非零即一）
	* @作者 杨发荷
	* @时间 2017年11月17号
	*/
	void setPWMParament(MStr &);

	/* @接口 设置斜屋面参数
	* @参数 int 是否设置（非零即一）
	* @作者 杨发荷
	* @时间 2017年11月17号
	*/
	void setXWMParament(int);

	/* @接口 设置斜屋面参数
	* @参数 int 是否设置（非零即一）
	* @作者 杨发荷
	* @时间 2017年11月17号
	*/
	void setXWMParament(MStr &);

	/* @接口 功能区中添加高度信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月30号
	 */
	void addHeightInfo(const CString &);

public:

	CEdit m_jzmj;
	CEdit m_jrmj;
	CEdit m_angle;
	CEdit m_layerbz;
	CEdit m_tjheight;
	CEdit m_wdheight;
	CEdit m_buildname;
	CEdit m_layername;
	CEdit m_sccontext;
	CEdit m_sjcontext;
	CButton m_xiewumian;
	CEdit m_cwallheight;
	CButton m_pingwumian;
	CEdit m_nvwallheight;
	CEdit m_sclayerheight;
	CEdit m_sjlayerheight;
	CComboBox m_tjlocation;

private:

	MStr m_filter;
	CString m_ldname;
	CString m_lcname;
	AcDbObjectId m_obj;

};

// CSetLayerInfoDlg 对话框

class CProjectInfoPrompt
	: public CAcUiDialog
{
	DECLARE_DYNAMIC(CProjectInfoPrompt)

public:

	CProjectInfoPrompt(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CProjectInfoPrompt();

	// 对话框数据
	enum { IDD = IDD_DIALOG_XMJG_PROJECTPROMPTDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();

public:

	CStatic m_promptcode;
	CStatic m_promptname;

};

class CProjectInfoDockbar
	: public CAdUiDockControlBar
{

	DECLARE_DYNAMIC(CProjectInfoDockbar)

public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年08月15日
	*/
	CProjectInfoDockbar();

public:

	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);

	virtual BOOL DestroyWindow();

	void setDlg(int IndexIDD);

protected:

	int m_IDD;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL AddCustomMenuItems(LPARAM hMenu);//去掉停靠工具栏 默认的右键菜单

	DECLARE_MESSAGE_MAP()

	virtual void SizeChanged(CRect *lpRect, BOOL bFloating, int flags);

public:

	CProjectInfoPrompt m_prompt;

};

// XMJGBridgeAnnotateDlg 对话框

class XMJGBridgeAnnotateDlg : public CDialog

{
	DECLARE_DYNAMIC(XMJGBridgeAnnotateDlg)

public:

	XMJGBridgeAnnotateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~XMJGBridgeAnnotateDlg();

	// 对话框数据
	enum { IDD = IDD_XMJG_BRIDGEANNOTATEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	afx_msg void OnBnClickedOk();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	afx_msg void OnBnClickedDelete();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	afx_msg void OnBnClickedButtonAdd();

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void OnAddNewRow(NMHDR *, LRESULT *);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	afx_msg void OnChangeSelectItem();

private:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	CString getListCtrlValues();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void setListCtrlValues(const CString &);

public:
	CComboBox m_comboBoxType;
	CEditListCtrl m_listCtrlValue;

private:

	VStr m_types;
	MStr m_value;

};

// COpenProjectDlg 对话框

class COpenProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(COpenProjectDlg)

public:
	COpenProjectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpenProjectDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_OPENPROJECTDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	VStr getOpenFiles();

	virtual BOOL OnInitDialog();

	CString getProjectPath();

private:

	virtual void OnOK();

	void initListCtrl();

	afx_msg void OnAllSelect();

	afx_msg void OnSelectProject();

	afx_msg void OnReverseSelect();

	void setListValue(const CString &);

	void setProjectDir(const CString &);

public:

	CEdit m_projectpath;
	CEditListCtrl m_projectdwg;

private:

	CString m_dir;
	VStr m_openFiles;
	CString m_projectPath;
	CStringArray m_fileNames;
	CStringArray m_pathNames;

};

// CPersonBridgeTechnicalDlg 对话框

class CPersonBridgeTechnicalDlg : public CDialog
{
	DECLARE_DYNAMIC(CPersonBridgeTechnicalDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月7号
	 */
	CPersonBridgeTechnicalDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月7号
	 */
	virtual ~CPersonBridgeTechnicalDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_TDOPOM };

protected:

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_pbpislicensedocs;
	CEdit m_pbexistinginfomation;
	CEdit m_pbcontrolbasepoint;
	CEdit m_pbtopographicsurvey;
	CEdit m_pboverpassmeasurment;
	CEdit m_pbyghxkbyzqk;
	CEdit m_pbqtxysmdqk;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};

// CSetHandlePipeLineDlg 对话框

class CSetHandlePipeLineDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSetHandlePipeLineDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	CSetHandlePipeLineDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	virtual ~CSetHandlePipeLineDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SETHANDLEPIPELINEDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEditListCtrl m_listpipeline;

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	void setListData();

	/* @接口 确定事件
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	virtual void OnOK();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	void initializeList();

	/* @接口 对话框初始化事件
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	virtual BOOL OnInitDialog();

	/* @接口 全选按钮事件
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	afx_msg void OnClickedButtonSelectAll();

	/* @接口 新增管线按钮事件
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	afx_msg void OnClickedButtonAddPipeline();

	/* @接口 反选按钮事件
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月8号
	 */
	afx_msg void OnClickedButtonReverseSelect();

};


// CPipeConfig 对话框

class CPipeConfig : public CDialog
{
	DECLARE_DYNAMIC(CPipeConfig)

public:

	/* @接口 默认构造函数
	 * @参数 bool 表示该对话框是创建项目，还是修改项目信息
	 * @参数 CWnd* 父类对话框的指针
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月2号
	 */
	CPipeConfig(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月2号
	 */
	virtual ~CPipeConfig();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PipiConfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_strdir;
	CEdit m_coorsystem;
	CEdit m_projectdir;
	CEdit m_projectname;
	CEdit m_projectcode;
	CEdit m_buildcompany;
	PipeLineTables m_fnts;
	CEdit m_projectaddress;
	CEdit m_meansurecompany;
	CEdit m_implementcompany;
	CEdit m_evelationsystem;

public:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	void Apply();

private:

	/*对话框初始化
	*/
	virtual BOOL OnInitDialog();

private:

	/*导入管线mdb
	*/
	void LoadMdb();

	/* @接口 创建文件夹
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月2号
	 */
	void createFolder(CString &strPath);

	/* @接口 设置项目信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月2号
	 */
	void setProjectInfo(CString &strPath);

	/* @接口 初始化项目信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月2号
	 */
	void initProjectInfo();

	/* @接口 初始化管线控件
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月2号
	 */
	void initPipeLineList();

public:
};

// CPipeZPTExport 对话框

class CPipeZPTExport : public CDialog
{
	DECLARE_DYNAMIC(CPipeZPTExport)

public:
	CPipeZPTExport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPipeZPTExport();

	// 对话框数据
	enum { IDD = IDD_DIALOG_EXPORTPIPEZPTS };
	CEditListCtrl m_pipelist;
	VMStr vmsLayers;

	/*获取需要输出的管线总平图
	*/
	void GetZPTLayers();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	virtual void OnOK();

	virtual BOOL OnInitDialog();

};

// CPipeLineJBXXDlg 对话框

class CPipeLineJBXXDlg : public CDialog
{
	DECLARE_DYNAMIC(CPipeLineJBXXDlg)

public:
	CPipeLineJBXXDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPipeLineJBXXDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINEJBXX };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	void Apply();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	void initProjectInfo();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	void initProjectJSSM();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	void saveProjectInfo();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	void saveProjectJSSM();

public:

	CEdit m_coorsystem;
	CEdit m_projectcode;
	CEdit m_projectname;
	CEdit m_buildcompany;
	CEdit m_projectaddress;
	CEdit m_implementcompany;
	CEdit m_evelationalsystem;
	CComboBox m_measurecompany;
	CButton m_dxgxdtc;
	CButton m_tqcl;

	CEdit m_cccgyjxkws;
	CEdit m_yyzlqk;
	CEdit m_tgkzqk;
	CEdit m_xbcljbqk;
	CEdit m_dxgxdtc_tqcl;
	CEdit m_yghxkbyzqk;
	CEdit m_qtxysmdqk;
	CDateTimeCtrl m_dataPick;

private:

	MIStr m_dxgxtctqcl;

};

// CPipeLineRoadInfo 对话框

class CPipeLineRoadInfo
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineRoadInfo)

public:
	CPipeLineRoadInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPipeLineRoadInfo();

	// 对话框数据
	enum { IDD = IDD_DIALOG_GHSSJBQKINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	void Apply();

	/* @接口 对话框初始化
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	afx_msg void OnSelectedShowRoadName();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	afx_msg void OnSelectedShowPipeName();

	/* @接口 对话框消息分发
	 * @参数 MSG *消息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	afx_msg void OnBnClickedButtonPipelineTypeAdd();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	afx_msg void OnBnClickedButtonPipelineTypeDel();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	afx_msg void OnBnClickedButtonRoadPingMianAdd();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	afx_msg void OnBnClickedButtonRoadPingMianDel();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	afx_msg void OnBnClickedButtonRoadZongDuanMianAdd();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	afx_msg void OnBnClickedButtonRoadZongDuanMianDel();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	afx_msg void OnBnClickedButtonRoadHengDuanMianAdd();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月29号
	 */
	afx_msg void OnBnClickedButtonRoadHengDuanMianDel();

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void initGHSSJBQK();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void initListCtrl();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void initComboxCtrl();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void setRoadListCtrlValue(const CString &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void setPipeListCtrlValue(const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void saveListCtrlValueRoadInfo(const CString &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void saveListCtrlValuePipeLine(const CString &);

public:

	CComboBox m_roadName;
	CComboBox m_szpipetype;
	CEditListCtrl m_szpipeinfo;
	CEditListCtrl m_roadkuandu;
	CEditListCtrl m_roadchangdu;
	CEditListCtrl m_roadduanmian;

private:

	typedef map<CString, GHSSJBQKPipe> MSGHSSJBQKPipe;
	typedef MSGHSSJBQKPipe::iterator MSGHSSJBQKPipeIter;
	typedef map<CString, GHSSJBQKInfo> MSRoadInfo;
	typedef MSRoadInfo::iterator MSRoadInfoIter;
	MSGHSSJBQKPipe m_msPipeLine;
	MSRoadInfo m_msRoadInfo;
	CString m_oldRoadName;
	CString m_oldPipeName;

};

// CPipeLineSelectDlg 对话框

class CPipeLineSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CPipeLineSelectDlg)

public:
	CPipeLineSelectDlg(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPipeLineSelectDlg();

	CString strColor;
	CString m_strdir;
	CEdit m_projectdir;
	CEdit m_projectname;
	CEdit m_projectcode;
	CEditListCtrl m_list;
	PipeLineTables m_fnts;

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINESELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	void Apply();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:

	/*导入管线mdb
	*/
	void LoadMdb();

	/*全选按钮
	*/
	afx_msg void OnClickedButtonSelectAll();

	/*全不选按钮
	*/
	afx_msg void OnClickedButtonSelectNone();

	/*反选按钮
	*/
	afx_msg void OnClickedButtonReverseSelect();

	/*选取颜色
	*/
	afx_msg void DBClickColor(NMHDR* pNMHDR, LRESULT* pResult);

};

// CPipeLineSelectOutputDlg 对话框

class CPipeLineSelectOutputDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineSelectOutputDlg)

public:
	CPipeLineSelectOutputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPipeLineSelectOutputDlg();

	VStr vmsLayers;
	VMStr vmRecords;
	CEditListCtrl m_pipelist;
	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINESELECTOUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	void Apply();

	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/*全选按钮
	*/
	afx_msg void OnClickedButtonSelectAll();

	/*反选按钮
	*/
	afx_msg void OnClickedButtonReverseSelect();

	/*全选按钮
	*/
	afx_msg void OnClickedButtonExport();

	/*反选按钮
	*/
	afx_msg void OnClickedButtonCancel();

	void SavePipeBlock(AcDbObjectIdArray &idArr);

	void GetZPTLayers();

	void PipefilterObj(VStr &vals, const CString &strPipeExport);

};

// CShiZhengPipeLine 对话框

class CShiZhengPipeLine : public CDialog
{
	DECLARE_DYNAMIC(CShiZhengPipeLine)

public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年5月30号
	*/
	CShiZhengPipeLine(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年5月30号
	*/
	virtual ~CShiZhengPipeLine();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SHIZHENGPIPELINE };

protected:

	/* @接口 对话框处初始化
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年5月30号
	*/
	virtual BOOL OnInitDialog();

	/* @接口
	* @参数
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年5月30号
	*/
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void initComboxCtrl();

public:

	CListCtrl m_pipeinfo;
	CComboBox m_pipetype;

};

// CPipeLineCreateDlg 对话框

class CPipeLineModifyProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineModifyProjectDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	CPipeLineModifyProjectDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	virtual ~CPipeLineModifyProjectDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINEMODIFYPROJECTINFO };

protected:

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

public:

	/* @接口 对话框初始化
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	virtual BOOL OnInitDialog();

	/* @接口 确定按钮事件
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	afx_msg void OnBnClickedOk();

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	void initTabContrl();

public:

	CTabCtrl m_tab;
	CPipeLineJBXXDlg m_jbxx;
	CPipeLineRoadInfo m_road;
	CPipeLineSelectDlg m_plp;

};

// CPipeLineOutputPartDlg 对话框

class CPipeLineOutputPartDlg
	: public CDialog
{

	DECLARE_DYNAMIC(CPipeLineOutputPartDlg)

public:
	CPipeLineOutputPartDlg(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPipeLineOutputPartDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINEOUTPUTPART };

	void Apply();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月16号
	 */
	VStr getExportPart();

	virtual BOOL OnInitDialog();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEditListCtrl m_list;

private:

	VStr m_export;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

// CPipeLineOutputDlg 对话框

class CPipeLineOutputDlg : public CDialog
{

	DECLARE_DYNAMIC(CPipeLineOutputDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	CPipeLineOutputDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	virtual ~CPipeLineOutputDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINEOUTPUT };

protected:

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

public:
	CTabCtrl m_tab;
	CPipeLineRoadInfo m_road;
	CPipeLineSelectOutputDlg m_sout;
};

// CPipeLineCreateProjectDlg 对话框

class CPipeLineCreateProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineCreateProjectDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	CPipeLineCreateProjectDlg(CWnd* pParent = NULL);

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	virtual ~CPipeLineCreateProjectDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINECREATEPROJECT };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CTabCtrl m_tab;
	virtual BOOL OnInitDialog();

private:

	CPipeConfig m_pconfig;
	CPipeLineSelectDlg m_pselect;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

// CSubPipeLinePropertyDlg 对话框

class CSubPipeLinePropertyDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSubPipeLinePropertyDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	CSubPipeLinePropertyDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual ~CSubPipeLinePropertyDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SUBPIPELINEPROPERTYDLG };

	void drawFinish();

	void addPoint(const AcGePoint2d &, const AcDbObjectId &, bool isNew = false, CString dh = _T(""));

	PipeLineTable getCurrentPipeLineType();

	afx_msg void OnClickedButtonPipelineReflashTable();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月28号
	 */
	void selectPoint(const AcDbObjectIdArray &);

	void reflashPLProperty();

protected:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnSelectShowPipeLine();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickedSelectRow(NMHDR *, LRESULT *);
	afx_msg void OnClickedButtonPipelineCorrelationEntity();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月8号
	 */
	void initListCtrl();

	void initComboxCtrl();

	void initComboxCtrlGXLX();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	void drawLineRemark();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void initListCtrlValue();

	void setListColumnReadOnly();

public:

	CComboBox m_gxlx;
	CComboBox m_combox;
	CEditListCtrl m_list;
	PipeLineTables m_plts;

private:

	int m_xloc;
	int m_yloc;
	int m_lastCount;

	map<CString, AcDbObjectId, CmpCstr> m_sid;

public:
	afx_msg void OnCbnSelchangeComboPipelinegxlx();
	CButton m_checkpl;
	CButton m_checkpp;
};

// CSubPipePointPropertyDlg 对话框

class CSubPipePointPropertyDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSubPipePointPropertyDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	CSubPipePointPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual ~CSubPipePointPropertyDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SUBPIPEPOINTPROPERTYDLG };

protected:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	afx_msg void OnSize(UINT nType, int cx, int cy);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	/* @接口 管点双击事件
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	afx_msg void OnClickedOnSelectRow(NMHDR* pNMHDR, LRESULT* pResult);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月20号
	 */
	afx_msg void OnSelectShowPipePoint();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void updatePipeLineProperty(const CString &, int &);

	DECLARE_MESSAGE_MAP()

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void initListCtrl();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月20号
	 */
	void initComboxCtrl();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void initListCtrlValue();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void middleEntity(const AcDbObjectId &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	void matchingPipePoint(int);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void matchingPipeLine(int);

public:

	CButton m_match;
	CButton m_matchpl;
	CComboBox m_combox;
	CEditListCtrl m_list;

private:

	PipeLineTables m_plts;
	AcDbObjectIdArray m_ids;
	map<CString, AcDbObjectIdArray> m_pids;		//以点号排序
	map<CString, AcDbObjectIdArray> m_lids;		//以图层排序
	map<CString, AcDbObjectId, CmpCstr> m_sid;

public:


	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月28号
	 */
	void selectPoint(const AcDbObjectIdArray &);

	void flushPipeLineProperty();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPipepointreassociate();
	afx_msg void OnBnClickedButtonPipepointflushaproperty();

	afx_msg void OnBnClickedCheckMatchPipePoint();
	afx_msg void OnBnClickedCheckMatchPipeLine();
};

// CPipeLinePropertyDlg 对话框

class CPipeLinePropertyDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLinePropertyDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	CPipeLinePropertyDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual ~CPipeLinePropertyDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINEPROPERTYDLG };

protected:

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual BOOL OnInitDialog();

public:

	CRect m_rect;
	CTabCtrl m_tab;
	CSubPipeLinePropertyDlg m_ldlg;
	CSubPipePointPropertyDlg m_pdlg;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

class CCommonDockControlBar
	: public CAdUiDockControlBar
{
	DECLARE_DYNAMIC(CCommonDockControlBar)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	CCommonDockControlBar();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual ~CCommonDockControlBar();

	/* @接口 设置子对话框
	 * @参数 CDialog *子对话框
	 * @参数 int 子对话框ID
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void SetChildDlg(CDialog* p_dlg, int IDD);

	/* @接口 创建对话框
	 * @参数 CWnd *
	 * @参数 TCHAR *标题
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual BOOL Create(CWnd*pParent, TCHAR *lpszTitle);

	/* @接口 销毁对话框
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	virtual BOOL DestroyWindow();

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void SizeChanged(CRect *lpRect, BOOL bFloating, int flags);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月4号
	 */
	afx_msg void OnHideBar();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月4号
	 */
	virtual bool OnClosing();

public:

	int m_IDD;
	CDialog* m_p_dlg;

};

// CModifyRoadPro 对话框
class CModifyRoadPro
	: public CAdUiDialog
{
	DECLARE_DYNAMIC(CModifyRoadPro)

public:
	CModifyRoadPro(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyRoadPro();

	/* @接口 获取图上道路中心线id
	 */
	BOOL GetRoadIds();

	/* @接口 初始化ComBox和ListControl控件内容
	 */
	void ComboxAndList();

	/* @接口 对话框初始化
	 */
	virtual BOOL OnInitDialog();

	/* @接口 ComBox下拉选项变化（选择不同道路）事件
	 */
	void OnSelRoad();

	void getRoadInfo(const AcDbObjectId &);

	void saveRoadInfo(const AcDbObjectId &);

	void readRoadInfo(const AcDbObjectId &);

	void OnBnClickedAdd();
	void OnBnClickedDel();
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancel();

	/* @接口 结束对项文本的编辑刷新ListControl事件，此处用于刷新【高差】值
	 */
	afx_msg void OnLvnItemchangingList1(NMHDR *pNMHDR, LRESULT *pResult);

	// 对话框数据
	enum { IDD = IDD_DIALOG_MODIFYROADPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	void setListItemValue(const VStr &, int);

private:

	MSId m_sid;
	CString m_oldName;

public:
	CComboBox m_road;
	CEditListCtrl m_list;
	CButton m_add;
	CButton m_del;
	AcDbObjectIdArray m_ids;
	CAcUiPickButton m_pickpoint;
	afx_msg void OnBnClickedButtonPickPoint();
};

typedef struct pip
{
	CString strNum;
	AcDbObjectId id;
}pip;

// CFilterLayerDlg 对话框

class CFilterLayerDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CFilterLayerDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月17号
	 */
	CFilterLayerDlg(const VStr &layers, CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月17号
	 */
	virtual ~CFilterLayerDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_FILTERLAYERDLG };

	virtual BOOL OnInitDialog();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月16号
	 */
	void setLayers(const VStr &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月16号
	 */
	VStr getLayers();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月12号
	 */
	double getScale();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CEdit m_scale;
	CEditListCtrl m_list;

private:

	double m_bl;
	VStr m_layers;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonImportall();
	afx_msg void OnBnClickedButtonPipelinReverse();
	afx_msg void OnBnClickedButtonPipelineNotSelect();
};

// CPipeConfig 对话框

class CSZConfig : public CDialog
{
	DECLARE_DYNAMIC(CSZConfig)

public:

	/* @接口 默认构造函数
	* @参数 bool 表示该对话框是创建项目，还是修改项目信息
	* @参数 CWnd* 父类对话框的指针
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月2号
	*/
	CSZConfig(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月2号
	*/
	virtual ~CSZConfig();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SHIZHENG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_strdir;
	//CEdit m_coorsystem;
	CEdit m_projectdir;
	CEdit m_projectname;
	CEdit m_projectcode;
	CEdit m_buildcompany;
	PipeLineTables m_fnts;
	CEdit m_projectaddress;
	CEdit m_implementcompany;
	CDateTimeCtrl m_dataPick;
	CComboBox m_meansurecompany;
	//CEdit m_evelationsystem;

public:

	/* @接口
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月16号
	*/
	void Apply();

private:

	/*对话框初始化
	*/
	virtual BOOL OnInitDialog();

private:

	/* @接口 设置项目信息
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月2号
	*/
	void setProjectInfo(CString &strPath);

	/* @接口 初始化项目信息
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月2号
	*/
	void initProjectInfo();

};

// CSZProjectDlg 对话框

class CSZProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSZProjectDlg)

public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月16号
	*/
	CSZProjectDlg(CWnd* pParent = NULL);

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月16号
	*/
	virtual ~CSZProjectDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PIPELINECREATEPROJECT1 };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	/* @接口
	* @参数
	* @参数
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月15号
	*/
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CTabCtrl m_tab;
	virtual BOOL OnInitDialog();

private:

	CSZConfig m_pconfig;
	CPipeLineSelectDlg m_pselect;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};


// TQZBINFO 对话框

class CTQZBINFO : public CDialog
{
	DECLARE_DYNAMIC(CTQZBINFO)

public:

	CTQZBINFO(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CTQZBINFO();

	/* @接口 对话框初始化
	*/
	virtual BOOL OnInitDialog();

	/* @接口 项目名称添加与删除
	 * @时间 2018年4月18日
	 */
	void OnBnClickedAdd();

	void OnBnClickedDel();

	/* @接口 桥梁净高添加与删除
	 * @时间 2018年4月18日
	 */
	void OnBnClickedAdd2();

	void OnBnClickedDel2();

	/* @接口 获取List2数据，用于保存与读取
	 * @时间 2018年4月18日
	 */
	VStr GetList2Data();

	/* @接口 单击不同项目，刷新List2、桥梁宽、备注
	 * @时间 2018年4月19日
	 */
	afx_msg void OnClickList1Main(NMHDR *, LRESULT *);

	/* @接口 保存桥梁宽
	 * @时间 2018年4月19日
	 */
	afx_msg void OnLvnEndlabeleditList1(NMHDR *, LRESULT *);

	/* @接口 保存备注
	 * @时间 2018年4月19日
	 */
	afx_msg void OnLvnEndlabeleditList2(NMHDR *, LRESULT *);

	afx_msg void OnBnClickedOK();

	afx_msg void OnBnClickedCancel();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年9月12号
	 */
	void setDialogInfo(const VMStr &);


	// 对话框数据
	enum { IDD = IDD_DIALOG_TQZBINFO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	MVStr mData;
	CButton m_add;
	CButton m_del;
	CButton m_add2;
	CButton m_del2;
	CString m_strZYZB;
	MStr m_Pro_Remarks;
	CEditListCtrl m_list;
	CEditListCtrl m_list2;
	MStr m_Pro_BridgeWidth;

private:

	CString m_strFormer;

public:
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
};

// CRoadHDMTLDlg 对话框

class CRoadHDMTLDlg
	: public CAdUiDialog
{
	DECLARE_DYNAMIC(CRoadHDMTLDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月11号
	 */
	CRoadHDMTLDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月11号
	 */
	virtual ~CRoadHDMTLDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_ROADHDMDZTTL };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月11号
	 */
	MIdStr getIdBlock();

	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedButtonSelectEnt();

	afx_msg void OnBnClickedButtonDeleteItem();

public:

	CEditListCtrl m_list;
	CAcUiPickButton m_selEnt;

private:

	MStr m_tuli;
	CString m_name;
	MIdStr m_idBlock;
	AcDbObjectIdArray m_ids;
};

// CLayerManagerDlg 对话框

class CLayerManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayerManagerDlg)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月14号
	 */
	CLayerManagerDlg(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月14号
	 */
	virtual ~CLayerManagerDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_LAYERMANAMGER };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

};

// CModifyProjectInfo 对话框

struct projectInfo
{
	CString m_name;
	CString m_code;
};

struct ProjectInfo
{
	bool isModify;
	CString m_pPath;
	projectInfo m_src;
	projectInfo m_des;
};

typedef vector<ProjectInfo> ProjectInfos;

class CModifyProjectInfo 
	: public CDialog
{

	DECLARE_DYNAMIC(CModifyProjectInfo)

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月16号
	 */
	CModifyProjectInfo(CWnd* pParent = NULL);   // 标准构造函数

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月16号
	 */
	virtual ~CModifyProjectInfo();

	// 对话框数据
	enum { IDD = IDD_DIALOGMODIFYPROJECTINFO };

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月16号
	 */
	void setProjectInfo(const ProjectInfos &);

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月24号
	 */
	ProjectInfos getProjectInfos();

protected:

	/* @接口 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月16号
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月16号
	 */
	virtual BOOL OnInitDialog();

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月16号
	 */
	void OnLvnClickListItem(NMHDR *pNMHDR, LRESULT *pResult);

private:

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月24号
	 */
	void saveChangeInfo(int);

public:

	int m_selRow;
	CEdit m_pname;
	CEdit m_pcode;
	CEdit m_dname;
	CEdit m_dcode;
	CEditListCtrl m_names;

private:

	ProjectInfos m_infos;

public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedRepost();
	virtual void OnOK();
};

#endif /*XMJGEXPORTDLG*/
#include "afxwin.h"
