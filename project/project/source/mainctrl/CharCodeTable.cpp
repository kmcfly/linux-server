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

#include "CharCodeTable.h"
#include "stdio.h"
#include "string.h"
#include <assert.h>

#ifdef __ENVIRONMENT_WIN32__
#include <afxext.h>
#endif

#define CCT_FILE_MAGIC		"tvt_cct"
#define CCT_FILE_VERSION	"1.0"

#ifdef __ENVIRONMENT_WIN32__
std::string CCharCodeTable::m_default_path = "\\ui\\code_table.dat";
#else
std::string CCharCodeTable::m_default_path = "/ui/code_table.dat";
#endif

CCharCodeTable* CCharCodeTable::Instance()
{
	static CCharCodeTable s_cct;
	return &s_cct;
}

int CCharCodeTable::Convert(std::string& outStr, int outCodeType, const char* pInStr, int inCodeType)
{
	return Instance()->ConvertBit8(outStr, outCodeType, pInStr, inCodeType);
}

CCharCodeTable::CCharCodeTable()
{
	m_p_filepath = NULL;
	m_p_section = NULL;
}

CCharCodeTable::~CCharCodeTable()
{

}

bool CCharCodeTable::Initial(const char *pFilePath)
{
	assert(NULL != pFilePath);

	m_p_filepath = new char [512];
	memset(m_p_filepath, 0, sizeof(char) * 512);

	if (NULL != pFilePath)
	{
		//方便linux下查找文件路径,末端都没带'\'或'/'
		strncpy(m_p_filepath, pFilePath, 512);
		int len = strlen(m_p_filepath);
#ifdef __ENVIRONMENT_WIN32__
		if ('/' == m_p_filepath[len - 1] || '\\' == m_p_filepath[len - 1] )
		{
			m_p_filepath[len - 1] = '\0';
		}
		strcat(m_p_filepath, m_default_path.c_str());
#else
		if ('/' == m_p_filepath[len - 1])
		{
			m_p_filepath[len - 1] = '\0';
		}
		strcat(m_p_filepath, m_default_path.c_str());
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strncpy(m_p_filepath, m_default_path.c_str(), 512);
#else
		strncpy(m_p_filepath, m_default_path.c_str(), 512);
#endif
	}

	FILE *pf = fopen(m_p_filepath, "rb");
	if(!pf)
	{
		delete [] m_p_filepath;
		m_p_filepath = NULL;
		return false;
	}

	//文件头
	memset(&m_cct_head, 0, sizeof(m_cct_head));

	fseek(pf, 0, SEEK_SET);
	if (1 != fread(&m_cct_head, sizeof(m_cct_head), 1, pf))
	{
		goto error;
	}

	//段区索引
	assert(m_cct_head.count < 64);
	m_p_section = new CCT_SECTION_INDEX[m_cct_head.count + 1];
	memset(m_p_section, 0, sizeof(CCT_SECTION_INDEX) * (m_cct_head.count + 1));

	fseek(pf, m_cct_head.addr_section, SEEK_SET);
	if(m_cct_head.count != fread(m_p_section, sizeof(CCT_SECTION_INDEX), m_cct_head.count, pf))
	{
		goto error;
	}

	//转换表
	for(unsigned long i = 0; i < m_cct_head.count; i++)
	{
		CCT_TABLE_NODE* p_table_node = new CCT_TABLE_NODE;
		memset(p_table_node, 0, sizeof(CCT_TABLE_NODE));

		fseek(pf, m_p_section[i].addr_table, SEEK_SET);
		if(1 != fread(&(p_table_node->table_head), sizeof(CCT_TABLE_HEAD), 1, pf))
		{
			delete p_table_node;
			goto error;
		}

		unsigned long cout_index = p_table_node->table_head.count_index;
		p_table_node->p_table_index = new CCT_TABLE_INDEX[cout_index + 1];
		memset(p_table_node->p_table_index, 0, sizeof(CCT_TABLE_INDEX) * (cout_index + 1));

		if(cout_index != fread(p_table_node->p_table_index, sizeof(CCT_TABLE_INDEX), cout_index, pf))
		{
			delete [] p_table_node->p_table_index;
			delete p_table_node;
			goto error;
		}

		p_table_node->len_data = m_p_section[i].size_table;
		p_table_node->p_data = new unsigned char[p_table_node->len_data + 1];

		fseek(pf, m_p_section[i].addr_table, SEEK_SET);
		if(1 != fread(p_table_node->p_data, p_table_node->len_data, 1, pf))
		{
			delete [] p_table_node->p_data;
			delete [] p_table_node->p_table_index;
			delete p_table_node;
			goto error;
		}

		m_table_map.insert(std::make_pair(p_table_node->table_head.trans_type, p_table_node));
	}

	fclose(pf);
	return true;

error:
	fclose(pf);
	Quit();
	return false;
}

