#ifndef BASE64CODER_DDNS_H
#define BASE64CODER_DDNS_H



class CBase64Coder
{
public:
	CBase64Coder();
	virtual ~CBase64Coder();
	void b64encode(unsigned char *src); 
	char *utf_8_uri_encoded(char *dest,char *src,char *prefix,char *suffix);

protected:

	void doNumConvert(long num,char *rStr,int *strLen,long *x);
	int convertToString(double num,char *rStr);
	char *numStr(double num,char *dest);
	int utf_8_encoded_size(unsigned char ch);
	int utf_8_get_code_point(unsigned char *ch,unsigned int *cp);
	unsigned int utf_8_code_point(unsigned char *ch);
	static char encode(unsigned char u);

};





#endif
