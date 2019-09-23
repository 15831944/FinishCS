#ifndef SETENTITYPROPERTY_H
#define SETENTITYPROPERTY_H

#include "Global.h"

class CSetEntityPropery
{
public:
	
	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年12月2号
	 */
	CSetEntityPropery();
	
	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @时间 2017年12月2号
	 */
	~CSetEntityPropery();

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月2号
	 */
	void setProjectLine(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月3号
	 */
	void setDXFWX(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月3号
	 */
	void setGNQ(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月3号
	 */
	void setBG(const AcDbObjectId &);

	/* @接口 设置天桥边界属性
	 * @参数 AcDbObjectId 天桥边界id
	 * @作者 杨发荷
	 * @时间 2018年1月4号
	 */
	void setBridge(const AcDbObjectId &);

	/* @接口 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月18号
	 */
	void setRedLine(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月3号
	 */
	void setJGBuild(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月3号
	 */
	void setBuild(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @时间 2017年12月3号
	 */
	void setBuildBoundaries(const AcDbObjectId &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月25号
	 */
	void setExteriorWall(const AcDbObjectId &);

	/* @接口 设置内墙线
	 * @参数 AcDbObjectId 内墙线的id
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月30号
	 */
	void setInnerWall(const AcDbObjectId &);

	/* @接口 获取实体左上角位置
	 */
	AcGePoint3d getRectangleTopLeftPos(const AcDbObjectId &id);

private:

	int m_gnqbh;
	double m_scale;
	IDataBaseOper oper;

};

#endif /*SETENTITYPROPERTY_H*/