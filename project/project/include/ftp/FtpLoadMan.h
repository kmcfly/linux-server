//////////////////////////////////////////////////////////////////////////////
/*
*
*
*FTP上传下载管理部分,主要用于流数据的上传下载
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
		
	unsigned long FtpOpen(const char *pFileName);//pFileName:要上传的文件名,返回值为FtpPI的识别号
	int FtpWrite(unsigned long ftpFD, const void *pData, unsigned long length);
	int FtpClose(unsigned long ftpFD);//关闭识别号为FtpFD的文件传输
	int FtpLoadPic(const char *pFileName, CFrameData *pFrameData);
	bool SetUpFtpConfig(const TVT_FTP_CONFIG &config);	//从配置读取
	bool GetFtpConfigInfo(const TVT_FTP_CONFIG_INFO &ConfigInfo);

	bool StartLoadPic(unsigned long BufLen);
	bool StartLoadStream();
	void StopLoadPic();
	void StopLoadStream();

private:

	bool ParseFileName(const char *pFileName, char *pNewFileName, CFtpPI *pFtpPI);//分析文件名，创建文件夹和切换目录
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

