// DlgDomainNodeResponse.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgDomainNodeResponse.h"


// CDlgDomainNodeResponse dialog

IMPLEMENT_DYNAMIC(CDlgDomainNodeResponse, CDialogEx)

CDlgDomainNodeResponse::CDlgDomainNodeResponse(CGABESDoc* pDoc, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_DOMAIN_NODE_RESPONSE, pParent)
	, m_pDoc(pDoc)
	, m_rModel(pDoc->m_Model)
	, m_pTrackNode(nullptr)

	, Dim1(0)
	, Dim2(0)
	, Dim3(0)

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
	, Sig_I(0)
	, Sig_II(0)
	, Sig_III(0)
	, Sig_Mises(0)
	, Sig_Tresca(0)

	, Eps_11(0)
	, Eps_22(0)
	, Eps_33(0)
	, Eps_12(0)
	, Eps_23(0)
	, Eps_31(0)
	, Eps_I(0)
	, Eps_II(0)
	, Eps_III(0)

	
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
	, bSig_I(FALSE)
	, bSig_II(FALSE)
	, bSig_III(FALSE)
	, bSig_Mises(FALSE)
	, bSig_Tresca(FALSE)

	
	, bEps_11(FALSE)
	, bEps_22(FALSE)
	, bEps_33(FALSE)
	, bEps_12(FALSE)
	, bEps_23(FALSE)
	, bEps_31(FALSE)
	, bEps_I(FALSE)
	, bEps_II(FALSE)
	, bEps_III(FALSE)

	, nFrame(0)
	, nCoordSys(0)
{

}

CDlgDomainNodeResponse::~CDlgDomainNodeResponse()
{
}


void CDlgDomainNodeResponse::InitModelessDialog()
{
	m_pTrackNode = new BEM_3D::Vertex;

	m_pDoc->UnselectAllRefFrames();

	m_pCurrFrame = &m_pDoc->m_GlobalFrame;
	m_pCurrFrame->m_bSelected = true;
	bU1 = FALSE;
	bU2 = FALSE;
	bU3 = FALSE;
	bU = FALSE;
	
	bSig_11 = FALSE;
	bSig_22 = FALSE;
	bSig_33 = FALSE;
	bSig_12 = FALSE;
	bSig_23 = FALSE;
	bSig_31 = FALSE;
	bSig_I = FALSE;
	bSig_II = FALSE;
	bSig_III = FALSE;
	bSig_Mises = FALSE;
	bSig_Tresca = FALSE;

	bEps_11 = FALSE;
	bEps_22 = FALSE;
	bEps_33 = FALSE;
	bEps_12 = FALSE;
	bEps_23 = FALSE;
	bEps_31 = FALSE;
	bEps_I = FALSE;
	bEps_II = FALSE;
	bEps_III = FALSE;
	
	nFrame = 0;
	nCoordSys = 0;

	// Initialize the Frames ComboBox
	cmbRefFrames.ResetContent();
	cmbRefFrames.AddString(m_pDoc->m_GlobalFrame.m_strName);
	for (BEM_3D::ReferenceFrame* pRefFrame : m_pDoc->m_ReferenceFrames)
		cmbRefFrames.AddString(pRefFrame->m_strName);

	// Select the Global Frame with Cartizian system
	cmbRefFrames.SetCurSel(0);
	cmbCoordSys.SetCurSel(0);

	strDim1 = _T("X:");
	strDim2 = _T("Y:");
	strDim3 = _T("Z:");
	
	UpdateData(FALSE); // Variables to controls
}


