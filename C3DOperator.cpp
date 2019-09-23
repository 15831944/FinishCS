#include "stdafx.h"
#include "C3DOperator.h"
#include <dbsol3d.h>


C3DOperator::C3DOperator(void)
{
}

C3DOperator::~C3DOperator(void)
{
}

AcDbObjectId C3DOperator::HatchObject(const AcDbObjectId &ObjId, int nColorIndex, const TCHAR *szHatchName)
{
	AcDbObjectId HatchId = AcDbObjectId::kNull;

	AcDbHatch* pHatch = new AcDbHatch();

	AcGeVector3d normal(0.0, 0.0, 1.0);
	pHatch->setNormal(normal);
	pHatch->setElevation(0.0);
	pHatch->setHatchStyle(AcDbHatch::kNormal); //设置Hatch模式为预定义AR-CONC类型
	pHatch->setPattern(AcDbHatch::kPreDefined, szHatchName); //就是这里，预定义的类型设置为SOLID时填充能完成
	pHatch->setPatternScale(0.2);
	pHatch->setColorIndex(nColorIndex);
	//设置关联边界
	pHatch->setAssociative(Adesk::kTrue);

	AcGePoint2dArray node2d;
	AcGeDoubleArray bugles2d;
	ads_name ent;
	acdbGetAdsName(ent, ObjId);
	GetPlList(ent, node2d, true);
	GetPlBulges(ent, bugles2d, true);
	if(distOF2d(node2d.first(), node2d.last()) > EPS)
	{
		node2d.append(node2d.first());
		bugles2d.append(0.0);
	}
	Acad::ErrorStatus es;
	es = pHatch->appendLoop(AcDbHatch::kExternal, node2d, bugles2d);
	if(es != Acad::eOk)
	{
		delete pHatch;
		return HatchId;
	}
	AcDbBlockTable* pBlockTable;
	if(acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pBlockTable, AcDb::kForRead) != Acad::eOk)
	{
		delete pHatch;
		return HatchId;
	}
	AcDbBlockTableRecord* pBlockTableRecord;
	if(pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite) != Acad::eOk)
	{
		delete pHatch;
		pBlockTable->close();
		return HatchId;
	}
	pBlockTable->close();
	if(pBlockTableRecord->appendAcDbEntity(HatchId, pHatch) != Acad::eOk)
	{
		delete pHatch;
		pBlockTableRecord->close();
		return HatchId;
	}
	es = pHatch->evaluateHatch();
	pBlockTableRecord->close();
	pHatch->close();
	ads_name entHatch;
	acdbGetAdsName(entHatch, HatchId);
	ads_command(RTSTR, _T("DRAWORDER"), RTENAME, entHatch, RTSTR, _T(""), RTSTR, _T("B"), RTNONE);
	return HatchId;
}

AcDbObjectId C3DOperator::HatchObject(const AREAINFO & ObjId, const int nColorIndex, TCHAR *szHatchName)
{
	AcDbObjectId HatchId = AcDbObjectId::kNull;

	AcDbHatch* pHatch = new AcDbHatch();

	AcGeVector3d normal(0.0, 0.0, 1.0);
	pHatch->setNormal(normal);
	pHatch->setElevation(0.0);
	pHatch->setHatchStyle(AcDbHatch::kNormal); //设置Hatch模式为预定义AR-CONC类型
	pHatch->setPattern(AcDbHatch::kPreDefined, szHatchName); //就是这里，预定义的类型设置为SOLID时填充能完成
	pHatch->setPatternScale(0.2);
	pHatch->setColorIndex(nColorIndex);
	//设置关联边界
	pHatch->setAssociative(Adesk::kTrue);

	AcGePoint2dArray node2d;
	AcGeDoubleArray bugles2d;
	ads_name ent;
	acdbGetAdsName(ent, ObjId.IdFlood);
	GetPlList(ent, node2d, true);
	GetPlBulges(ent, bugles2d, true);
	if(distOF2d(node2d.first(), node2d.last()) > EPS)
	{
		node2d.append(node2d.first());
		bugles2d.append(0.0);
	}
	Acad::ErrorStatus es;
	es = pHatch->appendLoop(AcDbHatch::kExternal, node2d, bugles2d);
	AcDbObjectIdArray dbObjIds;
	for(int i = 0; i < ObjId.idSafe.length(); i++)
	{
		dbObjIds.setLogicalLength(0);
		dbObjIds.append(ObjId.idSafe.at(i));
		es = pHatch->appendLoop(AcDbHatch::kDefault, dbObjIds);
	}
	if(es != Acad::eOk)
	{
		delete pHatch;
		return HatchId;
	}
	AcDbBlockTable* pBlockTable;
	if(acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pBlockTable, AcDb::kForRead) != Acad::eOk)
	{
		delete pHatch;
		return HatchId;
	}
	AcDbBlockTableRecord* pBlockTableRecord;
	if(pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite) != Acad::eOk)
	{
		delete pHatch;
		pBlockTable->close();
		return HatchId;
	}
	pBlockTable->close(); 
	if(pBlockTableRecord->appendAcDbEntity(HatchId, pHatch) != Acad::eOk)
	{
		delete pHatch;
		pBlockTableRecord->close();
		return HatchId;
	}
	es = pHatch->evaluateHatch();
	pBlockTableRecord->close();
	pHatch->close();
	ads_name entHatch;
	acdbGetAdsName(entHatch, HatchId);
	ads_command(RTSTR, _T("DRAWORDER"), RTENAME, entHatch, RTSTR, _T(""), RTSTR, _T("B"), RTNONE);
	return HatchId;
}

