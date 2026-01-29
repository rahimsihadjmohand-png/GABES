// DlgRangeSubSet.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgRangeSubSet.h"
#include "MainFrm.h"


double CDlgRangeSubSet::nt = 1.0E-2;         // tolerance value used for near comparaison _ne_ _nge_ _nle_

// CDlgRangeSubSet dialog

IMPLEMENT_DYNAMIC(CDlgRangeSubSet, CDialogEx)

CDlgRangeSubSet::CDlgRangeSubSet(BEM_3D::ReferenceFrame& rGlobalFrame,
	std::vector<BEM_3D::ReferenceFrame*>& rRefFrames,
	std::vector<int>& rSelectedElmsIndices,
	BEM_3D::Model& rModel, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_RANGE_SUBSET, pParent)
	, m_rGlobalFrame(rGlobalFrame)
	, m_rRefFrames(rRefFrames)
	, m_rSelectedElmsIndices(rSelectedElmsIndices)
	, m_rModel(rModel)
	, m_pCurrentFrame(&m_rGlobalFrame)
	, nCoordSys(0)
	, Min1(rGlobalFrame.m_X_min)
	, Min2(rGlobalFrame.m_X_min)
	, Min3(rGlobalFrame.m_Y_min)
	, Max1(rGlobalFrame.m_Y_max)
	, Max2(rGlobalFrame.m_Z_max)
	, Max3(rGlobalFrame.m_Z_max)	
	, strName(_T(""))
{

}

CDlgRangeSubSet::~CDlgRangeSubSet()
{
}

void CDlgRangeSubSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_COOR_SYS_RANGE_SUBSET, cmbCoordSys);
	DDX_CBIndex(pDX, IDC_CMB_COOR_SYS_RANGE_SUBSET, nCoordSys);
	DDX_Control(pDX, IDC_COMBO_REF_FRAMES, cmbRefFrames);
	DDX_Control(pDX, IDC_STATIC_DIM1, lblDim1);
	DDX_Control(pDX, IDC_STATIC_DIM2, lblDim2);
	DDX_Control(pDX, IDC_STATIC_DIM3, lblDim3);
	DDX_Text(pDX, IDC_EDIT_MIN_DIM1, Min1);
	DDX_Text(pDX, IDC_EDIT_MIN_DIM2, Min2);
	DDX_Text(pDX, IDC_EDIT_MIN_DIM3, Min3);
	DDX_Text(pDX, IDC_EDIT_MAX_DIM1, Max1);
	DDX_Text(pDX, IDC_EDIT_MAX_DIM2, Max2);
	DDX_Text(pDX, IDC_EDIT_MAX_DIM3, Max3);

	DDX_Control(pDX, IDC_SLIDER_MIN_DIM1, sliderMinDim1);
	DDX_Control(pDX, IDC_SLIDER_MAX_DIM1, sliderMaxDim1);
	DDX_Control(pDX, IDC_SLIDER_MIN_DIM2, sliderMinDim2);
	DDX_Control(pDX, IDC_SLIDER_MAX_DIM2, sliderMaxDim2);
	DDX_Control(pDX, IDC_SLIDER_MIN_DIM3, sliderMinDim3);
	DDX_Control(pDX, IDC_SLIDER_MAX_DIM3, sliderMaxDim3);


	if (pDX->m_bSaveAndValidate)
	{
		int nRefFrame = -1;
		DDX_CBIndex(pDX, IDC_COMBO_REF_FRAMES, nRefFrame);

		if (nRefFrame == 0)
			m_pCurrentFrame = &m_rGlobalFrame;
		else
			m_pCurrentFrame = m_rRefFrames[nRefFrame - 1];

	}
	else
	{
		int nRefFrame = -1;

		if (m_pCurrentFrame == &m_rGlobalFrame)
		{
			nRefFrame = 0;
		}
		else
		{
			for (int i = 0; i < m_rRefFrames.size(); i++)
			{
				if (m_rRefFrames[i] == m_pCurrentFrame)
				{
					nRefFrame = i + 1;
					break;
				}
			}
		}

		DDX_CBIndex(pDX, IDC_COMBO_REF_FRAMES, nRefFrame);
	}
	DDX_Text(pDX, IDC_EDIT_SUBSET_NAME, strName);
}


BEGIN_MESSAGE_MAP(CDlgRangeSubSet, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CMB_COOR_SYS_RANGE_SUBSET, &CDlgRangeSubSet::OnCbnSelchangeCmbCoorSysRangeSubset)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_REF_FRAMES, &CDlgRangeSubSet::OnCbnSelchangeComboRefFrames)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgRangeSubSet::OnBnClickedBtnApply)
END_MESSAGE_MAP()


