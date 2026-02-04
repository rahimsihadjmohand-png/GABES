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

// MainFrm.cpp : implementation of the CMainFrame class
//
#include "pch.h"
#include "framework.h"
#include "GABES.h"

#include "MainFrm.h"

#include "GABESDoc.h"
#include "GABESView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, &CMainFrame::OnToolbarReset)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILLMODE, &CMainFrame::OnUpdateViewFillModeCombo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWPOINTS_MODE, &CMainFrame::OnUpdateViewShowPointsModeCombo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUTMODE, &CMainFrame::OnUpdateViewOutputModeCombo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VARIABLE, &CMainFrame::OnUpdateViewVariableCombo)
	ON_CBN_SELCHANGE(ID_VIEW_FILLMODE, &CMainFrame::OnCbnSelChangeFillMode)
	ON_CBN_SELCHANGE(ID_VIEW_SHOWPOINTS_MODE, &CMainFrame::OnCbnSelChangeShowPointsMode)
	ON_CBN_SELCHANGE(ID_VIEW_OUTPUTMODE, &CMainFrame::OnCbnSelChangeOutputMode)
	ON_CBN_SELCHANGE(ID_VIEW_VARIABLE, &CMainFrame::OnCbnSelChangeVariable)
	ON_MESSAGE(WM_INITIALIZE_VIEWOPTIONS_TOOLBAR, &CMainFrame::OnInitializeViewOptionsToolbar)
	ON_WM_POWERBROADCAST()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WINDOWS_7);


}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	
	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);


	// Create a bold GUI font for the status bar
	LOGFONT lf = { 0 };
	CFont* pGuiFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pGuiFont->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;                  // Make it bold
	m_statusBarFont.CreateFontIndirect(&lf);
	m_wndStatusBar.SetFont(&m_statusBarFont);


	// Create and Load the View Options Toolbar
	if (!m_ViewOptionsBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_ViewOptionsBar.LoadToolBar(IDR_VIEWOPTIONS_TOOLBAR))
	{
		TRACE0("Failed to create the geometry toolbar\n");
		return -1;      // fail to create
	}


	// Create the ModelTree panel
	m_ModelTreePanel.Create(_T("Model Tree"), this, CRect(0, 0, 350, 300),
		TRUE, ID_SUBSETS_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_RIGHT | CBRS_FLOAT_MULTI);
	

	// Create the Info panel
	m_infoPanel.Create(_T("Information"), this, CRect(0, 0, 350, 300),
		TRUE, ID_INFO_PANEL,
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_RIGHT | CBRS_FLOAT_MULTI);


	// Dock the panels and bars
	//m_ViewOptionsBar.EnableDocking(CBRS_ALIGN_TOP);
	m_ModelTreePanel.EnableDocking(CBRS_ALIGN_ANY);
	m_infoPanel.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_ViewOptionsBar);
	DockPane(&m_ModelTreePanel);
	m_infoPanel.DockToWindow(&m_ModelTreePanel, CBRS_ALIGN_BOTTOM);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);



	// Post this Message to Force the update of the ViewOptions Toolbar
	PostMessage(WM_INITIALIZE_VIEWOPTIONS_TOOLBAR);


	// Set the Document and View Pointers for the panels
	// the cal to InitialUpdateFrame causes OnInitialUpdate to be called in the view object
	// which attaches it to the document in order to have a valid document pointer
	InitialUpdateFrame(nullptr, TRUE);
	

	// Set the Doc and View pointers for our SubSetsPanel and initialize its tree control
	m_ModelTreePanel.m_pDoc = (CGABESDoc*)GetActiveDocument();
	m_ModelTreePanel.m_pView = (CGABESView*)GetActiveView();
	m_ModelTreePanel.InitTreeCtrl();

	// Set the Doc and View pointers for our infoPanel and initialize its tab control
	CInfoTab& rInfoTab = m_infoPanel.m_InfoTab;
	rInfoTab.m_pDoc = (CGABESDoc*)GetActiveDocument();
	rInfoTab.m_pView = (CGABESView*)GetActiveView();
	rInfoTab.InitTabCtrl();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnApplicationLook(UINT id)
{
	/*CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}*/

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
	CDockingManager::SetDockingMode(DT_SMART);
	m_wndRibbonBar.SetWindows7Look(TRUE);

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // force Print Preview mode closed
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}



LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM lp)
{
	UINT uiToolBarId = (UINT)wp;

	switch (uiToolBarId)
	{
	case IDR_VIEWOPTIONS_TOOLBAR:
	{
		ASSERT(m_ViewOptionsBar.GetSafeHwnd() != NULL);

		

		// Replace "ID_VIEW_FILLMODE" button by a combobox:
		CMFCToolBarComboBoxButton comboFillMode(ID_VIEW_FILLMODE,
			GetCmdMgr()->GetCmdImage(ID_VIEW_FILLMODE),
			CBS_DROPDOWNLIST, 110);

		comboFillMode.AddItem(_T("Solid"));
		comboFillMode.AddItem(_T("Wireframe"));
		comboFillMode.AddItem(_T("Edged Faces"));
		comboFillMode.SelectItem(0);

		// Replace the placeholder in the actual toolbar
		m_ViewOptionsBar.ReplaceButton(ID_VIEW_FILLMODE, comboFillMode);

		CMFCToolBarComboBoxButton* pCmbBtn = (CMFCToolBarComboBoxButton*)
			m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_FILLMODE));

		CComboBox* pComboBox = pCmbBtn->GetComboBox();

		if (pComboBox != nullptr)
		{
			pComboBox->SetCurSel(0);
		}


		// Replace "ID_VIEW_SHOWPOINTS_MODE" button by a combobox:
		CMFCToolBarComboBoxButton comboShowPointsMode(ID_VIEW_SHOWPOINTS_MODE,
			GetCmdMgr()->GetCmdImage(ID_VIEW_SHOWPOINTS_MODE),
			CBS_DROPDOWNLIST, 150);

		comboShowPointsMode.AddItem(_T("Hide Points"));
		comboShowPointsMode.AddItem(_T("Show Vertices"));
		comboShowPointsMode.AddItem(_T("Show DOF Nodes"));
		comboShowPointsMode.SelectItem(0);

		// Replace the placeholder in the actual toolbar
		m_ViewOptionsBar.ReplaceButton(ID_VIEW_SHOWPOINTS_MODE, comboShowPointsMode);

		pCmbBtn = (CMFCToolBarComboBoxButton*)
			m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_SHOWPOINTS_MODE));

		pComboBox = pCmbBtn->GetComboBox();

		if (pComboBox != nullptr)
		{
			pComboBox->SetCurSel(0);
		}


		

		// Replace "ID_VIEW_OUTPUTMODE" button by a combobox:
		CMFCToolBarComboBoxButton comboOutputMode(ID_VIEW_OUTPUTMODE,
			GetCmdMgr()->GetCmdImage(ID_VIEW_OUTPUTMODE),
			CBS_DROPDOWNLIST, 120);

		comboOutputMode.AddItem(_T("Nothing"));
		comboOutputMode.AddItem(_T("Displacement"));
		comboOutputMode.AddItem(_T("Stress"));
		comboOutputMode.AddItem(_T("Strain"));
		comboOutputMode.SelectItem(0);

		// Replace the placeholder in the actual toolbar
		m_ViewOptionsBar.ReplaceButton(ID_VIEW_OUTPUTMODE, comboOutputMode);


	
		// Replace "ID_VIEW_OUTPUTMODE" button by a combobox:
		CMFCToolBarComboBoxButton comboButton3(ID_VIEW_VARIABLE,
			GetCmdMgr()->GetCmdImage(ID_VIEW_VARIABLE),
			CBS_DROPDOWNLIST, 120);

		// Replace the placeholder in the actual toolbar
		m_ViewOptionsBar.ReplaceButton(ID_VIEW_VARIABLE, comboButton3);

	}
	break;
	}


	return 0;
}


