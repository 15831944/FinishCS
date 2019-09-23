#include "stdafx.h"
#include "ContructsOperate.h"

bool CContructsOperate::readMustFillFieldFile()
{
	m_vMustFillField.clear();

	TiXmlDocument *tinyXml = NULL;

	if(false == openXMLFile(tinyXml, _T("绘制地物_必填字段.XML"))) return false;

	TiXmlElement* pRoot = tinyXml->FirstChildElement( "DrawingThings" );
	if ( NULL == pRoot )
	{
		delete tinyXml;
		return false;
	}

	TiXmlElement *pThing = pRoot->FirstChildElement("Thing");
	if(pThing == NULL)
	{
		delete tinyXml;
		return false;
	}

	CEditListCtrl editList;
	while(pThing)
	{
		SMustFillField fillField;

		fillField.strCode = editList.GetXMLAttribute_EditList(pThing, ("code"));

		TiXmlElement *pField = pThing->FirstChildElement("Field");
		while(pField)
		{
			DrawThingsData data;
			data.strText = editList.GetXMLAttribute_EditList(pField, ("text"));

			data.strName = editList.GetXMLAttribute_EditList(pField, ("name"));

			data.strType = editList.GetXMLAttribute_EditList(pField, ("type"));

			fillField.vFields.push_back(data);

			pField = pField->NextSiblingElement();

		}

		m_vMustFillField.push_back(fillField);

		pThing = pThing->NextSiblingElement();
	}

	delete tinyXml;
	
	return true;
}

bool			CContructsOperate::checkMustFillFields()
{
	
	if(false == readMustFillFieldFile()) return false;

	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 1);

	acutPrintf(_T("\n 属性检查开始"));

	AGrid_ClearSheet(_T("实体属性检查"));
	
	int nCount = 0;
	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		ads_name ent;
		if(Acad::eOk != acdbGetAdsName(ent, objArr.at(i))) continue;

		GetStdm(ent, szStdm);

		SMustFillField mustFill;
		if(findThingsInfo(szStdm, mustFill) == false) continue;

		TCHAR szMsg[500] = {0};

		bool bHaveQSLC = false, bHaveZZLC = false;
		double dQSLC = 0.0, dZZLC = 0.0;
		for(vector<DrawThingsData>::iterator it = mustFill.vFields.begin(); it != mustFill.vFields.end(); it ++)
		{
 			int nType = _ttoi(it->strType);

			TCHAR szValue[500] = {0};
			ReadXdata(ent, it->strName, nType, szValue);

			//起始，终止楼层不能等于0
			if(it->strName == _T("QSLC") || it->strName == _T("ZZLC"))
			{
				if(_tstof(szValue) == 0.0)
				{
					_stprintf(szMsg, _T("属性值[%s %s]为空或不正确！ "), it->strText, it->strName);
					AGrid_AppendMsgEnt(_T("实体属性检查"), szMsg, _T("警告"), ent);
					nCount ++;
				}

				if(it->strName == _T("QSLC"))
				{
					bHaveQSLC = true;
					dQSLC = _tstof(szValue);
				}
				if(it->strName == _T("ZZLC"))
				{
					bHaveZZLC = true;
					dZZLC = _tstof(szValue);
				}
			}
			//面积系数不能小于等于0
			if( it->strName == _T("MJXS"))
			{
				if(_tstof(szValue) <= 0.0)
				{
					_stprintf(szMsg, _T("属性值[%s %s]为空或不正确！ "), it->strText, it->strName);
					AGrid_AppendMsgEnt(_T("实体属性检查"), szMsg, _T("警告"), ent);
					nCount ++;
				}
			}

			//是否地下室与是否扣除只能为0或1
			if(it->strName == _T("CBJ") || it->strName == _T("SFKC"))
			{
				if(_ttoi(szValue) != 0 && _ttoi(szValue) != 1)
				{
					_stprintf(szMsg, _T("属性值[%s %s]只能为 0 或 1！ "), it->strText, it->strName);
					AGrid_AppendMsgEnt(_T("实体属性检查"), szMsg, _T("警告"), ent);
					nCount ++;
				}
			}
		}

		//终止楼层必须大于起始楼层，不管是地上还是地下
		if(bHaveQSLC && bHaveZZLC)
		{
			if(dZZLC < dQSLC)
			{
				AGrid_AppendMsgEnt(_T("实体属性检查"), _T("属性值[终止楼层]应该大于或等于[起始楼层]！"), _T("警告"), ent);
				nCount ++;
			}
		}

		//本来就是地下建筑的是否地下层应该为1
		if(mustFill.strCode == _T("410202") || mustFill.strCode == _T("420902"))
		{
			TCHAR szValue[500] = {0};
			ReadXdata(ent, _T("CBJ"), INT_TYPE, szValue);
			if(_ttoi(szValue) != 1)
			{
				_stprintf(szMsg, _T("属性值[是否地下层]应该为 1！"));
				AGrid_AppendMsgEnt(_T("实体属性检查"), szMsg, _T("警告"), ent);
				nCount ++;
			}
		}
		//中空的是否扣除应该为1
		if(mustFill.strCode == _T("420502"))
		{
			TCHAR szValue[500] = {0};
			ReadXdata(ent, _T("SFKC"), INT_TYPE, szValue);
			if(_ttoi(szValue) != 1)
			{
				_stprintf(szMsg, _T("属性值(是否扣除)应该为 1！"));
				AGrid_AppendMsgEnt(_T("实体属性检查"), szMsg, _T("警告"), ent);
				nCount ++;
			}
		}

	}

	if(nCount) AGrid_ShowMe();
	
	acutPrintf(_T("\n 属性检查结束，共检查 %d 个实体，发现 %d 个错误"), objArr.length(), nCount);

	return true;
}


