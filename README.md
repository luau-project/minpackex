# minpackex

An extension to MINPACK-1 library.

## Table of Contents

* [Introduction](#introduction)
    * [Description](#description)
    * [What is minpack?](#what-is-minpack)
    * [Why minpackex is needed?](#why-minpackex-is-needed)
* [Continuous Integration](#continuous-integration)
* [Documentation](#documentation)
* [References](#references)

## Introduction

### Description

This software is an extension to the popular double-precision version of MINPACK-1 library [[1]](https://www.netlib.org/minpack) - or simply *minpack*, which provides routines to solve a system of nonlinear equations and nonlinear least squares problems. Without any modification to the source code of minpack, employing the powerful CMake [[2]](https://cmake.org/) build system to handle platform and compiler intricacies on different operating systems, the goal of ```minpackex``` is to extend the original minpack function signatures to an easier-to-use API to the programmer, allowing user data to be shared across function calls.

> [!TIP]
> 
> ```minpackex``` has a hard dependency on the minpack library and cannot function without it. In the [documentation](doc/README.md), we describe in details how to build both libraries on different combinations of platform / compiler toolchains.

### What is minpack?

The original minpack [[1]](https://www.netlib.org/minpack) is a battle-tested **min**imization **pack**age written in Fortran 77 for solving a system of nonlinear equations and nonlinear least squares problems, discussed in details at [[3]](https://doi.org/10.2172/6997568). Throughout the years, popular scientific libraries like SciPy [[4]](https://github.com/scipy/scipy/tree/main/scipy/optimize/minpack) and Eigen [[5]](https://eigen.tuxfamily.org/dox/unsupported/index.html) have been using minpack to perform nonlinear optimization.

> [!IMPORTANT]
> 
> Do not confuse the original minpack [[1]](https://www.netlib.org/minpack) written in Fortran 77 with Modernized Minpack [[6]](https://github.com/fortran-lang/minpack). The modern minpack [[6]](https://github.com/fortran-lang/minpack) ported the initial code to newer Fortran standards. Here, we extend the original minpack library, not the modernized version.

### Why minpackex is needed?

* In the first place, the most important decision to use this library is the safety to employ the original implementation of minimization code, written in Fortran 77, provided by minpack authors [[1]](https://www.netlib.org/minpack), not the code ported to other programming languages by third parties;

* Second, once you compile the original minpack code to a binary, the usual API (which on Linux resembles minpack.h [[7]](https://salsa.debian.org/science-team/minpack/-/blob/master/minpack.h?ref_type=heads)) does not allow user data to be shared by optimization routines in a straight forward manner. To address this limitation, we apply a trick in the passage of parameters in order to allow user data to be forwarded to such functions. To further explain this point, see how the parameter ```void *userdata``` works in the basic usage below to perform a function fit on a pair of points $(1, -1)$ and $(4, 5)$ for a model in the form $f(x) = a \cdot x + b$.

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <minpackex.h>

typedef double (* model)(double x, const double *params);

typedef struct fit_data fit_data;

struct fit_data
{
    double *x;
    double *y;
    model f;
};

static void lmdif1_callback(
    void *userdata,
    int m,
    int n,
    const double *x,
    double *fvec,
    int *info)
{
    fit_data *data = (fit_data *)userdata;
    model f = data->f;
    double *px = data->x;
    double *py = data->y;

    for (int i = 0; i < m; i++)
    {
        fvec[i] = f(px[i], x) - py[i];
    }
}

static double linear_model(double x, const double *params)
{
    double a = params[0];
    double b = params[1];
    return a * x + b;
}

// void minpackex_lmdif1(
//    void *userdata,
//    minpackex_lmdif1_callback callback,
//    int m, int n, double *x, double *fvec, double tol,
//    int *info, int *iwa, double *wa, int lwa);

int main(void)
{
    double x[2] = { 1,  4 };
    double y[2] = { -1, 5 };

    fit_data data;
    data.x = x;
    data.y = y;
    data.f = linear_model;

    double guess[2] = { 1, 1 };
    
    /* size of x and y arrays (m = 2) */
    int m = sizeof(x) / sizeof(double);

    /*
    ** size of guess (n = 2),
    ** which is also the number of unknown parameters
    ** on the model -> linear_model (a and b)
    */
    int n = sizeof(guess) / sizeof(double);

    double tol = sqrt(minpackex_dpmpar(1));
    int info;
    int lwa = m * n + 5 * n + m;
    
    double *wa = (double *)malloc(lwa * sizeof(double));
    double *fvec = (double *)malloc(m * sizeof(double));
    int *iwa = (int *)malloc(n * sizeof(int));
    
    minpackex_lmdif1(
        (void *)&data,
        &lmdif1_callback,
        m,
        n,
        guess,
        fvec,
        tol,
        &info,
        iwa,
        wa,
        lwa);

    printf("\n\n// input:\n\n");
    printf("// \t (x, y):\n");
    for (int i = 0; i < m; i++)
    {
        printf("// \t\t (%g, %g)\n", x[i], y[i]);
    }

    printf("\n// \t model:\n");
    printf("// \t\t y = a * x + b\n\n");

    if (1 <= info && info <= 4)
    {
        printf("\n// nice fit\n\n");
        printf("// found parameters:\n");
        printf("// \t a = %g\n", guess[0]);
        printf("// \t b = %g\n\n", guess[1]);
    }
    else
    {
        printf("\n// bad fit\n\n");
    }

    free(iwa);
    free(fvec);
    free(wa);

    return 0;
}
```

Running the example above produces the following output:

```c


// input:

// 	 (x, y):
// 		 (1, -1)
// 		 (4, 5)

// 	 model:
// 		 y = a * x + b


// nice fit

// found parameters:
// 	 a = 2
// 	 b = -3


```

## Continuous Integration

At the moment, running a total of 276 different tests on multiple settings, the continuous integration on github is able to build and install ```minpackex``` in the following combination of platform / compiler toolchain, asserting that exactly the same output is obtained using either minpackex or minpack.

### Build Status

| Windows 11 | Ubuntu 22.04 |
|------------|--------------|
|![windows workflow](https://github.com/luau-project/minpackex/actions/workflows/minpackex-on-windows.yaml/badge.svg?branch=main) | ![ubuntu workflow](https://github.com/luau-project/minpackex/actions/workflows/minpackex-on-ubuntu.yaml/badge.svg?branch=main) |

> [!NOTE]
> 
> In the tables below, *MSVC-like* means the underline linker is MSVC-compatible. Therefore, *GCC-like* means the underline linker is GCC-compatible.

### Windows 11

| Fortran Compiler | Fortran Compiler Version | C Compiler | C Compiler Version |
|------------------|------------------|--------------|------------------|
| GFortran | 14.1.0 | GCC | 14.1.0 |
| LLVM Flang-new (*GCC-like*) | 18.1.4 | Clang | 18.1.4 |
| Intel LLVM ifx (*MSVC-like*) | 2024.1.0 | MSVC | 19.39.33523.0 |
| Intel LLVM ifx (*MSVC-like*) | 2024.1.0 | Clang-cl | 16.0.6 |
| Intel LLVM ifx (*MSVC-like*) | 2024.1.0 | Intel LLVM icx (*MSVC-like*) | 2024.1.0 |
| Intel ifort (*MSVC-like*) | 2021.10.0 | MSVC | 19.39.33523.0 |
| Intel ifort (*MSVC-like*) | 2021.10.0 | Clang-cl | 16.0.6 |
| Intel ifort (*MSVC-like*) | 2021.10.0 | Intel icl (*MSVC-like*) | 2021.10.0 |


### Ubuntu 22.04

| Fortran Compiler | Fortran Compiler Version | C Compiler | C Compiler Version |
|------------------|------------------|--------------|------------------|
| GFortran | 13.1.0 | GCC | 13.1.0 |
| LLVM Flang-new (*GCC-like*) | 18.1.6 | Clang | 18.1.6 |
| Intel LLVM ifx (*GCC-like*) | 2024.1.2 | Intel LLVM icx (*GCC-like*) | 2024.1.2 |
| Intel ifort (*GCC-like*) | 2021.10.0 | Intel icl (*GCC-like*) | 2021.10.0 |
| Nvidia nvfortran | 2023.11 | Nvidia nvc | 2023.11 |

## Documentation

Browse the [documentation](doc/README.md).

## References

1. minpack. Accessed May 2, 2024. [https://www.netlib.org/minpack](https://www.netlib.org/minpack);
2. CMake. Accessed May 2, 2024. [https://cmake.org/](https://cmake.org/);
3. Moré, Jorge J., Burton S. Garbow, and Kenneth E. Hillstrom. *User guide for MINPACK-1.[In FORTRAN]*. No. ANL-80-74. Argonne National Lab.(ANL), Argonne, IL (United States), 1980. DOI: [10.2172/6997568](https://doi.org/10.2172/6997568);
4. SciPy. Accessed May 2, 2024. [https://github.com/scipy/scipy/tree/main/scipy/optimize/minpack](https://github.com/scipy/scipy/tree/main/scipy/optimize/minpack);
5. Eigen. Accessed May 2, 2024. [https://eigen.tuxfamily.org/dox/unsupported/index.html](https://eigen.tuxfamily.org/dox/unsupported/index.html);
6. Modernized Minpack. Accessed May 3, 2024. [https://github.com/fortran-lang/minpack](https://github.com/fortran-lang/minpack);
7. Debian Science Team repository for minpack. Accessed May 28, 2024. [https://salsa.debian.org/science-team/minpack/-/blob/master/minpack.h?ref_type=heads](https://salsa.debian.org/science-team/minpack/-/blob/master/minpack.h?ref_type=heads).