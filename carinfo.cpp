#include "stdafx.h"
#include "carinfo.h"

#include "Global.h"
#include "XRecordOperate.h"
#include "XMJGMacro.h"
#include "XMJGTool.h"
#include "XMJGDialog.h"

CCarInfo::CCarInfo()
{
	IDataBaseOper oper; m_Scale = 500;
	read1(m_Scale); m_Scale /= 1000;
	m_cw = oper.readTextPropery(_T("建筑竣工车位"));
	m_cwzj = oper.readTextPropery(_T("建筑竣工车位注记"), true, true);
	CXRecordOperate recordOperate;
	recordOperate.ReadIntegerRecord(_T("XMJG"), _T("CWBH"), m_num);
}


CCarInfo::~CCarInfo()
{
	CXRecordOperate recordOperate;
	recordOperate.AddIntegerRecord(_T("XMJG"), _T("CWBH"), m_num);
}

void CCarInfo::drawCar()
{
	AcDbObjectId id = drawLine(m_cw._layer, m_cw._color);
	if(id == AcDbObjectId::kNull) return; setProperty(id);
}

void CCarInfo::setObject(const AcDbObjectId &id)
{

}

void CCarInfo::setObject(const AcDbObjectIdArray &ids)
{

}

void CCarInfo::setProperty(const AcDbObjectId &id)
{
	TCHAR info[255] = {0}, bh[20] = {0};
	TCHAR type[20] = {0}, sm[255] = {0};
	if(false == ReadXdata(aname(id), _T("CWLX"), 0, type))
	{
		_stprintf(type, _T("%s"), _T("标准车位"));
		AddXdata(aname(id), _T("CWLX"), 0, type);
	}
	if(false == ReadXdata(aname(id), _T("CWBH"), 0, bh))
	{
		_stprintf(bh, _T("%d"), m_num++);
		AddXdata(aname(id), _T("CWBH"), 0, bh);
	}
	if(false == ReadXdata(aname(id), _T("CWSM"), 1, sm))
		AddXdata(aname(id), _T("CWSM"), 1, _T("1"));
	if(false == ReadXdata(aname(id), _T("SZLC"), 0, info))
		AddXdata(aname(id), _T("SZLC"), 0, _T("地上"));
	TCHAR handle[255] = {0}; AcGePoint3d pos; GetMiddlePoint(id, pos);
	AcGePoint2dArray node; CString strBH(bh);
	GetPlList(aname(id), node);
	//if(node.length() != 4) return;
	double width = fabs(node[0].x - node[2].x);
	double height = fabs(node[0].y - node[2].y);
	if(ReadXdata(aname(id), _T("CWBHJB"), 0, handle))
	{
		AcDbObjectId bhid = GetObjectIdByHandle(handle);
		SetEntText(aname(bhid), bh);
	}
	else
	{
		_stprintf(info, _T("%s"), bh); 
		if(width < height) pos.y -= 1.2; else pos.x += 1.2;
		AcDbObjectId cwtext2 = DrawText(pos, info, _T("MC"), _T(""), m_cwzj._size * m_Scale);
		setlayer(aname(cwtext2), m_cwzj._layer); setcolor(aname(cwtext2), m_cwzj._color);
		SetTextStyle(cwtext2, m_cwzj._font, 1.0);
		CString cwbhzj = GetHandleByObject(cwtext2);
		AddXdata(aname(id), _T("CWBHJB"), 0, cwbhzj);
	}
	CString str(type), cwzj;
	if(str.CompareNoCase(_T("标准车位")) == 0)
		cwzj.Format(_T("%s"), _T("车位"));
	else
		cwzj = str.Left(str.GetLength());
	if (ReadXdata(aname(id), _T("CWZJJB"), 0, handle))
	{
		AcDbObjectId zjid = GetObjectIdByHandle(handle);
		SetEntText(aname(zjid), cwzj);
	}
	else
	{
		if(width < height) pos.y += 2.4; else pos.x -= 2.4;
		AcDbObjectId cwtext = DrawText(pos, cwzj, _T("MC"), _T(""), m_cwzj._size * m_Scale);
		setlayer(aname(cwtext), m_cwzj._layer); setcolor(aname(cwtext), m_cwzj._color);
		SetTextStyle(cwtext, m_cwzj._font, 1.0);
		CString cwbhzj = GetHandleByObject(cwtext);
		AddXdata(aname(id), _T("CWZJJB"), 0, cwbhzj);
	}

	if(cwzj.CompareNoCase(_T("子母车位")) == 0 && strBH.FindOneOf(_T("-")) != -1)
		AddXdata(aname(id), _T("CWSM"), 1, _T("0"));

}

