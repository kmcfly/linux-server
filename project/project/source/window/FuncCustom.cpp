
#include "FuncCustom.h"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "GuiAssert.h"
#include "NetDeviceDefine.h"
#ifdef __ENVIRONMENT_LINUX__
#include "dvr_func.h"
#endif
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
extern WND_STARTUP_INFO* g_p_cur_startup_info;
extern bool g_bHaveSnapPic;
extern ULONGLONG g_net_channel;

void TrySaveStartupInfo(WND_STARTUP_INFO* pCfgInfo, WND_STARTUP_INFO* pCurInfo, bool bForce)
{
	/*
		参数 : pCfgInfo与配置一致, pCurInfo与当前运行时一致
		保存用户修改的配置信息 : 跳台、工具条位置
		如果，此时被其他人占用配置，将不会去保存,程序退出时再保存
		netserver stop 后可以保证没有人在配置, 可以直接读写配置
	*/

	//比较差异。没有改变则直接退出
	std::vector<NCFG_ITEM_ID> cfgItemList;
	if(pCfgInfo->DoDwell != pCurInfo->DoDwell)
	{
		cfgItemList.push_back(NCFG_ITEM_DODWELL);
	}

	if(0 != memcmp(&(pCfgInfo->ToolbarPos), &(pCurInfo->ToolbarPos), sizeof(NCFG_INFO_TOOLBAR_POS)))
	{
		cfgItemList.push_back(NCFG_ITEM_TOOLBAR_POS);
	}

	if(cfgItemList.size() <= 0)
	{
		return;
	}

	//进入配置。没进配置则直接退出
	CMessageMan* pMsgMan = CMessageMan::Instance();

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	
	if(bForce)
	{
		//本地强行,使得配置退出
		pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
	}
	
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		cfgItemList.clear();
		pMsgMan->ReleaseMsgBuff( msgData );
		return;
	}
	pMsgMan->ReleaseMsgBuff(msgData);

	//保存修改了的数据
	unsigned long dataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size();
	for(std::vector<NCFG_ITEM_ID>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		switch(*iter)
		{
		case NCFG_ITEM_DODWELL:
			dataLen += sizeof(unsigned long);
			break;
		case NCFG_ITEM_TOOLBAR_POS:
			dataLen += sizeof(NCFG_INFO_TOOLBAR_POS);
			break;
		default:
			gui_assert(false, printf("error.empty item id = 0x%X\n", *iter));
			break;
		}
	}
	BYTE* pData = new BYTE[dataLen + 1];
	memset(pData, 0, dataLen + 1);

	//组装头信息
	NCFG_BLOCK_HEAD *pblockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pData);
	pblockHead->netcfgver = NCFG_VERSION;
	pblockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
	pblockHead->ItemNum = cfgItemList.size();

	BYTE* pWritePos = NULL;
	unsigned long itemOffsetLen = 0;
	int index = 0;
	NCFG_ITEM_HEAD* pItemInfo = reinterpret_cast<NCFG_ITEM_HEAD *>(pData + sizeof(NCFG_BLOCK_HEAD));
	for(std::vector<NCFG_ITEM_ID>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		switch(*iter)
		{
		case NCFG_ITEM_DODWELL:
			{
				pItemInfo[index].itemID = (*iter);
				pItemInfo[index].num = 1;
				pItemInfo[index].len = sizeof(unsigned long);
				pItemInfo[index].subLen = sizeof(unsigned long);
				pWritePos = pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() + itemOffsetLen;

				*reinterpret_cast<unsigned long*>(pWritePos) = pCurInfo->DoDwell;

				index++;
				itemOffsetLen += sizeof(unsigned long);
			}
			break;
		case NCFG_ITEM_TOOLBAR_POS:
			{
				pItemInfo[index].itemID = (*iter);
				pItemInfo[index].num = 1;
				pItemInfo[index].len = sizeof(NCFG_INFO_TOOLBAR_POS);
				pItemInfo[index].subLen = sizeof(NCFG_INFO_TOOLBAR_POS);
				pWritePos = pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() + itemOffsetLen;

				memcpy(pWritePos, &(pCurInfo->ToolbarPos), sizeof(NCFG_INFO_TOOLBAR_POS));

				index++;
				itemOffsetLen += sizeof(NCFG_INFO_TOOLBAR_POS);
			}
			break;
		default:
			gui_assert(false, printf("error.empty item id = 0x%X\n", *iter));
			break;
		}
	}

	//保存
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, pData, dataLen, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{	
		memcpy(pCfgInfo, pCurInfo, sizeof(WND_STARTUP_INFO));
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//退出配置
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);

	//
	cfgItemList.clear();
	delete [] pData;
}

