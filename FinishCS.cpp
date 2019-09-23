// HelloWorld.cpp : Initialization functions
#include "StdAfx.h"
#include "StdArx.h"
#include "Encryption.h"
#include "XMJGDialog.h"
#include "Dialogs.h"
#include "XMJGDialog.h"
#include "SYPSampleJJG.h"
//#include <afxdllx.h>

HINSTANCE _hdllInstance = NULL;
SYPSelectEntity *g_selectentity = NULL;

// This command registers an ARX command.
void AddCommand(const TCHAR* cmdGroup, const TCHAR* cmdInt, const TCHAR* cmdLoc,
	const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal = -1);

// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_MSG
void InitApplication();
void UnloadApplication();
//}}AFX_ARX_MSG

// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_ADDIN_FUNCS
//}}AFX_ARX_ADDIN_FUNCS


////////////////////////////////////////////////////////////////////////////
//
// Define the sole extension module object.
AC_IMPLEMENT_EXTENSION_MODULE(HelloWorldDLL);

// Now you can use the CAcModuleResourceOverride class in
// your application to switch to the correct resource instance.
// Please see the ObjectARX Documentation for more details

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		_hdllInstance = hInstance;
		// Extension DLL one time initialization
		HelloWorldDLL.AttachInstance(hInstance);
		InitAcUiDLL();
	}
	else if(dwReason == DLL_PROCESS_DETACH)
	{
		// Terminate the library before destructors are called
		HelloWorldDLL.DetachInstance();

	}
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// ObjectARX EntryPoint
extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch(msg)
	{
		case AcRx::kInitAppMsg:
			// Comment out the following line if your
			// application should be locked into memory
			acrxDynamicLinker->unlockApplication(pkt);
			acrxDynamicLinker->registerAppMDIAware(pkt);
			InitApplication();
			break;
		case AcRx::kUnloadAppMsg:
			UnloadApplication();
			break;
	}
	return AcRx::kRetOK;
}

