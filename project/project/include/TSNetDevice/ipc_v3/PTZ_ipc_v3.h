#ifndef PTZ_H_H_IPC_V3
#define PTZ_H_H_IPC_V3

namespace IPC_V3
{

enum PROTOCOL_TYPE
{
	PROTOCOL_NULL = 0,
	PROTOCOL_PELCOP,
	PROTOCOL_PELCOD,
	PROTOCOL_LILIN,
	PROTOCOL_MINKING,
	PROTOCOL_NEON,
	PROTOCOL_STAR,
	PROTOCOL_VIDO,
	PROTOCOL_DSCP,
	PROTOCOL_VISCA,		//<VISCA1>����Э��
	PROTOCOL_RANGE
};

//��̨���ƣ�����������ΪPTZCMD_CTRLʱ���������
enum
{
	PTZCOMMAND_STOP=0,
	PTZCOMMAND_LEFT,
	PTZCOMMAND_RIGHT,
	PTZCOMMAND_UP,
	PTZCOMMAND_DOWN,
	PTZCOMMAND_LEFT_UP,
	PTZCOMMAND_LEFT_DOWN,
	PTZCOMMAND_RIGHT_UP,
	PTZCOMMAND_RIGHT_DOWN,
	PTZCOMMAND_NEAR,
	PTZCOMMAND_FAR,
	PTZCOMMAND_ZOOMOUT,
	PTZCOMMAND_ZOOMIN,
	PTZCOMMAND_IRISOPEN,
	PTZCOMMAND_IRISCLOSE,
};

typedef enum _ptz_com_type
{
	PTZCMD_MOVE = 0,		//��̨�ƶ�
	PTZCMD_PRESETSET,		//����Ԥ�õ�
	PTZCMD_PRESETGO,		//���ڼ���Ԥ�õ�
	PTZCMD_CRUISESET,		//����Ѳ����
	PTZCMD_CRUISE_RUN,		//ѡ��һ��Ѳ���߽���Ѳ��
	PTZCMD_CRUISE_STOP,		//ֹͣѲ��
	PTZCMD_CRUISE_DEL		//ɾ��Ѳ����
}PTZ_CMD_TYPE;

//��������ֵ����
enum PORT_BAUDRATE
{
	SBR_110 = 0,
	SBR_300,
	SBR_600,
	SBR_1200,
	SBR_2400,
	SBR_4800,
	SBR_9600,
	SBR_19200,
	SBR_38400,
	SBR_57600,
	SBR_115200,
	SBR_230400,
	SBR_460800,
	SBR_921600
};

enum PARITYTYPE
{
	PARITYEVEN = 'E',		//żУ��
	PARITYODD = 'O',		//��У��
	PARITYMARK = 'M',		//
	PARITYSPACE = 'S',		//
	PARITYNONE = 'N'		//��У��
};

enum DATABITS
{
	DATABITS7 = 7,
	DATABITS8
};

enum STOPBITS
{
	STOPBITS1 = 2,
	STOPBITSONEHALF,
	STOPBITS2
};
//��̨Ѳ����
typedef struct _ptz_curise_point
{
    unsigned char   preset;
    unsigned char   dwellSpeed;
    unsigned short  dwellTime;
}PTZ_CURISE_POINT;

const unsigned long  INVALID_PRESET			= 0xff;

}
#endif

