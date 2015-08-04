/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-12-05
** Name         : NetProtocol.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _NET_PROTOCOL_H_
#define _NET_PROTOCOL_H_
#include "Typedef.h"
#include "dvrdvsdef.h"
#include "PTZ.h"
#include "Product.h"

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack(4)
#endif
/*/////////////////////////////////////////////////////////////////
//版本管理规则 
//1、设备端一直只保留对一个版本的支持，降低设备端复杂性
//2、客户端一直保持对之前的旧版本的支持
*//////////////////////////////////////////////////////////////////


typedef struct _pack_cmd
{
	unsigned long cmdType;//命令编号
	unsigned long cmdID;//指令序号，指令发起端设定的一个数值，对应的指令返回头中的指令序号与它完全相同
	//广播命令（没有请求的状态命令）的ID约定固定为0xffffffff（在客户端产生命令ID时不允许使用这个值）
	unsigned long cmdVer;//命令版本，可以直接根据这个版本确定这个命令的格式
	unsigned long dataLen;//数据长度
}PACKCMD;

const int PACK_HEAD_LENGTH=sizeof(PACKCMD);
//占用四字节
typedef enum _NET_PROTOCOL_H_
{
	//客户端登录
	CMD_BASENUM_LOGIN		= 0x0100,
	CMD_REQUEST_LOGIN,			//客户端请求登录
	CMD_REQUEST_LOGOUT,			//客户端请求登出
	CMD_END_LOGIN,

	//服务器端回复登录信息
	CMD_BASENUM_REPLY_LOGIN	= 0x010000,
	CMD_REPLY_LONGIN_SUCC,		//允许登录，附带设备信息
	CMD_REPLY_LONGIN_FAIL,		//拒绝登录，或者登录失败
	CMD_END_REPLY_LOGIN,

	//客户端控制（系统级）
	CMD_BASENUM_CTRL		= 0x0200,
	CMD_REQUEST_STREAM_START,			//客户端请求开始主码流、网络副码流、音频流
	CMD_REQUEST_STREAM_CHANGE,			//客户端请求改变主码流、网络副码流、音频流
	CMD_REQUEST_STREAM_STOP,			//客户端请求停止主码流、网络副码流、音频流
	CMD_REQUEST_KEYFRAME,				//请求通道关键帧，可以是任意通道的组合，快慢码流的组合
	CMD_REQUEST_SHUTDOWN,				//客户端请求关机
	CMD_REQUEST_REBOOT,					//客户端请求重启系统
	CMD_REQUEST_MANUAL_REC,				//客户端请求手动录像
	CMD_REQUEST_CHANGE_TIME,			//网络客户端请求修改系统时间
	CMD_REQUEST_L_CHANGE_ZONE_TIME,		//本地客户端请求修改系统[时区、时间]
	CMD_REQUEST_SYN_TIME_NOW,			//请求立即于和NTP服务器进行时间同步
	CMD_REQUEST_CHANGE_DISPLAY_MODE,	//请求切换现场，回放显示模式，如单画面，4画面
	CMD_REQUEST_GET_DISPLAY_INFO,		//请求查询显示状态
	CMD_REQUEST_CONTROL_LIVE_AUDIO,		//控制系统现场声音
	CMD_REQUEST_MODIFY_AUDIO_VOLUME,	//调节系统音量
	CMD_REQUEST_DISK_NUM_STATUS,		//请求系统硬盘数量的状态，如无硬盘，有新硬盘接入等
	CMD_REQUEST_DELETE_OVERDUEFILES,	//删除过期文件
	CMD_REQUEST_DODWELL_START,			//请求开始跳台
	CMD_REQUEST_DODWELL_STOP,			//请求结束跳台
	CMD_REQUEST_OPEN_ALARM,				//手动开启报警
	CMD_REQUEST_CLEAN_ALARM,			//手动清除所有报警
	CMD_REQUEST_MODIFY_MAC,             //请求修改mac地址
	CMD_REQUEST_MODIFY_HARDWARE_VER,    //请求修改硬件版本号
	CMD_REQUEST_MODIFY_PROD_SUB_ID,     //请求修改产品子版本号
	CMD_REQUEST_MODIFY_BOOT_IMAGE,      //修改开机画面
	CMD_REQUEST_MODIFY_DDNS_TYPE,       //修改不同公司的支持的DDNS种类
	CMD_REQUEST_MODIFY_CRYPTO_INFO,     //修改机器支持的语言种类
	CMD_REQUEST_EXFACTORY_SETUP,		//请求恢复出厂设置
	CMD_REQUEST_RESTOR_ADMIN_PASS,      //请求恢复admin用户的密码.
	CMD_REQUEST_SET_KEYBOARD_NAME,      //设置机器的支持的按键板
	CMD_REQUEST_EVICT_SOMEONE,			//驱逐某个客户端
	CMD_REQUEST_ZOOM,					//电子放大, ZOOM_INFO
	CMD_REQUEST_L_CHANGE_TIME,          //本地客户端请求修改时间
	CMD_REQUEST_JPEG_START,				//
	CMD_REQUEST_JPEG_CHANGE,			//
	CMD_REQUEST_JPEG_STOP,				//
	CMD_REQUEST_SET_FUNC,
	CMD_REQUEST_GET_DEV_COLOR,
	CMD_REQUEST_DEV_COLOR_SET,
	CMD_REQUEST_DEV_DEFA_COLOR,
	CMD_REQUEST_MANUAL_SNAP,
	CMD_REQUEST_DISK_SMART_INFO,
	CMD_REQUEST_SET_NETWORK,			//请求修改网络信息
	CMD_REQUEST_YUANZHENG_MANUAL_REC,   //远征的手动录像
	CMD_REQUEST_SET_YUANZHENG_PARAM,    //远征的心跳参数
	CMD_REQUEST_SET_BURN_INFO,			//设置刻录通道信息
	CMD_REQUEST_SET_BURN_REC,			//开始刻录
	CMD_REQUEST_MODIFY_COMPANY_NAME,	//修改厂家名称
	CMD_REQUEST_GB81_REC_CTRL,			//国标控制录像
	CMD_END_CTRL,

	//服务器端回复控制
	CMD_BASENUM_REPLY_CTRL	= 0x020000,
	CMD_REPLY_CTRL_SUCC,			//控制成功
	CMD_REPLY_CTRL_FAIL,			//控制失败
	CMD_REPLY_STREAM_CHANGE,		//改变码流
	CMD_REPLY_CHANAGE_TIME_SUCC,	//回复修改系统时间成功
	CMD_REPLY_CHANAGE_TIME_FAIL,	//回复修改系统时间失败
	CMD_REPLY_CHANAGE_DISPLAY_SUCC,	//回复改变现场通道成功
	CMD_REPLY_SYN_TIME_NOW_SUCC,	//立即同步时间成功
	CMD_REPLY_SYN_TIME_NOW_FAIL,	//立即同步时间失败
	CMD_REPLY_DEV_COLOR,
	CMD_REPLY_DISK_SMART_SUCC,
	CMD_REPLY_DISK_SMART_FAIL,
	CMD_END_REPLY_CTRL,

	//客户端通道控制
	CMD_BASENUM_CHNN_CTRL	= 0x0300,
	CMD_REQUEST_COLOR_GET,		//获取某通道的现场图像的颜色值
	CMD_REQUEST_COLOR_SET,		//调整某通道的现场图像的颜色值
	CMD_REQUEST_COLOR_DEFAULT,	//回放某通道颜色值为默认值
	CMD_REQUEST_COLOR_SAVE,		//保存某通道的现场图像颜色值
	CMD_REQUEST_CHNN_NAME,		//请求某个通道的名称	

	CMD_REQUEST_SET_IMAGE_MIRROR = 0x0319,	//图像镜像 unsigned char
	CMD_REQUEST_SET_IMAGE_FILP,		        //图像翻转 unsigned char
	CMD_REQUEST_SET_IMAGE_ROTATO,        	//图像旋转 unsigned long
	CMD_REQUEST_SET_AUTOFOCUS,	            //AF,调节焦距和Zoom  AF_CONTROL
	CMD_REQUEST_SET_IMAGE_DISTORTION,       //畸变校正 NCFG_ITEM_LENS_DISTORTION
	CMD_REQUEST_SET_IMAGE_ROI,

	CMD_REQUEST_SAVE_IMAGE_MIRROR,
	CMD_REQUEST_SAVE_IMAGE_FILP,
	CMD_REQUEST_SAVE_IMAGE_ROTATO,
	CMD_REQUEST_SAVE_IMAGE_DISTORTION,
	CMD_REQUEST_SAVE_IMAGE_ROI,

	CMD_REQUEST_GET_IMAGE_MIRROR,
	CMD_REQUEST_GET_IMAGE_FILP,
	CMD_REQUEST_GET_IMAGE_ROTATO,
	CMD_REQUEST_GET_IMAGE_DISTORTION,
	CMD_REQUEST_GET_IMAGE_ROI,
	CMD_REQUEST_SET_CAM_PARA_LOCAL_NUM,
	CMD_REQUEST_CHANGE_CVBS,
	CMD_END_CHNN_CTRL,

	//服务器端回复通道信息
	CMD_BASENUM_REPLY_CHNN_CTRL	= 0x030000,
	CMD_REPLY_CHNN_CTRL_COLOR,	//回复的某个通道的颜色值
	CMD_REPLY_COLOR_SAVE_SUCC,	//保存某个通道的颜色值成功
	CMD_REPLY_COLOR_SAVE_FAIL,	//保存某个通道颜色值失败
	CMD_REPLY_SAVE_IMAGE_MIRROR_SUCC,
	CMD_REPLY_SAVE_IMAGE_FILP_SUCC,
	CMD_REPLY_SAVE_IMAGE_ROTATO_SUCC,
	CMD_REPLY_SAVE_IMAGE_DISTORTION_SUCC,
	CMD_REPLY_SAVE_IMAGE_ROI_SUCC,
	CMD_REPLY_SAVE_IMAGE_MIRROR_FAIL,
	CMD_REPLY_SAVE_IMAGE_FILP_FAIL,
	CMD_REPLY_SAVE_IMAGE_ROTATO_FAIL,
	CMD_REPLY_SAVE_IMAGE_DISTORTION_FAIL,
	CMD_REPLY_SAVE_IMAGE_ROI_FAIL,
	CMD_REPLY_GET_IMAGE_MIRROR,
	CMD_REPLY_GET_IMAGE_FILP,
	CMD_REPLY_GET_IMAGE_ROTATO,
	CMD_REPLY_GET_IMAGE_DISTORTION,
	CMD_REPLY_GET_IMAGE_ROI,
	CMD_REPLY_SET_AUTOFOCUS_SUCC,
	CMD_REPLY_SET_AUTOFOCUS_FAIL,
	CMD_END_REPLY_CHNN_CTRL,

	//客户端配置
	CMD_BASENUM_CFG			= 0x0400,
	//配置进入、请求、保存、离开命令
	CMD_REQUEST_CFG_ENTER,		//进入配置
	CMD_REQUEST_CFG_LEAVE,		//离开配置
	CMD_REQUEST_CFG_GET,		//请求配置
	CMD_REQUEST_CFG_SET,		//保存配置
	CMD_REQUEST_CFG_DEF_DATA,	//请求默认配置数据，其后续数据和GET一样，只是返回的结果为默认数据
	CMD_REQUEST_CFG_DEFAULT,	//回复默认配置
	CMD_REQUEST_CFG_MODIFY_PASS,//请求修改密码，修改密码前需要进入配置

	//其他一些和配置相关的控制协议
	CMD_REQUEST_CFG_NET,		//通过多播设置网络相关的几个参数
	CMD_REQUEST_CFG_IMPORT,		//请求导入全部配置信息，附带全部配置信息
	CMD_REQUEST_CFG_EXPORT,		//请求导出全部配置信息

	CMD_END_CFG,

	//服务器端回复配置信息
	CMD_BASENUM_REPLY_CFG		= 0x040000,
	CMD_REPLY_CFG_SUCC,			//允许进入配置时回复成功
	CMD_REPLY_CFG_DATA,			//回复配置数据
	CMD_REPLY_CFG_DEFAULT,		//回复恢复默认配置信息
	CMD_REPLY_CFG_FAIL,			//回复配置失败，附带失败码
	CMD_REPLY_CFG_MODIFY_PASS_SUCC, //回复修改密码成功
	CMD_REPLY_CFG_MODIFY_PASS_FAIL, //恢复修改密码失败
	CMD_REPLY_CFG_IMPORT_SUCC,		//导入配置成功
	CMD_REPLY_CFG_IMPORT_FAIL,		//导入配置失败
	CMD_REPLY_CFG_EXPROT_SUCC,		//导出配置成功
	CMD_REPLY_CFG_EXPROT_FAIL,		//导出配置失败

	CMD_END_REPLY_CFG,

	//客户端数据控制
	CMD_BASENUM_DATA		= 0x0500,
	CMD_REQUEST_DATA_SEARCH,	//请求搜索
	CMD_REQUEST_DATA_READ,		//请求读取数据
	CMD_REQUEST_DATA_PAUSE,		//暂停读取数据
	CMD_REQUEST_DATA_RESUME,    //恢复播放状态
	CMD_REQUEST_DATA_FF,		//快进
	CMD_REQUEST_DATA_REW,		//快退
	CMD_REQUEST_DATA_STOP,		//停止读取数据
	CMD_REQUEST_DATA_LOCK,		//锁定文件
	CMD_REQUEST_DATA_UNLOCK,	//解锁文件
	CMD_REQUEST_DATA_DELETE,	//删除文件
	CMD_REQUEST_DATA_INDEX,		//回复已经处理的帧INDEX
	CMD_REQUEST_LAST_STREAM_ERROR,//最后一次请求现场或回放流时出错信息
	CMD_REQUEST_CHANGE_AUDIO,
	CMD_REQUEST_PIC_LOCK,
	CMD_REQUEST_PIC_UNLOCK,
	CMD_REQUEST_PIC_DELETE,	
	CMD_REQUEST_DISK_PIC_INFO,
	CMD_REQUEST_DEL_DISK_PIC,
	CMD_END_DATA,

	//服务器端回复数据
	CMD_BASENUM_REPLY_SEARCH	= 0x050000,
	CMD_REPLY_DATA_INFO,			//回复的搜索结果数据，附带数据
	CMD_REPLY_DATA_NULL,			//回复搜索，表示无数据
	CMD_REPLY_LAST_STREAM_ERROR,	//
	CMD_REPLY_DISK_PIC_INFO,
	CMD_END_REPLY_SEARCH,

	//客户端查询事件
	CMD_BASENUM_SYS_STATUS		= 0x0600,
	CMD_REQUEST_EVENT_INFO,			//请求查询事件
	CMD_REQUEST_LOG_INFO,			//请求查询操作和异常事件
	CMD_REQUEST_CLIENT_INFO,		//查看所有连接在设备上客户端信息
	CMD_REQUEST_FIRST_ENCODE_INFO,	//查询主码流编码信息
	CMD_REQUEST_SECOND_ENCODE_INFO,	//查询网络流编码信息
	CMD_REQUEST_THREE_ENCODE_INFO,	//查询手机流编码信息
	CMD_REQUEST_NETWORK_INFO,		//查询网络状态数据
	CMD_REQUEST_LOGIN_USER_INFO,	//查询登录用户的信息(包括本地)
	CMD_REQUEST_EXPORT_LOG,			//请求导出日志
	CMD_REQUEST_EXPORT_EVENT,		//请求导出事件列表
	CMD_REQUEST_WIRELESS_INFO,		//请求无线网络信息
	CMD_REQUEST_DTNETWORK,			//获取动态ip
	CMD_REQUEST_FIRST_SUPPORT_INFO,	//请求支持的主码流配置信息
	CMD_REQUEST_FIRST_CHECK_ENC_INFO,//请求检查主码流编码是否超过性能
	CMD_REQUEST_NAT_INFO,			//查询NAT信息
	CMD_REQUEST_SECOND_SUPPORT_INFO, //请求支持的子码流配置信息
	CMD_REQUEST_SECOND_CHECK_ENC_INFO,//请求检查子码流编码是否超过性能
	CMD_REQUEST_APP_DOWNLOAD_LINK_INFO,//请求phone app 下载信息
	CMD_REQUEST_TOKEN_SERVER_INFO,		//请求token server 信息
	CMD_END_SYS_STATUS,

	//服务器端回复查询事件
	CMD_BASENUM_REPLY_SYS_STATUS	= 0x060000,
	CMD_REPLY_EVENT_INFO,			//查询事件，附带事件列表
	CMD_REPLY_LOG_INFO,				//查询操作和异常事件，附带列表
	CMD_REPLY_CLIENT_INFO,			//所有客户端的信息
	CMD_REPLY_FIRST_ENCODE_INFO,	//回复主码流的编码参数信息
	CMD_REPLY_SECOND_ENCODE_INFO,	//回复网络流编码参数信息
	CMD_REPLY_THREE_ENCODE_INFO,	//回复手机流编码参数信息
	CMD_REPLY_NETWORK_INFO,			//回复网络状态数据
	CMD_REPLY_LOGIN_USER_INFO,		//回复登录用户的信息
	CMD_REPLY_SYS_STATUS_FAIL,		//查询信息失败,附带失败码
	CMD_REPLY_WIRELESS_INFO,		//回复查询无线状态信息
	CMD_REPLY_DTNETWORK,			//回复动态ip
	CMD_REPLY_FIRST_SUPPORT_INFO,	//回应主码流配置信息
	CMD_REPLY_FIRST_CHECK_ENC_INFO, //回应检查主码流编码是否超过性能
	CMD_REPLY_SECOND_SUPPORT_INFO,  //回应子码流配置信息
	CMD_REPLY_SECOND_CHECK_ENC_INFO,//回应检查子码流编码是否超过性能

	CMD_REPLY_APP_DOWNLOAD_LINK_INFO_SUCC,//请求app下载信息成功
	CMD_REPLY_APP_DOWNLOAD_LINK_INFO_FAIL,//请求app下载信息失败
	CMD_REPLY_TOKEN_SERVER_INFO_SUCC,	 //请求token server 信息成功
	CMD_REPLY_TOKEN_SERVER_INFO_FAIL,	 //请求token server 信息失败
	CMD_END_REPLY_SYS_STATUS,

	//客户端云台控制
	CMD_BASENUM_PTZ			= 0x0700,
	CMD_REQUEST_PTZ_START,			//请求云台控制
	CMD_REQUEST_PTZ_CTRL,			//云台控制命令
	CMD_REQUEST_PTZ_PRESET_INFO,	//预置点信息
	CMD_REQUEST_PTZ_CRUISE_INFO,	//巡航线信息
	CMD_REQUEST_PTZ_TRACK_INFO,		//轨迹信息
	CMD_REQUEST_PTZ_STOP,			//结束云台控制
	CMD_REQUEST_PTZ_STATUS,			//请求云台状态
	CMD_REQUEST_PTZ_CRUISE_PREVIEW,	//请求巡航线预览
	CMD_REQUEST_PTZ_CRUISE_PREVIEW_RECOVER,		//恢复预览的巡航线
	CMD_REQUEST_PTZ_STOP_CURRENT_STATUS,		//停止云台的当前状态，主要是停止自动扫描、巡航、轨迹
	
	CMD_END_PTZ,

	//服务器回复云台预置点、巡航信息
	CMD_BASENUM_REPLY_PTZ       = 0x70000,
	CMD_REPLY_PTZ_START_SUCC,       //请求开始云台返回成功
	CMD_REPLY_PTZ_START_FAIL,       //请求开始云台返回失败（附带错误原因）
	CMD_REPLY_PTZ_SET_SUCC,			//调整云台参数成功	
	CMD_REPLY_PTZ_SET_FAIL,			//调整云台参数失败
	CMD_REPLY_PTZ_PRESET_INFO,      //应答云台预置点信息
	CMD_REPLY_PTZ_CURISE_INFO,      //应答云台巡航线信息
	CMD_REPLY_PTZ_TRACK_INFO,		//应答云台轨迹线信息
	CMD_REPLY_PTZ_SEND_CMD_SUC,		//向云台发送命令成功
	CMD_REPLY_PTZ_SEND_CMD_FAIL,	//向云台发送命令失败
	CMD_REPLY_PTZ_STATUS_SUC,		//请求云台状态成功
	CMD_REPLY_PTZ_STATUS_FAIL,		//请求云台状态失败
	CMD_REPLY_PTZ_CRUISE_PREVIEW_SUC,//巡航线预览成功
	CMD_REPLY_PTZ_CRUISE_PREVIEW_FAIL,//巡航线预览失败
	CMD_END_REPLY_PTZ,

	//客户端磁盘管理
	CMD_BASENUM_DISK		= 0x0800,
	CMD_REQUEST_DISK_START,	                //请求磁盘管理
	CMD_REQUEST_DISK_MOUNT_REMOVABLE,       //搜索移动设备，并且挂载
	CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, //获取移动设备的每个分区的信息
	CMD_REQUEST_DISK_UNMOUNT_REMOVABLE,     //卸载移动设备
	CMD_REQUEST_DISK_GET_CD_INFO,           //请求光驱的信息
	CMD_REQUEST_DISK_STOP,			        //结束磁盘管理
	CMD_REQUEST_DISK_FORMAT_START,		    //格式化某个磁盘
	CMD_REQUEST_DISK_FORMAT_STOP,			//停止格式化某个磁盘
	CMD_REQUEST_DISK_CHANGE_PROPERTY,		//修改磁盘属性
	CMD_REQUEST_DISK_GET_OWNER,				//获取磁盘属主信息
	CMD_REQUEST_DISK_SET_SLEEP,				//设置磁盘休眠
	CMD_REQUEST_DISK_FULL_ALARM,            //磁盘满报警
	CMD_REQUEST_DISK_FORMAT_PERCENT,        //请求格式化进度
	CMD_END_DISK,

	//服务器端回复磁盘管理
	CMD_BASENUM_REPLY_DISK		= 0x80000,
	CMD_REPLY_DISK_INFO,				//回复磁盘管理成功，附带磁盘信息
	CMD_REPLY_DISK_FAIL,				//回复磁盘管理，附带失败码
	CMD_REPLY_DISK_MOUNT_REMOVABLE_SUC,	//手动卸载磁盘成功
	CMD_REPLY_DISK_MOUNT_REMOVABLE_FAIL,//手动卸载磁盘失败
	CMD_REPLY_FORMAT_FAIL,				//格式化失败
	CMD_REPLY_FORMAT_PERCENT,			//格式化进度
	CMD_REPLY_FORMAT_SUCC,				//格式化磁盘成功
	CMD_REPLY_DISK_CHANGE_PROPERTY_SUC,	//设置磁盘属性成功
	CMD_REPLY_DISK_CHANGE_PROPERTY_FAIL,//设置磁盘属性失败
    CMD_REPLY_DISK_GET_OWNER_SUC,		//修改磁盘属主成功
	CMD_END_REPLY_DISK,

	//移动侦测配置
	CMD_BASENUM_MOTION_AREA = 0X0900,
	CMD_REQUEST_MOTION_AREA_GET,		//请求获取移动侦测区域
	CMD_REQUEST_MOTION_AREA_SET,		//请求保存移动侦测区域
	CMD_END_MOTION_AREA,             

	//服务器回复移动侦测区域
	CMD_BASENUM_REPLY_MOTION_AREA   = 0X90000,
	CMD_REPLY_MOTION_AREA_GET_SUCC,     //回复移动侦测区域信息
	CMD_REPLY_MOTION_AREA_SET_SUCC,     //回复设置移动侦测区域成功
	CMD_REPLY_MOTION_AREA_GET_FAIL,     //回复获取移动侦测区域信息失败
	CMD_REPLY_MOTION_AREA_SET_FAIL,     //回复设置移动侦测区域失败
	CMD_END_REPLY_MOTION_AREA,

	//客户端升级控制
	CMD_BASENUM_UPDATE		= 0x0a00,
	CMD_REQUEST_UPDATE_START,		//请求软件升级
	CMD_REQUEST_UPDATE_DATA,		//上传软件升级包
	CMD_REQUEST_UPDATE_CANCEL,		//放弃升级
	CMD_REQUEST_UPDATE_ORIGIN,
	CMD_REQUEST_UPDATE_UPDATE,      //上传完数据包后，进行升级.
	CMD_END_UPDATE,

	//服务器端回复升级
	CMD_BASENUM_REPLY_UPDATE	= 0x0a0000,
	CMD_REPLY_CAN_UPDATE,		//回复表示可以升级
	CMD_REPLY_NOT_UPDATE,		//回复表示不能升级
	CMD_REPLY_UPDATE_FAIL,		//回复升级失败信息
	CMD_REPLY_UPDATE_PERCENT,	//回复升级进度
	CMD_END_REPLY_UPDATE,

	//对讲相关命令
	CMD_BASENUM_TALK			= 0x0b00,
	CMD_REQUEST_TALK_START,		    //请求开始对讲
	CMD_REQUEST_TALK_STREAM,		//客户端给设备发送对讲的音频数据
	CMD_REQUEST_TALK_STOP,			//停止对讲
	CMD_END_TALK,             

	//语音对象相关回复
	CMD_BASENUM_REPLYY_TALK		= 0x0b0000,
	CMD_REPLY_TALK_START_SUCC,		//回复请求开始对讲成功    
	CMD_REPLY_TALK_START_FAIL,		//回复请求开始对讲失败    
	CMD_END_REPLY_TALK, 

	//本地客户端回放控制
	CMD_BASENUM_LOCAL_CLIENT_PLAYBACK_CTRL = 0x0c00,
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PLAY,		//本地回放播放
    CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP,		//本地回放停止
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE,	//本地回放暂停
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM,	//本地回放恢复
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN,	//本地回放切换通道
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME,	//本地回放切换时间
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME,//本地回放下一帧
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF,		//本地回放快进
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW,		//本地回放快退
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_AUDIO,//本地回放切换音频
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CLICK,	//本地回放点击切换播放通道
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_LIVE,		//本地回放时显示现场
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CAM_NAME,	//本地回放时显示通道名称
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_HIT,      //判断本地回放点击的是回放画面还是现场画面
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHOSECHN, //本地回放选择通道	
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT,//本地回放开始时选择框设置为0通道
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_UNINIT,//本地回放结束时取消选择框

	//服务器端回复本地客户端回放的请求
	CMD_BASENUM_REPLY_LOCAL_CLIENT_PLAYBACK_CTRL = 0x0c0000,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_SUC,		//本地回放开启成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_FAIL,		//本地回放开启失败
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_STOP_SUC,		//本地回放停止成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PAUSE_SUC,		//本地回放暂停成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_RESUM_SUC,		//本地回放恢复成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_SUCC,	//本地回放切换通道成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_FAIL,	//本地回放切换通道失败
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_TIME_SUCC,	//本地回放切换时间成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_TIME_FAIL,	//本地回放切换时间失败
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_NEXTFRAME_SUC,	//本地回放下一帧成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_FF_SUC,			//本地回放快进成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_REW_SUC,		//本地回放快退成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CAM_NAME,       //本地回放时显示通道名称
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_HIT_SUC,        //本地回放点击的是回放画面
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_HIT_FAIL,       //本地回放点击的是现场画面
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CLICKIVE_SUC,   //本地回放点击增加现场按钮成功
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHOSE_CHNN_SUCC,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHOSE_CHNN_FAIL,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_INIT_CHNN_SUCC,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_UNINIT_CHNN_SUCC,
	//本地客户端备份控制
	CMD_BASENUM_LOCAL_CLIENT_BACKUP_CTRL = 0x0d00,
	CMD_REQUEST_LOCAL_CLIENT_BACKUP_STARTREAD,		//本地备份开始读取
	CMD_REQUEST_LOCAL_CLIENT_BACKUP_BACKUPING,		//本地备份开始备份
	CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP,			//本地备份停止

	//服务器端回复本地客户端备份的请求
	CMD_BASENUM_REPLY_LOCAL_CLIENT_BACKUP_CTRL = 0x0d0000,
	CMD_REPLY_LOCAL_CLIENT_BACKUP_STARTREAD_SUC,	//本地备份开启读取成功
	CMD_REPLY_LOCAL_CLIENT_BACKUP_STARTREAD_FAIL,	//本地备份开启读取失败
	CMD_REPLY_LOCAL_CLIENT_BACKUP_BACKUPING_SUC,	//本地备份成功
	CMD_REPLY_LOCAL_CLIENT_BACKUP_BACKUPING_FAIL,	//本地备份失败
	CMD_REPLY_LOCAL_CLIENT_BACKUP_STOP_SUC,

	//网络设备控制
	CMD_BASENUM_NET_DEVICE_LOG_CTRL = 0x0e00,
	CMD_REQUEST_CFG_ENTER_IPCAM,//请求进入设备管理页面配置
	CMD_REQUEST_CFG_LEAVE_IPCAM,//请求离开设备管理页面配置
	CMD_REQUEST_NET_DEVICE_LOG_MAX,		//请求网络设备容许的最大值
	CMD_REQUEST_NET_DEVICE_LOG_GET,		//请求网络设备信息
	CMD_REQUEST_NET_DEVICE_LOG_SET,		//请求设置网络设备信息, 参数为long + IP_DEVICE_INFO * N
	CMD_REQUEST_NET_DEVICE_SEARCH,		//请求搜索网络设备
	CMD_REQUEST_NET_DEVICE_INFO,		//请求连接上的网络设备信息
	CMD_REQUEST_SET_NET_DEVICE_INFO,	//请设置网络设备信息
	CMD_REQUEST_SUPPORT_DEVICE,			//获取NVR支持的设备种类

	//IPC数字信号接入配置
	CMD_BASENUM_REPLY_NET_DEVICE_LOG_CTRL = 0x0e0000,
	CMD_REPLY_NET_DEVICE_LOG_MAX,			//回复网络设备容许的最大值,参数为long
	CMD_REPLY_NET_DEVICE_LOG_GET_SUCC,		//回复请求网络设备信息成功，参数为long + IP_DEVICE_INFO * N
	CMD_REPLY_NET_DEVICE_LOG_GET_FAIL,		//回复请求网络设备信息失败
	CMD_REPLY_NET_DEVICE_LOG_SET_SUCC,		//回复设置网络设备信息成功,没有附加参数
	CMD_REPLY_NET_DEVICE_LOG_SET_FAIL,		//回复设置网络设备信息失败
	CMD_REPLY_NET_DEVICE_SEARCH,			//回复搜索网络设备,参数为long + DISCOVER_DEVICE_INFO * N
	CMD_REPLY_NET_DEVICE_INFO,				//回复网络设备信息
	CMD_REPLY_NET_SUPPORT_DEVICE,			//回复NVR支持的设备种类,参数为long + NVR_SUPPORT_MANUFACTURER * N + NVR_SUPPORT_PRODUCT * M


	//网络参数检测
	CMD_BASENUM_NETPARAMCHECK_CHECK	= 0x0f00,
	CMD_REQUEST_NETPARAMCHECK_PPPOE,		//检测PPPOE参数的正确性
	CMD_REQUEST_NETPARAMCHECK_DDNS,			//检测DDNS参数的正确性
	CMD_REQUEST_NETPARAMCHECK_DHCP,			//静态、动态分配IP地址的有效性
	CMD_REQUEST_NETPARAMCHECK_EMAIL,		//检测EMAIL 参数的的正确性
	CMD_REQUEST_SEND_EMAIL,                 //请求发送email
	CMD_REQUEST_NETPARAMCHECK_WIRELESS,     //检测3G参数的正确性
	CMD_REQUEST_NETPARAMCHECK_UPNP,			//检测UPNP是否打开
	CMD_REQUEST_ROUTERIP,					//UPNP获取路由器ip
	CMD_REQUEST_REMOVEDDNS,					//IN23删除DDNS
	CMD_END_NETPARAMCHECK,

	CMD_BASENUM_REPLY_NETPARAMCHECK_CHECK	= 0x0f0000,
	CMD_REPLY_NETPARAMCHECK_PPPOE_SUCC,		//返回检测PPPOE参数的正确性
	CMD_REPLY_NETPARAMCHECK_PPPOE_FAIL,		//返回检测PPPOE参数的失败
	CMD_REPLY_NETPARAMCHECK_DDNS_SUCC,		//返回检测DDNS参数的正确性
	CMD_REPLY_NETPARAMCHECK_DDNS_FAIL,		//返回检测DDNS参数的失败
	CMD_REPLY_NETPARAMCHECK_DHCP_SUCC,		//返回静态、动态分配网络地址的有效性
	CMD_REPLY_NETPARAMCHECK_DHCP_FAIL,		//返回静态、动态分配网络地址的失败
	CMD_REPLY_NETPARAMCHECK_EMAIL_SUCC,		//返回检测EMAIL参数的正确性
	CMD_REPLY_NETPARAMCHECK_EMAIL_FAIL,		//返回检测EMAIL参数的失败
	CMD_REPLY_NETPARAMCHECK_WIRELESS_SUCC,  //返回检测3G网卡连接成功
	CMD_REPLY_NETPARAMCHECK_WIRELESS_FAIL,  //返回检测3G网卡连接失败
	CMD_REPLY_NETPARAMCHECK_UPNP_SUCC,		//返回检测upnp映射成功
	CMD_REPLY_NETPARAMCHECK_UPNP_FAIL,		//返回检测upnp映射失败
	CMD_REPLY_GETROUTERIP_SUCC,				//UPNP获取路由器IP成功
	CMD_REPLY_GETROUTERIP_FAIL,				//UPNP获取路由器IP失败
	CMD_REPLY_REMOVEDDNS_SUCC,				//IN23删除DDNS成功
	CMD_REPLY_REMOVEDDNS_FAIL,				//IN23删除DDNS失败
	CMD_REPLY_IN23_DOMAIN_ALREADY_USE,		//IN23域名已经被使用
	CMD_REPLY_IN23_DOMAIN_CREATE_SUCC,		//IN23域名建立成功
	CMD_REPLY_IN23_DOMAIN_REPLACE_SUCC,		//IN23域名更换成功
	CMD_END__REPLY_NETPARAMCHECK,

#ifdef  __MOBILE_DVR__
	//车载相关界面请求
	CMD_REQUEST_NET_WIFI_SEARCH = 0x1000,	//请求搜索wifi信号
	CMD_REQUEST_NET_WIFI_CONNECT,			//请求检查wifi连接是否成功

	CMD_REPLY_NET_WIFI_SEARCH = 0x100000,	//回复搜索wifi信号
	CMD_REPLY_NET_WIFI_SEARCH_SUCC,			//回复搜索成功
	CMD_REPLY_NET_WIFI_SEARCH_FALL,			//回复搜索失败
	CMD_REPLY_NET_WIFI_CONNECT_SUCC,		//回复连接成功
	CMD_REPLY_NER_WIFI_CONNECT_FAIL,		//回复连接失败

#endif
	//关于iOS推送报警消息的命令
	CMD_BASENUM_PUSH_NOTIFICATION		= 0x1100,
	CMD_REQUEST_ADD_IPHONE_TOKEN,		//设置接收推送消息的iOS的设备令牌
	CMD_END_PUSH_NOTIFICATION,

	CMD_REQUEST_CHANNEL_CRRENT_VIDEO_ENCODE_INFO = 0x1200,		//NVR，客户端请求某个通道当前支持的编码信息
	CMD_REPLY_CHANNEL_CRRENT_VIDEO_ENCODE_INFO,					//NVR，设备端回应当前通道的编码信息

	CMD_REQUEST_MODIFY_CHANNEL_VIDEO_ENCODE_INFO = 0x1300,		//NVR，客户端请求设置某个通道的编码信息
	CMD_REPLY_MODIFY_CHANNEL_VIDEO_ENCODE_INFO_SUCC,			//NVR，设备端设置成功
	CMD_REPLY_MODIFY_CHANNEL_VIDEO_ENCODE_INFO_FAIL,			//NVR，设备端设置失败

	CMD_REQUEST_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO= 0x1400,	//IE/CMS等客户端请求数字通道当前支持的编码信息
	CMD_REPLY_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO,			//IE/CMS等端回应当前数字通道的编码信息

	CMD_REQUEST_NET_CHANNEL_VIDEO_DEF_BITSTREAM,				//IE/CMS等客户端请求数字通道当前分辨率默认的码率值
	CMD_REPLY_NET_CHANNEL_VIDEO_DEF_BITSTREAM,					//IE/CMS等端回应数字通道当前分辨率默认的码率值

	CMD_BASENUM_REPLY_PUSH_NOTIFICATION	= 0x110000,
	CMD_REPLY_ADD_IPHONE_TOKEN,
	CMD_END_REPLY_NOTIFICATION,

	//服务器端发送状态信息
	CMD_BASENUM_REPLY_STATUS	= 0x9000000,
	CMD_REPLY_VIDEO_LOSS,		//发送视频丢失状态
	CMD_REPLY_MOTION,			//发送移动侦测状态
	CMD_REPLY_SENSOR,			//发送报警输入状态
	CMD_REPLY_REC_STATUS,		//发送所有通道的录像状态
	CMD_REPLY_SHUT_DOWN,		//发送表示系统将关机
	CMD_REPLY_REBOOT,			//发送表示系统将重启
	CMD_REPLY_IP_CHANGE,		//发送表示网络地址将更改，附带新的网络地址
	CMD_REPLY_CHNN_NAME,		//发送某个通道的名称
	CMD_END_REPLY_STATUS,

	//服务器端发送视频/音频数据
	CMD_BASENUM_REPLY_STREAM	= 0xa000000,
	CMD_REPLY_DATA_STREAM,		//视频/音频数据
	CMD_END_REPLY_STREAM,

	//重新加载配置
	CMD_REQUEST_EFFECT_NETWORK	= 0xb000000,

	//要求客户端主动断开的命令
	CMD_REPLY_DISCONNECT		= 0xfffffffd,

	//没有权限的统一回复
	CMD_REPLY_NO_AUTH			= 0xfffffffe,

	//返回此值表示服务器不支持对应的功能
	CMD_REPLY_NO_SUPPORT		= 0xffffffff,

}NET_PROTOCOL;

