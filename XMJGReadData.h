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

	/* @�ӿ� Ĭ�Ϲ���ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	IProjectInfo(const CString &);

	~IProjectInfo();

	bool CreateProject(const MStr &);

	bool CreateProject(const CString &, const MStr &);

	bool CreateProject(const MStr &, CString &);

	bool CreateProject(const CString &, const CString &);

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��5��24��
	 */
	bool CreateProjectEx(const MStr &, const CString &);

	bool CreateProjectEx(const MStr &, CString &, const CString &);

private:

	void updataProjectInfo(CString = _T(""));

	void updataDWG(CString = _T("GC\\��Ŀͼ��.dwg"));

	void createProjectDir(CString = _T("GC\\��Ŀͼ��.dwg"));

	void createProjectDirEx(const CString &, CString = _T("GC\\��Ŀͼ��.dwg"));

private:

	MStr m_record;
	CString m_dir;
	CString m_name;
	CString m_code;
	CString m_type;
	CDatabaseOperManager *_manager;

};

/* @class ���ſ�����һ��ҵ����
* @author ���
* @time 2017��3��
*/
class XMJGRedLineSettingOut
{
public:

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	XMJGRedLineSettingOut();

	/* @deconstruction Ĭ����������
	* @author ���
	* @time 2017��2��27��
	*/
	~XMJGRedLineSettingOut();

public:

	/* @�ӿ� ��������
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void ImportData();

	/* @�ӿ� ��������
	 * @���� ���
	 * @ʱ�� 2017��11��17��
	 */
	void setProperty();

	/* @�ӿ� ��������
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void CreateRLProject();

	/* @function ��ȡdat�ļ�
	* @author ���
	* @time 2017��2��27��
	*/
	void ReadDatFile();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��28��
	 */
	void ReadReveseDat();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��08��28��
	 */
	void ReadReveseExcel();

	/* @function ��ȡExcel�ļ�
	* @author ���
	* @time 2017��3��7��
	*/
	void ReadExcelFile();

	/* @function ��ȡExcel�ļ�
	* @author ���
	* @time 2017��3��7��
	*/
	void ManualDrawPoint();

	/* @function ���ƺ�����
	* @author ���
	* @time 2017��2��27��
	*/
	void DrawRedLine();

	/* @function��������ӽڵ�
	* @author ���
	* @time 2017��2��27��
	*/
	void AddRedLineNode();

	/* @function ɾ�����߽ڵ�
	* @author ���
	* @time 2017��2��27��
	*/
	void DelRedLineNode();

	/* @function ɾ�����߽ڵ㣨��ǿ�棩
	* @author ���
	* @time 2017��2��27��
	*/
	void DelRedLineNodeEx();

	/* @function �ֶ������ڵ�˳��
	* @author ���
	* @time 2017��2��27��
	*/
	void AdjustNodesOrder();

	/* @function �ֶ����ע��
	* @author ���
	* @time 2017��2��27��
	*/
	void AddAnnotation();

	/* @function �ֶ����ע��
	* @author ���
	* @time 2017��2��
	*/
	void ManualAnnotation();

	/* @function �Զ����ע��
	* @author ���
	* @2017��2��
	*/
	void AutoAnnotation();

	/* @function �ƶ�ע��
	* @author ���
	* @time 2017��2��
	*/
	void MoveAnnotation();

	/* @function �ֶ���ӳ���ע��
	* @author ����
	* @time 2017��2��
	*/
	void ManualLineAnnotation();

	/* @function ���Ʒ�Χ��
	* @author ����
	* @time 2017��2��27��
	*/
	void DrawScopeLine();

	/* @function ������Ϣ
	* @author ����
	* @time 2017��3��
	*/
	void RedlineSetAttatchedInfo();

	/* @function ��ȡע�����Ϣ��������Ϣ�滻������
	* @author ���
	* @param SBreakBlock ʵ���
	* @time 2017��3��13��
	*/
	void ReadRegisterValue(SBreakBlock *);

	/* @function ��ȡע�����Ϣ��������Ϣ�滻������
	* @author ���
	* @param SBreakBlock ʵ���
	* @time 2017��3��13��
	*/
	void GetReplaceValue(MStr &);

	/* @function ���ͼ��ʵ�嵽dwg��
	* @author ���
	* @time 2017��3��
	*/
	void RedLineExportDWG();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��21��
	 */
	void InputCoorPos();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��18��
	 */
	void readLineAddNode();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��7��
	 */
	void redDistAnnotion();

private:

	/* @�ӿ� ɾ�������ע
	 * @���� AcDbObjectId ��ע��ʵ��
	 * @���� ���
	 * @ʱ�� 2017��07��18��
	 */
	void delAnnotation(AcDbObjectId &);

	/* @�ӿ� ע��һ��ʵ�����нڵ�
	 * @���� AcDbOjbectId ��ע�ǵ�ʵ��
	 * @���� double ע�ǵĳ���
	 * @���� ���
	 * @ʱ�� 2017��07��18��
	 */
	void autoAnnotation(AcDbObjectId &, double);

	/* @�ӿ� ��ȡ������
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool getInputPoint(AcGePoint2d &);

	/* @�ӿ� ��ȡ������
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool getInputManual(AcGePoint2d &);

	/* @�ӿ� ��ȡ������
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool getInputScreen(AcGePoint2d &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void purgeTxtDataFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void purgeDatDataFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void purgeExcelDataFile(const CString &, VPInfo &, VStr &, bool reverse = false);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void drawPoint(const VPInfo &);

private:

	double m_Scale;
	CString m_Code;
	CString m_SaveDir;
	TextProperty m_hxjdzj;	//���߽ڵ�
	TextProperty m_ywcd;	//Ұ����ע��
	TextProperty m_ywcdtk;	//Ұ����ͼ��

};

/* @class ���ſ����ڶ���ҵ����
* @author ���
* @time 2017��3��
*/
class XMJGBuildSettingOut
{

public:

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��2��27��
	*/
	XMJGBuildSettingOut();

	/* @deconstruction Ĭ����������
	* @author ���
	* @time 2017��2��27��
	*/
	~XMJGBuildSettingOut();

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void CreateBOSProject();

	/* @function �����к��ߵ�dwg���뵽��ǰͼ����
	* @author ���
	* @time 2017��3��
	*/
	void InsertRedLineDwg();

	/* @function ��ӷ�������
	* @author ���
	* @time 2017��3��
	*/
	void AddOutLineSymbol();

