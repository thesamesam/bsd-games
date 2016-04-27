// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once

struct edog {
    long hungrytime;		// at this time dog gets hungry
    long eattime;		// dog is eating
    long droptime;		// moment dog dropped object
    unsigned dropdist;		// dist of drpped obj from @
    unsigned apport;		// amount of training
    long whistletime;		// last time he whistled
};
#define	EDOG(mp)	((struct edog *)(&(mp->mextra[0])))
