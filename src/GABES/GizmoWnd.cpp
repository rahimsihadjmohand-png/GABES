// GizmoWnd.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "GizmoWnd.h"




// CGizmoWnd

IMPLEMENT_DYNAMIC(CGizmoWnd, CWnd)

CGizmoWnd::CGizmoWnd()
	: m_pD3ddev(nullptr)
	, m_pMeshXAxis(nullptr)
	, m_pMeshYAxis(nullptr)
	, m_pMeshZAxis(nullptr)
	, m_Z(90.0f)
	, m_Width(0)
	, m_Height(0)
{

}

CGizmoWnd::~CGizmoWnd()
{
	CleanUpD3D();
}



void CGizmoWnd::CleanUpD3D()
{
	if (m_pD3ddev)
	{
		m_pD3ddev->Release();
		m_pD3ddev = nullptr;
	}

	if (m_pMeshXAxis)
	{
		m_pMeshXAxis->Release();
		m_pMeshXAxis = nullptr;
	}

	if (m_pMeshYAxis)
	{
		m_pMeshYAxis->Release();
		m_pMeshYAxis = nullptr;
	}

	if (m_pMeshZAxis)
	{
		m_pMeshZAxis->Release();
		m_pMeshZAxis = nullptr;
	}
}



BEGIN_MESSAGE_MAP(CGizmoWnd, CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()




// CGizmoWnd message handlers


BOOL CGizmoWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszClass = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr
	);

	return TRUE;

}



BOOL CGizmoWnd::CreateGizmoWnd(CWnd* pParent)
{
	
	BOOL res = Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParent, ID_GIZMO_WND);	

	return res;
}




void CGizmoWnd::InitializeD3D()
{
	if (m_pD3ddev)
		m_pD3ddev->Release();

	IDirect3D9* pD3d = Direct3DCreate9(D3D_SDK_VERSION);


	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferWidth = m_Width;
	d3dpp.BackBufferHeight = m_Height;
	d3dpp.hDeviceWindow = GetSafeHwnd();
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = true;



	HRESULT hr = pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetSafeHwnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3ddev);

	pD3d->Release();


	// Load the Gizmo Meshes

	// 1. X Axis
	// Load resource to memory
	HRSRC hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MESH_X_AXIS), _T("MESH"));
	if (!hRes) return;

	HGLOBAL hData = LoadResource(AfxGetResourceHandle(), hRes);
	if (!hData) return;

	DWORD size = SizeofResource(AfxGetResourceHandle(), hRes);
	LPVOID pData = LockResource(hData);


	// Load the resource memory to the Mesh Object
	D3DXLoadMeshFromXInMemory(
		pData, size,
		D3DXMESH_MANAGED,
		m_pD3ddev,
		nullptr, nullptr, nullptr,
		nullptr,
		&m_pMeshXAxis);
	//=======================================================================================================

	// 1. Y Axis
	// Load resource to memory
	hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MESH_Y_AXIS), _T("MESH"));
	if (!hRes) return;

	hData = LoadResource(AfxGetResourceHandle(), hRes);
	if (!hData) return;

	size = SizeofResource(AfxGetResourceHandle(), hRes);
	pData = LockResource(hData);


	// Load the resource memory to the Mesh Object
	D3DXLoadMeshFromXInMemory(
		pData, size,
		D3DXMESH_MANAGED,
		m_pD3ddev,
		nullptr, nullptr, nullptr,
		nullptr,
		&m_pMeshYAxis);
	//=======================================================================================================

	// 1. Z Axis
	// Load resource to memory
	hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MESH_Z_AXIS), _T("MESH"));
	if (!hRes) return;

	hData = LoadResource(AfxGetResourceHandle(), hRes);
	if (!hData) return;

	size = SizeofResource(AfxGetResourceHandle(), hRes);
	pData = LockResource(hData);


	// Load the resource memory to the Mesh Object
	D3DXLoadMeshFromXInMemory(
		pData, size,
		D3DXMESH_MANAGED,
		m_pD3ddev,
		nullptr, nullptr, nullptr,
		nullptr,
		&m_pMeshZAxis);
	//=======================================================================================================

	// Set Lighting 
	// Get the camera position
	float x = 0.0f;
	float z = m_Z;
	float y = 0.0f;
	D3DXVECTOR3 Eye(x, y, z);

	// Get the camera orientation vector
	D3DXVECTOR3 Dir(-x, -y, -z);
	D3DXVec3Normalize(&Dir, &Dir);


	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	//light.Position = Eye;
	light.Direction = Dir;
	light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_pD3ddev->SetLight(0, &light);
	m_pD3ddev->LightEnable(0, true);

	// Set Render State
	// Froce the lighting to true and Fill mode to Solid
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}


void CGizmoWnd::SetGeometryPipeline(float XRotAng, float YRotAng)
{
	// Declare the Matrices
	D3DXMATRIX world, view, proj;

	// Set the world matrix
	D3DXMATRIX RotX, RotY, Scale;
	D3DXMatrixRotationX(&RotX, XRotAng);
	D3DXMatrixRotationY(&RotY,YRotAng);
	D3DXMatrixScaling(&Scale, 0.5f, 0.5f, 0.5f);
	world = RotX * RotY * Scale;
	m_pD3ddev->SetTransform(D3DTS_WORLD, &world);

	// Set the view matrix
	float x = 0.0f;
	float z = m_Z;
	float y = 0.0f;

	D3DXVECTOR3 Eye(x, y, z);
	D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtRH(&view, &Eye, &LookAt, &Up);
	m_pD3ddev->SetTransform(D3DTS_VIEW, &view);

	// Set the Projection Matrix
	D3DXMatrixPerspectiveFovRH(&proj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f);
	m_pD3ddev->SetTransform(D3DTS_PROJECTION, &proj);
}



void CGizmoWnd::RenderFrame(float XRotAng, float YRotAng)
{
	if (!m_pD3ddev)
		return;



	m_pD3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(110, 110, 110), 1.0f, 0);


	m_pD3ddev->BeginScene();

	// Draw the Gismo
	SetGeometryPipeline(XRotAng, YRotAng);


	

	// Create the materials
	D3DMATERIAL9 mtrX, mtrY, mtrZ;
	ZeroMemory(&mtrX, sizeof(mtrX));
	ZeroMemory(&mtrY, sizeof(mtrY));
	ZeroMemory(&mtrZ, sizeof(mtrZ));

	mtrX.Ambient = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
	mtrX.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	mtrY.Ambient = D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f);
	mtrY.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	mtrZ.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.8f, 1.0f);
	mtrZ.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	m_pD3ddev->SetMaterial(&mtrX);
	m_pMeshXAxis->DrawSubset(0);

	m_pD3ddev->SetMaterial(&mtrY);
	m_pMeshYAxis->DrawSubset(0);

	m_pD3ddev->SetMaterial(&mtrZ);
	m_pMeshZAxis->DrawSubset(0);


	m_pD3ddev->EndScene();


	m_pD3ddev->Present(NULL, NULL, NULL, NULL);

}


void CGizmoWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_Width = cx;
	m_Height = cy;
}