	/* @�ӿ� ���Ʒ��ݷ�Χ��
	 * @���� ���
	 * @ʱ�� 2017��07��04��
	 */
	void DrawBuildBoundaries();

	/* @function ��ʾ��������
	* @author ���
	* @time 2017��3��
	*/
	void ShowOutLineSymbol();

	/* @function ���ط�������
	* @author ���
	* @time 2017��3��
	*/
	void HideOutLineSymbol();

	/* @function ����ע������
	* @author ���
	* @time 2017��3��
	*/
	void SetAnnotationAttribute();

	/* @function �ı䷽λ˳��
	* @author ���
	* @time 2017��3��
	*/
	void ChangeAzimuthOrder();

	/* @function �ı����еķ�λ˳��
	* @author ���
	* @time 2017��3��
	*/
	void ChangeAzimuthOrderAll();

	/* @function ������Ϣ
	* @author ���
	* @time 2017��3��
	*/
	void SetAttatchedInfo();

	/* @function ��ʵ�������dwg
	* @author ���
	* @time 2017��3��
	*/
	void ExportToDWG();

	/* @function ��ʵ�������dwg
	* @author ���
	* @time 2017��3��
	*/
	void ExportToDWG2();

	/* @function ���doc�ĵ�
	* @author ���
	* @time 2017��3��
	*/
	void ExportToDOC();

	/* @function ��ȡע�����Ϣ��������Ϣ�滻������
	* @author ���
	* @param SBreakBlock ʵ���
	* @time 2017��3��13��
	*/
	void GetReplaceValue(MStr &);

	/* @function ��ȡע�����Ϣ������word����ı�ʶΪ�滻
	* @param word ���滻��word
	* @param char * �������滻����Ϣ
	* @author ���
	* @time 2017��3��
	*/
	void ReadRegisterValue(Word *);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��08��25��
	 */
	void splitMulLineText(MStr &, const CString &, const CString &);

	/* @�ӿ� ��������Χ�������ɵ������ӵ�ʵ����
	 * @���� ads_name ������Χ��
	 * @���� ���
	 * @ʱ�� 2017��07��04��
	 */
	void AddZDMJ2BuildEntity(ads_name);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��21��
	 */
	void DrawBuildNearestBoundaries();

	/* @�ӿ� �޸Ľ���������
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void ModifyBuildName();

	/* @�ӿ� �޸Ľ������
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void modifyBuildWidth();

private:

	double m_Scale;
	CString m_SaveDir;
	TextProperty m_jzwlk;
	TextProperty m_jzwmc;

};

/* @class ���ſ���������ҵ����
* @author ���
* @time 2017��3��
*/
class XMJGRoadSettingOut
{
public:

	/* @construction Ĭ�ϵĹ��캯��
	* @author ���
	* @time 2017��3��14��
	*/
	XMJGRoadSettingOut();

	/* @deconstruction Ĭ����������
	* @author ���
	* @time 2017��3��14��
	*/
	~XMJGRoadSettingOut();

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void CreateRSOProject();

	/* @function ���Ƶ�·����
	* @author ����
	* @time 2017��2��27��
	*/
	void DrawRoadEdgeLine();

	/* @function ���Ƶ�·������
	* @author ����
	* @time 2017��2��27��
	*/
	void DrawRoadCentreLine();

	/* @function ����������·�����ߣ�
	* @author ����
	* @time 2017��2��27��
	*/
	void ExportCoodinateTable();

	/* @function ��ע����յ�����
	* @author ����
	* @time 2017��2��27��
	*/
	void SetRoadText();

	/* @function ������Ϣ����
	* @author ����
	* @time 2017��2��27��
	*/
	void SetAttatchedInfo();

	/* @function DWG�ɹ����
	* @author ����
	* @time 2017��2��27��
	*/
	void ExportDWGOutput();

	/* @function WORD�ɹ����
	* @author ����
	* @time 2017��2��27��
	*/
	void ExportWORDOutput();

	/* @function ��ע�ʼ�
	* @author ����
	* @time 2017��2��27��
	*/
	void TextDataCheck();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��16��
	*/
	void SetRoadStartEndPoint();

private:

	/* @function ��ȡע�����Ϣ��������Ϣ�滻������
	* @author ���
	* @param SBreakBlock ʵ���
	* @time 2017��3��13��
	*/
	void ReadRegisterValue(SBreakBlock *);

	/* @function ��ȡע�����Ϣ��������Ϣ�滻������
	* @���� MStr
	* @author ���
	* @param SBreakBlock ʵ���
	* @time 2017��3��13��
	*/
	void GetReplaceValue(MStr &);

	/* @function ��ȡע�����Ϣ������word����ı�ʶΪ�滻
	* @param word ���滻��word
	* @param char * �������滻����Ϣ
	* @author ���
	* @time 2017��3��
	*/
	void ReadRegisterValue(Word *);

	/* @�ӿ�
	* @����
	* @����
	* @����
	* @����
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void GetTableRowAndCol(int &, int &, int &);

	/* @�ӿ� �жϵ��Ƿ��ڵ��������
	 * @���� AcGePoint2d ��Ҫ�жϵĵ�
	 * @���� AcGePoint2dArray �ڵ�����
	 * @���� ������ڽڵ������У�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��06��22��
	 */
	bool JudgePointInNodes(const AcGePoint2d &, const AcGePoint2dArray &);

	/* @�ӿ� ������������ʵ����
	 * @���� AcDbObjectId ʵ��ID
	 * @���� ads_point ����ӵĵ�
	 * @���� ���
	 * @ʱ�� 2017��06��22��
	 */
	void AddCoorInfoToEntity(AcDbObjectId &, const AcGePoint2d &, const AcGePoint2d &);

	/* @�ӿ� ��ȡҪע�ǵĵ�
	 * @���� AcDbObjectIdArray Ҫע�ǵ�ʵ�弯��
	 * @���� AcGePoint2dArray ���еĴ�ע�ǵĵ㼯
	 * @���� ���
	 * @ʱ�� 2017��08��24��
	 */
	void getAnnotationPoint(AcDbObjectIdArray &, AcGePoint2dArray &);

	/* @�ӿ� ��ȡ����ע�ǵ���Ϣ
	* @���� AcGePoint2dArray ���еĴ�ע�ǵĵ㼯
	* @���� VVStr �����˱��е���Ϣ
	* @���� int ��������
	* @���� int ����������һ����Ϊһ�У�
	* @���� ���
	* @ʱ�� 2017��08��24��
	*/
	void getRoadTableInfo(const AcGePoint2dArray &, VVStr &, int, int);

