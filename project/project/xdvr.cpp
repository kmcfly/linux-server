//xdvr.cpp
#include <stdio.h>
#include <signal.h> //信号处理所需要的头文件
#if !defined(__CHIP3520__) && !defined(__CHIP3515__) && !defined(__CHIP3531__) && !defined(__CHIP3535__) && !defined(__CHIP3521__) && !defined(__CHIP3520A__) && !defined(__CHIP3520D__) && !defined(__CHIPGM__) 
#include <execinfo.h>
#endif
#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "Product.h"
#include "ConfigEx.h"
#include "ConfigBlock.h"
#include "MainFrame.h"
#include "mylist.cpp"
#include "IPCDecDisplayMan.h"

#include <string.h>
/////////////////////////
#include "CheckUpgradeKey.h"

#include "RawToBmp.h"

#ifdef __GB28181_SERVER__
#include "Gb28181Server.h"
#include "RTPStreamDispatch.h"
#include "Gb28181Config.h"
#endif

#ifdef __TUTK_SERVER__
#include "TUTKStreamDispatch.h"
#endif

#ifdef __API_SERVER__
#include "APIStreamDispatch.h"
#include "TvtAlarm.h"
#endif

#ifdef __NETSERVER_TVT__
	#include "NetServer.h"
	#include "webserver.h"
#endif

#ifdef __DVR_ULTIMATE__
#include "NdLibMan.h"
#endif

#ifdef __NETSERVER_HXHT__
	#include "HxConnectMan.h"
#include "NetCfgAndUpMan.h"
#endif

#include "MessageMan.h"
#include "LocalUIMan.h"
#include "TZoneDZoneCfg.h"
#include "LanguagePack.h"
#include "ReadDdns.h"
#include "WndStructDef.h"
#include "FuncCustom.h"
#include "NICTypeMan.h"

#include "NetDaemon.h"

#ifdef __GB28181_SERVER__
CRTPStreamDispatch g_RTPStreamDisPatch;
#endif

#ifdef __TUTK_SERVER__
CTUTKStreamDispatch g_tutkStreamDispatch;
#endif

#ifdef __API_SERVER__
CAPIStreamDispatch *g_apiStreamDispatch = CAPIStreamDispatch::Instance();
#endif

DVR_SYSTEM_STATUS g_system_status = DVR_SYSTEM_RUNING;
VIDEO_FORMAT g_video_format = VIDEO_FORMAT_NTSC;
CRYTOINFO_STATUS g_crytoStatus = CRYTOINFO_STATUS_NULL;
bool g_bUse1104A1108ChipColor = false;
unsigned char g_coverAreaCountLimit = MAX_COVER_AREA_NUM;

extern LANGUAGE	g_language;
extern VGA_RESOLUTION g_resolution;
extern unsigned long g_timeMode;
extern unsigned long g_dateMode;
extern unsigned long g_showWizzard;
extern bool g_bChangOutDevice;
extern bool g_langRightOrder;
extern bool g_bProIdequeSubProdID;

extern bool g_b_rw_cfg;

extern WND_STARTUP_INFO* g_p_wnd_startup_info;
extern WND_STARTUP_INFO* g_p_cur_startup_info;
extern bool g_bHaveSnapPic;
unsigned int g_support_hd_type=0;
unsigned long g_IsChangeNetMode = 128;//128代表没有修改过，0代表以太网，1代表3G网络
bool g_curNet3G = false;              //当前使用的是哪种网卡
bool g_bCVBSChangeResolution = false;
unsigned long g_adjustCvbsVO = 0;
int  g_video_out_mode = 0;
CNetDaemon	netDaemon;

#if defined(__X_LINUX__)
static bool bUseUi = false;
#else
static bool bUseUi = true;
#endif

#if !defined(__CHIP3520__) && !defined(__CHIP3515__) && !defined(__CHIP3531__) && !defined(__CHIP3535__) && !defined(__CHIP3521__) && !defined(__CHIP3520A__) && !defined(__CHIP3520D__) && !defined(__CHIPGM__) 
const int SIZE = 100;
void DebugBackTrace(int)
{
	void *array [SIZE];
	int size = backtrace(array, SIZE);
	printf("\nBackTrace (%d deep):\n", size);
	char ** pStr = backtrace_symbols (array, size);
	for (int i=0; i<size; ++i)
	{
		printf("%d: %s\n", i, pStr[i]);
	}
	
	free( pStr);
	
	DVRSystem("exit");
	exit(0);
}
#endif
//判断两个地方的记录的视频制式是否一致
//返回值代表是否需要重启。
bool bRestartForCheckVideoFormat()
{
	//读取flash中的视频制式
	unsigned long videoflash;
	videoflash = CLocalDevice::GetVideoFormatFromFlash();
	printf("videoflash:%d\n", videoflash);

	bool bModifyFile = false;
	char videofile[16] = {0};
	do 
	{
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPGM__)
		FILE *pFile = fopen("/mnt/mtd/VideoFormat.dat", "rb");
#else
		FILE *pFile = fopen("/etc/init.d/VideoFormat.dat", "rb");
#endif
		if (NULL == pFile)
		{
			bModifyFile = true;
			break;
		}
		char str[80] = {0};
		fgets(str, 80, pFile);				
		fclose(pFile);
		snprintf(videofile, 16, "%s", str);
		printf("videofile:%s\n", videofile);

		if (videoflash != atoi(videofile))
		{
			bModifyFile = true;
			break;
		}

		bModifyFile = false;
	} while(0);

	if (bModifyFile)
	{
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPGM__)
		FILE * pFile = fopen("/mnt/mtd/VideoFormat.dat", "wb");
#else
		FILE * pFile = fopen("/etc/init.d/VideoFormat.dat", "wb");
#endif
		if (NULL == pFile)
		{
			return false;
		}
		char videoformat[8]={0};
		snprintf(videoformat, 8, "%d", videoflash);
		if (1 != fwrite(videoformat, 8, 1, pFile))
		{
			fclose(pFile);
			return false;
		}
		sync();
		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}

#ifdef __GB28181_SERVER__
void LoadGb28181Config()
{
	GB28181_CONFIG *pGb2818_Config = GB28181_CONFIG::Instance();

	const char *pClientName = "34020000";
	const char *pserverID="34020000002000000001";
	const char *pDvrId = "34020000001180000002";
	const char *FromPort = "5000";
	const char *ToPort = "5060";
	const char *Password = "12345678";


	const char *ToIp = "192.168.3.191";
	const char *FromIp = "192.168.3.190";


	memcpy(pGb2818_Config->m_wSipserverIP,ToIp,strlen(ToIp));
	memcpy(pGb2818_Config->m_SipServerId,pserverID,strlen(pserverID));

	memcpy(pGb2818_Config->m_Dvrip,FromIp,strlen(FromIp));
	memcpy(pGb2818_Config->m_DvrId,pDvrId,strlen(pDvrId));

	memcpy(pGb2818_Config->m_UserID,pDvrId,strlen(pDvrId));
	memcpy(pGb2818_Config->m_RegisterPasswd,Password,strlen(Password));

	pGb2818_Config->m_wSipserverPort = atoi(ToPort);
	pGb2818_Config->m_wSipDvrPort    = atoi(FromPort);
	pGb2818_Config->m_expires      = 3600;

	//test begin

	pGb2818_Config->InsertData("34020000001180000003",1);
	pGb2818_Config->InsertData("34020000001310000053",0);

	pGb2818_Config->InsertAlarmData("34020000001340000001",0);

	CCatalogItem *p= new  CCatalogItem;

	strcpy(p->m_szDeviceID,"123");
	strcpy(p->m_szName,"ipc1");
	strcpy(p->m_szManufacturer,"tongwei");
	strcpy(p->m_szModel,"TD-2316ME");
	strcpy(p->m_szParentID,"12134");
	strcpy(p->m_szOwner,"xxx");
	strcpy(p->m_szCivilCode,"xxx");
	strcpy(p->m_szAddress,"xxx");
	strcpy(p->m_szParental,"0");
	strcpy(p->m_szRegisterWay,"1");

	p->m_bSecrecy =0;
	p->m_bStatus  =1;
	pGb2818_Config->m_deqItem.push_back(p);
	
	CDeviceStatusItem *pAlarmDevice = new CDeviceStatusItem;
	
	memcpy(pAlarmDevice->m_szDeviceID,"34020000001340000001",strlen("34020000001340000001"));
	
	pGb2818_Config->m_deqAlarmItem.push_back(pAlarmDevice);
	//test  end
}
#endif
//判断两个不同地方记录的mac地址是否一致。
//返回值代表是否需要重启。
bool bRestartForCheckMac( )
{
	//读取flash中的mac地址
	char macflash[50] = {0};
	if (!CLocalDevice::GetMacFromFlash(macflash, 50))
	{
		//读取flash失败，就不需要比较了
		return false;
	}
	if (17 != strlen(macflash))
	{
		//flash中记录的有问题，就不需要比较了
		return false;
	}

	printf("macflash:%s MAC=", macflash);
	for (int i=0; i<17; i++)
	{
		printf("%02x", macflash[i]);
	}
	printf("\n");

	
	bool bModifyFile = false;
	char macfile[50] = {0};
	do 
	{
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPGM__)
		FILE *pFile = fopen("/mnt/mtd/mac.dat", "rb");
#else
		FILE *pFile = fopen("/etc/init.d/mac.dat", "rb");
#endif
		if (NULL == pFile)
		{
			bModifyFile = true;
			break;
		}
		char str[80] = {0};
		fgets(str, 80, pFile);				
		fclose(pFile);
		snprintf(macfile, 18, "%s", str);
		printf("macfile:%s MAC=", macfile);
		for (int i=0; i<17; i++)
		{
			printf("%02x", macfile[i]);
		}
		printf("\n");

		if (0 != strncmp(macfile, macflash, 17))
		{
			bModifyFile = true;
			int lenFile = strlen(macfile);
			int lenFlash = strlen(macflash);

			printf("%s:%s:%d, running at here, lenFile=%d, lenFlash=%d\n", __FUNCTION__, __FILE__, __LINE__, lenFile, lenFlash);
			break;
		}

		bModifyFile = false;
	} while(0);

	if (bModifyFile)
	{
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPGM__)
		FILE * pFile = fopen("/mnt/mtd/mac.dat", "wb");
#else
		FILE * pFile = fopen("/etc/init.d/mac.dat", "wb");
#endif

		if (NULL == pFile)
		{
			int lenFile = strlen(macfile);
			int lenFlash = strlen(macflash);

			printf("%s:%s:%d, running at here, lenFile=%d, lenFlash=%d\n", __FUNCTION__, __FILE__, __LINE__, lenFile, lenFlash);
			return false;
		}
		if (1 != fwrite(macflash, 17, 1, pFile))
		{
			fclose(pFile);
			int lenFile = strlen(macfile);
			int lenFlash = strlen(macflash);

			printf("%s:%s:%d, running at here, lenFile=%d, lenFlash=%d\n", __FUNCTION__, __FILE__, __LINE__, lenFile, lenFlash);
			return false;
		}
		sync();
		fclose(pFile);

		int lenFile = strlen(macfile);
		int lenFlash = strlen(macflash);

		printf("%s:%s:%d, running at here, lenFile=%d, lenFlash=%d\n", __FUNCTION__, __FILE__, __LINE__, lenFile, lenFlash);
		return true;
	}
	else
	{
		int lenFile = strlen(macfile);
		int lenFlash = strlen(macflash);

		printf("%s:%s:%d, running at here, lenFile=%d, lenFlash=%d\n", __FUNCTION__, __FILE__, __LINE__, lenFile, lenFlash);
		return false;
	}
}

typedef struct _param_flash_screen
{
	unsigned short cx;
	unsigned short cy;
}PARAM_FLASH_SCREEN;

enum FLASH_SCREEN_ENUM
{
	FLASH_SCREEN_MSG_NONE,
	FLASH_SCREEN_MSG_OK,
	FLASH_SCREEN_MSG_FAIL,
	FLASH_SCREEN_MSG_WAITING,
};

typedef struct _flash_screen_color
{
	unsigned short	pos;		//起始着色位置
	unsigned short	len;		//着色长度
	unsigned long	foreColor;	//字颜色
	unsigned long	bgColor;	//背景颜色
	unsigned long	style;		//绘制样式
}FLASH_SCREEN_COLOR;

typedef struct _flash_screen_msg
{
	unsigned long		id;				//[1 ~ n]
	unsigned long		state;			//0 none, 1 ok, 2 fail, 3 waiting
	unsigned long		dataLen;		//有效着色数据长度
	FLASH_SCREEN_COLOR	data[16];		//着色数据,保持递增
	char*				pText;
}FLASH_SCREEN_MSG;

bool						g_bFlashScreenRun = false;
CPUB_Lock					g_flashScreenLock;
CMyList<FLASH_SCREEN_MSG>	g_flashScreenMsgList;

void AddFlashScreenMsg(unsigned long id, unsigned long state, const char* pStr = NULL, FLASH_SCREEN_COLOR* pColor = NULL, unsigned long len = 0)
{
	if (!bUseUi)
	{
		return;
	}
	g_flashScreenLock.Lock();

	//去掉“强调部分单词”，即所有的对话都是白色字体
	pColor = NULL;
	len = 0;

	if(g_bFlashScreenRun)
	{
		FLASH_SCREEN_MSG msg;
		msg.id = id;
		msg.state = state;
		msg.dataLen = len;

		memset(msg.data, 0, sizeof(msg.data));
		if(len > 0)
		{
			assert(pColor);
			assert( sizeof(msg.data) / sizeof(FLASH_SCREEN_COLOR) >= len);
			memcpy(msg.data, pColor, sizeof(FLASH_SCREEN_COLOR) * len);
		}

		if(pStr)
		{
			int len = strlen(pStr);
			msg.pText = new char[len + 1];
			memset(msg.pText, 0, len + 1);

			strcpy(msg.pText, pStr);
		}
		else
		{
			msg.pText = NULL;
		}

		g_flashScreenMsgList.AddTail(msg);
	}
	g_flashScreenLock.UnLock();
}

bool GetFlashScreenMsg(FLASH_SCREEN_MSG& msg)
{
	bool ret = false;

	g_flashScreenLock.Lock();

	if(!g_flashScreenMsgList.IsEmpty())
	{
		msg = g_flashScreenMsgList.RemoveHead();
		ret = true;
	}

	g_flashScreenLock.UnLock();

	return ret;
}

