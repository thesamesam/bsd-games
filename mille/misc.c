// Copyright (c) 1982 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "mille.h"
#ifndef	unctrl
#include "unctrl.h"
#endif
#include <sys/file.h>
#include <termios.h>

enum { NUMSAFE = 4 };

bool error (const char *str, ...)
{
    va_list ap;
    va_start(ap, str);
    wmove(Score, ERR_Y, ERR_X);
    vwprintw(Score, str, ap);
    wclrtoeol(Score);
    putchar('\07');
    refresh();
    va_end(ap);
    return false;
}

CARD getcard (void)
{
    int c, c1;
    for (;;) {
	while ((c = readch()) == '\n' || c == '\r' || c == ' ')
	    continue;
	if (islower(c))
	    c = toupper(c);
	if (c == killchar() || c == erasechar())
	    return -1;
	addstr(unctrl(c));
	clrtoeol();
	switch (c) {
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
		c -= '0';
		break;
	    case '0':
	    case 'P':
	    case 'p':
		c = 0;
		break;
	    default:
		putchar('\07');
		addch('\b');
		if (!isprint(c))
		    addch('\b');
		c = -1;
		break;
	}
	refresh();
	if (c >= 0) {
	    while ((c1 = readch()) != '\r' && c1 != '\n' && c1 != ' ')
		if (c1 == killchar())
		    return -1;
		else if (c1 == erasechar()) {
		    addch('\b');
		    clrtoeol();
		    refresh();
		    goto cont;
		} else
		    write(0, "\07", 1);
	    return c;
	}
      cont:;
    }
}

int check_ext (bool forcomp)
{
    if (End == 700)
	if (Play == PLAYER) {
	    if (getyn(EXTENSIONPROMPT)) {
	      extend:
		if (!forcomp)
		    End = 1000;
		return true;
	    } else {
	      done:
		if (!forcomp)
		    Finished = true;
		return false;
	    }
	} else {
	    PLAY *pp, *op;
	    int i, safe, miles;

	    pp = &Player[COMP];
	    op = &Player[PLAYER];
	    for (safe = 0, i = 0; i < NUMSAFE; i++)
		if (pp->safety[i] != S_UNKNOWN)
		    safe++;
	    if (safe < 2)
		goto done;
	    if (op->mileage == 0 || onecard(op)
		|| (op->can_go && op->mileage >= 500))
		goto done;
	    for (miles = 0, i = 0; i < NUMSAFE; i++)
		if (op->safety[i] != S_PLAYED && pp->safety[i] == S_UNKNOWN)
		    miles++;
	    if (miles + safe == NUMSAFE)
		goto extend;
	    for (miles = 0, i = 0; i < HAND_SZ; i++)
		if ((safe = pp->hand[i]) <= C_200)
		    miles += Value[safe];
	    if (miles + (Topcard - Deck) * 3 > 1000)
		goto extend;
	    goto done;
    } else
	goto done;
}

// Get a yes or no answer to the given question.  Saves are
// also allowed.  Return true if the answer was yes, false if no.
int getyn (int promptno)
{
    char c;
    Saved = false;
    for (;;) {
	leaveok(Board, false);
	prompt(promptno);
	clrtoeol();
	refresh();
	switch (c = readch()) {
	    case 'n':
	    case 'N':
		addch('N');
		refresh();
		leaveok(Board, true);
		return false;
	    case 'y':
	    case 'Y':
		addch('Y');
		refresh();
		leaveok(Board, true);
		return true;
	    case 's':
	    case 'S':
		addch('S');
		refresh();
		Saved = save();
		continue;
	    case CTRL('L'):
		wrefresh(curscr);
		break;
	    default:
		addstr(unctrl(c));
		refresh();
		putchar('\07');
		break;
	}
    }
}

// Check to see if more games are desired.  If not, and game
// came from a saved file, make sure that they don't want to restore
// it.  Exit appropriately.
void check_more(void)
{
    On_exit = true;
    if (Player[PLAYER].total >= 5000 || Player[COMP].total >= 5000)
	if (getyn(ANOTHERGAMEPROMPT))
	    return;
	else {
	    // must do accounting normally done in main()
	    if (Player[PLAYER].total > Player[COMP].total)
		Player[PLAYER].games++;
	    else if (Player[PLAYER].total < Player[COMP].total)
		Player[COMP].games++;
	    Player[COMP].total = 0;
	    Player[PLAYER].total = 0;
    } else if (getyn(ANOTHERHANDPROMPT))
	return;
    if (!Saved && getyn(SAVEGAMEPROMPT))
	if (!save())
	    return;
    die (EXIT_SUCCESS);
}

int readch (void)
{
    static char c;
    for (int cnt = 0; read(0, &c, 1) <= 0; ++cnt)
	if (cnt > 100)
	    exit (EXIT_FAILURE);
    return c;
}
