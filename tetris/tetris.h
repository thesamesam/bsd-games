// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <curses.h>

// Definitions for Tetris.

// The display (board) is composed of 23 rows of 10 columns of characters.
// Columns 1 to 10 of rows 1 to 20 are the actual playing area, where
// shapes appear.
enum {
    S_COLS	= 80,
    S_ROWS	= 24,
    B_COLS	= 10,
    B_ROWS	= 23,
    B_STARTROW	= 1,
    B_STARTCOL	= B_COLS/2,
    MINLEVEL	= 1,	// Game level must be between 1 and 9.
    MAXLEVEL	= 9,	//	This controls the initial fall rate and affects scoring.
    ROWS_PER_LEVEL= 50	// Bump level every N rows removed
};
