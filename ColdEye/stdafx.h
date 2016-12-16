 
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include <afxsock.h>            // MFC 套接字扩展

 
/*****************************************
 *             USER_MSG define           *
 *****************************************/
#define USER_MSG_TEST          (WM_USER + 100)


#define USER_MSG_SCAN_DEV      (WM_USER + 101)
#define USER_MSG_LOGIN         (WM_USER + 102)
#define USER_MSG_RELOGIN       (WM_USER + 103)
#define USER_MSG_LOGOFF        (WM_USER + 104)
#define USER_MSG_DISCONNECT    (WM_USER + 105)


#define USER_MSG_INITFILE	   (WM_USER + 113)
#define USER_MSG_ADDFILE	   (WM_USER + 114)
#define USER_MSG_DELFILE	   (WM_USER + 115)

#define USER_MSG_NOFITY_KEYDOWN   (WM_USER + 120)


#define USER_MSG_SYSTEM_CONFIG     (WM_USER + 121)


#define USER_MSG_CAMERA_CONFIG_CHANGE      (WM_USER + 122)

#define USER_MSG_CAMERA_CONFIG_AWTIME      (WM_USER + 123)

#define USER_MSG_CAMERA_CONFIG_NAME		   (WM_USER + 124)
#define USER_MSG_CAMERA_CONFIG_SWITCH      (WM_USER + 125)
#define USER_MSG_CAMERA_CONFIG_VOLUME	   (WM_USER + 126)

#define USER_MSG_CAMERA_CONFIG_SAVE		   (WM_USER + 127)
#define USER_MSG_CAMERA_CONFIG_AWSWITCH	   (WM_USER + 128)


#define USER_MSG_CAMERA_PARAM              (WM_USER + 130)

#define USER_MSG_MESSAGE_BOX               (WM_USER + 140)

#define USER_MSG_RECORDVOICE			   (WM_USER + 150)	//录制报警音
#define USER_MSG_STOP_RECORD			   (WM_USER + 151)	//停止录音

#define USER_MSG_COPY_STATR				   (WM_USER + 160)	//开始复制
#define USER_MSG_COPY_STOP				   (WM_USER + 161)	//停止复制
#define USER_MSG_COPY_INFO				   (WM_USER + 162)	//复制信息

#define USER_MSG_PLAY_START					(WM_USER + 170)	//视频播放
#define USER_MSG_PLAY_PLAY					(WM_USER + 171) //正常速度
#define USER_MSG_PLAY_SLOW					(WM_USER + 172)	//慢放
#define USER_MSG_PLAY_FAST					(WM_USER + 173)	//快进

#define USER_MSG_PORT_VISIBILITY_CHANGE    (WM_USER + 180)

#define USER_MSG_HANDLE                     (WM_USER + 190) //握手消息



#define USER_MSG_SYS_VOLUME					(WM_USER + 210) //系统音量
#define USER_MSG_SYS_LIGHT					(WM_USER + 211)	// 系统亮度

#define USER_MSG_ALARM_LIGHT				(WM_USER + 220) //报警灯光

#define USER_MSG_STAR_PLAY_ALARM_VOICE		(WM_USER + 230)	//开始播放报警音
#define USER_MSG_STOP_PLAY_ALARM_VOICE		(WM_USER + 231)	//停止播放报警音

#define USER_MSG_EXHARDDRIVE_OUT            (WM_USER + 240) //U盘拔出
#define USER_MSG_CANCEL_COPY				(WM_USER + 245) //取消复制
//  DUI user message
#define DUI_USER_MSGTYPE_BACK      (_T("back"))


#define USER_MSG_SURFACE_ZOOM              (WM_USER + 250)

/*****************************************
*             Copy Flie Result           *
******************************************/
#define COPY_END_SUCCEED   1
#define COPY_END_FAIL      0

 /*****************************************
 *             Timer ID define            *
 ******************************************/
#define TIMER_ID_SECOND_TICK              1
#define TIMER_ID_AUTO_RECORD              2
#define TIMER_ID_AUTO_WATCH               3
#define TIMER_ID_ALARM                    4
#define TIMER_ID_RECONNECT                5
#define TIMER_ID_HANDLE                   6


 /*****************************************
 *            Direction define            *
 ******************************************/
