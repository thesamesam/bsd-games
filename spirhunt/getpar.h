// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"

typedef void (*cmdfun)(int);

struct cvntab {	// used for getcodpar() parameter list
    const char*	abrev;
    const char*	full;
    cmdfun	value;
    int		value2;
};

// getpar.c
int getintpar (const char *s);
float getfltpar (const char *s);
bool getynpar (const char *s);
const struct cvntab *getcodpar (const char *s, const struct cvntab *tab);
void getstrpar (const char *s, char *r, unsigned l, const char *t);
bool testnl (void);
void skiptonl (int c);
bool readdelim (char d);
int getcodi (int *co, float *di);
