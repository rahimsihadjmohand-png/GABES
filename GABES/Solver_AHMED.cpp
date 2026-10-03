#include "Ahmed_Overrides.h"



#define _IN_
#define _OUT_

// Implementation of AHMED H-Matrix Library DOFs
// Vertex DOF

using BEM_3D::ROW_DOF;
using BEM_3D::COL_DOF;
using BEM_3D::Model;
using BEM_3D::Element;
using BEM_3D::SOLVER;


SOLVER Model::m_Solver = HLU;                  // The solver type used
unsigned Model::m_NumSteps = 50u;              // The number of steps if we are using krylov subspace solvers
double Model::m_eps = 1.0E-6;                  // The error threshold in the solver
unsigned Model::m_nMinLeafSize = 36;           // The minimum leaf size
unsigned Model::m_nMaxRank = 100;              // The maximum rank 
double Model::m_Eta2 = 1.0;

// ROW DOF
double ROW_DOF::getcenter(unsigned i) const
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

//double ROW_DOF::getradius2() const
//{
//    return 0.0;
//}



// COL DOF

//std::vector<Element*>* COL_DOF::pElements = nullptr;

double COL_DOF::getcenter(unsigned i) const
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


//double COL_DOF::getradius2() const
//{
//    std::vector<Element*>& rElements = *pElements;
//
//    double xmin = pV->x, xmax = pV->x;
//    double ymin = pV->y, ymax = pV->y;
//    double zmin = pV->z, zmax = pV->z;
//
//    for (const VtxElmMap& vem : pV->m_ElmsMap)
//    {
//        Element* pElm = rElements[vem.ElmIdx];
//
//        for (int l = 0; l < 3; l++)
//        {
//            Vertex* pVl = pElm->Vdof_Ptr(l);
//
//            xmin = min(xmin, pVl->x);
//            xmax = max(xmax, pVl->x);
//
//            ymin = min(ymin, pVl->y);
//            ymax = max(ymax, pVl->y);
//
//            zmin = min(zmin, pVl->z);
//            zmax = max(zmax, pVl->z);
//        }
//    }
//
//    double dx = xmax - xmin;
//    double dy = ymax - ymin;
//    double dz = zmax - zmin;
//
//    return dx * dx + dy * dy + dz * dz;
//}




void Model::ResolveLinearSystem_AHMED()
{
	// Verify the dimension of the system
	if (m_Elements.empty())
		return;


    m_CurrentAdvance = 0.0;
    m_bLengthyJob = true;
	::AfxBeginThread(ResolveLinearSystemThread_AHMED, (LPVOID)this);
}



//void Model::GaussElimination()
//{
//    // Get the dimention of the A matrix
//    int N = 3 * (int)m_DOF_Vertices.size();
//
//
//    LD_Matrix<double> M(N, N+1, A.GetMaxRamAlloc(), _T("M.ldmat"), &m_CurrentAdvance);
//
//    for (int i = 0; i < N; i++)
//    {
//        for (int j = 0; j < N; j++)
//            M.Set(j, i, A[j][i]);
//        
//        M.Set(N, i, b[i]);
//    }
//  
//
//    
//    // Echlonate the matrix M (Make it triangular superior) 
//    double dN = (double)N;
//
//    double TotalSteps = dN * dN * dN / 3.0 + dN * dN - 4.0 * dN / 3.0;
//
//    double CummSteps = 0.0;
//    
//
//    m_strCurrentJob = _T("Echlonate the dense matrix [A]...");
//
//    for (int k = 0; k < N - 1; k++)
//    {
//        for (int i = k + 1; i < N; i++)
//        {
//            if (!m_bLengthyJob)
//                return;
//
//            double Pivot = M[k][i] / M[k][k];
//            for (int j = k; j < N + 1; j++, CummSteps += 1.0)
//            {
//
//                m_CurrentAdvance = (double)CummSteps / (double)TotalSteps;
//                M.Set(j, i, M[j][i] - Pivot * M[j][k]);
//            }
//        }
//    }
//
//    m_strCurrentJob = _T("Resolve the linear system by using backward substitution...");
//    // Resolve the system by calculating the last element and comming Up
//    for (int i = N - 1; i >= 0; i--)
//    {
//        if (!m_bLengthyJob)
//            return;
//
//        double S = 0.0;
//        for (int j = i + 1; j < N; j++, CummSteps += 1.0)
//        {
//            S += x[j] * M[j][i];
//            m_CurrentAdvance = (double)CummSteps / (double)TotalSteps;
//        }
//        x[i] = (M[N][i] - S) / M[i][i];
//    }
//
//}






