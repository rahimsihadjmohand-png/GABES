// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// GABESView.cpp : implementation of the CGABESView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GABES.h"
#endif

#include "GABESDoc.h"
#include "GABESView.h"
#include "MainFrm.h"
#include "DlgPartialEncaster.h"
#include "DlgDisplacementVector.h"
#include "DlgNormalDisplacement.h"
#include "DlgTractionVector.h"
#include "DlgNormalTraction.h"
#include "DlgPressure.h"
#include "DlgSetMaterialProperties.h"
#include "DlgSetIntegrationParameters.h"
#include "DlgSetDeformationScale.h"
#include "DlgWaitLengthyOperation.h"
#include "DlgPaletteSettings.h"
#include "DlgSetMeshType.h"
#include "DlgSetRamQuota.h"
#include "DlgSolverParameters.h"
#include "DlgDomainNodeResponse.h"
#include "DlgGenerateDomainResponseReport.h"
#include "ReferenceFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CGABESView

IMPLEMENT_DYNCREATE(CGABESView, CView)

BEGIN_MESSAGE_MAP(CGABESView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGABESView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_LOAD_MESH_FILE, &CGABESView::OnFileLoadMeshFromFile)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_FRONT, &CGABESView::OnViewFront)
	ON_COMMAND(ID_VIEW_LEFT, &CGABESView::OnViewLeft)
	ON_COMMAND(ID_VIEW_TOP, &CGABESView::OnViewTop)
	ON_COMMAND(ID_VIEW_BACK, &CGABESView::OnViewBack)
	ON_COMMAND(ID_VIEW_RIGHT, &CGABESView::OnViewRight)
	ON_COMMAND(ID_VIEW_BOTTOM, &CGABESView::OnViewBottom)
	ON_COMMAND(ID_VIEW_MOUSEACTION_ROTATE, &CGABESView::OnViewMouseactionRotate)
	ON_COMMAND(ID_VIEW_MOUSEACTION_ELEMENT_SELECTION, &CGABESView::OnViewMouseactionElementSelection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOUSEACTION_ROTATE, &CGABESView::OnUpdateViewMouseactionRotate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOUSEACTION_ELEMENT_SELECTION, &CGABESView::OnUpdateViewMouseactionElementSelection)
	ON_COMMAND(ID_MODELING_CREATE_ELEMENT_SET_FROM_SELECTION, &CGABESView::OnModelingCreateElementSetFromSelection)
	ON_UPDATE_COMMAND_UI(ID_MODELING_CREATE_ELEMENT_SET_FROM_SELECTION, &CGABESView::OnUpdateModelingCreateElementSetFromSelection)
	ON_COMMAND(ID_MODELING_IMPORT_ELEMENT_SET_FROM_OBJ_FILE, &CGABESView::OnModelingImportElementSetFromObjFile)
	ON_COMMAND(ID_FILE_DELETE_MODEL, &CGABESView::OnFileDeleteModel)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE_MODEL, &CGABESView::OnUpdateFileDeleteModel)
	ON_UPDATE_COMMAND_UI(ID_MODELING_IMPORT_ELEMENT_SET_FROM_OBJ_FILE, &CGABESView::OnUpdateModelingImportElementSetFromObjFile)
	ON_COMMAND(ID_MODELING_CREATE_RANGE_ELEMENT_SET, &CGABESView::OnModelingCreateRangeElementSet)
	ON_UPDATE_COMMAND_UI(ID_MODELING_CREATE_RANGE_ELEMENT_SET, &CGABESView::OnUpdateModelingCreateRangeElementSet)
	ON_COMMAND(ID_BCS_FULLY_ENCASTER, &CGABESView::OnBcsFullyEncaster)
	ON_COMMAND(ID_BCS_PARTIAL_ENCASTREMENT, &CGABESView::OnBcsPartialEncastrement)
	ON_COMMAND(ID_BCS_DISPLACEMENT_VECTOR, &CGABESView::OnBcsDisplacementVector)
	ON_COMMAND(ID_BCS_NORMAL_DISPLACEMENT, &CGABESView::OnBcsNormalDisplacement)
	ON_COMMAND(ID_BCS_TRACTION_VECTOR, &CGABESView::OnBcsTractionVector)
	ON_COMMAND(ID_BCS_NORMAL_TRACTION, &CGABESView::OnBcsNormalTraction)
	ON_COMMAND(ID_BCS_PRESSURE, &CGABESView::OnBcsPressure)
	ON_UPDATE_COMMAND_UI(ID_BCS_FULLY_ENCASTER, &CGABESView::OnUpdateBcsFullyEncaster)
	ON_UPDATE_COMMAND_UI(ID_BCS_PARTIAL_ENCASTREMENT, &CGABESView::OnUpdateBcsPartialEncastrement)
	ON_UPDATE_COMMAND_UI(ID_BCS_DISPLACEMENT_VECTOR, &CGABESView::OnUpdateBcsDisplacementVector)
	ON_UPDATE_COMMAND_UI(ID_BCS_NORMAL_DISPLACEMENT, &CGABESView::OnUpdateBcsNormalDisplacement)
	ON_UPDATE_COMMAND_UI(ID_BCS_TRACTION_VECTOR, &CGABESView::OnUpdateBcsTractionVector)
	ON_UPDATE_COMMAND_UI(ID_BCS_NORMAL_TRACTION, &CGABESView::OnUpdateBcsNormalTraction)
	ON_UPDATE_COMMAND_UI(ID_BCS_PRESSURE, &CGABESView::OnUpdateBcsPressure)
	ON_COMMAND(ID_BEM_SOLVE_BOUNDARY_VALUE_PROBLEM, &CGABESView::OnBemSolveBoundaryValueProblem)
	ON_COMMAND(ID_BEM_SHOW_DEFORMATION, &CGABESView::OnBemShowDeformation)
	ON_UPDATE_COMMAND_UI(ID_BEM_SHOW_DEFORMATION, &CGABESView::OnUpdateBemShowDeformation)
	ON_COMMAND(ID_BEM_SET_MATERIAL_PROPERTIES, &CGABESView::OnBemSetMaterialProperties)
	ON_COMMAND(ID_BEM_SET_INTEGRATION_PARAMETERS, &CGABESView::OnBemSetIntegrationParameters)
	ON_COMMAND(ID_BEM_SET_DEFORMATION_SCALE, &CGABESView::OnBemSetDeformationScale)
	ON_COMMAND(ID_BEM_POSTTREATMENT_MODE, &CGABESView::OnBemPosttreatmentMode)
	ON_UPDATE_COMMAND_UI(ID_BEM_POSTTREATMENT_MODE, &CGABESView::OnUpdateBemPosttreatmentMode)
	ON_UPDATE_COMMAND_UI(ID_BEM_SET_DEFORMATION_SCALE, &CGABESView::OnUpdateBemSetDeformationScale)
	ON_COMMAND(ID_BCS_CHECK_SHOW_FIXATIONS, &CGABESView::OnBcsCheckShowFixations)
	ON_COMMAND(ID_BCS_CHECK_SHOW_DISPLACEMENT_VECTORS, &CGABESView::OnBcsCheckShowDisplacementVectors)
	ON_COMMAND(ID_BCS_CHECK_SHOW_TRACTION_VECTORS, &CGABESView::OnBcsCheckShowTractionVectors)
	ON_UPDATE_COMMAND_UI(ID_BCS_CHECK_SHOW_FIXATIONS, &CGABESView::OnUpdateBcsCheckShowFixations)
	ON_UPDATE_COMMAND_UI(ID_BCS_CHECK_SHOW_DISPLACEMENT_VECTORS, &CGABESView::OnUpdateBcsCheckShowDisplacementVectors)
	ON_UPDATE_COMMAND_UI(ID_BCS_CHECK_SHOW_TRACTION_VECTORS, &CGABESView::OnUpdateBcsCheckShowTractionVectors)
	ON_WM_CREATE()
	ON_COMMAND(ID_BEM_PALETTE_SETTINGS, &CGABESView::OnBemPaletteSettings)
	ON_COMMAND(ID_BEM_SET_MEMORY_BAIL, &CGABESView::OnBemSetMemoryQuota)
	ON_COMMAND(ID_BEM_GET_DOMAIN_NODE_RESPONSE, &CGABESView::OnBemGetDomainNodeResponse)
	ON_COMMAND(ID_BEM_GENERATE_REPORT, &CGABESView::OnBemGenerateReport)
	ON_COMMAND(ID_MODELING_ADD_FRAME, &CGABESView::OnModelingAddFrame)
	ON_COMMAND(ID_VIEW_MOUSEACTION_POINT_SELECTION, &CGABESView::OnViewMouseactionPointSelection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOUSEACTION_POINT_SELECTION, &CGABESView::OnUpdateViewMouseactionPointSelection)
	ON_COMMAND(ID_VIEW_ISOMETRIC, &CGABESView::OnViewIsometric)
	ON_COMMAND(ID_BCS_TRACTION_FREE, &CGABESView::OnBcsTractionFree)
	ON_UPDATE_COMMAND_UI(ID_BCS_TRACTION_FREE, &CGABESView::OnUpdateBcsTractionFree)
