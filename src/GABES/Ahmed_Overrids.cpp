#include "Ahmed_Overrides.h"




static void genPlRot(double dx, double dy, double& cs, double& sn)
{
    if (dy == 0.0) {
        cs = 1.0;
        sn = 0.0;
    }
    else if (fabs(dy) > fabs(dx)) {
        double tmp = dx / dy;
        sn = 1.0 / sqrt(1.0 + tmp * tmp);
        cs = tmp * sn;
    }
    else {
        double tmp = dy / dx;
        cs = 1.0 / sqrt(1.0 + tmp * tmp);
        sn = tmp * cs;
    }
}


inline void applPlRot(double& dx, double& dy, double cs, double sn)
{
    double tmp = cs * dx + sn * dy;
    dy = cs * dy - sn * dx;
    dx = tmp;
}


// solve H y = s and update x += MVy
static void update(const Matrix<double>& A, unsigned k, double* H,
    unsigned ldH, double* s, double* V, double* x)
{
    double* y = new double[k];
    double* xh = new double[A.n];
    blas::copy(k, s, y);
    int inf;

    dtrtrs_(JOB_STR + 5, JOB_STR, JOB_STR, &k, &N_ONE, H, &ldH, y, &k, &inf);
    assert(inf == 0);

    // x += M V y
    blas::setzero(A.n, xh);
    blas::gemva(A.n, k, D_ONE, V, y, xh);
    A.precond_apply(xh);
    blas::add(A.n, xh, x);

    delete[] xh;
    delete[] y;
}



unsigned myGMRes(const Matrix<double>& A, double* const b, double* const x,
    double& eps, const unsigned m, unsigned& nsteps, double* m_CurrentAdv)
{
    double resid;
    unsigned j = 1;

    double* r = new double[2 * A.n + (A.n + m + 4) * (m + 1)];   // n
    double* V = r + A.n;                         // n x (m+1)
    double* H = V + A.n * (m + 1);                   // m+1 x m
    double* cs = H + (m + 1) * m;                  // m+1
    double* sn = cs + m + 1;                     // m+1
    double* s = sn + m + 1;                      // m+1
    double* xh = s + m + 1;                      // m+1

    // normb = norm(b)
    double normb = blas::nrm2(A.n, b);
    if (normb == 0.0) {
        blas::setzero(A.n, x);
        eps = 0.0;
        nsteps = 0;
        delete[] r;
        *m_CurrentAdv = 1.0;
        return 0;
    }

    // r = b - Ax
    blas::copy(A.n, b, r);
    A.amux(D_MONE, x, r);

    double beta = blas::nrm2(A.n, r);

    if ((resid = beta / normb) <= eps) {
        eps = resid;
        nsteps = 0;
        delete[] r;
        *m_CurrentAdv = 1.0;
        return 0;
    }

    while (j <= nsteps) {

        *m_CurrentAdv = (double)j / (double)nsteps;

        blas::copy(A.n, r, V);                // v0 first orthonormal vector
        blas::scal(A.n, 1.0 / beta, V);

        s[0] = beta;
        blas::setzero(m, s + 1);

        for (unsigned i = 0; i < m && j <= nsteps; i++, j++) {

            *m_CurrentAdv = (double)j / (double)nsteps;
            // w = A M * v[i];
            blas::copy(A.n, V + i * A.n, xh);
            A.precond_apply(xh);
            blas::setzero(A.n, V + (i + 1) * A.n);
            A.amux(D_ONE, xh, V + (i + 1) * A.n);

            for (unsigned k = 0; k <= i; k++) {
                H[k + i * (m + 1)] = blas::scpr(A.n, V + (i + 1) * A.n, V + k * A.n);
                blas::axpy(A.n, -H[k + i * (m + 1)], V + k * A.n, V + (i + 1) * A.n);
            }

            H[i * (m + 2) + 1] = blas::nrm2(A.n, V + (i + 1) * A.n);
            blas::scal(A.n, 1.0 / H[i * (m + 2) + 1], V + (i + 1) * A.n);

            // apply old Givens rotations to the last column in H
            for (unsigned k = 0; k < i; k++)
                applPlRot(H[k + i * (m + 1)], H[k + 1 + i * (m + 1)], cs[k], sn[k]);

            // generate new Givens rotation which eleminates H[i*(m+2)+1]
            genPlRot(H[i * (m + 2)], H[i * (m + 2) + 1], cs[i], sn[i]);

            // apply it to H and s
            applPlRot(H[i * (m + 2)], H[i * (m + 2) + 1], cs[i], sn[i]);
            applPlRot(s[i], s[i + 1], cs[i], sn[i]);

            if ((resid = fabs(s[i + 1] / normb)) < eps) {
                update(A, i + 1, H, m + 1, s, V, x);
                eps = resid;
                nsteps = j;
                delete[] r;
                *m_CurrentAdv = 1.0;
                return 0;
            }
#ifndef NDEBUG
            std::cout << "Step " << j << ", resid=" << resid << std::endl;
#endif
        }

        update(A, m, H, m + 1, s, V, x);

        // r = b - A x;
        blas::copy(A.n, b, r);
        A.amux(D_MONE, x, r);
        beta = blas::nrm2(A.n, r);

        if ((resid = beta / normb) < eps) {
            eps = resid;
            nsteps = j;
            delete[] r;
            *m_CurrentAdv = 1.0;
            return 0;
        }
    }

    eps = resid;
    delete[] r;
    *m_CurrentAdv = 1.0;
    return 1;
}




