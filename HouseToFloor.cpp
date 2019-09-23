#include "stdafx.h"
#include "HouseToFloor.h"
#include "HouseManagerHelper.h"
#include "LockManager.h"
#include "RollBackManager.h"
#include "LineTextHelper.h"
#include "DatabaseOperManager.h"
#include "Layers.h"
#include "apoint.hpp"

CHouseToFloor::CHouseToFloor(void)
{
	_tcscpy(m_zrz_floder , _T(""));
	_tcscpy(m_dwg_name , _T(""));
	
	read1(m_dH);
	m_dH /= 1000.0;

	m_current_qlr_database_name = _T("");
}


CHouseToFloor::~CHouseToFloor(void)
{
}
void CHouseToFloor::SetEnt(ads_name ent)
{
	ads_name_set(ent , m_ent);
	m_family.set_zrz(m_ent);
	GetEntExtent(m_ent , m_ptMin , m_ptMax , 5);
	//获得幢走向
	ads_point pt1 ={0}, pt2 = {0};
	GetEntityTrend(m_ent , pt1 , pt2);
	//计算角度
	double dAngle = ads_angle(pt1, pt2);
	m_rotate_angle = -dAngle;
	double dTemp = m_rotate_angle / PI;
	while (dTemp + 0.5 < 0)
	{
		dTemp += 1;
	}

	while (dTemp - 0.5 > 0)
	{
		dTemp -= 1;
	}
	m_rotate_angle = dTemp * PI;
	



	CXRecordOperate xRecord;
	//double dAngle = 0.0;
	xRecord.AddDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
	//获得当前的权利人数据库的名称
	CString str = GetCurrentDwgName();
	CString strFull;
	strFull = GetCurrentDwgFullPath();
	str.MakeLower();
	str.Replace(_T(".dwg"),_T(".mdb"));

	strFull.MakeLower();
	strFull.Replace(_T(".dwg"),_T(".mdb"));

	double dScale;
	read1(dScale);
	m_dH = dScale / 1000.0;

	TCHAR szNum[50] = {0};
	ReadXdata(m_ent , _T("ZRZH"),0 , szNum);
	m_strZRZH.Format(_T("%s") , szNum);
	m_current_qlr_database_name = str;	
	m_current_qlr_database_full_name = strFull;
	//AfxMessageBox(m_current_qlr_database_full_name);
	entitysInfo::GetEntHandle(m_ent , m_szHandle);
	//获得工程数据库信息
	m_righter_helper.InitialInformation();
}

bool CHouseToFloor::BeginToHousePlane()
{
		CLockManager lock;
	CRollBackManager rollback;//回滚起点
	CString strPath = GetZRZPath(_T(""),m_ent);
	if(strPath.IsEmpty())
	{
		acutPrintf(_T("\n获得自然幢文件夹失败"));
		return false;
	}
	_stprintf(m_zrz_floder , _T("%s") , strPath);
	

	if(false == GetZRZInfomation())
	{
		acutPrintf(_T("\n获得自然幢户信息 或 幢内房屋信息失败"));
		rollback.Abort();
		return false;
	}
	//保存
	//GetHousePlan(m_strZHH);
	rollback.Abort();
	return false;
}

bool CHouseToFloor::BeginToHousePlane_GD()
{
	CLockManager lock;
	CRollBackManager rollback;//回滚起点
	if (IsFWZRZ(m_ent))
	{
		if(false == GetZRZInfomation_GD())
		{
			acutPrintf(_T("\n获得自然幢户信息 或 幢内房屋信息失败"));
			rollback.Abort();
			return false;
		}
	}
	else
	{
		CString strPath = GetZRZPath(_T(""),m_ent);
		if(strPath.IsEmpty())
		{
			acutPrintf(_T("\n获得自然幢文件夹失败"));
			return false;
		}
		_stprintf(m_zrz_floder , _T("%s") , strPath);
		if(false == GetZRZInfomation())
		{
			acutPrintf(_T("\n获得自然幢户信息 或 幢内房屋信息失败"));
			rollback.Abort();
			return false;
		}
	}
	//保存
	//GetHousePlan(m_strZHH);
	rollback.Abort();
	return false;
}


bool CHouseToFloor::BeginToHouseFloor_Simple()
{
	CLockManager lock;
	CRollBackManager rollback;//回滚起点
	CString strPath = GetZRZPath(_T(""),m_ent);
	if(strPath.IsEmpty())
	{
		acutPrintf(_T("\n获得自然幢文件夹失败"));
		return false;
	}
	_stprintf(m_zrz_floder , _T("%s") , strPath);

	if(false == GetZRZInfomation())
	{
		acutPrintf(_T("\n获得自然幢户信息 或 幢内房屋信息失败"));
		rollback.Abort();
		return false;
	}
	rollback.Abort();
	return false;
}

CString CHouseToFloor::GetImagePath()
{
 	CString strPath = GetCurrentDwgFullPath();
// 	strPath = strPath.Find(_T("\\"));
// // 	strPath.MakeUpper();
// // 	strPath.Replace(_T(".DWG") , _T(".MDB"));
// // 	if(isFileExsist(strPath) == false)
// // 		return _T("");
 	return strPath;
}


bool CHouseToFloor::BeginToClassfiy_TK()
{
	CLockManager lock;
	CRollBackManager rollback;//回滚起点
	//if(false == GetZRZInfomation_floor())
	if(false == GetZRZInfomation_FC())
	{
		acutPrintf(_T("\n获得自然幢户信息 或 幢内房屋信息失败"));
		rollback.Abort();
		return false;
	}
	//保存
	SaveClassLayer();
	rollback.Abort();
	return true;
	rollback.Abort();
	return false;
}

