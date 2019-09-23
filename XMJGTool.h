#ifndef XMJGTOOL_H
#define XMJGTOOL_H

#include <string>
#include "XMJGMacro.h"
#include <fstream>
#include "TinyExcel.h"
#include "Global.h"

class SBreakBlock;
using namespace std;
using std::string;

typedef void(*XMJGCallBack)(MStr &, SBreakBlock *);

void XMJGReplaceField(MStr &, SBreakBlock *);

enum XMJGDocType
{
	PedestrianBridge,
	RoadAndPipeLine,
	RoadNoPipeLine,
	BuildProject
};

enum XMJGFilterObject
{
	ChangeAreaFlag,
	FenCengPingMianFlag
};

enum XMJGExportFontSizeConfigType
{
	Text,
	Number,
	AreaAndRemark
};

enum XMJGExportLineWidthConfigType
{
	QSX
};

/* @function 绘制控制点
* @author 杨发荷
* @time 2017年2月24号
*/
class XMJGControlPoint
{
public:

	XMJGControlPoint()
	{
	}

	~XMJGControlPoint()
	{
	}

	void DrawControlPoint();

private:

	void AddControlPointProperty(AcDbObjectId &, const CString &, const CString &);

private:

	CString m_ControlPointRank;
	CString m_ControlPointType;

};

class CLockManager
{
public:

	CLockManager();

	~CLockManager();
};

class CRollBackManager
{
public:

	CRollBackManager(void);

	~CRollBackManager(void);

	Acad::ErrorStatus Commit();//上交

	Acad::ErrorStatus Abort();//取消

private:

	bool m_bCommit;//是否已经上交或者取消

};

class XMJGPath
{
public:

	/* @function 默认构造函数
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	XMJGPath();

	/* @function 构造函数
	* @param TCHAR *文档保存路径
	* @param TCHAR *配置文件路径
	* @param TCHAR *图片文件夹路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	XMJGPath(const TCHAR *, const TCHAR *, const TCHAR *);

	/* @function 默认析构函数
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	~XMJGPath();

	/* @function 获取保存文件路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	string getSavePath() const;

	/* @function 设置文档保存路径
	* @param TCHAR *文档保存路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	void setSavePath(const TCHAR *);

	/* @function 设置文档保存路径
	* @param CString 文档保存路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	void setSavePath(const CString &);

	/* @function 获取配置文件路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	CString getConfigPath() const;

	/* @function 设置配置文件路径
	* @param TCHAR *配置文件路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	void setConfigPath(const TCHAR *);

	/* @function 设置配置文件路径
	* @param TCHAR *配置文件路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	void setConfigPath(const CString &);

	/* @function 获取图片文件夹路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	string getPicturePath() const;

	/* @function 设置图片文件夹路径
	* @param TCHAR *图片文件夹路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	void setPicturePath(const TCHAR *);

	/* @function 设置图片文件夹路径
	* @param TCHAR *图片文件夹路径
	* @author 杨发荷
	* @time 2017年4月25号
	*/
	void setPicturePath(const CString &);

private:

	CString m_SavePath;
	CString m_ConfigPath;
	CString m_PicturePath;

};

//打印图片的格式格式
enum RasterFormat
{
	Raster_Jpg = 0,
	Raster_Bmp = 1
};

class XMJGColor
{
public:

	XMJGColor();

	~XMJGColor();

	void setColorRed(const byte &);

	void setColorGreen(const byte &);

	void setColorBlue(const byte &);

	byte getColorRed() const;

	byte getColorGreen() const;

	byte getColorBlue() const;

private:

	byte m_ColorRed;
	byte m_ColorGreen;
	byte m_ColorBlue;

};

class AcPlPlotInfo;
class AcPlPlotEngine;
class AcDbPlotSettings;
class AcPlPlotProgressDialog;
class AcDbPlotSettingsValidator;

class XMJGPrintPicture
{
public:

	XMJGPrintPicture();

	~XMJGPrintPicture();

	void setPrintPath(const CString &);

	bool setPrintFormat(const RasterFormat &);

	void setObjectArray(const AcDbObjectIdArray &);

	bool BeginPrintPicture();

	bool setPrintSizeName();

	int rescale_image_jpeg();

	bool DeletePicture();

private:

	bool getPrintPath(CString &);

	bool copyPrintConfigure(const CString &, const CString &);

	void setBackgroundPrint();

	void getPrintSize();

	bool checkPrintSizeName();

	AcDbObjectId getActiveLayout();

	void setPrintConfig();

	void setPlotRange();

	void setPlotConfig();

	bool createPrintWindow();

	void createPrintMsgGUI();

	void createPlotInfo(AcDbObjectId &layoutId);

	void beginDocument();

	void beginEngine();

	void destoryWindow();

	string getFilePath(const string &);

	string getFileName(const string &);

	string getFileTitle(const string &);

	string getFileExt(const char *);

	string getFileExt(const string &);

private:

	int m_nType;
	double m_Scale;
	int m_Width;
	int m_Height;
	CString m_pnpName;
	CString m_SizeName;
	XMJGColor m_BGColor;
	CString m_PrintDevice;
	XMJGPath *m_PicturePath;
	AcPlPlotInfo *m_pPlotInfo;
	AcPlPlotEngine *m_pEngine;
	RasterFormat m_RasterFormat;
	AcDbObjectIdArray m_ObjectArray;
	AcDbPlotSettings* m_pPlotSettings;
	AcDbPlotSettingsValidator *m_pPSV;
	AcPlPlotProgressDialog *m_pPlotProgDlg;
};

// enum XMJGDocType
// {
// 	PedestrianBridge = 0,
// 	RoadAndPipeLine = 1,
// 	RoadNoPipeLine = 2,
// 	BuildProject = 3
// };

typedef class XMJGPictureInfo
{
public:

	XMJGPictureInfo();

	double getScale() const;

	bool initPictureInfo(const CString &);

	void setPictureOrder(int);

	int getPictureOrder() const;

	void setPictureFormat(int);

	void getPictureFormat(double &, double &) const;

	void setPictureName(const CString &);

	string getPictureName() const;

	void setPictureDescribe(const CString &);

	string getPictureDescribe() const;

private:

	double m_Scale;
	int m_PictureOrder;
	int m_PictureFormat;
	CString m_PictureName;
	CString m_PictureDescribe;

}PicInfo;

typedef map<int, XMJGPictureInfo> MPicInfo;
typedef MPicInfo::iterator MPicInfoIter;

class XMJGTool
{
public:

	XMJGTool();
	~XMJGTool();

public:

	/* @function 绘制照片方向
	* @author 杨发荷
	* @time 2017年2月24号
	*/
	void DrawPhotoPositon();

	void ExportDocument(const XMJGPath *, XMJGDocType);

private:

	void getFolderDayFile(const CString &, CStringArray &);

	bool drawPhoto(ads_point, double &, int, const CString &);

	bool setPhotoInfo(ads_point, double, int, float, const CString &, const CString &, int);

	bool getTemplateByType(XMJGDocType, char *);

	void readPictureConfig(const CString &, MPicInfo &);

private:

	double m_hh;

};

string UTF8ToGBK(const std::string &);

class XMJGPlan
{

public:

	XMJGPlan();

	~XMJGPlan();

	bool CreatePlan();

