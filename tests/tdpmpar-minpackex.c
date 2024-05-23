/*     driver for dpmpar example. */

#include <stdio.h>
#include <math.h>
#include "minpackex.h"
#include "tdpmpar-minpack.h"

void tdpmpar_minpackex_write_content(FILE *file)
{
    double dpmpar_1, dpmpar_2, dpmpar_3;
    int one = 1, two = 2, three = 3;

    dpmpar_1 = minpackex_dpmpar(one);
    dpmpar_2 = minpackex_dpmpar(two);
    dpmpar_3 = minpackex_dpmpar(three);

    fprintf(file, "      dpmpar(1)%15.7g\n\n", dpmpar_1);
    fprintf(file, "      dpmpar(2)%15.7g\n\n", dpmpar_2);
    fprintf(file, "      dpmpar(3)%15.7g\n\n", dpmpar_3);
    fprintf(file, "\n");
}