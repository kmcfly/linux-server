/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2013-08-09
** Version      : 1.0
** Description  : 解析h264的序列参数集
** Modify Record:
1:
***********************************************************************/

#ifndef __H264_PARSER__
#define __H264_PARSER__

//指数哥伦比亚码解码,返回值为编码的长度和，即Des下一次解码应该进行的偏移量,单位为bit
int H264_Ue_Decode(bool *pBinaryArray, int &Des);

int H264_Se_Decode(bool *pBinaryArray, int &Des);

unsigned long Scaling_list (int sizeOfScalingList, bool *pBinaryArray);

int ParseSPS(unsigned char *pSrc, unsigned long SrcLen, unsigned short &VideoWidth, unsigned short &VideoHeight);


#endif
