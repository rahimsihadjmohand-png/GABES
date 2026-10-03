

#pragma once
#ifndef WIN32
#define WIN32
#endif // !WIN32

#define _CRT_SECURE_NO_WARNINGS
#define _OPENMP



#include <matgen_omp.h>
#include <matrix.h>
#include <H.h>
#include <solvers.h>

namespace MKL
{
   #include <mkl.h>
};


#include "framework.h"
#include "Model.h"


using BEM_3D::Vector;
using BEM_3D::Element;

//#include <fstream>



template<class T, class MATGEN_T>
static bool myHLU(blcluster* const bl, mblock<T>** const A, mblock<T>** const L,
    mblock<T>** const U, const double eps, const unsigned rankmax,
    MATGEN_T& MatGen, unsigned nBlocks,               // fraction 0..1
    contBasis<T>* haar = nullptr)
{
    //====== Check for Lengthy job flag ======
    if (!MatGen.m_bLengthyJob)
        return false;
    //========================================

    if (bl->isleaf()) 
    {
        unsigned idx = bl->getidx();
        int inf = A[idx]->decomp_LU(L[idx], U[idx]);
        
        if (inf != 0)
            return false;      

    }
    else 
    {
        assert(bl->getnrs() == bl->getncs());
        unsigned i, j, k, ns = bl->getnrs();
        contBasis<T>* haarSon = nullptr;

        for (i = 0; i < ns; ++i) {
            blcluster* son = bl->getson(i, i);
            if (haar) haarSon = haar->son(i, i);

            for (k = 0; k < i; ++k) 
            {
                blcluster* son1 = bl->getson(i, k), * son2 = bl->getson(k, i);
                mltaGeHGeH((T)-1.0, son1, L, son2, U, son, A, eps, rankmax, haarSon);
            }

            if (!myHLU(son, A, L, U, eps, rankmax, MatGen, nBlocks, haarSon))
                return false;

            delete haarSon;

            for (j = i + 1; j < ns; ++j) 
            {
                blcluster* sonU = bl->getson(i, j);
                if (haar) haarSon = haar->son(i, j);
                for (k = 0; k < i; ++k) 
                {
                    blcluster* son1 = bl->getson(i, k), * son2 = bl->getson(k, j);
                    mltaGeHGeH((T)-1.0, son1, L, son2, U, sonU, A, eps, rankmax, haarSon);
                }
                LtHGeH_solve(son, L, sonU, A, U, eps, rankmax, haarSon);
                if (haarSon)
                    delete haarSon;

                blcluster* sonL = bl->getson(j, i);
                if (haar) haarSon = haar->son(j, i);
                for (k = 0; k < i; ++k) 
                {
                    blcluster* son1 = bl->getson(j, k), * son2 = bl->getson(k, i);
                    mltaGeHGeH((T)-1.0, son1, L, son2, U, sonL, A, eps, rankmax, haarSon);
                }
                GeHUtH_solve(son, U, sonL, A, L, eps, rankmax, haarSon);
                if (haarSon)
                    delete haarSon;
            }
        }
    }
    
    // --- Update progress as fraction ---
    #pragma omp critical   // safe for multithreaded H-LU
    {
       MatGen.m_CurrentAdvance += (1.0 / (double)nBlocks);
       // assumes each leaf knows total number of leaf blocks
       // or normalize later: m_CurrentAdv / totalLeaves
    }    

    return true;
}


// matrix generation procedure for unsymmetric matrices
template<class T, class T1, class T2, class MATGEN_T>
void MyApprxSVD_unsym(MATGEN_T& MatGen, mblock<T>*& mbl, bemblcluster<T1, T2>* bl,
    double eps, unsigned rankmax)
{
    unsigned n1 = bl->getn1();  // block row dimension
    unsigned n2 = bl->getn2();  // block column dimension

    unsigned b1 = bl->getb1();  // block row starting index 
    unsigned b2 = bl->getb2();  // block column strating index


    unsigned maxk = n1 * n2 / (n1 + n2); // the maximal benific rank

    //// maximal rank checking
    if (maxk > rankmax) 
        maxk = rankmax;

    // H-Matrix block allocation
    mbl = new mblock<T>(n1, n2);
    assert(mbl != NULL);

    // The success boolean flag
    bool succ = false;

    // If the block is admissible approximate it as a rank-r matrix r = maxk
    if (bl->isadm() && maxk)
    {
        // AT FIRST WE COMPUTE THE ECONOMY SVD
        int m = n1;
        int n = n2;
        int r = MIN(m, n); 
        
        
        double* A = new double[m * n];  // The original matrix A
        double* S = new double[r];  // The singular values
        double* U = new double[m * r]; // The Left Orthogonal matrix
        double* VT = new double[r * n]; // The Right Orhtogonal matrix
        double* V = A;

        // Compute the dense block
        MatGen.cmpbl(b1, n1, b2, n2, A);

        
        int lda = m;
        int ldu = m;
        int ldvt = r;
        char jobu = 'S';
        char jobvt = 'S';
        int lwork = -1;
        double wkopt;
        int info;
        

        // Do a workspace query
        MKL::dgesvd_(
            &jobu,
            &jobvt,
            &m,
            &n,
            A,
            &lda,
            S,
            U,
            &ldu,
            VT,
            &ldvt,
            &wkopt,
            &lwork,
            &info);

        // recover the optimal dimension of the workspace
        lwork = (int)wkopt;

        double* work = new double[lwork];


        // Now copute the  economy SVD
        MKL::dgesvd_(
            &jobu,
            &jobvt,
            &m,
            &n,
            A,
            &lda,
            S,
            U,
            &ldu,
            VT,
            &ldvt,
            work,
            &lwork,
            &info);
               

        if (info != 0)
        {
            succ = false;
        }
        else
        {
            // Compute the reduced SVD
            unsigned kt = r;

            while (kt > 0 && S[kt - 1] < S[0] * eps)  // in this way We obtain full rank !!
                --kt;


            if (kt > 0 && kt <= maxk) 
            {
                succ = true;
                mbl->setrank(kt);

                for (int j = 0; j < r; j++)
                    for (int i = 0; i < m; i++)
                        U[m * j + i] *= S[j];

                blas::transpose(r, n, VT, V);

                mbl->cpyLrM_cmpr(kt, U, m, V, n, eps, r);
            }
            else
                succ = false;
        }        

        delete[] A;
        delete[] U;
        delete[] VT;
        delete[] S;
        delete[] work;

    }
    
    if(!succ)
    {
        mbl->setGeM();
        MatGen.cmpbl(b1, n1, b2, n2, mbl->getdata());
    }
}



