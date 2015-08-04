/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-05-12
** Name         : StringTable.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _STRING_TABLE_H_
#define _STRING_TABLE_H_
#include "PUB_common.h"
#include "StringDefine.h"
#include <list>
#include <vector>
#include "dvrdvsdef.h"

typedef struct _str_table_file_head
{
	unsigned long	languageID;		//语言ID
	unsigned long	fileLength;		//文件长度
	unsigned long	stringNum;		//字符串数目
}STR_TABLE_FILE_HEAD;

typedef struct _string_head
{
	unsigned short	stringID;		//字符串ID
	unsigned short	stringLen;		//字符串长度
}STRING_HEAD;

typedef struct _string_info
{
	unsigned short	stringID;
	unsigned short	stringLen;
	char			*pString;

	bool operator< (const _string_info& str_info) const
	{
		return stringID < str_info.stringID;
	}
}STRING_INFO;

const unsigned long STR_TABLE_DATA_POS = sizeof(STR_TABLE_FILE_HEAD) + 32;

/***********************************************************************
							文件结构

							文件头

							字符串头（STRING_HEAD）
							字符串
								.
								.
								.
							字符串头（STRING_HEAD）
							字符串
***********************************************************************/

/***********************************************************************
本类思想：
	1、用于管理一种语言对应的所有的字符串，包括存取、内存管理等；
	2、可以使用本类的对象作为语言包文件的生成工具；
	3、可以使用本类的对象作为语言包文件的读取工具；
	4、每次只能增加一种语言的字符串，每次只能打开一种语言的文件包；
	5、添加的字符串内容和编码正确性由外部负责，本类不负责，只关心数据的存取；
	6、添加新的字符串时，直接在文件尾部添加，不需要排序；
	7、打开一个语言包后，把包内的所有的字符串信息加载至内存，使用一个列表
		进行管理；
	8、本类对象在设备上运行时，将不能具有增加等修改文件的功能，只有作为翻译
		及语言包生成工具时才具有修改功能；
	9、本类对象以单件形式存在，界面方案基于此可获得其字符串数据（依据ID）；
	10、每个字符串在文件中的位置都是16字节的倍数起始；
***********************************************************************/
class CStringTable
{
public:
	static CStringTable * Instance();
	~CStringTable();

	bool Initial(unsigned long language, const char *pFilePath);
	void Quit();

	//////////////////////////////////////////////////////////////////////////
	const char * String(const ENUM_STRING_ID stringID);

#if defined(__ENVIRONMENT_WIN32__) && defined(_CREATE_STRING)
	bool Initial(const char *pFilePath);
	bool Save(LANGUAGE languageID, const char *pFilePath);
	const std::list<STRING_INFO>& GetStringList();
	unsigned long GetLanguage();
	bool AddString(const ENUM_STRING_ID stringID, const char *pString, unsigned short stringLen);
#endif

protected:
private:
	CStringTable ();
	//////////////////////////////////////////////////////////////////////////
	bool LoadString();
	void AddToList(const STRING_INFO &strInfo);

	size_t OffSet(unsigned long stringLen);
	//////////////////////////////////////////////////////////////////////////
	FILE *m_pFile;
	char *m_pFilePath;
	unsigned long m_language;
	STR_TABLE_FILE_HEAD m_fileHead;
	std::list<STRING_INFO> m_strList;

	std::vector<STRING_INFO> m_strVec;	//只做读取数据时,使用vector加速读取
};
#endif //_STRING_TABLE_H_
