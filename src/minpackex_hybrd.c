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

#include <minpack.h>
#include "minpackex.h"
#include "minpackex_param.h"

static void __minpack_hybrd_callback(int *n, double *x, double *fvec, int *iflag)
{
    ModifiedIntParam *modifiedN = (ModifiedIntParam *)(void *)n;
    minpackex_hybrd_callback userCallback = (minpackex_hybrd_callback)(modifiedN->callback);
    userCallback(modifiedN->userdata, modifiedN->param, x, fvec, iflag);
}

MINPACKEX_API
void minpackex_hybrd(
    void *userdata,
    minpackex_hybrd_callback callback,
    int n, double *x, double *fvec, double xtol, int maxfev,
    int ml, int mu, double epsfcn, double *diag, int mode,
    double factor, int nprint, int *info, int *nfev,
    double *fjac, int ldfjac, double *r, int lr, double *qtf,
    double *wa1, double *wa2, double *wa3, double *wa4)
{
    ModifiedIntParam n_param;
    n_param.param = n;
    n_param.userdata = userdata;
    n_param.callback = (void *)callback;

    hybrd_(
        __minpack_hybrd_callback,
        (int *)(&n_param), x, fvec, &xtol, &maxfev,
        &ml, &mu, &epsfcn, diag, &mode,
        &factor, &nprint, info, nfev,
        fjac, &ldfjac, r, &lr, qtf,
        wa1, wa2, wa3, wa4);
}