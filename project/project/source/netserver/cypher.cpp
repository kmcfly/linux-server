#include "encrypt.h"

void MACCrypt::Strt()
{
	int iCnt;
	for (iCnt=0; iCnt<=9;iCnt++)
	{
		mint_pass[iCnt]=0;	
	}

	strcpy((char*) mint_pass, "cytkeykongttp");
	mint_a = 0;
	mint_b = 0;
	mint_c = 0;
	mint_d = 0;
	mint_e = 0;
	mint_f = 0;
	
	for (iCnt=0; iCnt < 5; iCnt++)
	{
		mint_h[iCnt];
	}
	
	mint_i = 0;
	mint_j = 0;
	mint_k = 0;
	mint_l = 0;
	mint_m = 0;
}

void MACCrypt::CryptBuffer(unsigned char strCryptBuffer[MAX_SIZE], unsigned int dataLength, unsigned char strBuffer[MAX_SIZE])
{
	unsigned char ch;
	mint_e=0;
	mint_f=0;
	mint_j=0;
	Strt();
	CalcInt();
	mint_l = mint_k>>8;
	mint_m = mint_k & 255; /* cfc^cfd = random byte */
	for (int iCnt = 0; iCnt < dataLength; iCnt++)
	{
		ch = strCryptBuffer[iCnt] ^ (mint_l ^ mint_m);
		for (int iCnt1 = 0; iCnt1 <= 9; iCnt1++)
		{
			/* we mix the plaintext byte with the key */
			mint_pass[iCnt1] = mint_pass[iCnt1]^strCryptBuffer[iCnt];
		}
		strBuffer[iCnt] = ch;
	}
}

void MACCrypt::CalcInt()
{
	mint_h[0] = ( mint_pass[0]*256 )+ mint_pass[1];
	ECod();
	mint_k = mint_i;
	mint_h[1] = mint_h[0] ^ ( (mint_pass[2]*256) + mint_pass[3] );
	ECod();
	mint_k = mint_k ^ mint_i;
	mint_h[2] = mint_h[1] ^ ( (mint_pass[4]*256) + mint_pass[5] );
	ECod();
	mint_k = mint_k ^ mint_i;
	mint_h[3] = mint_h[2] ^ ( (mint_pass[6]*256) + mint_pass[7] );
	ECod();
	mint_k = mint_k ^ mint_i;
	mint_h[4] = mint_h[3] ^ ( (mint_pass[8]*256) + mint_pass[9] );;
	ECod();
	mint_k = mint_k ^ mint_i;
	mint_j=0;
}

void MACCrypt::ECod()
{
	unsigned short m_tmp;
	mint_d = mint_f + mint_j;
	mint_a = mint_h[mint_j];
	mint_c = 0x015a;
	mint_b = 0x4e35;
	m_tmp = mint_a;
	mint_a = mint_e;
	mint_e = m_tmp;
	m_tmp = mint_a;
	mint_a = mint_d;
	mint_d = m_tmp;
	if (mint_a != 0)
	{
		mint_a = mint_a * mint_b;
	}
	m_tmp = mint_a;
	mint_a = mint_c;
	mint_c = m_tmp;
	if (mint_a != 0)
	{
		mint_a = mint_a * mint_e;
		mint_c = mint_a + mint_c;
	}
	m_tmp = mint_a;
	mint_a = mint_e;
	mint_e = m_tmp;
	mint_a = mint_a * mint_b;
	mint_d = mint_c + mint_d;
	mint_a = mint_a+1;
	mint_f = mint_d;
	mint_h[mint_j] = mint_a;
	mint_i = mint_a ^ mint_d;
	mint_j = mint_j + 1;
}









































