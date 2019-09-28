// DrawDomainDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrawDomainDialog.h"
#include "afxdialogex.h"
#include "Global.h"
#include "domainoperate.h"

FunctionNameTableEx g_fntEx;

// DrawDomainDialog �Ի���

IMPLEMENT_DYNAMIC(DrawDomainDialog, CDialog)

DrawDomainDialog::DrawDomainDialog(CWnd* pParent /*=NULL*/)
: CDialog()
{
	g_fntEx.m_init = false;
}

DrawDomainDialog::~DrawDomainDialog()
{

}

void DrawDomainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IMPORTBH, m_imPortBH);
	DDX_Control(pDX, IDC_COMBO_JRFACTOR, m_jrFactor);
	DDX_Control(pDX, IDC_COMBO_DOMAINNAME, m_domainName);
	DDX_Control(pDX, IDC_COMBO_AREAFACTOR, m_areaFactor);
	DDX_Control(pDX, IDC_CHECK_AREAKOUCHU, m_areaKouChu);
	DDX_Control(pDX, IDC_COMBO_SELECTSTYLE, m_selectStyle);
	DDX_Control(pDX, IDC_EDIT_DOMAINHEIGHT, m_domainHeight);
}

BEGIN_MESSAGE_MAP(DrawDomainDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BEGINDRAW, &DrawDomainDialog::OnBnClickedButtonBegindraw)
	ON_BN_CLICKED(IDC_CHECK_SELECTBH, &DrawDomainDialog::OnBnClickedCheckSelectbh)
	ON_CBN_SELCHANGE(IDC_COMBO_DOMAINNAME, &DrawDomainDialog::OnCbnSelchangeComboDomainname)
END_MESSAGE_MAP()

// DrawDomainDialog ��Ϣ�������

BOOL DrawDomainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	IDataBaseOper dbOper;
	m_domainName.AddString(_T(" "));
	dbOper.readFunctionTable(m_fnts);
	for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
	{
		FunctionNameTable fnt = m_fnts[idx];
		m_domainName.AddString(fnt.name);
	}
	m_selectStyle.AddString(_T("��ѡ"));
	m_selectStyle.AddString(_T("�߽�"));
	m_selectStyle.AddString(_T("�ֶ�����"));

	m_selectStyle.SetCurSel(0);
	this->MoveWindow(0, 0, 400, 30);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void DrawDomainDialog::OnBnClickedCheckSelectbh()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void DrawDomainDialog::setControlStatus(BOOL flag)
{
	m_imPortBH.EnableWindow(flag);
	m_jrFactor.EnableWindow(flag);
	m_areaKouChu.EnableWindow(flag);
	m_domainName.EnableWindow(flag);
	m_areaFactor.EnableWindow(flag);
	m_selectStyle.EnableWindow(flag);
	m_domainHeight.EnableWindow(flag);
}

void DrawDomainDialog::OnCbnSelchangeComboDomainname()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString domainName;	int curSel = m_domainName.GetCurSel();
	m_domainName.GetLBText(curSel, domainName);	//��ȡCComBox������ǰѡ�е�ֵ
	for(int idx = 0; idx < (int)m_fnts.size(); ++idx)
	{
		FunctionNameTable fnt = m_fnts[idx];
		if(domainName.CompareNoCase(fnt.name)) continue;
		m_jrFactor.SetWindowText(fnt.jrxs);
		m_areaFactor.SetWindowText(fnt.mjxs);
	}
}