void CMainFrame::OnUpdateViewFillModeCombo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE); // ✅ This enables the combo box
}

void CMainFrame::OnUpdateViewShowPointsModeCombo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE); // ✅ This enables the combo box
}


void CMainFrame::OnUpdateViewOutputModeCombo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE); // ✅ This enables the combo box
}

void CMainFrame::OnUpdateViewVariableCombo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE); // ✅ This enables the combo box
}

void CMainFrame::OnCbnSelChangeFillMode()
{
	CGABESView* pView = (CGABESView *) GetActiveView();

	// Get the ComboBox pointer	
	CMFCToolBarComboBoxButton* pCmbBtn = (CMFCToolBarComboBoxButton*)
		m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_FILLMODE));

	CComboBox* pComboBox = pCmbBtn->GetComboBox();
	int nSel = pComboBox->GetCurSel();

	pView->m_FillMode = (BEM_3D::FILL_MODE)nSel;
}

void CMainFrame::OnCbnSelChangeShowPointsMode()
{
	CGABESView* pView = (CGABESView*)GetActiveView();

	// Get the ComboBox pointer	
	CMFCToolBarComboBoxButton* pCmbBtn = (CMFCToolBarComboBoxButton*)
		m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_SHOWPOINTS_MODE));

	CComboBox* pComboBox = pCmbBtn->GetComboBox();
	int nSel = pComboBox->GetCurSel();

	pView->m_ShowPointsMode = (BEM_3D::SHOW_POINTS_MODE)nSel;
}


void CMainFrame::OnCbnSelChangeOutputMode()
{
	CGABESView* pView = (CGABESView*)GetActiveView();

	// Get the ComboBox pointer	
	CMFCToolBarComboBoxButton* pCmbBtn = (CMFCToolBarComboBoxButton*)
		m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_OUTPUTMODE));

	CComboBox* pComboBox = pCmbBtn->GetComboBox();
	int nSel = pComboBox->GetCurSel();

	// Set the contents of the Variable comboBox
	pCmbBtn = (CMFCToolBarComboBoxButton*)
		m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_VARIABLE));

	pComboBox = pCmbBtn->GetComboBox();
	pComboBox->ResetContent();
	switch (nSel)
	{
	case 0: // Nothing
		pCmbBtn->SelectItem(-1);
		//pView->pModel->SetOutputMode(BEM_3D::NONE);
		break;

	case 1: // Displacement
		pCmbBtn->AddItem(_T("U1"));
		pCmbBtn->AddItem(_T("U2"));
		pCmbBtn->AddItem(_T("U3"));
		pCmbBtn->AddItem(_T("Magnitude"));
		pCmbBtn->SelectItem(3);
		//pView->pModel->SetOutputMode(BEM_3D::U_M);
		break;

	case 2: // Stress
		pCmbBtn->AddItem(_T("Sig_11"));
		pCmbBtn->AddItem(_T("Sig_22"));
		pCmbBtn->AddItem(_T("Sig_33"));
		pCmbBtn->AddItem(_T("Sig_12"));
		pCmbBtn->AddItem(_T("Sig_23"));
		pCmbBtn->AddItem(_T("Sig_13"));
		pCmbBtn->AddItem(_T("VonMises"));
		pCmbBtn->SelectItem(6);
		//pView->pModel->SetOutputMode(BEM_3D::S_VON_MISES);
		break;

	case 3: // Strain
		pCmbBtn->AddItem(_T("Eps_11"));
		pCmbBtn->AddItem(_T("Eps_22"));
		pCmbBtn->AddItem(_T("Eps_33"));
		pCmbBtn->AddItem(_T("Eps_12"));
		pCmbBtn->AddItem(_T("Eps_23"));
		pCmbBtn->AddItem(_T("Eps_13"));
		pCmbBtn->AddItem(_T("VonMises"));
		pCmbBtn->SelectItem(6);
		//pView->pModel->SetOutputMode(BEM_3D::S_VON_MISES);
		break;


	default:
		break;
	}

	// Force the Variable ComboBox to update
	pComboBox->GetParent()->SendMessage(WM_COMMAND,
		MAKEWPARAM(pComboBox->GetDlgCtrlID(), CBN_SELCHANGE),
		(LPARAM)pComboBox->GetSafeHwnd());

	pView->pModel->UpdateVertexBuffer(pView->GetD3dDevice(), pView->m_bPostTreatment);
}


