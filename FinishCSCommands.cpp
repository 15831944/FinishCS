//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "Global.h"
#include "Dialogs.h"
#include "Tukuang.h"
#include "wordRpt.h"
#include "carinfo.h"
#include "apoint.hpp"
#include "excelRpt.h"
#include "XMJGTool.h"
#include "SaveBlocks.h"
#include "Encryption.h"
#include "XMJGDialog.h"
#include "SBreakBlock.h"
#include "EntitysCheck.h"
#include "XMJGReadData.h"
#include "AttributeDef.h"
#include "ExportMdbInfo.h"
#include "ContructsOperate.h"
#include "Selection_String.h"
#include "setentitypropery.h"
#include "LichengFromZdline.h"
#include "PipeSection.h"
#include "epsdef.h"
#include "XMJGMunicipal.h"
#include "FilePathOperater.h"
#include "C3DOperator.h"
#include "XMJGReConfigurationDlg.h"
#include "InfomationOfProjectRangeLine.h"
#include "MoveJig.h"
#include "heightpropertysettingdlg.h"

CProjectInfoDockbar g_projectpromptdock;
extern FieldChangeTable g_fieldchagetable;
AcApDocumentArray g_projectappdocs;
XMJGReConfigurationDockbar g_ReConfigurationDockBar;
XMJGDrawDomainDockControl g_DrawDomainDockControl;

/*MessageBox(GetForegroundWindow(), _T("�����Ƿ���빷!!!"), _T("������Ϣ"), MB_OK);\
return;\*/

#define SEEKDOG()\
	if(seekdog() == false)\
								{\
		MessageBox(GetForegroundWindow(), _T("�����Ƿ���빷!!!"), _T("������Ϣ"), MB_OK);\
		return;\
								}

void FCSCheckMustFillFields()
{
	if(seekdog() == false) return;
	CContructsOperate contructsOpe;
	contructsOpe.checkMustFillFields();
}

void FCSCheckResultWizard()
{
	if(seekdog() == false) return;
	CAcModuleResourceOverride myResources;
	CResultsCheckWizardDlg checkWizardDlg;
	checkWizardDlg.DoModal();
}

/*! \brief	�������ܵ�cass9��putp������죬�ڵ�����putp����󣬵������Ա���Ի������û���д��Ϣ
 *  \param
 *  \author		�춫��
 *  \date		[2016��04��25�� 10:41]
 */
void FCSPutP_FCS()
{
	if(seekdog() == false) return;
	CContructsOperate constantT;
	constantT.putP();
}

/*! \brief		�������ܵ�cass9��dzpb������죬�ڵ�����dzpb����󣬵������Ա���Ի������û���д��Ϣ
 *  \param
 *  \author		�춫��
 *  \date		[2016��04��25�� 10:42]
 */
void FCSDZPB_FCS()
{
	if(seekdog() == false) return;
	CContructsOperate constantT;
	constantT.dzpb();
}

void FCSCheckUnCalulatedBuildings()
{
	if(seekdog() == false) return;
	CContructsOperate constantT;
	constantT.checkUnCalulatedBuildings();
}

void FCSRefreshFrameData()
{
	if(seekdog() == false) return;
	CTukuang tukuan;
	tukuan.refreshFrameData();
}

void FCSCheckArea()
{
	if(seekdog() == false) return;
	CExportSummaryTable excelRpt;
	excelRpt.checkArea();
}

void FCSExtractText()
{
	if(seekdog() == false) return;
	CContructsOperate constructsOpe;
	constructsOpe.extractText();
}

void FCSExtractToOrignDoc()
{
	if(seekdog() == false) return;
	CExportSurveyTable exportToWord;
	exportToWord.extractToOrignDoc();
}

void FCSScaleDimensionText()
{
	if(seekdog() == false) return;
	CContructsOperate contructsOpe;
	contructsOpe.scaleDimensionText();
}

void FCSDrawThings()
{
	if(seekdog() == false) return;
	CContructsOperate contructsOpe;
	contructsOpe.drawThings();
}

/*! \brief	��ȡexcel�������ݲ���å��ͼ��
 *  \param
 *  \author		�춫��
 *  \date		[2015��05��28�� 09:36]
 */
void FCSNoteCoord_Total_Floor_Plan()
{

	if(seekdog() == false) return;
	CContructsOperate contructsOpe;
	contructsOpe.noteCoord_Total_Floor_Plan();
}

void FCSCheckElevationPoint()
{
	SEEKDOG();
	CEntitysCheck entitysChk;
	entitysChk.checkElevationPointAttribute();
}

void FCSExtentBuilding()
{
	SEEKDOG();
	CContructsOperate contructsOpe;
	contructsOpe.extentBuildings();
}

void FCSFCSTest()
{
	CTinyExcel			m_excel;
	m_excel.SetTemplete(_T("D:\\34.xlsx"));

	if(m_excel.StartExcel() == false) return;
	m_excel.Show();

	vector<int> vInt;
	m_excel.MergeSameStringNeightbourCell2(4, 1, 28, vInt, m_excel.m_range);		//ͬһ��
	for(int i = 0; i < (int)vInt.size(); i++)
		ads_printf(_T(" ---%d"), vInt.at(i));
	vInt.clear();
	m_excel.MergeSameStringNeightbourCell2(1, 1, 28, vInt, m_excel.m_range, true);//ͬһ��
	for(int i = 0; i < (int)vInt.size(); i++)
		ads_printf(_T(" +++%d"), vInt.at(i));
	return;
}

void FCSFCSRegister()
{
	TCHAR szCode[50] = _T("");
	if(ads_getstring(0, _T("\n������CASS�����������������:"), szCode) != RTNORM) return;
	if(0 == _tcscmp(szCode, _T(""))) return;
	bool bResult = Cass_Register(szCode, 52);
	if(true == bResult) ads_printf(_T("\n�ɹ�ע��!����������ϵͳ"));
	else ads_printf(_T("\n���벻��ȷ"));
}

void FCSExportGroundAttachmentsTable_Standard2()
{
	if(seekdog() == false) return;
	CExportMdbInfo exportInfo;
	exportInfo.exportGroundAttachmentsTable_Standard2();
}

void FCSExportGroundAttachmentsTable()
{
	if(seekdog() == false) return;
	CExportSummaryTable exportTable;
	exportTable.m_sReportType = IndoorReport_GroundAttachmentsTable;
	exportTable.exportGroundAttachmentsTable();
}

void FCSOwnerInfoTable()
{
	if(seekdog() == false) return;
	CExportSummaryTable exportTable;
	exportTable.m_sReportType = IndoorReport_OwnerInfoTable;
	exportTable.exportOwnerInfoTable();
}

void FCSOwnerInfoSummaryTable()
{
	if(seekdog() == false) return;
	CExportSummaryTable exportTable;
	exportTable.m_sReportType = IndoorReport_OwnerInfoSummaryTable;
	exportTable.exportOwnerInfoSummaryTable();
}

void FCSOwnerInfoSummaryTable_Standard2()
{
	if(seekdog() == false) return;
	CExportMdbInfo exportInfo;
	exportInfo.exportOwnerInfoSummaryTable_Standard2();
}

void FCSExtractFeatureTable()
{
	if(seekdog() == false) return;
	CExportSummaryTable exportSummary;
	exportSummary.extractFeatureTable();
}

void FCSExportSummaryTable()
{
	if(seekdog() == false) return;
	CExportSummaryTable exportSummary;
	exportSummary.exportSummaryTable();
}

void FCSExportSurveyResultReport()
{
	if(seekdog() == false) return;
	CExportSurveyTable exportReport;
	exportReport.exportSurveyResultReport();
}

void FCSStaticFloorInfo()
{
	if(seekdog() == false) return;
	CAcModuleResourceOverride myResources;
	CFloorInfoDlg *floorDlg = new CFloorInfoDlg();
	if(floorDlg->GetSafeHwnd() == NULL)
	{
		floorDlg->Create(IDD_DIALOG_FLOORINFO);
		floorDlg->ShowWindow(SW_SHOW);
		floorDlg->CenterWindow();
	}
}

void FCSInsertFrame_FloorPlane()
{
	if(seekdog() == false) return;
	CTukuang tukuan;
	tukuan.startFloorPlane();
}

void FCSInsertFrame_FloorPlane_Multy()
{
	if(seekdog() == false) return;
	CTukuang tukuan;
	tukuan.startFloorPlane_Multy();
}

void FCSProjectInfoDlg()
{
	if(seekdog() == false) return;
	CAcModuleResourceOverride myResources;

	CProjectInfoDlg projDlg;
	projDlg.DoModal();
}

void FCSInsertFrame_Total_Floor_Plan()
{
	if(seekdog() == false) return;
	CTukuang tukuan;
	tukuan.startTotal_Floor_Plan();

}

void FCSNoteIndoorsHeight()
{
	if(seekdog() == false) return;
	CContructsOperate contructsO;
	contructsO.noteIndoorsHeight();
}

void FCSInsertBaseMap()
{
	if(seekdog() == false) return;

	CContructsOperate contructsO;
	contructsO.insertAndChangeBaseMaps();

}

//��Ӹ�����ע�ǣ������ݲ㣬Χǽ�����֣����ע����ĸ�����ע�ǣ�����̨��̨�׵ȣ�
void FCSAddAttachmentNote()
{
	if(seekdog() == false) return;

	CContructsOperate contructsO;
	contructsO.noteAttachment();
}

//���Χǽע��
void FCSAddFenceNote()
{
	if(seekdog() == false) return;

	CContructsOperate contructsO;
	contructsO.noteFenceNote();
}

//��Ӹ���������ע��
void FCSAddTextNote()
{
	if(seekdog() == false) return;
	CContructsOperate contructsO;
	contructsO.noteTextNote();
}

//��Ӹ���������ע��_��׼2
void FCSAddTextNote2()
{
	if(seekdog() == false) return;
	CContructsOperate contructsO;
	contructsO.noteTextNote2();
}

//�����뻧ͼ��
void FCSInertHouseHoldFrame()
{
	if(seekdog() == false) return;

	CTukuang tukuan;
	tukuan.startIndoorFrame();
}

//��ӷ���ע�ǣ����ע��
void FCSAddHourseNote()
{
	if(seekdog() == false) return;

	CContructsOperate contructsO;
	contructsO.noteIndoorFrame();
}

void FCSDrawdeepbuilding()
{
	if(seekdog() == false) return;

	TCHAR szCode[200] = {0};
	if(ads_getstring(NULL, NULL, szCode) != RTNORM) return;

	resbuf *cmd, *result;
	ads_name ent;
	struct resbuf rb;
	ads_getvar(_T("osmode"), &rb);
	int oldpick = rb.resval.rint;
	cmd = ads_buildlist(RTSTR, _T("drawdjf"), RTSTR, szCode, RTSHORT, oldpick, 0);
	ads_invoke(cmd, &result);
	ads_relrb(cmd);
	if(result == NULL) return;
	if(RTENAME != result->restype) return;
	ads_name_set(result->resval.rlname, ent);
	ads_relrb(result);



	double dDeep = 0.0;
	ads_getreal(_T("\n ���������"), &dDeep);

	AddXdataDouble(ent, _T("CG"), dDeep);

}

// This is command 'TRIMPOLY'
void ArxTrimPoly()
{
	// TODO: Implement the command

}

#pragma region ���ſ���ҵ��һ

void FCSSetRLProjectCode()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.CreateRLProject();
}

void FCSImportData()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.ImportData();
}

void FCSRLSetPropery()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.setProperty();
}

void FCSReadExcel()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.ReadExcelFile();
}

void FCSManualDrawPoint()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.ManualDrawPoint();
}

void FCSDrawLine()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.DrawRedLine();
}

void FCSAddRedLineNode()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.AddRedLineNode();
}

void FCSDelRedLineNode()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.DelRedLineNode();
}

void FCSAdjustNode()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.AdjustNodesOrder();
}

void FCSAddAnnotation()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.AddAnnotation();
}

void FCSAutoAnnotation()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.AutoAnnotation();
}

void FCSManualAnnotation()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.ManualAnnotation();
}

void FCSMoveAnnotation()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.MoveAnnotation();
}

void FCSManualLineAnnotation()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.ManualLineAnnotation();
}

void FCSRedlineSetAttatchedInfo()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.RedlineSetAttatchedInfo();
}

void FCSRedLineExportDWG()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.RedLineExportDWG();
}

void FCSDrawScopeLine()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.DrawScopeLine();
}

void FCSReadReveseDat()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.ReadReveseDat();
}

void FCSRLComprassFile()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg(_T("���߷���"));
	dlg.DoModal();
}

void FCSInputCoorPos()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.InputCoorPos();
}

void FCSRedLineAddNode()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.readLineAddNode();
}

void FCSRedDistAnnotion()
{
	SEEKDOG(); XMJG(XMJGReadData);
	XMJGReadData.redDistAnnotion();
}

#pragma endregion

#pragma region ���ſ���ҵ���

void FCSSetJZProjectCode()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.CreateBOSProject();
}

void FCSAnnotationType()
{
	SEEKDOG(); XMJGAssist assist;
	assist.SetAnnotationType();
}

bool exportEntityInfoToMDB(const AcDbObjectId &id, const VMStr &datas, const ATTRINFO_DG *att, CString &strSQL)
{
	bool isSuccuss = true; CString strFields, strValue;
	CString strTemp; TCHAR szValue[255] = {0};
	TCHAR szXdataName[255] = {0}; strSQL = _T("");
	for(int m = 0; m < (int)att->AttrArr.size(); m++)
	{
		_stprintf(szXdataName, _T("%s"), att->AttrArr.at(m).name);
		strFields += szXdataName;
		int type = att->AttrArr.at(m).type;
		bool bMust = att->AttrArr.at(m).bMust;
		ReadXdata(aname(id), szXdataName, type % 10, szValue);
		if(bMust == true && _tcslen(szValue) == 0)
		{
			acutPrintf(_T("\n �ֶ�[%s]Ϊ�գ��������������"), att->AttrArr.at(m).text);
			isSuccuss = false;
		}

		if(type % 10 == 0)
			strTemp.Format(_T("'%s'"), szValue);
		else if(type % 10 == 1 || type % 10 == 2)
		{
			if(_tcslen(szValue) == 0)
				_stprintf(szValue, _T("%s"), _T("0"));
			strTemp.Format(_T("'%s'"), szValue);
		}
		else if(type % 10 == 3)
		{
			double area = _tstof(szValue);
			strTemp.Format(_T("'%.2lf'"), area);
		}
		if(_tcscmp(szXdataName, _T("YWLX")) == 0)
		{
			for(int indx = 0; indx < (int)datas.size(); ++indx)
			{
				MStr data = datas[indx];
				MSIter it = data.begin();
				if(it->second.CompareNoCase(szValue) != 0) continue;
				strTemp = (++it)->second;
				break;
			}
		}
		strValue += strTemp;
		if(m < (int)att->AttrArr.size() - 1)
		{
			strFields += _T(", ");
			strValue += _T(", ");
		}
	}
	strSQL.Format(_T("(%s) values (%s)"), strFields, strValue);
	return isSuccuss;
}

void FCSExportMDB()
{
	TCHAR pathmdb[255] = {0};
	if(RTNORM != ads_findfile(_T("XMFW.mdb"), pathmdb))
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ������ļ�[XMFW.mdb]"), _T("������Ϣ"), MB_OK);
		return;
	}
	IDataBaseOper oper;
	CString path = oper.readPathTable(_T("��ǰĿ¼"));
	CString name = oper.readFileNameTable(_T("��Ŀ������Ϣ"));
	int pos = path.ReverseFind(_T('\\'));
	pos = (pos == -1) ? path.ReverseFind(_T('/')) : pos;
	if(pos == -1) return; path = path.Mid(0, pos) + name;
	CopyFile(pathmdb, path, FALSE);
	SelectFilter sf(8, _T("XMFW"));	AcDbObjectId id;
	if(false == SelectEntity(id, sf, _T(""))) return;
	CAdoConnection _connect_; if(_taccess(path, 0) != 0) return;
	if(FALSE == _connect_.ConnectAccess(path)) return;
	CAdoRecordSet CARS;	CARS.SetAdoConnection(&_connect_);
	AttributeDef attribute; attribute.initAttributeDef();
	TCHAR szValue[3000] = {0}; TCHAR szXdataName[20] = {0};
	CString str, strSQL, strFields, strTemp, strValue;
	TCHAR lititem[255] = {0};
	if(RTNORM != ads_findfile(_T("lstitems.mdb"), lititem))
	{
		AfxMessageBox(_T("�޷��ҵ�����[lstitems.mdb]�ļ�"));
		return;
	}
	CDatabaseOperManager manager; manager.initialConnect(lititem);
	VMStr datas; VStr tables1, tables2; tables1.push_back(_T("XMFW"));
	tables1.push_back(_T("SQR")); tables2.push_back(_T("XMJBXX"));
	tables2.push_back(_T("SQR"));
	manager.ReadDataBase_all(_T("TBL_YWLX"), datas);
	AddXdata(aname(id), _T("XMFW_BSM"), 2, _T("1"));
	AddXdata(aname(id), _T("XM_BSM"), 2, _T("1"));
	AddXdata(aname(id), _T("SQR_BSM"), 2, _T("2"));

	for(int indx = 0; indx < (int)tables1.size(); ++indx)
	{
		CString table1 = tables1[indx];
		CString table2 = tables2[indx];
		for(int idx = 0; idx < (int)attribute.m_pattrinfo_array.size(); ++idx)
		{
			ATTRINFO_DG *att = attribute.m_pattrinfo_array.at(idx);
			if(_tcscmp(att->table, table1) != 0) continue;
			strSQL.Format(_T("insert into %s "), table2);
			if(!exportEntityInfoToMDB(id, datas, att, strTemp)) return;
			strTemp.Replace(table1 + _T("_"), _T(""));
			strSQL += strTemp; _connect_.Execute(strSQL); break;
		}
		// 		bool isSuccuss = true;
		// 		for(int m = 0; m < (int)att->AttrArr.size(); m++)
		// 		{
		// 			_stprintf(szXdataName, _T("%s"), att->AttrArr.at(m).name);
		// 			strFields += szXdataName;
		// 			int type = att->AttrArr.at(m).type;
		// 			bool bMust = att->AttrArr.at(m).bMust;
		// 			ReadXdata(aname(id), szXdataName, type, szValue);
		// 			if(bMust == true && _tcslen(szValue) == 0)
		// 			{
		// 				acutPrintf(_T("\n �ֶ�[%s]Ϊ�գ��������������"), szXdataName);
		// 				isSuccuss = false;
		// 			}
		// 
		// 			if(type % 10 == 0)
		// 				strTemp.Format(_T("'%s'"), szValue);
		// 			else if(type % 10 == 1 || type % 10 == 2)
		// 			{
		// 				if(_tcslen(szValue) == 0)
		// 					_stprintf(szValue, _T("%s"), _T("0"));
		// 				strTemp.Format(_T("%s"), szValue);
		// 			}
		// 			else if(type % 10 == 3)
		// 			{
		// 				double area = _tstof(szValue);
		// 				strTemp.Format(_T("'%.2lf'"), area);
		// 			}
		// 			if(_tcscmp(szXdataName, _T("YWLX")) == 0)
		// 			{
		// 				for(int indx = 0; indx < (int)datas.size(); ++indx)
		// 				{
		// 					MStr data = datas[indx];
		// 					MSIter it = data.begin();
		// 					if(it->second.CompareNoCase(szValue) != 0) continue;
		// 					strTemp = (++it)->second;
		// 					break;
		// 				}
		// 				strValue += _T("'") + strTemp + _T("'");
		// 			}
		// 			else strValue += strTemp;
		// 			if(m < (int)att->AttrArr.size() - 1)
		// 			{
		// 				strFields += _T(", ");
		// 				strValue += _T(", ");
		// 			}
		// 		}
		// 		str.Format(_T("(%s) values (%s)"), strFields, strValue);
	}
	_connect_.Close();
	acutPrintf(_T("\n ���[��Ŀ������Ϣ.mdb���]"));
}

