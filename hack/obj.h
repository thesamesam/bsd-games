// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once
#include "config.h"

struct obj {
    struct obj *nobj;
    unsigned o_id;
    unsigned o_cnt_id;		// id of container object is in
    xchar ox, oy;
    xchar odx, ody;
    uchar otyp;
    uchar owt;
    uchar quan;			// use oextra for tmp gold objects
    schar spe;			// quality of weapon, armor or ring (+ or -)
				// number of charges for wand ( >= -1 )
				// special for uball and amulet %% BAH
    char olet;
    char invlet;
    Bitfield(oinvis, 1);	// not yet implemented
    Bitfield(odispl, 1);
    Bitfield(known, 1);	// exact nature known
    Bitfield(dknown, 1);	// color or text known
    Bitfield(cursed, 1);
    Bitfield(unpaid, 1);	// on some bill
    Bitfield(rustfree, 1);
    Bitfield(onamelth, 6);
    long age;			// creation date
    long owornmask;
    long oextra[1];		// used for name of ordinary objects - length
				// is flexible; amount for tmp gold objects
};

// obj.owornmask values
enum {
    W_ARM	= 01,
    W_ARM2	= 02,
    W_ARMH	= 04,
    W_ARMS	= 010,
    W_ARMG	= 020,
    W_ARMOR	= W_ARM| W_ARM2| W_ARMH| W_ARMS| W_ARMG,
    W_RINGL	= 010000,	// make W_RINGL = RING_LEFT (see uprop)
    W_RINGR	= 020000,
    W_RING	= W_RINGL| W_RINGR,
    W_WEP	= 01000,
    W_BALL	= 02000,
    W_CHAIN	= 04000
};

extern struct obj *fobj;

#define newobj(xl)	(struct obj *) alloc((unsigned)(xl) + sizeof(struct obj))
#define	ONAME(otmp)	((char *) otmp->oextra)
#define	OGOLD(otmp)	(otmp->oextra[0])
