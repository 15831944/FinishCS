#ifndef _PINYIN_H
#define _PINYIN_H

#include <QTextCodec>

class Pinyin
{
public:
	Pinyin(void);
	~Pinyin(void);

	 void Init();
	 CString getChineseSpell(QString src);
	 QString FirstLetter(int nCode);
	 QString GetPYChar(QString src); 
	 QString GetPYString(QString src);

private:
	QTextCodec* pCode;
};

#endif