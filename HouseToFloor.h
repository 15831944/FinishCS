#ifndef _HOUSE_TO_FLOOR_
#define _HOUSE_TO_FLOOR_
#include "FamilyRoomManager.h"
#include "RighterForRoomHelper.h"
//幢到户室
class CHouseToFloor
{
public:
	CHouseToFloor(void);
	~CHouseToFloor(void);
	void SetEnt(ads_name ent);//设置自然幢
	bool BeginToClassfiy();
	bool SaveCurDwg(CString str_path);
	bool SaveCurDwg();
	bool BeginToClassfiy_TK();
	bool BeginToClassfiy_Export();
	bool BeginToHousePlane();
	bool BeginToHousePlane_GD();
	bool BeginToHouseFloor_Simple();
	CString GetImagePath();
	void GetFSInfo();
	bool GetRoomFSName(CString strFSFloor , CString strRoom , CString &strFloorName);
	bool GetHousePlan(vector<int> veFLoor , AcDbObjectIdArray & idArrZHH);
	vector<AcDbObjectIdArray> m_veIdArrLayer;
	ads_name m_ent;//自然幢
	double m_rotate_angle;//分层分户时 幢的旋转角度
	TCHAR m_dwg_name[MAX_PATH];//自然幢对应的分户图的保存全路径
	CString m_strDataBasePath;
	CString m_strDCBM;
	double m_dH;
private:
	CFamilyRoomManager m_family;
	
	TCHAR m_zrz_floder[MAX_PATH];//自然幢的文件夹路径
	
	CString m_dwg_guid;//分户图dwg名称
	std::map<int , AcDbObjectIdArray> m_floor_ents;//楼层和对应每层对应的实体
	std::vector<FaimilyStuct> m_room_list;//当前幢的户信息
	ads_point m_ptMax;//幢的最大点
	ads_point m_ptMin;//幢的最小点
	map<CString , vector<CString> > m_mapRoom_Floor;
	AcDbObjectIdArray m_all_save_ents;//需要写出的实体
	vector<vector<int> >  m_LayerName;//相同层的层名
	//double m_dH;
	CString m_strZHH;
	CString m_strZRZH;
	CString m_current_qlr_database_name;//当前的权利人数据库的名称
	CString m_current_qlr_database_full_name;
	TCHAR   m_szHandle[100];//自然撞的句柄
	CRighterForRoomHelper m_righter_helper;
private:
	//获得自然撞的分户信息 和 每一层对应的实体
	bool GetZRZInfomation();
	bool GetZRZInfomation_GD();  //去自然幢版本中使用
	bool GetZRZInfomation_FC();
	bool GetZRZInfomation_floor();
	bool SaveFamilyDwg();//保存每一户的信息
	bool SaveClassLayer();
	void NoteJZXDist(AcDbObjectIdArray idArrPoly);
	void NoteJG(AcDbObjectIdArray idArrPoly );
	void NoteLayer(AcDbObjectIdArray idArr , int nIdexLayer);
	bool CompareIdArr(AcDbObjectIdArray& idsFirst , AcDbObjectIdArray& idsSecond);
	//传入一个楼层 传出对应楼层的所有的房屋的外围线构成的外包，如果构面失败，返回对应的所有的实体 bCreateArea 是否需要构面
	bool GetExtendLineByFloorNum(int nFloor , AcDbObjectIdArray& ids , bool bCreateArea = true);
	bool GetExtendLineByFloorNum_FSC(double dFloor , AcDbObjectIdArray& ids , bool bCreateArea = true);//增加附属层
	//保存到指定的路径下面
	//bExplode表示是否炸开
	bool SaveToDwg(bool bExplode = true);
	//将自然幢的属性继承到户范围线
	//参数：户号 户范围线
	bool CopyXdata(TCHAR* strHuNum ,  ads_name entHu);
	//按照走向旋转实体
	void RotateEntitysByTrend(AcDbObjectIdArray& idArray , double dAngle ,ads_point ptBase);
};
#endif


