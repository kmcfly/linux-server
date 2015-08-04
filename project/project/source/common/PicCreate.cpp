#include "PicCreate.h"
#include "LocalDevice.h"

CPicCreate::CPicCreate()
{
	m_pDrawBuf	= NULL;
	m_pDrawBufLen= 0;
	m_width		= 0;
	m_height	= 0;
}

CPicCreate::~CPicCreate()
{
 	if (NULL != m_pDrawBuf)
 	{
 		delete m_pDrawBuf;
 		m_pDrawBuf = NULL;
 	}

	m_pDrawBufLen = 0;
}

int CPicCreate::CreatePic(const char * pStr, int w, int h, unsigned long color, unsigned long backColor/*=0x7fff*/, bool bResort/* = true*/)
{
	if (NULL != m_pDrawBuf)
	{
		delete m_pDrawBuf;
		m_pDrawBuf = NULL;
		m_pDrawBufLen = 0;
	}


	m_cx = w;
	m_cy = h;

	m_width = m_cx * FRAME_BUFFER_PIXSEL_NUM;
	m_height = m_cy * FRAME_BUFFER_PIXSEL_NUM;

	m_pDrawBufLen = w * h * FRAME_BUFFER_PIXSEL_NUM;
	m_pDrawBuf = new unsigned char [m_pDrawBufLen];

 	if (NULL == m_pDrawBuf)
 	{
 		printf("%s:%s:%d, get pic Data error\n", __FUNCTION__, __FILE__, __LINE__);
 		return -1;
 	}

	memset(m_pDrawBuf, 0, m_pDrawBufLen);

	m_transparency = TRANSPARENCY_0;
 	if (CreateRect(backColor))
 	{
 		printf("%s:%s:%d, Create back rect error\n", __FUNCTION__, __FILE__, __LINE__);
 		return -1;
 	}
	
 	if (NULL != pStr && CreateText(pStr, color, bResort))
 	{
 		printf("%s:%s:%d, Create text error\n", __FUNCTION__, __FILE__, __LINE__);
 		return -1;
 	}

	return 0;
}

int CPicCreate::GetPicData(unsigned char ** ppData, unsigned long &dataLen)
{
	if (NULL != ppData)
	{
		* ppData = m_pDrawBuf;
		dataLen = m_pDrawBufLen;
		return 0;
	}
	else
	{
		dataLen = 0;
		return -1;
	}
}

void CPicCreate::SetPicToAuxiliaryOutputDevice()
{
	CLocalDevice::Instance()->SetDeviceNomenuImage(m_pDrawBuf, m_cx, m_cy);
}

int CPicCreate::CreateRect(unsigned long color/*=0x7fff*/)
{
	//把第一行设成需要的颜色值，然后一行一行地把后面的设定
	unsigned char *pData = m_pDrawBuf;


	for (unsigned short w=0; w<m_cx; w++)
	{
		memcpy(pData, &color, FRAME_BUFFER_PIXSEL_NUM);
		pData += FRAME_BUFFER_PIXSEL_NUM;
	}

	pData = m_pDrawBuf + m_width;

	for (unsigned short h=1; h<m_cy; h++)
	{
		memcpy(pData, pData - m_width, m_width);
		pData += m_width;
	}

	return 0;
}

