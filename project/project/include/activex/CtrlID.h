#ifndef _CTRL_ID_H_
#define _CTRL_ID_H_

namespace GUI
{
	enum CTRLID
	{
		CTRLID_NULL = 0x00,
		//GUI�ڲ�ʹ��
		CTRLID_DOMODAL_DLG,
		CTRLID_STATIC, //���о�̬�ؼ�����ʹ�ô�ID
		CTRLID_PICTURE,
		CTRLID_DEFAULT_BTN_OK,
		CTRLID_DEFAULT_BTN_CANCEL,
		CTRLID_DEFAULT_BTN_APPLY,
		CTRLID_SCROLL_BTN_FIRST,
		CTRLID_SCROLL_BTN_SECOND,
		CTRLID_SCROLL_SLIDER,

		CTRLID_TAB_BTN_FIRST = 50,
		CTRLID_TAB_BTN_END = 86,

		CTRLID_COMBO_EDIT,
		CTRLID_COMBO_BTN_LEFT,
		CTRLID_COMBO_BTN_RIGHT,	

		CTRLID_LISTCTRL_HEDER,
		CTRLID_LISTCTRL_BOX,
		CTRLID_LIST_CTRL_SCROLL_V,
		CTRLID_LIST_EX_BTN_FIRST,
		CTRLID_LIST_EX_BTN_PREV,
		CTRLID_LIST_EX_BTN_NEXT,
		CTRLID_LIST_EX_BTN_END,
		CTRLID_LIST_EX_CK_ALL,
		CTRLID_LIST_EX_CK_SELECT_0 = 100,
		CTRLID_LIST_EX_CK_SELECT_31 = 131,
		
		CTRLID_TIME_SECTION_FIRST = 132,
		CTRLID_TIME_SECTION_LAST = 160,

		CTRLID_IPCTRL_FIELD1,
		CTRLID_IPCTRL_FIELD2,
		CTRLID_IPCTRL_FIELD3,
		CTRLID_IPCTRL_FIELD4,
		//
		CTRLID_DLG_FLEX_BTN,
		CTRLID_CALENDAR_COMBO_YEAR,
		CTRLID_CALENDAR_COMBO_MONTH,

		CTRLID_SCHEDULE_FIRST,
		CTRLID_SCHEDULE_PREV,
		CTRLID_SCHEDULE_NEXT,
		CTRLID_SCHEDULE_LAST,

		//����
		CTRLID_KEYBORAD_EDIT_INPUT,
		CTRLID_KEYBORAD_BTN_BACK,
		CTRLID_KEYBORAD_BTN_SHIFT,
		//CTRLID_KEYBORAD_BTN_OK,
		CTRLID_KEYBORAD_BTN_BEGIN = 200,
		CTRLID_KEYBORAD_BTN_END = 299,

		CTRLID_TOOL_BAR_DLG	= 300,
		CTRLID_TOOL_BAR_6X6,
		CTRLID_TOOL_BAR_5X5,
		CTRLID_TOOL_BAR_4X4,
		CTRLID_TOOL_BAR_1A12,
		CTRLID_TOOL_BAR_1A7,
		CTRLID_TOOL_BAR_3X3,
		CTRLID_TOOL_BAR_1A5,
		CTRLID_TOOL_BAR_2X3,
		CTRLID_TOOL_BAR_2X2,
		CTRLID_TOOL_BAR_1X1,

		CTRLID_TOOL_BAR_6X6_EX,
		CTRLID_TOOL_BAR_5X5_EX,
		CTRLID_TOOL_BAR_4X4_EX,
		CTRLID_TOOL_BAR_1A12_EX,
		CTRLID_TOOL_BAR_1A7_EX,
		CTRLID_TOOL_BAR_3X3_EX,
		CTRLID_TOOL_BAR_1A5_EX,
		CTRLID_TOOL_BAR_2X3_EX,
		CTRLID_TOOL_BAR_2X2_EX,
		CTRLID_TOOL_BAR_1X1_EX,

