#pragma once
#include "afxdialogex.h"


// CDlgDisplacementVector dialog

class CDlgDisplacementVector : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDisplacementVector)

public:
	CDlgDisplacementVector(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgDisplacementVector();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DISPLACEMENT_VECTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL bSetUx;
	BOOL bSetUy;
	BOOL bSetUz;
	double Ux;
	double Uy;
	double Uz;
	CEdit editUx;
	CEdit editUy;
	CEdit editUz;
	afx_msg void OnBnClickedCheckUx();
	CButton chkUx;
	CButton chkUy;
	CButton chkUz;
	afx_msg void OnBnClickedCheckUy();
	afx_msg void OnBnClickedCheckUz();
};