void FCSExportJPG()
{
	IDataBaseOper oper;
	CString dir = oper.readPathTable(_T("��ǰĿ¼"));
	AcDbObjectIdArray IdArr;
	if(false == SelectEntitys(IdArr, _T(""))) return;
	CString filter(_T("ͼƬ��ʽ(*.jpg)|*.jpg||"));
	CFileDialog *dlg = new CFileDialog(FALSE, 0, 0, 6, filter);
	dlg->m_ofn.lpstrInitialDir = dir;
	if(IDOK != dlg->DoModal())	return;
	XMJGExport exportdwg;
	CString savename = dlg->GetPathName();
	exportdwg.setSavePath(savename);
	exportdwg.setObjects(IdArr);
	exportdwg.ExportDwg2JPG(true);
	int count = oper.getFieldCountByTable(PICTURETABLE);
	oper.addPictureTableInfo(savename);
}

// bool isExistFolder(const CString &dir)
// {
// 	if(-1 != _taccess(dir, 0)) return true;
// 	CString path;
// 	int pos = dir.ReverseFind(_T('\\'));
// 	path = dir.Mid(0, pos);
// 	if(false == isExistFolder(path))
// 		_tmkdir(path);
// 	return false;
// }

void FCSSingleExportJPG()
{
	CString savePath = GetCurrentDwgFullPath();
	int pos = savePath.ReverseFind(_T('\\'));
	pos = (pos == -1) ? savePath.ReverseFind(_T('/')) : pos;
	CString path = savePath.Mid(0, pos + 1);
	CString name = savePath.Mid(pos);
	IDataBaseOper oper; pos = name.Find(_T(".dwg"));
	if(pos == -1) return; name = name.Mid(0, pos);
	pos = path.Find(_T("\\CG"));
	if(pos != -1)
		path = path.Mid(0, pos) + _T("\\CK");
	pos = path.Find(_T("\\GC"));
	if(pos != -1)
		path = path.Mid(0, pos) + _T("\\CK");
	isExistFolder(path);
	savePath = path + name;
	CmdEcho cmdEcho;
	convertDwg2WMF(savePath);
}

void FCSExportFCPMJPG()
{
	IDataBaseOper oper; AcDbObjectIdArray IdArr; XMJGAssist assist;
	CString savename = oper.readPathTable("��ǰĿ¼");
	CString name = GetCurrentDwgName(); IProjectMDB pdb;
	int pos = name.ReverseFind(_T('.'));
	name = name.Mid(0, pos); AcDbObjectIdArray del;
	savename += _T("\\CK\\"); savename += name;
	SelectFilter sf(8, _T("��ӡ��Χ"));
	if(false == SelectEntitys(IdArr, sf, _T("X"))) return;
	XMJGExport exportdwg; XMJGPrintPicture picture;
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	SNodesOpe nodesO; ads_point pmin, pmax;
	for(int idx = 0; idx < IdArr.length(); ++idx)
	{
		name.Format(_T("%s%d"), savename, idx);
		AcDbObjectId id = IdArr.at(idx);
		AcDbObjectIdArray outs; AcGePoint2dArray node;
		GetPlList(aname(id), node);
		GetEntExtent(aname(id), pmin, pmax);
		nodesO.modifyLB_RTPoint(pmin, pmax);
		ssFromNodes(outs, node);
		ZoomWindow(pmin, pmax);
		picture.setObjectArray(outs);
		picture.setPrintSizeName();
		picture.setPrintFormat(Raster_Jpg);
		picture.setPrintPath(name);
		if(false == picture.BeginPrintPicture()) break;
		convertDwg2Pdf(name);
	}
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
}

void FCSOverStepRedLine()
{
	IDataBaseOper oper; TextProperty m_jzwlk; AcDbObjectId mid, hxid;
	oper.readCommonTable(_T("������������������ͼ��"), m_jzwlk._layer);
	SelectFilter sf1(RTDXF0, _T("*polyline")), sf2(8, m_jzwlk._layer);
	acutPrintf(_T("\n��ѡ������������"));
	if(false == SelectEntity(mid, sf1, sf2, _T(""))) return;
	SelectFilter sf3(8, _T("����")); AcDbObjectIdArray tmps;
	acutPrintf(_T("\n��ѡ����ߣ�"));
	if(false == SelectEntity(hxid, sf1, sf3, _T(""))) return;
	AcDbObjectId id1 = C3DOperator::CreateRingRegion(mid, tmps);
	AcDbObjectId id2 = C3DOperator::CreateRingRegion(hxid, tmps);
	if(!C3DOperator::RegionBooleanOper(id1, id2, AcDb::kBoolSubtract)) return;
	ads_name ent, last; acdbGetAdsName(ent, id1); ads_entlast(last);
	ads_command(RTSTR, _T("HATCH"), RTSTR, _T("S"), RTENAME, ent, RTSTR, _T(""), RTNONE);
	ads_name next; ads_entlast(next); if(ads_name_equal(next, last)) return;
	TextProperty prop = oper.readTextPropery(_T("�����������߷�Χ"));
	TCHAR layer[255] = {0}; _stprintf(layer, _T("%s"), prop._layer);
	setlayer(next, prop._layer); setcolor(next, prop._color);
	ads_command(RTSTR, _T("draworder"), RTENAME, next, RTSTR, _T(""), RTSTR, _T("F"), RTNONE);
}

void FCSTQOverStepRedLine()
{
	IDataBaseOper oper; TextProperty m_jzwlk; AcDbObjectId mid, hxid;
	oper.readCommonTable(_T("���ſ����ű߽�ͼ��"), m_jzwlk._layer);
	SelectFilter sf1(RTDXF0, _T("*polyline")), sf2(8, m_jzwlk._layer);
	acutPrintf(_T("\n��ѡ������������"));
	if(false == SelectEntity(mid, sf1, sf2, _T(""))) return;
	SelectFilter sf3(8, _T("����")); AcDbObjectIdArray tmps;
	acutPrintf(_T("\n��ѡ����ߣ�"));
	if(false == SelectEntity(hxid, sf1, sf3, _T(""))) return;
	AcDbObjectId id1 = C3DOperator::CreateRingRegion(mid, tmps);
	AcDbObjectId id2 = C3DOperator::CreateRingRegion(hxid, tmps);
	if(!C3DOperator::RegionBooleanOper(id1, id2, AcDb::kBoolSubtract)) return;
	ads_name ent, last; acdbGetAdsName(ent, id1); ads_entlast(last);
	ads_command(RTSTR, _T("HATCH"), RTSTR, _T("S"), RTENAME, ent, RTSTR, _T(""), RTNONE);
	ads_name next; ads_entlast(next); if(ads_name_equal(next, last)) return;
	TextProperty prop = oper.readTextPropery(_T("�����������߷�Χ"));
	TCHAR layer[255] = {0}; _stprintf(layer, _T("%s"), prop._layer);
	setlayer(next, prop._layer); setcolor(next, prop._color);
	ads_command(RTSTR, _T("draworder"), RTENAME, next, RTSTR, _T(""), RTSTR, _T("F"), RTNONE);
}

void FCSJGSingleNodeAnnotation()
{
	IDataBaseOper oper; TextProperty prop, prop1;
	prop = oper.readTextPropery(_T("�������������ע"), true);
	prop1 = oper.readTextPropery(_T("�������������ע��ͷ"), true);
	singleNodeAnnotation(prop, prop1);
}

void FCSJGMulitNodeAnnotation()
{
	acutPrintf(_T("\n ��ѡ����Ҫע�ǵ�ʵ�壺"));
	SelectFilter f1(RTDXF0, _T("*polyline"));
	AcDbObjectIdArray idArr; IDataBaseOper oper; TextProperty prop;
	if(!SelectEntitys(idArr, f1, _T(""))) return;
	prop = oper.readTextPropery(_T("�������������ע"), true);
	mulitNodeAnnotation(idArr, prop);
}

void FCSCombine()
{
	acedCommand(RTSTR, _T(".circle"), RTSTR, _T("5.5"), RTSTR, PAUSE, RTSTR, _T("line"), RTSTR, _T("5.5"), RTSTR, _T("5.5"), RTSTR, _T(""), 0);
	acutPrintf(_T("\n ��ѡ����Ҫ�ϲ�����:")); ads_name ssName;
	if(RTNONE != acedSSGet(_T(""), NULL, NULL, NULL, ssName)) return;
	ads_command(RTSTR, _T("_PEDIT"), RTSTR, PAUSE, RTNONE);
}

void FCSExportPoint()
{
	AcDbObjectId id; AcGePoint2dArray node;
	if(false == SelectEntity(id, _T(""))) return;
	GetPlList(aname(id), node);

}

void FCSSetSubLine()
{
	IDataBaseOper oper; int type = 1; CString info;
	if(false == oper.readCommonTable(_T("�������Ƿ�ɾ��"), type)) return;
	info.Format(_T("\n �Ƿ�ɾ��������[1]������[2]ɾ����<%d>"), 3 - type);
	if(RTCAN == ads_getint(info, &type)) return;
	if(type == 1)
		acutPrintf(_T("\n �ɹ����ñ���������, �´ε���ع�֮�󣬸����߽�����ɾ����"));
	else
		acutPrintf(_T("\n �ɹ�����ɾ��������, �´ε���ع�֮�󣬸����߽��Զ�ɾ����"));
	oper.writeCommonTable(_T("�������Ƿ�ɾ��"), type);
}

void FCSAbjustText()
{
	AcDbObjectIdArray ids; double height = 2.0;
	if(RTCAN == ads_getreal(_T("\n �������ı��ĸ߶ȣ�<2.0>"), &height)) return;
	if(false == SelectEntitys(ids, _T(""))) return;
	modifyEntsTextHeight(ids, height);
}

void FCSAbjustArrow()
{
	AcDbObjectIdArray ids; double height = 2.0;
	if(RTCAN == ads_getreal(_T("\n �������ͷ�Ĵ�С��<2.0>"), &height)) return;
	if(false == SelectEntitys(ids, _T(""))) return;
	modifyEntsArrowSize(ids, height);
}

void FCSModifyAnnoate()
{
	CXRecordOperate oper; double scale = 1.0;
	if(FALSE == oper.ReadDoubleRecord(_T("GCCL"), _T("BLC"), scale))
	{
		MessageBox(GetForegroundWindow(), _T("��������ͼ������ִ�д������޸ĳ�ͼ�����ߡ�"), _T("������Ϣ"), 0);
		return;
	}

	AcDbObjectIdArray ids; int Scale = (int)(scale * 1000);
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	if(false == SelectEntitys(ids, _T("X"))) return;
	TCHAR info[255] = {0};
	_stprintf(info, _T("\n �������µı�����<1:%d> 1:"), Scale);
	if(RTNORM != ads_getint(info, &Scale)) return;
	scale = Scale * 1.0 / 1000;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		GetEntLayer(id, info);
		if(_tcscmp(info, _T("TK")) == 0)
		{
			_stprintf(info, _T("������1:%d"), Scale);
			changeTextByXData(id, _T("SCALE"), _T("SCALE"), info);
			continue;
		}
		modifyEntTextHeight(id, scale * 1.679);
		modifyEntArrowSize(id, scale);
		modifyEntCircleSize(id, scale);
	}
	oper.AddDoubleRecord(_T("GCCL"), _T("BLC"), scale);
	return;

	double height = 2.0; double size = 2.0;
	if(RTCAN == ads_getreal(_T("\n �������ͷ�Ĵ�С��<2.0>"), &size)) return;
	if(RTCAN == ads_getreal(_T("\n �������ı��ĸ߶ȣ�<2.0>"), &height)) return;
	if(false == SelectEntitys(ids, _T(""))) return;
	acedSetStatusBarProgressMeter(_T("��������ͼ��..."), 0, ids.length());
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = ids.at(idx);
		modifyEntArrowSize(id, size);
		modifyEntTextHeight(id, height);
	}
	acedRestoreStatusBar();
}

void FCSCheckBuildLayerAndMDB()
{
	CString dwgName = GetCurrentDwgName();
	CString subName = dwgName.Right(9);
	if(subName.CompareNoCase(_T("�ֲ�ƽ��ͼ.dwg")) != 0)
	{
		MessageBox(0, _T("����¥��ƽ��ͼ���м��"), _T("������Ϣ"), 0);
		return;
	}
	XMJGHouse house;
	house.checkBuildLayerAndMDB();
}

void FCSImportPipeLine()
{
	CXMJGPipeLine pl;
	pl.importPipeLine();
	/*acutPrintf(_T("\n��ѡ��ܵ��Excel��"));
	XMJGAssist assist;
	CString filter(_T("�����ļ�(*.*) | *.*; | TXT(*.txt) | *.txt | DAT(*.dat) | *.dat | Excel(*.csv; *.xls; *.xlsx;) | *.csv; *xls; *xlsx ||"));
	CString dir, szPathName;
	if(false == assist.SelectFile(filter, dir, szPathName)) return;
	VPInfo ptinfo; VStr error;
	int reverse = 0;
	assist.PipeExcel(szPathName, ptinfo, error, reverse);
	for(int i = 0; i < error.size(); ++i)
	acutPrintf(_T("\n%s�������ݲ�����"), error[i]);
	IDataBaseOper prjmdb;
	for(int i = 1; i < ptinfo.size(); ++i)
	{
	ads_point pt, ptText;
	CString str, strGdlx, strLayer, strGDDM;
	CPointInfo temp = ptinfo[i];
	pt[X] = temp.m_pt3.x; pt[Y] = temp.m_pt3.y; pt[Z] = temp.m_pt3.z;
	TCHAR buf[260] = {0};
	_stprintf(buf, _T("%s"), temp.m_Name);
	for(size_t j = 0; j < temp.m_Name.GetLength(); ++j)
	{
	if(!isalpha(temp.m_Name.GetAt(j))) break;
	strGdlx += temp.m_Name[j];
	}
	AcDbObjectId id = DrawInsert(pt, _T("Point"));
	prjmdb.readPipePointBM(strGdlx, strGDDM);
	addapp(aname(id), strGDDM);
	ADDMulXData(id, _T("�ܵ�����"), strGdlx, STRING_TYPE, 0);
	ADDMulXData(id, _T("���"), temp.m_Name, STRING_TYPE, 0);
	str.Format(_T("%f"), temp.m_pt3.x);
	ADDMulXData(id, _T("X����"), str, DOUBLE_TYPE, 0);
	str.Format(_T("%f"), temp.m_pt3.y);
	ADDMulXData(id, _T("Y����"), str, DOUBLE_TYPE, 0);
	str.Format(_T("%f"), temp.m_pt3.z);
	ADDMulXData(id, _T("����߳�"), str, DOUBLE_TYPE, 0);
	str.Format(_T("%f"), temp.m_pipeheight);
	ADDMulXData(id, _T("���߸߳�"), str, DOUBLE_TYPE, 0);
	str.Format(_T("%f"), temp.m_dvalue);
	ADDMulXData(id, _T("���߲�ֵ"), str, DOUBLE_TYPE, 0);
	strLayer.Format(_T("P_SC_%s"), strGdlx);
	setlayer(aname(id), strLayer);
	ptText[X] = pt[X] + 0.2;
	ptText[Y] = pt[Y] + 0.15;
	ptText[Z] = pt[Z];
	AcDbObjectId idText = DrawText(ptText, temp.m_Name, _T("BL"), _T(""), 0.3);
	setlayer(aname(idText), _T("��ע"));
	AcDbObjectId idGroup;
	bool bCreate = CreateGroup(aname(id), idGroup);
	bool bAdd = AddEntToGroup(idText, idGroup);
	}
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	acedCommand(RTSTR, _T("Regen"), RTNONE);*/

}

void FCSSetPBCLJSSM()
{
	CPersonBridgeTechnicalDlg dlg;
	dlg.DoModal();
}

void FCSChangePictureProperty()
{
	IDataBaseOper db;
	CString path = db.readPathTable(_T("��ǰĿ¼"));
	XMJGChangePictureProperty dlg(path);
	dlg.DoModal();
}

void FCSInsertRedLineFile()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.InsertRedLineDwg();
}

void FCSDrawBuildOutLine()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.AddOutLineSymbol();
}

void FCSDrawBuildBoundaries()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.DrawBuildBoundaries();
}

void FCSShowOutLineSymbol()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.ShowOutLineSymbol();
}

void FCSHideOutLineSymbol()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.HideOutLineSymbol();
}

void FCSSetAnnotationAttribute()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.SetAnnotationAttribute();
}

void FCSChangeAzimuthOrder()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.ChangeAzimuthOrder();
}

void FCSChangeAzimuthOrderAll()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.ChangeAzimuthOrderAll();
}

void FCSBOLSetAttatchedInfo()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.SetAttatchedInfo();
}

void FCSExportToDWG1()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.ExportToDWG();
}

void FCSExportToDWG2()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.ExportToDWG2();
}

void FCSExportToDOC()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.ExportToDOC();
}

void FCSJZComprassFile()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg(_T("��������"));
	dlg.DoModal();
}

void FCSDrawBuildNearestBoundaries()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.DrawBuildNearestBoundaries();
}

void FCSModifyBuildName()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.ModifyBuildName();
}

void FCSModifyBuildWidth()
{
	SEEKDOG(); BUILDOUTSET(BuildOutSet);
	BuildOutSet.modifyBuildWidth();
}

#pragma endregion

#pragma region ���ſ���ҵ����

void FCSSetDLProjectCode()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.CreateRSOProject();
}

void FCSDrawRoadEdgeLine()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.DrawRoadEdgeLine();
}

void FCSDrawRoadCentreLine()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.DrawRoadCentreLine();
}

void FCSExportCoodinateTable()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.ExportCoodinateTable();
}

void FCSSetRoadText()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.SetRoadText();
}

void FCSRSOSetAttatchedInfo()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.SetAttatchedInfo();
}

void FCSExportDWGOutput()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.ExportDWGOutput();
}

void FCSExportWORDOutput()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.ExportWORDOutput();
}

void FCSTextDataCheck()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.TextDataCheck();
}

void FCSSetRoadStartEndPoint()
{
	SEEKDOG(); ROADOUTSET(RoadOutSet);
	RoadOutSet.SetRoadStartEndPoint();
}

void FCSDLComprassFile()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg(_T("��·����"));
	dlg.DoModal();
}

#pragma endregion

#pragma region ���ſ���ҵ����

void FCSSetKZDProjectCode()
{
	SEEKDOG(); ELELINGELEVATION(ee);
	ee.createProject();
}

void FCSDrawControlPoint()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.DrawControlPoint();
}

void FCSEESetAttatchedInfo()
{
	SEEKDOG(); ELELINGELEVATION(ee);
	ee.SetAttatchedInfo();
}

void FCSExportResultToDWG1()
{
	if(false == seekdog())return; ELELINGELEVATION(ee);
	ee.ExportResult2DWG1();
}

void FCSExportResultToDWG2()
{
	if(false == seekdog())return; ELELINGELEVATION(ee);
	ee.ExportResult2DWG2();
}

void FCSSZGCComprassFile()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg(_T("ˮ׼�߳�"));
	dlg.DoModal();
}

#pragma endregion

#pragma region ���ſ���ҵ����

void FCSSetZFLXProjectCode()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.createProject();
}

void FCSDrawSurroundingBuild()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.DrawSurroundingBuild();
}

void FCSAxisAutoAnnotion()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.AxisAutoAnnotion();
}

void FCSAnnPretreatment()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.AnnPretreatment();
}

void FCSAxisManualAnnotion()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.AxisManualAnnotion();
}

