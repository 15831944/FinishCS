#pragma once
#include "dbmain.h"
#include "gemat3d.h"

//����ʵ����
//�������϶����ݿ����Ѿ����ڵ�ʵ�壬ͨ����һ��ѡ��ѡ���ʵ�弯
class CDatabaseJigEntity : public AcDbEntity
{
public:
	CDatabaseJigEntity(const AcDbObjectIdArray & ids) : m_Ids(ids) {}
	~CDatabaseJigEntity() {}
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
	void setXform(const AcGeMatrix3d & xform) { m_Xform = xform; }
	BOOL transform();
	BOOL transformedCopy();
private:
	AcDbObjectIdArray	m_Ids;	//���������϶������ID
	AcGeMatrix3d		m_Xform;//�任����
};



//�ƶ�ʵ����϶���
class CMoveJig : public AcEdJig
{
public:
	CMoveJig(const AcGePoint3d & fromPoint) : m_pEnt(NULL), m_FromPoint(fromPoint), m_ToPoint(fromPoint), m_isok(false){}
	~CMoveJig();
	void	doIt(const AcDbObjectIdArray & ids, bool bCopy = false);
	virtual DragStatus sampler();
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const;
	bool    isok() const;
private:
	CDatabaseJigEntity	*m_pEnt;
	AcGePoint3d			m_FromPoint;
	AcGePoint3d			m_ToPoint;
	AcGeMatrix3d		m_Xform;
	bool                m_isok;
};
