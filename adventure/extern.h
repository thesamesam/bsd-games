// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <string.h>

// crc.c
void crc_start(void);
unsigned long crc(const char*, int);

// done.c
int score(void);
void done(int) NORETURN;
void die(int);

// init.c
void init(void);
char* decr(int, int, int, int, int);
void linkdata(void);
void trapdel(int);
void startup(void);

// io.c
void getin(char **, char **);
int yes(int, int, int);
int yesm(int, int, int);
int next(void);
void rdata(void);
int rnum(void);
void rdesc(int);
void rtrav(void);
#ifndef NDEBUG
void twrite(int);
#endif
void rvoc(void);
void rlocs(void);
void rdflt(void);
void rliq(void);
void rhints(void);
void rspeak(int);
void mspeak(int);
struct text;
void speak(const struct text*);
void pspeak(int, int);

// save.c
int save(const char*);
int restore(const char*);

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

// vocab.c
void dstroy(int);
void juggle(int);
void move(int, int);
int put(int, int, int);
void carry(int, int);
void drop(int, int);
int vocab(const char *, int, int);

// These three used to be functions in vocab.c
#define copystr(src, dest)	strcpy((dest), (src))
#define weq(str1, str2)		(!strncmp((str1), (str2), 5))
#define length(str)		(strlen((str)) + 1)

void prht(void);

// wizard.c
void datime(int *, int *);
void poof(void);
int Start(void);
int wizard(void);
void ciao(void);
int ran(int);
