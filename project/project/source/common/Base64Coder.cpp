
#include"Base64Coder.h"
#include <stdlib.h>  
#include <stdio.h>
#include <string.h>


CBase64Coder::CBase64Coder()
{

}

CBase64Coder::~CBase64Coder()
{
			
}


void CBase64Coder::doNumConvert(long num,char *rStr,int *strLen,long *x)
{
	  if((*x=num/10))

		doNumConvert(*x,rStr,strLen,x);

	  *(rStr+(*(strLen))++)=num%10+'0';
}


int CBase64Coder::convertToString(double num,char *rStr)
{
	  int    strLen=0;
	  long   x;


	  doNumConvert((long) num,rStr,&strLen,&x);   

	  *(rStr+strLen)=0;


	  return strLen;
}

char *CBase64Coder::numStr(double num,char *dest)
{
	convertToString(num,dest);
	return dest;
}

int CBase64Coder::utf_8_encoded_size(unsigned char ch)
{

	if (ch<128)
		return 1;

	if (ch>=252)
		return 6;

	if (ch>=248)
		return 5;

	if (ch>=240)
		return 4;

	if (ch>=224)
		return 3;

	if (ch>=192)
		return 2;

	return 0;
}

int CBase64Coder::utf_8_get_code_point(unsigned char *ch,unsigned int *cp)
{

	int				i;
	int				numBytes;

	numBytes=utf_8_encoded_size(ch[0]);

	if (numBytes==0) 
	{

		*cp=0;
	}
	else 
	{

		if (numBytes==1) 
		{
			*cp=ch[0];
		} 
		else 
		{
			/*mask out bytes count bits, and make room for rest of character*/
			*cp=(ch[0] & (255>>numBytes))<<((numBytes-1)*6);

			for (i=1;i<numBytes;i++)

			/*mask out (& 0011 1111) meta bits (first two), and make room*/
			*cp+=((ch[i] & 63)<<((numBytes-1-i)*6));
		}
	}

	return numBytes;
}


unsigned int CBase64Coder::utf_8_code_point(unsigned char *ch)
{

	unsigned int cp;

	utf_8_get_code_point(ch,&cp);

	return cp;
}

char *CBase64Coder::utf_8_uri_encoded(char *dest,char *src,char *prefix,char *suffix)
{
	int				len;
	int				srcIndex=0;
	unsigned long	ch;
	char			cp_buff[12];

	len=strlen(src);
	
	while (srcIndex<len) 
	{
		ch=utf_8_code_point((unsigned char *) (src+srcIndex));
		if (!(ch==0)) 
		{
			if (ch==(unsigned char) src[srcIndex]) 
			{
				dest[strlen(dest)]=src[srcIndex];
			}
			else 
			{
				strcat(dest,prefix);
				strcat(dest,numStr(ch,cp_buff));
				strcat(dest,suffix);
			}
		}

		srcIndex++;
	}

	return dest;
}

char CBase64Coder::encode(unsigned char u) 
{

	if(u < 26)  return 'A'+u;
	if(u < 52)  return 'a'+(u-26);
	if(u < 62)  return '0'+(u-52);
	if(u == 62) return '+';
  
	return '/';

}


/*
 * Base64 encode and return size data in 'src'. The caller must free the
 * returned string.
 * @param size The size of the data in src
 * @param src The data to be base64 encode
 * @return encoded string otherwise NULL
 */
void CBase64Coder::b64encode(unsigned char *src) 
{

	int i, size;
	char *out, *p;
	size=strlen((char *)src);
	int destSize=size*4/3+4+4;
	out=(char *)malloc(destSize);
	memset(out,0,destSize);
	p=out;
	for(i=0; i<size; i+=3) 
	{
      
		unsigned char b1=0, b2=0, b3=0, b4=0, b5=0, b6=0, b7=0;
		b1 = src[i];
		if(i+1<size)
			b2 = src[i+1];
		if(i+2<size)
			b3 = src[i+2];
      
		b4= b1>>2;
		b5= ((b1&0x3)<<4)|(b2>>4);
		b6= ((b2&0xf)<<2)|(b3>>6);
		b7= b3&0x3f;
      
		*p++= encode(b4);
		*p++= encode(b5);
      
		if(i+1<size) 
		{
			*p++= encode(b6);
		} 
		else 
		{
			*p++= '=';
		}
      
		if(i+2<size)
		{
			*p++= encode(b7);
		} 
		else 
		{
			*p++= '=';
		}

	}
	memset(src,0,strlen((char*)src));
	strcpy((char*)src,out);
	free(out);
}


