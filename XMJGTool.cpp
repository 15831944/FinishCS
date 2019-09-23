#include "stdafx.h"

#ifndef XMJGTOOL_H
#include "XMJGTool.h"
#endif /*XMJGTOOL_H*/

#ifndef CODE_H
#include <tcode.h>
#endif /*CODE_H*/

#ifndef XMJGEXPORTDLG
#include "XMJGDialog.h"
#endif /*XMJGEXPORTDLG*/

#ifndef __SMARTTABLELUA_H__
#include "smarttable_lua.h"
#endif /*__SMARTTABLELUA_H__*/

#include "XRecordOperate.h"
#include <SBreakBlock.h>
#include <SaveBlocks.h>
#include <dbplotsettings.h>
#include <AcPlPlotEngine.h>
#include <atlpath.h>
#include <axlock.h>
#include <AcDbLMgr.h>
#include <dbsymtb.h>
#include <dbplotsetval.h>
#include <AcPlPlotProgress.h>
#include <AcPlPlotInfoValidator.h>
#include <AcPlPlotFactory.h>
#include <AutoGrid.h>
#include "StringsOpe.h"
#include <apoint.hpp>
#include "Global.h"
#include <acadi.h>
#include "gdal_priv.h"
#include "gdal.h"
#include "PipeTree.h"
#include "Dialogs.h"
#include "Tukuang_Common.h"
#include "FilePathOperater.h"
#include "dblead.h"

extern FieldChangeTable g_fieldchagetable;
void XMJGReplaceField(MStr &field, SBreakBlock *block)
{
	MSIter it = field.begin();
	for(; it != field.end(); ++it)
		block->replaceTextEx(it->first, it->second, true);
}

void XMJGControlPoint::DrawControlPoint()
{
	XMJGControlPointDlg *gp = new XMJGControlPointDlg;
	if(gp->DoModal() != IDOK) return;
	m_ControlPointType = gp->getControlPointType();
	m_ControlPointRank = gp->getControlPointRank();
	while(1)
	{
		ads_point textPt; TCHAR name[200] = {0}; TCHAR elevation[200] = {0}; double gc = 0.00;
		if(RTNORM != ads_getpoint(NULL, _T("\n 请输入或选择点坐标："), textPt)) return;
		if(RTNORM != ads_getstring(0, _T("\n 请输入点名："), name)) return;
		if(RTNORM != ads_getreal(_T("\n 请输入高程："), &gc)) return;
		AcDbObjectId id = DrawInsert(apoint(textPt), _T("ControlPoint"));
		textPt[X] += 1.5;
		DrawText(textPt, name, _T("ML"), _T(""), 2.0);
		_stprintf(elevation, _T("%.2lf"), gc);
		AddControlPointProperty(id, name, elevation);
	}
}

void XMJGControlPoint::AddControlPointProperty(AcDbObjectId &id, const CString &name, const CString &elevation)
{
	AddXdata(aname(id), _T("点名"), 0, name);
	AddXdata(aname(id), _T("高程"), 0, elevation);
	AddXdata(aname(id), _T("等级"), 0, m_ControlPointRank);
	AddXdata(aname(id), _T("类型"), 0, m_ControlPointType);
	setlayer(aname(id), _T("控制点"));
}

CLockManager::CLockManager(void)
{
	if(curDoc() != NULL)
		acDocManager->lockDocument(curDoc());//锁定文档
}

CLockManager::~CLockManager(void)
{
	if(curDoc() != NULL)
		acDocManager->unlockDocument(curDoc());//解锁文档
}

CRollBackManager::CRollBackManager(void)
{
	actrTransactionManager->startTransaction();//开始
	m_bCommit = false;
}

CRollBackManager::~CRollBackManager(void)
{
	if(m_bCommit == false)
		actrTransactionManager->abortTransaction();//取消
}

Acad::ErrorStatus CRollBackManager::Commit()
{
	if(m_bCommit == false)
	{
		m_bCommit = true;
		return actrTransactionManager->endTransaction();
	}
	return Acad::eOk;
}

Acad::ErrorStatus CRollBackManager::Abort()
{
	if(m_bCommit == false)
	{
		m_bCommit = true;
		return actrTransactionManager->abortTransaction();
	}
	return Acad::eOk;
}

XMJGPath::XMJGPath()
{

}

XMJGPath::XMJGPath(const TCHAR *savePath, const TCHAR *configPath, const TCHAR *picturePath)
{
	m_SavePath.Format(_T("%s"), savePath);
	m_ConfigPath.Format(_T("%s"), configPath);
	m_PicturePath.Format(_T("%s"), picturePath);
}

XMJGPath::~XMJGPath()
{

}

void XMJGPath::setSavePath(const TCHAR *savePath)
{
	m_SavePath.Format(_T("%s"), savePath);
}

void XMJGPath::setSavePath(const CString &savePath)
{
	m_SavePath.Format(_T("%s"), savePath);
}

string XMJGPath::getSavePath() const
{
	char save[255] = {0};
	TcharToChar(m_SavePath, save);
	string ret(save); return ret;
}

void XMJGPath::setConfigPath(const TCHAR *configPath)
{
	m_ConfigPath.Format(_T("%s"), configPath);
}

void XMJGPath::setConfigPath(const CString &configPath)
{
	m_ConfigPath.Format(_T("%s"), configPath);
}

CString XMJGPath::getConfigPath() const
{
	return m_ConfigPath;
}

void XMJGPath::setPicturePath(const TCHAR *picturePath)
{
	m_PicturePath.Format(_T("%s"), picturePath);
}

void XMJGPath::setPicturePath(const CString &picturePath)
{
	m_PicturePath.Format(_T("%s"), picturePath);
}

string XMJGPath::getPicturePath() const
{
	char pic[255] = {0};
	TcharToChar(m_PicturePath, pic);
	string ret(pic);	return ret;
}

XMJGColor::XMJGColor()
{

}

XMJGColor::~XMJGColor()
{

}

void XMJGColor::setColorRed(const byte &colorRed)
{
	m_ColorRed = colorRed;
}

void XMJGColor::setColorGreen(const byte &colorGreen)
{
	m_ColorGreen = colorGreen;
}

void XMJGColor::setColorBlue(const byte &colorBlue)
{
	m_ColorBlue = colorBlue;
}

byte XMJGColor::getColorRed() const
{
	return m_ColorRed;
}

byte XMJGColor::getColorGreen() const
{
	return m_ColorGreen;
}

byte XMJGColor::getColorBlue() const
{
	return m_ColorBlue;
}

XMJGPrintPicture::XMJGPrintPicture()
	: m_SizeName(_T(""))
{
	read1(m_Scale);
	m_Scale /= 1000;
	UpdatePrinterConfig();
	m_PicturePath = new XMJGPath;
}

XMJGPrintPicture::~XMJGPrintPicture()
{

}

void XMJGPrintPicture::setPrintPath(const CString &savePath)
{
	CString tempPath;
	int pos = savePath.Find(_T("."));
	if(pos == -1)
		tempPath.Format(_T("%s.jpg"), savePath);
	else
		tempPath.Format(_T("%s.jpg"), savePath.Left(pos));
	m_PicturePath->setPicturePath(tempPath);
	m_PicturePath->setConfigPath(tempPath);
	m_PicturePath->setSavePath(savePath);
}

bool XMJGPrintPicture::setPrintFormat(const RasterFormat &format)
{
	if(format == Raster_Jpg)
	{
		m_PrintDevice = _T("CASS_RASTER_JPG.pc3");
		m_pnpName = _T("CASS_RASTER_JPG.pmp");
	}
	else if(format == Raster_Bmp)
	{
		m_PrintDevice = _T("CASS_RASTER_BMP.pc3");
		m_pnpName = _T("CASS_RASTER_BMP.pmp");
	}
	else
		return false;
	return true;
}

void XMJGPrintPicture::setObjectArray(const AcDbObjectIdArray &ids)
{
	m_ObjectArray.setLogicalLength(0);
	m_ObjectArray.setPhysicalLength(0);
	m_ObjectArray.append(ids);
}

bool XMJGPrintPicture::BeginPrintPicture()
{
	setBackgroundPrint();
	getPrintSize();
	if(false == checkPrintSizeName()) return false;
	AcAxDocLock lock;
	AcDbObjectId layoutId = getActiveLayout();
	setPrintConfig();
	setPlotRange();
	setPlotConfig();
	if(false == createPrintWindow()) return false;
	createPlotInfo(layoutId);
	beginDocument();
	beginEngine();
	destoryWindow();
	return true;
}

bool XMJGPrintPicture::getPrintPath(CString &printPath)
{
	CWinApp *pApp = acedGetAcadWinApp();
	HRESULT hRes; LPDISPATCH pDisp = NULL;
	if(!pApp) return false;

	pDisp = pApp->GetIDispatch(TRUE);
	if(!pDisp) return false;

	CComPtr<IAcadApplication>  pComApp;
	hRes = pDisp->QueryInterface(IID_IAcadApplication, (void**)&pComApp);
	if(FAILED(hRes)) return false;

	CComPtr<IAcadPreferences>  pComPref;
	hRes = pComApp->get_Preferences(&pComPref);
	if(FAILED(hRes)) return false;

	CComPtr<IAcadPreferencesFiles>  pComOutputPref;
	hRes = pComPref->get_Files(&pComOutputPref);
	if(FAILED(hRes)) return false;

	BSTR bstDefaultFilePath;
	hRes = ((IAcadPreferencesFiles*)(pComOutputPref.p))->get_PrinterConfigPath(&bstDefaultFilePath);
	if(FAILED(hRes)) return false;
	printPath = CString(bstDefaultFilePath);
	return true;
}

bool XMJGPrintPicture::copyPrintConfigure(const CString &configPath, const CString &configName)
{
	TCHAR soure_path[255] = {0};
	if(RTNORM != acedFindFile(configName, soure_path))	return false;

	CPath path;
	path.Combine(configPath, configName);
	if(!IsPathFileExists(path.m_strPath))
		CopyFile(soure_path, path.m_strPath, false);
	return true;
}

void XMJGPrintPicture::setBackgroundPrint()
{
	struct resbuf rb;
	rb.restype = 5003;
	rb.resval.rint = 0;
	ads_setvar(_T("BACKGROUNDPLOT"), &rb);
}

bool XMJGPrintPicture::setPrintSizeName()
{
	int type = 1;
	if(m_SizeName.CompareNoCase(_T("XMJGA4一半")) == 0 ||
		m_SizeName.CompareNoCase(_T("XMJGA4")) == 0 ||
		m_SizeName.CompareNoCase(_T("XMJGA3")) == 0) return true;
	if(RTCAN == ads_getint(_T("\n 请输入打印的类型<1>A4一半、<2>A4、<3>A3： "), &type)) return false;
	bool flag = true;
	switch(type)
	{
		case 1: m_SizeName.Format(_T("%s"), _T("XMJGA4一半")); break;
		case 2: m_SizeName.Format(_T("%s"), _T("XMJGA4")); break;
		case 3: m_SizeName.Format(_T("%s"), _T("XMJGA3")); break;
		default: flag = false; break;
	}
	return flag;
}

void XMJGPrintPicture::getPrintSize()
{
	if(m_SizeName.CompareNoCase(_T("XMJGA4一半")) == 0)
	{
		m_Width = (int)(2100);
		m_Height = (int)(1480);
	}
	else if(m_SizeName.CompareNoCase(_T("XMJGA4")) == 0)
	{
		m_Width = (int)(2100);
		m_Height = (int)(2970);
	}
	else if(m_SizeName.CompareNoCase(_T("XMJGA3")) == 0)
	{
		m_Width = (int)(4200);
		m_Height = (int)(2970);
	}
}

bool XMJGPrintPicture::checkPrintSizeName()
{
	m_pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
	m_pPlotSettings = new AcDbPlotSettings(true);

	getActiveLayout();
	m_pPSV->setPlotCfgName(m_pPlotSettings, m_PrintDevice);
	m_pPSV->refreshLists(m_pPlotSettings);
	AcArray<const TCHAR*> MediaList;
	m_pPSV->canonicalMediaNameList(m_pPlotSettings, MediaList);
	for(int i = 0; i < MediaList.length(); i++)
	{
		const TCHAR *pLocaleName;
		m_pPSV->getLocaleMediaName(m_pPlotSettings, MediaList.at(i), pLocaleName);
		if(0 == _tcsicmp(pLocaleName, m_SizeName)) return true;
	}
	CString str;
	str.Format(_T("未找到名为\"%s\"的打印尺寸，请在打印设置中设置"), m_PrintDevice);
	acutPrintf(_T("%s"), str);
	if(m_pPlotSettings == NULL) return false;
	delete m_pPlotSettings;
	m_pPlotSettings = NULL;
	return false;
}

AcDbObjectId XMJGPrintPicture::getActiveLayout()
{
	Acad::ErrorStatus es;
	AcAxDocLock lock;
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager();
	AcDbObjectId btrId = pLayoutManager->getActiveLayoutBTRId();
	AcDbBlockTableRecord *pBTR;
	es = acdbOpenObject(pBTR, btrId, AcDb::kForRead);
	AcDbObjectId layoutId = pBTR->getLayoutId();
	AcDbLayout *pLayout = NULL;
	es = acdbOpenObject(pLayout, layoutId, AcDb::kForRead);
	m_pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	pBTR->close();
	return layoutId;
}

void XMJGPrintPicture::setPrintConfig()
{
	m_pPlotSettings->setPlotHidden(false);
	m_pPSV->setPlotRotation(m_pPlotSettings, AcDbPlotSettings::k0degrees);
	m_pPSV->setPlotCfgName(m_pPlotSettings, m_PrintDevice);
	m_pPSV->refreshLists(m_pPlotSettings);
	m_pPSV->setUseStandardScale(m_pPlotSettings, Adesk::kTrue);
	m_pPSV->setStdScaleType(m_pPlotSettings, AcDbPlotSettings::kScaleToFit);
}

void XMJGPrintPicture::setPlotRange()
{
	ads_point pt_min, pt_max;
	GetEntsExtent(m_ObjectArray, pt_min, pt_max, 2);
	ZoomWindow(pt_min, pt_max);
	m_pPSV->setPlotWindowArea(m_pPlotSettings, pt_min[X], pt_min[Y], pt_max[X], pt_max[Y]);
}

void XMJGPrintPicture::setPlotConfig()
{
	m_pPSV->setPlotType(m_pPlotSettings, AcDbPlotSettings::kWindow);
	m_pPSV->setPlotCentered(m_pPlotSettings, true);
	const TCHAR* p = (LPCTSTR)m_SizeName;
	m_pPSV->setCanonicalMediaName(m_pPlotSettings, p);
	double numerator, denominator;
	m_pPlotSettings->getCustomPrintScale(numerator, denominator);
	double m_Scale = numerator / denominator;
	m_pPSV->setPlotCentered(m_pPlotSettings, true);
}

bool XMJGPrintPicture::createPrintWindow()
{
	if(Acad::eOk != AcPlPlotFactory::createPublishEngine(m_pEngine))
	{
		AfxMessageBox(_T("正在进行其他打印任务，请完成后重试！"));
		return false;
	}
	createPrintMsgGUI();
	m_pEngine->beginPlot(m_pPlotProgDlg);
	return true;
}

void XMJGPrintPicture::createPrintMsgGUI()
{
	m_pPlotProgDlg = acplCreatePlotProgressDialog(acedGetAcadFrame()->m_hWnd, false, 1);
	m_pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kDialogTitle, _T("打印"));
	m_pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelJobBtnMsg, _T("取消任务"));
	m_pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kCancelSheetBtnMsg, _T("取消图纸"));
	m_pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetSetProgressCaption, _T("任务进度"));
	m_pPlotProgDlg->setPlotMsgString(AcPlPlotProgressDialog::kSheetProgressCaption, _T("图纸进度"));
	m_pPlotProgDlg->setPlotProgressRange(0, 100);
	m_pPlotProgDlg->onBeginPlot();
	m_pPlotProgDlg->setIsVisible(true);
}

void XMJGPrintPicture::createPlotInfo(AcDbObjectId &layoutId)
{
	m_pPlotInfo = new AcPlPlotInfo;
	m_pPlotInfo->setLayout(layoutId);
	m_pPlotInfo->setOverrideSettings(m_pPlotSettings);
	AcPlPlotInfoValidator validator;
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
	validator.validate(*m_pPlotInfo);
}

void XMJGPrintPicture::beginDocument()
{
	const TCHAR *szDocName = acDocManager->curDocument()->fileName();
	TCHAR savePath[255] = {0};
	_stprintf(savePath, _T("%s"), m_PicturePath->getConfigPath());
	//MessageBox(GetForegroundWindow(), savePath, _T("文件路径98898"), MB_OK);
	m_pEngine->beginDocument(*m_pPlotInfo, szDocName, NULL, 1, true, savePath);
	m_pPlotProgDlg->onBeginSheet();
	m_pPlotProgDlg->setSheetProgressRange(0, 100);
	m_pPlotProgDlg->setSheetProgressPos(0);
}

void XMJGPrintPicture::beginEngine()
{
	AcPlPlotPageInfo pageInfo;
	int ret = m_pEngine->beginPage(pageInfo, *m_pPlotInfo, true);
	ret = m_pEngine->beginGenerateGraphics();
	ret = m_pEngine->endGenerateGraphics();
	ret = m_pEngine->endPage();
	m_pPlotProgDlg->setSheetProgressPos(100);
	ret = m_pPlotProgDlg->onEndSheet();
	m_pPlotProgDlg->setPlotProgressPos(100);
	ret = m_pEngine->endDocument();
	ret = m_pEngine->endPlot();
}

void XMJGPrintPicture::destoryWindow()
{
	m_pEngine->destroy();
	m_pEngine = NULL;
	m_pPlotProgDlg->destroy();
	if(m_pPlotSettings)
	{
		delete m_pPlotSettings;
		m_pPlotSettings = NULL;
	}
}

string XMJGPrintPicture::getFilePath(const string &pathname)
{
	int n = (int)pathname.find_last_of('/');
	int m = (int)pathname.find_last_of('\\');
	int c = m > n ? m + 1 : n + 1;
	std::string str = pathname.substr(0, c);
	return str;
}

string XMJGPrintPicture::getFileName(const string &pathname)
{
	int n = (int)pathname.find_last_of('/');
	int m = (int)pathname.find_last_of('\\');
	int c = m > n ? m + 1 : n + 1;
	std::string str = pathname.substr(c, pathname.size() - c);
	return str;
}

string XMJGPrintPicture::getFileTitle(const string &pathname)
{
	std::string filename = getFileName(pathname);
	int n = (int)filename.find_last_of('.');
	std::string str = filename.substr(0, n);
	return str;
}

string XMJGPrintPicture::getFileExt(const string &pathname)
{
	std::string filename = getFileName(pathname);
	int n = (int)filename.find_last_of('.') + 1;
	std::string str;
	if(n)
		str = filename.substr(n, filename.size() - n);
	else
		str = "";
	return str;
}

string XMJGPrintPicture::getFileExt(const char *pathname)
{
	bool flag = false; char *ext = new char[4];
	char *ext1 = ext;
	while(*pathname)
	{
		char temp = *pathname++;
		if(temp != '.' && flag == false)
			continue;
		flag = true;
		if(temp == '.') continue;
		*ext1 = temp; ext1++;
	}
	return ext;
}

