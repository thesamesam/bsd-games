// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"
#include <math.h>

//{{{ Ship devices -----------------------------------------------------

// CHECK IF A DEVICE IS OUT
//
// The indicated device is checked to see if it is disabled. If
// it is, an attempt is made to use the starbase device. If both
// of these fails, it returns non-zero (device is REALLY out),
// otherwise it returns zero (I can get to it somehow).
//
// It prints appropriate messages too.
//
bool device_works (enum ShipDevice d)
{
    // check for device ok
    if (!device_damaged (d))
	return true;
    announce_device_damage (d);	// report it as being dead
    if (Ship.cond == DOCKED) {
	printf ("  Using starbase %s\n", DeviceName[d]);
	return true;
    }
    return false;
}

// damage control report
//
// Print damages and time to fix. This is taken from the event
// list. A couple of factors are set up, based on whether or not
// we are docked. (One of these factors will always be 1.0.)
// The event list is then scanned for damage fix events, the
// time until they occur is determined, and printed out. The
// magic number DAMFAC is used to tell how much faster you can
// fix things if you are docked.

void damage_report (int v UNUSED)
{
    // Set up the magic factors to output repair time
    float m1 = 1, m2 = Param.dockfac;
    if (Ship.cond == DOCKED) {
	m1 /= Param.dockfac;
	m2 = 1;
    }

    // scan for damages
    bool allok = true;
    for (unsigned i = 0; i < MAXEVENTS; ++i) {
	const struct event* e = &Event[i];
	if (e->evcode != E_FIXDV)
	    continue;
	// output the title first time
	if (allok) {
	    allok = false;
	    printf ("Damage control report:\n"
		    "\t\t\t  repair times\n"
		    "device\t\t\tin flight  docked\n");
	}
	// compute time till fixed, then adjust by the magic factors
	float x = e->date - Now.date;
	printf ("%-24s%7.2f  %7.2f\n", DeviceName[e->systemname], x*m1 + 0.005, x*m2 + 0.005);
    }

    // if everything was ok, reassure the nervous captain
    if (allok)
	printf ("All devices functional\n");
}

// Schedule Ship.damages to a Device
//
// Device 'dev1' is damaged in an amount 'dam'. Dam is measured
// in days, and is an additional amount of damage. It should
// be the amount to occur in non-docked mode. The adjustment
// to docked mode occurs automatically if we are docked.
//
// Note that the repair of the device occurs on a DATE, meaning
// that the dock() and undock() have to reschedule the event.
//
// dev1 - device index
// dam - time to repair
//
void damage_device (enum ShipDevice dev, float dam)
{
    if (dam <= 0)
	return; // ignore zero damages
    printf ("\t%s damaged\n", DeviceName[dev]);

    // find actual length till it will be fixed
    if (Ship.cond == DOCKED)
	dam *= Param.dockfac;
    // set the damage flag
    int f = device_damaged (dev);
    if (!f) {
	// new damages -- schedule a fix
	schedule(E_FIXDV, dam, 0, 0, dev);
	return;
    }
    // if device already damaged, add to existing damage
    for (unsigned i = 0; i < MAXEVENTS; ++i) {
	struct event* e = &Event[i];
	if (e->evcode != E_FIXDV || e->systemname != dev)
	    continue;
	// got the right one; add on the new damages
	reschedule (e, e->date - Now.date + dam);
	return;
    }
}

// This is a boolean function which returns non-zero if the
// specified device is broken. It does this by checking the
// event list for a "device fix" action on that device.
//
bool device_damaged (enum ShipDevice d)
{
    for (unsigned i = 0; i < MAXEVENTS; ++i)
	if (Event[i].evcode == E_FIXDV && Event[i].systemname == d)
	    return true;
    return false;	// device fix not in event list -- device must not be broken
}

// Announce Device Out
void announce_device_damage (enum ShipDevice dev)
{
    const char* d = DeviceName[dev];
    printf ("%s ", d);
    if (d[strlen(d)-1] == 's')
	printf ("are");
    else
	printf ("is");
    printf (" damaged\n");
}

