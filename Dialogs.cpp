// Dialogs.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dialogs.h"

// CProjectInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectInfoDlg, CDialog)

CProjectInfoDlg::CProjectInfoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CProjectInfoDlg::IDD, pParent)
{

}

CProjectInfoDlg::~CProjectInfoDlg()
{
}

bool CProjectInfoDlg::setPreProjectInfo()
{
	CString xml_str;
	if(project_type == "XMJG_RoadAlignment")
	{
		xml_str = "��Ŀ��Ϣ_��ʱ����_XMJG_RoadAlignment.XML";
	}
	else if(project_type == "XMJG_RedLine")
	{
		xml_str = "��Ŀ��Ϣ_��ʱ����_XMJG_RedLine.XML";
	}
	else
	{

		xml_str = "��Ŀ��Ϣ_��ʱ����.XML";
	}

	TCHAR* szName;
	TCHAR szPath[1000] = {0};

	szName = xml_str.GetBuffer(xml_str.GetLength());
	xml_str.ReleaseBuffer();

	ads_findfile(szName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n�������ļ�ʧ��: %s"), szName);
		return false;
	}

	TiXmlDocument tinyXml;
	char szTemp1[1000] = {0};
	bool es = false;
#ifdef _UNICODE
	TcharToChar(szPath, szTemp1);
	es = tinyXml.LoadFile(szTemp1, TIXML_DEFAULT_ENCODING);
#else
	//XML�ļ���󱣴��UTF-8��ʽ��Ȼ���ڲ��������UTF-8���룬��Ȼ���ȡ��������
	_tcscpy(szTemp1, szPath);
	es = tinyXml.LoadFile(szTemp1, TIXML_ENCODING_UTF8);
#endif

	if(es == false)
	{
		acutPrintf(_T("\n�������ļ�ʧ��: %s"), szName);
		return false;
	}

	TiXmlElement* pRoot = tinyXml.FirstChildElement("Project_Info");
	if(NULL == pRoot)
		return false;

	TiXmlElement *pField = pRoot->FirstChildElement("Field");

	CString strText, strText2;
	CEditListCtrl edtiList;

	for(size_t t = 0; t < m_vFieldName.size(); t++)
	{
		bool bFind = false;
		while(pField)
		{
			strText = edtiList.GetXMLAttribute_EditList(pField, ("name"));
			strText2 = m_vFieldName.at(t);
			if(strText == strText2)
			{
				SetXMLAttribute(pField, _T("value"), m_vValue.at(t));
				bFind = true;
				break;
			}

			pField = pField->NextSiblingElement();
		}


		if(bFind == false)
		{
#ifdef UNICODE
			char pointerNewItem[255] = {0};
			UnicodeToUTF8(_T("Field"), pointerNewItem);
			TiXmlElement pItemNew(pointerNewItem);
#else
			TiXmlElement pItemNew(GBToUTF8(_T("Field")));
#endif

			SetXMLAttribute(&pItemNew, _T("name"), m_vFieldName.at(t));
			SetXMLAttribute(&pItemNew, _T("value"), m_vValue.at(t));

			pRoot->InsertEndChild(pItemNew);
		}

		pField = pRoot->FirstChildElement("Field");
	}

	tinyXml.SaveFile();

	return true;
}

bool CProjectInfoDlg::readPreProjectInfo()
{
	CString xml_str;
	if(project_type == "XMJG_RoadAlignment")
	{
		xml_str = "��Ŀ��Ϣ_��ʱ����_XMJG_RoadAlignment.XML";
	}
	else if(project_type == "XMJG_RedLine")
	{
		xml_str = "��Ŀ��Ϣ_��ʱ����_XMJG_RedLine.XML";
	}
	else if(project_type == "XMJG_BuildSettingOut")
	{
		xml_str = "��Ŀ��Ϣ_��ʱ����_XMJG_BuildSettingOut.XML";
	}
	else
	{
		xml_str = "��Ŀ��Ϣ_��ʱ����.XML";
	}

	TCHAR* szName;
	TCHAR szPath[1000] = {0};

	szName = xml_str.GetBuffer(xml_str.GetLength());
	xml_str.ReleaseBuffer();

	ads_findfile(szName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n�������ļ�ʧ��: %s"), szName);
		return false;
	}

	TiXmlDocument tinyXml;
	char szTemp1[1000] = {0};
	bool es = false;
#ifdef _UNICODE
	TcharToChar(szPath, szTemp1);
	es = tinyXml.LoadFile(szTemp1, TIXML_DEFAULT_ENCODING);
#else
	//XML�ļ���󱣴��UTF-8��ʽ��Ȼ���ڲ��������UTF-8���룬��Ȼ���ȡ��������
	_tcscpy(szTemp1, szPath);
	es = tinyXml.LoadFile(szTemp1, TIXML_ENCODING_UTF8);
#endif

	if(es == false)
	{
		acutPrintf(_T("\n�������ļ�ʧ��: %s"), szName);
		return false;
	}

	TiXmlElement* pRoot = tinyXml.FirstChildElement("Project_Info");
	if(NULL == pRoot)
		return false;

	TiXmlElement *pField = pRoot->FirstChildElement("Field");
	if(pField == NULL) return false;

	CString strText;
	CEditListCtrl editList;
	while(pField)
	{
		strText = editList.GetXMLAttribute_EditList(pField, ("name"));
		m_vFieldName.push_back(strText);
		strText = editList.GetXMLAttribute_EditList(pField, ("value"));
		m_vValue.push_back(strText);
		pField = pField->NextSiblingElement();
	}
	return true;
}

CString	CProjectInfoDlg::findPreProjInfo(CString strFieldName)
{
	CString strText;
	for(size_t t = 0; t < m_vFieldName.size(); t++)
	{
		if(strFieldName == m_vFieldName.at(t))
		{
			strText = m_vValue.at(t);
			break;
		}
	}
	return strText;
}

BOOL CProjectInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	readPreProjectInfo();

	m_vKeys.clear();


	if(project_type == "XMJG_RoadAlignment")
	{
		m_list.Init_ByConfig(_T("��Ŀ��Ϣ�б�ؼ�_XMJG_RoadAlignment.XML"));
	}
	else if(project_type == "XMJG_RedLine")
	{
		m_list.Init_ByConfig(_T("��Ŀ��Ϣ�б�ؼ�_XMJG_RedLine.XML"));
	}
	else if(project_type == "XMJG_BuildSettingOut")
	{
		m_list.Init_ByConfig(_T("��Ŀ��Ϣ�б�ؼ�_XMJG_BuildSettingOut.XML"));
	}
	else
	{
		m_list.Init_ByConfig(_T("��Ŀ��Ϣ�б�ؼ�.XML"));

	}

	CXRecordOperate xrecordO;
	CString strText;
	CArray<ATTRFIELD, ATTRFIELD> *AttrArr = NULL;
	if(project_type == "XMJG_RoadAlignment")
	{
		if(findAttriTable(_T("XMJG3"), AttrArr))
		{
			for(int j = 0; j < AttrArr->GetSize(); j++)
			{
				m_vKeys.push_back(AttrArr->GetAt(j).name);

				xrecordO.ReadStringRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, strText);

				//�����ǰͼ��û�б�������ֶ�ֵ�������һ���������ȡ
				if(strText == _T(""))
					strText = findPreProjInfo(AttrArr->GetAt(j).name);

				m_list.InsertItem(j, AttrArr->GetAt(j).text);
				m_list.SetItemText(j, 1, strText);

			}
		}
	}
	else if(project_type == "XMJG_RedLine")
	{
		if(findAttriTable(_T("XMJG1"), AttrArr))
		{
			for(int j = 0; j < AttrArr->GetSize(); j++)
			{
				m_vKeys.push_back(AttrArr->GetAt(j).name);

				xrecordO.ReadStringRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, strText);

				//�����ǰͼ��û�б�������ֶ�ֵ�������һ���������ȡ
				if(strText == _T(""))
					strText = findPreProjInfo(AttrArr->GetAt(j).name);

				m_list.InsertItem(j, AttrArr->GetAt(j).text);
				m_list.SetItemText(j, 1, strText);

			}
		}
	}
	else if(project_type == "XMJG_BuildSettingOut")
	{
		if(findAttriTable(_T("XMJG2"), AttrArr))
		{
			for(int j = 0; j < AttrArr->GetSize(); j++)
			{
				m_vKeys.push_back(AttrArr->GetAt(j).name);

				xrecordO.ReadStringRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, strText);

				//�����ǰͼ��û�б�������ֶ�ֵ�������һ���������ȡ
				if(strText == _T(""))
					strText = findPreProjInfo(AttrArr->GetAt(j).name);

				m_list.InsertItem(j, AttrArr->GetAt(j).text);
				m_list.SetItemText(j, 1, strText);

			}
		}
	}
	else
	{
		if(findAttriTable(_T("XMXX"), AttrArr))
		{
			for(int j = 0; j < AttrArr->GetSize(); j++)
			{
				m_vKeys.push_back(AttrArr->GetAt(j).name);

				xrecordO.ReadStringRecord(DICT_PROJECTINFO, AttrArr->GetAt(j).name, strText);

				//�����ǰͼ��û�б�������ֶ�ֵ�������һ���������ȡ
				if(strText == _T(""))
					strText = findPreProjInfo(AttrArr->GetAt(j).name);

				m_list.InsertItem(j, AttrArr->GetAt(j).text);
				m_list.SetItemText(j, 1, strText);

			}
		}

	}
	return TRUE;
}

void CProjectInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CProjectInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProjectInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CProjectInfoDlg ��Ϣ�������

void CProjectInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();

	CXRecordOperate xrecordO;
	m_vFieldName.clear();
	m_vValue.clear();
	for(int t = 0; t < (int)m_vKeys.size(); t++)
	{
		xrecordO.AddStringRecord(DICT_PROJECTINFO, m_vKeys.at(t), m_list.GetItemText(t, 1));
		m_vFieldName.push_back(m_vKeys.at(t));
		m_vValue.push_back(m_list.GetItemText(t, 1));
	}

	setPreProjectInfo();
}

// CFloorInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CFloorInfoDlg, CAdUiBaseDialog)

CFloorInfoDlg::CFloorInfoDlg(CWnd* pParent /*=NULL*/)
: CAdUiBaseDialog(CFloorInfoDlg::IDD, pParent)
{
	m_dTextSize = 1.25;
}

CFloorInfoDlg::~CFloorInfoDlg()
{
}

bool	checkProjInfo()
{
	CXRecordOperate xrecordO;
	CString strText, strText2;
	xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("GCMC"), strText);
	xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("XKZBH"), strText2);

	if(strText != _T("") && strText2 != _T("")) return true;

	AfxMessageBox(_T("������ �������� �� ���֤��ţ�"));

	CAcModuleResourceOverride myResources;

	CProjectInfoDlg projDlg;
	projDlg.DoModal();


	return true;
}

BOOL CFloorInfoDlg::OnInitDialog()
{
	checkProjInfo();

	CDialog::OnInitDialog();

	m_list.Init_ByConfig(_T("¥����Ϣͳ���б�ؼ�.XML"));
	m_listText.Init();
	m_listText.InsertColumn(0, _T("ͳ����Ϣ"), LVCFMT_CENTER, 400);

	readRuleFile();

	CString strText;
	for(size_t t = 0; t < m_vRuleName.size(); t++)
	{
		strText += m_vRuleName.at(t);
		strText += _T(";");
	}
	m_list.SetComboColumns(2, strText);


	return TRUE;
}

void CFloorInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATICINFO, m_list);
	DDX_Control(pDX, IDC_LIST_STATIC_TEXT, m_listText);
	DDX_Text(pDX, IDC_EDIT_TEXT_SIZE, m_dTextSize);
}

BEGIN_MESSAGE_MAP(CFloorInfoDlg, CAdUiBaseDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CFloorInfoDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_CACULATE_AREA, &CFloorInfoDlg::OnBnClickedButtonCaculateArea)
	ON_BN_CLICKED(IDC_BUTTON_STATIC, &CFloorInfoDlg::OnBnClickedButtonStatic)
	ON_BN_CLICKED(IDC_BUTTON_NOTE, &CFloorInfoDlg::OnBnClickedButtonNote)
	ON_BN_CLICKED(IDC_BUTTON_POSITION, &CFloorInfoDlg::OnBnClickedButtonPosition)
END_MESSAGE_MAP()

// CFloorInfoDlg ��Ϣ�������

BOOL CFloorInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
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

void CFloorInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialog::OnOK();

	DestroyWindow();
	delete this;

}

void CFloorInfoDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialog::OnCancel();

	DestroyWindow();
	delete this;


}

LRESULT CFloorInfoDlg::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}

bool CFloorInfoDlg::readRuleFile()
{
	TCHAR szPath[1000] = {0};
	TCHAR szName[] = _T("��������������.XML");
	ads_findfile(szName, szPath);
	if(0 == _tcscmp(szPath, _T("")))
	{
		acutPrintf(_T("\n�������ļ�ʧ��: %s"), szName);
		return false;
	}

	TiXmlDocument tinyXml;
	char szTemp1[1000] = {0};
	bool es = false;
#ifdef _UNICODE
	TcharToChar(szPath, szTemp1);
	es = tinyXml.LoadFile(szTemp1, TIXML_DEFAULT_ENCODING);
#else
	//XML�ļ���󱣴��UTF-8��ʽ��Ȼ���ڲ��������UTF-8���룬��Ȼ���ȡ��������
	_tcscpy(szTemp1, szPath);
	es = tinyXml.LoadFile(szTemp1, TIXML_ENCODING_UTF8);
#endif

	if(es == false)
	{
		acutPrintf(_T("\n�������ļ�ʧ��: %s"), szName);
		return false;
	}

	m_vRuleName.clear();
	m_vParam.clear();

	TiXmlElement* pRoot = tinyXml.FirstChildElement("Building_Rule");
	if(NULL == pRoot)
		return false;

	TiXmlElement *pAreaRule = pRoot->FirstChildElement("Area_Rule");
	if(pAreaRule == NULL) return false;

	TiXmlElement *pRule = pAreaRule->FirstChildElement("rule");

	while(pRule)
	{
		//����
		CString strName = m_list.GetXMLAttribute_EditList(pRule, ("name"));
		//�п�
		double dValue = _tstof(m_list.GetXMLAttribute_EditList(pRule, ("param")));

		m_vRuleName.push_back(strName);
		m_vParam.push_back(dValue);

		pRule = pRule->NextSiblingElement();
	}


	return true;
}

