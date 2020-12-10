// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"

static int compare_pirates (const void* vp1, const void* vp2)
{
    const struct Pirate *p1 = vp1, *p2 = vp2;
    return sign (sector_distance (p1->sect, Ship.sect) - sector_distance (p2->sect, Ship.sect));
}

// sort pirates by distance
void sort_pirates (void)
{
    qsort (Etc.pirate, current_quad()->pirates, sizeof(Etc.pirate[0]), compare_pirates);
}

static unsigned ship_device_to_damage (void)
{
    //{{{ Damage probability per ship device
    // these probabilities must sum to 1000
    static const uint8_t c_damprob [NDEV] = {
	90,	// warp drive		 9.0%
	110,	// short range scanners	11.0%
	110,	// long range scanners	11.0%
	125,	// plasers		12.5%
	125,	// torpedoes		12.5%
	75,	// impulse engines	 7.5%
	150,	// shield control	15.0%
	20,	// computer		 2.0%
	35,	// radio		 3.5%
	30,	// life support		 3.0%
	50,	// cloak		 5.0%
	80,	// transporter		 8.0%
	0	// shuttle		 0.0%
    };
    //}}}
    // select a device from probability vector
    unsigned l = 0;
    for (unsigned p = nrand(1000); p > c_damprob[l]; ++l)
	p -= c_damprob[l];
    return l;
}

// Pirate Attack Routine
//
// This routine performs the pirate attack provided that
// (1) Something happened this move (i.e., not free), and
// (2) You are not cloaked. Note that if you issue the
// cloak command, you are not considered cloaked until you
// expend some time.
//
// Pirates are permitted to move both before and after the
// attack. They will tend to move toward you before the
// attack and away from you after the attack.
//
// Under certain conditions you can get a critical hit. This
// sort of hit damages devices. The probability that a given
// device is damaged depends on the device. Well protected
// devices (such as the computer, which is in the core of the
// ship and has considerable redundancy) almost never get
// damaged, whereas devices which are exposed (such as the
// warp engines) or which are particularly delicate (such as
// the transporter) have a much higher probability of being
// damaged.
//
// The actual amount of damage (i.e., how long it takes to fix
// it) depends on the amount of the hit and the "damfac[]"
// entry for the particular device.
//
// Casualties can also occur.
//
void attack (int resting)
{
    if (Move.free)
	return;
    if (!current_quad()->pirates || current_quad()->stars == SUPERNOVA)
	return;
    if (Ship.cloaked && Ship.cloakgood)
	return;

    // Move before attack
    move_pirates (0);
    if (Ship.cond == DOCKED) {
	if (!resting)
	    printf ("You are protected by starbase shields\n");
	return;
    }

    // Shields do not raise instantaneously, with effectiveness increasing over time
    float chgfac = 1.0;
    if (Move.shldchg)
	chgfac = 0.25 + 0.50 * fnrand();

    unsigned maxhit = 0, tothit = 0;

    // Let each pirate do his damndest
    for (int i = 0; i < current_quad()->pirates; ++i) {
	struct Pirate* p = &Etc.pirate[i];
	if (p->power < 20)
	    continue; // no power to attack with
	if (!device_damaged (SRSCAN))
	    printf ("Pirate at " SECT_FMT " fires plasers, and ", p->sect.x, p->sect.y);

	// Run a line to see what gets hit
	struct line_iterator li = make_line_iterator (p->sect, Ship.sect);
	enum SectorContents sc = EMPTY;
	do {
	    advance_line_iterator (&li);
	} while (li.p.x < NSECTS && li.p.y < NSECTS && ((sc = sector_contents (li.p.x, li.p.y)) == EMPTY || !nrand(32)));

	if (!device_damaged (SRSCAN)) {
	    if (sc == EMPTY)
		printf ("misses\n");
	    else if (sc == YOURSHIP)
		printf ("hits you\n");
	    else if (sc == STAR)
		printf ("hits a star\n");
	    else if (sc == BASE)
		printf ("hits the starbase\n");
	    else if (sc == INHABIT)
		printf ("hits the planet\n");
	    else if (sc == HOLE)
		printf ("hits a black hole\n");
	}

	// Calculate effectiveness
	unsigned eff = plaser_effectiveness (sector_distance (p->sect, Ship.sect));
	unsigned hit = p->power * (eff - nrand(eff/8)) / 128;
	p->power *= Param.plasfac * (1.0 + (fnrand() - 0.5) * 0.2);

	if (sc != YOURSHIP)
	    continue;

	// See how much of hit shields will absorb
	unsigned shldabsb = 0;
	if (Ship.shldup || Move.shldchg) {
	    shldabsb = min_u (chgfac * hit * Ship.shield / Param.shield, Ship.shield);
	    Ship.shield -= shldabsb;
	    hit -= shldabsb;
	}
	hit = min_u (hit, Ship.energy);

	// Keep track of hits to assess casualties
	tothit += hit;
	if (hit > maxhit)
	    maxhit = hit;

	// Actually do the hit
	printf (BOLD_ON "HIT:" BOLD_OFF " %u units", hit+shldabsb);
	if (shldabsb > 0)
	    printf (", shields absorb %u%%", shldabsb*100/(hit+shldabsb));
	printf ("\n");
	if (!(Ship.energy -= hit))
	    return lose (L_DSTRYD);

	// Check damages
	if (hit >= 13*(25-nrand(12))) {
	    printf (BOLD_ON "CRITICAL HIT!!!" BOLD_OFF "\n");
	    unsigned l = ship_device_to_damage();
	    damage_device (l, (hit * Param.damfac[l]) / (75 + nrand(25)) + 0.5);
	    if (device_damaged (SHIELD)) {
		if (Ship.shldup)
		    printf ("Shields knocked down, captain.\n");
		Ship.shield = 0;
		Ship.shldup = 0;
		Move.shldchg = 0;
	    }
	}
    }

    // Big hits, or a lot of smaller ones, kill crew
    if (maxhit >= 200 || tothit >= 500) {
	unsigned cas = min_u (tothit * fnrand() * 0.015, Ship.crew);
	if (cas >= 2) {
	    printf ("%u casualties reported.\n", cas);
	    Game.deaths += cas;
	    Ship.crew -= cas;
	}
    }

    // Allow pirates to move after attacking
    move_pirates (1);
}

