// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "deck.h"
#include "cribbage.h"
#include "cribcur.h"
#include "pathnames.h"
#include <curses.h>
#include <err.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    bool playing;
    FILE *f;
    int ch;
    int fd;
    int flags;

    f = fopen(_PATH_LOG, "a");
    if (f == NULL)
	warn("fopen %s", _PATH_LOG);
    if (f != NULL && fileno(f) < 3)
	exit(1);

    // Revoke setgid privileges
    setregid(getgid(), getgid());

    // Set close-on-exec flag on log file
    if (f != NULL) {
	fd = fileno(f);
	flags = fcntl(fd, F_GETFD);
	if (flags < 0)
	    err(1, "fcntl F_GETFD");
	flags |= FD_CLOEXEC;
	if (fcntl(fd, F_SETFD, flags) == -1)
	    err(1, "fcntl F_SETFD");
    }

    while ((ch = getopt(argc, argv, "eqr")) != -1) {
	switch (ch) {
	    case 'e': explain = true; break;
	    case 'q': quiet = true; break;
	    case 'r': rflag = true; break;
	    case '?':
	    default:
		(void) fprintf(stderr, "usage: cribbage [-eqr]\n");
		exit(1);
	}
    }

    initscr();
    (void) signal(SIGINT, receive_intr);
    cbreak();
    noecho();

    Playwin = subwin(stdscr, PLAY_Y, PLAY_X, 0, 0);
    Tablewin = subwin(stdscr, TABLE_Y, TABLE_X, 0, PLAY_X);
    Compwin = subwin(stdscr, COMP_Y, COMP_X, 0, TABLE_X + PLAY_X);
    Msgwin = subwin(stdscr, MSG_Y, MSG_X, Y_MSG_START, SCORE_X + 1);
    leaveok(Playwin, true);
    leaveok(Tablewin, true);
    leaveok(Compwin, true);
    clearok(stdscr, false);

    if (!quiet) {
	msg("Do you need instructions for cribbage? ");
	if (getuchar() == 'Y') {
	    endwin();
	    clear();
	    mvcur(0, COLS - 1, LINES - 1, 0);
	    fflush(stdout);
	    instructions();
	    cbreak();
	    noecho();
	    clear();
	    refresh();
	    msg("For cribbage rules, use \"man cribbage\"");
	}
    }
    playing = true;
    do {
	wclrtobot(Msgwin);
	msg(quiet ? "L or S? " : "Long (to 121) or Short (to 61)? ");
	if (glimit == SGAME)
	    glimit = (getuchar() == 'L' ? LGAME : SGAME);
	else
	    glimit = (getuchar() == 'S' ? SGAME : LGAME);
	game();
	msg("Another game? ");
	playing = (getuchar() == 'Y');
    } while (playing);

    if (f != NULL) {
	(void) fprintf(f, "%s: won %5.5d, lost %5.5d\n", getlogin(), cgames, pgames);
	(void) fclose(f);
    }
    bye();
    exit(0);
}

// makeboard: Print out the initial board on the screen
void makeboard(void)
{
    mvaddstr(SCORE_Y + 0, SCORE_X, "+---------------------------------------+");
    mvaddstr(SCORE_Y + 1, SCORE_X, "|  Score:   0     YOU                   |");
    mvaddstr(SCORE_Y + 2, SCORE_X, "| *.....:.....:.....:.....:.....:.....  |");
    mvaddstr(SCORE_Y + 3, SCORE_X, "| *.....:.....:.....:.....:.....:.....  |");
    mvaddstr(SCORE_Y + 4, SCORE_X, "|                                       |");
    mvaddstr(SCORE_Y + 5, SCORE_X, "| *.....:.....:.....:.....:.....:.....  |");
    mvaddstr(SCORE_Y + 6, SCORE_X, "| *.....:.....:.....:.....:.....:.....  |");
    mvaddstr(SCORE_Y + 7, SCORE_X, "|  Score:   0      ME                   |");
    mvaddstr(SCORE_Y + 8, SCORE_X, "+---------------------------------------+");
    gamescore();
}

