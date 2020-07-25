// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include <math.h>

static int compare_pirates (const void* vp1, const void* vp2)
{
    const struct Pirate *p1 = vp1, *p2 = vp2;
    return sign (p1->dist - p2->dist);
}

// sort pirates by distance
static void sort_pirates (void)
{
    qsort (Etc.pirate, Etc.npirates, sizeof(Etc.pirate[0]), compare_pirates);
}

// Compute pirate distances
//
// The pirate list has the distances for all pirates recomputed
// and sorted. The parameter is a Boolean flag which is set if
// we have just entered a new quadrant.
//
// This routine is used every time a ship moves
// f - set if new quadrant
//
void comp_pirate_dist (int f)
{
    if (Etc.npirates == 0)
	return;
    for (int i = 0; i < Etc.npirates; ++i) {
	// compute distance to the pirate
	int dx = Ship.sectx - Etc.pirate[i].x;
	int dy = Ship.secty - Etc.pirate[i].y;
	float d = sqrtf (dx*dx + dy*dy);

	// compute average of new and old distances to pirate
	if (!f)
	    Etc.pirate[i].avgdist = (d + Etc.pirate[i].dist)/2;
	else // new quadrant: average is current
	    Etc.pirate[i].avgdist = d;
	Etc.pirate[i].dist = d;
    }

    // leave them sorted
    sort_pirates();
}