END_MESSAGE_MAP()

// CGABESView construction/destruction

CGABESView::CGABESView() noexcept
	: m_pD3ddev(nullptr)
	, pModel(nullptr)
	, m_fNear(1.0f)
	, m_fFar(5000.0f)
	, m_CameraDistance(100.0f)
	, m_Theta(0.0f)
	, m_Phi(D3DX_PI / 2.0f)
	, m_XRotAng(0.0f)
	, m_YRotAng(0.0f)
	, m_Width(0.0f)
	, m_Height(0.0f)
	, m_W2(0.0f)
	, m_H2(0.0f)
	, m_FillMode(BEM_3D::EDGED_FACES)
	, m_MouseAction(NONE)
	, m_ShowPointsMode(BEM_3D::HIDE_POINTS)
	, m_HitTestedElmIdx(-1)
	, m_pHitTestedPoint(nullptr)
	, m_pSelectedSubSet(nullptr)
	, m_bPostTreatment(false)
	, m_DlgReferenceFrame(m_SelectedElmsIndices, m_SelectedPoints, nullptr)
	, m_pDlgRangeSubSet(nullptr)
	, m_pTempRefFrame(nullptr)
{
	// TODO: add construction code here

}

CGABESView::~CGABESView()
{
	CleanUpD3D();

	if (m_pDlgRangeSubSet)
	{
		delete m_pDlgRangeSubSet;
		m_pDlgRangeSubSet = nullptr;
	}
}

BOOL CGABESView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGABESView drawing

void CGABESView::OnDraw(CDC* /*pDC*/)
{
	CGABESDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	RenderFrameD3D();
}


// CGABESView printing


void CGABESView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGABESView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGABESView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGABESView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGABESView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGABESView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGABESView diagnostics

#ifdef _DEBUG
void CGABESView::AssertValid() const
{
	CView::AssertValid();
}

void CGABESView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGABESDoc* CGABESView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGABESDoc)));
	return (CGABESDoc*)m_pDocument;
}
#endif //_DEBUG


// CGABESView message handlers

void CGABESView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	
	// Initialize Direct 3D
	/*CRect rcClient;
	GetClientRect(rcClient);
	InitializeD3D(rcClient.Width(), rcClient.Height());*/

	// Get a reference to the document and Model object
	CGABESDoc* pDoc = GetDocument();
	pModel = &pDoc->m_Model;
	m_PaletteCtrl.SetModel(pModel);
	m_DlgReferenceFrame.pDoc = pDoc;

	// Create the RangeSubSet Modless Dialog Box
	m_pDlgRangeSubSet = new CDlgRangeSubSet(pDoc->m_GlobalFrame, pDoc->m_ReferenceFrames, m_SelectedElmsIndices, pDoc->m_Model, this);
	m_pDlgRangeSubSet->Create(IDD_DLG_RANGE_SUBSET, this);
}


