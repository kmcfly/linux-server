

/////////////////////////DeviceTypeHeader.h
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : �Ź���
** Date         : 2008-04-26
** Name         : DeviceTypeHeader.h
** Version      : 1.0
** Description  :
** Modify Record:
1:2008-1-14 csg ������������֧�ֵ��豸���͵Ķ���,˵���������ļ��ݶ����豸���͵Ĵ���ʽ��
		
***********************************************************************/

/*	
	��ģ����ҪĿ���Ƕ��壬�ǿ��ǵ����п�������֧�ֵ��豸���ͱ���ͳһ����

*/


#ifndef	_DEVICE_TYPE_H_
#define _DEVICE_TYPE_H_

namespace DEVICE_TYPE_DEF
{
  	/************************************************************************/                                                                  
	/* 	�豸������Ϣ������ͷ	*/
	/************************************************************************/
	const unsigned long DEVICE_TYPE_HEADFLAG = ( (unsigned long)(unsigned char)('h') | ( (unsigned long)(unsigned char)('e') << 8 ) |	\
		( (unsigned long)(unsigned char)('a') << 16 ) | ( (unsigned long)(unsigned char)('d') << 24 ) );		
		


  	/************************************************************************/                                                                  
	/* 
		���豸���Ͷ���ı�׼Ϊ:  �豸����_ϵ��_ͨ������
		�豸����
			DVS/IPCAMERA/DVR/DVRCARD/CMS/DECODER(����������뿨)
		ϵ��:
			DVS: 11ϵ��
			IPCAMERA: 11��12ϵ��
			DVR:20��23��24ϵ��
			DVRCARD��30��31��33��41ϵ��	 */
	/************************************************************************/
	typedef enum _tvt_product_type_
	{
		TVT_PRODUCT_DVR,			//Ƕ��ʽӲ��¼���
		TVT_PRODUCT_DVS,			//Ƕ��ʽ��Ƶ������
		TVT_PRODUCT_IPC,			//Ƕ��ʽ���������
		TVT_PRODUCT_NVR,			//Ƕ��ʽ����Ӳ��¼���
		TVT_PRODUCT_MDVR,			//Ƕ��ʽ����Ӳ��¼���
		TVT_PRODUCT_DECODEC,		//Ƕ��ʽ������
		TVT_PRODUCT_STORAGE,		//�洢��
		TVT_PRODUCT_INTELLIGENCE,    //���ܷ���ģ��
		TVT_PRODUCT_NETKEYBOARD,    //�������
		TVT_PRODUCT_CMS,			//�������������
		TVT_PRODUCT_ECMS,			//רҵ�����ƽ̨
		TVT_PRODUCT_CLIENT,			//����ͻ���
		TVT_PRODUCT_MOBILE_APP,		//�ƶ��ͻ���
	}TVT_PRODUCT_TYPE;

	//////////Ƕ��ʽDVR�Ĳ�Ʒ���Ͷ���
	enum PRODUCT_DVR
	{
		TD2304L=0,//ԭ��ʹ�õ���0��һֱ������0
		TD2304=3,
		TD2308,
		TD2316,
		TD2404,
		TD2408,
		TD2416,
		TD2304M,//9
		TD2308M,
		TD2316M,
		TD2504=21,
		TD2508,
		TD2516,
		TD2316C,
		TD5004,		//��·���ػ�
		TD5008,		//��·���ػ�
	};  

	////DVS�Ĳ�Ʒ���Ͷ���
	enum PRODUCT_DVS
	{		
		TS_1101_VSA,
		TS_1104,
		TS_1101_VSB,
		TS_1201,
		TS_1202,
		TS_1204,
	};

	///////IPCamera�Ĳ�Ʒ���Ͷ���
	enum PRODUCT_IPCAMERA
	{
		TC_1118,
		TC_1218	,
		TC_201CAB,//30������
		TC_211CAB,//100������
		TC_301CAB,//CCD	
		TC_211HCAB,//130������
		TC_100,//ָֻװCCD��ģ�飬�����ٶ���
		TC_201L,//201 8M FLASH�Ĳ�Ʒ���Ͷ��壬���⻥������
		TC_612M,//211��ǹ����Ʒ,������211��ȫһ��
		TC_221,//8M FLASH,200������,1600X1200�ֱ���
		TC_622,//��TC_221����һ��,ǹ����Ʒ,TC622M
		TC_20l_G3,//�����ƶ����Ʋ�Ʒ��CMS��SDK���ͻ��˶�����֧�֣��˲�Ʒֱ�ӽ���ƽ̨��
		TC_202,//20100112�ںˡ�armbootͬTC100
		TC_562,//G�汾500MHZ��cpu������ͬTC100