// CDlgRangeSubSet message handlers

void CDlgRangeSubSet::OnCbnSelchangeCmbCoorSysRangeSubset()
{
	// TODO: Add your control notification handler code here

	nCoordSys = cmbCoordSys.GetCurSel();
	switch (nCoordSys)
	{
	case 0:
		lblDim1.SetWindowText(_T("X Range:"));
		lblDim2.SetWindowText(_T("Y Range:"));
		lblDim3.SetWindowText(_T("Z Range:"));		

		Min1 = m_pCurrentFrame->m_X_min;
		Min2 = m_pCurrentFrame->m_Y_min;
		Min3 = m_pCurrentFrame->m_Z_min;

		Max1 = m_pCurrentFrame->m_X_max;
		Max2 = m_pCurrentFrame->m_Y_max;
		Max3 = m_pCurrentFrame->m_Z_max;
		break;

	case 1:
		lblDim1.SetWindowText(_T("Radius (R) Range:"));
		lblDim2.SetWindowText(_T("Angle (Theta) Range:"));
		lblDim3.SetWindowText(_T("Z Range:"));

		Min1 = m_pCurrentFrame->m_Rc_min;
		Min2 = -180.0;
		Min3 = m_pCurrentFrame->m_Z_min;

		Max1 = m_pCurrentFrame->m_Rc_max;
		Max2 = 180.0;
		Max3 = m_pCurrentFrame->m_Z_max;
		break;

	case 2:
		lblDim1.SetWindowText(_T("Radius (R) Range:"));
		lblDim2.SetWindowText(_T("Angle (Theta) Range:"));
		lblDim3.SetWindowText(_T("Angle (Phi) Range:"));

		Min1 = m_pCurrentFrame->m_Rs_min;
		Min2 = -180.0;
		Min3 = 0.0;

		Max1 = m_pCurrentFrame->m_Rs_max;
		Max2 = 180.0;
		Max3 = 180.0;
		break;

	default:
		break;
	}

	UpdateData(FALSE);

	UpdateElementSelection();
}


void CDlgRangeSubSet::OnCbnSelchangeComboRefFrames()
{
	// TODO: Add your control notification handler code here
	// Set the Current frame
	int nRefFrame = cmbRefFrames.GetCurSel();
	if (nRefFrame < 0)
		return;

	else if (nRefFrame == 0)
		m_pCurrentFrame = &m_rGlobalFrame;
	else
		m_pCurrentFrame = m_rRefFrames[nRefFrame - 1];


	switch (nCoordSys)
	{
	case 0:
		lblDim1.SetWindowText(_T("X Range:"));
		lblDim2.SetWindowText(_T("Y Range:"));
		lblDim3.SetWindowText(_T("Z Range:"));

		Min1 = m_pCurrentFrame->m_X_min;
		Min2 = m_pCurrentFrame->m_Y_min;
		Min3 = m_pCurrentFrame->m_Z_min;

		Max1 = m_pCurrentFrame->m_X_max;
		Max2 = m_pCurrentFrame->m_Y_max;
		Max3 = m_pCurrentFrame->m_Z_max;
		break;

	case 1:
		lblDim1.SetWindowText(_T("Radius (R) Range:"));
		lblDim2.SetWindowText(_T("Angle (Theta) Range:"));
		lblDim3.SetWindowText(_T("Z Range:"));

		Min1 = m_pCurrentFrame->m_Rc_min;
		Min2 = -180.0;
		Min3 = m_pCurrentFrame->m_Z_min;

		Max1 = m_pCurrentFrame->m_Rc_max;
		Max2 = 180.0;
		Max3 = m_pCurrentFrame->m_Z_max;
		break;

	case 2:
		lblDim1.SetWindowText(_T("Radius (R) Range:"));
		lblDim2.SetWindowText(_T("Angle (Theta) Range:"));
		lblDim3.SetWindowText(_T("Angle (Phi) Range:"));

		Min1 = m_pCurrentFrame->m_Rs_min;
		Min2 = -180.0;
		Min3 = 0.0;

		Max1 = m_pCurrentFrame->m_Rs_max;
		Max2 = 180.0;
		Max3 = 180.0;
		break;

	default:
		break;
	}



	// Select the choosen frame 
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESDoc* pDoc = (CGABESDoc*)pMainFrame->GetActiveDocument();
	pDoc->SelectRefFrame(m_pCurrentFrame);

	UpdateData(FALSE);

	UpdateElementSelection();
}


