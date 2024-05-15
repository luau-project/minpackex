#ifndef __MINPACKEX_H__
#define __MINPACKEX_H__

#include <minpack.h>

#ifdef __cplusplus
extern "C" {
#endif

#if HAVE_VISIBILITY && BUILDING_LIBMINPACKEX
#   define LIBMINPACKEX_SHLIB_EXPORTED __attribute__((__visibility__("default")))
#elif (defined _WIN32 && !defined __CYGWIN__ && !defined __MINGW32__) && BUILDING_SHARED && BUILDING_LIBMINPACKEX
#   if defined DLL_EXPORT
#       define LIBMINPACKEX_SHLIB_EXPORTED __declspec(dllexport)
#   else
#       define LIBMINPACKEX_SHLIB_EXPORTED
#   endif
#elif (defined _WIN32 && !defined __CYGWIN__ && !defined __MINGW32__) && BUILDING_SHARED
#   define LIBMINPACKEX_SHLIB_EXPORTED __declspec(dllimport)
#else
#   define LIBMINPACKEX_SHLIB_EXPORTED
#endif

#define MINPACKEX_API LIBMINPACKEX_SHLIB_EXPORTED

#define MINPACKEX_VERSION "0.1.0"

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

typedef void (*minpackex_lmdif1_callback)(void *userdata, int m, int n, const double *x, double *fvec, int *iflag);

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