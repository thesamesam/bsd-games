// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"

typedef void (*cmdfun) (int);
struct cvntab {			// used for getcodpar() parameter list
    const char *abrev;
    const char *full;
    cmdfun value;
    int value2;
};

extern const struct cvntab Skitab[];
extern const struct cvntab Lentab[];

int getintpar(const char *);
double getfltpar(const char *);
int getynpar(const char *);
const struct cvntab *getcodpar(const char *, const struct cvntab[]);
void getstrpar(const char *, char *, int, const char *);
int testnl(void);
void skiptonl(int);
int readdelim(int);