int CPicCreate::CreateText(const char * pStr, unsigned long color, bool bResort/*=false*/)
{
	unsigned long strLen = strlen(pStr);
	unsigned long len = strLen + 10;
	char textLine0[128] = {0};
	char textLine1[128] = {0};
	char textLine2[128] = {0};
	char textLine3[128] = {0};


	int count = 0;
	memcpy(textLine0, pStr, strLen);
	count++;
	char * pPos = strstr(textLine0, "\n");
	if (NULL != pPos)
	{
		* pPos = '\0';
		pPos ++;
	}
	else
	{
		pPos = textLine0 + strLen;
	}

	char * pNext1 = NULL;
	if (pPos < textLine0 + strLen)
	{
		pNext1 = strstr(pPos, "\n");
		if (NULL != pNext1)
		{
			memcpy(textLine1, pPos, pNext1-pPos);
			pPos = pNext1 + 1;
		}
		else
		{
			memcpy(textLine1, pPos, strLen - (pPos - textLine0));
			pPos = textLine0 + strLen;
		}
		count++;
	}

	if (pPos < textLine0 + strLen)
	{
		pNext1 = strstr(pPos, "\n");
		if (NULL != pNext1)
		{
			memcpy(textLine2, pPos, pNext1-pPos);
			pPos = pNext1 + 1;
		}
		else
		{
			memcpy(textLine2, pPos, strLen - (pPos - textLine0));
			pPos = textLine0 + strLen;
		}
		count++;
	}

	if (pPos < textLine0 + strLen)
	{
		pNext1 = strstr(pPos, "\n");
		if (NULL != pNext1)
		{
			memcpy(textLine3, pPos, pNext1-pPos);
			pPos = pNext1 + 1;
		}
		else
		{
			memcpy(textLine3, pPos, strLen - (pPos - textLine0));
			pPos = textLine0 + strLen;
		}
		count++;
	}

	unsigned short x = 0;
	unsigned short y = 0;
	unsigned long startPos = 0;
	unsigned short fwidth = m_cx;
	unsigned short owidth = m_cx - 4;
	unsigned short zoom = 1;
	unsigned short fhight = 20 * zoom;

	if (strlen(textLine0))
	{
		startPos = DrawText(textLine0, strlen(textLine0), 0 + fhight, y, fwidth, fhight, color, zoom, true, bResort);
	}

	y += (fhight + 4);
	x += fhight;
	if (strlen(textLine1))
	{
		DrawText(textLine1, strlen(textLine1), x, y, owidth - x, fhight, color, zoom, true, bResort);
	}

	y += (fhight + 4);
	if (strlen(textLine2))
	{
		DrawText(textLine2, strlen(textLine2), x, y, owidth - x, fhight, color, zoom, true, bResort);
	}

	y += (fhight + 4);
	if (strlen(textLine3))
	{
		DrawText(textLine3, strlen(textLine3), x, y, owidth - x, fhight, color, zoom, true, bResort);
	}

	return 0;
}

