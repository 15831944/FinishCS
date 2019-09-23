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

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月21号
	 */
	SYPSampleJJG();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月21号
	 */
	~SYPSampleJJG();
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月21号
	 */
	DrawType drawPoint(AcGePoint3d &, AcDbObjectId &);
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月21号
	 */
	DrawType selectPoint(AcGePoint3d &, AcDbObjectId &);

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月21号
	 */
	AcEdJig::DragStatus sampler();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月21号
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