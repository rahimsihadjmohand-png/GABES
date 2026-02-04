#include "pch.h"
#include "Model.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ppl.h>
#include "MainFrm.h"
#include "ContElm.h"
#include "DiscElm.h"
#include <atomic>



namespace ccy = concurrency;


using namespace BEM_3D;



#define _L Element::Lambda


unsigned Model::m_RAM_Quota_Gb = 28; // 28 Gb of memory
MESH_TYPE Model::m_MeshType = DISCONTINUOUS_SHARP_EDGES;
double Model::m_MaxSharpAngle = M_PI_4;
bool Model::m_bAutoSetRamQuotas = false;


Model::Model()
      // the extension .ldmat stands for: large and dense matrix
	: Q(m_RAM_Quota_Gb / 2 * 1024, _T("Q.ldmat")) 
	, R(m_RAM_Quota_Gb / 4 * 1024, _T("R.ldmat"))
	, A(m_RAM_Quota_Gb / 4 * 1024, _T("A.ldmat"), &m_CurrentAdvance)
	, b(nullptr)
	, x(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pWireVertexBuffer(nullptr)
	, m_OutputMode(NONE)
	, m_CurrentAdvance(0.0)
	, m_bLengthyJob(true)
	, m_strCurrentJob(_T(""))
	, m_Nce(0)
	, m_Nde(0)
{
	// This constructor is only called once so there is no problem to initialize all these static variables!!

	ElementSubSet::m_pElements = &m_Elements;

	
	// The Memory Quota is set by default to 90% of the total available RAM
	MEMORYSTATUSEX mem;
	mem.dwLength = sizeof(mem);
	GlobalMemoryStatusEx(&mem);
	Model::m_RAM_Quota_Gb = (UINT)(((double)mem.ullTotalPhys * 0.9) / pow(1024.0 , 3.0));
}

Model::~Model()
{
	m_bLengthyJob = false;
	ClearAll();
}

void Model::ClearAll()
{
	// Clear The BEM Matrix Data
	ClearBEMMatrixData();

	// Clear the Geometrical Data
	ClearGeometricData();	

	m_Nce = m_Nde = 0;
}


void Model::ClearGeometricData()
{
	// Clean up the vertex container
	for (Vertex* pV : m_Vertices)
		if (pV)
			delete pV;
	m_Vertices.clear();

	// Clean up the Vertex DOF container
	m_DOF_Vertices.clear();

	// Clean up the element container
	for (Element* pElm : m_Elements)
		if (pElm)
			delete pElm;
	m_Elements.clear();


	// Clean Up the Subsets container
	for (ElementSubSet* pSubSet : m_SubSets)
		if (pSubSet)
			delete pSubSet;
	m_SubSets.clear();

	// Clean up the D3D data
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_pWireVertexBuffer)
	{
		m_pWireVertexBuffer->Release();
		m_pWireVertexBuffer = nullptr;
	}
}


void Model::ClearBEMMatrixData()
{
	int Nv = m_Vertices.size();
	int N = Nv * 3;

	Q.CleanRamStorage();
	R.CleanRamStorage();
	A.CleanRamStorage();


	// delete b vector
	if (b)
	{
		delete[] b;
		b = nullptr;
	}

	// delete x vector
	if (x)
	{
		delete[] x;
		x = nullptr;
	}
}



void Model::ConstructVertexDOFContainer()
{
	// Clear the Vertex DOF container
	m_DOF_Vertices.clear();

	for (unsigned i = 0; i < m_Elements.size(); i++)
	{
		Element* pElm = m_Elements[i];

		for (unsigned l = 0; l < 3; l++)
		{
			Vertex* pVdof = &pElm->Vdof(l);

			auto iter = std::find(m_DOF_Vertices.begin(), m_DOF_Vertices.end(), pVdof);

			if (iter == m_DOF_Vertices.end())
			{
				pElm->SetDofIdx(l, m_DOF_Vertices.size());
				m_DOF_Vertices.push_back(pVdof);
			}
			else
				pElm->SetDofIdx(l, std::distance(m_DOF_Vertices.begin(), iter));
		}
	}
}


void Model::ComputeInfluenceCoefficients(double** Q_Chunck, double** P_Chunck, Matrix* CPV, int j, int m, int i)
{
	// Element of index j
	// Element Node of local index m
	// Dof Vertex of index i 

	if (!m_bLengthyJob)
		return;

	Element* pElm_j = m_Elements[j];
	Vertex* pRowVertex = m_DOF_Vertices[i];
	Vertex* pColVertex = pElm_j->Vdof_Ptr(m);



	const Vertex& rSrcPt = *pRowVertex;

	//============================== Q Matrix =======================================	
	// column direction c = 0, 1, 2
	for (int c = 0; c < 3; c++)
	{
		bool bDispBc = pColVertex->IsDispFixed(c);
		const Vector& Tm = pElm_j->T(m);
		bool bNotNull = (Tm[c] != 0.0);
		bool bCountCoeff = (bDispBc || bNotNull);

		
		for (int r = 0; r < 3; r++)
			if (bCountCoeff)
				Q_Chunck[3 * m + c][3 * i + r] = -pElm_j->Q(rSrcPt, r, c, m);
			else
				Q_Chunck[3 * m + c][3 * i + r] = 0.0;
	}
				
	//===============================================================================

	

	if (Element::m_bRigidBodyCPV && (pRowVertex == pColVertex))
		return;
	
	//============================== P Matrix =======================================
	// column direction c = 0, 1, 2
	for (int c = 0; c < 3; c++)
		for (int r = 0; r < 3; r++)
			P_Chunck[3 * m + c][3 * i + r] = pElm_j->P(rSrcPt, r, c, m);
	//===============================================================================


	if (Element::m_bRigidBodyCPV)
	{
		// Each Row DOF Vertex has a 3*3 Matrix which is the Diagonal Blocks of CPV
	    // integrals. which are computed using the rigid body condition (Row summation)
	    //============================== Update the CPVs ======================================
	    //	
	    // ========================= Row N°1 ======================================
		CPV[i]._11 -= P_Chunck[3 * m + 0][3 * i + 0];
		CPV[i]._12 -= P_Chunck[3 * m + 1][3 * i + 0];
		CPV[i]._13 -= P_Chunck[3 * m + 2][3 * i + 0];
		// ========================= Row N°2 ======================================
		CPV[i]._21 -= P_Chunck[3 * m + 0][3 * i + 1];
		CPV[i]._22 -= P_Chunck[3 * m + 1][3 * i + 1];
		CPV[i]._23 -= P_Chunck[3 * m + 2][3 * i + 1];
		// ========================= Row N°3 ======================================
		CPV[i]._31 -= P_Chunck[3 * m + 0][3 * i + 2];
		CPV[i]._32 -= P_Chunck[3 * m + 1][3 * i + 2];
		CPV[i]._33 -= P_Chunck[3 * m + 2][3 * i + 2];
		//=====================================================================================
	}		
}


void Model::ConstructCoefficientMatrices()
{
	//Clear the BEM Matrix data
	ClearBEMMatrixData();

	m_bLengthyJob = true;
	m_CurrentAdvance = 0.0;
	::AfxBeginThread(ConstructCoefficientMatricesThread, (LPVOID)this);
}



