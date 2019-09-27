//
// StdArx.h : include file for ObjectARX/DBX include files
// this file is only included once by your stdafx.h

#if !defined(AFX_STDARX_H__63491ABA_6EB3_40C3_A1A8_A4045FD3380E__INCLUDED_)
#define AFX_STDARX_H__63491ABA_6EB3_40C3_A1A8_A4045FD3380E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//{{AFX_ARX_INC_SELECTED
//}}AFX_ARX_INC_SELECTED

//{{AFX_ARX_INC_OTHERS
//}}AFX_ARX_INC_OTHERS
#include "AdskDMgr.h"         // Utility class for document data
#include "docdata.h"          // Your document specific data
#include "resource.h"
#include "Global.h"

// Declare it as an extern here so that it
// becomes available in all modules
//extern AsdkDataManager<CDocData> DocVars;
//{{AFX_ARX_FUNC
void FCSCheckMustFillFields();
void FCSCheckResultWizard();
void FCSPutP_FCS();
void FCSDZPB_FCS();
void FCSCheckUnCalulatedBuildings();
void FCSRefreshFrameData();
void FCSCheckArea();
void FCSExtractText();
void FCSAddAttachmentNote();
void FCSExtractToOrignDoc();
void FCSScaleDimensionText();
void FCSDrawThings();
void FCSNoteCoord_Total_Floor_Plan();
void FCSFCSTest();
void FCSFCSRegister();
void FCSAddHourseNote();
void FCSDrawdeepbuilding();

void FCSInsertBaseMap();
void FCSNoteIndoorsHeight();
void FCSInsertFrame_Total_Floor_Plan();
void FCSProjectInfoDlg();
void FCSStaticFloorInfo();
void FCSInsertFrame_FloorPlane();
void FCSInsertFrame_FloorPlane_Multy();
void FCSExportSurveyResultReport();
void FCSExportSummaryTable();
void FCSExtractFeatureTable();
void FCSAddFenceNote();
void FCSAddTextNote();
void FCSAddTextNote2();
void FCSInertHouseHoldFrame();
void FCSExportGroundAttachmentsTable_Standard2();
void FCSExportGroundAttachmentsTable();
void FCSOwnerInfoTable();
void FCSOwnerInfoSummaryTable();
void FCSOwnerInfoSummaryTable_Standard2();
void FCSExtentBuilding();
void FCSCheckElevationPoint();

//厦门竣工业务一
void FCSSetRLProjectCode();
void FCSImportData();
void FCSRLSetPropery();
void FCSReadExcel();
void FCSManualDrawPoint();
void FCSDrawLine();
void FCSAddRedLineNode();
void FCSDelRedLineNode();
void FCSAdjustNode();
void FCSAddAnnotation();
void FCSManualAnnotation();
void FCSAutoAnnotation();
void FCSMoveAnnotation();
void FCSManualLineAnnotation();
void FCSRedlineSetAttatchedInfo();
void FCSRedLineExportDWG();
void FCSReadReveseDat();
void FCSRLComprassFile();
void FCSInputCoorPos();
void FCSRedLineAddNode();
void FCSRedDistAnnotion();

//厦门竣工业务二
void FCSSetJZProjectCode();
void FCSInsertRedLineFile();
void FCSDrawBuildOutLine();
void FCSDrawBuildBoundaries();
void FCSShowOutLineSymbol();
void FCSHideOutLineSymbol();
void FCSSetAnnotationAttribute();
void FCSChangeAzimuthOrder();
void FCSChangeAzimuthOrderAll();
void FCSBOLSetAttatchedInfo();
void FCSExportToDWG1();
void FCSExportToDWG2();
void FCSExportToDOC();
void FCSJZComprassFile();
void FCSDrawBuildNearestBoundaries();
void FCSModifyBuildName();

//厦门竣工业务三
void FCSSetDLProjectCode();
void FCSSetRoadText();
void FCSTextDataCheck();
void FCSDrawScopeLine();
void FCSExportDWGOutput();
void FCSDrawRoadEdgeLine();
void FCSSetAttatchedInfo();
void FCSExportWORDOutput();
void FCSDrawRoadCentreLine();
void FCSRSOSetAttatchedInfo();
void FCSExportCoodinateTable();
void FCSSetRoadStartEndPoint();
void FCSDLComprassFile();

