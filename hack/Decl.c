// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#include "hack.h"

char nul [40];		// contains zeros
char plname [PL_NSIZ];	// player name
char lock [PL_NSIZ + 4] = "1lock";	// long enough for login name .99

bool in_mklev, restoring;

struct rm levl [COLNO][ROWNO];	// level map
#ifndef QUEST
#include "mkroom.h"
struct mkroom rooms [MAXNROFROOMS+1];
coord doors [DOORMAX];
#endif				// QUEST
struct monst* fmon = NULL;
struct trap* ftrap = NULL;
struct gold* fgold = NULL;
struct obj* fobj = NULL;
struct obj* fcobj = NULL;
struct obj* invent = NULL;
struct obj* uwep = NULL;
struct obj* uarm = NULL;
struct obj* uarm2 = NULL;
struct obj* uarmh = NULL;
struct obj* uarms = NULL;
struct obj* uarmg = NULL;
struct obj* uright = NULL;
struct obj* uleft = NULL;
struct obj* uchain = NULL;
struct obj* uball = NULL;
struct flag flags;
struct you u;
struct monst youmonst;		// dummy; used as return value for boomhit

xchar dlevel = 1;
xchar xupstair, yupstair, xdnstair, ydnstair;
char* save_cm = NULL;
const char* killer = NULL;
const char* nomovemsg = NULL;

long moves = 1;
long wailmsg = 0;

int multi = 0;
char genocided [60];
char fut_geno [60];

xchar curx = 0, cury = 0;
xchar seelx = 0, seehx = 0, seely = 0, seehy = 0;	// corners of lit room

coord bhitpos = {0,0};

const char quitchars[] = " \r\n\033";
