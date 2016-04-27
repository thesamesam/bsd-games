// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"
#include <err.h>
#include <time.h>
#include <getopt.h>
#include <unistd.h>

// Set up the strings on the screen.
void setup(void)
{
    const char *const *sp;
    static struct stat sbuf;

    noecho();
    cbreak();

    mvaddstr(PROMPTY, PROMPTX, "Guess:");
    mvaddstr(GUESSY, GUESSX, "Guessed:");
    mvaddstr(NUMBERY, NUMBERX, "Word #:");
    mvaddstr(AVGY, AVGX, "Current Average:");
    mvaddstr(AVGY + 1, AVGX, "Overall Average:");
    mvaddstr(KNOWNY, KNOWNX, "Word: ");

    for (sp = Noose_pict; *sp != NULL; sp++) {
	move(sp - Noose_pict, 0);
	addstr(*sp);
    }

    srand(time(NULL) + getpid());
    if ((Dict = fopen(Dict_name, "r")) == NULL) {
	endwin();
	err(1, "fopen %s", Dict_name);
    }
    fstat(fileno(Dict), &sbuf);
    Dict_size = sbuf.st_size;
}
