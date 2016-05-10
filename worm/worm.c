// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
// Worm.  Written by Michael Toy UCSC

#include "../config.h"
#include <curses.h>
#include <err.h>
#include <signal.h>
#include <termios.h>

#define newlink() (struct body *) malloc(sizeof (struct body));
#define HEAD '@'
#define BODY 'o'
#define LENGTH 7
#define RUNLEN 8
#define CNTRL(p) (p-'A'+1)

static WINDOW *tv;
static WINDOW *stw;
static struct body {
    int x;
    int y;
    struct body *prev;
    struct body *next;
} *head, *tail, goody;
static int growing = 0;
static int running = 0;
static int slow = 0;
static int score = 0;
static int start_len = LENGTH;
static int visible_len;
static int lastch;
static char outbuf[BUFSIZ];

static void crash(void);
static void display(const struct body *, int);
static void leave(int);
static void life(void);
static void newpos(struct body *);
static void process(int);
static void prize(void);
static int rnd(int);
static void setup(void);
static void wake(int);

int main(int argc, char **argv)
{
    // Revoke setgid privileges
    setregid(getgid(), getgid());

    setbuf(stdout, outbuf);
    srand(getpid());
    signal(SIGALRM, wake);
    signal(SIGINT, leave);
    signal(SIGQUIT, leave);
    initscr();
    cbreak();
    noecho();
#ifdef KEY_LEFT
    keypad(stdscr, true);
#endif
    slow = (baudrate() <= 1200);
    clear();
    if (COLS < 18 || LINES < 5) {
	// Insufficient room for the line with " Worm" and the
	// score if fewer than 18 columns; insufficient room for
	// anything much if fewer than 5 lines.
	endwin();
	errx(1, "screen too small");
    }
    if (argc == 2)
	start_len = atoi(argv[1]);
    if ((start_len <= 0) || (start_len > ((LINES - 3) * (COLS - 2)) / 3))
	start_len = LENGTH;
    stw = newwin(1, COLS - 1, 0, 0);
    tv = newwin(LINES - 1, COLS - 1, 1, 0);
    box(tv, '*', '*');
    scrollok(tv, false);
    scrollok(stw, false);
    wmove(stw, 0, 0);
    wprintw(stw, " Worm");
    refresh();
    wrefresh(stw);
    wrefresh(tv);
    life();		       // Create the worm
    prize();		       // Put up a goal
    while (1) {
	if (running) {
	    running--;
	    process(lastch);
	} else {
	    fflush(stdout);
	    process(getch());
	}
    }
    return EXIT_SUCCESS;
}

static void life(void)
{
    struct body *bp, *np;
    int i, j = 1;

    np = NULL;
    head = newlink();
    if (head == NULL)
	err(1, NULL);
    head->x = start_len % (COLS - 5) + 2;
    head->y = LINES / 2;
    head->next = NULL;
    display(head, HEAD);
    for (i = 0, bp = head; i < start_len; i++, bp = np) {
	np = newlink();
	if (np == NULL)
	    err(1, NULL);
	np->next = bp;
	bp->prev = np;
	if (((bp->x <= 2) && (j == 1)) || ((bp->x >= COLS - 4) && (j == -1))) {
	    j *= -1;
	    np->x = bp->x;
	    np->y = bp->y + 1;
	} else {
	    np->x = bp->x - j;
	    np->y = bp->y;
	}
	display(np, BODY);
    }
    tail = np;
    tail->prev = NULL;
    visible_len = start_len + 1;
}

static void display(const struct body *pos, int chr)
{
    wmove(tv, pos->y, pos->x);
    waddch(tv, chr);
}

static void leave(int dummy)
{
    endwin();

    if (dummy == 0) {	       // called via crash()
	printf("\nWell, you ran into something and the game is over.\n");
	printf("Your final score was %d\n\n", score);
    }
    exit(0);
}

static void wake (int dummy UNUSED)
{
    signal (SIGALRM, wake);
    fflush (stdout);
    process (lastch);
}

static int rnd(int range)
{
    return abs((rand() >> 5) + (rand() >> 5)) % range;
}

static void newpos(struct body *bp)
{
    if (visible_len == (LINES - 3) * (COLS - 3) - 1) {
	endwin();

	printf("\nYou won!\n");
	printf("Your final score was %d\n\n", score);
	exit(0);
    }
    do {
	bp->y = rnd(LINES - 3) + 1;
	bp->x = rnd(COLS - 3) + 1;
	wmove(tv, bp->y, bp->x);
    } while (winch(tv) != ' ');
}

static void prize(void)
{
    int value;

    value = rnd(9) + 1;
    newpos(&goody);
    waddch(tv, value + '0');
    wrefresh(tv);
}

static void process(int ch)
{
    int x, y;
    struct body *nh;

    alarm(0);
    x = head->x;
    y = head->y;
    switch (ch) {
#ifdef KEY_LEFT
	case KEY_LEFT:
#endif
	case 'h':
	    x--;
	    break;

#ifdef KEY_DOWN
	case KEY_DOWN:
#endif
	case 'j':
	    y++;
	    break;

#ifdef KEY_UP
	case KEY_UP:
#endif
	case 'k':
	    y--;
	    break;

#ifdef KEY_RIGHT
	case KEY_RIGHT:
#endif
	case 'l':
	    x++;
	    break;

	case 'H':
	    x--;
	    running = RUNLEN;
	    ch = tolower(ch);
	    break;
	case 'J':
	    y++;
	    running = RUNLEN / 2;
	    ch = tolower(ch);
	    break;
	case 'K':
	    y--;
	    running = RUNLEN / 2;
	    ch = tolower(ch);
	    break;
	case 'L':
	    x++;
	    running = RUNLEN;
	    ch = tolower(ch);
	    break;
	case '\f':
	    setup();
	    return;

	case ERR:
	case CNTRL('C'):
	case CNTRL('D'):
	    crash();
	    return;

	default:
	    if (!running)
		alarm(1);
	    return;
    }
    lastch = ch;
    if (growing == 0) {
	display(tail, ' ');
	tail->next->prev = NULL;
	nh = tail->next;
	free(tail);
	tail = nh;
	visible_len--;
    } else
	growing--;
    display(head, BODY);
    wmove(tv, y, x);
    if (isdigit(ch = winch(tv))) {
	growing += ch - '0';
	prize();
	score += growing;
	running = 0;
	wmove(stw, 0, COLS - 12);
	wprintw(stw, "Score: %3d", score);
	wrefresh(stw);
    } else if (ch != ' ')
	crash();
    nh = newlink();
    if (nh == NULL)
	err(1, NULL);
    nh->next = NULL;
    nh->prev = head;
    head->next = nh;
    nh->y = y;
    nh->x = x;
    display(nh, HEAD);
    head = nh;
    visible_len++;
    if (!(slow && running)) {
	wmove(tv, head->y, head->x);
	wrefresh(tv);
    }
    if (!running)
	alarm(1);
}

static void crash(void)
{
    leave(0);
}

static void setup(void)
{
    clear();
    refresh();
    touchwin(stw);
    wrefresh(stw);
    touchwin(tv);
    wrefresh(tv);
    alarm(1);
}
