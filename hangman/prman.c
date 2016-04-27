// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"

// Print out the man appropriately for the give number of incorrect guesses.
void prman(void)
{
    int i;
    for (i = 0; i < Errors; ++i)
	mvaddch(Err_pos[i].y, Err_pos[i].x, Err_pos[i].ch);
    for (; i < MAXERRS; ++i)
	mvaddch(Err_pos[i].y, Err_pos[i].x, ' ');
}
