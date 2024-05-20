#include "minpackex.h"
#include "minpackex_param.h"

static void __minpack_lmdif1_callback(int *m, int *n, double *x, double *fvec, int *iflag)
{
    ModifiedIntParam *modifiedM = (ModifiedIntParam *)(void *)m;
    minpackex_lmdif1_callback userCallback = (minpackex_lmdif1_callback)(modifiedM->callback);
    userCallback(modifiedM->userdata, modifiedM->param, *n, x, fvec, iflag);
}

MINPACKEX_API
void minpackex_lmdif1(
    void *userdata,
    minpackex_lmdif1_callback callback,
    int m, int n, double *x, double *fvec, double tol,
    int *info, int *iwa, double *wa, int lwa)
{
    ModifiedIntParam m_param;
    m_param.param = m;
    m_param.userdata = userdata;
    m_param.callback = (void *)callback;

    lmdif1_(__minpack_lmdif1_callback, (int *)(&m_param), &n, x, fvec, &tol, info, iwa, wa, &lwa);
}