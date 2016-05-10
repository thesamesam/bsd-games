// Copyright (c) 1994 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "gomoku.h"
#include <curses.h>

enum {
    SCRNW = 80,	// assume 80 chars for the moment
    SCRNH = 24	// assume 24 lines for the moment
};

static int lastline;
static char pcolor[] = "*O.?";

extern int interactive;
extern char *plyr[];

// Initialize screen display.
void cursinit (void)
{
    initscr();
    noecho();
    cbreak();
    leaveok (stdscr, true);
}

// Restore screen display.
void cursfini (void)
{
    leaveok (stdscr, false);
    move (23, 0);
    clrtoeol();
    refresh();
    endwin();
}

// Initialize board display.
void bdisp_init (void)
{
    // top border
    for (int i = 1; i < BSZ1; ++i) {
	move(0, 2 * i + 1);
	addch(letters[i]);
    }
    // left and right edges
    for (int j = BSZ1; --j > 0;) {
	move(20 - j, 0);
	printw("%2d ", j);
	move(20 - j, 2 * BSZ1 + 1);
	printw("%d ", j);
    }
    // bottom border
    for (int i = 1; i < BSZ1; ++i) {
	move(20, 2 * i + 1);
	addch(letters[i]);
    }
    bdwho(0);
    move(0, 47);
    addstr("#  black  white");
    lastline = 0;
    bdisp();
}

// Update who is playing whom.
void bdwho (int update)
{
    move (21, 0);
    clrtoeol();
    int i = 6 - strlen(plyr[BLACK]) / 2;
    move(21, i > 0 ? i : 0);
    printw ("BLACK/%s", plyr[BLACK]);
    i = 30 - strlen(plyr[WHITE]) / 2;
    move (21, i);
    printw("WHITE/%s", plyr[WHITE]);
    move (21, 19);
    addstr (" vs. ");
    if (update)
	refresh();
}

// Update the board display after a move.
void bdisp (void)
{
    for (int j = BSZ1; --j > 0;) {
	for (int i = 1; i < BSZ1; ++i) {
	    move(BSZ1 - j, 2 * i + 1);
	    struct spotstr* sp = &board[i + j * BSZ1];
	    int c = pcolor[sp->s_occ];
	    if (debug > 1 && sp->s_occ == EMPTY) {
		if (sp->s_flg & IFLAGALL)
		    c = '+';
		else if (sp->s_flg & CFLAGALL)
		    c = '-';
		else
		    c = '.';
	    }
	    addch(c);
	}
    }
    refresh();
}

#ifndef NDEBUG
// Dump board display to a file.
void bdump (FILE* fp)
{
    // top border
    fprintf (fp, "   A B C D E F G H J K L M N O P Q R S T\n");
    for (int j = BSZ1; --j > 0;) {
	// left edge
	fprintf (fp, "%2d ", j);
	for (int i = 1; i < BSZ1; ++i) {
	    struct spotstr *sp = &board[i + j * BSZ1];
	    int c = pcolor[sp->s_occ];
	    if (debug > 1 && sp->s_occ == EMPTY) {
		if (sp->s_flg & IFLAGALL)
		    c = '+';
		else if (sp->s_flg & CFLAGALL)
		    c = '-';
		else
		    c = '.';
	    }
	    putc (c, fp);
	    putc (' ', fp);
	}
	// right edge
	fprintf (fp, "%d\n", j);
    }
    // bottom border
    fprintf (fp, "   A B C D E F G H J K L M N O P Q R S T\n");
}
#endif				// DEBUG

// Display a transcript entry
void dislog (const char *str)
{
    if (++lastline >= SCRNH - 1) // move 'em up
	lastline = 1;
    move (lastline, 46);
    addnstr (str, SCRNW - 46 - 1);
    clrtoeol();
    move (lastline + 1, 46);
    clrtoeol();
}

// Display a question.
void ask (const char *str)
{
    mvaddstr (23, 0, str);
    clrtoeol();
    move (23, strlen(str));
    refresh();
}

int gomoku_getline (char *buf, int size)
{
    int c = 0;
    char* cp = buf;
    char* end = buf + size - 1;      // save room for the '\0'
    while (cp < end && (c = getchar()) != EOF && c != '\n' && c != '\r') {
	*cp++ = c;
	if (interactive) {
	    switch (c) {
		case 0x0c:    // ^L
		    wrefresh(curscr);
		    cp--;
		    continue;
		case 0x15:    // ^U
		case 0x18:    // ^X
		    while (cp > buf) {
			cp--;
			addch('\b');
		    }
		    clrtoeol();
		    break;
		case '\b':
		case 0x7f:    // DEL
		    if (cp == buf + 1) {
			cp--;
			continue;
		    }
		    cp -= 2;
		    addch('\b');
		    c = ' ';
		    // FALLTHROUGH
		default:
		    addch(c);
	    }
	    refresh();
	}
    }
    *cp = '\0';
    return c != EOF;
}