void DrawDomainDialog::OnBnClickedButtonBegindraw()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	setControlStatus(FALSE);
	DomainOperate domainOperate; 
	acDocManager->lockDocument(curDoc());//������ǰDWG�ĵ�
	acedGetAcadDockCmdLine()->SetFocus();
	m_areaFactor.GetWindowText(m_mjxs);//��ȡ���ϵ��������е�ֵ
	m_mjxs.Trim();//ȥ����β���пո�
	if (m_mjxs.GetLength() == 0 || m_mjxs.GetLength() != m_mjxs.SpanIncluding(_T("0123456789.")).GetLength())//�ж����ϵ������ֵ�Ƿ���Ϲ淶
	{
		CString cstrError = _T("���ϵ�����벻���Ϲ淶��");
		if (m_mjxs.IsEmpty() == true)cstrError = _T("���������ϵ��");
		MessageBox(cstrError, _T("������Ϣ"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	m_jrFactor.GetWindowText(m_jrxs);//��ȡ����ϵ��������е�ֵ
	m_jrxs.Trim();//ȥ����β���пո�
	if (m_jrxs.GetLength() != m_jrxs.SpanIncluding(_T("0123456789.")).GetLength())//�жϼ���ϵ������ֵ�Ƿ���Ϲ淶
	{
		CString cstrError = _T("����ϵ�����벻���Ϲ淶��");
		if (m_jrxs.IsEmpty() == true)cstrError = _T("���������ϵ��");
		MessageBox(cstrError, _T("������Ϣ"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	CString selsty;//ʰȡʵ��ķ�ʽ
	m_selectStyle.GetWindowText(selsty);//��ȡʰȡ��ʽ������е�ֵ
	selsty.Trim();//ȥ����β���пո�
	if (selsty.GetLength() == 0 || selsty.GetLength() != selsty.SpanIncluding(_T("�߽��ѡ�ֶ�����")).GetLength())
	{
		CString cstrError = _T("û�и�ʰȡ��ʽ��������ѡ��");
		if (selsty.IsEmpty() == true)cstrError = _T("��ѡ���������Ʒ�ʽ");
		MessageBox(cstrError, _T("������Ϣ"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	CString gd;
	m_domainHeight.GetWindowText(gd);//��ȡ�������߶�������е�ֵ
	gd.Trim();//ȥ����β���пո�
	if (gd.GetLength() == 0 || gd.GetLength() != gd.SpanIncluding(_T("0123456789.")).GetLength())
	{
		CString cstrError = _T("�������߶����벻�淶��");
		if (gd.IsEmpty() == true)cstrError = _T("�����빦�����߶�");
		MessageBox(cstrError, _T("������Ϣ"), 0);
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	CString name;//����������
	m_domainName.GetWindowText(name);
	name.Trim();//������������β�������пո�
	CString bh;//���������
	m_imPortBH.GetWindowText(bh);
	bh.Trim();
	if (bh.GetLength() != bh.SpanIncluding(_T("0123456789")).GetLength())
	{
		acutPrintf(_T("\n��������ű���Ϊ�����ֱ��"));
		acDocManager->unlockDocument(curDoc());
		setControlStatus(TRUE);
		return;
	}
	BOOL isFirstDraw = true;
	while(TRUE)
	{
		bool flag = true;//true��������ʰȡ�ɹ���false��ȡ��ʰȡ������
		m_drawStyle = m_selectStyle.GetCurSel();
		AcDbObjectId gnqid;//ѡ��Ķ����ID��
		switch(m_drawStyle)
		{
			case 0: flag = domainOperate.createDomainByBoundary(gnqid); break;
			case 1: flag = domainOperate.createDomainByPointSel(gnqid); break;
			case 2: flag = domainOperate.createDomainByDraw(gnqid); break;
		}
		if (!flag)//ȡ��ʰȡ��������ֱ�ӷ���
		{
			acDocManager->unlockDocument(curDoc());
			setControlStatus(TRUE);
			return;
		}
		if(name.IsEmpty())
		{
			do
			{
				ads_name txtent;
				ads_point txtpt;
				int ret = ads_entsel(_T("\n��ʰȡ���������ƣ�"), txtent, txtpt);
				TCHAR enttype[255] = { 0 };
				if (RTNORM == ret)//ʵ��ʰȡ�ɹ����ж�ʵ�������Ƿ����Ҫ��
				{
					if (!GetEntityType(txtent, enttype))
					{
						acutPrintf(_T("\n�޷��ж��Ƿ�Ϊ�ı���������ʰȡ"));
						continue;
					}
					if (_tcscmp(enttype, _T("TEXT")) != 0 && _tcscmp(enttype, _T("MTEXT")) != 0)
					{
						acutPrintf(_T("\n��ʰȡ�ı�"));
						continue;
					}
					else
					{
						TCHAR txt[255] = { 0 };
						GetEntText(txtent, txt);
						name = txt;
						break;
					}
				}
				else
				{
					acutPrintf(_T("\n��ȡ��ʰȡ����������"));
					break;
				}
			} while (TRUE);
		}
		if(bh.IsEmpty())
		{
			do
			{
				ads_name txtent;
				ads_point txtpt;
				int ret = ads_entsel(_T("\n��ʰȡ��������ţ�"), txtent, txtpt);
				TCHAR enttype[255] = { 0 };
				if (RTNORM == ret)//ʵ��ʰȡ�ɹ����ж�ʵ�������Ƿ����Ҫ��
				{
					if (!GetEntityType(txtent, enttype))
					{
						acutPrintf(_T("\n�޷��ж��Ƿ�Ϊ�ı���������ʰȡ"));
						continue;
					}
					if (_tcscmp(enttype, _T("TEXT")) != 0 && _tcscmp(enttype, _T("MTEXT")))
					{
						acutPrintf(_T("\n��ʰȡ�ı�"));
						continue;
					}
					else
					{
						TCHAR txt[255] = { 0 };
						GetEntText(txtent, txt);
						bh = txt;
						break;
					}
				}
				else//ȡ��ʰȡע�Ǳ���ı���ֱ�ӷ���
				{
					acutPrintf(_T("\n��ȡ��ʰȡ���������"));
					break;
				}
			} while (TRUE);
		}
		if (false == bh.IsEmpty())
		{
			while (TRUE)
			{
				if (isFirstDraw == FALSE)
				{
					int ibh = _tstoi(bh);
					ibh++;
					int cstrOriLen = bh.GetLength();
					bh.Format(_T("%d"), ibh);
					int cstrNewLen = bh.GetLength();
					for (int i = 0; i < cstrOriLen - cstrNewLen; i++)
					{
						bh.Insert(0, _T("0"));
					}
				}
				else//��һ�����ֱ�Ӳ����Ƿ���ڣ����Լ�һ
				{
					isFirstDraw = FALSE;
				}
				DomainOperate domainoperate;
				AcDbObjectId fcpmid = domainoperate.getBuildLayerRangeEntity(gnqid);
				AcDbObjectIdArray gnqids;
				if (fcpmid == AcDbObjectId::kNull)
				{
					SelectFilter sfe(8, _T("������")), sfl(RTDXF0, _T("*POLYLINE"));
					SelectEntitys(gnqids, sfe, sfl, _T("X"));
				}
				else
				{
					AcGePoint2dArray fcpmnodes;
					GetPlList(aname(fcpmid), fcpmnodes);
					CString gnqlayer;
					IDataBaseOper oper;
					oper.readCommonTable(_T("��������������ͼ��"), gnqlayer);
					TCHAR cstrlayer[255] = { 0 };
					_stprintf(cstrlayer, _T("%s"), gnqlayer);
					ssFromNodes(gnqids, fcpmnodes, 1, 0.5, _T("*POLYLINE"), cstrlayer);
				}
				BOOL existsamename = FALSE;
				for (int idx = 0; idx < gnqids.length(); idx++)
				{
					AcDbObjectId Xgnqid = gnqids.at(idx);
					TCHAR Xname[255] = { 0 };
					ReadXdata(aname(Xgnqid), _T("����������"), 0, Xname);
					TCHAR Xbh[255] = { 0 };
					ReadXdata(aname(Xgnqid), _T("���������"), 0, Xbh);
					if (_tcscmp(Xname, name) == 0 && _tcscmp(Xbh, bh) == 0)
					{
						existsamename = TRUE;
						break;
					}
				}
				if (existsamename == FALSE)break;
			}
		}
		bool isLand = (1 == m_areaKouChu.GetCheck());
		domainOperate.addDomainProperty(gnqid, name, m_mjxs, m_jrxs, gd, bh, isLand);
		acutPrintf(_T("\n�ع��������ɹ�"));
	}
}

BOOL DrawDomainDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_ESCAPE: //Esc�����¼�  
				return true;
			case VK_RETURN: //Enter�����¼�  
				return true;
			case VK_SPACE:
				return true;
			default:;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
