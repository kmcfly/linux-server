//
//  StringEncrypt.cpp
//  LHLSPlayerDemo
//
//  Created by chengweiqi on 15-3-5.
//  Copyright (c) 2015年 chengweiqi. All rights reserved.
//

#include "StringEncrypt.h"


unsigned char StringEncrypt::DefaultKey[]={5,4,1,2,3};

char StringEncrypt::MakecodeChar(char c, unsigned char key)
{
    return c=c^key;
}

char StringEncrypt::CutcodeChar(char c, unsigned char key)
{
    return c^key;
}

void StringEncrypt::Makecode(char *pstr, unsigned char *pkey)
{
    int len=strlen(pstr);//获取长度
    for(int i=0;i<len;i++)
        *(pstr+i)=MakecodeChar(*(pstr+i),pkey[i%5]);
}

void StringEncrypt::Cutcode(char *pstr, unsigned char *pkey)
{
    int len=strlen(pstr);
    for(int i=0;i<len;i++)
        *(pstr+i)=CutcodeChar(*(pstr+i),pkey[i%5]);
}
