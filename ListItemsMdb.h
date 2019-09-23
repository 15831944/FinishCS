#pragma once
#include "AdoConnection.h"
#include "AdoRecordSet.h"
#include "AdoCommand.h"

//�洢���е������б�,����ListItem�е�
struct FieldDict2
{
	TCHAR szDictName[128];//�ʵ�����
	//CString strData;//�ʵ�����ݣ��м���","���� д����չ���ݵĶ���. 89
	vector<CString> vRecord;
};

struct FieldDictLess 
{
	bool   operator()(const   FieldDict2&   a,   const   FieldDict2&   b)  
	{  
		return  _tcsicmp(a.szDictName,b.szDictName) < 0 ;
	}
};

class CListItemsMdb
{
public:
	CListItemsMdb(void);
	virtual ~CListItemsMdb(void);

	vector<FieldDict2> m_vFieldDicts;//���еĴʵ�

	bool readLstitem(const TCHAR *szName = _T("lstitems.mdb"));
};


