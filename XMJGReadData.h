#ifndef XMJGREADDATA
#define XMJGREADDATA

#include <tchar.h>
#include <vector>
#include <map>
#include <fstream>
#include "XMJGDialog.h"
#include "XMJGTool.h"
#include "XMJGPipeLine.h"

using namespace std;

#pragma warning( disable : 4278 )
// Makes change to the tlb name based on the AutoCAD version.
// For ex : acax18enu.tlb for AutoCAD 2010/2011 and 2012
//          acad19enu.tlb for AutoCAD 2013 
#if _MSC_VER == 1500
#import "acax18ENU.tlb" no_implementation raw_interfaces_only named_guids
#endif
#pragma warning( default : 4278 )

#include <acadi.h>

#ifndef XMJGMACRO
#include "XMJGMacro.h"
#endif /*XMJGMACRO*/

#include <tcode.h>

class SBreakBlock;
class AcDbPlotSettings;
class CDatabaseOperManager;

class IProjectInfo
{
public:

	/* @接口 默认构造接口
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	IProjectInfo(const CString &);

	~IProjectInfo();

	bool CreateProject(const MStr &);

	bool CreateProject(const CString &, const MStr &);

	bool CreateProject(const MStr &, CString &);

	bool CreateProject(const CString &, const CString &);

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年5月24号
	 */
	bool CreateProjectEx(const MStr &, const CString &);

	bool CreateProjectEx(const MStr &, CString &, const CString &);

private:

	void updataProjectInfo(CString = _T(""));

	void updataDWG(CString = _T("GC\\项目图形.dwg"));

	void createProjectDir(CString = _T("GC\\项目图形.dwg"));

	void createProjectDirEx(const CString &, CString = _T("GC\\项目图形.dwg"));

private:

	MStr m_record;
	CString m_dir;
	CString m_name;
	CString m_code;
	CString m_type;
	CDatabaseOperManager *_manager;

};

/* @class 厦门竣工第一个业务类
* @author 杨发荷
* @time 2017年3月
*/
class XMJGRedLineSettingOut
{
public:

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	XMJGRedLineSettingOut();

	/* @deconstruction 默认析构函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	~XMJGRedLineSettingOut();

public:

	/* @接口 导入数据
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void ImportData();

	/* @接口 设置属性
	 * @作者 杨发荷
	 * @时间 2017年11月17号
	 */
	void setProperty();

	/* @接口 创建工程
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void CreateRLProject();

	/* @function 读取dat文件
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void ReadDatFile();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月28日
	 */
	void ReadReveseDat();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年08月28日
	 */
	void ReadReveseExcel();

	/* @function 读取Excel文件
	* @author 杨发荷
	* @time 2017年3月7号
	*/
	void ReadExcelFile();

	/* @function 读取Excel文件
	* @author 杨发荷
	* @time 2017年3月7号
	*/
	void ManualDrawPoint();

	/* @function 绘制红线线
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void DrawRedLine();

	/* @function给红线添加节点
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void AddRedLineNode();

	/* @function 删除红线节点
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void DelRedLineNode();

	/* @function 删除红线节点（增强版）
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void DelRedLineNodeEx();

	/* @function 手动调整节点顺序
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void AdjustNodesOrder();

	/* @function 手动添加注记
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void AddAnnotation();

	/* @function 手动添加注记
	* @author 杨发荷
	* @time 2017年2月
	*/
	void ManualAnnotation();

	/* @function 自动添加注记
	* @author 杨发荷
	* @2017年2月
	*/
	void AutoAnnotation();

	/* @function 移动注记
	* @author 杨发荷
	* @time 2017年2月
	*/
	void MoveAnnotation();

	/* @function 手动添加扯线注记
	* @author 陶蓉
	* @time 2017年2月
	*/
	void ManualLineAnnotation();

	/* @function 绘制范围线
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void DrawScopeLine();

	/* @function 设置信息
	* @author 陶蓉
	* @time 2017年3月
	*/
	void RedlineSetAttatchedInfo();

	/* @function 读取注册表信息，并将信息替换到块中
	* @author 杨发荷
	* @param SBreakBlock 实体块
	* @time 2017年3月13号
	*/
	void ReadRegisterValue(SBreakBlock *);

	/* @function 读取注册表信息，并将信息替换到块中
	* @author 杨发荷
	* @param SBreakBlock 实体块
	* @time 2017年3月13号
	*/
	void GetReplaceValue(MStr &);

	/* @function 输出图面实体到dwg中
	* @author 杨发荷
	* @time 2017年3月
	*/
	void RedLineExportDWG();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月21日
	 */
	void InputCoorPos();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月18号
	 */
	void readLineAddNode();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月7号
	 */
	void redDistAnnotion();

private:

	/* @接口 删除坐标标注
	 * @参数 AcDbObjectId 标注的实体
	 * @作者 杨发荷
	 * @时间 2017年07月18日
	 */
	void delAnnotation(AcDbObjectId &);

	/* @接口 注记一个实体所有节点
	 * @参数 AcDbOjbectId 被注记的实体
	 * @参数 double 注记的长度
	 * @作者 杨发荷
	 * @时间 2017年07月18日
	 */
	void autoAnnotation(AcDbObjectId &, double);

	/* @接口 获取点坐标
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool getInputPoint(AcGePoint2d &);

	/* @接口 获取点坐标
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool getInputManual(AcGePoint2d &);

	/* @接口 获取点坐标
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool getInputScreen(AcGePoint2d &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void purgeTxtDataFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void purgeDatDataFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void purgeExcelDataFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void drawPoint(const VPInfo &);

private:

	double m_Scale;
	CString m_Code;
	CString m_SaveDir;
	TextProperty m_hxjdzj;	//红线节点
	TextProperty m_ywcd;	//野外测点注记
	TextProperty m_ywcdtk;	//野外测点图块

};

/* @class 厦门竣工第二个业务类
* @author 杨发荷
* @time 2017年3月
*/
class XMJGBuildSettingOut
{

public:

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	XMJGBuildSettingOut();

