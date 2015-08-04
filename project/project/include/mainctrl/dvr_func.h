
#ifndef __DVR_FUNC_H__
#define __DVR_FUNC_H__

#include "Typedef.h"
#include "device.h"
#ifdef __ENVIRONMENT_LINUX__
#include "lvfw.h"
#else
#include <vfw.h>
#endif
#include "IPDataBufferDefine.h"

#ifdef   __cplusplus   
	#define   EXTERN_C           extern   "C"   
#else   
	#define   EXTERN_C           extern   
#endif   

#define EXT_FLASH_PARAM_MAC_ADDR 0
#define EXT_FLASH_PARAM_HD_VERSION 1
#define EXT_FLASH_PARAM_SUB_VER 2
#define EXT_FLASH_PARAM_OUT_DEVICE 3
#define EXT_FLASH_PARAM_VIDEO_FORMAT 4
#define EXT_FLASH_PARAM_DDNS_TYPE      5
#define EXT_FLASH_PARAM_KEYBOARD_NAME  6
#define EXT_FLASH_PARAM_FUNC_CUSTOM    7
#define EXT_FLASH_PARAM_OUT_RESOLUTION 8
#define EXT_FLASH_PARAM_NETDEVICE_TYPE 10   // 1 -- 百兆, 其他 -- 千兆  NetChip.dat
#define EXT_FLASH_APP_KERNEL_UBOOT_VER 4096

#define EXT_AUDIO_SPEEK_NONE 0
#define EXT_AUDIO_SPEEK_DEVICE1 1
#define EXT_AUDIO_SPEEK_USE_CHL0 2

#define EXT_IP_IN_BUF_NUM 16

#define EXT_DVR_MAX_CHANNEL 128

//SDK是否支持功能
#define EXT_GET_SDK_PARAM_LIVE_COLOR		0
#define EXT_SET_SDK_PARAM_CVBS_SUPPORT		1
typedef enum __jpeg_attribute__
{
	JPEG_ATTRIBUTE_NO_SNAP	= 0,		//不支持抓图
	JPEG_ATTRIBUTE_NO_OSD	= 1,		//支持抓图,图片上无水印信息
	JPEG_ATTRIBUTE_HAVE_OSD	= 2,       ////支持抓图,图片上有水印信息
}JPEG_ATTRIBUTE;

typedef enum __local_pb_attribute__
{
	LOCAL_PB_ATTRIBUTE_SDK_CTRL                = 0,
	LOCAL_PB_ATTRIBUTE_NONE				= 1,	//回放时不做任何处理
	LOCAL_PB_ATTRIBUTE_HALF_RECORD		= 2,	//回放时录像降帧到1/2
	LOCAL_PB_ATTRIBUTE_QUARTERN_RECORD	= 3,	//回放时录像降帧到1/4
}LOCAL_PB_ATTRIBUTE;


typedef enum __video_type__
{
	VIDEO_TYPE_720P60	 = 0x00,	
	VIDEO_TYPE_720P50	 = 0x01,	
	VIDEO_TYPE_1080P30 = 0x02,	
	VIDEO_TYPE_1080P25 = 0x03,	
    VIDEO_TYPE_720P30 = 0x04,
	VIDEO_TYPE_720P25  = 0x05,
	VIDEO_TYPE_960H30 = 0x06,
	VIDEO_TYPE_960H25 = 0x07,
	VIDEO_TYPE_HDCIF30 = 0x08,
	VIDEO_TYPE_HDCIF25 = 0x09,
	VIDEO_TYPE_720P60_AHD = 0x0a,	
	VIDEO_TYPE_720P50_AHD = 0x0b,	
	VIDEO_TYPE_1080P30_AHD = 0x0c,	
	VIDEO_TYPE_1080P25_AHD = 0x0d,	
	VIDEO_TYPE_720P30_AHD = 0x0e,
	VIDEO_TYPE_720P25_AHD  = 0x0f,
	VIDEO_TYPE_960H30_AHD = 0x10,
	VIDEO_TYPE_960H25_AHD = 0x11,
	VIDEO_TYPE_HDCIF30_AHD = 0x12,
	VIDEO_TYPE_HDCIF25_AHD = 0x13,
}VIDEO_TPYE;

