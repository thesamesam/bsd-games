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
// The data file distributed with the fortran source is assumed to be called
// "glorkz" in the directory where the program is first run.
//
// The original FORTRAN version can be found at
// <URL:ftp://ftp.gmd.de/if-archive/games/source/advent-original.tar.gz>.

// hdr.h: included by c advent files

#include <signal.h>

extern int datfd;		// message file descriptor
extern volatile sig_atomic_t delhit;
extern int yea;
extern char data_file[];	// Virtual data file

#define TAB     011
#define LF      012
#define FLUSHLINE do { int flushline_ch; while ((flushline_ch = getchar()) != EOF && flushline_ch != '\n'); } while (0)
#define FLUSHLF   while (next()!=LF)

extern int loc, newloc, oldloc, oldlc2, wzdark, gaveup, kq, k, k2;
extern char *wd1, *wd2;		// the complete words
extern int verb, obj, spk;
extern int blklin;
extern int saveday, savet, mxscor, latncy;

#define SHORT 50	       // How short is a demo game?

#define MAXSTR  20	       // max length of user's words

#define HTSIZE  512	       // max number of vocab words
extern struct hashtab {		// hash table for vocabulary
    int val;			// word type &index (ktab)
    char *atab;			// pointer to actual string
} voc[HTSIZE];
#define SEED 1815622	       // "Encryption" seed

struct text {
    char *seekadr;		// Msg start in virtual disk
    int txtlen;			// length of msg starting here
};

#define RTXSIZ  205
extern struct text rtext[RTXSIZ];	// random text messages

#define MAGSIZ  35
extern struct text mtext[MAGSIZ];	// magic messages

extern int clsses;
#define CLSMAX  12
extern struct text ctext[CLSMAX];	// classes of adventurer
extern int cval[CLSMAX];

extern struct text ptext[101];	// object descriptions

#define LOCSIZ  141	       // number of locations
extern struct text ltext[LOCSIZ];	// long loc description
extern struct text stext[LOCSIZ];	// short loc descriptions

extern struct travlist {	// direcs & conditions of travel
    struct travlist *next;	// ptr to next list entry
    int conditions;		// m in writeup (newloc / 1000)
    int tloc;			// n in writeup (newloc % 1000)
    int tverb;			// the verb that takes you there
} *travel[LOCSIZ], *tkk;	// travel is closer to keys(...)

extern int atloc[LOCSIZ];

extern int plac[101];		// initial object placement
extern int fixd[101], fixed[101];	// location fixed?

extern int actspk[35];		// rtext msg for verb <n>

extern int cond[LOCSIZ];	// various condition bits

extern int setbit[16];		// bit defn masks 1,2,4,...

extern int hntmax;
extern int hints[20][5];	// info on hints
extern int hinted[20], hintlc[20];

extern int place[101], prop[101], links[201];
extern int abb[LOCSIZ];

extern int maxtrs, tally, tally2;	// treasure values

#define false   0
#define true    1

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

#define DECR(a,b,c,d,e) decr(a+'+',b+'-',c+'#',d+'&',e+'%')
