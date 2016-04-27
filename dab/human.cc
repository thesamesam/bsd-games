// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

// human.cc: Human interface for dots, using rogue-like keys.
#include "human.h"
#include "board.h"
#include "box.h"

#define CONTROL(a) ((a) & 037)

HUMAN::HUMAN (const char c)
: PLAYER(c)
,_curx(0)
,_cury(1)
{
}

void HUMAN::play (const BOARD& b, size_t& y, size_t& x, int& dir)
{
    b.setpos (_cury, _curx);
    for (int mv;;) {
	switch (mv = b.getmove()) {
	case 'h': case 'H':
	    _curx -= 2;
	    break;

	case 'l': case 'L':
	    _curx += 2;
	    break;

	case 'k': case 'K':
	    _cury -= 2;
	    break;

	case 'j': case 'J':
	    _cury += 2;
	    break;

	case 'u': case 'U':
	    _curx += 1;
	    _cury -= 1;
	    break;

	case 'y': case 'Y':
	    _curx -= 1;
	    _cury -= 1;
	    break;

	case 'b': case 'B':
	    _curx -= 1;
	    _cury += 1;
	    break;

	case 'n': case 'N':
	    _curx += 1;
	    _cury += 1;
	    break;

	case 'q': case 'Q':
	    exit(0);

	case CONTROL('L'): case CONTROL('R'):
	    b.clean();
	    b.paint();
	    break;

	case ' ':
	    {
		x = _curx / 2;
		y = _cury / 2;

		if (_cury & 1) {
		    if (_curx == 0)
			dir = BOX::left;
		    else {
			x--;
			dir = BOX::right;
		    }
		}

		if (_curx & 1) {
		    if (_cury == 0)
			dir = BOX::top;
		    else {
			y--;
			dir = BOX::bottom;
		    }
		}
	    }
	    return;

	default:
	    break;
	}

        // We add 2 before the comparison to avoid underflow
	if ((2 + _curx) - (_curx & 1) < 2)
	    _curx = (b.nx() * 2) + (_curx & 1);
	if (_curx >= (b.nx() * 2) + 1)
	    _curx = (_curx & 1);

	if ((2 + _cury) - (_cury & 1) < 2)
	    _cury = (b.ny() * 2) + (_cury & 1);
	if (_cury >= (b.ny() * 2) + 1)
	    _cury = (_cury & 1);

	b.setpos(_cury, _curx);
    }
}