	/* @�ӿ� ��ȡ����ע�ǵ���Ϣ
	 * @���� AcGePoint2dArray ���еĴ�ע�ǵĵ㼯
	 * @���� VVStr �����˱��е���Ϣ
	 * @���� int ��������
	 * @���� int ����������һ����Ϊһ�У�
	 * @���� ���
	 * @ʱ�� 2017��08��24��
	 */
	void getRoadTableInfo(const AcDbObjectIdArray &, VVStr &, int, int);

	/* @�ӿ� ��ȡ����ע�ǵ���Ϣ,������Ҫ��ֻ����п��Ƶ������
	 * @ʱ�� 2018��4��13��
	 */
	void getRoadTableInfo(vector<pair<AcDbObjectId, CString>> &vKZD, VVStr &, int, int);

	/* @�ӿ� ��ӵ�·�����ߵ�ע��
	 * @���� AcGePoint2d ��ӵ�·�߽ڵ���
	 * @���� int ������
	 * @���� ���
	 * @ʱ�� 2017��08��24��
	 */
	void addAnnotationCode(const AcGePoint2d &, int);

	/* @�ӿ� ��ȡ��·�������ϵĿ��Ƶ�
	 * @ʱ�� 2018��4��13��
	 */
	BOOL GetControlPoint(const AcGePoint2dArray &nodes, vector<pair<AcDbObjectId, CString>> &KZD);
private:

	TextProperty m_ywcdtk;	//Ұ����ͼ��
	int m_bhcolor;			//��·���ע����ɫ
	int m_zjcolor;			//��·��βע����ɫ
	float m_bhfont;			//��·���ע�Ǵ�С
	float m_zjfont;			//��·��λ�ڵ��С
	double m_Scale;
	CString m_SaveDir;
	CString m_bhlayer;		//��·���ע��ͼ��
	CString m_zjlayer;		//��·��βע��ͼ��
	TextProperty m_dlzxx;	//��·������
	TextProperty m_dlbx;	//��·����
};

/* @class ���ſ������ĸ�ҵ����
* @author ���
* @time 2017��3��
*/
class  XMJGEvelingElevation
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	XMJGEvelingElevation();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	~XMJGEvelingElevation();

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void createProject();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void AxisAutoAnnotion1();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void SetAttatchedInfo();

	/* @�ӿ� ���ʵ�嵽dwg����ʽ1��
	* @���� ���
	* @ʱ�� 2017��06��19��
	*/
	void ExportResult2DWG1();

	/* @�ӿ� ���ʵ�嵽dwg����ʽ2��
	* @���� ���
	* @ʱ�� 2017��06��19��
	*/
	void ExportResult2DWG2();

private:

	/* @�ӿ� ��ȡע����ȡ���滻��ֵ
	* @���� MStr ���滻��ֵ
	* @���� ���
	* @ʱ�� 2017��06��19��
	*/
	void GetReplaceValue(MStr &);

	/* @�ӿ� ��ȡ���Ƶ�ĸ���
	* @���� ���Ƶ�ĸ���
	* @���� ���
	* @ʱ�� 2017��06��19��
	*/
	int GetControlPointCount(MStr &);

private:

	double m_Scale;
	CString m_SaveDir;

};

/* @class ���ſ��������ҵ����
* @author ���
* @time 2017��3��
*/
class XMJGCheckLine
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	XMJGCheckLine();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	~XMJGCheckLine();

public:

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void createProject();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��20��
	*/
	void DrawSurroundingBuild();

	/* @�ӿ� ע��Ԥ����
	 * @���� ���
	 * @ʱ�� 2017��06��22��
	 */
	void AnnPretreatment();

	void AxisAutoAnnotion1();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void AxisAutoAnnotion();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void AxisManualAnnotion();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void SetAttatchedInfo();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void AutoBuildingInsideAnnotion();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��20��
	*/
	void ExportResult2DOC();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��20��
	*/
	void ExportResult2DWG1();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��20��
	*/
	void ExportResult2DWG2();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��23��
	 */
	void drawBuildOutLine();

private:

	void splitMulLineText(MStr &, const CString &, const CString &);


	/* @�ӿ� ��ȡע����ȡ���滻��ֵ
	* @���� MStr ���滻��ֵ
	* @���� ���
	* @ʱ�� 2017��06��19��
	*/
	void GetReplaceValue(MStr &);

	/* @�ӿ� ���ݴ����ʵ����ƶ����
	 * @���� AcDbObjectId ʵ��ID
	 * @���� ���
	 * @ʱ�� 2017��06��22��
	 */
	void DrawLines(AcDbObjectId &);

	/* @�ӿ� ���Ʋο���
	 * @���� AcDbObjectId ��Ҫ���Ʋο��ߵ�ʵ��ID
	 * @���� ���
	 * @ʱ�� 2017��06��22��
	 */
	void DrawReferenceEdge(AcDbObjectId &, double offset = 3.2);


	void ReadRegisterValue(SmartTable::Word *);

	void ReadAnnotionDist();

	/* @�ӿ� ��ȡ��ע�ǵ�ʵ��ID�Լ�ע�Ǳߵ������յ�
	 * @���� ��ע�ǵ�ʵ��ID
	 * @���� ע�Ǳߵ����
	 * @���� ע�Ǳߵ��յ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue
	 * @���� ���
	 * @ʱ�� 2017��06��22��
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

/* @�� ���ſ���������ҵ����
 * @���� ����
 * @ʱ�� 2017��3��21
 */
class XMJGPedestrianBridge
{

public:

	/* @�ӿ� ���캯��
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	XMJGPedestrianBridge();

	/* deconstruction ���캯��
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	~XMJGPedestrianBridge();

	/* @�ӿ� �����ƽͼ
	 * @���� ���
	 * @ʱ�� 2018��1��3��
	 */
	void bridgeZPT();

	/* @�ӿ� ����������ŵ�dwg
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	void ExportDWGPB();

	/* @�ӿ� �������
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void bridgeHatch();

	/* @�ӿ� �������
	 * @���� ���
	 * @ʱ�� 2017��12��25��
	 */
	void birdgeHatch();

	/* @�ӿ� ���ƽ���ռ������
	 * @���� ����
	 * @ʱ�� 2017��4��6��
	 */
	void DrawRedLine();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void createProject();

	/* @�ӿ� ���ű�ע
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void bridgeAnnoate();

	/* @�ӿ� �����ŵĲ���
	 * @���� ���
	 * @ʱ�� 2017��12��25��
	 */
	void setBirdgeLayer();

