#pragma once
#include "Vertex.h"

namespace BEM_3D
{
	// Struct to define integration nodes intrinsic coordinates and their weights
	struct ITND
	{
		ITND();
		ITND(double Xi_, double Nu_, double W_);

		double Xi;
		double Eta;
		double W;
	};

	
	// An enumeration that define the type of integration rule to use
	enum INTEG_RULE
	{
		_001_PT,   // 01 point  - 01 triangle       //////////////////////////////////////////////////////////
		_003_PT,   // 03 points - 01 triangle       // these 04 simple rules without splitting are just     //
		_004_PT,   // 04 points - 01 triangle ====> // included for convenience purposes they are generally //
		_007_PT,   // 07 points - 01 triangle       // non accurate in real BEM problems                    //
		_013_PT,   // 13 points - 01 triangle       //////////////////////////////////////////////////////////

		_028_PT,   // 07 points - 04 triangles      
		_052_PT,   // 13 points - 04 triangles       
		_056_PT,   // 07 points - 08 triangles ===> this is the rule used by default
		_104_PT,   // 13 points - 08 triangles
		_112_PT,   // 07 points - 16 triangles
		_208_PT,   // 13 points - 16 triangles
		
		_WS_NODE1, // A special rule used in case the weakly singular point is the first node in a discontinuous element
		_WS_NODE2, // A special rule used in case the weakly singular point is the second node in a discontinuous element
		_WS_NODE3, // A special rule used in case the weakly singular point is the third node in a discontinuous element
		_WS_VERT1, // A special rule used in case the weakly singular point is the first vertex in continuous element
		_WS_VERT2, // A special rule used in case the weakly singular point is the second vertex in continuous element
		_WS_VERT3, // A special rule used in case the weakly singular point is the third vertex in continuous element
	};


	enum OUTPUT_MODE
	{
		NONE, // No Data to output 
		// The Displacement vector
		U_1, // Ux
		U_2, // Uy
		U_3, // Uz
		U_M, // The Displacement Module
		// The Stress Tensor
		S_11, // Sig_11
		S_22, // Sig_22
		S_33, // Sig_33
		S_12, // Sig_12
		S_13, // Sig_13
		S_23, // Sig_23
		S_VON_MISES, // Von Mises stress
		S_TRESCA, // Tresca stress
		// The Strain Tensor
		E_11, // Eps_11
		E_22, // Eps_22
		E_33, // Eps_33
		E_12, // Eps_12
		E_13, // Eps_13
		E_23, // Eps_23
		E_VON_MISES, // Von Mises stress
		E_TRESCA, // Tresca stress

	};

	enum ELEMENT_TYPE
	{
		CONTINUOUS,
		DISCONTINUOUS
	};

	// The Core element class 
	class Element
	{
	public:
		// Constructors
		Element();
		Element(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3);
		virtual ~Element() {}

	// ====================================== Utility functions ================================
	public:
		/*==>*/virtual const Vertex& Vdof(int l)const = 0; // Gets the Node DOF of index l = 0, 1, 2 (constant version) 
		/*==>*/virtual Vertex& Vdof(int l) = 0;            // Gets the Node DOF of index l = 0, 1, 2 (non constant version)
		/*==>*/virtual const Vertex* Vdof_Ptr(int l)const = 0;        // Get the Node DOF of index l = 0, 1, 2 in pointer form (constant version) 
		/*==>*/virtual Vertex* Vdof_Ptr(int l) = 0;        // Get the Node DOF of index l = 0, 1, 2 in pointer form (non constant version)
		/*==>*/virtual inline ELEMENT_TYPE Type()const = 0;       // Returns the element runtime type, needed in polymorphisme
		/*==>*/virtual double CollFactor()const { return 0.0; };  // Returns the collocation factor
		
		// Get the Geometrical vertex from local index
		const Vertex& Vgeo(int l) const;
		Vertex& Vgeo(int l);
		const Vertex* Vgeo_Ptr(int l) const;
		Vertex* Vgeo_Ptr(int l);

		const Vector& T(int l)const; // Get the traction vector of local index l = 0, 1, 2 (constant version) 
		Vector& T(int l); // Get the traction vector of local index l = 0, 1, 2 (constant version)
		const Tensor& Sig(int l)const; // Get the stress tensor of local index l = 0, 1, 2 (constant version) 
		Tensor& Sig(int l); // Get the stress tensor of local index l = 0, 1, 2 (constant version)
		const Tensor& Eps(int l)const; // Get the strain tensor of local index l = 0, 1, 2 (constant version) 
		Tensor& Eps(int l); // Get the strain tensor of local index l = 0, 1, 2 (constant version)
		
		int GeometricalIdx(int l)const; // Returns the global index of the geometric vertex
		int DofIdx(int l)const;         // Returns the global index of the DOF Node

		void SetDofIdx(int l, int Idx); // Sets the global index of the DOF Node                    

