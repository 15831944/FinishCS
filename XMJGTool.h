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

/* @function ���ƿ��Ƶ�
* @author ���
* @time 2017��2��24��
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

	Acad::ErrorStatus Commit();//�Ͻ�

	Acad::ErrorStatus Abort();//ȡ��

private:

	bool m_bCommit;//�Ƿ��Ѿ��Ͻ�����ȡ��

};

class XMJGPath
{
public:

	/* @function Ĭ�Ϲ��캯��
	* @author ���
	* @time 2017��4��25��
	*/
	XMJGPath();

	/* @function ���캯��
	* @param TCHAR *�ĵ�����·��
	* @param TCHAR *�����ļ�·��
	* @param TCHAR *ͼƬ�ļ���·��
	* @author ���
	* @time 2017��4��25��
	*/
	XMJGPath(const TCHAR *, const TCHAR *, const TCHAR *);

	/* @function Ĭ����������
	* @author ���
	* @time 2017��4��25��
	*/
	~XMJGPath();

	/* @function ��ȡ�����ļ�·��
	* @author ���
	* @time 2017��4��25��
	*/
	string getSavePath() const;

	/* @function �����ĵ�����·��
	* @param TCHAR *�ĵ�����·��
	* @author ���
	* @time 2017��4��25��
	*/
	void setSavePath(const TCHAR *);

	/* @function �����ĵ�����·��
	* @param CString �ĵ�����·��
	* @author ���
	* @time 2017��4��25��
	*/
	void setSavePath(const CString &);

	/* @function ��ȡ�����ļ�·��
	* @author ���
	* @time 2017��4��25��
	*/
	CString getConfigPath() const;

	/* @function ���������ļ�·��
	* @param TCHAR *�����ļ�·��
	* @author ���
	* @time 2017��4��25��
	*/
	void setConfigPath(const TCHAR *);

	/* @function ���������ļ�·��
	* @param TCHAR *�����ļ�·��
	* @author ���
	* @time 2017��4��25��
	*/
	void setConfigPath(const CString &);

	/* @function ��ȡͼƬ�ļ���·��
	* @author ���
	* @time 2017��4��25��
	*/
	string getPicturePath() const;

	/* @function ����ͼƬ�ļ���·��
	* @param TCHAR *ͼƬ�ļ���·��
	* @author ���
	* @time 2017��4��25��
	*/
	void setPicturePath(const TCHAR *);

	/* @function ����ͼƬ�ļ���·��
	* @param TCHAR *ͼƬ�ļ���·��
	* @author ���
	* @time 2017��4��25��
	*/
	void setPicturePath(const CString &);

private:

	CString m_SavePath;
	CString m_ConfigPath;
	CString m_PicturePath;

};

//��ӡͼƬ�ĸ�ʽ��ʽ
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

	/* @function ������Ƭ����
	* @author ���
	* @time 2017��2��24��
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

	/* @function ���Ʒ�������
	* *author ���
	* @time 2017��5��9��
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

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	JGExportDWG();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	~JGExportDWG();

	/* @function �������Ŀ¼
	* @param TCHAR * ����˲���Ϊ�գ���ô������Զ�����ָ��Ŀ¼
	*				 ����ᰴ��ָ����Ŀ¼���
	* @return bool ����ֵ�����������һ������Ϊ�ղ���Ч
	* @author ���
	* @time 2017��5��3��
	*/
	bool setExportDir(TCHAR *dir = NULL);

	/* @function ���������ʵ��ID���˽����bool setExportRange()��ڲ���ͬʱ���ã�
	* @param AcDbObjectIdArray �������ʵ��ID����
	* @author ���
	* @time 2017��5��3��
	*/
	void setObjects(const AcDbObjectIdArray &);

	/* @function ����ͼ��
	* @param TCHAR * ͼ�������
	* @author ���
	* @time 2017��5��3��
	*/
	void setPictureFrame(const TCHAR *, double angle = 0.0);

	/* @function ���ñ���·��
	* @param TCHAR * �ļ����������·��
	* @author ���
	* @time 2017��5��4��
	*/
	void setSavePath(const TCHAR *);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��12��20��
	 */
	void isDrawRectangle(bool isdraw = false);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��24��
	 */
	void isFilterObject(bool);

	/* @function ���ñ���·��
	* @param CString * �ļ����������·��
	* @author ���
	* @time 2017��5��4��
	*/
	void setSavePath(const CString &);

	/* @function ����ʵ������ƫ�Ƶľ��루�˽ӿڽ�����bool setExportRange()�ӿ���Ч��
	* @param double ָ��ƫ�Ƶľ���
	* @author ���
	* @time 2017��5��3��
	*/
	void setOffsetDistance(double);

	/* @function �û��Լ�ѡ��Ҫ�����Щʵ��
	* @param AcDbObjectIdArray �������ʵ��ID����
	* @author ���
	* @time 2017��5��3��
	*/
	bool setExportRange();

	/* @function ���ʵ�嵽dwg��
	* @author ���
	* @time 2017��5��3��
	*/
	bool ExportDwg();

	/* @function ���ʵ�嵽dwg��
	* @author ���
	* @time 2017��5��3��
	*/
	bool ExportDwg(MStr &, XMJGCallBack);

	/* @function ���ʵ�嵽ͼƬ��
	* @param bool �Ƿ�ɾ���м����ɵ�ͼƬ
	* @author ���
	* @time 2017��5��3��
	*/
	bool ExportPicture(bool del = false);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	bool ExportDwg2JPG(bool del = false);

	/* @�ӿ� �滻ָ�����ֶ�
	* @���� MStr ���ֶκ�ֵ���
	* @���� void
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void setReplaceField(const MStr &);

	/* @�ӿ� ����¥���
	* @���� TCHAR ¥���
	* @���� void
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void setBuildNo(TCHAR *);

	/* @�ӿ� ����¥����
	* @���� TCHAR ¥����
	* @���� void
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void setFloorNo(TCHAR *);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��26��
	*/
	void setCallBack(MStr &, XMJGCallBack);

	/* @function ���ʵ�嵽ͼƬ��
	* @param bool �Ƿ�ɾ���м����ɵ�ͼƬ
	* @author ���
	* @time 2017��5��3��
	*/
	bool saveDwgToPicture(bool flag);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	bool setReplaceChangeFlag(bool);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��9��28��
	 */
	void setObjectFilterFlag(XMJGFilterObject);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��9��
	 */
	void RegenFontSize(int scale, AcDbObjectIdArray idArr);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��9��
	 */
	void RegenCompassSize(int scale, AcDbObjectIdArray idArr);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��9��
	 */
	void RegenLineWidth(int scale, AcDbObjectIdArray idArr);

