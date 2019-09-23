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
	m_cw = oper.readTextPropery(_T("建筑竣工车位"));//读取Common表中有关建筑竣工车位有关属性值
	m_dscwzj = oper.readTextPropery(_T("建筑竣工地上车位注记"), true, true);//读取Common表中有关建筑竣工地上车位注记有关属性值
	m_dscwbh = oper.readTextPropery(_T("建筑竣工地上车位编号"), true, true);//读取Common表中有关建筑竣工地上车位编号有关属性值
	m_dxcwzj = oper.readTextPropery(_T("建筑竣工地下车位注记"), true, true);//读取Common表中有关建筑竣工地下车位注记有关属性值
	m_dxcwbh = oper.readTextPropery(_T("建筑竣工地下车位编号"), true, true);//读取Common表中有关建筑竣工地下车位编号有关属性值
	CXRecordOperate recordOperate;
	recordOperate.ReadIntegerRecord(_T("XMJG"), _T("CWBH"), m_num);//获得当前的车位编号数目
}


CCarInfo::~CCarInfo()
{
	CXRecordOperate recordOperate;
	recordOperate.AddIntegerRecord(_T("XMJG"), _T("CWBH"), m_num);
}

void CCarInfo::drawCar()
{
	AcDbObjectId id = drawLine(m_cw._layer, m_cw._color);//画车位轮廓线
	if(id == AcDbObjectId::kNull) return;//如果创建车位轮廓线失败，则返回空ID号，然后直接返回
	setProperty(id);
}

void CCarInfo::setObject(const AcDbObjectId &id)
{

}

void CCarInfo::setObject(const AcDbObjectIdArray &ids)
{

}

void CCarInfo::setProperty(const AcDbObjectId &id)
{//默认为地下车位
	TCHAR info[255] = {0}, bh[20] = {0};
	TCHAR type[20] = {0}, sm[255] = {0};
	if(false == ReadXdata(aname(id), _T("CWLX"), 0, type))//判断是否存在车位类型属性
	{
		_stprintf(type, _T("%s"), _T("标准车位"));
		AddXdata(aname(id), _T("CWLX"), 0, type);//如果不存在车位类型属性，则添加，并且设置为标准车位
	}
	if (false == ReadXdata(aname(id), _T("CWBH"), 0, bh))//判断是否存在车位编号属性
	{
		_stprintf(bh, _T("%d"), m_num++);
		//如果不存在车位编号属性，则添加，
		//并且设置为当前车位编号数目，并且将当前车位数目加1（车位编号的起始值是0）
		AddXdata(aname(id), _T("CWBH"), 0, bh);
	}
	if(false == ReadXdata(aname(id), _T("CWSM"), 1, sm))//判断车位实体中是否存在车位数目属性
		AddXdata(aname(id), _T("CWSM"), 1, _T("1"));//如果不存在，则添加，并且设置为"1"，默认不是子车位（子车位车位数目为0）
	if(false == ReadXdata(aname(id), _T("SZLC"), 0, info))//判断车位实体中是否存在SZLC属性
		AddXdata(aname(id), _T("SZLC"), 0, _T("地上"));//如果不存在，则添加，并且设置为"地上"
	if (_tcscmp(info, _T("地下")) == 0)setcolor(aname(id), m_dxcwbh._color);//如果是地下车位，设置车位轮廓线颜色为青色
	TCHAR handle[255] = {0}; AcGePoint3d pos; 
	GetMiddlePoint(id, pos);//获得车位最小外包矩形的中点
	AcGePoint2dArray node; 
	CString strBH(bh);//使用CString储存车位编号
	GetPlList(aname(id), node);//获得车位轮廓线的所有节点
	//if(node.length() != 4) return;
	double width = fabs(node[0].x - node[2].x);
	double height = fabs(node[0].y - node[2].y);
	if(ReadXdata(aname(id), _T("CWBHJB"), 0, handle))//读取车位中的车位编号实体句柄
	{
		AcDbObjectId bhid = GetObjectIdByHandle(handle);//通过句柄获得车位编号实体的ID号
		SetEntText(aname(bhid), bh);//如果车位编号存在，则重新设置车位编号实体的文本为属性中的编号；
									//如果车位编号不存在，则设置车位编号实体的文本为当前车位编号数目
	}
	else
	{
		//_stprintf(info, _T("%s"), bh); 
		if(width < height) pos.y -= 1.2; else pos.x += 1.2;//设置车位编号的插入位置
		AcDbObjectId cwtext2 = DrawText(pos, bh, _T("MC"), _T(""), m_dscwbh._size * m_Scale);//插入车位编号
		setlayer(aname(cwtext2), m_dscwbh._layer);//设置车位编号的图层
		setcolor(aname(cwtext2), m_dscwbh._color);//设置车位编号的颜色
		SetTextStyle(cwtext2, m_dscwbh._font, 1.0);//设置车位编号的字体
		CString cwbhjb = GetHandleByObject(cwtext2);//获得车位编号的实体句柄
		AddXdata(aname(id), _T("CWBHJB"), 0, cwbhjb);//将车位编号的实体句柄加入实体XData中
	}
	CString str(type), cwzj;
	if(str.CompareNoCase(_T("标准车位")) == 0)//如果车位类型为标准车位
		cwzj.Format(_T("%s"), _T("车位"));//将车位注记设置为车位
	else//如果车位类型不是标准车位
		cwzj = str.Left(str.GetLength());//将车位注记设置为读取到的原有车位类型
	if (ReadXdata(aname(id), _T("CWZJJB"), 0, handle))//读取车位注记句柄
	{//如果读取成功，则设置车位注记的文本内容
		AcDbObjectId zjid = GetObjectIdByHandle(handle);
		SetEntText(aname(zjid), cwzj);//设置车位注记的文本内容为cwzj
	}
	else//如果读取失败，则新建车位注记
	{
		if(width < height) pos.y += 2.4; else pos.x -= 2.4;//设置车位注记的插入位置
		AcDbObjectId cwtext = DrawText(pos, cwzj, _T("MC"), _T(""), m_dscwzj._size * m_Scale);//插入车位注记
		setlayer(aname(cwtext), m_dscwzj._layer);//设置车位注记的图层
		setcolor(aname(cwtext), m_dscwzj._color);//设置车位注记的颜色
		SetTextStyle(cwtext, m_dscwzj._font, 1.0);//设置车位注记的字体
		CString cwjb = GetHandleByObject(cwtext);//获得车位注记的实体句柄
		AddXdata(aname(id), _T("CWZJJB"), 0, cwjb);//将车位注记的实体句柄加入车位XData中
	}

	if(cwzj.CompareNoCase(_T("子母车位")) == 0 && strBH.FindOneOf(_T("-")) != -1)//判断是否为子母车位中的子车位
		AddXdata(aname(id), _T("CWSM"), 1, _T("0"));//设置子车位的车位数目为0

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