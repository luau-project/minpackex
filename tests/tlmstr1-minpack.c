/*
** This test program was mostly taken
** from debian science team
** repository for minpack
** https://salsa.debian.org/science-team/minpack/-/blob/354dafc0ac489dc89f3347fe5b4b6fc0a71837db/debian/tlmstr1.c
*/

/*     driver for lmstr1 example. */

#include <stdio.h>
#include <math.h>
#include <minpack.h>
#include "tlmstr1-minpack.h"

void minpack_lmstr1_fcn(int *m, int *n, double *x, double *fvec, double *fjrow, int *iflag)
{
    /* function minpack_lmstr1_fcn for lmstr1 example */
    int i;
    double tmp1, tmp2, tmp3, tmp4;
    double y[15] = {1.4e-1, 1.8e-1, 2.2e-1, 2.5e-1, 2.9e-1, 3.2e-1, 3.5e-1,
                    3.9e-1, 3.7e-1, 5.8e-1, 7.3e-1, 9.6e-1, 1.34, 2.1, 4.39};

    if (*iflag < 2)
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
        i = *iflag - 1;
        tmp1 = i;
        tmp2 = 16 - i;
        tmp3 = tmp1;
        if (i > 8)
            tmp3 = tmp2;
        tmp4 = (x[2 - 1] * tmp2 + x[3 - 1] * tmp3);
        tmp4 = tmp4 * tmp4;
        fjrow[1 - 1] = -1;
        fjrow[2 - 1] = tmp1 * tmp2 / tmp4;
        fjrow[3 - 1] = tmp1 * tmp3 / tmp4;
    }
}

void tlmstr1_minpack_write_content(FILE *file)
{
    int m, n, ldfjac, info, lwa, ipvt[3], one = 1;
    double tol, fnorm;
    double x[3], fvec[15], fjac[9], wa[30];

    m = 15;
    n = 3;

    /*     the following starting values provide a rough fit. */

    x[1] = 1.;
    x[2] = 1.;
    x[3] = 1.;

    ldfjac = 3;
    lwa = 30;

    /*     set tol to the square root of the machine precision.
       unless high precision solutions are required,
       this is the recommended setting. */

    tol = sqrt(dpmpar_(&one));

    lmstr1_(&minpack_lmstr1_fcn, &m, &n,
            x, fvec, fjac, &ldfjac,
            &tol, &info, ipvt, wa, &lwa);

    fnorm = enorm_(&m, fvec);

    fprintf(file, "      FINAL L2 NORM OF THE RESIDUALS%15.7g\n\n", fnorm);
    fprintf(file, "      EXIT PARAMETER                %10i\n\n", info);
    fprintf(file, "      FINAL APPROXIMATE SOLUTION\n\n%15.7g%15.7g%15.7g\n",
            x[0], x[1], x[2]);
}