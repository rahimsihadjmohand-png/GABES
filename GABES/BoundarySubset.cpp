#include "pch.h"
#include "resource.h"
#include "BoundarySubset.h"

using namespace BEM_3D;

UINT BoundarySubset::m_nSubSetIdx = 1;
std::vector<Element*>* BoundarySubset::m_pElements = nullptr;
bool BoundarySubset::m_bShowFixations = true;
bool BoundarySubset::m_bShowDispVectors = true;
bool BoundarySubset::m_bShowTracVectors = true;

IDirect3DDevice9* BoundarySubset::m_pD3ddev = nullptr;
ID3DXMesh* BoundarySubset::m_pMeshVector = nullptr;
ID3DXMesh* BoundarySubset::m_pMeshFixationX = nullptr;
ID3DXMesh* BoundarySubset::m_pMeshFixationY = nullptr;
ID3DXMesh* BoundarySubset::m_pMeshFixationZ = nullptr;


BoundarySubset::BoundarySubset()
	: m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_strName.Format(_T("Subset N°%d"), m_nSubSetIdx);
	m_nSubSetIdx++;
}

BoundarySubset::BoundarySubset(const std::vector<int> IdxSubSet)
	: m_Indices(IdxSubSet)
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_strName.Format(_T("Subset N°%d"), m_nSubSetIdx);
	m_nSubSetIdx++;
}

BoundarySubset::BoundarySubset(const CString& strName) 
	: m_strName(strName)
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_nSubSetIdx++;
}

BoundarySubset::BoundarySubset(const std::vector<int> IdxSubSet, const CString& strName) 
	: m_Indices(IdxSubSet)
	, m_strName(strName) 
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_nSubSetIdx++;
}



void BoundarySubset::LoadD3DObjects(IDirect3DDevice9* pD3ddev)
{
	// Set the device pointer
	m_pD3ddev = pD3ddev;

	// 1. The vector mesh
	// Load resource to memory
	HRSRC hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MESH_VECTOR), _T("MESH"));
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
		&m_pMeshVector);
	//=======================================================================================================

	// 1. The X Fixation mesh
	// Load resource to memory
	hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MESH_FIXATION_X), _T("MESH"));
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
		&m_pMeshFixationX);
	//=======================================================================================================

	// 1. The Y Fixation mesh
	// Load resource to memory
	hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MESH_FIXATION_Y), _T("MESH"));
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
		&m_pMeshFixationY);
	//=======================================================================================================
	// 1. The Z Fixation mesh
	// Load resource to memory
	hRes = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MESH_FIXATION_Z), _T("MESH"));
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
		&m_pMeshFixationZ);
	//=======================================================================================================
}



