// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once

struct gold {
    struct gold *ngold;
    xchar gx, gy;
    long amount;
};

#define newgold()	(struct gold *) alloc(sizeof(struct gold))
extern struct gold *fgold;
