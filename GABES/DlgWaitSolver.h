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

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl PrgsAdvance;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
	CString strTime;
	CButton btnOk;

	SolverThreadData m_STHD;
	virtual void OnOK();
};
