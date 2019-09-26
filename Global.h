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

/*! \brief		获取几何实体中的几何中心
*  \param		nodes 包含了几何实体的顶点
*  \author		杨发荷
*  \date		[2015年08月27日 14:00]
*/
void GetMiddlePoint(AcGePoint2dArray & nodes, ads_point& pt);

/* @function 获取几何实体中的几何中心
* @param nodes 实体id
* @param AcGePointd2d实体集合中心
* @author		杨发荷
* @date		[2015年08月27日 14:00]
*/
void GetMiddlePoint(ads_name, ads_point &);

/* @function 获取几何实体中的几何中心
* @param nodes 实体id
* @param AcGePointd2d实体集合中心
* @author		杨发荷
* @date		[2015年08月27日 14:00]
*/
void GetMiddlePoint(const AcDbObjectId &id, AcGePoint2d &);

/* @function 获取几何实体中的几何中心
* @param nodes 实体id
* @param AcGePointd3d 实体集合中心
* @author		杨发荷
* @date		[2015年08月27日 14:00]
*/
void GetMiddlePoint(const AcDbObjectId &id, AcGePoint3d &);

bool InsertDwgFile(CString csPath);

//sType: 1 获取一般表格 2 获取查询设计类表格 3 获取所有表格
bool getAllTablesName(CAdoConnection *conn, vector<CString> &v_table_name, short sType = 1);

bool	checkLoadARX(const TCHAR *szArxName);

// 判断实体是否具有该扩展属性
bool HasXData(const AcDbObjectId& objId, const CString& csRegAppName);

bool AddEntsToGroup(const AcDbObjectId ObjId, AcDbObjectIdArray objArr);


bool setColorsEx(AcDbObjectIdArray objArr, Adesk::UInt16 nIndex);

int getDecimalPrec(CString strDecimal);

/*! \brief		根据组ID获取组内的所有实体
 *  \param
 *  \author		徐东江
 *  \date		[2015年06月03日 11:00]
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

//暂时写死，到时再从配置文件读取
CString getBuildStruct_FCS(AcDbObjectId objBuild);

//通过配置文件来获取房屋结构
CString getBuildStruct_FCS_Config(AcDbObjectId objBuild);

CString format_double_2(double dValue, int nDecimal);

//把指针传进来，记得外面要delete它
bool openXMLFile(TiXmlDocument *&tinyXml, TCHAR *szName);

bool readDefaultAreaParam();

void CreateLog(CString csPath);

void AddLog(CString csPath, CString csStr);

int AddXdataShort_bat(AcDbObjectIdArray objArr, const TCHAR *AppName, short nValue);

void AddXdataDouble_bat(AcDbObjectIdArray objArr, const TCHAR *AppName, double dValue);


bool ssFromLayers(AcDbObjectIdArray &objArr, const TCHAR *szLayer, bool bRemove);

bool checkDlgEditControlText(HWND hwnd, int nMaxCount, TCHAR *szSpecialWords = NULL);

//检查是否存在已重合的边界
bool IsPolyValid(AcGePoint2dArray &Nodes, CArray<AcGePoint2dArray, AcGePoint2dArray> &NodesArr);

extern bool	checkProjInfo();

/*! \brief		根据GIS表名过滤实体
 *  \param
 *  \author		徐东江
 *  \date		[2015年05月19日 12:00]
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

/* @function 获取多边形的西北角
* @param AcDbObjectId 多边形的实体id
* @return bool 成功获取返回true，否则返回值为false
* @author 杨发荷
* @time 2017年2月15号
*/
bool BeginWestNorth(const AcDbObjectId &);

/* @function 获取点到某条边的最近索引，索引值为起始索引，例如多边形中第三条边离pt最近，则返回的索引值为2
* @param AcGePoint2d 指定的点
* @param AcGePointdArray 多边形的点集
* @param int 用来返回索引值
* @author 杨发荷
* @time 2017年2月15号
*/
void getNearestEdge(const AcGePoint2d &, const AcGePoint2dArray &, int &);


/* @function 获取点到某条边的最近索引，索引值为起始索引，例如多边形中第三条边离pt最近，则返回的索引值为2
* @param AcGePoint2d 指定的点
* @param AcDbObjectId 多边形实体id
* @param int 用来返回索引值
* @author 杨发荷
* @time 2017年2月15号
*/
void getNearestEdge(const AcGePoint2d &, const AcDbObjectId &, int &);


/* @function 获取点到某条直线的中心距离
* @param AcGePoint2d 指定的点
* @param AcGePoint2d 直线的起始点
* @param AcGePoint2d 直线的终止点
* @return double 返回距离
* @author 杨发荷
* @time 2017年2月15号
*/
double getPoint2LineMidPointDist(const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &);

