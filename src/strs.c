#include "strs.h"

size_t strs_strlen(char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
	len++;
    }

    return len;
}

void include_chunk(STree *root, char *chunk, size_t *total_size) {
    size_t chunk_size = strs_strlen(chunk);
    *total_size += chunk_size + 1;

    int i = chunk_size - 1, char_position = *total_size - 1;

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
}
