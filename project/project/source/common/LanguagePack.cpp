
#include "LanguagePack.h"
#include <assert.h>
#include "StringTable.h"


#ifdef __ENVIRONMENT_WIN32__
#include "io.h"
#else
#include <sys/stat.h>
#include <dirent.h>
#endif

//语言名称。用当地UTF8编码
const char LANG_UTF8_ENGLISH[]		= {0x45,0x6E,0x67,0x6C,0x69,0x73,0x68,0x0};
const char LANG_UTF8_CHINESE_S[]	= {0xE7,0xAE,0x80,0xE4,0xBD,0x93,0xE4,0xB8,0xAD,0xE6,0x96,0x87,0x0};
const char LANG_UTF8_CHINESE_B[]	= {0xE7,0xB9,0x81,0xE9,0xAB,0x94,0xE4,0xB8,0xAD,0xE6,0x96,0x87,0x0};
const char LANG_UTF8_PORTUGUESE[]	= {0x50,0x6F,0x72,0x74,0x75,0x67,0x75,0xC3,0xAA,0x73,0x0};
const char LANG_UTF8_GREECE[]		= {0xCE,0x95,0xCE,0xBB,0xCE,0xBB,0xCE,0xB7,0xCE,0xBD,0xCE,0xB9,0xCE,0xBA,0xCE,0xAC,0x0};
const char LANG_UTF8_SPAISH[]		= {0x45,0x73,0x70,0x61,0xC3,0xB1,0x6F,0x6C,0x0};
const char LANG_UTF8_RUSSIAN[]		= {0xD0,0xA0,0xD1,0x83,0xD1,0x81,0xD1,0x81,0xD0,0xBA,0xD0,0xB8,0xD0,0xB9,0x0};
const char LANG_UTF8_NORWAY[]		= {0x4E,0x6F,0x72,0x73,0x6B,0x65,0x0};
const char LANG_UTF8_TURKEY[]		= {0x54,0xC3,0xBC,0x72,0x6B,0x0};
const char LANG_UTF8_ITALY[]		= {0x49,0x74,0x61,0x6C,0x69,0x61,0x6E,0x6F,0x0};
const char LANG_UTF8_CZECH[]		= {0xC4,0x8C,0x65,0x73,0x6B,0xC3,0xBD,0x0};
const char LANG_UTF8_GERMAN[]		= {0x44,0x65,0x75,0x74,0x73,0x63,0x68,0x0};
const char LANG_UTF8_HEBREW[]		= {0xD7,0xA2,0xD7,0x91,0xD7,0xA8,0xD7,0x99,0xD7,0xAA,0x0};
const char LANG_UTF8_JAPANESE[]		= {0xE6,0x97,0xA5,0xE6,0x9C,0xAC,0xE8,0xAA,0x9E,0x0};
const char LANG_UTF8_FRENCH[]		= {0x46,0x72,0x61,0x6E,0xC3,0xA7,0x61,0x69,0x73,0x0};
const char LANG_UTF8_POLISH[]		= {0x50,0x6F,0x6C,0x73,0x6B,0x69,0x0};
const char LANG_UTF8_BULGARIAN[]	= {0xD0,0x91,0xD1,0x8A,0xD0,0xBB,0xD0,0xB3,0xD0,0xB0,0xD1,0x80,0xD1,0x81,0xD0,0xBA,0xD0,0xB8,0x0};
const char LANG_UTF8_INDONESIA[]	= {0x49,0x6E,0x64,0x6F,0x6E,0x65,0x73,0x69,0x61,0x0};
const char LANG_UTF8_RUSSIAN_D[]	= {0xD0,0xA0,0xD1,0x83,0xD1,0x81,0xD1,0x81,0xD0,0xBA,0xD0,0xB8,0xD0,0xB9,0x0};
const char LANG_UTF8_THAI[]			= {0xE0,0xB9,0x84,0xE0,0xB8,0x97,0xE0,0xB8,0xA2,0x0};
const char LANG_UTF8_HUNGARY[]		= {0x4D,0x61,0x67,0x79,0x61,0x72,0x0};
const char LANG_UTF8_LITHUANIA[]	= {0x4C,0x69,0x65,0x74,0x75,0x76,0x6F,0x73,0x0};
const char LANG_UTF8_VIETNAMESE[]	= {0x56,0x69,0xE1,0xBB,0x87,0x74,0x0};
const char LANG_UTF8_PERSIAN[]		= {0xD9,0x81,0xD8,0xA7,0xD8,0xB1,0xD8,0xB3,0xDB,0x8C,0x0};
const char LANG_UTF8_DUTCH[]		= {0x4E,0x65,0x64,0x65,0x72,0x6C,0x61,0x6E,0x64,0x73,0x0};
const char LANG_UTF8_FINNISH[]      = {0x53,0x75,0x6F,0x6D,0x61,0x6C,0x61,0x69,0x6E,0x65,0x6E,0x0};
const char LANG_UTF8_SWEDISH[]      = {0x53,0x76,0x65,0x6E,0x73,0x6B,0x61,0x0};
const char LANG_UTF8_DANISH[]       = {0x44,0x61,0x6E,0x73,0x6B,0x65,0x0};
const char LANG_UTF8_KOREAN[]		= {0xED,0x95,0x9C,0xEA,0xB5,0xAD,0xEC,0x96,0xB4,0x0};
const char LANG_UTF8_ARABIC[]       = {0xD8,0xA7,0xD9,0x84,0xD8,0xB9,0xD8,0xB1,0xD8,0xA8,0xD9,0x8A,0xD8,0xA9,0x0};
const char LANG_UTF8_ROMANIAN[]     = {0x72,0x6F,0x6D,0xC3,0xA2,0x6E,0x0};

