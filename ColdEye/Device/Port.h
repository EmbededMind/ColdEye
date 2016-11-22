#pragma once



class CPort
{
public:
	CPort();
	~CPort();

	void  SetId(int id);
	int   GetId();
	void  SetNameId(int id);
	int   GetNameId();

    CString& GetName();

	void  SetMac(char* sMac);
	void  SetMac(UCHAR* pchMac);
    char* GetMac();

	void  SetVisibility(bool isVisible);
	bool  GetVisibility();

private:
	int  m_Id;
	int  m_nameId;
	char m_mac[20];
	bool m_bIsVisible;
	//int  m_recordFileNumber;
	//int  m_alarmFileNumber;
};