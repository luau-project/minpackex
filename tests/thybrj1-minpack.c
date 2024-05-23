/*
** This test program was mostly taken
** from debian science team
** repository for minpack
** https://salsa.debian.org/science-team/minpack/-/blob/354dafc0ac489dc89f3347fe5b4b6fc0a71837db/debian/thybrj1.c
*/

/*     driver for hybrj1 example. */

#include <stdio.h>
#include <math.h>
#include <minpack.h>
#include "thybrj1-minpack.h"

void minpack_hybrj1_fcn(int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag)
{
    /* function minpack_hybrj1_fcn for hybrj1 example */

    int j, k;
    double one = 1, temp, temp1, temp2, three = 3, two = 2, zero = 0, four = 4;

    if (*iflag != 2)
    {
        for (k = 1; k <= *n; k++)
        {
            temp = (three - two * x[k - 1]) * x[k - 1];
            temp1 = zero;
            if (k != 1)
                temp1 = x[k - 1 - 1];
            temp2 = zero;
            if (k != *n)
                temp2 = x[k + 1 - 1];
            fvec[k - 1] = temp - temp1 - two * temp2 + one;
        }
    }
    else
    {
        for (k = 1; k <= *n; k++)
        {
            for (j = 1; j <= *n; j++)
            {
                fjac[k - 1 + *ldfjac * (j - 1)] = zero;
            }
            fjac[k - 1 + *ldfjac * (k - 1)] = three - four * x[k - 1];
            if (k != 1)
                fjac[k - 1 + *ldfjac * (k - 1 - 1)] = -one;
            if (k != *n)
                fjac[k - 1 + *ldfjac * (k + 1 - 1)] = -two;
        }
    }
}

void thybrj1_minpack_write_content(FILE *file)
{
    int j, n, ldfjac, info, lwa;
    double tol, fnorm;
    double x[9], fvec[9], fjac[9 * 9], wa[99];
    int one = 1;

    n = 9;

    /*      the following starting values provide a rough solution. */

    for (j = 1; j <= 9; j++)
    {
        x[j - 1] = -1.;
    }

    ldfjac = 9;
    lwa = 99;

    /*      set tol to the square root of the machine precision. */
    /*      unless high solutions are required, */
    /*      this is the recommended setting. */

    tol = sqrt(dpmpar_(&one));

    hybrj1_(&minpack_hybrj1_fcn, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

    fnorm = enorm_(&n, fvec);

    fprintf(file, "      final l2 norm of the residuals%15.7g\n\n", fnorm);
    fprintf(file, "      exit parameter                %10i\n\n", info);
    fprintf(file, "      final approximate solution\n");
    for (j = 1; j <= n; j++)
        fprintf(file, "%s%15.7g", j % 3 == 1 ? "\n     " : "", x[j - 1]);
    fprintf(file, "\n");
}