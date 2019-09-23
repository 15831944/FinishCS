#include "stdafx.h"
#include "ExportHouseDiagram.h"
#include "HouseManagerHelper.h"
#include "SBreakBlock.h"
#include "SaveBlocks.h"
#include "MatchField.h"
#include "SimpleBuildingHelper.h"
extern Indexini g_index;
CExportHouseDiagram::CExportHouseDiagram(void)
{
	double dScale;
	read1(dScale);
	m_dH = dScale / 1000.0;
	m_str_divice = _T("PublishToWeb JPG.pc3");
	//m_str_divice = _T("PublishToWeb PNG.pc3");
	m_str_size_name = _T("A4竖");

#ifdef _NCDJ_GX
	m_TextHight1 = 2;
	m_TextHight2 = 1.5;
#else
	m_TextHight1 = 1;
	m_TextHight2 = 0.7;
#endif
	m_strFont1 = _T("宋体");
	m_strFont2 = _T("宋体");
}


CExportHouseDiagram::~CExportHouseDiagram(void)
{

}

int CExportHouseDiagram::GetNodeNum(AcDbObjectIdArray idArr)
{
	int nNum = 0;	
	for (int i = 0;i < idArr.length();i++)
	{
		AcGePoint2dArray nodes;
		GetPlList(aname(idArr.at(i)) , nodes);
		nNum += nodes.length();
	}
	return nNum;
}
void CExportHouseDiagram::NoteJZXDist_Final(AcDbObjectIdArray idArrPoly , AcDbObjectIdArray & idArrText)
{
	vector<Text_Attr> vectext;
	vectext.clear();
	acedSetStatusBarProgressMeter(_T("生成注记..."), 0, idArrPoly.length());
	for (int nIndexPoly = 0;nIndexPoly < idArrPoly.length();nIndexPoly++)
	{
		acedSetStatusBarProgressMeterPos(nIndexPoly);
		int nPolyType = GetPolyType(aname(idArrPoly.at(nIndexPoly)),true);
		if ( nPolyType < 0 ) 
		{
			continue; 
		}
		AcGePoint2dArray nodes;
		AcGeDoubleArray DbArray;

		//画附属物注记
		TCHAR szSTDM[50] = { 0 };
		GetStdm(aname(idArrPoly.at(nIndexPoly)), szSTDM);
		if (_tcscmp(szSTDM, _T("140001")) == 0)
		{
			AcGePoint2d ptMid; ads_point ptMid1;
			ptMid = getEntMiddlePoint(idArrPoly.at(nIndexPoly));
			ptMid1[X] = ptMid[X]; ptMid1[Y] = ptMid[Y]; ptMid1[Z] = 0;
			AcDbObjectId id = DrawText(ptMid1, _T("阳台"), _T("mc"), _T(""), m_TextHight1 * m_dScale * m_dH);
			SetTextStyle(id, m_strFont1.GetBuffer());
			m_strFont1.ReleaseBuffer();
			m_objArrSave.append(id);
		}

		GetPlList(aname(idArrPoly.at(nIndexPoly)) , nodes);
		GetPlBulges(aname(idArrPoly.at(nIndexPoly)) , DbArray);
		RemoveSameNodes(nodes);
		if (IsDeasil(nodes))
		{
			nodes.reverse();
		}
		nodes.append(nodes.first());
		map<AcDbObjectId , map<int ,  CString> >::iterator itFind = m_PloyDist.find(idArrPoly.at(nIndexPoly));
		if (itFind == m_PloyDist.end())
		{
			continue;
		}
		map<int ,  CString> mapNodeDist;
		mapNodeDist = itFind->second;
		for (int nIndexPt = 1;nIndexPt < nodes.length();nIndexPt++)
		{
			double dDist = distOF2d(nodes.at(nIndexPt) , nodes.at(nIndexPt - 1));
			TCHAR szDist[1000] = {0};
			_stprintf(szDist , _T("%.2f") , dDist);
		
			if (DbArray.at(nIndexPt - 1) > 0.001)
			{
				//acutPrintf(_T("\n%f,%f\n") , DbArray.at(nIndexPt - 1) , dDist);
				continue;
			}
			double dAngle = GetAngle(apoint(nodes.at(nIndexPt - 1)) , apoint(nodes.at(nIndexPt)));
			ads_point ptMid;
			ptMid[X] = (nodes.at(nIndexPt).x + nodes.at(nIndexPt - 1).x)/2;
			ptMid[Y] = (nodes.at(nIndexPt).y + nodes.at(nIndexPt - 1).y)/2;
			ptMid[Z] =0;
			//ads_polar(ptMid , PI/2 + dAngle , m_TextHight2*m_dScale*m_dH*0.8 , ptMid);
			//何育真
			ads_polar(ptMid, 3 * PI / 2 + dAngle, m_TextHight2*m_dScale*m_dH*0.8, ptMid);
			veStrDist.push_back(CString(szDist));
			map<int ,  CString>::iterator itFindDist = mapNodeDist.find(nIndexPt);
			if (itFindDist == mapNodeDist.end())
			{
				continue;
			}
			Text_Attr txt_Arr;
			if (dAngle > (PI - PI/18) && dAngle < (PI + PI/18))
			{
				AcDbObjectId idText = DrawText(ptMid , itFindDist->second.GetBuffer() , _T("MC") , _T("") , m_TextHight2*m_dH * m_dScale, dAngle+PI);
				itFindDist->second.ReleaseBuffer();
				SetTextStyle(idText , m_strFont2.GetBuffer());
				m_strFont2.ReleaseBuffer();
				//idArrText.append(idText);
				
				//存储注记相关信息
				txt_Arr.ObjId = idText;
				_stprintf(txt_Arr.szText,_T("%s"),itFindDist->second.GetBuffer());
				itFindDist->second.ReleaseBuffer();
				txt_Arr.pt1 = nodes.at(nIndexPt - 1);
				txt_Arr.pt2 = nodes.at(nIndexPt);
				txt_Arr.isPublic = false;
				vectext.push_back(txt_Arr);
			}
			else
			{
				AcDbObjectId idText = DrawText(ptMid , itFindDist->second.GetBuffer() , _T("MC") , _T("") , m_TextHight2*m_dH * m_dScale, dAngle);
				itFindDist->second.ReleaseBuffer();
				SetTextStyle(idText , m_strFont2.GetBuffer());
				m_strFont2.ReleaseBuffer();
				//idArrText.append(idText);

				//存储注记相关信息
				txt_Arr.ObjId = idText;
				_stprintf(txt_Arr.szText, _T("%s"), itFindDist->second.GetBuffer());
				itFindDist->second.ReleaseBuffer();
				txt_Arr.pt1 = nodes.at(nIndexPt - 1);
				txt_Arr.pt2 = nodes.at(nIndexPt);
				txt_Arr.isPublic = false;
				vectext.push_back(txt_Arr);
				
			}

		}
	}
	//公共线段注记删除
	for (int i = 0;i<vectext.size();i++)
	{
		for (int j = 0; j < vectext.size();j++)
		{
			if (i<vectext.size()-1)
			{
				if ((i == j) || vectext[i].isPublic)
					continue;

				if (vectext[i].pt1 == vectext[j].pt1)
				{//判断是否是公共线段的注记
					if (vectext[i].pt2 == vectext[j].pt2)
					{
						idArrText.append(vectext[i].ObjId);
						vectext[j].isPublic = true;
						break;
					}
				}
				else if (vectext[i].pt1 == vectext[j].pt2)
				{//判断是否是公共线段的注记
					if (vectext[i].pt2 == vectext[j].pt1)
					{
						idArrText.append(vectext[i].ObjId);
						vectext[j].isPublic = true;
						break;
					}
				}
				//若不是公共线段注记
				if (j == vectext.size() - 1)
				{
					idArrText.append(vectext[i].ObjId);
				}
			}
			else
			{
				if (!vectext[i].isPublic)
				{
					idArrText.append(vectext[i].ObjId);
					break;
				}
			}
		}
	}
	acutPrintf(_T("\n%d"), idArrText.length());
	acedRestoreStatusBar();
}

void CExportHouseDiagram::NoteJZXDist_Simple(AcDbObjectIdArray idArrPoly , AcDbObjectIdArray & idArrText)
{
	for (int nIndexPoly = 0;nIndexPoly < idArrPoly.length();nIndexPoly++)
	{
		map<int ,  CString> mapDist_Nodes;
		int nPolyType = GetPolyType(aname(idArrPoly.at(nIndexPoly)),true);
		if ( nPolyType < 0 ) 
		{
			return; 
		}
		else if ( nPolyType < 10 ) 
		{
			return; 
		}
		AcGePoint2dArray nodes;
		GetPlList(aname(idArrPoly.at(nIndexPoly)) , nodes);
		RemoveSameNodes(nodes);
		if (IsDeasil(nodes))
		{
			nodes.reverse();
		}
		nodes.append(nodes.first());
		for (int nIndexPt = 1;nIndexPt < nodes.length();nIndexPt++)
		{
			double dDist = distOF2d(nodes.at(nIndexPt) , nodes.at(nIndexPt - 1));
			TCHAR szDist[1000] = {0};
			_stprintf(szDist , _T("%.2f") , dDist);
			double dAngle = GetAngle(apoint(nodes.at(nIndexPt - 1)) , apoint(nodes.at(nIndexPt)));
			ads_point ptMid;
			ptMid[X] = (nodes.at(nIndexPt).x + nodes.at(nIndexPt - 1).x)/2;
			ptMid[Y] = (nodes.at(nIndexPt).y + nodes.at(nIndexPt - 1).y)/2;
			ads_polar(ptMid , PI/2 + dAngle , m_TextHight2*m_dScale*m_dH*1.2 , ptMid);
			ads_polar(ptMid , PI/2 + dAngle , m_TextHight2*m_dScale*m_dH*1.2 , ptMid);
			veStrDist.push_back(CString(szDist));
			vector<int> veIndexPt;
			veIndexPt.push_back(nIndexPt);
			veIndexPt.push_back(nIndexPt - 1);
			mapDist_Nodes.insert(pair<int ,  CString>(nIndexPt , CString(szDist)));
			if (dAngle > (PI - PI/18) && dAngle < (PI + PI/18))
			{
				AcDbObjectId idText = DrawText(ptMid , szDist , _T("MC") , _T("") , m_TextHight2*m_dH , dAngle+PI);
				SetTextStyle(idText , m_strFont2.GetBuffer());
				m_strFont2.ReleaseBuffer();
				idArrText.append(idText);
			}
			else
			{
				AcDbObjectId idText = DrawText(ptMid , szDist , _T("MC") , _T("") , m_TextHight2*m_dH , dAngle);
				SetTextStyle(idText , m_strFont2.GetBuffer());
				m_strFont2.ReleaseBuffer();
				idArrText.append(idText);
			}

		}
		m_PloyDist.insert(pair<AcDbObjectId , map<int ,  CString> >( idArrPoly.at(nIndexPoly) , mapDist_Nodes ));
	}

}

void CExportHouseDiagram::NoteJZXDist(AcDbObjectIdArray idArrPoly , AcDbObjectIdArray & idArrText)
{
	for (int nIndexPoly = 0;nIndexPoly < idArrPoly.length();nIndexPoly++)
	{
		map<int ,  CString> mapDist_Nodes;
		int nPolyType = GetPolyType(aname(idArrPoly.at(nIndexPoly)),true);
		if ( nPolyType < 0 ) 
		{
			continue; 
		}
// 		else if ( nPolyType < 10 ) 
// 		{
// 			return; 
// 		}
		AcGePoint2dArray nodes;
		GetPlList(aname(idArrPoly.at(nIndexPoly)) , nodes);
		RemoveSameNodes(nodes);
		if (IsDeasil(nodes))
		{
			nodes.reverse();
		}
		nodes.append(nodes.first());
		for (int nIndexPt = 1;nIndexPt < nodes.length();nIndexPt++)
		{
			double dDist = distOF2d(nodes.at(nIndexPt) , nodes.at(nIndexPt - 1));
			TCHAR szDist[1000] = {0};
			_stprintf(szDist , _T("%.2f") , dDist);
			double dAngle = GetAngle(apoint(nodes.at(nIndexPt - 1)) , apoint(nodes.at(nIndexPt)));
			ads_point ptMid;
			ptMid[X] = (nodes.at(nIndexPt).x + nodes.at(nIndexPt - 1).x)/2;
			ptMid[Y] = (nodes.at(nIndexPt).y + nodes.at(nIndexPt - 1).y)/2;
			ads_polar(ptMid , PI/2 + dAngle , m_TextHight2*m_dScale*m_dH*1.1 , ptMid);
			ads_polar(ptMid , PI/2 + dAngle , m_TextHight2*m_dScale*m_dH*1.1 , ptMid);
			veStrDist.push_back(CString(szDist));
			//vector<int> veIndexPt;
			//veIndexPt.push_back(nIndexPt);
			//veIndexPt.push_back(nIndexPt - 1);
			mapDist_Nodes.insert(pair<int ,  CString>(nIndexPt , CString(szDist)));
// 			if (dAngle > (PI - PI/18) && dAngle < (PI + PI/18))
// 			{
// 				AcDbObjectId idText = DrawText(ptMid , szDist , _T("MC") , _T("") , m_TextHight2*m_dH , dAngle+PI);
// 				SetTextStyle(idText , m_strFont2.GetBuffer());
// 				m_strFont2.ReleaseBuffer();
// 				idArrText.append(idText);
// 			}
// 			else
// 			{
// 				AcDbObjectId idText = DrawText(ptMid , szDist , _T("MC") , _T("") , m_TextHight2*m_dH , dAngle);
// 				SetTextStyle(idText , m_strFont2.GetBuffer());
// 				m_strFont2.ReleaseBuffer();
// 				idArrText.append(idText);
// 			}

		}
		m_PloyDist.insert(pair<AcDbObjectId , map<int ,  CString> >( idArrPoly.at(nIndexPoly) , mapDist_Nodes ));
	}

}

bool CExportHouseDiagram::IsSameFloorEnts(AcDbObjectIdArray idArry1, AcDbObjectIdArray idArry2)
{
	AcDbObjectId Main1, Main2;
	ads_point pt1, pt2;
	AcDbObjectIdArray idArry1_temp,idArry2_temp;
	AcDbObjectIdArray idArry2_As1;//和idArry1_temp存储的实体顺序相同（相同面积实体序号相同）
	std::vector<double> vecArea;
	std::vector<double> vecAngle;//与主体的相对位置角度
	for (int i = 0; i < idArry1.length();i++)
	{
		TCHAR szCode[50] = { 0 };
		ads_name ent = {0};
		acdbGetAdsName(ent, idArry1.at(i));
		GetStdm(ent, szCode);
		if (!_tcscmp(szCode, _T("400000")))
			idArry1_temp.append(idArry1.at(i));
	}

	for (int i = 0; i < idArry2.length(); i++)
	{
		TCHAR szCode[50] = { 0 };
		ads_name ent = {0};
		acdbGetAdsName(ent, idArry2.at(i));
		GetStdm(ent, szCode);
		if (!_tcscmp(szCode, _T("400000")))
			idArry2_temp.append(idArry2.at(i));
	}
	double dArea_max = 0.0;
	for (int i = 0; i < idArry1_temp.length(); i++)
	{
		double dArea = 0.0;
		ads_name ent = {0};
		acdbGetAdsName(ent, idArry1_temp.at(i));
		GetEntArea(ent, dArea);
		vecArea.push_back(dArea);
		if (dArea > dArea_max)
		{
			dArea_max = dArea;
			Main1 = idArry1_temp.at(i);
		}
	}
	dArea_max = 0.0;
	for (int j = 0; j < vecArea.size(); j++)
	{
		for (int i = 0; i < idArry2_temp.length(); i++)
		{
			double dArea = 0.0;
			ads_name ent = {0};
			acdbGetAdsName(ent, idArry2_temp.at(i));
			GetEntArea(ent, dArea);
			if (fabs(dArea-vecArea[j])<1e-4)
			{
				idArry2_As1.append(idArry2_temp.at(i));
			}
			if (dArea > dArea_max)
			{
				dArea_max = dArea;
				Main2 = idArry2_temp.at(i);
			}
			
		}
	}
	ads_name ent1 = {0};
	if(Acad::eOk != acdbGetAdsName(ent1, Main1))return false;
	GetEntCentroidPt(ent1, pt1);
	ads_name ent2 = {0};
	if (Acad::eOk != acdbGetAdsName(ent2, Main2))return false;
	GetEntCentroidPt(ent2, pt2);
	//位置对比
	for (int i = 0; i < idArry1_temp.length(); i++)
	{
		ads_name ent1_temp = { 0 }, ent2_temp = {0};
		ads_point pt1_temp = { 0 }, pt2_temp = {0};
		TCHAR szNmae1[64] = { 0 }, szNmae2[64] = { 0 };
		acdbGetAdsName(ent1_temp, idArry1_temp.at(i));
		GetEntCentroidPt(ent1_temp, pt1_temp);
		acdbGetAdsName(ent2_temp, idArry2_As1.at(i));
		GetEntCentroidPt(ent2_temp, pt2_temp);
		double angle1 = ads_angle(pt1, pt1_temp);
		double angle2 = ads_angle(pt2, pt2_temp);
		if (fabs((angle1 - angle2) > 1e-4))
			return false;
		ReadXdata(ent1_temp, _T("room_name"), 0, szNmae1);
		ReadXdata(ent2_temp, _T("room_name"), 0, szNmae2);
		if (_tcscmp(szNmae1, szNmae2))
			return false;
	}
	return true;
}

void CExportHouseDiagram::OderLayerName(int nIndex)
{
	vector<double> veLayerNum;
	m_vector_Layer_Oder.clear();
	vector<layerinfo>().swap(m_vector_Layer_Oder); 
	map<CString , AcDbObjectIdArray>::iterator it = m_vector_map_Layer_IdArr[nIndex].begin();
	for(; it != m_vector_map_Layer_IdArr[nIndex].end(); it ++)
	{	
		CString strTemp;
		strTemp = it->first;
		strTemp = strTemp.Left(strTemp.Find(_T("层")));
		veLayerNum.push_back(_tstof(strTemp));
	}
	for(int cmpnum = veLayerNum.size()-1; cmpnum != 0; --cmpnum)
	{
		for (int i = 0; i != cmpnum; ++i)
		{
			if (veLayerNum[i] - veLayerNum[i+1] > 0.0001)
			{
				double temp;
				temp = veLayerNum[i];
				veLayerNum[i] =veLayerNum[i+1];
				veLayerNum[i+1] = temp;
			}
		}
	}

    for (int i = 0;i < veLayerNum.size();i++)
    {
		CString strLayerName;
		strLayerName.Format(_T("%.1f层") , veLayerNum[i]);
		if (strLayerName.Find(_T(".0")) != -1)
		{
			strLayerName.Format(_T("%.0f层") , veLayerNum[i]);
		}
		it = m_vector_map_Layer_IdArr[nIndex].find(strLayerName);
		if (it != m_vector_map_Layer_IdArr[nIndex].end())
		{
			double dArea = getEntsArea(it->second);
			int nIdNum = it->second.length();
			int nNodes = GetNodeNum(it->second);
			int nFind = -1;
			AcDbObjectIdArray idarry_temp = NULL;
			idarry_temp.append(it->second);

			for (int nIndex = 0; nIndex < m_vector_Layer_Oder.size(); nIndex++)
			{
				if ((m_vector_Layer_Oder.at(nIndex).dArea - dArea) < 0.001&&m_vector_Layer_Oder.at(nIndex).idArr.length() == nIdNum&&m_vector_Layer_Oder.at(nIndex).nNodesNum == nNodes&&IsSameFloorEnts(idarry_temp, m_vector_Layer_Oder.at(nIndex).idArr))
				{
					nFind = nIndex;
				}
			}		
			if (nFind != -1)
			{
				m_vector_Layer_Oder.at(nFind).nLayerCount++;
				m_vector_Layer_Oder.at(nFind).veStrFloorName.push_back(strLayerName);	
			}
			else
			{
				layerinfo layerii;
				layerii.dArea = dArea;
				layerii.idArr.append(it->second);
				layerii.nLayerCount = 1;
				layerii.nNodesNum = nNodes;
				layerii.veStrFloorName.push_back(strLayerName);				
				m_vector_Layer_Oder.push_back(layerii);
			}
		}
    }
}

// bool CExportHouseDiagram::buildMainBlocks_Simple()
// {
// 	map<CString , ZRZinfo>::iterator it_BDCBH_ZRZINFO = m_mapBDCBH_ZRZinfo.begin();
// 	for(; it_BDCBH_ZRZINFO != m_mapBDCBH_ZRZinfo.end(); it_BDCBH_ZRZINFO++)
// 	{
// 		actrTransactionManager->startTransaction();
// 		m_objArrSave.setLogicalLength(0);
// 		ads_point ptInsert;
// 		ptInsert[X] = 0;
// 		ptInsert[Y] = 0;
// 		AcDbObjectId idTK;
// 		idTK = DrawInsert(ptInsert, _T("房产分户图_模板2"));
// 		if (idTK == AcDbObjectId::kNull)
// 		{
// 			AfxMessageBox(_T("找不到模版房产分户图_模板2"));
// 			return false;
// 		}
// 		m_objTukuang = idTK;
// 		SBreakBlock m_blockB;
// 		m_blockB.SetBlock(idTK);
// 		m_blockB.Explode();
// 
// 		AcGePoint3d m_pt3dWLBTK;
// 		AcGePoint3d m_pt3dWRTTK;
// 		AcGePoint3d m_ptInsertNorth;
// 		AcGePoint3d m_pt3dWLBTABLE;
// 		if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK) == false)
// 		{
// 			if (m_blockB.findPointByHyperLink(_T("右上角"), m_pt3dWRTTK) == false)
// 			{
// 				AfxMessageBox(_T("模板右上角点无法找到."));
// 				return false;
// 			}
// 		}
// 
// 		if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK) == false)
// 		{
// 			if (m_blockB.findPointByHyperLink(_T("左下角"), m_pt3dWLBTK) == false)
// 			{
// 				AfxMessageBox(_T("模版左下角点无法找到。"));
// 				return false;
// 			}
// 		}
// 
// 
// 		if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
// 		{
// 			if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
// 			{
// 				AfxMessageBox(_T("模版指北针插入点无法找到。"));
// 				return false;
// 			}
// 		}
// 		if ( it_BDCBH_ZRZINFO->first.GetLength() >=  19)
// 		{
// 			m_strZDDM.Format(_T("%s") ,  it_BDCBH_ZRZINFO->first.Left(19));
// 		}	
// 		m_strZH.Format(_T("%s") ,  it_BDCBH_ZRZINFO->second.strZH);
// 		m_strHH = it_BDCBH_ZRZINFO->second.strHH;
// 		m_strZL = it_BDCBH_ZRZINFO->second.strZL;
// 		m_strZCS = it_BDCBH_ZRZINFO->second.strZCS;
// 		m_strSZC = it_BDCBH_ZRZINFO->second.strSZC;
// 		m_strJG = it_BDCBH_ZRZINFO->second.strJG;
// 		m_strZYJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strZYJZMJ));
// 		m_strJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strJZMJ));
// 		m_strFTMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strFTMJ));		
// 		m_strZCS.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strZCS);
// 		m_strSZC.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strSZC);
// 		m_blockB.replaceText(_T("#zddm#") , m_strZDDM);
// 		m_blockB.replaceText(_T("#zrzh#") , m_strZH);
// 		m_blockB.replaceText(_T("#hh#") , m_strHH);
// 		m_blockB.replaceText(_T("#fdzl#") , m_strZL);
// 		m_blockB.replaceText(_T("#fwjg#") , m_strJG);
// 		m_blockB.replaceText(_T("#zcs#") , m_strZCS);
// 		m_blockB.replaceText(_T("#ZYJZMJ#") , m_strZYJZMJ);
// 		m_blockB.replaceText(_T("#FTJZMJ#") , m_strFTMJ);
// 		m_blockB.replaceText(_T("#JZMJ#") , m_strJZMJ);
// 		m_blockB.replaceText(_T("#szc#") , m_strSZC);
// 
// 		for (int i=0;i<m_strArrKey.GetSize();++i)
// 		{
// 			CString strKey=m_strArrKey.GetAt(i);
// 			CString strValue=m_strArrValue.GetAt(i);
// 			if(strKey.GetLength()<1)
// 				continue;
// 			if (0 == _tcscmp(strKey , _T("#UNIT#")) && strValue.GetLength() > 0)
// 			{
// 				CString strTemp;
// 				strTemp = strValue;
// 				strValue.Format(_T(""));
// 				strValue += strTemp.Mid(0 , 0);
// 				AcDbObjectId idTemp = AcDbObjectId::kNull;
// 				m_blockB.replaceText(strKey , strValue , idTemp , false);
// 				m_objArrSave.append(idTemp);
// 				if (idTemp != AcDbObjectId::kNull)
// 				{
// 					ads_point ptUnit;
// 					GetEntPosition(aname(idTemp) , ptUnit);
// 					int nlen = strTemp.GetLength();
// 					while (strTemp.GetLength() > 0)
// 					{
// 						ads_polar(ptUnit , -1*PI/2 , 8*m_dH*1.5 , ptUnit);
// 						CString strUnit;
// 						TCHAR ch = strTemp.GetAt(0);
// 						if (ch > 127 || ch < 0)
// 						{
// 							strUnit.Format(_T("%s") , strTemp.Left(2));
// 							strTemp.Format(_T("%s") , strTemp.Right(strTemp.GetLength() - 2));
// 						}
// 						else
// 						{
// 							strUnit.Format(_T("%s") , strTemp.Left(1));
// 							strTemp.Format(_T("%s") , strTemp.Right(strTemp.GetLength() - 1));
// 						} 						
// 						AcDbObjectId idText = DrawText(ptUnit , strUnit , _T("MC") , _T("") , m_dH*8);
// 						SetTextStyle(idText , m_strFont1.GetBuffer());
// 						m_strFont1.ReleaseBuffer();
// 						m_objArrSave.append(idText);
// 
// 					}
// 				}
// 			}
// 			else
// 			{
// 				m_blockB.replaceText(strKey , strValue);
// 			}
// 
// 		}
// 
// 		double dWidth = m_pt3dWRTTK.x - m_pt3dWLBTK.x;
// 		double dHigh = m_pt3dWRTTK.y - m_pt3dWLBTK.y;
// 
// 		int nFloorCount = 0;
// 		map<CString , AcDbObjectIdArray>::iterator it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
// 		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
// 		{
// 			nFloorCount++;
// 		}
// 		if (nFloorCount == 0)
// 		{
// 			continue;
// 		}
// 		double dStep_Total = dHigh / nFloorCount;
// 		double dStep = dStep_Total * 0.6;
// 
// 
// 
// 		it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
// 		int nIndex = 0;
// 		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
// 		{
// 			AcDbObjectIdArray idArrTemp;
// 			CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
// 			m_objArrSave.append(idArrTemp);
// 			ads_point ptNKMin , ptNKMax;
// 			AcGePoint3d ptNKMid;
// 			ptNKMin[X] = m_pt3dWLBTK[X];
// 			ptNKMin[Y] = m_pt3dWLBTK[Y] + dStep_Total*nIndex + dStep_Total * 0.4;
// 
// 			ptNKMax[X] = m_pt3dWRTTK[X];
// 			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total*(nIndex+1);
// 
// 			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
// 			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);
// 
// 			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
// 			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
// 			ptNKMid.z = 0;
// 
// 			ads_point ptMin , ptMax , ptMid;
// 			GetEntsExtent(idArrTemp , ptMin , ptMax);
// 			double dw = (ptMax[X] - ptMin[X]);
// 			double dh = (ptMax[Y] - ptMin[Y]);
// 			ptMid[X] = (ptMax[X] + ptMin[X])/2;
// 			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;
// 
// 			double dYMove = ptNKMid.y - ptMid[Y];
// 			double dXMove = ptNKMid.x - ptMid[X];
// 
// 			double dscale1 = ( dNKWidth / dw );
// 			double dscale2 = ( dStep / dh );
// 			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
// 			m_dScale = dNewScale;
// 			double l = m_dH * dNewScale;
// 			double dTemp = m_dH * 1000.0 / l;
// 			int nTemp = (int)(dTemp / 100 + 1 ); //比例最小为100
// 			dTemp = nTemp * 100;
// 			TCHAR szValue[1000] = {0};
// 			_stprintf(szValue, _T("1:200"), dTemp);
// 
// 			m_blockB.replaceText(_T("#SCALE#") , szValue);
// 
// 			TCHAR szFlage[1000] = {0};
// 			AcDbObjectId objIdTable;
// 			vector<CString> veFloorName;
// 			AnalyzeData(veFloorName , it_FloorNum_IdArr->first , _T(","));
// 			//insertNodeFloor(idArrTemp , ptNKMin , ptNKMax , veFloorName.size() , veFloorName);
// 			AcDbObjectIdArray idArrText;
// 			NoteJZXDist(idArrTemp , idArrText);
// 
// 			for(int nIndex = 0; nIndex < idArrText.length(); nIndex ++)
// 			{
// 				MoveObject(idArrText.at(nIndex) , dXMove , dYMove);
// 				ScaleEntity(aname(idArrText.at(nIndex)), ptNKMid , dNewScale);
// 			}
// 			m_objArrSave.append(idArrText);
// 
// 			for(int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
// 			{
// 				MoveObject(idArrTemp.at(nIndex) , dXMove , dYMove);
// 				ScaleEntity(aname(idArrTemp.at(nIndex)) , ptNKMid , dNewScale);
// 			}
// 			m_objArrSave.append(idArrTemp);
// 			ads_point ptMinFloor , ptMaxFloor;
// 			GetEntsExtent(idArrTemp , ptMinFloor , ptMaxFloor);
// 			//insertNodeFloor(idArrTemp , ptMinFloor , ptMaxFloor , veFloorName.size() , veFloorName);
// 			ReNoteHouse(idArrTemp);
// 			nIndex++;
// 		}
// 		m_objArrSave.append(m_blockB.m_objIdArr);
// 
// 		ads_name ssAll , ssSave;
// 		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
// 		IdArrToSS(m_objArrSave , ssSave);
// 		SubssName(ssAll , ssSave);
// 		EntsDel(ssAll);
// 		acedSSFree(ssSave);
// 		acedSSFree(ssAll);
// 
// 		CXRecordOperate xRecord;
// 		double dAngle = 0.0;
// 		xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
// 		AcDbObjectId idNorth = DrawInsert(apoint(m_ptInsertNorth) , _T("north1")  , m_dH);
// 
// 		RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);
// 		m_objArrSave.append(idNorth);
// 
// 		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
// 		m_objArrSave.setLogicalLength(0);
// 		SSToIdArr(ssAll , m_objArrSave);
// 
// 
// 
// 		ZoomWindow(apoint(m_pt3dWLBTK) ,  apoint(m_pt3dWRTTK));
// 
// 		CSaveBlocks saveB;
// 		saveB.setPoint(apoint(m_pt3dWLBTK));
// 
// 		CString	 strTempSavePath;
// 		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
// 		{
// 			strTempSavePath.Format(_T("%s%s_%s_%s"), m_strPath , m_strZDDM , m_strZH , m_strHH);
// 		}
// 		else
// 		{
// 			strTempSavePath.Format(_T("%s\\%s_%s_%s"), m_strPath , m_strZDDM , m_strZH , m_strHH);
// 		}
// 		m_strTempSavePath.Format(_T("%s") , strTempSavePath);
// 		RemoveSameObjects(m_objArrSave);
// 		saveB.setPath(strTempSavePath);
// 		saveB.saveBlocks2(m_objArrSave);
// 		actrTransactionManager->abortTransaction();
// 	}
// 	return TRUE;
// }

