// DlgDomainNodeResponse.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgDomainNodeResponse.h"


// CDlgDomainNodeResponse dialog

IMPLEMENT_DYNAMIC(CDlgDomainNodeResponse, CDialogEx)

CDlgDomainNodeResponse::CDlgDomainNodeResponse(BEM_3D::Model* pModel, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_DOMAIN_NODE_RESPONSE, pParent)
	, m_pModel(pModel)
	, x(0)
	, y(0)
	, z(0)

	, U1(0)
	, U2(0)
	, U3(0)
	, U(0)
	
	, Sig_11(0)
	, Sig_22(0)
	, Sig_33(0)
	, Sig_12(0)
	, Sig_23(0)
	, Sig_31(0)
	, Sig_VM(0)

	, Eps_11(0)
	, Eps_22(0)
	, Eps_33(0)
	, Eps_12(0)
	, Eps_23(0)
	, Eps_31(0)
	
	, bU1(FALSE)
	, bU2(FALSE)
	, bU3(FALSE)
	, bU(FALSE)
	
	, bSig_11(FALSE)
	, bSig_22(FALSE)
	, bSig_33(FALSE)
	, bSig_12(FALSE)
	, bSig_23(FALSE)
	, bSig_31(FALSE)
	
	, bEps_11(FALSE)
	, bEps_22(FALSE)
	, bEps_33(FALSE)
	, bEps_12(FALSE)
	, bEps_23(FALSE)
	, bEps_31(FALSE)
	, bSig_VM(FALSE)
{

}

CDlgDomainNodeResponse::~CDlgDomainNodeResponse()
{
}

