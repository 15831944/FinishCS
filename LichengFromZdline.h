// LichengFromZdline.h: interface for the CLichengFromZdline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LICHENGFROMZDLINE_H__09672EA1_2996_4BC9_B380_1F5A82EB7611__INCLUDED_)
#define AFX_LICHENGFROMZDLINE_H__09672EA1_2996_4BC9_B380_1F5A82EB7611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHeightDlg dialog
//class CHeightDlg : public CDialog
//{
//// Construction
//public:
//	CHeightDlg(CWnd* pParent = NULL);   // standard constructor
//
//// Dialog Data
//	//{{AFX_DATA(CHeightDlg)
//	enum { IDD = IDD_DIALOG_HEIGHT };
//	double	m_height;
//	//}}AFX_DATA
//	int		m_left;
//	int		m_top;
//
//
//// Overrides
//	// ClassWizard generated virtual function overrides
//	//{{AFX_VIRTUAL(CHeightDlg)
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	//}}AFX_VIRTUAL
//
//// Implementation
//protected:
//
//	// Generated message map functions
//	//{{AFX_MSG(CHeightDlg)
//	virtual BOOL OnInitDialog();
//	//}}AFX_MSG
//	DECLARE_MESSAGE_MAP()
//};

class CLichengFromZdline  
{
public:
	void writeFile(AcDbObjectId &idLine, double &dStart);
	void hdmdesign();
	void hdmtrim();
	void changelength();
	void hdmadd();
	void GetInfo(AcDbObjectId &idLine);
	bool hdmcreate(AcDbObjectId &idLine, double &dStart);
	CLichengFromZdline();
	virtual ~CLichengFromZdline();

private:
	AcDbObjectId m_groupId;
	double m_dQSLC;
	CString m_strDLMC, m_strLCZMC;
	int m_nSCorSJ;//nStdm: 0:实测道路中心线 1:设计道路中心线
	AcDbObjectIdArray DmxArr;

	void AddHdmLine2(ads_point pt0, double dAngle, double dLeft, double dRight);
	void AddHdmLine(ads_point pt0, double dAngle, double dLeft, double dRight, int nColor = -1);
	double GetMiddleAngle(ads_point pt1,ads_point pt,ads_point pt2);
	bool SelectMainLine(ads_name ent);
	void TrimOneHdm(ads_name entMain,ads_name entTrim,AcDbObjectId objId);
	void StandLineDmx(ads_name entMain,AcDbObjectIdArray &DmxArr,AcGeDoubleArray &LichengArr,AcGePoint3dArray &InterPtArr,AcGeDoubleArray &AngleArr);
	void InsertPointAtDmx(ads_name ent,ads_name entMain,double dStep,AcGePoint3dArray &Nodes,AcGeDoubleArray &DistArr);
	bool GetDesignHeight(ads_name ent,double &dDist1,double &dHeight1,double &dDist2,double &dHeight2);
	bool GetPointDeriv(AcDbCurve *pCurve,AcGePoint3d pt3d,double &dAngle);
	void WriteMiddlePointNumber(AcGePoint3dArray InterPtArr, double dBegin, AcGeDoubleArray LichengArr, AcGeDoubleArray AngleArr, const CString &strLCZMC, AcDbObjectId &idLine);
	bool GetPointNearMiddle(AcGePoint3dArray &tempPts,AcGePoint3d pt1,AcGePoint3d pt2,AcGePoint3d &ptInt);
	void GetHeadTailPoint(ads_name ent,ads_name entMain,AcGePoint2dArray &jiePtArr,AcGeDoubleArray &jieDistArr,ads_point ptt1,ads_point pttM,ads_point ptt2);
	void WriteHeighttoDMX(AcDbObjectId &idLine, AcGePoint3dArray InterPtArr);
};

#endif // !defined(AFX_LICHENGFROMZDLINE_H__09672EA1_2996_4BC9_B380_1F5A82EB7611__INCLUDED_)
