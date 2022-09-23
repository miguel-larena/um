/******************************************************************************
 *                                 MAIN.C
 *                         ----------------------
 *  Purpose: Initialize and run a UM
 *
 *  Dependencies: um.h
 *                Compile with gcc
 *
 *  By: Jacob Zimmerman and Miguel Larena
 *  Date: April 13 2022
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
// #include "um.h"
#include "assert.h"

int main(int argc, char **argv)
{
    FILE *fp = stdin;

    if (argc == 2) {
        fp = fopen(argv[1], "r");
        assert(fp);
    } else {
        fprintf(stderr, "um called with incorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }

    // UM_run(fp);
    Machine_run(fp);

    fclose(fp);
    return 0;
}
