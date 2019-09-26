#pragma once
#include "SBlockOpe.h"
#include "SNodesOpe.h"
#include "STrimPoly.h"
#include "Layers.h"
#include "structs.h"
#include "macros.h"
#include "indexini.h"
#include "workdef.h"
#include "tinyxml.h"
#include "ListItemsMdb.h"
#include "EditListCtrl.h"
#include "EntOper.h"
#include "stringsOpe.h"
#include "DatabaseOperManager.h"
#include "XMJGMacro.h"

#define Degree2Rad(deg) ((PI * deg) / 180)
#define Rad2Degree(rad) ((rad * 180) / PI)
#define SCDLBX _T("180301")
#define SCDLZXX _T("180302")
#define SJDLBX _T("180303")
#define SJDLZXX _T("180304")
#define DLFJX _T("180305")
#define SCDMX _T("992000")
#define SJDMX _T("992001")


const TCHAR COMMONTABLE[20] = _T("Common");
const TCHAR JGCOMMONTABLE[20] = _T("JGCommon");
const TCHAR FILENAMETABLE[20] = _T("FileName");
const TCHAR XMJGPATHTABLE[20] = _T("XMJGPath");
const TCHAR HEIGHTTABLE[20] = _T("XMJGHeight");
const TCHAR PICTURETABLE[20] = _T("XMJGPicture");
const TCHAR TEXTPROPERTY[20] = _T("TextProperty");
const TCHAR BRIDGE[20] = _T("XMJGBridge");
const TCHAR PROJECTINFOTABLE[20] = _T("XMJGProjectInfo");

extern int      g_nMaxInfo;
extern ATTRINFO g_AttrInfo[MAX_ATTRSIZE];

extern Indexini g_index;
extern Workdef g_workdef;
extern CListItemsMdb	g_lstitems;

extern vector<DefaultAreaParam> g_vDefaultAreaParam;

struct SelectFilter
{
	int type;
	CString value;

	SelectFilter()
	{

	}

	SelectFilter(int _type, const TCHAR *_value)
	{
		type = _type;
		value.Format(_T("%s"), _value);
	}

	SelectFilter(int _type, const CString &_value)
	{
		type = _type;
		value.Format(_T("%s"), _value);
	}

	~SelectFilter()
	{
	}

};

class FieldChangeTable
{

public:

	void clear();

	CString getNameByIndex(int);

	bool findName(const CString &);

	bool findIndex(const CString &);

	CString getIndexByName(const CString &);

	CString getNameByIndex(const CString &);

	void addField(const CString &, const CString &);

private:

	MStr m_indexname;
	MStr m_nameindex;

};



typedef vector<FNT> FNTS;
typedef vector<SelectFilter> SFilterArray;
typedef vector<FunctionNameTable> FunctionNameTables;
typedef vector<PipeLineTable> PipeLineTables;
typedef FNTS::iterator FNTIter;

/*! \brief		��ȡ����ʵ���еļ�������
*  \param		nodes �����˼���ʵ��Ķ���
*  \author		���
*  \date		[2015��08��27�� 14:00]
*/
void GetMiddlePoint(AcGePoint2dArray & nodes, ads_point& pt);

/* @function ��ȡ����ʵ���еļ�������
* @param nodes ʵ��id
* @param AcGePointd2dʵ�弯������
* @author		���
* @date		[2015��08��27�� 14:00]
*/
void GetMiddlePoint(ads_name, ads_point &);

/* @function ��ȡ����ʵ���еļ�������
* @param nodes ʵ��id
* @param AcGePointd2dʵ�弯������
* @author		���
* @date		[2015��08��27�� 14:00]
*/
void GetMiddlePoint(const AcDbObjectId &id, AcGePoint2d &);

/* @function ��ȡ����ʵ���еļ�������
* @param nodes ʵ��id
* @param AcGePointd3d ʵ�弯������
* @author		���
* @date		[2015��08��27�� 14:00]
*/
void GetMiddlePoint(const AcDbObjectId &id, AcGePoint3d &);

bool InsertDwgFile(CString csPath);

//sType: 1 ��ȡһ���� 2 ��ȡ��ѯ������� 3 ��ȡ���б��
bool getAllTablesName(CAdoConnection *conn, vector<CString> &v_table_name, short sType = 1);

bool	checkLoadARX(const TCHAR *szArxName);

// �ж�ʵ���Ƿ���и���չ����
bool HasXData(const AcDbObjectId& objId, const CString& csRegAppName);

bool AddEntsToGroup(const AcDbObjectId ObjId, AcDbObjectIdArray objArr);


bool setColorsEx(AcDbObjectIdArray objArr, Adesk::UInt16 nIndex);

int getDecimalPrec(CString strDecimal);

/*! \brief		������ID��ȡ���ڵ�����ʵ��
 *  \param
 *  \author		�춫��
 *  \date		[2015��06��03�� 11:00]
 */
bool getEntsByGroupId(AcDbObjectId objGroup, AcDbObjectIdArray &objArr);

void SetPolyCoord_NoArc(ads_name ent, AcGePoint2dArray &Nodes);

bool ssfromNodes_Arc(AcDbObjectIdArray &objArr, AcDbObjectId objPolyline, int nWay = 1, double dOffset = 1.0, TCHAR *szEntityType = NULL, TCHAR *szLayer = NULL);

int isPolyInPoly_Completely(AcDbObjectId obj1, AcDbObjectId obj2);


CString findDefaultValue(CString strCode);

bool getEntPositionEx(AcDbObjectId objId, AcGePoint3d &pt3d);



//bool isElevationPoint(AcDbObjectId ObjId);

bool setColorsEx2(AcDbObjectIdArray objArr, int nType = 1);

bool setColorsEx2(AcDbObjectId ObjId, int nType = 1);

AcDbObjectId drawWipeOut(AcGePoint2dArray nodes);

//��ʱд������ʱ�ٴ������ļ���ȡ
CString getBuildStruct_FCS(AcDbObjectId objBuild);

