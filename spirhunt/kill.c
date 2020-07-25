// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"

// KILL KILL KILL !!!
//
// This file handles the killing off of almost anything.
//
// Handle a pirate's death
//
// The pirate at the sector given by the parameters is killed
// and removed from the pirate list. Notice that it is not
// removed from the event list; this is done later, when the
// the event is to be caught. Also, the time left is recomputed,
// and the game is won if that was the last pirate.
//
void kill_pirate (int ix, int iy)
{
    printf ("   *** pirate at %d,%d destroyed ***\n", ix, iy);

    // remove the scoundrel
    Now.pirates -= 1;
    Sect[ix][iy] = EMPTY;
    Quad[Ship.quadx][Ship.quady].pirates -= 1;
    // %%% IS THIS SAFE???? %%%
    Quad[Ship.quadx][Ship.quady].scanned -= 100;
    ++Game.pirates_killed;

    // find the pirate in the pirate list
    for (int i = 0; i < Etc.npirates; ++i) {
	if (ix == Etc.pirate[i].x && iy == Etc.pirate[i].y) {
	    // purge him from the list
	    Etc.npirates -= 1;
	    for (; i < Etc.npirates; ++i)
		Etc.pirate[i] = Etc.pirate[i + 1];
	    break;
	}
    }

    // find out if that was the last one
    if (Now.pirates <= 0)
	win();

    // recompute time left
    Now.time = Now.resource / Now.pirates;
}

// handle a starbase's death
void kill_starbase (int qx, int qy)
{
    struct quad* q = &Quad[qx][qy];
    if (q->bases <= 0)
	return;
    if (!device_damaged (SSRADIO)) {
	// then update starchart
	if (q->scanned < 1000)
	    q->scanned -= 10;
	else if (q->scanned > 1000)
	    q->scanned = -1;
    }
    q->bases = 0;
    Now.bases -= 1;
    struct xy* b = Now.base;
    for (;; ++b)
	if (qx == b->x && qy == b->y)
	    break;
    *b = Now.base[Now.bases];
    if (qx == Ship.quadx && qy == Ship.quady) {
	Sect[Etc.starbase.x][Etc.starbase.y] = EMPTY;
	if (Ship.cond == DOCKED)
	    undock(0);
	printf ("Starbase at %d,%d destroyed\n", Etc.starbase.x, Etc.starbase.y);
    } else {
	if (!device_damaged (SSRADIO)) {
	    printf ("Headquarters reports that the starbase in\n");
	    printf ("   quadrant %d,%d has been destroyed\n", qx, qy);
	} else
	    schedule (E_PATSB | E_GHOST, FLT_MAX, qx, qy, 0);
    }
}

// kill an inhabited starsystem
// quad coords if f == 0, else sector coords
// f != 0 -- this quad;  f < 0 -- your fault
void kill_starsystem (int x, int y, int f)
{
    struct quad *q;
    if (f) {
	// current quadrant
	q = &Quad[Ship.quadx][Ship.quady];
	Sect[x][y] = EMPTY;
	const char* name = systemname(q);
	if (!name)
	    return;
	printf ("Inhabited starsystem %s at %d,%d destroyed\n", name, x, y);
	if (f < 0)
	    Game.killinhab += 1;
    } else // different quadrant
	q = &Quad[x][y];
    if (q->qsystemname & Q_DISTRESSED) {
	// distressed starsystem
	struct event* e = &Event[q->qsystemname & Q_SYSTEM];
	printf ("Distress call for %s invalidated\n", Systemname[e->systemname]);
	unschedule (e);
    }
    q->qsystemname = 0;
    --q->stars;
}

// "kill" a distress call
// quadrant coordinates
// f - set if user is to be informed
void kill_distress_call (unsigned x, unsigned y, bool f)
{
    struct quad* q = &Quad[x][y];
    for (unsigned i = 0; i < MAXEVENTS; ++i) {
	struct event* e = &Event[i];
	if (e->x != x || e->y != y)
	    continue;
	if (e->evcode == E_PDESB && f) {
	    printf ("Distress call for starbase in %u,%u nullified\n", x, y);
	    unschedule (e);
	} else if (e->evcode == E_ENSLV || e->evcode == E_REPRO) {
	    if (f) {
		printf ("Distress call for %s in quadrant %u,%u nullified\n", Systemname[e->systemname], x, y);
		q->qsystemname = e->systemname;
		unschedule (e);
	    } else
		e->evcode |= E_GHOST;
	}
    }
}

