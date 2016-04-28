// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#include "include.h"

int clck = 0, safe_planes = 0, start_time = 0, test_mode = 0;

FILE *filein = NULL, *fileout = NULL;

const C_SCREEN* sp = NULL;

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
