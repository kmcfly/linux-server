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
	PROTOCOL_VISCA,		//<VISCA1>增加协议
	PROTOCOL_RANGE
};

//云台控制，在命令类型为PTZCMD_CTRL时的命令参数
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
	PTZCMD_MOVE = 0,		//云台移动
	PTZCMD_PRESETSET,		//设置预置点
	PTZCMD_PRESETGO,		//到第几个预置点
	PTZCMD_CRUISESET,		//设置巡航线
	PTZCMD_CRUISE_RUN,		//选择一个巡航线进行巡航
	PTZCMD_CRUISE_STOP,		//停止巡航
	PTZCMD_CRUISE_DEL		//删除巡航线
}PTZ_CMD_TYPE;

//串口类型值定义
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
	PARITYEVEN = 'E',		//偶校验
	PARITYODD = 'O',		//奇校验
	PARITYMARK = 'M',		//
	PARITYSPACE = 'S',		//
	PARITYNONE = 'N'		//不校验
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
//云台巡航点
typedef struct _ptz_curise_point
{
    unsigned char   preset;
    unsigned char   dwellSpeed;
    unsigned short  dwellTime;
}PTZ_CURISE_POINT;

const unsigned long  INVALID_PRESET			= 0xff;

}
#endif

