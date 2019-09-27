#ifndef DOMAINOPERATE_H
#define DOMAINOPERATE_H

struct tagTextProperty;
typedef tagTextProperty TextProperty;

class DomainOperate
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	DomainOperate();
	
	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	~DomainOperate();

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	bool createDomainByDraw(AcDbObjectId &);

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	bool createDomainByPointSel(AcDbObjectId &);

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	bool createDomainByBoundary(AcDbObjectId &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	void addDomainProperty(const AcDbObjectId &, const CString &, const CString &, const CString &, const CString &, const CString &, bool);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	AcDbObjectId getBuildLayerRangeEntity(const AcDbObjectId &);

private:

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	void setDomainProperty();

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��20��
	 */
	void addDomainAnnotate(const AcDbObjectId &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��21��
	 */
	AcDbObjectId getBuildLayerEntity(const AcGePoint2d &);

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��9��21��
	 */
	void addXdataProperty(const AcDbObjectId &, const CString &, const CString &, const CString &, const CString &, const CString &, bool);

private:

	TextProperty m_gnq;				//������
	TextProperty m_fcpm;			//�ֲ㷶Χ��

};

#endif /*DOMAINOPERATE_H*/