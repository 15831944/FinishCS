#ifndef _HOUSE_TO_FLOOR_
#define _HOUSE_TO_FLOOR_
#include "FamilyRoomManager.h"
#include "RighterForRoomHelper.h"
//��������
class CHouseToFloor
{
public:
	CHouseToFloor(void);
	~CHouseToFloor(void);
	void SetEnt(ads_name ent);//������Ȼ��
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
	ads_name m_ent;//��Ȼ��
	double m_rotate_angle;//�ֲ�ֻ�ʱ ������ת�Ƕ�
	TCHAR m_dwg_name[MAX_PATH];//��Ȼ����Ӧ�ķֻ�ͼ�ı���ȫ·��
	CString m_strDataBasePath;
	CString m_strDCBM;
	double m_dH;
private:
	CFamilyRoomManager m_family;
	
	TCHAR m_zrz_floder[MAX_PATH];//��Ȼ�����ļ���·��
	
	CString m_dwg_guid;//�ֻ�ͼdwg����
	std::map<int , AcDbObjectIdArray> m_floor_ents;//¥��Ͷ�Ӧÿ���Ӧ��ʵ��
	std::vector<FaimilyStuct> m_room_list;//��ǰ���Ļ���Ϣ
	ads_point m_ptMax;//��������
	ads_point m_ptMin;//������С��
	map<CString , vector<CString> > m_mapRoom_Floor;
	AcDbObjectIdArray m_all_save_ents;//��Ҫд����ʵ��
	vector<vector<int> >  m_LayerName;//��ͬ��Ĳ���
	//double m_dH;
	CString m_strZHH;
	CString m_strZRZH;
	CString m_current_qlr_database_name;//��ǰ��Ȩ�������ݿ������
	CString m_current_qlr_database_full_name;
	TCHAR   m_szHandle[100];//��Ȼײ�ľ��
	CRighterForRoomHelper m_righter_helper;
private:
	//�����Ȼײ�ķֻ���Ϣ �� ÿһ���Ӧ��ʵ��
	bool GetZRZInfomation();
	bool GetZRZInfomation_GD();  //ȥ��Ȼ���汾��ʹ��
	bool GetZRZInfomation_FC();
	bool GetZRZInfomation_floor();
	bool SaveFamilyDwg();//����ÿһ������Ϣ
	bool SaveClassLayer();
	void NoteJZXDist(AcDbObjectIdArray idArrPoly);
	void NoteJG(AcDbObjectIdArray idArrPoly );
	void NoteLayer(AcDbObjectIdArray idArr , int nIdexLayer);
	bool CompareIdArr(AcDbObjectIdArray& idsFirst , AcDbObjectIdArray& idsSecond);
	//����һ��¥�� ������Ӧ¥������еķ��ݵ���Χ�߹��ɵ�������������ʧ�ܣ����ض�Ӧ�����е�ʵ�� bCreateArea �Ƿ���Ҫ����
	bool GetExtendLineByFloorNum(int nFloor , AcDbObjectIdArray& ids , bool bCreateArea = true);
	bool GetExtendLineByFloorNum_FSC(double dFloor , AcDbObjectIdArray& ids , bool bCreateArea = true);//���Ӹ�����
	//���浽ָ����·������
	//bExplode��ʾ�Ƿ�ը��
	bool SaveToDwg(bool bExplode = true);
	//����Ȼ�������Լ̳е�����Χ��
	//���������� ����Χ��
	bool CopyXdata(TCHAR* strHuNum ,  ads_name entHu);
	//����������תʵ��
	void RotateEntitysByTrend(AcDbObjectIdArray& idArray , double dAngle ,ads_point ptBase);
};
#endif


