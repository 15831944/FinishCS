#ifndef CARINFO_H
#define CARINFO_H

#include "XMJGMacro.h"

class CCarInfo
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	CCarInfo();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	~CCarInfo();

	/* @接口 绘制车位
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void drawCar();

	/* @接口 添加要设置的实体
	 * @参数 AcDbObjectId 被设置的实体id
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void setObject(const AcDbObjectId &);

	/* @接口 添加要设置的实体
	 * @参数 AcDbObjectIdArray 被设置的实体集合
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void setObject(const AcDbObjectIdArray &);

	/* @接口 设置属性
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void setProperty(const AcDbObjectId &);

	/* @接口 设置默认属性
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void setDefaultProperty();

	/* @接口 
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void removeText(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void removeText(const AcDbObjectIdArray &);

	/* @接口 获取车位的最大编号
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	int getCarSerialNum();

	/* @接口 车位重构
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void dimCar();

	/* @接口 添加车位注记
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void setCarAnnotation();

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月1号
	 */
	void setPropertys();

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年8月1号
	 */
	void setCarType();

private:

	int m_num;
	double m_Scale;
	TextProperty m_cw;				//车位
	TextProperty m_cwzj;			//车位注记

};

#endif /*CARINFO_H*/