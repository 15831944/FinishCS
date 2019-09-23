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

// XMJGExportDlg �Ի���

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

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��22��
	*/
	virtual ~IWindowControl()
	{
	}

	/* @�ӿ� ��ȡ���ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void getWindowControl(MStr &) = 0;

	/* @�ӿ� ���ô��ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void setWindowControl(MStr &) = 0;

};

#pragma region ҵ��һ��Ϣ���öԻ���

class XMJGSetInfo1
	: public CDialog

{
	DECLARE_DYNAMIC(XMJGSetInfo1)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	XMJGSetInfo1(bool, CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��17��
	*/
	virtual ~XMJGSetInfo1();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGSETINFO1
	};

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��23��
	 */
	int getEntityColor();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
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

#pragma region ҵ�����Ϣ���öԻ���

class XMJGSetInfo2
	: public CAcUiDialog
{
	DECLARE_DYNAMIC(XMJGSetInfo2)

private:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��16��
	 */
	XMJGSetInfo2(bool, CWnd* pParent = NULL, HINSTANCE hInstance = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��16��
	*/
	virtual ~XMJGSetInfo2();

public:

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGSETINFO2
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��16��
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

#pragma region ҵ������Ϣ���öԻ���

class XMJGSetInfo3
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGSetInfo3)

public:
	XMJGSetInfo3(bool, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGSetInfo3();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGSETINFO3
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

#pragma region ҵ������Ϣ���öԻ���

class XMJGSetInfo4
	: public CDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJGSetInfo4)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	XMJGSetInfo4(bool, CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��17��
	*/
	virtual ~XMJGSetInfo4();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGSETINFO4
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	CEdit m_AcceptCode;
	CEdit m_projectdir;
	CEdit m_ProjectName;
	CEdit m_EntrustCompany;

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	afx_msg void OnBnClickedOk();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ� ��ȡ���ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void getWindowControl(MStr &);

	/* @�ӿ� ���ô��ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void setWindowControl(MStr &);

private:

	bool m_isCreate;

public:
};

#pragma endregion

#pragma region ҵ������Ϣ���öԻ���

class XMJGSetInfo5
	: public CAcUiDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJGSetInfo5)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	XMJGSetInfo5(bool, CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��17��
	*/
	~XMJGSetInfo5();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGSETINFO5
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
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

	/* @�ӿ� ��ȡ���ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void getWindowControl(MStr &);

	/* @�ӿ� ���ô��ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
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

#pragma region ҵ����������������

class XMJGSetInfo6
	: public CAcUiDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJGSetInfo6)

public:

	XMJGSetInfo6(bool, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGSetInfo6();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGSETINFO6
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	afx_msg void OnBnClickedOk();

	/* @�ӿ� ��ȡ���ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void getWindowControl(MStr &);

	/* @�ӿ� ���ô��ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
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

#pragma region ҵ������Ϣ���öԻ���

class XMJGSetInfo7
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGSetInfo7)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��18��
	 */
	XMJGSetInfo7(bool, CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��18��
	*/
	virtual ~XMJGSetInfo7();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGSETINFO7
	};

protected:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
#ifdef _WIN64
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
#else
	afx_msg LONG onAcadKeepFocus(UINT n, LONG l);
#endif

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

#pragma region ҵ��һ�ɹ�����Ի���

#pragma endregion

#pragma region ҵ����ɹ�����Ի���

class XMJG2ExportDlg
	: public CAcUiDialog
	, public IWindowControl
{
	DECLARE_DYNAMIC(XMJG2ExportDlg)

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��21��
	 */
	XMJG2ExportDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��23��
	 */
	XMJG2ExportDlg(const CString &);

public:

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��21��
	*/
	virtual ~XMJG2ExportDlg();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJEXPORTINFO2
	};

protected:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
#ifdef _WIN64
	afx_msg LRESULT onAcadKeepFocus(WPARAM n, LPARAM l);
#else
	afx_msg LONG onAcadKeepFocus(UINT n, LONG l);
#endif

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	void setSavePath(const CString &);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

	/* @�ӿ� ���ö�ȡע����λ��
	* @���� bool Ĭ��Ϊtrue����ʾ��ȡҵ�����ע�����Ϣ��
	���Ϊfalse����ʾ��ȡҵ�����ע�����Ϣ��
	* @���� ���
	* @ʱ�� 2017��06��20��
	*/
	void SetRegPath(bool flag = true);

	/* @�ӿ� ��ȡ���ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void getWindowControl(MStr &);

	/* @�ӿ� ���ô��ڿؼ�������
	 * @���� ���ڿؼ���ֵ����
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	virtual void setWindowControl(MStr &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��21��
	 */
	static XMJG2ExportDlg *Instance(const CString &);

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��23��
	 */
	static void destory();

	afx_msg void OnBnClickedDistance();

	afx_msg void OnBnClickedBetween();

	/* @�ӿ� 0:�����н��蹤�̹滮���������������.docx	1:�����н��蹤��0-0���ߣ�������)�������.docx
	 * @���� ���
	 * @ʱ�� 2017��08��28��
	 */
	void exportWord(MStr &, int nflag = 0);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��28��
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

#pragma region ҵ�����ɹ�����Ի���

#pragma endregion

#pragma region ҵ���ĳɹ�����Ի���

#pragma endregion

#pragma region ҵ����ɹ�����Ի���

#pragma endregion

class XMJGControlPointDlg
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGControlPointDlg)

public:
	XMJGControlPointDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGControlPointDlg();

	// �Ի�������
	enum
	{
		IDD = IDD_CONTROLPOINTATTRIBUTE
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// XMJGPlanDlg ���÷ֲ�Ի���

class XMJGPlanDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGPlanDlg)

public:

	XMJGPlanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGPlanDlg();

	// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	VStr m_LayerInfo;
	AcDbObjectId m_ObjectID;

public:
	CEdit m_FloorNum1;
	CEdit m_FloorNum2;
	CEditListCtrl m_layerinfo;
};

