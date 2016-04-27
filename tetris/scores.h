// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <sys/types.h>

// Tetris scores.
struct highscore {
    char hs_name[20];		// login name
    int hs_score;		// raw score
    int hs_level;		// play level
    time_t hs_time;		// time at game end
};

enum {
    MAXHISCORES	= 80,
    MAXSCORES	= 9,	// maximum high score entries per person
    EXPIRATION	= 5 * 365 * 24 * 60 * 60
};

void savescore(int);
void showscores(int);
