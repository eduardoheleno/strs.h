#include "file.h"
#include "strs.h"
#include <string.h>
#include <curses.h>

#define SPACE_CHAR ' '
#define EOL '\n'
#define ESC_KEY 27
#define ENTER_KEY 10

static STree root;

WINDOW* init_ncurses() {
    WINDOW *w = initscr();
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    return w;
}

// TODO (maybe): '\n' char delimits the end of a word, but only ' ' should settle that
// TODO: accept all default ASCII characters

int read_word(char **word_buffer, char *line_buffer, int line_size, int *x) {
    size_t word_buffer_size = 0;
    static size_t cursor = 0;

    for (; cursor < line_size - 1; cursor++) {
	if (line_buffer[cursor] == EOL && word_buffer_size > 0) break;
	if (line_buffer[cursor] == SPACE_CHAR && word_buffer_size > 0) break;
	if (line_buffer[cursor] != SPACE_CHAR) word_buffer_size++;

	if (line_buffer[cursor] == EOL) {
	    cursor = 0;
	    return -1;
	}

	++*x;
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

void highlight_characters(Position **positions, char *c_arr, int line_size, size_t positions_size) {
    if (positions == NULL) return;

    attron(COLOR_PAIR(1));
    for (int i = 0; i < positions_size; i++) {
	Position *p = positions[i];

	mvprintw(p->y, p->x, "%s", c_arr);
	refresh();
    }
    attroff(COLOR_PAIR(1));
}

void clear_highlight(Position **positions, char *c_arr, int line_size, size_t positions_size) {
    if (positions == NULL) return;

    for (int i = 0; i < positions_size; i++) {
	Position *p = positions[i];

	mvprintw(p->y, p->x, "%s", c_arr);
	refresh();
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
	perror("not enough arguments");
	exit(1);
    }

    WINDOW *w = init_ncurses();
    int line_size = getmaxx(w), row_size = getmaxy(w);

    char *file_name = argv[1], line_buffer[line_size];
    FILE *f = open_file(file_name);

    int x = 0, y = 0;

    while (read_line(f, line_size, line_buffer) > 0) {
	char *word_buffer = NULL;
	x = 0;

	while (read_word(&word_buffer, line_buffer, line_size, &x) > 0) {
	    strs_include_chunk(&root, word_buffer, x, y);
	    free(word_buffer);
	}

	mvprintw(y++, 0, "%s", line_buffer);
    }
    refresh();
    fclose(f);

    size_t c_size = 0;
    char *c_arr;
    Position **positions = NULL;
    size_t positions_size = 0;

    for (;;) {
	char c = getch();
	if ((int) c == ESC_KEY) break;
	if (((int) c - ASCII_DECIMAL_MN) < 0 || ((int) c - ASCII_DECIMAL_MN) > 25) continue;

	clear_highlight(positions, c_arr, line_size, positions_size);

	c_arr = realloc(c_arr, (sizeof(char) * ++c_size) + sizeof(char));
	c_arr[c_size - 1] = c;
	c_arr[c_size] = '\0';

	positions = strs_search_positions(&root, c_arr, c_size, &positions_size);
	highlight_characters(positions, c_arr, line_size, positions_size);

	mvprintw(row_size - 1, 0, "searched pattern: %s", c_arr);
	refresh();
    }

    free(positions);
    free(c_arr);
    endwin();

    return 0;
}
