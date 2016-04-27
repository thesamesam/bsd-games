// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#include "include.h"

char GAMES[] = "Game_List";

int clck, safe_planes, start_time, test_mode;

const char *file;

FILE *filein, *fileout;

C_SCREEN screen, *sp = &screen;

LIST air, ground;

struct termios tty_start, tty_new;

DISPLACEMENT displacement[MAXDIR] = {
    {0, -1},
    {1, -1},
    {1, 0},
    {1, 1},
    {0, 1},
    {-1, 1},
    {-1, 0},
    {-1, -1}
};