//}}}-------------------------------------------------------------------
//{{{ Docking and resting

static bool adjacent_to_starbase (void)
{
    for (unsigned y = 0; y < NSECTS; ++y) {
	if (absv (Ship.secty-y) > 1)
	    continue;
	for (unsigned x = 0; x < NSECTS; ++x)
	    if (absv (Ship.sectx-x) <= 1 && Sect[x][y] == BASE)
		return true;
    }
    return false;
}

// DOCK TO STARBASE
//
// The starship is docked to a starbase. For this to work you
// must be adjacent to a starbase.
//
// You get your supplies replenished and your captives are
// disembarked. Note that your score is updated now, not when
// you actually take the captives.
//
// Any repairs that need to be done are rescheduled to take
// place sooner. This provides for the faster repairs when you
// are docked.

void dock (int v UNUSED)
{
    if (Ship.cond == DOCKED) {
	printf ("We are already docked.\n");
	return;
    }
    if (!adjacent_to_starbase()) {
	printf ("We are not adjacent to a starbase.\n");
	return;
    }
    printf ("Docking procedure successful.\n");

    // Reset ship's defenses
    Ship.cond = DOCKED;
    Ship.shldup = 0;
    Ship.cloaked = 0;
    Ship.reserves = Param.reserves;

    // Restore resources
    Ship.energy = Param.energy;
    Ship.torped = Param.torped;
    Ship.shield = Param.shield;
    Ship.crew = Param.crew;
    Game.captives += Param.brigfree - Ship.brigfree;
    Ship.brigfree = Param.brigfree;

    // Recalibrate space inertial navigation system
    Ship.sinsbad = 0;

    // If the radio was broken, starbase can update
    output_hidden_distress_calls();

    // Reschedule any device repairs
    for (unsigned i = 0; i < MAXEVENTS; ++i)
	if (Event[i].evcode == E_FIXDV)
	    reschedule (&Event[i], (Event[i].date - Now.date) * Param.dockfac);
}

// LEAVE A STARBASE
void undock (int v UNUSED)
{
    if (Ship.cond != DOCKED) {
	printf ("We are not docked.\n");
	return;
    }
    printf ("Undocking complete.\n");
    Ship.cond = GREEN;
    Move.free = 0;

    // Reschedule device repair times to take longer
    for (unsigned i = 0; i < MAXEVENTS; ++i)
	if (Event[i].evcode == E_FIXDV)
	    reschedule (&Event[i], (Event[i].date - Now.date) / Param.dockfac);
}

// REST FOR REPAIRS
//
// You sit around and wait for repairs to happen. Actually, you
// sit around and wait for anything to happen. I do want to point
// out however, that pirates are not as patient as you are, and
// they tend to attack you while you are resting.
//
// You can never rest through a long range tractor beam.
//
// In events() you will be given an opportunity to cancel the
// rest period if anything momentous happens.
//
void rest (int v UNUSED)
{
    // get the time to rest
    float t = getfltpar ("How long");
    if (t <= 0.0)
	return;
    unsigned percent = 100 * t / Now.time + 0.5;
    if (percent >= 70) {
	printf ("That would take %u%% of our remaining time.\n", percent);
	if (!getynpar ("Are you really certain that is wise"))
	    return;
    }
    Move.time = t;

    // boundary condition is the LRTB
    t = Now.eventptr[E_LRTB]->date - Now.date;
    if (Ship.cond != DOCKED && Move.time > t)
	Move.time = t + 0.0001;
    Move.free = 0;
    Move.resting = 1;
}

//}}}-------------------------------------------------------------------
//{{{ Plasers

#define	ALPHA		3.0    // spread
#define	BETA		3.0    // fnrand()
#define	GAMMA		0.30   // cos(angle)
#define	EPSILON		150.0  // dist ** 2
#define	OMEGA		10.596 // overall scaling factor

