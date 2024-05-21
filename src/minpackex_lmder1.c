#include <minpack.h>
#include "minpackex.h"
#include "minpackex_param.h"

static void __minpack_lmder1_callback(int *m, int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag)
{
    ModifiedIntParam *modifiedM = (ModifiedIntParam *)(void *)m;
    minpackex_lmder1_callback userCallback = (minpackex_lmder1_callback)(modifiedM->callback);
    userCallback(modifiedM->userdata, modifiedM->param, *n, x, fvec, fjac, *ldfjac, iflag);
}

MINPACKEX_API
void minpackex_lmder1(
    void *userdata,
    minpackex_lmder1_callback callback,
    int m, int n, double *x, double *fvec,
    double *fjac, int ldfjac, double tol,
    int *info, int *ipvt, double *wa, int lwa)
{
    ModifiedIntParam m_param;
    m_param.param = m;
    m_param.userdata = userdata;
    m_param.callback = (void *)callback;

    lmder1_(__minpack_lmder1_callback, (int *)(&m_param), &n, x, fvec, fjac, &ldfjac, &tol, info, ipvt, wa, &lwa);
}