bool CExportHouseDiagram::buildMainBlocks_Simple()
{
	int nReportType =0;
	//if(RTNORM != ads_getint(_T(""),&nReportType));
	if(RTNORM != ads_getint(_T("\n生成比例尺方式:（0）自动（1）手动<0>"),&nReportType));

	int nisMertis = 0;
	//if(RTNORM != ads_getint(_T(""),&nReportType));
	if (RTNORM != ads_getint(_T("\n是否融合自然幢内相邻房屋主体:（0）是（1）否<0>"), &nisMertis));

	map<CString , ZRZinfo>::iterator it_BDCBH_ZRZINFO = m_mapBDCBH_ZRZinfo.begin();
	for(; it_BDCBH_ZRZINFO != m_mapBDCBH_ZRZinfo.end(); it_BDCBH_ZRZINFO++)
	{
		actrTransactionManager->startTransaction();
		m_objArrSave.setLogicalLength(0);
		ads_point ptInsert;
		ptInsert[X] = 0;
		ptInsert[Y] = 0;
		AcDbObjectId idTK;
		idTK = DrawInsert(ptInsert, _T("房产分户图_模板2"));
		if (idTK == AcDbObjectId::kNull)
		{
			AfxMessageBox(_T("找不到模版房产分户图_模板2"));
			return false;
		}
		m_objTukuang = idTK;
		SBreakBlock m_blockB;
		m_blockB.SetBlock(idTK);
		m_blockB.Explode();

		AcGePoint3d m_pt3dWLBTK;
		AcGePoint3d m_pt3dWRTTK;
		AcGePoint3d m_ptInsertNorth;
		AcGePoint3d m_pt3dWLBTABLE;
		if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("右上角"), m_pt3dWRTTK) == false)
			{
				AfxMessageBox(_T("模板右上角点无法找到."));
				return false;
			}
		}

		if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("左下角"), m_pt3dWLBTK) == false)
			{
				AfxMessageBox(_T("模版左下角点无法找到。"));
				return false;
			}
		}


		if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
			{
				AfxMessageBox(_T("模版指北针插入点无法找到。"));
				return false;
			}
		}
		if ( it_BDCBH_ZRZINFO->first.GetLength() >=  19)
		{
			m_strZDDM.Format(_T("%s") ,  it_BDCBH_ZRZINFO->first.Left(19));
		}	
		m_strZH.Format(_T("%s") ,  it_BDCBH_ZRZINFO->second.strZH);
		m_strHH = it_BDCBH_ZRZINFO->second.strHH;
		m_strZL = it_BDCBH_ZRZINFO->second.strZL;
		m_strZCS = it_BDCBH_ZRZINFO->second.strZCS;
		m_strSZC = it_BDCBH_ZRZINFO->second.strSZC;
		m_strJG = it_BDCBH_ZRZINFO->second.strJG;
		vector<CString> saDatas;
		AnalyzeData(saDatas ,m_strJG , _T(" "));
		if (saDatas.size() >= 2)
		{
			m_strJG = saDatas[1];
		}
		m_strZYJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strZYJZMJ));
		m_strJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strJZMJ));
		m_strFTMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strFTMJ));		
		m_strZCS.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strZCS);
		m_strSZC.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strSZC);
		m_strQLR.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strQLR);
		m_blockB.replaceText(_T("#zddm#") , m_strZDDM);
		m_blockB.replaceText(_T("#zrzh#") , m_strZH);
		m_blockB.replaceText(_T("#hh#") , m_strHH);
		m_blockB.replaceText(_T("#fdzl#") , m_strZL);
		
		m_blockB.replaceText(_T("#fwjg#") , m_strJG);

		m_blockB.replaceText(_T("#zcs#") , m_strZCS);
		m_blockB.replaceText(_T("#ZYJZMJ#") , m_strZYJZMJ);
		m_blockB.replaceText(_T("#FTJZMJ#") , m_strFTMJ);
		m_blockB.replaceText(_T("#JZMJ#") , m_strJZMJ);
		m_blockB.replaceText(_T("#szc#") , m_strSZC);



		double dWidth = m_pt3dWRTTK.x - m_pt3dWLBTK.x;
		double dHigh = m_pt3dWRTTK.y - m_pt3dWLBTK.y;

		int nFloorCount = 0;
		map<CString , AcDbObjectIdArray>::iterator it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			nFloorCount++;
		}
		if (nFloorCount == 0)
		{
			continue;
		}
		double dStep_Total = dHigh / nFloorCount;
		double dStep = dStep_Total * 0.6;

		vector<double> veDNewScale ;
		for (it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			idArrTemp.append(it_FloorNum_IdArr->second);
			//CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			//m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y]  + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total;

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			GetEntsExtent(idArrTemp , ptMin , ptMax);
			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			double dscale1 = ( dNKWidth / dw );
			double dscale2 = ( dStep / dh );
			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			//m_dScale = dNewScale;
			veDNewScale.push_back(dNewScale);
		}

		sort(veDNewScale.begin() , veDNewScale.end());
		if (nReportType==0)
		{
			if (veDNewScale.size() > 0)
			{
				m_dScale = veDNewScale.at(0);
				double l = m_dH * m_dScale;
				double dTemp = m_dH * 1000.0 / l;
				int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
				if (nTemp==1)
				{
					nTemp=2;
				}
				dTemp = nTemp * 50;
				TCHAR szValue[100] = {0};
				_stprintf(szValue, _T("1:%.f"), dTemp);
				m_dScale = 1000/dTemp;
				m_blockB.replaceText(_T("#SCALE#") , szValue);
			}
		}
		else
		{
			int nReportType1 = 300;
			while (true)
			{
				if(RTNORM != ads_getint(_T("\n请输入比例尺1:<300>"),&nReportType1));
				if (nReportType1==0)
				{
					acutPrintf(_T("比例尺不正确"));
					continue;
				}
				else
				{
					break;
				}
			}
			
			m_dScale = 1000/nReportType1;
			TCHAR szValue[100] = {0};
			_stprintf(szValue, _T("1:%d"), nReportType1);

			m_blockB.replaceText(_T("#SCALE#") , szValue);
		}
		


		for (int i = 0; i < m_strArrKey.GetSize(); ++i)
		{
			CString strKey = m_strArrKey.GetAt(i);
			CString strValue = m_strArrValue.GetAt(i);
			if (strKey.GetLength() < 1)
				continue;
			if (0 == _tcscmp(strKey, _T("#UNIT#")) && strValue.GetLength() > 0)
			{
				CString strTemp;
				strTemp = strValue;
				strValue.Format(_T(""));
				strValue += strTemp.Mid(0, 0);
				AcDbObjectId idTemp = AcDbObjectId::kNull;
				m_blockB.replaceText(strKey, strValue, idTemp, false);
				double dHeight = GetTextHeight(aname(idTemp));
				m_objArrSave.append(idTemp);
				if (idTemp != AcDbObjectId::kNull)
				{
					ads_point ptUnit;
					GetEntPosition(aname(idTemp), ptUnit);
					int nlen = strTemp.GetLength();
					while (strTemp.GetLength() > 0)
					{
						ads_polar(ptUnit, -1 * PI / 2, dHeight*1.5, ptUnit);
						CString strUnit;
						TCHAR ch = strTemp.GetAt(0);
						if (ch > 127 || ch < 0)
						{
							strUnit.Format(_T("%s"), strTemp.Left(2));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 2));
						}
						else
						{
							strUnit.Format(_T("%s"), strTemp.Left(1));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 1));
						}
						AcDbObjectId idText = DrawText(ptUnit, strUnit, _T("MC"), _T(""), dHeight);
						m_objArrSave.append(idText);

					}
				}
			}
			else
			{
				m_blockB.replaceText(strKey, strValue);
			}

		}

		it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		int nIndex_Floor = 0;
		double dAngle = 0.0;
		double dAngle_Ro=0.0;
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);

			if (nisMertis == 0)
			{
				merits(idArrTemp);
			}

			ads_name reCASS;
			ads_ssadd(NULL, NULL, reCASS);
			for (int idx = 0; idx < idArrTemp.length();idx++)
			{
				ads_ssadd(aname(idArrTemp.at(idx)), reCASS, reCASS); //ssname是保存到文件的东西
			}

			struct resbuf *cmd, *result;
			cmd = ads_buildlist(RTSTR, _T("recass_some_entity"),
				RTPICKS, reCASS, RTNONE);
			ads_invoke(cmd, &result);
			ads_relrb(cmd);
			if (result != NULL) ads_relrb(result);

			m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y] + dStep_Total*nIndex_Floor + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total*(nIndex_Floor+1);

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			double dis_Max =0.0;
			for (int ii = 0; ii < idArrTemp.length(); ii ++)
			{
				ads_point ptMin_temp , ptMax_temp ;
				GetEntityTrend(aname(idArrTemp.at(ii)),ptMin_temp , ptMax_temp);
				double dis_Max_temp= distOF2d(apoint(ptMin_temp),apoint(ptMax_temp));
				if (dis_Max<dis_Max_temp)
				{
					ptMin[X]=ptMin_temp[X];ptMin[Y]=ptMin_temp[Y];
					ptMax[X]=ptMax_temp[X];ptMax[Y]=ptMax_temp[Y];
				}
			}

			ads_point ptMin_Ro={0} , ptMax_Ro={0};
			ptMin_Ro[X]=ptMin[X];ptMax_Ro[X]=ptMax[X];
			ptMin_Ro[Y]=ptMin[Y];ptMax_Ro[Y]=ptMax[Y];
			ptMin_Ro[Z]=0;ptMax_Ro[Z]=0;
			AcGePoint3d ptBase;
			ptBase[X]=ptMin_Ro[X];ptBase[Y]=ptMin_Ro[Y];ptBase[Z]=ptMin_Ro[Z];
			
			if (nIndex_Floor==0)
			{
				dAngle = ads_angle(ptMin_Ro , ptMax_Ro);

				double dTemp = -dAngle / PI;
				while (dTemp + 0.5 < 0)
				{
					dTemp += 1;
				}
				while (dTemp - 0.5 > 0)
				{
					dTemp -= 1;
				}
				dAngle_Ro = dTemp * PI;
			}
			for (int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				RotateEntity(aname(idArrTemp.at(nIndex)),dAngle_Ro,ptBase);
			}
			GetEntsExtent(idArrTemp , ptMin , ptMax);

			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

// 			double dscale1 = ( dNKWidth / dw );
// 			double dscale2 = ( dStep / dh );
// 			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
// 			m_dScale = dNewScale;
// 			double l = m_dH * dNewScale;
// 			double dTemp = m_dH * 1000.0 / l;
// 			int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
// 			dTemp = nTemp * 10;
// 			TCHAR szValue[100] = {0};
// 			_stprintf(szValue, _T("1:%.f"), dTemp);
// 
// 			m_blockB.replaceText(_T("#SCALE#") , szValue);

			TCHAR szFlage[100] = {0};
			AcDbObjectId objIdTable;
			vector<CString> veFloorName;
			AnalyzeData(veFloorName , it_FloorNum_IdArr->first , _T(","));
			insertNodeFloor_Simple(idArrTemp , ptNKMin , ptNKMax , veFloorName.size() , veFloorName,nIndex_Floor+1);
			AcDbObjectIdArray idArrText;
			//NoteJZXDist_Simple(idArrTemp , idArrText);
			NoteJZXDist(idArrTemp , idArrText);
			//m_objArrSave.append(idArrText);
			for(int nIndex = 0; nIndex < idArrText.length(); nIndex ++)
			{
				MoveObject(idArrText.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrText.at(nIndex)), ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrText);

			for(int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				MoveObject(idArrTemp.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrTemp.at(nIndex)) , ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrTemp);
			NoteJZXDist_Final(idArrTemp , idArrText);
			m_objArrSave.append(idArrText);
			ReNoteHouse(idArrTemp);
			nIndex_Floor++;
		}
		m_objArrSave.append(m_blockB.m_objIdArr);

		ads_name ssAll , ssSave;
		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		IdArrToSS(m_objArrSave , ssSave);
		SubssName(ssAll , ssSave);
		EntsDel(ssAll);
		acedSSFree(ssSave);
		acedSSFree(ssAll);

		CXRecordOperate xRecord;
		
		//xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
		AcDbObjectId idNorth = DrawInsert(apoint(m_ptInsertNorth) , _T("north1"));

		//RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);

		double dTemp = (PI / 2 - dAngle) / PI;
		if (dTemp < 0)
		{
			while (dTemp < 0)
			{
				dTemp += 1.0;
			}
		}
		else if (dTemp > 1)
		{
			while (dTemp - 1.0 > 0)
			{
				dTemp -= 1.0;
			}
		}
		RotateEntity(aname(idNorth), dTemp*PI, m_ptInsertNorth);
		
		//RotateEntity(aname(idNorth), dAngle_Ro, m_ptInsertNorth);
		m_objArrSave.append(idNorth);

		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		m_objArrSave.setLogicalLength(0);
		SSToIdArr(ssAll , m_objArrSave);

		ZoomWindow(apoint(m_pt3dWLBTK) ,  apoint(m_pt3dWRTTK));

		CSaveBlocks saveB;
		saveB.setPoint(apoint(m_pt3dWLBTK));

		vector<CString>layers;
		layers.push_back(_T("TK"));
		layers.push_back(_T("ZJ"));
		setLayersOn2(layers);

		CString	 strTempSavePath;
		CString strTempPath;
		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
		{
			strTempPath.Format(_T("%s%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"),strTempPath , m_strZDDM , m_strZH);
		}
		else
		{
			strTempPath.Format(_T("%s\\%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"), strTempPath , m_strZDDM , m_strZH);
		}
		m_strTempSavePath.Format(_T("%s") , strTempSavePath);
		RemoveSameObjects(m_objArrSave);
		saveB.setPath(strTempSavePath);

		//ScaleEntitys(m_objArrSave, m_pt3dWLBTK, 1 / m_dScale);

		saveB.saveBlocks2(m_objArrSave);
		actrTransactionManager->abortTransaction();
	}
	acutPrintf(_T("输出完成！"));
	return TRUE;
}

bool CExportHouseDiagram::buildMainBlocks_GD_Simple()
{
	int nReportType =0;
	if(RTNORM != ads_getint(_T("\n生成比例尺方式:（0）自动（1）手动<0>"),&nReportType));

	map<CString , ZRZinfo>::iterator it_BDCBH_ZRZINFO = m_mapBDCBH_ZRZinfo.begin();
	for(; it_BDCBH_ZRZINFO != m_mapBDCBH_ZRZinfo.end(); it_BDCBH_ZRZINFO++)
	{
		JZMJ=0.0;
		actrTransactionManager->startTransaction();
		m_objArrSave.setLogicalLength(0);
		ads_point ptInsert;
		ptInsert[X] = 0;
		ptInsert[Y] = 0;
		AcDbObjectId idTK;
		idTK = DrawInsert(ptInsert, _T("房产分户图_模板2"));
		if (idTK == AcDbObjectId::kNull)
		{
			AfxMessageBox(_T("找不到模版房产分户图_模板2"));
			return false;
		}
		m_objTukuang = idTK;
		SBreakBlock m_blockB;
		m_blockB.SetBlock(idTK);
		m_blockB.Explode();

		AcGePoint3d m_pt3dWLBTK;
		AcGePoint3d m_pt3dWRTTK;
		AcGePoint3d m_ptInsertNorth;
		AcGePoint3d m_pt3dWLBTABLE;
		if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("右上角"), m_pt3dWRTTK) == false)
			{
				AfxMessageBox(_T("模板右上角点无法找到."));
				return false;
			}
		}

		if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("左下角"), m_pt3dWLBTK) == false)
			{
				AfxMessageBox(_T("模版左下角点无法找到。"));
				return false;
			}
		}


		if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
			{
				AfxMessageBox(_T("模版指北针插入点无法找到。"));
				return false;
			}
		}
		if ( it_BDCBH_ZRZINFO->first.GetLength() >=  19)
		{
			m_strZDDM.Format(_T("%s") ,  it_BDCBH_ZRZINFO->first.Left(19));
		}	
		m_strZH.Format(_T("%s") ,  it_BDCBH_ZRZINFO->second.strZH);
		m_strHH = it_BDCBH_ZRZINFO->second.strHH;
		m_strZL = it_BDCBH_ZRZINFO->second.strZL;
		m_strZCS = it_BDCBH_ZRZINFO->second.strZCS;
		m_strSZC = it_BDCBH_ZRZINFO->second.strSZC;
		m_strJG = it_BDCBH_ZRZINFO->second.strJG;
		vector<CString> saDatas;
		AnalyzeData(saDatas ,m_strJG , _T(" "));
		if (saDatas.size() >= 2)
		{
			m_strJG = saDatas[1];
		}
		//m_strZYJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strZYJZMJ));
		//m_strJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strJZMJ));
		m_strFTMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strFTMJ));		
		//m_strZCS.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strZCS);
		//m_strSZC.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strSZC);
		m_strQLR.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strQLR);
		m_blockB.replaceText(_T("#zddm#") , m_strZDDM);
		m_blockB.replaceText(_T("#zrzh#") , m_strZH);
		m_blockB.replaceText(_T("#hh#") , m_strHH);
		m_blockB.replaceText(_T("#fdzl#") , m_strZL);

		m_blockB.replaceText(_T("#fwjg#") , m_strJG);
		m_blockB.replaceText(_T("#FTJZMJ#") , m_strFTMJ);
		m_blockB.replaceText(_T("#zcs#") , m_strZCS);
		int floor_num=_ttoi(m_strZCS);
		for (int ii=0;ii<floor_num;ii++)
		{
			CString temp;
			if (ii==floor_num-1)
			{
				temp.Format(_T("%d"),ii+1);
			}
			else
			{
				temp.Format(_T("%d;"),ii+1);
			}
			m_strSZC +=temp;
		}
		m_blockB.replaceText(_T("#szc#") , m_strSZC);
		
		double dWidth = m_pt3dWRTTK.x - m_pt3dWLBTK.x;
		double dHigh = m_pt3dWRTTK.y - m_pt3dWLBTK.y;

		int nFloorCount = 0;
		map<CString , AcDbObjectIdArray>::iterator it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			nFloorCount++;
		}
		if (nFloorCount == 0)
		{
			continue;
		}

		double dStep_Total = dHigh / nFloorCount;
		double dStep = dStep_Total * 0.6;

		vector<double> veDNewScale ;
		for (it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			idArrTemp.append(it_FloorNum_IdArr->second);
			//CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			//m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y]  + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total;

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			GetEntsExtent(idArrTemp , ptMin , ptMax);
			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			double dscale1 = ( dNKWidth / dw );
			double dscale2 = ( dStep / dh );
			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			//m_dScale = dNewScale;
			veDNewScale.push_back(dNewScale);
		}

		sort(veDNewScale.begin() , veDNewScale.end());
		if (nReportType==0)
		{
			if (veDNewScale.size() > 0)
			{
				m_dScale = veDNewScale.at(0);
				double l = m_dH * m_dScale;
				double dTemp = m_dH * 1000.0 / l;
				int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
				if (nTemp==1)
				{
					nTemp=2;
				}
				dTemp = nTemp * 50;
				TCHAR szValue[100] = {0};
				_stprintf(szValue, _T("1:%.f"), dTemp);
				m_dScale = 1000/dTemp;
				m_blockB.replaceText(_T("#SCALE#") , szValue);
			}
		}
		else
		{
			int nReportType1 = 300;
			while (true)
			{
				if(RTNORM != ads_getint(_T("\n请输入比例尺1:<300>"),&nReportType1));
				if (nReportType1==0)
				{
					acutPrintf(_T("比例尺不正确"));
					continue;
				}
				else
				{
					break;
				}
			}

			m_dScale = 1000/nReportType1;
			TCHAR szValue[100] = {0};
			_stprintf(szValue, _T("1:%d"), nReportType1);

			m_blockB.replaceText(_T("#SCALE#") , szValue);
		}



		for (int i = 0; i < m_strArrKey.GetSize(); ++i)
		{
			CString strKey = m_strArrKey.GetAt(i);
			CString strValue = m_strArrValue.GetAt(i);
			if (strKey.GetLength() < 1)
				continue;
			if (0 == _tcscmp(strKey, _T("#UNIT#")) && strValue.GetLength() > 0)
			{
				CString strTemp;
				strTemp = strValue;
				strValue.Format(_T(""));
				strValue += strTemp.Mid(0, 0);
				AcDbObjectId idTemp = AcDbObjectId::kNull;
				m_blockB.replaceText(strKey, strValue, idTemp, false);
				double dHeight = GetTextHeight(aname(idTemp));
				m_objArrSave.append(idTemp);
				if (idTemp != AcDbObjectId::kNull)
				{
					ads_point ptUnit;
					GetEntPosition(aname(idTemp), ptUnit);
					int nlen = strTemp.GetLength();
					while (strTemp.GetLength() > 0)
					{
						ads_polar(ptUnit, -1 * PI / 2, dHeight*1.5, ptUnit);
						CString strUnit;
						TCHAR ch = strTemp.GetAt(0);
						if (ch > 127 || ch < 0)
						{
							strUnit.Format(_T("%s"), strTemp.Left(2));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 2));
						}
						else
						{
							strUnit.Format(_T("%s"), strTemp.Left(1));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 1));
						}
						AcDbObjectId idText = DrawText(ptUnit, strUnit, _T("MC"), _T(""), dHeight);
						m_objArrSave.append(idText);

					}
				}
			}
			else
			{
				m_blockB.replaceText(strKey, strValue);
			}

		}

		it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		int nIndex_Floor = 0;
		double dAngle = 0.0;
		double dAngle_Ro=0.0;
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y] + dStep_Total*nIndex_Floor + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total*(nIndex_Floor+1);

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			double dis_Max =0.0;
			for (int ii = 0; ii < idArrTemp.length(); ii ++)
			{
				ads_point ptMin_temp , ptMax_temp ;
				GetEntityTrend(aname(idArrTemp.at(ii)),ptMin_temp , ptMax_temp);
				double dis_Max_temp= distOF2d(apoint(ptMin_temp),apoint(ptMax_temp));
				if (dis_Max<dis_Max_temp)
				{
					ptMin[X]=ptMin_temp[X];ptMin[Y]=ptMin_temp[Y];
					ptMax[X]=ptMax_temp[X];ptMax[Y]=ptMax_temp[Y];
				}
			}
			ads_point ptMin_Ro={0} , ptMax_Ro={0};
			ptMin_Ro[X]=ptMin[X];ptMax_Ro[X]=ptMax[X];
			ptMin_Ro[Y]=ptMin[Y];ptMax_Ro[Y]=ptMax[Y];
			ptMin_Ro[Z]=0;ptMax_Ro[Z]=0;
			AcGePoint3d ptBase;
			ptBase[X]=ptMin_Ro[X];ptBase[Y]=ptMin_Ro[Y];ptBase[Z]=ptMin_Ro[Z];

			if (nIndex_Floor==0)
			{
				dAngle = ads_angle(ptMin_Ro , ptMax_Ro);
				double dTemp = -dAngle / PI;
				while (dTemp + 0.5 < 0)
				{
					dTemp += 1;
				}

				while (dTemp - 0.5 > 0)
				{
					dTemp -= 1;
				}
				dAngle_Ro= dTemp * PI;
			}
			for (int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				RotateEntity(aname(idArrTemp.at(nIndex)),dAngle_Ro,ptBase);
			}
			GetEntsExtent(idArrTemp , ptMin , ptMax);

			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			// 			double dscale1 = ( dNKWidth / dw );
			// 			double dscale2 = ( dStep / dh );
			// 			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			// 			m_dScale = dNewScale;
			// 			double l = m_dH * dNewScale;
			// 			double dTemp = m_dH * 1000.0 / l;
			// 			int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
			// 			dTemp = nTemp * 10;
			// 			TCHAR szValue[100] = {0};
			// 			_stprintf(szValue, _T("1:%.f"), dTemp);
			// 
			// 			m_blockB.replaceText(_T("#SCALE#") , szValue);

			TCHAR szFlage[100] = {0};
			AcDbObjectId objIdTable;
			vector<CString> veFloorName;
			AnalyzeData(veFloorName , it_FloorNum_IdArr->first , _T(","));
			insertNodeFloor_Simple(idArrTemp , ptNKMin , ptNKMax , veFloorName.size() , veFloorName,nIndex_Floor+1);
			AcDbObjectIdArray idArrText;
			//NoteJZXDist_Simple(idArrTemp , idArrText);
			NoteJZXDist(idArrTemp , idArrText);
			//m_objArrSave.append(idArrText);
			for(int nIndex = 0; nIndex < idArrText.length(); nIndex ++)
			{
				MoveObject(idArrText.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrText.at(nIndex)), ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrText);

			for(int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				MoveObject(idArrTemp.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrTemp.at(nIndex)) , ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrTemp);
			NoteJZXDist_Final(idArrTemp , idArrText);
			m_objArrSave.append(idArrText);
			ReNoteHouse(idArrTemp);
			nIndex_Floor++;
		}
		m_strJZMJ.Format(_T("%.2f"),JZMJ);
		m_blockB.replaceText(_T("#ZYJZMJ#") , m_strJZMJ);
		m_blockB.replaceText(_T("#JZMJ#") , m_strJZMJ);


		m_objArrSave.append(m_blockB.m_objIdArr);

		ads_name ssAll , ssSave;
		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		IdArrToSS(m_objArrSave , ssSave);
		SubssName(ssAll , ssSave);
		EntsDel(ssAll);
		acedSSFree(ssSave);
		acedSSFree(ssAll);

		CXRecordOperate xRecord;

		//xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
		AcDbObjectId idNorth = DrawInsert(apoint(m_ptInsertNorth) , _T("north1"));

		//RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);

		double dTemp = (PI / 2 - dAngle) / PI;
		if (dTemp < 0)
		{
			while (dTemp < 0)
			{
				dTemp += 1.0;
			}
		}
		else if (dTemp > 1)
		{
			while (dTemp - 1.0 > 0)
			{
				dTemp -= 1.0;
			}
		}
		RotateEntity(aname(idNorth), dTemp*PI, m_ptInsertNorth);
		m_objArrSave.append(idNorth);

		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		m_objArrSave.setLogicalLength(0);
		SSToIdArr(ssAll , m_objArrSave);

		ZoomWindow(apoint(m_pt3dWLBTK) ,  apoint(m_pt3dWRTTK));

		CSaveBlocks saveB;
		saveB.setPoint(apoint(m_pt3dWLBTK));

		CString	 strTempSavePath;
		CString strTempPath;
		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
		{
			strTempPath.Format(_T("%s%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"),strTempPath , m_strZDDM , m_strZH);
		}
		else
		{
			strTempPath.Format(_T("%s\\%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"), strTempPath , m_strZDDM , m_strZH);
		}
		m_strTempSavePath.Format(_T("%s") , strTempSavePath);
		RemoveSameObjects(m_objArrSave);
		saveB.setPath(strTempSavePath);
		saveB.saveBlocks2(m_objArrSave);
		actrTransactionManager->abortTransaction();
	}
	acutPrintf(_T("输出完成！"));
	return TRUE;
}