typedef enum _net_ctrl_fail
{
	NETERR_SUCCESS					= 0,	//成功，所有地方的成功都可以使用此值
	NETERR_LOGIN_FAIL_VERSION_ERR	= 1,	//登陆时版本错误
	NETERR_LOGIN_FAIL_USERPWD_ERR	= 2,	//登陆时用户名或密码错误
	NETERR_LOGIN_FAIL_OVERFLOW		= 3,	//登陆网络超载，如果登陆超过一定数量后停止listen,此错误码就用不到
	NETERR_LOGIN_FAIL_REFUSE		= 4,	//登陆拒绝，可能是被过滤的IP的原因
	NETERR_OPERATE_FAIL_NOAUTHORITY	= 5,	//进操作没有权限，如配置、云台、回放
	NETERR_OPERATE_FAIL_OPERATING	= 6,	//只能一个人操作时返回已经有人在操作如配置、云台控制、回放
	NETERR_NO_DISK					= 7,	//无磁盘，在格式化、查询事件等和磁盘相关的操作时因为没有磁盘而失败时返回此值
	NETERR_NO_EVENT					= 8,	//无事件
	NETERR_FORMATERR_RECORDING		= 9,	//磁盘格式化错误，正在录像
	NETERR_NOTSUPPORT				= 10,	//不支持的操作
	NETERR_MUST_ENTER_CFG			= 11,	//在没有进入配置的情况下进行入得到、保存配置的操作时返回的错误码
	NETERR_DISK_IO_ERR				= 12,	//磁盘读写的IO错误
	NETERR_MODIFY_PASS_ERR			= 13,	//修改密码时,旧密码错误
	NETERR_MODIFY_PASS_UFUSER		= 14,	//修改密码时,在登录用户或配置中没有找到对应的用户
	NETERR_MODIFY_PASS_ERR_SAVE		= 15,	//修改密码时,保存密码到配置文件失败
	NETERR_FORMAT_DISK_FORMATING    = 16,   //格式化硬盘失败，因有人在格式化
	NETERR_FORMAT_DISK_BACKUP_PLAY  = 17,   //格式化硬盘失败，因有人在本地备份或有人在本地回放
	NETERR_UPDATE_FAIL_NO_DISK      = 18,   //远程升级失败，原因没有硬盘
	NETERR_UPDATE_FAIL_FORMAT_DISK  = 19,   //远程升级失败，原因正在格式硬盘硬盘
	NETERR_UPDATE_FAIL_UPDATING     = 20,   //远程升级失败，原因正在升级
	NETERR_UPDATE_FAIL_INVAID_FILE  = 21,   //远程升级失败，无效的文件
	NETERR_FAIL_NO_AUTHORITY        = 22,   //没有权限

	
	NETERR_NETPARAMCHECK_OK			= 500,	//信息通过服务器验证
	NETERR_NETPARAMCHECK_ENAMEPWD,			//用户名或者密码错误
	NETERR_NETPARAMCHECK_EHOSTDOMAIN,		//dydnds.com的用户名、密码或主机域名错误
	NETERR_NETPARAMCHECK_ETIMEOUT,			//超时。meibu使用UDP连接，在每次更新时不一定有返回
	NETERR_NETPARAMCHECK_ECONNECT,			//网络未连接,未插网线
	NETERR_NETPARMACHECK_ENETUNREACHD,		//网络不可达
	NETERR_NETPARMACHECK_EHOSTDOMAIN_EMPTY,	//主机域名为空
	NETERR_NETPARAMCHECK_EUNKNOWN,			//未知错误	
	NETERR_NETPARAMCHECK_ABUSE,				//dyndns.com上注册的域名被block
	NETERR_NETPARAMCHECK_NOHOST,			//dyndns.com上找不到相关域名
	NETERR_NETPARAMCHECK_NOTFQDN,			//dyndns.com上主机域名不符合规则
	NETERR_NETPARAMCHECK_NUMHOST,			//dyndns.com上太多的主机在update
	NETERR_NETPARAMCHECK_REGISTER_SUCC,		//向dynamic.visionica.com注册成功
	NETERR_NETPARAMCHECK_REGISTER_FAIL,		//向dynamic.visionica.com注册失败
	NETERR_NETPARAMCHECK_DOMAIN_ALREADY_USE = 515, //IN23的自动注册，域名已经被使用导致的注册失败
	NETERR_NETPARAMCHECK_DOMAIN_CREATE_OK,			//IN23域名建立成功
	NETERR_NETPARAMCHECK_DOMAIN_REPLACE_OK,			//IN23域名更换成功

	NETERR_NETPARAMCHECK_EIPCONFLICT= 600,	//静态设置的IP冲突
	NETERR_NETPARAMCHECK_EIPORSUBMASK,		//ip地址不可用,或者网络掩码错误
	NETERR_NETPARAMCHECK_ENONICDEVICE,		//没有指定的NIC设备
	NETERR_NETPARAMCHECK_EUSERNAMEEMPTY,	//用户名为空
	NETERR_NETPARAMCHECK_ENODEVICE,			//无线设备未连接
	NETERR_NETPARMACHECK_ENOCARD,			//没有UIM卡
	NETERR_NETPARMACHECK_ENOSIGNAL,			//没有网络信号
	NETERR_NETPARAMCHECK_EPARAM,			//拨号参数错误

	NETERR_STREAM_OK				= 1000,	//请求码流正常
	NETERR_STREAM_NO_ATH,					//没有相应权限
	NETERR_STREAM_NO_CAPABILITY,			//性能不够

	NETERR_EXPORT_CONFIG_OK				= 1100,
	NETERR_EXPORT_CONFIG_ERR,							//出错
	NETERR_EXPORT_CONFIG_DIRERR,						//目录不存在或者没有访问权限
	NETERR_EXPORT_CONFIG_FILEERR,						//没有选择文件
	NETERR_EXPORT_CONFIG_DVRIDERR,						//产品ID不匹配
	NETERR_EXPORT_CONFIG_FIRMWAREERR,					//固件版本不匹配
	NETERR_EXPORT_CONFIG_OVERONEUSER,					//同时又两个以上的用户在导入和导出
	NETERR_EXPORT_CONFIG_VERSION,						//导入文件中记录的版本和本地版本不一样

	NETERR_FAIL	= 0xffffffff,				//普通失败返回值，在不清楚原因的情况下返回此值
}NET_CTRL_FAIL;