void BoundarySubset::DrawDisplacementVector(const Element* pElm, const Vector& U)
{
	if (m_pD3ddev == nullptr)
		return;

	
	// Create the world matrix
	D3DXMATRIX matWorld, matObj, matTrans, matScale;


	D3DXMatrixIdentity(&matObj);

	// Get the rotation axis
	Vector Un = U.UnitVector();
	Vector V0(1.0, 0.0, 0.0);
	Vector k = V0 ^ Un;
	k.Normalize();
	D3DXVECTOR3 kf((float)k.x, (float)k.y, (float)k.z);
	// Get the Rotation angle
	double Theta = acos(Un.x);

	// Rotate the object Matrix
	D3DXMatrixRotationAxis(&matObj, &kf, (float)Theta);
	
	// Check the dot product to see if the Vector is beneath the surface
	double Dot = pElm->m_n * Un;
	bool bBeneath = (Dot < -1.0e-5);
	if (bBeneath)
	{
		D3DXMATRIX matBeneath;
		D3DXMatrixTranslation(&matBeneath,
			(float)(-Un.x * 55.2),
			(float)(-Un.y * 55.2),
			(float)(-Un.z * 55.2));
		matObj = matObj * matBeneath;
	}

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = 3.0 * Element::SpecLength() / 55.2;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);

	Vector Origin = pElm->GetCentroid();
	

	D3DXMatrixTranslation(&matTrans, (float)Origin.x, (float)Origin.y, (float)Origin.z);

	matObj = matObj * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matObj);


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
	D3DMATERIAL9 mtr;
	ZeroMemory(&mtr, sizeof(mtr));
	

	mtr.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
	mtr.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	

	m_pD3ddev->SetMaterial(&mtr);
	m_pMeshVector->DrawSubset(0);

	
	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void BoundarySubset::DrawTractionVector(const Element* pElm, const Vector& T)
{
	if (m_pD3ddev == nullptr)
		return;


	// Create the world matrix
	D3DXMATRIX matWorld, matObj, matTrans, matScale;


	D3DXMatrixIdentity(&matObj);


	// Get the rotation axis
	Vector Tn = T.UnitVector();
	Vector V0(1.0, 0.0, 0.0);
	Vector k = V0 ^ Tn;
	k.Normalize();
	D3DXVECTOR3 kf((float)k.x, (float)k.y, (float)k.z);
	// Get the Rotation angle
	double Theta = acos(Tn.x);

	// Rotate the object Matrix
	D3DXMatrixRotationAxis(&matObj, &kf, (float)Theta);



	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	// Check the dot product to see if the Vector is beneath the surface
	double Dot = pElm->m_n * Tn;
	bool bBeneath = (Dot < -1.0e-5);
	if (bBeneath)
	{
		D3DXMATRIX matBeneath;
		D3DXMatrixTranslation(&matBeneath,
			(float)(-Tn.x * 55.2),
			(float)(-Tn.y * 55.2),
			(float)(-Tn.z * 55.2));
		matObj = matObj * matBeneath;
	}

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = 3.0 * Element::SpecLength() / 55.2;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);

	Vector Origin = pElm->GetCentroid();
	

	D3DXMatrixTranslation(&matTrans, (float)Origin.x, (float)Origin.y, (float)Origin.z);

	matObj = matObj * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matObj);


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
	D3DMATERIAL9 mtr;
	ZeroMemory(&mtr, sizeof(mtr));


	mtr.Ambient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	mtr.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);



	m_pD3ddev->SetMaterial(&mtr);
	m_pMeshVector->DrawSubset(0);


	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void BoundarySubset::DrawNormalDisplacementVector(const Element* pElm, double Un)
{
	if (m_pD3ddev == nullptr)
		return;


	// Create the world matrix
	D3DXMATRIX matWorld, matObj, matTrans, matScale;


	D3DXMatrixIdentity(&matObj);


	// Get the rotation axis
	Vector Vn = pElm->m_n * Un / abs(Un);
	Vector V0(1.0, 0.0, 0.0);
	Vector k = V0 ^ Vn;
	k.Normalize();
	D3DXVECTOR3 kf((float)k.x, (float)k.y, (float)k.z);
	// Get the Rotation angle
	double Theta = acos(Vn.x);

	// Rotate the object Matrix
	D3DXMatrixRotationAxis(&matObj, &kf, (float)Theta);



	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	// Check the dot product to see if the Vector is beneath the surface
	bool bBeneath = (Un < 0.0);
	if (bBeneath)
	{
		D3DXMATRIX matBeneath;
		D3DXMatrixTranslation(&matBeneath,
			(float)(pElm->m_n.x * 55.2),
			(float)(pElm->m_n.y * 55.2),
			(float)(pElm->m_n.z * 55.2));
		matObj = matObj * matBeneath;
	}

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = 3.0 * Element::SpecLength() / 55.2;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);

	Vector Origin = pElm->GetCentroid();
	

	D3DXMatrixTranslation(&matTrans, (float)Origin.x, (float)Origin.y, (float)Origin.z);

	matObj = matObj * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matObj);


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
	D3DMATERIAL9 mtr;
	ZeroMemory(&mtr, sizeof(mtr));


	mtr.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
	mtr.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);



	m_pD3ddev->SetMaterial(&mtr);
	m_pMeshVector->DrawSubset(0);


	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void BoundarySubset::DrawNormalTractionVector(const Element* pElm, double Tn)
{
	if (m_pD3ddev == nullptr)
		return;


	// Create the world matrix
	D3DXMATRIX matWorld, matObj, matTrans, matScale;


	D3DXMatrixIdentity(&matObj);


	// Get the rotation axis
	Vector Vn = pElm->m_n * Tn / abs(Tn);
	Vector V0(1.0, 0.0, 0.0);
	Vector k = V0 ^ Vn;
	k.Normalize();
	D3DXVECTOR3 kf((float)k.x, (float)k.y, (float)k.z);
	// Get the Rotation angle
	double Theta = acos(Vn.x);

	// Rotate the object Matrix
	D3DXMatrixRotationAxis(&matObj, &kf, (float)Theta);



	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	// Check the dot product to see if the Vector is beneath the surface
	bool bBeneath = (Tn < 0.0);
	if (bBeneath)
	{
		D3DXMATRIX matBeneath;
		D3DXMatrixTranslation(&matBeneath,
			(float)(pElm->m_n.x * 55.2),
			(float)(pElm->m_n.y * 55.2),
			(float)(pElm->m_n.z * 55.2));
		matObj = matObj * matBeneath;
	}

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = 3.0 * Element::SpecLength() / 55.2;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);

	Vector Origin = pElm->GetCentroid();
	

	D3DXMatrixTranslation(&matTrans, (float)Origin.x, (float)Origin.y, (float)Origin.z);

	matObj = matObj * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matObj);


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
	D3DMATERIAL9 mtr;
	ZeroMemory(&mtr, sizeof(mtr));


	mtr.Ambient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	mtr.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);



	m_pD3ddev->SetMaterial(&mtr);
	m_pMeshVector->DrawSubset(0);


	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}