void CDlgDomainNodeResponse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	if (pDX->m_bSaveAndValidate)
	{
		// ========= Edit → variables =========
		DDX_Text(pDX, IDC_EDIT_X, x);
		DDX_Text(pDX, IDC_EDIT_Y, y);
		DDX_Text(pDX, IDC_EDIT_Z, z);

		DDX_Text(pDX, IDC_EDIT_U1, U1);
		DDX_Text(pDX, IDC_EDIT_U2, U2);
		DDX_Text(pDX, IDC_EDIT_U3, U3);
		DDX_Text(pDX, IDC_EDIT_U, U);

		DDX_Text(pDX, IDC_EDIT_SIG11, Sig_11);
		DDX_Text(pDX, IDC_EDIT_SIG22, Sig_22);
		DDX_Text(pDX, IDC_EDIT_SIG33, Sig_33);
		DDX_Text(pDX, IDC_EDIT_SIG12, Sig_12);
		DDX_Text(pDX, IDC_EDIT_SIG23, Sig_23);
		DDX_Text(pDX, IDC_EDIT_SIG31, Sig_31);
		DDX_Text(pDX, IDC_EDIT_SIG_VON_MISES, Sig_VM);

		DDX_Text(pDX, IDC_EDIT_EPS11, Eps_11);
		DDX_Text(pDX, IDC_EDIT_EPS22, Eps_22);
		DDX_Text(pDX, IDC_EDIT_EPS33, Eps_33);
		DDX_Text(pDX, IDC_EDIT_EPS12, Eps_12);
		DDX_Text(pDX, IDC_EDIT_EPS23, Eps_23);
		DDX_Text(pDX, IDC_EDIT_EPS31, Eps_31);
	}
	else
	{
		// ========= Variables → edit (FORMATTED) =========
		CString s;

		auto F = [&](int id, double v)
			{
				s.Format(_T("%.6f"), v);   // <-- choose your format here
				SetDlgItemText(id, s);
			};

		F(IDC_EDIT_X, x);
		F(IDC_EDIT_Y, y);
		F(IDC_EDIT_Z, z);

		F(IDC_EDIT_U1, U1);
		F(IDC_EDIT_U2, U2);
		F(IDC_EDIT_U3, U3);
		F(IDC_EDIT_U, U);

		F(IDC_EDIT_SIG11, Sig_11);
		F(IDC_EDIT_SIG22, Sig_22);
		F(IDC_EDIT_SIG33, Sig_33);
		F(IDC_EDIT_SIG12, Sig_12);
		F(IDC_EDIT_SIG23, Sig_23);
		F(IDC_EDIT_SIG31, Sig_31);
		F(IDC_EDIT_SIG_VON_MISES, Sig_VM);

		F(IDC_EDIT_EPS11, Eps_11);
		F(IDC_EDIT_EPS22, Eps_22);
		F(IDC_EDIT_EPS33, Eps_33);
		F(IDC_EDIT_EPS12, Eps_12);
		F(IDC_EDIT_EPS23, Eps_23);
		F(IDC_EDIT_EPS31, Eps_31);
	}

	// ========= Checkboxes (no formatting needed) =========
	DDX_Check(pDX, IDC_CHECK_U1, bU1);
	DDX_Check(pDX, IDC_CHECK_U2, bU2);
	DDX_Check(pDX, IDC_CHECK_U3, bU3);
	DDX_Check(pDX, IDC_CHECK_U, bU);

	DDX_Check(pDX, IDC_CHECK_SIG11, bSig_11);
	DDX_Check(pDX, IDC_CHECK_SIG22, bSig_22);
	DDX_Check(pDX, IDC_CHECK_SIG33, bSig_33);
	DDX_Check(pDX, IDC_CHECK_SIG12, bSig_12);
	DDX_Check(pDX, IDC_CHECK_SIG23, bSig_23);
	DDX_Check(pDX, IDC_CHECK_SIG31, bSig_31);
	DDX_Check(pDX, IDC_CHECK_SIG_VON_MISES, bSig_VM);

	DDX_Check(pDX, IDC_CHECK_EPS11, bEps_11);
	DDX_Check(pDX, IDC_CHECK_EPS22, bEps_22);
	DDX_Check(pDX, IDC_CHECK_EPS33, bEps_33);
	DDX_Check(pDX, IDC_CHECK_EPS12, bEps_12);
	DDX_Check(pDX, IDC_CHECK_EPS23, bEps_23);
	DDX_Check(pDX, IDC_CHECK_EPS31, bEps_31);


	DDX_Control(pDX, IDC_CHECK_U1, CheckU1);
	DDX_Control(pDX, IDC_CHECK_U2, CheckU2);
	DDX_Control(pDX, IDC_CHECK_U3, CheckU3);
	DDX_Control(pDX, IDC_CHECK_U, CheckU);

	DDX_Control(pDX, IDC_CHECK_SIG11, CheckSig_11);
	DDX_Control(pDX, IDC_CHECK_SIG22, CheckSig_22);
	DDX_Control(pDX, IDC_CHECK_SIG33, CheckSig_33);
	DDX_Control(pDX, IDC_CHECK_SIG12, CheckSig_12);
	DDX_Control(pDX, IDC_CHECK_SIG23, CheckSig_23);
	DDX_Control(pDX, IDC_CHECK_SIG31, CheckSig_31);
	DDX_Control(pDX, IDC_CHECK_SIG_VON_MISES, CheckSig_VM);

	DDX_Control(pDX, IDC_CHECK_EPS11, CheckEps_11);
	DDX_Control(pDX, IDC_CHECK_EPS22, CheckEps_22);
	DDX_Control(pDX, IDC_CHECK_EPS33, CheckEps_33);
	DDX_Control(pDX, IDC_CHECK_EPS12, CheckEps_12);
	DDX_Control(pDX, IDC_CHECK_EPS23, CheckEps_23);
	DDX_Control(pDX, IDC_CHECK_EPS31, CheckEps_31);

	DDX_Control(pDX, IDC_EDIT_U1, EditU1);
	DDX_Control(pDX, IDC_EDIT_U2, EditU2);
	DDX_Control(pDX, IDC_EDIT_U3, EditU3);
	DDX_Control(pDX, IDC_EDIT_U, EditU);

	DDX_Control(pDX, IDC_EDIT_SIG11, EditSig_11);
	DDX_Control(pDX, IDC_EDIT_SIG22, EditSig_22);
	DDX_Control(pDX, IDC_EDIT_SIG33, EditSig_33);
	DDX_Control(pDX, IDC_EDIT_SIG12, EditSig_12);
	DDX_Control(pDX, IDC_EDIT_SIG23, EditSig_23);
	DDX_Control(pDX, IDC_EDIT_SIG31, EditSig_31);
	DDX_Control(pDX, IDC_EDIT_SIG_VON_MISES, EditSig_VM);

	DDX_Control(pDX, IDC_EDIT_EPS11, EditEps_11);
	DDX_Control(pDX, IDC_EDIT_EPS22, EditEps_22);
	DDX_Control(pDX, IDC_EDIT_EPS33, EditEps_33);
	DDX_Control(pDX, IDC_EDIT_EPS12, EditEps_12);
	DDX_Control(pDX, IDC_EDIT_EPS23, EditEps_23);
	DDX_Control(pDX, IDC_EDIT_EPS31, EditEps_31);
}