bool	CFloorInfoDlg::checkEntity(int nPos, AcDbObjectId objId)
{


	if(CEntOper::ifPolyClosed(objId) == false)
	{
		AGrid_AppendMsgEnt(_T("ͳ��¥����Ϣ"), _T("ͼ�β���գ�"), _T("����"), objId);
		m_nErrorCount++;
	}

	//�������⽨�������ý�������������Լ��
	//TCHAR szCode[200] = {0};
	//if(0 == _tcscmp(szCode, _T("410212"))) return true;

	CString strText = _T("���Դ���");

	double dParam = 0.0;
	ReadXdataDouble(aname(objId), _T("MJXS"), dParam);
	if(dParam <= 0.0)
		strText += _T("���ϵ������");

	short sTop = 0, sUnder = 0, sCBJ = 0;
	ReadXdataShort(aname(objId), _T("ZZLC"), sTop);
	ReadXdataShort(aname(objId), _T("QSLC"), sUnder);
	ReadXdataShort(aname(objId), _T("CBJ"), sCBJ);
	if(sTop == 0 || sUnder == 0)
	{
		AGrid_AppendMsgEnt(_T("ͳ��¥����Ϣ"), _T("<��ʼ¥��>��<��ֹ¥��>���Զ�Ϊ0����"), _T("����"), objId);
		m_nErrorCount++;
	}
	if(sTop < 0 || sUnder < 0)
	{
		if(sCBJ == 0)
		{
			AGrid_AppendMsgEnt(_T("ͳ��¥����Ϣ"), _T("<�Ƿ���²�>������д����"), _T("����"), objId);
			m_nErrorCount++;
		}
	}
	if(sCBJ > 0)
	{
		if(sTop > 0 || sUnder > 0)
		{
			AGrid_AppendMsgEnt(_T("ͳ��¥����Ϣ"), _T("<��ʼ¥��>��<��ֹ¥��>������д����"), _T("����"), objId);
			m_nErrorCount++;
		}
	}

	if(sTop == 0 || sUnder == 0 || sUnder > sTop)
		strText += _T("��ʼ����ֹ¥��������");

	if(strText == _T("���Դ���"))
		return true;
	else
	{
		m_list.InsertItem(nPos, strText);
		return false;
	}

}

void	CFloorInfoDlg::getBuildName_Floor()
{
	if(m_objArr.length() == 0) return;

	set<CString> setName;
	vector<short> vFloor;
	for(int i = 0; i < m_objArr.length(); i++)
	{
		TCHAR szName[500] = {0};
		ReadXdata(aname(m_objArr.at(i)), _T("JZMC"), STRING_TYPE, szName);
		if(0 != _tcscmp(szName, _T("")))
		{
			setName.insert(szName);
		}

		short sTop = 0, sUnder = 0;
		ReadXdataShort(aname(m_objArr.at(i)), _T("QSLC"), sUnder);
		ReadXdataShort(aname(m_objArr.at(i)), _T("ZZLC"), sTop);
		vFloor.push_back(sUnder);
		vFloor.push_back(sTop);
	}
	sort(vFloor.begin(), vFloor.end());

	CString strText;
	for(set<CString>::const_iterator it = setName.begin(); it != setName.end(); it++)
		strText += *it;

	GetDlgItem(IDC_STATIC_BUILDING_NO)->SetWindowText(strText);
	strText.Format(_T("%d �㵽 %d��"), *vFloor.begin(), *(vFloor.end() - 1));
	GetDlgItem(IDC_STATIC_FLOOR_INFO)->SetWindowText(strText);


}

bool  CFloorInfoDlg::filterDivideEntitys()
{
	//���˳����⽨����
	m_objArrSpecail.append(m_objArr);
	SFilter filter;
	filter.filterObjectIdArray(m_objArrSpecail, _T("SOUTH"), _T("410212"));

	if(m_objArrSpecail.length() == 0) return false;

	if(IDYES == AfxMessageBox(_T("�����м������ϰ��������������Ƿ�Ϊ��ָ������������(��������Ҳ�������ϰ���¼���)"), MB_YESNO))
	{
		BeginEditorCommand();

		AcDbObjectIdArray objArr;
		CSelect selectT;
		selectT.SelectObjects2(objArr, 2, _T("*LINE*"));
		m_objArrSpecail.append(objArr);
		RemoveSameObjects(m_objArrSpecail);

		CompleteEditorCommand();

	}
	else
	{
		checkBuildInDivideBuild();
	}

	RemoveSameObjects(m_objArr, m_objArrSpecail);

	return true;
}

bool  CFloorInfoDlg::checkBuildInDivideBuild()
{
	for(int i = 0; i < m_objArrSpecail.length(); i++)
	{
		AcGePoint2dArray nodes;
		GetPlList(aname(m_objArrSpecail.at(i)), nodes);
		AcDbObjectIdArray objArr;
		ssFromNodes(objArr, nodes, 1, 0.1, _T("*LINE*"));
		if(objArr.length() > 0)
		{
			AfxMessageBox(_T("����һ������ϰ���½����ڲ���ʵ��û�������ƽ�֣�������Ǽ��뵽����ϰ�������ƽ�֣�"));
			return false;
		}
	}
	return true;
}

void CFloorInfoDlg::OnBnClickedButtonSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_nErrorCount = 0;
	AGrid_ClearSheet(_T("ͳ��¥����Ϣ"));

	BeginEditorCommand();
	acedSSSetFirst(NULL, NULL);

	//AcDbObjectIdArray objArr;
	m_objArr.setLogicalLength(0);
	CSelect select;
	select.SelectObjects2(m_objArr, 2, _T("*LINE*"), _T(""), _T(""));

	CompleteEditorCommand();

	SFilter filter;
	filter.filterObjectIdArray(m_objArr, _T("SOUTH"), _T(""), STRING_TYPE, 0, 21);

	m_list.DeleteAllItems();
	m_listText.DeleteAllItems();

	getBuildName_Floor();

	filterDivideEntitys();

	CString strText;
	for(int i = 0; i < m_objArr.length(); i++)
	{
		ads_name ent;
		if(Acad::eOk != acdbGetAdsName(ent, m_objArr.at(i))) continue;

		if(false == checkEntity(i, m_objArr.at(i))) continue;

		TCHAR szStdm[100] = {0};
		TCHAR szName[500] = {0};
		GetStdm(ent, szStdm);
		g_index.get_name(szStdm, szName);
		strText = szName;
		strText.Replace(_T("���ͼ"), _T(""));
		m_list.InsertItem(i, strText);

		int dValue = 0;
		short sTop = 0, sUnder = 0;
		ReadXdataShort(ent, _T("QSLC"), sUnder);
		ReadXdataShort(ent, _T("ZZLC"), sTop);
		dValue = sTop - sUnder;
		dValue++;
		_stprintf(szName, _T("%d"), dValue);
		m_list.SetItemText(i, 1, szName);

		m_list.SetItemText(i, 2, _T("Ĭ��"));

		double dParam = 0.0;
		ReadXdataDouble(ent, _T("MJXS"), dParam);
		_stprintf(szName, _T("%.2lf"), dParam);
		m_list.SetItemText(i, 3, szName);

		double dArea = 0.0;
		GetEntArea(ent, dArea);
		short sSFKC = 0;
		ReadXdataShort(ent, _T("SFKC"), sSFKC);
		if(sSFKC == 1) dArea *= -1;
		_stprintf(szName, _T("%.4lf"), dArea);
		m_list.SetItemText(i, 4, szName);

		double dArea2 = dArea * dParam;
		_stprintf(szName, _T("%.4lf"), dArea2);
		m_list.SetItemText(i, 5, szName);
	}

	/*
	for(int i = 0; i < m_objArrSpecail.length(); i ++)
	{
	TCHAR szName[500] = {0};
	g_index.get_name(_T("410212"), szName);
	strText = szName;
	strText.Replace(_T("���ͼ"), _T(""));

	int nCount = m_list.GetItemCount();
	m_list.InsertItem(nCount, strText);
	m_list.SetItemText(nCount, 1, _T("1"));
	m_list.SetItemText(nCount, 2, _T("Ĭ��"));
	m_list.SetItemText(nCount, 3, _T("1.00"));

	ads_name ent;
	if(Acad::eOk != acdbGetAdsName(ent, m_objArrSpecail.at(i))) continue;

	double dArea = 0.0;
	GetEntArea(ent, dArea);
	dArea /= 2.0;
	dArea -= 0.005;
	_stprintf(szName, _T("%.4lf"), dArea);
	m_list.SetItemText(nCount, 4, szName);
	m_list.SetItemText(nCount, 5, szName);

	nCount = m_list.GetItemCount();
	m_list.InsertItem(nCount, strText);
	m_list.SetItemText(nCount, 1, _T("1"));
	m_list.SetItemText(nCount, 2, _T("Ĭ��"));
	m_list.SetItemText(nCount, 3, _T("1.00"));

	GetEntArea(ent, dArea);
	dArea /= 2.0;
	dArea += 0.005;
	_stprintf(szName, _T("%.4lf"), dArea);
	m_list.SetItemText(nCount, 4, szName);
	m_list.SetItemText(nCount, 5, szName);
	}
	*/


	if(m_nErrorCount > 0) AGrid_ShowMe();
}

BOOL CFloorInfoDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if(wParam == IDC_LIST_STATICINFO)        //����Ƿ��б�ؼ�ID
	{
		NMHDR *pNMHDR = (NMHDR *)lParam;

		if(pNMHDR->code == LVN_ENDLABELEDIT)    //����Ƿ��ǵ�Ԫ����ɱ༭���¼�
		{
			//���ﴦ��Ԫ����ɱ༭��Ĳ���

			LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
			LV_ITEM *plvItem = &plvDispInfo->item;


			if(plvItem->iSubItem == 2)
			{
				CString strText = plvItem->pszText;
				int nIndex = -1;
				for(int t = 0; t < (int)m_vRuleName.size(); t++)
				{
					if(m_vRuleName.at(t) == strText)
					{
						nIndex = t;
						break;
					}
				}

				if(nIndex != -1)
				{
					strText.Format(_T("%.2lf"), m_vParam.at(nIndex));
					m_list.SetItemText(plvItem->iItem, plvItem->iSubItem + 1, strText);
				}

			}

			if(plvItem->pszText != NULL && plvItem->iItem != -1)
			{
				m_list.SetItemText(plvDispInfo->item.iItem, plvDispInfo->item.iSubItem,
					plvDispInfo->item.pszText);//�޸�Ϊ����ͬʱ�޸Ķ��
				for(int i = 0; i < m_list.GetItemCount(); i++)
				{
					if(m_list.GetItemState(i, LVIS_SELECTED))
					{
						m_list.SetItemText(i, plvDispInfo->item.iSubItem,
							plvDispInfo->item.pszText);
					}
				}
			}

		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CFloorInfoDlg::OnBnClickedButtonCaculateArea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText;
	for(int i = 0; i < m_list.GetItemCount(); i++)
	{
		double dParam = _tstof(m_list.GetItemText(i, 3));
		double dArea = _tstof(m_list.GetItemText(i, 4));
		strText.Format(_T("%.4lf"), dParam * dArea);

		m_list.SetItemText(i, 5, strText);

	}

}

void	CFloorInfoDlg::clearData()
{
	m_FloorStatic.dBuildArea = 0.0;
	m_FloorStatic.dBuildAreaAll = 0.0;
	m_FloorStatic.dFloorNum = 0;
	m_FloorStatic.dParam = 0.0;
	m_FloorStatic.vAttach.clear();
	m_FloorStatic.vMainBuild.clear();
	m_vStaticInfo.clear();

	m_FloorStatic.dAreaUnder_Private = 0.0;
	m_FloorStatic.dAreaUnder_Public = 0.0;
	m_FloorStatic.dAreaUp_Private = 0.0;
	m_FloorStatic.dAreaUp_Public = 0.0;

	m_FloorStatic.strArea = _T("");
	m_FloorStatic.strArea2 = _T("");
	m_FloorStatic.strAreaAll = _T("");
}

bool	CFloorInfoDlg::staticOrignData()
{

	clearData();

	//�������׷��ʱ��
	for(int i = 0; i < m_objArr.length(); i++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(m_objArr.at(i)), szStdm);
		short sUnder = 0, sJZXZ = 0;
		ReadXdataShort(aname(m_objArr.at(i)), _T("CBJ"), sUnder);
		ReadXdataShort(aname(m_objArr.at(i)), _T("JZXZ"), sJZXZ);

		short dTop = 0, dButton = 0;
		ReadXdataShort(aname(m_objArr.at(i)), _T("ZZLC"), dTop);
		ReadXdataShort(aname(m_objArr.at(i)), _T("QSLC"), dButton);
		int nFloorNum = (dTop - dButton) + 1;
		CString strName = m_list.GetItemText(i, 0);
		double  dParam = _tstof(m_list.GetItemText(i, 3));
		double dArea = 0.0;
		GetEntArea(aname(m_objArr.at(i)), dArea);

		short sSFKC = 0;
		ReadXdataShort(aname(m_objArr.at(i)), _T("SFKC"), sSFKC);
		if(sSFKC) dArea *= -1;

		m_FloorStatic.dFloorNum = nFloorNum;
		m_FloorStatic.sTopNum = dTop;
		m_FloorStatic.sButtomNum = dButton;

		//׷�����彨�������Ͻ��������½��������
		if(0 == _tcscmp(szStdm, _T("410102")) || 0 == _tcscmp(szStdm, _T("410202")))
		{

			MainBuild mBuild;
			mBuild.dArea = dArea;
			mBuild.dParam = dParam;
			m_FloorStatic.vMainBuild.push_back(mBuild);
			dArea *= dParam;
			m_FloorStatic.strBuildName = strName;
			m_FloorStatic.dParam = dParam;
			m_FloorStatic.dBuildArea += dArea;
			m_FloorStatic.dBuildAreaAll += dArea;

			//׷�ӵ��ϣ����������
			dArea = _tstof(m_list.GetItemText(i, 4)) * dParam;
			dArea *= nFloorNum;
			// 			if(sUnder == 1)
			// 			{
			// 				if(sJZXZ == 0)
			// 					m_FloorStatic.dAreaUnder_Private += dArea;
			// 				else
			// 					m_FloorStatic.dAreaUnder_Public += dArea;
			// 			}
			// 			else
			// 			{
			// 				if(sJZXZ == 0)
			// 					m_FloorStatic.dAreaUp_Private += dArea;
			// 				else
			// 					m_FloorStatic.dAreaUp_Public += dArea;
			// 			}

		}
		else		//׷�Ӹ����������
		{
			bool bFind = false;

			double dArea2 = dParam * dArea;
			m_FloorStatic.dBuildAreaAll += dArea2;

			//׷�ӵ��ϣ����������
			// 			double dArea3 = dParam * dArea2;
			// 			dArea3 *= nFloorNum;
			// 			if(sUnder == 1)
			// 			{
			// 				if(sJZXZ == 0)
			// 					m_FloorStatic.dAreaUnder_Private += dArea3;
			// 				else
			// 					m_FloorStatic.dAreaUnder_Public += dArea3;
			// 			}
			// 			else
			// 			{
			// 				if(sJZXZ == 0)
			// 					m_FloorStatic.dAreaUp_Private += dArea3;
			// 				else
			// 					m_FloorStatic.dAreaUp_Public += dArea3;
			// 			}

			for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
			{
				if(strName == m_FloorStatic.vAttach.at(t).strName &&
					fabs(dParam - m_FloorStatic.vAttach.at(t).dParam) <= 0.001)
				{
					m_FloorStatic.vAttach.at(t).dArea += dArea;
					m_FloorStatic.vAttach.at(t).dArea2 += dArea2;
					bFind = true;
					break;
				}
			}

			if(bFind == false)
			{
				AttachArea attachA;
				attachA.strName = strName;
				attachA.dParam = dParam;
				attachA.dArea = dArea;
				attachA.dArea2 = dArea * dParam;
				m_FloorStatic.vAttach.push_back(attachA);
			}

		}

	}

	return true;
}

