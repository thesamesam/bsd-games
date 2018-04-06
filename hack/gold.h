// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once

struct gold {
    struct gold *ngold;
    int8_t gx, gy;
    long amount;
};

static inline struct gold* newgold (void)
    { return alloc (sizeof(struct gold)); }
