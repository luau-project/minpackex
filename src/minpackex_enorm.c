#include <minpack.h>
#include "minpackex.h"

MINPACKEX_API
double minpackex_enorm(int n, double *x)
{
    return enorm_(&n, x);
}