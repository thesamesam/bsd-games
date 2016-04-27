// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

// Let the player play the current level
void play_level (void)
{
    move(My_pos.y, My_pos.x);
    addch(PLAYER);
    refresh();
    for (COORD* cp = Robots; cp < &Robots[MAXROBOTS]; ++cp) {
	if (cp->y < 0)
	    continue;
	move(cp->y, cp->x);
	addch(ROBOT);
    }
    refresh();
#ifndef NDEBUG
    standout();
    move(Min.y, Min.x);
    addch(inch());
    move(Max.y, Max.x);
    addch(inch());
    standend();
#endif				// DEBUG
    setjmp(End_move);
    flush_in();
    while (!Dead && Num_robots > 0) {
	move(My_pos.y, My_pos.x);
	if (!jumping())
	    refresh();
	get_move();
	if (Real_time)
	    alarm(0);
	if (Field[My_pos.y][My_pos.x] != 0)
	    Dead = true;
	if (!Dead)
	    move_robots(false);
	if (Was_bonus) {
	    move(Y_PROMPT, X_PROMPT);
	    clrtoeol();
	    move(Y_PROMPT + 1, X_PROMPT);
	    clrtoeol();
	    Was_bonus = false;
	}
    }

    // if the player didn't die, add on the possible bonuses

    if (!Dead) {
	Was_bonus = false;

	if (Level == Start_level && Start_level > 1) {
	    move(Y_PROMPT, X_PROMPT);
	    printw("Advance bonus: %d", S_BONUS);
	    refresh();
	    add_score(S_BONUS);
	    Was_bonus = true;
	}

	if (Wait_bonus != 0) {
	    if (!Was_bonus)
		move(Y_PROMPT, X_PROMPT);
	    else
		move(Y_PROMPT + 1, X_PROMPT);
	    printw("Wait bonus: %d", Wait_bonus);
	    refresh();
	    add_score(Wait_bonus);
	    Was_bonus = true;
	}
    }
}
