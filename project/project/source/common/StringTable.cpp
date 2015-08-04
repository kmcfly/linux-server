/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-05-12
** Name         : StringTable.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "StringTable.h"
#include <assert.h>
#include <algorithm>

/***********************************************************************
							public
***********************************************************************/
CStringTable * CStringTable::Instance()
{
	static CStringTable s_StringTable;
	return &s_StringTable;
}

CStringTable::~CStringTable()
{

}

bool CStringTable::Initial(unsigned long language, const char *pFilePath)
{
	assert(pFilePath != NULL);
	m_language = language;
	m_pFilePath = new char[256];
	memset(m_pFilePath, 0, 256);

	strcpy(m_pFilePath, pFilePath);

#ifdef __ENVIRONMENT_WIN32__
	switch(language)
	{
	case LANGUAGE_ENGLISH:
		strcat(m_pFilePath, "language\\english.dat");
		break;
	case LANGUAGE_CHINESE_S:
		strcat(m_pFilePath, "language\\chinese_s.dat");
		break;
	case LANGUAGE_CHINESE_B:
		strcat(m_pFilePath, "language\\chinese_b.dat");
	    break;
	case LANGUAGE_PORTUGUESE:
		strcat(m_pFilePath, "language\\portuguese.dat");
		break;
	case LANGUAGE_GREECE:
		strcat(m_pFilePath, "language\\greece.dat");
		break;
	case LANGUAGE_SPAISH:
		strcat(m_pFilePath, "language\\spaish.dat");
		break;
	case LANGUAGE_RUSSIAN:
		strcat(m_pFilePath, "language\\russian.dat");
		break;
	case LANGUAGE_NORWAY:
		strcat(m_pFilePath, "language\\norway.dat");
		break;
	case LANGUAGE_TURKEY:
		strcat(m_pFilePath, "language\\turkey.dat");
		break;
	case LANGUAGE_ITALY:
		strcat(m_pFilePath, "language\\italy.dat");
		break;
	case LANGUAGE_CZECH:
		strcat(m_pFilePath, "language\\czech.dat");
		break;
	case LANGUAGE_GERMAN:
		strcat(m_pFilePath, "language\\german.dat");
		break;
	case LANGUAGE_HEBREW:
		strcat(m_pFilePath, "language\\hebrew.dat");
		break;
	case LANGUAGE_JAPANESE:
		strcat(m_pFilePath, "language\\japanese.dat");
		break;
	case LANGUAGE_FRENCH:
		strcat(m_pFilePath, "language\\french.dat");
		break;
	case LANGUAGE_POLISH:
		strcat(m_pFilePath, "language\\polish.dat");
		break;
	case LANGUAGE_BULGARIAN:
		strcat(m_pFilePath, "language\\bulgarian.dat");
		break;
#if defined(__CUSTOM_PL14__)  || defined(__CUSTOM_US02_NEW__)
	case LANGUAGE_INDONESIA:
		strcat(m_pFilePath, "language\\romanian.dat");
		break;
#else
	case LANGUAGE_INDONESIA:
		strcat(m_pFilePath, "language\\indonesia.dat");
		break;
#endif
	case LANGUAGE_RUSSIAN_D:
		strcat(m_pFilePath, "language\\russian_d.dat");
		break;
	case LANGUAGE_THAI:
		strcat(m_pFilePath, "language\\thai.dat");
		break;
	case LANGUAGE_HUNGARY:
		strcat(m_pFilePath, "language\\hungary.dat");
		break;
	case LANGUAGE_LITHUANIA:
		strcat(m_pFilePath, "language\\lithuania.dat");
		break;
	case LANGUAGE_VIETNAMESE:
		strcat(m_pFilePath, "language\\vietnamese.dat");
		break;
	case LANGUAGE_PERSIAN:
		strcat(m_pFilePath, "language\\persian.dat");
		break;
	case LANGUAGE_DUTCH:
		strcat(m_pFilePath, "language\\dutch.dat");
		break;
#if defined(__CUSTOM_IN25__) || defined(__CUSTOM_GB31__) || defined(__CUSTOM_PL14__)
	case LANGUAGE_FINNISH:
		strcat(m_pFilePath, "language\\finnish.dat");
		break;
#else
	case LANGUAGE_FINNISH:
		strcat(m_pFilePath, "language\\arabic.dat");
		break;
#endif
	case LANGUAGE_SWEDISH:
		strcat(m_pFilePath, "language\\swedish.dat");
		break;
#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
	case LANGUAGE_DANISH:
		strcat(m_pFilePath, "language\\romanian.dat");
		break;
#else
	case LANGUAGE_DANISH:
		strcat(m_pFilePath, "language\\danish.dat");
		break;
#endif
	default:
		return false;
	    break;
	}
#else
	switch(language)
	{
	case LANGUAGE_ENGLISH:
		strcat(m_pFilePath, "language/english.dat");
		break;
#if defined(__CUSTOM_BR27__)
	case LANGUAGE_CHINESE_S:
		strcat(m_pFilePath, "language/korean.dat");
		break;
#else
	case LANGUAGE_CHINESE_S:
		#if defined(__KR24_IN__) || defined(__KOREAN__)
			strcat(m_pFilePath, "language/korean.dat");
		#else
			strcat(m_pFilePath, "language/chinese_s.dat");
		#endif
		break;
	#if defined(__KR24_IN__) || defined(__KOREAN__)
	#else
	case LANGUAGE_CHINESE_B:
		strcat(m_pFilePath, "language/chinese_b.dat");
		break;
	#endif
#endif
	case LANGUAGE_PORTUGUESE:
		strcat(m_pFilePath, "language/portuguese.dat");
		break;
	case LANGUAGE_GREECE:
		strcat(m_pFilePath, "language/greece.dat");
		break;
	case LANGUAGE_SPAISH:
		strcat(m_pFilePath, "language/spaish.dat");
		break;
	case LANGUAGE_RUSSIAN:
		strcat(m_pFilePath, "language/russian.dat");
		break;
	case LANGUAGE_NORWAY:
		strcat(m_pFilePath, "language/norway.dat");
		break;
	case LANGUAGE_TURKEY:
		strcat(m_pFilePath, "language/turkey.dat");
		break;
	case LANGUAGE_ITALY:
		strcat(m_pFilePath, "language/italy.dat");
		break;
	case LANGUAGE_CZECH:
		strcat(m_pFilePath, "language/czech.dat");
		break;
	case LANGUAGE_GERMAN:
		strcat(m_pFilePath, "language/german.dat");
		break;
	case LANGUAGE_HEBREW:
		strcat(m_pFilePath, "language/hebrew.dat");
		break;
		
#ifndef __CUSTOM_BR27__
	case LANGUAGE_JAPANESE:
		strcat(m_pFilePath, "language/japanese.dat");
		break;
#endif
	case LANGUAGE_FRENCH:
		strcat(m_pFilePath, "language/french.dat");
		break;
	case LANGUAGE_POLISH:
		strcat(m_pFilePath, "language/polish.dat");
		break;
	case LANGUAGE_BULGARIAN:
		strcat(m_pFilePath, "language/bulgarian.dat");
		break;
#if defined(__CUSTOM_PL14__) || defined (__CUSTOM_US02_NEW__)
	case LANGUAGE_INDONESIA:
		strcat(m_pFilePath, "language/romanian.dat");
		break;
#else
	case LANGUAGE_INDONESIA:
		strcat(m_pFilePath, "language/indonesia.dat");
		break;
#endif
	case LANGUAGE_RUSSIAN_D:
		strcat(m_pFilePath, "language/russian_d.dat");
		break;
	case LANGUAGE_THAI:
		strcat(m_pFilePath, "language/thai.dat");
		break;
	case LANGUAGE_HUNGARY:
		strcat(m_pFilePath, "language/hungary.dat");
		break;
	case LANGUAGE_LITHUANIA:
		strcat(m_pFilePath, "language/lithuania.dat");
		break;
	case LANGUAGE_VIETNAMESE:
		strcat(m_pFilePath, "language/vietnamese.dat");
		break;
	case LANGUAGE_PERSIAN:
		strcat(m_pFilePath, "language/persian.dat");
		break;
	case LANGUAGE_DUTCH:
		strcat(m_pFilePath, "language/dutch.dat");
		break;
#if defined(__CUSTOM_IN25__) || defined(__CUSTOM_GB31__) || defined(__CUSTOM_PL14__)
	case LANGUAGE_FINNISH:
		strcat(m_pFilePath, "language/finnish.dat");
		break;
#else
	case LANGUAGE_FINNISH:
		strcat(m_pFilePath, "language/arabic.dat");
		break;
#endif
	case LANGUAGE_SWEDISH:
		strcat(m_pFilePath, "language/swedish.dat");
		break;
#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
	case LANGUAGE_DANISH:
		strcat(m_pFilePath, "language/romanian.dat");
		break;
#else
	case LANGUAGE_DANISH:
		strcat(m_pFilePath, "language/danish.dat");
		break;
#endif
	default:
		return false;
		break;
	}
#endif

	m_pFile = fopen(m_pFilePath, "rb");
	if (NULL == m_pFile)
	{
		assert(false);
		return false;
	}

	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	if (1 != fread(&m_fileHead, sizeof(STR_TABLE_FILE_HEAD), 1, m_pFile))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	if (m_fileHead.languageID != m_language)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		assert(m_fileHead.languageID == m_language);
		return false;
	}

	if (!LoadString())
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	fclose(m_pFile);
	m_pFile = NULL;

	return true;
}