void CCharCodeTable::Quit()
{
	if(m_p_filepath != NULL)
	{
		delete [] m_p_filepath;
		m_p_filepath = NULL;
	}

	if(m_p_section != NULL)
	{
		delete [] m_p_section;
		m_p_section = NULL;
	}

	CCT_TABLE_MAP::iterator iter;
	for(iter = m_table_map.begin(); iter != m_table_map.end(); iter++)
	{
		if(iter->second->p_table_index)
		{
			delete [] iter->second->p_table_index;
			iter->second->p_table_index = NULL;
		}

		if(iter->second->p_data)
		{
			delete [] iter->second->p_data;
			iter->second->p_data = NULL;
		}

		delete iter->second;
	}

	m_table_map.clear();
}

int CCharCodeTable::ConvertBit8(std::string& outStr, int outCodeType, const char* pInStr, int inCodeType)
{
	unsigned long in_type = static_cast<unsigned long>(inCodeType);
	unsigned long out_type = static_cast<unsigned long>(outCodeType);
	unsigned long trans_type = ((in_type << 16) | out_type);

	switch (trans_type)
	{
	case (((static_cast<unsigned long>(CCT_CODE_UTF8))<< 16) | CCT_CODE_GBK):
		{
			//utf8 --> gbk
			return Translate(outStr, outCodeType, pInStr, inCodeType);
		}
		break;
	case (((static_cast<unsigned long>(CCT_CODE_GBK))<< 16) | CCT_CODE_UTF8):
		{
			//gbk --> utf8
			return Translate(outStr, outCodeType, pInStr, inCodeType);
		}
		break;
	default:
		break;
	}

	return -1;
}

