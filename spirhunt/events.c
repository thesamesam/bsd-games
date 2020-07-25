// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "getpar.h"
#include "spirhunt.h"
#include <math.h>
#include <err.h>

//----------------------------------------------------------------------

static void long_range_tractor_beam (void);
static void update_galaxy_snapshot (void);

//----------------------------------------------------------------------

// CAUSE TIME TO ELAPSE
//
// This routine does a hell of a lot. It elapses time, eats up energy,
// regenerates energy, processes any events that occur, and so on.
//
int events (bool in_time_warp)
{
    int i;
    int j = 0;
    struct Pirate *k;
    float rtime;
    float xdate;
    float idate;
    struct event *ev = NULL;
    int ix, iy;
    struct quad *q;
    struct event *e;
    int evnum;
    int restcancel;

    // if nothing happened, just allow for any pirates killed
    if (Move.time <= 0.0) {
	Now.time = Now.resource / Now.pirates;
	return 0;
    }

    // indicate that the cloaking device is now working
    Ship.cloakgood = true;

    // idate is the initial date
    idate = Now.date;

    // schedule attacks if resting too long
    if (Move.time > 0.5 && Move.resting)
	schedule (E_ATTACK, 0.5, 0, 0, 0);

    // scan the event list
    while (1) {
	restcancel = 0;
	evnum = -1;
	// xdate is the date of the current event
	xdate = idate + Move.time;

	// find the first event that has happened
	for (i = 0; i < MAXEVENTS; ++i) {
	    e = &Event[i];
	    if (e->evcode == 0 || (e->evcode & E_GHOST))
		continue;
	    if (e->date < xdate) {
		xdate = e->date;
		ev = e;
		evnum = i;
	    }
	}
	e = ev;

	// find the time between events
	rtime = xdate - Now.date;

	// decrement the magic "Galactic Resources" pseudo-variable
	Now.resource -= Now.pirates * rtime;
	// and recompute the time left
	Now.time = Now.resource / Now.pirates;

	// move us up to the next date
	Now.date = xdate;

	// check for out of time
	if (Now.time <= 0.0)
	    lose(L_NOTIME);

	// if evnum < 0, no events occurred
	if (evnum < 0)
	    break;

	// otherwise one did. Find out what it is
	switch (e->evcode & E_EVENT) {
	    case E_SNOVA:     // supernova
		snova (-1, 0);			// cause the supernova to happen
		xresched (e, E_SNOVA, 1);	// and schedule the next one
		break;

	    case E_LRTB:      // long range tractor beam
		// schedule the next one
		xresched (e, E_LRTB, Now.pirates);
		// LRTB cannot occur if we are docked
		if (Ship.cond != DOCKED) {
		    long_range_tractor_beam();
		    Move.time = xdate - idate; // truncate the move time
		}
		break;

	    case E_PATSB:     // pirate attacks starbase
		// if out of bases, forget it
		if (Now.bases <= 0) {
		    unschedule (e);
		    break;
		}

		// check for starbase and pirates in same quadrant
		for (i = 0; i < Now.bases; ++i) {
		    ix = Now.base[i].x;
		    iy = Now.base[i].y;
		    // see if a pirate exists in this quadrant
		    q = &Quad[ix][iy];
		    if (q->pirates <= 0)
			continue;

		    // see if already distressed
		    for (j = 0; j < MAXEVENTS; ++j) {
			e = &Event[j];
			if ((e->evcode & E_EVENT) != E_PDESB)
			    continue;
			if (e->x == ix && e->y == iy)
			    break;
		    }
		    if (j < MAXEVENTS)
			continue;

		    // got a potential attack
		    break;
		}
		e = ev;
		if (i >= Now.bases) {
		    // not now; wait a while and see if some pirates move in
		    reschedule (e, 3*fnrand());
		    break;
		}
		// schedule a new attack, and a destruction of the base
		xresched (e, E_PATSB, 1);
		e = xsched (E_PDESB, 1, ix, iy, 0);

		// report it if we can
		if (!device_damaged (SSRADIO)) {
		    printf ("\nCaptain, we have received a distress signal\n");
		    printf ("  from the starbase in quadrant %d,%d.\n", ix, iy);
		    ++restcancel;
		} else // SSRADIO out, make it so we can't see the distress call
		    e->evcode |= E_HIDDEN;
		break;

	    case E_PDESB:     // pirate destroys starbase
		unschedule (e);
		if (Quad[e->x][e->y].bases > 0 && Quad[e->x][e->y].pirates > 0) {
		    if (e->x == Ship.quadx && e->y == Ship.quady)
			printf ("\n"); // message if in this quadrant
		    kill_starbase (e->x, e->y);
		}
		break;

	    case E_ISSUE:     // issue a distress call
		xresched (e, E_ISSUE, 1);
		// try a whole bunch of times to find something suitable
		for (i = 0; i < 100; ++i) {
		    ix = nrand(NQUADS);
		    iy = nrand(NQUADS);
		    q = &Quad[ix][iy];
		    // need a quadrant which is not the current one,
		    // which has some stars which are inhabited and
		    // not already under attack, which is not
		    // supernova'ed, and which has some pirates in it
		    if (!((ix == Ship.quadx && iy == Ship.quady) || q->stars < 0 || (q->qsystemname & Q_DISTRESSED) || (q->qsystemname & Q_SYSTEM) == 0 || q->pirates <= 0))
			break;
		}
		if (i >= 100) // can't seem to find one; ignore this call
		    break;

		// got one!!  Schedule its enslavement
		++Ship.distressed;
		e = xsched (E_ENSLV, 1, ix, iy, q->qsystemname);
		q->qsystemname = (e - Event) | Q_DISTRESSED;

		// tell the captain about it if we can
		if (!device_damaged (SSRADIO)) {
		    printf ("\nCaptain, starsystem %s in quadrant %d,%d is under attack\n", Systemname[e->systemname], ix, iy);
		    ++restcancel;
		} else
		    e->evcode |= E_HIDDEN;
		break;

	    case E_ENSLV:     // starsystem is enslaved
		unschedule (e);
		// see if current distress call still active
		if (Quad[e->x][e->y].pirates <= 0)
		    Quad[e->x][e->y].qsystemname = e->systemname;	// no pirates, clean up; restore the system name
		else {
		    // Enslaved starsystems create more pirates
		    struct event* re = schedule (E_REPRO, Param.eventdly[E_REPRO] * fnrand(), e->x, e->y, e->systemname);
		    // Report the disaster if we can
		    if (!device_damaged (SSRADIO))
			printf ("\nWe've lost contact with starsystem %s\nin quadrant %d,%d.\n", Systemname[re->systemname], re->x, re->y);
		    else
			re->evcode |= E_HIDDEN;
		}
		break;

	    case E_REPRO:     // pirate reproduces
		// see if distress call is still active
		q = &Quad[e->x][e->y];
		if (q->pirates <= 0) {
		    unschedule (e);
		    q->qsystemname = e->systemname;
		    break;
		}
		xresched (e, E_REPRO, 1);
		// reproduce one pirate
		ix = e->x;
		iy = e->y;
		if (Now.pirates == INT8_MAX)
		    break;     // full right now
		if (q->pirates >= MAX_QUAD_PIRATES) {
		    // this quadrant not ok, pick an adjacent one
		    for (i = ix - 1; i <= ix + 1; ++i) {
			if (i < 0 || i >= NQUADS)
			    continue;
			for (j = iy - 1; j <= iy + 1; ++j) {
			    if (j < 0 || j >= NQUADS)
				continue;
			    q = &Quad[i][j];
			    // check for this quad ok (not full & no snova)
			    if (q->pirates >= MAX_QUAD_PIRATES || q->stars < 0)
				continue;
			    break;
			}
			if (j <= iy + 1)
			    break;
		    }
		    if (j > iy + 1)
			break; // cannot create another yet
		    ix = i;
		    iy = j;
		}
		// deliver the child
		++q->pirates;
		++Now.pirates;
		if (ix == Ship.quadx && iy == Ship.quady) {
		    // we must position pirate
		    sector(&ix, &iy);
		    Sect[ix][iy] = PIRATE;
		    k = &Etc.pirate[Etc.npirates++];
		    k->x = ix;
		    k->y = iy;
		    k->power = Param.piratepwr;
		    k->srndreq = 0;
		    comp_pirate_dist (Etc.pirate[0].dist == Etc.pirate[0].avgdist ? 0 : 1);
		}

		// recompute time left
		Now.time = Now.resource / Now.pirates;
		break;

	    case E_SNAP:      // take a snapshot of the galaxy
		xresched (e, E_SNAP, 1);
		update_galaxy_snapshot();
		break;

	    case E_ATTACK:    // pirates attack during rest period
		if (Move.resting) {
		    attack (1);
		    reschedule (e, 0.5);
		} else
		    unschedule (e);
		break;

	    case E_FIXDV:
		printf ("Repair work on the %s is finished.\n", DeviceName[e->systemname]);
		if (e->systemname == SINS && Ship.cond != DOCKED) {
		    Ship.sinsbad = true;
		    printf ("The Space Internal Navigation System can only be recalibrated when\n"
			    "docked at a starbase equipped with appropriate calibration standards.\n");
		} else if (e->systemname == SSRADIO)
		    restcancel = output_hidden_distress_calls();
		else if (e->systemname == LIFESUP)
		    Ship.reserves = Param.reserves;
		unschedule (e);
		break;
	}
	if (restcancel && Move.resting && getynpar("Shall we cancel our rest period"))
	    Move.time = xdate - idate;
    }

    // unschedule an attack during a rest period
    if ((e = Now.eventptr[E_ATTACK]) != NULL)
	unschedule (e);

    if (!in_time_warp) {
	// eat up energy if cloaked
	if (Ship.cloaked)
	    Ship.energy -= Param.cloakenergy * Move.time;

	// regenerate resources
	rtime = 1.0 - exp(-Param.regenfac * Move.time);
	Ship.shield += (Param.shield - Ship.shield) * rtime;
	Ship.energy += (Param.energy - Ship.energy) * rtime;

	// decrement life support reserves
	if (device_damaged (LIFESUP) && Ship.cond != DOCKED)
	    Ship.reserves -= Move.time;
    }
    return 0;
}

