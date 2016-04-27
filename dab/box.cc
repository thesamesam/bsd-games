// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

// box.cc: Box computations
#include "box.h"
#include "board.h"
#include "gamescreen.h"
#include <curses.h>

const POINT BOX::edges[BOX::last] =
    { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };
const POINT BOX::corners[BOX::last] =
    { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
const int  BOX::syms[BOX::last] =
    { GAMESCREEN::GS_HLINE, GAMESCREEN::GS_HLINE,
      GAMESCREEN::GS_VLINE, GAMESCREEN::GS_VLINE };

BOX::BOX(size_t py, size_t px, BOARD& b)
:_b (b)
{
    _centery = py * 2 + 1;
    _centerx = px * 2 + 1;
}

void BOX::addcorner (size_t y, size_t x)
{
    char sym;
    _b.getScrn()->moveto(y, x);
    if (x == 0) {
	if (y == 0)
	    sym = GAMESCREEN::GS_ULCORNER;
	else if (y == _b.ty() - 1)
	    sym = GAMESCREEN::GS_LLCORNER;
	else
	    sym = GAMESCREEN::GS_LTEE;
    } else if (x == _b.tx() - 1) {
	if (y == 0)
	    sym = GAMESCREEN::GS_URCORNER;
	else if (y == _b.ty() - 1)
	    sym = GAMESCREEN::GS_LRCORNER;
	else
	    sym = GAMESCREEN::GS_RTEE;
    } else if (y == 0)
	sym = GAMESCREEN::GS_TTEE;
    else if (y == _b.ty() - 1)
	sym = GAMESCREEN::GS_BTEE;
    else
	sym = GAMESCREEN::GS_PLUS;

    _b.getScrn()->addedge(sym);
}

// Paint a box
void BOX::paint (void)
{
    if (!_b.getScrn())
	return;

    _b.getScrn()->moveto(_centery, _centerx);
    _b.getScrn()->addsym(name());

    for (int e = BOX::first; e < BOX::last; e++) {
	addcorner(_centery + corners[e].y, _centerx + corners[e].x);
	_b.getScrn()->moveto(_centery + edges[e].y, _centerx + edges[e].x);
	_b.getScrn()->addedge(edge((EDGE) e));
    }
    _b.getScrn()->redraw();
}

// Return the name
int& BOX::name (void)
{
    return _b.data(_centery, _centerx);
}

// Set an edge
void BOX::set (int e)
{
    _b.data(_centery + edges[e].y, _centerx + edges[e].x) = syms[e];
}

// Clear an edge
void BOX::clr (int e)
{
    _b.data(_centery + edges[e].y, _centerx + edges[e].x) = ' ';
}

// Test an edge
int BOX::isset (int e) const
{
    return _b.data(_centery + edges[e].y, _centerx + edges[e].x) != ' ';
}

// Return the edge
int& BOX::edge (int e)
{
    return _b.data(_centery + edges[e].y, _centerx + edges[e].x);
}

// Count the number of edges set in the box
int BOX::count (void) const
{
    int cnt = 0;
    for (int e = BOX::first; e < BOX::last; e++)
	cnt += isset((EDGE) e);
    return cnt;
}

// Clear the box
void BOX::reset (void)
{
    for (int e = BOX::first; e < BOX::last; e++)
	clr((EDGE) e);
    name() = ' ';
} 
