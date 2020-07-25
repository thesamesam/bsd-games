// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"
#include <math.h>

//----------------------------------------------------------------------

static void move_to_random_location (void);
static void ram (unsigned ix, unsigned iy);

//----------------------------------------------------------------------

// Move Under Warp or Impulse Power
//
// 'Ramflag' is set if we are to be allowed to ram stars,
// pirates, etc. This is passed from warp(), which gets it from
// either play() or ram(). Course is the course (0 -> 360) at
// which we want to move. 'Speed' is the speed we
// want to go, and 'time' is the expected time. It
// can get cut short if a long range tractor beam is to occur. We
// cut short the move so that the user doesn't get docked time and
// energy for distance which he didn't travel.
//
// We check the course through the current quadrant to see that he
// doesn't run into anything. After that, though, space sort of
// bends around him. Note that this puts us in the awkward posi-
// tion of being able to be dropped into a sector which is com-
// pletely surrounded by stars. Oh Well.
//
// If the SINS (Space Inertial Navigation System) is out, we ran-
// domize the course accordingly before ever starting to move.
// We will still move in a straight line.
//
// Note that if your computer is out, you ram things anyway. In
// other words, if your computer and sins are both out, you're in
// potentially very bad shape.
//
// Pirates get a chance to zap you as you leave the quadrant.
// By the way, they also try to follow you (heh heh).
//
// Return value is the actual amount of time used.
//
//
// Uses trace flag 4.

float move_ship (int ramflag, int course, float time, float speed)
{
    float sectsize = NSECTS;
    // initialize delta factors for move
    float angle = course * 0.0174532925;
    if (device_damaged (SINS))
	angle += Param.navigcrud[1] * (fnrand() - 0.5);
    else if (Ship.sinsbad)
	angle += Param.navigcrud[0] * (fnrand() - 0.5);
    float dx = -cos(angle);
    float dy = sin(angle);
    float bigger = fabs(dx);
    float dist = fabs(dy);
    if (dist > bigger)
	bigger = dist;
    dx /= bigger;
    dy /= bigger;

    // check for long range tractor beams
    //***  TEMPORARY CODE == DEBUGGING  ***
    float evtime = Now.eventptr[E_LRTB]->date - Now.date;
    if (time > evtime && Etc.npirates < 3) {
	// then we got a LRTB
	evtime += 0.005;
	time = evtime;
    } else
	evtime = -1.0e50;
    dist = time * speed;

    // move within quadrant
    Sect[Ship.sectx][Ship.secty] = EMPTY;
    float x = Ship.sectx + 0.5;
    float y = Ship.secty + 0.5;
    float xn = NSECTS * dist * bigger;
    int n = xn + 0.5;
    Move.free = 0;

    int ix = 0, iy = 0;
    for (int i = 0; i < n; ++i) {
	ix = (x += dx);
	iy = (y += dy);
	if (x < 0.0 || y < 0.0 || x >= sectsize || y >= sectsize) {
	    // enter new quadrant
	    dx = Ship.quadx * NSECTS + Ship.sectx + dx * xn;
	    dy = Ship.quady * NSECTS + Ship.secty + dy * xn;
	    if (dx < 0.0)
		ix = -1;
	    else
		ix = dx + 0.5;
	    if (dy < 0.0)
		iy = -1;
	    else
		iy = dy + 0.5;
	    Ship.sectx = x;
	    Ship.secty = y;
	    comp_pirate_dist(0);
	    Move.newquad = 2;
	    attack(0);
	    checkcond();
	    Ship.quadx = ix / NSECTS;
	    Ship.quady = iy / NSECTS;
	    Ship.sectx = ix % NSECTS;
	    Ship.secty = iy % NSECTS;
	    if (ix < 0 || Ship.quadx >= NQUADS || iy < 0 || Ship.quady >= NQUADS) {
		if (device_damaged (COMPUTER))
		    lose (L_NEGENB);
		printf ("Computer applies full reverse power to avoid hitting the\n");
		printf ("negative energy barrier. A space warp was entered.\n");
		move_to_random_location();
	    }
	    initquad (false);
	    n = 0;
	    break;
	}
	if (Sect[ix][iy] != EMPTY) {
	    // we just hit something
	    if (!device_damaged (COMPUTER) && ramflag <= 0) {
		ix = x - dx;
		iy = y - dy;
		printf ("Computer reports navigation error; stopped at %d,%d\n", ix, iy);
		Ship.energy -= Param.stopengy * speed;
		break;
	    }
	    // test for a black hole
	    if (Sect[ix][iy] == HOLE) {
		// get dumped elsewhere in the galaxy
		printf ("You fall into a black hole.\n");
		move_to_random_location();
		initquad (false);
		n = 0;
		break;
	    }
	    ram(ix, iy);
	    break;
	}
    }
    if (n > 0) {
	dx = Ship.sectx - ix;
	dy = Ship.secty - iy;
	dist = sqrtf (dx * dx + dy * dy) / NSECTS;
	time = dist / speed;
	if (evtime > time)
	    time = evtime;     // spring the LRTB trap
	Ship.sectx = ix;
	Ship.secty = iy;
    }
    Sect[Ship.sectx][Ship.secty] = YOURSHIP;
    comp_pirate_dist (0);
    return time;
}