BOOL CDlgRangeSubSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	lblDim1.SetWindowText(_T("X Range:"));
	lblDim2.SetWindowText(_T("Y Range:"));
	lblDim3.SetWindowText(_T("Z Range:"));

	Min1 = m_pCurrentFrame->m_X_min;
	Min2 = m_pCurrentFrame->m_Y_min;
	Min3 = m_pCurrentFrame->m_Z_min;

	Max1 = m_pCurrentFrame->m_X_max;
	Max2 = m_pCurrentFrame->m_Y_max;
	Max3 = m_pCurrentFrame->m_Z_max;
	

	// Load the Reference frames
	cmbRefFrames.AddString(m_rGlobalFrame.m_strName);
	for (BEM_3D::ReferenceFrame* pRefFrame : m_rRefFrames)
		cmbRefFrames.AddString(pRefFrame->m_strName);

	// Set the current selection
	cmbRefFrames.SetCurSel(0); // Select the Global Frame by default
	
	// Set the current range According Coordinate system Selection
	sliderMinDim1.SetRange(0, 100);
	sliderMaxDim1.SetRange(0, 100);
	sliderMinDim2.SetRange(0, 100);
	sliderMaxDim2.SetRange(0, 100);
	sliderMinDim3.SetRange(0, 100);
	sliderMaxDim1.SetRange(0, 100);

	sliderMinDim1.SetPos(0);
	sliderMaxDim1.SetPos(100);
	sliderMinDim2.SetPos(0);
	sliderMaxDim2.SetPos(100);	
	sliderMinDim3.SetPos(0);
	sliderMaxDim3.SetPos(100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgRangeSubSet::InitModelessDialog()
{
	// TODO:  Add extra initialization here
	lblDim1.SetWindowText(_T("X Range:"));
	lblDim2.SetWindowText(_T("Y Range:"));
	lblDim3.SetWindowText(_T("Z Range:"));

	Min1 = m_pCurrentFrame->m_X_min;
	Min2 = m_pCurrentFrame->m_Y_min;
	Min3 = m_pCurrentFrame->m_Z_min;

	Max1 = m_pCurrentFrame->m_X_max;
	Max2 = m_pCurrentFrame->m_Y_max;
	Max3 = m_pCurrentFrame->m_Z_max;


	// Load the Reference frames
	cmbRefFrames.ResetContent();
	cmbRefFrames.AddString(m_rGlobalFrame.m_strName);
	for (BEM_3D::ReferenceFrame* pRefFrame : m_rRefFrames)
		cmbRefFrames.AddString(pRefFrame->m_strName);

	// Set the current selection
	cmbRefFrames.SetCurSel(0); // Select the Global Frame by default

	// Select the Global Frame
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESDoc* pDoc = (CGABESDoc*)pMainFrame->GetActiveDocument();
	pDoc->SelectGlobalFrame();

	UpdateData(FALSE);

	UpdateElementSelection();
}



//
//bool CDlgRangeSubSet::_ne_(double Val1, double Val2) // near equal ==
//{
//	return (abs(Val1 - Val2) <= nt);
//}
//
//bool CDlgRangeSubSet::_ng_(double Val1, double Val2) // near greater        >
//{
//	return (Val1 > (Val2 + nt));
//}
//
//bool CDlgRangeSubSet::_nl_(double Val1, double Val2) // near lesser         <
//{
//	return (Val1 < (Val2 - nt));
//}
//
//bool CDlgRangeSubSet::_nge_(double Val1, double Val2) // near greater_equal >=
//{
//	return (Val1 > (Val2 - nt));
//}
//
//bool CDlgRangeSubSet::_nle_(double Val1, double Val2) // near lesser_equal  <=
//{
//	return (Val1 < (Val2 + nt));
//}


void CDlgRangeSubSet::UpdateElementSelection()
{
	UpdateData();

	// Get the Element Set comprized in this range
	const std::vector<BEM_3D::Element*>& rElements = m_rModel.GetElements();
	m_rSelectedElmsIndices.clear();

	// Converte angles to Radians
	switch (nCoordSys)
	{
	case 0:     // Cartezian (No angles to convert
		break;

	case 1:    // Cylindrical
		Min2 = Min2 * M_PI / 180.0;
		Max2 = Max2 * M_PI / 180.0;
		break;

	case 2:    // Sphirical
		Min2 = Min2 * M_PI / 180.0;
		Max2 = Max2 * M_PI / 180.0;

		Min3 = Min3 * M_PI / 180.0;
		Max3 = Max3 * M_PI / 180.0;
		break;

	default:
		break;
	}




	// Deccrement the min values and increment the max values
	Min1 -= nt;
	Min2 -= nt;
	Min3 -= nt;

	Max1 += nt;
	Max2 += nt;
	Max3 += nt;


	for (int i = 0; i < rElements.size(); i++)
	{
		BEM_3D::Element* pElm = rElements[i];

		BEM_3D::Vertex V1 = *pElm->m_pV1;
		BEM_3D::Vertex V2 = *pElm->m_pV2;
		BEM_3D::Vertex V3 = *pElm->m_pV3;

		// The previous coordinates are in Global space 
		// Transform them to the current coordinate space
		m_pCurrentFrame->ToLocalCoord(V1);
		m_pCurrentFrame->ToLocalCoord(V2);
		m_pCurrentFrame->ToLocalCoord(V3);

		double x1 = V1.x;
		double y1 = V1.y;
		double z1 = V1.z;

		double x2 = V2.x;
		double y2 = V2.y;
		double z2 = V2.z;

		double x3 = V3.x;
		double y3 = V3.y;
		double z3 = V3.z;


		// The first index refers to the vertex the second index to the dimension
		double Dim11, Dim12, Dim13;
		double Dim21, Dim22, Dim23;
		double Dim31, Dim32, Dim33;


		switch (nCoordSys)
		{
		case 0: // Cartezian
		{
			Dim11 = x1;
			Dim12 = y1;
			Dim13 = z1;

			Dim21 = x2;
			Dim22 = y2;
			Dim23 = z2;

			Dim31 = x3;
			Dim32 = y3;
			Dim33 = z3;
		}
		break;

		case 1: // Cylindrical
		{
			double R1 = 0.0;
			double R2 = 0.0;
			double R3 = 0.0;

			double Theta1 = 0.0;
			double Theta2 = 0.0;
			double Theta3 = 0.0;

			double Z1 = 0.0;
			double Z2 = 0.0;
			double Z3 = 0.0;


			m_pCurrentFrame->ToCylindricalCoord(V1, R1, Theta1, Z1);
			m_pCurrentFrame->ToCylindricalCoord(V2, R2, Theta2, Z2);
			m_pCurrentFrame->ToCylindricalCoord(V3, R3, Theta3, Z3);


			Dim11 = R1;
			Dim12 = Theta1;
			Dim13 = Z1;

			Dim21 = R2;
			Dim22 = Theta2;
			Dim23 = Z2;

			Dim31 = R3;
			Dim32 = Theta3;
			Dim33 = Z3;
		}
		break;

		case 2: // Sphirical
		{

			double R1 = 0.0;
			double R2 = 0.0;
			double R3 = 0.0;

			double Theta1 = 0.0;
			double Theta2 = 0.0;
			double Theta3 = 0.0;

			double Phi1 = 0.0;
			double Phi2 = 0.0;
			double Phi3 = 0.0;

			m_pCurrentFrame->ToSphericalCoord(V1, R1, Theta1, Phi1);
			m_pCurrentFrame->ToSphericalCoord(V2, R2, Theta2, Phi2);
			m_pCurrentFrame->ToSphericalCoord(V3, R3, Theta3, Phi3);


			Dim11 = R1;
			Dim12 = Theta1;
			Dim13 = Phi1;

			Dim21 = R2;
			Dim22 = Theta2;
			Dim23 = Phi2;

			Dim31 = R3;
			Dim32 = Theta3;
			Dim33 = Phi3;
		}
		break;

		default:
			break;
		}

		// Virify for each Node if the 3 Dimensions are in the range
		bool b11 = false, b12 = false, b13 = false;
		bool b21 = false, b22 = false, b23 = false;
		bool b31 = false, b32 = false, b33 = false;

		//================ For Dimension 1 ====================================
		if (Dim11 > Min1 && Dim11 < Max1) // For Vertex 1
			b11 = true;

		if (Dim21 > Min1 && Dim21 < Max1) // For Vertex 2
			b21 = true;

		if (Dim31 > Min1 && Dim31 < Max1) // For Vertex 3
			b31 = true;

		//====================================================================================
		//================ For Dimension 2 ====================================
		if (Dim12 > Min2 && Dim12 < Max2) // For Vertex 1
			b12 = true;

		if (Dim22 > Min2 && Dim22 < Max2) // For Vertex 2
			b22 = true;

		if (Dim32 > Min2 && Dim32 < Max2) // For Vertex 3
			b32 = true;
		//====================================================================================
		//================ For Dimension 3 ====================================
		if (Dim13 > Min3 && Dim13 < Max3) // For Vertex 1
			b13 = true;

		if (Dim23 > Min3 && Dim23 < Max3) // For Vertex 2
			b23 = true;

		if (Dim33 > Min3 && Dim33 < Max3) // For Vertex 3
			b33 = true;
		//====================================================================================

		bool b1 = (b11 && b12 & b13);
		bool b2 = (b21 && b22 & b23);
		bool b3 = (b31 && b32 & b33);

		if (b1 && b2 && b3)
			m_rSelectedElmsIndices.push_back(i);
	}
}


void CDlgRangeSubSet::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateElementSelection();

	// Unselect the reference frames
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();
	CGABESDoc* pDoc = (CGABESDoc*)pMainFrame->GetActiveDocument();
	pDoc->UnselectAllRefFrames();
	
	if (m_rSelectedElmsIndices.empty())
		return CDialogEx::OnOK();

	// Create a new Element Set and add It to the elments Container
	BEM_3D::ElementSubSet* pSubSet = nullptr;
	if(strName.GetLength() == 0)
	    pSubSet = new BEM_3D::ElementSubSet(m_rSelectedElmsIndices); // Automatic naming
	else
		pSubSet = new BEM_3D::ElementSubSet(m_rSelectedElmsIndices, strName);
	m_rModel.GetSubSets().push_back(pSubSet);

	
	// Reset the Selections
	pView->ResetSelections();

	// Update the SubSets Panel
	pMainFrame->GetModelTreePanel().UpdateTreeCtrl();


	CDialogEx::OnOK();
}


