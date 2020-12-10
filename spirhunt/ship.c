// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"

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
    return current_quad()->bases
	&& absv (Ship.sect.x - Etc.starbase.x) <= 1
	&& absv (Ship.sect.y - Etc.starbase.y) <= 1;
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

    // Offload captured pirates
    unsigned ncaptives = Param.brigfree - Ship.brigfree;
    if (ncaptives) {
	printf ("%u captive pirates offloaded to starbase for trial\n", ncaptives);
	Game.captives += ncaptives;
	Ship.brigfree += ncaptives;
    }

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
    const struct event* lrtb = next_event_of_type (E_LRTB);
    if (lrtb && lrtb->date >= Now.date) {
	t = lrtb->date - Now.date;
	if (Ship.cond != DOCKED && Move.time > t)
	    Move.time = t + 0.0001;
    }
    Move.free = 0;
    Move.resting = 1;
}

//}}}-------------------------------------------------------------------
//{{{ Plasers

// 128-factor effectiveness at dist sectors
unsigned plaser_effectiveness (unsigned dist)
{
    static const uint8_t c_eff[] = // 128*0.9^d
	{ 128, 115, 104, 93, 84, 76, 68, 61, 55, 50, 45, 40, 36, 32, 29, 26 };
    return c_eff [min_u (dist, ArraySize(c_eff)-1)];
}

