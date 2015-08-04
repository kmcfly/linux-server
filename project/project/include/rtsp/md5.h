#ifndef _MD5_H_
#define _MD5_H_

#ifdef __alpha
typedef unsigned int uint32;
#else
typedef unsigned long uint32;
#endif

struct MD5Context
{
	uint32 buf[4];
	uint32 bits[2];
	unsigned char in[64];
};

void MD5_Init(struct MD5Context *context);
void MD5_Update(struct MD5Context *context, char const *buf,
			   unsigned len);
void MD5_Final(unsigned char digest[16], struct MD5Context *context);
void MD5_Transform(uint32 buf[4], uint32 const in[16]);

/*
* This is needed to make RSAREF happy on some MS-DOS compilers.
*/
typedef struct MD5Context MD5_CTX;

#endif /* !MD5_H */

//end
