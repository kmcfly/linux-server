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
#include "Font.h"
#include <assert.h>

using namespace GUI;

const unsigned long MAX_FONT_SECTION = 16;
const unsigned long	MAX_FONT_NUM	= 30000;
//const unsigned long FONT_BORD_COLOR = 0xff282828;

bool GUI::CFont::s_bDrawBorder = true;
unsigned long GUI::CFont::s_colorFontBorder = 0xff282828;

#ifdef __ENVIRONMENT_WIN32__
std::string GUI::CFont::s_strPath = "ui\\font.dat";
#else
std::string GUI::CFont::s_strPath = "ui/font.dat";
#endif

void GUI::CFont::SetDrawBorder(bool bDraw, unsigned long color)
{
	s_bDrawBorder = bDraw;
	s_colorFontBorder = color;
}

GUI::CFont::CFont():m_pFilePath(NULL), 
m_pFontFileHead(NULL),
m_pSection(NULL),
m_pIndex(NULL),
m_pFontData(NULL), 
m_sectionNum(0)
{

}

GUI::CFont::~CFont()
{

}

bool GUI::CFont::Initial(const char *pFilePath)
{
	assert(NULL != pFilePath);
	assert(NULL == m_pFilePath);
	assert(NULL == m_pFontFileHead);
	assert(NULL == m_pSection);
	assert(NULL == m_pIndex);
	assert(NULL == m_pFontData);
	assert(0 == m_sectionNum);

	m_pFilePath = new char [512];
	strcpy(m_pFilePath, pFilePath);

	m_pFontFileHead = new FONT_FILE_HEAD;
	memset(m_pFontFileHead, 0, sizeof(FONT_FILE_HEAD));

#ifdef _CREATE_FONT
	m_sectionNum = 0;
	m_pSection = new FONT_FILE_SECTION[MAX_FONT_SECTION];
	memset(m_pSection, 0, sizeof(FONT_FILE_SECTION) * MAX_FONT_SECTION);

	m_indexMaxLen = MAX_FONT_NUM;
	m_pIndex = new unsigned long[m_indexMaxLen];
	memset(m_pIndex, 0, sizeof(unsigned long) * MAX_FONT_NUM);

	//
	m_dataLen = 0;
	m_dataMaxLen = MAX_FONT_NUM * MAX_FONT_CHAR_LEN;
	m_pFontData = new unsigned char [m_dataMaxLen];	//最大字符的数据位置
	memset(m_pFontData, 0, m_dataLen);
#else
	//Open the font file
	FilePath(m_pFilePath);

	char szCmd[512] = {0};
#ifdef __COMPRESS_UI_FONT__
	#if defined(__CHIPTI__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)
		UnZipFont();
	#else
		snprintf(szCmd, 512, "cd /nfsdir && tar -xzvf /mnt/mtd/ui/ui.tar font.dat");
		printf("%s,%d,%s\n",__FILE__, __LINE__,szCmd);
		DVRSystem(szCmd);
	#endif

	snprintf(m_pFilePath, 512, "/nfsdir/font.dat");
#endif

	FILE *pFile = fopen(m_pFilePath, "rb");

	if (NULL == pFile)
	{
		printf("%s:%s:%d, open file %s error\n", __FUNCTION__, __FILE__, __LINE__, m_pFilePath);
		return false;
	}

	//Get font file head information from file
	if (1 != fread(m_pFontFileHead, sizeof(FONT_FILE_HEAD), 1, pFile))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	m_sectionNum = m_pFontFileHead->sectionNum;
	assert(m_sectionNum < MAX_FONT_SECTION);
	m_pSection = new FONT_FILE_SECTION[m_sectionNum];
	m_pIndex = new unsigned long[m_pFontFileHead->charNum];
	m_pFontData = new unsigned char[m_pFontFileHead->fontDataLen];

	if (0 != fseek(pFile, m_pFontFileHead->fontSectionAddr, SEEK_SET))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	if (1 != fread(m_pSection, m_sectionNum * sizeof(FONT_FILE_SECTION), 1, pFile))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	if (0 != fseek(pFile, m_pFontFileHead->fontIndexAddr, SEEK_SET))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	if (1 != fread(m_pIndex, m_pFontFileHead->charNum * sizeof(unsigned long), 1, pFile))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	if (0 != fseek(pFile, m_pFontFileHead->fontDataAddr, SEEK_SET))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	if (1 != fread(m_pFontData, m_pFontFileHead->fontDataLen, 1, pFile))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}

	fclose(pFile);
	pFile = NULL;