void FCSCLSetAttatchedInfo()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.SetAttatchedInfo();
}

void FCSAutoBuildingInsideAnnotion()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.AutoBuildingInsideAnnotion();
}

void FCSCLExportResult2DOC()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.ExportResult2DOC();
}

void FCSCLExportResult2DWG1()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.ExportResult2DWG1();
}

void FCSCLExportResult2DWG2()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.ExportResult2DWG2();
}

void FCSCLComprassFile()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg(_T("��������"));
	dlg.DoModal();
}

void FCSCLDrawBuildOutLine()
{
	SEEKDOG(); CHECKLINE(CheckLine);
	CheckLine.drawBuildOutLine();
}

#pragma endregion

#pragma region ���ſ���ҵ����

#pragma region ������������

void FCSSetPBProjectCode()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.createProject();
}

void FCSDrawPhotoPositon()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.DrawPhotoPositon();
}

void FCSBridgeDrawRedLine()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.DrawRedLine();
}

void FCSDrawOwnershipBoundaries()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.DrawOwnershipBoundaries();
}

void FCSGPBSetAttatchedInfo()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.SetAttatchedInfo();
}

void FCSControlPointAndRoadPicturePosition()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.ControlPointAndRoadPicturePosition();
}

void FCSExportDwgPB()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.ExportDWGPB();
}

void ExportDWGPBElevation()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.ExportDWGPBElevation();
}

void ExportDWGPBStratificationPlan()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.ExportDWGPBStratificationPlan();
}

void ExportBuildAreaHeightAndBirdgeHeight()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.ExportBuildAreaHeightAndBirdgeHeight();
}

void ExportApplyTable()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.ExportApplyTable();
}

void ExportPedestrianBridge()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.exportPedestrianBridge();
}

void FCSModifyPhoteSize()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.modifyPhoteSize();
}

void FCSSetBirdgeLayer()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.setBirdgeLayer();
}

void FCSBirdgeHatch()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.birdgeHatch();
}

void FCSBirdgeZPT()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.bridgeZPT();
}

void FCSBridgeHatch()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.bridgeHatch();
}

void FCSBridgeSetHeight()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.bridgeSetHeight();
}

void FCSBridgeAnnoate()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.bridgeAnnoate();
}

void FCSExportVerticalView()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.exportVerticalView();
}

void FCSSetChannelBodyPosition()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.setChannelBodyPosition();
}

void FCSPBStatisticAreaInfo()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.statisticAreaInfo();
}

void FCSSetPBClearWidth()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.setPBClearWidth();
}

void FCSSetPBClearHeight()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.setPBClearHeight();
}

void FCSRemarkLengthWidth()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.remarkLengthWidth();
}

void FCSRemarkBridgeThickness()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.remarkBridgeThickness();
}

void FCSRemarkBridgeBalustrade()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.remarkBridgeBalustrade();
}

void FCSRemarkLoadLevation()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(PedestrianBridge);
	PedestrianBridge.remarkLoadLevation();
}

void FCSRemarkBridgeLevation()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(PedestrianBridge);
	PedestrianBridge.remarkBridgeLevation();
}

void FCSPBChangePictureProperty()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(PedestrianBridge);
	PedestrianBridge.changePicturePropery();
}

void FCSExportPBReport()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.exportPBReport();
}

void FCSSetHandlePipeLine()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(Pedestrianbridge);
	Pedestrianbridge.setHandlePipeLine();
}

void FCSPDCompress()
{
	if(seekdog() == false) return;
	PEDESTRIANBRIDGE(PedestrianBridge);
	PedestrianBridge.compressFile();
}

extern Indexini g_index;

void FCSAddDimRoad()
{
	ads_name ent; ads_point pt; VStr codes; int type = 1; TCHAR ltype[255] = {0}; int isSave = 1;
	if(5100 != ads_getint(_T("\n��ѡ���ع������ͣ�[1]�滮��·�����ߡ�[2]�滮��·���ߡ�[3]ʵ���·�����ߡ�[4]ʵ���·���ߡ�[5]��·�ֽ���<1>*ȡ��*"), &type)) return;
	if(type < 1 || type > 5) return; if(5100 != ads_entsel(_T("\n��ѡ����ع���ʵ�壺"), ent, pt)) return;
	if(5100 != ads_getint(_T("\n�Ƿ񱣴�ԭʼ����[1]������[2]��������<1>"), &isSave)) return;
	AcDbObjectId nid; if(isSave == 1) nid = CopyEntity(aname(ent)); else nid = aname(ent);
	codes.push_back(_T("180304")); codes.push_back(_T("180303")); codes.push_back(_T("180302")); codes.push_back(_T("180301")); codes.push_back(_T("180305"));
	TCHAR lyr[255] = {0}; if(!g_workdef.get_layer(codes[type - 1], lyr)) return; int clr = g_workdef.get_entcolor(codes[type - 1]);
	if(!g_index.get_ltype(codes[type - 1], ltype)) return; setcolor(aname(nid), clr); setlayer(aname(nid), lyr); setlinetype(aname(nid), ltype);
	acutPrintf(_T("\n�ع��ɹ�������")); AddXdata(ent, _T("SOUTH"), 0, codes[type - 1]);
}

#pragma endregion

#pragma region ����

void FCSSetFWProjectCode()
{
	SEEKDOG(); HOUSE(house);
	house.createProject();
}

void FCSDrawCarAddress()
{
	SEEKDOG();
	CCarInfo car; car.drawCar();
}

void FCSDimensionsForCar()
{
	SEEKDOG();
	CCarInfo car; car.dimCar();
}

void FCSSetCarAnnotation()
{
	SEEKDOG();
	CCarInfo car; car.setCarAnnotation();
}

void FCSDrawStratificationPlan()
{
	SEEKDOG(); HOUSE(house);
	house.DrawStratificationPlan();
}

void FCSSetStratificationPlanInfo()
{
	SEEKDOG(); HOUSE(house);
	house.SetStratificationPlanInfo();
}

void FCSDimensionsForChangeArea()
{
	SEEKDOG(); HOUSE(house);
	house.DimensionsForChangeArea();
}

void FCSDrawBuildChangeArea()
{
	SEEKDOG(); HOUSE(house);
	house.DrawBuildChangeArea();
}

void FCSAddBuildChangeAnnotion()
{
	SEEKDOG(); HOUSE(house);
	house.AddBuildChangeAnnotion();
}

void FCSDrawFunctionalAreas()
{
	SEEKDOG(); HOUSE(house);
	house.DrawFunctionalAreas();
}

void FCSExportBuildHeightAndAreaDoc()
{
	SEEKDOG(); HOUSE(house);
	house.ExportBuildHeightAndAreaDoc();
}

void FCSDimensionsFunctionalAreas()
{
	SEEKDOG(); HOUSE(house);
	house.DimensionsFunctionalAreas();
}

void FCSSetFunctionalAreasInfo()
{
	SEEKDOG(); HOUSE(house);
	house.SetFunctionsAreasInfo();
}

void FCSStatisticFunctionArea()
{
	SEEKDOG(); HOUSE(house);
	house.StatisticFunctionArea();
}

void FCSExportHighlySchematic()
{
	SEEKDOG(); HOUSE(house);
	house.ExportHighlySchematic();
}

void FCSExportEntitys2File()
{
	SEEKDOG(); HOUSE(house);
	house.ExportEntitys2File();
}

void FCSExportStratificationPlan()
{
	SEEKDOG(); HOUSE(house);
	house.ExportStratificationPlan();
}

void FCSSetExportHouseDocInfo()
{
	SEEKDOG(); HOUSE(house);
	house.SetExportHouseDocInfo();
}

void FCSExportHouseDoc()
{
	SEEKDOG(); HOUSE(house);
	house.ExportHouseDoc();
}

void FCSExportPlanInfo2MDB()
{
	SEEKDOG(); HOUSE(house);
	house.ExportPlanInfo2MDB();
}

void FCSBuildSetAttatchedInfo()
{
	SEEKDOG(); HOUSE(house);
	house.BuildSetAttatchedInfo();
}

void FCSFunctionDefine()
{
	SEEKDOG(); HOUSE(house);
	house.FunctionDefine();
}

void FCSExportBuildHeightAndAreaInfo()
{
	SEEKDOG(); HOUSE(house);
	house.ExportBuildHeightAndAreaInfo();
}

void FCSExportChangeAreaDWG()
{
	SEEKDOG(); HOUSE(house);
	house.ExportChangeAreaDWG();
}

void FCSExportZPT()
{
	SEEKDOG();
	HOUSE(house);
	house.exportHouseZPT();

	// 	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	// 	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	// 	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	// 	if(false == assist.GetExportRange(lb, rt)) return;
	// 	assist.GetExportRange(lb, rt, nodes);
	// 	assist.GetExportEntitys(IdArr, nodes);
	// 	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	// 	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	// 	assist.DrawMesh(lb, rt, ZongPingTu); MVStr filter;
	// 	CString pathName = assist.getExportFilePath();
	// 	CString fileName = assist.getExportFileName(_T("����������ƽͼ"));
	// 	pathName += _T("\\"); pathName += fileName;	ads_name ssSave;
	// 	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	// 	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	// 	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	// 	CSaveBlocks blocks; blocks.setPath(pathName); ads_point ptMin, ptMax;
	// 	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	// 	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	// 	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	// 	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("��ӡ��Χ"));
	// 	idArr.append(dyfw); blocks.saveBlocks2(idArr, false);
	// 	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	// 	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	// 	openDwgCommand(pathName);

}

void FCSBuildHatch()
{
	SEEKDOG();	HOUSE(house);
	house.buildHatch();
}

void FCSCalculateJZZDMJ()//���㽨��ռ�����
{
	SEEKDOG(); HOUSE(house);
	house.calculateJZZDMJ();
}

void FCSDrawRotateLine()
{
	SEEKDOG(); HOUSE(house);
	house.drawRotateLine();
}

void FCSChangeLayerInfo()
{
	SEEKDOG(); HOUSE(house);
	house.changeLayerInfo();
}

void FCSDrawAreaRange()
{
	SEEKDOG(); HOUSE(house);
	house.drawAreaRange();
}

void FCSComprassFile()
{
	SEEKDOG(); HOUSE(house);
	house.comprassFile();
}

void FCSPicturePosition()
{
	SEEKDOG(); HOUSE(house);
	house.picturePosition();
}

void FCSAddCompress()
{
	SEEKDOG(); HOUSE(house);
	house.addCompass();
}

void FCSLineConvertChangeArea()
{
	SEEKDOG(); HOUSE(house);
	house.lineConvertChangeArea();
}

void FCSAddFunctionAnnotion()
{
	SEEKDOG(); HOUSE(house);
	house.AddFunctionAnnotion();
}

void FCSSetMainHeight()
{
	SEEKDOG(); HOUSE(house);
	house.setMainHeight();
}

void FCSSetMezzanineInfo()
{
	SEEKDOG(); HOUSE(house);
	house.setMezzanineInfo();
}

void FCSSetCarProperty()
{
	SEEKDOG(); HOUSE(house);
	house.setCarProperty();
}

void FCSSettingProjectInfo()
{
	SEEKDOG(); HOUSE(house);
	house.settingProjectInfo();
}

void FCSAddZPTElevation()
{
	SEEKDOG(); HOUSE(house);
	house.addZPTElevation();
}

void FCSDelZPTElevation()
{
	int type = 1;
	if(RTCAN == ads_getint(_T("\n ��ѡ��Ҫɾ����ߵ����ͣ�[1]���⡢[2]���ڡ�[3]����"), &type)) return;
	int oldGroup = GroupReset();
	GroupBack(1); vector<AcDbObjectId> ids;
	IProjectMDB pdb;
	IDataBaseOper oper; TextProperty prop = oper.readTextPropery(_T("�����������ڵ�����ע��"));
	SelectFilter sf(8, prop._layer); AcDbObjectId id;
	if(true == SelectEntity(id, sf, _T("")))
	{
		GetGroupEnts(id, ids); TCHAR bglx[200] = {0};
		for(int idx = 0; idx < (int)ids.size(); ++idx)
		{
			ReadXdata(aname(ids[idx]), _T("BGLX"), 0, bglx);
			if(_tcslen(bglx) == 0) continue;
			if((type == 1 && _tcscmp(bglx, _T("�����ƺ���")) == 0) ||
				(type == 2 && _tcscmp(bglx, _T("���ڵ�����")) == 0) ||
				(type == 3 && _tcscmp(bglx, _T("�������")) == 0))
			{
				DelGroupEnt(id); pdb.delBGTableInfo(bglx);
			}
			else
				MessageBox(GetForegroundWindow(), _T("ɾ���ı��������ѡ��ı�����Ͳ�һ��"), _T("������ʾ"), MB_OK);
		}
	}
	GroupBack(oldGroup);
}

void FCSSingleEdgeNote()
{
	SEEKDOG(); HOUSE(house);
	house.singleEdgeNote();
}

void FCSSetLayerInfoDlg()
{
	SEEKDOG(); HOUSE(house);
	house.setLayerInfoDlg();
}

void FCSExportXMFWX()
{
	SEEKDOG(); HOUSE(house);
	house.exportXMFWX();
}

void FCSSetLayerInfo()
{
	SEEKDOG(); HOUSE(house);
	house.setLayerInfo();
}

void FCSOutputInspection()
{
	IProjectMDB pdb;
	MStr record = pdb.getProjectInfo();
	XMJGAssist assist;
	CString path1 = assist.getExportFilePath(true);
	CString name = assist.getExportFileName(_T("���������ʼ챨��"));
	path1 += name;	char savepath[1024] = {0};
	TcharToChar(path1, savepath);
	SmartTable::Word word; TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("�ʼ챨��.docx"), path))
	{
		acutPrintf(_T("\n �޷��ҵ�ģ���ļ�[�ʼ챨��.docx]"));
		return;
	}
	char filepath[1024] = {0}; TcharToChar(path, filepath);
	word.setEncoding(SmartTable::Encoding_GBK);
	int ret = word.loadWord(filepath);
	TcharToChar(record[_T("���赥λ")], filepath); word.replaceText("#BuildCompany#", filepath);
	TcharToChar(record[_T("��Ŀ����")], filepath); word.replaceText("#ProjectName#", filepath);
	TcharToChar(record[_T("�����")], filepath); word.replaceText("#ACCEPTCODE#", filepath);
	MStr infos = pdb.getJSSMTableInfo(); string info;
	splitBigString(infos[_T("�Բ������¼")], info); word.replaceText("#ZCQKJL#", info.c_str()); info.clear();
	splitBigString(infos[_T("���������¼")], info); word.replaceText("#HCQKJL#", info.c_str()); info.clear();
	splitBigString(infos[_T("һ���ʼ����")], info); word.replaceText("#YJZJBG#", info.c_str()); info.clear();
	splitBigString(infos[_T("�����ʼ����")], info); word.replaceText("#EJZJQK#", info.c_str()); info.clear();
	splitBigString(infos[_T("���������¼")], info); word.replaceText("#CSQKJL#", info.c_str()); info.clear();
	word.save(savepath);
	acutPrintf(_T("\n ���������ɡ�·��Ϊ%s"), path1);
}

void FCSExportBuildHeightPicture()
{
	XMJGAssist assist; IDataBaseOper oper;
	CString savePath = assist.getExportFilePath();
	CString tempname = oper.readFileNameTable(_T("���������߶�ʾ��ͼ"));
	CString dwgpath = GetCurrentDwgFullPath();
	int pos1 = dwgpath.ReverseFind(_T('_')); int pos2 = dwgpath.Find(_T("."));
	CString name = dwgpath.Mid(pos1 + 1, pos2 - pos1 - 1);
	savePath += tempname;
	tempname.Format(savePath, name);
	convertDwg2Pdf(tempname);
}

AcDbObjectIdArray getObjectsByEnt(const AcDbObjectId &id, bool del)
{
	AcGePoint2dArray node; AcDbObjectIdArray ids;
	GetPlList(aname(id), node); ssFromNodes(ids, node);
	if(del == true)	ids.remove(id); return ids;
}

void FCSModifyLayerHieght()
{
	IDataBaseOper oper; TextProperty lmtcmc, lmtgdzj;
	oper.readCommonTable(_T("������������ͼ�����ƴ�С"), lmtcmc._size);
	oper.readCommonTable(_T("������������ͼ��������ɫ"), lmtcmc._color);
	oper.readCommonTable(_T("������������ͼ������ͼ��"), lmtcmc._layer);
	oper.readCommonTable(_T("������������ͼ�߶�ע�Ǵ�С"), lmtgdzj._size);
	oper.readCommonTable(_T("������������ͼ�߶�ע����ɫ"), lmtgdzj._color);
	oper.readCommonTable(_T("������������ͼ�߶�ע��ͼ��"), lmtgdzj._layer);

	double scale = 500;	read1(scale); scale /= 1000.0;
	CString fullPath = GetCurrentDwgFullPath();
	if(fullPath.Find(_T("�����߶�ʾ��ͼ")) == -1)
	{
		MessageBox(GetForegroundWindow(), _T("��򿪸߶�ʾ��ͼ"), _T("������Ϣ"), MB_OK);
		return;
	}
	IProjectMDB pdb; VMStr cxxs = pdb.getCXXTableInfo();
	AcDbObjectIdArray ids; SelectFilter sf(8, _T("GDSYT"));
	if(false == SelectEntitys(ids, sf, _T("X"))) return;
	TCHAR cg[255] = {0}; TCHAR ch[255] = {0};
	MIId miid; TCHAR chzjhandle[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(false == ReadXdata(aname(id), _T("CH"), 0, ch)) continue;
		miid[_ttoi(ch)] = id;
	}
	MIIdIter idit = miid.begin();
	for(; idit != miid.end(); ++idit)
	{
		AcDbObjectId id = idit->second;
		ReadXdata(aname(id), _T("CG"), 0, cg);
		_stprintf(ch, _T("%d"), idit->first);
		for(int indx = 0; indx < (int)cxxs.size(); ++indx)
		{
			MStr cxx = cxxs.at(indx);
			if(cxx[_T("���")].CompareNoCase(ch) != 0) continue;
			double oldcg = _tstof(cg), newcg = _tstof(cxx[_T("���")]);
			if(abs((oldcg - newcg)) < EPS) continue;
			double height = (newcg - oldcg) / 2;
			AcDbObjectIdArray ids = getObjectsByEnt(id, true);
			EntsDel(ids); ads_point mid; AcGePoint2dArray node;
			GetPlList(aname(id), node); GetMiddlePoint(node, mid);
			for(int index = 0; index < node.length(); ++index)
			{
				if(node.at(index).y < mid[Y]) continue;
				node.at(index).y += height * 2;
			}
			SetPolyCoord(aname(id), node); mid[Y] += height;
			CString name = g_fieldchagetable.getNameByIndex(ch);
			DrawText(mid, name, _T("MC"), _T(""), lmtcmc._size * scale);
			for(int index = 0; index < node.length(); ++index)
			{
				if(node.at(index).y < mid[X]) continue;
				mid[X] = node.at(index).x - 2.0 * scale;
			}
			AcDbObjectId textid = DrawText(mid, cxx[_T("���")], _T("MC"), _T(""), lmtgdzj._size * scale, PI / 2);
			MIIdIter iid = idit;
			for(; iid != miid.end(); ++iid)
			{
				AcDbObjectIdArray upids = getObjectsByEnt(iid->second, false);
				MoveEntitys(upids, 0, height * 2);
			}
		}
	}
}

void FCSAddChangeNote()
{
	SEEKDOG(); HOUSE(house);
	house.addChangeNote();
}

void reconsitutionGNQ()
{

}