// Init this application. Register your
// commands, reactors...
void InitApplication()
{
	CString strPath;
	strPath.Format(_T("%scasslog.log"), getSystemPath());
	DeleteFile(strPath);
	CreateLog(strPath);
	g_selectentity = new SYPSelectEntity();
	AddCommand(_T("FCS"), _T("FCSRegister"), _T("FCSRegister"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSFCSRegister);

#ifndef NOSENSE
	if(false == RequestSense4Context(3)) return; //������
	if(false == InitGsmhDog())
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ������"), _T("������Ϣ"), MB_OK);
		return;
	}
#ifndef _WIN64
	if(false == RSA_Verify_Dog()) return;
#endif
#endif
	
#ifdef CODEMETER
	//////////////////////////////////////////////////////////////////////////
	//����֤����Կ
	mpRandomCharArray = new unsigned char[RND_ARR_LEN];
	// initialize random generator
	//srand((unsigned int)GetTickCount());
	srand((UINT)GetTickCount());
	for(int i = 0; i < RND_ARR_LEN; i++)
	{
		mpRandomCharArray[i] = rand() * 255 / RAND_MAX;
	}
	CalculateSignature(mpRandomCharArray, RND_ARR_LEN);
	if(false == CheckLIC()) return;
	//////////////////////////////////////////////////////////////////////////
#endif
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_INIT
	//AddCommand(_T"FCS"),_T("TRIMPOLY"),_T("ʵ���и����"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ArxTrimPoly);
	AddCommand(_T("FCS"), _T("fcstest"), _T("f0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSFCSTest);
	AddCommand(_T("FCS"), _T("addhoursenote"), _T("f1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddHourseNote);
	AddCommand(_T("FCS"), _T("drawdeepbuilding"), _T("f2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawdeepbuilding);
	AddCommand(_T("FCS"), _T("InsertBaseMap"), _T("f3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSInsertBaseMap);
	AddCommand(_T("FCS"), _T("NoteIndoorsHeight"), _T("f4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSNoteIndoorsHeight);
	AddCommand(_T("FCS"), _T("InsertFrame_Total_Floor_Plan"), _T("f5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSInsertFrame_Total_Floor_Plan);
	AddCommand(_T("FCS"), _T("ProjectInfoDlg"), _T("f6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSProjectInfoDlg);
	AddCommand(_T("FCS"), _T("StaticFloorInfo"), _T("f7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSStaticFloorInfo);
	AddCommand(_T("FCS"), _T("InsertFrame_FloorPlane"), _T("f8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSInsertFrame_FloorPlane);
	AddCommand(_T("FCS"), _T("InsertFrame_FloorPlane_Multy"), _T("f23"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSInsertFrame_FloorPlane_Multy);
	AddCommand(_T("FCS"), _T("ExportSurveyResultReport"), _T("f9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportSurveyResultReport);
	AddCommand(_T("FCS"), _T("ExportSummaryTable"), _T("f10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportSummaryTable);
	AddCommand(_T("FCS"), _T("ExtractFeatureTable"), _T("f11"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExtractFeatureTable);
	AddCommand(_T("FCS"), _T("AddFenceNote"), _T("f12"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddFenceNote);
	AddCommand(_T("FCS"), _T("AddTextNote"), _T("f13"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddTextNote);
	AddCommand(_T("FCS"), _T("AddTextNote2"), _T("f33"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddTextNote2);
	AddCommand(_T("FCS"), _T("InertHouseHoldFrame"), _T("f14"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSInertHouseHoldFrame);
	AddCommand(_T("FCS"), _T("ExportGroundAttachmentsTable_Standard2"), _T("f34"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportGroundAttachmentsTable_Standard2);
	AddCommand(_T("FCS"), _T("ExportGroundAttachmentsTable"), _T("f15"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportGroundAttachmentsTable);
	AddCommand(_T("FCS"), _T("OwnerInfoTable"), _T("f16"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOwnerInfoTable);
	AddCommand(_T("FCS"), _T("OwnerInfoSummaryTable"), _T("f17"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOwnerInfoSummaryTable);
	AddCommand(_T("FCS"), _T("OwnerInfoSummaryTable_Standard2"), _T("f35"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOwnerInfoSummaryTable_Standard2);
	AddCommand(_T("FCS"), _T("ExtentBuilding"), _T("f18"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExtentBuilding);
	AddCommand(_T("FCS"), _T("checkElevationPoint"), _T("f19"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckElevationPoint);
	AddCommand(_T("FCS"), _T("NoteCoord_Total_Floor_Plan"), _T("f20"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSNoteCoord_Total_Floor_Plan);
	AddCommand(_T("FCS"), _T("DrawThings"), _T("f21"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawThings);
	AddCommand(_T("FCS"), _T("ScaleDimensionText"), _T("f22"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSScaleDimensionText);
	AddCommand(_T("FCS"), _T("ExtractToOrignDoc"), _T("f24"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExtractToOrignDoc);
	AddCommand(_T("FCS"), _T("AddAttachmentNote"), _T("f25"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddAttachmentNote);
	AddCommand(_T("FCS"), _T("ExtractText"), _T("f26"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExtractText);
	AddCommand(_T("FCS"), _T("CheckArea"), _T("f27"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckArea);
	AddCommand(_T("FCS"), _T("RefreshFrameData"), _T("f28"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRefreshFrameData);
	AddCommand(_T("FCS"), _T("CheckUnCalulatedBuildings"), _T("f29"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckUnCalulatedBuildings);
	AddCommand(_T("FCS"), _T("PutP_FCS"), _T("f30"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSPutP_FCS);
	AddCommand(_T("FCS"), _T("DZPB_FCS"), _T("bb"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDZPB_FCS);
	AddCommand(_T("FCS"), _T("CheckResultWizard"), _T("f31"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckResultWizard);
	AddCommand(_T("FCS"), _T("CheckMustFillFields"), _T("f32"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckMustFillFields);

	//���ſ���ͨ�ù���
	AddCommand(_T("FCS"), _T("FCSDrawScopeLine"), _T("f0-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawScopeLine);
	AddCommand(_T("FCS"), _T("FCSExportEntitys"), _T("F0-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportEntitys);
	AddCommand(_T("FCS"), _T("FCSSetProjectInfo"), _T("F0-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetProjectInfo);
	AddCommand(_T("FCS"), _T("FCSModifyLineWidth"), _T("F0-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyGCCLLineWidth);

	//0313 ��Ŀһ ���߷���
	AddCommand(_T("FCS"), _T("FCSSetRLProjectCode"), _T("f1-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetRLProjectCode);
	AddCommand(_T("FCS"), _T("FCSImportData"), _T("f1-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSImportData);
	AddCommand(_T("FCS"), _T("FCSRLSetPropery"), _T("f1-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRLSetPropery);
	AddCommand(_T("FCS"), _T("FCSManualDrawPoint"), _T("f1-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSManualDrawPoint);
	AddCommand(_T("FCS"), _T("FCSDrawLine"), _T("f1-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawLine);
	AddCommand(_T("FCS"), _T("FCSAddRedLineNode"), _T("f1-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddRedLineNode);
	AddCommand(_T("FCS"), _T("FCSDelRedLineNode"), _T("f1-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDelRedLineNode);
	AddCommand(_T("FCS"), _T("FCSAdjustNode"), _T("f1-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAdjustNode);
	AddCommand(_T("FCS"), _T("FCSAddAnnotation"), _T("f1-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAutoAnnotation);
	AddCommand(_T("FCS"), _T("FCSManualAnnotation"), _T("f1-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSManualAnnotation);
	AddCommand(_T("FCS"), _T("FCSMoveAnnotation"), _T("f1-10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSMoveAnnotation);
	AddCommand(_T("FCS"), _T("FCSManualLineAnnotation"), _T("f1-11"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSManualLineAnnotation);
	AddCommand(_T("FCS"), _T("FCSRedlineSetAttatchedInfo"), _T("f1-12"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRedlineSetAttatchedInfo);
	AddCommand(_T("FCS"), _T("FCSRedLineExportDWG"), _T("f1-13"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRedLineExportDWG);
	AddCommand(_T("FCS"), _T("FCSReadReveseDat"), _T("f1-14"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSReadReveseDat);
	AddCommand(_T("FCS"), _T("FCSRLComprassFile"), _T("f1-15"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRLComprassFile);
	AddCommand(_T("FCS"), _T("FCSInputCoorPos"), _T("f1-16"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSInputCoorPos);
	AddCommand(_T("FCS"), _T("FCSRedLineAddNode"), _T("f1-17"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRedLineAddNode);								//�ڵ����ע��
	AddCommand(_T("FCS"), _T("FCSRedDistAnnotion"), _T("f1-18"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRedDistAnnotion);

	//0313 ��Ŀ�� ��������
	AddCommand(_T("FCS"), _T("FCSSetJZProjectCode"), _T("F2-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetJZProjectCode);
	AddCommand(_T("FCS"), _T("FCSInsertRedLineFile"), _T("F2-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSInsertRedLineFile);
	AddCommand(_T("FCS"), _T("FCSDrawBuildOutLine"), _T("F2-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawBuildOutLine);							//���ƽ�����
	AddCommand(_T("FCS"), _T("FCSDrawBuildBoundaries"), _T("F2-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawBuildBoundaries);
	AddCommand(_T("FCS"), _T("FCSShowOutLineSymbol"), _T("F2-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSShowOutLineSymbol);						//��ʾ����������
	AddCommand(_T("FCS"), _T("FCSHideOutLineSymbol"), _T("F2-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSHideOutLineSymbol);						//���ؽ���������
	AddCommand(_T("FCS"), _T("FCSSetAnnotationAttribute"), _T("F2-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetAnnotationAttribute);				//��Ӿ����ע����
	AddCommand(_T("FCS"), _T("FCSChangeAzimuthOrder"), _T("F2-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSChangeAzimuthOrder);						//�����޸ľ����ע����
	AddCommand(_T("FCS"), _T("FCSChangeAzimuthOrderAll"), _T("F2-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSChangeAzimuthOrderAll);				//�����޸ľ����ע����
	AddCommand(_T("FCS"), _T("FCSBOLSetAttatchedInfo"), _T("F2-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBOLSetAttatchedInfo);					//������Ŀ��Ϣ
	AddCommand(_T("FCS"), _T("FCSExportToDWG1"), _T("F2-10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportToDWG1);									//����ɹ���ʽ1
	AddCommand(_T("FCS"), _T("FCSExportToDWG2"), _T("F2-11"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportToDWG2);									//����ɹ���ʽ2
	AddCommand(_T("FCS"), _T("FCSExportToDOC"), _T("F2-12"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportToDOC);									//����ɹ�����
	AddCommand(_T("FCS"), _T("FCSJZComprassFile"), _T("F2-13"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSJZComprassFile);								//�ļ����
	AddCommand(_T("FCS"), _T("FCSDrawBuildNearestBoundaries"), _T("F2-14"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawBuildNearestBoundaries);		//�����ܱ߽���
	AddCommand(_T("FCS"), _T("FCSModifyBuildName"), _T("F2-15"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyBuildName);							//�޸Ľ���������

	//taorong 0227 ��Ŀ�� ��·��������	
	AddCommand(_T("FCS"), _T("FCSSetDLProjectCode"), _T("f3-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetDLProjectCode);							//�½���Ŀ
	AddCommand(_T("FCS"), _T("FCSDrawRoadEdgeLine"), _T("f3-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawRoadEdgeLine);							//���Ƶ�·�߽�
	AddCommand(_T("FCS"), _T("FCSDrawRoadCentreLine"), _T("f3-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawRoadCentreLine);						//���Ƶ�·������
	AddCommand(_T("FCS"), _T("FCSExportCoodinateTable"), _T("f3-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportCoodinateTable);					//����̱߳�
	AddCommand(_T("FCS"), _T("FCSSetRoadText"), _T("f3-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetRoadText);									//��ӵ�·���յ�ע��
	AddCommand(_T("FCS"), _T("FCSSetAttatchedInfo"), _T("f3-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRSOSetAttatchedInfo);						//������Ŀ��Ϣ
	AddCommand(_T("FCS"), _T("FCSExportDWGOutput"), _T("f3-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportDWGOutput);							//����ɹ�dwg
	AddCommand(_T("FCS"), _T("FCSExportWORDOutput"), _T("f3-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportWORDOutput);							//����ɹ�����
	AddCommand(_T("FCS"), _T("FCSTextDataCheck"), _T("f3-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSTextDataCheck);								//
	AddCommand(_T("FCS"), _T("FCSSetRoadStartEndPoint"), _T("f3-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetRoadStartEndPoint);					//���õ�·���յ�
	AddCommand(_T("FCS"), _T("FCSDLComprassFile"), _T("f3-10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDLComprassFile);								//�ɹ����

	//taorong ��Ŀ�� ˮ׼�̲߳���
	AddCommand(_T("FCS"), _T("FCSSetKZDProjectCode"), _T("f4-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetKZDProjectCode);						//�½���Ŀ
	AddCommand(_T("FCS"), _T("FCSDrawControlPoint"), _T("f4-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawControlPoint);							//���ƿ��Ƶ�
	AddCommand(_T("FCS"), _T("FCSEESetAttatchedInfo"), _T("f4-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSEESetAttatchedInfo);						//������Ŀ��Ϣ
	AddCommand(_T("FCS"), _T("FCSExportResult2DWG1"), _T("f4-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportResultToDWG1);						//����ɹ���ʽ1
	AddCommand(_T("FCS"), _T("FCSExportResult2DWG2"), _T("f4-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportResultToDWG2);						//����ɹ���ʽ2
	AddCommand(_T("FCS"), _T("FCSSZGCComprassFile"), _T("F4-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSZGCComprassFile);							//�ɹ����

	//���Ʋ������Ƶ㣨��Ŀ�ģ���Ŀ��-���ţ���·��

	//taorong ��Ŀ�� ��0����
	AddCommand(_T("FCS"), _T("FCSSetZFLXProjectCode"), _T("f5-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetZFLXProjectCode);						//�½���Ŀ
	AddCommand(_T("FCS"), _T("FCSDrawSurroundingBuild"), _T("f5-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawSurroundingBuild);					//
	AddCommand(_T("FCS"), _T("FCSAxisAutoAnnotion"), _T("f5-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAxisAutoAnnotion);							//��������ע��
	AddCommand(_T("FCS"), _T("FCSAxisManualAnnotion"), _T("f5-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAxisManualAnnotion);						//��������ע��
	AddCommand(_T("FCS"), _T("FCSAutoBuildingInsideAnnotion"), _T("f5-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAutoBuildingInsideAnnotion);		//�߳���ע
	AddCommand(_T("FCS"), _T("FCSCLSetAttatchedInfo"), _T("f5-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCLSetAttatchedInfo);						//��Ŀ��Ϣ����
	AddCommand(_T("FCS"), _T("FCSCLExportResult2DOC"), _T("f5-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCLExportResult2DOC);						//����ɹ��ĵ�
	AddCommand(_T("FCS"), _T("FCSCLExportResult2DWG1"), _T("f5-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCLExportResult2DWG1);					//����ɹ���ʽ1
	AddCommand(_T("FCS"), _T("FCSCLExportResult2DWG2"), _T("f5-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCLExportResult2DWG2);					//����ɹ���ʽ2
	AddCommand(_T("FCS"), _T("FCSCLComprassFile"), _T("F5-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCLComprassFile);								//�ɹ����
	AddCommand(_T("FCS"), _T("FCSCLDrawBuildOutLine"), _T("F5-10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCLDrawBuildOutLine);						//���ƽ�����
	//taorong ��Ŀ�� �������ſ���
	//������Ƭ����λ�ã����ţ���·��
	AddCommand(_T("FCS"), _T("FCSSetPBProjectCode"), _T("f6-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetPBProjectCode);
	AddCommand(_T("FCS"), _T("FCSDrawPhotoPositon"), _T("f6-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawPhotoPositon);
	AddCommand(_T("FCS"), _T("FCSDrawBuildCoverArea"), _T("f6-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBridgeDrawRedLine);
	AddCommand(_T("FCS"), _T("FCSDrawOwnershipBoundaries"), _T("f6-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawOwnershipBoundaries);
	AddCommand(_T("FCS"), _T("FCSGPBSetAttatchedInfo"), _T("f6-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSGPBSetAttatchedInfo);
	AddCommand(_T("FCS"), _T("FCSControlPointAndRoadPicturePosition"), _T("f6-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSControlPointAndRoadPicturePosition);
	AddCommand(_T("FCS"), _T("FCSExportDwgPB"), _T("f6-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportDwgPB);
	AddCommand(_T("FCS"), _T("ExportDWGPBElevation"), _T("f6-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ExportDWGPBElevation);
	AddCommand(_T("FCS"), _T("ExportDWGPBStratificationPlan"), _T("f6-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_SESSION, ExportDWGPBStratificationPlan);
	AddCommand(_T("FCS"), _T("ExportBuildAreaHeightAndBirdgeHeight"), _T("f6-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ExportBuildAreaHeightAndBirdgeHeight);
	AddCommand(_T("FCS"), _T("ExportApplyTable"), _T("f6-10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ExportApplyTable);												//�����
	AddCommand(_T("FCS"), _T("ExportPedestrianBridge"), _T("f6-11"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, ExportPedestrianBridge);									//���
	AddCommand(_T("FCS"), _T("FCSModifyPhoteSize"), _T("f6-12"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyPhoteSize);											//�޸���Ƭ��С
	AddCommand(_T("FCS"), _T("FCSSetBirdgeLayer"), _T("f6-13"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetBirdgeLayer);												//�������ŵĲ���
	AddCommand(_T("FCS"), _T("FCSBirdgeHatch"), _T("f6-14"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBirdgeHatch);													//�������
	AddCommand(_T("FCS"), _T("FCSBirdgeZPT"), _T("f6-15"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBirdgeZPT);														//������ƽͼ
	AddCommand(_T("FCS"), _T("FCSBridgeHatch"), _T("F6-16"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBridgeHatch);													//�������
	AddCommand(_T("FCS"), _T("FCSBridgeSetHeight"), _T("F6-17"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBridgeSetHeight);											//�������Ÿ߶���Ϣ
	AddCommand(_T("FCS"), _T("FCSBridgeAnnoate"), _T("F6-18"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBridgeAnnoate);												//��ע�߶���Ϣ
	AddCommand(_T("FCS"), _T("FCSExportVerticalView"), _T("F6-19"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportVerticalView);										//������������ͼ
	AddCommand(_T("FCS"), _T("FCSSetChannelBodyPosition"), _T("F6-20"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetChannelBodyPosition);								//����ͨ������λ��
	AddCommand(_T("FCS"), _T("FCSPBStatisticAreaInfo"), _T("F6-21"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSPBStatisticAreaInfo);									//ͳ�����		
	AddCommand(_T("FCS"), _T("FCSSetPBClearWidth"), _T("F6-22"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetPBClearWidth);											//�������ž���
	AddCommand(_T("FCS"), _T("FCSSetPBClearHeight"), _T("F6-23"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetPBClearHeight);											//�������ž���
	AddCommand(_T("FCS"), _T("FCSRemarkLengthWidth"), _T("F6-24"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRemarkLengthWidth);										//��ע���ų���
	AddCommand(_T("FCS"), _T("FCSRemarkBridgeThickness"), _T("F6-25"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRemarkBridgeThickness);								//��ע���ź��
	AddCommand(_T("FCS"), _T("FCSRemarkBridgeBalustrade"), _T("F6-26"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRemarkBridgeBalustrade);								//��ע�������˸߶�
	AddCommand(_T("FCS"), _T("FCSRemarkLoadLevation"), _T("F6-27"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRemarkLoadLevation);										//��ע��·���
	AddCommand(_T("FCS"), _T("FCSRemarkBridgeLevation"), _T("F6-28"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRemarkBridgeLevation);									//��ע���ű��
	AddCommand(_T("FCS"), _T("FCSPBChangePictureProperty"), _T("F6-29"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSPBChangePictureProperty);
	AddCommand(_T("FCS"), _T("FCSSetPBCLJSSM"), _T("F6-30"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetPBCLJSSM);													//��������˵����Ϣ
	AddCommand(_T("FCS"), _T("FCSExportPBReport"), _T("F6-31"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportPBReport);												//����ɹ��ĵ�
	AddCommand(_T("FCS"), _T("FCSPDCompress"), _T("F6-32"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSPDCompress);														//���ݴ��
	AddCommand(_T("FCS"), _T("FCSAddDimRoad"), _T("F6-33"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddDimRoad);

	//AddCommand(_T("FCS"), _T("FCSExportVerticalView"), _T("F6-19"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportVerticalView);									//������������ͼ
	
	//��������
	AddCommand(_T("FCS"), _T("FCSSetFWProjectCode"), _T("f7-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetFWProjectCode);											//���÷��ݿ�����Ϣ
	AddCommand(_T("FCS"), _T("FCSDrawCarAddress"), _T("f7-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawCarAddress);												//���Ƴ�λ
	AddCommand(_T("FCS"), _T("FCSDimensionsForCar"), _T("f7-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDimensionsForCar);											//�ع���λע��
	AddCommand(_T("FCS"), _T("FCSSetCarAnnotation"), _T("f7-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetCarAnnotation);											//���ó�λע��
	AddCommand(_T("FCS"), _T("FCSDrawStratificationPlan"), _T("f7-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawStratificationPlan);								//���Ʒֲ�ƽ��ͼ
	AddCommand(_T("FCS"), _T("FCSSetStratificationPlanInfo"), _T("f7-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetStratificationPlanInfo);						//���÷ֲ�ƽ����Ϣ
	AddCommand(_T("FCS"), _T("FCSDrawBuildChangeArea"), _T("F7-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawBuildChangeArea);									//���Ʊ������
	AddCommand(_T("FCS"), _T("FCSDimensionsForChangeArea"), _T("F7-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDimensionsForChangeArea);							//�ع��������
	AddCommand(_T("FCS"), _T("FCSAddBuildChangeAnnotion"), _T("F7-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddBuildChangeAnnotion);								//��ӱ������ע��
	AddCommand(_T("FCS"), _T("FCSDrawFunctionalAreas"), _T("F7-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawFunctionalAreas);									//���ƹ�����
	AddCommand(_T("FCS"), _T("FCSSetFunctionalAreasInfo"), _T("F7-11"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetFunctionalAreasInfo);								//���ù�������Ϣ
	AddCommand(_T("FCS"), _T("FCSStatisticFunctionArea"), _T("F7-12"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSStatisticFunctionArea);								//ͳ�ƹ�����
	AddCommand(_T("FCS"), _T("FCSExportHighlySchematic"), _T("F7-13"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportHighlySchematic);								//����߶�ʾ��ͼ
	AddCommand(_T("FCS"), _T("FCSExportEntitys2File"), _T("F7-14"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportEntitys2File);										//���ʵ�嵽�ļ���
	AddCommand(_T("FCS"), _T("FCSExportStratificationPlan"), _T("F7-15"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportStratificationPlan);							//����ֲ�ƽ��ͼ
	AddCommand(_T("FCS"), _T("FCSExportBuildHeightAndAreaDoc"), _T("F7-16"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportBuildHeightAndAreaDoc);					//����߶������
	AddCommand(_T("FCS"), _T("FCSExportHouseDoc"), _T("F7-17"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportHouseDoc);												//������ݱ���
	AddCommand(_T("FCS"), _T("FCSExportPlanInfo2MDB"), _T("F7-18"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportPlanInfo2MDB);										//���ƽ����Ϣ�������ļ���
	AddCommand(_T("FCS"), _T("FCSBuildSetAttatchedInfo"), _T("F7-19"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBuildSetAttatchedInfo);								//������Ϣ
	AddCommand(_T("FCS"), _T("FCSFunctionDefine"), _T("F7-20"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSFunctionDefine);												//����������
	AddCommand(_T("FCS"), _T("FCSExportBuildHeightAndAreaInfo"), _T("F7-21"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_SESSION, FCSExportBuildHeightAndAreaInfo);//����߶Ⱥ������Ϣ
	AddCommand(_T("FCS"), _T("FCSExportChangeAreaDWG"), _T("F7-22"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportChangeAreaDWG);									//������ͼ
	AddCommand(_T("FCS"), _T("FCSExportZPT"), _T("F7-23"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportZPT);														//�����ƽͼ
	AddCommand(_T("FCS"), _T("FCSBuildHatch"), _T("F7-24"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBuildHatch);														//�������
	AddCommand(_T("FCS"), _T("FCSDrawRotateLine"), _T("F7-25"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawRotateLine);												//������ת��
	AddCommand(_T("FCS"), _T("FCSChangeLayerInfo"), _T("F7-26"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSChangeLayerInfo);											 
	AddCommand(_T("FCS"), _T("FCSDrawAreaRange"), _T("F7-27"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawAreaRange);												//���������Χ��
	AddCommand(_T("FCS"), _T("FCSComprassFile"), _T("F7-28"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSComprassFile);													//ѹ���ļ�
	AddCommand(_T("FCS"), _T("FCSPicturePosition"), _T("F7-29"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSPicturePosition);											//�����Ƭλ��ʾ��ͼ
	AddCommand(_T("FCS"), _T("FCSAddCompress"), _T("F7-30"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddCompress);													//���ָ����
	AddCommand(_T("FCS"), _T("FCSLineConvertChangeArea"), _T("F7-31"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSLineConvertChangeArea);								//��ת������
	AddCommand(_T("FCS"), _T("FCSAddFunctionAnnotion"), _T("F7-32"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddFunctionAnnotion);									//��ӷֲ��ע��Ϣ
	AddCommand(_T("FCS"), _T("FCSSetMainHeight"), _T("F7-33"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetMainHeight);												//��ӷֲ��ע��Ϣ
	AddCommand(_T("FCS"), _T("FCSSetMezzanineInfo"), _T("F7-34"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetMezzanineInfo);											//��Ӽв�
	AddCommand(_T("FCS"), _T("FCSSetCarProperty"), _T("F7-35"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetCarProperty);												//���ó�λ����
	AddCommand(_T("FCS"), _T("FCSSettingProjectInfo"), _T("F7-36"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSettingProjectInfo);										//������Ŀ��Ϣ
	AddCommand(_T("FCS"), _T("FCSAddZPTElevation"), _T("F7-37"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddZPTElevation);											//��ƽͼ���
	AddCommand(_T("FCS"), _T("FCSSingleEdgeNote"), _T("F7-38"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSingleEdgeNote);												//����ע��
	AddCommand(_T("FCS"), _T("FCSSetLayerInfoDlg"), _T("F7-39"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetLayerInfoDlg);											//���ò���Ϣ
	AddCommand(_T("FCS"), _T("FCSExportXMFWX"), _T("F7-40"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportXMFWX);													//�����Ŀ��Χ�ߵ�dwg
	AddCommand(_T("FCS"), _T("FCSSetLayerInfo"), _T("F7-41"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetLayerInfo);													//���÷ֲ���Ϣ
	AddCommand(_T("FCS"), _T("FCSOutputInspection"), _T("F7-42"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOutputInspection);											//����ʼ챨��
	AddCommand(_T("FCS"), _T("FCSExportBuildHeightPicture"), _T("F7-43"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportBuildHeightPicture);							//���÷ֲ���Ϣ
	AddCommand(_T("FCS"), _T("FCSModifyLayerHieght"), _T("F7-44"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyLayerHieght);										//�޸Ĳ�ĸ߶ȣ���������߶�ͼ֮��
	AddCommand(_T("FCS"), _T("FCSAddChangeNote"), _T("F7-45"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddChangeNote);
	AddCommand(_T("FCS"), _T("FCSDimLine"), _T("F7-46"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDimLine);															//�ع���
	AddCommand(_T("FCS"), _T("FCSBuildCornerNote"), _T("F7-47"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBuildCornerNote);
	AddCommand(_T("FCS"), _T("FCSDrawProjectLine"), _T("F7-48"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawProjectLine);											//������Ŀ��Χ��
	AddCommand(_T("FCS"), _T("FCSDrawDXFWX"), _T("F7-49"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawDXFWX);														//���Ƶ�����
	AddCommand(_T("FCS"), _T("FCSDelZPTElevation"), _T("F7-50"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDelZPTElevation);											//ɾ�����
	AddCommand(_T("FCS"), _T("FCSCompareExport"), _T("F7-51"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCompareExport);												//2018.1.30
	AddCommand(_T("FCS"), _T("FCSAreaStatistic"), _T("F7-52"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAreaStatistic);
	AddCommand(_T("FCS"), _T("FCSInnerWallLine"), _T("F7-53"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSInnerWallLine);
	AddCommand(_T("FCS"), _T("FCSStatisticHalfWall"), _T("F7-54"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSStatisticHalfWall);										//2018.1.31
	AddCommand(_T("FCS"), _T("FCSModifyJGBuildName"), _T("F7-55"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyJGBuildName);										//�޸Ľ���������	
	AddCommand(_T("FCS"), _T("FCSCheckFunctionDefine"), _T("F7-56"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckFunctionDefine);									//����Ѿ�����Ĺ�����
	AddCommand(_T("FCS"), _T("FCSDrawLayerFirst"), _T("F7-57"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawLayerFirst);												//����Ѿ�����Ĺ�����
	AddCommand(_T("FCS"), _T("FCSBoundaryNote"), _T("F7-58"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSBoundaryNote);													//���߽����ע��
	AddCommand(_T("FCS"), _T("FCSCreateCarPosition"), _T("F7-59"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCreateCarPosition);										//�޸Ľ���������	
	AddCommand(_T("FCS"), _T("FCSAddFunctionNote"), _T("F7-60"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddFunctionNote);
	AddCommand(_T("FCS"), _T("FCSSplitLayer"), _T("F7-61"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSplitLayer);														//���¥��	
	AddCommand(_T("FCS"), _T("FCSSetFont"), _T("F7-62"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetFont);															//���¥��	
	AddCommand(_T("FCS"), _T("FCSSetCarType"), _T("F7-63"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetCarType);														//���¥��	
	AddCommand(_T("FCS"), _T("FCSOpenDoor"), _T("F7-64"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOpenDoor);															//���¥��	
	AddCommand(_T("FCS"), _T("FCSTiQingMianJi"), _T("F7-65"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSTiQingMianJi);													//�޸Ľ���������	//2018.2.1
	AddCommand(_T("FCS"), _T("FCSDistAnnotate"), _T("F7-66"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDistAnnotate);													//�޸Ľ���������	//2018.2.1
	AddCommand(_T("FCS"), _T("FCSCheckGongNengQu"), _T("F7-67"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckGongNengQu);											//�޸Ľ���������	//2018.8.20
	AddCommand(_T("FCS"), _T("FCSDefineGongNengQu"), _T("F7-68"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDefineGongNengQu);											//�޸Ľ���������	//2018.8.20
	AddCommand(_T("FCS"), _T("FCSNewCreaetFCPMT"), _T("FCSNewCreaetFCPMT"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewCreaetFCPMT);
	AddCommand(_T("FCS"), _T("FCSNewSetLayerInfo"), _T("FCSNewSetLayerInfo"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewSetLayerInfo);
	AddCommand(_T("FCS"), _T("FCSNewQualityCheck"), _T("FCSNewQualityCheck"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewQualityCheck);
	AddCommand(_T("FCS"), _T("FCSGuiHuaBuYiZhiDWG"), _T("FCSGuiHuaBuYiZhiDWG"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSGuiHuaBuYiZhiDWG);
	AddCommand(_T("FCS"), _T("FCSNewCheWeiStatistic"), _T("FCSNewCheWeiStatistic"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewCheWeiStatistic);
	AddCommand(_T("FCS"), _T("FCSNewSplitMargeLayer"), _T("FCSNewSplitMargeLayer"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewSplitMargeLayer);
	AddCommand(_T("FCS"), _T("FCSNewDrawUpPlanning"), _T("FCSNewDrawUpPlanning"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewDrawUpPlanning);
	AddCommand(_T("FCS"), _T("FCSNewOpenLayer"), _T("FCSNewOpenLayer"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewOpenLayer);
	AddCommand(_T("FCS"), _T("FCSNewCloseLayer"), _T("FCSNewCloseLayer"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewCloseLayer);
	AddCommand(_T("FCS"), _T("FCSNewOpenAllLayer"), _T("FCSNewOpenAllLayer"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewOpenAllLayer);
	AddCommand(_T("FCS"), _T("FCSNewReConfigurationDlg"), _T("FCSNewReConfigurationDlg"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSNewReConfigurationDlg);
	AddCommand(_T("FCS"), _T("FCSNewGHSTYSGSTBYZ"), _T("FCSNewGHSTYSGSTBYZ"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSNewGHSTYSGSTBYZ);
	AddCommand(_T("FCS"), _T("FCSDrawDomain"), _T("FCSDrawDomain"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSDrawDomain);
	AddCommand(_T("FCS"), _T("FCSHeightPropertySettingDlg"), _T("FCSHeightPropertySettingDlg"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSHeightPropertySettingDlg);

	//13076890385
	AddCommand(_T("FCS"), _T("FCSSetDLProjectCode"), _T("F8-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetDLProjectCode);
	AddCommand(_T("FCS"), _T("FCSExportControlPointAndRoadPicture"), _T("F8-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportControlPointAndRoadPicture);
	AddCommand(_T("FCS"), _T("FCSExportRoadStratificationPlan"), _T("F8-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportRoadStratificationPlan);
	AddCommand(_T("FCS"), _T("FCSExportRoadProfile"), _T("F8-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportRoadProfile);
	AddCommand(_T("FCS"), _T("FCSExportRoadCrossSectional"), _T("F8-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportRoadCrossSectional);
	AddCommand(_T("FCS"), _T("FCSExportRoadComprehensiveCrossSectional"), _T("F8-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportRoadComprehensiveCrossSectional);
	AddCommand(_T("FCS"), _T("OpenDwgCmd"), _T("OpenDwgCmd"), ACRX_CMD_TRANSPARENT | ACRX_CMD_REDRAW | ACRX_CMD_SESSION, OpenDwgCmd);

	//�ʼ�	
	AddCommand(_T("FCS"), _T("FCSCheckRedLineQuality"), _T("F10-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckRedLineQuality);
	AddCommand(_T("FCS"), _T("FCSSetWFProjectContext"), _T("f11-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetProjectContext);
	AddCommand(_T("FCS"), _T("FCSAnnotationType"), _T("f11-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAnnotationType);
	AddCommand(_T("FCS"), _T("FCSCopyFiles"), _T("f11-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCopyFiles);
	AddCommand(_T("FCS"), _T("FCSClearJPG"), _T("f11-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSClearJPG);
	AddCommand(_T("FCS"), _T("FCSExportMDB"), _T("F11-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportMDB);
	AddCommand(_T("FCS"), _T("FCSExportJPG"), _T("f11-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportJPG);
	AddCommand(_T("FCS"), _T("FCSChangePictureProperty"), _T("F11-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSChangePictureProperty);
	AddCommand(_T("FCS"), _T("FCSOpenProject"), _T("F11-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_SESSION, FCSOpenProject);
	AddCommand(_T("FCS"), _T("FCSCompressProject"), _T("F11-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCompressProject);
	AddCommand(_T("FCS"), _T("FCSOpenPath"), _T("F11-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOpenPath);
	AddCommand(_T("FCS"), _T("FCSProjectInfoPrompt"), _T("F11-10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSProjectInfoPrompt);
	AddCommand(_T("FCS"), _T("FCSOutputDiXingTu"), _T("F11-11"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOutputDiXingTu);
	AddCommand(_T("FCS"), _T("FCSSingleExportJPG"), _T("F11-12"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSingleExportJPG);
	AddCommand(_T("FCS"), _T("FCSExportFCPMJPG"), _T("F11-13"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportFCPMJPG);
	AddCommand(_T("FCS"), _T("FCSOverStepRedLine"), _T("f11-14"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSOverStepRedLine);
	AddCommand(_T("FCS"), _T("FCSTQOverStepRedLine"), _T("FCSTQOverStepRedLine"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSTQOverStepRedLine);						//�������
	AddCommand(_T("FCS"), _T("FCSJGSingleNodeAnnotation"), _T("f11-15"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSJGSingleNodeAnnotation);							//����������ע
	AddCommand(_T("FCS"), _T("FCSJGMulitNodeAnnotation"), _T("f11-16"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSJGMulitNodeAnnotation);								//����������ע
	AddCommand(_T("FCS"), _T("FCSRegen"), _T("f11-17"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRegen);
	AddCommand(_T("FCS"), _T("FCSCheckBuildLayerAndMDB"), _T("f11-18"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCheckBuildLayerAndMDB);

	AddCommand(_T("FCS"), _T("F12-0"), _T("F12-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_REDRAW | ACRX_CMD_INTERRUPTIBLE, FCSTestCommand);
	AddCommand(_T("FCS"), _T("FCSCombine"), _T("F12-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSCombine);									//����������ע
	AddCommand(_T("FCS"), _T("FCSExportPoint"), _T("F12-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportPoint);							//����������ע
	AddCommand(_T("FCS"), _T("FCSSetSubLine"), _T("F12-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetSubLine);								//����������ע
	AddCommand(_T("FCS"), _T("FCSAbjustText"), _T("F12-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAbjustText);
	AddCommand(_T("FCS"), _T("FCSAbjustArrow"), _T("F12-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAbjustArrow);
	AddCommand(_T("FCS"), _T("FCSModifyAnnoate"), _T("F12-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyAnnoate);

	AddCommand(_T("FCS"), _T("FCSChangePipeSetting"), _T("F13-0"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_SESSION, FCSSetGXProjectCode);		//�½�������Ŀ
	AddCommand(_T("FCS"), _T("FCSPIPELINEModifyProjectInfo"), _T("F13-1"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSPIPELINEModifyProjectInfo);		//�½�������Ŀ
	AddCommand(_T("FCS"), _T("FCSImportPipeLine"), _T("F13-2"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSImportPipeLine);								//�����������
	AddCommand(_T("FCS"), _T("FCSDrawPipeLine"), _T("F13-3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawPipeLine);									//���ƹ���	
	AddCommand(_T("FCS"), _T("FCSSetHandlePipeLine"), _T("F13-4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetHandlePipeLine);						//
	AddCommand(_T("FCS"), _T("FCSHeightPointText"), _T("F13-5"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSHeightPointText);							//���ñ�ע����ʽ����š��̡߳��߲
	AddCommand(_T("FCS"), _T("FCSRoadSE"), _T("F13-6"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRoadStartEnd);										//���õ�·���յ�
	AddCommand(_T("FCS"), _T("FCSEElevationalPoint"), _T("F13-7"), ACRX_CMD_TRANSPARENT | ACRX_CMD_REDRAW, FCSEElevationalPoint);
	AddCommand(_T("FCS"), _T("FCSModifyPiPeLineProperty"), _T("F13-8"), ACRX_CMD_TRANSPARENT | ACRX_CMD_REDRAW, FCSModifyPiPeLineProperty);
	AddCommand(_T("FCS"), _T("FCSExportPipeZPT"), _T("F13-9"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportPipeZPT);								//������ߵ���ͼ����
	AddCommand(_T("FCS"), _T("FCSFlushPipeLineProperty"), _T("F13-10"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSFlushPipeLineProperty);				//������ߵ���ͼ����
	AddCommand(_T("FCS"), _T("FCSMatchingPipePoint"), _T("F13-11"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSMatchingPipePoint);						//������ߵ���ͼ����
	AddCommand(_T("FCS"), _T("FCSChangePipeLineZJ"), _T("F13-12"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSChangePipeLineZJ);
	AddCommand(_T("FCS"), _T("FCSAddPipeLineRemark"), _T("F13-13"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddPipeLineRemark);
	AddCommand(_T("FCS"), _T("FCSExportPipeLineDwg"), _T("F13-14"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportPipeLineDwg);
	AddCommand(_T("FCS"), _T("FCSExportSingleReport"), _T("F13-15"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportSingleReport);
	AddCommand(_T("FCS"), _T("FCSModifyPipeLineZJXH"), _T("F13-16"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyPipeLineZJXH);					//�޸Ĺܵ�ע�����
	AddCommand(_T("FCS"), _T("FCSExportPipeLineZPT"), _T("F13-17"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportPipeLineZPT);					//�޸Ĺܵ�ע�����
	AddCommand(_T("FCS"), _T("FCSExportPipeLineDZT"), _T("F13-18"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportPipeLineDZT);					//�޸Ĺܵ�ע�����
	AddCommand(_T("FCS"), _T("FCSSetBridgeProperty"), _T("F13-19"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSSetBridgeProperty);					//�޸Ĺܵ�ע�����

	AddCommand(_T("FCS"), _T("FCSRedLineLoftingA3"), _T("R3"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRedLineLoftingA3);
	AddCommand(_T("FCS"), _T("FCSRedLineLoftingA4"), _T("R4"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRedLineLoftingA4);
	AddCommand(_T("FCS"), _T("FCSExportRoadZPT"), _T("FCSExportRoadZPT"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportRoadZPT);					//��·��ƽͼ���
	AddCommand(_T("FCS"), _T("FCSPictureAndKZD"), _T("FCSPictureAndKZD"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSPictureAndKZD);					//��·������Ƶ�ֲ�����·�����Ƭ����λ��ʾ��ͼ
	AddCommand(_T("FCS"), _T("FCSDrawHDM"), _T("FCSDrawHDM"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDrawHDM);										//���ƺ����
	AddCommand(_T("FCS"), _T("CorssSection"), _T("CorssSection"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, CorssSection);								//�����ͼ
	AddCommand(_T("FCS"), _T("VerSection"), _T("VerSection"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, VerSection);
	AddCommand(_T("FCS"), _T("AddXData"), _T("AddXData"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, AddXData);
	AddCommand(_T("FCS"), _T("FCSModifyRoadPro"), _T("FCSModifyRoadPro"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyRoadPro);
	AddCommand(_T("FCS"), _T("FCSSetSZProject"), _T("FCSSetSZProject"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_SESSION, FCSSetSZProject);	//�½�������Ŀ
	AddCommand(_T("FCS"), _T("FCSExportSZ"), _T("FCSExportSZ"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_SESSION, FCSExportSZReport);			//�����������
	AddCommand(_T("FCS"), _T("FCSRemarkPipePoint"), _T("FCSRemarkPipePoint"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSRemarkPipePoint);				//�����������
	AddCommand(_T("FCS"), _T("FCSDimPipelineFlow"), _T("FCSDimPipelineFlow"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSDimPipelineFlow);				//���߱�ע
	AddCommand(_T("FCS"), _T("FCSAddPrintTK"), _T("FCSAddPrintTK"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSAddPrintTK);								//��Ӵ�ӡͼ��
	AddCommand(_T("FCS"), _T("FCSExportRoadJSSM"), _T("FCSExportRoadJSSM"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSExportRoadJSSM);					//���߱�ע
	AddCommand(_T("FCS"), _T("FCSModifyRoadHDMInfo"), _T("FCSModifyRoadHDMInfo"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyRoadHDMInfo);		//���õ�·�����ͼ��

	AddCommand(_T("FCS"), _T("FCSQinagHou"), _T("FCSQinagHou"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSQinagHou);		//���õ�·�����ͼ��
	AddCommand(_T("FCS"), _T("FCSModifyProjectInfo"), _T("FCSModifyProjectInfo"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyProjectInfo);		//���õ�·�����ͼ��
	AddCommand(_T("FCS"), _T("FCSModifyProjectRangeLineInfo"), _T("FCSModifyProjectRangeLineInfo"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSModifyProjectRangeLineInfo);
	AddCommand(_T("FCS"), _T("FCSEntityToWhite"), _T("FCSEntityToWhite"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSEntityToWhite);		// ��ͼ��ɫ���
	AddCommand(_T("FCS"), _T("FCSCreateMidLine"), _T("FCSCreateMidLine"), ACRX_CMD_USEPICKSET | ACRX_CMD_TRANSPARENT, FCSCreateMidLine);		// ����ǽ����

	//AddCommand(_T("FCS"), _T("FCS"), _T("FCSModifyRoadHDMInfo"), ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET, FCSModifyRoadHDMInfo);		//���õ�·�����ͼ��
	 IDataBaseOper oper;	oper.readFieldChangeTable(); InitParaMeter();// FCSNewReConfigurationDlg();
	//FCSProjectInfoPrompt(); //Text encryption	// TODO: add your initialization functions
	 FCSTestCommand();
}

// Unload this application. Unregister all objects
// registered in InitApplication.
void UnloadApplication()
{
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_EXIT
	acedRegCmds->removeGroup(_T("FCS"));
	//}}AFX_ARX_EXIT

	// TODO: clean up your application
	ReleaseSense4Context(); //������
	if(g_selectentity) delete g_selectentity;
	g_selectentity = NULL;
	if(g_extentBuildingDlg != NULL)
	{
		delete g_extentBuildingDlg;
		g_extentBuildingDlg = NULL;
	}
	XMJG2ExportDlg::destory();

#ifdef CODEMETER
	ReleaseCMStick();
#endif
}

void AddCommand(const TCHAR* cmdGroup, const TCHAR* cmdInt, const TCHAR* cmdLoc,
	const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal)
{
	TCHAR cmdLocRes[65];
	if(idLocal != -1)
	{
		::LoadString(_hdllInstance, idLocal, cmdLocRes, 64);
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLocRes, cmdFlags, cmdProc);
	}
	else
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLoc, cmdFlags, cmdProc);
}