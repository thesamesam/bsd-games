// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "trek.h"
#include "getpar.h"
#include <setjmp.h>

// INSTRUCTION READ AND MAIN PLAY LOOP
//
// Well folks, this is it.  Here we have the guts of the game.
// This routine executes moves.  It sets up per-move variables,
// gets the command, and executes the command.  After the command,
// it calls events() to use up time, attack() to have Klingons
// attack if the move was not free, and checkcond() to check up
// on how we are doing after the move.

extern jmp_buf env;

const struct cvntab Comtab[] = {
    {"abandon", "", abandon, 0},
    {"ca", "pture", capture, 0},
    {"cl", "oak", shield, -1},
    {"c", "omputer", computer, 0},
    {"da", "mages", dcrept, 0},
    {"destruct", "", destruct, 0},
    {"do", "ck", dock, 0},
    {"help", "", help, 0},
    {"i", "mpulse", impulse, 0},
    {"l", "rscan", lrscan, 0},
    {"m", "ove", dowarp, 0},
    {"p", "hasers", phaser, 0},
    {"ram", "", dowarp, 1},
    {"dump", "", dumpgame, 0},
    {"r", "est", rest, 0},
    {"sh", "ield", shield, 0},
    {"s", "rscan", srscan, 0},
    {"st", "atus", srscan, -1},
    {"terminate", "", myreset, 0},
    {"t", "orpedo", torped, 0},
    {"u", "ndock", undock, 0},
    {"v", "isual", visual, 0},
    {"w", "arp", setwarp, 0},
    {NULL, NULL, NULL, 0}
};

void myreset (int v UNUSED)
{
    longjmp(env, 1);
}

void play()
{
    while (1) {
	Move.free = 1;
	Move.time = 0.0;
	Move.shldchg = 0;
	Move.newquad = 0;
	Move.resting = 0;
	skiptonl(0);
	const struct cvntab* r = getcodpar("\nCommand", Comtab);
	(*r->value)(r->value2);
	events(0);
	attack(0);
	checkcond();
    }
}