const unsigned long		NET_PROTOCOL_VER				= 0x00000004;
#if defined(__STREAM_SERVER_RTP__)
const unsigned char		NET_PLAYBACK_DATA_FRAME_NUM		= 7;
#else
const unsigned char		NET_PLAYBACK_DATA_FRAME_NUM		= 25;
#endif

//////////////////////////////////////////////////////////////////////////
typedef enum _enum_connect_type{
	CONNECTTYPE_LOCAL,
	CONNECTTYPE_CTRL_CENTER,
	CONNECTTYPE_MOBILE_PHONE,
	CONNECTTYPE_IE,
	CONNECTTYPE_VOD,	//video-on-demand视频点播
	CONNECTTYPE_ECMS,   //支持主子码流
	CONNECTTYPE_MOBILE_PHONE_HIGH,		//高速手机流，非高清子码流设备传网络子码流，和IE的一样，高清子码流设备D1（不包括D1）以上传3帧，D1及以下和IE处理方式一样
	CONNECTTYPE_ACTIVE_CONNECT = 100,	//主动连接
}ENUM_CONNECT_TYPE;

//用户登录时用的结构体
typedef struct _net_login_info
{
	unsigned long	connectType;			//连接类型,ENUM_CONNECT_TYPE
	unsigned long   IP;					//客户端用来连接的IP地址

	char			username[36];			//用户姓名
	char			password[36];			//用户密码
	char			computerName[28];		//计算机名称

	unsigned char   MAC[6];					//网卡Mac地址
	unsigned char	resv[2];				//保留字节，对齐用
	unsigned long	netprotocolver;			//客户端目前使用的网络协议版本
}NET_LOGIN_INFO, *PNET_LOGIN_INFO;

