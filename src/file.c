#include "file.h"
#include <stdlib.h>

FILE* open_file(char *file_name) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
	perror("couldn't open the file");
	exit(1);
    }

    return f;
}

int read_line(FILE *f, int line_size, char *line_buffer) {
    if (fgets(line_buffer, line_size, f) == NULL) return -1;
    return 1;
}