private:

	/* @�ӿ� ���ָ����
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void addCompass();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��09��16��
	 */
	void runCallBack();

	/* @�ӿ� ����Ĭ�ϵ����ű���
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool setDefaultScale();

	/* @�ӿ� �������ű���
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void CalculateScale();

	/* @�ӿ� ����ͼ��
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool InsertPictureFrame();

	/* @�ӿ� ��ʵ�屣����dwg�ļ���
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool saveDwgToFile();

	/* @�ӿ� ��ʵ���ͼ�����ϵ�һ��
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool combineEntityAndTk();

	/* @�ӿ� �ƶ�ʵ�嵽ͼ�������
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void moveEntitysToTk();

	/* @�ӿ� �滻�ֶ�
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void ReplaceFieldValue();

	/* @�ӿ� �滻��ͷ�ֶ�
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void ReplaceTableHeaderField();

	/* @�ӿ� ͳ�����
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void StatisticArea(TCHAR *, TCHAR *, TCHAR *, TCHAR *);

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	bool replaceChangeNote();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��28��
	 */
	void replaceFCPMNote();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��28��
	 */
	void filterObject();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��10��
	 */
	void calcFontSize();

	/* @�ӿ� ������Χ����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��12��20��
	 */
	void drawRectangle(AcDbObjectIdArray &);

private:

	XMJGFilterObject m_filterobject;		//ʵ����˱��
	bool m_replaceflag;
	XMJGCallBack m_callback;
	double m_compass;						//ָ����ĽǶ�
	CString m_BuildNo;						//¥���
	CString m_FloorNo;						//¥����
	MStr m_ReplaceText;						//�滻�ı�
	CString m_ExportDir;						//����ļ���
	SBreakBlock *m_Block;
	double m_scale;							//����ͼ����ʵ������ű���
	double m_OffsetDist;					//�������ľ���
	int m_DefaultScale;					//Ĭ�ϵ����ű���
	CString m_SaveDwgPath;					//dwg�����·��
	bool m_isDraw;							//�Ƿ������Χ����
	CString m_PictureFrame;					//ͼ������
	AcGePoint3d m_ObjMiddle;
	AcGePoint2d m_RightTop;					//����
	AcGePoint2d m_LeftButton;				//����
	AcGePoint3d m_tkMiddle;
	AcGePoint3d m_tkRightTop;				//ͼ�����½ǵ�
	AcGePoint3d m_tkLeftButton;				//ͼ�����½ǵ�
	AcGePoint2dArray m_Rectangular;			//�������
	AcDbObjectIdArray m_PictureFrameIds;	//ͼ��ʵ��ID����
	AcDbObjectIdArray m_Objects;			//�����ʵ��ID����
	TextProperty m_bgbj;					//���ע��
	AcDbObjectId m_projectNameid;
	AcDbObjectId m_buildCompanyid;
	bool m_isfilterobject;
};

class XMJGExport
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	XMJGExport();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	~XMJGExport();

	/* @function �������Ŀ¼
	* @param TCHAR * ����˲���Ϊ�գ���ô������Զ�����ָ��Ŀ¼
	*				 ����ᰴ��ָ����Ŀ¼���
	* @return bool ����ֵ�����������һ������Ϊ�ղ���Ч
	* @author ���
	* @time 2017��5��3��
	*/
	bool setExportDir(TCHAR *dir = NULL);

	/* @function ���������ʵ��ID���˽����bool setExportRange()��ڲ���ͬʱ���ã�
	* @param AcDbObjectIdArray �������ʵ��ID����
	* @author ���
	* @time 2017��5��3��
	*/
	void setObjects(const AcDbObjectIdArray &);

	/* @function ����ͼ��
	* @param TCHAR * ͼ�������
	* @author ���
	* @time 2017��5��3��
	*/
	void setPictureFrame(const TCHAR *, double angle = 0.0);

	/* @function ���ñ���·��
	* @param TCHAR * �ļ����������·��
	* @author ���
	* @time 2017��5��4��
	*/
	void setSavePath(const TCHAR *);

	/* @�ӿ�
	* @����
	* @���� ���
	* @ʱ�� 2017��12��20��
	*/
	void isDrawRectangle(bool isdraw = false);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��11��24��
	*/
	void isFilterObject(bool);

	/* @function ���ñ���·��
	* @param CString * �ļ����������·��
	* @author ���
	* @time 2017��5��4��
	*/
	void setSavePath(const CString &);

	/* @function ����ʵ������ƫ�Ƶľ��루�˽ӿڽ�����bool setExportRange()�ӿ���Ч��
	* @param double ָ��ƫ�Ƶľ���
	* @author ���
	* @time 2017��5��3��
	*/
	void setOffsetDistance(double);

	/* @function �û��Լ�ѡ��Ҫ�����Щʵ��
	* @param AcDbObjectIdArray �������ʵ��ID����
	* @author ���
	* @time 2017��5��3��
	*/
	bool setExportRange();

	/* @function ���ʵ�嵽dwg��
	* @author ���
	* @time 2017��5��3��
	*/
	bool ExportDwg();

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��19��
	 */
	void setScaleEntity(const AcDbObjectId &);

	/* @function ���ʵ�嵽dwg��
	* @author ���
	* @time 2017��5��3��
	*/
	bool ExportDwg(MStr &, XMJGCallBack);

	/* @function ���ʵ�嵽ͼƬ��
	* @param bool �Ƿ�ɾ���м����ɵ�ͼƬ
	* @author ���
	* @time 2017��5��3��
	*/
	bool ExportPicture(bool del = false);

	/* @�ӿ�
	* @����
	* @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��9��24��
	*/
	bool ExportDwg2JPG(bool del = false);

	/* @�ӿ� �滻ָ�����ֶ�
	* @���� MStr ���ֶκ�ֵ���
	* @���� void
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void setReplaceField(const MStr &);

	/* @�ӿ� ����¥���
	* @���� TCHAR ¥���
	* @���� void
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void setBuildNo(TCHAR *);

	/* @�ӿ� ����¥����
	* @���� TCHAR ¥����
	* @���� void
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void setFloorNo(TCHAR *);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��26��
	*/
	void setCallBack(MStr &, XMJGCallBack);

	/* @function ���ʵ�嵽ͼƬ��
	* @param bool �Ƿ�ɾ���м����ɵ�ͼƬ
	* @author ���
	* @time 2017��5��3��
	*/
	bool saveDwgToPicture(bool flag);

	/* @�ӿ�
	* @����
	* @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��9��27��
	*/
	bool setReplaceChangeFlag(bool);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��9��28��
	*/
	void setObjectFilterFlag(XMJGFilterObject);