#ifdef __COMPRESS_UI_FONT__
	snprintf(szCmd, 512, "rm -rf /nfsdir/font.dat  && cd ~");
	DVRSystem(szCmd);
#endif

#endif

	assert (NULL != m_pFontData);

	return true;
}

void GUI::CFont::Quit()
{
	if (NULL != m_pFilePath)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}

	if (NULL != m_pFontFileHead)
	{
		delete m_pFontFileHead;
		m_pFontFileHead = NULL;
	}

	if(NULL != m_pSection)
	{
		delete [] m_pSection;
		m_pSection = NULL;
	}

	if(NULL != m_pIndex)
	{
		delete [] m_pIndex;
		m_pIndex = NULL;
	}

	if (NULL != m_pFontData)
	{
		delete [] m_pFontData;
		m_pFontData = NULL;
	}

	m_sectionNum = 0;

#ifdef _CREATE_FONT
	m_dataLen = 0;
	m_dataMaxLen = 0;
	m_indexMaxLen = 0;
#endif
}

#ifdef _CREATE_FONT

void GUI::CFont::AddSection(unsigned long low, unsigned long hi)
{
	unsigned long offset = 0;
	for (int i = 0; i < m_sectionNum; i++)
	{
		offset += m_pSection[i].indexNum;
	}

	m_pSection[m_sectionNum].low = low;
	m_pSection[m_sectionNum].hi = hi;
	m_pSection[m_sectionNum].indexNum = hi - low + 1;

	if(offset + m_pSection[m_sectionNum].indexNum >= m_indexMaxLen)
	{
		m_indexMaxLen = 0xFFFF;
		unsigned long* pNewIndex = new unsigned long[m_indexMaxLen];
		memset(pNewIndex, 0, sizeof(unsigned long) * m_indexMaxLen);
		memcpy(pNewIndex, m_pIndex, sizeof(unsigned long) * offset);
		delete [] m_pIndex;
		m_pIndex = pNewIndex;
	}

	m_pSection[m_sectionNum].indexAddr = offset;

	m_sectionNum += 1;
}

void GUI::CFont::SetBitmapBits(unsigned char *pBitmap, unsigned short width, unsigned char cx, unsigned char cy, unsigned short charID, bool bErrChar)
{
	assert (NULL != pBitmap);

	//Alloc temp buf
	unsigned short len = (((cy * cx) + 7) >> 3) + sizeof(unsigned long);
	unsigned char *pFont = new unsigned char [len];
	assert (NULL != pFont);
	memset(pFont, 0, len);

	*(reinterpret_cast<unsigned long *>(pFont)) = (charID << 16) + (cx << 8) + cy;

	unsigned char *pPicture = NULL;
	unsigned char *pFontStyle = pFont + sizeof(unsigned long);

	for (short y=0; y<cy; ++y)
	{
		pPicture = pBitmap + (y * width * FRAME_BUFFER_PIXSEL_NUM);

		for (short x=0; x<cx; ++x)
		{
			if (0x0 != *(reinterpret_cast<unsigned long *>(pPicture)))
			{
				pFontStyle[(y*cx+x)>>3] |= (0x01 << ((y*cx+x) & 0x0007));
			}

			pPicture += FRAME_BUFFER_PIXSEL_NUM;
		}
	}

	if(bErrChar)
	{
		assert(0 == m_dataLen);

		//设置字模
		m_dataLen = 0;
		unsigned char* pFontData = m_pFontData + m_dataLen;
		memcpy(pFontData, pFont, len);
		m_dataLen += len;
	}
	else
	{
		if(m_dataLen + len >= m_dataMaxLen)
		{
			//防止过界
			m_dataMaxLen += MAX_FONT_CHAR_LEN * 2000;
			unsigned char* pNewData = new unsigned char[m_dataMaxLen];
			memset(pNewData, 0, m_dataMaxLen);
			memcpy(pNewData, m_pFontData, m_dataLen);
			delete []m_pFontData;
			m_pFontData = pNewData;
		}

		//设置索引
		unsigned long* pIndex = CharIndex(charID);
		*pIndex = m_dataLen;

		//设置字模
		unsigned char* pFontData = m_pFontData + m_dataLen;
		memcpy(pFontData, pFont, len);
		m_dataLen += len;
	}

	delete [] pFont;
}