void CGABESView::InitializeD3D()
{
	if (m_pD3ddev)
		m_pD3ddev->Release();
	
	IDirect3D9* pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	

	CRect rcClient;
	GetClientRect(&rcClient);


	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = rcClient.Width();
	d3dpp.BackBufferHeight = rcClient.Height();
	d3dpp.hDeviceWindow = GetSafeHwnd();
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = true;



	HRESULT hr = pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetSafeHwnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3ddev);


	pD3d->Release();

	// Load the Gizmo Mesh which is a static variable for the ReferenceFrame class
	BEM_3D::ReferenceFrame::LoadGizmo(m_pD3ddev);
}







void CGABESView::CleanUpD3D()
{
	if (m_pD3ddev)
	{
		m_pD3ddev->Release();
		m_pD3ddev = nullptr;
	}


	// Clean up the Static Gizmo Mesh data for the Reference frame class
	if (BEM_3D::ReferenceFrame::m_pMeshXAxis)
	{
		BEM_3D::ReferenceFrame::m_pMeshXAxis->Release();
		BEM_3D::ReferenceFrame::m_pMeshXAxis = nullptr;
	}

	if (BEM_3D::ReferenceFrame::m_pMeshYAxis)
	{
		BEM_3D::ReferenceFrame::m_pMeshYAxis->Release();
		BEM_3D::ReferenceFrame::m_pMeshYAxis = nullptr;
	}

	if (BEM_3D::ReferenceFrame::m_pMeshZAxis)
	{
		BEM_3D::ReferenceFrame::m_pMeshZAxis->Release();
		BEM_3D::ReferenceFrame::m_pMeshZAxis = nullptr;
	}

	if (BEM_3D::Vertex::m_pMesh)
	{
		BEM_3D::Vertex::m_pMesh->Release();
		BEM_3D::Vertex::m_pMesh = nullptr;
	}
}


void CGABESView::RenderFrameD3D()
{
	if (!m_pD3ddev)
		return;


	SetLighting();


	m_pD3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(110, 110, 110), 1.0f, 0);


	m_pD3ddev->BeginScene();
	
	// Draw the Model
	SetOptions();
	SetMaterial();
	SetGeometryPipeline();

	if (!m_bPostTreatment)
	{
		// Draw the Model in Pre-Processing mode
		pModel->Draw(m_pD3ddev, m_FillMode, false, m_ShowPointsMode);
		
		// Draw the Selected Subset
		DrawSelectedSubSet();

		// Draw the Selected Elements
		DrawSelectedElements();

		// Draw the Selected Points
		DrawSelectedPoints();


		// Draw the HitTested objects
		switch (m_MouseAction)
		{
		case ELEMENT_SELECTION:
			DrawHitTestedElement();
			break;

		case POINT_SELECTION:
			DrawHitTestedPoint();
			break;

		}			

		// Draw the Boundary conditions of the subsets 
		for (BEM_3D::ElementSubSet* pSubSet : pModel->GetSubSets())
			pSubSet->DrawBoundaryConditions(m_pD3ddev, m_bPostTreatment);

		// Draw the reference frames
		// Draw the temporary Frame
		if (m_pTempRefFrame)
			m_pTempRefFrame->Draw();
		
		// Get the document pointer
		CGABESDoc* pDoc = GetDocument();
		// Draw the Global frame
		pDoc->m_GlobalFrame.Draw();
		// Draw the user created frames
		for (BEM_3D::ReferenceFrame* pRefFrame : pDoc->m_ReferenceFrames)
			pRefFrame->Draw();
	}
	else
		// Draw the Model in Post-Processing mode
		pModel->Draw(m_pD3ddev, m_FillMode, true, m_ShowPointsMode);	

			

	m_pD3ddev->EndScene();



	m_pD3ddev->Present(NULL, NULL, NULL, NULL);


	// Update the Frame in the Gizmo Window
	m_GizmoWnd.RenderFrame(m_XRotAng, m_YRotAng);
}


void CGABESView::OnFileLoadMeshFromFile()
{
	// TODO: Add your command handler code here
    // Get the Location of the object file =======================================
	OPENFILENAME ofn = { 0 };
	static TCHAR szFilter[] =
		_T("All Supported (*.stl;*.obj)\0*.stl;*.obj\0")
		_T("STL Files (*.stl)\0*.stl\0")
		_T("OBJ Files (*.obj)\0*.obj\0")
		_T("All Files (*.*)\0*.*\0\0");

	TCHAR szFileName[MAX_PATH] = _T("");
	TCHAR szFileTitle[MAX_PATH] = _T("");

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = ::AfxGetInstanceHandle();
	ofn.lpstrFilter = szFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = _T("stl");        // default extension if user omits it
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szFileTitle;


	if (!GetOpenFileName(&ofn))
		return;

	//===========================================================================
	// Ask the user to set the mesh type
	CDlgSetMeshType dlg(this);

	if(dlg.DoModal() != IDOK)
		return;

	// ======================= Load It ==========================================
	
	pModel->LoadMeshFromFile(szFileName);

	// Set the Coordinate range for the Global frame
	CGABESDoc* pDoc = GetDocument();
	pDoc->m_GlobalFrame.SetCoordinateRanges(*pModel);

	// Load the Vertex Node mesh which uses the Specific length
	BEM_3D::Vertex::LoadMesh(m_pD3ddev, BEM_3D::ElementSubSet::SpecLength);
	
	PathRemoveFileSpec(szFileName);

	pModel->SetWorkingDirectory(szFileName);
	pModel->SetFileName(szFileTitle);

	
	

	// ======================= Update the D3D buffer of the model ===============
	pModel->UpdateVertexBuffer(m_pD3ddev, m_bPostTreatment);

	// Reset the Selections
	ResetSelections();

	// Update the ModelTree  and Info Panels
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_ModelTreePanel.UpdateTreeCtrl();
	pMainFrame->m_infoPanel.UpdateInfo();
}