//语言名称。用中文UTF8编码

const char LANG_UTF8_ENGLISH_IN_CN[]= {0xE8,0x8B,0xB1,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_CHINESE_S_IN_CN[]= {0xE7,0xAE,0x80,0xE4,0xBD,0x93,0xE4,0xB8,0xAD,0xE6,0x96,0x87,0x0};
const char LANG_UTF8_CHINESE_B_IN_CN[]= {0xE7,0xB9,0x81,0xE4,0xBD,0x93,0xE4,0xB8,0xAD,0xE6,0x96,0x87,0x0};
const char LANG_UTF8_PORTUGUESE_IN_CN[]= {0xE8,0x91,0xA1,0xE8,0x90,0x84,0xE7,0x89,0x99,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_GREECE_IN_CN[]= {0xE5,0xB8,0x8C,0xE8,0x85,0x8A,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_SPAISH_IN_CN[]= {0xE8,0xA5,0xBF,0xE7,0x8F,0xAD,0xE7,0x89,0x99,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_RUSSIAN_IN_CN[]= {0xE4,0xBF,0x84,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_NORWAY_IN_CN[]= {0xE6,0x8C,0xAA,0xE5,0xA8,0x81,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_TURKEY_IN_CN[]= {0xE5,0x9C,0x9F,0xE8,0x80,0xB3,0xE5,0x85,0xB6,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_ITALY_IN_CN[]= {0xE6,0x84,0x8F,0xE5,0xA4,0xA7,0xE5,0x88,0xA9,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_CZECH_IN_CN[]= {0xE6,0x8D,0xB7,0xE5,0x85,0x8B,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_GERMAN_IN_CN[]= {0xE5,0xBE,0xB7,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_HEBREW_IN_CN[]= {0xE5,0xB8,0x8C,0xE4,0xBC,0xAF,0xE6,0x9D,0xA5,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_JAPANESE_IN_CN[]= {0xE6,0x97,0xA5,0xE6,0x96,0x87,0x0};
const char LANG_UTF8_FRENCH_IN_CN[]= {0xE6,0xB3,0x95,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_POLISH_IN_CN[]= {0xE6,0xB3,0xA2,0xE5,0x85,0xB0,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_BULGARIAN_IN_CN[]= {0xE4,0xBF,0x9D,0xE5,0x8A,0xA0,0xE5,0x88,0xA9,0xE4,0xBA,0x9A,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_INDONESIA_IN_CN[]= {0xE5,0x8D,0xB0,0xE5,0xBA,0xA6,0xE5,0xB0,0xBC,0xE8,0xA5,0xBF,0xE4,0xBA,0x9A,0x0};
const char LANG_UTF8_RUSSIAN_D_IN_CN[]= {0xE4,0xBF,0x84,0xE6,0x96,0x87,0x5B,0x52,0x55,0x30,0x34,0x5D,0x0};
const char LANG_UTF8_THAI_IN_CN[] = {0xE6,0xB3,0xB0,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_HUNGARY_IN_CN[] = {0xE5,0x8C,0x88,0xE7,0x89,0x99,0xE5,0x88,0xA9,0x0};
const char LANG_UTF8_LITHUANIA_IN_CN[] = {0xE7,0xAB,0x8B,0xE9,0x99,0xB6,0xE5,0xAE,0x9B,0x0};
const char LANG_UTF8_VIETNAMESE_IN_CN[]	= {0xE8,0xB6,0x8A,0xE5,0x8D,0x97,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_PERSIAN_IN_CN[] = {0xE6,0xB3,0xA2,0xE6,0x96,0xAF,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_DUTCH_IN_CN[] = {0xE8,0x8D,0xB7,0xE5,0x85,0xB0,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_FINNISH_IN_CN[] = {0xE8,0x8A,0xAC,0xE5,0x85,0xB0,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_SWEDISH_IN_CN[] = {0xE7,0x91,0x9E,0xE5,0x85,0xB8,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_DANISH_IN_CN[] = {0xE4,0xB8,0xB9,0xE9,0xBA,0xA6,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_KOREAN_CN[]	= {0xE9,0x9F,0xA9,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_ARABIC_IN_CN[] = {0xE9,0x98,0xBF,0xE6,0x8B,0x89,0xE4,0xBC,0xAF,0xE8,0xAF,0xAD,0x0};
const char LANG_UTF8_ROMANIAN_IN_CN[] = {0xE7,0xBD,0x97,0xE9,0xA9,0xAC,0xE5,0xB0,0xBC,0xE4,0xBA,0x9A,0x0};

CLanguagePack* CLanguagePack::Instance()
{
	static CLanguagePack s_languagePack;
	return &s_languagePack;
}

CLanguagePack::CLanguagePack()
: m_pFilePath(NULL)
, m_count(0)
{
	m_langFilterBites = LANGUAGE_ALL;
}

CLanguagePack::~CLanguagePack()
{
	Quit();
}

bool CLanguagePack::Initail(const char *pFilePath, unsigned long langFilterBites)
{
#ifdef __ENVIRONMENT_WIN32__
	langFilterBites = LANGUAGE_ALL;	
#endif

	Quit();

	m_pFilePath = new char[512];
	memset(m_pFilePath,0,512);

	if (NULL != pFilePath)
	{
		//方便linux下查找文件路径,末端都没带'\'或'/'
		strcpy(m_pFilePath,pFilePath);
		int len = strlen(m_pFilePath);
#ifdef __ENVIRONMENT_WIN32__
		if ('/' == m_pFilePath[len - 1] || '\\' == m_pFilePath[len - 1] )
		{
			m_pFilePath[len - 1] = '\0';
		}
		strcat(m_pFilePath, "\\language");
#else
		if ('/' == m_pFilePath[len - 1])
		{
			m_pFilePath[len - 1] = '\0';
		}
		strcat(m_pFilePath, "/language");
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(m_pFilePath, "language");
#else
		strcpy(m_pFilePath, "language");
#endif
	}

	m_langFilterBites = langFilterBites;
	return Search();
}

void CLanguagePack::Quit()
{
	if( m_pFilePath )
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}
	m_count = 0;
	m_langFilterBites = LANGUAGE_ALL;
	memset(m_languageID, 0, sizeof(m_languageID));
}

int CLanguagePack::GetCount()
{
	return m_count;
}

unsigned long CLanguagePack::GetLanguageID(int index)
{
	assert(index>=0);
	assert(index<m_count);

	return m_languageID[index];
}

unsigned long CLanguagePack::GetDefault()
{
#if defined (__CUSTOM_BR27__)
	int num = m_count;
	if (num > MAX_LANGUAGE_PACK_NUM)
	{
		num = MAX_LANGUAGE_PACK_NUM;
	}
	for ( int i=0;i<m_count;i++)
	{
		if (LANGUAGE_PORTUGUESE == m_languageID[i])
		{
			return LANGUAGE_PORTUGUESE;
		}
	}
	return m_languageID[0];
#else
	assert(m_count > 0);
	return m_languageID[0];
#endif
}

void CLanguagePack::GetLanguageName(unsigned long languageID, char *pszName)
{
	if (LANGUAGE_ENGLISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_ENGLISH);
	}
#if defined(__CUSTOM_BR27__)
	else if (LANGUAGE_CHINESE_S == languageID)
	{
		strcpy(pszName, LANG_UTF8_KOREAN);
	}
#else
	else if (LANGUAGE_CHINESE_S == languageID)
	{
		#if defined(__KR24_IN__) && !defined(__KR24_CN__) || defined(__KOREAN__)
			strcpy(pszName, LANG_UTF8_KOREAN);
		#else
			strcpy(pszName, LANG_UTF8_CHINESE_S);
		#endif
	}
	#if defined(__KR24_IN__) && !defined(__KR24_CN__) || defined(__KOREAN__)
	#else
	else if (LANGUAGE_CHINESE_B == languageID)
	{
		strcpy(pszName, LANG_UTF8_CHINESE_B);
	}
	#endif
#endif
	else if (LANGUAGE_PORTUGUESE == languageID)
	{
		strcpy(pszName, LANG_UTF8_PORTUGUESE);
	}
	else if (LANGUAGE_GREECE == languageID)
	{
		strcpy(pszName, LANG_UTF8_GREECE);
	}
	else if (LANGUAGE_SPAISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_SPAISH);
	}
	else if (LANGUAGE_RUSSIAN == languageID)
	{
		strcpy(pszName, LANG_UTF8_RUSSIAN);
	}
	else if (LANGUAGE_NORWAY == languageID)
	{
		strcpy(pszName, LANG_UTF8_NORWAY);
	}
	else if (LANGUAGE_TURKEY == languageID)
	{
		strcpy(pszName, LANG_UTF8_TURKEY);
	}
	else if (LANGUAGE_ITALY == languageID)
	{
		strcpy(pszName, LANG_UTF8_ITALY);
	}
	else if (LANGUAGE_CZECH == languageID)
	{
		strcpy(pszName, LANG_UTF8_CZECH);
	}
	else if (LANGUAGE_GERMAN == languageID)
	{
		strcpy(pszName, LANG_UTF8_GERMAN);
	}
	else if (LANGUAGE_HEBREW == languageID)
	{
		strcpy(pszName, LANG_UTF8_HEBREW);
	}
#ifndef __CUSTOM_BR27__
	else if (LANGUAGE_JAPANESE == languageID)
	{
		strcpy(pszName, LANG_UTF8_JAPANESE);
	}
#endif
	else if (LANGUAGE_FRENCH == languageID)
	{
		strcpy(pszName, LANG_UTF8_FRENCH);
	}
	else if (LANGUAGE_POLISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_POLISH);
	}
	else if (LANGUAGE_BULGARIAN == languageID)
	{
		strcpy(pszName, LANG_UTF8_BULGARIAN);
	}
#if defined(__CUSTOM_PL14__) || defined(__CUSTOM_US02_NEW__)
	else if (LANGUAGE_INDONESIA == languageID)
	{
		strcpy(pszName, LANG_UTF8_ROMANIAN);
	}
#else
	else if (LANGUAGE_INDONESIA == languageID)
	{
		strcpy(pszName, LANG_UTF8_INDONESIA);
	}
#endif
	else if (LANGUAGE_RUSSIAN_D == languageID)
	{
		strcpy(pszName, LANG_UTF8_RUSSIAN_D);
	}
	else if (LANGUAGE_THAI == languageID)
	{
		strcpy(pszName, LANG_UTF8_THAI);
	}
	else if (LANGUAGE_HUNGARY == languageID)
	{
		strcpy(pszName, LANG_UTF8_HUNGARY);
	}
	else if (LANGUAGE_LITHUANIA == languageID)
	{
		strcpy(pszName, LANG_UTF8_LITHUANIA);
	}
	else if (LANGUAGE_VIETNAMESE == languageID)
	{
		strcpy(pszName, LANG_UTF8_VIETNAMESE);
	}
	else if (LANGUAGE_PERSIAN == languageID)
	{
		strcpy(pszName, LANG_UTF8_PERSIAN);
	}
	else if (LANGUAGE_DUTCH == languageID)
	{
		strcpy(pszName, LANG_UTF8_DUTCH);
	}
# if defined(__CUSTOM_IN25__) || defined(__CUSTOM_GB31__) || defined(__CUSTOM_PL14__)
	else if (LANGUAGE_FINNISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_FINNISH);
	}
#else
	else if (LANGUAGE_FINNISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_ARABIC);
	}
#endif
	else if (LANGUAGE_SWEDISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_SWEDISH);
	}
#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
	else if (LANGUAGE_DANISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_ROMANIAN);
	}
#else
	else if (LANGUAGE_DANISH == languageID)
	{
		strcpy(pszName, LANG_UTF8_DANISH);
	}
#endif
	else
	{
		assert(false);
		strcpy(pszName, LANG_UTF8_ENGLISH);
	}
}


void CLanguagePack::GetLanguageNameForBackDoor(unsigned long languageID, char *pszName, int len)
{
	if (LANGUAGE_ENGLISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_ENGLISH,LANG_UTF8_ENGLISH_IN_CN);
	}
#if defined(__CUSTOM_BR27__)
	else if (LANGUAGE_CHINESE_S == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_KOREAN,LANG_UTF8_KOREAN_CN);
	}
#else
	else if (LANGUAGE_CHINESE_S == languageID)
	{
		#if defined(__KR24_IN__) || defined(__KOREAN__)
			snprintf(pszName, len, "%s(%s)", LANG_UTF8_KOREAN,LANG_UTF8_KOREAN_CN);
		#else
			snprintf(pszName, len, "%s(%s)", LANG_UTF8_CHINESE_S,LANG_UTF8_CHINESE_S_IN_CN);
		#endif
	}
	else if (LANGUAGE_CHINESE_B == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_CHINESE_B,LANG_UTF8_CHINESE_B_IN_CN);
	}
#endif
	else if (LANGUAGE_PORTUGUESE == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_PORTUGUESE,LANG_UTF8_PORTUGUESE_IN_CN);
	}
	else if (LANGUAGE_GREECE == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_GREECE, LANG_UTF8_GREECE_IN_CN);
	}
	else if (LANGUAGE_SPAISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_SPAISH,LANG_UTF8_SPAISH_IN_CN);
	}
	else if (LANGUAGE_RUSSIAN == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_RUSSIAN,LANG_UTF8_RUSSIAN_IN_CN);
	}
	else if (LANGUAGE_NORWAY == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_NORWAY,LANG_UTF8_NORWAY_IN_CN);
	}
	else if (LANGUAGE_TURKEY == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_TURKEY,LANG_UTF8_TURKEY_IN_CN);
	}
	else if (LANGUAGE_ITALY == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_ITALY,LANG_UTF8_ITALY_IN_CN);
	}
	else if (LANGUAGE_CZECH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_CZECH,LANG_UTF8_CZECH_IN_CN);
	}
	else if (LANGUAGE_GERMAN == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_GERMAN,LANG_UTF8_GERMAN_IN_CN);
	}
	else if (LANGUAGE_HEBREW == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_HEBREW,LANG_UTF8_HEBREW_IN_CN);
	}
#ifndef __CUSTOM_BR27__
	else if (LANGUAGE_JAPANESE == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_JAPANESE,LANG_UTF8_JAPANESE_IN_CN);
	}
#endif
	else if (LANGUAGE_FRENCH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_FRENCH,LANG_UTF8_FRENCH_IN_CN);
	}
	else if (LANGUAGE_POLISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_POLISH,LANG_UTF8_POLISH_IN_CN);
	}
	else if (LANGUAGE_BULGARIAN == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_BULGARIAN,LANG_UTF8_BULGARIAN_IN_CN);
	}
