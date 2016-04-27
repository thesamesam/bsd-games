// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum {
    MINLEN	= 6,
    MAXERRS	= 7,
    MESGY	= 12,
    MESGX	= 0,
    PROMPTY	= 11,
    PROMPTX	= 0,
    KNOWNY	= 10,
    KNOWNX	= 1,
    NUMBERY	= 4,
    AVGY	= 5,
    GUESSY	= 2
};

#define NUMBERX	(COLS - 1 - 26)
#define AVGX	(COLS - 1 - 26)
#define GUESSX	(COLS - 1 - 26)

typedef struct {
    short y, x;
    char ch;
} ERR_POS;

extern bool Guessed[];

extern char Word[], Known[];
extern const char *const Noose_pict[];

extern int Errors, Wordnum;
extern unsigned int Minlen;

extern double Average;

extern const ERR_POS Err_pos[];

extern const char *Dict_name;

extern FILE *Dict;

extern off_t Dict_size;

void die(int) NORETURN;
void endgame(void);
int main(int, char *[]);
void getguess(void);
void getword(void);
void playgame(void);
void prdata(void);
void prman(void);
void prword(void);
int readch(void);
void setup(void);