		CTRLID_TOOL_BAR_SYSTEM,
		CTRLID_TOOL_BAR_SEARCH,
		CTRLID_TOOL_BAR_BACKUP,
		CTRLID_TOOL_BAR_INFO,
		CTRLID_TOOL_BAR_DWELL,
		CTRLID_TOOL_BAR_ZOOM,
		CTRLID_TOOL_BAR_AUDIO,
		CTRLID_TOOL_BAR_PTZ,
		CTRLID_TOOL_BAR_REC,
		CTRLID_TOOL_BAR_PLAY,
		CTRLID_TOOL_BAR_ARM,
		CTRLID_TOOL_BAR_SHUTDOWN,
		CTRLID_TOOL_BAR_STATUS,
		CTRLID_TOOL_BAR_CHANGE_COLOR,
		CTRLID_TOOL_BAR_SNAP,
		CTRLID_TOOL_BAR_PLAY_EX,
		CTRLID_TOOL_BAR_PIP,
		CTRLID_TOOL_BAR_BURN,
		CTRLID_TOOL_BAR_BURN_EX,
		CTRLID_TOOL_BAR_SYSINFO,
		CTRLID_TOOL_BAR_CAM_PARA,

		CTRLID_PLAYER_PLAY_PAUSE = 360,
		CTRLID_PLAYER_STOP,
		CTRLID_PLAYER_NEXTFRAME,
		CTRLID_PLAYER_FASTFORWARD,
		CTRLID_PLAYER_FFEX,
		CTRLID_PLAYER_REWARD,
		CTRLID_PLAYER_REWEX,
		CTRLID_PLAYER_ONESPLIT,
		CTRLID_PLAYER_1X1EX,
		CTRLID_PLAYER_FOURSPLIT,

		CTRLID_PLAYER_2X2EX,
		CTRLID_PLAYER_SIXSPLIT,
		CTRLID_PLAYER_2X3EX,
		CTRLID_PLAYER_NINESPLIT,
		CTRLID_PLAYER_3X3EX,
		CTRLID_PLAYER_SIXTEENSPLIT,
		CTRLID_PLAYER_4X4EX,
		CTRLID_PLAYER_SPECSPLIT,
		CTRLID_PLAYER_LIVE,
		CTRLID_PLAYER_ZOOM,

		CTRLID_PLAYER_COLOR,
		CTRLID_PLAYER_BACKUP_START_TIME,
		CTRLID_PLAYER_BACKUP_END_TIME,
		CTRLID_PLAYER_BACKUP_SELECT_TIME,
		CTRLID_PLAYER_SNAP,
		CTRLID_PLAYER_BACKUP,
		CTRLID_PLAYER_MOVE,
		CTRLID_PLAYER_EXIT,
		CTRLID_PLAYER_HIDE,
		CTRLID_PLAYER_AUDIO,
		
		CTRLID_PLAYER_PRE_SECTION,
		CTRLID_PLAYER_NEXT_SECTION,
		CTRLID_PLAYER_CAM_PARA,
		
		CTRLID_SEARCH_MAN_DLG = 420,
		CTRLID_SEARCH_TIME_DLG,
		CTRLID_SEARCH_EVENT_DLG,
		CTRLID_SEARCH_FILE_DLG,
		CTRLID_SEARCH_EVENT_LIST,
		CTRLID_SEARCH_FILE_LIST,
		CTRLID_SEARCH_TABLE,
		CTRLID_SEARCH_CK_MOTION,
		CTRLID_SEARCH_CK_SENSOR,
		CTRLID_SEARCH_BTN_SEARCH,
		CTRLID_SEARCH_BTN_PLAY,
		CTRLID_SEARCH_BTN_LOCK,
		CTRLID_SEARCH_BTN_DELETE,
		CTRLID_SEARCH_BTN_1X1,
		CTRLID_SEARCH_BTN_2X2,
		CTRLID_SEARCH_BTN_3X3,
		CTRLID_SEARCH_BTN_4X4,
		CTRLID_SEARCH_BTN_5X5,
		CTRLID_SEARCH_BTN_6X6,
		CTRLID_SEARCH_BTN_7X7,
		CTRLID_SEARCH_BTN_8X8,
		CTRLID_SEARCH_TIME_CH_0		= 450,
		CTRLID_SEARCH_TIME_CH_31	= 450 + 31,

		//
		CTRLID_SEARCH_CONDITION_DLG,
		CTRLID_SEARCH_CALENDAR,
		CTRLID_SEARCH_CH_0			= 500,
		CTRLID_SEARCH_CH_31			= 500 + 31,
		CTRLID_SEARCH_GROUP_0	= 540,
		CTRLID_SEARCH_GROUP_7 = 540 + 7,

