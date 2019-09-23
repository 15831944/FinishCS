#ifndef _PIPESECTION_H_
#define _PIPESECTION_H_
#include "Global.h"

class CPipeSection
{
public:
	CPipeSection();
	~CPipeSection();

	void Start();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��11��
	 */
	void StartEx();

	//��ȡ���������ཻ�ĵ�·���߽���;
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

	AcDbObjectIdArray idarr_save;  //���������ʵ��;
	AcDbObjectId id_line;
	vector<CString> m_vstrBlock;

};


class CLoadVerFace
{
	
public:
	CLoadVerFace();
	~CLoadVerFace();
	//��·�ݶ���ͼ;
	void Start();

	void DrawLinePt(map<CString, double, CmpCstr> &map_dist, int nCurNum, AcGePoint2dArray &nodes,map<CString, double, CmpCstr> &map_earse);

	void InsertTk(ads_point pt);

	//��ȡ���׮��Ϣ;
	bool GetLczxx(ads_name ent, map<CString, double, CmpCstr> &map_res, int nType = 0);

	//�������ͼ������;
	void CalTkNum();

	void DrawTable(AcGePoint2dArray nodes);

	void InsertData(AcGePoint2dArray nodes);

	void CombineNodes(AcGePoint2dArray nodes1, AcGePoint2dArray nodes2, AcGePoint2dArray &nodesSrc);

	void Replace(AcDbObjectId id_tk);

private: 
	CString str_ver_blk;
	AcDbObjectIdArray idarr_save;  //���������ʵ��;
	vector<CString> m_vstrBlock;

	AcGePoint3d ptTable;
	int nNum;
	double dDist_one_tk; // һ��ͼ�������������;
	CString str_lcz_pre;  //���׮ǰ׺;
	CString str_dlmc; //��·����;
	map<CString, double, CmpCstr> map_dist_gc, map_dist_gc_sj;
	map<CString, double, CmpCstr> map_sc_earse, map_sj_earse;
};

#endif