	/* @deconstruction 默认析构函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	~XMJGBuildSettingOut();

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void CreateBOSProject();

	/* @function 将含有红线的dwg插入到当前图面中
	* @author 杨发荷
	* @time 2017年3月
	*/
	void InsertRedLineDwg();

	/* @function 添加放样符号
	* @author 杨发荷
	* @time 2017年3月
	*/
	void AddOutLineSymbol();

	/* @接口 绘制房屋范围线
	 * @作者 杨发荷
	 * @时间 2017年07月04日
	 */
	void DrawBuildBoundaries();

	/* @function 显示放样符号
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ShowOutLineSymbol();

	/* @function 隐藏放样符号
	* @author 杨发荷
	* @time 2017年3月
	*/
	void HideOutLineSymbol();

	/* @function 设置注记属性
	* @author 杨发荷
	* @time 2017年3月
	*/
	void SetAnnotationAttribute();

	/* @function 改变方位顺序
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ChangeAzimuthOrder();

	/* @function 改变所有的方位顺序
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ChangeAzimuthOrderAll();

	/* @function 设置信息
	* @author 杨发荷
	* @time 2017年3月
	*/
	void SetAttatchedInfo();

	/* @function 将实体输出到dwg
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ExportToDWG();

	/* @function 将实体输出到dwg
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ExportToDWG2();

	/* @function 输出doc文档
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ExportToDOC();

	/* @function 读取注册表信息，并将信息替换到块中
	* @author 杨发荷
	* @param SBreakBlock 实体块
	* @time 2017年3月13号
	*/
	void GetReplaceValue(MStr &);

	/* @function 读取注册表信息，并将word里面的标识为替换
	* @param word 被替换的word
	* @param char * 在外面替换的信息
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ReadRegisterValue(Word *);

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年08月25日
	 */
	void splitMulLineText(MStr &, const CString &, const CString &);

	/* @接口 将建筑范围线所构成的面积添加到实体中
	 * @参数 ads_name 建筑范围线
	 * @作者 杨发荷
	 * @时间 2017年07月04日
	 */
	void AddZDMJ2BuildEntity(ads_name);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月21日
	 */
	void DrawBuildNearestBoundaries();

	/* @接口 修改建筑物名称
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void ModifyBuildName();

	/* @接口 修改建筑宽度
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void modifyBuildWidth();

private:

	double m_Scale;
	CString m_SaveDir;
	TextProperty m_jzwlk;
	TextProperty m_jzwmc;

};

/* @class 厦门竣工第三个业务类
* @author 杨发荷
* @time 2017年3月
*/
class XMJGRoadSettingOut
{
public:

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年3月14号
	*/
	XMJGRoadSettingOut();

	/* @deconstruction 默认析构函数
	* @author 杨发荷
	* @time 2017年3月14号
	*/
	~XMJGRoadSettingOut();

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void CreateRSOProject();

	/* @function 绘制道路边线
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void DrawRoadEdgeLine();

	/* @function 绘制道路中心线
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void DrawRoadCentreLine();

	/* @function 输出坐标表（道路中心线）
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void ExportCoodinateTable();

	/* @function 标注起点终点坐标
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void SetRoadText();

	/* @function 附属信息设置
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void SetAttatchedInfo();

	/* @function DWG成果输出
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void ExportDWGOutput();

	/* @function WORD成果输出
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void ExportWORDOutput();

	/* @function 标注质检
	* @author 陶蓉
	* @time 2017年2月27号
	*/
	void TextDataCheck();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月16日
	*/
	void SetRoadStartEndPoint();

private:

	/* @function 读取注册表信息，并将信息替换到块中
	* @author 杨发荷
	* @param SBreakBlock 实体块
	* @time 2017年3月13号
	*/
	void ReadRegisterValue(SBreakBlock *);

	/* @function 读取注册表信息，并将信息替换到块中
	* @参数 MStr
	* @author 杨发荷
	* @param SBreakBlock 实体块
	* @time 2017年3月13号
	*/
	void GetReplaceValue(MStr &);

	/* @function 读取注册表信息，并将word里面的标识为替换
	* @param word 被替换的word
	* @param char * 在外面替换的信息
	* @author 杨发荷
	* @time 2017年3月
	*/
	void ReadRegisterValue(Word *);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void GetTableRowAndCol(int &, int &, int &);

	/* @接口 判断点是否在点的数组中
	 * @参数 AcGePoint2d 需要判断的点
	 * @参数 AcGePoint2dArray 节点数组
	 * @返回 如果点在节点数组中，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年06月22日
	 */
	bool JudgePointInNodes(const AcGePoint2d &, const AcGePoint2dArray &);

	/* @接口 将点的坐标添加实体中
	 * @参数 AcDbObjectId 实体ID
	 * @参数 ads_point 被添加的点
	 * @作者 杨发荷
	 * @时间 2017年06月22日
	 */
	void AddCoorInfoToEntity(AcDbObjectId &, const AcGePoint2d &, const AcGePoint2d &);

	/* @接口 获取要注记的点
	 * @参数 AcDbObjectIdArray 要注记的实体集合
	 * @参数 AcGePoint2dArray 所有的待注记的点集
	 * @作者 杨发荷
	 * @时间 2017年08月24日
	 */
	void getAnnotationPoint(AcDbObjectIdArray &, AcGePoint2dArray &);

	/* @接口 获取所有注记的信息
	* @参数 AcGePoint2dArray 所有的待注记的点集
	* @参数 VVStr 保存了表中的信息
	* @参数 int 表格的行数
	* @参数 int 表格的列数（一个点为一列）
	* @作者 杨发荷
	* @时间 2017年08月24日
	*/
	void getRoadTableInfo(const AcGePoint2dArray &, VVStr &, int, int);

	/* @接口 获取所有注记的信息
	 * @参数 AcGePoint2dArray 所有的待注记的点集
	 * @参数 VVStr 保存了表中的信息
	 * @参数 int 表格的行数
	 * @参数 int 表格的列数（一个点为一列）
	 * @作者 杨发荷
	 * @时间 2017年08月24日
	 */
	void getRoadTableInfo(const AcDbObjectIdArray &, VVStr &, int, int);

