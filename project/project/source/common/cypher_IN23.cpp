#include "encrypt_IN23.h"
#include <string.h>

void CCrypt::Init()
{
int iCnt;
for (iCnt=0; iCnt<=9;iCnt++)
{
m_key[iCnt]=0;
}
strcpy((char*) m_key, "cytkeyadi2jjr");
m_ax = 0;
m_bx = 0;
m_cx = 0;
m_dx = 0;
m_si = 0;
m_x1a2 = 0;
for (iCnt=0; iCnt < 5; iCnt++)
{
m_x1a0[iCnt];
}
m_res = 0;
m_i = 0;
m_interval = 0;
m_cfc = 0;
m_cfd = 0;
}

void CCrypt::EncryptBuffer(unsigned char strBuffer[MAX_CRYPT_BUFF], unsigned int dataLength, unsigned char strEncryptBuffer[MAX_CRYPT_BUFF])
{
m_si=0;
m_x1a2=0;
m_i=0;
Init();
CalculateInterval();
m_cfc = m_interval>>8;
m_cfd = m_interval & 255; /* cfc^cfd = random byte */
for (int iCnt = 0; iCnt < dataLength; iCnt++)
{
for (int iCnt1 = 0; iCnt1 <= 9;iCnt1++)
{
/* we mix the plaintext byte with the key */
m_key[iCnt1] = m_key[iCnt1]^strBuffer[iCnt];
}
strEncryptBuffer[iCnt] = strBuffer[iCnt] ^ (m_cfc ^ m_cfd);
}
}


void CCrypt::DecryptBuffer(unsigned char strDecryptBuffer[MAX_CRYPT_BUFF], unsigned int dataLength, unsigned char strBuffer[MAX_CRYPT_BUFF])
{
unsigned char ch;
m_si=0;
m_x1a2=0;
m_i=0;
Init();
CalculateInterval();
m_cfc = m_interval>>8;
m_cfd = m_interval & 255; /* cfc^cfd = random byte */
for (int iCnt = 0; iCnt < dataLength; iCnt++)
{
ch = strDecryptBuffer[iCnt] ^ (m_cfc ^ m_cfd);
for (int iCnt1 = 0; iCnt1 <= 9; iCnt1++)
{
/* we mix the plaintext byte with the key */
m_key[iCnt1] = m_key[iCnt1]^strDecryptBuffer[iCnt];
}
strBuffer[iCnt] = ch;
}
}

void CCrypt::CalculateInterval()
{
m_x1a0[0] = ( m_key[0]*256 )+ m_key[1];
Code();
m_interval = m_res;
m_x1a0[1] = m_x1a0[0] ^ ( (m_key[2]*256) + m_key[3] );
Code();
m_interval = m_interval ^ m_res;
m_x1a0[2] = m_x1a0[1] ^ ( (m_key[4]*256) + m_key[5] );
Code();
m_interval = m_interval ^ m_res;
m_x1a0[3] = m_x1a0[2] ^ ( (m_key[6]*256) + m_key[7] );
Code();
m_interval = m_interval ^ m_res;
m_x1a0[4] = m_x1a0[3] ^ ( (m_key[8]*256) + m_key[9] );;
Code();
m_interval = m_interval ^ m_res;
m_i=0;
}

void CCrypt::Code()
{
unsigned short m_tmp;
m_dx = m_x1a2 + m_i;
m_ax = m_x1a0[m_i];
m_cx = 0x015a;
m_bx = 0x4e35;
m_tmp = m_ax;
m_ax = m_si;
m_si = m_tmp;
m_tmp = m_ax;
m_ax = m_dx;
m_dx = m_tmp;
if (m_ax != 0)
{
m_ax = m_ax * m_bx;
}
m_tmp = m_ax;
m_ax = m_cx;
m_cx = m_tmp;
if (m_ax != 0)
{
m_ax = m_ax * m_si;
m_cx = m_ax + m_cx;
}
m_tmp = m_ax;
m_ax = m_si;
m_si = m_tmp;
m_ax = m_ax * m_bx;
m_dx = m_cx + m_dx;
m_ax = m_ax+1;
m_x1a2 = m_dx;
m_x1a0[m_i] = m_ax;
m_res = m_ax ^ m_dx;
m_i = m_i + 1;
}









































