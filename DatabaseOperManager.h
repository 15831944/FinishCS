#ifndef _RIGHTS_HOLDER_MANAGER_
#define _RIGHTS_HOLDER_MANAGER_

#include "AdoConnection.h"
#include "AdoRecordSet.h"

//map����ͳһȫ���ô�д

#define NIN _T("")
typedef std::map<CString, CString> MStr;
typedef MStr::iterator MSIter;
typedef MStr::const_iterator MSCIter;
typedef std::vector<MStr> VMStr;
typedef VMStr::iterator VMSIter;
typedef std::vector<CString> VStr;
typedef VStr::iterator VSIter;
typedef std::vector<int> VInt;
typedef VInt::iterator VIter;
typedef std::vector<VStr> VVStr;
typedef VVStr::iterator VVSIter;

class CDatabaseOperManager
{
public:

	/* @function Ĭ�Ϲ��캯��
	 * @author ������
	 */
	CDatabaseOperManager(void);

	/* @function Ĭ����������
	 * @author ������
	 */
	~CDatabaseOperManager(void);

public:

	void close();

	/* @�ӿ� 
	 * @���� ��� 
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��3��8��
	 */
	void reflash();
	
	/* @�ӿ� ��ȡȨ��������
	* @���� CString Ȩ���˵�ID
	* @���� CString Ȩ���˵�����
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	CString GetQLRMC(const CString &strID);

	/* @�ӿ� 
	 * @����
	 * @���� ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��9��24��
	 */
	bool clearTable(const CString &);

	/* @�ӿ� �ж��ļ��Ƿ����
	 * @���� CString �ļ�·��
	 * @���� �ļ����ڣ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��06��28��
	 */
	bool isFileExsist(const CString &strPath);

	/* @function ��ȡĳ������ֶθ�����Ҳ�����ǻ�ȡĳ���������
	* @param tableName ����
	* @return int ���ر������
	* @author ���
	* @time 2016��11��1��
	*/
	int GetTableRow(const CString &tableName);

	/* @function ɾ��ָ���ı�
	 * @param tableName ����
	 * @author ���
	 * @time 2016��11��1��
	 */
	bool DeleteTable(const CString &tableName);

	/* @function ��ʼ�����ݿ�����
	 * @param strPath ���ݿ�����·��(��������·��Ϊ�� ֱ�Ӷ�ȡ��Ӧ·�������Ȩ����mdb)
	 * @return bool ��ʼ���ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @author ������
	 */
	bool initialConnect(CString strPath = _T(""));

	/* @�ӿ�
	* @���� ���
	* @ʱ�� 2017��11��21��
	*/
	bool DeleteReocrd(const CString &, const MStr &);

	/* @�ӿ� ��ȡָ���ı��м�¼������
	* @���� CString ����
	* @���� int ���еļ�¼����
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	int GetTableRecordCount(const CString &tableName);

	bool DeleteDataBase_All(const CString& tableName);

	/* @function �ж�ָ���ļ�¼�Ƿ����
	* @param ID ��¼
	* @return bool ���ݸ��³ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	* @author ������
	*/
	bool hasRow(const CString& tableName, const CString& ID);

	/* @function ��ȡ���ݣ�ע�⣬����ӿ��Ƕ�ȡ�������ȫ������
	 * @param tableName ��Ҫ��ȡ�ı���
	 * @param record ��ȡ�Ľ�����ò�������һ��vector��map����
	 * @author ���
	 * @time 2016��11��1��
	 */
	bool ReadDataBase(const CString &tableName, VMStr &record);
	
	/* @�ӿ�
	 * @���� 
	 * @���� 
	 * @���� 
	 * @���� ���
	 * @���� 575814050@qq.com
	 * @ʱ�� 2018��1��25��
	 */
	bool ReadDataBase(const CString &, const CString &, MStr &);