//用户登录成功时，回复用的结构体
typedef struct _net_login_success_reply
{
	unsigned long	authority;		//用户网络权限或者用户级别
	ULONGLONG		authLiveCH;		//手动录像控制
	ULONGLONG		authRecordCH;		//手动录像控制
	ULONGLONG		authPlaybackCH;		//检索和回放
	ULONGLONG		authBackupCH;		//备份
	ULONGLONG		authPTZCtrlCH;		//云天控制
	ULONGLONG		authRemoteViewCH;	//远程预览
	PRODUCT_INFO	productInfo;	//产品信息
}NET_LOGIN_SUCCESS_REPLY;
/**************************************************登录握手协议***************************************************
			客户端										服务器端

1、	发出连接请求										回复协议版本号（NET_PROTOCOL_INFO）或者拒绝连接

2、	确定回复信号（协议版本是否兼容？）后，发出			验证登录用户名、密码及其他信息回复设备信息或者
	登录信息。见 NET_LOGIN_INFO 定义.					拒绝登录，见 NET_LOGIN_SUCCESS_REPLY 定义.			

3、	确定登录成功或者失败。如果登录成功，对回复的
	设备信息做一些必要的处理。

	客户端发送数据格式：	命令字段 - 信息（NET_LOGIN_INFO ）

	服务器端回复数据格式：	
			a)回复连接请求:		命令字段 - 信息字段（NET_PROTOCOL_INFO）
			b)回复验证成功:		命令字段 - 信息字段（NET_LOGIN_SUCCESS_REPLY）
			c)回复验证失败:		命令字段 - 失败信息码
*****************************************************************************************************************/

