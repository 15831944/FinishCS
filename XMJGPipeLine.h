#ifndef XMJGPIPELINE_H
#define XMJGPIPELINE_H

#include "XMJGMacro.h"

class XMJGAssist;
class CPointInfo; 
#include "AttributeDef.h"

class CXMJGPipeLine
{
public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��1��
	 */
	CXMJGPipeLine();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��1��
	 */
	~CXMJGPipeLine();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��16��
	 */
	void importPipeLine();

	/* @�ӿ� ������Ŀ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��1��
	 */
	void createPipeLineProject();

	/* @�ӿ� �޸���Ŀ��Ϣ
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��16��
	 */
	void modifyProjectInfo();

	/* @�ӿ� ���ƹ���
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	void drawPipeLine();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void exportPipeLineDWG();

	/* @�ӿ� ˢ�¹�������
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void flushPipeLineProperty();

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	void modifyPipeLineProperty();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��17��
	 */
	void matchingPipePoint();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��19��
	 */
	void changePipeLineZJ();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��23��
	 */
	void addPipeLineRemark();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��24��
	 */
	void exportPipeLineDwg();

	/* @�ӿ� �ܵ�ע��
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��3��
	 */
	void remarkPipePoint();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��3��
	 */
	void dimPipelineFlow();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��25��
	 */
	void addPrintTK();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��24��
	 */
	void exportSingleReport();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��11��
	 */
	void modifyPipeLineZJXH();

private:

	
	/* @�ӿ� ����־���滻Ϊ��Ƭ
	 * @ʱ�� 2018��4��25��
	 */
	bool ReplacePic(Word &, VMStr &, CString &);
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��3��
	 */
	void remarkMultiPipePoint();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��3��
	 */
	void remarkSinglePipePoint();
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��3��
	 */
	void clearExistRemark(ads_name);
	
	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��5��3��
	 */
	void modifyExistRemark(ads_name, int, bool);
	
	/* @�ӿ� 
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��16��
	 */
	AcDbObjectId addPipeLinePro(const CPointInfo &, const CString &);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void addPipeLine();

	/* @�ӿ� ��ȡ�ܵ���������
	 * @���� ads_name �ܵ�ʵ��
	 * @���� CString &�ܵ����
	 * @���� CString �ܵ������
	 * @���� AcGePoint2d &�ܵ��λ��
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	bool getPipePointData(const AcDbObjectId &, CString &, CString &, AcGePoint2d &);

	/* @�ӿ� ѡ��ܵ�
	 * @���� ads_name ��ȡ�Ĺܵ�ʵ��
	 * @���� const CString ��ȡ�ܵ����ʾ��Ϣ
	 * @���� const CString ��һ���ܵ������
	 * @���� int 0����ʾ�ɹ���1����ʾ�˳���2����ʾѡ��ܵ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	int choosePipePoint(AcDbObjectId &, const CString &, const CString &);

	/* @�ӿ� ��ȡ���ߵı������ɫ
	 * @���� CString ���ߵļ���
	 * @���� CString ���ߵı���
	 * @���� CString ���ߵ�ͼ��
	 * @���� int ���ߵ���ɫ
	 * @���� bool ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��17��
	 */
	bool getPipeLineProperty(const CString &, CString &, CString &, int &);

	/* @�ӿ�
	 * @����
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void PipefilterObj(VStr &vals, const CString &strPipeExport);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void SavePipeBlock(AcDbObjectIdArray &idArr);

	/* @�ӿ�
	 * @����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	bool exportEntityToDWG(const VStr &);

	/* @�ӿ�
	* @���� ���
	* @���� 575814050@qq.com
	* @ʱ�� 2018��3��19��
	*/
	void filterLayerEntity(const AcGePoint2dArray &node, MStr &strMCLy, int);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void filterLayerEntity(XMJGAssist &assist, const AcGePoint2dArray &node, MStr &strMCJM, int);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void saveEntityToDwg(const CString &strMC, const CString &fileName);

	/* @�ӿ�
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void saveEntityToDwg(XMJGAssist &assist, const CString &strMC, const CString &fileName);

	/* @�ӿ�
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void savePipeLineDZT(MStr &, const AcGePoint2dArray &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void savePipeLineZPT(const MStr &, const AcGePoint2dArray &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void exportPipeLineCGB(const MStr &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	void exportPipeLineDZB(const MStr &);

	/* @�ӿ� ��ȡͼ����ָ����ͼ������й���
	 * @���� CString ָ����ͼ��
	 * @���� AcDbObjectIdArray ����ʵ��ID����
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	AcDbObjectIdArray getPipeLineEntitys(const CString &);

	/* @�ӿ� ��ȡͼ����ָ��ͼ������йܵ�ʵ��
	 * @���� CString ָ����ͼ��
	 * @���� AcDbObjectIdArray �ܵ�ʵ��ID����
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��19��
	 */
	AcDbObjectIdArray getPipePointEntitys(const CString &);
	
	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void exportRoadJSZJ();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void exportRoadJSZJ(Word &);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void PlaneComparisonTable(const AcDbObjectIdArray &, Word &word);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void ZDMComparisonTable(const AcDbObjectIdArray &, Word &word);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void HDMComparisonTable(const AcDbObjectIdArray &, Word &word);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	void JSZJTable(const AcDbObjectIdArray &, Word &word);

	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��21��
	 */
	double Dist2Points(const vector<CString> &, const vector<CString> &);

	/* @�ӿ� ��·���߿����ɹ����ռ������ܽ�
	 * @ʱ�� 2018��4��17��
	 */
	void exportRoadGXJSZJ(Word &);
	
	/* @�ӿ� ��·�������ձ�����
	 * @ʱ�� 2018��4��20��
	 */
	void exportRoadJGYSBGS();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��4��25��
	 */
	void addPipeLineDZTuQian(ads_point, const CString &);

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��6��13��
	 */
	MSStr readPipeLineProperty(const AcDbObjectId &id, const ATTRFIELD_ARRAY &);

};

#endif /*XMJGPIPELINE_H*/