void CStringTable::Quit()
{
	delete [] m_pFilePath;
	m_pFilePath = NULL;

#ifdef _CREATE_STRING
	if (m_strList.size() > 0)
	{
		std::list<STRING_INFO>::iterator it;

		for (it = m_strList.begin(); it != m_strList.end(); ++it) 
		{
			delete[] (*it).pString;
			(*it).pString = NULL;
		}

		m_strList.clear();
	}
#else
	if(m_strVec.size() > 0)
	{
		std::vector<STRING_INFO>::iterator it;

		for (it = m_strVec.begin(); it != m_strVec.end(); ++it) 
		{
			delete[] (*it).pString;
			(*it).pString = NULL;
		}

		m_strVec.clear();
	}
#endif
}

const char * CStringTable::String(const ENUM_STRING_ID stringID) 
{
#ifdef _CREATE_STRING
	if (m_strList.size() > 0)
	{
		//由于列表是按照ID的升序排列，所以满足以下条件的话说明没有对应的ID。
		if ((stringID < m_strList.front().stringID) || (m_strList.back().stringID < stringID))
		{
			assert(false);
			return NULL;
		}
		
		std::list<STRING_INFO>::iterator it;

		for (it = m_strList.begin(); it != m_strList.end(); ++it) 
		{
			if ((*it).stringID == stringID)
			{
				return (*it).pString;
			}
		}
	}
#else
	STRING_INFO strinfo;
	strinfo.stringID = static_cast<unsigned short>(stringID);
	std::vector<STRING_INFO>::iterator iter = std::lower_bound(m_strVec.begin(), m_strVec.end(), strinfo);
	if(iter != m_strVec.end())
	{
		if(iter->stringID == static_cast<unsigned short>(stringID))
		{
			return (*iter).pString;
		}
		else
		{
			printf("(%s.%d) Cannot find string id=0x%X.\n", __FILE__, __LINE__, stringID);
			assert(false);
			return "xxxx";
		}
	}
#endif

	printf("(%s.%d) Cannot find string id=0x%X.\n", __FILE__, __LINE__, stringID);
	assert(false);
	return NULL;
}

