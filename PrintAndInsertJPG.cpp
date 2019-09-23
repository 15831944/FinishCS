#include "stdafx.h"
#include "PrintAndInsertJPG.h"
#include "axlock.h"
#include "dbplotsettings.h"
#include "dbplotsetval.h"
#include "AcPlPlotEngine.h"
#include "AcPlPlotFactory.h"
#include "AcPlPlotProgress.h"
#include "AcPlPlotInfo.h"
#include "AcPlPlotInfoValidator.h"
#include "AcDbLMgr.h"
#include "gdal_priv.h"
#include <string.h>

CPrintAndInsertJPG::CPrintAndInsertJPG(void)
{
	m_str_divice = _T("PublishToWeb JPG.pc3");
	m_str_size_name = _T("A4竖");
}


CPrintAndInsertJPG::~CPrintAndInsertJPG(void)
{

}

bool CPrintAndInsertJPG::GetPlotSizeName(AcDbPlotSettingsValidator* pPSV, AcDbPlotSettings* pPlotSettings, CString& real_size_name)
{
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); 
	AcDbObjectId btrId=pLayoutManager->getActiveLayoutBTRId();
	AcDbBlockTableRecord *pBTR;
	Acad::ErrorStatus es=acdbOpenObject(pBTR,btrId,AcDb::kForRead);
	AcDbObjectId layoutId=pBTR->getLayoutId();
	AcDbLayout *pLayout=NULL;
	acdbOpenObject(pLayout,layoutId,AcDb::kForRead);
	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	pBTR->close();

	es=pPSV->setPlotCfgName(pPlotSettings, m_str_divice);
	pPSV->refreshLists(pPlotSettings);
	AcArray<const TCHAR*> MediaList;
	pPSV->canonicalMediaNameList(pPlotSettings,MediaList);

	for(int i=0; i<MediaList.length(); i++)
	{
		const TCHAR *pLocaleName;
		pPSV->getLocaleMediaName(pPlotSettings,MediaList.at(i),pLocaleName);
		if (0 == _tcsicmp(pLocaleName, m_str_size_name))
		{
			real_size_name.Format(_T("%s"), MediaList[i]);
			return true;
		}
	}
	return false;
}