int XMJGPrintPicture::rescale_image_jpeg()
{
	GDALAllRegister();
	GDALDriverManager *driver_manager = GetGDALDriverManager();
	string savePath = m_PicturePath->getSavePath();
	string in_ext = getFileExt(savePath);
	GDALDriver *out_driver;
	if(in_ext.find("jpg") == 0)
		out_driver = driver_manager->GetDriverByName("JPEG");
	else if(in_ext.find("bmp") == 0)
		out_driver = driver_manager->GetDriverByName("BMP");

	///////////获取MEM Driver////////////////////////////////
	GDALDriver *mem_driver = NULL;
	mem_driver = driver_manager->GetDriverByName("MEM");
	if(NULL == mem_driver || NULL == out_driver)
		return false;

	///////////////////////////打开输入影像////////////////////////////////////////
	string in_path = m_PicturePath->getPicturePath();
	GDALDataset *in_dataset = (GDALDataset *)GDALOpen(in_path.c_str(), GA_ReadOnly);
	if(!in_dataset)	return 1;

	int channel_count = in_dataset->GetRasterCount();
	int x_size = in_dataset->GetRasterXSize();
	int y_size = in_dataset->GetRasterYSize();

	getPrintSize();
	m_BGColor.setColorBlue(255);
	m_BGColor.setColorGreen(255);
	m_BGColor.setColorRed(255);

	double scale_x = (double)x_size / m_Width;
	double scale_y = (double)y_size / m_Height;

	int read_x = (int)(scale_x);
	int read_y = (int)(scale_y);

	unsigned char *in_data_space = new unsigned char[x_size * read_y * 3];
	unsigned char *out_data_space = new unsigned char[m_Width * m_Height * 3];

	double x_counter = 0;
	double y_counter = 0;

	int out_counter = 0;

	unsigned char *data_start_h;
	unsigned char *data_start_w;

	int band_map[3];
	band_map[0] = 1;
	band_map[1] = 2;
	band_map[2] = 3;

	GDALDataset *mem_dataset = mem_driver->Create("", m_Width, m_Height, 3, GDT_Byte, NULL);

	for(int i = 0; i < m_Height; i++)
	{
		x_counter = 0;
		CPLErr read_error = in_dataset->RasterIO(GF_Read, 0, (int)(y_counter), x_size, read_y, in_data_space, x_size, read_y, GDT_Byte, 3, band_map, 3, x_size * 3, 1);
		data_start_h = in_data_space;
		data_start_w = data_start_h;
		for(int s = 0; s < m_Width; s++)
		{
			out_data_space[out_counter] = data_start_w[0];
			out_data_space[out_counter + 1] = data_start_w[1];
			out_data_space[out_counter + 2] = data_start_w[2];

			for(int t = 0; t < read_y; t++)
			{
				for(int u = 0; u < read_x; u++)
				{
					if(m_BGColor.getColorRed() != data_start_w[u * 3] ||
						m_BGColor.getColorGreen() != data_start_w[u * 3 + 1] ||
						m_BGColor.getColorBlue() != data_start_w[u * 3 + 2])
					{
						out_data_space[out_counter] = data_start_w[u * 3];
						out_data_space[out_counter + 1] = data_start_w[u * 3 + 1];
						out_data_space[out_counter + 2] = data_start_w[u * 3 + 2];
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

	CPLErr write_error = mem_dataset->RasterIO(GF_Write, 0, 0, m_Width, m_Height, out_data_space, m_Width, m_Height, GDT_Byte, 3, band_map, 3, m_Width * 3, 1);
	string out_path = m_PicturePath->getSavePath();
	const char *ErrorMsg = CPLGetLastErrorMsg();
	CPLErr type = CPLGetLastErrorType();
	int ErrorNo = CPLGetLastErrorNo();
	out_driver->CreateCopy(out_path.c_str(), mem_dataset, true, 0, 0, 0);
	delete driver_manager;
	delete mem_driver; delete in_dataset; delete out_driver; delete out_data_space;
	delete out_data_space; delete data_start_h; delete data_start_w; delete ErrorMsg;
	return 1;
}

bool XMJGPrintPicture::DeletePicture()
{
	return DeleteFile(m_PicturePath->getConfigPath()) == TRUE;
}

XMJGPictureInfo::XMJGPictureInfo()
{
	read1(m_Scale);
	m_Scale /= 1000;
}

double XMJGPictureInfo::getScale() const
{
	return m_Scale;
}

bool XMJGPictureInfo::initPictureInfo(const CString &str)
{
	CString line; line.Append(str); line.Trim();
	int pos = line.Find(_T(","));
	if(pos == -1) return false;
	setPictureOrder(_tstoi(line.Left(pos)));
	line = line.Right(line.GetLength() - pos - 1);
	if((pos = line.Find(_T(","))) == -1) return false;
	setPictureName(line.Left(pos));
	line = line.Right(line.GetLength() - pos - 1);
	if((pos = line.Find(_T(","))) == -1) return false;
	setPictureDescribe(line.Left(pos));
	line = line.Right(line.GetLength() - pos - 1);
	setPictureFormat(_tstoi(line));
	return true;
}

void XMJGPictureInfo::setPictureOrder(int pictureOrder)
{
	m_PictureOrder = pictureOrder;
}

int XMJGPictureInfo::getPictureOrder() const
{
	return m_PictureOrder;
}

void XMJGPictureInfo::setPictureFormat(int pictureFormat)
{
	m_PictureFormat = pictureFormat;
}

void XMJGPictureInfo::getPictureFormat(double &width, double &height) const
{
	switch(m_PictureFormat)
	{
		case 1:	width = 14.5; height = 10.88; break;
		case 2: width = 16.9; height = 22.22; break;
		default: width = 10.88; height = 14.5; break;
	}
}

void XMJGPictureInfo::setPictureName(const CString &pictureName)
{
	m_PictureName.Append(pictureName);
}

string XMJGPictureInfo::getPictureName() const
{
	char pic[255] = {0};
	TcharToChar(m_PictureName, pic);
	string ret(pic); return ret;
}

void XMJGPictureInfo::setPictureDescribe(const CString &pictureDescribe)
{
	m_PictureDescribe.Append(pictureDescribe);
}

string XMJGPictureInfo::getPictureDescribe() const
{
	char pictureDescribe[255] = {0};
	TcharToChar(m_PictureDescribe, pictureDescribe);
	string ret(pictureDescribe); return ret;
}

XMJGTool::XMJGTool()
{
	m_hh = 500;
	read1(m_hh);
	m_hh /= 1000;
}

XMJGTool::~XMJGTool()
{
}

void XMJGTool::DrawPhotoPositon()
{
	CXRecordOperate xrecord; int order = 0;
	if(false == xrecord.ReadIntegerRecord(_T("XMJG"), _T("ZPXH"), order)) order = 0;
	ads_point spt; double angle = 0.0;
	IDataBaseOper oper; int color; CString layer, font; float size = 0.0;
	oper.readCommonTable(_T("照片拍摄大小"), size);
	oper.readCommonTable(_T("照片拍摄字体"), font);
	oper.readCommonTable(_T("照片拍摄颜色"), color);
	oper.readCommonTable(_T("照片拍摄图层"), layer); int type = 1;
	if(RTCAN == ads_getint(_T("\n 请选择[1]自动编号、[2]手动编号："), &type)) return;
	while(true)
	{
		if(type == 1) order++;
		else
			if(RTCAN == ads_getint(_T("\n 输入编号："), &order)) break;
		if(!drawPhoto(spt, angle, color, layer)) break;
		if(!setPhotoInfo(spt, angle, color, size, layer, font, order)) break;
	}
	xrecord.AddIntegerRecord(_T("XMJG"), _T("ZPXH"), ++order);
}

void XMJGTool::ExportDocument(const XMJGPath *path, XMJGDocType type)
{
	MPicInfo infos;
	readPictureConfig(path->getConfigPath(), infos);
	SmartTable::Word word;
	char *templatePath = new char[255];
	if(!getTemplateByType(type, templatePath)) return;
	PicInfo info;
	word.setEncoding(SmartTable::Encoding_GBK);
	int order = -1, prop;
	word.loadWord(templatePath);
	double width = 0.0, height = 0.0;
	char picturePath[255] = {0};
	sprintf(picturePath, "%s\\", path->getPicturePath());
	word.setCurrentInsert("#mark#", 0, true);
	MPicInfoIter it = infos.begin();
	prop = word.setPropertyTable(0);
	for(; it != infos.end(); ++it)
	{
		order = it->first; info = it->second;
		char pictureName[255] = {0};
		sprintf(pictureName, "%s%s", picturePath, info.getPictureName());
		info.getPictureFormat(width, height);
		bool isok = word.appendImage(pictureName, width, height, SmartTable::Alignment_MC);
		word.appendParagraph(info.getPictureDescribe().c_str(), 0);
	}
	word.removeTable(0); word.save(path->getSavePath().c_str());
	delete templatePath;
}

void XMJGTool::getFolderDayFile(const CString &pathStr, CStringArray &arrStrFile)
{
	CString myDataPath, fdPath, strTmp;
	myDataPath = pathStr + "\\*.*";
	CFileFind find;
	BOOL bf = find.FindFile(myDataPath);
	while(bf)
	{
		bf = find.FindNextFile();
		if(find.IsDots()) continue;
		fdPath = find.GetFilePath();
		if(find.IsDirectory()) continue;
		//getFolderDayFile(fdPath, arrStrFile);     这一句是遍历当前文件夹中jpg
		strTmp = fdPath.Right(4).MakeLower();
		if(strTmp.CompareNoCase(_T(".jpg")) == 0)
			arrStrFile.Add(fdPath);
	}
	find.Close();
}

bool XMJGTool::setPhotoInfo(ads_point pt, double angle, int color, float size, const CString &layer, const CString &font, int order)
{
	TCHAR szText[2000] = {0};
	_stprintf(szText, _T("照片%d"), order);
	if(angle > 0 && angle < PI)
		pt[Y] -= (size * m_hh);
	else pt[Y] += (size * m_hh);
	AcDbObjectId id = DrawText(pt, szText, _T("MC"), _T(""), m_hh * size);
	SetTextStyle(id, font, 1.0);
	setcolor(aname(id), color);
	setlayer(aname(id), layer);
	return true;
}

bool XMJGTool::drawPhoto(ads_point spt, double &angle, int color, const CString &layer)
{
	if(RTNORM != ads_getpoint(NULL, _T("\n 请输入拍摄的位置："), spt)) return false;
	ads_point ept;
	if(RTNORM != ads_getpoint(NULL, _T("\n 请点击拍摄的方向："), ept)) return false;
	angle = ads_angle(spt, ept);
	createLayer(layer);
	AcDbLeader *leader = new AcDbLeader;
	AcGePoint3d pt1(ept[0], ept[1], 0);
	AcGePoint3d pt2(spt[0], spt[1], 0);
	leader->appendVertex(pt1);
	leader->appendVertex(pt2);
	leader->setDimasz(4.0);
	leader->setColorIndex(color);
	leader->setLayer(layer);
	AcDbObjectId lid;
	AppendEntity(leader, lid);
	leader->close();
	return true;
}

bool XMJGTool::getTemplateByType(XMJGDocType type, char *templatePath)
{
	TCHAR templateName[255] = {0}; TCHAR path[255] = {0};
	switch(type)
	{
		case PedestrianBridge:
			_stprintf(templateName, _T("%s"), _T("建筑.docx"));
			break;
		case RoadAndPipeLine:
			_stprintf(templateName, _T("%s"), _T("道路附有管线.docx"));
			break;
		case RoadNoPipeLine:
			_stprintf(templateName, _T("%s"), _T("道路不带管线.docx"));
			break;
		case BuildProject:
			_stprintf(templateName, _T("%s"), _T("市政天桥项目.docx"));
			break;
		default:
			break;
	}
	if(RTNORM != ads_findfile(templateName, path))
	{
		TCHAR info[255] = {0};
		_stprintf(info, _T("\n 无法知道模板文件%s"), templateName);
		acutPrintf(info);
		return false;
	}
	TcharToChar(path, templatePath);
	return true;
}

void XMJGTool::readPictureConfig(const CString &path, MPicInfo &infos)
{
	CStdioFile file;	CString line;
	bool flag = (TRUE == file.Open(path, CFile::modeRead));
	if(flag == FALSE) return;
	file.ReadString(line);
	while(file.ReadString(line))
	{
		PicInfo info;
		if(false == info.initPictureInfo(line)) continue;
		infos[info.getPictureOrder()] = info;
	}
}

std::string UTF8ToGBK(const std::string &strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1]; memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL); //strUTF8 = szGBK; 
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

XMJGPlan::XMJGPlan()
{
	CString path = GetCurrentDwgName();
	CString name = GetCurrentDwgFullPath();
	int pos = name.ReverseFind(_T('\\'));
	pos = pos == -1 ? name.ReverseFind(_T('/')) : pos;
	m_savedir = name.Left(pos);
	read1(m_Scale);	m_Scale /= 1000; IDataBaseOper oper;
	oper.readCommonTable(_T("建筑竣工分层平面图颜色"), m_fcpm._color);
	oper.readCommonTable(_T("建筑竣工分层平面图图层"), m_fcpm._layer);
}

XMJGPlan::~XMJGPlan()
{

}

bool XMJGPlan::CreatePlan()
{
	if(m_layerinfo.size() == 0) return false;
	bool bCreate = true;
	TCHAR szValue[100] = {0};
	ReadXdata(aname(m_ObjectID), _T("建筑物名称"), 0, szValue);
	m_ldname.Format(_T("%s"), szValue);
	m_dwg_guid.Format(_T("%s\\%s分层平面图.dwg"), m_savedir, szValue);
	_stprintf(m_dwgPathName, _T("%s"), GetCurrentDwgFullPath());
	AddXdata(aname(m_ObjectID), _T("分层平面图名"), 0, m_dwg_guid);
	DrawAdumbration();
	SavePlanDwg();
	if(true == SaveCurDwg())
		SendCommand();
	return true;
}

void XMJGPlan::setLayerInfo(const VStr &layerinfo)
{
	for(int idx = 0; idx < (int)layerinfo.size(); ++idx)
		m_layerinfo.push_back(layerinfo[idx]);
}

bool XMJGPlan::CreatePlan(const CString &path)
{
	if(m_layerinfo.size() == 0) return false;
	bool bCreate = true;
	TCHAR szValue[100] = {0};
	ReadXdata(aname(m_ObjectID), _T("建筑物名称"), 0, szValue);
	m_ldname.Format(_T("%s"), szValue);
	m_dwg_guid.Format(_T("%s\\%s分层平面图.dwg"), path, szValue);
	_stprintf(m_dwgPathName, _T("%s"), GetCurrentDwgFullPath());
	AddXdata(aname(m_ObjectID), _T("分层平面图名"), 0, m_dwg_guid);
	DrawAdumbration();
	SavePlanDwg();
	if(true == SaveCurDwg())
		SendCommand();
	return true;
}

bool XMJGPlan::CreatePlanEx(const CString &path, const CString &name)
{
	bool bCreate = true; m_ldname.Format(_T("%s"), name);
	m_dwg_guid.Format(_T("%s\\%s分层平面图.dwg"), path, m_ldname);
	_stprintf(m_dwgPathName, _T("%s"), GetCurrentDwgFullPath());
	DrawAdumbrationEx();
	SavePlanDwg();
	if(true == SaveCurDwg())
		SendCommand();
	return true;
}

bool XMJGPlan::setObjectID(const AcDbObjectId &id)
{
	m_ObjectID = id; TCHAR info[256] = {0};
	ReadXdata(aname(m_ObjectID), _T("楼栋号"), 0, info);
	m_ldname.Format(_T("%s"), info);
	entitysInfo::GetEntHandle(m_ObjectID, m_szHandle);
	return true;
}

void XMJGPlan::CopyPlan()
{
	int count = GetPlanCount();
	AcGePoint2d pos;
	for(int idx = 0; idx < count; ++idx)
	{
		AcDbObjectId id, textid;
		id = CopyEntity(m_ObjectID);
		setlayer(aname(id), m_fcpm._layer);
		setcolor(aname(id), m_fcpm._color);
		CalculatePlanPos(idx, pos, id);
		MoveEntity(aname(id), pos.x, 0);
		AddPlanInfo(id, textid, idx);
		DelPlanInfo(id);
		m_Ids.append(id);
		m_Ids.append(textid);
	}
}

void XMJGPlan::DrawAdumbration()
{
	AcGePoint2d pos;
	AcGePoint2dArray nodes;
	int count = m_layerinfo.size();
	GetEntExtent(m_ObjectID, nodes, 10.0);
	for(int idx = 0; idx < count; ++idx)
	{
		AcDbObjectId id, id1, textid;
		id = DrawPolyline2D(nodes, 0, true, _T(""));
		AddXdata(aname(id), _T("楼栋号"), 0, m_ldname);
		AddXdata(aname(id), _T("WJJX"), 0, _T("WJJX"));
		setlayer(aname(id), m_fcpm._layer);
		setcolor(aname(id), m_fcpm._color);
		CalculatePlanPos(idx, pos, id);
		MoveEntity(aname(id), pos.x, 0);
		AddPlanInfo(id, textid, idx);
		m_Ids.append(id);
		m_Ids.append(textid);
	}
}

bool XMJGPlan::DrawAdumbrationEx()
{
	int count = (int)m_layerinfo.size();
	int type = 0;
	if(RTNORM != ads_getint(_T("\n请选择分层的大小:[1]10X10、[2]20X20、[3]30X30、[4]40X40：<1>"), &type)) return false;
	AcGePoint2dArray nodes; AcGePoint2d lb(0, 0), lt(0, type * 10.0), rt(type * 10.0, type * 10.0), rb(type * 10.0, 0);
	nodes.append(lb); nodes.append(lt); nodes.append(rt); nodes.append(rb); AcGePoint2d pos;
	for(int idx = 0; idx < count; ++idx)
	{
		AcDbObjectId id, id1, textid;
		id = DrawPolyline2D(nodes, 0, true, _T(""));
		AddXdata(aname(id), _T("WJJX"), 0, _T("WJJX"));
		setlayer(aname(id), m_fcpm._layer);
		setcolor(aname(id), m_fcpm._color);
		CalculatePlanPos(idx, pos, id);
		MoveEntity(aname(id), pos.x, 0);
		AddPlanInfo(id, textid, idx);
		m_Ids.append(id);
		m_Ids.append(textid);
	}
	return true;
}

void XMJGPlan::AddPlanInfo(const AcDbObjectId &id, AcDbObjectId &textid, int idx)
{
	ads_point pMin, pMax; AcGePoint3d mid;
	GetEntExtent(aname(id), pMin, pMax);
	mid = GetMiddlePoint(apoint(pMin), apoint(pMax));
	mid.y -= (pMax[Y] - pMin[Y]) / 2 + 15;
	TCHAR szdata[255] = {0};
	AddXdata(aname(id), _T("楼栋号"), 0, m_ldname);
	CString layername(m_layerinfo[idx]); int pos = layername.Find(_T("_"));
	AddXdata(aname(id), _T("高度示意图"), 0, layername.Mid(0, pos));
	layername = layername.Mid(pos + 1);	CString info;
	int slayer, elayer;
	getLayerNum(layername, slayer, elayer);
	if(fabs(_tstof(layername) - slayer - 0.5) < EPS)
		info.Format(_T("%d夹层平面图"), slayer);
	else if(slayer != 0 && elayer != 0 && slayer == elayer)
		info.Format(_T("第%d层平面图"), slayer);
	else if(layername.Find(_T(" ---> ")) != -1)
	{
		info.Format(_T("%s层平面图"), layername);
		info.Replace(_T(" ---> "), _T("到"));
	}
	else
		info.Format(_T("%s平面图"), layername);
	AddXdata(aname(id), _T("楼层号"), 0, layername);
	textid = DrawText(mid, info, _T("MC"), _T(""), m_Scale * 5);
	CString handle = GetHandleByObject(textid, false);
	AddXdata(aname(id), _T("平面名称"), 0, handle);
	setlayer(aname(textid), _T("FCPMMC"));
	handle.Format(_T("%d"), idx);
	AddXdata(aname(id), _T("分层序号"), 0, handle);
}

void XMJGPlan::DelPlanInfo(const AcDbObjectId &id)
{
	RemoveXdata(aname(id), _T("相同楼层编号"));
	RemoveXdata(aname(id), _T("分层平面图名"));
}

bool XMJGPlan::SaveCurDwg()
{
	int es = acDocManager->mdiActiveDocument()->database()->saveAs(m_dwgPathName);
	if(es == Acad::eOk)
		return true;
	return false;
}

void XMJGPlan::SendCommand()
{
	SendCommand(_T("OpenDwgCmd "));
	SendCommand(m_dwg_guid);
	SendCommand(_T(" "));
}

bool XMJGPlan::SavePlanDwg()
{
	CSaveBlocks block;
	block.setPath(m_dwg_guid);
	block.saveBlocks2(m_Ids, true);

	// 	ads_name ssName;
	// 	int commandRes = -1;
	// 	TCHAR szPath[MAX_PATH] = {0};
	// 	ads_point pt;
	// 	pt[X] = pt[Y] = pt[Z] = 0.0;
	// 	IdArrToSS(m_Ids, ssName);
	// 	if(ads_findfile(m_dwgPathName, szPath) == RTNORM)
	// 		commandRes = ads_command(RTSTR, _T("wblock"), RTSTR, m_dwg_guid, RTSTR, _T("Y"), RTSTR, _T(""), RTPOINT, pt, RTPICKS, ssName, RTSTR, _T(""), RTNONE);
	// 	else
	// 		commandRes = ads_command(RTSTR, _T("wblock"), RTSTR, m_dwg_guid, RTSTR, _T(""), RTPOINT, pt, RTPICKS, ssName, RTSTR, _T(""), RTNONE);
	// 	ads_ssfree(ssName);
	return true;
}

int XMJGPlan::GetPlanCount()
{
	int len = GetXDataCount(aname(m_ObjectID), _T("相同楼层编号"));
	return len;
}

void XMJGPlan::SendCommand(const TCHAR *szCommand)
{
	COPYDATASTRUCT msg;
	msg.dwData = (DWORD)1;
	msg.cbData = (DWORD)(_tcslen(szCommand) + 1)*sizeof(TCHAR);
	msg.lpData = (TCHAR*)szCommand;

	acedGetAcadFrame()->SendMessage(WM_COPYDATA, (WPARAM)adsw_acadMainWnd(), (LPARAM)&msg);
	acedGetAcadDockCmdLine()->SetFocus();
}

void XMJGPlan::CalculatePlanPos(int index, AcGePoint2d &pos, const AcDbObjectId &id)
{
	AcGePoint3d mid; ads_point pMin, pMax;
	GetEntExtent(aname(id), pMin, pMax);
	mid = GetMiddlePoint(apoint(pMin), apoint(pMax));
	double width = pMax[X] - pMin[X];
	double xoff = index * (width + 50.0);
	pos.x = xoff; pos.y = mid.y;
}

void XMJGPlan::ExportPlanToPicture()
{

}

#pragma region 工程测量输出DWG

XMJGExport::XMJGExport()
	: m_isfilterobject(false)
{
	m_compass = 0.0;
	m_DefaultScale = 0;
	m_replaceflag = false;
	m_Block = new SBreakBlock;
	m_Objects.removeAll();
	m_callback = 0;
	IDataBaseOper oper;
	oper.readCommonTable(_T("建筑竣工变更注记大小"), m_bgbj._size);
	oper.readCommonTable(_T("建筑竣工变更注记图层"), m_bgbj._layer);
	oper.readCommonTable(_T("建筑竣工变更注记颜色"), m_bgbj._color);
}

XMJGExport::~XMJGExport()
{
	if(m_Block != NULL)
		delete m_Block;
	m_Block = NULL;
}

bool XMJGExport::setExportDir(TCHAR *dir /* = NULL */)
{
	bool flag = true;
	if(dir == NULL)
		flag = GetDirectory(_T("请选择保存的路径"), dir);
	if(false == flag) return false;
	m_ExportDir.Format(_T("%s"), dir);
	return true;
}

void XMJGExport::setObjects(const AcDbObjectIdArray &objs)
{
	m_Objects.removeAll();
	m_Objects.setLogicalLength(0);
	m_Objects.setPhysicalLength(0);
	m_Objects.append(objs);
}

void XMJGExport::setPictureFrame(const TCHAR *pictureFrame, double angle)
{
	m_PictureFrame.Format(_T("%s"), pictureFrame);
	m_compass = angle;
}

void XMJGExport::setScaleEntity(const AcDbObjectId &sent)
{
	m_sEnt = sent;
}

void XMJGExport::setSavePath(const TCHAR *savePath)
{
	m_SaveDwgPath.Format(_T("%s"), savePath);
}

void XMJGExport::isDrawRectangle(bool isdraw)
{
	m_isDraw = isdraw;
}

void XMJGExport::setSavePath(const CString &savePath)
{
	m_SaveDwgPath.Format(_T("%s"), savePath);
}

void XMJGExport::setOffsetDistance(double offsetDist)
{
	m_OffsetDist = offsetDist;
}

bool XMJGExport::setExportRange()
{
	SNodesOpe nodesO; ads_name ents; ads_point pMin, pMax;
	if(m_Objects.length() == 0)
	{
		if(RTNORM != ads_getpoint(NULL, _T("\n <请框选要输出的实体范围> 请按下第一个点："), pMin)) return false;
		if(RTNORM != ads_getcorner(pMin, _T("\n <请框选要输出的实体范围> 请按下第二个点："), pMax)) return false;
	}
	else
	{
		GetEntsExtent(m_Objects, pMin, pMax);
	}
	ZoomWindow(pMin, pMax);
	nodesO.modifyLB_RTPoint(pMin, pMax);
	m_LeftButton = apoint(pMin);
	m_RightTop = apoint(pMax);
	nodesO.LB_RT_Nodes(m_LeftButton, m_RightTop, m_Rectangular);
	AcDbObjectId tkid = DrawPolyline2D(m_Rectangular, 1.0, true, _T("2000000"));
	TrimPoly(aname(tkid), true, false, true);	EntDel(aname(tkid));
	ads_command(RTSTR, _T("PURGE"), RTSTR, _T("B"), RTSTR, _T("*"), RTSTR, _T("N"), RTNONE);
	if(!ssFromNodes(m_Rectangular, ents, _T("CP"))) return false;
	m_Objects.setLogicalLength(0); SSToIdArr(ents, m_Objects);
	GetEntsExtent(m_Objects, pMin, pMax);
	nodesO.modifyLB_RTPoint(pMin, pMax);
	m_LeftButton.x = pMin[X]; m_LeftButton.y = pMin[Y];
	m_RightTop.x = pMax[X]; m_RightTop.y = pMax[Y];
	m_ObjMiddle = GetMiddlePoint(apoint(m_LeftButton), apoint(m_RightTop));
	return true;
}

void XMJGExport::addCompass()
{
	AcGePoint3d pos;
	if(false == m_Block->findPointByHyperLink(_T("指北针"), pos)) return;
	AcDbObjectId id = DrawInsert(apoint(pos), _T("Compass"), m_scale, m_scale, m_scale, m_compass);
	m_PictureFrameIds.append(id);
}

void XMJGExport::runCallBack()
{
	TCHAR scale[24] = {0};
	_stprintf(scale, _T("%d"), m_DefaultScale);
	m_ReplaceText[_T("#SCALE#")] = scale;
	if(NULL == m_callback) return;
	m_callback(m_ReplaceText, m_Block);
}

bool XMJGExport::setDefaultScale()
{
	m_DefaultScale = (int)(1000.0 / m_scale);
	m_DefaultScale /= 100; m_DefaultScale++;
	m_DefaultScale *= 100; m_scale = 1000.0 / m_DefaultScale;
	return true;
}

void XMJGExport::CalculateScale()
{
	double TKH = m_tkRightTop.y - m_tkLeftButton.y;
	double TKW = m_tkRightTop.x - m_tkLeftButton.x;
	double EntH = m_RightTop.y - m_LeftButton.y;
	double EntW = m_RightTop.x - m_LeftButton.x;
	double tkscale = TKH / EntH;
	double entscale = TKW / EntW;
	m_scale = tkscale < entscale ? tkscale : entscale;
}

bool XMJGExport::InsertPictureFrame()
{
	AcGePoint2d pos(0, 0);
	AcDbObjectId tk = DrawInsert(apoint(pos), m_PictureFrame);
	if(tk == AcDbObjectId::kNull) return false;
	m_Block->SetBlock(tk); m_Block->Explode();
	m_PictureFrameIds.append(m_Block->m_objIdArr);
	AcDbObjectId lbid, rtid;
	m_Block->findPointByHyperLink(_T("左下角点"), m_tkLeftButton, lbid);
	m_Block->findPointByHyperLink(_T("右上角点"), m_tkRightTop, rtid);
	m_projectNameid = m_Block->findObjByHyperLink(_T("项目名称"));
	m_buildCompanyid = m_Block->findObjByHyperLink(_T("建设单位"));
	m_tkMiddle = GetMiddlePoint(m_tkLeftButton, m_tkRightTop);
	EntDel(aname(lbid)); EntDel(aname(rtid));
	addCompass(); return true;
}

void XMJGExport::drawRectangle(AcDbObjectIdArray &idArr)
{
	if(m_isDraw == false) return; ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(id), _T("打印范围"));
	idArr.append(id);
}

bool XMJGExport::combineEntityAndTk()
{
	RETFALSE(setExportRange()) false;
	if(0 == _tcslen(m_PictureFrame)) return false;
	RETFALSE(InsertPictureFrame()) false;
	if(m_isfilterobject == true)
		filterObject();
	CalculateScale();
	RETFALSE(setDefaultScale()) false;
	ScaleEntitys(m_PictureFrameIds, m_tkMiddle, 1.0 / m_scale);
	moveEntitysToTk();
	return true;
}

bool XMJGExport::saveDwgToFile()
{
	CSaveBlocks blocks;
	m_SaveDwgPath.Format(_T("%s"), m_SaveDwgPath);
	blocks.setPath(m_SaveDwgPath);
	ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr;
	SSToIdArr(ssSave, idArr);
	drawRectangle(idArr);
	blocks.saveBlocks2(idArr, false);
	return true;
}

void XMJGExport::moveEntitysToTk()
{
	double dXMove = m_ObjMiddle.x - m_tkMiddle.x;
	double dYMove = m_ObjMiddle.y - m_tkMiddle.y;
	MoveEntitys(m_PictureFrameIds, dXMove, dYMove);
}

bool XMJGExport::saveDwgToPicture(bool flag)
{
	XMJGPrintPicture picture;
	picture.setObjectArray(m_Objects);
	picture.setPrintSizeName();
	picture.setPrintFormat(Raster_Jpg);
	picture.setPrintPath(m_SaveDwgPath);
	picture.BeginPrintPicture();
	return true;
}

bool XMJGExport::setReplaceChangeFlag(bool flag)
{
	m_replaceflag = flag;
	return true;
}

void XMJGExport::setObjectFilterFlag(XMJGFilterObject filter)
{
	m_filterobject = filter;
}

void XMJGExport::ReplaceFieldValue()
{
	MSIter it = m_ReplaceText.begin();
	TCHAR field[255] = {0};
	TCHAR value[255] = {0};
	for(; it != m_ReplaceText.end(); ++it)
	{
		_stprintf(field, _T("%s"), it->first);
		_stprintf(value, _T("%s"), it->second);
		m_Block->replaceText(field, value);
	}
}

void XMJGExport::ReplaceTableHeaderField()
{
	TCHAR blc[10] = {0}; TCHAR jrarea[200] = {0};
	TCHAR jzarea[200] = {0}; TCHAR jzzarea[200] = {0};
	TCHAR jrzarea[200] = {0};
	_stprintf(blc, _T("%d"), m_DefaultScale);
	StatisticArea(jzarea, jrarea, jzzarea, jrzarea);
	m_Block->replaceTextEx(_T("#SCALE#"), blc);
}

void XMJGExport::StatisticArea(TCHAR *jzarea, TCHAR *jrarea, TCHAR *jzzarea, TCHAR *jrzarea)
{
	int len = m_Objects.length(); TCHAR area[200] = {0};
	double m_buildArea = 0, m_jrArea = 0.0; TCHAR floor[200] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		if(true != ReadXdata(aname(id), _T("建筑面积"), DOUBLE_TYPE, area)) continue;
		if(_tcslen(area) == 0) continue; m_buildArea += _tstof(area);
		ReadXdata(aname(id), _T("积容面积"), DOUBLE_TYPE, area);
		m_jrArea += _tstof(area);
	}
	CString build(m_FloorNo);
	int pos = build.FindOneOf(_T(" ---> "));
	if(pos == -1)
	{
		_stprintf(jrzarea, _T("%.3f"), m_jrArea);
		_stprintf(jzzarea, _T("%.3f"), m_buildArea);
	}
	else
	{
		int start = _tstoi(build.Mid(0, pos));
		pos = build.ReverseFind(_T(' '));
		int count = build.GetLength() - pos;
		int end = _tstoi(build.Mid(pos, count));
		_stprintf(jrzarea, _T("%.3f"), m_jrArea * (end - start + 1));
		_stprintf(jzzarea, _T("%.3f"), m_buildArea * (end - start + 1));
	}
	_stprintf(jrarea, _T("%.3f"), m_jrArea);
	_stprintf(jzarea, _T("%.3f"), m_buildArea);
}

bool XMJGExport::replaceChangeNote()
{
	VStr bgzj;
	int len = m_Objects.length();
	TCHAR info[255] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		if(false == ReadXdata(aname(id), _T("变更序号"), 0, info)) continue;
		bgzj.push_back(info);
	}
	AcDbObjectId bgid; AcGePoint3d bgpos;
	m_Block->findPointByHyperLink(_T("变更注记"), bgpos, bgid);
	len = (int)bgzj.size();
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint3d temp(bgpos);
		temp.y += m_bgbj._size * (len - idx - 1) * 2;
		AcDbObjectId tbgid = DrawText(temp, bgzj.at(idx), _T("ML"), _T(""), m_bgbj._size);
		setlayer(aname(tbgid), m_bgbj._layer); setcolor(aname(tbgid), m_bgbj._color);
		m_PictureFrameIds.append(tbgid);
	}
	EntDel(aname(bgid));
	return true;
}

void XMJGExport::replaceFCPMNote()
{
	int len = m_Objects.length();
	TCHAR layer[20] = {0};
	AcDbObjectIdArray ids;
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		GetEntLayer(id, layer);
		if(_tcscmp(layer, _T("变更标注")) == 0)
			ids.append(id);
	}
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		m_Objects.remove(ids.at(idx));
		EntDel(aname(ids.at(idx)));
	}
}

void XMJGExport::filterObject()
{
	TCHAR info[20] = {0};
	for(int idx = 0; idx < m_Objects.length(); ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		if(true == ReadXdata(aname(id), _T("WJJX"), 0, info)) continue;
		m_Objects.remove(id);
		EntDel(aname(id));
	}
	switch(m_filterobject)
	{
		case ChangeAreaFlag:
			replaceChangeNote();
			break;
		case FenCengPingMianFlag:
			replaceFCPMNote();
			break;
		default:
			break;
	}
}

void XMJGExport::calcFontSize()
{
	TCHAR info[255] = {0};
	ReadXdata(aname(m_projectNameid), _T("changdu"), 1, info);
	double len = _tstof(info);
	GetEntText(aname(m_projectNameid), info);
	int fontlen = (int)_tcslen(info);
	if(len < fontlen)
	{
		double height = GetTextHeight(aname(m_projectNameid));
		double size = len / fontlen * height;
		SetTextHeight(aname(m_projectNameid), size);
	}
	ReadXdata(aname(m_buildCompanyid), _T("changdu"), 1, info);
	len = _tstof(info);
	GetEntText(aname(m_buildCompanyid), info);
	fontlen = (int)_tcslen(info);
	if(len < fontlen)
	{
		double height = GetTextHeight(aname(m_buildCompanyid));
		double size = len / fontlen * height;
		SetTextHeight(aname(m_buildCompanyid), size);
	}
}

