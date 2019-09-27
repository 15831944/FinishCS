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

//���ſ���ҵ��һ
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

//���ſ���ҵ���
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

//���ſ���ҵ����
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

//���ſ���ҵ����
void FCSSetKZDProjectCode();
void FCSDrawControlPoint();
void FCSEESetAttatchedInfo();
void FCSExportResultToDWG1();
void FCSExportResultToDWG2();
void FCSSZGCComprassFile();


//���ſ���ҵ����
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

//���ſ���ҵ����
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

//ҵ���� ����
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
void FCSCalculateJZZDMJ();//���㽨��ռ�����
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
void FCSExportControlPointAndRoadPicture();					//��·���Ƶ��Լ���·ͼƬ
void FCSExportRoadStratificationPlan();						//��·��ƽͼ
void FCSExportRoadProfile();								//��·�ݶ���
void FCSExportRoadCrossSectional();							//��·�����
void FCSExportRoadComprehensiveCrossSectional();			//��·�ۺϺ���沼��ͼ
void FCSExportRoadResultContrastTechnicalSummary();			//��·�ɹ������Լ������ܽ�

//�ʼ�
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

//����
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

/* @�ӿ� ���ʵ����Ϣ(��ѡ�����dwg����jpg)
* @���� ���
* @ʱ�� 2017��05��23��
*/
void FCSExportEntitys();

/* @�ӿ� 
 * @���� ���
 * @ʱ�� 2017��08��11��
 */
void FCSSetProjectInfo();

/* @�ӿ� �޸��߿�
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��6��10��
 */
void FCSModifyGCCLLineWidth();

/* @�ӿ� 
 * @���� ���
 * @ʱ�� 2017��08��24��
 */
void FCSCopyFiles();

/* @�ӿ� 
 * @���� ���
 * @ʱ�� 2017��9��24��
 */
void FCSClearJPG();

void OpenDwgCmd();
void OpenDwg(CString , CString );
void FCSTestCommand();

void FCSRedLineLoftingA3();
void FCSRedLineLoftingA4();

/* @�ӿ� �����ߵĳ���
 * @���� AcGePoint2dArray ����Ľڵ㣨Ҳ�������ϵĽڵ㣩
 * @���� int �ߵ�������������õ�ʱ�򣬴���0��
 * @���� int �ߵ��յ����������õ�ʱ�򣬴���ڵ�ĸ����ټ�1�� Ҳ����node.length() - 1)
 * @���� double �ߵĳ���
 * @���� ���
 * @ʱ�� 2017��12��9��
 */
double CalcLineLength(const AcGePoint2dArray &node, int sidx = -1, int eidx = -1);

/* @�ӿ� �����ߵĳ���
 * @���� AcGePoint2dArray ����Ľڵ㣨Ҳ�������ϵĽڵ㣩
 * @���� double ����ڵ�����eidx֮ǰ�ĳ���
 * @���� AcGePoint2d ����������е�����
 * @���� double �ߵĳ���
 * @���� int �ߵ�������������õ�ʱ�򣬴���0��
 * @���� int �ߵ��յ����������õ�ʱ�򣬴���ڵ�ĸ����ټ�1�� Ҳ����node.length() - 1)
 * @���� bool �Ƿ���㵽���е㣬����ɹ�������ֵΪtrue�����򷵻�false
 * @���� ���
 * @ʱ�� 2017��12��9��
 */
bool CalcMiddlePoint(const AcGePoint2dArray &node, double &dist, AcGePoint2d &mid, double length, int sidx = -1, int eidx = -1);

/* @�ӿ� �����������ļ�
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��26��
 */
void clearInitFile();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��26��
 */
void clearOpenedDwgFile();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��26��
 */
void openNewDwgFile(const VStr &);

void FCSCompareExport();

/* @�ӿ� ���ͳ��
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��30��
 */
void FCSAreaStatistic();

/* @�ӿ� ������ǽ��
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��30��
 */
void FCSInnerWallLine();

/* @�ӿ� ͳ�����ǽ���
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��30��
 */
void FCSStatisticHalfWall();

/* @�ӿ� �޸Ŀ�������������
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��1��31��
 */
void FCSModifyJGBuildName();

/* @�ӿ� ����Ѿ�����Ĺ�����
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��2��1��
 */
void FCSCheckFunctionDefine();

/* @�ӿ� �Ȼ��Ʋ�
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��5��8��
 */
void FCSDrawLayerFirst();