UINT Model::ResolveLinearSystemThread_AHMED(LPVOID pParam)
{
    // Get the THIS pointer of the model object
    Model* pObj = (Model*)pParam;

   
    //====== Check for Lengthy job flag ======
    if (!pObj->m_bLengthyJob)
        return 0;
    //========================================

    
    pObj->m_strCurrentJob = _T("Copying boundary conditions from subsets...");
    pObj->m_CurrentAdvance = 0.0;
    pObj->CopyBoundaryConditionsFromSubSets();
    pObj->m_CurrentAdvance = 1.0;


    // Update the A matrix and construct the RHS vector using prescribed boundary conditions
    pObj->m_strCurrentJob = _T("Compute the {b} right-hand side vector...");
    pObj->m_CurrentAdvance = 0.0;
    pObj->ConstructRHSvector();
    pObj->m_CurrentAdvance = 1.0;


    //====== Check for Lengthy job flag ======
    if (!pObj->m_bLengthyJob)
        return 0;
    //========================================

  
   
    pObj->m_strCurrentJob = _T("Construct the H-Matrix...");
    unsigned Nv = pObj->m_DOF_Vertices.size(); // Number of vertices
    unsigned Ne = pObj->m_Elements.size();     // Number of elements
    unsigned N = Nv * 3;                       // Dimension of the matrices

   
    // 1) Create DOFs

    //COL_DOF::pElements = &(pObj->m_Elements);

    std::vector<ROW_DOF> row_dofs(Nv);
    std::vector<COL_DOF> col_dofs(Nv);


    for (unsigned i = 0; i < Nv; i++)
    {
        row_dofs[i].pV = pObj->m_DOF_Vertices[i];
        col_dofs[i].pV = pObj->m_DOF_Vertices[i];
    }


    
    // 2) Permutations
    std::vector<unsigned> v_op_perm(N);
    std::vector<unsigned> v_po_perm(Nv);

    std::vector<unsigned> e_op_perm(N);
    std::vector<unsigned> e_po_perm(Nv);

    for (unsigned i = 0; i < N; i++)
    {
        v_op_perm[i] = i / 3;
        v_po_perm[v_op_perm[i]] = i;

        e_op_perm[i] = i / 3;
        e_po_perm[e_op_perm[i]] = i;
    }

  
    // 3) Create 2 BEM clusters for Vertices and elements
    bemcluster<ROW_DOF> rowcl(row_dofs.data(), v_op_perm.data(), 0, N);
    rowcl.createClusterTree(m_nMinLeafSize, v_op_perm.data(), v_po_perm.data());


    bemcluster<COL_DOF> colcl(col_dofs.data(), e_op_perm.data(), 0, N);
    colcl.createClusterTree(m_nMinLeafSize, e_op_perm.data(), e_po_perm.data());

    // 4) Create the BEM Block cluster tree
    bemblcluster <ROW_DOF, COL_DOF> bct(& rowcl, & colcl);
    unsigned NBlocks = 0;
    bct.subdivide(&rowcl, &colcl, Model::m_Eta2, NBlocks);

    // 5) H-matrix
    GeHMatrix<double> H(N, N, &bct);   
   

    initGeH_0(H.blclTree, H.blcks);

    
   myMatgenGeH_omp<double, ROW_DOF, COL_DOF, Model>(*pObj,
        NBlocks,
        &bct,
        m_eps,
        m_nMaxRank,
        H.blcks);
    


   //====== Check for Lengthy job flag ======
   if (!pObj->m_bLengthyJob)
       return 0;
   //========================================



    
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

        myHLU(&bct, H.blcks, L.blcks, U.blcks, m_eps, m_nMaxRank, *pObj, NBlocks);
        CopyMemory(pObj->x, pObj->b, sizeof(double) * N);

        pObj->m_strCurrentJob = _T("Solve the decomposed system [L][U] {x} = {b} using Forward-Backward substitution...");
        HLU_solve(&bct, L.blcks, U.blcks, pObj->x);  
    }
    break;

    case GMRES:
    {
        pObj->m_strCurrentJob = _T("Generate an LU preconditioner...");
        double delta = 0.001;
        blcluster* blLU;
        mblock<double>** L;
        mblock<double>** U;
        genLUprecond(H.blclTree, H.blcks, delta, m_nMaxRank, blLU, L, U, false);


        pObj->m_strCurrentJob = _T("Solve the system [A] {x} = {b} using GMRes iterative solver...");
        double eps = m_eps;
        unsigned nSteps = m_NumSteps;
        myGMRes(H, pObj->b, pObj->x, eps, m_nMaxRank, nSteps, *pObj);
    }
    break;


    case BICGSTAB:
    {
        pObj->m_strCurrentJob = _T("Generate an LU preconditioner...");
        double delta = 0.001;
        blcluster* blLU;
        mblock<double>** L;
        mblock<double>** U;
        genLUprecond(H.blclTree, H.blcks, delta, m_nMaxRank, blLU, L, U, false);


        pObj->m_strCurrentJob = _T("Sove the decomposed system [A] {x} = {b} using BiCGStab iterative solver...");
        double eps = m_eps;
        unsigned nSteps = m_NumSteps;
        myBiCGStab(H, pObj->b, pObj->x, eps, nSteps, *pObj);
    }
    break;
    }    
  
    pObj->m_bLengthyJob = false;
    pObj->m_CurrentAdvance = 1.0;

    pObj->m_strCurrentJob = _T("The resolution of the boundary value problem completed successfully!");

    return 0;
}

