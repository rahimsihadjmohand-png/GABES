// DlgNormalTraction.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgNormalTraction.h"


// CDlgNormalTraction dialog

IMPLEMENT_DYNAMIC(CDlgNormalTraction, CDialogEx)

CDlgNormalTraction::CDlgNormalTraction(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_NORMAL_TRACTION, pParent)
	, Tn(0)
{

}

CDlgNormalTraction::~CDlgNormalTraction()
{
}

void CDlgNormalTraction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TN, Tn);
}


BEGIN_MESSAGE_MAP(CDlgNormalTraction, CDialogEx)
END_MESSAGE_MAP()


// CDlgNormalTraction message handlers