void plasers (int v UNUSED)
{
    if (device_damaged (PLASER)) {
	announce_device_damage (PLASER);
	return;
    }
    if (Ship.cond == DOCKED) {
	printf ("Plasers cannot fire through starbase shields\n");
	return;
    }
    if (Ship.cloaked) {
	printf ("Plasers are inoperative while cloaked\n");
	return;
    }
    unsigned max_targets = min_u (NBANKS, current_quad()->pirates);
    if (!max_targets) {
	printf ("There are no pirates in this quadrant\n");
	return;
    }
    if (Ship.energy < Param.energylow + max_targets) {
	printf ("Energy levels are too low to charge plaser banks\n");
	return;
    }
    unsigned energy = Ship.energy - Param.energylow;

    // Get all targets first
    printf ("Plasers locked on %u targets\n", max_targets);
    struct { uint16_t units; } bank [NBANKS] = {};
    unsigned ntargets = 0;
    for (; ntargets < max_targets; ++ntargets) {
	const struct Pirate* p = &Etc.pirate[ntargets];
	unsigned dist = sector_distance (Ship.sect, p->sect);
	unsigned eff = plaser_effectiveness (dist);
	printf ("Bank %u targets " SECT_FMT ", has %u units, %u%% eff\n", ntargets, p->sect.x, p->sect.y, energy, 100*eff/128);
	unsigned units = getintpar ("Units to fire");
	if (!units || units > energy)
	    break;
	energy -= units;
	bank[ntargets].units = units;
    }

    // Fire all banks
    unsigned wasted_energy = 0;
    for (unsigned i = 0; i < ntargets; ++i) {
	printf ("Plaser bank %u firing at " SECT_FMT " ", i, Etc.pirate[i].sect.x, Etc.pirate[i].sect.y);
	Ship.energy -= bank[i].units;
	unsigned hit = 0;
	for (struct line_iterator li = make_line_iterator (Ship.sect, Etc.pirate[i].sect);;) {
	    advance_line_iterator (&li);
	    if (!hit)
		printf (".");
	    if (li.p.x >= NSECTS || li.p.y >= NSECTS) {
		wasted_energy += bank[i].units;
		if (!hit)
		    printf (" missed\n");
		break;
	    }
	    enum SectorContents sc = sector_contents (li.p.x, li.p.y);
	    if (sc == EMPTY || !nrand(64))
		continue;
	    if (!hit)
		printf (" hit\n");
	    unsigned dist = sector_distance (Ship.sect, li.p);
	    unsigned eff = plaser_effectiveness (dist);
	    hit = bank[i].units * (eff - nrand(eff/8)) / 128;
	    if (sc == PIRATE) {
		struct Pirate* p = NULL;
		for (unsigned j = 0; j < current_quad()->pirates; ++j)
		    if (Etc.pirate[j].sect.x == li.p.x && Etc.pirate[j].sect.y == li.p.y)
			p = &Etc.pirate[j];
		if (!p) {
		    printf ("Uh... Sorry captain, I thought it hit something.\n");
		    break;
		}
		if (p->power < hit)
		    hit = p->power;
		printf ("%u unit hit on pirate", hit);
		if (!device_damaged (SRSCAN))
		    printf (" at " SECT_FMT, p->sect.x, p->sect.y);
		printf ("\n");
		p->power -= hit;
		if (!p->power)
		    kill_pirate (p->sect.x, p->sect.y);
		bank[i].units -= hit;
		if (bank[i].units)
		    continue;	// remaining energy continues throught the target
	    } else if (sc == BASE) {
		if (hit >= 350) {
		    kill_starbase (Ship.quad.x, Ship.quad.y);
		    ++Game.bases_killed;
		    bank[i].units -= hit;
		    if (bank[i].units)
			continue;
		} else
		    printf ("Plaser fire absorbed by starbase shields\n");
	    } else if (sc == STAR)
		printf ("Plaser fire absorbed by star\n");
	    else if (sc == HOLE)
		printf ("Plaser fire absorbed by black hole\n");
	    else if (sc == INHABIT)
		printf ("Plaser fire absorbed by planetary atmosphere\n");
	    else
		printf ("Unknown object '%c' at " SECT_FMT " destroyed\n", sc, li.p.x, li.p.y);
	    break;
	}
    }
    if (wasted_energy)
	printf ("%u units wasted on empty space\n", wasted_energy);
    Move.free = false;
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

void torped (int v UNUSED)
{
    if (Ship.cloaked) {
	printf ("Regulations do not permit attack while cloaked.\n");
	return;
    }
    if (!device_works (TORPED))
	return;
    if (!Ship.torped) {
	printf ("All torpedoes expended\n");
	return;
    }
    unsigned max_targets = min_u (min_u (Ship.torped, current_quad()->pirates), NTORPEDOES);
    if (!max_targets) {
	printf ("There's nobody here to torpedo, captain\n");
	return;
    }

    // get the targets
    struct xy torpt [NTORPEDOES] = {{UINT8_MAX,UINT8_MAX}};
    unsigned ntofire = 0;
    for (; ntofire < max_targets; ++ntofire) {
	printf ("Targeting torpedo tube %u\n", ntofire+1);
	torpt[ntofire] = getdest();
	if (torpt[ntofire].x >= NQUADS*NSECTS || torpt[ntofire].y >= NQUADS*NSECTS)
	    break;	// cancelled this tube
	torpt[ntofire].x %= NSECTS;	// use only sector address here
	torpt[ntofire].y %= NSECTS;
    }
    for (unsigned i = 0; i < ntofire; ++i) {
	--Ship.torped;
	printf ("Torpedo %u track:", i+1);
	for (struct line_iterator li = make_line_iterator (Ship.sect, torpt[i]);;) {
	    advance_line_iterator (&li);
	    if (li.p.x >= NSECTS || li.p.y >= NSECTS) {
		printf (" missed\n");
		break;
	    }
	    printf (" " SECT_FMT, li.p.x, li.p.y);
	    enum SectorContents sc = sector_contents (li.p.x, li.p.y);
	    if (sc == EMPTY || !nrand(64))
		continue;
	    printf (" hit\n");
	    if (sc == PIRATE) {
		for (unsigned p = 0; p < current_quad()->pirates; ++p) {
		    if (Etc.pirate[p].sect.x != li.p.x || Etc.pirate[p].sect.y != li.p.y)
			continue;
		    Etc.pirate[p].power -= min_u (Etc.pirate[p].power, 500 + nrand(501));
		    if (!Etc.pirate[p].power)
			kill_pirate (li.p.x, li.p.y);
		    else
			printf ("*** Hit on pirate at " SECT_FMT ": extensive damages\n", li.p.x, li.p.y);
		}
	    } else if (sc == STAR)
		nova (li.p.x, li.p.y);
	    else if (sc == HOLE)
		printf ("Torpedo disappears into a black hole\n");
	    else if (sc == INHABIT)
		kill_starsystem (li.p.x, li.p.y, -1);
	    else if (sc == BASE) {
		kill_starbase (Ship.quad.x, Ship.quad.y);
		++Game.bases_killed;
	    } else
		printf ("Unknown object '%c' at " SECT_FMT " destroyed\n", sc, li.p.x, li.p.y);
	    break;
	}
	if (device_damaged (TORPED) || current_quad()->stars == SUPERNOVA)
	    break;
    }
    Move.free = false;
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
// reports to the user the current mode, and asks if he wishes
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
    bool* stat;
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
    if (f <= 0) {
	static const struct cvntab Udtab[] = {
	    {"u","p",	NULL, true},
	    {"d","own",	NULL, false},
	    {}
	};
	const struct cvntab* a = getcodpar ("Up or down", Udtab);
	if (!a)
	    return;
	enable = a->value2;
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
    sleep (1);
    if (Now.bases <= 0) {
	printf ("Captain, there is no response to our distress call.\n");
	return;
    }

    // Find the closest destination base
    unsigned dist = UINT_MAX, l = 0;
    for (unsigned i = 0; i < Now.bases; ++i) {
	unsigned d = sector_distance (Ship.quad, Now.base[i]);
	if (d < dist) {
	    dist = d;
	    l = i;
	}
    }

    // Go to that quadrant
    printf ("Starbase in " QUAD_FMT " responds\n", Ship.quad.x, Ship.quad.y);
    Ship.quad = Now.base[l];
    initquad (true);
    ++Game.helps;

    // Find an empty spot around the starbase
    struct xy dest = { NSECTS, NSECTS };
    for (int dx = -1; dx <= 1; ++dx) {
	for (int dy = -1; dy <= 1; ++dy) {
	    struct xy spot = Etc.starbase;
	    if (spot.x + dx > 0 && (spot.x += dx) < NSECTS
		&& spot.y + dy > 0 && (spot.y += dy) < NSECTS
		&& sector_contents (spot.x, spot.y) == EMPTY)
		dest = spot;
	}
    }
    sleep(1);
    if (dest.x >= NSECTS || dest.y >= NSECTS) {
	printf ("Unfortunately the long range transporter malfunctions.\n");
	return lose (L_NOHELP);
    }
    printf ("Long range transport successful.\n");
    Ship.sect = dest;
    dock(0);
    sort_pirates();
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
    printf ("\n" BOLD_ON "--- WORKING ---" BOLD_OFF "\n");
    sleep(3);
    // output the count 10 9 8 7 6
    for (unsigned i = 10; i > 5; --i) {
	for (unsigned j = 10; j > i; --j)
	    printf ("   ");
	printf ("%u\n", i);
	sleep(1);
    }
    // check for password on new line only
    char checkpass[16];
    getstrpar ("For verification, enter password", ArrayBlock(checkpass)-1);
    sleep(1);
    if (strcmp (checkpass, "password") != 0) {
	printf ("Incorrect. Self destruct sequence aborted.\n");
	return;
    }
    printf ("Password verified; self destruct sequence continues:\n");
    sleep(1);
    // output count 5 4 3 2 1 0
    for (unsigned i = 6; i-- > 0;) {
	sleep(1);
	for (unsigned j = 5; j > i; --j)
	    printf ("   ");
	printf ("%u\n", i);
    }
    sleep(1);
    printf (BOLD_ON "***** Your ship is destroyed *****" BOLD_OFF "\n");
    Game.killed = true;
    // let's see what we can blow up!!!!
    float zap = 20.0 * Ship.energy;
    Game.deaths += Ship.crew;
    for (int i = 0; i < current_quad()->pirates;) {
	if (Etc.pirate[i].power * sector_distance (Ship.sect, Etc.pirate[i].sect) <= zap)
	    kill_pirate (Etc.pirate[i].sect.x, Etc.pirate[i].sect.y);
	else
	    ++i;
    }
    // Lose if we didn't kill the last pirate (detected by kill_pirate)
    lose (L_DSTRCT);
}

//}}}-------------------------------------------------------------------
