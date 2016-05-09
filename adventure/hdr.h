// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
//   ADVENTURE -- Jim Gillogly, Jul 1977
// This program is a re-write of ADVENT, written in FORTRAN mostly by
// Don Woods of SAIL.  In most places it is as nearly identical to the
// original as possible given the language and word-size differences.
// A few places, such as the message arrays and travel arrays were changed
// to reflect the smaller core size and word size.  The labels of the
// original are reflected in this version, so that the comments of the
// fortran are still applicable here.
//
// The original FORTRAN version can be found at
// <URL:ftp://ftp.gmd.de/if-archive/games/source/advent-original.tar.gz>.

#pragma once
#include "../config.h"
#include "vocab.h"
#include <stdbool.h>
#include <signal.h>
#include <ctype.h>

extern int datfd;		// message file descriptor
extern volatile sig_atomic_t delhit;
extern int yea;

#define FLUSHLINE do { int flushline_ch; while ((flushline_ch = getchar()) != EOF && flushline_ch != '\n'); } while (0)
#define FLUSHLF   while (next()!=LF)
enum {
    TAB		= 011,
    LF		= 012,
    SHORT	= 50,		// How short is a demo game?
    MAXSTR	= 20,		// max length of user's words
    SEED	= 1815622	// "Encryption" seed
};

extern int loc, newloc, oldloc, oldlc2, wzdark, gaveup, kq, k, k2;
extern char *wd1, *wd2;		// the complete words
extern int verb, obj, spk;
extern int blklin;
extern int saveday, savet, mxscor, latncy;

enum {
    RTXSIZ	= 205,
    MAGSIZ	= 35,
    LOCSIZ	= 141,		// number of locations
    OBJSIZ	= 101		// number of objects
};

extern const char* const rtext[RTXSIZ];	// random text messages

extern const char* const mtext[MAGSIZ];	// magic messages

extern const char* const ptext[OBJSIZ];	// object descriptions

extern const char* const ltext[LOCSIZ];	// long loc description
extern const char* const stext[LOCSIZ];	// short loc descriptions

struct travlist {		// direcs & conditions of travel
    short int conditions;	// m in writeup (newloc / 1000)
    short int tloc;		// n in writeup (newloc % 1000)
    short int tverb;		// the verb that takes you there
};
const struct travlist* const travel[LOCSIZ];
const struct travlist* tkk;	// travel is closer to keys(...)

extern int atloc[LOCSIZ];

extern const short int plac[OBJSIZ];	// initial object placement
extern const short int fixd[OBJSIZ];
extern int fixed[OBJSIZ];		// location fixed?

extern const short int actspk[35];	// rtext msg for verb <n>

extern const short int cond[LOCSIZ];	// various condition bits

extern const int setbit[16];	// bit defn masks 1,2,4,...

extern const short int hints[10][5];	// info on hints
extern int hinted[20], hintlc[20];

extern int place[OBJSIZ], prop[OBJSIZ], links[201];
extern int abb[LOCSIZ];

extern const int maxtrs;
extern int tally, tally2;	// treasure values

extern int keys, lamp, grate, cage, rod, rod2, steps,	// mnemonics
    bird, door, pillow, snake, fissur, tablet, clam, oyster, magzin,
    dwarf, knife, food, bottle, water, oil, plant, plant2, axe, mirror,
    dragon, chasm, troll, troll2, bear, messag, vend, batter, nugget,
    coins, chest, eggs, tridnt, vase, emrald, pyram, pearl, rug, chain,
    spices, back, look, cave, null, entrnc, dprssn, enter, stream, pour,
    say, lock, throw, find, invent;

// dwarf stuff
extern int chloc, chloc2, dseen[7], dloc[7], odloc[7], dflag, daltlc;
extern int tk[21], stick, dtotal, attack;

// various flags and counters
extern int turns, lmwarn, iwest, knfloc, detail, abbnum, maxdie, numdie,
   holdng, dkill, foobar, bonus, clock1, clock2, saved, closng, panic, closed, scorng;

extern int demo, limit;
