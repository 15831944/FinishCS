#include "StdAfx.h"
#include "MoveJig.h"

Adesk::Boolean CDatabaseJigEntity::subWorldDraw(AcGiWorldDraw *mode)
{
	//����ط��ǹؼ���
	mode->geometry().pushModelTransform(m_Xform);
	AcDbEntity* pEnt;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		//����ʵ������д��
		if (Acad::eOk == acdbOpenObject(pEnt, m_Ids[i], AcDb::kForRead))
		{
			mode->geometry().draw(pEnt);
			pEnt->close();
		}
	}
	//��䲻���٣��ָ��ֳ�
	mode->geometry().popModelTransform();
	return (AcDbEntity::subWorldDraw(mode));
}
//�����϶������󣬽�ʵ��任����λ��
BOOL CDatabaseJigEntity::transform()
{
	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt;
	AcDbObject *pObj;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForWrite))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		pEnt->transformBy(m_Xform);
	}
	acTransactionManagerPtr()->endTransaction();
	return TRUE;
}
//�����϶������󣬽�ʵ�帴�Ƶ���λ��
BOOL CDatabaseJigEntity::transformedCopy()
{
	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt;
	AcDbEntity *pNewEnt;
	AcDbObject *pObj;
	AcDbBlockTableRecord *pBlkRec;
	AcDbObjectId blkRecId;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForRead))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		if (0 == i)
		{
			blkRecId = pEnt->blockId();
			if (Acad::eOk != pTrans->getObject(pObj, blkRecId, AcDb::kForWrite))
			{
				acTransactionManagerPtr()->abortTransaction();
				return FALSE;
			}
			pBlkRec = AcDbBlockTableRecord::cast(pObj);
		}
		pEnt->getTransformedCopy(m_Xform, pNewEnt);
		pBlkRec->appendAcDbEntity(pNewEnt);
		acTransactionManagerPtr()->addNewlyCreatedDBRObject(pNewEnt);
	}
	acTransactionManagerPtr()->endTransaction();
	return TRUE;
}



CMoveJig::~CMoveJig()
{
	if (NULL != m_pEnt)
	{
		delete m_pEnt;
		m_pEnt = NULL;
	}
}

AcEdJig::DragStatus CMoveJig::sampler()
{
	DragStatus stat;
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
			| AcEdJig::kNoNegativeResponseAccepted
			| AcEdJig::kNoZeroResponseAccepted
			| AcEdJig::kNullResponseAccepted));
	static AcGePoint3d pointTemp;
	stat = acquirePoint(m_ToPoint);
	if (pointTemp != m_ToPoint)
		pointTemp = m_ToPoint;
	else if (stat == AcEdJig::kNormal)
		stat = AcEdJig::kNoChange;
	return stat;
}

Adesk::Boolean CMoveJig::update()
{
	m_Xform.setToTranslation(m_ToPoint - m_FromPoint);
	m_pEnt->setXform(m_Xform);
	return Adesk::kTrue;
}

AcDbEntity * CMoveJig::entity() const
{
	return m_pEnt;
}

bool CMoveJig::isok() const
{
	return m_isok;
}

void CMoveJig::doIt(const AcDbObjectIdArray &ids, bool bCopy /*= false*/)
{
	if (0 == ids.length())
	{
		return;
	}
	if (NULL != m_pEnt)
	{
		delete m_pEnt;
		m_pEnt = NULL;
	}

	m_pEnt = new CDatabaseJigEntity(ids);
	setDispPrompt(_T("\n��ʰȡ��ע��λ�ã�"));
	if (AcEdJig::kNormal == drag())
	{
		if (bCopy)
		{
			m_pEnt->transformedCopy();
		}
		else
		{
			m_pEnt->transform();
		}
		m_isok = true;
	}
	else
		m_isok = false;
	delete m_pEnt;
	m_pEnt = NULL;
}