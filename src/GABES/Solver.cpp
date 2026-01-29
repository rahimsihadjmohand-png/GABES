#include "Ahmed_Overrides.h"
#include "framework.h"
#include "Model.h"

#define _IN_
#define _OUT_





// Implementation of AHMED H-Matrix Library DOFs
// Vertex DOF

using BEM_3D::AHMED_Vertex;
using BEM_3D::AHMED_Element;
using BEM_3D::Model;
using BEM_3D::SOLVER;


SOLVER Model::m_Solver = HLU;            // The solver type used
unsigned Model::m_NumSteps = 50u;       // The number of steps if we are using krylov subspace solvers
double Model::m_eps = 1.0E-9;           // The error threshold in the solver


double AHMED_Vertex::getcenter(unsigned i) const
{
    switch (i)
    {
    case 0:
        return pV->x;

    case 1:
        return pV->y;

    case 2:
        return pV->z;
    }

    return 0.0; // Impossible value
}

double AHMED_Vertex::getradius2() const
{
    return 0.0;
}



// Element DOF
double AHMED_Element::getcenter(unsigned i) const
{
    Vertex C = pElm->GetCentroid();

    switch (i)
    {
    case 0:
        return C.x;

    case 1:
        return C.y;

    case 2:
        return C.z;
    }

    return 0.0; // Impossible value
}


double AHMED_Element::getradius2() const
{
    return pElm->GetRadius2();
}







template <typename T>
void DenseToCRS(_IN_ const T** D, 
                _IN_ unsigned N, 
                _IN_ unsigned M, 
                _OUT_ T*& A, 
                _OUT_ unsigned*& col_idx, 
                _OUT_ unsigned*& row_ptr)
{
    // Calculate NNZ
    unsigned NNZ = 0;

    for (unsigned i = 0; i < N; i++)
        for (unsigned j = 0; j < M; j++)
            if (D[i][j] != 0.0)
                NNZ++;

    // Allocate memory for the Sparce matrix
    A = new T[NNZ];
    col_idx = new unsigned[NNZ];
    row_ptr = new unsigned[N + 1];

    // Fill the Sparce matrix
    unsigned idx = 0;
    for (unsigned i = 0; i < N; i++)
    {
        row_ptr[i] = idx;
        for (unsigned j = 0; j < M; j++)
        {
            if (D[i][j] != 0.0)
            {
                A[idx] = D[i][j];
                col_idx[idx] = j;
                idx++;
            }

        }            
    }
    row_ptr[N] = NNZ;
}



template <typename T>
void DenseToCCS(_IN_ const T** D,
                _IN_ unsigned N,
                _IN_ unsigned M,
                _OUT_ T*& A,
                _OUT_ unsigned*& row_idx,
                _OUT_ unsigned*& col_ptr)
{
    // Calculate NNZ
    unsigned NNZ = 0;

    for (unsigned i = 0; i < N; i++)
        for (unsigned j = 0; j < M; j++)
            if (D[i][j] != 0.0)
                NNZ++;

    // Allocate memory for the Sparce matrix
    A = new T[NNZ];
    row_idx = new unsigned[NNZ];
    col_ptr = new unsigned[M + 1];

    // Fill the Sparce matrix
    unsigned idx = 0;
    for (unsigned j = 0; j < M; j++)
    {
        col_ptr[j] = idx;
        for (unsigned i = 0; i < N; i++)
        {
            if (D[i][j] != 0.0)
            {
                A[idx] = D[i][j];
                row_idx[idx] = i;
                idx++;
            }

        }
    }
    col_ptr[M] = NNZ;
}


void Model::ConstructAndResolveLinearSystem()
{
	// Verify the dimension of the system
	if (m_Elements.empty())
		return;


    m_CurrentAdvance = 0.0;
    m_bLengthyJob = true;
	::AfxBeginThread(ConstructAndResolveLinearSystemThread, (LPVOID)this);
}