void FCSDimLine1()
{
	int type = 1;
	if(RTCAN == ads_getint(_T("\n ��ѡ���ع������ͣ�[1]��Χ�ߡ�[2]����������[3]���·�Χ�ߡ�[4]��������Χ�ߡ�[5]������ߡ�[6]���ű߽硢[7]���ߡ�[8]��ǽ�ߡ�[9]��ǽ��<1>"), &type)) return;
	acutPrintf(_T("\n ��ѡ����Ҫ�ع����ߣ�"));
	int isdel = 1;
	if(RTCAN == ads_getint(_T("\n �Ƿ�ɾ��������[1]�ǡ�[2]��<1>"), &isdel)) return;
	while(true)
	{
		AcDbObjectIdArray ids, del; AcGePoint2dArray node;
		SelectFilter sf1(RTDXF0, _T("*POLYLINE")), sf2(RTDXF0, _T("*LINE"));
		if(false == SelectEntitysOr(ids, sf1, sf2, _T(""))) return;
		AcDbObjectId lineid;
		if(ids.length() > 3)
		{
			for(int idx = 0; ids.length() != 0;)
			{
				idx = idx % ids.length();
				AcDbObjectId id = ids.at(idx);
				if(true == getNextPointByLine(id, node))
					ids.remove(id);
				else idx++;
			}
			lineid = DrawPolyline2D(node, 1.0, true, _T(""));
			del.append(ids);
		}
		else if(ids.length() == 1)
			lineid = ids.first();
		else return; CSetEntityPropery prop;
		if(type == 1) prop.setProjectLine(lineid);
		else if(type == 2) prop.setJGBuild(lineid);
		else if(type == 3) prop.setDXFWX(lineid);
		else if(type == 4)
		{
			prop.setGNQ(lineid); CString code;
			int selType = 0; AcDbObjectIdArray gnqs; bool isInner = false;
			SelectFilter sf(8, _T("������")), sf1(RTDXF0, _T("*POLYLINE"));
			if(!SelectEntitys(gnqs, sf, sf1, _T("X"))) return;
			if(RTCAN == ads_getint(_T("\n��ѡ���ع��ķ�ʽ��[0]��ѡ��[1]�߽磺<0>"), &selType))  return;
			selType = selType != 1 ? 0 : 1;
			isInner = gnqs.contains(lineid); XMJGHouse house;
			FunctionNameTableEx last = house.setFunctionalAreaInfo(lineid);
			if(!last.m_init) continue;
			do
			{
				int yesorno = 0;
				if(isInner)
				{
					if(RTCAN == ads_getint(_T("\n��ʵ���Ѿ��ǹ��������Ƿ�Ҫ�����ع�<0>�ǡ�<1>��<0>"), &yesorno)) return;
					isInner = yesorno != 1;
				}
				if(!isInner)
				{
					house.addFunctionProperty(lineid, last);
					house.addFunctionAreaNote(lineid);
					if(last.jm.IsEmpty())
					{
						if(last.code < 9)
							last.cn.Format(_T("0%d"), ++last.code);
						else
							last.cn.Format(_T("%d"), ++last.code);
					}
					else
					{
						if(last.len == 2 && last.code < 9)
							last.cn.Format(_T("0%d"), ++last.code);
						else
							last.cn.Format(_T("%d"), ++last.code);
					}
					gnqs.append(lineid);
				}
				else
				{
					EntDel(aname(lineid)); isInner = false;
				}
				if(selType == 0)
				{
					ads_point pt;
					if(RTNORM != ads_getpoint(NULL, _T("\n������Ҫ�ع���ʵ���е�ѡ��ʵ������Ǳպϣ�:"), pt)) return;
					for(int index = 0; index < gnqs.length(); ++index)
					{
						AcGePoint2dArray node; GetPlList(aname(gnqs[index]), node);
						isInner = PointInPolygon_New(node, apoint(pt)) < 1;
						if(isInner) break;
					}
					if(isInner) continue;
					lineid = createBoundary(pt);
				}
				else
				{
					ads_name ent; ads_point pt;
					if(RTNORM != ads_entsel(_T("\n��ѡ���ع��ı߽�"), ent, pt)) return;
					acdbGetObjectId(lineid, ent); isInner = gnqs.contains(lineid);
				}
			} while(true);
		}
		else if(type == 5) prop.setBG(lineid);
		else if(type == 6) prop.setBridge(lineid);
		else if(type == 7) prop.setRedLine(lineid);
		else if(type == 8) prop.setExteriorWall(lineid);
		else if(type == 9) prop.setInnerWall(lineid);
		if(isdel == 1) EntsDel(del);
	}
}

void defineGnq()
{
	int selType = 0; AcDbObjectIdArray gnqs; AcDbObjectId lineid;
	SelectFilter sf(8, _T("������")), sf1(RTDXF0, _T("*POLYLINE"));
	SelectEntitys(gnqs, sf, sf1, _T("X")); bool isInner = false;
	if(RTCAN == ads_getint(_T("\n��ѡ���ع��ķ�ʽ��[0]��ѡ��[1]�߽磺<0>"), &selType))  return;
	selType = selType != 1 ? 0 : 1; CSetEntityPropery prop; XMJGHouse house;
	FunctionNameTableEx last; int yesorno = 0;
	do
	{
		isInner = false;
		if(selType == 0)
		{
			ads_point pt;
			if(RTNORM != ads_getpoint(NULL, _T("\n������Ҫ�ع���ʵ���е�ѡ��ʵ������Ǳպϣ�:"), pt)) return;
			for(int index = 0; index < gnqs.length(); ++index)
			{
				AcGePoint2dArray node; GetPlList(aname(gnqs[index]), node);
				isInner = PointInPolygon_New(node, apoint(pt)) < 1;
				if(isInner) break;
			}
			if(!isInner)
				lineid = createBoundary(pt);
		}
		else
		{
			ads_name ent; ads_point pt;
			if(RTNORM != ads_entsel(_T("\n��ѡ���ع��ı߽�"), ent, pt)) return;
			acdbGetObjectId(lineid, ent); isInner = gnqs.contains(lineid);
		}
		if(isInner)
		{
			yesorno = 0;
			if(RTCAN == ads_getint(_T("\n��ʵ���Ѿ��ǹ��������Ƿ�Ҫ�����ع�<0>�ǡ�<1>��<0>"), &yesorno)) return;
			isInner = yesorno == 1;
		}
		if(!isInner)
		{
			if(!last.m_init)
				last = house.setFunctionalAreaInfo(lineid);
			if(!last.m_init) continue;
			house.addFunctionProperty(lineid, last);
			ads_name entT; acdbGetAdsName(entT, lineid);
			// �۵�
			if(last.m_island == 1)
			{
				ADDMulXData(lineid, _T("�۵�"), _T("true"), 0, 0);
				setcolor(entT, 7);
			}
			else
			{
				ADDMulXData(lineid, _T("�۵�"), _T("false"), 0, 0);
				setcolor(entT, 4);
			}
			house.addFunctionAreaNote(lineid);
			if(last.jm.IsEmpty())
			{
				if(last.code < 9)
					last.cn.Format(_T("0%d"), ++last.code);
				else
					last.cn.Format(_T("%d"), ++last.code);
			}
			else
			{
				if(last.len == 2 && last.code < 9)
					last.cn.Format(_T("0%d"), ++last.code);
				else
					last.cn.Format(_T("%d"), ++last.code);
			}
			if(fabs(_tstof(last.mjxs) - 0.5) < EPS)
				setlinetype(aname(lineid), _T("x6"));
			SetLineWidth(lineid, 0.06);	gnqs.append(lineid); acdbGetAdsName(entT, lineid);
			acedCommand(RTSTR, _T("draworder"), RTENAME, entT, RTSTR, _T(""), RTSTR, _T("F"), 0);
		}
	} while(true);
}

void FCSDimLine()
{
	int type = 1; int isSave = 1;
	if(RTCAN == ads_getint(_T("\n��ѡ���ع������ͣ�[1]��Χ�ߡ�[2]����������[3]���·�Χ�ߡ�[4]��������Χ�ߡ�[5]������ߡ�[6]���ű߽硢[7]���ߡ�[8]��ǽ�ߡ�[9]��ǽ��<1>"), &type)) return;
	if(RTCAN == ads_getint(_T("\n�Ƿ���ԭʼ����[1]������[2]��������<1>"), &isSave)) return; if(isSave != 1) isSave = 2;
	if(type == 4) defineGnq();
	else if(type == 8 || type == 9)
	{
		CSetEntityPropery prop;
		while(true)
		{
			AcDbObjectIdArray ids, nids, del; AcGePoint2dArray node;
			SelectFilter sf1(RTDXF0, _T("*POLYLINE")), sf2(RTDXF0, _T("*LINE"));
			acutPrintf(_T("\n ��ѡ����Ҫ�ع����ߣ�"));
			if(false == SelectEntitysOr(ids, sf1, sf2, _T(""))) return;
			if(isSave == 1) CopyEntitys(ids, nids); else nids.append(ids);

			for(int i = 0; i < nids.length(); i++)
			{
				AcDbObjectId lineid = nids.at(i);
				if(type == 8) prop.setExteriorWall(lineid);
				else if(type == 9) prop.setInnerWall(lineid);
				ads_name ent; acdbGetAdsName(ent, lineid);
				ads_command(RTSTR, _T("draworder"), RTENAME, ent, RTSTR, _T(""), RTSTR, _T("F"), RTNONE);
			}
			acutPrintf(_T("\n �ع���ɣ�����"));
		}
	}
	else
	{
		CSetEntityPropery prop;
		while(true)
		{
			AcDbObjectIdArray ids, nids, del; AcGePoint2dArray node;
			SelectFilter sf1(RTDXF0, _T("*POLYLINE")), sf2(RTDXF0, _T("*LINE"));
			acutPrintf(_T("\n ��ѡ����Ҫ�ع����ߣ�"));
			if(false == SelectEntitysOr(ids, sf1, sf2, _T(""))) return;
			nids.append(ids); AcDbObjectId lineid;
			if(ids.length() > 3)
			{
				for(int idx = 0; nids.length() != 0;)
				{
					idx = idx % nids.length();
					AcDbObjectId id = nids.at(idx);
					if(true == getNextPointByLine(id, node))
						nids.remove(id);
					else idx++;
				}
				lineid = DrawPolyline2D(node, 1.0, true, _T(""));
				if(isSave == 2)	EntsDel(ids);
			}
			else if(ids.length() == 1)
			{
				lineid = CopyEntity(ids.first());
				if(isSave == 2) EntsDel(ids);
			}
			else return;
			if(type == 1) prop.setProjectLine(lineid);
			else if(type == 2) prop.setJGBuild(lineid);
			else if(type == 3) prop.setDXFWX(lineid);
			else if(type == 5) prop.setBG(lineid);
			else if(type == 6) prop.setBridge(lineid);
			else if(type == 7) prop.setRedLine(lineid);
			else if(type == 8) prop.setExteriorWall(lineid);
			else if(type == 9) prop.setInnerWall(lineid);
			ads_name ent; acdbGetAdsName(ent, lineid);
			ads_command(RTSTR, _T("draworder"), RTENAME, ent, RTSTR, _T(""), RTSTR, _T("F"), RTNONE);
			acutPrintf(_T("\n �ع���ɣ�����"));
		}
	}
}

void FCSBuildCornerNote()
{

}

void FCSDrawProjectLine()
{
	AcDbObjectId id = drawLine();
	if(id == AcDbObjectId::kNull) return;
	CSetEntityPropery prop;
	prop.setProjectLine(id);
}

void FCSDrawDXFWX()
{
	AcDbObjectId id = drawLine();
	if(id == AcDbObjectId::kNull) return;
	CSetEntityPropery prop;
	prop.setDXFWX(id);
}

void FCSEntityColor()
{
	struct resbuf rb;
	ads_getvar(_T("BACKGROUNDPLOT"), &rb);
	AcDbObjectId id = drawLine();
	setcolor(aname(id), 7);
	acutPrintf(_T("\n ʵ�����ɫ��%d"), getcolor(aname(id)));
}

#pragma endregion

#pragma region ��·��������

void FCSSetDLBDGXProjectCode()
{
	SEEKDOG();
	XMJGAssist assist;
	assist.SetProjectCode(_T("��·��������"));
}

void FCSExportControlPointAndRoadPicture()
{
	SEEKDOG(); ROADNOPIPELINE(Road);
	Road.ExportControlPointAndRoadPicture();
}

void FCSExportRoadStratificationPlan()
{
	SEEKDOG(); ROADNOPIPELINE(Road);
	Road.ExportRoadStratificationPlan();
}

void FCSExportRoadProfile()
{
	SEEKDOG(); ROADNOPIPELINE(Road);
	Road.ExportRoadProfile();
}

void FCSExportRoadCrossSectional()
{
	SEEKDOG(); ROADNOPIPELINE(Road);
	Road.ExportRoadCrossSectional();
}

void FCSExportRoadComprehensiveCrossSectional()
{
	SEEKDOG(); ROADNOPIPELINE(Road);
	Road.ExportRoadComprehensiveCrossSectional();
}

#pragma endregion

#pragma region ��·������

#pragma endregion

#pragma endregion

void FCSCheckRedLineQuality()
{
	XMJGCheckQuality quality;
	quality.CheckQualityRedLine();
}

void FCSSetProjectContext()
{
	XMJGAssist assist;
	assist.SetProjectContext1();
}

void FCSExportEntitys()
{
	AcDbObjectIdArray IdArr;
	if(false == SelectEntitys(IdArr, _T(""))) return;
	CFileDialog *dlg = new CFileDialog(FALSE, _T("��ѡ�񱣴�·��"));
	if(IDOK != dlg->DoModal())	return;
	XMJGExport exportdwg;	exportdwg.setObjects(IdArr);
	exportdwg.setSavePath(dlg->GetPathName());
	exportdwg.setObjects(IdArr); int exporttype = 0;
	exportdwg.setPictureFrame(_T("XMJG�ֲ�ƽ��ͼ"));
	ads_getint(_T("\n �����ʽ<0>dwg <1>:jpg "), &exporttype);
	if(exporttype == 0)	exportdwg.ExportDwg();
	else exportdwg.ExportPicture(true);
}

void FCSSetProjectInfo()
{
	//ADDMulXData()
	/*XMJGProjectInfo info;*/
	//info.DoModal();
}

void FCSModifyGCCLLineWidth()
{
	SelectFilter sf(RTDXF0, _T("*LINE"));
	AcDbObjectIdArray ids;
	if(!SelectEntitys(ids, sf, _T(""))) return;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		AcDbEntity *pEnt;
		acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
		if(pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pLine = AcDbPolyline::cast(pEnt);
			pLine->setConstantWidth(0.15);
		}
		else if(pEnt->isKindOf(AcDbLine::desc()))
		{
			AcDbLine *pLine = AcDbLine::cast(pEnt);
			pLine->setThickness(0.15);
		}
		pEnt->close();
	}
}

void FCSCopyFiles()
{
	XMJGAssist assist;
	CString path = assist.getExportFilePath();
	assist.copyFiles(path);
}

void FCSClearJPG()
{
	IProjectMDB pdb;
	pdb.clearPictureTable();
	TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("���ͳ��.ini"), path))
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ������ļ�[���ͳ��.ini]"), _T("�ļ���ʧ��ʾ"), MB_OK);
		return;
	}
	CStdioFile sf;
	CString filepath(path);
	if(FALSE == sf.Open(filepath, CFile::modeCreate | CFile::modeWrite))
		MessageBox(GetForegroundWindow(), _T("��������ļ�[���ͳ��.ini]ʧ��!!!"), _T("��Ϣ��ʾ"), MB_OK);
	sf.Close();
}

void OpenDwgCmd()
{
	TCHAR szPath[255] = {0};
	ads_getstring(0, _T(""), szPath);
	CString strPath = GetCurrentDwgPrefix();
	if(IsPathFileExists(szPath))
		OpenDWG(szPath);
}

double CalcLineLength(const AcGePoint2dArray &node, int sidx, int eidx)
{
	if(sidx == -1 || eidx == -1)
	{
		sidx = 0; eidx = node.length() - 1;
	}
	if((sidx + 1) == eidx)
		return distOF2d(node.at(sidx), node.at(eidx));
	int midx = (sidx + eidx) / 2; double dist = 0.0;
	dist = CalcLineLength(node, sidx, midx);
	dist += CalcLineLength(node, midx, eidx);
	return dist;
}

bool CalcMiddlePoint(const AcGePoint2dArray &node, double &dist, AcGePoint2d &mid, double length, int sidx, int eidx)
{
	if(sidx == -1 || eidx == -1)
	{
		sidx = 0; eidx = node.length() - 1;
	}
	double llen = 0.0;
	if(sidx == eidx) return false;
	if((sidx + 1) == eidx)
	{
		AcGePoint2d spt = node.at(sidx);
		AcGePoint2d ept = node.at(eidx);
		llen = distOF2d(spt, ept);
		if((dist + llen) > length)
		{
			double bl = (length - dist) / llen;
			mid.x = spt.x + (ept.x - spt.x) * bl;
			mid.y = spt.y + (ept.y - spt.y) * bl;
			return true;
		}
		dist += llen;
		return false;
	}
	int midx = (sidx + eidx) / 2;
	if(true == CalcMiddlePoint(node, dist, mid, length, sidx, midx)) return true;
	if(true == CalcMiddlePoint(node, dist, mid, length, midx, eidx)) return true;
	return false;
}

void testBlock(const AcDbObjectId &ObjId)
{


	//��������������ʵ��
	// 		AcDbBlockTableRecordIterator *iter;
	// 		AcDbBlockTableRecord *pRecord;
	// 		id = pBlockRef->blockTableRecord();
	// 		pEnt->close();
	// 		es = acdbOpenObject(pRecord, id, AcDb::kForRead);
	// 		pRecord->newIterator(iter);
	// 		pRecord->close();
	// 		for(iter->start(); !iter->done(); iter->step())
	// 		{
	// 			AcDbEntity * pent;
	// 			iter->getEntity(pent, AcDb::kForWrite);
	// 			//�ж��ǲ��ǿ����
	// 			if(pent->isKindOf(AcDbBlockReference::desc()))
	// 			{
	// 				//������ǿ�Ļ�������ݹ�
	// 				AcDbObjectId objIdTemp = pent->objectId();
	// 				pent->close();
	// 				ChangeBlockRefenceColor(objIdTemp, nRed, nGreen, nBlue);
	// 			}
	// 			else
	// 			{
	// 				AcCmColor color;
	// 				color.setRGB(nRed, nGreen, nBlue);
	// 				es = pent->setColor(color);
	// 				pent->close();
	// 			}
	// 		}
	// 		delete iter;
	//	}
	// 	else
	// 	{
	// 		AcCmColor color;
	// 		color.setColorMethod(AcCmEntityColor::kByBlock);
	// 		color.setRGB(nRed, nGreen, nBlue);
	// 		pEnt->setColor(color);
	// 		pEnt->close();
	// 	}
}

void modifyDimStyle()
{

	// ��õ�ǰͼ�εı�ע��ʽ�� 
	// 	AcDbDimStyleTable *pDimStyleTbl;  
	// 	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStyleTbl, AcDb::kForWrite); 
	// 
	// 	if(pDimStyleTbl->has(styleName))
	// 	{
	// 		pDimStyleTbl->close();
	// 			return;
	// 	}
	// 
	// 	// �����µı�ע��ʽ���¼
	// 	AcDbDimStyleTableRecord *pDimStyleTblRcd;
	// 	pDimStyleTblRcd = new AcDbDimStyleTableRecord(); 
	// 
	// 	// ���ñ�ע��ʽ������ 
	// 	pDimStyleTblRcd->setName(styleName);
	// 	// ��ʽ����  	
	// 	pDimStyleTblRcd->setDimasz(3); 
	// 	// ��ͷ����
	// 	pDimStyleTblRcd->setDimexe(3);   
	// 	// �ߴ�������ע ���ƫ���� 
	// 	pDimStyleTblRcd->setDimtad(1); 
	// 	// ����λ�ڱ�ע�� ���Ϸ�
	// 	pDimStyleTblRcd->setDimtxt(3);     // ��ע���ֵĸ߶� 
	// 
	// 	// ����ע��ʽ���¼��ӵ���ע��ʽ���� 
	// 	pDimStyleTbl->add(pDimStyleTblRcd); 
	// 
	// 	pDimStyleTblRcd->close();  
	// 	pDimStyleTbl->close();
}

