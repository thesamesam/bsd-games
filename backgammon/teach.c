// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "back.h"
#include "tutor.h"

const char *const helpm[] = {
    "\nEnter a space or newline to roll, or",
    "     b   to display the board",
    "     d   to double",
    "     q   to quit\n",
    0
};

const char *const contin[] = {
    "",
    0
};

int main(int argc UNUSED, char **argv)
{
    int i;

    // revoke setgid privileges
    setregid(getgid(), getgid());

    signal(SIGINT, getout);
    if (tcgetattr(0, &old) == -1)	// get old tty mode
	errexit("teachgammon(gtty)");
    noech = old;
    noech.c_lflag &= ~ECHO;
    bg_raw = noech;
    bg_raw.c_lflag &= ~ICANON; // set up modes
    ospeed = cfgetospeed(&old);	// for termlib
    tflag = getcaps(getenv("TERM"));
    while (*++argv != 0)
	getarg(&argv);
    if (tflag) {
	noech.c_oflag &= ~ONLCR;
	bg_raw.c_oflag &= ~ONLCR;
	clear();
    }
    text(hello);
    text(list);
    i = text(contin);
    if (i == 0)
	i = 2;
    init();
    while (i)
	switch (i) {
	    case 1:
		leave();

	    case 2:
		if ((i = text(intro1)) != 0)
		    break;
		wrboard();
		if ((i = text(intro2)) != 0)
		    break;

	    case 3:
		if ((i = text(moves)) != 0)
		    break;

	    case 4:
		if ((i = text(removepiece)) != 0)
		    break;

	    case 5:
		if ((i = text(hits)) != 0)
		    break;

	    case 6:
		if ((i = text(endgame)) != 0)
		    break;

	    case 7:
		if ((i = text(doubl)) != 0)
		    break;

	    case 8:
		if ((i = text(stragy)) != 0)
		    break;

	    case 9:
		if ((i = text(prog)) != 0)
		    break;

	    case 10:
		if ((i = text(lastch)) != 0)
		    break;
	}
    tutor();
    // NOTREACHED
    return 0;
}

void leave(void)
{
    if (tflag)
	clear();
    else
	writec('\n');
    fixtty(&old);
    execlp("backgammon", "backgammon", "-n", args[1] ? args : 0, NULL);
    writel("Help! Backgammon program is missing\007!!\n");
    exit(1);
}
