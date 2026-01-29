#pragma once
#include "Model.h"


namespace BEM_3D
{
	class ReferenceFrame
	{
	public:
		ReferenceFrame();
		ReferenceFrame(CString strName);

		void ToGlobalCoord(Vertex& V_local)const;
		void ToLocalCoord(Vertex& V_Global)const;

		void ToCylindricalCoord(const Vertex& V_local, double& R, double& Theta, double& Z_)const;
		void ToSphericalCoord(const Vertex& V_local, double& R, double& Theta, double& Phi)const;

		static void LoadGizmo(IDirect3DDevice9* pD3ddev);

		void Draw()const;
		bool IsValid() const;

		void SetCoordinateRanges(const Model& rModel);


	public:
		Vertex m_Origin;  // The coordinate of the origine
		Vector m_Vx;      // The unitary vector of the X axis
		Vector m_Vy;      // The unitary vector of the Y axis
		Vector m_Vz;      // The unitary vector of the Z axis

		// Cartezian Range
		double m_X_min;
		double m_X_max;
		double m_Y_min;
		double m_Y_max;
		double m_Z_min;
		double m_Z_max;

		// Cylindrincal Range
		double m_Rc_min;
		double m_Rc_max;
		
		// Spherical Range
		double m_Rs_min;
		double m_Rs_max;


		// Mathematically speaking we only need 6 variables to define a new frame in 3D Euclidian space
		// 3 coordinates of the origin and 3 Euler angles. However to facilitate the Frame definition
		// in the user's perspective we will define the frame using its 3 unitary orthogonal vectors!!
		// and We will force them to be an orthogonal direct triad!!

		CString m_strName;
		bool m_bSelected;
		static UINT m_nRefFrameIdx;

	public:
		// Make the mesh data static
		static IDirect3DDevice9* m_pD3ddev;
		static ID3DXMesh* m_pMeshXAxis;
		static ID3DXMesh* m_pMeshYAxis;
		static ID3DXMesh* m_pMeshZAxis;
	};

};


