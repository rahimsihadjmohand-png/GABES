#pragma once
#include "afxdialogex.h"


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
	int nSolver;
	CEdit edtSteps;
	CSpinButtonCtrl spnSteps;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboSolver();
	CComboBox m_ComboSolvers;
};
