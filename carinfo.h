#ifndef CARINFO_H
#define CARINFO_H

#include "XMJGMacro.h"

class CCarInfo
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	CCarInfo();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	~CCarInfo();

	/* @�ӿ� ���Ƴ�λ
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void drawCar();

	/* @�ӿ� ���Ҫ���õ�ʵ��
	 * @���� AcDbObjectId �����õ�ʵ��id
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void setObject(const AcDbObjectId &);

	/* @�ӿ� ���Ҫ���õ�ʵ��
	 * @���� AcDbObjectIdArray �����õ�ʵ�弯��
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void setObject(const AcDbObjectIdArray &);

	/* @�ӿ� ��������
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void setProperty(const AcDbObjectId &);

	/* @�ӿ� ����Ĭ������
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void setDefaultProperty();

	/* @�ӿ� 
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void removeText(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void removeText(const AcDbObjectIdArray &);

	/* @�ӿ� ��ȡ��λ�������
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	int getCarSerialNum();

	/* @�ӿ� ��λ�ع�
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void dimCar();

	/* @�ӿ� ��ӳ�λע��
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void setCarAnnotation();

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��12��1��
	 */
	void setPropertys();

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��8��1��
	 */
	void setCarType();

private:

	int m_num;//���浱ǰ��λ��Ŀ
	double m_Scale;//���ݱ����ߵĲ�ͬ���õ�����ϵ��
	TextProperty m_cw;//��λ
	TextProperty m_dscwzj;//���ϳ�λע��
	TextProperty m_dscwbh;//���ϳ�λ���
	TextProperty m_dxcwzj;//���³�λע��
	TextProperty m_dxcwbh;//���³�λ���

};

#endif /*CARINFO_H*/