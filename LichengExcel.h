#ifndef _VERFACE_H_
#define _VERFACE_H_
#include "StdAfx.h"
#include <excel9.h>
#include <OperateExcel.h>
#include <workdef10.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <tcode.h>
#include <map>
using namespace std;

typedef pair<double, double> pairitem;
class Picket // ׮
{
public:
	tstring m_num;
	vector<pairitem> m_items;
	int m_left;
	int m_right;
};

class VerFace 
{     
public:
    VerFace(){
		m_nDecimal = 3; 
		m_nTitleSize = 20;
		m_csTitleName = _T("������κ");
		m_bTitleBold = TRUE;
        m_nSurSize = 14;
        m_csSurName = _T("����");
        m_bSurBold = FALSE;
		m_nInfoSize = 12;
		m_csInfoName = _T("����");
		m_bInfoBold = FALSE;
		m_nContSize = 12;
		m_csContName = _T("����");
		m_bContBold = FALSE;
		m_nNumPerPage = 12;
	}
    ~VerFace()
        {  }
	//��ʽ��С��λ��
	CString FormatDouble(double dValue, int nDecimal)
	{
		CString strValue, strFormat;

		strFormat.Format(_T("%%.%df"), nDecimal);
		strValue.Format(strFormat, dValue);
        return strValue;
	}
	CString FormatDouble(const CString &str, int nDecNum)
	{
		CString strRes = _T("0.000");
		
		double dTmp = _tcstod(str,NULL);
		CString strFomate;
		strFomate.Format(_T("%%.%df"),nDecNum);
		strRes.Format(strFomate,dTmp);
		
		return strRes;
	}
	BOOL readFileOut(const TCHAR *fname, vector<Picket> &pckArr, int &left, int &right);
	BOOL xlsOut(
		const TCHAR *fname,
		const TCHAR *title
		);
    BOOL xlsOut(
        const TCHAR *title,
        const CStringArray &pntNum,
        const AcGePoint2dArray &pntArr,
        const AcGeDoubleArray &heightArr,
		const CStringArray &remarks
        );
	void setDecimal(int nDec);
	void setNumPerPage(int nNum);
    void setTitleFont(int size, const TCHAR *fontname, BOOL bold);
    void setSurveyFont(int size, const TCHAR *fontname, BOOL bold);
    void setInfoFont(int size, const TCHAR *fontname, BOOL bold);
    void setContentFont(int size, const TCHAR *fontname, BOOL bold);
	int getDecimal();
	int getNumPerPage();
public:
	int m_nDecimal;
	// title font setting[font]
	int m_nTitleSize;
	CString m_csTitleName;
	BOOL m_bTitleBold;
    // survey font setting[font]
    int m_nSurSize;
    CString m_csSurName;
    BOOL m_bSurBold;
	// info font setting[font]
	int m_nInfoSize;
	CString m_csInfoName;
	BOOL m_bInfoBold;
	// content font setting[font]
	int m_nContSize;
	CString m_csContName;
	BOOL m_bContBold;



	CString m_csObserver;       // �۲�
	CString m_csRecord;         // ��¼
	CString m_csMeasure;        // ����
	CString m_csCompute;        // �ﵩ
	int m_nNumPerPage;          // ÿһСҳ׮�ŵĸ���
};

#endif
