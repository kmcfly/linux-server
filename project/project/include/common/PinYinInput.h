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

const  BYTE	CNDISNUM = 14;	/*����һ����ʾ�ĸ���7 ��*/
const  BYTE	PinYinNUM = 7;	/*����ƴ������󳤶�*/
const  unsigned long PY_DISP_BUFF_LEN = 64;	

//���ڱ���ÿ����ĸ�������Ϣ
typedef struct _LetterIndex
{
	char letter;
	unsigned int index;
}  LetterIndex;

//���ڱ���ÿ��ƴ���������Ϣ
typedef struct _PinYinIndex
{
	char pinYin[10];
	unsigned int index;
}  PinYinIndex;

//���ڱ��浱ǰ��ĸǰ10��ƴ����
//�ϵ������Ϣ
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
	
	//����û�������ַ�����Ӧ��һ������
	void	GetText(char* str);

	//����û�������������Ӧ�ĺ���
	void GetSingleText(char* str);

	//���ôӼ����ϻ�ȡ�ַ���
	void SetChar(const char *str, BYTE len);
	
	//���ôӼ����ϻ�ȡ����
	void SetNumber(const char str);

	//��ȡ��һҳ����Ϣ
	bool GetPageUp(void);
	
	//��ȡ��һҳ����Ϣ
	bool GetPageDown(void);

	//��ĵ�ǰ���ֵ�����ҳ��
	unsigned short GetMaxPage(void);

	//���õ�ǰҳ����ȡ��ǰҳ�ĺ���
	void SetCurrPageInfo(unsigned short page);

	//��յ�ǰ�����е�����
	void ClearTextBuff();

protected:
	CPinYinInput();	
	
private:
	//��ȡ��ǰ�û�����ƴ���������Ϣ 
 	void GetPinYinInfo(void);

	//��ȡ��ǰ��������Ӧ�ĺ���
	void GetChineseInfo(ULONG offset);

	//���õ�ǰ���溺�ֵ�buffer
	BOOL SetCNBuffer(void);

	//���ÿҳ����ʾ����
	void GetDisplayPageInfo(unsigned int page);
	
	//unsigned int 	m_WordStaAddr;			/*���ڼ�¼��ǰ���ֺ��ֵ���ʼ��ַ*/
	unsigned int 	m_WordNum;				/*���ڼ�¼��ǰ���ֵ��ܸ���*/
	int				m_WordBufferLen;		//�ҵ����ֻ���������
	int				m_dispLen;				//��Ч��ʾ�ַ�������
	
	char* 			m_WordBuffer;			/*���ڴ�Ų��ҵ��ĺ���buffer*/
	char*			m_pFilePath;			//���Ĵʵ�·��
	char			m_Inputnum;				/*���ڱ���Ӽ����ϻ������*/
	BYTE			m_PageNum;				/*���ڼ�¼ҳ��*/
	char			m_displaybuf[PY_DISP_BUFF_LEN];	/*���ڴ洢��Ҫ��ʾ��7������*/
	char			m_Inputstr[PinYinNUM];	/*���ڱ���Ӽ����ϻ���ַ���*/
};


#endif
