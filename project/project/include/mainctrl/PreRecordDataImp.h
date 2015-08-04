#ifndef __PRERECORDDATAMAN_H__
#define __PRERECORDDATAMAN_H__
#include "FrameData.h"

typedef struct _prerecord_file_info
{
	unsigned long fileIndex;
	unsigned long fileSize;
}PRERECORD_FILE_INFO;

typedef struct _prerecord_buffer_file
{
	char szName[16];
	bool bUse;
}PRERECORD_BUFFER_FILE;

class CPreRecordDataImp
{
public:
	
	//
	CPreRecordDataImp(void);	
	~CPreRecordDataImp(void);

	//
	bool Initial(unsigned char channel, char *path);	
	void Quid();

	//插入数据
	int InsertData(unsigned char *buf, int nLen);

	//删除数据：
	//有两种方式，
	//一是:先得到数据，调用GetData，然后在删除数据，这时可以调用DeleteData，也可以调用DeleteDataEx，最好选择DeleteData
	//二是：直接删除数据，这时一定要调用DeleteDataEx。
	//DeleteData与DeleteDataEx的区别是：
	bool GetData(unsigned char **buf, int nLen);
	bool DeleteData(int nLen);
	bool DeleteDataEx(int nLen);

	//
	void DeleteAllData();

	//
	void CleanPreRecordPath();
	bool ResetPreRecordPath(char * path);

private:	
	
	//
	int FindBufferFileIndex();

	//
	int WriteBufToFile(unsigned char* buf ,int nLen, unsigned long &fileIndex);

	inline size_t PreRecFWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
private:

	//
	unsigned char m_channel;

	//
	bool m_bCanRecord;

	//
	char m_szDirPath[32];

	//
	unsigned char *m_pBuf;
	int m_nDelDataLen;
	int m_nAddDataLen;

	//
	CMyList <PRERECORD_FILE_INFO> m_fileList;	
	int m_nFileCount;
	FILE *m_pFile;
	unsigned char * m_pFileDataBuf;

	//
	int m_bufferFileNum;
	PRERECORD_BUFFER_FILE *m_pBufferFile;
};
#endif