static unsigned ship_device_to_damage (void)
{
    //{{{ Damage probability per ship device
    // these probabilities must sum to 1000
    static const uint8_t c_damprob [NDEV] = {
	70,	// warp drive		 7.0%
	110,	// short range scanners	11.0%
	110,	// long range scanners	11.0%
	125,	// plasers		12.5%
	125,	// torpedoes		12.5%
	75,	// impulse engines	 7.5%
	150,	// shield control	15.0%
	20,	// computer		 2.0%
	35,	// radio		 3.5%
	30,	// life support		 3.0%
	20, 	// navigation system	 2.0%
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
    if (Etc.npirates <= 0 || Quad[Ship.quadx][Ship.quady].stars < 0)
	return;
    if (Ship.cloaked && Ship.cloakgood)
	return;
    // move before attack
    move_pirates (0);
    if (Ship.cond == DOCKED) {
	if (!resting)
	    printf ("You are protected by starbase shields\n");
	return;
    }

    // setup shield effectiveness
    float chgfac = 1.0;
    if (Move.shldchg)
	chgfac = 0.25 + 0.50 * fnrand();
    int maxhit = 0, tothit = 0;
    bool hitflag = false;

    // let each pirate do his damndest
    for (int i = 0; i < Etc.npirates; ++i) {
	// if he's low on power he won't attack
	if (Etc.pirate[i].power < 20)
	    continue;
	if (!hitflag) {
	    hitflag = true;
	    printf ("\npirates attack:\n");
	}
	// complete the hit
	float dustfac = 0.90 + 0.01 * fnrand();
	float tothe = Etc.pirate[i].avgdist;
	int hit = Etc.pirate[i].power * pow(dustfac, tothe) * Param.hitfac;
	// deplete his energy
	dustfac = Etc.pirate[i].power;
	Etc.pirate[i].power = dustfac * Param.plasfac * (1.0 + (fnrand() - 0.5) * 0.2);
	// see how much of hit shields will absorb
	int shldabsb = 0;
	if (Ship.shldup || Move.shldchg) {
	    float propor = Ship.shield;
	    propor /= Param.shield;
	    shldabsb = propor * chgfac * hit;
	    if (shldabsb > Ship.shield)
		shldabsb = Ship.shield;
	    Ship.shield -= shldabsb;
	}
	// actually do the hit
	printf ("HIT: %d units", hit);
	if (!device_damaged (SRSCAN))
	    printf (" from %d,%d", Etc.pirate[i].x, Etc.pirate[i].y);
	int cas = (shldabsb * 100) / hit;
	hit -= shldabsb;
	if (shldabsb > 0)
	    printf (", shields absorb %d%%, effective hit %d\n", cas, hit);
	else
	    printf ("\n");
	tothit += hit;
	if (hit > maxhit)
	    maxhit = hit;
	Ship.energy -= hit;
	// see if damages occurred
	if (hit >= (int)(13*(25-nrand(12)))) {
	    printf ("CRITICAL HIT!!!\n");
	    unsigned l = ship_device_to_damage();
	    // compute amount of damage to device
	    damage_device (l, (hit * Param.damfac[l]) / (75 + nrand(25)) + 0.5);
	    if (device_damaged (SHIELD)) {
		if (Ship.shldup)
		    printf ("Shields knocked down, captain.\n");
		Ship.shldup = 0;
		Move.shldchg = 0;
	    }
	}
	if (Ship.energy <= 0)
	    lose(L_DSTRYD);
    }

    // see what our casualities are like
    if (maxhit >= 200 || tothit >= 500) {
	int cas = tothit * 0.015 * fnrand();
	if (cas >= 2) {
	    printf ("%d casualties reported.\n", cas);
	    Game.deaths += cas;
	    Ship.crew -= cas;
	}
    }

    // allow pirates to move after attacking
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
// equivalent to Ship.sectx and Ship.secty for the starship. Lookx and
// looky are the sector that you are going to look at to see
// if you can move their. Dx and dy are the increment. Fudgex
// and fudgey are the things you change around to change your
// course around stars.

void move_pirates (int fl)
{
    for (int n = 0; n < Etc.npirates; ++n) {
	struct Pirate* k = &Etc.pirate[n];
	int i = 100;
	if (fl)
	    i = 100.0 * k->power / Param.piratepwr;
	if (nrand(i) >= (unsigned) Param.moveprob[2 * Move.newquad + fl])
	    continue;
	// compute distance to move
	int motion = nrand(75) - 25;
	motion *= k->avgdist * Param.movefac[2 * Move.newquad + fl];
	// compute direction
	float dx = Ship.sectx - k->x + nrand(3) - 1;
	float dy = Ship.secty - k->y + nrand(3) - 1;
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
	int nextx = k->x;
	int nexty = k->y;
	for (; motion > 0; --motion) {
	    int lookx = nextx + dx;
	    int looky = nexty + dy;
	    if (lookx < 0 || lookx >= NSECTS || looky < 0 || looky >= NSECTS) {
		// new quadrant
		int qx = Ship.quadx;
		int qy = Ship.quady;
		if (lookx < 0)
		    qx -= 1;
		else if (lookx >= NSECTS)
		    qx += 1;
		if (looky < 0)
		    qy -= 1;
		else if (looky >= NSECTS)
		    qy += 1;
		if (qx < 0 || qx >= NQUADS || qy < 0 || qy >= NQUADS || Quad[qx][qy].stars < 0 || Quad[qx][qy].pirates > MAX_QUAD_PIRATES - 1)
		    break;
		if (!device_damaged (SRSCAN)) {
		    printf ("pirate at %d,%d escapes to quadrant %d,%d\n", k->x, k->y, qx, qy);
		    motion = Quad[qx][qy].scanned;
		    if (motion >= 0 && motion < 1000)
			Quad[qx][qy].scanned += 100;
		    motion = Quad[Ship.quadx][Ship.quady].scanned;
		    if (motion >= 0 && motion < 1000)
			Quad[Ship.quadx][Ship.quady].scanned -= 100;
		}
		Sect[k->x][k->y] = EMPTY;
		Quad[qx][qy].pirates += 1;
		Etc.npirates -= 1;
		*k = Etc.pirate[Etc.npirates];
		Quad[Ship.quadx][Ship.quady].pirates -= 1;
		k = 0;
		break;
	    }
	    if (Sect[lookx][looky] != EMPTY) {
		lookx = nextx + fudgex;
		if (lookx < 0 || lookx >= NSECTS)
		    lookx = nextx + dx;
		if (Sect[lookx][looky] != EMPTY) {
		    fudgex = -fudgex;
		    looky = nexty + fudgey;
		    if (looky < 0 || looky >= NSECTS || Sect[lookx][looky] != EMPTY) {
			fudgey = -fudgey;
			break;
		    }
		}
	    }
	    nextx = lookx;
	    nexty = looky;
	}
	if (k && (k->x != nextx || k->y != nexty)) {
	    if (!device_damaged (SRSCAN))
		printf ("pirate at %d,%d moves to %d,%d\n", k->x, k->y, nextx, nexty);
	    Sect[k->x][k->y] = EMPTY;
	    Sect[k->x = nextx][k->y = nexty] = PIRATE;
	}
    }
    comp_pirate_dist (0);
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
    // check for not cloaked
    if (Ship.cloaked) {
	printf ("Ship-ship communications out when cloaked\n");
	return;
    }
    if (device_damaged (SSRADIO)) {
	announce_device_damage (SSRADIO);
	return;
    }
    // find out if there are any at all
    if (Etc.npirates <= 0) {
	printf ("Getting no response, sir\n");
	return;
    }

    // if there is more than one pirate, find out which one
    struct Pirate* k = &Etc.pirate [nrand (Etc.npirates)];
    Move.free = 0;
    Move.time = 0.05;

    // check out that pirate
    ++k->srndreq;
    float x = Param.piratepwr;
    x *= Ship.energy;
    x /= k->power * Etc.npirates;
    x *= Param.srndrprob;
    int i = x;
    if (i <= (int) nrand(100)) {
	printf ("Fat chance, captain\n");
	return; // big surprise, he refuses to surrender
    }
    // guess what, he surrendered!!!
    printf ("pirate at %d,%d surrenders\n", k->x, k->y);
    i = nrand (Param.pirate_crew);
    if (i > 0)
	printf ("%d pirates commit suicide rather than be taken captive\n", Param.pirate_crew - i);
    if (i > Ship.brigfree)
	i = Ship.brigfree;
    Ship.brigfree -= i;
    printf ("%d captives taken\n", i);
    kill_pirate (k->x, k->y);
}