#if defined(__CUSTOM_PL14__)
	else if (LANGUAGE_INDONESIA == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_ROMANIAN,LANG_UTF8_ROMANIAN_IN_CN);
	}
#else
	else if (LANGUAGE_INDONESIA == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_INDONESIA,LANG_UTF8_INDONESIA_IN_CN);
	}
#endif
	else if (LANGUAGE_RUSSIAN_D == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_RUSSIAN_D,LANG_UTF8_RUSSIAN_D_IN_CN);
	}
	else if (LANGUAGE_THAI == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_THAI,LANG_UTF8_THAI_IN_CN);
	}
	else if (LANGUAGE_HUNGARY == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_HUNGARY, LANG_UTF8_HUNGARY_IN_CN);
	}
	else if (LANGUAGE_LITHUANIA == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_LITHUANIA, LANG_UTF8_LITHUANIA_IN_CN);
	}
	else if (LANGUAGE_VIETNAMESE == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_VIETNAMESE, LANG_UTF8_VIETNAMESE_IN_CN);
	}
	else if (LANGUAGE_PERSIAN == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_PERSIAN, LANG_UTF8_PERSIAN_IN_CN);
	}
	else if (LANGUAGE_DUTCH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_DUTCH, LANG_UTF8_DUTCH_IN_CN);
	}
