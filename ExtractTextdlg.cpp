// ExtractTextdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExtractTextdlg.h"

// CExtractTextdlg 对话框

IMPLEMENT_DYNAMIC(CExtractTextdlg, CAdUiBaseDialog)

CExtractTextdlg::CExtractTextdlg(CWnd* pParent /*=NULL*/)
	: CAdUiBaseDialog(CExtractTextdlg::IDD, pParent)
{

}

CExtractTextdlg::~CExtractTextdlg()
{
}

void CExtractTextdlg::DoDataExchange(CDataExchange* pDX)
{
	CAdUiBaseDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAINTEXT, m_strText);
}


BEGIN_MESSAGE_MAP(CExtractTextdlg, CAdUiBaseDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CExtractTextdlg::OnBnClickedButtonSelect)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()


// CExtractTextdlg 消息处理程序

BOOL CExtractTextdlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void CExtractTextdlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
	delete this;
}

LRESULT CExtractTextdlg::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}

BOOL CExtractTextdlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message   ==   WM_KEYDOWN)  
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			DestroyWindow();
			delete this;
			return WM_QUIT;
		}
	}

	return CAdUiBaseDialog::PreTranslateMessage(pMsg);
}


void CExtractTextdlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码

	BeginEditorCommand();
	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 2, _T("*TEXT*"));
	CompleteEditorCommand();

	CString strText;
	 for(int i = 0; i < objArr.length(); i ++)
	 {
		 TCHAR szText[1000] = {0};
		 GetEntText(aname(objArr.at(i)), szText);
		 strText += szText;
		 strText += _T("\r\n");
	 }
	 m_strText = strText;

	 UpdateData(FALSE);
}