void openMulitDwg()
{
	COpenProjectDlg dlg1;
	//dlg1.setProjectDir(path);
	dlg1.DoModal();
}

void SavePipeBlock1(AcDbObjectIdArray &idArr)
{
	ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	SSToIdArr(ssSave, idArr);
	ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("��ӡ��Χ"));
	idArr.append(dyfw);
}

enum Azimuth
{
	LB = 1,
	RB = 2,
	RT = 3,
	LT = 4
};

AcGePoint2d getExtremePoint(const AcGePoint2d &pt1, const AcGePoint2d &pt2, Azimuth az)
{
	AcGePoint2d pos;
	if(az == LB)
	{
		pos.x = pt1.x < pt2.x ? pt1.x : pt2.x;
		pos.y = pt1.y < pt2.y ? pt1.y : pt2.y;
	}
	else if(az == LT)
	{
		pos.x = pt1.x < pt2.x ? pt1.x : pt2.x;
		pos.y = pt1.y > pt2.y ? pt1.y : pt2.y;
	}
	else if(az == RT)
	{
		pos.x = pt1.x > pt2.x ? pt1.x : pt2.x;
		pos.y = pt1.y > pt2.y ? pt1.y : pt2.y;
	}
	else if(az == RB)
	{
		pos.x = pt1.x > pt2.x ? pt1.x : pt2.x;
		pos.y = pt1.y < pt2.y ? pt1.y : pt2.y;
	}
	return pos;
}

