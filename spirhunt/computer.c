// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"
#include <math.h>

// On-Board Computer
//
// chart -- print a star chart of the known galaxy. This includes
//	every quadrant that has ever had a long range or
//	a short range scan done of it, plus the location of
//	all starbases. This is of course updated by any sub-
//	space radio broadcasts (unless the radio is out).
//	The format is the same as that of a long range scan
//	except that ".1." indicates that a starbase exists
//	but we know nothing else.
//
// trajectory -- gives the course and distance to every know
//	pirate in the quadrant. Obviously this fails if the
//	short range scanners are out.
//
// course -- gives a course computation from whereever you are
//	to any specified location. If the course begins
//	with a slash, the current quadrant is taken.
//	Otherwise the input is quadrant and sector coordi-
//	nates of the target sector.
//
// move -- identical to course, except that the move is performed.
//
// score -- prints out the current score.
//
// pleff -- "PLaser EFFectiveness" at a given distance. Tells
//	you how much stuff you need to make it work.
//
// warpcost -- Gives you the cost in time and units to move for
//	a given distance under a given warp speed.
//
// impcost -- Same for the impulse engines.
//
// distresslist -- Gives a list of the currently known starsystems
//	or starbases which are distressed, together with their
//	quadrant coordinates.
//
// If a command is terminated with a semicolon, you remain in
// the computer; otherwise, you escape immediately to the main
// command processor.

//----------------------------------------------------------------------

// Course Calculation
//
// Computes and outputs the course and distance from position
// sqx,sqy/ssx,ssy to tqx,tqy/tsx,tsy.
//
static int kalc (int tqx, int tqy, int tsx, int tsy, float* dist)
{
    // normalize to quadrant distances
    float dx = (Ship.quadx + Ship.sectx / NSECTS) - (tqx + tsx / NSECTS);
    float dy = (tqy + tsy / NSECTS) - (Ship.quady + Ship.secty / NSECTS);

    // get the angle
    float angle = atan2(dy, dx);
    // make it 0 -> 2 pi
    if (angle < 0.0)
	angle += 6.283185307;
    // convert from radians to degrees
    int course = angle * 57.29577951 + 0.5;
    *dist = sqrtf (dx*dx + dy*dy);
    return course;
}

static void prkalc (int course, float dist)
{
    printf (": course %d  dist %.3f\n", course, dist);
}

//----------------------------------------------------------------------