template<class T, class T1, class T2, class MATGEN_T> static
void _myThr(MATGEN_T& MatGen, unsigned& co, unsigned nblcks,
    bemblcluster<T1, T2>* bl, double eps, unsigned rankmax, mblock<T>** A)
{
    /*char st[100];
    sprintf(st, "Approximating using %d threads ... ", omp_get_num_threads());
    progressbar(std::cout, st, co++, nblcks, 20, true);*/
    #pragma omp critical
    {
        MatGen.m_CurrentAdvance = (double)co++ / (double)nblcks;
    }

    //MyApprxSVD_unsym(MatGen, A[bl->getidx()], bl, eps, rankmax); // Economy SVD
    apprx_unsym(MatGen, A[bl->getidx()], bl, eps, rankmax);    // ACA
}



template<class T, class T1, class T2, class MATGEN_T>
void myMatgenGeH_omp(MATGEN_T& MatGen, unsigned nblcks, bemblcluster<T1, T2>* bl,
    double eps, unsigned rankmax, mblock<T>**& A)
{
    // generate the list of blocks
    blcluster** BlList;
    gen_BlSequence(bl, BlList);

   

   unsigned counter = 0;

    #pragma omp parallel for schedule(dynamic)
   for (int i = 0; i < (int)nblcks; i++)
   {
       //====== Check for Lengthy job flag ======
       if (!MatGen.m_bLengthyJob)
           break;
       //========================================

       _myThr(MatGen, counter, nblcks, (bemblcluster<T1, T2>*)BlList[i], eps,
           rankmax, A);
   }
        


    //===============================================================================
    std::fstream File("H-MatInfo.txt", std::ios::out);

    
    File << nblcks << std::endl; // The number of blocks
    File << BlList[nblcks - 1]->getb1() + BlList[nblcks - 1]->getn1() << std::endl; // The Number of DOFs


    for (int i = 0; i < (int)nblcks; i++)
    {

        int n1 = BlList[i]->getn1();
        int n2 = BlList[i]->getn2();
        int b1 = BlList[i]->getb1();
        int b2 = BlList[i]->getb2();
              

        File << b1 << "\t" << b2 << "\t" << n1 << "\t" << n2 << "\t" << BlList[i]->isLrM(A) << "\t" << BlList[i]->rank(A) << "\n";
    }
    

    File.close();
    // =========================================================================================


    delete[] BlList;
}



//// Derived class for right LU preconditioner on an existing H-matrix
//struct GeHMatrixLUPrecond : public GeHMatrix<double> {
//    mblock<double>** L;       // Lower H-matrix factor
//    mblock<double>** U;       // Upper H-matrix factor
//    blcluster* blAggl;         // Block cluster tree for LU
//    double* xf;               // Temporary vector for substitutions
//
//    // Constructor from existing H-matrix and LU factors
//    GeHMatrixLUPrecond(unsigned n, blcluster* tree = NULL)
//        : GeHMatrix<double>(n, n, tree)
//        , xf(new double[n])  
//        , L(nullptr)
//        , U(nullptr)
//    {     
//    }
//
//    ~GeHMatrixLUPrecond() {
//        delete[] xf;
//
//        if(L)
//            freembls(blAggl, L);
//        
//        if(U)
//            freembls(blAggl, U);
//    }
//
//    // Apply right preconditioner C = inv(L*U)
//    void precond_apply(double* x) const override {
//        for (unsigned i = 0; i < this->n; i++) xf[i] = (double)x[i];
//        HLU_solve(blAggl, L, U, xf);
//        for (unsigned i = 0; i < this->n; i++) x[i] = xf[i];
//    }
//};




unsigned myGMRes(const Matrix<double>& A, double* const b, double* const x,
    double& eps, const unsigned m, unsigned& nsteps, BEM_3D::Model& rObj);



unsigned myBiCGStab(const Matrix<double>& A, double* const b, double* const x,
    double& eps, unsigned& nsteps, BEM_3D::Model& rObj);