#pragma pack(4)

typedef struct _ext_dvr_input_param
{
	int videoFormat;
	int videoLocalSize[EXT_DVR_MAX_CHANNEL];
	int videoNetSize[EXT_DVR_MAX_CHANNEL];
	int videoChnnNum;
	int audioChnnNum;
	
	// 0~7bit 表示pb channel number
	// 8~15bit 表示 回放降帧处理方式
	// 16~23bit 表示录像最高支持大小 0 -> D1   1->960H
	int videoPbChnnNum;
	
	int video_out_mode;
	int dvr_type;
	int dvr_sub_version;
	int video_out_width;
	int video_out_height;
	int audio_speek_type;
	int jpeg_capture_enable;
}ext_dvr_input_param;



typedef struct _ext_dvr_modin_param
{
	int dvr_type;
	int videoFormat;
	int pci_device_cnt;
	int videoLocalSize[EXT_DVR_MAX_CHANNEL];
	int videoNetSize[EXT_DVR_MAX_CHANNEL];
	int videoChnnelNum;
	int videoPbChannelNum;
	
	int videoOutMode;
	int videoOutImageWidth;
	int videoOUtImageHeight;
	int videoOutTotalChl;
	
	int audioIsProcess;
	int audioDeviceNum;
	int audioPBDeviceNum;
	int one_ai_chl_need_enc_chl_num;
	int audioChlNumPreDev;
	int audioChnnelNum;
	
	int pciMasterDevice;

	int cascadeIsProcess;
	int cascadeMasterDevice;
	
	int audioIsSpeek;
	int audioSpeekType;
	int jpegIsEnable;
	
	int audioUsedAic23Speek;

	int videoSupportIPinFunc;

	// 340 : HD M340 chip
	//    0  : tw2864 nextchip 1104a
	int audioOutChipType;

	// ext2 version
	int app_is_ext2_ver_val;
	int app_disp_mode;
	
	int dvr_sub_type_val;
	int dvr_sub_dev_version;
}ext_dvr_modin_param;

typedef struct _ext_dvr_fb_screen_info
{
	int width;
	int height;
	int stride;
	int bytes;
	unsigned char* p_begin_addr;
	int phy_address;
	int frame_buf_number;
}ext_dvr_fb_screen_info;


typedef struct _ext_dvr_video_tpye_chg_info
{
	unsigned int channel;
	VIDEO_TPYE video_type;
	VIDEO_TPYE video_type_old;
}ext_dvr_video_tpye_chg_info;

typedef struct _ext_dvr_tde_buf_info
{
	unsigned char* vir_addr;
	int phy;
	int width;
	int height;
	int stribe;
	int cp_x;
	int cp_y;
	int cp_width;
	int cp_height;
}ext_dvr_tde_buf_info;

typedef struct _ext_dvr_tde_op_param
{
	ext_dvr_tde_buf_info src_buf;
	ext_dvr_tde_buf_info dst_buf;

	int fill_data;
}ext_dvr_tde_op_param;

typedef struct _ext_dvr_rtc_time
{
	unsigned int second;
	unsigned int minute;
	unsigned int hour;
	unsigned int dayofweek;
	unsigned int day;
	unsigned int month;
	unsigned int year;
}ext_dvr_rtc_time;

typedef struct _ext_dvr_product_info
{
	int product_id;
	int product_language;
	int product_type;
	int resv0;
	int resv1;
	int resv2;
}ext_dvr_product_info;

typedef struct _ext_dvr_ip_in_buf_addr
{
	int buf_number;
	unsigned char* buf_vir_addr[EXT_IP_IN_BUF_NUM];
	int buf_vir_len[EXT_IP_IN_BUF_NUM];
}ext_dvr_ip_in_buf_addr;

typedef struct _ext_dvr_ip_in_buffer_param
{
	long			chipId;
	unsigned long	dataLen;
	long			*pStatus;
	unsigned char	*pData;
}ext_dvr_ip_in_buffer_param;

typedef struct _ext_zorder
{
	int chnn;
	int zorder;
}ext_zorder;


