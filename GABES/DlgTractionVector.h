#pragma once
#include "afxdialogex.h"


// CDlgTractionVector dialog

class CDlgTractionVector : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTractionVector)

public:
	CDlgTractionVector(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgTractionVector();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TRACTION_VECTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton chkTx;
	CButton chkTy;
	CButton chkTz;
	CEdit editTx;
	CEdit editTy;
	CEdit editTz;
	BOOL bSetTx;
	BOOL bSetTy;
	BOOL bSetTz;
	double Tx;
	double Ty;
	double Tz;
	afx_msg void OnBnClickedCheckUx();
	afx_msg void OnBnClickedCheckUy();
	afx_msg void OnBnClickedCheckUz();
};