		//�����Ʒ�ķ����õ��Ǻ�˼��
		TC_612F=1000,
		TC_622F,
		TD_9322,
		TD_9321,
		TD_9422,
		TD_9421,
		TD_9522,
		TD_9521,
		TD_9322D,
		HI_END= 1099,

		//����Ĳ�Ʒ��8126��оƬ
		TD_9321L = 1100,		//cmos 720P
		TD_9314,				//CCD 30��
		TD_9421L,
		TD_9411,
	};  

	/////SuperDVR�Ĳ�Ʒ���Ͷ���
	enum  PRODUCT_SUPERDVR
	{
		DVR_CARD_TDNULL,
		DVR_CARD_TD3004,
 		DVR_CARD_TD3016,
		DVR_CARD_TD3116,
		DVR_CARD_TD3216,
		DVR_CARD_TD3108,
		DVR_CARD_TD3008,
		DVR_CARD_TD3316,
		DVR_CARD_TD3101,
		DVR_CARD_TD3104,
		DVR_CARD_TD4104,
		DVR_CARD_TD3316V1,
		DVR_CARD_TD4116,
		DVR_CARD_TD4108,
		DVR_CARD_TD4408,
		DVR_CARD_TD3304,
		DVR_CARD_TD4404,
		DVR_CARD_TD4308,
		DVR_CARD_TD4316,
		DVR_CARD_TD4304,
		DVR_CARD_TD4404L,
		DVR_CARD_TD4408L,
		DVR_CARD_TD4416L,
		DVR_CARD_TD3304NC
	};

	enum PRODUCT_DECODER
	{
		DECODER_1101D
	};
	
	/************************************************************************/
	/*   ���������������豸������յ�����Ľṹ����Ϣ���н�����ͨ���豸���ʹ�����Ӧ��  
	�������ͬʱ�����64���ֽڵ���Ϣֱ��ͨ���������ݸ��������*/                                                                     
	/************************************************************************/	
	typedef struct _net_device_type_info
	{
		unsigned long headFlag; ////ȡֵΪHEADFLAG
		unsigned long devicetype; /////////�豸���� DVS  IPCAMERA  DVR  SUPERDVR
		unsigned long producttype;  /////��Ʒ����
		unsigned long netprotrocolver; ///////////�豸��Ӧ������Э��汾
		unsigned long configver; ///////////�豸��Ӧ�����ð汾
		unsigned long devID; //////
		unsigned long encryptType;	//���ݼ��ܷ�ʽ��0 ��ʾû�м��ܷ�ʽ
		unsigned long encryptParam;	
		unsigned long ulReserve[7];/////������Ϣ���Ա��Ժ�����
		unsigned long dwUDPDefaultVideoStream;	//���ڴ�͸Ĭ�ϵ���
	}NETDEVICE_TYPE_INFO, *LPNETDEVICE_TYPE_INFO;

}  ////////////namespace DEVICE_TYPE_DEF end

//IPCamera �ļ��ܷ�ʽ
//encryptType Ϊ1.0��ʱ��Ϊ��򵥵�ֻ���û������û�������м��ܣ�
//encryptParam Ϊ�����õ��Ĳ�����1.0��ʱ������򵥵��������

//huxinhua add begin
//struct for iptool to update u-boot and kernel
typedef struct _ipcamera_version_check_info
{
    unsigned long checkflag;       //DEVICE_TYPE_HEADFLAG
    unsigned long packagetype;    // u-boot 1, kernel 2, update package 3(not use now)
    unsigned long devicetype;     // IPCAMERA    DVS    DVR   ...
    unsigned long producttype;    // 9322  9321 .....
}IPCAMERA_VERSION_CHECK_INFO;
//huxinhua add end

#endif /////#ifndef _DEVICE_TYPE_H_