void CPrintAndInsertJPG::Plot(AcDbObjectIdArray m_objArrSave)
{
	// 必须设置后台打印否则速度奇慢
	struct resbuf rb;
	rb.restype = 5003;
	rb.resval.rint = 0;
	ads_setvar(_T("BACKGROUNDPLOT"),&rb); 
#if  _MSC_VER == 1500
	//ChangeCassappInLoadFile();
#endif
	es_count = 0;
	AcDbPlotSettingsValidator* pPSV = NULL;
	AcDbPlotSettings* pPlotSettings = NULL;
	pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
	pPlotSettings = new AcDbPlotSettings(true);
	CString real_size_name;
	if (!GetPlotSizeName(pPSV, pPlotSettings, real_size_name))
	{
		CString str;
		str.Format(_T("未找到名为\"%s\"的打印尺寸，请在打印设置中设置"), m_str_size_name);
		if (pPlotSettings)
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
	// We want to instantiate our custom AcDbPlotSettings object, and inherit the 
	// properties from the active layout.
	AcDbObjectId layoutId = pBTR->getLayoutId();
	AcDbLayout *pLayout = NULL;
	es = acdbOpenObject(pLayout, layoutId, AcDb::kForRead);

	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	pBTR->close();
	pPlotSettings->setPlotHidden(false);
	es = pPSV->setPlotRotation(pPlotSettings, AcDbPlotSettings::k0degrees);
	//设置当前打印机
	es = pPSV->setPlotCfgName(pPlotSettings, m_str_divice);
	pPSV->refreshLists(pPlotSettings);
	es = pPSV->setUseStandardScale(pPlotSettings, Adesk::kTrue);
	es = pPSV->setStdScaleType(pPlotSettings, AcDbPlotSettings::kScaleToFit);
	vector<AcGePoint2dArray> v_extents;
	AcGePoint2dArray nodes , extents;
	ads_name ssSave;
	IdArrToSS(m_objArrSave , ssSave);
	if (!GetEntsExtent(ssSave , nodes))
	{
		AfxMessageBox(_T("没有找到宗地图框！"));
		if (pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
		return;
	}
	acedSSFree(ssSave);
	if ( false == IsDeasil(nodes) ) extents.reverse();
	if(Poly2dArrayOffset(nodes , 7, true, extents) == false)
	{
		extents.setLogicalLength( 0 );
		extents.append( nodes );
		AfxMessageBox(_T("外扩失败"));
		acutPrintf(_T("\noff failed"));
	}
	es = pPSV->setPlotWindowArea(pPlotSettings, extents.at(0)[X], extents.at(0)[Y], extents.at(2)[X], extents.at(2)[Y]);
	es = pPSV->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);
	es = pPSV->setPlotCentered(pPlotSettings, true);
	const TCHAR* p = (LPCTSTR)real_size_name;
	pPlotSettings->setPlotPlotStyles(Adesk::kTrue);
	pPlotSettings->setPrintLineweights(Adesk::kTrue);
	es = pPSV->setCanonicalMediaName(pPlotSettings, p);
	double numerator, denominator;
	es = pPlotSettings->getCustomPrintScale(numerator, denominator);
	double m_Scale = numerator / denominator;
	//开始打印
	es = pPSV->setPlotCentered(pPlotSettings, true);
	// Create a publish engine, and send the plot through...
	AcPlPlotEngine* pEngine = NULL;
	if (Acad::eOk == AcPlPlotFactory::createPublishEngine(pEngine))
	{
		// Here is the progress dialog for the current plot process...
		AcPlPlotProgressDialog *pPlotProgDlg = acplCreatePlotProgressDialog(acedGetAcadFrame()->m_hWnd, false, 1);

		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kDialogTitle, _T("打印"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelJobBtnMsg, _T("取消任务"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelSheetBtnMsg, _T("取消图纸"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetSetProgressCaption, _T("任务进度"));
		pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetProgressCaption, _T("图纸进度"));

		pPlotProgDlg->setPlotProgressRange(0, 100);
		pPlotProgDlg->onBeginPlot();
		pPlotProgDlg->setIsVisible(true);

		es = pEngine->beginPlot(pPlotProgDlg);
		//acadErrorStatusTextChs(es);
		AcPlPlotPageInfo pageInfo;
		AcPlPlotInfo plotInfo; // Used to describe how the plot is to be made

		// First, set the layout to the specified layout (which is the current layout in this sample)
		plotInfo.setLayout(layoutId);// This is required.

		// Now, override the layout settings with the plot settings we have been populating
		plotInfo.setOverrideSettings(pPlotSettings);

		// We need to validate these settings.
		AcPlPlotInfoValidator validator;
		validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
		es = validator.validate(plotInfo);
		CString str_dwg_pathname;
		str_dwg_pathname.Append(_T("D:\\123.jpg"));
		TCHAR jpg_pathname[MAX_PATH] = { 0 };
		_tcscpy(jpg_pathname, str_dwg_pathname);
		const TCHAR *szDocName = acDocManager->curDocument()->fileName();
		es = pEngine->beginDocument(plotInfo, szDocName, NULL, 1, true, jpg_pathname);
		pPlotProgDlg->onBeginSheet();
		pPlotProgDlg->setSheetProgressRange(0, 100);
		pPlotProgDlg->setSheetProgressPos(0);

		es = pEngine->beginPage(pageInfo, plotInfo, true);
		es = pEngine->beginGenerateGraphics();
		es = pEngine->endGenerateGraphics();
		es = pEngine->endPage();
		pPlotProgDlg->setSheetProgressPos(100);
		pPlotProgDlg->onEndSheet();
		pPlotProgDlg->setPlotProgressPos(100);
		es = pEngine->endDocument();
		//acadErrorStatusTextChs(es);
		es = pEngine->endPlot();
		//AfxMessageBox(_T("Plot endPlot"));
		//acadErrorStatusTextChs(es);
		// Destroy the engine 
		pEngine->destroy();
		pEngine = NULL;
		// and the progress dialog.
		pPlotProgDlg->destroy();
		if (pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}

		char *path = new char[200];
		char *path1 = new char[200];
		TCHAR tpath[200] = {0};
		_stprintf(tpath, _T("%s"), str_dwg_pathname);
		TcharToChar(tpath, path);
		_stprintf(tpath, _T("%s"), _T("D:\\213.jpg"));
		TcharToChar(tpath, path1);
		rescale_image_jpeg(path, path1, 255, 255, 255, 5346, 7560);
	}
	else
	{
		AfxMessageBox(_T("正在进行其他打印任务，请完成后重试！"));
		return;
	}
#if  _MSC_VER == 1500
	//RestoreCassappInLoadFile();
#endif

}


bool CPrintAndInsertJPG::insertZDCT(CTinyWord &m_tinyword)
{
	msword::Range range;

	TCHAR szTempName[255]={0};
	TCHAR szTempPath[255]={0};
	CString strTempPath;
	_tcscpy(szTempName,_T("宗地草图.jpg"));
	if (acedFindFile(szTempName,szTempPath) != RTNORM)
	{
		TCHAR szTemp[255]={0};
		_stprintf(szTemp,_T("\n 没有找到宗地草图.jpg:%s"),szTempName);
		acutPrintf(szTemp);
		return false;
	}
	strTempPath.Format(_T("%s") , szTempPath);

	if(m_tinyword.FindWord(_T("#宗地草图#"), range))
	{
		if(IsPathFileExists(strTempPath) == false)
		{
			acutPrintf(_T("\n 路径下找不到宗地草图图片！%s"), strTempPath);
			return false;
		}

		m_tinyword.FindWord(_T("#宗地草图#"), _T(""));
		m_tinyword.InsertPicture(range, strTempPath);
	}
	return true;
}

std::string getFilePath(const std::string &pathname)
{
	int n = pathname.find_last_of('/');
	int m = pathname.find_last_of('\\');
	int c = m > n ? m + 1 : n + 1;
	std::string str = pathname.substr(0, c);
	return str;
}

std::string getFileName(const std::string &pathname)
{
	int n = pathname.find_last_of('/');
	int m = pathname.find_last_of('\\');
	int c = m > n ? m + 1 : n + 1;
	std::string str = pathname.substr(c, pathname.size() - c);
	return str;
}

std::string getFileTitle(const std::string &pathname)
{
	std::string filename = getFileName(pathname);
	int n = filename.find_last_of('.');
	std::string str = filename.substr(0, n);
	return str;
}

std::string getFileExt(const std::string &pathname)
{
	std::string filename = getFileName(pathname);
	int n = filename.find_last_of('.') + 1;
	std::string str;
	if(n)
	{
		str = filename.substr(n, filename.size() - n);
	}
	else
	{
		str = "";
	}
	return str;
}

int rescale_image_jpeg(const char* in_file_name,
	const char* out_file_name,
	const unsigned char &background_r,
	const unsigned char &background_g,
	const unsigned char &background_b,
	const long &rescale_width,
	const long &rescale_height,
	void *callback)
{
	GDALAllRegister();
	GDALDriverManager *driver_manager = GetGDALDriverManager();
	////////////////获取输出Dirver///////////////////////////////
	std::string in_ext = getFileExt(out_file_name);
	GDALDriver *out_driver;
	if(!_stricmp(in_ext.c_str(), "jpg"))
	{
		out_driver = driver_manager->GetDriverByName("JPEG");
	}
	else if(!_stricmp(in_ext.c_str(), "png"))
	{
		out_driver = driver_manager->GetDriverByName("PNG");
	}

	///////////获取MEM Driver////////////////////////////////
	GDALDriver *mem_driver = NULL;
	mem_driver = driver_manager->GetDriverByName("MEM");
	if(NULL == mem_driver || NULL == out_driver)
	{
		return false;
	}

	///////////////////////////打开输入影像////////////////////////////////////////
	GDALDataset *in_dataset = (GDALDataset *)GDALOpen(in_file_name, GA_ReadOnly);

	if(!in_dataset)
	{
		return 1;
	}

	int channel_count = in_dataset->GetRasterCount();
	int x_size = in_dataset->GetRasterXSize();
	int y_size = in_dataset->GetRasterYSize();

	double scale_x = (double)x_size / rescale_width;
	double scale_y = (double)y_size / rescale_height;

	int read_x = (int)scale_x;
	int read_y = (int)scale_y;

	unsigned char *in_data_space = new unsigned char[x_size*read_y*3];
	unsigned char *out_data_space = new unsigned char[rescale_width*rescale_height*3];

	double x_counter = 0;
	double y_counter = 0;

	int out_counter = 0;

	unsigned char *data_start_h;
	unsigned char *data_start_w;

	int band_map[3];
	band_map[0] = 1;
	band_map[1] = 2;
	band_map[2] = 3;

	GDALDataset *mem_dataset = mem_driver->Create("", rescale_width, rescale_height, 3, GDT_Byte, NULL);

	for(int i = 0; i < rescale_height; i++)
	{
		x_counter = 0;
		CPLErr read_error = in_dataset->RasterIO(GF_Read, 0, (int)(y_counter), x_size, read_y, in_data_space, x_size, read_y, GDT_Byte, 3, band_map, 3, x_size * 3, 1);
		data_start_h = in_data_space;
		data_start_w = data_start_h;
		for(int s = 0; s < rescale_width; s++)
		{
			out_data_space[out_counter] = data_start_w[0];
			out_data_space[out_counter + 1] = data_start_w[1];
			out_data_space[out_counter + 2] = data_start_w[2];

			for(int t = 0; t < read_y; t++)
			{
				for(int u = 0; u < read_x; u++)
				{
					if(background_r != data_start_w[u * 3] || background_g != data_start_w[u * 3 + 1] || background_b != data_start_w[u * 3 + 2])
					{
						out_data_space[out_counter] = data_start_w[u * 3];
						out_data_space[out_counter+1] = data_start_w[u * 3 + 1];
						out_data_space[out_counter+2] = data_start_w[u * 3 + 2];
					}
				}
				data_start_w += x_size * 3;
			}
			x_counter += scale_x;
			data_start_w = data_start_h + (int)(x_counter)* 3;
			out_counter += 3;
		}
		y_counter += scale_y;

	}

	CPLErr write_error = mem_dataset->RasterIO(GF_Write, 0, 0, rescale_width, rescale_height, out_data_space, rescale_width, rescale_height, GDT_Byte, 3, band_map, 3, rescale_width * 3, 1);

	out_driver->CreateCopy(out_file_name, mem_dataset, true, 0, 0, 0);
	return true;
}
