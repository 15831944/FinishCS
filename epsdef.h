#ifndef EPSDEF_H
#define EPSDEF_H

#include "tcode.h"

typedef struct EPSDB
{

	int m_color;
	CString m_jm;
	double m_width;
	CString m_code;
	CString m_type;
	CString m_alias;
	CString m_layer;
	AcDbObjectId m_id;

	EPSDB()
		: m_color(-1), m_jm(_T("")), m_code(_T(""))
		, m_type(_T("")), m_alias(_T("")), m_width(0)
		, m_layer(_T(""))
	{
		
	}

}epsdb;

typedef vector<epsdb> vepsdb;

class EPSDef
{
public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	EPSDef();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	~EPSDef();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	bool initDef(CString path = _T(""));

	/* @接口 根据指定的简码获取相应的EPS属性
	 * @参数 CString jm 简码
	 * @参数 epsdb 属性
	 * @返回 bool 获取成功返回值为true， 否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	bool getdbFromJM(const CString &, epsdb &);

	/* @接口 根据指定的简码获取相应的EPS属性
	 * @参数 CString code 编码
	 * @参数 epsdb 属性
	 * @返回 bool 获取成功返回值为true， 否则返回值为false
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	bool getdbFromCode(const CString &, epsdb &);

private:

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	bool initPath( CString &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	bool readFile(const CString &);

private:

	vepsdb m_db;

};

class CorDef
{

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	CorDef();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	~CorDef();
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	void drawData();

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	bool initFile(const CString &);

private:
	
	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	AcGePoint3d analyzePoint(const vector<tstring> &);
	
	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	void drawPipeLine(AcGePoint3dArray &, epsdb &);

	/* @接口
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月18号
	 */
	bool drawPipePoint(AcGePoint3dArray &, epsdb &);

	/* @接口
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年4月19号
	 */
	void drawPipePointText(const AcGePoint3dArray &, const epsdb &);

private:

	typedef vector<AcGePoint3dArray> VPoint3dArray;
	typedef map<CString, VPoint3dArray> MSVPoint3dArray;
	typedef MSVPoint3dArray::iterator MSVPIter;
	MSVPoint3dArray m_pt;

};

#endif /*EPSDEF_H*/