		//double GetValue(double Xi, double Eta, OUTPUT_MODE om)const;
		double GetValue(int nInternalNode, OUTPUT_MODE om)const;    // Gets the value of a variable in a vertex node
		                                                            //   .. No need to override it
		
		bool IsVertexPartOf(const Vertex* pV)const;                 // Checks if the vertex is part of the element
		                                                            // .. No need to override it


		bool IsGeometricalVertexPartOf(const Vertex* pV)const;      // Checks if the vertex is part of the element
		                                                            //   .. No need to override it

		bool IsPointInside(const Vertex& DomainPt, double& Xi, double& Eta)const;            // Checks if a point in inside the element

		
		int GetVertexLocalIndex(const Vertex* pV)const;             // Gets the vertex local index 1, 2 or 3   
																	// .. No need to override it

		int GetGeometricalVertexLocalIndex(const Vertex* pV)const;             // Gets the vertex local index 1, 2 or 3   
		                                                            //   .. No need to override it

				
		Vertex GetCentroid()const; // Used in AHMED H-Matrix Solver to construct cluster trees 
		                           // No need to overload it
		
		double GetRadius2()const;  // Used in AHMED H-Matrix Solver to construct cluster trees
		                           // No need to overload it

		void CalculateTensors();  // Calculate the three tensors at vertex points

		virtual void UpdateGeometricalVertices() {}; // Updates the geometrical vertices


		// A helper static function from a given local index I0 it checks if 
											 // the second index is next in the CCW order
		static bool IsNext(int I0, int idx)
		{
			return(idx == ((I0 + 1) % 3));
		}
		
		virtual Vertex GetFieldPoint(double Xi, double Eta)const;               // no need to override it

		virtual double GetFieldDisp(double Xi, double Eta, int i)const;
		virtual double GetFieldStress(double Xi, double Eta, int i, int j)const;
		virtual double GetFieldStrain(double Xi, double Eta, int i, int j)const;

	protected:
		void CalculateNormal(); // Calculates the outward normal based on the triangle geometry 
		                        // No need to overload it
	    
		static void GetSig(Tensor& rSig, const Vector& P, double e11, double e22, double e12);
		static void GetEps(Tensor& rEps, const Tensor& rSig);
								
	// =========================================================================================

	// ==================================== D3D Functions ======================================
    // These functions don't need to be overriden
	public:
		void DrawTotalEncastrement(IDirect3DDevice9* pD3ddev, bool bPostTreatment)const;
		void DrawPartialEncastrement(IDirect3DDevice9* pD3ddev, bool bPostTreatment)const;
		void DrawDisplacementVector(IDirect3DDevice9* pD3ddev, const Vector& U, double Length, bool bPostTreatment)const;
		void DrawTractionVector(IDirect3DDevice9* pD3ddev, const Vector& T, double Length, bool bPostTreatment)const;
		void DrawNormalDisplacementVector(IDirect3DDevice9* pD3ddev, double Un, double Length, bool bPostTreatment)const;
		void DrawNormalTractionVector(IDirect3DDevice9* pD3ddev, double Tn, double Length, bool bPostTreatment)const;
	// =========================================================================================
	


	// ================================== BEM Elementary Functions =============================
	public:
		// The shape functions
		/*==>*/virtual inline double N1(double Xi, double Eta)const = 0;  // Has to be overriden in child classes   
		/*==>*/virtual inline double N2(double Xi, double Eta)const = 0;  // Has to be overriden in child classes
		/*==>*/virtual inline double N3(double Xi, double Eta)const = 0;  // Has to be overriden in child classes
		double N(int l, double Xi, double Eta)const; // Generic shape function no need to verride it

	protected:
		// Other elementary BEM functions
		virtual void GetFieldPoint(Vertex* pFldPt, double Xi, double Eta)const; // no need to override it
		static inline double KD(int i, int j);                                  
		static inline double r(const Vertex& SrcPt, const Vertex& FldPt);
		static double dr(const Vertex& SrcPt, const Vertex& FldPt, int k); // the index k is the direction: and is 0 based 
		static Vector dr(const Vertex& SrcPt, const Vertex& FldPt);
		double dr_dn(const Vertex& SrcPt, const Vertex& FldPt)const; // no need to override it
		double n(double Xi, double Eta, int k)const; // the index k is the direction: and is 0 based  // no need to override it
		Vector J(double Xi, double Eta)const;        // no need to override it                
		double Jn(double Xi, double Eta)const; // The Jacobian of the transformation (Its a linear interpolation so J dont depend on the intrinsic coordinates)
		                                                // no need to override it

		// Fundamental solutions 
		double U(const Vertex& SrcPt, int nRule, int nNode, int i, int j)const; // no need to override it
		double T(const Vertex& SrcPt, int nRule, int nNode, int i, int j)const; // no need to override it
		
