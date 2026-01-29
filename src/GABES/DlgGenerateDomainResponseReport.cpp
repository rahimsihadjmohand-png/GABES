// CDlgGenerateDomainResponseReport.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgGenerateDomainResponseReport.h"
#include <iomanip>


#ifdef DEBUG
#define _tfstream std::fstream
#else
#define _tfstream std::wfstream
#endif // DEBUG

// CDlgGenerateDomainResponseReport dialog

IMPLEMENT_DYNAMIC(CDlgGenerateDomainResponseReport, CDialogEx)

CDlgGenerateDomainResponseReport::CDlgGenerateDomainResponseReport(BEM_3D::Model* pModel, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_GENERATE_DOMAIN_RESPONSE_REPORT, pParent)
	, bUseInputFile(TRUE)
	, strInputFile(_T(""))
	, strOutputFile(_T(""))
	, m_pModel(pModel)
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
	, bSig_VM(FALSE)
	, bEps_11(FALSE)
	, bEps_22(FALSE)
	, bEps_33(FALSE)
	, bEps_12(FALSE)
	, bEps_23(FALSE)
	, bEps_31(FALSE)
{

}

CDlgGenerateDomainResponseReport::~CDlgGenerateDomainResponseReport()
{
}

void CDlgGenerateDomainResponseReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_INPUT_FILE, RadioInputFile);
	DDX_Control(pDX, IDC_RADIO_GENERATE_GRID, RadioGenerateGrid);
	DDX_Check(pDX, IDC_RADIO_INPUT_FILE, bUseInputFile);
	DDX_Control(pDX, IDC_EDIT_INPUT_FILE, EditInputFile);
	DDX_Control(pDX, IDC_BUTTON_INPUT_FILE, BtnInputFile);
	DDX_Control(pDX, IDC_BUTTON_OPEN_GRID_GENERATION_DIALOG, BtnGridGeneration);
	DDX_Control(pDX, IDC_CHECK_GRID_GENERATED, CheckGridGenerated);
	DDX_Text(pDX, IDC_EDIT_INPUT_FILE, strInputFile);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_FILE, strOutputFile);
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
	DDX_Check(pDX, IDC_CHECK_SIG_VON_MISES2, bSig_VM);
	DDX_Check(pDX, IDC_CHECK_EPS11, bEps_11);
	DDX_Check(pDX, IDC_CHECK_EPS22, bEps_22);
	DDX_Check(pDX, IDC_CHECK_EPS33, bEps_33);
	DDX_Check(pDX, IDC_CHECK_EPS12, bEps_12);
	DDX_Check(pDX, IDC_CHECK_EPS23, bEps_23);
	DDX_Check(pDX, IDC_CHECK_EPS31, bEps_31);
}


BEGIN_MESSAGE_MAP(CDlgGenerateDomainResponseReport, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_INPUT_FILE, &CDlgGenerateDomainResponseReport::OnBnClickedRadioInputFile)
	ON_BN_CLICKED(IDC_RADIO_GENERATE_GRID, &CDlgGenerateDomainResponseReport::OnBnClickedRadioGenerateGrid)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_FILE, &CDlgGenerateDomainResponseReport::OnBnClickedButtonInputFile)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_FILE, &CDlgGenerateDomainResponseReport::OnBnClickedButtonOutputFile)
END_MESSAGE_MAP()


// CDlgGenerateDomainResponseReport message handlers

BOOL CDlgGenerateDomainResponseReport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CheckRadioButton(IDC_RADIO_INPUT_FILE, IDC_RADIO_GENERATE_GRID,
		bUseInputFile ? IDC_RADIO_INPUT_FILE : IDC_RADIO_GENERATE_GRID);

	// TODO:  Add extra initialization here
	EditInputFile.EnableWindow(bUseInputFile);
	BtnInputFile.EnableWindow(bUseInputFile);
	BtnGridGeneration.EnableWindow(!bUseInputFile);
	CheckGridGenerated.EnableWindow(!bUseInputFile);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGenerateDomainResponseReport::OnBnClickedRadioInputFile()
{
	// TODO: Add your control notification handler code here
	bUseInputFile = RadioInputFile.GetCheck();

	EditInputFile.EnableWindow(bUseInputFile);
	BtnInputFile.EnableWindow(bUseInputFile);
	BtnGridGeneration.EnableWindow(!bUseInputFile);
	CheckGridGenerated.EnableWindow(!bUseInputFile);
}

void CDlgGenerateDomainResponseReport::OnBnClickedRadioGenerateGrid()
{
	// TODO: Add your control notification handler code here
	bUseInputFile = RadioInputFile.GetCheck();

	EditInputFile.EnableWindow(bUseInputFile);
	BtnInputFile.EnableWindow(bUseInputFile);
	BtnGridGeneration.EnableWindow(!bUseInputFile);
	CheckGridGenerated.EnableWindow(!bUseInputFile);
}