typedef struct _net_livestream_ctrl_info
{
	unsigned long	streamID;		//流ID

	ULONGLONG		fastVideoCH;	//用位表示各通道的状态_主码流
	ULONGLONG		secondVideoCH;	//用位表示各通道的状态_网络流
	ULONGLONG		threeVideoCH;	//用位表示各通道的状态_手机流
	ULONGLONG		audioCH;		//用位表示各通道的状态_音频流
}NET_LIVESTREAM_CTRL_INFO;

typedef struct _net_jpeg_stream_info
{
	unsigned long	streamID;
	ULONGLONG		jpegCH;
}NET_JPEG_STREAM_INFO;

typedef struct _net_display_mode_info
{
	ULONGLONG  live_mask;     //用位表示各通道的现场显示
	ULONGLONG  pb_mask;       //用位表示各通道的回放显示
}NET_DISPLAY_MODE_INFO;

typedef enum NET_DISPLAY_INFO_TYPE
{
	//命令CMD_REQUEST_CHANGE_DISPLAY_MODE, 回复CMD_REPLY_CTRL_SUCC
	NET_DISPLAY_CHANGE_DATA_PACK	= 0x0000,	//
	NET_DISPLAY_CHANGE_XY_PT,
	NET_DISPLAY_FREEZE,
	NET_DISPLAY_UNFREEZE,
	NET_DISPLAY_EXCHANGE_TWO_PT,
	NET_DISPLAY_SET_MINIWND_POS,
	NET_DISPLAY_FILTER,

	//命令CMD_REQUEST_GET_DISPLAY_INFO, 回复CMD_REPLY_CTRL_SUCC
	NET_DISPLAY_GET_INFO			= 0x0100,	//
	NET_DISPLAY_GET_INFO_XY,

	NET_DISPLAY_REPLY_ALL			= 0x0200,
	NET_DISPLAY_REPLY_HITS,
}NET_DISPLAY_INFO_TYPE;