bool			CContructsOperate::putP()
{
	if(checkLoadARX(_T("cass9.arx")) == false) return false;

	ads_point pt;
	ads_name ent;
	TCHAR szStdm[100] = {0};
	TCHAR szLineType[200] = {0};
	bool bPutCode = false;

	while(1)
	{
		ads_initget(NULL, _T("C"));
		int val = ads_entsel(_T("\n输入代码(C)/<选择已有地物>"), ent, pt);
		if(val == RTKWORD)
		{
			if(RTCAN == ads_getstring(NULL, _T("\n请输入代码:"), szStdm)) return false;
			bPutCode = true;
			break;
		}
		else if(val == RTNORM)
			break;
		else
			return false;
	}

	ads_name ssName;
	ads_printf(_T("\n选择要修改的目标对象："));
	if(ads_ssget(NULL,NULL,NULL,NULL,ssName) != RTNORM ) return false;

	long len = 0;
	ads_sslength(ssName, &len);
	if(len == 0) return false;

	if(bPutCode)
	{
		ads_command(RTSTR, _T("PUTP"), RTSTR, _T("C"), RTSTR, szStdm, RTPICKS, ssName, RTSTR, _T(""), RTNONE);
		g_index.get_ltype(szStdm, szLineType);
	}
	else
	{
		ads_command(RTSTR, _T("PUTP"), RTENAME, ent, RTPICKS, ssName, RTSTR, _T(""), RTNONE);
		GetStdm	(ent, szStdm);
		GetEntLineType(ent, szLineType);
	}

	for(long l = 0; l < len; l ++)
	{
		ads_name ent;
		ads_ssname(ssName, l, ent);
		setlinetype(ent, szLineType);
	}

	TCHAR szTable[500] = {0};
	g_index.get_table(szStdm, szTable);

	if(false == findThingsInfo(szStdm)) return false;

	if(m_sDrawThings.size() > 0)
	{
		CDrawThingsDlg drawDlg;

		//如果是复制cass编码的方式，则把源实体的必填属性读取出来，显示在对话框中
		if(bPutCode == false)
		{
			for(size_t t = 0; t < m_sDrawThings.size(); t++)
			{
				TCHAR szValue[200] = {0};
				ReadXdata(ent, m_sDrawThings.at(t).strName, _ttoi(m_sDrawThings.at(t).strType), szValue);
				m_sDrawThings.at(t).strMJXS = szValue;
			}
		}
		else
		{
					//如果是输入新的cass编码方式，则直接弹出对话框，让用户输入必填属性
		}

		drawDlg.m_sDrawThings.swap(m_sDrawThings);
		drawDlg.m_strTableName = szTable;
		drawDlg.m_strCode = szStdm;
		if(IDOK != drawDlg.DoModal()) return false;

		for(size_t t = 0; t < drawDlg.m_vFields.size(); t ++)
		{
			ATTRFIELD field = drawDlg.m_vFields.at(t);

			//这里要特别处理一下，如果一些字符串值是类似“编码 值”的格式，则只提取编码写进去就行了
			CStringArray strArr;
			AnalyzeData(strArr, field.text, _T(" "));
			if(strArr.GetSize() >= 2) 
			{
				if(CheckDigital(strArr.GetAt(0)))
				{
					_stprintf	(field.text, _T("%s"), strArr.GetAt(0));
				}
			}

			//AddXdata(aname(objLast), field.name, field.type, field.text);
			AddXdata_bat(ssName,  field.name, field.type, field.text);
		}

		m_sDrawThings.swap(drawDlg.m_sDrawThings);
	}


	return true;
}


bool			CContructsOperate::addAttributeInfo(ads_name ent)
{
	AcDbObjectId objId;
	if (Acad::eOk != acdbGetObjectId(objId, ent))
	{
		ads_printf(_T("\n 获取最后一个实体失败！"));
		return false;
	}
	CString szStdm;
	TCHAR szCode[50] = { 0 };
	GetStdm(ent, szCode);
	CString temp;
	temp.Format(_T("%s"), szCode);
	TCHAR sz_fwcs[20] = { 0 };
	if (temp == "141141" || temp == _T("141171") || temp == _T("141181"))
	{
		if (ads_getstring(0, _T("\n输入层数(有地下室输入格式:房屋层数-地下层数):"), sz_fwcs) != RTNORM) return false;
	}
	AcGePoint2dArray nodes;
	GetPlList(ent, nodes);
	TCHAR sz_fwjg[20] = { 0 };
	g_workdef.get_parTwo(szCode, sz_fwjg);
	ads_point pt;
	GetMiddlePoint(nodes, pt);
	_tcscat(sz_fwjg, sz_fwcs);
	if (_tcscmp(sz_fwjg, _T("0")) != 0)
		DrawText(pt, sz_fwjg, _T("MIDDLE"), szCode, 0.8);
	if (false == addEntToLayer(ent)) return false;

	temp.Format(_T("%s"), sz_fwcs);
	if (temp.Find(_T("-")) != -1)
	{
		temp.Replace(_T("-"), _T("."));
	}
	ADDMulXData(objId, _T("Structure"), sz_fwjg, 0);
	ADDMulXData(objId, _T("SOUTH"), temp, DOUBLE_TYPE, 1);


	//然后给实体添加必填字段等信息
	return writeThingsCodes(objId);
}

bool			CContructsOperate::addEntToLayer(ads_name ent)
{
	TCHAR szLayer[20] = { 0 };
	if (GetEntLayer(ent, szLayer) == false) return false;
	ads_name last_ent;
	int type = ads_entlast(last_ent);
	setlayer(last_ent, szLayer);
	return true;
}

bool			CContructsOperate::dzpb()
{
	//首先调用倪总的CASS9的命令“DZPB”,获取连接出来的最后一个实体
	resbuf *rResult;
	resbuf *rlist = ads_buildlist(RTSTR, _T("invokedzpb"), RTNONE);
	if(RTNORM != ads_invoke(rlist, &rResult)) return false;

	ads_name ent;
	ads_name_set(rResult->resval.rlname, ent);

	return addAttributeInfo(ent);
}

bool			CContructsOperate::checkUnCalulatedBuildings()
{
	vector<CString> vCode;
	for(size_t t = 0; t < g_index.m_items.size(); t ++)
	{
		if(0 == _tcscmp(g_index.m_items.at(t).table, _T("建筑物分层信息")))
		{
			vCode.push_back(g_index.m_items.at(t).code);
		}
	}

	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 1, _T("*LINE*"));
	if(objArr.length() == 0) return false;

	AGrid_ClearSheet(_T("未统计建筑面积检查"));

	int nCount = 0;
	for(int i = 0; i < objArr.length(); i ++)
	{
		ads_name ent;
		if(Acad::eOk != acdbGetAdsName(ent, objArr.at(i))) continue;

		TCHAR szStdm[100] = {0};
		GetStdm(ent, szStdm);
		if(_FindString(vCode, szStdm) < 0) continue;

		short sValue = 0;
		ReadXdataShort(ent, XDATA_CALCULATE_AREA, sValue);
		if(sValue == 0)
		{
			AGrid_AppendMsgEnt(_T("未统计建筑面积检查"),_T("建筑面积未统计!"),_T("警告"), ent);
			nCount ++;
		}
		else
		{
			vector<AcDbObjectId> vObj;
			GetGroupEnts(aname(ent), vObj);
			bool bFind = false;
			for(size_t t = 0; t < vObj.size(); t ++)
			{
				//TCHAR szValue[500] = {0};
				//READMulXData(aname(vObj.at(t)), XDATA_FloorSummary, STRING_TYPE, szValue);
				if(HasXData(vObj.at(t), XDATA_FloorSummary))
				{
					bFind = true;
					break;
				}
			}

			if(bFind == false)
			{
				AGrid_AppendMsgEnt(_T("未统计建筑面积检查"),_T("建筑面积未统计!"),_T("警告"), ent);
				nCount ++;
			}
		}
	}

	if(nCount) AGrid_ShowMe();

	return true;
}


