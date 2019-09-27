#ifndef DOMAINOPERATE_H
#define DOMAINOPERATE_H

struct tagTextProperty;
typedef tagTextProperty TextProperty;

class DomainOperate
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	DomainOperate();
	
	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	~DomainOperate();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	bool createDomainByDraw(AcDbObjectId &);

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	bool createDomainByPointSel(AcDbObjectId &);

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	bool createDomainByBoundary(AcDbObjectId &);

	/* @接口
	 * @参数
	 * @参数
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	void addDomainProperty(const AcDbObjectId &, const CString &, const CString &, const CString &, const CString &, const CString &, bool);

	/* @接口
	 * @参数 
	 * @参数 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	AcDbObjectId getBuildLayerRangeEntity(const AcDbObjectId &);

private:

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	void setDomainProperty();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月20号
	 */
	void addDomainAnnotate(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月21号
	 */
	AcDbObjectId getBuildLayerEntity(const AcGePoint2d &);

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年9月21号
	 */
	void addXdataProperty(const AcDbObjectId &, const CString &, const CString &, const CString &, const CString &, const CString &, bool);

private:

	TextProperty m_gnq;				//功能区
	TextProperty m_fcpm;			//分层范围线

};

#endif /*DOMAINOPERATE_H*/