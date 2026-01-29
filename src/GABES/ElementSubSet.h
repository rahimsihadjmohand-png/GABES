#pragma once
#include "Element.h"
#include <vector>

namespace BEM_3D
{
	class ElementSubSet
	{
	public:
		// Constructors
		ElementSubSet();
		ElementSubSet(const std::vector<int> IdxSubSet);
		ElementSubSet(const CString& strName);
		ElementSubSet(const std::vector<int> IdxSubSet, const CString& strName);
		virtual~ElementSubSet() {};


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
		void DrawBoundaryConditions(IDirect3DDevice9* pD3ddev, bool bPostTreatment)const;

	public:
		CString m_strName;
		std::vector<int> m_Indices;
		static std::vector<Element*>* m_pElements;  // A reference to the Elements container
		static double SpecLength; // This is the Longest Element Side in the Mesh, used for BC vectors drawing
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
	};

};