// XMJGSetStratPlanInfo �Ի���

class XMJGSetStratPlanInfo
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGSetStratPlanInfo)

public:
	XMJGSetStratPlanInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGSetStratPlanInfo();

	// �Ի�������
	enum
	{
		IDD = IDD_DIALOG_SETSTRATIFINCATIONPLANINFO
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// XMJGFunctionsAreasInfo �Ի���

class XMJGFunctionsAreasInfo
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGFunctionsAreasInfo)

public:

	XMJGFunctionsAreasInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGFunctionsAreasInfo();

	// �Ի�������
	enum
	{
		IDD = IDD_DIALOG_FUNCTIONALAREAS
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	CComboBox m_tiname;
	CEdit m_FunctionsCode;				//�������ı��
	CEdit m_FunctionsHeight;			//�������ĸ߶�
	CEdit m_AreaCoefficient;			//���ϵ��
	CEdit m_FunctionsFactor;			//����ϵ��

public:

	/* @�ӿ� ��������Ϣ���öԻ����ʼ��
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ� ���ok�¼�
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	afx_msg void OnBnClickedOk();

	/* @�ӿ� ���ȡ����ť
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	afx_msg void OnBnClickedCancel();

	/* @�ӿ� ��������������
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	afx_msg void OnClickedXmjgadd();

	/* @�ӿ� ɾ�����еĹ���������
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	afx_msg void OnClickedXmjgdel();

	FunctionNameTableEx getFNTInfo();

	afx_msg void OnFunctionChangeName();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��18��
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

// XMJGRedLineQuality �Ի���

class XMJGRedLineQuality : public CDialog
{
	DECLARE_DYNAMIC(XMJGRedLineQuality)

public:
	XMJGRedLineQuality(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGRedLineQuality();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJGREDLINEQUALITY
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	BOOL m_bAskJiange; //�Ƿ���ʾ����ͼ���
	BOOL m_bAskHpmt;  //�Ƿ���ʾ��ƽ��ͼ
	BOOL m_bAskBegin; //�Ƿ���ʾ��ʼ���
	AcGePoint2dArray m_Nodes; //�����������
	AcGePoint2dArray m_NextNodes; //���������
	int m_oldPick;
	ads_name m_Ent; //������
	double m_dLicheng; //��������

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

// CSetProjectCodeDlg �Ի���

class XMJGProjectCodeDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGProjectCodeDlg)

public:

	XMJGProjectCodeDlg(CWnd* pParent = NULL);   // ��׼���캯��

	XMJGProjectCodeDlg(const TCHAR *, bool);

	virtual ~XMJGProjectCodeDlg();

	// �Ի�������
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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// XMJGLicenseDocDlg �Ի���

class XMJGLicenseDocDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGLicenseDocDlg)

	typedef vector<CString> VStr;
	typedef map<int, CString> MIStr;
	typedef map<CString, CString> MStr;

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	XMJGLicenseDocDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	virtual ~XMJGLicenseDocDlg();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJG_LINCESEDOCDLG
	};

public:

	void getLicenseDoc(MStr &);

private:

	/* @�ӿ� ��ȡMDB
	 * @���� VMStr ��ȡ������
	 * @���� bool ��ȡMDB�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��06��28��
	 */
	bool ReadMDB();

	/* @�ӿ� ��ʼ������ؼ�
	 * @���� VMStr ��ʼ���ؼ�����
	 * @���� bool ��ʼ���ؼ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��06��28��
	 */
	bool InitListCtrl();

public:

	/* @�ӿ� ��ʼ���Ի���
	 * @���� ���
	 * @ʱ�� 2017��06��28��
	 */
	virtual BOOL OnInitDialog();

	virtual void OnOK();

	void OnLvnClickListItem(NMHDR *pNMHDR, LRESULT *pResult);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	CEditListCtrl m_LicenseDoc;

private:

	MStr m_Ret;
	VStr m_data;
	MIStr m_Data;

};

