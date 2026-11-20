#pragma once
#include <vector>
#include <list>
#include "BoundarySubset.h"



namespace BEM_3D
{
	enum MESH_TYPE { PURE_CONTINUOUS, PURE_DISCONTINUOUS, DISCONTINUOUS_SHARP_EDGES};

	enum FILL_MODE { SOLID, WIREFRAME, EDGED_FACES };

	enum SHOW_POINTS_MODE {	HIDE_POINTS, SHOW_VERTICES,	SHOW_NODES };

	// a helper structure for the direct computation of Cij free terms
	struct FTCH // FTCH stands for (Free Term Coefficient Helper) structure
	{
		const Vertex* pV0;  // The Vertex where the Cij is to be evaluated
		const Vertex* pVi;  // The Vertex at the end of the triangle Edge

		const Element* pE1; // The first connected element 
		              // (the first element is inserted in the search loop during the new creation of the FTCH)
		const Element* pE2; // The second connected element 
		              // (the second element is inserted in the search loop after failing the redendency test of the FTCH)

		              // The redendency test is performed on the pVi vertex

		Vector r1, r2, n1, n2;		

	};


	enum SOLVER
	{
		H_LU,
		GMRES,
	};


	class Model
	{
	public:
		// Constructors
		Model();
		virtual ~Model();

		// Getters
		std::list<BoundarySubset*>& GetSubSets() { return m_SubSets; }
		const std::list<BoundarySubset*>& GetSubSets() const { return m_SubSets; }
		bool IsModelEmpty() { return m_Vertices.empty() && m_Elements.empty(); }
		std::vector<Vertex*>& GetVertices() { return m_Vertices; }
		std::vector<Vertex*>& GetDOFVertices() { return m_DOF_Vertices; }
		std::vector<Element*>& GetElements() { return m_Elements; }
		const std::vector<Vertex*>& GetVertices()const { return m_Vertices; }
		const std::vector<Vertex*>& GetDOFVertices()const { return m_DOF_Vertices; }
		const std::vector<Element*>& GetElements()const { return m_Elements; }
		OUTPUT_MODE GetOutputMode()const { return m_OutputMode; }
		const CString& GetWorkingDirectory()const { return m_strWorkingDirectory; }
		const CString& GetFileName()const { return m_strFileName; }
		const CString& GetFileType()const { return m_strFileType; }
		unsigned GetNumContElms()const { return m_Nce; }
		unsigned GetNumDiscElms()const { return m_Nde; }

		// Setters
		void SetOutputMode(OUTPUT_MODE outputMode) { m_OutputMode = outputMode; }
		void SetWorkingDirectory(const CString& rhs) { m_strWorkingDirectory = rhs; }
		void SetFileName(const CString& rhs) { m_strFileName = rhs; }
		void SetFileType(const CString& rhs) { m_strFileType = rhs; }
		
		// Cleaning functions
		void ClearAll();
		void ClearBEMVectors();
		void ClearGeometricData();

		// Core BEM functions
		void CopyBoundaryConditionsFromSubSets();
		void ConstructRHSvector();
		
		
		// CORE H2Lib H-Matrix functions =================================================================================================
		void ResolveLinearSystem_H2LIB(LPVOID pParam);
		static UINT ResolveLinearSystemThread_H2LIB(LPVOID pParam);

		void UpdateBoundaryUnknowns(); // This method places the vector x components ito thier suitable places (Either tractions or displacements)
		//================================================================================================================================
		 
		
		// Post-processing BEM recovry functions
		// 1- Boundary strain and stress tensors
		void CalculateTensors(); 		
		// 2- Internal displacements
		double GetDomainDisplacement(const Vertex& DomainPt, int i)const; // i is the direction
		double GetDomainStress(const Vertex& DomainPt, int i, int j)const; // i, j are the 2 tensorial indices
		double GetDomainDispDerivative(const Vertex& DomainPt, int i, int j)const; // i, j are the indices of the displacement derivative dUi/dXj or Ui,j
		double GetDomainStrain(const Vertex& DomainPt, int i, int j)const; // i, j are the 2 tensorial indices

		Vector GetDomainDispVector(const Vertex& DomainPt)const;
		Tensor GetDomainStressTensor(const Vertex& DomainPt)const;
		Tensor GetDomainStrainTensor(const Vertex& DomainPt)const;
		// File Managment functions
	private:
		void LoadMeshFromOBJFile(LPTSTR szFileName);
		void LoadMeshFromSTLFile(LPTSTR szFileName);

	public:
		void LoadMeshFromFile(LPTSTR szFileName);

		void CreateSubSetFromWavefrontObjectFile(LPTSTR szFileName, LPTSTR szFileTitle);
		void CreateSubSetFromSteriolithographyFile(LPTSTR szFileName, LPTSTR szFileTitle);
		


