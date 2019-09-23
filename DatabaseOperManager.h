#ifndef _RIGHTS_HOLDER_MANAGER_
#define _RIGHTS_HOLDER_MANAGER_

#include "AdoConnection.h"
#include "AdoRecordSet.h"

//map主键统一全部用大写

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

	/* @function 默认构造函数
	 * @author 何星龙
	 */
	CDatabaseOperManager(void);

	/* @function 默认析构函数
	 * @author 何星龙
	 */
	~CDatabaseOperManager(void);

public:

	void close();

	/* @接口 
	 * @作者 杨发荷 
	 * @邮箱 575814050@qq.com
	 * @时间 2018年3月8号
	 */
	void reflash();
	
	/* @接口 获取权利人名称
	* @参数 CString 权利人的ID
	* @返回 CString 权利人的名称
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	CString GetQLRMC(const CString &strID);

	/* @接口 
	 * @参数
	 * @返回 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年9月24日
	 */
	bool clearTable(const CString &);

	/* @接口 判断文件是否存在
	 * @参数 CString 文件路径
	 * @返回 文件存在，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年06月28日
	 */
	bool isFileExsist(const CString &strPath);

	/* @function 获取某个表的字段个数，也可以是获取某个表的列数
	* @param tableName 表名
	* @return int 返回表的列数
	* @author 杨发荷
	* @time 2016年11月1号
	*/
	int GetTableRow(const CString &tableName);

	/* @function 删除指定的表
	 * @param tableName 表名
	 * @author 杨发荷
	 * @time 2016年11月1号
	 */
	bool DeleteTable(const CString &tableName);

	/* @function 初始化数据库连接
	 * @param strPath 数据库连接路径(如果传入的路径为空 直接读取对应路径下面的权利人mdb)
	 * @return bool 初始化成功返回值为true，否则返回值为false
	 * @author 何星龙
	 */
	bool initialConnect(CString strPath = _T(""));

	/* @接口
	* @作者 杨发荷
	* @时间 2017年11月21号
	*/
	bool DeleteReocrd(const CString &, const MStr &);

	/* @接口 获取指定的表中记录的条数
	* @参数 CString 表名
	* @返回 int 表中的记录条数
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	int GetTableRecordCount(const CString &tableName);

	bool DeleteDataBase_All(const CString& tableName);

	/* @function 判断指定的记录是否存在
	* @param ID 记录
	* @return bool 数据更新成功返回值为true，否则返回值为false
	* @author 何星龙
	*/
	bool hasRow(const CString& tableName, const CString& ID);

	/* @function 读取数据（注意，这个接口是读取整个表的全部数据
	 * @param tableName 需要读取的表名
	 * @param record 读取的结果，该参数是有一个vector和map构成
	 * @author 杨发荷
	 * @time 2016年11月1号
	 */
	bool ReadDataBase(const CString &tableName, VMStr &record);
	
	/* @接口
	 * @参数 
	 * @参数 
	 * @参数 
	 * @作者 杨发荷
	 * @邮箱 575814050@qq.com
	 * @时间 2018年1月25号
	 */
	bool ReadDataBase(const CString &, const CString &, MStr &);

	/* @接口 读取数据库中指定表的所有数据
	* @参数 CString 指定的表名
	* @参数 VMStr V(一行记录）M(字段名为key，值为value)Str表示都是CString类型
	* @返回 bool 读取成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	bool ReadDataBase_all(const CString& tableName, VMStr &data);
	
	/* @接口 读取字段
	 * @参数 CString 读取的表名
	 * @参数 CString 读取的字段名
	 * @参数 CString 读取的结果
	 * @返回 bool 读取成功，返回值为true，否则返回false
	 * @作者 杨发荷
	 * @时间 2017年07月18日
	 */
	bool ReadDataBase(const CString &, const CString &, CString &);

	/* @接口 读取数据库中的数据
	 * @参数 CString 读取的表名
	 * @参数 CString 读取的字段名
	 * @参数 MStr 读取数据的过滤条件
	 * @参数 VStr 读取的结果
	 * @返回 bool 读取成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年07月19日
	 */
	bool ReadDataBase(const CString &, const CString &, const MStr &, VStr &);

	/* @接口 读取数据
	 * @参数 CString 读取的表名
	 * @参数 CString 读取的字段名
	 * @参数 MStr 读取数据的过滤条件
	 * @参数 CString 读取的值
	 * @返回 bool 读取成功，返回值为true，否则返回false
	 * @作者 杨发荷
	 * @时间 2017年07月19日
	 */
	bool ReadDataBase(const CString &, const CString &, const MStr &, CString &);

	/* @接口 读取数据库中指定表名的最后一行记录
	* @参数 CString 指定的表名
	* @参数 MStr M(字段名为key，值为value)Str(类型为CString)
	* @返回 bool 读取成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	bool ReadDataBase_LastRow(const CString& tableName, MStr &data);

	/* @function 创建新的表
	* @param tableName 新的表名
	* @param fieldName 字段名集合
	* @return bool 创建成功返回值为true，否则返回值为false
	* @author 何星龙
	*/
	bool createTable(const CString &tableName, VStr &veStrFildName);

	/* @function 在指定的数据库中添加一条记录
	* @param tableName 表名
	* @param record 新增的一条记录
	* @return bool 添加成功返回值为true， 否则返回值为false
	* @author 杨发荷
	* @time 2016年11月1号
	*/
	bool InsertRecord(const CString &tableName, const MStr &record);

	/* @function 获得字段名
	* @param tableName 数据的表名
	* @param fieldName 字段名列表集合
	* @return bool 获得字段名成功返回值为true，否则返回值false
	* @author 何星龙
	*/
	bool getFieldNameList(const CString &tableName, VStr &fieldName);

	/* @function 更新指定的表名中的多条记录（根据第一个字段）
	* @param tableName 表名
	* @param record 更新记录的集合
	* @return bool 更新成功返回值为true， 否则返回值为false
	* @author 杨发荷
	* @time 2016年11月1号
	*/
	bool InsertRecord(const CString &tableName, const VMStr &records);

	/* @function 获取数据库中某个表中有多少行数据
	* @param tableName 需要查询的表名
	* @param field 给定表中某个字段，（查询的原理就是根据某个表中包含某个字段的个数）
	* @return int 表中有多少行数据
	* @author 杨发荷
	* @time 2016年11月1号
	*/
	int GetTableColumn(const CString &tableName, const CString &field);

	/* @接口 删除数据库中其他数据
	* @参数 CString 指定的表名
	* @参数 MStr M(要删除的字段名，以及值）
	* @返回 bool 如果删除成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	bool DeleteDataOther(const CString &tableName, const MStr &filter);

	/* @接口 根据指定的ID获取权利人的名称
	* @参数 CString 权利人的ID
	* @参数 CString 数据库的路径
	* @返回 CString 权利人的名称
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	CString GetQLRMC(const CString &strID, const CString &strDataBasePath);

	/* @function 这个函数的功能就是获取某个表中包含某个记录数据个数
	* @param tableName 表名
	* @param field_value 这个一个字段名和值所对应起来的map集合
	* @return int 根据field_value查找的结果个数
	*/
	int TableContainFieldValue(const CString &tableName, const MStr &field_value);

	bool addRow(const CString &tableName, const CString &ID, const MStr &valuePair);

	//执行查询 传入 表名 字段列表 查询的字段对应的值
	//返回所有的查询记录 包含每条记录的字段名称和值
	bool hasRow(const CString &tableName, const MStr &field_value, VMStr &selectRow);

	/* @function 获得字段名
	* @param tableName 数据的表名
	* @param fieldName 字段名列表
	* @param isDelete 是否删除原来的数据（默认为删除）
	* @param isMerge 是否合并原来的数据（默认为合并）
	* @author 何星龙
	*/
	bool getFieldNameList(const CString &tableName, VStr &fieldName, VInt &fieldType);

	/* @function 读取数据
	* @param tableName 需要读取的表名
	* @param record 读取的结果
	* @param condition 过滤条件，对应的是字段名和字段值，过滤条件如下：
	* eg. 初始化MCString condition; condition[_T("CBFMC")] = _T("张三");
	* condition[_T("DKBM")] = _T("1213232321321321"); …………
	* @author 杨发荷
	* @time 2016年11月1号
	*/
	bool ReadDataBase(const CString &tableName, VMStr &record, const MStr &condition);

	/* @接口 根据指定的字段和表名获取相应的值
	* @参数 CString 表名
	* @参数 CString 字段名
	* @参数 VStr 获取的值
	* @返回 获取值成功，返回值为true，否则返回值为false
	* @作者 杨发荷
	* @时间 2017年06月28日
	*/
	bool GetValueByField(const CString &tableName, const CString &field, VStr &value);

	/* @function 更新数据库中的一条记录
	* @param tableName 表名
	* @param ID 一条记录
	* @param valuePair
	* @return bool 数据更新成功返回值为true，否则返回值为false
	* @author 何星龙
	*/
	bool updateRow(const CString &tableName, const CString &ID, const MStr &valuePair);

	/* @function 从mdb中读取数据
	 * @param tableName 读取数据的表名
	 * @param strFieldName 读取的字段名
	 * @param data 获取的数据
	 * @return bool 读取输出成功返回值为true，否则返回值为false
	 * @author 何星龙
	 */
	bool ReadDataBase(const CString &tableName, const CString &strFieldName, VMStr &data);

	/* @接口 删除数据库中的记录
	 * @参数 CString 表名
	 * @参数 CString 字段名
	 * @参数 VStr 表中的值与VStr中的值相同，则删除
	 * @返回 
	 * @作者 杨发荷
	 * @时间 2017年07月03日
	 */
	bool DeleteDataBase(const CString &tableName, const CString &field, const VStr &value);

	/* @接口 删除表中的记录
	 * @参数 CString 表名
	 * @参数 CString 字段名
	 * @参数 VStr 与VStr中的值不同的都删掉
	 * @返回 
	 * @作者 杨发荷
	 * @时间 2017年07月03日
	 */
	bool DeleteDataOther(const CString &tableName, const CString &field, const VStr &value);

	/* @function 在数据库中添加一条新的记录，如果这条记录已经在数据库中，那么就仅仅更新这条记录，
	如果数据库中不存在这条记录，那么将这条记录添加到数据库中
	* @param tableName 表名
	* @param record 一条记录
	* @return bool 添加成功返回值为true， 否则返回值为false
	* @author 杨发荷
	* @time 2016年11月1号
	*/
	bool AddOneRecord(const CString &tableName, const MStr &record, const MStr &field_value);

	/* @function 更新指定的表明中的一条记录（根据第一个字段）
	* @param tableName 表名
	* @param record 更新的记录
	* @return bool 更新成功返回值为true， 否则返回值为false
	* @author 杨发荷
	* @time 2016年11月1号
	*/
	bool UpdataRecord(const CString &tableName, const MStr &field_value, const MStr &reocrd);

	/* @接口 删除表中的记录
	 * @参数 CString 表名
	 * @参数 CString 字段名
	 * @参数 CString 字段名对应的值删掉
	 * @返回 
	 * @作者 杨发荷
	 * @时间 2017年07月03日
	 */
	bool DeleteDataBase(const CString &tableName, const CString &field, const CString &value);

	/* @接口 删除表中其他的记录
	 * @参数 CString 表名
	 * @参数 CString 字段名
	 * @参数 CString 被保留的值
	 * @返回 
	 * @作者 杨发荷
	 * @时间 2017年07月03日
	 */
	bool DeleteDataOther(const CString &tableName, const CString &field, const CString &value);

	bool DeleteDataBase_Where(const CString &tableName, VStr &veStrFieldName, VStr &veStrValue);

	/* @接口 根据指定的字段和表名获取相应的值
	 * @参数 CString 表名
	 * @参数 CString 字段名
	 * @参数 VStr 获取的值
	 * @参数 MStr 过滤条件
	 * @返回 获取值成功，返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年06月28日
	 */
	bool GetValueByField(const CString &table, const CString &field, VStr &value, const MStr &filter);

	/* @function 将数据保存到mdb中
	* @param tableName 保存数据的表名
	* @param data 获取的数据
	* @param fieldType
	* @return bool 保存数据成功返回值为true，否则返回值为false
	* @author 何星龙
	*/
	bool saveDataBase(const CString &tableName, const VVStr &data, bool isDelete = true, bool isMerge = true);

	bool saveDataBase_QLR(const CString& tableName, const VVStr &data, const VStr &tbfield, bool ISUPDATEGUID);

	/* @function 查询数据根据跟定的条件
	* @param tableName 数据表名
	* @param fieldName 数据字段名
	* @param selValue 备选值
	* @param data 查询的结果
	* @return bool 查询成功返回值为true，否则返回值为false
	* @author 何星龙
	*/
	bool selectByCondition(const CString &tableName, const CString &strField, const VStr &selValue, VMStr &data);

	/* @function 比较某个表中的一行数据
	 * @param tableName 表名
	 * @param field 字段名
	 * @param value 字段值
	 * @param record 一条记录(仅仅一条记录，也就是一个字段和一个字。我承认当初设计数据结构的时候有问题）
	 * @return bool 如果传入的字段在表中记录与传入与记录中的值相同，返回值为true，否则返回值为false
	 * @author 杨发荷
	 * @time 2016年11月1号
	 */
	bool CompareRecord(const CString &tableName, const CString &field, const CString &value, const MStr &record);

	bool ReadDataBase_Where(const CString &tableName, VStr &veStrFieldName, VStr &veStrValue, CString &strResultFieldName, VMStr &data);

private:

	/* @接口 进入函数前先检查ado是否有效
	 * @返回 bool ado有效返回值为true，否则返回值为false
	 * @作者 杨发荷
	 * @时间 2017年06月28日
	 */
	bool checkAdo();

private:

	CAdoConnection _connect_;//数据库连接

};

#endif