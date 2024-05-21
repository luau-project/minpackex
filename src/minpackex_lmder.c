#include <minpack.h>
#include "minpackex.h"
#include "minpackex_param.h"

static void __minpack_lmder_callback(int *m, int *n, double *x, double *fvec, double *fjac, int *ldfjac, int *iflag)
{
    ModifiedIntParam *modifiedM = (ModifiedIntParam *)(void *)m;
    minpackex_lmder_callback userCallback = (minpackex_lmder_callback)(modifiedM->callback);
    userCallback(modifiedM->userdata, modifiedM->param, *n, x, fvec, fjac, *ldfjac, iflag);
}

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
    double *wa4)
{
    ModifiedIntParam m_param;
    m_param.param = m;
    m_param.userdata = userdata;
    m_param.callback = (void *)callback;

    lmder_(
        __minpack_lmder_callback,
        (int *)(&m_param), &n, x, fvec,
        fjac, &ldfjac, &ftol,
        &xtol, &gtol, &maxfev,
        diag, &mode, &factor, &nprint,
        info, nfev, njev, ipvt, qtf,
        wa1, wa2, wa3,
        wa4);
}