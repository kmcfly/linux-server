#include "NICTypeMan.h"
#include "LocalDevice.h"

CNICTypeMan::CNICTypeMan()
{

}

CNICTypeMan::~CNICTypeMan()
{

}

bool CNICTypeMan::ChangeNICType(NIC_TYPE nicType)
{
	if (nicType < NIC_TYPE_END)
	{
		//�������ͺ�д���ļ�
		if (!WriteNICTypeToFile(nicType))
		{
			printf("%s:%s:%d, Write nic type to file %s error\n", __FUNCTION__, __FILE__, __LINE__, NIC_TYPE_FILE);
			return false;
		}

		//�������ͺ�д��flash
		char buf[7] = {0};
		snprintf(buf, sizeof(buf), "%d", nicType);
		if (!CLocalDevice::Instance()->WriteNICTypeToFlash(buf, sizeof(buf)))
		{
			printf("%s:%s:%d, Write nic type %s to flash error\n", __FUNCTION__, __FILE__, __LINE__, buf);
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool CNICTypeMan::NICTypeIsNotSame(unsigned long productID)
{
	NIC_TYPE nicTypeFlash, nicTypeFile, nicTypeReal;

	//��flash�е������ͺ�
	nicTypeFlash = (NIC_TYPE)CLocalDevice::Instance()->GetNICTypeFromFlash();

	//���ݲ�Ʒ���ͻ�ȡ��ʵ����������
	nicTypeReal = GetRealNICType(productID);

	//���ļ��е������ͺ�
	if (!ReadNICTypeFromFile(nicTypeFile) || (nicTypeFlash != nicTypeFile) || ( nicTypeReal != nicTypeFlash))
	{
		//д��ʵ�����ͺŵ�flash
		char buf[7] = {0};
		snprintf(buf, sizeof(buf), "%d", nicTypeReal);
		if (!CLocalDevice::Instance()->WriteNICTypeToFlash(buf, sizeof(buf)))
		{
			printf("%s:%s:%d, Write nic type %d to flash error\n", __FUNCTION__, __FILE__, __LINE__, nicTypeReal);
		}

		//д��ʵ�����ͺŵ�/etc/init.d/NetChip.dat�ļ���
		if (!WriteNICTypeToFile(nicTypeReal))
		{
			printf("%s:%s:%d, Write nic type %d to file %s error\n", __FUNCTION__, __FILE__, __LINE__, nicTypeReal, NIC_TYPE_FILE);
		}

		return true;
	}

	return false;
}


bool CNICTypeMan::WriteNICTypeToFile(NIC_TYPE nicType)
{
	if (nicType < NIC_TYPE_END)
	{
		FILE *pFile = fopen(NIC_TYPE_FILE, "w");

		if (NULL == pFile)
		{
			return false;
		}

		fprintf(pFile, "%d", nicType);

		fclose(pFile);
		pFile = NULL;

		return true;
	}
	else
	{
		return false;
	}
}

bool CNICTypeMan::ReadNICTypeFromFile(NIC_TYPE &nicType)
{
	FILE *pFile = fopen(NIC_TYPE_FILE, "r");

	if (NULL == pFile)
	{
		return false;
	}

	char buf[10] = {0};
	if (NULL == fgets(buf, sizeof(buf), pFile))
	{
		fclose(pFile);
		pFile = NULL;
		return false;
	}
	
	fclose(pFile);
	pFile = NULL;

	nicType = (NIC_TYPE)atoi(buf);
	if (nicType < NIC_TYPE_END)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
*���ݲ�Ʒ�ͺŻ�ȡ�������͡�
*���룺productID ��ƷID
*���:����TD_2324SE_SΪ100M����������Ĭ��ʹ��1000M��
*/
NIC_TYPE CNICTypeMan::GetRealNICType(unsigned long productID)
{
	switch (productID)
	{
	case TD_2324SE_S:
		return NIC_TYPE_H;
	case TD_2316ME:
	case TD_2508HE:
		{
			//���� TD_2316ME��ʱ�򣬸���Ӳ���汾��ȷ���ǰ�M����ǧM��
			char hardWare[7] = {0};
			CLocalDevice::GetHardWareVersion(hardWare,6);
			printf("hardWare=%s\n",hardWare);
			int tmp=-1;
			for(int i = 0;i < 6;i++)
			{
				if(isdigit(hardWare[i]))
				{
					tmp=i;					
					break;
				}
			}
			if(tmp!=-1)
			{
				char *p=&hardWare[tmp];
				float netCheck=atof(p);
				printf("netCheck=%f\n",netCheck);
				if(netCheck>=1.5)
				{
					return NIC_TYPE_H;
				}
				else
				{
					return NIC_TYPE_T;
				}
			}
			else
			{
				return NIC_TYPE_T;
			}
		}
	case TD_2516HE_B_N100:		//3531 960H,100M������û�е�Ƭ��
	case TD_2604LE:
	case TD_2608LE:
	case TD_2616LE:
	case TD_2508HE_C_3521:
	case TD_2704XS_SL:
	case TD_2708XS_SL:
	case TD_2716XS_SL:
		return NIC_TYPE_H;
	case TD_2704AS_SL:
	case TD_2704AS_SL_A:
	case TD_2708AS_SL:
	case TD_2716AS_SL:
	case TD_2704AS_S:
	case TD_2708AS_S:
	case TD_2708AS_S_A:
	case TD_2716AS_S:
	case TD_2704AS_S_A:
	case TD_2716AS_S_A:
		return NIC_TYPE_H;
	case TD_2704TS_S:
	case TD_2708TS_S:
	case TD_2716TS_S:
	case TD_2704TS_PL:
	case TD_2708TS_PL:
	case TD_2716TS_PL:
	case TD_2704TE_M:
	case TD_2708TE_M:
	case TD_2716TE_M:
	case TD_2704AE_SL:
	case TD_2708AE_SL:
	case TD_2716AE_SL:
	case TD_2004AL:
	case TD_2008AS:
	case TD_2704AE_PL:
	case TD_2708AE_PL:
	case TD_2716AE_PL:
	case TD_2704AS_PL:
	case TD_2708AS_PL:
	case TD_2716AS_PL:
		return NIC_TYPE_H;
	default:
		return NIC_TYPE_T;
	}
}

