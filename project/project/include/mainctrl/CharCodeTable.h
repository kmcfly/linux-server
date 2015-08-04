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

//* 文件格式
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

//*文件头
typedef struct _cct_file_head
{
	char			magic[8];					//tvt_cct
	char			version[8];					//文件版本
	unsigned long	size;						//文件长度
	unsigned long	count;						//转换表总数
	unsigned long   addr_section;				//段区索引偏移[距离文件头]
}CCT_FILE_HEAD;

//段区索引
typedef struct _cct_section_index
{
	unsigned long   size_table;					//转换表大小
	unsigned long	addr_table;					//转换表偏移[距离文件头]
}CCT_SECTION_INDEX;

//*转换表 头
typedef struct _cct_table_head
{
	unsigned long	trans_type;					//转换表类型
	unsigned long	src_encode;					//源编码类型					
	unsigned long	dest_encode;				//目的编码类型
	unsigned long	count_index;				//索引表总数
}CCT_TABLE_HEAD;

//*转换表 索引
typedef struct _cct_table_index
{
	unsigned long	low_char;					//编码起始
	unsigned long	high_char;					//编码结束
	unsigned long	len;						//编码数目
	unsigned long   offset;						//本段数据偏移[相对表头]
}CCT_TABLE_INDEX;


//支持转换的格式
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
	//创建转换表
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