	/* @接口 获取所有注记的信息,新需求要求只输出有控制点的坐标
	 * @时间 2018年4月13日
	 */
	void getRoadTableInfo(vector<pair<AcDbObjectId, CString>> &vKZD, VVStr &, int, int);

	/* @接口 添加道路中心线的注记
	 * @参数 AcGePoint2d 添加道路线节点编号
	 * @参数 int 总体编号
	 * @作者 杨发荷
	 * @时间 2017年08月24日
	 */
	void addAnnotationCode(const AcGePoint2d &, int);

	/* @接口 获取道路中心线上的控制点
	 * @时间 2018年4月13日
	 */
	BOOL GetControlPoint(const AcGePoint2dArray &nodes, vector<pair<AcDbObjectId, CString>> &KZD);
private:

	TextProperty m_ywcdtk;	//野外测点图块
	int m_bhcolor;			//道路编号注记颜色
	int m_zjcolor;			//道路首尾注记颜色
	float m_bhfont;			//道路编号注记大小
	float m_zjfont;			//道路首位节点大小
	double m_Scale;
	CString m_SaveDir;
	CString m_bhlayer;		//道路编号注记图层
	CString m_zjlayer;		//道路首尾注记图层
	TextProperty m_dlzxx;	//道路中心线
	TextProperty m_dlbx;	//道路边线
};

/* @class 厦门竣工第四个业务类
* @author 杨发荷
* @time 2017年3月
*/
class  XMJGEvelingElevation
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	XMJGEvelingElevation();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	~XMJGEvelingElevation();

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void createProject();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void AxisAutoAnnotion1();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void SetAttatchedInfo();

	/* @接口 输出实体到dwg（样式1）
	* @作者 杨发荷
	* @时间 2017年06月19日
	*/
	void ExportResult2DWG1();

	/* @接口 输出实体到dwg（样式2）
	* @作者 杨发荷
	* @时间 2017年06月19日
	*/
	void ExportResult2DWG2();

private:

	/* @接口 读取注册表获取被替换的值
	* @参数 MStr 被替换的值
	* @作者 杨发荷
	* @时间 2017年06月19日
	*/
	void GetReplaceValue(MStr &);

	/* @接口 获取控制点的个数
	* @参数 控制点的个数
	* @作者 杨发荷
	* @时间 2017年06月19日
	*/
	int GetControlPointCount(MStr &);

private:

	double m_Scale;
	CString m_SaveDir;

};

/* @class 厦门竣工第五个业务类
* @author 杨发荷
* @time 2017年3月
*/
class XMJGCheckLine
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	XMJGCheckLine();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	~XMJGCheckLine();

public:

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void createProject();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月20日
	*/
	void DrawSurroundingBuild();

	/* @接口 注记预处理
	 * @作者 杨发荷
	 * @时间 2017年06月22日
	 */
	void AnnPretreatment();

	void AxisAutoAnnotion1();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void AxisAutoAnnotion();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void AxisManualAnnotion();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void SetAttatchedInfo();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void AutoBuildingInsideAnnotion();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月20日
	*/
	void ExportResult2DOC();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月20日
	*/
	void ExportResult2DWG1();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月20日
	*/
	void ExportResult2DWG2();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月23日
	 */
	void drawBuildOutLine();

private:

	void splitMulLineText(MStr &, const CString &, const CString &);


	/* @接口 读取注册表获取被替换的值
	* @参数 MStr 被替换的值
	* @作者 杨发荷
	* @时间 2017年06月19日
	*/
	void GetReplaceValue(MStr &);

	/* @接口 根据传入的实体绘制多段线
	 * @参数 AcDbObjectId 实体ID
	 * @作者 杨发荷
	 * @时间 2017年06月22日
	 */
	void DrawLines(AcDbObjectId &);

	/* @接口 绘制参考边
	 * @参数 AcDbObjectId 需要绘制参考边的实体ID
	 * @作者 杨发荷
	 * @时间 2017年06月22日
	 */
	void DrawReferenceEdge(AcDbObjectId &, double offset = 3.2);


	void ReadRegisterValue(SmartTable::Word *);

	void ReadAnnotionDist();

	/* @接口 获取被注记的实体ID以及注记边的起点和终点
	 * @参数 被注记的实体ID
	 * @参数 注记边的起点
	 * @参数 注记边的终点
	 * @返回 获取成功，返回值为true
	 * @作者 杨发荷
	 * @时间 2017年06月22日
	 */
	bool GetAnnotionEntAndPoint(AcDbObjectId &, AcGePoint2d &, AcGePoint2d &);

private:

	double m_Scale;
	double m_dist;
	CString m_SaveDir;
	TextProperty m_fwbczj;
	TextProperty m_zxbz;
	TextProperty m_zxbzy;
	TextProperty m_zxbzzx;

};

/* @类 厦门竣工第六个业务类
 * @作者 陶蓉
 * @时间 2017年3月21
 */
class XMJGPedestrianBridge
{

public:

	/* @接口 构造函数
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	XMJGPedestrianBridge();

	/* deconstruction 构造函数
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	~XMJGPedestrianBridge();

	/* @接口 输出总平图
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void bridgeZPT();

	/* @接口 输出人行天桥到dwg
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	void ExportDWGPB();

	/* @接口 天桥填充
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void bridgeHatch();

	/* @接口 天桥填充
	 * @作者 杨发荷
	 * @时间 2017年12月25号
	 */
	void birdgeHatch();

	/* @接口 绘制建筑占地区域
	 * @作者 陶蓉
	 * @时间 2017年4月6号
	 */
	void DrawRedLine();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void createProject();

	/* @接口 天桥标注
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void bridgeAnnoate();

	/* @接口 设置桥的层数
	 * @作者 杨发荷
	 * @时间 2017年12月25号
	 */
	void setBirdgeLayer();

	/* @接口 输出竣工报告
	 * @作者 杨发荷
	 * @时间 2018年2月7号
	 */
	void exportPBReport();

	/* @接口 设置天桥净高
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	void setPBClearWidth();

	/* @接口 设置天桥高度信息
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void bridgeSetHeight();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年9月29日
	 */
	void modifyPhoteSize();

