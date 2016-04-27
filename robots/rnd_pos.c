// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

#define	IS_SAME(p,y,x)	((p).y != -1 && (p).y == y && (p).x == x)

// Pick a random, unoccupied position
COORD* rnd_pos (void)
{
    static COORD pos;
    static int call = 0;
    do {
	pos.y = rnd(Y_FIELDSIZE - 1) + 1;
	pos.x = rnd(X_FIELDSIZE - 1) + 1;
	refresh();
    } while (Field[pos.y][pos.x] != 0);
    ++call;
    return &pos;
}

int rnd (int range)
{
    return rand() % range;
}
