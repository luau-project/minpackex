#include <minpack.h>
#include "minpackex.h"

MINPACKEX_API
double minpackex_dpmpar(int i)
{
    return dpmpar_(&i);
}