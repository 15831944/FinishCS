/**
 * @breif			ʵ�������и�����и����
 * @author			�춫��
 * @date			2014-5-6
 *
 */

#pragma once
#include "DivPoly.h"
#include "Global.h"
#include "AutoRegion.h"

class CAutoRebuild
{
public:
	CAutoRebuild(void);
	virtual ~CAutoRebuild(void);

	/**
		@brief			ʵ��objDiv �и�ʵ��objTB�������и��������ʵ��ResTBArr
		@author		�춫��
		@date			2014-5-6
	*/
	void			divEntity(AcDbObjectId objTB, AcDbObjectId objDiv, AcDbObjectIdArray &ResTBArr);

private:



	int _CompareNode( AcGePoint2d pt1 , AcGePoint2d pt2 );

	bool _FindNode( AcGePoint2dArray & nodes , AcGePoint2d pt , int &nIdx );

};