void CDlgGenerateDomainResponseReport::OnBnClickedButtonInputFile()
{
	// TODO: Add your control notification handler code here
	// Update the data
	UpdateData(TRUE); // TRUE from the controls to variables

	
	OPENFILENAME ofn; // Common dialogbox structure
	TCHAR szFile[MAX_PATH]; // The file name
	HWND hWnd = this->m_hWnd; // Owner window

	// Initialize the openfilename
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0Text\0*.txt\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the openfile dialog box
	if (GetOpenFileName(&ofn) == TRUE)
	{
		// Get the File path name
		strInputFile = ofn.lpstrFile;

		// Update the data
		UpdateData(FALSE); // FALSE from the variables to controls
	}

}

void CDlgGenerateDomainResponseReport::OnBnClickedButtonOutputFile()
{
	UpdateData(TRUE);

	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = _T("");
	HWND hWnd = this->m_hWnd;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;

	// ================= FILTER (CSV DEFAULT) =================
	ofn.lpstrFilter =
		_T("CSV Files (*.csv)\0*.csv\0")
		_T("Text Files (*.txt)\0*.txt\0")
		_T("All Files (*.*)\0*.*\0");

	ofn.nFilterIndex = 1;

	// ================= DEFAULT EXTENSION =================
	ofn.lpstrDefExt = _T("csv");   // <<< IMPORTANT

	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	// ================= SAVE FILE DIALOG =================
	if (GetSaveFileName(&ofn) == TRUE)
	{
		strOutputFile = ofn.lpstrFile;

		// ================= FORCE .csv EXTENSION =================
		CString ext = strOutputFile.Right(4);
		ext.MakeLower();

		if (ext != _T(".csv"))
		{
			strOutputFile += _T(".csv");   // <<< GUARANTEED CSV
		}

		UpdateData(FALSE);
	}
}



void CDlgGenerateDomainResponseReport::GetTrackPointsFromInputFile()
{
	// Create the File 
	_tfstream InputFile(strInputFile, _tfstream::in);

	// Read the dimension
	int N = 0;
	InputFile >> N;
	
	TrackNodes.clear();
	
	for (int i = 0; i < N; i++)
	{
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		InputFile >> x;
		InputFile >> y;
		InputFile >> z;

		TrackNodes.push_back(BEM_3D::Vertex(x, y, z));
	}

	InputFile.close();
}