static void long_range_tractor_beam (void)
{
    // pick a new quadrant
    unsigned ix, iy;
    do {
	int i = nrand(Now.pirates) + 1;
	for (ix = 0; ix < NQUADS; ++ix) {
	    for (iy = 0; iy < NQUADS; ++iy) {
		const struct quad *q = &Quad[ix][iy];
		if (q->stars >= 0)
		    if ((i -= q->pirates) <= 0)
			break;
	    }
	    if (i <= 0)
		break;
	}
    } while (Ship.quadx == ix && Ship.quady == iy);

    Ship.quadx = ix;
    Ship.quady = iy;
    Ship.sectx = nrand (NSECTS);
    Ship.secty = nrand (NSECTS);
    printf ("\nYour ship is caught in long range tractor beam\n"
	    "*** Pulled to quadrant %u,%u\n", Ship.quadx, Ship.quady);
    initquad (false);
}

static void update_galaxy_snapshot (void)
{
    char* p = Etc.snapshot;
    memcpy (p, Quad, sizeof(Quad));
    p += sizeof(Quad);
    memcpy (p, Event, sizeof(Event));
    p += sizeof(Event);
    memcpy (p, &Now, sizeof(Now));
    Game.snap = true;
}

bool output_hidden_distress_calls (void)
{
    bool emergency = false;
    for (unsigned j = 0; j < MAXEVENTS; ++j) {
	const struct event* e = &Event[j];
	if (!(e->evcode & E_HIDDEN))
	    continue; // not hidden
	if (e->evcode & E_GHOST) {
	    printf ("Starsystem %s in quadrant %u,%u is no longer distressed\n", systemname(&Quad[e->x][e->y]), e->x, e->y);
	    unschedule (&Event[j]);
	} else if (e->evcode == E_PDESB) {
	    printf ("Starbase in quadrant %u,%u is under attack\n", e->x, e->y);
	    emergency = true;
	} else if (e->evcode == E_ENSLV || e->evcode == E_REPRO) {
	    printf ("Starsystem %s in quadrant %u,%u is distressed\n", systemname(&Quad[e->x][e->y]), e->x, e->y);
	    emergency = true;
	}
    }
    return emergency;
}

