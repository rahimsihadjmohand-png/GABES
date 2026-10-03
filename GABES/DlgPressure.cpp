// DlgPressure.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgPressure.h"


// CDlgPressure dialog

IMPLEMENT_DYNAMIC(CDlgPressure, CDialogEx)

CDlgPressure::CDlgPressure(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_PRESSURE, pParent)
	, P(0)
{

}

CDlgPressure::~CDlgPressure()
{
}

void CDlgPressure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TN, P);
}


BEGIN_MESSAGE_MAP(CDlgPressure, CDialogEx)
END_MESSAGE_MAP()


// CDlgPressure message handlers