enum DISPLAY_SIGNAL_TYPE
{
	DISPLAY_SIGNAL_NULL = 0,
	DISPLAY_SIGNAL_ANALOG,
	DISPLAY_SIGNAL_DIGIT,
	DISPLAY_SIGNAL_NET,
};

enum DISPLAY_LAYER_TYPE
{
	DISPLAY_LAYER_NULL = 0,
	DISPLAY_LAYER_ANALOG,
	DISPLAY_LAYER_DIGIT,
	DISPLAY_LAYER_NET,
	DISPLAY_LAYER_MINI,
};

typedef struct _net_display_info
{
	unsigned short	type;			//命令类型 NET_DISPLAY_INFO_TYPE
	unsigned short	rec;			//操作是否被记录[用户操作、回放、跳台被记录; 大画面报警、调节云台、调节色彩不被记录]
	unsigned short	time_out;		//超时,0表示使用默认值, -1表示以冻结方式操作
	unsigned short	data_len;		//有效数据长度[data]

	unsigned short	x;				//x坐标
	unsigned short	y;				//y坐标

	unsigned long	split_mode[8];	//每个层的分割模式
	unsigned long	data[64];		//DISPLAY_PACK[层<<24] [信号类型<<16] [区域编号<<8] [通道(unsigned char)]
}NET_DISPLAY_INFO;

//控制现场音频结构体
typedef enum NET_LIVEAUDIO_TYPE
{
	NET_LIVE_AUDIO_OPEN,
	NET_LIVE_AUDIO_MUTE,
	NET_LIVE_AUDIO_AUTO,
	NET_LIVE_AUDIO_RECOVER, //在进入回放时，会关闭现场声音，用此TYPE可以恢复在原来的状态。
}NET_LIVEAUDIO_TYPE;
typedef struct _net_liveaudio_ctrl_info
{
	unsigned long type;             //控制类型
	unsigned long audiochnnel;      //音频通道号
}NET_LIVEAUDIO_CTRL_INFO;

/**********************************************控制（系统级）握手协议********************************************
			客户端										服务器端

1、					发出控制命令						接受到命令后，进行处理，再返回处理结果

注：a) CMD_REQUEST_STREAM_START:	命令需要带控制信息信息（见 NET_LIVESTREAM_CTRL_INFO），表示请求的通道。
	b) CMD_REQUEST_MANUAL_REC:		该命令不需要回复什么，因为设备会回复录像状态。
	c) CMD_REQUEST_KEYFRAME:		该命令不需要回复什么.
	d) CMD_REQUEST_SHUTDOWN 、CMD_REQUEST_REBOOT：如果不能操作，则回复无权限。
	e) CMD_REQUEST_CHANGE_TIME:		调整成功或者失败
	f) CMD_REQUEST_CHANGE_DISPLAY_MODE :切换现场，回放显示模式，如单画面，4画面。回复CMD_REPLY_CHANAGE_DISPLAY_SUCC。
	g) CMD_REQUEST_EVICT_SOMEONE:驱逐某个客户端

客户端发送数据格式:	
	a) 请求码流:		命令字段 - 通道信息(NET_LIVESTREAM_CTRL_INFO)
	b) 请求手动录像:	命令字段 - 通道信息(ULONGLONG)
	c) 请求关键帧:		命令字段 - 通道信息(ULONGLONG)
	d) 请求修改时间:	命令字段 - 时间(ULONGLONG)
    e) 请求切换显示模式 命令字段 - 现场和回放显示信息（NET_DISPLAY_INFO        ）
	f) 
	g) 请求驱逐某人：	命令字段 - 客户端网络地址（unsigned long)
服务器端回复数据格式:	
	a) 调整时间成功:	命令字段 - CMD_REPLY_CHANAGE_TIME_SUCC
	b) 调整时间失败:	命令字段 - CMD_REPLY_CHANAGE_TIME_FAIL
*****************************************************************************************************************/

typedef struct _net_chnn_color
{
	unsigned short	chnn;	//通道号
	unsigned short	hide;	//通道隐藏
	unsigned long	color;	//通道颜色值((brightness<<24) + (contrast<<16) + (saturation<<8) + hue);
}NET_CHNN_COLOR;

typedef struct _net_chnn_mirror
{
	unsigned short chnn;
	unsigned char  mirror; 
}NET_CHNN_MIRROR;

typedef struct _net_chnn_flip
{
	unsigned short chnn;
	unsigned char  flip; 
}NET_CHNN_FLIP;

typedef struct _net_chnn_rotato
{
	unsigned short chnn;
	unsigned long  rotato; 
}NET_CHNN_ROTATO;

typedef struct _net_chnn_lensDistortion
{
	unsigned short chnn;
	LENS_DISTORTION_CONFIG lensDistortion; 
}NET_CHNN_LENS_DISTORTION;

typedef struct _net_chnn_name
{
	unsigned short	chnn;
	unsigned short	nameLen;
	char			name [36];
}NET_CHNN_NAME;

typedef struct _net_camera_cover_info
{
	unsigned long chnn;				//通道
	CAMERA_COVER_INFO coverInfo;
}NET_CAMERA_COVER_INFO;

/*************************************************通道控制握手协议***********************************************
			客户端										服务器端

1、			发出控制命令								接受到命令后，进行处理，再返回处理结果

注：a、CMD_REQUEST_KEYFRAME: 不回复。
	b、CMD_REQUEST_COLOR_GET：回复颜色值（NET_CHNN_COLOR）。
	d、CMD_REQUEST_COLOR_SET：附带颜色值（NET_CHNN_COLOR）。
	e、CMD_REQUEST_COLOR_DEF：回复颜色值（NET_CHNN_COLOR）。

客户端发送数据格式：	命令字段 - 信息（NET_CHNN_COLOR）
						命令字段 - 无

服务器端回复数据格式：	命令字段 - 信息（NET_CHNN_COLOR）
						命令字段 - 信息（NET_CHNN_NAME）
*****************************************************************************************************************/

typedef struct _net_cfg_info
{
	unsigned long	len;			//配置数据的长度
	unsigned long	cfgType;		//配置类型
}NET_CFG_INFO;
/**********************************************配置握手协议*****************************************************
		客户端										服务器端

1、		发出请求（CMD_REQUEST_CFG_ENTER）			接受到请求，处理后返回配置信息或者拒绝配置

2、		请求配置数据（CMD_REQUEST_CFG_GET）			接受命令及数据，并根据要求发生配置数据。

3、		保存配置数据（CMD_REQUEST_CFG_SET）			接受命令及数据，并保存数据，回复成功或者失败。

3、		离开配置（CMD_REQUEST_CFG_LEAVE）			处理退出配置。

注：a、针对不同的配置请求，回复不同的配置信息。

客户端发送数据格式:	
	a) 命令字段 - 数据长度 - 配置类型 - 要求配置数据的信息(列出需求的项ID)
	b) 命令字段 - 数据长度 - 配置类型 - 配置数据

服务器端回复数据格式：	
	a) 命令字段 - 数据长度 - 配置类型 - 配置数据
	b) 命令字段 - 失败信息码
*****************************************************************************************************************/
enum NET_DATA_SEARCH_TYPE
{
	NET_DATA_SEARCH_PLAYBACK,
	NET_DATA_SEARCH_BACKUP,
};

typedef enum _data_search_type
{
	DATA_SEARCH_TYPE_YEAR       = 0x00,     //
	DATA_SEARCH_TYPE_TIME       = 0x01,		//按时间搜索
	DATA_SEARCH_TYPE_EVENT      = 0x02,		//搜索事件
	DATA_SEARCH_TYPE_FILE       = 0x03,		//搜索文件
	DATA_SEARCH_TYPE_PIC        = 0x04,     //搜索图片
}DATA_SEARCH_TYPE;