void CDlgGenerateDomainResponseReport::OnOK()
{
	// Update dialog data
	UpdateData(TRUE);

	if (bUseInputFile)
		GetTrackPointsFromInputFile();

	if (TrackNodes.empty())
		return CDialogEx::OnOK();

	if (!bU1 && !bU2 && !bU3 && !bU &&
		!bSig_11 && !bSig_22 && !bSig_33 && !bSig_12 && !bSig_23 && !bSig_31 && !bSig_VM &&
		!bEps_11 && !bEps_22 && !bEps_33 && !bEps_12 && !bEps_23 && !bEps_31)
		return CDialogEx::OnOK();

	// ================= CREATE CSV FILE =================
	_tfstream OutputFile(strOutputFile, _tfstream::out);
	if (!OutputFile.is_open())
		return CDialogEx::OnOK();

	// ================= EXCEL CSV HINT =================
	OutputFile << "sep=;\n";

	// ================= HEADER =================
	OutputFile << "N;X;Y;Z";

	if (bU1)     OutputFile << ";U1";
	if (bU2)     OutputFile << ";U2";
	if (bU3)     OutputFile << ";U3";
	if (bU)      OutputFile << ";|U|";

	if (bSig_11) OutputFile << ";Sig_11";
	if (bSig_22) OutputFile << ";Sig_22";
	if (bSig_33) OutputFile << ";Sig_33";
	if (bSig_12) OutputFile << ";Sig_12";
	if (bSig_23) OutputFile << ";Sig_23";
	if (bSig_31) OutputFile << ";Sig_31";
	if (bSig_VM) OutputFile << ";Sig_VM";

	if (bEps_11) OutputFile << ";Eps_11";
	if (bEps_22) OutputFile << ";Eps_22";
	if (bEps_33) OutputFile << ";Eps_33";
	if (bEps_12) OutputFile << ";Eps_12";
	if (bEps_23) OutputFile << ";Eps_23";
	if (bEps_31) OutputFile << ";Eps_31";

	OutputFile << "\n";

	// ================= DATA =================
	for (int i = 0; i < (int)TrackNodes.size(); ++i)
	{
		BEM_3D::Vertex& DomainPt = TrackNodes[i];

		double U1 = 0.0, U2 = 0.0, U3 = 0.0, U = 0.0;
		double Sig_11 = 0.0, Sig_22 = 0.0, Sig_33 = 0.0;
		double Sig_12 = 0.0, Sig_23 = 0.0, Sig_31 = 0.0, Sig_VM = 0.0;
		double Eps_11 = 0.0, Eps_22 = 0.0, Eps_33 = 0.0;
		double Eps_12 = 0.0, Eps_23 = 0.0, Eps_31 = 0.0;

		// -------- Displacements --------
		if (bU1) U1 = m_pModel->GetDomainDisplacement(DomainPt, 0);
		if (bU2) U2 = m_pModel->GetDomainDisplacement(DomainPt, 1);
		if (bU3) U3 = m_pModel->GetDomainDisplacement(DomainPt, 2);

		if (bU)
		{
			BEM_3D::Vector _U(
				bU1 ? U1 : m_pModel->GetDomainDisplacement(DomainPt, 0),
				bU2 ? U2 : m_pModel->GetDomainDisplacement(DomainPt, 1),
				bU3 ? U3 : m_pModel->GetDomainDisplacement(DomainPt, 2)
			);
			U = _U.Magnitude();
		}

		// -------- Stresses --------
		if (bSig_11) Sig_11 = m_pModel->GetDomainStress(DomainPt, 0, 0);
		if (bSig_22) Sig_22 = m_pModel->GetDomainStress(DomainPt, 1, 1);
		if (bSig_33) Sig_33 = m_pModel->GetDomainStress(DomainPt, 2, 2);
		if (bSig_12) Sig_12 = m_pModel->GetDomainStress(DomainPt, 0, 1);
		if (bSig_23) Sig_23 = m_pModel->GetDomainStress(DomainPt, 1, 2);
		if (bSig_31) Sig_31 = m_pModel->GetDomainStress(DomainPt, 2, 0);

		if (bSig_VM)
		{
			BEM_3D::Tensor Sig(
				bSig_11 ? Sig_11 : m_pModel->GetDomainStress(DomainPt, 0, 0),
				bSig_22 ? Sig_22 : m_pModel->GetDomainStress(DomainPt, 1, 1),
				bSig_33 ? Sig_33 : m_pModel->GetDomainStress(DomainPt, 2, 2),
				bSig_12 ? Sig_12 : m_pModel->GetDomainStress(DomainPt, 0, 1),
				bSig_31 ? Sig_31 : m_pModel->GetDomainStress(DomainPt, 2, 0),
				bSig_23 ? Sig_23 : m_pModel->GetDomainStress(DomainPt, 1, 2)
			);
			Sig_VM = Sig.VonMises();
		}

		// -------- Strains --------
		if (bEps_11) Eps_11 = m_pModel->GetDomainStrain(DomainPt, 0, 0);
		if (bEps_22) Eps_22 = m_pModel->GetDomainStrain(DomainPt, 1, 1);
		if (bEps_33) Eps_33 = m_pModel->GetDomainStrain(DomainPt, 2, 2);
		if (bEps_12) Eps_12 = m_pModel->GetDomainStrain(DomainPt, 0, 1);
		if (bEps_23) Eps_23 = m_pModel->GetDomainStrain(DomainPt, 1, 2);
		if (bEps_31) Eps_31 = m_pModel->GetDomainStrain(DomainPt, 2, 0);

		// -------- OUTPUT LINE --------
		OutputFile << i << ";"
			<< std::fixed << std::setprecision(4)
			<< DomainPt.x << ";"
			<< DomainPt.y << ";"
			<< DomainPt.z;

		OutputFile << std::scientific << std::setprecision(9);

		if (bU1)     OutputFile << ";" << U1;
		if (bU2)     OutputFile << ";" << U2;
		if (bU3)     OutputFile << ";" << U3;
		if (bU)      OutputFile << ";" << U;

		if (bSig_11) OutputFile << ";" << Sig_11;
		if (bSig_22) OutputFile << ";" << Sig_22;
		if (bSig_33) OutputFile << ";" << Sig_33;
		if (bSig_12) OutputFile << ";" << Sig_12;
		if (bSig_23) OutputFile << ";" << Sig_23;
		if (bSig_31) OutputFile << ";" << Sig_31;
		if (bSig_VM) OutputFile << ";" << Sig_VM;

		if (bEps_11) OutputFile << ";" << Eps_11;
		if (bEps_22) OutputFile << ";" << Eps_22;
		if (bEps_33) OutputFile << ";" << Eps_33;
		if (bEps_12) OutputFile << ";" << Eps_12;
		if (bEps_23) OutputFile << ";" << Eps_23;
		if (bEps_31) OutputFile << ";" << Eps_31;

		OutputFile << "\n";
	}

	OutputFile.close();
	CDialogEx::OnOK();
}
