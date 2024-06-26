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

static void __minpack_lmstr1_callback(int *m, int *n, double *x, double *fvec, double *fjrow, int *iflag)
{
    ModifiedIntParam *modifiedM = (ModifiedIntParam *)(void *)m;
    minpackex_lmstr1_callback userCallback = (minpackex_lmstr1_callback)(modifiedM->callback);
    userCallback(modifiedM->userdata, modifiedM->param, *n, x, fvec, fjrow, iflag);
}

MINPACKEX_API
void minpackex_lmstr1(
    void *userdata,
    minpackex_lmstr1_callback callback,
    int m, int n, double *x, double *fvec, double *fjac, int ldfjac,
    double tol, int *info, int *ipvt, double *wa, int lwa)
{
    ModifiedIntParam m_param;
    m_param.param = m;
    m_param.userdata = userdata;
    m_param.callback = (void *)callback;

    lmstr1_(
        &__minpack_lmstr1_callback,
        (int *)(&m_param), &n, x, fvec, fjac, &ldfjac,
        &tol, info, ipvt, wa, &lwa);
}