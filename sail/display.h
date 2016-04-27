// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <curses.h>

#ifdef SIGTSTP
#define SCREENTEST()	(initscr() != NULL && signal(SIGTSTP, SIG_DFL) != SIG_ERR && STAT_R < COLS && SCROLL_Y > 0)
#else
#define SCREENTEST()	(initscr() != NULL && STAT_R < COLS && SCROLL_Y > 0)
#endif

extern WINDOW *view_w;
extern WINDOW *slot_w;
extern WINDOW *scroll_w;
extern WINDOW *stat_w;
extern WINDOW *turn_w;