static RESULT WINAPI FlashScreenThread(LPVOID lpParameter)
{
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid = %d start flash screen.\n", __FILE__, __LINE__, pid, tid);

	PARAM_FLASH_SCREEN* pParam = reinterpret_cast<PARAM_FLASH_SCREEN *>(lpParameter);
	CFBInterface* pFBInterface = CFBInterface::Instance();

	int zoom = (pParam->cx < 1024) ? 1 : 2;
	char textMark[2][8] = {"[", "]"}; 
	char textInfo[4][16] = {"", "OK", "Fail", "Wait"};
	char textWaiting[4][16] = {"Wait", "Wait.", "Wait..", "Wait..."};
	unsigned long color[4] = {GUI_RGB(255, 255, 255), GUI_RGB(0, 255, 0), GUI_RGB(255, 0, 0), GUI_RGB(255, 255, 255)};

	unsigned short mark_width = 0, char_width = 0, char_height = 0;
	unsigned short char_off = 6;
	pFBInterface->TextSize(textMark[0], strlen(textMark[0]), mark_width, char_height, zoom);
	pFBInterface->TextSize(textWaiting[3], strlen(textWaiting[3]), char_width, char_height, zoom);

	unsigned short left_text_x = pParam->cx / 10, top_text_y = 80;
	unsigned short text_cx = pParam->cx * 17 / 30;

	unsigned short ret_cx = ((pParam->cx * 1 / 3 - 16) < (char_width + mark_width * 4 + 8)) ? (char_width + mark_width * 4 + 8) : (pParam->cx * 1 / 3 - 16);
	unsigned short left_ret_x = pParam->cx - ret_cx - 8;

	int screen_msg_row = 0;
	int screen_max_row = 14;//(pParam->cy - 2 * top_text_y) / (char_height + char_off);
	int indexWaiting = 0;

	if(pParam->cy > screen_max_row * (char_height + char_off))
	{
		top_text_y = (pParam->cy - screen_max_row * (char_height + char_off)) / 2;
	}
	else
	{
		screen_max_row = (pParam->cy - 2 * top_text_y) / (char_height + char_off);
	}

	FLASH_SCREEN_MSG lastMsg;
	memset(&lastMsg, 0, sizeof(FLASH_SCREEN_MSG));
	pFBInterface->Clear(0, 0, pParam->cx, pParam->cy);
	pFBInterface->Refresh(0, 0, pParam->cx, pParam->cy);

	int sleep_count = 0;

	while(g_bFlashScreenRun)
	{
		FLASH_SCREEN_MSG msg;
		memset(&msg, 0, sizeof(FLASH_SCREEN_MSG));

		if(GetFlashScreenMsg(msg))
		{
			//printf("get msg id = %d.\n", msg.id);
			assert(0 != msg.id);

			if(msg.id >= lastMsg.id)
			{
				if(msg.id > lastMsg.id)
				{
					screen_msg_row++;

					if(screen_msg_row >= screen_max_row)
					{
						pFBInterface->Clear(0, 0, pParam->cx, pParam->cy);
						pFBInterface->Refresh(0, 0, pParam->cx, pParam->cy);
						screen_msg_row = 1;
					}
				}

				//拷贝信息
				{
					char* pLastText = lastMsg.pText;

					lastMsg.id = msg.id;
					lastMsg.state = msg.state;

					if(msg.pText)
					{
						if(pLastText)
						{
							delete [] pLastText;
							pLastText = NULL;
						}
						lastMsg.pText = msg.pText;

						lastMsg.dataLen = msg.dataLen;
						memcpy(lastMsg.data, msg.data, sizeof(lastMsg.data));
					}
				}

				//绘制信息
				unsigned short y = top_text_y + (char_height + char_off) * (screen_msg_row - 1);
				unsigned short cx = pParam->cx - left_text_x;

				pFBInterface->Clear(left_text_x, y, cx, char_height + char_off);

				if(lastMsg.pText)
				{
					text_cx+=15;
					//普通绘制
					pFBInterface->DrawText(lastMsg.pText, strlen(lastMsg.pText), left_text_x, y, text_cx, char_height, color[0], zoom, true);

					//着色处理
					unsigned short color_x = left_text_x, color_cx = 0, color_cy = 0;
					unsigned short text_len = strlen(lastMsg.pText);
					for(int i = 0; i < lastMsg.dataLen; i++)
					{
						if(lastMsg.data[i].pos >= text_len)
						{
							printf("Error flash screen msg param. pos = %d, len = %d, text = %s.\n", lastMsg.data[i].pos, lastMsg.data[i].len, lastMsg.pText);
							assert(false);
							continue;
						}
						
						unsigned short draw_len = (text_len > (lastMsg.data[i].pos + lastMsg.data[i].len)) ? lastMsg.data[i].len : (text_len - lastMsg.data[i].pos);
						
						//起始位置
						pFBInterface->TextSize(lastMsg.pText, lastMsg.data[i].pos, color_cx, color_cy, zoom);
						color_x = left_text_x + color_cx;

						//字符宽度
						pFBInterface->TextSize(&lastMsg.pText[lastMsg.data[i].pos], draw_len, color_cx, color_cy, zoom);

						//绘制
						pFBInterface->DrawRect(color_x, y, color_cx, char_height, lastMsg.data[i].bgColor);
						pFBInterface->DrawText(&lastMsg.pText[lastMsg.data[i].pos], draw_len, color_x, y, color_cx, char_height, lastMsg.data[i].foreColor, zoom, true);
					
						//样式
						if(1 == lastMsg.data[i].style)
						{
							pFBInterface->DrawLine(color_x, y + color_cy + 2 * zoom, color_x + color_cx, y + color_cy + 2 * zoom, lastMsg.data[i].foreColor, zoom);
						}
					}
				}
				
				if((lastMsg.state < 4) && (FLASH_SCREEN_MSG_NONE != lastMsg.state))
				{
					pFBInterface->DrawText(textMark[0], strlen(textMark[0]), left_ret_x, y, mark_width, char_height, color[lastMsg.state], zoom, true);
					pFBInterface->DrawText(textMark[1], strlen(textMark[1]), left_ret_x + char_width + mark_width * 3, y, mark_width, char_height, color[lastMsg.state], zoom, true);
					pFBInterface->DrawText(textInfo[lastMsg.state], strlen(textInfo[lastMsg.state]), left_ret_x + mark_width * 2, y, char_width, char_height, color[lastMsg.state], zoom, true);
				}

				pFBInterface->Refresh(left_text_x, y, cx, char_height + char_off);

				if(FLASH_SCREEN_MSG_WAITING == lastMsg.state)
				{
					indexWaiting = 0;
				}
			}
			else
			{
				if(msg.pText)
				{
					delete [] msg.pText;
					msg.pText = NULL;
				}
			}
		}
		else
		{
			sleep_count++;
			
			if(sleep_count >= 10)
			{
				if(FLASH_SCREEN_MSG_WAITING == lastMsg.state)
				{
					indexWaiting++;
					if(indexWaiting >= 4)
					{
						indexWaiting = 0;
					}

					unsigned short y = top_text_y + (char_height + char_off) * (screen_msg_row - 1);

					pFBInterface->Clear(left_ret_x, y, ret_cx, char_height + char_off);

					pFBInterface->DrawText(textMark[0], strlen(textMark[0]), left_ret_x, y, mark_width, char_height, color[lastMsg.state], zoom, true);
					pFBInterface->DrawText(textMark[1], strlen(textMark[1]), left_ret_x + char_width + mark_width * 3, y, mark_width, char_height, color[lastMsg.state], zoom, true);
					pFBInterface->DrawText(textWaiting[indexWaiting], strlen(textWaiting[indexWaiting]), left_ret_x + mark_width * 2, y, char_width, char_height, color[lastMsg.state], zoom, true);
					
					pFBInterface->Refresh(left_ret_x, y, ret_cx, char_height + char_off);
				}

				sleep_count = 0;
			}
		}

		PUB_Sleep(20);
	}

	if(lastMsg.pText)
	{
		delete [] lastMsg.pText;
		lastMsg.pText = NULL;
	}

	//刷黑屏幕
	pFBInterface->Clear(0, 0, pParam->cx, pParam->cy);
	pFBInterface->Refresh(0, 0, pParam->cx, pParam->cy);

	//清空队列
	g_flashScreenLock.Lock();
	if(g_flashScreenMsgList.GetCount() > 0)
	{
		POS pos = g_flashScreenMsgList.GetHeadPosition();
		while(pos)
		{
			FLASH_SCREEN_MSG& msg = g_flashScreenMsgList.GetAt(pos);
			
			if(msg.pText)
			{
				delete [] msg.pText;
				msg.pText = NULL;
			}
			g_flashScreenMsgList.GetNext(pos);
		}

		g_flashScreenMsgList.RemoveAll();
	}
	g_flashScreenLock.UnLock();

	printf("%s, %d, pid = %d, end flash screen.\n", __FILE__, __LINE__, pid);
}

void SetDeviceNomenuImage(unsigned long language)
{
#ifdef __ENVIRONMENT_LINUX__
	//CImage * pImge = CImageList::Instance()->GetImage(BMP_TIP_IMG_EN);	
	//CLocalDevice::Instance()->SetDeviceNomenuImage(pImge->GetDate(), pImge->Width(), pImge->Height());	
#endif
} 


int main(int argc, char** argv)
{
	//需要的变量声明
	/*sigset_t    sig_set, sig_pending;*/
#if !defined(__CHIP3520__) && !defined(__CHIP3515__) && !defined(__CHIP3531__) && !defined(__CHIP3535__) && !defined(__CHIP3521__) && !defined(__CHIP3520A__) && !defined(__CHIP3520D__) && !defined(__CHIPGM__) 
	signal(SIGSEGV, DebugBackTrace);
#endif

#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
	signal(SIGPIPE, SIG_IGN);
#endif

#ifdef __ENVIRONMENT_LINUX__
	mkfifo(CLIENT_FIFO_NAME, 0660);
#endif

	// 设置信号阻塞
	/*sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGTERM);
	sigaddset(&sig_set, SIGINT);
	sigprocmask(SIG_BLOCK, &sig_set, NULL);*/

	//////////////////////////////////////////////////////////////////////////
	//读产品ID
	//subProductID是从硬件中读取的机器的厂家ID
	//DVR_CUSTOM_ID是升级包的厂家ID
	//软件的特性由根据升级包的厂家ID决定。
	//而subProductID用于判断：升级包的厂家ID与机器的厂家ID是否一致。
	//如果不一致，那么本来是不允许启动的，但是目前只对BR27做了限制，其他厂家没有限制

	g_crytoStatus = CLocalDevice::CrytoInfoStatus();
	unsigned long productID = CLocalDevice::GetProductID();
	printf("#############%s,%d,productID:%d\n",__FILE__,__LINE__,productID);
	unsigned long subProductID = 0;
	if (CRYTOINFO_STATUS_NEW == g_crytoStatus)
	{
		subProductID = CLocalDevice::GetProductSubIDNew();
	}
	else if (CRYTOINFO_STATUS_OLD == g_crytoStatus)
	{
		subProductID = CLocalDevice::GetProductSubID();
	}
	else
	{
		subProductID = 0;

	}	
	printf("productID:%d,subProductID:%d\n",productID, subProductID);

	//判断程序的厂家ID与机器的厂家ID是否一致
	{
#ifdef NDEBUG
		unsigned long programeId = DVR_CUSTOM_ID;
		printf("%s,%d,programeId:%d,subProductID:%d.",__FILE__,__LINE__,programeId,subProductID);
		
		//目前只有BR27的包有限制：1.BR27的包只能装到BR27的机器上，其他的包没有这个限制.
		//                        2.BR27的机器只能装BR27的包，其他的机器没有这个限制
		if ((5 == programeId) || (5 == subProductID))
		{
			if (subProductID == programeId)
			{
				g_bProIdequeSubProdID = true;
			}
			else
			{
				g_bProIdequeSubProdID = false;
			}
			printf("%s,%d,BR27 g_bProIdequeSubProdID:%d\n",__FILE__,__LINE__,g_bProIdequeSubProdID);
		}
		//IL03的限制
		else if ((13 == programeId))
		{
			if ((13 == programeId) && (TD_TVT_IL03 == subProductID))
			{
				g_bProIdequeSubProdID = true;
			}
			else
			{
				g_bProIdequeSubProdID = false;
			}
			printf("%s,%d,IL03 g_bProIdequeSubProdID:%d\n",__FILE__,__LINE__,g_bProIdequeSubProdID);

		}
		else if (TD_TVT_US02 == programeId)
		{
			//US02版本
			if ((TD_TVT_US02 == programeId) && (TD_TVT_US02 == subProductID))
			{
				g_bProIdequeSubProdID = true;
			}
			else
			{
				g_bProIdequeSubProdID = false;
			}
		}
		else
		{
			g_bProIdequeSubProdID = true;
			printf("%s,%d,g_bProIdequeSubProdID:%d\n",__FILE__,__LINE__,g_bProIdequeSubProdID);
		}		
#else
		g_bProIdequeSubProdID = true;
#endif
	}

	if ((productID == TD_2304CDS)
	  ||(productID == TD_2308CDS)
	  ||(productID == TD_2316CDS)
	  ||(productID == TD_2316SE_S)
	  ||(productID == TD_2304HS))
	{
		g_bCVBSChangeResolution = true;
		printf("######################%s,%d,change CVBS Resolution\n",__FILE__,__LINE__);
	}
	else
	{
		g_bCVBSChangeResolution = false;
		printf("######################%s,%d,no change CVBS Resolution\n",__FILE__,__LINE__);
	}
	
	//如果是中文版本，机器的子版本号自动修改成1，语言只有中文

	//初始化产品参数类;用软件的厂家去初始化。从flash中读到的厂家ID只用于
	g_support_hd_type=CLocalDevice::GetHdChipType();
	
	CProduct *pProduct = CProduct::Instance();

	unsigned long ulAppProductID = productID;
	if (TD_2004AL == productID)
	{
		ulAppProductID = TD_2704AS_SL;
	}

	if (!pProduct->Initial("./product/product.def", ulAppProductID, DVR_CUSTOM_ID, productID))
	{
		DVRSystem("rm -rf /mnt/mtd/config/* -rf");
		CProduct::Instance()->writeDefaultResolution();
		printf("Initial product manager fail!\n");
		DVRSystem("exit");
		exit(0);
	}
	

	pProduct->SetProductSubIDInHardware(subProductID);

	g_bHaveSnapPic = CProduct::Instance()->SupportSnapPic();
#if defined (__CUSTOM_BR27__)
	if (productID <= TD_2316ME)
	{
		g_bHaveSnapPic = false;
		CProduct::Instance()->ChangePreRecordTime(5);		
	}
	else
	{
		g_bHaveSnapPic = true;
		//BR27版本如果用的是磁盘预录像就把录像时间设置为30S，使用内存预录像的时候不改变这个值
		if (PRERECORD_MODE_DISK == CProduct::Instance()->GetPreRecordType())
		{
			CProduct::Instance()->ChangePreRecordTime(30);
		}
	}
#endif

	//初始化功能信息
	char func[64] = {0};
	CLocalDevice::GetFuncCustomFromFlash(func, 6);
	if((static_cast<unsigned char>(func[0]) != 0xC3) && (static_cast<unsigned char>(func[5]) != 0x3C))
	{
		memset(func, 0xFF, 64);
		func[0] = 0xC3;
		func[5] = 0x3C;
	}
	pProduct->SetFuncCustomInfo(&func[1], 0, 4);

	//修改按键板的类型
	{
		char szKeyBoardName[16] = {0};
		int keyBoardType = CLocalDevice::GetKeyBoardNameFromFlash(szKeyBoardName, 16);
		if (keyBoardType >= 0)
		{
			printf("%s,%d,ChangeKeyBoardType keyboardType:%d \n",__FILE__,__LINE__,keyBoardType);
			pProduct->ChangeKeyBoardType((KEYBOARD_TYPE)keyBoardType, szKeyBoardName);
		}
		else
		{
			printf("%s,%d, no need ChangeKeyBoardType\n",__FILE__,__LINE__);
		}

		if ((TD_7004SB == pProduct->ProductType()) || (TD_7008SB == pProduct->ProductType()))
		{
			printf("###%s,%d,7004 7008change ChangeKeyBoardType\n ",__FILE__,__LINE__);
			pProduct->ChangeKeyBoardType(KEYBOARD_1UA, const_cast<char *>("Q1"));
		}
		printf("%s,%d,local keyboardtype;%d\n",__FILE__,__LINE__,CProduct::Instance()->LocalKeyboardType());
	}

	//读取语言包信息
	unsigned long languageBites = LANGUAGE_ALL;
	if (CRYTOINFO_STATUS_NEW == g_crytoStatus)
	{
		languageBites = CLocalDevice::GetSupportLanguageNew();
	}
	else if (CRYTOINFO_STATUS_OLD == g_crytoStatus)
	{
		languageBites = LANGUAGE_ALL;
		/*
		languageBites = CLocalDevice::GetSupportLanguage();
		if (48 == languageBites)
		{
			languageBites = LANGUAGE_ALL;
		}
		else if (49 == languageBites)
		{
			languageBites = LANGUAGE_ENGLISH;
		}
		else if (50 == languageBites)
		{
			languageBites = LANGUAGE_CHINESE_S;
		}
		else if (50 == languageBites)
		{
			languageBites = (~LANGUAGE_CHINESE_S);
		}
		else
		{
			languageBites = LANGUAGE_ALL;
		}
		*/
	}
	else
	{
		languageBites = LANGUAGE_ALL;
	}

	//不论后门的语言怎么配置，只有IL03才会有希伯来语
	languageBites &= (~LANGUAGE_HEBREW);


	//在br27的版本中，强制加入西班牙语，这个功能：过一段时间后，要去掉
#if defined (__CUSTOM_BR27__)
	if (TD_TVT_BR27 == subProductID)
	{			
		if (languageBites & LANGUAGE_SPAISH)
		{
			printf("br27 have LANGUAGE_SPAISH\n");
		}
		else
		{
			languageBites |= LANGUAGE_SPAISH;
			printf("%s,%d,change language %x\n",__FILE__,__LINE__,languageBites);
			CLocalDevice::WriteCrytoInfoNew(languageBites, subProductID);
		}		
	}
#ifdef __CUSTOM_US02_NEW__
	languageBites |= LANGUAGE_SPAISH;
#endif

#elif defined (__CUSTOM_RU04__)
	//在ru04的版本中，不论后门中的语言如何配置，只会有一种语言：俄语方言LANGUAGE_RUSSIAN_D
	{
		languageBites = LANGUAGE_RUSSIAN_D;
	}
#elif defined (__CUSTOM_CN__) || defined(__CUSTOM_SHANGHAI__)
	//如果是中文版本，机器的子版本号自动修改成1，语言只有中文
	{
		languageBites = LANGUAGE_CHINESE_S;
	}
#elif defined(__CUSTOM_IL03__)
	//如果是IL03版本，就加入希伯来语
	{
		languageBites |= LANGUAGE_HEBREW;
	}
#elif defined(__TW01_RILI__)
	//TW01日立版本，只有日语
	{
		languageBites = LANGUAGE_JAPANESE;
	}
#else
	{

	}
#endif

	#if defined(__KR24_IN__) && !defined(__KR24_CN__) || defined(__KOREAN__)
		languageBites |= LANGUAGE_CHINESE_S;
		languageBites &= (~LANGUAGE_CHINESE_B);
		languageBites &= (~LANGUAGE_JAPANESE);
	#endif
	CLanguagePack *pLanguagePack = CLanguagePack::Instance();
	if (!pLanguagePack->Initail("./", languageBites))
	{
		printf("Initail CLanguagePack fail!\n");
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		printf("Intial CLanguagePack succ!\n");
	}
	
	//初始化DDNS信息。DVR_CUSTOM_ID=1表示国内，DVR_CUSTOM_ID不为1表示国外, 国外的产品部使用www.dvrdydns.com服务器
	if( CReadDdns::Instance()->Initial(DVR_CUSTOM_ID))
	{
		printf("Initial ReadDDdns ok\n");
	}
	else
	{
		printf("Initial ReadDDdns error\n");
	}
	
	g_video_format = static_cast<VIDEO_FORMAT>(CLocalDevice::GetVideoFormatFromFlash());

	CConfigEx	*pConfig = CConfigEx::Instance();
	if (!pConfig->Initial("./"))
	{
		printf("Inital config manager fail\n");
		DVRSystem("rm -rf /mnt/mtd/config/*");
		CProduct::Instance()->writeDefaultResolution();
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		printf("Inital config manager succ\n");
	}

	//删除升级时保留的备份文件
	DVRSystem("rm -rf /mnt/Bkconfig");
	//读取本段初始化需要的配置参数
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_MANUAL_REC);
	itemList.AddTail(NCFG_ITEM_DEVICE_ID);
	itemList.AddTail(NCFG_ITEM_DEVICE_NAME);
	itemList.AddTail(NCFG_ITEM_VIDEO_FORMAT);
	itemList.AddTail(NCFG_ITEM_DATE_FORMAT);
	itemList.AddTail(NCFG_ITEM_HOUR_FORMAT);
	itemList.AddTail(NCFG_ITEM_LANGUAGE);	
	itemList.AddTail(NCFG_ITEM_TIME_ZONE);
	itemList.AddTail(NCFG_ITEM_VGA_RESOLUTION);
	itemList.AddTail(NCFG_ITEM_DAYLIGHT_INFO);
	itemList.AddTail(NCFG_ITEM_SHOW_WIZZARD);
	itemList.AddTail(NCFG_ITEM_DODWELL);
	itemList.AddTail(NCFG_ITEM_TOOLBAR_POS);
	itemList.AddTail(NCFG_ITEM_CVBS_VO_ADJUST);
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	itemList.AddTail(NCFG_ITEM_CVBS_TO_SPOT);
#endif
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_MODE);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_1X1);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_2X2);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_2X3);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_3X3);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_4X4);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_5X5);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_6X6);
	itemList.AddTail(NCFG_ITEM_VIEW_SPLIT_4X6);
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR);
	itemList.AddTail(NCFG_ITEM_ENCODE_MINOR);
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
#ifdef __NETSERVER_TVT__
	itemList.AddTail(NCFG_ITEM_UPNP_ENABLE);
	itemList.AddTail(NCFG_ITEM_HTTP_PORT);
	itemList.AddTail(NCFG_ITEM_SERVER_PORT);
