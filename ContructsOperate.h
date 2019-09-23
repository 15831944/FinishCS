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

	//�����뻧����
	bool		drawThings();

	bool      writeThingsCodes(AcDbObjectId objLast);

	bool		extentBuildings();

	bool		noteCoord_Total_Floor_Plan();

	//����ƺע��
	bool		noteIndoorsHeight();

	//�������ע��
	bool		noteIndoorFrame();

	//����������ע��
	bool		noteAttachment();

	//Χǽע��
	bool		noteFenceNote();

	//����ͼ������ע��
	bool		noteTextNote();

	//����ͼ������ע��_��׼2
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