	void setLayerInfo(const VStr &);

	bool CreatePlan(const CString &);

	bool CreatePlanEx(const CString &, const CString &);

	bool setObjectID(const AcDbObjectId &);

	void ExportPlanToPicture();

private:

	void CopyPlan();

	/* @function 绘制房屋轮廓
	* *author 杨发荷
	* @time 2017年5月9号
	*/
	void DrawAdumbration();

	bool DrawAdumbrationEx();

	void AddPlanInfo(const AcDbObjectId &, AcDbObjectId &, int);

	void DelPlanInfo(const AcDbObjectId &);

	bool SaveCurDwg();

	void SendCommand();

	bool SavePlanDwg();

	int GetPlanCount();

	void SendCommand(const TCHAR *);

	void CalculatePlanPos(int, AcGePoint2d &, const AcDbObjectId &);

private:

	double m_Scale;
	VStr m_layerinfo;
	CString m_lcname;
	CString m_ldname;
	CString m_savedir;
	CString m_dwgName;
	CString m_dwgPath;
	CString m_dwg_guid;
	TextProperty m_fcpm;
	TCHAR m_szHandle[255];
	AcDbObjectId m_ObjectID;
	AcDbObjectIdArray m_Ids;
	TCHAR m_dwgPathName[255];
};

class JGExportDWG
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	JGExportDWG();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	~JGExportDWG();

	/* @function 设置输出目录
	* @param TCHAR * 如果此参数为空，那么程序会自动让你指定目录
	*				 否则会按照指定的目录输出
	* @return bool 返回值仅仅针对上面一个参数为空才有效
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool setExportDir(TCHAR *dir = NULL);

	/* @function 设置输出的实体ID（此借口与bool setExportRange()借口不能同时设置）
	* @param AcDbObjectIdArray 被输出的实体ID集合
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void setObjects(const AcDbObjectIdArray &);

	/* @function 设置图框
	* @param TCHAR * 图框的名称
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void setPictureFrame(const TCHAR *, double angle = 0.0);

	/* @function 设置保存路径
	* @param TCHAR * 文件保存的完整路径
	* @author 杨发荷
	* @time 2017年5月4号
	*/
	void setSavePath(const TCHAR *);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年12月20号
	 */
	void isDrawRectangle(bool isdraw = false);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月24号
	 */
	void isFilterObject(bool);

	/* @function 设置保存路径
	* @param CString * 文件保存的完整路径
	* @author 杨发荷
	* @time 2017年5月4号
	*/
	void setSavePath(const CString &);

	/* @function 设置实体往外偏移的距离（此接口仅仅对bool setExportRange()接口有效）
	* @param double 指定偏移的距离
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void setOffsetDistance(double);

	/* @function 用户自己选定要输出那些实体
	* @param AcDbObjectIdArray 被输出的实体ID集合
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool setExportRange();

	/* @function 输出实体到dwg中
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool ExportDwg();

	/* @function 输出实体到dwg中
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool ExportDwg(MStr &, XMJGCallBack);

	/* @function 输出实体到图片中
	* @param bool 是否删除中间生成的图片
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool ExportPicture(bool del = false);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	bool ExportDwg2JPG(bool del = false);

	/* @接口 替换指定的字段
	* @参数 MStr 有字段和值组成
	* @返回 void
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void setReplaceField(const MStr &);

	/* @接口 设置楼层号
	* @参数 TCHAR 楼层号
	* @返回 void
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void setBuildNo(TCHAR *);

	/* @接口 设置楼栋号
	* @参数 TCHAR 楼栋号
	* @返回 void
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void setFloorNo(TCHAR *);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月26日
	*/
	void setCallBack(MStr &, XMJGCallBack);

	/* @function 输出实体到图片中
	* @param bool 是否删除中间生成的图片
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool saveDwgToPicture(bool flag);

	/* @接口
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	bool setReplaceChangeFlag(bool);

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年9月28日
	 */
	void setObjectFilterFlag(XMJGFilterObject);

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月9号
	 */
	void RegenFontSize(int scale, AcDbObjectIdArray idArr);

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月9号
	 */
	void RegenCompassSize(int scale, AcDbObjectIdArray idArr);

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月9号
	 */
	void RegenLineWidth(int scale, AcDbObjectIdArray idArr);

private:

	/* @接口 添加指北针
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void addCompass();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年09月16日
	 */
	void runCallBack();

	/* @接口 设置默认的缩放比例
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool setDefaultScale();

	/* @接口 计算缩放比例
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void CalculateScale();

	/* @接口 插入图框
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool InsertPictureFrame();

	/* @接口 将实体保存至dwg文件中
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool saveDwgToFile();

	/* @接口 将实体和图框整合到一起
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool combineEntityAndTk();

	/* @接口 移动实体到图框的中心
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void moveEntitysToTk();

	/* @接口 替换字段
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void ReplaceFieldValue();

	/* @接口 替换表头字段
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void ReplaceTableHeaderField();

	/* @接口 统计面积
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void StatisticArea(TCHAR *, TCHAR *, TCHAR *, TCHAR *);

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月27日
	 */
	bool replaceChangeNote();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月28日
	 */
	void replaceFCPMNote();

	/* @接口
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月28日
	 */
	void filterObject();

	/* @接口
	 * @作者 杨发荷
	 * @时间 2017年11月10日
	 */
	void calcFontSize();

	/* @接口 绘制外围矩形
	 * @参数
	 * @作者 杨发荷
	 * @时间 2017年12月20号
	 */
	void drawRectangle(AcDbObjectIdArray &);

private:

	XMJGFilterObject m_filterobject;		//实体过滤标记
	bool m_replaceflag;
	XMJGCallBack m_callback;
	double m_compass;						//指北针的角度
	CString m_BuildNo;						//楼层号
	CString m_FloorNo;						//楼栋号
	MStr m_ReplaceText;						//替换文本
	CString m_ExportDir;						//输出文件夹
	SBreakBlock *m_Block;
	double m_scale;							//计算图框与实体的缩放比例
	double m_OffsetDist;					//往外扩的距离
	int m_DefaultScale;					//默认的缩放比例
	CString m_SaveDwgPath;					//dwg保存的路径
	bool m_isDraw;							//是否绘制外围矩形
	CString m_PictureFrame;					//图框名称
	AcGePoint3d m_ObjMiddle;
	AcGePoint2d m_RightTop;					//右上
	AcGePoint2d m_LeftButton;				//左下
	AcGePoint3d m_tkMiddle;
	AcGePoint3d m_tkRightTop;				//图框右下角点
	AcGePoint3d m_tkLeftButton;				//图框左下角点
	AcGePoint2dArray m_Rectangular;			//外包矩形
	AcDbObjectIdArray m_PictureFrameIds;	//图框实体ID集合
	AcDbObjectIdArray m_Objects;			//输出的实体ID集合
	TextProperty m_bgbj;					//变更注记
	AcDbObjectId m_projectNameid;
	AcDbObjectId m_buildCompanyid;
	bool m_isfilterobject;
};

