#ifndef FILE_H
#define FILE_H

#include <ncurses.h>
#include <stdlib.h>

FILE* open_file(char *file_name);
int read_line(FILE *f, int line_size, char *line_buffer);

#endif
