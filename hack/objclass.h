// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once
#include "config.h"

// definition of a class of objects
struct objclass {
    const char *oc_name;	// actual name
    const char *oc_descr;	// description when name unknown
    char *oc_uname;		// called by user
     Bitfield(oc_name_known, 1);
     Bitfield(oc_merge, 1);	// merge otherwise equal objects
    char oc_olet;
    schar oc_prob;		// probability for mkobj()
    schar oc_delay;		// delay when using such an object
    uchar oc_weight;
    schar oc_oc1, oc_oc2;
    int oc_oi;
#define	nutrition	oc_oi	// for foods
#define	a_ac		oc_oc1	// for armors - only used in ARM_BONUS
#define ARM_BONUS(obj)	((10 - objects[obj->otyp].a_ac) + obj->spe)
#define	a_can		oc_oc2	// for armors
#define bits		oc_oc1	// for wands and rings
#define	wldam		oc_oc1	// for weapons and PICK_AXE
#define	wsdam		oc_oc2	// for weapons and PICK_AXE
#define	g_val		oc_oi	// for gems: value on exit
};

// Some values of oc_oi
enum {
    // wands
    NODIR	= 1,
    IMMEDIATE	= 2,
    RAY		= 4,
    // rings
    SPEC	= 1	// +n is meaningful
};

extern struct objclass objects[];

// definitions of all object-symbols
enum {
    ILLOBJ_SYM	= '\\',
    AMULET_SYM	= '"',
    FOOD_SYM	= '%',
    WEAPON_SYM	= ')',
    TOOL_SYM	= '(',
    BALL_SYM	= '0',
    CHAIN_SYM	= '_',
    ROCK_SYM	= '`',
    ARMOR_SYM	= '[',
    POTION_SYM	= '!',
    SCROLL_SYM	= '?',
    WAND_SYM	= '/',
    RING_SYM	= '=',
    GEM_SYM	= '*'
};

// Other places with explicit knowledge of object symbols:
// ....shk.c:   char shtypes[] = "=/)%?![";
// mklev.c:     "=/)%?![<>"
// hack.mkobj.c:        char mkobjstr[] = "))[[!!!!????%%%%/=**";
// hack.apply.c:   otmp = getobj("0#%", "put in");
// hack.eat.c:     otmp = getobj("%", "eat");
// hack.invent.c:          if(index("!%?[)=*(0/\"", sym)){
// hack.invent.c:    || index("%?!*",otmp->olet))){