void CCarInfo::setDefaultProperty()
{

}

void CCarInfo::removeText(const AcDbObjectId &id)
{
	AcDbObjectIdArray ids = getInnerObjects(id);
	removeText(ids);
}

void CCarInfo::removeText(const AcDbObjectIdArray &ids)
{
	TCHAR type[255] = {0};
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		GetEntityType(aname(id), type);
		CString Type(type);
		if(Type.CompareNoCase(_T("Text")) != 0) continue;
		EntDel(aname(id));
	}
}

int CCarInfo::getCarSerialNum()
{
	int Serial = 0;
	CXRecordOperate recordOperate;
	if(recordOperate.ReadIntegerRecord(_T("XMJG"), _T("CWBZ"), Serial) == FALSE)
		Serial = 0;
	Serial++;
	return Serial;
}

void CCarInfo::dimCar()
{
	XMJGDimensions dim;	ads_point pt;
	if(RTNORM != ads_getpoint(NULL, _T("\n 请在需要构建车位的实体中选择一点："), pt)) return;
	dim.setEntityColor(m_cw._color);
	dim.setEntityLayer(m_cw._layer);
	dim.setPoint(apoint(pt));
	for(int idx = 1; idx < 10; ++idx)
	{
		dim.setSearchDist(2.5 * idx);
		if(true != dim.beginDimensions()) continue;
		AcDbObjectId carid = dim.getEntityId();
		AddXdata(aname(carid), _T("CWLX"), 0, _T("标准车位"));
		AddXdata(aname(carid), _T("CWSM"), 1, _T("1"));
		AddXdata(aname(carid), _T("SZLC"), 0, _T("地上"));
		AcGePoint3d pos; GetMiddlePoint(carid, pos);
		AcDbObjectId textid = DrawText(pos, _T("标准车位"), _T("MC"), _T(""), 1.5);
		setlayer(aname(textid), _T("车位标注")); setcolor(aname(textid), 4);
		acutPrintf(_T("\n 构建车位成功。。。"));
		acutPrintf(_T("\n 构面实体的范围是%.1f"), 2.5 * idx);
		return;
	}
	acutPrintf(_T("\n 构建车位失败！！！"));
}

void CCarInfo::setCarAnnotation()
{
	AcDbObjectIdArray ids; SelectFilter sf(8, m_cw._layer);
	if(false == SelectEntitys(ids, sf, _T(""))) return;
	int type = m_num; TCHAR bh[20] = {0};
	if(RTCAN == ads_getint(_T("\n 请输入车位编号："), &type)) return;
	MIId cw; MIIdIter it;
	for(int idx = 0; idx < ids.length(); ++idx)
	{
		AcDbObjectId id = ids.at(idx);
		ReadXdata(aname(id), _T("CWBH"), 0, bh);
		cw[_ttoi(bh)] = id;
	}
	for(it = cw.begin(); it != cw.end(); ++it)
	{
		AcDbObjectId id = it->second;
		_stprintf(bh, _T("%d"), type++);
		AddXdata(aname(id), _T("CWBH"), 0, bh);
		setProperty(id);
	}
	if(type < m_num)
	{
		TCHAR info[255] = {0}; int zj = 1;
		_stprintf(info, _T("下次注记是从%d开始注记还是从%d开始注记<1>%d / <2>%d："), type, m_num, type, m_num);
		ads_getint(info, &zj);	m_num = zj == 1 ? type : m_num;
	}
	else m_num = type;
}

void CCarInfo::setPropertys()
{
	SelectFilter sf(8, m_cw._layer);
	AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T(""))) return;
	CChangeCarPro dlg; dlg.setObjects(ids);	dlg.DoModal();
}

void CCarInfo::setCarType()
{
	SelectFilter sf(8, m_cw._layer);
	AcDbObjectIdArray ids;
	if(false == SelectEntitys(ids, sf, _T(""))) return;

}