UINT Model::ConstructCoefficientMatricesThread(LPVOID pParam)
{
	
	// Get the THIS pointer of the model object
	Model* pObj = (Model*)pParam;

	if (!pObj->m_bLengthyJob)
		return 0;
	
	pObj->m_strCurrentJob = _T("Copying boundary conditions from subsets...");
	pObj->CopyBoundaryConditionsFromSubSets();


	pObj->m_strCurrentJob = _T("Obtain sytem dimensions...");



	// Get the dimension of the system
	int Ne = pObj->m_Elements.size();     // Number of elements
	int Nv = pObj->m_DOF_Vertices.size(); // Number of vertices

	int M = Ne * 9;                    // The column dimension of the matrices
	int N = Nv * 3;                    // The row dimension of the matrices

	
	pObj->m_strCurrentJob = _T("Initialization of matrices Q and R...");
	// Initialize matrices R and Q
	pObj->Q.Initialize(N, M);
	pObj->R.Initialize(N, N);


	// Declare the variables for the progression tracking
	double Ntot = (double)Nv * ((double)Ne * 3.0 + 1.0);
	double Done = 0.0;


	// Open the Q and R Files here!!
	pObj->Q.OpenFile(true);
	pObj->R.OpenFile(true); // No need to zero the file



	pObj->m_strCurrentJob = _T("Allocate necessary resources...");
	// Declare the CPV blocks 
	Matrix* CPV = new Matrix[Nv];
	

	// Create a 9 x N arrays to avoid accessing OOC storage element by element and do entire column copy instead
	
	// Allocate the bi-dimensional Array chunks
	double* Q_Chunck[9];
	double* P_Chunck[9];

	// Affect contiguous memory to them
	for (unsigned t = 0; t < 9; t++)
	{
		Q_Chunck[t] = new double[N];
		P_Chunck[t] = new double[N];
	}

    
	pObj->m_strCurrentJob = _T("Calculate the coefficient matrices Q and R...");

	for (int j = 0; j < Ne; j++)
	{
		// Enter the loop to calculate the influence coefficients 
        #pragma omp parallel for
		for (int i = 0; i < Nv; i++)
		{
			if (!pObj->m_bLengthyJob)
				break;

			for (int m = 0; m < 3; m++)
				pObj->ComputeInfluenceCoefficients(Q_Chunck, P_Chunck, CPV, j, m, i);  // <===I still receive access violation here!!!			
		}

		if (!pObj->m_bLengthyJob)			
			break;  // or continue; (cannot return from parallel block)
		
		
		// Copy the Q Chunck array to the LD_Matrix Q
		pObj->Q.SetChunck(9 * j, 9, Q_Chunck); //<==== PARALLEL 

		// Copy the P Chunck columns to the LD_Matrix R
		for (int l = 0; l < 3; l++)
		{
			int DofIdx = pObj->m_Elements[j]->DofIdx(l);


			for (int k = 0; k < 3; k++)
				pObj->R.IncrementColumn(3 * DofIdx + k, P_Chunck[3 * l + k]);		//<==== PARALLEL 	
		}
			

		Done += ((double)Nv * 3.0);
		pObj->m_CurrentAdvance = Done / Ntot;
	}


	// Free Chunks memory
	for (int i = 0; i < 9; i++)
	{
		delete[] Q_Chunck[i];
		delete[] P_Chunck[i];
	}
	


	if (!pObj->m_bLengthyJob)
	{
		pObj->Q.CloseFile();
		pObj->R.CloseFile();


		if (CPV != nullptr)
		{
			delete[] CPV;
			CPV = nullptr;
		}

		return 0;
	}
	

	pObj->m_strCurrentJob = _T("Add the diagonal C.P.V blocs of matrix R...");

	
	// Set the C.P.V  integrals using rigid body Method or Add the free terms in case of Direct method
    
	#pragma omp parallel for
	for (int i = 0; i < Nv; i++)
	{
		if (Element::m_bRigidBodyCPV)
		{
			pObj->R.Set(3 * i + 0, 3 * i + 0, CPV[i]._11);
			pObj->R.Set(3 * i + 1, 3 * i + 0, CPV[i]._12);
			pObj->R.Set(3 * i + 2, 3 * i + 0, CPV[i]._13);

			pObj->R.Set(3 * i + 0, 3 * i + 1, CPV[i]._21);
			pObj->R.Set(3 * i + 1, 3 * i + 1, CPV[i]._22);
			pObj->R.Set(3 * i + 2, 3 * i + 1, CPV[i]._23);

			pObj->R.Set(3 * i + 0, 3 * i + 2, CPV[i]._31);
			pObj->R.Set(3 * i + 1, 3 * i + 2, CPV[i]._32);
			pObj->R.Set(3 * i + 2, 3 * i + 2, CPV[i]._33);
		}
		else
		{
			Vertex* pVdof = pObj->m_DOF_Vertices[i];
			const Matrix& C = pVdof->C;
			
			pObj->R.Add(3 * i + 0, 3 * i + 0, C._11);
			pObj->R.Add(3 * i + 1, 3 * i + 0, C._12);
			pObj->R.Add(3 * i + 2, 3 * i + 0, C._13);

			pObj->R.Add(3 * i + 0, 3 * i + 1, C._21);
			pObj->R.Add(3 * i + 1, 3 * i + 1, C._22);
			pObj->R.Add(3 * i + 2, 3 * i + 1, C._23);

			pObj->R.Add(3 * i + 0, 3 * i + 2, C._31);
			pObj->R.Add(3 * i + 1, 3 * i + 2, C._32);
			pObj->R.Add(3 * i + 2, 3 * i + 2, C._33);
		}	  	
	}	

	

	Done += (double)Nv;
	pObj->m_CurrentAdvance = Done / Ntot;


	if (CPV != nullptr)
	{
		delete[] CPV;
		CPV = nullptr;
	}



	// Close the files
	pObj->Q.CloseFile();
	pObj->R.CloseFile();


    // Set the progress advance to 100% and make the Lengthy Job flag to false
	pObj->m_CurrentAdvance = 1.0;
	pObj->m_bLengthyJob = false;
	

	pObj->m_strCurrentJob = _T("The computation of coefficient matrices completed successfully!");

	return 0;
}

void Model::CopyBoundaryConditionsFromSubSets()
{
	// Make the Boundary unknowns to their Default state
	for (Element* pElm : m_Elements)
	{
		for (int l = 0; l < 3; l++)
		{
			Vertex& V = pElm->Vdof(l);
			Vector& T = pElm->T(l);

			V.U.Set(0.0, 0.0, 0.0);
			T.Set(0.0, 0.0, 0.0);  
			V.m_bFixed_X = false;
			V.m_bFixed_Y = false;
			V.m_bFixed_Z = false;
		}		
	}

	// Loop through the Subsets and Copy the Boundary Conditions One By One
	for (ElementSubSet* pSubSet : m_SubSets)
	{
		// Check if the Subset Contains a normal displacement
		if (pSubSet->m_bNormalDisplacement && pSubSet->m_Un != 0.0)
		{
			// Apply a normal Displacement on all Elements
			for (int Idx : pSubSet->m_Indices)
			{
				Element* pElm = m_Elements[Idx];

				Vector U = pElm->m_n * pSubSet->m_Un;

				for (int l = 0; l < 3; l++)
				{
					Vertex& V = pElm->Vdof(l);
					Vector& T = pElm->T(l);

					V.U = U;
					T.Set(0.0, 0.0, 0.0);
					V.m_bFixed_X = pSubSet->m_bFixed_X;
					V.m_bFixed_Y = pSubSet->m_bFixed_Y;
					V.m_bFixed_Z = pSubSet->m_bFixed_Z;
				}				

			}

			continue;
		}

		// Check if the Subset Contains a normal traction
		else if (pSubSet->m_bNormalTraction && pSubSet->m_Tn != 0.0)
		{
			// Apply a normal Displacement on all Elements
			for (int Idx : pSubSet->m_Indices)
			{
				Element* pElm = m_Elements[Idx];

				Vector Tn = pElm->m_n * pSubSet->m_Tn;

				for (int l = 0; l < 3; l++)
				{
					Vertex& V = pElm->Vdof(l);
					Vector& T = pElm->T(l);
					T = Tn;
					V.U.Set(0.0, 0.0, 0.0);
					V.m_bFixed_X = pSubSet->m_bFixed_X;
					V.m_bFixed_Y = pSubSet->m_bFixed_Y;
					V.m_bFixed_Z = pSubSet->m_bFixed_Z;
				}

			}

			continue;
		}

		// Apply the Cartezian Boundary condition
		else
		{
			for (int Idx : pSubSet->m_Indices)
			{
				Element* pElm = m_Elements[Idx];

				for (int l = 0; l < 3; l++)
				{
					Vertex& V = pElm->Vdof(l);
					Vector& T = pElm->T(l);

					V.U = pSubSet->U;
					T = pSubSet->T;
					V.m_bFixed_X = pSubSet->m_bFixed_X;
					V.m_bFixed_Y = pSubSet->m_bFixed_Y;
					V.m_bFixed_Z = pSubSet->m_bFixed_Z;
				}

			}
		}
		
	}
}



void Model::ConstructLinearSystem()
{
	// Get the dimension of the system
	int Ne = m_Elements.size();     // Number of elements
	int Nv = m_DOF_Vertices.size(); // Number of vertices
	int M = Ne * 9;                 // The column dimension of the matrices
	int N = Nv * 3;                 // The row dimension of the matrices

	// Allocate memory 
	// Delete Ram Storage in A MatriX 
	A.CleanRamStorage();

	// delete b vector
	if (b)
	{
		delete[] b;
		b = nullptr;
	}

	// delete x vector
	if (x)
	{
		delete[] x;
		x = nullptr;
	}

	// Allocate memory for b  and x Vectors and Initialize A Matrix
	A.Initialize(N, N);

	b = new double[N];
	x = new double[N];

	//=====================================================================
	// Fill the vectors b and x by default by zeros
	ZeroMemory(b, sizeof(double) * N);
	ZeroMemory(x, sizeof(double) * N);

	// Open Q and R Matrices files for Read
	Q.OpenFile(true);
	R.OpenFile(true);
	// Open A matrix file for Write
	A.OpenFile(true);

	// Construct a map which has the indices owning elements for each Dof vertex!!
	std::vector<std::vector<int>> VertElmMap(Nv);

	for (int j = 0; j < Ne; j++)
	{
		Element* pElm = m_Elements[j];

		VertElmMap[pElm->m_J1].push_back(j);
		VertElmMap[pElm->m_J2].push_back(j);
		VertElmMap[pElm->m_J3].push_back(j);
	}

	// Declare the variables for the progression tracking
	double Ntot = (double)Nv;
	double Done = 0.0;
	m_CurrentAdvance = 0.0;

	// Fill the A Matrix based  on the Fixed Disp flag of the nodes	
	for (int i = 0; i < Nv; i++, Done += 1.0)
	{
		Vertex* pVi = m_DOF_Vertices[i];

		for (int e_idx : VertElmMap[i])
		{
			Element* pElm = m_Elements[e_idx];
			int e_node_idx = pElm->GetVertexLocalIndex(pVi);
			const Vector& T = pElm->T(e_node_idx);


			for (int k = 0; k < 3; k++)
			{
				int idx_Col_Q = 9 * e_idx + 3 * e_node_idx + k;
				int idx_Col_R = 3 * i + k;
				int idx_Col_A = idx_Col_R;


				LD_Column<double> Q_Col = Q[idx_Col_Q];
				LD_Column<double> R_Col = R[idx_Col_R];

				if (pVi->IsDispFixed(k))
				{
					A.SetColumn(idx_Col_A, Q_Col);
					//CopyMemory(A[col], Q[col], N * sizeof(double));

                    #pragma omp parallel for
					for (int row = 0; row < N; row++)
						b[row] -= (R_Col[row] * pVi->U[k]);
				}
				else
				{
					A.SetColumn(idx_Col_A, R_Col);
					//CopyMemory(A[col], P[col], N * sizeof(double));

                    #pragma omp parallel for
					for (int row = 0; row < N; row++)
						b[row] -= (Q_Col[row] * T[k]);
				}
			}
		}	

		m_CurrentAdvance = Done / Ntot;
	}


	// close the matrix files
	Q.CloseFile();
	R.CloseFile();
	A.CloseFile();
}



