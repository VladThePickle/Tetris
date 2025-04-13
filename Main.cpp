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