// Move Pirates Around
//
// This is a largely incomprehensible block of code that moves
// pirates around in a quadrant. It was written in a very
// "program as you go" fashion, and is a prime candidate for
// rewriting.
//
// The flag 'fl' is zero before an attack, one after an attack,
// and two if you are leaving a quadrant. This serves to
// change the probability and distance that it moves.
//
// Basically, what it will try to do is to move a certain number
// of steps either toward you or away from you. It will avoid
// stars whenever possible. Nextx and nexty are the next
// sector to move to on a per-pirate basis; they are roughly
// equivalent to Ship.sect.x and Ship.sect.y for the starship. Lookx and
// looky are the sector that you are going to look at to see
// if you can move their. Dx and dy are the increment. Fudgex
// and fudgey are the things you change around to change your
// course around stars.

void move_pirates (int fl)
{
    for (int n = 0; n < current_quad()->pirates; ++n) {
	struct Pirate* k = &Etc.pirate[n];
	int i = 100;
	if (fl)
	    i = 100 * k->power / Param.piratepwr;
	if (nrand(i) >= (unsigned) Param.moveprob[2 * Move.newquad + fl])
	    continue;
	// compute distance to move
	int motion = nrand(75) - 25;
	motion *= sector_distance (k->sect, Ship.sect) * Param.movefac[2 * Move.newquad + fl];
	// compute direction
	float dx = Ship.sect.x - k->sect.x + nrand(3) - 1;
	float dy = Ship.sect.y - k->sect.y + nrand(3) - 1;
	float bigger = dx;
	if (dy > bigger)
	    bigger = dy;
	if (bigger == 0.0)
	    bigger = 1.0;
	dx = dx / bigger + 0.5;
	dy = dy / bigger + 0.5;
	if (motion < 0) {
	    motion = -motion;
	    dx = -dx;
	    dy = -dy;
	}
	int fudgex = 1, fudgey = 1;
	// try to move the pirate
	int nextx = k->sect.x;
	int nexty = k->sect.y;
	for (; motion > 0; --motion) {
	    int lookx = nextx + dx;
	    int looky = nexty + dy;
	    if (lookx < 0 || lookx >= NSECTS || looky < 0 || looky >= NSECTS) {
		// new quadrant
		int qx = Ship.quad.x;
		int qy = Ship.quad.y;
		if (lookx < 0)
		    qx -= 1;
		else if (lookx >= NSECTS)
		    qx += 1;
		if (looky < 0)
		    qy -= 1;
		else if (looky >= NSECTS)
		    qy += 1;
		if (qx < 0 || qx >= NQUADS || qy < 0 || qy >= NQUADS || Quad[qy][qx].stars == SUPERNOVA || Quad[qy][qx].pirates > QUAD_PIRATES - 1)
		    break;
		if (!device_damaged (SRSCAN))
		    printf ("pirate at " SECT_FMT " escapes to quadrant " QUAD_FMT "\n", k->sect.x, k->sect.y, qx, qy);
		*k = Etc.pirate[--current_quad()->pirates];
		k = 0;
		++Quad[qy][qx].pirates;
		break;
	    }
	    if (sector_contents(lookx,looky) != EMPTY) {
		lookx = nextx + fudgex;
		if (lookx < 0 || lookx >= NSECTS)
		    lookx = nextx + dx;
		if (sector_contents(lookx,looky) != EMPTY) {
		    fudgex = -fudgex;
		    looky = nexty + fudgey;
		    if (looky < 0 || looky >= NSECTS || sector_contents(lookx,looky) != EMPTY) {
			fudgey = -fudgey;
			break;
		    }
		}
	    }
	    nextx = lookx;
	    nexty = looky;
	}
	if (k) {
	    if ((k->sect.x != nextx || k->sect.y != nexty) && !device_damaged (SRSCAN))
		printf ("pirate at " SECT_FMT " moves to " SECT_FMT "\n", k->sect.x, k->sect.y, nextx, nexty);
	    k->sect.x = nextx;
	    k->sect.y = nexty;
	}
    }
    sort_pirates();
}

