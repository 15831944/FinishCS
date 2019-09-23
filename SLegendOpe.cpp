#include "stdafx.h"
#include "SLegendOpe.h"

bool SLegendOpe::readLegendConfig(TCHAR *szFileName)
{
	TCHAR szPath[1000] = {0};
	ads_findfile(szFileName, szPath);
	if(0 == _tcscmp(szPath, _T(""))) 
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szFileName);
		return false;
	}

	TiXmlDocument tinyXml;
	char szTemp1[1000] = {0};
	bool es = false;
#ifdef _UNICODE
	TcharToChar(szPath, szTemp1);
	es = tinyXml.LoadFile(szTemp1, TIXML_DEFAULT_ENCODING );
#else
	//XML文件最后保存成UTF-8格式，然后内部编码采用UTF-8编码，不然会读取中文乱码
	_tcscpy(szTemp1,szPath);
	es = tinyXml.LoadFile(szTemp1, TIXML_ENCODING_UTF8 );
#endif

	if (es == false)
	{
		acutPrintf(_T("\n读配置文件失败: %s"), szFileName);
		return false;
	}

	TiXmlElement* pRoot = tinyXml.FirstChildElement( "Legend_Info" );
	if ( NULL == pRoot )
		return false;

	CEditListCtrl editList;
	TiXmlElement *pBody = pRoot->FirstChildElement("Body");
	if(pBody == NULL) return false;
	dWidth_Block = _tstof(editList.GetXMLAttribute_EditList(pBody, ("width_Block")));
	dHeight_block = _tstof(editList.GetXMLAttribute_EditList(pBody, ("height_block")));
	m_scaleBlock = _ttoi(editList.GetXMLAttribute_EditList(pBody, ("scaleBlock")));
	nInterval_width = (int)_tstof(editList.GetXMLAttribute_EditList(pBody, ("width_Interval")));
	nInterval_Height = (int)_tstof(editList.GetXMLAttribute_EditList(pBody, ("height_Interval")));
	nArrangeType = _ttoi(editList.GetXMLAttribute_EditList(pBody, ("arrangeType")));
	nMaxCount = _ttoi(editList.GetXMLAttribute_EditList(pBody, ("max_count")));

	TiXmlElement *pBorder = pRoot->FirstChildElement("Border");
	if(pBorder == NULL) return false;
	dFrameWidth_Gap = _tstof(editList.GetXMLAttribute_EditList(pBorder, ("width_Gap")));
	dFrameHeight_Gap = _tstof(editList.GetXMLAttribute_EditList(pBorder, ("height_Gap")));
	dWidth_Frame = _tstof(editList.GetXMLAttribute_EditList(pBorder, ("lineWidth")));
	_tcscpy(szBorderLineType, editList.GetXMLAttribute_EditList(pBorder, ("lineStyle")));
	sBlank = (short)_tstof(editList.GetXMLAttribute_EditList(pBorder, ("blank")));
	sBorderColor = _ttoi(editList.GetXMLAttribute_EditList(pBorder, ("color")));
	m_nDrawFrameOut = _ttoi(editList.GetXMLAttribute_EditList(pBorder, ("draw")));

	TiXmlElement *pTittle = pRoot->FirstChildElement("Tittle");
	if(pTittle == NULL) return false;
	_tcscpy(szLegendTittleName, editList.GetXMLAttribute_EditList(pTittle, ("name")));
	dWidth_Tittle_Temp = _tstof(editList.GetXMLAttribute_EditList(pTittle, ("width")));

	TiXmlElement *Buttom = pRoot->FirstChildElement("Buttom");
	if(Buttom == NULL) return false;
	_tcscpy(szLegendButtomName, editList.GetXMLAttribute_EditList(Buttom, ("name")));
	dWidth_Buttom_Temp = _tstof(editList.GetXMLAttribute_EditList(Buttom, ("width")));

	TiXmlElement *InsertPosition = pRoot->FirstChildElement("InsertPosition");
	sInsertPosition = _ttoi(editList.GetXMLAttribute_EditList(InsertPosition, ("type")));

	return true;
}

