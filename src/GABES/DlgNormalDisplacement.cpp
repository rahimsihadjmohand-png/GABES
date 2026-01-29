// DlgNormalDisplacement.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgNormalDisplacement.h"


// CDlgNormalDisplacement dialog

IMPLEMENT_DYNAMIC(CDlgNormalDisplacement, CDialogEx)

CDlgNormalDisplacement::CDlgNormalDisplacement(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_NORMAL_DISPLACEMENT, pParent)
	, Un(0)
{

}

CDlgNormalDisplacement::~CDlgNormalDisplacement()
{
}

void CDlgNormalDisplacement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UN, Un);
}


BEGIN_MESSAGE_MAP(CDlgNormalDisplacement, CDialogEx)
END_MESSAGE_MAP()


// CDlgNormalDisplacement message handlers