void	CContructsOperate::insertAndChangeBaseMaps()
{
	resbuf rc;
	if(RTERROR == ads_getfiled(_T(""), _T("底图数据.dwg"), _T("dwg"), NULL, &rc)) return;

	CString strDwgPath = rc.resval.rstring;

	AcDbObjectIdArray objArrBefore, objArrAfter;
	CSelect selectT;
	selectT.SelectObjects2(objArrBefore, 1);

	InsertDwg(strDwgPath);

	selectT.SelectObjects2(objArrAfter, 1);

	RemoveSameObjects(objArrAfter, objArrBefore);

	//setColorsEx(objArrAfter, 7);

	//AcCmColor color;
	//color.setColorIndex(7);
	//SetLayerColor(_T("DXT"), color);
	//setlayers(objArrAfter, _T("DXT"));
	ads_name ssElevPoint;
	ads_ssadd(NULL, NULL, ssElevPoint);

	acedSetStatusBarProgressMeter(_T("正在插入实体..."),0, objArrAfter.length());
	for(int i = 0; i < objArrAfter.length(); i ++)
	{
		acedSetStatusBarProgressMeterPos(i);

		TCHAR szType[100] = {0};
		GetEntityType(aname(objArrAfter.at(i)), szType);
		if(0 == _tcsicmp(szType, _T("INSERT")))		
		{
			TCHAR szStdm[100] = {0};
			GetStdm(aname(objArrAfter.at(i)), szStdm);
			if(CheckGcdCode(szStdm,false))
			{
				ads_name ent;
				if(Acad::eOk == acdbGetAdsName(ent, objArrAfter.at(i)))
				{
					ads_ssadd(ent, ssElevPoint, ssElevPoint);
				}
			}
// 			if(isElevationPoint(objArrAfter.at(i)) == true)
// 			{
// 				//ads_ssadd(aname(objArrAfter.at(i)), ssname, ssname);
// 
// 			}
			else
			{
				SBreakBlock blockB; //块炸开
				blockB.SetBlock(objArrAfter.at(i));
				blockB.Explode();
				// 				setlayers(blockB.m_objIdArr, _T("DXT"));
				// 				setColorsEx2(blockB.m_objIdArr, 1);
			}
		}
		// 		else
		// 		{
		// 			setColorsEx2(objArrAfter.at(i), 1);
		// 		}

	}

	ads_command(RTSTR, _T("EXPLODEGCD"), RTPICKS, ssElevPoint, RTSTR, _T(""), RTNONE);
	ads_ssfree(ssElevPoint);

	objArrAfter.setLogicalLength(0);
	selectT.SelectObjects2(objArrAfter, 1);
	RemoveSameObjects(objArrAfter, objArrBefore);
	setlayers(objArrAfter, _T("DXT"));
	setColorsEx2(objArrAfter, 1);

	acedRestoreStatusBar();

	return;
}


bool	CContructsOperate::extractText()
{
	CAcModuleResourceOverride myResources;

	CExtractTextdlg *extractDlg = new CExtractTextdlg();
	if(extractDlg->GetSafeHwnd() == NULL)
	{
		extractDlg->Create(IDD_DIALOG_EXTRACTDLG);
		extractDlg->ShowWindow(SW_SHOW);
		extractDlg->CenterWindow();
	}

	return true;
}


bool	CContructsOperate::scaleDimensionText()
{
	acutPrintf(_T("\n 请选择注记！"));
	AcDbObjectIdArray objArr;
	CSelect selectT;
	selectT.SelectObjects2(objArr, 2, _T("DIMENSION"));
	if(objArr.length() == 0)
	{
		acutPrintf(_T("\n 请选择注记！"));
		return false;
	}

	ads_real dScale = 1.0;
	if(RTCAN == ads_getreal(_T("\n 请输入注记缩放系数:\n"), &dScale)) return false;

	SDimensionOpe dimenOpe;
	for(int i = 0; i < objArr.length(); i ++)
	{
		dimenOpe.scaleText(objArr.at(i), dScale);
	}

	return true;
}


bool	CContructsOperate::getLastEntity_PolyLine(AcDbObjectId &objId)
{

	ads_name ent;
	ads_entlast(ent);
	bool bFind = false;

	TCHAR szType[200] = {0};
	GetEntityType(ent, szType);

	//判断方法：如果是线，或文字，则判断它是不是附属地物（cass编码带-）
	if(0 == _tcsicmp(szType, _T("LWPOLYLINE")) || 0 == _tcsicmp(szType, _T("LINE")) ||
		0 == _tcsicmp(szType, _T("TEXT")))
	{

		if(Acad::eOk != acdbGetObjectId(objId, ent))  return bFind;

		//如果是附属地物，则通过编组法获取到主地物
		TCHAR szCode2[100] = {0};
		GetStdm(ent, szCode2);
		CString strText = szCode2;
		if(strText.Find(_T("-")) >= 0)
		{
			vector<AcDbObjectId> vObjId;
			GetGroupEnts(aname(ent), vObjId);
			if(vObjId.size() == 0) return false;
			for(size_t t = 0; t < vObjId.size(); t ++)
			{
				GetStdm(aname(vObjId.at(t)), szCode2);
				strText = szCode2;
				if(strText.Find(_T("-")) < 0)
				{
					objId = vObjId.at(t);
					bFind = true;
					break;
				}
			}
		}
		else
			{

				//如果不是附属地物，但是是文字的话，则通过文字中心点选多边形获取实体
				if(0 == _tcsicmp(szType, _T("TEXT")))
				{
					//GetEntPosition(ent, pt);
					ads_point pt1, pt2, pt3, pt4;
					GetTextExtents(ent, pt1, pt2, pt3, pt4);
					AcGePoint3d ptMid = GetMiddlePoint(apoint(pt1), apoint(pt3));
					ads_point pt;
					pt[X] = ptMid.x; pt[Y] = ptMid.y; pt[Z] = ptMid.z;
					TCHAR szLayer[100] = {0};
					GetEntLayer(ent, szLayer);
					RegionSel_Auto(ent, pt, szLayer, _T(""), _T("*LINE*"));
					if(Acad::eOk == acdbGetObjectId(objId, ent)) bFind = true;
				}
				else
				{
					//然后其它情况都最后获取的一个实体
					bFind = true;
				}
				
			}
	}
	
	

	return bFind;
}

bool		CContructsOperate::findThingsInfo(const TCHAR *szCode, SMustFillField &FillField)
{
	for(size_t t = 0; t < m_vMustFillField.size(); t ++)
	{
		if(m_vMustFillField.at(t).strCode == szCode)
		{
			FillField = m_vMustFillField.at(t);
			return true;
		}
	}
	return false;
}


bool	CContructsOperate::findThingsInfo(const TCHAR *szCode, DrawThingsData &data)
{
	for(size_t t = 0; t < m_sDrawThings.size(); t ++)
	{
		if(m_sDrawThings.at(t).strCode == szCode)
		{
			data = m_sDrawThings.at(t);
			return true;
		}
	}

	return false;
}


bool	CContructsOperate::readNoteTextConfig_Indoor()
{
	m_sDrawThings.clear();

	TiXmlDocument *tinyXml = NULL;

	if(false == openXMLFile(tinyXml, _T("入户文字注记格式配置文件.XML"))) return false;

	TiXmlElement* pRoot = tinyXml->FirstChildElement( "NoteThings" );
	if ( NULL == pRoot )
	{
		delete tinyXml;
		return false;
	}

	TiXmlElement *pThing = pRoot->FirstChildElement("Thing");
	if(pThing == NULL)
	{
		delete tinyXml;
		return false;
	}

	bool bFind = false;
	CEditListCtrl editList;
	while(pThing)
	{
		DrawThingsData data;
		CString strName = editList.GetXMLAttribute_EditList(pThing, ("code"));
		data.strCode = strName;
		strName = editList.GetXMLAttribute_EditList(pThing, ("note"));
		data.strNote = strName;
		strName = editList.GetXMLAttribute_EditList(pThing, ("noteText"));
		data.strNoteText = strName;
		data.dTextHeight = _tstof(editList.GetXMLAttribute_EditList(pThing, ("height")));
		data.dTextWidth = _tstof(editList.GetXMLAttribute_EditList(pThing, ("width")));
		data.strTextStyle = editList.GetXMLAttribute_EditList(pThing, ("style"));
		m_sDrawThings.push_back(data);

		pThing = pThing->NextSiblingElement();
	}

	delete tinyXml;

	return true;
}