BEGIN_MESSAGE_MAP(CDlgDomainNodeResponse, CDialogEx)
	ON_BN_CLICKED(ID_CALCULATE, &CDlgDomainNodeResponse::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_CHECK_U1, &CDlgDomainNodeResponse::OnBnClickedCheckU1)
	ON_BN_CLICKED(IDC_CHECK_U2, &CDlgDomainNodeResponse::OnBnClickedCheckU2)
	ON_BN_CLICKED(IDC_CHECK_U3, &CDlgDomainNodeResponse::OnBnClickedCheckU3)
	ON_BN_CLICKED(IDC_CHECK_U, &CDlgDomainNodeResponse::OnBnClickedCheckU)
	ON_BN_CLICKED(IDC_CHECK_SIG11, &CDlgDomainNodeResponse::OnBnClickedCheckSig11)
	ON_BN_CLICKED(IDC_CHECK_SIG22, &CDlgDomainNodeResponse::OnBnClickedCheckSig22)
	ON_BN_CLICKED(IDC_CHECK_SIG33, &CDlgDomainNodeResponse::OnBnClickedCheckSig33)
	ON_BN_CLICKED(IDC_CHECK_SIG12, &CDlgDomainNodeResponse::OnBnClickedCheckSig12)
	ON_BN_CLICKED(IDC_CHECK_SIG23, &CDlgDomainNodeResponse::OnBnClickedCheckSig23)
	ON_BN_CLICKED(IDC_CHECK_SIG31, &CDlgDomainNodeResponse::OnBnClickedCheckSig31)
	ON_BN_CLICKED(IDC_CHECK_SIG_VON_MISES, &CDlgDomainNodeResponse::OnBnClickedCheckSigVonMises)
	ON_BN_CLICKED(IDC_CHECK_EPS11, &CDlgDomainNodeResponse::OnBnClickedCheckEps11)
	ON_BN_CLICKED(IDC_CHECK_EPS22, &CDlgDomainNodeResponse::OnBnClickedCheckEps22)
	ON_BN_CLICKED(IDC_CHECK_EPS33, &CDlgDomainNodeResponse::OnBnClickedCheckEps33)
	ON_BN_CLICKED(IDC_CHECK_EPS12, &CDlgDomainNodeResponse::OnBnClickedCheckEps12)
	ON_BN_CLICKED(IDC_CHECK_EPS23, &CDlgDomainNodeResponse::OnBnClickedCheckEps23)
	ON_BN_CLICKED(IDC_CHECK_EPS31, &CDlgDomainNodeResponse::OnBnClickedCheckEps31)
END_MESSAGE_MAP()


// CDlgDomainNodeResponse message handlers

void CDlgDomainNodeResponse::OnBnClickedCalculate()
{
	// TODO: Add your control notification handler code here
	// get the Domain node
	UpdateData();

	U1 = U2 = U3 = U = Sig_11 = Sig_22 = Sig_33 = Sig_12 = Sig_23 = Sig_31 = Sig_VM = Eps_11 = Eps_22 = Eps_33 = Eps_12 = Eps_23 = Eps_31 = 0.0;


	BEM_3D::Vertex DomainPt(x, y, z);


	// Displacements ===================================================
	if (bU1)
		U1 = m_pModel->GetDomainDisplacement(DomainPt, 0);
	if (bU2)
		U2 = m_pModel->GetDomainDisplacement(DomainPt, 1);
	if (bU3)
		U3 = m_pModel->GetDomainDisplacement(DomainPt, 2);
	if (bU)
	{
		double _U1 = 0.0;
		double _U2 = 0.0;
		double _U3 = 0.0;

		if (bU1)
			_U1 = U1;
		else
			_U1 = m_pModel->GetDomainDisplacement(DomainPt, 0);

		if (bU2)
			_U2 = U2;
		else
			_U2 = m_pModel->GetDomainDisplacement(DomainPt, 1);

		if (bU3)
			_U3 = U3;
		else
			_U3 = m_pModel->GetDomainDisplacement(DomainPt, 2);	

		BEM_3D::Vector _U(_U1, _U2, _U3);

		U = _U.Magnitude();
	}
	//===========================================================================

	// Stresses =================================================================
	if (bSig_11)
		Sig_11 = m_pModel->GetDomainStress(DomainPt, 0, 0);
	if (bSig_22)
		Sig_22 = m_pModel->GetDomainStress(DomainPt, 1, 1);
	if (bSig_33)
		Sig_33 = m_pModel->GetDomainStress(DomainPt, 2, 2);
	if (bSig_12)
		Sig_12 = m_pModel->GetDomainStress(DomainPt, 0, 1);
	if (bSig_23)
		Sig_23 = m_pModel->GetDomainStress(DomainPt, 1, 2);
	if (bSig_31)
		Sig_31 = m_pModel->GetDomainStress(DomainPt, 2, 0);
	if (bSig_VM)
	{
		double _11 = 0.0;
		double _22 = 0.0;
		double _33 = 0.0;
		double _12 = 0.0;
		double _23 = 0.0;
		double _31 = 0.0;

		if (bSig_11)
			_11 = Sig_11;
		else
			_11 = m_pModel->GetDomainStress(DomainPt, 0, 0);

		if (bSig_22)
			_22 = Sig_22;
		else
			_22 = m_pModel->GetDomainStress(DomainPt, 1, 1);
		
		if (bSig_33)
			_33 = Sig_33;
		else
			_33 = m_pModel->GetDomainStress(DomainPt, 2, 2);
		
		if (bSig_12)
			_12 = Sig_12;
		else
			_12 = m_pModel->GetDomainStress(DomainPt, 0, 1);
		
		if (bSig_23)
			_23 = Sig_23;
		else
			_23 = m_pModel->GetDomainStress(DomainPt, 1, 2);
		
		if (bSig_31)
			_31 = Sig_31;
		else
			_31 = m_pModel->GetDomainStress(DomainPt, 2, 0);

		BEM_3D::Tensor Sig(_11, _22, _33, _12, _31, _23);
		Sig_VM = Sig.VonMises();
	}

	//===========================================================================
	// Strains =================================================================
	if (bEps_11)
		Eps_11 = m_pModel->GetDomainStrain(DomainPt, 0, 0);
	if (bEps_22)
		Eps_22 = m_pModel->GetDomainStrain(DomainPt, 1, 1);
	if (bEps_33)
		Eps_33 = m_pModel->GetDomainStrain(DomainPt, 2, 2);
	if (bEps_12)
		Eps_12 = m_pModel->GetDomainStrain(DomainPt, 0, 1);
	if (bEps_23)
		Eps_23 = m_pModel->GetDomainStrain(DomainPt, 1, 2);
	if (bEps_31)
		Eps_31 = m_pModel->GetDomainStrain(DomainPt, 2, 0);
	//===========================================================================


	UpdateData(FALSE);
}