/* @function 获取点到直线的距离
* @param AcGePoint2d 指定的点
* @param AcGePoint2d 直线的起始点
* @param AcGePoint2d 直线的终止点
* @return double 返回距离
* @author 杨发荷
* @time 2017年2月15号
*/
double getPoint2LineDist(const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &);

/* @function 获取三个点之间的夹角如下图所示
							X(ptx)
							*
							*
							*
							*
							O(pto)*  *  *  *  *Y(pty)
							* @param ptx为夹角的一个顶点
							* @param pto为夹角的顶点
							* @param pty为夹角的另一个顶点
							* @return double 返回XOY之间的夹角（弧度制）
							* @author 杨发荷
							* @time 2017年2月22号*
							*/
double getIntersectionAngle(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pty);

/* @function 获取实体的每个角的角度
* @param AcDbObjectId 实体的ID
* @param vector<double> 角度的集合
* @author 杨发荷
* @time 2017年2月22号*
*/
void getIntersectionAngle(const AcDbObjectId &, vector<double> &);

/* @获取角度集合的角平分线
* @param angles角度的集合
* @param bisecs角平分线集合
* @param flag
* @author 杨发荷
* @time 2017年2月22号
*/
void getAngleBisection(const AcGePoint2dArray &, vector<double> &);

/* @获取某个实体的角平分线
* @param angles角度的集合
* @param bisecs角平分线集合
* @param flag
* @author 杨发荷
* @time 2017年2月22号
*/
void getAngleBisection(const AcDbObjectId &, vector<double> &);

/* @funtion 获取角平分线
* @param angle某个角度
* @param ptx 顶点x
* @param pto 夹角o
* @param pty 顶点y
* @return 返回夹角的角平分线
* @author 杨发荷
* @time 2017年2月22号*
*/
double getAngleBisection(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pty);

/* @获取角度集合的外角平分线
* @param angles 角度的集合
* @param bisecs 外角平分线集合
* @param flag
* @author 杨发荷
* @time 2017年2月22号
*/
void getOutAngleBisection(const AcGePoint2dArray &, vector<double> &);

/* @获取某个实体的外角平分线
* @param angles角度的集合
* @param bisecs 外角平分线集合
* @param flag
* @author 杨发荷
* @time 2017年2月22号
*/
void getOutAngleBisection(const AcDbObjectId &, vector<double> &);

/* @funtion 获取外角平分线
* @param angle某个角度
* @param ptx 顶点x
* @param pto 夹角o
* @param pty 顶点y
* @param bDeasil 如果点集是顺时针，则bDeasil为true，否则为false
* @return 返回外夹角的角平分线
* @author 杨发荷
* @time 2017年2月22号*
*/
double getOutAngleBisection(const AcGePoint2d &ptx, const AcGePoint2d &pto, const AcGePoint2d &pt);

/* @function 选择实体
* @param AcDbObjectId 被选择的实体id
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntity(AcDbObjectId &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectId 被选择的实体id
* @param SelectFilter 选择实体的过滤条件， 例如：实体是所在的图层是JZD图层，则
SelectFilter创建的方法为SelectFilter f2(8, _T("JZD"));
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntity(AcDbObjectId &, const SelectFilter &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectId 被选择的实体id
* @param SelectFilter 选择实体的过滤条件
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntity(AcDbObjectId &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectId 被选择的实体id
* @param SelectFilter 选择实体的过滤条件
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntity(AcDbObjectId &, const SelectFilter &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function 选择全部实体
* @param AcDbObjectIdArray 被选择的实体id集合
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntitys(AcDbObjectIdArray &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectIdArray 被选择的实体id集合
* @param SelectFilter 选择实体的过滤条件， 例如：实体是所在的图层是JZD图层，则
SelectFilter创建的方法为SelectFilter f2(8, _T("JZD"));
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntitys(AcDbObjectIdArray &, const SelectFilter &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectIdArray 被选择的实体id集合
* @param SelectFilter 选择实体的过滤条件
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntitys(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectIdArray 被选择的实体id集合
* @param SelectFilter 选择实体的过滤条件
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntitysOr(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectIdArray 被选择的实体id集合
* @param SelectFilter 选择实体的过滤条件
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntitysOr(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function 选择实体
* @param AcDbObjectIdArray 被选择的实体id集合
* @param SelectFilter 选择实体的过滤条件
* @param TCHAR 选择实体的方法
* @return bool 选择实体成功，返回值为true，否则返回值为false
* @author 杨发荷
* @time 2017年2月27号
*/
bool SelectEntitys(AcDbObjectIdArray &, const SelectFilter &, const SelectFilter &, const SelectFilter &, const TCHAR *);

