#ifndef __DISC_DRIVE_MAN_H__
#define __DISC_DRIVE_MAN_H__

#define CDROM_INFO_PATH_NAME "/proc/sys/dev/cdrom/info"
#ifndef WIN32
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#endif

#include "CScsi.h"

/* �������� */
typedef enum
{
	// BD����֧��
	DISCTYPE_BD_RE        = 0X0043, // BD-RE
	DISCTYPE_BD_R_RRM     = 0X0042, // BD-R
	DISCTYPE_BD_R_SRM     = 0X0041, // BD-R
	// DVD����֧��
	DISCTYPE_DVD_RDL_SEQ  = 0X0015, // (DVD-R/DL SEQUENTIAL RECORDING)
	DISCTYPE_DVD_RDL_JMP  = 0X0016, // (DVD-R/DL LAYER JUMP RECORDING)
	DISCTYPE_DVD_RW_DL    = 0X002A, // DVD+RW Double Layer
	DISCTYPE_DVD_PLUS_RDL = 0X002B, // (DVD+R/DL)
	DISCTYPE_DVD_PLUS_R   = 0X001B, // (DVD+R)
	DISCTYPE_DVD_PLUS_RW  = 0X001A, // (DVD+RW)
	DISCTYPE_DVD_RW_SEQ   = 0X0014, // (DVD-RW SEQUENTIAL RECORDING)
	DISCTYPE_DVD_RW_OVW   = 0X0013, // (DVD-RW RESTRICTED OVERWRITE)
	DISCTYPE_DVD_RAM      = 0X0012, // (DVD-RAM)
	DISCTYPE_DVD_R_SEQ    = 0X0011, // (DVD-R SEQUENTIAL RECORDING)
	DISCTYPE_DVD_R_ROM    = 0X0010, // (DVD-ROM)

}DISCTYPE_E;

/* simplified */
struct iso_primary_descriptor {
	unsigned char type	[1];
	unsigned char id	[5];
	unsigned char void1	[80-5-1];
	unsigned char volume_space_size [8];
	unsigned char void2	[2048-80-8];
};

class CDiscDriveMan
{
public:
	CDiscDriveMan(void);
	~CDiscDriveMan(void);

	bool Initial();
	void Quit();

	const char* GetCDPath();
	bool GetDiscInfo();					//�ڻ�ȡ����Ĳ���ʱ���ȵ���һ������ӿ�
public :
	int GetDiscStatus()
	{
		return m_discStatus;
	}

	int GetDIscType()
	{
		return m_discType;
	}

	unsigned long long GetTotalCapacity()
	{
		return (unsigned long long)m_totalSector * (unsigned long long)m_sectorSize;
	}
	
	unsigned long long GetDataSize()
	{
		return (unsigned long long)m_nextSession * (unsigned long long)m_sectorSize;
	}

	unsigned long long GetFreeCapacity()				
	{
		return (unsigned long long)(m_totalSector - m_nextSession)* (unsigned long long)m_sectorSize;
	}

	bool IsEraseAble()
	{
		return m_isErase;
	}

	bool isBlank()
	{
		if(0 == m_discStatus)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
protected:
	unsigned int Get_2k_Capacity();
	int GetMmcProfile();
	bool GetSessionInfo(unsigned int * pNextSession,unsigned int * pPrvSession);
	int ReadSector(int lba,int sectorCnt,unsigned char* buf, unsigned int size);

private:
	//return: 1,Success
	int GetDiscDrivePath();

private:
	char m_cdPath[36];					//�����豸��

	Scsi_Command  m_cmd;				//SCSI �������
	int m_fd;							//�豸���
	int m_nextTrack;					//��һ�����̹��
	int m_nextSession;					//��һ����д�Ĺ�������
	int m_discType;						//��������
	int m_discStatus;					//���̵�״̬ "blank", "appendable","complete",	"other"
	bool m_isErase;						//�����Ƿ�ɲ���
	unsigned long long m_freeSize;		//ʣ���С
	unsigned long long m_totalSector;   //�ܹ�������������
	unsigned long long m_sectorSize;	//ÿ�������Ĵ� С
	int m_bdr_plus_pow;
	unsigned char m_disc_info[32];		//������Ϣ
	unsigned char  m_formats[260];		//����������ʽ��Ϣ
	iso_primary_descriptor *m_pISO_Descr;//ISO ������Ϣ����
};

void mmc_sperror (const char *cmd,unsigned char *sense);
void mmc_sperror (const char *cmd,int err);

#endif