void computer (int v UNUSED)
{
    enum ComputerCommand {
	CC_NOP, CC_CHART, CC_TRAJECTORY, CC_COURSE, CC_SCORE,
	CC_PLEFF, CC_WARPCOST, CC_IMPCOST, CC_DISTRESS
    };
    static const struct cvntab Cputab[] = {
	{"ch","art",		(cmdfun) CC_CHART,	0},
	{"t", "rajectory",	(cmdfun) CC_TRAJECTORY,	0},
	{"c", "ourse",		(cmdfun) CC_COURSE,	0},
	{"m", "ove",		(cmdfun) CC_COURSE,	1},
	{"s", "core",		(cmdfun) CC_SCORE,	0},
	{"p", "heff",		(cmdfun) CC_PLEFF,	0},
	{"w", "arpcost",	(cmdfun) CC_WARPCOST,	0},
	{"i", "mpcost",		(cmdfun) CC_IMPCOST,	0},
	{"d", "istresslist",	(cmdfun) CC_DISTRESS,	0},
	{}
    };
    while (device_works (COMPUTER)) {
	const struct cvntab* r = getcodpar ("\nRequest", Cputab);
	switch ((intptr_t) r->value) {
	    case CC_CHART:
		printf ("Computer record of all long range sensor scans\n\n  ");
		// print top header
		for (unsigned i = 0; i < NQUADS; ++i)
		    printf ("-%d- ", i);
		printf ("\n");
		for (unsigned i = 0; i < NQUADS; ++i) {
		    printf ("%d ", i);
		    for (unsigned j = 0; j < NQUADS; ++j) {
			if (i == Ship.quadx && j == Ship.quady) {
			    printf ("$$$ ");
			    continue;
			}
			struct quad* q = &Quad[i][j];
			// 1000 or 1001 is special case
			if (q->scanned >= 1000) {
			    if (q->scanned > 1000)
				printf (".1. ");
			    else
				printf ("/// ");
			} else if (q->scanned < 0)
			    printf ("... ");
			else
			    printf ("%3d ", q->scanned);
		    }
		    printf ("%d\n", i);
		}
		printf ("  ");
		// print bottom footer
		for (unsigned i = 0; i < NQUADS; ++i)
		    printf ("-%d- ", i);
		printf ("\n");
		break;

	    case CC_TRAJECTORY:
		if (!device_works (SRSCAN)) {
		    break;
		}
		if (Etc.npirates <= 0) {
		    printf ("No pirates in this quadrant\n");
		    break;
		}
		// for each pirate, give the course & distance
		for (int i = 0; i < Etc.npirates; ++i) {
		    printf ("pirate at %d,%d", Etc.pirate[i].x, Etc.pirate[i].y);
		    float dist;
		    unsigned course = kalc (Ship.quadx, Ship.quady, Etc.pirate[i].x, Etc.pirate[i].y, &dist);
		    prkalc (course, dist);
		}
		break;

	    case CC_COURSE: {
		int ix, iy;
		int tqx, tqy;
		if (readdelim('/')) {
		    tqx = Ship.quadx;
		    tqy = Ship.quady;
		} else {
		    ix = getintpar ("Quadrant");
		    if (ix < 0 || ix >= NSECTS)
			break;
		    iy = getintpar ("q-y");
		    if (iy < 0 || iy >= NSECTS)
			break;
		    tqx = ix;
		    tqy = iy;
		}
		ix = getintpar ("Sector");
		if (ix < 0 || ix >= NSECTS)
		    break;
		iy = getintpar ("s-y");
		if (iy < 0 || iy >= NSECTS)
		    break;
		float dist;
		unsigned course = kalc (tqx, tqy, ix, iy, &dist);
		if (r->value2) {
		    warp(-1, course, dist);
		    break;
		}
		printf ("%d,%d/%d,%d to %d,%d/%d,%d", Ship.quadx, Ship.quady, Ship.sectx, Ship.secty, tqx, tqy, ix, iy);
		prkalc (course, dist);
		break; }

	    case CC_SCORE:
		score();
		break;

	    case CC_PLEFF: {	// plaser effectiveness
		float dist = getfltpar ("range");
		if (dist < 0)
		    break;
		dist *= 10.0;
		unsigned cost = pow (0.90, dist) * 98.0;
		printf ("Plasers are %u%% effective at that range\n", cost);
		break; }

	    case CC_WARPCOST: {
		float dist = getfltpar ("distance");
		if (dist < 0)
		    break;
		float warpfact = getfltpar ("warp factor");
		if (warpfact <= 0)
		    warpfact = Ship.warp;
		unsigned cost = (dist + 0.05) * warpfact * warpfact * warpfact;
		float time = Param.warptime * dist / (warpfact * warpfact);
		printf ("Warp %.2f distance %.2f cost %.2f days %d (%d w/ shlds up) units\n", warpfact, dist, time, cost, cost + cost);
		break; }

	    case CC_IMPCOST: {
		float dist = getfltpar ("distance");
		if (dist < 0.0)
		    break;
		unsigned cost = 20 + 100 * dist;
		float time = dist / 0.095;
		printf ("Distance %.2f cost %.2f days %d units\n", dist, time, cost);
		break; }

	    case CC_DISTRESS: {	// distresslist
		bool allok = true;
		printf ("\n");
		// scan the event list
		for (unsigned i = 0; i < MAXEVENTS; ++i) {
		    const struct event* e = &Event[i];
		    // ignore hidden entries
		    if (e->evcode & E_HIDDEN)
			continue;
		    switch (e->evcode & E_EVENT) {

			case E_PDESB:
			    printf ("Starbase in quadrant %d,%d is being attacked by pirates\n", e->x, e->y);
			    allok = false;
			    break;

			case E_ENSLV:
			case E_REPRO:
			    printf ("Starsystem %s in quadrant %d,%d is distressed\n", Systemname[e->systemname], e->x, e->y);
			    allok = false;
			    break;
		    }
		}
		if (allok)
		    printf ("No known distress calls are active\n");
		break; }

	}

	// skip to next semicolon or newline. Semicolon
	// means get new computer request; newline means
	// exit computer mode.
	for (char i; (i = getchar()) != ';';) {
	    if (i == '\0')
		exit (EXIT_FAILURE);
	    if (i == '\n') {
		ungetc (i, stdin);
		return;
	    }
	}
    }
}
