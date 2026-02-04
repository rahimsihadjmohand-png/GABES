// DlgReferenceFrame.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgReferenceFrame.h"
#include "MainFrm.h"





// CDlgReferenceFrame dialog

IMPLEMENT_DYNAMIC(CDlgReferenceFrame, CDialogEx)

CDlgReferenceFrame::CDlgReferenceFrame(std::vector<int>& rSelectedElmsIndices, std::vector<BEM_3D::Vertex*>& rSelectedPoints, CWnd* pParent)
	: CDialogEx(IDD_DLG_REFFRAME, pParent)
	, m_pCurrRefFrame(nullptr)
	, m_rSelectedElmsIndices(rSelectedElmsIndices)
	, m_rSelectedPoints(rSelectedPoints)
	, strName(_T(""))
	, Plane(XY)
	, Ox(0)
	, Oy(0)
	, Oz(0)
	, V1x(0)
	, V1y(0)
	, V1z(0)
	, V2x(0)
	, V2y(0)
	, V2z(0)
	, strVec1(_T(""))
	, strVec2(_T(""))
{
}

CDlgReferenceFrame::~CDlgReferenceFrame()
{
}



void CDlgReferenceFrame::UpdateControlsFromCurrentRefFrame()
{

	if (m_pCurrRefFrame)
	{
		strName = m_pCurrRefFrame->m_strName;

		Ox = m_pCurrRefFrame->m_Origin.x;
		Oy = m_pCurrRefFrame->m_Origin.y;
		Oz = m_pCurrRefFrame->m_Origin.z;

		switch (Plane)
		{
		case XY:
			strVec1 = _T("OX Vector");
			strVec2 = _T("OY Vector");
			V1x = m_pCurrRefFrame->m_Vx.x;
			V1y = m_pCurrRefFrame->m_Vx.y;
			V1z = m_pCurrRefFrame->m_Vx.z;

			V2x = m_pCurrRefFrame->m_Vy.x;
			V2y = m_pCurrRefFrame->m_Vy.y;
			V2z = m_pCurrRefFrame->m_Vy.z;
			break;

		case YZ:
			strVec1 = _T("OY Vector");
			strVec2 = _T("OZ Vector");
			V1x = m_pCurrRefFrame->m_Vy.x;
			V1y = m_pCurrRefFrame->m_Vy.y;
			V1z = m_pCurrRefFrame->m_Vy.z;

			V2x = m_pCurrRefFrame->m_Vz.x;
			V2y = m_pCurrRefFrame->m_Vz.y;
			V2z = m_pCurrRefFrame->m_Vz.z;
			break;

		case ZX:
			strVec1 = _T("OZ Vector");
			strVec2 = _T("OX Vector");
			V1x = m_pCurrRefFrame->m_Vz.x;
			V1y = m_pCurrRefFrame->m_Vz.y;
			V1z = m_pCurrRefFrame->m_Vz.z;

			V2x = m_pCurrRefFrame->m_Vx.x;
			V2y = m_pCurrRefFrame->m_Vx.y;
			V2z = m_pCurrRefFrame->m_Vx.z;
			break;
		}
	}
	else
	{
		Ox = 0.0f;
		Oy = 0.0f;
		Oz = 0.0f;

		switch (Plane)
		{
		case XY:
			strVec1 = _T("OX Vector");
			strVec2 = _T("OY Vector");
			V1x = 1.0f;
			V1y = 0.0f;
			V1z = 0.0f;

			V2x = 0.0f;
			V2y = 1.0f;
			V2z = 0.0f;
			break;

		case YZ:
			strVec1 = _T("OY Vector");
			strVec2 = _T("OZ Vector");
			V1x = 0.0f;
			V1y = 1.0f;
			V1z = 0.0f;

			V2x = 0.0f;
			V2y = 0.0f;
			V2z = 1.0f;
			break;

		case ZX:
			strVec1 = _T("OZ Vector");
			strVec2 = _T("OX Vector");
			V1x = 0.0f;
			V1y = 0.0f;
			V1z = 1.0f;

			V2x = 1.0f;
			V2y = 0.0f;
			V2z = 0.0f;
			break;
		}
	}

	UpdateData(FALSE);	
}


void CDlgReferenceFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FRAME_NAME, strName);
	DDX_Text(pDX, IDC_EDIT_OX, Ox);
	DDX_Text(pDX, IDC_EDIT_OY, Oy);
	DDX_Text(pDX, IDC_EDIT_OZ, Oz);
	DDX_Text(pDX, IDC_EDIT_V1X, V1x);
	DDX_Text(pDX, IDC_EDIT_V1Y, V1y);
	DDX_Text(pDX, IDC_EDIT_V1Z, V1z);
	DDX_Text(pDX, IDC_EDIT_V2X, V2x);
	DDX_Text(pDX, IDC_EDIT_V2Y, V2y);
	DDX_Text(pDX, IDC_EDIT_V2Z, V2z);

	if (pDX->m_bSaveAndValidate) // Controls to Data
	{
		// The Current plan
		int nPlane = -1;
		DDX_CBIndex(pDX, IDC_COMBO_PLANE, nPlane);
		Plane = (PLANE)nPlane;
	}
	else // Data to controls
	{
		// The Current plan
		int nPlane = (int)Plane;
		DDX_CBIndex(pDX, IDC_COMBO_PLANE, nPlane);		
	}

	DDX_Control(pDX, IDC_COMBO_PLANE, CmbPlane);
	DDX_Text(pDX, IDC_STATIC_VEC1, strVec1);
	DDX_Text(pDX, IDC_STATIC_VEC2, strVec2);
}


BEGIN_MESSAGE_MAP(CDlgReferenceFrame, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgReferenceFrame::OnBnClickedButtonApply)
	ON_CBN_SELCHANGE(IDC_COMBO_PLANE, &CDlgReferenceFrame::OnCbnSelchangeComboPlane)
	ON_BN_CLICKED(IDC_BTN_ORIGIN_PICK_SEL_POINT, &CDlgReferenceFrame::OnBnClickedBtnOriginPickSelPoint)
	ON_BN_CLICKED(IDC_BTN_ORIGIN_PICK_CENTER_3PTS, &CDlgReferenceFrame::OnBnClickedBtnOriginPickCenter3pts)
	ON_BN_CLICKED(IDC_BTN_VECTOR1_PICK_SEL_POINT, &CDlgReferenceFrame::OnBnClickedBtnVector1PickSelPoint)
	ON_BN_CLICKED(IDC_BTN_VECTOR1_PICK_SELECTED_ELEMENT_NORMAL, &CDlgReferenceFrame::OnBnClickedBtnVector1PickSelectedElementNormal)
	ON_BN_CLICKED(IDC_BTN_VECTOR2_PICK_SEL_POINT, &CDlgReferenceFrame::OnBnClickedBtnVector2PickSelPoint)
	ON_BN_CLICKED(IDC_BTN_VECTOR2_PICK_SELECTED_ELEMENT_NORMAL, &CDlgReferenceFrame::OnBnClickedBtnVector2PickSelectedElementNormal)
END_MESSAGE_MAP()


// CDlgReferenceFrame message handlers



void CDlgReferenceFrame::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here

	if (m_pCurrRefFrame == nullptr)
		return;

	// Store the Previous State
	m_PrevStateRefFrame = *m_pCurrRefFrame;


	UpdateData();
	
	m_pCurrRefFrame->m_strName = strName;



	m_pCurrRefFrame->m_Origin.x = Ox;
	m_pCurrRefFrame->m_Origin.y = Oy;
	m_pCurrRefFrame->m_Origin.z = Oz;


	BEM_3D::Vector& Vx = m_pCurrRefFrame->m_Vx;
	BEM_3D::Vector& Vy = m_pCurrRefFrame->m_Vy;
	BEM_3D::Vector& Vz = m_pCurrRefFrame->m_Vz;


	switch (Plane)
	{
	case XY: // OXY plane
	{
		Vx.Set(V1x, V1y, V1z);
		Vy.Set(V2x, V2y, V2z);

		Vz = Vx ^ Vy;
	}
	break;

	case YZ: // OYZ plane
	{
		Vy.Set(V1x, V1y, V1z);
		Vz.Set(V2x, V2y, V2z);

		Vx = Vy ^ Vz;
	}
	break;

	case ZX: // OZX plane
	{
		Vz.Set(V1x, V1y, V1z);
		Vx.Set(V2x, V2y, V2z);

		Vy = Vz ^ Vx;
	}
	break;
	}

	if (!m_pCurrRefFrame->IsValid())
	{
		MessageBox(_T("The configuration is not valid!!"));
		*m_pCurrRefFrame = m_PrevStateRefFrame;
		UpdateControlsFromCurrentRefFrame();
	}


	// Update the Tree Control
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->GetModelTreePanel().UpdateTreeCtrl();
}

