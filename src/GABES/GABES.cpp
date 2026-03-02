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

// GABES.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "GABES.h"
#include "MainFrm.h"

#include "GABESDoc.h"
#include "GABESView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGABESApp

BEGIN_MESSAGE_MAP(CGABESApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CGABESApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CGABESApp construction

CGABESApp::CGABESApp() noexcept
{

	m_nAppLook = 0;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("GABES.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CGABESApp object

CGABESApp theApp;


// CGABESApp initialization

BOOL CGABESApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGABESDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGABESView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CGABESApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CGABESApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickGabesGithubLink(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickRahimEmailLink(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickRahimResearchGateLink(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonCopyEmail();
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_GABES_GITHUB_LINK, &CAboutDlg::OnNMClickGabesGithubLink)
	ON_NOTIFY(NM_CLICK, IDC_RAHIM_EMAIL_LINK, &CAboutDlg::OnNMClickRahimEmailLink)
	ON_NOTIFY(NM_CLICK, IDC_RAHIM_RESEARCH_GATE_LINK, &CAboutDlg::OnNMClickRahimResearchGateLink)
	ON_BN_CLICKED(IDC_BUTTON_COPY_EMAIL, &CAboutDlg::OnBnClickedButtonCopyEmail)
END_MESSAGE_MAP()

// App command to run the dialog
void CGABESApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CGABESApp customization load/save methods

void CGABESApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CGABESApp::LoadCustomState()
{
}

void CGABESApp::SaveCustomState()
{
}

// CGABESApp message handlers




int CGABESApp::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	MSG msg;
	bool running = true;
	LONG lIdleCount = 0;
	bool bIdle = true;

	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = false;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			bIdle = true;  // a message was handled, so next idle cycle starts fresh
			lIdleCount = 0;
		}

		if (bIdle)
		{
			while (OnIdle(lIdleCount++))
			{
				// Keep calling OnIdle until it returns false
			}
			bIdle = false;
		}

		// Do game update and render
		CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
		CGABESView* pView = nullptr;
		if(pMainFrame)
			pView = (CGABESView*)pMainFrame->GetActiveView();
		
		if (pView)
		{
			pView->RenderFrameD3D();
		}
	}

	return (int)msg.wParam;

}

BOOL CGABESApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	
	// manually call OnIdleUpdateCmdUI
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame && ::IsWindow(pMainFrame->GetSafeHwnd()))
		pMainFrame->OnIdleUpdateCmdUI(); 
	



	return CWinAppEx::OnIdle(lCount);
}

void CAboutDlg::OnNMClickGabesGithubLink(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	ShellExecute(
		NULL,
		_T("open"),
		_T("https://github.com/rahimsihadjmohand-png/GABES.git"),
		NULL,
		NULL,
		SW_SHOWNORMAL);

	*pResult = 0;
}

void CAboutDlg::OnNMClickRahimEmailLink(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	ShellExecute(
		NULL,
		_T("open"),
		_T("mailto:rahim.si_hadj_mohand@g.enp.edu.dz?subject=GABES%20Support"),
		NULL,
		NULL,
		SW_SHOWNORMAL);

	*pResult = 0;
}

void CAboutDlg::OnNMClickRahimResearchGateLink(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	ShellExecute(
		NULL,
		_T("open"),
		_T("https://www.researchgate.net/profile/Rahim-Si-Hadj-Mohand"),
		NULL,
		NULL,
		SW_SHOWNORMAL);
	*pResult = 0;
}

void CAboutDlg::OnBnClickedButtonCopyEmail()
{
	// TODO: Add your control notification handler code here
	CString strEmail = _T("rahim.si_hadj_mohand@g.enp.edu.dz");

	if (OpenClipboard())
	{
		EmptyClipboard();

		// Allocate global memory for the text
		HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE,
			(strEmail.GetLength() + 1) * sizeof(TCHAR));

		if (hGlob)
		{
			LPTSTR pData = (LPTSTR)GlobalLock(hGlob);
			_tcscpy_s(pData, strEmail.GetLength() + 1, strEmail);
			GlobalUnlock(hGlob);

#ifdef UNICODE
			SetClipboardData(CF_UNICODETEXT, hGlob);
#else
			SetClipboardData(CF_TEXT, hGlob);
#endif
		}

		CloseClipboard();
	}
}
