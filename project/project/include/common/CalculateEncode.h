/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-01-05
* Name         : EncodeCalculate.h
* Version      : 1.0
* Description  : 

* Modify Record:

*/


#ifndef ___ENCODE__CALCULATE__H___
#define ___ENCODE__CALCULATE__H___

#include "dvrdvsdef.h"


//�������㷽ʽ�Ļ���
class CCalculateEncode
{
public:
	virtual ~CCalculateEncode(){};	

	//��Ҫ��������� �� ���ݵļ��㷽ʽ����
	virtual bool CheckEncodeParam(	unsigned long ulChnn,
									CHECK_ENC_INFO * pCheckEncInfo)
									= 0;

};

#endif
