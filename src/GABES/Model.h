#pragma once
#include <vector>
#include <list>
#include "ElementSubSet.h"
#include "LD_Matrix.h" 






namespace BEM_3D
{
	// Definition of AHMED H-Matrix Library DOFs
	// Vertex DOF
	struct AHMED_Vertex 
	{
	public:
		AHMED_Vertex() : pV(nullptr) {}
		AHMED_Vertex(const Vertex* pv) : pV(pv){}

		double getcenter(unsigned i) const;
		double getradius2() const;

		const Vertex* pV; // Reference to the vertex tied to this DOF
	};

	// Element DOF
	struct AHMED_Element 
	{
	public:
		AHMED_Element() : pElm(nullptr) {}
		AHMED_Element(const Element* pelm) : pElm(pelm) {}

		double getcenter(unsigned i) const;
		double getradius2() const;

		const Element* pElm; // Reference to the element tied to this DOF
	};


	enum MESH_TYPE { PURE_CONTINUOUS, PURE_DISCONTINUOUS, DISCONTINUOUS_SHARP_EDGES};

	enum FILL_MODE { SOLID, WIREFRAME, EDGED_FACES };

	enum SHOW_POINTS_MODE {	HIDE_POINTS, SHOW_VERTICES,	SHOW_NODES };