bool	CContructsOperate::findThingsInfo(const TCHAR *szCode)
{
	m_sDrawThings.clear();

	TiXmlDocument *tinyXml = NULL;

	if(false == openXMLFile(tinyXml, _T("绘制地物_必填字段.XML"))) return false;

	TiXmlElement* pRoot = tinyXml->FirstChildElement( "DrawingThings" );
	if ( NULL == pRoot )
	{
		delete tinyXml;
		return false;
	}

	TiXmlElement *pThing = pRoot->FirstChildElement("Thing");
	if(pThing == NULL)
	{
		delete tinyXml;
		return false;
	}

	bool bFind = false;
	CEditListCtrl editList;
	while(pThing)
	{
		CString strName = editList.GetXMLAttribute_EditList(pThing, ("code"));
		if(strName == szCode)
		{
			bFind = true;

			strName = editList.GetXMLAttribute_EditList(pThing, ("note"));
			if(strName == _T("1"))
			{
				m_bNote = true; 
				m_strNoteName = editList.GetXMLAttribute_EditList(pThing, ("block"));
			}
			else
				m_bNote = false;

			TiXmlElement *pField = pThing->FirstChildElement("Field");
			if(pField == NULL) break;

			int nCount = 0;
			while(pField)
			{
				DrawThingsData data;
				strName = editList.GetXMLAttribute_EditList(pField, ("text"));
				data.strText = strName;

				strName = editList.GetXMLAttribute_EditList(pField, ("name"));
				data.strName = strName;

				//设置默认面积系数
				if(strName == _T("MJXS"))
				{
					CString strDefault = findDefaultValue(szCode);
					data.strMJXS = strDefault;
				}

				strName = editList.GetXMLAttribute_EditList(pField, ("type"));
				data.strType = strName;

				m_sDrawThings.push_back(data);

				pField = pField->NextSiblingElement();
				nCount ++;
			}

			break;
		}

		if(bFind) break;

		pThing = pThing->NextSiblingElement();
	}

	delete tinyXml;

	return true;
}

void SendCommand(const TCHAR* szCommand)
{
	COPYDATASTRUCT msg;
	msg.dwData = (DWORD)1;
	msg.cbData = (DWORD)(_tcslen(szCommand) + 1)*sizeof(TCHAR);
	msg.lpData = (TCHAR*)szCommand;

	acedGetAcadFrame()->SendMessage(WM_COPYDATA, (WPARAM)adsw_acadMainWnd(), (LPARAM)&msg);
	acedGetAcadDockCmdLine()->SetFocus();
}


TCHAR g_sz[200] = _T("");
bool CContructsOperate::drawThings()
{

	TCHAR szCode[200] = {0};
	//if(ads_getstring(NULL, NULL, szCode) != RTNORM) return false;
	if(ads_getstring(NULL, g_sz, szCode) != RTNORM) return false;
	if(0 == _tcscmp(szCode, _T(""))) 
		_tcscpy(szCode, g_sz);
	else
		_tcscpy(g_sz, szCode);


	resbuf *cmd,*result;
// 	struct resbuf rb;
// 	ads_getvar(_T("osmode"),&rb);
// 	int oldpick = rb.resval.rint;


	cmd = ads_buildlist(RTSTR,_T("invokedd"),RTSTR,szCode,RTNONE);
	ads_invoke(cmd,&result);
	ads_relrb(cmd);	

	AcDbObjectId objLast;
	if(getLastEntity_PolyLine(objLast) == false)
	{
		acutPrintf(_T("\n 获取最后一个多边形失败！"));
		return false;
	}

	writeThingsCodes(objLast);

	return true;
}

bool   CContructsOperate::writeThingsCodes(AcDbObjectId objLast)
{
	TCHAR szCode[200] = {0};
	GetStdm(aname(objLast), szCode);
	if(0 == _tcscmp(szCode, _T(""))) return false;

	//特殊建筑物（一半算地上，一半算地下的），这里给它赋默认值，这样在楼层面积计算时不会报错了
	if(0 == _tcscmp(szCode, _T("410212")))
	{
		AddXdataDouble(aname(objLast), _T("MJXS"), 1.0);
		AddXdataShort(aname(objLast), _T("ZZLC"), 1);
		AddXdataShort(aname(objLast), _T("QSLC"), 1);
		AddXdataShort(aname(objLast), _T("CBJ"), 0);
	}

	//为了避免客户把当前颜色设置成其它颜色，绘制出来颜色跟符号实际颜色不同，这里绘制完后把实体颜色恢复成随层
	setColorsEx2(objLast);

	TCHAR szTable[500] = {0};
	g_index.get_table(szCode, szTable);

	//used to debug
	//AfxMessageBox("111");

	if(false == findThingsInfo(szCode)) return false;

	//如果实体有必填字段
	if(m_sDrawThings.size() > 0)
	{
		CAcModuleResourceOverride myResources;

		CDrawThingsDlg drawDlg;
		drawDlg.m_sDrawThings.swap(m_sDrawThings);
		drawDlg.m_strTableName = szTable;
		drawDlg.m_strCode = szCode;
		if(IDOK != drawDlg.DoModal()) return false;

		for(size_t t = 0; t < drawDlg.m_vFields.size(); t ++)
		{
			ATTRFIELD field = drawDlg.m_vFields.at(t);

			//这里要特别处理一下，如果一些字符串值是类似“编码 值”的格式，则只提取编码写进去就行了
			CStringArray strArr;
			AnalyzeData(strArr, field.text, _T(" "));
			if(strArr.GetSize() >= 2) 
			{
				if(CheckDigital(strArr.GetAt(0)))
				{
					_stprintf	(field.text, _T("%s"), strArr.GetAt(0));
				}
			}

			AddXdata(aname(objLast), field.name, field.type, field.text);
		}

		m_sDrawThings.swap(drawDlg.m_sDrawThings);
	}

	//如果实体绘制完后要手动注记，一般用于竣工业务
	if(m_bNote)
	{
		ads_point pt;
		if(RTCAN == ads_getpoint(NULL, _T("\n 请指定注记位置:"), pt)) return false;
		AcDbObjectId objBlock = DrawInsert(pt, m_strNoteName);
		if(objBlock == AcDbObjectId::kNull)
		{
			acutPrintf(_T("\n 插入块失败！%s.dwg"), m_strNoteName);
			return false;
		}

		if(m_sDrawThings.size())
		{
			SBreakBlock breakB;
			breakB.SetBlock(objBlock);
			breakB.Explode();
			for(size_t t = 0; t < m_sDrawThings.size(); t ++)
			{
				TCHAR szText[100] = {0};
				_stprintf(szText, _T("#%s#"), m_sDrawThings.at(t).strName);
				if(m_sDrawThings.at(t).strValue != _T(""))
					breakB.replaceTextEx(szText, m_sDrawThings.at(t).strValue);
				else
					breakB.replaceTextEx(szText, _T(" "));
			}

			//为了避免客户把当前颜色设置成其它颜色，绘制出来颜色跟符号实际颜色不同，这里绘制完后把实体颜色恢复成随层
			setColorsEx2(breakB.m_objIdArr, 2);
		}
	}

	return true;
}