int GetSplitIndex(unsigned long splitMode)
{
	int index = 0;

	if((splitMode >= VIEW_SPLIT_1A11) && (splitMode <= VIEW_SPLIT_6X6))
	{
		index = 7;
		return index;
	}

	if((splitMode >= VIEW_SPLIT_1A9) && (splitMode <= VIEW_SPLIT_5X5))
	{
		index = 6;
		return index;
	}

	if((splitMode >= VIEW_SPLIT_2A6) && (splitMode <= VIEW_SPLIT_4X6))
	{
		index = 5;
		return index;
	}

	if((splitMode >= VIEW_SPLIT_1A7) && (splitMode <= VIEW_SPLIT_4X4))
	{
		index = 4;
		return index;
	}

	if((splitMode >= VIEW_SPLIT_1A5) && (splitMode <= VIEW_SPLIT_3X3))
	{
		index = 3;
		return index;
	}

	if((splitMode >= VIEW_SPLIT_1A2) && (splitMode <= VIEW_SPLIT_2X3))
	{
		index = 2;
		return index;
	}

	if(splitMode == VIEW_SPLIT_2X2)
	{
		index = 1;
		return index;
	}

	if(splitMode == VIEW_SPLIT_1X1)
	{
		index = 0;
		return index;
	}

	assert(false);
	return index;
}

ULONGLONG GetMainMonitorCfg()
{
	WND_STARTUP_INFO* pCfgInfo = g_p_cur_startup_info;

	ULONGLONG authCH = 0x0;

	int index_split = GetSplitIndex(pCfgInfo->SplitMode);

	unsigned long count = pCfgInfo->SplitData[index_split][0];		//总数据组数
	unsigned long maxGroup = pCfgInfo->SplitData[index_split][1];	//有效数据组数

	unsigned long numGroup = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(pCfgInfo->SplitMode));	//每组个数

	for(int i = 0; i < numGroup * maxGroup; i++)
	{
		unsigned long chnnValue = pCfgInfo->SplitData[index_split][i + 2];
		if(chnnValue < g_p_login_succ->productInfo.videoInputNum)
		{
			authCH |= (static_cast<ULONGLONG>(0x1) << chnnValue);
		}
	}

	return authCH;
}

bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1 >= x2) ? (x1-x2) : (x2-x1);
	unsigned long y = (y1 >= y2) ? (y1-y2) : (y2-y1);

	if(((x*x) + (y*y)) <= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1>=x2)?(x1-x2):(x2-x1);
	unsigned long y = (y1>=y2)?(y1-y2):(y2-y1);

	if(((x*x) + (y*y)) >= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsSupportPTZ()
{
	bool bSupport = true;

	unsigned long bitCount = sizeof(unsigned long)*8;

	if(g_p_login_succ->productInfo.function[FUNC_PTZ_CTRL/bitCount] & (0x01 << (FUNC_PTZ_CTRL % bitCount)))
	{
		bSupport = true;
	}
	else
	{
		bSupport = false;
	}

#if defined(__TW01_RILI__)
	bSupport = false;
#endif
	return bSupport;
}

bool IsSupportWzd()
{
	bool bSupport = true;
#if defined (__CUSTOM_BR27__)
	bSupport = false;
//#elif defined (__CUSTOM_US02__)
//	bSupport = false;
#elif defined (__CUSTOM_GB31__)
	bSupport = false;
#elif defined (__CUSTOM_SHANGHAI__)
	bSupport = false;
#else
	bSupport = true;
#endif
	return bSupport;
}

bool IsSupportChangePlayBackColor()
{
	bool bSupport = true;
#if defined (__ENVIRONMENT_LINUX__) && defined (__CUSTOM_BR27__)
	bSupport = false;
	if (CProduct::Instance()->ProductType() >= TD_2404MD )
	{
		bSupport = true;
	}
#endif

	{
		PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
// 		if (   (TD_2508HC_B == productType) || (TD_2516HC_B == productType) || (TD_2508HE_B == productType) || (TD_2508HE_B_N100 == productType) || (TD_2516HE_B_N100 == productType)
// 			|| (TD_2516HE_B == productType) || (TD_2504HD_C == productType) || (TD_2508HD_C == productType) || (TD_2516HD_C == productType)
// 			|| (TD_2524HD_C == productType) || (TD_2532HD_C == productType) || (TD_2604LE   == productType)
// 			|| (TD_2608LE   == productType) || (TD_2616LE   == productType) || (TD_2604LD   == productType) || (TD_2508HE_C_3521   == productType) 
// 			|| (TD_2608LD   == productType) || (TD_2616LD   == productType) || (TD_2616LD_IP== productType) )
// 		{
// 			bSupport = false;
// 		}
		switch(productType)
		{
			case TD_2704XD_S:
			case TD_2708XD_S:
				bSupport = false;
				break;
			default:
				break;
		}
	}

	return bSupport;
}

bool IsSupportChangeToolBarColor()
{
	bool bSupport = true;
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if ((productType >= TD_2704XE_M) && (productType <= TD_2716XE_S))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704XD_S) && (productType <= TD_2716XD_S))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704XS_SL) && (productType <= TD_2716XS_SL))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704AS_SL) && (productType <= TD_2716AS_SL))
	{
		bSupport = true;
	}
	else if((productType >= TD_2704AS_SL_A) && (productType <= TD_2716AS_SL_A))
	{
		bSupport = true;
	}
	else if((productType >= TD_2704TS_S) && (productType <= TD_2716TS_S))
	{
		bSupport = true;
	}
	else if((productType >= TD_2704TS_PL) && (productType <= TD_2716TS_PL))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704TD_PL) && (productType <= TD_2732TD_PL))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704TD_P) && (productType <= TD_2716TD_P))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704AS_S) && (productType <= TD_2716AS_S))
	{
		bSupport = true;
	}
	else if((productType >= TD_2704TE_M) && (productType <= TD_2716TE_M))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704AE_SL) && (productType <= TD_2716AE_SL))
	{
		bSupport = true;
	}
	else
	{
#if defined (__CUSTOM_BR27__)
		if (productType >= TD_2404MD )
		{
			bSupport = true;
		}
		else
		{
			bSupport = false;
		}
#else
		bSupport = true;
#endif
	}
	#ifdef __ENVIRONMENT_LINUX__
	int  iParam=0;
	if(0 == ext_get_sdk_param(EXT_GET_SDK_PARAM_LIVE_COLOR,&iParam))
	{
		bSupport = (0 == iParam)? true: false;
	}
	#endif
	return bSupport;
}


bool IsSupportChangeLiveColor()
{
	bool bSupport = true;
	int  iParam=0;

	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if ((productType >= TD_2704XE_M) && (productType <= TD_2716XE_S))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704XD_S) && (productType <= TD_2716XD_S))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704XS_SL) && (productType <= TD_2716XS_SL))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704AS_SL) && (productType <= TD_2716AS_SL))
	{
		bSupport = true;
	}
	else if((productType >= TD_2704AS_SL_A) && (productType <= TD_2716AS_SL_A))
	{
		bSupport = true;
	}
	else if((productType >= TD_2704TS_S) && (productType <= TD_2716TS_S))
	{
		bSupport = true;
	}
	else if((productType >= TD_2704TS_PL) && (productType <= TD_2716TS_PL))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704TD_PL) && (productType <= TD_2732TD_PL))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704TD_P) && (productType <= TD_2716TD_P))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704TE_M) && (productType <= TD_2716TE_M))
	{
		bSupport = false;
	}
	else if((productType >= TD_2704AS_S) && (productType <= TD_2716AS_S))
	{
		bSupport = true;
	}
	else
	{
		bSupport = true;
	}
		
	#ifdef __ENVIRONMENT_LINUX__
	if(0 == ext_get_sdk_param(EXT_GET_SDK_PARAM_LIVE_COLOR,&iParam))
	{
		bSupport = (0 == iParam)? true: false;
	}
	#endif
	return bSupport;

}


bool IsSupportSnapPic()
{
	return g_bHaveSnapPic;
}

bool IsSupportShutDown()
{
//#if defined(__CUSTOM_TW01TS__)
//	return false;
//#else
	return true;
//#endif
}

