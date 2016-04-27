// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// board.h: Board functions

#pragma once
#include <stdlib.h>

class GAMESCREEN;
class PLAYER;

class BOARD {
public:
		BOARD (size_t y, size_t x, GAMESCREEN* scrn);	// For the main screen
		BOARD (const BOARD& b);				// For scratch screens
		~BOARD();

    inline size_t	nx (void) const		{ return _nx; }
    inline size_t	ny (void) const		{ return _ny; }
    inline size_t	tx (void) const		{ return _tx; }
    inline size_t	ty (void) const		{ return _ty; }
    inline GAMESCREEN*	getScrn (void) const	{ return _scrn; }
    inline int&		data (size_t y, size_t x)	{ return _b[y][x]; }

    // Computing
    int		domove (size_t y, size_t x, int dir, char c);	// Play move
    void	init (void);					// Initialize a new game
    int		full (void) const;				// True if no more moves
    int		bounds (size_t y, size_t x) const;		// True if in bounds

    // Screen updates
    void	paint (void) const;				// Redraw screen
    void	clean (void) const;				// Clear screen
    void	setpos (size_t y, size_t x) const;		// move cursor to pos
    int		getmove (void) const;				// Return move
    void	bell (void) const;				// Beep!
    void	score (size_t i, const PLAYER& p);		// Post score
    void	games (size_t i, const PLAYER& p);		// Post games
    void	total (size_t i, const PLAYER& p);		// Post totals
    void	ties (const PLAYER& p);				// Post ties
    void	abort (const char *s, ...) const;		// Algorithm error
private:
    size_t	_ty, _tx;	// number of symbols in x and y dimension
    size_t	_ny, _nx;	// number of boxes in the x and y dimension
    int**	_b;		// board array of symbols
    GAMESCREEN*	_scrn;		// screen access, if we have one
};