/* @function 通过ID获取实体的句柄
* @param AcDbObjectId 实体的id
* @param bool 保留参数
* @return TCHAR 实体的句柄
* @author 杨发荷
* @time 2017年2月27号
*/
CString GetHandleByObject(const AcDbObjectId &, bool);

/* @function 通过实体的句柄获取实体的ID
* @param TCHAR 实体的句柄
* @return AcDbObjectId 实体的ID
* @author 杨发荷
* @time 2017年2月27号
*/
AcDbObjectId GetObjectByHandle(const TCHAR *);

/* @function 通过ID获取实体的句柄
* @param AcDbObjectId 实体的id
* @return CString 实体的句柄
* @author 杨发荷
* @time 2017年2月27号
*/
CString GetHandleByObject(const AcDbObjectId &);

/* @function 通过实体的句柄获取实体的ID
* @param CString 实体的句柄
* @return AcDbObjectId 实体的ID
* @author 杨发荷
* @time 2017年2月27号
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


/* @function 自动填充
* @param
* @return AcDbObjectId 实体的ID
* @author 陶蓉
* @time 2017年3月13号
*/
AcDbObjectId DrawHatch_Auto(ads_name ent, TCHAR * szHatch, double dScale, double dAngle,
	AcDbObjectIdArray *pislandIds, int nColor, TCHAR * szLayer);

/* @function 自动填充
* @param
* @return AcDbObjectId 实体的ID
* @author 陶蓉
* @time 2017年3月13号
*/
AcDbObjectId DrawHatch(ads_name ent, TCHAR * szHatch, double dScale, double dAngle,
	AcDbObjectIdArray *pislandIds, int nColor, TCHAR * szLayer);

/* @function 判断点pk是否在线段pi pj上
* @param
* @return
* @author 陶蓉
* @time 2017年3月15号
*/
bool PointAtLine(AcGePoint2d pi, AcGePoint2d pj, AcGePoint2d pk);


/* @function 计算向量pkpi和向量pjpi的叉积
* @param
* @return
* @author 陶蓉
* @time 2017年3月15号
*/
double direction(AcGePoint2d pi, AcGePoint2d pj, AcGePoint2d pk);


/* @function 判断线段p1p2和p3p4是否相交
* @param
* @return
* @author 陶蓉
* @time 2017年3月15号
*/
bool judgeIntersection(AcGePoint2d p1, AcGePoint2d p2, AcGePoint2d p3, AcGePoint2d p4); //


/* @function 用于图面获取一个多段线实体的一条边
* @param
* @return  传出用户交选框后的边的起点及终点的array（size = 2）
* @author 陶蓉
* @time 2017年3月16号
*/
AcGePoint2dArray getOneStraightSideOfEnt(AcDbObjectId &outId);

/* @function 绘制一个轴线注记
* @param
* @return  轴线注记id
* @author 陶蓉
* @time 2017年3月16号
*/
AcDbObjectId drawSingleAxisAnno(AcGePoint2d spt, CString annoText, double annoAngle);

/* @function 绘制一个轴线注记
 * @param
 * @return  轴线注记id
 * @author 陶蓉
 * @time 2017年3月16号
 */
AcDbObjectId drawSingleAxisAnno(AcGePoint2d spt, CString annoText, double annoAngle, const TextProperty &, const TextProperty &, const TextProperty &);


/* @function 获取点inPt在特定直线上的投影点
* @param linept 用于确定直线位置的直线上任意一点
* @param angle 直线的倾斜角
* @param inPt 要获取投影的输入点
* @return  投影点
* @author 陶蓉
* @time 2017年3月20号
*/
AcGePoint2d getProjectedPointOnLine(AcGePoint2d linept, double lineAngle, AcGePoint2d inPt);


/* @function 修改图块颜色
* @return
* @author 陶蓉
* @time 2017年3月24号
*/
void ChangeBlockRefenceColor(AcDbObjectId ObjId, int nRed, int nGreen, int nBlue);

/* @function 修改图块颜色
* @return
* @author 陶蓉
* @time 2017年3月24号
*/
void ChangeBlockRefenceColor(AcDbObjectId ObjId, int colorIndex);

/* @接口 此接口是为了打印哪个函数有问题
 * @作者 杨发荷
 * @时间 2017年08月24日
 */
void printErrorFunction(const char *);

/* @接口
* @作者 杨发荷
* @时间 2017年08月24日
*/
void printErrorInfo(const char *, const char *, int);

/* @接口
 * @作者 杨发荷
 * @时间 2017年08月24日
 */
void printErrorInfo(const char *, const char *, int, const char *);

bool OpenDWG(CString strPath);

/* @接口
 * @作者 杨发荷
 * @时间 2017年08月24日
 */
bool saveCurDwg(const TCHAR *);

/* @接口
 * @作者 杨发荷
 * @时间 2017年08月24日
 */
void openDwgCommand(const TCHAR *);