void CGABESView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// Recreate D3D
	RecreateD3dComponents();

	// TODO: Add your message handler code here
	m_Width = (float)cx;
	m_Height = (float)cy;
	m_W2 = m_Width / 2.0f;
	m_H2 = m_Height / 2.0f;

	// Size and emplace the Gizmo Window
	m_GizmoWnd.MoveWindow(0, m_Height - 150, 150, 150);
	m_GizmoWnd.InitializeD3D();
}

BOOL CGABESView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
		m_CameraDistance *= 0.99f;
	else
		m_CameraDistance *= 1.01f;


	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGABESView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	switch (m_MouseAction)
	{
	case NONE:
		break;

	case ROTATE_VIEW:
		if (nFlags & MK_LBUTTON)
		{
			float dx = (float)(point.x - m_GrabPoint.x);
			float dy = (float)(point.y - m_GrabPoint.y);

			m_GrabPoint = point;


			m_YRotAng += dx / m_W2 * D3DX_PI;
			m_XRotAng += dy / m_H2 * D3DX_PI;
		}
		break;

	case ELEMENT_SELECTION:	
		HitTestElements((float)point.x, (float)point.y);
		break;

	case POINT_SELECTION:
		HitTestPoints((float)point.x, (float)point.y);
		break;

	
	default:
		break;
	}
	

	CView::OnMouseMove(nFlags, point);
}

void CGABESView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	switch (m_MouseAction)
	{
	case NONE:
		m_SelectedElmsIndices.clear();
		break;

	case ROTATE_VIEW:
		m_GrabPoint = point;
		break;

	case ELEMENT_SELECTION:
		if (nFlags & MK_CONTROL)
		{
			if (m_HitTestedElmIdx >= 0)
				m_SelectedElmsIndices.push_back(m_HitTestedElmIdx);
		}
		else
		{
			m_SelectedElmsIndices.clear();
			if (m_HitTestedElmIdx >= 0)
				m_SelectedElmsIndices.push_back(m_HitTestedElmIdx);
		}
		break;

	case POINT_SELECTION:
		if (nFlags & MK_CONTROL)
		{
			if (m_pHitTestedPoint != nullptr)
				m_SelectedPoints.push_back(m_pHitTestedPoint);
		}
		else
		{
			m_SelectedPoints.clear();
			if (m_pHitTestedPoint != nullptr)
				m_SelectedPoints.push_back(m_pHitTestedPoint);
		}
		break;


	default:
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CGABESView::SetOptions()
{
	m_pD3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(80, 80, 80));
	m_pD3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	switch (m_FillMode)
	{
	case BEM_3D::SOLID:
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;

	case BEM_3D::WIREFRAME:
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		break;
	
	case BEM_3D::EDGED_FACES:
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;
	default:
		break;
	}
}


void CGABESView::SetGeometryPipeline()
{
	// Declare the Matrices
	D3DXMATRIX world, view, proj;

	// Set the world matrix
	D3DXMATRIX RotX, RotY;
	D3DXMatrixRotationX(&RotX, m_XRotAng);
	D3DXMatrixRotationY(&RotY, m_YRotAng);
	world = RotX * RotY;
	m_pD3ddev->SetTransform(D3DTS_WORLD, &world);

	// Set the view matrix
	float R = m_CameraDistance;
	float x = R * sinf(m_Phi) * sinf(m_Theta);
	float z = R * sinf(m_Phi) * cosf(m_Theta);
	float y = R * cosf(m_Phi);

	D3DXVECTOR3 Eye(x, y, z);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtRH(&view, &Eye, &LookAt, &Up);
	m_pD3ddev->SetTransform(D3DTS_VIEW, &view);

	// Set the Projection Matrix
	D3DXMatrixPerspectiveFovRH(&proj, D3DX_PI / 4.0f, m_W2 / m_H2, m_fNear, m_fFar);
	m_pD3ddev->SetTransform(D3DTS_PROJECTION, &proj);
}



void CGABESView::SetLighting()
{
	// Get the camera position
	float R = m_CameraDistance;
	float x = R * sinf(m_Phi) * sinf(m_Theta);
	float z = R * sinf(m_Phi) * cosf(m_Theta);
	float y = R * cosf(m_Phi);
	D3DXVECTOR3 Eye(x, y, z);

	// Get the camera orientation vector
	D3DXVECTOR3 Dir(-x, -y, -z);
	D3DXVec3Normalize(&Dir, &Dir);


	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	//light.Position = Eye;
	light.Direction = Dir;
	light.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_pD3ddev->SetLight(0, &light);
	m_pD3ddev->LightEnable(0, true);
	
}

void CGABESView::SetMaterial()
{
	D3DMATERIAL9 steel;
	ZeroMemory(&steel, sizeof(steel));

	steel.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	steel.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	m_pD3ddev->SetMaterial(&steel);
}




void CGABESView::OnViewFront()
{
	m_XRotAng = 0.0f;
	m_YRotAng = 0.0f;
}


void CGABESView::OnViewRight()
{
	m_XRotAng = 0.0f;
	m_YRotAng = -D3DX_PI /2.0f;
}


void CGABESView::OnViewTop()
{
	m_XRotAng = D3DX_PI / 2.0f;
	m_YRotAng = 0.0f;
}


void CGABESView::OnViewBack()
{
	m_XRotAng = 0.0f;
	m_YRotAng = -D3DX_PI;
}


void CGABESView::OnViewLeft()
{
	m_XRotAng = 0.0f;
	m_YRotAng = D3DX_PI / 2.0f;
}


void CGABESView::OnViewBottom()
{
	m_XRotAng = -D3DX_PI / 2.0f;
	m_YRotAng = 0.0f;
}


void CGABESView::OnViewIsometric()
{
	// TODO: Add your command handler code here
	m_XRotAng = 0.0f;
	m_YRotAng = -D3DX_PI / 4.0f;
}