BOOL CDlgDomainNodeResponse::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// Disable all the variable edit controls
	EditU1.EnableWindow(FALSE);
	EditU2.EnableWindow(FALSE);
	EditU3.EnableWindow(FALSE);
	EditU.EnableWindow(FALSE);

	EditSig_11.EnableWindow(FALSE);
	EditSig_22.EnableWindow(FALSE);
	EditSig_33.EnableWindow(FALSE);
	EditSig_12.EnableWindow(FALSE);
	EditSig_23.EnableWindow(FALSE);
	EditSig_31.EnableWindow(FALSE);
	EditSig_VM.EnableWindow(FALSE);

	EditEps_11.EnableWindow(FALSE);
	EditEps_22.EnableWindow(FALSE);
	EditEps_33.EnableWindow(FALSE);
	EditEps_12.EnableWindow(FALSE);
	EditEps_23.EnableWindow(FALSE);
	EditEps_31.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgDomainNodeResponse::OnBnClickedCheckU1()
{
	// TODO: Add your control notification handler code here
	EditU1.EnableWindow(CheckU1.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckU2()
{
	// TODO: Add your control notification handler code here
	EditU2.EnableWindow(CheckU2.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckU3()
{
	// TODO: Add your control notification handler code here
	EditU3.EnableWindow(CheckU3.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckU()
{
	// TODO: Add your control notification handler code here
	EditU.EnableWindow(CheckU.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSig11()
{
	// TODO: Add your control notification handler code here
	EditSig_11.EnableWindow(CheckSig_11.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSig22()
{
	// TODO: Add your control notification handler code here
	EditSig_22.EnableWindow(CheckSig_22.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSig33()
{
	// TODO: Add your control notification handler code here
	EditSig_33.EnableWindow(CheckSig_33.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSig12()
{
	// TODO: Add your control notification handler code here
	EditSig_12.EnableWindow(CheckSig_12.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSig23()
{
	// TODO: Add your control notification handler code here
	EditSig_23.EnableWindow(CheckSig_23.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSig31()
{
	// TODO: Add your control notification handler code here
	EditSig_31.EnableWindow(CheckSig_31.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSigVonMises()
{
	// TODO: Add your control notification handler code here
	EditSig_VM.EnableWindow(CheckSig_VM.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEps11()
{
	// TODO: Add your control notification handler code here
	EditEps_11.EnableWindow(CheckEps_11.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEps22()
{
	// TODO: Add your control notification handler code here
	EditEps_22.EnableWindow(CheckEps_22.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEps33()
{
	// TODO: Add your control notification handler code here
	EditEps_33.EnableWindow(CheckEps_33.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEps12()
{
	// TODO: Add your control notification handler code here
	EditEps_12.EnableWindow(CheckEps_12.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEps23()
{
	// TODO: Add your control notification handler code here
	EditEps_23.EnableWindow(CheckEps_23.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEps31()
{
	// TODO: Add your control notification handler code here
	EditEps_31.EnableWindow(CheckEps_31.GetCheck());
}
