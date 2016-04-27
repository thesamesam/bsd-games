// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"

void playgame(void)
{
    getword();
    Errors = 0;
    bool* bp = Guessed;
    while (bp < &Guessed[26])
	*bp++ = false;
    while (Errors < MAXERRS && strchr(Known, '-') != NULL) {
	prword();
	prdata();
	prman();
	getguess();
    }
    endgame();
}
