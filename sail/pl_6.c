// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include "player.h"
#include <signal.h>

static int turned(void);

void repair(void)
{
    char c;
    char *repairs;
    struct shipspecs *ptr = mc;
    int count;

#define FIX(x, m) (m - ptr->x > count \
	? (ptr->x += count, count = 0) : (count -= m - ptr->x, ptr->x = m))

    if (repaired || loaded || fired || changed || turned()) {
	Msg("No hands free to repair");
	return;
    }
    c = sgetch("Repair (hull, guns, rigging)? ", (struct ship *) 0, 1);
    switch (c) {
	case 'h':
	    repairs = &mf->RH;
	    break;
	case 'g':
	    repairs = &mf->RG;
	    break;
	case 'r':
	    repairs = &mf->RR;
	    break;
	default:
	    Msg("Avast heaving!");
	    return;
    }
    if (++*repairs >= 3) {
	count = 2;
	switch (c) {
	    case 'h':{
		    int max = ptr->guns / 4;
		    if (ptr->hull < max) {
			FIX(hull, max);
			Write(W_HULL, ms, ptr->hull, 0, 0, 0);
		    }
		    break;
		}
	    case 'g':
		if (ptr->gunL < ptr->gunR) {
		    int max = ptr->guns / 5 - ptr->carL;
		    if (ptr->gunL < max) {
			FIX(gunL, max);
			Write(W_GUNL, ms, ptr->gunL, ptr->carL, 0, 0);
		    }
		} else {
		    int max = ptr->guns / 5 - ptr->carR;
		    if (ptr->gunR < max) {
			FIX(gunR, max);
			Write(W_GUNR, ms, ptr->gunR, ptr->carR, 0, 0);
		    }
		}
		break;
	    case 'r':
#define X 2
		if (ptr->rig4 >= 0 && ptr->rig4 < X) {
		    FIX(rig4, X);
		    Write(W_RIG4, ms, ptr->rig4, 0, 0, 0);
		}
		if (count && ptr->rig3 < X) {
		    FIX(rig3, X);
		    Write(W_RIG3, ms, ptr->rig3, 0, 0, 0);
		}
		if (count && ptr->rig2 < X) {
		    FIX(rig2, X);
		    Write(W_RIG2, ms, ptr->rig2, 0, 0, 0);
		}
		if (count && ptr->rig1 < X) {
		    FIX(rig1, X);
		    Write(W_RIG1, ms, ptr->rig1, 0, 0, 0);
		}
		break;
	}
	if (count == 2) {
	    Msg("Repairs completed.");
	    *repairs = 2;
	} else {
	    *repairs = 0;
	    blockalarm();
	    draw_stat();
	    unblockalarm();
	}
    }
    blockalarm();
    draw_slot();
    unblockalarm();
    repaired = 1;
}

static int turned(void)
{
    char *p;

    for (p = movebuf; *p; p++)
	if (*p == 'r' || *p == 'l')
	    return 1;
    return 0;
}

void loadplayer(void)
{
    char c;
    int loadL, loadR, ready, load;

    if (!mc->crew3) {
	Msg("Out of crew");
	return;
    }
    loadL = mf->loadL;
    loadR = mf->loadR;
    if (!loadL && !loadR) {
	c = sgetch("Load which broadside (left or right)? ", (struct ship *) 0, 1);
	if (c == 'r')
	    loadL = 1;
	else
	    loadR = 1;
    }
    if ((!loadL && loadR) || (loadL && !loadR)) {
	c = sgetch("Reload with (round, double, chain, grape)? ", (struct ship *) 0, 1);
	switch (c) {
	    case 'r':
		load = L_ROUND;
		ready = 0;
		break;
	    case 'd':
		load = L_DOUBLE;
		ready = R_DOUBLE;
		break;
	    case 'c':
		load = L_CHAIN;
		ready = 0;
		break;
	    case 'g':
		load = L_GRAPE;
		ready = 0;
		break;
	    default:
		Msg("Broadside not loaded.");
		return;
	}
	if (!loadR) {
	    mf->loadR = load;
	    mf->readyR = ready | R_LOADING;
	} else {
	    mf->loadL = load;
	    mf->readyL = ready | R_LOADING;
	}
	loaded = 1;
    }
}
