#ifndef STRS_H
#define STRS_H

#include <stdlib.h>

// MN magic number
#define ASCII_DECIMAL_MN 97

typedef struct STree {
    struct STree *childrens[26];

    int *positions;
    int positions_size;
} STree;

void strs_include_chunk(STree *root, char *chunk, size_t total_size);
int* strs_search_positions(STree *root, char *c_arr, size_t c_size, size_t *positions_size);

#endif