unsigned long CPicCreate::DrawText(const char *pStr, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color, unsigned char zoom/* = 1*/, bool bForceDrawBorder, bool bResort)
{
	assert (NULL != m_pDrawBuf);
	assert (x < m_cx);
	assert (y < m_cy);

	//把第一行设成需要的颜色值，然后一行一行地把后面的设定
	unsigned char *pData = m_pDrawBuf + m_width*y + x* GUI::FRAME_BUFFER_PIXSEL_NUM;
	unsigned short cx = 0, cy = 0;

	unsigned short *pWord = NULL;

	unsigned long i = 0;

	GUI::CFont * pFont = GUI::CFBInterface::Instance()->GetFont();

	if(len <= 0)
	{
		return 0;
	}


	if(!g_langRightOrder)
	{
		//// utf-8 [ucs-2. ISO 10646-1:1993]
		char* utf8Ptr = const_cast<char*>(pStr);

		unsigned long pre_code = 0;
		unsigned long pre_cx = 0;

		do
		{
			cx = width;
			cy = height;

			unsigned long charID = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			utf8Ptr += size;
			i += size;

			unsigned char* pShowData = pData;

			int ret_thai_check = THAI_RULE_N;
			if((charID >= 0x0E00) && (charID <= 0x0E7F))
			{
				ret_thai_check = ThaiCharRule(charID, pre_code);

				if(THAI_RULE_C == ret_thai_check)
				{
					unsigned short char_cx = 0, char_cy = 0;
					pFont->GetFontSize(char_cx, char_cy, charID, zoom);
					int char_lv = ThaiCharLevel(charID);
					int pre_char_lv = ThaiCharLevel(pre_code);

					if( ((THAI_LV_TOP == char_lv) || (THAI_LV_ABOVE == char_lv)) && 
						((THAI_LV_TOP == pre_char_lv) || (THAI_LV_ABOVE == pre_char_lv)))
					{
						pShowData = pData - char_cx * FRAME_BUFFER_PIXSEL_NUM - m_width * 4;
					}
					else
					{
						pShowData = pData - char_cx * FRAME_BUFFER_PIXSEL_NUM;
					}

					cx += char_cx;
				}
				else if((THAI_RULE_N == ret_thai_check) && (charID == 0x0E33))
				{
					pShowData = pData - 4 * FRAME_BUFFER_PIXSEL_NUM;
				}
			}

			if(cx <= 0)
			{
				/*
				[2011-06-22 L]泰语在宽width=0时, 并不为结束, 后续可能有和前面处于
				同一区域的字符
				*/
				break;
			}

			if(THAI_RULE_X != ret_thai_check)
			{
				int ret = pFont->GetBitmapBits(pShowData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				if(ret < 0)
				{
					//字体库中不存在或没创建的字符,使用?替代
					charID = GUI::FONT_ERR_CHAR_REPLACE;	//'?'
					pFont->GetBitmapBits(pShowData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				}
				else if(ret == GUI::CFont::FONT_ERR_OVERFLOW)
				{
					//如果越界,并没有输出字符,需回退
					i -= size;
					break;
				}
			}

			if(THAI_RULE_N == ret_thai_check)
			{
				if((charID == 0x0E33) && (cx >= 4))
				{
					cx -= 4;
				}

				pData += (cx * FRAME_BUFFER_PIXSEL_NUM);
				width -= cx;
				pre_cx = cx;
			}

			pre_code = charID;

		}while((0 <= width) && (width < m_cx) && (i < len));
	}
	else
	{
		pData = m_pDrawBuf + m_width*y + x*FRAME_BUFFER_PIXSEL_NUM + width * FRAME_BUFFER_PIXSEL_NUM;

		//// utf-8 [ucs-2. ISO 10646-1:1993]

		char* pNewStr = NULL;
		char* utf8Ptr = NULL;
		if(bResort)
		{
			pNewStr = new char[len + 1];
			memset(pNewStr, 0, len + 1);
			GUI::CFBInterface::Instance()->ReSortText(pNewStr, pStr, len);
			utf8Ptr = pNewStr;
		}
		else
		{
			utf8Ptr = const_cast<char*>(pStr);
		}

		unsigned long pre_code = 0;
		bool bIgnoreNext = false;

		do
		{
			cx = width;
			cy = height;

			unsigned long charID = 0;
			int size = 0;

			GUI_UTF8_TO_UCS2(utf8Ptr, charID, size);

			utf8Ptr += size;
			i += size;

			unsigned long oldID = charID;

			bool bIgnore = false;
			if((charID >= 0x0600) && (charID <= 0x06FF))
			{
				//阿拉伯语
				unsigned long next_code = 0;

				if(i < len)
				{
					int tmp_size = 0;

					GUI_UTF8_TO_UCS2(utf8Ptr, next_code, tmp_size);
				}

				charID = ArabicCharRule(pre_code, charID, next_code, bIgnore);
			}

			if(!bIgnoreNext)
			{
				unsigned short chcx = 0, chcy = 0;
				if(pFont->GetFontSize(chcx, chcy, charID, zoom))
				{
					pData -= chcx * FRAME_BUFFER_PIXSEL_NUM;
				}
				int ret = pFont->GetBitmapBits(pData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				if(ret < 0)
				{
					charID = GUI::FONT_ERR_CHAR_REPLACE;	//'?'
					pFont->GetFontSize(chcx, chcy, charID, zoom);
					pData -= chcx * FRAME_BUFFER_PIXSEL_NUM;
					pFont->GetBitmapBits(pData, m_cx, cx, cy, charID, color, FRAME_BUFFER_PIXSEL_NUM, zoom, bForceDrawBorder);
				}
				else if(ret == GUI::CFont::FONT_ERR_OVERFLOW)
				{
					i -= size;
					break;
				}

				width -= cx;
			}
			else
			{
				bIgnoreNext = false;
			}

			if(bIgnore)
			{
				bIgnoreNext = true;
			}

			pre_code = oldID;

		}while((0 < width) && (width < m_cx) && (i < len));

		if(NULL != pNewStr)
		{
			delete [] pNewStr;
		}
	}

	return i;
}