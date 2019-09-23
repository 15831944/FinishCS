#ifndef SYPSAMPLEJJG
#define SYPSAMPLEJJG

class SYPSampleJJG
	: public AcEdJig
{

public:

	enum DrawType
	{
		SelNull = -1,
		SelOk = 0,
		SelPoint = 1,
		SelEntity = 2
	};

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��21��
	 */
	SYPSampleJJG();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��21��
	 */
	~SYPSampleJJG();
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��21��
	 */
	DrawType drawPoint(AcGePoint3d &, AcDbObjectId &);
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��21��
	 */
	DrawType selectPoint(AcGePoint3d &, AcDbObjectId &);

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��21��
	 */
	AcEdJig::DragStatus sampler();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��21��
	 */
	Adesk::Boolean update();

	AcDbEntity *entity();

};

class SYPSelectEntity
	: public AcEditorReactor
{
public:

	SYPSelectEntity(const bool autoInitAndRelease = true);
	virtual ~SYPSelectEntity();
	virtual void commandWillStart(const TCHAR* cmdStr);
	virtual void commandEnded(const TCHAR* cmdStr);
	virtual void pickfirstModified();
private:

	bool m_autoInitAndRelease;
};

#endif /*SYPSAMPLEJJG*/