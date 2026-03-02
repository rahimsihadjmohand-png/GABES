// DlgSubsetBCs.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSubsetBCs.h"


// CDlgSubsetBCs dialog

IMPLEMENT_DYNAMIC(CDlgSubsetBCs, CDialogEx)

CDlgSubsetBCs::CDlgSubsetBCs(BEM_3D::ElementSubSet& rSubset, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SUBSET_BCS, pParent)
	, m_rSubset(rSubset)
	, strSubsetName(rSubset.m_strName)
	, bNormalDisp(rSubset.m_bNormalDisplacement)
	, Un(rSubset.m_Un)
	, bFixedUx(rSubset.m_bFixed_X)
	, Ux(rSubset.U.x)
	, bFixedUy(rSubset.m_bFixed_Y)
	, Uy(rSubset.U.y)
	, bFixedUz(rSubset.m_bFixed_Z)
	, Uz(rSubset.U.z)
	, bNormalTrac(rSubset.m_bNormalTraction)
	, Tn(rSubset.m_Tn)
	, Tx(rSubset.T.x)
	, Ty(rSubset.T.y)
	, Tz(rSubset.T.z)
{

}

CDlgSubsetBCs::~CDlgSubsetBCs()
{

}

void CDlgSubsetBCs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, strSubsetName);
	DDX_Check(pDX, IDC_CHECK_UN, bNormalDisp);
	DDX_Text(pDX, IDC_EDIT_UN, Un);
	DDX_Check(pDX, IDC_CHECK_U1, bFixedUx);
	DDX_Text(pDX, IDC_EDIT_U1, Ux);
	DDX_Check(pDX, IDC_CHECK_U2, bFixedUy);
	DDX_Text(pDX, IDC_EDIT_U2, Uy);
	DDX_Check(pDX, IDC_CHECK_U3, bFixedUz);
	DDX_Text(pDX, IDC_EDIT_U3, Uz);
	DDX_Check(pDX, IDC_CHECK_TN, bNormalTrac);
	DDX_Text(pDX, IDC_EDIT_TN, Tn);
	DDX_Text(pDX, IDC_EDIT_T1, Tx);
	DDX_Text(pDX, IDC_EDIT_T2, Ty);
	DDX_Text(pDX, IDC_EDIT_T3, Tz);
}


BEGIN_MESSAGE_MAP(CDlgSubsetBCs, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_UN, &CDlgSubsetBCs::OnBnClickedCheckUn)
	ON_BN_CLICKED(IDC_CHECK_TN, &CDlgSubsetBCs::OnBnClickedCheckTn)
	ON_BN_CLICKED(IDC_CHECK_U1, &CDlgSubsetBCs::OnBnClickedCheckU1)
	ON_BN_CLICKED(IDC_CHECK_U2, &CDlgSubsetBCs::OnBnClickedCheckU2)
	ON_BN_CLICKED(IDC_CHECK_U3, &CDlgSubsetBCs::OnBnClickedCheckU3)
END_MESSAGE_MAP()


// CDlgSubsetBCs message handlers

void CDlgSubsetBCs::UpdateControlsState()
{
	GetDlgItem(IDC_EDIT_UN)->EnableWindow(bNormalDisp);
	GetDlgItem(IDC_EDIT_TN)->EnableWindow(bNormalTrac);

	if (bNormalDisp || bNormalTrac)
	{
		GetDlgItem(IDC_CHECK_U1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_U2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_U3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_U1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_U2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_U3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_T1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_T2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_T3)->EnableWindow(FALSE);
	}
	else
	{		
		GetDlgItem(IDC_CHECK_U1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_U2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_U3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_U1)->EnableWindow(bFixedUx);
		GetDlgItem(IDC_EDIT_U2)->EnableWindow(bFixedUy);
		GetDlgItem(IDC_EDIT_U3)->EnableWindow(bFixedUz);
		GetDlgItem(IDC_EDIT_T1)->EnableWindow(!bFixedUx);
		GetDlgItem(IDC_EDIT_T2)->EnableWindow(!bFixedUy);
		GetDlgItem(IDC_EDIT_T3)->EnableWindow(!bFixedUz);
	}
}

BOOL CDlgSubsetBCs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateControlsState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSubsetBCs::OnBnClickedCheckUn()
{
	// TODO: Add your control notification handler code here

	UpdateData(); // Controls to variables

	// if bNormalDisplacement is true make the bNormalTraction false
	if (bNormalDisp)
		bNormalTrac = FALSE;

	UpdateData(FALSE); // Variables to controls

	UpdateControlsState();
}

void CDlgSubsetBCs::OnBnClickedCheckTn()
{
	// TODO: Add your control notification handler code here

	UpdateData(); // Controls to variables

	// if bNormalTraction is true make the bNormalDisplacement false
	if (bNormalTrac)
		bNormalDisp = FALSE;

	UpdateData(FALSE); // Variables to controls

	UpdateControlsState();
}

void CDlgSubsetBCs::OnBnClickedCheckU1()
{
	// TODO: Add your control notification handler code here

	UpdateData(); // Controls to variables

	UpdateControlsState();
}

void CDlgSubsetBCs::OnBnClickedCheckU2()
{
	// TODO: Add your control notification handler code here

	UpdateData(); // Controls to variables

	UpdateControlsState();
}

void CDlgSubsetBCs::OnBnClickedCheckU3()
{
	// TODO: Add your control notification handler code here

	UpdateData(); // Controls to variables

	UpdateControlsState();
}

void CDlgSubsetBCs::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::OnOK();

	m_rSubset.m_strName = strSubsetName;
	m_rSubset.m_bNormalDisplacement = bNormalDisp;
	m_rSubset.m_Un = Un;
	m_rSubset.m_bNormalTraction = bNormalTrac;
	m_rSubset.m_Tn = Tn;
	m_rSubset.m_bFixed_X = bFixedUx;
	m_rSubset.m_bFixed_Y = bFixedUy;
	m_rSubset.m_bFixed_Z = bFixedUz;
	m_rSubset.U.Set(Ux, Uy, Uz);
	m_rSubset.T.Set(Tx, Ty, Tz);
}