typedef struct _net_data_search
{
	unsigned long		searchType;	//搜索类型， DATA_SEARCH_TYPE
	unsigned long		streamID;	//要求发送的stream ID

	ULONGLONG       	videoCH;	//视频通道条件
	ULONGLONG           audioCH;    //音频通道条件

	unsigned long		startTime;	//开始时间
	unsigned long		endTime;	//结束时间

	unsigned long		dataType;	 //数据类型 是用于回放还是备份：NET_DATA_SEARCH_TYPE

	unsigned long		RecordTypeMASK;//录像类型组合

	unsigned long       dataOwnerby;  //要搜索的数据是在本设备的硬盘上，还是其他设备的硬盘的上。
}NET_DATA_SEARCH;

typedef struct _net_hx_data_search
{
	unsigned long		chnnNum;	//音视频通道
	unsigned long		startTime;	//开始时间
	unsigned long		endTime;	//结束时间

	unsigned long       dataOwnerby;  //要搜索的数据是在本设备的硬盘上，还是其他设备的硬盘的上。
}NET_HX_DATA_SEARCH;


typedef struct _net_data_info
{
	unsigned long	dataType;		//数据类型
	unsigned long	end;			//1 表示最后一批
	unsigned long   index;          //索引
	unsigned long	dataNum;		//数据个数
}NET_DATA_INFO;

typedef struct _net_section_info
{
	unsigned long   chnnnel;
	unsigned long   starTime;
	unsigned long   endTime;
}NET_SECTION_INFO;


typedef enum _net_play_speed
{
	NET_PLAY_SPEED_1_32X = 1,
	NET_PLAY_SPEED_1_16X,
	NET_PLAY_SPEED_1_8X,	//1/8
	NET_PLAY_SPEED_1_4X,	//1/4
	NET_PLAY_SPEED_1_2X,	//1/2
	NET_PLAY_SPEED_1X,		//1
	NET_PLAY_SPEED_2X,
	NET_PLAY_SPEED_4X,
	NET_PLAY_SPEED_8X,
	NET_PLAY_SPEED_16X,
	NET_PLAY_SPEED_32X,
}NET_PLAY_SPEED;
typedef struct _playback_speed_info
{
	unsigned long chnn;
	unsigned long speed;
}PLAYBACK_SPEED_INFO;

typedef struct _playback_changetime_info
{
	unsigned long chnn;
	unsigned long time;
	bool bsync;//是否是同步模式
	bool bpause;//是否是暂停状态，是暂停状态需要刷新回放“||”图标
}PLAYBACK_CHANGETIME_INFO;

typedef struct _nfcg_lens_distortion_config__
{
	unsigned char  ucLensDistortion;        // 0 no, 1 yes
	unsigned char  ucLensDistortionValue;   // 0 - 255
	char            noUse[2];
    unsigned short chnn;
}LENS_DISTORTION;


/*************************************************数据控制握手协议***********************************************
			客户端										服务器端

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1、	发出搜索请求										回复搜索数据，或者拒绝搜索请求（网络超载）

2、	接受到回复数据，进行刷新处理

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1、	发送读取请求，搜索条件（NET_DATA_SEARCH）			回复视频/音频数据或者失败码

2、	接受视频/音频数据，回放婚或者备份

3、	发送暂停命令										暂停法送数据

4、	发送快进命令										根据速度读取关键帧并发送

5、	发送快退命令										根据速度读取关键帧并发送

6、	发送停止命令										停止读取，并销毁发送队列，销毁读取线程
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1、	发送锁定命令，附带文件信息							接受文件列表后进行锁定动作

2、	发送解锁命令，附带文件信息							接受文件列表后进行解锁动作

3、	发送删除命令，附带文件信息							接受文件列表后进行删除动作

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

注：a、针对不同的搜索请求，回复不同的信息。
	b、NET_DATA_INFO结构体用于发送前和接受后使用。
	c、回复视频/音频数据时的格式见“服务器端发送视频/音频数据”握手协议。

客户端发送数据格式：	
	a、搜索条件：命令字段 - 搜索条件（NET_DATA_SEARCH），用于搜索需要使用搜索类型
	b、开始读取：命令字段 - 读取条件（NET_DATA_SEARCH），用于开始读取数据，此时取streamID
	c、其他操作：命令字段 - 参数（读取倍数、需要停止的流ID）
	d、删除锁定：命令字段 - 文件个数 - 数据

服务器端回复数据格式：	
	a、搜索成功：命令字段 - 数据信息（NET_DATA_INFO）- 数据，搜索回复
	b、搜索失败：命令字段 - 失败信息码
*****************************************************************************************************************/
typedef struct _net_event_search
{
	unsigned long		type;		//EVENT_TYPE组合

	unsigned long		startTime;	//开始时间
	unsigned long		endTime;	//结束时间

	ULONGLONG	        chnnBits;	//通道组合
}NET_EVENT_SEARCH;

typedef struct _net_event_info
{
	unsigned long	end;			//表示最后一批次
	unsigned long	eventNum;		//表示元素数目：EVENT_LOG X n
}NET_EVENT_INFO;

typedef struct _net_log_search
{
	unsigned long		content;	//LOG_TYPE组合(注意为每一栏的组合）

	unsigned long		startTime;	//开始时间
	unsigned long		endTime;	//结束时间
}NET_LOG_SEARCH;

typedef struct _net_log_info
{
	unsigned long	end;			//表示最后一批次
	unsigned long	logNum;			//表示元素数目：OPERATOR_LOG X n
}NET_LOG_INFO;
/**********************************************查询事件握手协议***************************************************
				客户端										服务器端

1、		发出请求（CMD_REQUEST_EVENT_INFO）					接受到请求，处理后返回结果

2、		发出请求（CMD_REQUEST_LOG_INFO）					接受到请求，处理后返回结果

3、		发出请求（CMD_REQUEST_CLIENT_INFO）					接受到请求，处理后返回结果

4、		发出请求（CMD_REQUEST_FIRST_ENCODE_INFO）			接受到请求，处理后返回结果

5、		发出请求（CMD_REQUEST_SECOND_ENCODE_INFO）			接受到请求，处理后返回结果

6、		发出请求（CMD_REQUEST_THREE_ENCODE_INFO）			接受到请求，处理后返回结果

7、		发出请求（CMD_REQUEST_NETWORK_INFO）				接受到请求，处理后返回结果

客户端发送数据格式：	
	a、事件搜索条件：		命令字段 - 搜索条件（NET_EVENT_SEARCH）
	b、日志搜索条件：		命令字段 - 搜索条件（NET_LOG_SEARCH）
	c、客户端信息搜索：		命令字段
	d、主码流编码状态搜索：	命令字段
	e、次码流编码状态搜索：	命令字段
	f、手机流编码状态搜索：	命令字段
	g、网络状态搜索：		命令字段

服务器端回复数据格式：	
	a、查询事件成功：		命令字段 - 事件信息（NET_EVENT_INFO）- 数据 X N
	b、查询日志成功：		命令字段 - 事件信息（NET_LOG_INFO）- 数据 X N
	c、查询客户端成功：		命令字段 - 元素数目（N，四字节） - 事件信息（NET_CLIENT_INFO_EX）- 数据 X N
	d、查询主码流成功：		命令字段 - 元素数目（N，四字节） - 事件信息（STREAM_INFO）- 数据 X N
	e、查询次码流成功：		命令字段 - 元素数目（N，四字节） - 事件信息（STREAM_INFO）- 数据 X N
	f、查询手机流成功：		命令字段 - 元素数目（N，四字节） - 事件信息（STREAM_INFO）- 数据 X N
	g、查询网络状态成功：	命令字段 - 数据（NETWORK_STATUS_INFO）
	h、查询失败：			命令字段 - 失败信息码
*****************************************************************************************************************/

/**********************************************云台控制握手协议***************************************************
客户端										服务器端

1、		发出请求（CMD_REQUEST_PTZ_START）				接受到请求，处理后返回对应地配置信息或者拒绝控制

2、		控制（CMD_REQUEST_PTZ_CTRL）					接受控制命令，根据不同的命令类型进行处理

3、		请求预置点信息（CMD_REQUEST_PTZ_PRESET_INFO）	接受控制命令，处理并回复

4、		请求巡航线信息（CMD_REQUEST_PTZ_CRUISE_INFO）	接受控制命令，处理并回复

5、		请求轨迹信息（CMD_REQUEST_PTZ_TRACK_INFO）		接受控制命令，处理并回复

6、		结束云台控制（CMD_REQUEST_PTZ_STOP）		接受命令并处理

注：a、CMD_REQUEST_PTZ_START：有些设备可能没有接入云台设备，或者其他原因拒绝控制，需要返回失败码。
	b、CMD_REQUEST_PTZ_CTRL：用于手动控制云台的运动。
	d、CMD_REQUEST_PTZ_STOP：当结束云台控制时，发送给服务器端以便做一些必要的处理。

客户端发送数据格式：	
	a、CMD_REQUEST_PTZ_CTRL - 控制参数（PTZ_CTRL）
	b、CMD_REQUEST_PTZ_CTRL - 控制参数（PTZ_CTRL：PTZ_CMD_PRESET_SET，PTZ_CMD_CRUISE_SET）- 数据（见注解）
	c、CMD_REQUEST_PTZ_PRESET_INFO - 通道号（四字节）
	d、CMD_REQUEST_PTZ_CRUISE_INFO - 通道号（四字节）
	e、CMD_REQUEST_PTZ_TRACK_INFO - 通道号（四字节）

注：巡航线数据的格式：	PTZ_CURISE - PTZ_CURISE_POINT X N

服务器端回复数据格式：	
	a、命令字段 - 失败信息码
	b、CMD_REPLY_PTZ_PRESET_INFO - 预置点数目（四字节）- PTZ_PRESET X N
	c、CMD_REPLY_PTZ_CURISE_INFO - 巡航线数目（四字节）- （PTZ_CURISE - （PTZ_CURISE_POINT X N）） X N
	d、CMD_REPLY_PTZ_TRACK_INFO - 轨迹线数目（四字节）
*****************************************************************************************************************/