bool CExportHouseDiagram::buildMainBlocks_Simple_Zhijiang()
{
	int zrz_num=0;
	logic_house=1;
	map<CString , ZRZinfo>::iterator it_BDCBH_ZRZINFO = m_mapBDCBH_ZRZinfo.begin();
	for(; it_BDCBH_ZRZINFO != m_mapBDCBH_ZRZinfo.end(); it_BDCBH_ZRZINFO++)
	{
		vec_logic.clear();
		int logic_num_up =it_BDCBH_ZRZINFO->second.logic_num+logic_house;
		zrz_num++;
		actrTransactionManager->startTransaction();
		m_objArrSave.setLogicalLength(0);
		ads_point ptInsert;
		ptInsert[X] = 0;
		ptInsert[Y] = 0;
		AcDbObjectId idTK;
		CString model_name;
		model_name.Format(_T("房产分户图_枝江%d"),it_BDCBH_ZRZINFO->second.logic_num);
		idTK = DrawInsert(ptInsert,model_name);
		if (idTK == AcDbObjectId::kNull)
		{
			CString aa;
			aa.Format(_T("找不到模板%s"),model_name);
			AfxMessageBox(aa);
			return false;
		}
		m_objTukuang = idTK;
		SBreakBlock m_blockB;
		m_blockB.SetBlock(idTK);
		m_blockB.Explode();


		AcGePoint3d m_pt3dWLBTK;
		AcGePoint3d m_pt3dWRTTK;
		AcGePoint3d m_ptInsertNorth;
		AcGePoint3d m_pt3dWLBTABLE;
		if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("右上角"), m_pt3dWRTTK) == false)
			{
				AfxMessageBox(_T("模板右上角点无法找到."));
				return false;
			}
		}

		if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("左下角"), m_pt3dWLBTK) == false)
			{
				AfxMessageBox(_T("模版左下角点无法找到。"));
				return false;
			}
		}


		if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
			{
				AfxMessageBox(_T("模版指北针插入点无法找到。"));
				return false;
			}
		}
		if ( it_BDCBH_ZRZINFO->first.GetLength() >=  19)
		{
			m_strZDDM.Format(_T("%s") ,  it_BDCBH_ZRZINFO->first.Left(19));
		}	
		m_strZH.Format(_T("%s") ,  it_BDCBH_ZRZINFO->second.strZH);
		m_strHH = it_BDCBH_ZRZINFO->second.strHH;
		m_strZL = it_BDCBH_ZRZINFO->second.strZL;
		m_strZCS = it_BDCBH_ZRZINFO->second.strZCS;
		m_strSZC = it_BDCBH_ZRZINFO->second.strSZC;
		m_strJG = it_BDCBH_ZRZINFO->second.strJG;
		vector<CString> saDatas;
		AnalyzeData(saDatas ,m_strJG , _T(" "));
		if (saDatas.size() >= 2)
		{
			m_strJG = saDatas[1];
		}
		m_strZYJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strZYJZMJ));
		m_strJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strJZMJ));
		m_strFTMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strFTMJ));		
		m_strZCS.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strZCS);
		m_strSZC.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strSZC);
		m_strQLR.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strQLR);
		m_blockB.replaceText(_T("#zddm#") , m_strZDDM);
		m_blockB.replaceText(_T("#zrzh#") , m_strZH);
		m_blockB.replaceText(_T("#hh#") , m_strHH);
		m_blockB.replaceText(_T("#fdzl#") , m_strZL);

		//m_blockB.replaceText(_T("#fwjg#") , m_strJG);

		//m_blockB.replaceText(_T("#zcs#") , m_strZCS);
		//m_blockB.replaceText(_T("#ZYJZMJ#") , m_strZYJZMJ);
		//m_blockB.replaceText(_T("#FTJZMJ#") , m_strFTMJ);
		//m_blockB.replaceText(_T("#JZMJ#") , m_strJZMJ);
		//m_blockB.replaceText(_T("#szc#") , m_strSZC);

		for (int i = 0; i < m_strArrKey.GetSize(); ++i)
		{
			CString strKey = m_strArrKey.GetAt(i);
			CString strValue = m_strArrValue.GetAt(i);
			if (strKey.GetLength() < 1)
				continue;
			if (0 == _tcscmp(strKey, _T("#UNIT#")) && strValue.GetLength() > 0)
			{
				CString strTemp;
				strTemp = strValue;
				strValue.Format(_T(""));
				strValue += strTemp.Mid(0, 0);
				AcDbObjectId idTemp = AcDbObjectId::kNull;
				m_blockB.replaceText(strKey, strValue, idTemp, false);
				double dHeight = GetTextHeight(aname(idTemp));
				m_objArrSave.append(idTemp);
				if (idTemp != AcDbObjectId::kNull)
				{
					ads_point ptUnit;
					GetEntPosition(aname(idTemp), ptUnit);
					int nlen = strTemp.GetLength();
					while (strTemp.GetLength() > 0)
					{
						ads_polar(ptUnit, -1 * PI / 2, dHeight*1.5, ptUnit);
						CString strUnit;
						TCHAR ch = strTemp.GetAt(0);
						if (ch > 127 || ch < 0)
						{
							strUnit.Format(_T("%s"), strTemp.Left(2));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 2));
						}
						else
						{
							strUnit.Format(_T("%s"), strTemp.Left(1));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 1));
						}
						AcDbObjectId idText = DrawText(ptUnit, strUnit, _T("MC"), _T(""), dHeight);
						m_objArrSave.append(idText);

					}
				}
			}
			else
			{
				m_blockB.replaceText(strKey, strValue);
			}

		}

		double dWidth = m_pt3dWRTTK.x - m_pt3dWLBTK.x;
		double dHigh = m_pt3dWRTTK.y - m_pt3dWLBTK.y;

		int nFloorCount = 0;
		map<CString , AcDbObjectIdArray>::iterator it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			nFloorCount++;
		}
		if (nFloorCount == 0)
		{
			continue;
		}
		double dStep_Total = dHigh / nFloorCount;
		double dStep = dStep_Total * 0.6;

		vector<double> veDNewScale ;
		for (it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			idArrTemp.append(it_FloorNum_IdArr->second);
			CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y]  + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total;

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			GetEntsExtent(idArrTemp , ptMin , ptMax);
			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			double dscale1 = ( dNKWidth / dw );
			double dscale2 = ( dStep / dh );
			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			m_dScale = dNewScale;
			veDNewScale.push_back(dNewScale);
		}

		sort(veDNewScale.begin() , veDNewScale.end());

		if (veDNewScale.size() > 0)
		{
			m_dScale = veDNewScale.at(0);
			double l = m_dH * m_dScale;
			double dTemp = m_dH * 1000.0 / l;
			int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
			dTemp = nTemp * 50;
			TCHAR szValue[100] = {0};
			_stprintf(szValue, _T("1:%.f"), dTemp);

			m_blockB.replaceText(_T("#SCALE#") , szValue);
		}

		it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		int nIndex_Floor = 0;
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y] + dStep_Total*nIndex_Floor + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total*(nIndex_Floor+1);

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			GetEntsExtent(idArrTemp , ptMin , ptMax);
			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			// 			double dscale1 = ( dNKWidth / dw );
			// 			double dscale2 = ( dStep / dh );
			// 			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			// 			m_dScale = dNewScale;
			// 			double l = m_dH * dNewScale;
			// 			double dTemp = m_dH * 1000.0 / l;
			// 			int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
			// 			dTemp = nTemp * 10;
			// 			TCHAR szValue[100] = {0};
			// 			_stprintf(szValue, _T("1:%.f"), dTemp);
			// 
			// 			m_blockB.replaceText(_T("#SCALE#") , szValue);

			TCHAR szFlage[100] = {0};
			AcDbObjectId objIdTable;
			vector<CString> veFloorName;
			AnalyzeData(veFloorName , it_FloorNum_IdArr->first , _T(","));
			insertNodeFloor_zhijiang(idArrTemp , ptNKMin , ptNKMax , veFloorName.size() , veFloorName);
			AcDbObjectIdArray idArrText;
			//NoteJZXDist_Simple(idArrTemp , idArrText);
			NoteJZXDist(idArrTemp , idArrText);
			//m_objArrSave.append(idArrText);
			for(int nIndex = 0; nIndex < idArrText.length(); nIndex ++)
			{
				MoveObject(idArrText.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrText.at(nIndex)), ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrText);

			for(int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				MoveObject(idArrTemp.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrTemp.at(nIndex)) , ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrTemp);
			NoteJZXDist_Final(idArrTemp , idArrText);
			m_objArrSave.append(idArrText);
			ReNoteHouse_zhijiang(idArrTemp,m_dScale,nIndex_Floor,logic_house);
			nIndex_Floor++;
		}
		//填写逻辑幢信息
		double hj1=0;
		double hj2=0;
		for (int lgc=0;lgc<it_BDCBH_ZRZINFO->second.logic_num;lgc++)
		{
			CString strlogic,logic_num,fwjg,zyjzmj,jzmj,cs;
			strlogic.Format(_T("#lg%d#"),lgc+1);
			fwjg.Format(_T("#fwjg%d#"),lgc+1);
			zyjzmj.Format(_T("#ZYJZMJ%d#"),lgc+1);
			jzmj.Format(_T("#JZMJ%d#"),lgc+1);
			cs.Format(_T("#cs%d#"),lgc+1);
			logic_num.Format(_T("%d"),logic_house);
			m_blockB.replaceText(strlogic , logic_num);
			vector<LOGIC> veclogic_temp;
			for (int logic_num_temp=0;logic_num_temp<vec_logic.size();logic_num_temp++)
			{
				if (vec_logic[logic_num_temp].logic_num==logic_house)
				{
					veclogic_temp.push_back(vec_logic[logic_num_temp]);
				}
			}
			CString strFWJG,strCS,strZYJZMJ,strJZMJ;
			double n_ZYJZMJ=0.0;
			double n_JZMJ=0.0;
			int cs_num=0;
			for (int temp_logic_num=0;temp_logic_num<veclogic_temp.size();temp_logic_num++)
			{
				CString temp;
				double JZMJ_temp =0.0;
				//层数
				int cs_num_temp;
				cs_num_temp =veclogic_temp[temp_logic_num].cs.at(0);
				if (temp_logic_num==0)
				{
					cs_num =cs_num_temp;
				}
				else
				{
					if (cs_num_temp>cs_num)
					{
						cs_num=cs_num_temp;
					}
				}
				temp.Format(_T("%d"),cs_num);
				strCS =temp;
				//占用建筑面积

				n_ZYJZMJ +=veclogic_temp[temp_logic_num].logic_area.at(0);
				JZMJ_temp =veclogic_temp[temp_logic_num].logic_area.at(0);

				//建筑面积
				if (temp_logic_num==0)
				{
					n_JZMJ =JZMJ_temp;
				}
				//结构
				strFWJG = veclogic_temp[temp_logic_num].fwjg;
			}
				
			strZYJZMJ.Format(_T("%.2f"),n_ZYJZMJ);
			strJZMJ.Format(_T("%.2f"),n_JZMJ);
			m_blockB.replaceText(fwjg,strFWJG);
			m_blockB.replaceText(zyjzmj,strZYJZMJ);
			m_blockB.replaceText(jzmj,strJZMJ);
			m_blockB.replaceText(cs,strCS);
			hj1 += n_ZYJZMJ;
			hj2 +=n_JZMJ;
			if (logic_house>logic_num_up)
			{
				break;
			}
			logic_house++;
		}
		CString strHJ1,strHJ2;
		strHJ1.Format(_T("%.2f"),hj1);
		strHJ2.Format(_T("%.2f"),hj2);
		m_blockB.replaceText(_T("#HJ1#"),strHJ1);
		m_blockB.replaceText(_T("#HJ2#"),strHJ2);
;
		m_objArrSave.append(m_blockB.m_objIdArr);


		ads_name ssAll , ssSave;
		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		IdArrToSS(m_objArrSave , ssSave);
		SubssName(ssAll , ssSave);
		EntsDel(ssAll);
		acedSSFree(ssSave);
		acedSSFree(ssAll);

		CXRecordOperate xRecord;
		double dAngle = 0.0;
		xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
		AcDbObjectId idNorth = DrawInsert(apoint(m_ptInsertNorth) , _T("north1"));

		RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);
		m_objArrSave.append(idNorth);

		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		m_objArrSave.setLogicalLength(0);
		SSToIdArr(ssAll , m_objArrSave);
		acedSSFree(ssAll);
		ZoomWindow(apoint(m_pt3dWLBTK) ,  apoint(m_pt3dWRTTK));

		CSaveBlocks saveB;
		saveB.setPoint(apoint(m_pt3dWLBTK));

		CString	 strTempSavePath;
		CString strTempPath;
		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
		{
			strTempPath.Format(_T("%s%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"), strTempPath , m_strZDDM , m_strZH);
		}
		else
		{
			strTempPath.Format(_T("%s\\%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"), strTempPath, m_strZDDM , m_strZH);
		}
		m_strTempSavePath.Format(_T("%s") , strTempSavePath);
		RemoveSameObjects(m_objArrSave);
		saveB.setPath(strTempSavePath);
		saveB.saveBlocks2(m_objArrSave);
		actrTransactionManager->abortTransaction();
		
	}
	acutPrintf(_T("输出完成！\n"));
	return TRUE;
}

bool CExportHouseDiagram::buildMainBlocks_Simple_ShiShou()
{
	int nReportType =0;
	//if(RTNORM != ads_getint(_T(""),&nReportType));
	if(RTNORM != ads_getint(_T("\n生成比例尺方式:（0）自动（1）手动<0>"),&nReportType));

	map<CString , ZRZinfo>::iterator it_BDCBH_ZRZINFO = m_mapBDCBH_ZRZinfo.begin();
	for(; it_BDCBH_ZRZINFO != m_mapBDCBH_ZRZinfo.end(); it_BDCBH_ZRZINFO++)
	{
		actrTransactionManager->startTransaction();
		m_objArrSave.setLogicalLength(0);
		ads_point ptInsert;
		ptInsert[X] = 0;
		ptInsert[Y] = 0;

		int nFloorCount = 0;
		map<CString , AcDbObjectIdArray>::iterator it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			nFloorCount++;
		}
		if (nFloorCount == 0)
		{
			continue;
		}

		AcDbObjectId idTK;
		CString model_name;
		model_name.Format(_T("房产分户图_石首%d"),nFloorCount);
		idTK = DrawInsert(ptInsert,model_name);
		if (idTK == AcDbObjectId::kNull)
		{
			CString aa;
			aa.Format(_T("找不到模板%s"),model_name);
			AfxMessageBox(aa);
			return false;
		}
		m_objTukuang = idTK;
		SBreakBlock m_blockB;
		m_blockB.SetBlock(idTK);
		m_blockB.Explode();

 		AcGePoint3d m_pt3dWLBTK;
 		AcGePoint3d m_pt3dWRTTK;
		AcGePoint3d m_ptInsertNorth;
		AcGePoint3d m_pt3dWLBTABLE;
		if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("右上角"), m_pt3dWRTTK) == false)
			{
				AfxMessageBox(_T("模板右上角点无法找到."));
				return false;
			}
		}

		if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("左下角"), m_pt3dWLBTK) == false)
			{
				AfxMessageBox(_T("模版左下角点无法找到。"));
				return false;
			}
		}


		if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
			{
				AfxMessageBox(_T("模版指北针插入点无法找到。"));
				return false;
			}
		}
		if ( it_BDCBH_ZRZINFO->first.GetLength() >=  19)
		{
			m_strZDDM.Format(_T("%s") ,  it_BDCBH_ZRZINFO->first.Left(19));
		}	
		m_strZH.Format(_T("%s") ,  it_BDCBH_ZRZINFO->second.strZH);
		m_strHH = it_BDCBH_ZRZINFO->second.strHH;
		m_strZL = it_BDCBH_ZRZINFO->second.strZL;
		m_strZCS = it_BDCBH_ZRZINFO->second.strZCS;
		m_strSZC = it_BDCBH_ZRZINFO->second.strSZC;
		m_strJG = it_BDCBH_ZRZINFO->second.strJG;
		vector<CString> saDatas;
		AnalyzeData(saDatas ,m_strJG , _T(" "));
		if (saDatas.size() >= 2)
		{
			m_strJG = saDatas[1];
		}
		m_strZYJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strZYJZMJ));
		m_strJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strJZMJ));
		m_strFTMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strFTMJ));		
		m_strZCS.Format(_T("%d") , it_BDCBH_ZRZINFO->second.mapFloor_IdArr.size());
		m_strSZC.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strSZC);
		m_strQLR.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strQLR);
		m_blockB.replaceText(_T("#zddm#") , m_strZDDM);
		

		m_blockB.replaceText(_T("#ZYJZMJ#") , m_strZYJZMJ);
		m_blockB.replaceText(_T("#FTJZMJ#") , m_strFTMJ);
		m_blockB.replaceText(_T("#szc#") , m_strSZC);