#define UI_RESOURCE_PATH           "..\\Resource"
#define RECORD_VOICE_PATH          "D:\\VOICE\\"
#define RECORD_VOICE_NAME_WAV	   "D:\\VOICE\\rwavalarm.wav"
#define RECORD_VOICE_NAME_WAV_TMP  "D:\\VOICE\\rwavalarmtmp.wav"
#define VOICE_NAME_WAV             "D:\\VOICE\\wavalarm.wav"
#define RECORD_VOICE_NAME	       "D:\\VOICE\\rG711"
#define RECORD_VOICE_NAME_TMP      "D:\\VOICE\\rG711tmp"
#define ALARM_VOICE_NAME	       "D:\\VOICE\\G711"
#define NORMAL_RECORD_PATH         "D:\\NormalRecord\\"
#define ALARM_RECORD_PATH          "E:\\AlarmRecord\\"
#define NORMALDISK                 "d:\\"
#define ALARMDISK                  "e:\\"

#define DEFAULT_VOICE              1
#define RECORD_VOICE               2
#define RECORD_VOICE_TMP           3

 /*****************************************
 *            File option macro           *
 ******************************************/
#define FILE_OPT_ADD     1
#define FILE_OPT_END     2
#define FILE_OPT_DEL     3


 /*****************************************
 *            Database macro define       *
 ******************************************/
#define DB_COL_OWNER            0
#define DB_COL_BEGIN_SEC        1
#define DB_COL_END_SEC          2
#define DB_COL_SIZE             3
#define DB_COL_STATUS           4


 /*****************************************
 *            LOG_TAG				      *
 ******************************************/
#define LOG_POWER_ONOFF   1       
#define LOG_SYS_TIME      2
#define LOG_AUTO_WATCH    3
#define LOG_CAM_ON		  4
#define LOG_CAM_OFF		  5
#define LOG_CAM_AWON	  6
#define LOG_CAM_AWOFF     7


 /*****************************************
 *         Time config macro define       *
 ******************************************/
#define RECONNECT_TIMER_ELAPSE   30 //短线后每隔30s重连一次。
#define RECORD_PACK_TIMER_ELAPSE 60 //录像文件自动打包时间60s
#define ALARM_TIMEOUT_CNT        30 //无移动帧后30s判定为报警结束
#define NORMAL_TIME              30 //正常文件保存时间


/**      Disk Surpluss Space     **/
const ULONGLONG SURPLUSSPACENORMAL = 2147483648;//正常文件所在磁盘剩余空间红线524288000
const ULONGLONG SURPLUSSPACEALARM = 2147483648;//报警文件所在磁盘剩余空间红线524088000

/**        Com num define        **/
#define COM_KB    1
#define COM_CAM   2

/**     COM_KB keyboard num     **/
#define KB_MENU                       1
#define KB_UP                         2
#define KB_BACK                       3
#define KB_AUTOWATCH                  4
#define KB_LEFT                       5
#define KB_ENTER                      6
#define KB_RIGHT                      7
#define KB_UDISK                      8
#define KB_VOLUP                      9
#define KB_DOWN                       10
#define KB_BRIUP                      11
#define KB_FUNC                       12
#define KB_VOLDOWN                    13
#define KB_TALKQUIET                  14
#define KB_BRIDOWN                    15
#define KB_SWITCH                     16
#define KB_PTTDOWN                    17
#define KB_PTTUP                      18

/**  NetSDK lib **/
#pragma comment(lib, "H264Play.lib")
#pragma comment(lib, "NetSdk.lib")


/**  sqlite3 lib **/
#ifdef _DEBUG
#pragma comment(lib, "sqlite3_d.lib")
#else
#pragma comment(lib, "sqlite3.lib")
#endif