// Ask a pirate To Surrender (Fat chance)
//
// The Subspace Radio is needed to ask a pirate if he will kindly
// surrender. A random pirate from the ones in the quadrant is
// chosen.
//
// The pirate is requested to surrender. The probability of this
// is a function of that pirate's remaining power, our power, etc.
//
void capture (int v UNUSED)
{
    if (device_damaged (SSRADIO)) {
	announce_device_damage (SSRADIO);
	return;
    }
    if (Ship.cloaked) {
	printf ("Pirates don't talk to ghosts\n");
	return;
    }
    if (!current_quad()->pirates) {
	printf ("Getting no response, sir\n");
	return;
    }
    if (Ship.brigfree < Param.pirate_crew) {
	printf ("Captain, our brig is already full of pirates\n");
	return;
    }

    Move.free = false;
    Move.time = 0.05;

    // See if any pirate wants to surrender
    struct Pirate* sp = NULL;
    for (unsigned i = 0, npirates = current_quad()->pirates; i < npirates; ++i) {
	if (nrand(100) < Param.piratepwr * Ship.energy / (Etc.pirate[i].power * npirates * 1024)) {
	    sp = &Etc.pirate[i];
	    break;
	}
    }
    if (!sp) {
	printf ("Captain, the pirates said... uh, I don't think I can repeat that\n");
	return; // big surprise, he refuses to surrender
    }

    // Unexpectedly, the pirate surrendered
    printf ("Pirate at " SECT_FMT " surrenders\n", sp->sect.x, sp->sect.y);
    unsigned captives = Param.pirate_crew;
    if (!nrand(8)) {
	captives -= nrand (Param.pirate_crew/2);
	printf ("%u pirates commit suicide rather than be taken captive\n", Param.pirate_crew - captives);
    }
    Ship.brigfree -= captives;
    printf ("%d captives taken\n", captives);
    kill_pirate (sp->sect.x, sp->sect.y);
}