#endif

#ifdef __NETSERVER_HXHT__
	itemList.AddTail(NCFG_ITEM_HTTP_PORT);
	itemList.AddTail(NCFG_ITEM_SERVER_PORT);
	itemList.AddTail(NCFG_ITEM_HXHT_NET_PARAM);
#endif
#ifdef __SUPPORT_3G__
		itemList.AddTail(NCFG_ITEM_NET_WIRELESS);
#endif
	itemList.AddTail(NCFG_ITEM_AUTO_REPORT);
#ifdef __SUPPORT_3G__
	itemList.AddTail(NCFG_ITEM_USE_3G);
#endif

	itemList.AddTail(NCFG_ITEM_SENSOR_IN_TYPE);

#ifdef __GB28181_SERVER__	
	//for  GB28181
	itemList.AddTail(NCFG_ITEM_GB28181_SERVER_INFO);
	itemList.AddTail(NCFG_ITEM_GB28181_CHANNEL_ID);
	itemList.AddTail(NCFG_ITEM_GB28181_ALARM_ID);
#endif
	
	unsigned char *pConfigData = NULL;
	unsigned long dataLen = 0;
	if (!pConfig->GetConfigEx(itemList, &pConfigData, dataLen))
	{
		printf("Get config data fail!\n");
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		itemList.RemoveAll();
	}

	CConfigBlock cfgBlock;
	cfgBlock.CreateBlock(pConfigData, dataLen);

	unsigned char *pData = NULL;
	//device information
	cfgBlock.GetConfig(NCFG_ITEM_DEVICE_ID, &pData, dataLen );
	unsigned short deviceID = *reinterpret_cast<const unsigned long *>(pData);

	pData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_CVBS_VO_ADJUST, &pData, dataLen );
	g_adjustCvbsVO = *reinterpret_cast<const unsigned long *>(pData);
	
	NCFG_CVBS_INFO CvbsData;
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	pData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_CVBS_TO_SPOT, &pData, dataLen );
	CvbsData.current =  (reinterpret_cast<NCFG_CVBS_INFO *>(pData))->current;
	CvbsData.last = (reinterpret_cast<NCFG_CVBS_INFO *>(pData))->last;
	printf("CVBS_ChangeParam********************************************************\n");
	pProduct->CVBS_ChangeParam(productID,CvbsData.current);
#endif	
	
	pData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_VIDEO_FORMAT, &pData, dataLen);
	g_video_format = static_cast<VIDEO_FORMAT>(*reinterpret_cast<unsigned long *>(pData));
	cfgBlock.GetConfig(NCFG_ITEM_DEVICE_NAME, &pData, dataLen );
	pProduct->SetDeviceInfo(reinterpret_cast<const NCFG_INFO_SHORT_NAME *>(pData)->name, deviceID,(unsigned long)g_video_format);

	//Initial device
	int videoOutMode = 0;
	ext_dvr_input_param parm;
	parm.videoFormat	= (int)g_video_format;
	parm.videoChnnNum	= pProduct->LocalVideoInputNum();
	parm.audioChnnNum	= pProduct->AudioInputNum();

	cfgBlock.GetConfig(NCFG_ITEM_VGA_RESOLUTION, &pData, dataLen);
	g_resolution = static_cast<VGA_RESOLUTION>(*reinterpret_cast<unsigned long *>(pData));

	if ((VGA_1920X1080 <= g_resolution))
	{
		if(VIDEO_OUT_DEVICE_HDMI & pProduct->VideoOutDeviceMask())
		{
			parm.video_out_mode = VIDEO_OUT_HDMI;
			if(340 == g_support_hd_type)
			{
				g_resolution =  (VIDEO_FORMAT_NTSC != g_video_format) ? VGA_1440X1152 : VGA_1440X960;
			}
		}
		else
		{
			parm.video_out_mode = VIDEO_OUT_VGA;
			if(VGA_1024X768 & pProduct->DisplayResolutionMask())
			{
				g_resolution =  VGA_1024X768;
			}
			else
			{
				g_resolution = VGA_1280X1024;
			}

			{
				unsigned long dataLen = sizeof(unsigned long) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD );
				unsigned char * pData = new unsigned char [dataLen];
				NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
				pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
				pBlockHead->ItemNum		= 1;
				pBlockHead->netcfgver	= NCFG_VERSION;

				NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
				pItemHead->itemID = NCFG_ITEM_VGA_RESOLUTION;
				pItemHead->len	  = sizeof(unsigned long);
				pItemHead->num	  = 1;
				pItemHead->subLen = sizeof(unsigned long);

				//修改问题：如果分辨率修改成CVBS，重启后，进入基本配置会出现断言。
				//如果分辨率是VGA_720X576, VGA_720X480，就表示是当前是CVBS输出，
				//而VGA_640X480表示CVBS输出。
				VGA_RESOLUTION resolutionTmp = g_resolution;
				
				memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &resolutionTmp, sizeof(unsigned long));

				if (!pConfig->SetConfig( pData, dataLen ))
				{
					assert (false);
				}

				delete [] pData;
				pData = NULL;
			}
			//把配置写入flash
			{				
				bool bCanWrite = true;
				PRODUCT_TYPE productTypeVideo = CProduct::Instance()->ProductType();
				if ((productTypeVideo >= TD_2704HD) && (productTypeVideo <= TD_2716XD_L))
				{
					if (VIDEO_OUT_CVBS == videoOutMode)
					{
						bCanWrite = false;
					}					
				}
				if (bCanWrite)
				{
					char szTmp[8] = {0};
					sprintf(szTmp, "%d", parm.video_out_mode);
					CLocalDevice::WriteDisplayModToFlash(szTmp, 1);
					printf("!!!!!!!!!!!!!!!!!%s,%d,szTmp:%s\n",__FILE__,__LINE__,szTmp);

					memset(szTmp, 0, sizeof(szTmp));
					snprintf(szTmp, 8, "%04x", g_resolution);
					CLocalDevice::WriteDisplayResToFlash(szTmp, 4);
					printf("!!!!!!!!!!!!!!!!!%s,%d,szTmp:%s\n",__FILE__,__LINE__,szTmp);
				}
			}
		}
	}
	else if (VGA_800X600 <= g_resolution)
	{
		parm.video_out_mode = VIDEO_OUT_VGA;	
#if defined(__TW01_RILI__)
		if(g_resolution < VGA_1024X768)
		{
			g_resolution = VGA_1280X1024;
		}
		{
			unsigned long dataLen = sizeof(unsigned long) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD );
			unsigned char * pData = new unsigned char [dataLen];
			NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
			pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
			pBlockHead->ItemNum		= 1;
			pBlockHead->netcfgver	= NCFG_VERSION;

			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
			pItemHead->itemID = NCFG_ITEM_VGA_RESOLUTION;
			pItemHead->len	  = sizeof(unsigned long);
			pItemHead->num	  = 1;
			pItemHead->subLen = sizeof(unsigned long);

			//修改问题：如果分辨率修改成CVBS，重启后，进入基本配置会出现断言。
			//如果分辨率是VGA_720X576, VGA_720X480，就表示是当前是CVBS输出，
			//而VGA_640X480表示CVBS输出。
			VGA_RESOLUTION resolutionTmp = g_resolution;

			memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &resolutionTmp, sizeof(unsigned long));

			if (!pConfig->SetConfig( pData, dataLen ))
			{
				assert (false);
			}

			delete [] pData;
			pData = NULL;
		}
#endif
	}
	else
	{
		parm.video_out_mode = VIDEO_OUT_CVBS;

		g_resolution =  (VIDEO_FORMAT_NTSC != g_video_format) ? VGA_720X576 : VGA_720X480;
	}
	videoOutMode = parm.video_out_mode;

	//获取配置里的分辨率
	{		
		int configVideoOut = videoOutMode;
		int configRes = g_resolution;
		int flashVideoOut = CLocalDevice::Instance()->GetDisplayModFromFlash( );//获得设备
		int flashRes = CLocalDevice::Instance()->GetDisplayResFromFlash();//获得分辨率
		if ((flashVideoOut != configVideoOut) || (configRes != flashRes))
		{
			printf("###%s,%d, configVideoOut:%d,flashVideoOut:%d,configRes:%d,flashRes:%d\n",__FILE__,__LINE__,configVideoOut,flashVideoOut,configRes,flashRes);
			//把配置写入flash
			{
				bool bCanWrite = true;
				PRODUCT_TYPE productTypeVideo = CProduct::Instance()->ProductType();
				if ((productTypeVideo >= TD_2704HD) && (productTypeVideo <= TD_2716XD_L))
				{
					if (VIDEO_OUT_CVBS == configVideoOut)
					{
						bCanWrite = false;
					}					
				}
				if (bCanWrite)
				{
					char szTmp[8] = {0};
					sprintf(szTmp, "%d", configVideoOut);
					CLocalDevice::WriteDisplayModToFlash(szTmp, 1);
					printf("!!!!!!!!!!!!!!!!!%s,%d,szTmp:%s\n",__FILE__,__LINE__,szTmp);

					memset(szTmp, 0, sizeof(szTmp));
					snprintf(szTmp, 8, "%04x", configRes);
					CLocalDevice::WriteDisplayResToFlash(szTmp, 4);
					printf("!!!!!!!!!!!!!!!!!%s,%d,szTmp:%s\n",__FILE__,__LINE__,szTmp);
				}		
			}				
		}
		else
		{
			printf("##%s,%d,videoout res flash == config\n",__FILE__,__LINE__);
		}
	}
	
	//

	
	//Encode
	cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR, &pData, dataLen);
	ENCODE_INFO *pMainEncode = reinterpret_cast<ENCODE_INFO *>(pData);
	for (unsigned char chnn = 0; chnn < parm.videoChnnNum; ++chnn)
	{
		parm.videoLocalSize[chnn] = pMainEncode[chnn].resolution;
	}
	cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MINOR, &pData, dataLen);
	ENCODE_INFO *pMinEncode = reinterpret_cast<ENCODE_INFO *>(pData);
	for (unsigned char chnn = 0; chnn < parm.videoChnnNum; ++chnn)
	{
		parm.videoNetSize[chnn] = pMinEncode[chnn].resolution;
	}