void BoundarySubset::DrawFixationX(const Element* pElm)
{
	if (m_pD3ddev == nullptr)
		return;


	// Create the world matrix
	D3DXMATRIX matWorld, matObj, matTrans, matScale;


	D3DXMatrixIdentity(&matObj);


	
	
	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	// Check the dot product to see if the Vector is beneath the surface
	Vector V0(1.0, 0.0, 0.0);
	double Dot = pElm->m_n * V0;
	bool bBeneath = (Dot < -1.0e-5);
	if (bBeneath)
	{
		D3DXMATRIX matBeneath;
		D3DXMatrixRotationZ(&matBeneath, D3DX_PI);
		matObj = matObj * matBeneath;
	}

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = Element::SpecLength() / 50.0;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);

	Vector Origin = pElm->GetCentroid();


	D3DXMatrixTranslation(&matTrans, (float)Origin.x, (float)Origin.y, (float)Origin.z);

	matObj = matObj * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matObj);


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
	D3DMATERIAL9 mtr;
	ZeroMemory(&mtr, sizeof(mtr));


	mtr.Ambient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	mtr.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);



	m_pD3ddev->SetMaterial(&mtr);
	m_pMeshFixationX->DrawSubset(0);


	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}
void BoundarySubset::DrawFixationY(const Element* pElm)
{
	if (m_pD3ddev == nullptr)
		return;


	// Create the world matrix
	D3DXMATRIX matWorld, matObj, matTrans, matScale;


	D3DXMatrixIdentity(&matObj);




	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	// Check the dot product to see if the Vector is beneath the surface
	Vector V0(0.0, 1.0, 0.0);
	double Dot = pElm->m_n * V0;
	bool bBeneath = (Dot < -1.0e-5);
	if (bBeneath)
	{
		D3DXMATRIX matBeneath;
		D3DXMatrixRotationX(&matBeneath, D3DX_PI);
		matObj = matObj * matBeneath;
	}

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = Element::SpecLength() / 50.0;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);

	Vector Origin = pElm->GetCentroid();


	D3DXMatrixTranslation(&matTrans, (float)Origin.x, (float)Origin.y, (float)Origin.z);

	matObj = matObj * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matObj);


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
	D3DMATERIAL9 mtr;
	ZeroMemory(&mtr, sizeof(mtr));


	mtr.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
	mtr.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);



	m_pD3ddev->SetMaterial(&mtr);
	m_pMeshFixationY->DrawSubset(0);


	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}