void CMainFrame::OnCbnSelChangeVariable()
{
	CGABESView* pView = (CGABESView*)GetActiveView();

	// Get the Selected Output Mode
	CMFCToolBarComboBoxButton* pCmbBtn = (CMFCToolBarComboBoxButton*)
		m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_OUTPUTMODE));

	CComboBox* pComboBox = pCmbBtn->GetComboBox();
	int nOutputMode = pComboBox->GetCurSel();

	// Get the Slected variable
	pCmbBtn = (CMFCToolBarComboBoxButton*)
		m_ViewOptionsBar.GetButton(m_ViewOptionsBar.CommandToIndex(ID_VIEW_VARIABLE));

	pComboBox = pCmbBtn->GetComboBox();
	int nVariable = pComboBox->GetCurSel();

	switch (nOutputMode)
	{
	case 0: // Nothing
		pView->pModel->SetOutputMode(BEM_3D::NONE);
		break;

	case 1: // Displacement
		switch (nVariable)
		{
		case 0: // U1
			pView->pModel->SetOutputMode(BEM_3D::U_1);
			break;

		case 1: // U2
			pView->pModel->SetOutputMode(BEM_3D::U_2);
			break;

		case 2: // U3
			pView->pModel->SetOutputMode(BEM_3D::U_3);
			break;

		case 3: // Magnitude
			pView->pModel->SetOutputMode(BEM_3D::U_M);
			break;

		default:
			pView->pModel->SetOutputMode(BEM_3D::NONE);
			break;
		}
		break;

	case 2: // Stress
		switch (nVariable)
		{
		case 0: // Sig_11
			pView->pModel->SetOutputMode(BEM_3D::S_11);
			break;

		case 1: // Sig_22
			pView->pModel->SetOutputMode(BEM_3D::S_22);
			break;

		case 2: // Sig_33
			pView->pModel->SetOutputMode(BEM_3D::S_33);
			break;

		case 3: // Sig_12
			pView->pModel->SetOutputMode(BEM_3D::S_12);
			break;

		case 4: // Sig_23
			pView->pModel->SetOutputMode(BEM_3D::S_23);
			break;

		case 5: // Sig_13
			pView->pModel->SetOutputMode(BEM_3D::S_13);
			break;

		case 6: // VonMises
			pView->pModel->SetOutputMode(BEM_3D::S_VON_MISES);
			break;

		default:
			pView->pModel->SetOutputMode(BEM_3D::NONE);
			break;
		}
		break;

	case 3: // Strain
		switch (nVariable)
		{
		case 0: // Eps_11
			pView->pModel->SetOutputMode(BEM_3D::E_11);
			break;

		case 1: // Eps_22
			pView->pModel->SetOutputMode(BEM_3D::E_22);
			break;

		case 2: // Eps_33
			pView->pModel->SetOutputMode(BEM_3D::E_33);
			break;

		case 3: // Eps_12
			pView->pModel->SetOutputMode(BEM_3D::E_12);
			break;

		case 4: // Eps_23
			pView->pModel->SetOutputMode(BEM_3D::E_23);
			break;

		case 5: // Eps_13
			pView->pModel->SetOutputMode(BEM_3D::E_13);
			break;

		case 6: // VonMises
			pView->pModel->SetOutputMode(BEM_3D::E_VON_MISES);
			break;

		default:
			pView->pModel->SetOutputMode(BEM_3D::NONE);
			break;
		}
		break;

	default:
		pView->pModel->SetOutputMode(BEM_3D::NONE);
		break;
	}

	pView->pModel->UpdateVertexBuffer(pView->GetD3dDevice(), pView->m_bPostTreatment);
}


