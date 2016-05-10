// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"

enum { BILLSZ = 200 };

struct bill_x {
    unsigned bo_id;
    unsigned useup:1;
    unsigned bquan:7;
    unsigned price;		// price per unit
};

struct eshk {
    long robbed;		// amount stolen by most recent customer
    bool following;		// following customer since he owes us sth
    schar shoproom;		// index in rooms; set by inshop()
    coord shk;			// usual position shopkeeper
    coord shd;			// position shop door
    int shoplevel;		// level of his shop
    int billct;
    struct bill_x bill[BILLSZ];
    int visitct;		// nr of visits by most recent customer
    char customer[PL_NSIZ];	// most recent customer
    char shknam[PL_NSIZ];
};
