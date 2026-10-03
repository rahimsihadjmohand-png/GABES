// DlgHMatrixViewer.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgHMatrixViewer.h"


// CDlgHMatrixViewer dialog

IMPLEMENT_DYNAMIC(CDlgHMatrixViewer, CDialogEx)

CDlgHMatrixViewer::CDlgHMatrixViewer(const BEM_3D::Model* pModel, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_HMATRIX_VIEWER, pParent)
	, m_HMatViewer(pModel)
	, m_cx(0)
	, m_cy(0)
{

}

CDlgHMatrixViewer::~CDlgHMatrixViewer()
{
}

void CDlgHMatrixViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEWER_FRAME, m_ViewerFrame);
	DDX_Control(pDX, IDOK, m_OKbtn);
}


BEGIN_MESSAGE_MAP(CDlgHMatrixViewer, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgHMatrixViewer message handlers

BOOL CDlgHMatrixViewer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// Here create the HMatrix Window 
	m_HMatViewer.CreateWnd(&m_ViewerFrame);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHMatrixViewer::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// Calculate the change in size

	if ((m_OKbtn.m_hWnd == nullptr) || (m_ViewerFrame.m_hWnd == nullptr) || (m_HMatViewer.m_hWnd == nullptr))
	{
		m_cx = cx;
		m_cy = cy;

		return;
	}
		

	int dx = cx - m_cx;
	int dy = cy - m_cy;

	m_cx = cx;
	m_cy = cy;
	
	// Move the OK Button and resize the Frame window and the H-Matrix Viewer
	CRect rcOKbtn, rcFrame, rcViewer;
	
	m_OKbtn.GetWindowRect(rcOKbtn);
	m_ViewerFrame.GetWindowRect(rcFrame);
	m_HMatViewer.GetWindowRect(rcViewer);

	rcOKbtn.left += dx;
	rcOKbtn.right += dx;
	rcOKbtn.top += dy;
	rcOKbtn.bottom += dy;

	rcFrame.right += dx;
	rcFrame.bottom += dy;

	rcViewer.right += dx;
	rcViewer.bottom += dy;


	ScreenToClient(rcOKbtn);
	ScreenToClient(rcFrame);
	m_ViewerFrame.ScreenToClient(rcViewer);

	m_OKbtn.MoveWindow(rcOKbtn);
	m_ViewerFrame.MoveWindow(rcFrame);
	m_HMatViewer.MoveWindow(rcViewer);
}
