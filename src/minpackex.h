#ifndef __MINPACKEX_H__
#define __MINPACKEX_H__

#include <minpack.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MINPACKEX_EXPORT
#   if defined(BUILD_SHARED)
#       if defined(_WIN32) || defined(_WIN64)
#           if defined(__CYGWIN__) || defined(__MINGW32__)
#               define MINPACKEX_EXPORT __attribute__((__visibility__("default")))
#           else
#               define MINPACKEX_EXPORT __declspec(dllexport)
#           endif
#       else
#           define MINPACKEX_EXPORT __attribute__((__visibility__("default")))
#       endif
#   else
#       define MINPACKEX_EXPORT __attribute__((__visibility__("default")))
#   endif
#endif

#if BUILDING_MINPACKEX
#   define MINPACKEX_API MINPACKEX_EXPORT
#else
#   define MINPACKEX_API
#endif

//
// minpackex_enorm
//

MINPACKEX_API
double minpackex_enorm(int n, double *x);

//
// minpackex_dpmpar
//

MINPACKEX_API
double minpackex_dpmpar(int i);

//
// minpackex_lmdif1
//

/* callback to minpackex_lmdif1 function */

typedef void (*minpackex_lmdif1_callback)(void *userdata, int m, int n, double *x, double *fvec, int *iflag);

/* minimize the sum of the squares of nonlinear functions in N
   variables by a modification of the Levenberg-Marquardt algorithm
   (Jacobian calculated by a forward-difference approximation) */

MINPACKEX_API
void minpackex_lmdif1(
    void *userdata,
    minpackex_lmdif1_callback callback,
    int m, int n, double *x, double *fvec, double tol,
    int *info, int *iwa, double *wa, int lwa);

#ifdef __cplusplus
}
#endif

#endif