typedef struct _ext_video_area_
{
	unsigned short recv;
	unsigned short enable;	//是否有效

	unsigned short x;		//横坐标(1/10000)
	unsigned short y;		//纵坐标(1/10000)

	unsigned short cx;		//宽度(宽度和高度为零，表示无效）(1/10000)
	unsigned short cy;		//高度(宽度和高度为零，表示无效）(1/10000)
}EXT_VIDEO_AREA;


typedef struct _ext_deccard_fn_list
{
	int (*set_video_format)(int fmt);

	int (*process_command)(unsigned char* p_cmd, int cmd_len );

	int (*insert_buf_stream_que)(unsigned char* buf_addr, int phy_addr, int buf_len);
	unsigned char* (*get_mem_buf_addr)( int * phy_addr, int * buf_len );
	
}ext_deccard_fn_list;

#pragma pack()


EXTERN_C int ext_change_boot_audio_format(int audioFormat); // 在ext_device_init函数之前调用决定音频格式

//设备初始化和退出
EXTERN_C int	ext_device_init( ext_dvr_input_param* p_dvr_input_param );
EXTERN_C int	ext_device_quit( int exitCode );

//使能捕捉图像
EXTERN_C int	ext_capture_start( int streamType );
EXTERN_C int	ext_capture_stop( int streamType );

EXTERN_C int	ext_get_video_format( int videoSize, BITMAPINFOHEADER* bitmapInfo );
EXTERN_C int	ext_get_audio_format( WAVEFORMATEX* waveFormat );


//获取数据码流
EXTERN_C int	ext_get_primary_video_stream( int chnn, FRAME_INFO_EX *pFrameInfo);
EXTERN_C int	ext_get_network_video_stream( int chnn, FRAME_INFO_EX *pFrameInfo);
EXTERN_C int	ext_get_audio_stream( int chnn, FRAME_INFO_EX *pFrameInfo);

//释放数据码流
EXTERN_C int ext_release_buf(FRAME_INFO_EX *pFrameInfo);	//PVS == primary video stream

//获取和释放对讲码流
EXTERN_C int ext_get_audio_speek_stream( FRAME_INFO_EX *pFrameInfo );
EXTERN_C int ext_release_audio_speek_buf( FRAME_INFO_EX *pFrameInfo );

//获取和释放JPEG 码流
EXTERN_C int ext_get_jpeg_stream( FRAME_INFO_EX *pFrameInfo );
EXTERN_C int ext_release_jpeg_buf( FRAME_INFO_EX *pFrameInfo );

//检测和设置图像输出设备
EXTERN_C int ext_current_output_device();
EXTERN_C int ext_change_output_device( int *VoDev, int *width, int * height );
EXTERN_C int ext_change_cvbs_output_size( int width, int height );

//设置画面显示优先级
EXTERN_C int ext_change_display_zorder(ext_zorder *pZorder, int chnnNum);

//更改显示画面
EXTERN_C int ext_change_display_format(DISPLAY_ATTRIB *pDispAttrib_app, int chnnNum);
EXTERN_C int ext_change_display_slave(DISPLAY_ATTRIB *pDispAttrib_app, int chnnNum);
EXTERN_C int ext_change_spot_display(int chl);

//回放开始和停止，回放缓冲区获取与释放
EXTERN_C int ext_playback_stop(void);
EXTERN_C int ext_playback_start(unsigned long ulChannlInfo);
EXTERN_C int ext_playback_restart(unsigned long ulChannlInfo);
//回放暂停，每位对应一个通道，非0表示暂停，0表示正常播放
#if  !defined(__CHIPGM__)
EXTERN_C int ext_playback_pause(unsigned long ulChnn, char bPause);
#endif

EXTERN_C int ext_playback_get_buf(FRAME_INFO_EX *pFrameInfo);
EXTERN_C int ext_playback_set_buf(FRAME_INFO_EX *pFrameInfo);