// SCHEDULE AN EVENT
//
// An event of type 'type' is scheduled for time NOW + 'offset'
// into the first available slot. 'x', 'y', and 'z' are
// considered the attributes for this event.
//
// The address of the slot is returned.
//
struct event* schedule (enum EventType type, float offset, uint8_t x, uint8_t y, uint8_t sysname)
{
    float date = Now.date + offset;
    for (unsigned i = 0; i < MAXEVENTS; ++i) {
	struct event *e = &Event[i];
	if (e->evcode)
	    continue;
	// got a slot
	e->evcode = type;
	e->date = date;
	e->x = x;
	e->y = y;
	e->systemname = sysname;
	Now.eventptr[type] = e;
	return e;
    }
    errx (1, "Cannot schedule event %u parm %u %u %u", type, x, y, sysname);
}

// UNSCHEDULE AN EVENT
//
// The event at slot 'e' is deleted.
void unschedule (struct event* e)
{
    Now.eventptr [e->evcode & E_EVENT] = 0;
    e->date = FLT_MAX;
    e->evcode = 0;
}

// Abreviated schedule routine
//
// Parameters are the event index and a factor for the time figure.
struct event* xsched (enum EventType ev, int factor, uint8_t x, uint8_t y, uint8_t sysname)
{
    return schedule (ev, -Param.eventdly[ev] * Param.time * log(fnrand()) / factor, x, y, sysname);
}

// Simplified reschedule routine
//
// Parameters are the event index, the initial date, and the
// division factor. Look at the code to see what really happens.
//
void xresched (struct event* e, enum EventType ev, int factor)
{
    reschedule (e, -Param.eventdly[ev] * Param.time * log(fnrand()) / factor);
}