unsigned myBiCGStab(const Matrix<double>& A, double* const b, double* const x,
    double& eps, unsigned& nsteps, double* m_CurrentAdv)
{
    double* v, * p, * phat, * s, * shat, * t, * r, * r0;
    unsigned N = A.n;

    v = new double[8 * N];
    p = v + N;
    phat = p + N;
    s = phat + N;
    shat = s + N;

    t = shat + N;
    r = t + N;
    r0 = r + N;

    double resid;

    // normb = |b|
    double nrmb = blas::nrm2(N, b);
    if (nrmb < D_PREC) nrmb = D_ONE;

    // r = r0 = b - A x0
    blas::copy(N, b, r0);
    A.amux(D_MONE, x, r0);
    blas::copy(N, r0, r);

    resid = blas::nrm2(N, r) / nrmb;

    if (resid < eps) {
        eps = resid;
        nsteps = 0;
        delete[] v;
        *m_CurrentAdv = 1.0;
        return 0;
    }

    double alpha = D_ZERO, omega = D_ZERO, rho2 = D_ZERO;

    for (unsigned l = 1; l <= nsteps; ++l) {

        *m_CurrentAdv = (double)l / (double)nsteps;

        // rho1 = r0 * r
        const double rho1 = blas::scpr(N, r0, r);
        if (fabs(rho1) < D_PREC) {
            eps = blas::nrm2(N, r) / nrmb;
            delete[] v;
            *m_CurrentAdv = 1.0;
            return 2;
        }

        if (l == 1)
            blas::copy(N, r, p);                // p = r
        else {
            blas::axpy(N, -omega, v, p);        // p = (p-omega v)*beta+r
            const double beta = rho1 * alpha / (rho2 * omega);
            blas::scal(N, beta, p);
            blas::axpy(N, D_ONE, r, p);
        }

        // p^ = C p
        blas::copy(N, p, phat);
        A.precond_apply(phat);

        // v = A p^
        blas::setzero(N, v);
        A.amux(D_ONE, phat, v);

        alpha = rho1 / blas::scpr(N, r0, v);

        // s = r - alpha v
        blas::copy(N, r, s);
        blas::axpy(N, -alpha, v, s);

        resid = blas::nrm2(N, s) / nrmb;
#ifndef NDEBUG
        std::cout << "Step " << l << ", resid=" << resid << std::endl;
#endif
        if (resid < eps) {
            // x += alpha p^
            blas::axpy(N, alpha, phat, x);
            eps = resid;
            nsteps = l;
            delete[] v;
            *m_CurrentAdv = 1.0;
            return 0;
        }

        // s^ = C s
        blas::copy(N, s, shat);
        A.precond_apply(shat);

        // t = A s^
        blas::setzero(N, t);
        A.amux(D_ONE, shat, t);

        // omega = t*s / t*t
        omega = blas::scpr(N, t, s) / blas::scpr(N, t, t);

        // x += alpha p^ + omega s^
        blas::axpy(N, alpha, phat, x);
        blas::axpy(N, omega, shat, x);

        // r = s - omega t
        blas::copy(N, s, r);
        blas::axpy(N, -omega, t, r);

        rho2 = rho1;

        resid = blas::nrm2(N, r) / nrmb;

        if (resid < eps) {
            eps = resid;
            nsteps = l;
            delete[] v;
            *m_CurrentAdv = 1.0;
            return 0;
        }

        if (fabs(omega) < D_PREC) {
            eps = resid;
            delete[] v;
            *m_CurrentAdv = 1.0;
            return 3;
        }
    }

    eps = resid;
    delete[] v;
    *m_CurrentAdv = 1.0;
    return 1;
}