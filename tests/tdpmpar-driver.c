#ifdef _WIN32
#   define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#ifdef _WIN32
#   undef _CRT_SECURE_NO_WARNINGS
#endif
#include "tdpmpar-minpack.h"
#include "tdpmpar-minpackex.h"

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

        tdpmpar_minpack_write_content(minpack_file);
        tdpmpar_minpackex_write_content(minpackex_file);

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