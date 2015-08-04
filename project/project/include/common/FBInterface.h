/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2009-07-16
** Name         : FBInterface.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _FRAME_BUFFER_INTERFACE_H_
#define _FRAME_BUFFER_INTERFACE_H_
#include <vector>
#include "Typedef.h"
#include "Font.h"
#include "gui.h"
#include "Image.h"
#include "Product.h"

#ifdef __ENVIRONMENT_WIN32__
#include "DisplayCtrl.h"
#else
#include <linux/fb.h>

#ifdef __TDHISI__
typedef unsigned char           HI_U8;
typedef unsigned char           HI_UCHAR;
typedef unsigned short          HI_U16;
typedef unsigned int            HI_U32;

typedef signed char             HI_S8;
typedef short                   HI_S16;
typedef int                     HI_S32;

#ifndef _M_IX86
typedef unsigned long long      HI_U64;
typedef long long               HI_S64;
#else
typedef __int64                   HI_U64;
typedef __int64                   HI_S64;
#endif

typedef char                    HI_CHAR;
typedef char*                   HI_PCHAR;

typedef float                   HI_FLOAT;
typedef double                  HI_DOUBLE;
typedef void                    HI_VOID;

typedef unsigned long           HI_SIZE_T;
typedef unsigned long           HI_LENGTH_T;

typedef enum __hi_tool__
{
	HI_FALSE    = 0,
	HI_TRUE     = 1,
} HI_BOOL;

#define IOC_TYPE_HIFB               'F'
#define FBIOGET_COLORKEY_HIFB       _IOR(IOC_TYPE_HIFB, 90, HIFB_COLORKEY_S)
#define FBIOPUT_COLORKEY_HIFB       _IOW(IOC_TYPE_HIFB, 91, HIFB_COLORKEY_S)
#define FBIOGET_ALPHA_HIFB          _IOR(IOC_TYPE_HIFB, 92, HIFB_ALPHA_S)
#define FBIOPUT_ALPHA_HIFB          _IOW(IOC_TYPE_HIFB, 93, HIFB_ALPHA_S)
#define FBIOGET_SCREEN_ORIGIN_HIFB  _IOR(IOC_TYPE_HIFB, 94, HIFB_POINT_S)
#define FBIOPUT_SCREEN_ORIGIN_HIFB  _IOW(IOC_TYPE_HIFB, 95, HIFB_POINT_S)
#define FBIOGET_DEFLICKER_HIFB      _IOR(IOC_TYPE_HIFB, 98, HIFB_DEFLICKER_S)
#define FBIOPUT_DEFLICKER_HIFB      _IOW(IOC_TYPE_HIFB, 99, HIFB_DEFLICKER_S)

#define FBIOGET_VBLANK_HIFB         _IO (IOC_TYPE_HIFB, 100)

#define FBIOPUT_SHOW_HIFB           _IOW(IOC_TYPE_HIFB, 101, HI_BOOL)
#define FBIOGET_SHOW_HIFB           _IOR(IOC_TYPE_HIFB, 102, HI_BOOL)

#define FBIOGET_CAPABILITY_HIFB     _IOR(IOC_TYPE_HIFB, 103, HIFB_CAPABILITY_S)
#define FBIOFLIP_SURFACE            _IOW(IOC_TYPE_HIFB, 104, HIFB_SURFACE_S)

#define FBIOPUT_CURSOR_HI3511       _IOW(IOC_TYPE_HIFB, 105, HI3511_CURSOR_S)
#define FBIOGET_CURSOR_HI3511       _IOR(IOC_TYPE_HIFB, 106, HI3511_CURSOR_S)


/*hifb extend*/
typedef struct
{
	HI_BOOL bKeyEnable;         /* colorkey enable flag */
	HI_BOOL bMaskEnable;        /* key mask enable flag */
	HI_U32 u32Key;              /* colorkey value, maybe contains alpha */
	HI_U8 u8RedMask;            /* red mask */
	HI_U8 u8GreenMask;          /* green mask */
	HI_U8 u8BlueMask;           /* blue mask */
	HI_U8 u8Reserved;
}HIFB_COLORKEY_S;

typedef struct
{
	HI_U32 u32PosX;         /* horizontal position */
	HI_U32 u32PosY;         /* vertical position */
}HIFB_POINT_S;

typedef struct hiHIFB_DEFLICKER_S
{
	HI_U32  u32HDfLevel;    /* horizontal deflicker level */
	HI_U32  u32VDfLevel;    /* vertical deflicker level */
	HI_U8   *pu8HDfCoef;    /* horizontal deflicker coefficient */
	HI_U8   *pu8VDfCoef;    /* vertical deflicker coefficient */
}HIFB_DEFLICKER_S;

