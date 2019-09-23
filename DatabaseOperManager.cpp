#include "stdafx.h"

#ifndef _RIGHTS_HOLDER_MANAGER_
#include "DatabaseOperManager.h"
#endif /*_RIGHTS_HOLDER_MANAGER_*/

#define CHECKADO(FLAG) if(TRUE != FLAG) return false;

CDatabaseOperManager::CDatabaseOperManager()
{

}

CDatabaseOperManager::~CDatabaseOperManager(void)
{
	if(_connect_.IsOpen())
		_connect_.Close();
}

bool CDatabaseOperManager::initialConnect(CString strPath)
{
	if(false == isFileExsist(strPath))
		return false;
	return _connect_.ConnectAccess(strPath) == TRUE;
}

bool CDatabaseOperManager::isFileExsist(const CString &strPath)
{
	TCHAR szPath[MAX_PATH] = {0};
	_stprintf(szPath, _T("%s"), strPath);
	if(_taccess(szPath, 0) != 0) return false;
	return true;
}

bool CDatabaseOperManager::ReadDataBase(const CString &table, const CString &field, CString &value)
{
	VStr vals;
	GetValueByField(table, field, vals);
	if(vals.size() == 0) return false;
	value = vals.at(0);
	return true;
}

bool CDatabaseOperManager::ReadDataBase(const CString &table, const CString &field, const MStr &filter, CString &data)
{
	VStr values;
	if(false == GetValueByField(table, field, values, filter)) return false;
	if(values.size() == 0) return false;
	data = values.at(0); return true;
}