bool	SLegendOpe::filterUseLegendBlock()
{

	for(set<CString>::iterator it = m_sCodes.begin(); it != m_sCodes.end(); it ++)
	{
		TCHAR szName[500] = {0};
		_stprintf(szName, _T("Legend_%s.dwg"), *it);
		TCHAR szPath[1000] = {0};
		ads_findfile(szName, szPath);
		if(0 == _tcscmp(szPath, _T("")))
		{
#ifdef _DEBUG
			acutPrintf(_T("\n cass编码 ：%s 找不到对应的图例模板！"), *it);
#endif
			continue;
		}

		_stprintf(szName, _T("Legend_%s"), *it);
		m_vStrBlockNames.push_back(szName);
	}

	return true;
}


bool		SLegendOpe::insertLegendTable(AcGePoint2d ptInsert)
{
	
	filterUseLegendBlock();

	if(m_vStrBlockNames.size() == 0)
	{
		acutPrintf(_T("\n 没有可生成图例的实体！生成图例失败！"));
		return false;
	}
	
	//按配置文件的排列规则插入图例小块
	AcDbObjectIdArray objArrBlocks;
	AcGePoint2d ptTemp = ptInsert;
	int nIndex = 0;
	AcDbObjectId objIdBlockBorder = DrawInsert(apoint(ptTemp), _T("Legend_BlockBorder"));
	SBreakBlock blockB;
	blockB.SetBlock(objIdBlockBorder);
	blockB.Explode();
	if(false == blockB.findTable(objIdBlockBorder)) return false;
	AcDbTable *pTable;
	if(Acad::eOk != acdbOpenObject(pTable, objIdBlockBorder, AcDb::kForWrite)) return false;
	pTable->setColumnWidth(0, dWidth_Block);
	pTable->setRowHeight(0, dHeight_block);
	pTable->close();
	blockB.m_objIdArr.setLogicalLength(0);

	if(nArrangeType == 2)		//行数固定，列数不固定
	{
		for(size_t t = 0; t < m_vStrBlockNames.size(); t ++)
		{
			//AcDbObjectId objId = DrawInsert(apoint(ptTemp), m_vStrBlockNames.at(t));
			AcDbObjectId objId = CopyEntity(objIdBlockBorder);
			if(objId == AcDbObjectId::kNull) continue;
			MoveEntity(aname(objId), ptTemp.x - ptInsert.x, ptTemp.y - ptInsert.y);
			objArrBlocks.append(objId);

			AcGePoint2d ptBlock = ptTemp;
			ptBlock.x += dWidth_Block / 2;
			ptBlock.y -= dHeight_block / 2;
			objId = DrawInsert(apoint(ptBlock), m_vStrBlockNames.at(t));

			ads_point ptmin, ptmax;
			GetEntExtent(aname(objId), ptmin, ptmax);
			if(m_scaleBlock == 1)
			{
			 	double dScale = dWidth_Block / (ptmax[X] - ptmin[X]);
			 	ScaleEntity(aname(objId), apoint(ptBlock), dScale * 0.9);
			}
			m_objArrBlocks.append(objId);
			objArrBlocks.append(objId);

			nIndex ++;
			if(nIndex < nMaxCount)
			{
				ptTemp.y -= dHeight_block;
				ptTemp.y -= nInterval_Height;

			}
			else
			{
				ptTemp.y = ptInsert.y;
				ptTemp.x += dWidth_Block;
				ptTemp.x += nInterval_width;

				nIndex = 0;
			}

		}

	}
	else		//列数固定，行数不固定
	{
		for(size_t t = 0; t < m_vStrBlockNames.size(); t ++)
		{
			AcDbObjectId objId = CopyEntity(objIdBlockBorder);
			if(objId == AcDbObjectId::kNull) continue;
			MoveEntity(aname(objId), ptTemp.x - ptInsert.x, ptTemp.y - ptInsert.y);
			objArrBlocks.append(objId);

			AcGePoint2d ptBlock = ptTemp;
			ptBlock.x += dWidth_Block / 2;
			ptBlock.y -= dHeight_block / 2;
			objId = DrawInsert(apoint(ptBlock), m_vStrBlockNames.at(t));

			//不炸开小块
			ads_point ptmin, ptmax;
			GetEntExtent(aname(objId), ptmin, ptmax);
			if(m_scaleBlock == 1)
			{
				double dScale = dWidth_Block / (ptmax[X] - ptmin[X]);
				ScaleEntity(aname(objId), apoint(ptBlock), dScale * 0.9);
			}
			m_objArrBlocks.append(objId);
			objArrBlocks.append(objId);

			nIndex ++;
			if(nIndex < nMaxCount)
			{
				ptTemp.x += dWidth_Block;
				ptTemp.x += nInterval_width;

			}
			else
			{
				ptTemp.x = ptInsert.x;
				ptTemp.y -= dHeight_block;
				ptTemp.y -= nInterval_Height;

				nIndex = 0;
			}

		}

	}


	EntDel(aname(objIdBlockBorder));
	
	blockB.m_objIdArr.append(objArrBlocks);
	m_objArrLegend.append(objArrBlocks);
// 	for(int i = 0; i < objArrBlocks.length(); i ++)
// 	{
// 		blockB.SetBlock(objArrBlocks.at(i));
// 		blockB.Explode();
// 	}

	ads_point ptmin, ptmax;
	GetEntsExtent(blockB.m_objIdArr, ptmin, ptmax);

	ptmin[X] -= dFrameWidth_Gap;ptmin[Y] -= dFrameHeight_Gap;
	ptmax[X] += dFrameWidth_Gap; ptmax[Y] += dFrameHeight_Gap;

	//插入图例头
	double dWidthTittle = ptmax[X] - ptmin[X];
	double dScaleX = dWidthTittle/ dWidth_Tittle_Temp;
	ptTemp.x = ptmin[X]; ptTemp.y = ptmax[Y];
	AcDbObjectId objTittle = DrawInsert(apoint(ptTemp), szLegendTittleName, dScaleX);
	if(objTittle != AcDbObjectId::kNull)
	{
		blockB.m_objIdArr.setLogicalLength(0);
		blockB.SetBlock(objTittle);
		blockB.Explode();
		m_objArrLegend.append(blockB.m_objIdArr);
	}

	//插入图例脚
	AcDbObjectId objButtom = DrawInsert(ptmin, szLegendButtomName, dScaleX);
	if(objButtom != AcDbObjectId::kNull)
	{
		blockB.m_objIdArr.setLogicalLength(0);
		blockB.SetBlock(objButtom);
		blockB.Explode();
		m_objArrLegend.append(blockB.m_objIdArr);
	}

	//插入图例外边框
	GetEntsExtent(m_objArrLegend, ptmin, ptmax);
	AcGePoint2dArray nodesBorder;
	SNodesOpe nodesO;
	nodesO.LT_RB_Nodes(apoint(ptmin), apoint(ptmax), nodesBorder);
	AcDbObjectId objBorder = DrawPolyline2D(nodesBorder, 0.0, true, _T(""), dWidth_Frame, 0, _T("TK"));
	setlinetype(aname(objBorder), szBorderLineType);
	setcolor(aname(objBorder), sBorderColor);
	m_objArrLegend.append(objBorder);


	//把所有图例要素移动到用户的插入点上
	AcGePoint2d ptLegendInsertPoint;
	if(sInsertPosition == 1)		//左下
	{
		ptLegendInsertPoint.x = ptmin[X]; ptLegendInsertPoint.y = ptmin[Y];
	}
	else if(sInsertPosition == 2)//左上
	{
		ptLegendInsertPoint.x = ptmin[X]; ptLegendInsertPoint.y = ptmax[Y];		
	}
	else if(sInsertPosition == 3)//右上
	{
		ptLegendInsertPoint.x = ptmax[X]; ptLegendInsertPoint.y = ptmax[Y];		
	}
	else if(sInsertPosition == 4)//右下
	{
		ptLegendInsertPoint.x = ptmax[X]; ptLegendInsertPoint.y = ptmin[Y];		
	}
	else//中间
	{
		ptLegendInsertPoint = apoint(GetMiddlePoint(apoint(ptmin), apoint(ptmax)));
	}

		//插入图例外边框消隐
	//移动消隐，把它放到最上面
	AcDbObjectId objBlank;
	if(sBlank)
	{
#if _MSC_VER > 1200
		//acedCommand(RTSTR,_T("WIPEOUT"),RTSTR,_T("P"),RTENAME,LastNamePL,RTSTR,_T("Y"),0);
		SWipeOut wipeO;
		objBlank = wipeO.WipeOut(nodesBorder);
#else
		objBlank = DrawPolyline2D(nodesBorder, 0.0, true, _T(""));
		if(objBlank != AcDbObjectId::kNull)
		{
			ads_name LastNamePL;
			acdbGetAdsName(LastNamePL, objBlank);
			acedCommand(RTSTR,_T("WIPEOUT"),RTSTR,_T("N"),RTENAME,LastNamePL,RTSTR,_T("Y"),0);
			ads_name WipeEntName; acdbEntLast(WipeEntName);
			acdbGetObjectId(objBlank, WipeEntName);
		}
#endif // _MSC_VER

		//不要在这里追加，因为下面会移动消隐，如果追加的话会移动两次
		//m_objArrLegend.append(objBlank);

		MoveEntity(aname(objBlank), ptInsert.x - ptLegendInsertPoint.x, ptInsert.y - ptLegendInsertPoint.y);
		if(m_dhh != 1.0)
			ScaleEntity(aname(objBlank), apoint(ptInsert), m_dhh);

		CLayers layer;
		layer.drawOrder2(objBlank);
	}
	//移动所有图例，把它放到最上面
	for(int i = 0; i < m_objArrLegend.length(); i ++)
	{
		MoveEntity(aname(m_objArrLegend.at(i)), ptInsert.x - ptLegendInsertPoint.x, ptInsert.y - ptLegendInsertPoint.y);
	}
	if(m_dhh != 1.0)
		ScaleEntitys(m_objArrLegend, apoint(ptInsert), m_dhh);

	CLayers layer;
	layer.drawOrder2(m_objArrLegend);

	if(sBlank) m_objArrLegend.append(objBlank);

	GetEntExtent(aname(objBorder), ptmin, ptmax);
	m_ptBorderLB = apoint(ptmin);
	m_ptBorderRT = apoint(ptmax);

	//最后，如果不要外边框，则把它删掉
	if(m_nDrawFrameOut == 0) EntDel(aname(objBorder));

	return true;

}

void	SLegendOpe::setHH(double dHH)
{
	m_dhh = dHH;
}


bool	SLegendOpe::setEntitys(AcDbObjectIdArray objArr)
{
	m_objArr.append(objArr);
	
	for(int i = 0; i < m_objArr.length(); i ++)
	{
		TCHAR szStdm[100] = {0};
		GetStdm(aname(m_objArr.at(i)), szStdm);
		if(0 != _tcscmp(szStdm, _T("")))
			m_sCodes.insert(szStdm);
	}

	//sort(m_sCodes.begin(), m_sCodes.end());
	
	return true;
}



SLegendOpe::SLegendOpe(void)
{
	m_nDrawFrameOut = 1;
	m_scaleBlock = 1;
	m_dhh = 1.0;
	m_bExplodeBlocks = false;
}


SLegendOpe::~SLegendOpe(void)
{
}
