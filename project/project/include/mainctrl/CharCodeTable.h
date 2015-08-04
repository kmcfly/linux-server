/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-03-15
** Name         : CharCodeTable.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CHAR_CODE_TABLE_H_
#define _CHAR_CODE_TABLE_H_

#include <map>
#include <string>

//* �ļ���ʽ
//*           CCT_FILE_HEAD
//*           CCT_SECTION_INDEX
//*           CCT_SECTION_INDEX
//*
//*           CCT_TABLE_HEAD					//no.1
//*           CCT_TABLE_INDEX
//*           CCT_TABLE_INDEX
//*           CCT_TABLE_DATA
//*           CCT_TABLE_DATA
//*
//*           CCT_TABLE_HEAD					//no.2
//*           CCT_TABLE_INDEX
//*           CCT_TABLE_DATA

//*�ļ�ͷ
typedef struct _cct_file_head
{
	char			magic[8];					//tvt_cct
	char			version[8];					//�ļ��汾
	unsigned long	size;						//�ļ�����
	unsigned long	count;						//ת��������
	unsigned long   addr_section;				//��������ƫ��[�����ļ�ͷ]
}CCT_FILE_HEAD;

//��������
typedef struct _cct_section_index
{
	unsigned long   size_table;					//ת�����С
	unsigned long	addr_table;					//ת����ƫ��[�����ļ�ͷ]
}CCT_SECTION_INDEX;

//*ת���� ͷ
typedef struct _cct_table_head
{
	unsigned long	trans_type;					//ת��������
	unsigned long	src_encode;					//Դ��������					
	unsigned long	dest_encode;				//Ŀ�ı�������
	unsigned long	count_index;				//����������
}CCT_TABLE_HEAD;

//*ת���� ����
typedef struct _cct_table_index
{
	unsigned long	low_char;					//������ʼ
	unsigned long	high_char;					//�������
	unsigned long	len;						//������Ŀ
	unsigned long   offset;						//��������ƫ��[��Ա�ͷ]
}CCT_TABLE_INDEX;


//֧��ת���ĸ�ʽ
enum CCT_CODE_TYPE
{
	CCT_CODE_NULL,
	CCT_CODE_UNICODE,
	CCT_CODE_UTF8,
	CCT_CODE_GBK,
};

class CCharCodeTable
{
protected:
	CCharCodeTable();

public:
	virtual ~CCharCodeTable();

	static CCharCodeTable* Instance();
	static int Convert(std::string& outStr, int outCodeType, const char* pInStr, int inCodeType);

	bool Initial(const char* pFilePath);
	void Quit();

	int ConvertBit8(std::string& outStr, int outCodeType, const char* pInStr, int inCodeType);

#ifdef __ENVIRONMENT_WIN32__
	//����ת����
	bool CreateTable(const char* filePath);
#endif

protected:
	typedef struct _cct_table_node
	{
		CCT_TABLE_HEAD		table_head;
		CCT_TABLE_INDEX*	p_table_index;
		unsigned char*		p_data;
		unsigned long		len_data;
	}CCT_TABLE_NODE;

	typedef std::map<unsigned long, CCT_TABLE_NODE*>	CCT_TABLE_MAP;

protected:
	int Translate(std::string& outStr, int outCodeType, const char* pInStr, int inCodeType);
	bool SearchTable(unsigned long& outChar, unsigned long inChar, const CCT_TABLE_NODE* pTableNode);

protected:
	char*				m_p_filepath;
	CCT_FILE_HEAD		m_cct_head;
	CCT_SECTION_INDEX*  m_p_section;

	CCT_TABLE_MAP		m_table_map;

	static std::string  m_default_path;
};

#endif

//end
