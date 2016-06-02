// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include <curses.h>

//----------------------------------------------------------------------

int ourclock = 120;		// time for all the flights in the game

//----------------------------------------------------------------------

enum {
    c_NStars = 64,
    c_CylonMoveTime = 500
};

static int _cylonLine = 0;
static int _cylonCol = 0;
static int _dr = 0, _dc = 0;
static uint64_t _cylonNextMove = 0;
static bool _destroyed = false;
static struct { uint8_t l, c; } _stars [c_NStars] = {{0,0}};

static WINDOW* _w = NULL;

//----------------------------------------------------------------------

static void blast(void);
static void create_space_window (void);

//----------------------------------------------------------------------

static void create_space_window (void)
{
    if (_w)
	mvwin (_w, LINES-24, 0);
    else {
	_w = newwin (24, 80, LINES-24, 0);
	keypad (_w, true);
    }
}

static void draw_space_window (void)
{
    werase (_w);
    for (unsigned i = 0; i < ArraySize(_stars); ++i)
	mvwaddch (_w, _stars[i].l, _stars[i].c, '.');
    mvwaddstr (_w, getmaxy(_w)/2, getmaxx(_w)/2 - 10, "-------   +   -------");
    for (unsigned n = getmaxy(_w)/2-4; n < getmaxy(_w)/2-1u; ++n) {
	mvwaddch (_w, n, getmaxx(_w)/2, '|');
	mvwaddch (_w, n+6, getmaxx(_w)/2, '|');
    }
    mvwaddstr (_w, _cylonLine, _cylonCol - 1, "/-\\"); // Cylon ship
    mvwprintw (_w, getmaxy(_w)-1, 21, "TORPEDOES: %3d  FUEL: %3d  TIME: %3d", torps, fuel, ourclock);
}

int visual (void)
{
    _destroyed = false;
    create_space_window();
    cbreak();
    noecho();
    curs_set (0);
    _cylonLine = nrand (getmaxy(_w)-3)+1;
    _cylonCol = nrand (getmaxx(_w)-2)+1;
    for (unsigned i = 0; i < ArraySize(_stars); ++i) {
	_stars[i].l = nrand(getmaxy(_w));
	_stars[i].c = nrand(getmaxx(_w));
    }
    for (;;) {
	uint64_t now = time_ms();
	if (now >= _cylonNextMove) {
	    if (_cylonLine+_dr <= getmaxy(_w)-3 && _cylonLine+_dr > 0)
		_cylonLine += _dr;
	    if (_cylonCol+_dc < getmaxx(_w)-1 && _cylonCol+_dc > 0)
		_cylonCol += _dc;
	    --ourclock;
	    _cylonNextMove = now + c_CylonMoveTime;
	}
	draw_space_window();
	wtimeout (_w, _cylonNextMove - now);
	int k = wgetch(_w);
	if (k == 'h' || k == KEY_LEFT) {
	    _dc = -1;
	    --fuel;
	} else if (k == 'l' || k == KEY_RIGHT) {
	    _dc = 1;
	    --fuel;
	} else if (k == 'j' || k == KEY_DOWN) {
	    _dr = 1;
	    --fuel;
	} else if (k == 'k' || k == KEY_UP) {
	    _dr = -1;
	    --fuel;
	} else if (k == ' ' || k == 'f') {
	    if (torps) {
		torps -= 2;
		blast();
		if (_cylonLine == getmaxy(_w)/2 && _cylonCol - getmaxx(_w)/2 < 2 && getmaxx(_w)/2 - _cylonCol < 2) {
		    _destroyed = true;
		    sleep (1);
		    break;
		}
	    } else {
		mvwaddstr (_w, 0, 0, "*** Out of torpedoes. ***");
		sleep (1);
		break;
	    }
	} else if (k == 'q') {
	    endwin();
	    return 0;
	} else if (k == KEY_RESIZE)
	    create_space_window();
	if (fuel < 0) {
	    fuel = 0;
	    mvwaddstr (_w, 0, 60, "*** Out of fuel ***");
	    sleep (1);
	    break;
	}
	if (ourclock <= 0) {
	    endwin();
	    die();
	}
    }
    endwin();
    return _destroyed;
}

static void blast(void)
{
    // Torpedo animation, flying torpedoes
    mvwprintw (_w, getmaxy(_w)-1, 24, "%3d", torps);
    for (unsigned n = getmaxy(_w)-1-2; n >= getmaxy(_w)/2+1u; --n) {
	mvwaddch (_w, n, getmaxx(_w)/2 + getmaxy(_w)/2 - n, '/');
	mvwaddch (_w, n, getmaxx(_w)/2 - getmaxy(_w)/2 + n, '\\');
	wrefresh (_w);
	usleep (50000);
    }
    mvwaddch (_w, getmaxy(_w)/2, getmaxx(_w)/2, '*');
    // Erase torpedo trail
    for (unsigned n = getmaxy(_w)-1-2; n >= getmaxy(_w)/2+1u; --n) {
	mvwaddch (_w, n, getmaxx(_w)/2 + getmaxy(_w)/2 - n, ' ');
	mvwaddch (_w, n, getmaxx(_w)/2 - getmaxy(_w)/2 + n, ' ');
	wrefresh (_w);
	usleep (50000);
    }
}
