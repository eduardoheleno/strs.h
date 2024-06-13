#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

FILE* open_file(char *file_name);
void print_file(FILE *fptr, WINDOW *w);

#endif