bool	CFloorInfoDlg::staticPrecData()
{
	//���ܸ��������
	CString strText, strText2;
	for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
	{
		double  dParam = m_FloorStatic.vAttach.at(t).dParam;
		double dArea = m_FloorStatic.vAttach.at(t).dArea;

		if(dParam == 1.0)
		{
			m_FloorStatic.vAttach.at(t).strArea = format_double_2(dArea, 2);
			m_FloorStatic.vAttach.at(t).strArea2 = format_double_2(dArea, 2);
		}
		else
		{
			//�����������������룬�ٳ���ϵ�����������������
			CString str1 = format_double_2(dArea, 2);
			m_FloorStatic.vAttach.at(t).strArea = str1;
			double d1 = _tstof(str1) * dParam;
			CString str2 = format_double_2(d1, 2);
			m_FloorStatic.vAttach.at(t).strArea2 = str2;
		}
	}
	//���½��������Ϊ���۳��Ĳ��֣�Ҳ�г���
	//�н������壬�����ڵ��£������³���۳���
	// 	if(m_FloorStatic.dBuildArea < 0.0)
	// 	{
	// 		double  dParam = m_FloorStatic.dParam;
	// 		double dArea = m_FloorStatic.dBuildArea;
	// 
	// 		if(dParam == 1.0)
	// 		{
	// 			//strText.Format(_T("����%s�������:%.2lf"), strName, dArea);
	// 		}
	// 		else
	// 		{
	// 			//���������룬�ٳ���ϵ�����������������
	// 			CString str1 = format_double_2(dArea, 2);
	// 			double d1 = _tstof(str1) * dParam;
	// 			CString str2 = format_double_2(d1, 2);
	// 			strText.Format(_T("����%s�������:%s * %.2lf = %s"), strName, str1, dParam, str2);
	// 		}
	// 
	// 	}

	//�г�һ�㽨�������
	//���н������壬���Ҵ���0ʱ
	//if(m_FloorStatic.dBuildArea > 0.0)
	if(m_FloorStatic.vMainBuild.size())
	{
		//��ͳ�Ƶ��������������
		double d1 = 0.0;
		for(size_t tt = 0; tt < m_FloorStatic.vMainBuild.size(); tt++)
		{
			double d2 = m_FloorStatic.vMainBuild.at(tt).dArea;
			d2 *= m_FloorStatic.vMainBuild.at(tt).dParam;
			d1 += d2;
		}
		CString str1 = format_double_2(d1, 2);
		m_FloorStatic.strArea = str1;
		m_FloorStatic.strArea2 = str1;

		//��׷�Ӹ��������(ȡ����ģ����ҳ������ϵ�����)
		if(m_FloorStatic.vAttach.size())		//�и������ͳ��
		{
			double d1 = 0.0;
			for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
			{
				d1 += _tstof(m_FloorStatic.vAttach.at(t).strArea2);
			}
			//����׷�ӵ������������
			CString str1 = format_double_2(d1, 2);
			d1 = _tstof(m_FloorStatic.strArea) + _tstof(str1);
			str1 = format_double_2(d1, 2);
			m_FloorStatic.strArea2 = str1;
		}
		else		//û�и������ͳ��
		{

			//			else
			{
				// 				if(m_FloorStatic.sButtomNum > 0)
				// 				{
				// 					double d1 = 0.0;
				// 					for(size_t tt = 0; tt < m_FloorStatic.vMainBuild.size(); tt ++)
				// 					{
				// 						double d2 = m_FloorStatic.vMainBuild.at(tt).dArea;
				// 						d2 *= m_FloorStatic.vMainBuild.at(tt).dParam;
				// 						d1 += d2;
				// 					}
				// 					CString str1 = format_double(d1, 3);
				// 					CString str2 = format_double(str1, 2);
				// 					strText.Format(_T("%s�㽨�����:%s"), LowerToUper(m_FloorStatic.sButtomNum), str2);
				// 				}
			}
		}
	}
	else	//���û�����彨�����Ǿ�ֱ�ӰѸ���׷�ӵ�һ���������
	{

		for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
		{
			double d1 = 0.0;
			for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
			{
				d1 += _tstof(m_FloorStatic.vAttach.at(t).strArea2);
			}
			m_FloorStatic.strArea2 = format_double_2(d1, 2);
		}
	}

	//ͳ��ȫ��¥��
	if(m_FloorStatic.dFloorNum > 1)//���
	{
		//����Ҫע����������׷��
		double d1 = _tstof(m_FloorStatic.strArea2) * m_FloorStatic.dFloorNum;
		CString str1 = format_double_2(d1, 2);
		m_FloorStatic.strAreaAll = str1;

	}
	else		//����
	{
		m_FloorStatic.strAreaAll = m_FloorStatic.strArea2;
	}

	//��������������������ó����ϣ����������
	if(m_FloorStatic.sButtomNum > 0)
	{
		m_FloorStatic.dAreaUp_Private = _tstof(m_FloorStatic.strAreaAll);
	}
	else
	{
		m_FloorStatic.dAreaUnder_Private = _tstof(m_FloorStatic.strAreaAll);
	}

	return true;
}

bool	CFloorInfoDlg::staticDivideAreaText()
{
	if(m_objArrSpecail.length() == 0) return false;

	m_vStaticInfo.clear();

	CString strText;
	double dDivideArea = 0.0;
	for(int i = 0; i < m_objArrSpecail.length(); i++)
	{
		ads_name ent;
		if(Acad::eOk != acdbGetAdsName(ent, m_objArrSpecail.at(i))) continue;

		double dArea = 0.0;
		GetEntArea(ent, dArea);
		double dParam = 1.0;
		ReadXdataDouble(ent, _T("MJXS"), dParam);
		dArea *= dParam;

		short sSFKC = 0;
		ReadXdataShort(ent, _T("SFKC"), sSFKC);
		if(sSFKC) dArea *= -1;

		dDivideArea += dArea;

		TCHAR szStdm[100] = {0};
		TCHAR szName[500] = {0};
		GetStdm(ent, szStdm);
		g_index.get_name(szStdm, szName);
		CString str = szName;
		str.Replace(_T("���ͼ"), _T(""));

		strText.Format(_T("����%s���: %.2lf"), str, dArea);

		m_vStaticInfo.push_back(strText);
	}

	//����������ֳ����ݣ��ж��Ƿ���˫��������ǣ���ƽ�֣�������ǣ����������ȵ��������0.01ƽ������׷�ӵ�����
	//���ϵ��������
	CString strArea = format_double_2(dDivideArea, 2);
	double dArea1 = 0.0, dArea2 = 0.0;
	if((int)(_tstof(strArea) * 100) % 2 == 0)
	{
		dArea1 = _tstof(strArea) / 2.0;
		dArea2 = _tstof(strArea) / 2.0;

		m_FloorStatic.dAreaUnder_Private += dArea1;
		m_FloorStatic.dAreaUp_Private += dArea2;
	}
	else
	{
		dArea1 = _tstof(strArea) / 2.0 + 0.005;
		dArea2 = _tstof(strArea) / 2.0 - 0.005;

		m_FloorStatic.dAreaUnder_Private += dArea1;
		m_FloorStatic.dAreaUp_Private += dArea2;
	}

	strText.Format(_T("                           ������½������:%.2lf"), dArea1);
	m_vStaticInfo.push_back(strText);
	strText.Format(_T("���չ滮���Ҫ��ò������Ͻ������:%.2lf"), dArea2);
	m_vStaticInfo.push_back(strText);

	for(int t = (int)m_vStaticInfo.size() - 1; t >= 0; t--)
	{
		int nCount = m_listText.GetItemCount();
		//nCount --;
		m_listText.InsertItem(nCount, m_vStaticInfo.at(t));
	}

	return true;
}

bool	CFloorInfoDlg::staticTextData()
{

	//�г����������
	CString strText, strText2;
	for(int t = 0; t < (int)m_FloorStatic.vAttach.size(); t++)
	{
		CString strName = m_FloorStatic.vAttach.at(t).strName;
		double dParam = m_FloorStatic.vAttach.at(t).dParam;

		if(dParam == 1.0)
		{
			//����������ڣ���ÿ��������Ҫ�Ӹ������С���
			if(m_FloorStatic.vMainBuild.size() > 0)
				strText.Format(_T("����%s�������:%s"), strName, m_FloorStatic.vAttach.at(t).strArea);
			else
			{
				//���û�����壬���ֻ��1����������Ҫ�����С�����Ȼȫ������ӡ����С�
				if(m_FloorStatic.vAttach.size() > 1)
					strText.Format(_T("����%s�������:%s"), strName, m_FloorStatic.vAttach.at(t).strArea);
				else
					strText.Format(_T("%s�������:%s"), strName, m_FloorStatic.vAttach.at(t).strArea);
			}
		}
		else
		{
			strText.Format(_T("����%s�������:%s * %.2lf = %s"),
				strName, m_FloorStatic.vAttach.at(t).strArea, dParam, m_FloorStatic.vAttach.at(t).strArea2);
		}

		m_vStaticInfo.push_back(strText);
	}
	//���½��������Ϊ���۳��Ĳ��֣�Ҳ�г���
	//�н������壬�����ڵ��£������³���۳���
	if(m_FloorStatic.dBuildArea < 0.0)
	{
		// 		CString strName = m_FloorStatic.strBuildName;
		// 		double  dParam = m_FloorStatic.dParam;
		// 		double dArea = m_FloorStatic.dBuildArea;
		// 
		// 		if(dParam == 1.0)
		// 			strText.Format(_T("����%s�������:%.2lf"), strName, dArea);
		// 		else
		// 		{
		// 			//���������룬�ٳ���ϵ�����������������
		// 			CString str1 = format_double_2(dArea, 2);
		// 			double d1 = _tstof(str1) * dParam;
		// 			CString str2 = format_double_2(d1, 2);
		// 			strText.Format(_T("����%s�������:%s * %.2lf = %s"), strName, str1, dParam, str2);
		// 		}
		// 
		// 		m_vStaticInfo.push_back(strText);
	}

	//�г�һ�㽨�������
	if(m_FloorStatic.vMainBuild.size())
	{
		if(m_FloorStatic.vAttach.size())		//�������� + ����������
		{
			if(m_FloorStatic.dFloorNum > 1)
				strText.Format(_T("����ÿ�㽨�����:%s"), m_FloorStatic.strArea);
			else
			{
				if(m_FloorStatic.sButtomNum > 0)
					strText.Format(_T("%s�㽨�����:%s"), LowerToUper(m_FloorStatic.sButtomNum), m_FloorStatic.strArea);
				else
					strText.Format(_T("����%s�㽨�����:%s"), LowerToUper(-m_FloorStatic.sButtomNum), m_FloorStatic.strArea);
			}
			for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
			{
				if(m_FloorStatic.vAttach.at(t).dArea >= 0)
					strText += _T("+");

				strText += m_FloorStatic.vAttach.at(t).strArea2;
			}
			strText2.Format(_T(" = %s"), m_FloorStatic.strArea2);
			strText += strText2;
		}
		else		//ֻ�н��������ͳ��
		{
			if(m_FloorStatic.dFloorNum > 1)
			{
				strText.Format(_T("ÿ�㽨�����:%s"), m_FloorStatic.strArea);
			}
			else
			{
				if(m_FloorStatic.sButtomNum > 0)
				{
					strText.Format(_T("%s�㽨�����:%s"), LowerToUper(m_FloorStatic.sButtomNum), m_FloorStatic.strArea);
				}
				else
					strText.Format(_T("����%s�㽨�����:%s"), LowerToUper(-m_FloorStatic.sButtomNum), m_FloorStatic.strArea);
			}
		}
		m_vStaticInfo.push_back(strText);
	}
	else		//ֻ�и������ͳ��
	{
		short sButtom = m_FloorStatic.sButtomNum;
		if(m_FloorStatic.vAttach.size() > 1)
		{

			strText.Format(_T("���н������:"));
			for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
			{
				strText += m_FloorStatic.vAttach.at(t).strArea2;
				if(t != m_FloorStatic.vAttach.size() - 1)
					strText += _T("+");
			}
			strText2.Format(_T(" = %s"), m_FloorStatic.strArea2);
			strText += strText2;

			m_vStaticInfo.push_back(strText);
		}

	}

	//ͳ��ȫ��¥��
	if(m_FloorStatic.dFloorNum > 1)		//�������� + ����������
	{
		short sButtom = m_FloorStatic.sButtomNum;
		short sTop = m_FloorStatic.sTopNum;
		if(sButtom < 0 && sTop > 0)
		{
			strText.Format(_T("����%s����%s�㽨�����:%s * %d = %s"), LowerToUper(-sButtom), LowerToUper(sTop), m_FloorStatic.strArea2,
				m_FloorStatic.dFloorNum, m_FloorStatic.strAreaAll);
		}
		else if(sButtom < 0 && sTop < 0)
		{
			strText.Format(_T("����%s��������%s�㽨�����:%s * %d = %s"), LowerToUper(-sButtom), LowerToUper(-sTop), m_FloorStatic.strArea2,
				m_FloorStatic.dFloorNum, m_FloorStatic.strAreaAll);
		}
		else
		{
			strText.Format(_T("%s����%s�㽨�����:%s * %d = %s"), LowerToUper(sButtom), LowerToUper(sTop), m_FloorStatic.strArea2,
				m_FloorStatic.dFloorNum, m_FloorStatic.strAreaAll);
		}

		m_vStaticInfo.push_back(strText);
	}
	else
	{
		//ֻ�и���������
		if(m_FloorStatic.vMainBuild.size() == 0 && m_FloorStatic.vAttach.size() > 0)
		{
			short sButtom = m_FloorStatic.sButtomNum;
			if(m_FloorStatic.vAttach.size() == 1)			//ֻ��һ��������
			{
				if(sButtom > 0)		//���ϸ�����
				{
				}
				else				//���¸�����
				{
				}
			}
			else														//���������
			{
				if(sButtom > 0)		//���ϸ�����
				{
				}
				else				//���¸�����
				{
					strText.Format(_T("����%s�㽨�����:"), LowerToUper(-sButtom));
					for(size_t t = 0; t < m_FloorStatic.vAttach.size(); t++)
					{
						if(m_FloorStatic.vAttach.at(t).dArea >= 0)
							strText += _T("+");
						strText += m_FloorStatic.vAttach.at(t).strArea2;
					}
					strText2.Format(_T(" = %s"), m_FloorStatic.strArea2);
					strText += strText2;

					m_vStaticInfo.push_back(strText);

				}

			}

		}


	}



	int nCount = 0;
	m_listText.DeleteAllItems();
	for(int t = (int)m_vStaticInfo.size() - 1; t >= 0; t--, nCount++)
	{
		m_listText.InsertItem(nCount, m_vStaticInfo.at(t));
	}

	return true;
}