#if defined(__CUSTOM_IN25__) || defined(__CUSTOM_GB31__) || defined(__CUSTOM_PL14__)
	else if (LANGUAGE_FINNISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_FINNISH, LANG_UTF8_FINNISH_IN_CN);
	}
#else
	else if (LANGUAGE_FINNISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_ARABIC, LANG_UTF8_ARABIC_IN_CN);
	}
#endif
	else if (LANGUAGE_SWEDISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_SWEDISH, LANG_UTF8_SWEDISH_IN_CN);
	}
#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
	else if (LANGUAGE_DANISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_ROMANIAN, LANG_UTF8_ROMANIAN_IN_CN);
	}
#else
	else if (LANGUAGE_DANISH == languageID)
	{
		snprintf(pszName, len, "%s(%s)", LANG_UTF8_DANISH, LANG_UTF8_DANISH_IN_CN);
	}
#endif
	else
	{		
		snprintf(pszName, len, "%s", "Reserve");
	}
}


int CLanguagePack::FindLanguage(unsigned long ID)
{
	int index = -1;
	for ( int i=0;i<m_count;i++)
	{
		if( m_languageID[i] == ID )
		{
			index = i;
			break;
		}
	}

	return index;
}

bool CLanguagePack::Search()
{
	assert(m_pFilePath);
	bool bRet = false;

	if( SearchDir(m_pFilePath) )
	{
		CheckSort();				
	}
	else
	{
		printf("%s,%d,error:CLanguagePack no language\n",__FILE__,__LINE__);
		m_languageID[0] = LANGUAGE_ENGLISH;
		m_count = 1;
	}
	return true;
}

