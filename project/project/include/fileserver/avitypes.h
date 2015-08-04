#ifndef AVITYPES_H
#define AVITYPES_H

#define  AVI_VERSION1    0x10000
#define	 AVI_VERSION	0x20000 //��ʱ�����������ʱ�������

#define AVI_WRITER_SECC				(0)
#define	AVI_WRITER_FAIL				(-1)
const BYTE TimeBufLen=sizeof(LONGLONG)+1;
enum AVI_TYPE
{
	AVI_TYPE_STD,		//��׼AVI�ļ�
	AVI_TYPE_DEFSIZE,	//����ʱ�����Ķ���AVI�ļ�
	AVI_TYPE_NOSIZE		//����ʱ�����ķǶ���AVI�ļ�
};

#define SEEK_NEXT			1	//seek����һ֡
#define SEEK_PREV			2	//seek����һ֡
#define SEEK_NEXTKEY		3	//seek����һ���ؼ�֡
#define SEEK_PREVKEY		4	//seek����һ���ؼ�֡
#define	SEEK_CURPREVKEY		5	//seek��ǰ֡�Ĺؼ�֡
#define SEEK_CURNEXTKEY		6	//�����ǰ֡���ǹؼ�֡������һ��


#define FLAG_RIFF				mmioFOURCC('R', 'I', 'F', 'F')
#define FLAG_FTMS				mmioFOURCC('f', 't', 'm', 's')	
#define FLAG_LIST				mmioFOURCC('L', 'I', 'S', 'T')

//����֡�������ı��
#define INDEX_BVIDEO	mmioFOURCC('0', '0', 'd', 'b')
#define	INDEX_CVIDEO	mmioFOURCC('0', '0', 'd', 'c')
#define	INDEX_AUDIO		mmioFOURCC('0', '1', 'w', 'b')
#define	INDEX_VIDEOTIME	mmioFOURCC('0', '2', 'd', 'b')
#define	INDEX_AUDIOTIME	mmioFOURCC('0', '3', 'd', 'b')

#define FLAG_RIFF				mmioFOURCC('R', 'I', 'F', 'F')
#define FLAG_FTMS				mmioFOURCC('f', 't', 'm', 's')	
#define FLAG_LIST				mmioFOURCC('L', 'I', 'S', 'T')
#define streamtypeVIDEO         mmioFOURCC('v', 'i', 'd', 's')
#define streamtypeAUDIO         mmioFOURCC('a', 'u', 'd', 's')
#define streamtypeMIDI			mmioFOURCC('m', 'i', 'd', 's')
#define streamtypeTEXT          mmioFOURCC('t', 'x', 't', 's')

#define streamtypeVEVIOTIME     mmioFOURCC('v', 'd', 't', 's')
#define streamtypeAUDIOTIME     mmioFOURCC('a', 'd', 't', 's')

#define formtypeAVI             mmioFOURCC('A', 'V', 'I', ' ')
#define listtypeAVIHEADER       mmioFOURCC('h', 'd', 'r', 'l')
#define listtypeODMLExtendedAVIHeader	mmioFOURCC('o', 'd', 'm', 'l')
#define ckidAVIMAINHDR          mmioFOURCC('a', 'v', 'i', 'h')
#define ckidODMLExtendedAVIHeader	mmioFOURCC('d', 'm', 'l', 'h')
#define listtypeSTREAMHEADER    mmioFOURCC('s', 't', 'r', 'l')
#define ckidSTREAMHEADER        mmioFOURCC('s', 't', 'r', 'h')
#define ckidSTREAMFORMAT        mmioFOURCC('s', 't', 'r', 'f')
#define ckidSTREAMHANDLERDATA   mmioFOURCC('s', 't', 'r', 'd')
#define ckidSTREAMNAME			mmioFOURCC('s', 't', 'r', 'n')
#define ckidVideoPropHeader		mmioFOURCC('v', 'p', 'r', 'p')