bool XMJGExport::ExportDwg()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	acDocManager->lockDocument(acDocManager->curDocument());
	bool flag = combineEntityAndTk(); AcGePoint3d pt;
	getEntPosition(aname(m_sEnt), pt);
	IDataBaseOper dbOper; CString biliechi, sfxs;
	dbOper.readCommonTable(_T("分层平面图面积注记缩放"), _T("属性值"), biliechi);
	dbOper.readCommonTable(_T("分层平面图面积注记缩放"), _T("属性值1"), sfxs);
	CStringArray biliechis, sfxss; double tSfxs = 0.0;
	AnalyzeData(biliechis, biliechi, _T(";"));
	AnalyzeData(sfxss, sfxs, _T(";"));
	for(int idx = 0; idx < biliechis.GetSize(); ++idx)
	{
		biliechi = biliechis.GetAt(idx);
		if(_ttoi(biliechi) == m_scale)
			tSfxs = _tstof(sfxss[idx]);
	}
	if(fabs(tSfxs) > EPS)
		ScaleEntity(aname(m_sEnt), pt, tSfxs);
	if(true == flag)
	{
		ReplaceFieldValue();
		runCallBack();
		saveDwgToFile();
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	if(flag == true) acutPrintf(_T("\n 输出dwg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

bool XMJGExport::ExportDwg(MStr &field, XMJGCallBack callback)
{
	setCallBack(field, callback);
	acTransactionManagerPtr()->startTransaction();
	bool flag = combineEntityAndTk();
	if(true == flag)
	{
		TCHAR _scale[100] = {0};
		_stprintf(_scale, _T("%d"), m_DefaultScale);
		field[_T("#SCALE#")] = _scale;
		runCallBack();
		calcFontSize();
		saveDwgToFile();
	}
	acTransactionManagerPtr()->abortTransaction();
	if(flag == true) acutPrintf(_T("\n 输出dwg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

bool XMJGExport::ExportPicture(bool del)
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	bool flag = combineEntityAndTk();
	runCallBack();
	if(true == flag)
	{
		saveDwgToPicture(del);
	}
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	if(flag == true) acutPrintf(_T("\n 输出jpg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

bool XMJGExport::ExportDwg2JPG(bool del /* = false */)
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	bool flag = saveDwgToPicture(del);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	if(flag == true) acutPrintf(_T("\n 输出jpg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

void XMJGExport::setReplaceField(const MStr &fieldValue)
{
	m_ReplaceText = fieldValue;
}

void XMJGExport::setBuildNo(TCHAR *no)
{
	m_BuildNo.Format(_T("%s"), no);
}

void XMJGExport::setFloorNo(TCHAR *no)
{
	m_FloorNo.Format(_T("%s"), no);
}

void XMJGExport::setCallBack(MStr &replace, XMJGCallBack back)
{
	m_ReplaceText = replace;
	m_callback = back;
}

#pragma endregion 工程测量输出DWG

#pragma region 竣工测量输出DWG

JGExportDWG::JGExportDWG()
	: m_isfilterobject(false)
{
	m_compass = 0.0;
	m_DefaultScale = 0;
	m_replaceflag = false;
	m_Block = new SBreakBlock;
	m_Objects.removeAll();
	m_callback = 0;
	IDataBaseOper oper;
	oper.readCommonTable(_T("建筑竣工变更注记大小"), m_bgbj._size);
	oper.readCommonTable(_T("建筑竣工变更注记图层"), m_bgbj._layer);
	oper.readCommonTable(_T("建筑竣工变更注记颜色"), m_bgbj._color);
}

JGExportDWG::~JGExportDWG()
{
	if(m_Block != NULL)
		delete m_Block;
	m_Block = NULL;
}

bool JGExportDWG::setExportDir(TCHAR *dir /* = NULL */)
{
	bool flag = true;
	if(dir == NULL)
		flag = GetDirectory(_T("请选择保存的路径"), dir);
	if(false == flag) return false;
	m_ExportDir.Format(_T("%s"), dir);
	return true;
}

void JGExportDWG::setObjects(const AcDbObjectIdArray &objs)
{
	m_Objects.removeAll();
	m_Objects.setLogicalLength(0);
	m_Objects.setPhysicalLength(0);
	m_Objects.append(objs);
}

void JGExportDWG::setPictureFrame(const TCHAR *pictureFrame, double angle)
{
	m_PictureFrame.Format(_T("%s"), pictureFrame);
	m_compass = angle;
}

void JGExportDWG::setSavePath(const TCHAR *savePath)
{
	m_SaveDwgPath.Format(_T("%s"), savePath);
}

void JGExportDWG::isDrawRectangle(bool isdraw)
{
	m_isDraw = isdraw;
}

void JGExportDWG::setSavePath(const CString &savePath)
{
	m_SaveDwgPath.Format(_T("%s"), savePath);
}

void JGExportDWG::setOffsetDistance(double offsetDist)
{
	m_OffsetDist = offsetDist;
}

bool JGExportDWG::setExportRange()
{
	SNodesOpe nodesO; ads_name ents; ads_point pMin, pMax;
	if(m_Objects.length() == 0)
	{
		if(RTNORM != ads_getpoint(NULL, _T("\n <请框选要输出的实体范围> 请按下第一个点："), pMin)) return false;
		if(RTNORM != ads_getcorner(pMin, _T("\n <请框选要输出的实体范围> 请按下第二个点："), pMax)) return false;
	}
	else
	{
		GetEntsExtent(m_Objects, pMin, pMax);
	}
	ZoomWindow(pMin, pMax);
	nodesO.modifyLB_RTPoint(pMin, pMax);
	m_LeftButton = apoint(pMin);
	m_RightTop = apoint(pMax);
	nodesO.LB_RT_Nodes(m_LeftButton, m_RightTop, m_Rectangular);
	AcDbObjectId tkid = DrawPolyline2D(m_Rectangular, 1.0, true, _T("2000000"));
	TrimPoly(aname(tkid), true, false, true);	EntDel(aname(tkid));
	ads_command(RTSTR, _T("PURGE"), RTSTR, _T("B"), RTSTR, _T("*"), RTSTR, _T("N"), RTNONE);
	if(!ssFromNodes(m_Rectangular, ents, _T("CP"))) return false;
	m_Objects.setLogicalLength(0); SSToIdArr(ents, m_Objects);
	GetEntsExtent(m_Objects, pMin, pMax);
	nodesO.modifyLB_RTPoint(pMin, pMax);
	m_LeftButton.x = pMin[X]; m_LeftButton.y = pMin[Y];
	m_RightTop.x = pMax[X]; m_RightTop.y = pMax[Y];
	m_ObjMiddle = GetMiddlePoint(apoint(m_LeftButton), apoint(m_RightTop));
	return true;
}

void JGExportDWG::addCompass()
{
	AcGePoint3d pos;
	if(false == m_Block->findPointByHyperLink(_T("指北针"), pos)) return;
	AcDbObjectId id = DrawInsert(apoint(pos), _T("Compass"), m_scale, m_scale, m_scale, m_compass);
	m_PictureFrameIds.append(id);
}
// 
// void JGExportDWG::addCompass(double dScale)
// {
// 	AcGePoint3d pos;
// 	if(false == m_Block->findPointByHyperLink(_T("指北针"), pos)) return;
// 	AcDbObjectId id = DrawInsert(apoint(pos), _T("Compass"), dScale, dScale, dScale, m_compass);
// 	m_PictureFrameIds.append(id);
// }

void JGExportDWG::runCallBack()
{
	TCHAR scale[24] = {0};
	_stprintf(scale, _T("%d"), m_DefaultScale);
	m_ReplaceText[_T("#SCALE#")] = scale;
	if(NULL == m_callback) return;
	m_callback(m_ReplaceText, m_Block);
}

bool JGExportDWG::setDefaultScale()
{
	m_DefaultScale = (int)(1000.0 / m_scale);
	int val = m_DefaultScale % 100;
	m_DefaultScale /= 100;
	if(val* 1.0 / 100 > 0.05) m_DefaultScale++;
	m_DefaultScale *= 100; m_scale = 1000.0 / m_DefaultScale;
	return true;
}

void JGExportDWG::CalculateScale()
{
	double TKH = m_tkRightTop.y - m_tkLeftButton.y;
	double TKW = m_tkRightTop.x - m_tkLeftButton.x;
	double EntH = m_RightTop.y - m_LeftButton.y;
	double EntW = m_RightTop.x - m_LeftButton.x;
	double tkscale = TKH / EntH;
	double entscale = TKW / EntW;
	m_scale = tkscale < entscale ? tkscale : entscale;
}

bool JGExportDWG::InsertPictureFrame()
{
	AcGePoint2d pos(0, 0);
	double objw = m_RightTop.x - m_LeftButton.x;
	double objh = m_RightTop.y - m_LeftButton.y;
	if(objw < objh) m_PictureFrame.Append(_T("竖"));
	else m_PictureFrame.Append(_T("横"));
	AcDbObjectId tk = DrawInsert(apoint(pos), m_PictureFrame);
	if(tk == AcDbObjectId::kNull) return false;
	m_Block->SetBlock(tk); m_Block->Explode();
	m_PictureFrameIds.append(m_Block->m_objIdArr);
	AcDbObjectId lbid, rtid;
	m_Block->findPointByHyperLink(_T("左下角点"), m_tkLeftButton, lbid);
	m_Block->findPointByHyperLink(_T("右上角点"), m_tkRightTop, rtid);
	m_projectNameid = m_Block->findObjByHyperLink(_T("项目名称"));
	m_buildCompanyid = m_Block->findObjByHyperLink(_T("建设单位"));
	m_tkMiddle = GetMiddlePoint(m_tkLeftButton, m_tkRightTop);
	EntDel(aname(lbid)); EntDel(aname(rtid));
	return true;
}

void JGExportDWG::drawRectangle(AcDbObjectIdArray &idArr)
{
	if(m_isDraw == false) return; ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0); AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId id = DrawPolyline2D(node, 1.0, true, _T(""));
	setlayer(aname(id), _T("打印范围")); idArr.append(id);
}

bool JGExportDWG::combineEntityAndTk()
{
	RETFALSE(setExportRange()) false;
	if(0 == _tcslen(m_PictureFrame)) return false;
	RETFALSE(InsertPictureFrame()) false;
	if(m_isfilterobject == true)
		filterObject();
	CalculateScale();
	RETFALSE(setDefaultScale()) false;
	addCompass();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	ScaleEntitys(m_PictureFrameIds, m_tkMiddle, 1.0 / m_scale);
	moveEntitysToTk();
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	return true;
}

void JGExportDWG::RegenFontSize(int scale, AcDbObjectIdArray idArr)
{
	XMJGExportFontSizeConfig config(scale);
	vector<XMJGExportFontSizeConfigType> vType;
	vType.push_back(Text);
	vType.push_back(Number);
	vType.push_back(AreaAndRemark);
	for(int i = 0; i < vType.size(); ++i)
	{
		XMJGExportFontSizeConfigType type = vType.at(i);
		vector<CString> vLayers;
		config.GetLayers(type, vLayers);
		double fontSize = 1.0;
		config.GetFontSize(type, fontSize);
		for(int j = 0; j < idArr.length(); ++j)
		{
			AcDbObjectId id = idArr.at(j);
			ads_name ent;
			acdbGetAdsName(ent, id);
			TCHAR szEntType[255] = {0};
			GetEntityType(ent, szEntType);
			CString strEntType = szEntType;
			TCHAR szsLyer[255] = {0};
			GetEntLayer(ent, szsLyer);
			if(FindString(vLayers, szsLyer) != -1)
			{
				if(0 == _tcsicmp(szEntType, _T("TEXT")))
				{
					SetTextHeight(ent, fontSize);
				}
				else
				{
					SetMTextHeight(ent, fontSize);
				}
			}

		}
	}
}

void JGExportDWG::RegenCompassSize(int scale, AcDbObjectIdArray idArr)
{
	XMJGExportCompassConfig config(scale);

	{
		double compassScale = 1.0;
		config.GetScale(compassScale);
		for(int j = 0; j < idArr.length(); ++j)
		{
			AcDbObjectId id = idArr.at(j);
			ads_name ent;
			acdbGetAdsName(ent, id);
			TCHAR szEntType[20] = {0};
			GetEntityType(ent, szEntType);
			if(0 != _tcsicmp(szEntType, _T("INSERT")))
			{
				continue;
			}
			CString strBlock = GetBlockName(ent);
			if(0 == _tcsicmp(strBlock, _T("Compass")))
			{
				ads_point pt;
				GetEntPosition(ent, pt);
				ScaleEntity(ent, asPnt3d(pt), compassScale);
			}
		}
	}
}

void JGExportDWG::RegenLineWidth(int scale, AcDbObjectIdArray idArr)
{
	XMJGExportLineWidthConfig config(scale);
	vector<XMJGExportLineWidthConfigType> vType;
	vType.push_back(QSX);
	for(int i = 0; i < vType.size(); ++i)
	{
		XMJGExportLineWidthConfigType type = vType.at(i);
		vector<CString> vLayers;
		config.GetLayers(type, vLayers);
		double width = 0.03;
		config.GetLineWidth(type, width);
		for(int j = 0; j < idArr.length(); ++j)
		{
			AcDbObjectId id = idArr.at(j);
			ads_name ent;
			acdbGetAdsName(ent, id);
			TCHAR szEntType[255] = {0};
			GetEntityType(ent, szEntType);
			CString strEntType = szEntType;
			TCHAR szsLyer[255] = {0};
			GetEntLayer(ent, szsLyer);
			if(FindString(vLayers, szsLyer) != -1)
			{
				SetPolyWidth(ent, width);
			}
		}
	}
}

bool JGExportDWG::saveDwgToFile()
{
	CSaveBlocks blocks;
	m_SaveDwgPath.Format(_T("%s"), m_SaveDwgPath);
	blocks.setPath(m_SaveDwgPath);
	ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
	addCompass();
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr;
	SSToIdArr(ssSave, idArr);
	RegenFontSize(m_DefaultScale, idArr);
	RegenLineWidth(m_DefaultScale, idArr);
	RegenCompassSize(m_DefaultScale, idArr);
	drawRectangle(idArr);
	ads_command(RTSTR, _T("regen"), RTNONE);
	for(int idx = 0; idx < idArr.length(); ++idx)
	{
		AcDbObjectId id = idArr[idx]; TCHAR xs[25] = {0};
		if(ReadXdata(aname(id), _T("面积系数"), 0, xs) && ReadXdata(aname(id), _T("计容系数"), 0, xs))
			SetPolyWidth(aname(id), 0.06);
	}
	blocks.saveBlocks2(idArr, false);
	return true;
}

void JGExportDWG::moveEntitysToTk()
{
	double dXMove = m_ObjMiddle.x - m_tkMiddle.x;
	double dYMove = m_ObjMiddle.y - m_tkMiddle.y;
	MoveEntitys(m_PictureFrameIds, dXMove, dYMove);
}

bool JGExportDWG::saveDwgToPicture(bool flag)
{
	XMJGPrintPicture picture;
	picture.setObjectArray(m_Objects);
	picture.setPrintSizeName();
	picture.setPrintFormat(Raster_Jpg);
	picture.setPrintPath(m_SaveDwgPath);
	picture.BeginPrintPicture();
	return true;
}

bool JGExportDWG::setReplaceChangeFlag(bool flag)
{
	m_replaceflag = flag;
	return true;
}

void JGExportDWG::setObjectFilterFlag(XMJGFilterObject filter)
{
	m_filterobject = filter;
}

void JGExportDWG::ReplaceFieldValue()
{
	MSIter it = m_ReplaceText.begin();
	TCHAR field[255] = {0};
	TCHAR value[255] = {0};
	for(; it != m_ReplaceText.end(); ++it)
	{
		_stprintf(field, _T("%s"), it->first);
		_stprintf(value, _T("%s"), it->second);
		m_Block->replaceText(field, value);
	}
}

void JGExportDWG::ReplaceTableHeaderField()
{
	TCHAR blc[10] = {0}; TCHAR jrarea[200] = {0};
	TCHAR jzarea[200] = {0}; TCHAR jzzarea[200] = {0};
	TCHAR jrzarea[200] = {0};
	_stprintf(blc, _T("%d"), m_DefaultScale);
	StatisticArea(jzarea, jrarea, jzzarea, jrzarea);
	m_Block->replaceTextEx(_T("#SCALE#"), blc);
}

void JGExportDWG::StatisticArea(TCHAR *jzarea, TCHAR *jrarea, TCHAR *jzzarea, TCHAR *jrzarea)
{
	int len = m_Objects.length(); TCHAR area[200] = {0};
	double m_buildArea = 0, m_jrArea = 0.0; TCHAR floor[200] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		if(true != ReadXdata(aname(id), _T("建筑面积"), DOUBLE_TYPE, area)) continue;
		if(_tcslen(area) == 0) continue; m_buildArea += _tstof(area);
		ReadXdata(aname(id), _T("积容面积"), DOUBLE_TYPE, area);
		m_jrArea += _tstof(area);
	}
	CString build(m_FloorNo);
	int pos = build.FindOneOf(_T(" ---> "));
	if(pos == -1)
	{
		_stprintf(jrzarea, _T("%.3f"), m_jrArea);
		_stprintf(jzzarea, _T("%.3f"), m_buildArea);
	}
	else
	{
		int start = _tstoi(build.Mid(0, pos));
		pos = build.ReverseFind(_T(' '));
		int count = build.GetLength() - pos;
		int end = _tstoi(build.Mid(pos, count));
		_stprintf(jrzarea, _T("%.3f"), m_jrArea * (end - start + 1));
		_stprintf(jzzarea, _T("%.3f"), m_buildArea * (end - start + 1));
	}
	_stprintf(jrarea, _T("%.3f"), m_jrArea);
	_stprintf(jzarea, _T("%.3f"), m_buildArea);
}

bool JGExportDWG::replaceChangeNote()
{
	VStr bgzj;
	int len = m_Objects.length();
	TCHAR info[255] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		if(false == ReadXdata(aname(id), _T("变更序号"), 0, info)) continue;
		bgzj.push_back(info);
	}
	AcDbObjectId bgid; AcGePoint3d bgpos;
	m_Block->findPointByHyperLink(_T("变更注记"), bgpos, bgid);
	len = (int)bgzj.size();
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint3d temp(bgpos);
		temp.y += m_bgbj._size * (len - idx - 1) * 2;
		AcDbObjectId tbgid = DrawText(temp, bgzj.at(idx), _T("ML"), _T(""), m_bgbj._size);
		setlayer(aname(tbgid), m_bgbj._layer); setcolor(aname(tbgid), m_bgbj._color);
		m_PictureFrameIds.append(tbgid);
	}
	EntDel(aname(bgid));
	return true;
}

void JGExportDWG::replaceFCPMNote()
{
	int len = m_Objects.length();
	TCHAR layer[20] = {0};
	AcDbObjectIdArray ids;
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		GetEntLayer(id, layer);
		if(_tcscmp(layer, _T("变更标注")) == 0)
			ids.append(id);
	}
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		m_Objects.remove(ids.at(idx));
		EntDel(aname(ids.at(idx)));
	}
}

void JGExportDWG::filterObject()
{
	TCHAR info[20] = {0};
	for(int idx = 0; idx < m_Objects.length(); ++idx)
	{
		AcDbObjectId id = m_Objects.at(idx);
		if(true == ReadXdata(aname(id), _T("WJJX"), 0, info)) continue;
		m_Objects.remove(id);
		EntDel(aname(id));
	}
	switch(m_filterobject)
	{
		case ChangeAreaFlag:
			replaceChangeNote();
			break;
		case FenCengPingMianFlag:
			replaceFCPMNote();
			break;
		default:
			break;
	}
}

void JGExportDWG::calcFontSize()
{
	TCHAR info[255] = {0};
	ReadXdata(aname(m_projectNameid), _T("changdu"), 1, info);
	double len = _tstof(info);
	GetEntText(aname(m_projectNameid), info);
	int fontlen = (int)_tcslen(info);
	if(len < fontlen)
	{
		double height = GetTextHeight(aname(m_projectNameid));
		double size = len / fontlen * height;
		SetTextHeight(aname(m_projectNameid), size);
	}
	ReadXdata(aname(m_buildCompanyid), _T("changdu"), 1, info);
	len = _tstof(info);
	GetEntText(aname(m_buildCompanyid), info);
	fontlen = (int)_tcslen(info);
	if(len < fontlen)
	{
		double height = GetTextHeight(aname(m_buildCompanyid));
		double size = len / fontlen * height;
		SetTextHeight(aname(m_buildCompanyid), size);
	}
}

bool JGExportDWG::ExportDwg()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	bool flag = combineEntityAndTk();
	if(true == flag)
	{
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		ReplaceFieldValue();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		runCallBack();
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();
		saveDwgToFile();
	}
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	if(flag == true) acutPrintf(_T("\n 输出dwg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

bool JGExportDWG::ExportDwg(MStr &field, XMJGCallBack callback)
{
	setCallBack(field, callback);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	bool flag = combineEntityAndTk();
	if(true == flag)
	{
		TCHAR _scale[100] = {0};
		_stprintf(_scale, _T("%d"), m_DefaultScale);
		field[_T("#SCALE#")] = _scale;
		runCallBack();
		calcFontSize();
		saveDwgToFile();
	}
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	if(flag == true) acutPrintf(_T("\n 输出dwg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

bool JGExportDWG::ExportPicture(bool del)
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	bool flag = combineEntityAndTk();
	runCallBack();
	if(true == flag)
	{
		saveDwgToPicture(del);
	}
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	if(flag == true) acutPrintf(_T("\n 输出jpg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

bool JGExportDWG::ExportDwg2JPG(bool del /* = false */)
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
	bool flag = saveDwgToPicture(del);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
	if(flag == true) acutPrintf(_T("\n 输出jpg完成。。。"));
	else acutPrintf(_T("\n 输出被中断。。。"));
	return flag;
}

void JGExportDWG::setReplaceField(const MStr &fieldValue)
{
	m_ReplaceText = fieldValue;
}

void JGExportDWG::setBuildNo(TCHAR *no)
{
	m_BuildNo.Format(_T("%s"), no);
}

void JGExportDWG::setFloorNo(TCHAR *no)
{
	m_FloorNo.Format(_T("%s"), no);
}

void JGExportDWG::setCallBack(MStr &replace, XMJGCallBack back)
{
	m_ReplaceText = replace;
	m_callback = back;
}

#pragma endregion 竣工测量输出DWG

int SearchCallback(void* id, void* arg)
{
	std::vector<void *> * pList = (std::vector<void *>*)arg;
	void * n = (void *)id;
	pList->push_back(n);
	return 1;
}

XMJGDimensions::XMJGDimensions()
{
	m_Color = -1;
	m_Dist = 50;
}

XMJGDimensions::~XMJGDimensions()
{
	if(m_Tree != NULL)
		delete m_Tree;
	m_Tree = NULL;
}

void XMJGDimensions::setPoint(const AcGePoint2d &pos)
{
	m_Point = pos;
}

void XMJGDimensions::setEntityLayer(const TCHAR *layer)
{
	if(layer == NULL) return;
	m_Layer.Format(_T("%s"), layer);
}

void XMJGDimensions::setEntityColor(int red, int green /* = -1 */, int blue /* = -1 */)
{
	if(green == blue && blue == -1)
		m_Color = red;
}

bool XMJGDimensions::beginDimensions()
{
	buildNodes();
	if(false == ssFromNodes(m_ObjectIDs, m_Nodes, 2)) return false;
	if(m_ObjectIDs.length() < 4) return false;
	InitTree();
	return searchNearestBoders();
}

AcDbObjectId XMJGDimensions::getEntityId()
{
	return m_ObjectID;
}

void XMJGDimensions::setSearchDist(double dist)
{
	m_Dist = dist;
}

void XMJGDimensions::buildNodes()
{
	m_Nodes.removeAll();
	AcGePoint2d lt, rt, rb, lb;
	lt.x = lb.x = m_Point.x - m_Dist;
	lt.y = rt.y = m_Point.y + m_Dist;
	rb.x = rt.x = m_Point.x + m_Dist;
	rb.y = lb.y = m_Point.y - m_Dist;
	m_Nodes.append(lb);  m_Nodes.append(lt);
	m_Nodes.append(rt);  m_Nodes.append(rb);
}

void XMJGDimensions::drawEntity()
{

}

void XMJGDimensions::setPropery()
{
	if(m_Layer.GetLength() != 0)
		setlayer(aname(m_ObjectID), m_Layer);
	if(m_Color != -1)
		setcolor(aname(m_ObjectID), m_Color);
}

void XMJGDimensions::filterEntity()
{
	AcDbObjectIdArray IdArr;
	IdArr.append(m_ObjectIDs);
	int len = IdArr.length();
	TCHAR EntityType[20] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = IdArr.at(idx);
		GetEntityType(aname(id), EntityType);
		if(_tcscmp(EntityType, _T("LINE")) != 0)
			m_ObjectIDs.remove(id, 0);
	}
}

bool XMJGDimensions::searchNearestBoders()
{
	CRTreeMBR *Mbr = new CRTreeMBR;
	Mbr->m_bound[0] = m_Nodes.at(0).x;
	Mbr->m_bound[1] = m_Nodes.at(0).y;
	Mbr->m_bound[2] = m_Nodes.at(2).x;
	Mbr->m_bound[3] = m_Nodes.at(2).y;
	vector<void *> vincludeID;
	m_Tree->SearchInclude(Mbr, SearchCallback, &vincludeID);
	vector<AcGePoint2dArray> vNodes;
	for(int idx = 0; idx < (int)vincludeID.size(); ++idx)
	{
		AcDbObjectId id;
		id.setFromOldId((Adesk::IntDbId)vincludeID.at(idx));
		AcGePoint2dArray node;
		GetPlList(aname(id), node);
		vNodes.push_back(node);
	}
	CArray< AcGePoint2dArray, AcGePoint2dArray > arrNodes;
	CArray< AcGeDoubleArray, AcGeDoubleArray > arrBulge;
	SNodesOpe nodeOpe;
	nodeOpe.getBorder(vNodes, arrNodes, arrBulge);
	for(int idx = 0; idx < arrNodes.GetSize(); ++idx)
	{
		if(-1 != PointInPolygon(arrNodes.GetAt(idx), m_Point)) continue;
		m_ObjectID = DrawPolyline2D(arrNodes.GetAt(idx), 0.1, true, _T(""));
		if(m_ObjectID == AcDbObjectId::kNull) continue;
		setPropery();
		return true;
	}
	return false;
}

void XMJGDimensions::InitTree()
{
	m_Tree = new CRTree;
	TCHAR type[255] = {0};
	int len = m_ObjectIDs.length();
	ads_point ptMin, ptMax;
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = m_ObjectIDs.at(idx);
		GetEntityType(aname(id), type);
		if(0 != _tcscmp(type, _T("LINE")) && 0 != _tcscmp(type, _T("LWPOLYLINE"))) continue;
		GetEntExtent(aname(id), ptMin, ptMax, 0.5);
		CRTreeMBR *mbr = new CRTreeMBR;
		mbr->m_bound[0] = ptMin[X];
		mbr->m_bound[1] = ptMin[Y];
		mbr->m_bound[2] = ptMax[X];
		mbr->m_bound[3] = ptMax[Y];
		m_Tree->InsertRect(mbr, (void *)id.asOldId());
	}
}

XMJGCheckQuality::XMJGCheckQuality()
{

}

XMJGCheckQuality::~XMJGCheckQuality()
{

}

void XMJGCheckQuality::CheckQualityRedLine()
{
	XMJGRedLineQuality quality;
	if(IDOK != quality.DoModal()) return;
	if(false == SelectEntitys(m_ObjectIDArr, _T(""))) return;
	if(true == quality.getCheckCoorAnn())
		checkRedLineCoorAnn();
	if(true == quality.getCheckLenghtAnn())
		checkRedLineLenghtAnn();
	if(true == quality.getCheckSegmentAnn())
		checkRedLineSegmentAnn();
	if(true == quality.getCheckRedLineNodeAnn())
		checkNoCrossRedLineNodeAnn();
	AGrid_ShowMe();
}

void XMJGCheckQuality::checkRedLineCoorAnn()
{
	AGrid_ClearSheet(_T("红线节点坐标注记检查"));
	int len = m_ObjectIDArr.length(); TCHAR szData[20] = {0}; TCHAR layer[20] = {0};
	acedSetStatusBarProgressMeter(_T("检查红线的节点处是不是有坐标标注。。。"), 0, len);
	for(int idx = 0; idx < len; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = m_ObjectIDArr.at(idx);
		CEntOper::getlayer(id, layer);
		if(0 != _tcscmp(layer, _T("红线"))) continue;
		int count = GetXDataCount(aname(id), _T("红线标注编号"));
		AcGePoint2dArray nodes; GetPlList(aname(id), nodes);
		int Count = nodes.length();
		if(Count == count) continue; TCHAR info[20] = {0};
		for(int indx = 0; indx < Count; ++indx)
		{
			READMulXData(id, _T("红线标注编号"), szData, 0, indx);
			_stprintf(info, _T("%d"), indx);
			if(0 == _tcscmp(info, szData)) continue;
			_stprintf(szData, _T("节点%d没有标注！！！"), indx + 1);
			AGrid_AppendMsgEnt(_T("红线节点坐标注记检查"), szData, _T("错误"), id);
		}
	}
	acedRestoreStatusBar();
}

void XMJGCheckQuality::checkRedLineLenghtAnn()
{
	AcDbObjectIdArray ids; ids.append(m_ObjectIDArr);
	int len = ids.length(); TCHAR szData[20] = {0};
	AGrid_ClearSheet(_T("坐标标注与坐标注记内容是否一致"));
	acedSetStatusBarProgressMeter(_T("检查坐标标注与坐标注记内容是否一致。。。"), 0, len);
	for(int idx = 0; idx < len; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = ids.at(idx);
		CEntOper::getlayer(id, szData);
		if(0 != _tcscmp(szData, _T("红线"))) continue;
		int count = GetXDataCount(aname(id), _T("红线标注句柄"));
		for(int indx = 0; indx < count; ++indx)
		{
			READMulXData(id, _T("红线标注句柄"), szData, 0, indx);
			AcDbObjectId obj = GetObjectByHandle(szData);
			if(true == checkCoor(obj)) continue;
			AGrid_AppendMsgEnt(_T("坐标标注与坐标注记内容是否一致"), _T("该坐标标注与坐标注记内容不一致"), _T("错误"), obj);
		}
	}
}

void XMJGCheckQuality::checkRedLineSegmentAnn()
{

}

void XMJGCheckQuality::checkNoCrossRedLineNodeAnn()
{
	int len = m_ObjectIDArr.length(); TCHAR szData[20] = {0};
	AcDbObjectIdArray IdArr; IdArr.append(m_ObjectIDArr); vector<AcDbObjectId> ids;
	acedSetStatusBarProgressMeter(_T("检查不经过红线节点的坐标标注。。。"), 0, len);
	for(int idx = 0; idx < len; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = m_ObjectIDArr.at(idx);
		CEntOper::getlayer(id, szData);
		if(0 != _tcscmp(szData, _T("红线")) && 0 != _tcscmp(szData, _T("标注")))
		{
			IdArr.remove(id, 0); continue;
		}
		if(0 == _tcscmp(szData, _T("标注"))) continue;
		IdArr.remove(id, 0);
		int count = GetXDataCount(aname(id), _T("红线标注句柄"));
		for(int indx = 0; indx < count; ++indx)
		{
			READMulXData(id, _T("红线标注句柄"), szData, 0, indx);
			AcDbObjectId obj = GetObjectByHandle(szData);
			int Count = GetXDataCount(aname(obj), _T("标注句柄"));
			for(int i = 0; i < Count; ++i)
			{
				READMulXData(obj, _T("标注句柄"), szData, 0, i);
				AcDbObjectId ID = GetObjectByHandle(szData);
				IdArr.remove(ID, 0);
			}
		}
	}
	AGrid_ClearSheet(_T("不经过红线节点的坐标标注"));
	for(int idx = 0; idx < IdArr.length(); ++idx)
		AGrid_AppendMsgEnt(_T("不经过红线节点的坐标标注"), _T("该标注没有经过红线节点"), _T("错误"), IdArr.at(idx));
	acedRestoreStatusBar();
}

bool XMJGCheckQuality::checkCoor(AcDbObjectId &id)
{
	TCHAR szData[20] = {0}; bool flag = false;
	int Count = GetXDataCount(aname(id), _T("坐标"));
	int count = GetXDataCount(aname(id), _T("标注句柄"));
	for(int idx = 0; idx < count; ++idx)
	{
		READMulXData(id, _T("标注句柄"), szData, 0, idx);
		AcDbObjectId obj = GetObjectByHandle(szData);
		GetEntityType(aname(obj), szData);
		if(0 != _tcscmp(szData, _T("TEXT"))) continue;
		AcDbEntity *pEnt = NULL;
		if(Acad::eOk != acdbOpenAcDbEntity(pEnt, obj, AcDb::kForRead)) continue;
		if(pEnt->isA() != AcDbText::desc()) continue;
		AcDbText *pText = AcDbText::cast(pEnt);
		TCHAR *info = pText->textString();
		pEnt->close();
		READMulXData(id, _T("坐标"), szData, 0, idx);
		if(0 != _tcscmp(szData, info)) return false;
	}
	return true;
}

CDrawArrow::CDrawArrow()
{
	read1(m_Scale);
	m_Scale /= 1000;
}

CDrawArrow::~CDrawArrow()
{

}

AcDbObjectId CDrawArrow::DrawLeader(ads_point spt, ads_point mpt, float font, const TCHAR *layer, int color, const CString &info)
{
	ads_name head; ads_entlast(head);
	int old_osmode = PickstyleReset(0);
	double len = _tcslen(info) * font * m_Scale * 2;
	ads_command(RTSTR, _T("QLEADER"), RTPOINT, spt, RTPOINT, mpt, RTSTR, _T("\0"), RTREAL, len, RTSTR, info, RTSTR, _T(""), RTNONE);
	PickstyleBack(old_osmode);
	acedCommand(RTSTR, _T("Regen"), RTNONE);
	getQLeaderText(head, font, layer, color);
	ads_name ent; AcDbObjectId id;
	ads_entlast(ent);
	acdbGetObjectId(id, ent);
	return id;
}

AcDbObjectId CDrawArrow::DrawLeader(ads_point spt, ads_point mpt, float font, const TCHAR *layer, int color)
{
	TCHAR info[32] = {0};
	_stprintf(info, _T("X=%09.3f\nY=%09.3f"), spt[Y], spt[X]);
	ads_name head;
	ads_entlast(head);
	int old_osmode = PickstyleReset(0);
	ads_command(RTSTR, _T("QLEADER"), RTPOINT, spt, RTPOINT, mpt, RTSTR, _T("\0"), RTREAL, 0.3, RTSTR, info, RTSTR, _T(""), RTNONE);
	PickstyleBack(old_osmode);
	acedCommand(RTSTR, _T("Regen"), RTNONE);
	getQLeaderText(head, font, layer, color);
	ads_name ent; AcDbObjectId id;
	ads_entlast(ent);
	acdbGetObjectId(id, ent);
	return id;
}

void CDrawArrow::getQLeaderText(ads_name head, float font, const TCHAR *layer, int color)
{
	bool flag = false;
	ads_name ent;
	while(flag == false)
	{
		ads_entnext(head, ent);
		if(ent[0] == 0) return;
		CString Type; TCHAR type[20] = {0};
		GetEntityType(ent, type);
		Type = type;
		if(Type.CompareNoCase(_T("mtext")) == 0 || Type.CompareNoCase(_T("text")) == 0)
		{
			flag = true;
			SetTextHeight(ent, m_Scale * font);
		}
		setlayer(ent, layer);
		setcolor(ent, color);
		ads_name_set(ent, head);
	}
	if(flag == false) return;
	AcDbEntity *pEnt;
	AcDbObjectId objId;
	acdbGetObjectId(objId, ent);
	if(Acad::eOk != acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite)) return;
	if(false == pEnt->isKindOf(AcDbMText::desc())) return;
	AcDbMText *pText = (AcDbMText *)pEnt;
	pText->setWidth(10);
	pText->setTextHeight(font * m_Scale);
	pText->close();
}

AcDbObjectId CDrawArrow::DrawArrow(const AcDbObjectId &id, double length)
{
	AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes);
	bool bDeasil = IsDeasil(nodes);
	TCHAR info1[200] = {0};
	TCHAR info2[200] = {0};
	int len = nodes.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint2d cor = nodes.at(idx);
		ads_point pt; double angle = 0.0;
		GetOutCornerAndPosition(nodes, idx, bDeasil, pt, angle);
		AcDbObjectId group;
		DrawArrow(apoint(pt), angle, group, length);
		CString handle = GetHandleByObject(group);
		AddPropertyToXData(aname(id), group, _T("红线标注句柄"), idx);	//红线检查
		AddAnnotionLength(group, length * m_Scale);
		AddCoorToObject(group, cor);
		_stprintf(info1, _T("X=%.3f"), cor.y);  //x坐标
		_stprintf(info2, _T("Y=%.3f"), cor.x);  //y坐标（因为cad坐标与真实坐标相反）
		AddTextInfo(group, angle, info1, info2);
	}
	return NULL;
}

void CDrawArrow::GetArrowLastPoint(const AcDbObjectId & id, AcGePoint2d &last)
{
	last.x = last.y = -99999.0;
	vector<AcDbObjectId> ids;
	ads_point pos;
	GetEntPosition(aname(id), pos);
	GetGroupEnts(id, ids);
	int len = (int)ids.size();
	double dist = 0;
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint2dArray nodes;
		GetPlList(aname(ids.at(idx)), nodes);
		int count = nodes.length();
		for(int indx = 0; indx < count; ++indx)
		{
			AcGePoint2d tmp = nodes.at(indx);
			double dis = distOF2d(apoint(pos), tmp);
			if(dist > dis && dis > EPS) continue;
			dist = dis;
			last = tmp;
		}
	}
}

AcDbObjectId CDrawArrow::DrawArrow(const AcGePoint2d &fPt, const AcGePoint2d &sPt, double length)
{
	double angle = ads_angle(apoint(sPt), apoint(fPt));
	AcDbObjectId id, group;	ads_point IPt; double dist;
	AcDbObjectId ent;
	int index = getEntityByPoint(fPt, ent);
	dist = distOF2d(fPt, sPt);
	if(dist <= 0.36 * m_Scale)
	{
		id = drawline(apoint(fPt), apoint(sPt), _T(""));
		AddPropertyToXData(id, id, _T("标注句柄"));
		setlayer(aname(id), _T("标注"));
		CreateGroup(aname(id), group);
	}
	else
	{
		ads_polar(apoint(fPt), angle, -0.18 * m_Scale, IPt);
		id = DrawInsert(apoint(IPt), _T("Arrow"), m_Scale, m_Scale, 1.0, angle);
		AddPropertyToXData(id, id, _T("标注句柄"));
		setlayer(aname(id), _T("标注"));
		CreateGroup(aname(id), group);
		AcDbObjectId lid1 = drawline(IPt, apoint(sPt), _T(""));
		AddPropertyToXData(id, lid1, _T("标注句柄"));
		setlayer(aname(lid1), _T("标注"));
		AddEntToGroup(lid1, group);
	}
	AddPropertyToXData(ent, id, _T("红线标注句柄"), index);	//红线检查
	ads_point ept;
	double corner = PI / 2;
	if(angle < corner || angle > 3 * corner)
		ads_polar(apoint(sPt), PI, 0.54 * m_Scale, ept);   //水平线的长度0.54
	else
		ads_polar(apoint(sPt), 0, 0.54 * m_Scale, ept);
	AcDbObjectId lid2 = drawline(apoint(sPt), apoint(ept), _T(""));
	AddPropertyToXData(id, lid2, _T("标注句柄"));
	setlayer(aname(lid2), _T("标注"));
	AddAnnotionLength(id, length);
	AddEntToGroup(lid2, group);
	AddCoorToObject(id, fPt);
	TCHAR info1[200] = {0};
	TCHAR info2[200] = {0};
	_stprintf(info1, _T("X=%.3f"), fPt.y);
	_stprintf(info2, _T("Y=%.3f"), fPt.x);
	AddTextInfo(id, angle, info1, info2);
	return group;
}

void CDrawArrow::AddAnnotionLength(AcDbObjectId &id, double length)
{
	TCHAR len[200] = {0};
	_stprintf(len, _T(".3f%"), length);
	AddXdata(aname(id), _T("注记长度"), 3, len);
}

void CDrawArrow::AddCoorToObject(AcDbObjectId &id, const AcGePoint2d &pos)
{
	TCHAR info1[200] = {0}, info2[200] = {0};
	_stprintf(info1, _T("X=%.3f"), pos.y);
	_stprintf(info2, _T("Y=%.3f"), pos.x);
	ADDMulXData(id, _T("坐标"), info1, 0, 0);
	ADDMulXData(id, _T("坐标"), info2, 0, 1);
}

void CDrawArrow::AddTextInfo(AcDbObjectId &id, double angle, TCHAR *info1, TCHAR *info2)
{
	AcGePoint2d last; TCHAR szText[200] = {0};
	GetArrowLastPoint(id, last); ads_point textpos;
	double font = 1.5 * m_Scale; textpos[X] = last.x;
	textpos[Y] = last.y + font + 0.05;
	AcDbObjectId group;  GetEntGroupEx(id, group);
	double corner = PI / 2;
	TCHAR *align = new TCHAR[4];
	if(angle < corner || angle > 3 * corner)
		_stprintf(align, _T("%s"), _T("TR"));
	else
		_stprintf(align, _T("%s"), _T("TL"));
	AcDbObjectId id1 = DrawText(textpos, info1, align, _T(""), font);
	AddPropertyToXData(id, id1, _T("标注句柄"), 0);
	setlayer(aname(id1), _T("标注"));
	AddEntToGroup(id1, group);	textpos[Y] = last.y - 0.05;
	if(info2 == NULL)	return;
	AcDbObjectId id2 = DrawText(textpos, info2, align, _T(""), font);
	AddPropertyToXData(id, id2, _T("标注句柄"), 1);
	setlayer(aname(id2), _T("标注"));
	AddEntToGroup(id2, group);
	delete align; align = NULL;
}

bool CDrawArrow::DrawInsertArrow(const AcGePoint2d &pos, const double &angle, AcDbObjectId &id)
{
	AcDbObjectId group;
	id = DrawInsert(apoint(pos), _T("Arrow"), m_Scale, m_Scale, 1.0, angle);
	if(id == AcDbObjectId::kNull) return false;
	setlayer(aname(id), _T("标注"));
	CreateGroup(aname(id), group);
	return true;
}

void CDrawArrow::DrawDiagonalLine(AcDbObjectId &group, const AcGePoint2d &spt, const AcGePoint2d &ept)
{
	AcDbObjectId lineid1 = drawline(apoint(spt), apoint(ept), _T(""));
	setlayer(aname(lineid1), _T("标注"));
	AddEntToGroup(lineid1, group);
}

void CDrawArrow::DrawHorizontalLine(AcDbObjectId &group, const double &angle, const AcGePoint2d &pt, double length)
{
	double corner = PI / 2; ads_point ept;
	if(angle < corner || angle > 3 * corner)
		ads_polar(apoint(pt), PI, length, ept);
	else
		ads_polar(apoint(pt), 0, length, ept);
	AcDbObjectId lineid2 = drawline(apoint(pt), ept, _T(""));
	setlayer(aname(lineid2), _T("标注"));
	AddEntToGroup(lineid2, group);
}

void CDrawArrow::AddPropertyToXData(AcDbObjectId &id, AcDbObjectId &obj, const TCHAR *AppName, int idx)
{
	CString handle = GetHandleByObject(obj);
	ADDMulXData(id, AppName, handle, 0, idx);
}

AcDbObjectId CDrawArrow::DrawArrow(AcDbObjectId &id, const AcGePoint2d &pos, TCHAR *Info)
{
	XMJGAssist assist; AcGePoint2dArray nodes;
	GetPlList(aname(id), nodes); ads_point pt;
	bool Deasil = IsDeasil(nodes); double angle;
	int idx = assist.GetPoint2PolyRect(nodes, pos);
	GetOutCornerAndPosition(nodes, idx, Deasil, pt, angle);
	DrawArrow(apoint(pt), angle, id);
	AddTextInfo(id, angle, Info);
	return id;
}

AcDbObjectId CDrawArrow::DrawArrow(const AcGePoint2d &fPt, const AcGePoint2d &sPt, TCHAR * inserttext)
{
	double angle = ads_angle(apoint(sPt), apoint(fPt));
	AcDbObjectId id, group;	ads_point IPt; double dist;
	dist = distOF2d(fPt, sPt);
	if(dist <= 1.0)
	{
		id = drawline(apoint(fPt), apoint(sPt), _T(""));
		CreateGroup(aname(id), group);
	}
	else
	{
		ads_polar(apoint(fPt), angle, -0.18, IPt);
		id = DrawInsert(apoint(IPt), _T("Arrow"), m_Scale, m_Scale, 1.0, angle);
		CreateGroup(aname(id), group);
		AcDbObjectId lid1 = drawline(IPt, apoint(sPt), _T(""));
		AddEntToGroup(lid1, group);
	}
	ads_point ept;
	double corner = PI / 2;
	if(angle < corner || angle > 3 * corner)
		ads_polar(apoint(sPt), PI, 10, ept);
	else
		ads_polar(apoint(sPt), 0, 10, ept);
	AcDbObjectId lid2 = drawline(apoint(sPt), apoint(ept), _T(""));
	AddEntToGroup(lid2, group);
	AddTextInfo(id, angle, inserttext);
	return group;
}

void CDrawArrow::DrawArrow(const AcGePoint2d &IPt, const double &angle, AcDbObjectId &id, double length)
{
	AcDbObjectId group; ads_point spt, ept;
	id = DrawInsert(apoint(IPt), _T("Arrow"), m_Scale, m_Scale, 1.0, angle);
	AddPropertyToXData(id, id, _T("标注句柄"));
	setlayer(aname(id), _T("标注"));
	CreateGroup(aname(id), group);
	ads_polar(apoint(IPt), angle + PI, length * m_Scale, spt);
	AcDbObjectId lineid1 = drawline(apoint(IPt), spt, _T(""));
	AddPropertyToXData(id, lineid1, _T("标注句柄"));
	setlayer(aname(lineid1), _T("标注"));
	AddEntToGroup(lineid1, group);
	double corner = PI / 2;
	if(angle < corner || angle > 3 * corner)
		ads_polar(spt, PI, 0.54 * m_Scale, ept);
	else
		ads_polar(spt, 0, 0.54 * m_Scale, ept);
	AcDbObjectId lineid2 = drawline(spt, ept, _T(""));
	AddPropertyToXData(id, lineid2, _T("标注句柄"));
	setlayer(aname(lineid2), _T("标注"));
	AddEntToGroup(lineid2, group);
}

AcDbObjectId CDrawArrow::DrawStraightArrow(const AcGePoint2d &spt, const AcGePoint2d &ept, double dscale, int color)
{
	double _angle = ads_angle(apoint(spt), apoint(ept));
	AcDbObjectId id, group;
	id = DrawInsert(apoint(ept), _T("Arrow"), m_Scale, m_Scale, 1.0, _angle);
	setlayer(aname(id), _T("照片"));
	ChangeBlockRefenceColor(aname(id), color);
	CreateGroup(aname(id), group);
	AcDbObjectId lineid = drawline(apoint(spt), apoint(ept), _T(""));
	setlayer(aname(lineid), _T("照片"));
	setcolor(aname(lineid), color);
	AddEntToGroup(lineid, group);
	return group;
}

AcDbObjectId CDrawArrow::GetArrowObjectID(AcDbObjectId &id, bool del)
{
	vector<AcDbObjectId> ids;
	GetGroupEnts(id, ids);
	ids.push_back(id);
	int len = (int)ids.size();
	TCHAR info[200] = {0}; bool bFind = false;
	AcDbObjectId ret; int index = -1;
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId tmp = ids.at(idx);
		if(false == READMulXData(tmp, _T("坐标"), info, 0, 0)) continue;
		if(false == READMulXData(tmp, _T("坐标"), info, 0, 1)) continue;
		ret = tmp; index = idx; bFind = true; break;
	}
	if(bFind == false) return AcDbObjectId::kNull;
	if(del == false) return ret;
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId tmp = ids.at(idx);
		if(idx != index) EntDel(aname(tmp));
	}
	return ret;
}

void CDrawArrow::GetOutCornerAndPosition(AcGePoint2dArray &Nodes, int i, bool bDeasil, ads_point &Insertpt, double &Angle)
{
	ads_point pt, pt1, pt2;
	double angle, dAngle1, dAngle2, dTemp;

	int nLen = Nodes.length();
	if(nLen < 2) return;
	pt[X] = Nodes[i].x; pt[Y] = Nodes[i].y; pt[Z] = 0.0;
	if(0 == i)
	{
		pt1[X] = Nodes[nLen - 1].x;
		pt1[Y] = Nodes[nLen - 1].y;
		pt1[Z] = 0.0;
	}
	else
	{
		pt1[X] = Nodes[i - 1].x;
		pt1[Y] = Nodes[i - 1].y;
		pt1[Z] = 0.0;
	}
	if(nLen - 1 == i)
	{
		pt2[X] = Nodes[0].x;
		pt2[Y] = Nodes[0].y;
		pt2[Z] = 0.0;
	}
	else
	{
		pt2[X] = Nodes[i + 1].x;
		pt2[Y] = Nodes[i + 1].y;
		pt2[Z] = 0.0;
	}

	dAngle1 = ads_angle(pt, pt1);
	dAngle2 = ads_angle(pt, pt2);

	if(true == bDeasil)
	{
		dTemp = dAngle1 - dAngle2;
		if(dTemp < 0.0)
			dTemp += PI * 2;
		angle = dAngle2 + dTemp / 2;
	}
	else
	{
		dTemp = dAngle2 - dAngle1;
		if(dTemp < 0.0)
			dTemp += PI * 2;
		angle = dAngle1 + dTemp / 2;
	}
	ads_polar(pt, angle, 0.18 * m_Scale, Insertpt);
	Angle = ads_angle(Insertpt, pt);
}

int CDrawArrow::getEntityByPoint(const AcGePoint2d &pt, AcDbObjectId &id)
{
	AcGePoint2d lb, lt, rt, rb; AcGePoint2dArray node;
	double offset = 0.5;
	lt.x = lb.x = pt.x - offset; rb.y = lb.y = pt.y - offset;
	rt.x = rb.x = pt.x + offset; lt.y = rt.y = pt.y + offset;
	node.append(lb); node.append(lt); node.append(rt); node.append(rb);
	AcDbObjectIdArray ids;
	ssFromNodes(ids, node, 2, m_Scale, 0, _T("红线"));
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		GetPlList(aname(ids.at(idx)), node);
		if(false == node.contains(pt)) continue;
		return node.find(pt);
	}
	return 0;
}

#pragma region 厦门竣工辅助类

XMJGAssist::XMJGAssist()
{
	m_Scale = 500;
	read1(m_Scale);
	m_Scale /= 1000;
}

XMJGAssist::~XMJGAssist()
{

}

void XMJGAssist::PurgeDatFile(const CString &szPathName, VPInfo &ptinfo, VStr &error, bool reverse)
{
	CStdioFile file; CString line; int type = 0;
	if(RTCAN == ads_getint(_T("[1]老数据、[2]全站仪、[3]RTK："), &type)) return;
	if(!file.Open(szPathName, CFile::modeRead)) return;
	while(file.ReadString(line))
	{
		VStr info;
		AnalyzeData(info, line, _T(","));
		if(type == 0)
		{
			if(info.size() != 5)
			{
				error.push_back(line);
				continue;
			}
		}
		else
		{
			if(info.size() != 4)
			{
				error.push_back(line);
				continue;
			}
			VSIter it = find(info.begin(), info.end(), info.at(0));
			info.insert(it, info[0]);
		}
		CPointInfo pt(info);
		if(true == reverse)
		{
			double temp = pt.m_pt3.x;
			pt.m_pt3.x = pt.m_pt3.y;
			pt.m_pt3.y = temp;
		}
		ptinfo.push_back(pt);
	}
}

void XMJGAssist::PurgeRTKFile(const CString &szPathName, VPInfo &ptinfo, VStr &error, bool reverse)
{
	CStdioFile file; CString line;
	if(!file.Open(szPathName, CFile::modeRead)) return;
	while(file.ReadString(line))
	{
		VStr info;
		AnalyzeData(info, line, _T(" "));
		if(info.size() != 4)
		{
			error.push_back(line);
			continue;
		}
		VSIter it = find(info.begin(), info.end(), info.at(0));
		info.insert(it, info[0]);
		CPointInfo pt(info);
		if(true == reverse)
		{
			float temp = (float)pt.m_pt3.x;
			pt.m_pt3.x = pt.m_pt3.y;
			pt.m_pt3.y = temp;
		}
		ptinfo.push_back(pt);
	}
}

void XMJGAssist::PurgeQZYFile(const CString &szPathName, VPInfo &ptinfo, VStr &error, bool reverse)
{
	CStdioFile file; CString line;
	if(!file.Open(szPathName, CFile::modeRead)) return;
	while(file.ReadString(line))
	{
		VStr info;
		AnalyzeData(info, line, _T(" "));
		if(info.size() != 4)
		{
			error.push_back(line);
			continue;
		}
		VSIter it = find(info.begin(), info.end(), info.at(0));
		info.insert(it, info[0]);
		CPointInfo pt(info);
		if(true == reverse)
		{
			double temp = pt.m_pt3.x;
			pt.m_pt3.x = pt.m_pt3.y;
			pt.m_pt3.y = temp;
		}
		ptinfo.push_back(pt);
	}
}

void XMJGAssist::PurgeExcelQZY(const CString &szPathName, VPInfo &ptinfo, VStr &error, bool reverse /* = false */)
{
	CTinyExcel excel;
	vector<_Range> vRange;
	excel.SetTemplete(szPathName);
	excel.StartExcel();
	int i = 0, j = 0;
	long nItemCount = 0;
	long nSubItemCount = 0;
	excel.GetWorksheet(excel.m_worksheets, excel.m_worksheet, _T("qzy"));
	excel.m_range.AttachDispatch(excel.m_worksheet.GetUsedRange());
	excel.GetRowColCount(nItemCount, nSubItemCount);
	acedSetStatusBarProgressMeter(_T("正在读取数据。。。"), 1, nItemCount);
	for(int row = 1; row <= nItemCount; ++row)
	{
		acedSetStatusBarProgressMeterPos(row);
		CString line, cell; VStr info;
		cell = excel.GetRangeContent(row, 1, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 2, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 3, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 4, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 5, excel.m_range); line.Append(cell);
		AnalyzeData(info, line, _T(","));
		if(info.size() != 5)
		{
			error.push_back(line);
			continue;
		}
		CPointInfo pt(info);
		if(reverse == true)
			SWAP(pt.m_pt3.x, pt.m_pt3.y);
		ptinfo.push_back(pt);
	}
	acedRestoreStatusBar();
}

void XMJGAssist::PurgeExcelRTK(const CString &szPathName, VPInfo &ptinfo, VStr &error, bool reverse /* = false */)
{
	CTinyExcel excel;
	vector<_Range> vRange;
	excel.SetTemplete(szPathName);
	excel.StartExcel();
	int i = 0, j = 0;
	long nItemCount = 0;
	long nSubItemCount = 0;
	excel.GetWorksheet(excel.m_worksheets, excel.m_worksheet, _T("rtk"));
	excel.m_range.AttachDispatch(excel.m_worksheet.GetUsedRange());
	excel.GetRowColCount(nItemCount, nSubItemCount);
	acedSetStatusBarProgressMeter(_T("正在读取数据。。。"), 1, nItemCount);
	for(int row = 1; row <= nItemCount; ++row)
	{
		acedSetStatusBarProgressMeterPos(row);
		CString line, cell; VStr info;
		cell = excel.GetRangeContent(row, 1, excel.m_range); line.Append(cell + _T(","));
		line.Append(cell + _T(","));	//点号和点名一样
		cell = excel.GetRangeContent(row, 4, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 5, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, 6, excel.m_range); line.Append(cell);
		AnalyzeData(info, line, _T(","));
		if(info.size() != 5)
		{
			error.push_back(line);
			continue;
		}
		CPointInfo pt(info);
		if(reverse == true)
			SWAP(pt.m_pt3.x, pt.m_pt3.y);
		ptinfo.push_back(pt);
	}
	acedRestoreStatusBar();
}

void XMJGAssist::PurgeExcelFile(const CString &szPathName, VPInfo &ptinfo, VStr &error, bool reverse)
{
	CTinyExcel excel;
	vector<_Range> vRange;
	excel.SetTemplete(szPathName);
	excel.StartExcel();
	int i = 0, j = 0, type = 0, ccode, cname, cx, cy, cz;
	long nItemCount = 0;
	long nSubItemCount = 0;
	if(RTCAN == ads_getint(_T("[0]老数据、[1]全站仪、[2]RTK："), &type)) return;
	if(type == 0) { ccode = 1; cname = 2; cx = 3; cy = 4; cz = 5; }
	else if(type == 1) { ccode = 1; cname = 2; cx = 3; cy = 4; cz = 5; }
	else if(type == 2) { ccode = 1; cname = 1; cx = 4; cy = 5; cz = 6; }
	excel.GetWorksheet(excel.m_worksheets, excel.m_worksheet, 1);
	excel.m_range.AttachDispatch(excel.m_worksheet.GetUsedRange());
	excel.GetRowColCount(nItemCount, nSubItemCount);
	if(nSubItemCount < cz)
	{
		acutPrintf(_T("数据文件格式错误，请检查数据格式"));
		return;
	}
	acedSetStatusBarProgressMeter(_T("正在读取数据。。。"), 1, nItemCount);
	for(int row = 1; row <= nItemCount; ++row)
	{
		acedSetStatusBarProgressMeterPos(row);
		CString line, cell; VStr info;
		cell = excel.GetRangeContent(row, ccode, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, cname, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, cx, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, cy, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, cz, excel.m_range); line.Append(cell);
		AnalyzeData(info, line, _T(","));
		if(info.size() != 5)
		{
			error.push_back(line);
			continue;
		}
		CPointInfo pt(info);
		if(reverse == true)
		{
			double temp = pt.m_pt3.x;
			pt.m_pt3.y = pt.m_pt3.x;
			pt.m_pt3.x = pt.m_pt3.y;
		}
		ptinfo.push_back(pt);
	}
	acedRestoreStatusBar();
}

void XMJGAssist::PipeExcel(const CString &szPathName, VPInfo &ptinfo, VStr &error, bool reverse)
{
	CTinyExcel excel;
	vector<_Range> vRange;
	excel.SetTemplete(szPathName);
	if(false == excel.StartExcel()) return;
	int i = 0, j = 0, ccode, cx, cy, cz, pipeheight, Dvalue;
	long nItemCount = 0;
	long nSubItemCount = 0;
	ccode = 1; cx = 2; cy = 3; cz = 4; pipeheight = 5; Dvalue = 6;
	excel.GetWorksheet(excel.m_worksheets, excel.m_worksheet, 1);
	excel.m_range.AttachDispatch(excel.m_worksheet.GetUsedRange());
	excel.GetRowColCount(nItemCount, nSubItemCount);
	if(nSubItemCount != Dvalue)
	{
		acutPrintf(_T("\n 数据文件格式错误，请检查数据格式！！！"));
		return;
	}
	acedSetStatusBarProgressMeter(_T("正在读取数据。。。"), 1, nItemCount);
	VStr dhs;
	for(int row = 1; row <= nItemCount; ++row)
	{
		acedSetStatusBarProgressMeterPos(row);
		CString line, cell; VStr info;
		cell = excel.GetRangeContent(row, ccode, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, cx, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, cy, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, cz, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, pipeheight, excel.m_range); line.Append(cell + _T(","));
		cell = excel.GetRangeContent(row, Dvalue, excel.m_range); line.Append(cell);
		AnalyzeData(info, line, _T(","));
		if(!info[4].Compare(_T("")) && !info[5].Compare(_T("")))
		{
			error.push_back(line);
			continue;
		}
		CPointInfo pt;
		pt.m_Name = info[0];
		pt.m_pt3.x = _tstof(info[1]);
		pt.m_pt3.y = _tstof(info[2]);
		pt.m_pt3.z = _tstof(info[3]);
		pt.m_pipeheight = _tstof(info[4]);
		pt.m_dvalue = _tstof(info[5]);
		if(find(dhs.begin(), dhs.end(), pt.m_Name) != dhs.end()) continue;
		dhs.push_back(pt.m_Name);
		if(!info[4].Compare(_T("")))
		{
			pt.m_pipeheight = _tstof(info[3]) - _tstof(info[5]);
		}
		if(!info[5].Compare(_T("")))
		{
			pt.m_pipeheight = _tstof(info[3]) - _tstof(info[4]);
		}
		if(reverse == true)
		{
			double temp = pt.m_pt3.y;
			pt.m_pt3.y = pt.m_pt3.x;
			pt.m_pt3.x = temp;
		}
		ptinfo.push_back(pt);
	}
	excel.Quit();
	acedRestoreStatusBar();
}

bool XMJGAssist::SelectFile(const CString &filter, const CString &dir, CString &szPathName, bool flag)
{
	CFileDialog dlg(flag, 0, 0, 6, filter);
	dlg.m_ofn.lpstrInitialDir = dir;
	if(dlg.DoModal() != IDOK) return false;
	szPathName = dlg.GetPathName();
	return true;
}

void XMJGAssist::DrawPoint(const CPointInfo &info)
{
	AcDbObjectId id = DrawInsert(apoint(info.m_pt3), _T("Point"));

	AddXdata(aname(id), _T("点号"), STRING_TYPE, info.m_Name);
	TCHAR szContent[50]; _stprintf(szContent, _T("%f"), info.m_pt3.x);
	AddXdata(aname(id), _T("X坐标"), DOUBLE_TYPE, szContent);
	_stprintf(szContent, _T("%f"), info.m_pt3.y);
	AddXdata(aname(id), _T("Y坐标"), DOUBLE_TYPE, szContent);
	_stprintf(szContent, _T("%f"), info.m_pt3.z);
	AddXdata(aname(id), _T("高程"), DOUBLE_TYPE, szContent);
	AcGePoint3d pt(info.m_pt3);
	pt.x += 0.25 * m_Scale;
	pt.y += 0.375 * m_Scale;
	AcDbObjectId codeid = DrawText(pt, info.m_Name, _T("ML"), _T(""), 1 * m_Scale);
	setlayer(aname(codeid), _T("点号"));
}

void XMJGAssist::DrawPoint(const VPInfo &ptinfo)
{
	int len = (int)ptinfo.size();
	acedSetStatusBarProgressMeter(_T("正在绘制点。。。"), 0, len);
	for(int idx = 0; idx < len; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		CPointInfo info = ptinfo.at(idx);
		DrawPoint(info);
	}
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	acedRestoreStatusBar();
}

AcDbObjectId XMJGAssist::DrawLine(const TCHAR *layer, const TCHAR *code, const int &color, TCHAR *Style, bool closeornot)
{
	ads_point spt, ept; AcGePoint2dArray node; int ret; AcDbObjectId id;
	if(RTNORM != ads_getpoint(NULL, _T("\n 请选择第一个点："), spt)) return id;
	if((ret = ads_getpoint(spt, _T("\n 请选择下一个点: "), ept)) != RTNORM) return id;
	node.append(apoint(spt));
	do
	{
		node.append(apoint(ept));
		ads_pointcopy(ept, spt);
		if(ret == RTCAN) break;
		else if(ret == RTNORM)
		{
			EntDel(aname(id));
			id = DrawPolyline2D(node, 1.0, false, code);
		}
		else if(ret == RTNONE)
		{
			int isClose = 0; bool flag = false;
			if(closeornot)
			{
				if(RTNORM != ads_getint(_T("是否闭合（<0> 闭合 <1> 不闭合）：<0>"), &isClose)) isClose = 0;
				flag = isClose == 1 ? false : true;
			}
			EntDel(aname(id));
			id = DrawPolyline2D(node, 1.0, flag, code);
			RemoveSameNodes(node); SetPolyCoord(aname(id), node);
			setcolor(aname(id), color);
			if(Style != NULL)
				setlinetype(aname(id), Style);
			node.removeAll(); setlayer(aname(id), layer);
			return id;
		}
		setcolor(aname(id), color);
		if(Style != NULL)
			setlinetype(aname(id), Style);
		ret = ads_getpoint(apoint(spt), _T("\n 请选择下一个点: "), ept);
	} while(RTCAN != ret);
	return id;
}

bool XMJGAssist::DrawLine(AcDbObjectId &id, const TCHAR *layer, const TCHAR *code, const int &color, TCHAR *Style, bool closeornot)
{
	ads_point spt, ept; AcGePoint2dArray node; int ret; bool bClose = false;
	if(RTNORM != ads_getpoint(NULL, _T("\n 请选择第一个点："), spt)) return false;
	if((ret = ads_getpoint(spt, _T("\n 请选择下一个点: "), ept)) != RTNORM) return false;
	node.append(apoint(spt));
	do
	{
		node.append(apoint(ept));
		ads_pointcopy(ept, spt);
		if(ret == RTCAN) break;
		else if(ret == RTNORM)
		{
			EntDel(aname(id));
			id = DrawPolyline2D(node, 1.0, false, code);
		}
		else if(ret == RTNONE)
		{
			int isClose = 0;
			if(closeornot)
			{
				if(RTNORM != ads_getint(_T("是否闭合（<0> 闭合 <1> 不闭合）：<0>"), &isClose)) isClose = 0;
				bClose = isClose == 1 ? false : true;
			}
			EntDel(aname(id));
			id = DrawPolyline2D(node, 1.0, bClose, code);
			RemoveSameNodes(node); SetPolyCoord(aname(id), node);
			setcolor(aname(id), color);
			if(Style != NULL)
				setlinetype(aname(id), Style);
			node.removeAll(); setlayer(aname(id), layer);
			return bClose;
		}
		setcolor(aname(id), color);
		ret = ads_getpoint(apoint(spt), _T("\n 请选择下一个点: "), ept);
	} while(RTCAN != ret);
	id = NULL;
	return bClose;
}

AcDbObjectId XMJGAssist::DrawLine(const TCHAR *layer, const TCHAR *code, const int &R, const int &G, const int &B, TCHAR *Style, bool closeornot)
{
	ads_point spt, ept; AcGePoint2dArray node; int ret; AcDbObjectId id;
	if(RTNORM != ads_getpoint(NULL, _T("\n 请选择第一个点："), spt)) return id;
	if((ret = ads_getpoint(spt, _T("\n 请选择下一个点: "), ept)) != RTNORM) return id;
	node.append(apoint(spt));
	do
	{
		node.append(apoint(ept));
		ads_pointcopy(ept, spt);
		if(ret == RTCAN) break;
		else if(ret == RTNORM)
		{
			EntDel(aname(id));
			id = DrawPolyline2D(node, 1.0, false, code);
		}
		else if(ret == RTNONE)
		{
			int isClose = 0; bool flag = false;
			if(closeornot)
			{
				if(RTNORM != ads_getint(_T("是否闭合（<0> 闭合 <1> 不闭合）：<0>"), &isClose)) isClose = 0;
				flag = isClose == 1 ? false : true;
			}
			EntDel(aname(id));
			id = DrawPolyline2D(node, 1.0, flag, code);
			setcolor(aname(id), R, G, B);
			if(Style != NULL)
				setlinetype(aname(id), Style);
			node.removeAll(); setlayer(aname(id), layer);
			return id;
		}
		setcolor(aname(id), R, G, B);
		ret = ads_getpoint(apoint(spt), _T("\n 请选择下一个点: "), ept);
	} while(RTCAN != ret);
	return id;
}

void XMJGAssist::PrintErrorInfo(const VStr &error)
{
	if(error.size() != 0)
		acutPrintf(_T("\n  错误信息"));
	for(int idx = 0; idx < (int)error.size(); ++idx)
		acutPrintf(_T("\n  %s"), error.at(idx));
}

void XMJGAssist::AutoCreateCode(const AcDbObjectId &id, bool flag, TCHAR *pre)
{
	if(pre == NULL)
	{
		pre = new TCHAR[20];
		ReadXdata(aname(id), _T("Pre"), 0, pre);
	}
	if(flag == true)
		BeginWestNorth(id);
	ClearPointCode(id);
	AddPointCode(id, pre);
	AddXdata(aname(id), _T("Pre"), 0, pre);
}

void XMJGAssist::AddPointCode(const AcDbObjectId &id, TCHAR *dh)
{
	TCHAR code[20];
	AcDbObjectIdArray idArr;
	AcGePoint2dArray node;
	GetPlList(aname(id), node);
	for(int idx = 0; idx < node.length(); ++idx)
	{
		AcGePoint3d mid = apoint(node.at(idx));
		mid[X] += 0.4 * m_Scale;
		_stprintf(code, _T("%s%d"), dh, idx + 1);
		AcDbObjectId textid = DrawText(mid, code, _T("MIDDLE"), _T("20000"), 2 * m_Scale);
		ADDMulXData(aname(id), _T("PtHandle"), GetHandleByObject(textid), 0, idx);
		setlayer(aname(textid), _T("注记"));
	}
}

void XMJGAssist::ClearPointCode(const AcDbObjectId &id)
{
	int len = GetXDataCount(aname(id), _T("PtHandle"));
	for(int idx = 0; idx < len; ++idx)
	{
		TCHAR *handle = new TCHAR[10];
		READMulXData(id, _T("PtHandle"), handle, 0, idx);
		AcDbObjectId zjid = GetObjectByHandle(handle);
		delete handle; handle = NULL;
		EntDel(aname(zjid));
	}
	RemoveXdata(aname(id), _T("PtHandle"));
}

void XMJGAssist::AdjustNodeOrder(const AcDbObjectId &id, int order, int index)
{
	AcGePoint2dArray node;
	GetPlList(aname(id), node);
	RemoveSameNodes(node);
	int len = node.length();
	AcGePoint2dArray tempNode;
	for(int idx = 0; idx < len; ++idx)
		tempNode.append(node.at((idx + index) % len));
	if(false == ((bool)order & IsDeasil(node)))
		tempNode.reverse();
	SetPolyCoord(aname(id), tempNode);
}

void XMJGAssist::findEntFromZJ(const AcDbObjectIdArray &ids, TCHAR *zj, AcDbObjectId &id)
{
	int len = ids.length();
	for(int idx = 0; idx < len; ++idx)
	{
		vector<AcDbObjectId> idArr;
		AcDbObjectId tmp = ids.at(idx);
		TCHAR text[50];
		GetEntText(aname(tmp), text);
		if(_tcsicmp(zj, text) != 0) continue;
		id = tmp;
		return;
	}
	id = 0;
}

void XMJGAssist::RemoveNodeFromID(AcGePoint2dArray &nodes, const AcDbObjectId &id)
{
	ads_point pt;
	GetEntPosition(aname(id), pt);
	if(nodes.contains(apoint(pt)))
		nodes.remove(apoint(pt));
}

void XMJGAssist::RemoveNodeFromID(AcGePoint2dArray &nodes, const AcDbObjectIdArray &delArr, bool flag /* = true */)
{
	for(int idx = 0; idx < delArr.length(); ++idx)
	{
		if(flag == true)
			RemoveNodeFromID(nodes, delArr.at(idx));
		else
		{
			vector<AcDbObjectId> ids;
			GetGroupEnts(delArr.at(idx), ids);
			for(int index = 0; index < (int)ids.size(); ++index)
				RemoveNodeFromID(nodes, ids.at(index));
		}
	}
}

void XMJGAssist::addAnnotation(const AcDbObjectId &id)
{
	CDrawArrow arrow;
	arrow.DrawArrow(id);
}

bool XMJGAssist::checkProjInfo(int pro_type)
{
	CXRecordOperate xrecordO;
	CString strText, strText2;

	CAcModuleResourceOverride myResources;
	CProjectInfoDlg projDlg;

	if(pro_type == 1)
	{
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("JSXM1"), strText);
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("SLH1"), strText2);

		if(strText != _T("") && strText2 != _T("")) return true;

		AfxMessageBox(_T("请设置 建设项目 跟 受理号！"));

		projDlg.project_type = "XMJG_RedLine";
		projDlg.DoModal();

	}
	else if(pro_type == 2)
	{
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("XMMC2"), strText);
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("SLH"), strText2);

		if(strText != _T("") && strText2 != _T("")) return true;

		AfxMessageBox(_T("请设置 项目名称 跟 受理号！"));

		projDlg.project_type = "XMJG_BuildSettingOut";
		projDlg.DoModal();

	}
	else if(pro_type == 3)
	{
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("XMMC"), strText);
		xrecordO.ReadStringRecord(DICT_PROJECTINFO, _T("SLH"), strText2);

		if(strText != _T("") && strText2 != _T("")) return true;
		AfxMessageBox(_T("请设置 项目名称 跟 受理号！"));
		projDlg.project_type = "XMJG_RoadAlignment";
		projDlg.DoModal();
	}
	return true;
}

