#ifndef __QR_CODE_H__
#define __QR_CODE_H__
#include "QRCodeStruc.h"

typedef struct QR_CODE_OUTPUT_DATA
{
   int width;
   unsigned char *dataMatrix;
}QrCodeOutputData;

class CQRCode
{
public:
  CQRCode();
  ~CQRCode();

  //get QR code matrix data
  bool GetQRCodeMatrixData(unsigned char *str,int strSize,QrCodeOutputData *outParam);
  void FreeQrCodeMatrix(QrCodeOutputData *outParam);

   //input process
  void InputInit(void);
  void InputFree(void);
  int  InputAppend(EQREncodeMode mode, int size, const unsigned char *data);
  int  InputSetErrorCorrectionLevel(EQRErrorLevel level);
  
  //frame generator
  bool InitQRFrame(void);
  void FillQRFrame(void);
  int  GetQRVersion(void);
  int  GetFrameWidth(void);
  void GetQRFrame(QrCodeOutputData *outParam);
  
  //encode
  void Encode(void);
  void EcEncode();

  //mask process
  void Mask();
  void MaskProcess(void);

  //inline function
  inline int InputLookAnTable(char c) {return ((c & 0x80)?-1:QrInputAnTable[(int)c]);}
  inline int GetRsBlockNum(void){return m_eccSpec[0] + m_eccSpec[3];}
  inline int GetRsBlockNum1(void) {return m_eccSpec[0];}
  inline int GetRsDataCodes1(void) {return m_eccSpec[1];}
  inline int GetRsEccCodes1(void)  {return m_eccSpec[2];}
  inline int GetRsBlockNum2(void) {return m_eccSpec[3];}
  inline int GetRsDataCodes2(void) {return m_eccSpec[4];}
  inline int GetRsEccCodes2(void)  {return m_eccSpec[2];}
  inline int GetRsDataLength(void) {return GetRsBlockNum1()*GetRsDataCodes1() + GetRsBlockNum2()*GetRsDataCodes2();}
  inline int GetRsEccLength(void) {return GetRsBlockNum()*GetRsEccCodes1();}
protected:
	//spec
	unsigned int GetSpecFormatInfo(int mask, EQRErrorLevel level);
	int GetSpeclengthIndicator(EQREncodeMode mode, int version);
	int GetSpecMinimumVersion(int size, EQRErrorLevel level);
	void GetSpecEccSpec(int version, EQRErrorLevel level);
	int  GetSpecDataLength(int version,int eccLevel);
	int  GetSpecEccLength(int version,int eccLevel);
	int  GetSpecRemainder(int version);
	int GetSpecMaximumWords(EQREncodeMode mode, int version);

	//put format information
   void PutFinderPattern(int x,int y);
   void PutSeparator(void);
   void SetMaskAreaInfo(void);
   void PutTimingPattern(void);
   void PutAlignmentPattern(void);
   void PutVersionInfo();
   int PutFormatInfo(unsigned char *frame,int mask); 

   void PutAlignmentMarker(int centerX,int centerY);
   //version and format information
   unsigned int GetVersionPattern(int version);
   unsigned int GetFormatInfo(int mask, EQRErrorLevel level);
   
   //input data process
   QRinput_List *InputListNewEntry(EQREncodeMode mode, int size, const unsigned char *data);
   void InputAppendEntry(QRinput_List *entry);
   int InputCheck(EQREncodeMode mode, int size, const unsigned char *data);
   int InputCheckModeNum(int size, const char *data);
   int InputCheckModeAn(int size, const char *data);
   int InputCheckModeKanji(int size, const unsigned char *data);

   int InputGetVersion(QRInputData *input);
   int InputSetVersion(QRInputData *input, int version);
   int InputEstimateVersion(QRInputData *input);
   int InputEstimateBitStreamSize(QRInputData *input, int version);
   int InputEstimateBitStreamSizeOfEntry(QRinput_List *entry, int version, int mqr);
   int InputEstimateBitsModeNum(int size);
   int InputEstimateBitsModeAn(int size);
   int InputEstimateBitsMode8(int size);
   int InputEstimateBitsModeKanji(int size);
   int InputIsSplittableMode(EQREncodeMode mode);
   int InputAppendPaddingBit(BitStream *bstream, QRInputData *input);

    //Reed-Solomon control block 
   int InitRs(RSblock *blocks, unsigned char *data, unsigned char *ecc);
   void InitRsBlock(RSblock *block, int dl, unsigned char *data, int el, unsigned char *ecc, QR_RS *rs);
   //error correct operation
   int Modnn(QR_RS *rs, int x);
   void CheckRS(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad);
   QR_RS *InitRsChar(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad);
   void FreeRsChar(QR_RS *rs);
   void FreeRsCache(void);

   void EncodeRsChar(QR_RS *rs, const unsigned char *data, unsigned char *parity);
  
   //input and raw process
   void FreeRrawData(QRRawCode *raw);

   //input bitstream, data block
	unsigned char *GetInputByteStream(QRInputData *input);
    BitStream *BitStreamNew(void);
	BitStream *MergeBitStream(QRInputData *input);
	int BitStreamAppend(BitStream *bstream, BitStream *arg);
	int BitStreamAllocate(BitStream *bstream, int length);
	BitStream *GetInputBitStream(QRInputData *input);
    unsigned char *InputBitStreamToByte(BitStream *bstream);
    void FreeInputBitStream(BitStream *bstream);

	//bitsream process
	int BitStreamAppendNum(BitStream *bstream, int bits, unsigned int num);
	BitStream *BitStreamNewFromNum(int bits, unsigned int num);
	int BitStreamAppendBytes(BitStream *bstream, int size, unsigned char *data);
	BitStream *BitStreamNewFromBytes(int size, unsigned char *data);

	//convert data to bits
	int CreateBitStream(QRInputData *input); //convert data to binary interface
	int ConvertDataToBits(QRInputData *input);

	//Fill frame
	FrameFiller *InitFrameFiller(int width, unsigned char *frame, int mqr);
	unsigned char *FrameFillerNext(FrameFiller *filler);
	//Get raw code
	unsigned char GetQrRawCode(void);
	
	//mask process
	int MakeMask(int maskNum,int width, const unsigned char *s, unsigned char *d);
	int MaskEvaluateSymbol(int width, unsigned char *frame);
	int MaskCalcN2(int width, unsigned char *frame);
	int MaskCalcRunLength(int width, unsigned char *frame, int dir, int *runLength);
	int MaskCalcN1N3(int length, int *runLength);

	//Data conversion
   int InputEncodeBitStream(QRinput_List *entry, int version, int mqr);
   int InputEncodeModeNum(QRinput_List *entry, int version, int mqr);
   int InputEncodeModeAn(QRinput_List *entry, int version, int mqr);
   int InputEncodeMode8(QRinput_List *entry, int version, int mqr);
private:
   int				m_qrVersion; // QR code version ,from 1 to 40
   unsigned int		m_ecLevel; //the error correct level,refer to EQRErrorLevel
   int				m_width;   //QR matrix width,the min is 27x27,it is will add 4 follow the m_qrVersion.

   unsigned char	*m_pFrame;

   //error correct
   QR_RS *m_pRsList;
   QR_RS *m_pCurrRs;

   //raw data 
   QRInputData *m_pInputData;
   QRRawCode *m_pRawData;

   //
   int m_eccSpec[5];

   //mask indicator
   int m_maskNum;
   
};
#endif



