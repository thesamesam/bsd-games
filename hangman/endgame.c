// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"

// endgame: do what's necessary at the end of the game
void endgame(void)
{
    prman();
    if (Errors >= MAXERRS)
	Errors = MAXERRS + 2;
    prword();
    prdata();
    move(MESGY, MESGX);
    if (Errors > MAXERRS)
	printw("Sorry, the word was \"%s\"\n", Word);
    else
	printw("You got it!\n");

    for (;;) {
	mvaddstr(MESGY + 1, MESGX, "Another word? ");
	leaveok(stdscr, false);
	refresh();
	char ch = readch();
	if (ch == 'n')
	    die(0);
	else if (ch == 'y')
	    break;
	mvaddstr(MESGY + 2, MESGX, "Please type 'y' or 'n'");
    }

    leaveok(stdscr, true);
    move(MESGY, MESGX);
    deleteln();
    deleteln();
    deleteln();
}
