#pragma once


// CFuckButton

class CFuckButton : public CButton
{
	DECLARE_DYNAMIC(CFuckButton)

public:
	CFuckButton();
	virtual ~CFuckButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


