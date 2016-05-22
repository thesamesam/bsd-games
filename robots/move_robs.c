// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

//      Move the robots around
void move_robots (int was_sig)
{
    COORD *rp;
    if (Real_time)
	signal(SIGALRM, move_robots);
#ifndef NDEBUG
    move(Min.y, Min.x);
    addch(inch());
    move(Max.y, Max.x);
    addch(inch());
#endif				// DEBUG
    for (rp = Robots; rp < &Robots[MAXROBOTS]; rp++) {
	if (rp->y < 0)
	    continue;
	mvaddch(rp->y, rp->x, ' ');
	Field[rp->y][rp->x]--;
	rp->y += sign(My_pos.y - rp->y);
	rp->x += sign(My_pos.x - rp->x);
	if (rp->y <= 0)
	    rp->y = 0;
	else if (rp->y >= Y_FIELDSIZE)
	    rp->y = Y_FIELDSIZE - 1;
	if (rp->x <= 0)
	    rp->x = 0;
	else if (rp->x >= X_FIELDSIZE)
	    rp->x = X_FIELDSIZE - 1;
	Field[rp->y][rp->x]++;
    }

    Min.y = Y_FIELDSIZE;
    Min.x = X_FIELDSIZE;
    Max.y = 0;
    Max.x = 0;
    for (rp = Robots; rp < &Robots[MAXROBOTS]; rp++)
	if (rp->y < 0)
	    continue;
	else if (rp->y == My_pos.y && rp->x == My_pos.x)
	    Dead = true;
	else if (Field[rp->y][rp->x] > 1) {
	    mvaddch(rp->y, rp->x, HEAP);
	    Scrap[Num_scrap++] = *rp;
	    rp->y = -1;
	    Num_robots--;
	    if (Waiting)
		Wait_bonus++;
	    add_score(ROB_SCORE);
	} else {
	    mvaddch(rp->y, rp->x, ROBOT);
	    if (rp->y < Min.y)
		Min.y = rp->y;
	    if (rp->x < Min.x)
		Min.x = rp->x;
	    if (rp->y > Max.y)
		Max.y = rp->y;
	    if (rp->x > Max.x)
		Max.x = rp->x;
	}

    if (was_sig) {
	refresh();
	if (Dead || Num_robots <= 0)
	    longjmp(End_move, 0);
    }
#ifndef NDEBUG
    standout();
    move(Min.y, Min.x);
    addch(inch());
    move(Max.y, Max.x);
    addch(inch());
    standend();
#endif				// DEBUG
    if (Real_time)
	alarm(3);
}

// Add a score to the overall point total
void add_score (int add)
{
    Score += add;
    move(Y_SCORE, X_SCORE);
    printw("%d", Score);
}