//xian
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	if ((TD_2704AS_SL_A==productID)||(TD_2708AS_S_A ==productID))
	{
		if ((0==CvbsData.current)||(1==CvbsData.current))
		{
			unsigned char *pCfgMajorData = NULL;
			unsigned char *pCfgExData = NULL;
			ENCODE_INFO_EX *pExInfo = NULL;
			ENCODE_INFO *pEncode = NULL;
			unsigned char* preadData = NULL;
			unsigned long readlen = 0;

			cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR, &pCfgMajorData, dataLen);
			pEncode = (ENCODE_INFO *)(pCfgMajorData);

			//读取底面配置ENCODE_INFO_EX并把pMajor的值赋给他
			unsigned long dataLenEX=0;
			cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR_EX, &pCfgExData, dataLenEX);
			pExInfo = (ENCODE_INFO_EX *)(pCfgExData);
			int localVideoInput_cvbs = pProduct->LocalVideoInputNum();
			int videoInputNum_cvbs = pProduct->VideoInputNum();
			for (int i=0; i<localVideoInput_cvbs; i++)//Ex超过m_localVideoInputNum部分是乱码,需要读取pExInfo的进行合成
			{
				if (pEncode[i].rate >15)
				{
					if (VIDEO_FORMAT_PAL == g_video_format)
					{
						pEncode[i].rate = 12;
						pExInfo[i].rate = 12;
					}
					if (VIDEO_FORMAT_NTSC == g_video_format)
					{
						pEncode[i].rate = 15;
						pExInfo[i].rate = 15;
					}
				}
			}

			NCFG_BLOCK_HEAD blockHead;
			blockHead.netcfgver = NCFG_VERSION;
			blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
			blockHead.ItemNum = 1;
			int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
			int setDataLen = 0;
			setDataLen = header_len + videoInputNum_cvbs *sizeof(ENCODE_INFO_EX);

			unsigned char *pData_cvbs = new unsigned char[setDataLen];
			NCFG_ITEM_HEAD itemHeadEx;
			itemHeadEx.itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
			itemHeadEx.len = videoInputNum_cvbs *sizeof(ENCODE_INFO_EX);
			itemHeadEx.num = videoInputNum_cvbs;
			itemHeadEx.subLen = sizeof(ENCODE_INFO_EX);
			ENCODE_INFO_EX *pNewEncode = (ENCODE_INFO_EX *)(pData_cvbs + header_len);
			memcpy( pData_cvbs, &blockHead, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
			memcpy(pData_cvbs+blockHead.biSize,&itemHeadEx, blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
			memcpy(pNewEncode,pExInfo,sizeof(ENCODE_INFO_EX)*videoInputNum_cvbs);

			pConfig->SetConfig(pData_cvbs, setDataLen);
			if(NULL != pData_cvbs)
			{
				delete [] pData_cvbs;
				pData_cvbs =NULL;
			}
			//
			NCFG_BLOCK_HEAD blockHead_MAJOR;
			blockHead_MAJOR.netcfgver = NCFG_VERSION;
			blockHead_MAJOR.biSize = sizeof(NCFG_BLOCK_HEAD);
			blockHead_MAJOR.ItemNum = 1;
			int header_len_major = blockHead_MAJOR.biSize + blockHead_MAJOR.ItemNum * sizeof(NCFG_ITEM_HEAD);
			int setData = 0;
			setData = header_len_major + localVideoInput_cvbs *sizeof(ENCODE_INFO);

			unsigned char *pDataMajor = new unsigned char[setData];
			NCFG_ITEM_HEAD itemHeadEx_major;
			itemHeadEx_major.itemID = NCFG_ITEM_ENCODE_MAJOR;
			itemHeadEx_major.len = localVideoInput_cvbs *sizeof(ENCODE_INFO);
			itemHeadEx_major.num = localVideoInput_cvbs;
			itemHeadEx_major.subLen = sizeof(ENCODE_INFO);
			ENCODE_INFO *pNewEncodeMajor = (ENCODE_INFO *)(pDataMajor + header_len_major);
			memcpy( pDataMajor, &blockHead_MAJOR, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
			memcpy(pDataMajor+blockHead_MAJOR.biSize,&itemHeadEx_major, blockHead_MAJOR.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
			memcpy(pNewEncodeMajor,pEncode,sizeof(ENCODE_INFO)*localVideoInput_cvbs);

			pConfig->SetConfig(pDataMajor, setData);
			if(NULL != pDataMajor)
			{
				delete [] pDataMajor;
				pDataMajor =NULL;
			}

			if(NULL != preadData)
			{
				delete [] preadData;
				preadData =NULL;
			}
		}
	}
#endif

	//这里读SENSOR IN 的类型是为了初始化MCU抓取的报警输入信号
	//当外部报警设备是常开时MCU抓取的值是1为无效，不能触发报警输出。为0时有效，必须触发报警输出。
	//启动时要把MCU中的报警输入变量初始化为1，防止报警管理线程抓取的前几个sensor值是假的
	//当外部报警设备是常闭时相反
	//在配置中NO为0，NC为1，和实际状态相反了
	cfgBlock.GetConfig(NCFG_ITEM_SENSOR_IN_TYPE, &pData, dataLen);
	unsigned long *pSensorInputType = reinterpret_cast< unsigned long *>(pData);
	DWORD sensorInputType;
	memset( &sensorInputType, 0, sizeof( DWORD ) );
	for( int i=0; i<pProduct->SensorInputNum(); i++ )
	{
		sensorInputType |= (0 != pSensorInputType[i]) ? (0x00):(0x01 << i);
	}

	//回放通道数目
	//SDI回放是否降帧的控制由SDK做处理;
	
	parm.videoPbChnnNum = 0;
	parm.videoPbChnnNum = static_cast<int>(pProduct->VideoOutputNum());
	parm.videoPbChnnNum = (parm.videoPbChnnNum & 0x000000ff);

#if defined(__PACKID_14__) && defined(__PRIMARY_WRDISK__) || defined(__PACKID_27__)
	{
		PRODUCT_TYPE productIDT = pProduct->ProductType();
		if ((TD_2716XD_P == productIDT) || (TD_2716XD_S == productIDT))
		{
			printf("%s:%s:%d, 2716XD_P, TD_2716XD_S回放不降帧\n", __FUNCTION__, __FILE__, __LINE__);
			parm.videoPbChnnNum |= (LOCAL_PB_ATTRIBUTE_NONE << 8);
		}
		else
		{
			parm.videoPbChnnNum |= (LOCAL_PB_ATTRIBUTE_SDK_CTRL << 8);
		}
	}
#else
	//
	//SDK没有使用这个变量做回放参数，就扩展为用来控制回放时是否降帧
	parm.videoPbChnnNum |= (LOCAL_PB_ATTRIBUTE_SDK_CTRL << 8);
#endif

	bool bHaveVideoMode = false;
	if ((TD_2308SL_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || 
		(TD_2308SE_C == productID) || (TD_2316SE_C == productID) || (TD_2308ME_C == productID) || 
		(TD_2316ME_C == productID))
	{
		bHaveVideoMode = true;
	}

#if defined(__CUSTOM_US02__)
	if (TD_2516HS_BL == productID)
	{
		bHaveVideoMode = true;
	}
#endif

	if (bHaveVideoMode)
	{
		int encodeSize = CLocalDevice::Instance()->GetEncodeSize();
		parm.videoPbChnnNum |= (encodeSize << 16);
	}

	printf("%s:%s:%d, videoPbChnnNum=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, parm.videoPbChnnNum);

	//确定产品的ID
	parm.dvr_type = static_cast<int>(productID);
	//产品子ID
	parm.dvr_sub_version = static_cast<int>(pProduct->ProductSubID());
	//
	unsigned short width = 0, height = 0;
	GetVGAResolution(g_resolution, width, height);
	parm.video_out_width = width;
	parm.video_out_height = height;
	//
	parm.audio_speek_type = pProduct->GetSpeekType();
#ifndef __SUPPORT_SNAPD1__
	parm.jpeg_capture_enable = JPEG_ATTRIBUTE_NO_OSD;
#else	
	parm.jpeg_capture_enable = JPEG_ATTRIBUTE_HAVE_OSD;
#endif

    //当以cvbs为主输出启动时，VGA上显示的图像大小为1024X768，但是sdk默认VGA输出化的大小为1280X1024
	bool bcvbs_change = false;
	int org_video_out_width = parm.video_out_width;
	int org_video_out_height = parm.video_out_height;
	int org_video_out_mode = parm.video_out_mode;

	
    if(g_resolution<VGA_800X600)
    {
		bcvbs_change = true;
		parm.video_out_width = 1024;
		parm.video_out_height = 768;
		parm.video_out_mode = VIDEO_OUT_VGA;

		if ((productID >= TD_2704HD) && (productID <= TD_2716XD_L))
		{
			bool bGetOk = false;
			int device=CLocalDevice::Instance()->GetDisplayModFromFlash( );//获得设备
			int resolution=CLocalDevice::Instance()->GetDisplayResFromFlash();//获得分辨率
			if (VIDEO_OUT_VGA == device)
			{
				if((resolution==VGA_800X600) || (resolution==VGA_1024X768) || (resolution==VGA_1280X960)|| (resolution==VGA_1280X1024))
				{
					bGetOk = true;
				}
				else
				{
					bGetOk = false;
				}
			}
			else if (VIDEO_OUT_HDMI == device)
			{
				if (resolution==VGA_1920X1080)
				{
					bGetOk = true;
				}
				else
				{
					bGetOk = false;
				}
			}
			else
			{
				bGetOk = false;
			}

			if (bGetOk)
			{
				parm.video_out_mode = device;
				unsigned short width = 0;
				unsigned short height = 0;
				GetVGAResolution((VGA_RESOLUTION)resolution, width, height);
				parm.video_out_width = width;
				parm.video_out_height = height;
				if ((TD_2704HD == productID) && (!CProduct::Instance()->Is2704HDNewHarWare()))
				{
					printf("is  2708 old\n");
				}
				else
				{
					printf("is not 2708 old\n");
					//parm.video_out_width = 1920;	
					//parm.video_out_height = 1080;
				}				
				printf("###%s,%d, get DISPLAY_DEV_MINOR ok,outdevice:%d,width:%d height:%d\n",__FILE__,__LINE__,parm.video_out_mode,parm.video_out_width,parm.video_out_height);
			}
			else 
			{
				printf("##%s,%d,get DISPLAY_DEV_MINOR faild\n",__FILE__,__LINE__);
				if ((TD_2704HD == productID) && (!CProduct::Instance()->Is2704HDNewHarWare()))
				{
					printf("is  2708 old \n");
					parm.video_out_mode = VIDEO_OUT_VGA;
					parm.video_out_width = 1024;
					parm.video_out_height = 768;
				}
				else
				{
					printf("is not 2708 old\n");
					parm.video_out_mode = VIDEO_OUT_HDMI;
					parm.video_out_width = 1920;
					parm.video_out_height = 1080;
				}
			}		
		}
	}

#if defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIPGM__)
	CLocalDevice::Instance()->AdjustCVBSVo(g_adjustCvbsVO);
#endif
	
	//初始化设备
	CLocalDevice	*pLocalDevice = CLocalDevice::Instance();
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__) || defined(__CHIP3531__)	
	if (!pLocalDevice->Initial(parm, pProduct->VideoOutputNum(), pProduct->NetVideoInputNum(), sensorInputType, pProduct->SupportMCU(),(unsigned char *)(&CvbsData) ))
#else
	if (!pLocalDevice->Initial(parm, pProduct->VideoOutputNum(), pProduct->NetVideoInputNum(), sensorInputType, pProduct->SupportMCU() ))
#endif	
	{
		printf("Initial local device fail!\n");
		if (pConfig->IsCreatNewFile())
		{
			DVRSystem("rm /mnt/mtd/config/*.dat");
		}
		pLocalDevice->FeedDog();
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		printf("Initial local device succ!\n");
	}
	//初始化后再改回来
	if(bcvbs_change)
	{
		bcvbs_change = false;
		parm.video_out_width =  org_video_out_width;
		parm.video_out_height =  org_video_out_height;
		parm.video_out_mode =  org_video_out_mode;
		pLocalDevice->ChangeOutpuDevice(parm.video_out_mode,parm.video_out_width,parm.video_out_height);
	}

	g_video_out_mode = parm.video_out_mode;

	g_bUse1104A1108ChipColor = CLocalDevice::IsUse1104A1108Color();

	{
		if (pConfig->IsCreatNewFile())
		{
			printf("#########%s,%d,change color defaule\n",__FILE__,__LINE__);
			int chnnLocalNum = pProduct->LocalVideoInputNum();
			int chnnTotalNum = pProduct->VideoInputNum();
			
			unsigned long dataLen = (sizeof(unsigned long)*chnnTotalNum) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD );
			unsigned char * pData = new unsigned char [dataLen];

			NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
			pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
			pBlockHead->ItemNum		= 1;
			pBlockHead->netcfgver	= NCFG_VERSION;

			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
			pItemHead->itemID = NCFG_ITEM_VIDEO_COLOR;
			pItemHead->len	  = (sizeof(unsigned long)*chnnTotalNum);
			pItemHead->num	  = chnnTotalNum;
			pItemHead->subLen = sizeof(unsigned long);
			unsigned long *pValue = new unsigned long [chnnTotalNum];
			VIDEO_FORMAT videoFormatTemp = static_cast<VIDEO_FORMAT>(CLocalDevice::GetVideoFormatFromFlash()); //读取flash里的视频制式信息
#if defined(__SPECIAL_COLOR)
			if (CLocalDevice::Is2316SSNewPCB())
			{
				for (int i = 0; i < chnnLocalNum; ++i)
				{
					pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_1108 : DEFAULT_COLOR_PAL_1108;
				}
			}
			else
			{
				for (int i = 0; i < chnnLocalNum; ++i)
				{
					pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_25838 : DEFAULT_COLOR_PAL_25838;
				}
			}

#else
			if (g_bUse1104A1108ChipColor)
			{
				for (int i = 0; i < chnnLocalNum; ++i)
				{
					pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_1104 : DEFAULT_COLOR_PAL_1104;
				}
			}
			else
			{
				for (int i = 0; i < chnnLocalNum; ++i)
				{
					pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_2864 : DEFAULT_COLOR_PAL_2864;
				}
			}
#endif
#if defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__HISI_SDK_0BX__) || defined(__CHIPGM__)
			char bright = 0;
			char hue = 0;
			char sat =0;
			char contrast = 0;

#if defined(__CHIPGM__)
			if (0 == ext_get_video_Def_Color(&bright, &hue, &sat, &contrast))
#else
			bright = 128;
			contrast = 128;
			sat = 128;
			hue =128;
#endif
			{
				for (int i = 0; i < chnnLocalNum; ++i)
				{
					pValue[i] = (bright << 24) + (contrast << 16) + (sat << 8) + hue;;
				}
			}
#endif
			if ((0 < chnnLocalNum) && (chnnLocalNum < chnnTotalNum))
			{
				for(int i = 0; i < chnnLocalNum; i++)
				{
					pValue[i] = (128 << 24) + (128 << 16) + (128 << 8) + 128;
				}
			}

			for(int i = chnnLocalNum; i < chnnTotalNum; i++)
			{
				pValue[i] = (128 << 24) + (128 << 16) + (128 << 8) + 128;
			}
			memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), pValue,  pItemHead->len);
			pConfig->SetConfig( pData, dataLen );
			delete [] pData;
			pData = NULL;
		}
	}

	if (CLocalDevice::Instance()->Is2316SSNewPCB())
	{
		CProduct::Instance()->Chang2316SSSpot();
	}

	//原来把字库的初始化放在界面初始化时做，而现在需要在初始化框架（具体是设置配置数据前）前获得字体数据
	//闪屏画面需要FB初始化,所以把FB的初始化提前到此
	if (!GUI::CFBInterface::Instance()->Initial("./", width, height, pProduct->LocalVideoInputNum(), pProduct->VideoInputNum(), \
		pProduct->VideoOutputNum(), pProduct->NetVideoInputNum()))
	{
		pLocalDevice->FeedDog();
		printf("%s:%s:%d, Initial CFBInterface error\n", __FUNCTION__, __FILE__, __LINE__);
		DVRSystem("exit");
		exit(0);
	}
	GUI::CFBInterface::Instance()->SetResolution(width, height); //闪屏画面需要绘图
	GUI::CFBInterface::Instance()->SetTransparency(TRANSPARENCY_0);
	
	//启动闪屏画面
	PARAM_FLASH_SCREEN paramFlashScreen;
	memset(&paramFlashScreen, 0, sizeof(PARAM_FLASH_SCREEN));
	paramFlashScreen.cx = width;
	paramFlashScreen.cy = height;

	PUB_thread_t flashScreenID = PUB_CREATE_THREAD_FAIL;
	if (bUseUi)
	{
		flashScreenID = PUB_CreateThread(FlashScreenThread, (void *)(&paramFlashScreen), &g_bFlashScreenRun);
		if(PUB_CREATE_THREAD_FAIL == flashScreenID)
		{
			printf("Initial flash screen thread fail!\n");
		}
		else
		{
			printf("Start flash screen ok!\n");
		}
	}

	//判断/etc/init.d/mac.dat中的mac地址 是否与 flash中记录的mac地址 一致
	//如果不一致，修改/etc/init.d/mac.dat中的值。然后重启.
	//判断/etc/init.d/VideoFormat.dat中的 制式的值， 是否与 flash中记录的 一致
	//如果不一致，修改/etc/init.d/VideoFormat.dat中的值。然后重启.
	if (bUseUi)
	{
		bool bRestart1 = false;
		bool bRestart2 = false;
		bool bRestart3 = false;
		bRestart1 = bRestartForCheckMac();
		bRestart2 = bRestartForCheckVideoFormat();
		bRestart3 = CNICTypeMan::NICTypeIsNotSame(productID);

		if (bRestart1 || bRestart2 || bRestart3)
		{
			printf("%s,%d,mac error, bRestart1=%d, bRestart2=%d, bRestart3=%d\n",__FILE__, __LINE__, bRestart1 , bRestart2 , bRestart3);

			if ((productID >= TD_2424MD) && (TD_2504HE_M != productID) && (TD_7004SB != productID) && (TD_7008SB != productID))
 			{
 				//如果是25XXXX系列的机器，采取看门狗重启的方式
 				pLocalDevice->FeedDog();
 			}
 			else
 			{
 #if defined (__TDFH__) || defined(__CHIPTI__)
 				//FH平台使用看门狗重启
 				pLocalDevice->FeedDog();				
 #else
 				DVRSystem("reboot");
 #endif
 			}	
			return 1;
		}
	}
	
#if defined(__ENVIRONMENT_LINUX__) && defined(__CHIP3535__)
	{
		char szHdVer[7] = {0};
		char szTmp[7] = {0};
                bool ret = pLocalDevice->GetHardWareVersion(szHdVer, 7);
		if (!ret)
		{
			printf("########### Get old hardware version failed. %s %d\n", __FILE__, __LINE__);
		}
		else
		{
			char *pNewHardwareVersion = UpgradeDevice::Instance()->Upgrade(subProductID, productID, szHdVer);
			if (NULL != pNewHardwareVersion)
			{
				printf("Vender ID:%lu, Product ID:%lu, New Hardware Version:%s. %s %d\n", subProductID, productID, pNewHardwareVersion, __FILE__, __LINE__);

				/* write hardware version to flash */
				bool bWriteSucc = false;
				memset(szTmp, 0, sizeof(szTmp));
				snprintf(szTmp, sizeof(szTmp), pNewHardwareVersion);
				if (pLocalDevice->WriteHardWareVersion(szTmp, 6))
				{
					printf("Write hardware version ok. %s %d\n", __FILE__, __LINE__);
					bWriteSucc = true;
				}
				else
				{
					printf("########### Write hardware version failed. %s %d\n", __FILE__, __LINE__);
					bWriteSucc = false;
				}

				/* remove all configurations and reboot */
				if (bWriteSucc)
				{
					DVRSystem("rm -rf /mnt/mtd/config/*");
					pLocalDevice->KillDog();
 					pLocalDevice->FeedDog();
					return 1;
				}
			}
		}
	}
#endif

	unsigned long screenMsgID = 1;
	unsigned long sreenMsgColorLen = 0;
	FLASH_SCREEN_COLOR sreenMsgColor[16];
	memset(sreenMsgColor, 0, sizeof(sreenMsgColor));
	sreenMsgColor[0].pos = strlen("Check system ");
	sreenMsgColor[0].len = strlen("memory");
	sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColorLen = 1;
	AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, "Check system memory.", sreenMsgColor, sreenMsgColorLen);
	PUB_Sleep(1000);
	AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_OK, NULL);

	//time zone and daylight.
	//在localDevice初始化时初始化和系统时间。时区和DST信息必须在系统时间初始化后初始化
	sreenMsgColor[0].pos = strlen("Initialize system ");
	sreenMsgColor[0].len = strlen("time");
	sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColor[1].pos = strlen("Initialize system time and ");
	sreenMsgColor[1].len = strlen("zone");
	sreenMsgColor[1].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColorLen = 2;
	AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, "Initialize system time and zone.", sreenMsgColor, sreenMsgColorLen);
	cfgBlock.GetConfig(NCFG_ITEM_TIME_ZONE, &pData, dataLen );
	CTZoneDZoneCfg::Instance()->SetTimeZone( *reinterpret_cast<const unsigned long *>(pData));
	pData = NULL;
	cfgBlock.GetConfig( NCFG_ITEM_DAYLIGHT_INFO, &pData, dataLen );
	CTZoneDZoneCfg::Instance()->SetDaylightZone( reinterpret_cast<const DAYLIGHT_INFO *>(pData));

	//初始化消息管理器
	CMessageMan	*pMsgMan	= CMessageMan::Instance();
	if (!pMsgMan->Initial(pProduct->VideoInputNum(), pProduct->SensorInputNum()+pProduct->NetVideoInputNum()))
	{
		printf("Initial message man fail!\n");
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		printf("Initial message man succ\n");
	}

	//2010-02-24 11:08:00 YSW
	//由于在不显示界面的情况，需要叠加时间戳和通道名称（需要字库）
	cfgBlock.GetConfig(NCFG_ITEM_DATE_FORMAT, &pData, dataLen);
	g_dateMode = *reinterpret_cast<unsigned long *>(pData);
	cfgBlock.GetConfig(NCFG_ITEM_HOUR_FORMAT, &pData, dataLen);
	g_timeMode = *reinterpret_cast<unsigned long *>(pData);

	cfgBlock.GetConfig(NCFG_ITEM_LANGUAGE, &pData, dataLen);
	g_language = static_cast<LANGUAGE>(*(reinterpret_cast<unsigned long *>(pData)));


	{
		if (g_language & languageBites)
		{
			printf("cur language is in language list\n");
		}
		else
		{
			printf("cur language is not in language list,so change language to default:");
			unsigned long dataLen = sizeof(unsigned long) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD );
			unsigned char * pData = new unsigned char [dataLen];
			NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
			pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
			pBlockHead->ItemNum		= 1;
			pBlockHead->netcfgver	= NCFG_VERSION;
			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
			pItemHead->itemID = NCFG_ITEM_LANGUAGE;
			pItemHead->len	  = sizeof(unsigned long);
			pItemHead->num	  = 1;
			pItemHead->subLen = sizeof(unsigned long);
			unsigned long value = CLanguagePack::Instance()->GetDefault();
			memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &value, sizeof(unsigned long));
			pConfig->SetConfig( pData, dataLen );			
			delete [] pData;
			pData = NULL;
			g_language = static_cast<LANGUAGE>((value));
			printf("%d\n",g_language);
		}
	}

	
	AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_OK, NULL);
	
	//初始化系统语言
	sreenMsgColor[0].pos = strlen("Load system ");
	sreenMsgColor[0].len = strlen("language");
	sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColorLen = 1;
	AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, "Load system language.", sreenMsgColor, sreenMsgColorLen);
	CStringTable::Instance()->Initial(g_language, "./");
	
	AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_OK, NULL);
	//初始化网络设备库