int CCharCodeTable::Translate(std::string& outStr, int outCodeType, const char* pInStr, int inCodeType)
{
	unsigned long in_type = static_cast<unsigned long>(inCodeType);
	unsigned long out_type = static_cast<unsigned long>(outCodeType);
	unsigned long trans_type = 0;

	//查表, 只能找unicode <---> gbk的对应关系
	if(in_type == CCT_CODE_GBK)
	{
		trans_type = ((CCT_CODE_GBK << 16) | CCT_CODE_UNICODE);
	}
	else if(in_type == CCT_CODE_UTF8)
	{
		trans_type = ((CCT_CODE_UNICODE << 16) | CCT_CODE_GBK);
	}

	CCT_TABLE_MAP::iterator iter = m_table_map.find(trans_type);
	if(iter != m_table_map.end())
	{
		CCT_TABLE_NODE* p_table_node = iter->second;

		if(inCodeType == CCT_CODE_GBK)
		{
			//gbk --> utf8
			size_t src_len = strlen(pInStr);

			if(src_len <= 0)
			{
				return 0;
			}

			char* p_in = (char*)pInStr;
			char* p_out = new char[src_len * 3];
			memset(p_out, 0, sizeof(char) * src_len * 3);

			size_t i = 0;
			size_t out_index = 0;
			do 
			{
				unsigned long src_char = static_cast<unsigned char>(p_in[i]);

				if(src_char < 0x080)
				{
					p_out[out_index++] = static_cast<unsigned char>(src_char);
					i++;
				}
				else
				{
					src_char = (static_cast<unsigned long>((unsigned char)(p_in[i])) << 8 | static_cast<unsigned long>((unsigned char)(p_in[i + 1])));
					i += 2;

					unsigned long src_ucs = 0;
					if(SearchTable(src_ucs, src_char, p_table_node))
					{
						if(src_ucs < 0x080)
						{
							p_out[out_index++] = static_cast<unsigned char>(src_ucs);
						}
						else if(src_ucs < 0x800)
						{
							p_out[out_index++] = 0xc0 | (src_ucs >> 6);
							p_out[out_index++] = 0x80 | (src_ucs & 0x3f);
						}
						else
						{
							p_out[out_index++] = 0xe0 | (src_ucs >> 12);
							p_out[out_index++] = 0x80 | ((src_ucs >> 6) & 0x3f);
							p_out[out_index++] = 0x80 | (src_ucs & 0x3f);
						}
					}
					else
					{
						//可能碰到无法转换的情况
						assert(false);

						delete [] p_out;
						return -1;
					}
				}
			} while (i < src_len);

			p_out[out_index++] = 0x00;
			outStr = p_out;

			delete [] p_out;
			return out_index;
		}
		else if(inCodeType == CCT_CODE_UTF8)
		{
			//utf8 --> gbk
			size_t src_len = strlen(pInStr);

			if(src_len <= 0)
			{
				return 0;
			}

			char* p_in = (char*)pInStr;
			char* p_out = new char[src_len + 1];			//utf8的字符长度不会比gbk字符占用空间少
			memset(p_out, 0, sizeof(char) * (src_len + 1));

			size_t i = 0;
			size_t out_index = 0;

			do 
			{
				unsigned long src_char = static_cast<unsigned char>(p_in[i]);

				if((src_char & 0x80) == 0)
				{
					//0xxxxxx
					p_out[out_index++] = static_cast<unsigned char>(src_char);
					i++;
				}
				else
				{
					unsigned long src_ucs = 0, dest_gbk = 0;

					if((src_char & 0xe0) == 0xe0)
					{
						//1110xxxx 10xxxxxx 10xxxxxx
						src_ucs = (src_char & 0x1F) << 12;
						src_char = static_cast<unsigned char>(p_in[i + 1]);
						src_ucs |= (src_char & 0x3F) << 6;
						src_char = static_cast<unsigned char>(p_in[i + 2]);
						src_ucs |= (src_char & 0x3F);

						i += 3;
					}
					else
					{
						//110xxxxx 10xxxxxx
						src_ucs = (src_char & 0x3F) << 6;
						src_char = static_cast<unsigned char>(p_in[i + 1]);
						src_ucs |= (src_char & 0x3F);

						i += 2;
					}

					if(SearchTable(dest_gbk, src_ucs, p_table_node))
					{
						p_out[out_index++] = ((dest_gbk >> 8) & 0xFF);
						p_out[out_index++] = (dest_gbk & 0xFF);
					}
					else
					{
						//可能碰到无法转换的情况
						assert(false);
						delete [] p_out;
						return -1;
					}
				}
			} while (i < src_len);

			p_out[out_index++] = 0x00;
			outStr = p_out;

			delete [] p_out;
			return out_index;
		}
	}

	return -1;
}

bool CCharCodeTable::SearchTable(unsigned long& outChar, unsigned long inChar, const CCT_TABLE_NODE* pTableNode)
{
	CCT_TABLE_INDEX* p_table_index = (CCT_TABLE_INDEX*)(pTableNode->p_table_index);
	unsigned char* p_data = (unsigned char*)(pTableNode->p_data);

	unsigned long char_id = inChar;

	if(pTableNode->table_head.src_encode == CCT_CODE_GBK)
	{
		for(int i = 0; i < pTableNode->table_head.count_index; i++)
		{
			//只有5个表索引区,直接比较不会带来太大效率问题
			unsigned long low_1 = ((p_table_index[i].low_char >> 16) & 0xFFFF);
			unsigned long low_2 =((p_table_index[i].low_char) & 0xFFFF);
			unsigned long hi_1 = ((p_table_index[i].high_char >> 16) & 0xFFFF);
			unsigned long hi_2 = ((p_table_index[i].high_char) & 0xFFFF);

			unsigned long bit1 = ((char_id >> 8) & 0xFF);
			unsigned long bit2 = (char_id & 0xFF);

			if((low_1 <= bit1) && (hi_1 >= bit1) && (low_2 <= bit2) && (hi_2 >= bit2))
			{
				unsigned long index = (hi_2 - low_2 + 1) * (bit1 - low_1) + bit2 - low_2;
				unsigned long offset = p_table_index[i].offset + index * sizeof(unsigned long);
				outChar = *reinterpret_cast<unsigned long*>(p_data + offset);

				return true;
			}
		}
	}
	else if(pTableNode->table_head.src_encode == CCT_CODE_UNICODE)
	{
		for(int i = 0; i < pTableNode->table_head.count_index; i++)
		{
			//只有4个表索引区
			if((p_table_index[i].low_char <= char_id) && (p_table_index[i].high_char >= char_id))
			{
				unsigned long index = (char_id - p_table_index[i].low_char);
				unsigned long offset = p_table_index[i].offset + index * sizeof(unsigned long);
				outChar = *reinterpret_cast<unsigned long*>(p_data + offset);

				return true;
			}
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

#ifdef __ENVIRONMENT_WIN32__

int WcharToMBCS(CStringA& outStr, const CStringW& inStr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)inStr, -1, NULL,0, NULL, NULL);

	if(len > 0)
	{
		char* pText = new char[len + 1];
		memset(pText, 0, sizeof(char) * (len + 1));

		WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)inStr, -1, pText, len, NULL, NULL);

		outStr = pText;
		delete [] pText;
	}
	else
	{
		outStr = "";
	}

	return len;
}

