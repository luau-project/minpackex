#ifdef _WIN32
#   define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#ifdef _WIN32
#   undef _CRT_SECURE_NO_WARNINGS
#endif

#include "tchkder-minpack.h"
#include "tchkder-minpackex.h"

#include "tdpmpar-minpack.h"
#include "tdpmpar-minpackex.h"

#include "tenorm-minpack.h"
#include "tenorm-minpackex.h"

#include "thybrd-minpack.h"
#include "thybrd-minpackex.h"

#include "thybrd1-minpack.h"
#include "thybrd1-minpackex.h"

#include "thybrj-minpack.h"
#include "thybrj-minpackex.h"

#include "thybrj1-minpack.h"
#include "thybrj1-minpackex.h"

#include "tlmder-minpack.h"
#include "tlmder-minpackex.h"

#include "tlmder1-minpack.h"
#include "tlmder1-minpackex.h"

#include "tlmdif-minpack.h"
#include "tlmdif-minpackex.h"

#include "tlmdif1-minpack.h"
#include "tlmdif1-minpackex.h"

#include "tlmstr-minpack.h"
#include "tlmstr-minpackex.h"

#include "tlmstr1-minpack.h"
#include "tlmstr1-minpackex.h"

int equal_files(FILE *a, FILE *b)
{
    int result = 0;

    fseek(a, 0, SEEK_SET);
    fseek(b, 0, SEEK_SET);

    int ca = 0;
    int cb = 0;

    while (ca == cb && ca != EOF)
    {
        ca = fgetc(a);
        cb = fgetc(b);
    }
    
    result = ca == cb ? 0 : 1;

    return result;
}

int main(int argc, char **argv)
{
    int result = 1;

    if (argc == 3)
    {
        char *minpack_filename = argv[1];
        char *minpackex_filename = argv[2];

        FILE *minpack_file = fopen(minpack_filename, "w+b");
        FILE *minpackex_file = fopen(minpackex_filename, "w+b");

        tchkder_minpack_write_content(minpack_file);
        tchkder_minpackex_write_content(minpackex_file);

        tdpmpar_minpack_write_content(minpack_file);
        tdpmpar_minpackex_write_content(minpackex_file);

        tenorm_minpack_write_content(minpack_file);
        tenorm_minpackex_write_content(minpackex_file);

        thybrd_minpack_write_content(minpack_file);
        thybrd_minpackex_write_content(minpackex_file);

        thybrd1_minpack_write_content(minpack_file);
        thybrd1_minpackex_write_content(minpackex_file);

        thybrj_minpack_write_content(minpack_file);
        thybrj_minpackex_write_content(minpackex_file);

        thybrj1_minpack_write_content(minpack_file);
        thybrj1_minpackex_write_content(minpackex_file);

        tlmder_minpack_write_content(minpack_file);
        tlmder_minpackex_write_content(minpackex_file);

        tlmder1_minpack_write_content(minpack_file);
        tlmder1_minpackex_write_content(minpackex_file);

        tlmdif_minpack_write_content(minpack_file);
        tlmdif_minpackex_write_content(minpackex_file);

        tlmdif1_minpack_write_content(minpack_file);
        tlmdif1_minpackex_write_content(minpackex_file);

        tlmstr_minpack_write_content(minpack_file);
        tlmstr_minpackex_write_content(minpackex_file);

        tlmstr1_minpack_write_content(minpack_file);
        tlmstr1_minpackex_write_content(minpackex_file);

        fflush(minpack_file);
        fflush(minpackex_file);

        result = equal_files(minpack_file, minpackex_file);

        if (result)
        {
            printf("\nThe content of files generated by minpack and minpackex had a divergence\n");
        }

        fclose(minpackex_file);
        fclose(minpack_file);
    }
    else
    {
        printf("\nInvalid number of arguments to the test driver\n");
    }
    
    return result;
}