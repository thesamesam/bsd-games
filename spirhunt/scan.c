// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"

// RETRIEVE THE STARSYSTEM NAME
const char* systemname (const struct quad* q)
{
    return q->systemname ? Systemname[q->systemname] : NULL;
}

// GET SECTOR CONTENTS
//
// Gets the printable char for the contents of sector x,y

enum SectorContents sector_contents (uint8_t x, uint8_t y)
{
    if (x >= NSECTS || y >= NSECTS)
	return EMPTY;
    if (Ship.sect.x == x && Ship.sect.y == y)
	return YOURSHIP;
    const struct quad* q = current_quad();
    if (q->bases && Etc.starbase.x == x && Etc.starbase.y == y)
	return BASE;
    if (q->systemname && Etc.inhabited.x == x && Etc.inhabited.y == y)
	return INHABIT;
    for (unsigned i = 0; i < q->holes; ++i)
	if (Etc.blackholes[i].x == x && Etc.blackholes[i].y == y)
	    return HOLE;
    for (unsigned i = 0; i < q->stars; ++i)
	if (Etc.stars[i].x == x && Etc.stars[i].y == y)
	    return STAR;
    for (unsigned i = 0; i < q->pirates; ++i)
	if (Etc.pirate[i].sect.x == x && Etc.pirate[i].sect.y == y)
	    return PIRATE;
    return EMPTY;
}

// A short range scan of the current quadrant.
void srscan (int f UNUSED)
{
    if (!device_works(SRSCAN))
	return;
    printf ("\nShort range sensor scan\n");
    struct quad *q = current_quad();
    printf ("  ");
    for (unsigned i = 0; i < NSECTS; ++i)
	printf ("%d ", i);
    printf ("\n");

    static const char Color[4][8] = { "GREEN", "DOCKED", "YELLOW", "RED" };
    for (unsigned i = 0; i < NSECTS; ++i) {
	printf ("%d ", i);
	for (unsigned j = 0; j < NSECTS; ++j)
	    printf ("%c ", sector_contents(j,i));
	printf ("%d   ", i);

	switch (i) {
	    case 0:
		printf ("date          %.2f", Now.date);
		break;
	    case 1:
		printf ("condition     %s", Ship.cloaked ? "CLOAKED" : Color[Ship.cond]);
		break;
	    case 2:
		printf ("position      " FULLCOORD_FMT, Ship.quad.x, Ship.quad.y, Ship.sect.x, Ship.sect.y);
		break;
	    case 3:
		printf ("warp factor   %u.%u", Ship.warp/10, Ship.warp%10);
		break;
	    case 4:
		printf ("total energy  %u", Ship.energy);
		break;
	    case 5:
		printf ("torpedoes     %u", Ship.torped);
		break;
	    case 6: {
		const char* s = "down";
		if (Ship.shldup)
		    s = "up";
		if (device_damaged (SHIELD))
		    s = "damaged";
		unsigned percent = 100 * Ship.shield / Param.shield;
		printf ("shields       %s, %u%%", s, percent);
		} break;
	    case 7:
		printf ("pirates left	%u", pirates_remaining());
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
    printf ("  ");
    for (unsigned i = 0; i < NSECTS; ++i)
	printf ("%u ", i);
    printf ("\n");

    if (q->distressed)
	printf ("Distressed ");
    if (q->systemname)
	printf ("Starsystem %s\n", systemname(q));
}

// LONG RANGE OF SCANNERS
//
// A summary of the quadrants that surround you is printed. The first
// digit is the number of starbases in the quadrant, the second digit
// is the number of pirates. If the printout is "*" it means that that
// quadrant is rendered uninhabitable by a supernova.
//
void lrscan(int v UNUSED)
{
    bool havelrs = device_works (LRSCAN);

    // print top header
    printf ("Long range %s from quadrant " QUAD_FMT "\n  ", havelrs ? "scan" : "record", Ship.quad.x, Ship.quad.y);
    for (unsigned i = 0; i < NQUADS; ++i)
	printf (i == Ship.quad.x ? BOLD_ON "%d  " BOLD_OFF : "%d  ", i);
    printf ("\n");
    for (unsigned i = 0; i < NQUADS; ++i) {
	printf (i == Ship.quad.y ? BOLD_ON "%d " BOLD_OFF : "%d ", i);
	for (unsigned j = 0; j < NQUADS; ++j) {
	    const struct quad* q = &Quad[i][j];
	    if (q->stars == SUPERNOVA) {
		printf (BOLD_ON "*  " BOLD_OFF);
		continue;
	    }
	    // Mark current quad with bold
	    struct xy cq = {j,i};
	    unsigned dist = sector_distance (Ship.quad, cq);
	    if (!dist)
		printf (BOLD_ON);

	    // First char is starbase indicator, B or D for distressed
	    char starbase = '.';
	    if (q->distressed && q->bases)
		starbase = 'A';
	    else if (q->distressed)
		starbase = 'D';
	    else if (q->bases)
		starbase = 'B';
	    putchar (starbase);

	    // Second char is the pirates count, out to scan range only
	    char pirates = ' ';
	    if (dist <= LRSCAN_RANGE) {
		if (havelrs || (!dist && device_works(SRSCAN)))
		    pirates = q->pirates ? '0'+q->pirates : '.';
		else
		    pirates = '?';
	    }
	    putchar (pirates);

	    if (!dist)
		printf (BOLD_OFF);
	    putchar (' ');
	}
	printf (i == Ship.quad.y ? BOLD_ON "%d\n" BOLD_OFF : "%d\n", i);
    }
    printf ("  ");
    // print bottom footer
    for (unsigned i = 0; i < NQUADS; ++i)
	printf (i == Ship.quad.x ? BOLD_ON "%d  " BOLD_OFF : "%d  ", i);
    printf ("\n");
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
    int ix = Ship.sect.x + v->x;
    int iy = Ship.sect.y + v->y;
    if (ix < 0 || ix >= NSECTS || iy < 0 || iy >= NSECTS)
	co = '?';
    else
	co = sector_contents(ix,iy);
    printf (SECT_FMT " %c ", ix, iy, co);
    ++v;
    ix = Ship.sect.x + v->x;
    iy = Ship.sect.y + v->y;
    if (ix < 0 || ix >= NSECTS || iy < 0 || iy >= NSECTS)
	co = '?';
    else
	co = sector_contents(ix,iy);
    printf ("%c ", co);
    ++v;
    ix = Ship.sect.x + v->x;
    iy = Ship.sect.y + v->y;
    if (ix < 0 || ix >= NSECTS || iy < 0 || iy >= NSECTS)
	co = '?';
    else
	co = sector_contents(ix,iy);
    printf ("%c " SECT_FMT "\n", co, ix, iy);
    Move.time = 0.05;
    Move.free = 0;
}

// Returns the quad where Ship is
struct quad* current_quad (void)
    { return &Quad[Ship.quad.y][Ship.quad.x]; }

// Distance between sector coordinates
unsigned sector_distance (struct xy a, struct xy b)
{
    unsigned dx = absv(b.x-a.x), dy = absv(b.y-a.y),
	ld = max_u (dx, dy), sd = min_u (dx, dy);
    // straight+diagonal approximation (181 = sqrt(2)*128)
    return (ld-sd)+sd*181/128;
}

// Counts remaining pirates in the galaxy
unsigned pirates_remaining (void)
{
    unsigned n = 0;
    for (unsigned y = 0; y < NQUADS; ++y)
	for (unsigned x = 0; x < NQUADS; ++x)
	    n += Quad[y][x].pirates;
    return n;
}