	/* @接口 设置信息
	 * @作者 杨发荷
	 * @接口 2017年4月18号
	 */
	void SetAttatchedInfo();

	/* @接口 输出规划条件核实申请表
	 * @作者 杨发荷
	 * @接口 2017年4月6号
	 */
	void ExportApplyTable();

	/* @接口 绘制照相机拍摄点
	 * @作者 陶蓉
	 * @时间 2017年3月10号
	 */
	void DrawPhotoPositon();

	/* @接口 绘制控制点
	 * @作者 陶蓉
	 * @时间 2017年3月10号
	 */
	void DrawControlPoint();

	/* @接口 设置净高
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void setPBClearHeight();

	/* @接口 标注桥梁的长宽
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	void remarkLengthWidth();

	/* @接口 统计面积信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	void statisticAreaInfo();

	/* @接口 设置需要处理的管线
	 * @作者 杨发荷
	 * @时间 2018年2月8号
	 */
	void setHandlePipeLine();

	/* @接口 输出立面图
	 * @作者 杨发荷
	 * @时间 2018年1月5号
	 */
	void exportVerticalView();

	/* @接口 路面标高注记
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月28号
	 */
	void remarkLoadLevation();

	/* @接口 桥面标高注记
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月28号
	 */
	void remarkBridgeLevation();

	/* @接口 修改图片信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月1号
	 */
	void changePicturePropery();

	/* @接口 标注边界线
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	void AnnotationBoundaries();

	/* @接口 输出人行天桥立面图到dwg
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	void ExportDWGPBElevation();

	/* @接口 标注桥面厚度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	void remarkBridgeThickness();

	/* @接口 标注栏杆高度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	void remarkBridgeBalustrade();

	/* @接口 设置通道主体位置
	 * @作者 杨发荷
	 * @时间 2018年2月26号
	 */
	void setChannelBodyPosition();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年09月11日
	 */
	void exportPedestrianBridge();

	/* @接口 绘制权属边界
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	void DrawOwnershipBoundaries();

	/* @接口 输出人行天桥分层平面图到dwg
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	void ExportDWGPBStratificationPlan();

	/* @接口 控制点分布以及道路外观照片拍摄位置图
	 * @作者 杨发荷
	 * @时间 2017年4月18号
	 */
	void ControlPointAndRoadPicturePosition();

	/* @接口 输出建筑面积、高度以及桥梁高汇总表
	 * @作者 杨发荷
	 * @时间 2017年4月6号
	 */
	void ExportBuildAreaHeightAndBirdgeHeight();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月3号
	 */
	void compressFile();

private:

	/* @接口 保存文档
	 * @参数 Word 文档对象
	 * @作者 杨发荷
	 * @时间 2018年2月7号
	 */
	void saveDoc(Word &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月22号
	 */
	void addPlotRectangle();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月28号
	 */
	void getReplaceField(MStr &);

	/* @接口 获取权属范围线的面积
	 * @返回 double 权属边界范围线的面积
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	double getScopeOfOwnership();

	/* @接口 打开文档模板
	 * @参数 Word 文档对象
	 * @作者 杨发荷
	 * @时间 2018年2月7号
	 */
	bool openDocTemplate(Word &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年09月18日
	 */
	void replaceWordField(Word &);

	/* @接口 获取用地范围线的面积
	 * @返回 double 用地红线面积
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月26号
	 */
	double getBoundaryLineOfLand();

	/* @接口 替换照片信息
	 * @参数 Word 文档对象
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月1号
	 */
	void replacePictureInfo(Word &);

	/* @接口 替换项目基本信息
	 * @参数 Word 文档对象
	 * @作者 杨发荷
	 * @时间 2018年2月7号
	 */
	void replaceProjectInfomation(Word &);

	/* @接口 替换规划技术说明信息
	 * @参数 Word 文档对象
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月7号
	 */
	void replacePlanningTechnicalInformation(Word &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月27号
	 */
	void replaceBuildAreaAndHeightSummarySheet(Word &);

	/* @接口 替换规划条件核实申请表附表
	 * @参数 Word 文档对象
	 * @作者 杨发荷
	 * @时间 2018年2月26号
	 */
	void replacePlanningConditionVerificationTable(Word &);

private:

	double m_Scale;
	CString m_SaveDir;
	TextProperty m_qbjx;			//桥边界线属性
	TextProperty m_tqtc;			//天桥填充属性
	TextProperty m_tqbg;			//天桥标高属性

};

#pragma region 厦门竣工业务六道路带有管线

class XMJGRoadAndPipeLine
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	XMJGRoadAndPipeLine();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	~XMJGRoadAndPipeLine();

public:

	/* @function 绘制项目范围线
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void DrawProjectScopeOfLine();

	/* @function 绘制控制点
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void DrawControlPoint();

	/* @function 绘制照片方向
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void DrawPhotoPositon();

	/* @function 输出成果图到dwg
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportResults2Dwg();

	/* @function 输出总平图
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportTotalFlatFigure();

	/* @function 纵段面图
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportLongitudinalSectionFigure();

	/* @function 道路横断面对照图
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportRoadCrossSectionalComparisonChart();

	/* @function 道路技术总结
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void RoadTechnologySummary();

	/* @function 管线总平图
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportPipeLineTotalFlatFigure();

	/* @function 输出管线成果表
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportPipeLineResultTable();

	/* @function 管线设计与实测成果对照表
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportPipeLineDesignAndMeasuredComparisonTable();

	/* @function 管线对照图
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportPipeComparisonFigure();

	/* @function 管线对照图
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportPipeLinecomprehensiveTotalFlatFigure();

	/* @function 管线横断面布置图
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void ExportPipeLineCrossSectionalDwg();

	/* @function 管线技术总结
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void PipelineTechnicalSummary();

	/* @function 测量结果表
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void MeasuredResultsTable();

	/* @function 技术基础
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void TopographicBase();

private:

	CString m_SaveDir;

};

#pragma endregion

#pragma region 业务六道路不带管线

class XMJGRoadNoPipeLine
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	XMJGRoadNoPipeLine();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	~XMJGRoadNoPipeLine();

public:

	/* @接口 绘制控制点以及道路照片
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	void ExportControlPointAndRoadPicture();

	/* @接口 绘制道路总平图
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	void ExportRoadStratificationPlan();

	/* @接口 输出道路纵断面
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	void ExportRoadProfile();

	/* @接口 输出道路横断面
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	void ExportRoadCrossSectional();

	/* @接口 输出道路横断面对照图
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	void ExportRoadComprehensiveCrossSectional();

	/* @接口 输出成果对照表以及技术总结
	* @作者 杨发荷
	* @时间 2017年05月23日
	*/
	void ExportResultsContrastTechnicalSummary();

private:

	double  m_scale;
	CString m_SaveDir;

};

#pragma endregion

class XMJGFinalAcceptance
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	XMJGFinalAcceptance();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	~XMJGFinalAcceptance();

public:

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void FillBuildingArea();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void SetAttatchedInfo();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void OwnershipBoundary();
};

class XMJGQualityInspection
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	XMJGQualityInspection();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	~XMJGQualityInspection();

};

