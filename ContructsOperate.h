#pragma once
#include "apoint.hpp"
#include "SBreakBlock.h"
#include "Selection_String.h"
#include "Global.h"
#include "Dialogs.h"
#include "CassInfo.h"
//#include "DockbarWindow.h"
#include "TinyExcel.h"
#include "SDimensionOpe.h"
#include "RegionFitPt.h"
#include "ExtractTextdlg.h"

void SendCommand(const TCHAR *);

class CContructsOperate
{
public:
	CContructsOperate(void);
	virtual ~CContructsOperate(void);

	double			m_dhh;

	AcGePoint2d			m_ptNoteInsertPoint;

	bool		addAttributeInfo(ads_name ent);

	bool		addEntToLayer(ads_name ent);

	bool			readMustFillFieldFile();
	
	bool			checkMustFillFields();

	bool			putP();

	bool			dzpb();

	bool			checkUnCalulatedBuildings();

	void	insertAndChangeBaseMaps();

	bool		extractText();

	bool		scaleDimensionText();

	//绘制入户地物
	bool		drawThings();

	bool      writeThingsCodes(AcDbObjectId objLast);

	bool		extentBuildings();

	bool		noteCoord_Total_Floor_Plan();

	//室内坪注记
	bool		noteIndoorsHeight();

	//房屋面积注记
	bool		noteIndoorFrame();

	//附属物名字注记
	bool		noteAttachment();

	//围墙注记
	bool		noteFenceNote();

	//不上图的文字注记
	bool		noteTextNote();

	//不上图的文字注记_标准2
	bool		noteTextNote2();

	bool		getFitPoint(AcDbObjectId objBuild);

private:

	ads_real					m_dAngle;
	bool						m_bNote;
	CString					m_strNoteName;
	vector<DrawThingsData>		m_sDrawThings;

	vector<SMustFillField>		m_vMustFillField;


	bool		setNoteStyle(AcDbObjectId objId, const DrawThingsData &data);

	bool		readNoteTextConfig_Indoor();

	bool		findThingsInfo(const TCHAR *szCode, DrawThingsData &data);

	bool		findThingsInfo(const TCHAR *szCode);

	bool		findThingsInfo(const TCHAR *szCode, SMustFillField &FillField);

	bool		getNoteInsertPoint(AcDbObjectId objBuild);

	bool		getNoteInsertPoint_Line(AcDbObjectId objPolyLine);

	bool		getLastEntity_PolyLine(AcDbObjectId &objId);

	bool		noteBaseRoom(AcDbObjectId objId);

};

