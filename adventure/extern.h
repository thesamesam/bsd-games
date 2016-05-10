// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <stdbool.h>
#include <string.h>

// done.c
int score(void);
void done(int) NORETURN;
void die(int);

// init.c
void init(void);
void trapdel(int);
void startup(void);

// io.c
void getin(char **, char **);
bool yes(int, int, int);
bool yesm(int, int, int);
void rdata(void);
void rspeak(int);
void mspeak(int);
void speak(const char*);
void pspeak(int, int);

// save.c
void ciao (void);
int restore (const char*);

// subr.c
int toting(int);
int here(int);
int at(int);
int liq2(int);
int liq(void);
int liqloc(int);
int bitset(int, int);
int forced(int);
int dark(void);
int pct(int);
int fdwarf(void);
int march(void);
int mback(void);
int specials(void);
int trbridge(void);
void badmove(void);
void bug(int) NORETURN;
void checkhints(void);
int trsay(void);
int trtake(void);
int dropper(void);
int trdrop(void);
int tropen(void);
int trkill(void);
int trtoss(void);
int trfeed(void);
int trfill(void);
void closing(void);
void caveclose(void);
int ran(int);

// vocab.c
void dstroy(int);
void juggle(int);
void move(int, int);
int put(int, int, int);
void carry(int, int);
void drop(int, int);
int vocab(const char *, int);

// These three used to be functions in vocab.c
#define copystr(src, dest)	strcpy((dest), (src))
#define weq(str1, str2)		(!strncmp((str1), (str2), 5))

// wizard.c