typedef struct
{
	HI_BOOL bAlphaEnable;   /* alpha enable flag */
	HI_BOOL bAlphaChannel;  /* alpha channel enable flag */
	HI_U8 u8Alpha0;         /* alpha0 value, used in ARGB1555 */
	HI_U8 u8Alpha1;         /* alpha1 value, used in ARGB1555 */
	HI_U8 u8GlobalAlpha;    /* global alpha value */
	HI_U8 u8Reserved;
}HIFB_ALPHA_S;

typedef enum __hifb_color_fmt_e__
{
	HIFB_FMT_1BPP = 0,      /* 1bpp */
	HIFB_FMT_2BPP,          /* 2bpp */
	HIFB_FMT_4BPP,          /* 4bpp */
	HIFB_FMT_8BPP,          /* 8bpp */
	HIFB_FMT_KRGB444,       /* RGB444 */
	HIFB_FMT_KRGB555,       /* RGB555 */
	HIFB_FMT_RGB565,        /* RGB565 */
	HIFB_FMT_ARGB4444,      /* RGB4444 */
	HIFB_FMT_ARGB1555,      /* RGB1555 */
	HIFB_FMT_KRGB888,       /* RGB888 */
	HIFB_FMT_ARGB8888,      /* RGB8888 */
	HIFB_FMT_BUTT
}HIFB_COLOR_FMT_E;

typedef struct
{
	HI_BOOL bKeyAlpha;              /* whether support colorkey alpha */
	HI_BOOL bGlobalAlpha;           /* whether support global alpha */
	HI_BOOL bCmap;                  /* whether support color map */
	HI_BOOL bColFmt[HIFB_FMT_BUTT]; /* support which color format */
	HI_U32  u32MaxWidth;            /* the max pixels per line */
	HI_U32  u32MaxHeight;           /* the max lines */
	HI_U32  u32MinWidth;            /* the min pixels per line */
	HI_U32  u32MinHeight;           /* the min lines */
	HI_U32  u32VDefLevel;           /* vertical deflicker level, 0 means vertical deflicker is unsupported */
	HI_U32  u32HDefLevel;           /* horizontal deflicker level, 0 means horizontal deflicker is unsupported */
}HIFB_CAPABILITY_S;

typedef struct
{
	HI_VOID* pvPhyAddr;         /* addr of the frame */
	HIFB_ALPHA_S stAlpha;       /* alpha properties */
	HIFB_COLORKEY_S stColorkey; /* colorkey properties */
}HIFB_SURFACE_S;

typedef enum __hi3511_cursor_e__
{
	HI3511_CURSOR_2COLOR = 0,
	HI3511_CURSOR_3COLOR,
	HI3511_CURSOR_4COLOR
}HI3511_CURSOR_E;

typedef struct
{
	HI_U8 u8Cr;
	HI_U8 u8Cb;
	HI_U8 u8Y;
	HI_U8 u8Reserved;
}HIFB_YCBCR_S;

typedef struct
{
	HI3511_CURSOR_E enCursor;
	HIFB_YCBCR_S stYCbCr[4];
}HI3511_CURSOR_S;
#endif
#endif

/*
���ڱ�����Ƶļ����뷨��
1������Ϊ�ϲ�ؼ�����ʵ�豸�Ľӿڲ㣬�����ṩ������ʾ�ӿڡ�
2��ÿ���ؼ��ͶԻ�����Ҫ�ӱ���̳У����߰���һ����Ա�������Ƽ�ʹ�ü̳й�ϵ��
3������ǲ�������̳У��������Է�ֹ�������౻�̳�ʱ����������
4���������Linux��Windows����ģʽ������Linux����frame buffer��������Windows����
����direct draw������ͨ����ȡ�Դ��ַ��ֱ�Ӱ���24bitλͼ��ʽ����ͼ�Ρ�
5�����пؼ����������඼�����������ֿռ�GUI�У������������ֳ�ͻ��
6��ָ���Դ�ĵ�ַ����ʾ�Դ泤��Ȳ����ĳ�Ա������Ϊ��̬����Ӧ�豸��Ψһ�ԡ�
7��������ʾ�����ֿ����ʽ���ֿ�Ϊ�Լ������ַ������ɣ�������ӳ�䵽24bitͼ����ʾ��
*/

namespace GUI
{
	typedef struct _mice_info
	{
		unsigned short x;
		unsigned short y;
		unsigned long bkGroup;
	}MICE_INFO;

	class CFBInterface
	{
	public:
		static CFBInterface * Instance();
		~CFBInterface();

#ifdef __ENVIRONMENT_WIN32__
		bool Initial(const char *pFilePath, CDisplayCtrl *pDisplay, unsigned char localVideoInputNum, unsigned char videoInputNum, unsigned char videoOutNum, unsigned char netVideoInputNum);
#else
		bool Initial(const char *pFilePath, unsigned short & width, unsigned short & height, unsigned char localVideoInputNum, unsigned char videoInputNum, unsigned char videoOutNum, unsigned char netVideoInputNum);
#endif
		void Quit();

