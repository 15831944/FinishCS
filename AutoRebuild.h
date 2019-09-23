/**
 * @breif			实体批量切割，单个切割的类
 * @author			徐东江
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
		@brief			实体objDiv 切割实体objTB，返回切割出来的新实体ResTBArr
		@author		徐东江
		@date			2014-5-6
	*/
	void			divEntity(AcDbObjectId objTB, AcDbObjectId objDiv, AcDbObjectIdArray &ResTBArr);

private:



	int _CompareNode( AcGePoint2d pt1 , AcGePoint2d pt2 );

	bool _FindNode( AcGePoint2dArray & nodes , AcGePoint2d pt , int &nIdx );

};
