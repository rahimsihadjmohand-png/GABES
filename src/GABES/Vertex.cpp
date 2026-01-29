#include "pch.h"
#include "Vertex.h"

using namespace BEM_3D;


double Vertex::m_DeformationScale = 100000.0;
bool Vertex::m_bIncludeDisplacements = false;
IDirect3DDevice9* Vertex::m_pD3ddev = nullptr; 
LPD3DXMESH Vertex::m_pMesh = nullptr;          


Vertex::Vertex()
	: Vector()
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bGeometrical(false)
{

}

Vertex::Vertex(double x_, double y_, double z_) 
	: Vector(x_, y_, z_)
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bGeometrical(false)
{

}


Vertex::Vertex(double x_, double y_, double z_, bool bGeometrical)
	: Vector(x_, y_, z_)
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bGeometrical(bGeometrical)
{

}


Vertex::Vertex(const Vector& V) 
	: Vector(V)
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bGeometrical(false)
{

}


double Vertex::Distance(const Vertex& rhs)const
{
	Vector Disp = *this - rhs;

	return Disp.Magnitude();
}


void Vertex::SetD3dVertexPosition(D3DVertex& rD3dVert, bool bPostTreatment)const
{
	if (m_bIncludeDisplacements && bPostTreatment)
	{
		rD3dVert.x = x + m_DeformationScale * U.x;
		rD3dVert.y = y + m_DeformationScale * U.y;
		rD3dVert.z = (z + m_DeformationScale * U.z);		
	}
	else
	{
		rD3dVert.x = x;
		rD3dVert.y = y;
		rD3dVert.z = z;
	}	
}

void Vertex::SetD3dRGBVertexPosition(D3D_RGB_Vertex& rD3dVert, bool bPostTreatment)const
{
	if (m_bIncludeDisplacements && bPostTreatment)
	{
		rD3dVert.x = x + m_DeformationScale * U.x;
		rD3dVert.y = y + m_DeformationScale * U.y;
		rD3dVert.z = (z + m_DeformationScale * U.z);		
	}
	else
	{
		rD3dVert.x = x;
		rD3dVert.y = y;
		rD3dVert.z = z;
	}
}


bool Vertex::IsDispFixed(int k)const
{
	switch (k)
	{
	case 0:
		return m_bFixed_X;

	case 1:
		return m_bFixed_Y;

	case 2:
		return m_bFixed_Z;

	default:
		break;
	}

	return false; // Impossible value
}



void Vertex::SetDispFixed(int k, bool bFixed)
{
	switch (k)
	{
	case 0:
		m_bFixed_X = bFixed;
		break;

	case 1:
		m_bFixed_Y = bFixed;
		break;

	case 2:
		m_bFixed_Z = bFixed;
		break;


	default:
		break;
	}
}


void Vertex::LoadMesh(IDirect3DDevice9* pD3ddev, double SpecLength)
{
	// Set the Device pointer
	m_pD3ddev = pD3ddev;

	// Load the sphere mesh
	float R = (float)SpecLength / 20.0f;
	D3DXCreateSphere(m_pD3ddev, R, 4, 4, &m_pMesh, NULL);
}



void Vertex::Draw(bool bPostTreatment)const
{
	D3DXMATRIX translation;
	D3DXMATRIX world;
	D3DXMATRIX newWorld;

	m_pD3ddev->GetTransform(D3DTS_WORLD, &world);


	D3DVertex d3d_V;
	SetD3dVertexPosition(d3d_V, bPostTreatment);	
	
	D3DXMatrixTranslation(&translation, d3d_V.x, d3d_V.y, d3d_V.z);


	newWorld = translation * world;

	m_pD3ddev->SetTransform(D3DTS_WORLD, &newWorld);



	m_pMesh->DrawSubset(0);


	m_pD3ddev->SetTransform(D3DTS_WORLD, &world);
}