/* @�ӿ� ���ĸ̵߳�ע�Ǳ�������
*/
void FCSHeightPointText();

/* @�ӿ� ���Ĺ��߻��Ʋ���
*/
void FCSChangePipeSetting();

/* @�ӿ� ���߻���
*/
void FCSDrawPipeLine();

/* @�ӿ� ָ����·��㡢�յ�
*/
void FCSRoadStartEnd();

/* @�ӿ� ���Ƶ�·��ƽͼ
*/
void FCSExportRoadZPT();

/* @�ӿ� ������Ƶ�ֲ�����·�����Ƭ����λ��ʾ��ͼ
*/
void FCSPictureAndKZD();

/* @�ӿ� ���ƺ����
*/
void FCSDrawHDM();

/* @�ӿ� ������ƽͼ���
*/
void FCSExportPipeZPT();

/* @�ӿ� ˢ�¹�������
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��3��19��
 */
void FCSFlushPipeLineProperty();

/* @�ӿ� ���˹滮����
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��4��16��
 */
void FCSMatchingPipePoint();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��4��19��
 */
void FCSChangePipeLineZJ();

/* @�ӿ� ���
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��4��23��
 */
void FCSAddPipeLineRemark();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��4��24��
 */
void FCSExportPipeLineDwg();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��5��24��
 */
void FCSExportSingleReport();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��6��11��
 */
void FCSModifyPipeLineZJXH();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��6��14��
 */
void FCSExportPipeLineZPT();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��6��14��
 */
void FCSExportPipeLineDZT();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2019��7��3��
 */
void FCSSetBridgeProperty();

// ��������ͼ;
void CorssSection();

// ����ݶ���ͼ;
void VerSection();

void AddXData();

/* @�ӿ� �޸ĵ�·����
 */
void FCSModifyRoadPro();

/* @�ӿ� �½�������Ŀ
 * @ʱ�� 2018��4��17��
 */
void FCSSetSZProject();

/* @�ӿ� �����������
 * @ʱ�� 2018��4��23��
 */
void FCSExportSZReport();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��5��3��
 */
void FCSRemarkPipePoint();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��5��3��
 */
void FCSDimPipelineFlow();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��5��25��
 */
void FCSAddPrintTK();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��5��25��
 */
void FCSExportRoadJSSM();

/* @�ӿ� 
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��6��11��
 */
void FCSModifyRoadHDMInfo();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��7��10��
 */
void FCSBoundaryNote();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��7��13��
 */
void FCSQinagHou();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��10��16��
 */
void FCSModifyProjectInfo();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��7��16��
 */
void FCSCreateCarPosition();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��7��18��
 */
void FCSAddFunctionNote();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��7��27��
 */
void FCSSplitLayer();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��1��
 */
void FCSSetFont();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��1��
 */
void FCSSetCarType();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��2��
 */
void FCSOpenDoor();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��16��
 */
void FCSTiQingMianJi();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��17��
 */
void FCSDistAnnotate();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��20��
 */
void FCSCheckGongNengQu();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��8��20��
 */
void FCSDefineGongNengQu();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��11��15��
 */
void FCSNewSetLayerInfo();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��11��16��
 */
void FCSNewQualityCheck();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��11��17��
 */
void FCSGuiHuaBuYiZhiDWG();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��11��17��
 */
void FCSNewCheWeiStatistic();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��11��26��
 */
void FCSNewSplitMargeLayer();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��11��15��
 */
void FCSNewCreaetFCPMT();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��11��27��
 */
void FCSNewDrawUpPlanning();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��12��10��
 */
void FCSNewOpenLayer();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��12��12��
 */
void FCSNewCloseLayer();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��12��11��
 */
void FCSNewOpenAllLayer();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��12��11��
 */
void FCSNewReConfigurationDlg();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2018��12��14��
 */
void FCSNewGHSTYSGSTBYZ();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2019��9��23��
 */
void FCSDrawDomain();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2019��9��23��
 */
void FCSHeightPropertySettingDlg();

void FCSCreateHeighlySchematic();

void FCSAddHeightSchematicFrame();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2019��8��21��
 */
void FCSModifyProjectRangeLineInfo();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2019��9��9��
 */
void FCSEntityToWhite();

/* @�ӿ� 
 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
 * @���� ��� 
 * @���� 575814050@qq.com
 * @ʱ�� 2019��9��9��
 */
void FCSCreateMidLine();
//}}AFX_ARX_FUNC

// TODO: Here you can add your own includes / declarations
#endif 

