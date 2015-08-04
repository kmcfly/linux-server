#ifndef __RAW_TO_BMP_H__
#define __RAW_TO_BMP_H__
#include "QRCode.h"
#include "string.h"
#include "stdio.h"
#include "NetProtocol.h"

class CRawToBmp{
public:
	CRawToBmp(char * rawStr);
	bool CreateBmp(char *fPath); //在fPath生成bmp图片
	bool WriteSNToJSFile(char* fPath);	//在fPath生成js文件
private:
	static bool CutRaw(char* rawBuffer, char* pFileName, int bw, int bH);
private:
	char sn[120];
};
#endif
