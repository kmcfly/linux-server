
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

	//��ʼ��: ���԰�·�������˹���
	bool Initail(const char *pFilePath, unsigned long langFilterBites);
	void Quit();

	//��ȡ���ж��ٸ����԰�,��������
	int GetCount();

	//�����,��ȡ��Ӧ������ID
	unsigned long GetLanguageID(int index);
	unsigned long GetDefault();

	//
	void GetLanguageName(unsigned long languageID, char *pszName);
	static void GetLanguageNameForBackDoor(unsigned long languageID, char *pszName, int len);


	//��������ID��Ӧ�ĵ�ǰ���,-1��ʾû��
	int FindLanguage(unsigned long ID);

	//ִ������
	bool Search();

protected:
	bool SearchDir(const char* path);						//����·���е�ÿ���ļ�,��������Ŀ¼
	bool FilterFile(const char* filename);					//���˷�����ID�ļ�,����¼����ӵ�е����԰�
	void CheckSort();										//���򲢼������IDֵ��Ч��

protected:
	CLanguagePack();

	char*				m_pFilePath;						//���԰�·��
	unsigned long		m_langFilterBites;						//���԰��������
	int					m_count;							//·�������԰�����
	unsigned long		m_languageID[MAX_LANGUAGE_PACK_NUM];	//���԰�ID
};

#endif

//end