LRESULT CMainFrame::OnInitializeViewOptionsToolbar(WPARAM, LPARAM)
{
	// Get Active view	
	CGABESView* pView = (CGABESView*)GetActiveView();


	// Get the ComboBox and reset it manually
	CMFCToolBarComboBoxButton* m_pCmbBtnFillMode =
		(CMFCToolBarComboBoxButton*)m_ViewOptionsBar.GetButton(
			m_ViewOptionsBar.CommandToIndex(ID_VIEW_FILLMODE));

	if (m_pCmbBtnFillMode != nullptr)
		m_pCmbBtnFillMode->SelectItem((int)pView->m_FillMode); // Set to the item you want (e.g., Solid)
	


	CMFCToolBarComboBoxButton* m_pCmbBtnShowPointsMode =
		(CMFCToolBarComboBoxButton*)m_ViewOptionsBar.GetButton(
			m_ViewOptionsBar.CommandToIndex(ID_VIEW_SHOWPOINTS_MODE));

	if (m_pCmbBtnShowPointsMode != nullptr)
		m_pCmbBtnShowPointsMode->SelectItem((int)pView->m_ShowPointsMode); // Set to the item you want (e.g., Solid)


	BEM_3D::OUTPUT_MODE outputMode = pView->pModel->GetOutputMode();


	CMFCToolBarComboBoxButton* m_pCmbBtnOutputMode =
		(CMFCToolBarComboBoxButton*)m_ViewOptionsBar.GetButton(
			m_ViewOptionsBar.CommandToIndex(ID_VIEW_OUTPUTMODE));
	
	if (m_pCmbBtnOutputMode != nullptr)
	{
		int nSel = -1;
		
		switch (outputMode)
		{
		case BEM_3D::NONE:
			nSel = 0;
			break;

		case BEM_3D::U_1:
		case BEM_3D::U_2:
		case BEM_3D::U_3:
		case BEM_3D::U_M:
			nSel = 1;
			break;

		case BEM_3D::S_11:
		case BEM_3D::S_22:
		case BEM_3D::S_33:
		case BEM_3D::S_12:
		case BEM_3D::S_13:
		case BEM_3D::S_23:
		case BEM_3D::S_VON_MISES:
		case BEM_3D::S_TRESCA:
			nSel = 2;
			break;

		case BEM_3D::E_11:
		case BEM_3D::E_22:
		case BEM_3D::E_33:
		case BEM_3D::E_12:
		case BEM_3D::E_13:
		case BEM_3D::E_23:
		case BEM_3D::E_VON_MISES:
		case BEM_3D::E_TRESCA:
			nSel = 3;
			break;

		default:
			break;
		}

		m_pCmbBtnOutputMode->SelectItem(nSel); // Set to the item you want (e.g., Solid)
	}


	CMFCToolBarComboBoxButton* m_pCmbBtnVariable =
		(CMFCToolBarComboBoxButton*)m_ViewOptionsBar.GetButton(
			m_ViewOptionsBar.CommandToIndex(ID_VIEW_VARIABLE));


	if (m_pCmbBtnVariable != nullptr)
	{
		int nSel = -1;

		switch (outputMode)
		{
		case BEM_3D::NONE:
			nSel = -1;
			break;

		case BEM_3D::U_1:
			nSel = 0;
			break;

		case BEM_3D::U_2:
			nSel = 1;
			break;
		    
		case BEM_3D::U_3:
			nSel = 2;
			break;

		case BEM_3D::U_M:
			nSel = 3;
			break;			

		case BEM_3D::S_11:
			nSel = 0;
			break;

		case BEM_3D::S_22:
			nSel = 1;
			break;

		case BEM_3D::S_33:
			nSel = 2;
			break;

		case BEM_3D::S_12:
			nSel = 3;
			break;

		case BEM_3D::S_23:
			nSel = 4;
			break;

		case BEM_3D::S_13:
			nSel = 5;
			break;

		case BEM_3D::S_VON_MISES:
			nSel = 6;
			break;

		

		case BEM_3D::E_11:
			nSel = 0;
			break;

		case BEM_3D::E_22:
			nSel = 1;
			break;

		case BEM_3D::E_33:
			nSel = 2;
			break;

		case BEM_3D::E_12:
			nSel = 3;
			break;

		case BEM_3D::E_23:
			nSel = 4;
			break;

		case BEM_3D::E_13:
			nSel = 5;
			break;

		case BEM_3D::E_VON_MISES:
			nSel = 6;
			break;

		default:
			break;
		}

		m_pCmbBtnVariable->SelectItem(nSel); // Set to the item you want (e.g., Solid)
	}



	return 0;
}