void Model::UpdateBoundaryUnknowns()
{
	// Get the dimension of the system
	int Ne = m_Elements.size();     // Number of elements
	int Nv = m_DOF_Vertices.size(); // Number of vertices
	int M = Ne * 9;                 // The column dimension of the matrices
	int N = Nv * 3;                 // The row dimension of the matrices


	for (int i = 0; i < Nv; i++)
	{
		Vertex* pVi = m_DOF_Vertices[i];

		// Look for the first element containing this vertex
		int e_idx = -1;
		int e_node_idx = -1;
		Element* pElm = nullptr;
		for (int j = 0; j < Ne; j++)
		{
			pElm = m_Elements[j];

			if (pElm->IsVertexPartOf(pVi))
			{
				e_idx = j;
				e_node_idx = pElm->GetVertexLocalIndex(pVi);
				break;
			}
		}

		for (int k = 0; k < 3; k++)
		{
			Vector& T = pElm->T(e_node_idx);
			if (pVi->IsDispFixed(k))
				T[k] = x[3 * i + k];
			else
				pVi->U[k] = x[3 * i + k];
		}
	}


	// Calculate the boundary stress and strain tensors
	CalculateTensors();

	// Calculate the response in the geometrical Vertices from internal control nodes
	UpdateVertices();

	// Calculate the scale By default to make the Maximum Displacement = 10
	double U = 0.0;
	for (Vertex* pV : m_Vertices)
	{
		if (U < pV->U.Magnitude())
			U = pV->U.Magnitude();
	}

	Vertex::m_DeformationScale = 10.0 / U;
}

void Model::UpdateVertices()
{
	for (Element* pElm : m_Elements)
		pElm->UpdateGeometricalVertices();
}


double Model::GetMinValue()const
{
	double val = DBL_MAX;

	for (const Element* pElm : m_Elements)
	{
		double Val1 = pElm->GetValue(0, m_OutputMode);
		double Val2 = pElm->GetValue(1, m_OutputMode);
		double Val3 = pElm->GetValue(2, m_OutputMode);

		if (val > Val1)
			val = Val1;

		if (val > Val2)
			val = Val2;

		if (val > Val3)
			val = Val3;
	}

	return val;
}


double Model::GetMaxValue()const
{
	double val = -DBL_MAX;


	for (const Element* pElm : m_Elements)
	{
		double Val1 = pElm->GetValue(0, m_OutputMode);
		double Val2 = pElm->GetValue(1, m_OutputMode);
		double Val3 = pElm->GetValue(2, m_OutputMode);

		if (val < Val1)
			val = Val1;

		if (val < Val2)
			val = Val2;

		if (val < Val3)
			val = Val3;
	}

	return val;
}


DWORD Model::GetColorFromValue(const Element* pElm0, int nVertex, double MinVal, double MaxVal)const
{
	double Val = 0.0;
	// Get the normal for the concerned element 
	const Vector& n = pElm0->m_n;
	// Get the vertex
	Vertex* pV = nullptr;
	
	// Get the corresponding value and vertex for this element
	switch (nVertex)
	{
	case 0:
		pV = pElm0->m_pV1;
		Val = pElm0->GetValue(0, m_OutputMode);
		break;

	case 1:
		pV = pElm0->m_pV2;
		Val = pElm0->GetValue(1, m_OutputMode);
		break;

	case 2:
		pV = pElm0->m_pV3;
		Val = pElm0->GetValue(2, m_OutputMode);
		break;

	default:
		break;
	}

	// Check the vertex
	if (pV == nullptr)
		return 0;

	double Ratio = fabs((Val - MinVal) / (MaxVal - MinVal));
	DWORD Color = 0;
	// Create the Central Color
	if (Ratio < (1.0 / 3.0))
	{
		int i = (int)(255.0 * Ratio * 3.0);
		Color = D3DCOLOR_XRGB(0, i, 255 - i);
	}
	else if (Ratio >= (1.0 / 3.0) && Ratio < (2.0 / 3.0))
	{
		double _Ratio = Ratio - (1.0 / 3.0);
		int i = (int)(255.0 * _Ratio * 3.0);
		Color = D3DCOLOR_XRGB(i, 255, 0);
	}
	else if (Ratio >= (2.0 / 3.0))
	{
		double _Ratio = Ratio - (2.0 / 3.0);
		int i = (int)(255.0 * _Ratio * 3);
		Color = D3DCOLOR_XRGB(255, 255 - i, 0);
	}

	return Color;
}


