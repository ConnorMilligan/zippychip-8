#include <curses.h>
#include <unistd.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#define FPS 30

typedef struct {
	unsigned short opcode;
	unsigned char memory[4096];
	unsigned char V[16];

	unsigned short index;
	unsigned short program_counter;

	unsigned char gfx[SCREEN_WIDTH * SCREEN_HEIGHT];

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16];
	unsigned short stack_pointer

	unsigned char key[16];
} Chip8;

void draw_menu();

Chip8 chip_init();

int main(int argc, char **argv) {
    int ch;
	unsigned short rows, cols;

    initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, TRUE);

	while (true) {
		getmaxyx(stdscr, rows, cols);

		if (rows < SCREEN_HEIGHT + 2 || cols < SCREEN_WIDTH + 2) {
			clear();
			mvprintw(0, 0, "Terminal size is too small. Please resize the terminal.");
			refresh();
			ch = getch();
			if (ch == 'q') {
				break;
			}
			continue;
		}



		draw_menu();

		mvprintw(1, 1, "Rows: %d, Cols: %d", rows, cols);
		refresh();

		usleep(1000000 / FPS);


		ch = getch();
		if (ch == 'q') {
			break;
		}
	}

	endwin();

	return 0;
}



void draw_menu() {
	for (int i = 0; i < SCREEN_HEIGHT + 2; i++) {
		mvaddch(i, 0, ACS_VLINE);
		mvaddch(i, SCREEN_WIDTH + 1, ACS_VLINE);
	}
	for (int i = 0; i < SCREEN_WIDTH + 1; i++) {
		mvaddch(0, i, ACS_HLINE);
		mvaddch(SCREEN_HEIGHT + 1, i, ACS_HLINE);
	}

	mvaddch(0, 0, ACS_ULCORNER);
	mvaddch(0, SCREEN_WIDTH + 1, ACS_URCORNER);
	mvaddch(SCREEN_HEIGHT + 1, 0, ACS_LLCORNER);
	mvaddch(SCREEN_HEIGHT + 1, SCREEN_WIDTH + 1, ACS_LRCORNER);
}