#ifndef STRS_H
#define STRS_H

#include <stdlib.h>

// MN magic number
#define ASCII_DECIMAL_MN 33

typedef struct Position {
    int x, y;
} Position;

typedef struct STree {
    struct STree *childrens[93];

    Position **positions;
    int positions_size;
} STree;

void strs_include_chunk(STree *root, char *chunk, int x, int y);
Position** strs_search_positions(STree *root, char *c_arr, size_t c_size, size_t *positions_size);

#endif
