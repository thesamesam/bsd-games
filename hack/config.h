// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"

#define HACK_SCOREFILE	_PATH_GAME_STATE "hack.scores"	// the file containing the list of topscorers
#define HACK_SAVEFILE	_PATH_SAVED_GAMES "hack.save"	// saved game in user's home directory

// size of terminal screen is (at least) (ROWNO+2) by COLNO
enum {
    COLNO = 80,
    ROWNO = 22
};
