#pragma once
#include "afxdialogex.h"
#include "Model.h"



// CDlgSolverParameters dialog

class CDlgSolverParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSolverParameters)

public:
	CDlgSolverParameters(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSolverParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SOLVER_PARAMETERS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboSolver();

	CComboBox combo_solvers;
	CEdit edt_HLU_tol;
	CButton chk_SetMaxIter;
	CEdit edt_steps;	
	CSpinButtonCtrl spn_steps;
	CEdit edt_GMRES_tol;
	CEdit edt_Precond_tol;


	afx_msg void OnBnClickedCheckSetMaxSteps();	
};
