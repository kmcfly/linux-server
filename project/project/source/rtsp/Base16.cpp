#include "Base16.h"

#ifdef WIN32
#ifdef _DEBUG
#define  new DEBUG_NEW
#endif
#endif


char CBase16::Decode16(unsigned char ch)
{
	return ch < 128 ? BASE16_VAL[ch] : -1;
}

bool CBase16::Encode(const void *pSrc, int SrcLen, char *pDes, bool IsCapitial /* = true */)
{
	if (NULL == pSrc || NULL == pDes)
	{
		return false;
	}
	
	const unsigned char *src = reinterpret_cast<const unsigned char *>(pSrc);
	const char *digits = IsCapitial ? BASE16_CH_UPPERCASE : BASE16_CH_LOWERCASE;

	int index;
	for (int i=0; i < SrcLen; i++)
	{
		//高四位
		index = src[i] >> 4;
		pDes[2*i] = digits[index];

		//低四位
		index = src[i] & 0x0f;
		pDes[2*i + 1] = digits[index];
	}

	return true;
}


bool CBase16::Decode(const char *pSrc, int SrcLen, void *pDes)
{
	if (SrcLen % 2 != 0)
	{
		return false;
	}

	unsigned char *des = reinterpret_cast<unsigned char *>(pDes);
	for (int i=0; i < SrcLen; i += 2)
	{
		//高四位
		char ch = Decode16(pSrc[i]);
		if (ch == -1)
		{
			return false;//非法字符
		}
		des[i / 2] = ch << 4;

		//低四位
		ch = Decode16(pSrc[i + 1]);
		if (ch == -1)
		{
			return false;
		}
		des[i / 2] |= ch; 
	}
	return true;
}