unsigned long* GUI::CFont::CharIndex(unsigned short charID)
{
	for (int i = 0; i < m_sectionNum; i++)
	{
		if(m_pSection[i].low <= static_cast<unsigned long>(charID) &&
			m_pSection[i].hi >= static_cast<unsigned long>(charID))
		{
			return m_pIndex + m_pSection[i].indexAddr + charID - m_pSection[i].low;
		}
	}
}

#endif

int GUI::CFont::GetBitmapBits(unsigned char *pBitmap, unsigned short width, unsigned short & cx, unsigned short & cy, unsigned short charID, \
							   unsigned long color, unsigned char bitsNum/* = FRAME_BUFFER_PIXSEL_NUM*/, unsigned char zoom/* = 1*/, bool bForceDrawBorder)
{
	assert (NULL != pBitmap);

	unsigned short xNum = cx, yNum = cy;

	//Alloc temp buf
	unsigned char *pFont = CharID(charID);
	if(!pFont)
	{
		//未找到字符
		return FONT_ERR_UNENCODE;
	}

	unsigned long fontInfo = 0;
	memcpy(&fontInfo, pFont, sizeof(unsigned long));

	cy = static_cast<unsigned short>(fontInfo & 0x000000ff); 
	cx = static_cast<unsigned short>(fontInfo & 0x0000ff00) >> 8;

	if(cx <= 0 || cy <= 0)
	{
		//字体中没有字模信息
		//printf("(file=%s.line=%d) error.get bitmap bit.char code = 0x%.4X.\n", __FILE__, __LINE__, charID);
		
		pFont = CharID(FONT_ERR_CHAR_REPLACE);
		fontInfo = 0;
		memcpy(&fontInfo, pFont, sizeof(unsigned long));

		cy = static_cast<unsigned short>(fontInfo & 0x000000ff); 
		cx = static_cast<unsigned short>(fontInfo & 0x0000ff00) >> 8;
	}

	if (cx > xNum)
	{
		//输出字符过界
		return FONT_ERR_OVERFLOW;
	}

	cx = (cx <= xNum) ? cx : xNum;
	cy = (cy <= yNum) ? cy : yNum;

	unsigned long fontColor = color, bordColor = s_colorFontBorder;
	if (2 == bitsNum)
	{
		fontColor = GUI_RGB32_TO_RGB1555(color);
		bordColor = GUI_RGB32_TO_RGB1555(s_colorFontBorder);
	}

	assert ((0 < zoom) && (zoom < 4));
	if ((0 == zoom) && (4 <= zoom))
	{
		zoom = 1;
	}

	unsigned char *pPicture = NULL, *pTemp = NULL;
	unsigned char *pFontStyle = pFont + sizeof(unsigned long);

	bool bBord = false;
	unsigned long index = 0;
	unsigned char style = 0;

	for (short y=0; y<cy; ++y)
	{
		pPicture = pBitmap + ((y * zoom) * (width * bitsNum));
		for (short x=0; x<cx; ++x)
		{
			index = y*cx+x;
			style = pFontStyle[index >> 3];
			if (style & (0x01 << (index & 0x0007)))
			{
				for (unsigned char h=0; h<zoom; ++h)
				{
					for (unsigned char w=0; w<zoom; ++w)
					{
						pTemp = pPicture + (h * width * bitsNum) + (w * bitsNum);
						memcpy(pTemp, &fontColor, bitsNum);
					}
				}
			}
			else
			{
				if(bForceDrawBorder || s_bDrawBorder)
				{
					bBord = false;

					if (0 < y)
					{
						index = (y-1)*cx+x;
						style = pFontStyle[index >> 3];
						if (style & (0x01 << (index & 0x0007)))
						{
							bBord = true;
							for (unsigned char w=0; w<zoom; ++w)
							{
								memcpy(pPicture + (w * bitsNum), &bordColor, bitsNum);
							}
						}
					}

					if (!bBord && (y < (cy - 1)))
					{
						index = (y+1)*cx+x;
						style = pFontStyle[index >> 3];
						if (style & (0x01 << (index & 0x0007)))
						{
							bBord = true;
							for (unsigned char w=0; w<zoom; ++w)
							{
								memcpy(pPicture + (w * bitsNum), &bordColor, bitsNum);
							}
						}
					}

					if (!bBord && (0 < x))
					{
						index = y*cx+x-1;
						style = pFontStyle[index >> 3];
						if (style & (0x01 << (index & 0x0007)))
						{
							bBord = true;
							for (unsigned char h=0; h<zoom; ++h)
							{
								memcpy(pPicture + (h * width * bitsNum), &bordColor, bitsNum);
							}
						}
					}

					if (!bBord && (x < (cx - 1)))
					{
						index = y*cx+x+1;
						style = pFontStyle[index >> 3];
						if (style & (0x01 << (index & 0x0007)))
						{
							for (unsigned char h=0; h<zoom; ++h)
							{
								memcpy(pPicture + (h * width * bitsNum), &bordColor, bitsNum);
							}
						}
					}
				}
			}

			pPicture += (zoom * bitsNum);
		}
	}

	cx *= zoom;
	cy *= zoom;
	return FONT_ERR_OK;
}

