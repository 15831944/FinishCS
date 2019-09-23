#ifndef _PIPESECTION_H_
#define _PIPESECTION_H_
#include "Global.h"

class CPipeSection
{
public:
	CPipeSection();
	~CPipeSection();

	void Start();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月11号
	 */
	void StartEx();

	//获取与中心线相交的道路边线交点;
	void GetInterNodes(ads_name ent, AcGePoint2dArray &nodes);

	void InsertTk();

	void PrepareLine(const AcDbObjectId &, const CString &);

	void AddNote(AcDbObjectId id);

	void Replace(AcDbObjectId id_tk);

protected:
private:

	ads_point m_insert;
	CString str_corss_blk;
	CString str_dlmc;

	AcDbObjectIdArray idarr_save;  //保存的所有实体;
	AcDbObjectId id_line;
	vector<CString> m_vstrBlock;

};


class CLoadVerFace
{
	
public:
	CLoadVerFace();
	~CLoadVerFace();
	//道路纵断面图;
	void Start();

	void DrawLinePt(map<CString, double, CmpCstr> &map_dist, int nCurNum, AcGePoint2dArray &nodes,map<CString, double, CmpCstr> &map_earse);

	void InsertTk(ads_point pt);

	//获取里程桩信息;
	bool GetLczxx(ads_name ent, map<CString, double, CmpCstr> &map_res, int nType = 0);

	//计算插入图框数量;
	void CalTkNum();

	void DrawTable(AcGePoint2dArray nodes);

	void InsertData(AcGePoint2dArray nodes);

	void CombineNodes(AcGePoint2dArray nodes1, AcGePoint2dArray nodes2, AcGePoint2dArray &nodesSrc);

	void Replace(AcDbObjectId id_tk);

private: 
	CString str_ver_blk;
	AcDbObjectIdArray idarr_save;  //保存的所有实体;
	vector<CString> m_vstrBlock;

	AcGePoint3d ptTable;
	int nNum;
	double dDist_one_tk; // 一个图框里面的最大距离;
	CString str_lcz_pre;  //里程桩前缀;
	CString str_dlmc; //道路名称;
	map<CString, double, CmpCstr> map_dist_gc, map_dist_gc_sj;
	map<CString, double, CmpCstr> map_sc_earse, map_sj_earse;
};

#endif