void BoundarySubset::DrawFixationZ(const Element* pElm)
{
	if (m_pD3ddev == nullptr)
		return;


	// Create the world matrix
	D3DXMATRIX matWorld, matObj, matTrans, matScale;


	D3DXMatrixIdentity(&matObj);


	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	// Check the dot product to see if the Vector is beneath the surface
	Vector V0(0.0, 0.0, 1.0);
	double Dot = pElm->m_n * V0;
	bool bBeneath = (Dot < -1.0e-5);
	if (bBeneath)
	{
		D3DXMATRIX matBeneath;
		D3DXMatrixRotationY(&matBeneath, D3DX_PI);
		matObj = matObj * matBeneath;
	}

	m_pD3ddev->GetTransform(D3DTS_WORLD, &matWorld);


	float Scale = Element::SpecLength() / 50.0;

	D3DXMatrixScaling(&matScale, Scale, Scale, Scale);

	Vector Origin = pElm->GetCentroid();


	D3DXMatrixTranslation(&matTrans, (float)Origin.x, (float)Origin.y, (float)Origin.z);

	matObj = matObj * matScale * matTrans * matWorld;


	m_pD3ddev->SetTransform(D3DTS_WORLD, &matObj);


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
	D3DMATERIAL9 mtr;
	ZeroMemory(&mtr, sizeof(mtr));


	mtr.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
	mtr.Diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);



	m_pD3ddev->SetMaterial(&mtr);
	m_pMeshFixationZ->DrawSubset(0);


	// Restore the previous Render state
	m_pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
	m_pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
	m_pD3ddev->SetFVF(dwPrevFillMode);

	// Reset the World transform
	m_pD3ddev->SetTransform(D3DTS_WORLD, &matWorld);
}


void BoundarySubset::DrawBoundaryConditions(bool bPostProcessing)const
{
	std::vector<Element*>& rElements = *m_pElements;

	// Check if there is a normal displacement
	if (m_bNormalDisplacement && m_Un != 0.0 && m_bShowDispVectors)
	{
		// In this case Draw a normal displacement vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			if (!bPostProcessing)
				DrawNormalDisplacementVector(pElm, m_Un);

		}

		return;
	}

	// Check if there is a normal traction
	if (m_bNormalTraction && m_Tn != 0.0 && m_bShowTracVectors)
	{
		// In this case Draw a traction vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			if (!bPostProcessing)
				DrawNormalTractionVector(pElm, m_Tn);

		}

		return;
	}


	// Check if there is a Total encastrement
	if ((m_bFixed_X && U.x == 0.0) && !m_bNormalDisplacement && m_bShowFixations)
	{
		// In this case Draw a Total Encastrement
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			DrawFixationX(pElm);
		}
	}
	if ((m_bFixed_Y && U.y == 0.0) && !m_bNormalDisplacement && m_bShowFixations)
	{
		// In this case Draw a Total Encastrement
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			DrawFixationY(pElm);
		}
	}
	if ((m_bFixed_Z && U.z == 0.0) && !m_bNormalDisplacement && m_bShowFixations)
	{
		// In this case Draw a Total Encastrement
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			DrawFixationZ(pElm);
		}
	}
	// Check If there is a Displacement vector
	if (((m_bFixed_X && U.x != 0.0) || (m_bFixed_Y && U.y != 0.0) || (m_bFixed_Z && U.z != 0.0)) && m_bShowDispVectors)
	{
		// In this case Draw a Displacement vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			DrawDisplacementVector(pElm, U);
		}
	}


	// Check If there is a traction vector
	if (((!m_bFixed_X && T.x != 0.0) || (!m_bFixed_Y && T.y != 0.0) || (!m_bFixed_Z && T.z != 0.0)) && m_bShowTracVectors)
	{
		// In this case Draw a traction vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			DrawTractionVector(pElm, T);
		}
	}

}



