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

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	XMJGMunicipal();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	~XMJGMunicipal();
	
	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	XMJGMunicipal(int);

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��25��
	 */
	void exportRoadJSZJ();
	
	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��3��29��
	 */
	void exportRoadGHHSSQB();

	/* @�ӿ� ��·�������ձ�����
	 * @ʱ�� 2018��4��20��
	 */
	void exportRoadJGYSBGS();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��25��
	 */
	void exporBridgeJGYSBGS();
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
	 */
	void exportPipeLineTable();	
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	void exportRoadPipeReport();
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��26��
	 */
	bool initWord(const CString &);

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��26��
	 */
	void exportRoadPipeBridgeReport();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��18��
	 */
	void save();

	/* @�ӿ� �滻������Ϣ
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��26��
	 */
	void replaceJBXX();

private:

	/* @�ӿ� 
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��7��4��
	 */
	void exportGHHSJBQK();

	/* @�ӿ� �滻��Ƭ
	 * @���� bool �ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2019��7��2��
	 */
	void replaceWordField();

	/* @�ӿ� �滻�滮ʵʩ�������
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��30��
	 */
	void replaceGHSSJBQKPipe();

	/* @�ӿ� ��·���������ܽ�С��
	 * @ʱ�� 2018��4��23��
	 */
	void JSZJTable(const AcDbObjectIdArray &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
	 */
	void exportPipeLineZPT(const CString &, int);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
	 */
	void exportPipeLineDZT(const CString &, int);

	/* @�ӿ� ����־���滻Ϊ��Ƭ
	 * @ʱ�� 2018��4��25��
	 */
	bool ReplacePic(VMStr &records, CString &strPic, int);
	
	/* @�ӿ� ��·�����ݶ϶��ձ�
	 * @ʱ�� 2018��4��23��
	 */
	void ZDMComparisonTable(const AcDbObjectIdArray &);

	/* @�ӿ� ��·������϶��ձ�
	 * @ʱ�� 2018��4��23��
	 */
	void HDMComparisonTable(const AcDbObjectIdArray &);
	
	/* @�ӿ� ��·����ƽ����ձ�
	 * @ʱ�� 2018��4��23��
	 */
	void PlaneComparisonTable(const AcDbObjectIdArray &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
	 */
	void addTitle(const CString &, const CString &, int);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
	 */
	void fillToFormTableDZB(WordTable &, const MStr &, int);
	
	/* @�ӿ�
	 * @ʱ�� 2018��4��23��
	 */
	double Dist2Points(const vector<CString> &pt1, const vector<CString> &pt2);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
	 */
	MSStr readPipeLineProperty(const AcDbObjectIdArray &, const ATTRFIELD_ARRAY &);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	void exportPipeLineCGB(WordTable &, const CString &, const AcDbObjectIdArray &, int);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	void exportPipeLineDZB(WordTable &, const CString &, const AcDbObjectIdArray &, int);
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
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