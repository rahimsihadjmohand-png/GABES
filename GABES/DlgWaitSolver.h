#pragma once
#include "afxdialogex.h"
#include "Model.h"
#include "Solver_H2LIB.h"
// CDlgWaitSolver dialog



class CDlgWaitSolver : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWaitSolver)

public:
	CDlgWaitSolver(BEM_3D::Model* _pModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgWaitSolver();

	void StartSolver(); // Does the same thing as OnInitDialog()


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_WAIT_SOLVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void FormatTime();
	LARGE_INTEGER ticksStart, ticksFreq;
	LARGE_INTEGER ticksHMatStart, ticksSolverStart;

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl PrgsAdvance;
	
	CString strTime;
	CButton btnOk;

	SolverThreadData m_STHD;
	CString str_RHS_time;
	CString str_HMAT_time;
	CString str_SOLV_time;


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnRHSVectorConstructed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHMatrixConstructed(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnOK();

};
