// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once
#include "config.h"

struct trap {
    struct trap *ntrap;
    xchar tx, ty;
    unsigned ttyp:5;
    unsigned tseen:1;
    unsigned once:1;
};

extern struct trap *ftrap;
#define newtrap()	(struct trap *) alloc(sizeof(struct trap))

// various kinds of traps
enum {
    BEAR_TRAP,
    ARROW_TRAP,
    DART_TRAP,
    TRAPDOOR,
    TELEP_TRAP,
    PIT,
    SLP_GAS_TRAP,
    PIERC,
    MIMIC,	// used only in mklev.c
    TRAPNUM	// if not less than 32, change sizeof(ttyp)
};		// see also mtrapseen (bit map)