bool CHouseToFloor::SaveCurDwg(CString str_path)
{
	int es = acDocManager->mdiActiveDocument()->database()->saveAs(str_path.GetBuffer());
	str_path.ReleaseBuffer();
	if (es == Acad::eOk)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CHouseToFloor::SaveCurDwg()
{
	CString str_path = GetCurrentDwgFullPath();
	return SaveCurDwg(str_path);
}
bool CHouseToFloor::BeginToClassfiy()
{
	CLockManager lock;
	CRollBackManager rollback;//回滚起点
	CString strPath = GetZRZPath(_T(""),m_ent);
	if(strPath.IsEmpty())
	{
		acutPrintf(_T("\n获得自然幢文件夹失败"));
		return false;
	}
	_stprintf(m_zrz_floder , _T("%s") , strPath);
	////判断当前图幢是否已经分层了
	bool bCreate = true;
	TCHAR szGUID[50] = {0};
	_stprintf(szGUID , _T("%s"),g_floor_dwg_name);
	TCHAR szValue[100] = {0};
	ReadXdata(m_ent , szGUID , 0 , szValue);//szValue是带有后缀dwg的
	if(_tcslen(szValue) > 0)//已经存在了
	{
		if ( MessageBox(adsw_hwndAcad,_T("当前幢已经有分户DWG，是否重新生成"),_T("CASS"),MB_YESNO) != IDYES )
		{
			bCreate = false;//不重新生成
			//打开对应的图形
			_stprintf(m_dwg_name , _T("%s%s"),m_zrz_floder ,szValue);
			m_dwg_guid.Format(_T("%s.dwg") , szValue);
			//打开图形
			SendCommand(_T("OpenDwgCmd "));
			SendCommand(m_szHandle);
			SendCommand(_T(" "));
			TCHAR szDatabaseName_Full[1000] = {0};
			_stprintf(szDatabaseName_Full , _T("%s") , m_current_qlr_database_full_name);
			CXRecordOperate xRecord;
			xRecord.AddStringRecord(_T("PROPERTYDATABASE_Full"),_T("PROPERTYDATABASE_Full"),szDatabaseName_Full);//当前的权利人数据库名称		
			return true;
		}
	}
	CString strname = newGUID3();
	_stprintf(m_dwg_name , _T("%s%s.dwg"),m_zrz_floder ,strname);
	m_dwg_guid.Format(_T("%s.dwg") , strname);
	if(bCreate)//新生成 是否删除原来的
	{
		TCHAR szDeletePath[MAX_PATH] = {0};
		_stprintf(szDeletePath , _T("%s%s"),m_zrz_floder ,szValue);
		if(IsPathFileExists(szDeletePath))
			DeleteFile((CString)szDeletePath);
	}
	if(false == GetZRZInfomation())
	{
		acutPrintf(_T("\n获得自然幢户信息 或 幢内房屋信息失败"));
		return false;
	}
// 	setcolor(aname(m_floor_ents.begin()->second.at(0)),1);
// 	MoveEntityTo( m_floor_ents.begin()->second.at(0) , AcGePoint3d(0,0,0));
// 	return false;
	//保存
	bool bOK = SaveFamilyDwg();
    //回滚
	rollback.Abort();
	//为当前自然幢写入户式dwg名称
	TCHAR szDwgName[50] = {0};
	_stprintf(szDwgName , _T("%s"),g_floor_dwg_name);
	AddXdata(m_ent , szDwgName , 0 , m_dwg_guid);
	SaveCurDwg();
	//如果成功 打开
	if(bOK)
	{
		SendCommand(_T("OpenDwgCmd "));
		SendCommand(m_szHandle);
		SendCommand(_T(" "));
	}
	return true;
}


bool CHouseToFloor::BeginToClassfiy_Export()
{
	CLockManager lock;
	CString strPath = GetZRZPath(_T(""),m_ent);
	if(strPath.IsEmpty())
	{
		acutPrintf(_T("\n获得自然幢文件夹失败"));
		return false;
	}
	_stprintf(m_zrz_floder , _T("%s") , strPath);
	////判断当前图幢是否已经分层了
	bool bCreate = true;
	TCHAR szGUID[50] = {0};
	_stprintf(szGUID , _T("%s"),g_floor_dwg_name);
	TCHAR szValue[100] = {0};
	ReadXdata(m_ent , szGUID , 0 , szValue);//szValue是带有后缀dwg的
	if(_tcslen(szValue) == 0)//已经存在了
	{
		acutPrintf(_T("分户图还没生成。"));
	}
	CString strname = newGUID3();
	_stprintf(m_dwg_name , _T("%s%s.dwg"),m_zrz_floder ,strname);
	m_dwg_guid.Format(_T("%s.dwg") , strname);
	if(bCreate)//新生成 是否删除原来的
	{
		TCHAR szDeletePath[MAX_PATH] = {0};
		_stprintf(szDeletePath , _T("%s%s"),m_zrz_floder ,szValue);
		if(IsPathFileExists(szDeletePath))
			DeleteFile((CString)szDeletePath);
	}
	if(false == GetZRZInfomation())
	{
		acutPrintf(_T("\n获得自然幢户信息 或 幢内房屋信息失败"));
		return false;
	}
	//保存
	SaveFamilyDwg();
	return true;
}

bool CHouseToFloor::GetZRZInfomation_floor()
{
	m_floor_ents.clear();
	//m_room_list.clear();
	AcGePoint2dArray node;
	GetPlList(m_ent , node);
	if(node.length() < 3)
		return false;
// 	if(false ==m_family.get_zrz_infomation(m_room_list))
// 	{
// 		acutPrintf(_T("\n获得自然幢户室信息失败"));
// 		return false;
// 	}
	//获得自然幢内部的所有房屋
	ads_name ssName,ent;
	struct resbuf * rc = acutBuildList(RTDXF0 , _T("*POLYLINE"),0);
	bool bSearch = ssFromNodes(node , ssName , _T("WP"),1.0, false , rc );
	acutRelRb(rc);
	if(bSearch == false)
	{
		ads_ssfree(ssName);
		acutPrintf(_T("\n选择房屋失败"));
		return false;
	}
	long nLen = 0L;
	AcDbObjectIdArray idList;
	SSToIdArr(ssName , idList);
	nLen = idList.length();
	//这里旋转实体
	if(fabs(m_rotate_angle) > 0.001)
	{
		RotateEntitysByTrend(idList , m_rotate_angle , m_ptMin);//这里旋转所有实体
		GetEntsExtent(idList , m_ptMin , m_ptMax , 5);
	}
	m_floor_ents.clear();
	std::map<int , AcDbObjectIdArray>::iterator iter;
	AGrid_ClearSheet(_T("获得房屋信息失败"));
	for (int i = 0 ; i < nLen; i++)
	{
		AcDbObjectId id = idList.at( i );
		acdbGetAdsName(ent , id);
		//if(ads_name_equal(ent , m_ent))
		//	continue;
		int up = 0 , down = 0;
		FloorType type = UpAndDown;//默认是地上地下
		CString strError;
		if(false == GetBuildingFloorInformation(ent ,type ,up , down , strError))
		{
			AGrid_AppendMsgEnt(_T("获得房屋信息失败") , strError , strError , ent);
			continue;
		}
		if(type == UpAndDown)
		{
			for (int j = 1 ; j <= up ; j++)//地上层数
			{
 				iter = m_floor_ents.find(j);
 				if(iter == m_floor_ents.end())
 				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
				}
 				else
 				{
 					iter->second.append(id);
				}
			}
			if(down > 0)
			{
				for (int j = -1 * down ; j < 0 ; j++)
				{
 					iter = m_floor_ents.find(j);
 					if(iter == m_floor_ents.end())
 					{
						AcDbObjectIdArray ids;
						ids.append(id);
						m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
					}
 					else
 					{
 						iter->second.append(id);
 					}
				}
			}	
		}
		else //第几层到第几层
		{
			for (int j = up ; j <= down ; j++)
			{
 				iter = m_floor_ents.find(j);
 				if(iter == m_floor_ents.end())
 				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
 				}
 				else
 				{
 					iter->second.append(id);
 				}
			}
		}			
	}
	if(m_floor_ents.size() <= 0)
		return false;
	return true;
}
void CHouseToFloor::GetFSInfo()
{
	int nCount = GetXDataCount(m_ent , _T("FSINFO"));
	for (int i = 0;i < nCount;i++)
	{
		TCHAR szFSINFO[500] = {0};
		READMulXData(aname(m_ent) , _T("FSINFO") , szFSINFO , STRING_TYPE , i);
		CString strFSINFO;
		strFSINFO.Format(_T("%s") , szFSINFO);
		vector<CString> saDatas;
		AnalyzeData(saDatas , strFSINFO , _T(";"));
		if (saDatas.size() == 2)
		{
			vector<CString> floorInfo;
			AnalyzeData(floorInfo , saDatas[1] , _T(","));
			m_mapRoom_Floor.insert(pair<CString , vector<CString> >(saDatas[0] , floorInfo));
		}
	}
}

