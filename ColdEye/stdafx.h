
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <afxsock.h>            // MFC �׽�����չ




/*****************************************
 *             USER_MSG define           *
 *****************************************/
#define USER_MSG_TEST          (WM_USER + 100)


#define USER_MSG_SCAN_DEV      (WM_USER + 110)
#define USER_MSG_LOGIN         (WM_USER + 111)
#define USER_MSG_RELOGIN       (WM_USER + 112)




 /*****************************************
 *            Direction define            *
 ******************************************/
#define UI_RESOURCE_PATH    "..\\Resource"
#define RECORD_PATH          "D:\\Record\\"
#define NORMAL_RECORD_PATH   "D:\\Record\\Normal\\"
#define ALARM_RECORD_PATH    "D:\\Record\\Alarm\\"
#define SOUND_PATH           "D:\\Record\\Alarm\\"



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



#define ALARM_TIMEOUT_CNT    30

/**  NetSDK lib **/
#pragma comment(lib, "H264Play.lib")
#pragma comment(lib, "NetSdk.lib")

/**  sqlite3 lib **/
#pragma comment(lib, "sqlite3.lib")



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


