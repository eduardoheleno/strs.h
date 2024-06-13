#include "file.h"
#include "strs.h"

// MN magic number

int main(int argc, char *argv[]) {
    if (argc < 2) {
	perror("not enough arguments");
	exit(1);
    }

    char *file_name = argv[1];
    FILE *fptr = open_file(file_name);

    WINDOW *w = initscr();
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    int line_size = getmaxx(w);
    char line_buffer[line_size];

    print_file(fptr, w);

    getch();

    endwin();

 //    STree *root = malloc(sizeof(STree));
 //    char *chunk = "teste";
 //    int chunk_size = strlen(chunk);
	//
 //    int str_size = 0;
 //    str_size += chunk_size;
	//
 //    int i = chunk_size - 1;
	//
 //    int char_position = str_size;
 //    while (i >= 0) {
	// STree *curr_root = root;
	//
	// for (int j = i; j < chunk_size; j++) {
	//     int asc_char = chunk[j] - ASCII_DECIMAL_MN;
	//
	//     if (curr_root->childrens[asc_char] == NULL) {
	// 	STree *node = malloc(sizeof(STree));
	// 	node->positions_size = 1;
	// 	node->positions = malloc(sizeof(int) * node->positions_size);
	// 	node->positions[0] = char_position;
	//
	// 	curr_root->childrens[asc_char] = node;
	// 	curr_root = node;
	//     } else {
	// 	curr_root = curr_root->childrens[asc_char];
	//
	// 	curr_root->positions_size++;
	// 	curr_root->positions = realloc(curr_root->positions, sizeof(int) * curr_root->positions_size);
	// 	curr_root->positions[curr_root->positions_size - 1] = char_position;
	//     }
	// }
	//
	// char_position--;
	// i--;
 //    }

    // printf("%i\n", root->childrens[(int)'s' - 97]->childrens[(int)'t' - 97]->positions[0]);
    return 0;
}