//厦门竣工业务四
void FCSSetKZDProjectCode();
void FCSDrawControlPoint();
void FCSEESetAttatchedInfo();
void FCSExportResultToDWG1();
void FCSExportResultToDWG2();
void FCSSZGCComprassFile();


//厦门竣工业务五
void FCSSetZFLXProjectCode();
void FCSAxisAutoAnnotion();
void FCSAnnPretreatment();
void FCSCLSetAttatchedInfo();
void FCSAxisManualAnnotion();
void FCSDrawSurroundingBuild();
void FCSAutoBuildingInsideAnnotion();
void FCSCLExportResult2DOC();
void FCSCLExportResult2DWG1();
void FCSCLExportResult2DWG2();
void FCSCLComprassFile();
void FCSCLDrawBuildOutLine();

//厦门竣工业务六
void FCSSetPBProjectCode();
void FCSDrawPhotoPositon();
void FCSOwnershipBoundary();
void FCSBridgeDrawRedLine();
void FCSGPBSetAttatchedInfo();
void FCSDrawOwnershipBoundaries();
void FCSControlPointAndRoadPicturePosition();
void FCSExportDwgPB();
void ExportDWGPBElevation();
void ExportDWGPBStratificationPlan();
void ExportBuildAreaHeightAndBirdgeHeight();
void ExportApplyTable();
void ExportPedestrianBridge();
void FCSModifyPhoteSize();
void FCSSetBirdgeLayer();
void FCSBirdgeHatch();
void FCSBirdgeZPT();
void FCSBridgeHatch();
void FCSBridgeSetHeight();
void FCSBridgeAnnoate();
void FCSExportVerticalView();
void FCSSetChannelBodyPosition();
void FCSPBStatisticAreaInfo();
void FCSSetPBClearWidth();
void FCSSetPBClearHeight();
void FCSRemarkLengthWidth();
void FCSRemarkBridgeThickness();
void FCSRemarkBridgeBalustrade();
void FCSRemarkLoadLevation();
void FCSRemarkBridgeLevation();
void FCSPBChangePictureProperty();
void FCSExportPBReport();
void FCSSetHandlePipeLine();

//业务六 房屋
void FCSSetFWProjectCode();
void FCSDrawCarAddress();
void FCSDimensionsForCar();
void FCSSetCarAnnotation();
void FCSDrawStratificationPlan();
void FCSSetStratificationPlanInfo();
void FCSExportBuildHeightAndAreaDoc();
void FCSDimensionsForChangeArea();
void FCSDrawBuildChangeArea();
void FCSAddBuildChangeAnnotion();
void FCSDrawFunctionalAreas();
void FCSDimensionsFunctionalAreas();
void FCSSetFunctionalAreasInfo();
void FCSStatisticFunctionArea();
void FCSExportHighlySchematic();
void FCSExportEntitys2File();
void FCSExportStratificationPlan();
void FCSSetExportHouseDocInfo();
void FCSExportHouseDoc();
void FCSExportPlanInfo2MDB();
void  FCSBuildSetAttatchedInfo();
void FCSFunctionDefine();
void FCSExportBuildHeightAndAreaInfo();
void FCSExportChangeAreaDWG();
void FCSExportZPT();
void FCSBuildHatch();
void FCSCalculateJZZDMJ();//计算建筑占地面积
void FCSDrawRotateLine();
void FCSChangeLayerInfo();
void FCSDrawAreaRange();
void FCSComprassFile();
void FCSPicturePosition();
void FCSAddCompress();
void FCSLineConvertChangeArea();
void FCSAddFunctionAnnotion();
void FCSSetMainHeight();
void FCSSetMezzanineInfo();
void FCSSetCarProperty();
void FCSSettingProjectInfo();
void FCSAddZPTElevation();
void FCSDelZPTElevation();
void FCSSingleEdgeNote();
void FCSModifyBuildWidth();
void FCSSetLayerInfoDlg();
void FCSExportXMFWX();
void FCSSetLayerInfo();
void FCSOutputInspection();
void FCSPDCompress();
void FCSAddDimRoad();

void FCSExportBuildHeightPicture();
void FCSModifyLayerHieght();
void FCSAddChangeNote();
void FCSDimLine();
void FCSBuildCornerNote();
void FCSDrawProjectLine();
void FCSDrawDXFWX();
void FCSEntityColor();

