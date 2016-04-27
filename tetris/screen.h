// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"

// Capabilities from TERMCAP (used in the score code).
extern char *SEstr;		// end standout mode
extern char *SOstr;		// begin standout mode

// putpad() is for padded strings with count=1.
#define	putpad(s)	tputs(s, 1, put)

int put(int);			// just calls putchar; for tputs
void scr_clear(void);
void scr_end(void);
void scr_init(void);
void scr_msg(char *, int);
void scr_set(void);
void scr_update(void);
