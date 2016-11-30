#pragma once

#include "UIlib.h"
#include "File\RecordFileInfo.h"

#include "Control\MyEditUI.h"
#include "Control\MySliderUI.h"
#include "Control\MyLabelUI.h"
#include "Control\MenuItemUI.h"
#include "Control\VideoListUI.h"
#include "Control\SwitchUI.h"
#include "Control\AlarmVoiceListUI.h"
#include "Control\AWOnOffListLabelUI.h"
#include "Control\TimeButtonUI.h"

#include "Wnd\PlayerWallWnd.h"


#include "Device\PortManager.h"
#include "ExHardDrive\ExHardDrive.h"


#include <list>

//==============BkColor=====================
#define LAYOUT_POP_FOCUSED		0xFFFFFFFF
#define	LAYOUT_POP_NOFOCUS		0xFFE6E6EF

#define LAYOUT_MENUITEM_FOCUSED		 0xFFFFFFFF
#define LAYOUT_MENUITEM_NOFOCUS		 0xFFE6E6EF
#define LAYOUT_MNEUITEM_SEL_NOFOCUS	 0xFFEFEFF4

#define LAYOUT_THIRD_NOFOUCS		 0xFFEFEFF4
#define LAYOUT_THIRD_FOCUSED		 0xFFFFFFFF

#define ITEM_FOCUS					 0xFF4198FE
#define POPMENU_SEL_NOFOCUS			 0xFF47688F
#define MENUITEM_SEL_NOFOCUS		 0xFF4178B7

using namespace std;


using namespace DuiLib;

typedef struct {
	UINT8 id;				   //����ͷID
	bool IsActivate;           //������Ƿ񿪻�
	bool IsVideoRecordEnabled; //��Ƶ�洢����
	bool IsAutoWatchEnabled;   //�Զ���������
	VIDEO_DIRECTION VideoDir;  //��Ƶ����
	UCHAR Volumn;              //����
	CString Name;				   //���������
}CameraInfo;


typedef struct {
	CTime time;
	CDuiString record_type;
} SwtichRecord;


class CMyMenuWnd :
	public WindowImplBase
{
public:

	struct CameraItem {
		UINT8 id;	//����ӿ�
		CLabelUI *pTitle;	//����
		CMyEditUI *pShipname;	//����ͷ����
		CSwitchUI *pSwitch;	//����
		CMySliderUI *pVolum;	//����
		CMyLabelUI *pSaveVideo;	//��Ƶ��������
		CMyLabelUI *pAutoWatch;	//�Զ���������
		CVideoListUI *pNormalList; //��Ƶ�б�
		CVideoListUI *pAlarmList;	//������Ƶ�б�
	};

public :
	CMyMenuWnd();
	~CMyMenuWnd();

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	void InitWindow();

	// BkColor
	void UpdataItemColor();

	// Item Notify
	void SliderNotify(TNotifyUI& msg);
	void EditNotify(TNotifyUI& msg);
	void MenuItemNotify(TNotifyUI& msg);
	void LabelNotify(TNotifyUI& msg);
	void CopyFileNotify(TNotifyUI& msg);
	void RecordVoiceNotify(TNotifyUI& msg);
	void AlarmVoiceListNotify(TNotifyUI& msg);
	void SwitchNotify(TNotifyUI& msg);
	void ListLabelNotify(TNotifyUI& msg);

	void ExpandCameraName();
	void ShowAlarmVoiceList(bool);
	void ThirdMenuSetFocus(CDuiString);
	int DetectHardDriver(list<CRecordFileInfo*>*);
	void PrepareCopy(list<CRecordFileInfo*>*,UINT8);
	void PlayVideo(WPARAM wParam, LPARAM lParam);
	void RecordVoice();
	void AddAlarmVoice();
	void refreshSuperscript(CMyListUI*); //ˢ�½Ǳ�
	void Notify(TNotifyUI& msg);

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetAllVirginNum();
	void DeleteAlarmCtl(CameraInfo);
	void DeleteCameraSetCtl(CameraInfo);
	void DeleteVideoObtain(CameraInfo);

	//�Զ��������ؼ�¼
	void InitAwOnOffRecord();
	CDuiString GetStringOption(int option,int tag);
	void AddAwOnOffRecord(CTime, CDuiString);
	void AwPage(int page);
	void AwOnOffRecordNextPage();
	void AwOnOffRecordLastPage();

	int InsertAt(UINT8 id, CVerticalLayoutUI *pLayout, UINT8 baseData);
	void AddCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void DeleteCtl(CameraInfo cameraInfo, CDuiString layoutName, int BaseData);
	void GetCameraItem(CVerticalLayoutUI *pLayout);
	void CameraInfoInit(CameraInfo cameraInfo);
	CameraInfo GetCameraSetInfo(int id);

	void SetWatchTime(DWORD beginTime, DWORD endTime);
	void GetWatchTime(DWORD* pBegining, DWORD* pEnd);


	bool CameraSetIsChange();
	bool ShipNameIsChange();
	bool SysSetIsChange();
	bool AwTimeIsChange();
	bool AlarmVoiceIsChange();
	bool AlarmLightIsChange();

	virtual LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void KeyDown_VK_BACK();

	CameraInfo cameraInfo;
	SwtichRecord recordInfo;
	CameraItem camera[6];
private:

	CMenuItemUI*  AddMenuItem(CPort* pPort, CDuiString layoutName, int baseData);

	void  AddAlarmMenuItem(CPort* pPort);
	void  AddVideoObtainMenuItem(CPort* pPort);
	//void  AddCameraConfigMenuItem(CCamera* pCamera);
	void  AddPortConfigMenuItem(CPort * pPort);

	void  InitRecordFile(list<CRecordFileInfo*>* pList);
	void  InitAlarmFile(list<CRecordFileInfo*>* pList);

	void   FillPortConfig(CPort* pPort);
	void   InitAlarmVoice();
	
	CMenuItemUI* pAlarmItem[6];

	CTabLayoutUI *pLayout_third;
	CTabLayoutUI *pLayout_Menuitem;
	CTabLayoutUI *pLayout_PopMenu;
	CContainerUI *pKeyBoard;	//���̿ؼ�
	CMyEditUI * pShipName;		//����
	CMySliderUI* pSysVolum;		//ϵͳ����
	CMySliderUI* pSysLight;		//ϵͳ����
	CMyLabelUI* pHostModel;		//�����ͺ�
	CTimeButtonUI* pAwTime[4];  //�Զ�����
	CSwitchUI* pAlmVicSwitch; //����������
	CAlarmVoiceListUI* pDefaultVoice;	//Ĭ�ϱ�����
	CAlarmVoiceListUI* pVoice1;		//¼����
	CSwitchUI*	pAlarmLight;		//�����ƹ�
	CListUI*	pAwOnOffRecordList;		//�Զ��������ؼ�¼�б�
	CLabelUI* pPage;		//ҳ��

	CButtonUI *FocusedItem[2];
	UINT8 focusLevel;
	UINT8 mAlarmVoiceSel;
	int mAwTotalPage;
	int mAwPage;
	CPlayerWallWnd	*mPlayerWall;
};