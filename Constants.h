#pragma once
#include <vector>

const int framerate = 42;
const int inaltime = 23;
const int latime = 10;
const double tickspeed = 7;
const int nextscore = 500;
const char cellval = '#';
const int coyoteticks = 1;

// FORME
// I

const int offsetI[4][2] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};

// O

const int offsetO[4][2] = { {0,0}, {0, 1}, {1, 0}, {1, 1} };

// T 

const int offsetT[4][2] = { {0,0}, {0,1}, {0, 2}, {1,1} };

// S

const int offsetS[4][2] = { {0, 1}, {0, 2}, {1, 0}, {1, 1} };

// Z 

const int offsetZ[4][2] = {{0, 0}, {0, 1}, {1,1}, {1,2}};

// J 

const int offsetL[4][2] = { {0,0}, {1,0}, {2,0}, {2,1} };

// L 

const int offsetJ[4][2] = { {0,1}, {1,1}, {2, 0}, {2,1} };