/*     driver for enorm example. */

#include <stdio.h>
#include <math.h>
#include "minpackex.h"
#include "tenorm-minpack.h"

void tenorm_minpackex_write_content(FILE *file)
{
    int n;
    double tol, vec_norm, expected_norm, dist;
    double x[2] = {3.0, 4.0};
    int one = 1;

    n = 2;
    expected_norm = 5.0;

    /*      set tol to the square root of the machine precision. */
    /*      unless high solutions are required, */
    /*      this is the recommended setting. */

    tol = sqrt(minpackex_dpmpar(one));
    
    vec_norm = minpackex_enorm(n, x);

    dist = fabs(vec_norm - expected_norm);

    fprintf(file, "      l2 norm of the vector%15.7g\n\n", vec_norm);
    fprintf(file, "      expected norm%15.7g\n\n", expected_norm);
    fprintf(file, "      distance between l2 norm of the vector and expected norm%15.7g\n\n", dist);
    fprintf(file, "      tolerance%15.7g\n\n", tol);
    fprintf(file, "\n");
}