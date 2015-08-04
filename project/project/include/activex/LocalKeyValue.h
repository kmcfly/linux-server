#ifndef _LOCAK_KEY_VALUE_H_
#define _LOCAK_KEY_VALUE_H_

/*
typedef enum _dvr_key_type
{
	DVR_KEY_1UA		= 0x00,
	DVR_KEY_1UB,
	DVR_KEY_1UC,
	DVR_KEY_2UA		= 0x10,
	DVR_KEY_2UB,
}DVR_KEY_TYPE;
*/

//本机键盘值定义

/*****************************   2 U A  **********************************************/
/***********前面板键值******************************
	01 02 03 05        19
	04 06 07 0a		18 1a 1c
	08 09 0b 0f		   1b
	0c 0d 0e 14     11 12 13      15 1d 16 17 1f 1e
					20	  
****************************************************/

/*************飞梭值*******************************
飞梭外轮:
	  左旋得:	51 52 53 54 55 56 57 58
	  右旋得:	59 5a 5b 5c 5d 5e 5f
	  中间复位: 50

飞梭内轮:
	  逆时钟旋转: 60
      顺时钟旋转: 61
***************************************************/

/***********前面板定义(UI)*****************************************************************
	1	2	3		SINGLE			UP

	4	5	6		QUAD		LEFT	ENTER	RIGHT

	7	8	9		NINE/+				DOWN

	10 10+	INFO	AUDIO/-		P.T.Z	BACKUP	MENU	REC	SEARCH	PLAY	REW	  FF	STOP

	   POWER
********************************************************************************************/

/***********前面板定义(P.T.Z)******************************************************************
	1	2	3		SINGLE					UP

	4	5	6		QUAD			LEFT	ENTER	RIGHT

	7	8	9		SPEED+/+				DOWN

	10 10+	INFO	SPEED-/-		P.T.Z	BACKUP	MENU	ZOOM/+	ZOOM/- FOCUS/+	FOCUS/-	IRIS/+	IRIS/-
************************************************************************************************/

const unsigned long U2_KEY_1     = 0x01;
const unsigned long U2_KEY_2     = 0x02;
const unsigned long U2_KEY_3     = 0x03;
const unsigned long U2_KEY_4     = 0x04;
const unsigned long U2_KEY_5     = 0x06;
const unsigned long U2_KEY_6     = 0x07;
const unsigned long U2_KEY_7     = 0x08;
const unsigned long U2_KEY_8     = 0x09;
const unsigned long U2_KEY_9     = 0x0b;
const unsigned long U2_KEY_0     = 0x0c;
const unsigned long U2_KEY_10    = 0x0d;

const unsigned long U2_KEY_SINGLE	= 0x05;
const unsigned long U2_KEY_QUAD		= 0x0a;
const unsigned long U2_KEY_NINE		= 0x0f;
const unsigned long U2_KEY_SIXTEEN	= 0x14;
const unsigned long U2_KEY_INFO		= 0x0e;
const unsigned long U2_KEY_AUDIO	= 0x10;

const unsigned long U2_KEY_LEFT		= 0x18;
const unsigned long U2_KEY_RIGHT	= 0x1c;
const unsigned long U2_KEY_DOWN		= 0x1b;
const unsigned long U2_KEY_UP		= 0x19;
const unsigned long U2_KEY_ENTER	= 0x1a;

const unsigned long U2_KEY_PTZ		= 0x11;
const unsigned long U2_KEY_BACKUP	= 0x12;
const unsigned long U2_KEY_MENU		= 0x13;

const unsigned long U2_KEY_RECORD	= 0x15;
const unsigned long U2_KEY_SEARCH	= 0x1d;
const unsigned long U2_KEY_PLAY		= 0x16;
const unsigned long U2_KEY_REW		= 0x17;
const unsigned long U2_KEY_FF		= 0x1f;
const unsigned long U2_KEY_STOP		= 0x1e;

const unsigned long U2_KEY_POWER	= 0x20;

//飞梭
const unsigned long U2_FLY_LEFT_1	= 0x51;
const unsigned long U2_FLY_LEFT_2	= 0x52;
const unsigned long U2_FLY_LEFT_3	= 0x53;
const unsigned long U2_FLY_LEFT_4	= 0x54;
const unsigned long U2_FLY_LEFT_5	= 0x55;
const unsigned long U2_FLY_LEFT_6	= 0x56;
const unsigned long U2_FLY_LEFT_7	= 0x57;
const unsigned long U2_FLY_LEFT_8	= 0x58;