void CDlgReferenceFrame::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// Apply the changes
	OnBnClickedButtonApply();


	// If the frame is new add it
	std::vector<BEM_3D::ReferenceFrame*>& rRefFrames = pDoc->m_ReferenceFrames;

	if (std::find(rRefFrames.begin(), rRefFrames.end(), m_pCurrRefFrame) == rRefFrames.end())  // New frame
	{
		m_pCurrRefFrame->m_bSelected = false;

		m_pCurrRefFrame->SetCoordinateRanges(pDoc->m_Model);

		rRefFrames.push_back(m_pCurrRefFrame);
		m_pCurrRefFrame = nullptr;

		// Update the Tree Control
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->GetModelTreePanel().UpdateTreeCtrl();
	}
	else // Modify existing frame
	{
		m_pCurrRefFrame->m_bSelected = false;
		m_pCurrRefFrame->SetCoordinateRanges(pDoc->m_Model);
		m_pCurrRefFrame = nullptr;
	}


	CDialogEx::OnOK();
}

void CDlgReferenceFrame::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	// If the frame is new delete it
	std::vector<BEM_3D::ReferenceFrame*>& rRefFrames = pDoc->m_ReferenceFrames;

	if (std::find(rRefFrames.begin(), rRefFrames.end(), m_pCurrRefFrame) == rRefFrames.end())
	{
		delete m_pCurrRefFrame;
		m_pCurrRefFrame = nullptr;
	}


	CDialogEx::OnCancel();
}

void CDlgReferenceFrame::OnCbnSelchangeComboPlane()
{
	// TODO: Add your control notification handler code here
	Plane = (PLANE)CmbPlane.GetCurSel();

	UpdateControlsFromCurrentRefFrame();
}

void CDlgReferenceFrame::OnBnClickedBtnOriginPickSelPoint()
{
	// Check if there is only 1 slected point

	if (m_rSelectedPoints.size() != 1)
		return;

	BEM_3D::Vertex* pV = m_rSelectedPoints[0];

	m_pCurrRefFrame->m_Origin = *pV;

	UpdateControlsFromCurrentRefFrame();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// Reset the Selections
	pView->ResetSelections();
}

void CDlgReferenceFrame::OnBnClickedBtnOriginPickCenter3pts()
{
	// TODO: Add your control notification handler code here

	// Check if there are only 3 slected point

	if (m_rSelectedPoints.size() != 3)
		return;

	BEM_3D::Vertex& V1 = *m_rSelectedPoints[0];
	BEM_3D::Vertex& V2 = *m_rSelectedPoints[1];
	BEM_3D::Vertex& V3 = *m_rSelectedPoints[2];


	// Get the Circumcenter
	BEM_3D::Vector a = V2 - V1;
	BEM_3D::Vector b = V3 - V1;

	

	// Claculate the non reduced normal vector
	BEM_3D::Vector n = a ^ b;

	// Calculate the Squared magnitudes
	double a2 = a.Magnitude(); a2 = a2 * a2;
	double b2 = b.Magnitude(); b2 = b2 * b2;
	double n2 = n.Magnitude(); n2 = n2 * n2;

	BEM_3D::Vertex C = V1 + ((b ^ n) * a2 + (n ^ a) * b2) / (2.0 * n2);

	m_pCurrRefFrame->m_Origin = C;

	UpdateControlsFromCurrentRefFrame();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// Reset the Selections
	pView->ResetSelections();
}

void CDlgReferenceFrame::OnBnClickedBtnVector1PickSelPoint()
{
	// TODO: Add your control notification handler code here
	// Check if there is only 1 slected point

	if (m_rSelectedPoints.size() != 1)
		return;

	const BEM_3D::Vertex& V = *m_rSelectedPoints[0];

	// Calculate a normalized Vector
	BEM_3D::Vector Vec1 = V - m_pCurrRefFrame->m_Origin;
	Vec1.Normalize();


	BEM_3D::Vector& Vx = m_pCurrRefFrame->m_Vx;
	BEM_3D::Vector& Vy = m_pCurrRefFrame->m_Vy;
	BEM_3D::Vector& Vz = m_pCurrRefFrame->m_Vz;

	switch (Plane)
	{
	case XY:
		Vx = Vec1;
		Vz = Vx ^ Vy;
		break;

	case YZ:
		Vy = Vec1;
		Vx = Vy ^ Vx;
		break;

	case ZX:
		Vz = Vec1;
		Vy = Vz ^ Vx;
		break;
	}

	UpdateControlsFromCurrentRefFrame();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// Reset the Selections
	pView->ResetSelections();
}

