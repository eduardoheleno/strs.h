#include "file.h"
#include "strs.h"

#define SPACE_CHAR ' '
#define ESC_KEY 27

WINDOW* init_ncurses() {
    WINDOW *w = initscr();
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    return w;
}

// wrong line_size "if line has '\n' char"
int read_word(char **word_buffer, char *line_buffer, int line_size, size_t *total_size) {
    size_t word_buffer_size = 0;
    static size_t cursor = 0;

    for (; cursor < line_size - 1; cursor++) {
	if (line_buffer[cursor] == SPACE_CHAR && word_buffer_size > 0) {
	    break;
	}

	if (line_buffer[cursor] != SPACE_CHAR) {
	    word_buffer_size++;
	}

	++*total_size;
    }
    if (cursor >= line_size - 1 && word_buffer_size == 0) {
	cursor = 0;
	return -1;
    }

    *word_buffer = malloc(sizeof(char) * (word_buffer_size + 1));
    size_t buf_ch_pos = 0;
    for (int i = cursor - word_buffer_size; i < cursor; i++) {
	(*word_buffer)[buf_ch_pos++] = line_buffer[i];
    }
    (*word_buffer)[buf_ch_pos] = '\0';

    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
	perror("not enough arguments");
	exit(1);
    }

    WINDOW *w = init_ncurses();
    int line_size = getmaxx(w), y_counter = 0;

    char *file_name = argv[1], line_buffer[line_size];
    FILE *f = open_file(file_name);

    STree *root = malloc(sizeof(STree));
    size_t total_size = 0;

    while (read_line(f, line_size, line_buffer) > 0) {
	char *word_buffer = NULL;
	while (read_word(&word_buffer, line_buffer, line_size, &total_size) > 0) {
	    strs_include_chunk(root, word_buffer, total_size);
	    free(word_buffer);
	}

	mvprintw(y_counter++, 0, "%s", line_buffer);
    }
    refresh();
    fclose(f);

    size_t c_size = 0;
    char *c_arr;
    int *positions = NULL;

    for (;;) {
	char c = getch();
	if (c == ESC_KEY) break;
	if ((c - ASCII_DECIMAL_MN) < 0 || (c - ASCII_DECIMAL_MN) > 25) continue;

	c_arr = realloc(c_arr, ++c_size);
	c_arr[c_size - 1] = c;

	positions = strs_search_positions(root, c_arr, c_size);
    }

    endwin();

    return 0;
}
