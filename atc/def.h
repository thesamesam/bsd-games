// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <math.h>

#define AUTHOR_STR		"ATC - by Ed James"

enum {
    DIR_NORTH,
    DIR_NE,
    DIR_EAST,
    DIR_SE,
    DIR_SOUTH,
    DIR_SW,
    DIR_WEST,
    DIR_NW,
    MAXDIR
};
enum {
    DEGREES_STEP	= 45,
    DEGREES_NORTH	= DEGREES_STEP* DIR_NORTH,
    DEGREES_NE		= DEGREES_STEP* DIR_NE,
    DEGREES_EAST	= DEGREES_STEP* DIR_EAST,
    DEGREES_SE		= DEGREES_STEP* DIR_SE,
    DEGREES_SOUTH	= DEGREES_STEP* DIR_SOUTH,
    DEGREES_SW		= DEGREES_STEP* DIR_SW,
    DEGREES_WEST	= DEGREES_STEP* DIR_WEST,
    DEGREES_NW		= DEGREES_STEP* DIR_NW
};

enum {
    D_LEFT = 1,
    D_RIGHT,
    D_UP,
    D_DOWN
};

enum {
    T_NODEST,
    T_BEACON,
    T_EXIT,
    T_AIRPORT
};

#undef S_NONE
#undef S_GONE
#undef S_MARKED
#undef S_UNMARKED
#undef S_IGNORED
enum {
    S_NONE,
    S_GONE,
    S_MARKED,
    S_UNMARKED,
    S_IGNORED
};

enum {
    INPUT_LINES	= 3,
    PLANE_COLS	= 20,
    LOWFUEL	= 15,
    REALLOC	= 10
};

#define DIR_FROM_DXDY(dx,dy)	((int) (atan2((double)(dy), (double)(dx)) \
				* MAXDIR / (2 * M_PI) + 2.5 + MAXDIR) % MAXDIR)
#define SGN(x)			((x < 0) ? -1 : ((x > 0) ? 1 : 0))
#define ABS(x)			((x < 0) ? -(x) : (x))
