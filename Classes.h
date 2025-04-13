#pragma once

#include <vector>
#include "Constants.h"

class board;
class gamemanager;

class piece {
private:
    int x{ 3 };
    std::vector<std::vector<char>> mat;
    int y{ 3 };
    int rotation{ 0 };
    char piecetype{ 'I' };
    int offsets[4][2];
    board* boardContext{ nullptr };  

public:
    // Constructor
    piece() = default;

    // Setters
    void setx(int x);
    void sety(int y);
    void setrotation(int rotation);
    void setpiecetype(char piecetype);
    void setoffsets(const int offsets[4][2]);
    void setBoard(board* b) { boardContext = b; }  
    int getoffsets(int pos1, int pos2);
    const int (*getoffset())[2];


    // Getters
    int getx();
    int gety();
    int getrotation();
    char getpiecetype();

    // ACTUAL functions
    void rotate();
    int falldown();
    void moveleft();
    void moveright();
};

class board {
private:
    char ecran[inaltime][latime];
    piece* piesaactiva;
    piece* urmatoareapiesa{ nullptr };
    gamemanager* managerContext{ nullptr };
public:
    // Constructor
    board();

    // Functions
    void printecran();
    void setecran(char value, int pos1, int pos2);
    char getecran(int pos1, int pos2);
    void setManager(gamemanager* b) { managerContext = b; };
    piece* getpiesaactiva();
    void update();
    void newpiece();
    bool validmove(int y, int x);
    void clearpiece();
    piece* geturmatoareapiesa();
    bool checkloss();
    void checkclear();
    void printurmatoare();
    void deletepiesaactiva();
    ~board();
};


class gamemanager
{
private:
    int score{ 0 };
    double ag{ 1 };
    int level{ 1 };
    bool running{ true };
    board* laberson;
public:

    gamemanager() = default;
    void mainloop();
    void setscore(int score);
    int getscore();
    int getlevel();
    bool getrunning();
    void checkscore();
    void startscreen();
    void losescreen();
    void deleteboard();
    ~gamemanager();
};