void CFloorInfoDlg::OnBnClickedButtonStatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	staticOrignData();

	staticPrecData();

	staticTextData();

	staticDivideAreaText();

	UpdateData(FALSE);
}

void CFloorInfoDlg::OnBnClickedButtonNote()
{
	if(m_dTextSize <= 0.0)
	{
		AfxMessageBox(_T("��������ȷ��ע�Ǵ�С��"));
		return;
	}

	UpdateData(TRUE);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BeginEditorCommand();
	acDocManager->lockDocument(curDoc()); // �����ĵ�

	ads_point pt;
	if(RTCAN == ads_getpoint(NULL, _T("\n����ע��λ��\n"), pt))
	{
		acDocManager->unlockDocument(curDoc());
		CancelEditorCommand();
		return;
	}

	CString strText;
	bool bMark = false;
	AcDbObjectIdArray objArrGroup;
	double dScale = m_dTextSize / 1.25;
	AcDbObjectId objMarked;
	//����ע�����
	for(int i = 0; i < m_listText.GetItemCount(); i++)
	{
		strText = m_listText.GetItemText(i, 0);
		TrimString_Space(strText);
		if(strText == _T("")) continue;

		AcDbObjectId objBlock = DrawInsert(pt, _T("�ֲ�ͼ_��Ϣע��"), dScale, dScale);
		if(objBlock == AcDbObjectId::kNull)
		{
			acutPrintf(_T("\n �����ʧ�ܣ��ֲ�ͼ_��Ϣע��.dwg"));
			acDocManager->unlockDocument(curDoc());
			CancelEditorCommand();
			return;
		}

		SBreakBlock breakB;
		breakB.SetBlock(objBlock);
		breakB.Explode();
		objArrGroup.append(breakB.m_objIdArr);
		if(breakB.m_objIdArr.length())
		{
			double dHeight = GetTextHeight(aname(breakB.m_objIdArr.first()));
			breakB.replaceText(_T("#STATIC_INFO#"), strText);
			pt[Y] -= dHeight * 1.5;
		}

		if(bMark == false)
		{
			objMarked = breakB.m_objIdArr.first();
			markSummaryArea(objMarked);
			bMark = true;
		}
	}

	/*
	if(m_objArrSpecail.length())
	{
	AcDbObjectId objBlock = DrawInsert(pt, _T("�ֲ�ͼ_��Ϣע��_������ϵ���"), dScale, dScale);
	if(objBlock == AcDbObjectId::kNull)
	{
	acutPrintf(_T("\n �����ʧ�ܣ��ֲ�ͼ_��Ϣע��.dwg"));
	acDocManager->unlockDocument(curDoc());
	CancelEditorCommand();
	return;
	}

	double dDivideArea = 0.0;
	for(int i = 0; i < m_objArrSpecail.length(); i ++)
	{
	double dArea = 0.0;
	GetEntArea(aname(m_objArrSpecail.at(i)), dArea);
	double dParam = 1.0;
	ReadXdataDouble(aname(m_objArrSpecail.at(i)), _T("MJXS"), dParam);
	dArea *= dParam;

	dDivideArea += dArea;
	}

	//����������ֳ����ݣ��ж��Ƿ���˫��������ǣ���ƽ�֣�������ǣ����������ȵ��������0.01ƽ������׷�ӵ�����
	//���ϵ��������
	CString strArea = format_double_2(dDivideArea, 2);
	double dArea1 = 0.0, dArea2 = 0.0;
	if((int)(_tstof(strArea) * 100) % 2 == 0)
	{
	dArea1 = _tstof(strArea) / 2.0;
	dArea2 = _tstof(strArea) / 2.0;

	m_FloorStatic.dAreaUnder_Private += dArea1;
	m_FloorStatic.dAreaUp_Private += dArea2;
	}
	else
	{
	dArea1 = _tstof(strArea) / 2.0 + 0.005;
	dArea2 = _tstof(strArea) / 2.0 - 0.005;

	m_FloorStatic.dAreaUnder_Private +=  dArea1;
	m_FloorStatic.dAreaUp_Private +=  dArea2;
	}

	SBreakBlock breakB;
	breakB.SetBlock(objBlock);
	breakB.Explode();
	objArrGroup.append(breakB.m_objIdArr);
	if(breakB.m_objIdArr.length())
	{
	strText.Format(_T("%.2lf"), dArea2);
	breakB.replaceText(_T("#AREA_UP#"), strText);
	strText.Format(_T("%.2lf"), dArea1);
	breakB.replaceText(_T("#AREA_UNDER#"), strText);
	}

	//������������ע�����û��ִ��(���û�ֻ��ѡ�˰���ϰ���½�����)�����������»�ȡ����ǵ�ע�ǣ����±�������Ϣ
	if(bMark == false && objMarked == AcDbObjectId::kNull)
	objMarked = breakB.m_objIdArr.first();

	//���¸���ע���ڱ���������Ϣ
	markSummaryArea(objMarked);

	}
	*/

	//��ע�ǣ�ͳ�Ƶ��������ͬһ�������
	//���ұ�����ͳ�ƹ����
	ads_name ss;
	IdArrToSS(m_objArr, ss);
	AddXdata_bat(ss, XDATA_CALCULATE_AREA, INT_TYPE, _T("1"));
	ads_ssfree(ss);
	objArrGroup.append(m_objArr);
	BuildGroup(objArrGroup, newGUID2());

	acDocManager->unlockDocument(curDoc());

	CompleteEditorCommand();
}

bool	CFloorInfoDlg::markSummaryArea(AcDbObjectId objId)
{
	RemoveXdata(aname(objId), _T("FloorSummary"));
	//�˴���ʱ��˳��д����ʱҲ��˳���
	CString strText;
	strText.Format(_T("%.5lf"), m_FloorStatic.dAreaUnder_Private);				//����˽��
	ADDMulXData(objId, _T("FloorSummary"), strText, DOUBLE_TYPE, 0);
	strText.Format(_T("%.5lf"), m_FloorStatic.dAreaUnder_Public);					//���¹���
	ADDMulXData(objId, _T("FloorSummary"), strText, DOUBLE_TYPE, 1);
	strText.Format(_T("%.5lf"), m_FloorStatic.dAreaUp_Private);					//����˽��
	ADDMulXData(objId, _T("FloorSummary"), strText, DOUBLE_TYPE, 2);
	strText.Format(_T("%.5lf"), m_FloorStatic.dAreaUp_Public);						//���Ϲ���
	ADDMulXData(objId, _T("FloorSummary"), strText, DOUBLE_TYPE, 3);

	//��������ע�����һ������룬�����ͼ��ʱ���飬��ֹ�û�˽�Ը��ƣ���������������
	AddXdata(aname(objId), _T("GUID"), STRING_TYPE, newGUID2());

	return true;
}

// CIndoorSurveyNote �Ի���

IMPLEMENT_DYNAMIC(CIndoorSurveyNoteDlg, CAdUiBaseDialog)

CIndoorSurveyNoteDlg::CIndoorSurveyNoteDlg(CWnd* pParent /*=NULL*/)
: CAdUiBaseDialog(CIndoorSurveyNoteDlg::IDD, pParent)
{
	m_nCount = 0;
	m_dLength = 0.0;
	//	m_dHeight = 0.0;
	m_nNoteType = 0;
	m_dhh = 0.5;
}

CIndoorSurveyNoteDlg::~CIndoorSurveyNoteDlg()
{
}

void CIndoorSurveyNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NOTE_TYPE, m_list1);
	DDX_Control(pDX, IDC_LIST_NOTE_TYPE2, m_list2);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_dLength);
	//DDX_Text(pDX, IDC_EDIT_HEIGHT, m_dHeight);

}

BOOL CIndoorSurveyNoteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list1.Init_ByConfig(_T("�뻧����_����ע���б�ؼ�1.XML"));
	m_list1.InsertItem(0, _T("��ľע��"));
	m_list1.InsertItem(1, _T("ˮ����ʩע��"));
	m_list1.InsertItem(2, _T("Ǩ����ʩע��"));
	m_list1.InsertItem(3, _T("���渽����ע��"));
	m_list1.InsertItem(4, _T("���ݸ���ע��"));

	m_list2.Init_ByConfig(_T("�뻧����_����ע���б�ؼ�2.XML"));
	int nCount = 0;
	for(size_t t = 0; t < g_index.m_items.size(); t++)
	{
		if(0 == _tcscmp(g_index.m_items.at(t).table, _T("SMZj")))
		{
			m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
		}
	}


	return TRUE;
}

BEGIN_MESSAGE_MAP(CIndoorSurveyNoteDlg, CAdUiBaseDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NOTE_TYPE, &CIndoorSurveyNoteDlg::OnNMClickListNoteType)
	ON_BN_CLICKED(IDOK, &CIndoorSurveyNoteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CIndoorSurveyNoteDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
END_MESSAGE_MAP()

// CIndoorSurveyNote ��Ϣ�������

BOOL CIndoorSurveyNoteDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
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

void CIndoorSurveyNoteDlg::OnNMClickListNoteType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM *plvItem = &plvDispInfo->item;

	m_list2.DeleteAllItems();
	m_nNoteType = pNMItemActivate->iItem;
	if(pNMItemActivate->iItem == 4)
	{
		GetDlgItem(IDC_STATIC_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LENGTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_LENGTH)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_STATIC_HEIGHT)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(SW_SHOW);

		int nCount = 0;
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, _T("NEQZJ")))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_COUNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_COUNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_LENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LENGTH)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_HEIGHT)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_EDIT_HEIGHT)->ShowWindow(SW_HIDE);

		int nCount = 0;
		TCHAR szTable[100] = {0};
		if(pNMItemActivate->iItem == 0)
			_tcscpy(szTable, _T("SMZJ"));
		else if(pNMItemActivate->iItem == 1)
			_tcscpy(szTable, _T("SJZJ"));
		else if(pNMItemActivate->iItem == 2)
			_tcscpy(szTable, _T("QYZJ"));
		else
			_tcscpy(szTable, _T("DMZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, szTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}

	m_list2.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

void CIndoorSurveyNoteDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialog::OnOK();

	UpdateData(TRUE);
	if(checkDlgEditControlText(this->m_hWnd, 100))
	{
		AfxMessageBox(_T("\n ������ַ���̫�������������룡"));
		return;
	}

	BeginEditorCommand();

	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());

	read1(m_dhh); m_dhh /= 1000.0;

	vector<int> vSel;
	m_list2.GetSelectedItem(vSel);
	if(vSel.size() == 0) return;
	CString strText = m_list2.GetItemText(vSel.at(0), 0);
	TCHAR szCode[100] = {0};
	if(false == index_name_code(strText, szCode)) return;

	ads_point pt;
	if(RTCAN == ads_getpoint(NULL, _T("\n ָ��ע�ǲ���㣺"), pt)) return;


	TCHAR szBlockName[500] = {0};
	_stprintf(szBlockName, _T("�뻧����_ע��_%s"), szCode);
	AcDbObjectId objId = DrawInsert(pt, szBlockName, m_dhh, m_dhh);
	if(AcDbObjectId::kNull == objId)
	{
		acutPrintf(_T("\n �Ҳ���ģ�壡 %s"), szBlockName);
		return;
	}

	if(m_nNoteType == 4)
	{
		TCHAR szLength[500] = {0}, szHeight[100] = {0};
		_stprintf(szLength, _T("%.2lf"), m_dLength);
		//_stprintf(szHeight, _T("%.2lf"), m_dHeight);

		SBreakBlock blockB;
		blockB.SetBlock(objId);
		blockB.Explode();
		blockB.replaceTextEx(_T("#LENGTH#"), szLength);
		blockB.replaceTextEx(_T("#HEIGHT#"), szHeight);
		AddXdataDouble_bat(blockB.m_objIdArr, _T("CD"), m_dLength);
		//AddXdataDouble_bat(blockB.m_objIdArr, _T("GD"), m_dHeight);
		AddXdata_bat(blockB.m_objIdArr, _T("SOUTH"), STRING_TYPE, szCode);

	}
	else
	{
		TCHAR szText[500] = {0};
		_stprintf(szText, _T("%d"), m_nCount);

		SBreakBlock blockB;
		blockB.SetBlock(objId);
		blockB.Explode();
		blockB.replaceTextEx(_T("#NUM#"), szText);
		AddXdataShort_bat(blockB.m_objIdArr, _T("SM"), m_nCount);
		AddXdata_bat(blockB.m_objIdArr, _T("SOUTH"), STRING_TYPE, szCode);
	}

	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
	CompleteEditorCommand();

}

void CFloorInfoDlg::OnBnClickedButtonPosition()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	vector<int> vSelect;
	m_list.GetSelectedItem(vSelect);
	if(vSelect.size() == 0) return;

	//BeginEditorCommand();
	AcDbObjectIdArray objArr;
	ads_name ssname;
	ads_ssadd(NULL, NULL, ssname);
	for(int t = 0; t < (int)vSelect.size(); t++)
	{
		if(t < 0 || t >= m_objArr.length()) continue;
		ads_name ent;
		if(acdbGetAdsName(ent, m_objArr.at(vSelect.at(t))) != Acad::eOk) continue;
		ads_ssadd(ent, ssname, ssname);
		//objArr.append();
	}

	//acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());

	BeginEditorCommand();

	ads_point ptmin, ptmax;
	GetEntsExtent(ssname, ptmin, ptmax);
	ZoomWindow(ptmin, ptmax);

	acedSSSetFirst(NULL, NULL);
	acedSSSetFirst(ssname, NULL);

	//acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());

	CompleteEditorCommand();
}

// CTestDlg �Ի���

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
END_MESSAGE_MAP()

// CExtentBuildingDlg �Ի���

IMPLEMENT_DYNAMIC(CExtentBuildingDlg, CAdUiBaseDialog)

BOOL CExtentBuildingDlg::OnInitDialog()
{
	CAdUiBaseDialog::OnInitDialog();
	m_list.Init_ByConfig(_T("����������Ի����б�ؼ�.XML"));

	m_btnSelectAll.SetCheck(0);

	return TRUE;
}

CExtentBuildingDlg::CExtentBuildingDlg(CWnd* pParent /*=NULL*/)
	: CAdUiBaseDialog(CExtentBuildingDlg::IDD, pParent)
{
	m_dDist = 0.1;
}

CExtentBuildingDlg::~CExtentBuildingDlg()
{
}

void CExtentBuildingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAIN, m_list);
	DDX_Text(pDX, IDC_EDIT_EXTENT_DIST, m_dDist);
	DDX_Control(pDX, IDC_CHECK_SELECTALL, m_btnSelectAll);
	DDX_Control(pDX, IDC_CHECK_UNSELECT, m_btnInvertSel);
}

