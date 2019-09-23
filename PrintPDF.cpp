#include "stdafx.h"
#include "Global.h"
#include "PrintPDF.h"


CPrintPDF::CPrintPDF()
	: m_str_size_name(_T(""))
{
	UpdatePrinterConfig();
	m_str_divice = _T("DWG To PDF.pc3");
}


CPrintPDF::~CPrintPDF()
{
}

bool CPrintPDF::GetPlotSizeName(AcDbPlotSettingsValidator* pPSV, AcDbPlotSettings* pPlotSettings, CString& real_size_name)
{
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager();
	AcDbObjectId btrId = pLayoutManager->getActiveLayoutBTRId();
	AcDbBlockTableRecord *pBTR;
	Acad::ErrorStatus es = acdbOpenObject(pBTR, btrId, AcDb::kForRead);
	AcDbObjectId layoutId = pBTR->getLayoutId();
	AcDbLayout *pLayout = NULL;
	acdbOpenObject(pLayout, layoutId, AcDb::kForRead);
	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	pBTR->close();
	es = pPSV->setPlotCfgName(pPlotSettings, m_str_divice);
	pPSV->refreshLists(pPlotSettings);
	AcArray<const TCHAR*> MediaList;
	pPSV->canonicalMediaNameList(pPlotSettings, MediaList);

	for(int i = 0; i < MediaList.length(); i++)
	{
		const TCHAR *pLocaleName;
		pPSV->getLocaleMediaName(pPlotSettings, MediaList.at(i), pLocaleName);
		if(0 == _tcsicmp(pLocaleName, m_str_size_name))
		{
			real_size_name.Format(_T("%s"), MediaList[i]);
			return true;
		}
	}
	return false;
}