void Model::UpdateVertexBuffer(IDirect3DDevice9* pD3ddev, bool bPostTreatment)
{
	// Clean up the D3D data
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_pWireVertexBuffer)
	{
		m_pWireVertexBuffer->Release();
		m_pWireVertexBuffer = nullptr;
	}

	// Create the dimension variables for future use
	size_t Nv = m_Elements.size() * 3; // The number of vertices
	size_t Ne = m_Elements.size(); // the number of triangles
	if (Ne == 0)
		return;
	size_t nBytes = 0;

	if (!bPostTreatment || m_OutputMode == NONE)
	{
		nBytes = m_Elements.size() * 3 * sizeof(D3DVertex); // the total memory size of the vertex buffer

		// Create a temporary buffer containing the D3DVertices and fill it
		D3DVertex* vertices = new D3DVertex[Nv];

		for (size_t i = 0; i < m_Elements.size(); i++)
		{
			Element* pElm = m_Elements[i];
			D3DXVECTOR3 n((float)pElm->m_n.x, (float)pElm->m_n.y, (float)pElm->m_n.z);
			pElm->m_pV1->SetD3dVertexPosition(vertices[3 * i + 0], bPostTreatment);
			pElm->m_pV2->SetD3dVertexPosition(vertices[3 * i + 1], bPostTreatment);
			pElm->m_pV3->SetD3dVertexPosition(vertices[3 * i + 2], bPostTreatment);
			vertices[3 * i + 0].n = n;
			vertices[3 * i + 1].n = n;
			vertices[3 * i + 2].n = n;
		}

		// Create the D3D Vertex Buffer
		pD3ddev->CreateVertexBuffer(nBytes, 0, g_FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

		// Lock the vertex Buffer and fill it then unlock it
		LPVOID pLockMem = nullptr;
		m_pVertexBuffer->Lock(0, nBytes, &pLockMem, D3DLOCK_DISCARD);
		CopyMemory(pLockMem, vertices, nBytes);
		m_pVertexBuffer->Unlock();

		// Delete the vertices container
		delete[] vertices;

	}
	else
	{
		nBytes = m_Elements.size() * 3 * sizeof(D3D_RGB_Vertex); // the total memory size of the vertex buffer

		double MinVal = GetMinValue();
		double MaxVal = GetMaxValue();

		// Create a temporary buffer containing the D3D_RGB_Vertices and fill it
		D3D_RGB_Vertex* vertices = new D3D_RGB_Vertex[Nv];


		for (size_t i = 0; i < m_Elements.size(); i++)
		{
			Element* pElm = m_Elements[i];
			pElm->m_pV1->SetD3dRGBVertexPosition(vertices[3 * i + 0], bPostTreatment);
			pElm->m_pV2->SetD3dRGBVertexPosition(vertices[3 * i + 1], bPostTreatment);
			pElm->m_pV3->SetD3dRGBVertexPosition(vertices[3 * i + 2], bPostTreatment);
			vertices[3 * i + 0].Color = GetColorFromValue(pElm, 0, MinVal, MaxVal);
			vertices[3 * i + 1].Color = GetColorFromValue(pElm, 1, MinVal, MaxVal);
			vertices[3 * i + 2].Color = GetColorFromValue(pElm, 2, MinVal, MaxVal);
			/*vertices[3 * i + 0].Color = pElm->GetColorFromValue(1.0, 0.0, m_OutputMode, MinVal, MaxVal);
			vertices[3 * i + 1].Color = pElm->GetColorFromValue(0.0, 1.0, m_OutputMode, MinVal, MaxVal);
			vertices[3 * i + 2].Color = pElm->GetColorFromValue(0.0, 0.0, m_OutputMode, MinVal, MaxVal);*/
		}

		// Create the D3D Vertex Buffer
		pD3ddev->CreateVertexBuffer(nBytes, 0, g_RGB_FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

		// Lock the vertex Buffer and fill it then unlock it
		LPVOID pLockMem = nullptr;
		m_pVertexBuffer->Lock(0, nBytes, &pLockMem, D3DLOCK_DISCARD);
		CopyMemory(pLockMem, vertices, nBytes);
		m_pVertexBuffer->Unlock();

		// Delete the vertices container
		delete[] vertices;
	}
	

	// Create the Wire Vertex Buffer
	nBytes = m_Elements.size() * 3 * sizeof(D3D_RGB_Vertex); // the total memory size of the vertex buffer

	
	// Create a temporary buffer containing the D3D_RGB_Vertices and fill it
	D3D_RGB_Vertex* vertices = new D3D_RGB_Vertex[Nv];


	for (size_t i = 0; i < m_Elements.size(); i++)
	{
		Element* pElm = m_Elements[i];
		pElm->m_pV1->SetD3dRGBVertexPosition(vertices[3 * i + 0], bPostTreatment);
		pElm->m_pV2->SetD3dRGBVertexPosition(vertices[3 * i + 1], bPostTreatment);
		pElm->m_pV3->SetD3dRGBVertexPosition(vertices[3 * i + 2], bPostTreatment);

		//// Temp Code
		//pElm->Vdof(0).SetD3dRGBVertexPosition(vertices[3 * i + 0], bPostTreatment);
		//pElm->Vdof(1).SetD3dRGBVertexPosition(vertices[3 * i + 1], bPostTreatment);
		//pElm->Vdof(2).SetD3dRGBVertexPosition(vertices[3 * i + 2], bPostTreatment);

		vertices[3 * i + 0].Color = D3DCOLOR_XRGB(150, 0, 0);
		vertices[3 * i + 1].Color = D3DCOLOR_XRGB(150, 0, 0);
		vertices[3 * i + 2].Color = D3DCOLOR_XRGB(150, 0, 0);
	}

	// Create the D3D Vertex Buffer
	pD3ddev->CreateVertexBuffer(nBytes, 0, g_RGB_FVF, D3DPOOL_DEFAULT, &m_pWireVertexBuffer, NULL);

	// Lock the vertex Buffer and fill it then unlock it
	LPVOID pLockMem = nullptr;
	m_pWireVertexBuffer->Lock(0, nBytes, &pLockMem, D3DLOCK_DISCARD);
	CopyMemory(pLockMem, vertices, nBytes);
	m_pWireVertexBuffer->Unlock();

	// Delete the vertices container
	delete[] vertices;

	
}


void Model::Draw(IDirect3DDevice9* pD3ddev, FILL_MODE fillMode, bool bPostTreatment, SHOW_POINTS_MODE ShowPointsMode) const
{
	if (!m_pVertexBuffer)
		return;


	if (!bPostTreatment || m_OutputMode == NONE)
	{
		pD3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
		pD3ddev->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(D3DVertex));
		pD3ddev->SetFVF(g_FVF);

		if (fillMode == EDGED_FACES)
		{
			// Store the previous Fill Mode
			DWORD dwPrevFillMode;
			pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);

			// Force the Fill Mode to be Solid
			pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Elements.size());

			// Force the Fill Mode to be wireframe
			pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			
			// Change Lighting, the stream source and FVF
			pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
			pD3ddev->SetStreamSource(0, m_pWireVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
			pD3ddev->SetFVF(g_RGB_FVF);
			

			pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Elements.size());

			// Restore the Previous Fill Mode
			pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
			// Turn Lighting On
			pD3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
		else
			pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Elements.size());
	}
	else
	{
		pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		pD3ddev->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
		pD3ddev->SetFVF(g_RGB_FVF);

		if (fillMode == EDGED_FACES)
		{
			// Store the previous Fill Mode
			DWORD dwPrevFillMode;
			pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);

			// Force the Fill Mode to be Solid
			pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Elements.size());
			
			// Force the Fill Mode to be wireframe
			pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			// Change Lighting, the stream source and FVF
			pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
			pD3ddev->SetStreamSource(0, m_pWireVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
			pD3ddev->SetFVF(g_RGB_FVF);

			pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Elements.size());

			// Restore the Previous Fill Mode
			pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);

		}
		else
			pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Elements.size());
	}   


	// Draw the nodes
	
	if (ShowPointsMode != HIDE_POINTS)
	{

		// Set the Fill Mode To Solid
		DWORD dwPrevFillMode;
		pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwPrevFillMode);
		pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		// Set Lighting to true
		DWORD dwPrevLighting;
		pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwPrevLighting);
		pD3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

		switch (ShowPointsMode)
		{
		case SHOW_VERTICES:
		{
			// Create a Material
			// Create the materials
			D3DMATERIAL9 NodeMaterial;
			ZeroMemory(&NodeMaterial, sizeof(NodeMaterial));


			NodeMaterial.Ambient = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
			NodeMaterial.Diffuse = D3DXCOLOR(0.0f, 8.0f, 0.0f, 1.0f);


			pD3ddev->SetMaterial(&NodeMaterial);

			for (Vertex* pV : m_Vertices)
				pV->Draw(bPostTreatment);
			
		}
		break;


		case SHOW_NODES:
		{
			// Create a Material
			// Create the materials
			D3DMATERIAL9 NodeMaterial;
			ZeroMemory(&NodeMaterial, sizeof(NodeMaterial));


			NodeMaterial.Ambient = D3DXCOLOR(0.0f, 0.1f, 0.5f, 1.0f);
			NodeMaterial.Diffuse = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);


			pD3ddev->SetMaterial(&NodeMaterial);

			for (Vertex* pV : m_DOF_Vertices)
				pV->Draw(bPostTreatment);

		}
		break;
		}

		// Restore Fill Mode
		pD3ddev->SetRenderState(D3DRS_FILLMODE, dwPrevFillMode);
		// Restore Lighting
		pD3ddev->SetRenderState(D3DRS_LIGHTING, dwPrevLighting);
		
	}

	
}


void Model::AutoSetMatrixMemoryQuotas()
{
	unsigned Ne = m_Elements.size();
	unsigned Nv = m_Vertices.size();
	unsigned Nv_dof = m_DOF_Vertices.size();
	unsigned N = Nv_dof * 3;
	unsigned M = Ne * 9;

	// Compute the total required memory
	// Compute Memory requirements
	// Model Memory
	double Elems_Mem = (double)(Ne * sizeof(BEM_3D::Element));
	double Vertex_Mem = (double)(Nv * sizeof(BEM_3D::Vertex));
	double V_Dof_Mem = (double)(Nv_dof * sizeof(BEM_3D::Vertex));
	double Model_Mem = Elems_Mem + Vertex_Mem + V_Dof_Mem;

	// Mat-Vec Memory
	double Q_Mem = (double)(N * M * sizeof(double));
	double RA_Mem = (double)(N * N * sizeof(double));
	double bx_Mem = (double)(N * sizeof(double));

	// Total Memory
	double Total_Mem = Model_Mem + Q_Mem + 2.0 * (RA_Mem + bx_Mem);

	// Remainig Matrix Memory
	double Ram_Quota = (double)m_RAM_Quota_Gb * pow(1024.0, 3.0);
	double Rem_Mat_Mem = max(0.0, Ram_Quota - Model_Mem - 2.0 * bx_Mem);
	double Req_Mat_Mem = Q_Mem + 2.0 * RA_Mem;

	// Calculate the Quotas for matrices Q, R and A
	double Q_fact = Q_Mem / Req_Mat_Mem;
	double RA_fact = RA_Mem / Req_Mat_Mem;
	double fQ_Quota = Rem_Mat_Mem * Q_fact;
	double fRA_Quota = Rem_Mat_Mem * RA_fact;
	unsigned Q_quota = (unsigned) (fQ_Quota / pow(1024.0, 2.0));
	unsigned RA_quota = (unsigned)(fRA_Quota / pow(1024.0, 2.0));

	Q.SetRamQuota(Q_quota);
	R.SetRamQuota(RA_quota);
	A.SetRamQuota(RA_quota);
}



void Model::SetMatrixMemoryQuotas(int Q_Quota, int R_Quota, int A_Quota)
{
	Q.SetRamQuota(Q_Quota * 1024ul);
	R.SetRamQuota(R_Quota * 1024ul);
	A.SetRamQuota(R_Quota * 1024ul);
}