bool GUI::CFont::GetFontSize(unsigned short & cx, unsigned short &cy, unsigned short charID, unsigned char zoom/* = 1*/)
{
	assert ((0 < zoom) && (zoom < 4));
	if ((0 == zoom) && (4 <= zoom))
	{
		zoom = 1;
	}

	unsigned char *pFont = CharID(charID);

	if(!pFont)
	{
		return false;
	}

	unsigned long fontInfo = 0;
	memcpy(&fontInfo, pFont, sizeof(unsigned long));

	cy = static_cast<unsigned short>(fontInfo & 0x000000ff) * zoom; 
	cx = (static_cast<unsigned short>(fontInfo & 0x0000ff00) >> 8) * zoom;

	if(cx <= 0 || cy <=0)
	{
		//字模信息错误时,使用'?'替换
		pFont = CharID(FONT_ERR_CHAR_REPLACE);
		fontInfo = 0;
		memcpy(&fontInfo, pFont, sizeof(unsigned long));

		cy = static_cast<unsigned short>(fontInfo & 0x000000ff) * zoom; 
		cx = (static_cast<unsigned short>(fontInfo & 0x0000ff00) >> 8) * zoom;
	}

	return true;
}

bool GUI::CFont::GetFont(unsigned char *pFontBuf, unsigned long & bufLen, unsigned short charID)
{
	assert (NULL != pFontBuf);

	unsigned char *pFont = CharID(charID);

	if(!pFont)
	{
		return false;
	}

	unsigned long fontInfo = 0;
	memcpy(&fontInfo, pFont, sizeof(unsigned long));

	unsigned short cy = static_cast<unsigned short>(fontInfo & 0x000000ff); 
	unsigned short cx = static_cast<unsigned short>(fontInfo & 0x0000ff00) >> 8;

	if(cx <= 0 || cy <=0)
	{
		//字模信息错误时,使用'?'替换
		pFont = CharID(FONT_ERR_CHAR_REPLACE);
		fontInfo = 0;
		memcpy(&fontInfo, pFont, sizeof(unsigned long));

		cy = static_cast<unsigned short>(fontInfo & 0x000000ff); 
		cx = static_cast<unsigned short>(fontInfo & 0x0000ff00) >> 8;
	}

	unsigned short len = sizeof(unsigned long) + (((cy * cx) + 7) >> 3);
	
	assert (len <= bufLen);

	memcpy(pFontBuf, pFont, len);

	fontInfo = (0xffff0000 & (charID << 16)) | (fontInfo & 0x0000ffff);
	memcpy(pFontBuf, &fontInfo, sizeof(unsigned long));

	bufLen = len;

	return true;
}

//////////////////////////////////////////////////////////////////////////
void GUI::CFont::FilePath(char *pFilePath)
{
	assert (NULL != pFilePath);

#ifdef __ENVIRONMENT_WIN32__
	strcat(pFilePath, s_strPath.c_str());
#else
	strcat(pFilePath, s_strPath.c_str());
#endif
}