bool CHouseToFloor::GetRoomFSName(CString strFSFloor , CString strRoom , CString &strFloorName)
{
	int nCount = GetXDataCount(m_ent , _T("FSINFO"));
	map<CString , vector<CString> > mapRoom_Floor;
	for (int i = 0;i < nCount;i++)
	{
		TCHAR szFSINFO[500] = {0};
		READMulXData(aname(m_ent) , _T("FSINFO") , szFSINFO , STRING_TYPE , i);
		CString strFSINFO;
		strFSINFO.Format(_T("%s") , szFSINFO);
		vector<CString> saDatas;
		AnalyzeData(saDatas , strFSINFO , _T(";"));
		if (saDatas.size() == 2)
		{
			vector<CString> floorInfo;
			AnalyzeData(floorInfo , saDatas[1] , _T(","));
			mapRoom_Floor.insert(pair<CString , vector<CString> >(saDatas[0] , floorInfo));
		}
	}
	map<CString , vector<CString> >::iterator itMap = mapRoom_Floor.find(strRoom);
	if (itMap == mapRoom_Floor.end())
	{
		return false;
	}

	for (int i = 0;i < itMap->second.size();i++)
	{
		CString strTemp = itMap->second[i];

		if (strTemp.Find(strFSFloor) != -1 && strFSFloor.Find(_T(".1")) != -1)
		{
			//acutPrintf(_T("\n%s\n") , strTemp);
			strFloorName = strTemp.Right(strTemp.GetLength() - strTemp.Find(_T("_")) - 1);
			return true;
		}
	}
 	return false;
}
//分层
bool CHouseToFloor::GetZRZInfomation_FC()
{
	m_floor_ents.clear();
	m_room_list.clear();
	AcGePoint2dArray node;
	GetPlList(m_ent , node);
	if(node.length() < 3)
		return false;
// 	if(false ==m_family.get_zrz_infomation(m_room_list))
// 	{
// 		acutPrintf(_T("\n获得自然幢户室信息失败"));
// 		return false;
// 	}
	GetFSInfo();
	//获得自然幢内部的所有房屋
	ads_name ssName,ent;
	struct resbuf * rc = acutBuildList(RTDXF0 , _T("*POLYLINE"),0);
	bool bSearch = ssFromNodes(node , ssName , _T("WP"),1.0, false , rc );
	acutRelRb(rc);
	if(bSearch == false)
	{
		ads_ssfree(ssName);
		acutPrintf(_T("\n选择房屋失败"));
		return false;
	}
	long nLen = 0L;
	AcDbObjectIdArray idList;
	SSToIdArr(ssName , idList);
	nLen = idList.length();
	//这里旋转实体
	if(fabs(m_rotate_angle) > 0.001)
	{
		RotateEntitysByTrend(idList , m_rotate_angle , m_ptMin);//这里旋转所有实体
		GetEntsExtent(idList , m_ptMin , m_ptMax , 5);
	}
	std::map<int , AcDbObjectIdArray>::iterator iter;
	AGrid_ClearSheet(_T("获得房屋信息失败"));
	for (int i = 0 ; i < nLen; i++)
	{
		AcDbObjectId id = idList.at( i );
		acdbGetAdsName(ent , id);
		if(ads_name_equal(ent , m_ent))
			continue;
		int up = 0 , down = 0;
		FloorType type = UpAndDown;//默认是地上地下
		CString strError;
		if(false == GetBuildingFloorInformation(ent ,type ,up , down , strError))
		{
			AGrid_AppendMsgEnt(_T("获得房屋信息失败") , strError , strError , ent);
			continue;
		}
		if(type == UpAndDown)
		{
			for (int j = 1 ; j <= up ; j++)//地上层数
			{
				iter = m_floor_ents.find(j);
				if(iter == m_floor_ents.end())
				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
				}
				else
				{
					iter->second.append(id);
				}
			}
			if(down > 0)
			{
				for (int j = -1 * down ; j < 0 ; j++)
				{
					iter = m_floor_ents.find(j);
					if(iter == m_floor_ents.end())
					{
						AcDbObjectIdArray ids;
						ids.append(id);
						m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
					}
					else
					{
						iter->second.append(id);
					}
				}
			}	
		}
		else //第几层到第几层
		{
			for (int j = up ; j <= down ; j++)
			{
				iter = m_floor_ents.find(j);
				if(iter == m_floor_ents.end())
				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
				}
				else
				{
					iter->second.append(id);
				}
			}
		}			
	}
//	if(m_floor_ents.size() <= 0 || m_room_list.size() <= 0)
//		return false;
	return true;
}

bool CHouseToFloor::GetZRZInfomation()
{
	m_floor_ents.clear();
	m_room_list.clear();
	AcGePoint2dArray node;
	//GetPlList(m_ent , node);
	GetEntExtent(m_ent, node) ;
	AcGeDoubleArray DbArray;
	GetPlBulges(m_ent , DbArray);
	if(node.length() < 3)
		return false;
	if(false ==m_family.get_zrz_infomation(m_room_list))
	{
		acutPrintf(_T("\n获得自然幢户室信息失败"));
		return false;
	}
	GetFSInfo();
	//获得自然幢内部的所有房屋
	ads_name ssName,ent;
	struct resbuf * rc = acutBuildList(RTDXF0 , _T("*POLYLINE"),0);
	
	bool bSearch = ssFromNodes(node , ssName , _T("WP"),1.0, false , rc );
	acutRelRb(rc);
	if(bSearch == false)
	{
		ads_ssfree(ssName);
		acutPrintf(_T("\n选择房屋失败"));
		return false;
	}
	long nLen = 0L;
	AcDbObjectIdArray idList;
	SSToIdArr(ssName , idList);
	nLen = idList.length();
	//这里旋转实体
	if(fabs(m_rotate_angle) > 0.001)
	{
		RotateEntitysByTrend(idList , m_rotate_angle , m_ptMin);//这里旋转所有实体
		GetEntsExtent(idList , m_ptMin , m_ptMax , 5);
	}
	std::map<int , AcDbObjectIdArray>::iterator iter;
	AGrid_ClearSheet(_T("获得房屋信息失败"));
	for (int i = 0 ; i < nLen; i++)
	{
		AcDbObjectId id = idList.at( i );
		acdbGetAdsName(ent , id);
		if(ads_name_equal(ent , m_ent))
			continue;

		int up = 0 , down = 0;
		FloorType type = UpAndDown;//默认是地上地下
		CString strError;
		if(false == GetBuildingFloorInformation(ent ,type ,up , down , strError))
		{
			AGrid_AppendMsgEnt(_T("获得房屋信息失败") , strError , strError , ent);
			continue;
		}
		if(type == UpAndDown)
		{
			for (int j = 1 ; j <= up ; j++)//地上层数
			{
				iter = m_floor_ents.find(j);
				if(iter == m_floor_ents.end())
				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
				}
				else
				{
					iter->second.append(id);
				}
			}
			if(down > 0)
			{
				for (int j = -1 * down ; j < 0 ; j++)
				{
					iter = m_floor_ents.find(j);
					if(iter == m_floor_ents.end())
					{
						AcDbObjectIdArray ids;
						ids.append(id);
						m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
					}
					else
					{
						iter->second.append(id);
					}
				}
			}	
		}
		else //第几层到第几层
		{
			for (int j = up ; j <= down ; j++)
			{
				iter = m_floor_ents.find(j);
				if(iter == m_floor_ents.end())
				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
				}
				else
				{
					iter->second.append(id);
				}
			}
		}			
	}
	if(m_floor_ents.size() <= 0 || m_room_list.size() <= 0)
		return false;
	return true;
}

