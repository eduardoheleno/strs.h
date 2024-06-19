#include "file.h"
#include "strs.h"

WINDOW* init_ncurses() {
    WINDOW *w = initscr();
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    return w;
}

// redo
int read_word(char **word_buffer, char *line_buffer, int line_size) {
    size_t word_buffer_size = 0;
    static size_t cursor = 0;

    if (cursor >= line_size || line_buffer[cursor] == ' ' || line_buffer[cursor] == '\0') {
	cursor = 0;
	return -1;
    }

    while (line_buffer[cursor] != ' ' && cursor < line_size) {
	cursor++;
	word_buffer_size++;
    }

    *word_buffer = malloc((sizeof(char) * word_buffer_size) + 1);
    int counter = 0;
    for (int i = cursor - word_buffer_size; i < cursor; ++i) {
	(*word_buffer)[counter++] = line_buffer[i];
    }

    (*word_buffer)[counter] = '\0';
    cursor++;

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
    size_t total_size = -1;

    while (read_line(f, line_size, line_buffer) > 0) {
	char *word_buffer = NULL;
	while (read_word(&word_buffer, line_buffer, line_size) > 0) {
	    // segmentation fault
	    strs_include_chunk(root, word_buffer, &total_size);
	    free(word_buffer);
	}

	mvprintw(y_counter++, 0, "%s", line_buffer);
    }

    fclose(f);
    refresh();
    getch();

    endwin();

    return 0;
}
