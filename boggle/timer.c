// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include "bog.h"
#include <sys/param.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <curses.h>
#include <setjmp.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static int waitch(int);

extern int tlimit;
extern time_t start_t;
extern jmp_buf env;

// Update the display of the remaining time while waiting for a character
// If time runs out do a longjmp() to the game controlling routine, returning
// non-zero; oth. return the character
// Leave the cursor where it was initially
int timerch(void)
{
    time_t prevt, t;
    int col, remaining, row;

    getyx(stdscr, row, col);
    prevt = 0L;
    for (;;) {
	if (waitch(1) == 1)
	    break;
	time(&t);
	if (t == prevt)
	    continue;
	prevt = t;
	remaining = tlimit - (int) (t - start_t);
	if (remaining < 0) {
	    longjmp(env, 1);
	 /*NOTREACHED*/}
	move(TIMER_LINE, TIMER_COL);
	printw("%d:%02d", remaining / 60, remaining % 60);
	move(row, col);
	refresh();
    }
    return getch() & 0177;
}

// Wait up to 'delay' microseconds for input to appear
// Returns 1 if input is ready, 0 oth.
static int waitch(int delay)
{
    struct pollfd set[1];
    set[0].fd = STDIN_FILENO;
    set[0].events = POLLIN;
    return poll(set, 1, delay);
}

void delay(int tenths)
{
    struct timespec duration;
    duration.tv_nsec = (tenths % 10) * 100000000L;
    duration.tv_sec = (long) (tenths / 10);
    nanosleep(&duration, NULL);
}
