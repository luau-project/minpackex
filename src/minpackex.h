//                                                                                 //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////
// License                                                                         //
/////////////////////////////////////////////////////////////////////////////////////
//                                                                                 //
// Copyright (c) 2024 luau-project                                                 //
//                    https://github.com/luau-project/minpackex                    //
//                                                                                 //
// Redistribution and use in source and binary forms, with or without              //
// modification, are permitted provided that the following conditions are met:     //
//                                                                                 //
// 1. Redistributions of source code must retain the above copyright notice,       //
// this list of conditions and the following disclaimer.                           //
//                                                                                 //
// 2. Redistributions in binary form must reproduce the above copyright            //
// notice, this list of conditions and the following disclaimer in the             //
// documentation and/or other materials provided with the distribution.            //
//                                                                                 //
// 3. Neither the name of the copyright holder nor the names of its                //
// contributors may be used to endorse or promote products derived from this       //
// software without specific prior written permission.                             //
//                                                                                 //
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”     //
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,           //
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR          //
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR               //
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,           //
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,             //
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;     //
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,        //
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR         //
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF          //
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                      //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
////                                 Body                                        ////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//                                                                                 //
//                                                                                 //

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
// minpackex_hybrd1
//

/* callback to minpackex_hybrd1 function */

typedef void (*minpackex_hybrd1_callback)(void *userdata, int n, const double *x, double *fvec, int *iflag);

/* find a zero of a system of N nonlinear functions in N variables by
   a modification of the Powell hybrid method (Jacobian calculated by
   a forward-difference approximation) */

MINPACKEX_API
void minpackex_hybrd1(
    void *userdata,
    minpackex_hybrd1_callback callback,
    int n, double *x, double *fvec, double tol, int *info,
    double *wa, int lwa);

//
// minpackex_hybrd
//

/* callback to minpackex_hybrd function */

typedef void (*minpackex_hybrd_callback)(void *userdata, int n, const double *x, double *fvec, int *iflag);

/* find a zero of a system of N nonlinear functions in N variables by
   a modification of the Powell hybrid method (Jacobian calculated by
   a forward-difference approximation, more general). */

MINPACKEX_API
void minpackex_hybrd(
    void *userdata,
    minpackex_hybrd_callback callback,
    int n, double *x, double *fvec, double xtol, int maxfev,
    int ml, int mu, double epsfcn, double *diag, int mode,
    double factor, int nprint, int *info, int *nfev,
    double *fjac, int ldfjac, double *r, int lr, double *qtf,
    double *wa1, double *wa2, double *wa3, double *wa4);

//
// minpackex_hybrj1
//

/* callback to minpackex_hybrj1 function */

typedef void (*minpackex_hybrj1_callback)(void *userdata, int n, const double *x, double *fvec, double *fjac, int ldfjac, int *iflag);

/* find a zero of a system of N nonlinear functions in N variables by
   a modification of the Powell hybrid method (Jacobian calculated by
   a forward-difference approximation) */

MINPACKEX_API
void minpackex_hybrj1(
    void *userdata,
    minpackex_hybrj1_callback callback,
    int n, double *x, double *fvec, double *fjac, int ldfjac, double tol,
    int *info, double *wa, int lwa);

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

//
// minpackex_lmder
//

/* callback to minpackex_lmder function */

typedef void (*minpackex_lmder_callback)(void *userdata, int m, int n, const double *x, double *fvec, double *fjac, int ldfjac, int *iflag);

/* minimize the sum of the squares of nonlinear functions in N
   variables by a modification of the Levenberg-Marquardt algorithm
   (user-supplied Jacobian, more general) */

MINPACKEX_API
void minpackex_lmder(
    void *userdata,
    minpackex_lmder_callback callback,
    int m, int n, double *x, double *fvec,
    double *fjac, int ldfjac, double ftol,
    double xtol, double gtol, int maxfev,
    double *diag, int mode, double factor, int nprint,
    int *info, int *nfev, int *njev, int *ipvt, double *qtf,
    double *wa1, double *wa2, double *wa3,
    double *wa4);

#ifdef __cplusplus
}
#endif

#endif