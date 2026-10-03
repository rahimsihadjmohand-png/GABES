#pragma once
#include "Element.h"

// Element_C : Implements a totally continuous element

namespace BEM_3D
{
	class Element_C : public Element
	{
	public:
		// ============================== Constructors =================================================================
		Element_C();
		Element_C(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3);
		Element_C(Vertex& rV1, Vertex& rV2, Vertex& rV3); // Dummy Element used just for element subdivision and it is not a real element of the mesh
		virtual ~Element_C() {}
		// =============================================================================================================

		// ============================= DOF Vertex Getters =================================================================
		virtual const Vertex& Vdof(int l)const; // Gets the Node DOF of index l = 0, 1, 2 (constant version) 
		virtual Vertex& Vdof(int l);            // Gets the Node DOF of index l = 0, 1, 2 (non constant version)
		virtual const Vertex* Vdof_Ptr(int l)const; // Get the Node DOF of index l = 0, 1, 2 in pointer form (constant version) 
		virtual Vertex* Vdof_Ptr(int l);            // Get the Node DOF of index l = 0, 1, 2 in pointer form (non constant version)
		// ===================================================================================================================

		// Type Getter
		virtual ELEMENT_TYPE inline Type()const { return ELEMENT_C; }


		// ============================ Shape Functions ===========================================
		virtual inline double N1(double Xi, double Eta)const;
		virtual inline double N2(double Xi, double Eta)const;
		virtual inline double N3(double Xi, double Eta)const;
		// ========================================================================================

		// ============================ Influence Coefficients ====================================
		virtual double Q(const Vertex* pSrcPt, int i, int j, int l)const;
		virtual double P(const Vertex* pSrcPt, int i, int j, int l)const;
		// ========================================================================================

		// =========================== Reusable shape functions calculations ======================
		virtual void ComputeReusableShapeFunctions();   // This function is called once !!!!!
		// ========================================================================================


		//========================= SINGULARITY TREATMENT ==========================================================
		double H(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j, int l_)const;
		double Ps(const Vertex& V1, const Vertex& V2, const Vertex& V3, int i, int j, int l_) const;
		// ===============================================================================================================


		
	private:
		// ============================ Reusable Shape Function Values  =============================
		virtual double N1s(int nRule, int nNode)const { return N1s_CE[nRule][nNode]; }
		virtual double N2s(int nRule, int nNode)const { return N2s_CE[nRule][nNode]; }
		virtual double N3s(int nRule, int nNode)const { return N3s_CE[nRule][nNode]; }
		// ==========================================================================================


	private:
		// The Reusable shape functions
		static double N1s_CE[17][208];
		static double N2s_CE[17][208];
		static double N3s_CE[17][208];

		static bool m_bCERsblShpFctsComputed;
	};
};