		CTRLID_INFO_MAN_DLG			= 600,
		CTRLID_INFO_SYSTEM_DLG,
		CTRLID_INFO_VERSION_DLG,
		CTRLID_INFO_STREAM_DLG,
		CTRLID_INFO_EVENT_DLG,
		CTRLID_INFO_LOG_DLG,
		CTRLID_INFO_NETWORK_DLG,
		CTRLID_INFO_NETWORK_SETUP_DLG,
		CTRLID_INFO_NETWORK_STREAM_DLG,
		CTRLID_INFO_NETWORK_DEVICE_DLG,
		CTRLID_INFO_ONLINE_USER_DLG,
		CTRLID_INFO_IVS_DLG,
		CTRLID_INFO_VERSION_LIST,
		CTRLID_INFO_STREAM_LIST,
		CTRLID_INFO_EVENT_LIST,
		CTRLID_INFO_LOG_LIST,
		CTRLID_INFO_ONLINE_USER_LIST,
		CTRLID_INFO_EVENT_BTN,
		CTRLID_INFO_START_TIME,
		CTRLID_INFO_END_TIME,
		CTRLID_INFO_BTN_SYSTEM,
		CTRLID_INFO_BTN_EVENT,
		CTRLID_INFO_BTN_LOG,
		CTRLID_INFO_BTN_NETWORK,
		CTRLID_INFO_BTN_ONLINE_USER,
		CTRLID_INFO_BTN_DEVICE,
		CTRLID_INFO_BTN_EXIT,
		CTRLID_INFO_BTN_IVS,
		CTRLID_INFO_MOTION,
		CTRLID_INFO_SENSOR,
		CTRLID_INFO_VIDEO_LOSS,
		CTRLID_INFO_BTN_SEARCH,
		CTRLID_INFO_CH_0 = 650,
		CTRLID_INFO_CH_31= 681,

		CTRLID_SEARCH_CDN2_DLG = 700,
		CTRLID_SEARCH_CDN2_GROUP_0 = 710,
		CTRLID_SEARCH_CDN2_GROUP_11 = 710 + 11,
		CTRLID_SEARCH_CDN2_CH_0 = 730,
		CTRLID_SEARCH_CDN2_CH_47 = 730 + 47,

		//�ⲿ�Ի���ʹ��
		CTRLID_MAINMENU_CONFIG		= 800,
		CTRLID_MAINMENU_SEARCH,
		CTRLID_MAINMENU_BACKUP,
		CTRLID_MAINMENU_INFO,
		CTRLID_MAINMENU_MANUAL_ALARM,
		CTRLID_MAINMENU_DISK_MAN,
		CTRLID_MAINMENU_SYSTEM_RESOURCES,
		CTRLID_MAINMENU_DEVICE_MAN,
		CTRLID_MAINMENU_LOGOFF,
		CTRLID_MAINMENU_SHUTDOWN,

		CTRLID_CFG_SYSTEM_MAN_DLG,
		CTRLID_CFG_SYSTEM_MAN_BASIC_BTN,
		CTRLID_CFG_SYSTEM_MAN_LIVE_BTN,
		CTRLID_CFG_SYSTEM_MAN_RECORD_BTN,
		CTRLID_CFG_SYSTEM_MAN_SCHEDULE_BTN,
		CTRLID_CFG_SYSTEM_MAN_ALARM_BTN,
		CTRLID_CFG_SYSTEM_MAN_NETWORK_BTN,
		CTRLID_CFG_SYSTEM_MAN_USERS_BTN,
		CTRLID_CFG_SYSTEM_MAN_PTZ_BTN,
		CTRLID_CFG_SYSTEM_MAN_ADVANCE_BTN,
		CTRLID_CFG_SYSTEM_MAN_GB28181_BTN,
		CTRLID_CFG_BASIC_DLG,
		CTRLID_CFG_LIVE_DLG,
		CTRLID_CFG_RECORD_DLG,
		CTRLID_CFG_SCHEDULE_DLG,
		CTRLID_CFG_ALARM_DLG,
		CTRLID_CFG_NETWORK_DLG,
		CTRLID_CFG_USERS_DLG,
		CTRLID_CFG_PTZ_DLG,
		CTRLID_CFG_ADVANCE_DLG,

		//Alarm
		CTRLID_CFG_ALARM_TABLE,
		CTRLID_CFG_ALARMOUT_DLG,
		CTRLID_CFG_MOTION_DLG,
		CTRLID_CFG_OTHERALARM_DLG,
		CTRLID_CFG_SENSOR_DLG,
		CTRLID_CFG_VIDEOLOSS_DLG,
		////Alarm Sensor
		CTRLID_CFG_SENSOR_LIST,