/* @接口
 * @作者 杨发荷
 * @时间 2017年08月24日
 */
void sendCommand(const TCHAR *);

class IDataBaseOper
{

	typedef CDatabaseOperManager DataBase;

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年08月24日
	 */
	IDataBaseOper();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年08月24日
	*/
	~IDataBaseOper();

	/* @接口 获取建筑竣工计容起算时间
	 * @作者 杨发荷
	 * @时间 2017年12月8号
	 */
	long readJRStartTime();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月19号
	 */
	CString readJGCommon(const CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月19号
	 */
	bool readJGCommon(const CString &, long &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月19号
	 */
	bool readJGCommon(const CString &, float &);

	/* @接口 读取数据库的内容
	 * @参数 DataBase *数据库对象指针
	 * @参数
	 * @参数
	 * @返回 初始化成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	bool readDataBase(const MStr &, MStr &);

	/* @接口 更新数据库的内容
	 * @参数 DataBase *数据库对象指针
	 * @参数
	 * @参数
	 * @返回 bool 执行成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年08月17日
	 */
	bool updataDataBase(const MStr &, const MStr &);

	/* @接口
	 * @参数
	 * @返回 bool 执行成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年08月22日
	 */
	bool readDataBase(const CString &, const MStr &, MStr &);

	/* @接口 
	 * @参数 
	 * @参数 
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月13号
	 */
	bool IDataBaseOper::readDataBase(const CString &table, const MStr &filter, VMStr &values);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @返回 bool 执行成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年08月23日
	 */
	bool readDataBase(const CString &, const CString &, CString &);

	/* @接口
	 * @参数
	 * @返回 bool 执行成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年08月22日
	 */
	bool readDataBase(const CString &, const CString &, const MStr &, VStr &);

	/* @接口
	 * @参数
	 * @返回 bool 执行成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年08月22日
	 */
	bool readDataBase(const CString &, const CString &, const MStr &, CString &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月23日
	 */
	CString readCommonTable(const CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年09月18日
	 */
	bool readCommonTable(const CString &, MStr &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月25日
	 */
	bool readCommonTable(const CString &, CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年08月25日
	 */
	bool readCommonTable(const CString &, int &);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年08月25日
	*/
	bool readCommonTable(const CString &, float &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月29日
	 */
	bool writeCommonTable(const CString &, int &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月29日
	 */
	bool writeCommonTable(const CString &, float &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月29日
	 */
	bool writeCommonTable(const CString &, const CString &);

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	bool clearPictureTable();

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	bool addPictureTableInfo(const CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	int getFieldCountByTable(const CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年09月11日
	 */
	bool readCommonTable(const CString &, const CString &, CString &);

	/* @接口 读取数据库中项目信息表
	 * @参数 CString 读取哪个业务
	 * @参数 MStr 读取的结果
	 * @返回 读取成功，返回值为true
	 * @作者 杨发荷
	 * @时间 2017年08月25日
	 */
	bool readProjectInfoTable(const CString &, MStr &);

	/* @接口
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年09月11日
	 */
	CString readPathTable(const CString &);

	/* @接口 读取文件路径表
	 * @参数 CString 被读取的文件路径
	 * @作者 杨发荷
	 * @时间 2017年09月18日
	 */
	CString readPathTableEx(const CString &);

	/* @接口 读取文件表，从而获取输出文件名
	 * @参数 CString 输出的文件的模板
	 * @返回 CString 输出文件名
	 * @作者 杨发荷
	 * @时间 2017年09月18日
	 */
	CString readFileNameTable(const CString &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月23日
	 */
	void readFunctionTable(FunctionNameTables &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月23日
	 */
	void saveFunctionTable(const FunctionNameTables &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void readHeightTable(MVMStr &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void writeHeightTable(const MVMStr &);

	/* @接口 此接口在程序初始化时已经调用
	 * @作者 杨发荷
	 * @时间 2017年10月14日
	 */
	bool readFieldChangeTable();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	bool readCarTable(MStr &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年12月4号
	 */
	CString readBGTable(const CString &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年12月4号
	 */
	void removeBGTableInfo(const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月31日
	 */
	bool readCompressTable(const CString &, VStr &);

	/* @接口 读取TextProperty表
	 * @参数 CString 读取的名称
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月25号
	 */
	TextProperty readTextPropertyTable(const CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年12月4号
	 */
	void AddBGInfoTable(const CString &, const CString &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年12月2号
	 */
	TextProperty readTextPropery(const CString &, bool size = false, bool font = false);

	/*读取绘制管线设置表
	*/
	void readPipelineTable(PipeLineTables &fnts);

	/*保存绘制管线设置表
	*/
	void savePipelineTable(const PipeLineTables &fnts);

	/*获取管点编码
	*/
	void readPipePointBM(const CString &strGDLX, CString &strGDBM);

	/*获取管线编码
	*/
	bool readPipelineDM(const CString &strGXLX, CString &strGXDM);
	
	/*获取管线编码
	*/
	bool readPipelineBM(const MStr &, CString &strGXDM);

	/*获取管点对应的块编码 
	 */
	void readPipePointKBM(const CString &strGDLX, CString &strKBM);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月20号
	 */
	CString readPipeLineTable(const CString &, const MStr &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月13号
	 */
	MStr readOutputFormatTable(const CString &);

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月25号
	 */
	VStr readAllPipeLineLayer();

private:

	DataBase *_db;

};

class IProjectMDB
{
	typedef CDatabaseOperManager DataBase;

public:

	/* @接口 默认构造接口
	 * @作者 杨发荷
	 * @时间 2017年11月3日
	 */
	IProjectMDB();

	/* @接口 默认析构接口
	 * @作者 杨发荷
	 * @时间 2017年11月3日
	 */
	~IProjectMDB();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	MStr getBridgeXX();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月21号
	 */
	MStr getProjectInfo();
	
	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月23号
	 */
	VMStr getBGInfoTable();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	CString getClearWidth();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	VMStr getCXXTableInfo();

	/* @接口 读取技术说明表
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	MStr getJSSMTableInfo();

	/* @接口 面积信息表
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	MStr getMJXXTableInfo();

	/* @接口
	 * @作者 杨发荷
	 * @返回 VMStr 返回图片信息表
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	VMStr getPictureTable();

	/* @接口 面积信息表
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	MStr getBGXXTableInfo();

	/* @接口
	 * @作者 杨发荷
	 * @返回 MStr 返回基本信息表信息
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	MStr getJBXXTableInfo();

	/* @接口
	 * @作者 杨发荷
	 * @返回 VMStr 获取栋信息
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	VMStr getDXXTableInfo();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月3日
	 */
	CString getProjectName();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年12月5号
	 */
	bool clearPictureTable();

	/* @接口 获取桥梁净高
	 * @返回 CString 桥梁净高
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	CString getClearHeight();

	/* @接口 获取天桥的宽度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	CString getBridgeWidth();

	/* @接口 获取天桥的长度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	CString getBridgeLength();

	/* @接口 获取道路标高
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月28号
	 */
	CString getLoadLevation();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月28号
	 */
	CString getBridgeLevation();

	/* @接口 获取天桥厚度
	 * @返回 CString天桥的厚度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	CString getBridgeThickness();

	/* @接口 获取天桥栏杆高度
	 * @返回 CString天桥栏杆的高度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	CString getBridgeBalustrade();

	/* @接口
	* @参数
	* @作者 杨发荷
	* @时间 2018年1月4号
	*/
	bool setBridgeXX(const MStr &);
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月18号
	 */
	VMStr getGNQXXInfo(const MStr &);
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月18号
	 */
	MStr getGNQXXInfoEx(const MStr &);
	
	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月12号
	 */
	void delCXXTableInfo(const MStr &);

	/* @接口 设置层信息
	 * @参数 MStr层信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void addCXXTableInfo(const MStr &);

	/* @接口 获取层信息
	 * @参数 MStr过滤条件
	 * @作者 杨发荷
	 * @时间 2018年2月26号
	 */
	MStr getCXXTableInfo(const MStr &);

	/* @接口 获取栋信息
	 * @参数 MStr栋信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	MStr getDXXTableInfo(const MStr &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	MStr getTableInfo(const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月26号
	 */
	void setCXXTableInfo(const MStr &);

	/* @接口 设置桥梁净宽
	 * @参数 CString桥梁净宽
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	bool setClearWidth(const CString &);

	/* @接口 面积信息表
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	void setBGXXTableInfo(const MStr &);

	/* @接口 读取技术说明表
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	void setJSSMTableInfo(const MStr &);

	/* @接口 设置基本信息表信息
	 * @参数 MStr基本表信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void setJBXXTableInfo(const MStr &);

	/* @接口 面积信息表
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	void setMJXXTableInfo(const MStr &);

	/* @接口 根据层号清除层信息表所有信息
	 * @参数 CString清除的层号
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void clearCXXTable(const CString &);

	/* @接口 设置天桥的宽度
	 * @参数 CString 天桥的宽度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	bool setBridgeWidth(const CString &);

	/* @接口 设置桥梁净高
	 * @参数 CString 桥梁净高值
	 * @返回 bool 设置成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	bool setClearHeight(const CString &);

	/* @接口 删除标高信息
	 * @参数 CString 删除指定的标高信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void delBGTableInfo(const CString &);

	/* @接口 设置道路标高
	 * @参数 CString 道路标高
	 * @返回 设置成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月28号
	 */
	bool setLoadLevation(const CString &);

	/* @接口 设置天桥的长度
	 * @参数 CString 天桥的长度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	bool setBridgeLength(const CString &);
	
	/* @接口 获取层信息
	 * @参数 MStr过滤条件
	 * @作者 杨发荷
	 * @时间 2018年2月26号
	 */
	VMStr getCXXTableInfoEx(const MStr &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月25日
	 */
	bool updataPictureTable(const VMStr &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月26号
	 */
	VMStr getCXXTableInfo(const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月3日
	 */
	CString getProjectInfo(const CString &);

	/* @接口 设置桥面标高
	 * @参数 CString 桥面标高
	 * @返回 设置成功，返回值为true， 否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月28号
	 */
	bool setBridgeLevation(const CString &);

	/* @接口 设置天桥厚度
	 * @参数 CString 天桥的厚度
	 * @返回 bool 设置成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	bool setBridgeThickness(const CString &);
	
	/* @接口 设置天桥栏杆高度
	 * @参数 CString 天桥栏杆的高度
	 * @返回 bool 设置成功返回值为true， 否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	bool setBridgeBalustrade(const CString &);

	/* @接口 设置通道主体高度
	 * @参数 MStr 通道主体高度
	 * @返回 bool设置成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void setChannelBodyPosition(const MStr &);
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年10月24号
	 */
	bool initMDB(const CString &path = _T(""));

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月18号
	 */
	void addGNQXXInfo(const MStr &, const MStr &);

	/* @接口 根据指定类型获取通道主体值
	 * @参数 CString 通道主体位置类型
	 * @返回 CString 通道主体位置值
	 * @作者 杨发荷
	 * @时间 2018年2月26号
	 */
	CString getChannelBodyPosition(const CString &);

	/* @接口 设置层信息
	 * @参数 MStr过滤条件
	 * @参数 MStr设置的层信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void setCXXTalbeInfo(const MStr &, const MStr &);

	/* @接口 设置栋信息
	 * @参数 MStr过滤条件
	 * @参数 MStr设置的栋信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void setDXXTalbeInfo(const MStr &, const MStr &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月21号
	 */
	void setProjectInfo(const CString &, const MStr &);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void updataCXXTableInfo(const MStr &, const MStr &);
	
	/* @接口 获取标高信息
	 * @参数 CString 获取的标高类型
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	CString getBGTable(const CString &, const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月3日
	 */
	void setProjectInfo(const CString &, const CString &);

	/* @接口 删除表中的记录
	 * @参数 CString 删除指定的表
	 * @参数 MStr 要删除的记录
	 * @返回 bool 删除成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	bool deleteTableRecord(const CString &, const MStr &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	bool updataBridgeXX(const CString &, const CString &);

	/* @接口 设置标高信息
	 * @参数 CString 设置标高字段
	 * @参数 CString 设置的标高值
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void setBGInfoTable(const CString &, const CString &, const CString &);

	/* @接口 设置标高信息
	 * @参数 CString 设置标高字段
	 * @参数 CString 设置的标高值
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void setBGInfoTable(const CString &, const CString &, const CString &, const CString &, const CString &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月15号
	 */
	void setBGGUID(const CString &);
	
	/* @接口 设置通道主体位置
	 * @参数 CString 设置通道主体的类型（通道主体起点X、通道主体起点Y、通道主体终点X、通道主体终点Y）
	 * @参数 CString 通道主体位置的值
	 * @返回 bool 设置成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	bool setChannelBodyPosition(const CString &, const CString &);

	/*保存PipeLine到project.mdb
	*/
	void setPipeLineTable(const CString &table, const VMStr &record);

	/*获取PIPELINE表中strGxlx管线类型的颜色
	*/
	bool getPipelineColor(const CString &strGxlx, int &nR, int &nG, int &nB);

	bool getPipelineColor(const CString &strGxlx, CString &strColor);

	/*更改连接数据库
	*/
	BOOL ChangeConnection(const CString &srtPath);

	/*获取PIPELINE表
	*/
	VMStr getPipeTable();

	/* @接口 设置道路信息
	 * @参数 MStr 道路信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	bool setRoadInfo(const CString &, const GHSSJBQKInfo &);
	
	/* @接口 设置道路信息
	 * @参数 MStr 道路信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	bool setRoadInfo(const CString &, const GHSSJBQKPipe &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void getRoadInfo(const CString &, GHSSJBQKInfo &);
	
	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void getRoadInfo(const CString &, GHSSJBQKPipe &);

	/* @接口 设置RXBridgeInfo表信息
	 * @时间 2018年4月25日
	 */
	void setRXBridgeTableInfo(const VMStr &records);

	/* @接口 读取RXBridgeInfo表信息
	 * @时间 2018年4月25日
	 */
	VMStr readRXBridgeTableInfo();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年7月4号
	 */
	VStr getAllRoadName();

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月8号
	 */
	int getRecordCount(const CString &, const MStr &);

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月9号
	 */
	bool hasTableRow(const CString& table, const MStr &field_value, VMStr &selectRow);

private:

	DataBase *db;

};

/* @接口
 * @参数
 * @参数
 * @参数
 * @返回 读取成功，返回值为true，否则返回值为false
 * @作者 杨发荷
 * @时间 2017年9月26日
 */
bool getLayerNum(const CString &, int &, int &);

/* @接口
 * @参数
 * @参数
 * @参数
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月25号
 */
void getJGLayerNum(const CString &, int &, int &);

/* @接口
 * @参数
 * @参数
 * @参数
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月25号
 */
void getStrAndNumByString(const CString &, CString &, int &);

bool closeDWG(const CString &);

void splitBigString(const CString &, string &);

#define ErrorInfo() printErrorInfo(__FILE__, __FUNCTION__, __LINE__)

/* @接口 对实体进行排序
 * @参数 AcDbObjectIdArray 实体id集合
 * @参数 CString 扩展属性名称
 * @参数 del 如果实体不包含扩展属性名称，那是否删移除该实体(默认移除)
 * @作者 杨发荷
 * @时间 2017年11月16号
 */
void sortObjectByXDataInt(AcDbObjectIdArray &, const VStr &, bool del = true);

/* @接口 对实体进行排序
 * @参数 AcDbObjectIdArray 实体id集合
 * @参数 CString 扩展属性名称
 * @参数 del 如果实体不包含扩展属性名称，那是否删移除该实体(默认移除)
 * @作者 杨发荷
 * @时间 2017年11月16号
 */
MSIds sortGNQByXData(const AcDbObjectIdArray &);//按照层高和计容系数进行功能区分类

/* @接口
 * @作者 杨发荷
 * @时间 2017年11月16号
 */
void sortObjectByXData(AcDbObjectIdArray &, const CString &, const CString &);

/* @接口
 * @参数
 * @作者 杨发荷
 * @时间 2017年11月20号
 */
bool stringIsDigit(const CString &);

/* @接口
* @作者 杨发荷
* @时间 2017年11月22号
*/
void convertDwg2Pdf(const CString &);

/* @接口
* @作者 杨发荷
* @时间 2017年11月22号
*/
void convertDwg2WMF(const CString &);

/* @接口
 * @参数
 * @参数
 * @参数
 * @作者 杨发荷
 * @时间 2017年11月26号
 */
void getLayerArea(const AcDbObjectId &, double &, double &, double);

/* @接口 获取车位的个数
 * @作者 杨发荷
 * @时间 2017年12月4号
 */
void getCarCount(const AcDbObjectId &id, MStr &, double &, double &);

/* @接口
 * @参数
 * @作者 杨发荷
 * @时间 2017年11月26号
 */
double getLayerJZMJArea(const AcDbObjectId &);

/* @接口
 * @参数
 * @作者 杨发荷
 * @时间 2017年11月26号
 */
double getLayerJRMJArea(const AcDbObjectId &);

/* @接口
 * @参数
 * @作者 杨发荷
 * @时间 2017年12月1号
 */
short isEntClosed(const AcDbObjectId &);

/* @接口
 * @参数
 * @参数
 * @作者 杨发荷
 * @时间 2017年12月1号
 */
bool SetLineWidth(const AcDbObjectId &, const double);		///< 设置线宽

/* @接口 获取实体内部的实体
 * @参数 AcDbObjectId 实体
 * @作者 杨发荷
 * @时间 2017年12月1号
 */
AcDbObjectIdArray getInnerObjects(const AcDbObjectId &);

/* @接口
 * @参数
 * @参数
 * @参数
 * @返回 读取成功，返回值为true，否则返回值为false
 * @作者 杨发荷
 * @时间 2017年10月10日
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

/* @接口 半径标注
 * @参数 AcDbObjectId 需要标注的实体
 * @参数 double 标注引线长度
 * @返回 AcDbObjectIdArray 半径标注所有实体
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年2月8号
 */
AcDbObjectIdArray AddRaidusNote(const AcDbObjectId &id, double);

/* @接口 半径标注
* @参数 AcDbObjectId 需要标注的实体
* @参数 double 标注引线长度
* @返回 AcDbObjectIdArray 半径标注所有实体
* @作者 杨发荷
* @邮箱 575814050@qq.com
* @时间 2018年2月8号
*/
AcDbObjectIdArray AddRaidusNote(const AcDbObjectId &id, double, const TextProperty &);

void mulitNodeAnnotation(const AcDbObjectIdArray &, const TextProperty &);

bool GetPrinterPath(CString &);

void UpdatePrinterConfig();

/* @接口 获取系统的版本
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月24号
 */
int getSystemVersion();

/* @接口 修改实体的箭头的大小(调用此接口之前首先要锁住文档）
 * @参数 AcDbObjectId 箭头实体的id
 * @参数 double 箭头的大小
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月24号
 */
void modifyEntArrowSize(const AcDbObjectId &, double);

/* @接口
 * @参数 
 * @参数 
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年3月28号
 */
void modifyEntCircleSize(const AcDbObjectId &, double);

/* @接口 修改实体的文本大小(调用此接口之前首先要锁住文档）
 * @参数 AcDbObjectId 文本实体的id
 * @参数 double 文本的大小
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月24号
 */
void modifyEntTextHeight(const AcDbObjectId &, double);

/* @接口 批量修改箭头的大小(调用此接口不需要锁住文档）
 * @参数 AcDbObjectIdArray 箭头实体集合
 * @参数 箭头的大小
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月24号
 */
void modifyEntsArrowSize(const AcDbObjectIdArray &, double);

/* @接口 批量修改箭头的大小(调用此接口不需要锁住文档）
* @参数 AcDbObjectIdArray 箭头实体集合
* @参数 箭头的大小
* @作者 杨发荷
* @邮箱 575814050@qq.com
* @时间 2018年1月24号
*/
void modifyEntsArrowSize(const AcDbObjectIdArray &, const TextProperty &);

/* @接口 批量修改实体文本的大小(调用此接口不需要锁住文档）
 * @参数 AcDbObjectIdArray 文本实体集合
 * @参数 double 文本的大小
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月24号
 */
void modifyEntsTextHeight(const AcDbObjectIdArray &, double);

/* @接口
 * @参数 
 * @参数 
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年3月28号
 */
void modifyEntsCircleSize(const AcDbObjectIdArray &, double);

/* @接口 获取外边界
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年1月30号
 */
bool GetBD(const CString &, AcDbObjectId &, ads_point, bool bMax = false);

/* @接口 获取图像大小
 * @参数 char *图像文件路径
 * @参数 int 图像的宽
 * @参数 int 图像的高
 * @返回 bool 获取成功，返回值为true，否则返回值为false
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年3月1号
 */
bool getImagePngSize(const char *, int &, int &);

//对齐标注;
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

/* @接口 修改实体的文本根据扩展属性
 * @参数 AcDbObjectId 被修改的实体
 * @参数 TCHAR 扩展属性的名称
 * @参数 TCHAR 扩展属性的值
 * @参数 TCHAR 被替换的值
 * @返回 bool 替换成功，返回值为true，否则返回值为false
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年3月24号
 */
bool changeTextByXData(AcDbObjectId &, const TCHAR *, const TCHAR *, const TCHAR *);

/* @接口 批量修改实体的文本根据扩展属性
 * @参数 AcDbObjectIdArray 被修改的实体
 * @参数 TCHAR 扩展属性的名称
 * @参数 TCHAR 扩展属性的值
 * @参数 TCHAR 被替换的值
 * @返回 bool 替换成功，返回值为true，否则返回值为false
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年3月24号
 */
bool changeTextByXData(AcDbObjectIdArray &, const TCHAR *, const TCHAR *, const TCHAR *);

void modifyCGFrame();

void drawLineRemark(const AcDbObjectIdArray &, bool);

class CBackScreen
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月21号
	 */
	CBackScreen();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月21号
	 */
	~CBackScreen();

};

/* @接口 删除图面上其他的所有实体
 * @参数 AcDbObjectIdArray 保留的实体
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年5月22号
 */
void deleteOtherAllEntitys(const AcDbObjectIdArray &);

/* @接口
 * @参数 
 * @参数 
 * @作者 杨发荷
 * @邮箱 575814050@qq.com
 * @时间 2018年6月11号
 */
void addRoadHDMTuLi(const AcDbObjectId &, const CString &);

AcDbObjectId GetObjectIdByHandle(TCHAR *);

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月2号
 */
bool isLayerExistinDb(const CString &layer);

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月2号
 */
bool createLayer(const CString &layer);

class CloseLayer
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月16号
	 */
	CloseLayer(const CString &);

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月16号
	 */
	~CloseLayer();

private:

	CStringArray m_layers;

};

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月20号
 */
AcDbObjectId createBoundary(ads_point);

class LayerOperator
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	LayerOperator();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	~LayerOperator();

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月12号
	 */
	void opens(VStr &, bool);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	void closes(VStr &, bool);

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月12号
	 */
	void open(const CString &, bool);
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	void close(const CString &, bool);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	bool frozen(const CString &, bool isfrozen = true);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月11号
	 */
	bool frozens(const VStr &, bool isfrozen = true);
	
};

class CmdEcho
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月6号
	 */
	CmdEcho();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月6号
	 */
	~CmdEcho();

};