bool XMJGAssist::GetExportEntitys(AcDbObjectIdArray &IdArr, const AcGePoint2dArray &nodes)
{
	ads_name ents;
	ads_command(RTSTR, _T("PURGE"), RTSTR, _T("B"), RTSTR, _T("*"), RTSTR, _T("N"), RTNONE);
	if(!ssFromNodes(nodes, ents, _T("CP"))) return false;
	SSToIdArr(ents, IdArr);
	return true;
}

double setDoubleMinLast(double val)
{
	int a = (int)(val);
	int b = a % 100;
	double c = (double)(a / 100) * 100.0;
	if(b > 50) c += 50;
	else c = c;
	return c;
}

double setDoubleMaxLast(double val)
{
	int a = (int)(val);
	int b = a % 100;
	double c = (double)(a / 100) * 100.0;
	if(b > 50) c += 100;
	else c = c + 50;
	return c;
}

void XMJGAssist::GetExportRange(const AcGePoint2d &lb, const AcGePoint2d &rt, AcGePoint2dArray &nodes)
{
	SNodesOpe nodesO;
	nodesO.LB_RT_Nodes(lb, rt, nodes);
}

bool XMJGAssist::GetExportRange(AcGePoint2d &lb, AcGePoint2d &rt)
{
	ads_point pt1, pt2;	SNodesOpe nodesO;
	if(RTCAN == ads_getpoint(NULL, _T("\n 请指定范围线左下角"), pt1)) return false;
	if(RTCAN == ads_getcorner(pt1, _T("\n 请指定范围线右上角"), pt2)) return false;
	nodesO.modifyLB_RTPoint(pt1, pt2);
	lb.x = pt1[X]; lb.y = pt1[Y];
	rt.x = pt2[X]; rt.y = pt2[Y];
	return true;
}

