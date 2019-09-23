// MatchFialogsEx.cpp : 实现文件
//

#include "stdafx.h"
#include "MatchFialogsEx.h"
//#include "afxdialog.h"


// CMatchFialogsEx 对话框

IMPLEMENT_DYNAMIC(CMatchFialogsEx, CDialog)

CMatchFialogsEx::CMatchFialogsEx(CWnd* pParent /*=NULL*/)
	: CDialog(CMatchFialogsEx::IDD, pParent)
{
	m_nColCount = 0;
	m_ObjId = NULL;
	_tcscpy(m_szListCtrlConfigFile, _T("字体匹配表列表控件.XML"));
	m_dFrameScale = 0;
	m_bShowFrameScaleControl = false;
	_tcscpy(m_szDivWord, _T(","));
}

CMatchFialogsEx::~CMatchFialogsEx()
{
}

bool CMatchFialogsEx::getListData()
{
	if(m_ListMain.GetHeaderCtrl()->GetItemCount() < 4) return false;

	CString strItem;
	for(int i = 0; i < m_ListMain.GetItemCount(); i ++)
	{
		strItem.Format(_T("%s$%s$%s$%s"), 
			m_ListMain.GetItemText(i, 0), m_ListMain.GetItemText(i, 1), 
			m_ListMain.GetItemText(i, 2), m_ListMain.GetItemText(i, 3));

		m_vLsiteData.push_back(strItem);
	}
	return true;
}


bool CMatchFialogsEx::getDatas()
{
	int i = 0, j = 0;
	CStringArray strArrSave;
	m_strArrKey.RemoveAll();
	m_strArrName.RemoveAll();
	m_strArrValue.RemoveAll();
	m_strArrType.RemoveAll();
	strArrSave.Add(m_strHeader);

	


	TCHAR szData[512] = {0};
	TCHAR szValue[100]={0};
	for (size_t i = 0; i < m_vLsiteData.size(); ++i)
	{
		CString strRow;

		CStringArray strArr;
		AnalyzeData(strArr, m_vLsiteData.at(i), _T("$"));
		if(strArr.GetSize() < 4) continue;

		CString strKey = strArr.GetAt(0);		//关键字
		CString strName = strArr.GetAt(1);	//名称
		CString strValue = strArr.GetAt(2);	//值
		CString strMark = strArr.GetAt(3);		//替换方式
		int sMark = _ttoi(strMark);

		strRow.Format(_T("%s%s%s%s%s%s%s"), strKey, m_szDivWord, strName,  m_szDivWord, strValue,  m_szDivWord, strMark);

		strValue.Replace(_T("["),_T(""));
		strValue.Replace(_T("]"),_T(""));

		_stprintf(szValue,_T("%s"),strValue);
		if(sMark == 1 || sMark == 11 || sMark == 12)
		{

			int nWitch = 0;
			if(strValue.Find(_T("|"))>= 0)
			{
				TCHAR sz1[100] = {0}, sz2[10] = {0};
				analyzeData(szValue, _T("|"), sz1, sz2);
				nWitch = _ttoi(sz2);
			}

			PurgeString(szValue);
			int nType = 0;
			if(GetXDataType(m_ObjId,szValue, nWitch,nType) == false)
			{
				strValue = _T("");
			}
			else
			{
				TCHAR szXdata[100] ={0};
				READMulXData(m_ObjId,szValue,szXdata,nType, nWitch);
				strValue = szXdata;
			}

			if(sMark == 11)
			{
				int nCode = TrimString2(strValue);
				strValue.Format(_T("%d"), nCode);
			}
			else if(sMark == 12)
			{
				strValue = TrimString3(strValue);
			}
		}
		else if(sMark == 2)
		{
			CXRecordOperate xoperate;
			CString strText;
			xoperate.ReadStringRecord(_T("Elec_Offer_Interface"), strValue, strText);
		}


		m_strArrKey.Add(strKey);
		m_strArrValue.Add(strValue);

		strArrSave.Add(strRow);
	}

	TCHAR szPath[1000] = {0};
	_tcscpy(szPath, m_strPath);
	SaveTxtFile(strArrSave, szPath);

	return true;
}

void CMatchFialogsEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAIN, m_ListMain);
	DDX_Radio(pDX, IDC_RADIO_FRAME_SCALE, m_dFrameScale);
}


BEGIN_MESSAGE_MAP(CMatchFialogsEx, CDialog)
	ON_BN_CLICKED(IDOK, CMatchFialogsEx::OnBnClickedOk)
END_MESSAGE_MAP()

void	CMatchFialogsEx::showControl()
{
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RADIO_FRAME_SCALE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RADIO3)->ShowWindow(SW_SHOW);
}


