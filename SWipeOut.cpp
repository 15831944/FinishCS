#include "stdafx.h"
#include "SWipeOut.h"

AcDbObjectId		SWipeOut::WipeOut(AcDbObjectId objId, bool bDelete)
{

	AcGePoint2dArray nodes;
	if(GetPlList(aname(objId), nodes) == false) return AcDbObjectId::kNull;

	AcDbObjectId objWipeOut = WipeOut(nodes);

	if(bDelete) EntDel(aname(objId));

	return objWipeOut;

}


AcDbObjectId		SWipeOut::WipeOut(AcGePoint2dArray node)
{
	if(node.length() != 4)return AcDbObjectId::kNull;
	AcGePoint3d pt3d;
	AcGePoint3dArray pt3dArr;

	ads_point pt1 = {node.at(0).x,node.at(0).y,0}, pt2={node.at(1).x,node.at(1).y,0}, pt3 = {node.at(2).x,node.at(2).y,0}, pt4 = {node.at(3).x,node.at(3).y,0};

	pt3d.set(pt1[X], pt1[Y],0.0);
	pt3dArr.append(pt3d);

	pt3d.set(pt2[X], pt2[Y],0.0);
	pt3dArr.append(pt3d);

	pt3d.set(pt3[X], pt3[Y],0.0);
	pt3dArr.append(pt3d);

	pt3d.set(pt4[X], pt4[Y],0.0);
	pt3dArr.append(pt3d);

	pt3d.set(pt1[X], pt1[Y],0.0);
	pt3dArr.append(pt3d);

	// TODO: Implement the command

	AcRxClass* pClass = AcRxClass::cast(acrxClassDictionary->at(_T("AcDbWipeout"))); 
	if (pClass == NULL) return AcDbObjectId::kNull;
	AcDbEntity *pWipeOut=(AcDbEntity *)pClass->create(); 

	AcDbRasterImage *pImage = AcDbRasterImage::cast(pWipeOut);
	if (pImage == NULL) return AcDbObjectId::kNull;

	// Set clip boundary
	AcGePoint3d origin;
	origin.set(pt1[X], pt1[Y],0.0);

	AcGeVector3d u(pt2[X]-pt1[X],pt2[Y]-pt1[Y],0);
	AcGeVector3d v(pt4[X]-pt1[X],pt4[Y]-pt1[Y],0);

	//pImage->getOrientation(origin, u, v); 
	Acad::ErrorStatus es ;
	pImage->setOrientation(origin, u, v); 
	es = pImage->setClipBoundaryToWholeImage(); 

	AcGeMatrix3d PixelToModel,ModelToPixel;
	es = pImage->getPixelToModelTransform(PixelToModel);
	ModelToPixel=PixelToModel.invert();
	int i = 0;
	for(i=0;i< pt3dArr.length();i++)
	{
		pt3dArr[i].transformBy(ModelToPixel);
	}

	AcGePoint2dArray pt2dArr;
	for(i = 0;i < pt3dArr.length();i++)
	{
		pt2dArr.append(AcGePoint2d(pt3dArr.at(i).x,pt3dArr.at(i).y));
	}


	es = pImage->setClipBoundary(AcDbRasterImage::kPoly,pt2dArr);

	pImage->setDisplayOpt(AcDbRasterImage::kTransparent,Adesk::kTrue);
	pImage->setDisplayOpt(AcDbRasterImage::kShow,Adesk::kTrue );

	pWipeOut=(AcDbEntity *)pImage;

	AcDbObjectId imageId;
	AppendEntity(pImage, imageId);
	pImage->close();
	pWipeOut->close();
	return imageId;

}


SWipeOut::SWipeOut(void)
{
}


SWipeOut::~SWipeOut(void)
{
}