void CDlgDomainNodeResponse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	if (pDX->m_bSaveAndValidate)
	{
		// ========= Edit → variables =========
		DDX_Text(pDX, IDC_EDIT_DIM1, Dim1);
		DDX_Text(pDX, IDC_EDIT_DIM2, Dim2);
		DDX_Text(pDX, IDC_EDIT_DIM3, Dim3);

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
		DDX_Text(pDX, IDC_EDIT_SIG_I, Sig_I);
		DDX_Text(pDX, IDC_EDIT_SIG_II, Sig_II);
		DDX_Text(pDX, IDC_EDIT_SIG_III, Sig_III);
		DDX_Text(pDX, IDC_EDIT_SIG_VON_MISES, Sig_Mises);
		DDX_Text(pDX, IDC_EDIT_SIG_TRESCA, Sig_Tresca);

		DDX_Text(pDX, IDC_EDIT_EPS11, Eps_11);
		DDX_Text(pDX, IDC_EDIT_EPS22, Eps_22);
		DDX_Text(pDX, IDC_EDIT_EPS33, Eps_33);
		DDX_Text(pDX, IDC_EDIT_EPS12, Eps_12);
		DDX_Text(pDX, IDC_EDIT_EPS23, Eps_23);
		DDX_Text(pDX, IDC_EDIT_EPS31, Eps_31);
		DDX_Text(pDX, IDC_EDIT_EPS_I, Eps_I);
		DDX_Text(pDX, IDC_EDIT_EPS_II, Eps_II);
		DDX_Text(pDX, IDC_EDIT_EPS_III, Eps_III);
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

		F(IDC_EDIT_DIM1, Dim1);
		F(IDC_EDIT_DIM2, Dim2);
		F(IDC_EDIT_DIM3, Dim3);

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
		F(IDC_EDIT_SIG_I, Sig_I);
		F(IDC_EDIT_SIG_II, Sig_II);
		F(IDC_EDIT_SIG_III, Sig_III);
		F(IDC_EDIT_SIG_VON_MISES, Sig_Mises);
		F(IDC_EDIT_SIG_TRESCA, Sig_Tresca);

		F(IDC_EDIT_EPS11, Eps_11);
		F(IDC_EDIT_EPS22, Eps_22);
		F(IDC_EDIT_EPS33, Eps_33);
		F(IDC_EDIT_EPS12, Eps_12);
		F(IDC_EDIT_EPS23, Eps_23);
		F(IDC_EDIT_EPS31, Eps_31);
		F(IDC_EDIT_EPS_I, Eps_I);
		F(IDC_EDIT_EPS_II, Eps_II);
		F(IDC_EDIT_EPS_III, Eps_III);
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
	DDX_Check(pDX, IDC_CHECK_SIG_I, bSig_I);
	DDX_Check(pDX, IDC_CHECK_SIG_II, bSig_II);
	DDX_Check(pDX, IDC_CHECK_SIG_III, bSig_III);
	DDX_Check(pDX, IDC_CHECK_SIG_VON_MISES, bSig_Mises);
	DDX_Check(pDX, IDC_CHECK_SIG_TRESCA, bSig_Tresca);

	DDX_Check(pDX, IDC_CHECK_EPS11, bEps_11);
	DDX_Check(pDX, IDC_CHECK_EPS22, bEps_22);
	DDX_Check(pDX, IDC_CHECK_EPS33, bEps_33);
	DDX_Check(pDX, IDC_CHECK_EPS12, bEps_12);
	DDX_Check(pDX, IDC_CHECK_EPS23, bEps_23);
	DDX_Check(pDX, IDC_CHECK_EPS31, bEps_31);
	DDX_Check(pDX, IDC_CHECK_EPS_I, bEps_I);
	DDX_Check(pDX, IDC_CHECK_EPS_II, bEps_II);
	DDX_Check(pDX, IDC_CHECK_EPS_III, bEps_III);


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
	DDX_Control(pDX, IDC_CHECK_SIG_I, CheckSig_I);
	DDX_Control(pDX, IDC_CHECK_SIG_II, CheckSig_II);
	DDX_Control(pDX, IDC_CHECK_SIG_III, CheckSig_III);
	DDX_Control(pDX, IDC_CHECK_SIG_VON_MISES, CheckSig_Mises);
	DDX_Control(pDX, IDC_CHECK_SIG_TRESCA, CheckSig_Tresca);

	DDX_Control(pDX, IDC_CHECK_EPS11, CheckEps_11);
	DDX_Control(pDX, IDC_CHECK_EPS22, CheckEps_22);
	DDX_Control(pDX, IDC_CHECK_EPS33, CheckEps_33);
	DDX_Control(pDX, IDC_CHECK_EPS12, CheckEps_12);
	DDX_Control(pDX, IDC_CHECK_EPS23, CheckEps_23);
	DDX_Control(pDX, IDC_CHECK_EPS31, CheckEps_31);
	DDX_Control(pDX, IDC_CHECK_EPS_I, CheckEps_I);
	DDX_Control(pDX, IDC_CHECK_EPS_II, CheckEps_II);
	DDX_Control(pDX, IDC_CHECK_EPS_III, CheckEps_III);

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
	DDX_Control(pDX, IDC_EDIT_SIG_I, EditSig_I);
	DDX_Control(pDX, IDC_EDIT_SIG_II, EditSig_II);
	DDX_Control(pDX, IDC_EDIT_SIG_III, EditSig_III);
	DDX_Control(pDX, IDC_EDIT_SIG_VON_MISES, EditSig_Mises);
	DDX_Control(pDX, IDC_EDIT_SIG_TRESCA, EditSig_Tresca);

	DDX_Control(pDX, IDC_EDIT_EPS11, EditEps_11);
	DDX_Control(pDX, IDC_EDIT_EPS22, EditEps_22);
	DDX_Control(pDX, IDC_EDIT_EPS33, EditEps_33);
	DDX_Control(pDX, IDC_EDIT_EPS12, EditEps_12);
	DDX_Control(pDX, IDC_EDIT_EPS23, EditEps_23);
	DDX_Control(pDX, IDC_EDIT_EPS31, EditEps_31);
	DDX_Control(pDX, IDC_EDIT_EPS_I, EditEps_I);
	DDX_Control(pDX, IDC_EDIT_EPS_II, EditEps_II);
	DDX_Control(pDX, IDC_EDIT_EPS_III, EditEps_III);

	DDX_Text(pDX, IDC_STATIC_DIM1, strDim1);
	DDX_Text(pDX, IDC_STATIC_DIM2, strDim2);
	DDX_Text(pDX, IDC_STATIC_DIM3, strDim3);
	DDX_CBIndex(pDX, IDC_COMBO_REF_FRAME, nFrame);
	DDX_CBIndex(pDX, IDC_COMBO_COORD_SYS, nCoordSys);
	DDX_Control(pDX, IDC_COMBO_REF_FRAME, cmbRefFrames);
	DDX_Control(pDX, IDC_COMBO_COORD_SYS, cmbCoordSys);
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
	ON_BN_CLICKED(IDC_CHECK_SIG_I, &CDlgDomainNodeResponse::OnBnClickedCheckSigI)
	ON_BN_CLICKED(IDC_CHECK_SIG_II, &CDlgDomainNodeResponse::OnBnClickedCheckSigII)
	ON_BN_CLICKED(IDC_CHECK_SIG_III, &CDlgDomainNodeResponse::OnBnClickedCheckSigIII)
	ON_BN_CLICKED(IDC_CHECK_SIG_VON_MISES, &CDlgDomainNodeResponse::OnBnClickedCheckSigMises)
	ON_BN_CLICKED(IDC_CHECK_SIG_TRESCA, &CDlgDomainNodeResponse::OnBnClickedCheckSigTresca)

	ON_BN_CLICKED(IDC_CHECK_EPS11, &CDlgDomainNodeResponse::OnBnClickedCheckEps11)
	ON_BN_CLICKED(IDC_CHECK_EPS22, &CDlgDomainNodeResponse::OnBnClickedCheckEps22)
	ON_BN_CLICKED(IDC_CHECK_EPS33, &CDlgDomainNodeResponse::OnBnClickedCheckEps33)
	ON_BN_CLICKED(IDC_CHECK_EPS12, &CDlgDomainNodeResponse::OnBnClickedCheckEps12)
	ON_BN_CLICKED(IDC_CHECK_EPS23, &CDlgDomainNodeResponse::OnBnClickedCheckEps23)
	ON_BN_CLICKED(IDC_CHECK_EPS31, &CDlgDomainNodeResponse::OnBnClickedCheckEps31)
	ON_BN_CLICKED(IDC_CHECK_EPS_I, &CDlgDomainNodeResponse::OnBnClickedCheckEpsI)
	ON_BN_CLICKED(IDC_CHECK_EPS_II, &CDlgDomainNodeResponse::OnBnClickedCheckEpsII)
	ON_BN_CLICKED(IDC_CHECK_EPS_III, &CDlgDomainNodeResponse::OnBnClickedCheckEpsIII)

	ON_CBN_SELCHANGE(IDC_COMBO_REF_FRAME, &CDlgDomainNodeResponse::OnCbnSelchangeComboRefFrame)
	ON_CBN_SELCHANGE(IDC_COMBO_COORD_SYS, &CDlgDomainNodeResponse::OnCbnSelchangeComboCoordSys)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgDomainNodeResponse::OnBnClickedButtonApply)	
