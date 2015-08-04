#ifndef _HXHT_CONST_COMMANDS_H_
#define _HXHT_CONST_COMMANDS_H_ 
  
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-07-30
** Name         : hxhtConstCommands.h
** Version      : 1.0
** Description  :
				DVR发向HXHT网络设备的命令，
				包括:
				1:DVR对HXHT网络设备的请求
				2:DVR对HXHT网络设备的应答
** Modify Record:

***********************************************************************/


/*********************************************************************************************************************/
//设备注册时DVR主动向接入服务器发送的命令
const char *pTermRegisterXml =
	"<TermRegister\r\n"
		"DeviceID=\"1\"\r\n"
		"DeviceIP=\"192.168.3.58\"\r\n"
		"DeviceLinkType=\"1\"\r\n"
		"DeviceMaxConnect=\"16\">\r\n"
		"<MsgPort>4602</MsgPort>\r\n"
		"<VideoPort>4602</VideoPort>\r\n"
		"<AudioPort>4602</AudioPort>\r\n"					
		"<UpdatePort>4602</UpdatePort>\r\n"
		"<WebPort>4602</WebPort>\r\n"
		"<Version>1</Version>\r\n"
		"<Protocol>TVTDigital_H264</Protocol>\r\n"
		"<Support\r\n"
			"NAT=\"false\"\r\n"
			"Scheme=\"true\"\r\n"
			"PtzLockRet=\"false\"\r\n"
			"Video2=\"true\"\r\n"
			"MainStream=\"true\"\r\n"
			"SubStream1=\"false\"\r\n"
			"SubStream2=\"false\"\r\n"
			"SubStream3=\"false\"/>\r\n"
	"</TermRegister>\r\n";


/*********************************************************************************************************************/
//存储注册
const char *pStoreRegisterXml =
		"<StoreRegister\r\n"
			"DeviceID=1\r\n" 
			"DeviceLinkType=4\r\n"
			"DeviceIP=0.0.0.0\r\n"
			"DeviceIP2=0.0.0.0>\r\n"
			"<VideoPort>0</VideoPort>\r\n"
			"<MsgPort>0</MsgPort>\r\n"
		"</StoreRegister>\r\n";
/*********************************************************************************************************************/

/*********************************************************************************************************************/
//DVR的心跳请求命令
const char *pKeepAliveXml =
	"<Message>\r\n"
		"<DevType>2</DevType>\r\n"
		"<Expires>120</Expires>\r\n"
	"</Message>\r\n\r\n"
;


/*********************************************************************************************************************/

/*********************************************************************************************************************/
//视频请求的应答
const char *pReplayVideoRequestXml = 
	"<Message>\r\n"
		"<Ticket>ticket</Ticket>\r\n"
		"<VideoPort>4602</VideoPort>\r\n"
		"<Protocol>TVTDigital_H264</Protocol>\r\n"
		"<CurrConnectNum>1</CurrConnectNum>\r\n"
		"<ErrScription></ErrScription>\r\n"
		"<Version>2</Version>\r\n"
	"</Message>\r\n\r\n"
;

//客户端连DVR视频请求的应答
const char *ReplayClientConnetForVXml = 
	"<Message>\r\n"
		"<StreamType>MainStream</StreamType>\r\n"
	"</Message>\r\n\r\n"
;

//向接入服务器发送开始视频请求
const char *pStartVideoRequestXml = 
"<Message>\r\n"
	"<Ticket>ticket</Ticket>\r\n"
	"<DevType>2</DevType>\r\n"
	"<Switch>1</Switch>\r\n"
	"<StreamType>MainStream</StreamType>\r\n"
"</Message>\r\n\r\n"
;
/*********************************************************************************************************************/
//下载方式查询的应答, 这个xml文件中不含Message头
const char *pReplayVDLTypeXml = 
"<ftpinfo user=\"admin\" pwd=\"123456\" port=\"4602\" type=\"STREAM_TCP\">\r\n"
"</ftpinfo>\r\n"
;
/*********************************************************************************************************************/
//云台状态应答信息，SwitchStatus的属性中缺少巡航状态和轨迹状态设置方式，没有巡航线和轨迹的配置信息设置方式、Support的属性中可以添加其他属性
const char *pPTZStatusXml = 
"<Message>\r\n"
	"<Status></Status>\r\n"
	"<SwitchStatus RandomScan=\"off\"/>\r\n"
	"<Preset>0</Preset>\r\n"
	"<PresetSetting Count=\"0\">\r\n"
	"</PresetSetting>\r\n"
	"<Support Direct=\"y\" Bright=\"y\" Zoom=\"y\" Focus=\"y\" >\r\n"
	"</Support>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/
//状态报告
const char *pDeviceStatusXml =
"<Message Version=\"1.0\">\r\n"
	"<Header SequenceNumber=\"\" SessionID=\"\"/>\r\n"
	"<Label HostType=\"\" IP=\"\" ItemType=\"\" ItemTable=\"\"/>\r\n"
	"<Body>\r\n"
	"<GatherTime>2010-08-29 17:43:20</GatherTime>\r\n"
		"<DeviceID>00</DeviceID>\r\n"
	"</Body>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

