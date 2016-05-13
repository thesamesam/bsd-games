// Copyright (c) 1982 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "mille.h"
#include <signal.h>
#include <time.h>

int main (int ac, char** av)
{
    bool restore;

    // Revoke setgid privileges
    setregid(getgid(), getgid());

    if (strcmp(av[0], "a.out") == 0) {
	outf = fopen("q", "w");
	setbuf(outf, (char *) NULL);
	Debug = true;
    }
    restore = false;
    switch (ac) {
	case 2:
	    rest_f(av[1]);
	    restore = true;
	case 1:
	    break;
	default:
	    printf("usage: milles [ restore_file ]\n");
	    return EXIT_FAILURE;
    }
    Play = PLAYER;
    initscr();
    delwin(stdscr);
    stdscr = Board = newwin(BOARD_Y, BOARD_X, 0, 0);
    Score = newwin(SCORE_Y, SCORE_X, 0, 40);
    Miles = newwin(MILES_Y, MILES_X, 17, 0);
#ifdef attron
    idlok(Board, true);
    idlok(Score, true);
    idlok(Miles, true);
#endif
    leaveok(Score, true);
    leaveok(Miles, true);
    clearok(curscr, true);
    srandrand();
    cbreak();
    noecho();
    signal(SIGINT, rub);
    for (;;) {
	if (!restore || (Player[PLAYER].total >= 5000 || Player[COMP].total >= 5000)) {
	    if (Player[COMP].total < Player[PLAYER].total)
		Player[PLAYER].games++;
	    else if (Player[COMP].total > Player[PLAYER].total)
		Player[COMP].games++;
	    Player[COMP].total = 0;
	    Player[PLAYER].total = 0;
	}
	do {
	    if (!restore)
		Handstart = Play = other(Handstart);
	    if (!restore || On_exit) {
		shuffle();
		init();
	    }
	    newboard();
	    if (restore)
		mvwaddstr(Score, ERR_Y, ERR_X, Initstr);
	    prboard();
	    do {
		domove();
		if (Finished)
		    newscore();
		prboard();
	    } while (!Finished);
	    check_more();
	    restore = On_exit = false;
	} while (Player[COMP].total < 5000 && Player[PLAYER].total < 5000);
    }
    return EXIT_SUCCESS;
}

// Routine to trap rubouts, and make sure they really want to quit.
void rub (int dummy UNUSED)
{
    signal(SIGINT, SIG_IGN);
    if (getyn (REALLYPROMPT))
	die (EXIT_SUCCESS);
    signal(SIGINT, rub);
}

void die (int code)
{
    signal (SIGINT, SIG_IGN);
    if (outf)
	fflush(outf);
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
    exit (code);
}
