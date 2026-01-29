// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ModelTreePanel.h"
#include "InfoPanel.h"

#define WM_INITIALIZE_VIEWOPTIONS_TOOLBAR WM_USER+100


class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	CModelTreePanel& GetModelTreePanel() { return m_ModelTreePanel; }
	const CModelTreePanel& GetModelTreePanel()const { return m_ModelTreePanel; }

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnIdleUpdateCmdUI();  // ?? manually declare it here
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CMFCToolBar m_ViewOptionsBar;
	CModelTreePanel m_ModelTreePanel;
	CInfoPanel m_infoPanel;
	CFont m_statusBarFont;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarReset(WPARAM wp, LPARAM lp);
	afx_msg void OnUpdateViewFillModeCombo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewShowPointsModeCombo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewOutputModeCombo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewVariableCombo(CCmdUI* pCmdUI);
	afx_msg void OnCbnSelChangeFillMode();
	afx_msg void OnCbnSelChangeShowPointsMode();
	afx_msg void OnCbnSelChangeOutputMode();
	afx_msg void OnCbnSelChangeVariable();
	afx_msg LRESULT OnInitializeViewOptionsToolbar(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg UINT OnPowerBroadcast(UINT nPowerEvent, LPARAM nEventData);
};