EXTERN_C int ext_ip_in_get_buf_addr( ext_dvr_ip_in_buf_addr* p_ip_in_addr, ext_dvr_ip_in_buf_addr* p_ip_in_small_addr );
EXTERN_C int ext_ip_in_start( unsigned long ulChannelInfo, unsigned long ulChannelSmallInfo, unsigned int* chip_id );
EXTERN_C int ext_ip_in_stop(void);
EXTERN_C int ext_ip_in_set_buffer_data(ext_dvr_ip_in_buffer_param *frame_buffer);
EXTERN_C int ext_ip_in_small_set_buffer_data(ext_dvr_ip_in_buffer_param *frame_buffer);

EXTERN_C int ext_ip_in_audio_start( int chl );
EXTERN_C int ext_ip_in_audio_stop(void);

//对讲
EXTERN_C int ext_audio_speek_start(void);
EXTERN_C int ext_audio_speek_stop(void);
EXTERN_C int ext_audio_speek_get_buf(FRAME_INFO_EX *pFrameInfo);
EXTERN_C int ext_audio_speek_set_buf(FRAME_INFO_EX *pFrameInfo);


//更改通道的图像的尺寸
EXTERN_C int ext_change_local_video_size( int chnn, int videoSize );
EXTERN_C int ext_change_net_video_size( int chnn, int videoSize );

EXTERN_C int ext_set_voutdev_brightness(int vo_dev, int value);
EXTERN_C int ext_set_voutdev_hue(int vo_dev, int value);
EXTERN_C int ext_set_voutdev_contrast(int vo_dev, int value);
EXTERN_C int ext_set_voutdev_saturation(int vo_dev, int value);
EXTERN_C int ext_exchang_vga_hd_color( int flag_exchg );

EXTERN_C int ext_set_video_brightness(int chl, int value);
EXTERN_C int ext_set_video_hue(int chl, int value);
EXTERN_C int ext_set_video_contrast(int chl, int value);
EXTERN_C int ext_set_video_saturation(int chl, int value);
// 0  indicate video detected. 
// 1  indicate video loss
EXTERN_C int ext_get_video_loss_status( unsigned long long* pu64Value);

//更改QCIF的通道数目
EXTERN_C int ext_change_qcif_channel( int chnn );

//清空缓冲区数据
EXTERN_C int ext_clear_buffer(void);

//设置motion的参数
EXTERN_C int ext_set_motion_sensitive( int chnn, int sensitive );
EXTERN_C int ext_set_motion_area( int chann, MOTION_AREA_INFO *pMotionArea );
EXTERN_C int ext_get_motion_area( int chann, unsigned char* mb_md, int len );
EXTERN_C int ext_get_video_md_alarm( unsigned long long* video_motion );
/*遮挡报警的参数*/
EXTERN_C int	ext_set_shelter_area( int chnn, EXT_VIDEO_AREA *pShelterArea, int areaNum);
EXTERN_C int	ext_clean_shelter_area(int chnn, int shelter_idx );
EXTERN_C int	ext_get_shelter_alarm( unsigned char *pShelter, int chnnNum);

//设置区域遮挡的参数
EXTERN_C int ext_set_cover_area( int chnn, int cover_idx, int x, int y, int cx, int cy, unsigned long color);
EXTERN_C int ext_clean_cover_area(int chnn, int cover_idx );

//设置帧率
EXTERN_C int ext_change_local_frame_rate( int chnn, int frm_nCount );
EXTERN_C int ext_change_net_frame_rate( int chnn, int frm_nCount );
EXTERN_C int ext_change_default_rate(void);

//设置图像画质
//bitStream代表码流大小，quality代表画质档次
EXTERN_C int ext_change_local_img_quality( int chnn, VIDEO_ENCODE_MODE type, int bitStream, int quality );
EXTERN_C int ext_change_net_img_quality( int chnn, VIDEO_ENCODE_MODE type, int bitStream, int quality );

//设置图像画质和帧率
EXTERN_C int ext_change_local_frame_quality( int chnn, int frm_nCount, VIDEO_ENCODE_MODE type, int bitStream, int quality );
EXTERN_C int ext_change_net_frame_quality( int chnn, int frm_nCount, VIDEO_ENCODE_MODE type, int bitStream, int quality );

