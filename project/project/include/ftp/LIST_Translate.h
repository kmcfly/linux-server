/*
*对LIST命令返回值进行解析，字符串分成需要的字串
*/
#ifndef		__LIST_TRANSLATE_H__
#define		__LIST_TRANSLATE_H__

#include <list>
#include <string>
#include <string.h>
using namespace std;


#define MAXLISTCMDLENGTH	256		//文件名长度最长为196

typedef struct _ListCMD_info_
{
    int  nType;
	char cFileName[256];
	char cFileDate[20];
	char cFileSize[20];
	char cFileAuthority[20];
}LISTCMD_INFO;

enum
{
	DIRTYPE,
	FILETYPE,
	LINKTYPE,
	RDONLY,
};

class CLIST_Translate
{
public:
	explicit CLIST_Translate(std::list<char *>);
	~CLIST_Translate();
	
	LISTCMD_INFO *GetTransInfo();
	//LISTCMD_INFO_OUT *GetTransInfo();

	unsigned char m_GetInfoCount();
	bool Translate(void);
	
	char *GetPrePathName(char *PathName);
	bool StrConvert(char *iter,std::string str[]);

protected:
	
private:
	std::list<char *>				m_listOriginal;
	LISTCMD_INFO					*m_plistTrans;
	unsigned char					m_InfoCount;

};

#endif