void CPrintPDF::Plot()
{
	struct resbuf rb;
	rb.restype = 5003;
	rb.resval.rint = 0;
	ads_setvar(_T("BACKGROUNDPLOT"), &rb);
	es_count = 0;
	AcDbPlotSettingsValidator* pPSV = NULL;
	AcDbPlotSettings* pPlotSettings = NULL;
	pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
	pPlotSettings = new AcDbPlotSettings(true);
	CString real_size_name;
	if(!GetPlotSizeName(pPSV, pPlotSettings, real_size_name))
	{
		CString str;
		str.Format(_T("未找到名为\"%s\"的打印尺寸，请在打印设置中设置"), m_str_size_name);
		if(pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
		AfxMessageBox(str);
		return;
	}
	Acad::ErrorStatus es;
	AcAxDocLock lock;
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager();
	AcDbObjectId btrId = pLayoutManager->getActiveLayoutBTRId();
	AcDbBlockTableRecord *pBTR;
	es = acdbOpenObject(pBTR, btrId, AcDb::kForRead);
	
	AcDbObjectId layoutId = pBTR->getLayoutId();
	AcDbLayout *pLayout = NULL;
	es = acdbOpenObject(pLayout, layoutId, AcDb::kForRead);

	es = pPlotSettings->copyFrom(pLayout);
	es = pLayout->close();
	es = pBTR->close();
	pPlotSettings->setPlotHidden(false);
	pPlotSettings->setPlotPlotStyles(Adesk::kFalse);
	pPlotSettings->setPrintLineweights(Adesk::kFalse);
	es = pPlotSettings->setShadePlotResLevel(AcDbPlotSettings::kMaximum);
	es = pPSV->setPlotRotation(pPlotSettings, AcDbPlotSettings::k0degrees);
	es = pPSV->setPlotCfgName(pPlotSettings, m_str_divice);
	pPSV->refreshLists(pPlotSettings);
	es = pPSV->setUseStandardScale(pPlotSettings, Adesk::kTrue);
	es = pPSV->setStdScaleType(pPlotSettings, AcDbPlotSettings::kScaleToFit);
	es = pPSV->setCurrentStyleSheet(pPlotSettings, _T("acad.ctb"));

	vector<AcGePoint2dArray> v_extents;
	AcGePoint2dArray nodes, extents;
	ads_name ssSave;
	IdArrToSS(m_objArrSave, ssSave);
	if(!GetEntsExtent(ssSave, nodes))
	{
		AfxMessageBox(_T("没有找到宗地图框！"));
		if(pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
		return;
	}
	acedSSFree(ssSave);

	if(false == IsDeasil(nodes)) extents.reverse();
	if(Poly2dArrayOffset(nodes, 0.5, true, extents) == false)
	{
		extents.setLogicalLength(0);
		extents.append(nodes);
		AfxMessageBox(_T("外扩失败"));
		acutPrintf(_T("\noff failed"));
	}
	es = pPSV->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);
	es = pPSV->setPlotWindowArea(pPlotSettings, extents.at(0)[X], extents.at(0)[Y], extents.at(2)[X], extents.at(2)[Y]);
	es = pPSV->setPlotCentered(pPlotSettings, true);
	const TCHAR* p = (LPCTSTR)real_size_name;
	es = pPSV->setCanonicalMediaName(pPlotSettings, p);
	double numerator, denominator;
	es = pPlotSettings->getCustomPrintScale(numerator, denominator);
	double m_Scale = numerator / denominator;
	es = pPSV->setPlotCentered(pPlotSettings, true);
	AcPlPlotEngine* pEngine = NULL;
	if(Acad::eOk == AcPlPlotFactory::createPublishEngine(pEngine))
	{
		// Here is the progress dialog for the current plot process...
		AcPlPlotProgressDialog *pPlotProgDlg = acplCreatePlotProgressDialog(acedGetAcadFrame()->m_hWnd, false, 1);

		bool flag = pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kDialogTitle, _T("打印"));
		flag = pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelJobBtnMsg, _T("取消任务"));
		flag = pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelSheetBtnMsg, _T("取消图纸"));
		flag = pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetSetProgressCaption, _T("任务进度"));
		flag = pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetProgressCaption, _T("图纸进度"));
		pPlotProgDlg->setPlotProgressRange(0, 100);
		flag = pPlotProgDlg->onBeginPlot();
		flag = pPlotProgDlg->setIsVisible(true);
		es = pEngine->beginPlot(pPlotProgDlg);
		AcPlPlotPageInfo pageInfo;
		AcPlPlotInfo plotInfo; // Used to describe how the plot is to be made
		plotInfo.setLayout(layoutId);// This is required.
		plotInfo.setOverrideSettings(pPlotSettings);
		AcPlPlotInfoValidator validator;
		validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
		es = validator.validate(plotInfo);
		const TCHAR *szDocName = acDocManager->curDocument()->fileName();
		es = pEngine->beginDocument(plotInfo, szDocName, NULL, 1, true, m_strTempSavePath);
		flag = pPlotProgDlg->onBeginSheet();
		pPlotProgDlg->setSheetProgressRange(0, 100);
		pPlotProgDlg->setSheetProgressPos(0);

		es = pEngine->beginPage(pageInfo, plotInfo, true);
		es = pEngine->beginGenerateGraphics();
		es = pEngine->endGenerateGraphics();
		es = pEngine->endPage();
		pPlotProgDlg->setSheetProgressPos(100);
		flag = pPlotProgDlg->onEndSheet();
		pPlotProgDlg->setPlotProgressPos(100);
		es = pEngine->endDocument();
		es = pEngine->endPlot();
		Sleep(1000);
		pEngine->destroy();
		pEngine = NULL;
		pPlotProgDlg->destroy();
		if(pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
	}
	else
	{
		AfxMessageBox(_T("正在进行其他打印任务，请完成后重试！"));
		return;
	}
}

bool CPrintPDF::setPrintSize()
{
	if(m_str_size_name.CompareNoCase(_T("A4一半")) == 0
		|| m_str_size_name.CompareNoCase(_T("A4竖")) == 0
		|| m_str_size_name.CompareNoCase(_T("A3横")) == 0)
		return true;
	int type = 1;
	if(RTCAN == ads_getint(_T("\n 请选择打印的大小<1>A4一半、<2>A4、<3>A3:"), &type)) return false;
	if(type == 1)
		m_str_size_name = _T("A4一半");
	else if(type == 2)
		m_str_size_name = _T("A4竖");
	else if(type == 3)
		m_str_size_name = _T("A3横");
	return true;
}