// OMEGA ~= 100 * (ALPHA + 1) * (BETA + 1) / (EPSILON + 1)

// Plaser Control
//
// There are up to NBANKS plaser banks which may be fired
// simultaneously. There are two modes, "manual" and
// "automatic". In manual mode, you specify exactly which
// direction you want each bank to be aimed, the number
// of units to fire, and the spread angle. In automatic
// mode, you give only the total number of units to fire.
//
// The spread is specified as a number between zero and
// one, with zero being minimum spread and one being maximum
// spread. You  will normally want zero spread, unless your
// short range scanners are out, in which case you probably
// don't know exactly where the pirates are. In that case,
// you really don't have any choice except to specify a
// fairly large spread.
//
// Plasers spread slightly, even if you specify zero spread.

void plasers (int v UNUSED)
{
    int i;
    int j;
    struct Pirate* k;
    float dx, dy;
    float anglefactor, distfactor;
    struct banks {
	int units;
	float angle;
	float spread;
    };
    struct banks *b;
    int flag, extra = 0;
    int hit;
    float tot;
    int n;
    int hitreqd[NBANKS];
    struct banks bank[NBANKS];

    if (Ship.cond == DOCKED) {
	printf ("Plasers cannot fire through starbase shields\n");
	return;
    }
    if (device_damaged (PLASER)) {
	announce_device_damage (PLASER);
	return;
    }
    if (Ship.cloaked) {
	printf ("Plasers are inoperative while cloaked\n");
	return;
    }

    // initialize the bank[] array
    flag = 1;
    for (i = 0; i < NBANKS; ++i)
	bank[i].units = 0;
    // automatic distribution of power
    if (Etc.npirates <= 0) {
	printf ("But there are no pirates in this quadrant\n");
	return;
    }
    printf ("Plasers locked on target. ");
    while (flag) {
	printf ("%d units available\n", Ship.energy);
	hit = getintpar("Units to fire");
	if (hit <= 0)
	    return;
	if (hit > Ship.energy) {
	    printf ("available energy exceeded. ");
	    skiptonl(0);
	    continue;
	}
	flag = 0;
	Ship.energy -= hit;
	extra = hit;
	n = Etc.npirates;
	if (n > NBANKS)
	    n = NBANKS;
	tot = n * (n + 1) / 2;
	for (i = 0; i < n; ++i) {
	    k = &Etc.pirate[i];
	    b = &bank[i];
	    distfactor = k->dist;
	    anglefactor = ALPHA * BETA * OMEGA / (distfactor * distfactor + EPSILON);
	    anglefactor *= GAMMA;
	    distfactor = k->power;
	    distfactor /= anglefactor;
	    hitreqd[i] = distfactor + 0.5;
	    dx = Ship.sectx - k->x;
	    dy = k->y - Ship.secty;
	    b->angle = atan2(dy, dx);
	    b->spread = 0.0;
	    b->units = ((n - i) / tot) * extra;
	    extra -= b->units;
	    hit = b->units - hitreqd[i];
	    if (hit > 0) {
		extra += hit;
		b->units -= hit;
	    }
	}

	// give out any extra energy we might have around
	if (extra > 0) {
	    for (i = 0; i < n; ++i) {
		b = &bank[i];
		hit = hitreqd[i] - b->units;
		if (hit <= 0)
		    continue;
		if (hit >= extra) {
		    b->units += extra;
		    extra = 0;
		    break;
		}
		b->units = hitreqd[i];
		extra -= hit;
	    }
	    if (extra > 0)
		printf ("%d units overkill\n", extra);
	}
    }

    // actually fire the shots
    Move.free = 0;
    for (i = 0; i < NBANKS; ++i) {
	b = &bank[i];
	if (b->units <= 0) {
	    continue;
	}
	printf ("\nPlaser bank %d fires:\n", i);
	n = Etc.npirates;
	k = Etc.pirate;
	for (j = 0; j < n; ++j) {
	    if (b->units <= 0)
		break;
	    //
	    // The formula for hit is as follows:
	    //
	    //  zap = OMEGA * [(sigma + ALPHA) * (rho + BETA)]
	    //     / (dist ** 2 + EPSILON)]
	    //     * [cos(delta * sigma) + GAMMA]
	    //     * hit
	    //
	    // where sigma is the spread factor,
	    // rho is a random number (0 -> 1),
	    // GAMMA is a crud factor for angle (essentially
	    //     cruds up the spread factor),
	    // delta is the difference in radians between the
	    //     angle you are shooting at and the actual
	    //     angle of the pirate,
	    // ALPHA scales down the significance of sigma,
	    // BETA scales down the significance of rho,
	    // OMEGA is the magic number which makes everything
	    //     up to "* hit" between zero and one,
	    // dist is the distance to the pirate
	    // hit is the number of units in the bank, and
	    // zap is the amount of the actual hit.
	    //
	    // Everything up through dist squared should maximize
	    // at 1.0, so that the distance factor is never
	    // greater than one. Conveniently, cos() is
	    // never greater than one, but the same restric-
	    // tion applies.
	    //
	    distfactor = BETA + fnrand();
	    distfactor *= ALPHA + b->spread;
	    distfactor *= OMEGA;
	    anglefactor = k->dist;
	    distfactor /= anglefactor * anglefactor + EPSILON;
	    distfactor *= b->units;
	    dx = Ship.sectx - k->x;
	    dy = k->y - Ship.secty;
	    anglefactor = atan2(dy, dx) - b->angle;
	    anglefactor = cos((anglefactor * b->spread) + GAMMA);
	    if (anglefactor < 0.0) {
		++k;
		continue;
	    }
	    hit = anglefactor * distfactor + 0.5;
	    k->power -= hit;
	    printf ("%d unit hit on pirate", hit);
	    if (!device_damaged (SRSCAN))
		printf (" at %d,%d", k->x, k->y);
	    printf ("\n");
	    b->units -= hit;
	    if (k->power <= 0) {
		kill_pirate (k->x, k->y);
		continue;
	    }
	    ++k;
	}
    }

    // compute overkill
    for (i = 0; i < NBANKS; ++i)
	extra += bank[i].units;
    if (extra > 0)
	printf ("\n%d units expended on empty space\n", extra);
}

