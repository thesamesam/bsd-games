// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"

#define _PATH_SCOREFILE	_PATH_GAME_STATE "tetris-bsd.scores"

struct highscore {
    char	name [16];	// login name
    uint32_t	score;		// raw score
    uint32_t	level;		// play level
    time_t	time;		// time at game end
};

enum {
    MAXHISCORES	= 20,
    EXPIRATION	= 7 * 365*24*60*60	// 7 years
};

extern struct highscore _scores [MAXHISCORES+1];
extern unsigned _nScores;

void savescore (unsigned score, unsigned level);