void CGABESView::OnViewMouseactionRotate()
{
	// TODO: Add your command handler code here
	if (m_MouseAction != ROTATE_VIEW)
		m_MouseAction = ROTATE_VIEW;
	else
		m_MouseAction = NONE;
}

void CGABESView::OnViewMouseactionElementSelection()
{
	// TODO: Add your command handler code here
	if (m_MouseAction != ELEMENT_SELECTION)
		m_MouseAction = ELEMENT_SELECTION;
	else
		m_MouseAction = NONE;
}

void CGABESView::OnUpdateViewMouseactionRotate(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_MouseAction == ROTATE_VIEW);
}

void CGABESView::OnUpdateViewMouseactionElementSelection(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_MouseAction == ELEMENT_SELECTION);
}


void CGABESView::HitTestElements(float x, float y)
{
	// Get the screen size
	float dx, dy;
	D3DXMATRIX View, Proj, invView, invProj;

	dx = (x / m_W2 - 1.0f) /*/ fAspect*/;
	dy = 1.0f - y / m_H2;


	m_pD3ddev->GetTransform(D3DTS_VIEW, &View);
	m_pD3ddev->GetTransform(D3DTS_PROJECTION, &Proj);


	D3DXMatrixInverse(&invProj, nullptr, &Proj);
	D3DXMatrixInverse(&invView, nullptr, &View);

	D3DXVECTOR4 p2(dx, dy, -1.0f, 1.0f);

	D3DXVec4Transform(&p2, &p2, &invProj);
	p2 = D3DXVECTOR4(p2.x, p2.y, -1.0f, 1.0f);
	D3DXVec4Transform(&p2, &p2, &invView);

	float R = m_CameraDistance;
	float Xcam = R * sinf(m_Phi) * sinf(m_Theta);
	float Ycam = R * cosf(m_Phi);
	float Zcam = R * sinf(m_Phi) * cosf(m_Theta);

	D3DXVECTOR3 P1(Xcam, Ycam, Zcam);
	D3DXVECTOR3 P2(p2.x, p2.y, p2.z);
	D3DXVECTOR3 e = P2 - P1;
	D3DXVec3Normalize(&e, &e);
	P2 = P1 + (m_fFar - m_fNear) * e;


    m_HitTestedElmIdx = -1;
	
	D3DXMATRIX world;
	m_pD3ddev->GetTransform(D3DTS_WORLD, &world);
	pModel->GetHitTestedElement(P1, P2, &m_HitTestedElmIdx, world);
}


void CGABESView::HitTestPoints(float x, float y)
{
	// Get the screen size
	float dx, dy;
	D3DXMATRIX View, Proj, invView, invProj;

	dx = (x / m_W2 - 1.0f) /*/ fAspect*/;
	dy = 1.0f - y / m_H2;


	m_pD3ddev->GetTransform(D3DTS_VIEW, &View);
	m_pD3ddev->GetTransform(D3DTS_PROJECTION, &Proj);


	D3DXMatrixInverse(&invProj, nullptr, &Proj);
	D3DXMatrixInverse(&invView, nullptr, &View);

	D3DXVECTOR4 p2(dx, dy, -1.0f, 1.0f);

	D3DXVec4Transform(&p2, &p2, &invProj);
	p2 = D3DXVECTOR4(p2.x, p2.y, -1.0f, 1.0f);
	D3DXVec4Transform(&p2, &p2, &invView);

	float R = m_CameraDistance;
	float Xcam = R * sinf(m_Phi) * sinf(m_Theta);
	float Ycam = R * cosf(m_Phi);
	float Zcam = R * sinf(m_Phi) * cosf(m_Theta);

	D3DXVECTOR3 P1(Xcam, Ycam, Zcam);
	D3DXVECTOR3 P2(p2.x, p2.y, p2.z);
	D3DXVECTOR3 e = P2 - P1;
	D3DXVec3Normalize(&e, &e);
	P2 = P1 + (m_fFar - m_fNear) * e;


	m_pHitTestedPoint = nullptr;

	D3DXMATRIX world;
	m_pD3ddev->GetTransform(D3DTS_WORLD, &world);

	pModel->GetHitTestedPoint(P1, P2, &m_pHitTestedPoint, world);
}


void CGABESView::DrawHitTestedElement()
{
	if (m_HitTestedElmIdx < 0)
		return;

	// Create a temporary Material
	D3DMATERIAL9 htMat;
	ZeroMemory(&htMat, sizeof(htMat));

	htMat.Ambient = D3DXCOLOR(0.3f, 0.0f, 0.3f, 1.0f);
	htMat.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.7f, 1.0f);

	m_pD3ddev->SetMaterial(&htMat);

	

	// Set the Stream source and FVF
	m_pD3ddev->SetStreamSource(0, pModel->GetVertexBuffer(), 0, sizeof(BEM_3D::D3DVertex));
	m_pD3ddev->SetFVF(BEM_3D::g_FVF);

	// Draw the hittested triangle
	m_pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 3 * m_HitTestedElmIdx, 1);
}


void CGABESView::DrawHitTestedPoint()
{
	if (m_pHitTestedPoint == nullptr)
		return;

	// Set the Fill Mode To Solid
	DWORD dwPrevFillMode;
	m_pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// Set Lighting to true
	DWORD dwPrevLighting;
	m_pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Create a Material
	// Create the materials
	D3DMATERIAL9 NodeMaterial;
	ZeroMemory(&NodeMaterial, sizeof(NodeMaterial));


	NodeMaterial.Ambient = D3DXCOLOR(5.0f, 0.1f, 0.5f, 1.0f);
	NodeMaterial.Diffuse = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);



	m_pD3ddev->SetMaterial(&NodeMaterial);

	std::vector<BEM_3D::Vertex*>& rVertices = pModel->GetVertices();

	m_pHitTestedPoint->Draw(m_bPostTreatment);


	// Restore Fill Mode
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	// Restore Lighting
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
}