void Model::LoadMeshFromOBJFile(LPTSTR szFileName)
{
	// Open the file using C++ standard library
	std::fstream file(szFileName, std::ios::in);
	
	// Clear All the previous Data (Geometric + BEM Matrix)
	ClearAll();

	// Construct the vertex container
	while (file.rdstate() == std::fstream::goodbit)
	{
		char line[MAX_PATH];
		file.getline(line, MAX_PATH);

		std::stringstream str;
		str << line;

		char c1, c2;
		c1 = str.get();
		c2 = str.get();

		if (c1 == 'v' && c2 == ' ')
		{
			double x, y, z;

			str >> x;
			str >> y;
			str >> z;

			m_Vertices.push_back(new Vertex(x, y, z, true));
		}		
		else if (c1 == 'f' && c2 == ' ')
		{
			int I1, I2, I3;

			str >> I1;
			str >> I2;
			str >> I3;

			I1--;
			I2--;
			I3--;
			

			switch (m_MeshType)
			{
			case PURE_CONTINUOUS:
			case DISCONTINUOUS_SHARP_EDGES: // The discontinuous elements will replace the continuous ones in sharp edges
				                            // The replacement is done once the model loding is completed
				m_Elements.push_back(new ContElm(m_Vertices[I1], m_Vertices[I2], m_Vertices[I3], I1, I2, I3));
				break;

			case PURE_DISCONTINUOUS:
				m_Elements.push_back(new DiscElm(m_Vertices[I1], m_Vertices[I2], m_Vertices[I3], I1, I2, I3));
				break;
			}

		}

	}

	m_strFileType = _T("ASCII (.OBJ)");

	//Close the file
	file.close();	
}




void Model::LoadMeshFromSTLFile(LPTSTR szFileName)
{

	// Clear All the previous Data (Geometric + BEM Matrix)
	ClearAll();

	// Check whether the file is ASCII
	bool bASCII = false;
	// Open the file using C++ standard library in binary mode
	std::fstream file(szFileName, std::ios::in|std::ios::binary);

	// Get the file size in bytes
	// Get file size
	file.seekg(0, std::ios::end);
	std::streamsize NBytes = file.tellg();
	file.seekg(0, std::ios::beg);

	// The file Data
	BYTE* pData = new BYTE[NBytes];
	file.read((char*)pData, NBytes);
	BYTE* pOffset = pData;

	// Ignore Header
	//UINT8 Header[80];
	//CopyMemory(Header, pOffset, sizeof(Header));
	pOffset += (sizeof(UINT8) * 80);

	UINT32 Ne = 0;
	CopyMemory(&Ne, pOffset, sizeof(Ne));
	pOffset += sizeof(Ne);

	std::streamsize PresumedSize = 80 * sizeof(UINT8) + sizeof(UINT32) + Ne * (12 * sizeof(float) + sizeof(UINT16));

	// Close the file
	file.close();
	

	if (PresumedSize != NBytes)
	{
		bASCII = true;
		if (pData)
			delete[] pData;
	}		
	else
	{
		bASCII = false;
		pOffset = pData;
	}
		


	if (bASCII)
	{
		// Open the file using C++ standard library in normal mode
		std::fstream file(szFileName, std::ios::in);

		int nVertIdx = -1;
		int I1 = -1;
		int I2 = -1;
		int I3 = -1;
		Vertex* pV1 = nullptr;
		Vertex* pV2 = nullptr;
		Vertex* pV3 = nullptr;
		Element* pElm = nullptr;



		// Construct the vertex container
		while (file.rdstate() == std::fstream::goodbit)
		{
			char szLine[MAX_PATH];  // Zero terminated string
			file.getline(szLine, MAX_PATH);

			std::string strLine(szLine); // Variable of type string



			if (strLine.find("solid") != std::string::npos) // Begin of a solid body
			{
				// Do nothing
				continue;
			}
			else if (strLine.find("facet normal") != std::string::npos) // a new facete (Element)
			{
				// We can ignore the normal here because we can reconstitute it 
				// But we can create a new element with no geometry if we want
				// Or create it after recovring the vertices
				continue;


				//// Transform the line zero terminated string into a strig stream
				//char* szData = szLine + strlen("facet normal");

				//std::stringstream str;
				//str << szData;			

				//str >> nx;
				//str >> ny;
				//str >> nz;
			}
			else if (strLine.find("outer loop") != std::string::npos) // begin the loop for the facete vertices
			{
				
				nVertIdx = 0;
			}
			else if (strLine.find("vertex") != std::string::npos) // read vertices;
			{
				char* szData = szLine + strLine.find("vertex") + strlen("vertex");

				std::stringstream str;
				str << szData;

				double x, y, z;
				str >> x;
				str >> y;
				str >> z;

				std::vector<Vertex*>::iterator iter;


				switch (nVertIdx % 3)
				{
				case 0:
					iter = std::find_if(m_Vertices.begin(), m_Vertices.end(), [=](Vertex* pV)
						{
							return (pV->x == x && pV->y == y && pV->z == z);
						});
					if (iter != m_Vertices.end())
					{
						pV1 = *iter;
						I1 = std::distance(m_Vertices.begin(), iter);
					}
					else
					{
						I1 = m_Vertices.size();
						pV1 = new Vertex(x, y, z, true);
						m_Vertices.push_back(pV1);
					}
					break;

				case 1:
					iter = std::find_if(m_Vertices.begin(), m_Vertices.end(), [=](Vertex* pV)
						{

							return (pV->x == x && pV->y == y && pV->z == z);
						});
					if (iter != m_Vertices.end())
					{
						pV2 = *iter;
						I2 = std::distance(m_Vertices.begin(), iter);
					}
					else
					{
						I2 = m_Vertices.size();
						pV2 = new Vertex(x, y, z, true);
						m_Vertices.push_back(pV2);
					}
					break;

				case 2:
					iter = std::find_if(m_Vertices.begin(), m_Vertices.end(), [=](Vertex* pV)
						{

							return (pV->x == x && pV->y == y && pV->z == z);
						});
					if (iter != m_Vertices.end())
					{
						pV3 = *iter;
						I3 = std::distance(m_Vertices.begin(), iter);
					}
					else
					{
						I3 = m_Vertices.size();
						pV3 = new Vertex(x, y, z, true);
						m_Vertices.push_back(pV3);
					}
					break;
				}
				nVertIdx++;
			}
			else if (strLine.find("endloop") != std::string::npos) // end vertex lecture;
			{
				nVertIdx = -1;
			}
			else if (strLine.find("endfacet") != std::string::npos) // end facete lecture and create a new element;
			{
				// Create the new element
				switch (m_MeshType)
				{
				case PURE_CONTINUOUS:
				case DISCONTINUOUS_SHARP_EDGES: // The discontinuous elements will replace the continuous ones in sharp edges
					// The replacement is done once the model loding is completed
					m_Elements.push_back(new ContElm(pV1, pV2, pV3, I1, I2, I3));
					break;

				case PURE_DISCONTINUOUS:
					m_Elements.push_back(new DiscElm(pV1, pV2, pV3, I1, I2, I3));
					break;
				}
			}
		}
		m_strFileType = _T("ASCII (.STL)");
		// Close the file
		file.close();
	}
	else
	{
		// Begin Model construction from the Loaded data
		// Ignore the model name
		pOffset += (sizeof(UINT8) * 80);
		// Extract the number of elements
		UINT32 Ne = 0;
		CopyMemory(&Ne, pOffset, sizeof(UINT32));
		pOffset += sizeof(Ne);
		// Create the elements using a loop
		//float n[3];
		
		for (UINT32 i = 0; i < Ne; i++)
		{
			float V1[3];
			float V2[3];
			float V3[3];
			// Ignore normal information
			//CopyMemory(n, pOffset, sizeof(n));
			pOffset += (sizeof(float) * 3);

			// First vertex
			CopyMemory(V1, pOffset, sizeof(float) * 3);
			pOffset += (sizeof(float) * 3);

			// Second vertex
			CopyMemory(V2, pOffset, sizeof(float) * 3);
			pOffset += (sizeof(float) * 3);

			// Third vertex
			CopyMemory(V3, pOffset, sizeof(float) * 3);
			pOffset += (sizeof(float) * 3);

			// Ignore the attribute
			pOffset += sizeof(UINT16);

			// find the three vertices
			Vertex* pV1 = nullptr;
			Vertex* pV2 = nullptr;
			Vertex* pV3 = nullptr;
			int I1 = -1;
			int I2 = -1;
			int I3 = -1;
			// Vertex 1
			double x = (double)V1[0];
			double y = (double)V1[1];
			double z = (double)V1[2];
			auto iter = std::find_if(m_Vertices.begin(), m_Vertices.end(), [=](Vertex* pV)
				{
					return (pV->x == x && pV->y == y && pV->z == z);
				});
			if (iter != m_Vertices.end())
			{
				pV1 = *iter;
				I1 = std::distance(m_Vertices.begin(), iter);
			}
			else
			{
				I1 = m_Vertices.size();
				pV1 = new Vertex(x, y, z, true);
				m_Vertices.push_back(pV1);
			}
			// Vertex 2
			x = (double)V2[0];
			y = (double)V2[1];
			z = (double)V2[2];
			iter = std::find_if(m_Vertices.begin(), m_Vertices.end(), [=](Vertex* pV)
				{
					
					return (pV->x == x && pV->y == y && pV->z == z);
				});
			if (iter != m_Vertices.end())
			{
				pV2 = *iter;
				I2 = std::distance(m_Vertices.begin(), iter);
			}
			else
			{
				I2 = m_Vertices.size();
				pV2 = new Vertex(x, y, z, true);
				m_Vertices.push_back(pV2);
			}
			// Vertex 3
			x = (double)V3[0];
			y = (double)V3[1];
			z = (double)V3[2];
			iter = std::find_if(m_Vertices.begin(), m_Vertices.end(), [=](Vertex* pV)
				{
					
					return (pV->x == x && pV->y == y && pV->z == z);
				});
			if (iter != m_Vertices.end())
			{
				pV3 = *iter;
				I3 = std::distance(m_Vertices.begin(), iter);
			}
			else
			{
				I3 = m_Vertices.size();
				pV3 = new Vertex(x, y, z, true);
				m_Vertices.push_back(pV3);
			}

			// Create the new element
			switch (m_MeshType)
			{
			case PURE_CONTINUOUS:
			case DISCONTINUOUS_SHARP_EDGES: // The discontinuous elements will replace the continuous ones in sharp edges
				// The replacement is done once the model loding is completed
				m_Elements.push_back(new ContElm(pV1, pV2, pV3, I1, I2, I3));
				break;

			case PURE_DISCONTINUOUS:
				m_Elements.push_back(new DiscElm(pV1, pV2, pV3, I1, I2, I3));
				break;
			}
		}

		if (pData)
			delete[] pData;

		m_strFileType = _T("BINARY (.STL)");
	}
}




