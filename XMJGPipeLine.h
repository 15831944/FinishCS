#ifndef XMJGPIPELINE_H
#define XMJGPIPELINE_H

#include "XMJGMacro.h"

class XMJGAssist;
class CPointInfo; 
#include "AttributeDef.h"

class CXMJGPipeLine
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月1号
	 */
	CXMJGPipeLine();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月1号
	 */
	~CXMJGPipeLine();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月16号
	 */
	void importPipeLine();

	/* @接口 创建项目
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月1号
	 */
	void createPipeLineProject();

	/* @接口 修改项目信息
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月16号
	 */
	void modifyProjectInfo();

	/* @接口 绘制管线
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void drawPipeLine();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void exportPipeLineDWG();

	/* @接口 刷新管线属性
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void flushPipeLineProperty();

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	void modifyPipeLineProperty();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月17号
	 */
	void matchingPipePoint();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月19号
	 */
	void changePipeLineZJ();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	void addPipeLineRemark();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void exportPipeLineDwg();

	/* @接口 管点注记
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月3号
	 */
	void remarkPipePoint();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月3号
	 */
	void dimPipelineFlow();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月25号
	 */
	void addPrintTK();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月24号
	 */
	void exportSingleReport();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月11号
	 */
	void modifyPipeLineZJXH();

private:

	
	/* @接口 将标志符替换为照片
	 * @时间 2018年4月25日
	 */
	bool ReplacePic(Word &, VMStr &, CString &);
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月3号
	 */
	void remarkMultiPipePoint();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月3号
	 */
	void remarkSinglePipePoint();
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月3号
	 */
	void clearExistRemark(ads_name);
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月3号
	 */
	void modifyExistRemark(ads_name, int, bool);
	
	/* @接口 
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月16号
	 */
	AcDbObjectId addPipeLinePro(const CPointInfo &, const CString &);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void addPipeLine();

	/* @接口 获取管点属性数据
	 * @参数 ads_name 管点实体
	 * @参数 CString &管点简码
	 * @参数 CString 管点的名称
	 * @参数 AcGePoint2d &管点的位置
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	bool getPipePointData(const AcDbObjectId &, CString &, CString &, AcGePoint2d &);

	/* @接口 选择管点
	 * @参数 ads_name 获取的管点实体
	 * @参数 const CString 获取管点的提示信息
	 * @参数 const CString 上一个管点的类型
	 * @返回 int 0：表示成功，1：表示退出，2：表示选错管点
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	int choosePipePoint(AcDbObjectId &, const CString &, const CString &);

	/* @接口 获取管线的编码和颜色
	 * @参数 CString 管线的简码
	 * @参数 CString 管线的编码
	 * @参数 CString 管线的图层
	 * @参数 int 管线的颜色
	 * @返回 bool 获取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月17号
	 */
	bool getPipeLineProperty(const CString &, CString &, CString &, int &);

	/* @接口
	 * @参数
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void PipefilterObj(VStr &vals, const CString &strPipeExport);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void SavePipeBlock(AcDbObjectIdArray &idArr);

	/* @接口
	 * @参数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	bool exportEntityToDWG(const VStr &);

	/* @接口
	* @作者 杨发荷
	* @邮箱 575814050@qq.com
	* @时间 2018年3月19号
	*/
	void filterLayerEntity(const AcGePoint2dArray &node, MStr &strMCLy, int);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void filterLayerEntity(XMJGAssist &assist, const AcGePoint2dArray &node, MStr &strMCJM, int);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void saveEntityToDwg(const CString &strMC, const CString &fileName);

	/* @接口
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void saveEntityToDwg(XMJGAssist &assist, const CString &strMC, const CString &fileName);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void savePipeLineDZT(MStr &, const AcGePoint2dArray &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void savePipeLineZPT(const MStr &, const AcGePoint2dArray &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void exportPipeLineCGB(const MStr &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	void exportPipeLineDZB(const MStr &);

	/* @接口 获取图面上指定的图层的所有管线
	 * @参数 CString 指定的图层
	 * @返回 AcDbObjectIdArray 管线实体ID集合
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	AcDbObjectIdArray getPipeLineEntitys(const CString &);

	/* @接口 获取图面上指定图层的所有管点实体
	 * @参数 CString 指定的图层
	 * @返回 AcDbObjectIdArray 管点实体ID集合
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月19号
	 */
	AcDbObjectIdArray getPipePointEntitys(const CString &);
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void exportRoadJSZJ();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void exportRoadJSZJ(Word &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void PlaneComparisonTable(const AcDbObjectIdArray &, Word &word);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void ZDMComparisonTable(const AcDbObjectIdArray &, Word &word);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void HDMComparisonTable(const AcDbObjectIdArray &, Word &word);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	void JSZJTable(const AcDbObjectIdArray &, Word &word);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月21号
	 */
	double Dist2Points(const vector<CString> &, const vector<CString> &);

	/* @接口 道路管线竣工成果对照及技术总结
	 * @时间 2018年4月17日
	 */
	void exportRoadGXJSZJ(Word &);
	
	/* @接口 道路竣工验收报告书
	 * @时间 2018年4月20日
	 */
	void exportRoadJGYSBGS();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月25号
	 */
	void addPipeLineDZTuQian(ads_point, const CString &);

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年6月13号
	 */
	MSStr readPipeLineProperty(const AcDbObjectId &id, const ATTRFIELD_ARRAY &);

};

#endif /*XMJGPIPELINE_H*/