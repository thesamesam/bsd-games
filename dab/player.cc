// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

// player.cc: Player base class

#include "board.h"
#include "player.h"

PLAYER::PLAYER (char who)
:_who(who)
,_score(0)
,_total(0)
,_games(0)
,_ties(0)
{
}

void PLAYER::init (void)
{
    _score = 0;
}

void PLAYER::wl (size_t sc)
{
    _total += _score;
    _games += sc < _score;
    _ties += sc == _score;
}

int PLAYER::domove (BOARD& b)
{
    size_t y, x;
    int dir;
    int score;

    for (;;) {
	if (b.full())
	    return 0;
	play(b, y, x, dir);
	switch (score = b.domove(y, x, dir, _who)) {
	    case 0:  return 1;		// No closure
	    case -1: b.bell(); break;	// Not a valid move
	    default: _score += score;	// Closure, play again
	}
    }
}