bool CHouseToFloor::GetZRZInfomation_GD()
{
	m_floor_ents.clear();
	m_room_list.clear();
	

	AcGePoint2dArray node;
	//GetPlList(m_ent , node);
	GetEntExtent(m_ent, node) ;
	AcGeDoubleArray DbArray;
	GetPlBulges(m_ent , DbArray);
	if(node.length() < 3)
		return false;
	if(false ==m_family.get_zrz_infomation(m_room_list))
	{
		acutPrintf(_T("\n获得自然幢户室信息失败"));
		return false;
	}
	GetFSInfo();
	//获得房屋自然幢编组内所有房屋（除了自己，自己为编组ID）
	vector<AcDbObjectId> idArry;
	GetGroupEnts(aname(m_ent),idArry);
	//将当前实体加入
	AcDbObjectId objId_cur;
	acdbGetObjectId(objId_cur,m_ent);
	idArry.push_back(objId_cur);
	ads_name ssName,ent;
	long nLen = 0L;
	AcDbObjectIdArray idList;
	for (int ii=0;ii<idArry.size();ii++)
	{
		idList.append(idList.at(ii));
	}
	nLen = idList.length();
	//这里旋转实体
	if(fabs(m_rotate_angle) > 0.001)
	{
		RotateEntitysByTrend(idList , m_rotate_angle , m_ptMin);//这里旋转所有实体
		GetEntsExtent(idList , m_ptMin , m_ptMax , 5);
	}
	std::map<int , AcDbObjectIdArray>::iterator iter;
	AGrid_ClearSheet(_T("获得房屋信息失败"));
	for (int i = 0 ; i < nLen; i++)
	{
		AcDbObjectId id = idList.at( i );
		acdbGetAdsName(ent , id);
		int up = 0 , down = 0;
		FloorType type = UpAndDown;//默认是地上地下
		CString strError;
		if(false == GetBuildingFloorInformation(ent ,type ,up , down , strError))
		{
			AGrid_AppendMsgEnt(_T("获得房屋信息失败") , strError , strError , ent);
			continue;
		}
		if(type == UpAndDown)
		{
			for (int j = 1 ; j <= up ; j++)//地上层数
			{
				iter = m_floor_ents.find(j);
				if(iter == m_floor_ents.end())
				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
				}
				else
				{
					iter->second.append(id);
				}
			}
			if(down > 0)
			{
				for (int j = -1 * down ; j < 0 ; j++)
				{
					iter = m_floor_ents.find(j);
					if(iter == m_floor_ents.end())
					{
						AcDbObjectIdArray ids;
						ids.append(id);
						m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
					}
					else
					{
						iter->second.append(id);
					}
				}
			}	
		}
		else //第几层到第几层
		{
			for (int j = up ; j <= down ; j++)
			{
				iter = m_floor_ents.find(j);
				if(iter == m_floor_ents.end())
				{
					AcDbObjectIdArray ids;
					ids.append(id);
					m_floor_ents.insert(std::pair<int , AcDbObjectIdArray>(j , ids));
				}
				else
				{
					iter->second.append(id);
				}
			}
		}			
	}
	if(m_floor_ents.size() <= 0 || m_room_list.size() <= 0)
		return false;
	return true;
}

//按照当前的走向旋转实体
void CHouseToFloor::RotateEntitysByTrend(AcDbObjectIdArray& idArray , double dAngle , ads_point ptBase)
{
	for (int i = 0 ; i < idArray.length() ; i++)
	{
		ads_name ent;
		acdbGetAdsName(ent , idArray.at( i ));
		RotateEntity(ent , dAngle , AcGePoint3d(ptBase[X] , ptBase[Y] , 0));
	}
}
bool CHouseToFloor::SaveFamilyDwg()
{
	TCHAR szExtendCode[50] = {0} , szExtendLayer[50] = {0};
	_stprintf(szExtendCode , _T("%s") , g_room_code);
	_stprintf(szExtendLayer , _T("%s") , g_room_layer);
	ads_point ptBase = {0};//排列的基准点
	ptBase[X] = m_ptMax[X] + 10;
	ptBase[Y] = m_ptMin[Y];
	double dWidth = m_ptMax[X] - m_ptMin[X] , dHeight = m_ptMax[Y] - m_ptMin[Y];
	if(dWidth < 10) dWidth = 10;
	if(dHeight < 10) dHeight = 10;
	if(m_floor_ents.size() <= 0 || m_room_list.size() <= 0)
		return false;
	for (int i = 0 ; i < m_room_list.size(); i++)//遍历所有的户
	{
		FaimilyStuct item = m_room_list.at( i );
		TCHAR szNum[60] = {0};
		_stprintf(szNum , _T("%s"),item.strCode);
		//获得户的层数
		std::map<CString , double>::iterator iter = item.floorList.begin();
		ads_polar(ptBase ,-PI / 2.0 , dHeight , ptBase);
		CString strSZC = _T("");//所在层数
		int j = 0;
		AcDbObjectIdArray idFamily;//一个户的所有的实体
		int nPreFloor = -1;
		for (; iter != item.floorList.end(); iter++)//一户的信息
		{
			ads_point pt;
			ads_point_set(ptBase , pt);
			CString str = iter->first;
			if(str.IsEmpty())
				continue;
			if (str.Find(_T(".")) == -1)
			{
				int nFloor = _ttoi(str.GetBuffer());
				str.ReleaseBuffer();
				//获得楼层的范围线
				AcDbObjectIdArray idFloorList;
				AcDbObjectIdArray idFloorList_Pre;//前一层
				GetExtendLineByFloorNum(nFloor ,idFloorList , false);
				if (nPreFloor != -1)
				{
					GetExtendLineByFloorNum(nPreFloor ,idFloorList_Pre , false);
				}
				if(idFloorList.length() <= 0)
					continue;
				//增加楼层注记
				TCHAR szText[100] = {0};
				_stprintf(szText , _T("%d层"),nFloor);
				ads_point ptTextMax = {0}, ptTextMin = {0};
				if (nPreFloor != -1)
				{
					GetEntsExtent(idFloorList_Pre , ptTextMin , ptTextMax );
				}
				else
				{
					GetEntsExtent(idFloorList , ptTextMin , ptTextMax );
				}


				double dDWidth = ptTextMax[X] - ptTextMin[X];
				dDWidth += 20;
				ads_point ptText = {( ptTextMin[X] + ptTextMax[X] ) * 0.5 , ptTextMin[Y] - 2*m_dH , 0};
				AcDbObjectId idText = DrawText(ptText , szText , _T("tc"),szExtendCode , 1*m_dH);
				setlayer(aname(idText) , szExtendLayer);
				idFloorList.append(idText);

				double dStep = dWidth; //修改宽度 因为旋转之后可能导致跌该
				if(dStep < dDWidth)
					dStep = dDWidth;

				ads_polar(ptBase , 0 , dStep * j++, pt);
				double xMove = pt[X] - m_ptMax[X] , yMove = pt[Y] - m_ptMax[Y];
				for (int k = 0 ; k < idFloorList.length(); k++)
				{
					ads_name entT;
					acdbGetAdsName(entT , idFloorList.at( k ));
					MoveEntity(entT , xMove , yMove);
				}			
				idFamily.append(idFloorList);
				CString strFloor;
				if(strSZC.IsEmpty())
				{
					strFloor.Format(_T("%d"),nFloor);
				}
				else
				{
					strFloor.Format(_T(",%d"),nFloor);
				}
				strSZC += strFloor;
				nPreFloor = nFloor;
			}
			else
			{
				double dFloor = _tstof(str.GetBuffer());
				str.ReleaseBuffer();
				//获得楼层的范围线
				AcDbObjectIdArray idFloorList;
				GetExtendLineByFloorNum_FSC(dFloor ,idFloorList , false);
				if(idFloorList.length() <= 0)
					continue;
				//增加楼层注记
				TCHAR szText[100] = {0};
				_stprintf(szText , _T("%.1f层"),dFloor);

				ads_point ptTextMax = {0}, ptTextMin = {0};
				GetEntsExtent(idFloorList , ptTextMin , ptTextMax );
				double dDWidth = ptTextMax[X] - ptTextMin[X];
				dDWidth += 20;
				ads_point ptText = {( ptTextMin[X] + ptTextMax[X] ) * 0.5 , ptTextMin[Y] - 2*m_dH , 0};
				AcDbObjectId idText = DrawText(ptText , szText , _T("tc"),szExtendCode , 1*m_dH );
				setlayer(aname(idText) , szExtendLayer);
				//idFloorList.append(idText);
				CString strFloorName;
				CString strTemp;
				strTemp.Format(_T("%.1f") , dFloor);
				if (GetRoomFSName(strTemp , CString(szNum) , strFloorName))
				{
					AcDbObjectId idTextNew =DrawText(ptText , strFloorName , _T("tc"),szExtendCode , 1*m_dH);
					idFloorList.append(idTextNew);
				}

				double dStep = dWidth; //修改宽度 因为旋转之后可能导致跌该
				if(dStep < dDWidth)
					dStep = dDWidth;

				ads_polar(ptBase , 0 , dStep * j++, pt);
				double xMove = pt[X] - m_ptMax[X] , yMove = pt[Y] - m_ptMax[Y];
				for (int k = 0 ; k < idFloorList.length(); k++)
				{
					ads_name entT;
					acdbGetAdsName(entT , idFloorList.at( k ));
					MoveEntity(entT , xMove , yMove);
				}			
				idFamily.append(idFloorList);
				CString strFloor;
				if(strSZC.IsEmpty())
				{
					strFloor.Format(_T("%.1f"),dFloor);
				}
				else
				{
					strFloor.Format(_T(",%.1f"),dFloor);
				}
				strSZC += strFloor;

				nPreFloor = dFloor;
			}

		}
		//获得整个户的范围
		ads_point ptMin , ptMax;
		GetEntsExtent(idFamily ,ptMin , ptMax , 5);
		AcGePoint2dArray node;
		node.append(AcGePoint2d(ptMin[X] , ptMin[Y]));
		node.append(AcGePoint2d(ptMax[X] , ptMin[Y]));
		node.append(AcGePoint2d(ptMax[X] , ptMax[Y]));
		node.append(AcGePoint2d(ptMin[X] , ptMax[Y]));
		
		AcDbObjectId idExtend = DrawPolyline2D(node , 0 , true ,szExtendCode , 0 , 0 , szExtendLayer);
		
		//设置BULGE
		//SetPolylineBulge(ads_name ent, AcGeDoubleArray &bulgeArr);
		//LWpoly->setBulgeAt(i, daBugle[i]);

		//继承自然幢的属性
		CopyXdata(szNum , aname(idExtend));
		//获得幢GUID
		TCHAR szZRZGUID[60] = {0};
		ReadXdata(aname(idExtend) , _T("GUID"),0,szZRZGUID);
		//添加户室范围线GUID
		CString strGuid;
		TCHAR szGUID_Room[100] = {0};
		READMulXData(aname(m_ent) , _T("ID_ROOM") , szGUID_Room , STRING_TYPE , i); 
		strGuid.Format(_T("%s") , szGUID_Room);

		AddXdata(aname(idExtend), _T("GUID"),0 , strGuid);
		TCHAR szFWJG[100] = {0};
		ReadXdata(m_ent , _T("FWJG") , STRING_TYPE , szFWJG);
		AddXdata(aname(idExtend) , _T("FWJG") , STRING_TYPE , CString(szFWJG));
		//自然幢的GUID
		AddXdata(aname(idExtend), _T("ZRZGUID"),0 , szZRZGUID);
		//写入所在层
		AddXdata(aname(idExtend), _T("SZC"), 0 ,strSZC);
		AddXdata(aname(idExtend) , _T("YWZH") , 0 , item.strYWZH);
		TCHAR szZL[100] = {0};
		ReadXdata(m_ent , _T("fwzl") , STRING_TYPE , szZL);
		AddXdata(aname(idExtend) , _T("FDZL") , STRING_TYPE , CString(szZL));
		//写入权利人信息
		m_righter_helper.BeginTransform(aname(idExtend));

		m_all_save_ents.append(idExtend);
		m_all_save_ents.append(idFamily);
		ptBase[Y] = ptMin[Y];
	}
	//保存到dwg 

	return SaveToDwg();
}