void BoundarySubset::FullyEncaster()
{
	m_bFixed_X = true;
	m_bFixed_Y = true;
	m_bFixed_Z = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.Set(0.0, 0.0, 0.0);
	T.Set(0.0, 0.0, 0.0);
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetUx(double Ux)
{
	m_bFixed_X = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.x = Ux;
	T.x = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetUy(double Uy)
{
	m_bFixed_Y = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.y = Uy;
	T.y = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetUz(double Uz)
{
	m_bFixed_Z = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.z = Uz;
	T.z = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetNormalDisplacement(double Un)
{
	m_bFixed_X = true;
	m_bFixed_Y = true;
	m_bFixed_Z = true;
	m_bNormalDisplacement = true;
	m_bNormalTraction = false;

	U.Set(0.0, 0.0, 0.0);
	T.Set(0.0, 0.0, 0.0);
	m_Un = Un;
	m_Tn = 0.0;
}

void BoundarySubset::SetDisplacementVector(const Vector& Uv)
{
	m_bFixed_X = true;
	m_bFixed_Y = true;
	m_bFixed_Z = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U = Uv;
	T.Set(0.0, 0.0, 0.0);
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::FreeSurface()
{
	m_bFixed_X = false;
	m_bFixed_Y = false;
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.x = 0.0;
	T.y = 0.0;
	T.z = 0.0;

	U.x = 0.0;
	U.y = 0.0;
	U.z = 0.0;

	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetTx(double Tx)
{
	m_bFixed_X = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.x = Tx;
	U.x = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetTy(double Ty)
{
	m_bFixed_Y = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.y = Ty;
	U.y = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetTz(double Tz)
{
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.z = Tz;
	U.z = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void BoundarySubset::SetNormalTraction(double Tn)
{
	m_bFixed_X = false;
	m_bFixed_Y = false;
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = true;

	U.Set(0.0, 0.0, 0.0);
	T.Set(0.0, 0.0, 0.0);
	m_Un = 0.0;
	m_Tn = Tn;
}

void BoundarySubset::SetTractionVector(const Vector& Tv)
{
	m_bFixed_X = false;
	m_bFixed_Y = false;
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.Set(0.0, 0.0, 0.0);
	T = Tv;
	m_Un = 0.0;
	m_Tn = 0.0;
}






void BoundarySubset::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// Store the subset name
		ar << m_strName;
		// Store the size of the index container
		ar << m_Indices.size();
		// Store the index container
		for (int idx : m_Indices)
			ar << idx;
		// store the fixed flag info
		ar << m_bFixed_X;
		ar << m_bFixed_Y;
		ar << m_bFixed_Z;

		// store the normal info
		ar << m_Un; 
		ar << m_bNormalDisplacement;
		ar << m_Tn;
		ar << m_bNormalTraction;
	}
	else
	{
		// Load the subset name
		ar >> m_strName;
		// Load the size of the index container
		size_t N = 0;
		ar >> N;
		// Load the index container
		for (size_t i = 0; i < N; i++)
		{
			int idx;
			ar >> idx;
			m_Indices.push_back(idx);
		}
		// load the fixed flag info
		ar >> m_bFixed_X;
		ar >> m_bFixed_Y;
		ar >> m_bFixed_Z;

		// load the normal info
		ar >> m_Un;
		ar >> m_bNormalDisplacement;
		ar >> m_Tn;
		ar >> m_bNormalTraction;
	}

	U.Serialize(ar);
	T.Serialize(ar);

}