#ifdef __DVR_ULTIMATE__
	CNdLibMan* pNdLibMan = CNdLibMan::Instance();
	if(!pNdLibMan->Initial("./"))
	{
		//可能加载不到库
		printf("Initial net device lib man fail!\n");
	}
	else
	{
		printf("Initial net device lib man succ\n");
	}
#endif

	//初始化主框架
	sreenMsgColor[0].pos = strlen("Check ");
	sreenMsgColor[0].len = strlen("disk");
	sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColorLen = 1;
	AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, "Check disk information.", sreenMsgColor, sreenMsgColorLen);

#ifdef __SUPPORT_3G__	
	/*cfgBlock.GetConfig(NCFG_ITEM_USE_3G, &pData, dataLen);
	unsigned long curNet3GTemp = *reinterpret_cast<const unsigned long *>(pData);
	if (1 == curNet3GTemp)
	{
		g_curNet3G = true;
	}*/

	//读取无线配置 enable
	/*CMyList <NCFG_ITEM_ID> tempitemList;
	tempitemList.AddTail(NCFG_ITEM_NET_WIRELESS);
	unsigned char *temp_pConfigData = NULL;
	unsigned long temp_dataLen = 0;
	if (!pConfig->GetConfigEx(tempitemList, &temp_pConfigData, temp_dataLen))
	{
		printf("Get config data fail!\n");
	}*/
	/*CConfigBlock tempCfgBlock;
	tempCfgBlock.CreateBlock(temp_pConfigData, temp_dataLen);*/

	unsigned char *temp_pData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_NET_WIRELESS, &pData, dataLen );
	MDVR_WIRELESS_CFG tempWirelesscfg = *reinterpret_cast<const MDVR_WIRELESS_CFG *>(pData);
	printf("tempWirelesscfg.enable=========%d\n", tempWirelesscfg.enable);
	if(0 == tempWirelesscfg.enable)
	{
		g_curNet3G = false;
	}
	else
	{
		g_curNet3G = true;
	}
#else
	g_curNet3G = false;
#endif
	

	CMainFrame *pMainFrame = new CMainFrame;
	if (!pMainFrame->Initial("./", "/mnt", subProductID))
	{
		printf("Initial main frame fail!\n");
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		printf("Initial main frame succ\n");
	}

    //在屏幕上刷硬盘信息
	{
		char szDiskInfoTemp[128] = {0};
		
		DVR_DISK_INFO_LIST dvrDiskInfoList;
		CDiskManager::Instance()->GetDeviceDiskInfo(NET_DISK_TYPE_FIXEDDISK, NULL, dvrDiskInfoList);
		//zxx新加获取开机硬盘个数
		int disknum=dvrDiskInfoList.GetCount();
		printf("disknum=%d\n",disknum);
		if(disknum==0)
		{
			//此时报警最好
			//pLocalDevice->SetOtherAlarm(OTHER_ALARM_TYPE_DISK_DISCONNET, true);
		}
		CDiskManager::Instance()->SetFirstDiskNum(disknum);

		snprintf(szDiskInfoTemp, 64, "Hard disk number is %d.", dvrDiskInfoList.GetCount());
		sreenMsgColor[0].pos = strlen("Hard ");
		sreenMsgColor[0].len = strlen("disk number");
		sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
		sreenMsgColor[1].pos = strlen("Hard disk number is ");
		sreenMsgColor[1].len = strlen(szDiskInfoTemp) - sreenMsgColor[1].pos - 1;
		sreenMsgColor[1].foreColor = COLOR_TEXT_FOCUS;
		sreenMsgColor[1].style = 0x1;
		sreenMsgColorLen = 2;
		AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_NONE, szDiskInfoTemp, sreenMsgColor, sreenMsgColorLen);
		PUB_Sleep(1000);

		int count = 1;
		POS pos = dvrDiskInfoList.GetHeadPosition();
		while (pos != NULL)
		{
			DVR_DISK_INFO diskInfo = dvrDiskInfoList.GetNext(pos);
			snprintf(szDiskInfoTemp, 64, "Check hard disk %d.", count++);
			
			sreenMsgColor[0].pos = strlen("Check hard ");
			sreenMsgColor[0].len = strlen(szDiskInfoTemp) - sreenMsgColor[0].pos;
			sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
			sreenMsgColorLen = 1;
			AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, szDiskInfoTemp, sreenMsgColor, sreenMsgColorLen);
			
			//PUB_Sleep(500);
			
			char strInfoTmp[64] = {0};
			snprintf(strInfoTmp, 64, "%s", diskInfo.diskname);
			strcpy(szDiskInfoTemp, "name:");
			sreenMsgColor[0].pos = strlen(szDiskInfoTemp);
			sreenMsgColor[0].len = strlen(strInfoTmp);
			sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
			strcat(szDiskInfoTemp, strInfoTmp);

			strcpy(strInfoTmp, "SATA");
			strcat(szDiskInfoTemp, " type:");
			sreenMsgColor[1].pos = strlen(szDiskInfoTemp);
			sreenMsgColor[1].len = strlen(strInfoTmp);
			sreenMsgColor[1].foreColor = COLOR_TEXT_FOCUS;
			sreenMsgColor[1].style = 0x1;
			strcat(szDiskInfoTemp, strInfoTmp);

			snprintf(strInfoTmp, 64, "%d MB", diskInfo.totalSpace);
			strcat(szDiskInfoTemp, " size:");
			sreenMsgColor[2].pos = strlen(szDiskInfoTemp);
			sreenMsgColor[2].len = strlen(strInfoTmp);
			sreenMsgColor[2].foreColor = COLOR_TEXT_FOCUS;
			sreenMsgColor[2].style = 0x1;	
			strcat(szDiskInfoTemp, strInfoTmp);
			strcat(szDiskInfoTemp, ".");

			sreenMsgColorLen = 3;
	
			//snprintf(szDiskInfoTemp, 64, "name:%s, type:SATA,size:%dMB", diskInfo.diskname, diskInfo.totalSpace);
			AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_NONE, szDiskInfoTemp, sreenMsgColor, sreenMsgColorLen);
		}
		dvrDiskInfoList.RemoveAll();
	}

	//去掉cdrom的检查。
	//在屏幕上刷光驱信息
	/*
	{
		sreenMsgColor[0].pos = strlen("Check ");
		sreenMsgColor[0].len = strlen("CD-ROM");
		sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
		sreenMsgColorLen = 1;
		AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, "Check CD-ROM information.", sreenMsgColor, sreenMsgColorLen);
		PUB_Sleep(1000);

		char szDiskInfoTemp[128] = {0};
		DVR_DISK_INFO_LIST dvrDiskInfoList;
		CDOperationMan::Instance()->GetDeviceDiskInfo(dvrDiskInfoList);
		snprintf(szDiskInfoTemp, 64, "CD-ROM number is %d.", dvrDiskInfoList.GetCount());
		sreenMsgColor[0].pos = strlen("CD-ROM number is ");
		sreenMsgColor[0].len = strlen(szDiskInfoTemp) - sreenMsgColor[0].pos - 1;
		sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
		sreenMsgColor[0].style = 0x1;
		sreenMsgColorLen = 1;
		AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_NONE, szDiskInfoTemp, sreenMsgColor, sreenMsgColorLen);
		//PUB_Sleep(1000);

		int count = 1;
		POS pos = dvrDiskInfoList.GetHeadPosition();
		while (pos != NULL)
		{
			DVR_DISK_INFO diskInfo = dvrDiskInfoList.GetNext(pos);
			snprintf(szDiskInfoTemp, 64, "Check CD-ROM %d.", count++);

			sreenMsgColor[0].pos = strlen("Check CD-ROM ");
			sreenMsgColor[0].len = strlen(szDiskInfoTemp) - sreenMsgColor[0].pos;
			sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
			sreenMsgColorLen = 1;
			AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, szDiskInfoTemp, sreenMsgColor, sreenMsgColorLen);

			//PUB_Sleep(500);

			char strInfoTmp[64] = {0};
			snprintf(strInfoTmp, 64, "%s", diskInfo.diskname);
			strcpy(szDiskInfoTemp, "name:");
			sreenMsgColor[0].pos = strlen(szDiskInfoTemp);
			sreenMsgColor[0].len = strlen(strInfoTmp);
			sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
			strcat(szDiskInfoTemp, strInfoTmp);

			strcpy(strInfoTmp, "CD-ROM");
			strcat(szDiskInfoTemp, " type:");
			sreenMsgColor[1].pos = strlen(szDiskInfoTemp);
			sreenMsgColor[1].len = strlen(strInfoTmp);
			sreenMsgColor[1].foreColor = COLOR_TEXT_FOCUS;
			sreenMsgColor[1].style = 0x1;
			strcat(szDiskInfoTemp, strInfoTmp);

			if (NET_DISK_NO_CD == diskInfo.diskStatus)
			{
				snprintf(strInfoTmp, 64, "No disc inserted", diskInfo.totalSpace);
				strcat(szDiskInfoTemp, " size:");
				sreenMsgColor[2].pos = strlen(szDiskInfoTemp);
				sreenMsgColor[2].len = strlen(strInfoTmp);
				sreenMsgColor[2].foreColor = COLOR_TEXT_FOCUS;
				sreenMsgColor[2].style = 0x1;	
				strcat(szDiskInfoTemp, strInfoTmp);
				strcat(szDiskInfoTemp, ".");
			}
			else
			{
				snprintf(strInfoTmp, 64, "%d MB", diskInfo.totalSpace);
				strcat(szDiskInfoTemp, " size:");
				sreenMsgColor[2].pos = strlen(szDiskInfoTemp);
				sreenMsgColor[2].len = strlen(strInfoTmp);
				sreenMsgColor[2].foreColor = COLOR_TEXT_FOCUS;
				sreenMsgColor[2].style = 0x1;	
				strcat(szDiskInfoTemp, strInfoTmp);
				strcat(szDiskInfoTemp, ".");
			}

			sreenMsgColorLen = 3;

			//snprintf(szDiskInfoTemp, 64, "name:%s, type:SATA,size:%dMB", diskInfo.diskname, diskInfo.totalSpace);
			AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_NONE, szDiskInfoTemp, sreenMsgColor, sreenMsgColorLen);

		}
		dvrDiskInfoList.RemoveAll();
	}
	PUB_Sleep(1000);
    */


#ifdef __DVR_BASIC__
	CSmtpMan::Instance()->ChangeLanguage();
#endif

	sreenMsgColor[0].pos = strlen("Check ");
	sreenMsgColor[0].len = strlen("Network Interface eth0");
	sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColorLen = 1;
	AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, "Check Network Interface eth0.", sreenMsgColor, sreenMsgColorLen);
	PUB_Sleep(1000);

#ifdef __NETSERVER_TVT__
	//初始化网络服务器
	NET_INIT_INFO netInitInfo;
	memset(&netInitInfo, 0, sizeof(NET_INIT_INFO));
	unsigned char *pPortData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_SERVER_PORT, &pPortData, dataLen);
	netInitInfo.nPort = *(reinterpret_cast<unsigned short *>(pPortData));
	cfgBlock.GetConfig(NCFG_ITEM_HTTP_PORT, &pPortData, dataLen);
	netInitInfo.nHttpPort = *(reinterpret_cast<unsigned short *>(pPortData));
	cfgBlock.GetConfig(NCFG_ITEM_DEVICE_NAME, &pData, dataLen );
	snprintf(netInitInfo.deviceName, sizeof(netInitInfo.deviceName), "%s", reinterpret_cast<const NCFG_INFO_SHORT_NAME *>(pData)->name);

	netInitInfo.dwMaxVideo		= pProduct->VideoInputNum();
	netInitInfo.dwMaxAudio		= pProduct->AudioInputNum();
	netInitInfo.dwMaxLink		= pProduct->MaxNetClientNum();
	netInitInfo.dwDeviceType	= 0;//pProduct->ProductType();
	netInitInfo.dwProductType	= pProduct->ProductType();
	netInitInfo.dwProductSubID	= pProduct->ProductSubID();
	char szDevVer[36] = {0};
	CLocalDevice::GetHardWareVersion(szDevVer, 36);
	const char *pVer = szDevVer;
	do 
	{
		if(isdigit(*pVer))
		{
			int a = 0, b = 0;
			char str[36] = {0};
			sscanf(pVer, "%d.%d.%s", &a, &b, str);
			netInitInfo.dwDevVer = ( (0xff000000 & (a << 24)) | (0x00ff0000 & (b << 16)) );
			break;
		}
		pVer++;
	} while( (pVer - szDevVer) < sizeof(szDevVer) );

	pVer = strstr(DVR_SOFT_VERSION_M, "=");
	if (NULL != pVer)
	{
		unsigned char len = strlen(pVer);
		if (len > strlen("3.0.0"))
		{
			unsigned long version = 0;

			//3.x.x
			netInitInfo.dwSoftVer |= (0xff000000 & (atoi(pVer+1) << 24));
			++ pVer;

			//x.0.x
			pVer = strstr(pVer, ".");
			netInitInfo.dwSoftVer |= (0x00ff0000 & (atoi(pVer+1) << 16));
			++ pVer;

			//x.x.0
			pVer = strstr(pVer, ".");
			netInitInfo.dwSoftVer |= (0x0000ff00 & (atoi(pVer+1) << 8) );
		}
	}

	struct tm tmBildtime = DVRTime32ToTm(GetBuildDate());
	netInitInfo.dwBuildTime	= ( tmBildtime.tm_year + 1900 )	 * 10000 + (tmBildtime.tm_mon + 1) * 100 + tmBildtime.tm_mday;

	
	CNetServer	*pNetServer = CNetServer::Instance();
	if(0 != pNetServer->Initial(&netInitInfo, pMsgMan, CProduct::Instance()->GetChnnSendFirstStreamNum(), CProduct::Instance()->SupportDoubleStream()))
	{
		printf("Initial network Server fail\n");
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		printf("Initial network Server succ\n");
	}