void FCSSetDLBDGXProjectCode();
void FCSExportControlPointAndRoadPicture();					//道路控制点以及道路图片
void FCSExportRoadStratificationPlan();						//道路总平图
void FCSExportRoadProfile();								//道路纵断面
void FCSExportRoadCrossSectional();							//道路横断面
void FCSExportRoadComprehensiveCrossSectional();			//道路综合横断面布置图
void FCSExportRoadResultContrastTechnicalSummary();			//道路成果对照以及技术总结

//质检
void FCSCheckRedLineQuality();
void FCSSetProjectContext();
void FCSAnnotationType();
void FCSExportMDB();
void FCSExportJPG();
void FCSSingleExportJPG();
void FCSExportFCPMJPG();
void FCSOverStepRedLine();
void FCSTQOverStepRedLine();
void FCSJGSingleNodeAnnotation();
void FCSJGMulitNodeAnnotation();
void FCSCombine();
void FCSExportPoint();
void FCSSetSubLine();
void FCSAbjustText();
void FCSAbjustArrow();
void FCSModifyAnnoate();
void FCSCheckBuildLayerAndMDB();

//管线
void FCSSetGXProjectCode();
void FCSPIPELINEModifyProjectInfo();
void FCSImportPipeLine();
void FCSSetPBCLJSSM();

void FCSChangePictureProperty();
void FCSOpenProject();
void FCSCompressProject();
void FCSOpenPath();
void FCSProjectInfoPrompt();
void FCSOutputDiXingTu();
void FCSRegen();
void FCSEElevationalPoint();
void FCSModifyPiPeLineProperty();

/* @接口 输出实体信息(可选择输出dwg还是jpg)
* @作者 杨发荷
* @时间 2017年05月23日
*/
void FCSExportEntitys();

/* @接口 
 * @作者 杨发荷
 * @时间 2017年08月11日
 */
void FCSSetProjectInfo();

/* @接口 修改线宽
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年6月10号
 */
void FCSModifyGCCLLineWidth();

/* @接口 
 * @作者 杨发荷
 * @时间 2017年08月24日
 */
void FCSCopyFiles();

/* @接口 
 * @作者 杨发荷
 * @时间 2017年9月24日
 */
void FCSClearJPG();

void OpenDwgCmd();
void OpenDwg(CString , CString );
void FCSTestCommand();

void FCSRedLineLoftingA3();
void FCSRedLineLoftingA4();

/* @接口 计算线的长度
 * @参数 AcGePoint2dArray 传入的节点（也就是线上的节点）
 * @参数 int 线的起点索引（调用的时候，传入0）
 * @参数 int 线的终点索引（调用的时候，传入节点的个数再减1， 也就是node.length() - 1)
 * @返回 double 线的长度
 * @作者 杨发荷
 * @时间 2017年12月9号
 */
double CalcLineLength(const AcGePoint2dArray &node, int sidx = -1, int eidx = -1);

/* @接口 计算线的长度
 * @参数 AcGePoint2dArray 传入的节点（也就是线上的节点）
 * @参数 double 计算节点索引eidx之前的长度
 * @参数 AcGePoint2d 计算出来的中点坐标
 * @参数 double 线的长度
 * @参数 int 线的起点索引（调用的时候，传入0）
 * @参数 int 线的终点索引（调用的时候，传入节点的个数再减1， 也就是node.length() - 1)
 * @返回 bool 是否计算到了中点，计算成功，返回值为true，否则返回false
 * @作者 杨发荷
 * @时间 2017年12月9号
 */
bool CalcMiddlePoint(const AcGePoint2dArray &node, double &dist, AcGePoint2d &mid, double length, int sidx = -1, int eidx = -1);

/* @接口 清除面积配置文件
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年1月26号
 */
void clearInitFile();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年1月26号
 */
void clearOpenedDwgFile();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年1月26号
 */
void openNewDwgFile(const VStr &);

void FCSCompareExport();

/* @接口 面积统计
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年1月30号
 */
void FCSAreaStatistic();

/* @接口 创建内墙线
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年1月30号
 */
void FCSInnerWallLine();

/* @接口 统计外半墙面积
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年1月30号
 */
void FCSStatisticHalfWall();

/* @接口 修改竣工建筑物名称
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年1月31号
 */
void FCSModifyJGBuildName();

/* @接口 检查已经定义的功能区
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年2月1号
 */
void FCSCheckFunctionDefine();

