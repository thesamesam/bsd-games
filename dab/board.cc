// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

// board.cc: Board manipulations
#include "board.h"
#include "gamescreen.h"
#include "box.h"
#include "player.h"

BOARD::BOARD (size_t y, size_t x, GAMESCREEN* scrn)
:_ny (y)
,_nx (x)
,_scrn (scrn)
{
    _ty = 2 * _ny + 1;
    _tx = 2 * _nx + 1;
    _b = new int*[_ty];
    for (y = 0; y < _ty; y++)
	_b[y] = new int[_tx];
    init();
}

BOARD::BOARD (const BOARD& b)
:_ty (b._ty)
,_tx (b._tx)
,_ny (b._ny)
,_nx (b._nx)
,_scrn (nullptr)
{
    _b = new int*[_ty];
    for (size_t y = 0; y < _ty; ++y) {
	_b[y] = new int[_tx];
	memcpy (_b[y], b._b[y], _tx * sizeof(int));
    }
}

BOARD::~BOARD (void)
{
    for (size_t y = 0; y < _ty; ++y)
	delete[] _b[y];
    delete[] _b;
}

// Clear all boxes and reset state for a new game
void BOARD::init (void)
{
    for (size_t y = 0; y < _ny; ++y) {
	for (size_t x = 0; x < _nx; ++x) {
	    BOX box(y, x, *this);
	    box.reset();
	}
    }
}

// Make a move for player with initial 'c', adding an edge at box(x, y)
// and the specified direction.
// returns:
//	-1:	Invalid move
//	 n:	Number of closures n E [0..2]
int BOARD::domove (size_t y, size_t x, int dir, char c)
{
    int closed = 0;

    // Check if out of bounds
    if (!bounds(y, x))
	return -1;

    BOX box1(y, x, *this);

    // Check if the edge is already there
    if (box1.isset(dir))
	return -1;

    box1.set(dir);

    if (box1.count() == 4) {
	// New box; name it and count it
	box1.name() = c;
	closed++;
    }

    box1.paint();

    // Check other box
    x += BOX::edges[dir].x;
    y += BOX::edges[dir].y;

    if (bounds(y, x)) {
	BOX box2(y, x, *this);
	if (box2.count() == 4) {
	    box2.name() = c;
	    box2.paint();
	    closed++;
	}
    }
    return closed;
}

// Return true if the board is full
int BOARD::full (void) const
{
    for (size_t y = 0; y < _ny; ++y) {
	for (size_t x = 0; x < _nx; ++x) {
	    BOX box(y, x, const_cast<BOARD&>(*this));
	    if (box.count() != 4)
		return 0;
	}
    }
    return 1;
}

// Return if the coordinates are within bounds; we don't check for < 0,
// since size_t is unsigned
int BOARD::bounds (size_t y, size_t x) const
{
    return x < _nx && y < _ny;
}

// Paint all boxes, effectively redrawing the board
void BOARD::paint (void) const
{
    for (size_t y = 0; y < _ny; ++y) {
	for (size_t x = 0; x < _nx; ++x) {
	    BOX box(y, x, const_cast<BOARD&>(*this));
	    box.paint();
	}
    }
}

// Clear the screen
void BOARD::clean (void) const
{
    if (!_scrn)
	return;
    _scrn->clean();
}

// Move cursor to x, y
void BOARD::setpos (size_t y, size_t x) const
{
    if (!_scrn)
	return;
    _scrn->moveto(y, x);
    _scrn->redraw();
}

// Return character indicating move
int BOARD::getmove (void) const
{
    if (!_scrn)
	return 'q';
    _scrn->redraw();
    return _scrn->getinput();
}

// Ring the bell
void BOARD::bell (void) const
{
    if (!_scrn)
	return;
    _scrn->bell();
}

// Post the score in the current game for player i
void BOARD::score (size_t i, const PLAYER& p)
{
    if (_scrn)
	_scrn->score(i, p);
}

// Post the number of games won for player i
void BOARD::games (size_t i, const PLAYER& p)
{
    if (_scrn)
	_scrn->games(i, p);
}

// Post the total score for player i
void BOARD::total (size_t i, const PLAYER& p)
{
    if (_scrn)
	_scrn->total(i, p);
}

// Post the total score for player i
void BOARD::ties (const PLAYER& p)
{
    if (_scrn)
	_scrn->ties(p);
}

// Internal algorithm error; post and abort
void BOARD::abort (const char* s, ...) const
{
    for (size_t i = 0; i < _ny; ++i)
	fprintf(stderr, "\n");
    va_list ap;
    fprintf(stderr, "Algorithm internal error: ");
    va_start(ap, s);
    vfprintf(stderr, s, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    ::abort();
}
