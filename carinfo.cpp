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
	m_cw = oper.readTextPropery(_T("����������λ"));//��ȡCommon�����йؽ���������λ�й�����ֵ
	m_dscwzj = oper.readTextPropery(_T("�����������ϳ�λע��"), true, true);//��ȡCommon�����йؽ����������ϳ�λע���й�����ֵ
	m_dscwbh = oper.readTextPropery(_T("�����������ϳ�λ���"), true, true);//��ȡCommon�����йؽ����������ϳ�λ����й�����ֵ
	m_dxcwzj = oper.readTextPropery(_T("�����������³�λע��"), true, true);//��ȡCommon�����йؽ����������³�λע���й�����ֵ
	m_dxcwbh = oper.readTextPropery(_T("�����������³�λ���"), true, true);//��ȡCommon�����йؽ����������³�λ����й�����ֵ
	CXRecordOperate recordOperate;
	recordOperate.ReadIntegerRecord(_T("XMJG"), _T("CWBH"), m_num);//��õ�ǰ�ĳ�λ�����Ŀ
}


CCarInfo::~CCarInfo()
{
	CXRecordOperate recordOperate;
	recordOperate.AddIntegerRecord(_T("XMJG"), _T("CWBH"), m_num);
}

void CCarInfo::drawCar()
{
	AcDbObjectId id = drawLine(m_cw._layer, m_cw._color);//����λ������
	if(id == AcDbObjectId::kNull) return;//���������λ������ʧ�ܣ��򷵻ؿ�ID�ţ�Ȼ��ֱ�ӷ���
	setProperty(id);
}

void CCarInfo::setObject(const AcDbObjectId &id)
{

}

void CCarInfo::setObject(const AcDbObjectIdArray &ids)
{

}

