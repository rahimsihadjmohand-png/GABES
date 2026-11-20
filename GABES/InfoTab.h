#pragma once
#include "GABESDoc.h"
#include "GABESView.h"

// CInfoTab

class CInfoTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CInfoTab)

public:
	CInfoTab();
	virtual ~CInfoTab();

	CGABESDoc* m_pDoc;
	CGABESView* m_pView;


	void InitTabCtrl();
	void UpdateInfo();


private:
	// Mesh information
	CString m_strWorkingDirectory;  // The Mesh placeholder folder (which is the default working directory)
	CString m_strMeshFileTitle;     // The Mesh file name (title)
	CString m_strFileType;          // The Mesh file type (.Obj) or (.stl)
	unsigned m_Ne;                  // The number of triangles
	unsigned m_Nv;                  // The number of vertices

	// Model information
	CString m_strMeshType;          // Continuous , Discontinuous or Discontinuous at Edges
	unsigned m_Nv_dof;              // The number of DOF vertices
	unsigned m_Nce;                 // The number of continuous elements
	unsigned m_Nde;                 // The number of discontinuous elements
	unsigned m_DOF;                 // The Algebraic system DOF

	// Material information
	double m_E;                     // The Young modulus
	double m_G;                     // The Shear modulus
	double m_v;                     // The poissons ratio

	// Integration information
	CString m_strIntegScheme;       // The used integration Scheme (Fixed cubature or Adaptive scheme)
	CString m_strIntegAccuracy;     // The integration error threshold
	CString m_strNumCubaturePoints; // The integration error threshold
	CString m_strRISP;              // The state of use of the RISP Algorithm

	// H-Matrix information
	CString m_strClusterMode;       // The type of H-Matrix clusturing mode used
	CString m_strACA_tol;           // The ACA error tolerance
	CString m_strEta;               // The H-Matrix Admissibility parameter
	CString m_strClf;               // The H-Matrix Maximum Leaf Size

	// Solver information
	CString m_strSolver;            // The type of linear system solver used
	CString m_strHLU_tol;           // The H-LU truncation error
	CString m_strGMRES_tol;         // The GMRES Error threshhold
	CString m_strPrecond_tol;       // The H-LU preconditionner truncation error
	CString m_strMaxIterations;     // The use or not of a max number of iteration if yes display it
	

private:

	void OutputInfo(CDC* pDC,
		const CString* const strProperties,
		const CString* const strValues,
		int count);

	void OutputMeshInfo(CDC* pDC);
	void OutputModelInfo(CDC* pDC);
	void OutputMaterialInfo(CDC* pDC);
	void OutputIntegrationInfo(CDC* pDC);
	void OutputHMatrixInfo(CDC* pDC);
	void OutputSolverInfo(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


