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
#include <cluster_bbx.h>


template<class T>
static bool myHLU(blcluster* const bl, mblock<T>** const A, mblock<T>** const L,
    mblock<T>** const U, const double eps, const unsigned rankmax,
    double* m_CurrentAdv, unsigned nBlocks,               // fraction 0..1
    contBasis<T>* haar = nullptr)
{
    if (bl->isleaf()) {
        unsigned idx = bl->getidx();
        int inf = A[idx]->decomp_LU(L[idx], U[idx]);
        if (inf != 0) return false;

        // --- Update progress as fraction ---
        if (m_CurrentAdv) {
#pragma omp critical   // safe for multithreaded H-LU
            {
                *m_CurrentAdv += 1.0 / (double)nBlocks;
                // assumes each leaf knows total number of leaf blocks
                // or normalize later: m_CurrentAdv / totalLeaves
            }
        }

    }
    else {
        assert(bl->getnrs() == bl->getncs());
        unsigned i, j, k, ns = bl->getnrs();
        contBasis<T>* haarSon = nullptr;

        for (i = 0; i < ns; ++i) {
            blcluster* son = bl->getson(i, i);
            if (haar) haarSon = haar->son(i, i);

            for (k = 0; k < i; ++k) {
                blcluster* son1 = bl->getson(i, k), * son2 = bl->getson(k, i);
                mltaGeHGeH((T)-1.0, son1, L, son2, U, son, A, eps, rankmax, haarSon);
            }

            if (!myHLU(son, A, L, U, eps, rankmax, m_CurrentAdv, nBlocks, haarSon))
                return false;
            delete haarSon;

            for (j = i + 1; j < ns; ++j) {
                blcluster* sonU = bl->getson(i, j);
                if (haar) haarSon = haar->son(i, j);
                for (k = 0; k < i; ++k) {
                    blcluster* son1 = bl->getson(i, k), * son2 = bl->getson(k, j);
                    mltaGeHGeH((T)-1.0, son1, L, son2, U, sonU, A, eps, rankmax, haarSon);
                }
                LtHGeH_solve(son, L, sonU, A, U, eps, rankmax, haarSon);
                if (haarSon)
                    delete haarSon;

                blcluster* sonL = bl->getson(j, i);
                if (haar) haarSon = haar->son(j, i);
                for (k = 0; k < i; ++k) {
                    blcluster* son1 = bl->getson(j, k), * son2 = bl->getson(k, i);
                    mltaGeHGeH((T)-1.0, son1, L, son2, U, sonL, A, eps, rankmax, haarSon);
                }
                GeHUtH_solve(son, U, sonL, A, L, eps, rankmax, haarSon);
                if (haarSon)
                    delete haarSon;
            }
        }
    }
    return true;
}



// Derived class for right LU preconditioner on an existing H-matrix
struct GeHMatrixLUPrecond : public GeHMatrix<double> {
    mblock<double>** L;       // Lower H-matrix factor
    mblock<double>** U;       // Upper H-matrix factor
    blcluster* blAggl;         // Block cluster tree for LU
    double* xf;               // Temporary vector for substitutions

    // Constructor from existing H-matrix and LU factors
    GeHMatrixLUPrecond(unsigned n, blcluster* tree = NULL)
        : GeHMatrix<double>(n, n, tree)
        , xf(new double[n])  
        , L(nullptr)
        , U(nullptr)
    {     
    }

    ~GeHMatrixLUPrecond() {
        delete[] xf;

        if(L)
            freembls(blAggl, L);
        
        if(U)
            freembls(blAggl, U);
    }

    // Apply right preconditioner C = inv(L*U)
    void precond_apply(double* x) const override {
        for (unsigned i = 0; i < this->n; i++) xf[i] = (double)x[i];
        HLU_solve(blAggl, L, U, xf);
        for (unsigned i = 0; i < this->n; i++) x[i] = xf[i];
    }
};




unsigned myGMRes(const Matrix<double>& A, double* const b, double* const x,
    double& eps, const unsigned m, unsigned& nsteps, double* m_CurrentAdv);



unsigned myBiCGStab(const Matrix<double>& A, double* const b, double* const x,
    double& eps, unsigned& nsteps, double* m_CurrentAdv);