double XMJGAssist::GetScale(const AcGePoint2d &tklb, const AcGePoint2d &tkrt, const AcGePoint2d &entlb, const AcGePoint2d &entrt)
{
	double TKH = tkrt.y - tklb.y;
	double TKW = tkrt.x - tklb.x;
	double EntH = entrt.y - entlb.y;
	double EntW = entrt.x - entlb.x;
	double tkscale = TKH / EntH;
	double entscale = TKW / EntW;
	double dscale = tkscale < entscale ? tkscale : entscale;
	return dscale;
}

bool XMJGAssist::SaveEntitysToDWG(const CString &path)
{
	CSaveBlocks blocks;	CString pathName(path);
	if(pathName.Right(4) != ".dwg")
		pathName.Append(_T(".dwg"));
	blocks.setPath(path); ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	blocks.saveBlocks2(idArr, false);
	return true;
}

bool XMJGAssist::SaveEntitysToDWG(const AcGePoint3d &mid, const double &scale)
{
	CSaveBlocks blocks;	CString path;
	CString filter(_T("*.dwg | *.dwg"));
	if(!SelectFile(filter, _T(""), path, false)) return false;
	blocks.setPath(path); ads_name ssSave;
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	ScaleEntitys(idArr, mid, 1 / scale);
	blocks.saveBlocks2(idArr, false);
	return true;
}

AcDbObjectId XMJGAssist::CreateScopeLine()
{
	ads_point pt1, pt2, pt3, pt4;//左下，右上，左上，右下
	if(RTCAN == ads_getpoint(NULL, _T("\n 请指定范围线左下角"), pt1)) return false;
	if(RTCAN == ads_getcorner(pt1, _T("\n 请指定范围线右上角"), pt2)) return false;

	SNodesOpe nodesO;
	nodesO.modifyLB_RTPoint(pt1, pt2);

	pt3[X] = pt1[X]; pt3[Y] = pt2[Y]; pt3[Z] = 0.0;
	pt4[X] = pt2[X]; pt4[Y] = pt1[Y]; pt4[Z] = 0.0;

	AcGePoint2dArray nodes;
	nodesO.LB_RT_Nodes(apoint(pt1), apoint(pt2), nodes);

	AcDbObjectId id = DrawPolyline2D(nodes, 1.0, true, _T("2000000"));
	setcolor(aname(id), 255, 0, 0);
	setlayer(aname(id), _T("ScopeLine"));

	return id;
}

AcDbObjectId XMJGAssist::drawControlPoint(CString _type, CString _rank, CString _name, CString height, AcGePoint2d _textPt)
{

	double dheight = _ttol(height);
	ads_point insert_pt;
	insert_pt[0] = _textPt.x;
	insert_pt[1] = _textPt.y;
	insert_pt[2] = dheight;
	AcDbObjectId id = DrawInsert(insert_pt, _T("ControlPoint"));

	AddXdata(aname(id), _T("点名"), 0, _name);
	AddXdata(aname(id), _T("高程"), 0, height);
	AddXdata(aname(id), _T("等级"), 0, _rank);
	AddXdata(aname(id), _T("类型"), 0, _type);
	setlayer(aname(id), _T("控制点"));
	return id;
}

AcDbObjectId XMJGAssist::DrawFinalScopeLine()
{
	ads_point pt1, pt2, pt3, pt4;//左下，右上，左上，右下
	if(RTCAN == ads_getpoint(NULL, _T("\n 请指定左下角"), pt1)) return AcDbObjectId();
	if(RTCAN == ads_getcorner(pt1, _T("\n 请指定右上角"), pt2)) return AcDbObjectId();

	SNodesOpe nodesO;
	nodesO.modifyLB_RTPoint(pt1, pt2);

	pt3[X] = pt1[X]; pt3[Y] = pt2[Y]; pt3[Z] = 0.0;
	pt4[X] = pt2[X]; pt4[Y] = pt1[Y]; pt4[Z] = 0.0;

	AcGePoint2d m_pt2dLBTK = apoint(pt1);
	AcGePoint2d m_pt2dRTTK = apoint(pt2);
	CTukuang_Common tukuanC;
	tukuanC.m_dhh = m_Scale;
	tukuanC.coordFit(m_pt2dLBTK, 2);
	tukuanC.coordFit(m_pt2dRTTK, 2);
	pt1[X] = m_pt2dLBTK.x; pt1[Y] = m_pt2dLBTK.y;
	pt2[X] = m_pt2dRTTK.x; pt2[Y] = m_pt2dRTTK.y;

	AcGePoint2dArray nodes;
	nodesO.LB_RT_Nodes(apoint(pt1), apoint(pt2), nodes);
	AcDbObjectIdArray objArr;
	ssFromNodes(objArr, nodes, 2, 0.0, _T("*LINE*"), _T("规划控制线"));

	return objArr[0];
}

void XMJGAssist::SetAzimuthInfo(int type, TCHAR *azimuth)
{
	switch(type)
	{
		case 1: _stprintf(azimuth, _T("%s"), _T("东")); break;
		case 2: _stprintf(azimuth, _T("%s"), _T("南")); break;
		case 3: _stprintf(azimuth, _T("%s"), _T("西")); break;
		case 4: _stprintf(azimuth, _T("%s"), _T("北")); break;
		case 5: _stprintf(azimuth, _T("%s"), _T("东南")); break;
		case 6: _stprintf(azimuth, _T("%s"), _T("东北")); break;
		case 7: _stprintf(azimuth, _T("%s"), _T("西南")); break;
		case 8: _stprintf(azimuth, _T("%s"), _T("西北")); break;
		default:		break;
	}
}

void XMJGAssist::SetAnnotationType()
{
	HKEY rKey; TCHAR type[200] = {0}; int index = 0;
	int es = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), 0, KEY_ALL_ACCESS, &rKey);
	if(es == ERROR_SUCCESS)
		CassQueryReg(rKey, _T("AnnotationType"), type);
	else
		RegCreateKey(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), &rKey);
	if(_tcslen(type) == 0) index = 0;
	else if(_tcscmp(type, _T("退红线距离标注")) == 0) index = 0;
	else if(_tcscmp(type, _T("建筑间距标注")) == 0) index = 1;
	TCHAR *info = new TCHAR[200];
	_stprintf(info, _T("请输入标注类型(<0>退红线距离标注 <1>建筑间距标注)：<%d>"), index);
	if(RTCAN == ads_getint(info, &index)) return;
	if(index == 0) _stprintf(type, _T("%s"), _T("退红线距离标注"));
	else if(index == 1)  _stprintf(type, _T("%s"), _T("建筑间距标注"));
	else  _stprintf(type, _T("%s"), _T("退红线距离标注"));
	CassSetReg(rKey, _T("AnnotationType"), type);
	RegCloseKey(rKey);
	delete info; info = NULL;
}

void XMJGAssist::GetAnnotationType(TCHAR *type)
{
	HKEY rKey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), 0, KEY_READ, &rKey) != ERROR_SUCCESS)
		SetAnnotationType();
	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), 0, KEY_READ, &rKey) == ERROR_SUCCESS)
		CassQueryReg(rKey, _T("AnnotationType"), type);
}

void XMJGAssist::AddAnnotationInfo(const AcDbObjectId &id, int color, TCHAR *info)
{
	setlayer(aname(id), _T("标注"));
	setcolor(aname(id), color);
	AddXdata(aname(id), _T("标注类型"), 0, info);
}

void XMJGAssist::AddAnnotationInfo(const AcDbObjectId &id, TCHAR *layer, MStr &xdata)
{
	setlayer(aname(id), layer);
	MSIter it = xdata.begin();
	for(it = xdata.begin(); it != xdata.end(); ++it)
	{
		AddXdata(aname(id), it->first, 0, it->second);
	}
}

void XMJGAssist::AddAnnotationInfo(const AcDbObjectIdArray &ids, TCHAR *layer, MStr &xdata)
{
	int len = ids.length();
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		setlayer(aname(id), layer);
		MSIter it = xdata.begin();
		for(it = xdata.begin(); it != xdata.end(); ++it)
		{
			AddXdata(aname(id), it->first, 0, it->second);
		}
	}
}

void XMJGAssist::GetRedLineAndBuildAnnotation(AcDbObjectIdArray &ids)
{
	IDataBaseOper oper; AcDbObjectId id;
	TextProperty prop1 = oper.readTextPropertyTable(_T("竣工距离标注引线"));
	SelectFilter f1(8, prop1._layer);
	AcDbObjectIdArray temps; MSId annid;
	if(!SelectEntitys(temps, f1, _T("X"))) return;
	int len = temps.length(); TCHAR type[200] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = temps.at(idx);
		ReadXdata(aname(id), _T("标注类型"), 0, type);
		if(_tcscmp(type, _T("退红线距离标注")) != 0) continue;
		if(!ReadXdata(aname(id), _T("编号"), 0, type)) continue;
		if(_ttoi(type) < 0) continue;
		annid[type] = id;
	}
	for(MSIdIter it = annid.begin(); it != annid.end(); ++it)
		ids.append(it->second);
}

void XMJGAssist::GetBuildAnnotation(AcDbObjectIdArray &ids)
{
	IDataBaseOper oper; AcDbObjectId id;
	TextProperty prop1 = oper.readTextPropertyTable(_T("竣工距离标注引线"));
	SelectFilter f1(8, prop1._layer);
	AcDbObjectIdArray temps; MSId annid;
	if(!SelectEntitys(temps, f1, _T("X"))) return;
	int len = temps.length(); TCHAR type[200] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = temps.at(idx);
		ReadXdata(aname(id), _T("标注类型"), 0, type);
		if(_tcscmp(type, _T("建筑间距标注")) != 0) continue;
		if(!ReadXdata(aname(id), _T("编号"), 0, type)) continue;
		if(_ttoi(type) < 0) continue;
		annid[type] = id;
	}
	for(MSIdIter it = annid.begin(); it != annid.end(); ++it)
		ids.append(it->second);
}