bool CHouseToFloor::CompareIdArr(AcDbObjectIdArray& idsFirst , AcDbObjectIdArray& idsSecond)
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

void CHouseToFloor::NoteLayer(AcDbObjectIdArray idArr , int nIdexLayer)
{
	ads_point ptMin , ptMax , ptMid;
	GetEntsExtent(idArr , ptMin , ptMax);
	ptMid[Y] = ptMin[Y];
	ptMid[X] = (ptMax[X] + ptMin[X])/2;
	ads_polar(ptMid , -PI/2 , 2 * m_dH , ptMid);
	CString strTextValue;
	strTextValue += _T("第");
	for (int nIndexLayerName = 0;nIndexLayerName < m_LayerName.at(nIdexLayer).size();nIndexLayerName++ )
	{
		CString strTemp;
		strTemp.Format(_T("%d") , m_LayerName.at(nIdexLayer).at(nIndexLayerName));
		strTextValue += strTemp;
		if (nIndexLayerName != (m_LayerName.at(nIdexLayer).size() - 1))
		{
			strTextValue += _T(",");
		}
	}
	strTextValue += _T("层");
	AcDbObjectId idTemp = DrawText(ptMid , strTextValue , _T("MC") , _T("") , 1*m_dH);
	m_all_save_ents.append(idTemp);
}

void CHouseToFloor::NoteJZXDist(AcDbObjectIdArray idArrPoly )
{
	AcDbObjectId idText;
	AcDbObjectIdArray idArrText;
	for (int nIndexPoly = 0;nIndexPoly < idArrPoly.length();nIndexPoly++)
	{
		int nPolyType = GetPolyType(aname(idArrPoly.at(nIndexPoly)),true);
		if ( nPolyType < 0 ) 
		{
			continue; 
		}
		else if ( nPolyType < 10 ) 
		{
			continue;; 
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
			TCHAR szDist[100] = {0};
			_stprintf(szDist , _T("%.2f") , dDist);
			double dAngle = GetAngle(apoint(nodes.at(nIndexPt - 1)) , apoint(nodes.at(nIndexPt)));
			ads_point ptMid;
			ptMid[X] = (nodes.at(nIndexPt).x + nodes.at(nIndexPt - 1).x)/2;
			ptMid[Y] = (nodes.at(nIndexPt).y + nodes.at(nIndexPt - 1).y)/2;
			ads_polar(ptMid , PI/2 + dAngle , 1*m_dH , ptMid);
			if (dAngle > (PI - PI/18) && dAngle < (PI + PI/18))
			{
				idText = DrawText(ptMid , szDist , _T("MC") , _T("") , 0.7*m_dH , PI + dAngle);
				idArrText.append(idText);
			}
			else if (dAngle > (PI/2 - PI/9) && dAngle < (PI/2 + PI/9))
			{
				idText = DrawText(ptMid , szDist , _T("MC") , _T("") , 0.7*m_dH , PI + dAngle);
				idArrText.append(idText);
			}
			else
			{

				idText = DrawText(ptMid , szDist , _T("MC") , _T("") , 0.7*m_dH , dAngle);
				idArrText.append(idText);
			}
		}
	}
	m_all_save_ents.append(idArrText);
}

