#ifndef _PTZ_H
#define _PTZ_H
#include "dvrdvsdef.h"
#define MAX_PRESET_NUM   255                            /*最大预置点数*/
/*direction  */
#define    PTZ_CTRL_DIRECTION_UP                  0x08                        /*云台以字节6给出的速度值向上方向运动*/
#define    PTZ_CTRL_DIRECTION_DOWN             0x04                       /*云台以字节6给出的速度值向下方向运动*/
#define    PTZ_CTRL_DIRECTION_LEFT                0x02                      /*云台以字节5给出的速度值向左方向运动*/
#define    PTZ_CTRL_DIRECTION_RIGHT              0x01                     /*云台以字节5给出的速度值向右方向运动*/
#define    PTZ_CTRL_STOP_ALL                           0x00                      /*PTZ的所有操作均停止*/
#define    PTZ_CTRL_DIRECTION_LEFT_UP          0x0A                        /*云台以字节5给出的速度值向左方向运动，同时以字节6的速度向上运动*/
#define    PTZ_CTRL_DIRECTION_LEFT_DOWN           0x06                     /*云台以字节5给出的速度值向左方向运动，同时以字节6的速度向下运动*/
#define    PTZ_CTRL_DIRECTION_RIGHT_UP           0x09                       /*云台以字节5给出的速度值向右方向运动，同时以字节6的速度向上运动*/
#define    PTZ_CTRL_DIRECTION_RIGHT_DOWN           0x05						/*云台以字节5给出的速度值向右方向运动，同时以字节6的速度向下运动*/
/*shot control*/

#define    PTZ_CTRL_ZOOM_OUT                           0x10                     /*镜头以字节7高4位的数值变倍放大*/
#define    PTZ_CTRL_ZOOM_IN                        0x20                        /*镜头以字节7高4位的数值变倍缩小*/

#define    PTZ_CTRL_APERTURE_SMALL               0x48                 /*镜头以字节6的数值缩小光圈*/
#define    PTZ_CTRL_APERTURE_LARGE               0x44                /*镜头以字节6的数值放大光圈*/

#define    PTZ_CTRL_FOCUS_NEAR                       0x42                         /*镜头以字节5的数值聚焦近*/
#define    PTZ_CTRL_FOCUS_FAR                          0x41                       /*镜头以字节5的数值聚焦远*/

#define    PTZ_CTRL_STOP_SHOT_ALL                   0x40            /*停止镜头的所有操作*/

/*preset*/
#define   PTZ_CTRL_PRESET_SET                           0x81                        /*设置预置位*/
#define   PTZ_CTRL_PRESET_CALL                         0x82                   /*调用预置位*/
#define   PTZ_CTRL_PRESET_CLEAR                       0x83         /*清除一个预置位*/

/*cruise*/
#define    PTZ_CTRL_CRUISE_ADD                        0x84          /*增加一个巡航点*/
#define   PTZ_CTRL_CRUISE_CLEAR                     0x85         /*清除一个巡航点*/
#define   PTZ_CTRL_CRUISE_SPEED                     0x86          /*设置巡航的速度*/
#define   PTZ_CTRL_CRUISE_TIME                       0x87             /*设置巡航停留时间*/
#define   PTZ_CTRL_CRUISE_START                    0x88              /*开始巡航*/

#define  PTZ_CTRL_AUTO_SCAN_START                        0x89    /*开始自动扫描*/
#define  PTZ_CTRL_AUTO_SCAN_SPEED                         0x8A        /*设置自动扫描速度*/

class CPTZControl
{
public:
	static CPTZControl* Instance();

	CPTZControl();
	
	//PTZ解析入口
	int ControlParsePtz(char *PtzStr, unsigned  long nChannelNum);

	/*设置PTZ的通道*/
	void SetPtzChannel(unsigned  long  ulChnn);

private:

	//使用单件，所以让构造函数私有
	~CPTZControl();

	int ParsePtzCommand(unsigned char *buf);

	//发送PTZ控制命令
	void SendPtzCtrlCmd(PTZ_CTRL *pCtrl);

	/*组合码命令解析*/
	void ParseCombinedCode(unsigned char *buf);

	/*解析组合码，方向运动命令 例如左上，左下*/
	void ParseCombinedDirection(unsigned char *buf);

	/*解析组合码，FI命令*/
	void ParseCombinedFI(unsigned char *buf);

	/*将字符串转换为16的数，并检查校验和*/
	int ParsePtzStr(  char *PtzStr,  unsigned char  *buf);

	// 依次将字符转换成整数
	int StrToInt( char *string ,unsigned char *pBuf);
	
	//判断8位数的某一位是否为0，iIndex取值为0-7
	bool HasBit8(unsigned char c, int iIndex);

	//将1-255转换成1-8的速度
	unsigned long SpeedAreaConvert(unsigned char speed);  

	void PtzCtrLock()   {m_PtzLock.Lock();}
	void PtzCtrUnLock() {m_PtzLock.UnLock();}

private:	
	unsigned  long				chnn;                                 //PTZ通道
	CGb28181Server				*m_pGb28181Server;			//访问Gb28181Server接口
	CPUB_Lock					m_PtzLock;			//m_PtzLock lock
};
#endif