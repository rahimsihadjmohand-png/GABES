// DlgGeneratePointGrid.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgGeneratePointGrid.h"


// CDlgGeneratePointGrid dialog

IMPLEMENT_DYNAMIC(CDlgGeneratePointGrid, CDialogEx)

CDlgGeneratePointGrid::CDlgGeneratePointGrid(std::vector<BEM_3D::Vertex>& rTrackNodes, CWnd* pParent)
	: CDialogEx(IDD_DLG_GENERATE_POINT_GRID, pParent)
	, m_pCurrentFrame(nullptr)
	, m_rTrackNodes(rTrackNodes)
	, m_nCoordSys(0)
	, strDim1Range(_T("Dim1 Range"))
	, strDim2Range(_T("Dim2 Range"))
	, strDim3Range(_T("Dim3 Range"))
	, strDim1Nbr(_T(""))
	, strDim2Nbr(_T(""))
	, strDim3Nbr(_T(""))
	, Min1(0)
	, Max1(0)
	, Min2(0)
	, Max2(0)
	, Min3(0)
	, Max3(0)
	, N1(1)
	, N2(1)
	, N3(1)
	, strCoordFrame(_T(""))
	, strCoordSys(_T(""))
	
{

}

CDlgGeneratePointGrid::~CDlgGeneratePointGrid()
{
}

void CDlgGeneratePointGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_DIM1, strDim1Range);
	DDX_Text(pDX, IDC_STATIC_DIM2, strDim2Range);
	DDX_Text(pDX, IDC_STATIC_DIM3, strDim3Range);
	DDX_Text(pDX, IDC_STATIC_NBR_PTS_DIM1, strDim1Nbr);
	DDX_Text(pDX, IDC_STATIC_NBR_PTS_DIM2, strDim2Nbr);
	DDX_Text(pDX, IDC_STATIC_NBR_PTS_DIM3, strDim3Nbr);

	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, IDC_EDIT_MIN_DIM1, Min1);
		DDX_Text(pDX, IDC_EDIT_MAX_DIM1, Max1);
		DDX_Text(pDX, IDC_EDIT_MIN_DIM2, Min2);
		DDX_Text(pDX, IDC_EDIT_MAX_DIM2, Max2);
		DDX_Text(pDX, IDC_EDIT_MIN_DIM3, Min3);
		DDX_Text(pDX, IDC_EDIT_MAX_DIM3, Max3);
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

		F(IDC_EDIT_MIN_DIM1, Min1);
		F(IDC_EDIT_MAX_DIM1, Max1);
		F(IDC_EDIT_MIN_DIM2, Min2);
		F(IDC_EDIT_MAX_DIM2, Max2);
		F(IDC_EDIT_MIN_DIM3, Min3);
		F(IDC_EDIT_MAX_DIM3, Max3);
	}
	
	
	
	DDX_Text(pDX, IDC_EDIT_N1, N1);
	DDX_Text(pDX, IDC_EDIT_N2, N2);
	DDX_Text(pDX, IDC_EDIT_N3, N3);
	DDX_Control(pDX, IDC_SLIDER_MIN_DIM1, sliderMinDim1);
	DDX_Control(pDX, IDC_SLIDER_MAX_DIM1, sliderMaxDim1);
	DDX_Control(pDX, IDC_SLIDER_MIN_DIM2, sliderMinDim2);
	DDX_Control(pDX, IDC_SLIDER_MAX_DIM2, sliderMaxDim2);
	DDX_Control(pDX, IDC_SLIDER_MIN_DIM3, sliderMinDim3);
	DDX_Control(pDX, IDC_SLIDER_MAX_DIM3, sliderMaxDim3);
	DDX_Text(pDX, IDC_EDIT_COORD_FRAME, strCoordFrame);
	DDX_Text(pDX, IDC_EDIT_COORD_SYS, strCoordSys);
	DDX_Control(pDX, IDC_SPIN1, spinN1);
	DDX_Control(pDX, IDC_SPIN2, spinN2);
	DDX_Control(pDX, IDC_SPIN3, spinN3);
}



