/*
** This test program was adapted
** from debian science team
** repository for minpack
** https://salsa.debian.org/science-team/minpack/-/blob/354dafc0ac489dc89f3347fe5b4b6fc0a71837db/debian/thybrd.c
*/

/*     driver for hybrd example. */

#include <stdio.h>
#include <math.h>
#include "minpackex.h"
#include "thybrd-minpackex.h"

void minpackex_hybrd_fcn(void *userdata, int n, const double *x, double *fvec, int *iflag)
{
    /* function minpackex_hybrd_fcn for hybrd example */

    int k;
    double one = 1, temp, temp1, temp2, three = 3, two = 2, zero = 0;

    if (iflag == 0)
    {
        /*      insert print statements here when nprint is positive. */
        return;
    }
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
    return;
}

void thybrd_minpackex_write_content(FILE *file)
{
    int j, n, maxfev, ml, mu, mode, nprint, info, nfev, ldfjac, lr;
    double xtol, epsfcn, factor, fnorm;
    double x[9], fvec[9], diag[9], fjac[9 * 9], r[45], qtf[9],
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

    maxfev = 2000;
    ml = 1;
    mu = 1;
    epsfcn = 0.;
    mode = 2;
    for (j = 1; j <= 9; j++)
    {
        diag[j - 1] = 1.;
    }

    factor = 1.e2;
    nprint = 0;

    minpackex_hybrd((void *)0, &minpackex_hybrd_fcn, n, x, fvec, xtol, maxfev, ml, mu, epsfcn,
           diag, mode, factor, nprint, &info, &nfev,
           fjac, ldfjac, r, lr, qtf, wa1, wa2, wa3, wa4);
    fnorm = minpackex_enorm(n, fvec);
    fprintf(file, "     final l2 norm of the residuals %15.7g\n\n", fnorm);
    fprintf(file, "     number of function evaluations  %10i\n\n", nfev);
    fprintf(file, "     exit parameter                  %10i\n\n", info);
    fprintf(file, "     final approximate solution\n");
    for (j = 1; j <= n; j++)
        fprintf(file, "%s%15.7g", j % 3 == 1 ? "\n     " : "", x[j - 1]);
    fprintf(file, "\n");
}