const char *pReportAlarmInfoXml =
"<Message>\r\n"
	"<Naming>Naming</Naming>\r\n"
	"<DevNaming>DevNaming</DevNaming>\r\n"
	"<DevType>5</DevType>\r\n"
	"<AlarmTime>2010-08-30</AlarmTime>\r\n"
	"<Type>type</Type>\r\n"
	"<Info>info</Info>\r\n"
	"<From>from</From>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

const char *pRepScheduleXml =
"<Message>\r\n"
	"<Plan>0</Plan>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/
const char *pRepSetAlarmOutStatusXml = 
"<Message>\r\n"
	"<ErrScription></ErrScription>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/
const char *pRepGetAlarmOutStatusXml = 
"<Message>\r\n"
	"<State>0</State>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pRepGetCaptionXml=
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<Message>\r\n"
	"<Para>6</Para>\r\n"
	"<Record\r\n"
	"bOSDing=\"0\"\r\n"
	"nOSDPlace=\"0\"\r\n"
	"nWordPlace=\"0\"\r\n"
	"Word=\"0\"\r\n"
	"/>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/
const char *pFaultXml =
"<Message Version=\"1.0\">\r\n"
	"<Header SequenceNumber=\"65535\" SessionID=\"123456\"/>\r\n"
	"<Label HostType=\"TERMINAL\" IP=\"192.168.1.100\" ItemType=\"FAULT\" ItemTable=\"DEVICE\"/>\r\n"
	"<Body>\r\n"
		"<GatherTime>2010-02-21 16:30:00</GatherTime>\r\n"
		"<DeviceID>ID</DeviceID>\r\n"
		"<DeviceType>001</DeviceType>\r\n"
		"<FaultType>1</FaultType>\r\n"
		"<FaultID>502</FaultID>\r\n"
		"<FaultTime>2010-02-21 16:30:00</FaultTime>\r\n"
		"<IsClear>0</IsClear>\r\n"
	"</Body>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pReplayGetCoverXml=
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<Message>\r\n"
"<Para>5</Para>\r\n"
"<Record\r\n"
"bLogoing=\"0\"\r\n"
"Num=\"0\"\r\n"
">\r\n"
"</Record>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pReplayGetMotionXml=
"<Message>\r\n"
"<Para>8</Para>\r\n"
"<Record\r\n"
"bDectecting=\"0\"\r\n"
"DetectGrade=\"0\"\r\n"
"Num=\"0\"\r\n"
">\r\n"
"</Record>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pReplayGetVideoXml1=
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<Message Stream=\"MainStream/SubStream1/SubStream2/SubStream3\">\r\n"
"<Para>1</Para>\r\n"
"<Record FrameRate=\"0\" KeyFrameIntervals=\"0\" />\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pReplayGetVideoXml2=
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<Message Stream=\"MainStream/SubStream1/SubStream2/SubStream3\">\r\n"
"<Para>2</Para>\r\n"
"<Record Level=\"0\" />\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pReplayGetVideoXml3=
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<Message Stream=\"MainStream/SubStream1/SubStream2/SubStream3\">\r\n"
"<Para>3</Para>\r\n"
"<Record  MaxBPS=\"0\" />\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pReplayGetVideoXml4=
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<Message Stream=\"MainStream/SubStream1/SubStream2/SubStream3\">\r\n"
"<Para>4</Para>\r\n"
"<Record PicFormat=\"0\" SupportFormat=\"0\" />\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/

const char *pReplayGetVideoXml7=
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"<Message Stream=\"MainStream/SubStream1/SubStream2/SubStream3\">\r\n"
"<Para>7</Para>\r\n"
"<Record Brightness=\"0\" Contrast=\"0\" Saturation=\"0\" Hue=\"0\" />\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/
const char *pReplaySucc=
"<Message>\r\n"
"<ErrScription>succ!</ErrScription>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/
const char *pRePlayVersion=
"<Message>\r\n"
"<Version>0</Version>\r\n"
"<BuildDateTime>0</BuildDateTime>\r\n"
"<ErrScription>succ!</ErrScription>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/
const char *pRePlayNetWork=
"<Message>\r\n"
"<IP>0</IP>\r\n"
"<NetMask>0</NetMask>\r\n"
"<GateWay>0</GateWay>\r\n"
"<ErrScription>succ!</ErrScription>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/*********************************************************************************************************************/
const char *pRePlayDisk=
"<Message>\r\n"
"<HardDisk\r\n"
"Type=\"0\"\r\n"
"DiskSpace=\"0\"\r\n"
"DiskSpaceRatio=\"0\"\r\n"
"DiskSpaceBalance=\"0\"\r\n"
"/>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/
/***************************语音对讲端口号***********************************************************************/
const char *pRePlayPort=
"<Message>\r\n"
"<Port>0</Port>\r\n"
"</Message>\r\n"
;
/*********************************************************************************************************************/

/********************************语音参数****************************************************************************/
const char *pRePlayAudioParam=
"<Message>\r\n"
"<nChannels>0</nChannels>\r\n"
"<wBitsPerSample>0</wBitsPerSample>\r\n"
"<nSamplesPerSec>0</nSamplesPerSec>\r\n"
"<CompressedFormat>0</CompressedFormat>\r\n"
"</Message>\r\n"
;
#endif /*_HXHT_CONST_COMMANDS_H_*/