// gamescore: Print out the current game score
void gamescore(void)
{
    if (pgames || cgames) {
	mvprintw(SCORE_Y + 1, SCORE_X + 28, "Games: %3d", pgames);
	mvprintw(SCORE_Y + 7, SCORE_X + 28, "Games: %3d", cgames);
    }
    Lastscore[0] = -1;
    Lastscore[1] = -1;
}

// Play one game up to glimit points. Actually, we only ASK the
// player what card to turn. We do a random one, anyway.
void game(void)
{
    int i, j;
    bool flag;
    bool compcrib;

    compcrib = false;
    makedeck(deck);
    shuffle(deck);
    if (gamecount == 0) {
	flag = true;
	do {
	    if (!rflag) {      // player cuts deck
		msg(quiet ? "Cut for crib? " : "Cut to see whose crib it is -- low card wins? ");
		cribbage_getline();
	    }
	    i = (rand() >> 4) % CARDS;	// random cut
	    do {	       // comp cuts deck
		j = (rand() >> 4) % CARDS;
	    } while (j == i);
	    addmsg(quiet ? "You cut " : "You cut the ");
	    msgcard(deck[i], false);
	    endmsg();
	    addmsg(quiet ? "I cut " : "I cut the ");
	    msgcard(deck[j], false);
	    endmsg();
	    flag = (deck[i].rank == deck[j].rank);
	    if (flag) {
		msg(quiet ? "We tied..." : "We tied and have to try again...");
		shuffle(deck);
		continue;
	    } else
		compcrib = (deck[i].rank > deck[j].rank);
	} while (flag);
	do_wait();
	clear();
	makeboard();
	refresh();
    } else {
	makeboard();
	refresh();
	werase(Tablewin);
	wrefresh(Tablewin);
	werase(Compwin);
	wrefresh(Compwin);
	msg("Loser (%s) gets first crib", (iwon ? "you" : "me"));
	compcrib = !iwon;
    }

    pscore = cscore = 0;
    flag = true;
    do {
	shuffle(deck);
	flag = !playhand(compcrib);
	compcrib = !compcrib;
    } while (flag);
    ++gamecount;
    if (cscore < pscore) {
	if (glimit - cscore > 60) {
	    msg("YOU DOUBLE SKUNKED ME!");
	    pgames += 4;
	} else if (glimit - cscore > 30) {
	    msg("YOU SKUNKED ME!");
	    pgames += 2;
	} else {
	    msg("YOU WON!");
	    ++pgames;
	}
	iwon = false;
    } else {
	if (glimit - pscore > 60) {
	    msg("I DOUBLE SKUNKED YOU!");
	    cgames += 4;
	} else if (glimit - pscore > 30) {
	    msg("I SKUNKED YOU!");
	    cgames += 2;
	} else {
	    msg("I WON!");
	    ++cgames;
	}
	iwon = true;
    }
    gamescore();
}

// Do up one hand of the game
int playhand (bool mycrib)
{
    werase(Compwin);
    wrefresh(Compwin);
    werase(Tablewin);
    wrefresh(Tablewin);

    knownum = 0;
    int deckpos = deal(mycrib);
    sorthand(chand, FULLHAND);
    sorthand(phand, FULLHAND);
    makeknown(chand, FULLHAND);
    prhand(phand, FULLHAND, Playwin, false);
    discard(mycrib);
    if (cut(mycrib, deckpos))
	return true;
    if (peg(mycrib))
	return true;
    werase(Tablewin);
    wrefresh(Tablewin);
    if (score(mycrib))
	return true;
    return false;
}

// deal cards to both players from deck
int deal (bool mycrib)
{
    int j = 0;
    for (int i = 0; i < FULLHAND; ++i) {
	if (mycrib) {
	    phand[i] = deck[j++];
	    chand[i] = deck[j++];
	} else {
	    chand[i] = deck[j++];
	    phand[i] = deck[j++];
	}
    }
    return j;
}