//}}}-------------------------------------------------------------------
//{{{ Torpedoes

// TORPEDO CONTROL
//
// Either one or three torpedoes are fired. If three are fired, it is called
// a "burst" and you also specify a spread angle.
//
// Torpedoes are never 100% accurate. There is always a random cludge factor
// in their course which is increased if you have your shields up. Hence,
// you will find that they are more accurate at close range. However, they
// have the advantage that at long range they don't lose any of their power
// as plasers do.
//
// When the course spreads too much, you get a misfire, and the course is
// randomized even more. You also have the chance that the misfire damages
// your torpedo tubes.

static int randcourse(int);

void torped (int v UNUSED)
{
    int ix, iy;
    float x, y, dx, dy;
    float angle;
    int course, course2;
    int k;
    float bigger;
    float sectsize;
    int burst;
    int n;

    if (Ship.cloaked) {
	printf ("Regulations do not permit attack while cloaked.\n");
	return;
    }
    if (!device_works (TORPED))
	return;
    if (Ship.torped <= 0) {
	printf ("All torpedoes expended\n");
	return;
    }

    // get the course
    course = getintpar("Torpedo course");
    if (course < 0 || course > 360)
	return;
    burst = -1;

    // need at least three torpedoes for a burst
    if (Ship.torped < 3) {
	printf ("No-burst mode selected\n");
	burst = 0;
    } else {
	// see if the user wants one
	if (!testnl()) {
	    k = ungetc(getchar(), stdin);
	    if (k >= '0' && k <= '9')
		burst = 1;
	}
    }
    if (burst < 0) {
	burst = getynpar("Do you want a burst");
    }
    if (burst) {
	burst = getintpar("burst angle");
	if (burst <= 0)
	    return;
	if (burst > 15) {
	    printf ("Maximum burst angle is 15 degrees\n");
	    return;
	}
    }
    sectsize = NSECTS;
    n = -1;
    if (burst) {
	n = 1;
	course -= burst;
    }
    for (; n && n <= 3; ++n) {
	// select a nice random course
	course2 = course + randcourse(n);
	angle = course2 * 0.0174532925;	// convert to radians
	dx = -cos(angle);
	dy = sin(angle);
	bigger = fabs(dx);
	x = fabs(dy);
	if (x > bigger)
	    bigger = x;
	dx /= bigger;
	dy /= bigger;
	x = Ship.sectx + 0.5;
	y = Ship.secty + 0.5;
	if (Ship.cond != DOCKED)
	    Ship.torped -= 1;
	printf ("Torpedo track");
	if (n > 0)
	    printf (", torpedo number %d", n);
	printf (":\n%6.1f\t%4.1f\n", x, y);
	while (1) {
	    ix = x += dx;
	    iy = y += dy;
	    if (x < 0.0 || x >= sectsize || y < 0.0 || y >= sectsize) {
		printf ("Torpedo missed\n");
		break;
	    }
	    printf ("%6.1f\t%4.1f\n", x, y);
	    switch (Sect[ix][iy]) {
		case EMPTY:
		    continue;

		case HOLE:
		    printf ("Torpedo disappears into a black hole\n");
		    break;

		case PIRATE:
		    for (k = 0; k < Etc.npirates; ++k) {
			if (Etc.pirate[k].x != ix || Etc.pirate[k].y != iy)
			    continue;
			Etc.pirate[k].power -= 500 + nrand(501);
			if (Etc.pirate[k].power > 0) {
			    printf ("*** Hit on pirate at %d,%d: extensive damages\n", ix, iy);
			    break;
			}
			kill_pirate (ix, iy);
			break;
		    }
		    break;

		case STAR:
		    nova(ix, iy);
		    break;

		case INHABIT:
		    kill_starsystem (ix, iy, -1);
		    break;

		case BASE:
		    kill_starbase (Ship.quadx, Ship.quady);
		    ++Game.bases_killed;
		    break;
		default:
		    printf ("Unknown object %c at %d,%d destroyed\n", Sect[ix][iy], ix, iy);
		    Sect[ix][iy] = EMPTY;
		    break;
	    }
	    break;
	}
	if (device_damaged (TORPED) || Quad[Ship.quadx][Ship.quady].stars < 0)
	    break;
	course += burst;
    }
    Move.free = 0;
}

