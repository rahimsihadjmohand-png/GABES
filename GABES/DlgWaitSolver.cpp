// DlgWaitSolver.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgWaitSolver.h"
#include "GABESView.h"
#include "MainFrm.h"

#define TIME_ELAPSE_MS 10

// CDlgWaitSolver dialog

IMPLEMENT_DYNAMIC(CDlgWaitSolver, CDialogEx)

CDlgWaitSolver::CDlgWaitSolver(BEM_3D::Model* _pModel, CWnd* pParent)
	: CDialogEx(IDD_DLG_WAIT_SOLVER, pParent)
	, strTime(_T(""))
	, str_RHS_time(_T(""))
	, str_HMAT_time(_T(""))
	, str_SOLV_time(_T(""))
{
	m_STHD.pObj = _pModel;
	m_STHD.bRhsVec = FALSE;
	m_STHD.bHMatrix = FALSE;
	m_STHD.bSolved = FALSE;
	m_STHD.CompRatio = 0.0;
	m_STHD.nIters = 0;
	m_STHD.pWaitSolverDlg = this;
}

CDlgWaitSolver::~CDlgWaitSolver()
{
	
}

void CDlgWaitSolver::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_ADVANCE, PrgsAdvance);
	DDX_Text(pDX, IDC_STATIC_TIME, strTime);
	DDX_Control(pDX, IDOK, btnOk);
	DDX_Check(pDX, IDC_CHECK_RHS_VEC, m_STHD.bRhsVec);
	DDX_Check(pDX, IDC_CHECK_HMATRIX, m_STHD.bHMatrix);
	DDX_Check(pDX, IDC_CHECK_SOLVED, m_STHD.bSolved);

	if (BEM_3D::Model::m_solver == BEM_3D::GMRES)
	{
		DDX_Text(pDX, IDC_EDIT_NUM_ITERS, m_STHD.nIters);
	}
	else
	{
		CString str;
		str.Format(_T(""));
		SetDlgItemText(IDC_EDIT_NUM_ITERS, str);
	}


	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, IDC_EDIT_COMP_RATIO, m_STHD.CompRatio);
	}
	else
	{
		CString str;
		str.Format(_T("%.2f"), m_STHD.CompRatio);
		SetDlgItemText(IDC_EDIT_COMP_RATIO, str);
	}
	DDX_Text(pDX, IDC_STATIC_RHS_TIME, str_RHS_time);
	DDX_Text(pDX, IDC_STATIC_HMAT_TIME, str_HMAT_time);
	DDX_Text(pDX, IDC_STATIC_SOLV_TIME, str_SOLV_time);
}


BEGIN_MESSAGE_MAP(CDlgWaitSolver, CDialogEx)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_RHS_VECTOR_CONSTRUCTED, &CDlgWaitSolver::OnRHSVectorConstructed)
	ON_MESSAGE(WM_USER_HMATRIX_CONSTRUCTED, &CDlgWaitSolver::OnHMatrixConstructed)
END_MESSAGE_MAP()


void CDlgWaitSolver::FormatTime()
{
	// Get the Actual ticks
	LARGE_INTEGER ticksCurr;
	QueryPerformanceCounter(&ticksCurr);

	// Get Global time in seconds
	double totalSec = (double)(ticksCurr.QuadPart - ticksStart.QuadPart) / (double) ticksFreq.QuadPart;
	
	int hrs = (int)(totalSec / 3600.0);
	int min = (int)((totalSec - hrs * 3600.0) / 60.0);
	double sec = totalSec - hrs * 3600.0 - min * 60.0;
	int secInt = (int)sec;
	int frac = (int)((sec - secInt) * 100.0);  // 2 digits after comma

	strTime.Format(_T("%02u : %02u : %02d,%02d"), hrs, min, secInt, frac);

	// Get RHS vector time
	if (!m_STHD.bRhsVec)
	{
		str_RHS_time.Format(_T("%02u : %02u : %02d,%02d"), hrs, min, secInt, frac);
		str_HMAT_time.Format(_T("%02u : %02u : %02d,%02d"), 0, 0, 0, 0);
		str_SOLV_time.Format(_T("%02u : %02u : %02d,%02d"), 0, 0, 0, 0);
	}

	// Get H-Matrix time
	else if (!m_STHD.bHMatrix)
	{
		// Get Global time in seconds
		totalSec = (double)(ticksCurr.QuadPart - ticksHMatStart.QuadPart) / (double)ticksFreq.QuadPart;

		hrs = (int)(totalSec / 3600.0);
		min = (int)((totalSec - hrs * 3600.0) / 60.0);
		sec = totalSec - hrs * 3600.0 - min * 60.0;
		secInt = (int)sec;
		frac = (int)((sec - secInt) * 100.0);  // 2 digits after comma

		str_HMAT_time.Format(_T("%02u : %02u : %02d,%02d"), hrs, min, secInt, frac);
		str_SOLV_time.Format(_T("%02u : %02u : %02d,%02d"), 0, 0, 0, 0);
	}
	// Get Solver time
	else if (!m_STHD.bSolved)
	{
		// Get Global time in seconds
		totalSec = (double)(ticksCurr.QuadPart - ticksSolverStart.QuadPart) / (double)ticksFreq.QuadPart;

		hrs = (int)(totalSec / 3600.0);
		min = (int)((totalSec - hrs * 3600.0) / 60.0);
		sec = totalSec - hrs * 3600.0 - min * 60.0;
		secInt = (int)sec;
		frac = (int)((sec - secInt) * 100.0);  // 2 digits after comma

		str_SOLV_time.Format(_T("%02u : %02u : %02d,%02d"), hrs, min, secInt, frac);
	}
		
}

