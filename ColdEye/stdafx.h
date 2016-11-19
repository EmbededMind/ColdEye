
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

#define USER_MSG_INITFILE	   (WM_USER + 113)
#define USER_MSG_ADDFILE	   (WM_USER + 114)
#define USER_MSG_DELFILE	   (WM_USER + 115)

#define USER_MSG_NOFITY_KEYDOWN   (WM_USER + 120)


#define USER_MSG_SYSTEM_STATUS_CHANGE      (WM_USER + 121)
#define USER_MSG_CAMERA_CONFIG_OO_CHANGE   (WM_USER + 122)  //摄像机开关设置改变
#define USER_MSG_CAMERA_CONFIG_AW_CHANGE   (WM_USER + 123)  //自动看船设置改变
#define USER_MSG_CAMERA_CONFIG_RD_CHANGE   (WM_USER + 124)  //录像设置改变

#define USER_MSG_CAMERA_PARAM              (WM_USER + 130)

#define USER_MSG_MESSAGE_BOX               (WM_USER + 140)



 /*****************************************
 *             Timer ID define            *
 ******************************************/
#define TIMER_ID_AUTO_RECORD              2
#define TIMER_ID_AUTO_WATCH               3
#define TIMER_ID_ALARM                    4
#define TIMER_ID_RECONNECT                5


 /*****************************************
 *            Direction define            *
 ******************************************/
#define UI_RESOURCE_PATH    "..\\Resource"
#define RECORD_PATH          "D:\\Record\\"
#define NORMAL_RECORD_PATH   "D:\\Record\\Normal\\"
#define ALARM_RECORD_PATH    "D:\\Record\\Alarm\\"
#define SOUND_PATH           "D:\\Record\\Alarm\\"
#define NORMALDISK _T("d")
#define ALARMDISK  _T("e")


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
 *         Time config macro define       *
 ******************************************/
#define RECONNECT_TIMER_ELAPSE   30 //短线后每隔30s重连一次。
#define RECORD_PACK_TIMER_ELAPSE 60 //录像文件自动打包时间60s
#define ALARM_TIMEOUT_CNT        30 //无移动帧后30s判定为报警结束
#define NORMAL_TIME              30 //正常文件保存时间


/**      Disk Surpluss Space     **/
#define SURPLUSSPACENORMAL 100     //正常文件所在磁盘剩余空间红线
#define SURPLUSSPACEALARM  100     //报警文件所在磁盘剩余空间红线

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
#pragma comment(lib, "sqlite3.lib")


/**  Set Volume  **/
#define ZERO_VOLUME     0
#define NORMAL_VOLUME   1
#define ENLARGE_VOLUME  2
#define REDUCE_VOLUME   3


extern int mutex;


#define USE_PRINT

#ifdef USE_PRINT
#define Print(format,...)  printf(format"\n",__VA_ARGS__)
#define PrintE(format,...) printf(format"\n",__VA_ARGS__)
#define PrintW(format,...) printf(format"\n",__VA_ARGS__)
#else
#define Print(format,...)
#endif



#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib, "DuiLib_d.lib")
#else
#pragma comment(lib, "DuiLib.lib")
#endif


#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