class XMJGExport
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	XMJGExport();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	~XMJGExport();

	/* @function 设置输出目录
	* @param TCHAR * 如果此参数为空，那么程序会自动让你指定目录
	*				 否则会按照指定的目录输出
	* @return bool 返回值仅仅针对上面一个参数为空才有效
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool setExportDir(TCHAR *dir = NULL);

	/* @function 设置输出的实体ID（此借口与bool setExportRange()借口不能同时设置）
	* @param AcDbObjectIdArray 被输出的实体ID集合
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void setObjects(const AcDbObjectIdArray &);

	/* @function 设置图框
	* @param TCHAR * 图框的名称
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void setPictureFrame(const TCHAR *, double angle = 0.0);

	/* @function 设置保存路径
	* @param TCHAR * 文件保存的完整路径
	* @author 杨发荷
	* @time 2017年5月4号
	*/
	void setSavePath(const TCHAR *);

	/* @接口
	* @参数
	* @作者 杨发荷
	* @时间 2017年12月20号
	*/
	void isDrawRectangle(bool isdraw = false);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年11月24号
	*/
	void isFilterObject(bool);

	/* @function 设置保存路径
	* @param CString * 文件保存的完整路径
	* @author 杨发荷
	* @time 2017年5月4号
	*/
	void setSavePath(const CString &);

	/* @function 设置实体往外偏移的距离（此接口仅仅对bool setExportRange()接口有效）
	* @param double 指定偏移的距离
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	void setOffsetDistance(double);

	/* @function 用户自己选定要输出那些实体
	* @param AcDbObjectIdArray 被输出的实体ID集合
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool setExportRange();

	/* @function 输出实体到dwg中
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool ExportDwg();

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月19号
	 */
	void setScaleEntity(const AcDbObjectId &);

	/* @function 输出实体到dwg中
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool ExportDwg(MStr &, XMJGCallBack);

	/* @function 输出实体到图片中
	* @param bool 是否删除中间生成的图片
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool ExportPicture(bool del = false);

	/* @接口
	* @参数
	* @返回 读取成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年9月24日
	*/
	bool ExportDwg2JPG(bool del = false);

	/* @接口 替换指定的字段
	* @参数 MStr 有字段和值组成
	* @返回 void
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void setReplaceField(const MStr &);

	/* @接口 设置楼层号
	* @参数 TCHAR 楼层号
	* @返回 void
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void setBuildNo(TCHAR *);

	/* @接口 设置楼栋号
	* @参数 TCHAR 楼栋号
	* @返回 void
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void setFloorNo(TCHAR *);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月26日
	*/
	void setCallBack(MStr &, XMJGCallBack);

	/* @function 输出实体到图片中
	* @param bool 是否删除中间生成的图片
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	bool saveDwgToPicture(bool flag);

	/* @接口
	* @参数
	* @返回 读取成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年9月27日
	*/
	bool setReplaceChangeFlag(bool);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年9月28日
	*/
	void setObjectFilterFlag(XMJGFilterObject);

private:

	/* @接口 添加指北针
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void addCompass();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年09月16日
	*/
	void runCallBack();

	/* @接口 设置默认的缩放比例
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool setDefaultScale();

	/* @接口 计算缩放比例
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void CalculateScale();

	/* @接口 插入图框
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool InsertPictureFrame();

	/* @接口 将实体保存至dwg文件中
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool saveDwgToFile();

	/* @接口 将实体和图框整合到一起
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	bool combineEntityAndTk();

	/* @接口 移动实体到图框的中心
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void moveEntitysToTk();

	/* @接口 替换字段
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void ReplaceFieldValue();

	/* @接口 替换表头字段
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void ReplaceTableHeaderField();

	/* @接口 统计面积
	* @作者 杨发荷
	* @时间 2017年06月08日
	*/
	void StatisticArea(TCHAR *, TCHAR *, TCHAR *, TCHAR *);

	/* @接口
	* @返回 读取成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年9月27日
	*/
	bool replaceChangeNote();

	/* @接口
	* @返回 读取成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年9月28日
	*/
	void replaceFCPMNote();

	/* @接口
	* @返回 读取成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年9月28日
	*/
	void filterObject();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年11月10日
	*/
	void calcFontSize();

	/* @接口 绘制外围矩形
	* @参数
	* @作者 杨发荷
	* @时间 2017年12月20号
	*/
	void drawRectangle(AcDbObjectIdArray &);

private:

	XMJGFilterObject m_filterobject;		//实体过滤标记
	bool m_replaceflag;
	XMJGCallBack m_callback;
	double m_compass;						//指北针的角度
	CString m_BuildNo;						//楼层号
	CString m_FloorNo;						//楼栋号
	MStr m_ReplaceText;						//替换文本
	CString m_ExportDir;						//输出文件夹
	SBreakBlock *m_Block;
	double m_scale;							//计算图框与实体的缩放比例
	double m_OffsetDist;					//往外扩的距离
	int m_DefaultScale;					//默认的缩放比例
	CString m_SaveDwgPath;					//dwg保存的路径
	bool m_isDraw;							//是否绘制外围矩形
	CString m_PictureFrame;					//图框名称
	AcGePoint3d m_ObjMiddle;
	AcGePoint2d m_RightTop;					//右上
	AcGePoint2d m_LeftButton;				//左下
	AcGePoint3d m_tkMiddle;
	AcGePoint3d m_tkRightTop;				//图框右下角点
	AcGePoint3d m_tkLeftButton;				//图框左下角点
	AcGePoint2dArray m_Rectangular;			//外包矩形
	AcDbObjectIdArray m_PictureFrameIds;	//图框实体ID集合
	AcDbObjectIdArray m_Objects;			//输出的实体ID集合
	TextProperty m_bgbj;					//变更注记
	AcDbObjectId m_projectNameid;
	AcDbObjectId m_buildCompanyid;
	bool m_isfilterobject;
	AcDbObjectId m_sEnt;					//带缩放的实体
};

class CRTree;

class XMJGDimensions
{
public:

	XMJGDimensions();

	~XMJGDimensions();

	bool beginDimensions();

	void setSearchDist(double);

	AcDbObjectId getEntityId();

	void setPoint(const AcGePoint2d &);

	void setEntityLayer(const TCHAR *);

	void setEntityColor(int, int green = -1, int blue = -1);

private:

	void buildNodes();

	void drawEntity();

	void setPropery();

	void filterEntity();

	bool searchNearestBoders();

	void InitTree();

private:

	int m_Color;
	double m_Dist;
	CRTree *m_Tree;
	CString m_Layer;
	AcGePoint2d m_Point;
	AcDbObjectId m_ObjectID;
	AcGePoint2dArray m_Nodes;
	AcDbObjectIdArray m_ObjectIDs;
};

class XMJGCheckQuality
{
public:

	XMJGCheckQuality();

	~XMJGCheckQuality();

	void CheckQualityRedLine();

private:

	void checkRedLineCoorAnn();

	void checkRedLineLenghtAnn();

	void checkRedLineSegmentAnn();

	bool checkCoor(AcDbObjectId &);

	void checkNoCrossRedLineNodeAnn();

	void getErrorRedLineCoorAnn(AcDbObjectId &);

private:

	AcDbObjectIdArray m_ObjectIDArr;

};

/* @class 绘制箭头类
* @author 杨发荷
* @time 2017年3月
*/
class CDrawArrow
{
public:

	/* @接口 默认构造函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	CDrawArrow();

	/* @接口 默认析构函数
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	~CDrawArrow();

	AcDbObjectId DrawLeader(ads_point, ads_point, float, const TCHAR *, int);

	void getQLeaderText(ads_name head, float, const TCHAR *, int);

	AcDbObjectId DrawLeader(ads_point, ads_point, float, const TCHAR *, int, const CString &);

	AcDbObjectId DrawArrow(const AcDbObjectId &, double length = 0.54);
	/* @function　绘制箭头
	*param AcGePoint2d
	*/
	void DrawArrow(const AcGePoint2d &, const double &, AcDbObjectId &, double length = 0.54);

	/* @接口 标注实体
	* @参数 AcGePoint2d 标注的位置
	* @参数 AcGePoint2d 标注的点
	* @返回 标注的实体di
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	AcDbObjectId DrawArrow(const AcGePoint2d &, const AcGePoint2d &, double length = 0.54);

	/* @function 添加变更注记
	* @param AcDbObjectId 被添加变更注记的实体ID
	* @param AcGePoint2d 变更注记插入的位置
	* @param TCHAR * 变更注记的信息
	* @param AcDbObjectId 变更标注的实体编组
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	AcDbObjectId DrawArrow(AcDbObjectId &, const AcGePoint2d &, TCHAR *);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	AcDbObjectId DrawArrow(const AcGePoint2d &fPt, const AcGePoint2d &sPt, TCHAR * inserttext);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	AcDbObjectId DrawStraightArrow(const AcGePoint2d &sPt, const AcGePoint2d &ePt, double dscale = 1.0, int color = 0);

	/* @接口 根据当前选中的实体ID来获取箭头的ID
	* @参数 AcDbObjectId 选中的注记ID
	* @参数 bool 是否删除传入实体所有编组实体
	* @返回 AcDbObjectId 箭头的ID
	* @作者 杨发荷
	* @时间 2017年06月15日
	*/
	AcDbObjectId GetArrowObjectID(AcDbObjectId &, bool del = true);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年07月06日
	*/
	int getEntityByPoint(const AcGePoint2d &, AcDbObjectId &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年07月06日
	*/
	bool getEntsByPoint(ads_point, AcDbObjectIdArray &);

private:

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void GetArrowLastPoint(const AcDbObjectId &, AcGePoint2d &);

	/* @function 获取箭头的插入点以及内角的角平分线
	* @param AcGePoint2dArray 实体的点集
	* @int 实体点集中需要被注记的点的索引
	* @bool 是顺时针还是逆时针
	* @ads_point 用来返回箭头的插入位置
	* @double 内角的角平分线
	* @author 杨发荷
	* @time 2017年3月7号
	*/
	void GetOutCornerAndPosition(AcGePoint2dArray &, int, bool, ads_point &, double &);

	/* @function 添加文本注记
	* @param AcDbObjectId 箭头以及线的ID组成的一个编组
	* @double 内角的角平分线
	* @param TCHAR *被添加的注记信息
	* @param TCHAR *被添加的注记信息
	* @author 杨发荷
	* @time 2017年3月7号
	*/
	void AddTextInfo(AcDbObjectId &, double angle, TCHAR *, TCHAR * = NULL);

	/* @接口 将坐标写到实体当中
	* @参数 AcDbObjectId 被写的实体ID
	* @参数 AcGePoint2d 二维点
	* @作者 杨发荷
	* @时间 2017年06月19日
	*/
	void AddCoorToObject(AcDbObjectId &, const AcGePoint2d &);

	/* @接口 将注记长度写入到实体中
	* @参数 AcDbObjectId 被写的实体ID
	* @参数 double 注记线的长度
	* @作者 杨发荷
	* @时间 2017年06月19日
	*/
	void AddAnnotionLength(AcDbObjectId &, double);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool DrawInsertArrow(const AcGePoint2d &, const double &, AcDbObjectId &);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void DrawDiagonalLine(AcDbObjectId &, const AcGePoint2d &, const AcGePoint2d &);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void DrawHorizontalLine(AcDbObjectId &, const double &, const AcGePoint2d &, double length = 4);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void AddPropertyToXData(AcDbObjectId &, AcDbObjectId &, const TCHAR *, int idx = 0);

private:

	double m_Scale;

};

/* @class 点信息类
* @author 杨发荷
* @time 2017年3月
*/
class CPointInfo
{
public:

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	CPointInfo()
	{
	}

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	CPointInfo(const VStr &str)
	{
		if(str.size() != 4) return;
		Convert(str.at(0), str.at(1), str.at(2), str.at(3));
	}

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	CPointInfo(const CString &name, const AcGePoint3d &pt)
	{
		m_Name = name;
		m_pt3.x = pt.x;
		m_pt3.y = pt.y;
		m_pt3.z = pt.z;
	}

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	CPointInfo(const CString &name, double x, double y, double z)
	{
		m_Name = name;
		m_pt3.x = x;
		m_pt3.y = y;
		m_pt3.z = z;
	}

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	CPointInfo(const CString &name, const CString &x, const CString &y, const CString &z)
	{
		Convert(name, x, y, z);
	}

	/* @deconstruction 默认析构函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	~CPointInfo()
	{
	}

public:

	CString m_Name;    //点名
	AcGePoint3d m_pt3; //坐标
	double m_pipeheight;//管线高程
	double m_dvalue;//管线差值
private:

	/* @function 格式化
	* @param CString 点的名称
	* @param CString 点的x坐标
	* @param CString 点的y坐标
	* @param CString 点的z坐标
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void Convert(const CString &name, const CString &x, const CString &y, const CString &z)
	{
		m_Name = name;
		m_pt3.x = _tstof(x);
		m_pt3.y = _tstof(y);
		m_pt3.z = _tstof(z);
	}

};

typedef vector<CPointInfo>  VPInfo;

class XMJGImageSize
{
	/* @接口 联合体，用来保存图像的宽
	* @作者 杨发荷
	* @时间 2017年06月27日
	*/
	union
	{
		int xsize;
		int width;
		int col;
	};
	/* @接口 联合体，用来保存图像的高
	* @作者 杨发荷
	* @时间 2017年06月27日
	*/
	union
	{
		int ysize;
		int height;
		int row;
	};

public:

	/* @接口 打开图片文件
	* @参数 TCHAR *图片文件名称
	* @返回 bool 打开图片文件成功，返回值为true
	* @作者 杨发荷
	* @时间 2017年06月27日
	*/
	bool loadImage(const char*fname)
	{
		fstream ffin(fname);
		if(!ffin)     return false;
		char s1[2] = {0}, s2[2] = {0};
		ffin.seekg(164);
		ffin.read(s1, 2);
		ffin.read(s2, 2);
		xsize = (unsigned int)(s1[1]) << 8 | (unsigned int)(s1[0]);
		ysize = (unsigned int)(s2[1]) << 8 | (unsigned int)(s2[0]);
		return true;
	}

	/* @接口 获取图片的宽
	* @作者 杨发荷
	* @时间 2017年06月27日
	*/
	int ImageWidth()
	{
		return xsize;
	}

	/* @接口 获取图片的高
	* @作者 杨发荷
	* @时间 2017年06月27日
	*/
	int ImageHeight()
	{
		return ysize;
	}