void CGABESView::DrawSelectedElements()
{
	if (m_SelectedElmsIndices.empty())
		return;

	// Create a temporary Material
	D3DMATERIAL9 SelMat;
	ZeroMemory(&SelMat, sizeof(SelMat));

	SelMat.Ambient = D3DXCOLOR(0.0f, 0.1f, 0.3f, 1.0f);
	SelMat.Diffuse = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);

	m_pD3ddev->SetMaterial(&SelMat);



	// Set the Stream source and FVF
	m_pD3ddev->SetStreamSource(0, pModel->GetVertexBuffer(), 0, sizeof(BEM_3D::D3DVertex));
	m_pD3ddev->SetFVF(BEM_3D::g_FVF);

	// Draw the selected Elements
	if (m_FillMode == BEM_3D::EDGED_FACES)
	{
		// Store the previous Fill Mode
		DWORD dwPrevFillMode;
		m_pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);

		// Force the Fill Mode to be Solid
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		for (int Idx : m_SelectedElmsIndices)
		{
			m_pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 3 * Idx, 1);
		}

		// Force the Fill Mode to be wireframe
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		// Change the Material
		D3DMATERIAL9 Wire;
		ZeroMemory(&Wire, sizeof(Wire));

		Wire.Ambient = D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f);
		Wire.Diffuse = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);

		m_pD3ddev->SetMaterial(&Wire);

		for (int Idx : m_SelectedElmsIndices)
		{
			m_pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 3 * Idx, 1);
		}

		// Restore the Previous Fill Mode
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);

	}
	else
		for (int Idx : m_SelectedElmsIndices)
		{
			m_pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 3 * Idx, 1);
		}	
}


void CGABESView::DrawSelectedPoints()
{
	if (m_SelectedPoints.empty())
		return;

	// Set the Fill Mode To Solid
	DWORD dwPrevFillMode;
	m_pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// Set Lighting to true
	DWORD dwPrevLighting;
	m_pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Create a Material
	// Create the materials
	D3DMATERIAL9 PointMaterial;
	ZeroMemory(&PointMaterial, sizeof(PointMaterial));


	PointMaterial.Ambient = D3DXCOLOR(5.0f, 0.0f, 0.0f, 1.0f);
	PointMaterial.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);



	m_pD3ddev->SetMaterial(&PointMaterial);



	for (BEM_3D::Vertex* pV : m_SelectedPoints)
		pV->Draw(m_bPostTreatment);
	
	
	// Restore Fill Mode
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	// Restore Lighting
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
}


void CGABESView::DrawSelectedSubSet()
{
	if (!m_pSelectedSubSet)
		return;

	// Create a temporary Material
	D3DMATERIAL9 SubSetMat;
	ZeroMemory(&SubSetMat, sizeof(SubSetMat));

	SubSetMat.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.3f, 1.0f);
	SubSetMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	m_pD3ddev->SetMaterial(&SubSetMat);



	// Set the Stream source and FVF
	m_pD3ddev->SetStreamSource(0, pModel->GetVertexBuffer(), 0, sizeof(BEM_3D::D3DVertex));
	m_pD3ddev->SetFVF(BEM_3D::g_FVF);

	// Draw the selected Elements
	if (m_FillMode == BEM_3D::EDGED_FACES)
	{
		// Store the previous Fill Mode
		DWORD dwPrevFillMode;
		m_pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);

		// Force the Fill Mode to be Solid
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		for (int Idx : m_pSelectedSubSet->m_Indices)
		{
			m_pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 3 * Idx, 1);
		}

		// Force the Fill Mode to be wireframe
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		// Change the Material
		D3DMATERIAL9 Wire;
		ZeroMemory(&Wire, sizeof(Wire));

		Wire.Ambient = D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f);
		Wire.Diffuse = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);

		m_pD3ddev->SetMaterial(&Wire);

		for (int Idx : m_pSelectedSubSet->m_Indices)
		{
			m_pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 3 * Idx, 1);
		}

		// Restore the Previous Fill Mode
		m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);

	}
	else
		for (int Idx : m_pSelectedSubSet->m_Indices)
		{
			m_pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 3 * Idx, 1);
		}
}


void CGABESView::OnModelingCreateElementSetFromSelection()
{
	// TODO: Add your command handler code here
	if (m_SelectedElmsIndices.empty())
		return;

	// Add the current Selection to the SubSets Container
	pModel->GetSubSets().push_back(new BEM_3D::ElementSubSet(m_SelectedElmsIndices));

	// Reset the Selections
	ResetSelections();

	// Update the ModelTree Panel
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->GetModelTreePanel().UpdateTreeCtrl();
}

void CGABESView::OnUpdateModelingCreateElementSetFromSelection(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_SelectedElmsIndices.empty());
}


void CGABESView::ResetSelections()
{
	m_HitTestedElmIdx = -1;
	m_pHitTestedPoint = nullptr;
	m_SelectedElmsIndices.clear();
	m_SelectedPoints.clear();
	m_pSelectedSubSet = nullptr;
}
void CGABESView::OnModelingImportElementSetFromObjFile()
{
	// TODO: Add your command handler code here
	// Get the Location of the object file =======================================
	OPENFILENAME ofn = { 0 };
	static TCHAR szFilter[] = _T("Wavefront 3D Object Files (*.obj)\0*.obj\0");
	TCHAR szFileName[MAX_PATH] = _T("");
	TCHAR szFileTitle[MAX_PATH] = _T("");

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = ::AfxGetInstanceHandle();
	ofn.lpstrFilter = szFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = TEXT("obj");
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szFileTitle;

	GetOpenFileName(&ofn);

	if (szFileName[0] == '\0')
		return;

	// ======================= Load It ==========================================
	// Remove the extension from the file title
	// Look for the last dot in the filename
	CString strFileTitle(szFileTitle);
	int dotPos = strFileTitle.ReverseFind('.');

	if (dotPos != -1)
	{
		CString ext = strFileTitle.Mid(dotPos);

		if (ext.CompareNoCase(_T(".obj")) == 0)
		{
			strFileTitle = strFileTitle.Left(dotPos);
		}
	}
	
	pModel->CreateSubSetFromWavefrontObjectFile(szFileName, strFileTitle.GetBuffer());
	
	// Reset the Selections
	ResetSelections();

	// Update the ModelTree Panel
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->GetModelTreePanel().UpdateTreeCtrl();
}


