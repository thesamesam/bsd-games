// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"

// Print out the current guesses
void prdata(void)
{
    move(GUESSY, GUESSX + sizeof "Guessed: ");
    bool* bp = Guessed;
    while (bp < &Guessed[26])
	if (*bp++)
	    addch((bp - Guessed) + 'a' - 1);
    clrtoeol();
    mvprintw(NUMBERY, NUMBERX + sizeof "Word #:          ", "%d", Wordnum);
    mvprintw(AVGY, AVGX + sizeof "Current Average: ", "%.3f", (Average * (Wordnum - 1) + Errors) / Wordnum);
    mvprintw(AVGY + 1, AVGX + sizeof "Overall Average: ", "%.3f", Average);
}