#define listtypeAVIMOVIE        mmioFOURCC('m', 'o', 'v', 'i')
#define listtypeAVIRECORD       mmioFOURCC('r', 'e', 'c', ' ')

#define ckidAVINEWINDEX         mmioFOURCC('i', 'd', 'x', '1')
#define ckidAVIPADDING          mmioFOURCC('J', 'U', 'N', 'K')

//AVI�ļ��Ĺ̶�����
const long AVIFILELENGTH    =   64*1024*1024;
const long MAXFRAMES     	= 	AVIFILELENGTH*3/1024;
//AVI�ļ����⴦��ط�
const ULONG POS_AVIVER	   = 0x7e0; //�Զ���AVI�汾��AVI�ļ��е�λ��
const ULONG SIZE_AVIVER	   = 0x04;  //�Զ���AVI�汾ռ�ݵ��ֽ���

const ULONG POS_AVINORMAL  = 0x7e4; //�ڴ�λ�ñ�ʾAVI�ļ��Ƿ���������
const ULONG SIZE_AVINORMAL = 0x04; 
const ULONG AVI_NORMAL	   = 0x00;  //�˴���ֵΪAVI_NORMAL��ʾ�ļ���������������Ϊ����������

const long  INDEX_OFFSET  = (AVIFILELENGTH - MAXFRAMES*sizeof(AVIINDEXENTRY));
const long  INDEX_LENGTH  =	(8 + MAXFRAMES*sizeof(AVIINDEXENTRY));
const long  MOVI_POS_OFFSET = 2048;

const long	AVI_FRAME_INDEX_UNIT	= 30*600;

/*		
const long	AVI_BUFFER_SIZE			= 40*1024; 
const long  AVI_WRITER_BUF_FRAMES_NUM = 20;
const long  AVI_WRITER_BUF_INDEX_NUM  = AVI_WRITER_BUF_FRAMES_NUM*6;
*/
const int	AVICHECK_NORMAL	        =	0x100;//AVI�ļ�����״̬
const int	AVICHECK_NOOPEN		    =	0x101; //AVI��û��
const int	AVICHECK_NOWRITEDATA	=	0x102; //AVI��ûд��һ֡����
const int	AVICHECK_FILEFULL		=	0x105; //AVIʣ��ռ��Ѿ������޷�д������
const int	AVICHECK_FILEWOULDFULL	=	0x103; //AVI�ļ�������ʣ��ռ䲻��10M
const int	AVICHECK_MAYCHANGEFILE	=   0x104; //AVI�ļ����Ի��ˣ�ʣ��ռ䲻��2M

const long	AVI_WOULDFULLSIZE		= (10*1024*1024);
//const long	AVI_WOULDFULLFRAMES	    = (15*10);
const long	AVI_WOULDFULLFRAMES	    = (15*25);//ƽ��ÿ��60֡���ң�ʱ�䡢��Ƶ��
const long	AVI_CHANGEFILESIZE		= (2*1024*1024);
//const long	AVI_CHANGEFILEFRAMES	= (15*2);
const long	AVI_CHANGEFILEFRAMES	= (15*5);//ƽ��ÿ��60֡���ң�ʱ�䡢��Ƶ��


//////////////////////////////////////////////////////////////////////////
typedef struct _avimainheader
{
	unsigned long fcc;						//����Ϊ'avih'
	unsigned long cb;						//�����ݽṹ�Ĵ�С�������������8���ֽڣ�fcc��cb������
	unsigned long dwMicroSecPerFrame;		//��Ƶ֡���ʱ�䣨�Ժ���Ϊ��λ��
	unsigned long dwMaxBytesPerSec;			//���AVI�ļ������������
	unsigned long dwPaddingGranularity;		//������������
	unsigned long dwFlags;					// AVI�ļ���ȫ�ֱ�ǣ������Ƿ����������
	unsigned long dwTotalFrames;			//��֡��
	unsigned long dwInitialFrames;			//Ϊ������ʽָ����ʼ֡�����ǽ�����ʽӦ��ָ��Ϊ0��
	unsigned long dwStreams;				//���ļ����������ĸ���
	unsigned long dwSuggestedBufferSize;	//�����ȡ���ļ��Ļ����С��Ӧ���������Ŀ飩
	unsigned long dwWidth;					//��Ƶͼ��Ŀ�������Ϊ��λ��
	unsigned long dwHeight;					//��Ƶͼ��ĸߣ�������Ϊ��λ��
	unsigned long dwReserved[4];			//����
}AVIMAINHEADER, *PAVIMAINHEADER;

