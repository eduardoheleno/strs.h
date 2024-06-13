#ifndef STRS_H
#define STRS_H

#define ASCII_DECIMAL_MN 97

typedef struct STree {
    struct STree *childrens[26];

    int *positions;
    int positions_size;
} STree;

#endif
