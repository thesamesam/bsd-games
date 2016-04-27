// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"

struct situatn {
    int brd[26];
    int roll1;
    int roll2;
    int mp[4];
    int mg[4];
    int new1;
    int new2;
    const char* const* com;
};

extern const char *const doubl[];
extern const char *const endgame[];
extern const char *const finis[];
extern const char *const hello[];
extern const char *const hits[];
extern const char *const intro1[];
extern const char *const intro2[];
extern const char *const lastch[];
extern const char *const list[];
extern int maxmoves;
extern const char *const moves[];
extern const char *const opts;
extern const char *const prog[];
extern const char *const prompt;
extern const char *const removepiece[];
extern const char *const stragy[];
extern const struct situatn test[];

int brdeq(const int *, const int *);
void clrest(void);
void leave(void) NORETURN;
void tutor(void) NORETURN;