struct XMJGHAH
{
	CString m_ldh;		//楼栋号
	CString m_lch;		//层次
	double m_cg;		//层高
	double m_jzmj;		//建筑面积
	double m_jrmj;		//计容面积
	double m_jzzmj;		//建筑总面积
	double m_jrzmj;		//计容总面积
	double m_jrmjxj;	//计容面积小计
	double m_jzmjxj;	//建筑面积小计
	double m_jzzdmj;	//建筑占地面积
	XMJGHAH()
		: m_cg(0.0), m_jzmjxj(0.0),
		m_lch(_T("")), m_jrzmj(0.0),
		m_ldh(_T("")), m_jzmj(0.0),
		m_jrmj(0.0), m_jzzdmj(0.0),
		m_jrmjxj(0.0), m_jzzmj(0.0)
	{

	}

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void setValue(const XMJGHAH &hah, int lch)
	{
		m_cg = hah.m_cg;
		m_jrmj = hah.m_jrmj;
		m_jzmjxj = hah.m_jzmjxj;
		m_lch.Format(_T("%d"), lch);
		m_ldh.Format(_T("%s"), hah.m_ldh);
		m_jzmj = hah.m_jzmj;
		m_jzzdmj = hah.m_jzzdmj;
		m_jrmjxj = hah.m_jrmjxj;
	}

};

struct FunctionHeightArea
{
	double _height;
	double _jzarea;
	double _jrarea;
};

typedef map<double, FunctionHeightArea> HeightFunction;

struct LayerHeightAreaInfo
{
	int _row;
	int _lcount;
	CString _ldh;
	CString lname;
	double _jzarea;
	double _jrarea;
	double _jzjdmj;
	CString slayer;
	CString elayer;
	CString _layerinfo;
	HeightFunction _functions;
};

typedef vector<LayerHeightAreaInfo> LHeightAreaInfos;

typedef map<CString, LHeightAreaInfos> BuildLHeightAreaInfo;

typedef std::map<CString, XMJGHAH> MSHAH;

struct SplitMargeLayer;

/* @类 房屋 */
class XMJGHouse
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	XMJGHouse();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	~XMJGHouse();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void createProject();

	/* @function 设置工程目录
	* @author 杨发荷
	* @time 2017年5月12号
	*/
	void setProjectContext();

	/* @function 绘制车位
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void DrawCarAddress();

	/* @function 重构车位
	* @author 杨发荷
	* @time 2017年5月9号
	*/
	void DimensionsForCar();

	/* @function 设置车位标注
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void SetCarAnnotation();

	/* @function 绘制分层平面图
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void DrawStratificationPlan();

	/* @function 设置分层平面信息
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void SetStratificationPlanInfo();

	/* @function 变更区域构面积实体
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void DimensionsForChangeArea();

	/* @function 绘制建筑变更区域
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void DrawBuildChangeArea();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年9月25日
	 */
	void ExportChangeAreaDWG();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月17号
	 */
	void ExportGuiHuaBuYiZhiDWG();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月17号
	 */
	void newCheWeiStatistic();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年9月25日
	 */
	void ExportBuildHeightAndAreaInfo();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月26日
	*/
	void ExportBuildHeightAndAreaDoc();

	/* @function 添加建筑更改标注
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void AddBuildChangeAnnotion();

	/* @function 绘制功能区
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void DrawFunctionalAreas();

	/* @function 功能区构面
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void DimensionsFunctionalAreas();

	/* @function 设置功能区域信息
	* @author 杨发荷
	* @time 2017年5月8号
	*/
	void SetFunctionsAreasInfo();

	/* @接口 统计功能区的面积
	 * @作者 杨发荷
	 * @时间 2017年07月05日
	 */
	void StatisticFunctionArea();

	/* @function 高度示意图
	* @author 杨发荷
	* @time 2017年5月4号
	*/
	void ExportHighlySchematic();

	/* @function 将实体输出到dwg文件
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void ExportEntitys2File();

	/* @function 输出分层平面图
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void ExportStratificationPlan();

	/* @接口 设置输出照片信息
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	void SetExportHouseDocInfo();

	/* @接口 输出总的竣工报告书
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void ExportHouseDoc();

	/* @接口 输出平面信息到mdb中
	 * @作者 杨发荷
	 * @时间 2017年07月03日
	 */
	void ExportPlanInfo2MDB();

	/* @接口 设置信息
	 * @作者 杨发荷
	 * @时间 2017年07月13日
	 */
	void BuildSetAttatchedInfo();

	/* @接口 定义功能区
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月23日
	 */
	void FunctionDefine();

	/* @接口 建筑物填充
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void buildHatch();

	/* @接口 计算建筑占地面积
	 * @作者 何鹏鹏
	 * @时间 2019年9月24日
	 */
	void calculateJZZDMJ();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void drawRotateLine();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void changeLayerInfo();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	void drawAreaRange();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年10月9日
	 */
	bool comprassFile();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年10月15日
	 */
	void picturePosition();

	/* @接口 分层平面图添加指北针
	 * @作者 杨发荷
	 * @时间 2017年10月21日
	 */
	void addCompass();

	/* @接口 普通线转为变更区域
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	void lineConvertChangeArea();

	/* @接口 添加分层标注信息
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	void AddFunctionAnnotion();

	/* @接口 设置主体高度
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	void setMainHeight();

	/* @接口 添加夹层信息
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	void setMezzanineInfo();

	/* @接口 设置车位属性
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	void setCarProperty();

	/* @接口 设置项目信息
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	void settingProjectInfo();

	/* @接口 总平图标高
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void addZPTElevation();

	/* @接口 单边注记
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	void singleEdgeNote();

	/* @接口 设置层信息
	 * @作者 杨发荷
	 * @时间 2017年11月17号
	 */
	void setLayerInfoDlg();

	/* @接口 输出项目范围线
	 * @作者 杨发荷
	 * @时间 2017年11月17号
	 */
	void exportXMFWX();

	/* @接口 设置层信息
	 * @作者 杨发荷
	 * @时间 2017年11月20号
	 */
	void setLayerInfo();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月28号
	 */
	void addChangeNote();

	/* @function 设置功能区域信息
	 * @param AcDbObjectId 需要设置信息实体ID
	 * @author 杨发荷
	 * @time 2017年5月8号
	 */
	FunctionNameTableEx setFunctionalAreaInfo(const AcDbObjectId &);

	/* @接口 设置变更区域信息
	 * @参数 AcDbObjectId 变更区域实体id
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	bool setChangeAreasInfo(const AcDbObjectId &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void exportHouseZPT();

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年10月15日
	 */
	void addFunctionAreaNote(const AcDbObjectId &);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月18号
	 */
	void addFunctionProperty(const AcDbObjectId &, const FunctionNameTableEx &);

	/* @接口 输出不一致表
	* @时间 2017年10月15日
	*/
	void CompareExport();

	/* @接口 面积统计
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月30号
	 */
	void areaStatistic();

	/* @接口 创建内墙线
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月30号
	 */
	void innerWallLine();

	/* @接口 外半墙面积统计
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月30号
	 */
	void statisticHalfWall();

	/* @接口 修改竣工建筑物名称
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月31号
	 */
	void modifyJGBuildName();

	/* @接口 检查已经定义的功能区
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年2月1号
	 */
	void checkFunctionDefine();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月8号
	 */
	void drawLayerFirst();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年7月10号
	 */
	void boundaryNote();

	/*输出道路总平图
	*/
	void exportRoadZPT();

	/*输出道路控制点分布及建筑物外观照片拍摄位置图
	*/
	void PicAndKZD();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年7月27号
	 */
	void splitLayer();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月16号
	 */
	void tiQingMianJi();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月17号
	 */
	void distAnnotate();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月20号
	 */
	void checkGongNengQu();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月20号
	 */
	void defineGongNengQu();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月12号
	 */
	void checkBuildLayerAndMDB();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月15号
	 */
	void newSetLayerInfo();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月16号
	 */
	void newQualityCheck();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月15号
	 */
	void newCreateFCPMT();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月26号
	 */
	void newSplitMargeLayer();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月27号
	 */
	void newDrawUpPlanning();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月14号
	 */
	void newGHSTYSGSTBYZ();