void Model::LoadMeshFromFile(LPTSTR szFileName)
{
	// Choose the Mthode to use to read the files depending on the file extension
	tstring strFileName(szFileName);
	std::transform(strFileName.begin(), strFileName.end(),
		strFileName.begin(),
		[](unsigned char c) { return std::tolower(c); });




	if (strFileName.find(_T(".obj")) != tstring::npos)
	{
		LoadMeshFromOBJFile(szFileName);
	}
	else if (strFileName.find(_T(".stl")) != tstring::npos)
	{
		LoadMeshFromSTLFile(szFileName);
	}
	else
		return;

	// if the mesh type is DISCONTINUOUS_SHARP_EDGES 
	// Transform elements situated in sharp regions to discontinuous elements
	if(m_MeshType == DISCONTINUOUS_SHARP_EDGES)
		MakeSharpRegionsDiscontinous();

	// Calculate the number of continuous and discontinuous elements
	ComputeNumContDiscElms();

	// Save the previous center of mass
	m_prevCOM = GetCenterOfMass();

	// Translate the Model to It's center of Mass
	TranslateToCenterOfMass();

	// Calculate the Specific Length for the Element Subset Class
	CalculateSpecificLength();

	// After the mesh loading, we construct the the Vertex DOF Container
	ConstructVertexDOFContainer();

	// We Compute the free term matrix for every DOF Vertex
	CalculateFreeTermMatrices();

	// We set the specific Radius R0 every DOF Vertex
	SetSpecificRadii();


	// Set the Matrix Ram bails
	if(m_bAutoSetRamQuotas)
		AutoSetMatrixMemoryQuotas();
}



void Model::CreateSubSetFromWavefrontObjectFile(LPTSTR szFileName, LPTSTR szFileTitle)
{
	// Open the file using C++ standard library
	std::fstream file(szFileName, std::ios::in);

	// Create a temporary vertex container and a temporary element container
	std::vector<Vertex*> tempVertices;
	std::vector<Element* > tempElements;

	// Construct the vertex container
	while (file.rdstate() == std::fstream::goodbit)
	{
		char line[MAX_PATH];
		file.getline(line, MAX_PATH);

		std::stringstream str;
		str << line;

		char c1, c2;
		c1 = str.get();
		c2 = str.get();

		if (c1 == 'v' && c2 == ' ')
		{
			double x, y, z;

			str >> x;
			str >> y;
			str >> z;

			tempVertices.push_back(new Vertex(x, y, z));
		}
		else if (c1 == 'f' && c2 == ' ')
		{
			int I1, I2, I3;

			str >> I1;
			str >> I2;
			str >> I3;

			I1--;
			I2--;
			I3--;

			tempElements.push_back(new ContElm(tempVertices[I1], tempVertices[I2], tempVertices[I3], I1, I2, I3));
		}
	}

	// Translate the Temporary Model to the previous center of mass
	for (Vertex* pV : tempVertices)
		*pV -= m_prevCOM;

	// Now We create a vector of real indices based on the comparaison between the Temp vertices and Model vertices
	std::vector<int> trueIndices(tempVertices.size());
	int Idx = 0;
	for (Vertex* pVtemp : tempVertices)
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			Vertex* pV = m_Vertices[i];
			if (pV->Distance(*pVtemp) == 0.0)
			{
				trueIndices[Idx] = i;
				break;
			}

		}

		Idx++;
	}

	// Now We construct the Real Element Set
	std::vector<int> Indices;

	for (Element* pTempElm : tempElements)
	{
		int trueI1 = trueIndices[pTempElm->m_I1];
		int trueI2 = trueIndices[pTempElm->m_I2];
		int trueI3 = trueIndices[pTempElm->m_I3];

		for (size_t i = 0; i < m_Elements.size(); i++)
		{
			Element* pElm = m_Elements[i];

			if ((pElm->m_I1 == trueI1 &&
				pElm->m_I2 == trueI2 &&
				pElm->m_I3 == trueI3) ||

				(pElm->m_I1 == trueI2 &&
					pElm->m_I2 == trueI3 &&
					pElm->m_I3 == trueI1) ||

				(pElm->m_I1 == trueI3 &&
					pElm->m_I2 == trueI1 &&
					pElm->m_I3 == trueI2))


				Indices.push_back(i);

		}
	}

	//Delete The Temporary Containers
	for (Vertex* pV : tempVertices)
		if (pV)
			delete pV;
	tempVertices.clear();

	// Clean up the element container
	for (Element* pElm : tempElements)
		if (pElm)
			delete pElm;
	tempElements.clear();

	// Create the New Subset
	m_SubSets.push_back(new ElementSubSet(Indices, szFileTitle));
}




void Model::CreateSubSetFromSteriolithographyFile(LPTSTR szFileName, LPTSTR szFileTitle)
{
	// NOT IMPLEMENTED YET !!
}

Vector Model::GetCenterOfMass()const
{
	// Calculate the center of mass
	Vector G;
	double V = 0.0;

	for (const Element* pElm : m_Elements)
	{
		const Vector& v0 = *pElm->m_pV1;
		const Vector& v1 = *pElm->m_pV2;
		const Vector& v2 = *pElm->m_pV3;

		// Volume of tetrahedron formed with the origin
		double volume = (v0 * (v1 ^ v2)) / 6.0;

		// Centroid of the tetrahedron (triangle + origin)
		Vector centroid = (v0 + v1 + v2) / 4.0;

		G += centroid * volume;
		V += volume;
	}

	G = G / V;

	return G;
}

void Model::TranslateToCenterOfMass()
{
	// Calculate the center of mass
	Vector G;
	double V = 0.0;

	for (const Element* pElm : m_Elements)
	{
		const Vector& v0 = *pElm->m_pV1;
		const Vector& v1 = *pElm->m_pV2;
		const Vector& v2 = *pElm->m_pV3;

		// Volume of tetrahedron formed with the origin
		double volume = (v0 * (v1 ^ v2)) / 6.0;

		// Centroid of the tetrahedron (triangle + origin)
		Vector centroid = (v0 + v1 + v2) / 4.0;

		G += centroid * volume;
		V += volume;
	}

	G = G / V;

	// Translate the Mesh to the center of mass
	for (Vertex* pV : m_Vertices)
		*pV -= G;

	// Recalculate the internal control Nodes of the Elements
	// And Recalculate the Reusable integration Data
	for (Element* pElm : m_Elements)
	{
		if (pElm->Type() == DISCONTINUOUS)
		{
			DiscElm* pDiscElm = (DiscElm*)pElm;
			pDiscElm->CalculateControlNodes();
		}

		pElm->ComputeReusableIntegrationData();
	}
		
}



void Model::GetHitTestedElement(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, int* pHitTestedElmIdx, const D3DXMATRIX& rWorld)const
{
	float alpha = 2.0;
	for (int i = 0; i < m_Elements.size(); i++)
	{
		const Element* pElm = m_Elements[i];
		if (pElm->GetIntersectingElm(P1, P2, &alpha, rWorld))
			*pHitTestedElmIdx = i;
	}		
}

