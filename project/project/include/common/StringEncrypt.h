//
//  StringEncrypt.h
//  LHLSPlayerDemo
//
//  Created by chengweiqi on 15-3-5.
//  Copyright (c) 2015�?chengweiqi. All rights reserved.
//

#ifndef __LHLSPlayerDemo__StringEncrypt__
#define __LHLSPlayerDemo__StringEncrypt__

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

class StringEncrypt
{
  
public:
    static unsigned char DefaultKey[];
    
    //加密字符�?    
	static void Makecode(char *pstr,unsigned char *pkey);
    //解密字符�?    
	static void Cutcode(char *pstr,unsigned char *pkey);
private:
    static char MakecodeChar(char c,unsigned char key);
    static char CutcodeChar(char c,unsigned char key);
};


#endif /* defined(__LHLSPlayerDemo__StringEncrypt__) */
