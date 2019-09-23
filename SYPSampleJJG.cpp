#include "StdAfx.h"

#ifndef SYPSAMPLEJJG
#include "SYPSampleJJG.h"
#endif /*SYPSAMPLEJJG*/
#include "XMJGDialog.h"
#include "Global.h"
extern bool g_pipelinedlg;

extern CPipeLinePropertyDlg g_pipelinepropertydlg;

SYPSampleJJG::SYPSampleJJG()
: AcEdJig()
{

}

SYPSampleJJG::~SYPSampleJJG()
{

}

AcEdJig::DragStatus SYPSampleJJG::sampler()
{
	return kNormal;
}

Adesk::Boolean SYPSampleJJG::update()
{
	return Adesk::kTrue;
}

AcDbEntity *SYPSampleJJG::entity()
{
	return NULL;
}

SYPSampleJJG::DrawType SYPSampleJJG::drawPoint(AcGePoint3d &pos, AcDbObjectId &id)
{
	setSpecialCursorType(AcEdJig::kCrosshair);
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoZeroResponseAccepted
		| AcEdJig::kAcceptOtherInputString
		| AcEdJig::kNoNegativeResponseAccepted));
	acutPrintf(_T("\n1:选择管线;2:连接管点："));
	setKeywordList(_T("1 2"));
	DragStatus ds = acquirePoint(pos);
	TCHAR info[20] = {0}; ads_point pt;
	int type = 0;
	switch(ds)
	{
		SetTextAlign(id, _T("MC"));
		case AcEdJig::kKW1:
			ads_name ent;
			if(RTNORM != ads_entsel(_T(""), ent, pt)) return SelNull;
			acdbGetObjectId(id, ent); type = 1;
		case AcEdJig::kKW2:
				if(RTNORM != ads_getpoint(NULL, _T(""), pt)) return SelNull;
				pos.x = pt[0]; pos.y = pt[1];  return type == 1 ? SelEntity : SelPoint;
		case  AcEdJig::kNormal:
			return SelOk;
		default: return SelNull;
	}
}

SYPSampleJJG::DrawType SYPSampleJJG::selectPoint(AcGePoint3d &pos, AcDbObjectId &id)
{
	setSpecialCursorType(AcEdJig::kCrosshair);
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoZeroResponseAccepted
		| AcEdJig::kAcceptOtherInputString
		| AcEdJig::kNoNegativeResponseAccepted));
	acutPrintf(_T("\n1:选择管点:"));
	setKeywordList(_T("1"));
	DragStatus ds = acquirePoint(pos);
	TCHAR info[20] = {0}; ads_point pt;
	int type = 0; id = AcDbObjectId::kNull;
	switch(ds)
	{
		case AcEdJig::kKW1:
			ads_name ent;
			if(RTNORM != ads_entsel(_T(""), ent, pt)) return SelNull;
			acdbGetObjectId(id, ent); type = 1;
			GetMiddlePoint(id, pos);  return SelEntity;
		case AcEdJig::kNormal: return SelOk;
		default: return SelNull;
	}
	return SelOk;
}

SYPSelectEntity::SYPSelectEntity(const bool autoInitAndRelease /* = true */)
{
	m_autoInitAndRelease = autoInitAndRelease;
	if(m_autoInitAndRelease)
		if(NULL != acedEditor)
			acedEditor->addReactor(this);
		else
			m_autoInitAndRelease = false;
}

SYPSelectEntity::~SYPSelectEntity()
{
	if(m_autoInitAndRelease)
		if(NULL != acedEditor)
			acedEditor->removeReactor(this);
}

void SYPSelectEntity::commandEnded(const TCHAR* cmdStr)
{

}

void SYPSelectEntity::commandWillStart(const TCHAR* cmdStr)
{

}

void SYPSelectEntity::pickfirstModified()
{
	if(g_pipelinedlg == false) return;
	struct resbuf *gset = NULL;
	struct resbuf *pset = NULL;
	TCHAR szCode[50] = {0};	LONG lLength;
	ads_name ent; AcDbObjectIdArray ids;
	acDocManagerPtr()->lockDocument(acDocManagerPtr()->curDocument());
	if(acedSSGetFirst(&gset, &pset) != RTNORM)
	{
		acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
		return;
	}

	if(pset != NULL)
	{
		if(acedSSLength(pset->resval.rlname, &lLength) == RTNORM)
		{
			for(LONG l = 0; l < lLength; l++)
			{
				if(acedSSName(pset->resval.rlname, l, ent) != RTNORM)
					continue;
				AcDbObjectId id;
				acdbGetObjectId(id, ent);
				ids.append(id);
			}
		}
	}
	g_pipelinepropertydlg.m_pdlg.selectPoint(ids);
	g_pipelinepropertydlg.m_ldlg.selectPoint(ids);
	acedSSFree(gset->resval.rlname);
	acedSSFree(pset->resval.rlname);
	acutRelRb(gset);
	acutRelRb(pset);
	gset = NULL;
	pset = NULL;

	acDocManagerPtr()->unlockDocument(acDocManagerPtr()->curDocument());
}