const unsigned long U2_FLY_MEDIUM	= 0x50;

const unsigned long U2_FLY_RIGHT_1	= 0x59;
const unsigned long U2_FLY_RIGHT_2	= 0x5a;
const unsigned long U2_FLY_RIGHT_3	= 0x5b;
const unsigned long U2_FLY_RIGHT_4	= 0x5c;
const unsigned long U2_FLY_RIGHT_5	= 0x5d;
const unsigned long U2_FLY_RIGHT_6	= 0x5e;
const unsigned long U2_FLY_RIGHT_7	= 0x5f;

const unsigned long U2_FLY_IN_LEFT	= 0x60;
const unsigned long U2_FLY_IN_RIGHT	= 0x61;

/*****************************   2 U B  **********************************************/
/***********前面板键值******************************
				20	01 02 03 04 05		15		19		1d
					06 07 08 09 0a		16	18  1a 1c	1e
					0b 0c 0d 0e 0f		17		1b		1f
					10 11 12 13 14					
****************************************************/

/*************飞梭值*******************************
飞梭外轮:
左旋得:	51 52 53 54 55 56 57 58
右旋得:	59 5a 5b 5c 5d 5e 5f
中间复位: 50

飞梭内轮:
逆时钟旋转: 60
顺时钟旋转: 61
***************************************************/

/***********(1UB)键盘布局按键值************************************************************************
01	04	07		0a																15

02	05	08		0b														16		13		17

03	06	09		0c			0d		0e	0f	10	11	12							14
************************************************************************************************************/


/***********(1UB)键盘布局-前面板定义(FUNC)************************************************************************
1	2	3		MENU / +																	UP

4	5	6		BACKUP / 10 /-														LEFT	ENTER	RIGHT

7	8	9		O / 10+			 REC		SEARCH	PLAY	REW		P.T.Z	ESC				DOWN
************************************************************************************************************/

/***********(1UB)键盘布局-前面板定义(PLAYBACK)************************************************************************
1	2	3		MENU / +																	UP

4	5	6		BACKUP / 10 /-														LEFT	ENTER	RIGHT

7	8	9		0 / 10+			REC		SEARCH	PLAY	REW		FF		ESC				DOWN
************************************************************************************************************/

/***********(1UB)键盘布局-前面板定义(P.T.Z)******************************************************************
1	2	3		+																	UP

4	5	6		-															LEFT	ENTER	RIGHT

7	8	9		10+			FOUCS	ZOOM	IRIS	SPEED	P.T.Z	STOP			DOWN
*************************************************************************************************************/

const unsigned long U1_KEY_1		= 0x01;
const unsigned long U1_KEY_2		= 0x04;
const unsigned long U1_KEY_3		= 0x07;
const unsigned long U1_KEY_4		= 0x02;
const unsigned long U1_KEY_5		= 0x05;
const unsigned long U1_KEY_6		= 0x08;
const unsigned long U1_KEY_7		= 0x03;
const unsigned long U1_KEY_8		= 0x06;
const unsigned long U1_KEY_9		= 0x09;

//ZHL 2010-03-05 1U的键盘面板丝印修改
const unsigned long U1_KEY_10		= 0x0c;				//10 和 0 在一起，处理10+ ， 没有以前的INFO功能
const unsigned long U1_KEY_ADD		= 0x0a;				//+ 和 MENU在一起，MENU在PTZ和PLAYBACK模式下处理隐藏
const unsigned long U1_KEY_DEC		= 0x0b;				//- 和 BACKUP在一起，


const unsigned long U1_KEY_LEFT  	= 0x16;
const unsigned long U1_KEY_RIGHT 	= 0x17;
const unsigned long U1_KEY_DOWN  	= 0x14;
const unsigned long U1_KEY_UP    	= 0x15;
const unsigned long U1_KEY_ENTER 	= 0x13;

const unsigned long U1_KEY_RECORD	= 0x0d;
const unsigned long U1_KEY_SEARCH	= 0x0e;
const unsigned long U1_KEY_PLAY		= 0x0f;
const unsigned long U1_KEY_REW		= 0x10;
const unsigned long U1_KEY_FF		= 0x11;
const unsigned long U1_KEY_STOP		= 0x12;


const unsigned long U1_KEY_POWER	= 0x18;


