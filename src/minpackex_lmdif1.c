#include "minpackex.h"

typedef struct ModifiedMParam ModifiedMParam;

/*
** The goal of ModifiedMParam
** is the exploitation of m param.
** We place extra data (userdata and
** a modified callback signature).
** at the end of int *m pointer.
** Since MINPACK-1 just
** operates on m as int, it seems
** a "safe" hack to employ.
*/

struct ModifiedMParam
{
    int m;
    void *userdata;
    minpackex_lmdif1_callback callback;
};

static void minpack_callback(int *m, int *n, double *x, double *fvec, int *iflag)
{
    ModifiedMParam *modifiedM = (ModifiedMParam *)(void *)m;
    modifiedM->callback(modifiedM->userdata, modifiedM->m, *n, x, fvec, iflag);
}

MINPACKEX_API
void minpackex_lmdif1(
    void *userdata,
    minpackex_lmdif1_callback callback,
    int m, int n, double *x, double *fvec, double tol,
    int *info, int *iwa, double *wa, int lwa)
{
    ModifiedMParam m_param;
    m_param.m = m;
    m_param.userdata = userdata;
    m_param.callback = callback;

    lmdif1_(minpack_callback, (int *)(&m_param), &n, x, fvec, &tol, info, iwa, wa, &lwa);
}