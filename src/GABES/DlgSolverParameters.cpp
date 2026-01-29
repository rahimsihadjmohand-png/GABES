// DlgSolverParameters.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSolverParameters.h"
#include "Model.h"

using BEM_3D::Model;

// CDlgSolverParameters dialog

IMPLEMENT_DYNAMIC(CDlgSolverParameters, CDialogEx)

CDlgSolverParameters::CDlgSolverParameters(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SOLVER_PARAMETERS, pParent)
	, nSolver((int)Model::m_Solver)
{

}

CDlgSolverParameters::~CDlgSolverParameters()
{
}

void CDlgSolverParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_SOLVER, nSolver);
	DDX_Text(pDX, IDC_EDIT_EPS, Model::m_eps);
	DDX_Control(pDX, IDC_EDIT_STEPS, edtSteps);
	DDX_Control(pDX, IDC_SPIN_STEPS, spnSteps);
	DDX_Text(pDX, IDC_EDIT_STEPS, Model::m_NumSteps);
	DDX_Control(pDX, IDC_COMBO_SOLVER, m_ComboSolvers);
}


BEGIN_MESSAGE_MAP(CDlgSolverParameters, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SOLVER, &CDlgSolverParameters::OnCbnSelchangeComboSolver)
END_MESSAGE_MAP()


// CDlgSolverParameters message handlers

BOOL CDlgSolverParameters::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	edtSteps.EnableWindow(nSolver != 0);
	spnSteps.EnableWindow(nSolver != 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSolverParameters::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::OnOK();

	Model::m_Solver = (BEM_3D::SOLVER)nSolver;
}

void CDlgSolverParameters::OnCbnSelchangeComboSolver()
{
	// TODO: Add your control notification handler code here
	edtSteps.EnableWindow(m_ComboSolvers.GetCurSel() != 0);
	spnSteps.EnableWindow(m_ComboSolvers.GetCurSel() != 0);
}
