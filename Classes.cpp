#define NOMINMAX         
#include <iostream>
#include "randomness.h"
#include "Classes.h"
#include <curses.h>
#include <cmath>
#include <algorithm>
#include <windows.h>

//----------------------- PIECE -----------------------------

// Setters
void piece::setx(int x) {
    this->x = x;
}

void piece::sety(int y) {
    this->y = y;
}

void piece::setrotation(int rotation) {
    this->rotation = rotation;
}

void piece::setpiecetype(char piecetype) {
    this->piecetype = piecetype;
}

void piece::setoffsets(const int offsets[4][2]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            this->offsets[i][j] = offsets[i][j];
        }
    }
}

// Getters
int piece::getx() {
    return x;
}

int piece::gety() {
    return y;
}

int piece::getrotation() {
    return rotation;
}

char piece::getpiecetype() {
    return piecetype;
}

int piece::getoffsets(int pos1, int pos2) {
    return offsets[pos1][pos2];
}

const int (*piece::getoffset())[2] {
    return offsets;
}

// ACTUAL functions
void piece::rotate() {
    if (boardContext == nullptr) return;

    boardContext->clearpiece();

    int newOffsets[4][2];
    for (int i = 0; i < 4; i++) {
        newOffsets[i][0] = offsets[i][1];
        newOffsets[i][1] = -offsets[i][0];
    }

    bool valid = true;
    for (int i = 0; i < 4; i++) {
        if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 1 || boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 4) {
            valid = false;
            break;
        }
        else if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 2)
        {
            //int tempx = x;
            while (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 2)
            {
                if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 1 && boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 4) {
                    x = x - 1;
                }
                else
                {
                    break;
                }
            }
        }
        else if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 3)
        {
            //int tempx = x;
            while (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) == 3)
            {
                if (boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 1 && boardContext->validmove(y + newOffsets[i][0], x + newOffsets[i][1]) != 4) {
                    x = x + 1;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if (valid) {
        for (int i = 0; i < 4; i++) {
            offsets[i][0] = newOffsets[i][0];
            offsets[i][1] = newOffsets[i][1];
        }
    }

    boardContext->update();
}

int piece::falldown() {
    if (boardContext == nullptr) return 0;
    boardContext->clearpiece();

    bool valid = true;
    for (int i = 0; i < 4; i++) {
        if (boardContext->validmove(y + offsets[i][0] + 1, x + offsets[i][1]) != 0) {
            valid = false;
            break;
        }
    }

    if (valid) {
        y = y + 1;
    }

    boardContext->update();
    return valid ? 1 : 2;
}

void piece::moveleft() {
    if (boardContext == nullptr) return;

    boardContext->clearpiece();

    bool valid = true;
    for (int i = 0; i < 4; i++) {
        if (boardContext->validmove(y + offsets[i][0], (x - 1) + offsets[i][1]) != 0) {
            valid = false;
            break;
        }
    }

    if (valid) {
        x = x - 1;
    }

    boardContext->update();
}

void piece::moveright() {
    if (boardContext == nullptr) return;

    boardContext->clearpiece();

    bool valid = true;
    for (int i = 0; i < 4; i++) {
        if (boardContext->validmove(y + offsets[i][0], (x + 1) + offsets[i][1]) != 0) {
            valid = false;
            break;
        }
    }

    if (valid) {
        x = x + 1;
    }

    boardContext->update();
}


//-------------------------- BOARD -----------------------------
board::board() {
    for (int i = 0; i < inaltime; i++) {
        for (int j = 0; j < latime; j++) {
            ecran[i][j] = ' ';
        }
    }
    piesaactiva = nullptr;
    urmatoareapiesa = nullptr;
}

bool board::isActivePieceCell(int row, int col) {
    if (piesaactiva == nullptr)
        return false;
    for (int i = 0; i < 4; i++) {
        int cellRow = piesaactiva->gety() + piesaactiva->getoffsets(i, 0);
        int cellCol = piesaactiva->getx() + piesaactiva->getoffsets(i, 1);
        if (cellRow == row && cellCol == col)
            return true;
    }
    return false;
}

void board::printecran() {
    mvprintw(2, 0, " __________");

    for (int i = 3; i < inaltime; i++) {
        mvprintw(i, 0, "|");
        for (int j = 0; j < latime; j++) {
            int screenX = j + 1;
            char cell = ecran[i][j];
            if (cell == cellval) {
                int colorPair = (isActivePieceCell(i, j) && piesaactiva != nullptr)
                    ? piesaactiva->getcolor()
                    : ecran_color[i][j];
                if (colorPair <= 0)
                    colorPair = 8; 
                attron(COLOR_PAIR(colorPair));
                mvprintw(i, screenX, "%c", cellval);
                attroff(COLOR_PAIR(colorPair));
            }
            else {
                mvprintw(i, screenX, " ");
            }
        }
        mvprintw(i, latime + 1, "|");
    }

    mvprintw(inaltime, 0, " ----------");

    mvprintw(0, 0, "LEVEL:");
    mvprintw(1, 5, "%d", managerContext->getlevel());
    mvprintw(0, latime + 3, "SCORE:");
    mvprintw(1, latime + 10, "%d", managerContext->getscore());

    refresh();
}

void board::printurmatoare() {
    if (urmatoareapiesa != nullptr) {
        mvprintw(inaltime / 2 - 3, latime + 8, "NEXT:");
        mvprintw(inaltime / 2 - 2, latime + 8, "_______");
        mvprintw(inaltime / 2 - 1, latime + 7, "|");
        mvprintw(inaltime / 2 - 1, latime + 15, "|");
        mvprintw(inaltime / 2, latime + 7, "|");
        mvprintw(inaltime / 2, latime + 15, "|");
        mvprintw(inaltime / 2 + 1, latime + 7, "|");
        mvprintw(inaltime / 2 + 1, latime + 15, "|");
        mvprintw(inaltime / 2 + 2, latime + 7, "|");
        mvprintw(inaltime / 2 + 2, latime + 15, "|");
        mvprintw(inaltime / 2 + 3, latime + 7, "|");
        mvprintw(inaltime / 2 + 3, latime + 15, "|");
        mvprintw(inaltime / 2 + 4, latime + 7, "|");
        mvprintw(inaltime / 2 + 4, latime + 15, "|");
        mvprintw(inaltime / 2 + 4, latime + 8, "_______");

        for (int i = 0; i < 4; i++) {
            int previewY = inaltime / 2 + urmatoareapiesa->getoffsets(i, 0);
            int previewX = latime + 10 + urmatoareapiesa->getoffsets(i, 1);
            attron(COLOR_PAIR(urmatoareapiesa->getcolor()));
            mvprintw(previewY, previewX, "%c", cellval);
            attroff(COLOR_PAIR(urmatoareapiesa->getcolor()));
        }
    }
}


void board::setecran(char value, int pos1, int pos2) {
    if (pos1 >= 0 && pos1 < inaltime && pos2 >= 0 && pos2 < latime)
        ecran[pos1][pos2] = value;
}

char board::getecran(int pos1, int pos2) {
    if (pos1 >= 0 && pos1 < inaltime && pos2 >= 0 && pos2 < latime)
        return ecran[pos1][pos2];
    return '\0';
}

piece* board::getpiesaactiva() {
    return piesaactiva;
}

void board::newpiece() {
    if (urmatoareapiesa != nullptr) {
        piesaactiva = urmatoareapiesa;
        urmatoareapiesa = nullptr; 
    }
    else {
        piesaactiva = new piece();
        piesaactiva->setBoard(this);
        char type = getNextPiece();
        switch (type) {
        case 'I': piesaactiva->setoffsets(offsetI); piesaactiva->setcolor(1); break;
        case 'O': piesaactiva->setoffsets(offsetO); piesaactiva->setcolor(2); break;
        case 'T': piesaactiva->setoffsets(offsetT); piesaactiva->setcolor(3); break;
        case 'S': piesaactiva->setoffsets(offsetS); piesaactiva->setcolor(4); break;
        case 'Z': piesaactiva->setoffsets(offsetZ); piesaactiva->setcolor(5); break;
        case 'L': piesaactiva->setoffsets(offsetL); piesaactiva->setcolor(6); break;
        case 'J': piesaactiva->setoffsets(offsetJ); piesaactiva->setcolor(7); break;
        default: break;
        }
    }

    
    if (urmatoareapiesa != nullptr) {
        delete urmatoareapiesa;
        urmatoareapiesa = nullptr;
    }

    urmatoareapiesa = new piece();
    urmatoareapiesa->setBoard(this);
    char type2 = getNextPiece();
    switch (type2) {
    case 'I': urmatoareapiesa->setoffsets(offsetI);urmatoareapiesa->setcolor(1); break;
    case 'O': urmatoareapiesa->setoffsets(offsetO);urmatoareapiesa->setcolor(2); break;
    case 'T': urmatoareapiesa->setoffsets(offsetT);urmatoareapiesa->setcolor(3); break;
    case 'S': urmatoareapiesa->setoffsets(offsetS);urmatoareapiesa->setcolor(4); break;
    case 'Z': urmatoareapiesa->setoffsets(offsetZ);urmatoareapiesa->setcolor(5); break;
    case 'L': urmatoareapiesa->setoffsets(offsetL);urmatoareapiesa->setcolor(6); break;
    case 'J': urmatoareapiesa->setoffsets(offsetJ);urmatoareapiesa->setcolor(7); break;
    default: break;
    }

    update();
}

void board::setCell(int row, int col, char c, int color) {
    if (row >= 0 && row < inaltime && col >= 0 && col < latime) {
        ecran[row][col] = c;
        if (c == cellval)
            ecran_color[row][col] = color;
        else
            ecran_color[row][col] = 0;
    }
}

void board::clearpiece() {
    if (piesaactiva == nullptr)
        return;

    for (int i = 0; i < 4; i++) {
        setecran(' ', piesaactiva->gety() + piesaactiva->getoffsets(i, 0),
            piesaactiva->getx() + piesaactiva->getoffsets(i, 1));
    }
}

piece* board::geturmatoareapiesa()
{
    return urmatoareapiesa;
}

void board::deletepiesaactiva() {
    if (piesaactiva != nullptr) {
        delete piesaactiva;
        piesaactiva = nullptr;
    }
}

board::~board() {
    if (piesaactiva != nullptr)
        delete piesaactiva;
    if (urmatoareapiesa != nullptr)
        delete urmatoareapiesa;
}

int board::validmove(int y, int x) {
    if (y >= inaltime || y < 0)
        return 1;
    if (x >= latime)
        return 2;
    if (x < 0)
        return 3;
    if (getecran(y, x) != ' ')
        return 4;
    return 0;
}

void board::update() {
    if (piesaactiva == nullptr)
        return;

    for (int i = 0; i < 4; i++) {
        int row = piesaactiva->gety() + piesaactiva->getoffsets(i, 0);
        int col = piesaactiva->getx() + piesaactiva->getoffsets(i, 1);
        setCell(row, col, cellval, piesaactiva->getcolor());
    }
}

void board::checkclear() {
    for (int i = inaltime - 1; i >= 0; i--) {
        bool full = true;

        for (int j = 0; j < latime; j++) {
            if (ecran[i][j] != cellval) {
                full = false;
                break;
            }
        }

        if (full) {
            managerContext->setscore(managerContext->getscore() + 100);
            managerContext->checkscore();

            for (int k = i; k > 0; k--) {
                for (int j = 0; j < latime; j++) {
                    ecran[k][j] = ecran[k - 1][j];
                }
            }

            Sleep(framerate);

            for (int j = 0; j < latime; j++) {
                ecran[0][j] = ' ';
            }
            printecran();
            i++;
        }
    }
}

bool board::checkloss() {
    for (int i = 0; i < latime; i++) {
        if (ecran[3][i] == cellval)
            return true;
    }
    return false;
}


//------------------------ GAMEMANAGER ----------------
void gamemanager::mainloop() {
    nodelay(stdscr, TRUE);

    const unsigned int targetFrameDelay = framerate; 

    double delay_frames = std::max(2.0, std::floor(20 - (2 * level)));
    double desired_delay_ms = delay_frames * framerate;
    ag = desired_delay_ms / (tickspeed * framerate);

    laberson = new board;
    laberson->newpiece();
    laberson->setManager(this);

    unsigned long lastRender = GetTickCount();
    unsigned long lastUpdate = GetTickCount();
    int count = 0;

    while (true) {
        unsigned long now = GetTickCount();

        if (now - lastRender >= targetFrameDelay) {
            int ch = getch();
            switch (ch) {
            case KEY_UP:
                if (laberson->getpiesaactiva())
                    laberson->getpiesaactiva()->rotate();
                break;
            case KEY_LEFT:
                if (laberson->getpiesaactiva())
                    laberson->getpiesaactiva()->moveleft();
                break;
            case KEY_RIGHT:
                if (laberson->getpiesaactiva())
                    laberson->getpiesaactiva()->moveright();
                break;
            default:
                break;
            }
            laberson->printecran();
            laberson->printurmatoare();
            lastRender = now;
        }

        int fallDelayMultiplier = (GetAsyncKeyState(VK_DOWN) & 0x8000) ? 5 : 1;

        if (now - lastUpdate >= ((tickspeed * ag) / fallDelayMultiplier) * targetFrameDelay) {
            if (laberson->getpiesaactiva()->falldown() == 2) {
                count++;
                if (count > coyoteticks) {
                    for (int i = 0; i < 4; i++)
                         {
                             mvprintw(inaltime / 2 + laberson->geturmatoareapiesa()->getoffsets(i, 0), latime + 10 + laberson->geturmatoareapiesa()->getoffsets(i, 1), " ");
                             mvprintw(inaltime / 2 + laberson->getpiesaactiva()->getoffsets(i, 0), latime + 10 + laberson->getpiesaactiva()->getoffsets(i, 1), " ");
                        }
                    laberson->deletepiesaactiva();
                    if (laberson->checkloss()) {
                        for (int i = 0; i < 30; i++) {
                            int start_col = 0;
                            mvprintw(i, start_col, "                                                                                                             ");
                        }
                        refresh();
                        deleteboard();
                        break;
                    }
                    laberson->checkclear();
                    laberson->newpiece();
                    count = 0;
                }
            }
            else
            {
                count = 0;
            }

            lastUpdate = now;
        }

        napms(1);
    }
}

void gamemanager::setscore(int score) {
    this->score = score;
}

int gamemanager::getscore() {
    return score;
}

void gamemanager::checkscore() {
    if (score % nextscore == 0) {
        level++;
        double delay_frames = std::max(2.0, std::floor(20 - (2 * level)));
        double desired_delay_ms = delay_frames * framerate;
        ag = desired_delay_ms / (tickspeed * framerate);
    }
}

int gamemanager::getlevel() {
    return level;
}

void gamemanager::startscreen() {
    int row, col;
    getmaxyx(stdscr, row, col);

    const char* startScreen[] = {
        " _________     __________     _________     ______      ________      _______  ",
        "|         |   |      ____|   |         |   |  ___ \\    |        |    /  ____/  ",
        "|__     __|   |     |___     |__     __|   | |___| |   |__    __|   /  /___   ",
        "   |   |      |      ___|       |   |      |      /       |  |      \\_____ \\   ",
        "   |   |      |     |____       |   |      |  ||  |     __|  |__   ___    \\ \\  ",
        "   |   |      |         |       |   |      |  | \\  \\   |        |  \\  \\___/ / ",
        "   |___|      |_________|       |___|      |__|  \\__\\  |________|   \\______/  ",
        "",
        "",
        "                         Press up_arrow to start...",
    };

    int numLines = sizeof(startScreen) / sizeof(startScreen[0]);
    int start_row = (row - numLines) / 2;

    for (int i = 0; i < numLines; i++) {
        int start_col = 20;
        mvprintw(start_row + i, start_col, "%s", startScreen[i]);
    }
    mvprintw(23, 23, "controls: left/right arrow to move sideways \n                              up arrow to rotate \n                              down arrow to fall faster\n");

    refresh();
    int count = 0;
    while (getch() != KEY_UP) {
        Sleep(framerate * 2);
        count++;
        if (count >= 3 && count < 6) {
            int start_col = 20;
            mvprintw(start_row + numLines - 1, start_col, "                         Press up_arrow to start    ");
            refresh();
        }
        else if (count >= 6 && count < 9) {
            int start_col = 20;
            mvprintw(start_row + numLines - 1, start_col, "                         Press up_arrow to start...");
        }
        else if (count >= 9) {
            count = 0;
        }
        refresh();
    }

    for (int i = 0; i < numLines+6; i++) {
        int start_col = 20;
        mvprintw(start_row + i, start_col, "                                                                                     ");
    }

    refresh();
}

void gamemanager::losescreen() {
    int row, col;
    getmaxyx(stdscr, row, col);

    const char* startScreen[] = {
        " __             _______       ________       _______     ______        _______        ______        __    __",
        "|  |           /  ___  \\     |  ____  \\     |   ____|   |  ___ \\      /  ____/       /  __  \\      |  \\  |  |",
        "|  |          |  |   |  |    | |____|  |    |  |___     | |___| |    /  /___        /  |  |  \\     |   \\ |  |",
        "|  |          |  |___|  |    |    ____/     |   ___|    |      /     \\_____  \\     |   |  |   |    |    \\|  |",
        "|  |_____     |   ___   |    |  ____  \\     |  |____    |  |\\  \\      __    \\ \\    |   |  |   |    |  |\\    |",
        "|        |    |  |   |  |    | |____|  |    |      |    |  | \\  \\    \\  \\___/ /     \\  |__|  /     |  | \\   |",
        "|________|    |__|   |__|    |________/     |______|    |__|  \\__\\    \\______/       \\______/      |__|  \\__|",
        "",
        "",
        "",
        "                                    * Press up_arrow to start again",
        "",
        "                                     * Press left_arrow to quit"
    };

    int numLines = sizeof(startScreen) / sizeof(startScreen[0]);
    int start_row = (row - numLines) / 2;

    for (int i = 0; i < numLines; i++) {
        int start_col = 6;
        mvprintw(start_row - 3 + i, start_col, "%s", startScreen[i]);
    }
    refresh();
    int count = 0;
    int ch;
    while (true) {
        ch = getch();
        if (ch == KEY_UP) {
            break;
        }
        else if (ch == KEY_LEFT) {
            running = false;
            break;
        }
        napms(framerate);
    }
    for (int i = 0; i < numLines + 3; i++) {
        int start_col = 6;
        mvprintw(start_row - 5 + i, start_col, "                                                                                                             ");
    }
    refresh();
}

void gamemanager::deleteboard() {
    if (laberson != nullptr) {
        delete laberson;
        laberson = nullptr;
    }
}

bool gamemanager::getrunning() {
    return running;
}

gamemanager::~gamemanager() {
    if (laberson != nullptr)
        delete laberson;
}
