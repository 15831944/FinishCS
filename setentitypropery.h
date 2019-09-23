#ifndef SETENTITYPROPERTY_H
#define SETENTITYPROPERTY_H

#include "Global.h"

class CSetEntityPropery
{
public:
	
	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��12��2��
	 */
	CSetEntityPropery();
	
	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @ʱ�� 2017��12��2��
	 */
	~CSetEntityPropery();

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��2��
	 */
	void setProjectLine(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��3��
	 */
	void setDXFWX(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��3��
	 */
	void setGNQ(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��3��
	 */
	void setBG(const AcDbObjectId &);

	/* @�ӿ� �������ű߽�����
	 * @���� AcDbObjectId ���ű߽�id
	 * @���� ���
	 * @ʱ�� 2018��1��4��
	 */
	void setBridge(const AcDbObjectId &);

	/* @�ӿ� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��18��
	 */
	void setRedLine(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��3��
	 */
	void setJGBuild(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��3��
	 */
	void setBuild(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��3��
	 */
	void setBuildBoundaries(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��25��
	 */
	void setExteriorWall(const AcDbObjectId &);

	/* @�ӿ� ������ǽ��
	 * @���� AcDbObjectId ��ǽ�ߵ�id
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��30��
	 */
	void setInnerWall(const AcDbObjectId &);

	/* @�ӿ� ��ȡʵ�����Ͻ�λ��
	 */
	AcGePoint3d getRectangleTopLeftPos(const AcDbObjectId &id);

private:

	int m_gnqbh;
	double m_scale;
	IDataBaseOper oper;

};

#endif /*SETENTITYPROPERTY_H*/