#pragma once
#include "afxdialogex.h"


// CDlgPaletteSettings dialog

class CDlgPaletteSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPaletteSettings)

public:
	CDlgPaletteSettings(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgPaletteSettings();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PALETTE_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int nFormat;
	int nNumDecimals;
	CSpinButtonCtrl spinCtrl;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