/**********************************************磁盘管理握手协议***************************************************
		客户端										服务器端

1、		发出请求（CMD_REQUEST_DISK_START）			接受到请求，处理后返回对应地磁盘信息或者拒绝控制

2、		控制（CMD_REQUEST_DISK_FORMAT）				格式化磁盘（建立在客户端检查了录像状态后）

3、		结束磁盘管理（CMD_REQUEST_DISK_STOP）		接受命令并处理

注：a、CMD_REQUEST_DISK_START：有些设备可能没有接入磁盘，或者其他原因拒绝控制，需要返回失败码。
	b、CMD_REQUEST_DISK_FORMAT：客户端需要检查录像状态，只有处于非录像状态时才格式化。
	c、CMD_REQUEST_DISK_STOP：当结束磁盘管理时，发送给服务器端以便做一些必要的处理。
	d、格式成功后，需要返回CMD_PEPLY_FORMAT_SUCC，并附带刚给格式化磁盘的信息。

客户端发送数据格式：	
	a、磁盘管理：	命令字段 - 无
	b、磁盘格式化：	命令字段 - 磁盘类型 - 磁盘序号
	c、退出管理：	命令字段 - 无

服务器端回复数据格式：	
	a、回复磁盘信息：		命令字段 - 磁盘数目（N，四字节） - 信息数据（DVR_DISK_INFO * N）
	b、回复磁盘格式化失败：	命令字段（CMD_REPLY_FORMAT_SUCC）
*****************************************************************************************************************/
typedef struct _net_motion_info
{
	unsigned long chnn;         //通道号
	MOTION_DETECT_INFO detectInfo;	//侦测的相关信息。
}NET_MOTION_INFO;


/***************************************移动侦测握手协议*********************************************************
客户端                                          服务器端

1、发出请求 (CMD_REQUEST_MOTION_AREA_GET)       接收到请求后，返回NETDEVICE_MOTION_INFO和区域信息，或者失败
2、发出请求 (CMD_REQUEST_MOTION_AREA_SET)       设置区域信息后，返回成功或失败

客户端发送数据格式:     命令字段 - 通道号(unsigned long)
命令字段 - 区域信息(NET_MOTION_INFO)

服务器端回复数据格式:   命令字段 - 区域信息(NET_MOTION_INFO)
命令字段 - 无
*****************************************************************************************************************/
typedef struct _net_update_info
{
	unsigned long totalLen;		//数据总长度
	unsigned long index;//包的编号从1  到100  表示进度， 0表示失败 
	unsigned long currPageLen;	//分包长度
}NET_UPDATE_INFO;
/**********************************************升级握手协议***************************************************
客户端										服务器端

1、		发出请求（CMD_REQUEST_UPDATE_START）			接受到请求，回复接受或者拒绝（无权限）

2、		发送升级包（CMD_REQUEST_UPDATE_DATE）						接受升级包

3、		放弃升级（CMD_REQUEST_UPDATE_CANCEL）						接受命令并处理

注：a、升级包采用分包的形式发送，所以需要进行组合处理。
b、放弃升级，则停止接受升级包，并删除已经接受的数据。

客户端发送数据格式：	命令字段 - 无
命令字段 - 分包信息 - 数据包

服务器端回复数据格式：	命令字段 - 回复码
*****************************************************************************************************************/
typedef struct _net_status
{
	unsigned long chnn_count;	//本地通道个数		
	ULONGLONG ullstatus;			//每位代表相关通道的状态
}NET_STATUS;
/**********************************************状态信息握手协议**************************************************
服务器端

服务器端回复数据格式：	命令字段 - 状态数据（video loss, motion, sensor）
命令字段 - 通道数目 - 录像类型数据 （n == chnn number）
命令字段 - 无
*****************************************************************************************************************/
typedef struct _net_rec_status
{
	unsigned long chnn_count;	//本地通道个数
	ULONGLONG ullManualRec;
	ULONGLONG ullScheduleRec;
	ULONGLONG ullMotionRec;
	ULONGLONG ullSensorRec;
}NET_REC_STATUS;

//回放类型：按文件，按时间
typedef enum _enum_playback_type
{
	PLAYBACK_ON_TIME,
	PLAYBACK_ON_FILE,
}ENUM_PLAYBACK_TYPE;

//本地客户端开始回放时用到结构体
typedef struct _local_client_playback_info
{
	unsigned long  playbackType;
	unsigned long  starTime;
	unsigned long  endTime;
	unsigned long  curTime;
	unsigned long  pFileList;
	unsigned long  splitMode;
	ULONGLONG      chnnBits;
	unsigned long  pMsgCallBack;
	unsigned long  pCallBackObj;
    unsigned long  diskOwnerIndex;
	bool		   bSync;
}LOCAL_CLINET_PLAYBACK_INFO, *PLOCAL_CLINET_PLAYBACK_INFO;

//本地客户端开始回放时用到结构体
typedef struct _local_client_pb_chg_ch_info
{
	unsigned long	splitMode;    
	ULONGLONG		chnnBits;
}LOCAL_CLINET_PB_CHG_CH_INFO, *PLOCAL_CLINET_PB_CHG_CH_INFO;

//本地客户端控制回放时，服务器端的回复，用到该结构体
typedef struct _local_client_playback_ctrl
{
	unsigned long playbackctrlcmd;  //发送回放控制命令
	unsigned long playbackreply;    //服务端的回复信息
}LOCAL_CLINET_PLAYBACK_CTRL_INFO;


typedef struct _curchnn_info_
{
	unsigned long clickchnn;//点击的通道号
	bool bsync;//是否同步，true为同步,false为异步
	unsigned long speed;//区分是真的暂停还是一倍速度播放
}CURCHNNINFO;

//本地客户端开始备份时用到结构体
typedef struct _local_client_backup_info
{
	unsigned long    pFileInfoBackup;
	unsigned long    fileNum;
	unsigned long    startTime;
	unsigned long    endTime;
	unsigned long    pDiskName;
	unsigned long    pMountDir;
	unsigned long    diskType;
	unsigned long    dataType;
	unsigned long    callBack;
	unsigned long    pCallBackObj;
	unsigned long    bHaveVideoPlay;
	unsigned long    bCombined;
	unsigned long    diskOwnerby;
}LOCAL_CLIENT_BACKUP_INFO;

typedef struct _crytoinfo
{
	unsigned long languageBits;
	unsigned char productSubID;
}CRYTOINFO;

//点击回放按钮时候用的的结构体
typedef struct _struct_clicklive_
{
	NET_DISPLAY_INFO curDisp;
	ULONGLONG   chnnBits;
}LIVESTRUCT;

typedef struct _yuanzheng_manual_rec_param_
{
	unsigned char   channel;		//对哪个通道进行操作
	unsigned char   recStatus;      //1-开始录像 0-结束录像     
	unsigned long	recTime;		//表示录像多长时间，单位为秒
}YUANZHENG_MANUAL_REC_PARAM;

typedef struct _yuanzheng_param_
{
	char		ipUrl[128];
	char		asCode[64];
}YUANZHENG_PARAM;

typedef struct _yuanzheng_snap_param_
{
	char		  ipUrl[128];
	unsigned long timeS;
}YUANZHENG_SNAP_PARAM;

//
typedef struct _ip_device_info_
{
	unsigned long	deviceID;
	unsigned short	channel;	//该网络设备在本地分配的通道
	unsigned short	status;		//连接状态

	char			szEtherName[16];		//如果为空，默认为eth0
	char			szServer[64];			//网络设备IP地址
	unsigned short	nPort;		//网络设备端口号
	unsigned short	nHttpPort;	//HTTP端口
	unsigned short	nCtrlPort;	//控制端口，一般与nPort相同
	char			szID[64];	//网络设备标识(或是MAC地址)
	char			username[36];//注册到网络设备上时所用的用户名
	char			password[36];//注册到网络设备上时所用的密码

	unsigned long	manufacturerId;	//设备厂商ID
	char			manufacturerName[36];//设备厂商名
	char			productModel[36];//产品型号

	unsigned char	bUseDefaultCfg;
	unsigned char	bPOEDevice;
	unsigned char	resv[2];
}IP_DEVICE_INFO;

typedef struct _discover_device_info
{
	char			szAddr[64];
	unsigned long	netmask;
	unsigned long	gateway;
	unsigned short	netport;
	unsigned short	nHttpPort;
	unsigned short	nCtrlPort;
	char			szID[64];	//网络设备标识
	unsigned long	manufacturerId;
	char			manufacturerName[36];
	char			productModel[36];
}DISCOVER_DEVICE_INFO;

typedef struct _nvr_support_manufacturer
{
	unsigned long	manufacturerId;
	char			manufacturerName[36];
	unsigned char	bNeedHttpPort;		//是否需要HTTP端口进行通信
	unsigned char	bNeedCtrlPort;		//是否需要专门的控制端口通信
	unsigned char	resv[2];
}NVR_SUPPORT_MANUFACTURER;

typedef struct _af_control_
{
	AF_CTRL_TYPE afType;
	int			 nValue;  //手动微调时，1代表+，0代表-，其他动作类型nValue就传0
}AF_CONTROL;

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack()
#endif

#endif //_NET_PROTOCOL_H_