	/* @接口 获取图片输出的大小
	* @参数 double 图片输出的宽
	* @参数 double 图片输出的高
	* @作者 杨发荷
	* @时间 2017年06月27日
	*/
	void getExportSize(double &x, double &y)
	{
		if(xsize > ysize)
		{
			x = 14.5;
			y = 10.8;
		}
		else
		{
			x = 16.9;
			y = 22.22;
		}
	}
};

/* @class 厦门竣工辅助类
* @author 杨发荷
* @time 2017年3月
*/
class XMJGAssist
{
public:

	/* @construction 默认的构造函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	XMJGAssist();

	/* @deconstruction 默认析构函数
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	~XMJGAssist();

	/* @function 选择文件
	* @param CString 选择选择文件进行过滤
	* @CString 备选则的文件全路径名
	* @bool flag 打开文件对话框还是保存文件对话框
	* @bool 选择文件成功返回值为true，否则返回值为false
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	bool SelectFile(const CString &, const CString &, CString &, bool flag = true);

	/* @function 将特定的字符串转为点
	* @param CString 特定的字符串
	* @param ACGePoint2d 点的坐标
	* @return 转换成功，返回值为true，否则返回值为false
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	bool StrConvertToPoint(const CString &, AcGePoint2d &);

	/* @function 读取dat文件并解析该文件
	* @param CString 文件全路径名
	* @param VPInfo 点的信息集合
	* @param VStr 读取文件时的错误信息
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void PurgeDatFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author 杨发荷
	* @time 2017年11月15号
	*/
	void PurgeRTKFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author 杨发荷
	* @time 2017年11月15号
	*/
	void PurgeQZYFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function 读取Excel文件并解析该文件
	* @param CString 文件全路径名
	* @param VPInfo 点的信息集合
	* @param VStr 读取文件时的错误信息
	* @author 杨发荷
	* @time 2017年3月7号
	*/
	void PurgeExcelFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function 读取Excel文件管点表
	*/
	void PipeExcel(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author 杨发荷
	* @time 2017年11月15号
	*/
	void PurgeExcelQZY(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author 杨发荷
	* @time 2017年11月15号
	*/
	void PurgeExcelRTK(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* function 绘制点
	* param VPInfo 单个点
	* @author 杨发荷
	* @time 2017年3月7号
	*/
	void DrawPoint(const CPointInfo &);

	/* function 绘制点集
	* param VPInfo 点的集合
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void DrawPoint(const VPInfo &);

	/* @function 绘制折线
	* @param TCHAR * 折线所在的图层
	* @param TCHAR * 折线的编码
	* @param int * 折线的颜色
	* @param closeornot * 是否提示闭合（默认为提示）
	* @return AcDbObjectId 返回绘制的折线id
	* @author 杨发荷
	* @time 2017年3月9号
	*/
	AcDbObjectId DrawLine(const TCHAR *, const TCHAR *, const int &, TCHAR *Style = NULL, bool closeornot = true);

	/* @function 绘制折线
	* @param AcDbObjectId 绘制实体的id
	* @param TCHAR * 折线所在的图层
	* @param TCHAR * 折线的编码
	* @param int * 折线的颜色
	* @param closeornot * 是否提示闭合（默认为提示）
	* @return bool 返回值为true，实体为闭合，否则实体没有闭合
	* @author 杨发荷
	* @time 2017年3月9号
	*/
	bool DrawLine(AcDbObjectId &, const TCHAR *, const TCHAR *, const int &, TCHAR *Style = NULL, bool closeornot = true);

	/* @function 绘制折线
	* @param TCHAR * 折线所在的图层
	* @param TCHAR * 折线的编码
	* @param int * 折线的R
	* @param int * 折线的G
	* @param int * 折线的B
	* @param closeornot * 是否提示闭合（默认为提示）
	* @return AcDbObjectId 返回绘制的折线id
	* @author 杨发荷
	* @time 2017年3月9号
	*/
	AcDbObjectId DrawLine(const TCHAR *, const TCHAR *, const int &, const int &, const int &, TCHAR *Style = NULL, bool closeornot = true);

	/* @function 打印错误信息
	* @param VString 收集错误信息
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void PrintErrorInfo(const VStr &);

	/* @function 自动创建编码
	* @param bool 是否从西北角点开始编号
	* @param pre 编号的前缀
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void AutoCreateCode(const AcDbObjectId &, bool flag = true, TCHAR *pre = NULL);

	/* @function 给点实体添加编码
	* @param AcDbObjectId 点实体ID
	* @param TCHAR 编号的前缀
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void AddPointCode(const AcDbObjectId &, TCHAR *);

	/* @function 清除点的编号
	* @param AcDbObjectId 被清除的点实体ID
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void ClearPointCode(const AcDbObjectId &);

	/* @function 调整实体的节点顺序
	* @param AcDbObjectId 实体id
	* @param int 实体节点的顺序
	* @param int 实体节点的其实位置
	* @author 杨发荷
	* @time 2017年2月13号
	*/
	void AdjustNodeOrder(const AcDbObjectId &, int, int);

	/*  @function 根据注记在实体ID集合中查找指定的实体
	* @param AcDbObjectIdArray 实体ID集合
	* @param TCHAR 注记
	* @param AcDbObjectId 找到的实体ID
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void findEntFromZJ(const AcDbObjectIdArray &, TCHAR *, AcDbObjectId &);

	/* @function 从点集中删除指定实体（注意：实体仅仅是点实体）
	* @param AcDbObjectId 点实体ID
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void RemoveNodeFromID(AcGePoint2dArray &, const AcDbObjectId &);

	/* @function 删除节点
	* @param AcGePoint2dArray
	* @param AcDbObjectIdArray
	* @param bool 如果flag为true，那么仅仅删除第二个集合中的所有实体id，
	如何flag为false，那么删除第二个集合中每个实体编组中的其他实体id
	* @author 杨发荷
	* @time 2017年2月16号
	*/
	void RemoveNodeFromID(AcGePoint2dArray &, const AcDbObjectIdArray &, bool flag = true);

	/* @function 为实体添加注记
	* @param AcDbObjectId 需要添加注记的实体ID
	* @author 杨发荷
	* @time 2017年2月27号
	*/
	void addAnnotation(const AcDbObjectId &);

	/* @function 检查工程信息设置是否完备
	* @author 陶蓉
	* @time 2017年3月3号
	*/
	bool checkProjInfo(int pro_type);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool GetExportEntitys(AcDbObjectIdArray &, const AcGePoint2dArray &nodes);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool GetExportRange(AcGePoint2d &, AcGePoint2d &);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void GetExportRange(const AcGePoint2d &, const AcGePoint2d &, AcGePoint2dArray &);

	/* @获取缩放比例
	* @param tklb 图框的左下角点
	* @param tkrt 图框的右上角点
	* @param entlb 实体的左下角点
	* @param entrt 实体的右下角点
	* @return double 缩放系数
	* @author 杨发荷
	* @time 2017年3月8号
	*/
	double GetScale(const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &);

	/* @function 保存实体到文件中
	* @param AcDbObjectIdArray 被保存的实体集合
	* @return bool 保存成功返回值为true，否则返回值false
	* @author 杨发荷
	* @time 2017年3月8号
	*/
	bool SaveEntitysToDWG(const CString &);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void SetAzimuthInfo(int, TCHAR *);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void SetAnnotationType();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void GetAnnotationType(TCHAR *);

	/* @function 添加注记信息
	* @param AcDbObjectId 添加注记信息的实体
	* @param int 注记实体的颜色
	* @param TCHAR 注记的信息
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void AddAnnotationInfo(const AcDbObjectId &, int color, TCHAR *info);

	/* @function 保存实体到dwg文件中
	* @param AcGePoint3D 实体缩放的点
	* @param double 实体缩放比例
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	bool SaveEntitysToDWG(const AcGePoint3d &, const double &);

	/* @function 获取建筑注记
	* @param AcDbObjectArray 注记实体ID集合
	* @param TCHAR 注记信息所在的图层
	* @param MStr 键与值一一对应，这些信息都被写到实体当中
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void AddAnnotationInfo(const AcDbObjectId &, TCHAR *, MStr &);

	/* @function 添加注记信息
	* @param AcDbObjectArray 需要添加注记实体ID集合
	* @param TCHAR 注记信息所在的图层
	* @param MStr 键与值一一对应，这些信息都被写到实体当中
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void AddAnnotationInfo(const AcDbObjectIdArray &, TCHAR *, MStr &);

	/* @function 获取红线和建筑物之间的注记实体
	* @param AcDbObjectArray 注记实体ID集合
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void GetRedLineAndBuildAnnotation(AcDbObjectIdArray &);

	/* @function 获取建筑注记
	* @param AcDbObjectArray 注记实体ID集合
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void GetBuildAnnotation(AcDbObjectIdArray &);
	
	/* @function 获取建筑物与边界线之间的距离
	* @param 建筑物与边界线之间的距离
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void GetEdgeDistanceInfo(CString &);

	/* @function 获取建筑物之间的距离
	* @return CString 建筑物之间的距离
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void GetBuildDistanceInfo(CString &);

	/* @function 获取建筑物与边界线之间的距离
	* @param 建筑物与边界线之间的距离
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void GetEdgeDistanceText(CString &);

	/* @function 获取建筑物之间的距离
	* @return CString 建筑物之间的距离
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void GetBuildDistanceText(CString &);

	/* @function 获取标注内的信息
	* @param AcDbobjectIdArray 标注实体ID集合
	* @param vector<AzimuthDist> 方位角以及距离的集合
	* @return AcDbObjectId 项目范围线
	* @author 杨发荷
	* @time 2017年3月13号
	*/
	void GetAzimuthDist(AcDbObjectIdArray &, vector<AzimuthDist> &);

	/* @function 绘制项目范围线（选择两点确定）
	* @return AcDbObjectId 项目范围线
	* @author 陶蓉
	* @time 2017年3月13号
	*/
	AcDbObjectId CreateScopeLine();

	/* @function 绘制不同类型和等级的测量点
	* @return AcDbObjectId 测量点
	* @author 陶蓉
	* @time 2017年3月21号
	*/
	AcDbObjectId drawControlPoint(CString, CString, CString, CString, AcGePoint2d);

	/* @function 框选边长为10的整数倍的范围线
	* @return AcDbObjectId 范围线ID(该线图层为"规划控制线")
	* @author 陶蓉
	* @time 2017年3月24号
	*/
	AcDbObjectId DrawFinalScopeLine();

	/* @function 根据点来获取点的实体
	例如：在绘制红线的时候，红线上的每个节点都是在导入的点上绘制的，所以在获取红线上的节点情况下下来获取导入的点。
	* @return AcDbObjectId 点实体ID
	* @author 杨发荷
	* @time 2017年3月30号
	*/
	AcDbObjectId GetPointEntByPoint(const AcGePoint2d &);

	/* @function 边界注记
	* @param AcDbObjectId 需要标注的实体id
	* @return AcDbObjectIdArray 标注符号的ID集合
	* @author 杨发荷
	* @time 2017年3月30号
	*/
	AcDbObjectIdArray BoundaryAnnotation(const AcDbObjectId &);

	/* @function 边界注记
	* @param AcGePoint2dArray 需要标注的实体点集
	* @return AcDbObjectIdArray 标注符号的ID集合
	* @author 杨发荷
	* @time 2017年3月30号
	*/
	AcDbObjectIdArray BoundAryAnnotation(const AcGePoint2dArray &);

	/* @function 边界注记
	* @param AcGePoint2d 边界标注的起点
	* @param AcGePoint2d 边界标注的终点
	* @return AcDbObjectId 标注符号的id（此ID是一个group)
	* @author 杨发荷
	* @time 2017年3月30号
	*/
	AcDbObjectId BoundAryAnnotation(const AcGePoint2d &, const AcGePoint2d &);

	/* @function 绘制格网
	* @Attention 根据当前图面上的实体坐标范围来绘制
	* @author 杨发荷
	* @time 2017年4月11号
	*/
	void DrawMesh(CString title = _T(""));

	/* @function 绘制格网
	* @Attention 根据当前图面上的实体坐标范围来绘制
	* @author 杨发荷
	* @time 2017年4月11号
	*/
	void DrawMesh(const AcDbObjectIdArray &, CString title = _T(""));

	/* @function 绘制格网
	* @param AcGePoint2d 绘制图框的坐标
	* @param AcGePoint2d 绘制图框的坐标
	* @Attention 上面两个参数是任意对角线坐标，一定是对角线最表
	* @author 杨发荷
	* @time 2017年4月11号
	*/
	void DrawMesh(const AcGePoint2d &, const AcGePoint2d &, ExportDWGType);

	/*绘制网格和标题-管线绘制专用
	 * nTable 0:总平图
	 nTable 1:对照图
	 */
	AcDbObjectIdArray DrawMesh(AcGePoint2d &, AcGePoint2d &, const CString &strGXJM, int nTable = 0);

	/* @function 获取点集整幅外包的左下与右上角点
	* @param AcGePoint2d 外包左下角点
	* @param AcGePoint2d 外包右上角点
	* @author 杨发荷
	* @time 2017年4月11号
	*/
	void GetExternPoint(const AcGePoint2dArray &, AcGePoint2d &, AcGePoint2d &);

	/* @function 外包的左下与右上角点计算整幅的坐标
	* @param AcGePoint2d 外包左下角点
	* @param AcGePoint2d 外包右上角点
	* @param AcGePoint2d 外扩的左下角点
	* @param AcGePoint2d 外扩的右上角点
	* @author 杨发荷
	* @time 2017年4月11号
	*/
	void GetExternPoint(const AcGePoint2d &, const AcGePoint2d &, AcGePoint2d &, AcGePoint2d &);

	/* @function 绘制图框内的十字丝
	* @param AcGePoint2d 被保存实体的左下角点
	* @param DrawDir 绘制十字丝的方向
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void DrawCrosshair(const AcGePoint2d &, const DrawDir &);

	/* @function 绘制图框内的十字丝
	* @param AcGePoint2d 被保存实体的左下角点
	* @param AcGePoint2d 被保存实体的右上角点
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void DrawCrosshair(const AcGePoint2d &, const AcGePoint2d &);

	/* @function 过滤实体
	* @param AcDbObjectIdArray 被保存的实体ID
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void DrawTuKuang(const AcGePoint2d &, const AcGePoint2d &);

	/* @function 添加图框四个角点坐标
	* @param AcGePoint2d 被保存实体的左下角点
	* @param AcGePoint2d 被保存实体的右上角点
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void AddCornerCoor(const AcGePoint2d &, const AcGePoint2d &);

	/* @function 添加图框标题
	* @param AcGePoint2d 被保存实体的左下角点
	* @param AcGePoint2d 被保存实体的右上角点
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &, ExportDWGType);

	/*绘制标题-管线专用
	*/
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &, const CString &, int nTable = 0);

	/* @function 插入图签
	* @param AcGePoint2d 被保存实体的左下角点
	* @param AcGePoint2d 被保存实体的右上角点
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	bool InsertTuQian(const AcGePoint2d &, const AcGePoint2d &);

	/* @function 过滤实体
	* @param AcDbObjectIdArray 被保存的实体ID
	* @param MStr 过滤实体的条件
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void FilterEntitys(AcDbObjectIdArray &, const MStr &);

	/* @function 过滤实体
	* @param AcDbObjectIdArray 被保存的实体ID
	* @param MVStr 过滤实体的条件
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void FilterEntitys(AcDbObjectIdArray &, const MVStr &);

	/* @function 过滤实体
	* @param AcDbObjectIdArray 被保存的实体ID
	* @param MVStr 过滤实体的条件
	* @author 杨发荷
	* @time 2017年4月13号
	*/
	void FilterEntitys(AcDbObjectIdArray &, const MIVStr &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool QueryRegisterValue(HKEY, const TCHAR *, const TCHAR *, TCHAR *);

	/* @function 将dwg转为图片文件
	* @author 杨发荷
	* @time 2017年4月19号
	*/
	void DWGConvertToPicture();

	/* @function 获取
	* @author 杨发荷
	* @time 2017年4月19号
	*/
	bool GetPlotDevice(RasterFormat1, CString &, CString &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	void CopyPrinterConfigFile();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool GetPlotSizeName(AcDbPlotSettingsValidator *, AcDbPlotSettings *, CString &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool GetPrinterPath(CString &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool GetRasterImageFilePath(RasterFormat1, CString &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool GetZdtPathName(CString &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年06月12日
	*/
	bool GetZdtPara();

	/* @function 获取点到多边形上最近点的索引
	* @param AcGePoint2dArray 多边形的点集
	* @param AcGePoint2d 被计算的点
	* @return int 返回最近点的索引
	* @author 杨发荷
	* @time 2017年5月3号
	*/
	int GetPoint2PolyRect(AcGePoint2dArray &, const AcGePoint2d &);

	/* @接口 根据指定的名称获取注册表
	* @参数 TCHAR 注册表名称
	* @参数 CString 注册表的值
	* @返回 读取成功，返回值为true
	* @作者 杨发荷
	* @时间 2017年06月23日
	*/
	bool ReadRegForContext(const TCHAR *, CString &);

	bool ReadRegForAcceptCode(const TCHAR *, CString &);

	bool SetProjectContext();
	bool SetProjectContext1();//比上一个多了新建文件夹按钮
	/* @接口
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年06月26日
	*/
	bool SetProjectCode(const TCHAR *, bool config = false);

	/* @接口 根据指定的文件路径是否存在
	* @参数 HEKY 注册表句柄
	* @参数 TCHAR 指定文件或路径
	* @返回 bool 如果文件或路径不存在，返回值为false，否则返回true
	* @作者 杨发荷
	* @时间 2017年06月26日
	*/
	bool DirIsExist(TCHAR *);

	/* @接口 如果指定文件或路径不存在，则新创建
	* @参数 HEKY 注册表句柄
	* @参数 TCHAR 指定文件或路径
	* @返回 bool 如果文件或路径不存在，返回值为false，否则返回true
	* @作者 杨发荷
	* @时间 2017年06月26日
	*/
	bool DirIsExistEx(HKEY &, TCHAR *);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年07月06日
	*/
	bool getEntsByPoint(ads_point, AcDbObjectIdArray &);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @参数
	* @返回 double 返回角度
	* @作者 杨发荷
	* @时间 2017年07月18日
	*/
	double GetOutCornerAndPosition(AcGePoint2dArray &, int, bool);

	/* @接口
	* @参数
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年07月18日
	*/
	double getAngle(double, double, bool);

	/* @接口 获取输出目录
	* @参数 CString 业务名称
	* @参数 isCG 是否保存到成果目录中
	* @返回 CString 返回值的输出目录
	* @作者 杨发荷
	* @时间 2017年07月19日
	*/
	CString getExportFilePath(const CString &, bool isCG = false);

	/* @接口 获取输出文件名称
	* @参数 CString 业务名称
	* @返回 CString 输出文件名称
	* @作者 杨发荷
	* @时间 2017年07月19日
	*/
	CString getExportFileName(const CString &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年08月23日
	*/
	CString getExportPathName(const CString &);

	/* @接口
	* @参数
	* @参数
	* @返回
	* @作者 杨发荷
	* @时间 2017年08月24日
	*/
	void copyFiles(const CString &);

	/* @接口
	* @作者 杨发荷
	* @时间 2017年08月24日
	*/
	CString getExportFilePath();

	/* @接口
	* @作者 杨发荷
	* @时间 2017年08月24日
	*/
	CString getExportFilePath(bool isCG);

	void readTableData(MSBuild &);

	void exportTable2Doc(SmartTable::Word &);

	void exportTable2Doc(SmartTable::Word &, const MSBuild &);

	/* @接口 输出建面积报告表
	* @参数 SmartTable::Word 报告文档
	* @参数 BuildHAH 建筑面积信息
	* @参数 int word文档中面积表当前的行
	* @参数 bool 是否写入计容面积
	* @作者 杨发荷
	* @时间 2017年12月8号
	*/
	void exportTable2Doc(SmartTable::Word &, const BuildHAH &, int &, float &, bool jr = true);

public:

	bool m_b_output_raster;	// 是否输出jpg
	RasterFormat1 m_raster_format;
	CString m_str_divice;		// 打印机名称
	CString m_str_pmp_file_name;	// pmp文件名

private:

	CString m_mdbPath;
	ads_name m_CurZongDi; //当前宗地
	ads_name m_CurZRZ;		// 当前自然幢

	int m_nType;
	AcDbObjectIdArray m_TkIdA;
	AcDbObjectId      m_obCTK;
	AcDbObjectId      m_objTK;
	TCHAR m_szPath[MAX_PATH];
	TCHAR m_szTkBLock[400]; //图框块名
	TCHAR m_szBlock[50]; //宗地图块名
	CString m_str_size_name;
	double m_dTkW;
	double m_dTkH;
	double m_Scale;
	double m_ScaleFactor;

};

class XMJGExportZPT
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	XMJGExportZPT();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	~XMJGExportZPT();

	/* @接口 绘制网格
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	bool drawMesh();

	bool drawMesh(CString strTitle);

	/* @接口 开始撤回（与setUndoFlag一起使用）
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void beginUndo();

	/* @接口 设置撤回的起点（与beginUndo一起使用）
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void setUndoFlag();

	/* @接口 设置输出范围（手动框选）
	 * @返回 bool 框选范围成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	bool setExportRange();

	/* @接口 开始输出总平图
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	CString beginExport(const CString &);

	/* @接口 设置图签
	 * @参数 CString 图签名称
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void setTuQian(const CString &);

	/* @接口 设置输出范围
	 * @参数 AcGePoint2d 左下角坐标
	 * @参数 AcGePoint2d 右上角坐标
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void setExportRange(const AcGePoint2d &, const AcGePoint2d &);
	
	/* @接口 设置建筑面积
	 * @参数 double 总建筑面积
	 * @参数 double dsjzmj 地上建筑面积（默认为零）
	 * @参数 double dxjzmj 地下建筑面积（默认为零）
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void setJZMJArea(double, double dsjzmj = 0.0, double dxjzmj = 0.0);

	/* @接口 设置建筑面积
	 * @参数 double 总建筑面积
	 * @参数 double 建设用地面积
	 * @参数 double dsjzmj 地上建筑面积（默认为零）
	 * @参数 double dxjzmj 地下建筑面积（默认为零）
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void setJZMJArea(double, double , double dsjzmj = 0.0, double dxjzmj = 0.0);

	/* @接口 绘制不带图签的网格
	*/
	bool drawCompareMesh();

private:

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月25号
	 */
	void delScaleNode(const AcGePoint2d &);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void GetExternPoint(AcGePoint2d &, AcGePoint2d &);

	/* @接口 绘制十字丝
	 * @参数 AcGePoint2d 开始绘制十字丝的左下角坐标
	 * @参数 AcGePoint2d 终止绘制十字丝的右上角坐标
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void DrawCrosshair(const AcGePoint2d &, const AcGePoint2d &);

	/* @接口 绘制十字丝
	 * @参数 AcGePoint2d 开始十字丝的坐标
	 * @参数 DrawDir 十字丝的朝向（绘制边框边界的时候用到）
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void DrawCrosshair(const AcGePoint2d &, const DrawDir &);

	/* @接口 绘制图框
	 * @参数 AcGePoint2d
	 * @参数 AcGePoint2d
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void DrawTuKuang(const AcGePoint2d &, const AcGePoint2d &);

	/* @接口 添加图框标题
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &);
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &, const CString strTitle);

	/* @接口 插入图签
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	bool InsertTuQian(const AcGePoint2d &, const AcGePoint2d &);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @时间 2018年1月3号
	 */
	void AddCornerCoor(const AcGePoint2d &, const AcGePoint2d &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年7月30号
	 */
	void filterEntity(const CString &);

private:

	double m_scale;
	double m_zjzmj;
	double m_jsydmj;
	double m_dxjzmj;
	double m_dsjzmj;
	CString m_tuqian;
	AcGePoint2d m_lb;
	AcGePoint2d m_rt;

};

class CCircleOperator
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月26号
	 */
	CCircleOperator(const AcDbObjectId &);

	/* @接口 获取弧段的凸度
	 * @参数 int 获取第几个弧段
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月27号
	 */
	double getCircleBulge(int);

	/* @接口 获取弧段的半径
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月27号
	 */
	double getCircleRadius(int);

	/* @接口 获取弧段中心点的坐标
	 * @参数 int 第几个弧段
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月26号
	 */
	AcGePoint3d getArcCenterPos(int);

	/* @接口 获取圆心坐标
	 * @参数 int 获取第几个弧段的圆心
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月27号
	 */
	AcGePoint3d getCircleCenterPos(int);

	/* @接口 获取的圆心角的大小
	 * @参数 int 获取线上的第几段
	 * @参数 bool 控制返回值是弧度还是角度，默认是弧度
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月27号
	 */
	double getArcAngle(int, bool isRad = true);

private:

	AcGePoint2dArray m_node;
	AcGeDoubleArray m_bulges;

};

class CExportGCCL
{

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月23号
	 */
	CExportGCCL();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月23号
	 */
	~CExportGCCL();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月23号
	 */
	bool beginExport(const CString &);

	/* @接口 设置输出范围
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月23号
	 */
	bool setExportRange();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月23号
	 */
	bool addExportFrame(const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月23号
	 */
	bool calculateScale();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月23号
	 */
	bool scaleFontSize();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月27号
	 */
	void modifyAllEntityLineScale();

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月24号
	 */
	void replaceText(const MStr &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月26号
	 */
	bool modifyArrow();

private:

	int m_Type;
	double m_Scale;
	bool m_isExport;
	SBreakBlock *m_Block;
	AcGePoint2d m_Middle;
	AcGePoint2d m_RightTop;
	AcGePoint2d m_PFMiddle;
	AcGePoint2d m_LeftButton;
	AcGePoint3d m_PFRightTop;
	AcGePoint3d m_PFLeftButton;
	AcDbObjectIdArray m_ExportIds;

};

class XMJGExportFontSizeConfig
{
public:
	XMJGExportFontSizeConfig(int scale)
	{
		m_scale = scale;
	}

	~XMJGExportFontSizeConfig() { }

	// 获取文字图层
	bool GetTextLayers(vector<CString> &vLyrs);

	// 获取数据图层
	bool GetNumberLayers(vector<CString> &vLyrs);

	// 获取面积及备注图层
	bool GetAreaAndRemarkLayers(vector<CString> &vLyrs);

	// 获取某种类型的图层
	bool GetLayers(XMJGExportFontSizeConfigType type, vector<CString> &vLyrs);

	// 获取某种类型文字字高设置
	bool GetFontSize(XMJGExportFontSizeConfigType type, double &size);

private:
	CString GetLayerFieldName(XMJGExportFontSizeConfigType type);
private:
	int m_scale;
	IDataBaseOper m_db;
};

// 指北针配置
class XMJGExportCompassConfig
{
public:
	XMJGExportCompassConfig(int scale)
	{
		m_scale = scale;
	}

	~XMJGExportCompassConfig() { }

	// 获取指北针缩放比
	bool GetScale(double &scale);

private:
	int m_scale;
	IDataBaseOper m_db;
};


class XMJGExportLineWidthConfig
{
public:
	XMJGExportLineWidthConfig(int scale)
	{
		m_scale = scale;
	}

	~XMJGExportLineWidthConfig() { }

	// 获取某种类型的图层
	bool GetLayers(XMJGExportLineWidthConfigType type, vector<CString> &vLyrs);

	// 获取某种类型文字字高设置
	bool GetLineWidth(XMJGExportLineWidthConfigType type, double &width);

private:
	CString GetLayerFieldName(XMJGExportLineWidthConfigType type);
private:
	int m_scale;
	IDataBaseOper m_db;
};

class CSelectFolderDlg
{
public:
	//创建一个选择文件夹的对话框，返回所选路径  
	static CString Show()
	{
		TCHAR           szFolderPath[MAX_PATH] = { 0 };
		CString         strFolderPath = TEXT("");

		BROWSEINFO      sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("请选择文件夹：");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = NULL;

		// 显示文件夹选择对话框  
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名  
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				strFolderPath = szFolderPath;
			}
		}
		if (lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}
		if (!strFolderPath.IsEmpty())
			if (strFolderPath[_tcslen(strFolderPath) - 1] != '\\') strFolderPath += "\\";
		return strFolderPath;
	}
};

/* @接口 创建文件夹
 * @时间 2018年4月18日
 */
bool isExistFolder(const CString &dir);

#endif /*XMJGTOOL_H*/