void CDlgRangeSubSet::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnCancel();


	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();



	// Reset the Selections
	pView->ResetSelections();

	// Unselect the reference frames
	CGABESDoc* pDoc = (CGABESDoc*)pMainFrame->GetActiveDocument();
	pDoc->UnselectAllRefFrames();
}



void CDlgRangeSubSet::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	UpdateData();

	CSliderCtrl* pSliderBar = (CSliderCtrl*)pScrollBar;

	if (pSliderBar != nullptr)
	{
		double Factor = (double)pSliderBar->GetPos() / 100.0;
		double MIN = 0.0;
		double Range = 0.0;

		if (*pSliderBar == sliderMinDim1)
		{
			//Check the Coordinate System Type
			switch (nCoordSys)
			{
			case 0:    // Cartezian
				Range = m_pCurrentFrame->m_X_max - m_pCurrentFrame->m_X_min;
				MIN = m_pCurrentFrame->m_X_min;
				break;

			case 1:    // Cylindrical
				Range = m_pCurrentFrame->m_Rc_max - m_pCurrentFrame->m_Rc_min;
				MIN = m_pCurrentFrame->m_Rc_min;
				break;

			case 2:    // Spherical
				Range = m_pCurrentFrame->m_Rs_max - m_pCurrentFrame->m_Rs_min;
				MIN = m_pCurrentFrame->m_Rs_min;
				break;
			}

			Min1 = MIN + Range * Factor;
		}
		else if (*pSliderBar == sliderMaxDim1)
		{
			//Check the Coordinate System Type
			switch (nCoordSys)
			{
			case 0:    // Cartezian
				Range = m_pCurrentFrame->m_X_max - m_pCurrentFrame->m_X_min;
				MIN = m_pCurrentFrame->m_X_min;
				break;

			case 1:    // Cylindrical
				Range = m_pCurrentFrame->m_Rc_max - m_pCurrentFrame->m_Rc_min;
				MIN = m_pCurrentFrame->m_Rc_min;
				break;

			case 2:    // Spherical
				Range = m_pCurrentFrame->m_Rs_max - m_pCurrentFrame->m_Rs_min;
				MIN = m_pCurrentFrame->m_Rs_min;
				break;
			}

			Max1 = MIN + Range * Factor;
		}
		else if (*pSliderBar == sliderMinDim2)
		{
			//Check the Coordinate System Type
			switch (nCoordSys)
			{
			case 0:    // Cartezian
				Range = m_pCurrentFrame->m_Y_max - m_pCurrentFrame->m_Y_min;
				MIN = m_pCurrentFrame->m_Y_min;
				break;

			case 1:    // Cylindrical
				Range = 360.0;
				MIN = -180.0;
				break;

			case 2:    // Spherical
				Range = 360.0;
				MIN = -180.0;
				break;
			}

			Min2 = MIN + Range * Factor;
		}
		else if (*pSliderBar == sliderMaxDim2)
		{
			//Check the Coordinate System Type
			switch (nCoordSys)
			{
			case 0:    // Cartezian
				Range = m_pCurrentFrame->m_Y_max - m_pCurrentFrame->m_Y_min;
				MIN = m_pCurrentFrame->m_Y_min;
				break;

			case 1:    // Cylindrical
				Range = 360.0;
				MIN = -180.0;
				break;

			case 2:    // Spherical
				Range = 360.0;
				MIN = -180.0;
				break;
			}

			Max2 = MIN + Range * Factor;
		}
		else if (*pSliderBar == sliderMinDim3)
		{
			//Check the Coordinate System Type
			switch (nCoordSys)
			{
			case 0:    // Cartezian
				Range = m_pCurrentFrame->m_Z_max - m_pCurrentFrame->m_Z_min;
				MIN = m_pCurrentFrame->m_Z_min;
				break;

			case 1:    // Cylindrical
				Range = m_pCurrentFrame->m_Z_max - m_pCurrentFrame->m_Z_min;
				MIN = m_pCurrentFrame->m_Z_min;
				break;

			case 2:    // Spherical
				Range = 180.0;
				MIN = 0.0;
				break;
			}

			Min3 = MIN + Range * Factor;
		}
		else if (*pSliderBar == sliderMaxDim3)
		{
			//Check the Coordinate System Type
			switch (nCoordSys)
			{
			case 0:    // Cartezian
				Range = m_pCurrentFrame->m_Z_max - m_pCurrentFrame->m_Z_min;
				MIN = m_pCurrentFrame->m_Z_min;
				break;

			case 1:    // Cylindrical
				Range = m_pCurrentFrame->m_Z_max - m_pCurrentFrame->m_Z_min;
				MIN = m_pCurrentFrame->m_Z_min;
				break;

			case 2:    // Spherical
				Range = 180.0;
				MIN = 0.0;
				break;
			}

			Max3 = MIN + Range * Factor;
		}

		UpdateData(FALSE);

		UpdateElementSelection();

	}

	
}




