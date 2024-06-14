#include "file.h"
#include "strs.h"

static STree *root;

// still not working
int read_word(char **word_buffer, char *line_buffer, int line_size) {
    static int cursor = 0;
    size_t word_buffer_size = 0;

    if (cursor >= line_size) {
	// not entering in this condition
	exit(1);
	cursor = 0;
	return -1;
    }

    while (line_buffer[cursor] != ' ' && cursor < line_size) {
	cursor++;
	word_buffer_size++;
    }

    *word_buffer = malloc((sizeof(char) * word_buffer_size) + 1);
    for (int i = 0; i < word_buffer_size; i++) {
	(*word_buffer)[i] = line_buffer[cursor];
	(*word_buffer)[word_buffer_size] = '\0';
    }

    return 1;
}

WINDOW* init_ncurses() {
    WINDOW *w = initscr();
    noecho();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    return w;
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

    root = malloc(sizeof(STree));
    size_t total_size = -1;

    while (read_line(f, line_size, line_buffer) > 0) {
	char *word_buffer = NULL;
	while (read_word(&word_buffer, line_buffer, line_size) > 0) {
	    // include_chunk(root, word_buffer, &total_size);
	    printw("%s", word_buffer);
	    refresh();
	    free(word_buffer);
	}

	// mvprintw(y_counter++, 0, "%s", line_buffer);
    }

    fclose(f);

    refresh();

    getch();

    endwin();

    // teste minimize



    // char *chunk = "teste\0";
    // char *chunk2 = "minimize\0";
    //
    // include_chunk(root, chunk, &total_size);
    // include_chunk(root, chunk2, &total_size);

    // printf("%zu", total_size);
    // include_chunk(root, )

    // printf("%i\n", root->childrens[(int)'s' - 97]->childrens[(int)'t' - 97]->positions[0]);
    // printf("%i\n", root->childrens[(int)'m' - 97]->positions[1]);
    return 0;
}