void CHouseToFloor::NoteJG(AcDbObjectIdArray idArrPoly )
{
	AcDbObjectIdArray idArrText;
	for (int nIndexPoly = 0;nIndexPoly < idArrPoly.length();nIndexPoly++)
	{
		AcGePoint2dArray nodes;
		ads_point ptMid;
		ads_name ent;
		acdbGetAdsName(ent , idArrPoly.at(nIndexPoly));
		GetPlList(ent , nodes );
		GetNodesMiddlePoint(nodes , ptMid);
		resbuf *cmd = NULL , *result = NULL;
		cmd = ads_buildlist(RTSTR , _T("ReadEntInfo_invoke") , RTENAME , ent , RTSTR , _T("JG") , RTLONG , 80 ,RTNONE);
		ads_invoke(cmd,&result);
		ads_relrb(cmd);
		CString strValue;
		if (result != NULL) 
		{
			if ( RTSTR == result->restype ) 
			{
				//_tcscpy(result->resval.rstring,strValue);
				strValue.Format(_T("%s") , result->resval.rstring);
				ads_relrb(result);
			}
			else 
			{
				ads_relrb(result);
				return;
			}
		}
		AcDbObjectId id  = DrawText(ptMid , strValue , _T("MC") , _T("") , 1*m_dH);
		idArrText.append(id);

		ads_polar(ptMid , -PI/2 , 2 , ptMid);
		//TCHAR szValue[100] = {0};
		//ReadXdata(aname(id) , _T("FWBH") , STRING_TYPE , szValue);
		//strValue.Format(_T("%s") , szValue);
		//id = DrawText(ptMid , strValue , _T("MC") , _T("") , 2);
		idArrText.append(id);

	}
	m_all_save_ents.append(idArrText);
}


bool CHouseToFloor::SaveClassLayer()
{

	double dH = 0.5;
	read1(dH);
	dH /= 1000.0;
	m_dH = dH;
	TCHAR szExtendCode[50] = {0} , szExtendLayer[50] = {0};
	_stprintf(szExtendCode , _T("%s") , g_room_code);
	_stprintf(szExtendLayer , _T("%s") , g_room_layer);
	ads_point ptBase = {0};//排列的基准点
	ptBase[X] = m_ptMax[X] + 10;
	ptBase[Y] = m_ptMin[Y];
	double dWidth = m_ptMax[X] - m_ptMin[X] , dHeight = (m_ptMax[Y] - m_ptMin[Y]);
	if(dWidth < 10) dWidth = 10;
	if(dHeight < 10) dHeight = 10;
	if(m_floor_ents.size() <= 0)
		return false;
	AcDbObjectIdArray idArrFloor_Pre;
	int j = 0;

	map<vector<int> , AcDbObjectIdArray> mapFloorInfo;
	m_all_save_ents.setLogicalLength(0);
	m_LayerName.clear();
	m_veIdArrLayer.clear();
	for (std::map<int , AcDbObjectIdArray>::iterator it = m_floor_ents.begin() ; it != m_floor_ents.end(); it++)//遍历所有的户
	{
		AcDbObjectIdArray idArrFloor_Cur;
		idArrFloor_Cur.append(it->second);
		AcDbObjectIdArray idCopy;
		if (idArrFloor_Pre.length() == 0)
		{
			CopyEntitys(idArrFloor_Cur , idCopy);
			m_veIdArrLayer.push_back(idCopy);
			ads_point pt;
			ads_point_set(ptBase , pt);
			ads_polar(ptBase , -PI/2 , dHeight * j++, pt);
			double xMove = pt[X] - m_ptMax[X] , yMove = pt[Y] - m_ptMax[Y];
			for (int k = 0 ; k < idCopy.length(); k++)
			{
				ads_name entT;
				acdbGetAdsName(entT , idCopy.at( k ));
				MoveEntity(entT , xMove , yMove);
			}
			vector<int> veNFlool;
			veNFlool.push_back(it->first);
			m_LayerName.push_back(veNFlool);
		}
		else
		{
// 			if (CompareIdArr(idArrFloor_Cur , idArrFloor_Pre))
// 			{
// 				m_LayerName[m_LayerName.size() - 1].push_back(it->first); 
// 			}
// 			else
// 			{
				CopyEntitys(idArrFloor_Cur , idCopy);
				m_veIdArrLayer.push_back(idCopy);
				vector<int> veNFlool;
				veNFlool.push_back(it->first);
				m_LayerName.push_back(veNFlool);
				ads_point pt;
				ads_point_set(ptBase , pt);
				ads_polar(ptBase , -PI/2 , dHeight * j++, pt);
				double xMove = pt[X] - m_ptMax[X] , yMove = pt[Y] - m_ptMax[Y];
				for (int k = 0 ; k < idCopy.length(); k++)
				{
					ads_name entT;
					acdbGetAdsName(entT , idCopy.at( k ));
					MoveEntity(entT , xMove , yMove);
				}
//			}
		}
		m_all_save_ents.append(idCopy);
		idArrFloor_Pre.setLogicalLength(0);
		idArrFloor_Pre.append(idArrFloor_Cur);
	}

	for (int i = 0;i < m_veIdArrLayer.size();i++)
	{
		NoteLayer(m_veIdArrLayer.at(i) , i);
		NoteJZXDist(m_veIdArrLayer.at(i));
		NoteJG(m_veIdArrLayer.at(i));
	}
	ads_point ptMin , ptMax;
	GetEntsExtent(	m_all_save_ents ,ptMin , ptMax , 5*dH);
	ads_point ptMax_NoOff , ptMin_Nooff , ptInsertNorth;
	GetEntsExtent(m_all_save_ents ,ptMin_Nooff , ptMax_NoOff);
	AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X] , ptMin[Y]));
	node.append(AcGePoint2d(ptMax[X] , ptMin[Y]));
	node.append(AcGePoint2d(ptMax[X] , ptMax[Y]));
	node.append(AcGePoint2d(ptMin[X] , ptMax[Y]));
	AcDbObjectId idExtend = DrawPolyline2D(node , 0 , true ,szExtendCode , 0 , 0 , szExtendLayer);
	m_all_save_ents.append(idExtend);
	ads_point ptTopMid;
	ptTopMid[X] = (ptMin[X] + ptMax[X])/2;
	ptTopMid[Y] = ptMax[Y];

	ads_polar(ptTopMid , PI/2 , 1.2*dH , ptTopMid);
	CString strTitle;
	strTitle.Format(_T("%s分层草图"), m_strDCBM);
	AcDbObjectId idTitle = DrawText(ptTopMid , strTitle , _T("M") , _T("") , 1.6*dH);
	m_all_save_ents.append(idTitle);

	ptInsertNorth[X] = (ptMax[X] + ptMax_NoOff[X])/2;
	ptInsertNorth[Y] = (ptMax[Y] + ptMax_NoOff[Y])/2;
// 	AcGePoint2dArray nodes;
// 	nodes.append(apoint(ptMax_NoOff));
// 	nodes.append(apoint(ptMax));
// 	GetNodesMiddlePoint(nodes , ptInsertNorth);
	CXRecordOperate xRecord;
	double dAngle = 0.0;
	xRecord.ReadDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),dAngle);
	AcDbObjectId idNorth = DrawInsert(apoint(ptInsertNorth) , _T("north2"));
	ScaleEntity(aname(idNorth) , apoint(ptInsertNorth) , 0.15);
	m_all_save_ents.append(idNorth);
	RotateEntity(aname(idNorth) , PI/2 - dAngle , apoint(ptInsertNorth));
	
	//保存到dwg 
	if(SaveToDwg(false))
	{
		return true;
	}
	return false;
}


