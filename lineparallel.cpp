#include "stdafx.h"
#include "lineparallel.h"


LineParallel::LineParallel()
{
}


LineParallel::~LineParallel()
{
}

AcDbObjectIdArray LineParallel::getExplodes(const AcDbObjectId &id)
{
	ads_name last1, last2, ent, next;
	ads_entlast(last1); acdbGetAdsName(ent, id);
	ads_command(RTSTR, _T("EXPLODE"), RTENAME, ent, RTNONE);
	ads_entlast(last2); ads_name_set(last1, next);
	AcDbObjectIdArray ids;
	while(!ads_name_equal(next, last2))
	{
		ads_name temp;	ads_entnext(next, temp);
		AcDbObjectId tid; acdbGetObjectId(tid, temp);
		ids.append(tid); ads_name_set(temp, next);
	}
	return ids;
}

AcDbObjectId LineParallel::polyOffset(const AcDbObjectId &id, double off)
{
	AcDbObjectIdArray ids = getExplodes(id);
	AcDbObjectIdArray newids;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcGePoint2dArray node, newNode;
		GetPlList(aname(ids[idx]), node);
		AcGePoint2d spt, ept, pt;
		spt = node.first(); ept = node.last();
		if(spt.x == ept.x)
		{
			if(spt.y > ept.y)
			{
				pt = ept;
				ept = spt;
				spt = pt;
			}
			pt = spt; pt.x -= off; pt.y -= off; newNode.append(pt);
			pt = ept; pt.x -= off; pt.y += off; newNode.append(pt);
		}
		if(spt.y == ept.y)
		{
			if(spt.x > ept.x)
			{
				pt = ept;
				ept = spt;
				spt = pt;
			}
			pt = spt; pt.y -= off; pt.x -= off; newNode.append(pt);
			pt = ept; pt.y += off; pt.x += off; newNode.append(pt);
		}
		
		AcDbObjectId newid = DrawPolyline2D(newNode, 0.0, false, _T(""));
		newids.append(newid);
	}
	ads_name last, ent, ssName; ads_entlast(last);
	IdArrToSS(newids, ssName);
	ads_command(RTSTR, _T("PEDIT"), RTSTR, _T("M"), RTPICKS, ssName, RTSTR, _T(""), RTSTR, _T("Y"), RTSTR, _T("J"), RTSTR, _T(""), RTSTR, _T(""), RTNONE);
	ads_entlast(ent); return AcDbObjectId::kNull;
}