/*
 * info.c - Main for info executable. Verify file format for input file.
 *
 * @author Eli Musgrove
 */

#include "processfile.h"
#include <stdio.h>
#include <stdlib.h>

/* Moves as processed from file by processfile.c */
extern unsigned int* moves;

int main() {
    /* Process input file and print the output */
    processFile(1, -1);

    /* Free moves array */
    free(moves);

    return 0;
}