private:

	/* @�ӿ� ���ָ����
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void addCompass();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��09��16��
	*/
	void runCallBack();

	/* @�ӿ� ����Ĭ�ϵ����ű���
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool setDefaultScale();

	/* @�ӿ� �������ű���
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void CalculateScale();

	/* @�ӿ� ����ͼ��
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool InsertPictureFrame();

	/* @�ӿ� ��ʵ�屣����dwg�ļ���
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool saveDwgToFile();

	/* @�ӿ� ��ʵ���ͼ�����ϵ�һ��
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	bool combineEntityAndTk();

	/* @�ӿ� �ƶ�ʵ�嵽ͼ�������
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void moveEntitysToTk();

	/* @�ӿ� �滻�ֶ�
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void ReplaceFieldValue();

	/* @�ӿ� �滻��ͷ�ֶ�
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void ReplaceTableHeaderField();

	/* @�ӿ� ͳ�����
	* @���� ���
	* @ʱ�� 2017��06��08��
	*/
	void StatisticArea(TCHAR *, TCHAR *, TCHAR *, TCHAR *);

	/* @�ӿ�
	* @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��9��27��
	*/
	bool replaceChangeNote();

	/* @�ӿ�
	* @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��9��28��
	*/
	void replaceFCPMNote();

	/* @�ӿ�
	* @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��9��28��
	*/
	void filterObject();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��11��10��
	*/
	void calcFontSize();

	/* @�ӿ� ������Χ����
	* @����
	* @���� ���
	* @ʱ�� 2017��12��20��
	*/
	void drawRectangle(AcDbObjectIdArray &);

private:

	XMJGFilterObject m_filterobject;		//ʵ����˱��
	bool m_replaceflag;
	XMJGCallBack m_callback;
	double m_compass;						//ָ����ĽǶ�
	CString m_BuildNo;						//¥���
	CString m_FloorNo;						//¥����
	MStr m_ReplaceText;						//�滻�ı�
	CString m_ExportDir;						//����ļ���
	SBreakBlock *m_Block;
	double m_scale;							//����ͼ����ʵ������ű���
	double m_OffsetDist;					//�������ľ���
	int m_DefaultScale;					//Ĭ�ϵ����ű���
	CString m_SaveDwgPath;					//dwg�����·��
	bool m_isDraw;							//�Ƿ������Χ����
	CString m_PictureFrame;					//ͼ������
	AcGePoint3d m_ObjMiddle;
	AcGePoint2d m_RightTop;					//����
	AcGePoint2d m_LeftButton;				//����
	AcGePoint3d m_tkMiddle;
	AcGePoint3d m_tkRightTop;				//ͼ�����½ǵ�
	AcGePoint3d m_tkLeftButton;				//ͼ�����½ǵ�
	AcGePoint2dArray m_Rectangular;			//�������
	AcDbObjectIdArray m_PictureFrameIds;	//ͼ��ʵ��ID����
	AcDbObjectIdArray m_Objects;			//�����ʵ��ID����
	TextProperty m_bgbj;					//���ע��
	AcDbObjectId m_projectNameid;
	AcDbObjectId m_buildCompanyid;
	bool m_isfilterobject;
	AcDbObjectId m_sEnt;					//�����ŵ�ʵ��
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

/* @class ���Ƽ�ͷ��
* @author ���
* @time 2017��3��
*/
class CDrawArrow
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	CDrawArrow();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	~CDrawArrow();

	AcDbObjectId DrawLeader(ads_point, ads_point, float, const TCHAR *, int);

	void getQLeaderText(ads_name head, float, const TCHAR *, int);

	AcDbObjectId DrawLeader(ads_point, ads_point, float, const TCHAR *, int, const CString &);

	AcDbObjectId DrawArrow(const AcDbObjectId &, double length = 0.54);
	/* @function�����Ƽ�ͷ
	*param AcGePoint2d
	*/
	void DrawArrow(const AcGePoint2d &, const double &, AcDbObjectId &, double length = 0.54);

	/* @�ӿ� ��עʵ��
	* @���� AcGePoint2d ��ע��λ��
	* @���� AcGePoint2d ��ע�ĵ�
	* @���� ��ע��ʵ��di
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	AcDbObjectId DrawArrow(const AcGePoint2d &, const AcGePoint2d &, double length = 0.54);

	/* @function ��ӱ��ע��
	* @param AcDbObjectId ����ӱ��ע�ǵ�ʵ��ID
	* @param AcGePoint2d ���ע�ǲ����λ��
	* @param TCHAR * ���ע�ǵ���Ϣ
	* @param AcDbObjectId �����ע��ʵ�����
	* @author ���
	* @time 2017��5��3��
	*/
	AcDbObjectId DrawArrow(AcDbObjectId &, const AcGePoint2d &, TCHAR *);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	AcDbObjectId DrawArrow(const AcGePoint2d &fPt, const AcGePoint2d &sPt, TCHAR * inserttext);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	AcDbObjectId DrawStraightArrow(const AcGePoint2d &sPt, const AcGePoint2d &ePt, double dscale = 1.0, int color = 0);

	/* @�ӿ� ���ݵ�ǰѡ�е�ʵ��ID����ȡ��ͷ��ID
	* @���� AcDbObjectId ѡ�е�ע��ID
	* @���� bool �Ƿ�ɾ������ʵ�����б���ʵ��
	* @���� AcDbObjectId ��ͷ��ID
	* @���� ���
	* @ʱ�� 2017��06��15��
	*/
	AcDbObjectId GetArrowObjectID(AcDbObjectId &, bool del = true);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��07��06��
	*/
	int getEntityByPoint(const AcGePoint2d &, AcDbObjectId &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��07��06��
	*/
	bool getEntsByPoint(ads_point, AcDbObjectIdArray &);

private:

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void GetArrowLastPoint(const AcDbObjectId &, AcGePoint2d &);

	/* @function ��ȡ��ͷ�Ĳ�����Լ��ڽǵĽ�ƽ����
	* @param AcGePoint2dArray ʵ��ĵ㼯
	* @int ʵ��㼯����Ҫ��ע�ǵĵ������
	* @bool ��˳ʱ�뻹����ʱ��
	* @ads_point �������ؼ�ͷ�Ĳ���λ��
	* @double �ڽǵĽ�ƽ����
	* @author ���
	* @time 2017��3��7��
	*/
	void GetOutCornerAndPosition(AcGePoint2dArray &, int, bool, ads_point &, double &);

	/* @function ����ı�ע��
	* @param AcDbObjectId ��ͷ�Լ��ߵ�ID��ɵ�һ������
	* @double �ڽǵĽ�ƽ����
	* @param TCHAR *����ӵ�ע����Ϣ
	* @param TCHAR *����ӵ�ע����Ϣ
	* @author ���
	* @time 2017��3��7��
	*/
	void AddTextInfo(AcDbObjectId &, double angle, TCHAR *, TCHAR * = NULL);

	/* @�ӿ� ������д��ʵ�嵱��
	* @���� AcDbObjectId ��д��ʵ��ID
	* @���� AcGePoint2d ��ά��
	* @���� ���
	* @ʱ�� 2017��06��19��
	*/
	void AddCoorToObject(AcDbObjectId &, const AcGePoint2d &);

	/* @�ӿ� ��ע�ǳ���д�뵽ʵ����
	* @���� AcDbObjectId ��д��ʵ��ID
	* @���� double ע���ߵĳ���
	* @���� ���
	* @ʱ�� 2017��06��19��
	*/
	void AddAnnotionLength(AcDbObjectId &, double);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool DrawInsertArrow(const AcGePoint2d &, const double &, AcDbObjectId &);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void DrawDiagonalLine(AcDbObjectId &, const AcGePoint2d &, const AcGePoint2d &);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void DrawHorizontalLine(AcDbObjectId &, const double &, const AcGePoint2d &, double length = 4);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void AddPropertyToXData(AcDbObjectId &, AcDbObjectId &, const TCHAR *, int idx = 0);

private:

	double m_Scale;

};

