// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
// Worm.  Written by Michael Toy UCSC

#include "../config.h"
#include <curses.h>
#include <signal.h>

enum {
    HEAD	= '@',
    BODY	= 'o',
    LENGTH	= 7,
    RUNLEN	= 8,
    XTIMER	= 120,
    YTIMER	= XTIMER*5/3,
    STOPTIMER	= -1
};

static WINDOW* tv = NULL;
static WINDOW* stw = NULL;

static struct body {
    unsigned x;
    unsigned y;
    struct body *prev;
    struct body *next;
} *head, *tail, goody;

static unsigned growing = 0;
static unsigned score = 0;
static unsigned start_len = LENGTH;
static unsigned visible_len = 0;

static void crash (void);
static void display (const struct body *, int);
static void leave (int);
static void life (void);
static void newpos (struct body *);
static void play (void);
static void prize (void);

int main (int argc, char **argv)
{
    srandrand();
    static const uint8_t c_FatalSignals[] = {
	SIGINT, SIGQUIT, SIGTERM, SIGILL, SIGBUS,
	SIGABRT, SIGFPE, SIGSYS, SIGSEGV, SIGHUP
    };
    for (unsigned i = 0; i < ArraySize(c_FatalSignals); ++i)
	signal(c_FatalSignals[i], leave);
    signal(SIGTSTP, SIG_IGN);
    if (!initscr()) {
	printf ("Error: unable to initialize terminal graphics\n");
	return EXIT_FAILURE;
    }
    cbreak();
    noecho();
    keypad(stdscr, true);
    clear();
    if (argc == 2)
	start_len = atoi(argv[1]);
    if (!start_len || start_len > ((LINES - 3) * (COLS - 2)) / 3u)
	start_len = LENGTH;
    stw = newwin (1, COLS - 1, 0, 0);
    tv = newwin (LINES - 1, COLS - 1, 1, 0);
    box (tv, 0, 0);
    scrollok(tv, false);
    scrollok(stw, false);
    wmove(stw, 0, 0);
    wprintw(stw, " Worm");
    refresh();
    wrefresh(stw);
    wrefresh(tv);
    life();		       // Create the worm
    prize();		       // Put up a goal
    play();
    endwin();
    printf("Your final score before quitting was %u\n", score);
    return EXIT_SUCCESS;
}

static struct body* newlink (void)
{
    struct body* l = (struct body *) malloc (sizeof (struct body));
    if (!l) {
	perror ("malloc");
	exit (EXIT_FAILURE);
    }
    return l;
}

static void life (void)
{
    head = newlink();
    head->x = start_len % (COLS - 5) + 2;
    head->y = LINES / 2;
    head->next = NULL;
    display(head, HEAD);
    struct body* bp = head;
    struct body* np = NULL;
    int j = 1;
    for (unsigned i = 0; i < start_len; ++i, bp = np) {
	np = newlink();
	np->next = bp;
	bp->prev = np;
	if ((bp->x <= 2 && j == 1) || (bp->x >= COLS - 4u && j == -1)) {
	    j *= -1;
	    np->x = bp->x;
	    np->y = bp->y + 1;
	} else {
	    np->x = bp->x - j;
	    np->y = bp->y;
	}
	display (np, BODY);
    }
    tail = np;
    tail->prev = NULL;
    visible_len = start_len + 1;
}

static void display(const struct body *pos, int chr)
{
    mvwaddch (tv, pos->y, pos->x, chr);
}

static void newpos (struct body *bp)
{
    if (visible_len == (LINES - 3) * (COLS - 3) - 1u) {
	endwin();
	printf("\nYou won!\nYour final score was %u\n\n", score);
	exit (EXIT_SUCCESS);
    }
    do {
	bp->y = nrand (LINES - 3) + 1;
	bp->x = nrand (COLS - 3) + 1;
	wmove(tv, bp->y, bp->x);
    } while (winch(tv) != ' ');
}

static void play (void)
{
    timeout (STOPTIMER);
    for (int lastch = 0;;) {
	int ch = getch();
	if (ch == ERR)
	    ch = lastch;
	unsigned x = head->x, y = head->y;
	switch (ch) {
	    case KEY_LEFT:
	    case 'h':	--x; timeout (XTIMER); break;
	    case KEY_RIGHT:
	    case 'l':	++x; timeout (XTIMER); break;
	    case KEY_UP:
	    case 'k':	--y; timeout (YTIMER); break;
	    case KEY_DOWN:
	    case 'j':	++y; timeout (YTIMER); break;
	    case 'q':	return;
	    default:	ch = lastch; break;
	}
	lastch = ch;
	if (x == head->x && y == head->y)
	    continue;
	if (growing == 0) {
	    display(tail, ' ');
	    tail->next->prev = NULL;
	    struct body* nh = tail->next;
	    free(tail);
	    tail = nh;
	    visible_len--;
	} else
	    growing--;
	display(head, BODY);
	wmove(tv, y, x);
	ch = winch(tv);
	if (isdigit(ch)) {
	    growing += ch - '0';
	    prize();
	    score += growing;
	    mvwprintw (stw, 0, COLS-12, "Score: %3u", score);
	    wrefresh (stw);
	} else if (ch != ' ')
	    crash();
	struct body* nh = newlink();
	nh->next = NULL;
	nh->prev = head;
	head->next = nh;
	nh->y = y;
	nh->x = x;
	display(nh, HEAD);
	head = nh;
	++visible_len;
	wmove(tv, head->y, head->x);
	wrefresh(tv);
    }
}

static void prize (void)
{
    unsigned value = nrand(9) + 1;
    newpos (&goody);
    waddch (tv, value + '0');
    wrefresh (tv);
}

static void crash (void)
{
    endwin();
    printf("Well, you ran into something and the game is over.\n"
	    "Your final score was %u\n", score);
    exit (EXIT_SUCCESS);
}

static void leave (int signo)
{
    endwin();
    psignal (signo, "Fatal error");
    exit (EXIT_SUCCESS);
}