	/* @�ӿ� �����������
	 * @���� ���
	 * @ʱ�� 2018��2��7��
	 */
	void exportPBReport();

	/* @�ӿ� �������ž���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	void setPBClearWidth();

	/* @�ӿ� �������Ÿ߶���Ϣ
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void bridgeSetHeight();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��9��29��
	 */
	void modifyPhoteSize();

	/* @�ӿ� ������Ϣ
	 * @���� ���
	 * @�ӿ� 2017��4��18��
	 */
	void SetAttatchedInfo();

	/* @�ӿ� ����滮������ʵ�����
	 * @���� ���
	 * @�ӿ� 2017��4��6��
	 */
	void ExportApplyTable();

	/* @�ӿ� ��������������
	 * @���� ����
	 * @ʱ�� 2017��3��10��
	 */
	void DrawPhotoPositon();

	/* @�ӿ� ���ƿ��Ƶ�
	 * @���� ����
	 * @ʱ�� 2017��3��10��
	 */
	void DrawControlPoint();

	/* @�ӿ� ���þ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void setPBClearHeight();

	/* @�ӿ� ��ע�����ĳ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	void remarkLengthWidth();

	/* @�ӿ� ͳ�������Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	void statisticAreaInfo();

	/* @�ӿ� ������Ҫ����Ĺ���
	 * @���� ���
	 * @ʱ�� 2018��2��8��
	 */
	void setHandlePipeLine();

	/* @�ӿ� �������ͼ
	 * @���� ���
	 * @ʱ�� 2018��1��5��
	 */
	void exportVerticalView();

	/* @�ӿ� ·����ע��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��28��
	 */
	void remarkLoadLevation();

	/* @�ӿ� ������ע��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��28��
	 */
	void remarkBridgeLevation();

	/* @�ӿ� �޸�ͼƬ��Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��1��
	 */
	void changePicturePropery();

	/* @�ӿ� ��ע�߽���
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	void AnnotationBoundaries();

	/* @�ӿ� ���������������ͼ��dwg
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	void ExportDWGPBElevation();

	/* @�ӿ� ��ע������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	void remarkBridgeThickness();

	/* @�ӿ� ��ע���˸߶�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	void remarkBridgeBalustrade();

	/* @�ӿ� ����ͨ������λ��
	 * @���� ���
	 * @ʱ�� 2018��2��26��
	 */
	void setChannelBodyPosition();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��09��11��
	 */
	void exportPedestrianBridge();

	/* @�ӿ� ����Ȩ���߽�
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	void DrawOwnershipBoundaries();

	/* @�ӿ� ����������ŷֲ�ƽ��ͼ��dwg
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	void ExportDWGPBStratificationPlan();

	/* @�ӿ� ���Ƶ�ֲ��Լ���·�����Ƭ����λ��ͼ
	 * @���� ���
	 * @ʱ�� 2017��4��18��
	 */
	void ControlPointAndRoadPicturePosition();

	/* @�ӿ� �������������߶��Լ������߻��ܱ�
	 * @���� ���
	 * @ʱ�� 2017��4��6��
	 */
	void ExportBuildAreaHeightAndBirdgeHeight();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��3��
	 */
	void compressFile();

private:

