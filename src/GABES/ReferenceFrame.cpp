#include "pch.h"
#include "resource.h"
#include "ReferenceFrame.h"


using namespace BEM_3D;

IDirect3DDevice9* ReferenceFrame::m_pD3ddev = nullptr;
ID3DXMesh* ReferenceFrame::m_pMeshXAxis = nullptr;
ID3DXMesh* ReferenceFrame::m_pMeshYAxis = nullptr;
ID3DXMesh* ReferenceFrame::m_pMeshZAxis = nullptr;
UINT ReferenceFrame::m_nRefFrameIdx = 0;



ReferenceFrame::ReferenceFrame()
	: m_Origin(0.0, 0.0, 0.0)
	, m_Vx(1.0, 0.0, 0.0)
	, m_Vy(0.0, 1.0, 0.0)
	, m_Vz(0.0, 0.0, 1.0)
	, m_bSelected(false)
{
	m_strName.Format(_T("Frame N°%d"), m_nRefFrameIdx);
	m_nRefFrameIdx++;
}


ReferenceFrame::ReferenceFrame(CString strName)
	: m_Origin(0.0, 0.0, 0.0)
	, m_Vx(1.0, 0.0, 0.0)
	, m_Vy(0.0, 1.0, 0.0)
	, m_Vz(0.0, 0.0, 1.0)
	, m_strName(strName)
	, m_bSelected(false)
{
	m_nRefFrameIdx++;
}



void ReferenceFrame::ToGlobalCoord(Vertex& V_local)const
{
	Matrix R(m_Vx, m_Vy, m_Vz);
	R = R.Transpose();  // R must be a matrix constructed of column unitary orthogonal vectors

	V_local = m_Origin + R * V_local;
}


void ReferenceFrame::ToLocalCoord(Vertex& V_global)const
{
	Matrix R(m_Vx, m_Vy, m_Vz);  // R must be a matrix constructed of row unitary orthogonal vectors

	V_global = R * (V_global - m_Origin);
}


void ReferenceFrame::ToCylindricalCoord(const Vertex& V, double& R, double& Theta, double& Z_)const
{
	R = sqrt(V.x * V.x + V.y * V.y);
	Theta = atan2(V.y, V.x);
	Z_ = V.z;

	//// 7 Bring the angle Theta to the range 0° - 360°
	//if (Theta < 0.0)
	//	Theta += 2.0 * M_PI;
}

void ReferenceFrame::ToSphericalCoord(const Vertex& V, double& R, double& Theta, double& Phi)const
{
	double SQRT = sqrt(V.x * V.x + V.y * V.y);
	Phi = atan2(SQRT, V.z);
	R = V.Magnitude();

	// 3 - Calculate Theta
	Theta = atan2(V.y, V.z);

	//// 7 Bring the angles to the range 0° - 360°
	//if (Theta < 0.0)
	//	Theta += 2.0 * M_PI;

	//if (Phi < 0.0)
	//	Phi += 2.0 * M_PI;
}


void ReferenceFrame::Draw()const
{
	if (m_pD3ddev == nullptr)
		return;

	// Create the world matrix
	D3DXMATRIX matWorld, matFrame, matTrans, matScale;


    D3DXMatrixIdentity(&matFrame);

	

	// X axis
	matFrame._11 = m_Vx.x;
	matFrame._12 = m_Vx.y;
	matFrame._13 = m_Vx.z;

	// Y axis
	matFrame._21 = m_Vy.x;
	matFrame._22 = m_Vy.y;
	matFrame._23 = m_Vy.z;

	// Z axis
	matFrame._31 = m_Vz.x;
	matFrame._32 = m_Vz.y;
	matFrame._33 = m_Vz.z;

	

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = m_bSelected ? 0.1f : 0.05f;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);
	D3DXMatrixTranslation(&matTrans, (float)m_Origin.x, (float)m_Origin.y, (float)m_Origin.z);

	matFrame = matFrame * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matFrame);


	// Get Previous states (lighting, Fill mode, FVF)
	DWORD dwPrevLighting, dwPrevFillMode, dwPrevFVF;
	m_pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwPrevLighting);
	m_pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);
	m_pD3ddev->GetFVF(&dwPrevFVF);

	// Froce the lighting to true, Fill mode to Solid and FVF to XYZ|NORMAL
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pD3ddev->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);

	// Create the materials
	D3DMATERIAL9 mtrX, mtrY, mtrZ;
	ZeroMemory(&mtrX, sizeof(mtrX));
	ZeroMemory(&mtrY, sizeof(mtrY));
	ZeroMemory(&mtrZ, sizeof(mtrZ));

	mtrX.Ambient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	mtrX.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	mtrY.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
	mtrY.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	mtrZ.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
	mtrZ.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	m_pD3ddev->SetMaterial(&mtrX);
	m_pMeshXAxis->DrawSubset(0);

	m_pD3ddev->SetMaterial(&mtrY);
	m_pMeshYAxis->DrawSubset(0);

	m_pD3ddev->SetMaterial(&mtrZ);
	m_pMeshZAxis->DrawSubset(0);


	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}



void ReferenceFrame::LoadGizmo(IDirect3DDevice9* pD3ddev)
{
	// Set the device pointer
	m_pD3ddev = pD3ddev;

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
}


bool ReferenceFrame::IsValid() const
{
	if (m_Vx * m_Vy > 1.0E-9)
		return false;

	if (m_Vy * m_Vz > 1.0E-9)
		return false;

	if (m_Vz * m_Vx > 1.0E-9)
		return false;

	return true;
}


void ReferenceFrame::SetCoordinateRanges(const BEM_3D::Model& rModel)
{
	m_X_min = DBL_MAX;
	m_X_max = -DBL_MAX;

	m_Y_min = DBL_MAX;
	m_Y_max = -DBL_MAX;
	
	m_Z_min = DBL_MAX;
	m_Z_max = -DBL_MAX;

	// Cylindrincal Range
	m_Rc_min = DBL_MAX;
	m_Rc_max = -DBL_MAX;

	// Spherical Range
	m_Rs_min = DBL_MAX;
	m_Rs_max = -DBL_MAX;

	// Get a reference to the Model vertices

	const std::vector<Vertex*>& rVertices = rModel.GetVertices();
	for (int i = 0; i < rVertices.size(); i++)
	{
		Vertex* pV = rVertices[i];
		Vertex V = *pV;

		ToLocalCoord(V);

		// The Cartezian part
		// X
		if (V.x < m_X_min)
			m_X_min = V.x;
		if (V.x > m_X_max)
			m_X_max = V.x;
		// Y
		if (V.y < m_Y_min)
			m_Y_min = V.y;
		if (V.y > m_Y_max)
			m_Y_max = V.y;
		// Z
		if (V.z < m_Z_min)
			m_Z_min = V.z;
		if (V.z > m_Z_max)
			m_Z_max = V.z;

		// The Cylindrincal part
		double Rc = 0.0;
		double Theta_c = 0.0;
		double Z_ = 0.0;
		ToCylindricalCoord(V, Rc, Theta_c, Z_);

		// R
		if (Rc < m_Rc_min)
			m_Rc_min = Rc;
		if (Rc > m_Rc_max)
			m_Rc_max = Rc;


		// The Spherical part
		double Rs = 0.0;
		double Theta_s = 0.0;
		double Phi_s = 0.0;
		ToSphericalCoord(V, Rs, Theta_s, Phi_s);

		// R
		if (Rs < m_Rs_min)
			m_Rs_min = Rs;
		if (Rs > m_Rs_max)
			m_Rs_max = Rs;
	}
}