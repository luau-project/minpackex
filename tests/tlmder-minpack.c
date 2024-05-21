/*
** This test program was mostly taken
** from debian science team
** repository for minpack
** https://salsa.debian.org/science-team/minpack/-/blob/354dafc0ac489dc89f3347fe5b4b6fc0a71837db/debian/tlmder.c
*/

/*     driver for lmder example. */

#include <stdio.h>
#include <math.h>
#include <minpack.h>
#include "tlmder-minpack.h"

void minpack_lmder_fcn(int *m, int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag)
{
    /*      subroutine fcn for lmder example. */

    int i;
    double tmp1, tmp2, tmp3, tmp4;
    double y[15] = {1.4e-1, 1.8e-1, 2.2e-1, 2.5e-1, 2.9e-1, 3.2e-1, 3.5e-1,
                    3.9e-1, 3.7e-1, 5.8e-1, 7.3e-1, 9.6e-1, 1.34, 2.1, 4.39};

    if (*iflag == 0)
    {
        /*      insert print statements here when nprint is positive. */
        return;
    }

    if (*iflag != 2)
    {
        for (i = 1; i <= 15; i++)
        {
            tmp1 = i;
            tmp2 = 16 - i;
            tmp3 = tmp1;
            if (i > 8)
                tmp3 = tmp2;
            fvec[i - 1] = y[i - 1] - (x[1 - 1] + tmp1 / (x[2 - 1] * tmp2 + x[3 - 1] * tmp3));
        }
    }
    else
    {
        for (i = 1; i <= 15; i++)
        {
            tmp1 = i;
            tmp2 = 16 - i;
            tmp3 = tmp1;
            if (i > 8)
                tmp3 = tmp2;
            tmp4 = (x[2 - 1] * tmp2 + x[3 - 1] * tmp3);
            tmp4 = tmp4 * tmp4;
            fjac[i - 1 + *ldfjac * (1 - 1)] = -1.;
            fjac[i - 1 + *ldfjac * (2 - 1)] = tmp1 * tmp2 / tmp4;
            fjac[i - 1 + *ldfjac * (3 - 1)] = tmp1 * tmp3 / tmp4;
        };
    }
    return;
}

void tlmder_minpack_write_content(FILE *file)
{
    int j, m, n, ldfjac, maxfev, mode, nprint, info, nfev, njev;
    int ipvt[3];
    double ftol, xtol, gtol, factor, fnorm;
    double x[3], fvec[15], fjac[15 * 3], diag[3], qtf[3],
        wa1[3], wa2[3], wa3[3], wa4[15];
    int one = 1;

    m = 15;
    n = 3;

    /*      the following starting values provide a rough fit. */

    x[1 - 1] = 1.;
    x[2 - 1] = 1.;
    x[3 - 1] = 1.;

    ldfjac = 15;

    /*      set ftol and xtol to the square root of the machine */
    /*      and gtol to zero. unless high solutions are */
    /*      required, these are the recommended settings. */

    ftol = sqrt(dpmpar_(&one));
    xtol = sqrt(dpmpar_(&one));
    gtol = 0.;

    maxfev = 400;
    mode = 1;
    factor = 1.e2;
    nprint = 0;

    lmder_(&minpack_lmder_fcn, &m, &n, x, fvec, fjac, &ldfjac, &ftol, &xtol, &gtol,
           &maxfev, diag, &mode, &factor, &nprint, &info, &nfev, &njev,
           ipvt, qtf, wa1, wa2, wa3, wa4);
    fnorm = enorm_(&m, fvec);
    fprintf(file, "      final l2 norm of the residuals%15.7g\n\n", fnorm);
    fprintf(file, "      number of function evaluations%10i\n\n", nfev);
    fprintf(file, "      number of Jacobian evaluations%10i\n\n", njev);
    fprintf(file, "      exit parameter                %10i\n\n", info);
    fprintf(file, "      final approximate solution\n");
    for (j = 1; j <= n; j++)
        fprintf(file, "%s%15.7g", j % 3 == 1 ? "\n     " : "", x[j - 1]);
    fprintf(file, "\n");
}