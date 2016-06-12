// Copyright (c) 1994 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "gomoku.h"
#include <curses.h>

enum {
    color_None,
    color_Field,
    color_Human,
    color_Computer,
    color_HumanCursor,
    color_ComputerCursor,
};

extern const char* _plyr[2];

static WINDOW* _w = NULL;

void initialize_field_window (void)
{
    if (_w)
	delwin (_w);
    _w = newwin (BSZ2, BSZ2*2-1, LINES-BSZ2, (min_u(80,COLS)-BSZ2)/2);
    keypad (_w, true);
    init_pair (color_Field,		COLOR_BLACK,	COLOR_BLACK);
    init_pair (color_Human,		COLOR_CYAN,	COLOR_BLACK);
    init_pair (color_Computer,		COLOR_MAGENTA,	COLOR_BLACK);
    init_pair (color_HumanCursor,	COLOR_BLACK,	COLOR_CYAN);
    init_pair (color_ComputerCursor,	COLOR_BLACK,	COLOR_MAGENTA);
    wbkgdset (_w, COLOR_PAIR(color_Field));
}

// Update the board display after a move.
void bdisp (void)
{
    werase (_w);
    wattrset (_w, A_BOLD| COLOR_PAIR(color_Field));
    box (_w, 0, 0);
    wattroff (_w, A_BOLD);
    for (unsigned j = 1; j < BSZ1; ++j) {
	for (unsigned i = 1; i < BSZ1; ++i) {
	    unsigned bo = j*BSZ1+i;
	    unsigned v = _board[bo].s_occ;
	    int color = A_BOLD| COLOR_PAIR(color_Field);
	    if (bo == _lastHumanMove)
		color = COLOR_PAIR(color_HumanCursor);
	    else if (bo == _lastComputerMove)
		color = COLOR_PAIR(color_ComputerCursor);
	    else if (v == _humanPlayer)
		color = COLOR_PAIR(color_Human);
	    else if (v == !_humanPlayer)
		color = COLOR_PAIR(color_Computer);
	    mvwaddch (_w, j, 2*i, color| "XO."[v]);
	}
    }
    wrefresh (_w);
}

void display_game_result_message (unsigned i, bool humanPlayer)
{
    bdisp();
    wmove(_w, getmaxy(_w)-1, 2);
    wattrset (_w, A_BOLD| COLOR_PAIR(color_Computer));
    if (i == WIN)
	waddstr (_w, humanPlayer ? "Rats! you won" : "Ha ha, I won");
    else
	waddstr (_w, "Wow! its a tie");
    wrefresh (_w);
    sleep (2);
    flushinp();
    wgetch (_w);
}

int usermove (void)
{
    for (;;) {
	bdisp();
	wattrset (_w, A_BOLD| COLOR_PAIR(color_Human));
	mvwaddstr (_w, getmaxy(_w)-1, 2, "Your move");
	unsigned lhy = _lastHumanMove/BSZ1, lhx = _lastHumanMove%BSZ1;
	flushinp();
	int c = wgetch(_w);
	if (c == 'q' || c == KEY_F(10))
	    return RESIGN;
	else if (c == KEY_RESIZE)
	    initialize_field_window();
	else if ((c == 'h' || c == KEY_LEFT) && lhx > 1)	// borders are part of the board
	    --_lastHumanMove;
	else if ((c == 'l' || c == KEY_RIGHT) && lhx < BSZ1-1)
	    ++_lastHumanMove;
	else if ((c == 'k' || c == KEY_UP) && lhy > 1)
	    _lastHumanMove -= BSZ1;
	else if ((c == 'j' || c == KEY_DOWN) && lhy < BSZ1-1)
	    _lastHumanMove += BSZ1;
	else if ((c == ' ' || c == '\n' || c == KEY_ENTER) && _board[_lastHumanMove].s_occ == EMPTY)
	    return _lastHumanMove;
    }
}