//设置关键桢
EXTERN_C int ext_set_local_keyframe( int chnn );
EXTERN_C int ext_set_net_keyframe( int chnn );

//设置声音现场通道
EXTERN_C int ext_set_audio_live_channel( int bOpen, int iChl );
EXTERN_C int ext_set_audio_output_volume( int volume );
EXTERN_C int ext_set_audio_input_volume( int volume );


//设置OSD和水印参数
EXTERN_C int ext_rec_osd_print( int ch, OSD_TYPE type, OSD_ATTRIB *pOSDAttrib);
EXTERN_C int ext_rec_osd_clean( int ch, OSD_TYPE type);
EXTERN_C int ext_rec_water_mask( int chnn, OSD_ATTRIB *pAttrib);
EXTERN_C int ext_rec_water_mark_set( int chnn, unsigned char* key, int key_len, 
							unsigned char* user_info, int user_len );
EXTERN_C int ext_rec_water_mask_clean( int chnn);

//设置抓拍的时间
EXTERN_C int ext_set_snap_image_time( int sec_tm );
//使能抓拍的通道号
EXTERN_C int ext_set_snap_channel_enable( int channel, int frm, int frm_tm,  int video_size );

//获取回放通道的图像
EXTERN_C int ext_capture_pb_image( int channel, int img_qty,
	unsigned char* p_img_buf, int buf_len, int* jpeg_len );


//设置放大缩小图像功能
EXTERN_C int ext_vidoe_image_zoom( int b_zoom, int x, int y, int w, int h );

//
EXTERN_C int ext_get_device_cap(void);

//更改系统时钟后的回调函数	
EXTERN_C int  ext_chagne_system_time_flag(void);	

EXTERN_C int ext_err_get_last_value(void);
EXTERN_C int  ext_get_decode_err_flag(void);

//加密检查
EXTERN_C int ext_crypto_check( void);
EXTERN_C int ext_crypto_check_ex( ext_dvr_product_info* pst_product_info );
EXTERN_C int ext_crypto_write_data( unsigned char* data, int len );
EXTERN_C int ext_crypto_read_data( unsigned char* data, int len );

//设置VGA的参数
EXTERN_C int ext_set_vga_fmt_size( int videoFormat, int size );
EXTERN_C int ext_change_vga_formate( int videoFormat, int size );

//更改RTC时间
EXTERN_C int ext_rtc_time_system( ext_dvr_rtc_time* pstTime, int bSet);
//更改启动时的制式
EXTERN_C int ext_change_boot_video_standard( int videoFormat );

//获取鼠标状态
EXTERN_C int ext_mouse_get_status( KEY_VALUE_INFO* p_key_val );
EXTERN_C int ext_mouse_set_pos( int x, int y );

//获取键盘，遥控器等按键数值
EXTERN_C int ext_keyboard_value( KEY_VALUE_INFO* p_key_val );
//set led light 
EXTERN_C int ext_led_set(unsigned int uiLed, unsigned int on_off);
//get extern alarm input status
EXTERN_C int ext_alarm_status_get(unsigned long long* data);
//set one channel alarm output
EXTERN_C int ext_alarm_output_set(unsigned int uiAlarmChannel, unsigned int uiAlarmOnOff );
//set watchdog 1: open 0: close
EXTERN_C int ext_wdg_set(unsigned int uiFlag);
//set buzzer 1: open 0: close
EXTERN_C int ext_buzzer_set(unsigned int uiFlag);
//set lcd screen on off
EXTERN_C int ext_lcd_screen_onoff( unsigned int ui_onoff );


//设置和获取FLASH 中的参数
EXTERN_C int ext_get_flash_param( int param_type, char* param_value, int param_len );
EXTERN_C int ext_set_flash_param( int param_type, char* param_value, int param_len );

//更改启动时显示的画面
EXTERN_C int ext_set_boot_image( char* image_name );

//设置无菜单输出的内容
EXTERN_C int ext_set_output_nomenu_image( char* image_data, 
			int img_width, int img_height, int img_bits );


//获取双缓冲区屏幕信息
EXTERN_C int ext_get_fb_info( int fb_index, ext_dvr_fb_screen_info* p_fb_screen_info );
EXTERN_C int ext_set_fb_position( int fb_index, int xpos, int ypos );