// RANDOMIZE COURSE
//
// This routine randomizes the course for torpedo number 'n'.
// Other things handled by this routine are misfires, damages
// to the tubes, etc.
static int randcourse (int n)
{
    float r;
    int d;

    d = ((fnrand() + fnrand()) - 1.0) * 20;
    if (abs(d) > 12) {
	printf ("Torpedo tubes misfire");
	if (n < 0)
	    printf ("\n");
	else
	    printf (" on torpedo %d\n", n);
	if (nrand(2)) {
	    damage_device (TORPED, 0.2 * abs(d) * (fnrand() + 1.0));
	}
	d *= 1.0 + 2.0 * fnrand();
    }
    if (Ship.shldup || Ship.cond == DOCKED) {
	r = Ship.shield;
	r = 1.0 + r / Param.shield;
	if (Ship.cond == DOCKED)
	    r = 2.0;
	d *= r;
    }
    return d;
}

//}}}-------------------------------------------------------------------
//{{{ Shields and cloaking

// SHIELD AND CLOAKING DEVICE CONTROL
//
// 'f' is one for auto shield up (in case of Condition RED),
// zero for shield control, and negative one for cloaking
// device control.
//
// Called with an 'up' or 'down' on the same line, it puts
// the shields/cloak into the specified mode. Otherwise it
// reports to the user the current mode, and asks if she wishes
// to change.
//
// This is not a free move. Hits that occur as a result of
// this move appear as though the shields are half up/down,
// so you get partial hits.