	/* @�ӿ� ��ȡ���ݿ���ָ�������������
	* @���� CString ָ���ı���
	* @���� VMStr V(һ�м�¼��M(�ֶ���Ϊkey��ֵΪvalue)Str��ʾ����CString����
	* @���� bool ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	bool ReadDataBase_all(const CString& tableName, VMStr &data);
	
	/* @�ӿ� ��ȡ�ֶ�
	 * @���� CString ��ȡ�ı���
	 * @���� CString ��ȡ���ֶ���
	 * @���� CString ��ȡ�Ľ��
	 * @���� bool ��ȡ�ɹ�������ֵΪtrue�����򷵻�false
	 * @���� ���
	 * @ʱ�� 2017��07��18��
	 */
	bool ReadDataBase(const CString &, const CString &, CString &);

	/* @�ӿ� ��ȡ���ݿ��е�����
	 * @���� CString ��ȡ�ı���
	 * @���� CString ��ȡ���ֶ���
	 * @���� MStr ��ȡ���ݵĹ�������
	 * @���� VStr ��ȡ�Ľ��
	 * @���� bool ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��07��19��
	 */
	bool ReadDataBase(const CString &, const CString &, const MStr &, VStr &);

	/* @�ӿ� ��ȡ����
	 * @���� CString ��ȡ�ı���
	 * @���� CString ��ȡ���ֶ���
	 * @���� MStr ��ȡ���ݵĹ�������
	 * @���� CString ��ȡ��ֵ
	 * @���� bool ��ȡ�ɹ�������ֵΪtrue�����򷵻�false
	 * @���� ���
	 * @ʱ�� 2017��07��19��
	 */
	bool ReadDataBase(const CString &, const CString &, const MStr &, CString &);

	/* @�ӿ� ��ȡ���ݿ���ָ�����������һ�м�¼
	* @���� CString ָ���ı���
	* @���� MStr M(�ֶ���Ϊkey��ֵΪvalue)Str(����ΪCString)
	* @���� bool ��ȡ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	bool ReadDataBase_LastRow(const CString& tableName, MStr &data);

	/* @function �����µı�
	* @param tableName �µı���
	* @param fieldName �ֶ�������
	* @return bool �����ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	* @author ������
	*/
	bool createTable(const CString &tableName, VStr &veStrFildName);

	/* @function ��ָ�������ݿ������һ����¼
	* @param tableName ����
	* @param record ������һ����¼
	* @return bool ��ӳɹ�����ֵΪtrue�� ���򷵻�ֵΪfalse
	* @author ���
	* @time 2016��11��1��
	*/
	bool InsertRecord(const CString &tableName, const MStr &record);

	/* @function ����ֶ���
	* @param tableName ���ݵı���
	* @param fieldName �ֶ����б���
	* @return bool ����ֶ����ɹ�����ֵΪtrue�����򷵻�ֵfalse
	* @author ������
	*/
	bool getFieldNameList(const CString &tableName, VStr &fieldName);

	/* @function ����ָ���ı����еĶ�����¼�����ݵ�һ���ֶΣ�
	* @param tableName ����
	* @param record ���¼�¼�ļ���
	* @return bool ���³ɹ�����ֵΪtrue�� ���򷵻�ֵΪfalse
	* @author ���
	* @time 2016��11��1��
	*/
	bool InsertRecord(const CString &tableName, const VMStr &records);

	/* @function ��ȡ���ݿ���ĳ�������ж���������
	* @param tableName ��Ҫ��ѯ�ı���
	* @param field ��������ĳ���ֶΣ�����ѯ��ԭ����Ǹ���ĳ�����а���ĳ���ֶεĸ�����
	* @return int �����ж���������
	* @author ���
	* @time 2016��11��1��
	*/
	int GetTableColumn(const CString &tableName, const CString &field);

