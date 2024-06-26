/*
** This test program was mostly taken
** from debian science team
** repository for minpack
** https://salsa.debian.org/science-team/minpack/-/blob/354dafc0ac489dc89f3347fe5b4b6fc0a71837db/debian/tlmder1.c
*/

/*     driver for lmder1 example. */

#include <stdio.h>
#include <math.h>
#include <minpack.h>
#include "tlmder1-minpack.h"

void minpack_lmder1_fcn(int *m, int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag)
{
    /*      subroutine fcn for lmder1 example. */

    int i;
    double tmp1, tmp2, tmp3, tmp4;
    double y[15] = {1.4e-1, 1.8e-1, 2.2e-1, 2.5e-1, 2.9e-1, 3.2e-1, 3.5e-1,
                    3.9e-1, 3.7e-1, 5.8e-1, 7.3e-1, 9.6e-1, 1.34, 2.1, 4.39};

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
        }
    }
}

void tlmder1_minpack_write_content(FILE *file)
{
    int j, m, n, ldfjac, info, lwa;
    int ipvt[3];
    double tol, fnorm;
    double x[3], fvec[15], fjac[15 * 3], wa[30];
    int one = 1;

    m = 15;
    n = 3;

    /*      the following starting values provide a rough fit. */

    x[1 - 1] = 1.;
    x[2 - 1] = 1.;
    x[3 - 1] = 1.;

    ldfjac = 15;
    lwa = 30;

    /*      set tol to the square root of the machine precision. */
    /*      unless high solutions are required, */
    /*      this is the recommended setting. */

    tol = sqrt(dpmpar_(&one));

    lmder1_(&minpack_lmder1_fcn, &m, &n, x, fvec, fjac, &ldfjac, &tol,
            &info, ipvt, wa, &lwa);
    fnorm = enorm_(&m, fvec);
    fprintf(file, "      final l2 norm of the residuals%15.7g\n\n", fnorm);
    fprintf(file, "      exit parameter                %10i\n\n", info);
    fprintf(file, "      final approximate solution\n");
    for (j = 1; j <= n; j++)
        fprintf(file, "%s%15.7g", j % 3 == 1 ? "\n     " : "", x[j - 1]);
    fprintf(file, "\n");
}