void XMJGAssist::GetEdgeDistanceInfo(CString &text)
{
	AcDbObjectIdArray ids;
	GetRedLineAndBuildAnnotation(ids);
	TCHAR info[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		ReadXdata(aname(ids[idx]), _T("间距"), 0, info);
		text.Append(info); text.Append(_T("\r\n"));
	}
}

void XMJGAssist::GetBuildDistanceInfo(CString &text)
{
	AcDbObjectIdArray ids;
	GetBuildAnnotation(ids);
	TCHAR info[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		ReadXdata(aname(ids[idx]), _T("间距"), 0, info);
		text.Append(info); text.Append(_T("\r\n"));
	}
}

void XMJGAssist::GetEdgeDistanceText(CString &text)
{
	AcDbObjectIdArray ids; vector<AzimuthDist> azimuthdist;
	GetRedLineAndBuildAnnotation(ids);
	GetAzimuthDist(ids, azimuthdist);
	int len = (int)azimuthdist.size();
	for(int idx = 0; idx < len; ++idx)
	{
		CString info(_T("#1退用地红线#2米；\n"));
		AzimuthDist tmp = azimuthdist.at(idx);
		info.Replace(_T("#1"), tmp.Azimuth);
		info.Replace(_T("#2"), tmp.dist);
		text.Append(info);
	}
}

void XMJGAssist::GetBuildDistanceText(CString &text)
{
	AcDbObjectIdArray ids; vector<AzimuthDist> azimuthdist;
	GetBuildAnnotation(ids);
	GetAzimuthDist(ids, azimuthdist);
	int len = (int)azimuthdist.size();
	for(int idx = 0; idx < len; ++idx)
	{
		CString info(_T("#1退用地建筑#2米；\n"));
		AzimuthDist tmp = azimuthdist.at(idx);
		info.Replace(_T("#1"), tmp.Azimuth);
		info.Replace(_T("#2"), tmp.dist);
		text.Append(info);
	}
}

void XMJGAssist::GetAzimuthDist(AcDbObjectIdArray &ids, vector<AzimuthDist> &index)
{
	int len = ids.length(); map<CString, AzimuthDist> indx;
	TCHAR code[200] = {0};
	for(int idx = 0; idx < len; ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		if(!ReadXdata(aname(id), _T("编号"), 0, code)) continue;
		if(_tcslen(code) == 0) continue;
		CString first(code); AzimuthDist azimuth;
		ReadXdata(aname(id), _T("方位"), 0, code); azimuth.Azimuth.Append(code);
		ReadXdata(aname(id), _T("距离"), 0, code); azimuth.dist.Append(code);
		indx[first] = azimuth;
	}
	map<CString, AzimuthDist>::iterator it = indx.begin();
	for(; it != indx.end(); ++it)
	{
		index.push_back(it->second);
	}
}

AcDbObjectId XMJGAssist::GetPointEntByPoint(const AcGePoint2d &pt)
{
	AcGePoint2dArray nodes; AcDbObjectIdArray idArr;
	nodes.append(AcGePoint2d(pt.x - 0.5, pt.y - 0.5));
	nodes.append(AcGePoint2d(pt.x - 0.5, pt.y + 0.5));
	nodes.append(AcGePoint2d(pt.x + 0.5, pt.y + 0.5));
	nodes.append(AcGePoint2d(pt.x + 0.5, pt.y - 0.5));
	ssFromNodes(idArr, nodes, 1, 0.5);
	for(int idx = 0; idx < idArr.length(); ++idx)
	{
		ads_point mid;
		AcDbObjectId id = idArr.at(idx);
		GetEntPosition(aname(id), mid);
		AcGePoint2d pt_mid(mid[X], mid[Y]);
		if(pt == pt_mid) return id;
	}
	return NULL;
}

AcDbObjectIdArray XMJGAssist::BoundAryAnnotation(const AcGePoint2dArray &)
{
	AcDbObjectIdArray IdArr;
	return IdArr;
}

AcDbObjectId XMJGAssist::BoundAryAnnotation(const AcGePoint2d &, const AcGePoint2d &)
{
	AcDbObjectId id;

	return id;
}

void XMJGAssist::DrawMesh(CString title)
{
	AcDbObjectIdArray IdArr;
	if(!SelectEntitys(IdArr, _T("X"))) return;
	ads_point pt1, pt2;
	GetEntsExtent(IdArr, pt1, pt2);
	//DrawMesh(apoint(pt1), apoint(pt2), title);
}

void XMJGAssist::DrawMesh(const AcDbObjectIdArray &IdArr, CString title)
{
	ads_point pt1, pt2;
	GetEntsExtent(IdArr, pt1, pt2);
	//DrawMesh(apoint(pt1), apoint(pt2), title);
}

void XMJGAssist::DrawMesh(const AcGePoint2d &pt1, const AcGePoint2d &pt2, ExportDWGType type)
{
	AcGePoint2d min, max;
	GetExternPoint(pt1, pt2, min, max);
	DrawCrosshair(min, max);
	DrawTuKuang(min, max);
	AddTuKuangTitle(min, max, type);
	if(type == ZongPingTu) InsertTuQian(min, max);
	AddCornerCoor(min, max);
}

AcDbObjectIdArray XMJGAssist::DrawMesh(AcGePoint2d &pt1, AcGePoint2d &pt2, const CString &strGXMC, int nTable)
{
	AcDbObjectIdArray ids;
	SelectEntitys(ids, _T("X"));
	AcGePoint2d min, max;
	GetExternPoint(pt1, pt2, min, max);
	DrawCrosshair(min, max);
	DrawTuKuang(min, max);
	AddTuKuangTitle(min, max, strGXMC, nTable);
	AddCornerCoor(min, max);
	pt1 = min; pt2 = max;
	return ids;
}

void XMJGAssist::GetExternPoint(const AcGePoint2dArray &nodes, AcGePoint2d &min, AcGePoint2d &max)
{
	int length = nodes.length();
	min = nodes.first(); max = nodes.first();
	for(int idx = 1; idx < length; ++idx)
	{
		AcGePoint2d temp = nodes.at(idx);
		min.x = min.x < temp.x ? min.x : temp.x;
		min.y = min.y < temp.y ? min.y : temp.y;
		max.x = max.x > temp.x ? max.x : temp.x;
		max.y = max.y > temp.y ? max.y : temp.y;
	}
	int XCount = (int)((max.x - min.x) / 50 + 1);
	int YCount = (int)((max.y - min.y) / 50 + 1);
	min.x = ((int)(min.x / 10)) * 10;
	min.y = ((int)(min.y / 10)) * 10;
	max.x = min.x + XCount * 50;
	max.y = min.y + YCount * 50;
}

void XMJGAssist::GetExternPoint(const AcGePoint2d &pt1, const AcGePoint2d &pt2, AcGePoint2d &min, AcGePoint2d &max)
{
	min.x = setDoubleMinLast(pt1.x);
	min.y = setDoubleMinLast(pt1.y);
	max.x = setDoubleMaxLast(pt2.x);
	max.y = setDoubleMaxLast(pt2.y);
}

void XMJGAssist::DrawCrosshair(const AcGePoint2d &pos, const DrawDir &dir)
{
	double scale = 500; read1(scale); scale /= 200; AcDbObjectId id;
	AcGePoint2d top(pos), left(pos), right(pos), buttom(pos);
	top.y += scale; left.x -= scale; right.x += scale; buttom.y -= scale;
	if(dir == TopDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(top), _T(""));
		setlayer(aname(id), _T("图框"));
	}
	if(dir == LeftDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(left), _T(""));
		setlayer(aname(id), _T("图框"));
	}
	if(dir == RightDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(right), _T(""));
		setlayer(aname(id), _T("图框"));
	}
	if(dir == ButtomDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(buttom), _T(""));
		setlayer(aname(id), _T("图框"));
	}
}

void XMJGAssist::DrawCrosshair(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	double scale = 500; scale /= 10;
	for(double row = lb.y; row <= rt.y; row += scale)
	{
		for(double col = lb.x; col <= rt.x; col += scale)
		{
			if(abs(col - lb.x) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), RightDir);
			else if(abs(row - lb.y) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), TopDir);
			else if(abs(col - rt.x) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), LeftDir);
			else if(abs(row - rt.y) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), ButtomDir);
			else
				DrawCrosshair(AcGePoint2d(col, row), AllDir);
		}
	}
}

void XMJGAssist::DrawTuKuang(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	AcGePoint2d lt, rb; AcDbObjectId id;
	lt.x = lb.x; lt.y = rt.y;
	rb.x = rt.x; rb.y = lb.y;
	AcGePoint2dArray nodes1, nodes2;
	nodes1.append(lb); nodes1.append(lt);
	nodes1.append(rt); nodes1.append(rb);
	id = DrawPolyline2D(nodes1, 0, true, _T("")); setlayer(aname(id), _T("图框"));
	AcGePoint2d lb1(lb.x, lb.y - m_Scale * 12), lb2(lb.x - m_Scale * 12, lb.y);
	AcGePoint2d lt1(lt.x, lt.y + m_Scale * 12), lt2(lt.x - m_Scale * 12, lt.y);
	AcGePoint2d rt1(rt.x, rt.y + m_Scale * 12), rt2(rt.x + m_Scale * 12, rt.y);
	AcGePoint2d rb1(rb.x, rb.y - m_Scale * 12), rb2(rb.x + m_Scale * 12, rb.y);
	id = drawline(apoint(lb1), apoint(lt1), _T("")); setlayer(aname(id), _T("图框"));
	id = drawline(apoint(lt2), apoint(rt2), _T("")); setlayer(aname(id), _T("图框"));
	id = drawline(apoint(rt1), apoint(rb1), _T("")); setlayer(aname(id), _T("图框"));
	id = drawline(apoint(rb2), apoint(lb2), _T("")); setlayer(aname(id), _T("图框"));
	Poly2dArrayOffset(nodes1, m_Scale * 12, false, nodes2);
	nodes2.first().y -= (m_Scale * 12); nodes2.last().y -= (m_Scale * 12);
	id = DrawPolyline2D(nodes2, 0, true, _T(""), m_Scale * 0.6);
	setlayer(aname(id), _T("图框")); AcGePoint3d pos;
	pos.x = lb1.x + 6 * m_Scale; pos.y = lb1.y + 2 * m_Scale;
	TCHAR info[20] = {0};
	_stprintf(info, _T("%s"), _T("（图廓坐标均以 “Km”为单位）"));
	id = DrawText(pos, info, _T("BL"), _T(""), 2.5 * m_Scale, 0.0, 1.0);
	pos[Y] -= 3.5;
	DrawText(pos, _T("注：此图为样图"), _T("BL"), _T(""), 2.5 * m_Scale, 0.0, 1.0);
	SetTextStyle(id, _T("黑体"), 1.0); setlayer(aname(id), _T("图框"));
}

void XMJGAssist::AddCornerCoor(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	AcGePoint2dArray nodes;
	nodes.append(lb); nodes.append(AcGePoint2d(lb.x, rt.y));
	nodes.append(rt); nodes.append(AcGePoint2d(rt.x, lb.y));
	int length = nodes.length();
	TCHAR info[100] = {0}; AcDbObjectId id;
	AcGePoint3d pos(0, 0, 0);
	AcGeVector3d offset(0, 0, 0);
	TCHAR Align1[20] = {0}, Align2[20] = {0};
	// 1 - 2
	// |   |  
	// 0   3
	for(int idx = 0; idx < length; ++idx)
	{
		if(idx == 0)
		{
			offset.y = -m_Scale * 12;
			_stprintf(Align1, _T("%s"), _T("BC"));
			_stprintf(Align2, _T("%s"), _T("BR"));
		}
		else if(idx == 1)
		{
			offset.y = m_Scale * 11.5;
			_stprintf(Align1, _T("%s"), _T("TC"));
			_stprintf(Align2, _T("%s"), _T("TR"));
		}
		else if(idx == 2)
		{
			offset.y = m_Scale * 11.5;
			_stprintf(Align1, _T("%s"), _T("TC"));
			_stprintf(Align2, _T("%s"), _T("TL"));
		}
		else
		{
			offset.y = -m_Scale * 12;
			_stprintf(Align1, _T("%s"), _T("BC"));
			_stprintf(Align2, _T("%s"), _T("BL"));
		}
		AcGePoint2d tmp = nodes.at(idx);
		pos.x = tmp.x; pos.y = tmp.y;
		//x
		_stprintf(info, _T("%.2f"), tmp.x / 1000);
		id = DrawText(pos + offset, info, Align1, _T(""), 3 * m_Scale);
		SetTextStyle(id, _T("黑体"), 1.0);
		setlayer(aname(id), _T("图框"));
		//y
		_stprintf(info, _T("%.2f"), tmp.y / 1000);
		id = DrawText(pos, info, Align2, _T(""), 3 * m_Scale);
		setlayer(aname(id), _T("图框"));
		SetTextStyle(id, _T("黑体"), 1.0);
	}
}

void XMJGAssist::AddTuKuangTitle(const AcGePoint2d &lb, const AcGePoint2d &rt, ExportDWGType type)
{
	AcGePoint3d lt(lb.x, rt.y, 0), mid; TCHAR info[255] = {0};
	mid.x = (lb.x + rt.x) / 2; mid.y = rt.y + 17 * m_Scale;
	AcGePoint3d rb(rt.x, lb.y, 0); TextProperty prop;
	IDataBaseOper oper; IProjectMDB pdb; MStr title = pdb.getJBXXTableInfo();
	switch(type)
	{
		case ZongPingTu:
			_stprintf(info, _T("%s%s"), title[_T("项目名称")], oper.readCommonTable(_T("建筑竣工总平图标题")));
			prop = oper.readTextPropertyTable(_T("建筑竣工总平图标题"));
			break;
		case ZhaoPianShiYiTu:
			_stprintf(info, _T("%s%s"), title[_T("项目名称")], oper.readCommonTable(_T("建筑竣工照片示意图标题")));
			prop = oper.readTextPropertyTable(_T("建筑竣工照片示意图标题"));
			break;
		case DLZhaoPianShiYiTu:
			_stprintf(info, _T("%s%s"), title[_T("项目名称")], oper.readCommonTable(_T("道路竣工照片示意图标题")));
			prop = oper.readTextPropertyTable(_T("道路竣工照片示意图标题"));
			break;
		case GHSTYSGSTBYZ:
			_stprintf(info, _T("%s%s"), title[_T("项目名称")], oper.readCommonTable(_T("规划审图与施工审图不一致标题")));
			prop = oper.readTextPropertyTable(_T("规划审图与施工审图不一致标题")); break;
		case DiXingTu:
			mid.y += 14 * m_Scale; prop = oper.readTextPropertyTable(_T("地形图标题"));
			break;
		default:
			break;
	}
	AcDbObjectId id = DrawText(mid, info, _T("MC"), _T(""), prop._size * m_Scale);
	setlayer(aname(id), prop._layer); SetTextStyle(id, prop._font, 1.0);
	setTextWidthFactor(id, 1.0); setcolor(aname(id), prop._color);
	if(DiXingTu != type) return;
	DrawInsert(apoint(lb), _T("地形图左下角"));
	DrawInsert(apoint(lt), _T("地形图左上角"));
	DrawInsert(apoint(rt), _T("地形图右上角"));
	AcDbObjectId yxj = DrawInsert(apoint(rb), _T("地形图右下角"));
	SBreakBlock sb; sb.SetBlock(yxj); sb.Explode();
	double x = ((int)(lb.x / 10)) * 0.01;
	double y = ((int)(lb.y / 10)) * 0.01;
	_stprintf(info, _T("%.2f -- %.2f"), x, y);
	mid.y -= 9 * m_Scale;
	id = DrawText(mid, info, _T("MC"), _T(""), prop._size * m_Scale);
	setlayer(aname(id), prop._layer); SetTextStyle(id, prop._font, 1.0);
	setTextWidthFactor(id, 1.0); setcolor(aname(id), prop._color);
}

void XMJGAssist::AddTuKuangTitle(const AcGePoint2d &lb, const AcGePoint2d &rt, const CString &strGXMC, int nTable)
{
	AcGePoint3d lt(lb.x, rt.y, 0), mid; TCHAR info[255] = {0};
	mid.x = (lb.x + rt.x) / 2; mid.y = rt.y + 17 * m_Scale;
	AcGePoint3d rb(rt.x, lb.y, 0);
	IDataBaseOper oper; IProjectMDB pdb; MStr title = pdb.getJBXXTableInfo();
	CString strTitleType = _T("");
	switch(nTable)
	{
		case 0:strTitleType = _T("管线总平图标题"); break;
		case 1:strTitleType = _T("管线对照图标题"); break;
		default:break;
	}
	_stprintf(info, _T("%s%s%s"), title[_T("项目名称")], strGXMC, oper.readCommonTable(strTitleType));
	AcDbObjectId id = DrawText(mid, info, _T("MC"), _T(""), 5 * m_Scale);
	setlayer(aname(id), _T("图框")); SetTextStyle(id, _T("黑体"));
	setTextWidthFactor(id, 1.0);
}

bool XMJGAssist::InsertTuQian(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	IProjectMDB pdb; VMStr areas = pdb.getCXXTableInfo();
	double dsarea = 0.0, dxarea = 0.0; TCHAR pszArea[255] = {0};
	for(int idx = 0; idx < (int)areas.size(); ++idx)
	{
		MStr area = areas[idx];
		if(_ttoi(area[_T("层号")]) < 0) dxarea += _tstof(area[_T("建筑面积")]);
		else  dsarea += _tstof(area[_T("建筑面积")]);
	}

	int Position = 0; AcGePoint2d pos;
	if(RTCAN == ads_getint(_T("\n 请输入插入图签的位置[0]左上[1]左下[2]右上[3]右下：<0>"), &Position)) return false;
	switch(Position)
	{
		case 0: pos.x = lb.x; pos.y = rt.y - 125 * m_Scale; break;
		case 1: pos.x = lb.x; pos.y = lb.y; break;
		case 2: pos.x = rt.x - 126 * m_Scale; pos.y = rt.y - 125 * m_Scale; break;
		case 3: pos.x = rt.x - 126 * m_Scale; pos.y = lb.y; break;
	}
	IProjectMDB oper; MStr record = oper.getJBXXTableInfo();
	AcDbObjectId id = DrawInsert(apoint(pos), _T("XMJGTuQian"), m_Scale, m_Scale);
	SBreakBlock m_Block; m_Block.SetBlock(id); m_Block.Explode();
	_stprintf(pszArea, _T("%.2lf"), dsarea + dxarea);
	m_Block.replaceText(_T("#ZJZMJ#"), pszArea);
	_stprintf(pszArea, _T("%.2lf"), dsarea);
	m_Block.replaceText(_T("#DSMJ#"), pszArea);
	_stprintf(pszArea, _T("%.2lf"), dxarea);
	m_Block.replaceText(_T("#DXMJ#"), pszArea);
	m_Block.replaceText(_T("#ProjectName#"), record[_T("项目名称")]);
	m_Block.replaceText(_T("#BuildCompany#"), record[_T("建设单位")]);
	m_Block.replaceText(_T("#MeansureCompany#"), record[_T("测量单位")]);
	setlayer(aname(id), _T("图框"));
	return true;
}

void XMJGAssist::FilterEntitys(AcDbObjectIdArray &IdArr, const MStr &filter)
{
	MSCIter it; int length = IdArr.length();
	IdArr.setLogicalLength(0); TCHAR info[200] = {0};
	for(int idx = 0; idx < length; ++idx)
	{
		CString key, value;
		AcDbObjectId id = IdArr.at(idx);
		for(it = filter.begin(); it != filter.end(); ++it)
		{
			key = it->first; value = it->second;
			if(key.CompareNoCase(_T("图层")) == 0)
			{
				GetEntLayer(id, info);
				if(value.CompareNoCase(info) == 0)
					EntDel(aname(id));
			}
		}
	}
}

void XMJGAssist::FilterEntitys(AcDbObjectIdArray &IdArr, const MVStr &filter)
{
	int length = IdArr.length(); TCHAR info[200] = {0};
	acedSetStatusBarProgressMeter(_T("正在过滤实体..."), 0, length);
	for(int idx = 0; idx < length; ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = IdArr.at(idx);
		MVSCIter it; VStr Vals; CString key;
		for(it = filter.begin(); it != filter.end(); ++it)
		{
			key = it->first; Vals = it->second;
			if(key.CompareNoCase(_T("图层")) == 0)
			{
				GetEntLayer(id, info);
				if(Vals.end() == find(Vals.begin(), Vals.end(), info)) continue;
				EntDel(aname(id)); break;
			}
		}
	}
	acedRestoreStatusBar();
}

void filterEntityByCode(AcDbObjectIdArray &IdArr, const VStr &Vals)
{
	TCHAR info[255] = {0};
	for(int idx = 0; idx < IdArr.length(); ++idx)
	{
		AcDbObjectId id = IdArr.at(idx);
		int count = (int)Vals.size(); CString val;
		GetEntLayer(id, info);
		for(int indx = 0; indx < count; ++indx)
		{
			val = Vals.at(indx);
			if(val.CompareNoCase(info) != 0) continue;
			EntDel(aname(id));
			break;
		}
	}
}

void XMJGAssist::FilterEntitys(AcDbObjectIdArray &IdArr, const MIVStr &filter)
{
	for(MIVSCIter it = filter.begin(); it != filter.end(); ++it)
	{
		switch(it->first)
		{
			case FilterCode: filterEntityByCode(IdArr, it->second);   break;
				//case FilterLayer:filterEntityByCode(IdArr, it->second);   break;
				//case FilterColor:filterEntityByCode(IdArr, it->second);   break;
			default:   break;
		}
	}
}

bool XMJGAssist::QueryRegisterValue(HKEY key, const TCHAR *path, const TCHAR *keySub, TCHAR *value)
{
	HKEY rKey;
	if(RegOpenKeyEx(key, path, 0, KEY_ALL_ACCESS, &rKey) != ERROR_SUCCESS) return false;
	CassQueryReg(rKey, keySub, value);
	return true;
}

void XMJGAssist::DWGConvertToPicture()
{
	if(!GetPlotDevice(m_raster_format, m_str_divice, m_str_pmp_file_name))
		return;
	CopyPrinterConfigFile();
	// 必须设置后台打印否则速度奇慢
	struct resbuf rb;
	rb.restype = 5003;
	rb.resval.rint = 0;
	m_nType = 3;
	ads_setvar(_T("BACKGROUNDPLOT"), &rb);
	es_count = 0;
	AcDbPlotSettingsValidator* pPSV = NULL;
	AcDbPlotSettings* pPlotSettings = NULL;
	pPSV = (AcDbPlotSettingsValidator*)acdbHostApplicationServices()->plotSettingsValidator();
	pPlotSettings = new AcDbPlotSettings(true);
	CString real_size_name;
	if(!GetZdtPara()) return;
	if(!GetPlotSizeName(pPSV, pPlotSettings, real_size_name))
	{
		CString str;
		str.Format(_T("未找到名为\"%s\"的打印尺寸，请在打印设置中设置"), m_str_size_name);
		if(pPlotSettings)
		{
			delete pPlotSettings;
			pPlotSettings = NULL;
		}
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

	pPlotSettings->copyFrom(pLayout);
	pLayout->close();
	pBTR->close();
	pPlotSettings->setPlotHidden(false);
	es = pPSV->setPlotRotation(pPlotSettings, AcDbPlotSettings::k0degrees);
	es = pPSV->setPlotCfgName(pPlotSettings, m_str_divice);
	pPSV->refreshLists(pPlotSettings);
	es = pPSV->setUseStandardScale(pPlotSettings, Adesk::kTrue);
	es = pPSV->setStdScaleType(pPlotSettings, AcDbPlotSettings::kScaleToFit);

	vector<AcGePoint2dArray> v_extents;
	if(!SelectEntitys(m_TkIdA, _T("X"))) return;

	ads_point pt_min, pt_max;
	GetEntsExtent(m_TkIdA, pt_min, pt_max, 4);

	if(m_nType == 7 || m_nType == 8)
	{
		EntDel(aname(m_objTK));
	}
	es = pPSV->setPlotWindowArea(pPlotSettings, pt_min[X], pt_min[Y], pt_max[X], pt_max[Y]);
	es = pPSV->setPlotType(pPlotSettings, AcDbPlotSettings::kWindow);
	es = pPSV->setPlotCentered(pPlotSettings, true);
	const TCHAR* p = (LPCTSTR)real_size_name;
	es = pPSV->setCanonicalMediaName(pPlotSettings, p);
	double numerator, denominator;
	es = pPlotSettings->getCustomPrintScale(numerator, denominator);
	double Scale = numerator / denominator;
	es = pPSV->setPlotCentered(pPlotSettings, true);
	AcPlPlotEngine* pEngine = NULL;
	if(Acad::eOk == AcPlPlotFactory::createPublishEngine(pEngine))
	{
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
		AcPlPlotPageInfo pageInfo;
		AcPlPlotInfo plotInfo;
		plotInfo.setLayout(layoutId);
		plotInfo.setOverrideSettings(pPlotSettings);
		AcPlPlotInfoValidator validator;
		validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);
		es = validator.validate(plotInfo);
		CString str_file_path_name;
		GetRasterImageFilePath(m_raster_format, str_file_path_name);
		TCHAR jpg_pathname[MAX_PATH] = {0};
		_tcscpy(jpg_pathname, str_file_path_name);
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
		es = pEngine->endPlot();
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

bool XMJGAssist::GetPlotDevice(RasterFormat1 format, CString &device_name, CString &pmp_name)
{
	if(format == Raster_Jpg)
	{
		device_name = _T("CASS_RASTER_JPG.pc3");
		pmp_name = _T("CASS_RASTER_JPG.pmp");
	}
	else if(format == Raster_Bmp)
	{
		device_name = _T("CASS_RASTER_BMP.pc3");
		pmp_name = _T("CASS_RASTER_BMP.pmp");
	}
	else
	{
		return false;
	}
	return true;
}

void XMJGAssist::CopyPrinterConfigFile()
{
	CString str_pc3_path, str_pmp_file_path;
	TCHAR sz_pc3_soure_path[1024] = {0}, sz_pmp_soure_path[1024] = {0};
	if(!GetPrinterPath(str_pc3_path))	return;

	// pc3 file
	if(RTNORM != acedFindFile(m_str_divice, sz_pc3_soure_path))	return;

	CPath path;
	path.Combine(str_pc3_path, m_str_divice);
	if(!IsPathFileExists(path.m_strPath))
		CopyFile(sz_pc3_soure_path, path.m_strPath, false);

	// pmp file
	if(RTNORM != acedFindFile(m_str_pmp_file_name, sz_pmp_soure_path)) return;

	path.Combine(str_pc3_path, _T("PMP Files"));
	str_pmp_file_path = path.m_strPath;
	path.Combine(str_pmp_file_path, m_str_pmp_file_name);
	if(!IsPathFileExists(path.m_strPath))
		CopyFile(sz_pmp_soure_path, path.m_strPath, false);
}

bool XMJGAssist::GetPlotSizeName(AcDbPlotSettingsValidator* pPSV, AcDbPlotSettings* pPlotSettings, CString& real_size_name)
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
		if(m_str_size_name.CompareNoCase(pLocaleName) == 0)
		{
			real_size_name.Format(_T("%s"), MediaList[i]);
			return true;
		}
	}
	return false;
}

