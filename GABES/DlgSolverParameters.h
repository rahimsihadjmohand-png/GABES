#pragma once
#include "afxdialogex.h"
#include "HMatrixViewer.h"



// CDlgSolverParameters dialog

class CDlgSolverParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSolverParameters)

public:
	CDlgSolverParameters(const BEM_3D::Model* pModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSolverParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SOLVER_PARAMETERS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	const BEM_3D::Model* m_pModel;


	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboSolver();

	CEdit edt_steps;
	CSpinButtonCtrl spn_steps;
	CComboBox combo_solvers;
	CSpinButtonCtrl spn_clf;


	afx_msg void OnBnClickedCheckSetMaxSteps();
};
