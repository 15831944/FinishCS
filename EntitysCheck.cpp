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
	AGrid_ClearSheet(_T("高程点注记检查"));
	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(objArr.at(i)), szStdm);
		if(0 > _FindString(vStrCode, szStdm)) continue;

		SBlockOpe blockO;
		str = blockO.getAttributeValue(objArr.at(i));
		if(_tstof(str) == 0.0)
		{
			AGrid_AppendMsgEnt(_T("高程点注记检查"),_T("高程点注记值为 0"),_T("警告"), objArr.at(i));
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