#if defined(__ENVIRONMENT_WIN32__) && defined(_CREATE_STRING)
bool CStringTable::Initial(const char *pFilePath)
{
	assert(pFilePath != NULL);
	m_pFilePath = new char[256];
	memset(m_pFilePath, 0, 256);
	strcpy(m_pFilePath, pFilePath);

	m_pFile = fopen(m_pFilePath, "rb");
	if (NULL == m_pFile)
	{
		assert(false);
		return false;
	}

	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	if (1 != fread(&m_fileHead, sizeof(STR_TABLE_FILE_HEAD), 1, m_pFile))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	m_language = m_fileHead.languageID;
	//if (m_fileHead.languageID != m_language)
	//{
	//	fclose(m_pFile);
	//	m_pFile = NULL;
	//	assert(m_fileHead.languageID == m_language);
	//	return false;
	//}

	if (!LoadString())
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	fclose(m_pFile);
	m_pFile = NULL;

	return true;
}

bool CStringTable::Save(LANGUAGE languageID, const char *pFilePath)
{
	assert (NULL != pFilePath);
	FILE *pFile = fopen(pFilePath, "wb+");
	if (NULL == pFile)
	{
		assert(false);
		return false;
	}

	if (0 != fseek(pFile, STR_TABLE_DATA_POS, SEEK_SET))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	STR_TABLE_FILE_HEAD fileHead;

	fileHead.languageID = languageID;
	fileHead.fileLength = STR_TABLE_DATA_POS;
	fileHead.stringNum = 0;
	
	unsigned long offset = 0;
	unsigned char buf [32] = {0};
	memset(buf, 0, 32);
	std::list<STRING_INFO>::iterator it;
	
	for (it = m_strList.begin(); it != m_strList.end(); ++it) 
	{
		if (1 != fwrite(&(*it), sizeof(STRING_HEAD), 1, pFile))
		{
			fclose(pFile);
			pFile = NULL;
			return false;
		}

		if (1 != fwrite((*it).pString, (*it).stringLen, 1, pFile))
		{
			fclose(pFile);
			pFile = NULL;
			return false;
		}

		offset = (sizeof(STRING_HEAD)+(*it).stringLen);
		if (OffSet(offset) > offset)
		{
			if (1 != fwrite(buf, OffSet(offset) - offset, 1, pFile))
			{
				fclose(pFile);
				pFile = NULL;
				return false;
			}
		}

		fileHead.fileLength += OffSet(offset);
		++ fileHead.stringNum;
	}

	if (0 != fseek(pFile, 0, SEEK_SET))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}
	if (1 != fwrite(&fileHead, sizeof(STR_TABLE_FILE_HEAD), 1, pFile))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	fclose(pFile);
	pFile = NULL;
	return true;	
}

