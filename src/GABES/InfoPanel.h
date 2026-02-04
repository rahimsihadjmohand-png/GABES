#pragma once
#include "InfoTab.h"


// CInfoPanel
#define ID_INFO_PANEL 13111961 
#define ID_TAB_CTRL   21061969


class CInfoPanel : public CDockablePane
{
	DECLARE_DYNAMIC(CInfoPanel)

public:
	CInfoPanel();
	virtual ~CInfoPanel();



	void UpdateInfo() 
	{ 
		m_InfoTab.UpdateInfo(); 
	}

public:
	CInfoTab m_InfoTab;

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL CanBeClosed() const override;


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


