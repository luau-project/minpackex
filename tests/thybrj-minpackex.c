/*
** This test program was adapted
** from debian science team
** repository for minpack
** https://salsa.debian.org/science-team/minpack/-/blob/354dafc0ac489dc89f3347fe5b4b6fc0a71837db/debian/thybrj.c
*/

/*     driver for hybrj example. */

#include <stdio.h>
#include <math.h>
#include "minpackex.h"
#include "thybrj-minpackex.h"

void minpackex_hybrj_fcn(void *userdata, int n, const double *x, double *fvec, double *fjac, int ldfjac, int *iflag)
{
    /* function minpackex_hybrj_fcn for hybrj example */

    int j, k;
    double one = 1, temp, temp1, temp2, three = 3, two = 2, zero = 0, four = 4;

    if (iflag == 0)
    {
        /*      insert print statements here when nprint is positive. */
        return;
    }

    if (*iflag != 2)
    {
        for (k = 1; k <= n; k++)
        {
            temp = (three - two * x[k - 1]) * x[k - 1];
            temp1 = zero;
            if (k != 1)
                temp1 = x[k - 1 - 1];
            temp2 = zero;
            if (k != n)
                temp2 = x[k + 1 - 1];
            fvec[k - 1] = temp - temp1 - two * temp2 + one;
        }
    }
    else
    {
        for (k = 1; k <= n; k++)
        {
            for (j = 1; j <= n; j++)
            {
                fjac[k - 1 + ldfjac * (j - 1)] = zero;
            }
            fjac[k - 1 + ldfjac * (k - 1)] = three - four * x[k - 1];
            if (k != 1)
                fjac[k - 1 + ldfjac * (k - 1 - 1)] = -one;
            if (k != n)
                fjac[k - 1 + ldfjac * (k + 1 - 1)] = -two;
        }
    }
}

void thybrj_minpackex_write_content(FILE *file)
{
    int j, n, ldfjac, maxfev, mode, nprint, info, nfev, njev, lr;
    double xtol, factor, fnorm;
    double x[9], fvec[9], fjac[9 * 9], diag[9], r[45], qtf[9],
        wa1[9], wa2[9], wa3[9], wa4[9];
    int one = 1;

    n = 9;

    /*      the following starting values provide a rough solution. */

    for (j = 1; j <= 9; j++)
    {
        x[j - 1] = -1.;
    }

    ldfjac = 9;
    lr = 45;

    /*      set xtol to the square root of the machine precision. */
    /*      unless high solutions are required, */
    /*      this is the recommended setting. */

    xtol = sqrt(minpackex_dpmpar(one));

    maxfev = 1000;
    mode = 2;
    for (j = 1; j <= 9; j++)
    {
        diag[j - 1] = 1.;
    }
    factor = 1.e2;
    nprint = 0;

    minpackex_hybrj((void *) 0, &minpackex_hybrj_fcn, n, x, fvec, fjac, ldfjac, xtol, maxfev, diag,
           mode, factor, nprint, &info, &nfev, &njev, r, lr, qtf,
           wa1, wa2, wa3, wa4);
    fnorm = minpackex_enorm(n, fvec);

    fprintf(file, "     final l2 norm of the residuals%15.7g\n\n", fnorm);
    fprintf(file, "     number of function evaluations%10i\n\n", nfev);
    fprintf(file, "     number of jacobian evaluations%10i\n\n", njev);
    fprintf(file, "     exit parameter                %10i\n\n", info);
    fprintf(file, "     final approximate solution\n\n");
    for (j = 1; j <= n; j++)
        fprintf(file, "%s%15.7g", j % 3 == 1 ? "\n     " : "", x[j - 1]);
    fprintf(file, "\n");
}