// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"
#include <err.h>

// This game written by Ken Arnold.
int main (int argc, char *argv[])
{
    // Revoke setgid privileges
    setregid(getgid(), getgid());
    for (int ch; (ch = getopt(argc, argv, "d:m:")) != -1;) {
	switch (ch) {
	    case 'd':
		Dict_name = optarg;
		break;
	    case 'm':
		Minlen = atoi(optarg);
		if (Minlen < 2)
		    errx(1, "minimum word length too short");
		break;
	    case '?':
	    default:
		fprintf(stderr, "usage: hangman [-d wordlist] [-m minlen]\n");
		return EXIT_FAILURE;
	}
    }
    initscr();
    signal(SIGINT, die);
    setup();
    for (;;) {
	Wordnum++;
	playgame();
	Average = (Average * (Wordnum - 1) + Errors) / Wordnum;
    }
    return EXIT_SUCCESS;
}

// Die properly.
void die (int dummy UNUSED)
{
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
    putchar('\n');
    exit(0);
}