// 		double dWidth = m_pt3dWRTTK.x - m_pt3dWLBTK.x;
// 		double dHigh = m_pt3dWRTTK.y - m_pt3dWLBTK.y;
// 
// 		double dStep_Total = dHigh / nrowCount;
// 		double dStep = dStep_Total * 0.6;
		


		vector<double> veDNewScale ;
		int nIndex_Floor = 0;
		for (it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{

			AcGePoint3d m_pt3dWLBTK1;
			AcGePoint3d m_pt3dWRTTK1;
			TCHAR szRT[50]={0},szLD[50]={0};
			_stprintf(szRT,_T("右上角%d"),nIndex_Floor);
			_stprintf(szLD,_T("左下角%d"),nIndex_Floor);
			if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK1) == false)
			{
				if (m_blockB.findPointByHyperLink(szRT, m_pt3dWRTTK1) == false)
				{
					CString aa;
					aa.Format(_T("模板%s点无法找到."),szRT);
					AfxMessageBox(aa);
					return false;
				}
			}

			if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK1) == false)
			{
				if (m_blockB.findPointByHyperLink(szLD, m_pt3dWLBTK1) == false)
				{
					CString aa;
					aa.Format(_T("模版%s点无法找到。"),szLD);
					AfxMessageBox(aa);
					return false;
				}
			}

			double dWidth = m_pt3dWRTTK1.x - m_pt3dWLBTK1.x;
			double dHigh = m_pt3dWRTTK1.y - m_pt3dWLBTK1.y;

			double dStep_Total = dHigh;
			double dStep = dStep_Total * 0.6;


			AcDbObjectIdArray idArrTemp;
			idArrTemp.append(it_FloorNum_IdArr->second);
			//CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			//m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK1[X];
			ptNKMin[Y] = m_pt3dWLBTK1[Y];

			ptNKMax[X] = m_pt3dWRTTK1[X];
			ptNKMax[Y] = m_pt3dWLBTK1[Y] + dStep_Total;

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			GetEntsExtent(idArrTemp , ptMin , ptMax);

			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			double dscale1 = ( dNKWidth / dw );
			double dscale2 = ( dStep / dh );
			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			//m_dScale = dNewScale;
			veDNewScale.push_back(dNewScale);
			nIndex_Floor++;
		}

		sort(veDNewScale.begin() , veDNewScale.end());
		if (nReportType==0)
		{
			if (veDNewScale.size() > 0)
			{
				m_dScale = veDNewScale.at(0);
				double l = m_dH * m_dScale;
				double dTemp = m_dH * 1000.0 / l;
				int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
				if (nTemp==1)
				{
					nTemp=2;
				}
				dTemp = nTemp * 50;
				TCHAR szValue[100] = {0};
				_stprintf(szValue, _T("1:%.f"), dTemp);
				m_dScale=1000/dTemp;
				m_blockB.replaceText(_T("#SCALE#") , szValue);
			}
		}
		else
		{
			int nReportType1 = 300;
			while (true)
			{
				if(RTNORM != ads_getint(_T("\n请输入比例尺1:<300>"),&nReportType1));
				if (nReportType1==0)
				{
					acutPrintf(_T("比例尺不正确"));
					continue;
				}
				else
				{
					break;
				}
			}

			m_dScale = 1000/nReportType1;
			TCHAR szValue[100] = {0};
			_stprintf(szValue, _T("1:%d"), nReportType1);

			m_blockB.replaceText(_T("#SCALE#") , szValue);
		}


		//责任单位等字段输出、可在Floor_MatchField.txt中配置
		for (int i = 0; i < m_strArrKey.GetSize(); ++i)
		{
			CString strKey = m_strArrKey.GetAt(i);
			CString strValue = m_strArrValue.GetAt(i);
			if (strKey.GetLength() < 1)
				continue;
			if (0 == _tcscmp(strKey, _T("#UNIT#")) && strValue.GetLength() > 0)
			{
				CString strTemp;
				strTemp = strValue;
				strValue.Format(_T(""));
				strValue += strTemp.Mid(0, 0);
				AcDbObjectId idTemp = AcDbObjectId::kNull;
				m_blockB.replaceText(strKey, strValue, idTemp, false);
				double dHeight = GetTextHeight(aname(idTemp));
				m_objArrSave.append(idTemp);
				if (idTemp != AcDbObjectId::kNull)
				{
					ads_point ptUnit;
					GetEntPosition(aname(idTemp), ptUnit);
					int nlen = strTemp.GetLength();
					while (strTemp.GetLength() > 0)
					{
						ads_polar(ptUnit, -1 * PI / 2, dHeight*1.5, ptUnit);
						CString strUnit;
						TCHAR ch = strTemp.GetAt(0);
						if (ch > 127 || ch < 0)
						{
							strUnit.Format(_T("%s"), strTemp.Left(2));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 2));
						}
						else
						{
							strUnit.Format(_T("%s"), strTemp.Left(1));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 1));
						}
						AcDbObjectId idText = DrawText(ptUnit, strUnit, _T("MC"), _T(""), dHeight);
						m_objArrSave.append(idText);

					}
				}
			}
			else
			{
				m_blockB.replaceText(strKey, strValue);
			}

		}

		it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		nIndex_Floor = 0;
		double dAngle = 0.0;
		double dAngle_Ro=0.0;
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			m_blockB.replaceText(_T("#zrzh#") , m_strZH);
			m_blockB.replaceText(_T("#hh#") , m_strHH);
			m_blockB.replaceText(_T("#fdzl#") , m_strZL);
			m_blockB.replaceText(_T("#fwjg#") , m_strJG);
			m_blockB.replaceText(_T("#JZMJ#") , m_strJZMJ);
			m_blockB.replaceText(_T("#zcs#") , m_strZCS);
			AcGePoint3d m_pt3dWLBTK2;
			AcGePoint3d m_pt3dWRTTK2;
			TCHAR szRT[50]={0},szLD[50]={0};
			_stprintf(szRT,_T("右上角%d"),nIndex_Floor);
			_stprintf(szLD,_T("左下角%d"),nIndex_Floor);
			if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK2) == false)
			{
				if (m_blockB.findPointByHyperLink(szRT, m_pt3dWRTTK2) == false)
				{
					AfxMessageBox(_T("模板右上角点无法找到."));
					return false;
				}
			}

			if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK2) == false)
			{
				if (m_blockB.findPointByHyperLink(szLD, m_pt3dWLBTK2) == false)
				{
					AfxMessageBox(_T("模版左下角点无法找到。"));
					return false;
				}
			}

			double dWidth = m_pt3dWRTTK2.x - m_pt3dWLBTK2.x;
			double dHigh = m_pt3dWRTTK2.y - m_pt3dWLBTK2.y;

			double dStep_Total = dHigh;
			double dStep = dStep_Total * 0.6;

			AcDbObjectIdArray idArrTemp;
			CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK2[X];
			ptNKMin[Y] = m_pt3dWLBTK2[Y];

			ptNKMax[X] = m_pt3dWRTTK2[X];
			ptNKMax[Y] = m_pt3dWRTTK2[Y];

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			double dis_Max =0.0;
			for (int ii = 0; ii < idArrTemp.length(); ii ++)
			{
				ads_point ptMin_temp , ptMax_temp ;
				GetEntityTrend(aname(idArrTemp.at(ii)),ptMin_temp , ptMax_temp);
				double dis_Max_temp= distOF2d(apoint(ptMin_temp),apoint(ptMax_temp));
				if (dis_Max<dis_Max_temp)
				{
					ptMin[X]=ptMin_temp[X];ptMin[Y]=ptMin_temp[Y];
					ptMax[X]=ptMax_temp[X];ptMax[Y]=ptMax_temp[Y];
				}
			}

			ads_point ptMin_Ro={0} , ptMax_Ro={0};
			ptMin_Ro[X]=ptMin[X];ptMax_Ro[X]=ptMax[X];
			ptMin_Ro[Y]=ptMin[Y];ptMax_Ro[Y]=ptMax[Y];
			ptMin_Ro[Z]=0;ptMax_Ro[Z]=0;
			AcGePoint3d ptBase;
			ptBase[X]=ptMin_Ro[X];ptBase[Y]=ptMin_Ro[Y];ptBase[Z]=ptMin_Ro[Z];

			if (nIndex_Floor==0)
			{
				dAngle = ads_angle(ptMin_Ro , ptMax_Ro);
				double dTemp = -dAngle / PI;
				while (dTemp + 0.5 < 0)
				{
					dTemp += 1;
				}

				while (dTemp - 0.5 > 0)
				{
					dTemp -= 1;
				}
				dAngle_Ro= dTemp * PI;
			}
			for (int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				RotateEntity(aname(idArrTemp.at(nIndex)),dAngle_Ro,ptBase);
			}
			GetEntsExtent(idArrTemp , ptMin , ptMax);

			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			// 			double dscale1 = ( dNKWidth / dw );
			// 			double dscale2 = ( dStep / dh );
			// 			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			// 			m_dScale = dNewScale;
			// 			double l = m_dH * dNewScale;
			// 			double dTemp = m_dH * 1000.0 / l;
			// 			int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
			// 			dTemp = nTemp * 10;
			// 			TCHAR szValue[100] = {0};
			// 			_stprintf(szValue, _T("1:%.f"), dTemp);
			// 
			// 			m_blockB.replaceText(_T("#SCALE#") , szValue);

			TCHAR szFlage[100] = {0};
			AcDbObjectId objIdTable;
			vector<CString> veFloorName;
			AnalyzeData(veFloorName , it_FloorNum_IdArr->first , _T(","));
			double Area_now=0.0;
			Area_now = insertNodeFloor_Simple_ShiShou(idArrTemp , ptNKMin , ptNKMax , veFloorName.size(),nIndex_Floor+1 , veFloorName,Area_now);
			TCHAR floor_Area[50]={0};
			TCHAR floor_word[50]={0};
			_stprintf(floor_Area,_T("%.2f"),Area_now);
			_stprintf(floor_word,_T("#gcmj%d#"),nIndex_Floor);
			m_blockB.replaceText(floor_word , floor_Area);
			AcDbObjectIdArray idArrText;
			//NoteJZXDist_Simple(idArrTemp , idArrText);
			NoteJZXDist(idArrTemp , idArrText);
			//m_objArrSave.append(idArrText);
			for(int nIndex = 0; nIndex < idArrText.length(); nIndex ++)
			{
				MoveObject(idArrText.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrText.at(nIndex)), ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrText);

			for(int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				MoveObject(idArrTemp.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrTemp.at(nIndex)) , ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrTemp);
			NoteJZXDist_Final(idArrTemp , idArrText);
			m_objArrSave.append(idArrText);
			ReNoteHouse(idArrTemp);
			nIndex_Floor++;
		}
		m_objArrSave.append(m_blockB.m_objIdArr);

		ads_name ssAll , ssSave;
		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		IdArrToSS(m_objArrSave , ssSave);
		SubssName(ssAll , ssSave);
		EntsDel(ssAll);
		acedSSFree(ssSave);
		acedSSFree(ssAll);

		CXRecordOperate xRecord;
		
		//xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
		AcDbObjectId idNorth = DrawInsert(apoint(m_ptInsertNorth) , _T("north1"));

		//RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);
		double dTemp = (PI / 2 - dAngle) / PI;
		if (dTemp < 0)
		{
			while (dTemp < 0)
			{
				dTemp += 1.0;
			}
		}
		else if (dTemp > 1)
		{
			while (dTemp - 1.0 > 0)
			{
				dTemp -= 1.0;
			}
		}
		RotateEntity(aname(idNorth), dTemp*PI, m_ptInsertNorth);

		m_objArrSave.append(idNorth);

		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		m_objArrSave.setLogicalLength(0);
		SSToIdArr(ssAll , m_objArrSave);

		ZoomWindow(apoint(m_pt3dWLBTK) ,  apoint(m_pt3dWRTTK));

		CSaveBlocks saveB;
		saveB.setPoint(apoint(m_pt3dWLBTK));

		CString	 strTempSavePath;
		CString strTempPath;
		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
		{
			strTempPath.Format(_T("%s%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"), strTempPath , m_strZDDM , m_strZH);
		}
		else
		{
			strTempPath.Format(_T("%s\\%s_%s\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001"), strTempPath , m_strZDDM , m_strZH);
		}
		m_strTempSavePath.Format(_T("%s") , strTempSavePath);
		RemoveSameObjects(m_objArrSave);
		saveB.setPath(strTempSavePath);

		//ScaleEntitys(m_objArrSave, m_pt3dWLBTK, 1 / m_dScale);

		saveB.saveBlocks2(m_objArrSave);
		actrTransactionManager->abortTransaction();
	}
	acutPrintf(_T("输出完成！"));
	return TRUE;
}

bool CExportHouseDiagram::buildMainBlocks_ZRZFloor_Sketch(int nReportType)
{
	

	map<CString , ZRZinfo>::iterator it_BDCBH_ZRZINFO = m_mapBDCBH_ZRZinfo.begin();
	for(; it_BDCBH_ZRZINFO != m_mapBDCBH_ZRZinfo.end(); it_BDCBH_ZRZINFO++)
	{
		actrTransactionManager->startTransaction();
		m_objArrSave.setLogicalLength(0);
		ads_point ptInsert;
		ptInsert[X] = 0;
		ptInsert[Y] = 0;
		AcDbObjectId idTK;
		idTK = DrawInsert(ptInsert, _T("分层草图_模板"));
		if (idTK == AcDbObjectId::kNull)
		{
			AfxMessageBox(_T("找不到模版分层草图_模板"));
			return false;
		}
		m_objTukuang = idTK;
		SBreakBlock m_blockB;
		m_blockB.SetBlock(idTK);
		m_blockB.Explode();

		AcGePoint3d m_pt3dWLBTK;
		AcGePoint3d m_pt3dWRTTK;
		AcGePoint3d m_ptInsertNorth;
		AcGePoint3d m_pt3dWLBTABLE;
		if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("右上角"), m_pt3dWRTTK) == false)
			{
				AfxMessageBox(_T("模板右上角点无法找到."));
				return false;
			}
		}

		if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("左下角"), m_pt3dWLBTK) == false)
			{
				AfxMessageBox(_T("模版左下角点无法找到。"));
				return false;
			}
		}


		if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
			{
				AfxMessageBox(_T("模版指北针插入点无法找到。"));
				return false;
			}
		}
		if ( it_BDCBH_ZRZINFO->first.GetLength() >=  19)
		{
			m_strZDDM.Format(_T("%s") ,  it_BDCBH_ZRZINFO->first.Left(19));
		}	
		m_strZH.Format(_T("%s") ,  it_BDCBH_ZRZINFO->second.strZH);
		m_strHH = it_BDCBH_ZRZINFO->second.strHH;
		m_strZL = it_BDCBH_ZRZINFO->second.strZL;
		m_strZCS = it_BDCBH_ZRZINFO->second.strZCS;
		m_strSZC = it_BDCBH_ZRZINFO->second.strSZC;
		m_strJG = it_BDCBH_ZRZINFO->second.strJG;
		vector<CString> saDatas;
		AnalyzeData(saDatas ,m_strJG , _T(" "));
		if (saDatas.size() >= 2)
		{
			m_strJG = saDatas[1];
		}
		m_strZYJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strZYJZMJ));
		m_strJZMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strJZMJ));
		m_strFTMJ.Format(_T("%.2f") , _tstof(it_BDCBH_ZRZINFO->second.strFTMJ));		
		m_strZCS.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strZCS);
		m_strSZC.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strSZC);
		m_strQLR.Format(_T("%s") , it_BDCBH_ZRZINFO->second.strQLR);
		m_blockB.replaceText(_T("#zddm#") , m_strZDDM);
		m_blockB.replaceText(_T("#zrzh#") , m_strZH);
		CString FWMC;
		FWMC.Format(_T("%s幢"),m_strZH);
		m_blockB.replaceText(_T("#JZWMC#") , FWMC);
		m_blockB.replaceText(_T("#hh#") , m_strHH);
		m_blockB.replaceText(_T("#fdzl#") , m_strZL);

		m_blockB.replaceText(_T("#FWJG#") , m_strJG);

		m_blockB.replaceText(_T("#zcs#") , m_strZCS);
		CString CC;
		CC.Format(_T("1-%s"),m_strZCS);
		m_blockB.replaceText(_T("#cc#") , CC);
		m_blockB.replaceText(_T("#ZYJZMJ#") , m_strZYJZMJ);
		m_blockB.replaceText(_T("#FTJZMJ#") , m_strFTMJ);
		m_blockB.replaceText(_T("#JZMJ#") , m_strJZMJ);
		m_blockB.replaceText(_T("#szc#") , m_strSZC);
		CString Title;
		Title.Format(_T("%s%s层分层草图"),FWMC,CC);
		m_blockB.replaceText(_T("#title#") , Title);


		double dWidth = m_pt3dWRTTK.x - m_pt3dWLBTK.x;
		double dHigh = m_pt3dWRTTK.y - m_pt3dWLBTK.y;

		int nFloorCount = 0;
		map<CString , AcDbObjectIdArray>::iterator it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			nFloorCount++;
		}
		if (nFloorCount == 0)
		{
			continue;
		}
		double dStep_Total = dHigh / nFloorCount;
		double dStep = dStep_Total * 0.6;

		vector<double> veDNewScale ;
		for (it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			idArrTemp.append(it_FloorNum_IdArr->second);
			//CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			//m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y]  + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total;

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			GetEntsExtent(idArrTemp , ptMin , ptMax);
			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			double dscale1 = ( dNKWidth / dw );
			double dscale2 = ( dStep / dh );
			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			//m_dScale = dNewScale;
			veDNewScale.push_back(dNewScale);
		}

		sort(veDNewScale.begin() , veDNewScale.end());
		if (nReportType==0)
		{
			if (veDNewScale.size() > 0)
			{
				m_dScale = veDNewScale.at(0);
				double l = m_dH * m_dScale;
				double dTemp = m_dH * 1000.0 / l;
				int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
				if (nTemp==1)
				{
					nTemp=2;
				}
				dTemp = nTemp * 50;
				m_dScale = 1000/dTemp ;
				TCHAR szValue[100] = {0};
				_stprintf(szValue, _T("1:%.f"), dTemp);

				m_blockB.replaceText(_T("#SCALE#") , szValue);
			}
		}
		else
		{
			int nReportType1 = 300;
			while (true)
			{
				if(RTNORM != ads_getint(_T("\n请输入比例尺1:<300>"),&nReportType1));
				if (nReportType1==0)
				{
					acutPrintf(_T("比例尺不正确"));
					continue;
				}
				else
				{
					break;
				}
			}

			m_dScale = 1000/nReportType1 ;
			TCHAR szValue[100] = {0};
			_stprintf(szValue, _T("1:%d"), nReportType1);

			m_blockB.replaceText(_T("#SCALE#") , szValue);
		}



		for (int i = 0; i < m_strArrKey.GetSize(); ++i)
		{
			CString strKey = m_strArrKey.GetAt(i);
			CString strValue = m_strArrValue.GetAt(i);
			if (strKey.GetLength() < 1)
				continue;
			if (0 == _tcscmp(strKey, _T("#UNIT#")) && strValue.GetLength() > 0)
			{
				CString strTemp;
				strTemp = strValue;
				strValue.Format(_T(""));
				strValue += strTemp.Mid(0, 0);
				AcDbObjectId idTemp = AcDbObjectId::kNull;
				m_blockB.replaceText(strKey, strValue, idTemp, false);
				double dHeight = GetTextHeight(aname(idTemp));
				m_objArrSave.append(idTemp);
				if (idTemp != AcDbObjectId::kNull)
				{
					ads_point ptUnit;
					GetEntPosition(aname(idTemp), ptUnit);
					int nlen = strTemp.GetLength();
					while (strTemp.GetLength() > 0)
					{
						ads_polar(ptUnit, -1 * PI / 2, dHeight*1.5, ptUnit);
						CString strUnit;
						TCHAR ch = strTemp.GetAt(0);
						if (ch > 127 || ch < 0)
						{
							strUnit.Format(_T("%s"), strTemp.Left(2));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 2));
						}
						else
						{
							strUnit.Format(_T("%s"), strTemp.Left(1));
							strTemp.Format(_T("%s"), strTemp.Right(strTemp.GetLength() - 1));
						}
						AcDbObjectId idText = DrawText(ptUnit, strUnit, _T("MC"), _T(""), dHeight);
						m_objArrSave.append(idText);

					}
				}
			}
			else
			{
				m_blockB.replaceText(strKey, strValue);
			}

		}
		double dAngle = 0.0;
		double dAngle_Ro=0.0;
		it_FloorNum_IdArr = it_BDCBH_ZRZINFO->second.mapFloor_IdArr.begin();
		int nIndex_Floor = 0;
		for (;it_FloorNum_IdArr != it_BDCBH_ZRZINFO->second.mapFloor_IdArr.end();it_FloorNum_IdArr++)
		{
			AcDbObjectIdArray idArrTemp;
			CopyEntitys(it_FloorNum_IdArr->second , idArrTemp);
			m_objArrSave.append(idArrTemp);
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y] + dStep_Total*nIndex_Floor + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total*(nIndex_Floor+1);

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			double dis_Max =0.0;
			for (int ii = 0; ii < idArrTemp.length(); ii ++)
			{
				ads_point ptMin_temp , ptMax_temp ;
				GetEntityTrend(aname(idArrTemp.at(ii)),ptMin_temp , ptMax_temp);
				double dis_Max_temp= distOF2d(apoint(ptMin_temp),apoint(ptMax_temp));
				if (dis_Max<dis_Max_temp)
				{
					ptMin[X]=ptMin_temp[X];ptMin[Y]=ptMin_temp[Y];
					ptMax[X]=ptMax_temp[X];ptMax[Y]=ptMax_temp[Y];
				}
			}
		    ads_point ptMin_Ro={0} , ptMax_Ro={0};
			ptMin_Ro[X] = ptMin[X]; ptMax_Ro[X] = ptMax[X];
			ptMin_Ro[Y] = ptMin[Y]; ptMax_Ro[Y] = ptMax[Y];
			ptMin_Ro[Z] = 0; ptMax_Ro[Z] = 0;
			AcGePoint3d ptBase;
			ptBase[X]=ptMin_Ro[X];ptBase[Y]=ptMin_Ro[Y];ptBase[Z]=ptMin_Ro[Z];
			if (nIndex_Floor==0)
			{
				dAngle = ads_angle(ptMin_Ro , ptMax_Ro);
				double dTemp = -dAngle / PI;
				while (dTemp + 0.5 < 0)
				{
					dTemp += 1;
				}

				while (dTemp - 0.5 > 0)
				{
					dTemp -= 1;
				}
				dAngle_Ro= dTemp * PI;
			}
			for (int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				RotateEntity(aname(idArrTemp.at(nIndex)),dAngle_Ro,ptBase);
			}
			GetEntsExtent(idArrTemp , ptMin , ptMax);
			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];

			// 			double dscale1 = ( dNKWidth / dw );
			// 			double dscale2 = ( dStep / dh );
			//			double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
			// 			m_dScale = dNewScale;
			// 			double l = m_dH * dNewScale;
			// 			double dTemp = m_dH * 1000.0 / l;
			// 			int nTemp = (int)(dTemp / 50 + 1 ); //比例最小为100
			// 			dTemp = nTemp * 10;
			// 			TCHAR szValue[100] = {0};
			// 			_stprintf(szValue, _T("1:%.f"), dTemp);
			// 
			// 			m_blockB.replaceText(_T("#SCALE#") , szValue);

			TCHAR szFlage[100] = {0};
			AcDbObjectId objIdTable;
			vector<CString> veFloorName;
			AnalyzeData(veFloorName , it_FloorNum_IdArr->first , _T(","));
			insertNodeFloor_zrzfloor_sketch(idArrTemp , ptNKMin , ptNKMax , veFloorName.size() , veFloorName,m_strZH);
			AcDbObjectIdArray idArrText;
			//NoteJZXDist_Simple(idArrTemp , idArrText);
			NoteJZXDist(idArrTemp , idArrText);
			//m_objArrSave.append(idArrText);
			for(int nIndex = 0; nIndex < idArrText.length(); nIndex ++)
			{
				MoveObject(idArrText.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrText.at(nIndex)), ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrText);

			for(int nIndex = 0; nIndex < idArrTemp.length(); nIndex ++)
			{
				MoveObject(idArrTemp.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrTemp.at(nIndex)) , ptNKMid , m_dScale);	
			}
			m_objArrSave.append(idArrTemp);
			NoteJZXDist_Final(idArrTemp , idArrText);
			m_objArrSave.append(idArrText);
			ReNoteHouse(idArrTemp);
			nIndex_Floor++;
		}
		m_objArrSave.append(m_blockB.m_objIdArr);

		ads_name ssAll , ssSave;
		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		IdArrToSS(m_objArrSave , ssSave);
		SubssName(ssAll , ssSave);
		EntsDel(ssAll);
		acedSSFree(ssSave);
		acedSSFree(ssAll);

		CXRecordOperate xRecord;
		
		//xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
		AcDbObjectId idNorth = DrawInsert(apoint(m_ptInsertNorth) , _T("north1"));

		double dTemp = (PI / 2 - dAngle) / PI;
		if (dTemp < 0)
		{
			while (dTemp < 0)
			{
				dTemp += 1.0;
			}
		}
		else if (dTemp > 1)
		{
			while (dTemp - 1.0 > 0)
			{
				dTemp -= 1.0;
			}
		}
		RotateEntity(aname(idNorth), dTemp*PI, m_ptInsertNorth);
		//RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);

// 		double dTemp = dAngle / PI;
// 		if (dTemp > -0.5 && dTemp < -0.01)
// 		{
// 			RotateEntity(aname(idNorth), PI / 2 - dAngle, m_ptInsertNorth);
// 		}
// 		else if (dTemp < -0.5 && dTemp > -1)
// 		{
// 			RotateEntity(aname(idNorth), PI / 2 - dAngle + PI, m_ptInsertNorth);
// 		}
// 		else if (dTemp > 0.5)
// 		{
// 			RotateEntity(aname(idNorth), PI / 2 - dAngle + PI, m_ptInsertNorth);
// 		}
// 		else
// 		{
// 			RotateEntity(aname(idNorth), PI / 2 - dAngle, m_ptInsertNorth);
// 		}

		m_objArrSave.append(idNorth);

		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		m_objArrSave.setLogicalLength(0);
		SSToIdArr(ssAll , m_objArrSave);

		ZoomWindow(apoint(m_pt3dWLBTK) ,  apoint(m_pt3dWRTTK));

		CSaveBlocks saveB;
		saveB.setPoint(apoint(m_pt3dWLBTK));

		CString	 strTempSavePath;
		CString  strTempPath;
		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
		{
			strTempPath.Format(_T("%s%s_%s\\调查成果\\分层草图\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001_分层草图"), strTempPath , m_strZDDM , m_strZH);
		}
		else
		{
			strTempPath.Format(_T("%s\\%s_%s\\调查成果\\分层草图\\") , m_strPath , m_strZDDM , m_strQLR);
			CreateDirectoryForbidOverWrite(strTempPath);
			strTempSavePath.Format(_T("%s%s_%s_0001_分层草图"), strTempPath , m_strZDDM , m_strZH);
		}
		m_strTempSavePath.Format(_T("%s") , strTempSavePath);
		RemoveSameObjects(m_objArrSave);
		saveB.setPath(strTempSavePath);

		//ScaleEntitys(m_objArrSave, m_pt3dWLBTK, 1 / m_dScale);

		saveB.saveBlocks2(m_objArrSave);
		actrTransactionManager->abortTransaction();
	}
	return TRUE;
}

