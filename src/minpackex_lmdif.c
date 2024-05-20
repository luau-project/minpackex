#include "minpackex.h"
#include "minpackex_param.h"

static void __minpack_lmdif_callback(int *m, int *n, double *x, double *fvec, int *iflag)
{
    ModifiedIntParam *modifiedM = (ModifiedIntParam *)(void *)m;
    minpackex_lmdif_callback userCallback = (minpackex_lmdif_callback)(modifiedM->callback);
    userCallback(modifiedM->userdata, modifiedM->param, *n, x, fvec, iflag);
}

MINPACKEX_API
void minpackex_lmdif(
    void *userdata,
    minpackex_lmdif_callback callback,
    int m, int n, double *x, double *fvec, double ftol,
    double xtol, double gtol, int maxfev, double epsfcn,
    double *diag, int mode, double factor, int nprint,
    int *info, int *nfev, double *fjac, int ldfjac, int *ipvt,
    double *qtf, double *wa1, double *wa2, double *wa3,
    double *wa4 )
{
    ModifiedIntParam m_param;
    m_param.param = m;
    m_param.userdata = userdata;
    m_param.callback = (void *)callback;

    lmdif_(
        __minpack_lmdif_callback,
        (int *)(&m_param), &n, x, fvec, &ftol,
        &xtol, &gtol, &maxfev, &epsfcn,
        diag, &mode, &factor, &nprint,
        info, nfev, fjac, &ldfjac, ipvt,
        qtf, wa1, wa2, wa3,
        wa4
    );
}