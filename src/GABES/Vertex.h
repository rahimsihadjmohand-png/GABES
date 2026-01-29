#pragma once
#include "Vector.h"
#include "Tensor.h"
#include <vector>

namespace BEM_3D
{

	struct D3DVertex
	{
		float x, y, z;
		D3DXVECTOR3 n;
	};

	struct D3D_RGB_Vertex
	{
		float x, y, z;
		DWORD Color;
	};

	const int g_FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
	const int g_RGB_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	class Vertex : public Vector
	{
	public:
		// Constructors
		Vertex();
		Vertex(double x_, double y_, double z_);
		Vertex(double x_, double y_, double z_, bool bGeometrucal);
		Vertex(const Vector& V);
		virtual ~Vertex() {}

		// ======Utility functions=====================
		// getters
		double Distance(const Vertex& rhs)const;
		bool IsDispFixed(int k)const;

		// setters
		void SetDispFixed(int k, bool bFixed);
		//============================================


		//======== D3D functions =====================
		static void LoadMesh(IDirect3DDevice9* pD3ddev, double SpecLength);
		void SetD3dVertexPosition(D3DVertex& rD3dVert, bool bPostTreatment)const;
		void SetD3dRGBVertexPosition(D3D_RGB_Vertex& rD3dVert, bool bPostTreatment)const;
		void Draw(bool bPostTreatment)const;
		//============================================


	public:
		// ======================================== BEM data ===================================================
		Vector U; // The displacement vector
		
		bool m_bFixed_X;
		bool m_bFixed_Y;
		bool m_bFixed_Z;

		bool m_bGeometrical; // Informs us if the vertex is also a geometrical vertex

		// Utility variables needed for CPV evaluation
		Matrix C; // The free term matrix 
		double R0; // This is the length of the smallest edge around the vertex divided by 2
	

		// The static variable
		static double m_DeformationScale;
		static bool m_bIncludeDisplacements;  // this variable is set to true if we want show the deformed body
		static IDirect3DDevice9* m_pD3ddev;   // A copy of the view D3D device
		static LPD3DXMESH m_pMesh;            // A statical mesh for nodes drawing
		//=======================================================================================================
	};
};



