#pragma once
#include "Element.h"
#include <vector>

namespace BEM_3D
{
	class BoundarySubset
	{
	public:
		// Constructors
		BoundarySubset();
		BoundarySubset(const std::vector<int> IdxSubSet);
		BoundarySubset(const CString& strName);
		BoundarySubset(const std::vector<int> IdxSubSet, const CString& strName);
		virtual~BoundarySubset() {};


		// Boundary Condition Utility functions
		void FullyEncaster();
		void SetUx(double Ux);
		void SetUy(double Uy);
		void SetUz(double Uz);
		void SetNormalDisplacement(double Un);
		void SetDisplacementVector(const Vector& Uv);
		void FreeSurface();
		void SetTx(double Tx);
		void SetTy(double Ty);
		void SetTz(double Tz);
		void SetNormalTraction(double Tn);
		void SetTractionVector(const Vector& Tv);



		// Direct 3D Functions
		void DrawBoundaryConditions(bool bPostProcessing)const;


		// Storage
		void Serialize(CArchive& ar);


		static void LoadD3DObjects(IDirect3DDevice9* pD3ddev);

	private:
		static void DrawDisplacementVector(const Element* pElm, const Vector& U);
		static void DrawTractionVector(const Element* pElm, const Vector& T);
		static void DrawNormalDisplacementVector(const Element* pElm, double Un);
		static void DrawNormalTractionVector(const Element* pElm, double Tn);
		static void DrawFixationX(const Element* pElm);
		static void DrawFixationY(const Element* pElm);
		static void DrawFixationZ(const Element* pElm);

	public:
		CString m_strName;
		std::vector<int> m_Indices;
		static std::vector<Element*>* m_pElements;  // A reference to the Elements container		
		static bool m_bShowFixations;
		static bool m_bShowDispVectors;
		static bool m_bShowTracVectors;
		
		Vector U; // The displacement vector
		
		Vector T; // The traction vector
		
		bool m_bFixed_X;
		bool m_bFixed_Y;
		bool m_bFixed_Z;
		
		double m_Un; bool m_bNormalDisplacement;
		double m_Tn; bool m_bNormalTraction;


		static UINT m_nSubSetIdx; // Used for Default naming

		// Make the mesh data static
		static IDirect3DDevice9* m_pD3ddev;
		static ID3DXMesh* m_pMeshVector;
		static ID3DXMesh* m_pMeshFixationX;
		static ID3DXMesh* m_pMeshFixationY;
		static ID3DXMesh* m_pMeshFixationZ;
	};

};

