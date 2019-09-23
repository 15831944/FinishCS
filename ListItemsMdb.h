#pragma once
#include "AdoConnection.h"
#include "AdoRecordSet.h"
#include "AdoCommand.h"

//存储所有的下拉列表,包括ListItem中的
struct FieldDict2
{
	TCHAR szDictName[128];//词典名字
	//CString strData;//词典的内容，中间以","隔开 写入扩展数据的东西. 89
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

	vector<FieldDict2> m_vFieldDicts;//所有的词典

	bool readLstitem(const TCHAR *szName = _T("lstitems.mdb"));
};


