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

	//��������
	int InsertData(unsigned char *buf, int nLen);

	//ɾ�����ݣ�
	//�����ַ�ʽ��
	//һ��:�ȵõ����ݣ�����GetData��Ȼ����ɾ�����ݣ���ʱ���Ե���DeleteData��Ҳ���Ե���DeleteDataEx�����ѡ��DeleteData
	//���ǣ�ֱ��ɾ�����ݣ���ʱһ��Ҫ����DeleteDataEx��
	//DeleteData��DeleteDataEx�������ǣ�
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

