#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "QRCode.h"

CQRCode::CQRCode()
{
   m_pFrame = NULL;
   m_width = 0;
   m_qrVersion =0;
   m_ecLevel = 0;
   
   m_pRsList = NULL;
   m_pCurrRs =NULL;

   //raw data
   m_pRawData =NULL;
   m_pInputData = NULL;

   //mask
   m_maskNum = 0;
}

CQRCode::~CQRCode()
{
    if(m_pFrame != NULL)
	{
		free(m_pFrame);
	}
	m_pFrame = NULL;
	InputFree();
	FreeRrawData(m_pRawData);
}

//get QR code matrix data interface
//str:in parameter,the transfer data source
//strSize:in parameter,the data source size
//out:out parameter,include matrix width and matrix data.the data is width*width matrix,and value 1 is black,0 is white.
//return:true,QR code success;false,QR code failed!
bool CQRCode::GetQRCodeMatrixData(unsigned char *str,int strSize,QrCodeOutputData *outParam)
{
   int i = 0;
   int j = 0;

	if(str == NULL || outParam== NULL)
   {
	   return false;
   }
	if(m_pFrame != NULL)
	{
		free(m_pFrame);
	}
	m_pFrame = NULL;
	InputFree();
	FreeRrawData(m_pRawData);

   //QR code process
	InputInit();
	InputAppend(QR_MODE_8, strSize, (unsigned char *)str);
	InputSetErrorCorrectionLevel(QR_ECLEVEL_L);

	Encode();
	InitQRFrame();
	FillQRFrame();

	MaskProcess();   
	//transfer into color value
	if(m_pFrame != NULL)
	{
		outParam->width = m_width;
		outParam->dataMatrix = NULL;
		outParam->dataMatrix = (unsigned char *)malloc(m_width*m_width*sizeof(unsigned char));
		if(outParam->dataMatrix == NULL)
		{
            return false;
		}
		memset(outParam->dataMatrix,0,m_width*m_width*sizeof(unsigned char));
		for(i= 0;i<m_width;i++)
		{
			for(j= 0;j<m_width;j++)
			{
			   outParam->dataMatrix[i*m_width +j] = m_pFrame[i*m_width +j];
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

//free the QR code data interface
void CQRCode::FreeQrCodeMatrix(QrCodeOutputData *outParam)
{
   if(outParam != NULL)
   {
	   outParam->width = 0;
	   if(outParam->dataMatrix != NULL)
	   {
		   free(outParam->dataMatrix);
	   }
	   outParam->dataMatrix = NULL;
   }
}

void CQRCode::InputInit(void)
{   
	m_pInputData = (QRInputData *)malloc(sizeof(QRInputData));
	if(m_pInputData == NULL) 
	{
		return;
	}

	m_pInputData->head = NULL;
	m_pInputData->tail = NULL;
	m_pInputData->version = 0;
	m_pInputData->level = QR_ECLEVEL_L;
	m_pInputData->mqr = 0;
	m_pInputData->fnc1 = 0; 
}

void CQRCode::InputFree(void)
{
	QRinput_List *list= NULL, *next=NULL;

	if(m_pInputData != NULL) 
	{
		list = m_pInputData->head;
		while(list != NULL) 
		{
			next = list->next;
			if(list->data!= NULL)
			{
				free(list->data);
			}
			list->data = NULL;
			if(list->bstream != NULL)
			{
				FreeInputBitStream(list->bstream);
			}
			list->bstream = NULL;

           free(list);
		    list = NULL;
			list = next;
		}

		free(m_pInputData);
	}
	list= NULL;
	next=NULL;
	m_pInputData = NULL;
}

int CQRCode::InputAppend(EQREncodeMode mode, int size, const unsigned char *data)
{
  
	QRinput_List *entry = NULL;

	entry = InputListNewEntry(mode, size, data);
	if(entry == NULL) 
	{
		return -1;
	}

	InputAppendEntry(entry);
	return 0;
}

int CQRCode::InputSetErrorCorrectionLevel(EQRErrorLevel level)
{
	if(level > QR_ECLEVEL_H) 
	{
		return -1;
	}

	m_pInputData->level = level;
    m_ecLevel = level;
	
	return 0;
}


//frame
bool CQRCode::InitQRFrame(void)
{
	int width = 0;
    int aVersion = 0;
    int aEcLevel = 0;

	//m_qrVersion = aVersion;
	aEcLevel = m_ecLevel;
	aVersion = m_qrVersion;

	width = QrcodeCapacity[aVersion].width;
	
	m_pFrame = (unsigned char *)malloc(width * width*sizeof(unsigned char));
	//m_pFrame = new unsigned char[width*width];
	if(m_pFrame == NULL) 
	{
		return false;
	}
	memset(m_pFrame, 0, width * width);

    m_width = width;


	/* Finder pattern */
	PutFinderPattern(0, 0);
	PutFinderPattern(width - 7, 0);
	PutFinderPattern(0, width - 7);
	/* Separator */
    PutSeparator();
	/* Mask format information area */
	SetMaskAreaInfo();
	/* Timing pattern */
	PutTimingPattern();
	/* Alignment pattern */
	PutAlignmentPattern();
	/* Version information */
    PutVersionInfo();

	/* and a little bit... */
	m_pFrame[width * (width - 8) + 8] = 0x81;
	return true;
}

void CQRCode::FillQRFrame(void)
{
	unsigned char *frame= NULL, *masked = NULL, *p= NULL, code=0, bit=0;
	FrameFiller *filler = NULL;
	int width= 0;
    int i = 0,j = 0;
    QRRawCode *raw=NULL;
    

    width= m_width;
	 raw = m_pRawData;//&m_qrRawData;
    

	 if(m_pFrame == NULL)
	 {
		 return;
	 }

	frame = m_pFrame;

	filler = InitFrameFiller(width, frame, 0);
	
	/* inteleaved data and ecc codes */
	for(i=0; i<raw->dataLength + raw->eccLength; i++) 
	{
		code = GetQrRawCode();
		bit = 0x80;
		for(j=0; j<8; j++) 
		{
			p = FrameFillerNext(filler);
			if(p == NULL)  
			{
				free(filler);
				
				return;
			}
			*p = 0x02 | (int)((bit & code) != 0);
			bit = bit >> 1;
		}
	}

	/* remainder bits */
	j = GetSpecRemainder(m_qrVersion);
	for(i=0; i<j; i++) {
		p = FrameFillerNext(filler);
		if(p == NULL)  
		{
			free(filler);

			return;
		}
		*p = 0x02;
	}

	raw=NULL;
	frame= NULL;
	masked = NULL;
	p= NULL;
	free(filler);
}

int  CQRCode::GetFrameWidth(void)
{
   return m_width;
}

int  CQRCode::GetQRVersion(void)
{
	return m_qrVersion;
}

void CQRCode::GetQRFrame(QrCodeOutputData *outParam)
{
   int i = 0;
   int j = 0;

   for(i=0;i<m_width;i++)
   {
	   for(j= 0;j<m_width;j++)
	   {
         printf("%02X ",m_pFrame[i*m_width+j]);
	   }
	   printf("\n");
   }

   //transfer into color value
   if(m_pFrame != NULL)
   {
	   outParam->width = m_width;
	   outParam->dataMatrix = NULL;
	   outParam->dataMatrix = (unsigned char *)malloc(m_width*m_width*sizeof(unsigned char));
	   if(outParam->dataMatrix == NULL)
	   {
		   return;
	   }
	   for(i= 0;i<m_width;i++)
	   {
		   for(j= 0;j<m_width;j++)
		   {
			   outParam->dataMatrix[i*m_width +j] =  m_pFrame[i*m_width +j];
		   }
	   }
   }

}

void CQRCode::Encode(void)
{
	QRRawCode *raw = NULL;
	int ret = -1;
    QRInputData *input= NULL;

	raw = (QRRawCode *)malloc(sizeof(QRRawCode));
	if(raw == NULL) 
	{
		return;
	}
    memset(raw,0,sizeof(QRRawCode));

    input = m_pInputData;
	raw->datacode = GetInputByteStream(input);
	if(raw->datacode == NULL) 
	{
		free(raw);
		return;
	}

	GetSpecEccSpec(input->version, input->level);

	raw->version = input->version;
	raw->b1 = GetRsBlockNum1();
	raw->dataLength = GetRsDataLength();
	raw->eccLength = GetRsEccLength();
	raw->ecccode = (unsigned char *)malloc(raw->eccLength*sizeof(unsigned char));
	if(raw->ecccode == NULL) 
	{
		if(raw->datacode != NULL)
		{
			free(raw->datacode);
		}
		raw->datacode = NULL;
		free(raw);
		return;
	}
	memset(raw->ecccode,0,raw->eccLength*sizeof(unsigned char));

	raw->blocks = GetRsBlockNum();
	raw->rsblock = (RSblock *)malloc(raw->blocks*sizeof(RSblock));
	if(raw->rsblock == NULL) 
	{
		FreeRrawData(raw);
		return ;
	}
    memset(raw->rsblock,0,raw->blocks*sizeof(RSblock));

	ret = InitRs(raw->rsblock,raw->datacode, raw->ecccode);
	if(ret < 0) 
	{
		FreeRrawData(raw);
		return;
	}
	raw->count = 0;
    
	m_pRawData = raw;
	raw = NULL;
	input= NULL;
	return;
}

void CQRCode::FreeRrawData(QRRawCode *raw)
{
	
	if(raw != NULL) 
	{
		if(raw->datacode!= NULL)
		{
			free(raw->datacode);
		}
		raw->datacode = NULL;
		if(raw->ecccode != NULL)
		{
			free(raw->ecccode);
		}
		raw->ecccode = NULL;
       if(raw->rsblock != NULL)
	   {
		   free(raw->rsblock);
	   }
	   raw->rsblock = NULL;
		free(raw);
	}
	raw = NULL;
}

void CQRCode::EcEncode()
{

}

void CQRCode::Mask()
{
  int maskValue=0;  
 
  m_maskNum = maskValue;
}


unsigned int CQRCode::GetSpecFormatInfo(int mask, EQRErrorLevel level)
{
	if(mask < 0 || mask > 7) 
	{
		return 0;
	}
	return QrFormatInfo[level][mask];
}

int CQRCode::GetSpeclengthIndicator(EQREncodeMode mode, int version)
{
	int l=0;

	if(!InputIsSplittableMode(mode)) 
	{
		return 0;
	}

	if(version <= 9) 
	{
		l = 0;
	} 
	else if(version <= 26) 
	{
		l = 1;
	} 
	else 
	{
		l = 2;
	}

	return QrCodeLengthTableBits[mode][l];
}

int CQRCode::GetSpecMinimumVersion(int size, EQRErrorLevel level)
{
	int i=0;
	int words=0;

	for(i=1; i<= gQRSpecVerMax; i++) 
	{
		words  = QrcodeCapacity[i].words - QrcodeCapacity[i].ec[level];
		if(words >= size) 
		{
			return i;
		}
	}

	return -1;
}

void CQRCode::GetSpecEccSpec(int version, EQRErrorLevel level)
{
	int b1, b2;
	int data, ecc;

	b1 = QrEccTable[version][level][0];
	b2 = QrEccTable[version][level][1];
	data = GetSpecDataLength(version, level);
	ecc  = GetSpecEccLength(version, level);

	if(b2 == 0) 
	{
		m_eccSpec[0] = b1;
		m_eccSpec[1] = data / b1;
		m_eccSpec[2] = ecc / b1;
		m_eccSpec[3] = 0;
		m_eccSpec[4] = 0;
	} 
	else 
	{
		m_eccSpec[0] = b1;
		m_eccSpec[1] = data / (b1 + b2);
		m_eccSpec[2] = ecc  / (b1 + b2);
		m_eccSpec[3] = b2;
		m_eccSpec[4] = m_eccSpec[1] + 1;
	}
}

int  CQRCode::GetSpecDataLength(int version,int eccLevel)
{
	return QrcodeCapacity[version].words - QrcodeCapacity[version].ec[eccLevel];   
}

int  CQRCode::GetSpecEccLength(int version,int eccLevel)
{
	return QrcodeCapacity[version].ec[eccLevel];
}

int CQRCode::GetSpecRemainder(int version)
{
	return QrcodeCapacity[version].remainder;
}

int CQRCode::GetSpecMaximumWords(EQREncodeMode mode, int version)
{
	int l;
	int bits;
	int words;

	if(!InputIsSplittableMode(mode)) 
	{
		return 0;
	}

	if(version <= 9) 
	{
		l = 0;
	} 
	else if(version <= 26) 
	{
		l = 1;
	} 
	else 
	{
		l = 2;
	}

	bits = QrCodeLengthTableBits[mode][l];
	words = (1 << bits) - 1;
	if(mode == QR_MODE_KANJI) 
	{
		words *= 2; // the number of bytes is required
	}
	return words;
}

//format information
void CQRCode::PutFinderPattern(int x,int y)
{
	static const unsigned char finder[] = {
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
		0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
		0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
	};
	int ox, oy;
	const unsigned char *s;
	unsigned char *tFrame=NULL;

    if(m_pFrame == NULL || m_width == 0)
	{
		return;
	}
   
	tFrame = m_pFrame + y * m_width + x;

	s = finder;
	for(oy=0; oy<7; oy++) 
	{
		for(ox=0; ox<7; ox++) 
		{
			tFrame[ox] = s[ox];
		}
		tFrame += m_width;
		s += 7;
	}
	tFrame =NULL;
}

void CQRCode::PutSeparator(void)
{
	unsigned char *p =NULL;
    unsigned char *q =NULL;
    int width = 0;
    int  y= 0;

	width = m_width;
	/* Separator */
	p = m_pFrame;
	q = m_pFrame + width * (width - 7);
	for(y=0; y<7; y++) 
	{
		p[7] = 0xc0;
		p[width - 8] = 0xc0;
		q[7] = 0xc0;
		p += width;
		q += width;
	}
	memset(m_pFrame + width * 7, 0xc0, 8);
	memset(m_pFrame + width * 8 - 8, 0xc0, 8);
	memset(m_pFrame + width * (width - 8), 0xc0, 8);
}

void CQRCode::SetMaskAreaInfo(void)
{
	unsigned char *p = NULL;
    int width = 0;
    int y=0;

	width = m_width;
	p= m_pFrame;
	/* Mask format information area */
	memset(m_pFrame + width * 8, 0x84, 9);
	memset(m_pFrame + width * 9 - 8, 0x84, 8);
	p = m_pFrame + 8;
	for(y=0; y<8; y++) 
	{
		*p = 0x84;
		p += width;
	}
	p = m_pFrame + width * (width - 7) + 8;
	for(y=0; y<7; y++) 
	{
		*p = 0x84;
		p += width;
	}
}

void CQRCode::PutTimingPattern(void)
{
	unsigned char *p = NULL;
	unsigned char *q = NULL;
	int width = 0;
    int x= 0;

	width = m_width;
	/* Timing pattern */
	p = m_pFrame + width * 6 + 8;
	q = m_pFrame + width * 8 + 6;
	for(x=1; x<width-15; x++) 
	{
		*p =  0x90 | (x & 1);
		*q =  0x90 | (x & 1);
		p++;
		q += width;
	}
}

void CQRCode::PutAlignmentPattern()
{
	int d, w, x, y, cx, cy;
    int version = 0;
	unsigned char *frame = NULL;
	int width = 0;
    
	version = m_qrVersion;
	frame = m_pFrame;
	width = m_width;

	if(version < 2) 
	{
		return;
	}

	d = QrAlignmentPattern[version][1] - QrAlignmentPattern[version][0];
	if(d < 0) 
	{
		w = 2;
	} 
	else 
	{
		w = (width - QrAlignmentPattern[version][0]) / d + 2;
	}

	if(w * w - 3 == 1) 
	{
		x = QrAlignmentPattern[version][0];
		y = QrAlignmentPattern[version][0];
		PutAlignmentMarker(x, y);
		return;
	}

	cx = QrAlignmentPattern[version][0];
	for(x=1; x<w - 1; x++) 
	{
		PutAlignmentMarker(6, cx);
		PutAlignmentMarker(cx,  6);
		cx += d;
	}

	cy = QrAlignmentPattern[version][0];
	for(y=0; y<w-1; y++) 
	{
		cx = QrAlignmentPattern[version][0];
		for(x=0; x<w-1; x++) 
		{
			PutAlignmentMarker(cx, cy);
			cx += d;
		}
		cy += d;
	}
}

void CQRCode::PutVersionInfo(void)
{
	int version = 0;
    unsigned int verinfo = 0;
    int x= 0,y = 0;
	int width = 0;
    int v = 0;
	unsigned char *p = NULL;

	width = m_width;
	version = m_qrVersion;

	/* Version information */
	if(version >= 7) 
	{
		verinfo = GetVersionPattern(version);

		p = m_pFrame + width * (width - 11);
		v = verinfo;
		for(x=0; x<6; x++) 
		{
			for(y=0; y<3; y++) 
			{
				p[width * y + x] = 0x88 | (v & 1);
				v = v >> 1;
			}
		}

		p = m_pFrame + width - 11;
		v = verinfo;
		for(y=0; y<6; y++) 
		{
			for(x=0; x<3; x++) 
			{
				p[x] = 0x88 | (v & 1);
				v = v >> 1;
			}
			p += width;
		}
	}
}

int CQRCode::PutFormatInfo(unsigned char *frame,int mask)
{
	unsigned int format= 0;
	unsigned char v= 0;
	int i= 0;
	int blacks = 0;
    int width= 0;

	EQRErrorLevel level = QR_ECLEVEL_L;


	level = (EQRErrorLevel)m_ecLevel;
	width = m_width;

	format = GetSpecFormatInfo(mask, level);

	for(i=0; i<8; i++) 
	{
		if(format & 1) 
		{
			blacks += 2;
			v = 0x85;
		} 
		else 
		{
			v = 0x84;
		}
		frame[width * 8 + width - 1 - i] = v;
		if(i < 6) 
		{
			frame[width * i + 8] = v;
		} 
		else 
		{
			frame[width * (i + 1) + 8] = v;
		}
		format= format >> 1;
	}
	for(i=0; i<7; i++) 
	{
		if(format & 1) 
		{
			blacks += 2;
			v = 0x85;
		} 
		else 
		{
			v = 0x84;
		}
		frame[width * (width - 7 + i) + 8] = v;
		if(i == 0) 
		{
			frame[width * 8 + 7] = v;
		} 
		else 
		{
			frame[width * 8 + 6 - i] = v;
		}
		format= format >> 1;
	}
    frame = NULL;
	return blacks; 
}

void CQRCode::PutAlignmentMarker(int centerX,int centerY)
{
	static const unsigned char finder[] = {
		0xa1, 0xa1, 0xa1, 0xa1, 0xa1,
		0xa1, 0xa0, 0xa0, 0xa0, 0xa1,
		0xa1, 0xa0, 0xa1, 0xa0, 0xa1,
		0xa1, 0xa0, 0xa0, 0xa0, 0xa1,
		0xa1, 0xa1, 0xa1, 0xa1, 0xa1,
	};
	int x, y;
	const unsigned char *s = NULL;
	unsigned char *frame =NULL;
    
	if(m_pFrame == NULL || m_width == 0)
	{
		return;
	}

    frame = m_pFrame;

	frame += (centerY - 2) * m_width + centerX - 2;
	s = finder;
	for(y=0; y<5; y++) 
	{
		for(x=0; x<5; x++) 
		{
			frame[x] = s[x];
		}
		frame += m_width;
		s += 5;
	}
	frame = NULL;
}

unsigned int CQRCode::GetVersionPattern(int version)
{
	if(version < 7 || version > gQRSpecVerMax) 
	{
		return 0;
	}

	return QrVersionPattern[version - 7];
}

unsigned int CQRCode::GetFormatInfo(int mask, EQRErrorLevel level)
{
	if(mask < 0 || mask > 7) 
	{
		return 0;
	}

	return QrFormatInfo[level][mask];
}

/******************************************************************************
* Entry of input data
*****************************************************************************/
QRinput_List *CQRCode::InputListNewEntry(EQREncodeMode mode, int size, const unsigned char *data)
{
	QRinput_List *entry=NULL;

	if(InputCheck(mode, size, data)) 
	{
		return NULL;
	}

	entry = (QRinput_List *)malloc(sizeof(QRinput_List));
	if(entry == NULL) 
	{
		return NULL;
	}

	entry->mode = mode;
	entry->size = size;
	if(size > 0) 
	{
		entry->data = (unsigned char *)malloc(size);
		if(entry->data == NULL) 
		{
			free(entry);
			return NULL;
		}
		memcpy(entry->data, data, size);
	}
	entry->bstream = NULL;
	entry->next = NULL;

	return entry;
}

void CQRCode::InputAppendEntry(QRinput_List *entry)
{
	if(m_pInputData->tail == NULL) 
	{
		m_pInputData->head = entry;
		m_pInputData->tail = entry;
	} 
	else 
	{
		m_pInputData->tail->next = entry;
		m_pInputData->tail = entry;
	}
	entry->next = NULL;
}

/******************************************************************************
* Validation
*****************************************************************************/

int CQRCode::InputCheck(EQREncodeMode mode, int size, const unsigned char *data)
{
	if((mode == QR_MODE_FNC1FIRST && size < 0) || size <= 0) 
	{
		return -1;
	}

	switch(mode) 
	{
	case QR_MODE_NUM:
		return InputCheckModeNum(size, (const char *)data);
	case QR_MODE_AN:
		return InputCheckModeAn(size, (const char *)data);
	case QR_MODE_KANJI:
		return InputCheckModeKanji(size, data);
	case QR_MODE_8:
		return 0;
	case QR_MODE_STRUCTURE:
		return 0;
	case QR_MODE_ECI:
		return 0;
	case QR_MODE_FNC1FIRST:
		return 0;
	case QR_MODE_FNC1SECOND:
		return 0;
	case QR_MODE_NUL:
		break;
	}

	return -1;
}
/******************************************************************************
* Numeric data
*****************************************************************************/
/**
* Check the input data.
* @param size
* @param data
* @return result
*/
int CQRCode::InputCheckModeNum(int size, const char *data)
{
	int i;

	for(i=0; i<size; i++) 
	{
		if(data[i] < '0' || data[i] > '9')
			return -1;
	}

	return 0;
}



/**
* Check the input data.
* @param size
* @param data
* @return result
*/
int CQRCode::InputCheckModeAn(int size, const char *data)
{
	int i;

	for(i=0; i<size; i++) 
	{
		//if(InputLookAnTable(data[i]) < 0)
		//{
		//	return -1;
		//}
	}

	return 0;
}

/**
* Check the input data.
* @param size
* @param data
* @return result
*/
int CQRCode::InputCheckModeKanji(int size, const unsigned char *data)
{
	int i=0;
	unsigned int val=0;

	if(size & 1)
	{
		return -1;
	}

	for(i=0; i<size; i+=2) 
	{
		val = ((unsigned int)data[i] << 8) | data[i+1];
		if(val < 0x8140 || (val > 0x9ffc && val < 0xe040) || val > 0xebbf) 
		{
			return -1;
		}
	}

	return 0;
}
int CQRCode::InputGetVersion(QRInputData *input)
{
	if(input == NULL)
	{
		return 0;
	}
	return input->version;
}

int CQRCode::InputSetVersion(QRInputData *input, int version)
{
	if(input->mqr || version < 0 || version > gQRSpecVerMax)
	{
		return -1;
	}

	input->version = version;

	return 0;
}
/**
* Estimates the required version number of the symbol.
* @param input input data
* @return required version number
*/
int CQRCode::InputEstimateVersion(QRInputData *input)
{
	int bits= 0;
	int version= 0;
	int prev= 0;

	version = 0;
	do 
	{
		prev = version;
		bits = InputEstimateBitStreamSize(input, prev);
		version = GetSpecMinimumVersion((bits + 7) / 8, input->level);
		if (version < 0) 
		{
			return -1;
		}
	} while (version > prev);

	return version;
}

/**
* Estimates the length of the encoded bit stream of the data.
* @param input input data
* @param version version of the symbol
* @return number of bits
*/
int CQRCode::InputEstimateBitStreamSize(QRInputData *input, int version)
{
	QRinput_List *list = NULL;
	int bits = 0;

	list = input->head;
	while(list != NULL) 
	{
		bits += InputEstimateBitStreamSizeOfEntry(list, version, input->mqr);
		list = list->next;
	}

	return bits;
}

/******************************************************************************
* Estimation of the bit length
*****************************************************************************/
/**
* Estimates the length of the encoded bit stream on the current version.
* @param entry
* @param version version of the symbol
* @param mqr
* @return number of bits
*/
int CQRCode::InputEstimateBitStreamSizeOfEntry(QRinput_List *entry, int version, int mqr)
{
	int bits = 0;
	int l, m;
	int num;

	if(version == 0) 
	{
		version = 1;
	}

	switch(entry->mode) 
	{
	case QR_MODE_NUM:
		bits = InputEstimateBitsModeNum(entry->size);
		break;
	case QR_MODE_AN:
		bits = InputEstimateBitsModeAn(entry->size);
		break;
	case QR_MODE_8:
		bits = InputEstimateBitsMode8(entry->size);
		break;
	case QR_MODE_KANJI:
		bits = InputEstimateBitsModeKanji(entry->size);
		break;
	case QR_MODE_STRUCTURE:
		return 0;
	case QR_MODE_ECI:

		break;
	case QR_MODE_FNC1FIRST:
		return 0;
	case QR_MODE_FNC1SECOND:
		return 0;
	default:
		return 0;
	}

	if(mqr) 
	{
		l = GetSpeclengthIndicator(entry->mode, version);
		m = version - 1;
		bits += l + m;
	} 
	else 
	{
		l = GetSpeclengthIndicator(entry->mode, version);
		m = 1 << l;
		num = (entry->size + m - 1) / m;

		bits += num * (gModeIndicatorSize + l);
	}

	return bits;
}

/**
* Estimates the length of the encoded bit stream of numeric data.
* @param size
* @return number of bits
*/
int CQRCode::InputEstimateBitsModeNum(int size)
{
	int w= 0;
	int bits = 0;

	w = size / 3;
	bits = w * 10;
	switch(size - w * 3) 
	{
	case 1:
		bits += 4;
		break;
	case 2:
		bits += 7;
		break;
	default:
		break;
	}

	return bits;
}

/**
* Estimates the length of the encoded bit stream of alphabet-numeric data.
* @param size
* @return number of bits
*/
int CQRCode::InputEstimateBitsModeAn(int size)
{
	int w=0;
	int bits=0;

	w = size / 2;
	bits = w * 11;
	if(size & 1) 
	{
		bits += 6;
	}

	return bits;
}

/**
* Estimates the length of the encoded bit stream of 8 bit data.
* @param size
* @return number of bits
*/
int CQRCode::InputEstimateBitsMode8(int size)
{
	return size * 8;
}
/**
* Estimates the length of the encoded bit stream of kanji data.
* @param size
* @return number of bits
*/
int CQRCode::InputEstimateBitsModeKanji(int size)
{
	return (size / 2) * 13;
}

/******************************************************************************
* Utilities
*****************************************************************************/
int CQRCode::InputIsSplittableMode(EQREncodeMode mode)
{
	return (mode >= QR_MODE_NUM && mode <= QR_MODE_KANJI);
}

/**
* Append padding bits for the input data.
* @param bstream Bitstream to be appended.
* @param input input data.
* @retval 0 success
* @retval -1 an error occurred and errno is set to indeicate the error.
*            See Execptions for the details.
* @throw ERANGE input data is too large.
* @throw ENOMEM unable to allocate memory.
*/
int CQRCode::InputAppendPaddingBit(BitStream *bstream, QRInputData *input)
{
	int bits=0, maxbits=0, words=0, maxwords=0, i=0, ret=0;
	BitStream *padding = NULL;
	unsigned char *padbuf= NULL;
	int padlen= 0;


	bits = bstream->length;
	maxwords = GetSpecDataLength(input->version, input->level);
	maxbits = maxwords * 8;

	if(maxbits < bits) 
	{
		return -1;
	}
	if(maxbits == bits) 
	{
		return 0;
	}

	if(maxbits - bits <= 4) 
	{
		ret = BitStreamAppendNum(bstream, maxbits - bits, 0);
		FreeInputBitStream(padding);
		return ret;
	}

	words = (bits + 4 + 7) / 8;

	padding = BitStreamNew();
	if(padding == NULL) 
	{
		return -1;
	}
	ret = BitStreamAppendNum(padding, words * 8 - bits, 0);
	if(ret < 0) 
	{
		FreeInputBitStream(padding);
		return ret;
	}

	padlen = maxwords - words;
	if(padlen > 0) 
	{
		padbuf = (unsigned char *)malloc(padlen);
		if(padbuf == NULL) 
		{
			ret = -1;

			FreeInputBitStream(padding);
			return ret;
		}
		for(i=0; i<padlen; i++) 
		{
			padbuf[i] = (i&1)?0x11:0xec;
		}
		ret = BitStreamAppendBytes(padding, padlen, padbuf);
		free(padbuf);
		if(ret < 0) 
		{
			FreeInputBitStream(padding);
			return ret;
		}
	}

	ret = BitStreamAppend(bstream, padding);

	FreeInputBitStream(padding);
	return ret;
}

//Rs control block
int CQRCode::InitRs(RSblock *blocks, unsigned char *data, unsigned char *ecc)
{
   int ret = 0;
   int i=0;
   RSblock *block = NULL;
   unsigned char *dp =NULL, *ep=NULL;
   QR_RS *rs =NULL;
   int el= 0, dl = 0;
   
   dl = GetRsDataCodes1();
   el = GetRsEccCodes1();
   CheckRS(8, 0x11d, 0, 1, el, 255 - dl - el);
   rs = m_pCurrRs;
   if(rs == NULL) 
   {
	   return -1;
   }

   block = blocks;
   dp = data;
   ep = ecc;
   for(i=0; i<GetRsBlockNum1(); i++) 
   {
	   InitRsBlock(block, dl, dp, el, ep, rs);
	   dp += dl;
	   ep += el;
	   block++;
   }

   if(GetRsBlockNum2() == 0) 
   {
	   return 0;
   }

   dl = GetRsDataCodes2();
   el = GetRsEccCodes2();
   CheckRS(8, 0x11d, 0, 1, el, 255 - dl - el);
   rs = m_pCurrRs;
   if(rs == NULL) 
   {
	   return -1;
   }
   for(i=0; i < GetRsBlockNum2(); i++) 
   {
	   InitRsBlock(block, dl, dp, el, ep, rs);
	   dp += dl;
	   ep += el;
	   block++;
   }
   
   FreeRsCache();
   return ret;
}

void CQRCode::InitRsBlock(RSblock *block, int dl, unsigned char *data, int el, unsigned char *ecc, QR_RS *rs)
{
	if(block == NULL)
	{
		return;
	}
	block->dataLength = dl;
	block->data = data;
	block->eccLength = el;
	block->ecc = ecc;

	EncodeRsChar(rs, data, ecc);
}


//error correct operation
void CQRCode::CheckRS(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad)
{
	QR_RS *rs = NULL;

	for(rs = m_pRsList; rs != NULL; rs = rs->next) 
	{
		if(rs->pad != pad) 
		{
			continue;
		}
		if(rs->nroots != nroots) 
		{
			continue;
		}
		if(rs->mm != symsize) 
		{
			continue;
		}
		if(rs->gfpoly != gfpoly) 
		{
			continue;
		}
		if(rs->fcr != fcr) 
		{
			continue;
		}
		if(rs->prim != prim) 
		{
			continue;
		}
       m_pCurrRs = rs;
		return;
	}

    rs =NULL;
	rs = InitRsChar(symsize, gfpoly, fcr, prim, nroots, pad);
	if(rs == NULL) 
	{
		return;
	}

	rs->next = m_pRsList;
	m_pRsList = rs;
    m_pCurrRs = rs;

	return;
}

int CQRCode::Modnn(QR_RS *rs, int x)
{
	while (x >= rs->nn) 
	{
		x -= rs->nn;
		x = (x >> rs->mm) + (x & rs->nn);
	}
	return x;
}


QR_RS *CQRCode::InitRsChar(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad)
{
	QR_RS *rs = NULL;

	int i, j, sr,root,iprim;

	/* Check parameter ranges */
	if(symsize < 0 || symsize > (int)(8*sizeof(unsigned char)))
	{
		return NULL;
	}

	if(fcr < 0 || fcr >= (1<<symsize))
	{	
		return NULL;
	}
	if(prim <= 0 || prim >= (1<<symsize))
	{
		return NULL;
	}
	if(nroots < 0 || nroots >= (1<<symsize))
	{
		return NULL; /* Can't have more roots than symbol values! */
	}
	if(pad < 0 || pad >= ((1<<symsize) -1 - nroots))
	{
		return NULL;; /* Too much padding */
	}

	rs = (QR_RS *)malloc(1*sizeof(QR_RS));
	if(rs == NULL)
	{	
		return NULL;
	}
	memset(rs,0,sizeof(QR_RS));

	rs->mm = symsize;
	rs->nn = (1<<symsize)-1;
	rs->pad = pad;

	rs->alpha_to = (unsigned char *)malloc(sizeof(unsigned char)*(rs->nn+1));
	if(rs->alpha_to == NULL)
	{
		free(rs);
		rs = NULL;
		return NULL;
	}
	rs->index_of = (unsigned char *)malloc(sizeof(unsigned char)*(rs->nn+1));
	if(rs->index_of == NULL)
	{
		free(rs->alpha_to);
		free(rs);
		rs = NULL;
		return NULL;
	}

	/* Generate Galois field lookup tables */
	rs->index_of[0] = rs->nn; /* log(zero) = -inf */
	rs->alpha_to[rs->nn] = 0; /* alpha**-inf = 0 */

	sr = 1;
	for(i=0;i<rs->nn;i++)
	{
		rs->index_of[sr] = i;
		rs->alpha_to[i] = sr;
		sr <<= 1;
		if(sr & (1<<symsize))
		{
			sr ^= gfpoly;
		}
		sr &= rs->nn;
	}
	if(sr != 1)
	{
		/* field generator polynomial is not primitive! */
		free(rs->alpha_to);
		free(rs->index_of);
		free(rs);
		rs = NULL;
		return NULL;
	}

	/* Form RS code generator polynomial from its roots */
	rs->genpoly = (unsigned char *)malloc(sizeof(unsigned char)*(nroots+1));
	if(rs->genpoly == NULL)
	{
		free(rs->alpha_to);
		free(rs->index_of);
		free(rs);
		rs = NULL;
		return NULL;
	}
	rs->fcr = fcr;
	rs->prim = prim;
	rs->nroots = nroots;
	rs->gfpoly = gfpoly;

	/* Find prim-th root of 1, used in decoding */
	for(iprim=1;(iprim % prim) != 0;iprim += rs->nn)
		;
	rs->iprim = iprim / prim;

	rs->genpoly[0] = 1;
	for (i = 0,root=fcr*prim; i < nroots; i++,root += prim) 
	{
		rs->genpoly[i+1] = 1;

		/* Multiply rs->genpoly[] by  @**(root + x) */
		for (j = i; j > 0; j--)
		{
			if (rs->genpoly[j] != 0)
			{
				rs->genpoly[j] = rs->genpoly[j-1] ^ rs->alpha_to[Modnn(rs,rs->index_of[rs->genpoly[j]] + root)];
			}
			else
			{
				rs->genpoly[j] = rs->genpoly[j-1];
			}
		}
		/* rs->genpoly[0] can never be zero */
		rs->genpoly[0] = rs->alpha_to[Modnn(rs,rs->index_of[rs->genpoly[0]] + root)];
	}

	/* convert rs->genpoly[] to index form for quicker encoding */
	for (i = 0; i <= nroots; i++)
	{
		rs->genpoly[i] = rs->index_of[rs->genpoly[i]];
	}

	return rs;
}

void CQRCode::FreeRsChar(QR_RS *rs)
{
	if(rs->alpha_to != NULL)
	{
		free(rs->alpha_to);
	}
    rs->alpha_to = NULL; 
	
	if(rs->index_of != NULL)
	{
	  free(rs->index_of);
	}
    rs->index_of = NULL;
	
	if(rs->genpoly != NULL)
	{
		free(rs->genpoly);
	}
	rs->genpoly = NULL;
	if(rs != NULL)
	{
		free(rs);
	}
	rs =NULL;
}

void CQRCode::FreeRsCache(void)
{
	QR_RS *rs =NULL;
	QR_RS *next =NULL;
   
	m_pCurrRs = NULL;
	rs = m_pRsList;
	while(rs != NULL) 
	{
		next = rs->next;
		FreeRsChar(rs);
		rs = next;
	}
	m_pRsList = NULL;
}

void CQRCode::EncodeRsChar(QR_RS *rs, const unsigned char *data, unsigned char *parity)
{
	int i= 0, j= 0;
	unsigned char feedback= 0;
    int nRoots = 0;
    int tNN = 0;
    int tPad = 0;
    unsigned char *tIndex = NULL;
	unsigned  char *tGenPoly=NULL;
	unsigned  char *tAlphaTo=NULL;

	if(rs == NULL)
	{
		return;
	}

	tNN = rs->nn;
	nRoots = rs->nroots;
	tPad = rs->pad;

	memset(parity,0,nRoots*sizeof(unsigned char));
    
	tIndex = rs->index_of;
	tGenPoly = rs->genpoly;
	tAlphaTo = rs->alpha_to;

	for(i=0;i<tNN-nRoots-tPad;i++)
	{
		feedback = tIndex[data[i] ^ parity[0]];
		if(feedback != tNN)
		{      /* feedback term is non-zero */
#ifdef UNNORMALIZED
			/* This line is unnecessary when GENPOLY[NROOTS] is unity, as it must
			* always be for the polynomials constructed by init_rs()
			*/
			feedback = Modnn(rs,tNN - tGenPoly[nRoots] + feedback);
#endif
			for(j=1;j<nRoots;j++)
			{
				parity[j] ^= tAlphaTo[Modnn(rs,feedback + tGenPoly[nRoots-j])];
			}
		}

		/* Shift */
		memmove(&parity[0],&parity[1],sizeof(unsigned char)*(nRoots-1));
		if(feedback != tNN)
		{
			parity[nRoots-1] = tAlphaTo[Modnn(rs,feedback + tGenPoly[0])];
		}
		else
		{
			parity[nRoots-1] = 0;
		}
	}
	tIndex = NULL;
	tGenPoly=NULL;
	tAlphaTo=NULL;
}

//generate bit stream
unsigned char *CQRCode::GetInputByteStream(QRInputData *input)
{
	BitStream *bstream = NULL;
	unsigned char *array = NULL;

	bstream = GetInputBitStream(input);
	if(bstream == NULL) 
	{
		return NULL;
	}
	array = InputBitStreamToByte(bstream);
	FreeInputBitStream(bstream);

	return array;
}

BitStream *CQRCode::GetInputBitStream(QRInputData *input)
{
	BitStream *bstream = NULL;
	int ret = 0;

	bstream = MergeBitStream(input);
	if(bstream == NULL) 
	{
		return NULL;
	}

	if(input->mqr) 
	{
		
	} 
	else 
	{
		ret = InputAppendPaddingBit(bstream, input);
	}

	if(ret < 0) 
	{
		FreeInputBitStream(bstream);
		return NULL;
	}

	return bstream;
}

BitStream *CQRCode::BitStreamNew(void)
{
	BitStream *bstream;

	bstream = (BitStream *)malloc(sizeof(BitStream));
	if(bstream == NULL) 
	{
		return NULL;
	}

	bstream->length = 0;
	bstream->data = NULL;

	return bstream;
}

/**
* Merge all bit streams in the input data.
* @param input input data.
* @return merged bit stream
*/
BitStream *CQRCode::MergeBitStream(QRInputData *input)
{
	BitStream *bstream = NULL;
	QRinput_List *list = NULL;
	int ret;

	if(input->mqr) 
	{

	} 
	else 
	{
		if(input->fnc1) 
		{

		}
		if(ConvertDataToBits(input) < 0) 
		{
			return NULL;
		}
	}

	bstream = BitStreamNew();
	if(bstream == NULL) 
	{
		return NULL;
	}

	list = input->head;
	while(list != NULL) 
	{
		ret = BitStreamAppend(bstream, list->bstream);
		if(ret < 0) 
		{
			FreeInputBitStream(bstream);
			return NULL;
		}
		list = list->next;
	}

	return bstream;
}



int CQRCode::BitStreamAppend(BitStream *bstream, BitStream *arg)
{
	unsigned char *data;

	if(arg == NULL) {
		return -1;
	}
	if(arg->length == 0) {
		return 0;
	}
	if(bstream->length == 0) 
	{
		if(BitStreamAllocate(bstream, arg->length)) 
		{
			return -1;
		}
		memcpy(bstream->data, arg->data, arg->length);
		return 0;
	}

	data = (unsigned char *)malloc(bstream->length + arg->length);
	if(data == NULL) 
	{
		return -1;
	}
	memcpy(data, bstream->data, bstream->length);
	memcpy(data + bstream->length, arg->data, arg->length);

	free(bstream->data);
	bstream->length += arg->length;
	bstream->data = data;

	return 0;
}

int CQRCode::BitStreamAllocate(BitStream *bstream, int length)
{
	unsigned char *data;

	if(bstream == NULL) 
	{
		return -1;
	}

	data = (unsigned char *)malloc(length);
	if(data == NULL) 
	{
		return -1;
	}

	if(bstream->data) 
	{
		free(bstream->data);
	}
	bstream->length = length;
	bstream->data = data;

	return 0;
}

unsigned char *CQRCode::InputBitStreamToByte(BitStream *bstream)
{
	int i, j, size, bytes;
	unsigned char *data =NULL, v;
	unsigned char *p =NULL;

    if(bstream == NULL)
	{
		return 0;
	}
	size = bstream->length;
	if(size == 0) 
	{
		return NULL;
	}

	data = (unsigned char *)malloc((size + 7) / 8);
	if(data == NULL) 
	{
		return NULL;
	}

	bytes = size  / 8;

	p = bstream->data;
	for(i=0; i<bytes; i++) 
	{
		v = 0;
		for(j=0; j<8; j++) 
		{
			v = v << 1;
			v |= *p;
			p++;
		}
		data[i] = v;
	}
	if(size & 7) 
	{
		v = 0;
		for(j=0; j<(size & 7); j++) 
		{
			v = v << 1;
			v |= *p;
			p++;
		}
		data[bytes] = v;
	}

	return data;
}

void CQRCode::FreeInputBitStream(BitStream *bstream)
{
	if(bstream != NULL) 
	{
		if(bstream->data != NULL)
		{
			free(bstream->data);
		}
		bstream->data =NULL;
		free(bstream);
	}
	bstream = NULL;
}

////////////////////////////////////////////////////////////////////////
/////bitstream process//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int CQRCode::BitStreamAppendNum(BitStream *bstream, int bits, unsigned int num)
{
	BitStream *b =NULL;
	int ret=0;

	if(bits == 0) 
	{
		return 0;
	}

	b = BitStreamNewFromNum(bits, num);
	if(b == NULL) 
	{
		return -1;
	}

	ret = BitStreamAppend(bstream, b);
	FreeInputBitStream(b);

	return ret;
}

BitStream *CQRCode::BitStreamNewFromNum(int bits, unsigned int num)
{
	unsigned int mask;
	int i;
	unsigned char *p =NULL;
	BitStream *bstream=NULL;

	bstream = BitStreamNew();
	if(bstream == NULL)
	{
		return NULL;
	}

	if(BitStreamAllocate(bstream, bits)) 
	{
		FreeInputBitStream(bstream);
		return NULL;
	}

	p = bstream->data;
	mask = 1 << (bits - 1);
	for(i=0; i<bits; i++) 
	{
		if(num & mask) 
		{
			*p = 1;
		} 
		else 
		{
			*p = 0;
		}
		p++;
		mask = mask >> 1;
	}

	return bstream;
}

int CQRCode::BitStreamAppendBytes(BitStream *bstream, int size, unsigned char *data)
{
	BitStream *b=NULL;
	int ret=0;

	if(size == 0) 
	{
		return 0;
	}

	b = BitStreamNewFromBytes(size, data);
	if(b == NULL) 
	{
		return -1;
	}

	ret = BitStreamAppend(bstream, b);
	FreeInputBitStream(b);

	return ret;
}


BitStream *CQRCode::BitStreamNewFromBytes(int size, unsigned char *data)
{
	unsigned char mask=0;
	int i=0, j=0;
	unsigned char *p=NULL;
	BitStream *bstream=NULL;

	bstream = BitStreamNew();
	if(bstream == NULL) 
	{
		return NULL;
	}

	if(BitStreamAllocate(bstream, size * 8)) 
	{
		FreeInputBitStream(bstream);
		return NULL;
	}

	p = bstream->data;
	for(i=0; i<size; i++) 
	{
		mask = 0x80;
		for(j=0; j<8; j++) 
		{
			if(data[i] & mask) 
			{
				*p = 1;
			} 
			else 
			{
				*p = 0;
			}
			p++;
			mask = mask >> 1;
		}
	}

	return bstream;
}

/**
* Convert the input data to a bit stream.
* @param input input data.
* @retval 0 success
* @retval -1 an error occurred and errno is set to indeicate the error.
*            See Execptions for the details.
* @throw ENOMEM unable to allocate memory.
*/
int CQRCode::CreateBitStream(QRInputData *input)
{
	QRinput_List *list= NULL;
	int bits = -1, total = 0;

	list = input->head;
	while(list != NULL) 
	{
		//encode bit stream interface
		bits = InputEncodeBitStream(list, input->version, input->mqr);
		if(bits < 0) 
		{
			return -1;
		}
		total += bits;
		list = list->next;
	}

	return total;
}



/**
* Convert the input data to a bit stream.
* When the version number is given and that is not sufficient, it is increased
* automatically.
* @param input input data.
* @retval 0 success
* @retval -1 an error occurred and errno is set to indeicate the error.
*            See Execptions for the details.
* @throw ENOMEM unable to allocate memory.
* @throw ERANGE input is too large.
*/
int CQRCode::ConvertDataToBits(QRInputData *input)
{
	int bits;
	int ver;

	ver = InputEstimateVersion(input);
	if(ver > InputGetVersion(input)) 
	{
		InputSetVersion(input, ver);
		m_qrVersion = ver;//
	}

	for(;;) 
	{
		bits = CreateBitStream(input);
		if(bits < 0) 
		{
			return -1;
		}
		ver = GetSpecMinimumVersion((bits + 7) / 8, input->level);
		if(ver < 0) 
		{
			return -1;
		} 
		else if(ver > InputGetVersion(input)) 
		{
			InputSetVersion(input, ver);
			m_qrVersion = ver;//
		} 
		else 
		{
			break;
		}
	}

	return 0;
}

/******************************************************************************
* Frame filling
*****************************************************************************/
FrameFiller *CQRCode::InitFrameFiller(int width, unsigned char *frame, int mqr)
{
	FrameFiller *filler;

	filler = (FrameFiller *)malloc(sizeof(FrameFiller));
	if(filler == NULL) 
	{
		return NULL;
	}
	filler->width = width;
	filler->frame = frame;
	filler->x = width - 1;
	filler->y = width - 1;
	filler->dir = -1;
	filler->bit = -1;
	filler->mqr = mqr;

	return filler;
}

unsigned char *CQRCode::FrameFillerNext(FrameFiller *filler)
{
	unsigned char *p =NULL;
	int x=0, y=0, w = 0;

	if(filler->bit == -1) 
	{
		filler->bit = 0;
		return filler->frame + filler->y * filler->width + filler->x;
	}

	x = filler->x;
	y = filler->y;
	p = filler->frame;
	w = filler->width;

	if(filler->bit == 0) 
	{
		x--;
		filler->bit++;
	} 
	else 
	{
		x++;
		y += filler->dir;
		filler->bit--;
	}

	if(filler->dir < 0) 
	{
		if(y < 0) 
		{
			y = 0;
			x -= 2;
			filler->dir = 1;
			if(!filler->mqr && x == 6) 
			{
				x--;
				y = 9;
			}
		}
	} 
	else 
	{
		if(y == w) 
		{
			y = w - 1;
			x -= 2;
			filler->dir = -1;
			if(!filler->mqr && x == 6) 
			{
				x--;
				y -= 8;
			}
		}
	}
	if(x < 0 || y < 0) 
	{
		return NULL;
	}

	filler->x = x;
	filler->y = y;
    
	//if this position has data 
	if(p[y * w + x] & 0x80) 
	{
		// This tail recursion could be optimized.
		return FrameFillerNext(filler);
	}
	return &p[y * w + x];
}

/**
* Return a code (byte).
* This function can be called iteratively.
* @param raw raw code.
* @return code
*/
unsigned char CQRCode::GetQrRawCode(void)
{
	int col =0, row = 0;
	unsigned char ret= 0;
    QRRawCode *raw = NULL;

	raw = m_pRawData;
	if(raw->rsblock == NULL)
	{
		raw = NULL;
		return 0;
	}

	if(raw->count < raw->dataLength) 
	{
		row = raw->count % raw->blocks;
		col = raw->count / raw->blocks;
		if(col >= raw->rsblock[0].dataLength) 
		{
			row += raw->b1;
		}
		ret = raw->rsblock[row].data[col];
	} 
	else if(raw->count < raw->dataLength + raw->eccLength) 
	{
		row = (raw->count - raw->dataLength) % raw->blocks;
		col = (raw->count - raw->dataLength) / raw->blocks;
		ret = raw->rsblock[row].ecc[col];
	} 
	else 
	{
		return 0;
	}
	raw->count++;
	raw = NULL;
	return ret;
}


/******************************************************************************
*************************Mask process******************************************
******************************************************************************/
//maskNum:the mask number from 0 to 7
//width:the QR matrix width
//s:source data before mask process
//d:the mask result
int CQRCode::MakeMask(int maskNum,int width, const unsigned char *s, unsigned char *d)
{
	int x, y;
	int b = 0;
	int exp =0;
   
	if(maskNum< 0 || maskNum >7)
	{
		return 0;
	}

	for(y=0; y<width; y++) 
	{
		for(x=0; x<width; x++) 
		{
			if(*s & 0x80) 
			{
				*d = *s;
			} 
			else 
			{
				switch(maskNum)
				{
					case 0:
						exp= (x+y)&1;
						break;
					case 1:
						exp= y&1;
						break;
					case 2:
						exp= x%3;
						break;
					case 3:
						exp= (x+y)%3;
						break;
					case 4:
						exp= ((y/2)+(x/3))&1;
						break;
					case 5:
						exp= ((x*y)&1)+(x*y)%3;
						break;
					case 6:
						exp= (((x*y)&1)+(x*y)%3)&1;
						break;
					case 7:
						exp= (((x*y)%3)+((x+y)&1))&1;
						break;
					default:
						break;
				}
				*d = *s ^ (exp == 0);
			}
			b += (int)(*d & 1);
			s++; 
			d++;
		}
	}
	return b;
}

int CQRCode::MaskCalcN1N3(int length, int *runLength)
{
	int i;
	int demerit = 0;
	int fact;

	for(i=0; i<length; i++) 
	{
		if(runLength[i] >= 5) 
		{
			demerit += MaskN1 + (runLength[i] - 5);
			//n1 += N1 + (runLength[i] - 5);
		}
		if((i & 1)) 
		{
			if(i >= 3 && i < length-2 && (runLength[i] % 3) == 0) 
			{
				fact = runLength[i] / 3;
				if(runLength[i-2] == fact &&
					runLength[i-1] == fact &&
					runLength[i+1] == fact &&
					runLength[i+2] == fact) 
				{
						if(i == 3 || runLength[i-3] >= 4 * fact) 
						{
							demerit += MaskN3;
							//n3 += N3;
						} 
						else if(i+4 >= length || runLength[i+3] >= 4 * fact) 
						{
							demerit += MaskN3;
							//n3 += N3;
						}
				}
			}
		}
	}

	return demerit;
}

int CQRCode::MaskCalcN2(int width, unsigned char *frame)
{
	int x= 0, y= 0;
	unsigned char *p;
	unsigned char b22= 0, w22= 0;
	int demerit = 0;

	p = frame + width + 1;
	for(y=1; y<width; y++) 
	{
		for(x=1; x<width; x++) 
		{
			b22 = p[0] & p[-1] & p[-width] & p [-width-1];
			w22 = p[0] | p[-1] | p[-width] | p [-width-1];
			if((b22 | (w22 ^ 1))&1) 
			{
				demerit += MaskN2;
			}
			p++;
		}
		p++;
	}

	return demerit;
}

int CQRCode::MaskCalcRunLength(int width, unsigned char *frame, int dir, int *runLength)
{
	int head;
	int i;
	unsigned char *p;
	int pitch;

	pitch = (dir==0)?1:width;
	if(frame[0] & 1) 
	{
		runLength[0] = -1;
		head = 1;
	} 
	else 
	{
		head = 0;
	}
	runLength[head] = 1;
	p = frame + pitch;

	for(i=1; i<width; i++) 
	{
		if((p[0] ^ p[-pitch]) & 1) 
		{
			head++;
			runLength[head] = 1;
		} 
		else 
		{
			runLength[head]++;
		}
		p += pitch;
	}

	return head + 1;
}


int CQRCode::MaskEvaluateSymbol(int width, unsigned char *frame)
{
	int x=0, y= 0;
	int demerit = 0;
	int runLength[gQrSpecWidthMax + 1];
	int length= 0;

	demerit += MaskCalcN2(width, frame);

	for(y=0; y<width; y++) 
	{
		length = MaskCalcRunLength(width, frame + y * width, 0, runLength);
		demerit += MaskCalcN1N3(length, runLength);
	}

	for(x=0; x<width; x++) 
	{
		length = MaskCalcRunLength(width, frame + x, 1, runLength);
		demerit += MaskCalcN1N3(length, runLength);
	}

	return demerit;
}


void CQRCode::MaskProcess(void)
{
	int i= 0;
	unsigned char *mask= NULL, *bestMask=NULL;
	int minDemerit = QR_INT_MAX;
	int blacks =0;
	int bratio=0;
	int demerit = 0;
    int width = 0;
	unsigned char *tmpMask = NULL;
	unsigned char *frame= NULL;
	EQRErrorLevel level = QR_ECLEVEL_L;

	int w2= 0;
    
	if(m_pFrame == NULL)
	{
		return;
	}

	frame = m_pFrame;
	width = m_width;
	level = (EQRErrorLevel)m_ecLevel;
    w2 = width * width;

	mask = (unsigned char *)malloc(w2);
	if(mask == NULL) 
	{
		return;
	}
	memset(mask,0,w2*sizeof(unsigned char));

	for(i=0; i < gMaskNum; i++) 
	{
		//n1 = n2 = n3 = n4 = 0;
		demerit = 0;
		tmpMask = NULL;
		tmpMask = mask;
		blacks = MakeMask(i,width, frame, tmpMask);
		blacks += PutFormatInfo(mask,i);
		bratio = (200 * blacks + w2) / w2 / 2; /* (int)(100*blacks/w2+0.5) */
		demerit = (abs(bratio - 50) / 5) * MaskN4;
		//n4 = demerit;
		demerit += MaskEvaluateSymbol(width, mask);

		//printf("(%d,%d,%d,%d)=%d\n", n1, n2, n3 ,n4, demerit);
		if(demerit < minDemerit) 
		{
			minDemerit = demerit;
			if(bestMask != NULL)
			{
				free(bestMask);
			}
			bestMask = NULL;
			bestMask = mask;
			mask = (unsigned char *)malloc(w2);
			if(mask == NULL) 
			{
				break;
			}
		}
	}
    tmpMask = NULL;
    if(mask != NULL)
	{
		free(mask);
	}
    mask = NULL;

   if(m_pFrame != NULL)
   {
	   free(m_pFrame);
   }
   m_pFrame = NULL;

   m_pFrame=bestMask;//get the best mask picture
   bestMask = NULL;
}


/******************************************************************************
* Data conversion
*****************************************************************************/
/**
* Convert the input data in the data chunk to a bit stream.
* @param entry
* @return number of bits (>0) or -1 for failure.
*/
int CQRCode::InputEncodeBitStream(QRinput_List *entry, int version, int mqr)
{
	int words, ret;
	QRinput_List *st1 = NULL, *st2 = NULL;
    
	if(entry == NULL)
	{
		return -1;
	}

	if(entry->bstream != NULL) 
	{
		FreeInputBitStream(entry->bstream);
		entry->bstream = NULL;
	}

	words = GetSpecMaximumWords(entry->mode, version);
	if(words != 0 && entry->size > words) 
	{

	} 
	else 
	{
		ret = 0;
		switch(entry->mode) 
		{
			case QR_MODE_NUM:
				ret = InputEncodeModeNum(entry, version, mqr);
				break;
			case QR_MODE_AN:
				ret = InputEncodeModeAn(entry, version, mqr);
				break;
			case QR_MODE_8:
				ret = InputEncodeMode8(entry, version, mqr);
				break;
			case QR_MODE_KANJI:
				
				break;
			case QR_MODE_STRUCTURE:
				
				break;
			case QR_MODE_ECI:
				
				break;
			case QR_MODE_FNC1SECOND:
				
				break;
			default:
				break;
		}

		if(ret < 0) 
		{
			return -1;
		}
	}

	return entry->bstream->length;

}


/**
* Convert the number data to a bit stream.
* @param entry
* @param mqr
* @retval 0 success
* @retval -1 an error occurred and errno is set to indeicate the error.
*            See Execptions for the details.
* @throw ENOMEM unable to allocate memory.
*/
int CQRCode::InputEncodeModeNum(QRinput_List *entry, int version, int mqr)
{
	int words, i, ret;
	unsigned int val;
    

	if(entry == NULL)
	{
		return -1;
	}

	entry->bstream = BitStreamNew();
	if(entry->bstream == NULL)
	{ 
		return -1;
	}

	if(mqr) 
	{

	} 
	else 
	{
		ret = BitStreamAppendNum(entry->bstream, 4, QrspecModeIDNum);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}

		ret = BitStreamAppendNum(entry->bstream, GetSpeclengthIndicator(QR_MODE_NUM, version), entry->size);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	}

	words = entry->size / 3;
	for(i=0; i<words; i++) 
	{
		val  = (entry->data[i*3  ] - '0') * 100;
		val += (entry->data[i*3+1] - '0') * 10;
		val += (entry->data[i*3+2] - '0');

		ret = BitStreamAppendNum(entry->bstream, 10, val);
		if(ret < 0)
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	}

	if(entry->size - words * 3 == 1) 
	{
		val = entry->data[words*3] - '0';
		ret = BitStreamAppendNum(entry->bstream, 4, val);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	} 
	else if(entry->size - words * 3 == 2) 
	{
		val  = (entry->data[words*3  ] - '0') * 10;
		val += (entry->data[words*3+1] - '0');
		BitStreamAppendNum(entry->bstream, 7, val);
		if(ret < 0)
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	}

	return 0;
}

/**
* Convert the alphabet-numeric data to a bit stream.
* @param entry
* @param mqr
* @retval 0 success
* @retval -1 an error occurred and errno is set to indeicate the error.
*            See Execptions for the details.
* @throw ENOMEM unable to allocate memory.
* @throw EINVAL invalid version.
*/
int CQRCode::InputEncodeModeAn(QRinput_List *entry, int version, int mqr)
{
	int words, i, ret;
	unsigned int val;
    
	if(entry == NULL)
	{
		return -1;
	}

	entry->bstream = BitStreamNew();
	if(entry->bstream == NULL) 
	{
		return -1;
	}

	if(mqr) 
	{

	} 
	else 
	{
		ret = BitStreamAppendNum(entry->bstream, 4, QrspecModeIDAn);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
		ret = BitStreamAppendNum(entry->bstream, GetSpeclengthIndicator(QR_MODE_AN, version), entry->size);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	}

	words = entry->size / 2;
	for(i=0; i<words; i++) 
	{
		val  = (unsigned int)InputLookAnTable(entry->data[i*2  ]) * 45;
		val += (unsigned int)InputLookAnTable(entry->data[i*2+1]);

		ret = BitStreamAppendNum(entry->bstream, 11, val);
		if(ret < 0)
		{ 
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	}

	if(entry->size & 1) 
	{
		val = (unsigned int)InputLookAnTable(entry->data[words * 2]);

		ret = BitStreamAppendNum(entry->bstream, 6, val);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	}

	return 0;

}

/**
* Convert the 8bits data to a bit stream.
* @param entry
* @param mqr
* @retval 0 success
* @retval -1 an error occurred and errno is set to indeicate the error.
*            See Execptions for the details.
* @throw ENOMEM unable to allocate memory.
*/
int CQRCode::InputEncodeMode8(QRinput_List *entry, int version, int mqr)
{
	int ret;
    
	if(entry == NULL)
	{
		return -1;
	}

	entry->bstream = BitStreamNew();
	if(entry->bstream == NULL) 
	{
		return -1;
	}

	if(mqr) 
	{

	} 
	else 
	{
		ret = BitStreamAppendNum(entry->bstream, 4, QrspecModeID8);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
		ret = BitStreamAppendNum(entry->bstream, GetSpeclengthIndicator(QR_MODE_8, version), entry->size);
		if(ret < 0) 
		{
			FreeInputBitStream(entry->bstream);
			entry->bstream = NULL;
			return -1;
		}
	}

	ret = BitStreamAppendBytes(entry->bstream, entry->size, entry->data);
	if(ret < 0) 
	{
		FreeInputBitStream(entry->bstream);
		entry->bstream = NULL;
		return -1;
	}

	return 0;
}