/* @接口 先绘制层
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年5月8号
 */
void FCSDrawLayerFirst();

/* @接口 更改高程点注记编组内容
*/
void FCSHeightPointText();

/* @接口 更改管线绘制参数
*/
void FCSChangePipeSetting();

/* @接口 管线绘制
*/
void FCSDrawPipeLine();

/* @接口 指定道路起点、终点
*/
void FCSRoadStartEnd();

/* @接口 绘制道路总平图
*/
void FCSExportRoadZPT();

/* @接口 输出控制点分布及道路外观照片拍摄位置示意图
*/
void FCSPictureAndKZD();

/* @接口 绘制横断面
*/
void FCSDrawHDM();

/* @接口 管线总平图输出
*/
void FCSExportPipeZPT();

/* @接口 刷新管线属性
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年3月19号
 */
void FCSFlushPipeLineProperty();

/* @接口 过滤规划数据
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年4月16号
 */
void FCSMatchingPipePoint();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年4月19号
 */
void FCSChangePipeLineZJ();

/* @接口 添加
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年4月23号
 */
void FCSAddPipeLineRemark();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年4月24号
 */
void FCSExportPipeLineDwg();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年5月24号
 */
void FCSExportSingleReport();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年6月11号
 */
void FCSModifyPipeLineZJXH();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年6月14号
 */
void FCSExportPipeLineZPT();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年6月14号
 */
void FCSExportPipeLineDZT();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2019年7月3号
 */
void FCSSetBridgeProperty();

// 输出横断面图;
void CorssSection();

// 输出纵段面图;
void VerSection();

void AddXData();

/* @接口 修改道路属性
 */
void FCSModifyRoadPro();

/* @接口 新建市政项目
 * @时间 2018年4月17日
 */
void FCSSetSZProject();

/* @接口 输出市政报告
 * @时间 2018年4月23日
 */
void FCSExportSZReport();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年5月3号
 */
void FCSRemarkPipePoint();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年5月3号
 */
void FCSDimPipelineFlow();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年5月25号
 */
void FCSAddPrintTK();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年5月25号
 */
void FCSExportRoadJSSM();

/* @接口 
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年6月11号
 */
void FCSModifyRoadHDMInfo();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年7月10号
 */
void FCSBoundaryNote();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年7月13号
 */
void FCSQinagHou();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年10月16号
 */
void FCSModifyProjectInfo();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年7月16号
 */
void FCSCreateCarPosition();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年7月18号
 */
void FCSAddFunctionNote();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年7月27号
 */
void FCSSplitLayer();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月1号
 */
void FCSSetFont();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月1号
 */
void FCSSetCarType();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月2号
 */
void FCSOpenDoor();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月16号
 */
void FCSTiQingMianJi();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月17号
 */
void FCSDistAnnotate();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月20号
 */
void FCSCheckGongNengQu();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年8月20号
 */
void FCSDefineGongNengQu();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年11月15号
 */
void FCSNewSetLayerInfo();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年11月16号
 */
void FCSNewQualityCheck();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年11月17号
 */
void FCSGuiHuaBuYiZhiDWG();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年11月17号
 */
void FCSNewCheWeiStatistic();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年11月26号
 */
void FCSNewSplitMargeLayer();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年11月15号
 */
void FCSNewCreaetFCPMT();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年11月27号
 */
void FCSNewDrawUpPlanning();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年12月10号
 */
void FCSNewOpenLayer();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年12月12号
 */
void FCSNewCloseLayer();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年12月11号
 */
void FCSNewOpenAllLayer();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年12月11号
 */
void FCSNewReConfigurationDlg();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2018年12月14号
 */
void FCSNewGHSTYSGSTBYZ();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2019年9月23号
 */
void FCSDrawDomain();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2019年9月23号
 */
void FCSHeightPropertySettingDlg();

void FCSCreateHeighlySchematic();

void FCSAddHeightSchematicFrame();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2019年8月21号
 */
void FCSModifyProjectRangeLineInfo();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2019年9月9号
 */
void FCSEntityToWhite();

/* @接口 
 * @返回 bool 成功返回值为true，否则返回值为false
 * @作者 杨发荷 
 * @邮箱 575814050@qq.com
 * @时间 2019年9月9号
 */
void FCSCreateMidLine();
//}}AFX_ARX_FUNC

// TODO: Here you can add your own includes / declarations
#endif 

