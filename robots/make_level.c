// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

// Make the current level
void make_level (void)
{
    int i;
    COORD *cp;
    int x;

    reset_count();
    for (i = 1; i < Y_FIELDSIZE; i++)
	for (x = 1; x < X_FIELDSIZE; x++)
	    if (Field[i][x] != 0)
		mvaddch(i, x, ' ');
    if (My_pos.y > 0)
	mvaddch(My_pos.y, My_pos.x, ' ');

    Waiting = false;
    Wait_bonus = 0;
    leaveok(stdscr, false);
    for (cp = Robots; cp < &Robots[MAXROBOTS]; cp++)
	cp->y = -1;
    My_pos.y = -1;

    memset(Field, 0, sizeof Field);
    Min.y = Y_FIELDSIZE;
    Min.x = X_FIELDSIZE;
    Max.y = 0;
    Max.x = 0;
    if ((i = Level * 10) > MAXROBOTS)
	i = MAXROBOTS;
    Num_robots = i;
    memset(Scrap, 0, sizeof(Scrap[0]) * MAXROBOTS);
    Num_scrap = 0;
    while (i-- > 0) {
	cp = rnd_pos();
	Robots[i] = *cp;
	Field[cp->y][cp->x]++;
	if (cp->y < Min.y)
	    Min.y = cp->y;
	if (cp->x < Min.x)
	    Min.x = cp->x;
	if (cp->y > Max.y)
	    Max.y = cp->y;
	if (cp->x > Max.x)
	    Max.x = cp->x;
    }
    My_pos = *rnd_pos();
    refresh();
}
