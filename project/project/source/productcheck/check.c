
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "Version.h"

//此文件的作用是：制作此软件所在升级包的校验文件
/*校验文件的格式
DvrPackID=X\0\0\0\0\0\0\0\0\0\0\0\0-----\0    //共80个字节
DvrSoftVersionM=X.X.X\0\0\0\0\0\0\0\0---\0    //共80个字节
DvrSoftVerLevel=X\0\0\0\0\0\0\0\0\0-----\0    //共80个字节
LevelNumEx=X\0\0\0\0\0\0\0\0\0----------\0    //共80个字节
DvrSoftVerLevelEx=X\0\0\0\0\0\0\0\0\0---\0    //共80个字节
BCheckSoftLevel=X\0\0\0\0\0\0\0\0\0-----\0    //共80个字节
BHighVerToThis=X\0\0\0\0\0\0\0\0\0\0----\0    //共80个字节
BCheckLowVer=0\0\0\0\0\0\0\0\0\0\0\0----\0    //共80个字节
StartLowVersion=X.X.X\0\0\0\0\0\0\0-----\0    //共80个字节
*/

//包信息（DvrPackID=X），从Version.h中得到
//软件版本号（DvrSoftVersionM=X.X.X），从Version.h中得到
//软件的级别（DvrSoftVerLevel=X），从Version.h中得到

//软件是否支持扩展级别。即此软件，是否可以装到多个级别的版本上。LevelNum=0,表示不支持。
//大于等于1表示支持。需要手动修改此参数.
//如果支持扩展级别，还要把手动修改DVR_LEVEL_EX，即写出支持哪些级别。
//levelNumEx=2
//DvrSoftVerLevel=P,U
#define LEVEL_NUM_EX  "LevelNumEx=0"
#define DVR_LEVEL_EX  "DvrSoftVerLevelEx=T,C,B,P,U"

//是否检查软件级别。需要手动修改宏B_CHECK_SOFT_LEVEL.1代表检查。0代表不检查
#define B_CHECK_SOFT_LEVEL  "BCheckSoftLevel=1"

//是否允许本升级包装到版本号较高的软件上。就是说,机器里的版本号3.0.5， 而此这个升级包的
//软件版本号为3.0.0，如果BHighVerToThis=1，就表示允许3.0.0升到3.0.5；
//需要手动修改宏B_HIGH_VER_TO_THIS.1代表允许。0代表不允许。
#define B_HIGH_VER_TO_THIS  "BHighVerToThis=1"

//是否检查可以安装这个升级包的最小版本号。就是说，比如这个升级包的软件版本号为3.0.8，
//如果BCheckLowVer=1 StartLowVersion=3.0.5，就是说只有机器里软件版本号为3.0.5（包含）到3.0.8（包含）的，
//才可以升级此包
#define B_CHECK_LOW_VER     "BCheckLowVer=0"
#define START_LOW_VERSION   "StartLowVersion=0.0.0"



