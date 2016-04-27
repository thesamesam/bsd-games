// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <termios.h>

// variable initialization.

int pnum = 2;			// color of player: -1 = white 1 = red 0 =
				// both 2 = not yet init'ed
int acnt = 1;			// length of args
char args[100] = "-";
int aflag = 1;			// flag to ask for rules or instructions
int bflag = 0;			// flag for automatic board printing
int cflag = 0;			// case conversion flag
int hflag = 1;			// flag for cleaning screen
int mflag = 0;			// backgammon flag
int raflag = 0;			// 'roll again' flag for recovered game
int rflag = 0;			// recovered game flag
int tflag = 0;			// cursor addressing flag
int iroll = 0;			// special flag for inputting rolls
int rfl = 0;

const char *const color[] = { "White", "Red", "white", "red" };

const char *const *Colorptr;
const char *const *colorptr;
int *inopp;
int *inptr;
int *offopp;
int *offptr;
char args[100];
int bar;
int begscr;
int board[26];
char cin[100];
int colen;
int cturn;
int curc;
int curr;
int d0;
int dice[2];
int dlast;
int g[5];
int gvalue;
int h[4];
int home;
int in[2];
int mvl;
int mvlim;
int ncin;
int off[2];
int p[5];
int rscore;
int table[6][6];
int wscore;
struct termios old, noech, bg_raw;