// CDlgWaitSolver message handlers

void CDlgWaitSolver::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	double Advance = m_STHD.pObj->m_CurrentAdvance * 100.0;

	PrgsAdvance.SetPos((int)(Advance));
	CDialogEx::OnTimer(nIDEvent);

	CString strAdvance;
	if(m_STHD.bRhsVec && m_STHD.bHMatrix && !m_STHD.bSolved)
		strAdvance.Format(_T(""));
	else 
		strAdvance.Format(_T("%.2f %%"), Advance);

	SetDlgItemText(IDC_STATIC_ADVANCE, strAdvance);
	SetDlgItemText(IDC_STATIC_CURRENT_JOB, m_STHD.pObj->m_strCurrentJob);


	if (m_STHD.pObj->m_bLengthyJob)
	{
		FormatTime();
		SetDlgItemText(IDC_STATIC_TIME, strTime);
		SetDlgItemText(IDC_STATIC_RHS_TIME, str_RHS_time);
		SetDlgItemText(IDC_STATIC_HMAT_TIME, str_HMAT_time);
		SetDlgItemText(IDC_STATIC_SOLV_TIME, str_SOLV_time);
	}
	else
	{
		btnOk.EnableWindow(TRUE);
	}
}



void CDlgWaitSolver::StartSolver()
{
	m_STHD.bRhsVec = FALSE;
	m_STHD.bHMatrix = FALSE;
	m_STHD.bSolved = FALSE;
	m_STHD.CompRatio = 0.0;
	m_STHD.nIters = 0;

	SetWindowText(_T("Solving Linear System"));


	if (BEM_3D::Model::m_solver == BEM_3D::H_LU)
	{
		GetDlgItem(IDC_EDIT_NUM_ITERS)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NUM_ITERS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_NUM_ITERS)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_NUM_ITERS)->EnableWindow(TRUE);
	}

	m_STHD.pObj->ResolveLinearSystem_H2LIB((LPVOID)&m_STHD);


	QueryPerformanceCounter(&ticksStart);
	QueryPerformanceFrequency(&ticksFreq);

	btnOk.EnableWindow(FALSE);

	SetTimer(0, TIME_ELAPSE_MS, NULL);

	UpdateData(FALSE);
}


LRESULT CDlgWaitSolver::OnRHSVectorConstructed(WPARAM wParam, LPARAM lParam)
{
	QueryPerformanceCounter(&ticksHMatStart);

	UpdateData(FALSE);

	return 0;
}

LRESULT CDlgWaitSolver::OnHMatrixConstructed(WPARAM wParam, LPARAM lParam)
{
	QueryPerformanceCounter(&ticksSolverStart);

	UpdateData(FALSE);

	return 0;
}




void CDlgWaitSolver::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	m_STHD.pObj->m_bLengthyJob = false;

	m_STHD.pObj->ClearBEMVectors();

	ShowWindow(SW_HIDE);
}

void CDlgWaitSolver::OnOK()
{
	// Get the Frame Window
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();

	// Get the View Window
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// TODO: Add your specialized code here and/or call the base class
	m_STHD.pObj->m_bLengthyJob = false;

    // Update the boundary unknowns and Set the view mode to post-processing
	// With an undeformed body and Von-Mises Stress output mode
	m_STHD.pObj->UpdateBoundaryUnknowns();
	pView->m_bPostProcessing = true;
	BEM_3D::Vertex::m_bIncludeDisplacements = false;
	m_STHD.pObj->SetOutputMode(BEM_3D::S_VON_MISES);
	m_STHD.pObj->UpdateVertexBuffer(pView->m_pD3ddev, pView->m_bPostProcessing);
	// Update the Output Mode Combo boxes in the View toolbar
	
	// Get the OutputMode Combobox
	CMFCToolBarComboBoxButton* m_pCmbBtnOutputMode =
		(CMFCToolBarComboBoxButton*)pMainFrame->m_ViewOptionsBar.GetButton(
			pMainFrame->m_ViewOptionsBar.CommandToIndex(ID_VIEW_OUTPUTMODE));
	// Get the Variable Combobox
	CMFCToolBarComboBoxButton* m_pCmbBtnVariable =
		(CMFCToolBarComboBoxButton*)pMainFrame->m_ViewOptionsBar.GetButton(
			pMainFrame->m_ViewOptionsBar.CommandToIndex(ID_VIEW_VARIABLE));

	// Set the selection to Stress in the OutputMode Combobox
	if (m_pCmbBtnOutputMode != nullptr)
		m_pCmbBtnOutputMode->SelectItem(2);

	// Update the Variable Combobox
	pMainFrame->OnCbnSelChangeOutputMode();

	// Set the selection to Von-Mises in the Variable Combobox
	if (m_pCmbBtnVariable != nullptr)
		m_pCmbBtnVariable->SelectItem(9);

	// Set the Modified Flag to true (default argument)
	pView->GetDocument()->SetModifiedFlag();

	ShowWindow(SW_HIDE);
}
