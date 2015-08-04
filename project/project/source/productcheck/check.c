
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "Version.h"

//���ļ��������ǣ����������������������У���ļ�
/*У���ļ��ĸ�ʽ
DvrPackID=X\0\0\0\0\0\0\0\0\0\0\0\0-----\0    //��80���ֽ�
DvrSoftVersionM=X.X.X\0\0\0\0\0\0\0\0---\0    //��80���ֽ�
DvrSoftVerLevel=X\0\0\0\0\0\0\0\0\0-----\0    //��80���ֽ�
LevelNumEx=X\0\0\0\0\0\0\0\0\0----------\0    //��80���ֽ�
DvrSoftVerLevelEx=X\0\0\0\0\0\0\0\0\0---\0    //��80���ֽ�
BCheckSoftLevel=X\0\0\0\0\0\0\0\0\0-----\0    //��80���ֽ�
BHighVerToThis=X\0\0\0\0\0\0\0\0\0\0----\0    //��80���ֽ�
BCheckLowVer=0\0\0\0\0\0\0\0\0\0\0\0----\0    //��80���ֽ�
StartLowVersion=X.X.X\0\0\0\0\0\0\0-----\0    //��80���ֽ�
*/

//����Ϣ��DvrPackID=X������Version.h�еõ�
//����汾�ţ�DvrSoftVersionM=X.X.X������Version.h�еõ�
//����ļ���DvrSoftVerLevel=X������Version.h�еõ�

//����Ƿ�֧����չ���𡣼���������Ƿ����װ���������İ汾�ϡ�LevelNum=0,��ʾ��֧�֡�
//���ڵ���1��ʾ֧�֡���Ҫ�ֶ��޸Ĵ˲���.
//���֧����չ���𣬻�Ҫ���ֶ��޸�DVR_LEVEL_EX����д��֧����Щ����
//levelNumEx=2
//DvrSoftVerLevel=P,U
#define LEVEL_NUM_EX  "LevelNumEx=0"
#define DVR_LEVEL_EX  "DvrSoftVerLevelEx=T,C,B,P,U"

//�Ƿ������������Ҫ�ֶ��޸ĺ�B_CHECK_SOFT_LEVEL.1�����顣0�������
#define B_CHECK_SOFT_LEVEL  "BCheckSoftLevel=1"

//�Ƿ�����������װ���汾�Žϸߵ�����ϡ�����˵,������İ汾��3.0.5�� ���������������
//����汾��Ϊ3.0.0�����BHighVerToThis=1���ͱ�ʾ����3.0.0����3.0.5��
//��Ҫ�ֶ��޸ĺ�B_HIGH_VER_TO_THIS.1��������0��������
#define B_HIGH_VER_TO_THIS  "BHighVerToThis=1"

//�Ƿ�����԰�װ�������������С�汾�š�����˵���������������������汾��Ϊ3.0.8��
//���BCheckLowVer=1 StartLowVersion=3.0.5������˵ֻ�л���������汾��Ϊ3.0.5����������3.0.8���������ģ�
//�ſ��������˰�
#define B_CHECK_LOW_VER     "BCheckLowVer=0"
#define START_LOW_VERSION   "StartLowVersion=0.0.0"



int ProductCheck(FILE *pFile)
{
	char *p = NULL;
	int i =0;

	//д����Ϣ
	char CheckDvrPackID[80] = {0};
	strcpy(CheckDvrPackID, DVR_PACK_ID);
	if (80 != fwrite(CheckDvrPackID, sizeof(char), 80, pFile))
	{
		return 1;
	}

	//д����汾��
	char CheckDvrSoftVersionM[80] = {0};
	strcpy(CheckDvrSoftVersionM, DVR_SOFT_VERSION_M);
	if (80 != fwrite(CheckDvrSoftVersionM, sizeof(char), 80, pFile))
	{		
		return 1;
	}

	//д����ļ���
	char CheckDvrSoftVerLevel[80] = {0};
	strcpy(CheckDvrSoftVerLevel, DVR_SOFT_LEVEL);
	if (80 != fwrite(CheckDvrSoftVerLevel, sizeof(char), 80, pFile))
	{		
		return 1;
	}

	//д��չ�������
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

	//д��չ����
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
	//strtok�������޸�CheckDvrSoftVerLevelEx�е�����
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

	//д�Ƿ����������
	char CheckBCheckSoftLevel[80] = {0};
	strcpy(CheckBCheckSoftLevel, B_CHECK_SOFT_LEVEL);
	if (80 != fwrite(CheckBCheckSoftLevel, sizeof(char), 80, pFile))
	{		
		return 1;
	}

	//д�Ƿ�����������װ���汾�Žϸߵ������
	char CheckBHighVerToThis[80] = {0};
	strcpy(CheckBHighVerToThis, B_HIGH_VER_TO_THIS);
	if (80 != fwrite(CheckBHighVerToThis, sizeof(char), 80, pFile))
	{
		return 1;
	}

	//д�Ƿ�����԰�װ�������������С�汾��
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
	//��������ķ���ֵ����������õ���
	//�������0������ɹ�����0����ʧ�ܡ�
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