void getNodesArray2(const AcGePoint2dArray &rect, const AcGePoint2dArray &output)
{
	SWipeOut out;
	AcGePoint2dArray sub1; AcGePoint2d temp1, temp2, temp3, temp4;
	temp1 = getExtremePoint(rect.at(0), output.at(0), LB); sub1.append(temp1);
	temp2 = getExtremePoint(rect.at(0), output.at(0), RB); sub1.append(temp2);
	temp3 = getExtremePoint(rect.at(1), output.at(1), RB); sub1.append(temp3);
	temp4 = getExtremePoint(rect.at(1), output.at(1), LB); sub1.append(temp4);
	AcDbObjectId id1 = DrawPolyline2D(sub1, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent1; acdbGetAdsName(ent1, id1); setcolor(ent1, 1); out.WipeOut(id1);

	AcGePoint2dArray sub2;
	temp1 = getExtremePoint(rect.at(1), output.at(1), LB); sub2.append(temp1);
	temp2 = getExtremePoint(rect.at(1), output.at(1), LT); sub2.append(temp2);
	temp3 = getExtremePoint(rect.at(2), output.at(2), LT); sub2.append(temp3);
	temp4 = getExtremePoint(rect.at(2), output.at(2), LB); sub2.append(temp4);
	AcDbObjectId id2 = DrawPolyline2D(sub2, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent2; acdbGetAdsName(ent2, id2); setcolor(ent2, 1); out.WipeOut(id2);

	AcGePoint2dArray sub3;
	temp1 = getExtremePoint(rect.at(2), output.at(2), LT); sub3.append(temp1);
	temp2 = getExtremePoint(rect.at(2), output.at(2), RT); sub3.append(temp2);
	temp3 = getExtremePoint(rect.at(3), output.at(3), RB); sub3.append(temp3);
	temp4 = getExtremePoint(rect.at(3), output.at(3), LB); sub3.append(temp4);
	AcDbObjectId id3 = DrawPolyline2D(sub3, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent3;  acdbGetAdsName(ent3, id3); setcolor(ent3, 1); out.WipeOut(id3);

	AcGePoint2dArray sub4;
	temp1 = getExtremePoint(rect.at(3), output.at(3), LT); sub4.append(temp1);
	temp2 = getExtremePoint(rect.at(3), output.at(3), LB); sub4.append(temp2);
	temp3 = getExtremePoint(rect.at(0), output.at(0), RB); sub4.append(temp3);
	temp4 = getExtremePoint(rect.at(0), output.at(0), RT); sub4.append(temp4);
	AcDbObjectId id4 = DrawPolyline2D(sub4, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent4; acdbGetAdsName(ent4, id4); setcolor(ent4, 1); out.WipeOut(id4);
}

void getNodesArray3(const AcGePoint2dArray &rect, const AcGePoint2dArray &output)
{
	AcColorSettings pColorSettings;
	acedGetCurrentColors(&pColorSettings); // Get the current colors
	int red = GetRValue(pColorSettings.dwGfxModelBkColor);
	int green = GetGValue(pColorSettings.dwGfxModelBkColor);
	int blue = GetBValue(pColorSettings.dwGfxModelBkColor);
	SWipeOut out;
	AcGePoint2dArray sub1; AcGePoint2d temp1, temp2, temp3, temp4;
	temp1 = getExtremePoint(rect.at(0), output.at(0), LB); sub1.append(temp1);
	temp2 = getExtremePoint(rect.at(0), output.at(0), RB); sub1.append(temp2);
	temp3 = getExtremePoint(rect.at(1), output.at(1), RT); sub1.append(temp3);
	temp4 = getExtremePoint(rect.at(1), output.at(1), LT); sub1.append(temp4);
	AcDbObjectId id1 = DrawPolyline2D(sub1, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent1; acdbGetAdsName(ent1, id1);  AcDbObjectId xy1 = out.WipeOut(id1);
	setcolor(aname(xy1), red, green, blue);

	AcGePoint2dArray sub2;
	temp1 = getExtremePoint(rect.at(1), output.at(1), RB); sub2.append(temp1);
	temp2 = getExtremePoint(rect.at(1), output.at(1), RT); sub2.append(temp2);
	temp3 = getExtremePoint(rect.at(2), output.at(2), LT); sub2.append(temp3);
	temp4 = getExtremePoint(rect.at(2), output.at(2), LB); sub2.append(temp4);
	AcDbObjectId id2 = DrawPolyline2D(sub2, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent2; acdbGetAdsName(ent2, id2);  AcDbObjectId xy2 = out.WipeOut(id2);
	setcolor(aname(xy2), red, green, blue);

	AcGePoint2dArray sub3;
	temp1 = getExtremePoint(rect.at(2), output.at(2), LT); sub3.append(temp1);
	temp2 = getExtremePoint(rect.at(2), output.at(2), RT); sub3.append(temp2);
	temp3 = getExtremePoint(rect.at(3), output.at(3), RB); sub3.append(temp3);
	temp4 = getExtremePoint(rect.at(3), output.at(3), LB); sub3.append(temp4);
	AcDbObjectId id3 = DrawPolyline2D(sub3, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent3;  acdbGetAdsName(ent3, id3);  AcDbObjectId xy3 = out.WipeOut(id3);
	setcolor(aname(xy3), red, green, blue);

	AcGePoint2dArray sub4;
	temp1 = getExtremePoint(rect.at(3), output.at(3), LT); sub4.append(temp1);
	temp2 = getExtremePoint(rect.at(3), output.at(3), LB); sub4.append(temp2);
	temp3 = getExtremePoint(rect.at(0), output.at(0), RB); sub4.append(temp3);
	temp4 = getExtremePoint(rect.at(0), output.at(0), RT); sub4.append(temp4);
	AcDbObjectId id4 = DrawPolyline2D(sub4, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent4; acdbGetAdsName(ent4, id4); AcDbObjectId xy4 = out.WipeOut(id4);
	setcolor(aname(xy4), red, green, blue);
}

void getNodesArray1(const AcGePoint2dArray &rect, const AcGePoint2dArray &output)
{
	AcColorSettings pColorSettings;
	acedGetCurrentColors(&pColorSettings); // Get the current colors
	int red = GetRValue(pColorSettings.dwGfxModelBkColor);
	int green = GetGValue(pColorSettings.dwGfxModelBkColor);
	int blue = GetBValue(pColorSettings.dwGfxModelBkColor);
	SWipeOut out;
	AcGePoint2dArray sub1; AcGePoint2d temp1, temp2, temp3, temp4;
	temp1 = getExtremePoint(rect.at(0), output.at(0), LB); sub1.append(temp1);
	temp2 = getExtremePoint(rect.at(0), output.at(0), RB); sub1.append(temp2);
	temp3 = getExtremePoint(rect.at(1), output.at(1), RT); sub1.append(temp3);
	temp4 = getExtremePoint(rect.at(1), output.at(1), LT); sub1.append(temp4);
	AcDbObjectId xy1 = out.WipeOut(sub1);
	//AcDbObjectId id1 = DrawPolyline2D(sub1, 1.0, true, _T("200000"), 0.0, 0, _T("CE"));
	//setcolor(aname(id1), 1);
	//ads_name ent1; acdbGetAdsName(ent1, id1);  AcDbObjectId xy1 = out.WipeOut(id1);
	//setcolor(aname(xy1), red, green, blue); 	

	AcGePoint2dArray sub2;
	temp1 = getExtremePoint(rect.at(1), output.at(1), RB); sub2.append(temp1);
	temp2 = getExtremePoint(rect.at(1), output.at(1), RT); sub2.append(temp2);
	temp3 = getExtremePoint(rect.at(2), output.at(2), LT); sub2.append(temp3);
	temp4 = getExtremePoint(rect.at(2), output.at(2), LB); sub2.append(temp4);
	AcDbObjectId id2 = DrawPolyline2D(sub2, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent2; acdbGetAdsName(ent2, id2);  AcDbObjectId xy2 = out.WipeOut(id2);
	//setcolor(aname(xy2), red, green, blue);

	AcGePoint2dArray sub3;
	temp1 = getExtremePoint(rect.at(2), output.at(2), LT); sub3.append(temp1);
	temp2 = getExtremePoint(rect.at(2), output.at(2), RT); sub3.append(temp2);
	temp3 = getExtremePoint(rect.at(3), output.at(3), RB); sub3.append(temp3);
	temp4 = getExtremePoint(rect.at(3), output.at(3), LB); sub3.append(temp4);
	AcDbObjectId id3 = DrawPolyline2D(sub3, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent3;  acdbGetAdsName(ent3, id3);  AcDbObjectId xy3 = out.WipeOut(id3);
	//setcolor(aname(xy3), red, green, blue);

	AcGePoint2dArray sub4;
	temp1 = getExtremePoint(rect.at(3), output.at(3), LT); sub4.append(temp1);
	temp2 = getExtremePoint(rect.at(3), output.at(3), LB); sub4.append(temp2);
	temp3 = getExtremePoint(rect.at(0), output.at(0), RB); sub4.append(temp3);
	temp4 = getExtremePoint(rect.at(0), output.at(0), RT); sub4.append(temp4);
	AcDbObjectId id4 = DrawPolyline2D(sub4, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
	ads_name ent4; acdbGetAdsName(ent4, id4); AcDbObjectId xy4 = out.WipeOut(id4);
	//setcolor(aname(xy4), red, green, blue);
}

void getNodesArray(const AcGePoint2dArray &rect, const AcGePoint2dArray &output)
{
	for(int idx = 0; idx < 4; ++idx)
	{
		AcGePoint2dArray sub; int pos = (idx + 1) % 4;
		AcGePoint2d temp1 = getExtremePoint(rect.at(idx), output.at(idx), Azimuth((LB + idx) % 4)); sub.append(temp1);
		AcGePoint2d temp2 = getExtremePoint(rect.at(idx), output.at(idx), Azimuth((RB + idx) % 4)); sub.append(temp2);
		AcGePoint2d temp3 = getExtremePoint(rect.at(pos), output.at(pos), Azimuth((RT + idx) % 4)); sub.append(temp3);
		AcGePoint2d temp4 = getExtremePoint(rect.at(pos), output.at(pos), Azimuth((LT + idx) % 4)); sub.append(temp4);
		AcDbObjectId tkid = DrawPolyline2D(sub, 1.0, true, _T("2000000"), 0.0, 0, _T("CE"));
		setcolor(aname(tkid), 1);
	}
}

AcGePoint2dArray createRectangle(ads_point lb, ads_point rt)
{
	AcGePoint2dArray node;
	node.append(AcGePoint2d(lb[X], lb[Y]));
	node.append(AcGePoint2d(lb[X], rt[Y]));
	node.append(AcGePoint2d(rt[X], rt[Y]));
	node.append(AcGePoint2d(rt[X], lb[Y]));
	return node;
}

bool getArcPoint(AcGePoint2d &spt, AcGePoint2d &ept, AcGePoint2d &cpt, double &dRad)
{
	AcGePoint2d ptCenter;	ads_point spt1, ept1, dir, pt4, pt5;
	if(RTNORM != ads_getpoint(NULL, _T("\n��ѡ�񻡶ε���㣺"), spt1)) return false;
	if(RTNORM != ads_getpoint(NULL, _T("\n��ѡ�񻡶ε��յ㣺"), ept1)) return false;
	if(RTNORM != ads_getpoint(NULL, _T("\n��ѡ�񻡶εķ���"), dir)) return false;
	spt.x = spt1[X]; spt.y = spt1[Y]; ept.x = ept1[X]; ept.y = ept1[Y];
	AcGePoint2d dir1(dir[X], dir[Y]); double len = spt.distanceTo(ept) / 2;
	TCHAR info[255] = {0};
	while(true)
	{
		_stprintf(info, _T("\n�����뻡�εİ뾶��[%.3lf]"), len);
		if(RTNORM != ads_getreal(info, &dRad)) return false;
		if(dRad >= len) break;
		acutPrintf(_T("\n ����İ뾶���ٴ���%.3lf��"), len);
	}
	double temp = acos(len / dRad);	double angle = ads_angle(spt1, ept1);
	ads_polar(spt1, angle - temp, dRad, pt4);
	ads_polar(spt1, angle + temp, dRad, pt5);
	if(dir1.distanceTo(apoint(pt4)) < dir1.distanceTo(apoint(pt5)))
		cpt = AcGePoint2d(pt5[X], pt5[Y]);
	else
		cpt = AcGePoint2d(pt4[X], pt4[Y]);
	return true;
}

AcDbObjectId drawArcEntity()
{
	AcGePoint2d spt, ept, cpt;	double dRad = 0.0; CLockManager lock;
	if(false == getArcPoint(spt, ept, cpt, dRad)) return AcDbObjectId::kNull;
	//����������ʱ����
	double dAngleS = 0.0, dAngleE = 0.0, dAngleM = 0;
	dAngleS = ads_angle(apoint(cpt), apoint(spt));
	dAngleE = ads_angle(apoint(cpt), apoint(ept));
	bool bSmall = false;
	if(dAngleS > dAngleE)
	{
		double temp = dAngleS;
		dAngleS = dAngleE;
		dAngleE = temp;
	}
	AcDbArc *pArc = new AcDbArc;
	pArc->setCenter(apoint(cpt));
	pArc->setRadius(dRad);
	pArc->setStartAngle(dAngleS);
	pArc->setEndAngle(dAngleE);
	AcDbObjectId id;
	AppendEntity(pArc, id);
	pArc->close();
	return id;
}

#include "SYPSampleJJG.h"
#include "XMJGMunicipal.h"

void print(ads_u_val val)
{
	acutPrintf(_T(" [%.3lf]"), val.rreal);
	acutPrintf(_T(" [%d]"), val.rint);
	acutPrintf(_T(" [%s]"), val.rstring);
	acutPrintf(_T(" [%ld %ld]"), val.rlname);
	acutPrintf(_T(" [%ld]"), val.rlong);
	acutPrintf(_T(" [%s]"), val.ihandle);
}

void DrawOrdinateDimension()
{
	ads_point spt, ept;
	if(RTNORM != ads_getpoint(NULL, _T("\n��ѡ���ע�����:"), spt)) return;
	if(RTNORM != ads_getpoint(spt, _T("\n��ѡ���ע���յ�:"), ept)) return;
	AcDbOrdinateDimension *dim = new AcDbOrdinateDimension;
	dim->useXAxis(); dim->setDefiningPoint(apoint(spt));
	dim->setLeaderEndPoint(apoint(ept)); AcDbObjectId id;
	AppendEntity(dim, id); dim->close();
}

void drawSDFW()
{
	ads_point pt1, pt2, pt3, pt4;
	ads_getpoint(NULL, _T(""), pt1);
	ads_getpoint(pt1, _T(""), pt2);
	ads_getpoint(pt2, _T(""), pt3);
	double dist = distOF2d(apoint(pt2), apoint(pt3));
	double angle = ads_angle(pt2, pt3);
	ads_polar(pt1, angle, dist, pt4);
	drawline(pt1, pt2, _T(""));	drawline(pt2, pt3, _T(""));
	drawline(pt3, pt4, _T(""));	drawline(pt4, pt1, _T(""));
}

#include "Psapi.h"
#pragma comment(lib,"Psapi.lib")
bool DebugPrivilege(const TCHAR *PName, BOOL bEnable)
{
	bool              bResult = TRUE;
	HANDLE            hToken;
	TOKEN_PRIVILEGES  TokenPrivileges;

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		bResult = FALSE;
		return bResult;
	}
	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

	LookupPrivilegeValue(NULL, PName, &TokenPrivileges.Privileges[0].Luid);
	AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	if(GetLastError() != ERROR_SUCCESS)
	{
		bResult = FALSE;
	}

	CloseHandle(hToken);
	return bResult;
}
//���ǻ����ԡ������������ʵ����ö�ٸý��̵������õ���ģ��ľ��������ӡ����
int main111()
{
	DebugPrivilege(SE_DEBUG_NAME, TRUE);
	HMODULE nHmodule[1024] = {NULL};
	TCHAR    lpFilename[MAX_PATH] = {0};
	DWORD cbNeeded = 0;

	DWORD idProcess = GetCurrentProcessId();//��ȡ����������ID
	HANDLE hCalc = ::OpenProcess(PROCESS_ALL_ACCESS, NULL, idProcess);//��ȡ���̵ľ��
	if(!hCalc)
	{
		::MessageBox(NULL, _T("���ź�����û�����м�����"), _T("EnumProcessModules"), MB_OK);
	}
	else
	{
		BOOL bRetn = ::EnumProcessModules(hCalc, nHmodule, sizeof(nHmodule), &cbNeeded);
		if(!bRetn)
		{
			CloseHandle(hCalc);
			::MessageBox(NULL, _T("���ź�,��ȡģ����ʧ��"), _T("EnumProcessModules"), MB_OK);
		}
		else
		{
			for(int i = 0; i < ((int)cbNeeded / (int)sizeof(HMODULE)); i++)
			{
				GetModuleFileNameEx(hCalc, nHmodule[i], lpFilename, MAX_PATH);
				acutPrintf(_T("\n��%d����%s"), i + 1, lpFilename);
			}
		}
	}
	DebugPrivilege(SE_DEBUG_NAME, FALSE);
	return 0;
}

void createDuandian(const AcGePoint2d &spt, const AcGePoint2d &ept, double buf, AcGePoint2d &left, AcGePoint2d &right)
{
	double angle = ads_angle(apoint(spt), apoint(ept)); ads_point pt;
	ads_polar(apoint(spt), angle + PI / 2, buf, pt); left.x = pt[X]; left.y = pt[Y];
	ads_polar(apoint(spt), angle - PI / 2, buf, pt); right.x = pt[X]; right.y = pt[Y];
	AcDbArc *pArc = new AcDbArc;
	pArc->setCenter(apoint(spt));
	pArc->setRadius(buf);
	pArc->setStartAngle(angle + PI / 2);
	pArc->setEndAngle(angle - PI / 2);
	AcDbObjectId id;
	AppendEntity(pArc, id);
	pArc->close();
}

void createMiddleBuffer(AcGePoint2d &lastpt, const AcGePoint2d &spt, const AcGePoint2d &mid, const AcGePoint2d &ept, double buf)
{
	double angle1 = ads_angle(apoint(mid), apoint(spt));
	double angle2 = ads_angle(apoint(mid), apoint(ept));
	double angle = (angle2 + angle1) / 2;
	double len = buf / sin(angle);
	ads_point pt; ads_polar(apoint(mid), angle, len, pt);
	ads_point ept1; ads_polar(apoint(ept), PI / 2 + angle2, buf, ept1);
	drawline(apoint(lastpt), pt, _T("")); drawline(pt, ept1, _T(""));
	lastpt.x = ept1[X]; lastpt.y = ept1[Y];
}

AcGePoint2d createLineLeft(const AcGePoint2d &lastpt, const AcGePoint2d &spt, const AcGePoint2d &mid, const AcGePoint2d &ept, double buf)
{
	double angle1 = ads_angle(apoint(mid), apoint(spt));
	double angle2 = ads_angle(apoint(mid), apoint(ept));
	double angle = (angle2 + angle1) / 2;
	double len = buf / sin(angle);
	ads_point pt; ads_polar(apoint(mid), angle, len, pt);
	ads_point ept1; ads_polar(apoint(ept), PI / 2 + angle2, buf, ept1);
	drawline(apoint(lastpt), pt, _T("")); drawline(pt, ept1, _T(""));
	return apoint(ept1);
}

AcGePoint2d createLineRight(const AcGePoint2d &lastpt, const AcGePoint2d &spt, const AcGePoint2d &mid, const AcGePoint2d &ept, double buf)
{
	double angle1 = ads_angle(apoint(mid), apoint(spt));
	double angle2 = ads_angle(apoint(mid), apoint(ept));
	double angle = (angle2 + angle1) / 2;
	double len = buf / sin(angle);
	ads_point pt; ads_polar(apoint(mid), angle, len, pt);
	ads_point ept1; ads_polar(apoint(ept), PI / 2 + angle2, buf, ept1);
	drawline(apoint(lastpt), pt, _T("")); drawline(pt, ept1, _T(""));
	return apoint(ept1);
}

void createBuffer()
{
	ads_name ent; ads_point pt; AcDbObjectId id; double dist = 1.0;
	if(RTNORM != ads_entsel(_T("\n��ѡ����Ҫ���ɻ��������ߣ�"), ent, pt)) return;
	if(RTNORM != ads_getreal(_T("\n�����뻺�����Ĵ�С��"), &dist)) return;
	AcGePoint2dArray node;	GetPlList(ent, node);
	AcGePoint2d left, right, lastpt;
	if(IsDeasil(node)) node.reverse(); int len = node.length();
	createDuandian(node.at(0), node.at(1), dist, left, right);
	for(int idx = 1; idx < node.length() - 1; ++idx)
	{
		right = createLineLeft(left, node[idx - 1], node[idx], node[idx + 1], dist);
		left = createLineRight(right, node[idx - 1], node[idx], node[idx + 1], dist);
	}
	createDuandian(node[len - 1], node[len - 2], dist, left, right);
}

#include "lineparallel.h"
#include "C3DOperator.h"

void ZffMyProjectintersect1()
{
	// TODO: Implement the command
	ads_name polyLinename;
	ads_name polyLinename2;
	ads_point pnt;
	ads_point pnt2;
	AcDbPolyline *pPolyline = NULL;
	AcDbPolyline *pPoly = NULL;
	AcDbRegion *pRegion1 = NULL;
	AcDbRegion *pRegion2 = NULL;
	acedEntSel(_T("\n��ѡ���ȡ�����!"), polyLinename, pnt);
	AcDbObjectId polyLineId;
	acdbGetObjectId(polyLineId, polyLinename);
	AcDbEntity *pEnt = NULL;
	AcGePoint3dArray pts, pts2;
	acdbOpenObject(pEnt, polyLineId, AcDb::kForWrite);

	if(pEnt->isKindOf(AcDbPolyline::desc()))
	{
		pPolyline = AcDbPolyline::cast(pEnt);
		pEnt->close();
	}
	AcDbVoidPtrArray curves;
	int num = pPolyline->numVerts();
	for(int i = 0; i < num; i++)
	{
		AcGePoint2d pt;
		pPolyline->getPointAt(i, pt);
		pts.append(apoint(pt));
	}
	pPolyline->getSplitCurves(pts, curves);
	pPolyline->close();
	AcDbObjectIdArray curvesId;
	AcDbCurve *pCurve = NULL;
	for(int i = 0; i < curves.length(); i++)
	{
		pCurve = static_cast<AcDbCurve*>(curves[i]);
		AcDbObjectId tempid;
		AppendEntity(pCurve, tempid);
		curvesId.append(tempid);
	}
	pCurve->close();
	acedEntSel(_T("\n��ѡ�񱻽�ȡ�����!"), polyLinename2, pnt2);
	acdbGetObjectId(polyLineId, polyLinename2);
	acdbOpenObject(pEnt, polyLineId, AcDb::kForWrite);
	if(pEnt->isKindOf(AcDbPolyline::desc()))
	{
		pPoly = AcDbPolyline::cast(pEnt);
		pEnt->close();
		//pPoly->intersectWith(pPolyline,AcDb::kOnBothOperands,pts,0,0);
	}
	AcDbVoidPtrArray curves2;
	num = pPoly->numVerts();
	for(int i = 0; i < num; i++)
	{
		AcGePoint2d pt2;
		pPoly->getPointAt(i, pt2);
		pts2.append(apoint(pt2));
	}
	pPoly->getSplitCurves(pts2, curves2);
	pPoly->close();
	AcDbObjectIdArray curvesId2;
	AcDbCurve *pCurve2 = NULL;
	for(int i = 0; i < curves2.length(); i++)
	{
		pCurve2 = static_cast<AcDbCurve*>(curves2[i]);
		AcDbObjectId tempid;
		AppendEntity(pCurve2, tempid);
		curvesId2.append(tempid);
	}
	pCurve2->close();
	AcDbObjectIdArray regId = CreateRegion(curvesId);
	AcDbObjectIdArray regId2 = CreateRegion(curvesId2);
	for(int i = 0; i < curvesId.length(); i++)
	{
		acdbOpenObject(pCurve, curvesId[i], AcDb::kForWrite);
		pCurve->erase();
		pCurve->close();
	}
	for(int i = 0; i < curvesId2.length(); i++)
	{
		acdbOpenObject(pCurve2, curvesId2[i], AcDb::kForWrite);
		pCurve2->erase();
		pCurve2->close();
	}
	acdbOpenObject(pRegion1, regId[0], AcDb::kForWrite);
	acdbOpenObject(pRegion2, regId2[0], AcDb::kForWrite);
	pRegion1->booleanOper(AcDb::kBoolIntersect, pRegion2);
	pRegion1->close();
	pRegion2->close();
}


void SetTextStyle1(ads_name ent, TCHAR *font, ads_real bili)
{
	AcDbObjectId id;
	AcDbText *pText;

	if(Acad::eOk != acdbGetObjectId(id, ent)) return;
	if(Acad::eOk != acdbOpenObject(pText, id, AcDb::kForWrite)) return;

	if(_tcscmp(font, _T("")))
	{
		AcDbTextStyleTable *pTextStyleTbl;
		acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl, AcDb::kForWrite);
		if(pTextStyleTbl->getAt(font, id) == Acad::eOk)
		{
			pText->setTextStyle(id);
		}
		else
		{
			AcDbTextStyleTableRecord *ptexttablerecord = new AcDbTextStyleTableRecord;
			ptexttablerecord->setName(font);
			CString Font(font); Font = Font.MakeLower();
			if(Font.Find(_T(".shx")) == -1)
				ptexttablerecord->setFont(font, 0, 0, 134, 49);
			else
				ptexttablerecord->setFileName(font);
			AcDbObjectId id;
			pTextStyleTbl->add(id, ptexttablerecord);
			ptexttablerecord->close();
			pText->setTextStyle(id);
		}
		pTextStyleTbl->close();
	}
	if(bili > 0.0)
	{
		pText->setWidthFactor(bili);
	}
	pText->close();
}

#include "BridgePropertyDlg.h"


bool loadCadMenu(const CString& strMenuFileName, const CString& strMenuGroupName)
{
	CString strFilePath = _T("D:\\cass\\CASS2010\\CASS 92 SSCS For AutoCAD2010\\system\\acad.cuix");
	//strFilePath += strMenuFileName;

	CFileStatus st;
	if(!CFile::GetStatus(strFilePath, st)) return false;

	if(strFilePath.GetLength() < 1) return false;

	CString strCmdLoad;
	strFilePath.Replace(_T("\\"), _T("\\\\"));
	if(_T("") != strMenuGroupName && strFilePath.GetLength() > 0)
	{
		//�����������Ĳ˵��ļ�
		strCmdLoad.Format(_T("(if (null (menugroup \"%s\")) (command \"menuload\" \"%s\"))"), strMenuGroupName, strFilePath);
	}
	else
	{
		strCmdLoad.Format(_T("(command \"menuload\" \"%s\")"), strFilePath);
	}

	int erStat = ads_queueexpr((LPTSTR)(LPCTSTR)strCmdLoad);//�˺������൱�ڳ�������󣬸�CAD����һ�����ز˵�������

	return true;
}

#include "DrawDomainDialog.h"

DrawDomainDialog g_drawDomainDialog;

void FCSTestCommand()
{
	
	return;







// 
// 	AcDbObjectIdArray ids;
// 	if(!SelectEntitys(ids, _T(""))) return;
// 	CKernalTool::WMFOutEx(_T("D:\\QQ����\\WMF\\WMF\\yfh.wmf"), ids);





	return;

	loadCadMenu(_T(""), _T(""));
	return;

	CAcModuleResourceOverride resource1;
	BridgePropertyDlg dlg;
	dlg.DoModal(); return;


	ads_name ent; ads_point pt; AcDbObjectId id;
	if(5100 != ads_entsel(_T("\nѡ��������ߣ�"), ent, pt)) return;
	acdbGetObjectId(id, ent); AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead);
	AcDbVoidPtrArray ptrs1, ptrs2; ptrs1.append(pEnt);
	AcDbRegion::createFromCurves(ptrs1, ptrs2);
	pEnt->close();
	if(ptrs2.length() == 0) acutPrintf(_T("\nѡ��������ཻ"));
	else acutPrintf(_T("\nѡ����߲��뽻"));

	return;
	TCHAR font[255] = {0};
	if(RTNORM != ads_entsel(_T("\n��ѡ��ʵ�壺"), ent, pt));
	if(RTNORM != ads_getstring(0, _T("\n���������壺"), font)) return;
	SetTextStyle1(ent, font, 1.3);
}

void FCSRedLineLoftingA3()
{

}

void modifyTextSize()
{

}

void FCSRedLineLoftingA4()
{
	modifyCGFrame();
	// 	
	// 	double ws = width / 210; double hs = height / 297;
	// 	double whs = ws < hs ? ws : hs;
	// 	AcDbObjectId tkid = DrawInsert(spt, _T("RedLineLoftingA4"));
	// 	SBreakBlock block; block.SetBlock(tkid); block.Explode();
	// 	ScaleEntitys(block.m_objIdArr, apoint(spt), whs);
	// 	int bl = (int)(whs / scale * 10);	TCHAR Scale[200] = {0};
	// 	bl *= 100; _stprintf(Scale, _T("%d"), bl);
	// 	block.replaceText(_T("#SCALE#"), Scale);
}

void FCSOpenProject()
{
	COpenProjectDlg dlg1;
	if(IDOK != dlg1.DoModal()) return;
	openNewDwgFile(dlg1.getOpenFiles());
	TCHAR mdbpath[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), mdbpath))
	{
		AfxMessageBox(_T("���ݿ��ļ�xmjg.mdb�޷��ҵ�"));
		return;
	}
	CDatabaseOperManager database;
	database.initialConnect(mdbpath);
	MStr filter, record;
	filter[_T("�ؼ���")] = _T("��ǰĿ¼");
	record[_T("����ֵ")] = dlg1.getProjectPath();
	database.UpdataRecord(_T("XMJGPath"), filter, record);
	clearInitFile();
}

void FCSCompressProject()
{
	CAcModuleResourceOverride myResource;
	CCompressDlg dlg;
	dlg.DoModal();
}

void FCSOpenPath()
{
	IDataBaseOper oper;
	CString dir = oper.readPathTable(_T("����Ŀ¼"));
	ShellExecute(NULL, _T("open"), dir, NULL, NULL, SW_SHOW);
}

void FCSProjectInfoPrompt()
{

	CAcModuleResourceOverride myResources;
	g_projectpromptdock.setDlg(CProjectInfoPrompt::IDD);
	if(g_projectpromptdock.GetSafeHwnd() != NULL)
	{
		if(g_projectpromptdock.IsVisible() == false)
		{
			acedGetAcadFrame()->ShowControlBar(&g_projectpromptdock, TRUE, FALSE);
		}
	}
	else
	{
		g_projectpromptdock.Create(acedGetAcadFrame(), _T("Ӱ�����"));
		g_projectpromptdock.EnableDocking(CBRS_ALIGN_ANY | CBRS_BORDER_ANY);
		g_projectpromptdock.RestoreControlBar();
	}
}

void FCSOutputDiXingTu()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	XMJGAssist assist;	AcDbObjectIdArray IdArr;
	AcGePoint2d lb, rt; AcGePoint2dArray nodes;
	if(false == assist.GetExportRange(lb, rt)) return;
	assist.GetExportRange(lb, rt, nodes);
	assist.GetExportEntitys(IdArr, nodes);
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
	assist.DrawMesh(lb, rt, DiXingTu); MVStr filter;
	CString pathName = assist.getExportFilePath();
	CString fileName = assist.getExportFileName(_T("������������ͼ"));
	pathName += _T("\\"); pathName += fileName;	ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	CSaveBlocks blocks; blocks.setPath(pathName);
	blocks.saveBlocks2(idArr, false);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	openDwgCommand(pathName);
}

void FCSRegen()
{
	acedCommand(RTSTR, _T("Regen"), RTNONE);
}

void FCSEElevationalPoint()
{
	struct resbuf *rb = ads_buildlist(8, _T("�̵߳�_hide"), RTNONE);
	ads_name ssName;
	if(RTNORM != ads_ssget(_T("X"), NULL, NULL, rb, ssName)) return;
	AcDbObjectIdArray ids; SSToIdArr(ssName, ids);
	acDocManager->lockDocument(curDoc());
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		setlayer(aname(id), _T("GCD"));
		addapp(aname(id), _T("202101"));
	}
	acDocManager->unlockDocument(curDoc());
}

void clearInitFile()
{
	TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("���ͳ��.ini"), path))
	{
		MessageBox(GetForegroundWindow(), _T("�޷��ҵ������ļ�[���ͳ��.ini]"), _T("�ļ���ʧ��ʾ"), MB_OK);
		return;
	}
	CStdioFile sf;
	CString filepath(path);
	if(FALSE == sf.Open(filepath, CFile::modeCreate | CFile::modeWrite))
		MessageBox(GetForegroundWindow(), _T("��������ļ�[���ͳ��.ini]ʧ��!!!"), _T("��Ϣ��ʾ"), MB_OK);
	sf.Close();
}

void clearOpenedDwgFile()
{
	AcApDocManager *pDocMgr = acDocManagerPtr();
	AcApDocumentIterator *pDocItr;
	AcApDocument *pMainDoc = NULL;
	pDocItr = pDocMgr->newAcApDocumentIterator();
	for(; !pDocItr->done(); pDocItr->step())
	{
		AcApDocument *pDoc = pDocItr->document(); bool bFind = false;
		AcApDocumentArray::iterator pit = g_projectappdocs.begin();
		for(; pit != g_projectappdocs.end(); ++pit)
		{
			if(*pit != pDoc) continue; bFind = true;
			acDocManagerPtr()->closeDocument(pDoc);
			g_projectappdocs.erase(pit); break;
		}
	}
	// 
	// 
	// 
	// 	AcApDocument *pDoc = NULL;
	// 	for(int idx = 0; idx < g_projectappdocs.size(); ++idx)
	// 	{
	// 		pDoc = g_projectappdocs[idx];
	// 		if(pDoc == NULL) continue;
	// 		acDocManager->closeDocument(pDoc);
	// 	}
	// 	g_projectappdocs.clear();
}

void openNewDwgFile(const VStr &paths)
{
	clearOpenedDwgFile();
	AcApDocument *pDoc = NULL;
	for(int idx = 0; idx < (int)paths.size(); ++idx)
	{
		CString pathName = paths[idx];
		if(acDocManager->isApplicationContext())
		{
			acDocManager->appContextOpenDocument(pathName);
			pDoc = acDocManager->curDocument();
			g_projectappdocs.push_back(pDoc);
		}
	}
}

void FCSCompareExport()
{
	SEEKDOG(); HOUSE(house);
	house.CompareExport();
}

void FCSAreaStatistic()
{
	SEEKDOG(); HOUSE(house);
	house.areaStatistic();
}

void FCSInnerWallLine()
{
	SEEKDOG(); HOUSE(house);
	house.innerWallLine();
}

void FCSStatisticHalfWall()
{
	SEEKDOG(); HOUSE(house);
	house.statisticHalfWall();
}

void FCSModifyJGBuildName()
{
	SEEKDOG(); HOUSE(house);
	house.modifyJGBuildName();
}

void FCSCheckFunctionDefine()
{
	SEEKDOG(); HOUSE(house);
	house.checkFunctionDefine();
}

void FCSDrawLayerFirst()
{
	SEEKDOG(); HOUSE(house);
	house.drawLayerFirst();
}

void FCSBoundaryNote()
{
	SEEKDOG();
	HOUSE(house);
	house.boundaryNote();
}

void getExtendProperty(const AcDbObjectId &id, int nType, const CString &xName, int dType)
{
	ads_point ptEnt = {0};
	GetEntPosition(aname(id), ptEnt);
	TCHAR szNum[255] = {0};
	READMulXData(id, xName, szNum, dType, 0);
	ads_point ptText;
	ptText[X] = ptEnt[X] + 0.25;
	ptText[Y] = ptEnt[Y] - 0.4 * (nType - 2);
	ptText[Z] = ptEnt[Z];
	if(nType != 1)
	{
		CString value(szNum);
		int pos = value.FindOneOf(_T("."));
		_stprintf(szNum, _T("%s"), value.Mid(0, pos + 3));
	}
	AcDbObjectId idText = DrawText(ptText, szNum, _T("ML"), _T(""), 0.3);
	setlayer(aname(idText), _T("��ע"));
	AcDbObjectId idGroup;
	bool bCreate = CreateGroup(aname(id), idGroup);
	bool bAdd = AddEntToGroup(idText, idGroup);

}

void FCSHeightPointText()
{
	SEEKDOG();
	int nType = 1;

	TCHAR sztext[255] = {0};
	long l = 0;
	AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, _T(""))) return;
	TCHAR code[10] = {0};
	if(RTCAN == ads_getint(_T("\n��ѡ����Ҫ��ʾ������:<1>��š�<2>�̡߳�<3>�߲<4>ȫ����[1]"), &nType)) return;
	if(nType < 1 || nType > 4) nType = 4; TCHAR value[20] = {0};
	acedSetStatusBarProgressMeter(_T("��������ע��..."), 0, ids.length() - 1);
	for(l = 0; l < ids.length(); l++)
	{
		acedSetStatusBarProgressMeterPos(l);
		AcDbObjectId id = ids.at(l);
		if(false == ReadXdata(aname(id), _T("�ܵ�����"), 0, value)) continue;
		EraseGroup(aname(id)); bool flag = false;
		switch(nType)
		{
			case 4: flag = true;
			case 1: getExtendProperty(id, 1, _T("���"), STRING_TYPE); if(!flag) break;
			case 2: getExtendProperty(id, 2, _T("����߳�"), DOUBLE_TYPE); if(!flag) break;
			case 3: getExtendProperty(id, 3, _T("���߲�ֵ"), DOUBLE_TYPE); if(!flag) break;
			default: break;
		}
	}
	acedRestoreStatusBar();
	acutPrintf(_T("\n ע�Ǹ��������"));
}