bool C3DOperator::RegionBooleanOper(AcDbObjectId regionId1, AcDbObjectId regionId2, AcDb::BoolOperType operType)
{
	AcDbRegion* pRegion1, *pRegion2;
	if(Acad::eOk != acdbOpenObject(pRegion1, regionId1, AcDb::kForWrite)) return false;
	if(Acad::eOk != acdbOpenObject(pRegion2, regionId2, AcDb::kForWrite))
	{
		pRegion1->close();
		return false;
	}
	bool result = false;
	if(Acad::eOk == pRegion1->booleanOper(operType, pRegion2))
	{
		ads_name ent;
		acdbGetAdsName(ent, regionId2);
		ads_entdel(ent);
		result = true;
	}
	pRegion1->close();
	pRegion2->close();
	return result;
}

bool C3DOperator::SolidBooleanOper(AcDbObjectId solidId1, AcDbObjectId solidId2, AcDb::BoolOperType operType)
{
	AcDb3dSolid* pSolid1, *pSolid2;
	if(Acad::eOk != acdbOpenObject(pSolid1, solidId1, AcDb::kForWrite)) return false;
	if(Acad::eOk != acdbOpenObject(pSolid2, solidId2, AcDb::kForWrite))
	{
		pSolid1->close();
		return false;
	}
	bool result = false;
	if(Acad::eOk == pSolid1->booleanOper(operType, pSolid2))
	{
		result = true;
	}
	pSolid1->close();
	pSolid2->close();
	return result;
}

bool C3DOperator::SolidBooleanUnion(const AcDbObjectId mainId, const AcDbObjectIdArray& idArr, AcDbObjectIdArray &unSuccessIds)
{
	AcDb3dSolid* pSolid1, *pSolid2;
	if(Acad::eOk != acdbOpenObject(pSolid1, mainId, AcDb::kForWrite)) return false;
	int count = idArr.length();
	int i = 0;
	AcDbObjectIdArray successIds;
	for (i = 0; i < count; ++i)
	{
		Acad::ErrorStatus es = acdbOpenObject(pSolid2, idArr[i], AcDb::kForWrite);
		if(Acad::eOk != es)
		{
			continue;
		}
		if(Acad::eOk == pSolid1->booleanOper(AcDb::kBoolUnite, pSolid2))
		{
			successIds.append(idArr[i]);
		}
		else
		{
			unSuccessIds.append(idArr[i]);
		}
		pSolid2->close();
	}
	pSolid1->close();
	for (i = 0; i < successIds.length(); ++i)
	{
		ads_name ent;
		acdbGetAdsName(ent, successIds[i]);
		EntDel(ent);
	}
	return true;
}

AcDbObjectId C3DOperator::CreateRingRegion(const AcDbObjectId outId, const AcDbObjectIdArray inIdArr)
{
	AcDbObjectId outRegion = CreateRegion(outId);
	AcDbObjectIdArray inRegionArr = CreateRegion(inIdArr);
	AcDbRegion* pRegion1, *pRegion2;
	if(Acad::eOk != acdbOpenObject(pRegion1, outRegion, AcDb::kForWrite)) return AcDbObjectId::kNull;
	
	int i = 0;
	int count = inRegionArr.length();
	AcDbObjectIdArray successIds;
	for(i = 0; i < count; ++i)
	{
		if(Acad::eOk != acdbOpenObject(pRegion2, inRegionArr[i], AcDb::kForWrite))
		{
			continue;
		}
		Acad::ErrorStatus es = pRegion1->booleanOper(AcDb::kBoolSubtract, pRegion2);
		if(Acad::eOk == es) successIds.append(inRegionArr[i]);
		pRegion2->close();
	}
	pRegion1->close();
	for (i = 0; i < successIds.length(); ++i)
	{
		ads_name ent;
		acdbGetAdsName(ent, successIds[i]);
		ads_entdel(ent);
	}
	return outRegion;
}

AcDbObjectId PostToModelSpace(AcDbEntity* pEnt)
{
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlockTable, AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);

	AcDbObjectId entId;
	pBlockTableRecord->appendAcDbEntity(entId, pEnt);

	pBlockTable->close();
	pBlockTableRecord->close();
	pEnt->close();

	return entId;
}

AcDbObjectId C3DOperator::CreateSolid(const AcDbObjectId regionId, double height, bool bDelRegion)
{
	AcDbRegion *pRegion;
	acdbOpenObject(pRegion, regionId, AcDb::kForWrite);
	AcDb3dSolid* pSolid = new AcDb3dSolid();
	Acad::ErrorStatus es = pSolid->extrude(pRegion, height);
	AcDbObjectId solidId = PostToModelSpace(pSolid);
	pRegion->close();
	if (bDelRegion)
	{
		ads_name regionEnt;
		acdbGetAdsName(regionEnt, regionId);
		EntDel(regionEnt);
	}
	return solidId;
}