void shield (int f)
{
    if (f > 0 && (Ship.shldup || device_damaged (SRSCAN)))
	return;
    const char *device, *dev2, *dev3;
    char *stat;
    int ind;
    if (f < 0) {
	// cloaking device
	device = "Cloaking device";
	dev2 = "is";
	ind = CLOAK;
	dev3 = "it";
	stat = &Ship.cloaked;
    } else {
	// shields
	device = "Shields";
	dev2 = "are";
	dev3 = "them";
	ind = SHIELD;
	stat = &Ship.shldup;
    }
    if (device_damaged (ind)) {
	if (f <= 0)
	    announce_device_damage (ind);
	return;
    }
    if (Ship.cond == DOCKED) {
	printf ("%s %s down while docked\n", device, dev2);
	return;
    }
    bool enable;
    if (f <= 0 && !testnl()) {
	static const struct cvntab Udtab[] = {
	    {"u","p",	(cmdfun) 1, 0},
	    {"d","own",	(cmdfun) 0, 0},
	    {}
	};
	enable = (intptr_t) getcodpar ("Up or down", Udtab)->value;
    } else {
	char s [100];
	if (*stat)
	    snprintf (ArrayBlock(s), "%s %s up. Do you want %s down", device, dev2, dev3);
	else
	    snprintf (ArrayBlock(s), "%s %s down. Do you want %s up", device, dev2, dev3);
	if (!getynpar (s))
	    return;
	enable = !*stat;
    }
    if (*stat == enable) {
	printf ("%s already ", device);
	if (enable)
	    printf ("up\n");
	else
	    printf ("down\n");
	return;
    }
    if (enable) {
	if (f >= 0)
	    Ship.energy -= Param.shupengy;
	else
	    Ship.cloakgood = 0;
    }
    Move.free = 0;
    if (f >= 0)
	Move.shldchg = 1;
    *stat = enable;
}

//}}}-------------------------------------------------------------------
//{{{ Emergency operations

// call starbase for help
//
// First, the closest starbase is selected. If there is a
// a starbase in your own quadrant, you are in good shape.
// This distance takes quadrant distances into account only.
//
// A magic number is computed based on the distance which acts
// as the probability that you will be rematerialized. You
// get three tries.
//
// When it is determined that you should be able to be remater-
// ialized (i.e., when the probability thing mentioned above
// comes up positive), you are put into that quadrant (anywhere).
// Then, we try to see if there is a spot adjacent to the star-
// base. If not, you can't be rematerialized!!! Otherwise,
// it drops you there. It only tries five times to find a spot
// to drop you. After that, it's your problem.

