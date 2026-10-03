#pragma once
#include "afxdialogex.h"


// CDlgPressure dialog

class CDlgPressure : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPressure)

public:
	CDlgPressure(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgPressure();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PRESSURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double P;
};