bool XMJGAssist::GetPrinterPath(CString &printer_path)
{
	CWinApp *pApp = acedGetAcadWinApp();
	HRESULT hRes; LPDISPATCH pDisp = NULL;
	if(!pApp) return false;

	pDisp = pApp->GetIDispatch(TRUE);
	if(!pDisp) return false;

	CComPtr<IAcadApplication>  pComApp;
	hRes = pDisp->QueryInterface(IID_IAcadApplication, (void**)&pComApp);
	if(FAILED(hRes)) return false;

	CComPtr<IAcadPreferences>  pComPref;
	hRes = pComApp->get_Preferences(&pComPref);
	if(FAILED(hRes)) return false;

	CComPtr<IAcadPreferencesFiles>  pComOutputPref;
	hRes = pComPref->get_Files(&pComOutputPref);
	if(FAILED(hRes)) return false;

	BSTR bstDefaultFilePath;
	hRes = ((IAcadPreferencesFiles*)(pComOutputPref.p))->get_PrinterConfigPath(&bstDefaultFilePath);
	if(FAILED(hRes)) return false;
	printer_path = CString(bstDefaultFilePath);
	return true;
}

bool XMJGAssist::GetRasterImageFilePath(RasterFormat1 format, CString &file_path)
{
	CString str_dwg_path_name;
	GetZdtPathName(str_dwg_path_name);
	CString str_ext;
	str_dwg_path_name.Replace(_T(".dwg"), str_ext);
	file_path = str_dwg_path_name;
	return true;
}

bool XMJGAssist::GetZdtPathName(CString& str_pathname)
{
	if(0 == _tcsicmp(m_szPath, _T("")))
		_tcscpy(m_szPath, GetCurrentDwgPrefix());
	CString str_zd_folder, str_zrz_folder;
	CString str_tmp_path;
	str_tmp_path.Format(_T("%s%s"), m_szPath, str_zd_folder);
	if(!IsPathFileExists(str_tmp_path))
	{
		if(!CreateDirectory(str_tmp_path, NULL))
		{
			return false;
		}
	}
	str_tmp_path += _T("\\");
	str_tmp_path += str_zrz_folder;
	if(!IsPathFileExists(str_tmp_path))
	{
		if(!CreateDirectory(str_tmp_path, NULL))
		{
			return false;
		}
	}
	CString str = str_tmp_path;
	CString str_file_name;
	TCHAR sz_djh[30] = {0};
	if(m_nType == 7)
	{
		str_file_name = _T("无框宗地图");
		str_tmp_path.Format(_T("%s\\%s.dwg"), str, str_file_name);
	}
	else if(m_nType == 8)
	{
		if(str_file_name.IsEmpty())
		{
			str_file_name = sz_djh;
		}
		str_tmp_path.Format(_T("%s\\%s_权证宗地图.dwg"), str, str_file_name);
	}
	else if(m_nType == DKTK_LANDMANAGE)
	{
		if(str_file_name.IsEmpty())
		{
			str_file_name = sz_djh;
		}
		str_tmp_path.Format(_T("%s\\%s_宗地草图.dwg"), str, str_file_name);
	}
	else
	{
		if(str_file_name.IsEmpty())
		{
			str_file_name = sz_djh;
		}
		str_tmp_path.Format(_T("%s\\%s_宗地图.dwg"), str, str_file_name);
	}

	str_tmp_path.Replace(_T("\\\\"), _T("\\"));
	str_pathname = str_tmp_path;
	return true;
}

bool XMJGAssist::GetZdtPara()
{
	m_dTkW = 130.0 * m_Scale; m_dTkH = 140.0 * m_Scale;
	m_str_size_name = _T("A3竖");
	return true;
}

int XMJGAssist::GetPoint2PolyRect(AcGePoint2dArray &nodes, const AcGePoint2d &pt)
{
	int len = nodes.length(), index = -1;
	double dist = 9999999.000;
	for(int idx = 0; idx < len; ++idx)
	{
		AcGePoint2d temp = nodes.at(idx);
		double dis = distOF2d(pt, temp);
		if(dist < dis) continue;
		index = idx; dist = dis;
	}
	return index;
}

bool XMJGAssist::ReadRegForContext(const TCHAR *name, CString &dir)
{
	TCHAR mdbpath[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), mdbpath))
	{
		AfxMessageBox(_T("数据库文件xmjg.mdb无法找到"));
		return false;
	}
	CDatabaseOperManager database;
	database.initialConnect(mdbpath);
	MStr filter; filter[_T("关键字")] = name; CString code, path, curDir;
	database.ReadDataBase(_T("XMJGPath"), _T("属性值"), filter, code);
	filter[_T("关键字")] = _T("工程目录");
	database.ReadDataBase(_T("XMJGPath"), _T("属性值"), filter, path);
	dir.Format(_T("%s%s\\%s"), path, name, code);
	filter[_T("关键字")] = _T("当前目录");
	database.ReadDataBase(_T("XMJGPath"), _T("属性值"), filter, curDir);

#pragma region 创建保存图片文件信息
	/*
	TCHAR config[256] = {0};
	_stprintf(config, _T("%s\\config.csv"), dir);
	if(0 == _taccess(config, 0)) return true;
	CStdioFile file;
	BOOL ret = file.Open(config, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	CString format(_T("编号, 名称, 描述\n"));
	file.WriteString(format);
	file.Flush();
	*/
#pragma endregion

	return true;
}

bool XMJGAssist::ReadRegForAcceptCode(const TCHAR *name, CString &code)
{
	HKEY rkey; TCHAR path[255] = {0}; CString dir;
	int es = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\SouthSurvey\\XMJG"), 0, KEY_ALL_ACCESS, &rkey);
	if(es != ERROR_SUCCESS) return false;
	CassQueryReg(rkey, name, path);
	RegCloseKey(rkey);
	dir.Format(_T("%s"), path);
	int len = (int)_tcslen(path);
	int pos = dir.ReverseFind(_T('\\'));
	code.Format(_T("%s"), dir.Right(len - pos + 1));
	return true;
}

bool XMJGAssist::SetProjectContext()
{
	TCHAR ProjectContext[255] = {0}; CString dir;
	if(false == GetDirectory(_T("请设置工程目录"), ProjectContext)) return false;
	CDatabaseOperManager manager;
	TCHAR path[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), path)) return false;
	manager.initialConnect(path); MStr filter, record;
	filter[_T("关键字")] = _T("工程目录");
	record[_T("属性值")] = ProjectContext;
	manager.UpdataRecord(_T("XMJGPath"), filter, record);
	manager.close();
	return false;
}

bool XMJGAssist::SetProjectContext1()
{
	TCHAR ProjectContext[255] = {0}; CString dir;

	CSelectFolderDlg dlg1;
	CString strPath = dlg1.Show();
	if(strPath.IsEmpty()) return false;

	CDatabaseOperManager manager;
	TCHAR path[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), path)) return false;
	manager.initialConnect(path); MStr filter, record;
	filter[_T("关键字")] = _T("工程目录");
	record[_T("属性值")] = strPath;
	manager.UpdataRecord(_T("XMJGPath"), filter, record);
	manager.close();
	return false;
}

bool XMJGAssist::SetProjectCode(const TCHAR *name, bool config)
{
	try
	{
		CAcModuleResourceOverride myResources;			//声明该对象用来解决资源冲突问题
		XMJGProjectCodeDlg dlg(name, config);
		if(IDOK != dlg.DoModal()) return false;
	}
	catch(...)
	{
		printErrorInfo(__FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

bool XMJGAssist::DirIsExist(TCHAR *path)
{
	return _taccess(path, 0) == -1 ? false : true;
	return true;
}

bool XMJGAssist::getEntsByPoint(ads_point pt, AcDbObjectIdArray &ids)
{
	AcGePoint2d lb, lt, rt, rb; AcGePoint2dArray node;
	double offset = 0.5;
	lt.x = lb.x = pt[X] - offset; rb.y = lb.y = pt[Y] - offset;
	rt.x = rb.x = pt[X] + offset; lt.y = rt.y = pt[Y] + offset;
	node.append(lb); node.append(lt); node.append(rt); node.append(rb);
	return ssFromNodes(ids, node, 2, 1.0, 0, _T("红线"));
}

double XMJGAssist::GetOutCornerAndPosition(AcGePoint2dArray &Nodes, int i, bool bDeasil)
{
	ads_point pt, pt1, pt2;
	double dAngle1, dAngle2;
	int nLen = Nodes.length();
	if(nLen < 2) return 0;
	pt[X] = Nodes[i].x; pt[Y] = Nodes[i].y; pt[Z] = 0.0;
	int sidx = (nLen + i - 1) % nLen, eidx = (nLen + i + 1) % nLen;
	pt1[X] = Nodes[sidx].x; pt1[Y] = Nodes[sidx].y; pt1[Z] = 0.0;
	pt2[X] = Nodes[eidx].x; pt2[Y] = Nodes[eidx].y; pt2[Z] = 0.0;
	dAngle1 = ads_angle(pt, pt1); dAngle2 = ads_angle(pt, pt2);
	return getAngle(dAngle1, dAngle2, bDeasil);
}

double XMJGAssist::getAngle(double dAngle1, double dAngle2, bool flag)
{
	double dTemp, angle;
	if(true == flag)
	{
		dTemp = dAngle1 - dAngle2;
		if(dTemp < 0.0)
			dTemp += PI * 2;
		angle = dAngle2 + dTemp / 2;
	}
	else
	{
		dTemp = dAngle2 - dAngle1;
		if(dTemp < 0.0)
			dTemp += PI * 2;
		angle = dAngle1 + dTemp / 2;
	}
	return angle < 2 * PI ? angle : (angle - 2 * PI);
}

CString XMJGAssist::getExportFilePath(const CString &name, bool isCG)
{
	TCHAR path[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), path))
	{
		MessageBox(GetForegroundWindow(),
			_T("\n 无法找到数据库文件XMJG.mdb！！！"),
			_T("错误信息"), MB_OK); return CString(_T(""));
	}
	CDatabaseOperManager manager; CString filename;
	RETFALSE(manager.initialConnect(path)) filename;
	MStr filter;
	filter[_T("关键字")] = name;
	RETFALSE(manager.ReadDataBase(_T("XMJGPath"), _T("属性值"), filter, filename)) _T("");
	CString filepath; filter.clear();
	filter[_T("关键字")] = _T("工程目录");
	RETFALSE(manager.ReadDataBase(_T("XMJGPath"), _T("属性值"), filter, filepath)) _T("");
	if(filepath.Right(1) != _T("\\") && filepath.Right(1) != _T("/"))
		filepath += _T("\\");
	filepath += name + _T("\\") + filename;
	if(isCG == false) return filepath;
	filepath += _T("\\CG");
	if(-1 == _taccess(filepath, 0))
		_tmkdir(filepath);
	return filepath;
}

CString XMJGAssist::getExportFileName(const CString &name)
{
	TCHAR path[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), path))
	{
		MessageBox(GetForegroundWindow(),
			_T("\n 无法找到数据库文件XMJG.mdb！！！"),
			_T("错误信息"), MB_OK); return CString(_T(""));
	}
	CDatabaseOperManager manager; CString filename;
	RETFALSE(manager.initialConnect(path)) filename;
	MStr filter;
	filter[_T("项目名称")] = name;
	RETFALSE(manager.ReadDataBase(_T("FileName"), _T("文件名称"), filter, filename)) _T("");
	return filename;
}

CString XMJGAssist::getExportPathName(const CString &name)
{
	CString path = getExportFilePath(name);
	CString file = getExportFileName(name);
	CString pathname;
	pathname.Format(_T("%s\\%s"), path, file);
	return pathname;
}

CString XMJGAssist::getExportFilePath()
{
	TCHAR path[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), path))
	{
		MessageBox(GetForegroundWindow(),
			_T("\n 无法找到数据库文件XMJG.mdb！！！"),
			_T("错误信息"), MB_OK); return CString(_T(""));
	}
	CDatabaseOperManager manager; CString filepath;
	RETFALSE(manager.initialConnect(path)) filepath;
	MStr filter;
	filter[_T("关键字")] = _T("当前目录");
	RETFALSE(manager.ReadDataBase(_T("XMJGPath"), _T("属性值"), filter, filepath)) _T("");
	return filepath;
}

CString XMJGAssist::getExportFilePath(bool isGC)
{
	TCHAR path[256] = {0};
	if(RTNORM != ads_findfile(_T("xmjg.mdb"), path))
	{
		MessageBox(GetForegroundWindow(),
			_T("\n 无法找到数据库文件XMJG.mdb！！！"),
			_T("错误信息"), MB_OK); return CString(_T(""));
	}
	CDatabaseOperManager manager; CString filepath;
	RETFALSE(manager.initialConnect(path)) filepath;
	MStr filter;
	filter[_T("关键字")] = _T("当前目录");
	RETFALSE(manager.ReadDataBase(_T("XMJGPath"), _T("属性值"), filter, filepath)) _T("");
	return filepath;
}

void XMJGAssist::copyFiles(const CString &path)
{
	CStringArray srcs;
	if(false == filesOpe::GetMulCassFileName(_T("请选择需要上传的文件"), _T(""), _T(""), NULL, 0, srcs)) return;
	int len = srcs.GetCount();
	for(int idx = 0; idx < len; ++idx)
	{
		CString src = srcs.GetAt(idx);
		bool isFolder = (bool)GetFileAttributes(src)&FILE_ATTRIBUTE_DIRECTORY;
		if(isFolder == true)
			CFilePathOperater::CopyFileEx(src, path, _T(""), true);
		else
		{
			int pos = src.ReverseFind(_T('\\'));
			CString name = src.Right(pos);
			CString des = path + name;
			CopyFile(src, des, FALSE);
		}
	}
}

void XMJGAssist::readTableData(MSBuild &info)
{
	TCHAR configfile[255] = {0};
	if(RTNORM != ads_findfile(_T("面积统计.ini"), configfile))
	{
		AfxMessageBox(_T("查找配置文件[面积统计.ini]失败！！！"));
		return;
	}
	CString filepath(configfile), line, lname, bname;
	CStdioFile sf; BuildHAH build; LayerHAH layer;
	if(FALSE == sf.Open(filepath, CFile::modeReadWrite)) return;
	while(sf.ReadString(line))
	{
		//栋号	层次	层高	建筑面积	计容面积	建筑面积小计	计容面积小计//	建筑占地面积
		int pos = line.Find(_T(":"));
		if(pos == -1) continue;
		CString name = line.Left(pos);
		line = line.Mid(pos + 2);
		pos = line.Find(_T(","));
		if(name.CompareNoCase(_T("Build")) == 0)
		{
			if(bname.CompareNoCase(build.m_build) != 0 && layer.m_function.size() != 0)
			{
				if(build.m_layers.find(layer.m_slayer) == build.m_layers.end())
					build.m_layers[layer.m_slayer] = layer;
				else
				{
					build.m_layers[layer.m_slayer].m_jrzmj = layer.m_jrzmj;
					build.m_layers[layer.m_slayer].m_jzzmj = layer.m_jzzmj;
				}
				build.m_layers[layer.m_slayer] = layer;
				layer.m_function.clear();
				bname = build.m_build;
				MSBIter buildit = info.find(build.m_build);
				if(build.m_layers.size() != 0 && buildit == info.end())
					info[build.m_build] = build;
			}
			build.m_build = line.Left(pos);
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			build.m_jzgd = _tstof(line.Left(pos));
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			build.m_jzmjxj = _tstof(line.Left(pos));
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			build.m_jrmjxj = _tstof(line.Left(pos));
			build.m_jzzdmj = _tstof(line.Mid(pos + 1));
		}
		else if(name.CompareNoCase(_T("Layer")) == 0)
		{
			if(layer.m_function.size() != 0)
			{
				build.m_layers[layer.m_slayer] = layer;
				build.funccount += layer.funccount;
				layer.m_function.clear();
				layer.funccount = 0;
			}
			layer.m_lch = line.Left(pos);
			if(fabs(_tstof(layer.m_lch) - _ttoi(layer.m_lch) - 0.5) < EPS) continue;
			getJGLayerNum(layer.m_lch, layer.m_slayer, layer.m_elayer);
			if(layer.m_slayer == 0 || layer.m_elayer == 0) continue;
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			layer.m_lcg = _tstof(line.Left(pos));
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			layer.m_jzzmj = _tstof(line.Left(pos));
			layer.m_jrzmj = _tstof(line.Mid(pos + 1));
		}
		else if(name.CompareNoCase(_T("Function")) == 0)
		{
			FunctionHAH funct;
			funct.m_cg = (float)_tstof(line.Left(pos));
			line = line.Mid(pos + 1);
			pos = line.Find(_T(","));
			funct.m_jzmj = _tstof(line.Left(pos));
			funct.m_jrmj = _tstof(line.Mid(pos + 1));
			layer.m_function[funct.m_cg] = funct;
			layer.funccount++;
		}
	}
	build.m_layers[layer.m_slayer] = layer;
	build.funccount += layer.funccount;
	info[build.m_build] = build;
}

void XMJGAssist::exportTable2Doc(SmartTable::Word &word)
{
	IProjectMDB pdb; VMStr cxxs = pdb.getCXXTableInfo();
	MSBuild build; CString bname;
	for(int idx = 0; idx < (int)cxxs.size(); ++idx)
	{
		MStr cxx = cxxs.at(idx);
		MSIter cxxit = cxx.begin();
		if(cxx[_T("楼栋号")].CompareNoCase(bname) != 0)
		{

		}



		for(; cxxit != cxx.end(); ++cxxit)
		{





		}


	}
}

void XMJGAssist::exportTable2Doc(SmartTable::Word &word, const MSBuild &hahs)
{
	IDataBaseOper oper; long standard = oper.readJRStartTime();
	IProjectMDB pdb; MStr record = pdb.getJBXXTableInfo();
	long starttime = TimeConvertLong(record[_T("审批时间")]);
	MSBCIter buildit = hahs.begin();  int rowidx = 8;
	float jzmjxj = 0.0, jrmjxj = 0.0, jzzdmjxj = 0.0;
	bool isJr = starttime > standard;
	for(; buildit != hahs.end(); ++buildit)
	{
		exportTable2Doc(word, buildit->second, rowidx, jzzdmjxj, isJr);
		jzmjxj += (float)buildit->second.m_jzmjxj;
		jrmjxj += (float)buildit->second.m_jrmjxj;
	}
	SmartTable::WordTable table;
	if(-1 == word.findTable(rowidx, 1, "#layer#", table))
	{
		acutPrintf(_T("\n 无法找到标志位!!!"));
		return;
	}
	else
	{
		word.replaceText("#layer#", "");
	}
	table.setCellText(rowidx, 1, "合计");
	char value[255] = {0};
	sprintf(value, "%.3f", jzmjxj);
	table.setCellText(rowidx, 4, value);
	table.setCellText(rowidx, 6, value);
	sprintf(value, "%.3f", jzzdmjxj);
	table.setCellText(rowidx, 8, value);
	if(!isJr) sprintf(value, "%s", "/");
	else sprintf(value, "%.3f", jrmjxj);
	table.setCellText(rowidx, 5, value);
	table.setCellText(rowidx, 7, value);
}

void XMJGAssist::exportTable2Doc(SmartTable::Word &word, const BuildHAH &hah, int &rowidx, float &zjzzdmj, bool jr)
{
	SmartTable::WordTable table; int srowidx = rowidx;
	if(-1 == word.findTable(rowidx, 1, "#layer#", table))
	{
		acutPrintf(_T("\n 无法找到标志位!!!"));
		return;
	}
	MILCIter layerit = hah.m_layers.begin();
	int lower = 0, upper = 0; char value[255] = {0};
	double dsjzmj = 0.0, dxjzmj = 0.0, bdxcjzmj = 0.0;
	double dxjrmj = 0.0, dsjrmj = 0.0, bdxcjrmj = 0.0;
	double jzzdmj = 0.0;
	int sidxds = 0, eidxds = -1;		//地上层的起始行号
	int	sidxdx = 0, eidxdx = -1;		//地下层的起始行号
	int sidxjzzdmj = 0, eidxjzzdmj = -1;	//建筑占地面积起始行号
	int sidxbdxc = 0, eidxbdxc = -1;
	IProjectMDB pdb; MStr filter, record;
	for(; layerit != hah.m_layers.end(); ++layerit)
	{
		CString layername = layerit->second.m_lch;
		if(layername.Left(1) != _T('-') && layername.Find(_T("--->")) == -1 && stringIsDigit(layername) == false)
		{
			layername.Format(_T("%s"), g_fieldchagetable.getIndexByName(layername));
			lower = upper = _ttoi(layername);
		}
		else
			getLayerNum(layername, lower, upper);

		for(int idx = lower; idx <= upper; ++idx)
		{
			if(idx == 0 || idx > 990)
			{
				TCHAR cindex[255] = {0};
				_stprintf(cindex, _T("%d"), idx);
				CString cIndex = g_fieldchagetable.getNameByIndex(cindex);
				TcharToChar(cIndex, value);
			}
			else sprintf(value, "%d", idx);
			filter[_T("层号")] = value;

			record = pdb.getCXXTableInfo(filter);

			table.setCellText(rowidx, 2, value);
			if(idx < 0)
			{
				dxjzmj += layerit->second.m_jzzmj;
				dxjrmj += layerit->second.m_jrzmj;
			}
			else if(idx == 0)
			{
				bdxcjzmj += layerit->second.m_jzzmj;
				bdxcjrmj += layerit->second.m_jrzmj;
			}
			LayerHAH layer = layerit->second;
			MFFIter it = layer.m_function.begin();
			int size = layer.funccount;
			int funcount = (int)layer.m_function.size();
			if(funcount == 0) continue;
			table.copyRow(rowidx, 0, funcount);
			table.setCellText(rowidx + 1, 2, "");
			table.merge(rowidx, 2, rowidx + funcount - 1, 2);
			table.merge(rowidx, 9, rowidx + funcount - 1, 9);
			TcharToChar(record[_T("层备注")], value);
			table.setCellText(rowidx, 9, value);
			for(; it != layer.m_function.end(); ++it)
			{
				if(idx < 0)
				{
					sidxdx = sidxdx == 0 ? rowidx : sidxdx;
					eidxdx++;
				}
				else if(idx == 0)
				{
					sidxbdxc = sidxbdxc == 0 ? rowidx : sidxbdxc;
					eidxbdxc++;
				}
				else if(idx > 0)
				{
					sidxds = sidxds == 0 ? rowidx : sidxds;
					eidxds++;

					dsjzmj += it->second.m_jzmj;
					dsjrmj += it->second.m_jrmj;
					if(idx == 1)
						jzzdmj = dsjzmj;
				}
				sprintf(value, "%.2f", it->first);
				table.setCellText(rowidx, 3, value);
				sprintf(value, "%.3f", it->second.m_jzmj);
				table.setCellText(rowidx, 4, value);
				sprintf(value, "%.3f", it->second.m_jrmj);
				if(jr == true)
					table.setCellText(rowidx, 5, value);
				rowidx++;
			}
		}
	}
	table.merge(sidxdx, 8, sidxdx + eidxdx + 1, 8);
	table.merge(sidxdx, 6, sidxdx + eidxdx, 6); sprintf(value, "%.3lf", dxjzmj); table.setCellText(sidxdx, 6, value);					//地下面积
	table.merge(sidxds, 6, sidxds + eidxds, 6); sprintf(value, "%.3lf", dsjzmj); table.setCellText(sidxds, 6, value);					//地上面积
	table.merge(sidxds, 8, sidxds + eidxds, 8); sprintf(value, "%.3lf", jzzdmj); table.setCellText(sidxds, 8, value);					//建筑占地面积
	table.merge(sidxbdxc, 6, sidxbdxc + eidxbdxc, 6); sprintf(value, "%.3lf", bdxcjzmj); table.setCellText(sidxbdxc, 6, value);			//半地下层
	if(jr)
	{
		table.merge(sidxdx, 7, sidxdx + eidxdx, 7); sprintf(value, "%.3lf", dxjrmj); table.setCellText(sidxdx, 7, value);
		table.merge(sidxds, 7, sidxds + eidxds, 7); sprintf(value, "%.3lf", dsjrmj); table.setCellText(sidxds, 7, value);
		table.merge(sidxbdxc, 7, sidxbdxc + eidxbdxc, 7); sprintf(value, "%.3lf", bdxcjrmj); table.setCellText(sidxbdxc, 7, value);
	}
	else
	{
		table.merge(sidxdx, 7, sidxdx + eidxdx, 7); sprintf(value, "%s", "/"); table.setCellText(sidxdx, 7, value);
		table.merge(sidxds, 7, sidxds + eidxds, 7); sprintf(value, "%s", "/"); table.setCellText(sidxds, 7, value);
		table.merge(sidxbdxc, 7, sidxbdxc + eidxbdxc, 7); sprintf(value, "%s", "/"); table.setCellText(sidxbdxc, 7, value);
	}
	TcharToChar(hah.m_build, value); int rowcount = rowidx - srowidx - 1; table.merge(srowidx, 1, rowidx - 1, 1);
	table.setCellText(srowidx, 1, value); zjzzdmj += jzzdmj;
}

#pragma endregion

XMJGExportZPT::XMJGExportZPT()
	: m_scale(500)
{
	read1(m_scale); m_scale /= 1000;
}

XMJGExportZPT::~XMJGExportZPT()
{
}

void XMJGExportZPT::setUndoFlag()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("G"), RTNONE);
}

void XMJGExportZPT::beginUndo()
{
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("E"), RTNONE);
	ads_command(RTSTR, _T("UNDO"), RTSTR, _T("1"), RTNONE);
}

bool XMJGExportZPT::drawMesh()
{
	AcGePoint2d min, max;
	GetExternPoint(min, max);
	DrawCrosshair(min, max);
	DrawTuKuang(min, max);
	AddTuKuangTitle(min, max);
	delScaleNode(apoint(min));
	if(!InsertTuQian(min, max)) return false;
	AddCornerCoor(min, max);
	return true;
}

bool XMJGExportZPT::drawMesh(CString strTitle)
{
	AcGePoint2d min, max;
	GetExternPoint(min, max);
	DrawCrosshair(min, max);
	DrawTuKuang(min, max);
	AddTuKuangTitle(min, max, strTitle);
	delScaleNode(apoint(min));
	InsertTuQian(min, max);
	AddCornerCoor(min, max);
	return true;
}

bool XMJGExportZPT::setExportRange()
{
	ads_point pt1, pt2;	SNodesOpe nodesO;
	if(RTCAN == ads_getpoint(NULL, _T("\n 请指定范围线左下角"), pt1)) return false;
	if(RTCAN == ads_getcorner(pt1, _T("\n 请指定范围线右上角"), pt2)) return false;
	nodesO.modifyLB_RTPoint(pt1, pt2);

	m_lb.x = pt1[X]; m_lb.y = pt1[Y];
	m_rt.x = pt2[X]; m_rt.y = pt2[Y];

	AcGePoint2dArray nodes;
	nodes.append(m_lb);
	nodes.append(AcGePoint2d(m_rt.x, m_lb.y));
	nodes.append(m_rt);
	nodes.append(AcGePoint2d(m_lb.x, m_rt.y));
	AcDbObjectId id = DrawPolyline2D(nodes, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);
	EntDel(aname(id));
	return true;
}

void XMJGExportZPT::setTuQian(const CString &tq)
{
	m_tuqian = tq;
}

void XMJGExportZPT::setJZMJArea(double zjzmj, double dsjzmj, double dxjzmj)
{
	m_zjzmj = zjzmj; m_dsjzmj = dsjzmj; m_dxjzmj = dxjzmj;
}

void XMJGExportZPT::setJZMJArea(double zjzmj, double jsydmj, double dsjzmj, double dxjzmj)
{
	m_jsydmj = jsydmj;
	m_zjzmj = zjzmj; m_dsjzmj = dsjzmj; m_dxjzmj = dxjzmj;
}

void XMJGExportZPT::setExportRange(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	SNodesOpe nodesO; AcGePoint2dArray node;
	nodesO.LB_RT_Nodes(lb, rt, node); m_lb = lb; m_rt = rt;
	AcDbObjectId id = DrawPolyline2D(node, 0, true, _T(""));
	TrimPoly(aname(id), true, false, true);	EntDel(aname(id));
}

bool isExistFolder(const CString &dir)
{
	if(-1 != _taccess(dir, 0)) return true;
	CString path;
	int pos = dir.ReverseFind(_T('\\'));
	path = dir.Mid(0, pos);
	isExistFolder(path);
	_tmkdir(dir);
	return true;
}