// Dump the starship somewhere in the galaxy
//
// Parameter is zero if bounce off of negative energy barrier,
// one if through a black hole
//
// Note that the quadrant is NOT initialized here. This must
// be done from the calling routine.
//
// Repair of devices must be deferred.

static void move_to_random_location (void)
{
    Ship.quadx = nrand (NQUADS);
    Ship.quady = nrand (NQUADS);
    Ship.sectx = nrand (NSECTS);
    Ship.secty = nrand (NSECTS);
    float x = 1.5 * fnrand();
    Move.time += x;
    // bump repair dates forward
    for (unsigned i = 0; i < MAXEVENTS; ++i) {
	struct event *e = &Event[i];
	if (e->evcode == E_FIXDV)
	    reschedule (e, (e->date - Now.date) + x);
    }
    events (1);
    printf ("You are now in quadrant %u,%u. Today is %.2f\n", Ship.quadx, Ship.quady, Now.date);
    Move.time = 0;
}

// RAM SOME OBJECT
//
// You have run into some sort of object. It may be a pirate,
// a star, or a starbase. If you run into a star, you are really
// stupid, because there is no hope for you.
//
// If you run into something else, you destroy that object. You
// also rack up incredible damages.
//
static void ram (unsigned ix, unsigned iy)
{
    printf ("\07RED ALERT\07: collision imminent\n");
    char c = Sect[ix][iy];
    if (c == PIRATE) {
	printf ("Your ship rams pirate at %d,%d\n", ix, iy);
	kill_pirate (ix, iy);
    } else if (c == STAR || c == INHABIT) {
	printf ("Captain, isn't it getting hot in here?\n");
	sleep (2);
	printf ("Hull temperature approaching 550 Degrees Kelvin.\n");
	lose (L_STAR);
    } else if (c == BASE) {
	printf ("You ran into the starbase at %d,%d\n", ix, iy);
	kill_starbase (Ship.quadx, Ship.quady);
	if (!device_damaged (SINS))	// don't penalize the captain if it wasn't his fault
	    ++Game.bases_killed;
    }
    sleep (2);
    puts ("Your ship sustained heavy damage.");

    unsigned deaths = 10 + nrand(40);	// Crew killed in the collision
    Ship.shldup = false;		// No chance that your shields remained up in all that
    Ship.crew -= deaths;
    Game.deaths += deaths;
    printf ("%u casualties reported.\n", deaths);

    // Damage devices with an 80% probability
    for (unsigned i = 0; i < NDEV; ++i)
	if (nrand(100) < 80)
	    damage_device (i, (2.5 * (fnrand() + fnrand()) + 1.0) * Param.damfac[i]);
}

// SET WARP FACTOR
//
// The warp factor is set for future move commands. It is
// checked for consistancy.
void setwarp (int v UNUSED)
{
    float warpfac = getfltpar("Warp factor");
    if (warpfac < 0)
	return;
    if (warpfac < 1) {
	printf ("Minimum warp speed is 1\n");
	return;
    }
    if (warpfac > 10) {
	printf ("Maximum speed is warp 10\n");
	return;
    }
    if (warpfac > 6)
	printf ("Damage to warp engines may occur above warp 6\n");
    Ship.warp = warpfac;
    Ship.warp2 = Ship.warp * warpfac;
    Ship.warp3 = Ship.warp2 * warpfac;
}

// MOVE UNDER WARP POWER
//
// This is both the "move" and the "ram" commands, differing
// only in the flag 'fl'. It is also used for automatic
// emergency override mode, when 'fl' is < 0 and 'c' and 'd'
// are the course and distance to be moved. If 'fl' >= 0,
// the course and distance are asked of the captain.
//
// The guts of this routine are in the routine move(), which
// is shared with impulse(). Also, the working part of this
// routine is very small; the rest is to handle the slight chance
// that you may be moving at some riduculous speed. In that
// case, there is code to handle time warps, etc.

void dowarp (int fl)
{
    int c;
    float d;
    if (getcodi (&c, &d))
	return;
    warp (fl, c, d);
}