// CAUSE A NOVA TO OCCUR
//
// A nova occurs. It is the result of having a star hit with a torpedo.
// There are several things which may happen. The star may not be affected.
// It may go nova. It may turn into a black hole. And it may go supernova.
//
// Stars that go nova cause stars which surround them to undergo the same
// probabilistic process. Pirates next to them are destroyed. And if the
// starship is next to it, it gets zapped. If the zap is too much, it
// gets destroyed.
//
void nova (int x, int y)
{
    if (Sect[x][y] != STAR || Quad[Ship.quadx][Ship.quady].stars < 0)
	return;
    if (nrand(100) < 15) {
	printf ("Star at %d,%d failed to nova.\n", x, y);
	return;
    }
    if (nrand(100) < 5) {
	snova(x, y);
	return;
    }
    printf ("Star at %d,%d gone nova\n", x, y);

    if (nrand(4) != 0)
	Sect[x][y] = EMPTY;
    else {
	Sect[x][y] = HOLE;
	Quad[Ship.quadx][Ship.quady].holes += 1;
    }
    Quad[Ship.quadx][Ship.quady].stars -= 1;
    ++Game.stars_killed;
    for (int i = x - 1; i <= x + 1; ++i) {
	if (i < 0 || i >= NSECTS)
	    continue;
	for (int j = y - 1; j <= y + 1; ++j) {
	    if (j < 0 || j >= NSECTS)
		continue;
	    switch (Sect[i][j]) {
		case EMPTY:
		case HOLE:	break;
		case PIRATE:	kill_pirate (i, j); break;
		case STAR:	nova (i, j); break;
		case INHABIT:	kill_starsystem (i, j, -1); break;
		case BASE:	kill_starbase (i, j); ++Game.bases_killed; break;
		case YOURSHIP: {
		    int se = 2000;
		    if (Ship.shldup) {
			if (Ship.shield >= se) {
			    Ship.shield -= se;
			    se = 0;
			} else {
			    se -= Ship.shield;
			    Ship.shield = 0;
			}
		    }
		    Ship.energy -= se;
		    if (Ship.energy <= 0)
			lose(L_SUICID);
		    break; }

		default:
		    printf ("Unknown object %c at %u,%u destroyed\n", Sect[i][j], i, j);
		    Sect[i][j] = EMPTY;
		    break;
	    }
	}
    }
}

// CAUSE SUPERNOVA TO OCCUR
//
// A supernova occurs. If 'ix' < 0, a random quadrant is chosen;
// otherwise, the current quadrant is taken, and (ix, iy) give
// the sector quadrants of the star which is blowing up.
//
// If the supernova turns out to be in the quadrant you are in,
// you go into "emergency override mode", which tries to get you
// out of the quadrant as fast as possible. However, if you
// don't have enough fuel, or if you by chance run into something,
// or some such thing, you blow up anyway. Oh yeh, if you are
// within two sectors of the star, there is nothing that can
// be done for you.
//
// When a star has gone supernova, the quadrant becomes uninhab-
// itable for the rest of eternity, i.e., the game. If you ever
// try stopping in such a quadrant, you will go into emergency
// override mode.

void snova (int x, int y)
{
    int qx, qy;
    int ix, iy = 0;
    int f;
    int dx, dy;
    int n;
    struct quad *q;

    f = 0;
    ix = x;
    if (ix < 0) {
	// choose a quadrant
	while (1) {
	    qx = nrand(NQUADS);
	    qy = nrand(NQUADS);
	    q = &Quad[qx][qy];
	    if (q->stars > 0)
		break;
	}
	if (Ship.quadx == qx && Ship.quady == qy) {
	    // select a particular star
	    n = nrand(q->stars);
	    for (ix = 0; ix < NSECTS; ++ix) {
		for (iy = 0; iy < NSECTS; ++iy)
		    if (Sect[ix][iy] == STAR || Sect[ix][iy] == INHABIT)
			if ((n -= 1) <= 0)
			    break;
		if (n <= 0)
		    break;
	    }
	    f = 1;
	}
    } else {
	// current quadrant
	iy = y;
	qx = Ship.quadx;
	qy = Ship.quady;
	q = &Quad[qx][qy];
	f = 1;
    }
    if (f) {
	// supernova is in your quadrant
	printf ("\nRED ALERT: supernova occuring at %d,%d\n", ix, iy);
	dx = ix - Ship.sectx;
	dy = iy - Ship.secty;
	if (dx * dx + dy * dy <= 2) {
	    printf ("***  Emergency override attem");
	    sleep(1);
	    printf ("\n");
	    lose(L_SNOVA);
	}
	q->scanned = 1000;
    } else {
	if (!device_damaged (SSRADIO)) {
	    q->scanned = 1000;
	    printf ("\nCaptain, Command reports a supernova\n");
	    printf ("  in quadrant %d,%d. Caution is advised\n", qx, qy);
	}
    }

    // clear out the supernova'ed quadrant
    dx = q->pirates;
    dy = q->stars;
    Now.pirates -= dx;
    if (x >= 0) {
	// You caused the supernova
	Game.stars_killed += dy;
	if (q->bases)
	    kill_starbase (qx, qy);
	Game.pirates_killed += dx;
    } else if (q->bases)
	kill_starbase (qx, qy);
    kill_distress_call (qx, qy, (x >= 0));
    q->stars = -1;
    q->pirates = 0;
    if (Now.pirates <= 0) {
	printf ("Lucky devil, that supernova destroyed the last pirate\n");
	win();
    }
}