bool CExportHouseDiagram::buildMainBlocks_new()
{
	int nReportType =0;
	if(RTNORM != ads_getint(_T("\n生成比例尺方式:（0）自动（1）手动<0>"),&nReportType));

	for (int nIndexMap = 0;nIndexMap < m_vector_map_Layer_IdArr.size();nIndexMap++)
	{
		JZMJ =0;
		OderLayerName(nIndexMap);
		actrTransactionManager->startTransaction();
		Show_All();
		m_objArrSave.setLogicalLength(0);
		ads_point ptInsert;
		ptInsert[X] = 0;
		ptInsert[Y] = 0;
		AcDbObjectId idTK;
		idTK = DrawInsert(ptInsert, _T("房产分户图_模板2"));
		if (idTK == AcDbObjectId::kNull)
		{
			AfxMessageBox(_T("找不到模版房产分户图_模板2"));
			return false;
		}
		m_objTukuang = idTK;
		SBreakBlock m_blockB;
		m_blockB.SetBlock(idTK);
		m_blockB.Explode();

		AcGePoint3d m_pt3dWLBTK;
		AcGePoint3d m_pt3dWRTTK;
		AcGePoint3d m_ptInsertNorth;
		AcGePoint3d m_pt3dWLBTABLE;
		if (m_blockB.findPointByHyperLink(_T("内框右上角"), m_pt3dWRTTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("右上角"), m_pt3dWRTTK) == false)
			{
				AfxMessageBox(_T("模板右上角点无法找到."));
				return false;
			}
		}

		if (m_blockB.findPointByHyperLink(_T("内框左下角"), m_pt3dWLBTK) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("左下角"), m_pt3dWLBTK) == false)
			{
				AfxMessageBox(_T("模版左下角点无法找到。"));
				return false;
			}
		}


		if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
		{
			if (m_blockB.findPointByHyperLink(_T("指北针插入点"), m_ptInsertNorth) == false)
			{
				AfxMessageBox(_T("模版指北针插入点无法找到。"));
				return false;
			}
		}

		TCHAR szValue[1000] = {0};
		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("ZDDM") , STRING_TYPE , szValue);
		m_strZDDM.Format(_T("%s") , szValue);
		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("ZH") , STRING_TYPE , szValue);
		m_strZH.Format(_T("%s") , szValue);

		CString strFamily;
		TCHAR szFamily[1000] = {0};
		ReadXdata(aname(m_IdArrExtern[nIndexMap]), _T("FamilyRoomList"), STRING_TYPE, szFamily);
		strFamily.Format(_T("%s") , szFamily);
		vector<CString> saDatas;
		AnalyzeData(saDatas ,strFamily , _T(";"));
		vector<CString>(m_veFSName).swap(m_veFSName);
		if (saDatas.size() >= 2)
		{
			m_strHH.Format(_T("%s") , saDatas[0]);
			map<CString , vector<CString> >::iterator itMap = m_mapRoom_Floor.find(m_strHH);
			if (itMap != m_mapRoom_Floor.end())
			{
				m_veFSName = itMap->second;
			}
		}


		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("ZL") , STRING_TYPE , szValue);
		m_strZL.Format(_T("%s") , szValue);

		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("ZH") , STRING_TYPE , szValue);
		m_strZH.Format(_T("%s") , szValue);



		int nToatal_LayerNum = 0;
		for (int i = 0;i < m_vector_Layer_Oder.size();i++)
		{
			nToatal_LayerNum += m_vector_Layer_Oder.at(i).nLayerCount;
		}
		m_strZCS.Format(_T("%d") , nToatal_LayerNum);

		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("FWJG") , STRING_TYPE , szValue);
		m_strJG.Format(_T("%s") , szValue);

		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("ZYJZMJ") , DOUBLE_TYPE , szValue);
		double dArea = 0.0;
		dArea = _tstof(szValue);
		m_strZYJZMJ.Format(_T("%.2f") , dArea);
		double zyjzmj_area=0.0;
		zyjzmj_area = _tstof(m_strZYJZMJ);

		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("JZMJ") , DOUBLE_TYPE , szValue);
		dArea = _tstof(szValue);
		m_strJZMJ.Format(_T("%.2f") , dArea);

		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("FTJZMJ") , DOUBLE_TYPE , szValue);
		dArea = _tstof(szValue);
		m_strFTMJ.Format(_T("%.2f") , dArea);
		double ftjzmj_area=0.0;
		ftjzmj_area =  _tstof(m_strFTMJ);

		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("ZCS") , INT_TYPE , szValue);
		m_strZCS.Format(_T("%s") , szValue);

		ReadXdata(aname(m_IdArrExtern[nIndexMap]) , _T("SZC") , STRING_TYPE , szValue);


		m_strSZC.Format(_T("%s") , szValue);

		vector<CString> saDatas_FS;
		map<CString , vector<CString> >::iterator itFind = m_mapRoom_Floor.find(m_strHH);
		if (itFind != m_mapRoom_Floor.end())
		{
			AnalyzeData(saDatas_FS , m_strSZC , _T(","));
			for (int i = 0;i < saDatas_FS.size();i++)
			{
				for(int j = 0;j < itFind->second.size();j++)
				{
					vector<CString> saDatas_Temp;
					AnalyzeData(saDatas_Temp , itFind->second[j] , _T("_"));
					if (saDatas_Temp.size() >= 2)
					{
						if (saDatas_Temp[0] == saDatas_FS[i])
						{
							saDatas_FS[i] = saDatas_Temp[1];
						}
					}
				}

			}
			m_strSZC = _T("");
		}
		
		for (int i = 0;i < saDatas_FS.size();i++)
		{
			
			m_strSZC += saDatas_FS[i];
			if (i != (saDatas_FS.size() -1))
			{
				m_strSZC += _T(",");
			}
			
		}

		m_blockB.replaceText(_T("#zddm#") , m_strZDDM);
		m_blockB.replaceText(_T("#zrzh#") , m_strZH);
		m_blockB.replaceText(_T("#hh#") , m_strHH);
		m_blockB.replaceText(_T("#fdzl#") , m_strZL);
		vector<CString> saDatasJG;
		AnalyzeData(saDatasJG,m_strJG,_T(" "));
		if (saDatasJG.size() >= 2)
		{
			m_blockB.replaceText(_T("#fwjg#") , saDatasJG[1]);
		}
		else
		{
			m_blockB.replaceText(_T("#fwjg#") , m_strJG);
		}

		m_blockB.replaceText(_T("#zcs#") , m_strZCS);
		//m_blockB.replaceText(_T("#JZMJ#"),m_strJZMJ);
		m_blockB.replaceText(_T("#FTJZMJ#") , m_strFTMJ);
		//m_blockB.replaceText(_T("#ZYJZMJ#"),m_strZYJZMJ);
		m_blockB.replaceText(_T("#szc#") , m_strSZC);

		for (int i = 0;i<m_strArrKey.GetSize();++i)
		{
			CString strKey=m_strArrKey.GetAt(i);
			CString strValue=m_strArrValue.GetAt(i);
			if(strKey.GetLength()<1)
				continue;
			if (0 == _tcscmp(strKey , _T("#UNIT#")) && strValue.GetLength() > 0)
			{
				CString strTemp;
				strTemp = strValue;
				strValue.Format(_T(""));
				strValue += strTemp.Mid(0 , 0);
				AcDbObjectId idTemp = AcDbObjectId::kNull;
				m_blockB.replaceText(strKey , strValue , idTemp , false);
				double dHeight = GetTextHeight(aname(idTemp));
				m_objArrSave.append(idTemp);
				if (idTemp != AcDbObjectId::kNull)
				{
					ads_point ptUnit;
					GetEntPosition(aname(idTemp) , ptUnit);
					int nlen = strTemp.GetLength();
					while (strTemp.GetLength() > 0)
					{
						
						ads_polar(ptUnit, -1 * PI / 2, dHeight * 1.5, ptUnit);
						CString strUnit;
						TCHAR ch = strTemp.GetAt(0);
						if (ch > 127 || ch < 0)
						{
							strUnit.Format(_T("%s") , strTemp.Left(2));
							strTemp.Format(_T("%s") , strTemp.Right(strTemp.GetLength() - 2));
						}
						else
						{
							strUnit.Format(_T("%s") , strTemp.Left(1));
							strTemp.Format(_T("%s") , strTemp.Right(strTemp.GetLength() - 1));
						} 						
						AcDbObjectId idText = DrawText(ptUnit, strUnit, _T("MC"), _T(""), dHeight);
						SetTextStyle(idText , m_strFont1.GetBuffer());
						m_strFont1.ReleaseBuffer();
						m_objArrSave.append(idText);
					}
				}
			}
			else
			{
				m_blockB.replaceText(strKey , strValue);
			}
		}
		double dWidth = m_pt3dWRTTK.x - m_pt3dWLBTK.x;
		double dHigh = m_pt3dWRTTK.y - m_pt3dWLBTK.y;

		double dStep_Total = dHigh / m_vector_Layer_Oder.size();
		double dStep = dStep_Total * 0.6;


		
		AcDbObjectIdArray idArrTextAll;
		for (int i = 0;i < m_vector_Layer_Oder.size();i++)
		{
			ads_point ptNKMin , ptNKMax;
			AcGePoint3d ptNKMid;
			ptNKMin[X] = m_pt3dWLBTK[X];
			ptNKMin[Y] = m_pt3dWLBTK[Y] + dStep_Total*i + dStep_Total * 0.4;

			ptNKMax[X] = m_pt3dWRTTK[X];
			ptNKMax[Y] = m_pt3dWLBTK[Y] + dStep_Total*(i+1);

			double dNKWidth = (ptNKMax[X] - ptNKMin[X]);
			double dNKHight = (ptNKMax[Y] - ptNKMin[Y]);

			ptNKMid.x = (ptNKMin[X] + ptNKMax[X]) / 2;
			ptNKMid.y = (ptNKMin[Y] + ptNKMax[Y]) / 2;
			ptNKMid.z = 0;

			ads_point ptMin , ptMax , ptMid;
			GetEntsExtent(m_vector_Layer_Oder.at(i).idArr , ptMin , ptMax);
			double dw = (ptMax[X] - ptMin[X]);
			double dh = (ptMax[Y] - ptMin[Y]);
			ptMid[X] = (ptMax[X] + ptMin[X])/2;
			ptMid[Y] = (ptMax[Y] + ptMin[Y])/2;

			double dYMove = ptNKMid.y - ptMid[Y];
			double dXMove = ptNKMid.x - ptMid[X];
			if (nReportType==0)
			{
				double dscale1 = ( dNKWidth / dw );
				double dscale2 = ( dStep / dh );
				double dNewScale = dscale1 < dscale2 ? dscale1 : dscale2;
				m_dScale = dNewScale;
				double l = dNewScale;
				double dTemp = (1000.0*m_dH) / l;
				dTemp = (int)(dTemp / 100 + 1 ); //比例最小为100
				if (dTemp == 1)
				{
					dTemp = 2;	
				}
				dTemp = dTemp * 100;
				TCHAR szValue[1000] = {0};
				_stprintf(szValue, _T("1:%.f"), dTemp);
				m_dScale = ((1000)/dTemp);
				m_blockB.replaceText(_T("#SCALE#") , szValue);
			}
			else
			{
				int nReportType1 = 300;
				while (true)
				{
					if(RTNORM != ads_getint(_T("\n请输入比例尺1:<300>"),&nReportType1));
					if (nReportType1==0)
					{
						acutPrintf(_T("比例尺不正确"));
						continue;
					}
					else
					{
						break;
					}
				}

				m_dScale = 1000/nReportType1;
				TCHAR szValue[100] = {0};
				_stprintf(szValue, _T("1:%d"), nReportType1);

				m_blockB.replaceText(_T("#SCALE#") , szValue);
			}

			TCHAR szFlage[1000] = {0};
			AcDbObjectId objIdTable;
			//insertNodeFloor(m_vector_Layer_Oder.at(i).idArr , ptNKMin , ptNKMax , m_vector_Layer_Oder.at(i).nLayerCount , m_vector_Layer_Oder.at(i).veStrFloorName);
			AcDbObjectIdArray idArrText;
			NoteJZXDist(m_vector_Layer_Oder.at(i).idArr , idArrText);
			//m_dScale = dNewScale;
			for(int nIndex = 0; nIndex < idArrText.length(); nIndex ++)
			{
				MoveObject(idArrText.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(idArrText.at(nIndex)), ptNKMid , m_dScale);
			}
			m_objArrSave.append(idArrText);

			for(int nIndex = 0; nIndex < m_vector_Layer_Oder.at(i).idArr.length(); nIndex ++)
			{
				MoveObject(m_vector_Layer_Oder.at(i).idArr.at(nIndex) , dXMove , dYMove);
				ScaleEntity(aname(m_vector_Layer_Oder.at(i).idArr.at(nIndex)), ptNKMid , m_dScale);
			}
			m_objArrSave.append(m_vector_Layer_Oder.at(i).idArr);
			ReNoteHouse(m_vector_Layer_Oder.at(i).idArr);
			ads_point ptMinFloor , ptMaxFloor;
			GetEntsExtent(m_vector_Layer_Oder.at(i).idArr , ptMinFloor , ptMaxFloor);
			insertNodeFloor(m_vector_Layer_Oder.at(i).idArr , ptMinFloor , ptMaxFloor , m_vector_Layer_Oder.at(i).nLayerCount , m_vector_Layer_Oder.at(i).veStrFloorName,m_strHH);
			idArrText.setLogicalLength(0);
			NoteJZXDist_Final(m_vector_Layer_Oder.at(i).idArr , idArrText);
			m_objArrSave.append(idArrText);
			idArrTextAll.append(idArrText);
		}
		m_strJZMJ.Format(_T("%.2f"),JZMJ);
		m_blockB.replaceText(_T("#JZMJ#") , m_strJZMJ);
		if ((zyjzmj_area+ftjzmj_area)!=JZMJ)
		{
			zyjzmj_area = JZMJ -ftjzmj_area;
		}
		CString ZYJZMJ;
		ZYJZMJ.Format(_T("%.2f"),zyjzmj_area);
		m_blockB.replaceText(_T("#ZYJZMJ#") , ZYJZMJ);
		m_objArrSave.append(m_blockB.m_objIdArr);
		ads_name ssAll , ssSave;
		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		IdArrToSS(m_objArrSave , ssSave);
		SubssName(ssAll , ssSave);
		EntsDel(ssAll);
		acedSSFree(ssSave);
		acedSSFree(ssAll);

		CXRecordOperate xRecord;
		double dAngle = 0.0;
		xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
		dAngle =-dAngle;
		AcDbObjectId idNorth = DrawInsert(apoint(m_ptInsertNorth) , _T("north1"));

		double dTemp = (PI / 2 - dAngle) / PI;
        if (dTemp < 0)
        {
			while (dTemp < 0)
			{
				dTemp += 1.0;
			}
        }
		else if (dTemp > 1)
		{
			while (dTemp - 1.0 > 0)
			{
				dTemp -= 1.0;
			}
		}
		RotateEntity(aname(idNorth), dTemp*PI, m_ptInsertNorth);

// 		if (dTemp > -0.5 && dTemp < -0.01)
// 		{
// 			RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);
// 		}
// 		else if (dTemp < -0.5 && dTemp > -1)
// 		{
// 			RotateEntity(aname(idNorth) , PI/2 - dAngle + PI , m_ptInsertNorth);
// 		}
// 		else if (dTemp > 0.5)
// 		{
// 			RotateEntity(aname(idNorth) , PI/2 - dAngle + PI , m_ptInsertNorth);
// 		}
// 		else
// 		{
// 			RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);
// 		}
// 		if (PI/2 - dAngle - PI > 0.1)
// 		{
// 			RotateEntity(aname(idNorth) , PI/2 - dAngle - PI , m_ptInsertNorth);
// 		}
// 		else if (PI/2 - dAngle < 0.1 && PI/2 - dAngle > -PI)
// 		{
// 			RotateEntity(aname(idNorth) , PI/2 - dAngle + PI , m_ptInsertNorth);
// 		}
// 		else
// 		{
// 			RotateEntity(aname(idNorth) , PI/2 - dAngle , m_ptInsertNorth);
// 		}

		ScaleEntity(aname(idNorth) , apoint(m_ptInsertNorth) , 1);
		m_objArrSave.append(idNorth);
		AcDbObjectIdArray objArrRoomLine;
		acedSSGet(_T("A"), NULL, NULL, NULL, ssAll);
		m_objArrSave.setLogicalLength(0);
		SSToIdArr(ssAll , m_objArrSave);
		for (int i = 0;i < m_objArrSave.length();i++)
		{
			TCHAR szSTDM[100] = {0};
			GetStdm(aname(m_objArrSave.at(i)) , szSTDM);
			if (0 == _tcscmp(szSTDM , _T("room_line")))
			{
				setlinetype(aname(m_objArrSave.at(i)) , _T("DASHED"));
			}
		}
		ZoomWindow(apoint(m_pt3dWLBTK) ,  apoint(m_pt3dWRTTK));

		CSaveBlocks saveB;
		saveB.setPoint(apoint(m_pt3dWLBTK));
		CString	 strTempSavePath;
		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
		{
			strTempSavePath.Format(_T("%s%sF%s%s"), m_strPath , m_strZDDM , m_strZH , m_strHH);
		}
		else
		{
			strTempSavePath.Format(_T("%s\\%sF%s%s"), m_strPath , m_strZDDM , m_strZH , m_strHH);
		}
		m_strTempSavePath.Format(_T("%s") , strTempSavePath);
		acedCommand( RTSTR , _T("zoom") , RTSTR , _T("e") , 0 );
		saveB.setPath(strTempSavePath);

		ScaleEntitys(m_objArrSave , m_pt3dWLBTK , 1/m_dScale);
		saveB.saveBlocks2(m_objArrSave);
		actrTransactionManager->abortTransaction();
	}

// 	resbuf *cmd = NULL , *result = NULL;
// 	cmd = ads_buildlist(RTSTR , _T("InvokeUpLoadImage") , RTSTR , m_strPath  ,RTNONE);
// 	ads_invoke(cmd,&result);
// 	ads_relrb(cmd);
// 	ads_relrb(result);

	return TRUE;
}
BOOL CExportHouseDiagram::CreateDirectoryForbidOverWrite(CString str_path)
{
	str_path.Replace(_T("\\\\"), _T("\\"));
	if (IsPathFileExists(str_path))
	{
		return FALSE;
	}
	else
	{
		return CreateDirectory(str_path, NULL);
	}
}
void CExportHouseDiagram::insertNodeFloor_zhijiang(AcDbObjectIdArray idArr , ads_point ptNKMin , ads_point ptNKMax , int nCountFloor ,  vector<CString> veStrLayerName)
{
	map<double , AcDbObjectId> map_Area_IdArr;
	for (int i = 0;i < idArr.length();i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)) , dArea);
		//dArea /= (m_dScale * m_dScale);
		map_Area_IdArr.insert(std::pair<double , AcDbObjectId>(dArea , idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder_AS , idArrOder;
	map<double , AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veDArea_As , veDArea;
	for (;it != map_Area_IdArr.end();it++)
	{
		idArrOder_AS.append(it->second);
		veDArea_As.push_back(it->first);
	}
	int nIndex = 0;

	for (int i = (idArrOder_AS.length() - 1);i >= 0;i--)
	{
		idArrOder.append(idArrOder_AS.at(i));
		veDArea.push_back(veDArea_As[i]);
	}
	map<CString , int> mapHouseoder;
	for (int i = 0;i < idArrOder.length() ;i++)
	{
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)) , nodes);
		GetNodesMiddlePoint(nodes , ptMid);
		TCHAR szValue[1000] = {0};
		if (m_nExportType == 1)
		{
			_tcscpy(szValue , m_strJG);
		}
		else
		{
			ReadXdata(aname(idArrOder.at(i)) , _T("room_name") , STRING_TYPE , szValue);
		}
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s") , szValue);
		map<CString , int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString , int>(strValue , 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas , strValue , _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s%02d"), saDatas[1] , nIndex );
		}
		else
		{
			strText.Format(_T("%s%02d") , strValue , nIndex);
		}
		veStrName.push_back(strText);
	}
	ads_point pt1;
	ads_polar(ptNKMin , -1*PI/2 , m_TextHight1*m_dScale*m_dH*2 , pt1);
	pt1[X] += (ptNKMax[X] - ptNKMin[X])/2;
	//ads_polar(pt1 , PI , (ptNKMax[X] - ptNKMin[X])/2 , pt1);
	if (idArrOder.length() > 0)
	{
		TCHAR szlayer[1000] = {0};
		GetEntLayer(idArrOder[0] , szlayer);
		CString strTemp;
		for (int i = 0;i < veStrLayerName.size();i++)
		{
			if (veStrLayerName[i].Find(_T(".1")) != -1)
			{
				for (int nFSIndex = 0;nFSIndex < m_veFSName.size();nFSIndex++)
				{
					CString strTempFS = m_veFSName[nFSIndex];
					vector<CString> saDatasFS;
					AnalyzeData(saDatasFS , strTempFS , _T("_"));
					if (saDatasFS.size() >= 2)
					{
						if (veStrLayerName[i].Find(saDatasFS[0]) != -1)
						{
							strTempFS = strTempFS.Right(strTempFS.GetLength() - strTempFS.Find(_T("_")) - 1);
							strTemp += strTempFS;						
						}

					}
				}
			}
			else
			{
				strTemp +=_T("第") + veStrLayerName[i];
			}

			if (i != (veStrLayerName.size() - 1))
			{
				strTemp += _T("、");
			}
		}
		//strTemp.Format(_T("第%s平面图") , szlayer);
		strTemp += _T("层平面图");
		AcDbObjectId id = DrawText(pt1 , strTemp , _T("mc") , _T("") , m_TextHight1 * m_dH * m_dScale);
		SetTextStyle(id , m_strFont1);
		m_objArrSave.append(id);
	}
	CString strText;
	double dArea_total = 0.0;
	for(int i = 0;i < veStrName.size();i++)
	{
		CString strTemp;
		strTemp.Format(_T("%s=%.2f") , veStrName[i] , veDArea[i]*nCountFloor);
		strText += strTemp;
		if (i != (veStrName.size() - 1))
		{
			strText += _T(",");
		}
		CString temp_area;
		temp_area.Format(_T("%.2f"),veDArea[i]*nCountFloor);
		double area_temp = _tstof(temp_area);
		dArea_total += area_temp;
	}
	ads_polar(pt1 , -1*PI/2 , m_TextHight1*m_dScale*m_dH*1.5 , pt1);
	strText.Format(_T("总面积为：%.2f") , dArea_total);
	AcDbObjectId id = DrawText(pt1 , strText , _T("mc") , _T("") , m_TextHight1 * m_dScale * m_dH);
	SetTextStyle(id , m_strFont1.GetBuffer());
	m_strFont1.ReleaseBuffer();
	m_objArrSave.append(id);
}