bool IsSupportPip()
{
	//
	PRODUCT_TYPE product = CProduct::Instance()->ProductType();
	if (((product >= TD_2504FD) && (product <= TD_2516FD))
	  ||(product == TD_2704HD)||(product == TD_2604LD_S)
      ||(product == TD_2608LD_S)||(product == TD_2616LD_S)
      ||(product == TD_2604LD)||(product == TD_2608LD)
      ||(product == TD_2616LD)||(product == TD_2316ME_A) ||(product == TD_2308ME_A) ||(product == TD_2304ME_A) || (product == TD_2504HE_B) ||(product == TD_2508HE_L)
	  ||(product == TD_2504HD_C) || (product == TD_2508HD_C) || (product == TD_2516HD_C) || (product == TD_2524HD_C) || (product == TD_2532HD_C)
	  ||(product == TD_2704XE_L) || (product == TD_2708XE_L) || (product == TD_2716XE_L)
	  ||(product == TD_2704XD_L) || (product == TD_2708XD_L) || (product == TD_2716XD_L)
	  ||(product == TD_2704XD_M) || (product == TD_2708XD_M) || (product == TD_2716XD_M)
	  ||(product == TD_2704XD_P) || (product == TD_2708XD_P) || (product == TD_2716XD_P)
	  ||(product == TD_2524HD_C) || (product == TD_2532HD_C) || (product == TD_2708XE_M) || (product == TD_2716XE_M)|| (product == TD_2708XE_MH)
	  ||(product == TD_2404MD_B) || (product == TD_2408MD_B) || (product == TD_2416MD_B)
	  ||(product == TD_2708XE_P)
	  ||(product == TD_2704XE_S) || (product == TD_2708XE_S) || (product == TD_2716XE_S)
	  ||(product == TD_2704XS_SL) || (product == TD_2708XS_SL) || (product == TD_2716XS_SL)
	  ||(product == TD_2704AS_SL) || (product == TD_2708AS_SL) || (product == TD_2716AS_SL)
	  ||(product == TD_2704AS_S) || (product == TD_2708AS_S) || (product == TD_2708AS_S_A) || (product == TD_2716AS_S)
	  ||(product == TD_2704TS_S) || (product == TD_2708TS_S) || (product == TD_2716TS_S)
	  ||(product == TD_2704TS_PL) || (product == TD_2708TS_PL) || (product == TD_2716TS_PL)
	  ||(product == TD_2704TD_PL) || (product == TD_2708TD_PL) || (product == TD_2716TD_PL) || (product == TD_2732TD_PL)
	  ||(product == TD_2704TD_P) || (product == TD_2708TD_P) || (product == TD_2716TD_P)
	  ||(product == TD_2524HE_B) || (product == TD_2532HE_B) || (product == TD_2524HE_C) || (product == TD_2532HE_C)
	  ||(product == TD_2704XD_S) || (product == TD_2708XD_S) || (product == TD_2716XD_S)
	  ||(product == TD_2816NE_A) || (product == TD_2816ND_A)
	  ||(product == TD_2704TE_M) || (product == TD_2708TE_M) || (product == TD_2716TE_M)
	  ||(product == TD_2704AS_SL)||(product == TD_2704AS_SL_A) ||(product == TD_2704AS_PL) || (product == TD_2708AE_SL) || (product == TD_2716AE_SL)
	  || (TD_2004AL == product) || (TD_2008AS == product)
	  ||(product == TD_2704AS_SL_A)
	  ||(product == TD_2704AS_PL) || (product == TD_2708AS_PL) || (product == TD_2716AS_PL)
	  || (product == TD_2716AC_PL)
	  ||(product == TD_2704AE_PL) || (product == TD_2708TE_M) || (product == TD_2716AE_PL))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool IsSupportAlarmOut()
{
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if ((TD_2304CDS == productType) || (TD_2304HS == productType) || (TD_2308CDS == productType)
		|| (TD_2304SS_B == productType) || (TD_2308SS_B == productType) || (TD_2316SS_B == productType) 
		|| (TD_2304SS_C == productType) || (TD_2308SS_C == productType) || (TD_2316SS_C == productType)
		|| (TD_2304SL_C == productType) || (TD_2308SL_C == productType)
		|| (TD_2508HS_BL == productType)|| (TD_2516HS_BL == productType)|| (TD_2304SS_SL == productType) || (TD_2308SS_SL == productType)
		|| (TD_2304SS_SDI == productType) || (TD_2308SS_SDI == productType) ||(TD_2304SS_SL_3515A == productType) \
		|| (TD_2804NS_SL == productType) || (TD_2808NS_SL == productType) || (TD_2816NS_SL == productType)
		|| (TD_2704XS_SL == productType) || (TD_2708XS_SL == productType) || (TD_2716XS_SL == productType)
		|| (TD_2704AS_SL == productType) || (TD_2708AS_SL == productType) || (TD_2716AS_SL == productType)
		|| (TD_2704AS_S == productType) || (TD_2708AS_S == productType) || (TD_2716AS_S == productType)
		|| (TD_2704TS_S == productType) || (TD_2708TS_S == productType) || (TD_2716TS_S == productType)
		|| (TD_2704TS_PL == productType) || (TD_2708TS_PL == productType) || (TD_2716TS_PL == productType)
		//|| (TD_2704TD_PL == productType) || (TD_2708TD_PL == productType) || (TD_2716TD_PL == productType)
		//|| (TD_2704TD_P == productType) || (TD_2708TD_P == productType) || (TD_2716TD_P == productType)
		|| (TD_2804NS_C == productType) || (TD_2808NS_C == productType) || (TD_2816NS_C == productType) || (TD_2804PS_C4 == productType) || (TD_2808PS_C4 == productType) || (TD_2808PS_C8 == productType) || (TD_2816PS_C8 == productType)
		|| (TD_2804NS_CL == productType) || (TD_2808NS_CL == productType)
		|| (TD_3004D1_4P == productType) || (TD_3008D1_8P == productType)
		|| (TD_2004AL == productType) || (TD_2008AS == productType))
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool IsSupportSpotOut()
{
	bool bHaveSpot = CProduct::Instance()->HaveSpot();
	return bHaveSpot;
}

bool IsSupportRecordResChange()
{
	return CProduct::Instance()->CanChangLocalVideoInfo();
}

bool IsSupportRecordVideoSize()
{
	return CProduct::Instance()->CanChangLocalVideoSize();
}



bool IsSupportLiPb(int& max)
{
	//
	max = 4;
	PRODUCT_TYPE product = CProduct::Instance()->ProductType();
	if ((TD_2316SE_B   == product) 
	  ||(TD_2316SE   == product) 
	  ||(TD_2316ME   == product)
//	  ||(TD_2316ME_A == product)			FH产品不支持回放看现场
	  ||(TD_2324SE_S == product)
	  ||((product >= TD_2404MD)   && (product <= TD_2416MD))
	  ||((product >= TD_2504HE)   && (product <= TD_2516HE))
	  ||((product >= TD_2504HC)   && (product <= TD_2516HC)) 
	  ||((product >= TD_2504HE_B) && (product <= TD_2516FD))	  
	  ||((product >= TD_2604LE)   && (product <= TD_2616LD))
	  ||(product == TD_2404MD_B) || (product == TD_2408MD_B) || (product == TD_2416MD_B))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool IsSupportDiskSmart()
{
	bool bSupport = true;
	PRODUCT_TYPE product = CProduct::Instance()->ProductType();
	if ((TD_7004SB == product) || (TD_7008SB == product))
	{
		bSupport = false;
	}
	return bSupport;	
}

//////////////////////////////////////////////////////////////////////////
//__NVR_BASIC__ 通道相关

bool IsSupportCH(ULONGLONG& CH, int* pChnnNum, int* pLocalVideoInputNum, int* pNetVideoInputNum, int* pVideoOuputNum)
{
	/*
	NET_LOGIN_SUCCESS_REPLY.productInfo.function[8], 中最后32位存储是否支持通道
	此部分从配置中读取修正
	*/
	int maxChnn = g_p_login_succ->productInfo.localVideoInputNum + g_p_login_succ->productInfo.netVideoInputNum;

	CH = 0;

	//本地
	for(int i = 0; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
	{
		CH |= (0x1 << i);
	}
//xian 将 1 改成 0 
#if 1
	//网络
	if(g_p_login_succ->productInfo.netVideoInputNum > 0)
	{
		CH |= g_net_channel;
	}
#else
	for(int i = 0; i < g_p_login_succ->productInfo.netVideoInputNum; i++)
	{
		CH |= (0x1 << (g_p_login_succ->productInfo.localVideoInputNum + i));
	}
#endif

	if(pChnnNum)
	{
		*pChnnNum = 0;
		for(int i = 0; i < maxChnn; i++)
		{
			if((CH >> i) & 0x1)
			{
				*pChnnNum += 1;
			}
		}
	}

	if(pLocalVideoInputNum)
	{
		*pLocalVideoInputNum = g_p_login_succ->productInfo.localVideoInputNum;
	}

	if(pNetVideoInputNum)
	{
		*pNetVideoInputNum = g_p_login_succ->productInfo.netVideoInputNum;
	}

	if(pVideoOuputNum)
	{
		*pVideoOuputNum = g_p_login_succ->productInfo.videoOuputNum;
	}

	return true;
}

bool IsSupportFunction(_FUNC_TYPE funcType)
{
	unsigned long bitCount = sizeof(unsigned long) * 8;

	if (g_p_login_succ->productInfo.function[FUNC_SERVER_ENCODE_CHECK/bitCount] & (0x01 << (FUNC_SERVER_ENCODE_CHECK % bitCount)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsStaticFrame()
{
	unsigned long bitCount = sizeof(unsigned long) * 8;

	if (g_p_login_succ->productInfo.function[FUNC_USE_STATIC_VIDEO_BITSTREAM/bitCount] & (0x01 << (FUNC_USE_STATIC_VIDEO_BITSTREAM % bitCount)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsSupportSubCode()
{
	unsigned long bitCount = sizeof(unsigned long) * 8;
	if (g_p_login_succ->productInfo.function[FUNC_SUPPORT_LOCAL_SUB_STREAM/bitCount] & (0x01 << (FUNC_SUPPORT_LOCAL_SUB_STREAM % bitCount)))
	{
		return true;
	}
	return false;
}

bool IsSupportHybridDisplay()
{
	PRODUCT_TYPE product = CProduct::Instance()->ProductType();
	if ((TD_2316SE_C   == product)||((TD_2704TS_PL  == product))||((TD_2304SE_C  == product))||(TD_2704AS_PL == product))
	{
		return false;
	}
	else
	{
		return true;
	}
}
unsigned long GetMainStreamMaxBitRate(unsigned long resolution)
{
	unsigned long maxBitRate = 0;
	switch(resolution)
	{
		case DVRVIDEO_SIZE_HD720:
			maxBitRate = 6*1024;
			break;
		case DVRVIDEO_SIZE_HD1080:
			maxBitRate = 12*1024;
			break;
		case DVRVIDEO_SIZE_960P:
			maxBitRate = 6*1024;
			break;
		case DVRVIDEO_SIZE_SXGA:
			maxBitRate = 6*1024;
			break;
		case DVRVIDEO_SIZE_3M:
			maxBitRate = 12*1024;
			break;	
		case DVRVIDEO_SIZE_16_9_3M:
			maxBitRate = 12*1024;
			break;	
		case DVRVIDEO_SIZE_4M:
			maxBitRate = 12*1024;
			break;	
		case DVRVIDEO_SIZE_5M:
			maxBitRate = 12*1024;
			break;	
		case DVRVIDEO_SIZE_D1:
			maxBitRate = 2*1024;
			break;
		case DVRVIDEO_SIZE_960H:
			maxBitRate = 2560;
			break;
		case DVRVIDEO_SIZE_VGA:
			maxBitRate = 1536;
			break;
		case DVRVIDEO_SIZE_HD1:
			maxBitRate = 1536;
			break;
		case DVRVIDEO_SIZE_CIF:
			maxBitRate = 1024;
			break;
		default:
			maxBitRate = 2*1024;
			break;
	}
	return maxBitRate;
}

unsigned long GetSubStreamMaxBitRate(unsigned long resolution)
{
	unsigned long maxBitRate = 0;
	switch(resolution)
	{
		case DVRVIDEO_SIZE_HD720:
			maxBitRate = 2*1024;
			break;
		case DVRVIDEO_SIZE_HD1080:
			maxBitRate = 4*1024;
			break;
		case DVRVIDEO_SIZE_960P:
			maxBitRate = 2*1024;
			break;
		case DVRVIDEO_SIZE_SXGA:
			maxBitRate = 2*1024;
			break;
		case DVRVIDEO_SIZE_3M:
			maxBitRate = 4*1024;
			break;	
		case DVRVIDEO_SIZE_16_9_3M:
			maxBitRate = 4*1024;
			break;	
		case DVRVIDEO_SIZE_4M:
			maxBitRate = 4*1024;
			break;	
		case DVRVIDEO_SIZE_5M:
			maxBitRate = 4*1024;
			break;	
		case DVRVIDEO_SIZE_D1:
			maxBitRate = 1024;
			break;
		case DVRVIDEO_SIZE_960H:
			maxBitRate = 2048;
			break;
		case DVRVIDEO_SIZE_VGA:
			maxBitRate = 1024;
			break;
		case DVRVIDEO_SIZE_HD1:
			maxBitRate = 512;
			break;
		case DVRVIDEO_SIZE_CIF:
			maxBitRate = 768;
			break;
		default:
			maxBitRate = 768;
			break;
	}
	return maxBitRate;
	}

//end