int MBCSToWchar(CStringW& outStr, const CStringA& inStr)
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)inStr, -1, NULL, 0);

	if(len > 0)
	{
		wchar_t* pText = new wchar_t[len + 1];
		memset(pText, 0, sizeof(wchar_t) * (len + 1));

		MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)inStr, -1, pText, len);

		outStr = pText;
		delete [] pText;
	}
	else
	{
		outStr = L"";
	}

	return len;
}

bool CCharCodeTable::CreateTable(const char* filePath)
{
	FILE* pf = fopen(filePath, "wb+");

	if(pf == NULL)
	{
		return false;
	}

	CCT_TABLE_HEAD table_head;
	CCT_TABLE_INDEX table_index;
	CCT_SECTION_INDEX section_index;
	unsigned long table_addr = 0, table_size = 0;

	//** CCT_FILE_HEAD
	CCT_FILE_HEAD cct_head;
	memset(&cct_head, 0, sizeof(CCT_FILE_HEAD));

	strncpy(cct_head.magic, CCT_FILE_MAGIC, sizeof(cct_head.magic));
	strncpy(cct_head.version, CCT_FILE_VERSION, sizeof(cct_head.version));
	cct_head.size = sizeof(CCT_FILE_HEAD);
	cct_head.count = 2;
	cct_head.addr_section = sizeof(CCT_FILE_HEAD);

	//** unicode --> gbk TABLE
	//中文处理以下几个区域
	//中文标点符号 0x2010 -- 0x2027
	//中文标点符号 0x3000 -- 0x30FF
	//中日韩文字区 0x4E00 -- 0x9FFF
	//中文标点符号 0xFF00 -- 0xFF9F
	table_addr = cct_head.addr_section + cct_head.count * sizeof(CCT_SECTION_INDEX);

	memset(&table_head, 0, sizeof(CCT_TABLE_HEAD));
	table_head.src_encode = CCT_CODE_UNICODE;
	table_head.dest_encode = CCT_CODE_GBK;
	table_head.trans_type = (table_head.src_encode << 16) + table_head.dest_encode;
	table_head.count_index = 4;

	fseek(pf, table_addr, SEEK_SET);
	fwrite(&table_head, sizeof(CCT_TABLE_HEAD), 1, pf);

	{
		unsigned long offset = sizeof(CCT_TABLE_HEAD) + sizeof(CCT_TABLE_INDEX) * table_head.count_index;
		unsigned long code_low_hi[] = {0x2010, 0x2027, 0x3000, 0x30FF, 0x4E00, 0x9FFF, 0xFF00, 0xFF9F};

		for(int i = 0; i < sizeof(code_low_hi) / (sizeof(code_low_hi[0]) * 2); i++)
		{
			memset(&table_index, 0, sizeof(CCT_TABLE_INDEX));

			table_index.low_char = code_low_hi[2 * i];
			table_index.high_char = code_low_hi[2 * i + 1];
			table_index.len = table_index.high_char - table_index.low_char;
			table_index.offset = offset;

			fseek(pf, table_addr + sizeof(CCT_TABLE_HEAD) + sizeof(CCT_TABLE_INDEX) * i, SEEK_SET);
			fwrite(&table_index, sizeof(CCT_TABLE_INDEX), 1, pf);

			fseek(pf, table_addr + table_index.offset, SEEK_SET);
			for(unsigned long charID = table_index.low_char; charID <= table_index.high_char; charID++)
			{
				unsigned long destID = 0;

				CStringW strW;
				strW.Format(L"%c", charID);

				CStringA strA;
				int len = WcharToMBCS(strA, strW);

				if(len > 2 && !strA.IsEmpty())
				{
					destID = static_cast<unsigned long>(0xFF00 & (strA[0] << 8)) + static_cast<unsigned long>(0xFF & strA[1]);
				}

				fwrite(&destID, sizeof(unsigned long), 1, pf);
				offset += sizeof(unsigned long);
			}
		}

		memset(&section_index, 0, sizeof(CCT_SECTION_INDEX));
		section_index.addr_table = table_addr;
		section_index.size_table = offset;

		fseek(pf, cct_head.addr_section, SEEK_SET);
		fwrite(&section_index, sizeof(CCT_SECTION_INDEX), 1, pf);

		table_addr += offset;
	}

	//** gbk --> unicode TABLE
	//Level GBK/1 	A1–A9 	A1–FE
	//Level GBK/2 	B0–F7 	A1–FE
	//Level GBK/3 	81–A0 	40–FE except 7F
	//Level GBK/4 	AA–FE 	40–A0 except 7F
	//Level GBK/5 	A8–A9 	40–A0 except 7F

	memset(&table_head, 0, sizeof(CCT_TABLE_HEAD));
	table_head.src_encode = CCT_CODE_GBK;
	table_head.dest_encode = CCT_CODE_UNICODE;
	table_head.trans_type = (table_head.src_encode << 16) + table_head.dest_encode;
	table_head.count_index = 5;

	fseek(pf, table_addr, SEEK_SET);
	fwrite(&table_head, sizeof(CCT_TABLE_HEAD), 1, pf);

	{
		unsigned long offset = sizeof(CCT_TABLE_HEAD) + sizeof(CCT_TABLE_INDEX) * table_head.count_index;
		unsigned long code_low_hi[] = { 0xA1, 0xA1, 0xA9, 0xFE,
			0xB0, 0xA1, 0xF7, 0xFE,
			0x81, 0x40, 0xA0, 0xFE,
			0xAA, 0x40, 0xFE, 0xA0,
			0xA8, 0x40, 0xA9, 0xA0};
		for(int i = 0; i < sizeof(code_low_hi) / (sizeof(code_low_hi[0]) * 4); i++)
		{
			unsigned long low_1 = code_low_hi[4 * i];
			unsigned long low_2 = code_low_hi[4 * i + 1];
			unsigned long hi_1 = code_low_hi[4 * i + 2];
			unsigned long hi_2 = code_low_hi[4 * i + 3];

			memset(&table_index, 0, sizeof(CCT_TABLE_INDEX));
			table_index.low_char =  (low_1 << 16) + low_2;
			table_index.high_char = (hi_1 << 16) + hi_2;
			table_index.len = (hi_2 - low_2) * (hi_1 - low_1);
			table_index.offset = offset;

			fseek(pf, table_addr + sizeof(CCT_TABLE_HEAD) + sizeof(CCT_TABLE_INDEX) * i, SEEK_SET);
			fwrite(&table_index, sizeof(CCT_TABLE_INDEX), 1, pf);

			fseek(pf, table_addr + table_index.offset, SEEK_SET);
			for(unsigned long j = 0; j <= (hi_1 - low_1); j++)
			{
				for(unsigned long k = 0; k <= (hi_2 - low_2); k++)
				{
					unsigned long destID = 0;
					//unsigned long charID = ((j + low_1) << 8) + k + low_2;

					CStringA strA;
					strA.Format("%c%c", (j + low_1), (k + low_2));

					if(!strA.IsEmpty())
					{
						CStringW strW;
						int len = MBCSToWchar(strW, strA);

						if(len > 0 && !strW.IsEmpty())
						{
							destID = static_cast<unsigned long>(strW[0]);
						}
					}

					fwrite(&destID, sizeof(unsigned long), 1, pf);
					offset += sizeof(unsigned long);
				}
			}
		}

		memset(&section_index, 0, sizeof(CCT_SECTION_INDEX));
		section_index.addr_table = table_addr;
		section_index.size_table = offset;

		fseek(pf, cct_head.addr_section + sizeof(CCT_SECTION_INDEX), SEEK_SET);
		fwrite(&section_index, sizeof(CCT_SECTION_INDEX), 1, pf);
	}

	//写文件头
	fseek(pf, 0, SEEK_SET);
	fwrite(&cct_head, sizeof(CCT_FILE_HEAD), 1, pf);

	fclose(pf);

	return true;
}

#endif

//end