void CExportHouseDiagram::insertNodeFloor(AcDbObjectIdArray idArr , ads_point ptNKMin , ads_point ptNKMax , int nCountFloor ,  vector<CString> veStrLayerName,CString HH)
{
	map<double , AcDbObjectId> map_Area_IdArr;
	for (int i = 0;i < idArr.length();i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)) , dArea);
		TCHAR szScale[1000] = { 0 };
		ReadXdata(aname(idArr.at(i)), _T("area_scale"), DOUBLE_TYPE, szScale);
		double dCale = _tstof(szScale);
		dArea /= (m_dScale * m_dScale);
		dArea *= dCale;
		map_Area_IdArr.insert(std::pair<double , AcDbObjectId>(dArea , idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder_AS , idArrOder;
	map<double , AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veStrFenTan;
	vector<double> veDArea_As , veDArea;
	
	for (;it != map_Area_IdArr.end();it++)
	{
		idArrOder_AS.append(it->second);
		veDArea_As.push_back(it->first);
	}
	int nIndex = 0;

	for (int i = (idArrOder_AS.length() - 1);i >= 0;i--)
	{
		idArrOder.append(idArrOder_AS.at(i));
		veDArea.push_back(veDArea_As[i]);
	}
	double fentanxs = 1.0;
	map<CString , int> mapHouseoder;
	for (int i = 0;i < idArrOder.length() ;i++)
	{
		fentanxs = 1.0;
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)) , nodes);
		GetNodesMiddlePoint(nodes , ptMid);
		TCHAR szValue[1000] = {0};
		TCHAR szFenTan[1000]={0};
		if (m_nExportType == 1)
		{
			_tcscpy(szValue , m_strJG);
		}
		else
		{
			ReadXdata(aname(idArrOder.at(i)) , _T("room_name") , STRING_TYPE , szValue);
			int XdataCount =GetXDataCount(aname(idArrOder.at(i)),_T("fen_tan_information_BF"));
			for (int roomnm=0;roomnm<XdataCount;roomnm++)
			{
				READMulXData(aname(idArrOder.at(i)),_T("fen_tan_information_BF"),szFenTan,STRING_TYPE,roomnm);
				CStringArray strArr;
				AnalyzeData(strArr,szFenTan,_T(":"));
				if (strArr.GetSize()>1)
				{
					if (_tcscmp(HH,strArr[0])==0)
					{
						fentanxs =_tstof(strArr[1]);
					}
				}
			}
			
		}
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s") , szValue);
		map<CString , int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString , int>(strValue , 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas , strValue , _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s%02d"), saDatas[1] , nIndex );
		}
		else
		{
			strText.Format(_T("%s%02d") , strValue , nIndex);
		}
		veStrName.push_back(strText);
		veStrFenTan.push_back(fentanxs);
	}
	ads_point pt1;
	ads_polar(ptNKMin , -1*PI/2 , m_TextHight1*m_dScale*m_dH*2 , pt1);
	pt1[X] += (ptNKMax[X] - ptNKMin[X])/2;
	//ads_polar(pt1 , PI , (ptNKMax[X] - ptNKMin[X])/2 , pt1);
	if (idArrOder.length() > 0)
	{
		TCHAR szlayer[1000] = {0};
		GetEntLayer(idArrOder[0] , szlayer);
		CString strTemp;
		for (int i = 0;i < veStrLayerName.size();i++)
		{
			if (veStrLayerName[i].Find(_T(".1")) != -1)
			{
				for (int nFSIndex = 0;nFSIndex < m_veFSName.size();nFSIndex++)
				{
					CString strTempFS = m_veFSName[nFSIndex];
					vector<CString> saDatasFS;
					AnalyzeData(saDatasFS , strTempFS , _T("_"));
					if (saDatasFS.size() >= 2)
					{
						if (veStrLayerName[i].Find(saDatasFS[0]) != -1)
						{
							strTempFS = strTempFS.Right(strTempFS.GetLength() - strTempFS.Find(_T("_")) - 1);
							strTemp += strTempFS;						
						}

					}
				}
			}
			else
			{
				strTemp +=_T("第") + veStrLayerName[i];
			}
			
			if (i != (veStrLayerName.size() - 1))
			{
				strTemp += _T("、");
			}
		}
		//strTemp.Format(_T("第%s平面图") , szlayer);
		strTemp += _T("平面图");
		AcDbObjectId id = DrawText(pt1 , strTemp , _T("mc") , _T("") , m_TextHight1 * m_dH * m_dScale);
		SetTextStyle(id , m_strFont1);
		m_objArrSave.append(id);
	}
	CString strText;
	double dArea_total = 0.0;
	for(int i = 0;i < veStrName.size();i++)
	{
		CString strTemp;
		strTemp.Format(_T("%s=%.2f") , veStrName[i] , veDArea[i]*nCountFloor*veStrFenTan[i]);
		strText += strTemp;
		if (i != (veStrName.size() - 1))
		{
			strText += _T(",");
		}
		CString temp_area;
		temp_area.Format(_T("%.2f"),veDArea[i]*nCountFloor*veStrFenTan[i]);
		double area_temp = _tstof(temp_area);
		dArea_total += area_temp;
	}
	JZMJ +=dArea_total;
	ads_polar(pt1 , -1*PI/2 , m_TextHight1*m_dScale*m_dH*1.5 , pt1);
	strText.Format(_T("总面积为：%.2f") , dArea_total);
	AcDbObjectId id = DrawText(pt1 , strText , _T("mc") , _T("") , m_TextHight1 * m_dScale * m_dH);
	SetTextStyle(id , m_strFont1.GetBuffer());
	m_strFont1.ReleaseBuffer();
	m_objArrSave.append(id);
}
void CExportHouseDiagram::insertNodeFloor_Simple(AcDbObjectIdArray idArr, ads_point ptNKMin, ads_point ptNKMax, int nCountFloor, vector<CString> veStrLayerName,int nFloor)
{
	map<double, AcDbObjectId> map_Area_IdArr;
	for (int i = 0; i < idArr.length(); i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)), dArea);
		map_Area_IdArr.insert(std::pair<double, AcDbObjectId>(dArea, idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder_AS, idArrOder;
	map<double, AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veDArea_As,veDArea;
	vector<vector<double> > veDArea_xishu;   //面积系数，进区分半算和全算
	for (; it != map_Area_IdArr.end(); it++)
	{
		idArrOder_AS.append(it->second);
		veDArea_As.push_back(it->first);
	}
	int nIndex = 0;
	for (int ii=0;ii<veStrLayerName.size();ii++)
	{
		vector<double> veDArea_xishu_one;
		int int_temp=_ttoi(veStrLayerName[ii]);
		for (int i = (idArrOder_AS.length() - 1); i >= 0; i--)
		{
			if (ii==0)
			{
				idArrOder.append(idArrOder_AS.at(i));
				veDArea.push_back(veDArea_As[i]);
			}
			TCHAR szSTDM[100] = {0};
			TCHAR szTable[100]={0};
			TCHAR szCode[100] = {0};
			AcDbObjectId idText;
			GetStdm(aname(idArrOder_AS.at(i)) , szSTDM);
			CString strtable;
			strtable.Format(_T("%s"), szSTDM);
			g_index.get_table(szSTDM,szTable);
			_stprintf(szCode,_T("%s"),strtable.Left(3));

			//附着物面积半算
			bool ishalf =false;
			int xdata_num = GetXDataCount(aname(idArrOder_AS.at(i)),_T("Half_Area"));
			for (int j=0;j<xdata_num;j++)
			{
				TCHAR half_floor[50]={0};
				READMulXData(idArrOder_AS.at(i),_T("Half_Area"),half_floor,STRING_TYPE,j);
				int temp_floor =_ttoi(half_floor);
				if (int_temp==temp_floor)
				{
					veDArea_xishu_one.push_back(0.5);
					break;
				}
				else
				{
					if (j==xdata_num-1)
					{
						veDArea_xishu_one.push_back(1);
					}
				}
			}
			if (xdata_num==-1)
			{
				veDArea_xishu_one.push_back(1);
			}
		}
		veDArea_xishu.push_back(veDArea_xishu_one);
	}


	map<CString, int> mapHouseoder;
	for (int i = 0; i < idArrOder.length(); i++)
	{
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)), nodes);
		GetNodesMiddlePoint(nodes, ptMid);
		TCHAR szValue[1000] = { 0 };
		if (m_nExportType == 1)
		{
			_tcscpy(szValue, m_strJG);
		}
		else
		{
			ReadXdata(aname(idArrOder.at(i)), _T("room_name"), STRING_TYPE, szValue);
		}
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s"), szValue);
		map<CString, int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString, int>(strValue, 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas, strValue, _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s%02d"), saDatas[1], nIndex);
		}
		else
		{
			strText.Format(_T("%s%02d"), strValue, nIndex);
		}
		veStrName.push_back(strText);
	}
	ads_point pt1;
	ads_polar(ptNKMin, -1 * PI / 2, m_TextHight1*m_dScale*m_dH * 3, pt1);
	pt1[X] += (ptNKMax[X] - ptNKMin[X]) / 2;
	//ads_polar(pt1 , PI , (ptNKMax[X] - ptNKMin[X])/2 , pt1);
	if (idArrOder.length() > 0)
	{
		TCHAR szlayer[1000] = { 0 };
		GetEntLayer(idArrOder[0], szlayer);
		CString strTemp;
		for (int i = 0; i < veStrLayerName.size(); i++)
		{
			if (veStrLayerName[i].Find(_T(".1")) != -1)
			{
				for (int nFSIndex = 0; nFSIndex < m_veFSName.size(); nFSIndex++)
				{
					CString strTempFS = m_veFSName[nFSIndex];
					vector<CString> saDatasFS;
					AnalyzeData(saDatasFS, strTempFS, _T("_"));
					if (saDatasFS.size() >= 2)
					{
						if (veStrLayerName[i].Find(saDatasFS[0]) != -1)
						{
							strTempFS = strTempFS.Right(strTempFS.GetLength() - strTempFS.Find(_T("_")) - 1);
							strTemp += strTempFS;
						}

					}
				}
			}
			else
			{
				strTemp += _T("第") + veStrLayerName[i];
			}

			if (i != (veStrLayerName.size() - 1))
			{
				strTemp += _T("、");
			}
		}
		//strTemp.Format(_T("第%s平面图") , szlayer);
		strTemp += _T("层平面图");
		AcDbObjectId id = DrawText(pt1, strTemp, _T("mc"), _T(""), m_TextHight1 * m_dH * m_dScale);
		SetTextStyle(id, m_strFont1);
		m_objArrSave.append(id);
	}
	CString strText;
	double dArea_total = 0.0;
	for (int num=0;num<veDArea_xishu.size();num++)
	{
		for (int i = 0; i < veStrName.size(); i++)
		{
			CString strTemp;
			strTemp.Format(_T("%s=%.2f"), veStrName[i], veDArea[i] * veDArea_xishu[num][i]);
			strText += strTemp;
			if (i != (veStrName.size() - 1))
			{
				strText += _T(",");
			}
			dArea_total += veDArea[i] * veDArea_xishu[num][i];
		}
	}
	
	ads_polar(pt1, -1 * PI / 2, m_TextHight1*m_dScale*m_dH*1.5, pt1);
	strText.Format(_T("总面积为：%.2f"), dArea_total);
	JZMJ +=dArea_total;
	AcDbObjectId id = DrawText(pt1, strText, _T("mc"), _T(""), m_TextHight1 * m_dScale * m_dH);
	SetTextStyle(id, m_strFont1.GetBuffer());
	m_strFont1.ReleaseBuffer();
	m_objArrSave.append(id);
}

void CExportHouseDiagram::insertNodeFloor_zrzfloor_sketch(AcDbObjectIdArray idArr , ads_point ptNKMin , ads_point ptNKMax , int nCountFloor ,  vector<CString> veStrLayerName,CString HH)
{
	map<double , AcDbObjectId> map_Area_IdArr;
	for (int i = 0;i < idArr.length();i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)) , dArea);
		TCHAR szScale[1000] = { 0 };
		ReadXdata(aname(idArr.at(i)), _T("area_scale"), DOUBLE_TYPE, szScale);
		double dCale = _tstof(szScale);
		dArea /= (m_dScale * m_dScale);
		dArea *= dCale;
		map_Area_IdArr.insert(std::pair<double , AcDbObjectId>(dArea , idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder_AS , idArrOder;
	map<double , AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veStrFenTan;
	vector<double> veDArea_As , veDArea;

	for (;it != map_Area_IdArr.end();it++)
	{
		idArrOder_AS.append(it->second);
		veDArea_As.push_back(it->first);
	}
	int nIndex = 0;

	for (int i = (idArrOder_AS.length() - 1);i >= 0;i--)
	{
		idArrOder.append(idArrOder_AS.at(i));
		veDArea.push_back(veDArea_As[i]);
	}
	double fentanxs = 1.0;
	map<CString , int> mapHouseoder;
	for (int i = 0;i < idArrOder.length() ;i++)
	{
		fentanxs = 1.0;
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)) , nodes);
		GetNodesMiddlePoint(nodes , ptMid);
		TCHAR szValue[1000] = {0};
		TCHAR szFenTan[1000]={0};
		if (m_nExportType == 1)
		{
			_tcscpy(szValue , m_strJG);
		}
		else
		{
			ReadXdata(aname(idArrOder.at(i)) , _T("room_name") , STRING_TYPE , szValue);
			int XdataCount =GetXDataCount(aname(idArrOder.at(i)),_T("fen_tan_information_BF"));
			for (int roomnm=0;roomnm<XdataCount;roomnm++)
			{
				READMulXData(aname(idArrOder.at(i)),_T("fen_tan_information_BF"),szFenTan,STRING_TYPE,roomnm);
				CStringArray strArr;
				AnalyzeData(strArr,szFenTan,_T(":"));
				if (strArr.GetSize()>1)
				{
					if (_tcscmp(HH,strArr[0])==0)
					{
						fentanxs =_tstof(strArr[1]);
					}
				}
			}

		}
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s") , szValue);
		map<CString , int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString , int>(strValue , 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas , strValue , _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s%02d"), saDatas[1] , nIndex );
		}
		else
		{
			strText.Format(_T("%s%02d") , strValue , nIndex);
		}
		veStrName.push_back(strText);
		veStrFenTan.push_back(fentanxs);
	}
	ads_point pt1;
	ads_polar(ptNKMin , -1*PI/2 , m_TextHight1*m_dScale*m_dH*2 , pt1);
	pt1[X] += (ptNKMax[X] - ptNKMin[X])/2;
	//ads_polar(pt1 , PI , (ptNKMax[X] - ptNKMin[X])/2 , pt1);
	if (idArrOder.length() > 0)
	{
		TCHAR szlayer[1000] = {0};
		GetEntLayer(idArrOder[0] , szlayer);
		CString strTemp;
		for (int i = 0;i < veStrLayerName.size();i++)
		{
			if (veStrLayerName[i].Find(_T(".1")) != -1)
			{
				for (int nFSIndex = 0;nFSIndex < m_veFSName.size();nFSIndex++)
				{
					CString strTempFS = m_veFSName[nFSIndex];
					vector<CString> saDatasFS;
					AnalyzeData(saDatasFS , strTempFS , _T("_"));
					if (saDatasFS.size() >= 2)
					{
						if (veStrLayerName[i].Find(saDatasFS[0]) != -1)
						{
							strTempFS = strTempFS.Right(strTempFS.GetLength() - strTempFS.Find(_T("_")) - 1);
							strTemp += strTempFS;						
						}

					}
				}
			}
// 			else
// 			{
// 				strTemp1 = _T("第") + veStrLayerName[0];
// 			}

			if (i == (veStrLayerName.size() - 1))
			{
				if(i==0)
				{
					strTemp +=HH+ _T("幢第")+veStrLayerName[i];
				}
				else
				{
					strTemp +=HH+ _T("幢第")+veStrLayerName[0]+_T("-")+veStrLayerName[i];
				}
				
			}
		}
		//strTemp.Format(_T("第%s平面图") , szlayer);
		
		strTemp += _T("层分层草图");
		AcDbObjectId id = DrawText(pt1 , strTemp , _T("mc") , _T("") , m_TextHight1 * m_dH * m_dScale);
		SetTextStyle(id , m_strFont1);
		m_objArrSave.append(id);
	}
// 	CString strText;
// 	double dArea_total = 0.0;
// 	for(int i = 0;i < veStrName.size();i++)
// 	{
// 		CString strTemp;
// 		strTemp.Format(_T("%s=%.2f") , veStrName[i] , veDArea[i]*nCountFloor*veStrFenTan[i]);
// 		strText += strTemp;
// 		if (i != (veStrName.size() - 1))
// 		{
// 			strText += _T(",");
// 		}
// 		CString temp_area;
// 		temp_area.Format(_T("%.2f"),veDArea[i]*nCountFloor*veStrFenTan[i]);
// 		double area_temp = _tstof(temp_area);
// 		dArea_total += area_temp;
// 	}
// 	JZMJ +=dArea_total;
// 	ads_polar(pt1 , -1*PI/2 , m_TextHight1*m_dScale*m_dH*1.5 , pt1);
// 	strText.Format(_T("总面积为：%.2f") , dArea_total);
// 	AcDbObjectId id = DrawText(pt1 , strText , _T("mc") , _T("") , m_TextHight1 * m_dScale * m_dH);
// 	SetTextStyle(id , m_strFont1.GetBuffer());
// 	m_strFont1.ReleaseBuffer();
// 	m_objArrSave.append(id);
}

double CExportHouseDiagram::insertNodeFloor_Simple_ShiShou(AcDbObjectIdArray idArr, ads_point ptNKMin, ads_point ptNKMax, int nCountFloor,int nFloor, vector<CString> veStrLayerName,double &Area_now)
{
	map<double, AcDbObjectId> map_Area_IdArr;
	for (int i = 0; i < idArr.length(); i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)), dArea);
		//dArea /= (m_dScale * m_dScale);
		map_Area_IdArr.insert(std::pair<double, AcDbObjectId>(dArea, idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder_AS, idArrOder;
	map<double, AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veDArea_As, veDArea;
	for (; it != map_Area_IdArr.end(); it++)
	{
		idArrOder_AS.append(it->second);
		veDArea_As.push_back(it->first);
	}
	int nIndex = 0;

	for (int i = (idArrOder_AS.length() - 1); i >= 0; i--)
	{
		idArrOder.append(idArrOder_AS.at(i));
		TCHAR szSTDM[100] = {0};
		TCHAR szTable[100]={0};
		TCHAR szCode[100] = {0};
		AcDbObjectId idText;
		GetStdm(aname(idArrOder_AS.at(i)) , szSTDM);
		CString strtable;
		strtable.Format(_T("%s"), szSTDM);
		g_index.get_table(szSTDM,szTable);
		_stprintf(szCode,_T("%s"),strtable.Left(3));
		//附着物面积半算
		if (_tcscmp(szCode,_T("141")))
		{
			bool ishalf =false;
			int xdata_num = GetXDataCount(aname(idArrOder_AS.at(i)),_T("Half_Area"));
			for (int j=0;j<xdata_num;j++)
			{
				TCHAR half_floor[50]={0};
				READMulXData(idArrOder_AS.at(i),_T("Half_Area"),half_floor,STRING_TYPE,j);
				int temp_floor =_ttoi(half_floor);
				if (temp_floor==nFloor)
				{
					ishalf =true;
					double area_temp = veDArea_As[i]/2;
					veDArea.push_back(area_temp);
				}
			}
			if (!ishalf)
			{
				veDArea.push_back(veDArea_As[i]);
			}
		}
		else
		{
			veDArea.push_back(veDArea_As[i]);
		}
		
	}
	map<CString, int> mapHouseoder;
	for (int i = 0; i < idArrOder.length(); i++)
	{
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)), nodes);
		GetNodesMiddlePoint(nodes, ptMid);
		TCHAR szValue[1000] = { 0 };
		if (m_nExportType == 1)
		{
			_tcscpy(szValue, m_strJG);
		}
		else
		{
			ReadXdata(aname(idArrOder.at(i)), _T("room_name"), STRING_TYPE, szValue);
		}
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s"), szValue);
		map<CString, int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString, int>(strValue, 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas, strValue, _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s%02d"), saDatas[1], nIndex);
		}
		else
		{
			strText.Format(_T("%s%02d"), strValue, nIndex);
		}
		veStrName.push_back(strText);
	}
	ads_point pt1;
	ads_polar(ptNKMin, -1 * PI / 2, m_TextHight1*m_dScale*m_dH * 2, pt1);
	pt1[X] += (ptNKMax[X] - ptNKMin[X]) / 2;
	//ads_polar(pt1 , PI , (ptNKMax[X] - ptNKMin[X])/2 , pt1);
// 	if (idArrOder.length() > 0)
// 	{
// 		TCHAR szlayer[1000] = { 0 };
// 		GetEntLayer(idArrOder[0], szlayer);
// 		CString strTemp;
// 		for (int i = 0; i < veStrLayerName.size(); i++)
// 		{
// 			if (veStrLayerName[i].Find(_T(".1")) != -1)
// 			{
// 				for (int nFSIndex = 0; nFSIndex < m_veFSName.size(); nFSIndex++)
// 				{
// 					CString strTempFS = m_veFSName[nFSIndex];
// 					vector<CString> saDatasFS;
// 					AnalyzeData(saDatasFS, strTempFS, _T("_"));
// 					if (saDatasFS.size() >= 2)
// 					{
// 						if (veStrLayerName[i].Find(saDatasFS[0]) != -1)
// 						{
// 							strTempFS = strTempFS.Right(strTempFS.GetLength() - strTempFS.Find(_T("_")) - 1);
// 							strTemp += strTempFS;
// 						}
// 
// 					}
// 				}
// 			}
// 			else
// 			{
// 				strTemp += _T("第") + veStrLayerName[i];
// 			}
// 
// 			if (i != (veStrLayerName.size() - 1))
// 			{
// 				strTemp += _T("、");
// 			}
// 		}
// 		//strTemp.Format(_T("第%s平面图") , szlayer);
// 		strTemp += _T("层平面图");
// 		AcDbObjectId id = DrawText(pt1, strTemp, _T("mc"), _T(""), m_TextHight1 * m_dH * m_dScale);
// 		SetTextStyle(id, m_strFont1);
// 		m_objArrSave.append(id);
// 	}
	CString strText;
	double dArea_total = 0.0;
	for (int i = 0; i < veStrName.size(); i++)
	{
		CString strTemp;
		strTemp.Format(_T("%s=%.2f"), veStrName[i], veDArea[i] * nCountFloor);
		strText += strTemp;
		if (i != (veStrName.size() - 1))
		{
			strText += _T(",");
		}
		dArea_total += veDArea[i] * nCountFloor;
	}
	Area_now =dArea_total;
	return Area_now;
//	ads_polar(pt1, -1 * PI / 2, m_TextHight1*m_dScale*m_dH*1.5, pt1);
//	strText.Format(_T("总面积为：%.2f"), dArea_total);
//	AcDbObjectId id = DrawText(pt1, strText, _T("mc"), _T(""), m_TextHight1 * m_dScale * m_dH);
//	SetTextStyle(id, m_strFont1.GetBuffer());
//	m_strFont1.ReleaseBuffer();
//	m_objArrSave.append(id);
}

void CExportHouseDiagram::insertNodeFloor_New(AcDbObjectIdArray idArr , ads_point ptNKMin , ads_point ptNKMax , int nCountFloor ,  vector<CString> veStrLayerName)
{
	map<double , AcDbObjectId> map_Area_IdArr;
	for (int i = 0;i < idArr.length();i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)) , dArea);
		map_Area_IdArr.insert(std::pair<double , AcDbObjectId>(dArea , idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder_AS , idArrOder;
	map<double , AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veDArea_As , veDArea;
	for (;it != map_Area_IdArr.end();it++)
	{
		idArrOder_AS.append(it->second);
		veDArea_As.push_back(it->first);
	}
	int nIndex = 0;

	for (int i = (idArrOder_AS.length() - 1);i >= 0;i--)
	{
		idArrOder.append(idArrOder_AS.at(i));
		veDArea.push_back(veDArea_As[i]);
	}
	map<CString , int> mapHouseoder;
	for (int i = 0;i < idArrOder.length() ;i++)
	{
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)) , nodes);
		GetNodesMiddlePoint(nodes , ptMid);
		TCHAR szValue[1000] = {0};
		if (m_nExportType == 1)
		{
			_tcscpy(szValue , m_strJG);
		}
		else
		{
			ReadXdata(aname(idArrOder.at(i)) , _T("room_name") , STRING_TYPE , szValue);
		}
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s") , szValue);
		map<CString , int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString , int>(strValue , 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas , strValue , _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s%02d"), saDatas[1] , nIndex );
		}
		else
		{
			strText.Format(_T("%s%02d") , strValue , nIndex);
		}
		veStrName.push_back(strText);
	}
	ads_point pt1;
	ads_polar(ptNKMin , -1*PI/2 , m_TextHight1*m_dScale*m_dH*2 , pt1);
	pt1[X] += (ptNKMax[X] - ptNKMin[X])/2;
	//ads_polar(pt1 , PI , (ptNKMax[X] - ptNKMin[X])/2 , pt1);
	if (idArrOder.length() > 0)
	{
		TCHAR szlayer[1000] = {0};
		GetEntLayer(idArrOder[0] , szlayer);
		CString strTemp;
		for (int i = 0;i < veStrLayerName.size();i++)
		{
			if (veStrLayerName[i].Find(_T(".1")) != -1)
			{
				for (int nFSIndex = 0;nFSIndex < m_veFSName.size();nFSIndex++)
				{
					CString strTempFS = m_veFSName[nFSIndex];
					vector<CString> saDatasFS;
					AnalyzeData(saDatasFS , strTempFS , _T("_"));
					if (saDatasFS.size() >= 2)
					{
						if (veStrLayerName[i].Find(saDatasFS[0]) != -1)
						{
							strTempFS = strTempFS.Right(strTempFS.GetLength() - strTempFS.Find(_T("_")) - 1);
							strTemp += strTempFS;						
						}

					}
				}
			}
			else
			{
				strTemp +=_T("第") + veStrLayerName[i];
			}

			if (i != (veStrLayerName.size() - 1))
			{
				strTemp += _T("、");
			}
		}
		//strTemp.Format(_T("第%s平面图") , szlayer);
		strTemp += _T("平面图");
		AcDbObjectId id = DrawText(pt1 , strTemp , _T("mc") , _T("") , m_TextHight1 * m_dH * m_dScale);
		SetTextStyle(id , m_strFont1);
		m_objArrSave.append(id);
	}
	CString strText;
	double dArea_total = 0.0;
	for(int i = 0;i < veStrName.size();i++)
	{
		CString strTemp;
		strTemp.Format(_T("%s=%.2f") , veStrName[i] , veDArea[i]*nCountFloor);
		strText += strTemp;
		if (i != (veStrName.size() - 1))
		{
			strText += _T(",");
		}
		dArea_total += veDArea[i]*nCountFloor;
	}
	ads_polar(pt1 , -1*PI/2 , m_TextHight1*m_dScale*m_dH*1.5 , pt1);
	strText.Format(_T("总面积为：%.2f") , dArea_total);
	AcDbObjectId id = DrawText(pt1 , strText , _T("mc") , _T("") , m_TextHight1 * m_dScale * m_dH);
	SetTextStyle(id , m_strFont1.GetBuffer());
	m_strFont1.ReleaseBuffer();
	m_objArrSave.append(id);
}