// Handle players discarding into the crib...
// Note: we call cdiscard() after prining first message so player doesn't wait
void discard (bool mycrib)
{
    const char *prompt;
    CARD crd;

    prcrib(mycrib, true);
    prompt = (quiet ? "Discard --> " : "Discard a card --> ");
    cdiscard(mycrib);	       // puts best discard at end
    crd = phand[infrom(phand, FULLHAND, prompt)];
    cremove(crd, phand, FULLHAND);
    prhand(phand, FULLHAND, Playwin, false);
    crib[0] = crd;

    // Next four lines same as last four except for cdiscard().
    crd = phand[infrom(phand, FULLHAND - 1, prompt)];
    cremove(crd, phand, FULLHAND - 1);
    prhand(phand, FULLHAND, Playwin, false);
    crib[1] = crd;
    crib[2] = chand[4];
    crib[3] = chand[5];
    chand[4].rank = chand[4].suit = chand[5].rank = chand[5].suit = EMPTY;
}

// Cut the deck and set turnover.  Actually, we only ASK the
// player what card to turn.  We do a random one, anyway.
int cut (bool mycrib, int pos)
{
    int i;
    bool win = false;
    if (mycrib) {
	if (!rflag) {	       // random cut
	    msg(quiet ? "Cut the deck? " : "How many cards down do you wish to cut the deck? ");
	    cribbage_getline();
	}
	i = (rand() >> 4) % (CARDS - pos);
	turnover = deck[i + pos];
	addmsg(quiet ? "You cut " : "You cut the ");
	msgcard(turnover, false);
	endmsg();
	if (turnover.rank == JACK) {
	    msg("I get two for his heels");
	    win = chkscr(&cscore, 2);
	}
    } else {
	i = (rand() >> 4) % (CARDS - pos) + pos;
	turnover = deck[i];
	addmsg(quiet ? "I cut " : "I cut the ");
	msgcard(turnover, false);
	endmsg();
	if (turnover.rank == JACK) {
	    msg("You get two for his heels");
	    win = chkscr(&pscore, 2);
	}
    }
    makeknown(&turnover, 1);
    prcrib(mycrib, false);
    return win;
}

// Print out the turnover card with crib indicator
void prcrib (bool mycrib, bool blank)
{
    int y, cardx;

    if (mycrib)
	cardx = CRIB_X;
    else
	cardx = 0;

    mvaddstr(CRIB_Y, cardx + 1, "CRIB");
    prcard(stdscr, CRIB_Y + 1, cardx, turnover, blank);

    if (mycrib)
	cardx = 0;
    else
	cardx = CRIB_X;

    for (y = CRIB_Y; y <= CRIB_Y + 5; y++)
	mvaddstr(y, cardx, "       ");
    refresh();
}

// Handle all the pegging...
static CARD Table[14];
static int Tcnt;