typedef struct _avistreamheader
{
	unsigned long fcc;					//����Ϊ'strh'
	unsigned long cb;					//�����ݽṹ�Ĵ�С�������������8���ֽڣ�fcc��cb������
	unsigned long fccType;				//�������ͣ�'auds'����Ƶ������'vids'����Ƶ������ 
										//'mids'��MIDI������'txts'����������
	unsigned long fccHandler;			//ָ�����Ĵ����ߣ���������Ƶ��˵���ǽ�����
	unsigned long dwFlages;				//��ǣ��Ƿ�����������������ɫ���Ƿ�仯��
	unsigned short wPriority;			//�������ȼ�
										//�����ж����ͬ���͵���ʱ���ȼ���ߵ�ΪĬ������
	unsigned short wLanguage;			//
	unsigned long dwInitialFrames;		//Ϊ������ʽָ����ʼ֡��
	unsigned long dwScale;				//�����ʹ�õ�ʱ��߶�
	unsigned long dwRate;				//
	unsigned long dwStart;				//���Ŀ�ʼʱ��
	unsigned long dwLength;				//���ĳ��ȣ���λ��dwScale��dwRate�Ķ����йأ�
	unsigned long dwSuggestedBufferSize;//��ȡ��������ݽ���ʹ�õĻ����С
	unsigned long dwQuality;			//�����ݵ�����ָ�꣨0 ~ 10,000��
	unsigned long dwSampleSize;			// Sample�Ĵ�С
	struct
	{
		short int left;
		short int top;
		short int right;
		short int bottom;
	}rcFrame;				//ָ�����������Ƶ����������������Ƶ�������е���ʾλ��
							// ��Ƶ��������AVIMAINHEADER�ṹ�е�dwWidth��dwHeight����
}AVISTREAMHEADER, *PAVISTREAMHEADER;

// fourcc
#define FOURCC_XVID		mmioFOURCC('X','V','I','D')
#define FOURCC_DIVX		mmioFOURCC('D','I','V','X')
#define FOURCC_MPEG2	mmioFOURCC('m','p','2','v')
#define FOURCC_MPEG1	mmioFOURCC('m','p','e','g')

typedef unsigned char       BYTE;

#define ACM_MPEG_LAYER1		(0x0001)
#define ACM_MPEG_LAYER2		(0x0002)
#define ACM_MPEG_LAYER3		(0x0004)
#define ACM_MPEG_STEREO		(0x0001)
#define ACM_MPEG_JOINTSTEREO	(0x0002)
#define ACM_MPEG_DUALCHANNEL	(0x0004)
#define ACM_MPEG_SINGLECHANNEL	(0x0008)
#define ACM_MPEG_PRIVATEBIT	(0x0001)
#define ACM_MPEG_COPYRIGHT	(0x0002)
#define ACM_MPEG_ORIGINALHOME	(0x0004)
#define ACM_MPEG_PROTECTIONBIT	(0x0008)
#define ACM_MPEG_ID_MPEG1	(0x0010)

#define AVIIF_LIST          0x00000001L // chunk is a 'LIST'
#define AVIIF_KEYFRAME      0x00000010L // this frame is a key frame.

#define AVIIF_NOTIME	    0x00000100L // this frame doesn't take any time
#define AVIIF_COMPUSE       0x0FFF0000L // these bits are for compressor use