bool	CContructsOperate::setNoteStyle(AcDbObjectId objId, const DrawThingsData &data)
{
	
	vector<AcDbObjectId> vObj;
	GetGroupEnts(objId, vObj);
	if(vObj.size() > 0)		//情况一，注记与实体在同一编组
	{
		for(size_t t = 0; t < vObj.size(); t ++)
		{
			ads_name ent;
			if(Acad::eOk != acdbGetAdsName(ent, vObj.at(t))) continue;

			TCHAR szText[200] = {0};
			GetEntText(ent, szText);
			if(0 == _tcscmp(szText, _T(""))) continue;

			SetTextHeight(ent, data.dTextHeight * m_dhh);
			setTextWidthFactor(vObj.at(t), data.dTextWidth);
			SetTextStyle(vObj.at(t), data.strTextStyle);
		}
	}
	else							//情况二，注记与实体不在同一编组，但插入点在实体里面
	{
		AcGePoint2dArray nodes;
		GetPlList(aname(objId), nodes);
		AcDbObjectIdArray objArr;
		ssFromNodes(objArr, nodes, 2, 0.0, _T("*TEXT*"));
		for(int i = 0; i < objArr.length(); i ++)
		{
			ads_name ent;
			if(Acad::eOk != acdbGetAdsName(ent, objArr.at(i))) continue;

			TCHAR szText[200] = {0};
			GetEntText(ent, szText);
			if(0 == _tcscmp(szText, _T(""))) continue;

			ads_point pt;
			GetTextPos(ent, pt);
			if(PointInPolygon(nodes, apoint(pt)) != -1) continue;

			SetTextHeight(ent, data.dTextHeight * m_dhh);
			setTextWidthFactor(objArr.at(i), data.dTextWidth);
			SetTextStyle(objArr.at(i), data.strTextStyle);

		}
	}
	


	return true;
}



bool CContructsOperate::noteCoord_Total_Floor_Plan()
{
	CFileDialog openFD(TRUE, NULL, NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("excel坐标文件 (*.xls;*.xlsx;)|*.xls;*.xlsx;||"));
	if (openFD.DoModal() != IDOK) return false;
	CString strPath = openFD.GetPathName();

	CTinyExcel excel;
	excel.SetTemplete(strPath);
	if(excel.StartExcel() == false) return false;

	long lRow = 0, lCol = 0;
	int nStartRow = 5, nCol0 = 1, nCol1 = 2, nCol2 = 3, nCol3 = 6, nCol4 = 7, nCol5 = 12;
	excel.GetRowColCount(lRow, lCol);
	CString strText, strName;
	vector<Parsing_Detecting> vData;
	for(int i = nStartRow; i <= lRow; i ++)
	{
		strText = excel.GetRangeContent(i, nCol0, excel.m_range	);
		TrimString_Space(strText);
		if(strText == _T("备注")) break;

		//如果是合并行跳过
 		if(excel.IsCellMerged(i, nCol0)) continue;

		strName = strText;

		strText = excel.GetRangeContent(i, nCol5, excel.m_range	);
		TrimString_Space(strText);

		if(strText == _T("")) 
		{
			acutPrintf(_T("\n 第 %d 行记录没有点名！"), i);
			continue;
		}

		//只处理条件点跟放线点，其它都不处理
		if(strText != _T("条件点") && strText != _T("放线点")) continue;

		if(getDecimalPrec(excel.GetRangeContent(i, nCol2, excel.m_range)) > 3 ||
			getDecimalPrec(excel.GetRangeContent(i, nCol1, excel.m_range)) > 3 ||
			getDecimalPrec(excel.GetRangeContent(i, nCol4, excel.m_range)) > 3 ||
			getDecimalPrec(excel.GetRangeContent(i, nCol3, excel.m_range)) > 3)
		{
			acutPrintf(_T("\n 第 %d 行记录发现有小数位数超过3位！已忽略！"), i);
			continue;
		}

		//注意这里X,Y的位置与模板要对应
		Parsing_Detecting data;
		data.strName = strName;
		if(strText == _T("条件点")) data.sType = 1;
		else data.sType = 2;
		data.dX1 = _tstof(excel.GetRangeContent(i, nCol2, excel.m_range));
		data.dY1 = _tstof(excel.GetRangeContent(i, nCol1, excel.m_range));
		data.dX2 = _tstof(excel.GetRangeContent(i, nCol4, excel.m_range));
		data.dY2 = _tstof(excel.GetRangeContent(i, nCol3, excel.m_range));

		if(data.dX2 == 0.0 && data.dY2 == 0.0)
			data.sType2 = 2;
		else
			data.sType2 = 1;

		vData.push_back(data);
		

	}

	excel.CloseWorkBook(excel.m_workbook);
	excel.Quit();

	if(vData.size() == 0) return false;

	int nCount = 0;
	for(size_t t = 0; t < vData.size(); t ++)
	{
		//注意这里插入点的XY值是调转的
		Parsing_Detecting data = vData.at(t);
		ads_point pt = {data.dY1, data.dX1, 0.0};
		TCHAR szName[200] = {0};
		if(data.sType == 1)
		{
			if(data.sType2 == 2)
				_tcscpy(szName, _T("总平图_建筑外围坐标_条件点_2"));
			else
				_tcscpy(szName, _T("总平图_建筑外围坐标_条件点"));
		}
		else
		{
			if(data.sType2 == 2)
				_tcscpy(szName, _T("总平图_建筑外围坐标_放线点_2"));
			else
				_tcscpy(szName, _T("总平图_建筑外围坐标_放线点"));
		}
		AcDbObjectId objBlock = DrawInsert(pt, szName);
		if(objBlock == AcDbObjectId::kNull)
		{
			acutPrintf(_T("插入块失败！ %s"), szName);
			continue;
		}
		SBreakBlock breakB;
		breakB.SetBlock(objBlock);
		breakB.Explode();
		breakB.replaceTextEx(_T("#NAME#"), data.strName);
		strText.Format(_T("%.3lf"), data.dX1);
		breakB.replaceTextEx(_T("#X1#"), strText);
		strText.Format(_T("%.3lf"), data.dX2);
		breakB.replaceTextEx(_T("#X2#"), strText);
		strText.Format(_T("%.3lf"), data.dY1);
		breakB.replaceTextEx(_T("#Y1#"), strText);
		strText.Format(_T("%.3lf"), data.dY2);
		breakB.replaceTextEx(_T("#Y2#"), strText);

		AcDbObjectId objLine, objMText;
		for(int i = 0; i < breakB.m_objIdArr.length(); i ++)
		{
			TCHAR szStdm[100] = {0};
			GetStdm(aname(breakB.m_objIdArr.at(i)), szStdm);
			
			if(0 == _tcscmp(szStdm, CASSCODE_999999))
			{
				objLine = breakB.m_objIdArr.at(i);
			}
			TCHAR szType[200] = {0};
			GetEntityType(aname(breakB.m_objIdArr.at(i)), szType);
			if(0 == _tcsicmp(szType, _T("MTEXT")))
			{
				objMText = breakB.m_objIdArr.at(i);
			}
			
		}
		if(objLine == AcDbObjectId::kNull || objMText == AcDbObjectId::kNull) continue;
		ads_point pt1;
		//GetTextExtents(aname(objMText), pt1, pt2, pt3, pt4);
		GetTextPos(aname(objMText), pt1);
		GetEntText(aname(objMText), szName);
		//double dWidth = GetTextHeight(aname(objMText));

		AcDbLine *pLine;
		if(Acad::eOk != acdbOpenObject(pLine, objLine, AcDb::kForWrite))  continue;
		AcGePoint3d ptEnd = pLine->endPoint();
		int nCountUnUsed = data.sType2 == 1? 12 : 6;
		double dLength = (_tcslen(szName) - nCountUnUsed + 2) * 0.7;
		ptEnd.x = pt1[X] + dLength;
		pLine->setEndPoint(ptEnd);
		pLine->close();

		nCount++;
	}
	
	acutPrintf(_T("\n 共标注了 %d 条记录！"), nCount);


	return true;
}

