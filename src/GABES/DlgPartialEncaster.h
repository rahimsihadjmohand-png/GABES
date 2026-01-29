#pragma once
#include "afxdialogex.h"


// CDlgPartialEncaster dialog

class CDlgPartialEncaster : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPartialEncaster)

public:
	CDlgPartialEncaster(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgPartialEncaster();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PARTIAL_ENCASTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL bEncasterUx;
	BOOL bEncasterUy;
	BOOL bEncasterUz;
};
