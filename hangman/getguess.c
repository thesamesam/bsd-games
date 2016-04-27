// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <sys/ttydefaults.h>
#include "hangman.h"
#ifndef CTRL
#define CTRL(x)	((x) & 037)
#endif

// Get another guess
void getguess(void)
{
    int i;
    int ch;
    bool correct;

    leaveok(stdscr, false);
    for (;;) {
	move(PROMPTY, PROMPTX + sizeof "Guess: ");
	refresh();
	ch = readch();
	if (isalpha(ch)) {
	    if (isupper(ch))
		ch = tolower(ch);
	    if (Guessed[ch - 'a'])
		mvprintw(MESGY, MESGX, "Already guessed '%c'", ch);
	    else
		break;
	} else if (ch == CTRL('D'))
	    die(0);
	else
	    mvprintw(MESGY, MESGX, "Not a valid guess: '%s'", unctrl(ch));
    }
    leaveok(stdscr, true);
    move(MESGY, MESGX);
    clrtoeol();

    Guessed[ch - 'a'] = true;
    correct = false;
    for (i = 0; Word[i] != '\0'; i++)
	if (Word[i] == ch) {
	    Known[i] = ch;
	    correct = true;
	}
    if (!correct)
	Errors++;
}

// Read a character from the input
int readch(void)
{
    for (char ch, cnt = 0;;) {
	if (read(0, &ch, sizeof ch) <= 0) {
	    if (++cnt > 100)
		die(0);
	} else if (ch == CTRL('L')) {
	    wrefresh(curscr);
	} else
	    return ch;
    }
}
