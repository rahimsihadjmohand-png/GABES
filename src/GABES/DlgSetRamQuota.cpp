// DlgSetRamQuota.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSetRamQuota.h"


// CDlgSetRamQuota dialog

IMPLEMENT_DYNAMIC(CDlgSetRamQuota, CDialogEx)

CDlgSetRamQuota::CDlgSetRamQuota(BEM_3D::Model& rModel, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SET_RAM_QUOTA, pParent)
	, m_rModel(rModel)
	, nTotalRam(0)
	, bAutoQuota(FALSE)
	, A_Quota(0)
{

}

CDlgSetRamQuota::~CDlgSetRamQuota()
{
}

void CDlgSetRamQuota::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOTAL_RAM, nTotalRam);
	DDX_Check(pDX, IDC_CHECK_AUTO_MATRIX_BAILS, bAutoQuota);
	DDX_Text(pDX, IDC_EDIT_A_MATRIX_BAIL, A_Quota);
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

	A_Quota = m_rModel.GetA_Quota();


	sldrAQuota.SetRange(0, nTotalRam, true);
	sldrAQuota.SetPos(A_Quota);


	GetDlgItem(IDC_STATIC_A_BAIL)->EnableWindow(!bAutoQuota);
	GetDlgItem(IDC_STATIC_A_GB)->EnableWindow(!bAutoQuota);
	GetDlgItem(IDC_EDIT_A_MATRIX_BAIL)->EnableWindow(!bAutoQuota);
	sldrAQuota.EnableWindow(!bAutoQuota);



	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetRamQuota::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar == (CScrollBar*)&sldrAQuota)
	{
		// This is a trackbar (slider), not a scrollbar
		A_Quota = sldrAQuota.GetPos();

		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgSetRamQuota::OnBnClickedCheckAutoMatrixQuotas()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_STATIC_A_BAIL)->EnableWindow(!bAutoQuota);
	GetDlgItem(IDC_STATIC_A_GB)->EnableWindow(!bAutoQuota);
	GetDlgItem(IDC_EDIT_A_MATRIX_BAIL)->EnableWindow(!bAutoQuota);
	sldrAQuota.EnableWindow(!bAutoQuota);
}