BOOL CMatchFialogsEx::OnInitDialog()
{
    CDialog::OnInitDialog();

	if(m_bShowFrameScaleControl) showControl();

    m_ListMain.Init_ByConfig(m_szListCtrlConfigFile);

    TCHAR szPath[255] = {0};
    _tcscpy(szPath, m_strPath.GetBuffer(0));
    m_strPath.ReleaseBuffer();
    CStringArray strTxtArr;
    readTxtFile(strTxtArr, szPath);
    int i = 0, j = 0;
    bool bFirst = true;
    int nCol = 1;
    int nCurRow = 0;

    for (i = 0; i < strTxtArr.GetSize(); ++i)
    {
        CString strTxt = strTxtArr.GetAt(i);
        strTxt.TrimLeft();
        strTxt.TrimRight();

        if (strTxt.Left(2) == _T("//") || strTxt == _T(""))
        {
            continue;
        }

        CStringArray SplitStrArr;
        AnalyzeData(SplitStrArr, strTxt, m_szDivWord);

        //添加字段列
        if (bFirst)
        {
            m_strHeader = strTxt;
            nCol = SplitStrArr.GetSize();
            m_nColCount = nCol;

//             for (j = 0; j < SplitStrArr.GetSize(); ++j)
//             {
//                 CString strCol = SplitStrArr.GetAt(j);
//                 strCol.TrimLeft();
//                 strCol.TrimRight();
//                 //m_ListMain.InsertColumn(j, strCol, LVCFMT_CENTER, 60);
//                 //m_ListMain.SetColumnReadOnly(j, FALSE);
// 
//             }

            bFirst = false;
            //nCurRow++;
        }
        //添加行
        else
        {
            if (nCol != SplitStrArr.GetSize())
            {
                continue;
            }

            for (j = 0; j < SplitStrArr.GetSize(); ++j)
            {
                CString strData = SplitStrArr.GetAt(j);
                strData.TrimLeft();
                strData.TrimRight();

                if (j == 0)
                {
                    m_ListMain.InsertItem(nCurRow, strData);
                    m_strArrKey.Add(strData);
                }
                else
                {
#if _MSC_VER > 1200
                    BOOL b = m_ListMain.SetItem(nCurRow, j, LVIF_TEXT, strData, 0, LVIS_SELECTED, LVIS_SELECTED, 0, 0);
#else
                    BOOL b = m_ListMain.SetItem(nCurRow, j, LVIF_TEXT, strData, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
#endif

                    if (1 == j)
                    {
                        m_strArrName.Add(strData);
                    }
                    else if (2 == j)
                    {
                        m_strArrValue.Add(strData);
                    }

                    /*if (!b)
                    {
                    	acutPrintf(_T("\n%d行%d列"),nCurRow,j+1);
                    }*/
                }
            }

            nCurRow++;
        }
    }

    m_nRowCount = nCurRow;
    //设置列宽
//     CRect rect;
//     m_ListMain.GetWindowRect(rect);
//     int width = int(rect.Width() / 4);
//     m_ListMain.SetColumnWidth(0, 0);
//     m_ListMain.SetColumnWidth(1, width * 1.5);
//     m_ListMain.SetColumnWidth(2, width * 1.5);

//     if (m_bIsMonospaced)
//     {
//         width = int(rect.Width() / 2);
//         m_ListMain.SetColumnWidth(0, 0);
//         m_ListMain.SetColumnWidth(1, width);
//         m_ListMain.SetColumnWidth(2, width);
//     }

// 	if(m_vReadOnly.size() > 0 && m_vReadOnly.size() <= m_ListMain.GetHeaderCtrl()->GetItemCount())
// 	{
// 		for(size_t t = 0; t < m_vReadOnly.size(); t ++)
// 		{
// 			if(m_vReadOnly.at(t) == 1)
// 				m_ListMain.SetColumnReadOnly(t, TRUE);
// 		}
// 	}

    UpdateData(FALSE);
    return TRUE;
}


// CMatchFialogsEx 消息处理程序


void CMatchFialogsEx::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	this->getListData();

	this->getDatas();

	CDialog::OnOK();
}


bool CMatchFialogsEx::SetObjId( const AcDbObjectId ObjId )
{
	if (ObjId == NULL)
	{
		return false;
	}
	else
	{
		m_ObjId = ObjId;
		return true;
	}
}

void CMatchFialogsEx::SetListCtrlConfigFile(TCHAR *szFile)
{
	_tcscpy(m_szListCtrlConfigFile, szFile);
}

bool CMatchFialogsEx::SetConfigFile(TCHAR *szFile )
{
	TCHAR tmpszPath[255] = {0};

	if (RTNORM != acedFindFile(szFile, tmpszPath))
	{
		acutPrintf(_T("\n未找到配置文件！"));
		return false;
	}
	else
	{
		m_strPath = tmpszPath;
		return true;
	}
}