	/* @�ӿ� �����ĵ�
	 * @���� Word �ĵ�����
	 * @���� ���
	 * @ʱ�� 2018��2��7��
	 */
	void saveDoc(Word &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��22��
	 */
	void addPlotRectangle();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��28��
	 */
	void getReplaceField(MStr &);

	/* @�ӿ� ��ȡȨ����Χ�ߵ����
	 * @���� double Ȩ���߽緶Χ�ߵ����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	double getScopeOfOwnership();

	/* @�ӿ� ���ĵ�ģ��
	 * @���� Word �ĵ�����
	 * @���� ���
	 * @ʱ�� 2018��2��7��
	 */
	bool openDocTemplate(Word &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��09��18��
	 */
	void replaceWordField(Word &);

	/* @�ӿ� ��ȡ�õط�Χ�ߵ����
	 * @���� double �õغ������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��26��
	 */
	double getBoundaryLineOfLand();

	/* @�ӿ� �滻��Ƭ��Ϣ
	 * @���� Word �ĵ�����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��1��
	 */
	void replacePictureInfo(Word &);

	/* @�ӿ� �滻��Ŀ������Ϣ
	 * @���� Word �ĵ�����
	 * @���� ���
	 * @ʱ�� 2018��2��7��
	 */
	void replaceProjectInfomation(Word &);

	/* @�ӿ� �滻�滮����˵����Ϣ
	 * @���� Word �ĵ�����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��7��
	 */
	void replacePlanningTechnicalInformation(Word &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��27��
	 */
	void replaceBuildAreaAndHeightSummarySheet(Word &);

	/* @�ӿ� �滻�滮������ʵ�������
	 * @���� Word �ĵ�����
	 * @���� ���
	 * @ʱ�� 2018��2��26��
	 */
	void replacePlanningConditionVerificationTable(Word &);

private:

	double m_Scale;
	CString m_SaveDir;
	TextProperty m_qbjx;			//�ű߽�������
	TextProperty m_tqtc;			//�����������
	TextProperty m_tqbg;			//���ű������

};

#pragma region ���ſ���ҵ������·���й���

class XMJGRoadAndPipeLine
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	XMJGRoadAndPipeLine();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	~XMJGRoadAndPipeLine();

public:

	/* @function ������Ŀ��Χ��
	* @author ���
	* @time 2017��2��24��
	*/
	void DrawProjectScopeOfLine();

	/* @function ���ƿ��Ƶ�
	* @author ���
	* @time 2017��2��24��
	*/
	void DrawControlPoint();

	/* @function ������Ƭ����
	* @author ���
	* @time 2017��2��24��
	*/
	void DrawPhotoPositon();

	/* @function ����ɹ�ͼ��dwg
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportResults2Dwg();

	/* @function �����ƽͼ
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportTotalFlatFigure();

	/* @function �ݶ���ͼ
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportLongitudinalSectionFigure();

	/* @function ��·��������ͼ
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportRoadCrossSectionalComparisonChart();

	/* @function ��·�����ܽ�
	* @author ���
	* @time 2017��2��24��
	*/
	void RoadTechnologySummary();

	/* @function ������ƽͼ
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportPipeLineTotalFlatFigure();

	/* @function ������߳ɹ���
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportPipeLineResultTable();

	/* @function ���������ʵ��ɹ����ձ�
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportPipeLineDesignAndMeasuredComparisonTable();

	/* @function ���߶���ͼ
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportPipeComparisonFigure();

	/* @function ���߶���ͼ
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportPipeLinecomprehensiveTotalFlatFigure();

	/* @function ���ߺ���沼��ͼ
	* @author ���
	* @time 2017��2��24��
	*/
	void ExportPipeLineCrossSectionalDwg();

	/* @function ���߼����ܽ�
	* @author ���
	* @time 2017��2��24��
	*/
	void PipelineTechnicalSummary();

	/* @function ���������
	* @author ���
	* @time 2017��2��24��
	*/
	void MeasuredResultsTable();

	/* @function ��������
	* @author ���
	* @time 2017��2��24��
	*/
	void TopographicBase();

private:

	CString m_SaveDir;

};

#pragma endregion

#pragma region ҵ������·��������

class XMJGRoadNoPipeLine
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��05��23��
	*/
	XMJGRoadNoPipeLine();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��05��23��
	*/
	~XMJGRoadNoPipeLine();

public:

	/* @�ӿ� ���ƿ��Ƶ��Լ���·��Ƭ
	* @���� ���
	* @ʱ�� 2017��05��23��
	*/
	void ExportControlPointAndRoadPicture();

	/* @�ӿ� ���Ƶ�·��ƽͼ
	* @���� ���
	* @ʱ�� 2017��05��23��
	*/
	void ExportRoadStratificationPlan();

	/* @�ӿ� �����·�ݶ���
	* @���� ���
	* @ʱ�� 2017��05��23��
	*/
	void ExportRoadProfile();

	/* @�ӿ� �����·�����
	* @���� ���
	* @ʱ�� 2017��05��23��
	*/
	void ExportRoadCrossSectional();

	/* @�ӿ� �����·��������ͼ
	* @���� ���
	* @ʱ�� 2017��05��23��
	*/
	void ExportRoadComprehensiveCrossSectional();

	/* @�ӿ� ����ɹ����ձ��Լ������ܽ�
	* @���� ���
	* @ʱ�� 2017��05��23��
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

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	XMJGFinalAcceptance();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	~XMJGFinalAcceptance();

public:

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void FillBuildingArea();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void SetAttatchedInfo();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	void OwnershipBoundary();
};

class XMJGQualityInspection
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	XMJGQualityInspection();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	~XMJGQualityInspection();

};

struct XMJGHAH
{
	CString m_ldh;		//¥����
	CString m_lch;		//���
	double m_cg;		//���
	double m_jzmj;		//�������
	double m_jrmj;		//�������
	double m_jzzmj;		//���������
	double m_jrzmj;		//���������
	double m_jrmjxj;	//�������С��
	double m_jzmjxj;	//�������С��
	double m_jzzdmj;	//����ռ�����
	XMJGHAH()
		: m_cg(0.0), m_jzmjxj(0.0),
		m_lch(_T("")), m_jrzmj(0.0),
		m_ldh(_T("")), m_jzmj(0.0),
		m_jrmj(0.0), m_jzzdmj(0.0),
		m_jrmjxj(0.0), m_jzzmj(0.0)
	{

	}

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��27��
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

/* @�� ���� */
class XMJGHouse
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	XMJGHouse();

	/* @�ӿ� Ĭ����������
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	~XMJGHouse();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void createProject();

	/* @function ���ù���Ŀ¼
	* @author ���
	* @time 2017��5��12��
	*/
	void setProjectContext();

	/* @function ���Ƴ�λ
	* @author ���
	* @time 2017��5��3��
	*/
	void DrawCarAddress();

	/* @function �ع���λ
	* @author ���
	* @time 2017��5��9��
	*/
	void DimensionsForCar();

	/* @function ���ó�λ��ע
	* @author ���
	* @time 2017��5��3��
	*/
	void SetCarAnnotation();

	/* @function ���Ʒֲ�ƽ��ͼ
	* @author ���
	* @time 2017��5��3��
	*/
	void DrawStratificationPlan();

	/* @function ���÷ֲ�ƽ����Ϣ
	* @author ���
	* @time 2017��5��3��
	*/
	void SetStratificationPlanInfo();

	/* @function ����������ʵ��
	* @author ���
	* @time 2017��5��3��
	*/
	void DimensionsForChangeArea();

	/* @function ���ƽ����������
	* @author ���
	* @time 2017��5��3��
	*/
	void DrawBuildChangeArea();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��9��25��
	 */
	void ExportChangeAreaDWG();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��17��
	 */
	void ExportGuiHuaBuYiZhiDWG();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��17��
	 */
	void newCheWeiStatistic();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��9��25��
	 */
	void ExportBuildHeightAndAreaInfo();

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��06��26��
	*/
	void ExportBuildHeightAndAreaDoc();

	/* @function ��ӽ������ı�ע
	* @author ���
	* @time 2017��5��3��
	*/
	void AddBuildChangeAnnotion();

	/* @function ���ƹ�����
	* @author ���
	* @time 2017��5��3��
	*/
	void DrawFunctionalAreas();

	/* @function ����������
	* @author ���
	* @time 2017��5��3��
	*/
	void DimensionsFunctionalAreas();

	/* @function ���ù���������Ϣ
	* @author ���
	* @time 2017��5��8��
	*/
	void SetFunctionsAreasInfo();

	/* @�ӿ� ͳ�ƹ����������
	 * @���� ���
	 * @ʱ�� 2017��07��05��
	 */
	void StatisticFunctionArea();

	/* @function �߶�ʾ��ͼ
	* @author ���
	* @time 2017��5��4��
	*/
	void ExportHighlySchematic();

	/* @function ��ʵ�������dwg�ļ�
	* @author ���
	* @time 2017��5��3��
	*/
	void ExportEntitys2File();

	/* @function ����ֲ�ƽ��ͼ
	* @author ���
	* @time 2017��5��3��
	*/
	void ExportStratificationPlan();

	/* @�ӿ� ���������Ƭ��Ϣ
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	void SetExportHouseDocInfo();

	/* @�ӿ� ����ܵĿ���������
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void ExportHouseDoc();

	/* @�ӿ� ���ƽ����Ϣ��mdb��
	 * @���� ���
	 * @ʱ�� 2017��07��03��
	 */
	void ExportPlanInfo2MDB();

	/* @�ӿ� ������Ϣ
	 * @���� ���
	 * @ʱ�� 2017��07��13��
	 */
	void BuildSetAttatchedInfo();

	/* @�ӿ� ���幦����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��23��
	 */
	void FunctionDefine();

	/* @�ӿ� ���������
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void buildHatch();

	/* @�ӿ� ���㽨��ռ�����
	 * @���� ������
	 * @ʱ�� 2019��9��24��
	 */
	void calculateJZZDMJ();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void drawRotateLine();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void changeLayerInfo();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	void drawAreaRange();

	/* @�ӿ�
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��10��9��
	 */
	bool comprassFile();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��10��15��
	 */
	void picturePosition();

	/* @�ӿ� �ֲ�ƽ��ͼ���ָ����
	 * @���� ���
	 * @ʱ�� 2017��10��21��
	 */
	void addCompass();

	/* @�ӿ� ��ͨ��תΪ�������
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	void lineConvertChangeArea();

	/* @�ӿ� ��ӷֲ��ע��Ϣ
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	void AddFunctionAnnotion();

	/* @�ӿ� ��������߶�
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	void setMainHeight();

	/* @�ӿ� ��Ӽв���Ϣ
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	void setMezzanineInfo();

	/* @�ӿ� ���ó�λ����
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	void setCarProperty();

	/* @�ӿ� ������Ŀ��Ϣ
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	void settingProjectInfo();

	/* @�ӿ� ��ƽͼ���
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void addZPTElevation();

	/* @�ӿ� ����ע��
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	void singleEdgeNote();

	/* @�ӿ� ���ò���Ϣ
	 * @���� ���
	 * @ʱ�� 2017��11��17��
	 */
	void setLayerInfoDlg();

	/* @�ӿ� �����Ŀ��Χ��
	 * @���� ���
	 * @ʱ�� 2017��11��17��
	 */
	void exportXMFWX();

	/* @�ӿ� ���ò���Ϣ
	 * @���� ���
	 * @ʱ�� 2017��11��20��
	 */
	void setLayerInfo();

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��28��
	 */
	void addChangeNote();

	/* @function ���ù���������Ϣ
	 * @param AcDbObjectId ��Ҫ������Ϣʵ��ID
	 * @author ���
	 * @time 2017��5��8��
	 */
	FunctionNameTableEx setFunctionalAreaInfo(const AcDbObjectId &);

	/* @�ӿ� ���ñ��������Ϣ
	 * @���� AcDbObjectId �������ʵ��id
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	bool setChangeAreasInfo(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void exportHouseZPT();

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��10��15��
	 */
	void addFunctionAreaNote(const AcDbObjectId &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��18��
	 */
	void addFunctionProperty(const AcDbObjectId &, const FunctionNameTableEx &);

	/* @�ӿ� �����һ�±�
	* @ʱ�� 2017��10��15��
	*/
	void CompareExport();

	/* @�ӿ� ���ͳ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��30��
	 */
	void areaStatistic();

	/* @�ӿ� ������ǽ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��30��
	 */
	void innerWallLine();

	/* @�ӿ� ���ǽ���ͳ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��30��
	 */
	void statisticHalfWall();

	/* @�ӿ� �޸Ŀ�������������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��31��
	 */
	void modifyJGBuildName();

	/* @�ӿ� ����Ѿ�����Ĺ�����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��2��1��
	 */
	void checkFunctionDefine();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��8��
	 */
	void drawLayerFirst();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��7��10��
	 */
	void boundaryNote();

	/*�����·��ƽͼ
	*/
	void exportRoadZPT();

	/*�����·���Ƶ�ֲ��������������Ƭ����λ��ͼ
	*/
	void PicAndKZD();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��7��27��
	 */
	void splitLayer();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��16��
	 */
	void tiQingMianJi();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��17��
	 */
	void distAnnotate();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��20��
	 */
	void checkGongNengQu();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��20��
	 */
	void defineGongNengQu();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��12��
	 */
	void checkBuildLayerAndMDB();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��15��
	 */
	void newSetLayerInfo();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��16��
	 */
	void newQualityCheck();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��15��
	 */
	void newCreateFCPMT();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��26��
	 */
	void newSplitMargeLayer();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��27��
	 */
	void newDrawUpPlanning();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��14��
	 */
	void newGHSTYSGSTBYZ();

private:

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��1��
	 */
	void addStoreyInfo(StoreyInfo &, const CString &, MStr &);

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��20��
	 */
	bool getIsOrNotJR();

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��27��
	 */
	void copyFCPMT(const SplitMargeLayer &, bool);

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��27��
	 */
	void moveFCPMT(const SplitMargeLayer &, bool);

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��27��
	 */
	void FCPMTNameOper(const SplitMargeLayer &, bool);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪzfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool replaceWordField(Word &);
	
	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void getReplaceValue(MStr &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void getReplaceValue(MStr &, const CString &);

	/* @function ��ȡ��ǰ��λ���ı��
	* @author ���
	* @time 2017��5��3��
	*/
	int GetCarSerialNum();

	/* @function ѡ����Ҫ�����ʵ��
	* @author ���
	* @time 2017��5��3��
	*/
	bool selectExportObject();

	/* @function ��ȡ�����ȫ��ʵ��
	* @param AcDbObjectId ͼ���ID
	* @author ���
	* @time 2017��5��3��
	*/
	void getExportObjects(AcDbObjectId &, AcDbObjectIdArray &);

	/* @function ��ȡ¥����Լ���Ӧ��¥��߶�
	* @param AcDbObjectId �ֲ�ƽ��ͼ��ʵ��ID
	* @param MIDouble ¥����Լ��߶�
	* @author ���
	* @time 2017��5��4��
	*/
	void getFloorHeight(AcDbObjectId &, MIStoreyInfo &);

	/* @function ��ȡ¥����Լ���Ӧ��¥��߶�
	 * @param AcDbObjectId �ֲ�ƽ��ͼ��ʵ��ID
	 * @param MIDouble ¥����Լ��߶�
	 * @author ���
	 * @time 2017��5��4��
	 */
	void getFloorHeight(AcDbObjectId &, MIStoreyInfo &, const double &);

	/* @function ��ȡ¥����Լ���Ӧ��¥��߶�
	 * @param AcDbObjectId �ֲ�ƽ��ͼ��ʵ��ID
	 * @param MIDouble ¥����Լ��߶�
	 * @author ���
	 * @time 2017��5��4��
	 */
	void getFloorHeight(AcDbObjectId &, MDStoreyInfo &, const double &);

	/* @function ���Ƹ߶�ʾ��ͼ
	* @param MIStoreyInfo ����Լ�¥��߶�
	* @param AcDbObjectIdArray ���Ƹ߶�ʾ��ͼ������ʵ��ID
	* @return bool ����ʾ��ͼ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @author ���
	* @time 2017��5��4��
	*/
	bool DrawHightlySchematic(const MIStoreyInfo &, AcDbObjectIdArray &, double, const TCHAR *);

	/* @�ӿ� ���Ƽв�
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	void drawMezzanineInfo(const StoreyInfo &, double, AcDbObjectIdArray &);

	/* @function ���ø߶�ʾ��ͼ��·��
	* @author ���
	* @time 2017��5��8��
	*/
	void setHighlySchematicPath();

	/* @function ����¥�� 
	* @param int ���һ������ʱΪ�˻��Ƶ��¸߶�ʾ��ͼʱ�����ƴ����Ŀ���
	* @param double ��Ҫ���Ƶ�¥��߶ȣ��˸߶��������ĸ߶ȣ�
	* @param StoreyInfo ¥����Ϣ
	* @param AcDbObjectIdArray ����Ҫ��ɾ����ʵ�弯��
	* @author ���
	* @time 2017��5��8��
	*/
	void drawOneLayer(double, const CString &, const StoreyInfo &, AcDbObjectIdArray &);

	/* @�ӿ� ���Ƶ��²�
	 * @���� double ���²����ʼ�߶�
	 * @���� CMIStoreyInfoIter ¥����Ϣ
	 * @���� AcDbObjectIdArray ����Ҫɾ����ʵ��id����
	 * @���� double�����ص��²�ĸ߶�
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	void drawUnderground(double &, CMIStoryInfoIter &, AcDbObjectIdArray &);

	/* @�ӿ� ���ư���²�
	 * @���� double ����²���ʼ�߶�
	 * @���� CMIStoreyInfoIter ¥����Ϣ
	 * @���� AcDbObjectIdArray ����Ҫɾ����ʵ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��22��
	 */
	void drawSubsurface(double &, CMIStoryInfoIter &, AcDbObjectIdArray &);

	/* @�ӿ� ���Ƹ���
	 * @���� double ��������ʼ��
	 * @���� double �����ĸ߶�
	 * @���� double ���ݵĿ��
	 * @���� AcDbObjectIdArray ����Ҫɾ����ʵ��id����
	 * @���� ���
	 * @ʱ�� 2017��12��3��
	 */
	void drawUnderFuTu(double &, double, double, AcDbObjectIdArray &);

	/* @function ������Ů��ǽ��¥��
	* @param double ��Ҫ���Ƶ�¥��߶ȣ��˸߶��������ĸ߶ȣ�
	* @param StoreyInfo ¥����Ϣ
	* @param AcDbObjectIdArray ����Ҫ��ɾ����ʵ�弯��
	* @author ���
	* @time 2017��5��8��
	*/
	void drawParapetWall(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	void drawHeightText(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @function ����û��Ů��ǽ��¥��
	* @param double ��Ҫ���Ƶ�¥��߶ȣ��˸߶��������ĸ߶ȣ�
	* @param StoreyInfo ¥����Ϣ
	* @param AcDbObjectIdArray ����Ҫ��ɾ����ʵ�弯��
	* @author ���
	* @time 2017��5��8��
	*/
	void drawNoParapetWall(double, const CString &, const StoreyInfo &, AcDbObjectIdArray &);

	/* @�ӿ� ���������ʵ��
	 * @���� double �߶�
	 * @���� StoreyInfo ¥����Ϣ
	 * @���� AcDbObjectIdArray ����Ҫ��ɾ����ʵ�弯��
	 * @���� ���
	 * @ʱ�� 2017��11��21��
	 */
	void drawWuMianEntity(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��21��
	 */
	void drawNvErQiangEntity(double, const StoreyInfo &, AcDbObjectIdArray &);

	/* @function ���ù���������Ϣ
	* @param AcDbObjectId ��Ҫ������Ϣʵ��ID
	* @author ���
	* @time 2017��5��8��
	*/
	bool setFunctionalAreaInfo(AcDbObjectId &, FunctionNameTableEx &);

	/* @�ӿ� ��ȡ����·��
	* @���� TCHAR �����ļ���·��
	* @���� TCHAR �����ļ�������
	* @���� TCHAR ����·���Լ��ļ���
	* @���� ���
	* @ʱ�� 2017��06��12��
	*/
	CString getSavePath(const TCHAR *, const TCHAR *);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��26��
	 */
	void getHouseAreaAndHeight(AcDbObjectId &, XMJGHAH &);

	/* @�ӿ� ��ʵ���ж�ȡ����Ϣ
	 * @���� AcDbObjectId ʵ�����
	 * @���� XMJGHAH ����Ϣ
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void getPerLevelArea(AcDbObjectId &, XMJGHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void getSameLevelArea(XMJGHAH &, MSHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void getBuildingArea(AcDbObjectId &, MSHAH &);

	/* @�ӿ� ��ȡ¥���
	 * @����
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��26��
	 */
	int getHouseLCH(const TCHAR *, int &, int &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��09��14��
	 */
	void getPlanInfo(AcDbObjectId &, MStr &, bool);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void readArea2IniFile(MSBuild &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void updataAreaIniFile(MSBuild &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	void writeArea2IniFile(const BuildHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void writeArea2IniFile(const MSBuild &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void exportDoc(SmartTable::Word &, const BuildHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void exportDoc(SmartTable::Word &, const MSHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	void replaceWordArea(SmartTable::Word &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��25��
	 */
	void replaceWordHeight(SmartTable::Word &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��06��29��
	 */
	void setProjectInfo(SmartTable::Word &);

	/* @�ӿ� �������֤�ĵ�
	 * @���� SmartTable::Word ���doc
	 * @���� ���
	 * @ʱ�� 2017��06��28��
	 */
	void setLicenseDoc(SmartTable::Word &);

	/* @�ӿ� ��word������뷿����Ƭ
	 * @���� SmartTable::Word �����doc
	 * @���� ���
	 * @ʱ�� 2017��06��27��
	 */
	void insertHousePicture(SmartTable::Word &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��07��05��
	 */
	void readHousePictureInfo();

	/* @�ӿ� ��ȡ����������Ϣ
	 * @���� ���
	 * @ʱ�� 2017��07��05��
	 */
	void readFunctionInfo(const AcDbObjectIdArray &, AcDbObjectIdArray &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��07��05��
	 */
	void drawFunctionFrame(AcDbObjectId &, AcDbObjectIdArray &);

	/* @�ӿ�
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	bool getLayerArea(const AcDbObjectId &, LayerHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��26��
	 */
	void layerArea(const AcDbObjectId &, double &, double &, double &);

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��12��25��
	 */
	void getWaiBanQiangArea(const AcDbObjectId &, double &, double &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	bool getFunctionArea(AcDbObjectId &, FunctionHAH &);

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��8��5��
	 */
	void removeSubFunction(const AcDbObjectId &, double &, double &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	bool addFuncion2Layer(const FunctionHAH &, LayerHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool addBuild2Layer(const LayerHAH &, BuildHAH &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool createStratificationPlan(const CString &, double);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��26��
	 */
	bool combineStratificationPlan(const CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	int createChangeAreaDWG(const CString &, double, const CString &, const CString &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��27��
	 */
	bool combineChangeAreaDWG(int, const CString &);

	/* @�ӿ� ��ע��������Ϣ
	 * @���� AcDbObjectId �ֲ�ƽ��ID��
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	bool addFunctionAnnotion(const AcDbObjectId &, CString &);

	/* @�ӿ� ��ע��������Ϣ
	 * @���� AcDbObjectId �ֲ�ƽ��ID��
	 * @���� ������
	 * @ʱ�� 2019��9��25��
	 */
	bool addCWAnnotion(const AcDbObjectId &, CString &);

	/* @�ӿ� ���ݷֲ㷶Χ�߻�ȡ�������еĹ�����
	 * @���� AcDbObjectId �ֲ�ƽ��ʵ��id
	 * @���� AcDbObjectIdArray ������ʵ��id����
	 * @���� ���
	 * @ʱ�� 2017��10��25��
	 */
	AcDbObjectIdArray getFunctionAreaIds(const AcDbObjectId &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��26��
	 */
	CString statisticLayerInfo(const VMStr &);

	/* @�ӿ� ���ó�λ����
	 * @���� AcDbObjectId ��λʵ���id
	 * @���� ���
	 * @ʱ�� 2017��10��26��
	 */
	void setCarProperty(const AcDbObjectIdArray &);

	/* @�ӿ� �滮ʵʩ���
	 * @���� ���
	 * @ʱ�� 2017��11��2��
	 */
	void DesignImplementation(SmartTable::Word &);

	/* @�ӿ� �������滻
	 * @���� ���
	 * @ʱ�� 2017��11��4��
	 */
	void AreaReplaceInfo(SmartTable::Word &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��6��
	 */
	void ExportDocInfo(SmartTable::Word &);

	/* @�ӿ�
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	void setJBXX(SmartTable::Word &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	void setJSSM(SmartTable::Word &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	void setMJXX(SmartTable::Word &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��25��
	 */
	void setBGXX(SmartTable::Word &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��26��
	 */
	CString statisticLayerInfo(const MStr &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��11��28��
	 */
	void writeArea2ProjectMDB(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� ��ײ�ĸ߶ȣ������������ĸ߶ȣ�
	 * @���� ���ݿ��
	 * @���� double &�����ƺ
	 * @���� ���
	 * @ʱ�� 2017��12��4��
	 */
	void drawDiPingXian(double &, double, double &, AcDbObjectIdArray &, const TCHAR *);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @ʱ�� 2017��12��28��
	 */
	void filterObject(VStr &, const CString &);

	/*��·��ƽͼ����
	*/
	void RoadfilterObj(VStr &);

	/*������ƽͼ����
	*/
	void PipefilterObj(VStr &, const CString&);

	/* @�ӿ�
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��1��
	 */
	AcDbObjectIdArray drawLeader(const ads_point spt, const ads_point ept, const CString &info);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��18��
	 */
	BuildLHeightAreaInfo initAreaHeightInfo();

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��18��
	 */
	void initFunctionAreaHeightInfo(const CString&, const CString &, HeightFunction &);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��11��27��
	 */
	void DrawUpPlanning(SmartTable::Word &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��2��
	 */
	void replaceWordTitle(SmartTable::Word &);

	// �Ƴ��۵�������
	void RemoveIslandFunctionArea(AcDbObjectIdArray &idArr);

	// �ع�ע�Ǵ�С
	void RegenFontSize(int scale, AcDbObjectIdArray idArr);
private:

	//ͼƬ��Ϣ
	struct HousePicture
	{
		int m_order;	//ͼƬ˳��
		CString m_name;	//ͼƬ����
		CString m_desc;	//������Ϣ
		CString m_pos;	//ͼƬλ��
	};
	typedef std::map<int, HousePicture> MIHPic;
	typedef MIHPic::iterator MIHPIter;

	int m_isNotNormalFloor;
	MIHPic m_PicInfo;	//ͼƬ��Ϣ
	CString m_SaveDir;				//����Ŀ¼
	double m_Scale;
	CString m_FloorName;			//��������
	CString m_savePath;				//����·��
	CString m_ProjectContext;		//����Ŀ¼
	MStr m_fieldTitle;
	MSInt m_isReplace;
	bool m_HighlySchematicFlag;		//���Ƹ߶�ʾ��ͼ��ʱ��ʹ��
	AcDbObjectIdArray m_object;
	TextProperty m_gnq;				//������
	TextProperty m_bg;
	TextProperty m_bgzj;			//�����ע
	TextProperty m_bgxxbz;			//�����Ϣ��ע
	TextProperty m_fcmjzs;			//�ֲ����ע��
	TextProperty m_fcpm;			//�ֲ�ƽ��
	TextProperty m_cw;				//��λ
	TextProperty m_cwzj;			//��λע��
	TextProperty m_fwbczj;			//���ݱ߳�ע��
	TextProperty m_mjzj;			//�ֲ�ƽ�����ע��
	TextProperty m_jzwlk;			//����������
	TextProperty m_lmtcmc;			//����ͼ������
	TextProperty m_lmtgdzj;			//����ͼ�߶�ע��
	TextProperty m_swdpbg;			//�����ƺ���
	TextProperty m_sndpbg;			//���ڵ�����
	TextProperty m_jzbg;			//�������
	TextProperty m_hxjd;			//�����������߽ڵ�
	double m_height;				//ע��Ҫ�õ�
	TextProperty m_dgzzbz;			//�������������ע
	TextProperty m_hxzbbz;			//�������������ע
	TextProperty m_dxfwx;			//���·�Χ��

};

void XMJGTest();

void OpenDwgCmd();

/* @macro ����ʵ��
* @author ���
* @time 2017��2��27��
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