
#ifndef _LANGUAGE_PACK_H_
#define _LANGUAGE_PACK_H_

#include "dvrdvsdef.h"
#include <vector>

extern const char LANG_UTF8_ENGLISH[];
extern const char LANG_UTF8_CHINESE_S[];
extern const char LANG_UTF8_CHINESE_B[];
extern const char LANG_UTF8_PORTUGUESE[];
extern const char LANG_UTF8_GREECE[];
extern const char LANG_UTF8_SPAISH[];
extern const char LANG_UTF8_RUSSIAN[];
extern const char LANG_UTF8_NORWAY[];
extern const char LANG_UTF8_TURKEY[];
extern const char LANG_UTF8_ITALY[];
extern const char LANG_UTF8_CZECH[];
extern const char LANG_UTF8_GERMAN[];
extern const char LANG_UTF8_HEBREW[];
extern const char LANG_UTF8_JAPANESE[];
extern const char LANG_UTF8_FRENCH[];
extern const char LANG_UTF8_POLISH[];
extern const char LANG_UTF8_BULGARIAN[];

class CLanguagePack
{
public:
	~CLanguagePack();

	static CLanguagePack* Instance();

	//初始化: 语言包路径、过滤规则
	bool Initail(const char *pFilePath, unsigned long langFilterBites);
	void Quit();

	//获取共有多少个语言包,按序排列
	int GetCount();

	//按序号,获取相应的语言ID
	unsigned long GetLanguageID(int index);
	unsigned long GetDefault();

	//
	void GetLanguageName(unsigned long languageID, char *pszName);
	static void GetLanguageNameForBackDoor(unsigned long languageID, char *pszName, int len);


	//查找语言ID对应的当前序号,-1表示没有
	int FindLanguage(unsigned long ID);

	//执行搜索
	bool Search();

protected:
	bool SearchDir(const char* path);						//查找路径中的每个文件,不查找子目录
	bool FilterFile(const char* filename);					//过滤非语言ID文件,并记录本机拥有的语言包
	void CheckSort();										//排序并检查语言ID值有效性

protected:
	CLanguagePack();

	char*				m_pFilePath;						//语言包路径
	unsigned long		m_langFilterBites;						//语言包处理规则
	int					m_count;							//路径下语言包个数
	unsigned long		m_languageID[MAX_LANGUAGE_PACK_NUM];	//语言包ID
};

#endif

//end
