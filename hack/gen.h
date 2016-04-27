// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once

struct gen {
    struct gen *ngen;
    xchar gx, gy;
    unsigned gflag;
};

// Values for gen.gflag
enum {
    TRAPTYPE	= 037,
    SEEN	= 040,
    ONCE	= 0100
};

extern struct gen *fgold, *ftrap;
#define newgen()	(struct gen *) alloc(sizeof(struct gen))