void CDlgRangeSubSet::OnBnClickedBtnApply()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	double Range1 = 0.0, Range2 = 0.0, Range3 = 0.0;
	double MIN1 = 0.0, MIN2 = 0.0, MIN3 = 0.0;
	switch (nCoordSys)
	{
	case 0:
		Range1 = m_pCurrentFrame->m_X_max - m_pCurrentFrame->m_X_min;
		Range2 = m_pCurrentFrame->m_Y_max - m_pCurrentFrame->m_Y_min;
		Range3 = m_pCurrentFrame->m_Z_max - m_pCurrentFrame->m_Z_min;
		MIN1 = m_pCurrentFrame->m_X_min;
		MIN2 = m_pCurrentFrame->m_Y_min;
		MIN3 = m_pCurrentFrame->m_Z_min;
		break;

	case 1:
		Range1 = m_pCurrentFrame->m_Rc_max - m_pCurrentFrame->m_Rc_min;
		Range2 = 360.0;
		Range3 = m_pCurrentFrame->m_Z_max - m_pCurrentFrame->m_Z_min;
		MIN1 = m_pCurrentFrame->m_Rc_min;
		MIN2 = -180.0;
		MIN3 = m_pCurrentFrame->m_Z_min;
		break;

	case 2:
		Range1 = m_pCurrentFrame->m_Rs_max - m_pCurrentFrame->m_Rs_min;
		Range2 = 360.0;
		Range3 = 180.0;
		MIN1 = m_pCurrentFrame->m_Rc_min;
		MIN2 = -180.0;
		MIN3 = 0.0;
		break;
	}

	sliderMinDim1.SetPos((Min1 - MIN1) / Range1 * 100);
	sliderMinDim2.SetPos((Min2 - MIN2) / Range2 * 100);
	sliderMinDim3.SetPos((Min3 - MIN3) / Range3 * 100);

	sliderMaxDim1.SetPos((Max1 - MIN1) / Range1 * 100);
	sliderMaxDim2.SetPos((Max2 - MIN2) / Range2 * 100);
	sliderMaxDim3.SetPos((Max3 - MIN3) / Range3 * 100);

	UpdateElementSelection();
}