		double D(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const; // no need to override it
		double S(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const; // no need to override it
	    double W(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const; // no need to override it
		double V(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const; // no need to override it
		//double T(const Vertex& SrcPt, double Xi, double Eta, int i, int j)const;

		// Integrands : which are the fundamental solutions multiplied by the shape functions and the jacobian
		double U_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int l)const; // no need to override it
		double T_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int l)const; // no need to override it
		double D_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const; // no need to override it
		double S_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const; // no need to override it
		double W_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const; // no need to override it
		double V_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const; // no need to override it
		//double T_(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int l)const;
		
		// Direct C.P.V Integrals (Singular Part)
		double f(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j)const;
		double GetTheta(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Eta_)const;
		double g(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Eta_)const;
		double Ro_(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta)const;
		double H(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Eta_, int i, int j)const;
		double Ps(const Vertex& V1, const Vertex& V2, const Vertex& V3, int i, int j) const;
		/*double Jn(int l)const;*/

		//virtual double H(double Eta_, int i, int j, int l)const {return 0.0;}; // the singular function to integer between Theta1 and Theta 2
		virtual double Ps(int i, int j, int l) const = 0;
		virtual double Pr(int i, int j, int l) const { return 0.0; }

		// Utilitary static functions
		static int RuleToIndex(INTEG_RULE rule); // Gets the index of the rule Array from the identifiant of the Rule 
		static int RuleToSize(INTEG_RULE rule); // Gets the Size of the rule Array from the identifiant of the Rule
		static void Get1DGaussianRule(double Xi[], double W[], int N);
		static void Construct2DGaussianRule(ITND _2D_Rule[], int N1, int N2);

		INTEG_RULE SelectIntegrationRule(const Vertex& SrcPtm, int nKernelType)const;
				
	public: 
		// Helper functions
		// This function is used to compute re-usable integration data, which is called when the Mesh is loaded
		void ComputeReusableIntegrationData();
		static void FillIntegrationNodes();

		// The shape functions
		virtual double N1s(int nRule, int nNode)const = 0;
		virtual double N2s(int nRule, int nNode)const = 0;
		virtual double N3s(int nRule, int nNode)const = 0;


	public:
		// Influence coefficients: which are the integrated quantities U_, T_, D_, S_, W_ and V_ 
		/*==>*/virtual double Q(const Vertex& SrcPt, int i, int j, int l)const = 0; // The surface integral of the quantity U_
		                                                                         // This must be overriden because the treatment
		                                                                        // of Weakly singular integrals differs between 
		                                                                        // Discontinuous and continuous elements

		/*==>*/virtual double P(const Vertex& SrcPt, int i, int j, int l)const = 0; // The surface integral of the quantity P_
		                                                                         // This must be overriden because the treatment
		                                                                        // of CPV singular integrals differs between 
		                                                                        // Discontinuous and continuous elements
		
		// Integrated D_
		double ID(const Vertex& SrcPt, int i, int j, int k, int l)const;
		// Integrated S_
		double IS(const Vertex& SrcPt, int i, int j, int k, int l)const;
		// Integrated W_
		double IW(const Vertex& SrcPt, int i, int j, int k, int l)const;
		// Integrated V_
		double IV(const Vertex& SrcPt, int i, int j, int k, int l)const;
	//===================================================================================================================================================

	//========================= Utility GUI Functions ===============================
		bool GetIntersectingElm(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, float* pAlpha, const D3DXMATRIX& rWorld)const;
	//=================================================================================

	public:
		// Geometrical data
		Vertex* m_pV1;  // Vertex 1
		Vertex* m_pV2;  // Vertex 2
		Vertex* m_pV3;  // Vertex 3
		int m_I1;       // Index of vetex 1
		int m_I2;       // Index of vetex 2
		int m_I3;       // Index of vetex 3
		Vector m_n;     // The normal of the element


		//=============================== BEM Data ======================================
		// The indices of DOF Nodes
		int m_J1;
		int m_J2;
		int m_J3;
		
		// the three traction vectors at nodal points
		Vector m_T1; // the traction at node 1
		Vector m_T2; // the traction at node 2
		Vector m_T3; // the traction at node 3
		

		// The three stress tensors at vertex points
		Tensor m_Sig1;
		Tensor m_Sig2;
		Tensor m_Sig3;

		// The three strain tensors at vertex points
		Tensor m_Eps1;
		Tensor m_Eps2;
		Tensor m_Eps3;


		// static mechanical data
		static double E;  // Young modulus
		static double v;  // Poisson's ratio
		static double G;  // Shear modulus
			

		// Static integration data 
		static INTEG_RULE m_IntegrationRule;
		static ITND ITNDs[17][208];
		static bool bIntegNodesFilled;
		static bool m_bRigidBodyCPV;  // Use the Rigid body method to recover the CPV integrals


		// Reusable integration Data
		double Js[17][208];

		


		Vertex FldPts[17][208];
		//===================================================================================================
	};
};



