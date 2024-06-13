#include "file.h"

FILE* open_file(char *file_name) {
    FILE *fptr = fopen(file_name, "r");
    if (fptr == NULL) {
	perror("couldn't open the file");
	exit(1);
    }

    return fptr;
}

void print_file(FILE *fptr, WINDOW *w) {
    int line_size = getmaxx(w), line_y = 0;
    char line_buffer[line_size];

    while (fgets(line_buffer, line_size, fptr) != NULL) {
	mvprintw(line_y++, 0, "%s", line_buffer);
    }

    refresh();
}