bool	CContructsOperate::extentBuildings()
{

	CAcModuleResourceOverride myResources;
	
	if(g_extentBuildingDlg == NULL)
	{
		g_extentBuildingDlg = new CExtentBuildingDlg();
		if(g_extentBuildingDlg->GetSafeHwnd() == NULL)
		{
			g_extentBuildingDlg->Create(IDD_DIALOG_EXTENT_BUILDING);
			g_extentBuildingDlg->ShowWindow(SW_SHOW);
			g_extentBuildingDlg->CenterWindow();
		}
	}
	else
	{
		g_extentBuildingDlg->ShowWindow(SW_SHOW);
	}


	return true;
}


bool	CContructsOperate::noteTextNote()
{
	CAcModuleResourceOverride myResources;
	
	CIndoorSurveyNoteDlg *noteDlg = new CIndoorSurveyNoteDlg();
	if(noteDlg->GetSafeHwnd() == NULL)
	{
		noteDlg->Create(IDD_DIALOG_INDOORSURVEY_NOTE);
		noteDlg->ShowWindow(SW_SHOW);
		noteDlg->CenterWindow();
	}
// 
// 	CIndoorSurveyNoteDlg noteDlg;
// 
// 	if(noteDlg.DoModal()!= IDOK) return false;


	return true;
}


bool	CContructsOperate::noteTextNote2()
{
	CAcModuleResourceOverride myResources;
	
	CIndoorSurveyNote_Standard2_Dlg *noteDlg = new CIndoorSurveyNote_Standard2_Dlg();
	if(noteDlg->GetSafeHwnd() == NULL)
	{
		noteDlg->Create(IDD_DIALOG_INDOORSURVEY_NOTE_STANDARD2);
		noteDlg->ShowWindow(SW_SHOW);
		noteDlg->CenterWindow();
	}

	return true;
}

bool	CContructsOperate::noteAttachment()
{
	double dh = 0.0;
	read1(dh); dh /= 1000.0;

	AcDbObjectIdArray objArr, objArrRes;
	acutPrintf(_T("\n 请选择要注记的附属物："));
	CSelect selectT;
	selectT.SelectObjects2(objArr, 2, _T("*LINE*"), _T("*"), _T(""));
	if(objArr.length() == 0) return false;

	bool bReslt = readNoteTextConfig_Indoor();

	for(int i = 0; i < objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0}, szTable[200] = {0};
		GetStdm(aname(objArr.at(i)), szStdm);
		g_index.get_table(szStdm, szTable);

		DrawThingsData data;
		if(false == findThingsInfo(szStdm, data)) continue;
		if(data.strNote != _T("1") && data.strNote != _T("2")) continue;

		//直接注记
		if(data.strNote == _T("1")) 
		{
			if(getNoteInsertPoint(objArr.at(i)) == false) continue;

			//棚房比较特殊，虽然在FW层上，但也要注记
 			TCHAR szTempName[300] = {0};
// 			if(0 == _tcscmp(szTable, _T("FSWM")) || 0 == _tcscmp(szStdm, CASSCODE_HUTS))
// 				_tcscpy(szTempName, _T("入户调查图_附属物名称注记"));
// 			else if(0 == _tcscmp(szTable, _T("FSWL")))
// 				_tcscpy(szTempName, _T("入户调查图_附属线名称注记"));
// 			else continue;
			if(0 == _tcscmp(szTable, _T("FSWL")))
				_tcscpy(szTempName, _T("入户调查图_附属线名称注记"));
			else
				_tcscpy(szTempName, _T("入户调查图_附属物名称注记"));

			AcDbObjectId objBlock = DrawInsert(apoint(m_ptNoteInsertPoint), szTempName, dh, dh);
			if(AcDbObjectId::kNull == objBlock)
			{
				acutPrintf(_T("\n 插入块失败！ %s.dwg"), szTempName);
				continue;
			}

			SBreakBlock breakB;
			breakB.SetBlock(objBlock);
			breakB.Explode();
			breakB.replaceText(_T("#NAME#"), data.strNoteText);

			if(0 == _tcscmp(szStdm, CASSCODE_HUTS))
				setlayers(breakB.m_objIdArr, _T("JMD"));

		}
		else		//更新注记
		{
			setNoteStyle(objArr.at(i), data);
		}
	}

	//FW层
// 	ads_name ssHuts;
// 	IdArrToSS(objArr, ssHuts, _T("$141500$"));
// 	long len = 0;
// 	ads_sslength(ssHuts, &len);
// 
// 	//FSWM
// 	filterObjectIdArr_ByGISTable(objArr, objArrRes, _T("FSWM"), true);
// 	
// 	if(objArrRes.length() == 0)
// 		acutPrintf(_T("\n FSWM层没有地物！"));
// 
// 	//FW层追加到FSWM层中一起注记
// 	for(long l = 0; l < len; l ++)
// 	{
// 		ads_name ent;
// 		ads_ssname(ssHuts, l, ent);
// 		objArrRes.append(aname(ent));
// 	}

return true;

	for(int i = 0; i < objArrRes.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(objArrRes.at(i)), szStdm);

		DrawThingsData data;
		if(false == findThingsInfo(szStdm, data)) continue;

		if(data.strNote != _T("1")) continue;

		if(getNoteInsertPoint(objArrRes.at(i)) == false) continue;

		AcDbObjectId objBlock = DrawInsert(apoint(m_ptNoteInsertPoint), _T("入户调查图_附属物名称注记"), dh, dh);
		if(AcDbObjectId::kNull == objBlock)
		{
			acutPrintf(_T("\n 插入块失败！ 入户调查图_附属物名称注记.dwg"));
			continue;
		}

		SBreakBlock breakB;
		breakB.SetBlock(objBlock);
		breakB.Explode();
		breakB.replaceText(_T("#NAME#"), data.strNoteText);
	}

	//FSWX
	filterObjectIdArr_ByGISTable(objArr, objArrRes, _T("FSWL"), true);
	for(int i = 0; i < objArrRes.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(objArrRes.at(i)), szStdm);

		DrawThingsData data;
		if(false == findThingsInfo(szStdm, data)) continue;

		if(data.strNote != _T("1")) continue;

		if(getNoteInsertPoint_Line(objArrRes.at(i)) == false) continue;

		AcDbObjectId objBlock = DrawInsert(apoint(m_ptNoteInsertPoint), _T("入户调查图_附属线名称注记"),
			dh, dh, 1.0, m_dAngle);

		if(AcDbObjectId::kNull == objBlock)
		{
			acutPrintf(_T("\n 插入块失败！ 入户调查图_附属线名称注记.dwg"));
			continue;
		}

		SBreakBlock breakB;
		breakB.SetBlock(objBlock);
		breakB.Explode();
		breakB.replaceText(_T("#NAME#"), data.strNoteText);
	}
	
	return true;
}



