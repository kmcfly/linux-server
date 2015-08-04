#ifndef __BASE_16__
#define __BASE_16__

#include <string>

const char BASE16_CH_UPPERCASE[17] = "0123456789ABCDEF";
const char BASE16_CH_LOWERCASE[17] = "0123456789abcdef";
const char BASE16_VAL[] = 
{
	-1, -1, -1, -1,   -1, -1, -1, -1,   -1, -1, -1, -1,  -1, -1, -1, -1,
	-1, -1, -1, -1,   -1, -1, -1, -1,   -1, -1, -1, -1,  -1, -1, -1, -1,
	-1, -1, -1, -1,   -1, -1, -1, -1,   -1, -1, -1, -1,  -1, -1, -1, -1,
	 0,  1,  2,  3,    4,  5,  6,  7,    8,  9, -1, -1,  -1, -1, -1, -1,
	-1, 10, 11, 12,   13, 14, 15, -1,   -1, -1, -1, -1,  -1, -1, -1, -1,
	-1, -1, -1, -1,   -1, -1, -1, -1,   -1, -1, -1, -1,  -1, -1, -1, -1,
	-1, 10, 11, 12,   13, 14, 15, -1,   -1, -1, -1, -1,  -1, -1, -1, -1,
	-1, -1, -1, -1,   -1, -1, -1, -1,   -1, -1, -1, -1,  -1, -1, -1, -1,
};

class CBase16
{
public:
	bool Encode(const void *pSrc, int SrcLen, char *pDes, bool IsCapitial = true);
	bool Decode(const char *pSrc, int SrcLen, void *pDes);

private:
	char Decode16(unsigned char ch);
};

#endif