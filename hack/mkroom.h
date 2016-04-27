// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once

struct mkroom {
    schar lx, hx, ly, hy;	// usually xchar, but hx may be -1
    schar rtype, rlit, doorct, fdoor;
};

enum {
    MAXNROFROOMS = 15,
    DOORMAX = 100
};

extern struct mkroom rooms [MAXNROFROOMS+1];
extern coord doors [DOORMAX];

// various values of rtype
// 0: ordinary room; 8-15: various shops
// Note: some code assumes that >= 8 means shop, so be careful when adding
// new roomtypes
enum {
    SWAMP = 3,
    VAULT,
    BEEHIVE,
    MORGUE,
    ZOO,
    SHOPBASE,
    WANDSHOP,
    GENERAL = 15
};