void FCSSetGXProjectCode()
{
	SEEKDOG();
	PIPELINE(PiPeLine);
	PiPeLine.createPipeLineProject();
}

void FCSPIPELINEModifyProjectInfo()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.modifyProjectInfo();
}

//ѡȡ�����ܵ㣬���عܵ����ͺ͹ܵ�����
bool ChoosePipePoint(AcDbObjectIdArray &ids, TCHAR* szGxlx, AcGePoint2d &pt, bool bfirst)
{
	TCHAR code[50] = {0}, szTemp[50] = {0};
	ads_point ptTemp, entpt;
	int nChoose = 0;//ѡ�еĹܵ㣨�ܵ���飩����
	ads_name ent; VObjectID vids;
	if(bfirst == true)
		if(RTNORM != ads_entsel(_T("\n ��ѡ���һ���ܵ㣺"), ent, entpt)) return false;
		else
			if(RTNORM != ads_entsel(_T("\n ��ѡ����һ���ܵ㣺"), ent, entpt)) return false;
	GetGroupEnts(aname(ent), vids);
	vids.push_back(aname(ent));
	for(size_t i = 0; i < (int)vids.size(); ++i)
	{
		AcDbObjectId id = vids[i];
		GetStdm(aname(id), code);
		if(false == ReadXdata(aname(id), _T("�ܵ�����"), 0, szGxlx)) continue;
		++nChoose;
		if(nChoose > 1)
		{
			acutPrintf(_T("ѡ�йܵ���࣬��ѡ�񵥸��ܵ㣡\n"));
			return false;
		}
		GetEntPosition(aname(id), ptTemp);
		pt[X] = ptTemp[X]; pt[Y] = ptTemp[Y];
	}

	// 
	// 	if(false == SelectEntitys(ids, _T(""))) return false;
	// 	for(size_t i = 0; i < ids.length(); ++i)
	// 	{
	// 		AcDbObjectId id = ids[i];
	// 		GetStdm(aname(id), code);
	// 		bool bDate = ReadXdata(aname(id), _T("�ܵ�����"), STRING_TYPE, szTemp);
	// 		if(!bDate) continue;
	// 		++nChoose;
	// 		if(nChoose > 1)
	// 		{
	// 			acutPrintf(_T("ѡ�йܵ���࣬��ѡ�񵥸��ܵ㣡\n"));
	// 			return false;
	// 		}
	// 		ReadXdata(aname(id), _T("�ܵ�����"), STRING_TYPE, szGxlx);
	// 		GetEntPosition(aname(id), ptTemp);
	// 		pt[X] = ptTemp[X]; pt[Y] = ptTemp[Y];
	// 	}
	return true;
}

void FCSDrawPipeLine()
{
	FCSModifyPiPeLineProperty();
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.drawPipeLine();
}

void PtNearestNode(AcGePoint3d pt, AcDbObjectId id, AcGePoint3d &nearNode)
{
	AcGePoint2dArray Pt2dArray;
	AcGePoint2d ptTemp, pt2d;
	int nNode = 0;

	pt2d[X] = pt[X]; pt2d[Y] = pt[Y];
	GetPlList(aname(id), Pt2dArray);
	ptTemp = Pt2dArray.at(0);
	double dist = distOF2d(pt2d, ptTemp);
	for(int i = 1; i < Pt2dArray.length(); i++)
	{
		ptTemp = Pt2dArray.at(i);
		double dTemp = distOF2d(pt2d, ptTemp);
		if(dTemp < dist)
		{
			dist = dTemp;
			nNode = i;
		}
	}

	nearNode[X] = Pt2dArray[nNode][X]; nearNode[Y] = Pt2dArray[nNode][Y];
	nearNode[Z] = pt[Z];
}

// ��ȡ�㵽�ߵ���̾���
bool GetBestDist(ads_name ent, ads_point pt, double& dist)
{
	AcDbObjectId objId;
	AcDbCurve* pCurve = NULL;
	if(Acad::eOk != acdbGetObjectId(objId, ent)) return false;
	if(Acad::eOk != acdbOpenObject(pCurve, objId, AcDb::kForRead)) return false;
	AcGePoint3d ptSource;
	ptSource.x = pt[X];
	ptSource.y = pt[Y];
	ptSource.z = pt[Z];
	AcGePoint3d ptOnCurve;
	pCurve->getClosestPointTo(ptSource, ptOnCurve);
	dist = distOF3d(ptSource, ptOnCurve);
	pCurve->close();
	return true;
}

//ѡ���·�����յ�
bool ChooseRoadPoint(AcGePoint3d &point, const AcDbObjectId &idRoad, const CString &info)
{
	ads_point pt; double dHeight = 0.0; ads_name ent;
	acdbGetAdsName(ent, idRoad);
	if(RTNORM != ads_getpoint(NULL, info, pt)) return false;
	double dist = 0.0; if(!GetBestDist(ent, pt, dist))
	{
		ads_printf(_T("\nѡ��ĵ�·�㲻�ڵ�·�������ϣ�����"));
		return false;
	}
	if(dist > 0.01) return true;
	if(RTNORM != ads_getreal(_T("\n ������õ�̣߳�"), &dHeight)) return false;
	pt[Z] = dHeight; point = apoint(pt);
	return true;
}

BOOL AddRoadXDate(AcDbObjectId &idRoad, AcGePoint3d nearNodeS, AcGePoint3d nearNodeE)
{
	CString strStart, strEnd;
	TCHAR szRoad[255] = {0};

	//if (RTCAN == ads_getstring(NULL, _T("\n�������·����������:"), szRoad)) return FALSE;
	strStart.Format(_T("%f,%f,%f"), nearNodeS[X], nearNodeS[Y], nearNodeS[Z]);
	strEnd.Format(_T("%f,%f,%f"), nearNodeE[X], nearNodeE[Y], nearNodeE[Z]);
	AddXdata(aname(idRoad), _T("��·���������"), STRING_TYPE, strStart);
	AddXdata(aname(idRoad), _T("��·�������յ�"), STRING_TYPE, strEnd);
	return TRUE;
}

void FCSRoadStartEnd()
{
	SEEKDOG();

	AcGePoint3d ptS, ptE, nearNodeS, nearNodeE;
	AcDbObjectId idRoad, idRoad1; ads_name ent;
	int nFlag = 0;

	while(1)
	{
		ads_point pos; bool flag = false;
		TCHAR szStdm[200] = {0}; int ret = 0;
		if(RTCAN == ads_entsel(_T("\n��ѡ���·�����ߣ�"), ent, pos)) return;
		do
		{
			GetStdm(ent, szStdm);
			if(0 == _tcscmp(szStdm, _T("180302")) || 0 == _tcscmp(szStdm, _T("180304")))
			{ flag = true; break; }
			if(RTCAN == ads_entsel(_T("\nѡ��Ĳ��ǵ�·�����ߣ�������ѡ���·�����ߣ�"), ent, pos)) return;
		} while(flag == false);

		if(!ChooseRoadPoint(ptS, aname(ent), _T("\n��ѡ���·���������"))) continue;;
		if(!ChooseRoadPoint(ptE, aname(ent), _T("\n��ѡ���·�������յ�"))) continue;
		if(distOF3d(ptS, ptE) < EPS)
		{
			acutPrintf(_T("��ѡ�����յ��ص�!"));
			continue;
		}
		break;
	};
	acdbGetObjectId(idRoad, ent);
	PtNearestNode(ptS, idRoad, nearNodeS);
	PtNearestNode(ptE, idRoad, nearNodeE);
	AddRoadXDate(idRoad, nearNodeS, nearNodeE);
	acutPrintf(_T("���!\n"));
}

void FCSExportRoadZPT()
{
	SEEKDOG();
	HOUSE(house);
	house.exportRoadZPT();
}

void FCSPictureAndKZD()
{
	SEEKDOG(); HOUSE(house);
	house.PicAndKZD();
}

void FCSDrawHDM()
{
	SEEKDOG();
	CLichengFromZdline fromzd;
	AcDbObjectId idLine;
	double dStart = 0.0;//���׮���
	if(!fromzd.hdmcreate(idLine, dStart)) return;
	fromzd.writeFile(idLine, dStart);
}

void FCSExportPipeZPT()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.exportPipeLineDWG();
}

void FCSFlushPipeLineProperty()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.flushPipeLineProperty();
}

void FCSMatchingPipePoint()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.matchingPipePoint();
}

void FCSChangePipeLineZJ()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.changePipeLineZJ();
}

void FCSAddPipeLineRemark()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.addPipeLineRemark();
}

void FCSExportPipeLineDwg()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.exportPipeLineDwg();
}

void FCSExportSingleReport()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.exportSingleReport();
}

void FCSModifyPipeLineZJXH()
{
	SEEKDOG();
	PIPELINE(PipeLine);
	PipeLine.modifyPipeLineZJXH();
}

void FCSModifyPiPeLineProperty()
{
	SEEKDOG();
	PIPELINE(PiPeLine);
	PiPeLine.modifyPipeLineProperty();
	::SetFocus(acedGetAcadDwgView()->GetSafeHwnd());
}

void FCSExportPipeLineZPT()
{
	VStr vmsLayers;
	CPipeLineSelectOutputDlg dlg;
	if(dlg.DoModal() != IDOK) return;

}

void FCSExportPipeLineDZT()
{
	VStr vmsLayers;
	CPipeLineSelectOutputDlg dlg;
	if(dlg.DoModal() != IDOK) return;
}

void FCSSetBridgeProperty()
{
	CAcModuleResourceOverride Resource1;
	BridgePropertyDlg dlg; double area = 0.0;
	dlg.DoModal(); AcDbObjectId hxid, tqid;
	SelectFilter sf1(8, _T("����"));
	SelectFilter sf2(5020, _T("*POLYLINE"));
	IProjectMDB pdb;
	if(SelectEntity(hxid, sf1, sf2, _T("X")))
	{
		AcDbEntity *pEnt = NULL;
		acdbOpenAcDbEntity(pEnt, hxid, AcDb::kForRead);
		AcDbPolyline *pLine = (AcDbPolyline *)pEnt;
		bool flag = (pLine->isClosed() == Adesk::kTrue);
		pEnt->close(); CString info;
		if(flag)
		{
			GetEntArea(aname(hxid), area);
			info.Format(_T("%.3lf"), area);
		}
		else
			info.Format(_T("%s"), _T("����δ�պ�"));
		MStr record; record[_T("�õغ������")] = info;
		pdb.setMJXXTableInfo(record);
	}
	else
	{
		MStr record; record[_T("�õغ������")] = _T("�޷���ȡ����");
		pdb.setMJXXTableInfo(record);
	}
	sf1 = SelectFilter(8, _T("���ű߽�"));
	sf2 = SelectFilter(5020, _T("*POLYLINE"));
	double tqarea = 0.0; MStr record;
	if(SelectEntity(tqid, sf1, sf2, _T("X")))
	{
		GetEntArea(aname(tqid), tqarea);
		record[_T("���Ž������")].Format(_T("%.3lf"), tqarea);
	}
	else
		record[_T("���Ž������")].Format(_T("�޷��ҵ����ű߽�"));
	if(area == 0.0 || tqarea == 0.0)
		record[_T("���Ž����ܶ�")].Format(_T("%.3lf"), 0.0);
	else record[_T("���Ž����ܶ�")].Format(_T("%.3lf"), tqarea / area);
	pdb.setMJXXTableInfo(record);
}

//�����;
void CorssSection()
{
	CPipeSection pipe;
	pipe.StartEx();
}

//�ݶ���;
void VerSection()
{
	CLoadVerFace load;
	load.Start();
}

void AddXData()
{
	ads_name ent; ads_point pos;
	if(RTNORM != ads_entsel(_T("\n ��ѡ����Ҫ�����չ���Ե�ʵ�壺"), ent, pos)) return;
	TCHAR name[255] = {0}; int type = 0; TCHAR val[255] = {0};
	if(RTNORM != ads_getstring(0, _T("\n ��������չ���Ե����ƣ�"), name)) return;
	if(RTNORM != ads_getint(_T("\n ��ѡ����չ���Ե����ͣ�<0>string��<1>int, <2>long, <3>double��"), &type)) return;
	if(RTNORM != ads_getstring(0, _T("\n ��������չ���Ե�ֵ��"), val)) return;
	AddXdata(ent, name, type, val);
}

void FCSModifyRoadPro()
{
	SEEKDOG();
	CAcModuleResourceOverride myResources;
	CModifyRoadPro dlg;
	if(IDOK != dlg.DoModal()) return;
}

void FCSSetSZProject()
{
	SEEKDOG();
	CAcModuleResourceOverride myResources;
	CSZProjectDlg dlg;
	if(IDOK != dlg.DoModal()) return;
}

bool strLess(const CString &str1, const CString &str2)
{
	return str1.Compare(str2) > 0;
}

void FCSExportSZReport()
{
	CPipeLineOutputPartDlg dlg;
	if(IDOK != dlg.DoModal()) return;
	VStr part = dlg.getExportPart();
	CString templet; int type = 0;
	map<CString, int> table;
	table[_T("��·��������")] = 1;
	table[_T("���߿�������")] = 2;
	table[_T("���ſ�������")] = 4;
	sort(part.begin(), part.end(), strLess);
	for(int idx = 0; idx < (int)part.size(); ++idx)
	{
		CString val = part[idx];
		type += table[val];
		templet += val.Mid(0, 2);
	}
	if(templet.IsEmpty() == true) return;
	templet += _T("��������.docx");
	XMJGMunicipal szReport;
	if(false == szReport.initWord(templet)) return;
	szReport.replaceJBXX();
	switch(type)
	{
		case 1: szReport.exportRoadJGYSBGS(); break;
		case 2: szReport.exportPipeLineTable(); break;
		case 3: szReport.exportRoadPipeReport(); break;
		case 4: szReport.exporBridgeJGYSBGS(); break;
		case 7: szReport.exportRoadPipeBridgeReport(); break;
	}

}

void FCSRemarkPipePoint()
{
	if(seekdog() == false) return;
	PIPELINE(PipeLine);
	PipeLine.remarkPipePoint();
}

void FCSDimPipelineFlow()
{
	if(seekdog() == false) return;
	PIPELINE(PipeLine);
	PipeLine.dimPipelineFlow();
}

void FCSAddPrintTK()
{
	if(seekdog() == false) return;
	PIPELINE(PipeLine);
	PipeLine.addPrintTK();
}

void FCSExportRoadJSSM()
{
	if(seekdog() == false) return;
	XMJGMunicipal mun(7); TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(_T("��·�����ܽ�.docx"), path)) return;
	if(false == mun.initWord(path)) return;
	mun.exportRoadJSZJ();
}

void FCSModifyRoadHDMInfo()
{
	if(seekdog() == false) return;
	MIdStr idstr;
	{
		CBackScreen bs;
		CRoadHDMTLDlg dlg;
		if(IDOK != dlg.DoModal()) return;
		idstr = dlg.getIdBlock();
	}
	for(MIDSIter it = idstr.begin(); it != idstr.end(); ++it)
		addRoadHDMTuLi(it->first, it->second);
}

void FCSQinagHou()
{
	while(true)
	{
		SelectFilter sf(RTDXF0, _T("*LINE")); AcDbObjectIdArray ids;
		if(!SelectEntitys(ids, sf, _T(""))) return;
		if(ids.length() != 2) return;
		AcDbEntity *iEnt1, *iEnt2;
		if(Acad::eOk != acdbOpenAcDbEntity(iEnt1, ids.first(), AcDb::kForRead)) return;
		if(Acad::eOk != acdbOpenAcDbEntity(iEnt2, ids.last(), AcDb::kForRead))
		{
			iEnt1->close(); continue;
		}
		AcGePoint3dArray node;
		if(Acad::eOk != iEnt1->intersectWith(iEnt2, AcDb::kOnBothOperands, node))
		{
			iEnt2->close();
			iEnt1->close();
			continue;
		}
		iEnt1->close();
		iEnt2->close();
		if(node.length() != 0)
		{
			acutPrintf(_T("\n��ѡ���������ཻ��ֱ��")); continue;
		}
		AcGePoint2dArray node1, node2;
		GetPlList(aname(ids.first()), node1);
		GetPlList(aname(ids.last()), node2);
		double len = chuiju(node1.first(), node2.first(), node2.last(), false);
		ads_point pos;
		pos[X] = 0.0;
		pos[Y] = 0.0;
		pos[Z] = 0.0;
		//if(RTNORM != ads_getpoint(NULL, _T("\n��ʰȡ��ע��λ�ã�"), pos)) return;
		TCHAR info[255] = {0};
		_stprintf(info, _T("%d"), (int)((len + 0.0001) * 100));
		AcDbObjectId cid = DrawCircle(pos, 0.4, _T(""));
		setcolor(aname(cid), 3);
		AcDbObjectId tid = DrawText(pos, info, _T("MC"), _T(""), 0.35, 0.0, 1.0);
		setcolor(aname(tid), 3); SetTextStyle(tid, _T("����"));
		AcDbObjectIdArray mids;
		mids.append(cid);
		mids.append(tid);

		CMoveJig * pJig = new CMoveJig(asPnt3d(pos));
		pJig->doIt(mids);
		if(!pJig->isok())
			EntsDel(mids);
		delete pJig;
		pJig = NULL;
	}
}

void FCSModifyProjectInfo()
{
	TCHAR path[255] = {0}; CStringArray paths, files;
	if(!GetDirectory(_T("��ѡ����Ҫ�޸ĵ���Ŀ�ļ���"), path)) return;
	if(!CFilePathOperater::GetChildDirectory(path, paths, files, false)) return;
	ProjectInfos infos;
	for(int idx = 0; idx < paths.GetSize(); ++idx)
	{
		CString pPath = paths.GetAt(idx);
		ProjectInfo info;
		int pos = pPath.ReverseFind('\\');
		pos = pos == -1 ? pPath.ReverseFind(_T('/')) : pos;
		info.m_pPath = pPath.Mid(0, pos + 1);
		pPath = pPath.Mid(pos + 1);
		if(pPath.CompareNoCase(_T("temp")) == 0) continue;
		pos = pPath.FindOneOf(_T("_")); info.isModify = false;
		info.m_des.m_code = info.m_src.m_code = pPath.Left(pos);
		info.m_des.m_name = info.m_src.m_name = pPath.Mid(pos + 1);
		infos.push_back(info);
	}
	CAcModuleResourceOverride myResources;
	CModifyProjectInfo dlg; dlg.setProjectInfo(infos);
	dlg.DoModal(); infos = dlg.getProjectInfos();
	acedSetStatusBarProgressMeter(_T("�����޸���Ŀ��Ϣ�����Ժ󡣡���"), 0, infos.size() - 1);
	TCHAR oldname[255] = {0}, newname[255] = {0};
	for(int idx = 0; idx < (int)infos.size(); ++idx)
	{
		ProjectInfo info = infos[idx];
		acedSetStatusBarProgressMeterPos(idx);
		if(!info.isModify) continue;
		CString src = info.m_pPath + info.m_src.m_code + _T("_") + info.m_src.m_name;
		CString des = info.m_pPath + info.m_des.m_code + _T("_") + info.m_des.m_name;
		_stprintf(oldname, _T("%s"), src); _stprintf(newname, _T("%s"), des);
		_trename(oldname, newname);
		CString dbpath = des + _T("\\�滮\\GC\\��Ŀ��Ϣ.mdb");
		IProjectMDB pdb; if(!pdb.initMDB(dbpath)) continue;
		MStr record; record[_T("�����")] = info.m_des.m_code;
		record[_T("��Ŀ����")] = info.m_des.m_name;
		pdb.setJBXXTableInfo(record);
	}
	acedRestoreStatusBar();
}