void CMainFrame::OnIdleUpdateCmdUI()
{
	CGABESView* pView = (CGABESView*)GetActiveView();
	CGABESDoc* pDoc = (CGABESDoc*)GetActiveDocument();

	if (pView)
		pView->OnUpdateCmdUI(this, TRUE);

	
	if (pDoc)
	{
		// Get the principal pane
		auto Pane = m_wndStatusBar.GetElement(0);  // <===I receive Runtime crash here!!!

		if (Pane == nullptr)
			return;

		CRect rcPane = Pane->GetRect();
		rcPane.SetRect(rcPane.left, rcPane.top, rcPane.left, rcPane.bottom);

		// Get a reference to the Model
		const BEM_3D::Model& rModel = pDoc->m_Model;


		// Get the index of the Hittested element
		int nHTDElm = -1;

		if (pView)
			nHTDElm = pView->m_HitTestedElmIdx;

		if (nHTDElm < 0)
		{
			Pane->SetText(_T(""));
			Pane->SetRect(rcPane);
			m_wndStatusBar.RecalcLayout();
			m_wndStatusBar.RedrawWindow();
			return;
		}

		// Get the Adress of the Hittested element
		const BEM_3D::Element* pHTDElm = rModel.GetElements()[nHTDElm];

		if (pHTDElm == nullptr)
		{
			Pane->SetText(_T(""));
			Pane->SetRect(rcPane);
			m_wndStatusBar.RecalcLayout();
			m_wndStatusBar.RedrawWindow();
			return;
		}

		// Get the 3 Vertices
		BEM_3D::Vertex* pV1 = pHTDElm->m_pV1;
		BEM_3D::Vertex* pV2 = pHTDElm->m_pV2;
		BEM_3D::Vertex* pV3 = pHTDElm->m_pV3;

		
		rcPane.SetRect(rcPane.left, rcPane.top, rcPane.left + 500, rcPane.bottom);

		CString strHTDElm;
		strHTDElm.Format(_T("Element of index [%d] ===> { V1(%.2f, %.2f, %.2f), V2(%.2f, %.2f, %.2f), V3(%.2f, %.2f, %.2f) }"),
			nHTDElm, pV1->x, pV1->y, pV1->z, pV2->x, pV2->y, pV2->z, pV3->x, pV3->y, pV3->z);

		Pane->SetText(strHTDElm);
		Pane->SetRect(rcPane);
		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();
	}
	
}



UINT CMainFrame::OnPowerBroadcast(UINT nPowerEvent, LPARAM nEventData)
{
	if (nPowerEvent == PBT_APMRESUMEAUTOMATIC)
	{
		CGABESView* pView = (CGABESView*)GetActiveView();


		pView->RecreateD3dComponents();

		return TRUE;
	}

	return 	CFrameWndEx::OnPowerBroadcast(nPowerEvent, nEventData);

}