void CExportHouseDiagram::ReNoteHouse(AcDbObjectIdArray idArr)
{
	map<double , AcDbObjectId> map_Area_IdArr;
	for (int i = 0;i < idArr.length();i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)) , dArea);

		map_Area_IdArr.insert(std::pair<double , AcDbObjectId>(dArea , idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder;
	map<double , AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veDArea;
	for (;it != map_Area_IdArr.end();it++)
	{
		idArrOder.append(it->second);
		veDArea.push_back(it->first);
	}
	int nIndex = 0;
	map<CString , int> mapHouseoder;
	for (int i = (idArrOder.length() - 1);i >= 0 ;i--)
	{
		nIndex ++;
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)) , nodes);
		GetNodesMiddlePoint(nodes , ptMid);
		TCHAR szValue[1000] = {0};
		ReadXdata(aname(idArrOder.at(i)) , _T("room_name") , STRING_TYPE , szValue);
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s") , szValue);
		map<CString , int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString , int>(strValue , 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas , strValue , _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s"), saDatas[1] );
		}
		else
		{
			strText.Format(_T("%s") , strValue );
		}
		veStrName.push_back(strText);

		AcDbObjectId id = DrawText(ptMid , strText , _T("mc") , _T("") , m_TextHight1 * m_dScale * m_dH);
		SetTextStyle(id , m_strFont1.GetBuffer());
		m_strFont1.ReleaseBuffer();
		m_objArrSave.append(id);
	}
}
void CExportHouseDiagram::ReNoteHouse_zhijiang(AcDbObjectIdArray idArr,double _scale,int nIndexfloor,int logic_start)
{
	map<double , AcDbObjectId> map_Area_IdArr;
	for (int i = 0;i < idArr.length();i++)
	{
		double dArea = 0.0;
		cass::GetEntArea(aname(idArr.at(i)) , dArea);

		map_Area_IdArr.insert(std::pair<double , AcDbObjectId>(dArea , idArr.at(i)));
	}
	AcDbObjectIdArray idArrOder;
	map<double , AcDbObjectId>::iterator it = map_Area_IdArr.begin();
	vector<CString> veStrName;
	vector<double> veDArea;
	for (;it != map_Area_IdArr.end();it++)
	{
		idArrOder.append(it->second);
		veDArea.push_back(it->first);
	}
	int nIndex = 0;
	map<CString , int> mapHouseoder;
	vector<LOGIC> vec_logic_temp;
	for (int i = (idArrOder.length() - 1);i >= 0 ;i--)
	{
		LOGIC logic_temp;
		logic_temp.logic_num=logic_start;
		logic_temp.cs.push_back(nIndexfloor+1);
		nIndex ++;
		AcGePoint2dArray nodes;
		ads_point ptMid;
		GetPlList(aname(idArrOder.at(i)) , nodes);
		logic_temp.nodes=nodes;
		GetNodesMiddlePoint(nodes , ptMid);
		TCHAR szValue[1000] = {0};
		ReadXdata(aname(idArrOder.at(i)) , _T("room_name") , STRING_TYPE , szValue);


		CString strlogic_num;
		TCHAR szSTDM[100] = {0};
		TCHAR szTable[100]={0};
		TCHAR szCode[100] = {0};
		AcDbObjectId idText;
		GetStdm(aname(idArrOder.at(i)) , szSTDM);
		CString strtable;
		vector<AcGePoint2dArray> node_log_all;
		strtable.Format(_T("%s"), szSTDM);
		g_index.get_table(szSTDM,szTable);
		_stprintf(szCode,_T("%s"),strtable.Left(3));
		if (_tcscmp(szCode,_T("141"))==0)
		{
			CString lg_area_num;
			AcGePoint2dArray lg_nodes;
			GetPlList(aname(idArrOder.at(i)) , lg_nodes);
			node_log_all.push_back(lg_nodes);
			double Area_now = GetNodesArea(lg_nodes);
			double lg_area =0.0;
			if (_scale!=0)
			{
				lg_area =Area_now/(_scale*_scale);
			}

			if (nIndexfloor!=0)
			{
				for (int i=0;i<vec_logic.size();i++)
				{
					if (vec_logic[i].cs.at(0)==1)
					{
						double Area_temp = GetNodesArea(vec_logic[i].nodes);
						if (fabs(Area_temp-Area_now)<0.000001)
						{
							logic_temp.logic_num=vec_logic[i].logic_num;
						}
					}
				}
			}
			lg_area_num.Format(_T("#zyzjmj%d#"),logic_temp.logic_num);
			
			logic_temp.logic_area.push_back(lg_area);

			TCHAR name_fwjg[100]={0};
			g_index.get_name(szSTDM,name_fwjg);
			logic_temp.fwjg.Format(_T("%s"),name_fwjg);
			
			CString strlg_area;
			strlg_area.Format(_T("%.2f"),lg_area);
			strlogic_num.Format(_T("%d") , logic_temp.logic_num);
			//逻辑幢号
			idText = DrawText(ptMid , strlogic_num , _T("MC") , _T("") , 4 * m_dH);
			m_objArrSave.append(idText);

			ads_polar(ptMid,-PI/2,m_dH,ptMid);
			//分隔下划线
			idText = DrawText(ptMid , _T("________"), _T("MC") , _T("") , 2*m_dH);
			m_objArrSave.append(idText);
			
			ads_polar(ptMid,-PI/2,m_dH*1.5,ptMid);
			//面积
			idText = DrawText(ptMid , strlg_area , _T("TC") , _T("") , 4 * m_dH);
			m_objArrSave.append(idText);
		
			
			vec_logic_temp.push_back(logic_temp);
			logic_start++;
			continue;
		}
		if (_tcscmp(szTable,_T("RESPY"))==0)
		{
			int same_point =0;
			int logic_num=0;
			AcGePoint2dArray lg_nodes;
			GetPlList(aname(idArrOder.at(i)) , lg_nodes);

			//点对比
			for (int n=0;n<node_log_all.size();n++)
			{
				int same_point_num=0;
				for (int j=0;j<node_log_all[n].length();j++)
				{
					for (int m=0;m<lg_nodes.length();m++)
					{
						if (lg_nodes.at(m)==node_log_all[n].at(j))
						{
							same_point_num++;
						}
					}
				}
				if (n==0)
				{
					same_point=same_point_num;
					logic_num=n;
				}
				else
				{
					if (same_point_num>same_point)
					{
						logic_num=n;
						same_point=same_point_num;
					}
				}
			}
		
			vector<LOGIC> vec_logic_temp1;                  //临时存储被pop出的逻辑幢
			LOGIC logic_temp_fs;                           //附属物所附属的逻辑幢
			logic_temp_fs =vec_logic_temp.at(logic_num);
			for (int n=logic_num;n<vec_logic_temp.size();n++)
			{
				if (n!=logic_num)
				{
					vec_logic_temp1.push_back(vec_logic_temp.at(n));
				}
			}
			int vec_logic_temp_num =vec_logic_temp.size();
			for (int n=logic_num;n<vec_logic_temp_num;n++)
			{
				vec_logic_temp.pop_back();
			}
		   //更改逻辑幢面积
			double lg_area= GetNodesArea(lg_nodes);
			if (_scale!=0)
			{
				lg_area =lg_area/(_scale*_scale);
			}
			//半封闭附属物判断
			TCHAR szfloor[50]={0};
			int half_floor=1;
			int nCount = GetXDataCount(aname(idArrOder.at(i)), _T("Half_Area"));
			for (int j=0;j<nCount;j++)
			{
				READMulXData(aname(idArrOder.at(i)),_T("Half_Area"),szfloor,STRING_TYPE,j);
				int temp_floor=_ttoi(szfloor);
				if (temp_floor==(nIndexfloor+1))
				{
					half_floor=2;
					break;
				}
			}
			

			double aa = logic_temp_fs.logic_area.at(0);
			aa +=(lg_area/half_floor);
			logic_temp_fs.logic_area.pop_back();
			logic_temp_fs.logic_area.push_back(aa);
			vec_logic_temp.push_back(logic_temp_fs);
			for (int j=0;j<vec_logic_temp1.size();j++)
			{
				vec_logic_temp.push_back(vec_logic_temp1.at(j));
			}
		}
		vector<CString> saDatas;
		CString strValue;
		CString strText;
		strValue.Format(_T("%s") , szValue);
		map<CString , int>::iterator it = mapHouseoder.find(strValue);
		if (it != mapHouseoder.end())
		{
			nIndex = it->second;
			nIndex++;
		}
		else
		{
			mapHouseoder.insert(std::pair<CString , int>(strValue , 1));
			nIndex = 1;
		}

		AnalyzeData(saDatas , strValue , _T(" "));
		if (saDatas.size() >= 2)
		{
			strText.Format(_T("%s"), saDatas[1] );
		}
		else
		{
			strText.Format(_T("%s") , strValue );
		}
		veStrName.push_back(strText);

		//AcDbObjectId id = DrawText(ptMid , _T("注记位置！") , _T("mc") , _T("") , m_TextHight1 * m_dScale * m_dH);
		//SetTextStyle(id , m_strFont1.GetBuffer());
		//m_strFont1.ReleaseBuffer();
		//m_objArrSave.append(id);
	}
	for (int i=0;i<vec_logic_temp.size();i++)
	{
		vec_logic.push_back(vec_logic_temp.at(i));
	}
}

void CExportHouseDiagram::Show_All()
{
	ads_name ssText;
	acedSSGet(_T("A"), NULL, NULL, NULL, ssText);
	AcDbObjectIdArray idArr;
	SSToIdArr(ssText , idArr);

	for (int i = 0;i < idArr.length();i++)
	{
		entitysOpe::setUnVisible(aname(idArr.at(i)) , true);
	}
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}

void CExportHouseDiagram::Export_Start_Simple()
{
	actrTransactionManager->startTransaction();
	m_nExportType = 1;
	AcDbObjectIdArray objArr;
	TCHAR szPath[1000] = {0};
    resbuf *cmd = ads_buildlist(RTSTR, _T("SimpleEntitysFilterDialog_invoke") ,  RTNONE);
	resbuf *list = NULL;
	int nState = ads_invoke(cmd, &list);
	ads_relrb(cmd); cmd = NULL;
	if(list == NULL) return;

	resbuf *rc = list;
	if (rc != NULL)
	{
		if(rc->restype == RTENAME)
		{
			long len = 0;
			ads_sslength(rc->resval.rlname, &len);
			if(len == 0)
			{ 
				return;
			}
			SSToIdArr(rc->resval.rlname, objArr, _T("$141010$"));
		}	
		rc = rc->rbnext;
	}

	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			_tcscpy(szPath, rc->resval.rstring);
			m_strPath.Format(_T("%s") , szPath);
// 			if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
// 			{
// 				m_strPath += _T("分层分户图\\");
// 			}
// 			else
// 			{
// 				m_strPath += _T("\\分层分户图\\");
// 			}
			CreateDirectoryForbidOverWrite(m_strPath);
		}
		rc = rc->rbnext;
	}
	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			m_strResult.Format(_T("%s") , rc->resval.rstring);
		}
	}
	AcDbObjectId idTK;
	AcDbObjectIdArray idArrTK;
	for (int i = 0;i < objArr.length();i++)
	{
		m_IdArrZRZ.append(objArr.at(i));
	}

	if (m_IdArrZRZ.length() == 0)
	{
		return;
	}
	CMatchFieldDlg dlg;
	if (!dlg.SetConfigFile(_T("Floor_MatchField.txt")))
	{
		return;
	}
	if(dlg.DoModal()!=IDOK) 
	{
		return;
	}
	m_strArrKey.Append(dlg.m_strArrKey);
	m_strArrValue.Append(dlg.m_strArrValue);
	vector<CString> saDatas_BDCH;
	AnalyzeData(saDatas_BDCH , m_strResult , _T(";"));
	for (int nIndexZRZ = 0;nIndexZRZ < m_IdArrZRZ.length();nIndexZRZ++)
	{
		ZRZinfo struct_ZRZ;
		TCHAR szZH[1000] = {0} , szZL[1000] = {0} , szZRZH[1000] = {0} , szHH[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZH") , STRING_TYPE , szZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZL") , STRING_TYPE , szZL);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZRZH") , STRING_TYPE , szZRZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_HH") , STRING_TYPE , szHH);
		struct_ZRZ.strZL = CString(szZL);
		struct_ZRZ.strZH = CString(szZH);
		struct_ZRZ.strHH = CString(szHH);
		TCHAR szZYJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_ZYJZMJ") , DOUBLE_TYPE , szZYJZMJ);
		struct_ZRZ.strZYJZMJ = CString(szZYJZMJ);
		TCHAR szFTJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_FTJZMJ") , DOUBLE_TYPE , szFTJZMJ);
		struct_ZRZ.strFTMJ = CString(szFTJZMJ);
		TCHAR szJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_JZMJ") , DOUBLE_TYPE , szJZMJ);
		struct_ZRZ.strJZMJ = CString(szJZMJ);
		TCHAR szSZC[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_SZC") , STRING_TYPE , szSZC);
		struct_ZRZ.strSZC = CString(szSZC);
		TCHAR szZCS[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZCS") , INT_TYPE , szZCS);
		struct_ZRZ.strZCS = CString(szZCS);

		TCHAR szQLR[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("QLR") , STRING_TYPE , szQLR);
		struct_ZRZ.strQLR = CString(szQLR);

		TCHAR szFWJG[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("FWJG") , STRING_TYPE , szFWJG);
		struct_ZRZ.strJG = CString(szFWJG);

		CString strFamily;
		AcDbObjectIdArray idArrQLR;

		CSimpleBuildingHelper house;
		house.SetZRZ(aname(m_IdArrZRZ.at(nIndexZRZ)));
		house.GetZrzInformation();
		std::map<int , AcDbObjectIdArray>::iterator it_Index_IdArr = house.m_floor_ents.begin();

		int nFloorIndex = 0;
		AcDbObjectIdArray idArrFloor_Pre;
		AcDbObjectIdArray idArrFloor_Cur;
		CString strFloor;
		idArrFloor_Pre.setLogicalLength(0);
		
		for (;it_Index_IdArr != house.m_floor_ents.end();it_Index_IdArr++)
		{	
			idArrFloor_Cur.setLogicalLength(0);
			idArrFloor_Cur.append(it_Index_IdArr->second);
			std::map<int , AcDbObjectIdArray>::iterator itTemp = it_Index_IdArr;
			itTemp ++;
			if (itTemp != house.m_floor_ents.end())
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					idArrFloor_Pre.append(idArrFloor_Cur);
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					continue;
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					idArrFloor_Pre.append(idArrFloor_Cur);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					strFloor += _T(",");
				}	
			}
			else
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
				}	
			}	
			nFloorIndex ++;
		}
		m_mapBDCBH_ZRZinfo.insert(pair<CString , ZRZinfo>(saDatas_BDCH[nIndexZRZ] , struct_ZRZ));
	}
	actrTransactionManager->abortTransaction();
	buildMainBlocks_Simple();
}

void CExportHouseDiagram::ExportGD_Start_Simple()
{
	actrTransactionManager->startTransaction();
	m_nExportType = 1;
	AcDbObjectIdArray objArr;
	TCHAR szPath[1000] = {0};
	resbuf *cmd = ads_buildlist(RTSTR, _T("SimpleEntitysFilterDialog_GD_invoke") ,  RTNONE);
	resbuf *list = NULL;
	int nState = ads_invoke(cmd, &list);
	ads_relrb(cmd); cmd = NULL;
	if(list == NULL) return;

	resbuf *rc = list;
	if (rc != NULL)
	{
		if(rc->restype == RTENAME)
		{
			long len = 0;
			ads_sslength(rc->resval.rlname, &len);
			if(len == 0)
			{ 
				return;
			}
			AcDbObjectIdArray objArr_temp;
			SSToIdArr(rc->resval.rlname, objArr_temp);
			for (int ii = 0; ii < objArr_temp.length(); ii++)
			{
				if (IsFWZRZ(objArr_temp.at(ii))||IsZRZ(objArr_temp.at(ii)))
				{
					objArr.append(objArr_temp.at(ii));
				}	
			}
		}	
		rc = rc->rbnext;
	}

	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			_tcscpy(szPath, rc->resval.rstring);
			m_strPath.Format(_T("%s") , szPath);
			CreateDirectoryForbidOverWrite(m_strPath);
		}
		rc = rc->rbnext;
	}
	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			m_strResult.Format(_T("%s") , rc->resval.rstring);
		}
	}
	AcDbObjectId idTK;
	AcDbObjectIdArray idArrTK;
	for (int i = 0;i < objArr.length();i++)
	{
		m_IdArrZRZ.append(objArr.at(i));
	}

	if (m_IdArrZRZ.length() == 0)
	{
		return;
	}
	CMatchFieldDlg dlg;
	if (!dlg.SetConfigFile(_T("Floor_MatchField.txt")))
	{
		return;
	}
	if(dlg.DoModal()!=IDOK) 
	{
		return;
	}
	m_strArrKey.Append(dlg.m_strArrKey);
	m_strArrValue.Append(dlg.m_strArrValue);
	vector<CString> saDatas_BDCH;
	AnalyzeData(saDatas_BDCH , m_strResult , _T(";"));
	for (int nIndexZRZ = 0;nIndexZRZ < m_IdArrZRZ.length();nIndexZRZ++)
	{
		ZRZinfo struct_ZRZ;
		TCHAR szZH[1000] = {0} , szZL[1000] = {0} , szZRZH[1000] = {0} , szHH[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZH") , STRING_TYPE , szZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("FDZL") , STRING_TYPE , szZL);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZRZH") , STRING_TYPE , szZRZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("HH") , STRING_TYPE , szHH);
		struct_ZRZ.strZL = CString(szZL);
		struct_ZRZ.strZH = CString(szZH);
		struct_ZRZ.strHH = CString(szHH);

		struct_ZRZ.strFTMJ = _T("0.00");
		TCHAR szJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("JZMJ") , DOUBLE_TYPE , szJZMJ);
		struct_ZRZ.strJZMJ = CString(szJZMJ);
		struct_ZRZ.strZYJZMJ =CString(szJZMJ);
		TCHAR szSZC[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("SZC") , STRING_TYPE , szSZC);
		struct_ZRZ.strSZC = CString(szSZC);
		TCHAR szZCS[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZCS") , INT_TYPE , szZCS);
		struct_ZRZ.strZCS = CString(szZCS);

		TCHAR szQLR[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("QLR_QLRMC") , STRING_TYPE , szQLR);
		struct_ZRZ.strQLR = CString(szQLR);

		TCHAR szFWJG[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("FWJG") , STRING_TYPE , szFWJG);
		struct_ZRZ.strJG = CString(szFWJG);

		CString strFamily;
		AcDbObjectIdArray idArrQLR;

		CSimpleBuildingHelper house;
		house.SetZRZ(aname(m_IdArrZRZ.at(nIndexZRZ)));
		house.GetZrzInformation_GD();
		CString strZCS;
		strZCS.Format(_T("%d"),house.m_floor_ents.size());
		struct_ZRZ.strZCS =strZCS;
		std::map<int , AcDbObjectIdArray>::iterator it_Index_IdArr = house.m_floor_ents.begin();

		int nFloorIndex = 0;
		AcDbObjectIdArray idArrFloor_Pre;
		AcDbObjectIdArray idArrFloor_Cur;
		CString strFloor;
		idArrFloor_Pre.setLogicalLength(0);
		for (;it_Index_IdArr != house.m_floor_ents.end();it_Index_IdArr++)
		{	
			idArrFloor_Cur.setLogicalLength(0);
			idArrFloor_Cur.append(it_Index_IdArr->second);
			std::map<int , AcDbObjectIdArray>::iterator itTemp = it_Index_IdArr;
			itTemp ++;
			if (itTemp != house.m_floor_ents.end())
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					idArrFloor_Pre.append(idArrFloor_Cur);
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					continue;
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					idArrFloor_Pre.append(idArrFloor_Cur);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					strFloor += _T(",");
				}	
			}
			else
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
				}	
			}	
			nFloorIndex ++;
		}
		m_mapBDCBH_ZRZinfo.insert(pair<CString , ZRZinfo>(saDatas_BDCH[nIndexZRZ] , struct_ZRZ));
	}
	actrTransactionManager->abortTransaction();
	buildMainBlocks_GD_Simple();
}

void CExportHouseDiagram::Export_Start_ZhiJiang()
{
	actrTransactionManager->startTransaction();
	m_nExportType = 1;
	AcDbObjectIdArray objArr;
	TCHAR szPath[1000] = {0};
	resbuf *cmd = ads_buildlist(RTSTR, _T("SimpleEntitysFilterDialog_invoke") ,  RTNONE);
	resbuf *list = NULL;
	int nState = ads_invoke(cmd, &list);
	ads_relrb(cmd); cmd = NULL;
	if(list == NULL) return;

	resbuf *rc = list;
	if (rc != NULL)
	{
		if(rc->restype == RTENAME)
		{
			long len = 0;
			ads_sslength(rc->resval.rlname, &len);
			if(len == 0)
			{ 
				return;
			}
			SSToIdArr(rc->resval.rlname, objArr, _T("$141010$"));
		}	
		rc = rc->rbnext;
	}

	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			_tcscpy(szPath, rc->resval.rstring);
			m_strPath.Format(_T("%s") , szPath);
			// 			if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
			// 			{
			// 				m_strPath += _T("分层分户图\\");
			// 			}
			// 			else
			// 			{
			// 				m_strPath += _T("\\分层分户图\\");
			// 			}
			CreateDirectoryForbidOverWrite(m_strPath);
		}
		rc = rc->rbnext;
	}
	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			m_strResult.Format(_T("%s") , rc->resval.rstring);
		}
	}
	AcDbObjectId idTK;
	AcDbObjectIdArray idArrTK;
	for (int i = 0;i < objArr.length();i++)
	{
		m_IdArrZRZ.append(objArr.at(i));
	}

	if (m_IdArrZRZ.length() == 0)
	{
		return;
	}
	CMatchFieldDlg dlg;
	if (!dlg.SetConfigFile(_T("Floor_MatchField.txt")))
	{
		return;
	}
	if(dlg.DoModal()!=IDOK) 
	{
		return;
	}
	m_strArrKey.Append(dlg.m_strArrKey);
	m_strArrValue.Append(dlg.m_strArrValue);
	vector<CString> saDatas_BDCH;
	AnalyzeData(saDatas_BDCH , m_strResult , _T(";"));
	for (int nIndexZRZ = 0;nIndexZRZ < m_IdArrZRZ.length();nIndexZRZ++)
	{
		ZRZinfo struct_ZRZ;
		TCHAR szZH[1000] = {0} , szZL[1000] = {0} , szZRZH[1000] = {0} , szHH[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZH") , STRING_TYPE , szZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZL") , STRING_TYPE , szZL);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZRZH") , STRING_TYPE , szZRZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_HH") , STRING_TYPE , szHH);
		struct_ZRZ.strZL = CString(szZL);
		struct_ZRZ.strZH = CString(szZH);
		struct_ZRZ.strHH = CString(szHH);
		TCHAR szZYJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_ZYJZMJ") , DOUBLE_TYPE , szZYJZMJ);
		struct_ZRZ.strZYJZMJ = CString(szZYJZMJ);
		TCHAR szFTJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_FTJZMJ") , DOUBLE_TYPE , szFTJZMJ);
		struct_ZRZ.strFTMJ = CString(szFTJZMJ);
		TCHAR szJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_JZMJ") , DOUBLE_TYPE , szJZMJ);
		struct_ZRZ.strJZMJ = CString(szJZMJ);
		TCHAR szSZC[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_SZC") , STRING_TYPE , szSZC);
		struct_ZRZ.strSZC = CString(szSZC);
		TCHAR szZCS[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZCS") , INT_TYPE , szZCS);
		struct_ZRZ.strZCS = CString(szZCS);

		TCHAR szQLR[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("QLR") , STRING_TYPE , szQLR);
		struct_ZRZ.strQLR = CString(szQLR);

		TCHAR szFWJG[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("FWJG") , STRING_TYPE , szFWJG);
		struct_ZRZ.strJG = CString(szFWJG);

		CString strFamily;
		AcDbObjectIdArray idArrQLR;

		CSimpleBuildingHelper house;
		house.SetZRZ(aname(m_IdArrZRZ.at(nIndexZRZ)));
		house.GetZrzInformation();
		int logic_num_temp=0;
		for (int i=0;i<house.m_floor_ents[1].length();i++)
		{
			ads_name ent_temp;
			TCHAR szCode[100]={0};
			TCHAR szTable[100]={0};
			acdbGetAdsName(ent_temp,house.m_floor_ents[1].at(i));
			GetStdm(ent_temp,szCode);
			CString strtable;
			strtable.Format(_T("%s"),szCode);
			_stprintf(szCode,_T("%s"),strtable.Left(3));;
			if (_tcscmp(szCode,_T("141"))==0)
			{
				logic_num_temp++;
			}
		}
		struct_ZRZ.logic_num =logic_num_temp;
		std::map<int , AcDbObjectIdArray>::iterator it_Index_IdArr = house.m_floor_ents.begin();

		int nFloorIndex = 0;
		AcDbObjectIdArray idArrFloor_Pre;
		AcDbObjectIdArray idArrFloor_Cur;
		CString strFloor;
		idArrFloor_Pre.setLogicalLength(0);
		for (;it_Index_IdArr != house.m_floor_ents.end();it_Index_IdArr++)
		{	
			idArrFloor_Cur.setLogicalLength(0);
			idArrFloor_Cur.append(it_Index_IdArr->second);
			std::map<int , AcDbObjectIdArray>::iterator itTemp = it_Index_IdArr;
			itTemp ++;
			if (itTemp != house.m_floor_ents.end())
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					idArrFloor_Pre.append(idArrFloor_Cur);
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					continue;
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					idArrFloor_Pre.append(idArrFloor_Cur);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					strFloor += _T(",");
				}	
			}
			else
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
				}	
			}	
			nFloorIndex ++;
		}
		m_mapBDCBH_ZRZinfo.insert(pair<CString , ZRZinfo>(saDatas_BDCH[nIndexZRZ] , struct_ZRZ));
	}
	actrTransactionManager->abortTransaction();
	buildMainBlocks_Simple_Zhijiang();
}

void CExportHouseDiagram::Export_Start_ShiShou()
{
	actrTransactionManager->startTransaction();
	m_nExportType = 1;
	AcDbObjectIdArray objArr;
	TCHAR szPath[1000] = {0};
	resbuf *cmd = ads_buildlist(RTSTR, _T("SimpleEntitysFilterDialog_invoke") ,  RTNONE);
	resbuf *list = NULL;
	int nState = ads_invoke(cmd, &list);
	ads_relrb(cmd); cmd = NULL;
	if(list == NULL) return;

	resbuf *rc = list;
	if (rc != NULL)
	{
		if(rc->restype == RTENAME)
		{
			long len = 0;
			ads_sslength(rc->resval.rlname, &len);
			if(len == 0)
			{ 
				return;
			}
			SSToIdArr(rc->resval.rlname, objArr, _T("$141010$"));
		}	
		rc = rc->rbnext;
	}

	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			_tcscpy(szPath, rc->resval.rstring);
			m_strPath.Format(_T("%s") , szPath);
			// 			if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
			// 			{
			// 				m_strPath += _T("分层分户图\\");
			// 			}
			// 			else
			// 			{
			// 				m_strPath += _T("\\分层分户图\\");
			// 			}
			CreateDirectoryForbidOverWrite(m_strPath);
		}
		rc = rc->rbnext;
	}
	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			m_strResult.Format(_T("%s") , rc->resval.rstring);
		}
	}
	AcDbObjectId idTK;
	AcDbObjectIdArray idArrTK;
	for (int i = 0;i < objArr.length();i++)
	{
		m_IdArrZRZ.append(objArr.at(i));
	}

	if (m_IdArrZRZ.length() == 0)
	{
		return;
	}
	CMatchFieldDlg dlg;
	if (!dlg.SetConfigFile(_T("Floor_MatchField.txt")))
	{
		return;
	}
	if(dlg.DoModal()!=IDOK) 
	{
		return;
	}
	m_strArrKey.Append(dlg.m_strArrKey);
	m_strArrValue.Append(dlg.m_strArrValue);
	vector<CString> saDatas_BDCH;
	AnalyzeData(saDatas_BDCH , m_strResult , _T(";"));
	for (int nIndexZRZ = 0;nIndexZRZ < m_IdArrZRZ.length();nIndexZRZ++)
	{
		ZRZinfo struct_ZRZ;
		TCHAR szZH[1000] = {0} , szZL[1000] = {0} , szZRZH[1000] = {0} , szHH[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZH") , STRING_TYPE , szZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZL") , STRING_TYPE , szZL);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZRZH") , STRING_TYPE , szZRZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_HH") , STRING_TYPE , szHH);
		struct_ZRZ.strZL = CString(szZL);
		struct_ZRZ.strZH = CString(szZH);
		struct_ZRZ.strHH = CString(szHH);
		TCHAR szZYJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_ZYJZMJ") , DOUBLE_TYPE , szZYJZMJ);
		struct_ZRZ.strZYJZMJ = CString(szZYJZMJ);
		TCHAR szFTJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_FTJZMJ") , DOUBLE_TYPE , szFTJZMJ);
		struct_ZRZ.strFTMJ = CString(szFTJZMJ);
		TCHAR szJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_JZMJ") , DOUBLE_TYPE , szJZMJ);
		struct_ZRZ.strJZMJ = CString(szJZMJ);
		TCHAR szSZC[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_SZC") , STRING_TYPE , szSZC);
		struct_ZRZ.strSZC = CString(szSZC);
		TCHAR szZCS[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZCS") , INT_TYPE , szZCS);
		struct_ZRZ.strZCS = CString(szZCS);

		TCHAR szQLR[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("QLR") , STRING_TYPE , szQLR);
		struct_ZRZ.strQLR = CString(szQLR);

		TCHAR szFWJG[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("FWJG") , STRING_TYPE , szFWJG);
		struct_ZRZ.strJG = CString(szFWJG);

		CString strFamily;
		AcDbObjectIdArray idArrQLR;

		CSimpleBuildingHelper house;
		house.SetZRZ(aname(m_IdArrZRZ.at(nIndexZRZ)));
		house.GetZrzInformation();
		int logic_num_temp=0;
		for (int i=0;i<house.m_floor_ents[1].length();i++)
		{
			ads_name ent_temp;
			TCHAR szCode[100]={0};
			TCHAR szTable[100]={0};
			acdbGetAdsName(ent_temp,house.m_floor_ents[1].at(i));
			GetStdm(ent_temp,szCode);
			CString strtable;
			strtable.Format(_T("%s"),szCode);
			_stprintf(szCode,_T("%s"),strtable.Left(3));;
			if (_tcscmp(szCode,_T("141"))==0)
			{
				logic_num_temp++;
			}
		}
		struct_ZRZ.logic_num =logic_num_temp;
		std::map<int , AcDbObjectIdArray>::iterator it_Index_IdArr = house.m_floor_ents.begin();

		int nFloorIndex = 0;
		AcDbObjectIdArray idArrFloor_Pre;
		AcDbObjectIdArray idArrFloor_Cur;
		CString strFloor;
		idArrFloor_Pre.setLogicalLength(0);
		for (;it_Index_IdArr != house.m_floor_ents.end();it_Index_IdArr++)
		{	
			idArrFloor_Cur.setLogicalLength(0);
			idArrFloor_Cur.append(it_Index_IdArr->second);
			std::map<int , AcDbObjectIdArray>::iterator itTemp = it_Index_IdArr;
			itTemp ++;
			if (itTemp != house.m_floor_ents.end())
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					idArrFloor_Pre.append(idArrFloor_Cur);
					continue;
				}
// 				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
// 				{
// 					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
// 					strFloor += _T(",");
// 					continue;
// 				}
// 				else
// 				{
					strFloor = strFloor.Left(strFloor.Find(_T(",")));
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					idArrFloor_Pre.append(idArrFloor_Cur);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					strFloor += _T(",");
/*				}*/	
			}
			else
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
					continue;
				}