//********************************************************//
//键盘类型 :1US 面板只有下面几个按键
//********************************************************//
//const unsigned long US1_KEY_MENU	= 0x0a;

const unsigned long US1_KEY_RECORD  = 0x01;
const unsigned long US1_KEY_PLAY    = 0x02;
const unsigned long US1_KEY_REW		= 0x03;
const unsigned long US1_KEY_FF		= 0x04;
const unsigned long US1_KEY_STOP	= 0x05;
const unsigned long US1_KEY_INFO	= 0x06;

const unsigned long US1_KEY_ENTER 	= 0x08;
const unsigned long US1_KEY_SEARCH  = 0x0b;

const unsigned long US1_KEY_LEFT  	= 0x07;
const unsigned long US1_KEY_RIGHT 	= 0x0c;
const unsigned long US1_KEY_DOWN  	= 0x09;
const unsigned long US1_KEY_UP    	= 0x0a;
const unsigned long US1_KEY_POWER   = 0x0d;


//********************************************************//
//台湾OEM485键盘 参看了Hisharp软件通过电脑串口发出的键值
//********************************************************//

const unsigned long TWOEM485_KEY_1		= 0x3031;
const unsigned long TWOEM485_KEY_2		= 0x3032;
const unsigned long TWOEM485_KEY_3		= 0x3033;
const unsigned long TWOEM485_KEY_4		= 0x3034;
const unsigned long TWOEM485_KEY_5		= 0x3035;
const unsigned long TWOEM485_KEY_6		= 0x3036;
const unsigned long TWOEM485_KEY_7		= 0x3037;
const unsigned long TWOEM485_KEY_8		= 0x3038;
const unsigned long TWOEM485_KEY_9		= 0x3039;

const unsigned long TWOEM485_KEY_0		= 0x0c;
const unsigned long TWOEM485_KEY_10		= 0x3130;
const unsigned long TWOEM485_KEY_11		= 0x3131;
const unsigned long TWOEM485_KEY_12		= 0x3132;
const unsigned long TWOEM485_KEY_13		= 0x3133;
const unsigned long TWOEM485_KEY_14		= 0x3134;
const unsigned long TWOEM485_KEY_15		= 0x3135;
const unsigned long TWOEM485_KEY_16		= 0x3136;

const unsigned long TWOEM485_KEY_MENU	= 0x4d55;
const unsigned long TWOEM485_KEY_ENTER 	= 0x4545;

const unsigned long TWOEM485_KEY_LEFT  	= 0x444c;
const unsigned long TWOEM485_KEY_RIGHT 	= 0x4452;
const unsigned long TWOEM485_KEY_DOWN  	= 0x4444;
const unsigned long TWOEM485_KEY_UP    	= 0x4455;

const unsigned long TWOEM485_KEY_RECORD = 0x5245;
const unsigned long TWOEM485_KEY_PLAY   = 0x504c;
const unsigned long TWOEM485_KEY_ESC	= 0x5343;
const unsigned long TWOEM485_KEY_AUTO	= 0x4554;

const unsigned long TWOEM485_KEY_2X2	= 0x5046;
const unsigned long TWOEM485_KEY_3X3	= 0x5350;
const unsigned long TWOEM485_KEY_4X4	= 0x4552;

const unsigned long TWOEM485_KEY_BACKUP	= 0x424B;
const unsigned long TWOEM485_KEY_ZOOM	= 0x4543;

const unsigned long TWOEM485_KEY_ADD	= 0x4144;
const unsigned long TWOEM485_KEY_DEC	= 0x4D49;

//下面的键值还没用到
const unsigned long TWOEM485_KEY_LIVE		= 0x5059;
const unsigned long TWOEM485_KEY_FULLSCREEN	= 0x5256;

const unsigned long TWOEM485_KEY_PIP		= 0x5049;
const unsigned long TWOEM485_KEY_LOCK		= 0x4C43;

const unsigned long TWOEM485_KEY_SELECT		= 0x5243;
const unsigned long TWOEM485_KEY_FREEZE		= 0x5050;

//const unsigned long TWOEM485_KEY_SEARCH  	= 0x0e;
//const unsigned long TWOEM485_KEY_REW		= 0x10;
//const unsigned long TWOEM485_KEY_FF		= 0x11;
//const unsigned long TWOEM485_KEY_STOP		= 0x12;

#endif//_LOCAK_KEY_VALUE_H_