	/* @�ӿ� ɾ�����ݿ�����������
	* @���� CString ָ���ı���
	* @���� MStr M(Ҫɾ�����ֶ������Լ�ֵ��
	* @���� bool ���ɾ���ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	bool DeleteDataOther(const CString &tableName, const MStr &filter);

	/* @�ӿ� ����ָ����ID��ȡȨ���˵�����
	* @���� CString Ȩ���˵�ID
	* @���� CString ���ݿ��·��
	* @���� CString Ȩ���˵�����
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	CString GetQLRMC(const CString &strID, const CString &strDataBasePath);

	/* @function ��������Ĺ��ܾ��ǻ�ȡĳ�����а���ĳ����¼���ݸ���
	* @param tableName ����
	* @param field_value ���һ���ֶ�����ֵ����Ӧ������map����
	* @return int ����field_value���ҵĽ������
	*/
	int TableContainFieldValue(const CString &tableName, const MStr &field_value);

	bool addRow(const CString &tableName, const CString &ID, const MStr &valuePair);

	//ִ�в�ѯ ���� ���� �ֶ��б� ��ѯ���ֶζ�Ӧ��ֵ
	//�������еĲ�ѯ��¼ ����ÿ����¼���ֶ����ƺ�ֵ
	bool hasRow(const CString &tableName, const MStr &field_value, VMStr &selectRow);

	/* @function ����ֶ���
	* @param tableName ���ݵı���
	* @param fieldName �ֶ����б�
	* @param isDelete �Ƿ�ɾ��ԭ�������ݣ�Ĭ��Ϊɾ����
	* @param isMerge �Ƿ�ϲ�ԭ�������ݣ�Ĭ��Ϊ�ϲ���
	* @author ������
	*/
	bool getFieldNameList(const CString &tableName, VStr &fieldName, VInt &fieldType);

	/* @function ��ȡ����
	* @param tableName ��Ҫ��ȡ�ı���
	* @param record ��ȡ�Ľ��
	* @param condition ������������Ӧ�����ֶ������ֶ�ֵ�������������£�
	* eg. ��ʼ��MCString condition; condition[_T("CBFMC")] = _T("����");
	* condition[_T("DKBM")] = _T("1213232321321321"); ��������
	* @author ���
	* @time 2016��11��1��
	*/
	bool ReadDataBase(const CString &tableName, VMStr &record, const MStr &condition);

	/* @�ӿ� ����ָ�����ֶκͱ�����ȡ��Ӧ��ֵ
	* @���� CString ����
	* @���� CString �ֶ���
	* @���� VStr ��ȡ��ֵ
	* @���� ��ȡֵ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	* @���� ���
	* @ʱ�� 2017��06��28��
	*/
	bool GetValueByField(const CString &tableName, const CString &field, VStr &value);

	/* @function �������ݿ��е�һ����¼
	* @param tableName ����
	* @param ID һ����¼
	* @param valuePair
	* @return bool ���ݸ��³ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	* @author ������
	*/
	bool updateRow(const CString &tableName, const CString &ID, const MStr &valuePair);

	/* @function ��mdb�ж�ȡ����
	 * @param tableName ��ȡ���ݵı���
	 * @param strFieldName ��ȡ���ֶ���
	 * @param data ��ȡ������
	 * @return bool ��ȡ����ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	 * @author ������
	 */
	bool ReadDataBase(const CString &tableName, const CString &strFieldName, VMStr &data);

	/* @�ӿ� ɾ�����ݿ��еļ�¼
	 * @���� CString ����
	 * @���� CString �ֶ���
	 * @���� VStr ���е�ֵ��VStr�е�ֵ��ͬ����ɾ��
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��07��03��
	 */
	bool DeleteDataBase(const CString &tableName, const CString &field, const VStr &value);

	/* @�ӿ� ɾ�����еļ�¼
	 * @���� CString ����
	 * @���� CString �ֶ���
	 * @���� VStr ��VStr�е�ֵ��ͬ�Ķ�ɾ��
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��07��03��
	 */
	bool DeleteDataOther(const CString &tableName, const CString &field, const VStr &value);

	/* @function �����ݿ������һ���µļ�¼�����������¼�Ѿ������ݿ��У���ô�ͽ�������������¼��
	������ݿ��в�����������¼����ô��������¼��ӵ����ݿ���
	* @param tableName ����
	* @param record һ����¼
	* @return bool ��ӳɹ�����ֵΪtrue�� ���򷵻�ֵΪfalse
	* @author ���
	* @time 2016��11��1��
	*/
	bool AddOneRecord(const CString &tableName, const MStr &record, const MStr &field_value);

