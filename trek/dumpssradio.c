// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "trek.h"

// output hidden distress calls
int dumpssradio(void)
{
    struct event *e;
    int chkrest = 0;
    for (int j = 0; j < MAXEVENTS; ++j) {
	e = &Event[j];
	// if it is not hidden, then just ignore it
	if ((e->evcode & E_HIDDEN) == 0)
	    continue;
	if (e->evcode & E_GHOST) {
	    unschedule(e);
	    printf("Starsystem %s in quadrant %d,%d is no longer distressed\n", systemname(&Quad[e->x][e->y]), e->x, e->y);
	    continue;
	}
	switch (e->evcode) {
	    case E_KDESB:
		printf("Starbase in quadrant %d,%d is under attack\n", e->x, e->y);
		++chkrest;
		break;
	    case E_ENSLV:
	    case E_REPRO:
		printf("Starsystem %s in quadrant %d,%d is distressed\n", systemname(&Quad[e->x][e->y]), e->x, e->y);
		++chkrest;
		break;
	}
    }
    return chkrest;
}
