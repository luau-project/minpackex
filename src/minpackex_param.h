
typedef struct ModifiedIntParam ModifiedIntParam;

/*
** The goal of ModifiedIntParam
** is the exploitation of an int * param
** on a minpack call (usually int *m or int *n).
** We place extra data (userdata and
** a modified callback signature).
** at the end of int *m or
** int *n pointer.
** Since MINPACK-1 is written in Fortran
** and arguments are passed by refeference,
** it seems a "safe" hack to employ.
*/

struct ModifiedIntParam
{
    int param;
    void *userdata;
    void *callback;
};