#define AVIF_HASINDEX		0x00000010
#define AVIF_MUSTUSEINDEX	0x00000020
#define AVIF_ISINTERLEAVED	0x00000100

// TEMP
enum IME6400_ENCODETYPE	{ IME6400_ET_RESERVED0, IME6400_ET_VIDEO, IME6400_ET_RESERVED1, IME6400_ET_SYSTEM };
enum IME6400_SOURCETYPE	{ IME6400_ST_NTSC, IME6400_ST_PAL, IME6400_ST_SECAM };
enum IME6400_STANDARD	{ IME6400_SD_MPEG4, IME6400_SD_MPEG2, IME6400_SD_MPEG1 };
enum IME6400_PICSIZE
{
IME6400_PS_320X240,
IME6400_PS_640X480,
IME6400_PS_720X480,
IME6400_PS_176X144,
IME6400_PS_320X288,
IME6400_PS_352X288,
IME6400_PS_704X576,
IME6400_PS_720X576,
IME6400_PS_RESERVED0,
IME6400_PS_640X240,
IME6400_PS_720X240,
IME6400_PS_RESERVED1,
IME6400_PS_RESERVED2,
IME6400_PS_RESERVED3,
IME6400_PS_704X288,
IME6400_PS_720X288,
IME6400_PS_SUB720X480_352X240,
IME6400_PS_SUB704X576_352X288,
IME6400_PS_SUB720X576_352X288
};
enum IME6400_FRAMERATE
{
IME6400_FR_FULL,
IME6400_FR_DIV2,
IME6400_FR_DIV4,
IME6400_FR_DIV8,
IME6400_FR_DIV16,
IME6400_FR_DIV32
};
enum IME6400_PICTYPE { IME6400_PT_I_P, IME6400_PT_IONLY, IME6400_PT_IPB, IME6400_PT_IPBB };
enum IME6400_IINTERVAL
{
IME6400_II_2,
IME6400_II_4,
IME6400_II_8,
IME6400_II_16,
IME6400_II_32,
IME6400_II_64,
IME6400_II_128,
IME6400_II_256
};

enum IME6400_CHANNELMODE { IME6400_CM_SINGLE, IME6400_CM_SUBPIC, IME6400_CM_MULTI };
enum IME6400_AUDIOMODE { IME6400_AM_MPEG1L2, IME6400_AM_MULAW, IME6400_AM_ADPCM };
enum IME6400_AUDIOCHANNEL { IME6400_AC_STEREO, IME6400_AC_MONO };
enum IME6400_AUDIOSAMPLERATE { IME6400_AS_8KH, IME6400_AS_48KH };
enum IME6400_AUDIOBITRATE { IME6400_AB_MOSTLOW, IME6400_AB_LOW, IME6400_AB_NORMAL, IME6400_AB_HIGH };//, IME6400_AB_MOSTHIGH };
enum IME6400_BITRATECTRLMODE { IME6400_BC_VBR, IME6400_BC_CBR, IME6400_BC_HBR };
enum IME6400_MDTYPE { IME6400_MT_DISABLE, IME6400_MT_INT, IME6400_MT_EXT, IME6400_MT_BOTH };

// Structure for IME6400 firmware
#define IME6400FIRMWARE_BASEADDRESS			0x4000
#define IME6400FIRMWARE_MAXSIZE				0x3800
#define IME6400FIRMWARE_VIDEOMAXSIZE		0x2400
#define IME6400FIRMWARE_AUDIOMAXSIZE		0x2000

typedef struct tag_IME6400_FIRMWARE
{
	unsigned short	uFirmwareVersion;
	unsigned short	uBaseAddress;
	unsigned short	uAddressOffset;
	unsigned short	uSize;
	unsigned char	bExternalMode;
	unsigned char	bReset;		// Hardware dependent reset
	unsigned char	aData[IME6400FIRMWARE_MAXSIZE];
} IME6400_FIRMWARE;

#endif
