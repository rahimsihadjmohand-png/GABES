// DlgSetRamQuota.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSetRamQuota.h"


// CDlgSetRamQuota dialog

IMPLEMENT_DYNAMIC(CDlgSetRamQuota, CDialogEx)

CDlgSetRamQuota::CDlgSetRamQuota(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SET_RAM_BAIL, pParent)
	, nTotalRam(0)
	, bAutoQuotas(FALSE)
	, Q_Quota(0)
	, R_Quota(0)
	, A_Quota(0)
{

}

CDlgSetRamQuota::~CDlgSetRamQuota()
{
}

void CDlgSetRamQuota::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RAM_BAIL, RAM_Quota);
	DDX_Text(pDX, IDC_EDIT_TOTAL_RAM, nTotalRam);
	DDX_Control(pDX, IDC_SLIDER_RAM_BAIL, sldrRamQuota);
	DDX_Check(pDX, IDC_CHECK_AUTO_MATRIX_BAILS, bAutoQuotas);
	DDX_Text(pDX, IDC_EDIT_Q_MATRIX_BAIL, Q_Quota);
	DDX_Text(pDX, IDC_EDIT_R_MATRIX_BAIL, R_Quota);
	DDX_Text(pDX, IDC_EDIT_A_MATRIX_BAIL, A_Quota);
	DDX_Control(pDX, IDC_SLIDER_Q_MATRIX_BAIL, sldrQQuota);
	DDX_Control(pDX, IDC_SLIDER_R_MATRIX_BAIL, sldrRQuota);
	DDX_Control(pDX, IDC_SLIDER_A_MATRIX_BAIL, sldrAQuota);
}


BEGIN_MESSAGE_MAP(CDlgSetRamQuota, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_AUTO_MATRIX_BAILS, &CDlgSetRamQuota::OnBnClickedCheckAutoMatrixQuotas)
END_MESSAGE_MAP()


// CDlgSetRamQuota message handlers

BOOL CDlgSetRamQuota::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// Get the available Memory
	MEMORYSTATUSEX mem;
	mem.dwLength = sizeof(mem);
	GlobalMemoryStatusEx(&mem);
	nTotalRam = mem.ullTotalPhys / (1024UL * 1024UL * 1024UL);
	Q_Quota = RAM_Quota / 2;
	R_Quota = RAM_Quota / 4;
	A_Quota = RAM_Quota / 4;

	sldrRamQuota.SetRange(0, nTotalRam, true);
	sldrRamQuota.SetPos(RAM_Quota);

	sldrQQuota.SetRange(0, RAM_Quota, true);
	sldrQQuota.SetPos(Q_Quota);

	sldrRQuota.SetRange(0, RAM_Quota, true);
	sldrRQuota.SetPos(R_Quota);

	sldrAQuota.SetRange(0, RAM_Quota, true);
	sldrAQuota.SetPos(A_Quota);


	GetDlgItem(IDC_STATIC_Q_BAIL)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_STATIC_Q_GB)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_EDIT_Q_MATRIX_BAIL)->EnableWindow(!bAutoQuotas);
	sldrQQuota.EnableWindow(!bAutoQuotas);

	GetDlgItem(IDC_STATIC_R_BAIL)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_STATIC_R_GB)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_EDIT_R_MATRIX_BAIL)->EnableWindow(!bAutoQuotas);
	sldrRQuota.EnableWindow(!bAutoQuotas);

	GetDlgItem(IDC_STATIC_A_BAIL)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_STATIC_A_GB)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_EDIT_A_MATRIX_BAIL)->EnableWindow(!bAutoQuotas);
	sldrAQuota.EnableWindow(!bAutoQuotas);



	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetRamQuota::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar == (CScrollBar*)&sldrRamQuota)
	{
		// This is a trackbar (slider), not a scrollbar
		RAM_Quota = sldrRamQuota.GetPos();

		if (!bAutoQuotas)
		{
			int nRemQuota = RAM_Quota;
			A_Quota = min(A_Quota, nRemQuota);
			nRemQuota -= A_Quota;

			R_Quota = min(R_Quota, nRemQuota);
			nRemQuota -= R_Quota;

			Q_Quota = min(Q_Quota, nRemQuota);



			sldrQQuota.SetRange(0, RAM_Quota, true);
			sldrQQuota.SetPos(Q_Quota);

			sldrRQuota.SetRange(0, RAM_Quota, true);
			sldrRQuota.SetPos(R_Quota);

			sldrAQuota.SetRange(0, RAM_Quota, true);
			sldrAQuota.SetPos(A_Quota);
		}

		UpdateData(FALSE);
	}
	else if (pScrollBar == (CScrollBar*)&sldrQQuota)
	{
		// This is a trackbar (slider), not a scrollbar
		Q_Quota = sldrQQuota.GetPos();

		int nRemQuota = RAM_Quota - Q_Quota; 

		// A Is Favoured
		A_Quota = min(A_Quota, nRemQuota);
		nRemQuota -= A_Quota;
		R_Quota = min(R_Quota, nRemQuota);

		sldrRQuota.SetPos(R_Quota);
		sldrAQuota.SetPos(A_Quota);


		UpdateData(FALSE);
	}
	else if (pScrollBar == (CScrollBar*)&sldrRQuota)
	{
		// This is a trackbar (slider), not a scrollbar
		R_Quota = sldrRQuota.GetPos();

		int nRemQuota = RAM_Quota - R_Quota;
		
		// A is favoured 
		A_Quota = min(A_Quota, nRemQuota);
		nRemQuota -= A_Quota;
		Q_Quota = min(Q_Quota, nRemQuota);

		sldrQQuota.SetPos(Q_Quota);
		sldrAQuota.SetPos(A_Quota);
		UpdateData(FALSE);
	}
	else if (pScrollBar == (CScrollBar*)&sldrAQuota)
	{
		// This is a trackbar (slider), not a scrollbar
		A_Quota = sldrAQuota.GetPos();

		// R is favoured
		int nRemQuota = RAM_Quota - A_Quota;
		R_Quota = min(R_Quota, nRemQuota);
		nRemQuota -= R_Quota;
		Q_Quota = min(Q_Quota, nRemQuota);

		sldrQQuota.SetPos(Q_Quota);
		sldrRQuota.SetPos(R_Quota);
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgSetRamQuota::OnBnClickedCheckAutoMatrixQuotas()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_STATIC_Q_BAIL)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_STATIC_Q_GB)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_EDIT_Q_MATRIX_BAIL)->EnableWindow(!bAutoQuotas);
	sldrQQuota.EnableWindow(!bAutoQuotas);

	GetDlgItem(IDC_STATIC_R_BAIL)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_STATIC_R_GB)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_EDIT_R_MATRIX_BAIL)->EnableWindow(!bAutoQuotas);
	sldrRQuota.EnableWindow(!bAutoQuotas);

	GetDlgItem(IDC_STATIC_A_BAIL)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_STATIC_A_GB)->EnableWindow(!bAutoQuotas);
	GetDlgItem(IDC_EDIT_A_MATRIX_BAIL)->EnableWindow(!bAutoQuotas);
	sldrAQuota.EnableWindow(!bAutoQuotas);
}
