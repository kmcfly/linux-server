/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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
	unsigned long	languageID;		//����ID
	unsigned long	fileLength;		//�ļ�����
	unsigned long	stringNum;		//�ַ�����Ŀ
}STR_TABLE_FILE_HEAD;

typedef struct _string_head
{
	unsigned short	stringID;		//�ַ���ID
	unsigned short	stringLen;		//�ַ�������
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
							�ļ��ṹ

							�ļ�ͷ

							�ַ���ͷ��STRING_HEAD��
							�ַ���
								.
								.
								.
							�ַ���ͷ��STRING_HEAD��
							�ַ���
***********************************************************************/

/***********************************************************************
����˼�룺
	1�����ڹ���һ�����Զ�Ӧ�����е��ַ�����������ȡ���ڴ����ȣ�
	2������ʹ�ñ���Ķ�����Ϊ���԰��ļ������ɹ��ߣ�
	3������ʹ�ñ���Ķ�����Ϊ���԰��ļ��Ķ�ȡ���ߣ�
	4��ÿ��ֻ������һ�����Ե��ַ�����ÿ��ֻ�ܴ�һ�����Ե��ļ�����
	5����ӵ��ַ������ݺͱ�����ȷ�����ⲿ���𣬱��಻����ֻ�������ݵĴ�ȡ��
	6������µ��ַ���ʱ��ֱ�����ļ�β����ӣ�����Ҫ����
	7����һ�����԰��󣬰Ѱ��ڵ����е��ַ�����Ϣ�������ڴ棬ʹ��һ���б�
		���й���
	8������������豸������ʱ�������ܾ������ӵ��޸��ļ��Ĺ��ܣ�ֻ����Ϊ����
		�����԰����ɹ���ʱ�ž����޸Ĺ��ܣ�
	9����������Ե�����ʽ���ڣ����淽�����ڴ˿ɻ�����ַ������ݣ�����ID����
	10��ÿ���ַ������ļ��е�λ�ö���16�ֽڵı�����ʼ��
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

	std::vector<STRING_INFO> m_strVec;	//ֻ����ȡ����ʱ,ʹ��vector���ٶ�ȡ
};
#endif //_STRING_TABLE_H_
