#pragma once
#include "Element.h"

// Element_D12 : Implements a semi-discontinuous element (Discontinuous at vertex 2)

namespace BEM_3D
{
	class Element_D12 : public Element
	{
	public:
		// ============================== Constructors =================================================================
		Element_D12();
		Element_D12(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3);
		Element_D12(const Vertex& rV1, const Vertex& rV2, const Vertex& rV3); // Dummy Element used just for element subdivision and it is not a real element of the mesh
		virtual ~Element_D12() {}
		// =============================================================================================================

		// ============================= DOF Vertex Getters =================================================================
		virtual const Vertex& Vdof(int l)const; // Gets the Node DOF of index l = 0, 1, 2 (constant version) 
		virtual Vertex& Vdof(int l);            // Gets the Node DOF of index l = 0, 1, 2 (non constant version)
		virtual const Vertex* Vdof_Ptr(int l)const; // Get the Node DOF of index l = 0, 1, 2 in pointer form (constant version) 
		virtual Vertex* Vdof_Ptr(int l);            // Get the Node DOF of index l = 0, 1, 2 in pointer form (non constant version)
		// ===================================================================================================================

		// Type Getter
		virtual ELEMENT_TYPE inline Type()const { return ELEMENT_D12; }

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


		// =============== Storage===================== 
		virtual void Serialize(CArchive& ar);
		// ============================================



		// ********************* Specific for discontinuous elements **************************************
		void CalculateControlNodes(); // This method calculate the internal control nodes from the geometrical boundary nodes
		// ************************************************************************************************


		//========================= SINGULARITY TREATMENT ==========================================================
		// H1: the integrand in the subtriangles around the 2nd internal node: 
		// l: shape function index, st: subtriangle index
		double H1(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j, int l, int st)const;
		// H: the integrand around the 1st and 3rd Vertex s = 0 or s = 2, l: shape function index 
		double H(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j, int s, int l)const;

		double ThetaIntegral(double (Element_D12::* Func)(const Vertex&, const Vertex&, const Vertex&, double, int, int, int, int)const,
			const Vertex& V1, const Vertex& V2, const Vertex& V3, int i, int j, int k1, int k2)const;
		//double Prn(int i, int j, int l)const;
		double Psn(int i, int j, int l)const;

		// The CPV integral 
		double Ps(int i, int j, int s, int l)const;
		//===============================================================================================================


	private:
		// ============================ Reusable Shape Function Values  =============================
		virtual double N1s(int nRule, int nNode)const { return N1s_D12E[nRule][nNode]; }
		virtual double N2s(int nRule, int nNode)const { return N2s_D12E[nRule][nNode]; }
		virtual double N3s(int nRule, int nNode)const { return N3s_D12E[nRule][nNode]; }
		// ==========================================================================================


	private:
		// The three control Nodes
		Vertex m_Node2; // node 2

		// The reusable shape functions
		static double N1s_D12E[17][208];
		static double N2s_D12E[17][208];
		static double N3s_D12E[17][208];
		static bool m_bD12ERsblShpFctsComputed;
	};
};