/* @class ����Ϣ��
* @author ���
* @time 2017��3��
*/
class CPointInfo
{
public:

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	CPointInfo()
	{
	}

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	CPointInfo(const VStr &str)
	{
		if(str.size() != 4) return;
		Convert(str.at(0), str.at(1), str.at(2), str.at(3));
	}

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	CPointInfo(const CString &name, const AcGePoint3d &pt)
	{
		m_Name = name;
		m_pt3.x = pt.x;
		m_pt3.y = pt.y;
		m_pt3.z = pt.z;
	}

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	CPointInfo(const CString &name, double x, double y, double z)
	{
		m_Name = name;
		m_pt3.x = x;
		m_pt3.y = y;
		m_pt3.z = z;
	}

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	CPointInfo(const CString &name, const CString &x, const CString &y, const CString &z)
	{
		Convert(name, x, y, z);
	}

	/* @deconstruction Ĭ����������
	* @author ���
	* @time 2017��2��27��
	*/
	~CPointInfo()
	{
	}

public:

	CString m_Name;    //����
	AcGePoint3d m_pt3; //����
	double m_pipeheight;//���߸߳�
	double m_dvalue;//���߲�ֵ
private:

	/* @function ��ʽ��
	* @param CString �������
	* @param CString ���x����
	* @param CString ���y����
	* @param CString ���z����
	* @author ���
	* @time 2017��2��27��
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
	/* @�ӿ� �����壬��������ͼ��Ŀ�
	* @���� ���
	* @ʱ�� 2017��06��27��
	*/
	union
	{
		int xsize;
		int width;
		int col;
	};
	/* @�ӿ� �����壬��������ͼ��ĸ�
	* @���� ���
	* @ʱ�� 2017��06��27��
	*/
	union
	{
		int ysize;
		int height;
		int row;
	};

public:

	/* @�ӿ� ��ͼƬ�ļ�
	* @���� TCHAR *ͼƬ�ļ�����
	* @���� bool ��ͼƬ�ļ��ɹ�������ֵΪtrue
	* @���� ���
	* @ʱ�� 2017��06��27��
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

	/* @�ӿ� ��ȡͼƬ�Ŀ�
	* @���� ���
	* @ʱ�� 2017��06��27��
	*/
	int ImageWidth()
	{
		return xsize;
	}

	/* @�ӿ� ��ȡͼƬ�ĸ�
	* @���� ���
	* @ʱ�� 2017��06��27��
	*/
	int ImageHeight()
	{
		return ysize;
	}

	/* @�ӿ� ��ȡͼƬ����Ĵ�С
	* @���� double ͼƬ����Ŀ�
	* @���� double ͼƬ����ĸ�
	* @���� ���
	* @ʱ�� 2017��06��27��
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

/* @class ���ſ���������
* @author ���
* @time 2017��3��
*/
class XMJGAssist
{
public:

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	XMJGAssist();

	/* @deconstruction Ĭ����������
	* @author ���
	* @time 2017��2��27��
	*/
	~XMJGAssist();

	/* @function ѡ���ļ�
	* @param CString ѡ��ѡ���ļ����й���
	* @CString ��ѡ����ļ�ȫ·����
	* @bool flag ���ļ��Ի����Ǳ����ļ��Ի���
	* @bool ѡ���ļ��ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	* @author ���
	* @time 2017��2��27��
	*/
	bool SelectFile(const CString &, const CString &, CString &, bool flag = true);

	/* @function ���ض����ַ���תΪ��
	* @param CString �ض����ַ���
	* @param ACGePoint2d �������
	* @return ת���ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @author ���
	* @time 2017��2��27��
	*/
	bool StrConvertToPoint(const CString &, AcGePoint2d &);

