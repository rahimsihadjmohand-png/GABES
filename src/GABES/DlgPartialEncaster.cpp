// DlgPartialEncaster.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgPartialEncaster.h"


// CDlgPartialEncaster dialog

IMPLEMENT_DYNAMIC(CDlgPartialEncaster, CDialogEx)

CDlgPartialEncaster::CDlgPartialEncaster(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_PARTIAL_ENCASTER, pParent)
	, bEncasterUx(FALSE)
	, bEncasterUy(FALSE)
	, bEncasterUz(FALSE)
{

}

CDlgPartialEncaster::~CDlgPartialEncaster()
{
}

void CDlgPartialEncaster::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ENCASTER_UX, bEncasterUx);
	DDX_Check(pDX, IDC_CHECK_ENCASTER_UY, bEncasterUy);
	DDX_Check(pDX, IDC_CHECK_ENCASTER_UZ, bEncasterUz);
}


BEGIN_MESSAGE_MAP(CDlgPartialEncaster, CDialogEx)
END_MESSAGE_MAP()


// CDlgPartialEncaster message handlers
