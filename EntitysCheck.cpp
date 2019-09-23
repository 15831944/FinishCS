#include "stdafx.h"
#include "EntitysCheck.h"

bool	CEntitysCheck::checkElevationPointAttribute()
{
	AcDbObjectIdArray objArr;
	//ssFromLayers(objArr, _T("DXT"), false);
	CSelect selectT;
	selectT.SelectObjects2(objArr, 1, _T(""), _T("*"), _T(""));
	vector<CString> vStrCode;
	vStrCode.push_back(_T("202101"));

	CString str;
	int nCount = 0;
	AGrid_ClearSheet(_T("�̵߳�ע�Ǽ��"));
	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(objArr.at(i)), szStdm);
		if(0 > _FindString(vStrCode, szStdm)) continue;

		SBlockOpe blockO;
		str = blockO.getAttributeValue(objArr.at(i));
		if(_tstof(str) == 0.0)
		{
			AGrid_AppendMsgEnt(_T("�̵߳�ע�Ǽ��"),_T("�̵߳�ע��ֵΪ 0"),_T("����"), objArr.at(i));
			nCount ++;
		}
	}

	if(nCount) AGrid_ShowMe();

	return true;
}


CEntitysCheck::CEntitysCheck(void)
{
}


CEntitysCheck::~CEntitysCheck(void)
{
}