//ͨ�������ļ�����ȡ���ݽṹ
CString getBuildStruct_FCS_Config(AcDbObjectId objBuild);

CString format_double_2(double dValue, int nDecimal);

//��ָ�봫�������ǵ�����Ҫdelete��
bool openXMLFile(TiXmlDocument *&tinyXml, TCHAR *szName);

bool readDefaultAreaParam();

void CreateLog(CString csPath);

void AddLog(CString csPath, CString csStr);

int AddXdataShort_bat(AcDbObjectIdArray objArr, const TCHAR *AppName, short nValue);

void AddXdataDouble_bat(AcDbObjectIdArray objArr, const TCHAR *AppName, double dValue);


bool ssFromLayers(AcDbObjectIdArray &objArr, const TCHAR *szLayer, bool bRemove);

bool checkDlgEditControlText(HWND hwnd, int nMaxCount, TCHAR *szSpecialWords = NULL);

//����Ƿ�������غϵı߽�
bool IsPolyValid(AcGePoint2dArray &Nodes, CArray<AcGePoint2dArray, AcGePoint2dArray> &NodesArr);

extern bool	checkProjInfo();

/*! \brief		����GIS��������ʵ��
 *  \param
 *  \author		�춫��
 *  \date		[2015��05��19�� 12:00]
 */
bool filterObjectIdArr_ByGISTable(AcDbObjectIdArray objArr, AcDbObjectIdArray &objArrResult, const TCHAR *szGisTableName, short sRemoveResult);

void	cstringArrToVector(const CStringArray &strArr, vector<CString> &vStr);

void vectorToStringArray(const vector<CString> &vStr, CStringArray &strArr);

void vectorToAcDbObjectIdArray(const vector<AcDbObjectId> vObjId, AcDbObjectIdArray &objArr, bool bClearResult = true);

//CArray<ATTRFIELD,ATTRFIELD> *AttrArr = NULL;
bool findAttriTable(const TCHAR *szTableName, CArray<ATTRFIELD, ATTRFIELD> *&AttrArr);

bool index_name_code(const TCHAR *szName, TCHAR *szCode);

bool index_name_code(const TCHAR *szTable, const TCHAR *szName, TCHAR *szCode);

void orderObjects(AcDbObjectIdArray &objArr);

bool filterSameString(vector<CString> &vStr);

void SetXMLAttribute(TiXmlElement * pEle, CString pName, CString pValue);

bool initAttribute();

bool InitParaMeter();

// bool BeginWestNorth(ads_name);

/* @function ��ȡ����ε�������
* @param AcDbObjectId ����ε�ʵ��id
* @return bool �ɹ���ȡ����true�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��15��
*/
bool BeginWestNorth(const AcDbObjectId &);

/* @function ��ȡ�㵽ĳ���ߵ��������������ֵΪ��ʼ���������������е���������pt������򷵻ص�����ֵΪ2
* @param AcGePoint2d ָ���ĵ�
* @param AcGePointdArray ����εĵ㼯
* @param int ������������ֵ
* @author ���
* @time 2017��2��15��
*/
void getNearestEdge(const AcGePoint2d &, const AcGePoint2dArray &, int &);


/* @function ��ȡ�㵽ĳ���ߵ��������������ֵΪ��ʼ���������������е���������pt������򷵻ص�����ֵΪ2
* @param AcGePoint2d ָ���ĵ�
* @param AcDbObjectId �����ʵ��id
* @param int ������������ֵ
* @author ���
* @time 2017��2��15��
*/
void getNearestEdge(const AcGePoint2d &, const AcDbObjectId &, int &);


/* @function ��ȡ�㵽ĳ��ֱ�ߵ����ľ���
* @param AcGePoint2d ָ���ĵ�
* @param AcGePoint2d ֱ�ߵ���ʼ��
* @param AcGePoint2d ֱ�ߵ���ֹ��
* @return double ���ؾ���
* @author ���
* @time 2017��2��15��
*/
double getPoint2LineMidPointDist(const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &);

/* @function ��ȡ�㵽ֱ�ߵľ���
* @param AcGePoint2d ָ���ĵ�
* @param AcGePoint2d ֱ�ߵ���ʼ��
* @param AcGePoint2d ֱ�ߵ���ֹ��
* @return double ���ؾ���
* @author ���
* @time 2017��2��15��
*/
double getPoint2LineDist(const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &);

/* @function ��ȡ������֮��ļн�����ͼ��ʾ
							X(ptx)
							*
							*
							*
							*
							O(pto)*  *  *  *  *Y(pty)
							* @param ptxΪ�нǵ�һ������
							* @param ptoΪ�нǵĶ���
							* @param ptyΪ�нǵ���һ������
							* @return double ����XOY֮��ļнǣ������ƣ�
							* @author ���
							* @time 2017��2��22��*
							*/
double getIntersectionAngle(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pty);

/* @function ��ȡʵ���ÿ���ǵĽǶ�
* @param AcDbObjectId ʵ���ID
* @param vector<double> �Ƕȵļ���
* @author ���
* @time 2017��2��22��*
*/
void getIntersectionAngle(const AcDbObjectId &, vector<double> &);

/* @��ȡ�Ƕȼ��ϵĽ�ƽ����
* @param angles�Ƕȵļ���
* @param bisecs��ƽ���߼���
* @param flag
* @author ���
* @time 2017��2��22��
*/
void getAngleBisection(const AcGePoint2dArray &, vector<double> &);

/* @��ȡĳ��ʵ��Ľ�ƽ����
* @param angles�Ƕȵļ���
* @param bisecs��ƽ���߼���
* @param flag
* @author ���
* @time 2017��2��22��
*/
void getAngleBisection(const AcDbObjectId &, vector<double> &);

/* @funtion ��ȡ��ƽ����
* @param angleĳ���Ƕ�
* @param ptx ����x
* @param pto �н�o
* @param pty ����y
* @return ���ؼнǵĽ�ƽ����
* @author ���
* @time 2017��2��22��*
*/
double getAngleBisection(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pty);

