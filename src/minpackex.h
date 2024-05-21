#ifndef __MINPACKEX_H__
#define __MINPACKEX_H__

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


//
// minpackex_lmdif
// 

/* callback to minpackex_lmdif function */

typedef void (*minpackex_lmdif_callback)(void *userdata, int m, int n, const double *x, double *fvec, int *iflag);

/* minimize the sum of the squares of nonlinear functions in N
   variables by a modification of the Levenberg-Marquardt algorithm
   (Jacobian calculated by a forward-difference approximation, more
   general) */

MINPACKEX_API
void minpackex_lmdif(
    void * userdata,
    minpackex_lmdif_callback callback,
    int m, int n, double *x, double *fvec, double ftol,
    double xtol, double gtol, int maxfev, double epsfcn,
    double *diag, int mode, double factor, int nprint,
    int *info, int *nfev, double *fjac, int ldfjac, int *ipvt,
    double *qtf, double *wa1, double *wa2, double *wa3,
    double *wa4 );

//
// minpackex_lmder1
//

/* callback to minpackex_lmder1 function */

typedef void (*minpackex_lmder1_callback)(void *userdata, int m, int n, const double *x, double *fvec, double *fjac, int ldfjac, int *iflag);

/* minimize the sum of the squares of nonlinear functions in N
   variables by a modification of the Levenberg-Marquardt algorithm
   (user-supplied Jacobian) */

MINPACKEX_API
void minpackex_lmder1(
    void *userdata,
    minpackex_lmder1_callback callback,
    int m, int n, double *x, double *fvec,
    double *fjac, int ldfjac, double tol,
    int *info, int *ipvt, double *wa, int lwa);

#ifdef __cplusplus
}
#endif

#endif