bool CDatabaseOperManager::ReadDataBase(const CString &tableName, const CString &strFieldName, VMStr &data)
{
	data.clear();
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select * from %s order by %s"), tableName, strFieldName);
	if(record.Open(strSQL) == FALSE)
		return false;
	int nCount = record.GetFieldsCount();
	MStr row; CString strValue, strField;
	while(record.IsEOF() == FALSE)
	{
		row.clear();
		for(int i = 0; i < nCount; i++)
		{
			strField = record.GetFieldName(i);
			record.GetCollect(i, strValue);
			row[strField.MakeUpper()] = strValue;
		}
		if(row.size() > 0)
			data.push_back(row);
		record.MoveNext();
	}
	record.Close();
	if(data.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::ReadDataBase_all(const CString& tableName, VMStr &data)
{
	data.clear();
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select * from %s"), tableName);
	if(record.Open(strSQL) == FALSE)
		return false;
	int nCount = record.GetFieldsCount();
	MStr row; CString strField, strValue;
	while(record.IsEOF() == FALSE)
	{
		row.clear();
		for(int i = 0; i < nCount; i++)
		{
			strField = record.GetFieldName(i);
			record.GetCollect(i, strValue);
			row[strField.MakeUpper()] = strValue;
		}
		if(row.size() > 0)
			data.push_back(row);
		record.MoveNext();
	}
	record.Close();
	if(data.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::ReadDataBase_LastRow(const CString& tableName, MStr &data)
{
	data.clear();
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select * from %s"), tableName);
	if(record.Open(strSQL) == FALSE)
		return false;
	int nCount = record.GetFieldsCount();
	CString strField, strValue;
	if(record.MoveLast())
	{
		record.MovePrevious();
		for(int i = 0; i < nCount; i++)
		{
			strField = record.GetFieldName(i);
			record.GetCollect(i, strValue);
			data[strField.MakeUpper()] = strValue;
		}
	}
	record.Close();
	if(data.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::getFieldNameList(const CString& tableName, VStr &fieldName)
{
	if(tableName.IsEmpty())
		return false;
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select * from %s"), tableName);
	if(record.Open(strSQL) == FALSE)
		return false;
	int nCount = record.GetFieldsCount();
	for(int i = 0; i < nCount; i++)
	{
		fieldName.push_back(record.GetFieldName(i));
	}
	record.Close();
	if(fieldName.size() == nCount)
		return true;
	return false;
}

CString CDatabaseOperManager::GetQLRMC(const CString &strID, const CString &strDataBasePath)
{
	CDatabaseOperManager dataOp;
	dataOp.initialConnect(strDataBasePath);
	vector<CString> veStrField;
	vector<CString> veStrValue;
	veStrField.push_back(_T("ID"));
	veStrValue.push_back(strID);
	VMStr data;  MSIter itData;
	dataOp.ReadDataBase_Where(CString(_T("QLR")), veStrField, veStrValue, CString(_T("QLRMC")), data);
	if(data.size() > 0)
	{
		itData = data[0].begin();
		if(itData != data[0].end())
			return itData->second;
	}
	CString strTemp = _T("");
	return strTemp;
}

bool CDatabaseOperManager::clearTable(const CString &table)
{
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("delete from %s"), table);
	if(record.Open(strSQL) == FALSE) return false;
	return true;
}

CString CDatabaseOperManager::GetQLRMC(const CString &strID)
{
	CDatabaseOperManager dataOp;
	dataOp.initialConnect();
	VStr veStrField, veStrValue;
	veStrField.push_back(_T("ID"));
	veStrValue.push_back(strID);
	VMStr data;  MSIter itData;
	dataOp.ReadDataBase_Where(CString(_T("QLR")), veStrField, veStrValue, CString(_T("QLRMC")), data);
	if(data.size() > 0)
	{
		itData = data[0].begin();
		if(itData != data[0].end())
			return itData->second;
	}
	CString strTemp = _T("");
	return strTemp;
}

bool CDatabaseOperManager::DeleteDataBase_Where(const CString &tableName, VStr &veStrFieldName, VStr &veStrValue)
{
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	if(veStrFieldName.size() == 0)
		return false;
	strSQL.Format(_T("delete * from %s"), tableName);
	for(int i = 0; i < (int)veStrFieldName.size(); i++)
	{
		if(i == 0)
			strSQL += _T(" where ");
		strSQL += veStrFieldName[i] + _T(" = ") + _T("'") + veStrValue[i] + _T("'");
		if(i != (veStrFieldName.size() - 1))
			strSQL += _T(" and ");
	}
	if(record.Open(strSQL) == FALSE)
		return false;
	record.Close();
	return true;
}

bool CDatabaseOperManager::DeleteDataBase_All(const CString &tableName)
{
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	if(tableName.IsEmpty())
		return false;
	strSQL.Format(_T("delete * from %s where 1 = 1"), tableName);
	if(record.Open(strSQL) == FALSE)
		return false;
	record.Close();
	return true;
}

bool CDatabaseOperManager::ReadDataBase_Where(const CString &tableName, VStr &veStrFieldName, VStr &veStrValue, CString &strResultFieldName, VMStr &data)
{
	data.clear();
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	if(veStrFieldName.size() == 0)
		return false;
	strSQL.Format(_T("select %s from %s"), strResultFieldName, tableName);
	for(int i = 0; i < (int)veStrFieldName.size(); i++)
	{
		if(i == 0)
			strSQL += _T(" where ");
		strSQL += veStrFieldName[i] + _T(" = ") + _T("'") + veStrValue[i] + _T("'");
		if(i != (veStrFieldName.size() - 1))
			strSQL += _T(" and ");
	}
	if(record.Open(strSQL) == FALSE)
		return false;
	int nCount = record.GetFieldsCount();
	MStr row; CString strField, strValue;
	while(record.IsEOF() == FALSE)
	{
		row.clear();
		for(int i = 0; i < nCount; i++)
		{
			strField = record.GetFieldName(i);
			record.GetCollect(i, strValue);
			row[strField.MakeUpper()] = strValue;
		}
		if(row.size() > 0)
			data.push_back(row);
		record.MoveNext();
	}
	record.Close();
	if(data.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::createTable(const CString &tableName, VStr &veStrFieldName)
{
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	if(veStrFieldName.size() == 0)
		return false;
	strSQL.Format(_T("CREATE TABLE %s ("), tableName);
	for(int i = 0; i < (int)veStrFieldName.size(); i++)
	{
		strSQL += veStrFieldName[i] + _T(" LongText");
		if(i == (veStrFieldName.size() - 1))
			strSQL += _T("");
		else
			strSQL += _T(",");
	}
	strSQL += _T(")");
	if(record.Open(strSQL) == FALSE)
		return false;
	record.Close();
	return true;
}

bool CDatabaseOperManager::DeleteDataOther(const CString &tableName, const CString &field, const CString &value)
{
	CString strSQL;
	strSQL.Format(_T("DELETE * FROM %s WHERE %s <> '%s'"), tableName, field, value);
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	BOOL flag = CARS.Open(strSQL);
	CARS.Close();
	return flag == TRUE;
}

bool CDatabaseOperManager::DeleteDataOther(const CString &tableName, const CString &field, const VStr &values)
{
	CString strSQL;
	strSQL.Format(_T("DELETE * FROM %s WHERE "), tableName);
	CString condition, value;
	int len = values.size();
	if(len == 0) return false;
	condition.Format(_T("%s <> '%s'"), field, values.at(0));
	strSQL.Append(condition);
	for(int idx = 1; idx < len; ++idx)
	{
		condition.Format(_T(" and %s <> '%s'"), field, values.at(idx));
		strSQL.Append(condition);
	}
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	BOOL flag = CARS.Open(strSQL);
	CARS.Close();
	return flag == TRUE;
}

bool CDatabaseOperManager::DeleteDataOther(const CString &tableName, const MStr &filter)
{
	CString strSQL;
	strSQL.Format(_T("DELETE * FROM %s WHERE "), tableName);
	MSCIter it = filter.begin();
	CString condition, field, value;
	field = it->first; value = it->second;
	CString temp; temp.Format(_T("%s <> '%s'"), field, value);
	strSQL.Append(temp);
	for(; it != filter.end(); ++it)
	{
		field = it->first; value = it->second;
		temp.Format(_T(" and %s <> '%s'"), field, value);
		strSQL.Append(temp);
	}
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	BOOL flag = CARS.Open(strSQL);
	CARS.Close();
	return flag == TRUE;
}

bool CDatabaseOperManager::DeleteDataBase(const CString &tableName, const CString &field, const VStr &value)
{
	int count = value.size();
	for(int idx = 0; idx < count; ++idx)
	{
		if(!DeleteDataBase(tableName, field, value.at(idx)))
			return false;
	}
	return true;
}

bool CDatabaseOperManager::DeleteDataBase(const CString &tableName, const CString &field, const CString &value)
{
	CString strSQL;
	strSQL.Format(_T("DELETE FROM %s WHERE %s = '%s'"), tableName, field, value);
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	BOOL flag = CARS.Open(strSQL);
	CARS.Close();
	return flag == TRUE;
}

bool CDatabaseOperManager::getFieldNameList(const CString &tableName, VStr &fieldName, VInt &fieldType)
{
	fieldType.clear();
	fieldName.clear();
	if(tableName.IsEmpty())
		return false;
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select * from %s"), tableName);
	if(record.Open(strSQL) == FALSE)
		return false;
	int nCount = record.GetFieldsCount();
	for(int i = 0; i < nCount; i++)
	{
		fieldName.push_back(record.GetFieldName(i));
		ADODB::DataTypeEnum type = record.GetFieldType(i);
		int nType = 0;
		//这里类型只是区分了字符串和数字
		if(ADODB::adVarWChar == type || ADODB::adWChar == type
			|| ADODB::adVarChar == type || ADODB::adBSTR == type
			|| ADODB::adChar == type || ADODB::adBSTR == type
			|| ADODB::adLongVarChar == type || ADODB::adLongVarWChar == type
			)
			nType = 0;
		else
			nType = 1;
		fieldType.push_back(nType);
	}
	record.Close();
	if(fieldName.size() == nCount)
		return true;
	return false;
}
//保存数据
bool CDatabaseOperManager::saveDataBase(const CString &tableName, const VVStr &data, bool isDelete, bool isMerge)
{
	if(data.size() <= 0)
		return false;
	CString strField;
	strField.Format(_T("insert into %s values("), tableName);
	VSIter iter;
	CString strSql;
	if(isDelete)
	{
		strSql.Format(_T("delete from %s"), tableName);
		_connect_.Execute(strSql);//清空数据
	}
	VStr row; CString strSQL, strData, str;
	for(int i = 0; i < (int)data.size(); i++)
	{
		row = data.at(i);
		iter = row.begin();
		for(iter; iter != row.end(); iter++)
		{
			str.Format(_T("\'%s\',"), (*iter));
			strData += str;
		}
		if(strData.IsEmpty())
			continue;
		strData = strData.Left(strData.GetLength() - 1);
		strData += _T(")");
		strSQL.Format(_T("%s%s"), strField, strData);
		_connect_.Execute(strSQL);
	}
	return true;
}

bool CDatabaseOperManager::saveDataBase_QLR(const CString& tableName, const VVStr &data, const VStr &tbfield, bool ISUPDATEGUID)
{
	if(data.size() <= 0)
		return false;
	if(data.at(0).size() < 5)
	{
		acutPrintf(_T("权利人模板表有误"));
		return false;
	}
	CAdoRecordSet rcst(&_connect_);
	CString strUPDate;
	strUPDate.Format(_T("UPDATE %s SET "), tableName);

	std::vector<CString>::iterator iter;
	CString strSql;
	for(int i = 0; i < (int)data.size(); i++)
	{
		CString querySQL;
		querySQL.Format(_T("select * from %s where QLRMC='%s' and ZJH = '%s'"), tableName, data.at(i)[2], data.at(i)[5]); //[2]为权利人名称[5]为证件号 隐患
		//_connect_.Execute(querySQL);
		if(rcst.Open(querySQL) == FALSE)
		{
			acutPrintf(_T("执行sql语句失败！"));
			return false;
		}
		int nCount = rcst.GetRecordCount();

		if(nCount == 0)//执行inset
		{
			CString querySQL2;
			querySQL2.Format(_T("select *from %s where QLRMC='%s'"), tableName, data.at(i)[2]); //[2]为权利人名称[5]为证件号 隐患
			//_connect_.Execute(querySQL);
			if(rcst.Open(querySQL2) == FALSE)
			{
				return false;
			}
			int nCount2 = rcst.GetRecordCount();
			if(nCount2 == 0)//真的insert
			{
				std::vector<CString> row = data.at(i);
				iter = row.begin();
				CString strData;
				for(iter; iter != row.end(); iter++)
				{
					CString str;
					str.Format(_T("\'%s\',"), (*iter));
					strData += str;
				}
				if(strData.IsEmpty())
					continue;
				strData = strData.Left(strData.GetLength() - 1);
				strData += _T(")");
				CString strSQL;
				CString strField;
				strField.Format(_T("insert into %s values("), tableName);
				strSQL.Format(_T("%s%s"), strField, strData);
				//acutPrintf(_T("\n%s\n") , strSQL);
				_connect_.Execute(strSQL);
			}
			if(nCount2 >= 1)//执行更新
			{
				rcst.MoveFirst();

				CString strData;
				int IDX = 0;
				if(!ISUPDATEGUID)IDX = 1;
				for(; IDX < (int)tbfield.size(); IDX++)
				{
					rcst.PutCollect(tbfield[IDX], data.at(i)[IDX]);
				}
				rcst.UpdateBatch();
			}
		}
		if(nCount >= 1)//执行udate
		{
			rcst.MoveFirst();
			int IDX = 0;
			if(!ISUPDATEGUID)IDX = 1;
			CString strData;
			for(; IDX < (int)tbfield.size(); IDX++)
			{
				rcst.PutCollect(tbfield[IDX], data.at(i)[IDX]);
			}
			rcst.UpdateBatch();
		}
	}
	return true;
}

//更新一条记录
//如果记录存在 更新指定的字段
//如果记录不存在 增加记录
bool CDatabaseOperManager::updateRow(const CString& tableName, const CString& ID, const MStr &valuePair)
{
	if(tableName.IsEmpty() || ID.IsEmpty() || valuePair.size() <= 0)
		return false;
	if(_connect_.IsOpen() == FALSE)
		return false;
	//判断是否存在

	CString IDval;
	MStr::const_iterator iter1 = valuePair.begin();
	for(iter1; iter1 != valuePair.end(); iter1++)
	{
		CString strFieldName = iter1->first, strValue = iter1->second;
		if(strFieldName == ID)
		{
			IDval = strValue;
			break;
		}
	}

	if(hasRow(tableName, IDval) == false)
	{
		addRow(tableName, ID, valuePair);
	}
	else
	{
		//更新字段
		CString strSQL;
		strSQL.Format(_T("UPDATE %s SET "), tableName);
		MStr::const_iterator iter = valuePair.begin();
		for(iter; iter != valuePair.end(); iter++)
		{
			CString strFieldName = iter->first, strValue = iter->second;
			CString strItem;
			if(iter != valuePair.begin())
				strItem.Format(_T(",%s = '%s'"), strFieldName, strValue);
			else
				strItem.Format(_T("%s = '%s'"), strFieldName, strValue);
			strSQL += strItem;
		}
		CString str;
		str.Format(_T(" WHERE ID = '%s'"), ID);
		strSQL += str;
		_connect_.Execute(strSQL);
	}
	return true;
}

bool CDatabaseOperManager::hasRow(const CString& tableName, const CString& ID)
{
	if(tableName.IsEmpty() || ID.IsEmpty())
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	if(_tcscmp(tableName, _T("ROOM")) == 0 || _tcscmp(tableName, _T("FLOOR")) == 0)
		strSQL.Format(_T("select count(*) as MCOUNT from %s where GUID = '%s'"), tableName, ID);
	else
		strSQL.Format(_T("select count(*) as MCOUNT from %s where ID = '%s'"), tableName, ID);

	if(FALSE == record.Open(strSQL))
		return false;
	_variant_t vCount;
	record.GetCollect(_T("MCOUNT"), vCount);
	record.Close();
	if(vCount.lVal <= 0)
		return false;
	return true;
}

bool CDatabaseOperManager::addRow(const CString &tableName, const CString &ID, const MStr &valuePair)
{
	if(tableName.IsEmpty() || ID.IsEmpty() || valuePair.size() <= 0)
		return false;
	if(_connect_.IsOpen() == FALSE)
		return false;
	//获得表字段
	std::vector<CString> field_name_list;
	if(getFieldNameList(tableName, field_name_list) == false)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("insert into %s values("), tableName);
	for(int i = 0; i < (int)field_name_list.size(); i++)
	{
		CString strFieldName = field_name_list.at(i);
		CString strValue = _T("");
		//获得值
		MStr::const_iterator iter = valuePair.find(strFieldName.MakeUpper());
		if(iter != valuePair.end())
			strValue = iter->second;
		CString strItem;
		if(i != field_name_list.size() - 1)
			strItem.Format(_T("'%s',"), strValue);
		else
			strItem.Format(_T("'%s')"), strValue);
		strSQL += strItem;
	}
	_connect_.Execute(strSQL);
	return true;
}

bool CDatabaseOperManager::selectByCondition(const CString& tableName, const CString &strField, const VStr &selValue, VMStr &selectRow)
{
	selectRow.clear();
	if(_connect_.IsOpen() == FALSE)
		return false;
	if(tableName.IsEmpty() || selValue.size() <= 0)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select * from %s where %s in("), tableName, strField);
	bool bSQL = false;
	for(int i = 0; i < (int)selValue.size(); i++)
	{
		CString str;
		str.Format(_T("'%s'"), selValue.at(i));
		if(i < (int)selValue.size() - 1)
			str += _T(",");
		else
			str += _T(")");
		strSQL += str;
		bSQL = true;
	}
	if(false == bSQL) return false;
	if(FALSE == record.Open(strSQL)) return false;
	MStr itemMap; CString field, value;
	while(record.IsEOF() == FALSE)
	{
		itemMap.clear();
		for(int i = 0; i < record.GetFieldsCount(); i++)
		{
			field = record.GetFieldName(i).MakeUpper();
			record.GetCollect(i, value);
			itemMap[field] = value;
		}
		if(itemMap.size() > 0)
			selectRow.push_back(itemMap);
		record.MoveNext();
	}
	if(selectRow.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::hasRow(const CString& tableName, const MStr &field_value, VMStr &selectRow)
{
	selectRow.clear();
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	if(tableName.IsEmpty() || field_value.size() <= 0)
		return false;
	CString strSQL;
	strSQL.Format(_T("select * from %s where "), tableName);
	std::map<CString, CString>::const_iterator iter = field_value.begin();
	int nCount = -1;
	bool bSQL = false;
	for(; iter != field_value.end(); iter++)
	{
		nCount++;
		CString strFieldName = iter->first;
		strFieldName.TrimLeft();
		strFieldName.TrimRight();
		if(strFieldName.GetLength() <= 0)
			continue;
		CString strValue = iter->second;
		CString strItem;
		strItem.Format(_T("%s = '%s' "), strFieldName, strValue);
		if(nCount < (int)field_value.size() - 1)
			strItem += _T("AND ");
		strSQL += strItem;
		bSQL = true;
	}
	if(false == bSQL || false == record.Open(strSQL)) return false;
	MStr itemMap; CString strField, strValue;
	for(int i = 0; i < record.GetFieldsCount(); i++)
	{
		strField = record.GetFieldName(i).MakeUpper();
		record.GetCollect(i, strValue);
		itemMap[strField] = strValue;
	}
	if(itemMap.size() > 0)
		selectRow.push_back(itemMap);
	record.Close();
	if(selectRow.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::AddOneRecord(const CString &tableName, const MStr &record, const MStr &field_value)
{
	if(tableName.IsEmpty() || record.size() == 0) return false;
	int nCount = TableContainFieldValue(tableName, field_value);
	if(nCount <= 0)
		return InsertRecord(tableName, record);
	else if(nCount == 1)
		return UpdataRecord(tableName, field_value, record);
	return false;
}

void CDatabaseOperManager::close()
{
	if(_connect_.IsOpen())
		_connect_.Close();
}

bool CDatabaseOperManager::InsertRecord(const CString &tableName, const MStr &record)
{
	if(tableName.IsEmpty() || record.size() == 0)
		return false;
	if(_connect_.IsOpen() == FALSE) return false;
	//获得表字段
	VStr field_name_list;
	if(getFieldNameList(tableName, field_name_list) == false)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	CString strSQL, strField, strValue, strItem;
	strSQL.Format(_T("insert into %s values("), tableName);
	for(int i = 0; i < (int)field_name_list.size(); i++)
	{
		strField = field_name_list.at(i);
		MStr::const_iterator iter = record.find(strField.MakeUpper());
		if(iter != record.end())
			strValue = iter->second;
		if(i != field_name_list.size() - 1)
			strItem.Format(_T("'%s',"), strValue);
		else
			strItem.Format(_T("'%s')"), strValue);
		strSQL += strItem; strValue = _T("");
	}
	_connect_.Execute(strSQL);
	CARS.Close();
	return true;
}

bool CDatabaseOperManager::UpdataRecord(const CString &tableName, const MStr &field_value, const MStr &record)
{
	if(tableName.IsEmpty() || record.size() == 0)
		return false;
	if(_connect_.IsOpen() == FALSE) return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	CString strSQL, _field, _value, fieldvalue;
	strSQL.Format(_T("UPDATE %s SET "), tableName);
	for(MStr::const_iterator it = record.begin(); it != record.end(); ++it)
	{
		_field = it->first; _value = it->second;
		if(it == record.begin()) fieldvalue.Format(_T("%s = '%s'"), _field, _value);
		else fieldvalue.Format(_T(", %s = '%s'"), _field, _value);
		strSQL += fieldvalue;
	}
	if(field_value.size() != 0)
	{
		MStr::const_iterator iter = field_value.begin();
		CString strSel;
		strSel.Format(_T(" where %s = '%s'"), iter->first, iter->second);
		strSQL += strSel;
		for(++iter; iter != field_value.end(); ++iter)
		{
			strSel.Format(_T(" and %s = '%s'"), iter->first, iter->second);
			strSQL += strSel;
		}
	}
	_connect_.Execute(strSQL);
	CARS.Close();
	return true;
}

bool CDatabaseOperManager::InsertRecord(const CString &tableName, const VMStr &records)
{
	int len = (int)records.size();
	for(int idx = 0; idx < len; ++idx)
	{
		MStr record = records[idx];
		if(true == InsertRecord(tableName, record)) continue;
		CString error(_T("\n 新记录插入失败："));
		for(MSIter it = record.begin(); it != record.end(); ++it)
		{
			CString info;
			info.Format(_T("字段%s：%s"), it->first, it->second);
			error += info;
		}
		acutPrintf(error);
	}
	return true;
}

int CDatabaseOperManager::GetTableColumn(const CString &tableName, const CString &field)
{
	if(_connect_.IsOpen() == FALSE)
		return 0;
	bool flag = false;
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select * from %s order by %s"), tableName, field);
	record.Open(strSQL);
	int nCount = record.GetFieldsCount();
	record.Close();
	return nCount;
}

bool CDatabaseOperManager::DeleteReocrd(const CString &tableName, const MStr &filter)
{
	CString strSQL; CString condition(_T("%s = '%s'"));
	strSQL.Format(_T("DELETE FROM %s WHERE "), tableName);
	if(filter.size() == 0) return false;
	MSCIter it = filter.begin(); CString temp(condition);
	temp.Format(condition, it->first, it->second);
	strSQL.Append(temp);
	for(++it; it != filter.end(); ++it)
	{
		temp.Format(condition, it->first, it->second);
		strSQL.Append(_T(" and ") + temp);
	}
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	BOOL flag = CARS.Open(strSQL);
	CARS.Close();
	return flag == TRUE;
}

int CDatabaseOperManager::GetTableRecordCount(const CString &tableName)
{
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);//暂时写定这个库，本应该是临时数据库
	CString SQL; SQL.Format(_T("select * from %s"), tableName);
	if(FALSE == record.Open(SQL)) return false;
	int nlen = record.GetRecordCount();
	return nlen;
}

int CDatabaseOperManager::GetTableRow(const CString &tableName)
{
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select count(*) as MCOUNT from %s"), tableName);
	if(FALSE == record.Open(strSQL)) return 0;
	_variant_t vCount;
	record.GetCollect(_T("MCOUNT"), vCount);
	record.Close();
	return vCount.iVal;
}

int CDatabaseOperManager::TableContainFieldValue(const CString &tableName, const MStr &field_value)
{
	if(tableName.IsEmpty() || field_value.size() == 0) return 0;
	if(_connect_.IsOpen() == FALSE) return false;
	CString strSQL, strSel; MStr::const_iterator it = field_value.begin();
	strSQL.Format(_T("select count(*) as MCOUNT from %s where %s = '%s'"), tableName, it->first, it->second);
	if(field_value.size() > 1)
	{
		for(; it != field_value.end(); ++it)
		{
			strSel.Format(_T(" and %s = '%s'"), it->first, it->second);
			strSQL += strSel;
		}
	}
	CAdoRecordSet record;
	record.SetAdoConnection(&_connect_);
	if(FALSE == record.Open(strSQL))
		return false;
	int nCount = record.GetRecordCount();
	_variant_t vCount;
	record.GetCollect(_T("MCOUNT"), vCount);
	record.Close();
	return vCount.lVal;
}

bool CDatabaseOperManager::CompareRecord(const CString &tableName, const CString &field, const CString &value, const MStr &record)
{
	return true;
}

bool CDatabaseOperManager::ReadDataBase(const CString &tableName, VMStr &record)
{
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);

	CString strSQL;
	strSQL.Format(_T("select * from %s "), tableName);
	if(CARS.Open(strSQL) == FALSE)
		return false;
	int nCount = CARS.GetFieldsCount();
	MStr row_value; CString strField, strValue;
	while(CARS.IsEOF() == FALSE)
	{
		row_value.clear();
		for(int i = 0; i < nCount; i++)
		{
			strField = CARS.GetFieldName(i);
			CARS.GetCollect(i, strValue);
			row_value[strField.MakeUpper()] = strValue;
		}
		if(row_value.size() > 0)
			record.push_back(row_value);
		CARS.MoveNext();
	}
	CARS.Close();
	if(record.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::ReadDataBase(const CString &tableName, const CString &field, MStr &record)
{
	VMStr records; MStr condition; condition[_T("名称")] = field;
	bool flag = ReadDataBase(tableName, records, condition);
	if(false == flag || records.size() == 0) return false;
	record = records[0]; return flag;
}

bool CDatabaseOperManager::ReadDataBase(const CString &tableName, VMStr &record, const MStr &condition)
{
	if(_connect_.IsOpen() == FALSE)
		return false;
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	MStr::const_iterator it = condition.begin();
	CString filter; filter.Format(_T("where %s = '%s' "), it->first, it->second);
	for(++it; it != condition.end(); ++it)
	{
		CString val;
		val.Format(_T("and %s = '%s'"), it->first, it->second);
		filter += val;
	}

	CString strSQL;
	strSQL.Format(_T("select * from %s "), tableName);
	strSQL += filter;
	if(CARS.Open(strSQL) == FALSE)
		return false;
	int nCount = CARS.GetFieldsCount();
	MStr row_value; CString strValue, strField;
	while(CARS.IsEOF() == FALSE)
	{
		row_value.clear();
		for(int i = 0; i < nCount; i++)
		{
			strField = CARS.GetFieldName(i);
			CARS.GetCollect(i, strValue);
			row_value[strField.MakeUpper()] = strValue;
		}
		if(row_value.size() > 0)
			record.push_back(row_value);
		CARS.MoveNext();
	}
	CARS.Close();
	if(record.size() > 0)
		return true;
	return false;
}

bool CDatabaseOperManager::DeleteTable(const CString &tableName)
{
	CString strSQL;
	strSQL.Format(_T("DELETE FROM %s"), tableName);
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	BOOL flag = CARS.Open(strSQL);
	CARS.Close();
	return flag == TRUE;
}

bool CDatabaseOperManager::GetValueByField(const CString &tableName, const CString &field, VStr &value)
{
	CHECKADO(_connect_.IsOpen());
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	CString strSQL;
	strSQL.Format(_T("select %s from %s "), field, tableName);
	CHECKADO(CARS.Open(strSQL));
	int nCount = CARS.GetRecordCount();
	CString val; int i = 0;
	while(CARS.IsEOF() == FALSE)
	{
		CARS.GetCollect(i, val);
		if(find(value.begin(), value.end(), val) != value.end())
		{
			CARS.MoveNext();
			continue;
		}
		value.push_back(val);
		CARS.MoveNext();
	}
	CARS.Close();
	return true;
}

bool CDatabaseOperManager::GetValueByField(const CString &table, const CString &field, VStr &value, const MStr &filter)
{
	CString strSQL;
	strSQL.Format(_T("SELECT %s FROM %s where "), field, table);
	MStr::const_iterator it = filter.begin(); CString tmp;
	tmp.Format(_T("%s = '%s'"), it->first, it->second);
	strSQL.Append(tmp);
	for(++it; it != filter.end(); ++it)
	{
		tmp.Format(_T("and %s = '%s'"), it->first, it->second);
		strSQL.Append(tmp);
	}
	CAdoRecordSet CARS;
	CARS.SetAdoConnection(&_connect_);
	if(CARS.Open(strSQL) == FALSE)
		return false;

	CString val; int i = 0;
	while(CARS.IsEOF() == FALSE)
	{
		CARS.GetCollect(i, val);
		value.push_back(val);
		CARS.MoveNext();
	}
	CARS.Close();
	return true;
}

bool CDatabaseOperManager::checkAdo()
{
	if(_connect_.IsOpen() == FALSE)
		return false;
	return true;
}