bool	CContructsOperate::noteFenceNote()
{
	double hh = 0.0;
	read1(hh); hh /= 1000.0;

	ads_name ent;
	ads_point pt;
	int es = RTNORM;
	while(1)
	{
		es = ads_entsel(_T("\n 选择围墙,栅栏,石堰"), ent, pt);
		if(es == RTCAN) return false;
		TCHAR szStdm[100] = {0};
		GetStdm(ent, szStdm);
		if(0 != _tcscmp(szStdm, _T("144311")) && 0 != _tcscmp(szStdm, _T("144410"))
			&& 0 != _tcscmp(szStdm, _T("184603")) && 0 != _tcscmp(szStdm, _T("184613")))
		{
			acutPrintf(_T("\n 请选择围墙！（cass编码：144311 或 144410 或 184603 或 184613）"));
			continue;
		}
		es = ads_getpoint(NULL, _T("n 请点击注记位置"), pt);
		if(es == RTCAN) return false;

		double dheight = 0.0;
		ReadXdataDouble(ent, _T("GD"), dheight);
		double dLength = 0.0;
		GetEntLength(ent, dLength);

		TCHAR szName[200] = {0};
		if(0 == _tcscmp(szStdm, _T("144311")) || 0 == _tcscmp(szStdm, _T("144410")))
			_tcscpy(szName, _T("入户调查图_围墙注记"));
		else
			_tcscpy(szName, _T("入户调查图_石堰注记"));

		AcDbObjectId objNote = DrawInsert(pt, szName, hh, hh);
		if(AcDbObjectId::kNull == objNote)
		{
			acutPrintf(_T("\n 插入块失败！ %s.dwg"), szName);
			return false;
		}

		SBreakBlock breakB;
		breakB.SetBlock(objNote);
		breakB.Explode();

		TCHAR szText[100] = {0};
		_stprintf(szText, _T("%.2lf"), dLength);
		breakB.replaceText(_T("#LENGTH#"), szText);
		AddXdata(ent, _T("ZJCD"), STRING_TYPE, szText);
		_stprintf(szText, _T("%.2lf"), dheight);				
		breakB.replaceText(_T("#HEIGHT#"), szText);
		g_index.get_name(szStdm, szName);

		if(0 == _tcscmp(szStdm, _T("144311")))
		{
			TCHAR szWQCZ[100] = {0};
			ReadXdata(ent, _T("WQCZ"), STRING_TYPE, szWQCZ);
			if(0 == _tcscmp(szWQCZ, _T("4")))
				_tcscpy(szName, _T("土坯墙"));
			else
				_tcscpy(szName, _T("砖墙"));
		}
		breakB.replaceText(_T("#NAME#"), szName);
		
	}
	return true;
}

bool	CContructsOperate::getNoteInsertPoint_Line(AcDbObjectId objPolyLine)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(objPolyLine), nodes);
	if(nodes.length() < 2) return false;
	
	double dMaxDist = distOF2d(nodes.first(), nodes.at(1));
	int nIndex = 0;
	for(int i = 0; i < nodes.length() - 1; i ++)
	{
		double dDist = distOF2d(nodes.at(i), nodes.at(i + 1));
		if(dDist > dMaxDist)
		{
			nIndex = i;
			dMaxDist = dDist;
		}
	}
	
	m_dAngle = ads_angle(apoint(nodes.at(nIndex)), apoint(nodes.at(nIndex + 1)));
	m_ptNoteInsertPoint = apoint(GetMiddlePoint(apoint(nodes.at(nIndex)), apoint(nodes.at(nIndex + 1))));


	return true;
}


bool CContructsOperate::getNoteInsertPoint(AcDbObjectId objBuild)
{
	vector<AcDbObjectId> vObj;
	GetGroupEnts(objBuild, vObj);
	bool bFind = false;
	//正常情况，cass绘制出来的根据编组获取里面的注记文字
	for(size_t t = 0; t < vObj.size(); t ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(vObj.at(t)), szStdm);
		CString strText = szStdm;
		if(strText.Find(_T("-")) >= 0)
		{
			GetEntText(aname(vObj.at(t)), szStdm);
			if(0 != _tcscmp(szStdm, _T("")))
			{
				ads_point pt1, pt2, pt3, pt4;
				GetTextExtents(aname(vObj.at(t)), pt1, pt2, pt3, pt4);
				m_ptNoteInsertPoint = apoint(GetMiddlePoint(apoint(pt1), apoint(pt2)));
				bFind = true;
				break;
			}
		}
	}

	//如果获取编组失败，则框选获取里面的注记文字
	if(bFind == false)
	{
		AcDbObjectIdArray objArr;
		AcGePoint2dArray nodes;
		GetPlList(aname(objBuild), nodes);
		ssFromNodes(objArr, nodes, 1, 0.0, _T("*TEXT*"), NULL);
		for(int i = 0; i < objArr.length(); i ++)
		{
			TCHAR szStdm[100] = {0};
			GetStdm(aname(objArr.at(i)), szStdm);
			CString strText = szStdm;
			if(strText.Find(_T("-")) >= 0)
			{
				GetEntText(aname(objArr.at(i)), szStdm);
				if(0 != _tcscmp(szStdm, _T("")))
				{
					ads_point pt1, pt2, pt3, pt4;
					GetTextExtents(aname(objArr.at(i)), pt1, pt2, pt3, pt4);
					m_ptNoteInsertPoint = apoint(GetMiddlePoint(apoint(pt1), apoint(pt2)));
					bFind = true;
					break;
				}
			}
		}

		//如果范围内找不到编码注记，则通过cass编码的要素名称来判断它是不是注记
		if(bFind == false)
		{
			for(int i = 0; i < objArr.length(); i ++)
			{
				TCHAR szStdm[100] = {0}, szNote[500] = {0};
				GetStdm(aname(objArr.at(i)), szStdm);
				g_index.get_name(szStdm, szNote);
				CString strText = szNote;
				if(strText.Find(_T("注记")) >= 0)
				{
					ads_point pt1, pt2, pt3, pt4;
					GetTextExtents(aname(objArr.at(i)), pt1, pt2, pt3, pt4);
					m_ptNoteInsertPoint = apoint(GetMiddlePoint(apoint(pt1), apoint(pt2)));
					bFind = true;
					break;
				}
			}
			

		}
	}

	//框选也获取失败，则直接获取建筑的内部最优点
	if(bFind == false)
	{
		getFitPoint(objBuild);
	}

	return true;
}

bool	CContructsOperate::getFitPoint(AcDbObjectId objBuild)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(objBuild), nodes);

	SNodesOpe nodesO;
	if(nodesO.isConcavePoly(nodes))
	{
		AcDbObjectIdArray objArr;
		m_ptNoteInsertPoint = CRegionFitPt::GetFitPoint(aname(objBuild), objArr);
	}
	else
	{
		ads_point pt;
		GetNodesMiddlePoint(nodes, pt);
		m_ptNoteInsertPoint = apoint(pt);
	}

	return true;
}


bool	CContructsOperate::noteIndoorsHeight()
{
	double hh = 0.0;
	read1(hh); hh /= 1000.0;

	ads_point pt;
	ads_real dValue = 0.0;

	while(1)
	{
		int es = ads_getpoint(NULL, _T("\n 请指定室内坪位置"), pt);
		if(es == RTCAN) break;

		ads_name entBuild;
		RegionSel_Auto(entBuild, pt, _T("*"), _T(""), _T("*LINE*"));

		es = ads_getreal(_T("\n 请输入值: \n"), &dValue);
		if(es == RTCAN) break;

		AcDbObjectId objId = DrawInsert(pt, _T("室内坪注记_模板"), hh, hh);
		if(objId == AcDbObjectId::kNull)
		{
			acutPrintf(_T("\n 插入块失败！室内坪注记_模板"));
			return false;
		}
		TCHAR szText[300] = {0};
		_stprintf(szText, _T("%.2lf"), dValue);

		SBreakBlock breakB;
		breakB.SetBlock(objId);
		breakB.Explode();
		breakB.replaceText(_T("#HEIGHT#"), szText);

		if(acdbGetObjectId(objId, entBuild) == Acad::eOk)
		{
			AddXdata(entBuild, XDATA_ENTITY_TYPE, INT_TYPE, _T("4"));
			breakB.m_objIdArr.append(objId);
			BuildGroup(breakB.m_objIdArr, newGUID2());
		}
		
	}

	return true;
}