CString XMJGExportZPT::beginExport(const CString &name)
{
	XMJGAssist assist;
	CString pathName = assist.getExportFilePath();
	CString fileName = assist.getExportFileName(name);
	pathName += _T("\\"); pathName += fileName;	ads_name ssSave;
	bool bSuc = isExistFolder(pathName); filterEntity(name);
	ads_command(RTSTR, _T("zoom"), RTSTR, _T("e"), RTNONE);
	ads_ssget(_T("X"), NULL, NULL, NULL, ssSave);
	AcDbObjectIdArray idArr; SSToIdArr(ssSave, idArr);
	CSaveBlocks blocks; blocks.setPath(pathName); ads_point ptMin, ptMax;
	GetEntsExtent(idArr, ptMin, ptMax, 1.0);
	/*ptMin[X] = m_lb.x;
	ptMin[Y] = m_lb.y;
	ptMax[X] = m_rt.x;
	ptMax[Y] = m_rt.y;*/
	AcGePoint2dArray node;
	node.append(AcGePoint2d(ptMin[X], ptMin[Y])); node.append(AcGePoint2d(ptMin[X], ptMax[Y]));
	node.append(AcGePoint2d(ptMax[X], ptMax[Y])); node.append(AcGePoint2d(ptMax[X], ptMin[Y]));
	AcDbObjectId dyfw = DrawPolyline2D(node, 1.0, true, _T("")); setlayer(aname(dyfw), _T("打印范围"));
	idArr.append(dyfw); blocks.saveBlocks2(idArr, false);
	return pathName;
}

void XMJGExportZPT::delScaleNode(const AcGePoint2d &pos)
{
	AcGePoint3d insertpos; insertpos.x = pos.x; insertpos.y = pos.y - 12;
	IDataBaseOper oper; TextProperty prop = oper.readTextPropertyTable(_T("总平图注解"));
	AcDbObjectId id = DrawText(insertpos, _T("注：此图为样图"), _T("ML"), _T(""), prop._size);
	setlayer(aname(id), prop._layer); setcolor(aname(id), prop._color);
	setTextWidthFactor(id, 1.0);
}

void XMJGExportZPT::GetExternPoint(AcGePoint2d &min, AcGePoint2d &max)
{
	min.x = m_lb.x < m_rt.x ? m_lb.x : m_rt.x;
	min.y = m_lb.y < m_rt.y ? m_lb.y : m_rt.y;
	max.x = m_lb.x > m_rt.x ? m_lb.x : m_rt.x;
	max.y = m_lb.y > m_rt.y ? m_lb.y : m_rt.y;

	min.x = setDoubleMinLast(min.x);
	min.y = setDoubleMinLast(min.y);
	max.x = setDoubleMaxLast(max.x);
	max.y = setDoubleMaxLast(max.y);
	return;
	int XCount = (int)((max.x - min.x) / 50 + 1);
	int YCount = (int)((max.y - min.y) / 50 + 1);
	min.x = ((int)(min.x / 10)) * 10;
	min.y = ((int)(min.y / 10)) * 10;
	max.x = min.x + XCount * 50;
	max.y = min.y + YCount * 50;
}

void XMJGExportZPT::DrawCrosshair(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	double scale = 500; scale /= 10;
	for(double row = lb.y; row <= rt.y; row += scale)
	{
		for(double col = lb.x; col <= rt.x; col += scale)
		{
			if(abs(col - lb.x) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), RightDir);
			else if(abs(row - lb.y) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), TopDir);
			else if(abs(col - rt.x) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), LeftDir);
			else if(abs(row - rt.y) < EPS)
				DrawCrosshair(AcGePoint2d(col, row), ButtomDir);
			else
				DrawCrosshair(AcGePoint2d(col, row), AllDir);
		}
	}
}

void XMJGExportZPT::DrawCrosshair(const AcGePoint2d &pos, const DrawDir &dir)
{
	double scale = 500; read1(scale); scale /= 200; AcDbObjectId id;
	AcGePoint2d top(pos), left(pos), right(pos), buttom(pos);
	top.y += scale; left.x -= scale; right.x += scale; buttom.y -= scale;
	if(dir == TopDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(top), _T(""));
		setlayer(aname(id), _T("图框"));
	}
	if(dir == LeftDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(left), _T(""));
		setlayer(aname(id), _T("图框"));
	}
	if(dir == RightDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(right), _T(""));
		setlayer(aname(id), _T("图框"));
	}
	if(dir == ButtomDir || dir == AllDir)
	{
		id = drawline(apoint(pos), apoint(buttom), _T(""));
		setlayer(aname(id), _T("图框"));
	}
}

void XMJGExportZPT::DrawTuKuang(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	AcGePoint2d lt, rb; AcDbObjectId id;
	lt.x = lb.x; lt.y = rt.y;
	rb.x = rt.x; rb.y = lb.y;
	AcGePoint2dArray nodes1, nodes2;
	nodes1.append(lb); nodes1.append(lt);
	nodes1.append(rt); nodes1.append(rb);
	id = DrawPolyline2D(nodes1, 0, true, _T("")); setlayer(aname(id), _T("图框"));
	AcGePoint2d lb1(lb.x, lb.y - m_scale * 12), lb2(lb.x - m_scale * 12, lb.y);
	AcGePoint2d lt1(lt.x, lt.y + m_scale * 12), lt2(lt.x - m_scale * 12, lt.y);
	AcGePoint2d rt1(rt.x, rt.y + m_scale * 12), rt2(rt.x + m_scale * 12, rt.y);
	AcGePoint2d rb1(rb.x, rb.y - m_scale * 12), rb2(rb.x + m_scale * 12, rb.y);
	id = drawline(apoint(lb1), apoint(lt1), _T("")); setlayer(aname(id), _T("图框"));
	id = drawline(apoint(lt2), apoint(rt2), _T("")); setlayer(aname(id), _T("图框"));
	id = drawline(apoint(rt1), apoint(rb1), _T("")); setlayer(aname(id), _T("图框"));
	id = drawline(apoint(rb2), apoint(lb2), _T("")); setlayer(aname(id), _T("图框"));
	Poly2dArrayOffset(nodes1, m_scale * 12, false, nodes2);
	nodes2.first().y -= (m_scale * 12); nodes2.last().y -= (m_scale * 12);
	id = DrawPolyline2D(nodes2, 0, true, _T(""), m_scale * 0.6);
	setlayer(aname(id), _T("图框")); AcGePoint3d pos;
	pos.x = lb1.x + 6 * m_scale; pos.y = lb1.y + 2 * m_scale;
	TCHAR info[20] = {0};
	_stprintf(info, _T("%s"), _T("（图廓坐标均以 “Km”为单位）"));
	id = DrawText(pos, info, _T("BL"), _T(""), 2.5 * m_scale);
	SetTextStyle(id, _T("黑体"), 1.0); setlayer(aname(id), _T("图框"));
}

void XMJGExportZPT::AddTuKuangTitle(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	AcGePoint3d lt(lb.x, rt.y, 0), mid; TCHAR info[255] = {0};
	mid.x = (lb.x + rt.x) / 2; mid.y = rt.y + 17 * m_scale;
	AcGePoint3d rb(rt.x, lb.y, 0);
	IDataBaseOper oper; IProjectMDB pdb; MStr title = pdb.getJBXXTableInfo();
	_stprintf(info, _T("%s%s"), title[_T("项目名称")], oper.readCommonTable(_T("建筑竣工总平图标题")));
	AcDbObjectId id = DrawText(mid, info, _T("MC"), _T(""), 5 * m_scale);
	setlayer(aname(id), _T("图框")); SetTextStyle(id, _T("黑体"), 1.0);
}

void XMJGExportZPT::AddTuKuangTitle(const AcGePoint2d &lb, const AcGePoint2d &rt, const CString strTitle)
{
	AcGePoint3d lt(lb.x, rt.y, 0), mid; TCHAR info[255] = {0};
	mid.x = (lb.x + rt.x) / 2; mid.y = rt.y + 17 * m_scale;
	AcGePoint3d rb(rt.x, lb.y, 0);
	IDataBaseOper oper; IProjectMDB pdb; MStr title = pdb.getJBXXTableInfo();
	_stprintf(info, _T("%s%s"), title[_T("项目名称")], oper.readCommonTable(strTitle));
	AcDbObjectId id = DrawText(mid, info, _T("MC"), _T(""), 5 * m_scale);
	setlayer(aname(id), _T("图框")); SetTextStyle(id, _T("黑体"));
}

bool XMJGExportZPT::InsertTuQian(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	int Position = 0; AcGePoint2d pos; TCHAR pszArea[255] = {0};
	if(RTCAN == ads_getint(_T("\n 请输入插入图签的位置[1]左上、[2]左下、[3]右上、[4]右下：<4>"), &Position)) return false;
	if(Position < 1 || Position > 4) Position = 4;
	switch(Position)
	{
		case 1: pos.x = lb.x; pos.y = rt.y - 100 * m_scale; break;
		case 2: pos.x = lb.x; pos.y = lb.y; break;
		case 3: pos.x = rt.x - 100 * m_scale; pos.y = rt.y - 100 * m_scale; break;
		case 4: pos.x = rt.x - 100 * m_scale; pos.y = lb.y; break;
	}
	IProjectMDB oper; MStr record = oper.getJBXXTableInfo();
	//AcDbObjectId id = DrawInsert(apoint(pos), m_tuqian, m_scale, m_scale);
	AcDbObjectId id;
	bool b = DrawInsert(apoint(pos), m_tuqian, _T(""), m_scale);
	if(b)
	{
		ads_name ent;
		ads_entlast(ent);
		acdbGetObjectId(id, ent);
	}
	else
	{
		return false;
	}
	SBreakBlock m_Block; m_Block.SetBlock(id); //m_Block.Explode();
	m_Block.ExplodeBlocks();
	_stprintf(pszArea, _T("%.2lf"), m_zjzmj);
	m_Block.replaceText(_T("#ZJZMJ#"), pszArea);
	_stprintf(pszArea, _T("%.2lf"), m_dsjzmj);
	m_Block.replaceText(_T("#DSMJ#"), pszArea);
	_stprintf(pszArea, _T("%.2lf"), m_dxjzmj);
	m_Block.replaceText(_T("#DXMJ#"), pszArea);
	_stprintf(pszArea, _T("%.2lf"), m_jsydmj);
	m_Block.replaceText(_T("#JZYDMJ#"), pszArea);
	m_Block.replaceText(_T("#ProjectName#"), record[_T("项目名称")]);
	m_Block.replaceText(_T("#BuildCompany#"), record[_T("建设单位")]);
	m_Block.replaceText(_T("#MeansureCompany#"), record[_T("测量单位")]);
	m_Block.replaceText(_T("#MeasurementData#"), record[_T("报告时间")]);
	setlayer(aname(id), _T("图框"));
	CString jsydmj;
	SelectFilter sf1(8, _T("红线")), sf2(RTDXF0, _T("*POLYLINE"));
	AcDbObjectId hxid; SelectEntity(hxid, sf1, sf2, _T("X"));
	if(!hxid.isValid() || GetPolyType(aname(hxid), true) < 10)
		jsydmj.Format(_T("%s"), _T("红线内局部用地，暂无法界定"));
	else
	{
		double area = 0.0;
		GetEntArea(aname(hxid), area);
		jsydmj.Format(_T("%.3lf平方米"), area);
	}
	m_Block.replaceText(_T("#JSYDMJ#"), jsydmj);
	return true;
}

void XMJGExportZPT::AddCornerCoor(const AcGePoint2d &lb, const AcGePoint2d &rt)
{
	AcGePoint2dArray nodes;
	nodes.append(lb); nodes.append(AcGePoint2d(lb.x, rt.y));
	nodes.append(rt); nodes.append(AcGePoint2d(rt.x, lb.y));
	int length = nodes.length();
	TCHAR info[100] = {0}; AcDbObjectId id;
	AcGePoint3d pos(0, 0, 0);
	AcGeVector3d offset(0, 0, 0);
	TCHAR Align1[20] = {0}, Align2[20] = {0};
	for(int idx = 0; idx < length; ++idx)
	{
		if(idx == 0)
		{
			offset.y = -m_scale * 12;
			_stprintf(Align1, _T("%s"), _T("BC"));
			_stprintf(Align2, _T("%s"), _T("BR"));
		}
		else if(idx == 1)
		{
			offset.y = m_scale * 11.5;
			_stprintf(Align1, _T("%s"), _T("TC"));
			_stprintf(Align2, _T("%s"), _T("TR"));
		}
		else if(idx == 2)
		{
			offset.y = m_scale * 11.5;
			_stprintf(Align1, _T("%s"), _T("TC"));
			_stprintf(Align2, _T("%s"), _T("TL"));
		}
		else
		{
			offset.y = -m_scale * 12;
			_stprintf(Align1, _T("%s"), _T("BC"));
			_stprintf(Align2, _T("%s"), _T("BL"));
		}
		AcGePoint2d tmp = nodes.at(idx);
		pos.x = tmp.x; pos.y = tmp.y;
		//x
		_stprintf(info, _T("%.2f"), tmp.x / 1000);
		id = DrawText(pos + offset, info, Align1, _T(""), 3 * m_scale);
		SetTextStyle(id, _T("黑体"), 1.0);
		setlayer(aname(id), _T("图框"));
		//y
		_stprintf(info, _T("%.2f"), tmp.y / 1000);
		id = DrawText(pos, info, Align2, _T(""), 3 * m_scale);
		setlayer(aname(id), _T("图框"));
		SetTextStyle(id, _T("黑体"), 1.0);
	}
}

void XMJGExportZPT::filterEntity(const CString &name)
{
	IDataBaseOper oper; AcDbObjectIdArray tids;
	CString layer = oper.readCommonTable(name);
	SelectFilter sf(8, layer);
	if(!SelectEntitys(tids, sf, _T("X"))) return;
	EntsDel(tids);
}

bool XMJGExportZPT::drawCompareMesh()
{
	AcGePoint2d min, max;
	GetExternPoint(min, max);
	DrawCrosshair(min, max);
	DrawTuKuang(min, max);

	//添加表头
	AcGePoint3d lt(min.x, max.y, 0), mid; TCHAR info[255] = {0};
	mid.x = (min.x + max.x) / 2; mid.y = max.y + 17 * m_scale;
	AcGePoint3d rb(max.x, min.y, 0);
	IDataBaseOper oper; IProjectMDB pdb; MStr title = pdb.getJBXXTableInfo();
	_stprintf(info, _T("%s%s"), title[_T("项目名称")], oper.readCommonTable(_T("建筑竣工不一致对照示意图标题")));
	AcDbObjectId id = DrawText(mid, info, _T("MC"), _T(""), 5 * m_scale);
	setlayer(aname(id), _T("图框")); SetTextStyle(id, _T("黑体"));
	delScaleNode(apoint(min));

	AddCornerCoor(min, max);
	return true;
}

CCircleOperator::CCircleOperator(const AcDbObjectId &id)
{
	GetPlList(aname(id), m_node);
	GetPlBulges(aname(id), m_bulges);
}

double CCircleOperator::getCircleRadius(int index)
{
	double radAngle = getArcAngle(index);
	if(radAngle == 0.0) return 0.0;
	AcGePoint2d spos = m_node.at(index);
	AcGePoint2d epos = m_node.at(index + 1);
	double dist = distOF2d(spos, epos);
	double angle = (PI - radAngle) / 2;
	double radius = dist / 2 / (double)cosf(angle);
	return radius;
}

double CCircleOperator::getCircleBulge(int index)
{
	return m_bulges.at(index);
}

double CCircleOperator::getArcAngle(int index, bool isRad)
{
	if(index < 0 || index >= m_node.length()) return 0.0;
	double bulge = m_bulges.at(index);
	if(bulge == 0) return 0.0;
	double radAngle = 4 * atan(bulge);
	return isRad ? radAngle : (radAngle * 180 / PI);
}

void swapPoint(ads_point pt1, ads_point pt2)
{
	double dist1 = distOF2d(apoint(pt1), AcGePoint2d(0, 0));
	double dist2 = distOF2d(apoint(pt2), AcGePoint2d(0, 0));
	if(dist1 < dist2) return; double tmp = pt1[X];
	pt1[X] = pt2[X]; pt1[X] = tmp; tmp = pt1[Y];
	pt1[Y] = pt2[Y]; pt2[Y] = tmp;
}

AcGePoint3d CCircleOperator::getArcCenterPos(int index)
{
	double radius = getCircleRadius(index);
	double cangle = getArcAngle(index) / 2;
	AcGePoint2d spos = m_node.at(index);
	AcGePoint2d epos = m_node.at(index + 1);
	ads_point spt; spt[X] = spos.x; spt[Y] = spos.y;
	ads_point ept; ept[X] = epos.x; ept[Y] = epos.y;
	double angle1 = ads_angle(spt, ept);
	double a = 0.0;
	if(angle1 > PI)
	{
		angle1 -= PI;
		a = 2 * PI - (PI / 2 - angle1 - cangle);
	}
	else if(angle1 < 0)
	{
		angle1 += PI;
		a = 2 * PI - (PI / 2 - angle1 - cangle);
	}
	else
	{
		angle1 = PI - angle1;
		a = PI + (PI / 2 - angle1 - cangle);
	}
	ads_point cpos;
	ads_polar(spt, a, radius, cpos);
	return AcGePoint3d(cpos[X], cpos[Y], 0);
}

AcGePoint3d CCircleOperator::getCircleCenterPos(int index)
{
	double radius = getCircleRadius(index);
	AcGePoint3d cpos = getArcCenterPos(index);
	double angle1 = ads_angle(apoint(cpos), apoint(m_node[index]));
	double angle2 = ads_angle(apoint(cpos), apoint(m_node[index]));
	double angle = (angle1 + angle2) * 0.5;
	ads_point mpos; ads_polar(apoint(cpos), angle, radius, mpos);
	return AcGePoint3d(mpos[X], mpos[Y], 0);
}

#define GCCL_RETURN(FLAG) \
	if(FLAG == false) {m_isExport = false; return false;}

CExportGCCL::CExportGCCL()
	: m_isExport(false), m_Block(new SBreakBlock), m_Type(0)
{
	SelectFilter sf(8, _T("TK")); AcDbObjectIdArray ids;
	SelectEntitys(ids, sf, _T("X")); EntsDel(ids);

}

CExportGCCL::~CExportGCCL()
{
	if(m_Block != NULL) delete m_Block; m_Block = NULL;
}

bool CExportGCCL::beginExport(const CString &pf)
{
	GCCL_RETURN(setExportRange());
	GCCL_RETURN(addExportFrame(pf));
	GCCL_RETURN(calculateScale());
	GCCL_RETURN(scaleFontSize());
	modifyAllEntityLineScale();
	acedCommand(RTSTR, _T("Regen"), RTNONE);
	return true;
}

bool CExportGCCL::setExportRange()
{
	ads_point pMin, pMax; SNodesOpe nodesO; AcGePoint2dArray node;
	if(RTNORM != ads_getpoint(NULL, _T("\n <请框选要输出的实体范围> 请按下第一个点："), pMin)) return false;
	if(RTNORM != ads_getcorner(pMin, _T("\n <请框选要输出的实体范围> 请按下第二个点："), pMax)) return false;
	ZoomWindow(pMin, pMax);	nodesO.modifyLB_RTPoint(pMin, pMax);
	m_LeftButton = apoint(pMin); m_RightTop = apoint(pMax);
	m_Middle.x = (m_LeftButton.x + m_RightTop.x) / 2;
	m_Middle.y = (m_LeftButton.y + m_RightTop.y) / 2;
	AcGePoint2dArray m_Rectangular;
	nodesO.LB_RT_Nodes(m_LeftButton, m_RightTop, m_Rectangular);
	ssFromNodes(m_ExportIds, m_Rectangular);
	ads_command(RTSTR, _T("PURGE"), RTSTR, _T("B"), RTSTR, _T("*"), RTSTR, _T("N"), RTNONE);
	m_isExport = true; return true;
}

bool CExportGCCL::calculateScale()
{
	double width = m_RightTop.x - m_LeftButton.x;
	double height = m_RightTop.y - m_LeftButton.y;
	double pfwidth = m_PFRightTop.x - m_PFLeftButton.x;
	double pfheight = m_PFRightTop.y - m_PFLeftButton.y;
	double dwidth = width / pfwidth;
	double dheight = height / pfheight;
	int ifactor = 100;
	if(dwidth > dheight)
	{
		m_Scale = width / pfwidth;
		int ifactor = round(m_Scale * 10); ifactor *= 100;
		m_Scale = ifactor * 1.0 / 1000;
		if((pfwidth * m_Scale) < width)
			m_Scale += 0.1;
	}
	else
	{
		m_Scale = height / pfheight;
		int f = round(m_Scale * 10); f *= 100;
		m_Scale = f * 1.0 / 1000;
		if((pfheight * m_Scale) < height)
			m_Scale += 0.1;
	}
	ifactor = round(m_Scale * 10); ifactor *= 100;
	TCHAR info[255] = {0}; _stprintf(info, _T("%d"), ifactor);
	ScaleEntitys(m_Block->m_objIdArr, apoint(m_Middle), m_Scale);
	m_Block->replaceText(_T("#SCALE#"), info, _T("SCALE"));
	CXRecordOperate oper;
	oper.AddDoubleRecord(_T("GCCL"), _T("BLC"), m_Scale);
	oper.AddIntegerRecord(_T("GCCL"), _T("TKLX"), m_Type);
	return true;
}

bool CExportGCCL::scaleFontSize()
{
	double size = m_Scale * 1.679;
	modifyEntsTextHeight(m_ExportIds, size);
	modifyEntsArrowSize(m_ExportIds, m_Scale);
	return true;
}

void CExportGCCL::modifyAllEntityLineScale()
{
	acedCommand(RTSTR, _T("zoom"), RTSTR, _T("e"), 0);
	AcDbObjectIdArray ids;
	SelectEntitys(ids, _T("X"));
	acedSetStatusBarProgressMeter(_T("整饰图面..."), 0, ids.length());
	acDocManager->lockDocument(curDoc());
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		acedSetStatusBarProgressMeterPos(idx);
		AcDbObjectId id = ids.at(idx);
		AcDbEntity *pEnt = NULL; Acad::ErrorStatus es;
		es = acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
		if(es != Acad::eOk) return;
		pEnt->setLinetypeScale(m_Scale);
		pEnt->close();
	}
	acDocManager->unlockDocument(curDoc());
	acedRestoreStatusBar();
}

bool CExportGCCL::addExportFrame(const CString &pf)
{
	AcDbObjectId tk = DrawInsert(apoint(m_Middle), pf);
	if(tk == AcDbObjectId::kNull) return false;
	m_Block->SetBlock(tk); m_Block->Explode();
	AcDbObjectId lbid, rtid;
	m_Block->findPointByHyperLink(_T("左下角点"), m_PFLeftButton, lbid);
	m_Block->findPointByHyperLink(_T("右上角点"), m_PFRightTop, rtid);
	EntDel(aname(lbid)); EntDel(aname(rtid));
	m_PFMiddle.x = (m_PFLeftButton.x + m_PFRightTop.x) / 2;
	m_PFMiddle.y = (m_PFLeftButton.y + m_PFRightTop.y) / 2;
	double XMove = m_Middle.x - m_PFMiddle.x;
	double YMove = m_Middle.y - m_PFMiddle.y;
	MoveEntitys(m_Block->m_objIdArr, XMove, YMove);
	if((m_PFRightTop.x - m_PFLeftButton.x) < 210)
		m_Type = 1;
	else if((m_PFRightTop.x - m_PFLeftButton.x) < 297)
		m_Type = 2;
	else if((m_PFRightTop.x - m_PFLeftButton.x) < 420)
		m_Type = 3;
	else if((m_PFRightTop.x - m_PFLeftButton.x) < 594)
		m_Type = 4;
	else if((m_PFRightTop.x - m_PFLeftButton.x) < 840)
		m_Type = 5;
	return true;
}

void CExportGCCL::replaceText(const MStr &record)
{
	MSCIter it = record.begin();
	for(; it != record.end(); ++it)
		m_Block->replaceTextEx(it->first, it->second);
}

bool CExportGCCL::modifyArrow()
{

	return true;
}

bool XMJGExportFontSizeConfig::GetTextLayers(vector<CString> &vLyrs)
{
	vLyrs.clear();
	CString strField = _T("数据注记图层"), strValue;
	if(m_db.readCommonTable(strField, strValue))
	{
		AnalyzeData(vLyrs, strValue, _T(","));
		return true;
	}
	return false;
}

bool XMJGExportFontSizeConfig::GetNumberLayers(vector<CString> &vLyrs)
{
	vLyrs.clear();
	CString strField = _T("文字注记图层"), strValue;
	if(m_db.readCommonTable(strField, strValue))
	{
		AnalyzeData(vLyrs, strValue, _T(","));
		return true;
	}
	return false;
}

bool XMJGExportFontSizeConfig::GetAreaAndRemarkLayers(vector<CString> &vLyrs)
{
	vLyrs.clear();
	CString strField = _T("面积及备注图层"), strValue;
	if(m_db.readCommonTable(strField, strValue))
	{
		AnalyzeData(vLyrs, strValue, _T(","));
		return true;
	}
	return false;
}

bool XMJGExportFontSizeConfig::GetFontSize(XMJGExportFontSizeConfigType type, double &size)
{
	CString strFieldName, strValue;
	CString strLyrFieldName = GetLayerFieldName(type);
	strFieldName.Format(_T("1:%d%s字高"), m_scale, strLyrFieldName);
	if(m_db.readCommonTable(strFieldName, strValue))
	{
		size = _tcstod(strValue, 0);
		return true;
	}
	return false;
}

bool XMJGExportFontSizeConfig::GetLayers(XMJGExportFontSizeConfigType type, vector<CString> &vLyrs)
{
	CString strFieldName, strValue;
	CString strLyrFieldName = GetLayerFieldName(type);
	strFieldName.Format(_T("%s图层"), strLyrFieldName);
	if(m_db.readCommonTable(strFieldName, strValue))
	{
		AnalyzeData(vLyrs, strValue, _T(","));
		return true;
	}
	return false;
}

CString XMJGExportFontSizeConfig::GetLayerFieldName(XMJGExportFontSizeConfigType type)
{
	switch(type)
	{
		case Text:
			return _T("文字注记");
		case Number:
			return _T("数据注记");
		case AreaAndRemark:
			return _T("面积及备注");
		default:
			return _T("");
	}
}

bool XMJGExportLineWidthConfig::GetLayers(XMJGExportLineWidthConfigType type, vector<CString> &vLyrs)
{
	CString strFieldName, strValue;
	CString strLyrFieldName = GetLayerFieldName(type);
	strFieldName.Format(_T("%s图层"), strLyrFieldName);
	if(m_db.readCommonTable(strFieldName, strValue))
	{
		AnalyzeData(vLyrs, strValue, _T(","));
		return true;
	}
	return false;
}

bool XMJGExportLineWidthConfig::GetLineWidth(XMJGExportLineWidthConfigType type, double &width)
{
	CString strFieldName, strValue;
	CString strLyrFieldName = GetLayerFieldName(type);
	strFieldName.Format(_T("1:%d%s线宽"), m_scale, strLyrFieldName);
	if(m_db.readCommonTable(strFieldName, strValue))
	{
		width = _tcstod(strValue, 0);
		return true;
	}
	return false;
}

CString XMJGExportLineWidthConfig::GetLayerFieldName(XMJGExportLineWidthConfigType type)
{
	switch(type)
	{
		case QSX:
			return _T("权属线");
		default:
			return _T("");
	}
}

bool XMJGExportCompassConfig::GetScale(double &scale)
{
	CString strFieldName, strValue;
	strFieldName.Format(_T("1:%d指北针缩放比例"), m_scale);
	if(m_db.readCommonTable(strFieldName, strValue))
	{
		scale = _tcstod(strValue, 0);
		return true;
	}
	return false;
}