/* @��ȡ�Ƕȼ��ϵ����ƽ����
* @param angles �Ƕȵļ���
* @param bisecs ���ƽ���߼���
* @param flag
* @author ���
* @time 2017��2��22��
*/
void getOutAngleBisection(const AcGePoint2dArray &, vector<double> &);

/* @��ȡĳ��ʵ������ƽ����
* @param angles�Ƕȵļ���
* @param bisecs ���ƽ���߼���
* @param flag
* @author ���
* @time 2017��2��22��
*/
void getOutAngleBisection(const AcDbObjectId &, vector<double> &);

/* @funtion ��ȡ���ƽ����
* @param angleĳ���Ƕ�
* @param ptx ����x
* @param pto �н�o
* @param pty ����y
* @param bDeasil ����㼯��˳ʱ�룬��bDeasilΪtrue������Ϊfalse
* @return ������нǵĽ�ƽ����
* @author ���
* @time 2017��2��22��*
*/
double getOutAngleBisection(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pt);

/* @function ѡ��ʵ��
* @param AcDbObjectId ��ѡ���ʵ��id
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntity(AcDbObjectId &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectId ��ѡ���ʵ��id
* @param SelectFilter ѡ��ʵ��Ĺ��������� ���磺ʵ�������ڵ�ͼ����JZDͼ�㣬��
SelectFilter�����ķ���ΪSelectFilter f2(8, _T("JZD"));
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntity(AcDbObjectId &, const SelectFilter &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectId ��ѡ���ʵ��id
* @param SelectFilter ѡ��ʵ��Ĺ�������
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntity(AcDbObjectId &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectId ��ѡ���ʵ��id
* @param SelectFilter ѡ��ʵ��Ĺ�������
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntity(AcDbObjectId &, const SelectFilter &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function ѡ��ȫ��ʵ��
* @param AcDbObjectIdArray ��ѡ���ʵ��id����
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntitys(AcDbObjectIdArray &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectIdArray ��ѡ���ʵ��id����
* @param SelectFilter ѡ��ʵ��Ĺ��������� ���磺ʵ�������ڵ�ͼ����JZDͼ�㣬��
SelectFilter�����ķ���ΪSelectFilter f2(8, _T("JZD"));
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntitys(AcDbObjectIdArray &, const SelectFilter &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectIdArray ��ѡ���ʵ��id����
* @param SelectFilter ѡ��ʵ��Ĺ�������
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntitys(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectIdArray ��ѡ���ʵ��id����
* @param SelectFilter ѡ��ʵ��Ĺ�������
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntitysOr(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectIdArray ��ѡ���ʵ��id����
* @param SelectFilter ѡ��ʵ��Ĺ�������
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntitysOr(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function ѡ��ʵ��
* @param AcDbObjectIdArray ��ѡ���ʵ��id����
* @param SelectFilter ѡ��ʵ��Ĺ�������
* @param TCHAR ѡ��ʵ��ķ���
* @return bool ѡ��ʵ��ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
* @author ���
* @time 2017��2��27��
*/
bool SelectEntitys(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function ͨ��ID��ȡʵ��ľ��
* @param AcDbObjectId ʵ���id
* @param bool ��������
* @return TCHAR ʵ��ľ��
* @author ���
* @time 2017��2��27��
*/
CString GetHandleByObject(const AcDbObjectId &, bool);

/* @function ͨ��ʵ��ľ����ȡʵ���ID
* @param TCHAR ʵ��ľ��
* @return AcDbObjectId ʵ���ID
* @author ���
* @time 2017��2��27��
*/
AcDbObjectId GetObjectByHandle(const TCHAR *);

/* @function ͨ��ID��ȡʵ��ľ��
* @param AcDbObjectId ʵ���id
* @return CString ʵ��ľ��
* @author ���
* @time 2017��2��27��
*/
CString GetHandleByObject(const AcDbObjectId &);

/* @function ͨ��ʵ��ľ����ȡʵ���ID
* @param CString ʵ��ľ��
* @return AcDbObjectId ʵ���ID
* @author ���
* @time 2017��2��27��
*/
AcDbObjectId GetObjectByHandle(const CString &);

AcDbObjectId DrawArrow(const AcGePoint2d &, const double &);

AcGePoint2d GetArrowEndPoint(const AcDbObjectId &);

void DrawArrow(const AcDbObjectId &);

AcDbObjectId DrawArrowEx(const AcDbObjectId &, const vector<CString> &);

void DrawTable(const AcGePoint2d InsertPt, AcDbObjectId &TblId, const int nRows,
	const int nCols, const double dWidth, const double dHeight, const bool bAutoScale,
	const TCHAR *szTxtStyle, const double dTxtHeight);

AcDbObjectId DrawArrow(const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &, const double &);

void DrawCadTable(const AcGePoint3d InsertPt, AcDbObjectId &TblId, const int nRows, const int nCols,
	const double dWidth, const double dHeight, bool bAutoScale, vector<vector<CString>> _content, double textHight = 2.0);

void DrawCadTable(const AcGePoint3d InsertPt, AcDbObjectId &TblId, const int nRows, const int nCols,
	const double dWidth, const double dHeight, bool bAutoScale);

bool GetTableEntFromID(const AcDbObjectId &id, AcDbTable *&pTable);
void SetCadTableAlignCenter(AcDbTable* p_table);


/* @function �Զ����
* @param
* @return AcDbObjectId ʵ���ID
* @author ����
* @time 2017��3��13��
*/
AcDbObjectId DrawHatch_Auto(ads_name ent, TCHAR * szHatch, double dScale, double dAngle,
	AcDbObjectIdArray *pislandIds, int nColor, TCHAR * szLayer);

/* @function �Զ����
* @param
* @return AcDbObjectId ʵ���ID
* @author ����
* @time 2017��3��13��
*/
AcDbObjectId DrawHatch(ads_name ent, TCHAR * szHatch, double dScale, double dAngle,
	AcDbObjectIdArray *pislandIds, int nColor, TCHAR * szLayer);

/* @function �жϵ�pk�Ƿ����߶�pi pj��
* @param
* @return
* @author ����
* @time 2017��3��15��
*/
bool PointAtLine(AcGePoint2d pi, AcGePoint2d pj, AcGePoint2d pk);


/* @function ��������pkpi������pjpi�Ĳ��
* @param
* @return
* @author ����
* @time 2017��3��15��
*/
double direction(AcGePoint2d pi, AcGePoint2d pj, AcGePoint2d pk);


/* @function �ж��߶�p1p2��p3p4�Ƿ��ཻ
* @param
* @return
* @author ����
* @time 2017��3��15��
*/
bool judgeIntersection(AcGePoint2d p1, AcGePoint2d p2, AcGePoint2d p3, AcGePoint2d p4); //


/* @function ����ͼ���ȡһ�������ʵ���һ����
* @param
* @return  �����û���ѡ���ıߵ���㼰�յ��array��size = 2��
* @author ����
* @time 2017��3��16��
*/
AcGePoint2dArray getOneStraightSideOfEnt(AcDbObjectId &outId);

/* @function ����һ������ע��
* @param
* @return  ����ע��id
* @author ����
* @time 2017��3��16��
*/
AcDbObjectId drawSingleAxisAnno(AcGePoint2d spt, CString annoText, double annoAngle);

/* @function ����һ������ע��
 * @param
 * @return  ����ע��id
 * @author ����
 * @time 2017��3��16��
 */
AcDbObjectId drawSingleAxisAnno(AcGePoint2d spt, CString annoText, double annoAngle, const TextProperty &, const TextProperty &, const TextProperty &);


/* @function ��ȡ��inPt���ض�ֱ���ϵ�ͶӰ��
* @param linept ����ȷ��ֱ��λ�õ�ֱ��������һ��
* @param angle ֱ�ߵ���б��
* @param inPt Ҫ��ȡͶӰ�������
* @return  ͶӰ��
* @author ����
* @time 2017��3��20��
*/
AcGePoint2d getProjectedPointOnLine(AcGePoint2d linept, double lineAngle, AcGePoint2d inPt);


/* @function �޸�ͼ����ɫ
* @return
* @author ����
* @time 2017��3��24��
*/
void ChangeBlockRefenceColor(AcDbObjectId ObjId, int nRed, int nGreen, int nBlue);

/* @function �޸�ͼ����ɫ
* @return
* @author ����
* @time 2017��3��24��
*/
void ChangeBlockRefenceColor(AcDbObjectId ObjId, int colorIndex);

/* @�ӿ� �˽ӿ���Ϊ�˴�ӡ�ĸ�����������
 * @���� ���
 * @ʱ�� 2017��08��24��
 */
void printErrorFunction(const char *);

/* @�ӿ�
* @���� ���
* @ʱ�� 2017��08��24��
*/
void printErrorInfo(const char *, const char *, int);

/* @�ӿ�
 * @���� ���
 * @ʱ�� 2017��08��24��
 */
void printErrorInfo(const char *, const char *, int, const char *);

bool OpenDWG(CString strPath);

/* @�ӿ�
 * @���� ���
 * @ʱ�� 2017��08��24��
 */
bool saveCurDwg(const TCHAR *);

/* @�ӿ�
 * @���� ���
 * @ʱ�� 2017��08��24��
 */
void openDwgCommand(const TCHAR *);

/* @�ӿ�
 * @���� ���
 * @ʱ�� 2017��08��24��
 */
void sendCommand(const TCHAR *);

class IDataBaseOper
{

	typedef CDatabaseOperManager DataBase;

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��08��24��
	 */
	IDataBaseOper();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��08��24��
	*/
	~IDataBaseOper();

	/* @�ӿ� ��ȡ����������������ʱ��
	 * @���� ���
	 * @ʱ�� 2017��12��8��
	 */
	long readJRStartTime();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��19��
	 */
	CString readJGCommon(const CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��19��
	 */
	bool readJGCommon(const CString &, long &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��19��
	 */
	bool readJGCommon(const CString &, float &);

	/* @�ӿ� ��ȡ���ݿ������
	 * @���� DataBase *���ݿ����ָ��
	 * @����
	 * @����
	 * @���� ��ʼ���ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	bool readDataBase(const MStr &, MStr &);

	/* @�ӿ� �������ݿ������
	 * @���� DataBase *���ݿ����ָ��
	 * @����
	 * @����
	 * @���� bool ִ�гɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��08��17��
	 */
	bool updataDataBase(const MStr &, const MStr &);

	/* @�ӿ�
	 * @����
	 * @���� bool ִ�гɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��08��22��
	 */
	bool readDataBase(const CString &, const MStr &, MStr &);

	/* @�ӿ� 
	 * @���� 
	 * @���� 
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��13��
	 */
	bool IDataBaseOper::readDataBase(const CString &table, const MStr &filter, VMStr &values);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� bool ִ�гɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��08��23��
	 */
	bool readDataBase(const CString &, const CString &, CString &);

	/* @�ӿ�
	 * @����
	 * @���� bool ִ�гɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��08��22��
	 */
	bool readDataBase(const CString &, const CString &, const MStr &, VStr &);

	/* @�ӿ�
	 * @����
	 * @���� bool ִ�гɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��08��22��
	 */
	bool readDataBase(const CString &, const CString &, const MStr &, CString &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��23��
	 */
	CString readCommonTable(const CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��09��18��
	 */
	bool readCommonTable(const CString &, MStr &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��25��
	 */
	bool readCommonTable(const CString &, CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��08��25��
	 */
	bool readCommonTable(const CString &, int &);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��08��25��
	*/
	bool readCommonTable(const CString &, float &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��29��
	 */
	bool writeCommonTable(const CString &, int &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��29��
	 */
	bool writeCommonTable(const CString &, float &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��29��
	 */
	bool writeCommonTable(const CString &, const CString &);

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	bool clearPictureTable();

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	bool addPictureTableInfo(const CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	int getFieldCountByTable(const CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��09��11��
	 */
	bool readCommonTable(const CString &, const CString &, CString &);

	/* @�ӿ� ��ȡ���ݿ�����Ŀ��Ϣ��
	 * @���� CString ��ȡ�ĸ�ҵ��
	 * @���� MStr ��ȡ�Ľ��
	 * @���� ��ȡ�ɹ�������ֵΪtrue
	 * @���� ���
	 * @ʱ�� 2017��08��25��
	 */
	bool readProjectInfoTable(const CString &, MStr &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��09��11��
	 */
	CString readPathTable(const CString &);

	/* @�ӿ� ��ȡ�ļ�·����
	 * @���� CString ����ȡ���ļ�·��
	 * @���� ���
	 * @ʱ�� 2017��09��18��
	 */
	CString readPathTableEx(const CString &);

	/* @�ӿ� ��ȡ�ļ����Ӷ���ȡ����ļ���
	 * @���� CString ������ļ���ģ��
	 * @���� CString ����ļ���
	 * @���� ���
	 * @ʱ�� 2017��09��18��
	 */
	CString readFileNameTable(const CString &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��23��
	 */
	void readFunctionTable(FunctionNameTables &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��23��
	 */
	void saveFunctionTable(const FunctionNameTables &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void readHeightTable(MVMStr &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void writeHeightTable(const MVMStr &);

	/* @�ӿ� �˽ӿ��ڳ����ʼ��ʱ�Ѿ�����
	 * @���� ���
	 * @ʱ�� 2017��10��14��
	 */
	bool readFieldChangeTable();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	bool readCarTable(MStr &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��12��4��
	 */
	CString readBGTable(const CString &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��12��4��
	 */
	void removeBGTableInfo(const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��31��
	 */
	bool readCompressTable(const CString &, VStr &);

	/* @�ӿ� ��ȡTextProperty��
	 * @���� CString ��ȡ������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��25��
	 */
	TextProperty readTextPropertyTable(const CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��12��4��
	 */
	void AddBGInfoTable(const CString &, const CString &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��12��2��
	 */
	TextProperty readTextPropery(const CString &, bool size = false, bool font = false);

	/*��ȡ���ƹ������ñ�
	*/
	void readPipelineTable(PipeLineTables &fnts);

	/*������ƹ������ñ�
	*/
	void savePipelineTable(const PipeLineTables &fnts);

	/*��ȡ�ܵ����
	*/
	void readPipePointBM(const CString &strGDLX, CString &strGDBM);

	/*��ȡ���߱���
	*/
	bool readPipelineDM(const CString &strGXLX, CString &strGXDM);
	
	/*��ȡ���߱���
	*/
	bool readPipelineBM(const MStr &, CString &strGXDM);

	/*��ȡ�ܵ��Ӧ�Ŀ���� 
	 */
	void readPipePointKBM(const CString &strGDLX, CString &strKBM);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��20��
	 */
	CString readPipeLineTable(const CString &, const MStr &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��13��
	 */
	MStr readOutputFormatTable(const CString &);

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��25��
	 */
	VStr readAllPipeLineLayer();

private:

	DataBase *_db;

};

class IProjectMDB
{
	typedef CDatabaseOperManager DataBase;

public:

	/* @�ӿ� Ĭ�Ϲ���ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��3��
	 */
	IProjectMDB();

	/* @�ӿ� Ĭ�������ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��3��
	 */
	~IProjectMDB();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	MStr getBridgeXX();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��21��
	 */
	MStr getProjectInfo();
	
	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��23��
	 */
	VMStr getBGInfoTable();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	CString getClearWidth();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	VMStr getCXXTableInfo();

	/* @�ӿ� ��ȡ����˵����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	MStr getJSSMTableInfo();

	/* @�ӿ� �����Ϣ��
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	MStr getMJXXTableInfo();

	/* @�ӿ�
	 * @���� ���
	 * @���� VMStr ����ͼƬ��Ϣ��
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	VMStr getPictureTable();

	/* @�ӿ� �����Ϣ��
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	MStr getBGXXTableInfo();

	/* @�ӿ�
	 * @���� ���
	 * @���� MStr ���ػ�����Ϣ����Ϣ
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	MStr getJBXXTableInfo();

	/* @�ӿ�
	 * @���� ���
	 * @���� VMStr ��ȡ����Ϣ
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	VMStr getDXXTableInfo();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��3��
	 */
	CString getProjectName();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��12��5��
	 */
	bool clearPictureTable();

	/* @�ӿ� ��ȡ��������
	 * @���� CString ��������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	CString getClearHeight();

	/* @�ӿ� ��ȡ���ŵĿ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	CString getBridgeWidth();

	/* @�ӿ� ��ȡ���ŵĳ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	CString getBridgeLength();

	/* @�ӿ� ��ȡ��·���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��28��
	 */
	CString getLoadLevation();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��28��
	 */
	CString getBridgeLevation();

	/* @�ӿ� ��ȡ���ź��
	 * @���� CString���ŵĺ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	CString getBridgeThickness();

	/* @�ӿ� ��ȡ�������˸߶�
	 * @���� CString�������˵ĸ߶�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	CString getBridgeBalustrade();

	/* @�ӿ�
	* @����
	* @���� ���
	* @ʱ�� 2018��1��4��
	*/
	bool setBridgeXX(const MStr &);
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��18��
	 */
	VMStr getGNQXXInfo(const MStr &);
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��18��
	 */
	MStr getGNQXXInfoEx(const MStr &);
	
	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��12��
	 */
	void delCXXTableInfo(const MStr &);

	/* @�ӿ� ���ò���Ϣ
	 * @���� MStr����Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void addCXXTableInfo(const MStr &);

	/* @�ӿ� ��ȡ����Ϣ
	 * @���� MStr��������
	 * @���� ���
	 * @ʱ�� 2018��2��26��
	 */
	MStr getCXXTableInfo(const MStr &);

	/* @�ӿ� ��ȡ����Ϣ
	 * @���� MStr����Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	MStr getDXXTableInfo(const MStr &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	MStr getTableInfo(const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��26��
	 */
	void setCXXTableInfo(const MStr &);

	/* @�ӿ� ������������
	 * @���� CString��������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	bool setClearWidth(const CString &);

	/* @�ӿ� �����Ϣ��
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	void setBGXXTableInfo(const MStr &);

	/* @�ӿ� ��ȡ����˵����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	void setJSSMTableInfo(const MStr &);

	/* @�ӿ� ���û�����Ϣ����Ϣ
	 * @���� MStr��������Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void setJBXXTableInfo(const MStr &);

	/* @�ӿ� �����Ϣ��
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	void setMJXXTableInfo(const MStr &);

	/* @�ӿ� ���ݲ���������Ϣ��������Ϣ
	 * @���� CString����Ĳ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void clearCXXTable(const CString &);

	/* @�ӿ� �������ŵĿ��
	 * @���� CString ���ŵĿ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	bool setBridgeWidth(const CString &);

	/* @�ӿ� ������������
	 * @���� CString ��������ֵ
	 * @���� bool ���óɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	bool setClearHeight(const CString &);

	/* @�ӿ� ɾ�������Ϣ
	 * @���� CString ɾ��ָ���ı����Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void delBGTableInfo(const CString &);

	/* @�ӿ� ���õ�·���
	 * @���� CString ��·���
	 * @���� ���óɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��28��
	 */
	bool setLoadLevation(const CString &);

	/* @�ӿ� �������ŵĳ���
	 * @���� CString ���ŵĳ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	bool setBridgeLength(const CString &);
	
	/* @�ӿ� ��ȡ����Ϣ
	 * @���� MStr��������
	 * @���� ���
	 * @ʱ�� 2018��2��26��
	 */
	VMStr getCXXTableInfoEx(const MStr &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��25��
	 */
	bool updataPictureTable(const VMStr &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��26��
	 */
	VMStr getCXXTableInfo(const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��3��
	 */
	CString getProjectInfo(const CString &);

	/* @�ӿ� ����������
	 * @���� CString ������
	 * @���� ���óɹ�������ֵΪtrue�� ���򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��28��
	 */
	bool setBridgeLevation(const CString &);

	/* @�ӿ� �������ź��
	 * @���� CString ���ŵĺ��
	 * @���� bool ���óɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	bool setBridgeThickness(const CString &);
	
	/* @�ӿ� �����������˸߶�
	 * @���� CString �������˵ĸ߶�
	 * @���� bool ���óɹ�����ֵΪtrue�� ���򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	bool setBridgeBalustrade(const CString &);

	/* @�ӿ� ����ͨ������߶�
	 * @���� MStr ͨ������߶�
	 * @���� bool���óɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void setChannelBodyPosition(const MStr &);
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��10��24��
	 */
	bool initMDB(const CString &path = _T(""));

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��18��
	 */
	void addGNQXXInfo(const MStr &, const MStr &);

	/* @�ӿ� ����ָ�����ͻ�ȡͨ������ֵ
	 * @���� CString ͨ������λ������
	 * @���� CString ͨ������λ��ֵ
	 * @���� ���
	 * @ʱ�� 2018��2��26��
	 */
	CString getChannelBodyPosition(const CString &);

	/* @�ӿ� ���ò���Ϣ
	 * @���� MStr��������
	 * @���� MStr���õĲ���Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void setCXXTalbeInfo(const MStr &, const MStr &);

	/* @�ӿ� ���ö���Ϣ
	 * @���� MStr��������
	 * @���� MStr���õĶ���Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void setDXXTalbeInfo(const MStr &, const MStr &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��21��
	 */
	void setProjectInfo(const CString &, const MStr &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void updataCXXTableInfo(const MStr &, const MStr &);
	
	/* @�ӿ� ��ȡ�����Ϣ
	 * @���� CString ��ȡ�ı������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	CString getBGTable(const CString &, const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��3��
	 */
	void setProjectInfo(const CString &, const CString &);

	/* @�ӿ� ɾ�����еļ�¼
	 * @���� CString ɾ��ָ���ı�
	 * @���� MStr Ҫɾ���ļ�¼
	 * @���� bool ɾ���ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	bool deleteTableRecord(const CString &, const MStr &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	bool updataBridgeXX(const CString &, const CString &);

	/* @�ӿ� ���ñ����Ϣ
	 * @���� CString ���ñ���ֶ�
	 * @���� CString ���õı��ֵ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void setBGInfoTable(const CString &, const CString &, const CString &);

	/* @�ӿ� ���ñ����Ϣ
	 * @���� CString ���ñ���ֶ�
	 * @���� CString ���õı��ֵ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void setBGInfoTable(const CString &, const CString &, const CString &, const CString &, const CString &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��15��
	 */
	void setBGGUID(const CString &);
	
	/* @�ӿ� ����ͨ������λ��
	 * @���� CString ����ͨ����������ͣ�ͨ���������X��ͨ���������Y��ͨ�������յ�X��ͨ�������յ�Y��
	 * @���� CString ͨ������λ�õ�ֵ
	 * @���� bool ���óɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	bool setChannelBodyPosition(const CString &, const CString &);

	/*����PipeLine��project.mdb
	*/
	void setPipeLineTable(const CString &table, const VMStr &record);

	/*��ȡPIPELINE����strGxlx�������͵���ɫ
	*/
	bool getPipelineColor(const CString &strGxlx, int &nR, int &nG, int &nB);

	bool getPipelineColor(const CString &strGxlx, CString &strColor);

	/*�����������ݿ�
	*/
	BOOL ChangeConnection(const CString &srtPath);

	/*��ȡPIPELINE��
	*/
	VMStr getPipeTable();

	/* @�ӿ� ���õ�·��Ϣ
	 * @���� MStr ��·��Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	bool setRoadInfo(const CString &, const GHSSJBQKInfo &);
	
	/* @�ӿ� ���õ�·��Ϣ
	 * @���� MStr ��·��Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	bool setRoadInfo(const CString &, const GHSSJBQKPipe &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void getRoadInfo(const CString &, GHSSJBQKInfo &);
	
	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void getRoadInfo(const CString &, GHSSJBQKPipe &);

	/* @�ӿ� ����RXBridgeInfo����Ϣ
	 * @ʱ�� 2018��4��25��
	 */
	void setRXBridgeTableInfo(const VMStr &records);

	/* @�ӿ� ��ȡRXBridgeInfo����Ϣ
	 * @ʱ�� 2018��4��25��
	 */
	VMStr readRXBridgeTableInfo();

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��7��4��
	 */
	VStr getAllRoadName();

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��8��
	 */
	int getRecordCount(const CString &, const MStr &);

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��9��
	 */
	bool hasTableRow(const CString& table, const MStr &field_value, VMStr &selectRow);

private:

	DataBase *db;

};

/* @�ӿ�
 * @����
 * @����
 * @����
 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ���
 * @ʱ�� 2017��9��26��
 */
bool getLayerNum(const CString &, int &, int &);

/* @�ӿ�
 * @����
 * @����
 * @����
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��25��
 */
void getJGLayerNum(const CString &, int &, int &);

/* @�ӿ�
 * @����
 * @����
 * @����
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��25��
 */
void getStrAndNumByString(const CString &, CString &, int &);

bool closeDWG(const CString &);

void splitBigString(const CString &, string &);

#define ErrorInfo() printErrorInfo(__FILE__, __FUNCTION__, __LINE__)

/* @�ӿ� ��ʵ���������
 * @���� AcDbObjectIdArray ʵ��id����
 * @���� CString ��չ��������
 * @���� del ���ʵ�岻������չ�������ƣ����Ƿ�ɾ�Ƴ���ʵ��(Ĭ���Ƴ�)
 * @���� ���
 * @ʱ�� 2017��11��16��
 */
void sortObjectByXDataInt(AcDbObjectIdArray &, const VStr &, bool del = true);

/* @�ӿ� ��ʵ���������
 * @���� AcDbObjectIdArray ʵ��id����
 * @���� CString ��չ��������
 * @���� del ���ʵ�岻������չ�������ƣ����Ƿ�ɾ�Ƴ���ʵ��(Ĭ���Ƴ�)
 * @���� ���
 * @ʱ�� 2017��11��16��
 */
MSIds sortGNQByXData(const AcDbObjectIdArray &);//���ղ�ߺͼ���ϵ�����й���������

/* @�ӿ�
 * @���� ���
 * @ʱ�� 2017��11��16��
 */
void sortObjectByXData(AcDbObjectIdArray &, const CString &, const CString &);

/* @�ӿ�
 * @����
 * @���� ���
 * @ʱ�� 2017��11��20��
 */
bool stringIsDigit(const CString &);

/* @�ӿ�
* @���� ���
* @ʱ�� 2017��11��22��
*/
void convertDwg2Pdf(const CString &);

/* @�ӿ�
* @���� ���
* @ʱ�� 2017��11��22��
*/
void convertDwg2WMF(const CString &);

/* @�ӿ�
 * @����
 * @����
 * @����
 * @���� ���
 * @ʱ�� 2017��11��26��
 */
void getLayerArea(const AcDbObjectId &, double &, double &, double);

/* @�ӿ� ��ȡ��λ�ĸ���
 * @���� ���
 * @ʱ�� 2017��12��4��
 */
void getCarCount(const AcDbObjectId &id, MStr &, double &, double &);

/* @�ӿ�
 * @����
 * @���� ���
 * @ʱ�� 2017��11��26��
 */
double getLayerJZMJArea(const AcDbObjectId &);

/* @�ӿ�
 * @����
 * @���� ���
 * @ʱ�� 2017��11��26��
 */
double getLayerJRMJArea(const AcDbObjectId &);

/* @�ӿ�
 * @����
 * @���� ���
 * @ʱ�� 2017��12��1��
 */
short isEntClosed(const AcDbObjectId &);

/* @�ӿ�
 * @����
 * @����
 * @���� ���
 * @ʱ�� 2017��12��1��
 */
bool SetLineWidth(const AcDbObjectId &, const double);		///< �����߿�

/* @�ӿ� ��ȡʵ���ڲ���ʵ��
 * @���� AcDbObjectId ʵ��
 * @���� ���
 * @ʱ�� 2017��12��1��
 */
AcDbObjectIdArray getInnerObjects(const AcDbObjectId &);

/* @�ӿ�
 * @����
 * @����
 * @����
 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ���
 * @ʱ�� 2017��10��10��
 */
bool getNextPointByLine(const AcDbObjectId &, AcGePoint2dArray &);

AcDbObjectId drawLine();

AcDbObjectId drawLine(const CString &, int);

long TimeConvertLong(const CString &value);

void openAllDwg(const VStr &);

void closeAllDwg(VStr &);

AcDbObjectId nodeAnnotation(ads_point, ads_point);

void singleNodeAnnotation(const TextProperty &, double scale = 1.0);

void singleNodeAnnotation(const TextProperty &, const TextProperty &);

/* @�ӿ� �뾶��ע
 * @���� AcDbObjectId ��Ҫ��ע��ʵ��
 * @���� double ��ע���߳���
 * @���� AcDbObjectIdArray �뾶��ע����ʵ��
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��2��8��
 */
AcDbObjectIdArray AddRaidusNote(const AcDbObjectId &id, double);

/* @�ӿ� �뾶��ע
* @���� AcDbObjectId ��Ҫ��ע��ʵ��
* @���� double ��ע���߳���
* @���� AcDbObjectIdArray �뾶��ע����ʵ��
* @���� ���
* @���� 575814050@qq.com
* @ʱ�� 2018��2��8��
*/
AcDbObjectIdArray AddRaidusNote(const AcDbObjectId &id, double, const TextProperty &);

void mulitNodeAnnotation(const AcDbObjectIdArray &, const TextProperty &);

bool GetPrinterPath(CString &);

void UpdatePrinterConfig();

/* @�ӿ� ��ȡϵͳ�İ汾
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��24��
 */
int getSystemVersion();

/* @�ӿ� �޸�ʵ��ļ�ͷ�Ĵ�С(���ô˽ӿ�֮ǰ����Ҫ��ס�ĵ���
 * @���� AcDbObjectId ��ͷʵ���id
 * @���� double ��ͷ�Ĵ�С
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��24��
 */
void modifyEntArrowSize(const AcDbObjectId &, double);

/* @�ӿ�
 * @���� 
 * @���� 
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��3��28��
 */
void modifyEntCircleSize(const AcDbObjectId &, double);

/* @�ӿ� �޸�ʵ����ı���С(���ô˽ӿ�֮ǰ����Ҫ��ס�ĵ���
 * @���� AcDbObjectId �ı�ʵ���id
 * @���� double �ı��Ĵ�С
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��24��
 */
void modifyEntTextHeight(const AcDbObjectId &, double);

/* @�ӿ� �����޸ļ�ͷ�Ĵ�С(���ô˽ӿڲ���Ҫ��ס�ĵ���
 * @���� AcDbObjectIdArray ��ͷʵ�弯��
 * @���� ��ͷ�Ĵ�С
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��24��
 */
void modifyEntsArrowSize(const AcDbObjectIdArray &, double);

/* @�ӿ� �����޸ļ�ͷ�Ĵ�С(���ô˽ӿڲ���Ҫ��ס�ĵ���
* @���� AcDbObjectIdArray ��ͷʵ�弯��
* @���� ��ͷ�Ĵ�С
* @���� ���
* @���� 575814050@qq.com
* @ʱ�� 2018��1��24��
*/
void modifyEntsArrowSize(const AcDbObjectIdArray &, const TextProperty &);

/* @�ӿ� �����޸�ʵ���ı��Ĵ�С(���ô˽ӿڲ���Ҫ��ס�ĵ���
 * @���� AcDbObjectIdArray �ı�ʵ�弯��
 * @���� double �ı��Ĵ�С
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��24��
 */
void modifyEntsTextHeight(const AcDbObjectIdArray &, double);

/* @�ӿ�
 * @���� 
 * @���� 
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��3��28��
 */
void modifyEntsCircleSize(const AcDbObjectIdArray &, double);

/* @�ӿ� ��ȡ��߽�
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��30��
 */
bool GetBD(const CString &, AcDbObjectId &, ads_point, bool bMax = false);

/* @�ӿ� ��ȡͼ���С
 * @���� char *ͼ���ļ�·��
 * @���� int ͼ��Ŀ�
 * @���� int ͼ��ĸ�
 * @���� bool ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��3��1��
 */
bool getImagePngSize(const char *, int &, int &);

//�����ע;
void AlignedNote(ads_point pt1, ads_point pt2, double dist = 0.0, double dOffset = 0.0);


bool IsScDMX(ads_name);
bool IsScDMX(const AcDbObjectId &);
bool IsSjDMX(ads_name);
bool IsSjDMX(const AcDbObjectId &);
bool IsScDlzxx(ads_name ent);
bool IsScDlzxx(AcDbObjectId id);
bool IsSjDlzxx(ads_name ent);
bool IsSjDlzxx(AcDbObjectId id);
bool IsScDlbx(ads_name ent);
bool IsScDlbx(AcDbObjectId id);
bool IsSjDlbx(ads_name ent);
bool IsSjDlbx(AcDbObjectId id);

/* @�ӿ� �޸�ʵ����ı�������չ����
 * @���� AcDbObjectId ���޸ĵ�ʵ��
 * @���� TCHAR ��չ���Ե�����
 * @���� TCHAR ��չ���Ե�ֵ
 * @���� TCHAR ���滻��ֵ
 * @���� bool �滻�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��3��24��
 */
bool changeTextByXData(AcDbObjectId &, const TCHAR *, const TCHAR *, const TCHAR *);

/* @�ӿ� �����޸�ʵ����ı�������չ����
 * @���� AcDbObjectIdArray ���޸ĵ�ʵ��
 * @���� TCHAR ��չ���Ե�����
 * @���� TCHAR ��չ���Ե�ֵ
 * @���� TCHAR ���滻��ֵ
 * @���� bool �滻�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��3��24��
 */
bool changeTextByXData(AcDbObjectIdArray &, const TCHAR *, const TCHAR *, const TCHAR *);

void modifyCGFrame();

void drawLineRemark(const AcDbObjectIdArray &, bool);

class CBackScreen
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��21��
	 */
	CBackScreen();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��21��
	 */
	~CBackScreen();

};

/* @�ӿ� ɾ��ͼ��������������ʵ��
 * @���� AcDbObjectIdArray ������ʵ��
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��5��22��
 */
void deleteOtherAllEntitys(const AcDbObjectIdArray &);

/* @�ӿ�
 * @���� 
 * @���� 
 * @���� ���
 * @���� 575814050@qq.com
 * @ʱ�� 2018��6��11��
 */
void addRoadHDMTuLi(const AcDbObjectId &, const CString &);

AcDbObjectId GetObjectIdByHandle(TCHAR *);

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��2��
 */
bool isLayerExistinDb(const CString &layer);

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��2��
 */
bool createLayer(const CString &layer);

class CloseLayer
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��16��
	 */
	CloseLayer(const CString &);

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��16��
	 */
	~CloseLayer();

private:

	CStringArray m_layers;

};

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��20��
 */
AcDbObjectId createBoundary(ads_point);

class LayerOperator
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	LayerOperator();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	~LayerOperator();

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��12��
	 */
	void opens(VStr &, bool);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	void closes(VStr &, bool);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��12��
	 */
	void open(const CString &, bool);
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	void close(const CString &, bool);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	bool frozen(const CString &, bool isfrozen = true);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��11��
	 */
	bool frozens(const VStr &, bool isfrozen = true);
	
};

class CmdEcho
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��6��
	 */
	CmdEcho();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��6��
	 */
	~CmdEcho();

};