		void SetImageAddr(unsigned char *pAudio, unsigned char *pMotion, unsigned char *pPTZ, unsigned char *pWait, unsigned char* pSnapSuccImage, unsigned char* pSnapFailImage, unsigned char* pLogo = NULL, int logoWidth = 0, int logoHeight = 0, int logoLen = 0);
		//////////////////////////////////////////////////////////////////////////
		void WaitProc(bool bShow);//����ʾ��MessageMan�е��á�

		void SetResolution(unsigned short cx, unsigned short cy);
		unsigned short GetResolutionWidth() const { return m_cx; }
		unsigned short GetResolutionHeight() const { return m_cy; }
		//////////////////////////////////////////////////////////////////////////
		void Refresh(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy);
		void Clear(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy);
		void DrawRect(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color);
		void DrawRectBorder(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, unsigned short height, unsigned short width);//���ƾ��α߿�
		void DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long color, unsigned short width);
		unsigned long DrawText(const char *pStr, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color, unsigned char zoom = 1, bool bForceDrawBorder = false, bool bResort = true);
		void DrawImage(const unsigned char *pImage, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short cx, unsigned short cy);

		//////////////////////////////////////////////////////////////////////////
		void DrawMice(unsigned short x, unsigned short y);

		bool TextSize(const char *pStr, unsigned long len, unsigned short &cx, unsigned short &cy, unsigned char zoom = 1);
		void SetTransparency(TRANSPARENCY transparency) {m_transparency = transparency;}

		//////////////////////////////////////////////////////////////////////////
		void RefreshLive();
		void SetDisplayAttrib(DISPLAY_ATTRIB *pDisplayAttrib, unsigned char chnnNum, unsigned long* pSplitMode, int splitNum);
		//////////////////////////////////////////////////////////////////////////
#ifdef __TDHISI__
		unsigned char *ImageBufAddr(unsigned char bufIndex);
#endif

		bool GetFont(unsigned char *pFontBuf, unsigned long & bufLen, unsigned short charID);
		bool GetFont(unsigned char *pFontBuf, unsigned long & bufLen, const char *pStr, unsigned long length);
	
		bool ReSortText(char* pText, const char* pSor, int len);

		void SetResStatusShow(unsigned long showCH) {m_showRecStatusCH = showCH;}

		void SetClickPos(unsigned long x,unsigned long y);
		void SendInitChnn(unsigned long chnn);

		GUI::CFont * GetFont(){return &m_font;}
		

	protected:
		//////////////////////////////////////////////////////////////////////////
	private:
		//////////////////////////////////////////////////////////////////////////
		CFBInterface();
		//////////////////////////////////////////////////////////////////////////
		unsigned char	*m_pFrameBuffer;
		unsigned char	*m_pDrawBuf;	//ָ���Դ���ַ��ָ��
		unsigned short	m_cx;			//ָ����ʾ��ȵ����ص���Ŀ
		unsigned short	m_cy;			//ָ����ʾ�߶ȵ����ص���Ŀ
		unsigned short	m_width;		//ָ��һ������ռ�õ��ֽ�����ָ����ʾ��ȵ����ص���Ŀ X 3��
		unsigned long	m_bufLen;		//ָ��������ʾ����Ĵ�С���ֽ�Ϊ��λ��
		GUI::CFont		m_font;

		unsigned char	*m_pAudioImage;
		unsigned char	*m_pMotionImage;
		unsigned char	*m_pPTZImage;
		unsigned char	*m_pWaitImage;
		unsigned char	*m_pLogoImage;
		unsigned char	*m_pSnapSuccImage;
		unsigned char	*m_pSnapFailImage;

		int				m_logoWidth;
		int				m_logoHeight;
		int				m_logoLen;
		//////////////////////////////////////////////////////////////////////////
#ifdef __ENVIRONMENT_WIN32__
		CDisplayCtrl	*m_pDisplay;
#else
		int			m_fb_fd;
		struct fb_fix_screeninfo m_fb_fix;
		struct fb_var_screeninfo m_fb_var;
#endif

		bool			m_bEnableRefreshLive;
		unsigned char	m_chnnNum;
		MICE_INFO		m_miceInfo;
		TRANSPARENCY	m_transparency;
		unsigned long	m_liveSplitMode[8];
		DISPLAY_ATTRIB	m_displayAttrib[MAX_CHNN_NUM];

		//��Ƶͨ��
		unsigned char	m_localVideoInputNum;
		unsigned char	m_videoInputNum;
		unsigned char	m_videoOutNum;
		unsigned char	m_netVideoInputNum;

		unsigned long	m_showRecStatusCH;

#ifdef __TDHISI__
		int						m_app_buf_addr_phy;
		unsigned char			*m_pAppBuf;
		int						m_fb_buf_addr_phy;
#endif


	unsigned long m_pos_x; //�طŷ��ص�x����
	unsigned long m_pos_y; //�طŷ��ص�y����
	unsigned long m_initchnn;//��ʼ��ʱ���ͨ����

	};
}
#endif //_FRAME_BUFFER_INTERFACE_H_
