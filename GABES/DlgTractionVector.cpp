// CDlgTractionVector.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgTractionVector.h"


// CDlgTractionVector dialog

IMPLEMENT_DYNAMIC(CDlgTractionVector, CDialogEx)

CDlgTractionVector::CDlgTractionVector(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_TRACTION_VECTOR, pParent)
	, bSetTx(FALSE)
	, bSetTy(FALSE)
	, bSetTz(FALSE)
	, Tx(0)
	, Ty(0)
	, Tz(0)
{

}

CDlgTractionVector::~CDlgTractionVector()
{
}

void CDlgTractionVector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_UX, chkTx);
	DDX_Control(pDX, IDC_CHECK_UY, chkTy);
	DDX_Control(pDX, IDC_CHECK_UZ, chkTz);
	DDX_Control(pDX, IDC_EDIT_TX, editTx);
	DDX_Control(pDX, IDC_EDIT_TY, editTy);
	DDX_Control(pDX, IDC_EDIT_TZ, editTz);
	DDX_Check(pDX, IDC_CHECK_UX, bSetTx);
	DDX_Check(pDX, IDC_CHECK_UY, bSetTy);
	DDX_Check(pDX, IDC_CHECK_UZ, bSetTz);
	DDX_Text(pDX, IDC_EDIT_TX, Tx);
	DDX_Text(pDX, IDC_EDIT_TY, Ty);
	DDX_Text(pDX, IDC_EDIT_TZ, Tz);

	editTx.EnableWindow(chkTx.GetCheck() == BST_CHECKED);
	editTy.EnableWindow(chkTy.GetCheck() == BST_CHECKED);
	editTz.EnableWindow(chkTz.GetCheck() == BST_CHECKED);
}


BEGIN_MESSAGE_MAP(CDlgTractionVector, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_UX, &CDlgTractionVector::OnBnClickedCheckUx)
	ON_BN_CLICKED(IDC_CHECK_UY, &CDlgTractionVector::OnBnClickedCheckUy)
	ON_BN_CLICKED(IDC_CHECK_UZ, &CDlgTractionVector::OnBnClickedCheckUz)
END_MESSAGE_MAP()


// CDlgTractionVector message handlers

void CDlgTractionVector::OnBnClickedCheckUx()
{
	// TODO: Add your control notification handler code here
	editTx.EnableWindow(chkTx.GetCheck() == BST_CHECKED);
}

void CDlgTractionVector::OnBnClickedCheckUy()
{
	// TODO: Add your control notification handler code here
	editTy.EnableWindow(chkTy.GetCheck() == BST_CHECKED);
}

void CDlgTractionVector::OnBnClickedCheckUz()
{
	// TODO: Add your control notification handler code here
	editTz.EnableWindow(chkTz.GetCheck() == BST_CHECKED);
}