void FCSCreateCarPosition()
{
	IDataBaseOper oper;
	double m_Scale = 500;//����ϵ��
	read1(m_Scale);//��ȡ�����߷�ĸ
	m_Scale /= 1000;//��������ϵ��
	int iCtype = 1;//��Ȩʶ���룬Ĭ�ϵ���Ȩ
	TextProperty m_cw = oper.readTextPropery(_T("����������λ"));//��ȡ����������λ�������
	TextProperty m_dxcwzj = oper.readTextPropery(_T("�����������³�λע��"), true, true);//��ȡ�����������³�λע���������
	TextProperty m_dscwzj = oper.readTextPropery(_T("�����������ϳ�λע��"), true, true);//��ȡ�����������ϳ�λע���������
	TextProperty m_dxcwbh = oper.readTextPropery(_T("�����������³�λ���"), true, true);//��ȡ�����������³�λ����������
	TextProperty m_dscwbh = oper.readTextPropery(_T("�����������ϳ�λ���"), true, true);//��ȡ�����������ϳ�λ����������
	SelectFilter sf(8, _T("��λ"));//����λ��ͼ��ɸѡ����
	SelectFilter sfpl(RTDXF0, _T("*POLYLINE"));//����ɸѡ����
	AcDbObjectIdArray ids;
	TCHAR pr[4] = { 0 };//��Ȩʶ����
	if (!SelectEntitys(ids, sf, sfpl, _T("X"))) return;//ѡ��ǰDWG�ļ������С���λ��ͼ���е�ʵ��
	do
	{
		int es = ads_getint(_T("[1]:��һ��Ȩ��[2]:���Ȩ<1>"), &iCtype);//�û�����ָ������
		if (es == RTCAN) return;//����û�ȡ�����룬ֱ�ӷ���
		if (es != RTNORM)
		{
			acutPrintf(_T("\n�����쳣�����������룡"));
			iCtype = 0;
		}
	} while (iCtype != 1 && iCtype != 2);//�û���������ָ��������Ҫ��������
	_stprintf(pr, _T("%d"), iCtype);//�����Ͳ�Ȩʶ����ת��ΪUnicode�ַ�
	TCHAR szlc[20] = { 0 };//���ڴ洢���ϡ�����������Ϣ
	int szlctype = 1;//���ϡ����³�λʶ����
	_stprintf(szlc, _T("%s"), _T("����"));
	do
	{
		int es = ads_getint(_T("\n��λ���ڣ�[1]���ϡ�[2]����<1>"), &szlctype);
		if (es == RTCAN)return;
		if (es != RTNORM) 
		{
			acutPrintf(_T("\n�����쳣�����������룡"));
			szlctype = 0;
		}
	} while (szlctype != 1 && szlctype != 2);
	if (szlctype == 2) _stprintf(szlc, _T("%s"), _T("����"));//���ʶ����Ϊ2����ֵΪ�����¡�
	acedSetStatusBarProgressMeter(_T("���ڹ�����λ���Ժ�"), 0, ids.length());//��ʾ������λ������
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);//���ù�������
		AcDbObjectId id = ids[idx];//���õ�ǰ��λID��
		AcDbObjectIdArray tids = getInnerObjects(id);
		for(int indx = 0; indx < tids.length(); ++indx)
		{
			TCHAR info[25] = { 0 };
			AcDbObjectId cid = tids[indx];
			if(!GetEntLayer(cid, info)) continue;
			if(_tcscmp(info, _T("��λ���")) == 0)
			{
				GetEntText(aname(cid), info);
				AddXdata(aname(id), _T("CWBH"), 0, info);
				CString handle = GetHandleByObject(cid);
				AddXdata(aname(id), _T("CWBHJB"), 0, handle);
				if (_tcscmp(szlc, _T("����")) == 0)
				{
					setlayer(aname(cid), m_dscwbh._layer);
					setcolor(aname(cid), m_dscwbh._color);
					SetTextStyle(cid, m_dscwbh._font);
					modifyTextSize();
				}
				else
				{
					setlayer(aname(cid), m_dxcwbh._layer);
					setcolor(aname(cid), m_dxcwbh._color);
					SetTextStyle(cid, m_dxcwbh._font);
					modifyTextSize();
				}
			}
			else if(_tcscmp(info, _T("��λע��")) == 0)
			{
				GetEntText(aname(cid), info);
				AddXdata(aname(id), _T("CWZJ"), 0, info);
				CString handle = GetHandleByObject(cid);
				AddXdata(aname(id), _T("CWZJJB"), 0, handle);
				if (_tcscmp(szlc, _T("����")) == 0)
				{
					setlayer(aname(cid), m_dscwzj._layer);
					setcolor(aname(cid), m_dscwzj._color);
					SetTextStyle(cid, m_dscwzj._font);
				}
				else
				{
					setlayer(aname(cid), m_dxcwzj._layer);
					setcolor(aname(cid), m_dxcwzj._color);
					SetTextStyle(cid, m_dxcwzj._font);
				}
			}
			else continue;
		}
		AddXdata(aname(id), _T("CWLX"), 0, _T("��׼��λ"));
		AddXdata(aname(id), _T("CWSM"), 1, _T("1"));
		AddXdata(aname(id), _T("SZLC"), 0, szlc);
		setlayer(aname(id), m_cw._layer);
		if (_tcscmp(szlc, _T("����")) == 0)
			setcolor(aname(id), m_cw._color);
		else
			setcolor(aname(id), m_dxcwbh._color);
		AddXdata(aname(id), _T("PR"), 0, pr);//��ӳ�λ��Ȩ��Ϣ
	}
	acedRestoreStatusBar();
}

void FCSAddFunctionNote()
{

}

void FCSSplitLayer()
{
	HOUSE(House);
	House.splitLayer();
}

void FCSSetFont()
{
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	AcDbObjectIdArray ids; TCHAR type[255] = {0};
	if(!SelectEntitys(ids, _T("X"))) return;
	CString text(_T("text"));
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids[idx];
		GetEntityType(aname(id), type);
		if(text.CompareNoCase(type) != 0) continue;
		SetTextStyle(id, _T("����"));
	}
}

void FCSSetCarType()
{
	CCarInfo info;
	info.setCarType();
}

void FCSOpenDoor()
{

}

void FCSTiQingMianJi()
{
	HOUSE(house);
	house.tiQingMianJi();
}

void FCSDistAnnotate()
{
	HOUSE(house);
	house.distAnnotate();
}

void FCSCheckGongNengQu()
{
	HOUSE(house);
	house.checkGongNengQu();
}

void FCSDefineGongNengQu()
{
	HOUSE(house);
	house.defineGongNengQu();
}

void FCSNewSetLayerInfo()
{
	SEEKDOG(); HOUSE(house);
	house.newSetLayerInfo();
}

void FCSNewQualityCheck()
{
	SEEKDOG(); HOUSE(house);
	house.newQualityCheck();
}

void FCSGuiHuaBuYiZhiDWG()
{
	SEEKDOG(); HOUSE(house);
	house.ExportGuiHuaBuYiZhiDWG();
}

void FCSNewCheWeiStatistic()
{
	SEEKDOG(); HOUSE(house);
	house.newCheWeiStatistic();
}

void FCSNewCreaetFCPMT()
{
	SEEKDOG(); HOUSE(house);
	house.newCreateFCPMT();
}

void FCSNewSplitMargeLayer()
{
	SEEKDOG(); HOUSE(house);
	house.newSplitMargeLayer();
}

void FCSNewDrawUpPlanning()
{
	SEEKDOG(); HOUSE(house);
	house.newDrawUpPlanning();
}

void FCSNewOpenLayer()
{
	SEEKDOG(); ads_name ssName; long len = 0;
	if(RTNORM != ads_ssget(_T(""), NULL, NULL, NULL, ssName)) return;
	ads_sslength(ssName, &len); VStr layers;
	TCHAR layer[255] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		ads_name ent; ads_ssname(ssName, idx, ent);
		GetEntLayer(ent, layer);
		VSIter it = find(layers.begin(), layers.end(), layer);
		if(it == layers.end()) layers.push_back(layer);
	}
	LayerOperator layerOper;
	layerOper.opens(layers, false);
}

void FCSNewCloseLayer()
{
	SEEKDOG(); ads_name ssName; long len = 0;
	if(RTNORM != ads_ssget(_T(""), NULL, NULL, NULL, ssName)) return;
	ads_sslength(ssName, &len); VStr layers;
	TCHAR layer[255] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		ads_name ent; ads_ssname(ssName, idx, ent);
		GetEntLayer(ent, layer);
		VSIter it = find(layers.begin(), layers.end(), layer);
		if(it == layers.end()) layers.push_back(layer);
	}
	LayerOperator layerOper;
	layerOper.closes(layers, false);
}

void FCSNewOpenAllLayer()
{
	SEEKDOG();
	LayerOperator layerOper;
	layerOper.opens(VStr(), false);
}

XMJGReConfigurationDlg g_configurationDlg;

void FCSNewReConfigurationDlg()
{
	CAcModuleResourceOverride myResources;
	g_ReConfigurationDockBar.setChildDlg(&g_configurationDlg, XMJGReConfigurationDlg::IDD);
	if(g_ReConfigurationDockBar.GetSafeHwnd() != NULL)
	{
		if(g_ReConfigurationDockBar.IsVisible() == false)
			acedGetAcadFrame()->ShowControlBar(&g_ReConfigurationDockBar, TRUE, FALSE);
	}
	else
	{
		g_ReConfigurationDockBar.Create(acedGetAcadFrame(), _T(""));
		g_ReConfigurationDockBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT | CBRS_ALIGN_ANY | CBRS_BORDER_RIGHT);
		g_ReConfigurationDockBar.RestoreControlBar();
	}
}

void FCSNewGHSTYSGSTBYZ()
{
	SEEKDOG(); HOUSE(house);
	house.newGHSTYSGSTBYZ();
}

void FCSDrawDomain()
{
	CAcModuleResourceOverride myResources;
	g_DrawDomainDockControl.setChildDlg(&g_drawDomainDialog, DrawDomainDialog::IDD);
	if(g_DrawDomainDockControl.GetSafeHwnd() != NULL)
	{
		if(g_DrawDomainDockControl.IsVisible() == false)
			acedGetAcadFrame()->ShowControlBar(&g_DrawDomainDockControl, TRUE, FALSE);
	}
	else
	{
		g_DrawDomainDockControl.Create(acedGetAcadFrame(), _T(""));
		g_DrawDomainDockControl.EnableDocking(CBRS_ALIGN_TOP);
		g_DrawDomainDockControl.RestoreControlBar();
	}
	g_DrawDomainDockControl.RestoreControlBar(AFX_IDW_DOCKBAR_TOP);
}

void FCSHeightPropertySettingDlg()
{
	CHeightPropertySettingDlg dlg;
	dlg.DoModal();
}

void FCSModifyProjectRangeLineInfo()
{
	SelectFilter sf1(8, _T("XMFW"));
	SelectFilter sf2(5020, _T("*POLYLINE"));
	AcDbObjectId id; CString namecode;
	if(!SelectEntity(id, sf1, sf2, _T("X")))
	{
		MessageBox(NULL, _T("�����Ŀͼ�λ�������Ŀͼ�� ���Ƿ������Ŀ��Χ��"), _T("������Ϣ"), 0);
		return;
	}
	{
		IProjectMDB pdb; pdb.initMDB();
		MStr record = pdb.getJBXXTableInfo();
		CString pName = record[_T("��Ŀ����")];
		CString pCode = record[_T("��Ŀ���")];
		namecode.Format(_T("%s_%s"), pName, pCode);
	}
	CString sxh; bool isExist = false;
	{
		TCHAR path[255] = {0}; VMStr records;
		if(5100 != ads_findfile(_T("xmjg.mdb"), path)) return;
		CDatabaseOperManager dbManager;
		dbManager.initialConnect(path);
		dbManager.ReadDataBase_all(_T("SXH"), records);
		for(int idx = 0; idx < (int)records.size(); ++idx)
		{
			MStr record = records[idx];
			CString temp = record[_T("��Ŀ����")];
			if(temp.CompareNoCase(namecode) == 0)
			{
				isExist = true;
				sxh = record[_T("˳���")];
			}
		}
		if(sxh.IsEmpty())
			sxh.Format(_T("%d"), records.size() + 3);
		if(!isExist)
		{
			MStr record; record[_T("��Ŀ����")] = namecode;
			record[_T("˳���")] = sxh;
			dbManager.InsertRecord(_T("SXH"), record);
		}
	}

	AddXdata(aname(id), _T("SXH"), 1, sxh);

	CAcModuleResourceOverride myResources;
	InfomationOfProjectRangeLine dlg;
	dlg.setObject(id);
	dlg.DoModal();
}

// ��ͼʵ����ɫ���
void FCSEntityToWhite()
{
	IDataBaseOper dbOper;
	CString strLyr = dbOper.readCommonTable(_T("��ͼ��ɫ����ʵ��ͼ��"));
	CString strCode = dbOper.readCommonTable(_T("��ͼ��ɫ����ʵ�����"));
	vector<CString> vLyr, vCode;
	AnalyzeData(vLyr, strLyr, _T(";"));
	AnalyzeData(vCode, strCode, _T(";"));
	ads_name ssname;
	ads_ssget(_T("X"), NULL, NULL, NULL, ssname);
	long len = 0;
	ads_sslength(ssname, &len);
	long i = 0;
	for(i = 0; i < len; ++i)
	{
		ads_name ent;
		ads_ssname(ssname, i, ent);
		AcDbObjectId id;
		acdbGetObjectId(id, ent);
		TCHAR szLyr[255] = {0}, szCode[20] = {0};
		GetEntLayer(ent, szLyr);
		GetStdm(ent, szCode);
		// �ų����õ�ͼ��ͱ���
		if(FindString(vLyr, szLyr) != -1) continue;
		if(FindString(vCode, szCode) != -1) continue;
		TCHAR szType[255] = {0};
		GetEntityType(ent, szType);
		if(0 == _tcsicmp(szType, _T("INSERT")))
		{
			ChangeBlockRefenceColor(id, 7);
		}
		else
		{
			setcolor(ent, 7);
		}
	}
	ads_ssfree(ssname);
	sendCommand(_T("regen "));
}

#include "dbxline.h"
void FCSCreateMidLine()
{
	ads_point pt;
	ads_name lineEnt1, lineEnt2;
	int es = ads_entsel(_T("\nѡ���һ���ߣ�"), lineEnt1, pt);
	if(es == RTCAN) return;
	TCHAR szEntType[255] = {0};
	GetEntityType(lineEnt1, szEntType);
	if(0 != _tcsicmp(szEntType, _T("line")))
	{
		acutPrintf(_T("\nѡ����󣬲���ֱ�ߣ�"));
		return;
	}

	es = ads_entsel(_T("\nѡ��ڶ����ߣ�"), lineEnt2, pt);
	GetEntityType(lineEnt2, szEntType);
	if(es == RTCAN) return;
	if(0 != _tcsicmp(szEntType, _T("line")))
	{
		acutPrintf(_T("\nѡ����󣬲���ֱ�ߣ�"));
		return;
	}
	ads_point pt11, pt12;
	ads_point pt21, pt22;
	GetLinePoint(lineEnt1, pt11, pt12);
	GetLinePoint(lineEnt2, pt21, pt22);
	bool bH1 = true, bH2 = true;		// ����������Ϊˮƽ���жϷ����ж������ǱȽ�x������y������ĸ����ĸ�������Ǹ�����
	if(fabs(pt11[X] - pt12[X]) < fabs(pt11[Y] - pt12[Y]))
	{
		bH1 = false;
	}
	if(fabs(pt21[X] - pt22[X]) < fabs(pt21[Y] - pt22[Y]))
	{
		bH2 = false;
	}
	// ����жϣ����������ͬ��˵���޷�������
	if(bH1 ^ bH2)
	{
		acutPrintf(_T("\n�����߷���ͬ���޷������ߣ�"));
		return;
	}
	ads_point ptmid1, ptmid2;	// �����ߵ�������
	if(bH1)
	{
		ads_point ptl1, ptl2, ptr1, ptr2;
		if(pt11[X] < pt12[X])
		{
			ads_point_set(pt11, ptl1);
			ads_point_set(pt12, ptr1);
		}
		else
		{
			ads_point_set(pt12, ptl1);
			ads_point_set(pt11, ptr1);
		}
		if(pt21[X] < pt22[X])
		{
			ads_point_set(pt21, ptl2);
			ads_point_set(pt22, ptr2);
		}
		else
		{
			ads_point_set(pt22, ptl2);
			ads_point_set(pt21, ptr2);
		}
		ptmid1[X] = (ptl1[X] + ptl2[X]) / 2;
		ptmid1[Y] = (ptl1[Y] + ptl2[Y]) / 2;

		ptmid2[X] = (ptr1[X] + ptr2[X]) / 2;
		ptmid2[Y] = (ptr1[Y] + ptr2[Y]) / 2;
	}
	else
	{
		ads_point ptup1, ptup2, ptdown1, ptdown2;
		if(pt11[Y] > pt12[Y])
		{
			ads_point_set(pt11, ptup1);
			ads_point_set(pt12, ptdown1);
		}
		else
		{
			ads_point_set(pt12, ptup1);
			ads_point_set(pt11, ptdown1);
		}
		if(pt21[Y] > pt22[Y])
		{
			ads_point_set(pt21, ptup2);
			ads_point_set(pt22, ptdown2);
		}
		else
		{
			ads_point_set(pt22, ptup2);
			ads_point_set(pt21, ptdown2);
		}
		ptmid1[X] = (ptup1[X] + ptup2[X]) / 2;
		ptmid1[Y] = (ptup1[Y] + ptup2[Y]) / 2;

		ptmid2[X] = (ptdown1[X] + ptdown2[X]) / 2;
		ptmid2[Y] = (ptdown1[Y] + ptdown2[Y]) / 2;
	}
	AcDbObjectId lineId = drawline(ptmid1, ptmid2, _T(""));
	AcDbXline xline;
	xline.setBasePoint(asPnt3d(ptmid1));
	AcGeVector3d vec3d(ptmid2[X] - ptmid1[X], ptmid2[Y] - ptmid1[Y], 0);
	xline.setUnitDir(vec3d);

	ads_point pt1, pt2;
	AcGePoint3d closePt3d;
	es = ads_getpoint(ptmid1, _T("\n������һ�㣺<�س���ո�����>"), pt1);
	if(es == RTCAN)
	{
		return;
	}
	if(es != RTNORM)
	{
		ads_point_set(ptmid1, pt1);
	}
	else
	{
		xline.getClosestPointTo(asPnt3d(pt1), closePt3d);
		ads_point_set(asDblArray(closePt3d), pt1);
	}
	SetLinePoint(aname(lineId), pt1, ptmid2);
	es = ads_getpoint(ptmid2, _T("\n�����ڶ��㣺<�س���ո�����>"), pt2);
	if(es == RTCAN)
	{
		return;
	}
	if(es != RTNORM)
	{
		ads_point_set(ptmid2, pt2);
	}
	else
	{
		xline.getClosestPointTo(asPnt3d(pt2), closePt3d);
		ads_point_set(asDblArray(closePt3d), pt2);
	}
	SetLinePoint(aname(lineId), pt1, pt2);
}