void CGABESView::OnFileDeleteModel()
{
	// TODO: Add your command handler code here
	pModel->ClearAll();
	
	pModel->SetWorkingDirectory(_T(""));
	pModel->SetFileName(_T(""));
	pModel->SetFileType(_T(""));


	// Reset the Selections
	ResetSelections();

	// Update the ModelTree  and Info Panels
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_ModelTreePanel.UpdateTreeCtrl();
	pMainFrame->m_infoPanel.UpdateInfo();
}

void CGABESView::OnUpdateFileDeleteModel(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!pModel->IsModelEmpty());
}

void CGABESView::OnUpdateModelingImportElementSetFromObjFile(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!pModel->IsModelEmpty());
}



void CGABESView::OnModelingCreateRangeElementSet()
{
	// TODO: Add your command handler code here
	

	m_pDlgRangeSubSet->InitModelessDialog();

	m_pDlgRangeSubSet->ShowWindow(SW_SHOWNORMAL);	
}

void CGABESView::OnUpdateModelingCreateRangeElementSet(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!pModel->IsModelEmpty());
}

void CGABESView::OnBcsFullyEncaster()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;

	m_pSelectedSubSet->FullyEncaster();
}

void CGABESView::OnBcsPartialEncastrement()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;

	CDlgPartialEncaster dlg(this);

	if (dlg.DoModal() != IDOK)
		return;

	if (dlg.bEncasterUx)
		m_pSelectedSubSet->SetUx(0.0);

	if (dlg.bEncasterUy)
		m_pSelectedSubSet->SetUy(0.0);

	if (dlg.bEncasterUz)
		m_pSelectedSubSet->SetUz(0.0);

}

void CGABESView::OnBcsDisplacementVector()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;

	CDlgDisplacementVector dlg(this);

	if (dlg.DoModal() != IDOK)
		return;

	if (dlg.bSetUx)
		m_pSelectedSubSet->SetUx(dlg.Ux);

	if (dlg.bSetUy)
		m_pSelectedSubSet->SetUy(dlg.Uy);

	if (dlg.bSetUz)
		m_pSelectedSubSet->SetUz(dlg.Uz);
}

void CGABESView::OnBcsNormalDisplacement()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;

	CDlgNormalDisplacement dlg(this);

	if (dlg.DoModal() != IDOK)
		return;

	m_pSelectedSubSet->SetNormalDisplacement(dlg.Un);
}

void CGABESView::OnBcsTractionVector()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;

	CDlgTractionVector dlg(this);

	if (dlg.DoModal() != IDOK)
		return;

	if (dlg.bSetTx)
		m_pSelectedSubSet->SetTx(dlg.Tx);

	if (dlg.bSetTy)
		m_pSelectedSubSet->SetTy(dlg.Ty);

	if (dlg.bSetTz)
		m_pSelectedSubSet->SetTz(dlg.Tz);
}


void CGABESView::OnBcsTractionFree()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;


	m_pSelectedSubSet->FreeSurface();
}


void CGABESView::OnBcsNormalTraction()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;

	CDlgNormalTraction dlg(this);

	if (dlg.DoModal() != IDOK)
		return;

	m_pSelectedSubSet->SetNormalTraction(dlg.Tn);
}

void CGABESView::OnBcsPressure()
{
	// TODO: Add your command handler code here
	if (m_pSelectedSubSet == nullptr)
		return;

	CDlgPressure dlg(this);

	if (dlg.DoModal() != IDOK)
		return;

	m_pSelectedSubSet->SetNormalTraction(-dlg.P);
}

void CGABESView::OnUpdateBcsFullyEncaster(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}

void CGABESView::OnUpdateBcsPartialEncastrement(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}

void CGABESView::OnUpdateBcsDisplacementVector(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}

void CGABESView::OnUpdateBcsNormalDisplacement(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}


void CGABESView::OnUpdateBcsTractionFree(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}

void CGABESView::OnUpdateBcsTractionVector(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}

void CGABESView::OnUpdateBcsNormalTraction(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}

void CGABESView::OnUpdateBcsPressure(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pSelectedSubSet != nullptr);
}



void CGABESView::OnBemSolveBoundaryValueProblem()
{
	// TODO: Add your command handler code here
	// STEP 0 Set the solver parameters
	CDlgSolverParameters dlgSolverProp(this);
	if(dlgSolverProp.DoModal() != IDOK)
		return;

	// STEP 1 Calculate the matrices!
	CDlgWaitLengthyOperation dlg(pModel, 0, this);
	if(dlg.DoModal() == IDCANCEL)
		return;


	dlg.nOperation = 1;

	
	// STEP 2 Resolve the linear system
	if (dlg.DoModal() == IDCANCEL)
		return;

	// STEP 3 Update the boundary unknowns
	pModel->UpdateBoundaryUnknowns();
	pModel->UpdateVertexBuffer(m_pD3ddev, m_bPostTreatment);
}

void CGABESView::OnBemShowDeformation()
{
	// TODO: Add your command handler code here
	// Toogle the Include Displacement static Flag for Vertex class
	BEM_3D::Vertex::m_bIncludeDisplacements = !BEM_3D::Vertex::m_bIncludeDisplacements;
	pModel->UpdateVertexBuffer(m_pD3ddev, m_bPostTreatment);

}

void CGABESView::OnUpdateBemShowDeformation(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(BEM_3D::Vertex::m_bIncludeDisplacements);
	pCmdUI->Enable(m_bPostTreatment);
}

void CGABESView::OnBemSetMaterialProperties()
{
	// TODO: Add your command handler code here
	CDlgSetMaterialProperties dlg(this);

	if(dlg.DoModal() != IDOK)
		return;

	// Update the information panel
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_infoPanel.UpdateInfo();
}