EXTERN_C int ext_tde_begin_job(void);
EXTERN_C int ext_tde_end_job(void);
EXTERN_C int ext_tde_copy( ext_dvr_tde_op_param* p_tde_op_param );
EXTERN_C int ext_tde_quick_fill( ext_dvr_tde_op_param* p_tde_op_param );
EXTERN_C int ext_fb_copy(int phy_dest, int phy_src, int width,int height ,int cx,int cy,int bit_flag);
EXTERN_C int ext_fb_quick_fill( unsigned char* addr, int data,int cx, int cy,int bit_flag );

EXTERN_C int ext_app_buf_addr(unsigned char** addr, int *phy_address, int *buf_len );

// FLASH APP UPDATE
EXTERN_C int ext_update_appliction( unsigned char* app_name );

//设置码流扩展数据的长度
EXTERN_C int ext_set_bs_extern_data_len( int i_data_len );
EXTERN_C int ext_set_gprs_info( unsigned char* p_gprs_info, int len );

//增加I2C  设备的读写操作
EXTERN_C int ext_i2c_device_read_write( int i2c_type, int b_read, 
		int dev_id, int dev_reg, int* p_val, int cnt  );

// get decode type
EXTERN_C int ext_get_board_used_chip_type( 
		unsigned int* type0, unsigned int* type1, 
		unsigned int* type2, unsigned int* type3 );
EXTERN_C int ext_get_hd_chip_type( unsigned int * hd_type );
EXTERN_C int ext_get_vdec_left_frame_cnt( int chnn, int * frame_cnt );
EXTERN_C int ext_get_network_restart_flag( int * networt_restart );
EXTERN_C int ext_set_venc_profile( int venc_profile );

#if defined(__CHIP3520A__) || defined(__CHIP3520D__) ||defined(__CHIP3521__) || (defined(__CHIP3531__) && defined(__HISI_SDK_0BX__)) || defined(__CHIPGM__)
EXTERN_C int ext_get_video_type_info( ext_dvr_video_tpye_chg_info* p_video_tpye_chg_info, int*  chnnNum);
#endif

#if defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__HISI_SDK_0BX__) || defined(__CHIPGM__) || defined(__CHIP3520D__)
EXTERN_C int ext_get_video_Def_Color(char * bright, char * hue, char * sat, char * contrast);
#endif

//tvt test mutex 
EXTERN_C int ext_mutex_minit( void* mutex_obj, void* mutex_attr );
EXTERN_C int ext_mutex_mdestroy( void* mutex_obj );
EXTERN_C int ext_mutex_mlock( void* mutex_obj, char* file_name, int  lines );
EXTERN_C int ext_mutex_mtrylock( void* mutex_obj, char* file_name, int  lines );
EXTERN_C int ext_mutex_munlock( void* mutex_obj, char* file_name, int  lines );


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

EXTERN_C int ext_deccard_proc_msg( ext_deccard_fn_list* p_fn_list );
EXTERN_C unsigned char* ext_deccard_mem_map( int phy_addr, int size );
EXTERN_C int ext_deccard_mem_unmap( unsigned char* vir_addr, int size);


EXTERN_C int ext_change_display_ext_main(DISPLAY_ATTRIB *pDispAttrib_app, int chnnNum);
EXTERN_C int ext_change_display_ext_spot(DISPLAY_ATTRIB *pDispAttrib_app, int chnnNum);

EXTERN_C int ext_deccard_channel_enable( int chanl, int b_enable, unsigned long long  id_flag );

EXTERN_C int ext_deccard_get_output_device_size( int output_device_index, int* width, int * height );


#if defined(__CHIP3531__) || defined(__CHIP3521__)
EXTERN_C int ext_set_ptz_device_control( int chn, int* data, int len );

EXTERN_C int ext_get_ptz_device_control( int chn, int* data, int *p_len );
#endif


EXTERN_C int ext_get_sdk_param(int paramNam, ...);
EXTERN_C int ext_set_sdk_param(int paramNam, ...);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif //__DVR_FUNC_H__

