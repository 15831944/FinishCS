#ifndef BUILDDATABASE_H
#define BUILDDATABASE_H

class CDatabaseOperManager;

class BuildDataBase
{
	typedef CDatabaseOperManager DataBase;

public:

	/* @�ӿ� Ĭ�Ϲ��캯��
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	BuildDataBase();

	/* @�ӿ� Ĭ����������
	 * @���� ���
	 * @ʱ�� 2017��11��16��
	 */
	~BuildDataBase();

	
private:

	DataBase *db;

};




#endif /*BUILDDATABASE_H*/