#ifdef __GB28181_SERVER__
	g_RTPStreamDisPatch.Initial();
	
	//LoadGb28181Config();
	CGb28181Server	*pGb28181Server=CGb28181Server::Instance();
 	if(0 != pGb28181Server->Initial(pMsgMan))
 	{
 		printf("Initial Gb28181Network Server fail\n");
 
 		return false;
 	}
 	else
 	{
 		printf("Initial network Server succ\n");
 	}


	/********************************/

	//初始化网络服务器

	Gb28181_INIT_INFO Gb28181InitInfo;
	memset(&Gb28181InitInfo, 0, sizeof(Gb28181_INIT_INFO));

	cfgBlock.GetConfig(NCFG_ITEM_DEVICE_NAME, &pData, dataLen );
	snprintf(Gb28181InitInfo.deviceName, sizeof(Gb28181InitInfo.deviceName), "%s", reinterpret_cast<const NCFG_INFO_SHORT_NAME *>(pData)->name);

	PRODUCT_INFO info;
	memset(&info,0,sizeof(PRODUCT_INFO));
	char szMac[32]={0};
	char szHardWare[32]={0};
	char szKernel[32]={0};
	char szMCU[32]={0};
	pProduct->GetProductInfo(info,szMac,szHardWare,szKernel,szMCU);

	Gb28181InitInfo.dwMaxVideo		= info.videoInputNum;
	Gb28181InitInfo.dwMaxAudio		= info.audioInputNum;
	Gb28181InitInfo.dwMaxALarm		= info.sensorInputNum+info.netVideoInputNum;
	Gb28181InitInfo.dwDeviceType	= 0;
	Gb28181InitInfo.dwProductType	= pProduct->ProductType();

	struct tm tmgbBildtime = DVRTime32ToTm(GetBuildDate());
	Gb28181InitInfo.dwBuildTime	= ( tmgbBildtime.tm_year + 1900 )	 * 10000 + (tmgbBildtime.tm_mon + 1) * 100 + tmgbBildtime.tm_mday;


	char szgbDevVer[36] = {0};
	CLocalDevice::GetHardWareVersion(szgbDevVer, 36);
	const char *pgbVer = szgbDevVer;

	pgbVer = strstr(DVR_SOFT_VERSION_M, "=");

	if (NULL != pgbVer)
	{	
		unsigned char len = strlen(pgbVer);
		if (len > strlen("3.0.0"))
		{
			unsigned long version = 0;

			//3.x.x
			Gb28181InitInfo.dwSoftVer |= (0xff000000 & (atoi(pgbVer+1) << 24));
			++ pgbVer;

			//x.0.x
			pgbVer = strstr(pgbVer, ".");
			Gb28181InitInfo.dwSoftVer |= (0x00ff0000 & (atoi(pgbVer+1) << 16));
			++ pgbVer;

			//x.x.0
			pgbVer = strstr(pgbVer, ".");
			Gb28181InitInfo.dwSoftVer |= (0x0000ff00 & (atoi(pgbVer+1) << 8) );
		}
	}
	pGb28181Server->SetGB28181Info(Gb28181InitInfo);

	/**********************************/
	unsigned char *pGb28181Data = NULL;	
	cfgBlock.GetConfig(NCFG_ITEM_GB28181_SERVER_INFO, &pGb28181Data, dataLen );
	GB28181_SIP_SERVER Gb28181Config = *reinterpret_cast<GB28181_SIP_SERVER*>(pGb28181Data);

	pGb28181Server->SetGB28181Cfg(Gb28181Config);


	unsigned char *pChannelData = NULL;	
	cfgBlock.GetConfig(NCFG_ITEM_GB28181_CHANNEL_ID, &pChannelData, dataLen );
	GB28181_CHANNEL_ID Gb28181ChannelID = *reinterpret_cast<GB28181_CHANNEL_ID*>(pChannelData);

	pGb28181Server->SetGb28181ChannelIDConfig(Gb28181ChannelID);


	unsigned char *pAlarmData = NULL;	
	cfgBlock.GetConfig(NCFG_ITEM_GB28181_ALARM_ID, &pAlarmData, dataLen);

	GB28181_ALARM_ID Gb28181AlarmID = *reinterpret_cast<GB28181_ALARM_ID*>(pAlarmData);

	pGb28181Server->SetGb28181AlarmIDConfig(Gb28181AlarmID);

	pGb28181Server->SetGb28181CatalogInfo(Gb28181ChannelID,Gb28181AlarmID);
	pGb28181Server->SetGb28181DeviceStatusInfo(Gb28181ChannelID,Gb28181AlarmID);
#endif

#ifdef __TUTK_SERVER__
	g_tutkStreamDispatch.Initial();
#endif

#ifdef __API_SERVER__
	g_apiStreamDispatch->Initial();
#endif

	{
		NET_AUTO_REPORT netAutoReport;
		memset(&netAutoReport, 0, sizeof(NET_AUTO_REPORT));
		pPortData = NULL;
		cfgBlock.GetConfig(NCFG_ITEM_AUTO_REPORT, &pPortData, dataLen);
		assert(dataLen == sizeof(NET_AUTO_REPORT));
		NET_AUTO_REPORT *pNetAutoReport = reinterpret_cast<NET_AUTO_REPORT *>(pPortData);
		snprintf(netAutoReport.host, 256, "%s", pNetAutoReport->host);
		netAutoReport.dwPort = pNetAutoReport->dwPort;
		netAutoReport.ID = pNetAutoReport->ID;
		netAutoReport.bUse = pNetAutoReport->bUse;

		printf("%s,%d,host:%s,dwPort:%d,ID:%d\n",__FILE__,__LINE__,netAutoReport.host,netAutoReport.dwPort, netAutoReport.ID);

		pNetServer->AutoReportInitial(CProduct::Instance()->AutoReport(), &netAutoReport);	



	}

	{
		PRODUCT_INFO productInfo;
		char macTmp[8] = {0};
		char szHardwareVersion[36] = {0};
		char szKernelVersion[64] = {0};
		char szMCUVersion[36] = {0};
#ifdef __ENVIRONMENT_LINUX__
		CNetInterface::Instance()->GetMAC(macTmp);
#endif
		{
			char szTemp1[36] = {0};
			char szTemp2[36] = {0};
			CProduct::Instance()->GetHardwarePreVersion(szTemp1, 36);
			CLocalDevice::GetHardWareVersion(szTemp2, 36);				
			snprintf(szHardwareVersion, 36, "%s-%s", szTemp1, szTemp2);
			CLocalDevice::GetKernelVersion(szKernelVersion, 64);
			CLocalDevice::Instance()->GetMCUVersion(szMCUVersion, 36);
		}
		CProduct::Instance()->GetProductInfo(productInfo, macTmp, szHardwareVersion, szKernelVersion, szMCUVersion);

		pNetServer->SetMaxFirstStreamSendNum(CProduct::Instance()->SupportLimitFirstStreamSendNum(), CProduct::Instance()->GetDVRSendFirstStreamNum());
		pNetServer->SetMaxIPCTransferChnnNum(productInfo.netFirstStreamNum);
	}


	//生产二维码
	#if 0
	#if defined __CUSTOM_ZNV__ && !defined __ZNV_SC__
		#ifdef __ENVIRONMENT_LINUX__		
			char sn[40];
			CLocalDevice::Instance()->GetSNFromFlash(sn);
			CRawToBmp rawBmp(sn);
			rawBmp.WriteSNToJSFile("/mnt/mtd/WebServer/logo/qr.js");
			rawBmp.CreateBmp("/mnt/mtd/WebServer/logo/qr.bmp");
		#endif
	#endif
	#endif

	
	char str[128]={0};
	#if defined (__CUSTOM_ZNV__) && (!defined (__ZNV_SC__) || defined(__ZNV_WNRL__))
		#ifdef	__ENVIRONMENT_LINUX__
			char tmpStr[120] = {0};
			CLocalDevice::Instance()->GetSNFromFlash(tmpStr);
			snprintf(str, 128, "<sn>%s</sn>", tmpStr);
		#endif
	#else
		//读取系统信息并组装字符串
		NETWORK_STATUS_INFO networkStatusInfo;
		const int verNum = 3;//3.0的版本号 --> 参看头文件InfoManDlg.h
		char user[36] = "admin";

		memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));

		unsigned char *ptmpData = new unsigned char [sizeof(NETWORK_STATUS_INFO)];
		memset(ptmpData, 0, sizeof(NETWORK_STATUS_INFO));

		#ifdef __ENVIRONMENT_LINUX__
				CNetInterface::Instance()->GetLocalNICStatusInfo(*reinterpret_cast<NETWORK_STATUS_INFO*>(ptmpData));
		#else
				NETWORK_STATUS_INFO *pNetStatus = reinterpret_cast<NETWORK_STATUS_INFO *>(ptmpData);
				pNetStatus->bDHCP = true;
				pNetStatus->bDDNS = true;
				pNetStatus->DNS1 = IP_ADDRESS(192, 168, 11, 1);
				pNetStatus->gateway = IP_ADDRESS(192, 168, 11, 1);
				pNetStatus->IP = IP_ADDRESS(192, 168, 11, 61);
				pNetStatus->subMask = IP_ADDRESS(255, 255, 255, 0);
				pNetStatus->serverPort = 2000;
				pNetStatus->httpPort = 80;
		#endif
		memcpy(&networkStatusInfo, ptmpData, sizeof(NETWORK_STATUS_INFO));

		snprintf(str,128,"<sn>%02X%02X%02X%02X%02X%02X</sn>",networkStatusInfo.MAC[0],networkStatusInfo.MAC[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);
		snprintf(str+strlen(str),128-strlen(str), "<user>%s</user>",user);
		snprintf(str+strlen(str),128-strlen(str), "<ip>%03d.%03d.%03d.%03d</ip>",(networkStatusInfo.IP&0xff), (networkStatusInfo.IP>>8)&0xff, (networkStatusInfo.IP>>16)&0xff, (networkStatusInfo.IP>>24)&0xff);
		snprintf(str+strlen(str),128-strlen(str), "<port>%d</port>",networkStatusInfo.httpPort);
		snprintf(str+strlen(str),128-strlen(str), "<ver>%d</ver>",verNum);
		
		delete [] ptmpData;
		ptmpData = NULL;
	#endif
	CRawToBmp rawBmp(str);
	rawBmp.WriteSNToJSFile("/mnt/mtd/WebServer/logo/qr.js");
	rawBmp.CreateBmp("/mnt/mtd/WebServer/logo/qr.bmp");


	//初始化Web服务器
	CWebServer	*pWebServer = CWebServer::Instance();
	if(!pWebServer->Initial(pProduct->VideoInputNum(), "./WebServer"))
	{
		printf("Initial Web Server fail\n");
		DVRSystem("exit");
		exit(0);
	}
	else
	{
		printf("Initial Web Server succ\n");
	}
#endif
	
#ifdef __NETSERVER_HXHT__
	//初始化HXHT
	HX_NET_INIT_INFO hxNetInitInfo;
	memset(&hxNetInitInfo, 0, sizeof(HX_NET_INIT_INFO));

	unsigned char *pHxNetData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_HXHT_NET_PARAM, &pHxNetData, dataLen);
	HXHT_NET_PARAM_INFO *pHxNetParmaInfo = (HXHT_NET_PARAM_INFO *)(pHxNetData);

	hxNetInitInfo.nMsgPort		= pHxNetParmaInfo->usVDevDataPort;
	hxNetInitInfo.nVideoPort	= pHxNetParmaInfo->usVDevDataPort;
	hxNetInitInfo.nAudioPort	= pHxNetParmaInfo->usVDevDataPort;
	hxNetInitInfo.nUpdatePort	= pHxNetParmaInfo->usVDevDataPort;
	hxNetInitInfo.nWebPort		= pHxNetParmaInfo->usVDevDataPort;

#if defined(__TDFH__) || defined(__CHIPTI__)
	snprintf(hxNetInitInfo.szCsgIP, sizeof(hxNetInitInfo.szCsgIP), "%d.%d.%d.%d",
		(pHxNetParmaInfo->csIP & 0xff),((pHxNetParmaInfo->csIP >> 8) & 0xff), ((pHxNetParmaInfo->csIP >> 16) & 0xff), ((pHxNetParmaInfo->csIP >> 24) & 0xff));

#else
	snprintf(hxNetInitInfo.szCsgIP, sizeof(hxNetInitInfo.szCsgIP), "%03d.%03d.%03d.%03d",
		(pHxNetParmaInfo->csIP & 0xff),((pHxNetParmaInfo->csIP >> 8) & 0xff), ((pHxNetParmaInfo->csIP >> 16) & 0xff), ((pHxNetParmaInfo->csIP >> 24) & 0xff));

#endif
	
	hxNetInitInfo.nCsgPort = pHxNetParmaInfo->csMessagePort;
	hxNetInitInfo.nStorePort = pHxNetParmaInfo->csStortPort;
	snprintf(hxNetInitInfo.szDeviceID, sizeof(hxNetInitInfo.szDeviceID), "%s", pHxNetParmaInfo->videoDeviceID);
	CNetInterface::Instance()->GetIPText(hxNetInitInfo.szLocalIP, sizeof(hxNetInitInfo.szLocalIP), "eth0");

	if (strlen(hxNetInitInfo.szDeviceID))
	{
		snprintf(hxNetInitInfo.szStoreID, sizeof(hxNetInitInfo.szStoreID), "%s_T", hxNetInitInfo.szDeviceID);
	}
	

	hxNetInitInfo.dwMaxVideo = pProduct->VideoInputNum();
	hxNetInitInfo.dwMaxAudio = pProduct->AudioInputNum();
	hxNetInitInfo.dwMaxLink  = pProduct->MaxNetClientNum() * hxNetInitInfo.dwMaxVideo;

	hxNetInitInfo.dwMaxSensorIn = pProduct->SensorInputNum();
	hxNetInitInfo.dwMaxSensorOut= pProduct->SensorOutputNum();

	if (!CHxConnectMan::Instance()->Initial(hxNetInitInfo))
	{
		printf("%s:%s:%d, Initial hxht server fail\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;
	}
	else
	{
		printf("%s:%s:%d, Initial hxht server succ\n", __FUNCTION__, __FILE__, __LINE__);
	}

	//初始化处理远程升级的类
	{
		NET_CFGUP_INIT_INFO netUpInfo;
		memset(&netUpInfo, 0, sizeof(NET_CFGUP_INIT_INFO));

		printf("%s,%d\n",__FILE__,__LINE__);
		unsigned char *pPortData = NULL;
		cfgBlock.GetConfig(NCFG_ITEM_SERVER_PORT, &pPortData, dataLen);
		netUpInfo.nPort = *(reinterpret_cast<unsigned short *>(pPortData));

		printf("%s,%d\n",__FILE__,__LINE__);
		cfgBlock.GetConfig(NCFG_ITEM_HTTP_PORT, &pPortData, dataLen);
		netUpInfo.nHttpPort = *(reinterpret_cast<unsigned short *>(pPortData));	

		printf("%s,%d\n",__FILE__,__LINE__);
		cfgBlock.GetConfig(NCFG_ITEM_DEVICE_NAME, &pData, dataLen );
		snprintf(netUpInfo.deviceName, sizeof(netUpInfo.deviceName), "%s", reinterpret_cast<const NCFG_INFO_SHORT_NAME *>(pData)->name);
		netUpInfo.dwDeviceType = 0;
		netUpInfo.dwProductType =  pProduct->ProductType();
		netUpInfo.dwProductSubID = pProduct->ProductSubID();
		char szDevVer[36] = {0};
		CLocalDevice::GetHardWareVersion(szDevVer, 36);
		const char *pVer = szDevVer;
		do 
		{
			if(isdigit(*pVer))
			{
				int a = 0, b = 0;
				char str[36] = {0};
				sscanf(pVer, "%d.%d.%s", &a, &b, str);
				netUpInfo.dwDevVer = ( (0xff000000 & (a << 24)) | (0x00ff0000 & (b << 16)) );
				break;
			}
			pVer++;
		} while( (pVer - szDevVer) < sizeof(szDevVer) );

		pVer = strstr(DVR_SOFT_VERSION_M, "=");
		if (NULL != pVer)
		{
			unsigned char len = strlen(pVer);
			if (len > strlen("3.0.0"))
			{
				unsigned long version = 0;

				//3.x.x
				netUpInfo.dwSoftVer |= (0xff000000 & (atoi(pVer+1) << 24));
				++ pVer;

				//x.0.x
				pVer = strstr(pVer, ".");
				netUpInfo.dwSoftVer |= (0x00ff0000 & (atoi(pVer+1) << 16));
				++ pVer;

				//x.x.0
				pVer = strstr(pVer, ".");
				netUpInfo.dwSoftVer |= (0x0000ff00 & (atoi(pVer+1) << 8) );
			}
		}

		struct tm tmBildtime = DVRTime32ToTm(GetBuildDate());
		netUpInfo.dwBuildTime	= ( tmBildtime.tm_year + 1900 )	 * 10000 + (tmBildtime.tm_mon + 1) * 100 + tmBildtime.tm_mday;

		CCfgUpNetServer	*pCfgUpNetServer = CCfgUpNetServer::Instance();
		if(0 != pCfgUpNetServer->Initial(&netUpInfo, pMsgMan))
		{
			printf("####Initial CCfgUpNetServer  fail\n");
			
		}
		else
		{
			printf("Initial CCfgUpNetServer succ\n");
		}
	}
	
#endif

	sreenMsgColor[0].pos = strlen("Network Interface ");
	sreenMsgColor[0].len = strlen("eth0 is Ok");
	sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColorLen = 1;
	AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_NONE, "Network Interface eth0 is Ok.", sreenMsgColor, sreenMsgColorLen);
	PUB_Sleep(1000);


	//启动服务
	sreenMsgColor[0].pos = strlen("Start system ");
	sreenMsgColor[0].len = strlen("services");
	sreenMsgColor[0].foreColor = COLOR_TEXT_FOCUS;
	sreenMsgColorLen = 1;
	AddFlashScreenMsg(++screenMsgID, FLASH_SCREEN_MSG_WAITING, "Start system services.", sreenMsgColor, sreenMsgColorLen);
	PUB_Sleep(1000);

	//////////////////////////////////////////////////////////////////////////
	//

	if (pMainFrame->Start())
	{
		printf("Start main frame succ\n");
	}
	else
	{
		printf("Start main frame fail\n");
		DVRSystem("exit");
		exit(0);
	}
	pMainFrame->SetEncodeAgain();	
   

#ifdef __NETSERVER_TVT__
	if (-1 != pNetServer->Start())
	{
		printf("Start network Server succ\n");
	}
	else
	{
		printf("Start network Server fail\n");
		DVRSystem("exit");
		exit(0);
	}

#ifdef __GB28181_SERVER__
	g_RTPStreamDisPatch.Start();
 	if (-1 != CGb28181Server::Instance()->Start())
 	{
 		printf("Start Gb28181Network Server succ\n");
 	}
 	else
 	{
 		printf("Start Gb28181Network Server fail\n");
 
 		return false;
 	}
#endif

#ifdef __TUTK_SERVER__
	g_tutkStreamDispatch.Start();
#endif

#ifdef __API_SERVER__
	g_apiStreamDispatch->Start();
	CTvtAlarm::Instance()->Start();