unsigned char* GUI::CFont::CharID(unsigned short charID)
{
	for (int i = 0; i < m_sectionNum; i++)
	{
		if(m_pSection[i].low <= static_cast<unsigned long>(charID) &&
			m_pSection[i].hi >= static_cast<unsigned long>(charID))
		{
			unsigned long offset = m_pSection[i].indexAddr + charID - m_pSection[i].low;
			return m_pFontData + m_pIndex[offset];
		}
	}

	return NULL;
}

#ifdef _CREATE_FONT
void GUI::CFont::SaveFile()
{
	assert(NULL != m_pFontData);
	assert(0 != m_sectionNum);

	//Open or Create a file what has font information and font data.
	FilePath(m_pFilePath);
	FILE *pFile = fopen(m_pFilePath, "wb+");

	//Write font information and font data into file.

	unsigned long charNum = 0, indexLen = 0;
	for (int i = 0; i < m_sectionNum; i++)
	{
		charNum += m_pSection[i].indexNum;
	}

	m_pFontFileHead->fontType = FONT_CODE_UNICODE;
	m_pFontFileHead->fontSectionAddr = sizeof(FONT_FILE_HEAD);
	m_pFontFileHead->fontIndexAddr = m_pFontFileHead->fontSectionAddr + sizeof(FONT_FILE_SECTION) * m_sectionNum;
	m_pFontFileHead->fontDataAddr = m_pFontFileHead->fontIndexAddr + sizeof(unsigned long) * charNum;
	m_pFontFileHead->fontDataLen = m_dataLen;

	m_pFontFileHead->charNum = charNum;
	m_pFontFileHead->sectionNum = m_sectionNum;

	if (1 != fwrite(m_pFontFileHead, sizeof(FONT_FILE_HEAD), 1, pFile))
	{
		assert(false);
	}

	if (1 != fwrite(m_pSection, sizeof(FONT_FILE_SECTION) * m_pFontFileHead->sectionNum, 1, pFile))
	{
		assert(false);
	}

	if(1 != fwrite(m_pIndex, sizeof(unsigned long) * m_pFontFileHead->charNum, 1, pFile))
	{
		assert(false);
	}

	if (1 != fwrite(m_pFontData, m_pFontFileHead->fontDataLen, 1, pFile))
	{
		assert(false);
	}

	//Close the font file.
	fclose(pFile);
}
#endif

void GUI::CFont::UnZipFont()
{
	/*
	TI的解压过程
	#解压UI部分
	rm /nfsdir/ui -rf 
	mkdir /nfsdir/ui -p
	cp /mnt/mtd/ui/ui.tar.lzma /nfsdir/
	cd /nfsdir/
	unlzma ui.tar.lzma
	cd ui
	tar xvf ../ui.tar
	rm ui.tar

	mv /nfsdir/ui/font.dat /nfsdir/
	rm -rf /nfsdir/ui
	cd /mnt/mtd
	*/

#if defined(__CHIPTI__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)
	char szCmd[512] = {0};

	//创建/nfsdir/ui目录
	snprintf(szCmd, 512, "mkdir /nfsdir/ui -p");
	do 
	{
		if (0 !=  DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);

	//拷贝lzma数据
	snprintf(szCmd, 512, "cp /mnt/mtd/ui/ui.tar.lzma /nfsdir/");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);

	//解压lzma数据
	snprintf(szCmd, 512, "cd /nfsdir && unlzma ui.tar.lzma");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);

	PUB_Sleep(50);

	//解压tar数据
	snprintf(szCmd, 512, "cd /nfsdir/ui && tar -xvf /nfsdir/ui.tar");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);
	PUB_Sleep(50);

	//拷贝字库
	snprintf(szCmd, 512, "mv /nfsdir/ui/font.dat /nfsdir/");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);
	PUB_Sleep(50);

	//删除ui.tar
	snprintf(szCmd, 512, "rm /nfsdir/ui.tar");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);
	PUB_Sleep(50);


	//删除ui
	snprintf(szCmd, 512, "rm -rf /nfsdir/ui");
	do 
	{
		if (0 != DVRSystem(szCmd))
		{
			printf("%s:%s:%d, system(%s) error\n", __FUNCTION__, __FILE__, __LINE__, szCmd);
			PUB_Sleep(50);
		}
		else
		{
			break;
		}
	} while (1);
	PUB_Sleep(50);

#endif
}
//////////////////////////////////////////////////////////////////////////
