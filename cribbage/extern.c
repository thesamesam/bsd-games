// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "deck.h"
#include "cribbage.h"
#include <curses.h>

bool explain = false;	// player mistakes explained
bool iwon = false;		// if comp won last game
bool quiet = false;		// if suppress random mess
bool rflag = false;		// if all cuts random

char explan[128];		// explanation

int cgames = 0;			// number games comp won
int cscore = 0;			// comp score in this game
int gamecount = 0;		// number games played
int glimit = LGAME;		// game playe to glimit
int knownum = 0;		// number of cards we know
int pgames = 0;			// number games player won
int pscore = 0;			// player score in this game

CARD chand[FULLHAND];		// computer's hand
CARD crib[CINHAND];		// the crib
CARD deck[CARDS];		// a deck
CARD known[CARDS];		// cards we have seen
CARD phand[FULLHAND];		// player's hand
CARD turnover;			// the starter

WINDOW *Compwin;		// computer's hand window
WINDOW *Msgwin;			// messages for the player
WINDOW *Playwin;		// player's hand window
WINDOW *Tablewin;		// table window
