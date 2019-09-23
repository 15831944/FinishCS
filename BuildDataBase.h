#ifndef BUILDDATABASE_H
#define BUILDDATABASE_H

class CDatabaseOperManager;

class BuildDataBase
{
	typedef CDatabaseOperManager DataBase;

public:

	/* @接口 默认构造函数
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	BuildDataBase();

	/* @接口 默认析构函数
	 * @作者 杨发荷
	 * @时间 2017年11月16号
	 */
	~BuildDataBase();

	
private:

	DataBase *db;

};




#endif /*BUILDDATABASE_H*/