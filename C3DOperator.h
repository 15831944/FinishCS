#pragma once

struct AREAINFO
{
	AcDbObjectId IdFlood;//水淹区
	AcDbObjectIdArray idSafe;//一个水淹区域内部岛状安全区
};

class C3DOperator
{
public:

	C3DOperator(void);

	~C3DOperator(void);
	
	/* @接口 填充面域
	 * @参数 AREAINFO 被填充的面域
	 * @参数 int 填充的颜色
	 * @参数 TCHAR *填充的类型
	 * @返回 AcDbObjectId 返回填充id
	 * @作者 杨发荷
	 * @时间 2017年12月11号
	 */
	static AcDbObjectId HatchObject(const AREAINFO &, int, TCHAR *);

	/* @接口 填充面域
	 * @参数 AcDbObjectId 被填充的面域
	 * @参数 int 填充的颜色
	 * @参数 TCHAR *填充的类型
	 * @返回 AcDbObjectId 返回填充id
	 * @作者 杨发荷
	 * @时间 2017年12月11号
	 */
	static AcDbObjectId HatchObject(const AcDbObjectId &, int, const TCHAR *);
	
	// 创建环状面域
	static AcDbObjectId CreateRingRegion(const AcDbObjectId outId, const AcDbObjectIdArray inIdArr);
	// 面域逻辑操作
	static bool RegionBooleanOper(AcDbObjectId regionId1, AcDbObjectId regionId2, AcDb::BoolOperType operType);
	// 三维实体逻辑操作
	static bool SolidBooleanOper(AcDbObjectId solidId1, AcDbObjectId solidId2, AcDb::BoolOperType operType);
	// 创建三维实体
	static AcDbObjectId CreateSolid(const AcDbObjectId regionId, double height, bool bDelRegion = true);
	// 三维实体批量取并
	// mainId为主实体，合并操作以该实体为准
	static bool SolidBooleanUnion(const AcDbObjectId mainId, const AcDbObjectIdArray& idArr, AcDbObjectIdArray &unSuccessIds);
};