const std::list<STRING_INFO>& CStringTable::GetStringList()
{
	return m_strList;
}

unsigned long CStringTable::GetLanguage()
{
	return m_language;
}

bool CStringTable::AddString(const ENUM_STRING_ID stringID, const char *pString, unsigned short stringLen)
{
	assert (NULL != pString);
	assert ((0 < stringLen) && (stringLen < 2048));
	assert (stringLen == strlen(pString));

	STRING_INFO strInfo;
	strInfo.stringID = stringID;
	strInfo.stringLen = stringLen;
	strInfo.pString = new char [stringLen+1];

	strcpy(strInfo.pString, pString);

	AddToList(strInfo);

	return true;
}

#endif
/***********************************************************************
							protected
***********************************************************************/


/***********************************************************************
							private
***********************************************************************/
CStringTable::CStringTable() : m_pFile(NULL), m_pFilePath(NULL),m_language(LANGUAGE_ENGLISH)
{

}

//////////////////////////////////////////////////////////////////////////
bool CStringTable::LoadString()
{
	assert(m_pFile != NULL);
	if (0 != fseek(m_pFile, 0, SEEK_END))
	{
		return false;
	}

	long fileLen = ftell(m_pFile);
	assert (fileLen > 0);

	unsigned char *pBuf = new unsigned char[fileLen];
	if (pBuf == NULL)
	{
		return false;
	}
	else
	{
		memset(pBuf, 0, fileLen);
	}

	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{
		delete [] pBuf;
		pBuf = NULL;
		return false;
	}

	if (1 != fread(pBuf, fileLen, 1, m_pFile))
	{
		delete [] pBuf;
		pBuf = NULL;
		return false;
	}

	long readPos = STR_TABLE_DATA_POS;
	STRING_INFO stringInfo;
	for (unsigned long i=0; i<m_fileHead.stringNum; i++)
	{
		STRING_HEAD *pStringHead = reinterpret_cast<STRING_HEAD*>(pBuf + readPos);

		stringInfo.stringID = pStringHead->stringID;
		stringInfo.stringLen = pStringHead->stringLen;
		stringInfo.pString = new char[stringInfo.stringLen+1];
		if (stringInfo.pString == NULL)
		{
			delete [] pBuf;
			pBuf = NULL;
			return false;
		}

		memset(stringInfo.pString, 0, stringInfo.stringLen+1);
		memcpy(stringInfo.pString, (pBuf +readPos + sizeof(STRING_HEAD)), stringInfo.stringLen);
		AddToList(stringInfo);
		
		readPos += OffSet(sizeof(STRING_HEAD)+stringInfo.stringLen);
	}

	delete [] pBuf;
	pBuf = NULL;

#ifndef _CREATE_STRING
	if(m_strVec.size() > 1)
	{
		std::sort(m_strVec.begin(), m_strVec.end());
	}

	if(m_strVec.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
#else
	return true;
#endif
}

void CStringTable::AddToList(const STRING_INFO &strInfo)
{
#ifdef _CREATE_STRING
	if (0 == m_strList.size())
	{
		m_strList.push_back(strInfo);
	}
	else
	{
		if (strInfo.stringID < m_strList.front().stringID)
		{
			m_strList.push_front(strInfo);
		}
		else if (m_strList.back().stringID < strInfo.stringID)
		{
			m_strList.push_back(strInfo);
		}
		else
		{
			std::list<STRING_INFO>::iterator it;

			for (it = m_strList.begin(); it != m_strList.end(); ++it) 
			{
				if (strInfo.stringID < (*it).stringID)
				{
					m_strList.insert(it, strInfo);
					break;
				}
			}
		}
	}
#else
	m_strVec.push_back(strInfo);
#endif
}

size_t CStringTable::OffSet(unsigned long stringLen)
{
	return ((stringLen + 15) >> 4 ) << 4;
}


//end


