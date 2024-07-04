#include "file.h"
#include "strs.h"
#include <string.h>
#include <curses.h>

#define SPACE_CHAR ' '
#define EOL '\n'
#define ESC_KEY 27
#define BACKSPACE_KEY 127
#define ENTER_KEY 10

static STree root;

WINDOW* init_ncurses() {
    WINDOW *w = initscr();
    noecho();
    keypad(w, TRUE);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    return w;
}

int read_word(char **word_buffer, char *line_buffer, int terminal_max_x, int *x) {
    size_t word_buffer_size = 0;
    static size_t cursor = 0;

    for (; cursor < terminal_max_x - 1; cursor++) {
	if (line_buffer[cursor] == EOL && word_buffer_size > 0) break;
	if (line_buffer[cursor] == SPACE_CHAR && word_buffer_size > 0) break;
	if (line_buffer[cursor] != SPACE_CHAR && line_buffer[cursor] != EOL) word_buffer_size++;

	if (line_buffer[cursor] == EOL) {
	    cursor = 0;
	    return -1;
	}

	++*x;
    }
    if (cursor >= terminal_max_x - 1 && word_buffer_size == 0) {
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

void highlight_characters(Position **positions, char *c_arr, int terminal_max_x, size_t positions_size) {
    if (positions == NULL) return;

    attron(COLOR_PAIR(1));
    for (int i = 0; i < positions_size; i++) {
	Position *p = positions[i];

	mvprintw(p->y, p->x, "%s", c_arr);
	refresh();
    }
    attroff(COLOR_PAIR(1));
}

void clear_highlight(Position **positions, char *c_arr, int terminal_max_x, size_t positions_size) {
    if (positions == NULL) return;

    for (int i = 0; i < positions_size; i++) {
	Position *p = positions[i];

	mvprintw(p->y, p->x, "%s", c_arr);
	refresh();
    }
}

void clear_last_char(size_t *c_size, char *c_arr, int terminal_max_y) {
    if (c_arr == NULL) return;

    if (*c_size == 0) {
	free(c_arr);
	c_arr = NULL;

	return;
    }
    c_arr[--*c_size] = '\0';
    c_arr = realloc(c_arr, (sizeof(char) * *c_size) + sizeof(char));
    mvdelch(terminal_max_y - 1, *c_size + 18);
}

char** load_file(FILE *f, int terminal_max_x, size_t *file_max_y) {
    if (f == NULL) perror("couldn't load the file");

    char **file_lines = NULL;
    char line_buffer[terminal_max_x];

    while (read_line(f, terminal_max_x, line_buffer) > 0) {
	size_t buffer_size = 0;
	while ((line_buffer[buffer_size] != '\n') && buffer_size < terminal_max_x) {
	    buffer_size++;
	}

	file_lines = realloc(file_lines, sizeof(char*) * ++*file_max_y);
	file_lines[*file_max_y - 1] = malloc(sizeof(char) * buffer_size + 1);

	for (int i = 0; i < buffer_size; i++) {
	    file_lines[*file_max_y - 1][i] = line_buffer[i];
	}
	file_lines[*file_max_y - 1][buffer_size] = '\0';
    }

    return file_lines;
}

void free_file(char **file_lines, size_t file_max_y) {
    for (size_t i = 0; i < file_max_y; i++) {
	free(file_lines[i]);
    }
    free(file_lines);
}

void terminal_render_file(char **file_lines, int y_offset, size_t file_max_y) {
    clear();
    for (int i = 0; y_offset < file_max_y; i++) {
	mvprintw(i, 0, "%s", file_lines[y_offset++]);
    }
    refresh();
}

void build_stree(char **file_lines, int terminal_max_x, size_t file_max_y) {
    int x = 0, y = 0;

    for (int i = 0; i < file_max_y; i++) {
	char *file_line = file_lines[i];
	x = strlen(file_lines[i]);
	strs_include_chunk(&root, file_lines[i], x, y);
	y++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
	perror("not enough arguments");
	exit(1);
    }

    WINDOW *w = init_ncurses();
    int terminal_max_x = getmaxx(w), terminal_max_y = getmaxy(w);
    size_t file_max_y = 0;

    char *file_name = argv[1], line_buffer[terminal_max_x];
    FILE *f = open_file(file_name);

    char **file_lines = load_file(f, terminal_max_x, &file_max_y);
    terminal_render_file(file_lines, 0, file_max_y);
    build_stree(file_lines, terminal_max_x, file_max_y);

    refresh();
    fclose(f);

    Position **positions = NULL;
    size_t c_size = 0, positions_size = 0;
    char *c_arr;
    int y_offset = 0;

    for (;;) {
	int c = getch();

	if (c == KEY_DOWN) {
	    if (file_lines[y_offset + 1] != NULL) {
		terminal_render_file(file_lines, ++y_offset, file_max_y);
	    }
	}
	if (c == KEY_UP) {
	    if (y_offset - 1 >= 0) {
		terminal_render_file(file_lines, --y_offset, file_max_y);
	    }
	}

	if (c == KEY_BACKSPACE) {
	    clear_highlight(positions, c_arr, terminal_max_x, positions_size);

	    clear_last_char(&c_size, c_arr, terminal_max_y);

	    positions = strs_search_positions(&root, c_arr, c_size, &positions_size);
	    highlight_characters(positions, c_arr, terminal_max_x, positions_size);
	    continue;
	}
	if (c == ESC_KEY) break;
	if ((c - ASCII_DECIMAL_MN) < 0 || (c - ASCII_DECIMAL_MN) > 93) continue;

	clear_highlight(positions, c_arr, terminal_max_x, positions_size);

	c_arr = realloc(c_arr, (sizeof(char) * ++c_size) + sizeof(char));
	c_arr[c_size - 1] = c;
	c_arr[c_size] = '\0';

	positions = strs_search_positions(&root, c_arr, c_size, &positions_size);
	highlight_characters(positions, c_arr, terminal_max_x, positions_size);

	mvprintw(terminal_max_y - 1, 0, "searched pattern: %s", c_arr);
	refresh();
    }

    // free properly
    free(positions);
    free_file(file_lines, file_max_y);
    free(c_arr);
    endwin();

    return 0;
}