void help (int v UNUSED)
{
    // Check to see if calling for help is reasonable ...
    if (Ship.cond == DOCKED) {
	printf ("But Captain, we're already docked\n");
	return;
    }
    // ... or possible
    if (device_damaged (SSRADIO)) {
	announce_device_damage (SSRADIO);
	return;
    }
    if (Now.bases <= 0) {
	printf ("Captain, there is no response to our distress call.\n");
	return;
    }

    // Tut tut, there goes the score
    ++Game.helps;

    // Dematerialize the ship
    Sect[Ship.sectx][Ship.secty] = EMPTY;

    // Find the destination base
    float dist = 0;
    unsigned l = 0;
    if (Quad[Ship.quadx][Ship.quady].bases <= 0) {
	dist = 100;	// search for closest base
	for (unsigned i = 0; i < Now.bases; ++i) {
	    int dx = Now.base[i].x - Ship.quadx;
	    int dy = Now.base[i].y - Ship.quady;
	    float d = sqrtf (dx*dx + dy*dy);
	    if (d < dist) {
		dist = d;
		l = i;
	    }
	}
	// Go to that quadrant
	Ship.quadx = Now.base[l].x;
	Ship.quady = Now.base[l].y;
	initquad (true);
    }
    printf ("Starbase in %d,%d responds\n", Ship.quadx, Ship.quady);

    // This next thing acts as a probability that it will work
    float x = pow(1.0 - pow(0.94, dist), 0.3333333);

    // attempt to rematerialize
    static const char Cntvect[3][8] = { "First", "Second", "Third" };
    for (unsigned i = 0; i < ArraySize(Cntvect); ++i) {
	sleep(2);
	printf ("%s attempt to rematerialize ", Cntvect[i]);
	if (fnrand() > x) {
	    // ok, that's good. let's see if we can set her down
	    unsigned j = 0;
	    int dx, dy;
	    for (; j < 5; ++j) {
		dx = Etc.starbase.x + nrand(3) - 1;
		if (dx < 0 || dx >= NSECTS)
		    continue;
		dy = Etc.starbase.y + nrand(3) - 1;
		if (dy < 0 || dy >= NSECTS || Sect[dx][dy] != EMPTY)
		    continue;
		break;
	    }
	    if (j < 5) {
		// found an empty spot
		printf ("succeeds\n");
		Ship.sectx = dx;
		Ship.secty = dy;
		Sect[dx][dy] = YOURSHIP;
		dock(0);
		comp_pirate_dist(0);
		return;
	    }
	    // the starbase must have been surrounded
	}
	printf ("fails\n");
    }

    // One, two, three strikes, you're out
    lose (L_NOHELP);
}

// Self Destruct Sequence
//
// The computer starts up the self destruct sequence. Obviously,
// if the computer is out nothing can happen. You get a countdown
// and a request for password. This must match the password that
// you entered at the start of the game.
//
// You get to destroy things when you blow up; hence, it is
// possible to win the game by destructing if you take the last
// pirate with you.
//
// By the way, the \032 in the message is a ^Z, which is because
// the terminal in my office is an ADM-3, which uses that char-
// acter to clear the screen. I also stick in a \014 (form feed)
// because that clears some other screens.
//
// Uses trace flag 41

void destruct(int v UNUSED)
{
    if (device_damaged (COMPUTER)) {
	announce_device_damage (COMPUTER);
	return;
    }
    printf ("\n\07 --- WORKING ---\07\n");
    sleep(3);
    // output the count 10 9 8 7 6
    for (unsigned i = 10; i > 5; --i) {
	for (unsigned j = 10; j > i; --j)
	    printf ("   ");
	printf ("%u\n", i);
	sleep(1);
    }
    // check for password on new line only
    skiptonl(0);
    char checkpass[15];
    getstrpar ("For verification, enter password", checkpass, 14, 0);
    sleep(2);
    if (strcmp (checkpass, "password") != 0) {
	printf ("Incorrect. Self destruct sequence aborted.\n");
	return;
    }
    printf ("Password verified; self destruct sequence continues:\n");
    sleep(2);
    // output count 5 4 3 2 1 0
    for (unsigned i = 6; i-- > 0;) {
	sleep(1);
	for (unsigned j = 5; j > i; --j)
	    printf ("   ");
	printf ("%u\n", i);
    }
    sleep(2);
    printf ("\032\014***** Your ship is destroyed *****\n");
    Game.killed = true;
    // let's see what we can blow up!!!!
    float zap = 20.0 * Ship.energy;
    Game.deaths += Ship.crew;
    for (int i = 0; i < Etc.npirates;) {
	if (Etc.pirate[i].power * Etc.pirate[i].dist <= zap)
	    kill_pirate (Etc.pirate[i].x, Etc.pirate[i].y);
	else
	    ++i;
    }
    // Lose if we didn't kill the last pirate (detected by kill_pirate)
    lose (L_DSTRCT);
}

//}}}-------------------------------------------------------------------