		CTRLID_SELECT_CHNN_CHECK_BEGIN		=1000,
		CTRLID_SELECT_CHNN_CHECK_END		=1040,

		///PTZ control
		CTRLID_PTZCONTROL_UP				=1100,
		CTRLID_PTZCONTROL_DOWN,
		CTRLID_PTZCONTROL_LEFT,
		CTRLID_PTZCONTROL_RIGHT,
		CTRLID_PTZCONTROL_UPLEFT,
		CTRLID_PTZCONTROL_UPRIGHT,
		CTRLID_PTZCONTROL_DOWNLEFT,
		CTRLID_PTZCONTROL_DOWNRIGHT,
		CTRLID_PTZCONTROL_ZOOMIN,
		CTRLID_PTZCONTROL_ZOOMOUT,
		CTRLID_PTZCONTROL_FOCUSIN,
		CTRLID_PTZCONTROL_FOCUSOUT,
		CTRLID_PTZCONTROL_IRISIN,
		CTRLID_PTZCONTROL_IRISOUT,
		CTRLID_PTZCONTROL_STOP,
		CTRLID_PTZCONTROL_BRUSH,
		CRTLID_PTZCONTROL_LAMP,
		CTRLID_CHANNEL_LIVE0			   = 1150,
		CTRLID_CHANNEL_LIVE31			   = 1181,					
		CTRLID_CHANNEL_LIVE_ALL,


		////TestDlg1
		CTRLID_TEST_DLG1 = 10000,
		CTRLID_TEST1_BTN1,
		CTRLID_TEST1_BTN2,
		CTRLID_TEST1_BTN3,
		CTRLID_TEST1_BTN4,
		CTRLID_TEST1_BTN5,
		CTRLID_TEST1_BTN6,
		CTRLID_TEST1_BTN_LIST,
		CTRLID_TEST1_CHECK1,
		CTRLID_TEST1_RADIO1,
		CTRLID_TEST1_RADIO2,
		CTRLID_TEST1_RADIO3,
		////TestDlg2
		CTRLID_TEST_DLG2,
		CTRLID_TEST2_TAB1,
		CTRLID_TEST_TABDLG1,
		CTRLID_TEST_TABDLG2,
		CTRLID_TEST_TABDLG3,
		CTRLID_TEST2_TIMECTRL,
		////TestDlg3
		CTRLID_TEST_DLG3,
		CTRLID_TEST3_BTN1,
		CTRLID_TEST3_BTN2,
		CTRLID_TEST3_BTN3,
		CTRLID_TEST3_BTN4,
		CTRLID_TEST3_TIMECTRL,
		////CTestOwnerDrawDlg
		CTRLID_TESTOWNERDRAWDLG,
		CTRLID_TESTOWNERDRAWDLG_BTN1,
		////CTestDefaultBtn
		CTRLID_TESTDEFAULTBTN_BTN1,
		////CTestTabDlg2
		CTRLID_TESTTABDLG2_EDIT_INFO,
		CTRLID_TESTTABDLG2_EDIT,
		CTRLID_TESTTABDLG2_EDIT_NUM,
		CTRLID_TESTTABDLG2_COMBO,
		CTRLID_TESTTABDLG2_COMBO2,
		CTRLID_TESTTABDLG2_PROGRESS,
		CTRLID_TESTTABDLG2_BTNTEST_PROGRESS,
		CTRLID_TESTTABDLG2_TIME,
		CTRLID_TESTTABDLG2_CHANGE_TIME_ORDER,
		CTRLID_TESTTABDLG2_TIME_GO,
		CTRLID_TESTTABDLG2_IP,
		////CTestTabDlg3
		CTRLID_CTESTTABDLG2_SLIDER,
		CTRLID_CTESTTABDLG2_SLIDER_V,
		CTRLID_CTESTTABDLG2_EDIT,
		CTRLID_CTESTTABDLG2_BTN1,
		CTRLID_CTESTTABDLG2_BTN2,
		////CTestScrollDlg
		CTRLID_CTESTSCROLLDLG_EDIT,
		CTRLID_CTESTSCROLLDLG_SCROLL,
		CTRLID_CTESTSCROLLDLG_SCROLL_V,
		////CTestListDlg
		CTRLID_CTESTLISTDLG_LISTCTRL,
		////CTestDataViewDlg
		CTRLID_CTESTDATAVIEWDLG_DATAVIEW,
		CTRLID_CTESTDATAVIEWDLG_RULER,

		//�������ID��ʼλ��
		RAND_CTRLID,
	};
}

#endif