bool CLanguagePack::SearchDir(const char* path)
{
	bool flag = false;

#ifdef __ENVIRONMENT_WIN32__
	char filepath[512];
	strcpy(filepath,path);
	strcat(filepath,"\\*.dat");

	//首先查找dir中符合要求的文件
	long hFile;
	struct _finddata_t fileinfo;
	if ((hFile=_findfirst(filepath,&fileinfo)) != -1)
	{
		do
		{
			//检查是不是目录
			//如果不是,则进行处理
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				snprintf(filepath, 512, "%s\\%s", path, fileinfo.name);
				if( FilterFile(filepath) )
				{
					flag = true;//至少找到一个则返回true
				}
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}
#else
	struct dirent* ptr=NULL;	
	DIR* dir = NULL;

	dir=opendir(path); 
	if(NULL == dir)
	{
		return false;
	}

	while(NULL != (ptr = readdir(dir)))
	{
		if ((0 == strcmp(ptr->d_name, "."))||(0 == strcmp(ptr->d_name, "..")))
		{
			continue;
		}
		char filepath[512] = {0};
		snprintf(filepath, 512, "%s/%s", path, ptr->d_name);
		if( FilterFile(filepath) )
		{
			flag = true;
		}		
	}
	closedir(dir);
#endif

	return flag;
}

bool CLanguagePack::FilterFile(const char* filename)
{
	if( m_count >= MAX_LANGUAGE_PACK_NUM)
	{
		assert(false);
		return false;
	}

#ifndef __ENVIRONMENT_WIN32__
	//检查后缀名.dat
	char dat[16] = {0};
	strncpy(dat,filename+strlen(filename)-4,4);
	if( 0 != strcmp(dat,".dat") ) return false;
#endif
	STR_TABLE_FILE_HEAD strFileHead;
	memset(&strFileHead,0,sizeof(STR_TABLE_FILE_HEAD));

	FILE* file = fopen(filename,"rb");

	if (NULL == file)
	{
		assert(false);
		return false;
	}

	if (0 != fseek(file, 0, SEEK_SET))
	{
		fclose(file);
		return false;
	}

	if (1 != fread(&strFileHead, sizeof(STR_TABLE_FILE_HEAD), 1, file))
	{
		fclose(file);
		return false;
	}

	fclose(file);


	if (0 == (m_langFilterBites & strFileHead.languageID))
	{
		return false;
	}	

	if( !((strFileHead.stringNum > 0) && (strFileHead.fileLength > 0)))
	{
		return false;
	}
	
	m_languageID[m_count] = strFileHead.languageID;
	m_count += 1;
	return true;	
}

void CLanguagePack::CheckSort()
{
	//数据量不大
	for ( int i=0;i<m_count;i++)
	{
		for (int j=i+1;j<m_count;j++)
		{
			if( m_languageID[j]<m_languageID[i] )
			{
				unsigned long tmp = m_languageID[i];
				m_languageID[i] = m_languageID[j];
				m_languageID[j] = tmp;
			}
		}
	}

	//LANGUAGE 值检查
}




//end

