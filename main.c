#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MN magic number
#define ASCII_DECIMAL_MN 97

typedef struct STree {
    struct STree *childrens[26];

    int *positions;
    int positions_size;
} STree;

int main() {
    STree *root = malloc(sizeof(STree));
    char *chunk = "teste";
    int chunk_size = strlen(chunk);

    int str_size = 0;
    str_size += chunk_size;

    int i = chunk_size - 1;

    int char_position = str_size;
    while (i >= 0) {
	STree *curr_root = root;

	for (int j = i; j < chunk_size; j++) {
	    int asc_char = chunk[j] - ASCII_DECIMAL_MN;

	    if (curr_root->childrens[asc_char] == NULL) {
		STree *node = malloc(sizeof(STree));
		node->positions_size = 1;
		node->positions = malloc(sizeof(int) * node->positions_size);
		node->positions[0] = char_position;

		curr_root->childrens[asc_char] = node;
		curr_root = node;
	    } else {
		curr_root = curr_root->childrens[asc_char];

		curr_root->positions_size++;
		curr_root->positions = realloc(curr_root->positions, sizeof(int) * curr_root->positions_size);
		curr_root->positions[curr_root->positions_size - 1] = char_position;
	    }
	}

	char_position--;
	i--;
    }

    printf("%i\n", root->childrens[(int)'s' - 97]->positions[0]);
    printf("%i", root->childrens[(int)'s' - 97]->positions_size);
}
