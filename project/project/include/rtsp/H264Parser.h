/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2013-08-09
** Version      : 1.0
** Description  : ����h264�����в�����
** Modify Record:
1:
***********************************************************************/

#ifndef __H264_PARSER__
#define __H264_PARSER__

//ָ�����ױ��������,����ֵΪ����ĳ��Ⱥͣ���Des��һ�ν���Ӧ�ý��е�ƫ����,��λΪbit
int H264_Ue_Decode(bool *pBinaryArray, int &Des);

int H264_Se_Decode(bool *pBinaryArray, int &Des);

unsigned long Scaling_list (int sizeOfScalingList, bool *pBinaryArray);

int ParseSPS(unsigned char *pSrc, unsigned long SrcLen, unsigned short &VideoWidth, unsigned short &VideoHeight);


#endif
