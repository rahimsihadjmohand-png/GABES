// DlgSolverParameters.cpp : implementation file
//
#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "Model.h"
#include "DlgSolverParameters.h"
#include "DlgHMatrixViewer.h"


using BEM_3D::SOLVER;
using BEM_3D::Model;


// CDlgSolverParameters dialog

IMPLEMENT_DYNAMIC(CDlgSolverParameters, CDialogEx)

CDlgSolverParameters::CDlgSolverParameters(const Model* pModel, CWnd* pParent)
	: CDialogEx(IDD_DLG_SOLVER_PARAMETERS, pParent)
	, m_pModel(pModel)
{

}

CDlgSolverParameters::~CDlgSolverParameters()
{
	
}

void CDlgSolverParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EPS, Model::m_accur);
	DDX_Control(pDX, IDC_EDIT_STEPS, edt_steps);
	DDX_Control(pDX, IDC_SPIN_STEPS, spn_steps);
	DDX_Check(pDX, IDC_CHECK_SET_MAX_STEPS, Model::m_bSetMaxIter);
	DDX_Text(pDX, IDC_EDIT_STEPS, Model::m_maxiter);
	DDX_Control(pDX, IDC_COMBO_SOLVER, combo_solvers);
	DDX_Text(pDX, IDC_EDIT_CLF, Model::m_clf);
	DDX_Control(pDX, IDC_SPIN_CLF, spn_clf);
	DDX_Text(pDX, IDC_EDIT_ETA, Model::m_Eta);
	DDX_CBIndex(pDX, IDC_COMBO_CLMODE, Model::m_clmode);

	if (pDX->m_bSaveAndValidate)
	{
		int nSolver = -1;
		DDX_CBIndex(pDX, IDC_COMBO_SOLVER, nSolver);
		Model::m_solver = (BEM_3D::SOLVER)nSolver;
	}
	else
	{
		int nSolver = (int)Model::m_solver;
		DDX_CBIndex(pDX, IDC_COMBO_SOLVER, nSolver);
	}

}


BEGIN_MESSAGE_MAP(CDlgSolverParameters, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SOLVER, &CDlgSolverParameters::OnCbnSelchangeComboSolver)
	ON_BN_CLICKED(IDC_CHECK_SET_MAX_STEPS, &CDlgSolverParameters::OnBnClickedCheckSetMaxSteps)
END_MESSAGE_MAP()


// CDlgSolverParameters message handlers

BOOL CDlgSolverParameters::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	edt_steps.EnableWindow(Model::m_bSetMaxIter && Model::m_solver == SOLVER::GMRES);
	spn_steps.EnableWindow(Model::m_bSetMaxIter && Model::m_solver == SOLVER::GMRES);

	spn_steps.SetRange(1, SHORT_MAX);
	spn_clf.SetRange(1, SHORT_MAX);


	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSolverParameters::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::OnOK();

	//Model::m_Solver = (BEM_3D::SOLVER)nSolver;
}

void CDlgSolverParameters::OnCbnSelchangeComboSolver()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	edt_steps.EnableWindow(Model::m_bSetMaxIter && Model::m_solver == SOLVER::GMRES);
	spn_steps.EnableWindow(Model::m_bSetMaxIter && Model::m_solver == SOLVER::GMRES);
}


void CDlgSolverParameters::OnBnClickedCheckSetMaxSteps()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	edt_steps.EnableWindow(Model::m_bSetMaxIter && Model::m_solver == SOLVER::GMRES);
	spn_steps.EnableWindow(Model::m_bSetMaxIter && Model::m_solver == SOLVER::GMRES);
}