END_MESSAGE_MAP()


// CDlgDomainNodeResponse message handlers

void CDlgDomainNodeResponse::UpdateTrackNode()
{
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;


	switch (nCoordSys)
	{
	case 0: // Cartesian
	{
		x = Dim1;
		y = Dim2;
		z = Dim3;
	}
	break;

	case 1: // Cylindrical
	{
		double R = Dim1;
		double Theta = Dim2 * M_PI / 180.0;
		x = R * cos(Theta);
		y = R * sin(Theta);
		z = Dim3;
	}
	break;

	case 2: // Spherical
	{
		double R = Dim1;
		double Theta = Dim2 * M_PI / 180.0;
		double Phi = Dim3 * M_PI / 180.0;
		x = R * sin(Phi) * cos(Theta);
		y = R * sin(Phi) * sin(Theta);
		z = R * cos(Phi);
	}
	break;
	}

	m_pTrackNode->Set(x, y, z);

	m_pCurrFrame->ToGlobalCoord(*m_pTrackNode);

}


void CDlgDomainNodeResponse::OnBnClickedCalculate()
{
	// Set the Wait Cursor
	HCURSOR hPrevCursor = GetCursor();
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	// TODO: Add your control notification handler code here
	UpdateData();

	U1 = U2 = U3 = U = 
		Sig_11 = Sig_22 = Sig_33 = Sig_12 = Sig_23 = Sig_31 = Sig_I = Sig_II = Sig_III = Sig_Mises = Sig_Tresca =
		Eps_11 = Eps_22 = Eps_33 = Eps_12 = Eps_23 = Eps_31 = Eps_I = Eps_II = Eps_III = 0.0;

	// Update the Track Node
	UpdateTrackNode();

	BEM_3D::Vertex& DomainPt = *m_pTrackNode;


	// Displacements ===================================================
	if (bU1)
		U1 = m_rModel.GetDomainDisplacement(DomainPt, 0);
	if (bU2)
		U2 = m_rModel.GetDomainDisplacement(DomainPt, 1);
	if (bU3)
		U3 = m_rModel.GetDomainDisplacement(DomainPt, 2);
	if (bU)
	{
		double _U1 = 0.0;
		double _U2 = 0.0;
		double _U3 = 0.0;

		if (bU1)
			_U1 = U1;
		else
			_U1 = m_rModel.GetDomainDisplacement(DomainPt, 0);

		if (bU2)
			_U2 = U2;
		else
			_U2 = m_rModel.GetDomainDisplacement(DomainPt, 1);

		if (bU3)
			_U3 = U3;
		else
			_U3 = m_rModel.GetDomainDisplacement(DomainPt, 2);	

		BEM_3D::Vector _U(_U1, _U2, _U3);

		U = _U.Magnitude();
	}
	//===========================================================================

	// Stresses =================================================================
	if (bSig_11)
		Sig_11 = m_rModel.GetDomainStress(DomainPt, 0, 0);
	if (bSig_22)
		Sig_22 = m_rModel.GetDomainStress(DomainPt, 1, 1);
	if (bSig_33)
		Sig_33 = m_rModel.GetDomainStress(DomainPt, 2, 2);
	if (bSig_12)
		Sig_12 = m_rModel.GetDomainStress(DomainPt, 0, 1);
	if (bSig_23)
		Sig_23 = m_rModel.GetDomainStress(DomainPt, 1, 2);
	if (bSig_31)
		Sig_31 = m_rModel.GetDomainStress(DomainPt, 2, 0);
	if (bSig_I)
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
			_11 = m_rModel.GetDomainStress(DomainPt, 0, 0);

		if (bSig_22)
			_22 = Sig_22;
		else
			_22 = m_rModel.GetDomainStress(DomainPt, 1, 1);

		if (bSig_33)
			_33 = Sig_33;
		else
			_33 = m_rModel.GetDomainStress(DomainPt, 2, 2);

		if (bSig_12)
			_12 = Sig_12;
		else
			_12 = m_rModel.GetDomainStress(DomainPt, 0, 1);

		if (bSig_23)
			_23 = Sig_23;
		else
			_23 = m_rModel.GetDomainStress(DomainPt, 1, 2);

		if (bSig_31)
			_31 = Sig_31;
		else
			_31 = m_rModel.GetDomainStress(DomainPt, 2, 0);

		BEM_3D::Tensor Sig(_11, _22, _33, _12, _31, _23);
		Sig_I = Sig.Principal(0);
	}
	if (bSig_II)
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
			_11 = m_rModel.GetDomainStress(DomainPt, 0, 0);

		if (bSig_22)
			_22 = Sig_22;
		else
			_22 = m_rModel.GetDomainStress(DomainPt, 1, 1);

		if (bSig_33)
			_33 = Sig_33;
		else
			_33 = m_rModel.GetDomainStress(DomainPt, 2, 2);

		if (bSig_12)
			_12 = Sig_12;
		else
			_12 = m_rModel.GetDomainStress(DomainPt, 0, 1);

		if (bSig_23)
			_23 = Sig_23;
		else
			_23 = m_rModel.GetDomainStress(DomainPt, 1, 2);

		if (bSig_31)
			_31 = Sig_31;
		else
			_31 = m_rModel.GetDomainStress(DomainPt, 2, 0);

		BEM_3D::Tensor Sig(_11, _22, _33, _12, _31, _23);
		Sig_II = Sig.Principal(1);
	}
	if (bSig_III)
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
			_11 = m_rModel.GetDomainStress(DomainPt, 0, 0);

		if (bSig_22)
			_22 = Sig_22;
		else
			_22 = m_rModel.GetDomainStress(DomainPt, 1, 1);

		if (bSig_33)
			_33 = Sig_33;
		else
			_33 = m_rModel.GetDomainStress(DomainPt, 2, 2);

		if (bSig_12)
			_12 = Sig_12;
		else
			_12 = m_rModel.GetDomainStress(DomainPt, 0, 1);

		if (bSig_23)
			_23 = Sig_23;
		else
			_23 = m_rModel.GetDomainStress(DomainPt, 1, 2);

		if (bSig_31)
			_31 = Sig_31;
		else
			_31 = m_rModel.GetDomainStress(DomainPt, 2, 0);

		BEM_3D::Tensor Sig(_11, _22, _33, _12, _31, _23);
		Sig_III = Sig.Principal(2);
	}
	if (bSig_Mises)
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
			_11 = m_rModel.GetDomainStress(DomainPt, 0, 0);

		if (bSig_22)
			_22 = Sig_22;
		else
			_22 = m_rModel.GetDomainStress(DomainPt, 1, 1);
		
		if (bSig_33)
			_33 = Sig_33;
		else
			_33 = m_rModel.GetDomainStress(DomainPt, 2, 2);
		
		if (bSig_12)
			_12 = Sig_12;
		else
			_12 = m_rModel.GetDomainStress(DomainPt, 0, 1);
		
		if (bSig_23)
			_23 = Sig_23;
		else
			_23 = m_rModel.GetDomainStress(DomainPt, 1, 2);
		
		if (bSig_31)
			_31 = Sig_31;
		else
			_31 = m_rModel.GetDomainStress(DomainPt, 2, 0);

		BEM_3D::Tensor Sig(_11, _22, _33, _12, _31, _23);
		Sig_Mises = Sig.VonMises();
	}
	if (bSig_Tresca)
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
			_11 = m_rModel.GetDomainStress(DomainPt, 0, 0);

		if (bSig_22)
			_22 = Sig_22;
		else
			_22 = m_rModel.GetDomainStress(DomainPt, 1, 1);

		if (bSig_33)
			_33 = Sig_33;
		else
			_33 = m_rModel.GetDomainStress(DomainPt, 2, 2);

		if (bSig_12)
			_12 = Sig_12;
		else
			_12 = m_rModel.GetDomainStress(DomainPt, 0, 1);

		if (bSig_23)
			_23 = Sig_23;
		else
			_23 = m_rModel.GetDomainStress(DomainPt, 1, 2);

		if (bSig_31)
			_31 = Sig_31;
		else
			_31 = m_rModel.GetDomainStress(DomainPt, 2, 0);

		BEM_3D::Tensor Sig(_11, _22, _33, _12, _31, _23);
		Sig_Tresca = Sig.Tresca();
	}

	//===========================================================================
	// Strains =================================================================
	if (bEps_11)
		Eps_11 = m_rModel.GetDomainStrain(DomainPt, 0, 0);
	if (bEps_22)
		Eps_22 = m_rModel.GetDomainStrain(DomainPt, 1, 1);
	if (bEps_33)
		Eps_33 = m_rModel.GetDomainStrain(DomainPt, 2, 2);
	if (bEps_12)
		Eps_12 = m_rModel.GetDomainStrain(DomainPt, 0, 1);
	if (bEps_23)
		Eps_23 = m_rModel.GetDomainStrain(DomainPt, 1, 2);
	if (bEps_31)
		Eps_31 = m_rModel.GetDomainStrain(DomainPt, 2, 0);
	if (bEps_I)
	{
		double _11 = 0.0;
		double _22 = 0.0;
		double _33 = 0.0;
		double _12 = 0.0;
		double _23 = 0.0;
		double _31 = 0.0;

		if (bEps_11)
			_11 = Eps_11;
		else
			_11 = m_rModel.GetDomainStrain(DomainPt, 0, 0);

		if (bEps_22)
			_22 = Eps_22;
		else
			_22 = m_rModel.GetDomainStrain(DomainPt, 1, 1);

		if (bEps_33)
			_33 = Eps_33;
		else
			_33 = m_rModel.GetDomainStrain(DomainPt, 2, 2);

		if (bEps_12)
			_12 = Eps_12;
		else
			_12 = m_rModel.GetDomainStrain(DomainPt, 0, 1);

		if (bEps_23)
			_23 = Eps_23;
		else
			_23 = m_rModel.GetDomainStrain(DomainPt, 1, 2);

		if (bEps_31)
			_31 = Eps_31;
		else
			_31 = m_rModel.GetDomainStrain(DomainPt, 2, 0);

		BEM_3D::Tensor Eps(_11, _22, _33, _12, _31, _23);
		Eps_I = Eps.Principal(0);
	}
	if (bEps_II)
	{
		double _11 = 0.0;
		double _22 = 0.0;
		double _33 = 0.0;
		double _12 = 0.0;
		double _23 = 0.0;
		double _31 = 0.0;

		if (bEps_11)
			_11 = Eps_11;
		else
			_11 = m_rModel.GetDomainStrain(DomainPt, 0, 0);

		if (bEps_22)
			_22 = Eps_22;
		else
			_22 = m_rModel.GetDomainStrain(DomainPt, 1, 1);

		if (bEps_33)
			_33 = Eps_33;
		else
			_33 = m_rModel.GetDomainStrain(DomainPt, 2, 2);

		if (bEps_12)
			_12 = Eps_12;
		else
			_12 = m_rModel.GetDomainStrain(DomainPt, 0, 1);

		if (bEps_23)
			_23 = Eps_23;
		else
			_23 = m_rModel.GetDomainStrain(DomainPt, 1, 2);

		if (bEps_31)
			_31 = Eps_31;
		else
			_31 = m_rModel.GetDomainStrain(DomainPt, 2, 0);

		BEM_3D::Tensor Eps(_11, _22, _33, _12, _31, _23);
		Eps_II = Eps.Principal(1);
	}
	if (bEps_III)
	{
		double _11 = 0.0;
		double _22 = 0.0;
		double _33 = 0.0;
		double _12 = 0.0;
		double _23 = 0.0;
		double _31 = 0.0;

		if (bEps_11)
			_11 = Eps_11;
		else
			_11 = m_rModel.GetDomainStrain(DomainPt, 0, 0);

		if (bEps_22)
			_22 = Eps_22;
		else
			_22 = m_rModel.GetDomainStrain(DomainPt, 1, 1);

		if (bEps_33)
			_33 = Eps_33;
		else
			_33 = m_rModel.GetDomainStrain(DomainPt, 2, 2);

		if (bEps_12)
			_12 = Eps_12;
		else
			_12 = m_rModel.GetDomainStrain(DomainPt, 0, 1);

		if (bEps_23)
			_23 = Eps_23;
		else
			_23 = m_rModel.GetDomainStrain(DomainPt, 1, 2);

		if (bEps_31)
			_31 = Eps_31;
		else
			_31 = m_rModel.GetDomainStrain(DomainPt, 2, 0);

		BEM_3D::Tensor Eps(_11, _22, _33, _12, _31, _23);
		Eps_III = Eps.Principal(2);
	}
	//===========================================================================

	// Restore the previous cursor
	SetCursor(hPrevCursor);

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
	EditSig_I.EnableWindow(FALSE);
	EditSig_II.EnableWindow(FALSE);
	EditSig_III.EnableWindow(FALSE);
	EditSig_Mises.EnableWindow(FALSE);
	EditSig_Tresca.EnableWindow(FALSE);

	EditEps_11.EnableWindow(FALSE);
	EditEps_22.EnableWindow(FALSE);
	EditEps_33.EnableWindow(FALSE);
	EditEps_12.EnableWindow(FALSE);
	EditEps_23.EnableWindow(FALSE);
	EditEps_31.EnableWindow(FALSE);
	EditEps_I.EnableWindow(FALSE);
	EditEps_II.EnableWindow(FALSE);
	EditEps_III.EnableWindow(FALSE);
	
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