void CDlgGeneratePointGrid::InitModlessDialog(BEM_3D::ReferenceFrame* pCurrentFrame, int nCoordSys)
{
	m_pCurrentFrame = pCurrentFrame;
	m_nCoordSys = nCoordSys;


	strCoordFrame = m_pCurrentFrame->m_strName;


	switch (nCoordSys)
	{
	case 0:
		strCoordSys = _T("Cartesian");

		strDim1Range = _T("X Range:");
		strDim2Range = _T("Y Range:");
		strDim3Range = _T("Z Range:");

		strDim1Nbr = _T("Nbr Pts Along X:");
		strDim2Nbr = _T("Nbr Pts Along Y:");
		strDim3Nbr = _T("Nbr Pts Along Z:");

		Min1 = m_pCurrentFrame->m_X_min;
		Min2 = m_pCurrentFrame->m_Y_min;
		Min3 = m_pCurrentFrame->m_Z_min;

		Max1 = m_pCurrentFrame->m_X_max;
		Max2 = m_pCurrentFrame->m_Y_max;
		Max3 = m_pCurrentFrame->m_Z_max;		
		break;

	case 1:
		strCoordSys = _T("Cylindrical");

		strDim1Range = _T("Radius (R) Range:");
		strDim2Range = _T("Angle (Theta) Range:");
		strDim3Range = _T("Z Range:");

		strDim1Nbr = _T("Nbr Pts Along R:");
		strDim2Nbr = _T("Nbr Pts Along Theta:");
		strDim3Nbr = _T("Nbr Pts Along Z:");


		Min1 = m_pCurrentFrame->m_Rc_min;
		Min2 = -180.0;
		Min3 = m_pCurrentFrame->m_Z_min;

		Max1 = m_pCurrentFrame->m_Rc_max;
		Max2 = 180.0;
		Max3 = m_pCurrentFrame->m_Z_max;
		break;

	case 2:
		strCoordSys = _T("Spherical");

		strDim1Range = _T("Radius (R) Range:");
		strDim2Range = _T("Angle (Theta) Range:");
		strDim3Range = _T("Angle (Phi) Range:");

		strDim1Nbr = _T("Nbr Pts Along R:");
		strDim2Nbr = _T("Nbr Pts Along Theta:");
		strDim3Nbr = _T("Nbr Pts Along Phi:");

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

	N1 = 1;
	N2 = 1;
	N3 = 1;

	UpdateData(FALSE);

	UpdateSliderBars();
	UpdateTrackNodes();
}









BEGIN_MESSAGE_MAP(CDlgGeneratePointGrid, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgGeneratePointGrid::OnBnClickedButtonApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgGeneratePointGrid::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgGeneratePointGrid::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDlgGeneratePointGrid::OnDeltaposSpin3)
END_MESSAGE_MAP()



// CDlgGeneratePointGrid message handlers

BOOL CDlgGeneratePointGrid::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

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

	spinN1.SetRange(1, SHORT_MAX);
	spinN2.SetRange(1, SHORT_MAX);
	spinN3.SetRange(1, SHORT_MAX);

	spinN1.SetPos(1);
	spinN2.SetPos(1);
	spinN3.SetPos(1);

	UpdateData();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgGeneratePointGrid::UpdateTrackNodes()
{
	UpdateData();

	// Empty the track nodes container
	m_rTrackNodes.clear();

	// Converte angles to Radians
	switch (m_nCoordSys)
	{
	case 0:     // Cartesian (No angles to convert
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


	// Start to construct the Grid
	switch (m_nCoordSys)
	{
	case 0: // Cartesian
	{
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		double dx = (N1 <= 1) ? 0.0 : (Max1 - Min1) / (N1 - 1);
		double dy = (N2 <= 1) ? 0.0 : (Max2 - Min2) / (N2 - 1);
		double dz = (N3 <= 1) ? 0.0 : (Max3 - Min3) / (N3 - 1);

		// Dimension 1
		for (int i = 0; i < N1; i++)
		{
			x = Min1 + i * dx;
			// Dimension 2
			for (int j = 0; j < N2; j++)
			{
				y = Min2 + j * dy;
				// Dimension 3
				for (int k = 0; k < N3; k++)
				{
					z = Min3 + k * dz;

					BEM_3D::Vertex V(x, y, z);

					m_pCurrentFrame->ToGlobalCoord(V);
					m_rTrackNodes.push_back(V);
				}
			}

		}
		
		
	}
	break;

	case 1: // Cylindrical
	{
		double R = 0.0;
		double Theta = 0.0;
		double z = 0.0;

		double dR = (N1 <= 1) ? 0.0 : (Max1 - Min1) / (N1 - 1);
		double dTheta = (N2 <= 1) ? 0.0 : (Max2 - Min2) / (N2 - 1);
		double dz = (N3 <= 1) ? 0.0 : (Max3 - Min3) / (N3 - 1);

		// Dimension 1
		for (int i = 0; i < N1; i++)
		{
			R = Min1 + i * dR;
			// Dimension 2
			for (int j = 0; j < N2; j++)
			{
				Theta = Min2 + j * dTheta;
				// Dimension 3
				for (int k = 0; k < N3; k++)
				{
					z = Min3 + k * dz;

					double x = R * cos(Theta);
					double y = R * sin(Theta);
					

					BEM_3D::Vertex V(x, y, z);

					m_pCurrentFrame->ToGlobalCoord(V);
					m_rTrackNodes.push_back(V);
				}
			}

		}
	}
	break;

	case 2: // Spherical
	{
		double R = 0.0;
		double Theta = 0.0;
		double Phi = 0.0;

		double dR = (N1 <= 1) ? 0.0 : (Max1 - Min1) / (N1 - 1);
		double dTheta = (N2 <= 1) ? 0.0 : (Max2 - Min2) / (N2 - 1);
		double dPhi = (N3 <= 1) ? 0.0 : (Max3 - Min3) / (N3 - 1);

		// Dimension 1
		for (int i = 0; i < N1; i++)
		{
			R = Min1 + i * dR;
			// Dimension 2
			for (int j = 0; j < N2; j++)
			{
				Theta = Min2 + j * dTheta;
				// Dimension 3
				for (int k = 0; k < N3; k++)
				{
					Phi = Min3 + k * dPhi;

					double x = R * sin(Phi) * cos(Theta);
					double y = R * sin(Phi) * sin(Theta);
					double z = R * cos(Phi);

					BEM_3D::Vertex V(x, y, z);

					m_pCurrentFrame->ToGlobalCoord(V);
					m_rTrackNodes.push_back(V);
				}
			}

		}
	}
	break;
	}


	// Reconverte angles to Degrees
	switch (m_nCoordSys)
	{
	case 0:     // Cartesian (No angles to convert
		break;

	case 1:    // Cylindrical
		Min2 = Min2 * 180.0 / M_PI;
		Max2 = Max2 * 180.0 / M_PI;
		break;

	case 2:    // Sphirical
		Min2 = Min2 * 180.0 / M_PI;
		Max2 = Max2 * 180.0 / M_PI;

		Min3 = Min3 * 180.0 / M_PI;
		Max3 = Max3 * 180.0 / M_PI;
		break;

	default:
		break;
	}
}
void CDlgGeneratePointGrid::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	m_rTrackNodes.clear();

	CDialogEx::OnCancel();
}

void CDlgGeneratePointGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			switch (m_nCoordSys)
			{
			case 0:    // Cartesian
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
			switch (m_nCoordSys)
			{
			case 0:    // Cartesian
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
			switch (m_nCoordSys)
			{
			case 0:    // Cartesian
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
			switch (m_nCoordSys)
			{
			case 0:    // Cartesian
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
			switch (m_nCoordSys)
			{
			case 0:    // Cartesian
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
			switch (m_nCoordSys)
			{
			case 0:    // Cartesian
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

		UpdateTrackNodes();

	}
}

void CDlgGeneratePointGrid::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	UpdateSliderBars();

	UpdateTrackNodes();
}



void CDlgGeneratePointGrid::UpdateSliderBars()
{
	double Range1 = 0.0, Range2 = 0.0, Range3 = 0.0;
	double MIN1 = 0.0, MIN2 = 0.0, MIN3 = 0.0;
	switch (m_nCoordSys)
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
}


void CDlgGeneratePointGrid::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	N1 = max(1, pNMUpDown->iPos +  pNMUpDown->iDelta);

	UpdateData(FALSE);

	UpdateTrackNodes();

	*pResult = 0;
}

void CDlgGeneratePointGrid::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	N2 = max(1, pNMUpDown->iPos + pNMUpDown->iDelta);

	UpdateData(FALSE);

	UpdateTrackNodes();

	*pResult = 0;
}

void CDlgGeneratePointGrid::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);  // read current edit value

	N3 = max(1, pNMUpDown->iPos + pNMUpDown->iDelta);

	UpdateData(FALSE);

	UpdateTrackNodes();

	*pResult = 0;
}
