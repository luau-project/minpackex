/*
** This test program was adapted
** from debian science team
** repository for minpack
** https://salsa.debian.org/science-team/minpack/-/blob/354dafc0ac489dc89f3347fe5b4b6fc0a71837db/debian/thybrd1.c
*/

/*     driver for hybrd1 example. */

#include <stdio.h>
#include <math.h>
#include "minpackex.h"
#include "thybrd1-minpackex.h"

void minpackex_hybrd1_fcn(void *userdata, int n, const double *x, double *fvec, int *iflag)
{
    /* function minpackex_hybrd1_fcn for hybrd1 example */

    int k;
    double one = 1, temp, temp1, temp2, three = 3, two = 2, zero = 0;

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

void thybrd1_minpackex_write_content(FILE *file)
{
    int j, n, info, lwa;
    double tol, fnorm;
    double x[9], fvec[9], wa[180];
    int one = 1;

    n = 9;

    /*      the following starting values provide a rough solution. */

    for (j = 1; j <= 9; j++)
    {
        x[j - 1] = -1.;
    }

    lwa = 180;

    /*      set tol to the square root of the machine precision. */
    /*      unless high solutions are required, */
    /*      this is the recommended setting. */

    tol = sqrt(minpackex_dpmpar(one));
    minpackex_hybrd1((void *)0, &minpackex_hybrd1_fcn, n, x, fvec, tol, &info, wa, lwa);
    fnorm = minpackex_enorm(n, fvec);

    fprintf(file, "     final L2 norm of the residuals %15.7g\n", fnorm);
    fprintf(file, "     exit parameter                 %10i\n", info);
    fprintf(file, "     final approximate solution\n");
    for (j = 1; j <= n; j++)
        fprintf(file, "%s%15.7g", j % 3 == 1 ? "\n     " : "", x[j - 1]);
    fprintf(file, "\n");
}