void CDlgDomainNodeResponse::OnBnClickedCheckSigI()
{
	// TODO: Add your control notification handler code here
	EditSig_I.EnableWindow(CheckSig_I.GetCheck());

}

void CDlgDomainNodeResponse::OnBnClickedCheckSigII()
{
	// TODO: Add your control notification handler code here
	EditSig_II.EnableWindow(CheckSig_II.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSigIII()
{
	// TODO: Add your control notification handler code here
	EditSig_III.EnableWindow(CheckSig_III.GetCheck());
}


void CDlgDomainNodeResponse::OnBnClickedCheckSigMises()
{
	// TODO: Add your control notification handler code here
	EditSig_Mises.EnableWindow(CheckSig_Mises.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckSigTresca()
{
	// TODO: Add your control notification handler code here
	EditSig_Tresca.EnableWindow(CheckSig_Tresca.GetCheck());
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

void CDlgDomainNodeResponse::OnBnClickedCheckEpsI()
{
	// TODO: Add your control notification handler code here
	EditEps_I.EnableWindow(CheckEps_I.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEpsII()
{
	// TODO: Add your control notification handler code here
	EditEps_II.EnableWindow(CheckEps_II.GetCheck());
}

void CDlgDomainNodeResponse::OnBnClickedCheckEpsIII()
{
	// TODO: Add your control notification handler code here
	EditEps_III.EnableWindow(CheckEps_III.GetCheck());
}


void CDlgDomainNodeResponse::OnCbnSelchangeComboRefFrame()
{
	// TODO: Add your control notification handler code here
	int nSel = cmbRefFrames.GetCurSel();

	if (nSel == 0)
	{
		m_pDoc->SelectGlobalFrame();
		m_pCurrFrame = &m_pDoc->m_GlobalFrame;
	}
	else
	{
		m_pDoc->SelectRefFrame(nSel - 1);
		m_pCurrFrame = m_pDoc->m_ReferenceFrames[nSel - 1];
	}
}

void CDlgDomainNodeResponse::OnCbnSelchangeComboCoordSys()
{
	// TODO: Add your control notification handler code here

	nCoordSys = cmbCoordSys.GetCurSel();

	switch (nCoordSys)
	{
	case 0: // Cartesian
		strDim1 = _T("X:");
		strDim2 = _T("Y:");
		strDim3 = _T("Z:");
		break;

	case 1: // Cylindrical
		strDim1 = _T("R:");
		strDim2 = _T("Theta:");
		strDim3 = _T("Z:");
		break;

	case 2: // Spherical
		strDim1 = _T("R:");
		strDim2 = _T("Theta:");
		strDim3 = _T("Phi:");
		break;
	}

	UpdateData(FALSE);
}

void CDlgDomainNodeResponse::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDoc->UnselectAllRefFrames();
	delete m_pTrackNode;
	m_pTrackNode = nullptr;
	CDialogEx::OnCancel();
}

void CDlgDomainNodeResponse::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here

	// Update the Track Node
	UpdateData();
	UpdateTrackNode();

	BEM_3D::Vertex& DomainPt = *m_pTrackNode;
}


