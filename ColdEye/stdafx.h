
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


#define USER_MSG_SCAN_DEV      (WM_USER + 101)
#define USER_MSG_LOGIN         (WM_USER + 102)
#define USER_MSG_RELOGIN       (WM_USER + 103)
#define USER_MSG_LOGOFF        (WM_USER + 104)

#define USER_MSG_INITFILE	   (WM_USER + 113)
#define USER_MSG_ADDFILE	   (WM_USER + 114)
#define USER_MSG_DELFILE	   (WM_USER + 115)

#define USER_MSG_NOFITY_KEYDOWN   (WM_USER + 120)


#define USER_MSG_SYSTEM_STATUS_CHANGE      (WM_USER + 121)
#define USER_MSG_CAMERA_CONFIG_OO_CHANGE   (WM_USER + 122)  //������������øı�
#define USER_MSG_CAMERA_CONFIG_AW_CHANGE   (WM_USER + 123)  //�Զ��������øı�
#define USER_MSG_CAMERA_CONFIG_RD_CHANGE   (WM_USER + 124)  //¼�����øı�

#define USER_MSG_CAMERA_PARAM              (WM_USER + 130)

#define USER_MSG_MESSAGE_BOX               (WM_USER + 140)

#define USER_MSG_RECORDVOICE			   (WM_USER + 150)	//¼�Ʊ�����
#define USER_MSG_STOP_RECORD			   (WM_USER + 151)	//ֹͣ¼��

#define USER_MSG_COPY_STATR				   (WM_USER + 160)	//��ʼ����
#define USER_MSG_COPY_STOP				   (WM_USER + 161)	//ֹͣ����
#define USER_MSG_COPY_INFO				   (WM_USER + 162)	//������Ϣ



#define USER_MSG_PORT_VISIBILITY_CHANGE    (WM_USER + 150)



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
#define RECONNECT_TIMER_ELAPSE   30 //���ߺ�ÿ��30s����һ�Ρ�
#define RECORD_PACK_TIMER_ELAPSE 60 //¼���ļ��Զ����ʱ��60s
#define ALARM_TIMEOUT_CNT        30 //���ƶ�֡��30s�ж�Ϊ��������
#define NORMAL_TIME              30 //�����ļ�����ʱ��


/**      Disk Surpluss Space     **/
#define SURPLUSSPACENORMAL 100     //�����ļ����ڴ���ʣ��ռ����
#define SURPLUSSPACEALARM  100     //�����ļ����ڴ���ʣ��ռ����

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


/**  Set Volume  **/
#define ZERO_VOLUME     0
#define NORMAL_VOLUME   1
#define ENLARGE_VOLUME  2
#define REDUCE_VOLUME   3

/**  type of messagebox  **/
#define SHIPNAME_LIMIT		0	//��������
#define CLOSE_CAMERA		1	//�ر�����ͷ
#define CLOSE_STROAGE		2	//�ر���Ƶ�洢
#define CLOSE_AUTOWATCH		3	//�ر��Զ�����
#define SAVE_CHANGES		4	//��������
#define UPDATE_REQUEST		5	//���������ʾ
#define NO_UPDATE_FILE		6	//�޸����ļ���ʾ
#define NO_UPDATE_DRIVE		7	//δ��⵽U����ʾ
#define	SOFT_UPDATING		8	//���������
#define	SOFT_UPDATE_SUCCESS	9	//������³ɹ�
#define FACTORY_RESET		10	//��������
#define RECORD				11	//¼�Ʊ�����
#define SAVE_RECORDED		12	//����¼�Ƶı�����
#define DELETE_CAMERA		13	//ɾ������ͷ
#define DISPLAY_MATCH		14  //��ʾ��ƥ����ʾ
#define CANNOT_COPY			15	//�޷�������Ƶ
#define	NO_COPY_DRIVE		16	//δ����U��
#define NO_ENOUGH_MEMORY	17	//U�����ݿռ䲻��
#define CONFIRM_COPY		18	//����ȷ��
#define COPYING				19	//������
#define STOP_COPY			20	//ֹͣ����
#define	COPY_SUCCESS		21	//���Ƴɹ�
#define	COPY_FAILURE		22	//����ʧ��

/**  MenuIten base userdata  **/
#define ALARM_VIDEO			0
#define CAMERA_SET			7
#define VIDEO_OBTAIN		18




#include "Database\sqlitewrapper.h"
extern int mutex;

extern 	SQLiteWrapper sqlite;


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