void CDlgReferenceFrame::OnBnClickedBtnVector1PickSelectedElementNormal()
{
	// TODO: Add your control notification handler code here
	// Check if there is only 1 slected point

	if (m_rSelectedElmsIndices.size() != 1)
		return;

	const std::vector<BEM_3D::Element*>& rElmsContainer = pDoc->m_Model.GetElements();
	int Idx = m_rSelectedElmsIndices[0];
	const BEM_3D::Element& Elm = *rElmsContainer[Idx];

	// Set the Element normal as the vector
	BEM_3D::Vector Vec1 = Elm.m_n;


	BEM_3D::Vector& Vx = m_pCurrRefFrame->m_Vx;
	BEM_3D::Vector& Vy = m_pCurrRefFrame->m_Vy;
	BEM_3D::Vector& Vz = m_pCurrRefFrame->m_Vz;

	switch (Plane)
	{
	case XY:
		Vx = Vec1;
		Vz = Vx ^ Vy;
		break;

	case YZ:
		Vy = Vec1;
		Vx = Vy ^ Vx;
		break;

	case ZX:
		Vz = Vec1;
		Vy = Vz ^ Vx;
		break;
	}

	UpdateControlsFromCurrentRefFrame();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// Reset the Selections
	pView->ResetSelections();
}

void CDlgReferenceFrame::OnBnClickedBtnVector2PickSelPoint()
{
	// TODO: Add your control notification handler code here
	// Check if there is only 1 slected point

	if (m_rSelectedPoints.size() != 1)
		return;

	const BEM_3D::Vertex& V = *m_rSelectedPoints[0];

	// Calculate a normalized Vector
	BEM_3D::Vector Vec2 = V - m_pCurrRefFrame->m_Origin;
	Vec2.Normalize();


	BEM_3D::Vector& Vx = m_pCurrRefFrame->m_Vx;
	BEM_3D::Vector& Vy = m_pCurrRefFrame->m_Vy;
	BEM_3D::Vector& Vz = m_pCurrRefFrame->m_Vz;

	switch (Plane)
	{
	case XY:
		Vy = Vec2;
		Vz = Vx ^ Vy;
		break;

	case YZ:
		Vz = Vec2;
		Vx = Vy ^ Vx;
		break;

	case ZX:
		Vx = Vec2;
		Vy = Vz ^ Vx;
		break;
	}

	UpdateControlsFromCurrentRefFrame();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// Reset the Selections
	pView->ResetSelections();
}

void CDlgReferenceFrame::OnBnClickedBtnVector2PickSelectedElementNormal()
{
	// TODO: Add your control notification handler code here
	// Check if there is only 1 slected point

	if (m_rSelectedElmsIndices.size() != 1)
		return;

	const std::vector<BEM_3D::Element*>& rElmsContainer = pDoc->m_Model.GetElements();
	int Idx = m_rSelectedElmsIndices[0];
	const BEM_3D::Element& Elm = *rElmsContainer[Idx];

	// Set the Element normal as the vector
	BEM_3D::Vector Vec2 = Elm.m_n;


	BEM_3D::Vector& Vx = m_pCurrRefFrame->m_Vx;
	BEM_3D::Vector& Vy = m_pCurrRefFrame->m_Vy;
	BEM_3D::Vector& Vz = m_pCurrRefFrame->m_Vz;

	switch (Plane)
	{
	case XY:
		Vy = Vec2;
		Vz = Vx ^ Vy;
		break;

	case YZ:
		Vz = Vec2;
		Vx = Vy ^ Vx;
		break;

	case ZX:
		Vx = Vec2;
		Vy = Vz ^ Vx;
		break;
	}

	UpdateControlsFromCurrentRefFrame();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// Reset the Selections
	pView->ResetSelections();
}