void warp (int fl, int c, float d)
{
    if (Ship.cond == DOCKED) {
	printf ("You are docked\n");
	return;
    }
    if (device_damaged (WARP)) {
	announce_device_damage (WARP);
	return;
    }

    int course = c;
    float dist = d;

    // check to see that we are not using an absurd amount of power
    float power = (dist + 0.05) * Ship.warp3;
    unsigned percent = 100 * power / Ship.energy;
    if (percent >= 85) {
	printf ("That would consume %u%% of our remaining energy.\n", percent);
	if (!getynpar ("Are you sure that is wise"))
	    return;
    }

    // compute the speed we will move at, and the time it will take
    float speed = Ship.warp2 / Param.warptime;
    float time = dist / speed;

    // check to see that that value is not ridiculous
    percent = 100 * time / Now.time;
    if (percent >= 85) {
	printf ("That would take %u%% of our remaining time.\n", percent);
	if (!getynpar("Are you sure that is wise"))
	    return;
    }

    // compute how far we will go if we get damages
    if (Ship.warp > 6.0 && nrand(100) < 20 + 15 * (Ship.warp - 6.0)) {
	float frac = fnrand();
	dist *= frac;
	time *= frac;
	damage_device (WARP, (frac + 1.0) * Ship.warp * (fnrand() + 0.25) * 0.20);
    }

    // do the move
    Move.time = move_ship (fl, course, time, speed);

    // see how far we actually went, and decrement energy appropriately
    dist = Move.time * speed;
    Ship.energy -= dist * Ship.warp3 * (Ship.shldup + 1);

    // test for bizarre events
    if (Ship.warp <= 9.0)
	return;
    printf ("\n\n  ___ Speed exceeding warp nine ___\n\n");
    sleep(2);
    printf ("Ship's safety systems malfunction\n");
    sleep(2);
    printf ("Crew experiencing extreme sensory distortion\n");
    sleep(4);
    if (nrand(100) >= 100 * dist) {
	printf ("Equilibrium restored -- all systems normal\n");
	return;
    }

    // select a bizzare thing to happen to us
    percent = nrand(100);
    if (percent < 70) {
	// time warp
	if (percent < 35 || !Game.snap) {
	    // positive time warp
	    time = (Ship.warp - 8.0) * dist * (fnrand() + 1.0);
	    Now.date += time;
	    printf ("Positive time portal entered -- we are now in %.2f\n", Now.date);
	    for (unsigned i = 0; i < MAXEVENTS; ++i) {
		percent = Event[i].evcode;
		if (percent == E_FIXDV || percent == E_LRTB)
		    Event[i].date += time;
	    }
	    return;
	}

	// got lucky: a negative time portal
	time = Now.date;
	char* p = (char*) Etc.snapshot;
	memcpy (p, Quad, sizeof Quad);
	p += sizeof Quad;
	memcpy (p, Event, sizeof Event);
	p += sizeof Event;
	memcpy (p, &Now, sizeof Now);
	printf ("Negative time portal entered -- we are now in %.2f\n", Now.date);
	for (unsigned i = 0; i < MAXEVENTS; ++i)
	    if (Event[i].evcode == E_FIXDV)
		reschedule (&Event[i], Event[i].date - time);
	return;
    }

    // test for just a lot of damage
    if (percent < 80)
	lose (L_TOOFAST);
    printf ("Equilibrium restored -- extreme damage occurred to ship systems\n");
    for (unsigned i = 0; i < NDEV; ++i)
	damage_device (i, (3.0 * (fnrand() + fnrand()) + 1.0) * Param.damfac[i]);
    Ship.shldup = 0;
}

// move under impulse power
void impulse (int v UNUSED)
{
    if (Ship.cond == DOCKED) {
	printf ("Sorry captain, but we are still docked.\n");
	return;
    }
    if (device_damaged (IMPULSE)) {
	announce_device_damage (IMPULSE);
	return;
    }
    int course;
    float dist;
    if (getcodi (&course, &dist))
	return;
    unsigned energy = 20 + 100 * dist;
    unsigned percent = 100 * energy / Ship.energy;
    if (percent >= 85) {
	printf ("That would consume %u%% of our remaining energy.\n", percent);
	if (!getynpar("Are you sure that is wise"))
	    return;
	printf ("Aye aye, sir\n");
    }
    const float speed = 0.095;
    float time = dist / speed;
    percent = 100 * time / Now.time;
    if (percent >= 85) {
	printf ("That would take %d%% of our remaining time.\n", percent);
	if (!getynpar ("Are you sure that is wise"))
	    return;
	printf ("(He's finally gone mad)\n");
    }
    Move.time = move_ship (0, course, time, speed);
    Ship.energy -= 20 + 100 * Move.time * speed;
}

// Automatic Override
//
// If we should be so unlucky as to be caught in a quadrant
// with a supernova in it, this routine is called. It is
// called from checkcond().
//
// It sets you to a random warp (guaranteed to be over 6.0)
// and starts sending you off "somewhere" (whereever that is).
//
// Please note that it is VERY important that you reset your
// warp speed after the automatic override is called. The new
// warp factor does not stay in effect for just this routine.
//
// This routine will never try to send you more than sqrt(2)
// quadrants, since that is all that is needed.
//
void autover (void)
{
    printf ("\07RED ALERT: You are in a supernova quadrant\n"
	    "*** Emergency override attempts to escape\n");
    // let's get our ass out of here
    Ship.warp = 6.0 + 2.0 * fnrand();
    Ship.warp2 = Ship.warp * Ship.warp;
    Ship.warp3 = Ship.warp2 * Ship.warp;
    float dist = 0.75 * Ship.energy / (Ship.warp3 * (Ship.shldup + 1));
    if (dist > 1.4142)
	dist = 1.4142;
    int course = nrand (360);
    Etc.npirates = -1;
    Ship.cond = RED;
    warp (-1, course, dist);
    attack (0);
}
