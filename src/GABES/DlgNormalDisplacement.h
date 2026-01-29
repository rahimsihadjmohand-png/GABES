#pragma once
#include "afxdialogex.h"


// CDlgNormalDisplacement dialog

class CDlgNormalDisplacement : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNormalDisplacement)

public:
	CDlgNormalDisplacement(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgNormalDisplacement();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NORMAL_DISPLACEMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double Un;
};
