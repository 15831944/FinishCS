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

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	EPSDef();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	~EPSDef();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	bool initDef(CString path = _T(""));

	/* @�ӿ� ����ָ���ļ����ȡ��Ӧ��EPS����
	 * @���� CString jm ����
	 * @���� epsdb ����
	 * @���� bool ��ȡ�ɹ�����ֵΪtrue�� ���򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	bool getdbFromJM(const CString &, epsdb &);

	/* @�ӿ� ����ָ���ļ����ȡ��Ӧ��EPS����
	 * @���� CString code ����
	 * @���� epsdb ����
	 * @���� bool ��ȡ�ɹ�����ֵΪtrue�� ���򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	bool getdbFromCode(const CString &, epsdb &);

private:

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	bool initPath( CString &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	bool readFile(const CString &);

private:

	vepsdb m_db;

};

class CorDef
{

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	CorDef();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	~CorDef();
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	void drawData();

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	bool initFile(const CString &);

private:
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	AcGePoint3d analyzePoint(const vector<tstring> &);
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	void drawPipeLine(AcGePoint3dArray &, epsdb &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��18��
	 */
	bool drawPipePoint(AcGePoint3dArray &, epsdb &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��19��
	 */
	void drawPipePointText(const AcGePoint3dArray &, const epsdb &);

private:

	typedef vector<AcGePoint3dArray> VPoint3dArray;
	typedef map<CString, VPoint3dArray> MSVPoint3dArray;
	typedef MSVPoint3dArray::iterator MSVPIter;
	MSVPoint3dArray m_pt;

};

#endif /*EPSDEF_H*/