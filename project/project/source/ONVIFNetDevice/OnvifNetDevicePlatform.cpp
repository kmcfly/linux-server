#include "NetDevicePlatform.h"

#include "OnvifDevDiscoverServ.h"

#include "OnvifNetDeviceServ.h"

#include "ClientRTPMain.h"

#include "OnvifEvent.h"

#include "StreamDeamon.h"

#ifdef __cplusplus
extern "C"
{
#endif

//初始化, 退出库
int nd_init()
{

    COnvifDevDiscover::Instance()->StartSearch();
    CClientRTPMain::Instance()->Start();
	CStreamDeamon::Instance()->Start();

#ifdef ONVIF_VER40_ALARM_EVENT
	COnvifEvent::EventInstance()->EventOpen();
#endif

#ifdef ONVIF_SNAP_ACTIVE  
	COnvifSnapJpeg::Instance()->Start(); //the thread catch picture
#endif

	return 0;
}

int	nd_quit()
{
	CStreamDeamon::Instance()->Stop();
	COnvifDevDiscover::Instance()->StopSearch();
	CClientRTPMain::Instance()->Stop();

#ifdef ONVIF_SNAP_ACTIVE  
	COnvifSnapJpeg::Instance()->Stop();
#endif

#ifdef ONVIF_VER40_ALARM_EVENT
	COnvifEvent::EventInstance()->EventClose();
#endif

	return 0;
}

//获取库信息,成功返回0
int	nd_get_info(nd_out ND_LIB_INFO* p_lib_info)
{
   
	if(p_lib_info == NULL)
	{
		return -1;
	}
	
	p_lib_info->manufacturerId = MID_ONVIF;
	strcpy(p_lib_info->version,"ONVIF 2.1");
	p_lib_info->bNeedHttpPort = false;
	p_lib_info->bNeedCtrlPort = false;

	return 0;
}

//搜索局域网设备，成功返回当前搜索到的设备数目
int	nd_get_discovered_device_num()
{
    int devNum = 0;

	devNum = COnvifDevDiscover::Instance()->GetDeviceNum();

	return devNum;
}

static int get_http_port(char *servAddr)
{
	int port = 0;
    char *tmp1= NULL;
	//char *tmp2 = NULL;
	char buf[10] = {'\0'};

	tmp1 = servAddr;
	tmp1 = strstr(tmp1,":");
	if(tmp1 != NULL)
	{
		tmp1 += 1;
     
		if(tmp1 != NULL)
		{
			strcpy(buf,tmp1);
		   port = atoi(buf);
		}
	}
	return port;
}

//获取搜索到的设备信息，成功返回设备数目
int	nd_get_discovered_device_info(nd_out ND_DEVICE_INFO* p_device_info, nd_in int deviceNum)
{
   int tDevNum = 0; 
   int i = 0;
   int httpPort = 0;
   ND_DEVICE_INFO* tmpDev=NULL;
   char *tmp1 =NULL;
   char *tmp2 =NULL;
   char addrBuf[64] = {0};
   bool bGetSerial = false;

   if(deviceNum == 0 || p_device_info == NULL)
   {
	   COnvifDevDiscover::Instance()->GetDeviceInfo(NULL,0, false );
	   return 0;
   }
   else if( 0xffff == deviceNum && (0 == strcmp(p_device_info->productModel, "5a5a5a5a")) )
   {
	   // 获取IPC的厂家名称
	   // 因为接口是统一的，增加接口比较麻烦，所以使用同一接口，根据参数不一样，获取不同的功能。
	   memset( addrBuf, 0, 64 );
	   strcpy( addrBuf, p_device_info->szAddr );
	   printf( "nd_get_discovered_device_info 0x5a5a5a5a = %s,%s,%d\n", addrBuf, __FILE__, __LINE__ );
	   int prod_id = COnvifDevDiscover::Instance()->GetIPCDevviceSerial( addrBuf, NULL, NULL, 5, bGetSerial );
	   p_device_info->netmask = prod_id;
	   return 0;
   }
   else if( 0xffff == deviceNum && (0 == strcmp(p_device_info->productModel, "5b5b5b5b")) )
   {
	   // 设置当前设备poe通道数目
	   // 因为接口是统一的，增加接口比较麻烦，所以使用同一接口，根据参数不一样，获取不同的功能。
	   COnvifDevDiscover::Instance()->SetCurDevicePoeChnnCnt( p_device_info->gateway );

	   return 0;

   }

   //if(OnvifDevDiscover != NULL)
   {
	   tDevNum = COnvifDevDiscover::Instance()->GetDeviceInfo(p_device_info,deviceNum, false);
		tmpDev= p_device_info;

		while( i < tDevNum)
		{
			httpPort = get_http_port(tmpDev->szAddr);
			tmpDev->netmask = 0;
			tmpDev->gateway = 0;
			if(httpPort == 0)
			{
				tmpDev->netport  = 80;
				tmpDev->nHttpPort = 0;
			}
			else
			{
				tmpDev->netport  = httpPort;
				tmpDev->nHttpPort = httpPort;
				tmp1 = tmpDev->szAddr;
				tmp2 = strstr(tmpDev->szAddr,":");
				if(tmp2 != NULL)
				{
                  memset(addrBuf,0,sizeof(addrBuf));
					strncpy(addrBuf,tmp1,tmp2-tmp1);
				    strcpy(tmpDev->szAddr,addrBuf);
				}
			}

			tmpDev->nCtrlPort = 0; 
			tmpDev->firmwareVersion[0] = '\0';
			tmpDev->manufacturerId = MID_ONVIF;
			strcpy(tmpDev->manufacturerName, "ONVIF");
			char manu_id_val = tmpDev->productModel[35];
			memset( tmpDev->productModel, 0, sizeof(tmpDev->productModel));
			strcpy(tmpDev->productModel,"ONVIF");
			tmpDev->productModel[35] = manu_id_val;
			tmpDev++;
			i++;
		}
   }

	return tDevNum;
}

//定时处理
void nd_on_timer(nd_in unsigned long current_time)
{

}

//创建, 释放 接口对象
CNetDevice* nd_create()
{
    COnvifNetDevice *pOnvifDev = NULL;
	pOnvifDev = new COnvifNetDevice();

	CStreamDeamon::Instance()->AddDevice(pOnvifDev);

	return pOnvifDev;
}

void nd_destroy(nd_in CNetDevice* p_net_device)
{
    if(p_net_device != NULL)
	{
		CStreamDeamon::Instance()->DelDevice(reinterpret_cast<COnvifNetDevice *>(p_net_device));
		delete p_net_device;
	}
	p_net_device = NULL;    
}


#ifdef __cplusplus
}
#endif