bool	CContructsOperate::noteBaseRoom(AcDbObjectId objId)
{
	AcDbObjectId objNote = DrawInsert(apoint(m_ptNoteInsertPoint), _T("入户调查图_面积注记_2_地下室"), 
		m_dhh, m_dhh);
	if(AcDbObjectId::kNull == objNote)
	{
		acutPrintf(_T("\n 插入块失败！ 入户调查图_面积注记_2_地下室.dwg"));
		return false;
	}

	double dArea = 0.0;
	GetEntArea(aname(objId), dArea);
	double dMJXS = 1.0;
	ReadXdataDouble(aname(objId), _T("MJXS"), dMJXS);
	if(dMJXS == 1.0)
	{
		int nFloor = GetBuildFloor(aname(objId));
		dArea *= nFloor;
	}
	dArea *= dMJXS;

	double dDeep = 0.0;
	ReadXdataDouble(aname(objId), _T("SD"), dDeep);

	SBreakBlock breakB;
	breakB.SetBlock(objNote);
	breakB.Explode();

	TCHAR szText[100] = {0};
	_stprintf(szText, _T("%.2lf"), dArea);				
	breakB.replaceText(_T("#AREA#"), szText);	
	AddXdata(aname(objId), _T("ZJMJ"), STRING_TYPE, szText);
	_stprintf(szText, _T("%.2lf"), dDeep);				
	breakB.replaceText(_T("#SD#"), szText);

	return true;
}




bool	CContructsOperate::noteIndoorFrame()
{
	double hh = 0.0;
	read1(hh); hh /= 1000.0;
	m_dhh = hh;

	acutPrintf(_T("\n 请按顺序选择要对房屋进行面积的图框范围线："));
	AcDbObjectIdArray objArrBorder;
	CSelect select;
	select.SelectObjects2(objArrBorder, 2, _T("*LINE*"), _T(""), _T(""));

	int nCount = 1;
	for(int k = 0; k < objArrBorder.length(); k++)
	{
		AcGePoint2dArray nodes;
		if(false == GetPlList(aname(objArrBorder.at(k)), nodes)) continue;
		AcDbObjectIdArray objArr;
		ssFromNodes(objArr, nodes, 1, 1.0, _T("*LINE*"));
		orderObjects(objArr);


		for(int i = 0; i < objArr.length(); i ++)
		{
			TCHAR szStdm[100] = {0};
			GetStdm(aname(objArr.at(i)), szStdm);
			TCHAR szTable[100] = {0};
			g_index.get_table(szStdm, szTable);
			if(0 == _tcscmp(szTable, _T("FW")))
			{
				getNoteInsertPoint(objArr.at(i));
				AcDbObjectId objNote = DrawInsert(apoint(m_ptNoteInsertPoint), _T("入户调查图_面积注记"), 
					hh, hh);
				if(AcDbObjectId::kNull == objNote)
				{
					acutPrintf(_T("\n 插入块失败！ 入户调查图_面积注记.dwg"));
					return false;
				}

				//房屋面积计算规则：单层面积*面积系数
				//如果面积系数为1，则房屋面积 = 房屋占地面积 * 层数 * 系数
				//如果不为1，如2.5，则房屋面积 = 占地面积 * 系数
				double dArea = 0.0;
				GetEntArea(aname(objArr.at(i)), dArea);
				double dMJXS = 1.0;
				ReadXdataDouble(aname(objArr.at(i)), _T("MJXS"), dMJXS);
				if(dMJXS == 1.0)
				{
					int nFloor = GetBuildFloor(aname(objArr.at(i)));
					dArea *= nFloor;
				}
				dArea *= dMJXS;
						

				SBreakBlock breakB;
				breakB.SetBlock(objNote);
				breakB.Explode();

				TCHAR szText[100] = {0};
				_stprintf(szText, _T("%d"), nCount);
				breakB.replaceText(_T("#NUM#"), szText);
				_stprintf(szText, _T("%.2lf"), dArea);				
				breakB.replaceText(_T("#AREA#"), szText);	
				AddXdata(aname(objArr.at(i)), _T("ZJMJ"), STRING_TYPE, szText);

				AddXdataShort(aname(objArr.at(i)), _T("FWBH"), nCount);
				nCount++;
			}
			else if(0 == _tcscmp(szTable, _T("FSWM")))
			{
				getNoteInsertPoint(objArr.at(i));

				//地下室比较特殊，虽然注记的是面积，但还要注记深度
				if(0 == _tcscmp(szStdm, _T("141170")))
				{
					noteBaseRoom(objArr.at(i));
					continue;
				}

				double dDeep = 0.0;
				ReadXdataDouble(aname(objArr.at(i)), _T("SD"), dDeep);
				double dArea = 0.0;
				GetEntArea(aname(objArr.at(i)), dArea);

				AcDbObjectId objNote;
				if(dDeep > 0.0)
				{
					objNote = DrawInsert(apoint(m_ptNoteInsertPoint), _T("入户调查图_体积注记"), 
						hh, hh);
					if(AcDbObjectId::kNull == objNote)
					{
						acutPrintf(_T("\n 插入块失败！ 入户调查图_体积注记.dwg"));
						return false;
					}

					dArea *= dDeep;

					SBreakBlock breakB;
					breakB.SetBlock(objNote);
					breakB.Explode();
					TCHAR szText[100] = {0};
					_stprintf(szText, _T("%.2lf"), dDeep);				
					breakB.replaceText(_T("#SD#"), szText);
					_stprintf(szText, _T("%.2lf"), dArea);				
					breakB.replaceText(_T("#VOLUMN#"), szText);
					AddXdata(aname(objArr.at(i)), _T("ZJTJ"), STRING_TYPE, szText);

				}
				else
				{
					objNote = DrawInsert(apoint(m_ptNoteInsertPoint), _T("入户调查图_面积注记_2"), 
						hh, hh);
					if(AcDbObjectId::kNull == objNote)
					{
						acutPrintf(_T("\n 插入块失败！ 入户调查图_体积注记_2.dwg"));
						return false;
					}

					double dMJXS = 1.0;
					ReadXdataDouble(aname(objArr.at(i)), _T("MJXS"), dMJXS);
					dArea *= dMJXS;

					SBreakBlock breakB;
					breakB.SetBlock(objNote);
					breakB.Explode();
					TCHAR szText[100] = {0};
					_stprintf(szText, _T("%.2lf"), dArea);				
					breakB.replaceText(_T("#AREA#"), szText);
					AddXdata(aname(objArr.at(i)), _T("ZJMJ"), STRING_TYPE, szText);
				}

			}
		}

	}
	


	return true;
}

CContructsOperate::CContructsOperate(void)
{
	m_dAngle = 0.0;
	m_bNote = false;
	m_dhh = 0.5;
}


CContructsOperate::~CContructsOperate(void)
{
}