void Model::GetHitTestedPoint(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, Vertex** ppHitTestedPoint, const D3DXMATRIX& rWorld)const
{

	float D0 = FLT_MAX;

	// Check for discontinuous elements nodes
	for (Element* pElm : m_Elements)
	{
		if (pElm->Type() == DISCONTINUOUS)
		{
			DiscElm* pDiscElm = (DiscElm*)pElm;

			for (int l = 0; l < 3; l++)
			{
				Vertex* pNode = pDiscElm->Vdof_Ptr(l);



				// Calculate the Intersection node between the Ray P1P2 and the Plan of the triangle
		// Calculate the normal vector n
				D3DXVECTOR4 _V((float)pNode->x, (float)pNode->y, (float)pNode->z, 1.0f);

				D3DXVec4Transform(&_V, &_V, &rWorld);


				D3DXVECTOR3 V(_V.x, _V.y, _V.z);


				D3DXVECTOR3 Cross;

				D3DXVECTOR3 V1 = V - P1;
				D3DXVECTOR3 V2 = P2 - P1;

				float dotProd = D3DXVec3Dot(&V1, &V2);

				bool bInfront = (dotProd > 0.0f);


				D3DXVec3Normalize(&V1, &V1);
				D3DXVec3Normalize(&V2, &V2);

				D3DXVec3Cross(&Cross, &V1, &V2);

				float magCross = D3DXVec3Length(&Cross);

				bool bColinear = (magCross <= 0.005f);

				if (bColinear && bInfront)  // Coliniarity + Direction test 
				{


					float dist = D3DXVec3Length(&V1);


					if (D0 > dist)
					{
						D0 = dist;
						*ppHitTestedPoint = pNode;
					}
				}

			}

		}

	}

	// Check for Geometrical vertices
	for (Vertex* pV : m_Vertices)
	{	

		// Calculate the Intersection node between the Ray P1P2 and the Plan of the triangle
	    // Calculate the normal vector n
		D3DXVECTOR4 _V((float)pV->x, (float)pV->y, (float)pV->z, 1.0f);
		
		D3DXVec4Transform(&_V, &_V, &rWorld);
		

		D3DXVECTOR3 V(_V.x, _V.y, _V.z);
		

		D3DXVECTOR3 Cross;

		D3DXVECTOR3 V1 = V - P1;
		D3DXVECTOR3 V2 = P2 - P1;

		float dotProd = D3DXVec3Dot(&V1, &V2);

		bool bInfront = (dotProd > 0.0f);


		D3DXVec3Normalize(&V1, &V1);
		D3DXVec3Normalize(&V2, &V2);

		D3DXVec3Cross(&Cross, &V1, &V2);

		float magCross = D3DXVec3Length(&Cross);

		bool bColinear = (magCross <= 0.005f);

		if (bColinear && bInfront)  // Coliniarity + Direction test 
		{


			float dist = D3DXVec3Length(&V1);


			if (D0 > dist)
			{
				D0 = dist;
				*ppHitTestedPoint = pV;
			}
		}			
	}
	
	
}



void Model::CalculateSpecificLength()const
{
	double L = 0.0;
	for (Element* pElm : m_Elements)
	{
		double L1 = pElm->m_pV1->Distance(*pElm->m_pV2);
		double L2 = pElm->m_pV2->Distance(*pElm->m_pV3);
		double L3 = pElm->m_pV3->Distance(*pElm->m_pV1);

		if (L < L1)
			L = L1;

		if (L < L2)
			L = L2;

		if (L < L3)
			L = L3;
	}

	ElementSubSet::SpecLength = L;
}




void Model::CalculateTensors()
{
	for (Element* pElm : m_Elements)
		pElm->CalculateTensors();


	// Uniformise tensors 
	// The critersion to make two tensors uniform is that they must share the same geometrical vertex
	for (Vertex* pV : m_Vertices)
	{
		// Create a list of elements which share this same Geometrical vertex
		std::list<Element*> CommonElms;

		for (Element* pElm : m_Elements)
		{
			// Check if the geometrical vertex is part of the elements
			if (pElm->IsGeometricalVertexPartOf(pV))
				CommonElms.push_back(pElm);
		}

		// Create the new uniformized Tensors 
		Tensor Sig, Eps;
		for (Element* pElm : CommonElms)
		{
			// Get the local index of node pV
			int l = pElm->GetGeometricalVertexLocalIndex(pV);

			// Increment the Uniformized tensors
			Sig += pElm->Sig(l);
			Eps += pElm->Eps(l);
		}
		Sig = Sig / CommonElms.size();
		Eps = Eps / CommonElms.size();

		// Affect the uniformised Tensors to common elements
		for (Element* pElm : CommonElms)
		{
			// Get the local index of node pV
			int l = pElm->GetGeometricalVertexLocalIndex(pV);

			// Increment the Uniformized tensors
			pElm->Sig(l) = Sig;
			pElm->Eps(l) = Eps;
		}

		// Clear the common Elements container
		CommonElms.clear();
	}

}

void Model::DetectSharpRegions(std::list<Element*>& rSharpRegions)const
{
	// Loop through Geometrical vertices
	for (Vertex* pV : m_Vertices)
	{
		// Detect common Elements
		std::vector<Element*> CommonElms;

		for (Element* pElm : m_Elements)
			if (pElm->IsGeometricalVertexPartOf(pV))
				CommonElms.push_back(pElm);
		
		// Check if the vertex is a sharp corner 
		// by comparing it common elements pairwise
		bool bSharpCorner = false;
		for (unsigned i = 0; i < CommonElms.size(); i++)
		{
			if (bSharpCorner)
			{
				// Add Common Elements to the Sharp Region container
				for (Element* pElm : CommonElms)
					if (std::find(rSharpRegions.begin(), rSharpRegions.end(), pElm) == rSharpRegions.end())
						rSharpRegions.push_back(pElm);				

				break;
			}
				

			const Vector& ni = CommonElms[i]->m_n;

			for (unsigned j = i + 1; j < CommonElms.size(); j++)
			{
				const Vector& nj = CommonElms[j]->m_n;

				double SProd = abs(ni * nj);
				
				double Cs = cos(m_MaxSharpAngle);

				if (SProd < Cs)
				{
					bSharpCorner = true;
					break;
				}
			}
		}
	}
}


void Model::MakeSharpRegionsDiscontinous()
{
	// Create a container of elements situated in sharp regions
	std::list<Element*> SharpRegions;

	// Detect the sharp regions
	DetectSharpRegions(SharpRegions);

	// Loop through the Elements vectors container
	// Check if it is situated in a sharp region
	// if True: Convert it into a discontinuous element!!
	for (unsigned i = 0; i <m_Elements.size(); i++)
	{
		Element* pElm = m_Elements[i];

		if (std::find(SharpRegions.begin(), SharpRegions.end(), pElm) != SharpRegions.end())
		{
			DiscElm* pDiscElm = new DiscElm(pElm->m_pV1, pElm->m_pV2, pElm->m_pV3, pElm->m_I1, pElm->m_I2, pElm->m_I3);
			
			// Delete the Old element from memory
			delete pElm;
			
			// Replace it by the discontinuous element
			m_Elements[i] = pDiscElm;
		}
	}
}



void Model::ComputeNumContDiscElms()
{
	m_Nce = 0;
	m_Nde = 0;
	
	for (Element* pElm : m_Elements)
		if (pElm->Type() == CONTINUOUS)
			m_Nce++;
		else
			m_Nde++;

}


void Model::ConstructFTCHContainer(std::list<FTCH>& FTCHs, const Vertex* pV0)const
{
	// Empty the container
	FTCHs.clear();

	// Create a temporary container containing the Element Set sharing the vertex pV0
	std::list<Element*> ElmSet;
	for (Element* pElm : m_Elements)
	{
		if (pElm->IsGeometricalVertexPartOf(pV0))
			ElmSet.push_back(pElm);
	}

	// Loop through the created element set
	for (Element* pElm : ElmSet)
	{
		// Get the next and prev vertices to pV0
		int nLocalIdx = pElm->GetGeometricalVertexLocalIndex(pV0);
		int nIdx1 = (nLocalIdx + 1) % 3;
		int nIdx2 = (nIdx1 + 1) % 3;		
		Vertex* pV1 = pElm->Vgeo_Ptr(nIdx1);
		Vertex* pV2 = pElm->Vgeo_Ptr(nIdx2);
		
		// Try to add two FTCH structures
		// Test for vertex 1 and vertex 2
		auto iter1 = std::find_if(FTCHs.begin(), FTCHs.end(), [=](const FTCH& ftch)
			{
				return ftch.pVi == pV1;
			});

		auto iter2 = std::find_if(FTCHs.begin(), FTCHs.end(), [=](const FTCH& ftch)
			{
				return ftch.pVi == pV2;
			});

		// Check for Test 1
		if (iter1 == FTCHs.end()) // If the redendency test is good Create a new FTCH entry
		{
			FTCH ftch;
			ftch.pV0 = pV0;
			ftch.pVi = pV1;
			ftch.pE2 = pElm;

			FTCHs.push_back(ftch);
		}
		else // If the redendency test is bad get the FTCH entry and update its second element
		{
			FTCH& ftch = *iter1;
			ftch.pE2 = pElm;		
		}



		// Check for Test 2
		if (iter2 == FTCHs.end()) // If the redendency test is good Create a new FTCH entry
		{
			FTCH ftch;
			ftch.pV0 = pV0;
			ftch.pVi = pV2;
			ftch.pE1 = pElm;

			FTCHs.push_back(ftch);
		}
		else // If the redendency test is bad get the FTCH entry and update its second element
		{
			FTCH& ftch = *iter2;
			ftch.pE1 = pElm;			
		}
	}

	// Loop through the created Fetch container and set up the (r1, r2, n1, n2)
	for (FTCH& ftch : FTCHs)
	{
		// get the remaining Vertex pVr in pE2
		int nIdx = ftch.pE2->GetGeometricalVertexLocalIndex(ftch.pVi);
		int nIdxNext = (nIdx + 1) % 3;
		const Vertex& Vr = ftch.pE2->Vdof(nIdxNext);

		ftch.r1 = *ftch.pVi - *ftch.pV0; ftch.r1.Normalize();
		ftch.r2 = Vr  - *ftch.pV0; ftch.r2.Normalize();
		ftch.n1 = ftch.pE1->m_n;
		ftch.n2 = ftch.pE2->m_n;
	}

}


