// CDlgDisplacementVector.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgDisplacementVector.h"


// CDlgDisplacementVector dialog

IMPLEMENT_DYNAMIC(CDlgDisplacementVector, CDialogEx)

CDlgDisplacementVector::CDlgDisplacementVector(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_DISPLACEMENT_VECTOR, pParent)
	, bSetUx(FALSE)
	, bSetUy(FALSE)
	, bSetUz(FALSE)
	, Ux(0)
	, Uy(0)
	, Uz(0)
{

}

CDlgDisplacementVector::~CDlgDisplacementVector()
{
}

void CDlgDisplacementVector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_UX, bSetUx);
	DDX_Check(pDX, IDC_CHECK_UY, bSetUy);
	DDX_Check(pDX, IDC_CHECK_UZ, bSetUz);
	DDX_Text(pDX, IDC_EDIT_UX, Ux);
	DDX_Text(pDX, IDC_EDIT_UY, Uy);
	DDX_Text(pDX, IDC_EDIT_UZ, Uz);
	DDX_Control(pDX, IDC_EDIT_UX, editUx);
	DDX_Control(pDX, IDC_EDIT_UY, editUy);
	DDX_Control(pDX, IDC_EDIT_UZ, editUz);
	DDX_Control(pDX, IDC_CHECK_UX, chkUx);
	DDX_Control(pDX, IDC_CHECK_UY, chkUy);
	DDX_Control(pDX, IDC_CHECK_UZ, chkUz);


	editUx.EnableWindow(chkUx.GetCheck() == BST_CHECKED);
	editUy.EnableWindow(chkUy.GetCheck() == BST_CHECKED);
	editUz.EnableWindow(chkUz.GetCheck() == BST_CHECKED);
}


BEGIN_MESSAGE_MAP(CDlgDisplacementVector, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_UX, &CDlgDisplacementVector::OnBnClickedCheckUx)
	ON_BN_CLICKED(IDC_CHECK_UY, &CDlgDisplacementVector::OnBnClickedCheckUy)
	ON_BN_CLICKED(IDC_CHECK_UZ, &CDlgDisplacementVector::OnBnClickedCheckUz)
END_MESSAGE_MAP()


// CDlgDisplacementVector message handlers

void CDlgDisplacementVector::OnBnClickedCheckUx()
{
	// TODO: Add your control notification handler code here
	editUx.EnableWindow(chkUx.GetCheck() == BST_CHECKED);
	
}

void CDlgDisplacementVector::OnBnClickedCheckUy()
{
	// TODO: Add your control notification handler code here
	editUy.EnableWindow(chkUy.GetCheck() == BST_CHECKED);
}

void CDlgDisplacementVector::OnBnClickedCheckUz()
{
	// TODO: Add your control notification handler code here
	editUz.EnableWindow(chkUz.GetCheck() == BST_CHECKED);

}