	/* @function ��ȡdat�ļ����������ļ�
	* @param CString �ļ�ȫ·����
	* @param VPInfo �����Ϣ����
	* @param VStr ��ȡ�ļ�ʱ�Ĵ�����Ϣ
	* @author ���
	* @time 2017��2��27��
	*/
	void PurgeDatFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author ���
	* @time 2017��11��15��
	*/
	void PurgeRTKFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author ���
	* @time 2017��11��15��
	*/
	void PurgeQZYFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function ��ȡExcel�ļ����������ļ�
	* @param CString �ļ�ȫ·����
	* @param VPInfo �����Ϣ����
	* @param VStr ��ȡ�ļ�ʱ�Ĵ�����Ϣ
	* @author ���
	* @time 2017��3��7��
	*/
	void PurgeExcelFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function ��ȡExcel�ļ��ܵ��
	*/
	void PipeExcel(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author ���
	* @time 2017��11��15��
	*/
	void PurgeExcelQZY(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @function
	* @param
	* @param
	* @param
	* @param
	* @author ���
	* @time 2017��11��15��
	*/
	void PurgeExcelRTK(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* function ���Ƶ�
	* param VPInfo ������
	* @author ���
	* @time 2017��3��7��
	*/
	void DrawPoint(const CPointInfo &);

	/* function ���Ƶ㼯
	* param VPInfo ��ļ���
	* @author ���
	* @time 2017��2��27��
	*/
	void DrawPoint(const VPInfo &);

	/* @function ��������
	* @param TCHAR * �������ڵ�ͼ��
	* @param TCHAR * ���ߵı���
	* @param int * ���ߵ���ɫ
	* @param closeornot * �Ƿ���ʾ�պϣ�Ĭ��Ϊ��ʾ��
	* @return AcDbObjectId ���ػ��Ƶ�����id
	* @author ���
	* @time 2017��3��9��
	*/
	AcDbObjectId DrawLine(const TCHAR *, const TCHAR *, const int &, TCHAR *Style = NULL, bool closeornot = true);

	/* @function ��������
	* @param AcDbObjectId ����ʵ���id
	* @param TCHAR * �������ڵ�ͼ��
	* @param TCHAR * ���ߵı���
	* @param int * ���ߵ���ɫ
	* @param closeornot * �Ƿ���ʾ�պϣ�Ĭ��Ϊ��ʾ��
	* @return bool ����ֵΪtrue��ʵ��Ϊ�պϣ�����ʵ��û�бպ�
	* @author ���
	* @time 2017��3��9��
	*/
	bool DrawLine(AcDbObjectId &, const TCHAR *, const TCHAR *, const int &, TCHAR *Style = NULL, bool closeornot = true);

	/* @function ��������
	* @param TCHAR * �������ڵ�ͼ��
	* @param TCHAR * ���ߵı���
	* @param int * ���ߵ�R
	* @param int * ���ߵ�G
	* @param int * ���ߵ�B
	* @param closeornot * �Ƿ���ʾ�պϣ�Ĭ��Ϊ��ʾ��
	* @return AcDbObjectId ���ػ��Ƶ�����id
	* @author ���
	* @time 2017��3��9��
	*/
	AcDbObjectId DrawLine(const TCHAR *, const TCHAR *, const int &, const int &, const int &, TCHAR *Style = NULL, bool closeornot = true);

	/* @function ��ӡ������Ϣ
	* @param VString �ռ�������Ϣ
	* @author ���
	* @time 2017��2��27��
	*/
	void PrintErrorInfo(const VStr &);

	/* @function �Զ���������
	* @param bool �Ƿ�������ǵ㿪ʼ���
	* @param pre ��ŵ�ǰ׺
	* @author ���
	* @time 2017��2��27��
	*/
	void AutoCreateCode(const AcDbObjectId &, bool flag = true, TCHAR *pre = NULL);

	/* @function ����ʵ����ӱ���
	* @param AcDbObjectId ��ʵ��ID
	* @param TCHAR ��ŵ�ǰ׺
	* @author ���
	* @time 2017��2��27��
	*/
	void AddPointCode(const AcDbObjectId &, TCHAR *);

	/* @function �����ı��
	* @param AcDbObjectId ������ĵ�ʵ��ID
	* @author ���
	* @time 2017��2��27��
	*/
	void ClearPointCode(const AcDbObjectId &);

	/* @function ����ʵ��Ľڵ�˳��
	* @param AcDbObjectId ʵ��id
	* @param int ʵ��ڵ��˳��
	* @param int ʵ��ڵ����ʵλ��
	* @author ���
	* @time 2017��2��13��
	*/
	void AdjustNodeOrder(const AcDbObjectId &, int, int);

	/*  @function ����ע����ʵ��ID�����в���ָ����ʵ��
	* @param AcDbObjectIdArray ʵ��ID����
	* @param TCHAR ע��
	* @param AcDbObjectId �ҵ���ʵ��ID
	* @author ���
	* @time 2017��2��27��
	*/
	void findEntFromZJ(const AcDbObjectIdArray &, TCHAR *, AcDbObjectId &);

	/* @function �ӵ㼯��ɾ��ָ��ʵ�壨ע�⣺ʵ������ǵ�ʵ�壩
	* @param AcDbObjectId ��ʵ��ID
	* @author ���
	* @time 2017��2��27��
	*/
	void RemoveNodeFromID(AcGePoint2dArray &, const AcDbObjectId &);

	/* @function ɾ���ڵ�
	* @param AcGePoint2dArray
	* @param AcDbObjectIdArray
	* @param bool ���flagΪtrue����ô����ɾ���ڶ��������е�����ʵ��id��
	���flagΪfalse����ôɾ���ڶ���������ÿ��ʵ������е�����ʵ��id
	* @author ���
	* @time 2017��2��16��
	*/
	void RemoveNodeFromID(AcGePoint2dArray &, const AcDbObjectIdArray &, bool flag = true);

	/* @function Ϊʵ�����ע��
	* @param AcDbObjectId ��Ҫ���ע�ǵ�ʵ��ID
	* @author ���
	* @time 2017��2��27��
	*/
	void addAnnotation(const AcDbObjectId &);

	/* @function ��鹤����Ϣ�����Ƿ��걸
	* @author ����
	* @time 2017��3��3��
	*/
	bool checkProjInfo(int pro_type);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool GetExportEntitys(AcDbObjectIdArray &, const AcGePoint2dArray &nodes);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool GetExportRange(AcGePoint2d &, AcGePoint2d &);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void GetExportRange(const AcGePoint2d &, const AcGePoint2d &, AcGePoint2dArray &);

	/* @��ȡ���ű���
	* @param tklb ͼ������½ǵ�
	* @param tkrt ͼ������Ͻǵ�
	* @param entlb ʵ������½ǵ�
	* @param entrt ʵ������½ǵ�
	* @return double ����ϵ��
	* @author ���
	* @time 2017��3��8��
	*/
	double GetScale(const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &, const AcGePoint2d &);

	/* @function ����ʵ�嵽�ļ���
	* @param AcDbObjectIdArray �������ʵ�弯��
	* @return bool ����ɹ�����ֵΪtrue�����򷵻�ֵfalse
	* @author ���
	* @time 2017��3��8��
	*/
	bool SaveEntitysToDWG(const CString &);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void SetAzimuthInfo(int, TCHAR *);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void SetAnnotationType();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void GetAnnotationType(TCHAR *);

	/* @function ���ע����Ϣ
	* @param AcDbObjectId ���ע����Ϣ��ʵ��
	* @param int ע��ʵ�����ɫ
	* @param TCHAR ע�ǵ���Ϣ
	* @author ���
	* @time 2017��3��13��
	*/
	void AddAnnotationInfo(const AcDbObjectId &, int color, TCHAR *info);

	/* @function ����ʵ�嵽dwg�ļ���
	* @param AcGePoint3D ʵ�����ŵĵ�
	* @param double ʵ�����ű���
	* @author ���
	* @time 2017��3��13��
	*/
	bool SaveEntitysToDWG(const AcGePoint3d &, const double &);

	/* @function ��ȡ����ע��
	* @param AcDbObjectArray ע��ʵ��ID����
	* @param TCHAR ע����Ϣ���ڵ�ͼ��
	* @param MStr ����ֵһһ��Ӧ����Щ��Ϣ����д��ʵ�嵱��
	* @author ���
	* @time 2017��3��13��
	*/
	void AddAnnotationInfo(const AcDbObjectId &, TCHAR *, MStr &);

	/* @function ���ע����Ϣ
	* @param AcDbObjectArray ��Ҫ���ע��ʵ��ID����
	* @param TCHAR ע����Ϣ���ڵ�ͼ��
	* @param MStr ����ֵһһ��Ӧ����Щ��Ϣ����д��ʵ�嵱��
	* @author ���
	* @time 2017��3��13��
	*/
	void AddAnnotationInfo(const AcDbObjectIdArray &, TCHAR *, MStr &);

	/* @function ��ȡ���ߺͽ�����֮���ע��ʵ��
	* @param AcDbObjectArray ע��ʵ��ID����
	* @author ���
	* @time 2017��3��13��
	*/
	void GetRedLineAndBuildAnnotation(AcDbObjectIdArray &);

	/* @function ��ȡ����ע��
	* @param AcDbObjectArray ע��ʵ��ID����
	* @author ���
	* @time 2017��3��13��
	*/
	void GetBuildAnnotation(AcDbObjectIdArray &);
	
	/* @function ��ȡ��������߽���֮��ľ���
	* @param ��������߽���֮��ľ���
	* @author ���
	* @time 2017��3��13��
	*/
	void GetEdgeDistanceInfo(CString &);

	/* @function ��ȡ������֮��ľ���
	* @return CString ������֮��ľ���
	* @author ���
	* @time 2017��3��13��
	*/
	void GetBuildDistanceInfo(CString &);

	/* @function ��ȡ��������߽���֮��ľ���
	* @param ��������߽���֮��ľ���
	* @author ���
	* @time 2017��3��13��
	*/
	void GetEdgeDistanceText(CString &);

	/* @function ��ȡ������֮��ľ���
	* @return CString ������֮��ľ���
	* @author ���
	* @time 2017��3��13��
	*/
	void GetBuildDistanceText(CString &);

	/* @function ��ȡ��ע�ڵ���Ϣ
	* @param AcDbobjectIdArray ��עʵ��ID����
	* @param vector<AzimuthDist> ��λ���Լ�����ļ���
	* @return AcDbObjectId ��Ŀ��Χ��
	* @author ���
	* @time 2017��3��13��
	*/
	void GetAzimuthDist(AcDbObjectIdArray &, vector<AzimuthDist> &);

	/* @function ������Ŀ��Χ�ߣ�ѡ������ȷ����
	* @return AcDbObjectId ��Ŀ��Χ��
	* @author ����
	* @time 2017��3��13��
	*/
	AcDbObjectId CreateScopeLine();

	/* @function ���Ʋ�ͬ���ͺ͵ȼ��Ĳ�����
	* @return AcDbObjectId ������
	* @author ����
	* @time 2017��3��21��
	*/
	AcDbObjectId drawControlPoint(CString, CString, CString, CString, AcGePoint2d);

	/* @function ��ѡ�߳�Ϊ10���������ķ�Χ��
	* @return AcDbObjectId ��Χ��ID(����ͼ��Ϊ"�滮������")
	* @author ����
	* @time 2017��3��24��
	*/
	AcDbObjectId DrawFinalScopeLine();

	/* @function ���ݵ�����ȡ���ʵ��
	���磺�ڻ��ƺ��ߵ�ʱ�򣬺����ϵ�ÿ���ڵ㶼���ڵ���ĵ��ϻ��Ƶģ������ڻ�ȡ�����ϵĽڵ������������ȡ����ĵ㡣
	* @return AcDbObjectId ��ʵ��ID
	* @author ���
	* @time 2017��3��30��
	*/
	AcDbObjectId GetPointEntByPoint(const AcGePoint2d &);

	/* @function �߽�ע��
	* @param AcDbObjectId ��Ҫ��ע��ʵ��id
	* @return AcDbObjectIdArray ��ע���ŵ�ID����
	* @author ���
	* @time 2017��3��30��
	*/
	AcDbObjectIdArray BoundaryAnnotation(const AcDbObjectId &);

	/* @function �߽�ע��
	* @param AcGePoint2dArray ��Ҫ��ע��ʵ��㼯
	* @return AcDbObjectIdArray ��ע���ŵ�ID����
	* @author ���
	* @time 2017��3��30��
	*/
	AcDbObjectIdArray BoundAryAnnotation(const AcGePoint2dArray &);

	/* @function �߽�ע��
	* @param AcGePoint2d �߽��ע�����
	* @param AcGePoint2d �߽��ע���յ�
	* @return AcDbObjectId ��ע���ŵ�id����ID��һ��group)
	* @author ���
	* @time 2017��3��30��
	*/
	AcDbObjectId BoundAryAnnotation(const AcGePoint2d &, const AcGePoint2d &);

	/* @function ���Ƹ���
	* @Attention ���ݵ�ǰͼ���ϵ�ʵ�����귶Χ������
	* @author ���
	* @time 2017��4��11��
	*/
	void DrawMesh(CString title = _T(""));

	/* @function ���Ƹ���
	* @Attention ���ݵ�ǰͼ���ϵ�ʵ�����귶Χ������
	* @author ���
	* @time 2017��4��11��
	*/
	void DrawMesh(const AcDbObjectIdArray &, CString title = _T(""));

	/* @function ���Ƹ���
	* @param AcGePoint2d ����ͼ�������
	* @param AcGePoint2d ����ͼ�������
	* @Attention ������������������Խ������꣬һ���ǶԽ������
	* @author ���
	* @time 2017��4��11��
	*/
	void DrawMesh(const AcGePoint2d &, const AcGePoint2d &, ExportDWGType);

	/*��������ͱ���-���߻���ר��
	 * nTable 0:��ƽͼ
	 nTable 1:����ͼ
	 */
	AcDbObjectIdArray DrawMesh(AcGePoint2d &, AcGePoint2d &, const CString &strGXJM, int nTable = 0);

	/* @function ��ȡ�㼯������������������Ͻǵ�
	* @param AcGePoint2d ������½ǵ�
	* @param AcGePoint2d ������Ͻǵ�
	* @author ���
	* @time 2017��4��11��
	*/
	void GetExternPoint(const AcGePoint2dArray &, AcGePoint2d &, AcGePoint2d &);

	/* @function ��������������Ͻǵ��������������
	* @param AcGePoint2d ������½ǵ�
	* @param AcGePoint2d ������Ͻǵ�
	* @param AcGePoint2d ���������½ǵ�
	* @param AcGePoint2d ���������Ͻǵ�
	* @author ���
	* @time 2017��4��11��
	*/
	void GetExternPoint(const AcGePoint2d &, const AcGePoint2d &, AcGePoint2d &, AcGePoint2d &);

	/* @function ����ͼ���ڵ�ʮ��˿
	* @param AcGePoint2d ������ʵ������½ǵ�
	* @param DrawDir ����ʮ��˿�ķ���
	* @author ���
	* @time 2017��4��13��
	*/
	void DrawCrosshair(const AcGePoint2d &, const DrawDir &);

	/* @function ����ͼ���ڵ�ʮ��˿
	* @param AcGePoint2d ������ʵ������½ǵ�
	* @param AcGePoint2d ������ʵ������Ͻǵ�
	* @author ���
	* @time 2017��4��13��
	*/
	void DrawCrosshair(const AcGePoint2d &, const AcGePoint2d &);

	/* @function ����ʵ��
	* @param AcDbObjectIdArray �������ʵ��ID
	* @author ���
	* @time 2017��4��13��
	*/
	void DrawTuKuang(const AcGePoint2d &, const AcGePoint2d &);

	/* @function ���ͼ���ĸ��ǵ�����
	* @param AcGePoint2d ������ʵ������½ǵ�
	* @param AcGePoint2d ������ʵ������Ͻǵ�
	* @author ���
	* @time 2017��4��13��
	*/
	void AddCornerCoor(const AcGePoint2d &, const AcGePoint2d &);

	/* @function ���ͼ�����
	* @param AcGePoint2d ������ʵ������½ǵ�
	* @param AcGePoint2d ������ʵ������Ͻǵ�
	* @author ���
	* @time 2017��4��13��
	*/
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &, ExportDWGType);

	/*���Ʊ���-����ר��
	*/
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &, const CString &, int nTable = 0);

	/* @function ����ͼǩ
	* @param AcGePoint2d ������ʵ������½ǵ�
	* @param AcGePoint2d ������ʵ������Ͻǵ�
	* @author ���
	* @time 2017��4��13��
	*/
	bool InsertTuQian(const AcGePoint2d &, const AcGePoint2d &);

	/* @function ����ʵ��
	* @param AcDbObjectIdArray �������ʵ��ID
	* @param MStr ����ʵ�������
	* @author ���
	* @time 2017��4��13��
	*/
	void FilterEntitys(AcDbObjectIdArray &, const MStr &);

	/* @function ����ʵ��
	* @param AcDbObjectIdArray �������ʵ��ID
	* @param MVStr ����ʵ�������
	* @author ���
	* @time 2017��4��13��
	*/
	void FilterEntitys(AcDbObjectIdArray &, const MVStr &);

	/* @function ����ʵ��
	* @param AcDbObjectIdArray �������ʵ��ID
	* @param MVStr ����ʵ�������
	* @author ���
	* @time 2017��4��13��
	*/
	void FilterEntitys(AcDbObjectIdArray &, const MIVStr &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool QueryRegisterValue(HKEY, const TCHAR *, const TCHAR *, TCHAR *);

	/* @function ��dwgתΪͼƬ�ļ�
	* @author ���
	* @time 2017��4��19��
	*/
	void DWGConvertToPicture();

	/* @function ��ȡ
	* @author ���
	* @time 2017��4��19��
	*/
	bool GetPlotDevice(RasterFormat1, CString &, CString &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void CopyPrinterConfigFile();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool GetPlotSizeName(AcDbPlotSettingsValidator *, AcDbPlotSettings *, CString &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool GetPrinterPath(CString &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool GetRasterImageFilePath(RasterFormat1, CString &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool GetZdtPathName(CString &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	bool GetZdtPara();

	/* @function ��ȡ�㵽�����������������
	* @param AcGePoint2dArray ����εĵ㼯
	* @param AcGePoint2d ������ĵ�
	* @return int ��������������
	* @author ���
	* @time 2017��5��3��
	*/
	int GetPoint2PolyRect(AcGePoint2dArray &, const AcGePoint2d &);

	/* @�ӿ� ����ָ�������ƻ�ȡע���
	* @���� TCHAR ע�������
	* @���� CString ע����ֵ
	* @���� ��ȡ�ɹ�������ֵΪtrue
	* @���� ���
	* @ʱ�� 2017��06��23��
	*/
	bool ReadRegForContext(const TCHAR *, CString &);

	bool ReadRegForAcceptCode(const TCHAR *, CString &);

	bool SetProjectContext();
	bool SetProjectContext1();//����һ�������½��ļ��а�ť
	/* @�ӿ�
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��26��
	*/
	bool SetProjectCode(const TCHAR *, bool config = false);

	/* @�ӿ� ����ָ�����ļ�·���Ƿ����
	* @���� HEKY ע�����
	* @���� TCHAR ָ���ļ���·��
	* @���� bool ����ļ���·�������ڣ�����ֵΪfalse�����򷵻�true
	* @���� ���
	* @ʱ�� 2017��06��26��
	*/
	bool DirIsExist(TCHAR *);

	/* @�ӿ� ���ָ���ļ���·�������ڣ����´���
	* @���� HEKY ע�����
	* @���� TCHAR ָ���ļ���·��
	* @���� bool ����ļ���·�������ڣ�����ֵΪfalse�����򷵻�true
	* @���� ���
	* @ʱ�� 2017��06��26��
	*/
	bool DirIsExistEx(HKEY &, TCHAR *);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��07��06��
	*/
	bool getEntsByPoint(ads_point, AcDbObjectIdArray &);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� double ���ؽǶ�
	* @���� ���
	* @ʱ�� 2017��07��18��
	*/
	double GetOutCornerAndPosition(AcGePoint2dArray &, int, bool);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��07��18��
	*/
	double getAngle(double, double, bool);

	/* @�ӿ� ��ȡ���Ŀ¼
	* @���� CString ҵ������
	* @���� isCG �Ƿ񱣴浽�ɹ�Ŀ¼��
	* @���� CString ����ֵ�����Ŀ¼
	* @���� ���
	* @ʱ�� 2017��07��19��
	*/
	CString getExportFilePath(const CString &, bool isCG = false);

	/* @�ӿ� ��ȡ����ļ�����
	* @���� CString ҵ������
	* @���� CString ����ļ�����
	* @���� ���
	* @ʱ�� 2017��07��19��
	*/
	CString getExportFileName(const CString &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��08��23��
	*/
	CString getExportPathName(const CString &);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��08��24��
	*/
	void copyFiles(const CString &);

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��08��24��
	*/
	CString getExportFilePath();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��08��24��
	*/
	CString getExportFilePath(bool isCG);

	void readTableData(MSBuild &);

	void exportTable2Doc(SmartTable::Word &);

	void exportTable2Doc(SmartTable::Word &, const MSBuild &);

	/* @�ӿ� �������������
	* @���� SmartTable::Word �����ĵ�
	* @���� BuildHAH ���������Ϣ
	* @���� int word�ĵ��������ǰ����
	* @���� bool �Ƿ�д��������
	* @���� ���
	* @ʱ�� 2017��12��8��
	*/
	void exportTable2Doc(SmartTable::Word &, const BuildHAH &, int &, float &, bool jr = true);

public:

	bool m_b_output_raster;	// �Ƿ����jpg
	RasterFormat1 m_raster_format;
	CString m_str_divice;		// ��ӡ������
	CString m_str_pmp_file_name;	// pmp�ļ���

private:

	CString m_mdbPath;
	ads_name m_CurZongDi; //��ǰ�ڵ�
	ads_name m_CurZRZ;		// ��ǰ��Ȼ��

	int m_nType;
	AcDbObjectIdArray m_TkIdA;
	AcDbObjectId      m_obCTK;
	AcDbObjectId      m_objTK;
	TCHAR m_szPath[MAX_PATH];
	TCHAR m_szTkBLock[400]; //ͼ�����
	TCHAR m_szBlock[50]; //�ڵ�ͼ����
	CString m_str_size_name;
	double m_dTkW;
	double m_dTkH;
	double m_Scale;
	double m_ScaleFactor;

};

class XMJGExportZPT
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	XMJGExportZPT();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	~XMJGExportZPT();

	/* @�ӿ� ��������
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	bool drawMesh();

	bool drawMesh(CString strTitle);

	/* @�ӿ� ��ʼ���أ���setUndoFlagһ��ʹ�ã�
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void beginUndo();

	/* @�ӿ� ���ó��ص���㣨��beginUndoһ��ʹ�ã�
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void setUndoFlag();

	/* @�ӿ� ���������Χ���ֶ���ѡ��
	 * @���� bool ��ѡ��Χ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	bool setExportRange();

	/* @�ӿ� ��ʼ�����ƽͼ
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	CString beginExport(const CString &);

	/* @�ӿ� ����ͼǩ
	 * @���� CString ͼǩ����
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void setTuQian(const CString &);

	/* @�ӿ� ���������Χ
	 * @���� AcGePoint2d ���½�����
	 * @���� AcGePoint2d ���Ͻ�����
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void setExportRange(const AcGePoint2d &, const AcGePoint2d &);
	
	/* @�ӿ� ���ý������
	 * @���� double �ܽ������
	 * @���� double dsjzmj ���Ͻ��������Ĭ��Ϊ�㣩
	 * @���� double dxjzmj ���½��������Ĭ��Ϊ�㣩
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void setJZMJArea(double, double dsjzmj = 0.0, double dxjzmj = 0.0);

	/* @�ӿ� ���ý������
	 * @���� double �ܽ������
	 * @���� double �����õ����
	 * @���� double dsjzmj ���Ͻ��������Ĭ��Ϊ�㣩
	 * @���� double dxjzmj ���½��������Ĭ��Ϊ�㣩
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void setJZMJArea(double, double , double dsjzmj = 0.0, double dxjzmj = 0.0);

	/* @�ӿ� ���Ʋ���ͼǩ������
	*/
	bool drawCompareMesh();

private:

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��25��
	 */
	void delScaleNode(const AcGePoint2d &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void GetExternPoint(AcGePoint2d &, AcGePoint2d &);

	/* @�ӿ� ����ʮ��˿
	 * @���� AcGePoint2d ��ʼ����ʮ��˿�����½�����
	 * @���� AcGePoint2d ��ֹ����ʮ��˿�����Ͻ�����
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void DrawCrosshair(const AcGePoint2d &, const AcGePoint2d &);

	/* @�ӿ� ����ʮ��˿
	 * @���� AcGePoint2d ��ʼʮ��˿������
	 * @���� DrawDir ʮ��˿�ĳ��򣨻��Ʊ߿�߽��ʱ���õ���
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void DrawCrosshair(const AcGePoint2d &, const DrawDir &);

	/* @�ӿ� ����ͼ��
	 * @���� AcGePoint2d
	 * @���� AcGePoint2d
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void DrawTuKuang(const AcGePoint2d &, const AcGePoint2d &);

	/* @�ӿ� ���ͼ�����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &);
	void AddTuKuangTitle(const AcGePoint2d &, const AcGePoint2d &, const CString strTitle);

	/* @�ӿ� ����ͼǩ
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	bool InsertTuQian(const AcGePoint2d &, const AcGePoint2d &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void AddCornerCoor(const AcGePoint2d &, const AcGePoint2d &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��7��30��
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

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��26��
	 */
	CCircleOperator(const AcDbObjectId &);

	/* @�ӿ� ��ȡ���ε�͹��
	 * @���� int ��ȡ�ڼ�������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��27��
	 */
	double getCircleBulge(int);

	/* @�ӿ� ��ȡ���εİ뾶
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��27��
	 */
	double getCircleRadius(int);

	/* @�ӿ� ��ȡ�������ĵ������
	 * @���� int �ڼ�������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��26��
	 */
	AcGePoint3d getArcCenterPos(int);

	/* @�ӿ� ��ȡԲ������
	 * @���� int ��ȡ�ڼ������ε�Բ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��27��
	 */
	AcGePoint3d getCircleCenterPos(int);

	/* @�ӿ� ��ȡ��Բ�ĽǵĴ�С
	 * @���� int ��ȡ���ϵĵڼ���
	 * @���� bool ���Ʒ���ֵ�ǻ��Ȼ��ǽǶȣ�Ĭ���ǻ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��27��
	 */
	double getArcAngle(int, bool isRad = true);

private:

	AcGePoint2dArray m_node;
	AcGeDoubleArray m_bulges;

};

class CExportGCCL
{

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��23��
	 */
	CExportGCCL();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��23��
	 */
	~CExportGCCL();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��23��
	 */
	bool beginExport(const CString &);

	/* @�ӿ� ���������Χ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��23��
	 */
	bool setExportRange();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��23��
	 */
	bool addExportFrame(const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��23��
	 */
	bool calculateScale();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��23��
	 */
	bool scaleFontSize();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��27��
	 */
	void modifyAllEntityLineScale();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��24��
	 */
	void replaceText(const MStr &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��26��
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

	// ��ȡ����ͼ��
	bool GetTextLayers(vector<CString> &vLyrs);

	// ��ȡ����ͼ��
	bool GetNumberLayers(vector<CString> &vLyrs);

	// ��ȡ�������עͼ��
	bool GetAreaAndRemarkLayers(vector<CString> &vLyrs);

	// ��ȡĳ�����͵�ͼ��
	bool GetLayers(XMJGExportFontSizeConfigType type, vector<CString> &vLyrs);

	// ��ȡĳ�����������ָ�����
	bool GetFontSize(XMJGExportFontSizeConfigType type, double &size);

private:
	CString GetLayerFieldName(XMJGExportFontSizeConfigType type);
private:
	int m_scale;
	IDataBaseOper m_db;
};

// ָ��������
class XMJGExportCompassConfig
{
public:
	XMJGExportCompassConfig(int scale)
	{
		m_scale = scale;
	}

	~XMJGExportCompassConfig() { }

	// ��ȡָ�������ű�
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

	// ��ȡĳ�����͵�ͼ��
	bool GetLayers(XMJGExportLineWidthConfigType type, vector<CString> &vLyrs);

	// ��ȡĳ�����������ָ�����
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
	//����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��  
	static CString Show()
	{
		TCHAR           szFolderPath[MAX_PATH] = { 0 };
		CString         strFolderPath = TEXT("");

		BROWSEINFO      sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("��ѡ���ļ��У�");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = NULL;

		// ��ʾ�ļ���ѡ��Ի���  
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			// ȡ���ļ�����  
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

/* @�ӿ� �����ļ���
 * @ʱ�� 2018��4��18��
 */
bool isExistFolder(const CString &dir);

#endif /*XMJGTOOL_H*/