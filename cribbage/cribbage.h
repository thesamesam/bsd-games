// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <stdbool.h>
#include <curses.h>

extern CARD deck[CARDS];	// a deck
extern CARD phand[FULLHAND];	// player's hand
extern CARD chand[FULLHAND];	// computer's hand
extern CARD crib[CINHAND];	// the crib
extern CARD turnover;		// the starter

extern CARD known[CARDS];	// cards we have seen
extern int knownum;		// # of cards we know

extern int pscore;		// player's score
extern int cscore;		// comp's score
extern int glimit;		// points to win game

extern int pgames;		// player's games won
extern int cgames;		// comp's games won
extern int gamecount;		// # games played
extern int Lastscore[2];	// previous score for each

extern bool iwon;		// if comp won last
extern bool explain;		// player mistakes explained
extern bool rflag;		// if all cuts random
extern bool quiet;		// if suppress random mess
extern bool playing;		// currently playing game

extern char explan[];		// string for explanation

void addmsg(const char *, ...) PRINTFLIKE(1,2);
int adjust(const CARD[], CARD);
int anymove(const CARD[], int, int);
int anysumto(const CARD[], int, int, int);
void bye(void);
int cchose(const CARD[], int, int);
void cdiscard(bool);
int chkscr(int *, int);
int comphand(const CARD[], const char *);
void cremove(CARD, CARD[], int);
int cut(bool, int);
int deal(bool);
void discard(bool);
void do_wait(void);
void endmsg(void);
int eq(CARD, CARD);
int fifteens(const CARD[], int);
void game(void);
void gamescore(void);
char *cribbage_getline(void);
int getuchar(void);
int incard(CARD *);
int infrom(const CARD[], int, const char *);
void instructions(void);
int is_one(CARD, const CARD[], int);
void makeboard(void);
void makedeck(CARD[]);
void makeknown(const CARD[], int);
void msg(const char *, ...) PRINTFLIKE(1,2);
int msgcard(CARD, bool);
int msgcrd(CARD, bool, const char *, bool);
int number(int, int, const char *);
int numofval(const CARD[], int, int);
int pairuns(const CARD[], int);
int peg(bool);
int pegscore(CARD, const CARD[], int, int);
int playhand(bool);
int plyrhand(const CARD[], const char *);
void prcard(WINDOW *, int, int, CARD, bool);
void prcrib(bool, bool);
void prhand(const CARD[], int, WINDOW *, bool);
void printcard(WINDOW *, int, CARD, bool);
void prpeg(int, int, bool);
void prtable(int);
int readchar(void);
void receive_intr(int) NORETURN;
int score(bool);
int scorehand(const CARD[], CARD, int, bool, bool);
void shuffle(CARD[]);
void sorthand(CARD[], int);
void wait_for(int);
