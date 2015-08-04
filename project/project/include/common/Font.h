/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-07-10
** Name         : Font.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _FONT_H_
#define _FONT_H_
#include "Typedef.h"
#include "PUB_common.h"

#define GUI_RGB32_TO_RGB1555(color)  (\
	(unsigned short)\
	(\
	(0x8000) | \
	(((0xff & (color >> 16)) >> 3) << 10) | \
	(((0xff & (color >> 8)) >> 3) << 5) | \
	((0xff & color) >> 3)\
	)\
	)

#define GUI_UTF8_TO_UCS2(PTR_UTF8, UCS2_ID, SIZE_UTF8) \
	{ \
		int iiii = 0; \
		unsigned char ch_uft8 = PTR_UTF8[iiii++]; \
		if((ch_uft8 & 0x80) == 0) \
		{ \
			UCS2_ID = ch_uft8; \
			SIZE_UTF8 = 1; \
		} \
		else if((ch_uft8 & 0xe0) == 0xe0) \
		{ \
			UCS2_ID = (ch_uft8 & 0x1F) << 12; \
			ch_uft8 = PTR_UTF8[iiii++]; \
			UCS2_ID |= (ch_uft8 & 0x3F) << 6; \
			ch_uft8 = PTR_UTF8[iiii++]; \
			UCS2_ID |= (ch_uft8 & 0x3F); \
			SIZE_UTF8 = 3; \
		} \
		else \
		{ \
			UCS2_ID = (ch_uft8 & 0x3F) << 6; \
			ch_uft8 = PTR_UTF8[iiii++]; \
			UCS2_ID |= (ch_uft8 & 0x3F); \
			SIZE_UTF8 = 2; \
		} \
	}

const BYTE MAX_FONT_HEIGHT = 20;

#define FONT_USE_BORD 0

#if FONT_USE_BORD
const unsigned long MAX_FONT_CHAR_LEN = 108;
#else
const unsigned long MAX_FONT_CHAR_LEN = 54;
#endif

namespace GUI
{
	enum FONT_CODE_TYPE
	{
		FONT_CODE_UNICODE = 0,		//iso10646
		FONT_CODE_GB2312,			//gb2312
		FONT_CODE_BIG5,				//big5
	};

/* �ļ���ʽ
 *			[FONT_FILE_HEAD]
 *
 *			[FONT_FILE_SECTION 0]
 *			[FONT_FILE_SECTION 1]
 *			[FONT_FILE_SECTION N]
 *
 *			[FONT_FILE_INDEX 0]
 *			[FONT_FILE_INDEX 1]
 *			[FONT_FILE_INDEX N]
 *
 *			[FONT_FILE_DATA ERR]
 *			[FONT_FILE_DATA 0]
 *			[FONT_FILE_DATA 1]
 *			[FONT_FILE_DATA N]
*/

	typedef struct _font_file_head
	{
		unsigned long	fontType;		//���弯��������
		unsigned long	fontSectionAddr;//�����λ����Ϣ�׵�ַ(������ļ��Ŀ�ͷ)
		unsigned long	fontIndexAddr;	//�������������׵�ַ(������ļ��Ŀ�ͷ)
		unsigned long	fontDataAddr;	//����������Ϣ�׵�ַ(������ļ��Ŀ�ͷ)
		unsigned long	fontDataLen;	//���������ܳ���

		unsigned long	charNum;		//�ַ���Ŀ����Ŀ
		unsigned long	sectionNum;		//�����ݸ���

	}FONT_FILE_HEAD;

	typedef struct _font_file_section
	{
		unsigned long	low;			//���α������
		unsigned long	hi;				//���α������
		unsigned long	indexNum;		//�����ָ���[����������]
		unsigned long	indexAddr;		//��������������׵�ַ(�������������)
	}FONT_FILE_SECTION;

#ifdef __ENVIRONMENT_WIN32__
#if defined(_CREATE_FONT) || defined (_READER_FONT)
	const char FRAME_BUFFER_PIXSEL_NUM = 4;
#else
	const char FRAME_BUFFER_PIXSEL_NUM = 2;
#endif
#else
	const char FRAME_BUFFER_PIXSEL_NUM = 2;
#endif

	const unsigned short FONT_ERR_CHAR_REPLACE = 0x3F;	//'?'
	const unsigned short FONT_ERR_CHAR_MASK =	 0x3F;	//'?',�����ֿ�ʱ,������޷���������ģ�ô��ַ����

class CFont
{
public:
	enum
	{
		FONT_ERR_UNENCODE	= -1,	//û�б�����
		FONT_ERR_OK			= 0,	//
		FONT_ERR_OVERFLOW	= 1,	//�������ֹ���
	};

public:
	CFont();
	~CFont();

	static void SetDrawBorder(bool bDraw, unsigned long color);

	bool Initial(const char *pFilePath);
	void Quit();

	int GetBitmapBits(unsigned char *pBitmap, unsigned short width, unsigned short & cx, unsigned short & cy, unsigned short charID, \
						unsigned long color, unsigned char bitsNum = FRAME_BUFFER_PIXSEL_NUM, unsigned char zoom = 1, bool bForceDrawBorder = false);
	bool GetFontSize(unsigned short & cx, unsigned short &cy, unsigned short charID, unsigned char zoom = 1);
	bool GetFont(unsigned char *pFontBuf, unsigned long & bufLen, unsigned short charID);

#ifdef _CREATE_FONT
	void AddSection(unsigned long low, unsigned long hi);
	void SetBitmapBits(unsigned char *pBitmap, unsigned short width, unsigned char cx, unsigned char cy, unsigned short charID, bool bErrChar = false);
	unsigned long* CharIndex(unsigned short charID);
	void SaveFile();
#endif

#ifdef _READER_FONT
public:
#else
private:
#endif

	void UnZipFont();
	//////////////////////////////////////////////////////////////////////////
	void FilePath(char *pFilePath);
	unsigned char* CharID(unsigned short charID);
	//////////////////////////////////////////////////////////////////////////

	char*				m_pFilePath;
	FONT_FILE_HEAD*		m_pFontFileHead;
	FONT_FILE_SECTION*	m_pSection;
	unsigned long*		m_pIndex;
	unsigned char*		m_pFontData;

	unsigned long		m_sectionNum;

#ifdef _CREATE_FONT
	unsigned long		m_dataLen;
	unsigned long		m_dataMaxLen;
	unsigned long		m_indexMaxLen;
#endif

private:
	static bool				s_bDrawBorder;
	static unsigned long	s_colorFontBorder;

	static std::string		s_strPath;
};
}

#endif //_FONT_H_