/**  type of messagebox  **/
#define SHIPNAME_LIMIT		0	//船名已满
#define CLOSE_CAMERA		1	//关闭摄像头
#define CLOSE_STROAGE		2	//关闭视频存储
#define CLOSE_AUTOWATCH		3	//关闭自动看船
#define SAVE_CHANGES		4	//保存设置
#define UPDATE_REQUEST		5	//软件更新提示
#define NO_UPDATE_FILE		6	//无更新文件提示
#define NO_UPDATE_DRIVE		7	//未检测到U盘提示
#define	SOFT_UPDATING		8	//软件更新中
#define	SOFT_UPDATE_SUCCESS	9	//软件更新成功
#define FACTORY_RESET		10	//出厂设置
#define RECORD				11	//录制报警音
#define SAVE_RECORDED		12	//保存录制的报警音
#define DELETE_CAMERA		13	//删除摄像头
#define DISPLAY_MATCH		14  //显示器匹配提示
#define CANNOT_COPY			15	//无法复制视频
#define	NO_COPY_DRIVE		16	//未检测出U盘
#define NO_ENOUGH_MEMORY	17	//U盘内容空间不足
#define CONFIRM_COPY		18	//复制确认
#define COPYING_NORMAL		19	//复制正常录像
#define COPYING_ALARM		20	//复制报警录像
#define STOP_COPY			21	//停止复制
#define	COPY_SUCCESS		22	//复制成功
#define	COPY_FAILURE		23	//复制失败

/**  MenuIten base userdata  **/
#define ALARM_VIDEO			0
#define CAMERA_SET			7
#define VIDEO_OBTAIN		18

/**  Copy Status  **/
#define STATUS_COPY_SUCCEED 1
#define STATUS_COPY_FIAL	0

/**  HardDriver Status  **/
#define NO_HARDDRIVER		0
#define OK_HARDDRIVER		1
#define FULL_HARDDRIVER		2

//DUI 消息类型
#define DUI_MSGTYPE_KEYDOWN			(_T("keydown"))
#define DUI_MSGTYPE_COPYFILE		(_T("copyfile"))
#define DUI_MSGTYPE_SLIDER			(_T("slider"))
#define DUI_MSGTYPE_EDIT			(_T("edit"))
#define DUI_MSGTYPE_LABEL			(_T("label"))
#define DUI_MSGTYPE_ALA_VOICE_LIST	(_T("alarm_voice"))
#define DUI_MSGTYPE_SWITCH			(_T("switch"))
#define DUI_MSGTYPE_PLAYER			(_T("player"))
#define DUI_MSGTYPE_LISTLABEL		(_T("listlabel"))
#define DUI_MSGTYPE_RECORDVOICE		(_T("recordvoice"))

//报警音播放类型
#define ALARM_VOICE_DEFAULT         0
#define ALARM_VOICE_RECORD          1

//系统音量设置
#define HOST_VOICE_LEVEL_0          0
#define HOST_VOICE_LEVEL_1          1
#define HOST_VOICE_LEVEL_2          2
#define HOST_VOICE_LEVEL_3          3
#define HOST_VOICE_LEVEL_4          4
#define HOST_VOICE_LEVEL_5          5
#define HOST_VOICE_LEVEL_6          6
#define HOST_VOICE_LEVEL_7          7
#define HOST_VOICE_LEVEL_8          8
#define HOST_VOICE_LEVEL_9          9
#define HOST_VOICE_LEVEL_10         10
#define HOST_VOICE_MUTE             11
#define HOST_VOICE_NOMUTE           12
#define HOST_VOICE_LEVEL_UP         13
#define HOST_VOICE_LEVEL_DOWN       14

#include "Database\sqlitewrapper.h"
extern int mutex;

extern 	SQLiteWrapper sqlite;


#define USE_PRINT

#ifdef USE_PRINT
#define Print(format,...)  printf(format"      %s\n",__VA_ARGS__,__FUNCTION__)
#define PrintE(format,...) printf(format"      %s\n",__VA_ARGS__,__FUNCTION__)
#define PrintW(format,...) printf(format"      %s\n",__VA_ARGS__,__FUNCTION__)
#else
#define Print(format,...)
#endif



#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib, "DuiLib_d.lib")
#else
#pragma comment(lib, "DuiLib.lib")
#endif

#pragma comment(lib, "WinMM.Lib")
#pragma comment(lib,"Shlwapi.lib")

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

void ShowMemoryInfo();