void Model::CalculateFreeTermMatrices() 
{
	// Check if the dof vertex is Geometrical

	for (Vertex* pV_dof : m_DOF_Vertices)
	{
		if (pV_dof->m_bGeometrical)
		{
			// Create an FTCH container
			std::list<FTCH> FTCHs;
			ConstructFTCHContainer(FTCHs, pV_dof);

			
			// Loop through the container
			double Alpha = 2.0 * M_PI;
			Matrix Ck; 

			pV_dof->R0 = FTCHs.back().r1.Magnitude();
			for (FTCH& ftch : FTCHs)
			{
 				double n1_v_n2_s_r1 = (ftch.n1 ^ ftch.n2) * ftch.r1;
				double n1_s_n2 = ftch.n1 * ftch.n2;


				// clamp dot product to [-1, 1]
				if (n1_s_n2 > 1.0) n1_s_n2 = 1.0;
				if (n1_s_n2 < -1.0) n1_s_n2 = -1.0;


				double sgn = 0.0;
				if(fabs(n1_v_n2_s_r1) >= 1.0E-9)
					sgn = n1_v_n2_s_r1 / fabs(n1_v_n2_s_r1);

				Alpha +=  (sgn * acos(n1_s_n2));


				Vector dr_v_n2 = (ftch.r2 - ftch.r1) ^ ftch.n2;

				Ck += Matrix::TensorProduct(dr_v_n2, ftch.n2);
			}
			Alpha /= (4.0 * M_PI);


			pV_dof->C = ((Matrix::Identity() * Alpha) - (Ck / (8.0 * M_PI * (1.0 - Element::v))));
		}
		else
			pV_dof->C = Matrix::Identity() * 0.5;

		
	}

	
	
}




void Model::SetSpecificRadii()
{
	for (Vertex* pV : m_Vertices)
	{
		pV->R0 = DBL_MAX;
		// Loop through the elements
		for (Element* pElm : m_Elements)
		{
			if (pElm->IsGeometricalVertexPartOf(pV)) // Check if the vertex is part of the element
			{

				int l1 = pElm->GetGeometricalVertexLocalIndex(pV);
				int l2 = (l1 + 1) % 3;
				int l3 = (l2 + 1) % 3;

				const Vertex& P1 = pElm->Vgeo(l1);
				const Vertex& P2 = pElm->Vgeo(l2);
				const Vertex& P3 = pElm->Vgeo(l3);

				// Calculate the Jump vectors
				Vector V21 = P1 - P2;
				Vector V23 = P3 - P2;
				

				// Calculate the lengths
				double L23 = V23.Magnitude();



				// Calculate the factor a
				double a = (V21 * V23) / (L23 * L23);
				

				Vertex Pm;

				if (a <= 0.0)
					Pm = P2;
				else if (a >= 1.0)
					Pm = P3;
				else
					Pm = P2 + V23 * a;

				double Dm = Pm.Distance(P1);

				if (pV->R0 > Dm)
					pV->R0 = Dm;

			}
		}

		pV->R0 *= 1.0;
	}
}




double Model::GetDomainDisplacement(const Vertex& DomainPt, int i)const
{
	// Get the number of elements
	int Ne = m_Elements.size();

	double Ui = 0.0;


	// Check if the Domain point is on the boundary
	for (int n = 0; n < Ne; n++)
	{
		Element* pElm = m_Elements[n];

		double Xi = 0.0;
		double Eta = 0.0;

		if (pElm->IsPointInside(DomainPt, Xi, Eta))
		{
			//interpolate the results inside the element and return !!

			Ui = pElm->GetFieldDisp(Xi, Eta, i);
			return Ui;
		}
	}


	


    #pragma omp parallel for
	for (int n = 0; n < Ne; n++)
	{
		Element* pElm = m_Elements[n];

		for (int l = 0; l < 3; l++)
		{
			const Vertex& Vdof = pElm->Vdof(l);
			const Vector& T = pElm->T(l);
			const Vector& U = Vdof.U;

			for (int j = 0; j < 3; j++)
			{
				double I1 = pElm->Q(DomainPt, i, j, l);
				double I2 = pElm->P(DomainPt, i, j, l);

                #pragma omp atomic
				Ui += (I1 * T[j] - I2 * U[j]);
			}
		}
	}

	return Ui;
}



double Model::GetDomainStress(const Vertex& DomainPt, int i, int j)const
{
	// Get the number of elements
	int Ne = m_Elements.size();

	double Sig_ij = 0.0;


	// Check if the Domain point is on the boundary
	for (int n = 0; n < Ne; n++)
	{
		Element* pElm = m_Elements[n];

		double Xi = 0.0;
		double Eta = 0.0;

		if (pElm->IsPointInside(DomainPt, Xi, Eta))
		{
			//interpolate the results inside the element and return !!

			Sig_ij = pElm->GetFieldStress(Xi, Eta, i, j);
			return Sig_ij;
		}
	}


    #pragma omp parallel for
	for (int n = 0; n < Ne; n++)
	{
		Element* pElm = m_Elements[n];

		for (int l = 0; l < 3; l++)
		{
			const Vertex& Vdof = pElm->Vdof(l);
			const Vector& T = pElm->T(l);
			const Vector& U = Vdof.U;

			for (int k = 0; k < 3; k++)
			{
				double I1 = pElm->ID(DomainPt, i, j, k, l);
				double I2 = pElm->IS(DomainPt, i, j, k, l);

                #pragma omp atomic
			    Sig_ij += (I1 * T[j] - I2 * U[j]);
			}
		}
	}

	return Sig_ij;
}



double Model::GetDomainDispDerivative(const Vertex& DomainPt, int i, int j)const
{
	// Get the number of elements
	int Ne = m_Elements.size();

	double Ui_j = 0.0;


	
    #pragma omp parallel for
	for (int n = 0; n < Ne; n++)
	{
		Element* pElm = m_Elements[n];

		for (int l = 0; l < 3; l++)
		{
			const Vertex& Vdof = pElm->Vdof(l);
			const Vector& T = pElm->T(l);
			const Vector& U = Vdof.U;

			for (int k = 0; k < 3; k++)
			{
				double I1 = pElm->IW(DomainPt, i, j, k, l);
				double I2 = pElm->IV(DomainPt, i, j, k, l);

                #pragma omp atomic
				Ui_j += (I1 * T[j] - I2 * U[j]);
			}
		}
	}

	return Ui_j;
}


double Model::GetDomainStrain(const Vertex& DomainPt, int i, int j)const
{
	// Get the number of elements
	int Ne = m_Elements.size();

	double Eps_ij = 0.0;

	// Check if the Domain point is on the boundary
	for (int n = 0; n < Ne; n++)
	{
		Element* pElm = m_Elements[n];

		double Xi = 0.0;
		double Eta = 0.0;

		if (pElm->IsPointInside(DomainPt, Xi, Eta))
		{
			//interpolate the results inside the element and return !!

			Eps_ij = pElm->GetFieldStrain(Xi, Eta, i, j);
			return Eps_ij;
		}
	}


	if (i == j)
		return GetDomainDispDerivative(DomainPt, i, j);

	double Ui_j = GetDomainDispDerivative(DomainPt, i, j);
	double Uj_i = GetDomainDispDerivative(DomainPt, j, i);

	return (Ui_j + Uj_i) * 0.5;
}


Vector Model::GetDomainDispVector(const Vertex& DomainPt)const
{
	double Ux = GetDomainDisplacement(DomainPt, 0);
	double Uy = GetDomainDisplacement(DomainPt, 1);
	double Uz = GetDomainDisplacement(DomainPt, 2);

	return Vector(Ux, Uy, Uz);
}

Tensor Model::GetDomainStressTensor(const Vertex& DomainPt)const
{
	double Sig_11 = GetDomainStress(DomainPt, 0, 0);
	double Sig_22 = GetDomainStress(DomainPt, 1, 1);
	double Sig_33 = GetDomainStress(DomainPt, 2, 2);
	double Sig_12 = GetDomainStress(DomainPt, 0, 1);
	double Sig_23 = GetDomainStress(DomainPt, 1, 2);
	double Sig_31 = GetDomainStress(DomainPt, 2, 0);

	return Tensor(Sig_11, Sig_22, Sig_33, Sig_12, Sig_31, Sig_23);
}

Tensor Model::GetDomainStrainTensor(const Vertex& DomainPt)const
{
	double Eps_11 = GetDomainStrain(DomainPt, 0, 0);
	double Eps_22 = GetDomainStrain(DomainPt, 1, 1);
	double Eps_33 = GetDomainStrain(DomainPt, 2, 2);
	double Eps_12 = GetDomainStrain(DomainPt, 0, 1);
	double Eps_23 = GetDomainStrain(DomainPt, 1, 2);
	double Eps_31 = GetDomainStrain(DomainPt, 2, 0);

	return Tensor(Eps_11, Eps_22, Eps_33, Eps_12, Eps_31, Eps_23);
}