// CDlgPaletteSettings.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgPaletteSettings.h"
#include "PaletteCtrl.h"


// CDlgPaletteSettings dialog

IMPLEMENT_DYNAMIC(CDlgPaletteSettings, CDialogEx)

CDlgPaletteSettings::CDlgPaletteSettings(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_PALETTE_SETTINGS, pParent)
	, nFormat(CPaletteCtrl::m_nFmt)
	, nNumDecimals(CPaletteCtrl::m_nNumDecimals)
{

}

CDlgPaletteSettings::~CDlgPaletteSettings()
{
}

void CDlgPaletteSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_NUMBER_FORMAT, nFormat);
	DDX_Text(pDX, IDC_EDIT_NUM_DECIMALS, nNumDecimals);
	DDX_Control(pDX, IDC_SPIN1, spinCtrl);
}


BEGIN_MESSAGE_MAP(CDlgPaletteSettings, CDialogEx)
END_MESSAGE_MAP()


// CDlgPaletteSettings message handlers

BOOL CDlgPaletteSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	spinCtrl.SetRange(0, 10);
	spinCtrl.SetPos(CPaletteCtrl::m_nNumDecimals);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPaletteSettings::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::OnOK();

	CPaletteCtrl::m_nFmt = nFormat;
	CPaletteCtrl::m_nNumDecimals = nNumDecimals;
}
