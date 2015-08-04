//////////////////////////////////////////////////////////////////////////////
/*
*
*
*FTP�ϴ����ع�����,��Ҫ���������ݵ��ϴ�����
*
*
*/

#ifndef __FTP_LOAD_MAN__
#define __FTP_LOAD_MAN__

#include "DataBuffer.h"
#include "FtpType.h"
#include "FtpLoadPic.h"
#include "FtpLoadStream.h"
#include "FrameData.h"
#include <map>

const unsigned short FTP_STREAM_NUM = 4;

typedef struct _tvt_ftp_config_info_
{
	TVT_FTP_LOAD_INFO LoadInfo[FTP_STREAM_NUM + 1];
}TVT_FTP_CONFIG_INFO;

class CFtpLoadMan
{
public:
	static CFtpLoadMan *Instance();
	~CFtpLoadMan();
		
	unsigned long FtpOpen(const char *pFileName);//pFileName:Ҫ�ϴ����ļ���,����ֵΪFtpPI��ʶ���
	int FtpWrite(unsigned long ftpFD, const void *pData, unsigned long length);
	int FtpClose(unsigned long ftpFD);//�ر�ʶ���ΪFtpFD���ļ�����
	int FtpLoadPic(const char *pFileName, CFrameData *pFrameData);
	bool SetUpFtpConfig(const TVT_FTP_CONFIG &config);	//�����ö�ȡ
	bool GetFtpConfigInfo(const TVT_FTP_CONFIG_INFO &ConfigInfo);

	bool StartLoadPic(unsigned long BufLen);
	bool StartLoadStream();
	void StopLoadPic();
	void StopLoadStream();

private:

	bool ParseFileName(const char *pFileName, char *pNewFileName, CFtpPI *pFtpPI);//�����ļ����������ļ��к��л�Ŀ¼
	char* GetDirName(char *pFileName, char *pDirName);
	bool GetFtpUpLoadInfo();

private:
	CFtpLoadMan();

private:
	bool						m_bIsFTPEnable;
	unsigned char				m_nOpenCount;
	
	CFtpLoadStream				m_ftpLoadStream[FTP_STREAM_NUM];
	CFtpLoadPic					m_ftpLoadPic;

	unsigned int				m_PicPos;
	unsigned int				m_CurPicLen;
};
#endif