// XMJGChangePictureProperty �Ի���

#define WM_SET_ITEMS	WM_USER + 0x7FFC

class XMJGChangePictureProperty
	: public CDialog
{
	DECLARE_DYNAMIC(XMJGChangePictureProperty)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	XMJGChangePictureProperty(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	XMJGChangePictureProperty(const TCHAR *);

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	virtual ~XMJGChangePictureProperty();

	// �Ի�������
	enum
	{
		IDD = IDD_XMJG_CHANGEPICTUREPROPERTY
	};

	/* @�ӿ� ����ͼƬ���Ե�����
	 * @���� CString ����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��1��
	 */
	void setPicturePropertyType(const CString &);

protected:

	/* @�ӿ� ��ͼƬ����
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	afx_msg void OnBnClickedXMJGMMoveUp();

	/* @�ӿ� ��ͼƬ�ö�
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	afx_msg void OnBnClickedXMJGMoveTop();

	/* @�ӿ� ��ͼƬ����
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	afx_msg void OnBnClickedMJGMoveDown();

	/* @�ӿ� ��ͼƬ�õ�
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	afx_msg void OnBnClickedMJGMoveButtom();

	/* @function ���ͼƬ�¼�
	* @author ���
	* @time 2016��10��23��
	*/
	afx_msg void OnClickedBrowIngPicture(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedXmjgAddPixture();

	afx_msg void OnBnClickedXMJGDelPicture();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	/* @�ӿ� ���ڳ�ʼ��
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ� ��ʼ��ListCtrl�ؼ�
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	void InitListCtrl();

	DECLARE_MESSAGE_MAP()

private:

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��29��
	 */
	void swapPicturePosition(int, int);

	/* @�ӿ� ��ȡ�����ļ�
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	void readConfigFile();

	/* @�ӿ� ��ȡͼƬ��Ϣ
	 * @���� ���
	 * @ʱ�� 2017��09��18��
	 */
	void readPictureInfo();

	void writeConfigFile();

	/* @�ӿ� дͼƬ��Ϣ
	 * @���� ���
	 * @ʱ�� 2017��09��18��
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
	XMJGFunctionDefine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGFunctionDefine();

	// �Ի�������
	enum
	{
		IDD = IDD_DIALOG_XMJGFUNCTIONDEFINE
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��09��26��
	 */
	XMJGChangeLayerInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @ʱ�� 2017��09��26��
	 */
	virtual ~XMJGChangeLayerInfoDlg();

	// �Ի�������
	enum
	{
		IDD = IDD_DIALOG_XMJG_CHANGELAYERINFO
	};

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	virtual void OnOK();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	afx_msg void OnChangeBuildName();

private:

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool initDialog();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool initComboBox();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool initListCtrl();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	bool setLayerInfo(const CString &);

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
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

// XMJGSubCompressDlg �Ի���

class XMJGSubCompressDlg : public CDialog
{
	DECLARE_DYNAMIC(XMJGSubCompressDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��09��26��
	 */
	XMJGSubCompressDlg(const TCHAR *);   // ��׼���캯��

	/* @�ӿ� Ĭ�Ϲ���ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��30��
	 */
	XMJGSubCompressDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ�������ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��30��
	 */
	virtual ~XMJGSubCompressDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SUBCOMPRESSDLG };

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	virtual void OnOK();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��30��
	 */
	void setWindowName(const CString &, const CString &, const CString &, const CString &);

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��10��9��
	 */
	CString getCompressFiles();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��31��
	 */
	void addCompressFile(const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��31��
	 */
	bool checkFileIsComplete();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��2��
	 */
	void deleteFiles();

	void judgeFileOccupy();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��10��9��
	 */
	void initListCtrl();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��30��
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

// CChangeCarPro �Ի���

class CChangeCarPro : public CDialog
{
	DECLARE_DYNAMIC(CChangeCarPro)

public:

	CChangeCarPro(CWnd* pParent = NULL);   // ��׼���캯��

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

	// �Ի�������
	enum { IDD = IDD_DIALOG_CHANGECARPRO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CProjectInfoSettingDlg �Ի���

class CProjectBaseInfoSettingDlg
	: public CDialog
{

	DECLARE_DYNAMIC(CProjectBaseInfoSettingDlg)

public:

	/* @�ӿ� Ĭ�Ϲ���ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	CProjectBaseInfoSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ�������ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	virtual ~CProjectBaseInfoSettingDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_XMJG_PROJECTBASEINFOSETTINGDLG };

	virtual void OnOK();

protected:

	DECLARE_MESSAGE_MAP()

	virtual void OnCancel();

	/* @�ӿ� ��ʼ���Ի���
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	CDeficiencyFilesDlg(const CString &, CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CDeficiencyFilesDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_DEFICIENCYFILESDLG };

protected:

	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CBaseProjectInfoDlg �Ի���

class CProjectExportDocSettingDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CProjectExportDocSettingDlg)

public:

	virtual void OnOK();

	virtual void OnCancel();

	virtual BOOL OnInitDialog();

	CProjectExportDocSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CProjectExportDocSettingDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_XMJG_PROJECTEXPORTDOCSETTINGDLG };

protected:

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CProjectAreaSettingDlg �Ի���

class CProjectAreaSettingDlg
	: public CDialog
{

	DECLARE_DYNAMIC(CProjectAreaSettingDlg)

public:

	virtual void OnOK();

	virtual void OnCancel();

	virtual BOOL OnInitDialog();

	CProjectAreaSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CProjectAreaSettingDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_XMJG_PROJECTAREASETTINGDLG };

protected:

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CExportDocInfoDlg �Ի���

class CExportDocInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportDocInfoDlg)

public:
	CExportDocInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExportDocInfoDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_XMJG_EXPORTINFODLG };

protected:

	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	void OnTcnSelchangeTab(NMHDR *, LRESULT *);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	CCompressDlg(CWnd* pParent = NULL);   // ��׼���캯��

	CCompressDlg(const CString &);

	virtual ~CCompressDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_XMJGCOMPRESSDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CSetLayerInfoDlg �Ի���

class CSetLayerInfoDlg : public CDialog
{

	DECLARE_DYNAMIC(CSetLayerInfoDlg)

public:

	CSetLayerInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CSetLayerInfoDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_XMJG_SETLAYERINFODLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	virtual void OnOK();

	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��17��
	 */
	void setObject(const AcDbObjectId &);

	afx_msg void OnBnClickedCheckSliXiewumian();

	afx_msg void OnBnClickedCheckSliPingwumian();

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��31��
	 */
	void statisticArea();

	/* @�ӿ� ��ȡƽ�������
	 * @���� ���
	 * @ʱ�� 2017��11��17��
	 */
	bool getPWMParament(MStr &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��21��
	 */
	void getCommonInfo(MStr &);

	/* @�ӿ� ��ȡб�������
	 * @���� ���
	 * @ʱ�� 2017��11��17��
	 */
	void getXWMParament(MStr &);

	/* @�ӿ� ����ƽ�������
	 * @���� int �Ƿ����ã����㼴һ��
	 * @���� ���
	 * @ʱ�� 2017��11��17��
	 */
	void setPWMParament(int);

	/* @�ӿ� ����ƽ�������
	* @���� int �Ƿ����ã����㼴һ��
	* @���� ���
	* @ʱ�� 2017��11��17��
	*/
	void setPWMParament(MStr &);

	/* @�ӿ� ����б�������
	* @���� int �Ƿ����ã����㼴һ��
	* @���� ���
	* @ʱ�� 2017��11��17��
	*/
	void setXWMParament(int);

	/* @�ӿ� ����б�������
	* @���� int �Ƿ����ã����㼴һ��
	* @���� ���
	* @ʱ�� 2017��11��17��
	*/
	void setXWMParament(MStr &);

	/* @�ӿ� ����������Ӹ߶���Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��30��
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

// CSetLayerInfoDlg �Ի���

class CProjectInfoPrompt
	: public CAcUiDialog
{
	DECLARE_DYNAMIC(CProjectInfoPrompt)

public:

	CProjectInfoPrompt(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CProjectInfoPrompt();

	// �Ի�������
	enum { IDD = IDD_DIALOG_XMJG_PROJECTPROMPTDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��08��15��
	*/
	CProjectInfoDockbar();

public:

	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);

	virtual BOOL DestroyWindow();

	void setDlg(int IndexIDD);

protected:

	int m_IDD;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual BOOL AddCustomMenuItems(LPARAM hMenu);//ȥ��ͣ�������� Ĭ�ϵ��Ҽ��˵�

	DECLARE_MESSAGE_MAP()

	virtual void SizeChanged(CRect *lpRect, BOOL bFloating, int flags);

public:

	CProjectInfoPrompt m_prompt;

};

// XMJGBridgeAnnotateDlg �Ի���

class XMJGBridgeAnnotateDlg : public CDialog

{
	DECLARE_DYNAMIC(XMJGBridgeAnnotateDlg)

public:

	XMJGBridgeAnnotateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~XMJGBridgeAnnotateDlg();

	// �Ի�������
	enum { IDD = IDD_XMJG_BRIDGEANNOTATEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	afx_msg void OnBnClickedOk();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	afx_msg void OnBnClickedDelete();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	afx_msg void OnBnClickedButtonAdd();

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void OnAddNewRow(NMHDR *, LRESULT *);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	afx_msg void OnChangeSelectItem();

private:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	CString getListCtrlValues();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void setListCtrlValues(const CString &);

public:
	CComboBox m_comboBoxType;
	CEditListCtrl m_listCtrlValue;

private:

	VStr m_types;
	MStr m_value;

};

// COpenProjectDlg �Ի���

class COpenProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(COpenProjectDlg)

public:
	COpenProjectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COpenProjectDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_OPENPROJECTDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CPersonBridgeTechnicalDlg �Ի���

class CPersonBridgeTechnicalDlg : public CDialog
{
	DECLARE_DYNAMIC(CPersonBridgeTechnicalDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��7��
	 */
	CPersonBridgeTechnicalDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��7��
	 */
	virtual ~CPersonBridgeTechnicalDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_TDOPOM };

protected:

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CSetHandlePipeLineDlg �Ի���

class CSetHandlePipeLineDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSetHandlePipeLineDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	CSetHandlePipeLineDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	virtual ~CSetHandlePipeLineDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SETHANDLEPIPELINEDLG };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	CEditListCtrl m_listpipeline;

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	void setListData();

	/* @�ӿ� ȷ���¼�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	virtual void OnOK();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	void initializeList();

	/* @�ӿ� �Ի����ʼ���¼�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ� ȫѡ��ť�¼�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	afx_msg void OnClickedButtonSelectAll();

	/* @�ӿ� �������߰�ť�¼�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	afx_msg void OnClickedButtonAddPipeline();

	/* @�ӿ� ��ѡ��ť�¼�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��8��
	 */
	afx_msg void OnClickedButtonReverseSelect();

};


// CPipeConfig �Ի���

class CPipeConfig : public CDialog
{
	DECLARE_DYNAMIC(CPipeConfig)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� bool ��ʾ�öԻ����Ǵ�����Ŀ�������޸���Ŀ��Ϣ
	 * @���� CWnd* ����Ի����ָ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��2��
	 */
	CPipeConfig(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��2��
	 */
	virtual ~CPipeConfig();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PipiConfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
	 */
	void Apply();

private:

	/*�Ի����ʼ��
	*/
	virtual BOOL OnInitDialog();

private:

	/*�������mdb
	*/
	void LoadMdb();

	/* @�ӿ� �����ļ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��2��
	 */
	void createFolder(CString &strPath);

	/* @�ӿ� ������Ŀ��Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��2��
	 */
	void setProjectInfo(CString &strPath);

	/* @�ӿ� ��ʼ����Ŀ��Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��2��
	 */
	void initProjectInfo();

	/* @�ӿ� ��ʼ�����߿ؼ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��2��
	 */
	void initPipeLineList();

public:
};

// CPipeZPTExport �Ի���

class CPipeZPTExport : public CDialog
{
	DECLARE_DYNAMIC(CPipeZPTExport)

public:
	CPipeZPTExport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPipeZPTExport();

	// �Ի�������
	enum { IDD = IDD_DIALOG_EXPORTPIPEZPTS };
	CEditListCtrl m_pipelist;
	VMStr vmsLayers;

	/*��ȡ��Ҫ����Ĺ�����ƽͼ
	*/
	void GetZPTLayers();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	virtual void OnOK();

	virtual BOOL OnInitDialog();

};

// CPipeLineJBXXDlg �Ի���

class CPipeLineJBXXDlg : public CDialog
{
	DECLARE_DYNAMIC(CPipeLineJBXXDlg)

public:
	CPipeLineJBXXDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPipeLineJBXXDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINEJBXX };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	void Apply();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
	 */
	void initProjectInfo();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
	 */
	void initProjectJSSM();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
	 */
	void saveProjectInfo();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
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

// CPipeLineRoadInfo �Ի���

class CPipeLineRoadInfo
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineRoadInfo)

public:
	CPipeLineRoadInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPipeLineRoadInfo();

	// �Ի�������
	enum { IDD = IDD_DIALOG_GHSSJBQKINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	void Apply();

	/* @�ӿ� �Ի����ʼ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	afx_msg void OnSelectedShowRoadName();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	afx_msg void OnSelectedShowPipeName();

	/* @�ӿ� �Ի�����Ϣ�ַ�
	 * @���� MSG *��Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	afx_msg void OnBnClickedButtonPipelineTypeAdd();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	afx_msg void OnBnClickedButtonPipelineTypeDel();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	afx_msg void OnBnClickedButtonRoadPingMianAdd();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	afx_msg void OnBnClickedButtonRoadPingMianDel();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	afx_msg void OnBnClickedButtonRoadZongDuanMianAdd();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	afx_msg void OnBnClickedButtonRoadZongDuanMianDel();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	afx_msg void OnBnClickedButtonRoadHengDuanMianAdd();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��29��
	 */
	afx_msg void OnBnClickedButtonRoadHengDuanMianDel();

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void initGHSSJBQK();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void initListCtrl();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void initComboxCtrl();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void setRoadListCtrlValue(const CString &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void setPipeListCtrlValue(const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void saveListCtrlValueRoadInfo(const CString &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
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

// CPipeLineSelectDlg �Ի���

class CPipeLineSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CPipeLineSelectDlg)

public:
	CPipeLineSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CPipeLineSelectDlg();

	CString strColor;
	CString m_strdir;
	CEdit m_projectdir;
	CEdit m_projectname;
	CEdit m_projectcode;
	CEditListCtrl m_list;
	PipeLineTables m_fnts;

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINESELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	void Apply();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:

	/*�������mdb
	*/
	void LoadMdb();

	/*ȫѡ��ť
	*/
	afx_msg void OnClickedButtonSelectAll();

	/*ȫ��ѡ��ť
	*/
	afx_msg void OnClickedButtonSelectNone();

	/*��ѡ��ť
	*/
	afx_msg void OnClickedButtonReverseSelect();

	/*ѡȡ��ɫ
	*/
	afx_msg void DBClickColor(NMHDR* pNMHDR, LRESULT* pResult);

};

// CPipeLineSelectOutputDlg �Ի���

class CPipeLineSelectOutputDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineSelectOutputDlg)

public:
	CPipeLineSelectOutputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPipeLineSelectOutputDlg();

	VStr vmsLayers;
	VMStr vmRecords;
	CEditListCtrl m_pipelist;
	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINESELECTOUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	void Apply();

	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/*ȫѡ��ť
	*/
	afx_msg void OnClickedButtonSelectAll();

	/*��ѡ��ť
	*/
	afx_msg void OnClickedButtonReverseSelect();

	/*ȫѡ��ť
	*/
	afx_msg void OnClickedButtonExport();

	/*��ѡ��ť
	*/
	afx_msg void OnClickedButtonCancel();

	void SavePipeBlock(AcDbObjectIdArray &idArr);

	void GetZPTLayers();

	void PipefilterObj(VStr &vals, const CString &strPipeExport);

};

// CShiZhengPipeLine �Ի���

class CShiZhengPipeLine : public CDialog
{
	DECLARE_DYNAMIC(CShiZhengPipeLine)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��5��30��
	*/
	CShiZhengPipeLine(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��5��30��
	*/
	virtual ~CShiZhengPipeLine();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SHIZHENGPIPELINE };

protected:

	/* @�ӿ� �Ի��򴦳�ʼ��
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��5��30��
	*/
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	* @����
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��5��30��
	*/
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void initComboxCtrl();

public:

	CListCtrl m_pipeinfo;
	CComboBox m_pipetype;

};

// CPipeLineCreateDlg �Ի���

class CPipeLineModifyProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineModifyProjectDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	CPipeLineModifyProjectDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	virtual ~CPipeLineModifyProjectDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINEMODIFYPROJECTINFO };

protected:

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

public:

	/* @�ӿ� �Ի����ʼ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ� ȷ����ť�¼�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	afx_msg void OnBnClickedOk();

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	void initTabContrl();

public:

	CTabCtrl m_tab;
	CPipeLineJBXXDlg m_jbxx;
	CPipeLineRoadInfo m_road;
	CPipeLineSelectDlg m_plp;

};

// CPipeLineOutputPartDlg �Ի���

class CPipeLineOutputPartDlg
	: public CDialog
{

	DECLARE_DYNAMIC(CPipeLineOutputPartDlg)

public:
	CPipeLineOutputPartDlg(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CPipeLineOutputPartDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINEOUTPUTPART };

	void Apply();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��16��
	 */
	VStr getExportPart();

	virtual BOOL OnInitDialog();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	CEditListCtrl m_list;

private:

	VStr m_export;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

// CPipeLineOutputDlg �Ի���

class CPipeLineOutputDlg : public CDialog
{

	DECLARE_DYNAMIC(CPipeLineOutputDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	CPipeLineOutputDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	virtual ~CPipeLineOutputDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINEOUTPUT };

protected:

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

public:
	CTabCtrl m_tab;
	CPipeLineRoadInfo m_road;
	CPipeLineSelectOutputDlg m_sout;
};

// CPipeLineCreateProjectDlg �Ի���

class CPipeLineCreateProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLineCreateProjectDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
	 */
	CPipeLineCreateProjectDlg(CWnd* pParent = NULL);

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
	 */
	virtual ~CPipeLineCreateProjectDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINECREATEPROJECT };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
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

// CSubPipeLinePropertyDlg �Ի���

class CSubPipeLinePropertyDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSubPipeLinePropertyDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	CSubPipeLinePropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual ~CSubPipeLinePropertyDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SUBPIPELINEPROPERTYDLG };

	void drawFinish();

	void addPoint(const AcGePoint2d &, const AcDbObjectId &, bool isNew = false, CString dh = _T(""));

	PipeLineTable getCurrentPipeLineType();

	afx_msg void OnClickedButtonPipelineReflashTable();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��28��
	 */
	void selectPoint(const AcDbObjectIdArray &);

	void reflashPLProperty();

protected:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnSelectShowPipeLine();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickedSelectRow(NMHDR *, LRESULT *);
	afx_msg void OnClickedButtonPipelineCorrelationEntity();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��8��
	 */
	void initListCtrl();

	void initComboxCtrl();

	void initComboxCtrlGXLX();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	void drawLineRemark();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
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

// CSubPipePointPropertyDlg �Ի���

class CSubPipePointPropertyDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSubPipePointPropertyDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	CSubPipePointPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual ~CSubPipePointPropertyDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SUBPIPEPOINTPROPERTYDLG };

protected:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	afx_msg void OnSize(UINT nType, int cx, int cy);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	/* @�ӿ� �ܵ�˫���¼�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	afx_msg void OnClickedOnSelectRow(NMHDR* pNMHDR, LRESULT* pResult);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��20��
	 */
	afx_msg void OnSelectShowPipePoint();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	void updatePipeLineProperty(const CString &, int &);

	DECLARE_MESSAGE_MAP()

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	void initListCtrl();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��20��
	 */
	void initComboxCtrl();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	void initListCtrlValue();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	void middleEntity(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	void matchingPipePoint(int);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
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
	map<CString, AcDbObjectIdArray> m_pids;		//�Ե������
	map<CString, AcDbObjectIdArray> m_lids;		//��ͼ������
	map<CString, AcDbObjectId, CmpCstr> m_sid;

public:


	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��28��
	 */
	void selectPoint(const AcDbObjectIdArray &);

	void flushPipeLineProperty();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPipepointreassociate();
	afx_msg void OnBnClickedButtonPipepointflushaproperty();

	afx_msg void OnBnClickedCheckMatchPipePoint();
	afx_msg void OnBnClickedCheckMatchPipeLine();
};

// CPipeLinePropertyDlg �Ի���

class CPipeLinePropertyDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CPipeLinePropertyDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	CPipeLinePropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual ~CPipeLinePropertyDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINEPROPERTYDLG };

protected:

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
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

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	CCommonDockControlBar();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual ~CCommonDockControlBar();

	/* @�ӿ� �����ӶԻ���
	 * @���� CDialog *�ӶԻ���
	 * @���� int �ӶԻ���ID
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	void SetChildDlg(CDialog* p_dlg, int IDD);

	/* @�ӿ� �����Ի���
	 * @���� CWnd *
	 * @���� TCHAR *����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual BOOL Create(CWnd*pParent, TCHAR *lpszTitle);

	/* @�ӿ� ���ٶԻ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	virtual BOOL DestroyWindow();

	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void SizeChanged(CRect *lpRect, BOOL bFloating, int flags);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��4��
	 */
	afx_msg void OnHideBar();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��4��
	 */
	virtual bool OnClosing();

public:

	int m_IDD;
	CDialog* m_p_dlg;

};

// CModifyRoadPro �Ի���
class CModifyRoadPro
	: public CAdUiDialog
{
	DECLARE_DYNAMIC(CModifyRoadPro)

public:
	CModifyRoadPro(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyRoadPro();

	/* @�ӿ� ��ȡͼ�ϵ�·������id
	 */
	BOOL GetRoadIds();

	/* @�ӿ� ��ʼ��ComBox��ListControl�ؼ�����
	 */
	void ComboxAndList();

	/* @�ӿ� �Ի����ʼ��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ� ComBox����ѡ��仯��ѡ��ͬ��·���¼�
	 */
	void OnSelRoad();

	void getRoadInfo(const AcDbObjectId &);

	void saveRoadInfo(const AcDbObjectId &);

	void readRoadInfo(const AcDbObjectId &);

	void OnBnClickedAdd();
	void OnBnClickedDel();
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancel();

	/* @�ӿ� ���������ı��ı༭ˢ��ListControl�¼����˴�����ˢ�¡��߲ֵ
	 */
	afx_msg void OnLvnItemchangingList1(NMHDR *pNMHDR, LRESULT *pResult);

	// �Ի�������
	enum { IDD = IDD_DIALOG_MODIFYROADPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CFilterLayerDlg �Ի���

class CFilterLayerDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CFilterLayerDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��17��
	 */
	CFilterLayerDlg(const VStr &layers, CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��17��
	 */
	virtual ~CFilterLayerDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_FILTERLAYERDLG };

	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��16��
	 */
	void setLayers(const VStr &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��16��
	 */
	VStr getLayers();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��12��
	 */
	double getScale();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CPipeConfig �Ի���

class CSZConfig : public CDialog
{
	DECLARE_DYNAMIC(CSZConfig)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� bool ��ʾ�öԻ����Ǵ�����Ŀ�������޸���Ŀ��Ϣ
	* @���� CWnd* ����Ի����ָ��
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��2��
	*/
	CSZConfig(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��2��
	*/
	virtual ~CSZConfig();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SHIZHENG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	/* @�ӿ�
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��16��
	*/
	void Apply();

private:

	/*�Ի����ʼ��
	*/
	virtual BOOL OnInitDialog();

private:

	/* @�ӿ� ������Ŀ��Ϣ
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��2��
	*/
	void setProjectInfo(CString &strPath);

	/* @�ӿ� ��ʼ����Ŀ��Ϣ
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��2��
	*/
	void initProjectInfo();

};

// CSZProjectDlg �Ի���

class CSZProjectDlg
	: public CDialog
{
	DECLARE_DYNAMIC(CSZProjectDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��16��
	*/
	CSZProjectDlg(CWnd* pParent = NULL);

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��16��
	*/
	virtual ~CSZProjectDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PIPELINECREATEPROJECT1 };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	/* @�ӿ�
	* @����
	* @����
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��15��
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


// TQZBINFO �Ի���

class CTQZBINFO : public CDialog
{
	DECLARE_DYNAMIC(CTQZBINFO)

public:

	CTQZBINFO(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CTQZBINFO();

	/* @�ӿ� �Ի����ʼ��
	*/
	virtual BOOL OnInitDialog();

	/* @�ӿ� ��Ŀ���������ɾ��
	 * @ʱ�� 2018��4��18��
	 */
	void OnBnClickedAdd();

	void OnBnClickedDel();

	/* @�ӿ� �������������ɾ��
	 * @ʱ�� 2018��4��18��
	 */
	void OnBnClickedAdd2();

	void OnBnClickedDel2();

	/* @�ӿ� ��ȡList2���ݣ����ڱ������ȡ
	 * @ʱ�� 2018��4��18��
	 */
	VStr GetList2Data();

	/* @�ӿ� ������ͬ��Ŀ��ˢ��List2����������ע
	 * @ʱ�� 2018��4��19��
	 */
	afx_msg void OnClickList1Main(NMHDR *, LRESULT *);

	/* @�ӿ� ����������
	 * @ʱ�� 2018��4��19��
	 */
	afx_msg void OnLvnEndlabeleditList1(NMHDR *, LRESULT *);

	/* @�ӿ� ���汸ע
	 * @ʱ�� 2018��4��19��
	 */
	afx_msg void OnLvnEndlabeleditList2(NMHDR *, LRESULT *);

	afx_msg void OnBnClickedOK();

	afx_msg void OnBnClickedCancel();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��9��12��
	 */
	void setDialogInfo(const VMStr &);


	// �Ի�������
	enum { IDD = IDD_DIALOG_TQZBINFO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

// CRoadHDMTLDlg �Ի���

class CRoadHDMTLDlg
	: public CAdUiDialog
{
	DECLARE_DYNAMIC(CRoadHDMTLDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��11��
	 */
	CRoadHDMTLDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��11��
	 */
	virtual ~CRoadHDMTLDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_ROADHDMDZTTL };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��11��
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

// CLayerManagerDlg �Ի���

class CLayerManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayerManagerDlg)

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��14��
	 */
	CLayerManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��14��
	 */
	virtual ~CLayerManagerDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_LAYERMANAMGER };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

};

// CModifyProjectInfo �Ի���

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

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��16��
	 */
	CModifyProjectInfo(CWnd* pParent = NULL);   // ��׼���캯��

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��16��
	 */
	virtual ~CModifyProjectInfo();

	// �Ի�������
	enum { IDD = IDD_DIALOGMODIFYPROJECTINFO };

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��16��
	 */
	void setProjectInfo(const ProjectInfos &);

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��24��
	 */
	ProjectInfos getProjectInfos();

protected:

	/* @�ӿ� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��16��
	 */
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��16��
	 */
	virtual BOOL OnInitDialog();

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��16��
	 */
	void OnLvnClickListItem(NMHDR *pNMHDR, LRESULT *pResult);

private:

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��24��
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