int ProductCheck(FILE *pFile)
{
	char *p = NULL;
	int i =0;

	//写包信息
	char CheckDvrPackID[80] = {0};
	strcpy(CheckDvrPackID, DVR_PACK_ID);
	if (80 != fwrite(CheckDvrPackID, sizeof(char), 80, pFile))
	{
		return 1;
	}

	//写软件版本号
	char CheckDvrSoftVersionM[80] = {0};
	strcpy(CheckDvrSoftVersionM, DVR_SOFT_VERSION_M);
	if (80 != fwrite(CheckDvrSoftVersionM, sizeof(char), 80, pFile))
	{		
		return 1;
	}

	//写软件的级别
	char CheckDvrSoftVerLevel[80] = {0};
	strcpy(CheckDvrSoftVerLevel, DVR_SOFT_LEVEL);
	if (80 != fwrite(CheckDvrSoftVerLevel, sizeof(char), 80, pFile))
	{		
		return 1;
	}

	//写扩展级别个数
	char CheckSzLevelNumEx[80] = {0};
	strcpy(CheckSzLevelNumEx, LEVEL_NUM_EX);
	if (80 != fwrite(CheckSzLevelNumEx, sizeof(char), 80, pFile))
	{
		return 1;
	}
	p = NULL;
	p = strstr(CheckSzLevelNumEx, "=");
	if (NULL == p)
	{
		return 1;
	}
	const char temp = *(p+1);
	int CheckLevelNumEx = atoi(&temp);
    if ((CheckLevelNumEx < 0) || (CheckLevelNumEx >= 10))
	{
		return 1;
	}

	//写扩展级别
	char CheckDvrSoftVerLevelEx[80] = {0};
	strcpy(CheckDvrSoftVerLevelEx, DVR_LEVEL_EX);
	if (CheckLevelNumEx > 0)
	{
		p = NULL;
		p = strstr(CheckDvrSoftVerLevelEx, "=");
		if (NULL == p)
		{
			return 1;
		}
		if (strlen(p+1) <= 0)
		{
			return 1;
		}
		int count = 0;
		char *ps = strtok(p+1, ",");
		while(ps != NULL)
		{
			count++;
			ps = strtok(NULL, ",");
		}
		if (count != CheckLevelNumEx)
		{
			return 1;
		}
	}
	//strtok函数会修改CheckDvrSoftVerLevelEx中的内容
	memset(CheckDvrSoftVerLevelEx, 0, sizeof(CheckDvrSoftVerLevelEx));
	strcpy(CheckDvrSoftVerLevelEx, DVR_LEVEL_EX);
	if (80 != fwrite(CheckDvrSoftVerLevelEx, sizeof(char), 80, pFile))
	{
		return 1;
	}
	/*typedef char LINECHAR[80];
	LINECHAR *pCheckDvrSoftVerLevel = (LINECHAR*)malloc(CheckLevelNum * 80 * sizeof(char));	
	for (i = 0; i < CheckLevelNum; ++i)
	{
		memset(pCheckDvrSoftVerLevel[i], 0, 80);
		strcpy(pCheckDvrSoftVerLevel[i], "DvrSoftVerLevel=T");
		printf("%s\n",pCheckDvrSoftVerLevel[i]);
		if (80 != fwrite(pCheckDvrSoftVerLevel[i], sizeof(char), 80, pFile))
		{
			return 1;
		}
	}*/

	//写是否检查软件级别
	char CheckBCheckSoftLevel[80] = {0};
	strcpy(CheckBCheckSoftLevel, B_CHECK_SOFT_LEVEL);
	if (80 != fwrite(CheckBCheckSoftLevel, sizeof(char), 80, pFile))
	{		
		return 1;
	}

	//写是否允许本升级包装到版本号较高的软件上
	char CheckBHighVerToThis[80] = {0};
	strcpy(CheckBHighVerToThis, B_HIGH_VER_TO_THIS);
	if (80 != fwrite(CheckBHighVerToThis, sizeof(char), 80, pFile))
	{
		return 1;
	}

	//写是否检查可以安装这个升级包的最小版本号
	char CheckBCheckLowVer[80] = {0};
	strcpy(CheckBCheckLowVer, B_CHECK_LOW_VER);
	if (80 != fwrite(CheckBCheckLowVer, sizeof(char), 80, pFile))
	{
		return 1;
	}

	//
	char CheckStartLowVersion[80] = {0};
	strcpy(CheckStartLowVersion, START_LOW_VERSION);
	if (80 != fwrite(CheckStartLowVersion, sizeof(char), 80, pFile))
	{
		return 1;
	}	
	
	return 0;	
}




int main(int argc, char** argv)
{
	//这个函数的返回值，在外面会用到。
	//如果返回0，代表成功。非0代表失败。
	int ret = 0;
	FILE *pFile = NULL;
	char szFileName[256] = {0};
	char szCmd[300] = {0};

	char *p = strrchr(argv[0], '/');
	if (NULL == p)
	{
		return 1;
	}
	*p = '\0';
	snprintf(szFileName, 256, "%s/check.dat", argv[0]);	
	snprintf(szCmd, 300, "rm -rf %s", szFileName);
	system(szCmd);
	pFile = fopen(szFileName, "wb");
	if (NULL == pFile)
	{
		return 1;
	}
	ret = ProductCheck(pFile);
	fclose(pFile);

	if (1 == ret)
	{
		printf("product check.dat error\n");
		system(szCmd);
		return 1;
	}
	else
	{
		printf("product check.dat ok\n");
		return 0;  
	}	
	
}