bool CHouseToFloor::GetHousePlan(vector<int> veFLoor , AcDbObjectIdArray & idArrZHH)
{
	TCHAR szExtendCode[50] = {0} , szExtendLayer[50] = {0};
	_stprintf(szExtendCode , _T("%s") , g_room_code);
	_stprintf(szExtendLayer , _T("%s") , g_room_layer);
	ads_point ptBase = {0};//排列的基准点
	ptBase[X] = m_ptMax[X] + 10;
	ptBase[Y] = m_ptMin[Y];
	double dWidth = m_ptMax[X] - m_ptMin[X] , dHeight = m_ptMax[Y] - m_ptMin[Y];
	if(dWidth < 10) dWidth = 10;
	if(dHeight < 10) dHeight = 10;
	if(m_floor_ents.size() <= 0 || m_room_list.size() <= 0)
		return false;
	AcDbObjectIdArray idArrFloor_Pre;
	int j = 0;

	for (int nFloorIndex = 0;nFloorIndex < veFLoor.size();nFloorIndex++)
	{
		std::map<int , AcDbObjectIdArray>::iterator it = m_floor_ents.find(veFLoor[nFloorIndex]);
		if (it != m_floor_ents.end())
		{
			idArrZHH.append(it->second);
		}
	}
	RemoveSameObjects(idArrZHH);
	return false;
}



bool CHouseToFloor::CopyXdata(TCHAR* strHuNum , ads_name entHu)
{
	TCHAR szCode[50] = {0};
	GetStdm(entHu , szCode);
	AcDbEntity *pZrz = NULL , *pHu = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pZrz , aname(m_ent) , AcDb::kForRead);
	if(es != Acad::eOk)
		return false;
	es = acdbOpenAcDbEntity(pHu , aname(entHu) , AcDb::kForWrite);
	if(es != Acad::eOk)
	{
		pZrz->close();
		return false;
	}
	resbuf* res = pZrz->xData();
	if(res != NULL)
	{
		pHu->setXData(res);
	}
	acutRelRb(res);
	res = NULL;
	pHu->close();
	pZrz->close();
	//修改cass编码
	addapp(entHu , szCode);
	//修改FamilyRoomList
	int nCount = GetXDataCount(entHu , _T("FamilyRoomList"));
	TCHAR szText[1000] = {0};
	for (int i = 0 ; i < nCount; i++)
	{
		TCHAR szValue[1000] = {0};
		READMulXData(aname(entHu) , _T("FamilyRoomList"),szValue , 0 , i);
		if(_tcsnicmp(szValue , strHuNum , _tcslen(strHuNum)) == 0)
		{
			vector<CString> saDatas;
			AnalyzeData(saDatas ,CString(szValue) , _T(";"));
			if(saDatas.size() >= 2)
			{
				CDatabaseOperManager dataOper;
				dataOper.initialConnect();

				vector<CString> saDatas_QLR;
				CString strQLRMC;
				AnalyzeData(saDatas_QLR ,saDatas[1] , _T(","));
				for (int nQLRIndex = 0;nQLRIndex < saDatas_QLR.size();nQLRIndex++)
				{
					CString strTemp;
					strTemp = dataOper.GetQLRMC(saDatas_QLR[nQLRIndex]);
					strQLRMC += strTemp;
					if (nQLRIndex != (saDatas_QLR.size() - 1))
					{
						strQLRMC += _T(",");
					}
				}

				
				AddXdata(entHu , _T("FWSYQR") , STRING_TYPE , strQLRMC);


				TCHAR szBL_Total[100] = {0};
				ReadXdata(entHu , _T("GYFTBL_Total") , STRING_TYPE , szBL_Total);
				TCHAR szBL[100] = {0};
				ReadXdata(entHu , _T("GYFTBL") , STRING_TYPE , szBL);
				TCHAR szBL_FT[100] = {0};
				ReadXdata(entHu , _T("GYFTBL_FTQY") , STRING_TYPE , szBL_FT);

				vector<CString> saDatas_Total;
				AnalyzeData(saDatas_Total , CString(szBL_Total) , _T(";"));
				vector<CString> saDatas_BL;
				AnalyzeData(saDatas_BL , CString(szBL) , _T(";"));
				vector<CString> saDatas_FT;
				AnalyzeData(saDatas_FT , CString(szBL_FT) , _T(";"));
				if (saDatas_Total.size() > 0)
				{
					for (int nIndex = 0;nIndex < saDatas_Total.size();nIndex++)
					{
						vector<CString> saDatas1;
						vector<CString> saDatas2;
						vector<CString> saDatas3;
						AnalyzeData(saDatas1 , saDatas_Total[nIndex] , _T(","));
						AnalyzeData(saDatas2 , saDatas_BL[nIndex] , _T(","));
						AnalyzeData(saDatas3 , saDatas_FT[nIndex] , _T(","));
						if(2 >= saDatas1.size())
						{
							if (0 == _tcscmp(strQLRMC , saDatas1[0]))
							{
								AddXdata(entHu , _T("GYFTBL_Total") , STRING_TYPE , saDatas1[1]);
								AddXdata(entHu , _T("GYFTBL") , STRING_TYPE , saDatas2[1]);
								AddXdata(entHu , _T("GYFTBL_FTQY") , STRING_TYPE , saDatas3[1]);
							}
						}
					}
				}

			}
			_tcscpy(szText , szValue);
			break;
		}

	}
	//写入HH户室编号
	AddXdata(entHu , _T("HH"),0 ,strHuNum);
	//户的不动产单元号
	TCHAR szZRZBDCDYH[50] = {0};
	ReadXdata(entHu , _T("BDCDYH"),0,szZRZBDCDYH);
	if(_tcslen(szZRZBDCDYH) > 24)
	{
		CString str = szZRZBDCDYH;
		str = str.Left( 24 );
		str += strHuNum;
		AddXdata(entHu , _T("BDCDYH"),0,str);
	}
	RemoveXdata(entHu , _T("FSInfo"));
	map<CString , vector<CString> >::iterator itMapFS = m_mapRoom_Floor.find(CString(strHuNum));
	if (itMapFS != m_mapRoom_Floor.end())
	{
		CString strFSInfo;
		for(int nFSIndex = 0;nFSIndex < itMapFS->second.size();nFSIndex++ )
		{
			strFSInfo += itMapFS->second[nFSIndex];
			strFSInfo += _T(",");
		}
		CString strTemp;
		strTemp.Format(_T("%s;%s") , strHuNum , strFSInfo);
		AddXdata(entHu , _T("FSINFO") , STRING_TYPE , strTemp);
	}

	RemoveXdata(entHu , _T("FamilyRoomList"));
	if(_tcslen(szText) > 0)
	{
		AddXdata(entHu , _T("FamilyRoomList"),0 , szText);
		return true;
	}	
	return false;
}
//这里保存的时候需要将每一个实体炸开为一段一段的线
bool CHouseToFloor::SaveToDwg(bool bExplode /* = true*/)
{
	if(m_all_save_ents.length() <= 0)
		return false;
	if(_tcslen(m_dwg_name) <= 0)
		return false;
	ads_name ssName;
	ads_ssadd(NULL , NULL ,ssName);
	CLineTextHelper lineLabelHelper;
	for (int i = 0 ; i < m_all_save_ents.length(); i++)
	{
		ads_name ent;
		acdbGetAdsName(ent , m_all_save_ents.at( i ));
		TCHAR szCode[50] = {0};
		GetStdm(ent , szCode);
		if(g_room_code.CompareNoCase(szCode) != 0)//不是户室范围线
		{
			if(bExplode)
			{
				AcDbObjectIdArray idExplodes;
				ExplodeEnt(ent , idExplodes , true);
				for (int j = 0 ; j < idExplodes.length(); j++)
				{
					ads_name entTemp;
					acdbGetAdsName(entTemp , idExplodes.at( j ));
					AddXdata(entTemp , _T("south") , 0 , g_room_line_code);
					////增加边长注记
					//if(lineLabelHelper.beginLabel(entTemp))
					//{
					//	ads_ssadd(lineLabelHelper.m_text_ent , ssName , ssName);
					//}
					ads_ssadd(entTemp , ssName , ssName);
				}
			}
			else
			{
				ads_ssadd(ent , ssName , ssName);
			}
		}
		else
		{
			ads_ssadd(ent , ssName , ssName);
		}
	}
	long nLen = 0L;
	ads_sslength(ssName , &nLen);
	if(nLen > 0)
	{
		TCHAR szPath[MAX_PATH] = {0};
		ads_point pt;
		pt[X] = pt[Y] = pt[Z] = 0.0;
		int commandRes = 0;
		if (ads_findfile(m_dwg_name,szPath) == RTNORM)
			commandRes = ads_command(RTSTR, _T("wblock"), RTSTR, m_dwg_name, RTSTR, _T("Y"), RTSTR, _T(""), RTPOINT, pt, RTPICKS, ssName, RTSTR, _T(""), RTNONE);
		else
			commandRes = ads_command(RTSTR, _T("wblock"), RTSTR, m_dwg_name, RTSTR, _T(""), RTPOINT, pt, RTPICKS, ssName, RTSTR, _T(""), RTNONE);
		ads_ssfree(ssName);
		
		//写入标记
		AcDbDatabase* pExternalDb = new AcDbDatabase(false);
		Acad::ErrorStatus es = pExternalDb->readDwgFile(m_dwg_name);
		if(es != Acad::eOk)
		{
			acutPrintf(_T("\n写入标记失败"));
			return false;
		}
		CXRecordOperate xRecord;
		xRecord.AddStringRecord(_T("DWGTYPE"),_T("DWG"),_T("floordwg"),pExternalDb);//写入标记 表示是户室图
		xRecord.AddDoubleRecord(_T("HOUSEROTATE"),_T("HOUSEROTATE"),m_rotate_angle,pExternalDb);//户旋转角度
		TCHAR szDatabaseName[200] = {0};
		_stprintf(szDatabaseName , _T("%s") , m_current_qlr_database_name);
	    xRecord.AddStringRecord(_T("PROPERTYDATABASE"),_T("PROPERTYDATABASE"),szDatabaseName ,pExternalDb);//当前的权利人数据库名称
		TCHAR szDatabaseName_Full[1000] = {0};
		//CXRecordOperate xRecord;
		_stprintf(szDatabaseName_Full , _T("%s") , m_current_qlr_database_full_name);
		xRecord.AddStringRecord(_T("PROPERTYDATABASE_Full"),_T("PROPERTYDATABASE_Full"),szDatabaseName_Full ,pExternalDb);//当前的权利人数据库名称
		


// 		AcDbLayerTable *pLayerTbl;
// 		pExternalDb->getLayerTable(pLayerTbl,AcDb::kForRead);
// 		AcDbLayerTableRecord *pLtTableRcd;
// 		if ( Acad::eOk != pLayerTbl->getAt(_T("room_externd_layer"),pLtTableRcd,AcDb::kForWrite) ) 
// 		{
// 			pLayerTbl->close(); return false;
// 		}
// 		pLayerTbl->close();
// 		//pLtTableRcd->setLinetypeObjectId()
// 		pLtTableRcd->setIsLocked(true);
// 		pLtTableRcd->close();


		es = pExternalDb->saveAs(m_dwg_name);
		if(es != Acad::eOk)
		{
			acutPrintf(_T("\n写入标记失败"));
			return false;
		}
		delete pExternalDb;
		pExternalDb = NULL;
        return true;
	}
	ads_ssfree(ssName);
	return false;
}
bool CHouseToFloor::GetExtendLineByFloorNum(int nFloor , AcDbObjectIdArray& ids , bool bCreateArea)
{
	ids.setLogicalLength(0);
	AcDbObjectIdArray idEnts;
	std::map<int , AcDbObjectIdArray>::iterator iter = m_floor_ents.find(nFloor);
	if(iter == m_floor_ents.end())
		return false;
	idEnts = iter->second;
	if(idEnts.length() <= 0)
		return false;
	CArray<AcGePoint2dArray , AcGePoint2dArray> nodesArray ;
	CArray<AcGeDoubleArray , AcGeDoubleArray> buglesOut;
	TCHAR szCode[50] = {0} , szLayer[50] = {0};
	_stprintf(szCode , _T("%s"),g_room_line_code);
	_stprintf(szLayer , _T("%d层"),nFloor);
	if(bCreateArea == false)
	{
		for (int i = 0 ; i < idEnts.length(); i++)
		{
			AcDbObjectId id = CopyEntity(idEnts.at( i ));
			AddXdata(aname(id) , _T("south"),0 , szCode);
			setlayer(aname(id) , szLayer);
			ids.append(id);
		}
	}
	else
	{
		if(CreateArea(idEnts ,nodesArray , buglesOut) == false)
		{
			for (int i = 0 ; i < idEnts.length(); i++)
			{
				AcDbObjectId id = CopyEntity(idEnts.at( i ));
				AddXdata(aname(id) , _T("south"),0 , szCode);
				setlayer(aname(id) , szLayer);
				ids.append(id);
			}
		}
		else
		{
			for (int i = 0 ; i < nodesArray.GetSize(); i++)
			{
				AcGePoint2dArray node = nodesArray.GetAt( i );
				if(node.length() > 0)
				{
					AcDbObjectId id = DrawPolyline2D(node , 0 , true , szCode , 0 ,0 ,szLayer);
					ids.append(id);
				}
			}
		}
	}	
	return true;
}