private:

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月1号
	 */
	void addStoreyInfo(StoreyInfo &, const CString &, MStr &);

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月20号
	 */
	bool getIsOrNotJR();

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月27号
	 */
	void copyFCPMT(const SplitMargeLayer &, bool);

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月27号
	 */
	void moveFCPMT(const SplitMargeLayer &, bool);

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月27号
	 */
	void FCPMTNameOper(const SplitMargeLayer &, bool);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为zfalse
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool replaceWordField(Word &);
	
	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void getReplaceValue(MStr &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void getReplaceValue(MStr &, const CString &);

	/* @function 获取当前车位最大的编号
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	int GetCarSerialNum();

	/* @function 选择需要输出的实体
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool selectExportObject();

	/* @function 获取输出的全部实体
	* @param AcDbObjectId 图框的ID
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void getExportObjects(AcDbObjectId &, AcDbObjectIdArray &);

	/* @function 获取楼层号以及对应的楼层高度
	* @param AcDbObjectId 分层平面图的实体ID
	* @param MIDouble 楼层号以及高度
	* @author 杨发荷
	* @time 2017年5月4号
	*/
	void getFloorHeight(AcDbObjectId &, MIStoreyInfo &);

	/* @function 获取楼层号以及对应的楼层高度
	 * @param AcDbObjectId 分层平面图的实体ID
	 * @param MIDouble 楼层号以及高度
	 * @author 杨发荷
	 * @time 2017年5月4号
	 */
	void getFloorHeight(AcDbObjectId &, MIStoreyInfo &, const double &);

	/* @function 获取楼层号以及对应的楼层高度
	 * @param AcDbObjectId 分层平面图的实体ID
	 * @param MIDouble 楼层号以及高度
	 * @author 杨发荷
	 * @time 2017年5月4号
	 */
	void getFloorHeight(AcDbObjectId &, MDStoreyInfo &, const double &);

	/* @function 绘制高度示意图
	* @param MIStoreyInfo 层号以及楼层高度
	* @param AcDbObjectIdArray 绘制高度示意图的所有实体ID
	* @return bool 绘制示意图成功，返回值为true，否则返回值为false
	* @author 杨发荷
	* @time 2017年5月4号
	*/
	bool DrawHightlySchematic(const MIStoreyInfo &, AcDbObjectIdArray &, double, const TCHAR *);

	/* @接口 绘制夹层
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	void drawMezzanineInfo(const StoreyInfo &, double, AcDbObjectIdArray &);

	/* @function 设置高度示意图的路径
	* @author 杨发荷
	* @time 2017年5月8号
	*/
	void setHighlySchematicPath();

	/* @function 绘制楼层 
	* @param int 最后一个参数时为了绘制地下高度示意图时，绘制次数的控制
	* @param double 需要绘制的楼层高度（此高度是离地面的高度）
	* @param StoreyInfo 楼层信息
	* @param AcDbObjectIdArray 最终要被删除的实体集合
	* @author 杨发荷
	* @time 2017年5月8号
	*/
	void drawOneLayer(double, const CString &, const StoreyInfo &, AcDbObjectIdArray &);

	/* @接口 绘制地下层
	 * @参数 double 地下层的起始高度
	 * @参数 CMIStoreyInfoIter 楼层信息
	 * @参数 AcDbObjectIdArray 最终要删除的实体id集合
	 * @返回 double，返回地下层的高度
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	void drawUnderground(double &, CMIStoryInfoIter &, AcDbObjectIdArray &);

	/* @接口 绘制半地下层
	 * @参数 double 半地下层起始高度
	 * @参数 CMIStoreyInfoIter 楼层信息
	 * @参数 AcDbObjectIdArray 最终要删除的实体
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月22号
	 */
	void drawSubsurface(double &, CMIStoryInfoIter &, AcDbObjectIdArray &);

	/* @接口 绘制覆土
	 * @参数 double 覆土的起始高
	 * @参数 double 覆土的高度
	 * @参数 double 房屋的宽度
	 * @参数 AcDbObjectIdArray 最终要删除的实体id集合
	 * @作者 杨发荷
	 * @时间 2017年12月3号
	 */
	void drawUnderFuTu(double &, double, double, AcDbObjectIdArray &);

	/* @function 绘制有女儿墙的楼层
	* @param double 需要绘制的楼层高度（此高度是离地面的高度）
	* @param StoreyInfo 楼层信息
	* @param AcDbObjectIdArray 最终要被删除的实体集合
	* @author 杨发荷
	* @time 2017年5月8号
	*/
	void drawParapetWall(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	void drawHeightText(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @function 绘制没有女儿墙的楼层
	* @param double 需要绘制的楼层高度（此高度是离地面的高度）
	* @param StoreyInfo 楼层信息
	* @param AcDbObjectIdArray 最终要被删除的实体集合
	* @author 杨发荷
	* @time 2017年5月8号
	*/
	void drawNoParapetWall(double, const CString &, const StoreyInfo &, AcDbObjectIdArray &);

	/* @接口 绘制屋面层实体
	 * @参数 double 高度
	 * @参数 StoreyInfo 楼层信息
	 * @参数 AcDbObjectIdArray 最终要被删除的实体集合
	 * @作者 杨发荷
	 * @时间 2017年11月21号
	 */
	void drawWuMianEntity(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月21号
	 */
	void drawNvErQiangEntity(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @function 设置功能区域信息
	* @param AcDbObjectId 需要设置信息实体ID
	* @author 杨发荷
	* @time 2017年5月8号
	*/
	bool setFunctionalAreaInfo(AcDbObjectId &, FunctionNameTableEx &);

	/* @接口 获取保存路径
	* @参数 TCHAR 保存文件的路径
	* @参数 TCHAR 保存文件的名称
	* @返回 TCHAR 保存路径以及文件名
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	CString getSavePath(const TCHAR *, const TCHAR *);

	/* @接口
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月26日
	 */
	void getHouseAreaAndHeight(AcDbObjectId &, XMJGHAH &);

	/* @接口 从实体中读取层信息
	 * @参数 AcDbObjectId 实体对象
	 * @参数 XMJGHAH 层信息
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void getPerLevelArea(AcDbObjectId &, XMJGHAH &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void getSameLevelArea(XMJGHAH &, MSHAH &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void getBuildingArea(AcDbObjectId &, MSHAH &);

	/* @接口 获取楼层号
	 * @参数
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月26日
	 */
	int getHouseLCH(const TCHAR *, int &, int &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年09月14日
	 */
	void getPlanInfo(AcDbObjectId &, MStr &, bool);

	/* @接口
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void readArea2IniFile(MSBuild &);

	/* @接口
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void updataAreaIniFile(MSBuild &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	void writeArea2IniFile(const BuildHAH &);

	/* @接口
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void writeArea2IniFile(const MSBuild &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void exportDoc(SmartTable::Word &, const BuildHAH &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void exportDoc(SmartTable::Word &, const MSHAH &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	void replaceWordArea(SmartTable::Word &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月25号
	 */
	void replaceWordHeight(SmartTable::Word &);

	/* @接口
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年06月29日
	 */
	void setProjectInfo(SmartTable::Word &);

	/* @接口 设置许可证文档
	 * @参数 SmartTable::Word 输出doc
	 * @作者 杨发荷
	 * @时间 2017年06月28日
	 */
	void setLicenseDoc(SmartTable::Word &);

	/* @接口 向word里面插入房屋照片
	 * @参数 SmartTable::Word 输出的doc
	 * @作者 杨发荷
	 * @时间 2017年06月27日
	 */
	void insertHousePicture(SmartTable::Word &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年07月05日
	 */
	void readHousePictureInfo();

	/* @接口 读取功能区的信息
	 * @作者 杨发荷
	 * @时间 2017年07月05日
	 */
	void readFunctionInfo(const AcDbObjectIdArray &, AcDbObjectIdArray &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回
	 * @作者 杨发荷
	 * @时间 2017年07月05日
	 */
	void drawFunctionFrame(AcDbObjectId &, AcDbObjectIdArray &);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	bool getLayerArea(const AcDbObjectId &, LayerHAH &);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月26号
	 */
	void layerArea(const AcDbObjectId &, double &, double &, double &);

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年12月25号
	 */
	void getWaiBanQiangArea(const AcDbObjectId &, double &, double &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	bool getFunctionArea(AcDbObjectId &, FunctionHAH &);

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年8月5号
	 */
	void removeSubFunction(const AcDbObjectId &, double &, double &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	bool addFuncion2Layer(const FunctionHAH &, LayerHAH &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool addBuild2Layer(const LayerHAH &, BuildHAH &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool createStratificationPlan(const CString &, double);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月26日
	 */
	bool combineStratificationPlan(const CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	int createChangeAreaDWG(const CString &, double, const CString &, const CString &);

	/* @接口
	 * @参数
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	bool combineChangeAreaDWG(int, const CString &);

	/* @接口 标注功能区信息
	 * @参数 AcDbObjectId 分层平面ID号
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	bool addFunctionAnnotion(const AcDbObjectId &, CString &);

	/* @接口 标注功能区信息
	 * @参数 AcDbObjectId 分层平面ID号
	 * @作者 何鹏鹏
	 * @时间 2019年9月25日
	 */
	bool addCWAnnotion(const AcDbObjectId &, CString &);

	/* @接口 根据分层范围线获取里面所有的功能区
	 * @参数 AcDbObjectId 分层平面实体id
	 * @返回 AcDbObjectIdArray 功能区实体id集合
	 * @作者 杨发荷
	 * @时间 2017年10月25日
	 */
	AcDbObjectIdArray getFunctionAreaIds(const AcDbObjectId &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月26号
	 */
	CString statisticLayerInfo(const VMStr &);

	/* @接口 设置车位属性
	 * @参数 AcDbObjectId 车位实体的id
	 * @作者 杨发荷
	 * @时间 2017年10月26日
	 */
	void setCarProperty(const AcDbObjectIdArray &);

	/* @接口 规划实施情况
	 * @作者 杨发荷
	 * @时间 2017年11月2日
	 */
	void DesignImplementation(SmartTable::Word &);

	/* @接口 面积相关替换
	 * @作者 杨发荷
	 * @时间 2017年11月4日
	 */
	void AreaReplaceInfo(SmartTable::Word &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月6日
	 */
	void ExportDocInfo(SmartTable::Word &);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	void setJBXX(SmartTable::Word &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	void setJSSM(SmartTable::Word &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	void setMJXX(SmartTable::Word &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月25号
	 */
	void setBGXX(SmartTable::Word &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月26号
	 */
	CString statisticLayerInfo(const MStr &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年11月28号
	 */
	void writeArea2ProjectMDB(const AcDbObjectId &);

	/* @接口
	 * @参数 最底层的高度（不包含覆土的高度）
	 * @参数 房屋宽度
	 * @参数 double &室外地坪
	 * @作者 杨发荷
	 * @时间 2017年12月4号
	 */
	void drawDiPingXian(double &, double, double &, AcDbObjectIdArray &, const TCHAR *);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年12月28号
	 */
	void filterObject(VStr &, const CString &);

	/*道路总平图过滤
	*/
	void RoadfilterObj(VStr &);

	/*管线总平图过滤
	*/
	void PipefilterObj(VStr &, const CString&);

	/* @接口
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月1号
	 */
	AcDbObjectIdArray drawLeader(const ads_point spt, const ads_point ept, const CString &info);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月18号
	 */
	BuildLHeightAreaInfo initAreaHeightInfo();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月18号
	 */
	void initFunctionAreaHeightInfo(const CString&, const CString &, HeightFunction &);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年11月27号
	 */
	void DrawUpPlanning(SmartTable::Word &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月2号
	 */
	void replaceWordTitle(SmartTable::Word &);

	// 移除扣岛功能区
	void RemoveIslandFunctionArea(AcDbObjectIdArray &idArr);

	// 重构注记大小
	void RegenFontSize(int scale, AcDbObjectIdArray idArr);
private:

	//图片信息
	struct HousePicture
	{
		int m_order;	//图片顺序
		CString m_name;	//图片名称
		CString m_desc;	//描述信息
		CString m_pos;	//图片位置
	};
	typedef std::map<int, HousePicture> MIHPic;
	typedef MIHPic::iterator MIHPIter;

	int m_isNotNormalFloor;
	MIHPic m_PicInfo;	//图片信息
	CString m_SaveDir;				//保存目录
	double m_Scale;
	CString m_FloorName;			//幢号名称
	CString m_savePath;				//保存路径
	CString m_ProjectContext;		//工程目录
	MStr m_fieldTitle;
	MSInt m_isReplace;
	bool m_HighlySchematicFlag;		//绘制高度示意图的时候使用
	AcDbObjectIdArray m_object;
	TextProperty m_gnq;				//功能区
	TextProperty m_bg;
	TextProperty m_bgzj;			//变更标注
	TextProperty m_bgxxbz;			//变更信息标注
	TextProperty m_fcmjzs;			//分层面积注释
	TextProperty m_fcpm;			//分层平面
	TextProperty m_cw;				//车位
	TextProperty m_cwzj;			//车位注记
	TextProperty m_fwbczj;			//房屋边长注记
	TextProperty m_mjzj;			//分层平面面积注记
	TextProperty m_jzwlk;			//建筑物轮廓
	TextProperty m_lmtcmc;			//立面图层名称
	TextProperty m_lmtgdzj;			//立面图高度注记
	TextProperty m_swdpbg;			//室外地坪标高
	TextProperty m_sndpbg;			//室内地面标高
	TextProperty m_jzbg;			//建筑标高
	TextProperty m_hxjd;			//建筑竣工红线节点
	double m_height;				//注记要用到
	TextProperty m_dgzzbz;			//竣工单个坐标标注
	TextProperty m_hxzbbz;			//竣工红线坐标标注
	TextProperty m_dxfwx;			//地下范围线

};

void XMJGTest();

void OpenDwgCmd();

/* @macro 创建实体
* @author 杨发荷
* @time 2017年2月27号
*/
#define XMJG(XMJGReadData) XMJGRedLineSettingOut XMJGReadData;
#define BUILDOUTSET(BuildOutSet) XMJGBuildSettingOut BuildOutSet;
#define ROADOUTSET(RoadOutSet) XMJGRoadSettingOut RoadOutSet;
#define ALTITUDESURVEY(AltitudeSurvey)XMJGAltitudeSurvey AltitudeSurvey;
#define PEDESTRIANBRIDGE(PedestrianBridge)XMJGPedestrianBridge PedestrianBridge;
#define FINALACCEPTANCE(FinalAcceptance) XMJGFinalAcceptance FinalAcceptance;
#define ELELINGELEVATION(EE) XMJGEvelingElevation EE;
#define CHECKLINE(CheckLine) XMJGCheckLine CheckLine;
#define HOUSE(house) XMJGHouse house;
#define ROADNOPIPELINE(RoadNoPipeLine) XMJGRoadNoPipeLine RoadNoPipeLine;
#define PIPELINE(PiPeLine)	CXMJGPipeLine PiPeLine;


#endif /*XMJGREADDATA*/