int peg (bool mycrib)
{
    static CARD ch[CINHAND], ph[CINHAND];
    int i, j, k;
    int l;
    int cnum, pnum, sum;
    bool myturn, mego, ugo, last, played;
    CARD crd;

    played = false;
    cnum = pnum = CINHAND;
    for (i = 0; i < CINHAND; ++i) {	// make copies of hands
	ch[i] = chand[i];
	ph[i] = phand[i];
    }
    Tcnt = 0;		       // index to table of cards played
    sum = 0;		       // sum of cards played
    mego = ugo = false;
    myturn = !mycrib;
    for (;;) {
	last = true;	       // enable last flag
	prhand(ph, pnum, Playwin, false);
	prhand(ch, cnum, Compwin, true);
	prtable(sum);
	if (myturn) {	       // my tyrn to play
	    if (!anymove(ch, cnum, sum)) {	// if no card to play
		if (!mego && cnum) {	// go for comp?
		    msg("GO");
		    mego = true;
		}
		// can player move?
		if (anymove(ph, pnum, sum))
		    myturn = !myturn;
		else {	       // give him his point
		    msg(quiet ? "You get one" : "You get one point");
		    do_wait();
		    if (chkscr(&pscore, 1))
			return true;
		    sum = 0;
		    mego = ugo = false;
		    Tcnt = 0;
		}
	    } else {
		played = true;
		j = -1;
		k = 0;
		// maximize score
		for (i = 0; i < cnum; ++i) {
		    l = pegscore(ch[i], Table, Tcnt, sum);
		    if (l > k) {
			k = l;
			j = i;
		    }
		}
		if (j < 0)     // if nothing scores
		    j = cchose(ch, cnum, sum);
		crd = ch[j];
		cremove(crd, ch, cnum--);
		sum += VAL(crd.rank);
		Table[Tcnt++] = crd;
		if (k > 0) {
		    addmsg(quiet ? "I get %d playing " : "I get %d points playing ", k);
		    msgcard(crd, false);
		    endmsg();
		    if (chkscr(&cscore, k))
			return true;
		}
		myturn = !myturn;
	    }
	} else {
	    if (!anymove(ph, pnum, sum)) {	// can player move?
		if (!ugo && pnum) {	// go for player
		    msg("You have a GO");
		    ugo = true;
		}
		// can computer play?
		if (anymove(ch, cnum, sum))
		    myturn = !myturn;
		else {
		    msg(quiet ? "I get one" : "I get one point");
		    do_wait();
		    if (chkscr(&cscore, 1))
			return true;
		    sum = 0;
		    mego = ugo = false;
		    Tcnt = 0;
		}
	    } else {	       // player plays
		played = false;
		if (pnum == 1) {
		    crd = ph[0];
		    msg("You play your last card");
		} else
		    for (;;) {
			prhand(ph, pnum, Playwin, false);
			crd = ph[infrom(ph, pnum, "Your play: ")];
			if (sum + VAL(crd.rank) <= 31)
			    break;
			else
			    msg("Total > 31 -- try again");
		    }
		makeknown(&crd, 1);
		cremove(crd, ph, pnum--);
		i = pegscore(crd, Table, Tcnt, sum);
		sum += VAL(crd.rank);
		Table[Tcnt++] = crd;
		if (i > 0) {
		    msg(quiet ? "You got %d" : "You got %d points", i);
		    if (pnum == 0)
			do_wait();
		    if (chkscr(&pscore, i))
			return true;
		}
		myturn = !myturn;
	    }
	}
	if (sum >= 31) {
	    if (!myturn)
		do_wait();
	    sum = 0;
	    mego = ugo = false;
	    Tcnt = 0;
	    last = false;      // disable last flag
	}
	if (!pnum && !cnum)
	    break;	       // both done
    }
    prhand(ph, pnum, Playwin, false);
    prhand(ch, cnum, Compwin, true);
    prtable(sum);
    if (last) {
	if (played) {
	    msg(quiet ? "I get one for last" : "I get one point for last");
	    do_wait();
	    if (chkscr(&cscore, 1))
		return true;
	} else {
	    msg(quiet ? "You get one for last" : "You get one point for last");
	    do_wait();
	    if (chkscr(&pscore, 1))
		return true;
	}
    }
    return false;
}

// Print out the table with the current score
void prtable(int score)
{
    prhand(Table, Tcnt, Tablewin, false);
    mvwprintw(Tablewin, (Tcnt + 2) * 2, Tcnt + 1, "%2d", score);
    wrefresh(Tablewin);
}

// Handle the scoring of the hands
int score (bool mycrib)
{
    sorthand(crib, CINHAND);
    if (mycrib) {
	if (plyrhand(phand, "hand"))
	    return true;
	if (comphand(chand, "hand"))
	    return true;
	do_wait();
	if (comphand(crib, "crib"))
	    return true;
	do_wait();
    } else {
	if (comphand(chand, "hand"))
	    return true;
	if (plyrhand(phand, "hand"))
	    return true;
	if (plyrhand(crib, "crib"))
	    return true;
    }
    return false;
}
