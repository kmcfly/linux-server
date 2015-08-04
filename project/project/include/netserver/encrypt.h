#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

#define MAX_SIZE 1024

#include <string.h>

class MACCrypt
{
public:
	MACCrypt() { };
	~MACCrypt() { };
	void Strt();
	void CryptBuffer(unsigned char strCryptBuffer[MAX_SIZE], unsigned int dataLength, unsigned char strBuffer[MAX_SIZE]) ;
private:
	void CalcInt();
	void ECod();
	unsigned char mint_pass[11];
	unsigned short mint_a;
	unsigned short mint_b;
	unsigned short mint_c;
	unsigned short mint_d;
	unsigned short mint_e;
	unsigned short mint_f;
	unsigned short mint_h[5];
	unsigned short mint_i;
	unsigned short mint_j;
	unsigned short mint_k;
	unsigned short mint_l;
	unsigned short mint_m;
};
#endif