#include "RawToBmp.h"

CRawToBmp::CRawToBmp(char * rawStr)
{
	strncpy(sn, rawStr, 119);
	sn[119] = '\0';
}
bool CRawToBmp::CutRaw(char* rawBuffer,char* pFileName,int bW,int bH)
{	
	FILE *fnew = fopen(pFileName, "wb");
	if(fnew == NULL)
	{
		printf("open bmp file failed:	file=%s,func=%s,line=%d\n",__FILE__,__FUNCTION__,__LINE__);
		return false;
	}


	BITMAPFILEHEADER fh;
	fh.bfType=0x4D42;//BM
	fh.bfSize=14+40+256*4+((bW+3)&~3)*bH;
	fh.bfReserved1=0;
	fh.bfReserved2=0;
	fh.bfOffBits=14+40+256*4;
	BITMAPINFOHEADER infoh;
	infoh.biBitCount=8;
	infoh.biClrImportant=0;
	infoh.biClrUsed=0;
	infoh.biCompression=0;
	infoh.biHeight=(long)bH;
	infoh.biPlanes=1;
	infoh.biSize=40;
	infoh.biSizeImage=0;
	infoh.biWidth=(long)bW;
	infoh.biXPelsPerMeter=800;
	infoh.biYPelsPerMeter=600;
	RGBQUAD clrq[256];
	for(int i=0;i<256;i++)
	{
		clrq[i].rgbRed=i;
		clrq[i].rgbGreen=i;
		clrq[i].rgbBlue=i;
		clrq[i].rgbReserved=0;
	}

	fwrite(&fh, 14, 1, fnew);
	fwrite(&infoh, 40, 1, fnew);
	fwrite(clrq, 256*4, 1, fnew);
	fwrite(rawBuffer, bH*((bW+3)&~3), 1, fnew);
	fclose(fnew);

	return 1;
}

bool CRawToBmp::CreateBmp(char *fPath)
{
	CQRCode codeMan;
	QrCodeOutputData qrMaix;
	qrMaix.dataMatrix =NULL;
	qrMaix.width = 0;

	codeMan.GetQRCodeMatrixData((unsigned char *)sn,strlen(sn),&qrMaix);

	char *qrRaw = new char[qrMaix.width*((qrMaix.width+3)&~3)];
	char *p = qrRaw;
	for(unsigned long h =0;h<qrMaix.width;++h)
	{
		for(unsigned long w =0;w<qrMaix.width;++w)
		{
			if(0x01 & (*(qrMaix.dataMatrix+(qrMaix.width-h-1)*qrMaix.width+w)))
			{
				//ºÚ¿é
				*p++ = 0x00;
			}
			else
			{	//°×¿é
				*p++ = 0xff;
			}
		}
		for(unsigned long w = qrMaix.width;w<((qrMaix.width+3)&~3);w++)
		{
			*p++ = 0xff;
		}
	}
	CutRaw(qrRaw, fPath, qrMaix.width, qrMaix.width);
	codeMan.FreeQrCodeMatrix(&qrMaix);
	delete[] qrRaw;
}

bool CRawToBmp::WriteSNToJSFile(char *fPath)
{
	FILE* jsFile = fopen(fPath, "w");
	if(jsFile == NULL)
	{
		printf("open qr.js failed:	file=%s,func=%s,line=%d\n",__FILE__,__FUNCTION__,__LINE__);
		return false;
	}
	else
	{
		fputs("var qr=", jsFile);
		fputs("\"", jsFile);
		fputs(sn, jsFile);
		fputs("\";\n", jsFile);
		fclose(jsFile);
		return true;
	}
}