	/* @function ����ָ���ı����е�һ����¼�����ݵ�һ���ֶΣ�
	* @param tableName ����
	* @param record ���µļ�¼
	* @return bool ���³ɹ�����ֵΪtrue�� ���򷵻�ֵΪfalse
	* @author ���
	* @time 2016��11��1��
	*/
	bool UpdataRecord(const CString &tableName, const MStr &field_value, const MStr &reocrd);

	/* @�ӿ� ɾ�����еļ�¼
	 * @���� CString ����
	 * @���� CString �ֶ���
	 * @���� CString �ֶ�����Ӧ��ֵɾ��
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��07��03��
	 */
	bool DeleteDataBase(const CString &tableName, const CString &field, const CString &value);

	/* @�ӿ� ɾ�����������ļ�¼
	 * @���� CString ����
	 * @���� CString �ֶ���
	 * @���� CString ��������ֵ
	 * @���� 
	 * @���� ���
	 * @ʱ�� 2017��07��03��
	 */
	bool DeleteDataOther(const CString &tableName, const CString &field, const CString &value);

	bool DeleteDataBase_Where(const CString &tableName, VStr &veStrFieldName, VStr &veStrValue);

	/* @�ӿ� ����ָ�����ֶκͱ�����ȡ��Ӧ��ֵ
	 * @���� CString ����
	 * @���� CString �ֶ���
	 * @���� VStr ��ȡ��ֵ
	 * @���� MStr ��������
	 * @���� ��ȡֵ�ɹ�������ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��06��28��
	 */
	bool GetValueByField(const CString &table, const CString &field, VStr &value, const MStr &filter);

	/* @function �����ݱ��浽mdb��
	* @param tableName �������ݵı���
	* @param data ��ȡ������
	* @param fieldType
	* @return bool �������ݳɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	* @author ������
	*/
	bool saveDataBase(const CString &tableName, const VVStr &data, bool isDelete = true, bool isMerge = true);

	bool saveDataBase_QLR(const CString& tableName, const VVStr &data, const VStr &tbfield, bool ISUPDATEGUID);

	/* @function ��ѯ���ݸ��ݸ���������
	* @param tableName ���ݱ���
	* @param fieldName �����ֶ���
	* @param selValue ��ѡֵ
	* @param data ��ѯ�Ľ��
	* @return bool ��ѯ�ɹ�����ֵΪtrue�����򷵻�ֵΪfalse
	* @author ������
	*/
	bool selectByCondition(const CString &tableName, const CString &strField, const VStr &selValue, VMStr &data);

	/* @function �Ƚ�ĳ�����е�һ������
	 * @param tableName ����
	 * @param field �ֶ���
	 * @param value �ֶ�ֵ
	 * @param record һ����¼(����һ����¼��Ҳ����һ���ֶκ�һ���֡��ҳ��ϵ���������ݽṹ��ʱ�������⣩
	 * @return bool ���������ֶ��ڱ��м�¼�봫�����¼�е�ֵ��ͬ������ֵΪtrue�����򷵻�ֵΪfalse
	 * @author ���
	 * @time 2016��11��1��
	 */
	bool CompareRecord(const CString &tableName, const CString &field, const CString &value, const MStr &record);

	bool ReadDataBase_Where(const CString &tableName, VStr &veStrFieldName, VStr &veStrValue, CString &strResultFieldName, VMStr &data);

private:

	/* @�ӿ� ���뺯��ǰ�ȼ��ado�Ƿ���Ч
	 * @���� bool ado��Ч����ֵΪtrue�����򷵻�ֵΪfalse
	 * @���� ���
	 * @ʱ�� 2017��06��28��
	 */
	bool checkAdo();

private:

	CAdoConnection _connect_;//���ݿ�����

};

#endif