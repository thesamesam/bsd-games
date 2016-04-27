// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include "driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define couldwin(f,t) (f->specs->crew2 > t->specs->crew2 * 1.5)

static int str_end(const char *);
static int score(struct ship *, struct ship *, char *, int);
static void move_ship(struct ship *, const char *, unsigned char *, short *, short *, char *);
static void try(struct ship *, struct ship *, char *, char *, int, int, int, int, int, int *, int);
static void rmend(char *);

const int dtab[] = { 0, 1, 1, 2, 3, 4, 4, 5 };	// diagonal distances in x==y

void thinkofgrapples(void)
{
    struct ship *sp, *sq;
    char friendly;

    foreachship(sp) {
	if (sp->file->captain[0] || sp->file->dir == 0)
	    continue;
	foreachship(sq) {
	    friendly = sp->nationality == capship(sq)->nationality;
	    if (!friendly) {
		if (sp->file->struck || sp->file->captured != 0)
		    continue;
		if (range(sp, sq) != 1)
		    continue;
		if (grappled2(sp, sq))
		    if (is_toughmelee(sp, sq, 0, 0))
			ungrap(sp, sq);
		    else
			grap(sp, sq);
		else if (couldwin(sp, sq)) {
		    grap(sp, sq);
		    sp->file->loadwith = L_GRAPE;
		}
	    } else
		ungrap(sp, sq);
	}
    }
}

void checkup(void)
{
    struct ship *sp, *sq;
    char explode, sink;

    foreachship(sp) {
	if (sp->file->dir == 0)
	    continue;
	explode = sp->file->explode;
	sink = sp->file->sink;
	if (explode != 1 && sink != 1)
	    continue;
	if (dieroll() < 5)
	    continue;
	Write(sink == 1 ? W_SINK : W_EXPLODE, sp, 2, 0, 0, 0);
	Write(W_DIR, sp, 0, 0, 0, 0);
	if (snagged(sp))
	    foreachship(sq)
		cleansnag(sp, sq, 1);
	if (sink != 1) {
	    makemsg(sp, "exploding!");
	    foreachship(sq) {
		if (sp != sq && sq->file->dir && range(sp, sq) < 4)
		    table(sp, sq, RIGGING, L_EXPLODE, sp->specs->guns / 13, 6);
	    }
	} else
	    makemsg(sp, "sinking!");
    }
}

void prizecheck(void)
{
    struct ship *sp;

    foreachship(sp) {
	if (sp->file->captured == 0)
	    continue;
	if (sp->file->struck || sp->file->dir == 0)
	    continue;
	if (sp->specs->crew1 + sp->specs->crew2 + sp->specs->crew3 > sp->file->pcrew * 6) {
	    Writestr(W_SIGNAL, sp, "prize crew overthrown");
	    Write(W_POINTS, sp->file->captured, sp->file->captured->file->points - 2 * sp->specs->pts, 0, 0, 0);
	    Write(W_CAPTURED, sp, -1, 0, 0, 0);
	}
    }
}

static int str_end(const char *str)
{
    const char *p;

    for (p = str; *p; p++);
    return p == str ? 0 : p[-1];
}

void closeon(struct ship *from, struct ship *to, char *command, int ta, int ma, int af)
{
    int high;
    char temp[10];

    temp[0] = command[0] = '\0';
    high = -30000;
    try(from, to, command, temp, ma, ta, af, ma, from->file->dir, &high, 0);
}

static int score(struct ship *ship, struct ship *to, char *movement, int onlytemp)
{
    char drift;
    int row, col, dir, total, ran;
    struct File *fp = ship->file;

    if ((dir = fp->dir) == 0)
	return 0;
    row = fp->row;
    col = fp->col;
    drift = fp->drift;
    move_ship(ship, movement, &fp->dir, &fp->row, &fp->col, &drift);
    if (!*movement)
	strcpy(movement, "d");

    ran = range(ship, to);
    total = -50 * ran;
    if (ran < 4 && gunsbear(ship, to))
	total += 60;
    if ((ran = portside(ship, to, 1) - fp->dir) == 4 || ran == -4)
	total = -30000;

    if (!onlytemp) {
	fp->row = row;
	fp->col = col;
	fp->dir = dir;
    }
    return total;
}

static void move_ship(struct ship *ship, const char *p, unsigned char *dir, short *row, short *col, char *drift)
{
    int dist;
    char moved = 0;

    for (; *p; p++) {
	switch (*p) {
	    case 'r':
		if (++*dir == 9)
		    *dir = 1;
		break;
	    case 'l':
		if (--*dir == 0)
		    *dir = 8;
		break;
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
		moved++;
		if (*dir % 2 == 0)
		    dist = dtab[*p - '0'];
		else
		    dist = *p - '0';
		*row -= dr[*dir] * dist;
		*col -= dc[*dir] * dist;
		break;
	}
    }
    if (!moved) {
	if (windspeed != 0 && ++*drift > 2) {
	    if ((ship->specs->class >= 3 && !snagged(ship))
		|| (turn & 1) == 0) {
		*row -= dr[winddir];
		*col -= dc[winddir];
	    }
	}
    } else
	*drift = 0;
}

static void try(struct ship *f, struct ship *t, char *command, char *temp, int ma, int ta, int af, int vma, int dir, int *high, int rakeme)
{
    int new, n;
    char st[4];
#define rakeyou (gunsbear(f, t) && !gunsbear(t, f))

    if ((n = str_end(temp)) < '1' || n > '9')
	for (n = 1; vma - n >= 0; n++) {
	    sprintf(st, "%d", n);
	    strcat(temp, st);
	    new = score(f, t, temp, rakeme);
	    if (new > *high && (!rakeme || rakeyou)) {
		*high = new;
		strcpy(command, temp);
	    }
	    try(f, t, command, temp, ma - n, ta, af, vma - n, dir, high, rakeme);
	    rmend(temp);
	}
    if ((ma > 0 && ta > 0 && (n = str_end(temp)) != 'l' && n != 'r') || !strlen(temp)) {
	strcat(temp, "r");
	new = score(f, t, temp, rakeme);
	if (new > *high && (!rakeme || (gunsbear(f, t) && !gunsbear(t, f)))) {
	    *high = new;
	    strcpy(command, temp);
	}
	try(f, t, command, temp, ma - 1, ta - 1, af, min(ma - 1, maxmove(f, (dir == 8 ? 1 : dir + 1), 0)), (dir == 8 ? 1 : dir + 1), high, rakeme);
	rmend(temp);
    }
    if ((ma > 0 && ta > 0 && (n = str_end(temp)) != 'l' && n != 'r') || !strlen(temp)) {
	strcat(temp, "l");
	new = score(f, t, temp, rakeme);
	if (new > *high && (!rakeme || (gunsbear(f, t) && !gunsbear(t, f)))) {
	    *high = new;
	    strcpy(command, temp);
	}
	try(f, t, command, temp, ma - 1, ta - 1, af, (min(ma - 1, maxmove(f, (dir - 1 ? dir - 1 : 8), 0))), (dir - 1 ? dir - 1 : 8), high, rakeme);
	rmend(temp);
    }
}

static void rmend(char *str)
{
    char *p;

    for (p = str; *p; p++);
    if (p != str)
	*--p = 0;
}