void CCarInfo::setProperty(const AcDbObjectId &id)
{//Ĭ��Ϊ���³�λ
	TCHAR info[255] = {0}, bh[20] = {0};
	TCHAR type[20] = {0}, sm[255] = {0};
	if(false == ReadXdata(aname(id), _T("CWLX"), 0, type))//�ж��Ƿ���ڳ�λ��������
	{
		_stprintf(type, _T("%s"), _T("��׼��λ"));
		AddXdata(aname(id), _T("CWLX"), 0, type);//��������ڳ�λ�������ԣ�����ӣ���������Ϊ��׼��λ
	}
	if (false == ReadXdata(aname(id), _T("CWBH"), 0, bh))//�ж��Ƿ���ڳ�λ�������
	{
		_stprintf(bh, _T("%d"), m_num++);
		//��������ڳ�λ������ԣ�����ӣ�
		//��������Ϊ��ǰ��λ�����Ŀ�����ҽ���ǰ��λ��Ŀ��1����λ��ŵ���ʼֵ��0��
		AddXdata(aname(id), _T("CWBH"), 0, bh);
	}
	if(false == ReadXdata(aname(id), _T("CWSM"), 1, sm))//�жϳ�λʵ�����Ƿ���ڳ�λ��Ŀ����
		AddXdata(aname(id), _T("CWSM"), 1, _T("1"));//��������ڣ�����ӣ���������Ϊ"1"��Ĭ�ϲ����ӳ�λ���ӳ�λ��λ��ĿΪ0��
	if(false == ReadXdata(aname(id), _T("SZLC"), 0, info))//�жϳ�λʵ�����Ƿ����SZLC����
		AddXdata(aname(id), _T("SZLC"), 0, _T("����"));//��������ڣ�����ӣ���������Ϊ"����"
	if (_tcscmp(info, _T("����")) == 0)setcolor(aname(id), m_dxcwbh._color);//����ǵ��³�λ�����ó�λ��������ɫΪ��ɫ
	TCHAR handle[255] = {0}; AcGePoint3d pos; 
	GetMiddlePoint(id, pos);//��ó�λ��С������ε��е�
	AcGePoint2dArray node; 
	CString strBH(bh);//ʹ��CString���泵λ���
	GetPlList(aname(id), node);//��ó�λ�����ߵ����нڵ�
	//if(node.length() != 4) return;
	double width = fabs(node[0].x - node[2].x);
	double height = fabs(node[0].y - node[2].y);
	if(ReadXdata(aname(id), _T("CWBHJB"), 0, handle))//��ȡ��λ�еĳ�λ���ʵ����
	{
		AcDbObjectId bhid = GetObjectIdByHandle(handle);//ͨ�������ó�λ���ʵ���ID��
		SetEntText(aname(bhid), bh);//�����λ��Ŵ��ڣ����������ó�λ���ʵ����ı�Ϊ�����еı�ţ�
									//�����λ��Ų����ڣ������ó�λ���ʵ����ı�Ϊ��ǰ��λ�����Ŀ
	}
	else
	{
		//_stprintf(info, _T("%s"), bh); 
		if(width < height) pos.y -= 1.2; else pos.x += 1.2;//���ó�λ��ŵĲ���λ��
		AcDbObjectId cwtext2 = DrawText(pos, bh, _T("MC"), _T(""), m_dscwbh._size * m_Scale);//���복λ���
		setlayer(aname(cwtext2), m_dscwbh._layer);//���ó�λ��ŵ�ͼ��
		setcolor(aname(cwtext2), m_dscwbh._color);//���ó�λ��ŵ���ɫ
		SetTextStyle(cwtext2, m_dscwbh._font, 1.0);//���ó�λ��ŵ�����
		CString cwbhjb = GetHandleByObject(cwtext2);//��ó�λ��ŵ�ʵ����
		AddXdata(aname(id), _T("CWBHJB"), 0, cwbhjb);//����λ��ŵ�ʵ��������ʵ��XData��
	}
	CString str(type), cwzj;
	if(str.CompareNoCase(_T("��׼��λ")) == 0)//�����λ����Ϊ��׼��λ
		cwzj.Format(_T("%s"), _T("��λ"));//����λע������Ϊ��λ
	else//�����λ���Ͳ��Ǳ�׼��λ
		cwzj = str.Left(str.GetLength());//����λע������Ϊ��ȡ����ԭ�г�λ����
	if (ReadXdata(aname(id), _T("CWZJJB"), 0, handle))//��ȡ��λע�Ǿ��
	{//�����ȡ�ɹ��������ó�λע�ǵ��ı�����
		AcDbObjectId zjid = GetObjectIdByHandle(handle);
		SetEntText(aname(zjid), cwzj);//���ó�λע�ǵ��ı�����Ϊcwzj
	}
	else//�����ȡʧ�ܣ����½���λע��
	{
		if(width < height) pos.y += 2.4; else pos.x -= 2.4;//���ó�λע�ǵĲ���λ��
		AcDbObjectId cwtext = DrawText(pos, cwzj, _T("MC"), _T(""), m_dscwzj._size * m_Scale);//���복λע��
		setlayer(aname(cwtext), m_dscwzj._layer);//���ó�λע�ǵ�ͼ��
		setcolor(aname(cwtext), m_dscwzj._color);//���ó�λע�ǵ���ɫ
		SetTextStyle(cwtext, m_dscwzj._font, 1.0);//���ó�λע�ǵ�����
		CString cwjb = GetHandleByObject(cwtext);//��ó�λע�ǵ�ʵ����
		AddXdata(aname(id), _T("CWZJJB"), 0, cwjb);//����λע�ǵ�ʵ�������복λXData��
	}

	if(cwzj.CompareNoCase(_T("��ĸ��λ")) == 0 && strBH.FindOneOf(_T("-")) != -1)//�ж��Ƿ�Ϊ��ĸ��λ�е��ӳ�λ
		AddXdata(aname(id), _T("CWSM"), 1, _T("0"));//�����ӳ�λ�ĳ�λ��ĿΪ0

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
	if(RTNORM != ads_getpoint(NULL, _T("\n ������Ҫ������λ��ʵ����ѡ��һ�㣺"), pt)) return;
	dim.setEntityColor(m_cw._color);
	dim.setEntityLayer(m_cw._layer);
	dim.setPoint(apoint(pt));
	for(int idx = 1; idx < 10; ++idx)
	{
		dim.setSearchDist(2.5 * idx);
		if(true != dim.beginDimensions()) continue;
		AcDbObjectId carid = dim.getEntityId();
		AddXdata(aname(carid), _T("CWLX"), 0, _T("��׼��λ"));
		AddXdata(aname(carid), _T("CWSM"), 1, _T("1"));
		AddXdata(aname(carid), _T("SZLC"), 0, _T("����"));
		AcGePoint3d pos; GetMiddlePoint(carid, pos);
		AcDbObjectId textid = DrawText(pos, _T("��׼��λ"), _T("MC"), _T(""), 1.5);
		setlayer(aname(textid), _T("��λ��ע")); setcolor(aname(textid), 4);
		acutPrintf(_T("\n ������λ�ɹ�������"));
		acutPrintf(_T("\n ����ʵ��ķ�Χ��%.1f"), 2.5 * idx);
		return;
	}
	acutPrintf(_T("\n ������λʧ�ܣ�����"));
}

void CCarInfo::setCarAnnotation()
{
	AcDbObjectIdArray ids; SelectFilter sf(8, m_cw._layer);
	if(false == SelectEntitys(ids, sf, _T(""))) return;
	int type = m_num; TCHAR bh[20] = {0};
	if(RTCAN == ads_getint(_T("\n �����복λ��ţ�"), &type)) return;
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
		_stprintf(info, _T("�´�ע���Ǵ�%d��ʼע�ǻ��Ǵ�%d��ʼע��<1>%d / <2>%d��"), type, m_num, type, m_num);
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