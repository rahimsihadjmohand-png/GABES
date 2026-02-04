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
	unsigned m_N;                   // The Algebraic system DOF
	unsigned m_M;                   // The Displacement coefficient Matrix column size

	// Material information
	double m_E;                     // The Young modulus
	double m_G;                     // The Shear modulus
	double m_v;                     // The poissons ratio

	// Analysis information
	CString m_strCPVmthd;           // the method used to treat CPV integrals
	CString m_strIntegScheme;       // the used integration Scheme (Fixed cubature or Adaptive scheme)
	CString m_str_Q_Matrix;         // the dimension of Q matirx
	CString m_str_RA_Matrix;        // the dimension of R and A matrix
	CString m_str_bx_Vector;        // the dimension of b and x vectors

	// Memory information
	CString m_str_Total_Req;        // The total Required Memory 
	CString m_str_Model_Req;        // The Memory Required for Model
	CString m_str_Q_Req;            // the Q matirx Required Memory
	CString m_str_RA_Req;           // the R and A matrix Required Memory
	CString m_str_Quota_Mem;         // the RAM Quota
	CString m_str_Q_Mem;            // the Q matrix Actual Quota
	CString m_str_R_Mem;            // the R matrix Actual Quota
	CString m_str_A_Mem;            // the A matrix Actual Quota

private:

	static void MemoryToString(CString& strMem, double Mem);
	static void RAM_OOC_ToString(CString& strMem, double RAM, double OOC);

	void OutputInfo(CDC* pDC,
		const CString* const strProperties,
		const CString* const strValues,
		int count);

	void OutputMeshInfo(CDC* pDC);
	void OutputModelInfo(CDC* pDC);
	void OutputMaterialInfo(CDC* pDC);
	void OutputAnalysisInfo(CDC* pDC);
	void OutputMemoryInfo(CDC* pDC);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


