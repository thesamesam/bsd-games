// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
// Re-coding of advent in C: data initialization

#include "hdr.h"
#include "extern.h"
#include <signal.h>
#include <time.h>

int blklin = true;

const int setbit[16] = { 1, 2, 4, 010, 020, 040, 0100, 0200, 0400, 01000, 02000, 04000, 010000, 020000, 040000, 0100000 };

int datfd;			// message file descriptor
volatile sig_atomic_t delhit = 0;

int loc, newloc = 1, oldloc, oldlc2, wzdark, gaveup, kq, k, k2;
char *wd1 = NULL, *wd2 = NULL;	// the complete words
int verb = 0, obj = 0, spk = 0;
int mxscor = 0;

const struct MapDestEntry* tkk = NULL;	// travel is closer to keys(...)

int atloc[LOCSIZ];

int fixed[101];	// location fixed?

int hinted[20], hintlc[20];

int place[101], prop[101], links[201];
int abb[LOCSIZ];

const int maxtrs = 79;
int tally = 0, tally2 = 0;	// treasure values

// mnemonics
int keys, lamp, grate, cage, rod, rod2, steps, bird, door, pillow, snake,
    fissur, tablet, clam, oyster, magzin, dwarf, knife, food, bottle,
    water, oil, plant, plant2, axe, mirror, dragon, chasm, troll, troll2,
    bear, messag, vend, batter, nugget, coins, chest, eggs, tridnt, vase,
    emrald, pyram, pearl, rug, chain, spices, back, look, cave, null,
    entrnc, dprssn, enter, stream, pour, say, lock, throw, find, invent;

// dwarf stuff
int chloc = 114;
int chloc2 = 140;
int dseen[7] = { false };
int dloc[7] = { 0, 19, 27, 33, 44, 64, 114 };
int odloc[7] = { 0 };
int dflag = 0;
int daltlc = 18;
int tk[21], stick, dtotal, attack;

// various flags and counters
int turns = 0;
int lmwarn = false;
int iwest = 0;
int knfloc = 0;
int detail = 0;
int abbnum = 5;
int maxdie = 3;
int numdie = 0;
int holdng = 0;
int dkill = 0;
int foobar = 0;
int bonus = 0;
int clock1 = 30;
int clock2 = 50;
int saved = false;
int closng = false;
int panic = false;
int closed = false;
int scorng = false;

int limit = 330;

void init(void)			// everything for 1st time run
{
    // array linkages
    for (int j = 100; j > 0; --j) {
	if (fixd[j] > 0) {
	    drop(j + 100, fixd[j]);
	    drop(j, plac[j]);
	}
    }
    for (int j = 100; j > 0; --j) {
	fixed[j] = fixd[j];
	if (plac[j] != 0 && fixd[j] <= 0)
	    drop(j, plac[j]);
    }

    for (int i = 50; i <= maxtrs; ++i) {
	if (ptext[i])
	    prop[i] = -1;
	tally -= prop[i];
    }

    // define mnemonics
    keys = vocab("keys", 1);
    lamp = vocab("lamp", 1);
    grate = vocab("grate", 1);
    cage = vocab("cage", 1);
    rod = vocab("rod", 1);
    rod2 = rod + 1;
    steps = vocab("steps", 1);
    bird = vocab("bird", 1);
    door = vocab("door", 1);
    pillow = vocab("pillo", 1);
    snake = vocab("snake", 1);
    fissur = vocab("fissu", 1);
    tablet = vocab("table", 1);
    clam = vocab("clam", 1);
    oyster = vocab("oyste", 1);
    magzin = vocab("magaz", 1);
    dwarf = vocab("dwarf", 1);
    knife = vocab("knife", 1);
    food = vocab("food", 1);
    bottle = vocab("bottl", 1);
    water = vocab("water", 1);
    oil = vocab("oil", 1);
    plant = vocab("plant", 1);
    plant2 = plant + 1;
    axe = vocab("axe", 1);
    mirror = vocab("mirro", 1);
    dragon = vocab("drago", 1);
    chasm = vocab("chasm", 1);
    troll = vocab("troll", 1);
    troll2 = troll + 1;
    bear = vocab("bear", 1);
    messag = vocab("messa", 1);
    vend = vocab("vendi", 1);
    batter = vocab("batte", 1);

    nugget = vocab("gold", 1);
    coins = vocab("coins", 1);
    chest = vocab("chest", 1);
    eggs = vocab("eggs", 1);
    tridnt = vocab("tride", 1);
    vase = vocab("vase", 1);
    emrald = vocab("emera", 1);
    pyram = vocab("pyram", 1);
    pearl = vocab("pearl", 1);
    rug = vocab("rug", 1);
    chain = vocab("chain", 1);

    back = vocab("back", 0);
    look = vocab("look", 0);
    cave = vocab("cave", 0);
    null = vocab("null", 0);
    entrnc = vocab("entra", 0);
    dprssn = vocab("depre", 0);
    enter = vocab("enter", 0);

    pour = vocab("pour", 2);
    say = vocab("say", 2);
    lock = vocab("lock", 2);
    throw = vocab("throw", 2);
    find = vocab("find", 2);
    invent = vocab("inven", 2);
}

void trapdel(int n UNUSED)
{
    delhit = 1;		       // main checks, treats as QUIT
    signal(SIGINT, trapdel);   // catch subsequent DELs
}

void startup(void)
{
    saved = -1;
    srand (time(NULL) + getpid());	// random seed
    hinted[3] = yes(65, 1, 0);
    if (hinted[3])
	limit = 1000;	       // better batteries if instrucs
}
