#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

FILE* open_file(char *file_name);
int read_line(FILE *f, int line_size, char *line_buffer);

#endif