BEGIN_MESSAGE_MAP(CExtentBuildingDlg, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_BUILDING, &CExtentBuildingDlg::OnBnClickedButtonSelectBuilding)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAIN, OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_COMBINE_TO_MAIN, &CExtentBuildingDlg::OnBnClickedButtonCombineToMain)
	ON_BN_CLICKED(IDC_BUTTON_COMBINE_TO_ATTACHMENT, &CExtentBuildingDlg::OnBnClickedButtonCombineToAttachment)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_SELECTALL, OnBnClickedChkselectall)
	ON_BN_CLICKED(IDC_CHECK_UNSELECT, OnBnClickedChkinvertsel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RESULT, &CExtentBuildingDlg::OnBnClickedButtonClearResult)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_OLD_BUILDING, &CExtentBuildingDlg::OnBnClickedButtonDeleteOldBuilding)
END_MESSAGE_MAP()

// CExtentBuildingDlg ��Ϣ�������

bool	CExtentBuildingDlg::changeArcPolyLine(AcDbObjectIdArray &objArr)
{
	for(int i = 0; i < objArr.length(); i++)
	{
		AcDbObjectId objId = objArr.at(i);
		if(changeArcPolyLine(objId))
			objArr.setAt(i, objId);
	}
	return true;
}

bool	CExtentBuildingDlg::changeArcPolyLine(AcDbObjectId &objId)
{
	if(HasPolyBulge(aname(objId)))
	{
		ads_name ent;
		if(Acad::eOk != acdbGetAdsName(ent, objId)) return false;

		AcGePoint2dArray Nodes;
		GetNodesHaveArc(ent, Nodes, 1.0);
		SetPolyCoord_NoArc(ent, Nodes);
		if(Acad::eOk != acdbGetObjectId(objId, ent)) return false;
	}
	return true;
}

bool	CExtentBuildingDlg::checkArcPolyLine(AcDbObjectIdArray objArr)
{
	for(int i = 0; i < objArr.length(); i++)
	{
		if(HasPolyBulge(aname(objArr.at(i))))
		{
			AfxMessageBox(_T("���ڴ����εĽ���������Դ����εĽ����������߻�����"));
			return true;
		}
	}

	return false;
}

void CExtentBuildingDlg::OnBnClickedButtonSelectBuilding()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	acedGetAcadFrame()->SetFocus();
	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());




	BeginEditorCommand();

Label1:
	_T("����һ�Σ�");

	CSelect selectT;
	AcDbObjectIdArray objArr;
	selectT.SelectObjects2(objArr, 5, _T("*LINE*"), _T("*"), _T(""));
	if(objArr.length() == 0)
	{
		CancelEditorCommand();
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
		return;
	}

	int es = AfxMessageBox(_T("�Ƿ�����Щ�������棿"), MB_YESNOCANCEL, MB_ICONEXCLAMATION);
	if(es == IDYES)
	{
		_T("�ܺã�");
	}
	else if(es == IDNO)
	{
		goto Label1;
	}
	else
	{
		CancelEditorCommand();
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
		return;
	}

	CompleteEditorCommand();

	if(checkArcPolyLine(objArr))
		changeArcPolyLine(objArr);

	AcDbObjectIdArray objArrExtented;
	m_objArrOldBuilding.setLogicalLength(0);
	for(int i = 0; i < objArr.length(); i++)
	{
		AcDbObjectId objId = objArr.at(i);
		AcGePoint2dArray nodes, nodesOut;
		GetPlList(aname(objId), nodes);
		ads_name entOut;
		if(IsDeasil(nodes))
		{
			PolylineOffset(aname(objId), m_dDist, entOut);
			CopyXData(aname(objId), entOut);
		}
		else
		{
			CEntOper::ReversePolyLine(objId);
			PolylineOffset(aname(objId), m_dDist, entOut);
			CopyXData(aname(objId), entOut);
		}
		//EntDel(aname(objId));
		setlayer(aname(objId), _T("FCS_OLD"));
		setcolor(aname(objId), 7);

		m_objArrOldBuilding.append(objId);
		objArrExtented.append(aname(entOut));
	}

	AcDbObjectIdArray objArrClip;
	for(int i = 0; i < objArrExtented.length(); i++)
	{
		AcGePoint2dArray nodes;
		AcDbObjectIdArray objArrTemp;
		GetPlList(aname(objArrExtented.at(i)), nodes);
		if(HasPolyBulge(aname(objArrExtented.at(i))) == false)
			ssFromNodes(objArrTemp, nodes, 2, 0.0, _T("*LINE*"));
		else
			ssfromNodes_Arc(objArrTemp, objArrExtented.at(i), 2, 0.0, _T("*LINE*"));		//�����ζ���ο�ѡ

		//��������Ķ���ι��˳�������Ҫ
		for(int j = 0; j < objArrTemp.length(); j++)
		{
			if(isPolyInPoly_Completely(objArrTemp.at(j), objArrExtented.at(i)) == 1)
				continue;
			else
			{
				changeArcPolyLine(objArrTemp.at(j));
				objArrClip.append(objArrTemp.at(j));
			}
		}
	}
	objArrClip.append(objArrExtented);
	RemoveSameObjects(objArrClip);
	for(int i = 0; i < m_objArrOldBuilding.length(); i++)
		objArrClip.remove(m_objArrOldBuilding.at(i));

	if(objArrClip.length() == 0)
	{
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
		return;
	}
	vector<AcGePoint2dArray> vNodes, vNodesOut;
	for(int i = 0; i < objArrClip.length(); i++)
	{
		AcGePoint2dArray nodes;
		GetPlList(aname(objArrClip.at(i)), nodes);
		if(nodes.length() < 3)
		{
			acutPrintf(_T("\n һ�������������"));
			continue;
		}
		if(CEntOper::ifPolyClosed(objArrClip.at(i))) nodes.append(nodes.first());
		vNodes.push_back(nodes);
	}
	SNodesOpe nodesO;
	nodesO.getLoopsIntersection(vNodes, vNodesOut);
	if(vNodesOut.size() == 0)
	{
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
		return;
	}

	//m_list.DeleteAllItems();
	//m_objArrExtentClip.setLogicalLength(0);
	//m_objArrClipAttach.setLogicalLength(0);
	int nCount = m_list.GetItemCount();
	CString strText;
	for(int t = 0; t < (int)vNodesOut.size(); t++)
	{
		AcGePoint2dArray nodes = vNodesOut.at(t);
		AcDbObjectId objId = DrawPolyline2D(nodes, 0.0, true, _T(""));
		setcolor(aname(objId), 7);
		setlayer(aname(objId), _T("FCS_TEMP"));



		ads_name entOut;
		if(IsDeasil(nodes) == false)
			CEntOper::ReversePolyLine(objId);

		PolylineOffset(aname(objId), 0.1, entOut);
		AcGePoint2dArray nodesOut;
		GetPlList(entOut, nodesOut);
		EntDel(entOut);

		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();

		objArrClip.setLogicalLength(0);
		ssFromNodes(objArrClip, nodesOut, 2, 0.0, _T("*LINE*"));
		objArrClip.remove(aname(objId));

		//����ѿ�ѡ���ľɵ�ʵ���Ƴ�
		for(int i = 0; i < m_objArrOldBuilding.length(); i++)
			objArrClip.remove(m_objArrOldBuilding.at(i));

		AcGePoint2d ptMid;
		GetRandomPointInNodes(nodes, ptMid);
		for(int j = 0; j < objArrClip.length(); j++)
		{
			nodes.setLogicalLength(0);
			GetPlList(aname(objArrClip.at(j)), nodes);
			if(PointInPolygon(nodes, ptMid) == 1)
			{
				objArrClip.removeAt(j);
				j--;
			}
		}

		vector<AcDbObjectId> vObj;
		vObj.push_back(objId);


		m_list.InsertItem(nCount + t, _T(" "));
		m_list.SetItemText(nCount + t, 1, _T("�ཻ"));

		if(objArrClip.length() >= 2)
		{


			TCHAR sz1[100] = {0}, sz2[500] = {0};
			GetStdm(aname(objArrClip.first()), sz1);
			g_index.get_name(sz1, sz2);
			strText = sz2;
			strText.Replace(_T("ƽ��ͼ"), _T(""));
			strText.Replace(_T("���ͼ"), _T(""));
			m_list.SetItemText(nCount + t, 2, strText);

			GetStdm(aname(objArrClip.at(1)), sz1);
			g_index.get_name(sz1, sz2);
			strText = sz2;
			strText.Replace(_T("ƽ��ͼ"), _T(""));
			strText.Replace(_T("���ͼ"), _T(""));
			m_list.SetItemText(nCount + t, 3, strText);

			vObj.push_back(objArrClip.first());
			vObj.push_back(objArrClip.at(1));
		}

		m_vObjArrRecord.push_back(vObj);

	}

	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());

	// 
	// 	
	// 	
	// 
	// 	objArrClip.remove(objId);
	// 	objArrClip.remove(m_objExtentMain);
	// 
	// 	
	// 	if(distOF2d(nodesOut.first(), nodesOut.last()) >= EPS) nodesOut.append(nodesOut.first());
	// 	vNodes.push_back(nodesOut);

	// 

	// 
	// 	TCHAR szStdm[100] = {0}, szName[300] = {0};
	// 	GetStdm(aname(objId), szStdm);
	// 	g_index.get_name(szStdm, szName);
	// 	EntDel(aname(objId));
	// 
	// 
	// 	if(m_objArrExtentClip.length() != m_objArrClipAttach.length())
	// 	{
	// 		acutPrintf(_T("\n ��Ŀ��ƥ�䣡"));
	// 	}



}

void CExtentBuildingDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	// 	vector<int> vSelect;
	// 	m_list.GetSelectedItem(vSelect);
	// 	if(vSelect.size() == 0) return;

	int nRow = pNMItemActivate->iItem;
	int nCol = pNMItemActivate->iSubItem;
	nCol--;
	if(nCol <= -1 || nCol > 3) return;
	if(nRow >= (int)m_vObjArrRecord.size()) return;

	acedGetAcadFrame()->SetFocus();

	AcGePoint2d ptMid;
	vector<AcDbObjectId> vObj = m_vObjArrRecord.at(nRow);
	if(nCol >= (int)vObj.size()) return;
	AcDbObjectId objId = vObj.at(nCol);
	ptMid = getEntMiddlePoint(objId);
	PanToCenter(apoint(ptMid), true);

	ads_name ssname;
	ads_name ent;
	//IdArrToSS(objArr, ssname);
	if(Acad::eOk != acdbGetAdsName(ent, objId)) return;
	ads_ssadd(NULL, NULL, ssname);
	ads_ssadd(ent, ssname, ssname);
	acedSSSetFirst(NULL, NULL);
	acedSSSetFirst(ssname, NULL);
	ads_ssfree(ssname);
}

void CExtentBuildingDlg::OnBnClickedButtonCombineToMain()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vector<int> vSelect;
	for(int i = 0; i < m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i)) vSelect.push_back(i);
	}
	if(vSelect.size() == 0) return;

	acedGetAcadFrame()->SetFocus();
	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());

	for(size_t t = 0; t < vSelect.size(); t++)
	{
		int nIndex = vSelect.at(t);
		vector<AcDbObjectId> vObj = m_vObjArrRecord.at(nIndex);

		AcDbObjectId objDiv = vObj.at(0);
		AcDbObjectId objToBeDiv = vObj.at(2);
		AcDbObjectId objCopy = CopyEntity(objToBeDiv);		//����Ҫ����������ʵ�壬�������︴��һ��
		setlayer(aname(objCopy), _T("FCS_OLD"));
		setcolor(aname(objCopy), 7);

		AcDbObjectIdArray objArrTemp;
		CAutoRebuild autoR;
		autoR.divEntity(objToBeDiv, objDiv, objArrTemp);
		AcGePoint2dArray nodesDiv;
		GetPlList(aname(objDiv), nodesDiv);

		AcDbObjectId objNew;
		for(int i = 0; i < objArrTemp.length(); i++)
		{
			if(IsPolyInPoly(objArrTemp.at(i), nodesDiv))
			{
				EntDel(aname(objArrTemp.at(i)));
			}
			else
			{
				objNew = objArrTemp.at(i);
			}
		}
		EntDel(aname(objDiv));

		// 		for(vector< vector<AcDbObjectId> >::iterator it = m_vObjArrRecord.begin(); it != m_vObjArrRecord.end(); it ++)
		// 		{
		// 			int nFind = -1;
		// 			it->find(objToBeDiv, nFind);
		// 			if(nFind >= 0)
		// 				it->setAt(nFind, objNew);
		//		}
	}

	for(int t = (int)vSelect.size() - 1; t >= 0; t--)
	{
		int nIndex = vSelect.at(t);
		m_list.DeleteItem(nIndex);
		vector< vector<AcDbObjectId> >::iterator it = m_vObjArrRecord.begin();
		it += nIndex;
		m_vObjArrRecord.erase(it);
	}


	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());

}

void CExtentBuildingDlg::OnBnClickedButtonCombineToAttachment()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vector<int> vSelect;
	for(int i = 0; i < m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i)) vSelect.push_back(i);
	}
	if(vSelect.size() == 0) return;

	acedGetAcadFrame()->SetFocus();
	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());

	for(size_t t = 0; t < vSelect.size(); t++)
	{
		int nIndex = vSelect.at(t);
		vector<AcDbObjectId> vObj = m_vObjArrRecord.at(nIndex);

		AcDbObjectId objDiv = vObj.at(0);
		AcDbObjectId objToBeDiv = vObj.at(1);
		AcDbObjectId objOld = objToBeDiv;
		AcDbObjectIdArray objArrTemp;
		CAutoRebuild autoR;
		autoR.divEntity(objToBeDiv, objDiv, objArrTemp);
		AcGePoint2dArray nodesDiv;
		GetPlList(aname(objDiv), nodesDiv);

		AcDbObjectId objNew;
		for(int i = 0; i < objArrTemp.length(); i++)
		{
			if(IsPolyInPoly(objArrTemp.at(i), nodesDiv))
			{
				EntDel(aname(objArrTemp.at(i)));
				//i--;
			}
			else
			{
				objNew = objArrTemp.at(i);
			}
		}
		EntDel(aname(objDiv));

		for(vector< vector<AcDbObjectId> >::iterator it = m_vObjArrRecord.begin(); it != m_vObjArrRecord.end(); it++)
		{
			for(vector<AcDbObjectId>::iterator itt = it->begin(); itt != it->end(); itt++)
			{
				if(objOld == *itt)
				{
					*itt = objNew;
				}
			}
			// 			int nFind = -1;
			// 			it->find(objToBeDiv, nFind);
			// 			if(nFind >= 0)
			// 				it->setAt(nFind, objNew);
		}

	}

	for(int t = (int)vSelect.size() - 1; t >= 0; t--)
	{
		int nIndex = vSelect.at(t);
		m_list.DeleteItem(nIndex);
		vector< vector<AcDbObjectId> >::iterator it = m_vObjArrRecord.begin();
		it += nIndex;
		m_vObjArrRecord.erase(it);
	}

	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
}