#endif

	{
		PRODUCT_TYPE productTypeTmp = CProduct::Instance()->ProductType();
		if ((TD_2304CDS == productTypeTmp) || (TD_2308CDS == productTypeTmp) || (TD_2304HS == productTypeTmp) 
			|| (TD_2304SE == productTypeTmp) || (TD_2308SE == productTypeTmp)
			|| (TD_2316ME_A == productTypeTmp) || (TD_2308ME_A == productTypeTmp) || (TD_2304ME_A == productTypeTmp)  || (TD_2504HE_B == productTypeTmp) || (TD_2508HE_L == productTypeTmp)
			|| (TD_2304SS_B == productTypeTmp) || (TD_2308SS_B == productTypeTmp) || (TD_2316SS_B == productTypeTmp) 
			|| (TD_2304SS_C == productTypeTmp) || (TD_2308SS_C == productTypeTmp) || (TD_2316SS_C == productTypeTmp)
			|| (TD_2304SL_C == productTypeTmp) || (TD_2308SL_C == productTypeTmp) 
			|| (TD_2304SE_B == productTypeTmp) || (TD_2308SE_B == productTypeTmp) || (TD_2316SE_B == productTypeTmp)
			|| (TD_2304SE_C == productTypeTmp) || (TD_2308SE_C == productTypeTmp) || (TD_2316SE_C == productTypeTmp)
			|| (TD_2304ME_C == productTypeTmp) || (TD_2308ME_C == productTypeTmp) || (TD_2316ME_C == productTypeTmp)
			|| (TD_2508HE_C_3521 == productTypeTmp) || (TD_2508HS_BL == productTypeTmp) || (TD_2516HS_BL == productTypeTmp)
			|| (TD_2304SS_SL == productTypeTmp) || (TD_2308SS_SL == productTypeTmp)
			|| (TD_2304SS_SDI == productTypeTmp) || (TD_2308SS_SDI == productTypeTmp)||(TD_2304SS_SL_3515A == productTypeTmp)
			|| (TD_2508HE_SDI == productTypeTmp) || (TD_2704XE_S == productTypeTmp) || (TD_2704XS_SL == productTypeTmp)
			|| (TD_2704AS_SL == productTypeTmp) || (TD_2704AS_SL_A == productTypeTmp) || (TD_2704AS_PL == productTypeTmp) || (TD_2704TS_S == productTypeTmp) || (TD_2704TS_PL == productTypeTmp)
			|| (TD_2704AS_S == productTypeTmp) || (TD_2708AS_S == productTypeTmp) || (TD_2708AS_S_A == productTypeTmp) || (TD_2008AS == productTypeTmp)
			|| (TD_2804NS_CL == productTypeTmp) || (TD_2808NS_CL == productTypeTmp)
			|| (TD_3004D1_4P == productTypeTmp) || (TD_3008D1_8P == productTypeTmp)
			|| (TD_2704AS_S_A == productTypeTmp) || (TD_2708AS_S_A == productTypeTmp))
		{
			DVRSystem("rm -rf /nfsdir/language");
			DVRSystem("mkdir -p /nfsdir/language");
		}
		else
		{
			if (pWebServer->Start(netInitInfo.nHttpPort, netInitInfo.nPort))
			{
				printf("Start Web Server succ\n");
			}
			else
			{
				printf("Start Web Server fail\n");
				DVRSystem("exit");
				exit(0);
			}
		}
	}
#endif
#ifdef __CHIP3531__
	netDaemon.Start();
#endif

	if (pLocalDevice->Start())
	{
		printf("Start device succ\n");
	}
	else
	{
		printf("Start device fail\n");
		DVRSystem("exit");
		exit(0);
	}

#ifdef __NETSERVER_HXHT__
	if (CHxConnectMan::Instance()->Start())
	{
		printf("%s:%s:%d, start hxht OK\n", __FUNCTION__, __FILE__, __LINE__);
	}
	else
	{
		printf("%s:%s:%d, start hxht error\n", __FUNCTION__, __FILE__, __LINE__);
	}


	if (-1 != CCfgUpNetServer::Instance()->Start())
	{
		printf("Start CCfgUpNetServer  succ\n");
	}
	else
	{
		printf("Start CCfgUpNetServer fail\n");		
	}
#endif

#ifdef __SUPPORT_HRSST__
	CPhoneSync::Instance()->Start();
#endif

	//恢复手动录像
	cfgBlock.GetConfig(NCFG_ITEM_MANUAL_REC, &pData, dataLen);
	ULONGLONG manualRec = *(reinterpret_cast<ULONGLONG *>(pData));
	if (0 != manualRec)
	{
		//如果为零，表示不需要开启，不能发送命令，因为全为零的命令表示停止手动录像。
		pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_MANUAL_REC, (unsigned char*)&manualRec, sizeof(ULONGLONG));
	}

	//启动是否需要向导
	cfgBlock.GetConfig(NCFG_ITEM_SHOW_WIZZARD, &pData, dataLen);
	g_showWizzard = *(reinterpret_cast<unsigned long *>(pData));

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//先把按键盘的类型设入
	CKeyProc::SetKeyboardType(static_cast<KEYBOARD_TYPE>(pProduct->LocalKeyboardType()));
	//设置视频通道个数，在按键值转换的时候使用
	CKeyProc::SetVideoInputNum(pProduct->VideoInputNum());

	char strShutDown [512] = {0}; //用于记录关闭电源的提示字符串。

	//把所有的初始化并启动后，开始初始化窗体
	CLocalUIMan *pLocalUIMan = CLocalUIMan::Instance();

	cfgBlock.GetConfig(NCFG_ITEM_DODWELL, &pData, dataLen);
	g_p_wnd_startup_info->DoDwell = *reinterpret_cast<unsigned long*>(pData);

	cfgBlock.GetConfig(NCFG_ITEM_TOOLBAR_POS, &pData, dataLen);
	memcpy(&(g_p_wnd_startup_info->ToolbarPos), pData, sizeof(NCFG_INFO_TOOLBAR_POS));

	cfgBlock.GetConfig(NCFG_ITEM_VIEW_SPLIT_MODE, &pData, dataLen);
	g_p_wnd_startup_info->SplitMode = *reinterpret_cast<unsigned long*>(pData);

	for(int i = 0; i < 7; i++)
	{
		cfgBlock.GetConfig(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_VIEW_SPLIT_1X1 + i), &pData, dataLen);
		memcpy(g_p_wnd_startup_info->SplitData[ViewSplitToIndex(NCFG_ITEM_ID(NCFG_ITEM_VIEW_SPLIT_1X1 + i))], pData, dataLen);
	}

	cfgBlock.GetConfig(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_VIEW_SPLIT_4X6), &pData, dataLen);
	memcpy(g_p_wnd_startup_info->SplitData[ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_4X6)], pData, dataLen);


	memcpy(g_p_cur_startup_info, g_p_wnd_startup_info, sizeof(WND_STARTUP_INFO));

	//////////////////////////////////////////////////////////////////////////
	cfgBlock.DestroyBlock();
	//////////////////////////////////////////////////////////////////////////

	bool bIniting = true;	//表示正在初始化，以防止下面的FB重复初始化。

	//最后结束屏幕消息
	AddFlashScreenMsg(screenMsgID, FLASH_SCREEN_MSG_OK, NULL);

	//初始设置语序
#if defined(__CUSTOM_IN25__) || defined(__CUSTOM_GB31__) || defined(__CUSTOM_PL14__)
	if((LANGUAGE_HEBREW == g_language)||(LANGUAGE_PERSIAN == g_language))
#else
	if((LANGUAGE_HEBREW == g_language) || (LANGUAGE_PERSIAN == g_language) || (LANGUAGE_FINNISH == g_language))
#endif
	{
		PUB_Sleep(1000);
		g_langRightOrder = true;
	}
	else
	{
		g_langRightOrder = false;
	}
#if defined(__SUPPORT_SNAPD1__)
	CShanghaiConnectMan *pShanghaiConnectMan =CShanghaiConnectMan::Instance();
	pShanghaiConnectMan->SetStartHeartProc();
#endif
	bool bFirStart = true;
	bool bHaveReadFlash = false;
	char szHardwareVersion[36] = {0};
	char szKernelVersion[64] = {0};
	char szMCUVersion[36] = {0};

	//检查第一次VI变化是否结束
	pMainFrame->FirstCheckVIOver();

	if (!bUseUi)
	{
		int runingtime = 2;
		int sleepCount = runingtime * 60 * 50;
		do
		{
			PUB_Sleep(20);
			sleepCount--;
		} while(0 < sleepCount);
		printf("over---------\n");
		goto endui;
	}
	
	do 
	{
		g_system_status = DVR_SYSTEM_RUNING;
		printf("1761 xdvr\n");
		if (!bIniting)
		{
			//获取输出设备和分辨率
			int outDevice = VIDEO_OUT_VGA;			
			{
				if ((VGA_1920X1080 <= g_resolution) && (VIDEO_OUT_DEVICE_HDMI & pProduct->VideoOutDeviceMask()))
				{
					if(340 == g_support_hd_type)
					{
						g_resolution =  (VIDEO_FORMAT_NTSC != g_video_format) ? VGA_1440X1152 : VGA_1440X960;
					}
					outDevice = VIDEO_OUT_HDMI;
				}
				else if (VGA_800X600 <= g_resolution)
				{
					outDevice = VIDEO_OUT_VGA;
					if (VGA_1920X1080 == g_resolution)
					{
						g_resolution =  VGA_1024X768;
					}
				}
				else
				{
					outDevice = VIDEO_OUT_CVBS;
					g_resolution =  (VIDEO_FORMAT_NTSC != g_video_format) ? VGA_720X576 : VGA_720X480;
				}
			}
			videoOutMode = outDevice;

			GetVGAResolution(g_resolution, width, height);
			pLocalDevice->ChangeOutpuDevice(outDevice, width, height);

			//把配置写入flash
			{
				bool bCanWrite = true;
				PRODUCT_TYPE productTypeVideo = CProduct::Instance()->ProductType();
				if ((productTypeVideo >= TD_2704HD) && (productTypeVideo <= TD_2716XD_L))
				{
					if (VIDEO_OUT_CVBS == videoOutMode)
					{
						bCanWrite = false;
					}					
				}
				if (bCanWrite)
				{
					char szTmp[8] = {0};
					sprintf(szTmp, "%d", videoOutMode);
					CLocalDevice::WriteDisplayModToFlash(szTmp, 1);
					printf("!!!!!!!!!!!!!!!!!%s,%d,szTmp:%s\n",__FILE__,__LINE__,szTmp);

					memset(szTmp, 0, sizeof(szTmp));
					snprintf(szTmp, 8, "%04x", g_resolution);
					CLocalDevice::WriteDisplayResToFlash(szTmp, 4);
					printf("!!!!!!!!!!!!!!!!!%s,%d,szTmp:%s\n",__FILE__,__LINE__,szTmp);
				}		
			}

			bool ret = GUI::CFBInterface::Instance()->Initial("./", width, height, pProduct->LocalVideoInputNum(), pProduct->VideoInputNum(), \
				pProduct->VideoOutputNum(), pProduct->NetVideoInputNum());
			if (!ret)
			{
				printf("Start CFBInterface fail\n");
				DVRSystem("exit");
				exit(0);
			}

			GUI::CFBInterface::Instance()->SetTransparency(TRANSPARENCY_0);
			CStringTable::Instance()->Initial(g_language, "./");
#if defined(__CUSTOM_IN25__) || defined(__CUSTOM_GB31__) || defined(__CUSTOM_PL14__)
			if((LANGUAGE_HEBREW == g_language)||(LANGUAGE_PERSIAN == g_language))
#else
			if((LANGUAGE_HEBREW == g_language) || (LANGUAGE_PERSIAN == g_language) || (LANGUAGE_FINNISH == g_language))
#endif
			{
				g_langRightOrder = true;
			}
			else
			{
				g_langRightOrder = false;
			}
#ifdef __DVR_BASIC__
			CSmtpMan::Instance()->ChangeLanguage();
#endif

			pLocalDevice->SetBuzzerStatus(true);
			PUB_Sleep(1000);
			pLocalDevice->SetBuzzerStatus(false);
		}

		if (!pLocalUIMan->Initial(static_cast<LANGUAGE>(g_language), "./", width, height, g_dateMode, g_timeMode))
		{
			printf("Start local window fail\n");
			DVRSystem("exit");
			exit(0);
		}
		else
		{
			printf("Start local window succ\n");
		}

		PRODUCT_INFO productInfo;
		{
			char macTmp[8] = {0};
#ifdef __ENVIRONMENT_LINUX__
			CNetInterface::Instance()->GetMAC(macTmp);
#endif
			if (bHaveReadFlash)
			{
				
			}
			else
			{
				char szTemp1[36] = {0};
				char szTemp2[36] = {0};
				CProduct::Instance()->GetHardwarePreVersion(szTemp1, 36);
				CLocalDevice::GetHardWareVersion(szTemp2, 36);				
				snprintf(szHardwareVersion, 36, "%s-%s", szTemp1, szTemp2);
				CLocalDevice::GetKernelVersion(szKernelVersion, 64);
				CLocalDevice::Instance()->GetMCUVersion(szMCUVersion, 36);
				bHaveReadFlash = true;
			}			
			CProduct::Instance()->GetProductInfo(productInfo, macTmp, szHardwareVersion, szKernelVersion, szMCUVersion);
		}
		

		pLocalUIMan->SetProductInfo(productInfo);

		pLocalUIMan->SetResolution(width, height);
		pLocalDevice->SetResolution(width, height);

		//2010-02-03 18:12:00 YSW
		//原来DisplayMan里在处理未初始化时也会处理业务，后来增加了是否初始化完成的标记（该标记通过SetResolution才得以修改）
		//所以把下面这段原本在上面的代码移到下面来
		pMainFrame->SetResolution(videoOutMode, width, height);
		//当使用配置的分辨率去初始化显示设备可能不成功，所以我们应该根据其返回的结果来初始化界面的一部分东西。
		//end 2010-02-03 18:12:00 YSW

		pLocalUIMan->Start();

		//启动了闪屏,关闭闪屏
		if(bIniting && (PUB_THREAD_ID_NOINIT != flashScreenID))
		{		
			PUB_ExitThread(&flashScreenID, &g_bFlashScreenRun);
			printf("Stop flash screen!\n");
		}
		bIniting = false;//以防止FB重复初始化。

		//在没有在菜单的显示设备上，进行提示
#if defined(__TDFH__)
		SetDeviceNomenuImage(g_language);
#endif
		CIPCDecDisplayMan::Instance()->Start();
		
		{
			PRODUCT_TYPE productTypeTmp = CProduct::Instance()->ProductType();
			if ((TD_2304CDS == productTypeTmp) || (TD_2308CDS == productTypeTmp) || (TD_2304HS == productTypeTmp)
				|| (TD_2304SE == productTypeTmp) || (TD_2308SE == productTypeTmp)
				|| (TD_2316ME_A == productTypeTmp) || (TD_2308ME_A == productTypeTmp) || (TD_2304ME_A == productTypeTmp) || (TD_2504HE_B == productTypeTmp) || (TD_2508HE_L == productTypeTmp)
				|| (TD_2304SS_B == productTypeTmp) || (TD_2308SS_B == productTypeTmp) || (TD_2316SS_B == productTypeTmp) 
				|| (TD_2304SS_C == productTypeTmp) || (TD_2308SS_C == productTypeTmp) || (TD_2316SS_C == productTypeTmp)
				|| (TD_2304SL_C == productTypeTmp) || (TD_2308SL_C == productTypeTmp)
				|| (TD_2304SE_B == productTypeTmp) || (TD_2308SE_B == productTypeTmp) || (TD_2316SE_B == productTypeTmp)
				|| (TD_2304SE_C == productTypeTmp) || (TD_2308SE_C == productTypeTmp) || (TD_2316SE_C == productTypeTmp)
				|| (TD_2304ME_C == productTypeTmp) || (TD_2308ME_C == productTypeTmp) || (TD_2316ME_C == productTypeTmp)
				|| (TD_2508HE_C_3521 == productTypeTmp) || (TD_2508HS_BL == productTypeTmp) || (TD_2516HS_BL == productTypeTmp)
				|| (TD_2304SS_SL == productTypeTmp) || (TD_2308SS_SL == productTypeTmp)
				|| (TD_2304SS_SDI == productTypeTmp) || (TD_2308SS_SDI == productTypeTmp)|| (TD_2304SS_SL_3515A == productTypeTmp)
				|| (TD_2508HE_SDI == productTypeTmp) || (TD_2704XE_S == productTypeTmp)  || (TD_2704XS_SL == productTypeTmp)
				|| (TD_2704AS_SL == productTypeTmp) || (TD_2704AS_SL_A == productTypeTmp) || (TD_2704AS_PL == productTypeTmp)|| (TD_2704TS_S == productTypeTmp) || (TD_2704TS_PL == productTypeTmp)
				|| (TD_2704AS_S == productTypeTmp) || (TD_2708AS_S == productTypeTmp) || (TD_2708AS_S_A == productTypeTmp) || (TD_2008AS == productTypeTmp)
				|| (TD_2804NS_CL == productTypeTmp) || (TD_2808NS_CL == productTypeTmp)
				|| (TD_3004D1_4P == productTypeTmp) || (TD_3008D1_8P == productTypeTmp)
				|| (TD_2704AS_S_A == productTypeTmp) || (TD_2708AS_S_A == productTypeTmp)
				|| (TD_2704AS_SL_A == productTypeTmp))
			{
				if (bFirStart)
				{
					bFirStart = false;
#ifdef __NETSERVER_TVT__
					if (pWebServer->Start(netInitInfo.nHttpPort, netInitInfo.nPort))
					{
						printf("Start Web Server succ\n");
					}
					else
					{
						printf("Start Web Server fail\n");
						DVRSystem("exit");
						exit(0);
					}
#endif
				}
			}
		}


		pLocalUIMan->DoModal();

		CIPCDecDisplayMan::Instance()->Stop();

		//关闭DisplayMan中设置分辨率
		pMainFrame->SetResolution(videoOutMode,width, height, false);

		pLocalUIMan->Stop();

		//如果是需要重绘界面，则反初始化界面和FB，以备再次初始化。
		//如果不是重绘，则把该动作留到后面做，因为关机需要提示关闭电源，不能在未完成所有的反初始化之前提示。
		if (DVR_SYSTEM_REDRAW == g_system_status)
		{
			//在长按ESC键，进行切换输出设备时。如果设备不支持高清，在VGA和CVBS之间切换。
			if (g_bChangOutDevice)
			{
				if (g_resolution < VGA_800X600)
				{
// #ifdef __CUSTOM_BR27__
// 					g_resolution = VGA_800X600;
// #else
					if (pProduct->IsHardwareHasS7())
					{
						g_resolution = VGA_800X600;
					}
					else
					{
						if (TD_7004SB == productID || TD_7008SB == productID)
						{
							g_resolution = VGA_800X600;
						}
						else
						{
							g_resolution = VGA_1024X768;
							if (!(g_resolution & pProduct->DisplayResolutionMask()))
							{
								g_resolution = VGA_1280X1024;
							}
						}
					}
//#endif
				}
				else if ((g_resolution < VGA_1920X1080))
				{
					if(VIDEO_OUT_DEVICE_HDMI & pProduct->VideoOutDeviceMask())
					{
						if(g_support_hd_type==340)
						{
							g_resolution =  (VIDEO_FORMAT_NTSC != g_video_format) ? VGA_1440X1152 : VGA_1440X960;
						}
						else
						{
							g_resolution = VGA_1920X1080;
						}
					}
					else
					{
						g_resolution = VGA_640X480;
					}					
				}
				else
				{
					g_resolution = VGA_640X480;
					if (!(VIDEO_OUT_DEVICE_CVBS & pProduct->VideoOutDeviceMask()))
					{
						//3531不支持CVBS输出,从高清切换到VGA
						if (pProduct->IsHardwareHasS7())
						{
							g_resolution = VGA_800X600;
						}
						else
						{
							if (TD_7004SB == productID || TD_7008SB == productID)
							{
								g_resolution = VGA_800X600;
							}
							else
							{
								g_resolution = VGA_1024X768;
								if (!(g_resolution & pProduct->DisplayResolutionMask()))
								{
									g_resolution = VGA_1280X1024;
								}
							}
						}
					}
				}
				g_bChangOutDevice = false;
			}

			
			pLocalUIMan->Quit();

			GUI::CFBInterface::Instance()->Quit();
			CStringTable::Instance()->Quit();
			

			//2010-04-12 19:30:00 YSW
			//因为可以强制修改输出，所以把最后一次选择的保存起来。
			{
				unsigned long dataLen = sizeof(unsigned long) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD );
				unsigned char * pData = new unsigned char [dataLen];

				NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
				pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
				pBlockHead->ItemNum		= 1;
				pBlockHead->netcfgver	= NCFG_VERSION;

				NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
				pItemHead->itemID = NCFG_ITEM_VGA_RESOLUTION;
				pItemHead->len	  = sizeof(unsigned long);
				pItemHead->num	  = 1;
				pItemHead->subLen = sizeof(unsigned long);

				//修改问题：如果分辨率修改成CVBS，重启后，进入基本配置会出现断言。
				//如果分辨率是VGA_720X576, VGA_720X480，就表示是当前是CVBS输出，
				//而VGA_640X480表示CVBS输出。
				VGA_RESOLUTION resolutionTmp = g_resolution;
				if ((VGA_720X576 == g_resolution) || (VGA_720X480 == g_resolution))
				{
					resolutionTmp = VGA_640X480;
				}

				if(g_support_hd_type==340)
				{
					if((VGA_1440X1152 == g_resolution) || (VGA_1440X960 == g_resolution))
					{
						resolutionTmp = VGA_1920X1080;
					}
				}
				
				memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &resolutionTmp, sizeof(unsigned long));

				if (!pConfig->SetConfig( pData, dataLen ))
				{
					assert (false);
				}

				delete [] pData;
				pData = NULL;
			}
			
		}
		else
		{
			//不是重绘则退出循环，之前要记录提示字符串。
			strcpy(strShutDown, CStringTable::Instance()->String(E_STR_ID_SAFELY_TURN_OFF));
			break;
		}
	} while(DVR_SYSTEM_REDRAW == g_system_status);
	
	//////////////////////////////////////////////////////////////////////////
	/*do
	{
		if (DVR_SYSTEM_RUNING != g_system_status)
		{
			printf("Reboot system or shutdown system\n");
			break;
		}
		else
		{
			sigpending(&sig_pending);
			if(sigismember(&sig_pending, SIGTERM) || sigismember(&sig_pending, SIGINT))
			{
				printf("Ctrl + C Exit ! \n");

				break;
			}

			PUB_Sleep(1000);
		}
	}while(true);*/