bool CHouseToFloor::GetExtendLineByFloorNum_FSC(double dFloor , AcDbObjectIdArray& ids , bool bCreateArea)
{
	ids.setLogicalLength(0);
	AcDbObjectIdArray idEnts;
	std::map<int , AcDbObjectIdArray>::iterator iter = m_floor_ents.find((int)dFloor);
	if(iter == m_floor_ents.end())
		return false;
	idEnts = iter->second;
	if(idEnts.length() <= 0)
		return false;
	CArray<AcGePoint2dArray , AcGePoint2dArray> nodesArray ;
	CArray<AcGeDoubleArray , AcGeDoubleArray> buglesOut;
	TCHAR szCode[50] = {0} , szLayer[50] = {0};
	_stprintf(szCode , _T("%s"),g_room_line_code);
	_stprintf(szLayer , _T("%.1f层"),dFloor);
	if(bCreateArea == false)
	{
		for (int i = 0 ; i < idEnts.length(); i++)
		{
			AcDbObjectId id = CopyEntity(idEnts.at( i ));
			AddXdata(aname(id) , _T("south"),0 , szCode);
			setlayer(aname(id) , szLayer);
			ids.append(id);
		}
	}
	else
	{
		if(CreateArea(idEnts ,nodesArray , buglesOut) == false)
		{
			for (int i = 0 ; i < idEnts.length(); i++)
			{
				AcDbObjectId id = CopyEntity(idEnts.at( i ));
				AddXdata(aname(id) , _T("south"),0 , szCode);
				setlayer(aname(id) , szLayer);
				ids.append(id);
			}
		}
		else
		{
			for (int i = 0 ; i < nodesArray.GetSize(); i++)
			{
				AcGePoint2dArray node = nodesArray.GetAt( i );
				if(node.length() > 0)
				{
					AcDbObjectId id = DrawPolyline2D(node , 0 , true , szCode , 0 ,0 ,szLayer);
					ids.append(id);
				}
			}
		}
	}	
	return true;
}