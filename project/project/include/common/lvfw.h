#ifndef LAVITYPES_H
#define LAVITYPES_H


typedef struct { 
    unsigned short  wFormatTag; 
    unsigned short  nChannels; 
    unsigned long   nSamplesPerSec; 
    unsigned long   nAvgBytesPerSec; 
    unsigned short  nBlockAlign; 
} __attribute__((packed)) WAVEFORMAT; 

typedef struct tWAVEFORMATEX
{
    unsigned short       wFormatTag;         /* format type */
    unsigned short       nChannels;          /* number of channels (i.e. mono, stereo...) */
    unsigned long       nSamplesPerSec;     /* sample rate */
    unsigned long       nAvgBytesPerSec;    /* for buffer estimation */
    unsigned short        nBlockAlign;        /* block size of data */
    unsigned short        wBitsPerSample;     /* number of bits per sample of mono data */
    unsigned short        cbSize;             /* the count in bytes of the size of */
				    /* extra information (after cbSize) */
} __attribute__((packed)) WAVEFORMATEX;

typedef struct mpeg1waveformat_tag {
	WAVEFORMATEX    wfx;
	unsigned short	fwHeadLayer;
	unsigned long	dwHeadBitrate;
	unsigned short	fwHeadMode;
	unsigned short	fwHeadModeExt;
	unsigned short	wHeadEmphasis;
	unsigned short	fwHeadFlags;
	unsigned long	dwPTSLow;
	unsigned long	dwPTSHigh;
} __attribute__((packed)) MPEG1WAVEFORMAT; // 40

typedef	MPEG1WAVEFORMAT	*PMPEG1WAVEFORMAT;

typedef struct mpeglayer3waveformat_tag {
	WAVEFORMATEX  wfx;
	unsigned short	wID;
	unsigned short	fdwFlags;
	unsigned short	nBlockSize;
	unsigned short	nFramesPerBlock;
	unsigned short	nCodecDelay;
} __attribute__((packed)) MPEGLAYER3WAVEFORMAT;

//
//  IMA endorsed ADPCM structure definitions--note that this is exactly
//  the same format as Intel's DVI ADPCM.
//
//      for WAVE_FORMAT_IMA_ADPCM   (0x0011)
//
//
typedef struct ima_adpcmwaveformat_tag {
	WAVEFORMATEX    wfx;
	WORD            wSamplesPerBlock;
} __attribute__((packed)) IMAADPCMWAVEFORMAT;

typedef struct tagBITMAPINFOHEADER{ // bmih 
	unsigned long  biSize; 
	long   biWidth; 
	long   biHeight; 
	unsigned short   biPlanes; 
	unsigned short   biBitCount;
	unsigned long  biCompression; 
	unsigned long  biSizeImage; 
	long   biXPelsPerMeter; 
	long   biYPelsPerMeter; 
	unsigned long  biClrUsed; 
	unsigned long  biClrImportant; 
}__attribute__((packed)) BITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER 
{
	unsigned short  bfType;
	unsigned long	bfSize;
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned long   bfOffBits;
} __attribute__((packed))BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagRGBQUAD {
	unsigned char    rgbBlue;
	unsigned char    rgbGreen;
	unsigned char    rgbRed;
	unsigned char    rgbReserved;
} __attribute__((packed))RGBQUAD;

#ifndef mmioFOURCC
#define mmioFOURCC( ch0, ch1, ch2, ch3 ) \
	( (unsigned long)(unsigned char)(ch0) | ( (unsigned long)(unsigned char)(ch1) << 8 ) |	\
	( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )
#endif

typedef struct 
{
	unsigned long	ckid;
	unsigned long	dwFlags;
	unsigned long	dwChunkOffset;		// Position of chunk
	unsigned long	dwChunkLength;		// Length of chunk
} __attribute__((packed)) AVIINDEXENTRY;

#define  WAVE_FORMAT_ALAW       0x0006  /*  Microsoft Corporation  */
#define  WAVE_FORMAT_MULAW      0x0007  /*  Microsoft Corporation  */
#define  WAVE_FORMAT_IMA_ADPCM  0x0011  /*  Intel Corporation  */
#define  WAVE_FORMAT_MPEG       0x0050  /*  Microsoft Corporation  */
#define  WAVE_FORMAT_MPEGLAYER3 0x0055  /*  ISO/MPEG Layer3 Format Tag */
#define  WAVE_FORMAT_DVI_ADPCM	WAVE_FORMAT_IMA_ADPCM
#define AVIERR_OK		0				
#define AVIERR_ERROR	1				



#endif