	// a helper structure for the direct computation of Cij free terms
	struct FTCH
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
		HLU,
		GMRES,
		BICGSTAB
	};


	class Model
	{
	public:
		// Constructors
		Model();
		virtual ~Model();

		// Getters
		std::list<ElementSubSet*>& GetSubSets() { return m_SubSets; }
		const std::list<ElementSubSet*>& GetSubSets() const { return m_SubSets; }
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

		unsigned GetQ_Quota()const { return Q.GetMaxRamAlloc() / 1024UL; }
		unsigned GetR_Quota()const { return R.GetMaxRamAlloc() / 1024UL; }
		unsigned GetA_Quota()const { return A.GetMaxRamAlloc() / 1024UL; }

		// Setters
		void SetOutputMode(OUTPUT_MODE outputMode) { m_OutputMode = outputMode; }
		void SetWorkingDirectory(const CString& rhs) { m_strWorkingDirectory = rhs; }
		void SetFileName(const CString& rhs) { m_strFileName = rhs; }
		void SetFileType(const CString& rhs) { m_strFileType = rhs; }
		void AutoSetMatrixMemoryQuotas(); // Sets the matrix memory bails depending on the total bail
		void SetMatrixMemoryQuotas(int Q_Quota, int R_Quota, int A_Quota);

		// Cleaning functions
		void ClearAll();
		void ClearBEMMatrixData();
		void ClearGeometricData();

		// Core BEM functions
		void ConstructCoefficientMatrices();
		void CopyBoundaryConditionsFromSubSets();
		void ConstructLinearSystem();
		void ConstructAndResolveLinearSystem();
		static UINT ConstructAndResolveLinearSystemThread(LPVOID pParam);
		void UpdateBoundaryUnknowns(); // This method places the vector x components ito thier suitable places (Either tractions or displacements)
		static UINT ConstructCoefficientMatricesThread(LPVOID pParam);

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
		void UpdateVertexBuffer(IDirect3DDevice9* pD3ddev, bool bPostTreatment);
		void Draw(IDirect3DDevice9* pD3ddev, FILL_MODE fillMode, bool bPostTreatment, SHOW_POINTS_MODE ShowPointsMode) const;
		void GetHitTestedElement(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, int* pHitTestedElmIdx, const D3DXMATRIX& rWorld)const;
		void GetHitTestedPoint(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, Vertex** ppHitTestedNode, const D3DXMATRIX& rWorld)const;
		IDirect3DVertexBuffer9* GetVertexBuffer() { return m_pVertexBuffer; }


	


	private:
		// Helper method to compute influence coefficient between node <i> and element <j>  
		
		void ComputeInfluenceCoefficients(double** Q_Chunck,  // The Local Element Column Chunck 9xN of the Q Matrix 
										  double** P_Chunck,  // The Local Element Column Chunck 9xN of the P Matrix
										  Matrix* CPV,        // The Cauchy Principal value integrals 3x3 diagonal blocks 
										  int j,              // j: the element column index
										  int m,              // m: the local Element node index
										  int i);             // i: the vertex DOF Row index

		void UpdateVertices();
		// Helper Method that calculates the Static variable (Specific length) of the Element Subset class: used to Draw Boundary Condtions 
		void CalculateSpecificLength()const;

		void TranslateToCenterOfMass();
		Vector GetCenterOfMass()const;

		void ConstructVertexDOFContainer(); // Construncts the vertex DOF vector (shallow copies of Real DOF vertices)
		                                    // Which are Geometric vertices in case of continuous elements
		                                    // and internal nodes in case of discontinuous elements


		void DetectSharpRegions(std::list<Element*>& rSharpRegions)const; // Detects sharp regions
		void MakeSharpRegionsDiscontinous(); // Converts the elements situated in Sharp regions to discontinuous elements 
		void ComputeNumContDiscElms(); // Calculates the number of continuous and discontinuous elements

		void ConstructFTCHContainer(std::list<FTCH>& FTCHs, const Vertex* pV0)const;
		void CalculateFreeTermMatrices(); // Calculates the free term coefficients Cij
		void SetSpecificRadii(); // For each geometrical Dof vertex it computes the Specific Radius R0 Which is the smallest R(theta)

	public:
		double GetMaxValue()const;
		double GetMinValue()const;
	    DWORD GetColorFromValue(const Element* pElm, int nVertex, double MinVal, double MaxVal)const;


	private:
		// Geometrical data 		
		// The MESH
		std::vector<Vertex*> m_Vertices;    // Geometric vertices 
		
		std::vector<Element*> m_Elements;   //<====== Polymorphic vector : the triangular elements

		Vector m_prevCOM; // the previous center of mass used in case we load additional Subsets from Files to keep the relationship between the vertices
		
		unsigned m_Nce; // the number of continuous elements
		unsigned m_Nde; // the number of discontinuous elements

		// The Subsets Used for BCs
		std::list<ElementSubSet*> m_SubSets;
		
		// BEM Data
		// Vertex DOFs
		std::vector<Vertex*> m_DOF_Vertices; // A vector of DOF vertices which can be diffrent from geometrical vertices
		// The vertex pointers are sallow copies and are not owned by the vector 

		// BEM Matrix data
		LD_Matrix<double> Q;  // Displacement influence coefficients
		LD_Matrix<double> R;  // The reduced traction matrix after injecting continuity condition (Reduced automatically during process)
		LD_Matrix<double> A;  // The algebraic system matrix
		double* b;            // The Right Hand Side vector (RHS)
		double* x;            // The vector of unknowns
		

		// D3D data
		IDirect3DVertexBuffer9* m_pVertexBuffer;
		IDirect3DVertexBuffer9* m_pWireVertexBuffer; // Used for Edged Surfaces
		OUTPUT_MODE m_OutputMode;

		CString m_strWorkingDirectory;
		CString m_strFileName;
		CString m_strFileType;

	public:
		double m_CurrentAdvance; // Used to Get the Rate of Advancement of a Lengthy JOB
		bool m_bLengthyJob;      // Used to control the Lengthy Job 
		CString m_strCurrentJob; // The string indicating the current lengthy JOB
		static unsigned m_RAM_Quota_Gb; // The Maximum Gigabytes allowed in RAM Storage
		static MESH_TYPE m_MeshType; // The Model Mesh Discretization type
		static double m_MaxSharpAngle; // The Limit angle to decide if the region is Sharp
		static bool m_bAutoSetRamQuotas;

		// ========== Solver static data ============================================================
		static SOLVER m_Solver;        // The solver type used
		static unsigned m_NumSteps;    // The number of steps if we are using krylov subspace solvers
		static double m_eps;           // The error threshold in the solver
	};
};