void CGABESView::OnBemSetIntegrationParameters()
{
	// TODO: Add your command handler code here
	CDlgSetIntegrationParameters dlg(this);

	if (dlg.DoModal() != IDOK)
		return;

	// Update the information panel
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_infoPanel.UpdateInfo();
}

void CGABESView::OnBemSetDeformationScale()
{
	// TODO: Add your command handler code here
	CDlgSetDeformationScale dlg(pModel, this);

	if(dlg.DoModal() != IDOK)
		return;

	pModel->UpdateVertexBuffer(m_pD3ddev, m_bPostTreatment);
}

void CGABESView::OnBemPosttreatmentMode()
{
	// TODO: Add your command handler code here
	m_bPostTreatment = !m_bPostTreatment;

	pModel->UpdateVertexBuffer(m_pD3ddev, m_bPostTreatment);
}

void CGABESView::OnUpdateBemPosttreatmentMode(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bPostTreatment);
}

void CGABESView::OnUpdateBemSetDeformationScale(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(BEM_3D::Vertex::m_bIncludeDisplacements && m_bPostTreatment);
}

void CGABESView::OnBcsCheckShowFixations()
{
	// TODO: Add your command handler code here
	BEM_3D::ElementSubSet::m_bShowFixations = !BEM_3D::ElementSubSet::m_bShowFixations;
}

void CGABESView::OnBcsCheckShowDisplacementVectors()
{
	// TODO: Add your command handler code here
	BEM_3D::ElementSubSet::m_bShowDispVectors = !BEM_3D::ElementSubSet::m_bShowDispVectors;
}

void CGABESView::OnBcsCheckShowTractionVectors()
{
	// TODO: Add your command handler code here
	BEM_3D::ElementSubSet::m_bShowTracVectors = !BEM_3D::ElementSubSet::m_bShowTracVectors;
}

void CGABESView::OnUpdateBcsCheckShowFixations(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(BEM_3D::ElementSubSet::m_bShowFixations);
}

void CGABESView::OnUpdateBcsCheckShowDisplacementVectors(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(BEM_3D::ElementSubSet::m_bShowDispVectors);
}

void CGABESView::OnUpdateBcsCheckShowTractionVectors(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(BEM_3D::ElementSubSet::m_bShowTracVectors);
}

int CGABESView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	// TODO:  Add your specialized creation code here
	// Create the Gizmo Child Window
	m_GizmoWnd.CreateGizmoWnd(this);
	
	// Create the PaletteCtrl
	m_PaletteCtrl.Create(this, CRect(10, 10, 250, 350));

	// Create the ReferenceFrame Dialog
	m_DlgReferenceFrame.Create(IDD_DLG_REFFRAME, this);


	return 0;
}





void CGABESView::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	// Then manually call OnUpdateCmdUI for your palette
	if (m_PaletteCtrl.GetSafeHwnd())
	{
		bool bShowWindow = (m_bPostTreatment && pModel->GetOutputMode() != BEM_3D::NONE);

		m_PaletteCtrl.ShowWindow(bShowWindow);

		if (bShowWindow)
		{
			m_PaletteCtrl.Update();
		}
	}
}


void CGABESView::OnBemPaletteSettings()
{
	// TODO: Add your command handler code here
	CDlgPaletteSettings dlg(this);

	if (dlg.DoModal() == IDOK)
		m_PaletteCtrl.Update();
}



void CGABESView::OnBemSetMemoryQuota()
{
	// TODO: Add your command handler code here
	CDlgSetRamQuota dlg(this);

	dlg.RAM_Quota = pModel->m_RAM_Quota_Gb;

	if (dlg.DoModal() != IDOK)
		return;

	pModel->m_RAM_Quota_Gb = dlg.RAM_Quota;

	pModel->m_bAutoSetRamQuotas = dlg.bAutoQuotas;

	if (!dlg.bAutoQuotas)
		pModel->SetMatrixMemoryQuotas(dlg.Q_Quota, dlg.R_Quota, dlg.A_Quota);
	else
		pModel->AutoSetMatrixMemoryQuotas();

	// Update the information panel
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_infoPanel.UpdateInfo();	
}



void CGABESView::OnBemGetDomainNodeResponse()
{
	// TODO: Add your command handler code here
	CDlgDomainNodeResponse dlg(pModel, this);

	dlg.DoModal();
}

void CGABESView::OnBemGenerateReport()
{
	// TODO: Add your command handler code here
	CDlgGenerateDomainResponseReport dlg(pModel, this);

	dlg.DoModal();
}



void CGABESView::OnModelingAddFrame()
{
	// TODO: Add your command handler code here
	m_pTempRefFrame = new BEM_3D::ReferenceFrame();
	m_pTempRefFrame->m_bSelected = true;
	m_DlgReferenceFrame.m_pCurrRefFrame = m_pTempRefFrame;
	m_DlgReferenceFrame.UpdateControlsFromCurrentRefFrame();
	m_DlgReferenceFrame.ShowWindow(SW_SHOWNORMAL);
}

void CGABESView::OnViewMouseactionPointSelection()
{
	// TODO: Add your command handler code here

	if (m_MouseAction != POINT_SELECTION)
		m_MouseAction = POINT_SELECTION;
	else
		m_MouseAction = NONE;
}

void CGABESView::OnUpdateViewMouseactionPointSelection(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_MouseAction == POINT_SELECTION);

}





void CGABESView::RecreateD3dComponents()
{
	CleanUpD3D();
	InitializeD3D();
	if (m_pD3ddev == nullptr)
		return;

	BEM_3D::Vertex::LoadMesh(m_pD3ddev, BEM_3D::ElementSubSet::SpecLength);
	m_GizmoWnd.InitializeD3D();

	if(pModel != nullptr)
		if (!pModel->IsModelEmpty())
			pModel->UpdateVertexBuffer(m_pD3ddev, m_bPostTreatment);
}



