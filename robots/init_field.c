// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

static int telx = 0;
static int tely = 0;

// init_field:
//      Lay down the initial pattern whih is constant across all levels,
//      and initialize all the global variables.
void init_field (void)
{
    int i;
    static bool first = true;
    static const char *const desc[] = {
	"Directions:",
	"",
	"y k u",
	" \\|/",
	"h- -l",
	" /|\\",
	"b j n",
	"",
	"Commands:",
	"",
	"w:  wait for end",
	"t:  teleport",
	"q:  quit",
	"^L: redraw screen",
	"",
	"Legend:",
	"",
	"+:  robot",
	"*:  junk heap",
	"@:  you",
	"",
	"Score: 0",
	NULL
    };

    Dead = false;
    Waiting = false;
    Score = 0;

    erase();
    move(0, 0);
    addch('+');
    for (i = 1; i < Y_FIELDSIZE; i++) {
	move(i, 0);
	addch('|');
    }
    move(Y_FIELDSIZE, 0);
    addch('+');
    for (i = 1; i < X_FIELDSIZE; i++)
	addch('-');
    addch('+');
    if (first)
	refresh();
    move(0, 1);
    for (i = 1; i < X_FIELDSIZE; i++)
	addch('-');
    addch('+');
    for (i = 1; i < Y_FIELDSIZE; i++) {
	move(i, X_FIELDSIZE);
	addch('|');
    }
    if (first)
	refresh();
    for (i = 0; desc[i] != NULL; i++) {
	move(i, X_FIELDSIZE + 2);
	addstr(desc[i]);
    }
    telx = X_FIELDSIZE + 2;
    tely = i;
    if (first)
	refresh();
    first = false;
#ifdef	FANCY
    if (Pattern_roll)
	Next_move = &Move_list[-1];
#endif
}

void telmsg (int on)
{
    move(tely, telx);
    addstr(on ? "Teleport!" : "         ");
}