LRESULT CExtentBuildingDlg::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}

BOOL CExtentBuildingDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			// 			DestroyWindow();
			// 			delete this;
			//			return WM_QUIT;
			this->ShowWindow(SW_HIDE);

		}
	}

	return CAdUiBaseDialog::PreTranslateMessage(pMsg);
}

void CExtentBuildingDlg::OnDestroy()
{
	DestroyWindow();
	delete this;

	ads_name ssname;
	ssFromLayers(ssname, _T("FCS_TEMP"));
	EntsDel(ssname);
	ads_ssfree(ssname);
}

void CExtentBuildingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 	DestroyWindow();
	// 	delete this;

	this->ShowWindow(SW_HIDE);

	// 	ads_name ssname;
	// 	ssFromLayers(ssname, _T("FCS_TEMP"));
	// 	EntsDel(ssname);
	//	ads_ssfree(ssname);
}

void CExtentBuildingDlg::OnBnClickedChkselectall()
{
	UpdateData(TRUE);
	m_btnInvertSel.SetCheck(0);
	if(m_btnSelectAll.GetCheck() == 0)
	{
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			m_list.SetCheck(i, FALSE);
		}
	}
	else
	{
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			m_list.SetCheck(i, TRUE);
		}
	}
	UpdateData(FALSE);
}

void CExtentBuildingDlg::OnBnClickedChkinvertsel()
{
	int i, nCount;
	UpdateData(true);
	m_btnSelectAll.SetCheck(0);
	if(m_btnInvertSel.GetCheck() == 1)
	{
		//�����ѡ
		nCount = m_list.GetItemCount();//��ȡ��ѡ����
		for(i = 0; i < nCount; i++)
		{
			//��ÿ��ѡ����в���
			m_list.SetCheck(i, m_list.GetCheck(i) == TRUE ? FALSE : TRUE);
		}
	}
}

CExtentBuildingDlg *g_extentBuildingDlg = NULL;

// CDrawThingsDlg �Ի���

IMPLEMENT_DYNAMIC(CDrawThingsDlg, CDialog)

CDrawThingsDlg::CDrawThingsDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDrawThingsDlg::IDD, pParent)
{

}

CDrawThingsDlg::~CDrawThingsDlg()
{
}

void CDrawThingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CDrawThingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDrawThingsDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDrawThingsDlg::OnNMDblclkList1)
END_MESSAGE_MAP()

// CDrawThingsDlg ��Ϣ�������

void CDrawThingsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();

	bool bModify = false;
	for(int i = 0; i < m_list.GetItemCount(); i++)
	{
		ATTRFIELD field;
		_tcscpy(field.name, m_list.GetItemText(i, 1));
		field.type = _ttoi(m_list.GetItemText(i, 2));
		_tcscpy(field.text, m_list.GetItemText(i, 3));
		m_vFields.push_back(field);

		if(0 == _tcscmp(field.name, _T("CBJ")))
		{
			if(0 < _ttoi(field.text))
				bModify = true;
		}

		for(size_t t = 0; t < m_sDrawThings.size(); t++)
		{
			if(m_sDrawThings.at(t).strName == m_list.GetItemText(i, 1))
			{
				m_sDrawThings.at(t).strValue = m_list.GetItemText(i, 3);
				break;
			}
		}
	}

	if(bModify)
	{
		for(vector<ATTRFIELD>::iterator it = m_vFields.begin(); it != m_vFields.end(); it++)
		{
			if(0 == _tcscmp(it->name, _T("QSLC")) || 0 == _tcscmp(it->name, _T("ZZLC")))
			{
				if(_ttoi(it->text) > 0)
				{
					CString strText;
					strText.Format(_T("%d"), -_ttoi(it->text));
					_tcscpy(it->text, strText);
				}
			}
		}
		for(size_t t = 0; t < m_sDrawThings.size(); t++)
		{
			if(m_sDrawThings.at(t).strName == _T("QSLC") || m_sDrawThings.at(t).strName == _T("ZZLC"))
			{
				if(_ttoi(m_sDrawThings.at(t).strValue) > 0)
				{
					CString strText;
					strText.Format(_T("%d"), -_ttoi(m_sDrawThings.at(t).strValue));
					m_sDrawThings.at(t).strValue = strText;
				}
			}
		}
	}
}

bool		CDrawThingsDlg::checkAndModifyUnderGroundNumber()
{

	return true;
}

BOOL CDrawThingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.Init_ByConfig(_T("���Ƶ����б�ؼ������ļ�.XML"));

	for(int t = 0; t < (int)m_sDrawThings.size(); t++)
	{
		m_list.InsertItem(t, m_sDrawThings.at(t).strText);
		m_list.SetItemText(t, 1, m_sDrawThings.at(t).strName);
		m_list.SetItemText(t, 2, m_sDrawThings.at(t).strType);
		m_list.SetItemText(t, 3, m_sDrawThings.at(t).strMJXS);
	}

	return TRUE;

	// 
	// 	TiXmlDocument *tinyXml = NULL;
	// 
	// 	if(false == openXMLFile(tinyXml, _T("���Ƶ���_�����ֶ�.XML"))) return TRUE;
	// 
	// 	TiXmlElement* pRoot = tinyXml->FirstChildElement( "DrawingThings" );
	// 	if ( NULL == pRoot )
	// 	{
	// 		delete tinyXml;
	// 		return false;
	// 	}
	// 
	// 	TiXmlElement *pTable = pRoot->FirstChildElement("Table");
	// 	if(pTable == NULL)
	// 	{
	// 		delete tinyXml;
	// 		return false;
	// 	}
	// 
	// 	while(pTable)
	// 	{
	// 		CString strName = m_list.GetXMLAttribute_EditList(pTable, _T("name"));
	// 		if(strName == m_strTableName)
	// 		{
	// 			
	// 			TiXmlElement *pField = pTable->FirstChildElement("Field");
	// 			if(pField == NULL) return false;
	// 
	// 			int nCount = 0;
	// 			while(pField)
	// 			{
	// 				strName = m_list.GetXMLAttribute_EditList(pField, _T("text"));
	// 				m_list.InsertItem(nCount, strName);
	// 				
	// 				strName = m_list.GetXMLAttribute_EditList(pField, _T("name"));
	// 				m_list.SetItemText(nCount, 1, strName);
	// 
	// 				//����Ĭ�����ϵ��
	// 				if(strName == _T("MJXS"))
	// 				{
	// 					CString strDefault = findDefaultValue();
	// 					m_list.SetItemText(nCount, 3, strDefault);
	// 				}
	// 
	// 				strName = m_list.GetXMLAttribute_EditList(pField, _T("type"));
	// 				m_list.SetItemText(nCount, 2, strName);
	// 				
	// 				pField = pField->NextSiblingElement();
	// 				nCount ++;
	// 			}
	// 
	// 			break;
	// 		}
	// 
	// 		pTable = pTable->NextSiblingElement();
	// 	}
	// 
	// 	delete tinyXml;

	//	return TRUE;
}

LRESULT CIndoorSurveyNoteDlg::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}

void CIndoorSurveyNoteDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
	delete this;
}

// CIndoorSurveyNote_Standard2_Dlg �Ի���

IMPLEMENT_DYNAMIC(CIndoorSurveyNote_Standard2_Dlg, CAdUiBaseDialog)

CIndoorSurveyNote_Standard2_Dlg::CIndoorSurveyNote_Standard2_Dlg(CWnd* pParent /*=NULL*/)
: CAdUiBaseDialog(CIndoorSurveyNote_Standard2_Dlg::IDD, pParent)
{
	m_nNoteType = 0;
	// 	_tcscpy(m_nCount, _T(""));
	// 	_tcscpy(m_dLength, _T(""));
	// 	_tcscpy(m_dHeight, _T(""));
	// 	_tcscpy(m_dDeep, _T(""));
	// 	_tcscpy(m_dSpan, _T(""));
	// 	_tcscpy(m_dWelldiameter, _T(""));
	// 	_tcscpy(m_dTreediameter, _T(""));
}

CIndoorSurveyNote_Standard2_Dlg::~CIndoorSurveyNote_Standard2_Dlg()
{
}

BOOL CIndoorSurveyNote_Standard2_Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
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

void CIndoorSurveyNote_Standard2_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAdUiBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NOTE_TYPE, m_list1);
	DDX_Control(pDX, IDC_LIST_NOTE_TYPE2, m_list2);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_dLength);
	DDX_Text(pDX, IDC_EDIT_HEIGH, m_dHeight);
	DDX_Text(pDX, IDC_EDIT_DEEP, m_dDeep);
	DDX_Text(pDX, IDC_EDIT_SPAN, m_dSpan);
	DDX_Text(pDX, IDC_EDIT_WELL_DIAMETER, m_dWelldiameter);
	DDX_Text(pDX, IDC_EDIT_TREE_DIAMETER, m_dTreediameter);

}

BOOL CIndoorSurveyNote_Standard2_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list1.Init_ByConfig(_T("�뻧����_����ע���б�ؼ�1.XML"));
	m_list1.InsertItem(0, _T("���ݸ���ע��"));
	m_list1.InsertItem(1, _T("����ע��"));
	m_list1.InsertItem(2, _T("̨��\\ʯ��ע��"));
	m_list1.InsertItem(3, _T("ˮ��ע��"));
	m_list1.InsertItem(4, _T("��ľ������ע��"));
	m_list1.InsertItem(5, _T("������ľע��"));
	m_list1.InsertItem(6, _T("���渽����ע��"));
	m_list1.InsertItem(7, _T("Ǩ��ע��"));


	m_list2.Init_ByConfig(_T("�뻧����_����ע���б�ؼ�2.XML"));
	int nCount = 0;
	for(size_t t = 0; t < g_index.m_items.size(); t++)
	{
		if(0 == _tcscmp(g_index.m_items.at(t).table, _T("NEQZJ")))
		{
			m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
		}
	}


	return TRUE;
}

void CIndoorSurveyNote_Standard2_Dlg::OnNMClickListNoteType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	LV_DISPINFO *plvDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM *plvItem = &plvDispInfo->item;

	m_list2.DeleteAllItems();
	m_nNoteType = pNMItemActivate->iItem;
	int nCount = 0;

	GetDlgItem(IDC_EDIT_HEIGH)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_LENGTH)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_SPAN)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DEEP)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_WELL_DIAMETER)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_COUNT)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_TREE_DIAMETER)->SetWindowText(_T(""));

	if(pNMItemActivate->iItem == 0)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);

		_tcscpy(m_szCurrentTable, _T("NEQZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}
	else if(pNMItemActivate->iItem == 1)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);

		_tcscpy(m_szCurrentTable, _T("HDZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}


	}
	else if(pNMItemActivate->iItem == 2)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);

		_tcscpy(m_szCurrentTable, _T("TTSYZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}
	else if(pNMItemActivate->iItem == 3)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);

		_tcscpy(m_szCurrentTable, _T("SJZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}
	else if(pNMItemActivate->iItem == 4)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(TRUE);

		_tcscpy(m_szCurrentTable, _T("QGZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}
	else if(pNMItemActivate->iItem == 5)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);

		_tcscpy(m_szCurrentTable, _T("SMZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}
	else if(pNMItemActivate->iItem == 6)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);

		_tcscpy(m_szCurrentTable, _T("DMZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}
	else if(pNMItemActivate->iItem == 7)
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);

		_tcscpy(m_szCurrentTable, _T("QYZJ"));
		for(size_t t = 0; t < g_index.m_items.size(); t++)
		{
			if(0 == _tcscmp(g_index.m_items.at(t).table, m_szCurrentTable))
			{
				m_list2.InsertItem(nCount++, g_index.m_items.at(t).name);
			}
		}
	}

	m_list2.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

}

void CIndoorSurveyNote_Standard2_Dlg::OnNMClickListNoteType2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	if(m_nNoteType != 3) return;
	int nIndex = m_list2.GetSelectedItem();
	CString str = m_list2.GetItemText(nIndex, 0);
	if(str == _T("��ѹ��"))
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_HEIGH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WELL_DIAMETER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TREE_DIAMETER)->EnableWindow(FALSE);
	}
}

void CIndoorSurveyNote_Standard2_Dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(checkDlgEditControlText(this->m_hWnd, 100))
	{
		AfxMessageBox(_T("\n ������ַ���̫�������������룡"));
		return;
	}

	BeginEditorCommand();

	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());
	double	m_dhh = 500;
	read1(m_dhh); m_dhh /= 1000.0;

	vector<int> vSel;
	m_list2.GetSelectedItem(vSel);
	if(vSel.size() == 0)
	{
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
		return;
	}


	ads_point pt;
	if(RTCAN == ads_getpoint(NULL, _T("\n ָ��ע�ǲ���㣺"), pt))
	{
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
		return;
	}


	TCHAR szBlockName[500] = {0};
	TCHAR szCode[100] = {0};
	CString strName;
	strName = m_list2.GetItemText(vSel.at(0), 0);
	if(0 == _tcscmp(m_szCurrentTable, _T("QGZJ")) && m_list2.GetSelectedItem() == 0)			//�����������ر���
		_tcscpy(szCode, _T("141019"));
	else if(0 == _tcscmp(m_szCurrentTable, _T("QGZJ")) && m_list2.GetSelectedItem() == 1)
		_tcscpy(szCode, _T("141279"));
	else
	{
		if(false == index_name_code(m_szCurrentTable, strName, szCode))
		{
			acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());

			return;
		}
	}

	_stprintf(szBlockName, _T("�뻧����_ע��_%s"), szCode);

	AcDbObjectId objId = DrawInsert(pt, szBlockName, m_dhh, m_dhh);
	if(AcDbObjectId::kNull == objId)
	{
		acutPrintf(_T("\n �Ҳ���ģ�壡 %s"), szBlockName);
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());

		return;
	}

	SBreakBlock blockB;
	blockB.SetBlock(objId);
	blockB.Explode();

	for(int i = 0; i < blockB.m_objIdArr.length(); i++)
	{
		ads_name ent;
		if(Acad::eOk != acdbGetAdsName(ent, blockB.m_objIdArr.at(i))) continue;

		CStringArray strArr;
		GetAllAppName(ent, strArr);
		for(int j = 0; j < strArr.GetSize(); j++)
			RemoveXdata(ent, strArr.GetAt(j));
	}

	blockB.replaceTextEx(_T("#NAME#"), strName);

	TCHAR szText[500] = {0};
	if(_tcslen(m_nCount) > 0)
	{
		short sCount = _ttoi(m_nCount);
		blockB.replaceTextEx(_T("#NUM#"), m_nCount);
		AddXdataShort_bat(blockB.m_objIdArr, _T("SM"), sCount);
	}
	if(_tcslen(m_dLength) > 0.0)
	{
		blockB.replaceTextEx(_T("#LENGTH#"), m_dLength);
		double dLength = _tstof(m_dLength);
		AddXdataDouble_bat(blockB.m_objIdArr, _T("ZJCD"), dLength);
	}
	if(_tcslen(m_dHeight) > 0.0)
	{
		blockB.replaceTextEx(_T("#HEIGH#"), m_dHeight);
		double dHeight = _tstof(m_dHeight);
		AddXdataDouble_bat(blockB.m_objIdArr, _T("GD"), dHeight);

	}
	if(_tcslen(m_dDeep) > 0.0)
	{
		blockB.replaceTextEx(_T("#DEEP#"), m_dDeep);
		double dDeep = _tstof(m_dDeep);
		AddXdataDouble_bat(blockB.m_objIdArr, _T("DEEP"), dDeep);

	}
	if(_tcslen(m_dSpan) > 0.0)
	{
		blockB.replaceTextEx(_T("#KJ#"), m_dSpan);
		double dSpan = _tstof(m_dSpan);
		AddXdataDouble_bat(blockB.m_objIdArr, _T("KJ"), dSpan);
	}
	if(_tcslen(m_dWelldiameter) > 0.0)
	{
		blockB.replaceTextEx(_T("#GJ#"), m_dWelldiameter);
		double dWellDiameter = _tstof(m_dWelldiameter);
		AddXdataDouble_bat(blockB.m_objIdArr, _T("GJ"), dWellDiameter);
	}
	if(_tcslen(m_dTreediameter) > 0.0)
	{
		blockB.replaceTextEx(_T("#XJ#"), m_dTreediameter);
		double dTreediameter = _tstof(m_dTreediameter);
		AddXdataDouble_bat(blockB.m_objIdArr, _T("XJ"), dTreediameter);
	}

	AddXdata_bat(blockB.m_objIdArr, _T("NAME"), STRING_TYPE, strName);
	AddXdata_bat(blockB.m_objIdArr, _T("SOUTH"), STRING_TYPE, szCode);


	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
	CompleteEditorCommand();
}

