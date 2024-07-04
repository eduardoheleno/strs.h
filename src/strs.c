#include "strs.h"
#include <string.h>

size_t strs_strlen(char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
	len++;
    }

    return len;
}

void strs_include_chunk(STree *root, char *chunk, int x, int y) {
    size_t chunk_size = strs_strlen(chunk);
    int i = chunk_size - 1, x_position = x - 1, y_position = y;

    while (i >= 0) {
	STree *curr_root = root;
	Position *p = malloc(sizeof(Position));
	p->x = x_position;
	p->y = y_position;

	for (int j = i; j < chunk_size; j++) {
	    int asc_char = chunk[j] - ASCII_DECIMAL_MN;
	    if (asc_char >= 0 && asc_char < 93) {
		if (curr_root->childrens[asc_char] == NULL) {
		    STree *node = malloc(sizeof(STree));
		    node->positions_size = 1;
		    node->positions = malloc(sizeof(Position*) * node->positions_size);
		    node->positions[0] = p;

		    memset(node->childrens, 0, sizeof(node->childrens));

		    curr_root->childrens[asc_char] = node;
		    curr_root = node;
		} else {
		    curr_root = curr_root->childrens[asc_char];

		    curr_root->positions_size++;
		    curr_root->positions = realloc(curr_root->positions, sizeof(Position*) * curr_root->positions_size);
		    curr_root->positions[curr_root->positions_size - 1] = p;
		}
	    }
	}
	x_position--;
	i--;
    }
}

Position** strs_search_positions(STree *root, char *c_arr, size_t c_size, size_t *positions_size) {
    STree *curr_root = root;
    Position **positions = NULL;

    for (int i = 0; i < c_size; i++) {
	if (curr_root->childrens[c_arr[i] - ASCII_DECIMAL_MN] == NULL) {
	    curr_root = NULL;
	    break;
	}
	curr_root = curr_root->childrens[c_arr[i] - ASCII_DECIMAL_MN];
    }

    if (curr_root != NULL) {
	*positions_size = curr_root->positions_size;
	positions = malloc(sizeof(int) * curr_root->positions_size);
	positions = curr_root->positions;
    }

    return positions;
}