endui:

	//////////////////////////////////////////////////////////////////////////
#ifdef __SUPPORT_3G__
	if(g_IsChangeNetMode!=128)
	{
		//组织数据头保存配置项目
		int setDataLen = sizeof(unsigned long) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD );
		unsigned char *tPdata = new unsigned char [setDataLen];
		memset(tPdata,0,setDataLen);
		NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(tPdata);
		pBlockHead->biSize=sizeof(NCFG_BLOCK_HEAD);
		pBlockHead->ItemNum=1;//根据配置数目进行设置，只改一个写为1
		pBlockHead->netcfgver=NCFG_VERSION;

		NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(tPdata + sizeof(NCFG_BLOCK_HEAD));
		pItemHead->itemID=NCFG_ITEM_USE_3G;
		pItemHead->len =sizeof(unsigned long);
		pItemHead->num =1;
		pItemHead ->subLen=sizeof(unsigned long);

		int cgf=g_IsChangeNetMode;
		memcpy(tPdata + sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &cgf, sizeof(unsigned long));
		if (!pConfig->SetConfig( tPdata, setDataLen ))
		{
			assert (false);
		}
		delete []tPdata;
		tPdata =NULL;

		//读取无线配置并修改第一个项目enable
		CMyList <NCFG_ITEM_ID> tmpitemList;
		tmpitemList.AddTail(NCFG_ITEM_NET_WIRELESS);
		unsigned char *pConfigData = NULL;
		unsigned long dataLen = 0;
		if (!pConfig->GetConfigEx(tmpitemList, &pConfigData, dataLen))
		{
			printf("Get config data fail!\n");
			DVRSystem("exit");
			exit(0);
		}
		else
		{
			tmpitemList.RemoveAll();
		}
		CConfigBlock cfgBlock;
		cfgBlock.CreateBlock(pConfigData, dataLen);

		unsigned char *pData = NULL;
		cfgBlock.GetConfig(NCFG_ITEM_NET_WIRELESS, &pData, dataLen );
		MDVR_WIRELESS_CFG wirelesscfg = *reinterpret_cast<const MDVR_WIRELESS_CFG *>(pData);
		wirelesscfg.enable = 0;
		

		cfgBlock.DestroyBlock();

		//再写回去
		setDataLen = sizeof(MDVR_WIRELESS_CFG) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD );
		tPdata = new unsigned char [setDataLen];
		memset(tPdata,0,setDataLen);
		pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(tPdata);
		pBlockHead->biSize=sizeof(NCFG_BLOCK_HEAD);
		pBlockHead->ItemNum=1;//根据配置数目进行设置，只改一个写为1
		pBlockHead->netcfgver=NCFG_VERSION;
		pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(tPdata + sizeof(NCFG_BLOCK_HEAD));
		pItemHead->itemID=NCFG_ITEM_NET_WIRELESS;
		pItemHead->len =sizeof(MDVR_WIRELESS_CFG);
		pItemHead->num =1;
		pItemHead ->subLen=sizeof(MDVR_WIRELESS_CFG);
		memcpy(tPdata + sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &wirelesscfg, sizeof(MDVR_WIRELESS_CFG));
		if (!pConfig->SetConfig( tPdata, setDataLen ))
		{
			assert (false);
		}
		delete []tPdata;
		tPdata =NULL;
	}
#endif

	pMainFrame->Stop();

#ifdef __SUPPORT_HRSST__
	CPhoneSync::Instance()->Stop();
#endif

#ifdef __NETSERVER_HXHT__
	CHxConnectMan::Instance()->Stop();
	CCfgUpNetServer::Instance()->Stop();
#endif

#ifdef __CHIP3531__
	netDaemon.Stop();
#endif

#ifdef __NETSERVER_TVT__
	pWebServer->Stop();
	pNetServer->Stop();
#endif

#ifdef __GB28181_SERVER__
	CGb28181Server::Instance()->Stop();
	g_RTPStreamDisPatch.Stop();
#endif

#ifdef __TUTK_SERVER__
	g_tutkStreamDispatch.Stop();
#endif

#ifdef __API_SERVER__
	g_apiStreamDispatch->Stop();
	CTvtAlarm::Instance()->Stop();
#endif

	//所有网络服务已经停止, 这里可以直接去写配置
	if(g_b_rw_cfg)
	{
		TrySaveStartupInfo(g_p_wnd_startup_info, g_p_cur_startup_info, true);
	}
	
	//pMainFrame->Stop();
	pLocalDevice->Stop();
	//////////////////////////////////////////////////////////////////////////
#ifdef __RELEASE__
	//杀狗
	if (DVR_SYSTEM_REBOOT == g_system_status)
	{
		//如果是重启，就不杀狗了。因为如果提前把狗杀掉，在pMainFrame->Quit();中会出现段错误。导致程序退出，不会执行下面的重启命令。
		//
	}
	else
	{
		pLocalDevice->KillDog();
	}
#endif

#ifdef __NETSERVER_TVT__
	pWebServer->Quit();
	pNetServer->Quit();
#endif

#ifdef __GB28181_SERVER__
	CGb28181Server::Instance()->Quit();
#endif

#ifdef __TUTK_SERVER__
	g_tutkStreamDispatch.Quit();
#endif

#ifdef __API_SERVER__
	g_apiStreamDispatch->Quit();
#endif

#ifdef __NETSERVER_HXHT__ 
	CHxConnectMan::Instance()->Quit();
	CCfgUpNetServer::Instance()->Quit();
#endif


	pMsgMan->Quit();
	pMainFrame->Quit();

	//网络设备库退出
#ifdef __DVR_ULTIMATE__
	//CNdLibMan::Instance()->Quit();
#endif

	{
		//设备反初始化前，提示关闭电源
		if (DVR_SYSTEM_EXIT == g_system_status)
		{
#if defined(__CUSTOM_BR27__)
			char strTmpFront[256]={0};
			char strTmpAfter[256]={0};
			char *p = NULL;
			p = strstr(strShutDown,",");
			if(NULL != p)
			{
				memset(strTmpFront,0,sizeof(strTmpFront));
				strncpy(strTmpFront,strShutDown,p-strShutDown);
				printf("strTmpFront=%s\n",strTmpFront);
				memset(strTmpAfter,0,sizeof(strTmpAfter));
				strcpy(strTmpAfter,p+1);
				printf("strTmpAfter=%s\n",strTmpAfter);
				GUI::CFBInterface::Instance()->Clear(0, 0, width, height);
				unsigned char zoom = (width < 1024) ? 1 : 2;
				unsigned long len = strlen(strTmpFront);
				unsigned short cx = 0, cy = 0;
				GUI::CFBInterface::Instance()->TextSize(strTmpFront, len, cx, cy, zoom);

				unsigned short x = (width - cx)/2;
				unsigned short y = (height - cy)/2;
				x = (width <= x) ? 0 : x;
				y = (height <= y) ? 0 : y;

				GUI::CFBInterface::Instance()->DrawRect(x, y, cx, cy, GUI_RGB(0, 0, 0));
				GUI::CFBInterface::Instance()->DrawText(strTmpFront, len, x, y, cx, cy, (width < 800) ? 
				GUI_RGB(255, 255, 255) : GUI_RGB(255, 0, 0), zoom);
				GUI::CFBInterface::Instance()->Refresh(x, y, cx, cy);

				//GUI::CFBInterface::Instance()->Clear(0, 0, width, height);
				zoom = (width < 1024) ? 1 : 2;
				len = strlen(strTmpAfter);
				cx = 0;
				cy = 0;
				GUI::CFBInterface::Instance()->TextSize(strTmpAfter, len, cx, cy, zoom);

				x = (width - cx)/2;
				y = (height - cy)/2;
				x = (width <= x) ? 0 : x;
				y = (height <= y) ? 0 : y;

				GUI::CFBInterface::Instance()->DrawRect(x, y+50, cx, cy, GUI_RGB(0, 0, 0));
				GUI::CFBInterface::Instance()->DrawText(strTmpAfter, len, x, y+50, cx, cy, (width < 800) ? 

				GUI_RGB(255, 255, 255) : GUI_RGB(255, 0, 0), zoom);
				GUI::CFBInterface::Instance()->Refresh(x, y+50, cx, cy);
			}
			else
			{
				GUI::CFBInterface::Instance()->Clear(0, 0, width, height);
				//GUI::CFBInterface::Instance()->Refresh(0, 0, width, height);
				unsigned char zoom = (width < 1024) ? 1 : 2;
				unsigned long len = strlen(strShutDown);
				unsigned short cx = 0, cy = 0;
				GUI::CFBInterface::Instance()->TextSize(strShutDown, len, cx, cy, zoom);

				unsigned short x = (width - cx)/2;
				unsigned short y = (height - cy)/2;
				x = (width <= x) ? 0 : x;
				y = (height <= y) ? 0 : y;

				GUI::CFBInterface::Instance()->DrawRect(x, y, cx, cy, GUI_RGB(0, 0, 0));
				GUI::CFBInterface::Instance()->DrawText(strShutDown, len, x, y, cx, cy, (width < 800) ? GUI_RGB(255, 255, 255) : GUI_RGB(255, 0, 0), zoom);
				GUI::CFBInterface::Instance()->Refresh(x, y, cx, cy);
			}
#else
			if (bUseUi)
			{
				GUI::CFBInterface::Instance()->Clear(0, 0, width, height);
				//GUI::CFBInterface::Instance()->Refresh(0, 0, width, height);
				unsigned char zoom = (width < 1024) ? 1 : 2;
				unsigned long len = strlen(strShutDown);
				unsigned short cx = 0, cy = 0;
				GUI::CFBInterface::Instance()->TextSize(strShutDown, len, cx, cy, zoom);

				unsigned short x = (width - cx)/2;
				unsigned short y = (height - cy)/2;
				x = (width <= x) ? 0 : x;
				y = (height <= y) ? 0 : y;

				GUI::CFBInterface::Instance()->DrawRect(x, y, cx, cy, GUI_RGB(0, 0, 0));
				GUI::CFBInterface::Instance()->DrawText(strShutDown, len, x, y, cx, cy, (width < 800) ? GUI_RGB(255, 255, 255) : GUI_RGB(255, 0, 0), zoom);
				GUI::CFBInterface::Instance()->Refresh(x, y, cx, cy);
			}
#endif

#if defined(__CHIP3535__)
			//sleep(5);
#else
			DVRSleep(5000);
#endif
		}

		//反初始化界面和FB。
		if (bUseUi)
		{
			pLocalUIMan->Quit();

			GUI::CFBInterface::Instance()->Quit();
		}
	}

	pConfig->Quit();
	pLanguagePack->Quit();
	pProduct->Quit();

	//////////////////////////////////////////////////////////////////////////
	//DVRSystem("exit");

	if (DVR_SYSTEM_REBOOT == g_system_status)
	{
#ifdef __TD8181
		exit(0);
#endif

		pLocalDevice->KillDog();
		pLocalDevice->FeedDog();
// 
//  		if ((productID >= TD_2424MD) && (TD_2504HE_M != productID) && (TD_7004SB != productID) && (TD_7008SB != productID))
//  		{
//  			//如果是25XXXX系列的机器，采取看门狗重启的方式
//  			pLocalDevice->KillDog();
//  			pLocalDevice->FeedDog();
//  		}
//  		else
//  		{
//  #if defined (__TDFH__) || defined(__CHIPTI__)
//  			//FH平台使用看门狗重启
//  			pLocalDevice->KillDog();
//  			pLocalDevice->FeedDog();			
//  #else
//  			DVRSystem("reboot");
//  #endif
// 		}
	}
	else if (DVR_SYSTEM_EXIT == g_system_status)
	{
#ifdef __TD8181
		exit(0);
#endif

		//system("cd /mnt/mtd; ./power_off");
		//system("halt");
#ifdef	__PACKID_14__

#else
		if (productID == TD_7008SB)
		{
			printf("productId=%d\n",productID);
			DVRSystem("halt");
		}
#endif
	}
	else
	{
		assert (false);
	}

	pLocalDevice->Quit();
	
	if (DVR_SYSTEM_EXIT == g_system_status)
	{
		if (!((TD_2708XS_SL == productID) || (TD_2716XS_SL == productID)))
		{
			DVRSystem("halt");
		}
	}

	DVRSystem("exit");

	exit(0);

	return 1;
}

//end