void CIndoorSurveyNote_Standard2_Dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
	delete this;
}

LRESULT CIndoorSurveyNote_Standard2_Dlg::onAcadKeepFocus(WPARAM n, LPARAM l)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CIndoorSurveyNote_Standard2_Dlg, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NOTE_TYPE, &CIndoorSurveyNote_Standard2_Dlg::OnNMClickListNoteType)
	ON_BN_CLICKED(IDOK, &CIndoorSurveyNote_Standard2_Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CIndoorSurveyNote_Standard2_Dlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NOTE_TYPE2, &CIndoorSurveyNote_Standard2_Dlg::OnNMClickListNoteType2)
END_MESSAGE_MAP()

// CIndoorSurveyNote_Standard2_Dlg ��Ϣ�������

void CDrawThingsDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	if(pNMItemActivate->iSubItem != 3) return;

	CString strKey = m_list.GetItemText(m_list.GetSelectedItem(), 1);
	//strKey.Replace(_T("#"), _T(""));

	CArray<ATTRFIELD, ATTRFIELD> *AttrArr = NULL;
	if(false == findAttriTable(m_strTableName, AttrArr)) return;

	//�����lisitem���ҵ�ƥ�������������б�
	CString strLstTable;
	bool bFind = false;
	for(int i = 0; i < AttrArr->GetSize(); i++)
	{
		if(strKey == AttrArr->GetAt(i).name)
		{
			ATTRFIELD field = AttrArr->GetAt(i);
			strLstTable = field.dict;
			bFind = true;
			break;
		}
	}
	if(bFind == false) return;

	for(size_t t = 0; t < g_lstitems.m_vFieldDicts.size(); t++)
	{
		//�ҵ��ˣ���������ѡ��
		if(strLstTable == g_lstitems.m_vFieldDicts.at(t).szDictName)
		{
			vector<CString> vStr = g_lstitems.m_vFieldDicts.at(t).vRecord;
			CString strItem;
			for(size_t tt = 0; tt < vStr.size(); tt++)
			{
				strItem += vStr.at(tt);
				strItem += _T(";");
			}
			m_list.ClearColumnSetInfo();
			m_list.SetComboColumns(3, strItem);
			return;
		}
	}

	//���û�ҵ�����ԭ
	m_list.ClearColumnSetInfo();

}

// CEntitysListDlg �Ի���

IMPLEMENT_DYNAMIC(CEntitysListDlg, CAdUiBaseDialog)

CEntitysListDlg::CEntitysListDlg(CWnd* pParent /*=NULL*/)
: CAdUiBaseDialog(CEntitysListDlg::IDD, pParent)
{
	m_sType = 0;
	_tcscpy(m_szConfigFileName, _T("ʵ���б�ؼ�.XML"));
}

CEntitysListDlg::~CEntitysListDlg()
{
}

void CEntitysListDlg::DoDataExchange(CDataExchange* pDX)
{
	CAdUiBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CEntitysListDlg, CAdUiBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_SPLASH, &CEntitysListDlg::OnBnClickedButtonSplash)
	ON_BN_CLICKED(IDOK, &CEntitysListDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CEntitysListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_sType == 1)
	{
		m_list.Init_ByConfig(_T("ʵ���б�ؼ�.XML"));

		for(int i = 0; i < m_objIdArr.length(); i++)
		{
			TCHAR szCode[200] = {0}, szName[200] = {0};
			GetStdm(aname(m_objIdArr.at(i)), szCode);
			g_index.get_name(szCode, szName);

			m_list.InsertItem(i, szName);

		}
	}
	else if(m_sType == 2)
	{
		m_list.Init_ByConfig(_T("ʵ���б�ؼ�_2.XML"));

		for(int i = 0; i < m_objIdArr.length(); i++)
		{
			TCHAR sz[100] = {0};
			_stprintf(sz, _T("�иΧ%d"), i + 1);
			m_list.InsertItem(i, sz);
		}
	}



	return TRUE;
}
// CEntitysListDlg ��Ϣ�������

BOOL CEntitysListDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
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

void CEntitysListDlg::OnBnClickedButtonSplash()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vector<int> vSelect;
	m_list.GetSelectedItem(vSelect);
	AcDbObjectIdArray objArr;
	ads_name ssname;
	ads_ssadd(NULL, NULL, ssname);
	for(size_t t = 0; t < vSelect.size(); t++)
	{
		objArr.append(m_objIdArr.at(vSelect.at(t)));
		ads_ssadd(aname(m_objIdArr.at(vSelect.at(t))), ssname, ssname);
	}

	SplashEntitys(objArr, 1);
	acedSSSetFirst(NULL, NULL);
	acedSSSetFirst(ssname, NULL);
	ads_command(RTSTR, _T("REDRAW"), RTNONE);
	ads_ssfree(ssname);
}

void CEntitysListDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();

	vector<int> vSelect;
	m_list.GetSelectedItem(vSelect);
	for(size_t t = 0; t < vSelect.size(); t++)
	{
		m_objIdArrSelected.append(m_objIdArr.at(vSelect.at(t)));
	}

	acedSSSetFirst(NULL, NULL);
}

void CExtentBuildingDlg::OnBnClickedButtonDeleteOldBuilding()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());

	EntsDel(m_objArrOldBuilding);
	m_objArrOldBuilding.setLogicalLength(0);

	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
}

void CExtentBuildingDlg::OnBnClickedButtonClearResult()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();

	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());

	for(vector< vector<AcDbObjectId> >::iterator it = m_vObjArrRecord.begin(); it != m_vObjArrRecord.end(); it++)
	{
		if(it->size() == 0) continue;
		EntDel(aname(it->at(0)));
	}
	m_vObjArrRecord.clear();

	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
}

// CResultsCheckWizardDlg �Ի���

IMPLEMENT_DYNAMIC(CResultsCheckWizardDlg, CAdUiBaseDialog)

CResultsCheckWizardDlg::CResultsCheckWizardDlg(CWnd* pParent /*=NULL*/)
: CAdUiBaseDialog(CResultsCheckWizardDlg::IDD, pParent)
{
	m_dPagePos = 0;
}

CResultsCheckWizardDlg::~CResultsCheckWizardDlg()
{
}

void CResultsCheckWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CAdUiBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WIZ_FRAME, m_GroupControl);

}

BOOL CResultsCheckWizardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pProperSheet = new CPropertySheet(_T("�ɹ������"));
	m_SelectTypePage = new CSelectTypeWizard();
	m_SelectPathPage = new CSelectPathWizard();
	//m_ExportResultPage = new CExportResultWizard();

	m_pProperSheet->AddPage(m_SelectTypePage);
	m_pProperSheet->AddPage(m_SelectPathPage);
	//m_pProperSheet->AddPage(m_ExportResultPage);

	m_pProperSheet->SetWizardMode();

	m_pProperSheet->Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	m_dPageCount = m_pProperSheet->GetPageCount();

	CRect rect, rectGroup;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	m_GroupControl.GetClientRect(&rectGroup);
	m_GroupControl.ShowWindow(SW_HIDE);
	m_pProperSheet->MoveWindow(0, 0, 500, 200);


	m_pProperSheet->SetActivePage(0);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(FALSE);


	return TRUE;
}

BEGIN_MESSAGE_MAP(CResultsCheckWizardDlg, CAdUiBaseDialog)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &CResultsCheckWizardDlg::OnBnClickedButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CResultsCheckWizardDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDOK, &CResultsCheckWizardDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CSelectTypeWizard �Ի���

IMPLEMENT_DYNAMIC(CSelectTypeWizard, CPropertyPage)

CSelectTypeWizard::CSelectTypeWizard()
: CPropertyPage(CSelectTypeWizard::IDD)
{

}

CSelectTypeWizard::~CSelectTypeWizard()
{
}

void CSelectTypeWizard::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHECK_TYPE, m_list);
}

BOOL CSelectTypeWizard::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_list.Init_ByConfig(_T("�ɹ�����б�ؼ�.XML"));

	m_list.InsertItem(0, _T("¥��һ���Լ��"));
	m_list.InsertItem(1, _T("����ƺһ���Լ��"));

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSelectTypeWizard, CPropertyPage)
END_MESSAGE_MAP()

// CSelectPathWizard �Ի���

IMPLEMENT_DYNAMIC(CSelectPathWizard, CPropertyPage)

CSelectPathWizard::CSelectPathWizard()
: CPropertyPage(CSelectPathWizard::IDD)
{

}

CSelectPathWizard::~CSelectPathWizard()
{
}

void CSelectPathWizard::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FEATURE_TABLE, m_strPathFeatureTabe);
	DDX_Text(pDX, IDC_EDIT_BUILDING_AREA_FRAME, m_strPathBuildAreaFrame);
	DDX_Text(pDX, IDC_EDIT_SUMMARY_TABLE, m_strPathSummaryTable);
	DDX_Text(pDX, IDC_EDIT_TOTAL_FRAME, m_strPathTotalFrame);

}

BOOL CSelectPathWizard::OnInitDialog()
{
	SReadConfig readConfig;
	m_strPathFeatureTabe = readConfig.getConfigValue(_T("�ɹ����_Ҫ�ر�_����·��"), _T("�������������ļ�.xml"));
	m_strPathSummaryTable = readConfig.getConfigValue(_T("�ɹ����_���ܱ�_����·��"), _T("�������������ļ�.xml"));
	m_strPathBuildAreaFrame = readConfig.getConfigValue(_T("�ɹ����_���ͼ_����·��"), _T("�������������ļ�.xml"));
	m_strPathTotalFrame = readConfig.getConfigValue(_T("�ɹ����_��ƽͼ_����·��"), _T("�������������ļ�.xml"));

	return CPropertyPage::OnInitDialog();
}

BEGIN_MESSAGE_MAP(CSelectPathWizard, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE1, &CSelectPathWizard::OnBnClickedButtonBrowse1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, &CSelectPathWizard::OnBnClickedButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE3, &CSelectPathWizard::OnBnClickedButtonBrowse3)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE4, &CSelectPathWizard::OnBnClickedButtonBrowse4)
END_MESSAGE_MAP()

// CExportResultWizard �Ի���

IMPLEMENT_DYNAMIC(CExportResultWizard, CPropertyPage)

CExportResultWizard::CExportResultWizard()
: CPropertyPage(CExportResultWizard::IDD)
{

}

CExportResultWizard::~CExportResultWizard()
{
}

void CExportResultWizard::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExportResultWizard, CPropertyPage)
END_MESSAGE_MAP()

// CExportResultWizard ��Ϣ�������

void CResultsCheckWizardDlg::OnBnClickedButtonPrevious()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_dPagePos == 1)
	{
		m_dPagePos--;
		m_pProperSheet->SetActivePage(m_dPagePos);
		GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	// 	else if(m_dPagePos == 1)
	// 	{
	// 		m_dPagePos--;
	// 		m_pProperSheet->SetActivePage(m_dPagePos);
	// 		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
	// 		GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(FALSE);
	// 		GetDlgItem(IDOK)->EnableWindow(FALSE);	
	// 	}

}

void CResultsCheckWizardDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_dPagePos == 0)
	{
		m_SelectTypePage->UpdateData(TRUE);

		m_dPagePos++;
		m_pProperSheet->SetActivePage(m_dPagePos);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	}
	// 	else if(m_dPagePos == 1)
	// 	{
	// 		m_dPagePos++;
	// 		//m_pProperSheet->SetActivePage(m_dPagePos);	
	// 		GetDlgItem(IDOK)->EnableWindow(TRUE);
	// 		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	// 		GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(TRUE);
	// 	}

}

void CSelectPathWizard::OnBnClickedButtonBrowse1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	resbuf rc;
	if(RTERROR == ads_getfiled(_T(""), _T("Ҫ�ؼ����.xlsx"), _T("xls;xlsx"), NULL, &rc)) return;

	m_strPathFeatureTabe = rc.resval.rstring;

	UpdateData(FALSE);
}

void CSelectPathWizard::OnBnClickedButtonBrowse2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	resbuf rc;
	if(RTERROR == ads_getfiled(_T(""), _T("�������ͼ.dwg"), _T("dwg"), NULL, &rc)) return;

	m_strPathBuildAreaFrame = rc.resval.rstring;
	UpdateData(FALSE);

}

void CSelectPathWizard::OnBnClickedButtonBrowse3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	resbuf rc;
	if(RTERROR == ads_getfiled(_T(""), _T("�����ɹ����ܱ�"), _T("xls;xlsx"), NULL, &rc)) return;

	m_strPathSummaryTable = rc.resval.rstring;
	UpdateData(FALSE);

}

void CSelectPathWizard::OnBnClickedButtonBrowse4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	resbuf rc;
	if(RTERROR == ads_getfiled(_T(""), _T("��ƽͼ"), _T("dwg"), NULL, &rc)) return;

	m_strPathTotalFrame = rc.resval.rstring;
	UpdateData(FALSE);

}

void CResultsCheckWizardDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	vector<int>		vSelectedType;
	for(int i = 0; i < m_SelectTypePage->m_list.GetItemCount(); i++)
	{
		if(m_SelectTypePage->m_list.GetCheck(i) == TRUE)
			vSelectedType.push_back(i);
	}
	if(vSelectedType.size() == 0)
	{
		AfxMessageBox(_T("��ѡ��Ҫ������Ŀ!"));
		return;
	}

	CAdUiBaseDialog::OnOK();

	CCheckResult checkResult;
	checkResult.m_strPathFeatureTable = m_SelectPathPage->m_strPathFeatureTabe;
	checkResult.m_strPathBuildingAreaFrame = m_SelectPathPage->m_strPathBuildAreaFrame;
	checkResult.m_strPathSummartTable = m_SelectPathPage->m_strPathSummaryTable;
	checkResult.m_strPathTotalFrame = m_SelectPathPage->m_strPathTotalFrame;

	SReadConfig readConfig;
	readConfig.setConfigValue(_T("�ɹ����_Ҫ�ر�_����·��"), m_SelectPathPage->m_strPathFeatureTabe, _T("�������������ļ�.xml"));
	readConfig.setConfigValue(_T("�ɹ����_���ܱ�_����·��"), m_SelectPathPage->m_strPathSummaryTable, _T("�������������ļ�.xml"));
	readConfig.setConfigValue(_T("�ɹ����_���ͼ_����·��"), m_SelectPathPage->m_strPathBuildAreaFrame, _T("�������������ļ�.xml"));
	readConfig.setConfigValue(_T("�ɹ����_��ƽͼ_����·��"), m_SelectPathPage->m_strPathTotalFrame, _T("�������������ļ�.xml"));

	acutPrintf(_T("\n ��ʼ�ɹ���� ===================================== \n"));
	for(size_t t = 0; t < vSelectedType.size(); t++)
	{
		int nType = vSelectedType.at(t);
		switch(nType)
		{
			case 0:
			{
				checkResult.checkBuildingName();
				break;
			}
			case 1:
			{
				checkResult.checkIndoorHeight();
				break;
			}
			case 2:
				break;
			default:
				break;
		}
	}

	acutPrintf(_T("\n\n �ɹ������� ====================================="));


}

// CCreateHdmDlg dialog

CCreateHdmDlg::CCreateHdmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateHdmDlg::IDD, pParent)
	, m_start(0)
{
	//{{AFX_DATA_INIT(CCreateHdmDlg)
	m_dLeft = 5.0;
	m_dRight = 5.0;
	m_dStep = 20.0;
	m_nMiddle = 1;
}

void CCreateHdmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HDMCREATE_LEFT, m_dLeft);
	DDV_MinMaxDouble(pDX, m_dLeft, 0., 10000.);
	DDX_Text(pDX, IDC_HDMCREATE_RIGHT, m_dRight);
	DDV_MinMaxDouble(pDX, m_dRight, 0., 10000.);
	DDX_Text(pDX, IDC_HDMCREATE_STEP, m_dStep);
	DDV_MinMaxDouble(pDX, m_dStep, 0.1, 10000.);
	DDX_Text(pDX, IDC_HDMCREATE_START, m_start);

}

BEGIN_MESSAGE_MAP(CCreateHdmDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateHdmDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCreateHdmDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateHdmDlg message handlers

BOOL CCreateHdmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
// 	GetDlgItem(IDC_HDMCREATE_MIDDLE1)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_HDMCREATE_MIDDLE2)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_HDMCREATE_MIDDLE3)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateHdmDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CButton *btn1 = (CButton *)GetDlgItem(IDC_HDMCREATE_MIDDLE1);
	CButton *btn2 = (CButton *)GetDlgItem(IDC_HDMCREATE_MIDDLE2);
	CButton *btn3 = (CButton *)GetDlgItem(IDC_HDMCREATE_MIDDLE3);
	if(btn1->GetCheck() == 1) m_nMiddle = 1;
	else if(btn2->GetCheck() == 1) m_nMiddle = 2;
	else if(btn3->GetCheck() == 2) m_nMiddle = 3;
	CDialog::OnOK();
}

// CWriteLchFileDlg dialog

CWriteLchFileDlg::CWriteLchFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWriteLchFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWriteLchFileDlg)
	m_strDatFile = _T("");
	m_strGcdFile = _T("");
	m_dInterval = 5.0;
	m_strLchFile = _T("");
	m_strLCZMC = _T("");
	m_bExcel = FALSE;
	m_nType = 1;
	m_bChazhi = FALSE;
	m_bJiaodian = TRUE;
	//}}AFX_DATA_INIT
}

void CWriteLchFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWriteLchFileDlg)
	//DDX_Text(pDX, IDC_LCHFILE_DATFILE, m_strDatFile);
	DDX_Text(pDX, IDC_LCHFILE_GCDFILE, m_strGcdFile);
	DDX_Text(pDX, IDC_LCHFILE_DIST, m_dInterval);
	//DDX_Text(pDX, IDC_LCHFILE_LCHFILE, m_strLchFile);
	DDX_Text(pDX, IDC_LCHFILE_LCZMC, m_strLCZMC);
	DDX_Check(pDX, IDC_LCHFILE_EXCEL, m_bExcel);
	DDX_Radio(pDX, IDC_LCHFILE_TYPE1, m_nType);
	DDX_Check(pDX, IDC_LCHFILE_CHAZHI, m_bChazhi);
	DDX_Check(pDX, IDC_LCHFILE_JIAODIAN, m_bJiaodian);
	DDX_Text(pDX, IDC_LCHFILE_DLMC, m_strDLMC);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWriteLchFileDlg, CDialog)
	//{{AFX_MSG_MAP(CWriteLchFileDlg)
	ON_BN_CLICKED(IDC_LCHFILE_GCDBROWSE, OnLchfileGcdbrowse)
	ON_BN_CLICKED(IDC_LCHFILE_LCHBROWSE, OnLchfileLchbrowse)
	ON_BN_CLICKED(IDC_LCHFILE_DATBROWSE, OnLchfileDatbrowse)
	ON_BN_CLICKED(IDC_LCHFILE_TYPE1, OnLchfileType1)
	ON_BN_CLICKED(IDC_LCHFILE_TYPE2, OnLchfileType2)
	ON_BN_CLICKED(IDC_LCHFILE_CHAZHI, OnLchfileChazhi)
	ON_BN_CLICKED(IDC_LCHFILE_TYPE3, OnLchfileType3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWriteLchFileDlg message handlers

BOOL CWriteLchFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	ShowDatFilenameControl();
	ShowIntervalControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWriteLchFileDlg::OnOK()
{
	UpdateData(TRUE);

	if(0 == m_nType && m_strGcdFile.IsEmpty())
	{
		ads_alert(_T("�̵߳������ļ�������Ϊ��")); return;
	}
	if(2 == m_nType && m_strGcdFile.IsEmpty())
	{
		ads_alert(_T("�����������ļ�������Ϊ��")); return;
	}
	/*if (m_strLchFile.IsEmpty())
	{
	ads_alert(_T("����ļ�������Ϊ��")); return;
	}*/
	if(m_dInterval < 0.01)
	{
		ads_alert(_T("��ֵ���̫С")); return;
	}
	if(m_strDLMC.IsEmpty())
	{
		ads_alert(_T("��·���Ʋ���Ϊ��")); return;
	}
	if(m_strLCZMC.IsEmpty())
	{
		ads_alert(_T("���׮���Ʋ���Ϊ��")); return;
	}
	CDialog::OnOK();
}

void CWriteLchFileDlg::OnLchfileGcdbrowse()
{
	TCHAR szFile[MAX_PATH];
	TCHAR szTip[50] = _T("����̵߳������ļ���"), szExt[50] = _T("dat");
	UpdateData(TRUE);
	if(2 == m_nType)
	{
		_tcscpy(szTip, _T("���������������ļ���"));
		_tcscpy(szExt, _T("sjw"));
	}
	if(GetCassFileName(szTip, g_szUsePath, szExt, 0, szFile) == true)
	{
		m_strGcdFile.Format(_T("%s"), szFile);
		UpdateData(FALSE);
	}
}

void CWriteLchFileDlg::OnLchfileLchbrowse()
{
	TCHAR szFile[MAX_PATH];
	if(GetCassFileName(_T("��������ļ���"), g_szUsePath, _T("hdm"), 1, szFile) == true)
	{
		UpdateData(TRUE);
		m_strLchFile.Format(_T("%s"), szFile);
		UpdateData(FALSE);
	}
}

void CWriteLchFileDlg::OnLchfileDatbrowse()
{
	TCHAR szFile[MAX_PATH];
	if(GetCassFileName(_T("������Ӧ�����������ļ���"), g_szUsePath, _T("dat"), 1, szFile) == true)
	{
		UpdateData(TRUE);
		m_strDatFile.Format(_T("%s"), szFile);
		UpdateData(FALSE);
	}
}

void CWriteLchFileDlg::OnLchfileType1()
{
	UpdateData(TRUE);
	ShowDatFilenameControl();
}

void CWriteLchFileDlg::OnLchfileType2()
{
	UpdateData(TRUE);
	ShowDatFilenameControl();
}

void CWriteLchFileDlg::OnLchfileType3()
{
	UpdateData(TRUE);
	ShowDatFilenameControl();
}

void CWriteLchFileDlg::ShowDatFilenameControl()
{
	GetDlgItem(IDC_LCHFILE_FILETIP)->EnableWindow(0 == m_nType || 2 == m_nType);
	GetDlgItem(IDC_LCHFILE_GCDFILE)->EnableWindow(0 == m_nType || 2 == m_nType);
	GetDlgItem(IDC_LCHFILE_GCDBROWSE)->EnableWindow(0 == m_nType || 2 == m_nType);
}

void CWriteLchFileDlg::OnLchfileChazhi()
{
	UpdateData(TRUE);
	ShowIntervalControl();
}

void CWriteLchFileDlg::ShowIntervalControl()
{
	GetDlgItem(IDC_LCHFILE_DIST)->EnableWindow(TRUE == m_bChazhi);
}

// CDrawDzxDlg dialog

CDrawDzxDlg::CDrawDzxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawDzxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawDzxDlg)
	m_dDeltz = 1.0;
	m_dMaxH = 0.0;
	m_dMinH = 0.0;
	m_nNihe = 2;
	m_bSingle = FALSE;
	m_dSingleH = 0.0;
	m_dSmgc = 0.0;
	m_dStep = 2.0;
	m_dTolerance = 0.0;
	//}}AFX_DATA_INIT
	m_nDzxType = 1;
}

void CDrawDzxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawDzxDlg)
	DDX_Text(pDX, IDC_DZX_DELTZ, m_dDeltz);
	DDV_MinMaxDouble(pDX, m_dDeltz, 1.e-003, 10000.);
	DDX_Text(pDX, IDC_DZX_MAXH, m_dMaxH);
	DDX_Text(pDX, IDC_DZX_MINH, m_dMinH);
	DDX_Radio(pDX, IDC_DZX_NIHE1, m_nNihe);
	DDX_Check(pDX, IDC_DZX_SINGLE, m_bSingle);
	DDX_Text(pDX, IDC_DZX_SINGLEH, m_dSingleH);
	DDX_Text(pDX, IDC_DZX_SMGC, m_dSmgc);
	DDX_Text(pDX, IDC_DZX_STEP, m_dStep);
	DDV_MinMaxDouble(pDX, m_dStep, 1.e-003, 10000.);
	DDX_Text(pDX, IDC_DZX_TOLERANCE, m_dTolerance);
	DDV_MinMaxDouble(pDX, m_dTolerance, 0., 10000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDrawDzxDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawDzxDlg)
	ON_BN_CLICKED(IDC_DZX_SINGLE, OnDzxSingle)
	ON_BN_CLICKED(IDC_DZX_NIHE1, OnDzxNihe1)
	ON_BN_CLICKED(IDC_DZX_NIHE2, OnDzxNihe2)
	ON_BN_CLICKED(IDC_DZX_NIHE3, OnDzxNihe3)
	ON_BN_CLICKED(IDC_DZX_NIHE4, OnDzxNihe4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawDzxDlg message handlers

BOOL CDrawDzxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(2 == m_nDzxType)
	{
		GetDlgItem(IDC_DRAWDZX_STATIC1)->SetWindowText(_T("��ǳ��ˮ��:"));
		GetDlgItem(IDC_DRAWDZX_STATIC2)->SetWindowText(_T("�����ˮ��:"));
		GetDlgItem(IDC_DZX_SINGLE)->SetWindowText(_T("����������:"));
		GetDlgItem(IDC_DZX_SINGLE_TXT1)->SetWindowText(_T("����������ˮ��:"));
		GetDlgItem(IDC_DZX_DELTZ_TXT1)->SetWindowText(_T("�����:"));
	}

	ShowControl1();
	ShowControl2();
	GetDlgItem(IDC_DZX_SMGC)->EnableWindow(2 == m_nDzxType);
	GetDlgItem(IDC_DZX_SMGC_TXT1)->EnableWindow(2 == m_nDzxType);
	GetDlgItem(IDC_DZX_SMGC_TXT2)->EnableWindow(2 == m_nDzxType);

#ifdef GD_HANGDAO
	GetDlgItem(IDC_DZX_SMGC_TXT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DZX_SMGC_TXT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DZX_SMGC)->ShowWindow(SW_HIDE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawDzxDlg::OnDzxSingle()
{
	ShowControl1();
}

void CDrawDzxDlg::OnDzxNihe1()
{
	ShowControl2();
}

void CDrawDzxDlg::OnDzxNihe2()
{
	ShowControl2();
}

void CDrawDzxDlg::OnDzxNihe3()
{
	ShowControl2();
}

void CDrawDzxDlg::OnDzxNihe4()
{
	ShowControl2();
}

void CDrawDzxDlg::ShowControl1()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_DZX_SINGLEH)->EnableWindow(TRUE == m_bSingle);
	GetDlgItem(IDC_DZX_SINGLE_TXT1)->EnableWindow(TRUE == m_bSingle);
	GetDlgItem(IDC_DZX_SINGLE_TXT2)->EnableWindow(TRUE == m_bSingle);
	GetDlgItem(IDC_DZX_DELTZ)->EnableWindow(FALSE == m_bSingle);
	GetDlgItem(IDC_DZX_DELTZ_TXT1)->EnableWindow(FALSE == m_bSingle);
	GetDlgItem(IDC_DZX_DELTZ_TXT2)->EnableWindow(FALSE == m_bSingle);
}

void CDrawDzxDlg::ShowControl2()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_DZX_STEP)->EnableWindow(1 == m_nNihe);
	GetDlgItem(IDC_DZX_STEP_TXT)->EnableWindow(1 == m_nNihe);
	GetDlgItem(IDC_DZX_TOLERANCE)->EnableWindow(3 == m_nNihe);
	GetDlgItem(IDC_DZX_TOLERANCE_TXT)->EnableWindow(3 == m_nNihe);
}

void CDrawDzxDlg::OnOK()
{
	UpdateData(TRUE);
	if(TRUE == m_bSingle)
	{
		if(m_dSingleH <= m_dMinH || m_dSingleH >= m_dMaxH)
		{
			ads_alert(_T("�����ȸ��߸߳�ֵ������Χ"));
			return;
		}
	}

	CDialog::OnOK();
}