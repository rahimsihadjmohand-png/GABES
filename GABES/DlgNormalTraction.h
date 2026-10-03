#pragma once
#include "afxdialogex.h"


// CDlgNormalTraction dialog

class CDlgNormalTraction : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNormalTraction)

public:
	CDlgNormalTraction(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgNormalTraction();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NORMAL_TRACTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double Tn;
};