		// Direct 3D functions
		void UpdateVertexBuffer(IDirect3DDevice9* pD3ddev, bool bPostProcessing);
		void Draw(IDirect3DDevice9* pD3ddev, FILL_MODE fillMode, bool bPostProcessing, SHOW_POINTS_MODE ShowPointsMode) const;
		void GetHitTestedElement(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, int* pHitTestedElmIdx, const D3DXMATRIX& rWorld)const;
		void GetHitTestedPoint(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, Vertex** ppHitTestedNode, const D3DXMATRIX& rWorld)const;
		IDirect3DVertexBuffer9* GetVertexBuffer() { return m_pVertexBuffer; }


	


	private:
		void UpdateVertices();

		// Helper Method that calculates the Static variables (ShortestLength & LongestLength) of the Element class: 
		// used to Draw Boundary Condtions 
		// And other Direct 3D user Interface objects
		void CalculateSpecificLengths()const;

		void TranslateToCenterOfMass();
		Vector GetCenterOfMass()const;


		void ComputeReusableIntegrationData();

		void ConstructVertexDOFContainer(); // Construncts the vertex DOF vector (shallow copies of Real DOF vertices)
		                                    // Which are Geometric vertices in case of continuous elements
		                                    // and internal nodes in case of discontinuous elements


		void DetectSharpRegions()const; // Detects sharp vertices
		void MakeSharpRegionsDiscontinous(); // Converts the elements situated in Sharp regions to discontinuous elements 
		void ComputeNumContDiscElms(); // Calculates the number of continuous and discontinuous elements

		void ConstructFTCHContainer(std::list<FTCH>& FTCHs, const Vertex* pV0)const;
		void CalculateFreeTermMatrices(); // Calculates the free term coefficients Cij
		void SetSpecificRadii(); // For each geometrical Dof vertex it computes the Specific Radius R0 Which is the smallest R(theta)

		void DrawMinValueLocation(IDirect3DDevice9* pD3ddev)const;
		void DrawMaxValueLocation(IDirect3DDevice9* pD3ddev)const;

	public:
		double GetMaxValue()const;
		double GetMinValue()const;
		void UpdateMaxLocation();
		void UpdateMinLocation();
	    DWORD GetColorFromValue(const Element* pElm, int nVertex, double MinVal, double MaxVal)const;


		// Storage
		void Serialize(CArchive& ar);

	private:
		// Geometrical data 		
		// The MESH
		std::vector<Vertex*> m_Vertices;    // Geometric vertices 
		
		std::vector<Element*> m_Elements;   //<====== Polymorphic vector : the triangular elements

		Vector m_prevCOM; // the previous center of mass used in case we load additional Subsets from Files to keep the relationship between the vertices
		
		unsigned m_Nce; // the number of continuous elements
		unsigned m_Nde; // the number of discontinuous elements

		// The Subsets Used for BCs
		std::list<BoundarySubset*> m_SubSets;
		
		// BEM Data
		// Vertex DOFs
		std::vector<Vertex*> m_DOF_Vertices; // A vector of DOF vertices which can be diffrent from geometrical vertices
		// The vertex pointers are sallow copies and are not owned by the vector 

		// BEM Linear System Vectors		                    
		double* b;            // The Right Hand Side vector (RHS)
		double* x;            // The vector of unknowns
		

		// D3D data
		IDirect3DVertexBuffer9* m_pVertexBuffer;
		IDirect3DVertexBuffer9* m_pWireVertexBuffer; // Used for Edged Surfaces
		LPD3DXMESH m_pMinMaxLocationMesh;            // A mesh for Min/Max Location  drawing
		OUTPUT_MODE m_OutputMode;
		Vertex* m_pMinLocation;
		Vertex* m_pMaxLocation;

		CString m_strWorkingDirectory;
		CString m_strFileName;
		CString m_strFileType;

	public:
		bool m_bShowMinValLocation;  // This boolean flag controls if the minimum color mapped value location is shown or not
		bool m_bShowMaxValLocation;  // This boolean flag controls if the maximum color mapped value location is shown or not

		
		static MESH_TYPE m_MeshType; // The Model Mesh Discretization type
		static double m_MaxSharpAngle; // The Limit angle to decide if the region is Sharp


		// ========== Solver data ============================================================		
		double m_CurrentAdvance; // Used to Get the Rate of Advancement of a Lengthy JOB
		bool m_bLengthyJob;      // Used to control the Lengthy Job 
		CString m_strCurrentJob; // The string indicating the current lengthy JOB

		// H2LIB
		static double m_Eta;           // The admissibility parameter
		static unsigned int m_clf;     // The maximum leaf size
		static double m_ACA_tol;       // The ACA error tolerance
		static int m_clmode;           // The clustering mode
		static BOOL m_bSetMaxIter;     // The set max-iter flag for GMRES solver
		static unsigned int m_maxiter; // The max iteration number for GMRES solver
		static SOLVER m_solver;        // The solver type 
		static double m_GMRES_tol;     // The GMRES error convergence tolerance
		static double m_HLU_tol;       // The H-LU direct solver truncation tolerance
		static double m_Precond_tol;   // The H-LU preconditionner tolerance  
	};
};