UINT Model::ConstructAndResolveLinearSystemThread(LPVOID pParam)
{
    Model* pObj = (Model*)pParam; 
    
    pObj->m_CurrentAdvance = 0.0;
    pObj->m_strCurrentJob = _T("Construction of a linear system [A] . {x} = {b}...");
    pObj->ConstructLinearSystem();


    pObj->m_CurrentAdvance = 0.0;
    pObj->m_strCurrentJob = _T("Transform the square matrix [A] into an H-Matrix...");
    unsigned Nv = pObj->m_DOF_Vertices.size(); // Number of vertices
    unsigned N = Nv * 3;                       // Dimension of the matrices

    // Rank max
    unsigned rankmax = 50;

    // Eta
    double Eta = 2.0;

    // leaf size
    unsigned leafsize = 36; 
    
   


    // 1) Create DOFs
    std::vector<AHMED_Vertex> V_dofs(Nv);
    //std::vector<AHMED_Element> E_dofs(N);


    for (unsigned i = 0; i < Nv; i++)
    {
        V_dofs[i].pV = pObj->m_DOF_Vertices[i];
    }

    // 2) Permutations
    std::vector<unsigned> v_op_perm(N);
    std::vector<unsigned> v_po_perm(Nv);

    /*std::vector<unsigned> e_op_perm(N);
    std::vector<unsigned> e_po_perm(Ne);*/

    for (unsigned i = 0; i < N; i++)
    {
        v_op_perm[i] = i / 3;
        v_po_perm[v_op_perm[i]] = i;
        /*e_op_perm[i] = i / 9;
        e_po_perm[e_op_perm[i]] = i;*/
    }

  
    // 3) Create 2 BEM clusters for Vertices and elements
    bemcluster<AHMED_Vertex> bemcl(V_dofs.data(), v_op_perm.data(), 0, N);
    bemcl.createClusterTree(leafsize, v_op_perm.data(), v_po_perm.data());

    /*bemcluster<AHMED_Element> colcl(E_dofs.data(), e_op_perm.data(), 0, N);
    colcl.createClusterTree(leafsize, e_op_perm.data(), e_po_perm.data());*/

    // 4) Create the BEM Block cluster tree
    bemblcluster <AHMED_Vertex, AHMED_Vertex> bct(&bemcl, &bemcl);
    unsigned NBlocks = 0;
    bct.subdivide(&bemcl, &bemcl, Eta * Eta, NBlocks);

    // 5) H-matrix
    GeHMatrix<double> H(N, N, &bct);   
   

    initGeH_0(H.blclTree, H.blcks);

    
    pObj->A.OpenFile(true);
    matgenGeH_omp<double, AHMED_Vertex, AHMED_Vertex, LD_Matrix<double>>(pObj->A,
        NBlocks, 
        &bct,
        m_eps,
        rankmax,
        H.blcks);
    pObj->A.CloseFile();
    

    
    pObj->m_CurrentAdvance = 0.0;
    switch (m_Solver)
    {
    case HLU:
    {
        pObj->m_strCurrentJob = _T("Create an H-LU decomposition...");

        GeHMatrix<double> L(N, N, &bct);
        GeHMatrix<double> U(N, N, &bct);
        initLtH_0(L.blclTree, L.blcks);
        initUtH_0(U.blclTree, U.blcks);

        myHLU(&bct, H.blcks, L.blcks, U.blcks, m_eps, rankmax, &pObj->m_CurrentAdvance, NBlocks);
        CopyMemory(pObj->x, pObj->b, sizeof(double) * N);

        pObj->m_strCurrentJob = _T("Solve the decomposed system [L][U] {x} = {b} using Forward-Backward substitution...");
        HLU_solve(&bct, L.blcks, U.blcks, pObj->x);  
    }
    break;

    case GMRES:
    {
       /* pObj->m_strCurrentJob = _T("Generate an LU preconditioner...");
        genLUprecond(H.blclTree, H.blcks, 0.1, rankmax, H.blAggl, H.L, H.U, true);*/


        pObj->m_strCurrentJob = _T("Solve the decomposed system [A] {x} = {b} using GMRes iterative solver...");
        double eps = m_eps;
        unsigned nSteps = m_NumSteps;
        myGMRes(H, pObj->b, pObj->x, eps, rankmax, nSteps, &pObj->m_CurrentAdvance);
    }
    break;


    case BICGSTAB:
    {
       /* pObj->m_strCurrentJob = _T("Generate an LU preconditioner...");
        genLUprecond(H.blclTree, H.blcks, 0.1, rankmax, H.blAggl, H.L, H.U, true);*/


        pObj->m_strCurrentJob = _T("Sove the decomposed system [A] {x} = {b} using BiCGStab iterative solver...");
        double eps = m_eps;
        unsigned nSteps = m_NumSteps;
        myBiCGStab(H, pObj->b, pObj->x, eps, nSteps, &pObj->m_CurrentAdvance);
    }
    break;
    }    
  
    pObj->m_bLengthyJob = false;
    pObj->m_CurrentAdvance = 1.0;

    pObj->m_strCurrentJob = _T("The resolution of the boundary value problem completed successfully!");

    return 0;
}

