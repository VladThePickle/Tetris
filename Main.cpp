
#include <curses.h>
#include <iostream>
#include "Classes.h"
#include "randomness.h"
#include "Constants.h"

int main() {

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();


    init_pair(1, COLOR_CYAN, COLOR_BLACK);  // I 
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // O 
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);  // T 
    init_pair(4, COLOR_GREEN, COLOR_BLACK);  // S 
    init_pair(5, COLOR_RED, COLOR_BLACK);  // Z 
    init_pair(6, COLOR_BLUE, COLOR_BLACK);  // J 
    init_pair(7, COLOR_WHITE, COLOR_BLACK);  // L


    while (true) {
        gamemanager tetris; 
        tetris.startscreen();
        tetris.mainloop();
        tetris.losescreen();

        if (!tetris.getrunning()) {
            break;
        }
    }
    endwin();
    return 0;
}
