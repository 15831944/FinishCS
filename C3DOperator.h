#pragma once

struct AREAINFO
{
	AcDbObjectId IdFlood;//ˮ����
	AcDbObjectIdArray idSafe;//һ��ˮ�������ڲ���״��ȫ��
};

class C3DOperator
{
public:

	C3DOperator(void);

	~C3DOperator(void);
	
	/* @�ӿ� �������
	 * @���� AREAINFO ����������
	 * @���� int ������ɫ
	 * @���� TCHAR *��������
	 * @���� AcDbObjectId �������id
	 * @���� ���
	 * @ʱ�� 2017��12��11��
	 */
	static AcDbObjectId HatchObject(const AREAINFO &, int, TCHAR *);

	/* @�ӿ� �������
	 * @���� AcDbObjectId ����������
	 * @���� int ������ɫ
	 * @���� TCHAR *��������
	 * @���� AcDbObjectId �������id
	 * @���� ���
	 * @ʱ�� 2017��12��11��
	 */
	static AcDbObjectId HatchObject(const AcDbObjectId &, int, const TCHAR *);
	
	// ������״����
	static AcDbObjectId CreateRingRegion(const AcDbObjectId outId, const AcDbObjectIdArray inIdArr);
	// �����߼�����
	static bool RegionBooleanOper(AcDbObjectId regionId1, AcDbObjectId regionId2, AcDb::BoolOperType operType);
	// ��άʵ���߼�����
	static bool SolidBooleanOper(AcDbObjectId solidId1, AcDbObjectId solidId2, AcDb::BoolOperType operType);
	// ������άʵ��
	static AcDbObjectId CreateSolid(const AcDbObjectId regionId, double height, bool bDelRegion = true);
	// ��άʵ������ȡ��
	// mainIdΪ��ʵ�壬�ϲ������Ը�ʵ��Ϊ׼
	static bool SolidBooleanUnion(const AcDbObjectId mainId, const AcDbObjectIdArray& idArr, AcDbObjectIdArray &unSuccessIds);
};

