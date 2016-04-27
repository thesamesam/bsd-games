// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once
#include "config.h"

#ifndef NOWORM
// worm structure
struct wseg {
    struct wseg *nseg;
    xchar wx, wy;
    unsigned wdispl:1;
};
#define newseg()	(struct wseg *) alloc(sizeof(struct wseg))
#endif // NOWORM
