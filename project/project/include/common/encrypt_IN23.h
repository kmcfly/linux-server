#ifndef _ENCRYPT_IN23_H_
#define _ENCRYPT_IN23_H_
#define MAX_CRYPT_BUFF 1024
class CCrypt
{
public:
CCrypt() { Init();}
~CCrypt() { };
void Init();
void EncryptBuffer(unsigned char strBuffer[MAX_CRYPT_BUFF], unsigned int dataLength, unsigned char strEncryptBuffer[MAX_CRYPT_BUFF]);
void DecryptBuffer(unsigned char strDecryptBuffer[MAX_CRYPT_BUFF], unsigned int dataLength, unsigned char strBuffer[MAX_CRYPT_BUFF]) ;
private:
void CalculateInterval();
void Code();
unsigned char m_key[11];
unsigned short m_ax;
unsigned short m_bx;
unsigned short m_cx;
unsigned short m_dx;
unsigned short m_si;
unsigned short m_x1a2;
unsigned short m_x1a0[5];
unsigned short m_res;
unsigned short m_i;
unsigned short m_interval;
unsigned short m_cfc;
unsigned short m_cfd;
};
#endif