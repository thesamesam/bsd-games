// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"

// RETRIEVE THE STARSYSTEM NAME
//
// Very straightforward, this routine just gets the starsystem
// name. It returns zero if none in the specified quadrant
// (which, by the way, is passed it).
//
// This routine knows all about such things as distressed
// starsystems, etc.

const char* systemname (const struct quad* q)
{
    int i = q->qsystemname;
    if (i & Q_DISTRESSED)
	i = Event[i & Q_SYSTEM].systemname;
    i &= Q_SYSTEM;
    if (i == 0)
	return 0;
    return Systemname[i];
}

// SHORT RANGE SENSOR SCAN
//
// A short range scan is taken of the current quadrant. If the flag 'f'
// is one, it is an "auto srscan". It does a status report and a srscan.
// If 'f' is -1, you get a status report only. If it is zero, you get a
// srscan and an optional status report. The status report is taken if
// you enter "srscan yes"; for all srscans thereafter you get a status
// report with your srscan until you type "srscan no". It defaults to on.
//
// The current quadrant is filled in on the computer chart.

void srscan (int f)
{
    int i, j;
    int statinfo;
    const char *s;
    int percent;
    struct quad *q = NULL;

    if (f >= 0 && !device_works(SRSCAN))
	return;
    if (f)
	statinfo = 1;
    else {
	if (!testnl())
	    Etc.statreport = getynpar("status report");
	statinfo = Etc.statreport;
    }
    if (f > 0)
	Etc.statreport = 1;
    if (f >= 0) {
	printf ("\nShort range sensor scan\n");
	q = &Quad[Ship.quadx][Ship.quady];
	q->scanned = q->pirates * 100 + q->bases * 10 + q->stars;
	printf ("  ");
	for (i = 0; i < NSECTS; ++i) {
	    printf ("%d ", i);
	}
	printf ("\n");
    }

    for (i = 0; i < NSECTS; ++i) {
	if (f >= 0) {
	    printf ("%d ", i);
	    for (j = 0; j < NSECTS; ++j)
		printf ("%c ", Sect[i][j]);
	    printf ("%d", i);
	    if (statinfo)
		printf ("   ");
	}
	static const char Color[4][8] = { "GREEN", "DOCKED", "YELLOW", "RED" };
	if (statinfo)
	    switch (i) {
		case 0:
		    printf ("date          %.2f", Now.date);
		    break;
		case 1:
		    printf ("condition     %s", Color[Ship.cond]);
		    if (Ship.cloaked)
			printf (", CLOAKED");
		    break;
		case 2:
		    printf ("position      %d,%d/%d,%d", Ship.quadx, Ship.quady, Ship.sectx, Ship.secty);
		    break;
		case 3:
		    printf ("warp factor   %.1f", Ship.warp);
		    break;
		case 4:
		    printf ("total energy  %d", Ship.energy);
		    break;
		case 5:
		    printf ("torpedoes     %d", Ship.torped);
		    break;
		case 6:
		    s = "down";
		    if (Ship.shldup)
			s = "up";
		    if (device_damaged (SHIELD))
			s = "damaged";
		    percent = 100.0 * Ship.shield / Param.shield;
		    printf ("shields       %s, %d%%", s, percent);
		    break;
		case 7:
		    printf ("pirates left	%d", Now.pirates);
		    break;
		case 8:
		    printf ("time left     %.2f", Now.time);
		    break;
		case 9:
		    printf ("life support  ");
		    if (device_damaged (LIFESUP)) {
			printf ("damaged, reserves = %.2f", Ship.reserves);
			break;
		    }
		    printf ("active");
		    break;
	    }
	printf ("\n");
    }
    if (f < 0) {
	printf ("current crew  %d\n", Ship.crew);
	printf ("brig space    %d\n", Ship.brigfree);
	printf ("pirate power %d\n", Param.piratepwr);
	return;
    }
    printf ("  ");
    for (i = 0; i < NSECTS; ++i)
	printf ("%d ", i);
    printf ("\n");

    if (q->qsystemname & Q_DISTRESSED)
	printf ("Distressed ");
    if (q->qsystemname)
	printf ("Starsystem %s\n", systemname(q));
}

// LONG RANGE OF SCANNERS
//
// A summary of the quadrants that surround you is printed. The
// hundreds digit is the number of pirates in the quadrant,
// the tens digit is the number of starbases, and the units digit
// is the number of stars. If the printout is "///" it means
// that that quadrant is rendered uninhabitable by a supernova.
// It also updates the "scanned" field of the quadrants it scans,
// for future use by the "chart" option of the computer.

void lrscan(int v UNUSED)
{
    if (!device_works (LRSCAN))
	return;
    printf ("Long range scan for quadrant %d,%d\n\n", Ship.quadx, Ship.quady);

    // print the header on top
    for (int j = Ship.quady - 1; j <= Ship.quady + 1; ++j) {
	if (j < 0 || j >= NQUADS)
	    printf ("      ");
	else
	    printf ("     %1d", j);
    }

    // scan the quadrants
    for (int i = Ship.quadx - 1; i <= Ship.quadx + 1; ++i) {
	printf ("\n  -------------------\n");
	if (i < 0 || i >= NQUADS) {
	    // negative energy barrier
	    printf ("  !  *  !  *  !  *  !");
	    continue;
	}

	// print the left hand margin
	printf ("%1d !", i);
	for (int j = Ship.quady - 1; j <= Ship.quady + 1; ++j) {
	    if (j < 0 || j >= NQUADS) {
		// negative energy barrier again
		printf ("  *  !");
		continue;
	    }
	    struct quad* q = &Quad[i][j];
	    if (q->stars < 0) {
		// supernova
		printf (" /// !");
		q->scanned = 1000;
		continue;
	    }
	    q->scanned = q->pirates * 100 + q->bases * 10 + q->stars;
	    printf (" %3d !", q->scanned);
	}
    }
    printf ("\n  -------------------\n");
}

// VISUAL SCAN
//
// A visual scan is made in a particular direction of three sectors
// in the general direction specified. This takes time, and pirates
// can attack you, so it should be done only when sensors are out.

void visual (int z UNUSED)
{
    // This struct[] has the delta x, delta y for particular directions
    static const struct xy Visdelta[11] = {
	{-1, -1},
	{-1, 0},
	{-1, 1},
	{0, 1},
	{1, 1},
	{1, 0},
	{1, -1},
	{0, -1},
	{-1, -1},
	{-1, 0},
	{-1, 1}
    };
    int co = getintpar ("direction");
    if (co < 0 || co > 360)
	return;
    co = (co + 22) / 45;
    const struct xy* v = &Visdelta[co];
    int ix = Ship.sectx + v->x;
    int iy = Ship.secty + v->y;
    if (ix < 0 || ix >= NSECTS || iy < 0 || iy >= NSECTS)
	co = '?';
    else
	co = Sect[ix][iy];
    printf ("%d,%d %c ", ix, iy, co);
    ++v;
    ix = Ship.sectx + v->x;
    iy = Ship.secty + v->y;
    if (ix < 0 || ix >= NSECTS || iy < 0 || iy >= NSECTS)
	co = '?';
    else
	co = Sect[ix][iy];
    printf ("%c ", co);
    ++v;
    ix = Ship.sectx + v->x;
    iy = Ship.secty + v->y;
    if (ix < 0 || ix >= NSECTS || iy < 0 || iy >= NSECTS)
	co = '?';
    else
	co = Sect[ix][iy];
    printf ("%c %d,%d\n", co, ix, iy);
    Move.time = 0.05;
    Move.free = 0;
}