// 				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
// 				{
// 					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
// 					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
// 				}
// 				else
// 				{
					strFloor = strFloor.Left(strFloor.Find(_T(",")));
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
//				}	
			}	
			nFloorIndex ++;
		}
		m_mapBDCBH_ZRZinfo.insert(pair<CString , ZRZinfo>(saDatas_BDCH[nIndexZRZ] , struct_ZRZ));
	}
	actrTransactionManager->abortTransaction();
	buildMainBlocks_Simple_ShiShou();
}

void CExportHouseDiagram::Export_Start_ZRZFloor_Sketch()
{
	actrTransactionManager->startTransaction();
	m_nExportType = 1;
	AcDbObjectIdArray objArr;
	TCHAR szPath[1000] = {0};
	resbuf *cmd = ads_buildlist(RTSTR, _T("SimpleZRZEntitysFilterDialog_invoke") ,  RTNONE);
	resbuf *list = NULL;
	int nState = ads_invoke(cmd, &list);
	ads_relrb(cmd); cmd = NULL;
	if(list == NULL) return;

	resbuf *rc = list;
	if (rc != NULL)
	{
		if(rc->restype == RTENAME)
		{
			long len = 0;
			ads_sslength(rc->resval.rlname, &len);
			if(len == 0)
			{ 
				return;
			}
			SSToIdArr(rc->resval.rlname, objArr, _T("$141010$"));
		}	
		rc = rc->rbnext;
	}

	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			_tcscpy(szPath, rc->resval.rstring);
			m_strPath.Format(_T("%s") , szPath);
			// 			if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
			// 			{
			// 				m_strPath += _T("分层分户图\\");
			// 			}
			// 			else
			// 			{
			// 				m_strPath += _T("\\分层分户图\\");
			// 			}
			CreateDirectoryForbidOverWrite(m_strPath);
		}
		rc = rc->rbnext;
	}
	if (rc != NULL)
	{
		if(rc->restype == RTSTR)
		{
			m_strResult.Format(_T("%s") , rc->resval.rstring);
		}
	}
	AcDbObjectId idTK;
	AcDbObjectIdArray idArrTK;
	for (int i = 0;i < objArr.length();i++)
	{
		m_IdArrZRZ.append(objArr.at(i));
	}

	if (m_IdArrZRZ.length() == 0)
	{
		return;
	}
	CMatchFieldDlg dlg;
	if (!dlg.SetConfigFile(_T("Floor_MatchField_1.txt")))   //分层草图使用模版system下
	{
		return;
	}
	if(dlg.DoModal()!=IDOK) 
	{
		return;
	}
	m_strArrKey.Append(dlg.m_strArrKey);
	m_strArrValue.Append(dlg.m_strArrValue);
	vector<CString> saDatas_BDCH;
	AnalyzeData(saDatas_BDCH , m_strResult , _T(";"));
	int nReportType =0;
	if(RTNORM != ads_getint(_T("\n生成比例尺方式:（0）自动（1）手动<0>"),&nReportType));
	for (int nIndexZRZ = 0;nIndexZRZ < m_IdArrZRZ.length();nIndexZRZ++)
	{
		ZRZinfo struct_ZRZ;
		TCHAR szZH[1000] = {0} , szZL[1000] = {0} , szZRZH[1000] = {0} , szHH[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZH") , STRING_TYPE , szZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZL") , STRING_TYPE , szZL);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZRZH") , STRING_TYPE , szZRZH);
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_HH") , STRING_TYPE , szHH);
		struct_ZRZ.strZL = CString(szZL);
		struct_ZRZ.strZH = CString(szZH);
		struct_ZRZ.strHH = CString(szHH);
		TCHAR szZYJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_ZYJZMJ") , DOUBLE_TYPE , szZYJZMJ);
		struct_ZRZ.strZYJZMJ = CString(szZYJZMJ);
		TCHAR szFTJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_FTJZMJ") , DOUBLE_TYPE , szFTJZMJ);
		struct_ZRZ.strFTMJ = CString(szFTJZMJ);
		TCHAR szJZMJ[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_JZMJ") , DOUBLE_TYPE , szJZMJ);
		struct_ZRZ.strJZMJ = CString(szJZMJ);
		TCHAR szSZC[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ROOM_SZC") , STRING_TYPE , szSZC);
		struct_ZRZ.strSZC = CString(szSZC);
		TCHAR szZCS[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("ZCS") , INT_TYPE , szZCS);
		struct_ZRZ.strZCS = CString(szZCS);

		TCHAR szQLR[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("QLR") , STRING_TYPE , szQLR);
		struct_ZRZ.strQLR = CString(szQLR);

		TCHAR szFWJG[1000] = {0};
		ReadXdata(aname(m_IdArrZRZ.at(nIndexZRZ)) , _T("FWJG") , STRING_TYPE , szFWJG);
		struct_ZRZ.strJG = CString(szFWJG);

		CString strFamily;
		AcDbObjectIdArray idArrQLR;

		CSimpleBuildingHelper house;
		house.SetZRZ(aname(m_IdArrZRZ.at(nIndexZRZ)));
		house.GetZrzInformation();
		int logic_num_temp=0;
		for (int i=0;i<house.m_floor_ents[1].length();i++)
		{
			ads_name ent_temp;
			TCHAR szCode[100]={0};
			TCHAR szTable[100]={0};
			acdbGetAdsName(ent_temp,house.m_floor_ents[1].at(i));
			GetStdm(ent_temp,szCode);
			CString strtable;
			strtable.Format(_T("%s"),szCode);
			_stprintf(szCode,_T("%s"),strtable.Left(3));;
			if (_tcscmp(szCode,_T("141"))==0)
			{
				logic_num_temp++;
			}
		}
		struct_ZRZ.logic_num =logic_num_temp;
		std::map<int , AcDbObjectIdArray>::iterator it_Index_IdArr = house.m_floor_ents.begin();

		int nFloorIndex = 0;
		AcDbObjectIdArray idArrFloor_Pre;
		AcDbObjectIdArray idArrFloor_Cur;
		CString strFloor;
		idArrFloor_Pre.setLogicalLength(0);
		for (;it_Index_IdArr != house.m_floor_ents.end();it_Index_IdArr++)
		{	
			idArrFloor_Cur.setLogicalLength(0);
			idArrFloor_Cur.append(it_Index_IdArr->second);
			std::map<int , AcDbObjectIdArray>::iterator itTemp = it_Index_IdArr;
			itTemp ++;
			if (itTemp != house.m_floor_ents.end())
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					idArrFloor_Pre.append(idArrFloor_Cur);
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					strFloor += _T(",");
					continue;
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					idArrFloor_Pre.append(idArrFloor_Cur);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					strFloor += _T(",");
				}	
			}
			else
			{
				if(idArrFloor_Pre.length() == 0)
				{
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
					continue;
				}
				if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
				{
					strFloor.Format(_T("%s%d") , strFloor , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
				}
				else
				{
					strFloor = strFloor.Left(strFloor.GetLength() - 1);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Pre));
					idArrFloor_Pre.setLogicalLength(0);
					strFloor.Format(_T(""));
					strFloor.Format(_T("%d") , it_Index_IdArr->first);
					struct_ZRZ.mapFloor_IdArr.insert(pair<CString , AcDbObjectIdArray>(strFloor , idArrFloor_Cur));
				}	
			}	
			nFloorIndex ++;
		}
		m_mapBDCBH_ZRZinfo.insert(pair<CString , ZRZinfo>(saDatas_BDCH[nIndexZRZ] , struct_ZRZ));
		actrTransactionManager->abortTransaction();
		buildMainBlocks_ZRZFloor_Sketch(nReportType);
		m_mapBDCBH_ZRZinfo.clear();
	}
	acutPrintf(_T("输出完成！"));
}

bool CExportHouseDiagram::CompareIdArr(AcDbObjectIdArray& idsFirst , AcDbObjectIdArray& idsSecond)
{
	bool isSame = true;
	int nSame = 0;
	int nDifferent = 0;
	if (idsSecond.length() != idsFirst.length())
	{
		return false;
	}
	for (int i = 0;i < idsFirst.length();i++)
	{
		for (int j = 0;j < idsSecond.length();j++)
		{
			if (idsFirst.at(i) == idsSecond.at(j))
			{
				break;
			}
			else
			{
				if (j == (idsSecond.length() - 1))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void CExportHouseDiagram::Export_Start()
{
	m_nExportType = 2;
	actrTransactionManager->startTransaction();
	Show_All();
	AcDbObjectIdArray objArr;
	TCHAR szPath[4000] = {0};
	resbuf *cmd = ads_buildlist(RTSTR , _T("EntitysFilterDialog_invoke") , RTNONE);
	resbuf *list = NULL;
	int nState = ads_invoke(cmd , &list);
	ads_relrb(cmd); 
	cmd = NULL;
	if(list == NULL) return;
	resbuf *rc = list;
	if(rc->restype == RTENAME)
	{
		long len = 0;
		ads_sslength(rc->resval.rlname, &len);
		if(len == 0)
		{ 
			AfxMessageBox(_T("选择户。"));
			return;
		}
		SSToIdArr(rc->resval.rlname, objArr, _T("$room_code$"));
	}	
	rc = rc->rbnext;
	if(rc->restype == RTSTR)
	{
		_tcscpy(szPath, rc->resval.rstring);
		m_strPath.Format(_T("%s") , szPath);
		if (0 == _tcscmp(m_strPath.Right(1) , _T("\\")))
		{
			m_strPath += _T("分层分户图\\");
		}
		else
		{
			m_strPath += _T("\\分层分户图\\");
		}
		CreateDirectoryForbidOverWrite(m_strPath);
	}
	AcDbObjectId idTK;
	AcDbObjectIdArray idArrTK;
	for (int i = 0;i < objArr.length();i++)
	{
		m_IdArrExtern.append(objArr.at(i));
	}
	if (m_IdArrExtern.length() == 0)
	{
		return;
	}
	CMatchFieldDlg dlg;
	if (!dlg.SetConfigFile(_T("Floor_MatchField.txt")))
	{
		return;
	}
	if(dlg.DoModal()!=IDOK) 
	{
		return;
	}
	m_strArrKey.Append(dlg.m_strArrKey);
	m_strArrValue.Append(dlg.m_strArrValue);

	for (int nTKIndex = 0;nTKIndex < m_IdArrExtern.length();nTKIndex++)
	{
		AcGePoint2dArray nodes;
		GetPlList(aname(m_IdArrExtern.at(nTKIndex)) , nodes);
		TCHAR szFSINFO[300] = {0};
		vector<CString> saDatas_FS;
		ReadXdata(aname(m_IdArrExtern.at(nTKIndex)) , _T("FSINFO") , STRING_TYPE , szFSINFO);
		AnalyzeData(saDatas_FS , CString(szFSINFO) , _T(";"));
		if (saDatas_FS.size() == 2)
		{
			vector<CString> veFloor;
			AnalyzeData(veFloor , CString(saDatas_FS[1]) , _T(","));
			m_mapRoom_Floor.insert(pair<CString , vector<CString> >(saDatas_FS[0] , veFloor));
		}


		AcDbObjectIdArray idArr , idArrArcText;
		ads_name ssWN , ssArcText;
		idArr.setLogicalLength(0);
		ads_relrb(rc);
		rc = NULL;
		rc = acutBuildList(RTDXF0 , _T("*LINE") ,0);
		ssFromNodes(nodes , ssWN , _T("CP") , 0 , false , rc);
		SSToIdArr(ssWN , idArr);
		ads_relrb(rc);
		rc = NULL;

		rc = acutBuildList(RTDXF0 , _T("TEXT") ,0);
		ssFromNodes(nodes , ssArcText , _T("CP") , 0 , false , rc);
		SSToIdArr(ssArcText , idArrArcText , _T("$ARC$"));
		ads_relrb(rc);
		rc = NULL;
		idArr.append(idArrArcText);
		map<CString , AcDbObjectIdArray> map_Layer_IdArr;
		map<CString, vector<double> > map_Layer_dScale;
		for (int i = 0;i < idArr.length();i++)
		{
			TCHAR szLayer[1000] = {0};
			GetEntLayer(idArr.at(i) , szLayer);
			if (0 == _tcscmp(szLayer , _T("room_extend_layer")))
			{
				continue;
			}
			TCHAR szSTDM[1000] = {0};
			GetStdm(aname(idArr.at(i)) , szSTDM);
			if (0 == _tcscmp(_T("room_line") , szSTDM))
			{
				setlinetype(aname(idArr.at(i)),_T("HIDDEN2"));
			}
			CString strLayer;
			strLayer.Format(_T("%s") , szLayer);
			map<CString , AcDbObjectIdArray>::iterator it = map_Layer_IdArr.find(strLayer);
			map<CString, vector<double> >::iterator it_Scale = map_Layer_dScale.find(strLayer);
			if(it != map_Layer_IdArr.end())
			{
				it->second.append(idArr.at(i));
				TCHAR szScale[1000] = { 0 };
				ReadXdata(aname(idArr.at(i)), _T("area_scale"), DOUBLE_TYPE, szScale);
				double dCale = _tstof(szScale);
				it_Scale->second.push_back(dCale);
			}
			else
			{
				AcDbObjectIdArray ids;
				ids.append(idArr.at(i));
				map_Layer_IdArr.insert(pair<CString , AcDbObjectIdArray>(strLayer , ids));
				TCHAR szScale[1000] = { 0 };
				ReadXdata(aname(idArr.at(i)), _T("area_scale"), DOUBLE_TYPE, szScale);
				double dCale = _tstof(szScale);
				vector<double> veScale;
				veScale.push_back(dCale);
				map_Layer_dScale.insert(pair<CString, vector<double> >(strLayer, veScale));
			}
		}
		m_vector_map_Layer_IdArr.push_back(map_Layer_IdArr);
		m_vector_map_Layer_dScale.push_back(map_Layer_dScale);
	}
	actrTransactionManager->abortTransaction();
	buildMainBlocks_new();
    //原图图层乱序，做调整还原
	ads_name ssNameALL, ssNameArea;
	acedSSGet(_T("X"), NULL, NULL, NULL, ssNameALL);
	AcDbObjectIdArray idArrArea;
	SSToIdArr(ssNameALL, idArrArea, _T("$400000$"));
	IdArrToSS(idArrArea, ssNameArea);
	if (idArrArea.length() > 0)
	{
		acedCommand(RTSTR, _T("DRAWORDER"), RTPICKS, ssNameArea, RTSTR, _T(""), RTSTR, _T("F"), RTNONE);
	}
	ads_ssfree(ssNameALL);
}

// void CExportHouseDiagram::GetFSInfo()
// {
// 	int nCount = GetXDataCount(m_ent , _T("FSINFO"));
// 	for (int i = 0;i < nCount;i++)
// 	{
// 		TCHAR szFSINFO[500] = {0};
// 		READMulXData(aname(m_ent) , _T("FSINFO") , szFSINFO , STRING_TYPE , i);
// 		CString strFSINFO;
// 		strFSINFO.Format(_T("%s") , szFSINFO);
// 		vector<CString> saDatas;
// 		AnalyzeData(saDatas , strFSINFO , _T(";"));
// 		if (saDatas.size() == 2)
// 		{
// 			vector<CString> floorInfo;
// 			AnalyzeData(floorInfo , saDatas[1] , _T(","));
// 			m_mapRoom_Floor.insert(pair<CString , vector<CString> >(saDatas[0] , floorInfo));
// 		}
// 	}
// }

bool CExportHouseDiagram::GetPlotSizeName(AcDbPlotSettingsValidator* pPSV, AcDbPlotSettings* pPlotSettings, CString& real_size_name)
{
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); 
	AcDbObjectId btrId=pLayoutManager->getActiveLayoutBTRId();
	AcDbBlockTableRecord *pBTR;
	Acad::ErrorStatus es=acdbOpenObject(pBTR,btrId,AcDb::kForRead);
	AcDbObjectId layoutId=pBTR->getLayoutId();
	AcDbLayout *pLayout=NULL;
	acdbOpenObject(pLayout,layoutId,AcDb::kForRead);
	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	pBTR->close();
	es=pPSV->setPlotCfgName(pPlotSettings, m_str_divice);
	pPSV->refreshLists(pPlotSettings);
	AcArray<const TCHAR*> MediaList;
	pPSV->canonicalMediaNameList(pPlotSettings,MediaList);

	for(int i=0; i<MediaList.length(); i++)
	{
		const TCHAR *pLocaleName;
		pPSV->getLocaleMediaName(pPlotSettings,MediaList.at(i),pLocaleName);
		if (0 == _tcsicmp(pLocaleName, m_str_size_name))
		{
			real_size_name.Format(_T("%s"), MediaList[i]);
			return true;
		}
	}
	return false;
}

void CExportHouseDiagram::Plot()
{
	// 必须设置后台打印否则速度奇慢
	struct resbuf rb;
	rb.restype = 5003;
	rb.resval.rint = 0;
	ads_setvar(_T("BACKGROUNDPLOT"),&rb); 
#if  _MSC_VER == 1500
	//ChangeCassappInLoadFile();
#endif
	es_count = 0;
	AcDbPlotSettingsValidator* pPSV = NULL;
	AcDbPlotSettings* pPlotSettings = NULL;
	pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
	pPlotSettings = new AcDbPlotSettings(true);
	CString real_size_name;
	if (!GetPlotSizeName(pPSV, pPlotSettings, real_size_name))
	{
		CString str;
		str.Format(_T("未找到名为\"%s\"的打印尺寸，请在打印设置中设置"), m_str_size_name);
		if (pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
		AfxMessageBox(str);
		return;
	}
	Acad::ErrorStatus es;
	AcAxDocLock lock;
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager();
	AcDbObjectId btrId = pLayoutManager->getActiveLayoutBTRId();
	AcDbBlockTableRecord *pBTR;
	es = acdbOpenObject(pBTR, btrId, AcDb::kForRead);
	// We want to instantiate our custom AcDbPlotSettings object, and inherit the 
	// properties from the active layout.
	AcDbObjectId layoutId = pBTR->getLayoutId();
	AcDbLayout *pLayout = NULL;
	es = acdbOpenObject(pLayout, layoutId, AcDb::kForRead);

	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	pBTR->close();
	pPlotSettings->setPlotHidden(false);
	es = pPSV->setPlotRotation(pPlotSettings, AcDbPlotSettings::k0degrees);
	//acadErrorStatusTextChs(es);
	//设置当前打印机
	es = pPSV->setPlotCfgName(pPlotSettings, m_str_divice);
	pPSV->refreshLists(pPlotSettings);
	es = pPSV->setUseStandardScale(pPlotSettings, Adesk::kTrue);
	es = pPSV->setStdScaleType(pPlotSettings, StdScaleType::kScaleToFit);
	vector<AcGePoint2dArray> v_extents;
	AcGePoint2dArray nodes , extents;
	ads_name ssSave;
	IdArrToSS(m_objArrSave , ssSave);
	if (!GetEntsExtent(ssSave , nodes))
	{
		AfxMessageBox(_T("没有找到宗地图框！"));
		if (pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
		//acDocManager->unlockDocument(acDocManager->curDocument());
		return;
	}
	acedSSFree(ssSave);
	
	if ( false == IsDeasil(nodes) ) extents.reverse();
	if(Poly2dArrayOffset(nodes , 7, true, extents) == false)
	{
		extents.setLogicalLength( 0 );
		extents.append( nodes );
		AfxMessageBox(_T("外扩失败"));
		acutPrintf(_T("\noff failed"));
	}
	
// 	acdbGetAdsName(ent, m_objTukuang);
// 	if (!GetEntExtent(ent , extents , 7.0))
// 	{
// 		AfxMessageBox(_T("没有找到宗地图框！"));
// 		if (pPlotSettings)
// 		{
// 			delete pPlotSettings;
// 			pPlotSettings = NULL;
// 		}
// 		//acDocManager->unlockDocument(acDocManager->curDocument());
// 		return;
// 	}
	//DrawPolyline2D(extents, 0, true, _T(""), 0.5);
	//OffsetPt2dArray(m_TKNodes, extents, 10.0);
	es = pPSV->setPlotWindowArea(pPlotSettings, extents.at(0)[X], extents.at(0)[Y], extents.at(2)[X], extents.at(2)[Y]);
	//acadErrorStatusTextChs(es);
	es = pPSV->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);
	//acadErrorStatusTextChs(es);
	es = pPSV->setPlotCentered(pPlotSettings, true);
	//acadErrorStatusTextChs(es);
	const TCHAR* p = (LPCTSTR)real_size_name;
	es = pPSV->setCanonicalMediaName(pPlotSettings, p);
	//acadErrorStatusTextChs(es);
	double numerator, denominator;
	es = pPlotSettings->getCustomPrintScale(numerator, denominator);
	//acadErrorStatusTextChs(es);
	double m_Scale = numerator / denominator;
	//es = pPSV->setCurrentStyleSheet(pPlotSettings, _T("acad.stb"));
	//acadErrorStatusTextChs(es);
	//开始打印
	es = pPSV->setPlotCentered(pPlotSettings, true);
	//acadErrorStatusTextChs(es);
	// Create a publish engine, and send the plot through...
	AcPlPlotEngine* pEngine = NULL;
	if (Acad::eOk == AcPlPlotFactory::createPublishEngine(pEngine))
	{
		// Here is the progress dialog for the current plot process...
		AcPlPlotProgressDialog *pPlotProgDlg = acplCreatePlotProgressDialog(acedGetAcadFrame()->m_hWnd, false, 1);

		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::PlotMSGIndex::kDialogTitle, _T("打印"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::PlotMSGIndex::kCancelJobBtnMsg, _T("取消任务"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::PlotMSGIndex::kCancelSheetBtnMsg, _T("取消图纸"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::PlotMSGIndex::kSheetSetProgressCaption, _T("任务进度"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::PlotMSGIndex::kSheetProgressCaption, _T("图纸进度"));

		pPlotProgDlg->setPlotProgressRange(0, 100);
		pPlotProgDlg->onBeginPlot();
		pPlotProgDlg->setIsVisible(true);

		es = pEngine->beginPlot(pPlotProgDlg);
		//acadErrorStatusTextChs(es);
		AcPlPlotPageInfo pageInfo;
		AcPlPlotInfo plotInfo; // Used to describe how the plot is to be made

		// First, set the layout to the specified layout (which is the current layout in this sample)
		plotInfo.setLayout(layoutId);// This is required.

		// Now, override the layout settings with the plot settings we have been populating
		plotInfo.setOverrideSettings(pPlotSettings);

		// We need to validate these settings.
		AcPlPlotInfoValidator validator;
		validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::MatchingPolicy::kMatchEnabled);
		es = validator.validate(plotInfo);
		//acadErrorStatusTextChs(es);
		CString str_dwg_pathname;
		//GetZdtPathName(str_dwg_pathname);
		str_dwg_pathname = m_strTempSavePath + _T(".dwg");
		str_dwg_pathname.Replace(_T(".dwg"), _T(".jpg"));
		vector<CString> saDatas;
		CString strFileName;
		CString strReplaceFileName;
		AnalyzeData(saDatas , str_dwg_pathname , _T("\\"));
		if (saDatas.size() > 0)
		{
			strFileName.Format(_T("%s") , saDatas.at(saDatas.size() - 1));
			strReplaceFileName = _T("分层分户图_") + strFileName;
			str_dwg_pathname.Replace(strFileName, strReplaceFileName);
		}
		
		TCHAR jpg_pathname[MAX_PATH] = { 0 };
		_tcscpy(jpg_pathname, str_dwg_pathname);
		const TCHAR *szDocName = acDocManager->curDocument()->fileName();
		es = pEngine->beginDocument(plotInfo, szDocName, NULL, 1, true, jpg_pathname);
		//if (pEngine->isBackgroundPackaging())
		//{
		//	AfxMessageBox(_T("后台打印！"));
		//}
		pPlotProgDlg->onBeginSheet();
		pPlotProgDlg->setSheetProgressRange(0, 100);
		pPlotProgDlg->setSheetProgressPos(0);

		es = pEngine->beginPage(pageInfo, plotInfo, true);
		//acadErrorStatusTextChs(es);
		es = pEngine->beginGenerateGraphics();
		//acadErrorStatusTextChs(es);
		es = pEngine->endGenerateGraphics();
		//acadErrorStatusTextChs(es);
		es = pEngine->endPage();
		//acadErrorStatusTextChs(es);
		pPlotProgDlg->setSheetProgressPos(100);
		pPlotProgDlg->onEndSheet();
		pPlotProgDlg->setPlotProgressPos(100);
		es = pEngine->endDocument();
		//acadErrorStatusTextChs(es);
		es = pEngine->endPlot();
		//AfxMessageBox(_T("Plot endPlot"));
		//acadErrorStatusTextChs(es);
		// Destroy the engine 
		pEngine->destroy();
		pEngine = NULL;
		// and the progress dialog.
		pPlotProgDlg->destroy();
		if (pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
	}
	else
	{
		AfxMessageBox(_T("正在进行其他打印任务，请完成后重试！"));
		return;
	}
#if  _MSC_VER == 1500
	//RestoreCassappInLoadFile();
#endif
}

