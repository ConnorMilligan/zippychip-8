#include <curses.h>
#include <unistd.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#define FPS 30

typedef struct {
	unsigned short opcode;
	unsigned char memory[4096];
	unsigned char V[16];

	unsigned short I;
	unsigned short pc;

	unsigned char gfx[SCREEN_WIDTH * SCREEN_HEIGHT];

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16];
	unsigned short sp;

	unsigned char key[16];
    WINDOW *screen;
    bool draw_flag;
} Chip8;

void draw_menu();

Chip8 chip_init(WINDOW *win);
void chip_cycle(Chip8 *chip);

int main(int argc, char **argv) {
    int ch;
	unsigned short rows, cols;

    initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	nodelay(stdscr, TRUE);
    draw_menu();
    refresh();

	WINDOW *canvas = newwin(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1);
    Chip8 chip8 = chip_init(canvas);

	while (true) {
		getmaxyx(stdscr, rows, cols);

		if (rows < SCREEN_HEIGHT + 2 || cols < SCREEN_WIDTH + 2) {
			wclear(canvas);
			mvwprintw(canvas, 0, 0, "Terminal size is too small. Please resize the terminal.");
            
			wrefresh(canvas);
			ch = getch();
			if (ch == 'q') {
				break;
			}
			continue;
		}




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

Chip8 chip_init(WINDOW *win) {
    Chip8 chip;

	chip.opcode = 0;
    // clear memory
    for (int i = 0; i < 4096; i++) {
        chip.memory[i] = 0;
    }
    // clear registers & stack
    for (int i = 0; i < 16; i++) {
        chip.V[i] = 0;
        chip.stack[i] = 0;
    }

	chip.I = 0;
	chip.pc = 0x200;

    chip.delay_timer = 0;
	chip.sound_timer = 0;

    chip.sp = 0;
    chip.screen = win;

    chip.draw_flag = false;

    return chip;
}

void chip_cycle(Chip8 *chip) {
    // Combine first 4 bits for full opcode
    chip->opcode = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
    
    // Switch for instruction
    switch(chip->opcode & 0xF000) {

        case 0xA000: // ANNN: Sets I to the address NNN
            // Execute opcode
            chip->I = chip->opcode & 0x0FFF;
            chip->pc += 2;
            break;
        default:
            
    }

    // timers
    if(chip->delay_timer > 0)
        chip->delay_timer--;
 
    if(chip->sound_timer > 0) {
        if(chip->sound_timer == 1)
            printf("BEEP!\n");
        chip->sound_timer--;
    }  
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
