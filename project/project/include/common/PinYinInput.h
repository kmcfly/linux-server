/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author		: lufen
** Date		: 2008-09-15
** Name		: PinYinInput.h
** Version 	: 1.0
** Description  :
** Modify Record:
***********************************************************************/

#ifndef _PINYININPUT_H_
#define	_PINYININPUT_H_


typedef unsigned long       ULONG;
typedef unsigned char       BYTE;
typedef int                 	  BOOL;

const  BYTE	CNDISNUM = 14;	/*汉字一次显示的个数7 个*/
const  BYTE	PinYinNUM = 7;	/*输入拼音的最大长度*/
const  unsigned long PY_DISP_BUFF_LEN = 64;	

//用于保存每个字母的相关信息
typedef struct _LetterIndex
{
	char letter;
	unsigned int index;
}  LetterIndex;

//用于保存每个拼音的相关信息
typedef struct _PinYinIndex
{
	char pinYin[10];
	unsigned int index;
}  PinYinIndex;

//用于保存当前字母前10个拼音组
//合的相关信息
typedef struct _ReturnWord
{
	unsigned int index;		
	unsigned int wordID;		
	char word[10];
}  ReturnWord;

class CPinYinInput
{
public:
	static CPinYinInput* Instance();
	virtual ~CPinYinInput();

	bool Initial(const char* path);
	void Quit();
	
	//获的用户输入的字符所对应的一串汉字
	void	GetText(char* str);

	//获的用户输入数字所对应的汉字
	void GetSingleText(char* str);

	//设置从键盘上获取字符串
	void SetChar(const char *str, BYTE len);
	
	//设置从键盘上获取数字
	void SetNumber(const char str);

	//获取上一页的信息
	bool GetPageUp(void);
	
	//获取下一页的信息
	bool GetPageDown(void);

	//获的当前汉字的所有页数
	unsigned short GetMaxPage(void);

	//设置当前页并获取汉前页的汉字
	void SetCurrPageInfo(unsigned short page);

	//清空当前缓存中的数据
	void ClearTextBuff();

protected:
	CPinYinInput();	
	
private:
	//获取当前用户输入拼音的相关信息 
 	void GetPinYinInfo(void);

	//获取当前获音所对应的汉字
	void GetChineseInfo(ULONG offset);

	//设置当前保存汉字的buffer
	BOOL SetCNBuffer(void);

	//获的每页该显示的字
	void GetDisplayPageInfo(unsigned int page);
	
	//unsigned int 	m_WordStaAddr;			/*用于记录当前汉字汉字的起始地址*/
	unsigned int 	m_WordNum;				/*用于记录当前汉字的总个数*/
	int				m_WordBufferLen;		//找到汉字缓冲区长度
	int				m_dispLen;				//有效显示字符区长度
	
	char* 			m_WordBuffer;			/*用于存放查找到的汉字buffer*/
	char*			m_pFilePath;			//中文词典路径
	char			m_Inputnum;				/*用于保存从键盘上获的数字*/
	BYTE			m_PageNum;				/*用于记录页数*/
	char			m_displaybuf[PY_DISP_BUFF_LEN];	/*用于存储将要显示的7个汉字*/
	char			m_Inputstr[PinYinNUM];	/*用于保存从键盘上获的字符串*/
};


#endif
