#ifndef XMJGMUNICIPAL_H
#define XMJGMUNICIPAL_H

#include "XMJGMacro.h"
#include "AttributeDef.h"
#include "smarttable_lua.h"

class XMJGAssist;
class CPointInfo;

using namespace SmartTable;

class XMJGMunicipal
{

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	XMJGMunicipal();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	~XMJGMunicipal();
	
	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	XMJGMunicipal(int);

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月25号
	 */
	void exportRoadJSZJ();
	
	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年3月29号
	 */
	void exportRoadGHHSSQB();

	/* @接口 道路竣工验收报告书
	 * @时间 2018年4月20日
	 */
	void exportRoadJGYSBGS();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月25号
	 */
	void exporBridgeJGYSBGS();
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void exportPipeLineTable();	
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	void exportRoadPipeReport();
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月26号
	 */
	bool initWord(const CString &);

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月26号
	 */
	void exportRoadPipeBridgeReport();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月18号
	 */
	void save();

	/* @接口 替换基本信息
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月26号
	 */
	void replaceJBXX();

private:

	/* @接口 
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年7月4号
	 */
	void exportGHHSJBQK();

	/* @接口 替换照片
	 * @返回 bool 成功返回值为true，否则返回值为false
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2019年7月2号
	 */
	void replaceWordField();

	/* @接口 替换规划实施基本情况
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年5月30号
	 */
	void replaceGHSSJBQKPipe();

	/* @接口 道路竣工技术总结小表
	 * @时间 2018年4月23日
	 */
	void JSZJTable(const AcDbObjectIdArray &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void exportPipeLineZPT(const CString &, int);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void exportPipeLineDZT(const CString &, int);

	/* @接口 将标志符替换为照片
	 * @时间 2018年4月25日
	 */
	bool ReplacePic(VMStr &records, CString &strPic, int);
	
	/* @接口 道路竣工纵断对照表
	 * @时间 2018年4月23日
	 */
	void ZDMComparisonTable(const AcDbObjectIdArray &);

	/* @接口 道路竣工横断对照表
	 * @时间 2018年4月23日
	 */
	void HDMComparisonTable(const AcDbObjectIdArray &);
	
	/* @接口 道路竣工平面对照表
	 * @时间 2018年4月23日
	 */
	void PlaneComparisonTable(const AcDbObjectIdArray &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void addTitle(const CString &, const CString &, int);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void fillToFormTableDZB(WordTable &, const MStr &, int);
	
	/* @接口
	 * @时间 2018年4月23日
	 */
	double Dist2Points(const vector<CString> &pt1, const vector<CString> &pt2);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	MSStr readPipeLineProperty(const AcDbObjectIdArray &, const ATTRFIELD_ARRAY &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	void exportPipeLineCGB(WordTable &, const CString &, const AcDbObjectIdArray &, int);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月23号
	 */
	void exportPipeLineDZB(WordTable &, const CString &, const AcDbObjectIdArray &, int);
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月24号
	 */
	void fillToFormTableCGB(WordTable &, const MStr &, int, const ATTRFIELD_ARRAY &,CString ljd = _T(""), int loc = 2);

	double Dist2Points(const CString &strSX, const CString &strSY, const CString &strSZ, const CString &strEX, const CString &strEY, const CString &strEZ);

private:

	int m_type;
	Word m_word;
	vector<double> vSCLength;
	vector<double> vSJLength;
	vector<double> vDLength;
	typedef map<CString, AcDbObjectIdArray> MSIDS;

};

#endif /*XMJGPIPELINE_H*/