/*     driver for enorm example. */

#include <stdio.h>
#include <math.h>
#include <minpack.h>
#include "tenorm-minpack.h"

void tenorm_minpack_write_content(FILE *file)
{
    int j, n;
    double tol, vec_norm, expected_norm, dist;
    double x[2] = {3.0, 4.0};
    int one = 1;

    n = 2;
    expected_norm = 5.0;

    /*      set tol to the square root of the machine precision. */
    /*      unless high solutions are required, */
    /*      this is the recommended setting. */

    tol = sqrt(dpmpar_(&one));
    
    vec_norm = enorm_(&n, x);

    dist = fabs(vec_norm - expected_norm);

    fprintf(file, "      l2 norm of the vector%15.7g\n\n", vec_norm);
    fprintf(file, "      expected norm%15.7g\n\n", expected_norm);
    fprintf(file, "      final distance of l2 norm of the vector and expected norm%15.7g\n\n", dist);
    fprintf(file, "\n");
}