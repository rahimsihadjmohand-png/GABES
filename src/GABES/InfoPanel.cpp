// CInfoPanel.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "InfoPanel.h"


// CInfoPanel

IMPLEMENT_DYNAMIC(CInfoPanel, CDockablePane)

CInfoPanel::CInfoPanel()
{

}

CInfoPanel::~CInfoPanel()
{
}


BEGIN_MESSAGE_MAP(CInfoPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CInfoPanel message handlers



int CInfoPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_InfoTab.CreateEx(TCS_EX_FLATSEPARATORS, WS_CHILD | WS_BORDER | WS_VISIBLE ,
		CRect(0, 0, 0, 0), this, ID_TAB_CTRL);

	// Set its font to match the app’s GUI font ---
	CFont* pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	m_InfoTab.SetFont(pFont, TRUE);

	return 0;
}

void CInfoPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// Resize the Tab Control to occupy the Panel client area
	m_InfoTab.MoveWindow(0, 0, cx, cy);
}


BOOL CInfoPanel::CanBeClosed() const
{
	return FALSE;   // 🔒 prevents closing (docked AND floating)
}


